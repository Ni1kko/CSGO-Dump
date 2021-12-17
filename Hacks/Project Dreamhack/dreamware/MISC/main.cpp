#include "includes.h"

#include "../UTILS/interfaces.h"
#include "../HOOKS/hooks.h"
#include "../UTILS/offsets.h"
#include "../FEATURES/EventListener.h"
#include "../SDK/RecvData.h"
#include "../SDK/CClientEntityList.h"

#include "../SDK/IEngine.h"

#include "../MENU/menu_framework.h"
#include "recv.h"
#include "AWHitmarkers.h"
#include <chrono>
#include <thread>


bool using_fake_angles[65];
bool full_choke;
bool is_shooting;
int resolve_type[65];
bool in_tp;
bool fake_walk;
bool hurtcalled;

int target;
int shots_fired[65];
int shots_hit[65];
int shots_missed[65];
bool didMiss = true;
bool didShot = false;
int backtrack_missed[65];

bool side2 = false;
bool side1 = false;

bool back = false;

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

void Start()
{
	INTERFACES::InitInterfaces();
	OFFSETS::InitOffsets();

	UTILS::INPUT::input_handler.Init();
	FONTS::InitFonts();
	HOOKS::InitHooks();
	HOOKS::InitNetvarHooks();
	pHitmarker->Initialize();
	CGameEvents::InitializeEventListeners();
	NetvarHook();

}

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
/*	if (Reason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Start, NULL, NULL, NULL);
	return true;*/

	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
	{
#ifdef DEV_MODE
		
#endif

		
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Start, NULL, NULL, NULL);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		LOG("DETACHING");
		RemoveFontResource("C:\\Bameware\\Resources\\Fonts\\againts.ttf");

#ifdef DEV_MODE
		{
			fclose(stdin);
			fclose(stdout);
			FreeConsole();
		}
#endif

		HOOKS::UnHook();

		FreeLibraryAndExitThread(Instance, 0);

		break;
	}
	}

	return true;


}

