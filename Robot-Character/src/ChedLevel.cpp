#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>
#include <ChibiEngine/UserInterface/ImagePicker.h>
#include <ChibiEngine/UserInterface/InputField.h>
#include <ChibiEngine/UserInterface/Checkbox.h>
#include <ChibiEngine/UserInterface/FieldContainer.h>
#include <ChibiEngine/UserInterface/DropDownListBox.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <ChibiEngine/SkeletonAnimation/Animation.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <UI/FieldContainers.h>

#include "ChedPhysBox.h"
#include <ChedLevel.h>
#include <ChedBone.h>
#include <Input/ChedInputListener.h>
#include "ChedSlot.h"
#include <UI/AnimationControl.h>

#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/UserInterface/LineContainer.h>
#include "Generated/ChedSettingsParser.h"
#include "Generated/SaveParser.h"
#include <ChibiEngine/UserInterface/ContainerSelector.h>
#include <ChibiEngine/UserInterface/LabelsList.h>

using namespace game;
using namespace std;
using namespace glm;
using namespace rapidxml;

static const float MAX_Z = 10.5;
static const float MIN_Z = 0.05;
static const float MAX_CAM_HW = 36;
static const float MAX_CAM_HH = 23;
static const string SAVE_FILE_PATH = "ched/saved.dat";

static const string SAV_PROP_BAR = "properties";
static const string SAV_DRAW_BAR = "draw";
static const string SAV_TARG_BAR = "targets";
static const string SAV_ANIM = "workTarget";
static const string SAV_DRAW_BONES = "drawBones";
static const string SAV_SELECT_BONES = "selectBones";
static const string SAV_DRAW_PICS = "drawPics";
static const string SAV_SELECT_PICS = "selectPics";
static const string SAV_DRAW_PHYS = "drawBoxes";
static const string SAV_SELECT_PHYS = "selectBoxes";

inline LabelsList* getEventList(){
    return static_cast<LabelsList*>(Game::getUserInterface()->findElement("eventList"));
}

inline void fadeBackground(bool isFaded){
	Game::getUserInterface()->findElement("backDiv")->setVisible(isFaded);
}

inline void refreshEventList(Animation* a){
	if(a==nullptr){
		return;
	}
	LabelsList* ll = getEventList();
	ll->clear();
//	ll->add(a->getEventList());
}

ChedLevel::ChedLevel(const std::string& worldFile)
	:inputListener(make_unique<ChedInputListener>(this)),
	 hovered(nullptr),
	 selected(nullptr),
     action("move"),
     mode(Mode::ANIM),
	 worldFile(worldFile),
     drawBones(true),
     drawPics(true),
     drawBoxes(true),
     selectBones(true),
     selectPics(true),
     selectBoxes(true),
	 picContainer(new PicContainer(this)),
	 boneContainer(new BoneContainer(this)),
	 boxContainer(new BoxContainer(this)),
	 grid(MAX_CAM_HW,MAX_CAM_HH),
    log(Game::getLoggingSystem()->createLogger("ChedLevel")){

	UserInterface* ui = Game::getUserInterface();
	imagePicker = dynamic_cast<ImagePicker*>(ui->findElement("image_picker"));

	Game::getInputSystem()->addRawListener(inputListener.get());
	ui->addEventListener("menu.file",bind(&ChedLevel::onMenuFile, this, placeholders::_1));
	ui->addEventListener("menu.animation",bind(&ChedLevel::onMenuAnimation, this, placeholders::_1));
	ui->addEventListener("menu.slot",bind(&ChedLevel::onMenuSlot, this, placeholders::_1));
	ui->addEventListener("menu.view",bind(&ChedLevel::onMenuView, this, placeholders::_1));
	ui->addEventListener("menu.instrument",[this](const std::string& name){
		if(name=="move"){
			animationControl->selectTransform(MoveTransform);
		}else if(name=="rotate"){
			animationControl->selectTransform(RotateTransform);
		}else if(name=="scale"){
			animationControl->selectTransform(ScaleTransform);
		}
	});

	ui->addEventListener("image_picker.select",bind(&ChedLevel::onSelectPresent, this, placeholders::_1));
	ui->addEventListener("new_slot_image.select",bind(&ChedLevel::onSelectSlotPresent, this, placeholders::_1));
	ui->addEventListener("slot_images.select",bind(&ChedLevel::onSelectSlot, this, placeholders::_1));

    // Input name dialog
    ui->addEventListener("cancelName",[this](const string& name){
        Game::getUserInterface()->findElement("input_name_dialog")->setVisible(false);
        fadeBackground(false);
    });
    ui->addEventListener("okName",[this](const string& name){
        Game::getUserInterface()->findElement("input_name_dialog")->setVisible(false);
        fadeBackground(false);
        currentInputNameCallback(Game::getUserInterface()->findElement("new_name")->getValue());
    });
    // ------- enter event name
    ui->addEventListener("add2EventList",[this](const string& name){
        showInputNameDialog("Enter event name", [this](const string& name){});
    });

	ui->addEventListener("setAnimDDLB",bind(&ChedLevel::onSelectAnimation, this, placeholders::_1));

	ui->addEventListener("drawPics",[this](const string& name){
        drawPics=name=="true";
    });
    ui->addEventListener("drawBones",[this](const string& name){
        drawBones=name=="true";
    });
    ui->addEventListener("drawBoxes",[this](const string& name){
        drawBoxes=name=="true";
    });
    ui->addEventListener("selectPics",[this](const string& name){
        selectPics=name=="true";
    });
    ui->addEventListener("selectBones",[this](const string& name){
        selectBones=name=="true";
    });
    ui->addEventListener("selectBoxes",[this](const string& name){
        selectBoxes=name=="true";
    });
	ui->addEventListener("playbackMultiplier",[this](const string& name){
        animationControl->setPlaybackMultiplier(toFloat(name));
    });
	ui->addEventListener(SAV_ANIM,[this](const string& name){
		onSelectWorkTarget(name);
    });
	ui->addEventListener("charName",[this](const string& name){
        Game::switchGameState(name);
    });

	load();
    loadSavedSettings(Game::getLevelClock());
	setFirstAnimation();
    skeleton->addTimeEvents([this](const AnimationTimeEvent& e){
        for(auto& bone: bones){
            bone->skeletonChanged(this);
        }
	    for(auto& slot : slots){
			slot->skeletonChanged(this);
		}
    });
}

AnimationControl * ChedLevel::getAnimationControl(){
	return animationControl;
}

void ChedLevel::reName(const std::string& newName){
	ChedBone * bone = dynamic_cast<ChedBone *>(selected);
	if(bone==nullptr)
		return;

	indexedBones.erase(bone->getName());
	bone->setName(newName);
	indexedBones[bone->getName()]=bone;

}

void ChedLevel::refreshAllObjects(){
	for(auto& bone : bones){
		bone->skeletonChanged(this);
	}
}

vector<ChedSlot*> ChedLevel::findSlotByBone(Animation::BoneId boneId){
	vector<ChedSlot*> result;
	for(auto& slot : slots){
		if(slot->getSkeletonSlot()->getJoint()->getId() == boneId){
			result.push_back(slot.get());
		}
	}

	return result;
}

void ChedLevel::selectBone(const glm::vec2& pnt){
    ChedObject* obj = findObject(pnt);
	if(obj)
        reselect(obj);
	hideMenus();
}

void ChedLevel::addPresentation(){
    Game::getUserInterface()->findElement("image_dialog")->setVisible(true);
}

void ChedLevel::deleteBone(){
	if(selected==nullptr)
		return;
	if(selected==hovered)
		hovered=nullptr;

	{
		ChedBone * bone = dynamic_cast<ChedBone *>(selected);
		if(bone!=nullptr){
			//invalidate UI
			boneContainer->link(nullptr);

			// create list of bones tha will be deleted
			std::set<Animation::BoneId> children;
			skeleton->findBoneChildren(children, bone->getId());

			// delete from animation
			for(auto id : children){
				animationControl->deleteBone(indexedByIdBones.find(id)->second);
			}

			//delete from indices
			for (auto iter = indexedBones.begin();iter != indexedBones.end();) {
				if (children.find(iter->second->getId())!=children.end()){
					indexedBones.erase(iter++);
				}else ++iter;
			}
			for(auto id: children){
				indexedByIdBones.erase(id);
			}

			// delete bones slots wrapper
			slots.erase(std::remove_if(slots.begin(), slots.end(),[&children](unique_ptr<ChedSlot>& part){
				return children.find(part->getSkeletonSlot()->getJoint()->getId())!=children.end();
			} ), slots.end());

			// delete bones wrapper
			bones.erase(std::remove_if(bones.begin(), bones.end(),[&children](unique_ptr<ChedBone>& bone){
				return children.find(bone->getId())!=children.end();
			} ), bones.end());

			// remove bones from skeleton
			skeleton->deleteJointWithChildren(bone->getId());

			selected = nullptr;
			return;
		}
	}

	{
		ChedSlot * part = dynamic_cast<ChedSlot *>(selected);
		if(part!=nullptr){
			picContainer->link(nullptr);

			slots.erase(std::remove_if(slots.begin(), slots.end(), [part](const std::unique_ptr<ChedSlot>& _slot){
				return _slot.get()==part;
			} ), slots.end());
			skeleton->deleteSlot(part->getSkeletonSlot());

			selected = nullptr;
			return;
		}
	}
	{
		ChedPhysBox* box = dynamic_cast<ChedPhysBox*>(selected);
		if(box!=nullptr){
			boneContainer->link(nullptr);

			boxes.erase(std::remove_if(boxes.begin(), boxes.end(), [box](unique_ptr<ChedPhysBox>& other){
				return other.get()==box;
			}), boxes.end());

			selected = nullptr;
			return;
		}
	}
}

void ChedLevel::hover(const glm::vec2& pnt){
    instrumentHelper.processHover(pnt);
	if(hovered!=nullptr)
		hovered->setHighlighted(false);

	hovered = findObject(pnt);
	if(hovered!=nullptr){
		hovered->setHighlighted(true);
	}
}

ChedLevel::~ChedLevel(){
    saveSetting();
	//todo implement
	/*while(bones.size()>0){
		for(auto it : bones){
			if(it->getParent()==nullptr){
				bones.erase(std::remove_if(bones.begin(), bones.end(),[](ChedBone* bone){ return bone->isMarked4Delete();} ), bones.end());
				delete it;
				break;
			}
		}
	}*/

/*	for(auto it: animationRef){
		delete it.second;
	}*/
	delete boneContainer;
	delete picContainer;
	delete boxContainer;
}

void ChedLevel::draw(CameraCHandle camera){
	grid.draw();
	if(drawPics){
    	skeleton->draw(camera, Color::White);
		for(auto& slot : slots){
            slot->draw(camera);
		}
	}
	if(drawBones){
		for(auto& it : bones){
			it->draw(camera);
		}
	}
	if(drawBoxes){
		for(auto& box : boxes){
			box->draw(camera);
		}
	}
	inputListener->draw(camera);
    instrumentHelper.draw();
}

void ChedLevel::createBone(const glm::vec2& beg, const glm::vec2& end, const std::string& name, ChedBone* parent){

	SkeletonJoint* joint = new SkeletonJoint(beg,end);
	skeleton->addJoint(unique_ptr<SkeletonJoint>(joint));
	ChedBone * b = new ChedBone(this, skeleton.get(),joint, name);
	bones.push_back(unique_ptr<ChedBone>(b));
	animationControl->addBone(b);

	indexedBones[b->getName()]=b;
	indexedByIdBones[b->getId()]=b;

	if(parent){
		b->getJoint()->setParent(parent->getJoint());
	}
}


void ChedLevel::createPhysBox(const glm::vec2& pos, float hw, float hh){
	ChedPhysBox* created = new ChedPhysBox(this, pos, hw, hh);
	created->setParent(this, dynamic_cast<ChedBone*>(selected));
	boxes.push_back(unique_ptr<ChedPhysBox>(created));
}


ChedObject* ChedLevel::findObject(const glm::vec2& pnt){
	if(drawBones && selectBones){
		for(auto& it : bones){
			if(it->containsPoint(pnt)){
				return it.get();
			}
		}
	}
	if(drawPics && selectPics && mode!=Mode::ANIM){
		for(auto it = slots.rbegin(); it!=slots.rend();++it){
			if((*it)->containsPoint(pnt)){
				return it->get();
			}
		}
	}
	if(drawBoxes && selectBoxes && mode!=Mode::ANIM){
		for(std::vector<unique_ptr<ChedPhysBox>>::reverse_iterator it = boxes.rbegin();
				it!=boxes.rend();++it){
			if((*it)->containsPoint(pnt)){
				return it->get();
			}
		}
	}
	return nullptr;
}

