#define GLM_FORCE_RADIANS

#include <EditorInputListener.h>
#include <EditorLevel.h>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ChibiEngine/GameState/GameState.h>
#include <ChibiEngine/Log/LoggingSystem.h>

using namespace game;
using namespace std;
using namespace glm;

const static float CAMERA_MOVE_DELTA = 0.5;
const static float OBJ_MOV_ARROWS_FAST = 1;
const static float OBJ_MOV_ARROWS = 0.2;

const static Clock::TimeDelta KEY_REPEAT_DELAY_START = 200;
const static Clock::TimeDelta KEY_REPEAT_INTERVAL = 100;

const static char* EDITOR_LOGGING="EditorInputListener";

void EditorInputListener::repeatKey(){
	onSpecialKeyPressed(keyCode, InputEvent::PRESSED);
}

void EditorInputListener::scheduleSpecialRepeat(KeyCode keyCode){
    this->keyCode = keyCode;
	if(!keyRepeatEvent){
		ClockHandle clock = Game::getLevelClock();
		keyRepeatEvent = Game::getEventSystem()->scheduleInfiteRepeat(clock, bind(&EditorInputListener::repeatKey, this), clock->getTime() + KEY_REPEAT_DELAY_START, KEY_REPEAT_INTERVAL);
	}
}

void EditorInputListener::deleteSpecialRepeatition(){
	Game::getEventSystem()->deleteEvent(Game::getLevelClock(), keyRepeatEvent);
	keyRepeatEvent = NullEventHandle;
}

bool EditorInputListener::onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state){
	logger.trace(state == InputEvent::PRESSED ? "special %i pressed " : "special %i released ", keyCode);

	switch(keyCode){
	case LCTRL_KEY:
	case RCTRL_KEY:
		scalling = state==InputEvent::PRESSED;
		ctrlPressed = state==InputEvent::PRESSED;
		break;
	case LSHIFT:
	case RSHIFT:
		smoth = state==InputEvent::PRESSED;
		break;
	case UP_ARROW:
	case LEFT_ARROW:
	case RIGHT_ARROW:
	case DOWN_ARROW:
		if(state==InputEvent::PRESSED){
			scheduleSpecialRepeat(keyCode);
			float speed = ctrlPressed?OBJ_MOV_ARROWS_FAST:OBJ_MOV_ARROWS;
			switch(keyCode){
			case UP_ARROW:
				level->moveObject(vec3(0.0,speed,0.0));
				break;
			case LEFT_ARROW:
				level->moveObject(vec3(-speed,0,0));
				break;
			case RIGHT_ARROW:
				level->moveObject(vec3(speed,0,0));
				break;
			case DOWN_ARROW:
				level->moveObject(vec3(0.0, -speed,0.0));
				break;
			}
		}else{
			deleteSpecialRepeatition();
		}
		break;
	}
	return false;
}


EditorInputListener::EditorInputListener(EditorLevel* level)
	:level(level),
	 mouseCurrentButton(MouseButton::NO_BUTTON),
	 logger(Game::getLoggingSystem()->createLogger(EDITOR_LOGGING)),
	 keyRepeatEvent(NullEventHandle),
	 smoth(false),
	 ctrlPressed(false),
	 scalling(false),
	 mouseLastRay(vec3(0,0,0),vec3(0,0,0))
{
	CameraCHandle camera = Game::getCamera();
	vec4 viewPort(-1,-1,2,2);
	nearZ = glm::project(vec3(0,0,0),camera->getViewMatrix(),camera->getProjectionMatrix(),viewPort).z;
	farZ = glm::project(vec3(0,0,100),camera->getViewMatrix(),camera->getProjectionMatrix(),viewPort).z;
    keyCode = 0;
}

