#include <Character.h>
#include <RobotLevel.h>
#include <ChibiEngine/GameState/GameState.h>

using namespace std;
using namespace game;
using namespace glm;

//todo take out 2 settings
const char* FORWARD_EVENT_NAME = "forward";
const char* BACWARD_EVENT_NAME = "backward";
const char* JUMP_EVENT_NAME = "jump";
const char* ATTACK_EVENT_NAME = "attack";

ProtagonistMove::ProtagonistMove(RobotLevel* parent):
	parent(parent){
}

void ProtagonistMove::execute(const std::string& eventName, InputEvent::State state){
	if(Game::getLevelClock()->isPaused())
		return;

    if(isBlocked)
        return;

	Character* protagonist = parent->getCharacter();

	if(state == PRESSED){
		if(eventName==FORWARD_EVENT_NAME){
			protagonist->move(true);
		}else if(eventName==BACWARD_EVENT_NAME){
			protagonist->move(false);
		}else if(eventName == JUMP_EVENT_NAME){
			protagonist->jump();
		}else if(eventName == ATTACK_EVENT_NAME){
			protagonist->attack(true,0);
		}else if(eventName == "debug"){
			parent->toggleDebug();
		}else if(eventName == "pres"){
            parent->togglePres();
        }else if(eventName=="interact"){
            parent->interact();
        }

	}else if(state==RELEASED) {
		if(eventName==FORWARD_EVENT_NAME || eventName==BACWARD_EVENT_NAME){
			protagonist->stop();
		}else if(eventName==ATTACK_EVENT_NAME){
			protagonist->attack(false,0);
		}
	}
}

void ProtagonistMove::setBlockInput(bool isBlocked){
    this->isBlocked = isBlocked;
}

ProtagonistMove::~ProtagonistMove(){
}
