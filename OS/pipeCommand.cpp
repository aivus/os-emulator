#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <process.h>
#include <Windows.h>
#include "pipeCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"
#include "Extra.h"
#include "Pipe.h"
#include <queue>

using namespace std;

CRITICAL_SECTION cs_pipe;
bool pipe_flag = false;

// �������, ������� ��������� ������ � ��������� ������
void writeProc(void* pParams)
{
	long pipe_id = (long) pParams;

	for (int i = 1; i <= 100; i++)
	{

		std::string str("������ � ������. ����� ������: ");
		str.append(Extra::convertIntToString(i));

		while(pipe_flag) Sleep(10);
		Pipe::write(pipe_id, str.c_str());
		EnterCriticalSection(&cs_pipe);
		pipe_flag = true;
		cout << "��������:\t" << str.c_str() << endl;
		pipe_flag = false;
		LeaveCriticalSection(&cs_pipe);
	}
}

// �������, ������� ��������� ������ � ��������� ������
void readProc(void* pParams)
{
	long pipe_id = (long) pParams;

	for (int i = 1; i <= 100; i++)
	{
		while(pipe_flag) Sleep(10);
		std::string str = Pipe::read(pipe_id);
		EnterCriticalSection(&cs_pipe);
		pipe_flag = true;
		cout << "���������:\t" << str << endl;
		pipe_flag = false;
		LeaveCriticalSection(&cs_pipe);
	}
}


// �����������
pipeCommand::pipeCommand()
{
	//
}

// ����������
pipeCommand::~pipeCommand()
{
	//
}

int pipeCommand::action(VFS *vfs, std::vector<std::string> data)
{
	// �������� ����� ����� � �������� ���
	long pipe_id = Pipe::getFreePipeNo();
	Pipe::allocate(pipe_id);

	HANDLE hThreads[2];
	InitializeCriticalSection(&cs_pipe);
	hThreads[0] = (HANDLE) _beginthread(writeProc, 0, (void*)pipe_id);
	hThreads[1] = (HANDLE) _beginthread(readProc, 0, (void*)pipe_id);
	WaitForMultipleObjects(2, hThreads, true, INFINITE);
	DeleteCriticalSection(&cs_pipe);
	return OSSUCCESS;
}

