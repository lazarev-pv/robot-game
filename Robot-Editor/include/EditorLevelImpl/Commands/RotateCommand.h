#ifndef __ROTATECOMMAND_H_
#define __ROTATECOMMAND_H_

#include "EditorCommand.h"
#include "../EditorPres2D.h"

namespace game{

class AddRotationCommand : public EditorCommand{
public:
	static const int TYPE = 3;

	AddRotationCommand(const float angle, EditorPres2D* obj);
	virtual ~AddRotationCommand(){};

	virtual void execute();
	virtual void undo();
	virtual bool merge(EditorCommand* other);

	virtual std::string toString();
private:
	EditorPres2D* obj;
	float angle;
};


}


#endif
