/*
 * TexturedObject.h
 *
 *  Created on: May 9, 2014
 *      Author: paul
 */

#ifndef __NINES_LICE_SPRITE_H_
#define __NINES_LICE_SPRITE_H_

#include <ChibiEngine/Resource/Resources.h>
#include <ChibiEngine/Resource/Texture.h>
#include <ChibiEngine/Render/Sprites/DrawableObject.h>
#include <vector>
#include <string>

namespace game{
    class NineSliceSprite : private UniqueClass {
    public:
        //Создает объект с текстурой
        NineSliceSprite(SpriteInfo::CHandle data, float hw, float hh);
        void draw(const glm::vec3& pos, const std::string& frameName="") const;

        void setSize(const glm::vec2& size);
    private:
        void createIndexBuffer();
        void genVertsBuffer(float hw, float hh);
        void genVertsFrame(float hw, float hh, SpriteInfo::FrameCHandle frame, std::vector<GLfloat>& verts);

        int getFrameNum4Draw(const std::string& name) const;
        const SpriteInfo* info;
        ShaderValues sv;
        float hw,hh;
    };
}


#endif /* TEXTUREDOBJECT_H_ */
