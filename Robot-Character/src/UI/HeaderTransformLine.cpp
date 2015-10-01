#include <UI/HeaderTransformLine.h>
#include <UI/TransformLinesGroup.h>

using namespace game;
using namespace std;
using namespace glm;

HeaderTransformLine::HeaderTransformLine(TransformLinesGroup* parent,
        const std::string& name,
        const glm::vec3& pos, const glm::vec2& size)
        :TransformLine(parent,name, "icon.bone", Color::White, pos, size, NoTransform){

    bgColor = rgb(118, 147, 148);
    drawCurves = false;
}

void HeaderTransformLine::onKeyPressed(KeyCode keyCode, bool special){
    for(auto& dep : parent->getLines()){
        dep->onKeyPressed(keyCode, special);
    }
}

void HeaderTransformLine::recalcKeys(){
    keys.clear();
    for(auto& dep:parent->getLines()){
        for(auto key : dep->getKeys()){
            keys[key.first]=TransformKey(key.first);
        }
    }
}

void HeaderTransformLine::executePushOnAnimation() {
    vec2 pnt = Game::getInputSystem()->getCurrentMousePosition();
    if(findKey(pnt)){
        vec2 tstPnt(pnt.x,pnt.y-getHH()*2.0f);
        for(auto& dep : parent->getLines()){
            dep->try2HookKey(tstPnt);
            tstPnt.y-=getHH()*2.0f;
        }
    }
}

void HeaderTransformLine::executePushOffAnimation() {
    vec2 pnt = Game::getInputSystem()->getCurrentMousePosition();
    vec2 tstPnt(pnt.x,pnt.y-getHH()*2.0f);
    for(auto& dep : parent->getLines()){
        dep->releaseKey(tstPnt);
        tstPnt.y-=getHH()*2.0f;
    }
}
