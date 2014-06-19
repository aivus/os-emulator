#ifndef VFS_SUPERBLOCK_H
#define VFS_SUPERBLOCK_H

#include "VFS_Types.h"

#define VFS_START_SUPERBLOCK	1024	// Смещение начального адреса суперблока
#define VFS_BLOCK_SIZE			4096	// Размер блока 4Кб
#define VFS_MAGIC				0x2FF2	// Сигнатура файловой системы
#define VFS_VERSION				0x03	// Версия файловой системы
#define VFS_SUPERBLOCK_SIZE		sizeof(struct VFS_Superblock)	// Размер суперблока

struct VFS_Superblock
{
	VFS_DWORD	s_inodes_count;			// Количество инод
	VFS_DWORD	s_blocks_count;			// Количество блоков
	VFS_DWORD	s_first_data_block;		// Адрес первого блока данных
	VFS_WORD	s_block_size;			// Размер блока
	VFS_WORD	s_magic;				// Сигнатура FS
	VFS_WORD	s_version;				// Версия файловой системы
	VFS_BYTE	s_reserved[1006];		// Выравниваем до 1024 байт
};

#endif