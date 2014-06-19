#include <map>
#include "SharedMemory.h"

std::map<std::string, void*> SharedMemory::segments;

// ��������� ��� ������� ������.
// @return - ��������� �� ������ ��� NULL � ������ ������
void* SharedMemory::shmget(std::string key)
{
	if (!segments.count(key))
	{
		// �������� ������
		void* memory = new char[PAGE_SIZE];
		segments.insert(std::pair<std::string, void*>(key, memory));
	}

	return segments[key];
}

int SharedMemory::getPageSize()
{
	return PAGE_SIZE;
}

// �������� ������
void SharedMemory::clear()
{
	std::map<std::string, void*>::iterator it = segments.begin(), end = segments.end(); 
	for(; it != end; ++it)
		delete[] it->second;

	segments.clear();
}