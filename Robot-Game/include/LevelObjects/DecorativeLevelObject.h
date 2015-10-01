/*
 * DecorativeLevelObject.h
 *
 *  Created on: Mar 15, 2014
 *      Author: paul
 */

#ifndef ROBOT_DECORATIVELEVELOBJECT_H_
#define ROBOT_DECORATIVELEVELOBJECT_H_

#include <LevelObjects/LevelObject.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <memory>
#include <ChibiEngine/Render/Color.h>

namespace game{

    class ObjectFixture;
	class TexturedObject;
	class PatternSprite;
	class DecorativeLevelObject : public LevelObject{
	public:
        DecorativeLevelObject(int uid, ArtType* data, float z,ObjectFixture* parent, Layer* layer);

        inline void setParent(ObjectFixture* parent);
        inline void setColor(const Color& color) override;
        void draw(CameraCHandle camera,const Color& ambientLight) override;

        glm::vec3 getPosition() override;
	private:
        glm::vec3 offset;
        glm::vec2 scale;
        float rotation;
        std::unique_ptr<PatternSprite> presentation;
        ObjectFixture* parent;
        Color color;
	};

    inline void DecorativeLevelObject::setParent(ObjectFixture* parent){
        DecorativeLevelObject::parent = parent;
    }

    inline void DecorativeLevelObject::setColor(const Color& color){
        DecorativeLevelObject::color = color;
    }
}

#endif
