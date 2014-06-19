#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "removeUserCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"

using namespace std;

// Конструктор
removeUserCommand::removeUserCommand()
{
	//
}

// Деструктор
removeUserCommand::~removeUserCommand()
{
	//
}

int removeUserCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size() > 1)
	{
		switch(vfs->removeUser(data[1].c_str()))
		{
			case 0:
				cout << "Пользователь удален!" << endl;
				break;
			case -1:
				cerr << "Пустой логин!" << endl;
				break;
			case -2:
				cerr << "Только администратор (root) может удалять пользователей!" << endl;
				break;
			case -3:
				cerr << "Пользователь с таким логином не найден!" << endl;
				break;
			case -4:
				cerr << "Нельзя удалить администратора (root)!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Неверное количество параметров!" << endl;
	}

	return OSSUCCESS;
}