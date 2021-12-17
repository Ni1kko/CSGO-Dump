#include "fakelag.h"
#include "../../../helpers/input.hpp"
#include "../../../helpers/console.h"
#include "../../../options.h"

void c_fakelag::on_create_move ( usercmd_t* cmd, bool& send_packet )
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    const auto ticks_to_choke = get_ticks_to_choke ( cmd );

    if ( ticks_to_choke != 0 )
    {
        if ( static_cast< int > ( interfaces::client_state->chokedcommands ) <= ticks_to_choke )
            send_packet = false;
    }

    static auto last_pos = Vector ( 0, 0, 0 );

    if ( interfaces::client_state->chokedcommands == 0 )
    {
        globals.breaking_lc = ( g_local->vec_origin( ) - last_pos ).length_sqr( ) > 4096.f;
        last_pos = g_local->vec_origin( );
    }
}

int c_fakelag::get_ticks_to_choke ( usercmd_t* cmd )
{
    int ticks_to_choke;

    if ( g_inputsys.is_key_down ( g_options.ragebot_antiaim_fakeduck_key ) )
        return 15;

    if ( !g_options.ragebot_fakelag_enable )
    {
        if ( g_options.ragebot_antiaim_desync_mode != "none" )
            return 1;

        return 0;
    }

    const auto moving = g_local->vec_velocity( ).length2d( );
    const auto in_air = ! ( g_local->flags( ) & fl_onground );
    const auto slowwalking = !in_air && g_inputsys.is_key_down ( g_options.ragebot_accuracy_slowwalk_key );

    const auto state = reinterpret_cast< uintptr_t > ( g_local->get_base_player_anim_state( ) );

    if ( !state )
        return 1;

    const auto duck_amount = *reinterpret_cast< float* > ( state + 164 );

    if ( g_options.ragebot_fakelag_while_unduck && !in_air && cmd->buttons & IN_DUCK && duck_amount == 1.f )
        return 1;

    if ( slowwalking && moving )
        ticks_to_choke = g_options.ragebot_fakelag_slowwalk_ticks;
    else if ( g_local->vec_velocity( ).length2d( ) > 0.1f )
        ticks_to_choke = g_options.ragebot_fakelag_moving_ticks;
    else
        ticks_to_choke = g_options.ragebot_fakelag_standing_ticks;

    /*
    if ( slow_walking && !in_air && moving )
        ticks_to_choke = g_options.ragebot_fakelag_slowwalking;
    else if ( in_air )
        ticks_to_choke = g_options.ragebot_fakelag_in_air;
    else if ( moving )
        ticks_to_choke = g_options.ragebot_fakelag_moving;
    else
        ticks_to_choke = g_options.ragebot_fakelag_standing;
    	*/

    return ticks_to_choke;
}
