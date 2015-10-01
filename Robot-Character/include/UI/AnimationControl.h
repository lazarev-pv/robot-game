#ifndef __ROBOT_CHARACTER_UI_ANIMATION_CONTROL_H_
#define __ROBOT_CHARACTER_UI_ANIMATION_CONTROL_H_

#include <ChibiEngine/Render/Shaders.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChibiEngine/Clock/ClockHandles.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include <ChibiEngine/Common/RAIIUtils.h>

#include <UI/TransformLine.h>
#include <ChibiEngine/UserInterface/ElementCollection.h>
#include <ChibiEngine/UserInterface/UserInterface.h>
#include "TransformLinesGroup.h"

class ChedTimeLine;
class TimeLineType;
namespace game {

    inline float toAnimationTime(TransformKey::Time time){
        return time*0.01f;
    }

    inline float toTransformKeyTime(float animTime){
        return animTime*100.0f;
    }

    class Skeleton;
    class SpriteText;
    class NineSliceSprite;
    class Clock;
    class ChedBone;
    class TimeLine;
    class Button;
    class AnimationControl : public ElementCollection {
    public:

        AnimationControl(ChedTimeLine *data, TimeLineType *settings, ClockHandle clock, std::vector<std::unique_ptr<ChedBone>> &bones, Skeleton* skeleton);

        void draw() override;

        void processHighlight(const glm::vec2 &pnt);
        void setPlaying(bool playing);
        void setCurrentTimeSkeleton(TransformKey::Time time);

        void setAnimation(const std::string& name);
        void setPlaybackMultiplier(float multiplier);
        void saveTransform(const glm::vec2 &delta, const glm::vec2 &currPnt);
        void selectTransform(TransformId transformId);
        void selectBone(const std::string &boneName);
        TransformKey::Time calcMaxTime();
        void refreshAnimation();

        void addBone(ChedBone * bone);
        void deleteBone(ChedBone * bone);

        inline Animation *getAnimation() const;
        inline TransformKey::Time getMaxTime() const;
        inline TransformLinesGroup* getActiveLine() const;
        inline TimeLine* getTimeLine() const;
        TimeLineType *saveSettings();

        void setSlotsVisible(bool flag) override;

    private:
        class NextFrame;

        void setActiveLine(TransformLinesGroup* active);

        void onPlay(const std::string& name);

        Skeleton::AnimationTimeLine* animationTimeLine = nullptr;
        std::vector<std::unique_ptr<ChedBone>> &bones;

        std::unordered_map<std::string, std::unique_ptr<TransformLinesGroup>> lines;
        std::unordered_map<Animation::BoneId, TransformLinesGroup *> linesById;
        TransformLinesGroup *activeLine = nullptr;
        TimeLine* timeline;
        Button *playButton, *cycledButton, *backButton, *forwardButton;
        const Logger logger;
        Skeleton* skeleton;
    };

    inline Animation*AnimationControl::getAnimation() const {
        return animationTimeLine?&animationTimeLine->getAnimation()->second:nullptr;
    }

    inline TransformLinesGroup* AnimationControl::getActiveLine() const{
        return activeLine;
    }

    inline TimeLine* AnimationControl::getTimeLine() const{
        return timeline;
    }
}

#endif
