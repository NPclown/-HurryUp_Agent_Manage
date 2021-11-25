#include "CService.h"

#define AGNET_DIRPATH TEXT("/var/log/hurryup/agent")
#define ENV_PATH TEXT("/var/log/hurryup/agent/env.json")
#define ERROR_CHECK TEXT("echo $?")
#define KILL TEXT("kill -9 ")
#define WGET TEXT("wget -P /usr/local/bin -O /usr/local/bin/HurryUp_Agent")
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

std::tstring CService::AgentEnvironment(std::tstring data)
{
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Setting Agent Environment Start"));

	CheckDirectory(AGNET_DIRPATH);

	if (data == "") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Packet Data Error"));
		return "Pacekt Data Error.";
	}

	if (AgentTerminate() != "Success") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Agent Stop Error"));
		return "Agent Stop Error.";
	}

	ST_ENV_INFO envInfo;
	std::tstring errMessage;
	core::ReadJsonFromString(&envInfo, data);
	core::WriteJsonToFile(&envInfo, ENV_PATH, &errMessage);

	if (errMessage != "") {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("WriteJsonToFile Error"), TEXT(errMessage.c_str()));
		return "Convert Json To File Error : " + errMessage;
	}
	
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Setting Agent Environment End"));
	return "Success";
}

std::tstring CService::AgentExecute()
{
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent Execute Start."));

	if (AgentTerminate() != "Success") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("AgentStop Error"));
		return "Agent Stop Error.";
	}

	if (!core::PathFileExistsA(AGENT_PATH)) {
		core::Log_Error(TEXT("CService.cpp - [%s]"), TEXT("Agent File Not Exisit."));
		return "Agent File Not Exisit.";
	}

	std::tstring startCommand = "nohup " + this->serviceName + " 1> /dev/null 2>&1 &";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Start Command"), startCommand.c_str());

	std::tstring result = exec(startCommand.c_str());

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + startCommand;
	}

	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent Execute End."));
	return "Success";
}

std::tstring CService::AgentTerminate()
{
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent Terminate Start."));

	std::tstring pidCommand = "ps -eaf | grep " + this->serviceName + " | grep -v grep | grep -v HurryUp_Agent_Manager | awk '{print $2}'";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Pid Command"), pidCommand.c_str());

	std::tstring result = exec(pidCommand.c_str());

	if (result == "")
		return "Success";

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + pidCommand;
	}

	std::tstring tmp = "";
	for (auto i : Split(result, "\n"))
		tmp += i + " ";

	std::tstring killCommand = KILL + tmp;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Kill Command"), killCommand.c_str());

	result = exec(killCommand.c_str());

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + killCommand;
	}

	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent Terminate End."));
	return "Success";
}

std::tstring CService::AgentUpdate(std::tstring data)
{
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent File Update Start"));

	if (data == "") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Packet Data Error"));
		return "Pacekt Data Error.";
	}

	if (AgentTerminate() != "Success") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Agent Stop Error"));
		return "Agent Stop Error.";
	}

	if (AgentDelete() != "Success") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Agent Delete Error"));
		return "Agent Delete Error.";
	}
	
	//TODO :: WGET 보안 위협 제거 필요
	std::tstring updateCommand = WGET + std::tstring(" ") + data;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Update Command"), updateCommand.c_str());

	std::tstring result = exec(updateCommand.c_str());
	//TODO :: wget Error 잡기

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + updateCommand;
	}

	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Chmod Command"), CHMOD);

	result = exec(CHMOD);
	//TODO :: chmod Error 잡기

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + CHMOD;
	}

	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent File Update End"));
	return "Success";
}

std::tstring CService::AgentDelete()
{
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent File Delete Start"));

	if (AgentTerminate() != "Success") {
		core::Log_Warn(TEXT("CService.cpp - [%s]"), TEXT("Agent Stop Error"));
		return "Agent Stop Error.";
	}

	std::tstring deleteCommand = DELETE;;
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Delete Command"), deleteCommand.c_str());

	std::tstring result = exec(deleteCommand.c_str());

	std::tstring error = Split(exec(ERROR_CHECK), "\n")[0];
	if (error != std::tstring("0") && error != std::tstring("127")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + CHMOD;
	}
	
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent File Delete End"));
	return "Success";
}

std::tstring CService::AgentStatus()
{
	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent Status Check Start"));

	std::tstring pidCommand = "ps -ax -o command | grep " + this->serviceName + " | grep -v grep | grep -v HurryUp_Agent_Manager | wc -l";
	core::Log_Debug(TEXT("CService.cpp - [%s] : %s"), TEXT("Pid Command"), pidCommand.c_str());

	std::tstring result = exec(pidCommand.c_str());

	std::cout << result << std::endl;

	if (Split(result, "\n")[0] == std::tstring("0"))
		return "Dead";

	if (Split(exec(ERROR_CHECK), "\n")[0] != std::tstring("0")) {
		core::Log_Error(TEXT("CService.cpp - [%s] : %s"), TEXT("Exec Command Error."), TEXT(result.c_str()));
		return "Exec Command Error [" + result + "] : " + CHMOD;
	}

	core::Log_Info(TEXT("CService.cpp - [%s]"), TEXT("Agent Status Check End"));
	return "Live";
}
