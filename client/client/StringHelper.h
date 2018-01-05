#pragma once

#include <string>
#include <vector>

namespace StringHelper
{
	// Разделить строку
	std::vector<std::string> Split(std::string str, char separator);
	// Удалить последний символ
	std::string DeleteLastLeter(std::string str);
	// Добавить символ
	std::string AddLetter(std::string str, char letter);
};
