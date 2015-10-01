#include <AICharacter.h>
#include <ChibiEngine/Physics/Physics.h>
#include <Character.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Script/Script.h>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include <RobotLevel.h>
#include <Layer.h>


using namespace std;
using namespace game;
using namespace glm;

AICharacter::~AICharacter(){}

AICharacter::AICharacter(const CharacterSettings &settings, glm::vec2 startPosition, Layer* layer, int uid)
    :Character(settings,
        startPosition,
        CategoryReference::get(CategoryReference::ENEMY),
        layer,uid),
     eye(this, vec2(3.0,0.75), Color(0,1,0,0.3f),"_onSeeBegin","_onSeeEnd"),
     contact(this, vec2(0.2,0.75), Color(1,0,0,0.3f),"_onContactBegin","_onContactEnd"){

}


void AICharacter::BeginContact(b2Fixture *contactedFixture, b2Fixture *other){
    Character::BeginContact(contactedFixture, other);
    eye.beginContact(contactedFixture, other);
    contact.beginContact(contactedFixture, other);
}

void AICharacter::EndContact(b2Fixture *contactedFixture, b2Fixture *other){
    Character::EndContact(contactedFixture, other);
    eye.endContact(contactedFixture, other);
    contact.endContact(contactedFixture, other);
}

void AICharacter::hitted(float health){
    if(markedDeleted)
        return;

    Character::hitted(health);
    Game::getUserInterface()->findElement("enemy_bar")->
            setValue(to_string(getHealth()));
    Game::getScriptSystem()->doScript("_onHitted",{
            arg(getUid()),
            arg(RobotLevel::instance()->getCharacter()->getUid())});

    if(getHealth()<=0.0){
        getLayer()->deleteObject(this);
    }
}

void AICharacter::draw(CameraCHandle camera,const Color& ambientLight){
    Character::draw(camera,ambientLight);
    if(isDrawDebug()){
        eye.draw(camera);
        contact.draw(camera);
    }
}


AICharacter::Sensor::Sensor(AICharacter* parent,
                        const glm::vec2& area,
                        const Color& color,
                        const std::string& beginScript,
                        const std::string& endScript)

    :area(area),
     color(color),
     contactedFixtures(0),
     beginScript(beginScript),
     endScript(endScript),
     parent(parent){

    b2FixtureDef fd;
    fd.isSensor = true;
    b2PolygonShape shape;
    shape.SetAsBox(area.x, area.y);
    fd.shape = &shape;
    Category cat = CategoryReference::get(CategoryReference::ENEMY_WEAPON_SENSOR);
    fd.filter.categoryBits = cat.category;
    fd.filter.maskBits = cat.mask;
    fix = parent->getBody()->CreateFixture(&fd);
}

void AICharacter::Sensor::callAiScript(const std::string& name,b2Fixture *contactedFixture, b2Fixture *other){
    int firstUid = static_cast<Character*>(contactedFixture->GetBody()->GetUserData())->getUid();
    int secondUid = static_cast<Character*>(other->GetBody()->GetUserData())->getUid();

    if(firstUid==parent->getUid()){
        Game::getScriptSystem()->doScript(name,{arg(firstUid),arg(secondUid)});
    }else{
        Game::getScriptSystem()->doScript(name,{arg(secondUid),arg(firstUid)});
    }
}



void AICharacter::Sensor::beginContact(b2Fixture *contactedFixture, b2Fixture *other){
    if(fix==contactedFixture || fix == other){
        contactedFixtures++;
    }
    if(contactedFixtures==1 && !parent->markedDeleted){
        callAiScript(beginScript,contactedFixture,other);
    }
}

void AICharacter::Sensor::endContact(b2Fixture *contactedFixture, b2Fixture *other){
    if(fix==contactedFixture || fix == other){
        contactedFixtures--;
    }
    if(contactedFixtures==0 && !parent->markedDeleted){
        callAiScript(endScript,contactedFixture,other);
    }
}

void AICharacter::Sensor::draw(CameraCHandle camera){
    Game::getPrimitiveDrawer()->drawFilledRectangle(
            createMatrix(vec3(parent->get2dPos(), 0),area, camera),
            color);
}

void AICharacter::requestDelete(bool instant) {
    markedDeleted = true;
    Character::requestDelete(instant);
}
