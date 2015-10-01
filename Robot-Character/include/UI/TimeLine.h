#ifndef __ROBOT_CHARACTER_UI_TIMELINE_H_
#define __ROBOT_CHARACTER_UI_TIMELINE_H_

#include <ChibiEngine/UserInterface/UIElement.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>
#include "AnimationControl.h"

class ChedTimeLine;
class TimeLineType;
namespace game{

    class AnimationControl;
    class TimeLine : public UIElement{
    public:
        TimeLine(AnimationControl* parent,
                ChedTimeLine* data,TimeLineType *settings);

        void processDraggingAction(const glm::vec2 &delta) override;
        void executeAction() override;
        bool executeScroll(bool up) override;
        void draw() override;

        inline float getCurrentTime();
        inline TransformKey::Time getMaxTime();
        inline glm::vec2 getInterval() const;

        void setCurrentTime(float time);
        void setMaxTime(TransformKey::Time time);
        void saveSettings(TimeLineType* xml);
        inline void recalcActiveLineTimeMarks();
    private:
        struct Label {
            Label(const std::string &txt, glm::vec3 coords, int size);

            void draw(const glm::mat4 &transform, const glm::vec3 &labelColor);

            SpriteText sprite;
            glm::vec3 coords;
        };

        void calcTimeMarks(float startX, float endX, int divStep, int markFreq);

        AnimationControl* parent;
        float lineHeight, midLineHeight;
        SpriteText currTimeLabel, endTimeLabel;

        ShaderValues sv;
        std::vector<Label> labels;
        float startX, endX;
        int divStep, markFreq, divCount;
        float originalDivLen;
        int labelSize;
        float currentTime = 0.0f;
        TransformKey::Time maxTime = 0;
    };

    inline glm::vec2 TimeLine::getInterval() const {
        return glm::vec2(startX, endX);
    }

    inline float TimeLine::getCurrentTime(){
        return currentTime;
    }

    inline TransformKey::Time TimeLine::getMaxTime(){
        return maxTime;
    }

    inline void TimeLine::recalcActiveLineTimeMarks(){
        if(parent->getActiveLine())
            parent->getActiveLine()->calcTimeMarks(startX, endX, divStep, markFreq);
    }

}

#endif