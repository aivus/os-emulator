#ifndef PIPE_COMM_H
#define PIPE_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class pipeCommand: public Command
{
public:
	pipeCommand();
	~pipeCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);
};

#endif