#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "sudoCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"

using namespace std;

// �����������
sudoCommand::sudoCommand()
{
	//
}

// ����������
sudoCommand::~sudoCommand()
{
	//
}

int sudoCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	cout << "�����: ";
	vector<string> login = Console::showPromt();
	cout << "������: ";
	string password = Console::enterPassword();

	switch(int uid = vfs->login(login[0].c_str(), password.c_str()))
	{
		case 0:
			cout << "�������� ���� � �������!" << endl;
			break;
		case -1:
			cerr << "�������� ����� ��� ������!" << endl;
			break;
	}

	return OSSUCCESS;
}