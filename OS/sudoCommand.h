#ifndef SUDO_COMM_H
#define SUDO_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class sudoCommand: public Command
{
public:
	sudoCommand();
	~sudoCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif