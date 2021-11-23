#include "stdafx.h"
#include "CCommunication.h"

void SetLogger(std::tstring _name, DWORD _inputOption)
{
	std::tstring strModuleFile = core::GetFileName();
	std::tstring strModuleDir = core::ExtractDirectory(strModuleFile);
	std::tstring strModuleName = core::ExtractFileNameWithoutExt(strModuleFile);
	std::tstring strLogFile = strModuleDir + TEXT("/") + strModuleName + TEXT(".log");

	core::ST_LOG_INIT_PARAM_EX init;
	init.strLogFile = strLogFile;
	init.strID = TEXT(_name);
	init.dwInputFlag = _inputOption;
	init.dwOutputFlag = core::LOG_OUTPUT_FILE | core::LOG_OUTPUT_CONSOLE | core::LOG_OUTPUT_DBGWND;
	init.dwMaxFileSize = 10 * 1000 * 1000;
	init.dwMaxFileCount = 10;
	init.nLogRotation = core::LOG_ROTATION_SIZE;
	core::InitLog(init);
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	SetLogger("Agent-Log", core::LOG_INFO | core::LOG_WARN | core::LOG_ERROR | core::LOG_DEBUG);
	core::Log_Info(TEXT("main.cpp - [%s]"), TEXT("Program is Debug Mode"));
#else
	SetLogger("Agent-Log", core::LOG_INFO | core::LOG_WARN | core::LOG_ERROR);
	core::Log_Info(TEXT("main.cpp - [%s]"), TEXT("Program is Release Mode"));
#endif

	CommunicationManager()->Init("9090");

	CommunicationManager()->Start();

	CommunicationManager()->Recv();
}