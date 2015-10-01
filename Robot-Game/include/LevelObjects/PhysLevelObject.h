#ifndef _ROBOT_PHYSLEVELOBJECT_H_
#define _ROBOT_PHYSLEVELOBJECT_H_

#include <LevelObjects/LevelObject.h>
#include <vector>
#include <Box2D/Box2D.h>
#include <functional>

class GeometrySlot;
class b2ContactImpulse;
class b2Fixture;
namespace game {

    class DecorativeLevelObject;
    class PhysLevelObject : public LevelObject{
    public:
        PhysLevelObject(int uid, GeometrySlot *slot, Layer* layer);

        virtual void BeginContact(b2Fixture *contactedFixture, b2Fixture *other) {};
        virtual void EndContact(b2Fixture *contactedFixture, b2Fixture *other) {};
        virtual void PostSolve(b2Fixture *fixture, const b2ContactImpulse *impulse) {};
        virtual void hitted(float f){};

        glm::vec2 get2dPos();
        inline virtual glm::vec3 getPosition();
        inline b2Body *getBody() const;
        inline void addDestroyListener(const std::function<void(void)>& listener);

        virtual void requestDelete(bool instant);

    protected:
        PhysLevelObject(int uid, Layer* layer, int zOrder, const std::string& name, b2Body* body);
        ~PhysLevelObject();
        void setBody(b2Body* body);
    private:
        class DeleteCallback;
        friend class DeleteCallback;

        b2Body *body;
        std::vector<std::function<void()>> destroyListeners;
    };

    inline b2Body* PhysLevelObject::getBody() const{
        return body;
    }

    inline glm::vec3 PhysLevelObject::getPosition(){
        return glm::vec3(get2dPos(),0);
    }

    inline void PhysLevelObject::addDestroyListener(const std::function<void(void)>& listener){
        destroyListeners.push_back(listener);
    }

}

#endif
