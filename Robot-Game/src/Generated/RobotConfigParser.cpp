#include <cstring>
#include "RobotConfigParser.h"
#include "util.h"
using namespace rapidxml;
using namespace std;
CameraConfigType::CameraConfigType(){}

CameraConfigType::CameraConfigType(CameraConfigType&& other)
	:xVelocity(other.xVelocity),yVelocity(other.yVelocity)
{
}
CameraConfigType::CameraConfigType(rapidxml::xml_node<>*  node){
	setValue(&xVelocity, node->first_attribute("xVelocity"));
	setValue(&yVelocity, node->first_attribute("yVelocity"));
}

CameraConfigType::~CameraConfigType(){
}

void CameraConfigType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" xVelocity=\""<<xVelocity<<"\"";
		out<<" yVelocity=\""<<yVelocity<<"\"";
		out<<"/>";
	}
	out<<endl;
}

CharacterSettingsType::CharacterSettingsType()
	:weapon(0x0),name(""),file(""){}

CharacterSettingsType::CharacterSettingsType(CharacterSettingsType&& other)
	:weapon(move(other.weapon)),name(other.name),file(other.file),health(other.health),jumpVelocity(other.jumpVelocity),runVelocity(other.runVelocity),animationMultiplier(other.animationMultiplier)
{
}
CharacterSettingsType::CharacterSettingsType(rapidxml::xml_node<>*  node)
	:weapon(0x0),name(""),file(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&file, node->first_attribute("file"));
	setValue(&health, node->first_attribute("health"));
	setValue(&jumpVelocity, node->first_attribute("jumpVelocity"));
	setValue(&runVelocity, node->first_attribute("runVelocity"));
	setValue(&animationMultiplier, node->first_attribute("animationMultiplier"));
	for(xml_node<>* currNode = node->first_node("weapon");
			currNode!=0;
			currNode = currNode->next_sibling("weapon")){
		weapon.push_back(new WeaponType(currNode));
	}
}

CharacterSettingsType::~CharacterSettingsType(){
	for(vector<WeaponType* >::iterator it = weapon.begin();
			it!=weapon.end();++it){
		delete *it;
	}
;}

