#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Console.h"
#include "pwdCommand.h"
#include "CommandExecutor.h"
#include "VFS.h"
#include "retCodes.h"
#ifdef _DEBUG
//#include <vld.h>
#endif

using namespace std;

int start(void)
{
	system("cls");
	VFS* fs = NULL;

	// Пытаемся загрузиться
	try
	{
		fs = new VFS(VFS_DISK_PATH);
	}
	catch(const runtime_error& e)
	{
		// Возникла ошибка во время монтирования раздела
		cerr << e.what() << endl;
		cerr << "Файл виртуальной файловой системы поврежден или отсутствует. Вам следует создать его." << endl;
		system("pause");
		delete fs;
		return -1;
	}

	CommandExecutor executor;

	bool loop = true;

	while (loop)
	{

		// Выводим приглашение
		cout << fs->getCurrentUserLogin() << "@";
		cout << "/" ;
		std::vector<std::string>::iterator it = pwdCommand::path.begin(), end = pwdCommand::path.end();
		for (; it != end; ++it)
			cout << it->c_str() << "/";
		cout << "> ";

		// Запрашиваем команду
		vector<string> command = Console::showPromt();
		switch(executor.execute(fs, command))
		{
			case OSERROR:
				cout << "Неизвестная команда!" << endl;
				break;
			case OSEXIT:
				loop = false;
				break;
		}
	}

	delete fs;
	return 0;
}

void install(void)
{
	system("cls");
	cout << "Минимальный размер диска файловой системы 1048576 байт." << endl;
	cout << "Введите размер диска (в байтах): ";
	vector<string> disk = Console::showPromt();
	cout << "Введите пароль администратора (root): ";
	vector<string> password = Console::showPromt();

	try
	{
		switch(VFS::makeVFS(VFS_DISK_PATH, atoi(disk[0].c_str()), password[0].c_str()))
		{
			case -1:
				cerr << "[!] Ошибка! Файловая система не создана. Минимальный размер файловой системы 1 МБ!" << endl;
				break;
		}
	}

	catch(const runtime_error& e)
	{
		cerr << e.what() << endl;
	}

	system("pause");
}

int main(int argc, char* argv[])
{
	setlocale( LC_ALL,"Russian" );
	bool loop = true;
	while(loop)
	{
		system("cls");
		cout << "Выберите действие:" << endl;
		cout << "1 - Запуск системы" << endl;
		cout << "2 - Установка системы" << endl;
		cout << "3 - Выход" << endl;
		cout << "Введите действие: ";
		vector<string> command = Console::showPromt();
		if (command.size() > 0)
		{
			switch(atoi(command[0].c_str()))
			{
				case 1:
					if(start() == 0)
						loop = false;
					break;
				case 2:
					install();
					break;
				case 3:
					loop = false;
					break;
				default:
					cerr << "Неверный выбор!" << endl;
					break;
			}
		}
	}

	return 0;
}