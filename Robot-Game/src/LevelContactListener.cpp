#include <ChibiEngine/GameState/Level.h>
#include <RobotLevel.h>

using namespace std;
using namespace game;
using namespace glm;

#define _TY_PE_ PhysLevelObject

//Contactable
//PhysLevelObject
void LevelContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
	b2Body* first = contact->GetFixtureA()->GetBody();
	b2Body* second = contact->GetFixtureB()->GetBody();
	if(first == second)
		return; //alarm!!!

	if(first->GetUserData()!=nullptr){
		_TY_PE_ * lo = static_cast<_TY_PE_ *>(first->GetUserData());
		lo->PostSolve(contact->GetFixtureA(),impulse);
	}

	if(second->GetUserData()!=nullptr){
		_TY_PE_ * lo = static_cast<_TY_PE_ *>(second->GetUserData());
		lo->PostSolve(contact->GetFixtureB(),impulse);
	}


}

void LevelContactListener::BeginContact(b2Contact* contact){
	b2Body* first = contact->GetFixtureA()->GetBody();
	b2Body* second = contact->GetFixtureB()->GetBody();
	if(first == second)
		return; //alarm!!!

	if(first->GetUserData()!=nullptr){
		_TY_PE_ * lo = static_cast<_TY_PE_ *>(first->GetUserData());
		if(lo!=nullptr)
			lo->BeginContact(contact->GetFixtureA(),contact->GetFixtureB());
	}

	if(second->GetUserData()!=nullptr){
		_TY_PE_ * lo = static_cast<_TY_PE_ *>(second->GetUserData());
		if(lo!=nullptr)
			lo->BeginContact(contact->GetFixtureB(),contact->GetFixtureA());
	}
}

void LevelContactListener::EndContact(b2Contact* contact){
	b2Body* first = contact->GetFixtureA()->GetBody();
	b2Body* second = contact->GetFixtureB()->GetBody();
	if(first == second)
		return; //alarm!!!

	if(first->GetUserData()!=nullptr){
		_TY_PE_ * lo = static_cast<_TY_PE_ *>(first->GetUserData());
		lo->EndContact(contact->GetFixtureA(),contact->GetFixtureB());
	}

	if(second->GetUserData()!=nullptr){
		_TY_PE_ * lo = static_cast<_TY_PE_ *>(second->GetUserData());
		lo->EndContact(contact->GetFixtureB(),contact->GetFixtureA());
	}
}

#undef _TY_PE_
