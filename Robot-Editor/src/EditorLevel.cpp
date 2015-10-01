#include <ostream>
#include <fstream>

#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/UserInterface/DropDownListBox.h>
#include <ChibiEngine/UserInterface/ImagePicker.h>
#include <ChibiEngine/UserInterface/RGBSlider.h>
#include <ChibiEngine/UserInterface/Slider.h>
#include <ChibiEngine/UserInterface/Checkbox.h>
#include <ChibiEngine/UserInterface/InputField.h>
#include <ChibiEngine/UserInterface/Radio.h>
#include <ChibiEngine/UserInterface/TableContainer.h>
#include <ChibiEngine/UserInterface/ContainerSelector.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Common/StringUtils.h>

#include <EditorLevel.h>

#include <EditorLevelImpl/FXObject.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/EditorPres2D.h>
#include <EditorLevelImpl/PointObject.h>
#include <EditorLevelImpl/EditorGeomJoint.h>
#include <EditorLevelImpl/EnemyObject.h>
#include <EditorLevelImpl/EventObject.h>
#include <EditorLevelImpl/Layer.h>
#include <EditorLevelImpl/LightObject.h>
#include <EditorLevelImpl/GeometryLineChain.h>
#include <EditorLevelImpl/Commands/CommandsHistory.h>
#include <EditorLevelImpl/Commands/MoveCommand.h>
#include <EditorLevelImpl/Commands/ScaleCommand.h>
#include <EditorLevelImpl/Commands/RotateCommand.h>
#include <EditorLevelImpl/DirLightObject.h>
#include <EditorLevelImpl/EditorPres3D.h>
#include <EditorLevelImpl/EditorConstants.h>
#include "Generated/SaveParser.h"
#include "EditorLevelImpl/EditorLevelScripts.hpp"

using namespace game;
using namespace glm;
using namespace std;
using namespace rapidxml;

static const string PRES_CON_MENU = "pres_con_menu";
static const string GEOM_CON_MENU = "geom_con_menu";
static const string CONSTR_CAMERA_BUTTON = "camera_control";
static const string PRES_CONTEXT_BUTTON = "pres_con_menu";
static const string GEOM_CONTEXT_BUTTON = "geom_con_menu";
static const string IMAGE_DIALOG = "image_dialog";
static const string IMAGE_PICKER = "image_picker";

static const int MAX_HISTORY_LEN = 10;
static const string MAIN_LAYER = "main";

static const string SAVE_FILE_PATH = "editor/saved.dat";

void EditorLevel::setTexels(const glm::vec2& tex){
    EditorPres3D * b = dynamic_cast<EditorPres3D *>(selectedObject);
    if(b!=nullptr){
        b->setTexels(tex);
    }
}

bool EditorLevel::ObjCompare::operator()(OperableObject* const lhs, OperableObject*  const rhs) const{
	return lhs->getPosition().z >= rhs->getPosition().z;
}

struct LayerComp{
    bool operator()(const Layer* l1, const Layer* l2){
        return l1->getZ()<l2->getZ();
    };
};

EditorLevel::EditorLevel(const std::string& worldFile):
        layers(),
        activeLayer(nullptr),

        log(Game::getLoggingSystem()->createLogger("EditorLevel")),
        inputListener(this),
        drawer(),
        object4Copy(nullptr),
        worldFile(worldFile),
        history(new CommandsHistory(MAX_HISTORY_LEN)),
        cameraConstrained(false),

        pres3DInfo(this),
        pres2DInfo(this),
        geomChainInfo(this),
        geomObjectInfo(this),
        geomFixtureInfo(this),
        geomJointInfo(this),
        spotLightInfo(this),
        dirLightInfo(this),
        fxObjectInfo(this),
        eventInfo(this),
        enemyInfo(this),
        pointInfo(this),

        imgDialogInvokeCoords(),
        grid(10,20),

        ambientLight(Color::White),
        dayLight(Color::White),
        nightLight(Color::Black),
        daytime(1.0f),

        selectedObject(nullptr){


	UserInterface* ui = Game::getUserInterface();
	InputSystem* is = Game::getInputSystem();

	contextMenu = ui->findContextMenu("editor_context_menu");
	objectMenu = ui->findContextMenu(GEOM_CON_MENU);
	presMenu = ui->findContextMenu(PRES_CON_MENU);
	dialog = ui->findElement(IMAGE_DIALOG);
	imagePicker = dynamic_cast<ImagePicker*>(ui->findElement(IMAGE_PICKER));

	is->addRawListener(&inputListener);

	ui->addEventListener("menu.file", bind(&EditorLevel::onMenuFile, this, placeholders::_1));
	ui->addEventListener("menu.weather", bind(&EditorLevel::onMenuWeather, this, placeholders::_1));
	ui->addEventListener("menu.layers",bind(&EditorLevel::onMenuLayers, this, placeholders::_1));

	ui->addEventListener(CONSTR_CAMERA_BUTTON,bind(&EditorLevel::setCameraConstrained, this, placeholders::_1));
	ui->addEventListener("editor_context_menu.geometry", bind(&EditorLevel::onAddGeometry, this, placeholders::_1));
	ui->addEventListener("editor_context_menu.light", bind(&EditorLevel::onAddLight, this, placeholders::_1));
	ui->addEventListener("editor_context_menu.fx", bind(&EditorLevel::onAddFX, this, placeholders::_1));
	ui->addEventListener("editor_context_menu", bind(&EditorLevel::onContextMenu, this, placeholders::_1));
	ui->addEventListener("image_picker.select", bind(&EditorLevel::onImageSelect, this, placeholders::_1));

	ui->addEventListener("dayTime", bind(&EditorLevel::onDaytime, this, placeholders::_1));
	ui->addEventListener("night_color", bind(&EditorLevel::onNightColor, this, placeholders::_1));
	ui->addEventListener("day_color", bind(&EditorLevel::onDayColor, this, placeholders::_1));

    ui->addEventListener("geomObject.addPres", bind(&EditorLevel::onAddPres2GeomObject, this, placeholders::_1));
    ui->addEventListener("geomObjectParent.addFixture", bind(&EditorLevel::onAddFixture2GeomObject, this, placeholders::_1));

	load();
	initUIValues();
}


void EditorLevel::calcAmbLight(){
	ambientLight=nightLight*(1.0f-daytime)+dayLight*daytime;
}

void EditorLevel::initUIValues(){
	UserInterface* ui = Game::getUserInterface();
	ui->findRGBSlider("night_color")->setColor(nightLight);
	ui->findRGBSlider("day_color")->setColor(dayLight);
	ui->findSlider("dayTime")->setFloatValue(daytime);

    //layers
    //static_cast<RadioButton*>(ui->findElement("main"))->setChecked(true);
    //ui->findElement("main.layer")->setEnabled(false);
}

void EditorLevel::addLayerRecordInTable(Layer* newLayer){
    UserInterface* ui = Game::getUserInterface();
    TableContainer* table = ui->findTable("layers");
    InputField* inf = new InputField(newLayer->getName());
    inf->setName(newLayer->getName()+".layer");
    RadioButton* rb = new RadioButton(vec3(0,0,0),newLayer->getName(),"layer_group",false);
    Checkbox* cb = new Checkbox(vec3(0,0,0),newLayer->getName()+".visible",true);
    table->add(inf);
    table->add(rb);
    table->add(cb);
    table->pack();
    ui->add(inf);
    ui->add(rb);
    ui->add(cb);
    table->move(vec3(0, -inf->getHH(), 0));
}

void EditorLevel::removeLayerRecordInTable(Layer* l){
    //todo implement
}

void EditorLevel::onDayColor(const std::string& name){
	dayLight = Color(name);
	calcAmbLight();
}

void EditorLevel::onNightColor(const std::string& name){
	nightLight = Color(name);
	calcAmbLight();
}

void EditorLevel::onDaytime(const std::string& name){
	daytime=toFloat(name);
	calcAmbLight();
}


EditorLevel::~EditorLevel(){
    saveSettings();
    for(auto iter : layers){
        delete iter;
    }

	delete history;
}

void EditorLevel::moveCamera(const glm::vec2& delta){
	/*if(cameraConstrained)
		camConstraints->moveCamera(Game::getCamera(),delta);
	else*/
        Game::getCamera()->moveCamera(vec3(delta,0));
}

void EditorLevel::zoomCamera(float delta){
    Game::getCamera()->moveCamera(vec3(0,0,delta));
}

