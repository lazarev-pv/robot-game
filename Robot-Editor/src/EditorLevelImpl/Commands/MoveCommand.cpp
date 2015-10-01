#include <EditorLevelImpl/Commands/MoveCommand.h>
#include <sstream>
using namespace glm;
using namespace game;

MoveCommand::MoveCommand(const glm::vec3& pnt, OperableObject* obj):
		EditorCommand(TYPE),
		obj(obj),
		delta(pnt){}

void MoveCommand::execute(){
	obj->Move(delta);
}
void MoveCommand::undo(){
	obj->Move(-delta);
}
bool MoveCommand::merge(EditorCommand* other){
	if(other->getType()!=MoveCommand::TYPE)
		return false;
	MoveCommand* otherMove = static_cast<MoveCommand*>(other);
	if(this->obj!=otherMove->obj)
		return false;
	this->delta+=otherMove->delta;
	return true;
}


std::string MoveCommand::toString(){
	std::ostringstream ss;
	ss<<"move ["<<delta.x<<","<<delta.y<<","<<delta.z<<"]";
	return ss.str();
}
