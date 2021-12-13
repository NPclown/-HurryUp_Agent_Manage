#include "stdafx.h"
#include "CCommunication.h"
#include "CService.h"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	SetLogger("AgentManager", core::LOG_INFO | core::LOG_WARN | core::LOG_ERROR | core::LOG_DEBUG);
	core::Log_Info(TEXT("main.cpp - [%s]"), TEXT("Program is Debug Mode v2.0"));
#else
	SetLogger("AgentManager", core::LOG_INFO | core::LOG_WARN | core::LOG_ERROR);
	core::Log_Info(TEXT("main.cpp - [%s]"), TEXT("Program is Release Mode v2.0"));
#endif
	CommunicationManager()->Init("9090");

	CommunicationManager()->Start();
	ServiceManager()->Init("HurryUp_Agent");

	CommunicationManager()->Recv();
}