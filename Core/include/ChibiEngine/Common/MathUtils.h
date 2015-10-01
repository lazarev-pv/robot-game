#ifndef __CHIBIENGINE_COMMON_MATH_UTILS_H
#define	 __CHIBIENGINE_COMMON_MATH_UTILS_H

#include <glm/glm.hpp>
namespace game {

    // geometry
    bool contains(const glm::vec2& pos, float hw, float hh, const glm::vec2& pnt);
    float getAngle(const glm::vec2& direction);
    template<typename T> inline T transformInterval(T srcBeg, T srcEnd, T destBeg, T destEnd, T value);
    inline glm::vec3 toVec3(const glm::vec4& point);
    inline glm::vec2 toVec2(const glm::vec4& point);
    inline glm::vec2 toVec2(const glm::vec3& point);

    // easing
    inline double easeIn(double passedTime,double duration, double p);
    inline double easeOut(double passedTime,double duration, double p);
    inline double easeInOut(double passedTime,double duration, double p);

}

template<typename T>
inline T game::transformInterval(T srcBeg, T srcEnd, T destBeg, T destEnd, T value){
    T k = (destEnd-destBeg)/(srcEnd-srcBeg);
    T b = (destBeg*srcEnd-srcBeg*destEnd)/(srcEnd-srcBeg);
    return k*value+b;
}

inline double game::easeIn(double passedTime,double duration, double p){
    assert(p>0);
    assert(passedTime>=0);
    assert(duration>0);
    return std::pow(passedTime/duration,p);
}

inline double game::easeOut(double passedTime,double duration, double p){
    assert(p>0);
    assert(passedTime>=0);
    assert(duration>0);
    return (1-std::pow(fabs(1-passedTime/duration),p));
}

inline double game::easeInOut(double passedTime,double duration, double p){
    assert(duration>0);
    return (passedTime/duration)<=0.5?
            easeIn(passedTime,duration,p):
            easeOut(passedTime, duration, p);
}

inline glm::vec3 game::toVec3(const glm::vec4& p){
    return glm::vec3(p.x/p.w,p.y/p.w,p.z/p.w);
}

inline glm::vec2 game::toVec2(const glm::vec4& p){
    return glm::vec2(p.x/p.w,p.y/p.w);
}

inline glm::vec2 game::toVec2(const glm::vec3& p){
    return glm::vec2(p.x,p.y);
}

#endif