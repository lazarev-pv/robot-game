#ifndef __SCHEMAS_CHARACTER_XSD
#define __SCHEMAS_CHARACTER_XSD
#include <vector>
#include <string>
#include <ostream>
#include <rapid/rapidxml.hpp>
#include "CommonParser.h"
#include "ResourcesDescriptionParser.h"

// ----- DECLARAIONS ------ //
struct AnimationEventType;
struct AnimationListType;
struct BodyPartType;
struct BoneSlotType;
struct BoneType;
struct CharBoxType;
struct CharPhysType;
struct CharacterParserRoot;
struct CharacterType;
struct MoveTransitionType;
struct RotateTransitionType;
struct ScaleTransitionType;
struct SkeletonAnimationBoneType;
struct SkeletonAnimationType;
struct SkeletonType;
// ----- DEFINITIONS ------ //

// ----- AnimationEventType
struct AnimationEventType{
	AnimationEventType();
	AnimationEventType(AnimationEventType&& other);
	AnimationEventType(const AnimationEventType& other) = delete;
	AnimationEventType& operator=(const AnimationEventType& other) = delete;
	AnimationEventType(rapidxml::xml_node<>* node);
	~AnimationEventType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::string name;
	float time;

};

// ----- AnimationListType
struct AnimationListType{
	AnimationListType();
	AnimationListType(AnimationListType&& other);
	AnimationListType(const AnimationListType& other) = delete;
	AnimationListType& operator=(const AnimationListType& other) = delete;
	AnimationListType(rapidxml::xml_node<>* node);
	~AnimationListType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<SkeletonAnimationType* > animation;

};

// ----- BodyPartType
struct BodyPartType{
	BodyPartType();
	BodyPartType(BodyPartType&& other);
	BodyPartType(const BodyPartType& other) = delete;
	BodyPartType& operator=(const BodyPartType& other) = delete;
	BodyPartType(rapidxml::xml_node<>* node);
	~BodyPartType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  pos;
	Point2D*  scale;
	float rotation;
	std::string pres;
	std::string name;

};

// ----- BoneSlotType
struct BoneSlotType{
	BoneSlotType();
	BoneSlotType(BoneSlotType&& other);
	BoneSlotType(const BoneSlotType& other) = delete;
	BoneSlotType& operator=(const BoneSlotType& other) = delete;
	BoneSlotType(rapidxml::xml_node<>* node);
	~BoneSlotType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<BodyPartType* > pres;
	int id;
	std::string name;
	std::string defaultPres;
	int zOrder;

};

// ----- BoneType
struct BoneType{
	BoneType();
	BoneType(BoneType&& other);
	BoneType(const BoneType& other) = delete;
	BoneType& operator=(const BoneType& other) = delete;
	BoneType(rapidxml::xml_node<>* node);
	~BoneType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	Point2D*  begin;
	Point2D*  end;
	std::vector<BoneSlotType* > slot;
	ColorType*  color;
	std::string name;
	std::string parent;
	int id;

};

// ----- CharBoxType
struct CharBoxType{
	CharBoxType();
	CharBoxType(CharBoxType&& other);
	CharBoxType(const CharBoxType& other) = delete;
	CharBoxType& operator=(const CharBoxType& other) = delete;
	CharBoxType(rapidxml::xml_node<>* node);
	~CharBoxType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float pox_x;
	float pox_y;
	float hw;
	float hh;
	float friction;
	float density;
	bool sensor;
	float rotation;
	std::string name;
	std::string parent;

};

// ----- CharPhysType
struct CharPhysType{
	CharPhysType();
	CharPhysType(CharPhysType&& other);
	CharPhysType(const CharPhysType& other) = delete;
	CharPhysType& operator=(const CharPhysType& other) = delete;
	CharPhysType(rapidxml::xml_node<>* node);
	~CharPhysType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<CharBoxType* > box;

};

