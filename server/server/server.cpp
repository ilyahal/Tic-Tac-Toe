// server.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <iostream>
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

// Превратить роль в строку
string RoleToString(Role role)
{
	if (role == X)
	{
		return "X";
	} 
	else
	{
		return "O";
	}
}

// Получить код результата шага
string GetMoveResultCode(SocketGame *client)
{
	string result;

	auto status = client->game->GetStatus();
	if (status == Lose)
	{
		result = to_string(LOSE_CODE);
	}
	else if (status == Tie)
	{
		result = to_string(TIE_CODE);
	}
	else if (status == Win)
	{
		result = to_string(WIN_CODE);
	}
	else
	{
		result = to_string(DEFAULT_CODE);
	}

	return result;
}

// Выполнить ход сервера
void MainMove(TCP *tcp, SocketGame *client)
{
	auto result = client->game->MoveMain();

	string answer = GetMoveResultCode(client) + ":" + to_string(result.first) + ":" + to_string(result.second);
	tcp->SendData(client->socket, StringHelper::AddLetter(answer, ENDMARK));
}

int main()
{
	setlocale(LC_ALL, "Russian");

	TCP* tcp = new TCP(ENDMARK);
	
	SOCKET master_socket;
	vector<SocketGame *> *clients = new vector<SocketGame *>();
	
	try
	{
		tcp->Startup();
		master_socket = tcp->InitSocket();
		tcp->Bind(master_socket, PORT);

		cout << "Прослушиваемый порт: " << PORT << endl;
		tcp->Listen(master_socket, SOMAXCONN);

		bool is_alive = true;
		while (is_alive)
		{
			fd_set set;
			tcp->WaitActivity(&set, master_socket, clients);

			for (unsigned int i = 0; i < clients->size(); i++)
			{
				SocketGame *client = clients->at(i);
				if (tcp->IsActive(&set, client->socket))
				{
					bool is_answered = false;

					string *str = tcp->ReceiveData(client->socket);
					if (str != nullptr && str->length() > 1)
					{
						string command = StringHelper::DeleteLastLeter(*str);

						if (client->game == nullptr)
						{
							if (command == KILL_COMMAND)
							{
								is_alive = false;
								break;
							}
							else if (command == HELLO_COMMAND)
							{
								client->game = new Game();

								tcp->SendData(client->socket, StringHelper::AddLetter(OK_RESPONSE, ENDMARK));
								is_answered = true;
							}
						}
						else if (client->game->GetStatus() == Setup)
						{
							if (command.length() == 1)
							{
								if (command[0] == STOPGAME_COMMAND)
								{
									client->game->Stop();

									tcp->SendData(client->socket, StringHelper::AddLetter(OK_RESPONSE, ENDMARK));
									is_answered = true;
								}
								else
								{
									int side = atoi(&command[0]);
									if (side % 2 == 1 && side >= 3 && side < 10)
									{
										client->game->Setup(side);

										Role player_role = client->game->GetPlayerRole();

										tcp->SendData(client->socket, StringHelper::AddLetter(RoleToString(player_role), ENDMARK));
										is_answered = true;

										if (client->game->GetMainRole() == X)
										{
											MainMove(tcp, client);
										}
									}
								}
							}
						}
						else if (client->game->GetStatus() == InGame)
						{
							if (command.length() == 1)
							{
								if (command[0] == STOPGAME_COMMAND)
								{
									client->game->Stop();

									tcp->SendData(client->socket, StringHelper::AddLetter(OK_RESPONSE, ENDMARK));
									is_answered = true;
								}
							}
							else if (command.length() == 3)
							{
								int row = atoi(&command[0]);
								int column = atoi(&command[2]);

								bool is_completed = client->game->MovePlayer(row, column);
								if (is_completed)
								{
									string answer = GetMoveResultCode(client);
									tcp->SendData(client->socket, StringHelper::AddLetter(answer, ENDMARK));
									is_answered = true;

									if (client->game->GetStatus() == InGame)
									{
										MainMove(tcp, client);
									}
								}
								else
								{
									string answer = to_string(MISTAKE_CODE);
									tcp->SendData(client->socket, StringHelper::AddLetter(answer, ENDMARK));
									is_answered = true;
								}
							}
						}
						else
						{
							if (command == EXIT_COMMAND)
							{
								tcp->CloseConnection(client->socket);
								
								clients->erase(clients->begin() + i);
								i--;

								continue;
							}
							else if (command == START_COMMAND)
							{
								client->game->Start();

								tcp->SendData(client->socket, StringHelper::AddLetter(OK_RESPONSE, ENDMARK));
								is_answered = true;
							}
						}
					}

					if (!is_answered)
					{
						tcp->SendData(client->socket, StringHelper::AddLetter(INVALIDCOMMAND_RESPONSE, ENDMARK));
						is_answered = true;
					}
				}
			}

			if (tcp->IsActive(&set, master_socket))
			{
				tcp->Accept(master_socket, clients);
			}
		}
	}
	catch (string error)
	{
		cout << error << endl;
	}

	tcp->CloseConnection(master_socket);
	for (unsigned int i = 0; i < clients->size(); i++)
	{
		tcp->CloseConnection(clients->at(i)->socket);
	}

	tcp->Finish();

	_getch();
    return 0;
}
