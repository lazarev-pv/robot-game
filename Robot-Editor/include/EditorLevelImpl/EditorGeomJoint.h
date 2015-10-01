#include "OperableObject.h"
#include <EditorLevelImpl/NamedPointObject.h>

#ifndef __EDITOR_GEOM_JOINT_H_
#define __EDITOR_GEOM_JOINT_H_

class JointType;
namespace game{

    //todo это вообще ниразу не OperableObject
    class EditorGeomJoint : public OperableObject{
    public:
        EditorGeomJoint(const glm::vec3& pos,
                int zOrder=0, std::string name="",Layer* layer=nullptr);

        EditorGeomJoint(JointType* it, Layer* layer=nullptr);

        virtual ~EditorGeomJoint(){};


        virtual void Move(const glm::vec3& delta){};
        virtual void grab(const glm::vec2& pnt){};
        virtual void save2World(WorldType* world){};
        virtual void release(){};
        virtual OperableObject* clone(){ return nullptr;};
        virtual void setPosition(const glm::vec3& position){};
        virtual glm::vec3 getPosition() const{return glm::vec3(0,0,0);};

        virtual void draw(CameraCHandle camera);
        virtual OperableObject* containsPoint(const glm::vec2& pnt);
        virtual void save2Layer(WorldLayers* layer) const;
        virtual void setSelected(bool isSelected){};

        inline void setFrequence(float frequence);
        inline void setDamp(float damp);
        inline void setObj1(const std::string& obj1Name);
        inline void setObj2(const std::string& obj2Name);

        inline float getFrequence() const;
        inline float getDamp() const;
        inline const std::string& getObj1Name() const;
        inline const std::string& getObj2Name() const;

    private:
        class PointWrapper : public NamedPointObject {
        public:
            PointWrapper(const glm::vec3& pos,EditorGeomJoint* par);
            virtual ~PointWrapper();
            virtual void setSelected(bool isSelected);
        private:
            EditorGeomJoint* par;
        };

        PointWrapper obj1,obj2;
        float damp, frequence;
    };

    inline void EditorGeomJoint::setFrequence(float frequence){
        EditorGeomJoint::frequence = frequence;
    }
    inline void EditorGeomJoint::setDamp(float damp){
        EditorGeomJoint::damp = damp;
    }
    inline float EditorGeomJoint::getFrequence() const{
        return frequence;
    }
    inline float EditorGeomJoint::getDamp() const{
        return damp;
    }
    inline void EditorGeomJoint::setObj1(const std::string& obj1Name){
        obj1.setName(obj1Name);
    }
    inline void EditorGeomJoint::setObj2(const std::string& obj2Name){
        obj2.setName(obj2Name);
    }
    inline const std::string& EditorGeomJoint::getObj1Name() const{
        return obj1.getName();
    }
    inline const std::string& EditorGeomJoint::getObj2Name() const{
        return obj2.getName();
    }

}

#endif