#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <map>
#include <string>

// Размер виртуальной страницы
#define PAGE_SIZE 4096

class SharedMemory
{
	static std::map<std::string, void*> segments;
public:
	// Захватить или вернуть память.
	// @return - указатель на память или NULL в случае ошибки
	static void* shmget(std::string key);

	// Вернуть размер виртуальной страницы
	static int getPageSize();

	// Очистить память
	static void clear();
};

#endif