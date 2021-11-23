#include "CCommunication.h"

CCommunication::CCommunication()
{
}

CCommunication::~CCommunication()
{
}

void CCommunication::Send()
{
}

void CCommunication::Recv()
{
	core::Log_Debug(TEXT("CCommunication.cpp - [%s]"), TEXT("Working Recv In Thread"));

	while (1)
	{
		int clientLength = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientLength);
		if (clientSocket < 0)
		{
			printf("Server: accept failed.\n");
			exit(0);
		}

		int messageLength;
		char message[BUFFER_SIZE + 1];

		while (1) {

			messageLength = read(clientSocket, &message, BUFFER_SIZE);
			core::Log_Debug(TEXT("CCommunication.cpp - [%s] : %d"), TEXT("Read Complete"), messageLength);
			core::Log_Debug(TEXT("CCommunication.cpp - [%s] : %s"), TEXT("Recieve Message"), message);

			// 서버가 종료되었을 경우
			if (messageLength == 0)
			{
				//Disconnect();
				break;
			}
			// 수신 받을 읽기에 문제가 생긴 경우, 보통 읽을 메세지가 없는 경우
			else if (messageLength < 0)
			{
				core::Log_Warn(TEXT("CCommunication.cpp - [%s] : %d"), TEXT("Read Error Code"), errno);
				core::Log_Debug(TEXT("CCommunication.cpp - [%s] : %s"), TEXT("Remain MessageBuffer"), TEXT(messageBuffers.c_str()));
				break;
			}
			// 메세지를 정상적으로 수신했을 경우
			else
			{
				message[messageLength] = 0;
				messageBuffers += message;

				while (1)
				{
					size_t start_location = messageBuffers.find("BOBSTART");
					size_t end_location = messageBuffers.find("BOBEND");
					core::Log_Debug(TEXT("CCommunication.cpp - [%s] : %d, %d"), TEXT("Find Position(Start, End)"), start_location, end_location);

					if (start_location == -1 || end_location == -1)
						break;

					ST_PACKET_INFO stPacketInfo;
					core::ReadJsonFromString(&stPacketInfo, messageBuffers.substr(start_location + 8, end_location - (start_location + 8)));

					Match(stPacketInfo.protocol, stPacketInfo.data);
					messageBuffers = messageBuffers.substr(end_location + 6);
				}
			}
			memset(message, 0, sizeof(message));
		}
	}
}

void CCommunication::Match(int protocol, std::tstring)
{
	switch (protocol)
	{
	case AGENT_INIT:
		//TODO :: 환경변수 파일 생성 구현 필요
		std::cout << "AGENT_INIT" << std::endl;
		break;
	case AGENT_START:
		//TODO :: 에이전트 실행
		std::cout << "AGENT_START" << std::endl;
		break;
	case AGENT_STOP:
		//TODO :: 에이전트 중지
		std::cout << "AGENT_STOP" << std::endl;
		break;
	case AGENT_UPDATE:
		//TODO :: 에이전트 업데이트
		std::cout << "AGENT_UPDATE" << std::endl;
		break;
	case AGENT_DELETE:
		//TODO :: 에이전트 삭제
		std::cout << "AGENT_DELETE" << std::endl;
		break;
	default:
		std::cout << "WRONG" << std::endl;
	}
}

CCommunication* CCommunication::GetInstance(void)
{
	static CCommunication instance;
	return &instance;
}

void CCommunication::Init(std::tstring port)
{
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(atoi(port.c_str()));
}

void CCommunication::Start()
{
	core::Log_Debug(TEXT("CCommunication.cpp - [%s]"), TEXT("Working Start"));
	int requestCount = 5;
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket < 0)
		core::Log_Warn(TEXT("CCommunication.cpp - [%s] : %d"), TEXT("Socket Create Fail"), errno);

	while (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		sleep(5);
		core::Log_Warn(TEXT("CCommunication.cpp - [%s] : %d"), TEXT("Socket Bind Fail"), errno);
	}

	while (listen(serverSocket, requestCount) < 0)
	{
		sleep(5);
		core::Log_Warn(TEXT("CCommunication.cpp - [%s] : %d"), TEXT("Socket Listen Fail"), errno);
	}

	core::Log_Debug(TEXT("CCommunication.cpp - [%s]"), TEXT("Working End"));
}

void CCommunication::End()
{
}
