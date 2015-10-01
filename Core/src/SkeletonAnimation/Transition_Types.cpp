#include <ChibiEngine/SkeletonAnimation/Transition_Types.h>
#include <ChibiEngine/XMLParser/CharacterParser.h>
#include <glm/ext.hpp>

using namespace game;
using namespace glm;
using namespace std;

RotateTransition::RotateTransition(RotateTransitionType* res)
        :Transition(res->time),
         value(res->q_w/*/2.0*/,0.0f,0.0f,res->q_z/*/2.0*/){}

RotateTransitionType* RotateTransition::save() const{
    RotateTransitionType* res = new RotateTransitionType();
    res->q_w= value.w;
    res->q_z= value.z;
    res->time=time;
    return res;
}

glm::quat RotateTransition::createInterpolated(float currTime, float duration, const RotateTransition & next) const{
    float _endTime = next.time>=this->time?next.time:duration;
    if(_endTime-this->time<=std::numeric_limits<float>::epsilon()){//тот же самый кадр
        return value;
    }

    float timeAlpha = (currTime-this->time)/(_endTime-this->time);
    return glm::mix(value,next.value,timeAlpha);
}

MoveTransition::MoveTransition(MoveTransitionType* res)
        :Transition(res->time),
         value(res->offset_x/*/2.0*/, res->offset_y/*/2.0*/){}

MoveTransitionType* MoveTransition::save() const{
    MoveTransitionType* res = new MoveTransitionType();
    res->offset_x= value.x;
    res->offset_y= value.y;
    res->time=time;
    return res;
}

glm::vec2 MoveTransition::createInterpolated(float currTime, float duration, const MoveTransition & next) const{
    float _endTime = next.time>=this->time?next.time:duration;
    if(_endTime-this->time<=std::numeric_limits<float>::epsilon()){//тот же самый кадр
        return value;
    }

    float timeAlpha = (currTime-this->time)/(_endTime-this->time);
    return glm::lerp(value,next.value,timeAlpha);
}



ScaleTransition::ScaleTransition(ScaleTransitionType* res)
        :Transition(res->time),
        value(res->scale_x, res->scale_y){}

ScaleTransitionType* ScaleTransition::save() const{
    ScaleTransitionType* res = new ScaleTransitionType();
    res->scale_x= value.x;
    res->scale_y= value.y;
    res->time=time;
    return res;
}

glm::vec2 ScaleTransition::createInterpolated(float currTime, float duration, const ScaleTransition & next) const{
    float _endTime = next.time>=this->time?next.time:duration;
    if(_endTime-this->time<=std::numeric_limits<float>::epsilon()){//тот же самый кадр
        return value;
    }

    float timeAlpha = (currTime-this->time)/(_endTime-this->time);
    return glm::lerp(value,next.value,timeAlpha);
}


EventTransition::EventTransition(AnimationEventType* res)
        :Transition(res->time),value(res->name){}

AnimationEventType* EventTransition::save() const{
    AnimationEventType* res = new AnimationEventType();
    res->name=value;
    res->time=time;
    return res;
}