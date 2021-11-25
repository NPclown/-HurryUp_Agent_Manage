#include "CService.h"

#define AGNET_DIRPATH TEXT("/var/log/hurryup/agent")
#define ENV_PATH TEXT("/var/log/hurryup/agent/env.json")
#define ERROR_CHECK TEXT("echo $?")
#define KILL TEXT("kill -9 ")
#define WGET TEXT("wget -P /usr/local/bin -O HurryUp_Agent")
#define DELETE TEXT("rm -rf /usr/local/bin/HurryUp_Agent")
#define CHMOD TEXT("chmod 755 /usr/local/bin/HurryUp_Agent")
#define AGENT_PATH TEXT("/usr/local/bin/HurryUp_Agent")

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

	CheckDirectory(AGNET_DIRPATH);

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

	if (!core::PathFileExistsA(AGENT_PATH)) {
		core::Log_Error(TEXT("CService.cpp - [%s]"), TEXT("Agent Not Exisit."));
		return -1;
	}

	std::tstring startCommand = "nohup " + this->serviceName + " 1> /dev/null 2>&1 &";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Start Command"), startCommand.c_str());

	std::tstring result = exec(startCommand.c_str());

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStart End"));
	return 0;
}

int CService::AgentStop()
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStop"));

	std::tstring pidCommand = "ps -eaf | grep " + this->serviceName + " | grep -v grep | grep -v HurryUp_Agent_Manager | awk '{print $2}'";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Pid Command"), pidCommand.c_str());

	std::tstring result = exec(pidCommand.c_str());

	if (result == "")
		return 0;

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	std::tstring tmp = "";
	for (auto i : Split(result, "\n"))
		tmp += i + " ";

	std::tstring killCommand = KILL + tmp;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Kill Command"), killCommand.c_str());

	result = exec(killCommand.c_str());

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStop End"));
	return 0;
}

int CService::AgentUpdate(std::tstring data)
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentUpdate"));

	if (data == "") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Data Error"));
		core::Log_Warn(TEXT("Data Error - [%s]"), TEXT(data.c_str()));
		return -1;
	}

	if (AgentDelete() == -1) {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("AgentDelete Error"));
		return -1;
	}
	
	std::tstring updateCommand = WGET + std::tstring(" ") + data;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Update Command"), updateCommand.c_str());

	std::tstring result = exec(updateCommand.c_str());

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Chmod Command"), CHMOD);

	result = exec(CHMOD);

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

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

	std::tstring deleteCommand = DELETE;;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Delete Command"), deleteCommand.c_str());

	std::tstring result = exec(deleteCommand.c_str());

	std::tstring error = Split(exec(ERROR_CHECK), "\n")[0];
	if (error != std::tstring("0") && error != std::tstring("127")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentDelete End"));
	return 0;
}

int CService::AgentStatus()
{
	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStatus"));

	std::tstring pidCommand = "ps -ax -o command | grep " + this->serviceName + " | grep -v grep | grep -v HurryUp_Agent_Manager | wc -l";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Pid Command"), pidCommand.c_str());

	std::tstring result = exec(pidCommand.c_str());

	std::cout << result << std::endl;

	if (Split(result, "\n")[0] == std::tstring("0"))
		return -1;

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error"), TEXT(result.c_str()));
		return -1;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s]"), TEXT("AgentStatus End"));
	return 0;
}
