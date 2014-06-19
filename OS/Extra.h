#ifndef EXTRA_H
#define EXTRA_H

#include <string>
#include <sstream>

class Extra
{
public:
	// Конвертирование int в string
	static std::string convertIntToString(int number)
	{
	   std::stringstream ss;	// Create a stringstream
	   ss << number;			// Add number to the stream
	   return ss.str();			// Return a string with the contents of the stream
	}
};

#endif