#ifndef VFS_BITMAP_H
#define VFS_BITMAP_H

#include "VFS_Types.h"

// Размер VFS_BYTE в битах
#define VFS_BYTE_SIZE (sizeof(VFS_BYTE) * 8);

class VFS_Bitmap
{
	VFS_BYTE* Bitmap;	// Битовая карта
	VFS_DWORD size;		// Размер битовой карты
public:
	// Конструктор. Загрузить уже имеющуюся битовую карту.
	// bitmap - Битовая карта
	// count - количестов элементов 
	VFS_Bitmap(VFS_BYTE* bitmap, VFS_DWORD count);

	// Конструктор. Создать новую битовую карту с кол-вом объектов count.
	VFS_Bitmap(VFS_DWORD count);

	// Деструктор. Чистим память.
	~VFS_Bitmap();

	// Установка флага
	void SetBit(VFS_DWORD position, bool flag);
	
	// Проверить состояние флага
	bool testBit(VFS_DWORD position);

	// Найти свободный блок
	VFS_DWORD searchFree();

	// Вернуть битовую карту
	VFS_BYTE* getBitmap();

	// Вернуть количество элементов в битовой карте
	VFS_DWORD getSize();
};

#endif