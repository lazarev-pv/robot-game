#include <EditorLevelImpl/Commands/RotateCommand.h>
#include <sstream>
using namespace glm;
using namespace game;

AddRotationCommand::AddRotationCommand(const float pnt, EditorPres2D* obj):
		EditorCommand(TYPE),
		obj(obj),
		angle(pnt){}

void AddRotationCommand::execute(){
	obj->setRotation(obj->getRotation()+angle);
}
void AddRotationCommand::undo(){
	obj->setRotation(obj->getRotation()-angle);
}
bool AddRotationCommand::merge(EditorCommand* other){
	if(other->getType()!=AddRotationCommand::TYPE)
		return false;
	AddRotationCommand* otherMove = static_cast<AddRotationCommand*>(other);
	if(this->obj!=otherMove->obj)
		return false;
	this->angle+=otherMove->angle;
	return true;
}


std::string AddRotationCommand::toString(){
	std::ostringstream ss;
	ss<<"Add rotation ["<<angle<<"]";
	return ss.str();
}
