#ifndef __SCHEMAS_WORLD_XSD
#define __SCHEMAS_WORLD_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include "CommonParser.h"
#include "ResourcesDescriptionParser.h"

// ----- DECLARAIONS ------ //
struct ArtType;
struct Box3DType;
struct DirLightType;
struct ERBoxType;
struct ERChainType;
struct EnemyList;
struct EnemyType;
struct EventType;
struct FXListType;
struct FXType;
struct GeometryList;
struct GeometrySlot;
struct JointType;
struct LevelSettings;
struct LightListType;
struct LightType;
struct NamedPoint2D;
struct Obj3DType;
struct Reference;
struct RobotCameraConstraints;
struct RobotCameraSettings;
struct SpotLightType;
struct WeatherType;
struct WorldLayers;
struct WorldParserRoot;
struct WorldType;
// ----- DEFINITIONS ------ //

// ----- ArtType
struct ArtType{
	ArtType();
	ArtType(ArtType&& other);
	ArtType(const ArtType& other) = delete;
	ArtType& operator=(const ArtType& other) = delete;
	ArtType(rapidxml::xml_node<>* node);
	~ArtType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Reference*  image;
	Point2D*  scale;
	Point2D*  repeat;
	Point3D*  pos;
	ColorType*  color;
	int zOrder;
	float rotation;
	std::string name;

};

// ----- Box3DType
struct Box3DType{
	Box3DType();
	Box3DType(Box3DType&& other);
	Box3DType(const Box3DType& other) = delete;
	Box3DType& operator=(const Box3DType& other) = delete;
	Box3DType(rapidxml::xml_node<>* node);
	~Box3DType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  pos;
	Point3D*  size;
	Point2D*  tex;
	ColorType*  color;
	std::string texture;
	std::string name;

};

// ----- DirLightType
struct DirLightType{
	DirLightType();
	DirLightType(DirLightType&& other);
	DirLightType(const DirLightType& other) = delete;
	DirLightType& operator=(const DirLightType& other) = delete;
	DirLightType(rapidxml::xml_node<>* node);
	~DirLightType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  lt;
	Point3D*  rt;
	Point3D*  lb;
	Point3D*  rb;

};

// ----- ERBoxType
struct ERBoxType{
	ERBoxType();
	ERBoxType(ERBoxType&& other);
	ERBoxType(const ERBoxType& other) = delete;
	ERBoxType& operator=(const ERBoxType& other) = delete;
	ERBoxType(rapidxml::xml_node<>* node);
	~ERBoxType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ArtType* > art;
	float x;
	float y;
	float hw;
	float hh;
	float friction;
	float density;
	bool sensor;
	std::string category;

};

// ----- ERChainType
struct ERChainType{
	ERChainType();
	ERChainType(ERChainType&& other);
	ERChainType(const ERChainType& other) = delete;
	ERChainType& operator=(const ERChainType& other) = delete;
	ERChainType(rapidxml::xml_node<>* node);
	~ERChainType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<Point2D* > point;
	float friction;
	std::string category;

};

// ----- EnemyList
struct EnemyList{
	EnemyList();
	EnemyList(EnemyList&& other);
	EnemyList(const EnemyList& other) = delete;
	EnemyList& operator=(const EnemyList& other) = delete;
	EnemyList(rapidxml::xml_node<>* node);
	~EnemyList();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<EnemyType* > enemy;

};

// ----- EnemyType
struct EnemyType{
	EnemyType();
	EnemyType(EnemyType&& other);
	EnemyType(const EnemyType& other) = delete;
	EnemyType& operator=(const EnemyType& other) = delete;
	EnemyType(rapidxml::xml_node<>* node);
	~EnemyType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  pos;
	std::string name;
	std::string enemyClass;

};

