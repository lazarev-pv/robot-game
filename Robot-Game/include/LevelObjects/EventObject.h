/*
 * DecorativeLevelObject.h
 *
 *  Created on: Mar 15, 2014
 *      Author: paul
 */

#ifndef __ROBOT_EVENT_OBJET_H_
#define __ROBOT_EVENT_OBJET_H_

#include <LevelObjects/PhysLevelObject.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <string>

namespace game {

    class Character;
    class EventObject : public PhysLevelObject {
    public:
        EventObject(int uid, EventType *data, Layer* layer);

        virtual void draw(CameraCHandle camera, const Color &ambientLight);
        virtual void BeginContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void EndContact(b2Fixture *contactedFixture, b2Fixture *other);
        virtual void setEnabled(bool val);
    protected:
        virtual ~EventObject(){};
    private:
        bool infinite;
        std::string onEnter, onLeave;
        Character* ch;
        float hw, hh;
        int fixturesCount;
    };
}

#endif
