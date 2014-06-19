#include <string>
#include <stdexcept>
#include "retCodes.h"
#include "VFS_Types.h"
#include "VFS_Bitmap.h"

// �����������. ��������� ��� ��������� ������� �����.
// bitmap - ������� �����
// count - ���������� ��������� 
VFS_Bitmap::VFS_Bitmap(VFS_BYTE* bitmap, VFS_DWORD count)
{
	Bitmap = new VFS_BYTE[count / 8];
	memcpy(Bitmap, bitmap, count / 8 * sizeof(VFS_BYTE));
	//Bitmap = bitmap;
	SetBit(0, true);
	this->size = count / 8;
}

// �����������. ������� ����� ������� ����� � ���-��� �������� count.
VFS_Bitmap::VFS_Bitmap(VFS_DWORD count)
{
	Bitmap = new VFS_BYTE[count / 8];
	memset(Bitmap, 0x00, count / 8 * sizeof(VFS_BYTE));
	SetBit(0, true);
	this->size = count / 8;
}

// ����������. ������ ������.
VFS_Bitmap::~VFS_Bitmap()
{
	delete[] Bitmap;
}

// ��������� �����
void VFS_Bitmap::SetBit(VFS_DWORD position, bool flag)
{
	VFS_DWORD elementNumber = position / VFS_BYTE_SIZE;
	VFS_DWORD bitOffset = position % VFS_BYTE_SIZE;

	if (flag)
		Bitmap[elementNumber] = Bitmap[elementNumber] | (1 << bitOffset);
	else
		Bitmap[elementNumber] = Bitmap[elementNumber] &~ (1 << bitOffset);
}

// ��������� ��������� �����
bool VFS_Bitmap::testBit(VFS_DWORD position)
{
	VFS_DWORD elementNumber = position / VFS_BYTE_SIZE;
	VFS_BYTE bitOffset = position % VFS_BYTE_SIZE;

	return ((Bitmap[elementNumber] & (1 << bitOffset)) == (1 << bitOffset));
}

// ����� ��������� ����
VFS_DWORD VFS_Bitmap::searchFree()
{
	VFS_DWORD counter = 0;
	bool found = false;

	// ��������� �����
	for (VFS_DWORD i=0; i<this->getSize(), !found; i++)
	{
		// ���� ��� ���� ����������� � 1 - ���������� ������ ����
		if (Bitmap[i] ==  (VFS_BYTE)(~Bitmap[i]|Bitmap[i])){
			counter+=8;
			continue;
		}

		// ��������� ����
		for (size_t j=0; j<8, !found; j++)
		{
			VFS_WORD mask = (1 << j);
			if ((Bitmap[i] & mask ) == mask)
				counter++;
			else
				// ����� ��������� ����
				found = true;

		}

	}

	if (found)
		return counter;
	else
		// ��������� ��� �� ������. ������� ����������.
		throw (VFS_BYTE)101;
}

// ������� ������� �����
VFS_BYTE* VFS_Bitmap::getBitmap()
{
	return Bitmap;
}

// ������� ���������� ��������� � ������� �����
VFS_DWORD VFS_Bitmap::getSize()
{
	return this->size;
}