bool EditorInputListener::onClickDown(MouseButton code, const glm::vec2 &pnt){
	mouseClickDownCoords = pnt;
	onMouseMove(pnt);
	mouseCurrentButton = code;

	float zFactor = smoth?0: Game::getCamera()->getPosition().z*0.1;
	switch(code){
	case MouseButton::LEFT:
		return level->selectObject(getMouseRay(), ctrlPressed);
	case MouseButton::SCROLL_UP:
		if(!scalling){
			level->moveCamera(vec2(0,CAMERA_MOVE_DELTA+zFactor));
		}else{
			level->zoomCamera(CAMERA_MOVE_DELTA+zFactor);
		}
		return true;
	case MouseButton::SCROLL_DOWN:
		if(!scalling){
			level->moveCamera(vec2(0,-CAMERA_MOVE_DELTA-zFactor));
		}else{
			level->zoomCamera(-CAMERA_MOVE_DELTA-zFactor);
		}

		return true;
	case MouseButton::SCROLL_LEFT:
		level->moveCamera(vec2(-CAMERA_MOVE_DELTA-zFactor,0));
		break;
	case MouseButton::SCROLL_RIGHT:
		level->moveCamera(vec2(CAMERA_MOVE_DELTA+zFactor,0));
		return true;
    case MouseButton::MIDDLE:
    case MouseButton::RIGHT:
    case MouseButton::NO_BUTTON:
        break;
	}
	return false;

}

bool EditorInputListener::onClickUp(MouseButton code, const glm::vec2 &pnt){

	if(mouseCurrentButton == MouseButton::RIGHT){
		onMouseMove(pnt);

		level->hideMenus();
		level->viewContextMenu(pnt);
	}else{ // not right mouse button
		level->hideMenus();
		level->endDraging();
	}
	mouseCurrentButton = MouseButton::NO_BUTTON;
	return false;
}

bool EditorInputListener::onMouseMove(const glm::vec2 &pnt){
	mouseCurrentPos = pnt;
	MouseRay currentRay = getMouseRay();

	if(mouseCurrentButton==MouseButton::LEFT){
		level->onMouseDrag(currentRay,mouseLastRay);
	}

	mouseLastRay = currentRay;

	return false;
}

void EditorInputListener::draw(CameraCHandle camera){
	mouseLastRay.draw(camera);
}

void EditorInputListener::MouseRay::draw(CameraCHandle camera) const{
	static const int W = 5;
	Game::getPrimitiveDrawer()->drawRectangleBorder(
			createMatrix(begin, vec2(W, W),camera), Color::Red);
}

glm::vec3 EditorInputListener::MouseRay::calcPos(float z) const{
	glm::vec3 n = end - begin;
	return n*(z - begin.z)/n.z+begin;
}

glm::vec2 EditorInputListener::getWorldCoordinatesZeroPlane() const{
	vec3 pos = convertToWorldCoordinates(mouseCurrentPos);
	return vec2(pos.x,pos.y);
}

EditorInputListener::MouseRay EditorInputListener::getMouseRay() const{
	CameraCHandle camera = Game::getCamera();
	vec4 viewPort(-1,-1,2,2);

	vec3 nearPoint = glm::unProject(vec3(mouseCurrentPos,nearZ),
			camera->getViewMatrix(),
			camera->getProjectionMatrix(),
			viewPort);

	vec3 farPoint = glm::unProject(vec3(mouseCurrentPos,farZ),
			camera->getViewMatrix(),
			camera->getProjectionMatrix(),
			viewPort);

	return MouseRay(nearPoint, farPoint);
}

bool EditorInputListener::onKeyPressed(KeyCode keyCode, InputEvent::State state){

	switch(keyCode){
	case 127: // delete
		level->deleteObject();
		return true;
	}

	if(!ctrlPressed || state!=InputEvent::PRESSED)
		return false;

	switch(keyCode){
	case CTRL_Z: //ctrl+z
		logger.debug("undo");
		level->undo();
		break;
	case CTRL_Y: //ctrl+y
		logger.debug("redo");
		level->redo();
		break;
	case CTRL_C://ctrl+c
		level->markForCopy();
		break;
	case 22: // ctrl+p
		level->paste();
		break;
	case CTRL_S: // ctrl+s
		level->save();
		break;
	default:
		return false;
	}

	return true;
}
