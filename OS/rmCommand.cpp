#include <iostream>
#include <vector>
#include <string>
#include "rmCommand.h"
#include "pwdCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
rmCommand::rmCommand()
{
	//
}

// Деструктор
rmCommand::~rmCommand()
{
	//
}

int rmCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	const char* name;
	bool recursive;

	if (data.size() > 1)
	{
		
		if (data[1] == "-r")
		{
			name = data[2].c_str();
			recursive = true;
		}
		else
		{
			name = data[1].c_str();
			recursive = false;
		}

		switch(vfs->remove(name, recursive))
		{
			case 0:
				cout << "Запись успешно удалена!" << endl;
				break;
			case -1:
				cerr << "Запись '" << name << "' не найдена!" << endl;
				break;
			case -2:
				cerr << "Невозможно удалить . или .." << endl;
				break;
			case -3:
				cerr << "Папка не пуста!" << endl;
				break;
			case -4:
				cerr << "У вас недостаточно прав!" << endl;
				break;
			case -5:
				cerr << "Невозможно удалить защищенный файл!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Неверное количество параметров!" << endl;
	}

	return OSSUCCESS;
}