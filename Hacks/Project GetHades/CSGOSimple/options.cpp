#include "options.h"
#include <filesystem>
#include <utility>
#include "include/INIReader.h"
#include "helpers/debug_console.h"
#include "include/json.h"
#include "helpers/console.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

c_config g_options;
bool g_unload = false;
user_data_t g_user;

void c_config::setup_config ( )
{
    std::string appdata = getenv ( xor_str ( "APPDATA" ) );

    settings_folder_ = appdata + xor_str ( "\\hades" );

    if ( !fs::exists ( settings_folder_ ) )
        fs::create_directory ( settings_folder_ );

    //
    // esp
    //
    set ( xor_str ( "esp_master" ), &g_options.esp_master, true );
    set ( xor_str ( "esp_local_box" ), &g_options.esp_local_box, true );
    set ( xor_str ( "esp_enemy_box" ), &g_options.esp_enemy_box, true );
    set ( xor_str ( "esp_enemy_health" ), &g_options.esp_enemy_health, true );
    set ( xor_str ( "esp_enemy_armor" ), &g_options.esp_enemy_armor, false );
    set ( xor_str ( "esp_enemy_flag_scoped" ), &g_options.esp_enemy_flag_scoped, true );
    set ( xor_str ( "esp_enemy_flag_fake" ), &g_options.esp_enemy_flag_fake, true );
    set ( xor_str ( "esp_enemy_flag_reloading" ), &g_options.esp_enemy_flag_reloading, true );
    set ( xor_str ( "esp_enemy_name" ), &g_options.esp_enemy_name, true );
    set ( xor_str ( "esp_enemy_weapon" ), &g_options.esp_enemy_weapon, true );
    set ( xor_str ( "esp_team_box" ), &g_options.esp_team_box, true );
    // world
    set ( xor_str ( "esp_world_weapons" ), &g_options.esp_world_weapons, true );
    set ( xor_str ( "esp_weapon_box" ), &g_options.weapon_box, false );
    set ( xor_str ( "esp_world_weapons_ammo" ), &g_options.esp_world_weapons_ammo, true );
    //
    // chams
    //
    set ( xor_str ( "chams_master" ), &g_options.chams_master, true );
    set ( xor_str ( "chams_fake_desync" ), &g_options.chams_fake_desync, true );
    set ( xor_str ( "chams_fake_pos" ), &g_options.chams_fake_pos, true );
    set ( xor_str ( "color_chams_fake_desync" ), &g_options.color_chams_fake_desync, Color ( 255, 255, 255, 255 ) );
    set ( xor_str ( "color_chams_fake_pos" ), &g_options.color_chams_fake_pos, Color ( 0, 111, 255, 255 ) );
    set ( xor_str ( "color_chams_backtrack" ), &g_options.color_chams_backtrack, Color ( 50, 50, 50, 255 ) );

    set ( xor_str ( "chams_local" ), &g_options.chams_local, false );
    set ( xor_str ( "chams_fake_local" ), &g_options.chams_fake_local, true );
    set ( xor_str ( "chams_enemy" ), &g_options.chams_enemy, true );
    set ( xor_str ( "chams_team" ), &g_options.chams_team, false );
    set ( xor_str ( "chams_type_local" ), &g_options.chams_type_local, 0 );
    set ( xor_str ( "chams_type_enemy" ), &g_options.chams_type_enemy, 1 );
    set ( xor_str ( "chams_type_team" ), &g_options.chams_type_team, 0 );
    set ( xor_str ( "chams_color_local" ), &g_options.chams_color_local, Color ( 0, 0, 255, 255 ) );
    set ( xor_str ( "chams_color_enemy" ), &g_options.chams_color_enemy, Color ( 255, 111, 0, 255 ) );
    set ( xor_str ( "chams_enemy_backtrack" ), &g_options.chams_enemy_backtrack, true );
    set ( xor_str ( "chams_color_team" ), &g_options.chams_color_team, Color ( 0, 255, 0 ) );
    set ( xor_str ( "chams_color_xqz_local" ), &g_options.chams_color_xqz_local, Color::white );
    set ( xor_str ( "chams_color_xqz_enemy" ), &g_options.chams_color_xqz_enemy, Color ( 255, 255, 255 ) );
    set ( xor_str ( "chams_color_xqz_team" ), &g_options.chams_color_xqz_team, Color::white );
    //
    // visuals
    //
    set ( xor_str ( "visuals_local_lc" ), &g_options.visuals_local_lc, true );
    set ( xor_str ( "visuals_local_desync" ), &g_options.visuals_local_desync, true );
    //set ( xor_str ( "visuals_scope_overlay_mode" ), &g_options.visuals_scope_overlay_mode, 2 );
    set ( xor_str ( "visuals_remove_scope_zoom" ), &g_options.visuals_remove_scope_zoom, true );
    set ( xor_str ( "visuals_fov" ), &g_options.visuals_fov, 90.f );
    set ( xor_str ( "visuals_scope_transparency" ), &g_options.visuals_scope_transparency, 0.f );
    set ( xor_str ( "visuals_nightmode" ), &g_options.visuals_nightmode, true );
    set ( xor_str ( "visuals_nightmode_alpha" ), &g_options.visuals_nightmode_alpha, 0.6f );
    set ( xor_str ( "visuals_radar" ), &g_options.visuals_radar, true );
    set ( xor_str ( "visuals_footstep_esp" ), &g_options.visuals_footstep_esp, true );
    set ( xor_str ( "visuals_footstep_esp_color" ), &g_options.visuals_footstep_esp_color, Color ( 255, 0, 0, 255 ) );
    set ( xor_str ( "visuals_beam_color_bulle" ), &g_options.visuals_beams_bullet_tracer, Color ( 153, 0, 153, 255 ) );
    set ( xor_str ( "visuals_bullet_tracers" ), &g_options.visuals_bullet_tracers, true );
    set ( xor_str ( "visuals_nosmoke_mode" ), &g_options.visuals_nosmoke_mode, 0 );
    set ( xor_str ( "visuals_crosshair" ), &g_options.visuals_crosshair, true );
    set ( xor_str ( "visuals_asuswalls" ), &g_options.visuals_asuswalls, false );
    set ( xor_str ( "visuals_damage_indicator" ), &g_options.esp_world_damage_indicator, false );
    //set ( xor_str ( "visuals_thirdperson" ), &g_options.visuals_thirdperson, false );
    set ( xor_str ( "visuals_thirdperson_key" ), &g_options.visuals_thirdperson_key, -1 );
    set ( xor_str ( "visuals_thirdperson_distance" ), &g_options.visuals_thirdperson_distance, 150.f );
    set ( xor_str ( "visuals_noflash" ), &g_options.visuals_noflash, true );
    set ( xor_str ( "visual_beam_ring_size" ), &g_options.visuals_beam_ring_size, 500.f );
    set ( xor_str ( "visual_beam_speed" ), &g_options.visuals_beam_speed, 1.f );
    set ( xor_str ( "visuals_offscreen" ), &g_options.esp_offscreen, true );
    set ( xor_str ( "visuals_remove_viewpunch" ), &g_options.visuals_remove_viewpunch, false );

    set ( xor_str ( "visuals_offscreen_enemy" ), &g_options.esp_offscreen_enemyonly, true );
    set ( xor_str ( "visuals_offscreen_color" ), &g_options.visuals_offscreen_color, Color ( 255, 0, 0, 255 ) );
    set ( xor_str ( "visuals_offscreen_range" ), &g_options.visuals_invisible_players_range, 0.5f );

    set ( xor_str ( "visuals_manual_aa_indicator" ), &g_options.visuals_manual_aa_indicator, false );
    //
    // ragebot
    //
    set ( xor_str ( "ragebot_master" ), &g_options.ragebot_master, true );
    set ( xor_str ( "ragebot_resolver" ), &g_options.ragebot_resolver, false );
    set ( xor_str ( "ragebot_autoshoot" ), &g_options.ragebot_autoshoot, true );
    //set ( xor_str ( "ragebot_resolver_mode" ), &g_options.ragebot_resolver_mode, 0 );
    set ( xor_str ( "ragebot_resolver_overwrite" ), &g_options.ragebot_resolver_overwrite, false );
    set ( xor_str ( "ragebot_resolver_baim" ), &g_options.ragebot_resolver_baim, true );
    set ( xor_str ( "ragebot_autostop" ), &g_options.ragebot_autostop, false );
    set ( xor_str ( "ragebot_delay_mode" ), &g_options.ragebot_delay_mode, 0 );
    set ( xor_str ( "ragebot_backtrack_mode" ), &g_options.ragebot_backtrack_mode, 0 );
    // main settings
    set ( xor_str ( "ragebot_min_damage" ), &g_options.ragebot_min_damage, 12 );
    set ( xor_str ( "ragebot_hitchance" ), &g_options.ragebot_hitchance, .5f );
    set ( xor_str ( "ragebot_hitbox_head" ), &g_options.ragebot_hitbox_head, true );
    set ( xor_str ( "ragebot_hitbox_neck" ), &g_options.ragebot_hitbox_neck, true );
    set ( xor_str ( "ragebot_hitbox_stomach" ), &g_options.ragebot_hitbox_stomach, true );
    set ( xor_str ( "ragebot_hitbox_chest" ), &g_options.ragebot_hitbox_chest, true );
    set ( xor_str ( "ragebot_hitbox_feet" ), &g_options.ragebot_hitbox_feet, true );
    set ( xor_str ( "ragebot_hitbox_arms" ), &g_options.ragebot_hitbox_arms, false );
    set ( xor_str ( "ragebot_hitbox_head_scale" ), &g_options.ragebot_hitbox_head_scale, 0.6f );
    set ( xor_str ( "ragebot_hitbox_body_scale" ), &g_options.ragebot_hitbox_body_scale, 0.67f );

    set ( xor_str ( "ragebot_hitbox_head_pointscale" ), &g_options.ragebot_hitbox_head_pointscale, true );
    set ( xor_str ( "ragebot_hitbox_neck_pointscale" ), &g_options.ragebot_hitbox_neck_pointscale, false );
    set ( xor_str ( "ragebot_hitbox_stomach_pointscale" ), &g_options.ragebot_hitbox_stomach_pointscale, true );
    set ( xor_str ( "ragebot_hitbox_chest_pointscale" ), &g_options.ragebot_hitbox_chest_pointscale, true );
    set ( xor_str ( "ragebot_hitbox_feet_pointscale" ), &g_options.ragebot_hitbox_feet_pointscale, false );
    set ( xor_str ( "ragebot_hitbox_arms_pointscale" ), &g_options.ragebot_hitbox_arms_pointscale, false );
    set ( xor_str ( "ragebot_autostop_mode" ), &g_options.ragebot_autostop_mode, 1 );
    set ( xor_str ( "ragebot_shooting_mode" ), &g_options.ragebot_shooting_mode, 3 );
    set ( xor_str ( "ragebot_baim_mode" ), &g_options.ragebot_baim_mode, 2 );
    set ( xor_str ( "ragebot_baim_after_shots" ), &g_options.ragebot_baim_after_shots, 1 );
    set ( xor_str ( "ragebot_baim_when_lethal" ), &g_options.ragebot_baim_when_lethal, true );
    // anti-aim
    set ( xor_str ( "ragebot_antiaim_disable_on_knife" ), &g_options.ragebot_antiaim_disable_on_knife, false );
    set ( xor_str ( "ragebot_antiaim_enable" ), &g_options.ragebot_antiaim_enable, true );
    set ( xor_str ( "ragebot_antiaim_yaw_modes" ), &g_options.ragebot_antiaim_yaw_mode, xor_str ( "none" ) );
    set ( xor_str ( "ragebot_antiaim_yaw_add" ), &g_options.ragebot_antiaim_yaw_add, 0.f );
    set ( xor_str ( "ragebot_antiaim_yaw_jitter_range" ), &g_options.ragebot_antiaim_yaw_jitter_range, 35.f );
    set ( xor_str ( "ragebot_antiaim_yaw_switch_range" ), &g_options.ragebot_antiaim_yaw_switch_range, 35.f );
    set ( xor_str ( "ragebot_antiaim_yaw_base_modes" ), &g_options.ragebot_antiaim_yaw_base_mode, xor_str ( "none" ) );
    set ( xor_str ( "ragebot_antiaim_pitch_mode_s" ), &g_options.ragebot_antiaim_pitch_mode, xor_str ( "none" ) );
    set ( xor_str ( "ragebot_antiaim_desync_modes" ), &g_options.ragebot_antiaim_desync_mode, xor_str ( "none" ) );
    set ( xor_str ( "ragebot_antiaim_fakeduck_key" ), &g_options.ragebot_antiaim_fakeduck_key, 0 );
    set ( xor_str ( "ragebot_antiaim_manual_right_key" ), &g_options.ragebot_antiaim_manual_right_key, 0x0 );
    set ( xor_str ( "ragebot_antiaim_manual_left_key" ), &g_options.ragebot_antiaim_manual_left_key, 0x0 );
    set ( xor_str ( "ragebot_antiaim_manual_back_key" ), &g_options.ragebot_antiaim_manual_back_key, 0x0 );
    //set ( xor_str ( "ragebot_accuracy_slowwalk" ), &g_options.ragebot_accuracy_slowwalk, false );
    set ( xor_str ( "ragebot_accuracy_slowwalk_key" ), &g_options.ragebot_accuracy_slowwalk_key, 0 );
    set ( xor_str ( "ragebot_accuracy_slowwalk_speed" ), &g_options.ragebot_accuracy_slowwalk_speed, 1.f );
    // fake-lag
    set ( xor_str ( "ragebot_fakelag_enable" ), &g_options.ragebot_fakelag_enable, true );
    set ( xor_str ( "ragebot_fakelag_while_unduck" ), &g_options.ragebot_fakelag_while_unduck, false );
    /*
    set ( xor_str ( "ragebot_fakelag_standing" ), &g_options.ragebot_fakelag_standing, 7 );
    set ( xor_str ( "ragebot_fakelag_moving" ), &g_options.ragebot_fakelag_moving, 15 );
    set ( xor_str ( "ragebot_fakelag_slowwalking" ), &g_options.ragebot_fakelag_slowwalking, 7 );
    set ( xor_str ( "ragebot_fakelag_in_air" ), &g_options.ragebot_fakelag_in_air, 15 );
    set ( xor_str ( "ragebot_fakelag_standing" ), &g_options.ragebot_fakelag_mode_standing, 0 );
    set ( xor_str ( "ragebot_fakelag_moving" ), &g_options.ragebot_fakelag_mode_moving, 0 );
    set ( xor_str ( "ragebot_fakelag_slowwalking" ), &g_options.ragebot_fakelag_mode_slowwalking, 0 );
    set ( xor_str ( "ragebot_fakelag_in_air" ), &g_options.ragebot_fakelag_mode_in_air, 0 );
    */
    set ( xor_str ( "ragebot_fakelag_standing_ticks" ), &g_options.ragebot_fakelag_standing_ticks, 7 );
    set ( xor_str ( "ragebot_fakelag_moving_ticks" ), &g_options.ragebot_fakelag_moving_ticks, 15 );
    set ( xor_str ( "ragebot_fakelag_slowwalk_ticks" ), &g_options.ragebot_fakelag_slowwalk_ticks, 7 );
    set ( xor_str ( "ragebot_fakelag_while_shooting" ), &g_options.ragebot_fakelag_while_shooting, true );
    //
    // misc
    //
    set ( xor_str ( "misc_bhop" ), &g_options.misc_bhop, true );
    set ( xor_str ( "misc_autostrafer" ), &g_options.misc_autostrafer, true );
    set ( xor_str ( "misc_eventlog" ), &g_options.misc_eventlog, true );
    set ( xor_str ( "misc_clangtag_changer" ), &g_options.misc_clangtag_changer, false );
    set ( xor_str ( "misc_clantag_changer_speed" ), &g_options.misc_clantag_changer_speed, 0.5f );

    set ( xor_str ( "misc_viewmodel_x" ), &g_options.misc_viewmodel_x, 0.f );
    set ( xor_str ( "misc_viewmodel_y" ), &g_options.misc_viewmodel_y, 0.f );
    set ( xor_str ( "misc_viewmodel_z" ), &g_options.misc_viewmodel_z, 0.f );
    set ( xor_str ( "misc_airduck" ), &g_options.misc_airduck, false );
    set ( xor_str ( "misc_slidewalk" ), &g_options.misc_slidewalk, false );

    set ( xor_str ( "menu_color" ), &g_options.menu_color1, Color ( 255, 0, 0, 255 ) );
    set ( xor_str ( "menu_color_selected" ), &g_options.menu_color_selected, Color ( 255, 0, 0, 255 ) );
}

