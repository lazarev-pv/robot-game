#ifndef __EDITORLEVELOBJECT_H_
#define __EDITORLEVELOBJECT_H_

#include <ChibiEngine/Render/Primitives/BoxArea.h>
#include <EditorLevelImpl/PointObject.h>
#include <vector>
#include <unordered_map>
#include "BoxAreaObject.h"
#include <ChibiEngine/Physics/CategoryReference.h>

class ERBoxType;
class GeometrySlot;
namespace game{

    class EditorFixtureObject;
    class EditorGeometryObject : public PointObject{
    public:

        EditorGeometryObject(glm::vec3 pos,
                std::string name="",Layer* layer=nullptr,
                int physType = 2);
        EditorGeometryObject(GeometrySlot* slot, Layer* layer);
        virtual ~EditorGeometryObject();

        //todo implement
        virtual OperableObject* clone(){return nullptr;};

        virtual void Move(const glm::vec3& delta);
        virtual void setPosition(const glm::vec3& position);
        virtual void draw(CameraCHandle camera);
        virtual OperableObject* containsPoint(const glm::vec2& pnt);
        virtual void save2Layer(WorldLayers* layer) const;
        virtual void setSelected(bool isSelected);

        void setPhysType(const std::string& name);
        std::string& getPhysType();
        void add(EditorFixtureObject* fixture);
    private:
        static std::unordered_map<std::string, int> STR2INT_REFERENCE;
        static std::unordered_map<int, std::string> INT2STR_REFERENCE;

        std::vector<EditorFixtureObject*> elements;
        int physType;
    };

    class Layer;
    class EditorPres2D;
    enum class ObjectType{WATER, GROUND, LASER};
    class EditorFixtureObject : public BoxAreaObject {
    public:

        EditorFixtureObject(glm::vec3 pos,
                EditorGeometryObject* parent,
                Layer* layer,
                glm::vec2 size=glm::vec2(1,1),
                float friction = 1.0, float density = 1.0,
                const std::string& category = CategoryReference::PHYS,
                bool sensor = false);

        EditorFixtureObject(ERBoxType* slot,EditorGeometryObject* parent, Layer* layer);
        ~EditorFixtureObject();

        void addPresentation(EditorPres2D* pres);
        void removePresentation(EditorPres2D* pres);

        inline float getDensity() const;
        inline float getFriction() const;
        inline bool isSensor() const;
        inline const std::string& getCategory() const;
        inline void setDensity(float d);
        inline void setFriction(float f);
        inline void setCategory(const std::string& name);
        inline void setSensor(bool v);
        inline EditorGeometryObject* getParent();

        virtual void forceMove(const glm::vec3& delta);
        virtual void draw(CameraCHandle camera);
        virtual OperableObject* containsPoint(const glm::vec2& pnt);
        virtual void Move(const glm::vec3& delta);
        virtual void setPosition(const glm::vec3& position);
        virtual OperableObject* clone();
        virtual void setSelected(bool isSelected);

        ERBoxType* save() const;
    private:
        float density,friction;
        std::string category;
        bool sensor;
        EditorGeometryObject* parent;
        std::vector<EditorPres2D*> presentations;
    };

    EditorGeometryObject *EditorFixtureObject::getParent(){
        return parent;
    }

    inline float EditorFixtureObject::getDensity() const{
        return EditorFixtureObject::density;
    }
    inline float EditorFixtureObject::getFriction() const{
        return EditorFixtureObject::friction;
    }
    inline void EditorFixtureObject::setDensity(float d){
        EditorFixtureObject::density = d;
    }
    inline void EditorFixtureObject::setFriction(float f){
        EditorFixtureObject::friction = f;
    }
    inline void EditorFixtureObject::setCategory(const std::string& name){
        EditorFixtureObject::category = name;
    }

    inline const std::string& EditorFixtureObject::getCategory() const{
        return EditorFixtureObject::category;
    }

    inline void EditorFixtureObject::setSensor(bool v){
        EditorFixtureObject::sensor = v;
    }

    inline bool EditorFixtureObject::isSensor() const{
        return EditorFixtureObject::sensor;
    }
}



#endif
