#pragma once
#include "stdafx.h"
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
//TCP 서버 클래스 생성
class CCommunication
{
private:
	int serverSocket, clientSocket;
	struct sockaddr_in serverAddress, clientAddress;
	std::tstring messageBuffers;

	CCommunication();
	~CCommunication();

public:
	static CCommunication* GetInstance(void);
	
	void Init(std::tstring port);
	void Start();
	void Send();
	void Recv();
	void Match(int protocol, std::tstring);
	void End();
};

inline CCommunication* CommunicationManager()
{
	return CCommunication::GetInstance();
}
