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
		int AgentInit(std::tstring data);
		int AgentStart();
		int AgentStop();
		int AgentUpdate();
		int AgentDelete();
};

inline CService* ServiceManager()
{
	return CService::GetInstance();
}
