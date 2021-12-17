#define NOMINMAX
#include <Windows.h>
#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "helpers/input.hpp"
#include "hooks/hooks.hpp"
#include "menu/menu.hpp"
#include "options.h"
#include "helpers/console.h"
#include "features/misc.h"
#include <chrono>
#include <thread>
#include "security/security.h"
#include "features/features.h"
#include "helpers/helpers.h"
#include "hooks/listener_entity.h"

#include "lua/lua_support.h"

namespace ctx
{
    client_t client = client_t{ };
    cheat_user_t cheat_user = cheat_user_t{ };
    cheat_t cheat = cheat_t{ };
}

HANDLE security_thread_handle = nullptr;
HANDLE check_thread_handle = nullptr;
HANDLE heartbeat_thread_handle = nullptr;
HANDLE test = nullptr;
bool s_ended = false;
bool c_ended = false;
bool h_ended = false;

bool important_threads_still_up()
{
    #ifdef _DEBUG
    return true;
    #endif // _DEBUG

    const auto s_result = LIF ( WaitForSingleObject ) ( security_thread_handle, 0 );
    const auto h_result = LIF ( WaitForSingleObject ) ( heartbeat_thread_handle, 0 );

    return ! ( s_result == WAIT_ABANDONED_0 || h_result == WAIT_ABANDONED_0 );
}

bool is_one_thread_still_up()
{
    #ifdef _RELEASE

    if ( s_ended && c_ended && h_ended )
        return false;

    DWORD SResult = LIF ( WaitForSingleObject ) ( security_thread_handle, 0 );
    DWORD HResult = LIF ( WaitForSingleObject ) ( heartbeat_thread_handle, 0 );
    DWORD CResult = LIF ( WaitForSingleObject ) ( check_thread_handle, 0 );

    return SResult != WAIT_ABANDONED_0 || HResult != WAIT_ABANDONED_0 || CResult != WAIT_ABANDONED_0;
    /*
    #else

    if ( s_ended && c_ended && h_ended )
        return false;
    	*/
    #else
    return false;
    #endif // _RELEASE
}

DWORD WINAPI OnDllAttachStart ( LPVOID base )
{
    //
    // Wait at most 10s for the main game modules to be loaded.
    //
    if ( g_utils.wait_for_modules ( 15000, { L"client_panorama.dll", L"engine.dll", L"shaderapidx9.dll" } ) == WAIT_TIMEOUT )
    {
        return FALSE; // One or more modules were not loaded in time
    }

    std::set_terminate ( []()
    {
        std::exception_ptr ptr;
        ptr = std::current_exception();

        try
        {
            if ( ptr )
                std::rethrow_exception ( ptr );
        }
        catch ( const std::exception& e )
        {
            std::string what = e.what();
            std::wstring t ( what.begin(), what.end() );
            MessageBoxA ( NULL, what.data(), xor_str ( "error" ), NULL );
            abort();
        }
    } );

    #ifdef _RELEASE

    security.init();
    #else
    console::attach();
    #endif //

    while ( !g_user.is_allowed && !g_unload )
        std::this_thread::sleep_for ( std::chrono::microseconds ( 50 ) );

    if ( g_unload )
    {
        while ( is_one_thread_still_up() )
            std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );

        console::detach();

        std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );
        LIF ( FreeLibraryAndExitThread ) ( static_cast<HMODULE> ( base ), 1 );
    }
    else
    {
        try
        {
            console::write_line ( xor_str ( "initializing..." ) );

            interfaces::initialize();
            interfaces::dump();

            g_netvarsys.Initialize();
            #ifdef _DEBUG
            g_netvarsys.Dump();
            #endif // _DEBUG
            menu.initialize();

            hooks::initialize();

            g_listener_entity.init();

            g_helpers.material_helper.init();

            // Register some hotkeys.
            // - Note:  The function that is called when the hotkey is pressed
            //          is called from the WndProc thread, not this thread.
            //

            #ifdef _DEBUG
            // Panic button
            g_inputsys.register_hotkey ( VK_DELETE, []()
            {
                g_unload = true;
            } );
            #endif

            // Menu Toggle
            g_inputsys.register_hotkey ( VK_INSERT, []()
            {
                menu.toggle();
            } );

            #ifdef _DEBUG
            console::write_line ( xor_str ( "finished." ) );
            #endif // _DEBUG

            g_features.anti_aim.setup(  );

            //lua_support::setup();

            while ( !g_unload )
            {
                if ( !security.still_valid() )
                    g_unload = true;

                std::this_thread::sleep_for ( std::chrono::milliseconds ( 500 ) );
            }

            c_misc::disable_nightmode();
            hooks::shutdown();

            con_var* var_x = interfaces::cvar->find_var ( xor_str ( "viewmodel_offset_x" ) );
            con_var* var_y = interfaces::cvar->find_var ( xor_str ( "viewmodel_offset_y" ) );
            con_var* var_z = interfaces::cvar->find_var ( xor_str ( "viewmodel_offset_z" ) );
            var_x->set_value ( g_options.org_misc_viewmodel_x );
            var_y->set_value ( g_options.org_misc_viewmodel_y );
            var_z->set_value ( g_options.org_misc_viewmodel_z );

            if ( g_local )
                g_local->flash_max_alpha() = 255.f;

            while ( is_one_thread_still_up() )
                std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );

            g_listener_entity.remove();

            event_handler.stop();

            interfaces::cvar->find_var ( xor_str ( "crosshair" ) )->set_value ( true );

            LIF ( FreeLibraryAndExitThread ) ( static_cast<HMODULE> ( base ), 1 );

        }
        catch ( const std::exception& ex )
        {
            #ifdef _DEBUG
            console::write_line ( "An error occured during initialization:" );
            console::write_line ( "%s", ex.what() );
            console::write_line ( "Press any key to exit." );
            console::read_key();
            console::detach();
            #endif // _DEBUG

            LIF ( FreeLibraryAndExitThread ) ( static_cast<HMODULE> ( base ), 1 );
        }
    }

    return FALSE;
}

