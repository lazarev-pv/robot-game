#ifndef __ROBOT_CHARACTER_CONCRETE_TRANSFORM_LINES_H_
#define __ROBOT_CHARACTER_CONCRETE_TRANSFORM_LINES_H_
#include <UI/TransformLine.h>
#include <ChibiEngine/SkeletonAnimation/InterpolationTransformTimeLine_Types.h>

namespace game{

    class TransformLinesGroup;
    class MoveTransformLine : public TransformLine{
    public:
        MoveTransformLine(TransformLinesGroup* parent,
                const glm::vec3& pos, const glm::vec2& size);

    protected:
        void onKeyDelete(TransformKey::Time time) override;
        void onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) override;
        void onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime) override;
        void onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) override;
    private:

        MoveTransformTimeLine* getAnimationTimeLine();
    };

    class RotateTransformLine :public TransformLine {
    public:
        RotateTransformLine(TransformLinesGroup* parent,
                const glm::vec3& pos, const glm::vec2& size);

    protected:
        void onKeyDelete(TransformKey::Time time) override;
        void onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) override;
        void onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime) override;
        void onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) override;
    private:
        RotateTransformTimeLine* getAnimationTimeLine();
    };

    class ScaleTransformLine :public TransformLine {
    public:
        ScaleTransformLine(TransformLinesGroup* parent,
                const glm::vec3& pos, const glm::vec2& size);

    protected:
        void onKeyDelete(TransformKey::Time time) override;
        void onMoveKey(TransformKey::Time oldTime, TransformKey::Time newTime) override;
        void onKeyModify(const glm::vec2& delta, const glm::vec2& currPnt, TransformKey::Time currentTime) override;
        void onCopyKey(TransformKey::Time oldTime, TransformKey::Time newTime) override;
    private:
        ScaleTransformTimeLine* getAnimationTimeLine();
    };
}

#endif