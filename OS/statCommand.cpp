#include <iostream>
#include <vector>
#include <string>
#include "statCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
statCommand::statCommand()
{
	//
}

// Деструктор
statCommand::~statCommand()
{
	//
}

int statCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	vfs->printStat();

	return OSSUCCESS;
}