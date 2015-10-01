#include <Input/MouseStrategy.h>
#include <ChibiEngine/Render/Camera.h>

using namespace game;
using namespace std;
using namespace glm;

const string& NoneStrat::NAME = "none";


bool ChedInputListener::MouseStrategy::onDown(glm::vec2 pntScreen, MouseButton code){
    vec3 _pnt = Game::getCamera()->convertToWorldCoordinates(pntScreen);
    currentButton = code;
    clickDownCoords = vec2(_pnt.x, _pnt.y);
    currentPos = clickDownCoords;
    lastPos = clickDownCoords;
    return false;
}

bool ChedInputListener::MouseStrategy::onUp(glm::vec2 pnt){
    currentButton=MouseButton::NO_BUTTON;
    return false;
}

void ChedInputListener::MouseStrategy::onMove(glm::vec2 pntScreen){
    vec3 _pnt = Game::getCamera()->convertToWorldCoordinates(pntScreen);
    currentPos = vec2(_pnt.x,_pnt.y);
    delta = currentPos - lastPos;
    lastPos = currentPos;
}

MouseButton ChedInputListener::MouseStrategy::getCurrentButton(){
    return currentButton;
}

const glm::vec2& ChedInputListener::MouseStrategy::getMoveDelta(){
    return delta;
}

void ChedInputListener::MouseStrategy::draw(CameraCHandle camera){
}

const glm::vec2& ChedInputListener::MouseStrategy::getCurrentPos(){
    return currentPos;
}

ChedInputListener::MouseStrategy::MouseStrategy(ChedLevel* level):
        level(level),
        lastPos(0,0),
        clickDownCoords(0,0),
        currentButton(MouseButton::NO_BUTTON),
        currentPos(0,0){}

