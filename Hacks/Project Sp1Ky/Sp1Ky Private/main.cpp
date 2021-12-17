#define NOMINMAX
#include <Windows.h>
#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "helpers/input.hpp"
#include "hooks.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "./Hitmarker.h"
#include "./BulletBeams.h"
#include "AntiDBG.h"
#include "features/XorCompileTime.hpp"

DWORD WINAPI OnDllAttach(LPVOID base)
{
		if (Utils::WaitForModules(10000, { (Utils::IsPanorama() ? L"client.dll" : L"client.dll"), L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
			return FALSE;
		}
#ifdef _DEBUG
		Utils::AttachConsole();
#endif
		try {
			Utils::ConsolePrint(XorStr("Initializing...\n"));
			Interfaces::Initialize();
			Interfaces::Dump();
			HitMarkerEvent::Get().RegisterSelf();
			BulletImpactEvent::Get().RegisterSelf();
			BulletBeamsEvent::Get().RegisterSelf();
			NetvarSys::Get().Initialize();
			InputSys::Get().Initialize();
			Menu::Get().Initialize();
			Hooks::Initialize();
			InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
				Menu::Get().Toggle();
			});
			InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
				Menu::Get().Toggle();
			});
			Utils::ConsolePrint(XorStr("Finished.\n"));
			while (!g_Unload)
				Sleep(1000);
			g_CVar->FindVar(XorStr("crosshair"))->SetValue(true);
			FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
		}
		catch (const std::exception& ex) {
			Utils::ConsolePrint(XorStr("An error occured during initialization:\n"));
			Utils::ConsolePrint(XorStr("%s\n"), ex.what());
			Utils::ConsolePrint(XorStr("Press any key to exit.\n"));
			Utils::ConsoleReadKey();
			Utils::DetachConsole();
			FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
		}
		//return TRUE;
}
BOOL WINAPI OnDllDetach()
{
#ifdef _DEBUG
    Utils::DetachConsole();
#endif
	HitMarkerEvent::Get().UnregisterSelf();
	BulletImpactEvent::Get().UnregisterSelf();
	BulletBeamsEvent::Get().UnregisterSelf();
    Hooks::Shutdown();
    Menu::Get().Shutdown();
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
        case DLL_PROCESS_DETACH:
            if(lpvReserved == nullptr)
                return OnDllDetach();
            return TRUE;
        default:
            return TRUE;
    }
}



















































