#include <ChedLevel.h>
#include <Input/CreateBoxStrat.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>

using namespace game;
using namespace std;
using namespace glm;

CreateBoxStrat::CreateBoxStrat(ChedLevel* level):
        MouseStrategy(level), ex(0),ey(0),bx(0),by(0){}

void CreateBoxStrat::draw(CameraCHandle camera) {
    MouseStrategy::draw(camera);
    if (currentButton == MouseButton::LEFT) {
        Game::getPrimitiveDrawer()->drawRectangleBorder(createMatrix(
                        vec3(0.5 * (bx + ex), 0.5 * (by + ey), 0.0f),
                        vec2(0.5 * (fabs(bx - ex)), 0.5 * (fabs(by - ey))),
                        camera),
                Color::White);
    }
}

bool CreateBoxStrat::onDown(glm::vec2 pnt, MouseButton code){
    MouseStrategy::onDown(pnt, code);
    bx=currentPos.x;
    by=currentPos.y;
    return false;
}

void CreateBoxStrat::onMove(glm::vec2 pnt){
    MouseStrategy::onMove(pnt);
    ex=currentPos.x;
    ey=currentPos.y;
}

bool CreateBoxStrat::onUp(glm::vec2 pnt){
    MouseStrategy::onUp(pnt);
    level->createPhysBox(vec2(0.5*(bx+ex),0.5*(by+ey)),0.5*(fabs(bx-ex)),0.5*(fabs(by-ey)));
    return false;
}

const string& CreateBoxStrat::NAME = "box";
