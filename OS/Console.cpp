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

	// ���� �����
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
	// END ���� �����

	// ������� ������� ������ "��� �������"
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

	// ���� �����
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
	// END ���� �����

	cin.sync();
	cin.clear();

	return command;
}