#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include "ChedSlot.h"
#include "ChedBone.h"
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonSlot.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>
#include <ChedLevel.h>
#include <complex>

using namespace game;
using namespace glm;
using namespace std;

ChedSlot::ChedSlot(ChedLevel* level, SkeletonSlot* pic, ChedBone* bone):
	slot(pic),bone(bone){

	hw = 1.0f;
	hh = 1.0f;
	skeletonChanged(level);
}

void ChedSlot::addPicMeta(const std::string& name, const glm::vec3& pos, const glm::vec2& scale, float rot){
	meta.insert(make_pair(name,PicMeta{pos, scale, rot}));
}

void ChedSlot::addPic(const TexturedObject* obj, const std::string& slotName, const glm::vec2& scale){
	PicMeta picMeta{vec3(0,0,0), scale, 0.0f};
	meta.insert(make_pair(slotName,picMeta));
	slot->addPicture(slotName,obj, picMeta.pos, picMeta.scale, picMeta.rotation);
	slot->setActive(slotName);
}

ChedSlot::ChedSlot(ChedLevel* level, SkeletonSlot* pic, BoneSlotType* data, ChedBone* bone):
	ChedSlot(level, pic, bone){

	defaultSlot = data->defaultPres;
	for(auto pres : data->pres){
		addPicMeta(pres->name, vec3(parse(pres->pos),0),parse(pres->scale),pres->rotation);
	}
}

void ChedSlot::skeletonChanged(ChedLevel* level){
	vec2 size = slot->getActive().getTextureObject()->getSize();
	calcBoundVerts(slot->getActive().getDrawMatrix()
			* glm::scale(vec3(size.x,size.y,1.0f))) ;
	fireEvent("");
}

/*
const std::string& ChedSlot::getPresName() const{
    return pres->getName();
}
*/
/*

BodyPartType* ChedSlot::save(){
	BodyPartType* part = new BodyPartType();
	part->pres = pres->getName();
	part->pos = create2D(pos);
	part->rotation = rotation;
	part->scale = create2D(scale);
	return part;
}
*/
/*

void ChedSlot::rotate(const glm::vec2& currPnt){
	vec2 pos = this->pos+(parent!=nullptr?parent->getPosition():vec2(0,0));
	//vec2 pos = (parent!=nullptr?parent->getPosition():vec2(0,0));
	vec2 n = normalize(currPnt-pos);

	*/
/*normal.setPosition(0,vec3(pos,0));
	normal.setPosition(1,vec3(n+pos,0));*//*

	if(pos.x<currPnt.x){
		rotation = -(M_PI_2 - glm::asin(n.y));
	}else{
		rotation = (M_PI_2 -glm::asin(n.y));
	}

	if(parent!=nullptr){
		rotation-=parent->getRotation();
	}
	recalcSupportData();
}

void ChedSlot::rotate(float rot){
	rotation+=rot;
	recalcSupportData();
}
*/

/*
void ChedSlot::move(const glm::vec2 currPnt, const glm::vec2& delta){

	//vec2 delta=pos+_delta;

	vec4 _d = glm::rotate(-rotation,vec3(0,0,1))*vec4(delta*/
/*-parent->getPosition()*//*
,0,1);

	pos.x-=_d.x;//+parent->getPosition().x;
	pos.y-=_d.y;//+parent->getPosition().y;

	recalcSupportData();
}
*/

void ChedSlot::draw(CameraCHandle camera){
	ChedObject::draw(camera);

	if(selected || highligthed){

		vec2 size = slot->getActive().getTextureObject()->getSize();

		Game::getPrimitiveDrawer()->drawRectangleBorder(
				camera->getMVPMatrix()
						* slot->getActive().getDrawMatrix()
						* glm::scale(vec3(size.x,size.y,1.0f)),
				selected?Color::White:Color::Yellow);
	}
}

void ChedSlot::scale(const glm::vec2& currPnt, const glm::vec2& delta){
	PicMeta& m = meta.find(slot->getActive().getName())->second;
	m.scale*=vec2(1,1)+delta/vec2(hw,hh);
	slot->getActive().resetInitMatrix(m.pos,m.scale,m.rotation);

	ChedLevel::instance()->getSkeleton()->refreshAnimation();
	skeletonChanged(ChedLevel::instance());
}

BoneSlotType* ChedSlot::save() const{
	BoneSlotType* xmlSlot = new BoneSlotType();
	xmlSlot->defaultPres = defaultSlot;
	xmlSlot->name = slot->getName();
	xmlSlot->zOrder = slot->getZOrder();
	xmlSlot->id = slot->getId();
	for(const SkeletonPicture& pic : slot->getPresentations()){
		BodyPartType* xmlPic = new BodyPartType();
		xmlPic->name = pic.getName();
		xmlPic->pres = pic.getTextureObject()->getName();
		const auto m = meta.find(xmlPic->name);
		if(m == meta.end())
			continue;

		xmlPic->rotation = m->second.rotation;
		xmlPic->pos = create2D(m->second.pos);
		xmlPic->scale = create2D(m->second.scale);
		xmlSlot->pres.push_back(xmlPic);
	}
	return xmlSlot;
}


void ChedSlot::rotate(const glm::vec2 &currPnt, const glm::vec2 &delta){
	PicMeta& m = meta.find(slot->getActive().getName())->second;
	m.rotation += getAngle(currPnt, delta, toVec2(m.pos) + bone->getPosition());

	slot->getActive().resetInitMatrix(m.pos,m.scale,m.rotation);
	ChedLevel::instance()->getSkeleton()->refreshAnimation();
	skeletonChanged(ChedLevel::instance());
}

void ChedSlot::move(const glm::vec2 currPnt, const glm::vec2 &delta){
	PicMeta& m = meta.find(slot->getActive().getName())->second;
	{
		vec4 pos = glm::inverse(slot->getJoint()->getAnimationMatrix())*vec4(delta,0,0);
		m.pos+=vec3(pos.x,pos.y,pos.z);
	}

	slot->getActive().resetInitMatrix(m.pos,m.scale,m.rotation);
	ChedLevel::instance()->getSkeleton()->refreshAnimation();
	skeletonChanged(ChedLevel::instance());
}

float ChedSlot::getRotation() const{
	return meta.find(slot->getActive().getName())->second.rotation;
}

glm::vec2 ChedSlot::getPosition() const{
	return toVec2(meta.find(slot->getActive().getName())->second.pos)+bone->getPosition();
}

const glm::vec2 ChedSlot::getDirection() const{
	vec4 p = glm::rotate(getRotation(), vec3(0.0f, 0.0f, 1.0f))*vec4(0,1,0,0);
	return vec2(p.x,p.y);
}

void ChedSlot::setActiveSlotDefault(){
	defaultSlot = slot->getActive().getName();
}

std::vector<std::string> ChedSlot::getSlots() const{
	vector<string> res;
	for(auto& iter : meta){
		res.push_back(slot->getPicture(iter.first).getTextureObject()->getName());
	}
	return res;
}

glm::vec2 ChedSlot::getBegin() const{
	return getPosition();
}

/*
void ChedSlot::recalcSupportData(){
	// pos - recalcing at draw
	// hw, hh - in constructor
	vec2 pos = parent!=nullptr?
			parent->getPosition()+this->pos:
			this->pos;
	float rotation = parent!=nullptr?
			parent->getRotation()+this->rotation:
			this->rotation;

	calcBoundVerts(pos, rotation,-this->pos);
}
*/
/*

void ChedSlot::setParent(ChedBone* parent){
	this->parent = parent;
	recalcSupportData();
}
*/
