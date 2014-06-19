#ifndef MAKE_DIR_COMM_H
#define MAKE_DIR_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class makeDirCommand: public Command
{
public:
	makeDirCommand();
	~makeDirCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif