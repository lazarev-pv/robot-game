#include <ChibiEngine/UserInterface/DropDownListBox.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>

using namespace game;
using namespace std;
using namespace glm;

const string DropDownListBox::TYPE_NAME = "DropDownListBox";

DropDownListBox::DropDownListBox(SlotType* iter)
:Button(iter->choiceValue.ddlb->text,vec3(iter->x,iter->y,iter->z)),
 listPart(new ButtonContextMenu()){

	setName(iter->name.size()>0?iter->name:iter->choiceValue.ddlb->text);
	listPart->setName((iter->name.size()>0?iter->name:iter->choiceValue.ddlb->text)+".menu");
	for(auto elIt : iter->choiceValue.ddlb->elem){
		listPart->add(elIt->name);
	}

	setSize(vec2(iter->hw>0?iter->hw:listPart->getHW(),
			iter->hh>0?iter->hh:getHH()));

    Game::getUserInterface()->addEventListener(listPart->getName(),
			[this](const std::string& name){
				setElem(name,true);
			});
}

DropDownListBox::DropDownListBox(const std::string& text)
	:Button(text),
	 listPart(new ButtonContextMenu()){
}

UIElement* DropDownListBox::add(const string& name){
	UIElement* el = listPart->add(name);
    Game::getUserInterface()->add(el);
	setSize(vec2(listPart->getHW(),listPart->getHH()/listPart->getElementCount()));
	return el;
}

void DropDownListBox::remove(const std::string& name){
	listPart->remove(name);
}

void DropDownListBox::setElem(const std::string& name, bool fireEvent){
	setValue(name);
	if(fireEvent){
        Game::getUserInterface()->fireEvent(getName(),name);
	}
}

void DropDownListBox::executeLostFocus(){
	Game::getEventSystem()->doNextFrame(bind(&ButtonContextMenu::hide, listPart), 1);
}
void DropDownListBox::add2UI(UserInterface* ui){
    UIElement::add2UI(ui);
    listPart->add2UI(ui);
}

void DropDownListBox::setVisible(bool flag){
	UIElement::setVisible(flag);
	listPart->setVisible(false);
}

void DropDownListBox::executeAction(){
	listPart->viewAt(vec2(getPosition().x-getHW(),getPosition().y+getHH()),getHW());
}

std::string DropDownListBox::getValue() const{
    return Button::getValue();
}
void DropDownListBox::setValue(const std::string& value){
    Button::setValue(value);
}

