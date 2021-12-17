#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>

#include "visuals.hpp"
#include "../options.h"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../features/ragebot/aimbot/autowall.h"
#include "../features/misc/event_logger.h"
#include "../renderer/vgui_render.h"
#include "features.h"

RECT get_bbox ( C_BaseEntity* ent )
{
    const RECT rect{ };
    const auto collideable = ent->get_collideable( );

    if ( !collideable )
        return rect;

    const auto min = collideable->obb_mins( );
    const auto max = collideable->obb_maxs( );

    const auto& trans = ent->rgfl_coordinate_frame( );

    const auto vector_transform = [ ] ( Vector vector, matrix3x4_t matrix )
    {
        return Vector ( vector.dot ( matrix[ 0 ] ) + matrix[ 0 ][ 3 ], vector.dot ( matrix[ 1 ] ) + matrix[ 1 ][ 3 ],
                        vector.dot ( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
    };

    Vector points[ ] =
    {
        Vector ( min.x, min.y, min.z ),
        Vector ( min.x, max.y, min.z ),
        Vector ( max.x, max.y, min.z ),
        Vector ( max.x, min.y, min.z ),
        Vector ( max.x, max.y, max.z ),
        Vector ( min.x, max.y, max.z ),
        Vector ( min.x, min.y, max.z ),
        Vector ( max.x, min.y, max.z )
    };

    Vector points_transformed[ 8 ];

    for ( auto i = 0; i <= 7; i++ )
    {
        if ( !g_math.world_to_screen ( vector_transform ( points[ i ], trans ), points_transformed[ i ] ) )
            return rect;
    }

    Vector box_array[ ] =
    {
        points_transformed[ 3 ], // Front left
        points_transformed[ 5 ], // Back right
        points_transformed[ 0 ], // Back left
        points_transformed[ 4 ], // Front right
        points_transformed[ 2 ], // Front right
        points_transformed[ 1 ], // Back right
        points_transformed[ 6 ], // Back left
        points_transformed[ 7 ] // Front Left
    };

    auto left = points_transformed[ 3 ].x,
         bottom = points_transformed[ 3 ].y,
         right = points_transformed[ 3 ].x,
         top = points_transformed[ 3 ].y;

    for ( auto i = 0; i <= 7; i++ )
    {
        if ( left > box_array[ i ].x )
            left = box_array[ i ].x;

        if ( bottom < box_array[ i ].y )
            bottom = box_array[ i ].y;

        if ( right < box_array[ i ].x )
            right = box_array[ i ].x;

        if ( top > box_array[ i ].y )
            top = box_array[ i ].y;
    }

    return RECT{ static_cast< long > ( left ), static_cast< long > ( top ), static_cast< long > ( right ), static_cast< long > ( bottom ) };
}

c_visuals::c_visuals ( )
{
    InitializeCriticalSection ( &cs_ );
}

c_visuals::~c_visuals ( )
{
    DeleteCriticalSection ( &cs_ );
}

Vector2D c_visuals::get_screensize ( ) const
{
    return Vector2D ( screen_x_, screen_y_ );
}

void c_visuals::update_screensize ( )
{
    interfaces::engine_client->get_screen_size ( screen_x_, screen_y_ );
}

//--------------------------------------------------------------------------------
bool c_visuals::Player::begin ( C_BasePlayer* pl )
{
    if ( !pl->is_alive( ) )
        return false;

    //if ( pl->is_dormant ( ) || !pl->is_alive ( ) )
    //    return false;

    const auto i = pl->ent_index( );

    if ( pl->is_dormant( ) )
    {
        if ( !g_features.visuals.dormant_data[ i ].was_last )
        {
            g_features.visuals.dormant_data[ i ].time = interfaces::global_vars->curtime;
            g_features.visuals.dormant_data[ i ].was_last = true;
        }

        auto delta = interfaces::global_vars->curtime - g_features.visuals.dormant_data[ i ].time;

        if ( delta > 1.f )
            delta = 1.f;

        ctx.percent = 1.f - delta / 2.f;
        ctx.clr = Color ( 255, 255, 255, 25 + static_cast< int > ( ctx.percent * 230 ) );
    }
    else
    {
        ctx.percent = 1.f;
        g_features.visuals.dormant_data[ i ].was_last = false;
        ctx.clr = Color ( 255, 255, 255, 255 );
    }

    ctx.pl = pl;
    ctx.is_enemy = g_local->team_num( ) != pl->team_num( );
    auto head = pl->get_hitbox_pos ( hitbox_head );
    ctx.is_visible = g_local->can_see_player ( pl, head );

    //if ( !ctx.is_enemy && g_Options.esp_enemies_only )
    //    return false;
    /*ctx.is_enemy ? ( ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded ) : ( ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded );*/

    ctx.bbox = get_bbox ( ctx.pl );

    const auto origin = pl->vec_origin( );

    head.z += 7;

    if ( !g_math.world_to_screen ( head, ctx.head_pos ) || !g_math.world_to_screen ( origin, ctx.origin_pos ) || !g_math.world_to_screen (
        pl->get_eye_pos( ), ctx.eye_pos ) )
        return false;

    return true;
}

//--------------------------------------------------------------------------------
void c_visuals::Player::render_box ( ) const
{
    g_render.box ( ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1,
                   Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 230 ) ), 1.f );
    g_render.box ( ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1,
                   Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 230 ) ), 1.f );
    g_render.box ( ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, ctx.clr, 1.f );
}

