/*
 * DecorativeLevelObject.h
 *
 *  Created on: Mar 15, 2014
 *      Author: paul
 */

#ifndef __ROBOT_OBJECT_3D_H_
#define __ROBOT_OBJECT_3D_H_

#include <LevelObjects/LevelObject.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/Color.h>

namespace game{

	class Block;
	class Object3D : public LevelObject{
	public:
        Object3D(int uid, Box3DType* data, Layer* layer);

		inline glm::vec3 getPosition() override;

		inline void setColor(const Color& color) override;
		void draw(CameraCHandle camera,const Color& ambientLight) override;

    protected:
        virtual ~Object3D();
	private:
		glm::vec3 pos;
        glm::vec3 size;
		Block* presentation;
		Color color;
	};

	inline glm::vec3 Object3D::getPosition() {
		return pos;
	}

	inline void Object3D::setColor(const Color& color) {
		Object3D::color = color;
	}
}

#endif