// ----- CharacterParserRoot
struct CharacterParserRoot{
	CharacterParserRoot();
	CharacterParserRoot(CharacterParserRoot&& other);
	CharacterParserRoot(const CharacterParserRoot& other) = delete;
	CharacterParserRoot& operator=(const CharacterParserRoot& other) = delete;
	CharacterParserRoot(rapidxml::xml_node<>* node);
	~CharacterParserRoot();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	CharacterType*  character;

};

// ----- CharacterType
struct CharacterType{
	CharacterType();
	CharacterType(CharacterType&& other);
	CharacterType(const CharacterType& other) = delete;
	CharacterType& operator=(const CharacterType& other) = delete;
	CharacterType(rapidxml::xml_node<>* node);
	~CharacterType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	ResourceDescriptionList*  resources;
	SkeletonType*  skeleton;
	CharPhysType*  physics;
	AnimationListType*  animations;
	std::string ai;

};

// ----- MoveTransitionType
struct MoveTransitionType{
	MoveTransitionType();
	MoveTransitionType(MoveTransitionType&& other);
	MoveTransitionType(const MoveTransitionType& other) = delete;
	MoveTransitionType& operator=(const MoveTransitionType& other) = delete;
	MoveTransitionType(rapidxml::xml_node<>* node);
	~MoveTransitionType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float time;
	float offset_x;
	float offset_y;

};

// ----- RotateTransitionType
struct RotateTransitionType{
	RotateTransitionType();
	RotateTransitionType(RotateTransitionType&& other);
	RotateTransitionType(const RotateTransitionType& other) = delete;
	RotateTransitionType& operator=(const RotateTransitionType& other) = delete;
	RotateTransitionType(rapidxml::xml_node<>* node);
	~RotateTransitionType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float time;
	float q_z;
	float q_w;

};

// ----- ScaleTransitionType
struct ScaleTransitionType{
	ScaleTransitionType();
	ScaleTransitionType(ScaleTransitionType&& other);
	ScaleTransitionType(const ScaleTransitionType& other) = delete;
	ScaleTransitionType& operator=(const ScaleTransitionType& other) = delete;
	ScaleTransitionType(rapidxml::xml_node<>* node);
	~ScaleTransitionType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	float time;
	float scale_x;
	float scale_y;

};

// ----- SkeletonAnimationBoneType
struct SkeletonAnimationBoneType{
	SkeletonAnimationBoneType();
	SkeletonAnimationBoneType(SkeletonAnimationBoneType&& other);
	SkeletonAnimationBoneType(const SkeletonAnimationBoneType& other) = delete;
	SkeletonAnimationBoneType& operator=(const SkeletonAnimationBoneType& other) = delete;
	SkeletonAnimationBoneType(rapidxml::xml_node<>* node);
	~SkeletonAnimationBoneType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<MoveTransitionType* > offset;
	std::vector<RotateTransitionType* > rotate;
	std::vector<ScaleTransitionType* > scale;
	int id;

};

// ----- SkeletonAnimationType
struct SkeletonAnimationType{
	SkeletonAnimationType();
	SkeletonAnimationType(SkeletonAnimationType&& other);
	SkeletonAnimationType(const SkeletonAnimationType& other) = delete;
	SkeletonAnimationType& operator=(const SkeletonAnimationType& other) = delete;
	SkeletonAnimationType(rapidxml::xml_node<>* node);
	~SkeletonAnimationType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<AnimationEventType* > event;
	std::vector<SkeletonAnimationBoneType* > bone;
	std::string name;
	float duration;

};

// ----- SkeletonType
struct SkeletonType{
	SkeletonType();
	SkeletonType(SkeletonType&& other);
	SkeletonType(const SkeletonType& other) = delete;
	SkeletonType& operator=(const SkeletonType& other) = delete;
	SkeletonType(rapidxml::xml_node<>* node);
	~SkeletonType();
	void save(std::string tagName, std::ostream& out, int nestingLevel = 0);

	std::vector<BoneType* > bones;

};
#endif
