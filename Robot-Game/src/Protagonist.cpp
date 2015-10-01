#include <Protagonist.h>
#include <ChibiEngine/UserInterface/UserInterface.h>

using namespace std;
using namespace game;
using namespace glm;

Protagonist::Protagonist(const CharacterSettings &settings, glm::vec2 startPosition, const Category& category, Layer* layer, int uid)
    :Character(settings,startPosition,category,layer,uid){

    addSlot(nullptr);
}

Protagonist::~Protagonist(){
}

void Protagonist::hitted(float health){
    Character::hitted(health);
    Game::getUserInterface()->findElement("health_bar")->
            setValue(to_string(getHealth()));
}