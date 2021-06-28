#pragma once
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <vector>
class Process
{
public:
	DWORD pid;
	std::wstring name;
	std::wstring command;
};