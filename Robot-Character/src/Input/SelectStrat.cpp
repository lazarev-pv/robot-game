#include <Input/SelectStrat.h>
#include <ChedLevel.h>

using namespace game;
using namespace glm;
using namespace std;

SelectStrat::SelectStrat(ChedLevel* level):
        MouseStrategy(level){}

bool SelectStrat::onDown(glm::vec2 pnt, MouseButton code){
    MouseStrategy::onDown(pnt, code);
    level->selectBone(currentPos);
    return false;
}

void SelectStrat::onMove(glm::vec2 pnt){
    MouseStrategy::onMove(pnt);
    if(currentButton==MouseButton::LEFT){
        level->mouseDrag(delta, clickDownCoords, currentPos);
    }else if(currentButton==MouseButton::NO_BUTTON){
        level->hover(currentPos);
    }
}

const string& SelectStrat::NAME = "select";
