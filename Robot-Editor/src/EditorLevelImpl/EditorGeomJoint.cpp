#include <EditorLevelImpl/EditorGeomJoint.h>
#include <EditorLevel.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/Common/ParserUtils.h>
#include <EditorLevelImpl/Layer.h>

using namespace game;
using namespace std;
using namespace glm;

EditorGeomJoint::PointWrapper::PointWrapper(const glm::vec3& pos,EditorGeomJoint* par)
        :NamedPointObject(pos,"",EditorConstants::GEOM_JOINT),
         par(par){}

EditorGeomJoint::PointWrapper::~PointWrapper(){
}

void EditorGeomJoint::PointWrapper::setSelected(bool isSelected) {
    EditorLevel::instance()->selectGeomJoint(par);
}

EditorGeomJoint::EditorGeomJoint(JointType* it, Layer* layer)
        :OperableObject(100,it->name,layer),
        obj1(parse(it->offset1),this),
        obj2(parse(it->offset2),this),
        damp(it->damp),frequence(it->frequence){

    setObj1(it->obj1Name);
    if(it->obj1Name.size()>0){
        OperableObject* obj = layer->findByName(it->obj1Name);
        if(obj!=nullptr){
            obj1.Move(obj->getPosition());
        }else{
            Game::getLogger().error("EditorGeomJoint %s not found",it->obj1Name.c_str());
        }
    }

    setObj2(it->obj2Name);
    if(it->obj2Name.size()>0){
        OperableObject* obj = layer->findByName(it->obj2Name);
        if(obj!=nullptr){
            obj2.Move(obj->getPosition());
        }else{
            Game::getLogger().error("EditorGeomJoint %s not found",it->obj2Name.c_str());
        }
    }
}

EditorGeomJoint::EditorGeomJoint(const glm::vec3& pos,
        int zOrder, std::string name,Layer* layer)
        :OperableObject(zOrder,name,layer),
         obj1(pos-vec3(0.5f,0.0f,0.0f),this),
         obj2(pos+vec3(0.5f,0.0f,0.0f),this),
         damp(1.0),frequence(1.0){
}


void EditorGeomJoint::draw(CameraCHandle camera){
    Game::getPrimitiveDrawer()->drawLine(obj1.getPosition(), obj2.getPosition(), Color::Green, camera);
    obj1.draw(camera);
    obj2.draw(camera);
}

OperableObject* EditorGeomJoint::containsPoint(const glm::vec2& pnt){
    if(!EditorLevel::instance()->isSelectable(EditorConstants::GEOM_JOINT)){
        return nullptr;
    }
    {
        OperableObject* o = obj1.containsPoint(pnt);
        if(o!=nullptr)
            return o;
    }{
        OperableObject* o = obj2.containsPoint(pnt);
        if(o!=nullptr)
            return o;
    }
    return nullptr;
}

void EditorGeomJoint::save2Layer(WorldLayers* layer) const{
    JointType* s = new JointType();
    s->obj1Name = getObj1Name();
    s->obj2Name = getObj2Name();
    s->name = getName();
    {
        OperableObject *obj = nullptr;
        if (getObj1Name().size() > 0) {
            obj = getLayer()->findByName(getObj1Name());
        }
        s->offset1 = (obj != nullptr)?
                create3D(obj1.getPosition() - obj->getPosition()):
                create3D(obj1.getPosition());
    }{
        OperableObject *obj = nullptr;
        if (getObj2Name().size() > 0) {
            obj = getLayer()->findByName(getObj2Name());
        }
        s->offset2 = (obj != nullptr)?
                create3D(obj2.getPosition() - obj->getPosition()):
                create3D(obj2.getPosition());
    }

    s->frequence = frequence;
    s->damp = damp;
    layer->geometry->join.push_back(s);
}