void EditorLevel::onImageSelect(const std::string& name){
    onSpriteSelectCallback();
}

void EditorLevel::addPresent(){
    activeLayer->add(new EditorPres2D(
            glm::vec3(imgDialogInvokeCoords,0),
            imagePicker->getSelected()->getName(),
            imagePicker->getDrawScale(),nullptr,activeLayer));
    dialog->setVisible(false);
}

void EditorLevel::addPresent2GeomObj(){
    EditorPres2D* pres = new EditorPres2D(vec3(0,0,0),
            imagePicker->getSelected()->getName(),
            imagePicker->getDrawScale(),
            targetFixtureObject,activeLayer);
    targetFixtureObject->addPresentation(pres);
}

void EditorLevel::addBox(){
    add(new EditorPres3D(glm::vec3(imgDialogInvokeCoords,0),imagePicker->getSelected()->getTexture(),activeLayer));
    dialog->setVisible(false);
}

void EditorLevel::onMenuWeather(const std::string& name){
	if(name=="global light")
        Game::getUserInterface()->findElement("light_dialog")->setVisible(true);
}

void EditorLevel::onMenuLayers(const std::string& name){
    if(name=="add"){
        Layer* l = new Layer("new layer");
        layers.push_back(l);
        layersIndex[l->getName()]=l;
        //addLayerRecordInTable(l);
    }else if(name=="delete" && activeLayer->getName()!=MAIN_LAYER){
        //todo impl
        /*layers.erase(remove(layers.begin(),layers.end(),activeLayer));
        layersIndex.erase(activeLayer->getName());
        activeLayer=layersIndex[MAIN_LAYER];
        static_cast<RadioButton*>(Game::getUserInterface()->findElement(MAIN_LAYER))->setValue(true);*/
        removeLayerRecordInTable(activeLayer);
    }

}

void EditorLevel::onMenuFile(const std::string &name){
	if(name=="save")
		save();
    else if(name=="settings")
        Game::getUserInterface()->findElement("settings_dialog")->setVisible(true);
}

void EditorLevel::setCameraConstrained(const std::string& name){
	cameraConstrained = name=="true";
}

void EditorLevel::hideMenus(){
	contextMenu->hide();
	objectMenu->hide();
	presMenu->hide();
}

void EditorLevel::onAddGeometry(const std::string& name){
	//if(name=="ground"){
	//	addObject(getMouseCoordsInZeroPlane(), ObjectType::GROUND);
	//}else{
	//}
    if(name == "chain") {
        addChain(getMouseCoordsInZeroPlane());
    }else if(name=="3d box") {
        dialog->setVisible(true);
        imgDialogInvokeCoords = getMouseCoordsInZeroPlane();
        onSpriteSelectCallback = bind(&EditorLevel::addBox, this);
    }else if(name == "box"){
        EditorGeometryObject* ego = new EditorGeometryObject(vec3(getMouseCoordsInZeroPlane(),0),"",activeLayer);
        ego->add(new EditorFixtureObject(vec3(0,0,0),ego,activeLayer));
        add(ego);
    }else if(name == "joint"){
        activeLayer->add(new EditorGeomJoint(vec3(getMouseCoordsInZeroPlane(),0),100,"",activeLayer));
    }

	hideMenus();
}

void EditorLevel::onAddPres2GeomObject(const std::string& name){
    dialog->setVisible(true);
    targetFixtureObject = static_cast<EditorFixtureObject*>(selectedObject);
    onSpriteSelectCallback = bind(&EditorLevel::addPresent2GeomObj, this);
}

void EditorLevel::onAddFixture2GeomObject(const std::string& name){
    EditorGeometryObject* ego = static_cast<EditorGeometryObject*>(selectedObject);
    ego->add(new EditorFixtureObject(vec3(0,0,0),ego,activeLayer));
}

void EditorLevel::onAddLight(const std::string& name){
	if(name=="spot"){
        activeLayer->add(new SpotLightObject(vec3(getMouseCoordsInZeroPlane(),0),vec2(1,1)));
	}else if(name=="directional"){
        activeLayer->add(new DirLightObject(vec3(getMouseCoordsInZeroPlane(),0),vec2(1,1)));
    }
}

void EditorLevel::onAddFX(const std::string& name){
	if(activeLayer==nullptr)
        return;

    add(new FXObject(name,
            EffectParams(
                    vec3(getMouseCoordsInZeroPlane(),0),
                    vec2(1,1),
                    vec2(0,1),
                    Color::White)));
	hideMenus();
}

void EditorLevel::onContextMenu(const std::string& name){
	if(name=="present") {
        dialog->setVisible(true);
        imgDialogInvokeCoords = getMouseCoordsInZeroPlane();
        onSpriteSelectCallback = bind(&EditorLevel::addPresent, this);
    }else if(name=="add enemy") {
        othersObjects.push_back(new EnemyObject(getMouseCoordsInZeroPlane(), "zombie"));
    }else if(name=="add point"){
        othersObjects.push_back(new NamedPointObject(vec3(getMouseCoordsInZeroPlane(),0),"new point"));
    }else if(name == "event"){
        othersObjects.push_back(new EventObject(
                getMouseCoordsInZeroPlane(),
                "new event"));
    }
}

void EditorLevel::draw(CameraCHandle cam){
//    Game::getResources()->findFont("default")->debugDrawAtals(cam);
    for(auto iter : layers){
        iter->draw();
    }

    for(auto iter : othersObjects){
        iter->draw(cam);
    }

    grid.draw();
}
void EditorLevel::invalidateObject(){
    pres3DInfo.link(nullptr);
    pres2DInfo.link(nullptr);
    geomChainInfo.link(nullptr);
    geomFixtureInfo.link(nullptr);
    geomJointInfo.link(nullptr);
    spotLightInfo.link(nullptr);
    dirLightInfo.link(nullptr);
    fxObjectInfo.link(nullptr);
    eventInfo.link(nullptr);
    enemyInfo.link(nullptr);
    pointInfo.link(nullptr);
    geomObjectInfo.link(nullptr);
}

void EditorLevel::selectPres2D(EditorPres2D* pres2D){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::PRES2D);
    pres2DInfo.link(pres2D);
}

void EditorLevel::selectPres3D(EditorPres3D * pres3D){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::PRES3D);
    pres3DInfo.link(pres3D);
}

void EditorLevel::selectGeomChain(GeometryLineChain* geomChain){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::GEOM_CHAIN);
    geomChainInfo.link(geomChain);
}

void EditorLevel::selectSpotLight( SpotLightObject* spotLight){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::SPOT_LIGHT);
    spotLightInfo.link(spotLight);
}

void EditorLevel::selectDirLight( DirLightObject* dirLight){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::DIR_LIGHT);
    dirLightInfo.link(dirLight);
}

void EditorLevel::selectFxObject( FXObject * fxObject){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::FX_OBJECT);
    fxObjectInfo.link(fxObject);
}

void EditorLevel::selectEvent( EventObject* event){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::EVENT);
    eventInfo.link(event);
}
void EditorLevel::selectEnemy( EnemyObject* enemy){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::ENEMY);
    enemyInfo.link(enemy);
}
void EditorLevel::selectPoint( NamedPointObject* enemy){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::POINT);
    pointInfo.link(enemy);
}

void EditorLevel::selectGeomObject( EditorGeometryObject* object ){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::GEOM_OBJECT);
    geomObjectInfo.link(object);
}

void EditorLevel::selectFixtureObject( EditorFixtureObject* object ){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::GEOM_FIXTURE);
    geomFixtureInfo.link(object);
}

void EditorLevel::selectGeomJoint( EditorGeomJoint* object){
    Game::getUserInterface()->findContainerSelector("conSelector")->activate(EditorConstants::GEOM_JOINT);
    geomJointInfo.link(object);
}

//todo void geomObject(const GeometryObject* geomObj);
//todo void selectGeomJoint(const GeometryJoint* geomJoint);

void EditorLevel::add(EditorPres3D * box){
    activeLayer->add(box);
}

void EditorLevel::add(EventObject* box){
    othersObjects.push_back(box);
}

void EditorLevel::add(FXObject * fx){
    activeLayer->add(fx);
}

void EditorLevel::add(EditorGeometryObject* obj){
    activeLayer->add(obj);
}


/*void EditorLevel::addObject(const glm::vec2& pos, ObjectType type){
    if(activeLayer!= nullptr)
	    activeLayer->addGeometry(new EditorGeometryObject(pos,type,this));
}*/

void EditorLevel::addChain(const glm::vec2& pos){
    if(activeLayer==nullptr)
        return;

	GeometryLineChain* chain = new GeometryLineChain(activeLayer);
	GeometryLineChainElement* el = new GeometryLineChainElement(vec3(pos,0), chain);
	chain->add(el);
    activeLayer->add(chain);
}

void EditorLevel::deleteObject(){
	if(selectedObject==nullptr)
		return;
	if(object4Copy==selectedObject)
		object4Copy=nullptr;

	invalidateObject();
	// На тот случай, если владелец объекта - уровень

    if(activeLayer!= nullptr){
        activeLayer->deleteObject(selectedObject);
    }
    othersObjects.erase(remove(othersObjects.begin(),othersObjects.end(),selectedObject),othersObjects.end());

	delete selectedObject;
	selectedObject = nullptr;
}

void EditorLevel::paste(){
	if(object4Copy==nullptr)
		return;
	object4Copy->clone();
}

bool EditorLevel::selectObject(const EditorInputListener::MouseRay& ray, bool selectNext){
	OperableObject* obj = findObject(ray, selectNext);
	if(obj!=selectedObject && selectedObject!=nullptr)
		selectedObject->setSelected(false);

	selectedObject = obj;
	if(obj!=nullptr){
		vec3 p = ray.calcPos(obj->getPosition().z);
		grabObject(obj,vec2(p.x, p.y));
		return true;
	}
	return false;
}

void EditorLevel::markForCopy(){
	object4Copy=selectedObject;
}

void EditorLevel::viewContextMenu(const glm::vec2& pnt){
	endDraging();
    contextMenu->viewAt(pnt);
}

void EditorLevel::grabObject(OperableObject* object, const glm::vec2& pos){
	//Отменяем выбор старого
	if(selectedObject!=nullptr){
		selectedObject->setSelected(false);
	}

	selectedObject = object;
	// Выделяем новый
	if(selectedObject!=nullptr){
		selectedObject->grab(pos);
		selectedObject->setSelected(true);
	}else{
		//propertyBar->resetView();
	}
}

void EditorLevel::loadSavedSettings(){
    UserInterface* ui = Game::getUserInterface();

    auto root(createRoot<SaveParserRoot>(SAVE_FILE_PATH));

    //setting camera pos
    Game::getCamera()->setPosition(parse(root.saveData->cameraPos));
    //setting selectFilter
    for(string name : EditorConstants::getTypes()){
        ui->findCheckbox("select."+name)->setChecked(false);
    }
    for(string name : root.saveData->selectFilter->entityName){
        ui->findCheckbox("select."+name)->setChecked(true);
    }

    for(string name : EditorConstants::getTypes()){
        selectRef[name]=ui->findCheckbox("select."+name)->isChecked();
    }
}

void EditorLevel::saveSettings(){
    UserInterface* ui = Game::getUserInterface();
    SaveParserRoot root;
    root.saveData = new SaveDataType();
    root.saveData->cameraPos = create3D(Game::getCamera()->getPosition());
    root.saveData->selectFilter = new SelectFilterType();
    for(string name : EditorConstants::getTypes()){
        if(ui->findCheckbox("select."+name)->isChecked()){
            root.saveData->selectFilter->entityName.push_back(name);
        }
    }
    if(ui->findCheckbox("select.all")->isChecked()){
        root.saveData->selectFilter->entityName.push_back("all");
    }
    ofstream out(Game::getFileSystem()->getFullName(SAVE_FILE_PATH));
    root.save("",out);
    out.close();
}

