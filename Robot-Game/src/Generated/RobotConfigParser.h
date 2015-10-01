#ifndef __SCHEMAS_ROBOT_CONFIG_XSD
#define __SCHEMAS_ROBOT_CONFIG_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>

// ----- DECLARAIONS ------ //
struct CameraConfigType;
struct CharacterSettingsType;
struct CharactersType;
struct ColdStateType;
struct ConfigType;
struct FireStateType;
struct RobotConfigParserRoot;
struct ShockStateType;
struct WeaponType;
struct WeaponUpgradeType;
struct WeaponsType;
// ----- DEFINITIONS ------ //

// ----- CameraConfigType
struct CameraConfigType{
	CameraConfigType();
	CameraConfigType(CameraConfigType&& other);
	CameraConfigType(const CameraConfigType& other) = delete;
	CameraConfigType& operator=(const CameraConfigType& other) = delete;
	CameraConfigType(rapidxml::xml_node<>* node);
	~CameraConfigType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float xVelocity;
	float yVelocity;

};

// ----- CharacterSettingsType
struct CharacterSettingsType{
	CharacterSettingsType();
	CharacterSettingsType(CharacterSettingsType&& other);
	CharacterSettingsType(const CharacterSettingsType& other) = delete;
	CharacterSettingsType& operator=(const CharacterSettingsType& other) = delete;
	CharacterSettingsType(rapidxml::xml_node<>* node);
	~CharacterSettingsType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<WeaponType* > weapon;
	std::string name;
	std::string file;
	float health;
	float jumpVelocity;
	float runVelocity;
	float animationMultiplier;

};

// ----- CharactersType
struct CharactersType{
	CharactersType();
	CharactersType(CharactersType&& other);
	CharactersType(const CharactersType& other) = delete;
	CharactersType& operator=(const CharactersType& other) = delete;
	CharactersType(rapidxml::xml_node<>* node);
	~CharactersType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<CharacterSettingsType* > character;

};

// ----- ColdStateType
struct ColdStateType{
	ColdStateType();
	ColdStateType(ColdStateType&& other);
	ColdStateType(const ColdStateType& other) = delete;
	ColdStateType& operator=(const ColdStateType& other) = delete;
	ColdStateType(rapidxml::xml_node<>* node);
	~ColdStateType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float duration;
	float p;
	float slowdown;

};

// ----- ConfigType
struct ConfigType{
	ConfigType();
	ConfigType(ConfigType&& other);
	ConfigType(const ConfigType& other) = delete;
	ConfigType& operator=(const ConfigType& other) = delete;
	ConfigType(rapidxml::xml_node<>* node);
	~ConfigType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	WeaponsType*  weapons;
	CharactersType*  characters;
	CameraConfigType*  camera;

};

// ----- FireStateType
struct FireStateType{
	FireStateType();
	FireStateType(FireStateType&& other);
	FireStateType(const FireStateType& other) = delete;
	FireStateType& operator=(const FireStateType& other) = delete;
	FireStateType(rapidxml::xml_node<>* node);
	~FireStateType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float duration;
	float p;

};

// ----- RobotConfigParserRoot
struct RobotConfigParserRoot{
	RobotConfigParserRoot();
	RobotConfigParserRoot(RobotConfigParserRoot&& other);
	RobotConfigParserRoot(const RobotConfigParserRoot& other) = delete;
	RobotConfigParserRoot& operator=(const RobotConfigParserRoot& other) = delete;
	RobotConfigParserRoot(rapidxml::xml_node<>* node);
	~RobotConfigParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ConfigType*  config;

};

// ----- ShockStateType
struct ShockStateType{
	ShockStateType();
	ShockStateType(ShockStateType&& other);
	ShockStateType(const ShockStateType& other) = delete;
	ShockStateType& operator=(const ShockStateType& other) = delete;
	ShockStateType(rapidxml::xml_node<>* node);
	~ShockStateType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float duration;
	float p;

};

// ----- WeaponType
struct WeaponType{
	WeaponType();
	WeaponType(WeaponType&& other);
	WeaponType(const WeaponType& other) = delete;
	WeaponType& operator=(const WeaponType& other) = delete;
	WeaponType(rapidxml::xml_node<>* node);
	~WeaponType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<WeaponUpgradeType* > upgrade;
	std::string name;
	float damage;
	float firingRate;
	bool closeHanded;
	std::string collideSensor;

};

// ----- WeaponUpgradeType
struct WeaponUpgradeType{
	WeaponUpgradeType();
	WeaponUpgradeType(WeaponUpgradeType&& other);
	WeaponUpgradeType(const WeaponUpgradeType& other) = delete;
	WeaponUpgradeType& operator=(const WeaponUpgradeType& other) = delete;
	WeaponUpgradeType(rapidxml::xml_node<>* node);
	~WeaponUpgradeType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	FireStateType*  fire;
	ColdStateType*  cold;
	ShockStateType*  shock;
	float cost;
	int tier;

};

// ----- WeaponsType
struct WeaponsType{
	WeaponsType();
	WeaponsType(WeaponsType&& other);
	WeaponsType(const WeaponsType& other) = delete;
	WeaponsType& operator=(const WeaponsType& other) = delete;
	WeaponsType(rapidxml::xml_node<>* node);
	~WeaponsType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<WeaponType* > weapon;

};
#endif
