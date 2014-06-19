#ifndef VFS_H
#define VFS_H

#include <string>
#include <stdio.h>
#include "VFS_Superblock.h"
#include "VFS_Bitmap.h"
#include "VFS_Inode.h"
#include "VFS_Shadow.h"
#include "VFS_Directory_Entry.h"

// Путь к файлу VFS
#define VFS_DISK_PATH	"fs.bin"

// Вычисляем сдвиг блока относительно начала
#define VFS_BLOCK_OFFSET(block) (VFS_START_SUPERBLOCK + VFS_SUPERBLOCK_SIZE + (superblock->s_first_data_block + block) * getBlockSize())

// Размер выделяемых записей при обозревании каталога
#define VFS_ALLOC_SIZE_4_VIEW_DIR	50

// Типы поиска для функции VFS::observeDir
enum TYPE_OF_OBSERVER_SEARCH
{
	// Что нужно искать?
	OBSERVER_FIND_SAME_ENTRY		= 0x001,	// Найти запись с таким же именем
	OBSERVER_FIND_EMPTY_ENTRY	= 0x002,	// Найти пустаю запись
	OBSERVER_FIND_ALL_ENTRIES	= 0x004,		// Найти все записи

	// Что нужно вернуть?
	OBSERVER_NEED_BLOCK_ENTRY			= 0x100,	// Нужно вернуть блок
};

// Структура, которую возвращает функция VFS::observeDir
struct VFS_Observer_Return
{
	bool				empty;		// Найдена пустая запись?
	bool				exist;		// Найдена запись с таким же именем?
	VFS_BYTE*			block;		// Указатель на блок данных, в котором была найдена нужная запись
	VFS_WORD			block_no;	// Номер блока в иноде, который содержит block
	VFS_Directory_Entry*entry;		// Указатель на найденную запись
	VFS_DWORD			countFound;	// Количество найденных записей (используется в OBSERVER_FIND_ALL_ENTRIES)
};

class VFS
{
	VFS_Inode*	curDirInode;	// Инод текущей директории
	VFS_DWORD	curDirInodeNo;	// Номер инода текущей директории
	VFS_WORD	curUserID;		// ID текущего пользователя
	char		curUserLogin[VFS_USER_LOGIN_MAX_LEN];		// Login текущего пользователя

	// ================================

	VFS_Superblock* superblock;			// Суперблок
	VFS_Bitmap* inodeBitmap;			// Битовая карта инодов
	VFS_Bitmap* blocksBitmap;			// Битовая карта блоков
	VFS_Inode*	inodeTable;				// Таблица инодов
	VFS_BYTE	initialized;			// Флаг инициализации (если =0, то неинициализировано)
	int VFS_File;						// Дескриптор файла

	// Прочитать информацию об корневом каталоге
	void read_root();

	// Создать инод
	VFS_DWORD create_inode(VFS_Inode *inode);
	VFS_DWORD create_inode(VFS_DWORD inode_no, VFS_Inode *inode);

	// Освободить инод
	void free_inode(VFS_DWORD inode_no);

	// Прочитать inode
	void read_inode(VFS_DWORD inode_no, VFS_Inode *&inode);

	// Прочитать блок
	void read_block(VFS_Inode *inode, VFS_DWORD block, VFS_BYTE *&buffer);

	// Записать блок
	void write_block(VFS_Inode *inode, VFS_DWORD block, VFS_BYTE *buffer);

	// Выделить (занять) блок данных
	VFS_DWORD allocate_block();

	// Освободить блок
	void free_block(VFS_DWORD block);

	// Создать заготовку директории
	void buildEmptyDir(VFS_DWORD currentInodeNo, VFS_DWORD parentInodeNo, VFS_Inode* inode);

	// Выделить и заполнить блок структурой директорий (расширяем директорию)
	VFS_Directory_Entry* fillDirBlock();

	// Функция поиска нужной записи в каталоге
	VFS_Observer_Return VFS::observeDir(const char* name, VFS_DWORD inode_no, VFS_WORD type);

	// Функция, которая расширяет 
	VFS_BYTE extendInode(VFS_Inode* inode);

public:
	VFS(std::string filename);
	VFS(std::string filename, bool read_root);
	virtual ~VFS();

	// Логин
	int login(const char* login, const char* password);

	// Получить текущий каталог
	int get_current_directory();

	// Изменить текущую директорию
	int change_directory(const char* dirName);

	// Создать папку
	int makeDir(const char* dirName);

	// Создать файл
	int makeFile(const char* fileName, VFS_WORD flag);

	// Создать файл-канал
	int makePipe(const char* fileName);

	// Прочитать данные из файла
	VFS_DWORD readFile(const char* fileName, void*& data);
	VFS_DWORD readFile(const char* fileName, void*& data, VFS_BYTE force);

	// Записать данные в файл
	int writeFile(const char* fileName, void* data, VFS_DWORD size);

	// Копировать файл
	int copy(const char* source_name, const char* dest_name);

	// Удалить папку/файл
	int remove(const char* name, bool recursive);

	// Переименовать папку/файл
	int rename(const char* old_name, const char* new_name);

	// Изменить владельца директории/файла
	int chown(int uid, const char* name);

	// Изменить права на директорию/файл
	int chmod(int access, const char* name);

	// Добавить нового пользователя
	int addUser(const char* login, const char* password);
	
	// Удалить пользователя
	int removeUser(const char* login);

	// Вернуть номер инода по имени
	long getInodeNo(const char* name);

	// Вернуть ID текущего пользователя
	VFS_WORD getCurrentUserID();

	// Вернуть Login текущего пользователя
	char* getCurrentUserLogin();

	// Вернуть размер блока
	VFS_WORD getBlockSize();

	// Вывести информацию о суперблоке
	void printStat();

	// Создание файла с VFS
	static int makeVFS(std::string filename, VFS_DWORD size, const char* rootPassword);

};
#endif