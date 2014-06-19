#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <process.h>
#include <Windows.h>
#include "namedPipeCommand.h"
#include "catCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"
#include "Extra.h"
#include "Pipe.h"
#include <queue>

using namespace std;

CRITICAL_SECTION cs_named_pipe;
bool named_flag = false;

// �������, ������� ��������� ������ � ��������� ������
void writeNamedPipeProc(void* pParams)
{
	long pipe_id = (long) pParams;
	for (int i = 1; i <= 100; i++)
	{
		std::string str("������ � ������. ����� ������: ");
		str.append(Extra::convertIntToString(i));

		while(named_flag) Sleep(10);
		Pipe::write(pipe_id, str.c_str());
		EnterCriticalSection(&cs_named_pipe);
		named_flag = true;
		cout << "��������:\t" << str.c_str() << endl;
		named_flag = false;
		LeaveCriticalSection(&cs_named_pipe);
	}
}

// �������, ������� ��������� ������ � ��������� ������
void readNamedPipeProc(void* pParams)
{
	long pipe_id = (long) pParams;
	for (int i = 1; i <= 100; i++)
	{
		while(named_flag) Sleep(10);
		std::string str = Pipe::read(pipe_id);
		EnterCriticalSection(&cs_named_pipe);
		named_flag = true;
		cout << "���������:\t" << str << endl;
		named_flag = false;
		LeaveCriticalSection(&cs_named_pipe);
	}
}


// �����������
namedPipeCommand::namedPipeCommand()
{
	//
}

// ����������
namedPipeCommand::~namedPipeCommand()
{
	//
}

int namedPipeCommand::action(VFS *vfs, std::vector<std::string> data)
{
	int res;
	if ((res = vfs->makePipe("pipe_test")) < 0 && res != -1)
	{
		cerr << "��������� ������. " << Extra::convertIntToString(res) << endl;
		return OSSUCCESS;
	}

	long pipe_id = vfs->getInodeNo("pipe_test");

	HANDLE hThreads[2];
	InitializeCriticalSection(&cs_named_pipe);
	hThreads[0] = (HANDLE) _beginthread(writeNamedPipeProc, 0, (void*) pipe_id);
	hThreads[1] = (HANDLE) _beginthread(readNamedPipeProc, 0, (void*) pipe_id);
	WaitForMultipleObjects(2, hThreads, true, INFINITE);
	DeleteCriticalSection(&cs_named_pipe);
	return OSSUCCESS;
}

