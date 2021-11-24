#include "CUtils.h"
#include <regex>
#include <fstream>

#define DIRPATH TEXT("/var/log/hurryup/manager/")
//TODO :: 경로에 대한 유효성 검증이 필요

void SetLogger(std::tstring _name, DWORD _inputOption)
{
	std::tstring strModuleFile = core::GetFileName();
	std::tstring strModuleDir = core::ExtractDirectory(strModuleFile);
	std::tstring strModuleName = core::ExtractFileNameWithoutExt(strModuleFile);
	std::tstring strLogFile = DIRPATH + strModuleName + TEXT(".log");

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

std::tstring exec(const char* cmd)
{
	std::array<char, 256> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}

	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}

	return result;
}

std::vector<std::tstring> Split(std::tstring input, std::tstring delimiter)
{
	std::vector<std::tstring> result;
	std::tstring temp = input;
	std::tstring next = "";

	while (1) {
		std::tstring r = core::Split(temp, delimiter, &next);
		result.push_back(r);
		
		if (r == temp)
			break;

		temp = next;
	}

	return result;
}