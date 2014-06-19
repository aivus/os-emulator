#ifndef INTERNAL_COMM_H
#define INTERNAL_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class internalCommand: public Command
{
	void help();
public:
	internalCommand();
	~internalCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif