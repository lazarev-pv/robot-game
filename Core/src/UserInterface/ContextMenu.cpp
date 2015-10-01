#include <ChibiEngine/Game.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/UserInterface/ContextMenu.h>
#include <ChibiEngine/Common/RAIIUtils.h>

using namespace game;
using namespace std;
using namespace glm;

const string ButtonContextMenu::TYPE_NAME = "ButtonContextMenu";

ButtonContextMenu::AbstractDraw::AbstractDraw(ButtonContextMenu& parent):
		parent(parent),
		laf(Game::getLookAndFeel().menu){}

class ButtonContextMenu::DrawSimple : public ButtonContextMenu::AbstractDraw{
public:
	DrawSimple(ButtonContextMenu& parent):AbstractDraw(parent){};
	void draw() override{
		Game::getPrimitiveDrawer()->drawBorderedFilledRectangle(
				parent.getPosition(),
				parent.getSize(),
				laf.borderThickness, laf.backColor, laf.borderColor);
		parent.ElementCollection::draw();
	}

	void drawSelected(const glm::vec3& ) override{
		Game::getPrimitiveDrawer()->drawBorderedFilledRectangle(
				parent.getPosition(),
				vec2(parent.getHW(), parent.averageHeight),
				laf.borderThickness, laf.hoveredColor, laf.borderColor);
	}
};

class ButtonContextMenu::DrawSprite : public ButtonContextMenu::AbstractDraw {
public:
	DrawSprite( const std::string& _image,
				const std::string& selImageName,
				ButtonContextMenu& parent,
				float hw, float hh)

			:AbstractDraw(parent),
			 imageName(_image), selImageName(selImageName),
			 image(Game::getResources()->findSpriteInfo(imageName),
					 hw*Game::getScreen()->getWidth()/2.0f,
					 hh*Game::getScreen()->getHeight()/2.0f),
			 selImage(Game::getResources()->findSpriteInfo(selImageName),
					 hw*Game::getScreen()->getWidth()/2.0f,
					 parent.averageHeight*Game::getScreen()->getHeight()/2.0f){
	}

	void setSize(const glm::vec2& _size) override{
		selImage.setSize(vec2(_size.x*Game::getScreen()->getWidth()/2.0f,
				parent.averageHeight*Game::getScreen()->getHeight())/2.0f);
		image.setSize(_size*Game::getScreen()->getSize()/2.0f);
	}

	void drawSelected(const glm::vec3& pos ) override{
		selImage.draw(pos);
	}

	void draw() override{
		image.draw(parent.getPosition());
		parent.ElementCollection::draw();
	}

private:
	string imageName;
	string selImageName;
	NineSliceSprite image, selImage;
};


class MenuFolder : public UIElement{
public:
	MenuFolder(const string& name, ButtonContextMenu* menu,ButtonContextMenu* parent)
		:UIElement(),
		 text(Game::getLookAndFeel().menu.font,name+" >"),
		 menu(menu),
		 parent(parent){

		setPosition(menu->getPosition()+vec3(0,0,1));
		const MenuLAF& laf = Game::getLookAndFeel().menu;
		color=laf.font.color;

		UIElement::setSize(SpriteText::toScreenSize(vec2(
                text.getWorldHW()+laf.padding.x+laf.padding.z,
                text.getWorldHH()+laf.padding.y+laf.padding.w
        )));
	}

	const std::string& getTypeName() override{
		return TYPE_NAME;
	}

	void draw() override{
		if(menu->isVisible())
			menu->draw();
		text.draw(getPosition(),color);
	}

	virtual void executeHoverOnAnimation(){
		parent->hideOpenedFolder();
		menu->viewAt(glm::vec2(getPosition().x+getHW(), getPosition().y+getHH()/**2*/));
		parent->openSubMenu(menu);
	}

	virtual void add2UI(UserInterface* ui){
		UIElement::add2UI(ui);
		menu->add2UI(ui);
	}

	void hide(){
		menu->hide();
	}

	virtual UIElement* containPoint(const glm::vec2& pnt){
		UIElement* thisP = UIElement::containPoint(pnt);
		if(thisP!=nullptr)
			return thisP;
		return menu->containPoint(pnt);
	}

private:
	const static std::string TYPE_NAME;
	SpriteText text;
	ButtonContextMenu* menu;
	ButtonContextMenu* parent;
	Color color;
};

const string MenuFolder::TYPE_NAME = "MenuFolder";

class ButtonContextMenu::ButtonWrap : public UIElement {
public:
	ButtonWrap(ButtonContextMenu* parent, const std::string& _text)
		:parent(parent),
		 text(Game::getLookAndFeel().menu.font, _text){

		setPosition(parent->getPosition()+vec3(0,0,2));
		setVisible(false);
		assert(_text.size()>0);

		setInstantAction(true);
		const MenuLAF& laf = Game::getLookAndFeel().menu;

		UIElement::setSize(SpriteText::toScreenSize(vec2(
                text.getWorldHW()+laf.padding.x+laf.padding.z,
                text.getWorldHH()+laf.padding.y+laf.padding.w)));

	};

	virtual const std::string& getTypeName(){
		return TYPE_NAME;
	}

	virtual void executeHoverOnAnimation(){
        UIElement::executeHoverOnAnimation();
		parent->hideOpenedFolder();
	}

	virtual void executeAction(){
        Game::getUserInterface()->fireEvent(parent->getName(),text.getText());
		parent->hideAll();
	}

	const string& getText() const{
		return text.getText();
	}

	virtual void draw(){
		if(isHovered()){
			parent->getDrawer()->drawSelected(getPosition());
		}
		text.draw(getPosition(), Game::getLookAndFeel().menu.font.color);
	}

private:
	const static std::string TYPE_NAME;
	ButtonContextMenu* parent;
	SpriteText text;
};

