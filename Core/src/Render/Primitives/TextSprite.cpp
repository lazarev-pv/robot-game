#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/Resource/Font.h>
#include <malloc.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>


using namespace game;
using namespace glm;
using namespace std;

void SpriteText::genBuffers(){

	if(text.size()==0)
		return;

	//fill screenWidth, screenHeight, w,h
	vector<GLfloat> buffer;

	float x=0,y=0,maxH=0;

	AtlasProperties atlas = font->getAtlas(size);
	for(unsigned charNum = 0; charNum<text.size();++charNum){
		const GlythData& glyth = atlas.getGlythData(text[charNum]);
		float x1 = x+glyth.topLeftPnt.x;
		float y1 = y+glyth.topLeftPnt.y;

		float x2 = x1+glyth.bitmapSize.x;
		float y2 = y1-glyth.bitmapSize.y;

		float u1 = glyth.offset.x;
		float v1 = 0.0f;//glyth.offset.y;
		float u2 = u1 + glyth.bitmapSize.x/atlas.getAtlasSize().x;
		float v2 = v1 + glyth.bitmapSize.y/atlas.getAtlasSize().y;
		maxH = std::max(maxH,glyth.topLeftPnt.y+glyth.bitmapSize.y-1);
		x+=glyth.advance.x;
	//	y+=glyth.advance.y;

		vector<GLfloat> tmp({
				x1,y1,0,1.0,u1,v1,
				x2,y1,0,1.0,u2,v1,
				x1,y2,0,1.0,u1,v2,
				x2,y1,0,1.0,u2,v1,
				x1,y2,0,1.0,u1,v2,
				x2,y2,0,1.0,u2,v2
		});

		buffer.insert(buffer.end(),tmp.begin(),tmp.end());
	}

	// aligning
	hw = 0.5f*x;
	hh = 0.4f*(maxH);

    {
        vec2 s = SpriteText::toScreenSize(vec2(hw,hh));
        screenHWidth = s.x;
        screenHHeight = s.y;
    }

	for(unsigned i = 0;i<buffer.size();i+=6){
		buffer[i]-=hw;
		buffer[i+1]-=hh/2;
	}

	sv.samplerValues["s_texture"]= atlas.texUid;
	sv.count = buffer.size()/6;
	sv.fillBuffer(buffer);
}

SpriteText::SpriteText(FontCHandle font, const std::string& text, int size):
		font(font), text(text),size(size)
{
    genBuffers();
}

SpriteText::SpriteText(const FontLAF& laf, const std::string& text)
	//:SpriteText(laf.font, text, laf.size){
	:font(laf.font), text(text),hw(0),hh(0),size(laf.size){

    genBuffers();
}

glm::vec2 SpriteText::toScreenSize(const glm::vec2& worldSize){
    ScreenSystem* screen =Game::getScreen();
    return vec2(worldSize.x*screen->getWidthDensity(),worldSize.y*screen->getHeightDensityNoAspect());
}

float SpriteText::getScreenHWidth(){
	return screenHWidth;
}
float SpriteText::getScreenHHeight(){
	return screenHHeight;
}

float SpriteText::getWorldHW(){
	return hw;
}

float SpriteText::getWorldHH(){
	return hh;
}

void SpriteText::setText(const std::string& newText){
	this->text = newText;
	genBuffers();
}

inline glm::mat4 createDrawMatrix(const glm::vec3& pos, const vec2& addScale, CameraCHandle camera){

    vec3 scale(Game::getScreen()->getWidthDensity()/**0.5*/*addScale.x,
            Game::getScreen()->getHeightDensityNoAspect()/**0.5*/*addScale.y,1.0);

    if(camera){
        vec3 botLeft = camera->convertToWorldCoordinates(vec2(-1,-1));
        vec3 upRight = camera->convertToWorldCoordinates(vec2(1,1));

        scale.x*=(upRight.x-botLeft.x)*0.5;
        scale.y*=(upRight.y-botLeft.y)*0.5;

        return camera->getMVPMatrix()*
                glm::translate(pos)*
                glm::scale(mat4(),scale);
    }else{
        return glm::translate(pos)*
                glm::scale(mat4(),scale);
    }
}


void SpriteText::draw(const glm::vec3& pos, const Color& color, CameraCHandle camera,bool shadow){
    if(text.size()==0){
        return;
    }
    if(shadow){
        ScreenSystem* screen = Game::getScreen();
        const Texture& bufferTex = screen->getFrameTexture(getFrameId());

        screen->bindFrame(getFrameId());
        sv.uniformValues["u_textColor"]=UniformValue(Color(color).setAlpha(1.0));
        sv.uniformValues["u_mvp_matrix"]=UniformValue(createMatrix(vec2(0,0),vec2(1.0f/hw,1.0f/hh)));
        Game::getShaderSystem()->get("text").draw(sv);

        screen->mainFrame();
        Game::getPrimitiveDrawer()->drawTexture(
                createDrawMatrix(pos, vec2(hw,hh), camera),
                bufferTex,
                "blur");
    }

    drawImpl(pos, color, camera);
}


void SpriteText::drawImpl(const glm::vec3 &pos, const Color &color, CameraCHandle camera) {
    if(text.size()==0)
        return;

    mat4 m = createDrawMatrix(camera?pos:vec3(pos.x,pos.y,0.0f), vec2(1,1), camera);
   // Game::getPrimitiveDrawer()->drawRectangleBorder(m*scale(mat4(1.0f),hw,hh,1.0f), Color::Black);
    sv.uniformValues["u_textColor"]=UniformValue(color);
    sv.uniformValues["u_mvp_matrix"] = m;
    Game::getShaderSystem()->get("text").draw(sv);
}

ScreenFrameId SpriteText::getFrameId() {
    static ScreenFrameId frameId = Game::getScreen()->generateFrameNumber(
            ivec2(64,32));
    return frameId;
}
