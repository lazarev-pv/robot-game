#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonDynamicGeometry.h>
#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace std;
using namespace glm;
using namespace game;

SkeletonDynamicGeometry::SkeletonDynamicGeometry(b2World *world, CharBoxType *data, SkeletonJoint* parent, const Category &category)
        : parent(parent),
          ownPos(vec3(data->pox_x, data->pox_y, 0)),
          ownRotation(data->rotation),
          color(1, 1, 0, 1),
          name(data->name),
          hw(data->hw),
          hh(data->hh) {
    {
        b2BodyDef bd;
        bd.position.SetZero();
        bd.type = b2_dynamicBody;
        bd.active = true;
        part = world->CreateBody(&bd);
        //part->SetGravityScale(0.0f);
        b2PolygonShape shape;
        shape.SetAsBox(data->hw, data->hh);
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.userData = nullptr;
        fd.density = data->density;
        fd.friction = data->friction;
        fd.isSensor = true;
        fd.filter.categoryBits = category.category;
        fd.filter.maskBits = category.mask;
        fd.userData = &name;
        part->CreateFixture(&fd);
    }
}

SkeletonDynamicGeometry::SkeletonDynamicGeometry(SkeletonDynamicGeometry &&other) :
        parent(move(other.parent)),
        ownPos(move(other.ownPos)),
        ownRotation(other.ownRotation),
        color(move(other.color)),
        name(move(other.name)),
        hw(other.hw),
        hh(other.hh),
        part(other.part) {
    other.part = nullptr;
}

SkeletonDynamicGeometry &SkeletonDynamicGeometry::operator=(SkeletonDynamicGeometry &&other) {
    parent = other.parent;
    ownPos = move(other.ownPos);
    ownRotation = other.ownRotation;
    color = move(other.color);
    name = move(other.name);
    hw = other.hw;
    hh = other.hh;
    part = other.part;
    other.part = nullptr;
    return *this;
}

SkeletonDynamicGeometry::~SkeletonDynamicGeometry() {
    if (part)
        Game::getPhysicsSystem()->deleteObject(part);
}

void SkeletonDynamicGeometry::draw(CameraCHandle camera) const{
    const b2Vec2& p = part->GetPosition();
    Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(vec3(p.x,p.y,0), part->GetAngle(), vec2(hw,hh),camera),
            color);
}

void SkeletonDynamicGeometry::applyTransform(bool reflected, Skeleton* skeleton){

    mat4 anM = parent->getAnimationMatrix();
    vec4 begin = glm::scale(vec3(reflected?-1.0f:1.0f,1.0f,1.0f))
            * anM
            * glm::rotate(ownRotation,glm::vec3(0.0f,0.0f,1.0f))
            * glm::translate(ownPos)
            * vec4(0,-hh,0,1);

    vec4 end = glm::scale(vec3(reflected?-1.0f:1.0f,1.0f,1.0f))
            * anM
            * glm::rotate(ownRotation,glm::vec3(0.0f,0.0f,1.0f))
            * glm::translate(ownPos)
            * vec4(0,hh,0,1);

    float rotation;
    if(begin.x<end.x){
        rotation =-glm::acos((end.y-begin.y)/(2*hh));
    }else{
        rotation = glm::acos((end.y-begin.y)/(2*hh));
    }

    const vec4 p = (begin+end)*0.5f;
    b2Vec2 resPnt(p.x/p.w+skeleton->getBody()->GetPosition().x,p.y/p.w+skeleton->getBody()->GetPosition().y);
    part->SetTransform(resPnt,rotation);
}

b2Body* SkeletonDynamicGeometry::getBody(){
    return part;
}
