#include <iostream>
#include <vector>
#include <string>
#include "renameCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
renameCommand::renameCommand()
{
	//
}

// Деструктор
renameCommand::~renameCommand()
{
	//
}

int renameCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>2)
	{
		switch(vfs->rename(data[1].c_str(), data[2].c_str()))
		{
			case 0:
				cout << "Запись успешно переименована!" << endl;
				break;
			case -1:
				cerr << "Запись '" << data[1] << "' не найдена!" << endl;
				break;
			case -2:
				cerr << "Невозможно переименовать . или .." << endl;
				break;
			case -3:
				cerr << "Запись '" << data[2] << "' уже существует!" << endl;
				break;
			case -4:
				cerr << "Слишком длинное имя!" << endl;
				break;
			case -5:
				cerr << "У вас недостаточно прав!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Неверное количество параметров!" << endl;
		usage();
	}

	return OSSUCCESS;
}

void renameCommand::usage()
{
	cout << "Использование:" << endl;
	cout << "\trename <old_name> <new_name>" << endl;
	cout << "\t\tПереименование файла/директории." << endl;
	cout << "\t\t<old_name> - Старое имя файла/директории." << endl;
	cout << "\t\t<new_name> - Новое имя файла/директории." << endl;
	cout << endl;

}