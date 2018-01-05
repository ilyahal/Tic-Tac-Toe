// client.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "StringHelper.h"
#include "TCP.h"

#define PORT 4526

#define ENDMARK '.'

#define KILL_COMMAND "KILL"
#define HELLO_COMMAND "HELLO"
#define EXIT_COMMAND "EXIT"
#define START_COMMAND "START"
#define STOPGAME_COMMAND '#'

#define OK_RESPONSE "OK"
#define INVALIDCOMMAND_RESPONSE "INVALIDCOMMAND"

#define LOSE_CODE -2
#define MISTAKE_CODE -1
#define DEFAULT_CODE 0
#define TIE_CODE 1
#define WIN_CODE 2

using namespace std;

// �������� ������ ��� ����
string GetIcon(int role_id)
{
	if (role_id == -1)
	{
		return "O";
	}
	else if (role_id == 1)
	{
		return "X";
	}

	return " ";
}

// ���������� �������
void Draw(vector<vector<int>> *board, int side)
{
	cout << endl;

	for (int i = 0; i < side; i++)
	{
		cout << "   ";
		for (int j = 0; j < side - 1; j++) {
			cout << "|   ";
		}
		cout << endl;

		vector<int> line = board->at(i);
		for (int j = 0; j < side; j++) {
			if (j != 0)
			{
				cout << "|";
			}
			cout << " " << GetIcon(line.at(j)) << " ";
			if (j == side - 1)
			{
				cout << endl;
			}
		}

		cout << "   ";
		for (int j = 0; j < side - 1; j++) {
			cout << "|   ";
		}
		cout << endl;

		if (i != side - 1)
		{
			cout << "---";
			for (int j = 0; j < side - 1; j++) {
				cout << "+---";
			}
			cout << endl;
		}
	}

	cout << endl;
}

// ��������� ���� �������
int GetMainMove(TCP *tcp, vector<vector<int>> *board, int server_role_id)
{
	string response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
	vector<string> move_info = StringHelper::Split(response, ':');
	if (move_info.size() == 3)
	{
		int code = stoi(move_info.at(0));
		int row = stoi(move_info.at(1));
		int column = stoi(move_info.at(2));

		board->at(row).at(column) = server_role_id;

		return code;
	}
	
	throw string("����������� ������");
}

int main()
{
	setlocale(LC_ALL, "Russian");

	TCP* tcp = new TCP(ENDMARK);

	try
	{
		tcp->Startup();
		tcp->InitSocket();

		cout << "������� ����: ";
		string host;
		cin >> host;

		tcp->Connect(PORT, host);

		tcp->SendData(StringHelper::AddLetter(HELLO_COMMAND, ENDMARK));
		string response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
		if (response == OK_RESPONSE)
		{
			while (true)
			{
				cout << "0. �����" << endl;
				cout << "1. ������ ����" << endl;

				string command;
				cin >> command;

				if (command == "0")
				{
					tcp->SendData(StringHelper::AddLetter(EXIT_COMMAND, ENDMARK));

					cout << "������� �� ����!";
					break;
				}
				else if (command == "1")
				{
					tcp->SendData(StringHelper::AddLetter(START_COMMAND, ENDMARK));

					response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
					if (response == OK_RESPONSE)
					{
						cout << "0. 3 x 3" << endl;
						cout << "1. 5 x 5" << endl;
						cout << "2. 7 x 7" << endl;
						cout << "3. 9 x 9" << endl;

						cin >> command;

						int side;
						if (command == "0")
						{
							side = 3;
						}
						else if (command == "1")
						{
							side = 5;
						}
						else if (command == "2")
						{
							side = 7;
						}
						else if (command == "3")
						{
							side = 9;
						}
						else
						{
							string answer;
							answer.push_back(STOPGAME_COMMAND);
							
							tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

							response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
							if (response == OK_RESPONSE)
							{
								cout << "����� ����." << endl;
								continue;
							}
							else
							{
								throw string("����������� ������");
							}
						}

						string answer = to_string(side);
						tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

						int role_id;
						int server_role_id;

						response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
						if (response == "X")
						{
							server_role_id = -1;
							role_id = 1;
						}
						else if (response == "O")
						{
							role_id = -1;
							server_role_id = 1;
						}
						else
						{
							throw string("����������� ������");
						}

						vector<vector<int>> board(side, vector<int>(side, 0));

						if (server_role_id == 1)
						{
							GetMainMove(tcp, &board, server_role_id);
						}

						Draw(&board, side);

						bool is_end = false;
						while (!is_end)
						{
							string tmp;

							cout << "������: ";
							cin >> tmp;

							if (tmp[0] == STOPGAME_COMMAND)
							{
								answer = "";
								answer.push_back(STOPGAME_COMMAND);

								tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

								response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
								if (response == OK_RESPONSE)
								{
									cout << "����� ����." << endl;
									break;
								}
								else
								{
									throw string("����������� ������");
								}
							}
							
							int row;
							row = stoi(tmp);

							cout << "�������: ";
							cin >> tmp;

							if (tmp[0] == STOPGAME_COMMAND)
							{
								answer = "";
								answer.push_back(STOPGAME_COMMAND);

								tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

								response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
								if (response == OK_RESPONSE)
								{
									cout << "����� ����." << endl;
									break;
								}
								else
								{
									throw string("����������� ������");
								}
							}

							int column;
							column = stoi(tmp);

							answer = to_string(row) + ":" + to_string(column);
							tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

							response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
							int code = stoi(response);

							if (code == LOSE_CODE || code == TIE_CODE || code == WIN_CODE)
							{
								board.at(row).at(column) = role_id;
								Draw(&board, side);

								is_end = true;

								if (code == LOSE_CODE)
								{
									cout << "�� ���������." << endl;
								}
								else if (code == TIE_CODE)
								{
									cout << "�����." << endl;
								}
								else
								{
									cout << "�� ��������!" << endl;
								}
							}
							else if (code == MISTAKE_CODE)
							{
								cout << "������! ���������� ��� ���." << endl;
							}
							else if (code == DEFAULT_CODE)
							{
								board.at(row).at(column) = role_id;
								Draw(&board, side);

								GetMainMove(tcp, &board, server_role_id);
								Draw(&board, side);
							}
						}
					}
				}
				else
				{
					cout << "������! ���������� ��� ���." << endl;
				}
			}
		}
	}
	catch (string error)
	{
		cout << error << endl;
	}

	tcp->CloseConnection();

	_getch();
    return 0;
}
