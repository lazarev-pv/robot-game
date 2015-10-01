#include <ChibiEngine/Common/MathUtils.h>

using namespace game;
using namespace glm;
using namespace std;

namespace game {
    bool contains(const glm::vec2 &pos, float hw, float hh, const glm::vec2 &pnt) {
        return pnt.x > pos.x-hw &&
                pnt.x < pos.x+hw &&
                pnt.y > pos.y-hh &&
                pnt.y < pos.y+hh;
    }

    float getAngle(const glm::vec2& direction){
        return (direction.x>0?-1.0f:1.0f)*glm::acos(direction.y);
    }
}