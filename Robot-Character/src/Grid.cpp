#include <Grid.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/Sprites/PatternSprite.h>
#include <sstream>
#include <iomanip>

using namespace game;
using namespace std;
using namespace glm;

const static int GRID_ELS = 10;
const static float DELTA = 0.2;
const static float SEG_LEN = 0.05;
//2 poits per Element per 2 directions per 2 axis + 2 axis with 2 points
const static int INDEX_NUM = GRID_ELS*2*2*2+4;

inline void pushPnt(float x, float y, vector<GLfloat> &vects){
	vects.push_back(x);
	vects.push_back(y);
	vects.push_back(0.0f);
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

Grid::Grid(float hw, float hh){
	SpriteInfo::CHandle info = Game::getResources()->findSpriteInfo("back");
	backScale = vec2(0.4/info->hw, 0.4/info->hh);
	background = new PatternSprite(info, 8.00, 8.00);

    sv.count = INDEX_NUM;
    sv.uniformValues["u_color"]=UniformValue(Color::Green);
    sv.drawMode=GL_LINES;
	{
		vector<GLubyte> inds(INDEX_NUM);
		for(size_t i=0;i<inds.size();++i){
			inds[i]=i;
		}
		sv.fillIndex(inds);
	}{
		vector<GLfloat> vects;
		// axises - X
		pushPnt(-DELTA*GRID_ELS,0,vects);
		pushPnt(DELTA*GRID_ELS,0,vects);
		// axises - Y
		pushPnt(0,DELTA*GRID_ELS,vects);
		pushPnt(0,-DELTA*GRID_ELS,vects);
		// X segments - negative
		for(int i=1;i<=GRID_ELS;++i){
			pushPnt(-DELTA*i,SEG_LEN,vects);
			pushPnt(-DELTA*i,-SEG_LEN,vects);
		}
		// X segments - positive
		for(int i=1;i<=GRID_ELS;++i){
			pushPnt(DELTA*i,SEG_LEN,vects);
			pushPnt(DELTA*i,-SEG_LEN,vects);
		}
		// Y segments - negative
		for(int i=1;i<=GRID_ELS;++i){
			pushPnt(SEG_LEN,-DELTA*i,vects);
			pushPnt(-SEG_LEN,-DELTA*i,vects);
		}
		// y segments - positive
		for(int i=1;i<=GRID_ELS;++i){
			pushPnt(SEG_LEN,DELTA*i,vects);
			pushPnt(-SEG_LEN,DELTA*i,vects);
		}
		sv.fillBuffer(vects);
	}{
		FontLAF laf = Game::getLookAndFeel().font;
		for(int i=1;i<=GRID_ELS;++i){
			labels.push_back(Label(new SpriteText(laf,to_string_with_precision(-i*DELTA)),-DELTA*i));
		}
		for(int i=1;i<=GRID_ELS;++i){
			labels.push_back(Label(new SpriteText(laf,to_string_with_precision(i*DELTA)),DELTA*i));
		}
	}
}

Grid::~Grid(){
	delete background;
}

void Grid::draw(){
	CameraCHandle cam = Game::getCamera();
	background->draw(createMatrix(vec3(0,0,0),backScale,cam),Color::White);
    sv.uniformValues["u_mvp_matrix"]=UniformValue(cam->getMVPMatrix());
    Game::getShaderSystem()->get("simple").draw(sv);
	for(auto iter : labels){
		iter.sprite->draw(vec3(SEG_LEN*2,iter.offset,0),Color::Green, cam);
		iter.sprite->draw(vec3(iter.offset,SEG_LEN*2,0),Color::Green, cam);
	}
}
