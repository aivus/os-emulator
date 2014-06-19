#ifndef CHMOD_COMM_H
#define CHMOD_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class chmodCommand: public Command
{
public:
	chmodCommand();
	~chmodCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif