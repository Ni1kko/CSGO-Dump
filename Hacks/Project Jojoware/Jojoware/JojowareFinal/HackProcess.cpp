#include "HackProcess.h"
using namespace std;


void InitMemory::StartMemory(void)
{
	cout << "Getting Cs:go" << endl;
	while (GetApp()) { Sleep(10); };
	ClientDLL = GetModule("client_panorama.dll");
	ServerDLL = GetModule("server.dll");
	EngineDLL = GetModule("engine.dll");
}
bool InitMemory::GetApp()
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(handle, &entry))
	{
		while (Process32Next(handle, &entry))
		{
			cout << entry.szExeFile << endl;
			if (!strcmp(entry.szExeFile, "csgo.exe"))
			{
				PID = entry.th32ProcessID;
				CloseHandle(handle);
				Hdl = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
				return false;
			}
		}
	}
	return true;
}
DWORD InitMemory::GetModule(LPCSTR module)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 mentry;
	mentry.dwSize = sizeof(mentry);
	while (Module32Next(handle, &mentry))
	{
		if (!strcmp(mentry.szModule, module))
		{
			CloseHandle(handle);
			return (DWORD)mentry.modBaseAddr;
		}
	}
	return 0;
}


