#include <ChibiEngine/Render/Sprites/ArbitrarySprite.h>
#include <ChibiEngine/Resource/FileSystem.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Render/Camera.h>

using namespace game;
using namespace std;
using namespace glm;

ArbitrarySprite::ArbitrarySprite(glm::vec3 vecs[4], const SpriteInfo*  info){
    sv.count=4;
    sv.drawMode= GL_TRIANGLE_STRIP;
    sv.useIndex=false;
    sv.samplerValues["s_texture"]=info->texture->getUID();

    texes[0].x=info->frame->topLeftPnt.x;
    texes[0].y=info->frame->bottomRightPnt.y;

    texes[1].x=info->frame->topLeftPnt.x;
    texes[1].y=info->frame->topLeftPnt.y;

    texes[2].x=info->frame->bottomRightPnt.x;
    texes[2].y=info->frame->bottomRightPnt.y;

    texes[3].x=info->frame->bottomRightPnt.x;
    texes[3].y=info->frame->topLeftPnt.y;

    changeVecs(vecs);
}

void ArbitrarySprite::draw(const glm::mat4& transform,const Color& color) const{
    sv.uniformValues["u_mvp_matrix"]=UniformValue(transform);
    sv.uniformValues["u_ambientLight"]=UniformValue(color);

    Game::getShaderSystem()->get("sprite").draw(sv);
}

void ArbitrarySprite::changeVecs(glm::vec3 vecs[4]){
    vector<GLfloat> verts;
    for(int i =0; i<4;++i){
        verts.push_back(vecs[i].x);
        verts.push_back(vecs[i].y);
        verts.push_back(vecs[i].z);

        verts.push_back(texes[i].x);
        verts.push_back(texes[i].y);
    }
    sv.fillBuffer(verts);
}

