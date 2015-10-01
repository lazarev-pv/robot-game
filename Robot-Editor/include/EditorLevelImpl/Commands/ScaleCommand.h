#ifndef __SCALE_COMMAND_
#define __SCALE_COMMAND_

#include "EditorCommand.h"
#include "../EditorPres2D.h"

namespace game{

class ScaleCommand : public EditorCommand{
public:
	static const int TYPE = 1;

	ScaleCommand(const glm::vec2& scale, EditorPres2D* obj);
	virtual ~ScaleCommand(){};

	virtual void execute();
	virtual void undo();
	virtual bool merge(EditorCommand* other);

	virtual std::string toString();
private:
	EditorPres2D* obj;
	glm::vec2 scale;
};


}

#endif
