#include <ChibiEngine/XMLParser/ResourcesDescriptionParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
LinkType::LinkType()
	:name(""),file(""){}

LinkType::LinkType(LinkType&& other)
	:name(other.name),file(other.file)
{
}
LinkType::LinkType(rapidxml::xml_node<>*  node)
	:name(""),file(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&file, node->first_attribute("file"));
}

LinkType::~LinkType(){
}

void LinkType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" file=\""<<file<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ResourceDescriptionList::ResourceDescriptionList()
	:font(0x0),texture(0x0),sprite(0x0),resource(0x0){}

ResourceDescriptionList::ResourceDescriptionList(ResourceDescriptionList&& other)
	:font(move(other.font)),texture(move(other.texture)),sprite(move(other.sprite)),resource(move(other.resource))
{
}
ResourceDescriptionList::ResourceDescriptionList(rapidxml::xml_node<>*  node)
	:font(0x0),texture(0x0),sprite(0x0),resource(0x0){
	for(xml_node<>* currNode = node->first_node("font");
			currNode!=0;
			currNode = currNode->next_sibling("font")){
		font.push_back(new LinkType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("texture");
			currNode!=0;
			currNode = currNode->next_sibling("texture")){
		texture.push_back(new LinkType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("sprite");
			currNode!=0;
			currNode = currNode->next_sibling("sprite")){
		sprite.push_back(new SpriteLinkType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("resource");
			currNode!=0;
			currNode = currNode->next_sibling("resource")){
		resource.push_back(new LinkType(currNode));
	}
}

ResourceDescriptionList::~ResourceDescriptionList(){
	for(vector<LinkType* >::iterator it = font.begin();
			it!=font.end();++it){
		delete *it;
	}
;	for(vector<LinkType* >::iterator it = texture.begin();
			it!=texture.end();++it){
		delete *it;
	}
;	for(vector<SpriteLinkType* >::iterator it = sprite.begin();
			it!=sprite.end();++it){
		delete *it;
	}
;	for(vector<LinkType* >::iterator it = resource.begin();
			it!=resource.end();++it){
		delete *it;
	}
;}

void ResourceDescriptionList::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:font){
		iter->save("font",out,nestingLevel+1);
	}
	for(auto iter:texture){
		iter->save("texture",out,nestingLevel+1);
	}
	for(auto iter:sprite){
		iter->save("sprite",out,nestingLevel+1);
	}
	for(auto iter:resource){
		iter->save("resource",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

ResourcesDescriptionParserRoot::ResourcesDescriptionParserRoot()
	:resources(0x0){}

ResourcesDescriptionParserRoot::ResourcesDescriptionParserRoot(ResourcesDescriptionParserRoot&& other)
	:resources(other.resources)
{
	other.resources=nullptr;
}
ResourcesDescriptionParserRoot::ResourcesDescriptionParserRoot(rapidxml::xml_node<>*  node)
	:resources(0x0){
		create(&resources, node->first_node("resources"));
}

ResourcesDescriptionParserRoot::~ResourcesDescriptionParserRoot(){
	delete resources;
}

void ResourcesDescriptionParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(resources!=0x0){
		resources->save("resources",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SpriteLinkType::SpriteLinkType()
	:name(""),file(""),w(-1),h(-1),top(-1),left(-1),right(-1),bottom(-1){}

SpriteLinkType::SpriteLinkType(SpriteLinkType&& other)
	:name(other.name),file(other.file),w(other.w),h(other.h),top(other.top),left(other.left),right(other.right),bottom(other.bottom)
{
}
SpriteLinkType::SpriteLinkType(rapidxml::xml_node<>*  node)
	:name(""),file(""),w(-1),h(-1),top(-1),left(-1),right(-1),bottom(-1){
	setValue(&name, node->first_attribute("name"));
	setValue(&file, node->first_attribute("file"));
	setValue(&w, node->first_attribute("w"));
	setValue(&h, node->first_attribute("h"));
	setValue(&top, node->first_attribute("top"));
	setValue(&left, node->first_attribute("left"));
	setValue(&right, node->first_attribute("right"));
	setValue(&bottom, node->first_attribute("bottom"));
}

SpriteLinkType::~SpriteLinkType(){
}

void SpriteLinkType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" file=\""<<file<<"\"";
		out<<" w=\""<<w<<"\"";
		out<<" h=\""<<h<<"\"";
		out<<" top=\""<<top<<"\"";
		out<<" left=\""<<left<<"\"";
		out<<" right=\""<<right<<"\"";
		out<<" bottom=\""<<bottom<<"\"";
		out<<"/>";
	}
	out<<endl;
}

