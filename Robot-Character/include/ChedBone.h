#ifndef __ROBOT_CHARACTER_CHED_BONE_
#define __ROBOT_CHARACTER_CHED_BONE_


#include <ChibiEngine/SkeletonAnimation/Animation.h>
#include <ChibiEngine/SkeletonAnimation/SkeletonPicture.h>
#include <ChedObject.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

namespace game{

    float getAngle(const glm::vec2& currPnt, const glm::vec2& delta, const glm::vec2& pos);

    class ChedLevel;
    class Skeleton;
    class ChedBone : public ChedObject{
    public:

        ChedBone(ChedLevel* level, Skeleton* skeleton, BoneType* data);
        ChedBone(ChedLevel* level,
            Skeleton* skeleton,
            SkeletonJoint* joint,
            const std::string& name="",
            const Color& color=Color::White);

        BoneType* save();
        virtual void draw(CameraCHandle camera);

        // modificators
        void setName(const std::string& name);
        void setSelected(bool flag) override;
        inline void setColor(const Color& color);
        void skeletonChanged(ChedLevel* level) override;
        void move(const glm::vec2 currPnt, const glm::vec2 &delta) override;
        void rotate(const glm::vec2 &currPnt, const glm::vec2 &delta) override;
        void scale(const glm::vec2& currPnt, const glm::vec2& delta) override;

        // Getters
        const std::string& getName() const;
        glm::vec2 getBegin() const override;
        const glm::vec2 getDirection() const;
        float getRotation() const override;
        glm::vec2 getPosition() const override;
        glm::vec2 getScale() const;
        Animation::BoneId getId() const;
        inline Color getColor() const;
        inline SkeletonJoint* getJoint();

        // Animation helpers
        glm::vec2 calcOffset(const glm::vec2& delta) const;
        glm::vec2 calcScale(const glm::vec2& delta) const;
        glm::quat calcRotation(const glm::vec2& currPnt, const glm::vec2& delta) const;

    private:
        static int boneCount;

        void calcBeginAndEnd(glm::vec2* beg, glm::vec2* end) const;
        glm::vec3 getBaseEnd() const;

        SkeletonJoint* joint;
        const TexturedObject* obj;

        float currentRotation;
        glm::vec2 currentDirection;
        std::string name;
        Color color = Color::White;

        glm::vec3 initDir;

    };

    inline Color ChedBone::getColor() const{
        return color;
    }

    inline void ChedBone::setColor(const Color& color){
        this->color = color;
    }

    inline SkeletonJoint*ChedBone::getJoint(){
        return joint;
    }
}

#endif