void c_config::set ( const std::string& name, bool* b, const bool start_val, const bool save )
{
    if ( !b )
        throw std::exception ( );

    *b = start_val;
    config_data[ name ] = config_saver_struct_t { b, config_types_t::t_bool, save };
}

void c_config::set ( const std::string& name, int* i, const int start_val, const bool save )
{
    if ( !i )
        throw std::exception ( );

    *i = start_val;
    config_data[ name ] = config_saver_struct_t { i, config_types_t::t_int, save };
}

void c_config::set ( const std::string& name, float* f, const float start_val, const bool save )
{
    if ( !f )
        throw std::exception ( );

    *f = start_val;
    config_data[ name ] = config_saver_struct_t { f, config_types_t::t_float, save };
}

void c_config::set ( const std::string& name, Color* clr, const Color start_val, const bool save )
{
    if ( !clr )
        throw std::exception ( );

    *clr = start_val;
    config_data[ name ] = config_saver_struct_t { clr, config_types_t::t_color, save };
}

void c_config::set ( const std::string& name, std::string* text, std::string start_val, bool save )
{
    if ( !text )
        throw std::exception ( );

    *text = std::move ( start_val );

    config_data[ name ] = config_saver_struct_t { text, config_types_t::t_string, save };
}

void c_config::save_config ( const std::string& name )
{
    debug_console::print ( xor_str ( "saving config " ) + name );
    std::string save_path = settings_folder_ + xor_str ( "\\" ) + name;

    for ( const auto& data : config_data )
    {
        if ( !data.second.save )
            continue;

        switch ( data.second.type )
        {
        case config_types_t::t_bool:
            {
                WritePrivateProfileStringA ( xor_str ( "HADES" ), data.first.data ( ),
                                             std::to_string ( * static_cast< bool* > ( data.second.ptr ) ).data ( ),
                                             save_path.data ( ) );
            }
            break;

        case config_types_t::t_int:
            {
                WritePrivateProfileStringA ( xor_str ( "HADES" ), data.first.data ( ), std::to_string ( *static_cast< int* > ( data.second.ptr ) ).data ( ),
                                             save_path.data ( ) );
            }
            break;

        case config_types_t::t_float:
            {
                WritePrivateProfileStringA ( xor_str ( "HADES" ), data.first.data ( ),
                                             std::to_string ( *static_cast< float* > ( data.second.ptr ) ).data ( ),
                                             save_path.data ( ) );
            }
            break;

        case config_types_t::t_color:
            {
                json j;
                Color tmp = *static_cast< Color* > ( data.second.ptr );
                j[ xor_str ( "r" ) ] = tmp.r ( );
                j[ xor_str ( "g" ) ] = tmp.g ( );
                j[ xor_str ( "b" ) ] = tmp.b ( );
                j[ xor_str ( "a" ) ] = tmp.a ( );
                WritePrivateProfileStringA ( xor_str ( "HADES" ), data.first.data ( ), std::string ( j.dump ( ) ).data ( ), save_path.data ( ) );
            }
            break;

        case config_types_t::t_string:
            {
                WritePrivateProfileStringA ( xor_str ( "HADES" ), data.first.data ( ),
                                             reinterpret_cast<std::string*> ( data.second.ptr )->data(  ),
                                             save_path.data ( ) );
            }
            break;

        default:
            break;
        }
    }

    debug_console::print ( xor_str ( "config saved" ) );
}

