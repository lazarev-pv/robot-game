#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <ChibiEngine/Render/Primitives/BoxArea.h>

using namespace game;
using namespace glm;

BoxArea::BoxArea(const glm::vec3& pos, const glm::vec2& size, float borderWidth, float rotation):
		pos(pos),
		pivotPoint(pos),
		rotation(rotation),
		hw(size.x),
		hh(size.y),
		borderWidth(borderWidth),
		normalColor(Color::White),
		allowAllMove(true){
	resetMoveStates();
	recalcVerts();
}

void BoxArea::setBorderWidth(float width){
	borderWidth = width;
}

bool BoxArea::contains(const glm::vec2& pnt){
	return containsPoint(pnt,allBox);
}

bool BoxArea::isBorderMoving() const{
   return (leftMoving || rightMoving || topMoving || bottomMoving)
            &&
    !(leftMoving && rightMoving && topMoving && bottomMoving);
}

void BoxArea::setAllMoveEnabled(bool isEnabled){
	allowAllMove = isEnabled;
}

void BoxArea::setNormalColor(const Color& color){
	normalColor = color;
}

void BoxArea::grab(const glm::vec2& pnt){
	resetMoveStates();
	if(containsPoint(pnt,leftBox))
		leftMoving = true;
	if(containsPoint(pnt,rightBox))
		rightMoving = true;
	if(containsPoint(pnt,topBox))
		topMoving = true;
	if(containsPoint(pnt,bottomBox))
		bottomMoving = true;

	if(allowAllMove && !leftMoving && !rightMoving && !bottomMoving && !topMoving){
		leftMoving = true;
		rightMoving = true;
		bottomMoving = true;
		topMoving = true;
	}
}

void BoxArea::release(){
	resetMoveStates();
}

void BoxArea::drag(const glm::vec2& delta){
	//todo prevent negative w, h
	vec3 oldPos = pos;
	if(leftMoving){
		hw-=delta.x/2;
		pos.x+=delta.x/2;
	}
	if(rightMoving){
		hw+=delta.x/2;
		pos.x+=delta.x/2;
	}
	if(topMoving){
		hh+=delta.y/2;
		pos.y+=delta.y/2;
	}
	if(bottomMoving){
		hh-=delta.y/2;
		pos.y+=delta.y/2;
	}
	pivotPoint+=(pos-oldPos);

	recalcVerts();
}

void BoxArea::resetMoveStates(){
	leftMoving = false;
	topMoving = false;
	rightMoving = false;
	bottomMoving = false;
}

inline void applyMat(const mat4& result, vec2* verts,int n){
	for(int i =0; i<n;++i){
		vec4 tmp(verts[i],0,1);
		tmp=result*tmp;
		verts[i].x=tmp.x/tmp.w;
		verts[i].y=tmp.y/tmp.w;
	}
}

void rotateI(glm::vec3 offset, glm::vec3 pos,  float hw, float hh, float angle, glm::vec2* verts){

	glm::mat4 result = glm::translate(pos)
					* glm::rotate(mat4(1.0),angle,glm::vec3(0.0f,0.0f,1.0f))
					* glm::translate(offset);
	verts[0]=vec2(-hw,hh);
	verts[1]=vec2(hw,hh);
	verts[2]=vec2(hw,-hh);
	verts[3]=vec2(-hw,-hh);

	applyMat(result,verts,4);
}

void BoxArea::recalcVerts(){
	rotateI(vec3(0,0,0),pos,hw,hh, rotation,allBox);
	rotateI(vec3(-hw+borderWidth*0.5,0,0),pos,borderWidth,hh, rotation,leftBox);
	rotateI(vec3(hw-borderWidth*0.5,0,0),pos,borderWidth,hh, rotation,rightBox);

	rotateI(vec3(0,hh-borderWidth*0.5,0),pos,hw,borderWidth, rotation,topBox);
	rotateI(vec3(0,-hh+borderWidth*0.5,0),pos,hw,borderWidth, rotation,bottomBox);
}

bool BoxArea::containsPoint(const glm::vec2& pnt, glm::vec2 *verts){
	int i, j, c = 0;
	for (i = 0, j = 3; i < 4; j = i++) {
		if ( ((verts[i].y>pnt.y) != (verts[j].y>pnt.y)) &&
				(pnt.x < (verts[j].x-verts[i].x) * (pnt.y-verts[i].y) / (verts[j].y-verts[i].y) + verts[i].x) )
			c = !c;
	}
	return c;
}

void BoxArea::draw(CameraCHandle cam, const Color& color){
	draw(cam,color,Color::Blue, Game::getPrimitiveDrawer());
}

