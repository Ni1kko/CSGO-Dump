#include "misc.h"
#include "../options.h"
#include "../helpers/input.hpp"
#include "../helpers/math.hpp"
#include "../helpers/console.h"
#include "../options.h"
#include "features.h"

void c_misc::bhop ( usercmd_t* cmd )
{
    if ( !g_options.misc_bhop )
        return;

    if ( !g_local->is_alive( ) || ! ( cmd->buttons & IN_JUMP ) || g_local->move_type( ) == MOVETYPE_LADDER || g_local->move_type( ) == MOVETYPE_NOCLIP )
        return;

    if ( g_local->flags( ) & fl_onground )
        cmd->buttons |= IN_JUMP;
    else
        cmd->buttons &= ~IN_JUMP;

    //static auto jumped_last_tick = false;
    //static auto should_fake_jump = false;

    //if ( !jumped_last_tick && should_fake_jump )
    //{
    //    should_fake_jump = false;
    //    cmd->buttons |= IN_JUMP;
    //}
    //else if ( cmd->buttons & IN_JUMP )
    //{
    //    if ( g_local->flags() & fl_onground )
    //    {
    //        jumped_last_tick = true;
    //        should_fake_jump = true;
    //    }
    //    else
    //    {
    //        cmd->buttons &= ~IN_JUMP;
    //        jumped_last_tick = false;
    //    }
    //}
    //else
    //{
    //    jumped_last_tick = false;
    //    should_fake_jump = false;
    //}
}

void c_misc::auto_strafe ( usercmd_t* cmd, QAngle vangles )
{
    if ( !g_options.misc_autostrafer )
    {
        g_math.normalize_angles ( vangles );
        g_math.clamp_angles ( vangles );
        globals.old_forwardmove = cmd->forwardmove;
        globals.old_sidemove = cmd->sidemove;
        g_features.misc.movement.fix_movement ( cmd, vangles );
        return;
    }

    auto local = static_cast< C_BasePlayer* > ( g_local );

    if ( !local || !local->is_alive( ) || local->move_type( ) == MOVETYPE_LADDER )
        return;

    static auto left_right = false;

    bool on_ground = local->flags( ) & fl_onground;

    if ( !on_ground )
    {
        if ( local->vec_velocity( ).length2d( ) <= 50.0f )
        {
            if ( cmd->buttons & IN_FORWARD )
                cmd->forwardmove = 250.0f;
            else if ( cmd->buttons & IN_BACK )
                cmd->forwardmove = -250.0f;
            else if ( cmd->buttons & IN_MOVELEFT )
                cmd->forwardmove = 250.0f;
            else if ( cmd->buttons & IN_MOVERIGHT )
                cmd->forwardmove = 250.0f;
        }

        auto to_side = false;

        if ( cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT )
            to_side = true;

        static auto last_y = 0.f;

        if ( cmd->buttons & IN_MOVELEFT )
        {
            if ( last_y < 90.f )
                last_y += 180.f * interfaces::global_vars->frametime;

            vangles.yaw += last_y;
        }
        else if ( cmd->buttons & IN_MOVERIGHT )
        {
            if ( last_y < 90.f )
                last_y += 180.f * interfaces::global_vars->frametime;

            vangles.yaw -= last_y;
        }
        else if ( cmd->buttons & IN_BACK )
        {
            if ( last_y < 180.f )
                last_y += 180.f * interfaces::global_vars->frametime;

            vangles.yaw -= last_y;
        }
        else
            last_y = 0.f;

        auto yaw_change = 0.0f;

        if ( local->vec_velocity( ).length( ) > 50.f )
            yaw_change = 30.0f * fabsf ( 30.0f / local->vec_velocity( ).length( ) );

        C_BaseCombatWeapon* active_weapon = local->active_weapon( );

        if ( active_weapon && active_weapon->can_fire( ) && cmd->buttons & IN_ATTACK )
            yaw_change = 0.0f;

        if ( !on_ground )
        {
            if ( left_right || cmd->mousedx > 1 )
            {
                vangles.yaw += yaw_change;

                if ( !to_side )
                    cmd->sidemove = 350.f;
                else
                    cmd->sidemove = 350.f;
            }
            else if ( !left_right || cmd->mousedx < 1 )
            {
                vangles.yaw -= yaw_change;

                if ( !to_side )
                    cmd->sidemove = -350.f;
                else
                    cmd->sidemove = -350.f;
            }

            left_right = !left_right;
        }
    }

    g_math.normalize_angles ( vangles );
    g_math.clamp_angles ( vangles );
    globals.old_forwardmove = cmd->forwardmove;
    globals.old_sidemove = cmd->sidemove;
    g_features.misc.movement.fix_movement ( cmd, vangles );
}

