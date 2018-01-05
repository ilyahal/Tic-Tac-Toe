#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <time.h>
#include "TCP.h"

#define DEFAULT_BUFLEN 1024

using namespace std;

bool TCP::FindEndMark(string str)
{
	return str.find(end_mark) != string::npos;
}

void TCP::SetNonblock(SOCKET socket)
{
	const int nonblocking = 1;
	if (ioctlsocket(socket, FIONBIO, (unsigned long *)&nonblocking) == SOCKET_ERROR)
	{
		throw string("Setup socket error: " + to_string(WSAGetLastError()));
	}
}

TCP::TCP(char end_mark)
{
	this->end_mark = end_mark;
}

void TCP::Startup()
{
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR)
	{
		throw string("Initialising WinSock failed. Error code:" + to_string(WSAGetLastError()));
	}
}

SOCKET TCP::InitSocket()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		throw string("Could not create socket:" + to_string(WSAGetLastError()));
	}

	SetNonblock(s);
	return s;
}

void TCP::Bind(SOCKET socket, int port)
{
	SOCKADDR_IN socketaddr;
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(port);
	socketaddr.sin_addr.s_addr = htons(INADDR_ANY);

	if (bind(socket, (struct sockaddr *)&socketaddr, sizeof(socketaddr)) == SOCKET_ERROR)
	{
		throw string("Bind failed with error code: " + to_string(WSAGetLastError()));
	}
}

void TCP::Listen(SOCKET socket, int backlog)
{
	listen(socket, backlog);
}

void TCP::WaitActivity(fd_set *set, SOCKET master_socket, vector<SocketGame *> *clients)
{
	FD_ZERO(set);

	FD_SET(master_socket, set);
	for (unsigned int i = 0; i < clients->size(); i++)
	{
		FD_SET(clients->at(i)->socket, set);
	}

	if (select(0, set, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		throw string("Select call failed with error code:" + to_string(WSAGetLastError()));
	}
}

bool TCP::IsActive(fd_set *set, SOCKET socket)
{
	return FD_ISSET(socket, set);
}

bool TCP::Accept(SOCKET master_socket, vector<SocketGame *> *clients)
{
	int length = sizeof(SOCKADDR);
	SOCKADDR_IN client_socketaddr;
	SOCKET socket = accept(master_socket, (LPSOCKADDR)&client_socketaddr, &length);
	if (socket == INVALID_SOCKET)
	{
		return false;
	}

	SetNonblock(socket);
	clients->push_back(new SocketGame(socket, nullptr));

	return true;
}

string *TCP::ReceiveData(SOCKET socket)
{
	char recvbuf[DEFAULT_BUFLEN];
	int recv_result = recv(socket, recvbuf, DEFAULT_BUFLEN, 0);
	if (recv_result == SOCKET_ERROR || recv_result == 0)
	{
		return nullptr;
	}

	recvbuf[recv_result] = '\0';
	string str(recvbuf);

	time_t timeStart = time(0);
	while (!FindEndMark(str))
	{
		recv_result = recv(socket, recvbuf, DEFAULT_BUFLEN, 0);
		if (recv_result == SOCKET_ERROR || recv_result == 0)
		{
			break;
		}

		recvbuf[recv_result] = '\0';
		str += string(recvbuf);

		if (difftime(time(0), timeStart) > 10)
		{
			break;
		}
	}

	string *result = new string(str);
	return result;
}

void TCP::SendData(SOCKET socket, string str)
{
	(send(socket, str.c_str(), str.length(), 0) == SOCKET_ERROR);
}

void TCP::CloseConnection(SOCKET socket)
{
	closesocket(socket);

}

void TCP::Finish()
{
	WSACleanup();
}
