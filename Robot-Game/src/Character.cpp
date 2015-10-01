
#include <Character.h>
#include <LevelObjects/Ground.h>

#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/GameState/GameState.h>

#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include "Generated/RobotConfigParser.h"
#include <ChibiEngine/UserInterface/UserInterface.h>

using namespace std;
using namespace game;
using namespace glm;

const static char* RUN_ANIMATION = "walk";
const static char* STAND_ANIMATION = "idle";
const static char* JUMP_ANIMATION = "jump";

Character::Character(const CharacterSettings &settings, glm::vec2 startPosition, const Category& category, Layer* layer, int uid):
        PhysLevelObject(uid,layer, 1,"",nullptr),
        isJumping(false),
        reflected(false),
        state(STOP),
		settings(settings),
        contactedFixtures(0),
        totalHealth(settings.health){

	skeleton=new Skeleton(startPosition, settings.animData->character, category);
	skeleton->addAnimation(STAND_ANIMATION,true);
	skeleton->setPlaybackMultiplier(settings.animationMultiplier);
	setBody(skeleton->getBody());
	for (b2Body *b : skeleton->getBoneSensors()) {
		b->SetUserData(this);
	}
	for(auto iter = settings.weapons.cbegin();iter!=settings.weapons.cend();++iter){
		addSlot(new Weapon(&(*iter),this));
	}

	updatePhysicsId = Game::getPhysicsSystem()->addAdditionalProcessing(
			bind(&Character::updatePhysics,this));
}

void Character::requestDelete(bool instant){
	Game::getPhysicsSystem()->deleteAdditionalProcessing(updatePhysicsId);
	PhysLevelObject::requestDelete(instant);
}

Character::~Character(){
	Game::getLogger().debug("Delete character ");
	delete skeleton;
	for(auto it : weapons){
		delete it;
	}
}

void Character::setReflected(bool flag){
	reflected = flag;
	skeleton->setReflected(flag);
}

void Character::draw(CameraCHandle camera,const Color& ambientLight){
    if(isEnabled()) {
        skeleton->draw(camera, ambientLight);
    }
	if(isDrawDebug()){
		skeleton->drawBoxes(camera);
	}
}

void Character::BeginContact(b2Fixture* contactedFixture, b2Fixture* other){
	for(Weapon* w : weapons){
		if(w!=nullptr) {
			w->beginContact(contactedFixture, other);
		}
	}

	if(!contactedFixture->IsSensor())
		return;
	if(contactedFixture->GetUserData()==nullptr)
		return;

    string fixName = *((string*)contactedFixture->GetUserData());
    if(fixName=="ground"){
		contactedFixtures++;

		PhysLevelObject* lo = static_cast<PhysLevelObject*>(other->GetBody()->GetUserData());
		if(lo==nullptr)
			return;

        if (isJumping) {
            isJumping = false;
            switch (state) {
                case STOP:
					skeleton->removeAnimation(RUN_ANIMATION);
                    skeleton->addAnimation(STAND_ANIMATION,true);
                    break;
                case RUN_BACK:
                case RUN_FORWARD:
					skeleton->removeAnimation(STAND_ANIMATION);
                    skeleton->addAnimation(RUN_ANIMATION, true);
                    break;
                case DEAD:
                    break;
            }
        }
	}
}

void Character::PostSolve(b2Fixture* fixture, const b2ContactImpulse* impulse){
}

void Character::EndContact(b2Fixture* contactedFixture, b2Fixture* other){
	for(Weapon* w : weapons){
		if(w!=nullptr) {
			w->endContact(contactedFixture, other);
		}
	}

	if(!contactedFixture->IsSensor())
		return;

	if(contactedFixture->GetUserData()==nullptr)
			return;
	string fixName = *((string*)contactedFixture->GetUserData());

     if(fixName=="ground"){
		contactedFixtures--;
		PhysLevelObject* lo = static_cast<PhysLevelObject*>(other->GetBody()->GetUserData());
		if(lo==nullptr)
			return;
		if(dynamic_cast<Ground*>(lo)!=nullptr){
			if(contactedFixtures<=0)
				isJumping = true;
		}
	}
}

void Character::attack(bool turnOn, unsigned slotNum){
	if(state==DEAD)
		return;

	assert(slotNum<weapons.size());
	Weapon* w = weapons[slotNum];
	if(w!=nullptr){
		w->attack(turnOn);
		if(turnOn){
			skeleton->addAnimation("attack",true);
		}else{
			skeleton->removeAnimation("attack");
		}
	}
}

void Character::updatePhysics(){
	switch(state){
	case RUN_BACK:
		getBody()->SetLinearVelocity(b2Vec2(-settings.runVelocity,getBody()->GetLinearVelocity().y));
		break;
	case RUN_FORWARD:
        getBody()->SetLinearVelocity(b2Vec2(settings.runVelocity,getBody()->GetLinearVelocity().y));
		break;
    case STOP:
    case DEAD:
        break;
	}
}

void Character::move(bool forward){
	if(state==DEAD)
		return;

	setReflected(!forward);
	state = forward?RUN_FORWARD:RUN_BACK;
	if(!isJumping){
		skeleton->addAnimation(RUN_ANIMATION,true);
	}
}

void Character::stop(){
	if(state==DEAD)
		return;
	state = STOP;
	if(!isJumping){
		skeleton->removeAnimation(RUN_ANIMATION);
		skeleton->addAnimation(STAND_ANIMATION,true);
	}
}

void Character::jump(){
	if(state==DEAD)
		return;
	if(isJumping)
		return;
	b2Vec2 v = getBody()->GetLinearVelocity();
	v.y = settings.jumpVelocity;
    getBody()->SetLinearVelocity(v);
	skeleton->removeAnimation(state==STOP?STAND_ANIMATION:RUN_ANIMATION);
	skeleton->addAnimation(JUMP_ANIMATION,false);
}


void Character::hitted(float health){
    skeleton->addAnimation("hit",false);
    totalHealth-=health;
}