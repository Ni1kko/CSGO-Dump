#include "hades_menu.h"
#include <WindowsX.h>
#include <utility>
#include "../helpers/console.h"
#include "../helpers/input.hpp"
#include "../helpers/math.hpp"
#include "../options.h"
#include "menu.hpp"
#include "../features/features.h"
Color text_selected = g_options.menu_color1;
hades_cursor g_cursor;

enum tabs_t
{
    ragebot,
    antiaim,
    legitbot,
    visuals,
    misc,
    skinchanger
};

c_hades_menu hades_menu;

void c_hades_menu::test_on_draw ( )
{
    if ( !menu.is_visible( ) )
    {
        was_last_open = false;
        return;
    }

    begin_menu ( 700, 400 );

    tab ( xor_str ( "RAGEBOT" ) );
    tab ( xor_str ( "ANTIAIM" ) ); // im gay just compile it again plz
    tab ( xor_str ( "LEGITBOT" ) );
    tab ( xor_str ( "VISUALS" ) );
    tab ( xor_str ( "MISC" ) );
    tab ( xor_str ( "SKINCHANGER" ) );

    auto get_index = [] ( std::vector< std::string > vec, std::string selected_str ) -> int
    {
        auto selected = 0;
        auto tmp_it = std::find (
            vec.begin( ),
            vec.end( ),
            selected_str );

        if ( tmp_it == vec.end( ) )
            selected = 0;
        else
            selected = std::distance ( vec.begin( ), tmp_it );

        return selected;
    };

    switch ( get_selected_tab( ) )
    {
    case ragebot:
        {
            set_section_count ( 3 );
            section( );
            {
                trimline ( xor_str ( "ragebot" ) );
                checkbox ( xor_str ( "enable" ), &g_options.ragebot_master );

                static bool hitbox_select_o = false;
                select_dropdown ( xor_str ( "hitboxes" ),
                                  {
                                      xor_str ( "head" ), xor_str ( "neck" ), xor_str ( "stomach" ), xor_str ( "chest" ), xor_str ( "feet" ),
                                      xor_str ( "arms" )
                                  },
                                  {
                                      &g_options.ragebot_hitbox_head, &g_options.ragebot_hitbox_neck,
                                      &g_options.ragebot_hitbox_stomach, &g_options.ragebot_hitbox_chest,
                                      &g_options.ragebot_hitbox_feet, &g_options.ragebot_hitbox_arms
                                  }, &hitbox_select_o );

                static bool pointscale_select_o = false;
                select_dropdown ( xor_str ( "multi-point" ),
                                  {
                                      xor_str ( "head" ), xor_str ( "neck" ), xor_str ( "stomach" ), xor_str ( "chest" ), xor_str ( "feet" ),
                                      xor_str ( "arms" )
                                  },
                                  {
                                      &g_options.ragebot_hitbox_head_pointscale, &g_options.ragebot_hitbox_neck_pointscale,
                                      &g_options.ragebot_hitbox_stomach_pointscale, &g_options.ragebot_hitbox_chest_pointscale,
                                      &g_options.ragebot_hitbox_feet_pointscale, &g_options.ragebot_hitbox_arms_pointscale
                                  }, &pointscale_select_o );

                if ( g_options.ragebot_hitbox_head_pointscale )
                    slider_percent ( xor_str ( "pointscale head" ), &g_options.ragebot_hitbox_head_scale );

                if ( g_options.ragebot_hitbox_neck_pointscale || g_options.ragebot_hitbox_stomach_pointscale || g_options.ragebot_hitbox_chest_pointscale ||
                    g_options.ragebot_hitbox_feet_pointscale || g_options.ragebot_hitbox_arms_pointscale
                )
                    slider_percent ( xor_str ( "pointscale body" ), &g_options.ragebot_hitbox_body_scale );

                checkbox ( xor_str ( "auto shoot" ), &g_options.ragebot_autoshoot );
                static bool shooting_mode_o = false;
                dropdown ( xor_str ( "silent aim" ),
                           {
                               xor_str ( "none" ), xor_str ( "instant" ), xor_str ( "client" ), xor_str ( "anti-aim" ), xor_str ( "supress" ),
                               xor_str ( "supress2" )
                           },
                           &g_options.ragebot_shooting_mode, &shooting_mode_o );
            }
            next_section( );
            section( );
            {
                trimline ( xor_str ( "accuracy" ) );
                slider_int ( xor_str ( "min damage" ), &g_options.ragebot_min_damage, 1, 100 );
                slider_percent ( xor_str ( "hitchance" ), &g_options.ragebot_hitchance );
                checkbox ( xor_str ( "autostop" ), &g_options.ragebot_autostop );
                label ( xor_str ( "slowwalk" ) );
                keybind ( &g_options.ragebot_accuracy_slowwalk_key );
                slider_percent ( xor_str ( "speed" ), &g_options.ragebot_accuracy_slowwalk_speed );
                checkbox ( xor_str ( "resolver" ), &g_options.ragebot_resolver );
                //ragebot_autostop_mode
                static bool baim_mode_o = false;
                /*
                think about making a multi select box with these options
                --> needs ragebot rewrites
                // bodyaim mode
                */
                dropdown ( xor_str ( "bodyaim mode" ),
                           {
                               xor_str ( "none" ), xor_str ( "adaptive" ), xor_str ( "desync" ), xor_str ( "prefer" )
                           },
                           &g_options.ragebot_baim_mode, &baim_mode_o );
                slider_int ( xor_str ( "bodyaim after shots" ), &g_options.ragebot_baim_after_shots, 0, 10 );
                checkbox ( xor_str ( "bodyaim when lethal" ), &g_options.ragebot_baim_when_lethal );
                static bool o_delay_mode = false;
                dropdown ( xor_str ( "delay" ), { xor_str ( "none" ), xor_str ( "wait for shot" ), xor_str ( "unlag" ) }, &g_options.ragebot_delay_mode,
                           &o_delay_mode );
                static bool o_backtrack_dropdown = false;
                dropdown ( xor_str ( "backtrack" ), { xor_str ( "none" ), xor_str ( "best" ), xor_str ( "all" ), xor_str ( "last" ) },
                           &g_options.ragebot_backtrack_mode, &o_backtrack_dropdown );
            }
            next_section( );
            section( );
        }
        break;

    case antiaim:
        set_section_count ( 3 );
        section( );
        {
            trimline ( xor_str ( "anti-aim" ) );
            checkbox ( xor_str ( "enable" ), &g_options.ragebot_antiaim_enable );

            static bool o_pitch_dropdown = false;
            //dropdown ( xor_str ( "pitch" ),
            //{
            //    xor_str ( "none" ),
            //    xor_str ( "down" ),
            //    xor_str ( "zero" ),
            //    xor_str ( "up" )
            //},
            //&g_options.ragebot_antiaim_pitch_mode, &o_pitch_dropdown );

            auto selected_pitch_mode = get_index ( g_features.anti_aim.pitch_antiaim_names, g_options.ragebot_antiaim_pitch_mode );
            //static auto selected_desync_mode = 0;
            dropdown ( xor_str ( "pitch" ), g_features.anti_aim.pitch_antiaim_names, &selected_pitch_mode, &o_pitch_dropdown );

            g_options.ragebot_antiaim_pitch_mode = g_features.anti_aim.pitch_antiaim_names.at ( selected_pitch_mode );

            static bool o_yaw_base_dropdown = false;

            auto selected_base_antiaim = get_index ( g_features.anti_aim.real_base_antiaim_names, g_options.ragebot_antiaim_yaw_base_mode );
            dropdown ( xor_str ( "yaw base" ),
                       g_features.anti_aim.real_base_antiaim_names,
                       &selected_base_antiaim, &o_yaw_base_dropdown );

            g_options.ragebot_antiaim_yaw_base_mode = g_features.anti_aim.real_base_antiaim_names.at ( selected_base_antiaim );

            static bool o_yaw_dropdown = false;
            auto selected_add_antiaim = get_index ( g_features.anti_aim.real_add_antiaim_names, g_options.ragebot_antiaim_yaw_mode );
            dropdown ( xor_str ( "yaw" ),
                       g_features.anti_aim.real_add_antiaim_names,
                       &selected_add_antiaim, &o_yaw_dropdown );
            slider_degrees ( xor_str ( "yaw add" ), &g_options.ragebot_antiaim_yaw_add, -180.f, 180.f );

            g_options.ragebot_antiaim_yaw_mode = g_features.anti_aim.real_add_antiaim_names.at ( selected_add_antiaim );

            /*
                set ( xor_str ( "ragebot_antiaim_yaw_jitter_range" ), &g_options.ragebot_antiaim_yaw_jitter_range, 35.f );
            set ( xor_str ( "ragebot_antiaim_yaw_switch_range" ), &g_options.ragebot_antiaim_yaw_switch_range, 35.f );
            */
            if ( g_options.ragebot_antiaim_yaw_mode == xor_str ( "jitter" ) )
                slider_degrees ( xor_str ( "jitter range" ), &g_options.ragebot_antiaim_yaw_jitter_range, 0, 180.f );
            else if ( g_options.ragebot_antiaim_yaw_mode == xor_str ( "switch" ) )
                slider_degrees ( xor_str ( "switch range" ), &g_options.ragebot_antiaim_yaw_switch_range, -180.f, 180.f );

            //static float degree_test = 0.f;
            //slider_degrees ( "yaw add", &degree_test, -180, 180 );
            label ( xor_str ( "manual aa right" ) );
            keybind ( &g_options.ragebot_antiaim_manual_right_key );
            label ( xor_str ( "manual aa left" ) );
            keybind ( &g_options.ragebot_antiaim_manual_left_key );
            label ( xor_str ( "manual aa back" ) );
            keybind ( &g_options.ragebot_antiaim_manual_back_key );
            checkbox ( xor_str ( "disable antiam on knife" ), &g_options.ragebot_antiaim_disable_on_knife );
        }
        next_section( );
        section( );
        {
            trimline ( xor_str ( "desync" ) );
            static bool o_desync_mode = false;
            auto selected_desync_mode = get_index ( g_features.anti_aim.desync_antiaim_names, g_options.ragebot_antiaim_desync_mode );
            //static auto selected_desync_mode = 0;
            dropdown ( xor_str ( "desync mode" ), g_features.anti_aim.desync_antiaim_names, &selected_desync_mode, &o_desync_mode );

            g_options.ragebot_antiaim_desync_mode = g_features.anti_aim.desync_antiaim_names.at ( selected_desync_mode );

            spacing( );
            label ( xor_str ( "fakeduck" ) );
            keybind ( &g_options.ragebot_antiaim_fakeduck_key );
        }
        next_section( );
        section( );
        {
            trimline ( xor_str ( "fakelag" ) );
            checkbox ( xor_str ( "enable" ), &g_options.ragebot_fakelag_enable );
            slider_int ( xor_str ( "ticks standing" ), &g_options.ragebot_fakelag_standing_ticks, 1, 16 );
            slider_int ( xor_str ( "ticks moving" ), &g_options.ragebot_fakelag_moving_ticks, 1, 16 );
            slider_int ( xor_str ( "ticks slowwalking" ), &g_options.ragebot_fakelag_slowwalk_ticks, 1, 16 );
            checkbox ( xor_str ( "fakelag while shooting" ), &g_options.ragebot_fakelag_while_shooting );
            checkbox ( xor_str ( "fakelag while unduck" ), &g_options.ragebot_fakelag_while_unduck );
        }
        break;

    case legitbot:
        label ( xor_str ( "coming soon" ) );
        break;

    case visuals:
        set_section_count ( 3 );
        section ( 1.f );
        {
            trimline ( xor_str ( "esp" ) );
            checkbox ( xor_str ( "box" ), &g_options.esp_enemy_box );
            checkbox ( xor_str ( "health" ), &g_options.esp_enemy_health );
            checkbox ( xor_str ( "armor" ), &g_options.esp_enemy_armor );
            checkbox ( xor_str ( "name" ), &g_options.esp_enemy_name );
            checkbox ( xor_str ( "weapon" ), &g_options.esp_enemy_weapon );

            static bool o_e_flags = false;
            select_dropdown ( xor_str ( "flags" ), { xor_str ( "scoped" ), xor_str ( "fake" ), xor_str ( "reloading" ) },
                              { &g_options.esp_enemy_flag_scoped, &g_options.esp_enemy_flag_fake, &g_options.esp_enemy_flag_reloading }, &o_e_flags );
            trimline ( xor_str ( "local" ) );
            //checkbox ( xor_str ( "thirdperson" ), &g_options.visuals_thirdperson );
            label ( xor_str ( "thirdperson" ) );
            keybind ( &g_options.visuals_thirdperson_key );
            slider_float ( xor_str ( "distance" ), &g_options.visuals_thirdperson_distance, 10, 300, 0 );
            checkbox ( xor_str ( "noflash" ), &g_options.visuals_noflash );
            checkbox ( xor_str ( "remove viewpunch" ), &g_options.visuals_remove_viewpunch );
            checkbox ( xor_str ( "manual aa indicator" ), &g_options.visuals_manual_aa_indicator );

            static bool o_indicator_selector = false;
            select_dropdown ( xor_str ( "indicators" ), { xor_str ( "desync" ), xor_str ( "lc" ) },
                              { &g_options.visuals_local_desync, &g_options.visuals_local_lc }, &o_indicator_selector );

            checkbox ( xor_str ( "projectiles" ), &g_options.esp_world_weapons );
            checkbox ( xor_str ( "weapon box" ), &g_options.weapon_box );
            checkbox ( xor_str ( "weapon ammo" ), &g_options.esp_world_weapons_ammo );

            checkbox ( xor_str ( "offscreen" ), &g_options.esp_offscreen );
            static bool offscreencolor = false;
            colorp ( &g_options.visuals_offscreen_color, &offscreencolor, 11 );

            slider_percent ( xor_str ( "range" ), &g_options.visuals_invisible_players_range );

            checkbox ( xor_str ( "offscreen enemy only" ), &g_options.esp_offscreen_enemyonly );
        }
        next_section( );
        section( );
        {
            trimline ( xor_str ( "chams" ) );
            checkbox ( xor_str ( "enable" ), &g_options.chams_master );
            static bool o_enemy_chams_mode = false;
            static bool o_color_chams_enemy = false;
            static bool o_color_chams_enemy_xqz = false;
            dropdown ( xor_str ( "enemy chams mode" ), { xor_str ( "none" ), xor_str ( "normal" ), xor_str ( "flat" ), xor_str ( "metallic" ) },
                       &g_options.chams_type_enemy, &o_enemy_chams_mode, true,
                       !o_color_chams_enemy && !o_color_chams_enemy_xqz );
            colorp ( &g_options.chams_color_enemy, &o_color_chams_enemy );
            colorp ( &g_options.chams_color_xqz_enemy, &o_color_chams_enemy_xqz, 11 );
            static bool o_team_chams_mode = false;
            static bool o_color_chams_team = false;
            static bool o_color_chams_team_xqz = false;
            dropdown ( xor_str ( "team chams mode" ), { xor_str ( "none" ), xor_str ( "normal" ), xor_str ( "flat" ), xor_str ( "metallic" ) },
                       &g_options.chams_type_team, &o_team_chams_mode, true,
                       !o_color_chams_team && !o_color_chams_team_xqz );
            colorp ( &g_options.chams_color_team, &o_color_chams_team );
            colorp ( &g_options.chams_color_xqz_team, &o_color_chams_team_xqz, 11 );
            static bool o_local_chams_mode = false;
            static bool o_color_chams_local = false;
            dropdown ( xor_str ( "local chams mode" ), { xor_str ( "none" ), xor_str ( "normal" ), xor_str ( "flat" ), xor_str ( "metallic" ) },
                       &g_options.chams_type_local, &o_local_chams_mode, true, !o_color_chams_local );
            colorp ( &g_options.chams_color_local, &o_color_chams_local );
            static bool o_desync_chams = false;
            static bool o_desync_chams_desync = false;
            static bool o_desync_chams_pos = false;
            select_dropdown ( xor_str ( "fake chams" ), { xor_str ( "desync" ), xor_str ( "fakelag position" ) },
                              { &g_options.chams_fake_desync, &g_options.chams_fake_pos }, &o_desync_chams, !o_desync_chams_desync && !o_desync_chams_pos );
            colorp ( &g_options.color_chams_fake_desync, &o_desync_chams_desync );
            colorp ( &g_options.color_chams_fake_pos, &o_desync_chams_pos, 11 );
            slider_percent ( xor_str ( "local scope transperancy" ), &g_options.visuals_scope_transparency );
            static bool o_backtrack_chams = false;
            checkbox ( xor_str ( "backtrack chams" ), &g_options.chams_enemy_backtrack );
            colorp ( &g_options.color_chams_backtrack, &o_backtrack_chams );
        }
        next_section( );
        section( );
        {
            trimline ( xor_str ( "world" ) );
            checkbox ( xor_str ( "nightmode" ), &g_options.visuals_nightmode );

#ifdef _DEBUG
            checkbox ( xor_str ( "asus_walls" ), &g_options.visuals_asuswalls );
#endif // _DEBUG

            //checkbox ( xor_str ( "enable" ), &g_options.esp_master );
            if ( g_options.visuals_nightmode )
                slider_percent ( xor_str ( "nightmode alpha" ), &g_options.visuals_nightmode_alpha );

            trimline ( xor_str ( "additionals" ) );
            slider_float ( xor_str ( "fov" ), &g_options.visuals_fov, 1, 130, 0 );
            checkbox ( xor_str ( "radar" ), &g_options.visuals_radar );
            checkbox ( xor_str ( "force crosshair" ), &g_options.visuals_crosshair );

            trimline ( xor_str ( "removals" ) );
            static bool b_visuals_nosmoke_mode_o = false;
            checkbox ( xor_str ( "remove scope zoom" ), &g_options.visuals_remove_scope_zoom );
            dropdown ( xor_str ( "no-smoke" ),
                       {
                           xor_str ( "disabled" ), xor_str ( "wireframe" ), xor_str ( "no render" )
                       },
                       &g_options.visuals_nosmoke_mode, &b_visuals_nosmoke_mode_o );
            trimline ( xor_str ( "footesp beam" ) );
            checkbox ( xor_str ( "footstep esp" ), &g_options.visuals_footstep_esp );
            static bool o_color_footstep = false;
            colorp ( &g_options.visuals_footstep_esp_color, &o_color_footstep );
            slider_float ( xor_str ( "size" ), &g_options.visuals_beam_ring_size, 5, 500, 0 );
            trimline ( xor_str ( "bullet tracers" ) );
            checkbox ( xor_str ( "enable" ), &g_options.visuals_bullet_tracers );
            static bool o_color_beams = false;
            colorp ( &g_options.visuals_beams_bullet_tracer, &o_color_beams );
        }
        break;

    case misc:
        {
            set_section_count ( 3 );
            section( );
            {
                trimline ( xor_str ( "misc" ) );
                checkbox ( xor_str ( "bhop" ), &g_options.misc_bhop );
                checkbox ( xor_str ( "autostrafer" ), &g_options.misc_autostrafer );
                checkbox ( xor_str ( "eventlog" ), &g_options.misc_eventlog );
                checkbox ( xor_str ( "clangtag changer" ), &g_options.misc_clangtag_changer );

                if ( g_options.misc_clangtag_changer )
                    slider_percent ( xor_str ( "clantag speed" ), &g_options.misc_clantag_changer_speed );

                slider_float ( xor_str ( "viewmodel x" ), &g_options.misc_viewmodel_x, -15.f, 15.f, 1 );
                slider_float ( xor_str ( "viewmodel y" ), &g_options.misc_viewmodel_y, -15.f, 15.f, 1 );
                slider_float ( xor_str ( "viewmodel z" ), &g_options.misc_viewmodel_z, -15.f, 15.f, 1 );

                label ( xor_str ( "Menu Color" ) );
                colorp ( &g_options.menu_color1, &g_options.menu_color, 11 );

                checkbox ( xor_str ( "airduck" ), &g_options.misc_airduck );
                checkbox ( xor_str ( "slidewalk" ), &g_options.misc_slidewalk );
            }
            next_section( );
            section( );
            {
                trimline ( xor_str ( "other" ) );
                static bool b_open_config_select = false;
                static int config_selection = 0;
                dropdown ( xor_str ( "configurations" ),
                           {
                               xor_str ( "legit" ), xor_str ( "hvh" ), xor_str ( "scout" ), xor_str ( "awp" ), xor_str ( "pistol" )
                           },
                           &config_selection, &b_open_config_select );

                auto get_cfg_name = [] ( ) -> std::string
                {
                    switch ( config_selection )
                    {
                    case 0:
                        return xor_str ( "legit" );

                    case 1:
                        return xor_str ( "hvh" );

                    case 2:
                        return xor_str ( "scout" );

                    case 3:
                        return xor_str ( "awp" );

                    case 4:
                        return xor_str ( "pistol" );

                    default:
                        ;
                    }
                    return xor_str ( "err" );
                };

                button ( xor_str ( "load" ), [&] ( )
                {
                    g_options.load_config ( get_cfg_name( ) );
                } );
                button ( xor_str ( "save" ), [&] ( )
                {
                    g_options.save_config ( get_cfg_name( ) );
                } );
            }
            next_section( );
            section( );
            {
                trimline( );
                button ( xor_str ( "unload" ), [] ( )
                {
                    g_unload = true;
                } );
            }
        }
        break;

    case skinchanger:
        label ( xor_str ( "coming soon" ) );
        break;
    }

    end_menu( );
    keybinds_rendered = 0;
}

