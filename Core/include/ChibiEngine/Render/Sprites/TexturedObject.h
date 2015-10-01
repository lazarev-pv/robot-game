#ifndef __TEXTUREDOBJECT_H_
#define __TEXTUREDOBJECT_H_

#include <string>
#include <ChibiEngine/Resource/Texture.h>
#include <ChibiEngine/Render/Sprites/DrawableObject.h>
#include <ChibiEngine/Resource/Resources.h>

namespace game {

    class SpriteInfo;
    class Texture;
    class TexturedObject : public DrawableObject {
    public:
        TexturedObject(SpriteInfo::CHandle presentation);
        TexturedObject(TextureCHandle texture, float weight, float height);

        void draw(const glm::mat4& transform, const Color& ambient = Color::White) const override;

        TextureCHandle getTexture() const;

    protected:
        TexturedObject(float w, float h, TextureCHandle texture, const std::string &name);
    private:

        void generateIndex();
        void generateVertex(const glm::vec2& tl, const glm::vec2& br);

        std::string name;
        TextureCHandle texture;
    };

}


#endif
