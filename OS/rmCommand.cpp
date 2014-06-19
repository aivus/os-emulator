#include <iostream>
#include <vector>
#include <string>
#include "rmCommand.h"
#include "pwdCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
rmCommand::rmCommand()
{
	//
}

// ����������
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
				cout << "������ ������� �������!" << endl;
				break;
			case -1:
				cerr << "������ '" << name << "' �� �������!" << endl;
				break;
			case -2:
				cerr << "���������� ������� . ��� .." << endl;
				break;
			case -3:
				cerr << "����� �� �����!" << endl;
				break;
			case -4:
				cerr << "� ��� ������������ ����!" << endl;
				break;
			case -5:
				cerr << "���������� ������� ���������� ����!" << endl;
				break;
		}
	}
	else
	{
		cerr << "�������� ���������� ����������!" << endl;
	}

	return OSSUCCESS;
}