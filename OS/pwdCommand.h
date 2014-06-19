#ifndef PWD_COMM_H
#define PWD_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class pwdCommand: public Command
{
public:
	static std::vector<std::string> path;

	pwdCommand();
	~pwdCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif