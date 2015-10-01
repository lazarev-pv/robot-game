#ifndef __OPERABLEOBJECT_H_
#define __OPERABLEOBJECT_H_

#include <ChibiEngine/Common/EventSupportable.h>
#include <glm/glm.hpp>
#include <string>
#include <ChibiEngine/Handles.h>

class WorldType;
class WorldLayers;
namespace game{

    class EditorLevel;
    class EditorGeometryObject;
    class PrimitiveDrawer;
    class Camera;
    class Layer;
    class OperableObject : public EventSupportableImpl {
    public:

        OperableObject(int zOrder=0, std::string name="",Layer* layer=nullptr);

        virtual ~OperableObject(){};
        inline int getZOrder() const;
        virtual void setZOrder(int zOrder);
        inline const std::string& getName() const;
        virtual void setName(const std::string& name);

        virtual void Move(const glm::vec3& delta) = 0;
        virtual void grab(const glm::vec2& pnt) = 0;
        virtual void release() = 0;
        virtual void draw(CameraCHandle camera) = 0;
        virtual OperableObject* containsPoint(const glm::vec2& pnt) = 0;
        virtual void save2World(WorldType* world) = 0;
        virtual void save2Layer(WorldLayers* layer) const = 0;
        virtual OperableObject* clone() = 0;
        virtual void setPosition(const glm::vec3& position) = 0;
        virtual void setSelected(bool isSelected);
        virtual glm::vec3 getPosition() const = 0;

        inline void setEnable(bool enable);
        inline bool isEnable() const;
    protected:
        inline bool isSelected() const;
        inline Layer* getLayer() const;
    private:
        inline void setLayer(Layer* layer);
        int zOrder;
        bool selected;
        Layer* layer;
        std::string name;
        bool enable = true;
	};

    inline int OperableObject::getZOrder() const{
        return OperableObject::zOrder;
    }

    inline const std::string& OperableObject::getName() const{
        return name;
    }

    inline bool OperableObject::isSelected() const{
        return selected;
    }

    inline void OperableObject::setLayer(Layer* layer){
        OperableObject::layer = layer;
    }

    inline Layer* OperableObject::getLayer() const{
        return OperableObject::layer;
    }

    inline bool OperableObject::isEnable() const{
        return enable;
    }

    inline void OperableObject::setEnable(bool enable){
        OperableObject::enable = enable;
    }
}




#endif /* OPERABLEOBJECT_H_ */
