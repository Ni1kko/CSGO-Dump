#include "ragebot.h"
#include "../../../helpers/math.hpp"
#include "../../../helpers/console.h"
#include "../../../helpers/debug_console.h"
#include "../../../options.h"
#include "autowall.h"
#include "../resolver/resolver.h"
#include "../../misc/event_logger.h"
#include "../../features.h"

/* todo fix bug when backtrack turned on baim when lethal not working */
void c_ragebot::on_create_move ( usercmd_t* cmd, bool& send_packet )
{
    static auto last_in_shot = false;
    static auto last_shot_time = 0.f;

    if ( last_in_shot )
    {
        last_in_shot = false;
        return;
    }

    const auto curtime = interfaces::global_vars->curtime;

    this->did_shoot = false;

    if ( last_shot_time > curtime )
        last_shot_time = curtime;

    const auto suppressed_tick_delta = interfaces::global_vars->tickcount - globals.prev_supressed_tick;

    if ( suppressed_tick_delta < 0 )
        globals.prev_supressed_tick = interfaces::global_vars->tickcount;

    if ( !g_options.ragebot_master )
        return;

    g_features.autowall.reset_overwrite( );

    const auto local = static_cast< C_BasePlayer* > ( g_local );

    if ( !local || !local->is_alive( ) )
        return;

    auto weapon = local->active_weapon( ).get( );

    if ( !weapon || !weapon->has_bullets( ) || !weapon->can_fire( ) || weapon->item( ).item_definition_index( ) == WEAPON_C4 )
        return;

    if ( weapon->is_zeus( ) )
    {
        zeusbot ( cmd, local, weapon, send_packet );
        return;
    }

    if ( weapon->is_knife( ) )
        return;

    auto weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return;

    if ( curtime - ( last_shot_time + *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( weapon_data ) + 0xDC ) ) < 0.f )
        return;

    if ( !g_options.ragebot_autoshoot && ! ( cmd->buttons & IN_ATTACK ) )
        return;

    if ( !g_options.ragebot_autoshoot )
        cmd->buttons &= ~IN_ATTACK;

    if ( g_options.ragebot_shooting_mode == static_cast< int > ( shooting_modes_t::supress ) && interfaces::client_state->chokedcommands >= 15 )
        return;

    //console::write_line ( "sequence: " + std::to_string ( weapon->get_sequence() ) );

    //update_weapon_config ( weapon );

    auto damage = 0.f;
    auto position = Vector ( 0, 0, 0 );
    auto entity_index = -1;
    static auto last_entity = entity_index;
    auto currently_baiming = false;
    auto ragebot_hitbox = 0;

    create_hitbox_list( );

    backtrack_all_data ball_data;
    auto using_backtrack = false;

    if ( !get_target_by_damage ( local, weapon, damage, position, entity_index, currently_baiming, ragebot_hitbox, ball_data, using_backtrack ) )
        return;

    /*if ( using_backtrack )
        console::write_line ( "backtrack" );*/

    if ( ( last_entity == -1 || last_entity != entity_index ) && static_cast< shooting_modes_t > ( g_options.ragebot_shooting_mode ) == shooting_modes_t::
        normal
    )
    {
        last_entity = entity_index;

        if ( entity_index != -1 )
        {
            QAngle ang = g_math.calc_angle ( g_local->get_eye_pos ( g_features.animations.m_real_state ), position );
            g_math.normalize_angles ( ang );
            g_math.clamp_angles ( ang );
            interfaces::engine_client->set_viewangles ( &ang );
        }

        return;
    }

    if ( entity_index == -1 )
        return;

    const auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( entity_index ) );

    if ( !entity || !entity->is_alive( ) )
        return;

    player_backup_data_t org_data;

    auto simtime_to_shoot = entity->simulation_time( );

    if ( using_backtrack )
    {
        simtime_to_shoot = ball_data.simtime;
        org_data.create ( entity );
        ball_data.ndata.restore ( entity );
    }

    if ( static_cast< shooting_modes_t > ( g_options.ragebot_shooting_mode ) == shooting_modes_t::antiaim && interfaces::client_state->chokedcommands >= 1 )
        send_packet = true;

    if ( static_cast< shooting_modes_t > ( g_options.ragebot_shooting_mode ) == shooting_modes_t::antiaim && !send_packet )
    {
        if ( using_backtrack )
            org_data.restore ( entity );

        return;
    }

    const auto tick = time_to_ticks ( simtime_to_shoot + g_features.rage_backtrack.get_lerp_time() );
    auto ang = g_math.calc_angle ( g_local->get_eye_pos ( g_features.animations.m_real_state ), position );
    static auto weapon_recoil_scale = interfaces::cvar->find_var ( xor_str ( "weapon_recoil_scale" ) );
    ang -= globals.old_aimpunch * weapon_recoil_scale->get_float( );
    g_math.normalize_angles ( ang );
    static auto last_in_scope = 0;

    if ( weapon->is_sniper( ) && last_in_scope <= 3 )
    {
        last_in_scope++;

        if ( using_backtrack )
            org_data.restore ( entity );

        return;
    }

    if ( weapon->is_sniper( ) && !local->is_scoped( ) )
    {
        last_in_scope = 0;
        auto_scope ( cmd );

        if ( using_backtrack )
            org_data.restore ( entity );

        return;
    }

    auto_stop ( cmd );

    if ( !hit_chance ( ang, entity, g_options.ragebot_hitchance ) )
    {
        if ( using_backtrack )
            org_data.restore ( entity );

        return;
    }

    cmd->viewangles = ang;
    g_math.normalize_angles ( cmd->viewangles );
    g_math.clamp_angles ( cmd->viewangles );

    cmd->tick_count = tick;

    if ( weapon->is_r8( ) )
        cmd->buttons |= IN_ATTACK2;
    else
        cmd->buttons |= IN_ATTACK;

    if ( !using_backtrack )
        g_features.event_logger.set_rbot_data ( entity, entity_index, ang );
    else
        g_features.event_logger.set_rbot_data_backtrack ( entity, entity_index, ang, ball_data );

    //const auto ba_last_shot_time = last_shot_time;
    this->did_shoot = true;

    if ( !weapon->is_r8( ) )
        last_shot_time = curtime;

    switch ( static_cast< shooting_modes_t > ( g_options.ragebot_shooting_mode ) )
    {
    case shooting_modes_t::normal:
        interfaces::engine_client->set_viewangles ( &cmd->viewangles );
        break;

    case shooting_modes_t::instant:
        send_packet = true;
        break;

    case shooting_modes_t::supress:
        send_packet = false;
        should_supress = true;
        break;

    default:
        break;
    }

    last_in_shot = true;
    globals.real_yaw = cmd->viewangles.yaw;

    if ( using_backtrack )
        org_data.restore ( entity );

#ifdef _DEBUG

    std::string hb;

    switch ( ragebot_hitbox )
    {
    case hitbox_head:
        hb = "HEAD";
        break;

    case hitbox_neck:
        hb = "NECK";
        break;

    case hitbox_pelvis:
    case hitbox_stomach:
        hb = "STOMACH";
        break;

    case hitbox_lower_chest:
    case hitbox_chest:
    case hitbox_upper_chest:
        hb = "CHEST";
        break;

    case hitbox_right_thigh:
    case hitbox_left_thigh:
    case hitbox_right_calf:
    case hitbox_left_calf:
    case hitbox_right_foot:
    case hitbox_left_foot:
        hb = "FOOT";
        break;

    case hitbox_right_hand:
    case hitbox_left_hand:
    case hitbox_right_upper_arm:
    case hitbox_right_forearm:
    case hitbox_left_upper_arm:
    case hitbox_left_forearm:
        hb = "ARM";
        break;

    default:
        break;
    }

    debug_console::debug ( "[ DEBUG ] ragebot => hitbox: " + hb
        //+ " shots_fired_at_entity: " + std::to_string ( g_features.resolver.resolver_data[ entity_index ].shots )
        + " choked: " + std::to_string ( interfaces::client_state->chokedcommands )
        //+ " method: " + current_method
        //+ " mode: " + current_mode
        + " backtrack: " + std::string ( using_backtrack ? "true" : "false" ) );
#endif // _DEBUG
}

bool c_ragebot::did_shoot_this_tick ( ) const
{
    return this->did_shoot;
}

// todo use next 2 functions
void c_ragebot::set_shooting ( C_BaseCombatWeapon* weapon )
{
    const auto wpn_data = weapon->get_cs_weapon_data( );

    if ( wpn_data )
    {
        this->next_shot_time = interfaces::global_vars->curtime + *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( wpn_data ) + 0xDC );
        return;
    }

    this->next_shot_time = interfaces::global_vars->curtime + ticks_to_time ( 1 );
}

bool c_ragebot::can_shoot ( ) const
{
    return interfaces::global_vars->curtime - this->next_shot_time > 0;
}

void c_ragebot::create_hitbox_list ( )
{
    current_hitboxes.clear( );

    if ( g_options.ragebot_hitbox_head )
    {
        current_hitboxes.push_back ( { hitbox_head, g_options.ragebot_hitbox_head_pointscale ? g_options.ragebot_hitbox_head_scale : 0.f } );
    }

    if ( g_options.ragebot_hitbox_neck )
    {
        current_hitboxes.push_back ( { hitbox_neck, g_options.ragebot_hitbox_neck_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
    }

    if ( g_options.ragebot_hitbox_stomach )
    {
        current_hitboxes.push_back ( { hitbox_pelvis, g_options.ragebot_hitbox_stomach_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_stomach, g_options.ragebot_hitbox_stomach_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
    }

    if ( g_options.ragebot_hitbox_chest )
    {
        current_hitboxes.push_back ( { hitbox_lower_chest, g_options.ragebot_hitbox_chest_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_chest, g_options.ragebot_hitbox_chest_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_upper_chest, g_options.ragebot_hitbox_chest_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
    }

    if ( g_options.ragebot_hitbox_feet )
    {
        current_hitboxes.push_back ( { hitbox_right_calf, g_options.ragebot_hitbox_feet_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_left_calf, g_options.ragebot_hitbox_feet_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_right_foot, g_options.ragebot_hitbox_feet_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_left_foot, g_options.ragebot_hitbox_feet_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
    }

    if ( g_options.ragebot_hitbox_arms )
    {
        current_hitboxes.push_back ( { hitbox_right_upper_arm, g_options.ragebot_hitbox_arms_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_right_forearm, g_options.ragebot_hitbox_arms_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_left_upper_arm, g_options.ragebot_hitbox_arms_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
        current_hitboxes.push_back ( { hitbox_left_forearm, g_options.ragebot_hitbox_arms_pointscale ? g_options.ragebot_hitbox_body_scale : 0.f } );
    }
}

bool c_ragebot::hit_chance ( const QAngle ang, C_BasePlayer* player, float hitchance ) const
{
    constexpr auto iterations = 150.f;

    auto weapon = g_local->active_weapon( ).get( );

    if ( !weapon )
        return false;

    Vector forward, right, up;
    const auto src = g_local->get_eye_pos ( g_features.animations.m_real_state );
    g_math.angle_vectors ( ang, forward, right, up );

    hitchance *= 100.f;

    auto c_hits = 0;
    const auto c_needed_hits = static_cast< int > ( iterations * ( hitchance / 100.f ) );

    weapon->update_accuracy_penalty( );
    const auto weapon_spread = weapon->get_spread( );
    const auto weapon_inaccuracy = weapon->get_inaccuracy( );

    for ( auto i = 0; i < static_cast< int > ( iterations ); i++ )
    {
        auto a = g_math.random_float ( 0.f, 1.f );
        const auto b = g_math.random_float ( 0.f, 2.f * pi_f );
        auto c = g_math.random_float ( 0.f, 1.f );
        const auto d = g_math.random_float ( 0.f, 2.f * pi_f );

        if ( weapon->item( ).item_definition_index( ) == WEAPON_REVOLVER )
        {
            a = 1.f - a * a;
            c = 1.f - c * c;
        }

        const auto inaccuracy = a * weapon_inaccuracy;
        const auto spread = c * weapon_spread;

        const Vector spread_view ( cos ( b ) * inaccuracy + cos ( d ) * spread, sin ( b ) * inaccuracy + sin ( d ) * spread, 0 );
        Vector direction;

        direction.x = forward.x + spread_view.x * right.x + spread_view.y * up.x;
        direction.y = forward.y + spread_view.x * right.y + spread_view.y * up.y;
        direction.z = forward.z + spread_view.x * right.z + spread_view.y * up.z;
        direction.normalize_in_place( );

        QAngle view_angles_spread;
        g_math.vector_angles ( direction, up, view_angles_spread );
        //viewAnglesSpread.Normalize();
        g_math.normalize_angles ( view_angles_spread );

        Vector view_forward;
        g_math.angle_vectors ( view_angles_spread, view_forward );
        view_forward.normalize_in_place( );

        view_forward = src + view_forward * weapon->get_cs_weapon_data( )->range;

        trace_t tr;
        Ray_t ray;

        ray.Init ( src, view_forward );
        interfaces::engine_trace->clip_ray_to_entity ( ray, MASK_SHOT | CONTENTS_GRATE, player, &tr );

        if ( tr.hit_entity == player )
            ++c_hits;

        if ( static_cast< int > ( static_cast< float > ( c_hits ) / iterations * 100.f ) > hitchance )
            return true;

        if ( static_cast< int > ( iterations ) - i + c_hits < c_needed_hits )
            return false;
    }

    return false;
}

/// selects choosen mode between all target selction modes
void c_ragebot::find_target ( C_BasePlayer* local, C_BaseCombatWeapon* weapon )
{
    if ( !weapon )
        return;

    const auto weapon_data = weapon->get_cs_weapon_data( );

    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !local || !entity->is_player( ) || entity == local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        this->saved_player_data[ i ] = { };

        if ( g_local->vec_origin( ).dist_to ( entity->vec_origin( ) ) > weapon_data->range )
            continue;

        if ( should_delay( ) )
            continue;

        this->saved_player_data[ i ].breaking_lc = is_breaking_lc( );
    }
}

bool c_ragebot::is_baim_hitbox ( const int hitbox )
{
    switch ( hitbox )
    {
    case hitbox_pelvis:
    case hitbox_stomach:
    case hitbox_lower_chest:
    case hitbox_chest:
        return true;

    default:
        return false;
    }
}

bool c_ragebot::should_delay ( )
{
    if ( g_options.ragebot_delay_mode == static_cast< int > ( delay_mode_t::none ) )
        return false;

    if ( this->saved_player_data[ this->player->ent_index( ) ].last_simtime != this->player->simulation_time( ) )
    {
        this->saved_player_data[ this->player->ent_index( ) ].last_simtime = this->player->simulation_time( );
        return false;
    }

    return true;
}

bool c_ragebot::is_breaking_lc ( )
{
    return false;
}

rbot_baim_modes_t c_ragebot::get_baimmode ( ) const
{
    if ( g_options.ragebot_baim_mode == static_cast< int > ( user_baim_modes_t::prefer ) )
        return rbot_baim_modes_t::baim;

    if ( g_options.ragebot_baim_after_shots != 0 && g_features.resolver.rdata.at ( this->player->ent_index( ) ).shots >= g_options.ragebot_baim_after_shots )
        return rbot_baim_modes_t::baim;

    if ( g_options.ragebot_baim_mode == static_cast< int > ( user_baim_modes_t::desync ) && !g_features
                                                                                             .resolver.rdata.at ( this->player->ent_index( ) ).desyncing )
        return rbot_baim_modes_t::baim;

    if ( g_options.ragebot_baim_mode == static_cast< int > ( user_baim_modes_t::adaptive ) && !g_features
                                                                                               .resolver.rdata.at ( this->player->ent_index( ) ).desyncing )
        return rbot_baim_modes_t::baim;

    return rbot_baim_modes_t::none;
}

/* todo finish implementation and fix all this code */
bool c_ragebot::calculate_target_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, float& damage_out, Vector& pos_out, int& hitbox_out,
                                          bool& using_backtrack_out, backtrack_all_data& backtrackdata_out )
{
    const auto baim = get_baimmode( );

    if ( !prepare_hitboxes ( this->player ) )
        return false;

    can_hit_struct_t can_hit_head;

    auto found_hittable = false;
    const auto i = this->player->ent_index( );

    auto entity_same_data_as_before = false;

    if ( std::ceil ( last_autowall_data[ i ].last_enemy_eyeang.yaw ) == std::ceil ( this->player->eye_angles( ).yaw )
        && std::ceil ( last_autowall_data[ i ].last_enemy_eyeang.pitch ) == std::ceil ( this->player->eye_angles( ).pitch )
        // position check
        && last_autowall_data[ i ].last_enemy_pos == this->player->get_eye_pos( )
        && last_autowall_data[ i ].local_eyepos == g_local->get_eye_pos ( g_features.animations.m_real_state )
    )
        entity_same_data_as_before = true;

    for ( auto hb : current_hitboxes )
    {
        if ( entity_same_data_as_before && !last_autowall_data[ i ].can_hit[ hb.hitbox ] )
            continue;

        if ( baim == rbot_baim_modes_t::force_baim && ( hb.hitbox == hitbox_head || hb.hitbox == hitbox_neck ) )
            continue;

        float scale = hb.pointscale;

        std::vector< point_scan_struct_t > points = get_points_for_hitscan ( this->player, hb.hitbox, scale );

        can_hit_struct_t can_hit_center;
        auto cbest_damage = -1.f;
        Vector cbest_point;

        std::deque< float > ba_damages = last_autowall_data[ i ].damage[ hb.hitbox ];
        last_autowall_data[ i ].damage[ hb.hitbox ].clear( );

        const auto ba_size = ba_damages.size( );

        for ( size_t point = 0; point < points.size( ); point++ )
        {
            auto dmg = -1.f;

            if ( entity_same_data_as_before && ba_size > point )
                dmg = ba_damages[ point ];
            else
                dmg = g_features.autowall.damage ( points[ point ].pos );

            last_autowall_data[ i ].damage[ hb.hitbox ].push_back ( dmg );

            if ( dmg == -1.f )
                continue;

            if ( points[ point ].center && dmg >= static_cast< float > ( g_options.ragebot_min_damage ) )
            {
                can_hit_center.can_hit = true;
                can_hit_center.damage = dmg;
                can_hit_center.pos = points[ point ].pos;
            }

            if ( dmg > cbest_damage )
            {
                cbest_point = points[ point ].pos;
                cbest_damage = dmg;
            }
        }

        if ( can_hit_center.can_hit )
        {
            cbest_damage = can_hit_center.damage;
            cbest_point = can_hit_center.pos;
        }

        if (
            baim == rbot_baim_modes_t::baim && hb.hitbox == hitbox_head && cbest_damage >= static_cast< float > ( g_options.ragebot_min_damage ) &&
            ( !can_hit_head.can_hit || can_hit_head.damage < cbest_damage )
        )
        {
            can_hit_head.can_hit = true; //need to test 4 highest dmg
            can_hit_head.damage = cbest_damage;
            can_hit_head.pos = cbest_point;
            continue;
        }

        last_autowall_data[ i ].init = true;
        last_autowall_data[ i ].can_hit[ hb.hitbox ] = cbest_damage != -1.f;

        auto can_baim_kill = (
            g_options.ragebot_baim_when_lethal && ( ( hb.hitbox == hitbox_chest || hb.hitbox == hitbox_lower_chest ||
                hb.hitbox == hitbox_stomach || hb.hitbox == hitbox_pelvis ) && cbest_damage >= this->player->health( ) )
        );

        if ( baim == rbot_baim_modes_t::baim )
        {
            switch ( hb.hitbox )
            {
            case hitbox_pelvis:
            case hitbox_stomach:
            case hitbox_lower_chest:
            case hitbox_chest:
                {
                    if ( cbest_damage != -1.f && cbest_damage >= static_cast< float > ( g_options.ragebot_min_damage ) && ( ( can_hit_body.can_hit &&
                        cbest_damage > can_hit_body.damage ) || !can_hit_body.can_hit ) )
                    {
                        can_hit_body.can_hit = true;
                        can_hit_body.damage = cbest_damage;
                        can_hit_body.pos = cbest_point;
                        can_hit_body.hitbox = hb.hitbox;
                        can_hit_body.using_backtrack = false;
                        using_backtrack_out = false;
                    }

                    break;
                }

            default:
                break;
            }
        }

        if ( can_baim_kill || ( cbest_damage != -1.f && cbest_damage > damage_out && cbest_damage >= static_cast< float > ( g_options.ragebot_min_damage ) )
        )
        {
            damage_out = cbest_damage;
            pos_out = cbest_point;
            //best_baim = hb.hitbox != hitbox_head && hb.hitbox != hitbox_neck && hb.hitbox != hitbox_upper_chest;
            hitbox_out = hb.hitbox;
            using_backtrack_out = false;
            found_hittable = true;
        }
    }

    auto backtrack_dmg = 0.f;
    auto backtrack_pos = Vector ( 0, 0, 0 );
    backtrack_all_data backtrack_data;
    auto backtrack_should_overwrite = false;

    auto tmp_backtrack_hitbox = -1;

    if ( process_backtrack_by_damage ( local, weapon, this->player, backtrack_dmg, backtrack_pos, backtrack_data, backtrack_should_overwrite,
                                       tmp_backtrack_hitbox ) )
    {
        if ( backtrack_should_overwrite && ( backtrack_dmg >= damage_out || backtrack_dmg >= this->player->health( ) ) )
        {
            damage_out = backtrack_dmg;
            pos_out = backtrack_pos;
            using_backtrack_out = true;
            hitbox_out = tmp_backtrack_hitbox;
            backtrackdata_out = backtrack_data;
            found_hittable = true;
        }
        else if ( backtrack_dmg > damage_out && backtrack_dmg > g_options.ragebot_min_damage )
        {
            damage_out = backtrack_dmg;
            pos_out = backtrack_pos;
            hitbox_out = tmp_backtrack_hitbox;
            backtrackdata_out = backtrack_data;
            using_backtrack_out = true;
            found_hittable = true;
        }
    }

    /* todo add backtrack to this (its not used rn) */
    if ( can_hit_body.can_hit && baim == rbot_baim_modes_t::baim )
    {
        damage_out = can_hit_body.damage;
        pos_out = can_hit_body.pos;
        hitbox_out = can_hit_body.hitbox;
        using_backtrack_out = can_hit_body.using_backtrack;
        found_hittable = true;

        if ( using_backtrack_out )
            backtrackdata_out = can_hit_body.bdata;
    }

    /* todo put this part into extra function so we can test if no one hit and overwrite it */
    if ( can_hit_head.can_hit )
    {
        damage_out = can_hit_head.damage;
        pos_out = can_hit_head.pos;
        using_backtrack_out = false;
        hitbox_out = hitbox_head;
        using_backtrack_out = can_hit_head.using_backtrack;
        found_hittable = true;

        if ( using_backtrack_out )
            backtrackdata_out = can_hit_head.bdata;
    }

    last_autowall_data[ i ].last_enemy_eyeang = this->player->eye_angles( );
    last_autowall_data[ i ].last_enemy_pos = this->player->get_eye_pos( );
    last_autowall_data[ i ].local_eyepos = g_local->get_eye_pos ( g_features.animations.m_real_state );

    return found_hittable;
}

bool c_ragebot::calculate_target_backtrack_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, backtrack_all_data& record, float& damage_out,
                                                    Vector& pos_out, int& hitbox_out )
{
    damage_out = 0.f;
    hitbox_out = -1;

    const auto baim = get_baimmode( );
    auto found_hitable = false;

    can_hit_struct_t can_hit_head;

    for ( auto hitbox : current_hitboxes )
    {
        switch ( hitbox.hitbox )
        {
        case hitbox_right_calf:
        case hitbox_left_calf:
        case hitbox_right_foot:
        case hitbox_left_foot:
        case hitbox_right_upper_arm:
        case hitbox_right_forearm:
        case hitbox_left_upper_arm:
        case hitbox_left_forearm:
            continue;

        default:
            break;
        }

        Vector pos;
        float dmg;

        g_features.autowall.overwrite_hitbox ( hitbox.hitbox );

        if ( record.adata.did_run_autowall[ hitbox.hitbox ] && record.adata.local_pos == local->get_eye_pos( ) )
        {
            pos = record.adata.hitbox_positions[ hitbox.hitbox ];
            dmg = record.adata.autowall_dmg[ hitbox.hitbox ];
        }
        else
        {
            if ( !get_hitbox ( this->player, hitbox.hitbox, pos ) )
                continue;

            /* todo implement pointscale for head only */

            //if ( ( hitbox.hitbox == hitbox_head || hitbox.hitbox == hitbox_neck || hitbox.hitbox == hitbox_pelvis || hitbox.hitbox == hitbox_chest ||
            //        hitbox.hitbox == hitbox_stomach ) && g_local->can_see_player ( entity, pos ) )
            //{
            //    if ( hitbox.hitbox == hitbox_head )
            //        dmg = 200.f;
            //    else
            //        dmg = 45.f;
            //}
            //else

            //    if ( hitbox.hitbox == hitbox_head )
            //    {
            //        for ( size_t point = 0; point < points.size ( ); point++ )
            //{
            //    auto dmg = -1.f;

            //    if ( entity_same_data_as_before && ba_size > point )
            //        dmg = ba_damages[ point ];
            //    else
            //        dmg = g_features.autowall.damage ( points[ point ].pos );

            //    last_autowall_data[ i ].damage[ hb.hitbox ].push_back ( dmg );

            //    if ( dmg == -1.f )
            //        continue;

            //    if ( points[ point ].center && dmg >= static_cast< float > ( g_options.ragebot_min_damage ) )
            //    {
            //        can_hit_center.can_hit = true;
            //        can_hit_center.damage = dmg;
            //        can_hit_center.pos = points[ point ].pos;
            //    }

            //    if ( dmg > cbest_damage )
            //    {
            //        cbest_point = points[ point ].pos;
            //        cbest_damage = dmg;
            //    }
            //}
            //    }
            //    else
            //{
            //
            //}

            dmg = g_features.autowall.can_hit_point_dmg ( pos, local->get_eye_pos( ) );

            record.adata.did_run_autowall[ hitbox.hitbox ] = true;
            record.adata.autowall_dmg[ hitbox.hitbox ] = dmg;
            record.adata.local_pos = local->get_eye_pos( );
            record.adata.hitbox_positions[ hitbox.hitbox ] = pos;
        }

        if ( dmg == -1.f )
            continue;

        /*
        if ( baim == rbot_baim_modes_t::baim )
        {
            switch ( hitbox.hitbox )
            {
            case hitbox_pelvis:
            case hitbox_stomach:
            case hitbox_lower_chest:
            case hitbox_chest:
                {
                    if ( dmg >= static_cast< float > ( g_options.ragebot_min_damage ) && ( ( can_hit_body.can_hit &&
                            dmg > can_hit_body.damage ) || !can_hit_body.can_hit ) )
                    {
                        can_hit_body.can_hit = true;
                        can_hit_body.damage = dmg;
                        can_hit_body.pos = pos;
                        can_hit_body.hitbox = hitbox.hitbox;
                        can_hit_body.using_backtrack = true;
                        can_hit_body.bdata = record;
                    }

                    break;
                }

            default:
                break;
            }
        }
        */

        if ( !record.rdata.in_shot && baim == rbot_baim_modes_t::baim && hitbox.hitbox == hitbox_head &&
            dmg >= static_cast< float > ( g_options.ragebot_min_damage ) &&
            ( !can_hit_head.can_hit || can_hit_head.damage < dmg )
        )
        {
            can_hit_head.can_hit = true; //need to test 4 highest dmg
            can_hit_head.damage = dmg;
            can_hit_head.pos = pos;
            can_hit_head.bdata = record;
            continue;
        }

        if ( dmg > damage_out && dmg > g_options.ragebot_min_damage )
        {
            damage_out = dmg;
            found_hitable = true;
            pos_out = pos;
            hitbox_out = hitbox.hitbox;

            if ( hitbox.hitbox == hitbox_head && damage_out > this->player->health( ) && record.rdata.in_shot )
                break;
        }
    }

    if ( !found_hitable && can_hit_body.can_hit )
    {
        found_hitable = true;
        damage_out = can_hit_body.damage;
        pos_out = can_hit_body.pos;
        hitbox_out = can_hit_body.hitbox;
    }

    if ( !found_hitable && can_hit_head.can_hit )
    {
        found_hitable = true;
        damage_out = can_hit_head.damage;
        pos_out = can_hit_head.pos;
        hitbox_out = hitbox_head;
    }

    g_features.autowall.reset_overwrite( );

    return found_hitable && damage_out >= static_cast< float > ( g_options.ragebot_min_damage );
}

struct hitbox_cache
{
    matrix3x4_t matrix[maxstudiobones];
};

/// precaches the hitboxes so the ragebot takes less fps
/**
 * @return whether it was was successfull to precache the hitboxes
*/
bool c_ragebot::prepare_hitboxes ( C_BasePlayer* player )
{
    const auto model = player->get_model( );

    if ( !model )
        return false;

    cached_studio_hdr = interfaces::mdl_info->get_studiomodel ( model );

    if ( !cached_studio_hdr )
        return false;

    if ( !player->setup_bones ( cached_matrix, maxstudiobones, bone_used_by_anything, player->simulation_time( ) ) ) //bone_used_by_anything
        return false;

    cached_studiohitboxset = cached_studio_hdr->get_hitbox_set ( 0 );

    return cached_studiohitboxset != nullptr;
}

/// returns the hitbox possition
/**
 * @return the hitbox possition of selected hitbox
*/
bool c_ragebot::get_hitbox ( C_BasePlayer* player, int hitbox, Vector& hitbox_pos )
{
    if ( hitbox >= hitbox_max )
        return false;

    cached_studio_box = cached_studiohitboxset->get_hitbox ( hitbox );

    if ( !cached_studio_box )
        return false;

    Vector min, max;

    g_math.vector_transform ( cached_studio_box->bbmin, cached_matrix[ cached_studio_box->bone ], min );
    g_math.vector_transform ( cached_studio_box->bbmax, cached_matrix[ cached_studio_box->bone ], max );

    hitbox_pos = ( min + max ) * 0.5f;

    return true;
}

bool c_ragebot::get_hitbox_backtrack ( C_BasePlayer* player, int hitbox, Vector& hitbox_pos, backtrack_all_data data )
{
    if ( hitbox >= hitbox_max )
        return false;

    cached_studio_box = cached_studiohitboxset->get_hitbox ( hitbox );
    //data.ndata.studiohitboxset->get_hitbox ( hitbox ); //cached_studiohitboxset->get_hitbox ( hitbox );

    if ( !cached_studio_box )
        return false;

    Vector min, max;

    g_math.vector_transform ( cached_studio_box->bbmin, data.ndata.matrix[ cached_studio_box->bone ], min );
    g_math.vector_transform ( cached_studio_box->bbmax, data.ndata.matrix[ cached_studio_box->bone ], max );

    hitbox_pos = ( min + max ) * 0.5f;

    return true;
}

bool c_ragebot::start_prediction ( C_BasePlayer* player )
{
    if ( in_prediction )
    {
#ifdef _DEBUG
        throw std::exception ( "new prediction started before old one ended" );
#else
        std::exception();
#endif // DEBUG
    }

    pre_prediction_data.create ( player );

    auto ticks = time_to_ticks ( interfaces::global_vars->curtime - player->simulation_time() );
    ticks = std::min ( ticks, 2 );
    ticks = std::max ( ticks, 12 );
    auto end = player->vec_origin( ) + player->vec_velocity( ) * interfaces::global_vars->interval_per_tick * ticks;

    player->vec_origin( ) = end;
    player->vec_origin( ) = end;
    player->set_abs_original ( end );

    if ( !player->setup_bones ( prediction_matrix, maxstudiobones, bone_used_by_anything, player->simulation_time( ) + ticks_to_time ( 0 ) /* xD */ ) )
        return false;

    in_prediction = true;
    return true;
}

void c_ragebot::end_prediction ( C_BasePlayer* player )
{
    if ( !in_prediction )
    {
#ifdef _DEBUG
        throw std::exception ( "prediction can not be finished when there is no prediction running" );
#else
        std::exception();
#endif // DEBUG
    }

    pre_prediction_data.restore ( player );

    in_prediction = false;
}

bool c_ragebot::get_predicted_hitbox ( C_BasePlayer* player, int hitbox, Vector& hitbox_pos, int& new_tickcount ) const
{
    if ( hitbox >= hitbox_max )
        return false;

    auto ticks = time_to_ticks ( interfaces::global_vars->curtime - player->simulation_time() );

    ticks = std::min ( ticks, 0 );

    if ( ticks > 12 )
        return false;

    auto state = player->get_base_player_anim_state( );

    if ( !state )
        return false;

    auto org_vec_origin = player->vec_origin( );

    auto vel_per_tick = player->vec_velocity( ) * interfaces::global_vars->interval_per_tick;
    player->vec_origin( ) += vel_per_tick * ticks;
    state->m_vOrigin = player->vec_origin( );

    player->update_client_side_animation( );

    //
    // getting all data needed to get hitbox pos here
    //

    player->vec_origin( ) = org_vec_origin;
    state->m_vOrigin = player->vec_origin( );

    return true;
}

std::vector< point_scan_struct_t > c_ragebot::get_points_for_hitscan ( C_BasePlayer* player, int hitbox, float pointscale )
{
    std::vector< point_scan_struct_t > return_val;

    if ( !cached_studiohitboxset || !cached_studio_hdr )
        return return_val;

    Vector center;

    if ( !get_hitbox ( player, hitbox, center ) )
        return return_val;

    const auto cur_angles = g_math.calc_angle ( center, g_local->get_eye_pos ( g_features.animations.m_real_state ) );

    Vector forward;
    g_math.angle_vectors ( cur_angles, forward );

    const auto right = forward.cross ( Vector ( 0, 0, 1 ) );
    const auto left = Vector ( -right.x, -right.y, right.z );

    const auto top = Vector ( 0, 0, 1 );
    auto bot = Vector ( 0, 0, -1 );

    if ( pointscale == 0.f )
    {
        return_val.emplace_back ( point_scan_struct_t{ center, true } );
        return return_val;
    }

    const auto bbox = cached_studio_box;

    if ( hitbox == hitbox_head )
    {
        for ( auto i = 0; i < 5; ++i )
        {
            return_val.emplace_back ( point_scan_struct_t{ center } );
        }

        return_val[ 1 ].pos += right * ( bbox->m_flRadius * pointscale );
        return_val[ 2 ].pos += left * ( bbox->m_flRadius * pointscale );
        return_val[ 3 ].pos = center;
        return_val[ 3 ].center = true;
        return_val[ 4 ].pos += top * ( bbox->m_flRadius * .25f );
        return_val[ 4 ].center = true;
    }
    else
    {
        for ( auto i = 0; i < 3; ++i )
        {
            return_val.emplace_back ( point_scan_struct_t{ center } );
        }

        return_val[ 0 ].pos += right * ( bbox->m_flRadius * pointscale );
        return_val[ 1 ].pos += left * ( bbox->m_flRadius * pointscale );
        //return_val[2].pos += top * ( bbox->m_flRadius * pointscale );
        //return_val[3].pos += bot * ( bbox->m_flRadius * pointscale );
        return_val[ 2 ].pos = center;
        return_val[ 2 ].center = true;
    }

    return return_val;
}

void c_ragebot::auto_scope ( usercmd_t* cmd )
{
    static auto was_last_air = false;

    if ( was_last_air || ! ( g_local->flags( ) & fl_onground ) )
    {
        was_last_air = !was_last_air;
        return;
    }

    if ( g_local->is_scoped( ) || cmd->buttons & IN_ZOOM )
        return;

    cmd->buttons |= IN_ZOOM;
}

void c_ragebot::auto_stop ( usercmd_t* cmd )
{
    if ( !g_options.ragebot_autostop )
        return;

    switch ( autostop_modes_t::faststop )
    {
    case autostop_modes_t::slowwalk:
        globals.rbot_force_slowwalk = true;
        break;

    case autostop_modes_t::faststop:
        {
            //float min_speed = g_math.fastsqrt ( cmd->forwardmove * cmd->forwardmove + cmd->sidemove * cmd->sidemove + cmd->upmove * cmd->upmove );

            /*if ( min_speed <= 0.f )
                return;

            globals.old_forwardmove *= -min_speed;
            globals.old_sidemove *= -min_speed;*/
            auto max_speed = g_local->is_scoped( )
                                 ? *reinterpret_cast< float* > ( uintptr_t ( g_local->active_weapon( ).get( )->get_cs_weapon_data( ) ) + 308 )
                                 : *reinterpret_cast< float* > ( uintptr_t ( g_local->active_weapon( ).get( )->get_cs_weapon_data( ) ) + 304 );

            if ( ctx::cheat.unpredicted_velocity.length2d( ) > max_speed * .17f ) // half of .34
            {
                const auto velocity = ctx::cheat.unpredicted_velocity;
                const float_t speed = g_local->vec_velocity( ).length( );

                auto vector_angle = [] ( const vector_t& forward, vector_t& angles )
                {
                    if ( forward.x == 0.f && forward.y == 0.f )
                    {
                        angles.x = forward.z > 0.f ? -90.f : 90.f;
                        angles.y = 0.f;
                    }
                    else
                    {
                        angles.x = rad2deg ( std::atan2 ( -forward.z, forward.length2d() ) );
                        angles.y = rad2deg ( std::atan2 ( forward.y, forward.x ) );
                    }

                    angles.z = 0.f;
                };

                vector_t direction;
                vector_angle ( velocity, direction );

                direction.y = cmd->viewangles.yaw - direction.y;

                auto angle_to_vec = [] ( const vector_t& angles, vector_t& forward )
                {
                    float sp, sy, cp, cy;

                    g_math.sin_cos ( deg2rad ( angles.y ), &sy, &cy );
                    g_math.sin_cos ( deg2rad ( angles.x ), &sp, &cp );

                    forward.x = cp * cy;
                    forward.y = cp * sy;
                    forward.z = -sp;
                };

                vector_t forward;
                //math::angle_to_vector ( direction, forward );
                angle_to_vec ( direction, forward );
                vector_t negated_direction = forward * -speed;

                const float factor = std::max ( negated_direction.x, negated_direction.y ) / 450.f;
                negated_direction *= factor;

                globals.old_forwardmove = negated_direction.x;
                globals.old_sidemove = negated_direction.y;

                cmd->forwardmove = negated_direction.x;
                cmd->sidemove = negated_direction.y;
            }

            //ctx::cheat.unpredicted_velocity

            break;
        }

    case autostop_modes_t::stop:
        globals.old_forwardmove = 0.f;
        globals.old_sidemove = 0.f;
        break;
    }
}

void c_ragebot::zeusbot ( usercmd_t* cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon, bool& send_packet )
{
    auto weapon_data = weapon->get_cs_weapon_data( );

    static bool did_shoot_last = false;

    if ( did_shoot_last )
        did_shoot_last = false;

    auto best_damage = 0.f;
    auto best_entity = -1;
    auto best_pos = Vector ( 0, 0, 0 );
    C_BasePlayer* best_pentity = nullptr;
    auto best_simtime = 0.f;

    for ( int i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !local || !entity->is_player( ) || entity == local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        if ( !prepare_hitboxes ( entity ) )
            continue;

        float distance = entity->vec_origin( ).dist_to ( local->vec_origin( ) );

        for ( int hbi = 0; hbi < 3; hbi++ )
        {
            if ( distance > weapon_data->range )
                continue;

            int hb = hitbox_max;

            switch ( hbi )
            {
            case 0:
                hb = hitbox_chest;
                break;

            case 1:
                hb = hitbox_pelvis;
                break;

            case 2:
                hb = hitbox_stomach;
                break;

            default:
                continue;
            }

            Vector pos;

            if ( !get_hitbox ( entity, hb, pos ) )
                continue;

            const auto dmg = g_features.autowall.damage ( pos );

            if ( dmg == -1.f )
                continue;

            if ( dmg > entity->health( ) + 20.f && dmg > best_damage )
            {
                best_damage = dmg;
                best_entity = i;
                best_pos = pos;
                best_pentity = entity;
                best_simtime = entity->simulation_time( );
            }
        }
    }

    if ( best_entity == -1 )
        return;

    auto ang = g_math.calc_angle ( local->get_eye_pos( ), best_pos );

    g_math.normalize_angles ( ang );
    g_math.clamp_angles ( ang );

    if ( !hit_chance ( ang, best_pentity, .6f ) )
        return;

    cmd->viewangles = ang;
    cmd->buttons |= IN_ATTACK;
    cmd->tick_count = time_to_ticks ( best_simtime + g_features.rage_backtrack.get_lerp_time() );
    did_shoot_last = true;
    send_packet = true;
}

void c_ragebot::knifebot ( usercmd_t* cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon )
{
    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !local || !entity->is_player( ) || entity == local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        if ( !prepare_hitboxes ( entity ) )
            continue;
    }
}

bool c_ragebot::in_fakelag ( C_BasePlayer* player )
{
    auto return_val = false;

    if ( simtimes[ player->ent_index( ) ] != player->simulation_time( ) )
        simtimes[ player->ent_index( ) ] = player->simulation_time( );
    else
        return_val = true;

    return return_val;
}

// todo fix ragebot

bool c_ragebot::get_target_by_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, float& damage,
                                       Vector& hitbox_pos_export, int& entity_out, bool& is_baiming_out, int& hitbox_out,
                                       backtrack_all_data& bdata_out, bool& using_backtrack_out )
{
    using_backtrack_out = false;
    const auto weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return false;

    auto best_damage = 0.f;
    auto best_pos = Vector ( 0, 0, 0 );
    auto best_entity = -1;
    auto best_baim = false;
    auto found_killable_entity = false;
    auto best_hitbox = 0;
    auto best_backtrack = false;
    auto skip_other_loops = false;

    static float simtimes[64];

    // loop through all players
    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        if ( skip_other_loops )
            break;

        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !g_local || !entity->is_player( ) || entity == g_local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        this->player = entity;

        switch ( static_cast< delay_mode_t > ( g_options.ragebot_delay_mode ) )
        {
        case delay_mode_t::shot:
            {
                if ( weapon->is_knife( ) || weapon->is_zeus( ) || weapon->is_grenade( ) || weapon->item( ).item_definition_index( ) == WEAPON_C4 )
                    break;

                if ( fabs ( weapon->last_shot_time( ) - entity->simulation_time( ) ) > interfaces::global_vars->interval_per_tick )
                    continue;
            }

            break;

        case delay_mode_t::unlag:
            if ( entity->simulation_time( ) == simtimes[ i ] )
                continue;

            simtimes[ i ] = entity->simulation_time( );
            break;
        }

        if ( found_killable_entity )
            continue;

        const auto baim = get_baimmode( );

        if ( local->vec_origin( ).dist_to ( entity->vec_origin( ) ) > weapon_data->range )
            continue;

        if ( !prepare_hitboxes ( entity ) )
            continue;

        can_hit_struct_t can_hit_head;
        can_hit_struct_t can_hit_body;
        auto can_baim_kill2 = false;

        /*
        last_autowall_data[i].last_enemy_eyeang = entity->eye_angles();
        last_autowall_data[i].last_enemy_pos = entity->get_eye_pos();
        last_autowall_data[i].local_eyepos = g_local->get_eye_pos();
        */
        auto entity_same_data_as_before = false;

        if ( ceil ( last_autowall_data[ i ].last_enemy_eyeang.yaw ) == ceil ( entity->eye_angles( ).yaw )
            && ceil ( last_autowall_data[ i ].last_enemy_eyeang.pitch ) == ceil ( entity->eye_angles( ).pitch )
            // position check
            && last_autowall_data[ i ].last_enemy_pos == entity->get_eye_pos( )
            && last_autowall_data[ i ].local_eyepos == g_local->get_eye_pos ( g_features.animations.m_real_state )
        )
            entity_same_data_as_before = true;

        for ( auto hb : current_hitboxes )
        {
            if ( skip_other_loops )
                break;

            if ( entity_same_data_as_before && !last_autowall_data[ i ].can_hit[ hb.hitbox ] )
                continue;

            if ( found_killable_entity )
                continue;

            if ( baim == rbot_baim_modes_t::force_baim && ( hb.hitbox == hitbox_head || hb.hitbox == hitbox_neck ) )
                continue;

            float scale = hb.pointscale;

            std::vector< point_scan_struct_t > points = get_points_for_hitscan ( entity, hb.hitbox, scale );

            can_hit_struct_t can_hit_center;
            auto cbest_damage = -1.f;
            Vector cbest_point;

            std::deque< float > ba_damages = last_autowall_data[ i ].damage[ hb.hitbox ];
            last_autowall_data[ i ].damage[ hb.hitbox ].clear( );

            const auto ba_size = ba_damages.size( );

            for ( size_t point = 0; point < points.size( ); point++ )
            {
                auto dmg = -1.f;

                if ( entity_same_data_as_before && ba_size > point )
                    dmg = ba_damages[ point ];
                else
                    dmg = g_features.autowall.damage ( points[ point ].pos );

                last_autowall_data[ i ].damage[ hb.hitbox ].push_back ( dmg );

                if ( dmg == -1.f )
                    continue;

                if ( points[ point ].center && dmg >= static_cast< float > ( g_options.ragebot_min_damage ) )
                {
                    can_hit_center.can_hit = true;
                    can_hit_center.damage = dmg;
                    can_hit_center.pos = points[ point ].pos;
                }

                if ( dmg > cbest_damage )
                {
                    cbest_point = points[ point ].pos;
                    cbest_damage = dmg;
                }
            }

            if ( can_hit_center.can_hit )
            {
                cbest_damage = can_hit_center.damage;
                cbest_point = can_hit_center.pos;
            }

            if (
                baim == rbot_baim_modes_t::baim && hb.hitbox == hitbox_head && cbest_damage >= static_cast< float > ( g_options.ragebot_min_damage ) &&
                ( !can_hit_head.can_hit || can_hit_head.damage < cbest_damage )
            )
            {
                can_hit_head.can_hit = true; //need to test 4 highest dmg
                can_hit_head.damage = cbest_damage;
                can_hit_head.pos = cbest_point;
                continue;
            }

            last_autowall_data[ i ].init = true;
            last_autowall_data[ i ].can_hit[ hb.hitbox ] = cbest_damage != -1.f;

            auto can_baim_kill = (
                g_options.ragebot_baim_when_lethal && ( ( hb.hitbox == hitbox_chest || hb.hitbox == hitbox_lower_chest ||
                    hb.hitbox == hitbox_stomach || hb.hitbox == hitbox_pelvis ) && cbest_damage >= entity->health( ) )
            );

            if ( baim == rbot_baim_modes_t::baim )
            {
                switch ( hb.hitbox )
                {
                case hitbox_pelvis:
                case hitbox_stomach:
                case hitbox_lower_chest:
                case hitbox_chest:
                    {
                        if ( cbest_damage != -1.f && cbest_damage >= static_cast< float > ( g_options.ragebot_min_damage ) && ( can_hit_body.can_hit &&
                            cbest_damage > can_hit_body.damage || !can_hit_body.can_hit ) )
                        {
                            can_hit_body.can_hit = true;
                            can_hit_body.damage = cbest_damage;
                            can_hit_body.pos = cbest_point;
                            can_hit_body.hitbox = hb.hitbox;
                            best_backtrack = false;
                        }

                        break;
                    }
                }
            }

            if ( cbest_damage != -1.f && cbest_damage > best_damage && cbest_damage >= static_cast< float > ( g_options.ragebot_min_damage ) || can_baim_kill
            )
            {
                best_damage = cbest_damage;
                best_pos = cbest_point;
                best_entity = i;
                best_baim = hb.hitbox != hitbox_head && hb.hitbox != hitbox_neck && hb.hitbox != hitbox_upper_chest;
                best_hitbox = hb.hitbox;
                best_backtrack = false;

                if ( can_baim_kill )
                {
                    can_baim_kill2 = true;
                    found_killable_entity = true;
                }
                else
                    can_baim_kill2 = false;
            }
        }

        auto backtrack_dmg = 0.f;
        auto backtrack_pos = Vector ( 0, 0, 0 );
        backtrack_all_data backtrack_data;
        auto backtrack_should_overwrite = false;

        auto tmp_backtrack_hitbox = -1;

        if ( process_backtrack_by_damage ( local, weapon, entity, backtrack_dmg, backtrack_pos, backtrack_data, backtrack_should_overwrite,
                                           tmp_backtrack_hitbox ) )
        {
            if ( backtrack_should_overwrite && ( backtrack_dmg >= best_damage || backtrack_dmg >= entity->health( ) ) )
            {
                skip_other_loops = true;
                best_damage = backtrack_dmg;
                best_pos = backtrack_pos;
                best_baim = false;
                using_backtrack_out = true;
                best_hitbox = tmp_backtrack_hitbox;
                bdata_out = backtrack_data;
                best_entity = i;
                best_backtrack = true;
            }
            else if ( backtrack_dmg > best_damage && backtrack_dmg > g_options.ragebot_min_damage )
            {
                best_damage = backtrack_dmg;
                best_pos = backtrack_pos;
                best_entity = i;
                best_baim = false;
                best_hitbox = tmp_backtrack_hitbox;
                bdata_out = backtrack_data;
                best_backtrack = true;
            }
        }

        if ( !skip_other_loops )
        {
            //if ( can_hit_body.can_hit && baim == rbot_baim_modes_t::baim )
            //{
            //    best_damage = can_hit_body.damage;
            //    best_pos = can_hit_body.pos;
            //    best_entity = i;
            //    best_hitbox = can_hit_body.hitbox;
            //    best_baim = true;
            //    best_backtrack = false;
            //}

            if ( can_hit_head.can_hit && best_entity == -1 )
            {
                best_baim = false;
                best_damage = can_hit_head.damage;
                best_pos = can_hit_head.pos;
                best_entity = i;
                best_backtrack = false;
                best_hitbox = hitbox_head;
            }

            if ( can_hit_body.can_hit && baim == rbot_baim_modes_t::baim )
            {
                best_damage = can_hit_body.damage;
                best_pos = can_hit_body.pos;
                best_entity = i;
                best_hitbox = can_hit_body.hitbox;
                best_baim = true;
                best_backtrack = false;
                skip_other_loops = true;
            }

            last_autowall_data[ i ].last_enemy_eyeang = entity->eye_angles( );
            last_autowall_data[ i ].last_enemy_pos = entity->get_eye_pos( );
            last_autowall_data[ i ].local_eyepos = g_local->get_eye_pos ( g_features.animations.m_real_state );
        }
    }

    if ( best_entity == -1 )
        return false;

    using_backtrack_out = best_backtrack;
    is_baiming_out = best_baim;
    damage = best_damage;
    hitbox_pos_export = best_pos;
    hitbox_out = best_hitbox;
    entity_out = best_entity;

    return true;
}

// still needs big optimisation/ eg store when already calculated damage_out
bool c_ragebot::process_backtrack_by_damage ( C_BasePlayer* local, C_BaseCombatWeapon* weapon, C_BasePlayer* entity, float& damage_out,
                                              Vector& pos_out, backtrack_all_data& backtrackrecord_out, bool& should_overwrite_out, int& hitbox_out )
{
    should_overwrite_out = false;
    hitbox_out = -1;
    damage_out = 0.f;

    const auto backtrack_mode = static_cast< backtrack_modes_t > ( g_options.ragebot_backtrack_mode );

    if ( backtrack_mode == backtrack_modes_t::none )
        return false;

    const auto i = entity->ent_index( );

    if ( g_features.rage_backtrack.saved_data[ i ].size( ) < 3 )
        return false;

    player_backup_data_t org_data;
    org_data.create ( entity );
    std::vector< backtrack_all_data > best_records;

    auto found_entity = false;
    const auto baim = get_baimmode( );
    const auto local_eyepos = local->get_eye_pos( );

    //can_hit_struct_t can_hit_head;
    auto last_simtime = 0.f;
    //can_hit_struct_t local_can_hit_head;

    if ( backtrack_mode == backtrack_modes_t::last )
    {
        auto record = g_features.rage_backtrack.saved_data[ i ].front( );

        if ( record.simtime == entity->simulation_time( ) )
            return false;

        if ( !g_features.rage_backtrack.tick_valid ( time_to_ticks ( record.simtime ) ) )
            return false;

        if ( !record.ndata.restore ( entity ) )
            return false;

        memcpy ( cached_matrix, record.ndata.matrix, sizeof ( matrix3x4_t ) * 128 );

        if ( calculate_target_backtrack_damage ( local, weapon, record, damage_out, pos_out, hitbox_out ) )
        {
            found_entity = true;
            backtrackrecord_out = record;
        }

        g_features.rage_backtrack.saved_data[ i ].front( ) = record;
    }
    else if ( backtrack_mode == backtrack_modes_t::all )
    {
        const auto size = static_cast< int > ( g_features.rage_backtrack.saved_data[ i ].size( ) );

        for ( auto idx = 0; idx < size; idx++ )
        {
            if ( last_simtime == g_features.rage_backtrack.saved_data[ i ][ idx ].simtime || g_features.rage_backtrack.saved_data[ i ][ idx ].simtime ==
                entity->
                simulation_time( ) )
                continue;

            last_simtime = g_features.rage_backtrack.saved_data[ i ][ idx ].simtime;

            if ( should_overwrite_out )
                break;

            if ( !g_features.rage_backtrack.tick_valid ( time_to_ticks ( g_features.rage_backtrack.saved_data[i][idx].simtime ) ) )
                continue;

            if ( !g_features.rage_backtrack.saved_data[ i ][ idx ].ndata.restore ( entity ) )
                continue;

            memcpy ( cached_matrix, g_features.rage_backtrack.saved_data[ i ][ idx ].ndata.matrix, sizeof ( matrix3x4_t ) * 128 );

            auto record = g_features.rage_backtrack.saved_data[ i ][ idx ];
            float tmp_dmg;
            Vector tmp_pos;
            int tmp_hitbox;

            if ( calculate_target_backtrack_damage ( local, weapon, record, tmp_dmg, tmp_pos, tmp_hitbox ) )
            {
                if ( tmp_dmg > damage_out && tmp_dmg > g_options.ragebot_min_damage )
                {
                    if ( baim == rbot_baim_modes_t::none )
                    {
                        found_entity = true;
                        damage_out = tmp_dmg;
                        pos_out = tmp_pos;
                        hitbox_out = tmp_hitbox;
                        backtrackrecord_out = record;

                        if ( record.rdata.in_shot && tmp_hitbox == hitbox_head )
                            should_overwrite_out = true;
                    }
                    else
                    {
                        if ( is_baim_hitbox ( tmp_hitbox ) )
                        {
                            damage_out = tmp_dmg;
                            pos_out = tmp_pos;
                            hitbox_out = tmp_hitbox;
                            backtrackrecord_out = record;
                        }

                        //else
                        //{
                        //    found_entity = true;
                        //    local_can_hit_head.damage = tmp_dmg;
                        //    local_can_hit_head.pos = tmp_pos;
                        //    local_can_hit_head.hitbox = tmp_hitbox;
                        //    local_can_hit_head.bdata = record;
                        //    local_can_hit_head.can_hit = true;
                        //}
                    }
                }
            }

            g_features.rage_backtrack.saved_data[ i ][ idx ] = record;

            if ( should_overwrite_out )
                break;
        }

        //if ( !found_entity && local_can_hit_head.can_hit )
        //{
        //    found_entity = true;
        //    damage_out = local_can_hit_head.damage;
        //    pos_out = local_can_hit_head.pos;
        //    hitbox_out = local_can_hit_head.hitbox;
        //    backtrackrecord_out = local_can_hit_head.bdata;
        //}
    }
    else if ( backtrack_mode == backtrack_modes_t::best )
    {
        for ( auto idx = 0; idx < static_cast< int > ( g_features.rage_backtrack.saved_data[ i ].size( ) ); idx++ )
        {
            if ( last_simtime == g_features.rage_backtrack.saved_data[ i ][ idx ].simtime ||
                g_features.rage_backtrack.saved_data[ i ][ idx ].simtime == entity->simulation_time( ) )
                continue;

            last_simtime = g_features.rage_backtrack.saved_data[ i ][ idx ].simtime;

            if ( should_overwrite_out )
                break;

            auto inair = ! ( g_features.rage_backtrack.saved_data[ i ][ idx ].ndata.flags & fl_onground );

            if ( g_features.rage_backtrack.saved_data[ i ].front( ) != g_features.rage_backtrack.saved_data[ i ][ idx ] )
            {
                if ( !g_features.rage_backtrack.saved_data[ i ][ idx ].rdata.in_shot
                    && g_features.rage_backtrack.saved_data[ i ][ idx ].ndata.vec_velocity.length2d( ) < 200.f
                    && !inair
                    && g_features.rage_backtrack.saved_data[ i ][ idx ].rdata.server_send_ang.pitch > -30.f )
                    continue;
            }

            if ( !g_features.rage_backtrack.tick_valid ( time_to_ticks ( g_features.rage_backtrack.saved_data[i][idx].simtime ) ) )
                continue;

            if ( !g_features.rage_backtrack.saved_data[ i ][ idx ].ndata.restore ( entity ) )
                continue;

            memcpy ( cached_matrix, g_features.rage_backtrack.saved_data[ i ][ idx ].ndata.matrix, sizeof ( matrix3x4_t ) * 128 );

            auto record = g_features.rage_backtrack.saved_data[ i ][ idx ];
            float tmp_dmg;
            Vector tmp_pos;
            int tmp_hitbox;

            if ( calculate_target_backtrack_damage ( local, weapon, record, tmp_dmg, tmp_pos, tmp_hitbox ) )
            {
                if ( tmp_dmg > damage_out && tmp_dmg > g_options.ragebot_min_damage )
                {
                    if ( baim == rbot_baim_modes_t::none )
                    {
                        found_entity = true;
                        damage_out = tmp_dmg;
                        pos_out = tmp_pos;
                        hitbox_out = tmp_hitbox;
                        backtrackrecord_out = record;

                        if ( record.rdata.in_shot && tmp_hitbox == hitbox_head )
                            should_overwrite_out = true;
                    }
                    else
                    {
                        if ( is_baim_hitbox ( tmp_hitbox ) )
                        {
                            damage_out = tmp_dmg;
                            pos_out = tmp_pos;
                            hitbox_out = tmp_hitbox;
                            backtrackrecord_out = record;
                        }

                        //else
                        //{
                        //    found_entity = true;
                        //    local_can_hit_head.damage = tmp_dmg;
                        //    local_can_hit_head.pos = tmp_pos;
                        //    local_can_hit_head.hitbox = tmp_hitbox;
                        //    local_can_hit_head.bdata = record;
                        //    local_can_hit_head.can_hit = true;
                        //}
                    }
                }
            }

            g_features.rage_backtrack.saved_data[ i ][ idx ] = record;

            if ( should_overwrite_out )
                break;
        }
    }

    g_features.autowall.reset_overwrite( );

    org_data.restore ( entity );

    return found_entity && damage_out >= static_cast< float > ( g_options.ragebot_min_damage );
}