//-------------------------------------------------------------------------------
void c_visuals::Player::draw_crosshair ( )
{
    const auto local = static_cast< C_BasePlayer* > ( g_local );

    if ( !local || !local->is_alive( ) )
        return;

    static const auto crosshair = interfaces::cvar->find_var ( xor_str ( "weapon_debug_spread_show" ) );
    crosshair->m_fnChangeCallbacks.m_Size = 0;

    if ( g_options.visuals_crosshair && !local->is_scoped( ) )
        crosshair->set_value ( 3 );
    else
        crosshair->set_value ( 0 );
}

//--------------------------------------------------------------------------------
void c_visuals::Player::render_name ( ) const
{
    player_info_t info = ctx.pl->get_player_info( );

    const std::string name = info.sz_name;
    auto name_string = ( name.length( ) > 0 ? name : xor_str ( "empty" ) );

    if ( name_string.size( ) > 12 )
        name_string.resize ( 12 );

    const auto pos = g_render.get_size ( g_fonts.normal, name_string );

    g_render.text ( g_fonts.normal, name_string, ctx.bbox.left + ( ctx.bbox.right - ctx.bbox.left ) * 0.5 - pos.x * 0.5, ctx.bbox.top - pos.y,
                    Color ( 255, 255, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );
}

//--------------------------------------------------------------------------------
void c_visuals::Player::render_health ( )
{
    auto health = ctx.pl->health( );

    if ( health > 100 )
        health = 100;

    const auto box_h = static_cast< float > ( fabs ( ctx.bbox.bottom - ctx.bbox.top ) );
    const auto off = 8 + ctx.left_offset;

    ctx.left_offset += 4;

    const auto height = box_h - box_h * health / 100;

    const auto x = ctx.bbox.left - off;
    const auto y = ctx.bbox.top;
    const auto w = 3;
    const auto h = box_h + 1;

    g_render.box ( x + 2, y - 1, x + w - 1 + 2, y + h, Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 185 ) ), 1.f );
    g_render.filled_box ( x + 1 + 2, y, x + w - 1 + 2, y + h, Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 150 ) ) );
    g_render.filled_box ( x + 1 + 2, y + height, x + w - 1 + 2, y + h, Color ( 0, 255, 0, 25 + static_cast< int > ( ctx.percent * 230 ) ) );
    //   g_render.text ( g_fonts.normal_small, std::to_string ( health ), x + 1, y + height, Color ( 255, 255, 255 ) );
}

void c_visuals::Player::render_armor ( )
{
    auto armor = ctx.pl->armor_value( );

    if ( armor > 100 )
        armor = 100;

    const auto box_h = static_cast< float > ( fabs ( ctx.bbox.bottom - ctx.bbox.top ) );
    const auto off = 8 + ctx.left_offset;
    ctx.left_offset += 4;

    const auto height = box_h - box_h * armor / 100;

    const auto x = ctx.bbox.left - off;
    const auto y = ctx.bbox.top;
    const auto w = 3;
    const auto h = box_h + 1;

    g_render.box ( x + 2, y - 1, x + w - 1 + 2, y + h, Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 185 ) ), 1.f );
    g_render.filled_box ( x + 1 + 2, y, x + w - 1 + 2, y + h, Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 150 ) ) );
    g_render.filled_box ( x + 1 + 2, y + height, x + w - 1 + 2, y + h, Color ( 0, 170, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );
}

void c_visuals::Player::render_ammo ( ) const
{
    C_BaseCombatWeapon* weapon = ctx.pl->active_weapon( );

    if ( !weapon )
        return;

    const auto ammo = weapon->clip1( );

    const auto x = ctx.bbox.left;
    const auto y = ctx.bbox.bottom + 4;

    g_render.box_wh ( x - 1, y, ctx.bbox.right - ctx.bbox.left + 2, 2, Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 185 ) ), 1.f );
    g_render.filled_box_wh ( x, y + 1, ctx.bbox.right - ctx.bbox.left + 1, 1, Color ( 0, 0, 0, 25 + static_cast< int > ( ctx.percent * 150 ) ) );
    g_render.filled_box_wh ( x, y + 1, ammo / static_cast< float > ( weapon->get_cs_weapon_data( )->max_clip1 ) * ( ctx.bbox.right - ctx.bbox.left + 1 ), 1,
                             Color ( 60, 170, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );
}

