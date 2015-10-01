#ifndef __SCHEMAS_COMMON_XSD
#define __SCHEMAS_COMMON_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>

// ----- DECLARAIONS ------ //
struct ColorType;
struct CommonParserRoot;
struct Point2D;
struct Point3D;
// ----- DEFINITIONS ------ //

// ----- ColorType
struct ColorType{
	ColorType();
	ColorType(ColorType&& other);
	ColorType(const ColorType& other) = delete;
	ColorType& operator=(const ColorType& other) = delete;
	ColorType(rapidxml::xml_node<>* node);
	~ColorType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float r;
	float g;
	float b;
	float a;
	std::string hex;

};

// ----- CommonParserRoot
struct CommonParserRoot{
	CommonParserRoot();
	CommonParserRoot(CommonParserRoot&& other);
	CommonParserRoot(const CommonParserRoot& other) = delete;
	CommonParserRoot& operator=(const CommonParserRoot& other) = delete;
	CommonParserRoot(rapidxml::xml_node<>* node);
	~CommonParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);


};

// ----- Point2D
struct Point2D{
	Point2D();
	Point2D(Point2D&& other);
	Point2D(const Point2D& other) = delete;
	Point2D& operator=(const Point2D& other) = delete;
	Point2D(rapidxml::xml_node<>* node);
	~Point2D();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float x;
	float y;

};

// ----- Point3D
struct Point3D{
	Point3D();
	Point3D(Point3D&& other);
	Point3D(const Point3D& other) = delete;
	Point3D& operator=(const Point3D& other) = delete;
	Point3D(rapidxml::xml_node<>* node);
	~Point3D();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float x;
	float y;
	float z;

};
#endif
