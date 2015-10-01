#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/SkeletonAnimation/SkeletonStaticGeometry.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>

using namespace std;
using namespace glm;
using namespace game;

SkeletonStaticGeometry::SkeletonStaticGeometry(CharBoxType *d) :
        pos(d->pox_x, d->pox_y, 0),
        hw(d->hw),
        hh(d->hh),
        color(d->sensor ? vec4(1, 0, 0, 1) : vec4(0, 1, 0, 1)),
        name(d->name) {
}

void SkeletonStaticGeometry::draw(const glm::vec3& pos, CameraCHandle camera) const{
    Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(pos+this->pos,vec2(hw,hh),camera),
            color);
}

std::string* SkeletonStaticGeometry::getName(){
    return new string(name);
}