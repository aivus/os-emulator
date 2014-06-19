#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <string>
#include <map>
#include "Command.h"
#include "VFS.h"

class CommandExecutor
{
	std::map<std::string, Command*> commands;	// Map ������ � �������, ���������� �� ���
	void _fillCommands();						// ��������� commands ���������
	void _releaseCommands();					// ���������� commands �� ������
public:
	CommandExecutor();
	~CommandExecutor();

	int execute(VFS *vfs, std::vector<std::string> command);
};

#endif