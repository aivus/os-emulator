#ifndef RENAME_COMM_H
#define RENAME_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class renameCommand: public Command
{
public:
	renameCommand();
	~renameCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);
	void usage();

};

#endif