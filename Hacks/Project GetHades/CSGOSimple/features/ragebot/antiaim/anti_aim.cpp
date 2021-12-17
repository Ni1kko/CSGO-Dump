#include "anti_aim.h"
#include "../../../helpers/console.h"

#include <utility>

#include "../../../helpers/input.hpp"

#include "../../../helpers/math.hpp"

#include "../../../options.h"
#include "../../features.h"

void c_anti_aim::setup ( )
{
    auto get_lowest_fov_player = [] ( ) -> C_BasePlayer*
    {
        const auto eye_pos = g_local->get_eye_pos( );
        QAngle engine_viewangles;
        interfaces::engine_client->get_viewangles ( &engine_viewangles );

        auto best_fov = FLT_MAX;
        C_BasePlayer* best_player = nullptr;

        for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
        {
            auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

            if ( !entity || !g_local || !entity->is_player( ) || entity == g_local || entity->is_dormant( )
                || !entity->is_alive( ) || !entity->is_enemy( ) )
                continue;

            const auto tmp = g_math.calc_angle ( eye_pos, entity->get_eye_pos( ) );
            const auto fov = g_math.get_fov ( engine_viewangles, tmp );

            if ( !entity->active_weapon( ).get( ) )
                continue;

            if ( fov < best_fov )
            {
                best_player = entity;
                best_fov = fov;
            }
        }

        return best_player;
    };

    this->register_real_base_antiaim ( xor_str ( "none" ), [] ( const std::string& /*unused*/, usercmd_t* /*cmd*/, bool /*unused*/ )
    {
    } );
    this->register_real_base_antiaim ( xor_str ( "at target" ), [] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        auto best_fov = FLT_MAX;
        C_BasePlayer* best_player = nullptr;

        QAngle viewangles;
        interfaces::engine_client->get_viewangles ( &viewangles );

        for ( auto i = 1; i < interfaces::engine_client->get_max_clients( ); i++ )
        {
            auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

            if ( !entity || !g_local || !entity->is_player( ) || entity == g_local || entity->is_dormant( )
                || !entity->is_alive( ) || !entity->is_enemy( ) )
                continue;

            const auto current_fov = g_math.get_fov ( viewangles, g_math.calc_angle ( g_local->get_eye_pos( ), entity->get_eye_pos( ) ) );

            if ( current_fov < best_fov )
            {
                best_fov = current_fov;
                best_player = entity;
            }
        }

        if ( best_player )
            cmd->viewangles.yaw = g_math.calc_angle ( g_local->get_eye_pos( ), best_player->vec_origin( ) ).yaw;
    } );
    this->register_real_base_antiaim ( xor_str ( "spinbot" ), [] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        cmd->viewangles.yaw = fmodf ( interfaces::global_vars->tickcount * 10.f, 360.f );
    } );
    this->register_real_base_antiaim ( xor_str ( "random" ), [] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        cmd->viewangles.yaw = g_math.random_float ( 0, 360.f );
    } );
    this->register_real_base_antiaim ( xor_str ( "freestand" ), [&] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        auto local = static_cast< C_BasePlayer* > ( g_local );

        if ( !local || !local->is_alive( ) )
        {
            cmd->viewangles.yaw -= 180.f;
            return;
        }

        auto best_player = get_lowest_fov_player( );

        if ( best_player == nullptr )
        {
            cmd->viewangles.yaw -= 180.f;
            return;
        }

        auto eye_pos = local->get_eye_pos( );

        auto enemy_eye_position = best_player->get_eye_pos( );

        g_features.autowall.overwrite_hitbox ( hitbox_head );

        auto get_rotation_position = [&] ( const float rotation, const float used_radius = 60.f )
        {
            return vector_t ( used_radius * cos ( deg2rad ( rotation ) ) + eye_pos.x, used_radius * sin ( deg2rad ( rotation ) ) + eye_pos.y,
                              eye_pos.z + 4.f );
        };
        const auto get_thickness = [&] ( const float rotation, vector_t start_pos ) -> float
        {
            return g_features.autowall.get_thickness ( start_pos, get_rotation_position ( rotation ), 4096.f );
        };

        const auto quick_trace_hit = [&] ( const float rotation, float rad ) -> bool
        {
            CGameTrace trace;
            CTraceFilter filter;
            filter.pSkip = best_player;
            Ray_t ray;
            ray.Init ( enemy_eye_position, get_rotation_position ( rotation, rad ) );
            interfaces::engine_trace->trace_ray ( ray, MASK_SHOT, &filter, &trace );

            return trace.hit_entity == nullptr || trace.hit_entity == local;
        };

        const auto tmp_calc_rad = local->get_hitbox_pos ( hitbox_head ).dist_to ( local->vec_origin( ) );

        const auto plus = get_thickness ( cmd->viewangles.yaw + 90.f, enemy_eye_position );
        const auto minus = get_thickness ( cmd->viewangles.yaw - 90.f, enemy_eye_position );
        const auto hit_plus = plus < 5.f || quick_trace_hit ( cmd->viewangles.yaw + 90.f, tmp_calc_rad );
        const auto hit_minus = minus < 5.f || quick_trace_hit ( cmd->viewangles.yaw - 90.f, tmp_calc_rad );

        auto pred_plus = 0.f;
        auto pred_minus = 0.f;
        auto pred_hit_plus = false;
        auto pred_hit_minus = false;
        auto use_pred = false;;

        if ( best_player->vec_velocity( ).length2d( ) > 0.1f )
        {
            use_pred = true;
            // the player is moving so we predict his position so we cant get backtracked that easy
            const auto predicted_eye_position = enemy_eye_position + ( best_player->vec_velocity( ) * interfaces::global_vars->interval_per_tick ) * 10.f;

            pred_plus = get_thickness ( cmd->viewangles.yaw + 90.f, predicted_eye_position );
            pred_minus = get_thickness ( cmd->viewangles.yaw - 90.f, predicted_eye_position );
            pred_hit_plus = plus < 5.f || quick_trace_hit ( cmd->viewangles.yaw + 90.f, tmp_calc_rad );
            pred_hit_minus = minus < 5.f || quick_trace_hit ( cmd->viewangles.yaw - 90.f, tmp_calc_rad );
        }

        if ( plus == minus || ( hit_plus && hit_minus ) || ( use_pred && pred_hit_plus && pred_hit_minus ) || ( use_pred && pred_plus == pred_minus ) )
        {
            cmd->viewangles.yaw += 180.f;
            return;
        }

        if ( plus > minus )
        {
            cmd->viewangles.yaw += 90.f;
            return;
        }

        cmd->viewangles.yaw -= 90.f;
    } );
#ifdef _DEBUG
    //this->register_real_base_antiaim ( xor_str ( "peek desync" ), [&] ( const std::string&, usercmd_t* cmd, bool send_packet )
    //{

    //} );
#endif // DEBUG

    this->register_real_add_antiaim ( xor_str ( "none" ), [] ( const std::string& /*unused*/, usercmd_t* /*unused*/, bool /*unused*/ )
    {
    } );
    this->register_real_add_antiaim ( xor_str ( "jitter" ), [] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        static auto last_yaw = 0.f;
        static auto st = 0;

        if ( interfaces::client_state->chokedcommands == 0 )
        {
            st++;

            if ( st < 2 )
            {
                static auto j = false;
                cmd->viewangles.yaw += j ? g_options.ragebot_antiaim_yaw_jitter_range : -g_options.ragebot_antiaim_yaw_jitter_range;
                last_yaw = cmd->viewangles.yaw;
                j = !j;
            }
            else
            {
                st = 0;
                last_yaw = cmd->viewangles.yaw;
            }
        }

        cmd->viewangles.yaw = last_yaw;
    } );
    this->register_real_add_antiaim ( xor_str ( "switch" ), [] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        static auto last_real = 0.f;

        if ( interfaces::client_state->chokedcommands == 0 )
        {
            static auto sw = false;
            cmd->viewangles.yaw += sw ? g_options.ragebot_antiaim_yaw_switch_range : -g_options.ragebot_antiaim_yaw_switch_range;
            last_real = cmd->viewangles.yaw;
            sw = !sw;
        }
        else
            cmd->viewangles.yaw = last_real;
    } );
    this->register_real_add_antiaim ( xor_str ( "half spin" ), [] ( const std::string& /*unused*/, usercmd_t* cmd, bool /*unused*/ )
    {
        const auto y = cmd->viewangles.yaw - 90.f;
        cmd->viewangles.yaw = y + fmodf ( interfaces::global_vars->tickcount * 5, 180.f );
    } );

    // desync
    this->register_desync_antiaim ( xor_str ( "none" ), [&] ( const std::string& /*unused*/, usercmd_t* /*unused*/, bool& /*unused*/, float /*unused*/ )
    {
        m_should_overwrite_real = false;
    } );
    this->register_desync_antiaim ( xor_str ( "static" ), [&] ( const std::string& /*unused*/, usercmd_t* cmd, bool& send_packet, const float max_rotation )
    {
        if ( !send_packet )
            return;

        m_should_overwrite_real = false;
        cmd->viewangles.yaw -= max_rotation; // bug fix head lby flick
    } );
    this->register_desync_antiaim ( xor_str ( "test" ), [&] ( const std::string& /*unused*/, usercmd_t* cmd, bool& send_packet, const float max_rotation )
    {
        m_should_overwrite_real = true; // bug not enough rotation -> rotate more while choking not last tick
        static auto state = false;

        if ( !send_packet )
        {
            if ( state )
                m_overwrite_real = cmd->viewangles.yaw;
            else
                m_overwrite_real = cmd->viewangles.yaw - 180.f;

            return;
        }

        if ( state )
            cmd->viewangles.yaw = m_overwrite_real + max_rotation;
        else
            cmd->viewangles.yaw = m_overwrite_real - max_rotation;

        state = !state;
    } );
    this->register_desync_antiaim ( xor_str ( "test2" ), [&] ( const std::string& /*unused*/, usercmd_t* cmd, bool& send_packet, float /*unused*/ )
    {
        static auto sw = false; // bug fix moving shit

        if ( send_packet )
        {
            m_overwrite_real = cmd->viewangles.yaw + ( sw ? -80.f : 80.f );
            m_should_overwrite_real = true;
        }
        else
        {
            //m_should_overwrite_real = false;
            sw = !sw;
        }
    } );
    this->register_desync_antiaim ( xor_str ( "jitter" ), [&] ( const std::string& /*unused*/, usercmd_t* cmd, bool& send_packet, const float max_rotation )
    {
        m_should_overwrite_real = false; // fix standing
        auto jitter_side = 1;

        const auto in_lby_update = g_local->flags( ) & fl_onground &&
            m_next_lby_update_time - globals.curtime <= interfaces::global_vars->interval_per_tick;

        switch ( g_options.ragebot_manualaa_mode )
        {
        case 0:
            jitter_side = get_automatic_desync_dir( );
            break;

        case 1:
            jitter_side = -1;
            break;

        case 2:
            jitter_side = 1;
            break;
        }

        send_packet = true;
        const auto yaw_desync = max_rotation;
        const auto yaw_inverse_desync = 190.0f - yaw_desync;
        const auto yaw_jitter = 180.0f - yaw_inverse_desync * 0.5f;

        if ( jitter_side == 1 )
            cmd->viewangles.yaw += yaw_jitter;
        else if ( jitter_side == -1 )
            cmd->viewangles.yaw -= yaw_jitter;

        const auto jitter_rotate = ( 90.0f * jitter_side ) + cmd->viewangles.yaw;

        if ( desync_rotate ( jitter_rotate, jitter_side, cmd, send_packet ) )
        {
            m_rotate = 0;
            return;
        }

        auto rotate = 0;

        if ( globals.curtime < this->m_next_lby_update_time )
            rotate = m_rotate;
        else
            m_rotate = 0;

        rotate--;

        if ( rotate )
        {
            if ( rotate == 1 )
            {
                if ( jitter_side == 1 )
                    cmd->viewangles.yaw = yaw_inverse_desync;
                else
                    cmd->viewangles.yaw += yaw_desync - 190.0f;
            }
        }
        else
        {
            if ( jitter_side == 1 )
                cmd->viewangles.yaw += yaw_desync - 190.0f;
            else
                cmd->viewangles.yaw += yaw_inverse_desync;

            send_packet = false;
        }

        if ( ++m_rotate >= 3 )
            m_rotate = 0;

        g_math.normalize_angles ( cmd->viewangles );
    } );

    // pitch
    this->register_pitch_antiaim ( xor_str ( "none" ), [&] ( const std::string& /*name*/, usercmd_t* /*cmd*/, bool /*send_packet*/ )
    {
    } );
    this->register_pitch_antiaim ( xor_str ( "down" ), [&] ( const std::string& /*name*/, usercmd_t* cmd, bool /*send_packet*/ )
    {
        cmd->viewangles.pitch = 89.f;
    } );
    this->register_pitch_antiaim ( xor_str ( "up" ), [&] ( const std::string& /*name*/, usercmd_t* cmd, bool /*send_packet*/ )
    {
        cmd->viewangles.pitch = -89.f;
    } );
    this->register_pitch_antiaim ( xor_str ( "zero" ), [&] ( const std::string& /*name*/, usercmd_t* cmd, bool /*send_packet*/ )
    {
        cmd->viewangles.pitch = 0.f;
    } );
}

bool c_anti_aim::desync_rotate ( const float rotation, const int direction, usercmd_t* cmd, bool& send_packet )
{
    const auto anim_state = g_local->get_base_player_anim_state( );
    const auto feet_yaw = deg2rad ( anim_state->m_flGoalFeetYaw );

    float feet_sin, feet_cos;
    DirectX::XMScalarSinCos ( &feet_sin, &feet_cos, feet_yaw );

    float feet_sin1, feet_cos1;
    DirectX::XMScalarSinCos ( &feet_sin1, &feet_cos1, m_goal_feet_yaw );

    float feet_sin2, feet_cos2;
    DirectX::XMScalarSinCos ( &feet_sin2, &feet_cos2, m_previous_feet_yaw );

    m_previous_feet_yaw = m_goal_feet_yaw;
    m_goal_feet_yaw = anim_state->m_flGoalFeetYaw;

    auto total_rotation = atan2 ( feet_sin1 + feet_sin + feet_sin2, feet_cos1 + feet_cos + feet_cos2 );
    //totalRotation = g_math.normalize_ ::AngleNormalize ( rad2deg ( totalRotation ) - rotation );
    total_rotation = rad2deg ( total_rotation ) - rotation;
    g_math.normalize_yaw ( total_rotation );

    if ( direction == 1 )
    {
        if ( total_rotation >= 0.0f )
        {
            m_rotate_iteration = 1;
            return false;
        }
    }
    else if ( total_rotation <= 0.0f )
    {
        m_rotate_iteration = 1;
        return false;
    }

    const auto rotate = static_cast< float > ( 30.f * ( m_rotate_iteration % 12 ) );

    if ( direction == 1 )
        cmd->viewangles.yaw -= rotate;
    else
        cmd->viewangles.yaw += rotate;

    //cmd->viewangles.y = Math::AngleNormalize ( g_Context->m_Cmd->viewangles.y );
    //cmd->viewangles.yaw =
    g_math.normalize_yaw ( cmd->viewangles.yaw );
    send_packet = false;
    ++m_rotate_iteration;
    return true;
}

