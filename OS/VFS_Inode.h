#ifndef VFS_INODE_H
#define VFS_INODE_H

#include "VFS_Types.h"

// Специальные системные (зарезервированные) inode
#define VFS_INODE_ROOT			1	// Корневой inode
#define VFS_INODE_FIRST_FREE	2	// Первый не зарезервированный inode

// Адресация
#define VFS_INODE_BLOCKS		25		// Максимум блоков в иноде

// Определение значений i_mode
#define VFS_IMODE_PIPE		0x4000	// Инод канала
#define VFS_IMODE_FILE		0x2000	// Инод файл
#define VFS_IMODE_DIR		0x1000	// Инод директория

// Права доступа
#define VFS_IMODE_UREAD		0x0020	// User read
#define VFS_IMODE_UWRITE	0x0010	// User write
#define VFS_IMODE_UEXEC		0x0008	// User exec
#define VFS_IMODE_UALL		(VFS_IMODE_UREAD | VFS_IMODE_UWRITE | VFS_IMODE_UEXEC)
									// User all permissions

#define VFS_IMODE_OREAD		0x0004	// Other read
#define VFS_IMODE_OWRITE	0x0002	// Other write
#define VFS_IMODE_OEXEC		0x0001	// Other exec
#define VFS_IMODE_OALL		(VFS_IMODE_OREAD | VFS_IMODE_OWRITE | VFS_IMODE_OEXEC)
									// Other all permissions

// Определение значений i_flags
#define VFS_IFLAG_UNDEL_FL	0x00000001

// Размер структуры инода
#define VFS_INODE_SIZE sizeof(struct VFS_Inode)

// Максимальный размер файла
#define VFS_MAX_FILE_SIZE (VFS_INODE_BLOCKS * VFS_BLOCK_SIZE)

struct VFS_Inode
{
	VFS_WORD i_mode;	// Тип (файл или директория) и права
	VFS_WORD i_uid;		// ID владельца
	VFS_DWORD i_size;	// Pазмер файла
	VFS_WORD i_flag;	// Флаг файла
	VFS_DWORD i_blocks;	// Количество занимаемых блоков
	VFS_DWORD i_block[VFS_INODE_BLOCKS];		// Указатели на занимаемые блоки данных
};

#endif