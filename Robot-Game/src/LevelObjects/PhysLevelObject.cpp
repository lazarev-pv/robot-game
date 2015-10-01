#include <LevelObjects/PhysLevelObject.h>
#include <LevelObjects/Joint.h>
#include <Layer.h>
#include <LevelObjects/DecorativeLevelObject.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Physics/Physics.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

PhysLevelObject::PhysLevelObject(int uid, GeometrySlot* slot, Layer* layer)
    :PhysLevelObject(uid,layer,0,slot->name, Game::getPhysicsSystem()->createGeometry(slot)) {
}

//todo плохо что надо учитывать interact
void PhysLevelObject::requestDelete(bool instant){
    for(auto& iter : destroyListeners){
        iter();
    }

    // Очень странно, ну а вдруг
    if(body==nullptr){
        LevelObject::requestDelete(instant);
        return;
    }

    // Родителький requestDelete вызываться не будет, поэтому делаем за него удаление interact-а
    if(getInteract()){
        getInteract()->getLayer()->deleteObject(getInteract());
    }

    for(b2JointEdge* edge = getBody()->GetJointList();edge;edge=edge->next){
        Joint* j = static_cast<Joint*>(edge->joint->GetUserData());
        j->getLayer()->deleteObject(j);
    }

    Game::getPhysicsSystem()->deleteObject(body);
    Game::getPhysicsSystem()->addPostStepAction([this](){
        delete this;
    });
}

PhysLevelObject::PhysLevelObject(int uid, Layer* layer, int zOrder, const std::string& name, PhysicsBody* body)
    :LevelObject(uid,layer,0,name),body(body){
    if(body!=nullptr) {
        body->SetUserData(this);
    }
}

PhysLevelObject::~PhysLevelObject(){
}

void PhysLevelObject::setBody(b2Body* body){
    PhysLevelObject::body = body;
    body->SetUserData(this);
}

glm::vec2 PhysLevelObject::get2dPos(){
	b2Vec2 pos = body->GetPosition();
	return vec2(pos.x,pos.y);
}
