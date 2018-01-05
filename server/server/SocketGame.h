#pragma once

#include "WinSock2.h"
#include "Game.h"

struct SocketGame
{
	// �����
	SOCKET socket;
	// ����
	Game *game;

	SocketGame(SOCKET socket, Game *game)
	{
		this->socket = socket;
		this->game = game;
	}
};
