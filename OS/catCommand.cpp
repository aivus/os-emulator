#include <iostream>
#include <vector>
#include <string>
#include "Console.h"
#include "catCommand.h"
#include "VFS.h"
#include "retCodes.h"
#include "Pipe.h"

using namespace std;

// �����������
catCommand::catCommand()
{
	//
}

// ����������
catCommand::~catCommand()
{
	//
}

int catCommand::action(VFS *vfs, std::vector<std::string> data)
{
	if (vfs == NULL)
		return OSERROR;

	if (data.size()>1)
	{
		if (data.size() == 3 && data[1] == ">>")
		{
			// TODO: ������� �������� �� ������

			// ����� � ����
			vector<string> command = Console::showPromt(false);
			string buffer;

			std::vector<std::string>::iterator it = command.begin(), end = command.end();
			for (; it != end; it++)
			{
				buffer += it->c_str();
				if (it != end - 1) buffer += "\n";
			}

			switch(vfs->writeFile(data[2].c_str(), (void*)buffer.c_str(), buffer.size()))
			{
				case -1:
					cerr << "������ �������� � ����������!" << endl;
					break;
				case -2:
					cerr << "���� '" << data[2] << "' �� ������!" << endl;
					break;
				case -3:
					cerr << "������ ������ ������� ����� ��� ������!" << endl;
					break;
				case -4:
					cerr << "� ��� ������������ ����!" << endl;
					break;
				case -1000:
					long pipe_id = vfs->getInodeNo(data[2].c_str());
					if (pipe_id < 0)
					{
						cerr << "����� �����-�� ���������!" << endl;
						break;
					}

					Pipe::allocate(pipe_id);
					Pipe::write(pipe_id, buffer);
					break;

			}
		}
		else
		{
			void* mem = NULL;
			long pipe_id;
			switch(int size = vfs->readFile(data[1].c_str(), mem))
			{
				case -1:
					cerr << "������ ������ ����������!" << endl;
					break;
				case -2:
					cerr << "���� '" << data[1] << "' �� ������!" << endl;
					break;
				case -3:
					cerr << "� ��� ������������ ����!" << endl;
					break;
				case -1000:
					pipe_id = vfs->getInodeNo(data[1].c_str());
					if (pipe_id < 0)
					{
						cerr << "����� �����-�� ���������!" << endl;
						break;
					}

					Pipe::allocate(pipe_id);
					cout << Pipe::read(pipe_id) << endl;
					break;
				default:
					cout << (char*)mem << endl;
					break;
			}

			delete[] mem;
		}
	}
	else
	{
		cerr << "�������� ���������� ����������!" << endl;
	}

	return OSSUCCESS;
}