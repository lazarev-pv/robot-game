/*
 * DrawableObject.h
 *
 *  Created on: May 9, 2014
 *      Author: paul
 */

#ifndef __DRAWABLEOBJECT_H_
#define __DRAWABLEOBJECT_H_

#include <glm/glm.hpp>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Platform/Platform.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Common/MatrixBuilder.h>

namespace game {

	class DrawableObject : private UniqueClass {
	public:
		//Создает закрашеный объект без текстуры
		DrawableObject(const glm::vec2& size, const std::string& name);
		virtual ~DrawableObject() = default;

		float getHW() const;
		float getHH() const;
		inline glm::vec2 getSize() const;
        const std::string &getName() const;

		virtual void draw(const glm::mat4& transform, const Color& ambient=Color::White) const = 0;

		const ShaderValues& getShaderValue() const;

	protected:
        ShaderValues sv;
    private:

        glm::vec2 size;
        std::string name;
	};

	inline glm::vec2 DrawableObject::getSize() const{
		return glm::vec2(getHW(),getHH());
	}

}


#endif /* DRAWABLEOBJECT_H_ */
