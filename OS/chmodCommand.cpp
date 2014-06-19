#include <iostream>
#include <vector>
#include <string>
#include "chmodCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
chmodCommand::chmodCommand()
{
	//
}

// ����������
chmodCommand::~chmodCommand()
{
	//
}

int chmodCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>2)
	{
		switch(vfs->chmod(atoi(data[1].c_str()), data[2].c_str()))
		{
			case 0:
				cout << "����� ������� ������� ��������!" << endl;
				break;
			case -1:
				cerr << "������ '" << data[2] << "' �� �������!" << endl;
				break;
			case -2:
				cerr << "�� �� ��������� ���������� ������!" << endl;
				break;
			case -3:
				cerr << "�������� ����� �������!" << endl;
				break;
		}
	}
	else
	{
		cerr << "�������� ���������� ����������!" << endl;
	}

	return OSSUCCESS;
}