#pragma once

#include <string>
#include <vector>
#include "WinSock2.h"
#include "SocketGame.h"

class TCP
{
private:
	// ������ ����� �������
	char end_mark;
	// ����� ������� ����� �������
	bool FindEndMark(std::string str);
	// ������� �������������
	void SetNonblock(SOCKET socket);
public:
	TCP(char end_mark);

	// ������������� WinSock DLL
	void Startup();
	// ������������� ����������� ������
	SOCKET InitSocket();
	// ����� ������ � ������
	void Bind(SOCKET socket, int port);
	// ������� �����
	void Listen(SOCKET socket, int backlog);
	// �������� ����������
	void WaitActivity(fd_set *set, SOCKET master_socket, std::vector<SocketGame *> *clients);
	// �������� ��������
	bool IsActive(fd_set *set, SOCKET socket);
	// ���������� �������
	bool Accept(SOCKET master_socket, std::vector<SocketGame *> *clients);
	// �������� ������ �� �������
	std::string *ReceiveData(SOCKET socket);
	// ��������� ��������� �������
	void SendData(SOCKET socket, std::string str);
	// �������� ����������
	void CloseConnection(SOCKET socket);
	// ���������� ������
	void Finish();
};
