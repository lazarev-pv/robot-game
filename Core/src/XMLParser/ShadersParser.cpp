#include <ChibiEngine/XMLParser/ShadersParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
AttributeType::AttributeType()
	:name(""),offset(false),count(1){}

AttributeType::AttributeType(AttributeType&& other)
	:name(other.name),offset(other.offset),count(other.count)
{
}
AttributeType::AttributeType(rapidxml::xml_node<>*  node)
	:name(""),offset(false),count(1){
	setValue(&name, node->first_attribute("name"));
	setValue(&offset, node->first_attribute("offset"));
	setValue(&count, node->first_attribute("count"));
}

AttributeType::~AttributeType(){
}

void AttributeType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" offset=\""<<offset<<"\"";
		out<<" count=\""<<count<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ShaderType::ShaderType()
	:uniform(0x0),attribute(0x0),name(""),file(""),atrStep(-1),useDrawElements(true){}

ShaderType::ShaderType(ShaderType&& other)
	:uniform(move(other.uniform)),attribute(move(other.attribute)),name(other.name),file(other.file),atrStep(other.atrStep),useDrawElements(other.useDrawElements)
{
}
ShaderType::ShaderType(rapidxml::xml_node<>*  node)
	:uniform(0x0),attribute(0x0),name(""),file(""),atrStep(-1),useDrawElements(true){
	setValue(&name, node->first_attribute("name"));
	setValue(&file, node->first_attribute("file"));
	setValue(&atrStep, node->first_attribute("atrStep"));
	setValue(&useDrawElements, node->first_attribute("useDrawElements"));
	for(xml_node<>* currNode = node->first_node("uniform");
			currNode!=0;
			currNode = currNode->next_sibling("uniform")){
		uniform.push_back(new UniformType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("attribute");
			currNode!=0;
			currNode = currNode->next_sibling("attribute")){
		attribute.push_back(new AttributeType(currNode));
	}
}

ShaderType::~ShaderType(){
	for(vector<UniformType* >::iterator it = uniform.begin();
			it!=uniform.end();++it){
		delete *it;
	}
;	for(vector<AttributeType* >::iterator it = attribute.begin();
			it!=attribute.end();++it){
		delete *it;
	}
;}

void ShaderType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" file=\""<<file<<"\"";
		out<<" atrStep=\""<<atrStep<<"\"";
		out<<" useDrawElements=\""<<useDrawElements<<"\"";
		out<<">\n";
	}
	for(auto iter:uniform){
		iter->save("uniform",out,nestingLevel+1);
	}
	for(auto iter:attribute){
		iter->save("attribute",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ShadersParserRoot::ShadersParserRoot()
	:shaders(0x0){}

ShadersParserRoot::ShadersParserRoot(ShadersParserRoot&& other)
	:shaders(other.shaders)
{
	other.shaders=nullptr;
}
ShadersParserRoot::ShadersParserRoot(rapidxml::xml_node<>*  node)
	:shaders(0x0){
		create(&shaders, node->first_node("shaders"));
}

ShadersParserRoot::~ShadersParserRoot(){
	delete shaders;
}

void ShadersParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(shaders!=0x0){
		shaders->save("shaders",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ShadersType::ShadersType()
	:shader(0x0){}

ShadersType::ShadersType(ShadersType&& other)
	:shader(move(other.shader))
{
}
ShadersType::ShadersType(rapidxml::xml_node<>*  node)
	:shader(0x0){
	for(xml_node<>* currNode = node->first_node("shader");
			currNode!=0;
			currNode = currNode->next_sibling("shader")){
		shader.push_back(new ShaderType(currNode));
	}
}

ShadersType::~ShadersType(){
	for(vector<ShaderType* >::iterator it = shader.begin();
			it!=shader.end();++it){
		delete *it;
	}
;}

void ShadersType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:shader){
		iter->save("shader",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

UniformType::UniformType()
	:name(""),matrix(false),count(1),sampler(false){}

UniformType::UniformType(UniformType&& other)
	:name(other.name),matrix(other.matrix),count(other.count),sampler(other.sampler)
{
}
UniformType::UniformType(rapidxml::xml_node<>*  node)
	:name(""),matrix(false),count(1),sampler(false){
	setValue(&name, node->first_attribute("name"));
	setValue(&matrix, node->first_attribute("matrix"));
	setValue(&count, node->first_attribute("count"));
	setValue(&sampler, node->first_attribute("sampler"));
}

UniformType::~UniformType(){
}

void UniformType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" matrix=\""<<matrix<<"\"";
		out<<" count=\""<<count<<"\"";
		out<<" sampler=\""<<sampler<<"\"";
		out<<"/>";
	}
	out<<endl;
}

