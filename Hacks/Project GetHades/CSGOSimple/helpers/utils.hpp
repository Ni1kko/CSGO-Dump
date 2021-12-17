#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>
#include "../valve_sdk/sdk.hpp"
#include "../datamap.hpp"
#include <functional>

class c_utils
{
public:
    static unsigned int find_in_data_map ( datamap_t* pMap, const char* name );

    /*
     * @brief Wait for all the given modules to be loaded
     *
     * @param timeout How long to wait
     * @param modules List of modules to wait for
     *
     * @returns See WaitForSingleObject return values.
     */
    int wait_for_modules ( std::int32_t timeout, const std::initializer_list< std::wstring >& modules );

    float get_estimate_server_time ( usercmd_t* cmd ) const;

    //std::uint8_t* pattern_scan ( const std::string& msg, const char* signature );

    /*
     * @brief Scan for a given byte pattern on a module
     *
     * @param module    Base of the module to search
     * @param signature IDA-style byte array pattern
     *
     * @returns Address of the first occurence
     */
    std::uint8_t* pattern_scan ( std::string module_name, const char* signature );

    /*
     * @brief Set player clantag
     *
     * @param tag New clantag
     */
    void set_clantag ( const char* tag );

    /*
     * @brief Set player name
     *
     * @param name New name
     */
    void set_name ( const char* name ) const;

    /*
     * @brief Reveal the ranks of all players on the server
     *
     */
    void rank_reveal_all ( );

    void trace_line ( Vector& vec_abs_start, Vector& vec_abs_end, const unsigned int mask, C_BaseEntity* ignore, CGameTrace* ptr ) const;

    enum player_loop_flags_t
    {
        player_loop_flag_none,
        player_loop_flag_enemy_only,
        player_loop_flag_team_only
    };

    void loop_through_players ( std::function< void  ( C_BasePlayer* ) > func, player_loop_flags_t flags );

    void on_player_create ( C_BasePlayer* player );
    void on_player_destroy ( C_BasePlayer* player );
private:
    std::vector< C_BasePlayer* > current_players;
};

extern c_utils g_utils;