void c_misc::nightmode ( )
{
    if ( !g_options.visuals_nightmode || g_unload )
    {
        for ( auto i = 1; i <= interfaces::entity_list->get_highest_entity_index( ); ++i )
        {
            auto entity = C_BaseEntity::get_entity_by_index ( i );

            if ( !entity || entity->is_dormant( ) )
                continue;

            if ( entity->get_client_class( )->class_id == class_id_t::CEnvTonemapController )
            {
                auto hdr = dynamic_cast< C_EnvTonemapController* > ( entity );
                hdr->use_custom_auto_exposure_min( ) = false;
                hdr->use_custom_auto_exposure_max( ) = false;
                hdr->custom_auto_exposure_min( ) = 1.f;
                hdr->custom_auto_exposure_max( ) = 1.f;
            }
        }

        return;
    }

    for ( auto i = 1; i <= interfaces::entity_list->get_highest_entity_index( ); ++i )
    {
        auto entity = C_BaseEntity::get_entity_by_index ( i );

        if ( !entity || entity->is_dormant( ) )
            continue;

        if ( entity->get_client_class( )->class_id == class_id_t::CEnvTonemapController )
        {
            auto hdr = dynamic_cast< C_EnvTonemapController* > ( entity );
            hdr->use_custom_auto_exposure_min( ) = true;
            hdr->use_custom_auto_exposure_max( ) = true;
            hdr->custom_auto_exposure_min( ) = 1.f * ( 1.f - g_options.visuals_nightmode_alpha ) + 0.01;
            hdr->custom_auto_exposure_max( ) = 1.f * ( 1.f - g_options.visuals_nightmode_alpha ) + 0.01;
        }
    }
}

void c_misc::disable_nightmode ( )
{
    for ( auto i = 1; i <= interfaces::entity_list->get_highest_entity_index( ); ++i )
    {
        auto entity = C_BaseEntity::get_entity_by_index ( i );

        if ( !entity || entity->is_dormant( ) )
            continue;

        if ( entity->get_client_class( )->class_id == class_id_t::CEnvTonemapController )
        {
            auto hdr = dynamic_cast< C_EnvTonemapController* > ( entity );
            hdr->use_custom_auto_exposure_min( ) = false;
            hdr->use_custom_auto_exposure_max( ) = false;
            hdr->custom_auto_exposure_min( ) = 1.f;
            hdr->custom_auto_exposure_max( ) = 1.f;
        }
    }
}

void c_misc::radar ( )
{
    if ( !g_options.visuals_radar )
        return;

    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !g_local || !entity->is_player( ) || entity == g_local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        entity->spotted( ) = true;
    }
}

void c_misc::clantag_changer ( )
{
    static auto did_run_last = false;

    if ( !g_options.misc_clangtag_changer )
    {
        if ( did_run_last )
        {
            did_run_last = false;
            g_utils.set_clantag ( xor_str ( "" ) );
        }

        return;
    }

    const std::string org_name = xor_str ( "gethad.es " );
    static std::string name = org_name;
    //static std::string current_str = name;
    static auto last_change = 0.f;

    if ( interfaces::global_vars->realtime - last_change > 0.1f + ( 0.9f - 0.9f * g_options.misc_clantag_changer_speed ) )
    {
        last_change = interfaces::global_vars->realtime;
        std::rotate ( name.begin( ), name.begin( ) + 1, name.end( ) );
        g_utils.set_clantag ( name.data( ) );
    }

    did_run_last = true;
}

void c_misc::airduck ( usercmd_t* cmd )
{
    if ( !g_options.misc_airduck || !g_local || !g_local->is_alive( ) )
        return;

    if ( ! ( g_local->flags( ) & fl_onground ) )
        cmd->buttons |= IN_DUCK;
}
