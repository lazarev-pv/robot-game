/*
 * PrimitiveDrawer.h
 *
 *  Created on: May 9, 2014
 *      Author: paul
 */

#ifndef PRIMITIVEDRAWER_H_
#define PRIMITIVEDRAWER_H_

#include <glm/glm.hpp>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Game.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Common/MatrixBuilder.h>

namespace game{

    class Texture;
	class PrimitiveDrawer : private UniqueClass {
	public:
		PrimitiveDrawer();
		~PrimitiveDrawer();


        void drawTexture(const glm::mat4& transform, const Texture& texture, const std::string& shaderName) const;

        // Симметричные относительно центра примитивы
		void drawRectangleBorder(const glm::mat4& transform, const Color& color) const;
		void drawFilledRectangle(const glm::mat4& transform, const Color& color) const;
		void drawBorderedFilledRectangle(const glm::vec3 &pos,
                const glm::vec2& size,
                float borderWidth,
                const Color &backColor,
                const Color &borderColor,
                CameraCHandle camera=nullptr) const;

        // Примитивы симметричные относительно своей оси
        void drawFilledArrow(const glm::mat4& transform, const Color& color) const;

        void drawSimpleArrow(
				const glm::vec3 &beg,
				const glm::vec3 &end,
                const Color& color,
				CameraCHandle camera = nullptr) const;

        void drawLine(
                const glm::vec3 &beg,
                const glm::vec3 &end,
                const Color& color,
                CameraCHandle cam = nullptr) const;

        // Несиммитричные примитивы
		/**@param borders - left top right bottom */
		void drawPartialBorderedFilledRectangle(
				const glm::vec3 &pos,
                const glm::vec2& size,
				float borderWidth,
                const Color& backColor,
                const Color& borderColor,
				glm::bvec4 borders=glm::bvec4(true,true,true,true),
				CameraCHandle cam = nullptr) const;

		/** @param borders - left top right bottom */
		void drawPartialRectangleBorder(
				const glm::vec3 &pos,
                const glm::vec2& size,
				float borderWidth,
                const Color& borderColor,
                glm::bvec4 borders=glm::bvec4(true,true,true,true),
				CameraCHandle cam = nullptr) const;

	private:
        mutable ShaderValues sv;

        void generateArrow();

		GLuint arrowVertsBuffer;
		GLuint barVertsBuffer;
		GLuint trianlesIndexBuffer;
		GLuint lineIndexBuffer;

        GLuint simpleArrowIndexes;
        GLuint simpleArrowBuffers;

        GLuint drawTextVertsBuffer;
	};

}



#endif /* PRIMITIVEDRAWER_H_ */
