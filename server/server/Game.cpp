#include "stdafx.h"
#include <time.h>
#include "Game.h"

using namespace std;

void Game::UpdateStatus()
{
	int winner_id = board->GetWinner();
	int main_role_id = main_role == X ? 1 : -1;
	int player_role_id = player_role == X ? 1 : -1;

	if (winner_id == main_role_id)
	{
		status = Lose;
	}
	else if (winner_id == player_role_id)
	{
		status = Win;
	}
	else
	{
		auto possible_moves = board->GetPossibleMoves();
		if (possible_moves.size() == 0)
		{
			status = Tie;
		}
	}
}

Game::Game() : status(Authorized)
{ }

GameStatus Game::GetStatus()
{
	return status;
}

Role Game::GetMainRole()
{
	return main_role;
}

Role Game::GetPlayerRole()
{
	return player_role;
}

void Game::Start()
{
	srand((unsigned int)time(NULL));
	if (rand() % 2)
	{
		main_role = O;
		player_role = X;
	}
	else 
	{
		main_role = X;
		player_role = O;
	}

	status = GameStatus::Setup;
}

void Game::Setup(int side)
{
	board = new Board(side);

	status = InGame;
}

void Game::Stop()
{
	status = Authorized;
}

pair<unsigned int, unsigned int> Game::MoveMain()
{
	int role_id = main_role == X ? 1 : -1;

	auto possible_moves = board->GetPossibleMoves();
	int move_index = rand() % possible_moves.size();
	auto move = possible_moves.at(move_index);

	board->Update(role_id, move.first, move.second);
	UpdateStatus();

	return move;
}

bool Game::MovePlayer(unsigned int row, unsigned int column)
{
	int role_id = player_role == X ? 1 : -1;

	bool is_completed = board->Update(role_id, row, column);
	if (is_completed)
	{
		UpdateStatus();
		
		return true;
	}

	return false;
}
