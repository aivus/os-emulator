#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "addUserCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"

using namespace std;

// Конструктор
addUserCommand::addUserCommand()
{
	//
}

// Деструктор
addUserCommand::~addUserCommand()
{
	//
}

int addUserCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	cout << "Логин: ";
	vector<string> login = Console::showPromt();
	cout << "Пароль: ";
	string password = Console::enterPassword();

	switch(int uid = vfs->addUser(login[0].c_str(), password.c_str()))
	{
		case -1:
			cerr << "Пустой логин или пароль!" << endl;
			break;
		case -2:
			cerr << "Только администратор может создавать пользователей!" << endl;
			break;
		case -3:
			cerr << "Пользователь с таким именем уже существует!" << endl;
			break;
		case -4:
			cerr << "Слишком длинный логин! Максимальная длинна " << VFS_USER_LOGIN_MAX_LEN << endl;
			break;
		case -5:
			cerr << "Слишком длинный пароль! Максимальная длинна " << VFS_USER_PASS_MAX_LEN << endl;
			break;
		case -6:
			cerr << "Достигнуто максимальное число пользователей!" << endl;
			break;
		default:
			cout << "Пользователь создан. UID: "<< uid << endl;
	}

	return OSSUCCESS;
}