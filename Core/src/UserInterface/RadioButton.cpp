#include <ChibiEngine/UserInterface/Radio.h>
#include <ChibiEngine/XMLParser/UserInterfaceParser.h>
#include <glm/glm.hpp>

using namespace game;
using namespace std;
using namespace glm;

const std::string RadioButton::TYPE_NAME = "RadioButton";

RadioButton::RadioButton(const glm::vec3& pos,const std::string& name, const std::string& group, bool value)
:Checkbox(pos,name, value),
 groupName(group){
    Game::getUserInterface()->addEventListener(groupName,
            [this](const std::string& name){
                if(name!=getName()){
                    setChecked(false);
                }
            });
}

RadioButton::RadioButton(SlotType* iter):
        RadioButton(vec3(iter->x,iter->y,iter->z),
            iter->name,
            iter->choiceValue.radio->group,
            iter->choiceValue.radio->value){}

void RadioButton::executeAction(){
    setChecked(true);
    Game::getUserInterface()->fireEvent(groupName,getName());
}
