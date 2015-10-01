
#include <EditorLevelImpl/BoxAreaObject.h>
#include <ChibiEngine/Common/MathUtils.h>
#include <EditorLevel.h>

using namespace game;
using namespace std;
using namespace glm;

BoxAreaObject::BoxAreaObject(const glm::vec3& pos, const glm::vec2& size, const std::string& classType,
        int zOrder, const std::string& name, Layer* layer)
:OperableObject(zOrder,name,layer),area(pos,size,0.1),classType(classType){
}

void BoxAreaObject::Move(const glm::vec3& delta){
	area.drag(vec2(delta.x,delta.y));
	fireEvent("");
}

void BoxAreaObject::grab(const glm::vec2& pnt){
	area.grab(pnt);
}

void BoxAreaObject::release(){
	area.release();
}

void BoxAreaObject::draw(CameraCHandle camera){
    area.draw(camera);
}

OperableObject* BoxAreaObject::containsPoint(const glm::vec2& pnt){
    return EditorLevel::instance()->isSelectable(classType) && (area.contains(pnt))? this: nullptr;
}

glm::vec3 BoxAreaObject::getPosition() const{
	return area.getPosition();
}

void BoxAreaObject::setPosition(const glm::vec3& position){
	area.setPosition(position);
}

glm::vec2 BoxAreaObject::getSize() const{
    return vec2(area.getHWidth(),area.getHHeight());
}

float BoxAreaObject::getRotation() const{
    return area.getRotation();
}

void BoxAreaObject::setSize(const glm::vec2& size){
    area.setSize(size);
}

void BoxAreaObject::setRotation(float angle){
    area.setRotation(angle);
}