void ChedLevel::reselect(ChedObject* bone){
	if(selected!=nullptr)
		selected->setSelected(false);
	selected=bone;
	if(selected!=nullptr)
		selected->setSelected(true);

	instrumentHelper.attach2Bone(selected);

    ChedSlot* part = dynamic_cast<ChedSlot*>(bone);
	if(part!=nullptr){
		picContainer->link(part);
        Game::getUserInterface()->findContainerSelector("conSelector")->
                activate("pic");
		return;
	}else{
		picContainer->link(nullptr);
	}

	ChedBone * _bone = dynamic_cast<ChedBone *>(bone);
	if(_bone!=nullptr){
		boneContainer->link(_bone);
        Game::getUserInterface()->findContainerSelector("conSelector")->
                activate("bone");
		currentBone = _bone;
		return;
	}else{
		boneContainer->link(nullptr);
		currentBone = nullptr;
	}

	ChedPhysBox* _box = dynamic_cast<ChedPhysBox*>(bone);
	if(_box!=nullptr){
		boxContainer->link(_box);
        Game::getUserInterface()->findContainerSelector("conSelector")->
                activate("box");
		return;
	}else{
		boxContainer->link(nullptr);
	}
}

void ChedLevel::viewContextMenu(const vec2& pnt){
	if(hovered!=nullptr){
		reselect(hovered);
		Game::getUserInterface()->findContextMenu("bone_menu")->viewAt(pnt);
	}else{
		Game::getUserInterface()->findContextMenu("ched_menu")->viewAt(pnt);
	}
}

void ChedLevel::hideMenus(){
	Game::getUserInterface()->findContextMenu("bone_menu")->hide();
	Game::getUserInterface()->findContextMenu("ched_menu")->hide();
}

void ChedLevel::mouseClickUp(){
    instrumentHelper.setMouseDragging(false);
}

void ChedLevel::mouseDrag(const glm::vec2& delta,const glm::vec2& grabPnt, const glm::vec2& currPnt){
	if(selected==nullptr){
		return;
	}
	ChedBone * bone = dynamic_cast<ChedBone *>(selected);

	instrumentHelper.setMouseDragging(true);
	if(mode==Mode::ANIM && bone!=nullptr){
        animationControl->saveTransform(instrumentHelper.constrain(grabPnt, delta), currPnt);
	}else {
		switch(instrumentHelper.getActiveTransform()){
			case MoveTransform:
				selected->move(currPnt, instrumentHelper.constrain(grabPnt, delta));
				break;
			case RotateTransform:
				selected->rotate(currPnt, instrumentHelper.constrain(grabPnt, delta));
				break;
			case ScaleTransform:
				selected->scale(currPnt, instrumentHelper.constrain(grabPnt, delta));
				break;
			case NoTransform:
				break;
		}
	}
}


ChedLevel *ChedLevel::instance(){
	return static_cast<ChedLevel*>(Game::getGame()->getLevel());
}

void ChedLevel::onSelectPresent(const std::string& name){
    Game::getUserInterface()->findElement("image_dialog")->setVisible(false);
    ChedBone * bone=dynamic_cast<ChedBone *>(selected);
    if(bone!=nullptr){

		vec3 pos(0,0,0);
		vec2 size = imagePicker->getDrawScale();
		float rot = 0;
		string name = "";

		// add 2 skeleton
		SkeletonSlot* slot = new SkeletonSlot(bone->getJoint(),"",0,++slotMaxId);
		slot->addPicture(name,imagePicker->getSelected(),pos,size,rot);
		skeleton->addSlot(unique_ptr<SkeletonSlot>(slot));
		// add wrappers
		ChedSlot* chedSlot = new ChedSlot(this, slot,bone);
		chedSlot->addPicMeta("default",pos,size,rot);
		chedSlot->skeletonChanged(this);
		slots.push_back(unique_ptr<ChedSlot>(chedSlot));
        reSort();
    }
}

void ChedLevel::onSelectSlotPresent(const std::string& name){
    UserInterface* ui = Game::getUserInterface();

    ImagePicker* ip = ui->findImagePicker("new_slot_image");
    ChedSlot* slot = dynamic_cast<ChedSlot*>(selected);
    if(slot == nullptr) {
        log.error("ChedSlot not selected");
		return;
    }

    string slotName = ui->findInputField("slot_img_name")->getValue();
    if(slotName.size() == 0) {
        log.error("name not setted");
		return;
    }

	slot->addPic(ip->getSelected(), slotName, ip->getDrawScale());

    reSort();
    ui->findElement("new_slot_image_dialog")->setVisible(false);
}

