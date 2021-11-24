#include "CService.h"

#define ENV_PATH TEXT("/usr/local/bin/env.json")
#define ERROR_CHECK TEXT("echo $?")
#define KILL TEXT("kill -9 ")

CService::CService()
{
	this->serviceName = "";
}

CService::~CService()
{
}

CService* CService::GetInstance(void)
{
	static CService instance;
	return &instance;
}

void CService::Init(std::tstring _serviceName)
{
	this->serviceName = _serviceName;
}

int CService::AgentInit(std::tstring data)
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentInit"));

	if (data == "") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Data Error"));
		core::Log_Warn(TEXT("Data Error - [%s]"), TEXT(data.c_str()));
		return -1;
	}

	if (AgentStop() == -1) {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("AgentStop Error"));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("Agent Environment Variable Init"));

	ST_ENV_INFO envInfo;
	std::tstring errMessage;
	core::ReadJsonFromString(&envInfo, data);
	core::WriteJsonToFile(&envInfo, ENV_PATH, &errMessage);

	if (errMessage != "") {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("WriteJsonToFile Error"), TEXT(errMessage.c_str()));
		return -1;
	}
	
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentInit End"));
	return 0;
}

int CService::AgentStart()
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStart"));

	if (AgentStop() == -1) {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("AgentStop Error"));
		return -1;
	}

	std::tstring result = exec(this->serviceName.c_str());

	if (exec(ERROR_CHECK) != "0") {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStart End"));
	return 0;
}

int CService::AgentStop()
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStop"));

	std::tstring pidCommand = "ps -eaf | grep " + this->serviceName + " | grep - v grep | awk '{print $2}'";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Pid Command"), pidCommand.c_str());

	std::tstring result = exec(pidCommand.c_str());

	if (result == "")
		return 0;

	if (exec(ERROR_CHECK) != "0") {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	std::tstring tmp = "";
	for (auto i : Split(result, "\n"))
		tmp += i + " ";

	std::tstring killCommand = KILL + tmp;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Kill Command"), killCommand.c_str());

	result = exec(killCommand.c_str());

	if (exec(ERROR_CHECK) != "0") {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStop End"));
	return 0;
}

int CService::AgentUpdate()
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentUpdate"));

	if (AgentStop() == -1) {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("AgentStop Error"));
		return -1;
	}
	
	//TODO :: 에이전트 업데이트 구현 필요
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentUpdate End"));
	return 0;
}

int CService::AgentDelete()
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentDelete"));

	if (AgentStop() == -1) {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("AgentStop Error"));
		return -1;
	}

	//TODO :: 에이전트 삭제 구현 필요
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentDelete End"));
	return 0;
}
