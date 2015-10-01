#ifndef __ROBOT_INTERACTIVE_H_
#define __ROBOT_INTERACTIVE_H_

#include <LevelObjects/PhysLevelObject.h>
#include <string>
#include <Box2D/Box2D.h>

namespace game {

    class Interactive : public PhysLevelObject{
    public:
        Interactive(int uid, LevelObject *parent, const glm::vec2& size, const std::string& script);

        virtual void draw(CameraCHandle camera, const Color &ambientLight);
        virtual void BeginContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void EndContact(b2Fixture *contactedFixture, b2Fixture *other);

        void interact();

        void requestDelete(bool instant) override;
    private:
        void updatePhysics();

        bool active;
        LevelObject *parent;
        glm::vec2 size;
        std::string script;
        int procId;
    };

}

#endif
