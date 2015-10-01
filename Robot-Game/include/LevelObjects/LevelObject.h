#ifndef __ROBOT_LEVEL_OBJECT_H_
#define __ROBOT_LEVEL_OBJECT_H_

#include <ChibiEngine/Render/Camera.h>
#include <string>
#include <ChibiEngine/Render/Color.h>

namespace game {

    class Interactive;
    class Layer;
    class LevelObject {
    public:
        inline LevelObject(int uid, Layer* layer, int zOrder = 0, const std::string& name="");

        virtual void draw(CameraCHandle camera, const Color &ambientLight) = 0;

        inline virtual void setEnabled(bool val);
        inline virtual void setColor(const Color& color);
        virtual void requestDelete(bool instant);
        virtual glm::vec3 getPosition()=0;

        inline void setDrawDebug(bool val);
        void link2Interact(Interactive* interact);

        inline int getZOrder() const;
        inline int getUid() const;
        inline Layer* getLayer() const;
        inline const std::string& getName() const;

    protected:
        inline virtual ~LevelObject();
        inline bool isDrawDebug() const;
        inline bool isEnabled() const;
        Interactive* getInteract();
    private:
        Layer* layer;
        bool drawDebug;
        bool enabled;
        int zOrder;
        std::string name;
        Interactive* interact;
        int uid;

    };

    inline LevelObject::LevelObject(int uid, Layer* layer, int zOrder, const std::string& name) :
            layer(layer),
            drawDebug(false),
            enabled(true),
            zOrder(zOrder),
            name(name),
            interact(nullptr),
            uid(uid){
    }

    inline LevelObject::~LevelObject() {
    }

    inline int LevelObject::getZOrder() const {
        return zOrder;
    }

    inline void LevelObject::setDrawDebug(bool val) {
        drawDebug = val;
    }

    inline void LevelObject::setEnabled(bool val) {
        enabled = val;
    }

    inline const std::string& LevelObject::getName() const{
        return name;
    }

    inline Layer* LevelObject::getLayer() const{
        return layer;
    }


    inline bool LevelObject::isDrawDebug() const{
        return drawDebug;
    }

    inline bool LevelObject::isEnabled() const{
        return enabled;
    }

    inline int LevelObject::getUid() const{
        return uid;
    }

    inline void LevelObject::setColor(const Color& color){
    }
}
#endif
