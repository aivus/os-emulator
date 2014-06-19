#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "removeUserCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"

using namespace std;

// �����������
removeUserCommand::removeUserCommand()
{
	//
}

// ����������
removeUserCommand::~removeUserCommand()
{
	//
}

int removeUserCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size() > 1)
	{
		switch(vfs->removeUser(data[1].c_str()))
		{
			case 0:
				cout << "������������ ������!" << endl;
				break;
			case -1:
				cerr << "������ �����!" << endl;
				break;
			case -2:
				cerr << "������ ������������� (root) ����� ������� �������������!" << endl;
				break;
			case -3:
				cerr << "������������ � ����� ������� �� ������!" << endl;
				break;
			case -4:
				cerr << "������ ������� �������������� (root)!" << endl;
				break;
		}
	}
	else
	{
		cerr << "�������� ���������� ����������!" << endl;
	}

	return OSSUCCESS;
}