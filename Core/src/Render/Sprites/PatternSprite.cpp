#include <ChibiEngine/Render/Sprites/PatternSprite.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Resource/Resources.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace std;
using namespace glm;
using namespace game;

static const float EPSILON = 0.0001;

static void calcIndecies(int xLength, int yLength, vector<GLuint>& indices){

	for(int i=0; i<(xLength-1)*(yLength-1);++i){
		indices.push_back(0+i*4);
		indices.push_back(1+i*4);
		indices.push_back(2+i*4);
		indices.push_back(1+i*4);
		indices.push_back(3+i*4);
		indices.push_back(2+i*4);
	}
}

static void expand(float val, std::vector<GLfloat>& xInds){
	float xInteger, xMod;
	xMod = modf(val, &xInteger);
	int xCount = static_cast<int>(xInteger);

	for(int i = 0; i<=xCount;++i){
		xInds.push_back(i);
	}
	if(xMod>EPSILON){
		xInds.push_back(xInds.size()-1+xMod);
	}
}

PatternSprite::PatternSprite(SpriteInfo::CHandle pres, float xrepeats, float yrepeats)
	:texHW(pres->hw),
	 texHH(pres->hh),
	 targetHW(pres->hw*xrepeats),
	 targetHH(pres->hh*yrepeats),
	 name(pres->name){

	t1 = pres->frame->topLeftPnt;
	t2 = pres->frame->bottomRightPnt;

	genBuffers();
    sv.indexSize = GL_UNSIGNED_INT;
    sv.samplerValues["s_texture"]=pres->texture->getUID();
}

void PatternSprite::reGenBuffers(){
	genBuffers();
}

void PatternSprite::genBuffers(){
	std::vector<GLfloat> result;

	std::vector<float> xInds;
	std::vector<float> yInds;
	expand(targetHW/texHW, xInds);
	expand(targetHH/texHH, yInds);

	float dx=t2.x-t1.x;
	float dy=t1.y-t2.y;
	for(size_t y=0;y<yInds.size()-1;++y){
		for(size_t x=0;x<xInds.size()-1;++x){
			result.push_back(-targetHW + texHW*2*xInds[x]);
			result.push_back(-targetHH + texHH*2*yInds[y]);
			result.push_back(0.0);
			result.push_back(t1.x);
			result.push_back(t2.y);

			result.push_back(-targetHW + texHW*2*xInds[x]);
			result.push_back(-targetHH + texHH*2*yInds[1+y]);
			result.push_back(0.0);
			result.push_back(t1.x);
			result.push_back(t2.y+dy*(yInds[1+y]-y));

			result.push_back(-targetHW + texHW*2*xInds[1+x]);
			result.push_back(-targetHH + texHH*2*yInds[y]);
			result.push_back(0.0);
			result.push_back(t1.x+dx*(xInds[1+x]-x));
			result.push_back(t2.y);

			result.push_back(-targetHW + texHW*2*xInds[1+x]);
			result.push_back(-targetHH + texHH*2*yInds[1+y]);
			result.push_back(0.0);
			result.push_back(t1.x+dx*(xInds[1+x]-x));
			result.push_back(t2.y+dy*(yInds[1+y]-y));

		}
	}

	std::vector<GLuint> inds;
	calcIndecies(xInds.size(),yInds.size(), inds);

	sv.fillBuffer(result);
	sv.fillIndex(inds);
    sv.count = inds.size();
}

void PatternSprite::setTextureWidth(float textureWidth){
	this->texHW=textureWidth;
	reGenBuffers();
}
void PatternSprite::setTextureHeight(float textureHeight){
	this->texHH=textureHeight;
	reGenBuffers();
}
void PatternSprite::setTargetWidth(float targetW){
	this->targetHW=targetW;
	reGenBuffers();
}
void PatternSprite::setTargetHeight(float targetH){
	this->targetHW=targetH;
	reGenBuffers();
}

void PatternSprite::spread(const glm::vec2& targetSizeDelta){
	targetHW+=targetSizeDelta.x;
	targetHH+=targetSizeDelta.y;
	reGenBuffers();
}

void PatternSprite::draw(const glm::mat4& mat, const Color& ambient) const{
    Game::getShaderSystem()->get("sprite")
            .draw(sv,unordered_map<string, UniformValue>({
            {"u_ambientLight",UniformValue(ambient)},
            {"u_mvp_matrix",UniformValue(mat)}}));
}