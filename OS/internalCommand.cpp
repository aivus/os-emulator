#include <iostream>
#include <vector>
#include <string>
#include "internalCommand.h"
#include "retCodes.h"
#include "VFS.h"


// Конструктор
internalCommand::internalCommand()
{
	//
}

// Деструктор
internalCommand::~internalCommand()
{
	//
}

int internalCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (data[0] == "clear")
		system("cls");
	else if (data[0] == "shutdown" || data[0] == "exit")
		return OSEXIT;
	else if (data[0] == "help")
		help();

	return OSSUCCESS;
}

void internalCommand::help()
{
	std::cout << "============================================" << std::endl;
	std::cout << "\t\tПомощь" << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "help\t\t- Помощь" << std::endl;
	std::cout << "clear\t\t- Очистка экрана" << std::endl;
	std::cout << "stat\t\t- Вывод информации суперблока" << std::endl;
	std::cout << "mkdir\t\t- Создание новой директории" << std::endl;
	std::cout << "cd\t\t- Смена текущей директории" << std::endl;
	std::cout << "ls\t\t- Просмотр содержимого текущей директории" << std::endl;
	std::cout << "pwd\t\t- Отображение текущего пути" << std::endl;
	std::cout << "touch\t\t- Создание файла" << std::endl;
	std::cout << "cat\t\t- Просмотр содержимого файла" << std::endl;
	std::cout << "chmod\t\t- Смена прав доступа на файл/директорию" << std::endl;
	std::cout << "chown\t\t- Смена владельца файла/директории" << std::endl;
	std::cout << "rename\t\t- Переименование файла/директории" << std::endl;
	std::cout << "cp\t\t- Копирование файла" << std::endl;
	std::cout << "rm\t\t- Удаление файла/директории(пустой)" << std::endl;
	std::cout << "who\t\t- Показать ID текущего пользователя" << std::endl;
	std::cout << "sudo\t\t- Вход в систему" << std::endl;
	std::cout << "adduser\t\t- Добавление нового пользователя" << std::endl;
	std::cout << "rmuser\t\t- Удаление пользователя" << std::endl;
	std::cout << "shutdown\t- Выключение" << std::endl;
	std::cout << "exit\t\t- Выключение" << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "\tКоманды межпроцессного взаимодействия" << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "testPipe\t- Проверка работы каналов" << std::endl;
	std::cout << "testNamedPipe\t- Проверка работы именованных каналов" << std::endl;
	std::cout << "testShm\t\t- Првоерка работы разделяемой памяти" << std::endl;
	std::cout << "============================================" << std::endl;

}