void c_visuals::Player::render_weapon ( ) const
{
    C_BaseCombatWeapon* weapon = ctx.pl->active_weapon( ).get( );

    if ( !weapon )
        return;

    const auto text = g_features.visuals.get_name_for_weapon ( weapon );
    const auto size = g_render.get_size ( g_fonts.normal, text );
    g_render.text ( g_fonts.normal, text, ctx.origin_pos.x - size.x / 2, ctx.bbox.bottom + 2 + 4,
                    Color ( 255, 255, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );
}

void c_visuals::Player::render_flags ( )
{
    auto add = 0;
    const auto add_flag = [&] ( std::string str, Color clr ) -> void
    {
        g_render.text ( g_fonts.normal, str, ctx.bbox.right + 4, ctx.bbox.top + add - 1, clr );
        add += 12;
    };

    if ( g_options.esp_enemy_flag_scoped && ctx.pl->is_scoped( ) )
        add_flag ( xor_str ( "scoped" ), Color ( 225, 255, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );

    if ( g_options.esp_enemy_flag_fake && g_features.resolver.rdata.at ( ctx.pl->ent_index( ) ).desyncing )
        add_flag ( xor_str ( "fake" ), Color ( 255, 255, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );

    if ( g_options.esp_enemy_flag_reloading && ctx.pl->active_weapon( ).get( ) && ctx.pl->active_weapon( ).get( )->is_reloading( ) )
        add_flag ( xor_str ( "reloading" ), Color ( 225, 255, 255, 25 + static_cast< int > ( ctx.percent * 230 ) ) );

    //if ( g_options.esp_enemy_flag_shift )
    //    add_flag ( xor_str ( "tick shifting" ), Color ( 225, 255, 255, 25 + static_cast<int> ( ctx.percent * 230 ) ) );
}

void c_visuals::Player::draw_damage ( C_BaseEntity* ent ) const
{
    for ( auto j = g_features.visuals.damage_data.at ( ent->ent_index( ) ).begin( ); j != g_features.visuals.damage_data.at ( ent->ent_index( ) ).end( );
          ++j )
    {
        const auto delta = interfaces::global_vars->realtime - j->start_time;

        if ( delta > 2.f )
        {
            g_features.visuals.damage_data.at ( ent->ent_index( ) ).erase ( j );

            if ( !g_features.visuals.damage_data.at ( ent->ent_index( ) ).empty( ) )
                j = g_features.visuals.damage_data.at ( ent->ent_index( ) ).begin( );
            else
                break;
        }

        const auto p = 1.f - ( delta / 2.f );

        Vector tmp;

        if ( g_math.world_to_screen ( j->position, tmp ) )
        {
            g_render.text ( g_fonts.font_16, std::to_string ( j->damage ), tmp.x, tmp.y - ( 25.f * ( 1.f - p ) ),
                            Color ( 255, 255, 255, std::clamp ( static_cast< int > ( 255.f * p ), 0, 255 ) ) );

            if ( j->hs )
            {
                g_render.text ( g_fonts.font_32, xor_str ( "HEADSHOT" ), tmp.x + j->hs_add, tmp.y - j->hs_add_x - ( 35.f * ( 1.f - p ) ),
                                Color ( 0, 255, 0, std::clamp ( static_cast< int > ( 255.f * p ), 0, 255 ) ) );
            }
        }
    }
}

void c_visuals::render_ground_weapon ( C_BaseCombatWeapon* ent ) const
{
    if ( !g_local )
        return;

    if ( ent->owner_entity( ).IsValid( ) || ent->is_dormant( ) )
        return;

    const auto render_origin = ent->get_render_origin( );
    Vector out;

    if ( !g_math.world_to_screen ( render_origin, out ) )
        return;

    const auto text = g_features.visuals.get_name_for_weapon ( ent );

    const auto text_size = g_render.get_size ( g_fonts.itemesp, text );
    const auto dist = g_local->get_eye_pos( ).dist_to ( render_origin );
    auto percent = 1.f;

    if ( dist > 500.f )
    {
        const auto delta = dist - 500.f;
        percent = 1.f - delta / 100.f;
    }

    if ( percent > 0.f )
    {
        g_render.text ( g_fonts.itemesp, text, out.x - ( text_size.x / 2.f ), out.y, Color ( 255, 255, 255, static_cast< int > ( 255.f * percent ) ) );

        if ( g_options.esp_world_weapons_ammo && ent->item( ).item_definition_index( ) != WEAPON_C4 && !ent->is_grenade( ) )
        {
            g_render.box_wh ( out.x - 41, out.y + text_size.y + 2, 81, 2, Color ( 0, 0, 0, static_cast< int > ( 210.f * percent ) ), 1.f );
            const auto ammo_p = static_cast< float > ( ent->clip1( ) ) / static_cast< float > ( ent->get_cs_weapon_data( )->max_clip1 );
            g_render.filled_box_wh ( out.x - 40, out.y + text_size.y + 2 + 1, 80, 1, Color ( 0, 0, 0, static_cast< int > ( 175.f * percent ) ) );
            g_render.filled_box_wh ( out.x - 40, out.y + text_size.y + 2 + 1, 80 * ammo_p, 1,
                                     Color ( 0, 175, 255, static_cast< int > ( 255.f * percent ) ) );
        }
    }
}

std::string c_visuals::get_name_for_weapon ( C_BaseCombatWeapon* entity ) const
{
    std::string name;

    switch ( entity->item( ).item_definition_index( ) )
    {
    case WEAPON_DEAGLE:
        name = xor_str ( "Deagle" );
        break;

    case WEAPON_ELITE:
        name = xor_str ( "Dual Berettas" );
        break;

    case WEAPON_FIVESEVEN:
        name = xor_str ( "Five-SeveN" );
        break;

    case WEAPON_GLOCK:
        name = xor_str ( "Glock-18" );
        break;

    case WEAPON_AK47:
        name = xor_str ( "AK-47" );
        break;

    case WEAPON_AUG:
        name = xor_str ( "AUG" );
        break;

    case WEAPON_AWP:
        name = xor_str ( "AWP" );
        break;

    case WEAPON_FAMAS:
        name = xor_str ( "FAMAS" );
        break;

    case WEAPON_G3SG1:
        name = xor_str ( "G3SG1" );
        break;

    case WEAPON_GALIL:
        name = xor_str ( "Galil AR" );
        break;

    case WEAPON_M249:
        name = xor_str ( "M249" );
        break;

    case WEAPON_M4A4:
        name = xor_str ( "M4A4" );
        break;

    case WEAPON_MAC10:
        name = xor_str ( "MAC-10" );
        break;

    case WEAPON_P90:
        name = xor_str ( "P90" );
        break;

    case WEAPON_MP5SD:
        name = xor_str ( "MP5-SD" );
        break;

    case WEAPON_UMP45:
        name = xor_str ( "UMP-45" );
        break;

    case WEAPON_XM1014:
        name = xor_str ( "XM1014" );
        break;

    case WEAPON_BIZON:
        name = xor_str ( "PP-Bizon" );
        break;

    case WEAPON_MAG7:
        name = xor_str ( "MAG-7" );
        break;

    case WEAPON_NEGEV:
        name = xor_str ( "Negev" );
        break;

    case WEAPON_SAWEDOFF:
        name = xor_str ( "Sawed-Off" );
        break;

    case WEAPON_TEC9:
        name = xor_str ( "Tec-9" );
        break;

    case WEAPON_ZEUS:
        name = xor_str ( "Zeus x27" );
        break;

    case WEAPON_P2000:
        name = xor_str ( "P2000" );
        break;

    case WEAPON_MP7:
        name = xor_str ( "MP7" );
        break;

    case WEAPON_MP9:
        name = xor_str ( "MP9" );
        break;

    case WEAPON_NOVA:
        name = xor_str ( "Nova" );
        break;

    case WEAPON_P250:
        name = xor_str ( "P250" );
        break;

    case WEAPON_SCAR20:
        name = xor_str ( "SCAR-20" );
        break;

    case WEAPON_SG553:
        name = xor_str ( "SG 553" );
        break;

    case WEAPON_SSG08:
        name = xor_str ( "SSG 08" );
        break;

    case WEAPON_FLASHBANG:
        name = xor_str ( "Flashbang" );
        break;

    case WEAPON_HEGRENADE:
        name = xor_str ( "HE Grenade" );
        break;

    case WEAPON_SMOKEGRENADE:
        name = xor_str ( "Smoke Grenade" );
        break;

    case WEAPON_MOLOTOV:
        name = xor_str ( "Molotov" );
        break;

    case WEAPON_DECOY:
        name = xor_str ( "Decoy Grenade" );
        break;

    case WEAPON_INC:
        name = xor_str ( "Incendiary Grenade" );
        break;

    case WEAPON_C4:
        name = xor_str ( "C4" );
        break;

    case WEAPON_KNIFE:
    case WEAPON_KNIFE_T:
        name = xor_str ( "Knife" );
        break;

    case WEAPON_M4A1S:
        name = xor_str ( "M4A1-S" );
        break;

    case WEAPON_USPS:
        name = xor_str ( "USP-S" );
        break;

    case WEAPON_CZ75:
        name = xor_str ( "CZ75-Auto" );
        break;

    case WEAPON_REVOLVER:
        name = xor_str ( "R8 Revolver" );
        break;

    case WEAPON_KNIFE_BAYONET:
    case WEAPON_KNIFE_FLIP:
    case WEAPON_KNIFE_GUT:
    case WEAPON_KNIFE_KARAMBIT:
    case WEAPON_KNIFE_M9_BAYONET:
    case WEAPON_KNIFE_TACTICAL:
    case WEAPON_KNIFE_FALCHION:
    case WEAPON_KNIFE_SURVIVAL_BOWIE:
    case WEAPON_KNIFE_BUTTERFLY:
    case WEAPON_KNIFE_PUSH:
    case WEAPON_KNIFE_GYPSY_JACKKNIFE:
    case WEAPON_KNIFE_STILETTO:
    case WEAPON_KNIFE_URSUS:
    case WEAPON_KNIFE_WIDOWMAKER:
        name = xor_str ( "Knife" );
        break;

    default:
        name = xor_str ( "" );
    }

    return name;
}

std::string c_visuals::get_weapon_icon ( C_BaseCombatWeapon* entity ) const
{
    return xor_str ( "" );
}

void c_visuals::add_damage_data ( int i, int damage, Vector position, bool headshot )
{
    position.x += static_cast< int > ( g_math.random_float ( -30.f, 30.f ) );
    damage_data.at ( i ).push_back (
        {
            damage,
            interfaces::global_vars->realtime,
            position,
            headshot,
            static_cast< int > ( g_math.random_float ( - ( 32.f * 3.f ), ( 32.f * 3.f ) ) ),
            static_cast< int > ( g_math.random_float ( - ( 32.f * 2.f ), ( 32.f * 2.f ) ) )
        }
    );
}

void c_visuals::inaccuracy ( IDirect3DDevice9* m_device )
{
    C_BasePlayer* local = g_local;

    if ( !local )
        return;

    if ( !interfaces::engine_client->is_connected( ) && !interfaces::engine_client->is_in_game( ) )
        return;

    C_BaseCombatWeapon* weapon = g_local->active_weapon( );

    if ( !weapon )
        return;

    static float rot = 0.f;

    int w, h;

    interfaces::engine_client->get_screen_size ( w, h );

    w /= 2, h /= 2;

    //int r, g, b;
    //r = g_cfg.esp.show_spread_color.r( );
    //g = g_cfg.esp.show_spread_color.g( );
    //b = g_cfg.esp.show_spread_color.b( );

    g_render.circle_dual_colour ( w, h, weapon->get_inaccuracy( ) * 500.0f, rot, 1, 50, m_device );

    rot += 0.5f;

    if ( rot > 360.f )
        rot = 0.f;
}

void c_visuals::draw_offscreenesp ( ) const
{
    if ( !g_options.esp_offscreen )
        return;

    if ( !g_local || !g_local->is_alive( ) )
        return;

    const auto alpha = ( 0.8f + sin ( interfaces::global_vars->curtime * 10.f ) * .2f ) * 255.f;

    const auto size = get_screensize( );
    const auto screen_center_x = size.x / 2;
    const auto screen_center_y = size.y / 2;

    for ( auto i = 1; i <= interfaces::global_vars->max_clients; i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !entity->is_player( ) || entity->is_dormant( ) || !entity->is_alive( )
            || !entity->is_enemy( ) )
            continue;

        Vector screen_point;
        interfaces::debug_overlay->screen_position ( entity->get_abs_origin( ), screen_point );

        if ( screen_point.x < 0 || screen_point.y < 0 || screen_point.x > size.x || screen_point.y > size.y )
        {
            QAngle angle;

            g_math.vector_angles ( Vector ( screen_center_x - screen_point.x, screen_center_y - screen_point.y, 0 ), angle );
            const auto angle_yaw_rad1 = g_math.degrees_to_radians ( angle.yaw );

            const auto max_radius = screen_center_y - 100;

            const auto new_point_x = screen_center_x +
                ( g_options.visuals_invisible_players_range * max_radius + 50 ) * cosf ( angle_yaw_rad1 );
            const auto new_point_y = screen_center_y +
                ( g_options.visuals_invisible_players_range * max_radius + 50 ) * sinf ( angle_yaw_rad1 );

            std::array< Vector2D, 3 > points
            {
                Vector2D ( new_point_x - 15, new_point_y - 15 ),
                Vector2D ( new_point_x + 15, new_point_y ),
                Vector2D ( new_point_x - 15, new_point_y + 15 )
            };
            g_math.rotate_triangle ( points, angle.yaw );

            const auto tmp = g_options.visuals_offscreen_color;

            g_render.add_triangle_filled ( points, Color ( tmp.r( ), tmp.g( ), tmp.b( ), static_cast< int > ( alpha ) ) );
        }
    }
}

void c_visuals::draw_hitmarker ( ) const
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    const auto size = get_screensize( );
    const auto cx = size.x / 2;
    const auto cy = size.y / 2;

    if ( interfaces::global_vars->realtime - globals.hitmarker.time < .5f )
    {
        const auto p = ( interfaces::global_vars->realtime - globals.hitmarker.time ) / .5f;

        const auto clr = globals.hitmarker.did_kill
                             ? Color ( 255, 50, 10, 255 - static_cast< int > ( 255 * p ) )
                             : Color ( 255, 255, 255, 255 - static_cast< int > ( 255 * p ) );

        g_render.line ( cx - 10, cy - 10, cx - 6, cy - 6, clr, 1.f );
        g_render.line ( cx + 10, cy + 10, cx + 6, cy + 6, clr, 1.f );
        g_render.line ( cx - 10, cy + 10, cx - 6, cy + 6, clr, 1.f );
        g_render.line ( cx + 10, cy - 10, cx + 6, cy - 6, clr, 1.f );
    }
}

void c_visuals::draw_indicators ( )
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    const auto size = get_screensize( );

    auto height = 0;
    const auto add_indicator = [&] ( std::string text, Color clr ) -> void
    {
        g_render.text ( g_fonts.indicators, text, 10, size.y - ( 100 + height ), clr );
        height += 35;
    };

    if ( g_options.visuals_local_desync )
    {
        const auto desync_g = static_cast< int > ( std::clamp ( fabs ( g_features.animations.m_fake_delta ), 0.f, 58.f ) / 58.f * 255.f );

        const auto desync_color = Color ( 255 - desync_g, desync_g, 0, 255 );
        add_indicator ( xor_str ( "DESYNC" ), desync_color );
    }

    if ( g_options.visuals_local_lc && ( ! ( g_local->flags( ) & fl_onground ) || g_options.ragebot_fakelag_moving_ticks == 16 ) )
        add_indicator ( xor_str ( "LC" ), globals.breaking_lc ? Color ( 0, 255, 0, 255 ) : Color ( 255, 0, 0, 255 ) );

    if ( g_options.ragebot_manualaa_mode != 0 )
    {
        std::string st;

        switch ( g_options.ragebot_manualaa_mode )
        {
        case 1:
            st = xor_str ( "LEFT" );
            break;

        case 2:
            st = xor_str ( "RIGHT" );
            break;

        case 3:
            st = xor_str ( "BACK" );
            break;

        default:
            break;
        }

        add_indicator ( st, Color ( 0, 255, 0, 255 ) );
    }
}

void c_visuals::draw_scopelines ( ) const
{
    if ( !g_local || !g_local->is_alive( ) || !g_local->is_scoped( ) )
        return;

    const auto size = get_screensize( );
    const auto cx = size.x / 2;
    const auto cy = size.y / 2;

    C_BaseCombatWeapon* weapon = g_local->active_weapon( );
    auto width = 1.f;

    if ( weapon )
        width = 1.f + weapon->get_inaccuracy( ) * 200.f;

    const auto clr = Color ( 0, 0, 0, std::clamp ( 255 - static_cast< int > ( 255.f * ( width / 100.f ) ), 0, 255 ) );

    //g_render.gradient_vertical ( 0, cy, size.x, cy + static_cast<int> ( width / 2.f ), Color ( 0, 0, 0, 255 ), Color ( 0, 0, 0, 255 ) );
    //g_render.gradient_vertical ( 0, cy, size.x, cy - static_cast<int> ( width / 2.f ), Color ( 0, 0, 0, 255 ), Color ( 0, 0, 0, 255 ) );
    //g_render.gra

    const auto numb = static_cast< int > ( width );

    g_render.line ( 0, cy, size.x, cy, clr, 1 );
    g_render.line ( cx, 0, cx, size.y, clr, 1 );

    for ( auto i = 1; i < numb; ++i )
    {
        const auto p = 1.f - ( i / static_cast< float > ( numb ) );
        g_render.line ( 0, cy - i, size.x, cy - i, Color ( 0, 0, 0, static_cast< int > ( clr.a( ) * p ) ), 1 );
        g_render.line ( cx - i, 0, cx - i, size.y, Color ( 0, 0, 0, static_cast< int > ( clr.a( ) * p ) ), 1 );
    }

    for ( auto i = 1; i < numb; ++i )
    {
        const auto p = 1.f - ( i / static_cast< float > ( numb ) );
        g_render.line ( 0, cy + i, size.x, cy + i, Color ( 0, 0, 0, static_cast< int > ( clr.a( ) * p ) ), 1 );
        g_render.line ( cx + i, 0, cx + i, size.y, Color ( 0, 0, 0, static_cast< int > ( clr.a( ) * p ) ), 1 );
    }
}

