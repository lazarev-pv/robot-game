#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChedLevel.h>
#include "ChedPhysBox.h"
#include "ChedBone.h"

using namespace game;
using namespace glm;
using namespace std;

ChedPhysBox::ChedPhysBox(ChedLevel* level, const glm::vec2& pos, float hw, float hh)
:friction(1),
 density(1),
 sensor(true){
	this->pos=pos;
	this->hw=hw;
	this->hh=hh;
	skeletonChanged(level);
}

ChedPhysBox::ChedPhysBox(ChedLevel* level, CharBoxType* iter)
:friction(iter->friction),
 density(iter->density),
 name(iter->name),
 sensor(iter->sensor){
	this->pos=vec2(iter->pox_x,iter->pox_y);
	rotation = iter->rotation;
	this->hw=iter->hw;
	this->hh=iter->hh;
	skeletonChanged(level);
}

void ChedPhysBox::setParent(ChedLevel* level, ChedBone* bone){
	this->parent = bone;
	skeletonChanged(level);
}

ChedPhysBox::~ChedPhysBox(){
}

void ChedPhysBox::scale(const glm::vec2& currPnt, const glm::vec2& delta){

}

void ChedPhysBox::rotate(const glm::vec2 &currPnt, const glm::vec2 &delta){
	vec2 pos = this->pos+(parent!=nullptr?parent->getPosition():vec2(0,0));
	//vec2 pos = (parent!=nullptr?parent->getPosition():vec2(0,0));
	vec2 n = normalize(currPnt-pos);

	/*normal.setPosition(0,vec3(pos,0));
	normal.setPosition(1,vec3(n+pos,0));*/
	if(pos.x<currPnt.x){
		rotation = -(M_PI_2 - glm::asin(n.y));
	}else{
		rotation = (M_PI_2 -glm::asin(n.y));
	}

	if(parent!=nullptr){
		rotation-=parent->getRotation();
	}
	skeletonChanged(ChedLevel::instance());
}

void ChedPhysBox::move(const glm::vec2 currPnt, const glm::vec2 &delta){
	pos+=delta;
}

void ChedPhysBox::draw(CameraCHandle camera){
	ChedObject::draw(camera);

    Game::getPrimitiveDrawer()->drawRectangleBorder(MatrixBuilder()
            .pos(parent?parent->getPosition()+pos:pos)
            .scale(vec2(hw,hh))
            .rotate(parent?parent->getRotation()+rotation:rotation,parent?-pos:vec2(0,0))
            .camera(camera)
            .toMatrix(),
            highligthed?Color::Yellow:Color::White);
}

void ChedPhysBox::skeletonChanged(ChedLevel* level){
	// pos - recalcing at draw
	// hw, hh - in constructor
	vec2 pos = parent!=nullptr?
			parent->getPosition()+this->pos:
			this->pos;
	float rotation = parent!=nullptr?
			parent->getRotation()+this->rotation:
			this->rotation;

	calcBoundVerts(MatrixBuilder()
						   .pos(pos)
						   .rotate(rotation,parent==nullptr?vec2(0,0):-this->pos)
							.toMatrix()
			);
}


CharBoxType* ChedPhysBox::save(){
	CharBoxType* d = new CharBoxType();
	d->pox_x = pos.x;
	d->pox_y = pos.y;
	d->hw = hw;
	d->hh = hh;
	d->friction = friction;
	d->density = density;
	d->sensor = sensor;
	d->name=name;
	d->rotation = rotation;
	d->parent=parent!=nullptr?parent->getName():"";
	return d;
}

float ChedPhysBox::getRotation() const{
	return this->rotation;
}

glm::vec2 ChedPhysBox::getPosition() const{
	return this->pos;
}

glm::vec2 ChedPhysBox::getBegin() const{
	return getPosition();
}

const glm::vec2 ChedPhysBox::getDirection() const{
	return vec2(0,0);
}
