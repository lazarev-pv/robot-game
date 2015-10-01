#include <ChibiEngine/XMLParser/SettingsParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
CameraType::CameraType(){}

CameraType::CameraType(CameraType&& other)
	:near(other.near),far(other.far),x(other.x),y(other.y),z(other.z),fov(other.fov)
{
}
CameraType::CameraType(rapidxml::xml_node<>*  node){
	setValue(&near, node->first_attribute("near"));
	setValue(&far, node->first_attribute("far"));
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&z, node->first_attribute("z"));
	setValue(&fov, node->first_attribute("fov"));
}

CameraType::~CameraType(){
}

void CameraType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" near=\""<<near<<"\"";
		out<<" far=\""<<far<<"\"";
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" z=\""<<z<<"\"";
		out<<" fov=\""<<fov<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ClickedType::ClickedType(){}

ClickedType::ClickedType(ClickedType&& other)
	:code(other.code)
{
}
ClickedType::ClickedType(rapidxml::xml_node<>*  node){
	setValue(&code, node->first_attribute("code"));
}

ClickedType::~ClickedType(){
}

void ClickedType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" code=\""<<code<<"\"";
		out<<"/>";
	}
	out<<endl;
}

DescriptionType::DescriptionType()
	:file(""){}

DescriptionType::DescriptionType(DescriptionType&& other)
	:file(other.file)
{
}
DescriptionType::DescriptionType(rapidxml::xml_node<>*  node)
	:file(""){
	setValue(&file, node->first_attribute("file"));
}

DescriptionType::~DescriptionType(){
}

void DescriptionType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" file=\""<<file<<"\"";
		out<<"/>";
	}
	out<<endl;
}

GameStateType::GameStateType()
	:name(""),levelDescription(""){}

GameStateType::GameStateType(GameStateType&& other)
	:name(other.name),levelDescription(other.levelDescription)
{
}
GameStateType::GameStateType(rapidxml::xml_node<>*  node)
	:name(""),levelDescription(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&levelDescription, node->first_attribute("levelDescription"));
}

GameStateType::~GameStateType(){
}

void GameStateType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" levelDescription=\""<<levelDescription<<"\"";
		out<<"/>";
	}
	out<<endl;
}

GameStatesType::GameStatesType()
	:state(0x0),initial(""),userInterface(""),dataRootDir(""){}

GameStatesType::GameStatesType(GameStatesType&& other)
	:state(move(other.state)),initial(other.initial),userInterface(other.userInterface),dataRootDir(other.dataRootDir)
{
}
GameStatesType::GameStatesType(rapidxml::xml_node<>*  node)
	:state(0x0),initial(""),userInterface(""),dataRootDir(""){
	setValue(&initial, node->first_attribute("initial"));
	setValue(&userInterface, node->first_attribute("userInterface"));
	setValue(&dataRootDir, node->first_attribute("dataRootDir"));
	for(xml_node<>* currNode = node->first_node("state");
			currNode!=0;
			currNode = currNode->next_sibling("state")){
		state.push_back(new GameStateType(currNode));
	}
}

GameStatesType::~GameStatesType(){
	for(vector<GameStateType* >::iterator it = state.begin();
			it!=state.end();++it){
		delete *it;
	}
;}

