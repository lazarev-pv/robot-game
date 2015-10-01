#ifndef __MOVE_COMMAND_
#define __MOVE_COMMAND_

#include <glm/glm.hpp>
#include "EditorCommand.h"
#include "../OperableObject.h"

namespace game{

class MoveCommand : public EditorCommand{
public:
	static const int TYPE = 0;

	MoveCommand(const glm::vec3& delta, OperableObject* obj);
	virtual ~MoveCommand(){};

	virtual void execute();
	virtual void undo();
	virtual bool merge(EditorCommand* other);

	virtual std::string toString();
private:
	OperableObject* obj;
	glm::vec3 delta;
};


}

#endif
