#ifndef CHOWN_COMM_H
#define CHOWN_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class chownCommand: public Command
{
public:
	chownCommand();
	~chownCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif