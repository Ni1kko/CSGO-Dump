#include "hooks.hpp"

#define NOMINMAX
#include <Windows.h>

#include "Options.hpp"
#include "Menu.hpp"
#include "Helpers/utils.hpp"
#include "Helpers/input_system.hpp"

bool on_dll_attach(void* base)
{
    if(utils::wait_for_modules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT)
        throw std::runtime_error("Wait timed out");

    csgo::initialize();
    utils::attach_console();
#ifdef _DEBUG

    csgo::dump_info();
    netvars.dump();
#endif

    input_system::instance().initialize();

    hooks::initialize();

    while(!g_Unload) {
        Sleep(1000);
    }

    menu::instance().hide();

    Sleep(100);
    hooks::cleanup();

    Sleep(1000);

    FreeLibraryAndExitThread((HMODULE)base, 0);

}

bool on_dll_detach()
{
    utils::detach_console();
    csgo::cleanup();
    hooks::cleanup();
    return 1;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_	LPVOID    lpvReserved
)
{
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH:
            if(lpvReserved == nullptr)
                return on_dll_detach();
    }
    return TRUE;
}