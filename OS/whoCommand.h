#ifndef WHO_COMM_H
#define WHO_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class whoCommand: public Command
{
public:
	whoCommand();
	~whoCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif