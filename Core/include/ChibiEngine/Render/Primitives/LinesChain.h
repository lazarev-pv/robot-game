/*
 * VisualVector.h
 *
 *  Created on: May 9, 2014
 *      Author: paul
 */

#ifndef __LINES_CHAIN_H_
#define __LINES_CHAIN_H_

#include <glm/glm.hpp>
#include <ChibiEngine/Render/Camera.h>
#include <ChibiEngine/Render/Shaders.h>
#include <ChibiEngine/Common/ClassUtils.h>
#include <vector>

namespace game {

	class LinesChain : private UniqueClass {
	public:
		LinesChain(const Color& color);
		LinesChain(const Color& color, const std::vector<glm::vec3>& points);
		void draw(CameraCHandle camera) const;
		void add(glm::vec3 point);
		void add(const std::vector<glm::vec3>& points);
		void move(int index, const glm::vec3& delta);
		void setPosition(int index, const glm::vec3& pos);
		void deleteElement(int index);

	private:
		void genBuffer();
        mutable ShaderValues sv;

		std::vector<GLfloat> points;
	};


}


#endif
