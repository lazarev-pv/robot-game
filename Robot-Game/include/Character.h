#ifndef __CHARACTER_HEADER_
#define __CHARACTER_HEADER_

#include <LevelObjects/PhysLevelObject.h>
#include <vector>
#include <ChibiEngine/SkeletonAnimation/Skeleton.h>
#include <ChibiEngine/Physics/Physics.h>
#include <unordered_set>
#include <CharacterReference.h>
#include <Weapon.h>

class RobotConfigType;
class CharacterType;
class CharacterParserRoot;
namespace game {

    class CharacterSettings;
    class PhysicsSystem;
    class Skeleton;
    class Character : public PhysLevelObject {
    public:
        Character(const CharacterSettings &settings, glm::vec2 startPosition, const Category& category, Layer* layer, int uid);

        // Char control
        void attack(bool turnOn, unsigned slotNum);
        void move(bool forward);
        void stop();
        void jump();

        virtual void hitted(float health);

        //Getters
        inline bool isLookForward();
        inline Skeleton* getSkeleton();
        inline float getHealth() const;
        inline float getMaxHealth() const;
        inline void addSlot(Weapon* weapon);
        inline void setSlot(unsigned i, Weapon* weapon);

        // Processing
        virtual void BeginContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void EndContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void PostSolve(b2Fixture *fixture, const b2ContactImpulse *impulse);
        virtual void draw(CameraCHandle camera, const Color &ambientLight);

        virtual void requestDelete(bool instant) override;
    protected:
        virtual ~Character();

        void setReflected(bool flag);
    private:
        int updatePhysicsId;
        void updatePhysics();

        enum State {
            RUN_BACK, RUN_FORWARD, STOP, DEAD
        };
        bool isJumping;
        bool reflected;
        State state;

        CharacterSettings settings;
        int contactedFixtures;

        Skeleton *skeleton;
        // health
        float totalHealth;

        std::vector<Weapon*> weapons;
    };

    inline Skeleton* Character::getSkeleton(){
        return skeleton;
    }

    inline bool Character::isLookForward() {
        return !reflected;
    }

    inline float Character::getHealth() const{
        return totalHealth;
    }

    inline float Character::getMaxHealth() const{
        return settings.health;
    }

    inline void Character::addSlot(Weapon* weapon){
        weapons.push_back(weapon);
    }

    inline void Character::setSlot(unsigned i, Weapon* weapon){
        assert(weapons.size()>i);
        weapons[i]=weapon;
    }
}


#endif
