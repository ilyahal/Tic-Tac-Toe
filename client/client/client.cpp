// client.cpp: определяет точку входа для консольного приложения.
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

// Получить иконку для роли
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

// Нарисовать таблицу
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

// Получение хода сервера
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
	
	throw string("Неизвестная ошибка");
}

int main()
{
	setlocale(LC_ALL, "Russian");

	TCP* tcp = new TCP(ENDMARK);

	try
	{
		tcp->Startup();
		tcp->InitSocket();

		cout << "Введите хост: ";
		string host;
		cin >> host;

		tcp->Connect(PORT, host);

		tcp->SendData(StringHelper::AddLetter(HELLO_COMMAND, ENDMARK));
		string response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
		if (response == OK_RESPONSE)
		{
			while (true)
			{
				cout << "0. Выход" << endl;
				cout << "1. Начало игры" << endl;

				string command;
				cin >> command;

				if (command == "0")
				{
					tcp->SendData(StringHelper::AddLetter(EXIT_COMMAND, ENDMARK));

					cout << "Спасибо за игру!";
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
								cout << "Конец игры." << endl;
								continue;
							}
							else
							{
								throw string("Неизвестная ошибка");
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
							throw string("Неизвестная ошибка");
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

							cout << "Строка: ";
							cin >> tmp;

							if (tmp[0] == STOPGAME_COMMAND)
							{
								answer = "";
								answer.push_back(STOPGAME_COMMAND);

								tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

								response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
								if (response == OK_RESPONSE)
								{
									cout << "Конец игры." << endl;
									break;
								}
								else
								{
									throw string("Неизвестная ошибка");
								}
							}
							
							int row;
							row = stoi(tmp);

							cout << "Столбец: ";
							cin >> tmp;

							if (tmp[0] == STOPGAME_COMMAND)
							{
								answer = "";
								answer.push_back(STOPGAME_COMMAND);

								tcp->SendData(StringHelper::AddLetter(answer, ENDMARK));

								response = StringHelper::DeleteLastLeter(tcp->ReceiveCommand());
								if (response == OK_RESPONSE)
								{
									cout << "Конец игры." << endl;
									break;
								}
								else
								{
									throw string("Неизвестная ошибка");
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
									cout << "Вы проиграли." << endl;
								}
								else if (code == TIE_CODE)
								{
									cout << "Ничья." << endl;
								}
								else
								{
									cout << "Вы победили!" << endl;
								}
							}
							else if (code == MISTAKE_CODE)
							{
								cout << "Ошибка! Попробуйте еще раз." << endl;
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
					cout << "Ошибка! Попробуйте еще раз." << endl;
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
