/*
 * AnimatedSprite.h
 *
 *  Created on: May 9, 2014
 *      Author: paul
 */

#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include <ChibiEngine/Render/Sprites/TexturedObject.h>
#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <map>

namespace game {

    class EventSystem;
    class Event;
    class Clock;

    class AnimatedSprite : public TexturedObject {
    public:
        AnimatedSprite(SpriteInfo::CHandle pres);
        virtual ~AnimatedSprite();

        std::string getInitalAnimationName();

        void setAnimation(const std::string& name);
        void nextFrame();
        void draw(const glm::mat4& transform, const Color& ambient=Color::White) const override;

    private:
        SpriteInfo::CHandle data;

        void deleteOldAnimation();
        void generateAnimation(const std::string& name);

        EventHandle currentAnimationEvent;

        unsigned currentFrame;
        SpriteInfo::AnimationCHandle currentAnimation;

    };


}  // namespace game



#endif /* ANIMATEDSPRITE_H_ */
