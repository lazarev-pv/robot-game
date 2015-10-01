#ifndef __SCHEMAS_SHADERS_XSD
#define __SCHEMAS_SHADERS_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>

// ----- DECLARAIONS ------ //
struct AttributeType;
struct ShaderType;
struct ShadersParserRoot;
struct ShadersType;
struct UniformType;
// ----- DEFINITIONS ------ //

// ----- AttributeType
struct AttributeType{
	AttributeType();
	AttributeType(AttributeType&& other);
	AttributeType(const AttributeType& other) = delete;
	AttributeType& operator=(const AttributeType& other) = delete;
	AttributeType(rapidxml::xml_node<>* node);
	~AttributeType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	int offset;
	int count;

};

// ----- ShaderType
struct ShaderType{
	ShaderType();
	ShaderType(ShaderType&& other);
	ShaderType(const ShaderType& other) = delete;
	ShaderType& operator=(const ShaderType& other) = delete;
	ShaderType(rapidxml::xml_node<>* node);
	~ShaderType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<UniformType* > uniform;
	std::vector<AttributeType* > attribute;
	std::string name;
	std::string file;
	int atrStep;
	bool useDrawElements;

};

// ----- ShadersParserRoot
struct ShadersParserRoot{
	ShadersParserRoot();
	ShadersParserRoot(ShadersParserRoot&& other);
	ShadersParserRoot(const ShadersParserRoot& other) = delete;
	ShadersParserRoot& operator=(const ShadersParserRoot& other) = delete;
	ShadersParserRoot(rapidxml::xml_node<>* node);
	~ShadersParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ShadersType*  shaders;

};

// ----- ShadersType
struct ShadersType{
	ShadersType();
	ShadersType(ShadersType&& other);
	ShadersType(const ShadersType& other) = delete;
	ShadersType& operator=(const ShadersType& other) = delete;
	ShadersType(rapidxml::xml_node<>* node);
	~ShadersType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<ShaderType* > shader;

};

// ----- UniformType
struct UniformType{
	UniformType();
	UniformType(UniformType&& other);
	UniformType(const UniformType& other) = delete;
	UniformType& operator=(const UniformType& other) = delete;
	UniformType(rapidxml::xml_node<>* node);
	~UniformType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	bool matrix;
	int count;
	bool sampler;

};
#endif
