#pragma once

#include <string>
#include "GameStatus.h"
#include "Role.h"
#include "Board.h"

class Game
{
private:
	// Статус игры
	GameStatus status;
	// Роль сервера
	Role main_role;
	// Роль игрока
	Role player_role;
	// Игровое поле
	Board *board;
	// Обновить статус
	void UpdateStatus();
public:
	Game();

	// Получение статуса игры
	GameStatus GetStatus();
	// Получить роль сервера
	Role GetMainRole();
	// Получить роль игрока
	Role GetPlayerRole();
	// Запуск игры
	void Start();
	// Настройка игры
	void Setup(int side);
	// Остановить
	void Stop();
	// Выполнить шаг сервера
	std::pair<unsigned int, unsigned int> MoveMain();
	// Выполнить шаг игрока
	bool MovePlayer(unsigned int row, unsigned int column);
};
