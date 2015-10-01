#include <EditorLevelImpl/Commands/ScaleCommand.h>
#include <sstream>

using namespace glm;
using namespace game;

ScaleCommand::ScaleCommand(const glm::vec2& sc,EditorPres2D* obj):
		EditorCommand(TYPE),
		obj(obj),
		scale(sc){}

void ScaleCommand::execute(){
	obj->setScale(obj->getScale()+scale);
}

void ScaleCommand::undo(){
	obj->setScale(obj->getScale()-scale);
}
bool ScaleCommand::merge(EditorCommand* other){
	if(other->getType()!=ScaleCommand::TYPE)
		return false;
	ScaleCommand* otherMove = static_cast<ScaleCommand*>(other);
	if(this->obj!=otherMove->obj)
		return false;
	this->scale+=otherMove->scale;
	return true;
}


std::string ScaleCommand::toString(){
	std::ostringstream ss;
	ss<<"Add Scale ["<<scale.x<<","<<scale.y<<"]";
	return ss.str();
}

