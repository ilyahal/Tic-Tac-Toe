#pragma once

#include <string>
#include <vector>

namespace StringHelper
{
	// ��������� ������
	std::vector<std::string> Split(std::string str, char separator);
	// ������� ��������� ������
	std::string DeleteLastLeter(std::string str);
	// �������� ������
	std::string AddLetter(std::string str, char letter);
};
