#ifndef STAT_COMM_H
#define STAT_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class statCommand: public Command
{
public:
	statCommand();
	~statCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif