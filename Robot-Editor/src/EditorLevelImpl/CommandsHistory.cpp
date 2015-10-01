#include <EditorLevelImpl/Commands/CommandsHistory.h>
#include <EditorLevelImpl/Commands/EditorCommand.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Log/LoggingSystem.h>

using namespace game;
using namespace std;


CommandsHistory::CommandsHistory(int len):
	 history(len, nullptr),
	 current(-1),
	 lastAdded(-1),
	 logger(Game::getLoggingSystem()->createLogger("CommandsHistory")){
}
CommandsHistory::~CommandsHistory(){
	deleteAll(history);
}

void CommandsHistory::add(EditorCommand* command){
	/* Если была добавлена команда значит команды, к которым можно
	* было вернутся через Redo теряются */
	while(lastAdded!=current){
		if(history[lastAdded]!=nullptr){
			delete history[lastAdded];
			history[lastAdded]=nullptr;
		}

		lastAdded=pred(lastAdded);
	}
	/* Чтобы не засорять историю схожими командами пытаемся слить новую с текущей*/
	if(current>0 && history[current]!=nullptr && history[current]->merge(command)){
		delete command;
		return;
	}

	current = next(current);
	if(history[current]!=nullptr){
		delete history[current];
	}
	history[current] = command;
	lastAdded=current;
}

int CommandsHistory::next(int val){
	return (val+1)%history.size();
}

int CommandsHistory::pred(int val){
	val--;
	if(val<0)
		val=history.size()-1;

	return val;
}

void CommandsHistory::execute(EditorCommand* command){
	command->execute();
	logger.trace("Adding " + command->toString());
	add(command);
}

void CommandsHistory::undo(){
	if(current<0 || history[current]==nullptr || pred(current)==lastAdded)
		return;

	logger.debug("Undo " + history[current]->toString());
	history[current]->undo();
	current=pred(current);
}

void CommandsHistory::redo(){
	if(current<0 || current==lastAdded)
		return;

	current=next(current);
	logger.debug("Redo " + history[current]->toString());
	history[current]->execute();
}

