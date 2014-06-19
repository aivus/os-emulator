#ifndef LIST_COMM_H
#define LIST_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class listCommand: public Command
{
public:
	listCommand();
	~listCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif