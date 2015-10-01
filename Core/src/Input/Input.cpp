#define __INPUT_HEADER_IMPLEMENTATION_
#define GLM_FORCE_RADIANS

#include <ChibiEngine/Input/Input.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Render/Camera.h>

#include <glm/gtc/matrix_transform.hpp>


#include <ChibiEngine/Platform/Platform.h>

using namespace game;
using namespace glm;
using namespace std;

const static string INPUT_LOGGER = "InputSystem";

glm::vec3 game::convertToWorldCoordinates(glm::vec2 screenCoordinates){
	CameraCHandle camera = Game::getCamera();
	vec4 viewPort(-1,-1,2,2);
	vec3 projCam = glm::project(vec3(0,0,0), camera->getViewMatrix(), camera->getProjectionMatrix(),viewPort);
	vec3 winCoord(screenCoordinates,projCam.z);

	return glm::unProject(winCoord,
			camera->getViewMatrix(),
			camera->getProjectionMatrix(),
			viewPort);
}

void InputSystem::keySpecialPressed(KeyCode keyCode, InputEvent::State state){
	logger.trace(state == InputEvent::PRESSED ? "special %c - %i pressed " : "%c - %i released ", keyCode, keyCode);

	for(auto iter:rawListeners){
		if(iter->onSpecialKeyPressed(keyCode, state))
			break;
	}
}

void InputSystem::keyPressed(KeyCode keyCode, InputEvent::State state){
	logger.trace(state == InputEvent::PRESSED ? "%c - %i pressed " : "%c - %i released ", keyCode, keyCode);

	for(auto iter:rawListeners){
		if(iter->onKeyPressed(keyCode, state))
			break;
	}
	if(isValidCode(keyCode)){
		auto iter = keyMap.find(keyCode);
		if(iter!=keyMap.end()){
			for(auto list : this->inputEvents){
				list->execute(iter->second,state);
			}
		}else{
			logger.trace("Unknown event for key " + keyCode);
		}
	}else{
		logger.trace("Not valid key %i", keyCode);
	}
}


InputSystem::InputSystem(ProfileType* inputProfile):
		logger(Game::getLoggingSystem()->createLogger(INPUT_LOGGER)),
		currentMousePosition(0,0){

	if(inputProfile!=nullptr){
		for(auto iter : inputProfile->key){
			if(!isValidCode(iter->code)){
				logger.error("Code %i must be within [0, %i]", iter->code, MAX_KEY_CODE);
				continue;
			}
			logger.debug("Code %i assigned with '%s' event", iter->code, iter->event.c_str());
			keyMap[iter->code]=iter->event;
		}
	}
}

bool InputSystem::isValidCode(int code){
	return code>0 && code<=MAX_KEY_CODE;
}

void InputSystem::mouseMoveEvent(const glm::vec2 &pnt){
	currentMousePosition = pnt;
    for(auto iter:rawListeners){
    	if(iter->onMouseMove(pnt))
    		break;
    }
}

void InputSystem::clickDown(MouseButton code, const glm::vec2 &pnt){
    for(auto iter:rawListeners){
    	if(iter->onClickDown(code, pnt))
    		break;
    }
}

void InputSystem::clickUp(MouseButton code, const glm::vec2 &pnt){
    for(auto iter:rawListeners){
    	if(iter->onClickUp(code, pnt))
    		break;
    }
}

void InputSystem::addRawListener(RawEventListener::Handle listener){
    rawListeners.push_back(listener);
}

void InputSystem::setMouseCursor(Cursor cursor){
	switch(cursor){
	case Cursor::USUAL:
		setCursorUsual();
		break;
	case Cursor::HAND:
		setCursorHand();
		break;
	case Cursor::TEXT:
		setCursorText();
		break;
	}
}

void InputSystem::addEventListener(InputEvent::Handle event){
	inputEvents.push_back(event);
}
