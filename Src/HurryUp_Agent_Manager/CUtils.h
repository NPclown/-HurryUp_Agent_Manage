#pragma once
#include "stdafx.h"

void SetLogger(std::tstring _name, DWORD _inputOption);
std::tstring exec(const char* cmd);
std::vector<std::tstring> Split(std::tstring input, std::tstring delimiter);
void CheckDirectory(std::tstring _path);
