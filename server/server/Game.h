#pragma once

#include <string>
#include "GameStatus.h"
#include "Role.h"
#include "Board.h"

class Game
{
private:
	// ������ ����
	GameStatus status;
	// ���� �������
	Role main_role;
	// ���� ������
	Role player_role;
	// ������� ����
	Board *board;
	// �������� ������
	void UpdateStatus();
public:
	Game();

	// ��������� ������� ����
	GameStatus GetStatus();
	// �������� ���� �������
	Role GetMainRole();
	// �������� ���� ������
	Role GetPlayerRole();
	// ������ ����
	void Start();
	// ��������� ����
	void Setup(int side);
	// ����������
	void Stop();
	// ��������� ��� �������
	std::pair<unsigned int, unsigned int> MoveMain();
	// ��������� ��� ������
	bool MovePlayer(unsigned int row, unsigned int column);
};
