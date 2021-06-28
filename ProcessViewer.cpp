#include "ListProcess.h"
#include <vector>
#include <fstream>

int main(void)
{
	std::wofstream f;
	f.open("output.txt", std::ios::out);
    std::vector<Process> vecProcess = getRunningProcesses();
    for (Process temp : vecProcess) {
		f << L"PID: " << temp.pid << std::endl;
		f << L"Name: " << temp.name << std::endl;
		f << L"Command: " << temp.command << std::endl;
		f << L"------------------------------------------------------------------------------------- " << std::endl;
    }
    return 0;
}