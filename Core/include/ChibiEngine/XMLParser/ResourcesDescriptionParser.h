#ifndef __SCHEMAS_RESOURCES_DESCRIPTION_XSD
#define __SCHEMAS_RESOURCES_DESCRIPTION_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>

// ----- DECLARAIONS ------ //
struct LinkType;
struct ResourceDescriptionList;
struct ResourcesDescriptionParserRoot;
struct SpriteLinkType;
// ----- DEFINITIONS ------ //

// ----- LinkType
struct LinkType{
	LinkType();
	LinkType(LinkType&& other);
	LinkType(const LinkType& other) = delete;
	LinkType& operator=(const LinkType& other) = delete;
	LinkType(rapidxml::xml_node<>* node);
	~LinkType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	std::string file;

};

// ----- ResourceDescriptionList
struct ResourceDescriptionList{
	ResourceDescriptionList();
	ResourceDescriptionList(ResourceDescriptionList&& other);
	ResourceDescriptionList(const ResourceDescriptionList& other) = delete;
	ResourceDescriptionList& operator=(const ResourceDescriptionList& other) = delete;
	ResourceDescriptionList(rapidxml::xml_node<>* node);
	~ResourceDescriptionList();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<LinkType* > font;
	std::vector<LinkType* > texture;
	std::vector<SpriteLinkType* > sprite;
	std::vector<LinkType* > resource;

};

// ----- ResourcesDescriptionParserRoot
struct ResourcesDescriptionParserRoot{
	ResourcesDescriptionParserRoot();
	ResourcesDescriptionParserRoot(ResourcesDescriptionParserRoot&& other);
	ResourcesDescriptionParserRoot(const ResourcesDescriptionParserRoot& other) = delete;
	ResourcesDescriptionParserRoot& operator=(const ResourcesDescriptionParserRoot& other) = delete;
	ResourcesDescriptionParserRoot(rapidxml::xml_node<>* node);
	~ResourcesDescriptionParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ResourceDescriptionList*  resources;

};

// ----- SpriteLinkType
struct SpriteLinkType{
	SpriteLinkType();
	SpriteLinkType(SpriteLinkType&& other);
	SpriteLinkType(const SpriteLinkType& other) = delete;
	SpriteLinkType& operator=(const SpriteLinkType& other) = delete;
	SpriteLinkType(rapidxml::xml_node<>* node);
	~SpriteLinkType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	std::string file;
	float w;
	float h;
	float top;
	float left;
	float right;
	float bottom;

};
#endif
