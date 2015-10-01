#include <ChibiEngine/Render/Sprites/NineSliceSprite.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/ScreenSystem.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace game;
using namespace glm;
using namespace std;

static constexpr int VERTS_PER_FRAME=16;
static constexpr int INDICES_PER_FRAME=9*2*3;
static constexpr int COMPONENTS_PER_VERTEX=5;

int NineSliceSprite::getFrameNum4Draw(const std::string& name) const{
	if(name.size()==0){
		return 0;
	}
    if(info->animations.size()==0)
        return 0;

	auto it = info->animations.find(name);
	if(it==info->animations.end()){
		return 0;
	}

	return it->second->getNumber();
}

void NineSliceSprite::draw(const glm::vec3& pos, const std::string& frameName) const{
	int offset = VERTS_PER_FRAME*COMPONENTS_PER_VERTEX*getFrameNum4Draw(frameName);

	glm::mat4 result = glm::translate(vec3(pos.x,pos.y,0.0f))*glm::scale(vec3(
			Game::getScreen()->getWidthDensity(),
			2.0/Game::getScreen()->getHeight(), //No aspect ratio multiply!
			1.0));

    sv.startOffset = offset;
    Game::getShaderSystem()->get("sprite")
            .draw(sv,unordered_map<string, UniformValue>({
            {"u_ambientLight",UniformValue(Color::White)},
            {"u_mvp_matrix",UniformValue(result)}}));

}

void NineSliceSprite::createIndexBuffer(){

	GLubyte inds[] = {
			0,8,12, // lb
			8, 4, 12,
			12, 4,13,//b
			4,5,13,
			13,5,1,//rb
			5,9,1,
			8,10,4,//l
			10,7,4,
			5,6,9,//r
			6,11,9,
			10,2,7,//ul
			2,14,7,
			7,14,6,//u
			14,15,6,
			6,15,11,//ur
			15,3,11,
			4,7,5,//c
			7,6,5
	};

    std::vector<GLubyte> indices;
    if(info->animations.size()>0) {
        indices.resize(INDICES_PER_FRAME * info->animations.size());
        for (const auto& frame : info->animations) {
            int num = frame.second->initialOffset / 20;

            float currentIndex = num * VERTS_PER_FRAME;
            for (int i = 0; i < INDICES_PER_FRAME; ++i) {
                indices[num * INDICES_PER_FRAME + i] = inds[i] + currentIndex;
            }
        }
    }else{
        indices.resize(INDICES_PER_FRAME);
        for (int i = 0; i < INDICES_PER_FRAME; ++i) {
            indices[i] = inds[i];
        }
    }

	sv.fillIndex(indices);
}

NineSliceSprite::NineSliceSprite(SpriteInfo::CHandle  data, float hw, float hh):
    info(data),hw(hw),hh(hh){
	createIndexBuffer();
	genVertsBuffer(hw,hh);

    sv.samplerValues["s_texture"]=info->texture->getUID();
    sv.count = INDICES_PER_FRAME;
}

void NineSliceSprite::setSize(const glm::vec2& size){
	genVertsBuffer(size.x,size.y);
}

void NineSliceSprite::genVertsFrame(float hw, float hh, SpriteInfo::FrameCHandle frame, std::vector<GLfloat>& verts){

	vec2 t1 = frame->topLeftPnt;
	vec2 t2 = frame->bottomRightPnt;

	float thw = std::max(hw, 0.5f*(info->left+info->right));
	float thh = std::max(hh, 0.5f*(info->top+info->bot));

	float kx = 0.5*(t2.x-t1.x)/info->hw;
	float ky = 0.5*(t2.y-t1.y)/info->hh;

	float dt74x = info->left*kx;
	float dt65x = info->right*kx;
	float dt76y = info->top*ky;
	float dt45y = info->bot*ky;

	GLfloat vertsArray[] = {
		-thw, -thh, 0, t1.x, t2.y, //0
		thw, -thh, 0, t2.x, t2.y, //1
		-thw, thh, 0, t1.x, t1.y, //2
		thw, thh, 0, t2.x, t1.y, //3

		-thw + info->left, -thh + info->bot, 0, t1.x + dt74x, t2.y - dt45y, //4 near 0
		thw - info->right, -thh + info->bot, 0, t2.x-dt65x, t2.y-dt45y, //5 near 1
		thw - info->right, thh - info->top, 0, t2.x-dt65x, t1.y+dt76y, // 6 near 3
		-thw + info->left, thh - info->top, 0, t1.x+dt74x, t1.y+dt76y, // 7 near 2

		-thw, -thh + info->bot, 0, t1.x, t2.y - dt45y,// 8 near 0,4
		thw, -thh + info->bot, 0, t2.x, t2.y - dt45y, //9 near 1,5
		-thw, thh - info->top, 0, t1.x, t1.y+dt76y, //10 near 2,7
		thw, thh - info->top, 0, t2.x, t1.y+dt76y, // 11 near 3,6

		-thw + info->left, -thh, 0, t1.x + dt74x, t2.y, // 12
		thw - info->right, -thh, 0, t2.x - dt65x, t2.y, // 13
		-thw + info->left, thh, 0, t1.x + dt74x, t1.y, // 14
		thw - info->right, thh, 0, t2.x - dt65x, t1.y // 15

	};
    int number = frame->parent!=nullptr?frame->parent->getNumber():0;
	for(int i=0; i<VERTS_PER_FRAME*COMPONENTS_PER_VERTEX;++i){
		verts[i+number*VERTS_PER_FRAME*COMPONENTS_PER_VERTEX] = vertsArray[i];
	}
}

void NineSliceSprite::genVertsBuffer(float hw, float hh){
	this->hw=hw;
	this->hh=hh;

    int framesCount = (info->animations.size()>0)?info->animations.size():1;
	std::vector<GLfloat> verts(VERTS_PER_FRAME*COMPONENTS_PER_VERTEX*framesCount);

    if(info->animations.size()>0) {
        for (const auto& frame : info->animations) {
            genVertsFrame(hw, hh, frame.second->frames[0].get(), verts);
        }
    }else{
        genVertsFrame(hw, hh, info->frame.get(), verts);
    }

	sv.fillBuffer(verts);
}
