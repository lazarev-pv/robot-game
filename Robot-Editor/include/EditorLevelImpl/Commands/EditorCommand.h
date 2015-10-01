/*
 * EditorCommand.h
 *
 *  Created on: Aug 5, 2014
 *      Author: paul
 */

#ifndef __EDITORCOMMAND_H_
#define __EDITORCOMMAND_H_

#include <string>
namespace game{

class EditorCommand{
public:
	virtual ~EditorCommand(){};

	virtual void execute() = 0;
	virtual void undo() = 0;
	/** @return true if merged */
	virtual bool merge(EditorCommand* other){ return false; };

	int getType(){
		return type;
	}

	virtual std::string toString() = 0;
protected:
	EditorCommand(int type):type(type){};

private:
	int type;
};


}
#endif /* EDITORCOMMAND_H_ */
