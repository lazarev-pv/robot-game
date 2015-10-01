#include <ChibiEngine/Render/3D/Block.h>


#include <ChibiEngine/Resource/Texture.h>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace game;
using namespace std;


void Block::generateIndex(){
    vector<GLubyte> inds = {
            //front
            0,1,2,
            1,3,2,
            //top
            4,5,6,
            5,7,6,
            //bot
            8,9,10,
            9,11,10,
            //right
            12,14,13,
            13,14,15,
            //left
            16,17,18,
            17, 19, 18
    };

    sv.fillIndex(inds);
    sv.count = inds.size();
}

void Block::generateVerts(const glm::vec2& tex){
    vector<GLfloat> verts({
            //front
            -1.0f, -1.0f, 1.0f, 0, 0,  0, 0, 1,
            -1.0f,  1.0f, 1.0f, 0, tex.y,  0, 0, 1,
            1.0f, -1.0f, 1.0f, tex.x, 0,  0, 0, 1,
            1.0f,  1.0f, 1.0f, tex.x, tex.y,  0, 0, 1,

            //top
            -1.0f,  1.0f,  1.0f, 0, tex.y, 0,1,0,
            -1.0f,  1.0f, -1.0f, 0, tex.y, 0,1,0,
            1.0f,  1.0f,  1.0f, tex.x, 0, 0,1,0,
            1.0f,  1.0f, -1.0f, tex.x, tex.y, 0,1,0,

            //bot
            -1.0f,  -1.0f, -1.0f, 0, 0, 0,-1,0,
            -1.0f,  -1.0f,  1.0f, 0, tex.y, 0,-1,0,
            1.0f,  -1.0f, -1.0f, tex.x, 0, 0,-1,0,
            1.0f,  -1.0f,  1.0f, tex.x, tex.y, 0,-1,0,

            //right
            1.0f, -1.0f, -1.0f, 0, 0,  1, 0, 0,
            1.0f,  1.0f, -1.0f, tex.x, 0,  1, 0, 0,
            1.0f, -1.0f,  1.0f, 0, tex.y,  1, 0, 0,
            1.0f,  1.0f,  1.0f, tex.x, tex.y,  1, 0, 0,

            //left
            -1.0f, -1.0f, -1.0f, 0, 0,  -1, 0, 0,
            -1.0f,  1.0f, -1.0f, tex.x, 0,  -1, 0, 0,
            -1.0f, -1.0f,  1.0f, 0, tex.y,  -1, 0, 0,
            -1.0f,  1.0f,  1.0f, tex.x, tex.y,  -1, 0, 0
    });

    sv.fillBuffer(verts);
}

Block::Block(
        GLuint textureId,
        const glm::vec2& tex)
        :tex(tex){
    sv.samplerValues["s_texture"]=textureId;

    generateIndex();
    generateVerts(tex);
}

void Block::draw(const glm::mat4 &mvp, const Color &ambient) const{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    Game::getShaderSystem()->get("3d").
            draw(sv,unordered_map<string, UniformValue>({
            {"u_ambientLight",UniformValue(ambient)},
            {"u_mvp_matrix",UniformValue(mvp)}}));
    glDisable(GL_CULL_FACE);
    if(!Game::getScreen()->isEnabledDepthTest()){
        glDisable(GL_DEPTH_TEST);
    }
}

void Block::draw(const glm::vec3 pos, const glm::vec3& size, CameraCHandle cam, const Color &ambient) const{
    glm::mat4 result = cam->getMVPMatrix()
            * glm::translate(pos)* glm::scale(size);
    draw(result, ambient);
}