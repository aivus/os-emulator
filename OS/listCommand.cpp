#include <iostream>
#include <vector>
#include <string>
#include "listCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// �����������
listCommand::listCommand()
{
	//
}

// ����������
listCommand::~listCommand()
{
	//
}

int listCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;
	switch(vfs->get_current_directory())
	{
		case -1:
			cerr << "� ��� ������������ ����!" << endl;
			break;
	}

	return OSSUCCESS;
}