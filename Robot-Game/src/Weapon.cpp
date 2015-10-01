#include <Weapon.h>
#include <CharacterReference.h>
#include "Generated/RobotConfigParser.h"
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <Character.h>

using namespace std;
using namespace game;

Weapon::Weapon(const WeaponSettings* data,Character* parent):
    attacking(false),
    data(data),
    sensor(nullptr) {

    if (data->collideSensor.size() > 0) {
        for (b2Body *b : parent->getSkeleton()->getBoneSensors()) {
            b2Fixture *fix = b->GetFixtureList();
            string name = *(static_cast<string *>(fix->GetUserData()));
            if (name == data->collideSensor) {
                sensor = b;
            }
        }
        //sword->SetAwake(false);
    }
}

bool Weapon::getFixtures(b2Fixture *contactedFixture, b2Fixture *other, b2Fixture** self, b2Fixture** target){
    if(!contactedFixture->IsSensor() && !other->IsSensor())
        return false;

    if(contactedFixture->GetUserData()==nullptr || other->GetUserData()==nullptr)
        return false;

    string fixName = contactedFixture->GetUserData()!=nullptr?
            *((string*)contactedFixture->GetUserData()):"";

    if(fixName==data->collideSensor){
        *self=contactedFixture;
        *target=other;
        return true;
    }

    return false;
/*
    string otherName = other->GetUserData()!=nullptr?
            *((string*)other->GetUserData()):"";

    if(otherName==data->collideSensor){
        *self=other;
        *target=contactedFixture;
        return true;
    }

    return false;*/
}

void Weapon::beginContact(b2Fixture *contactedFixture, b2Fixture *other){
    b2Fixture *self, *target;
    if(!getFixtures(contactedFixture,other,&self,&target))
        return;

    PhysLevelObject *lo = static_cast<PhysLevelObject *>(target->GetBody()->GetUserData());
    if(attacking){
        lo->hitted(data->damage);
    }
}

void Weapon::endContact(b2Fixture *contactedFixture, b2Fixture *other){
    b2Fixture *self, *target;
    if(!getFixtures(contactedFixture,other,&self,&target))
        return;
}

void Weapon::attack(bool turnOn){
    attacking = turnOn;
}