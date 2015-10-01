#ifndef __LIGHT_OBJECT_H_
#define __LIGHT_OBJECT_H_

#include <EditorLevelImpl/BoxAreaObject.h>
#include <glm/glm.hpp>
#include <ChibiEngine/Render/Primitives/BoxArea.h>
#include "BoxAreaObject.h"

class LightType;
namespace game{

    class Camera;
    class Layer;
    class EditorGeometryObject;
    class TexturedObject;
    class SpotLightObject : public BoxAreaObject {
    public:
        SpotLightObject(const glm::vec3& offset, const glm::vec2& size);
        SpotLightObject(const LightType* l);

        virtual ~SpotLightObject(){}
        virtual void draw(CameraCHandle camera);
        virtual void save2Layer(WorldLayers* layer) const;
        virtual OperableObject *clone();
        virtual void setSelected(bool isSelected);

        inline const Color& getColor() const;
        inline void setColor(const Color& color);
    private:
        TexturedObject* pres;
        Color color;
        float intensity;
    };

    inline const Color& SpotLightObject::getColor() const{
        return SpotLightObject::color;
    }
    inline void SpotLightObject::setColor(const Color& color){
        SpotLightObject::color = color;
    }
}

#endif