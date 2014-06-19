#include <iostream>
#include <vector>
#include <string>
#include "whoCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
whoCommand::whoCommand()
{
	//
}

// ����������
whoCommand::~whoCommand()
{
	//
}

int whoCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;
	cout << "ID �������� ������������: " << vfs->getCurrentUserID() << endl;
	return OSSUCCESS;
}