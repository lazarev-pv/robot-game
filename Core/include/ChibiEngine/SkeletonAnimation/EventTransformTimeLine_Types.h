#ifndef __CHIBIENGINE_SKELETON_ANIMATION_EVENT_TRANSFORM_TIMELINE_TYPES_
#define __CHIBIENGINE_SKELETON_ANIMATION_EVENT_TRANSFORM_TIMELINE_TYPES_

#include <ChibiEngine/SkeletonAnimation/AbstractTransformTimeLine.h>
#include <ChibiEngine/SkeletonAnimation/Transition_Types.h>

namespace game{

    template<typename TransType, typename ValueType, typename XMLType, ValueType const& defaultValue>
    class StepTransformTimeLine : public AbstractTransformTimeLine<TransType,ValueType,XMLType,defaultValue> {
    public:
        using AbstractTransformTimeLine<TransType,ValueType,XMLType,defaultValue>::AbstractTransformTimeLine;

        void check(float time, const std::function<const ValueType&>& handler) const{
            if(this->transforms.size()==0)
                return;

            auto next = this->transforms.begin();
            auto current = this->transforms.begin();

            while(next!= this->transforms.end() && next->time<time){
                current=next;
                ++next;
            }

            if(next == this->transforms.end()){
                next = current;
            }

            int currTime = static_cast<int>(round(current->time*100.0f));

            if(currTime!=lastEventTime){
                lastEventTime=currTime;
                handler(next->value);
            }

        }
    private:
        int lastEventTime = -1;
    };

    extern const std::string EVENT_DEFAULT_VALUE;
    class EventTransformTimeLine : public StepTransformTimeLine<EventTransition, std::string, AnimationEventType, EVENT_DEFAULT_VALUE>{
    public:
        using BaseClass = StepTransformTimeLine<EventTransition, std::string, AnimationEventType, EVENT_DEFAULT_VALUE>;
        using BaseClass::StepTransformTimeLine;

        inline void setKey(float time, const std::string& value){
            BaseClass::setKey(time, value, [](TransformIterator iter, const std::string& other){
                iter->value=other;
            });
        }
    };

}





#endif