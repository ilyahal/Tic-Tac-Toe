#pragma once

#include <string>
#include <vector>
#include "WinSock2.h"
#include "SocketGame.h"

class TCP
{
private:
	// Символ конца команды
	char end_mark;
	// Поиск символа конца команды
	bool FindEndMark(std::string str);
	// Сделать неблокирующим
	void SetNonblock(SOCKET socket);
public:
	TCP(char end_mark);

	// Инициализация WinSock DLL
	void Startup();
	// Инициализация дескриптора сокета
	SOCKET InitSocket();
	// Связь сокета с портом
	void Bind(SOCKET socket, int port);
	// Слушать сокет
	void Listen(SOCKET socket, int backlog);
	// Ожидание активности
	void WaitActivity(fd_set *set, SOCKET master_socket, std::vector<SocketGame *> *clients);
	// Является активным
	bool IsActive(fd_set *set, SOCKET socket);
	// Подключить клиента
	bool Accept(SOCKET master_socket, std::vector<SocketGame *> *clients);
	// Получить данные от клиента
	std::string *ReceiveData(SOCKET socket);
	// Отправить сообщение клиенту
	void SendData(SOCKET socket, std::string str);
	// Закрытие соединения
	void CloseConnection(SOCKET socket);
	// Завершение работы
	void Finish();
};
