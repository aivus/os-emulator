#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "Console.h"

using namespace std;

string Console::enterPassword()
{	
	string result;
	char code;
	bool end = false;

	// Цикл ввода
	do
	{
		code = getch();

		if (code == 13)
		{
			end = true;
		}
		else if(code == 8 && result.length() > 0)
		{
			result.pop_back();
		}
		else
		{
			result.push_back(code);
		}
	}
	while(!end);
	// END Цикл ввода

	// Выводим перевод строки "для красоты"
	cout << endl;

	return result;
}

vector<string> Console::showPromt()
{
	return Console::showPromt(true);
}

vector<string> Console::showPromt(bool useDelim)
{
	vector<string> command;
	string tmp;
	bool end = false;

	command.clear();

	// Цикл ввода
	do
	{
		if ((cin >> tmp) && ((useDelim && (cin.peek() == '\n'))) || cin.eof())
		{
			end = true;
		}

		if (!cin.eof())
			command.push_back(tmp);
	}
	while(!end);
	// END Цикл ввода

	cin.sync();
	cin.clear();

	return command;
}