#include "util.h"
void setValue(int* var, rapidxml::xml_base<>* value){
	if(value!=0)
		*var=atoi(value->value());
}

void setValue(float* var, rapidxml::xml_base<>* atr){
	if(atr!=0)
		*var=atof(atr->value());
}

void setValue(double* var, rapidxml::xml_base<>* atr){
	if(atr!=0)
		*var=atof(atr->value());
}

void setValue(std::string* var, rapidxml::xml_base<>* atr){
	if(atr!=0)
		*var=(atr->value());
}

void setValue(bool* var, rapidxml::xml_base<>* atr){
	if(atr!=0)
		*var=(strcmp(atr->value(),"true")==0)||(strcmp(atr->value(),"1")==0);
}


void createTag(std::ostream& out, std::string name, std::string value){
	if(value.size()>0)
		out<<"<"<<name<<">"<<value<<"</"<<name<<">\n";
	else
		out<<"<"<<name<<"/>\n";
}

void createTag(std::ostream& out, std::string name, int value){
	out<<"<"<<name<<">"<<value<<"</"<<name<<">\n";
}

void createTag(std::ostream& out, std::string name, double value){
	out<<"<"<<name<<">"<<value<<"</"<<name<<">\n";
}

void createTag(std::ostream& out, std::string name, float value){
	out<<"<"<<name<<">"<<value<<"</"<<name<<">\n";
}

void createTag(std::ostream& out, std::string name, bool value){
	out<<"<"<<name<<">"<<value<<"</"<<name<<">\n";
}
