#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Console.h"
#include "pwdCommand.h"
#include "CommandExecutor.h"
#include "VFS.h"
#include "retCodes.h"
#ifdef _DEBUG
//#include <vld.h>
#endif

using namespace std;

int start(void)
{
	system("cls");
	VFS* fs = NULL;

	// �������� �����������
	try
	{
		fs = new VFS(VFS_DISK_PATH);
	}
	catch(const runtime_error& e)
	{
		// �������� ������ �� ����� ������������ �������
		cerr << e.what() << endl;
		cerr << "���� ����������� �������� ������� ��������� ��� �����������. ��� ������� ������� ���." << endl;
		system("pause");
		delete fs;
		return -1;
	}

	CommandExecutor executor;

	bool loop = true;

	while (loop)
	{

		// ������� �����������
		cout << fs->getCurrentUserLogin() << "@";
		cout << "/" ;
		std::vector<std::string>::iterator it = pwdCommand::path.begin(), end = pwdCommand::path.end();
		for (; it != end; ++it)
			cout << it->c_str() << "/";
		cout << "> ";

		// ����������� �������
		vector<string> command = Console::showPromt();
		switch(executor.execute(fs, command))
		{
			case OSERROR:
				cout << "����������� �������!" << endl;
				break;
			case OSEXIT:
				loop = false;
				break;
		}
	}

	delete fs;
	return 0;
}

void install(void)
{
	system("cls");
	cout << "����������� ������ ����� �������� ������� 1048576 ����." << endl;
	cout << "������� ������ ����� (� ������): ";
	vector<string> disk = Console::showPromt();
	cout << "������� ������ �������������� (root): ";
	vector<string> password = Console::showPromt();

	try
	{
		switch(VFS::makeVFS(VFS_DISK_PATH, atoi(disk[0].c_str()), password[0].c_str()))
		{
			case -1:
				cerr << "[!] ������! �������� ������� �� �������. ����������� ������ �������� ������� 1 ��!" << endl;
				break;
		}
	}

	catch(const runtime_error& e)
	{
		cerr << e.what() << endl;
	}

	system("pause");
}

int main(int argc, char* argv[])
{
	setlocale( LC_ALL,"Russian" );
	bool loop = true;
	while(loop)
	{
		system("cls");
		cout << "�������� ��������:" << endl;
		cout << "1 - ������ �������" << endl;
		cout << "2 - ��������� �������" << endl;
		cout << "3 - �����" << endl;
		cout << "������� ��������: ";
		vector<string> command = Console::showPromt();
		if (command.size() > 0)
		{
			switch(atoi(command[0].c_str()))
			{
				case 1:
					if(start() == 0)
						loop = false;
					break;
				case 2:
					install();
					break;
				case 3:
					loop = false;
					break;
				default:
					cerr << "�������� �����!" << endl;
					break;
			}
		}
	}

	return 0;
}