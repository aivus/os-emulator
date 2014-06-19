#include <iostream>
#include <vector>
#include <string>
#include "pwdCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

std::vector<std::string> pwdCommand::path;

// Конструктор
pwdCommand::pwdCommand()
{
	//
}

// Деструктор
pwdCommand::~pwdCommand()
{
	//
}

int pwdCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	std::vector<std::string>::iterator it = pwdCommand::path.begin(), end = pwdCommand::path.end();
	cout << "/" ;
	for (; it != end; ++it)
		cout << it->c_str() << "/";
	cout << endl;
	return OSSUCCESS;
}