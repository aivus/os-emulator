#ifndef TEST_COMM_H
#define TEST_COMM_H

#include <vector>
#include <string>
#include "Command.h"
#include "VFS.h"

class testCommand: public Command
{
public:
	testCommand();
	~testCommand();
	virtual int action(VFS *vfs, std::vector<std::string> data);

};

#endif