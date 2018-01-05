#include "stdafx.h"
#include "Board.h"

using namespace std;

bool Board::IsValidCell(unsigned int row, unsigned int column)
{
	return row >= 0 && row < side && column >= 0 && column < side;
}

bool Board::IsVaildMove(unsigned int row, unsigned int column)
{
	return IsValidCell(row, column) && board[row][column] == empty;
}

bool Board::CheckFirstDiagonal()
{
	int top_left_id = board[0][0];
	for (unsigned int i = 1; i < side; i++)
	{
		if (board[i][i] != top_left_id)
		{
			return false;
		}
	}

	return true;
}

bool Board::CheckSecondDiagonal()
{
	int bottom_left_id = board[side - 1][0];
	for (unsigned int i = side - 2, j = 1; i > 0; i--, j++)
	{
		if (board[i][j] != bottom_left_id)
		{
			return false;
		}
	}

	return true;
}

bool Board::CheckRow(unsigned int index)
{
	int left_id = board[index][0];
	for (unsigned int i = 1; i < side; i++)
	{
		if (board[index][i] != left_id)
		{
			return false;
		}
	}

	return true;
}

bool Board::CheckColumn(unsigned int index)
{
	int top_id = board[0][index];
	for (unsigned int i = 1; i < side; i++)
	{
		if (board[i][index] != top_id)
		{
			return false;
		}
	}

	return true;
}

Board::Board(unsigned int side) : board(side, vector<int>(side, empty))
{
	this->side = side;
}

bool Board::Update(int role_id, unsigned int row, unsigned int column)
{
	if (IsVaildMove(row, column))
	{
		board[row][column] = role_id;
		return true;
	}

	return false;
}

vector<pair<unsigned int, unsigned int>> Board::GetPossibleMoves()
{
	vector<pair<unsigned int, unsigned int>> result;
	for (unsigned int i = 0; i < side; i++) {
		for (unsigned int j = 0; j < side; j++) {
			if (board[i][j] == empty) {
				result.push_back(make_pair(i, j));
			}
		}
	}

	return result;
}

int Board::GetWinner()
{
	if (board[side / 2][side / 2] != empty) {
		if (CheckFirstDiagonal())
		{
			return board[0][0];
		}
		else if (CheckSecondDiagonal())
		{
			return board[side - 1][0];
		}
	}

	for (unsigned int i = 0; i < side; i++)
	{
		int left_id = board[i][0];
		if (left_id != empty)
		{
			if (CheckRow(i))
			{
				return left_id;
			}
		}

		int top_id = board[0][i];
		if (top_id != empty)
		{
			if (CheckColumn(i))
			{
				return top_id;
			}
		}
	}

	return empty;
}
