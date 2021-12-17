#include "resolver.h"
#include "../../../helpers/console.h"

#include "../../../helpers/debug_console.h"

#include "../../../helpers/math.hpp"

#include "../../../options.h"

#include "../../features.h"

// todo hooks setup_velocity and replace goal_feet_yaw at the end  ||  find a way to overwrite goalFeetYaw
// todo add anti-freestand

void c_resolver::on_create_move ( )
{
    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        const auto player = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !player || !player->is_alive( ) || player->is_dormant( ) )
            continue;

        if ( !rdata.at ( i ).setup_player ( player ) )
            continue;

        if ( this->rdata.at ( i ).last_update_simtime == player->simulation_time( ) )
            continue;

        this->rdata.at ( i ).last_update_simtime = player->simulation_time( );

        this->rdata.at ( i ).last_abs_yaw = abs ( server_goal_feet_yaw ( player ) );

        this->rdata.at ( i ).is_resolving = false;
        this->rdata.at ( i ).last_angle = player->eye_angles( );

        resolve_yaw ( player );
    }
}

void c_resolver::on_frame_stage_notify ( )
{
    if ( !g_options.ragebot_resolver )
        return;

    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        const auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) )
            continue;

        //if ( this->rdata.at ( i ).last_update_simtime == entity->simulation_time(  ) || interfaces::global_vars->tickcount == this->rdata.at ( i ).last_tickcount )
        //    continue;

        //this->rdata.at ( i ).last_tickcount = interfaces::global_vars->tickcount;
        //this->rdata.at ( i ).last_update_simtime = entity->simulation_time(  );

        //this->rdata.at ( i ).is_resolving = false;
        //this->rdata.at ( i ).last_angle = entity->eye_angles(  );

        //resolve_yaw ( entity );

        if ( this->rdata.at ( i ).should_resolve )
        {
            auto state = entity->get_base_player_anim_state( );

            auto quick_normalize = [] ( float& yaw )
            {
                for ( ; yaw > 180.f; yaw = yaw - 360.f );

                for ( ; yaw < -180.f; yaw = yaw + 360.f );
            };

            if ( state )
            {
                //entity->eye_angles( ).yaw = this->rdata.at ( i ).last_abs_yaw;
                //state->m_flYaw = entity->eye_angles( ).yaw + this->rdata.at ( i ).desync_add;
                //state->m_flGoalFeetYaw = this->rdata.at ( i ).last_abs_yaw + this->rdata.at ( i ).desync_add;
                //state->m_flGoalFeetYaw = server_goal_feet_yaw ( entity );
                //quick_normalize ( state->m_flGoalFeetYaw );
                auto normalized_add = state->m_flGoalFeetYaw + this->rdata.at ( i ).desync_add;
                quick_normalize( normalized_add );
                entity->set_abs_angles ( qangle_t ( 0.f, normalized_add, 0.f ) );
                this->rdata.at ( i ).resolver_abs_yaw = normalized_add;
                //entity->eye_angles().yaw += this->rdata.at ( i ).desync_add;
            }

            quick_normalize ( entity->eye_angles( ).yaw );
        }

        //this->rdata.at ( i ).last_resolved_angle = entity->eye_angles(  );
    }
}

bool c_resolver::get_is_desyncing ( const int idx )
{
    return this->rdata.at ( idx ).desyncing;
}

bool c_resolver::get_is_tick_shifting ( const int idx )
{
    return this->rdata.at ( idx ).tick_shifting;
}

void c_resolver::reset_resolver_data ( )
{
    this->rdata = { };
}

void c_resolver::reset_player_resolver_data ( const int idx )
{
    this->rdata.at ( idx ) = { };
}

void c_resolver::count_player_shot ( const int idx, const c_resolver::resolver_data_t rdata )
{
    this->rdata.at ( idx ).count_shot ( rdata );
}

void c_resolver::remove_player_shot ( const int idx, const c_resolver::resolver_data_t rdata )
{
    this->rdata.at ( idx ).remove_shot ( rdata );
}

float c_resolver::server_goal_feet_yaw ( C_BasePlayer* entity )
{
    auto animstate = entity->get_base_player_anim_state( );

    if ( !animstate )
        return 0.f;

    /* data */
    auto ground_fraction = * ( float* )( animstate + 0x11C );
    auto duck_ammount = * ( float* )( animstate + 0xA4 );
    auto ducking_speed = std::max ( 0.f, std::min ( 1.f, *reinterpret_cast< float* > ( animstate + 0xFC ) ) );
    auto running_speed = std::max ( 0.f, std::min ( *reinterpret_cast< float* > ( animstate + 0xF8 ), 1.f ) );
    /* offsets */
    auto backup_eflags = entity->eflag( );

    entity->eflag( ) = ( 1 << 12 );
    auto abs_velocity = *reinterpret_cast< vector_t* > ( uintptr_t ( entity ) + 0x94 );
    entity->eflag( ) = backup_eflags;

    auto speed = std::fmin ( abs_velocity.length( ), 260.0f );

    auto goal_feet_yaw = animstate->m_flGoalFeetYaw;

    auto angle_diff = [] ( float destAngle, float srcAngle ) -> float
    {
        auto delta = 0.f;

        delta = fmodf ( destAngle - srcAngle, 360.0f );

        if ( destAngle > srcAngle )
        {
            if ( delta >= 180 )
                delta -= 360;
        }
        else
        {
            if ( delta <= -180 )
                delta += 360;
        }

        return delta;
    };

    auto eye_feet_delta = angle_diff ( animstate->m_flYaw, goal_feet_yaw );

    auto flYawModifier = ( ( ( ground_fraction * -0.3f ) - 0.2f ) * running_speed ) + 1.0f;

    if ( duck_ammount > 0.0f )
        flYawModifier = flYawModifier + ( ( duck_ammount * ducking_speed ) * ( 0.5f - flYawModifier ) );

    auto flMaxYawModifier = flYawModifier * 58.f;
    auto flMinYawModifier = flYawModifier * -58.f;

    if ( eye_feet_delta <= flMaxYawModifier )
    {
        if ( flMinYawModifier > eye_feet_delta )
            goal_feet_yaw = fabs ( flMinYawModifier ) + animstate->m_flYaw;
    }
    else
        goal_feet_yaw = animstate->m_flYaw - fabs ( flMaxYawModifier );

    if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
        return 0.f;

    g_math.normalize_yaw ( goal_feet_yaw );

    if ( speed > 0.1f || fabs ( abs_velocity.z ) > 100.0f )
    {
        goal_feet_yaw = g_math.fl_approach_angle (
            animstate->m_flYaw,
            goal_feet_yaw,
            ( ( ground_fraction * 20.0f ) + 30.0f )
            * animstate->m_flLastClientSideAnimationUpdateTime );
    }
    else
    {
        goal_feet_yaw = g_math.fl_approach_angle (
            entity->lower_body_yaw_target( ),
            goal_feet_yaw,
            animstate->m_flLastClientSideAnimationUpdateTime * 100.0f );
    }

    if ( goal_feet_yaw > 5000 || goal_feet_yaw < -5000 )
        return 0.f;

    g_math.normalize_yaw ( goal_feet_yaw );

    return goal_feet_yaw;
}

c_resolver::resolver_angle_overwrites_t c_resolver::resolver_data_t::get_brutforce_overwrite ( )
{
    auto lowest_misses = std::numeric_limits< int >::max( );
    auto best_mode = resolver_angle_overwrites_t::plus;

    for ( size_t i = 0; i < static_cast< size_t > ( resolver_angle_overwrites_t::max ); i++ )
    {
        // ReSharper disable once CppIncompleteSwitchStatement
        switch ( static_cast< resolver_angle_overwrites_t > ( i ) )
        {
        case resolver_angle_overwrites_t::max:
            continue;
        }

        if ( this->overall_overwrites_shots[ i ] < lowest_misses )
        {
            lowest_misses = this->overall_overwrites_shots[ i ];
            best_mode = static_cast< resolver_angle_overwrites_t > ( i );
        }
    }

    return best_mode;
}

c_resolver::resolver_modes_t c_resolver::resolver_data_t::get_best_resolver_mode ( ) const
{
    //constexpr auto damage_percentage = 1.f / 3.f;

    //if(this.mod)

    switch ( this->shots % 2 )
    {
    case 0:
        return resolver_modes_t::damage;
        break;

    case 1:
        return resolver_modes_t::brutforce;
        break;
    }

    return resolver_modes_t::damage;
    //if ( this->shots <= 0 )
    //    return resolver_modes_t::damage;

    //if ( this->shots % 3 )
    //    return resolver_modes_t::damage;

    //return resolver_modes_t::brutforce;
}

bool c_resolver::resolver_data_t::is_playing_shot_anim ( )
{
    const auto weapon_anim_layer = 1;
    const auto shooting_anim_layer = player->get_anim_overlay ( weapon_anim_layer );

    const auto current_activity = player->get_sequence_activity ( shooting_anim_layer->sequence );

    switch ( current_activity )
    {
    case 961:
    case 964:
        return shooting_anim_layer->weight < 0.5f && shooting_anim_layer->cycle <= 0.04f;

    default:
        break;
    }

    return false;
}

bool c_resolver::resolver_data_t::is_979_playing ( )
{
    const auto l = player->get_anim_overlay ( 3 );

    if ( ( l == nullptr ) || l->cycle == 0.f || l->weight == 0.f )
        return false;

    const auto activity = player->get_sequence_activity ( l->sequence );

    if ( l->cycle != this->previous_layer.cycle || l->weight == 1.f )
    {
        if ( ( activity == 979 && l->cycle < 0.01f ) || ( l->weight == 0.f && ( this->previous_layer.cycle > 0.92f && l->cycle > 0.92f ) ) )
        {
            this->previous_layer = *l;
            return true;
        }
    }

    return false;
}

bool c_resolver::resolver_data_t::is_desyncing ( )
{
    if ( this->in_shot )
        return false;

    if ( this->player->vec_velocity( ).length2d( ) > 0.1f )
    {
        if ( detection_last_simtime == player->simulation_time( ) || detection_last_simtime == detection_pre_last_simtime )
        {
            detection_pre_last_simtime = detection_last_simtime;
            detection_last_simtime = player->simulation_time( );
            return true;
        }

        detection_pre_last_simtime = detection_last_simtime;
        detection_last_simtime = player->simulation_time( );
        return false;
    }

    detection_pre_last_simtime = detection_last_simtime;
    detection_last_simtime = player->simulation_time( );

    if ( this->player->simulation_time( ) - this->times.last_979 <= 1.4f )
        return true;

    if ( this->player->simulation_time( ) - this->times.last_choke <= 1.4f )
        return true;

    return false;
}

