#include <EditorLevelImpl/OperableObject.h>
#include <EditorLevelImpl/Layer.h>

using namespace game;
using namespace std;
using namespace glm;

OperableObject::OperableObject(int zOrder, std::string name,Layer* layer):
        zOrder(zOrder),selected(false),layer(layer),name(name){
}

void OperableObject::setSelected(bool isSelected){
	selected = isSelected;
}

void OperableObject::setZOrder(int zOrder){
    OperableObject::zOrder = zOrder;
    if(layer!=nullptr){
        layer->resort();
    }
}

void OperableObject::setName(const std::string& name){
    string old = OperableObject::name;
    OperableObject::name = name;
    if(layer!=nullptr){
        layer->rename(old, this);
    }

}
