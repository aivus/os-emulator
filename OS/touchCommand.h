#ifndef TOUCH_COMM_H
#define TOUCH_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class touchCommand: public Command
{
public:
	touchCommand();
	~touchCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif