#include <iostream>
#include <vector>
#include <string>
#include "renameCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
renameCommand::renameCommand()
{
	//
}

// ����������
renameCommand::~renameCommand()
{
	//
}

int renameCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>2)
	{
		switch(vfs->rename(data[1].c_str(), data[2].c_str()))
		{
			case 0:
				cout << "������ ������� �������������!" << endl;
				break;
			case -1:
				cerr << "������ '" << data[1] << "' �� �������!" << endl;
				break;
			case -2:
				cerr << "���������� ������������� . ��� .." << endl;
				break;
			case -3:
				cerr << "������ '" << data[2] << "' ��� ����������!" << endl;
				break;
			case -4:
				cerr << "������� ������� ���!" << endl;
				break;
			case -5:
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

void renameCommand::usage()
{
	cout << "�������������:" << endl;
	cout << "\trename <old_name> <new_name>" << endl;
	cout << "\t\t�������������� �����/����������." << endl;
	cout << "\t\t<old_name> - ������ ��� �����/����������." << endl;
	cout << "\t\t<new_name> - ����� ��� �����/����������." << endl;
	cout << endl;

}