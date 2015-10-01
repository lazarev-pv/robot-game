#include <EditorLevelImpl/EditorInputContainers.h>
#include <ChibiEngine/UserInterface/FieldContainer.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <ChibiEngine/Common/StringUtils.h>
#include <EditorLevel.h>

#include <EditorLevelImpl/FXObject.h>
#include <EditorLevelImpl/EditorGeometryObject.h>
#include <EditorLevelImpl/EditorGeomJoint.h>
#include <EditorLevelImpl/EditorPres2D.h>
#include <EditorLevelImpl/PointObject.h>
#include <EditorLevelImpl/EnemyObject.h>
#include <EditorLevelImpl/EventObject.h>
#include <EditorLevelImpl/LightObject.h>
#include <EditorLevelImpl/GeometryLineChain.h>
#include <EditorLevelImpl/DirLightObject.h>
#include <EditorLevelImpl/EditorPres3D.h>

#include <glm/glm.hpp>
using namespace game;
using namespace glm;
using namespace std;

Pres2DInfo::Pres2DInfo(EditorLevel* level)
        :FieldsContainer(
        {"pres2d.parent",
        "pres2d.pos.x","pres2d.pos.y","pres2d.pos.z",
        "pres2d.scale.x","pres2d.scale.y",
        "pres2d.angle",
        "pres2d.zOrder",
        "pres2d.spread",
        "pres2d.name",
        "pres2d.color"},"",level){
}

Pres2DInfo::~Pres2DInfo(){
}

void Pres2DInfo::updateObject(){
    obj->setPosition(vec3(
            toFloat(fields[1]->getValue()),
            toFloat(fields[2]->getValue()),
            toFloat(fields[3]->getValue())
    ));
    obj->setScale(vec2(
            toFloat(fields[4]->getValue()),
            toFloat(fields[5]->getValue())
    ));
    obj->setRotation(toFloat(fields[6]->getValue()));
    obj->setZOrder(toInt(fields[7]->getValue()));
    obj->setSpreadMode(toBool(fields[8]->getValue()));
    obj->setName(fields[9]->getValue());
    obj->setColor(parseFromToString(fields[10]->getValue()));
}

void Pres2DInfo::updateFields(){
    {
        string p = obj->getParent() != nullptr ?
                obj->getParent()->getName() :
                "";
        fields[0]->setValue(p);
    }
    vec3 p = obj->getPosition();
    fields[1]->setValue(toString(p.x));
    fields[2]->setValue(toString(p.y));
    fields[3]->setValue(toString(p.z));
    vec2 s = obj->getScale();
    fields[4]->setValue(toString(s.x));
    fields[5]->setValue(toString(s.y));

    fields[6]->setValue(toString(obj->getRotation()));
    fields[7]->setValue(toString(obj->getZOrder()));
    fields[8]->setValue(toString(obj->isSpreadMode()));
    fields[9]->setValue(obj->getName());
    fields[10]->setValue(obj->getColor().toString());
}

Pres3DInfo::Pres3DInfo(EditorLevel* level)
        :FieldsContainer({"pres3d.parent",
        "pres3d.pos.x","pres3d.pos.y","pres3d.pos.z",
        "pres3d.tex.x","pres3d.tex.y","pres3d.color"},"",level){
}

Pres3DInfo::~Pres3DInfo(){}

void Pres3DInfo::updateObject(){
    obj->setPosition(vec3(
            toFloat(fields[1]->getValue()),
            toFloat(fields[2]->getValue()),
            toFloat(fields[3]->getValue())
    ));
    obj->setTexels(vec2(
            toFloat(fields[4]->getValue()),
            toFloat(fields[5]->getValue())
    ));
    obj->setColor(parseFromToString(fields[6]->getValue()));
}

void Pres3DInfo::updateFields(){
    {
        string p = obj->getParent() != nullptr ?
                obj->getParent()->getName() :
                "";
        fields[0]->setValue(p);
    }
    vec3 p = obj->getPosition();
    fields[1]->setValue(toString(p.x));
    fields[2]->setValue(toString(p.y));
    fields[3]->setValue(toString(p.z));
    vec2 s = obj->getTexels();
    fields[4]->setValue(toString(s.x));
    fields[5]->setValue(toString(s.y));
    fields[6]->setValue(obj->getColor().toString());
}

GeomChainInfo::GeomChainInfo(EditorLevel* level)
        :FieldsContainer({"geomChain.name",
        "geomChain.friction"},"",level){
}

GeomChainInfo::~GeomChainInfo(){}

void GeomChainInfo::updateObject(){
    obj->setName(fields[0]->getValue());
    obj->setFriction(toFloat(fields[1]->getValue()));
}

void GeomChainInfo::updateFields(){
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(toString(obj->getFriction()));
}

///////////////////////////////////////////////////////

GeomObjectInfo::GeomObjectInfo(EditorLevel* level)
        :FieldsContainer({
        "geomObjectParent.name",
        "geomObjectParent.type"},"",level){
}

GeomObjectInfo::~GeomObjectInfo(){}

void GeomObjectInfo::updateObject(){
    obj->setName(fields[0]->getValue());
    obj->setPhysType(fields[1]->getValue());
}

void GeomObjectInfo::updateFields(){
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(obj->getPhysType());
}
//////////////////////////////////////////////

GeomFixtureInfo::GeomFixtureInfo(EditorLevel* level)
        :FieldsContainer({
        "geomFixture.density",
        "geomFixture.friction",
        "geomFixture.category",
        "geomFixture.sensor"
        },"",level){
}

GeomFixtureInfo::~GeomFixtureInfo(){}

void GeomFixtureInfo::updateObject(){
    obj->setDensity(toFloat(fields[0]->getValue()));
    obj->setFriction(toFloat(fields[1]->getValue()));
    obj->setCategory(fields[2]->getValue());
    obj->setSensor(toBool(fields[3]->getValue()));
}

void GeomFixtureInfo::updateFields(){
    fields[0]->setValue(toString(obj->getDensity()));
    fields[1]->setValue(toString(obj->getFriction()));
    fields[2]->setValue(obj->getCategory());
    fields[3]->setValue(toString(obj->isSensor()));
}

///////////////////////////////////////////////////////

GeomJointInfo::GeomJointInfo(EditorLevel* level)
        :FieldsContainer({
        "geomJoint.obj1Name",
        "geomJoint.obj2Name",
        "geomJoint.frequence",
        "geomJoint.damp",
        "geomJoint.name"},"",level){
}

GeomJointInfo::~GeomJointInfo(){}

void GeomJointInfo::updateObject(){
    obj->setObj1(fields[0]->getValue());
    obj->setObj2(fields[1]->getValue());
    obj->setFrequence(toFloat(fields[2]->getValue()));
    obj->setDamp(toFloat(fields[3]->getValue()));
    obj->setName(fields[4]->getValue());
}

void GeomJointInfo::updateFields(){
    fields[0]->setValue(obj->getObj1Name());
    fields[1]->setValue(obj->getObj2Name());
    fields[2]->setValue(toString(obj->getFrequence()));
    fields[3]->setValue(toString(obj->getDamp()));
    fields[4]->setValue(obj->getName());
}

////////////////////////////////////////////////


SpotLightInfo::SpotLightInfo(EditorLevel* level)
:FieldsContainer({"spotLight.name",
        "spotLight.color",
        "spotLight.enabled"},"",level){

}
SpotLightInfo::~SpotLightInfo(){}

void SpotLightInfo::updateObject(){
    obj->setName(fields[0]->getValue());
    obj->setColor(parseFromToString(fields[1]->getValue()));
    obj->setEnable(toBool(fields[2]->getValue()));
}
void SpotLightInfo::updateFields(){
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(obj->getColor().toString());
    fields[2]->setValue(toString(obj->isEnable()));
}

DirLightInfo::DirLightInfo(EditorLevel* level)
        :FieldsContainer({"dirLight.name",
        "dirLight.color",
        "dirLight.enabled"},"",level){
}

DirLightInfo::~DirLightInfo(){}

void DirLightInfo::updateObject(){
    obj->setName(fields[0]->getValue());
    obj->setColor(parseFromToString(fields[1]->getValue()));
    obj->setEnable(toBool(fields[2]->getValue()));
}

void DirLightInfo::updateFields(){
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(obj->getColor().toString());
    fields[2]->setValue(toString(obj->isEnable()));
}

FxObjectInfo::FxObjectInfo(EditorLevel* level)
        :FieldsContainer({"fxObject.name",
        "fxObject.color",
        "fxObject.zOrder"},"",level){}

FxObjectInfo::~FxObjectInfo(){}
void FxObjectInfo::updateObject(){
    obj->setName(fields[0]->getValue());
    obj->setColor(parseFromToString(fields[1]->getValue()));
    obj->setZOrder(toInt(fields[2]->getValue()));
}
void FxObjectInfo::updateFields(){
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(obj->getColor().toString());
    fields[2]->setValue(to_string(obj->getZOrder()));
}

EventInfo::EventInfo(EditorLevel* level)
    : FieldsContainer({
        "event.name",
        "event.onEnter",
        "event.onLeave",
        "event.infinite"
    },"",level){}

EventInfo::~EventInfo(){}

void EventInfo::updateObject(){
    obj->setName(fields[0]->getValue());
    obj->setOnEnter(fields[1]->getValue());
    obj->setOnLeave(fields[2]->getValue());
    obj->setInfinite(fields[3]->getValue()=="true");
}

void EventInfo::updateFields(){
    fields[0]->setValue(obj->getName());
    fields[1]->setValue(obj->getOnEnter());
    fields[2]->setValue(obj->getOnLeave());
    fields[3]->setValue(toString(obj->isInfinite()));

}

EnemyInfo::EnemyInfo(EditorLevel* level)
        : FieldsContainer({"enemy.name"},"",level){}
EnemyInfo::~EnemyInfo(){}
void EnemyInfo::updateObject(){
    obj->setName(fields[0]->getValue());
}
void EnemyInfo::updateFields(){
    fields[0]->setValue(obj->getName());
}

PointInfo::PointInfo(EditorLevel* level)
        : FieldsContainer({"point.name"},"",level){}

PointInfo::~PointInfo(){}

void PointInfo::updateObject(){
    obj->setName(fields[0]->getValue());
}

void PointInfo::updateFields(){
    fields[0]->setValue(obj->getName());
}