void c_anti_aim::run_lby_prediction ( )
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    static auto spawntime = -1.f;
    auto state = reinterpret_cast< CCSGOPlayerAnimState* > ( g_local->get_base_player_anim_state( ) );

    if ( !state )
        return;

    if ( spawntime != g_local->spawn_time( ) )
    {
        spawntime = g_local->spawn_time( );
        m_next_lby_update_time = 0.f;
    }

    if ( state->get_vec_velocity( ).length2d( ) > 0.1f )
        m_next_lby_update_time = globals.curtime + 0.22f;
    else if ( m_next_lby_update_time == -1.f || globals.curtime >= m_next_lby_update_time )
        m_next_lby_update_time = globals.curtime + 1.1f;
}

void c_anti_aim::run_antiaim ( usercmd_t* cmd, bool& send_packet )
{
    if ( !g_options.ragebot_antiaim_enable )
        return;

    const auto local = ctx::client.local;

    if ( !local || !local->is_alive( ) )
        return;

    auto weapon = local->active_weapon( ).get( );

    if ( !weapon )
        return;

    if ( weapon->is_grenade( ) )
    {
        if ( weapon->throw_time( ) > 0.1f )
            return;
    }
    else
    {
        if ( ( weapon->can_fire( ) || weapon->is_knife( ) ) && cmd->buttons & IN_ATTACK )
            return;

        if ( weapon->item( ).item_definition_index( ) != WEAPON_C4 && cmd->buttons & IN_USE )
            return;

        if ( weapon->item( ).item_definition_index( ) != WEAPON_C4 && weapon->is_rifle( ) && !weapon->can_fire( ) )
            cmd->buttons &= ~IN_ATTACK;

        if ( weapon->is_knife( ) && g_options.ragebot_antiaim_disable_on_knife )
            return;
    }

    if ( !pitch_antiaims.empty( ) )
        this->pitch_antiaims.at ( g_options.ragebot_antiaim_pitch_mode ) ( g_options.ragebot_antiaim_pitch_mode, cmd, send_packet );

    // setting yaw
    if ( !manual_aa ( cmd ) )
    {
        // we just need to check for one bc they all get setup at the same time
        if ( !this->real_base_antiaims.empty( ) )
        {
            // run all yaw things here
            this->real_base_antiaims.at ( g_options.ragebot_antiaim_yaw_base_mode ) ( g_options.ragebot_antiaim_yaw_base_mode, cmd, send_packet );

            if ( g_options.ragebot_antiaim_yaw_base_mode != xor_str ( "freestand" ) )
                cmd->viewangles.yaw += g_options.ragebot_antiaim_yaw_add;

            this->real_add_antiaims.at ( g_options.ragebot_antiaim_yaw_mode ) ( g_options.ragebot_antiaim_yaw_mode, cmd, send_packet );
        }
    }

    this->last_pre_yaw = cmd->viewangles.yaw;

	if (g_options.ragebot_antiaim_desync_mode == xor_str("static"))
			cmd->viewangles.yaw += g_local->get_max_desync_delta();
		

    // run desync here
    if ( !this->desync_antiaims.empty( ) )
        this->desync_antiaims.at ( g_options.ragebot_antiaim_desync_mode ) ( g_options.ragebot_antiaim_desync_mode, cmd, send_packet,
                                                                             g_local->get_max_desync_delta( ) );

    if ( m_should_overwrite_real && !send_packet )
        cmd->viewangles.yaw = m_overwrite_real;

    if ( !send_packet )
        m_real_yaw_jitter = cmd->viewangles.yaw;
    else
        m_last_fake_yaw = cmd->viewangles.yaw;
}

