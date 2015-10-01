#include <InstrumentHelper.h>
#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Render/ScreenSystem.h>
#include <cmath>
#include <ChibiEngine/Common/MathUtils.h>
#include <ChedBone.h>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>

using namespace game;
using namespace std;
using namespace glm;

static constexpr float ROT_HSIZE = 0.1f;
static constexpr float MOVE_HSIZE = 0.025f;

InstrumentHelper::InstrumentHelper():
    mover(this),
    rotator(this),
    scaler(this){
    ref.insert(make_pair(RotateTransform, &rotator));
    ref.insert(make_pair(MoveTransform, &mover));
    ref.insert(make_pair(ScaleTransform, &scaler));
}

void InstrumentHelper::attach2Bone(ChedObject* bone){
    InstrumentHelper::bone=bone;
}

void InstrumentHelper::setCurrentTransform(TransformId value){
    auto it = ref.find(value);
    activeStrategy = it!=ref.end()?it->second:nullptr;
    activeTransform = value;
}

void InstrumentHelper::Rotator::processHover(const glm::vec2 &pnt)  {

}

InstrumentHelper::Rotator::Rotator(InstrumentHelper *parent)
    :InstrumentHelper::Strategy(parent),
    obj(Game::getResources()->findSprite("rotate_instrument")){
}

void InstrumentHelper::Rotator::draw() const{
    float scale = ROT_HSIZE/obj->getHW();
    obj->draw(
            MatrixBuilder()
                .pos(vec4(parent->bone->getBegin(), 0.0f,1.0))
                .rotate(parent->bone->getRotation())
                .scale(scale,scale)
                .camera(Game::getCamera())
                .toMatrix(),
            Color(Color::White).setAlpha(parent->mouseDragging ?0.5f:1.0f));
}

glm::vec2 InstrumentHelper::Rotator::constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const{
    return delta;
}


vec3 getYLocalPoint(const glm::vec2 &pnt, ChedObject* bone){
    CameraCHandle camera = Game::getCamera();
    vec3 localPnt;
    localPnt = camera->convertToWorldCoordinates(pnt);
    localPnt = localPnt-vec3(bone->getPosition(),0);
    localPnt = glm::rotate(localPnt,-bone->getRotation(),vec3(0,0,1));
    return localPnt;
}

vec3 getXLocalPoint(const glm::vec2 &pnt, ChedObject* bone){
    CameraCHandle camera = Game::getCamera();
    vec3 localPnt;
    localPnt = camera->convertToWorldCoordinates(pnt);
    localPnt = localPnt-vec3(bone->getPosition(),0);
    localPnt = glm::rotate(localPnt,-bone->getRotation(),vec3(0,0,1));
    localPnt = localPnt-vec3(0,-MOVE_HSIZE*3.0f,0);
    localPnt = glm::rotate(localPnt,-static_cast<float>(M_PI_2),vec3(0,0,1));
    localPnt = localPnt+vec3(0,-MOVE_HSIZE*3.0f,0);
    return localPnt;
}

void InstrumentHelper::Mover::processHover(const glm::vec2 &pnt) {
    {
        vec3 localPnt = getYLocalPoint(Game::getInputSystem()->getCurrentMousePosition(),parent->bone);
        yConstraintActive = game::contains(vec2(0,0),
                MOVE_HSIZE, MOVE_HSIZE*obj->getHH()/obj->getHW(),
                vec2(localPnt.x,localPnt.y));
    }
    {
        vec3 localPnt = getXLocalPoint(Game::getInputSystem()->getCurrentMousePosition(),parent->bone);
        xConstraintActive = game::contains(vec2(0,0),
                MOVE_HSIZE, MOVE_HSIZE*obj->getHH()/obj->getHW(),
                vec2(localPnt.x,localPnt.y));
    }
}

InstrumentHelper::Mover::Mover(InstrumentHelper* parent)
    :Mover(parent,"move_instrument"){}

InstrumentHelper::Mover::Mover(InstrumentHelper* parent, const std::string& pres)
        :InstrumentHelper::Strategy(parent),
         obj(Game::getResources()->findSprite(pres)){}

void InstrumentHelper::Mover::draw() const {

    float scale = MOVE_HSIZE/obj->getHW();
    obj->draw(MatrixBuilder()
                    .pos(parent->bone->getPosition())
                    .camera(Game::getCamera())
                    .rotate(parent->bone->getRotation()/*getAngle(parent->bone->getDirection())*/)
                    .scale(scale,scale)
                    .toMatrix(),
            Color(yConstraintActive?Color::White:Color::Red).setAlpha(parent->mouseDragging ?0.3f:1.0f));

/*
    Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(
                    getXLocalPoint(
                            Game::getInputSystem()->getCurrentMousePosition(),
                            parent->bone),
                    vec2(0.01f,0.01f),
                    Game::getCamera()),
            Color::Black
    );
*/

    obj->draw(MatrixBuilder()
                    .pos(parent->bone->getPosition())
                    .camera(Game::getCamera())
                    .rotate(static_cast<float>(M_PI_2), vec3(0,-MOVE_HSIZE*3.0f,0))
                    .rotate(getAngle(parent->bone->getDirection()))
                    .scale(scale,scale)
                    .toMatrix(),
            Color(xConstraintActive?Color::White:Color::Green).setAlpha(parent->mouseDragging ?0.3f:1.0f));
}

glm::vec2 InstrumentHelper::Mover::constrain(const glm::vec2& grabPnt, const glm::vec2& delta) const{
    if(!yConstraintActive && !xConstraintActive){
        return delta;
    }

    vec2 dir = parent->bone->getDirection();
    if(yConstraintActive){
        return dir*dot(delta,dir);
    }

    vec2 dirNormal(-dir.y,dir.x);
    return dirNormal*dot(delta,dirNormal);
}


InstrumentHelper::Scaler::Scaler(InstrumentHelper* parent)
        :InstrumentHelper::Mover(parent,"scale_instrument"){}