void GameStatesType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" initial=\""<<initial<<"\"";
		out<<" userInterface=\""<<userInterface<<"\"";
		out<<" dataRootDir=\""<<dataRootDir<<"\"";
		out<<">\n";
	}
	for(auto iter:state){
		iter->save("state",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

InputConfigType::InputConfigType()
	:profile(0x0),activeProfile(""){}

InputConfigType::InputConfigType(InputConfigType&& other)
	:profile(move(other.profile)),activeProfile(other.activeProfile)
{
}
InputConfigType::InputConfigType(rapidxml::xml_node<>*  node)
	:profile(0x0),activeProfile(""){
	setValue(&activeProfile, node->first_attribute("activeProfile"));
	for(xml_node<>* currNode = node->first_node("profile");
			currNode!=0;
			currNode = currNode->next_sibling("profile")){
		profile.push_back(new ProfileType(currNode));
	}
}

InputConfigType::~InputConfigType(){
	for(vector<ProfileType* >::iterator it = profile.begin();
			it!=profile.end();++it){
		delete *it;
	}
;}

void InputConfigType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" activeProfile=\""<<activeProfile<<"\"";
		out<<">\n";
	}
	for(auto iter:profile){
		iter->save("profile",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

KeyType::KeyType()
	:event(""){}

KeyType::KeyType(KeyType&& other)
	:code(other.code),event(other.event)
{
}
KeyType::KeyType(rapidxml::xml_node<>*  node)
	:event(""){
	setValue(&code, node->first_attribute("code"));
	setValue(&event, node->first_attribute("event"));
}

KeyType::~KeyType(){
}

void KeyType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" code=\""<<code<<"\"";
		out<<" event=\""<<event<<"\"";
		out<<"/>";
	}
	out<<endl;
}

MouseInputType::MouseInputType()
	:event(""){}

MouseInputTypeUnion::MouseInputTypeUnion()
	:clicked(0x0){}

MouseInputType::MouseInputType(MouseInputType&& other)
	:choiceValue(move(other.choiceValue)),event(other.event){
}
MouseInputTypeUnion::MouseInputTypeUnion(MouseInputTypeUnion&& other)
	:move(other.move),clicked(other.clicked)
{
	other.clicked=nullptr;
}
MouseInputType::MouseInputType(rapidxml::xml_node<>*  node)
	:event(""){
	setValue(&event, node->first_attribute("event"));
	xml_node<>* choiceNode = node->first_node();
	if(choiceNode!=0){
		if(strcmp(choiceNode->name(),"move")==0){
			setValue(&choiceValue.move, choiceNode);
		}
		else if(strcmp(choiceNode->name(),"clicked")==0){
			create(&choiceValue.clicked, choiceNode);
		}
	}
}
MouseInputType::~MouseInputType(){
	delete choiceValue.clicked;
}

void MouseInputType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" event=\""<<event<<"\"";
		out<<">\n";
	}
		out<<nestPrefix;
		createTag(out,"move",choiceValue.move);
	if(choiceValue.clicked!=0x0){
		choiceValue.clicked->save("clicked",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

PhysicsType::PhysicsType()
	:gravity(0x0),calcTimeInterval(20),physicsStepDimension(0.001){}

PhysicsType::PhysicsType(PhysicsType&& other)
	:gravity(other.gravity),timeMultiply(other.timeMultiply),velocityIterations(other.velocityIterations),positionIterations(other.positionIterations),calcTimeInterval(other.calcTimeInterval),physicsStepDimension(other.physicsStepDimension)
{
	other.gravity=nullptr;
}
PhysicsType::PhysicsType(rapidxml::xml_node<>*  node)
	:gravity(0x0),calcTimeInterval(20),physicsStepDimension(0.001){
	setValue(&timeMultiply, node->first_attribute("timeMultiply"));
	setValue(&velocityIterations, node->first_attribute("velocityIterations"));
	setValue(&positionIterations, node->first_attribute("positionIterations"));
	setValue(&calcTimeInterval, node->first_attribute("calcTimeInterval"));
	setValue(&physicsStepDimension, node->first_attribute("physicsStepDimension"));
		create(&gravity, node->first_node("gravity"));
}

PhysicsType::~PhysicsType(){
	delete gravity;
}

void PhysicsType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" timeMultiply=\""<<timeMultiply<<"\"";
		out<<" velocityIterations=\""<<velocityIterations<<"\"";
		out<<" positionIterations=\""<<positionIterations<<"\"";
		out<<" calcTimeInterval=\""<<calcTimeInterval<<"\"";
		out<<" physicsStepDimension=\""<<physicsStepDimension<<"\"";
		out<<">\n";
	}
	if(gravity!=0x0){
		gravity->save("gravity",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ProfileType::ProfileType()
	:key(0x0),mouse(0x0),name(""){}

ProfileType::ProfileType(ProfileType&& other)
	:key(move(other.key)),mouse(move(other.mouse)),name(other.name)
{
}
ProfileType::ProfileType(rapidxml::xml_node<>*  node)
	:key(0x0),mouse(0x0),name(""){
	setValue(&name, node->first_attribute("name"));
	for(xml_node<>* currNode = node->first_node("key");
			currNode!=0;
			currNode = currNode->next_sibling("key")){
		key.push_back(new KeyType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("mouse");
			currNode!=0;
			currNode = currNode->next_sibling("mouse")){
		mouse.push_back(new MouseInputType(currNode));
	}
}

ProfileType::~ProfileType(){
	for(vector<KeyType* >::iterator it = key.begin();
			it!=key.end();++it){
		delete *it;
	}
;	for(vector<MouseInputType* >::iterator it = mouse.begin();
			it!=mouse.end();++it){
		delete *it;
	}
;}

void ProfileType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	for(auto iter:key){
		iter->save("key",out,nestingLevel+1);
	}
	for(auto iter:mouse){
		iter->save("mouse",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

PropertiesList::PropertiesList()
	:property(0x0){}

PropertiesList::PropertiesList(PropertiesList&& other)
	:property(move(other.property))
{
}
PropertiesList::PropertiesList(rapidxml::xml_node<>*  node)
	:property(0x0){
	for(xml_node<>* currNode = node->first_node("property");
			currNode!=0;
			currNode = currNode->next_sibling("property")){
		property.push_back(new PropertyType(currNode));
	}
}

PropertiesList::~PropertiesList(){
	for(vector<PropertyType* >::iterator it = property.begin();
			it!=property.end();++it){
		delete *it;
	}
;}

void PropertiesList::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:property){
		iter->save("property",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

PropertyType::PropertyType()
	:name(""),value(""){}

PropertyType::PropertyType(PropertyType&& other)
	:name(other.name),value(other.value)
{
}
PropertyType::PropertyType(rapidxml::xml_node<>*  node)
	:name(""),value(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&value, node->first_attribute("value"));
}

PropertyType::~PropertyType(){
}

void PropertyType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" value=\""<<value<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ResourcesListType::ResourcesListType()
	:description(0x0){}

ResourcesListType::ResourcesListType(ResourcesListType&& other)
	:description(move(other.description))
{
}
ResourcesListType::ResourcesListType(rapidxml::xml_node<>*  node)
	:description(0x0){
	for(xml_node<>* currNode = node->first_node("description");
			currNode!=0;
			currNode = currNode->next_sibling("description")){
		description.push_back(new DescriptionType(currNode));
	}
}

ResourcesListType::~ResourcesListType(){
	for(vector<DescriptionType* >::iterator it = description.begin();
			it!=description.end();++it){
		delete *it;
	}
;}

void ResourcesListType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:description){
		iter->save("description",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SavedDataType::SavedDataType()
	:path(""){}

SavedDataType::SavedDataType(SavedDataType&& other)
	:path(other.path)
{
}
SavedDataType::SavedDataType(rapidxml::xml_node<>*  node)
	:path(""){
	setValue(&path, node->first_attribute("path"));
}

SavedDataType::~SavedDataType(){
}

void SavedDataType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" path=\""<<path<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ScreenType::ScreenType()
	:icon(""),title(""),depthTest(false),blending(true),shaders(""){}

ScreenType::ScreenType(ScreenType&& other)
	:icon(other.icon),title(other.title),x(other.x),y(other.y),width(other.width),height(other.height),depthTest(other.depthTest),blending(other.blending),shaders(other.shaders)
{
}
ScreenType::ScreenType(rapidxml::xml_node<>*  node)
	:icon(""),title(""),depthTest(false),blending(true),shaders(""){
	setValue(&icon, node->first_attribute("icon"));
	setValue(&title, node->first_attribute("title"));
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&width, node->first_attribute("width"));
	setValue(&height, node->first_attribute("height"));
	setValue(&depthTest, node->first_attribute("depthTest"));
	setValue(&blending, node->first_attribute("blending"));
	setValue(&shaders, node->first_attribute("shaders"));
}

ScreenType::~ScreenType(){
}

void ScreenType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" icon=\""<<icon<<"\"";
		out<<" title=\""<<title<<"\"";
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" width=\""<<width<<"\"";
		out<<" height=\""<<height<<"\"";
		out<<" depthTest=\""<<depthTest<<"\"";
		out<<" blending=\""<<blending<<"\"";
		out<<" shaders=\""<<shaders<<"\"";
		out<<"/>";
	}
	out<<endl;
}

SettingsParserRoot::SettingsParserRoot()
	:settings(0x0){}

SettingsParserRoot::SettingsParserRoot(SettingsParserRoot&& other)
	:settings(other.settings)
{
	other.settings=nullptr;
}
SettingsParserRoot::SettingsParserRoot(rapidxml::xml_node<>*  node)
	:settings(0x0){
		create(&settings, node->first_node("settings"));
}

SettingsParserRoot::~SettingsParserRoot(){
	delete settings;
}

void SettingsParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(settings!=0x0){
		settings->save("settings",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SettingsType::SettingsType()
	:screen(0x0),physics(0x0),camera(0x0),gameStates(0x0),inputConfig(0x0),properties(0x0),resources(0x0){}

SettingsType::SettingsType(SettingsType&& other)
	:screen(other.screen),physics(other.physics),camera(other.camera),gameStates(other.gameStates),inputConfig(other.inputConfig),properties(other.properties),resources(other.resources)
{
	other.screen=nullptr;
	other.physics=nullptr;
	other.camera=nullptr;
	other.gameStates=nullptr;
	other.inputConfig=nullptr;
	other.properties=nullptr;
	other.resources=nullptr;
}
SettingsType::SettingsType(rapidxml::xml_node<>*  node)
	:screen(0x0),physics(0x0),camera(0x0),gameStates(0x0),inputConfig(0x0),properties(0x0),resources(0x0){
		create(&screen, node->first_node("screen"));
		create(&physics, node->first_node("physics"));
		create(&camera, node->first_node("camera"));
		create(&gameStates, node->first_node("gameStates"));
		create(&inputConfig, node->first_node("inputConfig"));
		create(&properties, node->first_node("properties"));
		create(&resources, node->first_node("resources"));
}

SettingsType::~SettingsType(){
	delete screen;
	delete physics;
	delete camera;
	delete gameStates;
	delete inputConfig;
	delete properties;
	delete resources;
}

void SettingsType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(screen!=0x0){
		screen->save("screen",out,nestingLevel+1);
	}
	if(physics!=0x0){
		physics->save("physics",out,nestingLevel+1);
	}
	if(camera!=0x0){
		camera->save("camera",out,nestingLevel+1);
	}
	if(gameStates!=0x0){
		gameStates->save("gameStates",out,nestingLevel+1);
	}
	if(inputConfig!=0x0){
		inputConfig->save("inputConfig",out,nestingLevel+1);
	}
	if(properties!=0x0){
		properties->save("properties",out,nestingLevel+1);
	}
	if(resources!=0x0){
		resources->save("resources",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

Vector2D::Vector2D(){}

Vector2D::Vector2D(Vector2D&& other)
	:x(other.x),y(other.y)
{
}
Vector2D::Vector2D(rapidxml::xml_node<>*  node){
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
}

Vector2D::~Vector2D(){
}

void Vector2D::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<"/>";
	}
	out<<endl;
}

