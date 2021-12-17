#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Utils\GlobalVars.h"
#include "Utils/Math.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include "Features/SkinChanger/KnifeHandler.h"

inline float RandomFloat(float min, float max) {
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

#include <ShlObj.h>
DWORD WINAPI OnDllAttach(PVOID base)
{
#ifdef _DEBUG       // Create console only in debug mode
    AllocConsole();                                 // Alloc memory and create console		
    freopen_s((FILE**)stdin,  "CONIN$", "r",  stdin);   // ----------------------------------------------
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);  //  Make iostream library use our console handle
                                                        // ----------------------------------------------
    SetConsoleTitleA(" debug console");   // Set console name to a custom one
#endif
    
    Utils::Log("Console Allocated!");
    Hooks::Init();

	//KnifeChangeDLLMAIN();
    while (true)
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
}

VOID WINAPI OnDllDetach()
{
#ifdef _DEBUG
    fclose((FILE*)stdin);
    fclose((FILE*)stdout);

    HWND hw_ConsoleHwnd = GetConsoleWindow();     //Get the HWND of the console.
    FreeConsole();                                //Detach the console.
    PostMessageW(hw_ConsoleHwnd, WM_CLOSE, 0, 0); //Destroys the window.
#endif
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) 
    {
        DisableThreadLibraryCalls(hModule);
        auto h = CreateThread(nullptr, NULL, OnDllAttach, hModule, NULL, nullptr);
		Globals::Module = HMODULE(hModule);
        CloseHandle(h);
    }
    else if (dwReason == DLL_PROCESS_DETACH) 
    {
        if (!lpReserved)
        {
            Hooks::Restore();
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        OnDllDetach();
    }
    return TRUE;
}

