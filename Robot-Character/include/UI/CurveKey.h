#ifndef __ROBOT_CHARACTER_CURVE_KEY_H_
#define __ROBOT_CHARACTER_CURVE_KEY_H_

#include <glm/glm.hpp>

namespace game{

    class CurveKey{
    public:
        void draw(const glm::vec2& begin, const glm::vec2& end, bool highlighted) const;
    };

}


#endif

