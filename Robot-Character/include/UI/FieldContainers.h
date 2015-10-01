#ifndef __ROBOT_CHARACTER_UI_FIELD_CONTAINERS_
#define __ROBOT_CHARACTER_UI_FIELD_CONTAINERS_

#include <ChedLevel.h>
#include <ChibiEngine/UserInterface/FieldContainer.h>
#include <ChedBone.h>
#include <ChedSlot.h>
#include <ChedPhysBox.h>

namespace game{

    class BoneContainer : public FieldsContainer<ChedBone, ChedLevel>{
    public:
        BoneContainer(ChedLevel* level);
        void updateObject() override;
        void updateFields() override;
    };

    class PicContainer : public FieldsContainer<ChedSlot, ChedLevel>{
    public:
        PicContainer(ChedLevel* level);
        void updateObject() override;
        void updateFields() override;
    };

    class BoxContainer : public FieldsContainer<ChedPhysBox, ChedLevel>{
    public:
        BoxContainer(ChedLevel* level);
        void updateObject() override;
        void updateFields() override;
    };

}

#endif