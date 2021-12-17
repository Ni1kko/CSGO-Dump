#include "legitbot.h"
#include "../../helpers/math.hpp"
#include "../../helpers/input.hpp"
#include "../../helpers/console.h"

void c_legitbot::on_create_move ( usercmd_t* cmd )
{
    local = g_local;

    if ( !local || !local->is_alive( ) )
        return;

    weapon = local->active_weapon( ).get( );

    if ( !weapon )
        return;

    weapon_data = weapon->get_cs_weapon_data( );

    if ( !weapon_data )
        return;

    interfaces::engine_client->get_viewangles ( &this->viewangles );

    do_aimbot ( cmd );
}

void c_legitbot::do_aimbot ( usercmd_t* cmd )
{
    static auto last_recoil = QAngle ( 0, 0, 0 );

    //if ( !g_inputsys.is_key_down ( VK_MBUTTON ) ) // g_options.lbot_key
    //    return;

    auto best_fov = 10.f; // set this to g_options.lbot_fov
    auto best_ang = QAngle{ };
    auto best_found = false;

    auto hitboxes = get_hitboxes_to_scan( );

    viewangles += last_recoil;

    for ( auto i = 0; i < interfaces::engine_client->get_max_clients( ); i++ )
    {
        auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

        if ( !entity || !g_local || !entity->is_player( ) || entity == g_local || entity->is_dormant( )
            || !entity->is_alive( ) || !entity->is_enemy( ) )
            continue;

        if ( !prepare_player ( entity ) )
            continue;

        for ( auto hitbox : hitboxes )
        {
            auto hitbox_pos = Vector{ };
            get_hitbox_pos ( hitbox, hitbox_pos );

            if ( !local->can_see_player ( entity, hitbox_pos ) )
                continue;

            const auto aim_ang = g_math.calc_angle ( local->get_eye_pos( ), hitbox_pos );
            const auto fov = g_math.get_fov ( this->viewangles, aim_ang );

            if ( fov < best_fov )
            {
                best_found = true;
                best_ang = aim_ang;
                best_fov = fov;
            }
        }
    }

    if ( !best_found )
        return;

    auto view_aim_delta = viewangles - best_ang;

    g_math.normalize_angles ( view_aim_delta );

    const auto random_pitch = g_math.random_float ( 1.f, 2.f );
    const auto random_yaw = g_math.random_float ( 1.f, 2.f );

    auto aim_add = QAngle ( 0, 0, 0 );

    aim_add.yaw = view_aim_delta.yaw / random_yaw;
    aim_add.pitch = view_aim_delta.pitch / random_pitch;

    viewangles -= aim_add;

    auto aimpunch = local->aim_punch_angle( );
    //last_recoil = aimpunch;

    viewangles -= aimpunch;

    cmd->viewangles = viewangles;
    g_math.normalize_angles ( cmd->viewangles );
    g_math.clamp_angles ( cmd->viewangles );

    interfaces::engine_client->set_viewangles ( &cmd->viewangles );
}

std::vector< int > c_legitbot::get_hitboxes_to_scan ( )
{
    auto returnval = std::vector< int >( );

    returnval.push_back ( hitbox_head );

    return returnval;
}

void c_legitbot::do_rcs ( usercmd_t* cmd )
{
}

void c_legitbot::reset_rcs ( usercmd_t* cmd )
{
}

QAngle c_legitbot::smooth_angle ( QAngle input, QAngle new_angle )
{
    return QAngle( );
}

bool c_legitbot::prepare_player ( C_BasePlayer* player )
{
    const auto model = player->get_model( );

    if ( !model )
        return false;

    auto scan_hdr = interfaces::mdl_info->get_studiomodel ( model );

    if ( !scan_hdr )
        return false;

    if ( !player->setup_bones ( this->scan_matrix, maxstudiobones, 0x100, 0 ) )
        return false;

    scan_hitboxset = scan_hdr->get_hitbox_set ( 0 );

    return scan_hitboxset != nullptr;
}

bool c_legitbot::get_hitbox_pos ( const int hitbox, Vector& pos ) const
{
    const auto studio_box = this->scan_hitboxset->get_hitbox ( hitbox );

    if ( !studio_box )
        return false;

    Vector min, max;

    g_math.vector_transform ( studio_box->bbmin, this->scan_matrix[ studio_box->bone ], min );
    g_math.vector_transform ( studio_box->bbmax, this->scan_matrix[ studio_box->bone ], max );

    pos = ( min + max ) * 0.5f;

    return true;
}
