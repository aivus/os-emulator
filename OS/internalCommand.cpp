#include <iostream>
#include <vector>
#include <string>
#include "internalCommand.h"
#include "retCodes.h"
#include "VFS.h"


// �����������
internalCommand::internalCommand()
{
	//
}

// ����������
internalCommand::~internalCommand()
{
	//
}

int internalCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (data[0] == "clear")
		system("cls");
	else if (data[0] == "shutdown" || data[0] == "exit")
		return OSEXIT;
	else if (data[0] == "help")
		help();

	return OSSUCCESS;
}

void internalCommand::help()
{
	std::cout << "============================================" << std::endl;
	std::cout << "\t\t������" << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "help\t\t- ������" << std::endl;
	std::cout << "clear\t\t- ������� ������" << std::endl;
	std::cout << "stat\t\t- ����� ���������� ����������" << std::endl;
	std::cout << "mkdir\t\t- �������� ����� ����������" << std::endl;
	std::cout << "cd\t\t- ����� ������� ����������" << std::endl;
	std::cout << "ls\t\t- �������� ����������� ������� ����������" << std::endl;
	std::cout << "pwd\t\t- ����������� �������� ����" << std::endl;
	std::cout << "touch\t\t- �������� �����" << std::endl;
	std::cout << "cat\t\t- �������� ����������� �����" << std::endl;
	std::cout << "chmod\t\t- ����� ���� ������� �� ����/����������" << std::endl;
	std::cout << "chown\t\t- ����� ��������� �����/����������" << std::endl;
	std::cout << "rename\t\t- �������������� �����/����������" << std::endl;
	std::cout << "cp\t\t- ����������� �����" << std::endl;
	std::cout << "rm\t\t- �������� �����/����������(������)" << std::endl;
	std::cout << "who\t\t- �������� ID �������� ������������" << std::endl;
	std::cout << "sudo\t\t- ���� � �������" << std::endl;
	std::cout << "adduser\t\t- ���������� ������ ������������" << std::endl;
	std::cout << "rmuser\t\t- �������� ������������" << std::endl;
	std::cout << "shutdown\t- ����������" << std::endl;
	std::cout << "exit\t\t- ����������" << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "\t������� �������������� ��������������" << std::endl;
	std::cout << "============================================" << std::endl;
	std::cout << "testPipe\t- �������� ������ �������" << std::endl;
	std::cout << "testNamedPipe\t- �������� ������ ����������� �������" << std::endl;
	std::cout << "testShm\t\t- �������� ������ ����������� ������" << std::endl;
	std::cout << "============================================" << std::endl;

}
