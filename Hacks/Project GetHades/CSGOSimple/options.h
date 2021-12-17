#pragma once
#include <string>
#include <unordered_map>
#include "valve_sdk/misc/Color.hpp"

constexpr auto ragebot_weapon_number = 6;
#define cfg(type, var) type var;
#define cfg_local(type, var, value) type var = value;

//class Vector;

enum class config_types_t : int
{
    t_none,
    t_bool,
    t_int,
    t_float,
    t_vector,
    t_qangle,
    t_rbot_configs,
    t_color,
    t_string
};

struct config_saver_struct_t
{
    void* ptr = nullptr;
    config_types_t type = config_types_t::t_none;
    bool save = true;
};

enum class weapon_settings_ids_t : int
{
    pistol,
    deagle_r8,
    smg_lmg,
    autosniper,
    sniper,
    rifle
};

struct user_data_t
{
    std::string username = "";
    #ifdef _DEBUG
    bool is_allowed = true;
    #else
    bool is_allowed = false;
    #endif // _DEBUG
};

class c_config
{
public:
    c_config()
    {
        setup_config();
    }

    //
    // esp
    //
    cfg ( bool, esp_master )
    cfg ( bool, esp_local_box )
    cfg ( bool, esp_enemy_box )
    cfg ( bool, esp_enemy_health )
    cfg ( bool, esp_enemy_armor )
    cfg ( bool, esp_enemy_weapon )
    cfg ( bool, esp_offscreen )
    cfg ( bool, esp_offscreen_enemyonly )


    cfg ( bool, esp_enemy_flag_scoped )
    cfg ( bool, esp_enemy_flag_fake )
    cfg ( bool, esp_enemy_flag_reloading )
    cfg ( bool, esp_enemy_name )
    cfg ( bool, esp_team_box )
    // world
    cfg ( bool, esp_world_weapons )
    cfg ( bool, weapon_box )
    cfg ( bool, esp_world_weapons_ammo )


    cfg ( bool, esp_world_damage_indicator )
    //
    // chams
    //
    cfg ( bool, chams_master )
    cfg ( int, chams_weapon_drop_type )
    cfg ( bool, chams_weapon_drop_static )
    cfg ( bool, chams_weapon_drop_pulsing )
    cfg ( Color, chams_weapon_dropped_color );
    cfg ( bool, chams_local )
    cfg ( bool, chams_fake_local )
    cfg ( bool, chams_enemy )
    cfg ( bool, chams_team )
    cfg ( int, chams_type_local )
    cfg ( int, chams_type_enemy )
    cfg ( bool, chams_enemy_backtrack )
    cfg ( int, chams_type_team )
    cfg ( bool, chams_fake_desync )
    cfg ( bool, chams_fake_pos )
    cfg ( Color, color_chams_fake_desync );
    cfg ( Color, color_chams_fake_pos );

    cfg ( Color, chams_color_local );
    cfg ( Color, chams_color_enemy );
    cfg ( Color, chams_color_team );
    cfg ( Color, chams_color_xqz_local );
    cfg ( Color, chams_color_xqz_enemy );
    cfg ( Color, chams_color_xqz_team );
    cfg ( Color, menu_color1 );
    cfg ( Color, menu_color_selected );
    cfg ( Color, color_chams_backtrack );

    //cfg ( bool, chams_waving_local );
    //
    // visuals
    //
    //cfg ( bool, visuals_master );
    cfg ( bool, visuals_local_lc )
    cfg ( bool, visuals_local_desync )

    //cfg ( bool, visuals_scope_overlay_mode );
    cfg ( bool, visuals_remove_scope_zoom )
    cfg ( float, visuals_fov )
    cfg ( float, visuals_beam_ring_size )
    cfg ( float, visuals_beam_speed )

    cfg ( float, visuals_scope_transparency )
    cfg ( bool, visuals_nightmode )
    cfg ( bool, visuals_asuswalls )
    cfg ( float, visuals_nightmode_alpha )
    cfg ( bool, visuals_radar )
    cfg ( bool, visuals_crosshair )
    cfg ( bool, visuals_footstep_esp )
    cfg ( Color, visuals_footstep_esp_color );
    cfg ( Color, visuals_beams_bullet_tracer );
    cfg ( bool, visuals_bullet_tracers )
    cfg ( int, visuals_nosmoke_mode )
    // cfg ( bool, visuals_thirdperson );
    cfg ( int, visuals_thirdperson_key )
    cfg_local ( bool, visuals_thirdperson_toggle, true );
    cfg ( float, visuals_thirdperson_distance )
    cfg ( float, visuals_invisible_players_range )
    cfg ( Color, visuals_offscreen_color );
    cfg ( bool, visuals_noflash )
    cfg ( bool, visuals_remove_viewpunch )

    cfg ( bool, visuals_manual_aa_indicator )

    //
    // ragebot
    //
    cfg ( bool, ragebot_master )
    cfg ( bool, ragebot_resolver )
    cfg ( bool, ragebot_autoshoot )
    cfg ( bool, ragebot_resolver_overwrite )
    cfg ( bool, ragebot_resolver_baim )
    cfg ( int, ragebot_delay_mode )
    cfg ( int, ragebot_backtrack_mode )
    // weapon specific settings
    //ragebot_weapon_settings ragebot_weapons[ragebot_weapon_number];
    cfg ( int, ragebot_min_damage )
    cfg ( float, ragebot_hitchance )
    cfg ( bool, ragebot_hitbox_head )
    cfg ( bool, ragebot_hitbox_neck )
    cfg ( bool, ragebot_hitbox_stomach )
    cfg ( bool, ragebot_hitbox_chest )
    cfg ( bool, ragebot_hitbox_feet )
    cfg ( bool, ragebot_hitbox_arms )
    cfg ( float, ragebot_hitbox_head_scale )
    cfg ( float, ragebot_hitbox_body_scale )
    // enable pointscales
    cfg ( bool, ragebot_hitbox_head_pointscale )
    cfg ( bool, ragebot_hitbox_neck_pointscale )
    cfg ( bool, ragebot_hitbox_stomach_pointscale )
    cfg ( bool, ragebot_hitbox_chest_pointscale )
    cfg ( bool, ragebot_hitbox_feet_pointscale )
    cfg ( bool, ragebot_hitbox_arms_pointscale )
    /*
    cfg ( float, ragebot_hitbox_stomach_scale );
    cfg ( float, ragebot_hitbox_chest_scale );
    cfg ( float, ragebot_hitbox_feet_scale );
    cfg ( float, ragebot_hitbox_arms_scale );
    */
    cfg ( int, ragebot_autostop_mode )
    cfg ( int, ragebot_shooting_mode )
    cfg ( bool, ragebot_autostop )
    cfg ( int, ragebot_baim_mode )
    cfg ( int, ragebot_baim_after_shots )
    cfg ( bool, ragebot_baim_when_lethal )
    cfg ( bool, ragebot_antiaim_disable_on_knife )
    // anti-aim
    cfg ( bool, ragebot_antiaim_enable )
    //cfg ( int, ragebot_antiaim_yaw_base_mode )
    cfg ( std::string, ragebot_antiaim_yaw_base_mode )
    //cfg ( int, ragebot_antiaim_yaw_mode )
    cfg ( std::string, ragebot_antiaim_yaw_mode );
    cfg ( float, ragebot_antiaim_yaw_add )
    cfg ( float, ragebot_antiaim_yaw_jitter_range )
    cfg ( float, ragebot_antiaim_yaw_switch_range )
    cfg ( int, ragebot_antiaim_manual_right_key )
    cfg ( int, ragebot_antiaim_manual_left_key )
    cfg ( int, ragebot_antiaim_manual_back_key )
    cfg_local ( int, ragebot_manualaa_mode, 0 );
    cfg ( std::string, ragebot_antiaim_pitch_mode )
    //cfg ( int, ragebot_antiaim_desync_mode )
    cfg ( std::string, ragebot_antiaim_desync_mode )
    cfg ( int, ragebot_antiaim_fakeduck_key )
    cfg ( int, desync_switch_key )
    //cfg ( bool, ragebot_accuracy_slowwalk );
    cfg ( int, ragebot_accuracy_slowwalk_key )
    cfg ( float, ragebot_accuracy_slowwalk_speed )

    // fake-lag
    cfg ( bool, ragebot_fakelag_enable )
    cfg ( bool, ragebot_fakelag_while_unduck )
    /*
    cfg ( int, ragebot_fakelag_standing );
    cfg ( int, ragebot_fakelag_moving );
    cfg ( int, ragebot_fakelag_slowwalking );
    cfg ( int, ragebot_fakelag_in_air );
    cfg ( int, ragebot_fakelag_mode_standing );
    cfg ( int, ragebot_fakelag_mode_moving );
    cfg ( int, ragebot_fakelag_mode_slowwalking );
    cfg ( int, ragebot_fakelag_mode_in_air );
    */
    cfg ( int, ragebot_fakelag_standing_ticks )
    cfg ( int, ragebot_fakelag_moving_ticks )
    cfg ( int, ragebot_fakelag_slowwalk_ticks )
    cfg ( bool, ragebot_fakelag_while_shooting )
    //
    // misc
    //
    cfg ( bool, misc_bhop )
    cfg ( bool, misc_autostrafer )
    cfg ( bool, misc_eventlog )
    cfg ( bool, misc_clangtag_changer )
    cfg ( bool, menu_color )
    cfg ( bool, menu_color2 )
    cfg ( float, misc_clantag_changer_speed )
    cfg ( float, misc_viewmodel_x )
    cfg ( float, misc_viewmodel_y )
    cfg ( float, misc_viewmodel_z )
    cfg ( bool, misc_airduck )
    cfg ( bool, misc_slidewalk )

    cfg_local ( float, org_misc_viewmodel_x, 0.f );
    cfg_local ( float, org_misc_viewmodel_y, 0.f );
    cfg_local ( float, org_misc_viewmodel_z, 0.f );

    void save_config ( const std::string& name );
    void load_config ( const std::string& name );

    config_saver_struct_t get_option_data ( const std::string name );
    std::string settings_folder_ = "";
private:
    void setup_config();

    void set ( const std::string& name, bool* b, bool start_val, bool save = true );
    void set ( const std::string& name, int* i, int start_val, bool save = true );
    void set ( const std::string& name, float* f, float start_val, bool save = true );
    //void set ( std::string name, ragebot_weapon_settings* s, bool save = true );
    void set ( const std::string& name, Color* clr, Color start_val, bool save = true );
    void set ( const std::string& name, std::string* text, std::string start_val, bool save = true );
    //void set( std::string name, Vector* i, int start_val, bool save = true );
    //void set( std::string name, int* i, int start_val, bool save = true );

    std::unordered_map< std::string, config_saver_struct_t > config_data; // name, config_data
};

extern c_config g_options;
extern bool g_unload;
extern user_data_t g_user;