const std::string ButtonContextMenu::ButtonWrap::TYPE_NAME = "ButtonWrap";

ButtonContextMenu* ButtonContextMenu::createSubMenu(ContextMenuElemType* _elem, ButtonContextMenu* parent){
	ButtonContextMenu* menu = new ButtonContextMenu();
	menu->setVisible(false);
	menu->setName(parent->getName()+"."+_elem->name);
	for(auto elem : _elem->elem){
		if(elem->elem.size()>0){
			menu->addSubMenu(elem->name,createSubMenu(elem,menu));
		}else{
			menu->add(elem->name);
		}
	}
	return menu;
}


ButtonContextMenu::ButtonContextMenu(ContextMenuElemType* iter,const std::string name)
	:ElementCollection(),
	 laf(Game::getLookAndFeel().menu),
	 openedFolder(nullptr),
	 averageHeight(0.0f),
	 parent(nullptr){

	setName(name);
	for(auto elem : iter->elem){
		if(elem->elem.size()>0){
			addSubMenu(elem->name,createSubMenu(elem,this));
		}else{
			add(elem->name);
		}
	}

}

ButtonContextMenu::ButtonContextMenu(SlotType* iter)
	:ElementCollection(),
	 laf(Game::getLookAndFeel().menu),
	 openedFolder(nullptr),
	 averageHeight(0.0f),
	 parent(nullptr){

	setPosition(vec3(iter->x,iter->y,0.5+iter->z));
	setName(iter->name);


	for(auto elem : iter->choiceValue.context->elem){
		if(elem->elem.size()>0){
			addSubMenu(elem->name,createSubMenu(elem,this));
		}else{
			add(elem->name);
		}
	}
}

ButtonContextMenu::ButtonContextMenu()
	:ElementCollection(),
	 laf(Game::getLookAndFeel().menu),
 	 openedFolder(nullptr),
 	 averageHeight(0.0f),
 	 parent(nullptr){
	setVisible(false);
	setPosition(vec3(0,0,1));
}

void ButtonContextMenu::openSubMenu(ButtonContextMenu* menu){
	openedFolder = menu;
}

void ButtonContextMenu::executeLostFocus(){
	hide();
}

void ButtonContextMenu::add(UIElement *element){
	ElementCollection::add(element);
	float hw = std::max(getHW(), element->getHW());
	float hh = std::max(getHH(),0.0f)+element->getHH();
	averageHeight = hh/getElementCount();
	setSize(vec2(hw,hh));
    setHW2AllSlots(hw);
}

void ButtonContextMenu::remove(const std::string& name){
    ElementCollection::remove_if([&name](const UIElement* iter){
        const ButtonWrap* wrap = dynamic_cast<const ButtonWrap*>(iter);
        return (wrap!=nullptr && wrap->getText()==name);
    });
}

void ButtonContextMenu::viewAt(const glm::vec2& pnt, float hw){
	if(getElementCount()>0){
		setPosition2All(glm::vec3(pnt.x+(hw>0?hw:getHW()),pnt.y-getHH()/getElementCount(), 1),-1,hw);
		setVisible(true);
	}
}

void ButtonContextMenu::hide(){
	setVisible(false);
	hideOpenedFolder();
}

void ButtonContextMenu::hideAll(){
	ButtonContextMenu* current = this;
	while(current!=nullptr){
		current->hide();
		current=current->parent;
	}
}

void ButtonContextMenu::hideOpenedFolder(){
	if(openedFolder!=nullptr)
			openedFolder->hide();
	openedFolder=nullptr;
}

void ButtonContextMenu::setPosition2All(const glm::vec3& pnt, int direction,float hw){
	if(getSlotsSize()==0)
		return;

    getSlotElem(0)->setPosition(pnt);
	if(hw>0){
        getSlotElem(0)->setSize(vec2(hw,getSlotElem(0)->getHH()));
	}

	glm::vec3 currentCenter = pnt;
	for(size_t i=1;i< getSlotsSize();++i){
		currentCenter.y+=direction*(getSlotElem(i)->getHH()+getSlotElem(i-1)->getHH());
        getSlotElem(i)->setPosition(currentCenter);
		if(hw>0){
            getSlotElem(i)->setSize(vec2(hw,getSlotElem(i)->getHH()));
		}
	}

	float top = getSlotElem(0)->getPosition().y;
	float bottom = getSlotElem(getSlotsSize()-1)->getPosition().y;
	UIElement::setPosition(vec3(pnt.x,(top+bottom)*0.5,getPosition().z));
}

UIElement* ButtonContextMenu::add(const std::string& name){
	ButtonWrap* gb = new ButtonWrap(this, name);
	add(gb);
	gb->setSize(vec2(getHW(), getHH()/getElementCount()));
	return gb;
}

void ButtonContextMenu::addSubMenu(const std::string& name, ButtonContextMenu* subMenu){
	subMenu->parent = this;
	MenuFolder* gb = new MenuFolder(name, subMenu, this);
	add(gb);
	gb->setSize(vec2(getHW(), getHH()/getElementCount()));
}

void ButtonContextMenu::draw(){
	getDrawer()->draw();
}

void ButtonContextMenu::setSize(const glm::vec2& size){
	ElementCollection::setSize(size);
	if(drawer){
		drawer->setSize(size);
	}
}

ButtonContextMenu::AbstractDraw* ButtonContextMenu::getDrawer(){
	if(!drawer){
		if(laf.image.size()>0){
			drawer.reset(new DrawSprite(laf.image,laf.hoverImage,*this,getHW(),getHH()));
		}else{
			drawer.reset(new DrawSimple(*this));
		}
	}

	return drawer.get();
}