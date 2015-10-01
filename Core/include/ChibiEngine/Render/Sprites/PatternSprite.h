/*
 * DrawableObject.h
 *
 *  Created on: May 9, 2014
 *      Author: paul
 */

#ifndef __PATTERN_SPRITE_H_
#define __PATTERN_SPRITE_H_

#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Resource/Texture.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Resource/Resources.h>


namespace game {

    class SpriteInfo;
	class PatternSprite {
	public:
		PatternSprite(SpriteInfo::CHandle presentation, float xrepeats=1.0, float yrepeats=1.0);

		PatternSprite(PatternSprite const&) = delete;
		void operator=(PatternSprite const&) = delete;

        inline glm::vec2 getSize() const;
		float getHW() const{
			return targetHW;
		}

		float getHH() const{
			return targetHH;
		}

		float getXRepeats() const{
			return targetHW/texHW;
		}

		float getYRepeats() const{
			return targetHH/texHH;
		}

		const std::string& getName(){
			return name;
		}

		void setTextureWidth(float textureWidth);
		void setTextureHeight(float textureHeight);
		void setTargetWidth(float targetW);
		void setTargetHeight(float targetW);

		void spread(const glm::vec2& targetSizeDelta);

		//Рисует изображения исходя из переданных координат
        void draw(const glm::mat4& mat, const Color& ambient) const;
	private:
		void reGenBuffers();
		void genBuffers();

        ShaderValues sv;
		//Положение, ориентация, полуразмеры
		float texHW, texHH, targetHW, targetHH;

		glm::vec2 t1, t2;
		std::string name;

	};


    inline glm::vec2 PatternSprite::getSize() const{
        return glm::vec2(getHW(),getHH());
    }
}


#endif /* DRAWABLEOBJECT_H_ */
