#ifndef VFS_H
#define VFS_H

#include <string>
#include <stdio.h>
#include "VFS_Superblock.h"
#include "VFS_Bitmap.h"
#include "VFS_Inode.h"
#include "VFS_Shadow.h"
#include "VFS_Directory_Entry.h"

// ���� � ����� VFS
#define VFS_DISK_PATH	"fs.bin"

// ��������� ����� ����� ������������ ������
#define VFS_BLOCK_OFFSET(block) (VFS_START_SUPERBLOCK + VFS_SUPERBLOCK_SIZE + (superblock->s_first_data_block + block) * getBlockSize())

// ������ ���������� ������� ��� ����������� ��������
#define VFS_ALLOC_SIZE_4_VIEW_DIR	50

// ���� ������ ��� ������� VFS::observeDir
enum TYPE_OF_OBSERVER_SEARCH
{
	// ��� ����� ������?
	OBSERVER_FIND_SAME_ENTRY		= 0x001,	// ����� ������ � ����� �� ������
	OBSERVER_FIND_EMPTY_ENTRY	= 0x002,	// ����� ������ ������
	OBSERVER_FIND_ALL_ENTRIES	= 0x004,		// ����� ��� ������

	// ��� ����� �������?
	OBSERVER_NEED_BLOCK_ENTRY			= 0x100,	// ����� ������� ����
};

// ���������, ������� ���������� ������� VFS::observeDir
struct VFS_Observer_Return
{
	bool				empty;		// ������� ������ ������?
	bool				exist;		// ������� ������ � ����� �� ������?
	VFS_BYTE*			block;		// ��������� �� ���� ������, � ������� ���� ������� ������ ������
	VFS_WORD			block_no;	// ����� ����� � �����, ������� �������� block
	VFS_Directory_Entry*entry;		// ��������� �� ��������� ������
	VFS_DWORD			countFound;	// ���������� ��������� ������� (������������ � OBSERVER_FIND_ALL_ENTRIES)
};

class VFS
{
	VFS_Inode*	curDirInode;	// ���� ������� ����������
	VFS_DWORD	curDirInodeNo;	// ����� ����� ������� ����������
	VFS_WORD	curUserID;		// ID �������� ������������
	char		curUserLogin[VFS_USER_LOGIN_MAX_LEN];		// Login �������� ������������

	// ================================

	VFS_Superblock* superblock;			// ���������
	VFS_Bitmap* inodeBitmap;			// ������� ����� ������
	VFS_Bitmap* blocksBitmap;			// ������� ����� ������
	VFS_Inode*	inodeTable;				// ������� ������
	VFS_BYTE	initialized;			// ���� ������������� (���� =0, �� ������������������)
	int VFS_File;						// ���������� �����

	// ��������� ���������� �� �������� ��������
	void read_root();

	// ������� ����
	VFS_DWORD create_inode(VFS_Inode *inode);
	VFS_DWORD create_inode(VFS_DWORD inode_no, VFS_Inode *inode);

	// ���������� ����
	void free_inode(VFS_DWORD inode_no);

	// ��������� inode
	void read_inode(VFS_DWORD inode_no, VFS_Inode *&inode);

	// ��������� ����
	void read_block(VFS_Inode *inode, VFS_DWORD block, VFS_BYTE *&buffer);

	// �������� ����
	void write_block(VFS_Inode *inode, VFS_DWORD block, VFS_BYTE *buffer);

	// �������� (������) ���� ������
	VFS_DWORD allocate_block();

	// ���������� ����
	void free_block(VFS_DWORD block);

	// ������� ��������� ����������
	void buildEmptyDir(VFS_DWORD currentInodeNo, VFS_DWORD parentInodeNo, VFS_Inode* inode);

	// �������� � ��������� ���� ���������� ���������� (��������� ����������)
	VFS_Directory_Entry* fillDirBlock();

	// ������� ������ ������ ������ � ��������
	VFS_Observer_Return VFS::observeDir(const char* name, VFS_DWORD inode_no, VFS_WORD type);

	// �������, ������� ��������� 
	VFS_BYTE extendInode(VFS_Inode* inode);

public:
	VFS(std::string filename);
	VFS(std::string filename, bool read_root);
	virtual ~VFS();

	// �����
	int login(const char* login, const char* password);

	// �������� ������� �������
	int get_current_directory();

	// �������� ������� ����������
	int change_directory(const char* dirName);

	// ������� �����
	int makeDir(const char* dirName);

	// ������� ����
	int makeFile(const char* fileName, VFS_WORD flag);

	// ������� ����-�����
	int makePipe(const char* fileName);

	// ��������� ������ �� �����
	VFS_DWORD readFile(const char* fileName, void*& data);
	VFS_DWORD readFile(const char* fileName, void*& data, VFS_BYTE force);

	// �������� ������ � ����
	int writeFile(const char* fileName, void* data, VFS_DWORD size);

	// ���������� ����
	int copy(const char* source_name, const char* dest_name);

	// ������� �����/����
	int remove(const char* name, bool recursive);

	// ������������� �����/����
	int rename(const char* old_name, const char* new_name);

	// �������� ��������� ����������/�����
	int chown(int uid, const char* name);

	// �������� ����� �� ����������/����
	int chmod(int access, const char* name);

	// �������� ������ ������������
	int addUser(const char* login, const char* password);
	
	// ������� ������������
	int removeUser(const char* login);

	// ������� ����� ����� �� �����
	long getInodeNo(const char* name);

	// ������� ID �������� ������������
	VFS_WORD getCurrentUserID();

	// ������� Login �������� ������������
	char* getCurrentUserLogin();

	// ������� ������ �����
	VFS_WORD getBlockSize();

	// ������� ���������� � ����������
	void printStat();

	// �������� ����� � VFS
	static int makeVFS(std::string filename, VFS_DWORD size, const char* rootPassword);

};
#endif