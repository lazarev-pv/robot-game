/*
 * CommandsHistory.h
 *
 *  Created on: Aug 6, 2014
 *      Author: paul
 */

#ifndef __COMMANDSHISTORY_H_
#define __COMMANDSHISTORY_H_

#include <vector>
#include <ChibiEngine/Log/Logger.h>

namespace game{

class Logger;
class EditorCommand;
class CommandsHistory{
public:
	CommandsHistory(int len);
	~CommandsHistory();

	void execute(EditorCommand* command);
	void undo();
	void redo();
private:
	void add(EditorCommand* command);
	int next(int val);
	int pred(int val);

	std::vector<EditorCommand*> history;
	int current;
	int lastAdded;
	Logger logger;
};

}



#endif
