#include <ChibiEngine/Physics/CategoryReference.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/Log/Logger.h>
using namespace glm;
using namespace std;
using namespace game;

static uint16 _PHYS = 0x0001;
static uint16 _ENEMY = 0x0002;
static uint16 _DECORATION = 0x0004;
static uint16 _PROTAGONIST = 0x0008;
static uint16 _INTERACTIVE = 0x0010;
static uint16 _WEAPON_SENSOR = 0x0020;

const std::string CategoryReference::PROTAGONIST="protagonist";
const std::string CategoryReference::ENEMY="enemy";
const std::string CategoryReference::DECORATION="decoration";
const std::string CategoryReference::PHYS="phys";
const std::string CategoryReference::INTERACTIVE="interactive";
const std::string CategoryReference::PROTAGONIST_WEAPON_SENSOR ="pweapon";
const std::string CategoryReference::ENEMY_WEAPON_SENSOR ="eweapon";

const Category CategoryReference::nullCategory{};

Category::Category(uint16 category, uint16 mask, const std::string& name)
    :category(category),mask(mask),name(name){}

Category::Category():Category(0,0,""){}

CategoryReference::CategoryReference(){
    reference[PROTAGONIST]=Category(_PROTAGONIST,_PHYS|_INTERACTIVE/*|_ENEMY*/,PROTAGONIST);
    reference[PROTAGONIST_WEAPON_SENSOR]=Category(_WEAPON_SENSOR,_PHYS|_ENEMY,PROTAGONIST_WEAPON_SENSOR);

    reference[ENEMY]=Category(_ENEMY,_PHYS|_WEAPON_SENSOR/*|_PROTAGONIST*/,ENEMY);
    reference[ENEMY_WEAPON_SENSOR]=Category(_INTERACTIVE,_PROTAGONIST,ENEMY_WEAPON_SENSOR);

    reference[DECORATION]=Category(_DECORATION,_DECORATION|_PHYS,DECORATION);
    reference[PHYS]=Category(_PHYS,_DECORATION|_PHYS|_PROTAGONIST|_ENEMY|_WEAPON_SENSOR,PHYS);
    reference[INTERACTIVE]=Category(_INTERACTIVE,_PROTAGONIST,INTERACTIVE);
}

const Category& CategoryReference::get(const std::string& name){
    static CategoryReference instance;

    auto ref = instance.reference.find(name);
    if(ref==instance.reference.end()){
        Game::getPhysicsSystem()->getLogger().error("Category %s not found", name.c_str());
        return nullCategory;
    }
    return ref->second;
}