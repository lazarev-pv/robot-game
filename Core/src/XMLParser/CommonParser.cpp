#include <ChibiEngine/XMLParser/CommonParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
ColorType::ColorType()
	:r(0),g(0),b(0),a(1.0),hex(""){}

ColorType::ColorType(ColorType&& other)
	:r(other.r),g(other.g),b(other.b),a(other.a),hex(other.hex)
{
}
ColorType::ColorType(rapidxml::xml_node<>*  node)
	:r(0),g(0),b(0),a(1.0),hex(""){
	setValue(&r, node->first_attribute("r"));
	setValue(&g, node->first_attribute("g"));
	setValue(&b, node->first_attribute("b"));
	setValue(&a, node->first_attribute("a"));
	setValue(&hex, node->first_attribute("hex"));
}

ColorType::~ColorType(){
}

void ColorType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" r=\""<<r<<"\"";
		out<<" g=\""<<g<<"\"";
		out<<" b=\""<<b<<"\"";
		out<<" a=\""<<a<<"\"";
		out<<" hex=\""<<hex<<"\"";
		out<<"/>";
	}
	out<<endl;
}

CommonParserRoot::CommonParserRoot(){}

CommonParserRoot::CommonParserRoot(CommonParserRoot&& other)
{
}
CommonParserRoot::CommonParserRoot(rapidxml::xml_node<>*  ){
}

CommonParserRoot::~CommonParserRoot(){
}

void CommonParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<"/>";
	}
	out<<endl;
}

Point2D::Point2D()
	:x(0),y(0){}

Point2D::Point2D(Point2D&& other)
	:x(other.x),y(other.y)
{
}
Point2D::Point2D(rapidxml::xml_node<>*  node)
	:x(0),y(0){
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
}

Point2D::~Point2D(){
}

void Point2D::save(std::string tagName, std::ostream& out,int nestingLevel){
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

Point3D::Point3D()
	:x(0),y(0),z(0){}

Point3D::Point3D(Point3D&& other)
	:x(other.x),y(other.y),z(other.z)
{
}
Point3D::Point3D(rapidxml::xml_node<>*  node)
	:x(0),y(0),z(0){
	setValue(&x, node->first_attribute("x"));
	setValue(&y, node->first_attribute("y"));
	setValue(&z, node->first_attribute("z"));
}

Point3D::~Point3D(){
}

void Point3D::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" x=\""<<x<<"\"";
		out<<" y=\""<<y<<"\"";
		out<<" z=\""<<z<<"\"";
		out<<"/>";
	}
	out<<endl;
}

