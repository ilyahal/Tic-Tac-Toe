#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "TCP.h"

#define DEFAULT_BUFLEN 1024

using namespace std;

bool TCP::FindEndMark(string str)
{
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == end_mark)
		{
			return true;
		}
	}

	return false;
}

string TCP::ReceiveData()
{
	char recvbuf[DEFAULT_BUFLEN];

	int recv_result = recv(s, recvbuf, DEFAULT_BUFLEN, 0);
	if (recv_result == SOCKET_ERROR)
	{
		throw string("Ошибка при получении данных: " + to_string(WSAGetLastError()));
	}
	else if (recv_result == 0)
	{
		throw string("Подключение закрыто.");
	}

	recvbuf[recv_result] = '\0';

	string str = string(recvbuf);
	return str;
}

TCP::TCP(char end_mark)
{
	this->end_mark = end_mark;
}

void TCP::Startup()
{
	WORD winsock_version = MAKEWORD(2, 2);
	WSAData wsa_data;
	if (WSAStartup(winsock_version, &wsa_data) != NO_ERROR)
	{
		throw string("Ошибка при инициализации WinSock.");
	}
}

void TCP::InitSocket()
{
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		throw string("Ошибка при инициализации сокета:" + std::to_string(WSAGetLastError()));
	}
}

void TCP::Connect(int port, string host)
{
	DWORD addr = inet_addr(host.c_str());
	if (addr == INADDR_NONE) {
		throw string("The IPv4 address entered must be a legal address.");
	}
	HOSTENT *hinfo = gethostbyaddr((char *)&addr, 4, AF_INET);
	if (hinfo == NULL || !hinfo) {
		throw string("Can't find host " + host);
	}

	SOCKADDR_IN socketaddr;
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(port);
	memcpy((char FAR *)&(socketaddr.sin_addr.s_addr), hinfo->h_addr_list[0], hinfo->h_length);

	if (connect(s, (LPSOCKADDR)&socketaddr, sizeof(socketaddr)) == SOCKET_ERROR)
	{
		throw string("Ошибка при подключении: " + to_string(WSAGetLastError()));
	}
}

void TCP::SendData(string str)
{
	if (send(s, str.c_str(), str.length(), 0) == SOCKET_ERROR) {
		throw string("Ошибка при отправке данных: " + to_string(WSAGetLastError()));
	}
}

string TCP::ReceiveCommand()
{
	string str = "";
	while (true)
	{
		str += ReceiveData();
		if (FindEndMark(str))
		{
			break;
		}
	}

	return str;
}

void TCP::CloseConnection()
{
	closesocket(s);
	WSACleanup();
}
