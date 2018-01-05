#pragma once

#include <vector>

class Board
{
private:
	// ������ ������
	const int empty = 0;
	// ������ ������� ����
	unsigned int side;
	// ������� ����
	std::vector<std::vector<int>> board;
	// ���������� ������
	bool IsValidCell(unsigned int row, unsigned int column);
	// ���������� ����
	bool IsVaildMove(unsigned int row, unsigned int column);
	// �������� ������ ���������
	bool CheckFirstDiagonal();
	// �������� ������ ���������
	bool CheckSecondDiagonal();
	// �������� ������
	bool CheckRow(unsigned int index);
	// �������� �������
	bool CheckColumn(unsigned int index);
public:
	Board(unsigned int side);
	
	// ��������� ������
	bool Update(int role_id, unsigned int row, unsigned int column);
	// �������� ��������� ����
	std::vector<std::pair<unsigned int, unsigned int>> GetPossibleMoves();
	// �������� ����������
	int GetWinner();
};

