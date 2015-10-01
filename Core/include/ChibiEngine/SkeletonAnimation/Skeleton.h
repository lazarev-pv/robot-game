#ifndef __CORE_SKELETON_H_
#define __CORE_SKELETON_H_

#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <ChibiEngine/SkeletonAnimation/Animation.h>
#include <ChibiEngine/Physics/Physics.h>
#include <functional>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Render/Color.h>
#include "SkeletonPicture.h"
#include "SkeletonSlot.h"
#include "SkeletonDynamicGeometry.h"
#include "SkeletonStaticGeometry.h"

class CharacterType;
class CharBoxType;
class CharPhysType;
class b2Body;
class b2World;
class BoneSlotType;
namespace game{

    class Camera;
    class SkeletonJoint;
    class SkeletonPicture;
    class Animation;
    class Clock;
    class Event;

    struct AnimationEvent{
        std::string animationName;
        std::string eventName;
    };

    struct AnimationTimeEvent{
        std::string animationName;
        float currTime;
    };

    using AnimationHandle = std::map<std::string,Animation>::iterator;
    class Skeleton : private UniqueClass {
    public:
        static const std::string ANIMATION_END;

        class AnimationTimeLine{
        public:
            inline AnimationTimeLine(Skeleton* parent, AnimationHandle animation, bool cycled, bool removeAtEnd);

            inline void setCycled(bool val);
            void moveTime(float delta);
            void setCurrentTime(float time);
            void refreshAnimation();

            inline AnimationHandle getAnimation() const;
            inline bool isCycled() const;
        private:
            Skeleton* parent;
            float currentTime = 0.0f;
            AnimationHandle animationIter;
            bool cycled,removeAtEnd;
            const std::string& name;
        };

        Skeleton(const glm::vec2 &startPos, CharacterType* data, const Category& category);
        ~Skeleton(); //todo !! Тело не удаляется

        // Render and render modifications
        void draw(CameraCHandle camera,const Color& ambientLight) const;
        void drawBoxes(CameraCHandle camera) const;
        void setReflected(bool reflected);
        void changeSlotImage(const std::string& name, const std::string& imgName);

        // Animation process modification
        inline void setIsPlaying(bool value);
        inline bool isPlaying() const;
        inline void setPlaybackMultiplier(float playbackMultiplier);
        void setCurrentTime(float time);
        AnimationTimeLine* addAnimation(const std::string& name, bool cycled, bool removeOnEnd = true);
        void removeAnimation(const std::string& name);
        void removeAllAnimations();
        void refreshAnimation();
        std::vector<std::string> getAnimationNames() const;
        void saveAnimations(std::vector<SkeletonAnimationType*>& xmlData) const;

        // Modification
        AnimationHandle createNewAnimation(const std::string& name);
        void deleteAnimation(const std::string& name);
        void addJoint(std::unique_ptr<SkeletonJoint>&& joint);
        void addSlot(std::unique_ptr<SkeletonSlot>&& slot);
        void deleteJointWithChildren(Animation::BoneId id);
        void deleteSlot(SkeletonSlot* slot);

        // Listeners
        inline int addEventListeners(const std::function<void(const AnimationEvent&)>& functor);
        inline void addTimeEvents(const std::function<void(const AnimationTimeEvent&)>& onTimeTick);
        inline void removeEventListeners(int id);

        // Getters and finders
        inline b2Body* getBody();
        std::vector<b2Body*> getBoneSensors();
        inline const std::vector<std::unique_ptr<SkeletonSlot>>& getSlots();
        SkeletonJoint* findJoint(Animation::BoneId id) const;
        SkeletonSlot* findSlot(Animation::BoneId id) const;
        void findBoneChildren(std::set<Animation::BoneId>& res, Animation::BoneId rootId);

    private:

        void moveCurrentTime();
        void createCharacterBody(b2World *world, const glm::vec2 &startPos, CharPhysType *data, const Category &category);
        inline void fireEvent(const AnimationEvent& name);
        inline void fireEvent(const AnimationTimeEvent& name);

        // Animation state
        float playbackMultiplier = 1.0f;
        Animation activeAnimation;
        bool reflected = false;
        bool playing = true;
        int eventLastId = 0;
        int boneLastId = -1;

        // Animation data
        std::unordered_map<Animation::BoneId, std::unique_ptr<SkeletonJoint>> bones;
        std::vector<std::unique_ptr<SkeletonSlot>> pics;
        std::unordered_map<std::string, SkeletonSlot*> indexedSlots;
        std::map<std::string,Animation> animations;
        std::map<std::string,std::unique_ptr<AnimationTimeLine>> timelines;

        // Physic integration
        b2Body* body;
        std::vector<SkeletonStaticGeometry> staticGeomParts;
        std::vector<SkeletonDynamicGeometry> dynamicGeomParts;
        std::unordered_map<int,std::function<void(const AnimationEvent&)>> eventListeners;
        std::vector<std::function<void(const AnimationTimeEvent&)>> timeListeners;
        EventHandle nextFrameEvent;
    };

    inline int Skeleton::addEventListeners(const std::function<void(const AnimationEvent&)>& functor){
        eventLastId++;
        eventListeners[eventLastId]=functor;
        return eventLastId;
    }

    inline void Skeleton::addTimeEvents(const std::function<void(const AnimationTimeEvent&)>& onTimeTick){
        timeListeners.push_back(onTimeTick);
    }

    inline void Skeleton::removeEventListeners(int id){
        eventListeners.erase(id);
    }

    inline void Skeleton::fireEvent(const AnimationEvent& event){
        for(auto iter = eventListeners.begin();iter!=eventListeners.end();++iter){
            iter->second(event);
        }
    }

    inline void Skeleton::fireEvent(const AnimationTimeEvent& name){
        for(auto iter = timeListeners.begin();iter!=timeListeners.end();++iter){
            iter->operator()(name);
        }
    }

    inline void Skeleton::setPlaybackMultiplier(float playbackMultiplier){
        this->playbackMultiplier = playbackMultiplier;
    }

    inline b2Body* Skeleton::getBody(){
        return body;
    }

    inline void Skeleton::setReflected(bool reflected){
        Skeleton::reflected = reflected;
    }

    inline Skeleton::AnimationTimeLine::AnimationTimeLine(Skeleton* parent, AnimationHandle animation, bool cycled, bool removeAtEnd)
        :parent(parent),
         cycled(cycled),
         animationIter(animation),
         name(animation->second.getName()),
         removeAtEnd(removeAtEnd){}

    inline void Skeleton::setIsPlaying(bool value){
        playing = value;
    }

    inline bool Skeleton::isPlaying() const{
        return playing;
    }

    inline void Skeleton::AnimationTimeLine::setCycled(bool val){
        cycled = val;
    }

    inline AnimationHandle Skeleton::AnimationTimeLine::getAnimation() const{
        return animationIter;
    }

    inline bool Skeleton::AnimationTimeLine::isCycled() const{
        return cycled;
    }

    inline const std::vector<std::unique_ptr<SkeletonSlot>>& Skeleton::getSlots(){
        return pics;
    }
}


#endif
