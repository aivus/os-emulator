#include <iostream>
#include <vector>
#include <string>
#include "makePipeCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
makePipeCommand::makePipeCommand()
{
	//
}

// Деструктор
makePipeCommand::~makePipeCommand()
{
	//
}

int makePipeCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>1)
	{
		switch(vfs->makePipe(data[1].c_str()))
		{
			case 0:
				cout << "Канал успешно создан!" << endl;
				break;
			case -1:
				cerr << "Файл с таким именем уже существует!" << endl;
				break;
			case -2:
				cerr << "Слишком длинное имя файла!" << endl;
				break;
			case -3:
				cerr << "Невозможно создать файл. В текущей директории уже содержится максимальное количество записей!" << endl;
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