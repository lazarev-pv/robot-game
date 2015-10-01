#ifndef __SCHEMAS_CHED_SETTINGS_XSD
#define __SCHEMAS_CHED_SETTINGS_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include <ChibiEngine/XMLParser/CommonParser.h>

// ----- DECLARAIONS ------ //
struct ChedSettingsParserRoot;
struct ChedSettingsType;
struct ChedTimeLine;
// ----- DEFINITIONS ------ //

// ----- ChedSettingsParserRoot
struct ChedSettingsParserRoot{
	ChedSettingsParserRoot();
	ChedSettingsParserRoot(ChedSettingsParserRoot&& other);
	ChedSettingsParserRoot(const ChedSettingsParserRoot& other) = delete;
	ChedSettingsParserRoot& operator=(const ChedSettingsParserRoot& other) = delete;
	ChedSettingsParserRoot(rapidxml::xml_node<>* node);
	~ChedSettingsParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ChedSettingsType*  chedSettings;

};

// ----- ChedSettingsType
struct ChedSettingsType{
	ChedSettingsType();
	ChedSettingsType(ChedSettingsType&& other);
	ChedSettingsType(const ChedSettingsType& other) = delete;
	ChedSettingsType& operator=(const ChedSettingsType& other) = delete;
	ChedSettingsType(rapidxml::xml_node<>* node);
	~ChedSettingsType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ChedTimeLine*  timeline;

};

// ----- ChedTimeLine
struct ChedTimeLine{
	ChedTimeLine();
	ChedTimeLine(ChedTimeLine&& other);
	ChedTimeLine(const ChedTimeLine& other) = delete;
	ChedTimeLine& operator=(const ChedTimeLine& other) = delete;
	ChedTimeLine(rapidxml::xml_node<>* node);
	~ChedTimeLine();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  pos;
	float hw;
	float hh;
	float xPadding;
	float yPadding;
	int elementsCount;
	int numPerElement;
	int labelSize;
	float labelYOffset;
	std::string background;
	std::string linesHexColor;
	std::string labelHexColor;
	std::string frameHexColor;
	std::string animEndHexColor;
	float frameHW;
	std::string currentTimeHexColor;
	float currentHW;
	float lineHeightPercent;
	float midLineHeightPercent;

};
#endif
