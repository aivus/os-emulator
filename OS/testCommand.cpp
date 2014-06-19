#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>
#include "testCommand.h"
#include "VFS.h"
#include "retCodes.h"

using namespace std;

// Конструктор
testCommand::testCommand()
{
	//
}

// Деструктор
testCommand::~testCommand()
{
	//
}

int testCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	//vfs->addUser("admin", "password");
	//vfs->login("admin", "password");
	/*
	vfs->writeFile("test", "aaaaaaaaaaaaa", 14);

	void* da;
	if (vfs->readFile("test", da)>0)
	{
		cout << (char*) da;
		delete[] da;
	}
	*/
	/*
	for (int i = 0; i < 1740; i++)
	{
		std::stringstream s;
		//s << "Folder_" << i;
		//vfs->makeDir(s.str().c_str());
		s << "File_" << i;
		if(vfs->makeFile(s.str().c_str(), 0) == -3)
			break;
	}
	*/
	/*
	std::stringstream stream;
	for (int i = 0; i < 8200; i++)
	{
		stream << "a";
	}
	vfs->writeFile("test", (void*)stream.str().c_str(), 8200);
	*/
	
	for (int i = 0; i < 1600; i++)
	{
		std::stringstream s;
		//s << "Folder_" << i;
		//vfs->makeDir(s.str().c_str());
		if (i % 100 == 0)
			cout << "Пользователь " << i << "..." << endl;
		s << "User_" << i;
		vfs->addUser(s.str().c_str(), "pass");
		Sleep(10);
	}
	

	return OSSUCCESS;
}