void ChedLevel::reSort(){
	sort(slots.begin(),slots.end(),[](const std::unique_ptr<ChedSlot>& o1, const std::unique_ptr<ChedSlot>& o2){
		return o1->getSkeletonSlot()->getZOrder()<o2->getSkeletonSlot()->getZOrder();
	});
}

void ChedLevel::onSelectSlot(const std::string& name){
    UserInterface* ui = Game::getUserInterface();
    ChedSlot* slot=dynamic_cast<ChedSlot*>(selected);
    if(slot==nullptr) {
        log.error("ChedSlot not selected");
    }

	SkeletonPicture& pic = slot->getSkeletonSlot()->getPictureByTextureName(ui->findImagePicker("slot_images")->getSelected()->getName());
	slot->getSkeletonSlot()->setActive(pic.getName());
    ui->findElement("slot_images_dialog")->setVisible(false);
}


inline static string findElementName4View(const std::string& name){
    if(name=="Properties"){
        return "properties";
    }else if(name=="Draw Settings"){
        return "draw";
    }else if(name=="Events"){
        return "events";
    }else if(name=="Targets"){
        return "targets";
    }else if(name=="Timeline"){
        return "AnimationControl";
    }
    return "";
}

void ChedLevel::onMenuView(const std::string& name){
    auto elemName = findElementName4View(name);
    if(elemName.size()>0){
        Game::getUserInterface()->findElement(elemName)->setVisible(true);
    }
}

void ChedLevel::onMenuSlot(const std::string& name){

    ChedSlot *slot = dynamic_cast<ChedSlot*>(selected);
    if (slot == nullptr) {
        log.error("ChedSlot not selected");
        return;
    }

    if(name=="add"){
        Game::getUserInterface()->findElement("new_slot_image_dialog")->setVisible(true);
    }else if(name=="slots"){
		ImagePicker *ip = Game::getUserInterface()->findImagePicker("slot_images");
		ip->clearImages();
		ip->addImages(slot->getSlots());
        Game::getUserInterface()->findElement("slot_images_dialog")->setVisible(true);
    }else if(name=="set default"){
    	slot->setActiveSlotDefault();
    }
}

void ChedLevel::onMenuFile(const std::string& name){
	if(name=="save")
		save();
}

void ChedLevel::onMenuAnimation(const std::string& name){
	if(name=="new animation"){
		showInputNameDialog("Enter animation name", [this](const string& text){
			skeleton->createNewAnimation(text);
			Game::getUserInterface()->findDDLB("setAnimDDLB")->add(text);
			Game::getUserInterface()->findDDLB("setAnimDDLB")->setValue(text);
			onSelectAnimation(text);
		});

	}else if(name=="remove animation"){
		if(currentAnimationName!=""){
			skeleton->deleteAnimation(currentAnimationName);
			Game::getUserInterface()->findDDLB("setAnimDDLB")->remove(currentAnimationName);
		}
		setFirstAnimation();
	}
}

void ChedLevel::onSelectAnimation(const std::string& name){
	skeleton->removeAllAnimations();
	skeleton->addAnimation(name, false,false);
	currentAnimationName = name;
	animationControl->setAnimation(name);
	//refreshEventList(animation);
}

void ChedLevel::onSelectWorkTarget(const std::string& name){
	if(name=="animation"){
		mode=Mode::ANIM;
	}else if(name=="char"){
		mode=Mode::CHAR;
	}
	reselect(nullptr);
	animationControl->setEnabled(mode==Mode::ANIM);
	animationControl->setVisible(mode==Mode::ANIM);
}

void ChedLevel::setFirstAnimation(){
	vector<string> result = skeleton->getAnimationNames();
	if(result.size()>0){
		onSelectAnimation(result[0]);
		Game::getUserInterface()->findDDLB("setAnimDDLB")->setElem(result[0]);
	}else{
		currentAnimationName = "";
		Game::getUserInterface()->findDDLB("setAnimDDLB")->setElem("select animation");
	}
}

