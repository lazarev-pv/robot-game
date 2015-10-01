
#ifndef __PHYSICS_SYSTEM_H_
#define __PHYSICS_SYSTEM_H_

#include <glm/glm.hpp>
#include <vector>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/XMLParser/SettingsParser.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Physics/CategoryReference.h>
#include <Box2D/Box2D.h>
#include <ChibiEngine/Clock/ClockHandles.h>

class b2World;
class b2Body;
struct b2BodyDef;
struct b2JointDef;
class b2WheelJoint;
class BoxGeometryType;

namespace game {

    using PhysicsBody = b2Body;
    using PhysicsWorld = b2World;
    using PhysicsBodyDef = b2BodyDef;
    using JointDef = b2JointDef;
    using WheelJoint = b2WheelJoint;

    class GeometrySettings;


    class Contactable {
    public:
        virtual ~Contactable() = default;
        virtual void beginContact(b2Fixture *contactedFixture, b2Fixture *other) = 0;
        virtual void endContact(b2Fixture *contactedFixture, b2Fixture *other) = 0;
        virtual void postSolve(b2Fixture *fixture, const b2ContactImpulse *impulse) = 0;
    };

    class PhysicsSystem : private UniqueClass {
    public:
        PhysicsSystem(PhysicsType* settings, ClockHandle gameClock, EventSystem* eventSystem);
        ~PhysicsSystem();

        PhysicsBody* createGeometry(GeometrySlot *bgt);
        PhysicsBody* createCircularGeometry(float radius, float density, float friction);
        PhysicsBody* create(PhysicsBodyDef *def);
        WheelJoint*  createWheelJoint(JointDef *def);

        void setContactListener(b2ContactListener * listener);

        // Дополнительные воздействия (приложение силы, трансформация) - выполняется КАЖДЫЙ РАЗ до шага
        int addAdditionalProcessing(const std::function<void()>& action);
        void deleteAdditionalProcessing(int id);
        // Обработчики выполняющиеся после вычисления физики - выполняются ЕДИНОЖНДЫ
        inline void addPostStepAction(const std::function<void()>& callback);

        inline b2World* getWorld();
        inline void deleteObject(b2Joint* joint);
        inline void deleteObject(b2Body* body);
        inline const CategoryReference& getCategoryReference();
        inline const Logger& getLogger() const;
    private:

        void step();
        PhysicsBody* createBoxGeometry(GeometrySlot *bgt);
        PhysicsBody* createChainGeometry(GeometrySlot* bgt);

        int positionIters;
        int velocityIters;
        Logger logger;
        PhysicsWorld* world;

        std::map<int,std::function<void()>> additionalProcessing;
        std::vector<std::function<void()>> postStepActions;
        std::vector<b2Joint*> jointDeleteList;
        std::vector<b2Body*> bodyDeleteList;

        int eventInterval;
        float physicsStepDimension;
        int lastId;
    };

    inline void PhysicsSystem::deleteObject(b2Joint* joint){
        jointDeleteList.push_back(joint);
    }

    inline void PhysicsSystem::deleteObject(b2Body* body){
        bodyDeleteList.push_back(body);
    }

    inline void PhysicsSystem::addPostStepAction(const std::function<void()>& callback){
        postStepActions.push_back(callback);
    }

    inline const Logger& PhysicsSystem::getLogger() const{
        return logger;
    }

    inline b2World* PhysicsSystem::getWorld(){
        return world;
    }
}
#endif
