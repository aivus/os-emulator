#pragma warning(disable : 4996)
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include "VFS.h"
#include "VFS_Superblock.h"
#include "VFS_Types.h"
#include "VFS_Inode.h"
#include "VFS_Bitmap.h"
#include "VFS_Shadow.h"
#include "Extra.h"
#include "md5.h"

// �����-��������� ��
#undef _DEBUG

using namespace std;

// ����������� �� ���������
VFS::VFS(string filename)
{
	// TODO: �����������, ��� ������� �������
	this->VFS::VFS(filename, true);
}

// �����������
VFS::VFS(string filename, bool read_root)
{
	// ��������� ����
	VFS_File = open(filename.c_str(), _O_RDWR  | _O_BINARY, _S_IREAD | 
                            _S_IWRITE);

	if (VFS_File == -1)
	{
		string error = "[!] ���������� ������� ���� ����������� �������� ������� " + filename;
		throw runtime_error(error);
	}

	// �������������� ��������� ����������
	superblock = new VFS_Superblock;
	memset(superblock, 0, VFS_SUPERBLOCK_SIZE);
	
	// ��������� � �������� ����������
	_lseek(VFS_File, VFS_START_SUPERBLOCK, SEEK_SET);

	// ������ ���������
	if (_read(VFS_File, superblock, VFS_SUPERBLOCK_SIZE) != VFS_SUPERBLOCK_SIZE)
	{
		string error = "[!] ���� " + filename + " ���������. ���������� ��������� ���������.";
		throw runtime_error(error);
	}

	if (superblock->s_magic != VFS_MAGIC)
	{
		string error = "[!] ���� " + filename + " �� �������� ���������� ������ ����������� �������� �������.";
		throw runtime_error(error);
	}

	if (superblock->s_version != VFS_VERSION)
	{
		string error = "[!] ���� " +  filename + " ����� ���������� ������. " + Extra::convertIntToString(VFS_VERSION) + " �����������, �� " + Extra::convertIntToString(superblock->s_version) + " ����������.";
		throw runtime_error(error);
	}
	
	// ������ ������� ����� ������
	blocksBitmap = new VFS_Bitmap(superblock->s_blocks_count);
	if (_read(VFS_File, blocksBitmap->getBitmap(), blocksBitmap->getSize()) != blocksBitmap->getSize())
	{
		string error = "[!] ���� " + filename + " ���������. ���������� ��������� ������� ����� ������.";
		throw runtime_error(error);
	}

	// ������ ������� ����� ������
	inodeBitmap = new VFS_Bitmap(superblock->s_inodes_count);
	if (_read(VFS_File, inodeBitmap->getBitmap(), inodeBitmap->getSize()) != inodeBitmap->getSize())
	{
		string error = "[!] ���� " + filename + " ���������. ���������� ��������� ������� ����� ������.";
		throw runtime_error(error);
	}

	// ������ ������� ������
	inodeTable = new VFS_Inode[superblock->s_inodes_count];
	memset(inodeTable, 0x00, superblock->s_inodes_count * VFS_INODE_SIZE);
	if (_read(VFS_File, inodeTable, VFS_INODE_SIZE * superblock->s_inodes_count) != VFS_INODE_SIZE * superblock->s_inodes_count)
	{
		string error = "[!] ���� " + filename + " ���������. ���������� ��������� ������� ������.";
		throw runtime_error(error);
	}

	if (read_root)
	{
		this->read_root();
	}

	// TODO: �������� �� �������� ID ������������
	curUserID = (VFS_WORD)-1;
	memset(curUserLogin, 0, VFS_USER_LOGIN_MAX_LEN);
	memcpy(curUserLogin, "nobody", 7);

}

// ��������� ���������� �� �������� ��������
void VFS::read_root()
{
	// ��������� ������ ��� ��������� (��������) ����������
	read_inode(VFS_INODE_ROOT, curDirInode);
	curDirInodeNo = VFS_INODE_ROOT;
}

// ����������
VFS::~VFS()
{
	// ��������� �� �������� ���������� � ���������� ������
	_lseek(VFS_File, VFS_START_SUPERBLOCK, SEEK_SET);
	_write(VFS_File, superblock, VFS_SUPERBLOCK_SIZE);
	
	// ���������� block bitmap
	_write(VFS_File, blocksBitmap->getBitmap(), sizeof(VFS_BYTE) * blocksBitmap->getSize());

	// ���������� inode bitmap
	_write(VFS_File, inodeBitmap->getBitmap(), sizeof(VFS_BYTE) * inodeBitmap->getSize());

	// ���������� inode table	
	_write(VFS_File, inodeTable, VFS_INODE_SIZE * superblock->s_inodes_count);
	
	// ������� ��������� �� ������
	delete superblock;
	delete blocksBitmap;
	delete inodeBitmap;
	delete[] inodeTable;

	// ��������� ���� � VFS
	_close(VFS_File);
}

int VFS::login(const char* login, const char* password)
{
	// ���������� ������������ ����
	VFS_DWORD originalInodeNo = curDirInodeNo;
	VFS_Inode* originalInode = curDirInode;

	// ��������� � ������
	curDirInodeNo = VFS_INODE_ROOT;
	read_inode(curDirInodeNo, curDirInode);

	void* data;
	VFS_Shadow* shadow;
	VFS_WORD prev_uid = 0;	// UID ���������� ������

	readFile("shadow", data, 1);
	shadow = (VFS_Shadow*) data;

	// �������� ������
	md5::md5_context CTX;
	md5::md5_starts(&CTX);
	md5::md5_update(&CTX, (unsigned char*)password, strlen(password));
	md5::md5_finish(&CTX, (unsigned char*)password);

	VFS_BYTE loop = 1;
	while(strcmp(shadow->login, "") != 0 && strcmp(shadow->password, "") != 0 && loop == 1)
	{
		// ��������� ����� � ������
		if (strcmp(shadow->login, login) == 0 && strncmp(shadow->password, password, VFS_USER_PASS_MAX_LEN) == 0 && shadow->active !=0)
		{
			curUserID = shadow->uid;
			memset(curUserLogin, 0, strlen(curUserLogin));
			memcpy(curUserLogin, login, strlen(login) + 1);
			loop = 0;
		}
		else
		{
			// �������� � ��������� ������
			shadow++;
			prev_uid = shadow->uid;
		}
	}

	delete[] data;

	// ��������������� ������������ ����
	curDirInodeNo = originalInodeNo;
	curDirInode = originalInode;

	return (loop == 1)? -1 : 0;
}

// �������� ������ ������������
// ���������� UID
int VFS::addUser(const char* login, const char* password)
{
	if (curUserID != 0)
		return -2;

	if (strcmp(login, "") == 0 || strcmp(password, "") == 0)
		return -1;

	if (strlen(login)>VFS_USER_LOGIN_MAX_LEN)
		return -4;

	if (strlen(password)>VFS_USER_PASS_MAX_LEN)
		return -5;


	// ���������� ������������ ����
	VFS_DWORD originalInodeNo = curDirInodeNo;
	VFS_Inode* originalInode = curDirInode;

	// ��������� � ������
	curDirInodeNo = VFS_INODE_ROOT;
	read_inode(curDirInodeNo, curDirInode);

	void* data;
	VFS_Shadow* shadow;

	VFS_DWORD real_file_size = readFile("shadow", data, 1);

	shadow = (VFS_Shadow*)data;
	VFS_WORD uid = (VFS_WORD)-1;
	VFS_DWORD shadow_file_size = 0;

	// ��������� ��� ���. �����
	while(shadow->uid == (VFS_WORD)(uid + 1))
	{
		if ((strcmp(shadow->login, login) == 0) && shadow->active != 0)
		{
			delete[] data;
			return -3;
		}
		else
		{
			uid = shadow->uid;
		}

		shadow_file_size += sizeof(VFS_Shadow);
		shadow++;
	}

	// ���� ���� ������������� ����� - ��������� ���
	if (shadow_file_size == real_file_size)
	{
		VFS_DWORD new_size = (real_file_size * 2 > VFS_MAX_FILE_SIZE)? VFS_MAX_FILE_SIZE : real_file_size * 2;
		void* new_data = new VFS_BYTE[new_size];
		memset(new_data, 0, new_size);
		memcpy(new_data, data, real_file_size);
		real_file_size = new_size;
		delete[] data;
		data = new_data;
	}

	// ��������� ������������
	shadow = (VFS_Shadow*)data;
	uid = (VFS_WORD)-1;

	// ������� ������ ����
	while(shadow->uid == (VFS_WORD)(uid + 1) && shadow->active != 0){ uid = shadow->uid; shadow++;}

	// ���� ������� ������������ ���-�� �������������,
	// ������� ����� �������� � ����� ��
	if ((VFS_WORD)(uid + 1) >= VFS_MAX_FILE_SIZE / sizeof (VFS_Shadow))
	{
		delete[] data;
		return -6;
	}

	// ��������� ������
	uid++;
	memset(shadow->login, 0, VFS_USER_LOGIN_MAX_LEN);
	memset(shadow->password, 0, VFS_USER_PASS_MAX_LEN);
	memcpy(shadow->login, login, strlen(login));
	md5::md5_context CTX;
	md5::md5_starts(&CTX);
	md5::md5_update(&CTX, (unsigned char*)password, strlen(password));
	md5::md5_finish(&CTX, (unsigned char*)shadow->password);
	//memcpy(shadow->password, password, strlen(password));
	shadow->active = 1;
	shadow->uid = uid;

	writeFile("shadow", data, real_file_size);

	// ��������������� ������������ ����
	curDirInodeNo = originalInodeNo;
	curDirInode = originalInode;

	delete[] data;
	return uid;
}

int VFS::removeUser(const char* login)
{
	// ������� ������� ������������ �� root'��
	if (curUserID != 0)
	{
		return -2;
	}

	// ������� ������� ������ ������
	if (strcmp(login, "") == 0)
	{
		return -1;
	}

	// ������� ������� �����������������
	if (strcmp(login, "root") == 0)
	{
		return -4;
	}

	// ���������� ������������ ����
	VFS_DWORD originalInodeNo = curDirInodeNo;
	VFS_Inode* originalInode = curDirInode;

	// ��������� � ������
	curDirInodeNo = VFS_INODE_ROOT;
	read_inode(curDirInodeNo, curDirInode);

	void* data;
	VFS_Shadow* shadow;
	VFS_DWORD size = readFile("shadow", data, 1);

	shadow = (VFS_Shadow*)data;
	VFS_BYTE loop = 1;

	while(strcmp(shadow->login, "") != 0 && strcmp(shadow->password, "") != 0 && loop == 1)
	{
		if (strcmp(shadow->login, login) == 0 && shadow->active != 0)
		{
			shadow->active = 0;
			loop = 0;
		}

		shadow++;
	}

	writeFile("shadow", data, size);

	// ��������������� ������������ ����
	curDirInodeNo = originalInodeNo;
	curDirInode = originalInode;

	delete[] data;
	return (loop == 1)? -3 : 0;
}

