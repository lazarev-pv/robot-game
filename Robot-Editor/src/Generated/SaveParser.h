#ifndef __SCHEMAS_SAVE_XSD
#define __SCHEMAS_SAVE_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include <ChibiEngine/XMLParser/CommonParser.h>

// ----- DECLARAIONS ------ //
struct SaveDataType;
struct SaveParserRoot;
struct SelectFilterType;
// ----- DEFINITIONS ------ //

// ----- SaveDataType
struct SaveDataType{
	SaveDataType();
	SaveDataType(SaveDataType&& other);
	SaveDataType(const SaveDataType& other) = delete;
	SaveDataType& operator=(const SaveDataType& other) = delete;
	SaveDataType(rapidxml::xml_node<>* node);
	~SaveDataType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	SelectFilterType*  selectFilter;
	Point3D*  cameraPos;

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

	SaveDataType*  saveData;

};

// ----- SelectFilterType
struct SelectFilterType{
	SelectFilterType();
	SelectFilterType(SelectFilterType&& other);
	SelectFilterType(const SelectFilterType& other) = delete;
	SelectFilterType& operator=(const SelectFilterType& other) = delete;
	SelectFilterType(rapidxml::xml_node<>* node);
	~SelectFilterType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<std::string> entityName;

};
#endif
