#include <iostream>
#include <vector>
#include <string>
#include "chmodCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
chmodCommand::chmodCommand()
{
	//
}

// Деструктор
chmodCommand::~chmodCommand()
{
	//
}

int chmodCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>2)
	{
		switch(vfs->chmod(atoi(data[1].c_str()), data[2].c_str()))
		{
			case 0:
				cout << "Права доступа успешно изменены!" << endl;
				break;
			case -1:
				cerr << "Запись '" << data[2] << "' не найдена!" << endl;
				break;
			case -2:
				cerr << "Вы не являетесь владельцем записи!" << endl;
				break;
			case -3:
				cerr << "Неверные права доступа!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Неверное количество параметров!" << endl;
	}

	return OSSUCCESS;
}