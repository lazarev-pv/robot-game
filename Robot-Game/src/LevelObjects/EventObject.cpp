#include <LevelObjects/EventObject.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <Box2D/Box2D.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <Character.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

//todo уюрать нах object type
EventObject::EventObject(int uid, EventType* data, Layer* layer)
:PhysLevelObject(uid,layer,0,data->name,nullptr),
 infinite(data->infinite),
 onEnter(data->onEnter),
 onLeave(data->onLeave),
 ch(nullptr),
 hw(data->size->x),
 hh(data->size->y),
 fixturesCount(0){

    b2BodyDef bd;
    bd.position.Set(data->pos->x,data->pos->y);
    bd.type=b2_staticBody;
    b2Body* geometry = Game::getPhysicsSystem()->getWorld()->CreateBody(&bd);
    b2PolygonShape shape;
    shape.SetAsBox(data->size->x,data->size->y);
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.isSensor = true;
    geometry->CreateFixture(&fd);
    setBody(geometry);
}

void EventObject::BeginContact(b2Fixture* contactedFixture, b2Fixture* other){
    fixturesCount++;
    if(fixturesCount>1)
        return;

    ch = dynamic_cast<Character*>(static_cast<PhysLevelObject*>(other->GetBody()->GetUserData()));
    if(isEnabled()){
        Game::getScriptSystem()->doScript(onEnter);
    }
}

void EventObject::EndContact(b2Fixture* contactedFixture, b2Fixture* other) {
    fixturesCount--;
    if(fixturesCount>0){
        return;
    }

    if(isEnabled()) {
        Game::getScriptSystem()->doScript(onLeave);
    }
    ch=nullptr;
    PhysLevelObject::setEnabled(infinite);
}

void EventObject::setEnabled(bool enabled){
    PhysLevelObject::setEnabled(enabled);
    if(ch!=nullptr){
        Game::getScriptSystem()->doScript(enabled?onEnter:onLeave);
    }
}

void EventObject::draw(CameraCHandle camera, const Color &ambientLight){
    if(isDrawDebug() && isEnabled()){
        Game::getPrimitiveDrawer()->drawFilledRectangle(
                createMatrix(vec3(get2dPos(), 0),vec2(hw, hh),camera),
                Color(Color::Red).setAlpha(0.5));
    }
}