#include <iostream>
#include <vector>
#include <string>
#include "changeDirCommand.h"
#include "pwdCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
changeDirCommand::changeDirCommand()
{
	//
}

// Деструктор
changeDirCommand::~changeDirCommand()
{
	//
}

int changeDirCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size() > 1)
	{
		int result = vfs->change_directory(data[1].c_str());
		switch(result)
		{
			case 0:
				// Удаляем, если переходим в ..
				if (data[1] == ".." && pwdCommand::path.size()>0)
					pwdCommand::path.pop_back();

				if (data[1] != "." && data[1] != "..")
					pwdCommand::path.push_back(data[1]);

				break;
			case -1:
				cerr << "Директория '" << data[1] << "' не найдена!" << endl;
				break;
			case -2:
				cerr << "У вас недостаточно прав!" << endl;
				break;
		}
	}
	else
	{
		cerr << "Не задано имя директории!" << endl;
	}

	return OSSUCCESS;
}