void c_visuals::draw_manual_aa_indicators ( )
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    const auto size = get_screensize( );
    const auto screen_center_x = static_cast< int > ( size.x / 2.f ) + 4;
    const auto screen_center_y = static_cast< int > ( size.y / 2.f );

    const auto max_radius = screen_center_y - 100;

    const auto add_triangle = [&] ( const float rotation, const Color clr ) -> void
    {
        const auto angle_yaw_rad1 = g_math.degrees_to_radians ( rotation );
        const auto new_point_x = screen_center_x + 0.15f * max_radius * cosf ( angle_yaw_rad1 );
        const auto new_point_y = screen_center_y + 0.15f * max_radius * sinf ( angle_yaw_rad1 );

        std::array< Vector2D, 3 > points
        {
            Vector2D ( new_point_x - 10, new_point_y - 10 ),
            Vector2D ( new_point_x + 10, new_point_y ),
            Vector2D ( new_point_x - 10, new_point_y + 10 )
        };

        g_math.rotate_triangle ( points, rotation );

        g_render.add_triangle_filled ( points, clr );
    };

    if ( g_options.ragebot_manualaa_mode == 2 )
        add_triangle ( 0.f, Color ( 0, 255, 150, 200 ) );
    else
        add_triangle ( 0.f, Color ( 255, 255, 255, 125 ) );

    if ( g_options.ragebot_manualaa_mode == 1 )
        add_triangle ( 180.f, Color ( 0, 255, 150, 200 ) );
    else
        add_triangle ( 180.f, Color ( 255, 255, 255, 125 ) );

    if ( g_options.ragebot_manualaa_mode == 3 )
        add_triangle ( 90.f, Color ( 0, 255, 150, 200 ) );
    else
        add_triangle ( 90.f, Color ( 255, 255, 255, 125 ) );
}

void c_visuals::draw_angle_lines ( ) const
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    const auto pos = g_local->get_render_origin( );
    Vector tmp;

    if ( g_math.world_to_screen ( pos, tmp ) )
    {
        Vector draw_tmp;
        const Vector real_pos ( 50.f * cos ( deg2rad ( globals.real_yaw ) ) + pos.x, 50.f * sin ( deg2rad ( globals.real_yaw ) ) + pos.y, pos.z );

        if ( g_math.world_to_screen ( real_pos, draw_tmp ) )
        {
            g_render.line ( tmp.x, tmp.y, draw_tmp.x, draw_tmp.y, Color ( 0, 255, 0, 255 ), 1.f );
            g_render.text ( g_fonts.menu12, xor_str ( "real" ), draw_tmp.x, draw_tmp.y, Color ( 0, 255, 0, 255 ) );
        }

        const Vector fake_pos ( 50.f * cos ( deg2rad ( globals.fake_yaw_ ) ) + pos.x, 50.f * sin ( deg2rad ( globals.fake_yaw_ ) ) + pos.y, pos.z );

        if ( g_math.world_to_screen ( fake_pos, draw_tmp ) )
        {
            g_render.line ( tmp.x, tmp.y, draw_tmp.x, draw_tmp.y, Color ( 255, 0, 0, 255 ), 1.f );
            g_render.text ( g_fonts.menu12, xor_str ( "fake" ), draw_tmp.x, draw_tmp.y, Color ( 255, 0, 0, 255 ) );
        }

        const Vector lby_pos ( 50.f * cos ( deg2rad ( g_local->lower_body_yaw_target() ) ) + pos.x,
                               50.f * sin ( deg2rad ( g_local->lower_body_yaw_target() ) ) + pos.y, pos.z );

        if ( g_math.world_to_screen ( lby_pos, draw_tmp ) )
        {
            g_render.line ( tmp.x, tmp.y, draw_tmp.x, draw_tmp.y, Color ( 0, 0, 0, 255 ), 1.f );
            g_render.text ( g_fonts.menu12, xor_str ( "lby" ), draw_tmp.x, draw_tmp.y, Color ( 255, 255, 255, 255 ) );
        }

#ifdef _DEBUG
        const Vector fake_sim_pos ( 50.f * cos ( deg2rad ( g_features.animations.m_fake_rotation ) ) + pos.x,
                                    50.f * sin ( deg2rad ( g_features.animations.m_fake_rotation ) ) + pos.y, pos.z );

        if ( g_math.world_to_screen ( fake_sim_pos, draw_tmp ) )
        {
            g_render.line ( tmp.x, tmp.y, draw_tmp.x, draw_tmp.y, Color ( 0, 0, 255, 255 ), 1.f );
            g_render.text ( g_fonts.menu12, "sim fake", draw_tmp.x, draw_tmp.y, Color ( 0, 0, 255, 255 ) );
        }

#endif
    }
}

