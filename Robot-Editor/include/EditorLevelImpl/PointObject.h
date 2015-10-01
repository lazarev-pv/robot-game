#ifndef __POINTOBJECT_H_
#define __POINTOBJECT_H_

#include <EditorLevelImpl/OperableObject.h>
#include <ChibiEngine/Render/Primitives/SpriteText.h>

class NamedPoint2D;

namespace game{
	class PointObject : public OperableObject{
	public:
        PointObject(const glm::vec3& pos,const std::string& name="",Layer* layer=nullptr);
        virtual ~PointObject();

        virtual void save2World(WorldType* world){};
        virtual void save2Layer(WorldLayers* layer) const{};

        virtual void Move(const glm::vec3& delta);
        virtual void grab(const glm::vec2& pnt);
        virtual void release();
        virtual void draw(CameraCHandle camera);
        virtual OperableObject* containsPoint(const glm::vec2& pnt);
        virtual glm::vec3 getPosition() const;
        virtual void setPosition(const glm::vec3& position);

        inline bool isGrabbed() const;
	private:
		glm::vec3 pos;
		bool grabbed;
	};

    inline bool PointObject::isGrabbed() const{
        return grabbed;
    }
}



#endif /* POINTOBJECT_H_ */
