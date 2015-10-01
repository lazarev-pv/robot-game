#include <UI/ConcreteTransformLines.h>
#include <UI/AnimationControl.h>
#include <UI/TransformLinesGroup.h>
#include <ChedLevel.h>
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChedBone.h>

using namespace std;
using namespace game;
using namespace glm;

inline static Animation* getAnimation(){
    return ChedLevel::instance()->getAnimationControl()->getAnimation();
}

MoveTransformLine::MoveTransformLine(TransformLinesGroup* parent,
        const glm::vec3& pos, const glm::vec2& size)
    :TransformLine(parent,"Move","icon.move",rgb(3, 116, 202),pos,size, MoveTransform)
{}

MoveTransformTimeLine* MoveTransformLine::getAnimationTimeLine(){
    return &(getAnimation()->requestBone(parent->getBone()->getId())->moves);

}

void MoveTransformLine::onKeyDelete(TransformKey::Time time){
    getAnimationTimeLine()->deleteKey(toAnimationTime(time));
}

void MoveTransformLine::onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime){
    getAnimationTimeLine()->setKey(toAnimationTime(currentTime), parent->getBone()->calcOffset(delta));
    ChedLevel::instance()->getAnimationControl()->refreshAnimation();
}

void MoveTransformLine::onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) {
    getAnimationTimeLine()->moveKey(toAnimationTime(oldTime), toAnimationTime(newTime));
}

void MoveTransformLine::onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) {
    getAnimationTimeLine()->copyKey(toAnimationTime(oldTime), toAnimationTime(newTime));
}

///////////////////////////// ROTATE

RotateTransformLine::RotateTransformLine(TransformLinesGroup* parent,
        const glm::vec3& pos, const glm::vec2& size)
    :TransformLine(parent,"Rotate","icon.rotate",rgb(2, 195, 2),pos,size,RotateTransform)
{}

RotateTransformTimeLine* RotateTransformLine::getAnimationTimeLine(){
    return &(getAnimation()->requestBone(parent->getBone()->getId())->rotations);
}

void RotateTransformLine::onKeyDelete(TransformKey::Time time){
    getAnimationTimeLine()->deleteKey(toAnimationTime(time));
}

void RotateTransformLine::onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime){
    getAnimationTimeLine()->setKey(toAnimationTime(currentTime), parent->getBone()->calcRotation(currPnt, delta));
    ChedLevel::instance()->getAnimationControl()->refreshAnimation();
}

void RotateTransformLine::onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) {
    getAnimationTimeLine()->moveKey(toAnimationTime(oldTime), toAnimationTime(newTime));
}

void RotateTransformLine::onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) {
    getAnimationTimeLine()->copyKey(toAnimationTime(oldTime), toAnimationTime(newTime));
}

///////////////////////////// SCALE

ScaleTransformLine::ScaleTransformLine(TransformLinesGroup* parent,
        const glm::vec3& pos, const glm::vec2& size)
    :TransformLine(parent,"Scale","icon.scale",rgb(195, 2, 2),pos,size,ScaleTransform)
{}

ScaleTransformTimeLine* ScaleTransformLine::getAnimationTimeLine(){
    return &(getAnimation()->requestBone(parent->getBone()->getId())->scales);
}

void ScaleTransformLine::onKeyDelete(TransformKey::Time time){
    getAnimationTimeLine()->deleteKey(toAnimationTime(time));
}

void ScaleTransformLine::onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime){
   getAnimationTimeLine()->setKey(toAnimationTime(currentTime), parent->getBone()->calcScale(delta));
    ChedLevel::instance()->getAnimationControl()->refreshAnimation();
}

void ScaleTransformLine::onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) {
    getAnimationTimeLine()->moveKey(toAnimationTime(oldTime), toAnimationTime(newTime));
}

void ScaleTransformLine::onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) {
    getAnimationTimeLine()->copyKey(toAnimationTime(oldTime), toAnimationTime(newTime));
}
