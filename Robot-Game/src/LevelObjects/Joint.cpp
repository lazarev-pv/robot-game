#include <LevelObjects/Joint.h>
#include <Layer.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Physics/Physics.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

Joint::Joint(int uid, JointType *slot, Layer* layer)
    :LevelObject(uid,layer,4,slot->name){
    b2DistanceJointDef jd;
    jd.collideConnected = true;
    jd.bodyA = layer->findPhysBody(slot->obj1Name);
    jd.bodyB = layer->findPhysBody(slot->obj2Name);
    jd.localAnchorA = b2Vec2(slot->offset1->x,slot->offset1->y);
    jd.localAnchorB = b2Vec2(slot->offset2->x,slot->offset2->y);
    b2Vec2 p1 = jd.bodyA->GetWorldPoint(jd.localAnchorA);
    b2Vec2 p2 = jd.bodyB->GetWorldPoint(jd.localAnchorB);
    jd.length = (p2 - p1).Length();

    jd.frequencyHz = slot->frequence;
    jd.dampingRatio = slot->damp;
    jd.userData = this;
    joint = Game::getPhysicsSystem()->getWorld()->CreateJoint(&jd);
}

void Joint::draw(CameraCHandle camera, const Color &ambientLight){
    if(isDrawDebug()) {
        b2Vec2 b1 = joint->GetAnchorA();
        b2Vec2 b2 = joint->GetAnchorB();
        Game::getPrimitiveDrawer()->drawLine(vec3(b1.x, b1.y, 0), vec3(b2.x, b2.y, 0), Color::Green, camera);
    }
}

void Joint::requestDelete(bool instant) {
    assert(getInteract()==nullptr);
    Game::getPhysicsSystem()->deleteObject(joint);
    Game::getPhysicsSystem()->addPostStepAction([this](){
        delete this;
    });
}