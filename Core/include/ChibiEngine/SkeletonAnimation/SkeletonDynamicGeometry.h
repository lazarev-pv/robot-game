#ifndef __CHIBIENGINE_SKELETON_ANIMATION_SKELETON_PHYS_PART_
#define __CHIBIENGINE_SKELETON_ANIMATION_SKELETON_PHYS_PART_

#include <ChibiEngine/Physics/Physics.h>
#include <glm/glm.hpp>
#include <ChibiEngine/SkeletonAnimation/SkeletonJoint.h>

class CharBoxType;
namespace game{

    class Skeleton;
    class SkeletonDynamicGeometry : private Noncopyable {
    public:
        SkeletonDynamicGeometry(b2World *world, CharBoxType *data, SkeletonJoint* parent, const Category &category);
        SkeletonDynamicGeometry(SkeletonDynamicGeometry &&other);
        SkeletonDynamicGeometry &operator=(SkeletonDynamicGeometry &&other);
        ~SkeletonDynamicGeometry();

        void draw(CameraCHandle camera) const;
        void applyTransform(bool reflected, Skeleton* skeleton);
        b2Body *getBody();
    private:

        SkeletonJoint* parent;
        glm::vec3 ownPos;
        float ownRotation;
        glm::vec4 color;
        std::string name;
        float hw, hh;
        b2Body *part = nullptr;
    };

}


#endif
