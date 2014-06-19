#ifndef CHANGE_DIR_COMM_H
#define CHANGE_DIR_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class changeDirCommand: public Command
{
public:
	changeDirCommand();
	~changeDirCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif