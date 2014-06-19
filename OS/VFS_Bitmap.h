#ifndef VFS_BITMAP_H
#define VFS_BITMAP_H

#include "VFS_Types.h"

// ������ VFS_BYTE � �����
#define VFS_BYTE_SIZE (sizeof(VFS_BYTE) * 8);

class VFS_Bitmap
{
	VFS_BYTE* Bitmap;	// ������� �����
	VFS_DWORD size;		// ������ ������� �����
public:
	// �����������. ��������� ��� ��������� ������� �����.
	// bitmap - ������� �����
	// count - ���������� ��������� 
	VFS_Bitmap(VFS_BYTE* bitmap, VFS_DWORD count);

	// �����������. ������� ����� ������� ����� � ���-��� �������� count.
	VFS_Bitmap(VFS_DWORD count);

	// ����������. ������ ������.
	~VFS_Bitmap();

	// ��������� �����
	void SetBit(VFS_DWORD position, bool flag);
	
	// ��������� ��������� �����
	bool testBit(VFS_DWORD position);

	// ����� ��������� ����
	VFS_DWORD searchFree();

	// ������� ������� �����
	VFS_BYTE* getBitmap();

	// ������� ���������� ��������� � ������� �����
	VFS_DWORD getSize();
};

#endif