#ifndef COPY_COMM_H
#define COPY_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class copyCommand: public Command
{
public:
	copyCommand();
	~copyCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);
	void usage();

};

#endif