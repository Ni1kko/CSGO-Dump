#include "security.h"
#undef _HAS_STD_BYTE
#include "WinHttpClient.h"
#include "../helpers/console.h"
#include "../options.h"
#include <iostream>
#include <filesystem>
#include <TlHelp32.h>
#include <chrono>
#include <cstdio>
#include <windows.h>

// todo make delay before crash bigger and dont close on connection fail

using json = nlohmann::json;

c_security security;

typedef NTSTATUS ( NTAPI* pfnNtQueryInformationProcess ) (
    _In_ HANDLE ProcessHandle,
    _In_ UINT ProcessInformationClass,
    _Out_ PVOID ProcessInformation,
    _In_ ULONG ProcessInformationLength,
    _Out_opt_ PULONG ReturnLength
);

DWORD g_origCrc = 0x2bd0;

DWORD CalcFuncCrc ( PUCHAR funcBegin, PUCHAR funcEnd )
{
    DWORD crc = 0;

    for ( ; funcBegin < funcEnd; ++funcBegin )
        crc += *funcBegin;

    return crc;
}

#pragma auto_inline(off)
VOID debuggee_function ( )
{
    auto calc = 0;
    calc += 2;
    calc <<= 8;
    calc -= 3;
}

VOID debuggee_function_end ( )
{
};
#pragma auto_inline(on)

HANDLE GetProcessByName ( PCSTR name )
{
#ifdef _RELEASE
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot =  ( CreateToolhelp32Snapshot ) ( TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32 process;
    ZeroMemory ( &process, sizeof ( process ) );
    process.dwSize = sizeof ( process );

    // Walkthrough all processes.
    if (  ( Process32First ) ( snapshot, &process ) )
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
    // trim .exe if necessary, etc.

            // std::wstring tmp = process.szExeFile;

            if ( std::string ( process.szExeFile ) == std::string ( name ) )
            {
                pid = process.th32ProcessID;
                break;
            }
        }
        while (  ( Process32Next ) ( snapshot, &process ) );
    }

    ( CloseHandle ) ( snapshot );

    if ( pid != 0 )
        return  ( OpenProcess ) ( PROCESS_ALL_ACCESS, FALSE, pid );

    // Not found
#endif // _RELEASE

    return NULL;
}

void c_security::run ( )
{
    check_for_analysis_process( );

    /////////////////////////////////////////////////////////////////// ISDEBUGGERPRESENT
    {
        if ( ( IsDebuggerPresent )( ) )
        {
            if ( this->hwid != xor_str ( "" ) )
            {
                json j;
                j[ xor_str ( "type" ) ] = xor_str ( "debugger" );
                j[ xor_str ( "hwid" ) ] = this->hwid;
                j[ xor_str ( "authkey" ) ] = this->authkey;
                inform_about_security_issue ( xor_str ( "security_alert_cheat" ), j );
            }

            std::abort( );
        }
    }

    //////////////////////////////////////////////////////////////// CHECKREMOTEDEBUGGERPRESENT
    {
        BOOL isDebuggerPresent = FALSE;

        if ( ( CheckRemoteDebuggerPresent ) ( ( GetCurrentProcess )( ), &isDebuggerPresent ) )
        {
            if ( isDebuggerPresent )
            {
                if ( this->hwid != xor_str ( "" ) )
                {
                    json j;
                    j[ xor_str ( "type" ) ] = xor_str ( "debugger" );
                    j[ xor_str ( "hwid" ) ] = this->hwid;
                    j[ xor_str ( "authkey" ) ] = this->authkey;
                    inform_about_security_issue ( xor_str ( "security_alert_cheat" ), j );
                }

                std::abort( );
            }
        }
    }

    //////////////////////////////////////////////////////////////// NTQUERYINFORMATIONPROCESS
    {
        pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
        DWORD isDebuggerPresent2 = 0;
        HMODULE hNtDll = ( LoadLibraryA ) ( xor_str ( "ntdll.dll" ) );

        if ( NULL != hNtDll )
        {
            NtQueryInformationProcess = ( pfnNtQueryInformationProcess )GetProcAddress ( hNtDll, xor_str ( "NtQueryInformationProcess" ) );

            if ( NULL != NtQueryInformationProcess )
            {
                NTSTATUS status = NtQueryInformationProcess (
                    GetCurrentProcess( ),
                    7,
                    &isDebuggerPresent2,
                    sizeof ( DWORD ),
                    NULL );

                if ( status == 0x00000000 && isDebuggerPresent2 != 0 )
                {
                    if ( this->hwid != xor_str ( "" ) )
                    {
                        json j;
                        j[ xor_str ( "type" ) ] = xor_str ( "ntquery_process" );
                        j[ xor_str ( "hwid" ) ] = this->hwid;
                        j[ xor_str ( "authkey" ) ] = this->authkey;
                        inform_about_security_issue ( xor_str ( "security_alert_cheat" ), j );
                    }

                    std::abort( );
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////// FUNCCRC
    {
        DWORD crc = ( CalcFuncCrc ) ( ( PUCHAR )debuggee_function, ( PUCHAR )debuggee_function_end );

        if ( g_origCrc == crc )
        {
            if ( this->hwid != xor_str ( "" ) )
            {
                json j;
                j[ xor_str ( "type" ) ] = xor_str ( "fuc_crc" );
                j[ xor_str ( "hwid" ) ] = this->hwid;
                j[ xor_str ( "authkey" ) ] = this->authkey;
                inform_about_security_issue ( xor_str ( "security_alert_cheat" ), j );
            }

            std::abort( );
        }
    }

    //////////////////////////////////////////////////////////////// GETTHREADCONTEXT
    {
        CONTEXT ctx = { };
        ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        if ( ( GetThreadContext ) ( ( GetCurrentThread )( ), &ctx ) )
        {
            if ( ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0 )
            {
                if ( this->hwid != xor_str ( "" ) )
                {
                    json j;
                    j[ xor_str ( "type" ) ] = xor_str ( "get_thread_context" );
                    j[ xor_str ( "hwid" ) ] = this->hwid;
                    j[ xor_str ( "authkey" ) ] = this->authkey;
                    inform_about_security_issue ( xor_str ( "security_alert_cheat" ), j );
                }

                std::abort( );
            }
        }
    }
}

nlohmann::json c_security::inform_about_security_issue ( std::string action, json j )
{
    WinHttpClient client ( std::wstring ( url.begin( ), url.end( ) ) );
    // Set post data.
    std::string data = xor_str ( "action=" ) + action + xor_str ( "&data=" ) + j.dump( );
    client.SetAdditionalDataToSend ( ( BYTE* )data.c_str( ), data.size( ) );
    client.SetUserAgent ( std::wstring ( useragent.begin( ), useragent.end( ) ) );

    // Set request headers.
    wchar_t szSize[50] = L"";
    swprintf_s ( szSize, L"%d", data.size( ) );
    std::wstring headers = L"Content-Length: ";
    headers += szSize;
    headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
    client.SetAdditionalRequestHeaders ( headers );

    // Send HTTP post request.
    client.SendHttpRequest ( L"POST" );
    std::wstring tmp_res = client.GetResponseContent( );
    std::string response = std::string ( tmp_res.begin( ), tmp_res.end( ) );

    if ( response == xor_str ( "" ) )
        return json( );

    json tmp = json::parse ( response );

    return tmp;
}

void c_security::run_heartbeat ( )
{
    if ( !info_data.empty( ) )
        send_resolver_data( );

    if ( this->authkey.empty( ) )
        return;

    try
    {
        WinHttpClient client ( std::wstring ( url.begin( ), url.end( ) ) );
        json j;
        j[ xor_str ( "hwid" ) ] = this->hwid;
        j[ xor_str ( "authkey" ) ] = this->authkey;

        // Set post data.
        std::string data = xor_str ( "action=cheat_heartbeat&data=" ) + std::string ( j.dump( ) );
        client.SetAdditionalDataToSend ( ( BYTE* )data.c_str( ), data.size( ) );
        client.SetUserAgent ( std::wstring ( useragent.begin( ), useragent.end( ) ) );

        // Set request headers.
        wchar_t szSize[50] = L"";
        swprintf_s ( szSize, L"%d", data.size( ) );
        std::wstring headers = L"Content-Length: ";
        headers += szSize;
        headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
        client.SetAdditionalRequestHeaders ( headers );

        // Send HTTP post request.
        client.SendHttpRequest ( L"POST" );
        std::wstring tmp_res = client.GetResponseContent( );
        std::string response = std::string ( tmp_res.begin( ), tmp_res.end( ) );
        j = json::parse ( response );

        if ( j[ xor_str ( "error" ) ].get< bool >( ) )
        {
            std::abort( );
            g_unload = true;
            return;
        }

        this->last_run_time = std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now( ) );
    }
    catch ( ... )
    {
    }
}

bool c_security::still_valid ( ) const
{
    return true;
#ifdef _DEBUG
    return true;
#endif // _DEBUG

    return ( last_run_time - std::chrono::system_clock::to_time_t ( std::chrono::system_clock::now( ) ) < 60.f );
}

void c_security::init ( )
{
    //if ( is_inside_vmware() || is_inside_vpc() )
    //    std::abort();

    char computerName[1024 + 1];
    DWORD size = 1024;
    GetComputerNameA ( computerName, &size );
    char username[256 + 1];
    DWORD username_len = 256;
    GetUserNameA ( username, &username_len );
    int CPUInfo[4] = { -1 };
    char CPUBrandString[0x40];
    __cpuid ( CPUInfo, 0x80000000 );
    unsigned int nExIds = CPUInfo[ 0 ];
    ( memset ) ( CPUBrandString, 0, sizeof ( CPUBrandString ) );

    // Get the information associated with each extended ID.
    for ( int i = 0x80000000; i <= nExIds; ++i )
    {
        __cpuid ( CPUInfo, i );

        // Interpret CPU brand string.
        if ( i == 0x80000002 )
            ( memcpy ) ( CPUBrandString, CPUInfo, sizeof ( CPUInfo ) );
        else if ( i == 0x80000003 )
            ( memcpy ) ( CPUBrandString + 16, CPUInfo, sizeof ( CPUInfo ) );
        else if ( i == 0x80000004 )
            ( memcpy ) ( CPUBrandString + 32, CPUInfo, sizeof ( CPUInfo ) );
    }

    SYSTEM_INFO siSysInfo;
    ( GetSystemInfo ) ( &siSysInfo );
    ( GetSystemInfo ) ( &siSysInfo );
    std::stringstream converter;
    converter << computerName << xor_str ( "-" ) << username << xor_str ( "-" ) << CPUBrandString << xor_str ( "-" ) << siSysInfo.dwOemId << xor_str ( "-" )
        << siSysInfo.dwNumberOfProcessors << xor_str ( "-" ) << siSysInfo.dwProcessorType;
    std::string returner = converter.str( );
    returner.erase ( remove ( returner.begin( ), returner.end( ), ' ' ), returner.end( ) );

    for ( int i = 0; ( i < 100 && returner[ i ] != '\0' ); i++ )
        returner[ i ] = returner[ i ] + ( static_cast< int > ( returner.length( ) ) / 15 );

    returner.resize ( remove_if ( returner.begin( ), returner.end( ), [] ( char x )
    {
        return !isalnum ( x ) && !isspace ( x );
    } ) - returner.begin( ) );

    this->hwid = returner;

    // making web request to check with hwid
    try
    {
        WinHttpClient client ( std::wstring ( url.begin( ), url.end( ) ) );

        json j;
        j[ xor_str ( "hwid" ) ] = this->hwid;

        // Set post data.
        std::string data = xor_str ( "action=cheat_login&data=" ) + std::string ( j.dump( ) );
        client.SetAdditionalDataToSend ( ( BYTE* )data.c_str( ), data.size( ) );
        client.SetUserAgent ( std::wstring ( useragent.begin( ), useragent.end( ) ) );

        // Set request headers.
        wchar_t szSize[50] = L"";
        swprintf_s ( szSize, L"%d", data.size( ) );
        std::wstring headers = L"Content-Length: ";
        headers += szSize;
        headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
        client.SetAdditionalRequestHeaders ( headers );

        // Send HTTP post request.
        client.SendHttpRequest ( L"POST" );
        std::wstring tmp_res = client.GetResponseContent( );

        j = json::parse ( tmp_res );

        if ( j[ xor_str ( "error" ) ].get< bool >( ) )
        {
            g_unload = true;
            return;
        }

        this->authkey = j[ xor_str ( "authkey" ) ].get< std::string >( );
        g_user.is_allowed = true;
    }
    catch ( ... )
    {
        std::abort( );
    }
}

void c_security::check_for_analysis_process ( )
{
    std::string szProcesses[] =
    {
        xor_str ( ( "ollydbg.exe" ) ), // OllyDebug debugger
        xor_str ( ( "ProcessHacker.exe" ) ), // Process Hacker
        xor_str ( ( "tcpview.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "autoruns.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "autorunsc.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "filemon.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "procmon.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "regmon.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "procexp.exe" ) ), // Part of Sysinternals Suite
        xor_str ( ( "idaq.exe" ) ), // IDA Pro Interactive Disassembler
        xor_str ( ( "idaq64.exe" ) ), // IDA Pro Interactive Disassembler
        xor_str ( ( "ImmunityDebugger.exe" ) ), // ImmunityDebugger
        xor_str ( ( "Wireshark.exe" ) ), // Wireshark packet sniffer
        xor_str ( ( "dumpcap.exe" ) ), // Network traffic dump tool
        xor_str ( ( "HookExplorer.exe" ) ), // Find various types of runtime hooks
        xor_str ( ( "ImportREC.exe" ) ), // Import Reconstructor
        xor_str ( ( "PETools.exe" ) ), // PE Tool
        xor_str ( ( "LordPE.exe" ) ), // LordPE
        xor_str ( ( "dumpcap.exe" ) ), // Network traffic dump tool
        xor_str ( ( "SysInspector.exe" ) ), // ESET SysInspector
        xor_str ( ( "proc_analyzer.exe" ) ), // Part of SysAnalyzer iDefense
        xor_str ( ( "sysAnalyzer.exe" ) ), // Part of SysAnalyzer iDefense
        xor_str ( ( "sniff_hit.exe" ) ), // Part of SysAnalyzer iDefense
        xor_str ( ( "windbg.exe" ) ), // Microsoft WinDbg
        xor_str ( ( "joeboxcontrol.exe" ) ), // Part of Joe Sandbox
        xor_str ( ( "joeboxserver.exe" ) ), // Part of Joe Sandbox
        xor_str ( ( "fiddler.exe" ) ),
        xor_str ( ( "TeamViewer_Service.exe" ) ),
        xor_str ( ( "TeamViewer.exe" ) ),
        xor_str ( ( "tv_w32.exe" ) ),
        xor_str ( ( "tv_x64.exe" ) ),
    };

    WORD iLength = sizeof ( szProcesses ) / sizeof ( szProcesses[ 0 ] );

    for ( int i = 0; i < iLength; i++ )
    {
        if ( ( GetProcessByName ) ( szProcesses[ i ].c_str( ) ) )
        {
            json j;
            j[ xor_str ( "name" ) ] = szProcesses[ i ];
            inform_about_security_issue ( xor_str ( "report_process_cheat" ), j );
            std::abort( );
        }
    }

    std::string ProcessWindowNames[] =
    {
        xor_str ( "Process Hacker" ),
        xor_str ( "OllyDBG" ),
        xor_str ( "IDA" )
    };

    iLength = sizeof ( ProcessWindowNames ) / sizeof ( ProcessWindowNames[ 0 ] );

    for ( int i = 0; i < iLength; i++ )
    {
        if ( ( FindWindowExA ) ( nullptr, nullptr, ProcessWindowNames[ i ].c_str( ), ProcessWindowNames[ i ].c_str( ) ) )
        {
            json j;
            j[ xor_str ( "name" ) ] = ProcessWindowNames[ i ];
            inform_about_security_issue ( xor_str ( "report_process_cheat" ), j );
            std::abort( );
        }
    }
}

void c_security::send_resolver_data ( )
{
    try
    {
        for ( auto info : info_data )
        {
            WinHttpClient client ( std::wstring ( url.begin( ), url.end( ) ) );
            json j;
            j[ xor_str ( "resolved" ) ] = info.resolved;
            j[ xor_str ( "hit" ) ] = info.did_hit;
            j[ xor_str ( "shots" ) ] = info.shots;
            j[ xor_str ( "damage" ) ] = info.damage;
            j[ xor_str ( "missed_due_to_spread" ) ] = info.missed_due_to_spread;
            j[ xor_str ( "mode" ) ] = info.mode_name;

            // Set post data.
            std::string data = xor_str ( "action=resolver_data&data=" ) + std::string ( j.dump( ) );
            client.SetAdditionalDataToSend ( ( BYTE* )data.c_str( ), data.size( ) );
            client.SetUserAgent ( std::wstring ( useragent.begin( ), useragent.end( ) ) );

            // Set request headers.
            wchar_t szSize[50] = L"";
            swprintf_s ( szSize, L"%d", data.size( ) );
            std::wstring headers = L"Content-Length: ";
            headers += szSize;
            headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
            client.SetAdditionalRequestHeaders ( headers );

            // Send HTTP post request.
            client.SendHttpRequest ( L"POST" );
        }
    }
    catch ( ... )
    {
    }

    info_data.clear( );
}

bool c_security::is_inside_vmware ( ) const
{
    bool rc = true;

    __try
    {
        __asm
        {
            push edx
            push ecx
            push ebx

            mov eax, 'VMXh'
            mov ebx, 0 // any value but not the MAGIC VALUE
            mov ecx, 10 // get VMWare version
            mov edx, 'VX' // port number

            in eax, dx // read port
            // on return EAX returns the VERSION
            cmp ebx, 'VMXh' // is it a reply from VMWare?
            setz[rc] // set return value

            pop ebx
            pop ecx
            pop edx
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        rc = false;
    }

    return rc;
}

DWORD __forceinline IsInsideVPC_exceptionFilter ( LPEXCEPTION_POINTERS ep )
{
    PCONTEXT ctx = ep->ContextRecord;

    ctx->Ebx = -1; // Not running VPC
    ctx->Eip += 4; // skip past the "call VPC" opcodes
    return EXCEPTION_CONTINUE_EXECUTION;
    // we can safely resume execution since we skipped faulty instruction
}

bool c_security::is_inside_vpc ( ) const
{
    bool rc = true;

    __try
    {
        _asm push ebx
        _asm mov ebx, 0 // It will stay ZERO if VPC is running
        _asm mov eax, 1 // VPC function number

        // call VPC
        _asm __emit 0Fh
        _asm __emit 3Fh
        _asm __emit 07h
        _asm __emit 0Bh

        _asm test ebx, ebx
        _asm setz[rc]
        _asm pop ebx
    }

        // The except block shouldn't get triggered if VPC is running!!
    __except ( IsInsideVPC_exceptionFilter ( GetExceptionInformation( ) ) )
    {
        rc = false;
    }

    return rc;
}

std::string c_security::get_string ( std::wstring istr )
{
    return std::string ( istr.begin( ), istr.end( ) );
}
