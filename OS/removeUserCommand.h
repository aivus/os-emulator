#ifndef REMOVE_USER_COMM_H
#define REMOVE_USER_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class removeUserCommand: public Command
{
public:
	removeUserCommand();
	~removeUserCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif