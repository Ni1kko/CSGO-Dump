#include "new_ragebot.h"
#include "../../../helpers/math.hpp"
#include "../../../options.h"
#include "../../features.h"

new_ragebot::can_hit_t::can_hit_t ( const float damage, const vector_t position ): damage ( damage ), position ( position )
{
}

void new_ragebot::run ( ) noexcept
{
    if ( !ctx::client.local || !ctx::client.cmd )
        return;

    if ( m_next_shot_time >= interfaces::global_vars->curtime )
        return; // we can't shoot bc the weapon is still in its shooting cycle -> return here

    run_hitscan( );

    compensate_recoil( );

    // m_last_player_index todo add check for entity index change
}

void new_ragebot::finish_run ( ) noexcept
{
    if ( m_next_shot_time >= interfaces::global_vars->curtime )
        return;

    if ( !ctx::client.local )
        return;

    auto weapon = ctx::client.local->active_weapon( ).get( );

    if ( !weapon )
        return;

    auto weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return;

    if ( ctx::client.cmd->buttons & IN_ATTACK )
        m_next_shot_time = interfaces::global_vars->curtime + *reinterpret_cast< float* > ( uintptr_t ( weapon_data ) + 0xDC ); // flCycleTime
    else if ( ctx::client.cmd->buttons & IN_ATTACK2 )
        m_next_shot_time = interfaces::global_vars->curtime + *reinterpret_cast< float* > ( uintptr_t ( weapon_data ) + 0xE0 ); // flCycleTimeAlt
}

void new_ragebot::reset_ragebot ( ) noexcept
{
    m_next_shot_time = 0.f;
}

void new_ragebot::compensate_recoil ( ) noexcept
{
    ctx::client.cmd->viewangles -= ctx::client.local->aim_punch_angle( ) * 2.f;
}

void new_ragebot::build_hitbox_list ( )
{
    if ( g_options.ragebot_hitbox_head )
        m_hitboxes.push_back ( hitbox_head );

    if ( g_options.ragebot_hitbox_neck )
        m_hitboxes.push_back ( hitbox_neck );

    if ( g_options.ragebot_hitbox_stomach )
        m_hitboxes.push_back ( hitbox_stomach );

    if ( g_options.ragebot_hitbox_chest )
    {
        m_hitboxes.push_back ( hitbox_chest );
        m_hitboxes.push_back ( hitbox_lower_chest );
        m_hitboxes.push_back ( hitbox_upper_chest );
    }

    if ( g_options.ragebot_hitbox_feet )
    {
        m_hitboxes.push_back ( hitbox_right_thigh );
        m_hitboxes.push_back ( hitbox_left_thigh );
        m_hitboxes.push_back ( hitbox_right_foot );
        m_hitboxes.push_back ( hitbox_left_foot );
    }

    if ( g_options.ragebot_hitbox_arms )
    {
        m_hitboxes.push_back ( hitbox_right_hand );
        m_hitboxes.push_back ( hitbox_left_hand );
        m_hitboxes.push_back ( hitbox_right_upper_arm );
        m_hitboxes.push_back ( hitbox_left_upper_arm );
    }
}

void new_ragebot::run_hitscan ( )
{
    switch ( hitscan_t::damage )
    {
    case hitscan_t::damage:
        run_damage_hitscan( );
        break;

    case hitscan_t::automatic:
        run_automatic_hitscan( );
        break;

    case hitscan_t::distance:
        run_distance_hitscan( );
        break;

    default:
        throw std::exception ( "invalid hitscan type" );
    }
}

void new_ragebot::run_damage_hitscan ( )
{
    auto best_damage = 0.f;
    auto best_position = vector_t( );
    C_BasePlayer* best_player = nullptr;

    g_utils.loop_through_players ( [&] ( C_BasePlayer* player ) -> void
    {
        if ( !prepare_player ( player ) )
            return;

        auto damage = 0.f;
        auto position = vector_t( );

        if ( !get_best_hitpoint ( damage, position ) )
            return;

        if ( damage > best_damage )
        {
            best_damage = damage;
            best_position = position;
            best_player = player;
        }
    }, c_utils::player_loop_flag_enemy_only );

    if ( !best_player )
        return;

    ctx::client.cmd->viewangles = g_math.calc_angle ( g_local->get_eye_pos ( g_features.animations.m_real_state ), best_position );
}

