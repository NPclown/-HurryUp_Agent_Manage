#pragma once
#include "stdafx.h"

class CService
{
	private:
		std::tstring serviceName;

		CService();
		~CService();
	public:
		static CService* GetInstance(void);
		
		void Init(std::tstring _serviceName);
		std::tstring AgentEnvironment(std::tstring data);
		std::tstring AgentExecute();
		std::tstring AgentTerminate();
		std::tstring AgentUpdate(std::tstring data);
		std::tstring AgentDelete();
		std::tstring AgentStatus();
};

inline CService* ServiceManager()
{
	return CService::GetInstance();
}
