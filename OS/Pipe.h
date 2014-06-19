#ifndef PIPE_H
#define PIPE_H

#include <Windows.h>
#include <string>
#include <queue>
#include <map>
#include "VFS_Types.h"

#define PIPE_MAX_SIZE 5
#define PIPE_TABLE_SIZE 1024

class Pipe
{
	static CRITICAL_SECTION cs_pipe;
	static std::queue<std::string> pipes[PIPE_TABLE_SIZE];
	static VFS_DWORD free_pipe_no;
	static long free_simple_pipe_no;
	static std::map<long, VFS_DWORD> links;
	static bool initialized;
	static void init();
	Pipe();
	~Pipe();
public:
	static void allocate(long pipe_id);
	static long getFreePipeNo();
	static std::string read(long pipe_id);
	static void write(long pipe_id, std::string data);

};

#endif