void c_config::load_config ( const std::string& name )
{
    debug_console::print ( xor_str ( "loading config " ) + name );
    const std::string load_path = settings_folder_ + "\\" + name;

    if ( !fs::exists ( load_path ) )
        return;

    INIReader reader ( load_path );

    if ( reader.ParseError ( ) < 0 )
    {
        debug_console::print ( xor_str ( "error parsing config file" ) );
        return;
    }

    for ( auto& data : config_data )
    {
        if ( !data.second.save )
            continue;

        switch ( data.second.type )
        {
        case config_types_t::t_bool:
            {
                *static_cast< bool* > ( data.second.ptr ) = reader.GetBoolean ( xor_str ( "HADES" ), data.first, *static_cast< bool* > ( data.second.ptr ) );
            }
            break;

        case config_types_t::t_int:
            {
                *static_cast< int* > ( data.second.ptr ) = reader.GetInteger ( xor_str ( "HADES" ), data.first, *static_cast< int* > ( data.second.ptr ) );
            }
            break;

        case config_types_t::t_float:
            {
                *static_cast< float* > ( data.second.ptr ) = reader.GetReal ( xor_str ( "HADES" ), data.first, *static_cast< float* > ( data.second.ptr ) );
            }
            break;

        case config_types_t::t_color:
            {
                auto backup_clr = *static_cast< Color* > ( data.second.ptr );
                json tmp_j1;
                tmp_j1[ xor_str ( "r" ) ] = backup_clr.r ( );
                tmp_j1[ xor_str ( "g" ) ] = backup_clr.g ( );
                tmp_j1[ xor_str ( "b" ) ] = backup_clr.b ( );
                tmp_j1[ xor_str ( "a" ) ] = backup_clr.a ( );
                auto str = reader.Get ( xor_str ( "HADES" ), data.first, tmp_j1.dump ( ) );
                auto j = json::parse ( str );
                *static_cast< Color* > ( data.second.ptr ) = Color ( j[ xor_str ( "r" ) ].get< int > ( ), j[ xor_str ( "g" ) ].get< int > ( ),
                        j[ xor_str ( "b" ) ].get< int > ( ), j[ xor_str ( "a" ) ].get< int > ( ) );
            }
            break;

        case config_types_t::t_string:
            {
                *reinterpret_cast<std::string*> ( data.second.ptr ) = reader.Get ( xor_str ( "HADES" ), data.first, xor_str ( "none" ) );
            }
            break;

        default:
            break;
        }
    }

    debug_console::print ( xor_str ( "config loaded" ) );
}

config_saver_struct_t c_config::get_option_data ( const std::string name )
{
    return this->config_data.at ( name );
}
