#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class sharedMemCommand: public Command
{
public:
	sharedMemCommand();
	~sharedMemCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);
};

#endif