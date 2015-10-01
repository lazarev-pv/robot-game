#ifndef __WEAPON_ROBOT_H_
#define __WEAPON_ROBOT_H_

#include <CharacterReference.h>
#include <unordered_set>

class b2Body;
class b2Fixture;
namespace game{

    class PhysLevelObject;
    class Weapon{
    public:
        Weapon(const WeaponSettings* data,Character* parent);

        void beginContact(b2Fixture *contactedFixture, b2Fixture *other);
        void endContact(b2Fixture *contactedFixture, b2Fixture *other);

        void attack(bool turnOn);
    private:
        bool getFixtures(b2Fixture *contactedFixture, b2Fixture *other, b2Fixture** self, b2Fixture** target);

        bool attacking;
        const WeaponSettings* data;
        b2Body* sensor;
    };


}

#endif