#include "stdafx.h"
#include "StringHelper.h"

using namespace std;

string StringHelper::DeleteLastLeter(string str)
{
	return str.substr(0, str.length() - 1);
}

string StringHelper::AddLetter(string str, char letter)
{
	str += letter;
	return str;
}
