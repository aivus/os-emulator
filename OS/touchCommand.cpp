#include <iostream>
#include <vector>
#include <string>
#include "touchCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
touchCommand::touchCommand()
{
	//
}

// Деструктор
touchCommand::~touchCommand()
{
	//
}

int touchCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>1)
	{
		switch(vfs->makeFile(data[1].c_str(), 0))
		{
			case 0:
				cout << "Файл успешно создан!" << endl;
				break;
			case -1:
				cerr << "Файл с таким именем уже существует!" << endl;
				break;
			case -2:
				cerr << "Слишком длинное имя файла!" << endl;
				break;
			case -3:
				cerr << "Невозможно создать файл. В текущей директории уже содержится максимальное количество записей!!" << endl;
				break;
			case -4:
				cerr << "У вас недостаточно прав!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Неверное количество параметров!" << endl;
	}

	return OSSUCCESS;
}