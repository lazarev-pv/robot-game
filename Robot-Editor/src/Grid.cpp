#define GLM_FORCE_RADIANS

#include <Grid.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace game;
using namespace std;
using namespace glm;

const static Color GRID_COLOR(0.8f,0.8f,0.0f,0.3f);
const static float DELTA=1.0;

inline void pushPnt(float x, float y, vector<GLfloat> &vects){
	vects.push_back(x);
	vects.push_back(y);
	vects.push_back(0.0f);
}

Grid::Grid(int xCelsHC, int yCelsHC){
    pos=vec3(0,0,0);
	sv.count=xCelsHC*2*2+2+yCelsHC*2*2+2;
    sv.uniformValues["u_color"]=GRID_COLOR;
    sv.drawMode=GL_LINES;
	{
		vector<GLubyte> inds(sv.count);
		for(size_t i=0;i<inds.size();++i){
			inds[i]=i;
		}
		sv.fillIndex(inds);
	}{
		vector<GLfloat> vects;

		// zero axises - X
		pushPnt(-DELTA*yCelsHC,0,vects);
		pushPnt(DELTA*yCelsHC,0,vects);
		// zero axises - Y
		pushPnt(0,DELTA*xCelsHC,vects);
		pushPnt(0,-DELTA*xCelsHC,vects);

		// X axis - negative
		for(int i=1;i<=xCelsHC;++i){
			pushPnt(-DELTA*yCelsHC, -DELTA*i,vects);
			pushPnt(DELTA*yCelsHC, -DELTA*i,vects);
		}
		// X segments - positive
		for(int i=1;i<=xCelsHC;++i){
			pushPnt(-DELTA*yCelsHC, DELTA*i,vects);
			pushPnt(DELTA*yCelsHC, DELTA*i,vects);
		}
		// Y segments - negative
		for(int i=1;i<=yCelsHC;++i){
			pushPnt(-DELTA*i,-DELTA*xCelsHC,vects);
			pushPnt(-DELTA*i,DELTA*xCelsHC,vects);
		}
		// y segments - positive
		for(int i=1;i<=yCelsHC;++i){
			pushPnt(DELTA*i,-DELTA*xCelsHC,vects);
			pushPnt(DELTA*i,DELTA*xCelsHC,vects);
		}

		sv.fillBuffer(vects);
	}
}

void Grid::draw(){
    mat4 m = Game::getCamera()->getMVPMatrix()*translate(mat4(1.0f),pos);
    sv.uniformValues["u_mvp_matrix"]=UniformValue(m);
    Game::getShaderSystem()->get("simple").draw(sv);
}
