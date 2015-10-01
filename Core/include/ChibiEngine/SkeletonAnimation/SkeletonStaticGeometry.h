#ifndef __SOURCE_SKELETON_ANIMATION_PHYS_BOX_
#define __SOURCE_SKELETON_ANIMATION_PHYS_BOX_

#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/Handles.h>

class CharBoxType;
namespace game {

    class SkeletonStaticGeometry {
    public:
        SkeletonStaticGeometry(CharBoxType *data);

        void draw(const glm::vec3& pos, CameraCHandle camera) const;
        std::string* getName();
    private:
        glm::vec3 pos;
        float hw, hh;
        glm::vec4 color;
        std::string name;
    };

}

#endif