int c_anti_aim::get_automatic_desync_dir ( ) // bug we definitely need to fix this
{
    qangle_t vang;
    interfaces::engine_client->get_viewangles ( &vang );
    const auto eye_position = g_local->get_eye_pos( );

    auto best_fov = FLT_MAX;
    C_BasePlayer* best_player = nullptr;

    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto player = static_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !player || !player->is_alive( ) )
            continue;

        vector_t v;

        if ( !player->get_hitbox_pos ( hitbox_head, v ) )
            continue;

        const auto tmp = g_math.get_fov ( vang, g_math.calc_angle ( eye_position, v ) );

        if ( tmp < best_fov )
        {
            best_fov = tmp;
            best_player = player;
        }
    }

    if ( !best_player )
        return 1;

    const auto distance = g_local->vec_origin( ).dot ( best_player->vec_origin( ) );
    return 2 * ( distance <= 0.0f ) - 1;
}

bool c_anti_aim::manual_aa ( usercmd_t* cmd )
{
    if ( g_options.ragebot_manualaa_mode == 0 )
        return false;

    switch ( g_options.ragebot_manualaa_mode )
    {
    case 1:
        cmd->viewangles.yaw += 90;
        return true;

    case 2:
        cmd->viewangles.yaw -= 90;
        return true;

    case 3:
        cmd->viewangles.yaw -= 180;
        return true;

    default:
        break;
    }

    return false;
}

// todo remove this function
void c_anti_aim::lby_breaker ( usercmd_t* cmd, bool& send_packet )
{
    globals.in_lby_break = false;

    //if ( g_options.ragebot_antiaim_desync_mode == xor_str ( "none" ) )
    //    return;

    if ( !g_options.ragebot_antiaim_enable /*|| g_options.ragebot_antiaim_desync_mode*/ )
        return;

    if ( !g_local || !g_local->is_alive( ) )
        return;

    static auto spawntime = g_local->spawn_time( );
    auto weapon = g_local->active_weapon( ).get( );

    if ( !weapon )
        return;

    if ( weapon->item( ).item_definition_index( ) != WEAPON_C4 && ( cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK ) )
        return;

    auto state = reinterpret_cast< CCSGOPlayerAnimState* > ( g_local->get_base_player_anim_state( ) );

    if ( !state )
        return;

    static auto next_update = -1.f;

    const auto curtime = g_utils.get_estimate_server_time ( cmd );
    {
        if ( spawntime != g_local->spawn_time( ) )
        {
            spawntime = g_local->spawn_time( );
            next_update = -1.f;
        }

        if ( state->get_vec_velocity( ).length2d( ) > 0.1f )
            next_update = curtime + 0.22f;
        else if ( next_update == -1.f || curtime >= next_update )
            next_update = curtime + 1.1f;
    }

    if ( g_local->vec_velocity( ).length2d( ) > 0.1f )
        return;

    if ( g_options.ragebot_antiaim_desync_mode == xor_str ( "none" ) )
        return;

    const auto in_lby_update = g_local->flags( ) & fl_onground && next_update - curtime <= interfaces::global_vars->
        interval_per_tick;

    auto broke_this_tick = false;

    if ( in_lby_update )
    {
        broke_this_tick = true;

        if ( g_options.ragebot_antiaim_desync_mode == xor_str ( "jitter" ) )
        {
            cmd->viewangles.yaw = last_pre_yaw;
            //switch ( g_options.ragebot_manualaa_mode )
            //{
            //case 0:
            //    cmd->viewangles.yaw = last_pre_yaw - 120.f;
            //    break;

            //case 1:
            //    cmd->viewangles.yaw = last_pre_yaw - 120.f;
            //    break;

            //case 2:
            //    cmd->viewangles.yaw = last_pre_yaw + 180.f;
            //    break;
            //}
        }
        else
            cmd->viewangles.yaw = last_pre_yaw - 120.f;

        send_packet = false;
    }

    static auto broke_last_tick = false;
    static auto was_break_lastlast_tick = false;

    if ( was_break_lastlast_tick )
    {
        send_packet = false;
        was_break_lastlast_tick = false;
        return;
    }

    if ( broke_last_tick )
    {
        was_break_lastlast_tick = true;
        send_packet = true;
        cmd->viewangles.yaw = last_pre_yaw;
        broke_last_tick = false;
        return;
    }

    if ( broke_this_tick )
    {
        if ( !weapon->is_grenade( ) )
            cmd->buttons &= ~IN_ATTACK;

        send_packet = false;
        globals.set_curtime_after_prediction = true;
        globals.in_lby_break = true;
        broke_last_tick = true;
    }
}

void c_anti_aim::finish_antiaim ( usercmd_t* cmd, bool& send_packet )
{
    last_tick_yaw_ = 0.f;
    this->saved_data.did_shot_last_tick = false;

    if ( cmd->buttons & IN_ATTACK )
    {
        last_tick_in_shot_ = true;
        last_tick_yaw_ = cmd->viewangles.yaw;
        did_shot_in_this_fakelag_ = true;
        did_shot_in_this_fakelag_angle_ = cmd->viewangles;
        this->saved_data.did_shot_last_tick = true;
    }

    if ( interfaces::client_state->chokedcommands == 0 )
    {
        //globals.prev_real_yaw = globals.last_real_yaw;
        //globals.prev_real_pitch = globals.real_pitch;
        did_shot_in_this_fakelag_ = false;
        did_shot_in_this_fakelag_angle_ = QAngle ( 0, 0, 0 );
    }

    /* todo get this one tick earlier somehow please xD bc aas are broken bc of this */
    static auto last_ang = QAngle ( 0, 0, 0 );

    static auto last_sendpacket = false;

    if ( !send_packet || ( last_sendpacket && send_packet ) )
        last_ang = cmd->viewangles;

    if ( send_packet )
    {
        saved_data.last_ang = last_ang; // does always save last yaw bc its real
        ctx::cheat.should_update_animations = true;
    }

    last_sendpacket = send_packet;
}

void c_anti_aim::register_real_base_antiaim ( const std::string& name, const std::function< void  ( std::string, usercmd_t*, bool ) >& callback )
{
    this->real_base_antiaims[ name ] = callback;
    this->real_base_antiaim_names.push_back ( name );
}

