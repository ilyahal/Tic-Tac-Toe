#include "stdafx.h"
#include <sstream>
#include "StringHelper.h"

using namespace std;

vector<string> StringHelper::Split(string str, char separator)
{
	vector<string> result;

	stringstream ss(str);
	string item;
	while (getline(ss, item, separator))
	{
		result.push_back(item);
	}

	return result;
}

string StringHelper::DeleteLastLeter(string str)
{
	return str.substr(0, str.length() - 1);
}

string StringHelper::AddLetter(string str, char letter)
{
	str += letter;
	return str;
}
