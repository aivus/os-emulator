#include <iostream>
#include <vector>
#include <string>
#include "makeDirCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
makeDirCommand::makeDirCommand()
{
	//
}

// ����������
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
				cout << "����� ������� �������!" << endl;
				break;
			case -1:
				cerr << "����� � ����� ������ ��� ����������!" << endl;
				break;
			case -2:
				cerr << "������� ������� ��� �����!" << endl;
				break;
			case -3:
				cerr << "���������� ������� �����. � ������� ���������� ��� ���������� ������������ ���������� �������!" << endl;
				break;
			case -4:
				cerr << "� ��� ������������ ����!" << endl;
				break;
		}
	}	
	else
	{
		cout << "�������� ���������� ����������!" << endl;
	}
	return OSSUCCESS;
}