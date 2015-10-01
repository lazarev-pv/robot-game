#include <Input/CreateBoneStrat.h>

using namespace game;
using namespace std;
using namespace glm;

CreateBoneStrat::CreateBoneStrat(ChedLevel* level):
        MouseStrategy(level),
        bone(){}

void CreateBoneStrat::draw(CameraCHandle camera){
    MouseStrategy::draw(camera);
    if(currentButton==MouseButton::LEFT){
        bone.draw();
    }
}

void CreateBoneStrat::setParent(ChedBone* parentBone){
    CreateBoneStrat::parentBone = parentBone;
}

bool CreateBoneStrat::onDown(glm::vec2 pnt, MouseButton code){
    MouseStrategy::onDown(pnt, code);
    bone.setBegin(currentPos);
    return false;
}

void CreateBoneStrat::onMove(glm::vec2 pnt){
    MouseStrategy::onMove(pnt);
    bone.setEnd(currentPos);
}

bool CreateBoneStrat::onUp(glm::vec2 pnt){
    MouseStrategy::onUp(pnt);
    level->showInputNameDialog("ChedBone Name",
    [this](const std::string& name){
        level->createBone(clickDownCoords,lastPos, name, parentBone);
        parentBone = nullptr;
    });

    return false;
}

const string& CreateBoneStrat::NAME = "create";

