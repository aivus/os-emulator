#ifndef NAMED_PIPE_COMM_H
#define NAMED_PIPE_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class namedPipeCommand: public Command
{
public:
	namedPipeCommand();
	~namedPipeCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);
};

#endif