void c_hades_menu::on_wnd_proc ( HWND, const UINT u_msg, const WPARAM w_param, LPARAM )
{
    switch ( u_msg )
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        {
            if ( keybind_selected != -1 )
            {
                if ( static_cast< int > ( w_param ) == VK_ESCAPE )
                    keybind_cancel = true;

#ifdef _DEBUG
                console::write_line ( "keydown nigga" );
#endif // _DEBUG

                console::write_line ( get_name_for_key ( w_param ) );
                console::write_line ( keybind_selected );
                keybind_last_key_selected = w_param;
            }
        }
        break;

    case WM_XBUTTONDOWN:
        keybind_last_key_selected = ( HIWORD ( w_param ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
        break;

    case WM_MBUTTONDOWN:
        keybind_last_key_selected = VK_MBUTTON;
        break;

    default:
        ;
    }
}

void c_hades_menu::begin_menu ( int width, int height )
{
    //if ( !was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x, draw_pos.y, width, 20 ) ) )
    //last_hover = Vector4D ( draw_pos.x, draw_pos.y, width, 20 );

    const auto in_hover = !was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh (
        Vector4D ( draw_pos.x, draw_pos.y, width, 20 )
    ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x, draw_pos.y, width, 20 )*/last_hover );
    const auto m_down = g_inputsys.is_key_down ( VK_LBUTTON );
    const auto in_click = in_hover && m_down;

    if ( ( in_click && was_last_open ) || ( m_down && did_move_last ) )
    {
        draw_pos.x += g_cursor.get_pos( ).x - last_mouse_pos.x;
        draw_pos.y += g_cursor.get_pos( ).y - last_mouse_pos.y;
        did_move_last = true;
        //console::write_line ( "window move" );
    }
    else
        did_move_last = false;

    in_render = true;

    menu_wh = { width, height };

    // outline gradient
    //g_render.gradient_vertical_wh ( draw_pos.x, draw_pos.y + 10, width + 4, height - 10, Color ( 0, 0, 0, 255 ), Color ( 0, 0, 0, 0 ) );

    interfaces::vgui_surface->clipping_enabled( ) = true;
    interfaces::vgui_surface->set_clip_rect ( draw_pos.x - 1, draw_pos.y - 1, draw_pos.x + width + 1, draw_pos.y + height + 1 );

    g_render.filled_box_wh ( draw_pos.x - 1, draw_pos.y - 1, width + 2, height + 2, Color ( 1, 1, 1, 255 ) );
    g_render.filled_box_wh ( draw_pos.x, draw_pos.y, width, height, Color ( 28, 28, 28, 255 ) );

    //outline gay
    g_render.filled_box_wh ( draw_pos.x + 8 - 1, draw_pos.y + 20 - 1, width - 16 + 2, height - 28 + 2, Color ( 1, 1, 1, 255 ) ); // 24 ; 48

    // real inline (where all components are placed)
    g_render.filled_box_wh ( draw_pos.x + 8, draw_pos.y + 20, width - 16, height - 28, Color ( 25, 25, 25, 255 ) ); // 24 ; 48

    add_size ( 12, -4 );

    /* menu section */
    g_render.filled_box_wh ( draw_pos.x + 8, draw_pos.y + 20, 99, height - 28, Color ( 42, 42, 42, 255 ) );

    std::string title1 = xor_str ( "| gethad.es | " ) + std::string ( xor_str ( __DATE__ ) );
    std::transform ( title1.begin( ), title1.end( ), title1.begin( ), ::tolower );
    /* title */
    const auto text_size = g_render.get_size ( g_fonts.menu14, xor_str ( "GETHAD.ES" ) );
    const auto x_center = draw_pos.x + ( static_cast< int > ( width / 2.f ) - static_cast< int > ( text_size.x / 2.f ) );
    const auto gayshit = draw_pos.x + ( static_cast< int > ( width ) - 130 );
    const auto y_center = static_cast< int > ( ( 20 - text_size.y ) / 2.f );
    g_render.text ( g_fonts.menu14, xor_str ( ".ES" ), x_center + 37, draw_pos.y + y_center, g_options.menu_color1 );
    g_render.text ( g_fonts.menu14, xor_str ( "GETHAD" ), x_center - 6, draw_pos.y + y_center, Color ( 255, 255, 255, 255 ) );

    g_render.text ( g_fonts.menu_watermark, title1, gayshit - 6, draw_pos.y + y_center + 2, Color ( 90, 90, 90, 255 ) );

#ifdef _DEBUG
    g_render.text ( g_fonts.menu14, "(DEV)", x_center + 56, draw_pos.y + y_center, Color ( 255, 255, 255, 255 ) );
#endif // _DEBUG

    was_m1_pressed = g_inputsys.was_key_pressed ( VK_LBUTTON );

    add_size ( 100, 20 );
    was_last_open = true;
    last_mouse_pos = g_cursor.get_pos( );

    section_y_offset = current_y_add;
}

void c_hades_menu::end_menu ( )
{
    if ( !in_render )
    {
        std::exception give_me_a_name ( xor_str ( "end_menu called before begin_menu" ) );
        return;
    }

    const int tabsize = tab_draw_list.size( );

    if ( tabsize != 0 )
    {
        const auto center_y = ( ( menu_wh.y ) / 2 ) - 28 - 2 + 40;

        for ( auto i = 0; i < tabsize; i++ )
        {
            std::string tab = tab_draw_list[ i ];

            const auto y = ( center_y - ( 20 * tabsize ) ) + ( ( 44 ) * i );
            //Color clr = Color ( 50, 50, 50, 255 );

            if ( i == selected_tab )
                g_render.filled_box_wh ( draw_pos.x + 8, draw_pos.y + y, 100, 40, Color ( 25, 25, 25, 255 ) );

            //if ( !was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + 8, draw_pos.y + y, 100, 40 ) ) )
            //    last_hover = Vector4D ( draw_pos.x + 8, draw_pos.y + y, 100, 40 );

            bool in_hover = g_cursor.in_vector_wh (
                Vector4D ( draw_pos.x + 8, draw_pos.y + y, 100, 40 )
            ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + 8, draw_pos.y + y, 100, 40 )*/last_hover );

            Vector2D t_pos = g_render.get_size ( g_fonts.menu12, tab );

            Color text_clr = text_unselected;

            if ( i == selected_tab )
                text_clr = g_options.menu_color1;

            g_render.text ( g_fonts.menu12, tab, draw_pos.x + 10 + ( 50 - ( t_pos.x / 2 ) ), draw_pos.y + y + ( ( 40 / 2 ) - ( t_pos.y / 2 ) ), text_clr );

            g_render.text ( g_fonts.icon, "H", draw_pos.x + 33, draw_pos.y + 20, g_options.menu_color1 );

            if ( i == selected_tab )
            {
                //g_render.filled_box_wh ( draw_pos.x + 8, draw_pos.y + y - 3, 100, 2, Color ( 33, 33, 33, 255 ) );
                //g_render.filled_box_wh ( draw_pos.x + 8, draw_pos.y + y + 41, 100, 2, Color ( 33, 33, 33, 255 ) );
                g_render.filled_box_wh ( draw_pos.x + 12, draw_pos.y + y + 10, 2, 20, g_options.menu_color1 );
            }

            if ( in_hover && was_m1_pressed )
                selected_tab = i;
        }
    }

    tab_draw_list.clear( );

    current_y_add = 0;
    current_x_add = 0;
    tab_count = 0;

    was_m1_pressed = false;

    g_cursor.render( );
    current_section_components_width = 250;

    was_m1_last_down = g_inputsys.is_key_down ( VK_LBUTTON );
    interfaces::vgui_surface->clipping_enabled( ) = false;
}

void c_hades_menu::tab ( const std::string& title )
{
    //int old_tab_count = tab_count;
    tab_count++;

    tab_draw_list.push_back ( title );

    //return old_tab_count == selected_tab;
}

void c_hades_menu::colorp ( Color* clr, bool* open, float offset )
{
    //float p = 50.f / 360.f;
    int x = draw_pos.x + current_x_add + current_section_width - 15;
    int y = draw_pos.y + last_element_y + offset + 2;

    bool clr_box_in_hover = g_cursor.in_vector_wh (
        Vector4D ( x, y, 11, 8 )
    );

    g_render.filled_box_wh ( x, y, 11, 8, *clr );

    if ( clr_box_in_hover && was_m1_pressed )
        *open = !*open;

    if ( !*open )
        g_render.gradient_vertical_wh ( x, y, 11, 8, Color ( 255, 255, 255, 255 ), Color ( 30, 30, 30, 255 ) );

    bool picker_in_hover = false;

    if ( *open )
    {
        int w = static_cast< int > ( current_section_components_width / 2.f );
        x -= ( current_section_components_width / 2.f ) + 4;

        g_render.filled_box_wh ( x + w + 1, y - 5 + 1, 1 - 2, 25 - 2, Color ( 10, 10, 10, 255 ) );

        for ( auto pos = 0; pos < w; pos++ )
        {
            const auto p = static_cast< float > ( pos ) / static_cast< float > ( w );
            const auto hue = static_cast< int > ( p * 360.f );
            const auto c = hsv_to_rgb ( p, 1, 1 );
            g_render.filled_box_wh ( x + pos, y - 5, 1, 20, Color ( c.r( ), c.g( ), c.b( ), 255 ) );
        }

        bool in_hover = g_cursor.in_vector_wh (
            Vector4D ( x, y, w, 20 )
        );

        picker_in_hover = in_hover;

        bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

        if ( in_click )
        {
            console::write_line ( ( g_cursor.get_pos( ).x - x ) );
            float percent = static_cast< float > ( g_cursor.get_pos( ).x - x ) / static_cast< float > ( w );
            Color c = hsv_to_rgb ( percent, 1, 1 );
            *clr = Color ( c.r( ), c.g( ), c.b( ), 255 );

            if ( was_m1_pressed )
                *open = false;
        }

        //percent = ( g_cursor.get_pos().x - ( draw_pos.x + current_x_add + 15 ) ) / ( current_section_components_width - 15 - 15 );
        //if()
    }

    if ( !clr_box_in_hover && !picker_in_hover && was_m1_pressed )
        *open = false;
}

template < typename Functor >
inline void c_hades_menu::button ( const std::string& text, Functor lambda )
{
    //if ( !was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 ) ) )
    //    last_hover = Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 );

    bool in_hover = g_cursor.in_vector_wh (
        Vector4D ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20 )
    ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 )*/last_hover );
    bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 20 );
        return;
    }

    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20, outline );

    if ( in_click )
    {
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 2 - 15 - 15, 18,
                                 Color ( 35, 35, 35, 255 ) );
        g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1,
                                        current_section_components_width - 2 - 15 - 15, 18, Color ( 35, 35, 35, 255 ), Color ( 50, 50, 50, 255 ) );
    }
    else
    {
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 2 - 15 - 15, 18,
                                 Color ( 50, 50, 50, 255 ) );
        g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1,
                                        current_section_components_width - 2 - 15 - 15, 18, Color ( 50, 50, 50, 255 ), Color ( 35, 35, 35, 255 ) );
    }

    pos_vec next_text_pos = get_text_center ( g_fonts.menu12, text, current_section_components_width, 20 );
    g_render.text ( g_fonts.menu12, text, next_text_pos.x, next_text_pos.y, Color ( 255, 255, 255, 255 ) );

    if ( was_m1_pressed && in_hover )
        lambda( );

    add_size ( 0, 20 );
}

void c_hades_menu::slider_int ( std::string label, int* i, int min, int max )
{
    if ( *i < min )
        *i = min;

    if ( *i > max )
        *i = max;

    float f2 = *i - min;
    float p = ( f2 ) / ( max - min );

    std::string t = "  " + std::to_string ( *i );
    slider_text ( std::move ( label ), t, &p );

    *i = static_cast< int > ( min + p * ( max - min ) );
}

void c_hades_menu::slider_float ( std::string label, float* f, float min, float max, int decimals )
{
    if ( *f < min )
        *f = min;

    if ( *f > max )
        *f = max;

    float f2 = *f - min;
    float p = ( f2 ) / ( max - min );

    std::string t = to_string_with_precision ( *f, decimals );
    slider_text ( std::move ( label ), t, &p );

    *f = min + p * ( max - min );
}

void c_hades_menu::slider_degrees ( std::string label, float* f, float min, float max, int decimals )
{
    if ( *f < min )
        *f = min;

    if ( *f > max )
        *f = max;

    float f2 = *f - min;
    float p = ( f2 ) / ( max - min );

    std::string t = to_string_with_precision ( *f, decimals );
    t += "*";
    slider_text ( std::move ( label ), t, &p );

    *f = min + p * ( max - min );
}

void c_hades_menu::slider_percent ( std::string label, float* percent )
{
    std::string t = to_string_with_precision ( *percent * 100, 0 );
    slider_text ( std::move ( label ), t + "%", percent );
}

void c_hades_menu::slider_text ( std::string label, const std::string& text, float* percent )
{
    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 1 + 4, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 22 );
        return;
    }

    g_render.text ( g_fonts.menu12, std::move ( label ), draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add, Color ( 255, 255, 255, 255 ) );

    add_size ( 0, 10 );

    //if ( !was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 8 ) ) )
    //    last_hover = Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 8 );

    Vector4D vec_to_hover = Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width - 15, 8 );
    /*
    if ( g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( vec_to_hover ) )
    	slider_last_hover = vec_to_hover;
    else if ( !g_inputsys.is_key_down ( VK_LBUTTON ) )
    	slider_last_hover = { -1, -1, -1, -1 };
    	*/

    bool in_hover = g_cursor.in_vector_wh (
        vec_to_hover
    ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 8 )*/last_hover );

    bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 8, outline );
    //g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 15 , draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 8, Color(90,90,90,255) );

    g_render.gradient_horizontal_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 30, 6,
                                      Color ( 90, 90, 90, 255 ), Color ( 90, 90, 90, 255 ) );

    float width = ( current_section_components_width - 15 - 15 ) * *percent;

    {
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, width, 6, g_options.menu_color1 );
        g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, width, 6, Color ( 60, 60, 60, 255 ),
                                        Color ( 60, 60, 60, 90 ) );
    }

    Vector2D ts = g_render.get_size ( g_fonts.menu12, text );
    int x_size = static_cast< int > ( ts.x / 2.f );

    if ( x_size <= 1 )
        x_size = 0;

    g_render.text ( g_fonts.menu12, text, draw_pos.x + current_x_add + width - static_cast< int > ( x_size / 2.f ) + 15,
                    draw_pos.y + current_y_add + 4 - static_cast< int > ( ts.y / 2.f ), Color ( 255, 255, 255, 255 ) );

    if ( in_click )
    {
        *percent = ( g_cursor.get_pos( ).x - ( draw_pos.x + current_x_add + 15 ) ) / ( current_section_components_width - 15 - 15 );

        if ( *percent < 0.f )
            *percent = 0.f;

        if ( *percent > 1.f )
            *percent = 1.f;
    }

    add_size ( 0, 10 );
}

void c_hades_menu::checkbox ( const std::string& text, bool* b )
{
    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 10 );
        return;
    }

    //if ( !was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, 10, 10 ) ) )
    //    last_hover = Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, 10, 10 );

    bool in_hover = g_cursor.in_vector_wh (
        Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, 10, 10 )
    ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, 10, 10 )*/last_hover );
    bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, 10, 10, Color ( 1, 1, 1, 255 ) );
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 1, draw_pos.y + current_y_add + 1, 8, 8, Color ( 40, 40, 40, 255 ) );

    if ( was_m1_pressed && in_hover )
        *b = !*b;

    bool tmp_b = in_click && !*b;

    if ( *b || tmp_b )
    {
        if ( in_click )
        {
            g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4, draw_pos.y + current_y_add + 1, 8, 8, Color ( 50, 0, 0, 255 ) );
            g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4, draw_pos.y + current_y_add + 1, 8, 8, Color ( 50, 0, 0, 255 ),
                                            g_options.menu_color1 );
        }
        else
        {
            g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4, draw_pos.y + current_y_add + 1, 8, 8, g_options.menu_color1 );
            g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4, draw_pos.y + current_y_add + 1, 8, 8, Color ( 0, 0, 0, 255 ),
                                            g_options.menu_color1 );
            g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4, draw_pos.y + current_y_add + 1, 8, 8, Color ( 0, 0, 0, 0 ),
                                            Color ( 40, 40, 40, 60 ) );
        }
    }

    Vector2D text_size = g_render.get_size ( g_fonts.menu12, text );
    int y_re = ( text_size.y / 2.f ) - 5;
    g_render.text ( g_fonts.menu12, text, draw_pos.x + current_x_add + 15 + 4, draw_pos.y + current_y_add - y_re - 1, Color ( 255, 255, 255, 255 ) );

    add_size ( 0, 10 );
}

void c_hades_menu::checkbox_color ( ) const
{
}

// im gay

void c_hades_menu::dropdown ( std::vector< std::string > items, int* selected, bool* open, bool close_on_select, bool enable_interaction )
{
    if ( items.size( ) == 0 )
        std::exception ( xor_str ( "item size == 0" ) );

    if ( items.size( ) <= ( size_t )*selected )
        *selected = 0;

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 20 );
        return;
    }

    bool in_hover = enable_interaction && g_cursor.in_vector_wh (
        Vector4D ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20 )
    );
    //!was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 )*/last_hover );
    bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20, outline );
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 2 - 15 - 15, 18,
                             Color ( 40, 40, 40, 255 ) );
    g_render.gradient_vertical_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 2 - 15 - 15,
                                    18, Color ( 40, 40, 40, 255 ), Color ( 50, 50, 50, 255 ) );

    pos_vec next_text_pos = get_text_center ( g_fonts.menu12, items[ *selected ], current_section_components_width - 15 - 15, 20 );
    g_render.text ( g_fonts.menu12, items[ *selected ], draw_pos.x + current_x_add + 1 + 12 + 15, next_text_pos.y, Color ( 255, 255, 255, 255 ) );

    int item_window_height = items.size( ) * 20;
    last_dropdown_wh = { 0, 0 };
    last_dropdown_pos = { 0, 0 };
    bool dropdown_hovered = g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1 + 18 + 2,
                                                               current_section_components_width - 2 - 15 - 15, item_window_height ) );

    if ( in_hover && was_m1_pressed )
        *open = !*open;
    else if ( !in_hover && was_m1_pressed && !dropdown_hovered )
        *open = false;

    //
    // maybe remove spacing and add outline
    //
    if ( *open )
    {
        interfaces::vgui_surface->clipping_enabled( ) = false;

        last_dropdown_wh.h = item_window_height;
        last_dropdown_wh.w = current_section_components_width - 15 - 15;
        last_dropdown_pos.y = draw_pos.y + current_y_add + 1 + 18 + 2;
        last_dropdown_pos.x = draw_pos.x + current_x_add + 1 + 4;
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1 + 18 + 2,
                                 current_section_components_width - 2 - 15 - 15, item_window_height, Color ( 50, 50, 50, 255 ) );

        for ( size_t i = 0; i < items.size( ); i++ )
        {
            bool item_in_hover = g_cursor.in_vector_wh (
                Vector4D (
                    draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 22 + ( i * 20 ) + 1,
                    current_section_components_width - 1 - 15 - 15, 20 - 2
                )
            );

            if ( item_in_hover )
                g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 22 + ( i * 20 ),
                                         current_section_components_width - 2 - 15 - 15, 20, Color ( 33, 33, 33, 255 ) );

            auto item = items[ i ];
            pos_vec next_text_pos = get_text_center ( g_fonts.menu12, item, current_section_components_width - 15 - 15, 20 );
            Color text_clr = *selected == i ? g_options.menu_color1 : Color ( 255, 255, 255, 255 );
            g_render.text ( g_fonts.menu12, item, draw_pos.x + current_x_add + 1 + 12 + 15, next_text_pos.y + 22 + ( i * 20 ), text_clr );

            if ( item_in_hover && was_m1_pressed )
            {
                if ( close_on_select )
                    *open = !open;

                *selected = i;
            }
        }

        interfaces::vgui_surface->clipping_enabled( ) = true;
    }

    add_size ( 0, 20 );
}

// todo
// do title on top of box
void c_hades_menu::dropdown ( std::string title, std::vector< std::string > items, int* selected, bool* open, bool close_on_select, bool enable_interaction )
{
    if ( items.empty( ) )
        std::exception ( xor_str ( "item size == 0" ) );

    if ( items.size( ) <= ( size_t )*selected )
        *selected = 0;

    if ( should_render ( pos_vec ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add ) ) )
    {
        g_render.text ( g_fonts.menu12, std::move ( title ), draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add,
                        Color ( 255, 255, 255, 255 ) );
        add_size ( 0, 10 );
    }
    else
        add_size ( 0, 10 );

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 20 );
        return;
    }

    bool in_hover = enable_interaction && g_cursor.in_vector_wh (
        Vector4D ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20 )
    );
    //!was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 )*/last_hover );
    bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20, outline );
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 2 - 15 - 15, 18,
                             Color ( 60, 60, 60, 255 ) );
    //g_render.text(g_fonts.menu12, "▼", draw_pos.x + current_x_add + current_section_components_width - 20, draw_pos.y + current_y_add + 1, Color(30, 30, 30, 255));

    pos_vec next_text_pos = get_text_center ( g_fonts.menu12, items[ *selected ], current_section_components_width - 15 - 15, 20 );
    g_render.text ( g_fonts.menu12, items[ *selected ], draw_pos.x + current_x_add + 1 + 12 + 15, next_text_pos.y, Color ( 255, 255, 255, 255 ) );

    int item_window_height = items.size( ) * 20;
    last_dropdown_wh = { 0, 0 };
    last_dropdown_pos = { 0, 0 };
    bool dropdown_hovered = g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1 + 21 + 2,
                                                               current_section_components_width - 2 - 15 - 15, item_window_height ) );

    if ( in_hover && was_m1_pressed )
        *open = !*open;
    else if ( !in_hover && was_m1_pressed && !dropdown_hovered )
        *open = false;

    Vertex_t Verts2[3];
    Verts2[ 0 ].m_Position.x = draw_pos.x + current_x_add + current_section_components_width - 11 - 10;
    Verts2[ 0 ].m_Position.y = draw_pos.y + current_y_add + 9;
    Verts2[ 1 ].m_Position.x = draw_pos.x + current_x_add + current_section_components_width - 11 - 5;
    Verts2[ 1 ].m_Position.y = draw_pos.y + current_y_add + 9;
    Verts2[ 2 ].m_Position.x = draw_pos.x + current_x_add + current_section_components_width - 11 - 7.5;
    Verts2[ 2 ].m_Position.y = draw_pos.y + current_y_add + 12;

    g_render.polygon ( 3, Verts2, Color ( 92, 92, 92, 255 ) );

    //
    // maybe remove spacing and add outline
    //
    if ( *open )
    {
        interfaces::vgui_surface->clipping_enabled( ) = false;
        last_dropdown_wh.h = item_window_height;
        last_dropdown_wh.w = current_section_components_width - 15 - 15;
        last_dropdown_pos.y = draw_pos.y + current_y_add + 1 + 21 + 2;
        last_dropdown_pos.x = draw_pos.x + current_x_add + 1 + 4;
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15 - 1, draw_pos.y + current_y_add + 1 + 21 + 2 - 1,
                                 current_section_components_width - 2 - 15 - 15 + 2, item_window_height + 2, Color ( 1, 1, 1, 255 ) );
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1 + 21 + 2,
                                 current_section_components_width - 2 - 15 - 15, item_window_height, Color ( 60, 60, 60, 255 ) );

        for ( size_t i = 0; i < items.size( ); i++ )
        {
            bool item_in_hover = g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 1 + 4 + 15,
                                                                    draw_pos.y + current_y_add + 22 + ( i * 20 ) + 1,
                                                                    current_section_components_width - 1 - 15 - 15, 20 - 2 ) );
            Color colorhover = *selected == i ? text_selected : Color ( 255, 0, 0, 255 );
            Color colorhovermouse = *selected == i ? text_selected : Color ( 200, 200, 200, 255 );
            auto item = items[ i ];

            if ( item_in_hover )
                //		g_render.text(g_fonts.menu12, "<", draw_pos.x + current_x_add + current_section_components_width - 20, draw_pos.y + current_y_add + 24 + (i * 20) - 1, colorhover);

                pos_vec next_text_pos = get_text_center ( g_fonts.menu12, item, current_section_components_width - 15 - 15, 20 );

            Color text_clr1 = *selected == i ? g_options.menu_color1 : Color ( 255, 255, 255, 255 );
            g_render.text ( g_fonts.menu12, item, draw_pos.x + current_x_add + 1 + 12 + 15, next_text_pos.y + 22 + ( i * 20 ), text_clr1 );

            if ( item_in_hover )
                g_render.text ( g_fonts.menu12, item, draw_pos.x + current_x_add + 1 + 12 + 15, draw_pos.y + current_y_add + 22 + ( i * 20 ) + 3,
                                colorhovermouse );

            if ( item_in_hover && was_m1_pressed )
            {
                if ( close_on_select )
                    *open = !open;

                *selected = i;
            }
        }

        interfaces::vgui_surface->clipping_enabled( ) = true;
    }

    add_size ( 0, 20 );
}

void c_hades_menu::select_dropdown ( std::string title, std::vector< std::string > items, std::vector< bool* > selected, bool* open,
                                     bool enable_interaction )
{
    if ( items.empty( ) )
        throw std::exception ( xor_str ( "item size == 0" ) );

    if ( should_render ( pos_vec ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add ) ) )
    {
        g_render.text ( g_fonts.menu12, std::move ( title ), draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add,
                        Color ( 255, 255, 255, 255 ) );
        add_size ( 0, 10 );
    }
    else
        add_size ( 0, 10 );

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 20 );
        return;
    }

    bool in_hover = enable_interaction && g_cursor.in_vector_wh (
        Vector4D ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20 )
    );
    //!was_m1_last_down && g_inputsys.is_key_down ( VK_LBUTTON ) && g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 ); //g_cursor.in_vector_wh ( /*Vector4D ( draw_pos.x + current_x_add + 4, draw_pos.y + current_y_add, current_section_components_width, 20 )*/last_hover );
    bool in_click = in_hover && g_inputsys.is_key_down ( VK_LBUTTON );

    g_render.filled_box_wh ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, current_section_components_width - 15 - 15, 20, outline );
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1, current_section_components_width - 2 - 15 - 15, 18,
                             Color ( 60, 60, 60, 255 ) );

    std::string text = "";

    std::string pre_old_text = "";

    for ( size_t i = 0; i < items.size( ); i++ )
    {
        std::string old_text = text;

        if ( *selected[ i ] )
        {
            if ( text.empty( ) )
                text = items[ i ];
            else
                text += ", " + items[ i ];

            Vector2D ts = g_render.get_size ( g_fonts.menu12, text );

            if ( ts.x > ( current_section_components_width - 15 - 15 - 12 ) )
            {
                Vector2D ts2 = g_render.get_size ( g_fonts.menu12, old_text + ", ..." );

                if ( ts.x > ( current_section_components_width - 15 - 15 - 12 ) )
                    text = pre_old_text + ", ...";
                else
                    text = old_text + ", ...";

                break;
            }

            pre_old_text = old_text;
        }
    }

    pos_vec next_text_pos = get_text_center ( g_fonts.menu12, text, current_section_components_width - 15 - 15, 20 );
    g_render.text ( g_fonts.menu12, text, draw_pos.x + current_x_add + 1 + 12 + 15, next_text_pos.y, Color ( 255, 255, 255, 255 ) );

    int item_window_height = items.size( ) * 20;
    last_dropdown_wh = { 0, 0 };
    last_dropdown_pos = { 0, 0 };
    bool dropdown_hovered = g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1 + 18 + 2,
                                                               current_section_components_width - 2 - 15 - 15, item_window_height ) );

    if ( in_hover && was_m1_pressed )
        *open = !*open;
    else if ( !in_hover && was_m1_pressed && !dropdown_hovered )
        *open = false;

    //
    // maybe remove spacing and add outline
    //
    if ( *open )
    {
        interfaces::vgui_surface->clipping_enabled( ) = false;
        last_dropdown_wh.h = item_window_height;
        last_dropdown_wh.w = current_section_components_width - 15 - 15;
        last_dropdown_pos.y = draw_pos.y + current_y_add + 1 + 21 + 2;
        last_dropdown_pos.x = draw_pos.x + current_x_add + 1 + 4;
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15 - 1, draw_pos.y + current_y_add + 1 + 21 + 2 - 1,
                                 current_section_components_width - 2 - 15 - 15 + 2, item_window_height + 2, Color ( 1, 1, 1, 255 ) );
        g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 1 + 21 + 2,
                                 current_section_components_width - 2 - 15 - 15, item_window_height, Color ( 60, 60, 60, 255 ) );

        for ( size_t i = 0; i < items.size( ); i++ )
        {
            bool item_in_hover = g_cursor.in_vector_wh ( Vector4D ( draw_pos.x + current_x_add + 1 + 4 + 15,
                                                                    draw_pos.y + current_y_add + 22 + ( i * 20 ) + 1,
                                                                    current_section_components_width - 1 - 15, 20 - 2 ) );

            //    g_render.filled_box_wh ( draw_pos.x + current_x_add + 1 + 4 + 15, draw_pos.y + current_y_add + 22 + ( i * 20 ), current_section_components_width - 2 - 15 - 15, 20, Color ( 33, 33, 33, 255 ) );

            auto item = items[ i ];

            pos_vec next_text_pos = get_text_center ( g_fonts.menu12, item, current_section_components_width - 15 - 15, 20 );
            Color colorhovermouse = *selected[ i ] ? text_selected : Color ( 200, 200, 200, 255 );
            Color text_clr1 = *selected[ i ] ? g_options.menu_color1 : Color ( 255, 255, 255, 255 );

            g_render.text ( g_fonts.menu12, item, draw_pos.x + current_x_add + 1 + 12 + 15, next_text_pos.y + 22 + ( i * 20 ), text_clr1 );

            if ( item_in_hover )
                g_render.text ( g_fonts.menu12, item, draw_pos.x + current_x_add + 1 + 12 + 15, draw_pos.y + current_y_add + 22 + ( i * 20 ) + 3,
                                text_clr1 );

            if ( item_in_hover )
                g_render.text ( g_fonts.menu12, item, draw_pos.x + current_x_add + 1 + 12 + 15, draw_pos.y + current_y_add + 22 + ( i * 20 ) + 3,
                                colorhovermouse );

            if ( item_in_hover && was_m1_pressed )
                *selected[ i ] = !*selected[ i ];
        }

        interfaces::vgui_surface->clipping_enabled( ) = true;
    }

    Vertex_t Verts2[3];
    Verts2[ 0 ].m_Position.x = draw_pos.x + current_x_add + current_section_components_width - 11 - 10;
    Verts2[ 0 ].m_Position.y = draw_pos.y + current_y_add + 9;
    Verts2[ 1 ].m_Position.x = draw_pos.x + current_x_add + current_section_components_width - 11 - 5;
    Verts2[ 1 ].m_Position.y = draw_pos.y + current_y_add + 9;
    Verts2[ 2 ].m_Position.x = draw_pos.x + current_x_add + current_section_components_width - 11 - 7.5;
    Verts2[ 2 ].m_Position.y = draw_pos.y + current_y_add + 12;

    g_render.polygon ( 3, Verts2, Color ( 92, 92, 92, 255 ) );

    add_size ( 0, 20 );
}