void BoxArea::draw(CameraCHandle cam, const Color& color,const Color& moveColor, const PrimitiveDrawer* drawer){
	float cx = pos.x/* 0.5*(left+right)*/;
	float cy = pos.y/*0.5*(top+bottom)*/;
	float hb = borderWidth*0.5;
	//float hw = /*0.5*(right-left)*/;
	//float hh = 0.5*(top-bottom);
	float left = pos.x - hw;
	float right = pos.x + hw;
	float top = pos.y + hh;
	float bottom = pos.y - hh;
	drawer->drawRectangleBorder(MatrixBuilder()
            .pos(vec3(left + hb, cy, 0))
            .scale(vec2(hb, hh))
            .rotate(rotation, -vec3(-hw + hb, 0, 0))
            .camera(cam)
            .toMatrix(),
            leftMoving ? moveColor : color);

	drawer->drawRectangleBorder(MatrixBuilder()
            .pos(vec3(right - hb, cy, 0))
            .scale(vec2(hb, hh))
            .rotate(rotation,-vec3(hw - hb, 0, 0))
            .camera(cam)
            .toMatrix(),
            rightMoving ? moveColor : color);

    drawer->drawRectangleBorder(MatrixBuilder()
            .pos(vec3(cx, top - hb, 0))
            .scale(vec2(hw, hb))
            .rotate(rotation,-vec3(0, hh - hb, 0))
            .camera(cam)
            .toMatrix(),
            topMoving ? moveColor : color);

    drawer->drawRectangleBorder(MatrixBuilder()
            .pos(vec3(cx, bottom + hb, 0))
            .scale(vec2(hw, hb))
            .rotate(rotation,-vec3(0, -hh + hb, 0))
            .camera(cam)
            .toMatrix(),
            bottomMoving ? moveColor : color);

	/*float bb=0.01;
	for(int i=0;i<4;++i){
		drawer->drawRectangleBorder(vec3(leftBox[i],0),bb,bb,vec4(0.8,0,0,1),cam);
	}*/
	/*for(int i=0;i<4;++i){
		drawer->drawRectangleBorder(vec3(topBox[i],0),bb,bb,vec4(0,0.8,0,0.7),cam);
	}
	for(int i=0;i<4;++i){
		drawer->drawRectangleBorder(vec3(leftBox[i],0),bb,bb,vec4(0,0,0.8,0.7),cam);
	}
	for(int i=0;i<4;++i){
		drawer->drawRectangleBorder(vec3(rightBox[i],0),bb,bb,vec4(0.3,0.3,0,0.7),cam);
	}
	for(int i=0;i<4;++i){
		drawer->drawRectangleBorder(vec3(bottomBox[i],0),bb,bb,vec4(0,0.3,0.3,0.7),cam);
	}
*/
}

void BoxArea::draw(CameraCHandle cam) {
	draw(cam,normalColor,Color::Red, Game::getPrimitiveDrawer());
}

glm::vec3 BoxArea::getPosition() const{
	return pos;
}

void BoxArea::setPosition(const glm::vec3& pos){
	this->pos = pos;
	recalcVerts();
/*	float hw = 0.5*(right-left);
	float hh = 0.5*(top-bottom);

	left=pos.x-hw;
	right=pos.x+hw;
	top=pos.y+hh;
	bottom=pos.y-hh;*/
}

void BoxArea::move(const glm::vec2& delta){
	pos+=vec3(delta,0);
	recalcVerts();
}

float BoxArea::getHWidth() const{
	return hw;
}

float BoxArea::getHHeight() const{
	return hh;
}
/*
void BoxArea::setRotation(float rot){
	this->rotation = rot;
	recalcVerts();
}*/

void BoxArea::rotate(float rot){
	this->rotation+=rot;
	recalcVerts();
}



void BoxArea::rotate(float rot, glm::vec2 pivPnt){
	glm::mat4 result = glm::translate(vec3(pivPnt,0))
					* glm::rotate(mat4(1.0),rot,glm::vec3(0.0f,0.0f,1.0f))
					* glm::translate(vec3(-pivPnt,0));
	applyMat(result,allBox,4);
	applyMat(result,topBox,4);
	applyMat(result,bottomBox,4);
	applyMat(result,leftBox,4);
	applyMat(result,rightBox,4);
    vec2 p(pos.x,pos.y);
	applyMat(result,&p,1);

	vec2 end=(topBox[0]+topBox[1])*0.5f;
	vec2 begin=(bottomBox[0]+bottomBox[1])*0.5f;

	if(begin.x<end.x){
		rotation =-glm::acos((end.y-begin.y)/(2*hh));
	}else{
		rotation = glm::acos((end.y-begin.y)/(2*hh));
	}
}


void BoxArea::setSize(const glm::vec2& size){
    hw = size.x;
    hh = size.y;
    recalcVerts();
}

void BoxArea::setRotation(float r){
    rotation = r;
}
float BoxArea::getRotation() const{
    return rotation;
}
