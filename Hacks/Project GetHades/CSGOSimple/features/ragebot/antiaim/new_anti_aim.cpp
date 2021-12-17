#include "new_anti_aim.h"
#include "../../../options.h"
#include "../../../helpers/math.hpp"

void new_anti_aim::on_create_move ( )
{
    if ( !ctx::client.local || !ctx::client.local->is_alive( ) )
        return;

    // run lby prediction first so we can use values from it in the next functions
    run_lby_predication( );
}

void new_anti_aim::run_pitch_antiaim ( )
{
    switch ( pitch_antiaim_t t = pitch_antiaim_t::down )
    {
    case pitch_antiaim_t::up:
        pitch_up( );
        break;

    case pitch_antiaim_t::zero:
        pitch_zero( );
        break;

    case pitch_antiaim_t::down:
        pitch_down( );
        break;

    default:
        break;
    }
}

void new_anti_aim::run_yaw_antiaim ( )
{
    switch ( yaw_antiaim_t t = yaw_antiaim_t::at_target )
    {
    case yaw_antiaim_t::at_target:
        yaw_at_target( );
        break;

    case yaw_antiaim_t::spinbot:
        yaw_spinbot( );
        break;

    case yaw_antiaim_t::random:
        yaw_random( );
        break;

    case yaw_antiaim_t::freestand:
        yaw_freestand( );
        break;

    default:
        break;
    }
}

void new_anti_aim::run_desync_antiaim ( )
{
    switch ( desync_antiaim_t t = desync_antiaim_t::none )
    {
    case desync_antiaim_t::directional_static:
        desync_static( );
        break;

    case desync_antiaim_t::static_fake:
        desync_switch_real( );
        break;

    case desync_antiaim_t::jitter:
        desync_jitter( );
        break;

    default:
        break;
    }
}

void new_anti_aim::pitch_down ( )
{
    ctx::client.cmd->viewangles.pitch = 89.f;
}

void new_anti_aim::pitch_up ( )
{
    ctx::client.cmd->viewangles.pitch = -89.f;
}

void new_anti_aim::pitch_zero ( )
{
    ctx::client.cmd->viewangles.pitch = 0.f;
}

void new_anti_aim::yaw_at_target ( )
{
    //
}

void new_anti_aim::yaw_spinbot ( )
{
    //
}

void new_anti_aim::yaw_random ( )
{
    //
}

void new_anti_aim::yaw_freestand ( )
{
    //
}

void new_anti_aim::desync_static ( )
{
}

void new_anti_aim::desync_switch_real ( )
{
}

void new_anti_aim::desync_jitter ( )
{
}

direction_t new_anti_aim::get_direction_for_desync ( )
{
    return direction_t::mid;
}

C_BasePlayer* new_anti_aim::get_low_fov_player ( )
{
    C_BasePlayer* best_player = nullptr;
    auto best_fov = 180.f;

    const auto local_pos = ctx::client.local->vec_origin( );
    qangle_t local_viewangles;
    interfaces::engine_client->get_viewangles ( &local_viewangles );

    g_utils.loop_through_players ( [&] ( C_BasePlayer* player )
    {
        const auto fov = g_math.get_fov ( local_viewangles, g_math.calc_angle ( local_pos, player->vec_origin( ) ) );

        if ( fov < best_fov )
        {
            best_fov = fov;
            best_player = player;
        }
    }, c_utils::player_loop_flags_t::player_loop_flag_enemy_only );

    return best_player;
}

void new_anti_aim::run_lby_predication ( )
{
    static auto spawntime = -1.f;
    auto state = reinterpret_cast< CCSGOPlayerAnimState* > ( ctx::client.local->get_base_player_anim_state( ) );

    if ( !state )
        return;

    if ( spawntime != ctx::client.local->spawn_time( ) )
    {
        spawntime = ctx::client.local->spawn_time( );
        m_next_lby_update = 0.f;
    }

    if ( state->get_vec_velocity( ).length2d( ) > 0.1f )
        m_next_lby_update = globals.curtime + 0.22f;
    else if ( m_next_lby_update == -1 || globals.curtime >= m_next_lby_update )
        m_next_lby_update = globals.curtime + 1.1f;
}

bool new_anti_aim::should_skip_antiaim ( )
{
    const auto weapon = ctx::client.local->active_weapon( ).get( );

    if ( !weapon )
        return false;

    if ( weapon->is_grenade( ) )
    {
        if ( weapon->throw_time( ) > 0.1f )
            return true;
    }
    else
    {
        if ( ( weapon->can_fire( ) || weapon->is_knife( ) ) && ctx::client.cmd->buttons & IN_ATTACK )
            return true;

        if ( weapon->item( ).item_definition_index( ) != WEAPON_C4 && ctx::client.cmd->buttons & IN_USE )
            return true;

        if ( weapon->item( ).item_definition_index( ) != WEAPON_C4 && weapon->is_rifle( ) && !weapon->can_fire( ) )
            ctx::client.cmd->buttons &= ~IN_ATTACK;

        if ( weapon->is_knife( ) && g_options.ragebot_antiaim_disable_on_knife )
            return true;
    }

    return false;
}