void c_resolver::resolve_yaw ( C_BasePlayer* player )
{
    const auto index = player->ent_index( );

    this->rdata.at ( index ).should_resolve = false;

    if ( this->rdata.at ( index ).desyncing )
    {
        this->rdata.at ( player->ent_index( ) ).is_resolving = true;

        //player->eye_angles( ).yaw = get_resolved_yaw_by_anti_freestand ( player );
        //return;

        const auto resolver_mode = this->rdata.at ( index ).get_best_resolver_mode( );

        this->rdata.at ( index ).should_resolve = true;

        //const auto distance = player->vec_origin().dot ( g_local->vec_origin() );
        //auto direction = 2 * ( distance <= 0.0f ) - 1;

        //this->rdata.at ( index ).desync_add = 60.f * direction;

        //auto tmp = get_resolved_yaw_by_brutforce ( player );
        //this->rdata.at ( index ).desync_add = tmp;

        switch ( resolver_modes_t::damage )
        {
        case resolver_modes_t::damage:
            {
                auto tmp = get_resolved_yaw_by_damage ( player );

                if ( tmp == 0.f )
                    tmp = get_resolved_yaw_by_brutforce ( player );

                //auto tmp = -60.f;

                //auto calc_ang = g_math.calc_angle ( g_local->vec_origin( ), player->vec_origin( ) ).yaw;

                //if ( player->eye_angles( ).yaw < calc_ang )
                //    tmp = 60.f;

                this->rdata.at ( index ).desync_add = tmp;
            }
            break;

        case resolver_modes_t::brutforce:
            {
                auto tmp = get_resolved_yaw_by_brutforce ( player );
                this->rdata.at ( index ).desync_add = tmp;
            }
            break;
        }

        //    tmp = get_resolved_yaw_by_brutforce ( player );

        //this->rdata.at ( index ).desync_add = tmp;

        //if ( this->rdata.at ( index ).last_hit_head.did_hit )
        //{
        //    switch ( this->rdata.at ( index ).last_hit_head.addang )
        //    {
        //    case resolver_angle_overwrites_t::none:
        //        this->rdata.at ( index ).desync_add = 0.f;
        //        break;

        //    case resolver_angle_overwrites_t::plus:
        //        this->rdata.at ( index ).desync_add = 60.f;
        //        break;

        //    case resolver_angle_overwrites_t::minus:
        //        this->rdata.at ( index ).desync_add = -60.f;
        //        break;

        //    default:
        //        ;
        //    }
        //}
        //else
        //{
        //    //auto tmp = get_resolved_yaw_by_damage ( player );

        //    //if ( tmp == 0.f )
        //    //    tmp = get_resolved_yaw_by_brutforce ( player );

        //    //this->rdata.at ( index ).desync_add = tmp;
        //    switch ( resolver_mode )
        //    {
        //    case resolver_modes_t::damage:
        //        {
        //            auto tmp = get_resolved_yaw_by_damage ( player );

        //            if ( tmp == 0.f )
        //                tmp = get_resolved_yaw_by_brutforce ( player );

        //            this->rdata.at ( index ).desync_add = tmp;
        //        }
        //        break;

        //    case resolver_modes_t::brutforce:
        //        this->rdata.at ( index ).desync_add = get_resolved_yaw_by_brutforce ( player );
        //        break;

        //    default:
        //        ;
        //    }
        //}

        //auto state = player->get_base_player_anim_state(  );

        //if ( state && this->rdata.at ( index ).should_resolve )
        //{
        //    state->m_goal_feet_yaw += g_features.resolver.rdata.at ( index ).desync_add;

        //    for ( ; state->m_goal_feet_yaw > 180.f ; state->m_goal_feet_yaw = state->m_goal_feet_yaw - 360.f );

        //    for ( ; state->m_goal_feet_yaw < -180.f ; state->m_goal_feet_yaw = state->m_goal_feet_yaw + 360.f );
        //}

        //player->eye_angles(  ).yaw += this->rdata.at ( index ).desync_add;

        //this->rdata.at ( index ).last_yaw = player->eye_angles(  ).yaw;
    }
}

void c_resolver::resolve_pitch ( C_BasePlayer* player )
{
    //
}

