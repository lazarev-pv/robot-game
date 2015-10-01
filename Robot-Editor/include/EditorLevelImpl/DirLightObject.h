#ifndef __DIR_LIGHT_OBJECT_H_
#define __DIR_LIGHT_OBJECT_H_

#include <EditorLevelImpl/LightObject.h>
#include <glm/glm.hpp>

class LightType;
class WorldType;
namespace game{

    class Camera;
    class Layer;
    class EditorLevel;
    class LanEditorLevelObject;
    class ArbitrarySprite;
    class OperableObject;
    class DirLightObject : public OperableObject {
    public:
        DirLightObject(const glm::vec3& offset, const glm::vec2& size);
        DirLightObject(const LightType* l);
        virtual ~DirLightObject();

        virtual void Move(const glm::vec3 &delta);
        virtual void grab(const glm::vec2 &pnt);
        virtual void release();
        virtual void draw(CameraCHandle camera);
        virtual OperableObject *containsPoint(const glm::vec2 &pnt);
        virtual void save2World(WorldType *world);
        virtual void save2Layer(WorldLayers* layer) const;
        virtual void viewContextMenu(EditorLevel *level, const glm::vec2 &pnt);
        virtual OperableObject *clone();
        virtual void setPosition(const glm::vec3 &position);
        virtual glm::vec3 getPosition() const;
        virtual void setSelected(bool isSelected);

        inline const Color& getColor() const;
        inline void setColor(const Color& color);
    private:
        int moveIndex;
        Color color;
        ArbitrarySprite* pres;
        glm::vec3 pnts[4];
    };

    inline const Color& DirLightObject::getColor() const{
        return DirLightObject::color;
    }
    inline void DirLightObject::setColor(const Color& color){
        DirLightObject::color = color;
    }
}

#endif