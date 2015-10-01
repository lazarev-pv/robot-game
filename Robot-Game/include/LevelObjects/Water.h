#ifndef __WATER_HEADER__
#define __WATER_HEADER__

#include <LevelObjects/PhysLevelObject.h>
#include <set>

namespace game{

	class Water : public PhysLevelObject {
	public:

		Water(int uid, GeometrySlot* type,Layer* layer);

		virtual void BeginContact(b2Fixture* contactedFixture, b2Fixture* other);
		virtual void EndContact(b2Fixture* contactedFixture, b2Fixture* other);
    protected:
        virtual ~Water();
	private:
        void updatePhysics();

		std::set<b2Fixture*> contactedFixtures;
		int procId;
	};
}



#endif
