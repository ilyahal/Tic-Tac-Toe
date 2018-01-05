#pragma once

#include <string>
#include "WinSock2.h"

class TCP
{
private:
	// ����� �������
	SOCKET s;
	// ������ ����� �������
	char end_mark;
	// ����� ������� ����� �������
	bool FindEndMark(std::string str);
	// ��������� ������ �� �������
	std::string ReceiveData();
public:
	TCP(char end_mark);

	// ������������� WinSock DLL
	void Startup();
	// ������������� ����������� ������
	void InitSocket();
	// ����������� � �������
	void Connect(int port, std::string host);
	// �������� ������
	void SendData(std::string str);
	// �������� ������ �� ������� ����� �������
	std::string ReceiveCommand();
	// �������� ����������
	void CloseConnection();
};

