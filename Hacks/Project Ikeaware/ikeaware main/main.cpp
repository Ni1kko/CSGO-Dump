#define NOMINMAX
#include <Windows.h>

#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "helpers/input.hpp"
#include "helpers/memory.hpp"

#include "valve_sdk/misc/ClassIdManager.hpp"

#include "hooks/hooks.hpp"

#include "menu.hpp"
#include "options.hpp"
#include "render.hpp"

#include "features/aimbot.hpp"
#include "features/visuals.hpp"
#include "features/glow.hpp"
#include "features/chams.hpp"
#include "features/misc.hpp"
#include "features/skins.hpp"
#include "features/profile.hpp"
#include "features/inventory.hpp"

BOOL WINAPI OnDllDetach();

DWORD WINAPI OnDllAttach(LPVOID base)
{
	if (Utils::WaitForModules(100000, { L"client.dll", L"serverbrowser.dll" }) == WAIT_TIMEOUT)
		return FALSE;

#ifdef _DEBUG
    Utils::AttachConsole();
#endif

    Utils::ConsolePrint("Initializing...\n");

    Interfaces::Initialize();
    Interfaces::Dump();

	ClassId = new ClassIdManager();

	g_NetvarSys = new NetvarSys();
	g_NetvarSys->Initialize();

	g_Menu = new Menu();
	g_Menu->Initialize();

	g_InputSys = new InputSys();
	g_InputSys->Initialize();

	g_Render = new Render();
	g_Render->Initialize();

	g_Options.Initialize();

	g_Memory = new Memory();
	g_Ragebot = new CRagebot();
	g_Legitbot = new CLegitbot();
	g_Visuals = new Visuals();
	g_SoundEsp = new SoundEsp();
	g_Glow = new Glow();
	g_Chams = new Chams();
	g_Misc = new Misc();
	g_Skins = new Skins();
	g_Inventory = new Inventory();
	g_Profile = new Profile();

    Hooks::Initialize();

	g_InputSys->RegisterHotkey(VK_INSERT, [base]() {
        g_Menu->Toggle();
    });

	g_Skins->ParseSkins();

    while(!g_Unload)
        Sleep(1000);

	OnDllDetach();
}

BOOL WINAPI OnDllDetach()
{
	if (g_Menu->IsVisible())
		g_Menu->Toggle();

#ifdef _DEBUG
    Utils::DetachConsole();
#endif

    Hooks::Shutdown();

	delete g_Misc;
	delete g_Chams;
	delete g_Glow;
	delete g_SoundEsp;
	delete g_Visuals;
	delete g_Legitbot;
	delete g_Ragebot;
	delete g_Memory;

	g_Menu->Shutdown();
	delete g_Menu;

	delete g_Render;
	delete g_InputSys;
	delete g_NetvarSys;
    return TRUE;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_  LPVOID    lpvReserved
)
{
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
            return TRUE;
        default:
            return TRUE;
    }
}
