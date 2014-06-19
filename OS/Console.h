#ifndef CONSOLE_H
#define CONSOLE_H

class Console
{
public:
	static std::string enterPassword();
	static std::vector<std::string> showPromt();
	static std::vector<std::string> showPromt(bool useDelim);
};
#endif