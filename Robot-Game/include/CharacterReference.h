#ifndef __REFERENCE_CHARACTER_HEADER_
#define __REFERENCE_CHARACTER_HEADER_

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <ChibiEngine/Log/Logger.h>

class CharacterSettingsType;
class ConfigType;
class CharacterParserRoot;
class WeaponType;
namespace game {

    class Character;
    class Category;
    class Layer;
    class AICharacter;
    class Weapon;
    class Logger;

    struct WeaponSettings{
        WeaponSettings(WeaponType* data);
        WeaponSettings();
        std::string name,collideSensor;
        float damage, firingRate;
    };

    struct CharacterSettings{
        CharacterSettings(CharacterSettingsType* iter, const std::string &relativePath);
        CharacterSettings();
        std::string name,file;
        float health, jumpVelocity, runVelocity, animationMultiplier;
        CharacterParserRoot* animData;
        std::vector<WeaponSettings> weapons;
    };

    class CharacterReference {
    public:
        CharacterReference(ConfigType *settings, const std::string &relativePath);
        ~CharacterReference();

        Character *createChar(int uid,const std::string &name, glm::vec2 startPosition, const Category& category, Layer* layer);
        AICharacter *createEnemy(int uid,const std::string &name, glm::vec2 startPosition, Layer* layer);
        Weapon* createWeapon(const std::string& name, Character* character);

    private:
        Logger logger;
        ConfigType *settings;
        std::string relativePath;
        std::unordered_map<std::string, CharacterSettings> reference;
        std::unordered_map<std::string, WeaponSettings> commonWeapons;
    };

}

#endif