void c_hades_menu::dropdown_color ( ) const
{
}

void c_hades_menu::title ( std::string text )
{
    g_render.text ( g_fonts.menu14, std::move ( text ), draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, Color ( 255, 255, 255, 255 ) );
    add_size ( 0, 14 );
}

void c_hades_menu::label ( std::string text )
{
    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, 12 );
        return;
    }

    g_render.text ( g_fonts.menu12, std::move ( text ), draw_pos.x + current_x_add + 4 + 15, draw_pos.y + current_y_add, Color ( 255, 255, 255, 255 ) );
    add_size ( 0, 12 );
}

void c_hades_menu::set_section_count ( int n )
{
    reset_sections( );
    current_section_count = n;
    current_section_width = static_cast< int > ( std::ceil ( ( menu_wh.x - ( 120 + current_section_count * 4 ) ) / current_section_count ) );

    if ( n == 3 )
        current_section_width += 1;

    //section_height =

    current_section_components_width = current_section_width - 8;
    current_y_add += 4;

    section_original_x_add = current_x_add;
    section_original_y_add = current_y_add;
}

void c_hades_menu::reset_sections ( )
{
    current_section_count = 1;
    current_draw_section = 0;
    current_section_width = 0;
    last_dropdown_wh = { 0, 0 };
    last_dropdown_pos = { 0, 0 };
}

void c_hades_menu::next_section ( )
{
    current_draw_section++;
    current_section_start_height = 0.f;
    current_section_height = 0.f;

    current_y_add = section_original_y_add;

    if ( current_draw_section > current_section_count )
        std::exception ( xor_str ( "more sections drawing than set" ) );

    current_x_add += current_section_width + 4;
}

void c_hades_menu::section ( float height_end )
{
    int height = ( menu_wh.y - 36 ) * height_end;

    int x_start = ( draw_pos.x + section_original_x_add ) + ( current_draw_section * current_section_width - 1 ) + ( 4 * current_draw_section );
    g_render.filled_box_wh ( x_start - 1, draw_pos.y + section_y_offset - 1, current_section_width - 1 + 2, height + 2, Color ( 1, 1, 1, 255 ) );
    g_render.filled_box_wh ( x_start, draw_pos.y + section_y_offset, current_section_width - 1, height, Color ( 19, 19, 19, 255 ) );

    current_section_height += height_end;
    current_section_start_height += height_end;
}

void c_hades_menu::dubble_section ( )
{
    int height = menu_wh.y - 36;

    int x_start = ( draw_pos.x + section_original_x_add ) + ( current_draw_section * current_section_width ) + ( 4 * current_draw_section );

    g_render.filled_box_wh ( x_start - 1, draw_pos.y + section_y_offset - 1, current_section_width + 2, height + 2, Color ( 1, 1, 1, 255 ) );
    g_render.filled_box_wh ( x_start, draw_pos.y + section_y_offset, current_section_width, height, Color ( 19, 19, 19, 255 ) );

    current_section_height += 1.f;
    current_section_start_height += 1.f;
}

void c_hades_menu::trimline ( )
{
    Vector2D text_size = g_render.get_size ( g_fonts.menu12, "test" );

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 5, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, text_size.y );
        return;
    }

    int y = static_cast< int > ( text_size.y / 2 ) + draw_pos.y + current_y_add;
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 5, y, current_section_components_width - 2, 1, Color ( 50, 50, 50, 255 ) );

    add_size ( 0, text_size.y );
}

void c_hades_menu::trimline ( const std::string& text )
{
    Vector2D text_size = g_render.get_size ( g_fonts.menu12, text );

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 5, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, text_size.y );
        return;
    }

    int y = static_cast< int > ( text_size.y / 2 ) + draw_pos.y + current_y_add;
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 5 + 1, y, 6, 1, Color ( 50, 50, 50, 255 ) );
    g_render.text ( g_fonts.menu12, text, draw_pos.x + current_x_add + 5 + 8 + 4 + 1, draw_pos.y + current_y_add, Color ( 255, 255, 255 ) );
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 5 + 14 + text_size.x + 2, y, current_section_components_width - 4 - text_size.x - 14 - 1, 1,
                             Color ( 50, 50, 50, 255 ) );

    add_size ( 0, text_size.y );
}

void c_hades_menu::trimline_title ( const std::string& text )
{
    Vector2D text_size = g_render.get_size ( g_fonts.menu14, text );

    if ( !should_render ( pos_vec ( draw_pos.x + current_x_add + 5, draw_pos.y + current_y_add ) ) )
    {
        add_size ( 0, text_size.y );
        return;
    }

    int y = static_cast< int > ( text_size.y / 2 ) + draw_pos.y + current_y_add;
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 5, y, 6, 1, Color ( 50, 50, 50, 255 ) );
    g_render.text ( g_fonts.menu14, text, draw_pos.x + current_x_add + 5 + 8 + 4, draw_pos.y + current_y_add, Color ( 255, 255, 255 ) );
    g_render.filled_box_wh ( draw_pos.x + current_x_add + 5 + 14 + text_size.x + 2, y, current_section_components_width - 4 - text_size.x - 14, 1,
                             Color ( 50, 50, 50, 255 ) );

    add_size ( 0, text_size.y );
}

void c_hades_menu::keybind ( int* key )
{
    std::string text;

    if ( *key <= 0 )
        text = "[ ]";
    else
        text = "[" + get_name_for_key ( *key ) + "]";

    //console::write_line ( "key: " + std::to_string ( *key )  + " text: " + text );

    Vector2D text_size = g_render.get_size ( g_fonts.menu10, text );

    int x = draw_pos.x + current_x_add + current_section_width - ( 15 + static_cast< int > ( text_size.x / 2 ) );
    int y = draw_pos.y + last_element_y - static_cast< int > ( text_size.y / 2 ) + 5;

    if ( !should_render ( pos_vec ( x, y ) ) )
        return;

    bool in_hover = g_cursor.in_vector_wh ( Vector4D ( x, y, text_size.x, text_size.y ) );

    if ( in_hover && was_m1_pressed )
        keybind_selected = keybinds_rendered;

    if ( keybind_cancel )
    {
        *key = -1;
        keybind_last_key_selected = -1;
        keybind_selected = -1;
        keybind_cancel = false;
    }

    if ( keybind_selected == keybinds_rendered && keybind_last_key_selected != -1 )
    {
        //console::write_line ( "settings key to :" + std::to_string ( keybind_last_key_selected ) );
        *key = keybind_last_key_selected;
        keybind_last_key_selected = -1;
        keybind_selected = -1;
    }

    g_render.text ( g_fonts.menu10, text, x,
                    y /* + 2 bc of font size*/ /*- static_cast<int> ( text_size.y / 2 )*/,
                    keybinds_rendered == keybind_selected ? Color ( 255, 255, 255, 255 ) : Color ( 150, 150, 150, 255 ) ); //last_element_y

    keybinds_rendered++;
}

