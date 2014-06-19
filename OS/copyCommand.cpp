#include <iostream>
#include <vector>
#include <string>
#include "copyCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
copyCommand::copyCommand()
{
	//
}

// Деструктор
copyCommand::~copyCommand()
{
	//
}

int copyCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>2)
	{
		switch(vfs->copy(data[1].c_str(), data[2].c_str()))
		{
			case 0:
				cout << "Файл успешно скопирован!" << endl;
				break;
			case -1:
				cerr << "Невозможно копировать . или .." << endl;
				break;
			case -2:
				cerr << "Слишком длинное имя!" << endl;
				break;
			case -3:
				cerr << "Файл '" << data[1] << "' не найден!" << endl;
				break;
			case -4:
				cerr << "Файл '" << data[2] << "' уже существует!" << endl;
				break;
			case -5:
				cerr << "Запись '" << data[1] << "' - директория! Вы можете копировать только файлы!" << endl;
				break;
			case -6:
				cerr << "Невозможно создать копию файла. В папке уже содержится максимальное количество файлов!" << endl;
				break;
			case -7:
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

void copyCommand::usage()
{
	cout << "Использование:" << endl;
	cout << "\tcp <source_name> <dest_name>" << endl;
	cout << "\t\tКопировать файл." << endl;
	cout << "\t\t<source_name> - Исходный файл." << endl;
	cout << "\t\t<dest_name> - Конечный файл." << endl;
	cout << endl;

}