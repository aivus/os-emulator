#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include "VFS.h"

// Abstract class
class Command
{
public:
	virtual ~Command(){};
	virtual int action(VFS *vfs, std::vector<std::string> data) =0;
};

#endif