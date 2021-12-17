#include "includes.h"
#include "UTILS\interfaces.h"
#include "HOOKS\hooks.h"
#include "UTILS\offsets.h"
#include "FEATURES\EventListener.h"
#include "SDK\RecvData.h"
#include "SDK\CClientEntityList.h"
#include "AWHitmarkers.h"
#include "SDK\IEngine.h"
#include "FEATURES/recv.h"
#include "HOOKS/anti_leak.h"
#include "security/check_version.h"
#include <chrono>
#include <thread>

bool using_fake_angles[65];
bool full_choke;
bool is_shooting;

bool in_tp;
bool fake_walk;

int resolve_type[65];

int target;
int shots_fired[65];
int shots_hit[65];
int shots_missed[65];
bool didMiss = true;
bool didShot = false;
int backtrack_missed[65];

float tick_to_back[65];
float lby_to_back[65];
bool backtrack_tick[65];

float lby_delta;
float update_time[65];
float walking_time[65];

float local_update;

int hitmarker_time;
int random_number;

bool menu_hide;

int oldest_tick[65];
float compensate[65][12];
Vector backtrack_hitbox[65][20][12];
float backtrack_simtime[65][12];
bool fake = false;

bool ErasePEHeader(HMODULE hModule)
{
	hModule = GetModuleHandle(NULL);
	if ((DWORD)hModule == 0) return 0;
	DWORD IMAGE_NT_HEADER = *(int*)((DWORD)hModule + 60);
	for (int i = 0; i < 0x108; i++)
		*(BYTE*)(IMAGE_NT_HEADER + i) = 0;
	for (int i = 0; i < 120; i++)
		*(BYTE*)((DWORD)hModule + i) = 0;
	return 1;
}

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}


void Start()
{
	INTERFACES::InitInterfaces();
	OFFSETS::InitOffsets();
	NetvarHook();
	UTILS::INPUT::input_handler.Init();
	FONTS::InitFonts();
	pHitmarker->Initialize();
	HOOKS::InitHooks();
	HOOKS::InitNetvarHooks();
	CGameEvents::InitializeEventListeners();
}

tool* tools;

#define VERSION (std::string)"1.0" //PROGRAM VERSION!
#define WEBSITE (std::string)"google.com" //  !!! CHANGE THIS TO YOUR DOMAIN !!!

#include <string>

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{

	if (Reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Start, NULL, NULL, NULL);
	}
		

	return true;
}