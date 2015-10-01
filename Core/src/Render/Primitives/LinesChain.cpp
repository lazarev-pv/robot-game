#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/Primitives/LinesChain.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace game;
using namespace std;
using namespace glm;

LinesChain::LinesChain(const Color& color){
    sv.uniformValues["u_color"]=UniformValue(color);
    sv.drawMode=GL_LINE_STRIP;
}
LinesChain::LinesChain(const Color& color, const std::vector<glm::vec3>& _points){
    sv.uniformValues["u_color"]=UniformValue(color);
    sv.drawMode=GL_LINE_STRIP;
	add(_points);
}

void LinesChain::draw(CameraCHandle camera) const{
	if(points.size()<2)
		return;

    sv.uniformValues["u_mvp_matrix"]=UniformValue(camera->getMVPMatrix());
    sv.count=points.size()/3;
    Game::getShaderSystem()->get("simple").draw(sv);
    /*Game::getShaderSystem()->GetSimpleShader()->
			draw(GL_LINE_STRIP,camera->getMVPMatrix(),buffer[1],buffer[0],color,points.size()/3);*/
}


void LinesChain::genBuffer(){
	if(points.size()<1)
		return;

	vector<GLubyte> pnts(points.size()/3);
	for(size_t i=0;i<points.size()/3;++i){
		pnts[i]=i;
	}
	sv.fillIndex(pnts);
	sv.fillBuffer(points);
}

void LinesChain::add(const std::vector<glm::vec3>& _points){
	for(auto point: _points){
		points.push_back(point.x);
		points.push_back(point.y);
		points.push_back(point.z);

	}
	genBuffer();
}

void LinesChain::add(glm::vec3 point){
	points.push_back(point.x);
	points.push_back(point.y);
	points.push_back(point.z);
	genBuffer();
}

void LinesChain::move(int index,const glm::vec3& delta){
	assert(static_cast<int>(points.size())/3>index);

	points[index*3+0]+=delta.x;
	points[index*3+1]+=delta.y;
	points[index*3+2]+=delta.z;

	genBuffer();
}

void LinesChain::setPosition(int index,const glm::vec3& delta){
	assert(static_cast<int>(points.size())/3>index);

	points[index*3+0]=delta.x;
	points[index*3+1]=delta.y;
	points[index*3+2]=delta.z;

	genBuffer();
}

void LinesChain::deleteElement(int index){
	assert(static_cast<int>(points.size())/3>index);

	points.erase(points.begin()+index*3,points.begin()+(index+1)*3);
	genBuffer();
}
