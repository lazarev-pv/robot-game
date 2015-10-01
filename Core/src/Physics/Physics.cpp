
#include <ChibiEngine/Physics/Physics.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Log/LoggingSystem.h>


using namespace std;
using namespace game;

const static string LOGGER_NAME = "PhysicsSystem";

void PhysicsSystem::step(){
    if(world!=nullptr){
        for(auto iter : additionalProcessing)
            iter.second();

        float delta = Game::getLevelClock()->getTimeDelta()*0.001f*physicsStepDimension;
        world->Step(delta,velocityIters,positionIters);
        for(b2Joint* j : jointDeleteList){
            world->DestroyJoint(j);
        }
        for(b2Body* j : bodyDeleteList){
            world->DestroyBody(j);
        }
        jointDeleteList.clear();
        bodyDeleteList.clear();

        for(auto action : postStepActions){
            action();
        }
        postStepActions.clear();
    }
}

int PhysicsSystem::addAdditionalProcessing(const function<void()>& action){
    lastId++;
    additionalProcessing[lastId]=action;
    return lastId;
}

void PhysicsSystem::deleteAdditionalProcessing(int id){
    additionalProcessing.erase(id);
}

void PhysicsSystem::setContactListener(b2ContactListener* listener){
	this->world->SetContactListener(listener);
}

PhysicsSystem::PhysicsSystem(PhysicsType* settings, ClockHandle , EventSystem* eventSystem)
    :logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)){
    lastId = -1;

    if(settings){
        positionIters = settings->positionIterations;
        velocityIters = settings->velocityIterations;
        eventInterval = settings->calcTimeInterval;
        physicsStepDimension = settings->physicsStepDimension;

        world = new PhysicsWorld(b2Vec2(settings->gravity->x,settings->gravity->y));
        eventSystem->doEveryFrame(bind(&PhysicsSystem::step, this));
    }else{
        positionIters = 0;
        velocityIters = 0;
        eventInterval = 1000;
        physicsStepDimension = 10;

        world = new PhysicsWorld(b2Vec2(0,0));
    }
    //1/60*1000
    //eventSystem->scheduleInfiteRepeat(
      //      gameClock,updateAction,0,settings->calcTimeInterval);
}

PhysicsSystem::~PhysicsSystem(){
    if(world != nullptr)
        delete world;
}

PhysicsBody* PhysicsSystem::createCircularGeometry(float radius, float density, float friction){
   b2BodyDef bd;
   bd.type = b2_dynamicBody;
   bd.active = false;
   b2Body* geometry = world->CreateBody(&bd);
   b2CircleShape shape;
   shape.m_radius = radius;
   b2FixtureDef fd;
   fd.shape = &shape;
   fd.density = density;
   fd.friction = friction;
   geometry->CreateFixture(&fd);
   return geometry;
}

inline b2BodyType convertType(int type){
    switch(type){
        case 0:
            return b2_staticBody;
        case 1:
            return b2_kinematicBody;
        default:
            return b2_dynamicBody;
    }
}

PhysicsBody* PhysicsSystem::createBoxGeometry(GeometrySlot *bgt){
    b2BodyDef bd;
	b2PolygonShape shape;
	b2FixtureDef fd;

	bd.position.Set(bgt->x, bgt->y);

    bd.type = convertType(bgt->physType);
	bd.active = true;
	b2Body* geometry = world->CreateBody(&bd);

    for(ERBoxType* iter : bgt->box){
        shape.SetAsBox(iter->hw,iter->hh,b2Vec2(iter->x,iter->y),0.0);
        fd.shape = &shape;
        fd.density = iter->density;
        fd.friction = iter->friction;
        Category cat = CategoryReference::get(iter->category);
        fd.filter.categoryBits = cat.category;
        fd.filter.maskBits = cat.mask;
        fd.userData = iter;
        fd.isSensor = iter->sensor;
        geometry->CreateFixture(&fd);
    }

	return geometry;
}


PhysicsBody* PhysicsSystem::createChainGeometry(GeometrySlot* bgt){
	b2BodyDef bd;
	bd.position.Set(bgt->x,bgt->y);
	bd.type = b2_staticBody;
	bd.active = true;
	b2Body* geometry = world->CreateBody(&bd);

	vector<b2Vec2> vecs;
	for(auto pnt : bgt->chain->point){
		vecs.push_back(b2Vec2(pnt->x,pnt->y));
	}

	b2ChainShape chain;
	chain.CreateChain(&(vecs[0]),vecs.size());

	b2FixtureDef fd;
	fd.shape = &chain;
    {
        Category cat = CategoryReference::get(bgt->chain->category);
        fd.filter.categoryBits = cat.category;
        fd.filter.maskBits = cat.mask;
    }
	fd.friction = bgt->chain->friction;
	geometry->CreateFixture(&fd);
	return geometry;
}

PhysicsBody* PhysicsSystem::createGeometry(GeometrySlot *bgt){
	if(bgt->box.size()>0){
		return createBoxGeometry(bgt);
	}else if(bgt->chain!=nullptr){
		return createChainGeometry(bgt);
	}else{
		return nullptr;
	}
}

PhysicsBody* PhysicsSystem::create(PhysicsBodyDef *def){
    return world->CreateBody(def);
}

WheelJoint* PhysicsSystem::createWheelJoint(JointDef *def){
    return (b2WheelJoint*)world->CreateJoint(def);
}
