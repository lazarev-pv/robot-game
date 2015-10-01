#ifndef __EDITOR_LAYER_
#define __EDITOR_LAYER_

#include <vector>
#include <EditorLevelImpl/EditorPres2D.h>
#include <functional>
#include <unordered_map>
#include <ChibiEngine/Render/RenderHandles.h>

class WorldLayers;
class WorldType;
class JointType;
namespace game {

    class OperableObject;
    class FXObject;
    class EditorPres2D;
    class GeometryLineChain;
    class LightObject;
    class EditorPres3D;
    class EditorGeometryObject;
    class EditorGeomJoint;
    class Layer {
    public:
        Layer(const std::string& name);
        Layer(WorldLayers *iter);
        ~Layer();

        void draw();

        void save(WorldType* world);

        void add(OperableObject *fx);
        void add(SpotLightObject* light);
        void add(DirLightObject* light);

        void deleteObject(OperableObject *obj);

        OperableObject *find(const glm::vec3 &pntInObjPlane);
        void resort();
        void rename(const std::string& oldName, OperableObject* obj);

        inline bool isSelectable() const;
        inline void setSelectable(bool selectable);
        inline bool isDrawable() const;
        inline void setDrawable(bool drawable);
        inline void setZ(float z);
        inline float getZ() const;
        inline const std::string& getName() const;
        inline void setName(const std::string& name);

        OperableObject* findByName(const std::string& name);

    private:
        void onRename(const std::string& name);
        void onDraw(const std::string& name);
        OperableObject* add2Index(OperableObject* obj);

        bool selectable, drawable;
        std::vector<OperableObject* >lights;
        std::vector<OperableObject*> drawList;
        std::unordered_map<std::string,OperableObject*> indexedObjs;
        float z;
        std::string name;
        ScreenFrameId frameId, lightFrameId;
    };

    inline void Layer::setZ(float z){
        this->z = z;
    }

    inline float Layer::getZ() const{
        return z;
    }

    inline bool Layer::isSelectable() const {
        return selectable;
    }

    inline void Layer::setSelectable(bool selectable) {
        this->selectable = selectable;
    }

    inline bool Layer::isDrawable() const {
        return drawable;
    }

    inline void Layer::setDrawable(bool drawable) {
        this->drawable = drawable;
    }

    inline const std::string& Layer::getName() const {
        return name;
    }

    inline void Layer::setName(const std::string &name) {
        Layer::name = name;
    }

}


#endif