void EditorLevel::load(){
	auto root(createRoot<WorldParserRoot>(worldFile));

	if(root.world==nullptr)
		return;

    {
        levelResources.loadResources(root.world->resources, worldFile);
        ImagePicker* ip = Game::getUserInterface()->findImagePicker("image_picker");
        vector<string> names;
        levelResources.getSpriteNames(names);
        ip->addImages(names);
    }


    for(WorldLayers* layer :  root.world->layer){
        Layer* l = new Layer(layer);
        layersIndex[layer->name]= l;
        layers.push_back(l);
    }
    sort(layers.begin(),layers.end(), LayerComp());

    if(layers.size()==0){
        Layer* l = new Layer(MAIN_LAYER);
        layersIndex[MAIN_LAYER]=l;
        layers.push_back(l);
    }else{
        activeLayer=layersIndex[MAIN_LAYER];
        if(activeLayer==nullptr){
            Layer* l = new Layer(MAIN_LAYER);
            layersIndex[MAIN_LAYER]=l;
            layers.push_back(l);
        }
    }
    activeLayer=layersIndex[MAIN_LAYER];

	if(root.world->points.size()==0){
		othersObjects.push_back(new NamedPointObject(vec3(0,0,0),"begin"));

	}else{
		for(auto iter:root.world->points)
            othersObjects.push_back(new NamedPointObject(iter));
	}

	if(root.world->enemies!=nullptr){
		for(auto iter : root.world->enemies->enemy){
            othersObjects.push_back(new EnemyObject(iter));
		}
	}
    for(auto iter : root.world->event){
        othersObjects.push_back(new EventObject(iter));
    }

	camSettings=CamSettings(root.world->camera);
    Game::getCamera()->setPosition(camSettings.pos);

    UserInterface* ui = Game::getUserInterface();
	if(root.world->weather!=nullptr){
		WeatherType* weather = root.world->weather;
		daytime = weather->daytime;
		dayLight = Color(weather->dayLight->x,weather->dayLight->y,weather->dayLight->z,1.0);
		nightLight = Color(weather->nightLight->x,weather->nightLight->y,weather->nightLight->z,1.0);
		calcAmbLight();
        ui->findCheckbox("thunder")->setChecked(weather->thunder);
	}

    // load layers list
   /* for(Layer *layer : layers){
     //   addLayerRecordInTable(layer);
    }
*/
    LevelSettings* ls = root.world->levelSettings;
    if(ls!=nullptr) {
        ui->findInputField("script_file")->setValue(ls->scriptName);
        ui->findInputField("start_event")->setValue(ls->startEvent);
    }
    loadSavedSettings();
    installScripts();
}

OperableObject* EditorLevel::findObject(const EditorInputListener::MouseRay& ray, bool selectNext){
    vec3 pntZero = ray.calcPos(0.0f);
    for(OperableObject* otherObject : othersObjects){
        OperableObject* other = otherObject->containsPoint(vec2(pntZero.x,pntZero.y));
        if(other!=nullptr){
            return other;
        }
    }
    return activeLayer->find(ray.calcPos(activeLayer->getZ()));
}

void EditorLevel::endDraging(){
	if(selectedObject==nullptr)
		return;

	selectedObject->release();
}

void EditorLevel::onMouseDrag(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin){
    moveObject(end,begin);
}

void EditorLevel::moveObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin){
	if(selectedObject==nullptr)
		return;

	moveObject(end.calcPos(selectedObject->getPosition().z)-begin.calcPos(selectedObject->getPosition().z), false);
}

void EditorLevel::moveObject(const glm::vec3& delta, bool silent){
	if(selectedObject==nullptr)
		return;

	selectedObject->setSilentMode(silent);
	history->execute(new MoveCommand(delta,selectedObject));
	selectedObject->setSilentMode(false);
}

void EditorLevel::setZOrder(int zOrder){
    if(selectedObject== nullptr)
        return;

    selectedObject->setZOrder(zOrder);
    activeLayer->resort();
}

void EditorLevel::scaleObject(const glm::vec2 newScale, bool silent){
	if(selectedObject==nullptr)
		return;

	EditorPres2D* pres = dynamic_cast<EditorPres2D*>(selectedObject);
	if(pres!=0){
		pres->setSilentMode(silent);
		history->execute(new ScaleCommand(newScale, pres));
		pres->setSilentMode(false);
	}
}

void EditorLevel::scaleObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin){
	if(selectedObject==nullptr)
		return;

	vec3 pnt1 = end.calcPos(selectedObject->getPosition().z);
	vec3 pnt2 = begin.calcPos(selectedObject->getPosition().z);

	scaleObject(vec2(0.03*(pnt1.x-pnt2.x),0.03*(pnt1.y-pnt2.y)),false);
}

