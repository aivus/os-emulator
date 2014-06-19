#ifndef CAT_COMM_H
#define CAT_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class catCommand: public Command
{
public:
	catCommand();
	~catCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif