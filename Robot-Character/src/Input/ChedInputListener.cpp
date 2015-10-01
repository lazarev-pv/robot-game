#include <Input/ChedInputListener.h>
#include <Input/MouseStrategy.h>
#include <Input/SelectStrat.h>
#include <Input/CreateBoneStrat.h>
#include <Input/CreateBoxStrat.h>
#include <ChedLevel.h>
#include <ChibiEngine/Log/LoggingSystem.h>
#include <glm/ext.hpp>
#include <UI/AnimationControl.h>
#include <ChedBone.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/UserInterface/UserInterface.h>

using namespace game;
using namespace std;
using namespace glm;

const static char* CHED_LOGGING="ChedInputListener";
const static float CAMERA_MOVE_DELTA = 0.05;

const glm::vec2& ChedInputListener::getCurrentPos(){
	return strat->getCurrentPos();
}

ChedInputListener::ChedInputListener(ChedLevel* level)
	:level(level),
	 logger(Game::getLoggingSystem()->createLogger(CHED_LOGGING)),
	 ctrlPressed(false),
	 shiftPressed(false),
	 nextStrat(nullptr),
	 ignoreUp(false)
{
	reference[NoneStrat::NAME] = new NoneStrat(level);
	reference[CreateBoneStrat::NAME] = new CreateBoneStrat(level);
	reference[SelectStrat::NAME] = new SelectStrat(level);
	//reference[CreateBoxStrat::NAME] = new CreateBoxStrat(level);
	UserInterface * ui = Game::getUserInterface();

	/*actionButton = ui->findDDLB("action_type_button");
	selectButton = ui->findDDLB("select_type_button");*/
	ui->addEventListener("bone_menu",bind(&ChedInputListener::onBoneContextMenu, this, placeholders::_1));
	ui->addEventListener("ched_menu",bind(&ChedInputListener::onChedContextMenu, this, placeholders::_1));
	strat = reference[SelectStrat::NAME];
}

ChedInputListener::~ChedInputListener(){
	for(auto iter : reference){
		delete iter.second;
	}
}

bool ChedInputListener::onClickDown(MouseButton code, const glm::vec2 &pnt){
	switch(code){
	case MouseButton::RIGHT:
		level->viewContextMenu(pnt);
		break;
	case MouseButton::LEFT:
		strat->onDown(pnt, code);
		break;
	case MouseButton::SCROLL_UP:
		if(!ctrlPressed){
			level->moveCamera(vec2(0,CAMERA_MOVE_DELTA));
		}else{
			level->zoomCamera(CAMERA_MOVE_DELTA);
		}
		return true;
	case MouseButton::SCROLL_DOWN:
        if(!ctrlPressed){
			level->moveCamera(vec2(0,-CAMERA_MOVE_DELTA));
		}else{
			level->zoomCamera(-CAMERA_MOVE_DELTA);
		}
		break;
	case MouseButton::SCROLL_LEFT:
			level->moveCamera(vec2(-CAMERA_MOVE_DELTA,0));
			break;
		case MouseButton::SCROLL_RIGHT:
			level->moveCamera(vec2(CAMERA_MOVE_DELTA,0));
			return true;
	default:
		return false;
	}
	return true;
}

bool ChedInputListener::onClickUp(MouseButton code, const glm::vec2 &pnt){
	//Необходимо дождаться mouseDown?
	if(ignoreUp){
		ignoreUp=false;
		return true;
	}
	if(code == MouseButton::LEFT){
        level->mouseClickUp();

		bool res = strat->onUp(pnt);
		if(nextStrat!=nullptr){
			strat=nextStrat;
			nextStrat=nullptr;
		}
		return res;

	}else if(code == MouseButton::RIGHT){
		return true;
	}else{
		bool res = strat->onUp(pnt);
		if(nextStrat!=nullptr){
			strat=nextStrat;
			nextStrat=nullptr;
		}
		return res;
	}
}

bool ChedInputListener::onMouseMove(const glm::vec2 &pnt){
	strat->onMove(pnt);
    level->getAnimationControl()->processHighlight(pnt);
	return false;
}

bool ChedInputListener::onKeyPressed(KeyCode keyCode, InputEvent::State state){
	logger.trace(state == InputEvent::PRESSED ? "%c - %i pressed " : "%c - %i released ", keyCode, keyCode);

    if(state!=InputEvent::PRESSED)
        return false;

	switch(keyCode){
	case DELETE_KEY:
        level->deleteBone();
		return true;
	}
	if(!ctrlPressed)
		return false;

	switch(keyCode){
	case CTRL_M:
        level->getAnimationControl()->selectTransform(MoveTransform);
		break;
	case CTRL_R:
        level->getAnimationControl()->selectTransform(RotateTransform);
		break;
	case CTRL_L:
		level->getAnimationControl()->selectTransform(ScaleTransform);
		break;
	case CTRL_S:
		level->save();
		break;
	case CTRL_D:
		ChedLevel::instance()->toggleDebugMode();
		break;
	}
	return false;
}

bool ChedInputListener::onSpecialKeyPressed(KeyCode keyCode, InputEvent::State state){
	logger.trace(state == InputEvent::PRESSED ? "special %i pressed " : "special %i released ", keyCode);

    switch(keyCode){
	case LCTRL_KEY:
	case RCTRL_KEY:
		ctrlPressed = state==InputEvent::PRESSED;
		break;
	case LSHIFT:
	case RSHIFT:
		shiftPressed = state==InputEvent::PRESSED;
		break;
	}
	return false;
}

void ChedInputListener::draw(CameraCHandle camera){
	strat->draw(camera);
}

void ChedInputListener::onBoneContextMenu(const std::string& name){
	if(name=="add child"){
		nextStrat=strat;
		strat=reference[CreateBoneStrat::NAME];
		static_cast<CreateBoneStrat*>(strat)->setParent(ChedLevel::instance()->getCurrentBone());
		ignoreUp=true;
	}else if(name=="set presentation"){
		level->addPresentation();
	}else if(name=="add physics box"){
		nextStrat=strat;
		strat=reference[CreateBoxStrat::NAME];
		ignoreUp=true;
	}
}
void ChedInputListener::onChedContextMenu(const std::string& name){
	if(name=="add bone"){
		nextStrat=strat;
		strat=reference[CreateBoneStrat::NAME];
		ignoreUp=true;
	}else if(name=="add physics box"){
		nextStrat=strat;
		strat=reference[CreateBoxStrat::NAME];
		ignoreUp=true;
	}
}

