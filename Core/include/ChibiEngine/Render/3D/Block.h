#ifndef __3D_BLOCK_
#define __3D_BLOCK_

#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Common/ClassUtils.h>

namespace game{

    class Camera;
    class Block : private UniqueClass {
    public:
        Block(
                GLuint textureId,
                const glm::vec2& tex=glm::vec2(1.0f,1.0f));

        void draw(const glm::mat4 &mvp, const Color &ambient) const;
        void draw(const glm::vec3 pos, const glm::vec3& size, CameraCHandle cam, const Color& ambient) const;

        inline void setTexes(const glm::vec2& tex);
        inline const glm::vec2& getTexes() const;

    private:
        void generateIndex();
        void generateVerts(const glm::vec2& tex);
        glm::vec2 tex;
        ShaderValues sv;
    };

    inline const glm::vec2& Block::getTexes()const{
        return tex;
    }

    inline void Block::setTexes(const glm::vec2& tex){
        Block::tex = tex;
        generateVerts(tex);
    }
}

#endif