float c_resolver::get_resolved_yaw_by_damage ( C_BasePlayer* player )
{
    if ( player->simulation_time( ) == this->rdata.at ( player->ent_index( ) ).last_damage_data.simtime )
    {
        switch ( this->rdata.at ( player->ent_index( ) ).last_damage_data.overwrite )
        {
        case resolver_angle_overwrites_t::none:
            return 0.f;

        case resolver_angle_overwrites_t::plus:
            return 60.f;

        case resolver_angle_overwrites_t::minus:
            return -60.f;
        }
    }

    constexpr auto radius = 45.f;
    const auto eye_pos = player->get_eye_pos( );

    const auto get_head_pos = [&] ( float add_angle ) -> vector_t
    {
        add_angle += player->eye_angles( ).yaw;
        return vector_t ( radius * cos ( deg2rad ( add_angle ) ) + eye_pos.x, radius * sin ( deg2rad ( add_angle ) ) + eye_pos.y, eye_pos.z + 4.f );
    };

    auto local_eye_pos = g_local->get_eye_pos( );

    auto tmp = get_head_pos ( 60.f );
    const auto t1 = g_features.autowall.get_thickness ( local_eye_pos, tmp, -1.f );
    // const auto hit1 = g_features.autowall.can_hit_point ( tmp, g_local->get_eye_pos(  ) );
    tmp = get_head_pos ( -60.f );
    const auto t2 = g_features.autowall.get_thickness ( local_eye_pos, tmp, -1.f );
    //const auto hit2 = g_features.autowall.can_hit_point ( tmp, g_local->get_eye_pos(  ) );

    auto overwrite_to_angle = [&] ( resolver_angle_overwrites_t mode ) -> float
    {
        switch ( mode )
        {
        case resolver_angle_overwrites_t::none:
            return 0.f;

        case resolver_angle_overwrites_t::plus:
            return 60.f;

        case resolver_angle_overwrites_t::minus:
            return -60.f;

        case resolver_angle_overwrites_t::max:
            return 0.f;

        default:
            return 0.f;
        }
    };

    auto is_valid_direction = [] ( resolver_angle_overwrites_t mode ) -> bool
    {
        switch ( mode )
        {
        case resolver_angle_overwrites_t::none:
            return false;

        case resolver_angle_overwrites_t::plus:
            return true;

        case resolver_angle_overwrites_t::minus:
            return true;

        case resolver_angle_overwrites_t::max:
            return false;

        default:
            return false;
        }
    };

    this->rdata.at ( player->ent_index( ) ).last_damage_data.simtime = player->simulation_time( );

    //if ( !hit1 && !hit2 && is_valid_direction ( this->rdata.at ( player->ent_index( ) ).last_can_hit_head_points.last_direction ) && player->simulation_time( )
    //        - this->rdata.at ( player->ent_index( ) ).last_can_hit_head_points.last_simtime < 5.f )
    //    return overwrite_to_angle ( this->rdata.at ( player->ent_index( ) ).last_can_hit_head_points.last_direction );

    //if ( ( !hit1 && hit2 ) || ( !hit2 && hit1 ) )
    //{
    //    if ( !hit1 && hit2 )
    //    {
    //        this->rdata.at ( player->ent_index(  ) ).last_can_hit_head_points.last_simtime = player->simulation_time(  );
    //        this->rdata.at ( player->ent_index(  ) ).last_can_hit_head_points.last_direction = resolver_angle_overwrites_t::plus;
    //        return max_rotation;
    //    }

    //    if ( !hit2 && hit1 )
    //    {
    //        this->rdata.at ( player->ent_index(  ) ).last_can_hit_head_points.last_simtime = player->simulation_time(  );
    //        this->rdata.at ( player->ent_index(  ) ).last_can_hit_head_points.last_direction = resolver_angle_overwrites_t::minus;
    //        return -max_rotation;
    //    }
    //}

    if ( t1 == t2 )
    {
        this->rdata.at ( player->ent_index( ) ).last_damage_data.overwrite = resolver_angle_overwrites_t::none;
        return 0.f;
    }

    if ( t1 > t2 )
    {
        this->rdata.at ( player->ent_index( ) ).last_damage_data.overwrite = resolver_angle_overwrites_t::plus;
        return 60.f;
    }

    this->rdata.at ( player->ent_index( ) ).last_damage_data.overwrite = resolver_angle_overwrites_t::minus;
    return -60.f;
}

float c_resolver::get_resolved_yaw_by_brutforce ( C_BasePlayer* player ) const
{
    switch ( this->rdata.at ( player->ent_index( ) ).shots % 2 )
    {
    case 0:
        return 60.f;

    case 1:
        return -60.f;

    default:
        return 0.f;
    }
}

float c_resolver::get_resolved_yaw_by_anti_freestand ( C_BasePlayer* player )
{
    auto local_eye_position = ctx::client.local->get_eye_pos ( g_features.animations.m_real_state );
    auto enemy_eye_position = player->get_eye_pos( );

    const auto backwards = g_math.calc_angle ( ctx::client.local->vec_origin( ), player->vec_origin( ) ).yaw - 180.f;

    const auto get_thickness = [&] ( const float end_rot, const float used_radius = 60.f ) -> float
    {
        auto rotation_pos = vector_t ( used_radius * cos ( deg2rad ( end_rot ) ) + enemy_eye_position.x,
                                       used_radius * sin ( deg2rad ( end_rot ) ) + enemy_eye_position.y,
                                       enemy_eye_position.z + 4.f );

        return g_features.autowall.get_thickness ( local_eye_position, rotation_pos, 2048.f );
    };

    const auto minus = get_thickness ( backwards - 90.f );
    const auto plus = get_thickness ( backwards + 90.f );

    if ( minus == plus )
        return player->eye_angles( ).yaw;

    if ( minus > plus )
        return backwards - 90.f;

    return backwards + 90.f;
}

bool resolverdata_t::update ( C_BasePlayer* player )
{
    if ( !player || player->is_dormant( ) || !player->is_alive( ) )
        return false;

    if ( !m_init )
    {
        m_last_simtime = player->simulation_time( );
        m_init = true;
    }

    return true;
}