// �������� ������� �������
int VFS::get_current_directory()
{
	// �������� �� ����� ��������� ����������� ������� ����������
	if (!(curDirInode->i_uid == curUserID && (curDirInode->i_mode & VFS_IMODE_UREAD) == VFS_IMODE_UREAD) &&
		!(curDirInode->i_uid != curUserID && (curDirInode->i_mode & VFS_IMODE_OREAD) == VFS_IMODE_OREAD))
	{
		return -1;
	}

	VFS_Observer_Return dir = observeDir(NULL, curDirInodeNo, OBSERVER_FIND_ALL_ENTRIES);

	cout << "������\t\t��������\t������\t���" << endl;

	for (VFS_DWORD i = 0; i < dir.countFound; i++)
	{

		// ������ �����
		VFS_Inode* inode;
		read_inode(dir.entry[i].inode, inode);

		// ���� ���� �������
		if ((inode->i_flag & VFS_IFLAG_UNDEL_FL) == VFS_IFLAG_UNDEL_FL)
			cout << "*";
		else
			cout << "-";

		// ������� ��� ������
		if ((inode->i_mode & VFS_IMODE_FILE) == VFS_IMODE_FILE)
			cout << "f";
		else if ((inode->i_mode & VFS_IMODE_DIR) == VFS_IMODE_DIR)
			cout << "d";
		else if ((inode->i_mode & VFS_IMODE_PIPE) == VFS_IMODE_PIPE)
			cout << "p";
		else
			cout << "?";

		// ������� ����� ������� ��� ���������
		if ((inode->i_mode & VFS_IMODE_UREAD) == VFS_IMODE_UREAD)
			cout << "r";
		else
			cout << "-";
		if ((inode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE)
			cout << "w";
		else
			cout << "-";
		if ((inode->i_mode & VFS_IMODE_UEXEC) == VFS_IMODE_UEXEC)
			cout << "x";
		else
			cout << "-";

		// ������� ����� ������� ��� ������
		if ((inode->i_mode & VFS_IMODE_OREAD) == VFS_IMODE_OREAD)
			cout << "r";
		else
			cout << "-";
		if ((inode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE)
			cout << "w";
		else
			cout << "-";
		if ((inode->i_mode & VFS_IMODE_OEXEC) == VFS_IMODE_OEXEC)
			cout << "x";
		else
			cout << "-";

		cout << "\t";

		// ������� ���������
		cout << inode->i_uid << "\t\t";

		// ������� ������
		cout << inode->i_size << "\t";

		// ������� ���
		cout << dir.entry[i].name << endl;
	}

	//������ ������
	delete[] dir.entry;

	return 0;
}

// �������� ������� ����������
int VFS::change_directory(const char* dirName)
{
	VFS_Observer_Return observe;

	// ���� ������ � ����� �� ������
	observe = observeDir(dirName, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� �� ���������� ����� ����������
	if (!observe.exist || observe.entry->file_type != VFS_FT_DIR)
	{
		delete[] observe.block;
		return -1;
	}

	VFS_Inode* inode;
	read_inode(observe.entry->inode, inode);

	// �������� �� ����� �������� � ��������� ����������
	if (!(inode->i_uid == curUserID && (inode->i_mode & VFS_IMODE_UREAD) == VFS_IMODE_UREAD) &&
		!(inode->i_uid != curUserID && (inode->i_mode & VFS_IMODE_OREAD) == VFS_IMODE_OREAD))
	{
		delete[] observe.block;
		return -2;
	}

	// ������ ������� �����
	curDirInodeNo = observe.entry->inode;
	curDirInode = inode;

	delete[] observe.block;

	return 0;
}

// ��������� inode
// inode_no - ����� ����� ��� ������
// inode - ������ �� ��������� ����� (�������� ��������, ����������� ��������������� ������ ��� NULL)
void VFS::read_inode(VFS_DWORD inode_no, VFS_Inode *&inode)
{
	// ���������, ����� �� �������� ����
	if (!inodeBitmap->testBit(inode_no))
	{
		// ���� ���� �� ����� - ������
		cerr << "[!] ������� ������ ������� �����!" << endl;
		return;
	}

	if (inode_no < VFS_INODE_ROOT || inode_no > (VFS_INODE_SIZE * superblock->s_inodes_count))
	{
		inode = NULL;
		cerr << "[!] ������� ������ ������������� ����� no. " << inode_no << endl;
		return;
	}

	inode = &inodeTable[inode_no];

#ifdef _DEBUG
	cout << "[D] ������ ����� no. " << inode_no << "." << endl;
	cout << "[D] i_size = " << inode->i_size << endl;
#endif
}

// ������� ���� (���� ���������� ������ ������������ ����� �����)
// inode_no - ����� ������������ �����
// inode - ��������� �� ����������� ���������, ������� ����� ��������
VFS_DWORD VFS::create_inode(VFS_DWORD inode_no, VFS_Inode *inode)
{
	// ���� ��� �����
	if (inodeBitmap->testBit(inode_no))
	{
		return 0;
	}

	inodeBitmap->SetBit(inode_no, true);
	inodeTable[inode_no] = *inode;

	#ifdef _DEBUG
	cout << "[D] ������ ��������� ���� no." << inode_no << endl;
	#endif


	return inode_no;
}

// �������� inode (����� ���������� ����� � ������� ���)
// inode - ��������� �� ����������� ���������, ������� ����� ��������
VFS_DWORD VFS::create_inode(VFS_Inode *inode)
{
	VFS_DWORD inode_no;

	try {
		// ����� ���������� ����� � ������� �����
		inode_no = inodeBitmap->searchFree();
	}
	catch(VFS_BYTE)
	{
		cerr << "[!] ��������� ���� �� ������. ���������� ������� ����!";
		return 0;
	}

	return this->create_inode(inode_no, inode);
}

// ���������� ����
void VFS::free_inode(VFS_DWORD inode_no)
{
	#ifdef _DEBUG
	cout << "[D] ����������� ���� no." << inode_no << endl;
	#endif

	inodeBitmap->SetBit(inode_no, false);
}

// ��������� ����
// inode - ��������� �� ��������� �����
// block - ����� ����� �� inode->i_blocks.
// buffer - �������� �����.
void VFS::read_block(VFS_Inode *inode, VFS_DWORD block, VFS_BYTE *&buffer)
{
	// ��������, ����� �� ����
	if (!blocksBitmap->testBit(inode->i_block[block]))
	{
		cerr << "[!] ������� ������ ���������� �����!";
		return;
	}

	buffer = new VFS_BYTE[getBlockSize()];
	_lseek(VFS_File, VFS_BLOCK_OFFSET(inode->i_block[block]), SEEK_SET);
	if(_read(VFS_File, buffer, getBlockSize())<0)
	{
		cerr << "[!] ������ ��� ������ ���������� �����!" << endl;
		delete[] buffer;
		return;
	}

}

// �������� ����
// inode - ��������� �� ��������� �����
// block - ����� ����� �� inode->i_blocks.
// buffer - �������� �����. ������, ������� ����� ��������. (����� ������ ���� �� ������ ������� �����)
void VFS::write_block(VFS_Inode *inode, VFS_DWORD block, VFS_BYTE *buffer)
{
	// ��������, ����� �� ����
	if (!blocksBitmap->testBit(inode->i_block[block]))
	{
		cerr << "[!] ������� �������� ���������� � ��������� ����!";
		return;
	}

	_lseek(VFS_File, VFS_BLOCK_OFFSET(inode->i_block[block]), SEEK_SET);
	if(_write(VFS_File, buffer, getBlockSize()) < 0)
	{
		cerr << "[!] ������ ��� ������ ���������� � ����!" << endl;
		return;
	}

}

// �������� (������) ���� ������
VFS_DWORD VFS::allocate_block()
{
	// ���� ��������� ���� � �������� ���
	VFS_DWORD block_no = blocksBitmap->searchFree();
	blocksBitmap->SetBit(block_no, true);

	#ifdef _DEBUG
	cout << "[D] ������ ��������� ���� no." << block_no << endl;
	#endif

	return block_no;
}

void VFS::free_block(VFS_DWORD block_no)
{
	#ifdef _DEBUG
	cout << "[D] ���������� ���� no." << block_no << endl;
	#endif

	blocksBitmap->SetBit(block_no, false);
}

// ������� ������(������) ������ ������ � ��������
// name - ��� ��� ������
// type - ��� ������
VFS_Observer_Return VFS::observeDir(const char* name, VFS_DWORD inode_no, VFS_WORD type)
{
	// ��������� ������������ ���������
	VFS_Observer_Return ret;

	bool retExist = false;			// ����, ���� �� ����� � ����� �� ������?
	bool retFoundEmpty = false;		// ����, ������ �� ��������� ����?

	VFS_Directory_Entry *entry;
	VFS_Inode* inode;
	VFS_BYTE *block = NULL;
	VFS_WORD dirblock_no = 0;
	VFS_DWORD size = 0;		// ������ ����������� ������

	// ���������� ��� OBSERVER_FIND_ALL_ENTRIES
	// ��������� �� ������ �������� VFS_Directory_Entry
	VFS_Directory_Entry* viewDir = NULL;
	// ���������� ���������� ������� viewDir
	VFS_DWORD extendCountViewDir = 0;
	// ���������� �������
	VFS_DWORD countEntries = 0;

	//���� ���������� �������, �� �������� N �������
	if ((type & OBSERVER_FIND_ALL_ENTRIES) == OBSERVER_FIND_ALL_ENTRIES)
	{
		viewDir = new VFS_Directory_Entry[VFS_ALLOC_SIZE_4_VIEW_DIR];
	}

	read_inode(inode_no, inode);

	while(dirblock_no < inode->i_blocks && !retFoundEmpty && !retExist)
	{
		// ������ ������ � ����������� ����
		delete[] block;

		read_block(inode, dirblock_no, block);
		
		entry = (VFS_Directory_Entry*) block;
		size = getBlockSize() * dirblock_no;
		
		while(size < inode->i_size && !retFoundEmpty && !retExist && entry->rec_len == VFS_DIRECTORY_ENTRY_SIZE)
		{
			// ��������� ��� ����� (���� �� ��� ����� � ����� ������?)
			if ((type & OBSERVER_FIND_SAME_ENTRY) == OBSERVER_FIND_SAME_ENTRY && strcmp(name,entry->name)==0 && strlen(entry->name) == entry->name_len)
				retExist = true;

			// ����� ������ ������
			else if ((type & OBSERVER_FIND_EMPTY_ENTRY) == OBSERVER_FIND_EMPTY_ENTRY && entry->file_type == VFS_FT_UNKNOWN)
				retFoundEmpty = true;

			// ���� ������� ������ �� �������� - ��������� ������
			else
			{
				if ((type & OBSERVER_FIND_ALL_ENTRIES) == OBSERVER_FIND_ALL_ENTRIES && entry->name_len != 0)
				{
					// ���� ������ ���������� - ���������
					if (countEntries >= VFS_ALLOC_SIZE_4_VIEW_DIR * (extendCountViewDir + 1))
					{
						VFS_Directory_Entry* temp = new VFS_Directory_Entry[countEntries*2];
						memset(temp, 0, VFS_DIRECTORY_ENTRY_SIZE * countEntries * 2);
						memcpy(temp, viewDir, VFS_DIRECTORY_ENTRY_SIZE * countEntries);

						delete[] viewDir;
						viewDir = temp;
						extendCountViewDir++;
					}

					viewDir[countEntries] = *entry;
					countEntries++;
				}

				entry++;
				size += entry->rec_len;
			}
		}
		dirblock_no++;
	}

	// ������������ ����� �����
	if (retExist || retFoundEmpty)
		dirblock_no--;

	// ����� �� ����?
	if ((type & OBSERVER_NEED_BLOCK_ENTRY) == OBSERVER_NEED_BLOCK_ENTRY)
	{
		ret.block = block;
		ret.entry = entry;
		ret.block_no = dirblock_no;
	}
	else
	{
		delete[] block;
		block = NULL;
		entry = NULL;
	}

	if ((type & OBSERVER_FIND_ALL_ENTRIES) == OBSERVER_FIND_ALL_ENTRIES)
	{
		ret.entry = viewDir;
		ret.countFound = countEntries;
	}

	// ��������� ������������ ���������
	ret.empty = retFoundEmpty;
	ret.exist = retExist;

	return ret;
}

// ���������� �����(��������� ������ ����� ������)
// inode - ���� ������������ �����
VFS_BYTE VFS::extendInode(VFS_Inode* inode)
{
		#ifdef _DEBUG
		cout << "[D] ���� ��������!" << endl;
		cout << "[D] ������� ������ ����� ����!" << endl;
		#endif

		VFS_DWORD newblock_no = allocate_block();
		
		// ���������� �� ����
		inode->i_blocks++;

		// ���� ����������� �����
		if (inode->i_blocks > VFS_INODE_BLOCKS)
		{
			cerr << "[!] ������! ����������� ����� ������!" << endl;

			inode->i_blocks--;
			free_block(newblock_no);
			return 1;
		}

		// ����������� ������ ������
		inode->i_size += getBlockSize();
		inode->i_block[inode->i_blocks-1] = newblock_no;

		return 0;
}

// ������� �����
int VFS::makeDir(const char* dirName)
{
	// �������� �� ����� �������� ����� � ���. ����������
	if (!(curDirInode->i_uid == curUserID && (curDirInode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE) &&
		!(curDirInode->i_uid != curUserID && (curDirInode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE))
	{
		return -4;
	}

	// ��������� ����� �����, ���� ������ ��� ��������� - �����
	if (strlen(dirName)>VFS_NAME_LEN)
		return -2;

	// �������� ���� ������ ��� ����������� �����
	VFS_DWORD block_no = allocate_block();

	// ��������� � ������� ���� ��� ����������� �����
	VFS_Inode *inode = new VFS_Inode();
	inode->i_mode = VFS_IMODE_DIR | VFS_IMODE_UALL;
	inode->i_size = getBlockSize();
	inode->i_block[0] = block_no;
	inode->i_blocks = 1;
	inode->i_flag = 0;
	inode->i_uid = curUserID;
	VFS_DWORD inode_no = this->create_inode(inode);

	// ������� ������ ���������
	buildEmptyDir(inode_no, curDirInodeNo, inode);

	// � ������� ���������� ������� ������ �� ����� ��������� ����������
	VFS_Observer_Return some = observeDir(dirName, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY);

	// ������� ����� � ����� �� ������
	if (some.exist)
	{

		#ifdef _DEBUG
		cout << "[D] ������� ����� � ����� �� ������." << endl;
		#endif

		// ����������� ������� ����� ����
		free_inode(inode_no);

		// ����������� ������� ����� ����
		free_block(block_no);

		// ������ ������
		delete inode;

		return -1;
	}

	VFS_Observer_Return emptyObserver = observeDir(NULL, curDirInodeNo, OBSERVER_FIND_EMPTY_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� ������ ������ (VFS_Directory_Entry) �� ������� � ������� ��������, �� ��������� ���.
	if (!emptyObserver.empty)
	{
		delete[] emptyObserver.block;

		// ���� ������� extendInode ������� 1, ������ ��������� ����� � ����� �����������.
		if (extendInode(curDirInode) == 1)
		{
			free_block(block_no);
			free_inode(inode_no);
			delete inode;
			return -3;
		}

		emptyObserver.block = (VFS_BYTE*)fillDirBlock();
		emptyObserver.entry = (VFS_Directory_Entry*) emptyObserver.block;
	}

	// ���������� ������
	emptyObserver.entry->file_type = VFS_FT_DIR;
	emptyObserver.entry->inode = inode_no;
	emptyObserver.entry->name_len = strlen(dirName);
	memcpy(emptyObserver.entry->name, dirName, emptyObserver.entry->name_len);

	// ��������� ����
	write_block(curDirInode, emptyObserver.block_no, emptyObserver.block);

	// ������ ������
	delete inode;
	delete[] emptyObserver.block;

	return 0;
}

int VFS::makeFile(const char* fileName, VFS_WORD flag)
{
	// �������� �� ����� �������� ����� � ���. ����������
	if (!(curDirInode->i_uid == curUserID && (curDirInode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE) &&
		!(curDirInode->i_uid != curUserID && (curDirInode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE) && curUserID != 0)
	{
		return -4;
	}

	// ��������� ����� �����, ���� ������ ��� ��������� - �����
	if (strlen(fileName)>VFS_NAME_LEN)
		return -2;

	// ����� ������ � ����� �� ������
	VFS_Observer_Return some = observeDir(fileName, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY);
	if (some.exist)
	{
		return -1;
	}

	// ������� ���� �����
	VFS_Inode* inode = new VFS_Inode();
	inode->i_blocks = 1;
	inode->i_block[0] = allocate_block();
	inode->i_mode = VFS_IMODE_FILE | VFS_IMODE_UALL;
	inode->i_flag = flag;
	inode->i_size = getBlockSize();
	inode->i_uid = curUserID;
	VFS_DWORD inode_no = create_inode(inode);

	// �������� ������������ �����
	VFS_Observer_Return emptyEntry = observeDir(NULL, curDirInodeNo, OBSERVER_FIND_EMPTY_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� ������ ������ (VFS_Directory_Entry) �� ������� � ������� ��������, �� ��������� ���.
	if (!emptyEntry.empty)
	{
		delete[] emptyEntry.block;

		// ���� ������� extendInode ������� 1, ������ ��������� ����� � ����� �����������.
		if (extendInode(curDirInode) == 1)
		{
			free_block(inode->i_block[0]);
			free_inode(inode_no);
			delete inode;
			return -3;
		}

		emptyEntry.block = (VFS_BYTE*)fillDirBlock();
		emptyEntry.entry = (VFS_Directory_Entry*) emptyEntry.block;
	}

	// �������� ������������ ����������
	emptyEntry.entry->file_type = VFS_FT_REG_FILE;
	emptyEntry.entry->inode = inode_no;
	emptyEntry.entry->name_len = strlen(fileName);
	memcpy(emptyEntry.entry->name, fileName, emptyEntry.entry->name_len);

	// ���������� ����
	write_block(curDirInode, emptyEntry.block_no, emptyEntry.block);

	// ������ ������
	delete inode;
	delete[] emptyEntry.block;

	return 0;
}

int VFS::makePipe(const char* fileName)
{
	// �������� �� ����� �������� ����� � ���. ����������
	if (!(curDirInode->i_uid == curUserID && (curDirInode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE) &&
		!(curDirInode->i_uid != curUserID && (curDirInode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE) && curUserID != 0)
	{
		return -4;
	}

	// ��������� ����� �����, ���� ������ ��� ��������� - �����
	if (strlen(fileName)>VFS_NAME_LEN)
		return -2;

	// ����� ������ � ����� �� ������
	VFS_Observer_Return some = observeDir(fileName, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY);
	if (some.exist)
	{
		return -1;
	}

	// ������� ���� �����
	VFS_Inode* inode = new VFS_Inode();
	inode->i_blocks = 0;
	inode->i_mode = VFS_IMODE_PIPE | VFS_IMODE_UALL | VFS_IMODE_OREAD;
	inode->i_flag = 0;
	inode->i_size = 0;
	inode->i_uid = curUserID;
	VFS_DWORD inode_no = create_inode(inode);

	// �������� ������������ �����
	VFS_Observer_Return emptyEntry = observeDir(NULL, curDirInodeNo, OBSERVER_FIND_EMPTY_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� ������ ������ (VFS_Directory_Entry) �� ������� � ������� ��������, �� ��������� ���.
	if (!emptyEntry.empty)
	{
		delete[] emptyEntry.block;

		// ���� ������� extendInode ������� 1, ������ ��������� ����� � ����� �����������.
		if (extendInode(curDirInode) == 1)
		{
			free_block(inode->i_block[0]);
			free_inode(inode_no);
			delete inode;
			return -3;
		}

		emptyEntry.block = (VFS_BYTE*)fillDirBlock();
		emptyEntry.entry = (VFS_Directory_Entry*) emptyEntry.block;
	}

	// �������� ������������ ����������
	emptyEntry.entry->file_type = VFS_FT_FIFO;
	emptyEntry.entry->inode = inode_no;
	emptyEntry.entry->name_len = strlen(fileName);
	memcpy(emptyEntry.entry->name, fileName, emptyEntry.entry->name_len);

	// ���������� ����
	write_block(curDirInode, emptyEntry.block_no, emptyEntry.block);

	// ������ ������
	delete inode;
	delete[] emptyEntry.block;

	return 0;
}

// ��������� ������ �� �����
// ���������� ���-�� ����������� ���������� ��� �������� <0 � ������ ������.
VFS_DWORD VFS::readFile(const char* fileName, void*& data)
{
	return readFile(fileName, data, 0);
}

VFS_DWORD VFS::readFile(const char* fileName, void*& data , VFS_BYTE force)
{
	VFS_Observer_Return observer = observeDir(fileName, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� �� ������� ������ - ������
	if (!observer.exist)
	{
		delete[] observer.block;
		return -2;
	}

	// ���� ������� ������ - �� ����
	if (observer.entry->file_type != VFS_FT_REG_FILE && observer.entry->file_type != VFS_FT_FIFO)
	{
		delete[] observer.block;
		return -1;
	}

	VFS_Inode* inode;

	// ������ �����
	read_inode(observer.entry->inode, inode);

	// �������� �� ����� ������ �����
	if (!(inode->i_uid == curUserID && (inode->i_mode & VFS_IMODE_UREAD) == VFS_IMODE_UREAD) &&
		!(inode->i_uid != curUserID && (inode->i_mode & VFS_IMODE_OREAD) == VFS_IMODE_OREAD) && force == 0)
	{
		delete[] observer.block;
		return -3;
	}

	// ���� ������������ ���� - �����
	if (observer.entry->file_type == VFS_FT_FIFO)
	{
		delete[] observer.block;
		return -1000;
	}


	VFS_BYTE* ret = new VFS_BYTE[inode->i_size];
	VFS_BYTE* pointer = ret;		// ��������� �� ������ ��������� ����
	VFS_BYTE* buffer = NULL;

	for (VFS_DWORD i = 0; i < inode->i_blocks; i++)
	{
		// ������ ���� ������
		read_block(inode, i, buffer);
		memcpy(pointer, buffer, getBlockSize());
		pointer += getBlockSize();
		delete[] buffer;
	}

	data = ret;
	delete[] observer.block;
	return inode->i_size;
}

// �������� ������ � ����
int VFS::writeFile(const char* fileName, void* data, VFS_DWORD size)
{
	// ���� ������ ������������� ����� ������ �������������
	if (size / getBlockSize() > VFS_INODE_BLOCKS)
		return -3;

	VFS_Observer_Return observer = observeDir(fileName, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� �� ������� ������ - ������
	if (!observer.exist)
	{
		delete[] observer.block;
		return -2;
	}

	// ���� ��������� ������ - �� ����
	if (observer.entry->file_type != VFS_FT_REG_FILE && observer.entry->file_type != VFS_FT_FIFO)
	{
		delete[] observer.block;
		return -1;
	}

	VFS_Inode* inode;

	// ������ �����
	read_inode(observer.entry->inode, inode);

	// �������� �� ����� ������ � ����
	if (!(inode->i_uid == curUserID && (inode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE) &&
		!(inode->i_uid != curUserID && (inode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE))
	{
		delete[] observer.block;
		return -4;
	}

	// ���� ������������ ���� - �����
	if (observer.entry->file_type == VFS_FT_FIFO)
	{
		delete[] observer.block;
		return -1000;
	}


	VFS_BYTE* pointer = (VFS_BYTE*)data;
	VFS_BYTE* mem = NULL;
	for (VFS_DWORD i = 0; i <= (size - 1) / getBlockSize(); i++)
	{
		// ���� ����������� ���������� ����� - �������� �����, ���� �� ������� - ���������� NULL
		if (i == inode->i_blocks && extendInode(inode) == 1)
		{
			return (i - 1) * getBlockSize();
		}

		if (i == size / getBlockSize())
		{
			mem = new VFS_BYTE[getBlockSize()];
			memset(mem, 0, getBlockSize());
			memcpy(mem, pointer, size % getBlockSize());
			write_block(inode, i, mem);
			delete[] mem;
			mem = NULL;
		}
		else
			write_block(inode, i, pointer);

		pointer += getBlockSize();
	}

	// ������ ������ �����
	inode->i_size = ((size - 1) / getBlockSize() + 1) * getBlockSize();

	delete[] observer.block;
	return 0;
}

// ���������� ����
int VFS::copy(const char* source_name, const char* dest_name)
{
	// ���� �������� ����������� "." ��� ".." - �������� �����.
	if (strcmp(source_name, ".") == 0 || strcmp(source_name, "..") == 0 || strlen(source_name) == 0 ||
		strcmp(dest_name, ".") == 0 || strcmp(dest_name, "..") == 0 || strlen(dest_name) == 0)
		return -1;

	// ��������� ����� �����, ���� ������ ��� ��������� - �����
	if (strlen(source_name) > VFS_NAME_LEN || strlen(dest_name) > VFS_NAME_LEN)
		return -2;

	// ���������, ���� �� ������ � ������ dest_name, ���� ��� - ������.
	VFS_Observer_Return observerSource = observeDir(source_name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);
	if (!observerSource.exist)
	{
		delete[] observerSource.block;
		return -3;
	}

	VFS_Inode* source_inode;
	// ������ source-�����
	read_inode(observerSource.entry->inode, source_inode);

	// �������� �� ����� ��������
	if (!(source_inode->i_uid == curUserID && (source_inode->i_mode & VFS_IMODE_UREAD) == VFS_IMODE_UREAD) &&
		!(source_inode->i_uid != curUserID && (source_inode->i_mode & VFS_IMODE_OREAD) == VFS_IMODE_OREAD))
	{
		delete[] observerSource.block;
		return -7;
	}

	// ���������� ����� ������ ����.
	if (observerSource.entry->file_type != VFS_FT_REG_FILE)
	{
		delete[] observerSource.block;
		return -5;
	}

	// ���������, ���� �� ������ � ������ dest_name, ���� ���� - ������.
	VFS_Observer_Return observerDest = observeDir(dest_name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY);
	if(observerDest.exist)
	{
		delete[] observerSource.block;
		return -4;
	}

	VFS_Inode* dest_inode = new VFS_Inode();

	// �������� source-����� � dest-�����.
	memcpy(dest_inode, source_inode, VFS_INODE_SIZE);

	// ������ dest inode
	dest_inode->i_uid = curUserID;

	// ������� dest �����
	VFS_DWORD dest_inode_no = create_inode(dest_inode);

	// �������� ����� ����� ������, ��� dest � �������� � ��� ������
	for (VFS_WORD i = 0; i < source_inode->i_blocks; i++)
	{
		VFS_BYTE* data;
		VFS_DWORD block_no = allocate_block();
		read_block(source_inode, i, data);
		write_block(dest_inode, i, data);

		delete[] data;
	}

	// ���� ������ ������ � ������� ��������
	VFS_Observer_Return emptyEntry = observeDir(NULL, curDirInodeNo, OBSERVER_FIND_EMPTY_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� ������ �� ������� - ��������� ����
	if (!emptyEntry.empty)
	{
		delete[] emptyEntry.block;

		// ���� ������� extendInode ������� 1, ������ ��������� ����� � ����� �����������.
		if (extendInode(curDirInode) == 1)
		{
			for (VFS_WORD i = 0; i < dest_inode->i_blocks; i++)
			{
				free_block(dest_inode->i_block[i]);
			}

			delete dest_inode;
			delete[] observerSource.block;
			free_inode(dest_inode_no);
			return -6;
		}

		emptyEntry.block = (VFS_BYTE*)fillDirBlock();
		emptyEntry.entry = (VFS_Directory_Entry*) emptyEntry.block;
	}

	// ���������� ������ � ����� ����� � ������� ����������
	emptyEntry.entry->file_type = observerSource.entry->file_type;
	emptyEntry.entry->inode = dest_inode_no;
	emptyEntry.entry->name_len = strlen(dest_name);
	memcpy(emptyEntry.entry->name, dest_name, emptyEntry.entry->name_len);

	write_block(curDirInode, emptyEntry.block_no, emptyEntry.block);

	delete dest_inode;
	delete[] observerSource.block;
	delete[] emptyEntry.block;
	return 0;
}

// ������� �����/����
// name - ��� �����/����� ��� ��������
int VFS::remove(const char* name, bool recursive)
{
	// ���� �������� ������� "." ��� ".." - �������� �����.
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 || strlen(name) == 0)
		return -2;
	
	VFS_Observer_Return observer = observeDir(name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ������� �� �����/���� � ����� ������
	if (!observer.exist)
	{
		delete[] observer.block;
		return -1;
	}

	// �������� �� ����� ��������
	VFS_Inode* inode;
	read_inode(observer.entry->inode, inode);
	if (!(inode->i_uid == curUserID && (inode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE) &&
		!(inode->i_uid != curUserID && (inode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE))
	{
		delete[] observer.block;
		return -4;
	}

	// ���� ���������� ���� �������������
	if (inode->i_flag & VFS_IFLAG_UNDEL_FL)
	{
		delete[] observer.block;
		return -5;
	}

	// ���� ��������� ������ ����� - ��������� ������ �� ���
	if (observer.entry->file_type == VFS_FT_DIR)
	{
		VFS_Observer_Return allentries = observeDir(NULL, observer.entry->inode, OBSERVER_FIND_ALL_ENTRIES);
		
		bool not_permissions = false;

		// ����������� ��������
		if (recursive)
		{
			VFS_Directory_Entry* dir = allentries.entry;
			VFS_DWORD count = allentries.countFound;
			for (VFS_DWORD i = 0; i<count; i++, dir++)
			{
				// ���������� ������������ ����
				VFS_DWORD originalInodeNo = curDirInodeNo;
				VFS_Inode* originalInode = curDirInode;

				if (change_directory(name) == 0)
				{
					int res = remove(dir->name, recursive);
					if (res == 0)
						allentries.countFound--;
					else if (res == -4)
						not_permissions |= true; 
				}
				else
					continue;

				// ��������������� ������������ ����
				curDirInodeNo = originalInodeNo;
				curDirInode = originalInode;

			}
		}

		if (allentries.countFound > 2 && recursive == true && not_permissions != true)
			remove(name, recursive);

		else if (allentries.countFound > 2)
		{
			delete[] allentries.entry;
			delete[] observer.block;
			return (not_permissions)? -4 : -3;
		}

		delete[] allentries.entry;
	}

	// ������� ������
	for (VFS_WORD i = 0; i < inode->i_blocks; i++)
		free_block(inode->i_block[i]);
	free_inode(observer.entry->inode);
	
	// ���������� ������������ �����
	observer.entry->file_type = VFS_FT_UNKNOWN;
	memset(observer.entry->name, 0, observer.entry->name_len);
	observer.entry->name_len = 0;
	write_block(curDirInode, observer.block_no, observer.block);

	delete[] observer.block;
	return 0;
}

// ������������� �����/����
int VFS::rename(const char* old_name, const char* new_name)
{
	// ���� �������� ������� "." ��� ".." - �������� �����.
	if (strcmp(old_name, ".") == 0 || strcmp(old_name, "..") == 0 || strlen(old_name) == 0)
		return -2;

	// ��������� ����� �����, ���� ������ ��� ��������� - �����
	if (strlen(old_name)>VFS_NAME_LEN || strlen(new_name)>VFS_NAME_LEN)
		return -4;

	// ������� ������ ������
	VFS_Observer_Return find = observeDir(old_name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� ������ �� ������� - ������� � �������
	if (!find.exist)
	{
		delete[] find.block;
		return -1;
	}

	// �������� �� ����� ���������������
	VFS_Inode* inode;
	read_inode(find.entry->inode, inode);
	if (!(inode->i_uid == curUserID && (inode->i_mode & VFS_IMODE_UWRITE) == VFS_IMODE_UWRITE) &&
		!(inode->i_uid != curUserID && (inode->i_mode & VFS_IMODE_OWRITE) == VFS_IMODE_OWRITE))
	{
		delete[] find.block;
		return -5;
	}

	// ��������� new_name �� �������������
	VFS_Observer_Return find_new = observeDir(new_name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY);

	// ���� ������ ������� - ������� � �������
	if (find_new.exist)
	{
		delete[] find.block;
		return -3;
	}

	// ���� ������ ������� - ������ �
	find.entry->name_len = strlen(new_name);
	memset(find.entry->name, 0, strlen(old_name));
	memcpy(find.entry->name, new_name, find.entry->name_len);

	write_block(curDirInode, find.block_no, find.block);

	delete[] find.block;
	return 0;
}

// �������� ��������� ����������/�����
int VFS::chown(int uid, const char* name)
{
	if (uid < 0 || uid > (VFS_WORD)-1)
		return -3;

	VFS_Observer_Return observer = observeDir(name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);
	if (!observer.exist)
	{
		return -1;
	}

	VFS_Inode* inode;
	read_inode(observer.entry->inode, inode);

	// ���������, ����� �� ����� ������� ������������ ���� ����� ������
	if (inode->i_uid != curUserID && curUserID != 0)
	{
		delete[] observer.block;
		return -2;
	}

	inode->i_uid = uid;

	delete[] observer.block;
	return 0;
}

// �������� ����� �� ����������/����
int VFS::chmod(int access, const char* name)
{
	VFS_Observer_Return observer = observeDir(name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);
	if (!observer.exist)
	{
		delete[] observer.block;
		return -1;
	}

	VFS_Inode* inode;
	read_inode(observer.entry->inode, inode);

	// ���������, ����� �� ����� ������� ������������ ���� ����� ������
	if (inode->i_uid != curUserID && curUserID != 0)
	{
		delete[] observer.block;
		return -2;
	}


	VFS_BYTE user_mode = access / 10;
	VFS_BYTE other_mode = access % 10;

	// ��������� �� �������� ����
	if (((user_mode << 3) > VFS_IMODE_UALL || other_mode > VFS_IMODE_OALL) || access < 0)
	{
		delete[] observer.block;
		return -3;
	}

	VFS_WORD new_access = 0;
	if ((inode->i_mode & VFS_IMODE_FILE) == VFS_IMODE_FILE)
		new_access |= VFS_IMODE_FILE;
	if ((inode->i_mode & VFS_IMODE_DIR) == VFS_IMODE_DIR)
		new_access |= VFS_IMODE_DIR;

	inode->i_mode = new_access | (user_mode << 3) | other_mode;

	delete[] observer.block;
	return 0;
}

// ������� ��������� ����������
// currentInodeNo - ����� ����� ���������� ��������
// parentInodeNo - ����� ����� ������������� ��������
// inode - ����� ���������� ��������
void VFS::buildEmptyDir(VFS_DWORD currentInodeNo, VFS_DWORD parentInodeNo, VFS_Inode* inode)
{
	// �������� ���������� "."
	VFS_Directory_Entry *currentDirEntry = new VFS_Directory_Entry();
	currentDirEntry->inode = currentInodeNo;
	currentDirEntry->file_type = VFS_FT_DIR;
	currentDirEntry->name_len = 1;
	currentDirEntry->rec_len = VFS_DIRECTORY_ENTRY_SIZE;
	memcpy(currentDirEntry->name, ".", 1);

	// �������� ���������� ".."
	VFS_Directory_Entry *parentDirEntry = new VFS_Directory_Entry();
	parentDirEntry->inode = parentInodeNo;
	parentDirEntry->file_type = VFS_FT_DIR;
	parentDirEntry->name_len = 2;
	parentDirEntry->rec_len = VFS_DIRECTORY_ENTRY_SIZE;
	memcpy(parentDirEntry->name, "..", 2);

	// ���� ������ ��� ������. ��������� ���� ������� �����������
	VFS_Directory_Entry* block = fillDirBlock();

	// ������ ������ �� "." � ".."
	block[0] = *currentDirEntry;
	block[1] = *parentDirEntry;

	write_block(inode, 0, (VFS_BYTE*) block);

	delete[] block;
	delete currentDirEntry;
	delete parentDirEntry;
}

// �������� � ��������� ���� ���������� ���������� (��������� ����������)
VFS_Directory_Entry* VFS::fillDirBlock()
{
	// ������ ������
	VFS_Directory_Entry* writeData = new VFS_Directory_Entry[getBlockSize() / VFS_DIRECTORY_ENTRY_SIZE];

	// "������" ������
	VFS_Directory_Entry* emptyDirEntry = new VFS_Directory_Entry();
	emptyDirEntry->inode = 0;
	emptyDirEntry->file_type = VFS_FT_UNKNOWN;
	emptyDirEntry->name_len = 0;
	emptyDirEntry->rec_len = VFS_DIRECTORY_ENTRY_SIZE;

	// ��������� ����
	for (VFS_WORD i = 0; i < getBlockSize() / VFS_DIRECTORY_ENTRY_SIZE; i++)
	{
		writeData[i] = *emptyDirEntry;
	}

	delete emptyDirEntry;
	return writeData;
}

// ������� ����� ����� �� �����
long VFS::getInodeNo(const char* name)
{
	VFS_Observer_Return observer = observeDir(name, curDirInodeNo, OBSERVER_FIND_SAME_ENTRY | OBSERVER_NEED_BLOCK_ENTRY);

	// ���� �� ������� ������ - ������
	if (!observer.exist)
	{
		delete[] observer.block;
		return -2;
	}

	VFS_DWORD inode_no = observer.entry->inode;
	delete[] observer.block;
	return inode_no;
}

// ������� ID �������� ������������
VFS_WORD VFS::getCurrentUserID()
{
	return curUserID;
}

// ������� Login �������� ������������
char* VFS::getCurrentUserLogin()
{
	return curUserLogin;
}

// ������� ������ �����
VFS_WORD VFS::getBlockSize()
{
	return superblock->s_block_size;
}

void VFS::printStat()
{
// ������� ���������� ����������, ��� ��� �������.
	cout << "===========================================" << endl;
	cout << "���������� � ����������� �������� �������:" << endl;
	cout << "===========================================" << endl;
	cout << "���������� ������: " << superblock->s_blocks_count << endl;
	cout << "������ ���� ������: " << superblock->s_first_data_block << endl;
	cout << "���������� ������: " << superblock->s_inodes_count << endl;
	cout << "������ �����: " << getBlockSize() << endl;
	cout << "���������� ���������: " << superblock->s_magic << endl;
	cout << "������: " << superblock->s_version << endl;
	cout << "===========================================" << endl;
}

// �������� ����� � VFS
int VFS::makeVFS(string filename, VFS_DWORD size, const char* rootPass)
{
	// ���� ������ < 1048576
	// TODO: ��������� ����������� ������
	if (size < 1048576)
		return -1;
	
	// ������� ���������
	VFS_Superblock* super = new VFS_Superblock;
	memset(super, 0x00, VFS_SUPERBLOCK_SIZE);

	// ���������� ������ = ������ ����� / VFS_BLOCK_SIZE (������ 1 �����)
	VFS_DWORD blocks_count = size / VFS_BLOCK_SIZE;

	// ���������� ���� = ���������� ������
	VFS_DWORD inodes_count = blocks_count;

	// ������� ������� ����� ������
	VFS_Bitmap* blocksBitmap = new VFS_Bitmap(blocks_count);

	// ������� ������� ����� ������
	VFS_Bitmap* inodeBitmap = new VFS_Bitmap(inodes_count);

	// ������� ������� ������
	VFS_Inode* inodeTable = new VFS_Inode[inodes_count];
	memset(inodeTable, 0x00, inodes_count * VFS_INODE_SIZE);

	// ������ ����� ������� ����� � �������
	VFS_DWORD first_data_block = (blocksBitmap->getSize() / VFS_BLOCK_SIZE + 1)
		+ (inodeBitmap->getSize() / VFS_BLOCK_SIZE + 1)
		+ ((inodes_count * VFS_INODE_SIZE) / VFS_BLOCK_SIZE + 1);

	// ���������� ��������� ������ �������� ���������
	VFS_DWORD free_blocks_count = blocks_count - first_data_block;

	// ��������� ���������
	super->s_blocks_count		= blocks_count;				// ���������� ������
	super->s_inodes_count		= inodes_count;				// ���������� ������ = ���������� ������ / 2
	super->s_first_data_block	= first_data_block;			// ������ ����� ����� � �������
	super->s_block_size			= VFS_BLOCK_SIZE;			// ������ ����� �����������.
	super->s_magic				= VFS_MAGIC;				// ���������� ���������
	super->s_version			= VFS_VERSION;				// ���������� ������

	// ������� ���� VFS
	int VFS_File = open(filename.c_str(), _O_RDWR  | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | 
                            _S_IWRITE);
	if (VFS_File == -1)
	{
		string error = "[!] ���������� ������� ���� ���������� �������� ������� " + filename;
		throw runtime_error(error);
	}


	// ��������� �� �������� ������ ����������
	_lseek(VFS_File, VFS_START_SUPERBLOCK, SEEK_SET);

	// ���������� ���������
	_write(VFS_File, super, VFS_SUPERBLOCK_SIZE);

	// ���������� block bitmap
	_write(VFS_File, blocksBitmap->getBitmap(), sizeof(VFS_BYTE) * blocksBitmap->getSize());

	// ���������� inode bitmap
	_write(VFS_File, inodeBitmap->getBitmap(), sizeof(VFS_BYTE) * inodeBitmap->getSize());

	// ���������� inode table	
	_write(VFS_File, inodeTable, VFS_INODE_SIZE * inodes_count);

	// ��������� ����
	_chsize(VFS_File, size);

	// ��������� ����
	_close(VFS_File);

	// ������� root-�������
	// ��������� ��
	VFS* vfs = new VFS(filename, false);
	VFS_DWORD block_no = vfs->allocate_block();
	VFS_Inode* rootInode = new VFS_Inode();
	rootInode->i_block[0] = block_no;
	rootInode->i_blocks = 1;
	rootInode->i_mode = VFS_IMODE_DIR | VFS_IMODE_UALL | VFS_IMODE_OREAD;
	rootInode->i_flag = VFS_IFLAG_UNDEL_FL;
	rootInode->i_uid = 0;
	rootInode->i_size = VFS_BLOCK_SIZE;
	vfs->buildEmptyDir(VFS_INODE_ROOT, VFS_INODE_ROOT, rootInode);
	vfs->create_inode(VFS_INODE_ROOT, rootInode);

	// ������� ���� �������������
	// ������ ������ ��� ��������� ������
	vfs->read_root();
	vfs->curUserID = 0;		// ���������� ����� ��� �������� ������ ��
	vfs->makeFile("shadow", VFS_IFLAG_UNDEL_FL);

	// ��������� ������������ root
	vfs->addUser("root", rootPass);

	delete vfs;
	delete rootInode;
	

	// ������� �� ������ ���������
	delete super;
	delete blocksBitmap;
	delete inodeBitmap;
	delete[] inodeTable;

	cout << endl << "�������� ������� ������� ������� � " << filename.c_str() << endl;

	return 0;
}