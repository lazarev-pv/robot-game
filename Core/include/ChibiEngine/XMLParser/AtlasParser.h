#ifndef __SCHEMAS_ATLAS_XSD
#define __SCHEMAS_ATLAS_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>

// ----- DECLARAIONS ------ //
struct AnimationType;
struct AtlasParserRoot;
struct FrameType;
struct SpriteDescrType;
struct TextureType;
// ----- DEFINITIONS ------ //

// ----- AnimationType
struct AnimationType{
	AnimationType();
	AnimationType(AnimationType&& other);
	AnimationType(const AnimationType& other) = delete;
	AnimationType& operator=(const AnimationType& other) = delete;
	AnimationType(rapidxml::xml_node<>* node);
	~AnimationType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<FrameType* > frame;
	std::string name;
	bool cycled;
	int frameTime;

};

// ----- AtlasParserRoot
struct AtlasParserRoot{
	AtlasParserRoot();
	AtlasParserRoot(AtlasParserRoot&& other);
	AtlasParserRoot(const AtlasParserRoot& other) = delete;
	AtlasParserRoot& operator=(const AtlasParserRoot& other) = delete;
	AtlasParserRoot(rapidxml::xml_node<>* node);
	~AtlasParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	TextureType*  texture;

};

// ----- FrameType
struct FrameType{
	FrameType();
	FrameType(FrameType&& other);
	FrameType(const FrameType& other) = delete;
	FrameType& operator=(const FrameType& other) = delete;
	FrameType(rapidxml::xml_node<>* node);
	~FrameType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	int x1;
	int y1;
	int x2;
	int y2;

};

// ----- SpriteDescrType
struct SpriteDescrType{
	SpriteDescrType();
	SpriteDescrType(SpriteDescrType&& other);
	SpriteDescrType(const SpriteDescrType& other) = delete;
	SpriteDescrType& operator=(const SpriteDescrType& other) = delete;
	SpriteDescrType(rapidxml::xml_node<>* node);
	~SpriteDescrType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	FrameType*  frame;
	std::vector<AnimationType* > animation;
	std::string name;
	std::string initialAnimation;
	float w;
	float h;
	float top;
	float left;
	float right;
	float bottom;

};

// ----- TextureType
struct TextureType{
	TextureType();
	TextureType(TextureType&& other);
	TextureType(const TextureType& other) = delete;
	TextureType& operator=(const TextureType& other) = delete;
	TextureType(rapidxml::xml_node<>* node);
	~TextureType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<SpriteDescrType* > sprite;
	std::string file;

};
#endif
