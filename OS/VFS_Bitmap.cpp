#include <string>
#include <stdexcept>
#include "retCodes.h"
#include "VFS_Types.h"
#include "VFS_Bitmap.h"

// Конструктор. Загрузить уже имеющуюся битовую карту.
// bitmap - Битовая карта
// count - количестов элементов 
VFS_Bitmap::VFS_Bitmap(VFS_BYTE* bitmap, VFS_DWORD count)
{
	Bitmap = new VFS_BYTE[count / 8];
	memcpy(Bitmap, bitmap, count / 8 * sizeof(VFS_BYTE));
	//Bitmap = bitmap;
	SetBit(0, true);
	this->size = count / 8;
}

// Конструктор. Создать новую битовую карту с кол-вом объектов count.
VFS_Bitmap::VFS_Bitmap(VFS_DWORD count)
{
	Bitmap = new VFS_BYTE[count / 8];
	memset(Bitmap, 0x00, count / 8 * sizeof(VFS_BYTE));
	SetBit(0, true);
	this->size = count / 8;
}

// Деструктор. Чистим память.
VFS_Bitmap::~VFS_Bitmap()
{
	delete[] Bitmap;
}

// Установка флага
void VFS_Bitmap::SetBit(VFS_DWORD position, bool flag)
{
	VFS_DWORD elementNumber = position / VFS_BYTE_SIZE;
	VFS_DWORD bitOffset = position % VFS_BYTE_SIZE;

	if (flag)
		Bitmap[elementNumber] = Bitmap[elementNumber] | (1 << bitOffset);
	else
		Bitmap[elementNumber] = Bitmap[elementNumber] &~ (1 << bitOffset);
}

// Проверить состояние флага
bool VFS_Bitmap::testBit(VFS_DWORD position)
{
	VFS_DWORD elementNumber = position / VFS_BYTE_SIZE;
	VFS_BYTE bitOffset = position % VFS_BYTE_SIZE;

	return ((Bitmap[elementNumber] & (1 << bitOffset)) == (1 << bitOffset));
}

// Найти свободный блок
VFS_DWORD VFS_Bitmap::searchFree()
{
	VFS_DWORD counter = 0;
	bool found = false;

	// Проверяем байты
	for (VFS_DWORD i=0; i<this->getSize(), !found; i++)
	{
		// Если все биты установлены в 1 - пропускаем данный байт
		if (Bitmap[i] ==  (VFS_BYTE)(~Bitmap[i]|Bitmap[i])){
			counter+=8;
			continue;
		}

		// Проверяем биты
		for (size_t j=0; j<8, !found; j++)
		{
			VFS_WORD mask = (1 << j);
			if ((Bitmap[i] & mask ) == mask)
				counter++;
			else
				// Нашли свободный блок
				found = true;

		}

	}

	if (found)
		return counter;
	else
		// Свободный бит не найден. Бросаем исключение.
		throw (VFS_BYTE)101;
}

// Вернуть битовую карту
VFS_BYTE* VFS_Bitmap::getBitmap()
{
	return Bitmap;
}

// Вернуть количество элементов в битовой карте
VFS_DWORD VFS_Bitmap::getSize()
{
	return this->size;
}