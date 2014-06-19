#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <map>
#include <string>

// ������ ����������� ��������
#define PAGE_SIZE 4096

class SharedMemory
{
	static std::map<std::string, void*> segments;
public:
	// ��������� ��� ������� ������.
	// @return - ��������� �� ������ ��� NULL � ������ ������
	static void* shmget(std::string key);

	// ������� ������ ����������� ��������
	static int getPageSize();

	// �������� ������
	static void clear();
};

#endif