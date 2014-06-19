#include <iostream>
#include <vector>
#include <string>
#include "chownCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
chownCommand::chownCommand()
{
	//
}

// Деструктор
chownCommand::~chownCommand()
{
	//
}

int chownCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>2)
	{
		switch(vfs->chown(atoi(data[1].c_str()), data[2].c_str()))
		{
			case 0:
				cout << "Владелец успешно изменен!" << endl;
				break;
			case -1:
				cerr << "Запись '" << data[2] << "' не найдена!" << endl;
				break;
			case -2:
				cerr << "Вы не являетесь владельцем записи!" << endl;
				break;
			case -3:
				cerr << "Неверный UID!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Неверное количество параметров!" << endl;
	}

	return OSSUCCESS;
}