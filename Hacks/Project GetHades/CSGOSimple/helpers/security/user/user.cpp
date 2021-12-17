#include "user.h"
#include "../../api/hades_api.hpp"

#include "../../ctx/cheat_user.h"

#include "../xor_str.h"

#include <Windows.h>

#include <atlsimpstr.h>

#include <sstream>

security::cheat_user::cheat_user ( )
{
    if ( !init( ) )
        std::terminate( );
}

bool security::cheat_user::init ( )
{
    m_hwid = create_hwid( );

    nlohmann::json json;

    json[ xor_str ( "hwid" ) ] = m_hwid;

    auto j = hades_api::send_action ( xor_str ( "cheat_login" ), json );

    if ( j[ xor_str ( "error" ) ].get< bool >( ) )
        return false;

    ctx::cheat_user = ctx::cheat_user_t ( j[ xor_str ( "username" ) ].get< std::string >( ), j[ xor_str ( "avatar" ) ].get< std::string >( ) );

    m_authkey = j[ xor_str ( "authkey" ) ].get< std::string >( );

    return true;
}

std::string security::cheat_user::create_hwid ( ) const
{
    char computer_name[1024 + 1];
    DWORD size = 1024;
    GetComputerNameA ( computer_name, &size );
    char username[256 + 1];
    DWORD username_len = 256;
    GetUserNameA ( username, &username_len );
    int cpu_info[4] = { -1 };
    char cpu_brand_string[0x40];
    __cpuid ( cpu_info, 0x80000000 );
    const unsigned int n_ex_ids = cpu_info[ 0 ];
    ( memset ) ( cpu_brand_string, 0, sizeof ( cpu_brand_string ) );

    // Get the information associated with each extended ID.
    for ( int i = 0x80000000; i <= n_ex_ids; ++i )
    {
        __cpuid ( cpu_info, i );

        // Interpret CPU brand string.
        if ( i == 0x80000002 )
            ( memcpy ) ( cpu_brand_string, cpu_info, sizeof ( cpu_info ) );
        else if ( i == 0x80000003 )
            ( memcpy ) ( cpu_brand_string + 16, cpu_info, sizeof ( cpu_info ) );
        else if ( i == 0x80000004 )
            ( memcpy ) ( cpu_brand_string + 32, cpu_info, sizeof ( cpu_info ) );
    }

    SYSTEM_INFO si_sys_info;
    ( GetSystemInfo ) ( &si_sys_info );
    ( GetSystemInfo ) ( &si_sys_info );
    std::stringstream converter;
    converter << computer_name << xor_str ( "-" ) << username << xor_str ( "-" ) << cpu_brand_string << xor_str ( "-" ) << si_sys_info.dwOemId <<
        xor_str ( "-" ) << si_sys_info.dwNumberOfProcessors << xor_str ( "-" ) << si_sys_info.dwProcessorType;
    auto hwid = converter.str( );
    hwid.erase ( remove ( hwid.begin( ), hwid.end( ), ' ' ), hwid.end( ) );

    for ( auto i = 0; ( i < 100 && hwid[ i ] != '\0' ); i++ )
        hwid[ i ] = hwid[ i ] + ( static_cast< int > ( hwid.length( ) ) / 15 );

    hwid.resize ( remove_if ( hwid.begin( ), hwid.end( ), [] ( char x )
    {
        return !isalnum ( x ) && !isspace ( x );
    } ) - hwid.begin( ) );

    return hwid;
}
