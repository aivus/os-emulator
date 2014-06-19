#include <iostream>
#include <vector>
#include <string>
#include "makeDirCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
makeDirCommand::makeDirCommand()
{
	//
}

// Деструктор
makeDirCommand::~makeDirCommand()
{
	//
}

int makeDirCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>1)
	{
		switch(vfs->makeDir((char*)data[1].c_str()))
		{
			case 0:
				cout << "Папка успешно создана!" << endl;
				break;
			case -1:
				cerr << "Папка с таким именем уже существует!" << endl;
				break;
			case -2:
				cerr << "Слишком длинное имя папки!" << endl;
				break;
			case -3:
				cerr << "Невозможно создать папку. В текущей директории уже содержится максимальное количество записей!" << endl;
				break;
			case -4:
				cerr << "У вас недостаточно прав!" << endl;
				break;
		}
	}	
	else
	{
		cout << "Неверное количество параметров!" << endl;
	}
	return OSSUCCESS;
}