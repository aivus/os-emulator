#include <iostream>
#include <vector>
#include <string>
#include "makePipeCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
makePipeCommand::makePipeCommand()
{
	//
}

// ����������
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
				cout << "����� ������� ������!" << endl;
				break;
			case -1:
				cerr << "���� � ����� ������ ��� ����������!" << endl;
				break;
			case -2:
				cerr << "������� ������� ��� �����!" << endl;
				break;
			case -3:
				cerr << "���������� ������� ����. � ������� ���������� ��� ���������� ������������ ���������� �������!" << endl;
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