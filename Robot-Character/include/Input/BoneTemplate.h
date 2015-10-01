#ifndef __ROBOT_CHARACTER_INPUT_BONE_TEMPLATE_
#define __ROBOT_CHARACTER_INPUT_BONE_TEMPLATE_

#include <glm/glm.hpp>

namespace game{

    class TexturedObject;
    class BoneTemplate{
    public:
        BoneTemplate();
        void draw();

        inline void setBegin(const glm::vec2& begin);
        inline void setEnd(const glm::vec2& end);
    private:
        void recalc();
        glm::vec2 begin;
        glm::vec2 end;
        const TexturedObject* obj;

        float len;
        glm::vec2 pos;
        float rotation;
    };

    inline void BoneTemplate::setBegin(const glm::vec2& begin){
        BoneTemplate::begin = begin;
        recalc();
    }

    inline void BoneTemplate::setEnd(const glm::vec2& end){
        BoneTemplate::end = end;
        recalc();
    }

}

#endif