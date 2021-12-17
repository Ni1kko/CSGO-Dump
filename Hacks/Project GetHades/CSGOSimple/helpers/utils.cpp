#include "utils.hpp"
#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "../valve_sdk/csgostructs.hpp"
#include "math.hpp"

c_utils g_utils;

unsigned int c_utils::find_in_data_map ( datamap_t* pMap, const char* name )
{
    while ( pMap )
    {
        for ( int i = 0; i < pMap->dataNumFields; i++ )
        {
            if ( pMap->dataDesc[ i ].fieldName == nullptr )
                continue;

            if ( strcmp ( name, pMap->dataDesc[ i ].fieldName ) == 0 )
                return pMap->dataDesc[ i ].fieldOffset[ TD_OFFSET_NORMAL ];

            if ( pMap->dataDesc[ i ].fieldType == FIELD_EMBEDDED )
            {
                if ( pMap->dataDesc[ i ].td )
                {
                    unsigned int offset;

                    if ( ( offset = find_in_data_map ( pMap->dataDesc[ i ].td, name ) ) != 0 )
                        return offset;
                }
            }
        }

        pMap = pMap->baseMap;
    }

    return 0;
}

/*
 * @brief Wait for all the given modules to be loaded
 *
 * @param timeout How long to wait
 * @param modules List of modules to wait for
 *
 * @returns See WaitForSingleObject return values.
 */
int c_utils::wait_for_modules ( std::int32_t timeout, const std::initializer_list< std::wstring >& modules )
{
    bool signaled[32] = { false };
    auto success = false;

    std::uint32_t totalSlept = 0;

    if ( timeout == 0 )
    {
        for ( auto& mod : modules )
        {
            if ( GetModuleHandleW ( std::data ( mod ) ) == NULL )
                return WAIT_TIMEOUT;
        }

        return WAIT_OBJECT_0;
    }

    if ( timeout < 0 )
        timeout = INT32_MAX;

    while ( true )
    {
        for ( auto i = 0u; i < modules.size( ); ++i )
        {
            auto& module = * ( modules.begin( ) + i );

            if ( !signaled[ i ] && GetModuleHandleW ( std::data ( module ) ) != nullptr )
            {
                signaled[ i ] = true;

                //
                // Checks if all modules are signaled
                //
                bool done = true;

                for ( auto j = 0u; j < modules.size( ); ++j )
                {
                    if ( !signaled[ j ] )
                    {
                        done = false;
                        break;
                    }
                }

                if ( done )
                {
                    success = true;
                    goto exit;
                }
            }
        }

        if ( totalSlept > std::uint32_t ( timeout ) )
            break;

        Sleep ( 10 );
        totalSlept += 10;
    }

exit:
    return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
}

float c_utils::get_estimate_server_time ( usercmd_t* cmd ) const
{
    //double v0; // st7@0

    const auto v1 = static_cast< INetChannelInfo* > ( interfaces::engine_client->get_net_channel_info( ) );

    const auto v3 = v1->GetAvgLatency ( INetChannelInfo::LOCALPLAYER );
    const auto v4 = v1->GetAvgLatency ( INetChannelInfo::GENERIC );

    return v3 + v4 + ticks_to_time ( 1 ) + ticks_to_time ( cmd->tick_count );
}

//std::uint8_t* c_utils::pattern_scan ( const std::string& msg, const char* signature )
//{
//    return pattern_scan ( LIF ( GetModuleHandleA ) ( msg ), signature );
//}

/*
 * @brief Scan for a given byte pattern on a module
 *
 * @param module    Base of the module to search
 * @param signature IDA-style byte array pattern
 *
 * @returns Address of the first occurence
 */
std::uint8_t* c_utils::pattern_scan ( std::string module_name, const char* signature )
{
    void* module = LIF ( GetModuleHandleA ) ( module_name.data( ) );

    static auto pattern_to_byte = [] ( const char* pattern )
    {
        auto bytes = std::vector< int >{ };
        auto start = const_cast< char* > ( pattern );
        auto end = const_cast< char* > ( pattern ) + strlen ( pattern );

        for ( auto current = start; current < end; ++current )
        {
            if ( *current == '?' )
            {
                ++current;

                if ( *current == '?' )
                    ++current;

                bytes.push_back ( -1 );
            }
            else
                bytes.push_back ( strtoul ( current, &current, 16 ) );
        }

        return bytes;
    };

    auto dosHeader = ( PIMAGE_DOS_HEADER )module;
    auto ntHeaders = ( PIMAGE_NT_HEADERS )( ( std::uint8_t* )module + dosHeader->e_lfanew );

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte ( signature );
    auto scanBytes = reinterpret_cast< std::uint8_t* > ( module );

    auto s = patternBytes.size( );
    auto d = patternBytes.data( );

    for ( auto i = 0ul; i < sizeOfImage - s; ++i )
    {
        bool found = true;

        for ( auto j = 0ul; j < s; ++j )
        {
            if ( scanBytes[ i + j ] != d[ j ] && d[ j ] != -1 )
            {
                found = false;
                break;
            }
        }

        if ( found )
            return &scanBytes[ i ];
    }

    return nullptr;
}

/*
 * @brief Set player clantag
 *
 * @param tag New clantag
 */
void c_utils::set_clantag ( const char* tag )
{
    using ClantagChanged = int ( __fastcall*) ( const char*, const char* );
    static uint8_t* fnClantagChanged = pattern_scan ( xor_str ( "engine.dll" ), xor_str ( "53 56 57 8B DA 8B F9 FF 15" ) );

    reinterpret_cast< ClantagChanged > ( fnClantagChanged ) ( tag, tag );
}

/*
 * @brief Set player name
 *
 * @param name New name
 */
void c_utils::set_name ( const char* name ) const
{
    static auto nameConvar = interfaces::cvar->find_var ( "name" );
    nameConvar->m_fnChangeCallbacks.m_Size = 0;

    // Fix so we can change names how many times we want
    // This code will only run once because of `static`
    static auto do_once = ( nameConvar->set_value ( "\n���" ), true );

    nameConvar->set_value ( name );
}

/*
 * @brief Reveal the ranks of all players on the server
 *
 */
void c_utils::rank_reveal_all ( )
{
    using ServerRankRevealAll = char ( __cdecl*) ( int* );

    static uint8_t* fnServerRankRevealAll = pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                           xor_str (
                                                               "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D" ) );

    if ( fnServerRankRevealAll )
    {
        int v[3] = { 0, 0, 0 };

        reinterpret_cast< ServerRankRevealAll > ( fnServerRankRevealAll ) ( v );
    }
}

void c_utils::trace_line ( Vector& vec_abs_start, Vector& vec_abs_end, const unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr ) const
{
    CTraceFilter filter;
    filter.pSkip = ignore;
    Ray_t ray;
    ray.Init ( vec_abs_start, vec_abs_end );
    interfaces::engine_trace->trace_ray ( ray, mask, &filter, ptr );
}

void c_utils::on_player_create ( C_BasePlayer* player )
{
    this->current_players.push_back ( player );
}

void c_utils::on_player_destroy ( C_BasePlayer* player )
{
    auto pos = std::find ( this->current_players.begin( ), this->current_players.end( ), player );

    if ( pos != this->current_players.end( ) )
        this->current_players.erase ( pos );
}

void c_utils::loop_through_players ( std::function< void  ( C_BasePlayer* ) > func, const player_loop_flags_t flags )
{
    for ( auto player : this->current_players )
    {
        switch ( flags )
        {
        case c_utils::player_loop_flag_none:
            if ( !player || !player->is_alive( ) || player->is_dormant( ) )
                continue;

            break;

        case c_utils::player_loop_flag_enemy_only:
            if ( !player || !player->is_alive( ) || player->is_dormant( ) || !player->is_enemy( ) ) // no local check needed bc local is same team
                continue;

            break;

        case c_utils::player_loop_flag_team_only:
            if ( !player || !player->is_alive( ) || player->is_dormant( ) || player->ent_index( ) == interfaces::engine_client->get_localplayer( ) || player
                ->is_enemy( ) ) // no local check needed bc local is same team
                continue;

            break;
        }

        func ( player );
    }
}
