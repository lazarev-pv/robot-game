#ifndef __UTIL
#define __UTIL
#include <cstring>
#include <rapid/rapidxml.hpp>
#include <string>
#include <ostream>


void setValue(int* var, rapidxml::xml_base<>* value);
void setValue(float* var, rapidxml::xml_base<>* atr);
void setValue(double* var, rapidxml::xml_base<>* atr);
void setValue(std::string* var, rapidxml::xml_base<>* atr);
void setValue(bool* var, rapidxml::xml_base<>* atr);


void createTag(std::ostream& out, std::string name, std::string value);
void createTag(std::ostream& out, std::string name, int value);
void createTag(std::ostream& out, std::string name, double value);
void createTag(std::ostream& out, std::string name, float value);
void createTag(std::ostream& out, std::string name, bool value);

template<class T> void create(T** variable, rapidxml::xml_node<>* node){
	if(node!=nullptr)
		*variable = new T(node);
}
template<class T> void create(T** variable, rapidxml::xml_document<>* node){
	if(node!=nullptr)
		*variable = new T(node);
}
#endif
