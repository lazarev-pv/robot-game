#include <UI/FieldContainers.h>
#include <zlib.h>
#include <ChibiEngine/Common/StringUtils.h>
#include "ChedPhysBox.h"

using namespace game;
using namespace glm;
using namespace std;

BoxContainer::BoxContainer(ChedLevel * level)
    :FieldsContainer({
        "box.name",
        "box.parent",
        "box.friction",
        "box.density",
        "box.sensor"
},"",level){}

void BoxContainer::updateObject() {
    obj->setName(fields[0]->getValue());
    //todo implement parent
    obj->setFriction(toFloat(fields[2]->getValue()));
    obj->setDensity(toFloat(fields[3]->getValue()));
    obj->setSensor(fields[4]->getValue()=="true");
}

void BoxContainer::updateFields() {
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(obj->getParent()?obj->getParent()->getName():"");
    fields[2]->setValue(toString(obj->getFriction()));
    fields[3]->setValue(toString(obj->getDensity()));
    fields[4]->setValue(obj->isSensor()?"true":"false");
}

PicContainer::PicContainer(ChedLevel *level)
        :FieldsContainer({
        "pic.name",
        "pic.parent",
        "pic.zOrder"
},"",level){}

void PicContainer::updateObject() {
    //todo implement  obj->name = fields[0]->getValue();
    //todo implement parent
    //todo implement obj->zOrder = toFloat(fields[2]->getValue());
    //todo implement level->reSort();
}

void PicContainer::updateFields() {
    fields[0]->setValue(obj->getSkeletonSlot()->getActive().getName());
   // fields[1]->setValue(obj->parent?obj->parent->getName():"");
    fields[2]->setValue(toString(obj->getSkeletonSlot()->getZOrder()));
}


BoneContainer::BoneContainer(ChedLevel *level)
        :FieldsContainer({
        "bone.name",
        "bone.parent",
        "bone.color",
        "bone.scale.x",
        "bone.scale.y",
        "bone.pos.x",
        "bone.pos.y",
        "bone.angle"
},"",level){}

void BoneContainer::updateObject() {
    level->reName(fields[0]->getValue());
    //todo implement parent
    obj->setColor(parseFromToString(fields[2]->getValue()));
}

void BoneContainer::updateFields() {
    fields[0]->setValue(obj->getName());
   // fields[1]->setValue(obj->getParent()!=nullptr?obj->getParent()->getName():"");
    fields[2]->setValue(obj->getColor().toString());
    fields[3]->setValue(toString(obj->getScale().x));
    fields[4]->setValue(toString(obj->getScale().x));
    fields[5]->setValue(toString(obj->getPosition().x));
    fields[6]->setValue(toString(obj->getPosition().y));
    fields[7]->setValue(toString(obj->getRotation()));
}
