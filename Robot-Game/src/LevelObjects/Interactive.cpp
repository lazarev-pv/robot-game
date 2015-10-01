#include <LevelObjects/Interactive.h>
#include <ChibiEngine/Physics/CategoryReference.h>
#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Script/Script.h>
#include <RobotLevel.h>

using namespace game;
using namespace std;
using namespace glm;

Interactive::Interactive(int uid, LevelObject *parent, const glm::vec2& size, const std::string& script)
    :PhysLevelObject(uid,parent->getLayer(),0,"",nullptr),active(true),parent(parent),size(size),script(script){
    {
        b2BodyDef bd;
        vec3 v = parent->getPosition();
        bd.position.x=v.x;
        bd.position.y=v.y;
        bd.type = b2_staticBody;
        b2PolygonShape shape;
        shape.SetAsBox(size.x, size.y);
        b2FixtureDef fd;
        fd.shape = &shape;
        {
            Category cat = CategoryReference::get(CategoryReference::INTERACTIVE);
            fd.filter.categoryBits = cat.category;
            fd.filter.maskBits = cat.mask;
        }
        fd.isSensor = true;
        b2Body* geometry = Game::getPhysicsSystem()->getWorld()->CreateBody(&bd);
        geometry->CreateFixture(&fd);
        setBody(geometry);
    }
    procId = Game::getPhysicsSystem()->addAdditionalProcessing(bind(&Interactive::updatePhysics,this));
    parent->link2Interact(this);
}

void Interactive::draw(CameraCHandle camera, const Color &ambientLight) {
    if(isDrawDebug()){
        Game::getPrimitiveDrawer()->drawFilledRectangle(
                createMatrix(vec3(getBody()->GetPosition().x, getBody()->GetPosition().y, 0),size,camera),
                Color(Color::Red).setAlpha(0.5));
    }
}

void Interactive::updatePhysics(){
    vec3 v = parent->getPosition();
    getBody()->SetTransform(b2Vec2(v.x,v.y), 0.0f);
}

void Interactive::BeginContact(b2Fixture *contactedFixture, b2Fixture *other) {
    RobotLevel::instance()->viewInteractive(this);
    active = true;
}

void Interactive::EndContact(b2Fixture *contactedFixture, b2Fixture *other) {
    RobotLevel::instance()->viewInteractive(nullptr);
    active = false;
}

void Interactive::interact(){
    Game::getScriptSystem()->doScript(script);
}

void Interactive::requestDelete(bool instant){
    if(active)
        RobotLevel::instance()->viewInteractive(nullptr);
    Game::getPhysicsSystem()->deleteAdditionalProcessing(procId);
    parent->link2Interact(nullptr);
    PhysLevelObject::requestDelete(instant);
}

