#ifndef RM_COMM_H
#define RM_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class rmCommand: public Command
{
public:
	rmCommand();
	~rmCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif