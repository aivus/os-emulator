#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <process.h>
#include <Windows.h>
#include "SharedMemory.h"
#include "sharedMemCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Console.h"
#include "Extra.h"

using namespace std;

CRITICAL_SECTION cs_shm;

#define SHARED_MEMORY_KEY "Test SHM Key"

// �������, ������� ��������� ������ � ��������� ������
void writeSharedMemProc(void* pParams)
{
	EnterCriticalSection(&cs_shm);

	// �������� ������ � ������ SHARED_MEMORY_KEY
	char* str = (char*) SharedMemory::shmget(SHARED_MEMORY_KEY);

	// �������� �
	memset(str,0, SharedMemory::getPageSize());

	// ���������� �������
	std::string text("������, ���! � ����� �� ����������� ������. �������� �� ������ " + Extra::convertIntToString((int)str));
	memcpy(str, text.c_str(), text.size());
	cout << "������ ��������!" << endl;
	LeaveCriticalSection(&cs_shm);
}

// �������, ������� ��������� ������ � ��������� ������
void readSharedMemProc(void* pParams)
{
	char* title = (char*) pParams;
	EnterCriticalSection(&cs_shm);
	char* str = (char*) SharedMemory::shmget(SHARED_MEMORY_KEY);
	std::cout << title << " " << str << std::endl;
	LeaveCriticalSection(&cs_shm);
}


// �����������
sharedMemCommand::sharedMemCommand()
{
	//
}

// ����������
sharedMemCommand::~sharedMemCommand()
{
	//
}

int sharedMemCommand::action(VFS *vfs, std::vector<std::string> data)
{
	HANDLE hThreads[5];
	InitializeCriticalSection(&cs_shm);
	// ��������� ����� ������
	hThreads[0] = (HANDLE) _beginthread(writeSharedMemProc, 0, NULL);
	WaitForSingleObject(hThreads[0], INFINITE);

	// ��������� ������ ������
	hThreads[1] = (HANDLE) _beginthread(readSharedMemProc, 0, "����� �1");
	hThreads[2] = (HANDLE) _beginthread(readSharedMemProc, 0, "����� �2");
	hThreads[3] = (HANDLE) _beginthread(readSharedMemProc, 0, "����� �3");
	hThreads[4] = (HANDLE) _beginthread(readSharedMemProc, 0, "����� �4");
	WaitForSingleObject(hThreads[1], 5000);
	WaitForSingleObject(hThreads[2], 5000);
	WaitForSingleObject(hThreads[3], 5000);
	WaitForSingleObject(hThreads[4], 5000);
	DeleteCriticalSection(&cs_shm);

	// ����������� ������� ������
	SharedMemory::clear();

	return OSSUCCESS;
}

