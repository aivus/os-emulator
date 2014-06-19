#ifndef VFS_SUPERBLOCK_H
#define VFS_SUPERBLOCK_H

#include "VFS_Types.h"

#define VFS_START_SUPERBLOCK	1024	// �������� ���������� ������ ����������
#define VFS_BLOCK_SIZE			4096	// ������ ����� 4��
#define VFS_MAGIC				0x2FF2	// ��������� �������� �������
#define VFS_VERSION				0x03	// ������ �������� �������
#define VFS_SUPERBLOCK_SIZE		sizeof(struct VFS_Superblock)	// ������ ����������

struct VFS_Superblock
{
	VFS_DWORD	s_inodes_count;			// ���������� ����
	VFS_DWORD	s_blocks_count;			// ���������� ������
	VFS_DWORD	s_first_data_block;		// ����� ������� ����� ������
	VFS_WORD	s_block_size;			// ������ �����
	VFS_WORD	s_magic;				// ��������� FS
	VFS_WORD	s_version;				// ������ �������� �������
	VFS_BYTE	s_reserved[1006];		// ����������� �� 1024 ����
};

#endif