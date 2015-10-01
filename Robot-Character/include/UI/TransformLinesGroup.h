#ifndef __CHARACTER_EDITOR_UI_TRANSFORM_LINES_GROUP_
#define __CHARACTER_EDITOR_UI_TRANSFORM_LINES_GROUP_

#include <ChedConstants.h>
#include <UI/TransformLine.h>
#include <ChibiEngine/Common/RAIIUtils.h>
#include <vector>
#include <ChibiEngine/SkeletonAnimation/Animation.h>

namespace game{

    class TransformLabel;
    class ChedBone;
    class Animation;
    class HeaderTransformLine;
    class TransformLinesGroup{
    public:
        using LinesCollection = std::vector<TransformLine*>;

        TransformLinesGroup(ChedBone * bone, const glm::vec3& pos, const glm::vec2& size);

        // Утилиты
        void clear();
        void calcTimeMarks(float startX, float endX, int divStep, int markFreq);
        TransformKey::Time calcMaxTime() const;
        void processHighlight(const glm::vec2& pnt);
        void saveTransform(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime);

        // Модификаторы
        void setSelectedTransform(TransformId transformId);
        void setVisible(bool visible);
        void setBoneAnimation(const Animation::Bone * anim);

        // Геттеры
        inline HeaderTransformLine* getHeader();
        inline LinesCollection& getLines();
        inline TransformId getSelectedTransform();
        inline size_t getDependentCout() const;
        inline ChedBone * getBone() const;

    private:

        HeaderTransformLine* header;
        ChedBone * bone;
        LinesCollection lines;
        TransformId lastSelectedTimeLine = NoTransform;
    };

    inline HeaderTransformLine* TransformLinesGroup::getHeader(){
        return header;
    }

    inline TransformLinesGroup::LinesCollection& TransformLinesGroup::getLines(){
        return lines;
    }

    inline TransformId TransformLinesGroup::getSelectedTransform(){
        return lastSelectedTimeLine;
    }

    inline size_t TransformLinesGroup::getDependentCout() const{
        return lines.size();
    }

    inline ChedBone * TransformLinesGroup::getBone() const{
        return bone;
    }
}

#endif