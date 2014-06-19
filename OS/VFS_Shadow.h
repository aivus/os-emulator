#ifndef VFS_SHADOW_H
#define VFS_SHADOW_H

#include "VFS_Types.h"

#define VFS_USER_LOGIN_MAX_LEN 13
#define VFS_USER_PASS_MAX_LEN 16

struct VFS_Shadow
{
	VFS_WORD uid;
	char login[VFS_USER_LOGIN_MAX_LEN];
	char password[VFS_USER_PASS_MAX_LEN];
	VFS_BYTE active;
};

#endif