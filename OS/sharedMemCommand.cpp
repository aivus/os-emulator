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

// Функция, которая выполняет запись в отдельном потоке
void writeSharedMemProc(void* pParams)
{
	EnterCriticalSection(&cs_shm);

	// Получаем память с ключем SHARED_MEMORY_KEY
	char* str = (char*) SharedMemory::shmget(SHARED_MEMORY_KEY);

	// Обнуляем её
	memset(str,0, SharedMemory::getPageSize());

	// Записываем строчку
	std::string text("Привет, Мир! Я текст из разделяемой памяти. Сохранен по адресу " + Extra::convertIntToString((int)str));
	memcpy(str, text.c_str(), text.size());
	cout << "Память записана!" << endl;
	LeaveCriticalSection(&cs_shm);
}

// Функция, которая выполняет чтение в отдельном потоке
void readSharedMemProc(void* pParams)
{
	char* title = (char*) pParams;
	EnterCriticalSection(&cs_shm);
	char* str = (char*) SharedMemory::shmget(SHARED_MEMORY_KEY);
	std::cout << title << " " << str << std::endl;
	LeaveCriticalSection(&cs_shm);
}


// Конструктор
sharedMemCommand::sharedMemCommand()
{
	//
}

// Деструктор
sharedMemCommand::~sharedMemCommand()
{
	//
}

int sharedMemCommand::action(VFS *vfs, std::vector<std::string> data)
{
	HANDLE hThreads[5];
	InitializeCriticalSection(&cs_shm);
	// Запускаем поток записи
	hThreads[0] = (HANDLE) _beginthread(writeSharedMemProc, 0, NULL);
	WaitForSingleObject(hThreads[0], INFINITE);

	// Запускаем потоки чтения
	hThreads[1] = (HANDLE) _beginthread(readSharedMemProc, 0, "Поток №1");
	hThreads[2] = (HANDLE) _beginthread(readSharedMemProc, 0, "Поток №2");
	hThreads[3] = (HANDLE) _beginthread(readSharedMemProc, 0, "Поток №3");
	hThreads[4] = (HANDLE) _beginthread(readSharedMemProc, 0, "Поток №4");
	WaitForSingleObject(hThreads[1], 5000);
	WaitForSingleObject(hThreads[2], 5000);
	WaitForSingleObject(hThreads[3], 5000);
	WaitForSingleObject(hThreads[4], 5000);
	DeleteCriticalSection(&cs_shm);

	// Освобождаем занятую память
	SharedMemory::clear();

	return OSSUCCESS;
}