void c_hades_menu::spacing ( )
{
    add_size ( 0, 4 );
}

bool c_hades_menu::should_render ( pos_vec v ) const
{
    //last_dropdown_height = item_window_height;
    //last_dropdown_y = draw_pos.y + current_y_add + 1 + 18 + 2;

    return ! ( ( last_dropdown_pos.y + last_dropdown_wh.h ) > v.y && ( last_dropdown_pos.x + last_dropdown_wh.w ) > v.x );
}

std::string c_hades_menu::get_name_for_key ( int key )
{
    switch ( key )
    {
    case 0x3:
        return "mb";

    case 0x4:
        return "mb";

    case 0x5:
        return "m4";

    case 0x6:
        return "m5";

    case 0x10:
        return "shift";

    case 0x11:
        return "ctrl";

    case 0x30:
        return "0";

    case 0x31:
        return "1";

    case 0x32:
        return "2";

    case 0x33:
        return "3";

    case 0x34:
        return "4";

    case 0x35:
        return "5";

    case 0x36:
        return "6";

    case 0x37:
        return "7";

    case 0x38:
        return "8";

    case 0x39:
        return "9";

    case 0x41:
        return "a";

    case 0x42:
        return "b";

    case 0x43:
        return "c";

    case 0x44:
        return "d";

    case 0x45:
        return "e";

    case 0x46:
        return "f";

    case 0x47:
        return "g";

    case 0x48:
        return "h";

    case 0x49:
        return "i";

    case 0x4A:
        return "j";

    case 0x4B:
        return "k";

    case 0x4C:
        return "l";

    case 0x4D:
        return "m";

    case 0x4E:
        return "n";

    case 0x4F:
        return "o";

    case 0x50:
        return "p";

    case 0x51:
        return "q";

    case 0x52:
        return "r";

    case 0x53:
        return "s";

    case 0x54:
        return "t";

    case 0x55:
        return "u";

    case 0x56:
        return "v";

    case 0x57:
        return "w";

    case 0x58:
        return "x";

    case 0x59:
        return "y";

    case 0x5A:
        return "z";

    default:
        return "";
    }
}

void c_hades_menu::hsv_to_rgb ( float h, float s, float v, float& out_r, float& out_g, float& out_b ) const
{
    if ( s == 0.0f )
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = std::fmod ( h, 1.0f ) / ( 60.0f / 360.0f );
    int i = ( int )h;
    float f = h - ( float )i;
    float p = v * ( 1.0f - s );
    float q = v * ( 1.0f - s * f );
    float t = v * ( 1.0f - s * ( 1.0f - f ) );

    switch ( i )
    {
    case 0:
        out_r = v;
        out_g = t;
        out_b = p;
        break;

    case 1:
        out_r = q;
        out_g = v;
        out_b = p;
        break;

    case 2:
        out_r = p;
        out_g = v;
        out_b = t;
        break;

    case 3:
        out_r = p;
        out_g = q;
        out_b = v;
        break;

    case 4:
        out_r = t;
        out_g = p;
        out_b = v;
        break;

    case 5:
    default:
        out_r = v;
        out_g = p;
        out_b = q;
        break;
    }
}

Color c_hades_menu::hsv_to_rgb ( float h, float s, float v )
{
    float r = 255.f, g = 255.f, b = 255.f;

    hsv_to_rgb ( h, s, v, r, g, b );

    return Color ( r, g, b, 255.f );
}

bool hades_cursor::in_vector ( Vector4D next_vector, pos_vec current_pos ) const
{
    if ( current_pos.x < next_vector.x )
        return false;

    if ( current_pos.x > next_vector.z )
        return false;

    if ( current_pos.y < next_vector.y )
        return false;

    if ( current_pos.y > next_vector.w )
        return false;

    return true;
}

void c_hades_menu::add_size ( int x, int y )
{
    last_element_y = current_y_add;
    current_x_add += x;
    current_y_add += y + spacing_y;
}

pos_vec c_hades_menu::get_text_center ( vfont font, std::string text, int w, int h )
{
    Vector2D text_size = g_render.get_size ( std::move ( font ), std::move ( text ) );
    int x_center = draw_pos.x + current_x_add + ( static_cast< int > ( w / 2.f ) - static_cast< int > ( text_size.x / 2.f ) );
    int y_center = draw_pos.y + current_y_add + static_cast< int > ( ( h - text_size.y ) / 2.f );

    return pos_vec ( x_center, y_center );
}

void hades_cursor::render ( ) const
{
    const auto x = this->cursor_pos.x;
    const auto y = this->cursor_pos.y;

    g_render.filled_box_wh ( x + 1, y, 1, 17, Color ( 3, 6, 26, 255 ) );

    for ( int i = 0; i < 11; i++ )
        g_render.filled_box_wh ( x + 2 + i, y + 1 + i, 1, 1, Color ( 3, 6, 26, 255 ) );

    g_render.filled_box_wh ( x + 8, y + 12, 5, 1, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 8, y + 13, 1, 1, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 9, y + 14, 1, 2, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 10, y + 16, 1, 2, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 8, y + 18, 2, 1, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 7, y + 16, 1, 2, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 6, y + 14, 1, 2, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 5, y + 13, 1, 1, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 4, y + 14, 1, 1, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 3, y + 15, 1, 1, Color ( 3, 6, 26, 255 ) );
    g_render.filled_box_wh ( x + 2, y + 16, 1, 1, Color ( 3, 6, 26, 255 ) );

    for ( int i = 0; i < 4; i++ )
        g_render.filled_box_wh ( x + 2 + i, y + 2 + i, 1, 14 - ( i * 2 ), g_options.menu_color1 );

    g_render.filled_box_wh ( x + 6, y + 6, 1, 8, g_options.menu_color1 );
    g_render.filled_box_wh ( x + 7, y + 7, 1, 9, g_options.menu_color1 );

    for ( int i = 0; i < 4; i++ )
        g_render.filled_box_wh ( x + 8 + i, y + 8 + i, 1, 4 - i, g_options.menu_color1 );

    g_render.filled_box_wh ( x + 8, y + 14, 1, 4, g_options.menu_color1 );
    g_render.filled_box_wh ( x + 9, y + 16, 1, 2, g_options.menu_color1 );

    ///TODO: render this fucker to texture first and stop wasting time rendering that
    // Draw inner fill color
    //SPoint ptPos1 = { x + 1,  y + 1 };
    //SPoint ptPos2 = { x + 25, y + 12 };
    //SPoint ptPos3 = { x + 12, y + 25 };
    //g_Render.TriangleFilled ( ptPos1, ptPos2, ptPos3, MenuMain::style.colCursor );

    // Draw second smaller inner fill color
    //ptPos1 = { x + 6,  y + 6 };
    //ptPos2 = { x + 19, y + 12 };
    //ptPos3 = { x + 12, y + 19 };
    //g_Render.TriangleFilled ( ptPos1, ptPos2, ptPos3, MenuMain::style.colCursor );

    // Draw border
    //g_render.Triangle ( { x, y }, { x + 25, y + 12 }, { x + 12, y + 25 }, Color::Black ( 200 ) );
}

void hades_cursor::run_think ( UINT u_msg, LPARAM lparam )
{
    switch ( u_msg )
    {
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
        this->set_pos ( GET_X_LPARAM ( lparam ), GET_Y_LPARAM ( lparam ) );
        break;

        /*
        case WM_LBUTTONDOWN:
        this->bLMBPressed = true;
        break;
    
        case WM_LBUTTONUP:
        this->bLMBHeld = false;
        this->bLMBPressed = false;
        break;
    
        case WM_RBUTTONDOWN:
        this->bRMBPressed = true;
        break;
    
        case WM_RBUTTONUP:
        this->bRMBHeld = false;
        this->bRMBPressed = false;
        break;
        */

    default:
        break;
    }
}

bool hades_cursor::in_vector ( Vector4D v ) const
{
    return in_vector ( v, this->cursor_pos );
}

bool hades_cursor::in_vector_wh ( Vector4D v ) const
{
    return in_vector ( Vector4D ( v.x, v.y, v.x + v.z, v.y + v.w ), this->cursor_pos );
}
