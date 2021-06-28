#pragma once

#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>
#include <vector>
#include <winternl.h>
#include "Process.h"


std::vector<Process> getRunningProcesses() {
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	PVOID pebAddress;
	LPVOID UserProcParamsAddress;
	UNICODE_STRING commandLine;
	PROCESS_BASIC_INFORMATION pbi;
	unsigned int i;
	std::vector<Process> processList;
	PWSTR cmdLine;

	LONG(WINAPI * NtQueryInformationProcess)(HANDLE ProcessHandle,
		ULONG ProcessInformationClass, PVOID ProcessInformation,
		ULONG ProcessInformationLength, PULONG ReturnLength);

	*(FARPROC*)&NtQueryInformationProcess = GetProcAddress(LoadLibrary(L"ntdll"), "NtQueryInformationProcess");


	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return processList;
	}
	cProcesses = cbNeeded / sizeof(DWORD);
	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

			//lay processbasicinfo vao bien pbi
			NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), 0);

			if (NULL != hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;
				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
				{
					//lay ten cua process
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));

					pebAddress = pbi.PebBaseAddress;
					
					//lay processparam no chua commanline cua process trong do
					ReadProcessMemory(hProcess, (PCHAR)pebAddress + 0x10, &UserProcParamsAddress, sizeof(PVOID), NULL);
					
					//lay ra dia chi commandline trong processparam
					ReadProcessMemory(hProcess, (PCHAR)UserProcParamsAddress + 0x40, &commandLine, sizeof(commandLine), NULL);
					cmdLine = (PWSTR) new char[commandLine.MaximumLength];
					
					//lay chuoi commandline tu dia chi vua lay duoc
					ReadProcessMemory(hProcess, commandLine.Buffer, cmdLine, commandLine.MaximumLength, NULL);
					
					Process p;
					p.pid = aProcesses[i];
					p.name = std::wstring(&szProcessName[0]);
					std::wstring cmd = std::wstring(cmdLine);
					p.command = cmd;
					processList.push_back(p);
				}
			}
		}
	}
	return processList;
}