#include <EditorLevelImpl/GeometryLineChain.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <EditorLevel.h>

using namespace game;
using namespace std;
using namespace glm;

const static float WIDTH = 0.05;

GeometryLineChainElement::GeometryLineChainElement(const glm::vec3& pos, GeometryLineChain* parent, int index)
	:OperableObject(0),
	 pos(pos),
	 index(index),
	 grabbed(false),
	 need2Unbind(true),
	 chain(parent){}

GeometryLineChainElement::~GeometryLineChainElement(){
	if(need2Unbind){
		chain->deleteElement(index);
	}
}

void GeometryLineChainElement::Move(const glm::vec3& delta){
	pos+=delta;
	chain->moveElement(index,delta);
	fireEvent(EventSupportable::MOVE_EVENT);
}

void GeometryLineChainElement::grab(const glm::vec2& pnt){
	grabbed = true;
}

void GeometryLineChainElement::release(){
	grabbed = false;
}

glm::vec3 GeometryLineChainElement::getPosition() const{
	return pos;
}

void GeometryLineChainElement::setPosition(const glm::vec3& position){
	pos = position;
	chain->setElementPosition(index, pos);
}

void GeometryLineChainElement::draw(CameraCHandle camera){
	Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(getPosition(), vec2(WIDTH, WIDTH),camera),
            grabbed ? Color::Red : Color::White);
}

OperableObject* GeometryLineChainElement::containsPoint(const glm::vec2& pnt){
	return pnt.x > pos.x-WIDTH && pnt.x < pos.x+WIDTH && pnt.y > pos.y- WIDTH && pnt.y <pos.y+WIDTH?
			this:nullptr;
}

void GeometryLineChainElement::save2World(WorldType* ){

}

void GeometryLineChainElement::viewContextMenu(EditorLevel* ,const glm::vec2& ){
}

OperableObject* GeometryLineChainElement::clone(){
	GeometryLineChainElement* el = new GeometryLineChainElement(pos, chain);
	chain->add(el);
	el->Move(vec3(0.5,0,0));
	return el;
}

void GeometryLineChainElement::setSelected(bool isSelected){
    OperableObject::setSelected(isSelected);
    if(isSelected)
        EditorLevel::instance()->selectGeomChain(chain);
}