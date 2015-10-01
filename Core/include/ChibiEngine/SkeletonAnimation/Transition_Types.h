#ifndef __CHIBIENGINE_SKELETON_ANIMATION_TRANSITION_TYPES_
#define __CHIBIENGINE_SKELETON_ANIMATION_TRANSITION_TYPES_
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

class RotateTransitionType;
class MoveTransitionType;
class ScaleTransitionType;
class AnimationEventType;

//todo добавить инкапсуляцию что ли
namespace game{

    class Transition{
    public:
        inline Transition(float time);

        float time;
    };

    class RotateTransition : public Transition {
    public:
        inline RotateTransition(const glm::quat& rotation, float time);
        RotateTransition(RotateTransitionType* res);

        RotateTransitionType* save() const;
        glm::quat createInterpolated(float currTime, float duration, const RotateTransition & next) const;

        glm::quat value;
    };

    class MoveTransition : public Transition {
    public:
        inline MoveTransition(const glm::vec2& offset, float time);
        MoveTransition(MoveTransitionType* res);

        MoveTransitionType* save() const;
        glm::vec2 createInterpolated(float currTime, float duration, const MoveTransition & next) const;

        glm::vec2 value;
    };

    class ScaleTransition : public Transition {
    public:
        inline ScaleTransition(const glm::vec2& scale, float time);
        ScaleTransition(ScaleTransitionType* res);

        ScaleTransitionType* save() const;
        glm::vec2 createInterpolated(float currTime, float duration, const ScaleTransition & next) const;

        glm::vec2 value;
    };

    class EventTransition : public Transition {
    public:
        inline EventTransition(const std::string& name, float time);
        EventTransition(AnimationEventType* res);

        AnimationEventType* save() const;

        std::string value;
    };

}

inline game::RotateTransition::RotateTransition(const glm::quat& rotation, float time)
        :Transition(time), value(rotation){}

inline game::MoveTransition::MoveTransition(const glm::vec2& offset, float time)
        :Transition(time), value(offset){}

inline game::Transition::Transition(float time)
        :time(time){}

inline game::ScaleTransition::ScaleTransition(const glm::vec2& scale, float time)
        :Transition(time), value(scale){}

inline game::EventTransition::EventTransition(const std::string& name, float time)
        :Transition(time), value(name){}

#endif