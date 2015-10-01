#ifndef __ARBITRARY_SPRITE_H_
#define __ARBITRARY_SPRITE_H_

#include <string>
#include <ChibiEngine/Resource/Texture.h>
#include <glm/glm.hpp>
#include <vector>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Resource/Resources.h>

namespace game {

    class SpriteInfo;
    class Camera;
    class ArbitrarySprite : private UniqueClass {
    public:
        //Создает объект с текстурой
        ArbitrarySprite(glm::vec3 vecs[4], SpriteInfo::CHandle info);

        //Рисует изображения исходя из переданных координат
        void draw(const glm::mat4& transform, const Color& color=Color::White) const;
        void changeVecs(glm::vec3 vecs[4]);

    private:
        ShaderValues sv;
        glm::vec2 texes[4];
    };
}

#endif