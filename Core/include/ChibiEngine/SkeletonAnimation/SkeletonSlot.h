#ifndef _CHIBIENGINE_SKELETON_ANIMATION_SKELETONSLOT_H_
#define _CHIBIENGINE_SKELETON_ANIMATION_SKELETONSLOT_H_

#include <ChibiEngine/Common/ClassUtils.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <ChibiEngine/Render/RenderHandles.h>

class BoneSlotType;
namespace game{

    class SkeletonJoint;
    class SkeletonPicture;
    class SkeletonSlot : private Noncopyable{
    public:
        SkeletonSlot(BoneSlotType* slot, SkeletonJoint* bone);
        SkeletonSlot(SkeletonJoint* parent,
                const std::string& name,
                int zOrder,
                size_t id);

        void addPicture(const std::string& name,
                TexturedObjectCHandle pres,
                const glm::vec3& pos,
                const glm::vec2& size,
                float rot);

        const SkeletonPicture& getActive() const;
        SkeletonPicture& getActive();
        SkeletonPicture& getPicture(const std::string& name);
        SkeletonPicture& getPictureByTextureName(const std::string& name);
        inline const std::vector<SkeletonPicture>& getPresentations() const;

        inline SkeletonJoint* getJoint();
        void setActive(const std::string& name);
        inline int getZOrder() const;
        inline size_t getId() const;

        inline const std::string& getName() const;
    private:
        SkeletonJoint* parent;
        size_t active;
        std::vector<SkeletonPicture> presRef;
        int zOrder;
        std::string name;
        size_t id;
    };

    inline SkeletonJoint* SkeletonSlot::getJoint() {
        return parent;
    }

    inline int SkeletonSlot::getZOrder() const{
        return zOrder;
    }

    inline const std::string& SkeletonSlot::getName() const{
        return name;
    }

    inline const std::vector<SkeletonPicture>& SkeletonSlot::getPresentations() const{
        return presRef;
    }

    inline size_t SkeletonSlot::getId() const{
        return id;
    }
}

#endif