void ChedLevel::loadSavedSettings(ClockHandle clock){
	auto root(createRoot<ChedSettingsParserRoot>(Game::getConfigPath()+"/"+Game::getProperty("settings")));
    auto saveSettingsRoot(createRoot<SaveParserRoot>(SAVE_FILE_PATH));


    { // Creating timeLine
        animationControl =new AnimationControl(root.chedSettings->timeline,
                saveSettingsRoot.savedData? saveSettingsRoot.savedData->timeLine:nullptr, clock, bones, skeleton.get());

        animationControl->add2UI(Game::getUserInterface());
    }

    if(!saveSettingsRoot.savedData)
        return;

    // Load Windows position
    UserInterface* ui = Game::getUserInterface();

    for(WindowPropsType* props : saveSettingsRoot.savedData->windows){
        ElementCollection* window = dynamic_cast<ElementCollection*>(ui->findElement(props->name));
        if(window==nullptr){
            log.warn("Window restoring: element [%s] not found ",props->name.c_str());
            continue;
        }
        window->setPosition(vec3(parse(props->pos),window->getPosition().z));
        window->setMinimazed(props->minimalized);
        window->setVisible(props->visible);
    }

    //Config UI
    if(saveSettingsRoot.savedData->draw){
        SavedDrawType* props = saveSettingsRoot.savedData->draw;
        {  // Config checkboxes
            selectBones = props->bonesState->selectable;
            selectPics = props->picsState->selectable;
            selectBoxes = props->physState->selectable;

            drawBones = props->bonesState->drawable;
            drawPics = props->picsState->drawable;
            drawBoxes = props->physState->drawable;

            ui->findCheckbox(SAV_SELECT_BONES)->setChecked(selectBones);
            ui->findCheckbox(SAV_SELECT_PICS)->setChecked(selectPics);
            ui->findCheckbox(SAV_SELECT_PHYS)->setChecked(selectBoxes);

            ui->findCheckbox(SAV_DRAW_BONES)->setChecked(drawBones);
            ui->findCheckbox(SAV_DRAW_PICS)->setChecked(drawPics);
            ui->findCheckbox(SAV_DRAW_PHYS)->setChecked(drawBoxes);
        }
    }

    if(saveSettingsRoot.savedData->properties){
        SavedPropertiesType* props = saveSettingsRoot.savedData->properties;
        // Config animation ddlb
        ui->findDDLB(SAV_ANIM)->setValue(props->workTarget);
		 onSelectWorkTarget(props->workTarget);

        ui->findDDLB("setAnimDDLB")->setValue(props->animation);
        onSelectAnimation(props->animation);
    }

    if(saveSettingsRoot.savedData->camera)
        Game::getCamera()->setPosition(parse(saveSettingsRoot.savedData->camera));
}

void ChedLevel::reindexBones(){
    indexedByIdBones.clear();
    for(auto& bone:bones){
        indexedByIdBones[bone->getId()]=bone.get();
    }
}

void ChedLevel::reSortBones(){
	//vector<ChedBone*> newOrder;
	/*for(auto& bone : bones){
		if(bone->getParent()==nullptr){
			//bone->addThisWithChilds(newOrder);
		}
	}*/
	/*bones.clear();
	bones.assign(newOrder.begin(),newOrder.end());*/
	//bones=newOrder;
    reindexBones();
}

void ChedLevel::saveSetting(){
    SaveParserRoot root;
    root.savedData = new ChedSavedData();
    root.savedData->timeLine = animationControl->saveSettings();
    root.savedData->camera = create3D(Game::getCamera()->getPosition());
    UserInterface* ui = Game::getUserInterface();

    for(string name : {"properties","draw","events","targets","AnimationControl"}){
        ElementCollection* window = dynamic_cast<ElementCollection*>(ui->findElement(name));
        if(window==nullptr) {
            log.warn("Window saving: element [%s] not found ",name.c_str());
            continue;
        }

        WindowPropsType* props = new WindowPropsType();
        props->pos = create2D(window->getPosition());
        props->visible = window->isVisible();
        props->minimalized = window->isMinimazed();
        props->name = name;
        root.savedData->windows.push_back(props);
    }

    {
        SavedPropertiesType* props = new SavedPropertiesType();
        root.savedData->properties = props;
        props->workTarget=ui->findElement(SAV_ANIM)->getValue();
        props->animation=ui->findElement("setAnimDDLB")->getValue();

    }{
        SavedDrawType* props = new SavedDrawType();
        root.savedData->draw = props;

        props->bonesState=new PropEntityStateType();
        props->bonesState->selectable=selectBones;
        props->bonesState->drawable=drawBones;

        props->picsState=new PropEntityStateType();
        props->picsState->selectable=selectPics;
        props->picsState->drawable=drawPics;

        props->physState=new PropEntityStateType();
        props->physState->selectable=selectBoxes;
        props->physState->drawable=drawBoxes;
    }

    ofstream out(Game::getFileSystem()->getFullName(SAVE_FILE_PATH));
    root.save("",out);
    out.close();
}

