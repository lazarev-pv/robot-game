#include <CameraController.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include "Generated/RobotConfigParser.h"
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Physics/Physics.h>
#include <RobotLevel.h>
#include <ChibiEngine/Clock/ClockSystem.h>
#include <cmath>

using namespace std;
using namespace game;
using namespace glm;

CameraController::CameraController(RobotCameraSettings* gs, CameraConfigType* ls):
        left(gs->constraints->left),
        right(gs->constraints->right),
        top(gs->constraints->top),
        bottom(gs->constraints->bottom),
        hook(nullptr),
        velocity(0,0,0),
        offsetVelocity(ls->xVelocity,ls->yVelocity,0)
{
    Game::getCamera()->setPosition(parse(gs->initPos));
    //updateId = Game::getPhysicsSystem()->addAdditionalProcessing(bind(&CameraController::update,this));

    //Game::getEventSystem()->doEveryFrame(bind(&CameraController::update, this));

    Game::getEventSystem()->scheduleInfiteRepeat(Game::getClockSystem()->getImmutableClock(), bind(&CameraController::update, this), Game::getLevelClock()->getTime(), 20);
}

CameraController::~CameraController(){
    //Game::getPhysicsSystem()->deleteAdditionalProcessing(updateId);
}

void CameraController::update(){
    CameraHandle c = Game::getCamera();

    vec3 cPos = c->getPosition();
    if(hook!=nullptr) {
        //todo это конечно костыль но он работает...
        vec3 realOffset = offset;
        if(hook==RobotLevel::instance()->getCharacter()){
            if(!static_cast<Character*>(hook)->isLookForward())
                realOffset.x= -realOffset.x;
        }

/*        vec3 offset = offsetVelocity*(dt*0.001f);*/
        cPos += (realOffset+hook->getPosition()-cPos)
                * glm::pow(offsetVelocity*Game::getLevelClock()->getScaleFactor(),
                           vec3(easingPow,easingPow,1));
        cPos.x=clamp(cPos.x,left,right);
        cPos.y=clamp(cPos.y,bottom,top);
    }

    cPos+=velocity/10.0f;
    c->setPosition(cPos);
}