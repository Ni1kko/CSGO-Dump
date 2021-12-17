#include "debug_overlay.h"
#include <utility>
#include "../renderer/vgui_render.h"
#include "../helpers/math.hpp"
#include "visuals.hpp"
#include "features.h"
#include "../options.h"

void c_debug_overlay::on_add_to_draw_list ( )
{
#ifdef _DEBUG

    if ( !g_local || !g_local->is_alive( ) )
        return;

    int w, h;
    interfaces::engine_client->get_screen_size ( w, h );
    const auto cx = w / 2;

    auto height = 0;
    const auto add_text = [&] ( std::string text )
    {
        g_render.text ( g_fonts.normal, std::move ( text ), cx, 72 + height, Color ( 255, 255, 255 ) );
        height += 12;
    };

    auto state = g_local->get_base_player_anim_state( );

    if ( !state )
        return;

    auto goal_feet_yaw = state->m_flGoalFeetYaw;
    g_math.normalize_yaw ( goal_feet_yaw );
    /*
    std::string lby = xor_str ( "lowerbody: " ) + std::to_string ( g_local->lower_body_yaw_target() );
    std::string goal_feet_yaw = xor_str ( "goalf_yaw: " ) + std::to_string ( m_goal_feet_yaw );
    std::string feet_yaw = xor_str ( "feet_yaw: " ) + std::to_string ( state->m_current_feet_yaw );
    std::string real_yaw = xor_str ( "real_yaw_client: " ) + std::to_string ( globals.real_yaw );
    std::string fake_yaw = xor_str ( "fake_yaw_client: " ) + std::to_string ( globals.real_yaw + globals.fake_yaw_add );
    std::string choked_cmd = xor_str ( "choked_cmds: " ) + std::to_string ( interfaces::client_state->chokedcommands );
    std::string pitch = xor_str ( "pitch: " ) + std::to_string ( g_local->eye_angles().pitch );
    std::string max_desync_yaw = xor_str ( "max_desync: " ) + std::to_string ( g_local->get_max_desync_delta() );
    */

    add_text ( "m_goal_feet_yaw: " + std::to_string ( state->m_flGoalFeetYaw ) );
    auto clamped_goal_feet_yaw = state->m_flGoalFeetYaw;
    g_math.normalize_yaw ( clamped_goal_feet_yaw );
    add_text ( "globals.real_yaw: " + std::to_string ( globals.real_yaw ) );

    auto current_yaw = globals.real_yaw;

    /*
    if ( !g_options.visuals_thirdperson_toggle )
    {
        QAngle tmp;
        interfaces::engine_client->get_viewangles ( &tmp );
        cu
    rrent_yaw = tmp.yaw;
    }
    */

    add_text ( "goal_feet_eye_delta: " + std::to_string (
        *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( state ) + 0x78 ) - clamped_goal_feet_yaw ) );
    add_text ( "m_current_feet_yaw: " + std::to_string ( state->m_flCurrentFeetYaw ) );
    add_text ( "ClampedYaw: " + std::to_string ( * reinterpret_cast< float* > ( reinterpret_cast< int > ( state ) + 0x78 ) ) );
    add_text ( "lby broken: " + std::string ( globals.lby_broken ? "true" : "false" ) );
    add_text ( "max rotation: " + std::to_string ( g_local->get_max_desync_delta( ) ) );
    add_text ( "fake_yaw: " + std::to_string ( globals.fake_yaw_ ) );

    auto poses = g_local->pose_parameter( );

    for ( auto idx = 0; idx < size_t( poses.size( ) ); idx++ )
    {
        const auto pose = poses[ idx ];
        //
        add_text ( "pose_param[" + std::to_string ( idx ) + "]: " + std::to_string ( pose ) );
    }

#endif // _DEBUG
}

