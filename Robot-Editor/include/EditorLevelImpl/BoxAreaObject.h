#ifndef __BOX_AREA_OBJECT_H_
#define __BOX_AREA_OBJECT_H_

#include <EditorLevelImpl/OperableObject.h>
#include <ChibiEngine/Render/Primitives/BoxArea.h>

namespace game{

    class EditorGeometryObject;
    class PrimitiveDrawer;
    class Camera;
    class BoxAreaObject : public OperableObject {
    public:

        BoxAreaObject(const glm::vec3& pos, const glm::vec2& size, const std::string& classType,
                int zOrder = 0, const std::string& name="", Layer* layer=nullptr);
        virtual ~BoxAreaObject(){};

        virtual OperableObject* clone() = 0;

        virtual void save2World(WorldType* world){};
        virtual void save2Layer(WorldLayers* layer) const{};
        virtual void Move(const glm::vec3& delta);
        virtual void grab(const glm::vec2& pnt);
        virtual void release();
        virtual void draw(CameraCHandle camera);
        virtual OperableObject* containsPoint(const glm::vec2& pnt);
        virtual void setPosition(const glm::vec3& position);
        virtual glm::vec3 getPosition() const;

        glm::vec2 getSize() const;
        void setSize(const glm::vec2& size);
        void setRotation(float angle);
        float getRotation() const;

    protected:
        inline bool isBorderMoving() const;
    private:
        BoxArea area;
        std::string classType;
	};

    inline bool BoxAreaObject::isBorderMoving() const{
        return area.isBorderMoving();
    }
}




#endif /* OPERABLEOBJECT_H_ */
