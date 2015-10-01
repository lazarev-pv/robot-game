#ifndef __SCHEMAS_SETTINGS_XSD
#define __SCHEMAS_SETTINGS_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>

// ----- DECLARAIONS ------ //
struct CameraType;
struct ClickedType;
struct DescriptionType;
struct GameStateType;
struct GameStatesType;
struct InputConfigType;
struct KeyType;
struct MouseInputType;
struct PhysicsType;
struct ProfileType;
struct PropertiesList;
struct PropertyType;
struct ResourcesListType;
struct SavedDataType;
struct ScreenType;
struct SettingsParserRoot;
struct SettingsType;
struct Vector2D;
// ----- DEFINITIONS ------ //

// ----- CameraType
struct CameraType{
	CameraType();
	CameraType(CameraType&& other);
	CameraType(const CameraType& other) = delete;
	CameraType& operator=(const CameraType& other) = delete;
	CameraType(rapidxml::xml_node<>* node);
	~CameraType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float near;
	float far;
	float x;
	float y;
	float z;
	float fov;

};

// ----- ClickedType
struct ClickedType{
	ClickedType();
	ClickedType(ClickedType&& other);
	ClickedType(const ClickedType& other) = delete;
	ClickedType& operator=(const ClickedType& other) = delete;
	ClickedType(rapidxml::xml_node<>* node);
	~ClickedType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	int code;

};

// ----- DescriptionType
struct DescriptionType{
	DescriptionType();
	DescriptionType(DescriptionType&& other);
	DescriptionType(const DescriptionType& other) = delete;
	DescriptionType& operator=(const DescriptionType& other) = delete;
	DescriptionType(rapidxml::xml_node<>* node);
	~DescriptionType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string file;

};

// ----- GameStateType
struct GameStateType{
	GameStateType();
	GameStateType(GameStateType&& other);
	GameStateType(const GameStateType& other) = delete;
	GameStateType& operator=(const GameStateType& other) = delete;
	GameStateType(rapidxml::xml_node<>* node);
	~GameStateType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	std::string levelDescription;

};

// ----- GameStatesType
struct GameStatesType{
	GameStatesType();
	GameStatesType(GameStatesType&& other);
	GameStatesType(const GameStatesType& other) = delete;
	GameStatesType& operator=(const GameStatesType& other) = delete;
	GameStatesType(rapidxml::xml_node<>* node);
	~GameStatesType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<GameStateType* > state;
	std::string initial;
	std::string userInterface;
	std::string dataRootDir;

};

// ----- InputConfigType
struct InputConfigType{
	InputConfigType();
	InputConfigType(InputConfigType&& other);
	InputConfigType(const InputConfigType& other) = delete;
	InputConfigType& operator=(const InputConfigType& other) = delete;
	InputConfigType(rapidxml::xml_node<>* node);
	~InputConfigType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ProfileType* > profile;
	std::string activeProfile;

};

// ----- KeyType
struct KeyType{
	KeyType();
	KeyType(KeyType&& other);
	KeyType(const KeyType& other) = delete;
	KeyType& operator=(const KeyType& other) = delete;
	KeyType(rapidxml::xml_node<>* node);
	~KeyType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	int code;
	std::string event;

};

// ----- MouseInputType
struct MouseInputTypeUnion {
	MouseInputTypeUnion();
	MouseInputTypeUnion(MouseInputTypeUnion&& other);
	std::string move;
	struct ClickedType* clicked;
};
struct MouseInputType{
	MouseInputType();
	MouseInputType(MouseInputType&& other);
	MouseInputType(const MouseInputType& other) = delete;
	MouseInputType& operator=(const MouseInputType& other) = delete;
	MouseInputType(rapidxml::xml_node<>* node);
	~MouseInputType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	MouseInputTypeUnion choiceValue;

	std::string event;
};

// ----- PhysicsType
struct PhysicsType{
	PhysicsType();
	PhysicsType(PhysicsType&& other);
	PhysicsType(const PhysicsType& other) = delete;
	PhysicsType& operator=(const PhysicsType& other) = delete;
	PhysicsType(rapidxml::xml_node<>* node);
	~PhysicsType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Vector2D*  gravity;
	float timeMultiply;
	int velocityIterations;
	int positionIterations;
	int calcTimeInterval;
	float physicsStepDimension;

};

// ----- ProfileType
struct ProfileType{
	ProfileType();
	ProfileType(ProfileType&& other);
	ProfileType(const ProfileType& other) = delete;
	ProfileType& operator=(const ProfileType& other) = delete;
	ProfileType(rapidxml::xml_node<>* node);
	~ProfileType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<KeyType* > key;
	std::vector<MouseInputType* > mouse;
	std::string name;

};

// ----- PropertiesList
struct PropertiesList{
	PropertiesList();
	PropertiesList(PropertiesList&& other);
	PropertiesList(const PropertiesList& other) = delete;
	PropertiesList& operator=(const PropertiesList& other) = delete;
	PropertiesList(rapidxml::xml_node<>* node);
	~PropertiesList();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<PropertyType* > property;

};

// ----- PropertyType
struct PropertyType{
	PropertyType();
	PropertyType(PropertyType&& other);
	PropertyType(const PropertyType& other) = delete;
	PropertyType& operator=(const PropertyType& other) = delete;
	PropertyType(rapidxml::xml_node<>* node);
	~PropertyType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	std::string value;

};

// ----- ResourcesListType
struct ResourcesListType{
	ResourcesListType();
	ResourcesListType(ResourcesListType&& other);
	ResourcesListType(const ResourcesListType& other) = delete;
	ResourcesListType& operator=(const ResourcesListType& other) = delete;
	ResourcesListType(rapidxml::xml_node<>* node);
	~ResourcesListType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<DescriptionType* > description;

};

// ----- SavedDataType
struct SavedDataType{
	SavedDataType();
	SavedDataType(SavedDataType&& other);
	SavedDataType(const SavedDataType& other) = delete;
	SavedDataType& operator=(const SavedDataType& other) = delete;
	SavedDataType(rapidxml::xml_node<>* node);
	~SavedDataType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string path;

};

// ----- ScreenType
struct ScreenType{
	ScreenType();
	ScreenType(ScreenType&& other);
	ScreenType(const ScreenType& other) = delete;
	ScreenType& operator=(const ScreenType& other) = delete;
	ScreenType(rapidxml::xml_node<>* node);
	~ScreenType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string icon;
	std::string title;
	int x;
	int y;
	int width;
	int height;
	bool depthTest;
	bool blending;
	std::string shaders;

};

// ----- SettingsParserRoot
struct SettingsParserRoot{
	SettingsParserRoot();
	SettingsParserRoot(SettingsParserRoot&& other);
	SettingsParserRoot(const SettingsParserRoot& other) = delete;
	SettingsParserRoot& operator=(const SettingsParserRoot& other) = delete;
	SettingsParserRoot(rapidxml::xml_node<>* node);
	~SettingsParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	SettingsType*  settings;

};

// ----- SettingsType
struct SettingsType{
	SettingsType();
	SettingsType(SettingsType&& other);
	SettingsType(const SettingsType& other) = delete;
	SettingsType& operator=(const SettingsType& other) = delete;
	SettingsType(rapidxml::xml_node<>* node);
	~SettingsType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ScreenType*  screen;
	PhysicsType*  physics;
	CameraType*  camera;
	GameStatesType*  gameStates;
	InputConfigType*  inputConfig;
	PropertiesList*  properties;
	ResourcesListType*  resources;

};

// ----- Vector2D
struct Vector2D{
	Vector2D();
	Vector2D(Vector2D&& other);
	Vector2D(const Vector2D& other) = delete;
	Vector2D& operator=(const Vector2D& other) = delete;
	Vector2D(rapidxml::xml_node<>* node);
	~Vector2D();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float x;
	float y;

};
#endif