DWORD WINAPI OnDllAttachStartSecurity ( LPVOID base )
{
    #ifdef _DEBUG
    console::write_line ( "security threadd!!!" );
    #endif // _DEBUG

    while ( !g_user.is_allowed && !g_unload )
        std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );

    while ( !g_unload )
    {
        security.run();
        std::this_thread::sleep_for ( std::chrono::seconds ( 2 ) );
    }

    s_ended = true;

    return TRUE;
}

DWORD WINAPI OnDllAttachCheckThread ( LPVOID base )
{
    while ( !g_user.is_allowed && !g_unload )
        std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );

    while ( !g_unload )
    {
        if ( !important_threads_still_up() )
            std::abort();

        std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );
    }

    c_ended = true;

    return TRUE;
}

DWORD WINAPI OnDllAttachStartHeartbeat ( LPVOID base )
{
    while ( !g_user.is_allowed && !g_unload )
        std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );

    while ( !g_unload )
    {
        security.run_heartbeat();
        std::this_thread::sleep_for ( std::chrono::seconds ( 2 ) );
    }

    h_ended = true;

    return TRUE;
}

BOOL WINAPI OnDllDetach()
{
    #ifdef _DEBUG
    console::attach();
    #endif

    menu.shutdown();
    return TRUE;
}

BOOL WINAPI DllMain ( _In_ HINSTANCE hinstDll, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved )
{
    switch ( fdwReason )
    {
    case DLL_PROCESS_ATTACH:
        {
            LIF ( DisableThreadLibraryCalls ) ( hinstDll );
            #ifdef _RELEASE
            security_thread_handle = LIF ( CreateThread ) ( nullptr, 0,  ( OnDllAttachStartSecurity ), hinstDll, 0, nullptr );
            heartbeat_thread_handle = LIF ( CreateThread ) ( nullptr, 0, ( OnDllAttachStartHeartbeat ), hinstDll, 0, nullptr );
            check_thread_handle = LIF ( CreateThread ) ( nullptr, 0, ( OnDllAttachCheckThread ), hinstDll, 0, nullptr );
            #endif // _RELEASE
            test = LIF ( CreateThread ) ( nullptr, 0, ( OnDllAttachStart ), hinstDll, 0, nullptr );

        }

        // CreateThread ( nullptr, 0, OnDllAttachStartSecurity, hinstDll, 0, nullptr );
        //CreateThread ( nullptr, 0, OnDllAttachStart, hinstDll, 0, nullptr );
        // add security thread start
        return TRUE;

    case DLL_PROCESS_DETACH:
        if ( lpvReserved == nullptr )
            return OnDllDetach();

        return TRUE;

    default:
        return TRUE;
    }
}