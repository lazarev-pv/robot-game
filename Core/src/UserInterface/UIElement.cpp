#include <ChibiEngine/UserInterface/UserInterface.h>
#include <ChibiEngine/Log/Logger.h>

using namespace game;
using namespace glm;
using namespace std;

const string UIElement::TYPE_NAME = "UIElement";


UIElement::UIElemParms::UIElemParms()
	:pos(0,0,0.5),
	 hw(-1),
	 hh(-1),
	 name(),
	 visible(true),
	 enabled(true){}

UIElement::UIElemParms::UIElemParms(const std::string& name,
		float x, float y, float z,
		float hw, float hh,
		bool visible,
		bool enabled)
			:pos(x,y,z),
			 hw(hw),
			 hh(hh),
			 name(name),
			 visible(visible),
			 enabled(enabled){}

UIElement::UIElement():
		parms(),
		instantAction(false),
        managedByUI(false),
        hovered(false){

}
        
UIElement::~UIElement(){}

void UIElement::executeAction(){}
void UIElement::executePushOnAnimation(){}
void UIElement::executePushOffAnimation(){}
void UIElement::executeHoverOnAnimation(){
    hovered = true;
}
void UIElement::executeHoverOutAnimation(){
    hovered = false;
}

bool UIElement::executeScroll(bool /*up*/){
    return false;
}

void UIElement::processDraggingAction(const glm::vec2& ){}
void UIElement::onKeyPressed(int , bool ){}

void UIElement::add2UI(UserInterface* ui){
	ui->add(this);
    managedByUI=true;
}

bool UIElement::isVisible(){
	return parms.visible;
}

void UIElement::setSize(const glm::vec2& size){
	this->parms.hw=size.x;
	this->parms.hh=size.y;
}

void UIElement::setVisible(bool flag){
	Game::getUserInterface()->getLogger().trace("[%s] setted to [%s]", getName().c_str(), flag ? "true" : "false");
	parms.visible = flag;
}

bool UIElement::isEnabled(){
	return parms.enabled;
}

void UIElement::setPosition(const glm::vec3& pos){
    parms.pos=pos;
}

const glm::vec3& UIElement::getPosition() const{
	return parms.pos;
}

void UIElement::move(const glm::vec3 &delta){
    parms.pos+=delta;
}

UIElement* UIElement::containPoint(const glm::vec2& pnt){
    return parms.containPoint(pnt)? this : nullptr;
}

/* Perform action on click down - else on click up*/
bool UIElement::isInstantAction(){
    return instantAction;
}

glm::vec2 UIElement::getSize(){
	return glm::vec2(parms.hw, parms.hh);
}

const std::string& UIElement::getName() const{
	return parms.name;
}

float UIElement::getHW(){
	return parms.hw;
}

float UIElement::getHH(){
	return parms.hh;
}
/*

void UIElement::setValue(const std::string& ){}
std::string UIElement::getValue() const{return "";}*/
