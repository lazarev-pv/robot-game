#include <Input/BoneTemplate.h>
#include <ChibiEngine/Common/MatrixBuilder.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Render/Color.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>

using namespace game;
using namespace glm;
using namespace std;


BoneTemplate::BoneTemplate()
        :begin(),
         end(),
        obj(Game::getResources()->findSprite("bone")){}

void BoneTemplate::recalc(){
    pos = 0.5f*(begin+end);
    vec2 dir = end-begin;
    len = glm::length(dir);
    rotation = getAngle(dir/len);
}

void BoneTemplate::draw(){
    // Draw ChedBone
    float _scale = 0.5*len/obj->getHH();
    obj->draw(createMatrix(
                    pos,
                    rotation,
                    vec2(_scale,_scale),
                    Game::getCamera()),
              Color::White);
}
