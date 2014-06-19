#include <vector>
#include <string>
#include <utility>
#include "listCommand.h"
#include "makeDirCommand.h"
#include "touchCommand.h"
#include "makePipeCommand.h"
#include "CommandExecutor.h"
#include "internalCommand.h"
#include "changeDirCommand.h"
#include "chownCommand.h"
#include "chmodCommand.h"
#include "whoCommand.h"
#include "pwdCommand.h"
#include "rmCommand.h"
#include "catCommand.h"
#include "copyCommand.h"
#include "renameCommand.h"
#include "statCommand.h"
#include "testCommand.h"
#include "sudoCommand.h"
#include "addUserCommand.h"
#include "removeUserCommand.h"
#include "retCodes.h"

#include "pipeCommand.h"
#include "namedPipeCommand.h"
#include "sharedMemCommand.h"

// Конструктор
CommandExecutor::CommandExecutor()
{
	_fillCommands();
}

// Деструктор
CommandExecutor::~CommandExecutor()
{
	_releaseCommands();
}

// Заполнить командами
void CommandExecutor::_fillCommands()
{
	commands.insert(std::pair<std::string, Command*> ("ls", new listCommand));
	commands.insert(std::pair<std::string, Command*> ("mkdir", new makeDirCommand));
	commands.insert(std::pair<std::string, Command*> ("touch", new touchCommand));
	commands.insert(std::pair<std::string, Command*> ("mknod", new makePipeCommand));
	commands.insert(std::pair<std::string, Command*> ("cd", new changeDirCommand));
	commands.insert(std::pair<std::string, Command*> ("pwd", new pwdCommand));
	commands.insert(std::pair<std::string, Command*> ("who", new whoCommand));
	commands.insert(std::pair<std::string, Command*> ("cat", new catCommand));
	commands.insert(std::pair<std::string, Command*> ("rm", new rmCommand));
	commands.insert(std::pair<std::string, Command*> ("cp", new copyCommand));
	commands.insert(std::pair<std::string, Command*> ("rename", new renameCommand));
	commands.insert(std::pair<std::string, Command*> ("chmod", new chmodCommand));
	commands.insert(std::pair<std::string, Command*> ("chown", new chownCommand));
	commands.insert(std::pair<std::string, Command*> ("sudo", new sudoCommand));
	commands.insert(std::pair<std::string, Command*> ("adduser", new addUserCommand));
	commands.insert(std::pair<std::string, Command*> ("rmuser", new removeUserCommand));
	commands.insert(std::pair<std::string, Command*> ("clear", new internalCommand));
	commands.insert(std::pair<std::string, Command*> ("help", new internalCommand));
	commands.insert(std::pair<std::string, Command*> ("shutdown", new internalCommand));
	commands.insert(std::pair<std::string, Command*> ("exit", new internalCommand));
	commands.insert(std::pair<std::string, Command*> ("test", new testCommand));
	commands.insert(std::pair<std::string, Command*> ("stat", new statCommand));

	// Команды межпроцессного взаимодействия
	commands.insert(std::pair<std::string, Command*> ("testPipe", new pipeCommand));
	commands.insert(std::pair<std::string, Command*> ("testNamedPipe", new namedPipeCommand));
	commands.insert(std::pair<std::string, Command*> ("testShm", new sharedMemCommand));

}

// Освободить память, занимаемую командами
void CommandExecutor::_releaseCommands()
{
	std::map<std::string, Command*>::iterator it = commands.begin(), end = commands.end(); 
	for(; it != end; ++it)
		delete it->second;
}

int CommandExecutor::execute(VFS *vfs, std::vector<std::string> command)
{
	if(commands.count(command[0]))
		return commands[command[0]]->action(vfs, command);
	else
		return OSERROR;
}