void ChedLevel::load(){
	auto root(createRoot<CharacterParserRoot>(worldFile));

	if(root.character==nullptr){
		return;
	}

    {
        vector<string> names;
        levelResources.loadResources(root.character->resources, worldFile);
        levelResources.getSpriteNames(names);
        Game::getUserInterface()->
                findImagePicker("image_picker")->addImages(names);
        Game::getUserInterface()->
                findImagePicker("new_slot_image")->addImages(names);
    }

    skeleton.reset(new Skeleton(vec2(0,0),root.character,
            CategoryReference::get(CategoryReference::PROTAGONIST)));


	for(auto iter:root.character->skeleton->bones){
		ChedBone * bone = new ChedBone(this, skeleton.get(),iter);
		bones.push_back(unique_ptr<ChedBone>(bone));
		indexedBones[bone->getName()]=bone;
		indexedByIdBones[bone->getId()]=bone;
	}

	{
		unordered_map<int, BoneSlotType*> xmlMap;
		for(auto xmlBone : root.character->skeleton->bones){
			for(auto xmlSlot : xmlBone->slot){
				xmlMap.insert(make_pair(xmlSlot->id,xmlSlot));
			}
		}

		for(auto& iter : skeleton->getSlots()){
			BoneSlotType* xmlData = xmlMap.find(iter->getId())->second;
			slotMaxId = glm::max(xmlData->id,static_cast<int>(slotMaxId));
			slots.push_back(make_unique<ChedSlot>(this,
					iter.get(),
					xmlData,
					indexedByIdBones[iter->getJoint()->getId()]));
		}
	}

	reSort();
	reSortBones();
	if(root.character->animations!=nullptr){
		for(auto iter: root.character->animations->animation){
			Game::getUserInterface()->findDDLB("setAnimDDLB")->add(iter->name);
		}
	}
	if(root.character->physics!=nullptr){
		for(CharBoxType* iter: root.character->physics->box){
			ChedPhysBox* box = new ChedPhysBox(this, iter);
			if(iter->parent.size()>0){
				box->setParent(this, indexedBones[iter->parent]);
			}
			boxes.push_back(unique_ptr<ChedPhysBox>(box));
		}
	}
}

void ChedLevel::save(){
	CharacterParserRoot root;
	root.character = new CharacterType();
	root.character->skeleton = new SkeletonType();
	root.character->physics = new CharPhysType();
	for(auto& iter : bones){
		root.character->skeleton->bones.push_back(iter->save());
	}
	root.character->animations=new AnimationListType();
	skeleton->saveAnimations(root.character->animations->animation);

	for(auto& iter : boxes){
		root.character->physics->box.push_back(iter->save());
	}
    {
        root.character->resources=new ResourceDescriptionList();
        levelResources.saveResources(root.character->resources);
    }

	ofstream out(Game::getFileSystem()->getFullName(worldFile));
	root.save("",out);
	out.close();
}

void ChedLevel::setCurrentTransformation(TransformId transform){
    instrumentHelper.setCurrentTransform(transform);
}

void ChedLevel::moveCamera(const glm::vec2& delta){
    Game::getCamera()->moveCamera(vec3(delta,0));
}

void ChedLevel::zoomCamera(float delta){
    Game::getCamera()->moveCamera(vec3(0,0,delta));
}

void ChedLevel::showInputNameDialog(const std::string title, const std::function<void(const std::string &)> &callback) {
    fadeBackground(true);
    UserInterface* ui = Game::getUserInterface();
    ui->findElement("input_name_dialog")->setVisible(true);
    ui->findElement("input_name_title")->setValue(title);
    ui->findElement("new_name")->setValue("");
    currentInputNameCallback = callback;
}
