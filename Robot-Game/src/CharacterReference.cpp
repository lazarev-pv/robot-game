#include <Protagonist.h>
#include <AICharacter.h>
#include <CharacterReference.h>
#include "Generated/RobotConfigParser.h"
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <RobotLevel.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

CharacterSettings::CharacterSettings(){}

CharacterSettings::CharacterSettings(CharacterSettingsType* iter, const std::string &relativePath):
        name(iter->name),
        file(relativePath+iter->file),
        health(iter->health),
        jumpVelocity(iter->jumpVelocity),
        runVelocity(iter->runVelocity),
        animationMultiplier(iter->animationMultiplier)
    {

        animData = new CharacterParserRoot(createRoot<CharacterParserRoot>(file));
        Game::getResources()->load(animData->character->resources, file);

        for(WeaponType* it : iter->weapon){
            weapons.push_back(it);
        }
}

WeaponSettings::WeaponSettings(WeaponType* data):
    name(data->name),
    collideSensor(data->collideSensor),
    damage(data->damage),
    firingRate(data->firingRate){
}

WeaponSettings::WeaponSettings(){}

CharacterReference::CharacterReference(ConfigType* settings, const std::string& relativePath)
        :logger(Game::getLoggingSystem()->createLogger("CharacterReference")),
         settings(settings),
         relativePath(relativePath+"/"){
    for(CharacterSettingsType* iter : settings->characters->character){
        reference[iter->name]=CharacterSettings(iter,this->relativePath);
        Game::getLogger().debug("Created char " + iter->name);
    }

    for(WeaponType* it : settings->weapons->weapon){
        commonWeapons[it->name] = WeaponSettings(it);
    }
}

CharacterReference::~CharacterReference(){
    for(auto i : reference){
        delete i.second.animData;
    }
}

Character* CharacterReference::createChar(int uid, const std::string& name, glm::vec2 startPosition,  const Category& category, Layer* layer){
    auto it = reference.find(name);
    if(it==reference.end()){
        logger.error("Character %s not found", name.c_str());
    }
    return new Protagonist(it->second,startPosition,category,layer,uid);
}

AICharacter* CharacterReference::createEnemy(int uid, const std::string& name, glm::vec2 startPosition, Layer* layer){
    auto it = reference.find(name);
    if(it==reference.end()){
        logger.error("Character %s not found", name.c_str());
    }
    Game::getScriptSystem()->loadFile(FileSystem::getPath(it->second.file)+"/ai.lua");
    return new AICharacter(it->second,startPosition,layer,uid);
}

Weapon* CharacterReference::createWeapon(const std::string& name, Character* character){
    auto iter = commonWeapons.find(name);
    if(iter==commonWeapons.end()){
        logger.error("Weapon [%s] not found", name.c_str());
        return nullptr;
    }
    return new Weapon(&(iter->second),character);
}
