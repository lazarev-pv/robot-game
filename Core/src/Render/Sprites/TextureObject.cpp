#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Resource/Resources.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace std;
using namespace glm;
using namespace game;


TexturedObject::TexturedObject(TextureCHandle texture, float weight, float height)
	:DrawableObject(vec2(weight*0.5,height*0.5),""),
     texture(texture){

	generateIndex();
	generateVertex(vec2(0, 1),vec2(1, 0));
    sv.samplerValues["s_texture"]=texture->getUID();
}

//Создает объект с текстурой
TexturedObject::TexturedObject(SpriteInfo::CHandle pres)
	:DrawableObject(vec2(pres->hw*0.5, pres->hh*0.5),pres->name),
     texture(pres->texture){

	generateIndex();
	generateVertex(pres->frame->topLeftPnt,pres->frame->bottomRightPnt);
    sv.samplerValues["s_texture"]=pres->texture->getUID();
}

void TexturedObject::generateVertex(const glm::vec2& topLeftPnt, const glm::vec2& bottomRightPnt){
    float hw = getHW();
    float hh = getHH();
	GLfloat verts[] = {
		-hw, -hh, 0, topLeftPnt.x,bottomRightPnt.y,
		-hw,  hh, 0, topLeftPnt.x, topLeftPnt.y,
		 hw, -hh, 0, bottomRightPnt.x, bottomRightPnt.y,
		 hw,  hh, 0, bottomRightPnt.x, topLeftPnt.y
	};

	sv.fillBuffer(verts, 5*4);
}

void TexturedObject::generateIndex(){
	GLubyte inds[] = {
		0,1,2,1,3,2
	};

	sv.fillIndex(inds, 6);
}

TexturedObject::TexturedObject(float w, float h, TextureCHandle texture, const std::string& name)
	:DrawableObject(vec2(w*0.5,h*0.5),name),
	 texture(texture){
	generateIndex();
    sv.samplerValues["s_texture"]=texture->getUID();
}

TextureCHandle TexturedObject::getTexture() const {
    return texture;
}

void TexturedObject::draw(const glm::mat4& transform, const Color& ambient) const{
  // glEnable(GL_DEPTH_TEST);
    Game::getShaderSystem()->get("sprite")
            .draw(sv,unordered_map<string, UniformValue>({
            {"u_ambientLight",UniformValue(ambient)},
            {"u_mvp_matrix",UniformValue(transform)}}));
   // glDisable(GL_DEPTH_TEST);
}

