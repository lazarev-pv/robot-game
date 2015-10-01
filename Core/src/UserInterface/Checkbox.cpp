#include <ChibiEngine/UserInterface/Checkbox.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/UserInterface/LookAndFeel.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Script/Script.h>
using namespace game;
using namespace std;
using namespace glm;

const string Checkbox::TYPE_NAME = "Checkbox";

Checkbox::Checkbox(SlotType* iter)
:Checkbox(vec3(iter->x,iter->y,iter->z),iter->name,iter->choiceValue.checkbox->value){
    Checkbox::script = iter->choiceValue.checkbox->script;
}

Checkbox::Checkbox(const glm::vec3& pos, const std::string& name, bool value)
:UIElement(),value(value){
    setName(name);
    const CheckboxLAF& laf = Game::getLookAndFeel().checkbox;
    UIElement::setSize(vec2(laf.hw, laf.hh));
    setPosition(pos);
}

void Checkbox::executeAction(){
	value=!value;
    Game::getScriptSystem()->doString(script);
    Game::getUserInterface()->fireEvent(getName(),value?"true":"false");
}

void Checkbox::draw(){
	const CheckboxLAF& laf = Game::getLookAndFeel().checkbox;
	if(value && laf.checked!=nullptr){
		laf.checked->draw(createMatrix(getPosition(),laf.checkedScale));
	}else if(!value && laf.normal!=nullptr){
		laf.normal->draw(createMatrix(getPosition(),laf.normalScale));
	}
}
