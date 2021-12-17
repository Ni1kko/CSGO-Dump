#pragma once
#include "HookIncludes.h"
typedef void(__thiscall* play_sound_t)(void*, const char*);

void __stdcall hkPlaySound(const char* szFileName)
{
    static auto ofunc = hooks::surface.get_original<play_sound_t>(82);
	//Call original PlaySound
    ofunc(g_Surface, szFileName);

	if (g_Engine->IsInGame()) return;

	if (strstr(szFileName, "UI/competitive_accept_beep.wav")) 
	{

        static auto CServerConfirmedReservationCheckCallback__IsReady
            = (void(__cdecl*)(void))U::pattern_scan(
                GetModuleHandleW(L"client.dll"),
                "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"
            );
        CServerConfirmedReservationCheckCallback__IsReady();
	}
}
