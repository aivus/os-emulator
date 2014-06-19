#include <iostream>
#include <vector>
#include <string>
#include "whoCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
whoCommand::whoCommand()
{
	//
}

// Деструктор
whoCommand::~whoCommand()
{
	//
}

int whoCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;
	cout << "ID текущего пользователя: " << vfs->getCurrentUserID() << endl;
	return OSSUCCESS;
}