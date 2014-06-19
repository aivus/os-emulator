#include <string>
#include <queue>
#include <map>
#include <Windows.h>
#include "Pipe.h"

CRITICAL_SECTION Pipe::cs_pipe;
std::queue<std::string> Pipe::pipes[PIPE_TABLE_SIZE];
VFS_DWORD Pipe::free_pipe_no = 0;
long Pipe::free_simple_pipe_no = 0;
std::map<long, VFS_DWORD> Pipe::links;
bool Pipe::initialized = false;

// Инициализация каналов
void Pipe::init()
{
	if (initialized)
		return;
	bool initialized = true;
	InitializeCriticalSection(&cs_pipe);
}

// Выделение канала
void Pipe::allocate(long pipe_id)
{
	links.insert(std::pair<long, VFS_DWORD>(pipe_id, Pipe::free_pipe_no));
	Pipe::free_pipe_no++;
}

long Pipe::getFreePipeNo()
{
	return free_simple_pipe_no--;
}

// Чтение из канала
std::string Pipe::read(long pipe_id)
{
	if (!initialized)
		Pipe::init();

	VFS_DWORD internal_id = Pipe::links[pipe_id];
	
	// Если очередь пуста - блоокируемся
	while(pipes[internal_id].size() == 0)
		Sleep(10);
	EnterCriticalSection(&cs_pipe);
	std::string result = pipes[internal_id].front();
	pipes[internal_id].pop();
	LeaveCriticalSection(&cs_pipe);

	return result;
}

// Запись в канал
void Pipe::write(long pipe_id, std::string data)
{
	if (!initialized)
		Pipe::init();

	VFS_DWORD internal_id = Pipe::links[pipe_id];

	// Если канал полон - блокируемся
	if (pipes[internal_id].size() == PIPE_MAX_SIZE)
		Sleep(10);

	EnterCriticalSection(&cs_pipe);
	pipes[internal_id].push(data);
	LeaveCriticalSection(&cs_pipe);
}