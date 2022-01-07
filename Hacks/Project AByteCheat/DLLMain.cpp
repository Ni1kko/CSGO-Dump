#include "DLLMain.h"
#include "Utilities.h"
#include "INJ/ReflectiveLoader.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"

#include "hitmarker.h"
#include "AntiLeak.h"
#include <d3d9.h>
#include <strstream>
#include <fstream>
#include "Skinchanger.h"
#include <iostream>
#include <ostream>
#include "CBulletListener.h"
#include "Dumping.h"

int missedLogHits[65];
template <int XORSTART, int BUFLEN, int XREFKILLER>

class Xor
{
private:
	Xor();
public:
	char s[BUFLEN];

	Xor(const char* xs);
	~Xor()
	{
		for (int i = 0; i < BUFLEN; i++) s[i] = 0;
	}
};

template <int XORSTART, int BUFLEN, int XREFKILLER>

Xor<XORSTART, BUFLEN, XREFKILLER>::Xor(const char* xs)
{
	int xvalue = XORSTART;
	int i = 0;
	for (; i < (BUFLEN - 1); i++)
	{
		s[i] = xs[i - XREFKILLER] ^ xvalue;
		xvalue += 1;
		xvalue %= 256;
	}
	s[BUFLEN - 1] = (2 * 2 - 3) - 1;
}

extern HINSTANCE hAppInstance;

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

bool ErasePEHeader(HMODULE hModule)
{
	hModule = GetModuleHandle(NULL);
	if ((DWORD)hModule == 0) return 0;
	DWORD IMAGE_NT_HEADER = *(int*)((DWORD)hModule + 60);
	for (int i = 0; i < 0x108; i++)
		* (BYTE*)(IMAGE_NT_HEADER + i) = 0;
	for (int i = 0; i < 120; i++)
		* (BYTE*)((DWORD)hModule + i) = 0;
	return 1;
}

HINSTANCE HThisModule;
bool DoUnload;


bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void create_folder(const char* path) {
	if (!CreateDirectory(path, NULL)) {
		return;
	}
}

void initialize_configs_file() {

	create_folder("bytecheats");
	create_folder("ByteCheats\\");

	if (!is_file_exist("ByteCheats\\ByteCheats_ConfigList.txt")) {
		std::ofstream("ByteCheats\\ByteCheats_ConfigList.txt");
	}
}

void CAntiLeak::ErasePE()
{
	char* pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}

int InitialThread()
{
	PrintMetaHeader();
	initialize_configs_file();
	Offsets::Initialise();
	interfaces::Initialise();
	skinchanger.set_skins();
	NetVar.RetrieveClasses();
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	hitmarker::singleton()->initialize();
	Hacks::SetupHacks();
	options::SetupMenu();
	Hooks::Initialise();
	CBulletListener::singleton()->init();
	FLASHWINFO flash;
	flash.cbSize = sizeof(FLASHWINFO);
	flash.hwnd = interfaces::window;
	flash.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	flash.uCount = 0;
	flash.dwTimeout = 0;
	FlashWindowEx(&flash);
	while (DoUnload == false)
	{
		Sleep(1000);
	}
	Hooks::UndoHooks();
	Sleep(2000);
	FreeLibraryAndExitThread(HThisModule, 0);
	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);
		Sleep(100);
		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}