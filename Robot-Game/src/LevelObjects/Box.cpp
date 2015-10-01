#include <LevelObjects/Box.h>
#include <LevelObjects/DecorativeLevelObject.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Log/Logger.h>
#include <RobotLevel.h>
#include <Layer.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;

static const Color HITTED_DMG(1.0f,0.1f,0.1f,1.0f);

ObjectFixture::ObjectFixture(Box* parent, b2Fixture* fixture)
    :box(parent),fixture(fixture){

    assert(box!=nullptr);
    ERBoxType* d = static_cast<ERBoxType*>(fixture->GetUserData());
    offset=vec2(d->x,d->y);
    hw = d->hw;
    hh = d->hh;

    for(auto it:d->art){
        presentations.push_back(new DecorativeLevelObject(-1,it,0.0f,this,parent->getLayer()));
    }
}

ObjectFixture::ObjectFixture(Box* parent,ObjectFixture* other, b2Fixture* fixture):
        box(parent),
    fixture(fixture),
    offset(other->offset),
    hw(other->hw),
    hh(other->hh),
    presentations(other->presentations){

    assert(box!=nullptr);
    other->presentations.clear();

    for(auto it:presentations){
        it->setParent(this);
    }
}

ObjectFixture::ObjectFixture(ObjectFixture&& other):
    box(other.box),fixture(other.fixture),
    offset(move(other.offset)),hw(other.hw),hh(other.hh),
    presentations(move(other.presentations)){
    assert(box!=nullptr);
}

float ObjectFixture::getAngle() const{
    assert(box!=nullptr);
    return box->getBody()->GetAngle();
}

void ObjectFixture::setColor(const Color& color){
    for(auto i : presentations){
        i->setColor(color);
    }
}

ObjectFixture::~ObjectFixture(){
    for(auto iter:presentations){
        iter->requestDelete(true);// doesn't matter true or false - maybe remove?
    }
}

void ObjectFixture::draw(CameraCHandle camera, bool drawDebug, const Color& damageColor, const Color &ambientLight) const{
    for (auto iter : presentations){
         iter->draw(camera, ambientLight*damageColor);
    }
    if(drawDebug) {
        Game::getPrimitiveDrawer()->drawRectangleBorder(MatrixBuilder()
                .pos(getFullOffset())
                .scale(vec2(hw, hh))
                .camera(camera)
                .rotate(getAngle(),-offset)
                .toMatrix(),
                Color::Green);
    }
}

Box::Box(int uid, GeometrySlot* slot, Layer* layer)
    :PhysLevelObject(uid,slot,layer),
    health(20),
    damageColor(Color::White){

    for(b2Fixture* f=getBody()->GetFixtureList(); f; f=f->GetNext()){
        fixtures.push_back(new ObjectFixture(this,f));
    }

}


Box::Box(int uid, ObjectFixture* fixture, b2BodyDef* bd, b2FixtureDef* fd, Layer* layer)
    :PhysLevelObject(uid,layer, 0, "", nullptr),
     health(20),
     damageColor(Color::White){

    b2Body* b = Game::getPhysicsSystem()->getWorld()->CreateBody(bd);
    b2Fixture* f = b->CreateFixture(fd);
    setBody(b);
    fixtures.push_back(new ObjectFixture(this,fixture,f));
}

void Box::draw(CameraCHandle camera,const Color& ambientLight){
    if(isEnabled()) {
        for (auto it = fixtures.begin(); it != fixtures.end(); ++it) {
            (*it)->draw(camera, isDrawDebug(),damageColor, ambientLight);
        }
    }
}

Box::~Box(){
    Game::getLogger().debug("Delete box event %p ", colorEvent);
    Game::getEventSystem()->deleteEvent(Game::getLevelClock(), colorEvent);
    for (auto it : fixtures) {
        delete it;
    }
}

void Box::hitted(float f){
    health-=f;
    damageColor = HITTED_DMG;
    colorEvent = Game::getEventSystem()->schedule(Game::getLevelClock(), bind(&Box::restoreColor, this),
            Game::getLevelClock()->getTime() + 500, 0, 1);
    if(health<=0.0){
        getLayer()->deleteObject(this);
    }
}

void Box::restoreColor() {
    damageColor = Color::White;
    colorEvent = NullEventHandle;
}

void Box::breakBody(){
    Game::getPhysicsSystem()->addPostStepAction(bind(&Box::breakBodyImpl,this));
}

b2PolygonShape ObjectFixture::copyShape() const{
    b2PolygonShape shape;
    shape.SetAsBox(hw,hh,b2Vec2(offset.x,offset.y),0.0);
    return shape;
}

void Box::breakBodyImpl(){
    b2Body* body = getBody();
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = body->GetPosition();
    bd.angle = body->GetAngle();

    Layer* layer = getLayer();
    b2FixtureDef fd;
    RobotLevel* level = RobotLevel::instance();
    for(auto iter : fixtures){
        b2Fixture* fix = iter->getFixture();
        b2PolygonShape shape = iter->copyShape();
        fd.shape = &shape;
        fd.friction = fix->GetFriction();
        fd.density = fix->GetDensity();
        fd.filter = fix->GetFilterData();

        getBody()->DestroyFixture(fix);
        layer->add(new Box(level->generateUid(),iter,&bd,&fd,layer));

    }
    layer->deleteObject(this);
}

void Box::setColor(const Color &color) {
    LevelObject::setColor(color);
    for(auto iter : fixtures){
        iter->setColor(color);
    }
}
