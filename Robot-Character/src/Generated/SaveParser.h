#ifndef __SCHEMAS_SAVE_XSD
#define __SCHEMAS_SAVE_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include <ChibiEngine/XMLParser/CommonParser.h>

// ----- DECLARAIONS ------ //
struct ChedSavedData;
struct PropEntityStateType;
struct SaveParserRoot;
struct SavedDrawType;
struct SavedPropertiesType;
struct TimeLineType;
struct WindowPropsType;
// ----- DEFINITIONS ------ //

// ----- ChedSavedData
struct ChedSavedData{
	ChedSavedData();
	ChedSavedData(ChedSavedData&& other);
	ChedSavedData(const ChedSavedData& other) = delete;
	ChedSavedData& operator=(const ChedSavedData& other) = delete;
	ChedSavedData(rapidxml::xml_node<>* node);
	~ChedSavedData();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	TimeLineType*  timeLine;
	SavedPropertiesType*  properties;
	SavedDrawType*  draw;
	Point3D*  camera;
	std::vector<WindowPropsType* > windows;

};

// ----- PropEntityStateType
struct PropEntityStateType{
	PropEntityStateType();
	PropEntityStateType(PropEntityStateType&& other);
	PropEntityStateType(const PropEntityStateType& other) = delete;
	PropEntityStateType& operator=(const PropEntityStateType& other) = delete;
	PropEntityStateType(rapidxml::xml_node<>* node);
	~PropEntityStateType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	bool drawable;
	bool selectable;

};

// ----- SaveParserRoot
struct SaveParserRoot{
	SaveParserRoot();
	SaveParserRoot(SaveParserRoot&& other);
	SaveParserRoot(const SaveParserRoot& other) = delete;
	SaveParserRoot& operator=(const SaveParserRoot& other) = delete;
	SaveParserRoot(rapidxml::xml_node<>* node);
	~SaveParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ChedSavedData*  savedData;

};

// ----- SavedDrawType
struct SavedDrawType{
	SavedDrawType();
	SavedDrawType(SavedDrawType&& other);
	SavedDrawType(const SavedDrawType& other) = delete;
	SavedDrawType& operator=(const SavedDrawType& other) = delete;
	SavedDrawType(rapidxml::xml_node<>* node);
	~SavedDrawType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	PropEntityStateType*  bonesState;
	PropEntityStateType*  picsState;
	PropEntityStateType*  physState;

};

// ----- SavedPropertiesType
struct SavedPropertiesType{
	SavedPropertiesType();
	SavedPropertiesType(SavedPropertiesType&& other);
	SavedPropertiesType(const SavedPropertiesType& other) = delete;
	SavedPropertiesType& operator=(const SavedPropertiesType& other) = delete;
	SavedPropertiesType(rapidxml::xml_node<>* node);
	~SavedPropertiesType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string animation;
	std::string workTarget;

};

// ----- TimeLineType
struct TimeLineType{
	TimeLineType();
	TimeLineType(TimeLineType&& other);
	TimeLineType(const TimeLineType& other) = delete;
	TimeLineType& operator=(const TimeLineType& other) = delete;
	TimeLineType(rapidxml::xml_node<>* node);
	~TimeLineType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float startX;
	float endX;
	int divStep;
	int markFreq;

};

// ----- WindowPropsType
struct WindowPropsType{
	WindowPropsType();
	WindowPropsType(WindowPropsType&& other);
	WindowPropsType(const WindowPropsType& other) = delete;
	WindowPropsType& operator=(const WindowPropsType& other) = delete;
	WindowPropsType(rapidxml::xml_node<>* node);
	~WindowPropsType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  pos;
	std::string name;
	bool minimalized;
	bool visible;

};
#endif
