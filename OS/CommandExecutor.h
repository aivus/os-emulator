#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <string>
#include <map>
#include "Command.h"
#include "VFS.h"

class CommandExecutor
{
	std::map<std::string, Command*> commands;	// Map команд и классов, отвечающих за них
	void _fillCommands();						// «аполнить commands командами
	void _releaseCommands();					// ќсвободить commands от команд
public:
	CommandExecutor();
	~CommandExecutor();

	int execute(VFS *vfs, std::vector<std::string> command);
};

#endif