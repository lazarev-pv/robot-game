#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <ChedObject.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChedLevel.h>

using namespace game;
using namespace glm;

ChedObject::ChedObject():
	highligthed(false),
	selected(false),
	hh(0),hw(0),
	normal(Color::White, {vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), vec3(0,0,0)}){

}

/** @see http://stackoverflow.com/questions/11716268/point-in-polygon-algorithm */
bool ChedObject::containsPoint(const glm::vec2& pnt){
	int i, j;
    bool result = false;
	for (i = 0, j = 3; i < 4; j = i++) {
		if ( ((verts[i].y>pnt.y) != (verts[j].y>pnt.y)) &&
				(pnt.x < (verts[j].x-verts[i].x) * (pnt.y-verts[i].y) / (verts[j].y-verts[i].y) + verts[i].x) )
			result = !result;
	}
	return result;
}

void ChedObject::draw(CameraCHandle camera){
	if(ChedLevel::instance()->isDebugMode())
		normal.draw(camera);
}

void ChedObject::setHighlighted(bool flag){
	highligthed = flag;
}
void ChedObject::setSelected(bool flag){
	selected = flag;
}

void ChedObject::calcBoundVerts(const glm::mat4& animMat){
	verts[0]=vec2(-hw,hh);
	verts[1]=vec2(hw,hh);
	verts[2]=vec2(hw,-hh);
	verts[3]=vec2(-hw,-hh);

	for(int i =0; i<4;++i){
        verts[i]=toVec2(animMat*vec4(verts[i],0,1));
	}

	for(int i = 0; i<4; ++i){
		normal.setPosition(i, vec3(verts[i],0));
	}
	normal.setPosition(4, vec3(verts[0],0));
}