// ----- EventType
struct EventType{
	EventType();
	EventType(EventType&& other);
	EventType(const EventType& other) = delete;
	EventType& operator=(const EventType& other) = delete;
	EventType(rapidxml::xml_node<>* node);
	~EventType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  pos;
	Point2D*  size;
	std::string name;
	std::string onEnter;
	std::string onLeave;
	bool infinite;

};

// ----- FXListType
struct FXListType{
	FXListType();
	FXListType(FXListType&& other);
	FXListType(const FXListType& other) = delete;
	FXListType& operator=(const FXListType& other) = delete;
	FXListType(rapidxml::xml_node<>* node);
	~FXListType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<FXType* > fx;

};

// ----- FXType
struct FXType{
	FXType();
	FXType(FXType&& other);
	FXType(const FXType& other) = delete;
	FXType& operator=(const FXType& other) = delete;
	FXType(rapidxml::xml_node<>* node);
	~FXType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  dir;
	ColorType*  color;
	std::string name;
	std::string effect;
	float hw;
	float hh;
	float x;
	float y;
	int zOrder;

};

// ----- GeometryList
struct GeometryList{
	GeometryList();
	GeometryList(GeometryList&& other);
	GeometryList(const GeometryList& other) = delete;
	GeometryList& operator=(const GeometryList& other) = delete;
	GeometryList(rapidxml::xml_node<>* node);
	~GeometryList();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<GeometrySlot* > slot;
	std::vector<JointType* > join;

};

// ----- GeometrySlot
struct GeometrySlot{
	GeometrySlot();
	GeometrySlot(GeometrySlot&& other);
	GeometrySlot(const GeometrySlot& other) = delete;
	GeometrySlot& operator=(const GeometrySlot& other) = delete;
	GeometrySlot(rapidxml::xml_node<>* node);
	~GeometrySlot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ERBoxType* > box;
	ERChainType*  chain;
	std::string name;
	int physType;
	float x;
	float y;

};

// ----- JointType
struct JointType{
	JointType();
	JointType(JointType&& other);
	JointType(const JointType& other) = delete;
	JointType& operator=(const JointType& other) = delete;
	JointType(rapidxml::xml_node<>* node);
	~JointType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  offset1;
	Point3D*  offset2;
	std::string name;
	std::string obj1Name;
	std::string obj2Name;
	float frequence;
	float damp;

};

// ----- LevelSettings
struct LevelSettings{
	LevelSettings();
	LevelSettings(LevelSettings&& other);
	LevelSettings(const LevelSettings& other) = delete;
	LevelSettings& operator=(const LevelSettings& other) = delete;
	LevelSettings(rapidxml::xml_node<>* node);
	~LevelSettings();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string scriptName;
	std::string startEvent;

};

// ----- LightListType
struct LightListType{
	LightListType();
	LightListType(LightListType&& other);
	LightListType(const LightListType& other) = delete;
	LightListType& operator=(const LightListType& other) = delete;
	LightListType(rapidxml::xml_node<>* node);
	~LightListType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<LightType* > light;

};

// ----- LightType
struct LightTypeUnion {
	LightTypeUnion();
	LightTypeUnion(LightTypeUnion&& other);
	struct SpotLightType* spot;
	struct DirLightType* directional;
};
struct LightType{
	LightType();
	LightType(LightType&& other);
	LightType(const LightType& other) = delete;
	LightType& operator=(const LightType& other) = delete;
	LightType(rapidxml::xml_node<>* node);
	~LightType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	LightTypeUnion choiceValue;

	std::string name;
	float r;
	float g;
	float b;
	float a;
	bool enable;
};

// ----- NamedPoint2D
struct NamedPoint2D{
	NamedPoint2D();
	NamedPoint2D(NamedPoint2D&& other);
	NamedPoint2D(const NamedPoint2D& other) = delete;
	NamedPoint2D& operator=(const NamedPoint2D& other) = delete;
	NamedPoint2D(rapidxml::xml_node<>* node);
	~NamedPoint2D();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float x;
	float y;
	std::string name;

};

// ----- Obj3DType
struct Obj3DType{
	Obj3DType();
	Obj3DType(Obj3DType&& other);
	Obj3DType(const Obj3DType& other) = delete;
	Obj3DType& operator=(const Obj3DType& other) = delete;
	Obj3DType(rapidxml::xml_node<>* node);
	~Obj3DType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<Box3DType* > box3D;

};

// ----- Reference
struct Reference{
	Reference();
	Reference(Reference&& other);
	Reference(const Reference& other) = delete;
	Reference& operator=(const Reference& other) = delete;
	Reference(rapidxml::xml_node<>* node);
	~Reference();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;

};

// ----- RobotCameraConstraints
struct RobotCameraConstraints{
	RobotCameraConstraints();
	RobotCameraConstraints(RobotCameraConstraints&& other);
	RobotCameraConstraints(const RobotCameraConstraints& other) = delete;
	RobotCameraConstraints& operator=(const RobotCameraConstraints& other) = delete;
	RobotCameraConstraints(rapidxml::xml_node<>* node);
	~RobotCameraConstraints();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float left;
	float right;
	float top;
	float bottom;

};

// ----- RobotCameraSettings
struct RobotCameraSettings{
	RobotCameraSettings();
	RobotCameraSettings(RobotCameraSettings&& other);
	RobotCameraSettings(const RobotCameraSettings& other) = delete;
	RobotCameraSettings& operator=(const RobotCameraSettings& other) = delete;
	RobotCameraSettings(rapidxml::xml_node<>* node);
	~RobotCameraSettings();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  initPos;
	RobotCameraConstraints*  constraints;

};

// ----- SpotLightType
struct SpotLightType{
	SpotLightType();
	SpotLightType(SpotLightType&& other);
	SpotLightType(const SpotLightType& other) = delete;
	SpotLightType& operator=(const SpotLightType& other) = delete;
	SpotLightType(rapidxml::xml_node<>* node);
	~SpotLightType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  pos;
	float i;
	float hw;
	float hh;

};

// ----- WeatherType
struct WeatherType{
	WeatherType();
	WeatherType(WeatherType&& other);
	WeatherType(const WeatherType& other) = delete;
	WeatherType& operator=(const WeatherType& other) = delete;
	WeatherType(rapidxml::xml_node<>* node);
	~WeatherType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point3D*  dayLight;
	Point3D*  nightLight;
	float daytime;
	bool thunder;

};

// ----- WorldLayers
struct WorldLayers{
	WorldLayers();
	WorldLayers(WorldLayers&& other);
	WorldLayers(const WorldLayers& other) = delete;
	WorldLayers& operator=(const WorldLayers& other) = delete;
	WorldLayers(rapidxml::xml_node<>* node);
	~WorldLayers();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ArtType* > art;
	Obj3DType*  objs3d;
	GeometryList*  geometry;
	FXListType*  fxes;
	LightListType*  light;
	float z;
	std::string name;

};

// ----- WorldParserRoot
struct WorldParserRoot{
	WorldParserRoot();
	WorldParserRoot(WorldParserRoot&& other);
	WorldParserRoot(const WorldParserRoot& other) = delete;
	WorldParserRoot& operator=(const WorldParserRoot& other) = delete;
	WorldParserRoot(rapidxml::xml_node<>* node);
	~WorldParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	WorldType*  world;

};

// ----- WorldType
struct WorldType{
	WorldType();
	WorldType(WorldType&& other);
	WorldType(const WorldType& other) = delete;
	WorldType& operator=(const WorldType& other) = delete;
	WorldType(rapidxml::xml_node<>* node);
	~WorldType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ResourceDescriptionList*  resources;
	LevelSettings*  levelSettings;
	RobotCameraSettings*  camera;
	std::vector<NamedPoint2D* > points;
	std::vector<WorldLayers* > layer;
	EnemyList*  enemies;
	WeatherType*  weather;
	std::vector<EventType* > event;

};
#endif
