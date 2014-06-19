#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "addUserCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"

using namespace std;

// �����������
addUserCommand::addUserCommand()
{
	//
}

// ����������
addUserCommand::~addUserCommand()
{
	//
}

int addUserCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	cout << "�����: ";
	vector<string> login = Console::showPromt();
	cout << "������: ";
	string password = Console::enterPassword();

	switch(int uid = vfs->addUser(login[0].c_str(), password.c_str()))
	{
		case -1:
			cerr << "������ ����� ��� ������!" << endl;
			break;
		case -2:
			cerr << "������ ������������� ����� ��������� �������������!" << endl;
			break;
		case -3:
			cerr << "������������ � ����� ������ ��� ����������!" << endl;
			break;
		case -4:
			cerr << "������� ������� �����! ������������ ������ " << VFS_USER_LOGIN_MAX_LEN << endl;
			break;
		case -5:
			cerr << "������� ������� ������! ������������ ������ " << VFS_USER_PASS_MAX_LEN << endl;
			break;
		case -6:
			cerr << "���������� ������������ ����� �������������!" << endl;
			break;
		default:
			cout << "������������ ������. UID: "<< uid << endl;
	}

	return OSSUCCESS;
}