void c_visuals::draw_grenades ( C_BaseCombatWeapon* nade ) const
{
    if ( !nade || nade->is_dormant( ) )
        return;

    const auto model = nade->get_model( );

    if ( !model )
        return;

    const auto render_origin = nade->get_render_origin( );
    Vector out;

    if ( !g_math.world_to_screen ( render_origin, out ) )
        return;

    auto hdr = interfaces::mdl_info->get_studiomodel ( model );

    if ( !hdr )
        return;

    std::string entity_name = hdr->szName;

    switch ( nade->get_client_class( )->class_id )
    {
    case 9:
        entity_name = entity_name[ 16 ] == 's' ? xor_str ( "flash" ) : xor_str ( "frag" );
        break;

    case CSmokeGrenadeProjectile:
        entity_name = xor_str ( "smoke" );
        break;

    case CMolotovProjectile:
        entity_name = xor_str ( "fire" );
        break;

    case CDecoyProjectile:
        entity_name = xor_str ( "decoy" );
        break;

    default:
        return;
    }

    g_render.text ( g_fonts.normal, entity_name, out.x, out.y, Color ( 255, 255, 255, 255 ) );
}

void c_visuals::third_person ( )
{
    auto local = static_cast< C_BasePlayer* > ( g_local );

    if ( !local || !interfaces::engine_trace || !interfaces::input )
        return;

    if ( !g_options.visuals_thirdperson_toggle )
    {
        interfaces::input->m_fCameraInThirdPerson = false;
        return;
    }

    if ( local->is_alive( ) )
    {
        interfaces::input->m_fCameraInThirdPerson = true;
        QAngle vang;
        interfaces::engine_client->get_viewangles ( &vang );
        /* getting correct distance */
        const auto get_correct_distance = [&local] ( float ideal_distance ) -> float
        {
            QAngle inverse_angles;
            interfaces::engine_client->get_viewangles ( &inverse_angles );
            inverse_angles.pitch *= -1.f, inverse_angles.yaw += 180.f;
            Vector direction;
            g_math.angle_vectors ( inverse_angles, direction );
            Ray_t ray;
            trace_t trace;
            CTraceFilterWorldAndPropsOnly filter;
            ray.Init ( local->get_eye_pos( ), local->get_eye_pos( ) + direction * ideal_distance );
            interfaces::engine_trace->trace_ray ( ray, MASK_SHOT, &filter, &trace );

            return ideal_distance * trace.fraction - 10.f;
        };

        const auto best_dist = get_correct_distance ( g_options.visuals_thirdperson_distance );
        interfaces::input->m_vecCameraOffset.z = best_dist;
        interfaces::input->m_nCameraOldX = interfaces::input->m_nCameraX;
        interfaces::input->m_nCameraOldY = interfaces::input->m_nCameraY;
        interfaces::input->m_angPreviousViewAngles = vang;
        interfaces::input->m_angPreviousViewAnglesTilt = QAngle ( 0, 0, 0 );
    }
    else
        interfaces::input->m_fCameraInThirdPerson = false;
}

