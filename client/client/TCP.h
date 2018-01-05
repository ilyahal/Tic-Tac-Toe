#pragma once

#include <string>
#include "WinSock2.h"

class TCP
{
private:
	// Сокет клиента
	SOCKET s;
	// Символ конца команды
	char end_mark;
	// Поиск символа конца команды
	bool FindEndMark(std::string str);
	// Получение данных от сервера
	std::string ReceiveData();
public:
	TCP(char end_mark);

	// Инициализация WinSock DLL
	void Startup();
	// Инициализация дескриптора сокета
	void InitSocket();
	// Подключение к серверу
	void Connect(int port, std::string host);
	// Отправка данных
	void SendData(std::string str);
	// Получить данные до символа конца команды
	std::string ReceiveCommand();
	// Закрытие соединения
	void CloseConnection();
};

