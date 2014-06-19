#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "sudoCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"

using namespace std;

// Конструктор
sudoCommand::sudoCommand()
{
	//
}

// Деструктор
sudoCommand::~sudoCommand()
{
	//
}

int sudoCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	cout << "Логин: ";
	vector<string> login = Console::showPromt();
	cout << "Пароль: ";
	string password = Console::enterPassword();

	switch(int uid = vfs->login(login[0].c_str(), password.c_str()))
	{
		case 0:
			cout << "Успешный вход в систему!" << endl;
			break;
		case -1:
			cerr << "Неверный логин или пароль!" << endl;
			break;
	}

	return OSSUCCESS;
}