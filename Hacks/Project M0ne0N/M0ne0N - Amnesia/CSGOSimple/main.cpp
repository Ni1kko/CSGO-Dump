#define NOMINMAX
#include <Windows.h>

#include "config.hpp"
#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "helpers/input.hpp"

#include "hooks.hpp"
#include "menu.hpp"
#include "security.hpp"
#include "options.hpp"
#include "render.hpp"

DWORD WINAPI OnDllAttach( LPVOID base ) {

	#ifdef _RELEASE
	/*
		switch (Negr("m0ne0n.cf", "/panel/scripts/func.php?build", "gmk9r3gm9324")) {
		case 0: MessageBoxA(NULL, "Internet error, check connection with m0ne0n.cf", "M0ne0N Free", NULL); exit(0);
		case 24696:
			MessageBoxA(NULL, "Your version of the cheat is out of date!\nDownload new version on the site https://m0ne0n.cf/",
				"M0ne0N Free", NULL);
			ShellExecute(NULL, L"open", L"https://m0ne0n.cf/", NULL, NULL, SW_SHOW);
			exit(0);
		case 22971: break;
		default: MessageBoxA(NULL, "Unknown error", "M0ne0N Free", NULL); exit(0);
		*/
		}
	#endif

	// 
	// Wait at most 10s for the main game modules to be loaded.
	// 
	if (Utils::WaitForModules(10000, { L"client_panorama.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
		// One or more modules were not loaded in time
		return FALSE;
	}

#ifdef _DEBUG
	Utils::AttachConsole();
#endif

	try {
		Config->Setup();
		Utils::ConsolePrint("Initializing...\n");

		Interfaces::Initialize();
		Interfaces::Dump();

		NetvarSys::Get().Initialize();
		InputSys::Get().Initialize();
		Render::Get().Initialize();
		Menu::Get().Initialize();

		Hooks::Initialize();

		// Register some hotkeys.
		// - Note:  The function that is called when the hotkey is pressed
		//          is called from the WndProc thread, not this thread.
		// 

		// Panic button

#ifdef _DEBUG
		InputSys::Get().RegisterHotkey(VK_DELETE, [base]() {
			g_Unload = true;
		});
#endif

		// Menu Toggle
		InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
			Menu::Get().Toggle();
		});

		Utils::ConsolePrint("Finished.\n");

		while (!g_Unload)
			Sleep(1000);

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);

	}
	catch (const std::exception& ex) {
		Utils::ConsolePrint("An error occured during initialization:\n");
		Utils::ConsolePrint("%s\n", ex.what());
		Utils::ConsolePrint("Press any key to exit.\n");
		Utils::ConsoleReadKey();
		Utils::DetachConsole();

		FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
	}

	// unreachable
	//return TRUE;
}

BOOL WINAPI OnDllDetach( ) {
#ifdef _DEBUG
	Utils::DetachConsole( );
#endif

	Hooks::Shutdown( );

	Menu::Get( ).Shutdown( );
	return TRUE;
}

BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_  LPVOID    lpvReserved
) {
	switch( fdwReason ) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hinstDll );
		CreateThread( nullptr, 0, OnDllAttach, hinstDll, 0, nullptr );
		return TRUE;
	case DLL_PROCESS_DETACH:
		if( lpvReserved == nullptr )
			return OnDllDetach( );
		return TRUE;
	default:
		return TRUE;
	}
}