void c_visuals::Player::draw_debug ( )
{
    if ( ctx.pl->is_dormant( ) )
        return;

#ifdef _DEBUG
    auto height = 0;
    const auto add_text = [&] ( std::string text )
    {
        g_render.text ( g_fonts.normal, text, ctx.bbox.right + 4 + 40, ctx.bbox.top + height, Color ( 255, 255, 255 ) );
        height += 12;
    };

    const auto i = ctx.pl->ent_index( );

    add_text ( "shots: " + std::to_string ( g_features.resolver.rdata.at ( i ).shots ) );
    //add_text ( "shots_overall_no_resolver_overwrite: " + std::to_string ( g_features.resolver.rdata.at ( i ).overall_shots_no_resolver ) );
    //add_text ( "overall_shots: " + std::to_string ( g_features.resolver.rdata.at ( i ).overall_shots ) );
    //add_text ( "yaw: " + std::to_string ( ctx.pl->eye_angles().yaw ) );
    //add_text ( "goalfeetyaw: " + std::to_string ( ctx.pl->get_base_player_anim_state()->m_goal_feet_yaw ) );
    //add_text ( "resolved: " + std::string ( g_resolver.resolver_data[i].resolved ? "true" : "false" ) );
    //add_text ( "desync: " + std::string ( g_resolver.resolver_data[i].desyncing ? "true" : "false" ) );
    //add_text ( "jittering: " + std::string ( g_resolver.resolver_data[i].jittering ? "true" : "false" ) );
    //add_text ( "eye_lby_delta: " + std::to_string ( g_features.resolver.resolver_data[i].eye_lby_delta ) );
    add_text ( "desync: " + std::to_string ( g_features.resolver.rdata.at ( i ).desyncing ) );
    add_text ( "resolved: " + std::to_string ( g_features.resolver.rdata.at ( i ).desyncing ) );
    //add_text ( "choked: " + std::to_string ( g_resolver.resolver_data[i].choked_packets ) );
    //add_text ( "in_slowwalk: " + std::string ( g_resolver.resolver_data[i].in_slowwalk ? "true" : "false" ) );
    //add_text ( "shots_overall: " + std::to_string ( g_resolver.resolver_data[i].shots_overall ) );
    //add_text ( "velocity: " + std::to_string ( ctx.pl->vec_velocity().length2d() ) );
    //add_text ( "resolved: " + std::string ( g_resolver.resolver_data[i].resolved ? "true" : "false" ) );
    //std::string direction = "";

    /*std::string mode_name;

    switch ( g_features.resolver.resolver_data[i].current_mode )
    {
    case resolver_modes_t::plus:
        mode_name = "plus";
        break;

    case resolver_modes_t::minus:
        mode_name = "minus";
        break;

    case resolver_modes_t::plus_120:
        mode_name = "plus_120";
        break;

    case resolver_modes_t::minus_120:
        mode_name = "minus_120";
        break;

    case resolver_modes_t::none:
        mode_name = "none";
        break;

    case resolver_modes_t::shot:
        mode_name = "shot";
        break;

    case resolver_modes_t::max:
        mode_name = "max";
        break;

    default:
        mode_name = "invalid";
        break;
    }

    add_text ( mode_name );
    add_text ( std::to_string ( g_features.resolver.resolver_data[i]._979_times_last_second ) );

    std::string method_name;

    switch ( g_features.resolver.resolver_data[i].current_method )
    {
    case resolver_method_t::wall_thickness:
        method_name = "wall_thickness";
        break;

    case resolver_method_t::delta:
        method_name = "delta";
        break;

    case resolver_method_t::brutforce:
        method_name = "brutforce";
        break;

    case resolver_method_t::air:
        method_name = "air";
        break;

    case resolver_method_t::shot:
        method_name = "shot";
        break;

    case resolver_method_t::at_target:
        method_name = "at_target";
        break;

    case resolver_method_t::old_headshot_spot:
        method_name = "old_headshot_spot";
        break;

    case resolver_method_t::hitable_spot:
        method_name = "hitable_spot";
        break;

    case resolver_method_t::max:
        method_name = "max";
        break;

    default:
        method_name = "";
        break;
    }

    add_text ( method_name );
    add_text ( g_features.resolver.resolver_data[i].jittering ? "jitter real" : "static real" );
    add_text ( interfaces::global_vars->curtime - g_features.resolver.resolver_data[i].last_tickshift_time < 0.6f ? "tick shifting" : "legit | desync" );*/

    const auto pos = ctx.pl->vec_origin( );

    Vector tmp;

    if ( g_math.world_to_screen ( pos, tmp ) )
    {
        Vector draw_tmp;
        const Vector resolved_pos (
            50.f * cos ( deg2rad ( ctx.pl->eye_angles ( ).yaw + g_features.resolver.rdata.at ( ctx.pl->ent_index( ) ).desync_add ) ) + pos.x,
            50.f * sin ( deg2rad ( ctx.pl->eye_angles ( ).yaw + g_features.resolver.rdata.at ( ctx.pl->ent_index( ) ).desync_add ) ) + pos.y, pos.z );

        if ( g_math.world_to_screen ( resolved_pos, draw_tmp ) )
        {
            g_render.line ( tmp.x, tmp.y, draw_tmp.x, draw_tmp.y, Color ( 255, 0, 0, 255 ), 1.f );
            g_render.text ( g_fonts.menu12, "resolved", draw_tmp.x, draw_tmp.y, Color ( 255, 0, 0, 255 ) );
        }

        const Vector fake_pos ( 50.f * cos ( deg2rad ( g_features.resolver.rdata.at ( ctx.pl->ent_index( ) ).server_send_ang.yaw ) ) + pos.x,
                                50.f * sin ( deg2rad ( g_features.resolver.rdata.at ( ctx.pl->ent_index( ) ).server_send_ang.yaw ) ) + pos.y, pos.z );

        if ( g_math.world_to_screen ( fake_pos, draw_tmp ) )
        {
            g_render.line ( tmp.x, tmp.y, draw_tmp.x, draw_tmp.y, Color ( 0, 255, 0, 255 ), 1.f );
            g_render.text ( g_fonts.menu12, "real", draw_tmp.x, draw_tmp.y, Color ( 0, 255, 0, 255 ) );
        }
    }

    add_text ( g_features.resolver.rdata.at ( i ).in_shot ? "in_shot" : "no" );

    //switch ( g_resolver.resolver_data[i].last_979_direction )
    //{
    //case anim_direction::none:
    //    direction = "none";
    //    break;

    //case anim_direction::unknown:
    //    direction = "unknown";
    //    break;

    //case anim_direction::negative:
    //    direction = "negative";
    //    break;

    //case anim_direction::positive:
    //    direction = "positive";
    //    break;
    //}

    //add_text ( "979_direction: " + direction );

    //std::string mode = "";

    //switch ( g_resolver.resolver_data[i].mode )
    //{
    //case modes::air:
    //    mode = "air";
    //    break;

    //case modes::moving:
    //    mode = "moving";
    //    break;

    //case modes::slowwalk:
    //    mode = "slowwalk";
    //    break;

    //case modes::standing:
    //    mode = "standing";
    //    break;
    //}

    //add_text ( "mode: " + mode );
    //add_text ( "jittering: " + std::string ( g_resolver.resolver_data[i].is_jittering ? "true" : "false" ) );*/

#endif // _DEBUG
}
