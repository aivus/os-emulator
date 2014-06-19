#include <iostream>
#include <vector>
#include <string>
#include "touchCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
touchCommand::touchCommand()
{
	//
}

// ����������
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
				cout << "���� ������� ������!" << endl;
				break;
			case -1:
				cerr << "���� � ����� ������ ��� ����������!" << endl;
				break;
			case -2:
				cerr << "������� ������� ��� �����!" << endl;
				break;
			case -3:
				cerr << "���������� ������� ����. � ������� ���������� ��� ���������� ������������ ���������� �������!!" << endl;
				break;
			case -4:
				cerr << "� ��� ������������ ����!" << endl;
				break;
		}
	}
	else
	{
		cerr << "�������� ���������� ����������!" << endl;
	}

	return OSSUCCESS;
}