void c_visuals::watermark ( ) const
{
    C_BasePlayer* local = g_local;

    if ( !local || !local->is_alive( ) )
        return;

    static int fps, old_tick_count;

    if ( interfaces::global_vars->tickcount - old_tick_count > 50 )
    {
        fps = static_cast< int > ( 1.f / interfaces::global_vars->frametime );

        old_tick_count = interfaces::global_vars->tickcount;
    }

    int w, h;
    interfaces::engine_client->get_screen_size ( w, h );
    const auto cx = w - 300;
    std::stringstream ss;
    const auto ping = std::to_string ( local->ping( ) < 0 ? 0 : local->ping( ) );
#ifdef _RELEASE // meme w chuj ~ dark
    ss << xor_str ( "gethad.es beta " ) << xor_str ( "> " ) << xor_str ( "fps: " ) << fps << xor_str ( " | ping: " ) << ping << xor_str ( "ms" );
#endif
#ifdef _DEBUG
    ss << xor_str ( "gethad.es alpha " ) << xor_str ( "> " ) << xor_str ( "fps: " ) << fps << xor_str ( " | ping: " ) << ping << xor_str ( "ms" );
#endif
    //const auto text_size = g_render.get_size ( g_fonts.normal, ss.str ( ) );
    g_render.text ( g_fonts.normal, ( ss.str( ) ), cx, 10, Color ( 255, 255, 255, 255 ) );
}

void c_visuals::add_to_draw_list ( )
{
    if ( !g_local )
        return;

    update_screensize( );

    for ( auto i = 1; i <= interfaces::entity_list->get_highest_entity_index( ); ++i )
    {
        auto entity = C_BaseEntity::get_entity_by_index ( i );

        if ( !entity )
            continue;

        if ( entity == g_local && !interfaces::input->m_fCameraInThirdPerson )
            continue;

        if ( i <= 64 )
        {
            auto player = Player( );

            if ( player.begin ( dynamic_cast< C_BasePlayer* > ( entity ) ) )
            {
                if ( entity->team_num( ) == g_local->team_num( ) )
                    continue;

                if ( g_options.esp_enemy_box )
                    player.render_box( );

                player.draw_crosshair( );

                if ( g_options.esp_enemy_name )
                    player.render_name( );

                if ( g_options.esp_enemy_weapon )
                {
                    player.render_weapon( );
                    player.render_ammo( );
                }

                player.render_flags( );

                if ( g_options.esp_enemy_armor )
                    player.render_armor( );

                if ( g_options.esp_enemy_health )
                    player.render_health( );
            }

            player.draw_damage ( entity );
        }

        if ( g_options.esp_world_weapons && entity->is_weapon( ) )
            render_ground_weapon ( dynamic_cast< C_BaseCombatWeapon* > ( entity ) );

        draw_grenades ( dynamic_cast< C_BaseCombatWeapon* > ( entity ) );
    }

    draw_offscreenesp( );
    draw_hitmarker( );
    draw_indicators( );

    if ( g_options.visuals_manual_aa_indicator )
        draw_manual_aa_indicators( );

    draw_scopelines( );
    //g_features.debug_overlay.on_add_to_draw_list ( );
    g_features.event_logger.on_draw( );
}
