#ifndef __CHIBIENGINE_SKELETON_ANIMATION_INTERPOLATION_TRANSFORM_TIMELINE_TYPES_
#define __CHIBIENGINE_SKELETON_ANIMATION_INTERPOLATION_TRANSFORM_TIMELINE_TYPES_

#include <ChibiEngine/SkeletonAnimation/AbstractTransformTimeLine.h>
#include <ChibiEngine/SkeletonAnimation/Transition_Types.h>

namespace game{

    template<typename TransType, typename ValueType, typename XMLType, ValueType const& defaultValue>
    class InterpolationTransformTimeLine : public AbstractTransformTimeLine<TransType,ValueType,XMLType,defaultValue> {
    public:
        using AbstractTransformTimeLine<TransType,ValueType,XMLType,defaultValue>::AbstractTransformTimeLine;

        ValueType getInterpolated(float time, float duration) const{
            if(this->transforms.size()==0)
                return defaultValue;

            auto next = this->transforms.begin();
            auto current = this->transforms.begin();

            while(next!= this->transforms.end() && next->time<time){
                current=next;
                ++next;
            }

            if(next == this->transforms.end()){
                next = current;
            }
            return current->createInterpolated(time, duration,*next);
        }

    };

    extern const glm::vec2 MOVE_DEFAULT_VALUE;
    class MoveTransformTimeLine : public InterpolationTransformTimeLine<MoveTransition, glm::vec2, MoveTransitionType, MOVE_DEFAULT_VALUE>{
    public:
        using BaseClass = InterpolationTransformTimeLine<MoveTransition, glm::vec2, MoveTransitionType,MOVE_DEFAULT_VALUE>;
        using BaseClass::InterpolationTransformTimeLine;

        inline void setKey(float time, const glm::vec2& value){
            BaseClass::setKey(time, value, [](TransformIterator iter, const glm::vec2& other){
                iter->value+=other;
            });
        }
    };


    extern const glm::quat ROTATE_DEFAULT_VALUE;
    class RotateTransformTimeLine : public InterpolationTransformTimeLine<RotateTransition, glm::quat, RotateTransitionType, ROTATE_DEFAULT_VALUE>{
    public:
        using BaseClass = InterpolationTransformTimeLine<RotateTransition, glm::quat, RotateTransitionType,ROTATE_DEFAULT_VALUE>;
        using BaseClass::InterpolationTransformTimeLine;

        inline void setKey(float time, const glm::quat& value){
            BaseClass::setKey(time, value, [](TransformIterator iter, const glm::quat& other){
                iter->value=glm::cross(iter->value,other);
            });
        }

    };

    extern const glm::vec2 SCALE_DEFAULT_VALUE;
    class ScaleTransformTimeLine : public InterpolationTransformTimeLine<ScaleTransition, glm::vec2, ScaleTransitionType,SCALE_DEFAULT_VALUE>{
    public:
        using BaseClass = InterpolationTransformTimeLine<ScaleTransition, glm::vec2, ScaleTransitionType,SCALE_DEFAULT_VALUE>;
        using BaseClass::InterpolationTransformTimeLine;

        inline void setKey(float time, const glm::vec2& value){
            BaseClass::setKey(time, value, [](TransformIterator iter, const glm::vec2& other){
                iter->value*=other;
            });
        }

    };

}





#endif