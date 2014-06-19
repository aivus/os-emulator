#ifndef MAKE_PIPE_COMM_H
#define MAKE_PIPE_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class makePipeCommand: public Command
{
public:
	makePipeCommand();
	~makePipeCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif