#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

#include "../helpers/console.h"
#include "../features/features.h"

c_event_handler event_handler;

void hooks::fire_event_handler ( IGameEvent* event )
{
    if ( !event )
        return;

    g_features.event_logger.on_fire_event ( event );
}

bool __stdcall hooks::hk_fire_event ( IGameEvent* pEvent )
{
    static auto ofunc = gameevents_hook.get_original< fire_event > ( index::fire_event );

    if ( g_unload )
        return ofunc ( interfaces::game_events, pEvent );

    fire_event_handler ( pEvent );

    return ofunc ( interfaces::game_events, pEvent );
}

void hooks::setup_fire_events ( )
{
    event_handler.start( );
}

void c_event_handler::start ( )
{
    m_listener.start( );
}

void c_event_handler::stop ( )
{
    m_listener.stop( );
}

void c_event_handler::listener::start ( )
{
    if ( !interfaces::game_events->add_listener ( this, xor_str ( "player_hurt" ), false ) )
        throw std::exception ( xor_str ( "Failed to register the event" ) );

    if ( !interfaces::game_events->add_listener ( this, xor_str ( "bullet_impact" ), false ) )
        throw std::exception ( xor_str ( "Failed to register the event" ) );

    if ( !interfaces::game_events->add_listener ( this, xor_str ( "item_purchase" ), false ) )
        throw std::exception ( xor_str ( "Failed to register the event" ) );

    if ( !interfaces::game_events->add_listener ( this, xor_str ( "player_footstep" ), false ) )
        throw std::exception ( xor_str ( "Failed to register the event" ) );

    if ( !interfaces::game_events->add_listener ( this, xor_str ( "player_jump" ), false ) )
        throw std::exception ( xor_str ( "Failed to register the event" ) );
}

void c_event_handler::listener::stop ( )
{
    interfaces::game_events->remove_listener ( this );
}

void c_event_handler::listener::FireGameEvent ( IGameEvent* event )
{
    hooks::fire_event_handler ( event );
}

int c_event_handler::listener::GetEventDebugID ( void )
{
    return EVENT_DEBUG_ID_INIT;
}