void new_ragebot::run_distance_hitscan ( )
{
#if _DEBUG
    throw std::exception ( "not implemented" );
#else
    throw std::exception();
#endif
}

void new_ragebot::run_automatic_hitscan ( )
{
#if _DEBUG
    throw std::exception ( "not implemented" );
#else
    throw std::exception();
#endif
}

bool new_ragebot::prepare_player ( C_BasePlayer* player )
{
    const auto model = player->get_model( );

    if ( !model )
        return false;

    m_tmp_studio_hdr = interfaces::mdl_info->get_studiomodel ( model );

    if ( !m_tmp_studio_hdr )
        return false;

    if ( !player->setup_bones ( m_tmp_bone_matrix.data( ), 128, bone_used_by_anything, player->simulation_time( ) ) )
        return false;

    m_tmp_studio_hitboxset = m_tmp_studio_hdr->get_hitbox_set ( 0 );

    return m_tmp_studio_hitboxset != nullptr;
}

bool new_ragebot::get_best_hitpoint ( float& damage_out, vector_t& point_out )
// bug set mindmg to health when health lower than mindmg does not work bc not entity is passed as argument
{
    damage_out = -1.f;

    for ( const auto hitbox : m_hitboxes ) // todo check for should baim, add another argument to set if can baim when we should baim
    {
        auto hitpoints = get_hitbox_points ( hitbox );

        if ( hitpoints.empty( ) )
            continue;

        auto hitbox_can_hit_center = can_hit_t( );
        auto hitbox_best_damage = -1.f;
        auto hitbox_best_position = vector_t( );

        for ( const auto hitpoint : hitpoints )
        {
            const auto damage = g_features.autowall.damage ( hitpoint.position );

            if ( hitpoint.center )
                hitbox_can_hit_center = can_hit_t ( damage, hitpoint.position );

            if ( damage > g_options.ragebot_min_damage && damage > hitbox_best_damage )
            {
                hitbox_best_damage = damage;
                hitbox_best_position = hitpoint.position;
            }
        }

        if ( hitbox_can_hit_center.damage != -1.f && hitbox_can_hit_center.damage > ( hitbox_best_damage / 2.f /* this value can be adjusted later */ ) )
        {
            hitbox_best_damage = hitbox_can_hit_center.damage;
            hitbox_best_position = hitbox_can_hit_center.position;
        }

        if ( hitbox_best_damage > damage_out /* used as best_damage here */ )
        {
            damage_out = hitbox_best_damage;
            point_out = hitbox_best_position;
        }
    }

    return damage_out != -1.f;
}

std::vector< hitpoint_t > new_ragebot::get_hitbox_points ( const int hitbox ) noexcept
{
    std::vector< hitpoint_t > return_val;

    if ( hitbox >= hitbox_max )
        return return_val;

    const auto studio_box = m_tmp_studio_hitboxset->get_hitbox ( hitbox );

    if ( !studio_box )
        return return_val;

    Vector min, max;

    g_math.vector_transform ( studio_box->bbmin, m_tmp_bone_matrix[ studio_box->bone ], min );
    g_math.vector_transform ( studio_box->bbmax, m_tmp_bone_matrix[ studio_box->bone ], max );

    const auto center_hitbox = ( min + max ) * 0.5f;

    return_val.emplace_back ( hitpoint_t ( center_hitbox, true ) );

    const auto get_multipoint_position = [&] ( const vector_t& multiplier ) -> vector_t
    {
        return center_hitbox * multiplier;
    };

    const auto cur_angles = g_math.calc_angle ( center_hitbox, g_local->get_eye_pos ( g_features.animations.m_real_state ) );

    vector_t forward;
    g_math.angle_vectors ( cur_angles, forward );

    const auto right = forward.cross ( vector_t ( 0, 0, 1 ) );
    const auto left = vector_t ( -right.x, -right.y, right.z );

    const auto top = vector_t ( 0, 0, 1 );
    const auto bot = vector_t ( 0, 0, -1 );

    if ( hitbox == hitbox_head )
    {
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( right ) ) );
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( left ) ) );
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( top ) ) );
    }
    else
    {
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( right ) ) );
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( left ) ) );
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( top ) ) );
        return_val.emplace_back ( hitpoint_t ( get_multipoint_position ( bot ) ) );
    }

    return return_val;
}
