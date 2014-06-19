#ifndef VFS_DIRECTORY_ENTRY_H
#define VFS_DIRECTORY_ENTRY_H

#include "VFS_Types.h"

#define VFS_NAME_LEN 64
#define VFS_DIRECTORY_ENTRY_SIZE sizeof(struct VFS_Directory_Entry)

// Типы файлов
#define VFS_FT_UNKNOWN	0
#define VFS_FT_REG_FILE 1	// Обычный файл
#define VFS_FT_DIR		2	// Директория
#define VFS_FT_FIFO		3

struct VFS_Directory_Entry
{
	VFS_DWORD inode;
	VFS_WORD rec_len;
	VFS_BYTE name_len;
	VFS_BYTE file_type;
	char name[VFS_NAME_LEN];
};

#endif