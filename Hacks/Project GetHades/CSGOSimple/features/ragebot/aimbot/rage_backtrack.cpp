#include "rage_backtrack.h"
#include "../../../helpers/math.hpp"
#include "../../../helpers/console.h"
#include "../../features.h"

void c_rage_backtrack::on_create_move ( )
{
    add_record( );
}

float c_rage_backtrack::get_lerp_time ( ) const
{
    auto ud_rate = interfaces::cvar->find_var ( xor_str ( "cl_updaterate" ) )->get_int( );
    static auto min_ud_rate = interfaces::cvar->find_var ( xor_str ( "sv_minupdaterate" ) );
    static auto max_ud_rate = interfaces::cvar->find_var ( xor_str ( "sv_maxupdaterate" ) );

    if ( min_ud_rate && max_ud_rate )
        ud_rate = max_ud_rate->get_int( );

    auto ratio = interfaces::cvar->find_var ( xor_str ( "cl_interp_ratio" ) )->get_float( );

    if ( ratio == 0 )
        ratio = 1.0f;

    auto lerp = interfaces::cvar->find_var ( xor_str ( "cl_interp" ) )->get_float( );
    static auto c_min_ratio = interfaces::cvar->find_var ( xor_str ( "sv_client_min_interp_ratio" ) );
    static auto c_max_ratio = interfaces::cvar->find_var ( xor_str ( "sv_client_max_interp_ratio" ) );

    if ( c_min_ratio && c_max_ratio && c_min_ratio->get_float( ) != 1 )
        ratio = std::clamp ( ratio, c_min_ratio->get_float( ), c_max_ratio->get_float( ) );

    return std::max< float > ( lerp, ratio / ud_rate );
}

bool c_rage_backtrack::tick_valid ( int tick ) const
{
    const auto nci = interfaces::engine_client->get_net_channel_info( );
    static auto sv_maxunlag = interfaces::cvar->find_var ( xor_str ( "sv_maxunlag" ) );

    if ( !nci || !sv_maxunlag )
        return false;

    const auto correct = std::clamp ( nci->GetLatency ( 0 ) + nci->GetLatency ( 1 ) + get_lerp_time( ), 0.f, sv_maxunlag->get_float( ) );
    const auto deltaTime = correct - ( interfaces::global_vars->curtime - ticks_to_time ( tick ) );

    return fabsf ( deltaTime ) < 0.2f;
}

void c_rage_backtrack::add_record ( )
{
    for ( auto i = 1; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !g_local || !entity->is_player( ) || entity == g_local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        player_backup_data_t _data;
        backtrack_all_data data;
        _data.got = _data.create ( entity );

        data.ndata = _data;
        data.rdata = g_features.resolver.rdata.at ( i );
        data.simtime = entity->simulation_time( );
        data.creation_time = interfaces::global_vars->realtime;

        g_features.rage_backtrack.saved_data[ i ].push_back ( data );

        for ( auto record = g_features.rage_backtrack.saved_data[ i ].begin( ); record != g_features.rage_backtrack.saved_data[ i ].end( ); record++ )
        {
            if ( !record->ndata.got || !tick_valid ( time_to_ticks ( record->simtime ) ) )
            {
                g_features.rage_backtrack.saved_data[ i ].erase ( record );

                if ( !g_features.rage_backtrack.saved_data[ i ].empty( ) )
                    record = g_features.rage_backtrack.saved_data[ i ].begin( );
                else
                    break;
            }
        }
    }
}

player_backup_data_t c_rage_backtrack::get_normal_data ( int i ) const
{
    if ( g_features.rage_backtrack.saved_data[ i ].size( ) == 0 )
        return player_backup_data_t( );

    const auto t = g_features.rage_backtrack.saved_data[ i ].end( );

    return t->ndata;
}

backtrack_all_data c_rage_backtrack::get_last_backtrack_data ( int i )
{
    auto last_simtime = FLT_MAX;
    backtrack_all_data best_r;

    for ( auto data : g_features.rage_backtrack.saved_data[ i ] )
    {
        if ( data.ndata.got && tick_valid ( time_to_ticks ( data.simtime ) ) )
        {
            if ( last_simtime > data.simtime )
            {
                last_simtime = data.simtime;
                best_r = data;
            }
        }
    }

    return best_r;
}

bool player_backtrack_t::backup ( C_BasePlayer* player, matrix3x4_t* matrix )
{
    auto renderable = player->get_client_renderable( );
    auto bone_count = *reinterpret_cast< int* > ( uintptr_t ( renderable ) + 0x2918 );
    // #ref: SetupBones "SetupBones: invalid bone array size (%d - needs %d)\n"
    auto bone_cache = reinterpret_cast< void* > ( uintptr_t ( renderable ) + 0x290C );
    // #ref: SetupBones "SetupBones: invalid bone array size (%d - needs %d)\n"

    return false;
}

bool player_backtrack_t::restore ( C_BasePlayer* player )
{
    return false;
}

player_backup_data_t::player_backup_data_t ( )
{
    flags = 0;
    got = false;
}

bool player_backup_data_t::restore ( C_BasePlayer* player ) const
{
    if ( !init )
        return false;

    auto renderable = player->get_client_renderable( );

    void* bonecache = * reinterpret_cast< uintptr_t** > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x290C );
    //bonecache = interfaces::mem_alloc->Alloc ( sizeof ( matrix3x4_t ) * max_bones_count );
    uint32_t& count = * reinterpret_cast< uintptr_t* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x2918 );
    memcpy ( bonecache, bone_matrix_cache, sizeof ( matrix3x4_t ) * ( max_bones_count < 256 ? max_bones_count : 256 ) );
    count = max_bones_count;
    //memcpy ( g_rbacktrack.overwrite_matrix, bone_matrix_cache, sizeof ( matrix3x4_t ) * 128 );
    player->get_collideable( )->obb_mins( ) = min;
    player->get_collideable( )->obb_maxs( ) = max;
    player->flags( ) = flags;
    //player->lower_body_yaw_target() = lower_body_yaw;
    player->vec_velocity( ) = vec_velocity;
    player->set_abs_original ( origin );
    /*
    player->has_heavy_armor() = heav_armor;
    player->armor_value() = armor;
    */

    return true;
}
