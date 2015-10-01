#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <cstring>
#include "util.h"
using namespace rapidxml;
using namespace std;
AnimationEventType::AnimationEventType()
	:name(""){}

AnimationEventType::AnimationEventType(AnimationEventType&& other)
	:name(other.name),time(other.time)
{
}
AnimationEventType::AnimationEventType(rapidxml::xml_node<>*  node)
	:name(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&time, node->first_attribute("time"));
}

AnimationEventType::~AnimationEventType(){
}

void AnimationEventType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" time=\""<<time<<"\"";
		out<<"/>";
	}
	out<<endl;
}

AnimationListType::AnimationListType()
	:animation(0x0){}

AnimationListType::AnimationListType(AnimationListType&& other)
	:animation(move(other.animation))
{
}
AnimationListType::AnimationListType(rapidxml::xml_node<>*  node)
	:animation(0x0){
	for(xml_node<>* currNode = node->first_node("animation");
			currNode!=0;
			currNode = currNode->next_sibling("animation")){
		animation.push_back(new SkeletonAnimationType(currNode));
	}
}

AnimationListType::~AnimationListType(){
	for(vector<SkeletonAnimationType* >::iterator it = animation.begin();
			it!=animation.end();++it){
		delete *it;
	}
;}

void AnimationListType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:animation){
		iter->save("animation",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

BodyPartType::BodyPartType()
	:pos(0x0),scale(0x0),pres(""),name(""){}

BodyPartType::BodyPartType(BodyPartType&& other)
	:pos(other.pos),scale(other.scale),rotation(other.rotation),pres(other.pres),name(other.name)
{
	other.pos=nullptr;
	other.scale=nullptr;
}
BodyPartType::BodyPartType(rapidxml::xml_node<>*  node)
	:pos(0x0),scale(0x0),pres(""),name(""){
	setValue(&rotation, node->first_attribute("rotation"));
	setValue(&pres, node->first_attribute("pres"));
	setValue(&name, node->first_attribute("name"));
		create(&pos, node->first_node("pos"));
		create(&scale, node->first_node("scale"));
}

BodyPartType::~BodyPartType(){
	delete pos;
	delete scale;
}

void BodyPartType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" rotation=\""<<rotation<<"\"";
		out<<" pres=\""<<pres<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<">\n";
	}
	if(pos!=0x0){
		pos->save("pos",out,nestingLevel+1);
	}
	if(scale!=0x0){
		scale->save("scale",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

BoneSlotType::BoneSlotType()
	:pres(0x0),name(""),defaultPres(""){}

BoneSlotType::BoneSlotType(BoneSlotType&& other)
	:pres(move(other.pres)),id(other.id),name(other.name),defaultPres(other.defaultPres),zOrder(other.zOrder)
{
}
BoneSlotType::BoneSlotType(rapidxml::xml_node<>*  node)
	:pres(0x0),name(""),defaultPres(""){
	setValue(&id, node->first_attribute("id"));
	setValue(&name, node->first_attribute("name"));
	setValue(&defaultPres, node->first_attribute("defaultPres"));
	setValue(&zOrder, node->first_attribute("zOrder"));
	for(xml_node<>* currNode = node->first_node("pres");
			currNode!=0;
			currNode = currNode->next_sibling("pres")){
		pres.push_back(new BodyPartType(currNode));
	}
}

BoneSlotType::~BoneSlotType(){
	for(vector<BodyPartType* >::iterator it = pres.begin();
			it!=pres.end();++it){
		delete *it;
	}
;}

void BoneSlotType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" id=\""<<id<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<" defaultPres=\""<<defaultPres<<"\"";
		out<<" zOrder=\""<<zOrder<<"\"";
		out<<">\n";
	}
	for(auto iter:pres){
		iter->save("pres",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

BoneType::BoneType()
	:begin(0x0),end(0x0),slot(0x0),color(0x0),name(""),parent(""){}

BoneType::BoneType(BoneType&& other)
	:begin(other.begin),end(other.end),slot(move(other.slot)),color(other.color),name(other.name),parent(other.parent),id(other.id)
{
	other.begin=nullptr;
	other.end=nullptr;
	other.color=nullptr;
}
BoneType::BoneType(rapidxml::xml_node<>*  node)
	:begin(0x0),end(0x0),slot(0x0),color(0x0),name(""),parent(""){
	setValue(&name, node->first_attribute("name"));
	setValue(&parent, node->first_attribute("parent"));
	setValue(&id, node->first_attribute("id"));
		create(&begin, node->first_node("begin"));
		create(&end, node->first_node("end"));
	for(xml_node<>* currNode = node->first_node("slot");
			currNode!=0;
			currNode = currNode->next_sibling("slot")){
		slot.push_back(new BoneSlotType(currNode));
	}
		create(&color, node->first_node("color"));
}

BoneType::~BoneType(){
	delete begin;
	delete end;
	for(vector<BoneSlotType* >::iterator it = slot.begin();
			it!=slot.end();++it){
		delete *it;
	}
;	delete color;
}

void BoneType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" parent=\""<<parent<<"\"";
		out<<" id=\""<<id<<"\"";
		out<<">\n";
	}
	if(begin!=0x0){
		begin->save("begin",out,nestingLevel+1);
	}
	if(end!=0x0){
		end->save("end",out,nestingLevel+1);
	}
	for(auto iter:slot){
		iter->save("slot",out,nestingLevel+1);
	}
	if(color!=0x0){
		color->save("color",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CharBoxType::CharBoxType()
	:friction(0.0),density(0.0),sensor(false),rotation(0.0),name(""),parent(""){}

CharBoxType::CharBoxType(CharBoxType&& other)
	:pox_x(other.pox_x),pox_y(other.pox_y),hw(other.hw),hh(other.hh),friction(other.friction),density(other.density),sensor(other.sensor),rotation(other.rotation),name(other.name),parent(other.parent)
{
}
CharBoxType::CharBoxType(rapidxml::xml_node<>*  node)
	:friction(0.0),density(0.0),sensor(false),rotation(0.0),name(""),parent(""){
	setValue(&pox_x, node->first_attribute("pox_x"));
	setValue(&pox_y, node->first_attribute("pox_y"));
	setValue(&hw, node->first_attribute("hw"));
	setValue(&hh, node->first_attribute("hh"));
	setValue(&friction, node->first_attribute("friction"));
	setValue(&density, node->first_attribute("density"));
	setValue(&sensor, node->first_attribute("sensor"));
	setValue(&rotation, node->first_attribute("rotation"));
	setValue(&name, node->first_attribute("name"));
	setValue(&parent, node->first_attribute("parent"));
}

CharBoxType::~CharBoxType(){
}

void CharBoxType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" pox_x=\""<<pox_x<<"\"";
		out<<" pox_y=\""<<pox_y<<"\"";
		out<<" hw=\""<<hw<<"\"";
		out<<" hh=\""<<hh<<"\"";
		out<<" friction=\""<<friction<<"\"";
		out<<" density=\""<<density<<"\"";
		out<<" sensor=\""<<sensor<<"\"";
		out<<" rotation=\""<<rotation<<"\"";
		out<<" name=\""<<name<<"\"";
		out<<" parent=\""<<parent<<"\"";
		out<<"/>";
	}
	out<<endl;
}

CharPhysType::CharPhysType()
	:box(0x0){}

CharPhysType::CharPhysType(CharPhysType&& other)
	:box(move(other.box))
{
}
CharPhysType::CharPhysType(rapidxml::xml_node<>*  node)
	:box(0x0){
	for(xml_node<>* currNode = node->first_node("box");
			currNode!=0;
			currNode = currNode->next_sibling("box")){
		box.push_back(new CharBoxType(currNode));
	}
}

CharPhysType::~CharPhysType(){
	for(vector<CharBoxType* >::iterator it = box.begin();
			it!=box.end();++it){
		delete *it;
	}
;}

void CharPhysType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:box){
		iter->save("box",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CharacterParserRoot::CharacterParserRoot()
	:character(0x0){}

CharacterParserRoot::CharacterParserRoot(CharacterParserRoot&& other)
	:character(other.character)
{
	other.character=nullptr;
}
CharacterParserRoot::CharacterParserRoot(rapidxml::xml_node<>*  node)
	:character(0x0){
		create(&character, node->first_node("character"));
}

CharacterParserRoot::~CharacterParserRoot(){
	delete character;
}

void CharacterParserRoot::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	if(character!=0x0){
		character->save("character",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

CharacterType::CharacterType()
	:resources(0x0),skeleton(0x0),physics(0x0),animations(0x0),ai("ai.lua"){}

CharacterType::CharacterType(CharacterType&& other)
	:resources(other.resources),skeleton(other.skeleton),physics(other.physics),animations(other.animations),ai(other.ai)
{
	other.resources=nullptr;
	other.skeleton=nullptr;
	other.physics=nullptr;
	other.animations=nullptr;
}
CharacterType::CharacterType(rapidxml::xml_node<>*  node)
	:resources(0x0),skeleton(0x0),physics(0x0),animations(0x0),ai("ai.lua"){
	setValue(&ai, node->first_attribute("ai"));
		create(&resources, node->first_node("resources"));
		create(&skeleton, node->first_node("skeleton"));
		create(&physics, node->first_node("physics"));
		create(&animations, node->first_node("animations"));
}

CharacterType::~CharacterType(){
	delete resources;
	delete skeleton;
	delete physics;
	delete animations;
}

void CharacterType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" ai=\""<<ai<<"\"";
		out<<">\n";
	}
	if(resources!=0x0){
		resources->save("resources",out,nestingLevel+1);
	}
	if(skeleton!=0x0){
		skeleton->save("skeleton",out,nestingLevel+1);
	}
	if(physics!=0x0){
		physics->save("physics",out,nestingLevel+1);
	}
	if(animations!=0x0){
		animations->save("animations",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

MoveTransitionType::MoveTransitionType(){}

MoveTransitionType::MoveTransitionType(MoveTransitionType&& other)
	:time(other.time),offset_x(other.offset_x),offset_y(other.offset_y)
{
}
MoveTransitionType::MoveTransitionType(rapidxml::xml_node<>*  node){
	setValue(&time, node->first_attribute("time"));
	setValue(&offset_x, node->first_attribute("offset_x"));
	setValue(&offset_y, node->first_attribute("offset_y"));
}

MoveTransitionType::~MoveTransitionType(){
}

void MoveTransitionType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" time=\""<<time<<"\"";
		out<<" offset_x=\""<<offset_x<<"\"";
		out<<" offset_y=\""<<offset_y<<"\"";
		out<<"/>";
	}
	out<<endl;
}

RotateTransitionType::RotateTransitionType(){}

RotateTransitionType::RotateTransitionType(RotateTransitionType&& other)
	:time(other.time),q_z(other.q_z),q_w(other.q_w)
{
}
RotateTransitionType::RotateTransitionType(rapidxml::xml_node<>*  node){
	setValue(&time, node->first_attribute("time"));
	setValue(&q_z, node->first_attribute("q_z"));
	setValue(&q_w, node->first_attribute("q_w"));
}

RotateTransitionType::~RotateTransitionType(){
}

void RotateTransitionType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" time=\""<<time<<"\"";
		out<<" q_z=\""<<q_z<<"\"";
		out<<" q_w=\""<<q_w<<"\"";
		out<<"/>";
	}
	out<<endl;
}

ScaleTransitionType::ScaleTransitionType(){}

ScaleTransitionType::ScaleTransitionType(ScaleTransitionType&& other)
	:time(other.time),scale_x(other.scale_x),scale_y(other.scale_y)
{
}
ScaleTransitionType::ScaleTransitionType(rapidxml::xml_node<>*  node){
	setValue(&time, node->first_attribute("time"));
	setValue(&scale_x, node->first_attribute("scale_x"));
	setValue(&scale_y, node->first_attribute("scale_y"));
}

ScaleTransitionType::~ScaleTransitionType(){
}

void ScaleTransitionType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" time=\""<<time<<"\"";
		out<<" scale_x=\""<<scale_x<<"\"";
		out<<" scale_y=\""<<scale_y<<"\"";
		out<<"/>";
	}
	out<<endl;
}

SkeletonAnimationBoneType::SkeletonAnimationBoneType()
	:offset(0x0),rotate(0x0),scale(0x0){}

SkeletonAnimationBoneType::SkeletonAnimationBoneType(SkeletonAnimationBoneType&& other)
	:offset(move(other.offset)),rotate(move(other.rotate)),scale(move(other.scale)),id(other.id)
{
}
SkeletonAnimationBoneType::SkeletonAnimationBoneType(rapidxml::xml_node<>*  node)
	:offset(0x0),rotate(0x0),scale(0x0){
	setValue(&id, node->first_attribute("id"));
	for(xml_node<>* currNode = node->first_node("offset");
			currNode!=0;
			currNode = currNode->next_sibling("offset")){
		offset.push_back(new MoveTransitionType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("rotate");
			currNode!=0;
			currNode = currNode->next_sibling("rotate")){
		rotate.push_back(new RotateTransitionType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("scale");
			currNode!=0;
			currNode = currNode->next_sibling("scale")){
		scale.push_back(new ScaleTransitionType(currNode));
	}
}

SkeletonAnimationBoneType::~SkeletonAnimationBoneType(){
	for(vector<MoveTransitionType* >::iterator it = offset.begin();
			it!=offset.end();++it){
		delete *it;
	}
;	for(vector<RotateTransitionType* >::iterator it = rotate.begin();
			it!=rotate.end();++it){
		delete *it;
	}
;	for(vector<ScaleTransitionType* >::iterator it = scale.begin();
			it!=scale.end();++it){
		delete *it;
	}
;}

void SkeletonAnimationBoneType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" id=\""<<id<<"\"";
		out<<">\n";
	}
	for(auto iter:offset){
		iter->save("offset",out,nestingLevel+1);
	}
	for(auto iter:rotate){
		iter->save("rotate",out,nestingLevel+1);
	}
	for(auto iter:scale){
		iter->save("scale",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SkeletonAnimationType::SkeletonAnimationType()
	:event(0x0),bone(0x0),name(""),duration(0.0){}

SkeletonAnimationType::SkeletonAnimationType(SkeletonAnimationType&& other)
	:event(move(other.event)),bone(move(other.bone)),name(other.name),duration(other.duration)
{
}
SkeletonAnimationType::SkeletonAnimationType(rapidxml::xml_node<>*  node)
	:event(0x0),bone(0x0),name(""),duration(0.0){
	setValue(&name, node->first_attribute("name"));
	setValue(&duration, node->first_attribute("duration"));
	for(xml_node<>* currNode = node->first_node("event");
			currNode!=0;
			currNode = currNode->next_sibling("event")){
		event.push_back(new AnimationEventType(currNode));
	}
	for(xml_node<>* currNode = node->first_node("bone");
			currNode!=0;
			currNode = currNode->next_sibling("bone")){
		bone.push_back(new SkeletonAnimationBoneType(currNode));
	}
}

SkeletonAnimationType::~SkeletonAnimationType(){
	for(vector<AnimationEventType* >::iterator it = event.begin();
			it!=event.end();++it){
		delete *it;
	}
;	for(vector<SkeletonAnimationBoneType* >::iterator it = bone.begin();
			it!=bone.end();++it){
		delete *it;
	}
;}

void SkeletonAnimationType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<" name=\""<<name<<"\"";
		out<<" duration=\""<<duration<<"\"";
		out<<">\n";
	}
	for(auto iter:event){
		iter->save("event",out,nestingLevel+1);
	}
	for(auto iter:bone){
		iter->save("bone",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

SkeletonType::SkeletonType()
	:bones(0x0){}

SkeletonType::SkeletonType(SkeletonType&& other)
	:bones(move(other.bones))
{
}
SkeletonType::SkeletonType(rapidxml::xml_node<>*  node)
	:bones(0x0){
	for(xml_node<>* currNode = node->first_node("bones");
			currNode!=0;
			currNode = currNode->next_sibling("bones")){
		bones.push_back(new BoneType(currNode));
	}
}

SkeletonType::~SkeletonType(){
	for(vector<BoneType* >::iterator it = bones.begin();
			it!=bones.end();++it){
		delete *it;
	}
;}

void SkeletonType::save(std::string tagName, std::ostream& out,int nestingLevel){
	string nestPrefix;
	for(int i=1; i<nestingLevel;++i)
		nestPrefix+="\t";

	if(tagName.size()>0){
		out<<nestPrefix<<"<"<<tagName;
		out<<">\n";
	}
	for(auto iter:bones){
		iter->save("bones",out,nestingLevel+1);
	}
	if(tagName.size()>0){
		out<<nestPrefix<<"</"<<tagName<<">";
	}
	out<<endl;
}

