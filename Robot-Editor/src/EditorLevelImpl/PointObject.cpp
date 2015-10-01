#include <EditorLevelImpl/PointObject.h>
#include <ChibiEngine/Render/Primitives/PrimitiveDrawer.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Common/MathUtils.h>

using namespace game;
using namespace std;
using namespace glm;

static const float WIDTH = 0.05;


PointObject::PointObject(const glm::vec3& pos,const std::string& name,Layer* layer)
	:OperableObject(100,name,layer),
     pos(pos),
     grabbed(false){}

PointObject::~PointObject(){}

void PointObject::Move(const glm::vec3& delta){
    pos+=delta;
	fireEvent(EventSupportable::MOVE_EVENT);
}

void PointObject::grab(const glm::vec2& pnt){
	grabbed = true;
}

void PointObject::release(){
	grabbed = false;
}

glm::vec3 PointObject::getPosition() const{
	return pos;
}

void PointObject::setPosition(const glm::vec3& position){
	pos = position;
}

void PointObject::draw(CameraCHandle camera){
	Game::getPrimitiveDrawer()->drawRectangleBorder(
            createMatrix(getPosition(), vec2(WIDTH, WIDTH),camera),
            Color::Red);
}

OperableObject* PointObject::containsPoint(const glm::vec2& pnt){
    return game::contains(vec2(pos.x,pos.y), WIDTH, WIDTH, pnt)?this:nullptr;
}

