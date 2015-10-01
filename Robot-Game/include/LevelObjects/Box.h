#ifndef __ROBOT_BOX_H_
#define __ROBOT_BOX_H_

#include <LevelObjects/PhysLevelObject.h>
#include <vector>
#include <Box2D/Box2D.h>
#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Render/Color.h>

class GeometrySlot;
class ERBoxType;
namespace game {
    class Box;
    class ObjectFixture {
    public:
        ObjectFixture(Box* parent, b2Fixture* fixture);
        ObjectFixture(Box* parent, ObjectFixture* other, b2Fixture* fixture);
        ObjectFixture(ObjectFixture&& other);
        ~ObjectFixture();

        inline glm::vec3 getFullOffset() const;
        inline glm::vec3 getFixtureOffset() const;
        inline b2Fixture* getFixture() const;
        b2PolygonShape copyShape() const;
        float getAngle() const;
        void draw(CameraCHandle camera,
                bool drawDebug,
                const Color& damageColor,
                const Color& ambientLight) const;

        void setColor(const Color& color);
    private:
        ObjectFixture(const ObjectFixture& other) = delete;

        const Box* box;
        b2Fixture* fixture;
        glm::vec2 offset;
        float hw,hh;
        std::vector<DecorativeLevelObject *> presentations;
    };

    class DecorativeLevelObject;
    class Event;
    class Box : public PhysLevelObject{
    public:
        Box(int uid, GeometrySlot *slot, Layer* layer);
        virtual void draw(CameraCHandle camera, const Color& ambientLight);
        virtual void hitted(float f);
        void breakBody();

        virtual void setColor(const Color& color) override;

    protected:
        virtual ~Box();
    private:
        Box(int uid, ObjectFixture* fixture, b2BodyDef* bd, b2FixtureDef* fd,Layer* layer);
        void breakBodyImpl();

        void restoreColor();
        std::vector<ObjectFixture*> fixtures;
        float health;
        Color damageColor;
        EventHandle colorEvent{};
    };

    inline glm::vec3 ObjectFixture::getFullOffset() const{
        assert(box!=nullptr);
        const b2Vec2& pos = box->getBody()->GetPosition();
        return glm::vec3(pos.x+offset.x,pos.y+offset.y,0);
    }

    inline glm::vec3 ObjectFixture::getFixtureOffset() const{
        return glm::vec3(offset,0);
    }

    inline b2Fixture* ObjectFixture::getFixture() const{
        return fixture;
    }



}

#endif
