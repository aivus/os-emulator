#include <iostream>
#include <vector>
#include <string>
#include "copyCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
copyCommand::copyCommand()
{
	//
}

// ����������
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
				cout << "���� ������� ����������!" << endl;
				break;
			case -1:
				cerr << "���������� ���������� . ��� .." << endl;
				break;
			case -2:
				cerr << "������� ������� ���!" << endl;
				break;
			case -3:
				cerr << "���� '" << data[1] << "' �� ������!" << endl;
				break;
			case -4:
				cerr << "���� '" << data[2] << "' ��� ����������!" << endl;
				break;
			case -5:
				cerr << "������ '" << data[1] << "' - ����������! �� ������ ���������� ������ �����!" << endl;
				break;
			case -6:
				cerr << "���������� ������� ����� �����. � ����� ��� ���������� ������������ ���������� ������!" << endl;
				break;
			case -7:
				cerr << "� ��� ������������ ����!" << endl;
				break;
		}
	}
	else
	{
		cerr << "�������� ���������� ����������!" << endl;
		usage();
	}

	return OSSUCCESS;
}

void copyCommand::usage()
{
	cout << "�������������:" << endl;
	cout << "\tcp <source_name> <dest_name>" << endl;
	cout << "\t\t���������� ����." << endl;
	cout << "\t\t<source_name> - �������� ����." << endl;
	cout << "\t\t<dest_name> - �������� ����." << endl;
	cout << endl;

}