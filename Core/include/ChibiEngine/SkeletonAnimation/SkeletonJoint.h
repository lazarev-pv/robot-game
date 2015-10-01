#ifndef __CHIBIENGINE_SKELETON_ANIMATION_SKELETON_JOINT_
#define __CHIBIENGINE_SKELETON_ANIMATION_SKELETON_JOINT_

#include <ChibiEngine/SkeletonAnimation/Animation.h>
#include <vector>

namespace game{

    class SkeletonJoint : private Noncopyable {
    public:
        SkeletonJoint(BoneType* xml);
        SkeletonJoint(const glm::vec2& begin, const glm::vec2& end);

        // modificators
        void applyTransition(const Animation::TimeStamp& frame);
        inline void setParent(SkeletonJoint* parent);
        inline void setId(size_t id);
        inline void prepareForAnimation();

        // animation independent getters
        inline float getLength() const;
        inline size_t getId() const;

        //current getters
        inline const glm::vec3& getAnimationPositionOffset() const;
        glm::mat4 getAnimationMatrix() const;
        inline const glm::vec3& getCurrentScale() const;
        inline SkeletonJoint* getParent() const;

        // base getters and setters
        void setBase(const glm::vec3& begin, const glm::vec3 end);
        inline const glm::vec3& getBaseBegin() const;
        glm::vec3 getBaseEnd() const;
        inline const glm::vec3& getBoneVector() const;
        inline void setBaseBegin(const glm::vec3 val);
        void setBaseRotate(float angle);
    private:

        Animation::BoneId id = Animation::NO_ID;
        SkeletonJoint* parent;
        glm::vec3 baseBegin;
        float length;
        glm::vec3 boneVector;

        glm::vec3 animationPositionOffset;
        glm::vec3 animationScale;
        glm::mat4 baseRotationTransform;
        glm::mat4 animationRotationTransform;

    };

    inline float SkeletonJoint::getLength() const{
        return length;
    }

    inline void SkeletonJoint::setParent(SkeletonJoint* parent){
        SkeletonJoint::parent = parent;
    }

    inline void SkeletonJoint::setId(size_t id){
        SkeletonJoint::id=id;
    }

    inline const glm::vec3& SkeletonJoint::getAnimationPositionOffset() const {
        return animationPositionOffset;
    }

    inline void SkeletonJoint::prepareForAnimation(){
        animationScale = glm::vec3(1,1,1);
        animationRotationTransform = glm::mat4(1.0);
        animationPositionOffset = glm::vec3(0,0,0);
    }

    inline const glm::vec3& SkeletonJoint::getBaseBegin() const{
        return baseBegin;
    }

    inline void SkeletonJoint::setBaseBegin(const glm::vec3 val){
        baseBegin = val;
    }

    inline const glm::vec3& SkeletonJoint::getBoneVector() const{
        return boneVector;
    }

    inline size_t SkeletonJoint::getId() const{
        return id;
    }

    inline const glm::vec3& SkeletonJoint::getCurrentScale() const{
        return animationScale;
    }

    inline SkeletonJoint* SkeletonJoint::getParent() const{
        return parent;
    }
}

#endif