void EditorLevel::rotateObject(float newAngle, bool silent){
	if(selectedObject==nullptr)
		return;

	EditorPres2D* pres = dynamic_cast<EditorPres2D*>(selectedObject);
	if(pres!=0){
		pres->setSilentMode(silent);
		history->execute(new AddRotationCommand(newAngle, pres));
		pres->setSilentMode(false);
	}
}

void EditorLevel::horSpreadObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin){
	if(selectedObject==nullptr)
		return;

	vec3 pnt1 = end.calcPos(selectedObject->getPosition().z);
	vec3 pnt2 = begin.calcPos(selectedObject->getPosition().z);

	EditorPres2D* pres = dynamic_cast<EditorPres2D*>(selectedObject);
	if(pres!=0){
		pres->setSilentMode(true);
		pres->spreadTexture(vec2(pnt1.x-pnt2.x,0));
		pres->setSilentMode(false);
	}

}

void EditorLevel::verSpreadObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin){
	if(selectedObject==nullptr)
		return;

	vec3 pnt1 = end.calcPos(selectedObject->getPosition().z);
	vec3 pnt2 = begin.calcPos(selectedObject->getPosition().z);

	EditorPres2D* pres = dynamic_cast<EditorPres2D*>(selectedObject);
	if(pres!=0){
		pres->setSilentMode(true);
		pres->spreadTexture(vec2(0,pnt1.y-pnt2.y));
		pres->setSilentMode(false);
	}

}

void EditorLevel::rotateObject(const EditorInputListener::MouseRay& end, const EditorInputListener::MouseRay& begin){
	if(selectedObject==nullptr)
		return;

	vec3 pnt1 = end.calcPos(selectedObject->getPosition().z);
	vec3 pnt2 = begin.calcPos(selectedObject->getPosition().z);
	rotateObject(0.03*(pnt1.x-pnt2.x),false);
}

void EditorLevel::undo(){
	history->undo();
}
void EditorLevel::redo(){
	history->redo();
}

void EditorLevel::add2Pres(EditorPres2D* obj){
    activeLayer->add(obj);
}

void EditorLevel::save(){
	WorldParserRoot* root = new WorldParserRoot();
	WorldType* world = new WorldType();
	root->world = world;

    world->camera=camSettings.create();
    for(auto layer : layers){
        layer->save(root->world);
    }
    for(auto iter : othersObjects){
        iter->save2World(world);
    }
	{
		{
			WeatherType* w = new WeatherType();
            w->dayLight = dayLight.toPoint3D();
            w->nightLight = dayLight.toPoint3D();
			w->daytime = daytime;
			w->thunder = Game::getUserInterface()->findCheckbox("thunder")->isChecked();
			world->weather=w;
		}
	}
    {
        world->levelSettings=new LevelSettings();
        world->levelSettings->scriptName=Game::getUserInterface()->findInputField("script_file")->getValue();
        world->levelSettings->startEvent=Game::getUserInterface()->findInputField("start_event")->getValue();
    }
    {
        world->resources=new ResourceDescriptionList();
        levelResources.saveResources(world->resources);
    }

	ofstream out(Game::getFileSystem()->getFullName(worldFile));
	root->save("",out);
	out.close();
	log.info("Saved in "+worldFile);
	delete root;
}

EditorLevel *EditorLevel::instance(){
    return static_cast<EditorLevel*>(Game::getGame()->getLevel());
}


EditorLevel::CamSettings::CamSettings()
:pos(0,0,0),rec(-40,40,40,-40){

}

EditorLevel::CamSettings::CamSettings(RobotCameraSettings* iter)
:pos(parse(iter->initPos)),rec(
        iter->constraints->left,
        iter->constraints->top,
        iter->constraints->right,
        iter->constraints->bottom){

}
RobotCameraSettings* EditorLevel::CamSettings::create(){
    RobotCameraSettings* s = new RobotCameraSettings();
    s->initPos=create3D(pos);
    RobotCameraConstraints* c = new RobotCameraConstraints();
    c->left = rec.x;
    c->top = rec.y;
    c->right = rec.z;
    c->bottom = rec.w;
    s->constraints = c;
    return s;
}

void EditorLevel::add2Others(OperableObject* obj){
    othersObjects.push_back(obj);
}

void EditorLevel::setSelection(const std::string& name, bool selectable){
    selectRef[name]=selectable;
}

bool EditorLevel::isSelectable(const std::string& name){
    return selectRef[name];
}