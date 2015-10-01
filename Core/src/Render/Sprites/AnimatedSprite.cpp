#include <ChibiEngine/Render/Sprites/AnimatedSprite.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/Clock/EventSystem.h>

#define GLM_FORCE_RADIANS

#include <glm/ext.hpp>


using namespace game;
using namespace std;
using namespace glm;

AnimatedSprite::AnimatedSprite(SpriteInfo::CHandle pres) :
        TexturedObject(pres->hw, pres->hh, pres->texture, pres->name),
        data(pres),
        currentAnimationEvent{},
        currentAnimation{} {

    float hw = getHW();
    float hh = getHH();
    vector<GLfloat> verts;
    for (const auto& iter:pres->animations) {
        for (const auto& frame : iter.second->frames) {

            verts.push_back(-hw);
            verts.push_back(-hh);
            verts.push_back(0);
            verts.push_back(frame->topLeftPnt.x);
            verts.push_back(frame->bottomRightPnt.y);

            verts.push_back(-hw);
            verts.push_back(hh);
            verts.push_back(0);
            verts.push_back(frame->topLeftPnt.x);
            verts.push_back(frame->topLeftPnt.y);

            verts.push_back(hw);
            verts.push_back(-hh);
            verts.push_back(0);
            verts.push_back(frame->bottomRightPnt.x);
            verts.push_back(frame->bottomRightPnt.y);

            verts.push_back(hw);
            verts.push_back(hh);
            verts.push_back(0);
            verts.push_back(frame->bottomRightPnt.x);
            verts.push_back(frame->topLeftPnt.y);
        }
    }
    sv.fillBuffer(verts);
    generateAnimation(getInitalAnimationName());

}

std::string AnimatedSprite::getInitalAnimationName() {
    return data->initialAnimation;
}

AnimatedSprite::~AnimatedSprite() {
    deleteOldAnimation();
}

void AnimatedSprite::nextFrame() {
    currentFrame++;
    if (currentAnimation->frames.size() <= currentFrame)
        currentFrame = 0;
}

void AnimatedSprite::setAnimation(const std::string& name) {
    if (currentAnimation != nullptr && currentAnimation->name == name) {
        return;
    }

    generateAnimation(name);
}

void AnimatedSprite::deleteOldAnimation() {
    // Удаляем старую анимацию
    if (currentAnimationEvent) {
        Game::getEventSystem()->deleteEvent(data->clock, currentAnimationEvent);
        currentAnimationEvent = NullEventHandle;
    }
}

void AnimatedSprite::generateAnimation(const std::string& name) {
    currentFrame = 0;
    deleteOldAnimation();

    auto iter = data->animations.find(name);
    if (iter == data->animations.end())
        return;

    currentAnimation = iter->second.get();
    currentFrame = 0;

    if (currentAnimation->frames.size() > 1) {
        if (iter->second->cycled)
            currentAnimationEvent = Game::getEventSystem()->scheduleInfiteRepeat(
                    data->clock,
                    bind(&AnimatedSprite::nextFrame, this),
                    data->clock->getTime() + currentAnimation->frameTime, currentAnimation->frameTime);
        else
            currentAnimationEvent = Game::getEventSystem()->schedule(
                    data->clock,
                    bind(&AnimatedSprite::nextFrame, this),
                    data->clock->getTime() + currentAnimation->frameTime, currentAnimation->frameTime,
                    currentAnimation->frames.size() - 1);
    }
}

void AnimatedSprite::draw(const glm::mat4& transform, const Color& ambient) const {
    sv.startOffset = currentAnimation->initialOffset + currentFrame * 4 * 5;
    TexturedObject::draw(transform, ambient);
}