void c_anti_aim::register_real_add_antiaim ( const std::string& name, const std::function< void  ( std::string, usercmd_t*, bool ) >& callback )
{
    this->real_add_antiaims[ name ] = callback;
    this->real_add_antiaim_names.push_back ( name );
}

void c_anti_aim::register_desync_antiaim ( const std::string& name, std::function< void  ( std::string, usercmd_t*, bool&, float ) > callback )
{
    this->desync_antiaims[ name ] = std::move ( callback );
    this->desync_antiaim_names.push_back ( name );
}

void c_anti_aim::register_pitch_antiaim ( const std::string& name, std::function< void  ( std::string, usercmd_t*, bool ) > callback )
{
    this->pitch_antiaims[ name ] = std::move ( callback );
    this->pitch_antiaim_names.push_back ( name );
}

void c_anti_aim::slowwalk ( usercmd_t* cmd )
{
    static auto has_stopped = false;
    globals.in_slowwalk = false;

    if ( !globals.rbot_force_slowwalk )
        has_stopped = false;

    if ( !g_local || !g_local->is_alive( ) || ! ( g_local->flags( ) & fl_onground ) )
        return;

    if ( !g_inputsys.is_key_down ( g_options.ragebot_accuracy_slowwalk_key ) && !globals.rbot_force_slowwalk )
        return;

    cmd->buttons &= ~IN_WALK;

    if ( !g_local || !g_local->is_alive( ) )
        return;

    if ( ! ( g_local->flags( ) & fl_onground ) )
        return;

    auto weapon = g_local->active_weapon( ).get( );

    if ( !weapon || weapon->is_planted_c4( ) || weapon->get_client_class( )->class_id == CC4 )
        return;

    globals.in_slowwalk = true;

    auto speed = *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( g_local->active_weapon( ).get( )->get_cs_weapon_data( ) ) + 0x0130 ) *
        0.30f * 0.5f
        * g_options.ragebot_accuracy_slowwalk_speed; //  its 0.34f but 0.32 works better

    if ( globals.rbot_force_slowwalk )
        speed *= 0.5f;

    const auto min_speed = g_math.fastsqrt ( cmd->forwardmove * cmd->forwardmove + cmd->sidemove * cmd->sidemove + cmd->upmove * cmd->upmove );

    if ( min_speed <= 0.f )
        return;

    if ( cmd->buttons & IN_DUCK )
    {
        const auto state = g_local->get_base_player_anim_state( );

        if ( state )
            speed *= 2.94117647f - ( 2.94117647f * state->m_fDuckAmount );
        else
            speed *= 2.94117647f;
    }

    if ( min_speed <= speed )
        return;

    if ( globals.rbot_force_slowwalk )
    {
        static auto last_speed = g_local->vec_velocity( ).length2d( );

        if ( g_local->vec_velocity( ).length2d( ) > last_speed )
            has_stopped = true;

        if ( has_stopped )
        {
            cmd->forwardmove = 0;
            cmd->sidemove = 0;
            cmd->upmove = 0;
        }
        else
        {
            cmd->forwardmove = -cmd->forwardmove;
            cmd->sidemove = -cmd->sidemove;
            cmd->upmove = -cmd->upmove;
            has_stopped = false;
        }

        last_speed = g_local->vec_velocity( ).length2d( );
        globals.rbot_force_slowwalk = false;

        return;
    }

    const auto final_speed = speed / min_speed;

    cmd->forwardmove *= final_speed;
    cmd->sidemove *= final_speed;
    cmd->upmove *= final_speed;
    globals.rbot_force_slowwalk = false;
}

void c_anti_aim::fake_crouch ( usercmd_t* cmd, bool& send_packet )
{
    globals.in_fakeduck = false;

    if ( !g_local || !g_local->is_alive( ) )
        return;

    if ( !g_inputsys.is_key_down ( g_options.ragebot_antiaim_fakeduck_key ) )
        return;

    globals.in_fakeduck = true;

    if ( interfaces::client_state->chokedcommands <= 7 )
        cmd->buttons &= ~IN_DUCK;
    else
        cmd->buttons |= IN_DUCK;

    send_packet = interfaces::client_state->chokedcommands > 14;

    globals.fackeduck_send_packet_overwrite = send_packet;
}
