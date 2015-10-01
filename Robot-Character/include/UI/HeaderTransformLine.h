#ifndef __ROBOT_CHARACTER_HEADER_TRANSFORM_LINE_H_
#define __ROBOT_CHARACTER_HEADER_TRANSFORM_LINE_H_
#include <UI/TransformLine.h>

#include <ChibiEngine/SkeletonAnimation/Animation.h>
#include <ChedConstants.h>

namespace game{

    class ChedBone;
    class HeaderTransformLine : public TransformLine{
    public:

        HeaderTransformLine(TransformLinesGroup* parent,
                const std::string& name,
                const glm::vec3& pos, const glm::vec2& size);

        //Свои методы
        void recalcKeys();

        // Переорпделенные у трансформ лайн методы
        void executePushOnAnimation() override;
        void executePushOffAnimation() override;
        void onKeyPressed(KeyCode keyCode, bool special) override;

    protected:
        void onKeyDelete(TransformKey::Time time) override {};
        void onKeyModify(const glm::vec2 &delta, const glm::vec2 &currPnt, TransformKey::Time currentTime) override {};
        void onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) override{};
        void onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) override{};
    };



}

#endif