void CharacterSettingsType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" file=\""<<file<<"\"";
		out<<" health=\""<<health<<"\"";
		out<<" jumpVelocity=\""<<jumpVelocity<<"\"";
		out<<" runVelocity=\""<<runVelocity<<"\"";
		out<<" animationMultiplier=\""<<animationMultiplier<<"\"";
		out<<">\n";
	}
	for(auto iter:weapon){
		iter->save("weapon",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CharactersType::CharactersType()
	:character(0x0){}

CharactersType::CharactersType(CharactersType&& other)
	:character(move(other.character))
{
}
CharactersType::CharactersType(rapidxml::xml_node<>*  node)
	:character(0x0){
	for(xml_node<>* currNode = node->first_node("character");
			currNode!=0;
			currNode = currNode->next_sibling("character")){
		character.push_back(new CharacterSettingsType(currNode));
	}
}

CharactersType::~CharactersType(){
	for(vector<CharacterSettingsType* >::iterator it = character.begin();
			it!=character.end();++it){
		delete *it;
	}
;}

void CharactersType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:character){
		iter->save("character",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ColdStateType::ColdStateType(){}

ColdStateType::ColdStateType(ColdStateType&& other)
	:duration(other.duration),p(other.p),slowdown(other.slowdown)
{
}
ColdStateType::ColdStateType(rapidxml::xml_node<>*  node){
	setValue(&duration, node->first_attribute("duration"));
	setValue(&p, node->first_attribute("p"));
	setValue(&slowdown, node->first_attribute("slowdown"));
}

ColdStateType::~ColdStateType(){
}

void ColdStateType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" duration=\""<<duration<<"\"";
		out<<" p=\""<<p<<"\"";
		out<<" slowdown=\""<<slowdown<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ConfigType::ConfigType()
	:weapons(0x0),characters(0x0),camera(0x0){}

ConfigType::ConfigType(ConfigType&& other)
	:weapons(other.weapons),characters(other.characters),camera(other.camera)
{
	other.weapons=nullptr;
	other.characters=nullptr;
	other.camera=nullptr;
}
ConfigType::ConfigType(rapidxml::xml_node<>*  node)
	:weapons(0x0),characters(0x0),camera(0x0){
		create(&weapons, node->first_node("weapons"));
		create(&characters, node->first_node("characters"));
		create(&camera, node->first_node("camera"));
}

ConfigType::~ConfigType(){
	delete weapons;
	delete characters;
	delete camera;
}

void ConfigType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(weapons!=0x0){
		weapons->save("weapons",out,nestingLevel+1);
	}
	if(characters!=0x0){
		characters->save("characters",out,nestingLevel+1);
	}
	if(camera!=0x0){
		camera->save("camera",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

FireStateType::FireStateType(){}

FireStateType::FireStateType(FireStateType&& other)
	:duration(other.duration),p(other.p)
{
}
FireStateType::FireStateType(rapidxml::xml_node<>*  node){
	setValue(&duration, node->first_attribute("duration"));
	setValue(&p, node->first_attribute("p"));
}

FireStateType::~FireStateType(){
}

void FireStateType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" duration=\""<<duration<<"\"";
		out<<" p=\""<<p<<"\"";
		out<<"/>";
	}
	out<<endl;
}

RobotConfigParserRoot::RobotConfigParserRoot()
	:config(0x0){}

RobotConfigParserRoot::RobotConfigParserRoot(RobotConfigParserRoot&& other)
	:config(other.config)
{
	other.config=nullptr;
}
RobotConfigParserRoot::RobotConfigParserRoot(rapidxml::xml_node<>*  node)
	:config(0x0){
		create(&config, node->first_node("config"));
}

RobotConfigParserRoot::~RobotConfigParserRoot(){
	delete config;
}

void RobotConfigParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(config!=0x0){
		config->save("config",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ShockStateType::ShockStateType(){}

ShockStateType::ShockStateType(ShockStateType&& other)
	:duration(other.duration),p(other.p)
{
}
ShockStateType::ShockStateType(rapidxml::xml_node<>*  node){
	setValue(&duration, node->first_attribute("duration"));
	setValue(&p, node->first_attribute("p"));
}

ShockStateType::~ShockStateType(){
}

void ShockStateType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" duration=\""<<duration<<"\"";
		out<<" p=\""<<p<<"\"";
		out<<"/>";
	}
	out<<endl;
}

WeaponType::WeaponType()
	:upgrade(0x0),name(""),collideSensor(""){}

WeaponType::WeaponType(WeaponType&& other)
	:upgrade(move(other.upgrade)),name(other.name),damage(other.damage),firingRate(other.firingRate),closeHanded(other.closeHanded),collideSensor(other.collideSensor)
{
}
WeaponType::WeaponType(rapidxml::xml_node<>*  node)
	:upgrade(0x0),name(""),collideSensor(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&damage, node->first_attribute("damage"));
	setValue(&firingRate, node->first_attribute("firingRate"));
	setValue(&closeHanded, node->first_attribute("closeHanded"));
	setValue(&collideSensor, node->first_attribute("collideSensor"));
	for(xml_node<>* currNode = node->first_node("upgrade");
			currNode!=0;
			currNode = currNode->next_sibling("upgrade")){
		upgrade.push_back(new WeaponUpgradeType(currNode));
	}
}

WeaponType::~WeaponType(){
	for(vector<WeaponUpgradeType* >::iterator it = upgrade.begin();
			it!=upgrade.end();++it){
		delete *it;
	}
;}

void WeaponType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" damage=\""<<damage<<"\"";
		out<<" firingRate=\""<<firingRate<<"\"";
		out<<" closeHanded=\""<<closeHanded<<"\"";
		out<<" collideSensor=\""<<collideSensor<<"\"";
		out<<">\n";
	}
	for(auto iter:upgrade){
		iter->save("upgrade",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

WeaponUpgradeType::WeaponUpgradeType()
	:fire(0x0),cold(0x0),shock(0x0){}

WeaponUpgradeType::WeaponUpgradeType(WeaponUpgradeType&& other)
	:fire(other.fire),cold(other.cold),shock(other.shock),cost(other.cost),tier(other.tier)
{
	other.fire=nullptr;
	other.cold=nullptr;
	other.shock=nullptr;
}
WeaponUpgradeType::WeaponUpgradeType(rapidxml::xml_node<>*  node)
	:fire(0x0),cold(0x0),shock(0x0){
	setValue(&cost, node->first_attribute("cost"));
	setValue(&tier, node->first_attribute("tier"));
		create(&fire, node->first_node("fire"));
		create(&cold, node->first_node("cold"));
		create(&shock, node->first_node("shock"));
}

WeaponUpgradeType::~WeaponUpgradeType(){
	delete fire;
	delete cold;
	delete shock;
}

void WeaponUpgradeType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" cost=\""<<cost<<"\"";
		out<<" tier=\""<<tier<<"\"";
		out<<">\n";
	}
	if(fire!=0x0){
		fire->save("fire",out,nestingLevel+1);
	}
	if(cold!=0x0){
		cold->save("cold",out,nestingLevel+1);
	}
	if(shock!=0x0){
		shock->save("shock",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

WeaponsType::WeaponsType()
	:weapon(0x0){}

WeaponsType::WeaponsType(WeaponsType&& other)
	:weapon(move(other.weapon))
{
}
WeaponsType::WeaponsType(rapidxml::xml_node<>*  node)
	:weapon(0x0){
	for(xml_node<>* currNode = node->first_node("weapon");
			currNode!=0;
			currNode = currNode->next_sibling("weapon")){
		weapon.push_back(new WeaponType(currNode));
	}
}

WeaponsType::~WeaponsType(){
	for(vector<WeaponType* >::iterator it = weapon.begin();
			it!=weapon.end();++it){
		delete *it;
	}
;}

void WeaponsType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:weapon){
		iter->save("weapon",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

