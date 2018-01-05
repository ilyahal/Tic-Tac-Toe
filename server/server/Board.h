#pragma once

#include <vector>

class Board
{
private:
	// Пустая ячейка
	const int empty = 0;
	// Размер стороны поля
	unsigned int side;
	// Игровое поле
	std::vector<std::vector<int>> board;
	// Валидность ячейки
	bool IsValidCell(unsigned int row, unsigned int column);
	// Валидность хода
	bool IsVaildMove(unsigned int row, unsigned int column);
	// Проверка первой диагонали
	bool CheckFirstDiagonal();
	// Проверка второй диагонали
	bool CheckSecondDiagonal();
	// Проверка строки
	bool CheckRow(unsigned int index);
	// Проверка столбца
	bool CheckColumn(unsigned int index);
public:
	Board(unsigned int side);
	
	// Заполнить ячейку
	bool Update(int role_id, unsigned int row, unsigned int column);
	// Получить возможные ходы
	std::vector<std::pair<unsigned int, unsigned int>> GetPossibleMoves();
	// Получить победителя
	int GetWinner();
};

