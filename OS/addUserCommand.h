#ifndef ADD_USER_COMM_H
#define ADD_USER_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class addUserCommand: public Command
{
public:
	addUserCommand();
	~addUserCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif