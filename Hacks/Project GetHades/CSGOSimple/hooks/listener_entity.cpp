#include "listener_entity.h"
#include "hooks.hpp"
#include "../helpers/console.h"
#include "../features/features.h"

c_listener_entity g_listener_entity;

void c_listener_entity::on_entity_created ( C_BaseEntity* ent )
{
    if ( !ent )
        return;

    const auto index = ent->ent_index( );

    if ( index < 0 )
        return;

    const auto cc = ent->get_client_class( );

    if ( !cc )
        return;

    if ( cc->class_id == CCSPlayer )
    {
#ifdef _DEBUG
        console::write_line ( "on_entity_created player" );
#endif // _DEBUG
        hook_entity ( ent );
        g_utils.on_player_create ( reinterpret_cast< C_BasePlayer* > ( ent ) );
    }
    else if ( ent->is_weapon( ) )
    {
#ifdef _DEBUG
        console::write_line ( "on_entity_created weapon" );
#endif // _DEBUG
        hook_weapon ( ent );
    }
}

void c_listener_entity::on_entity_deleted ( C_BaseEntity* ent )
{
    if ( !ent )
        return;

    auto index = ent->ent_index( );

    if ( index < 0 )
        return;

    auto it = std::find_if ( m_track.begin( ), m_track.end( ), [&] ( const container_t& data )
    {
        return data.m_index == index;
    } );

    if ( it == m_track.end( ) )
        return;

#ifdef _DEBUG
    console::write_line ( "on_entity_deleted player" );
#endif // _DEBUG

    if ( m_track[ it->m_index ].m_hooked )
        m_track[ it->m_index ].vmt.unhook_all( );

    if ( it->m_index < 64 )
    {
        g_features.resolver.rdata.at ( it->m_index ) = { };
        g_utils.on_player_destroy ( reinterpret_cast< C_BasePlayer* > ( ent ) );
    }
}

bool c_listener_entity::init ( )
{
    interfaces::entity_list->add_listener_entity ( this );

    if ( interfaces::engine_client->is_in_game( ) )
    {
        for ( auto i = 0; i < interfaces::global_vars->max_clients; i++ )
        {
            const auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

            if ( !entity )
                continue;

#ifdef _DEBUG
            console::write_line ( "init hook player" );
#endif // _DEBUG
            hook_entity ( entity );
        }
    }

    return true;
}

void c_listener_entity::remove ( )
{
    interfaces::entity_list->remove_listener_entity ( this );

    for ( auto i = 0; i < 64; i++ )
    {
        if ( m_track[ i ].m_hooked )
            m_track[ i ].vmt.unhook_all( );
    }
}

void c_listener_entity::hook_entity ( C_BaseEntity* ent )
{
    auto i = ent->ent_index( );
    m_track[ i ].m_index = i;
    m_track[ i ].vmt.setup ( ent );
    m_track[ i ].vmt.hook_index ( index::do_extra_bones_processing, hooks::hk_do_extra_bones_processing );
    m_track[ i ].vmt.hook_index ( index::update_client_side_animation, hooks::hk_update_client_side_animation );
    //m_track[ i ].vmt.hook_index ( index::post_data_update, hooks::hk_post_data_update );
    m_track[ i ].vmt.hook_index ( index::accumulate_layers, hooks::hk_accumulate_layers );
    //m_track[i].vmt.hook_index ( index::get_abs_origin, hooks::hk_get_abs_origin );
    //m_track[i].vmt.hook_index ( index::calc_view, hooks::hk_calc_view );
    m_track[ i ].m_hooked = true;
}

void c_listener_entity::hook_weapon ( C_BaseEntity* ent )
{
    //auto i = ent->ent_index ( );
    //m_track[ i ].m_index = i;
    //m_track[ i ].vmt.setup ( ent );
    //m_track[ i ].vmt.hook_index ( index::update_client_side_animation, hooks::hk_update_client_side_animation );
    //m_track[ i ].m_hooked = true;
}
