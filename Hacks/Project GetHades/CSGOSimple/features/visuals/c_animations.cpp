#include "c_animations.h"
#include "../features.h"
#include "../../helpers/console.h"

void c_animations::on_create_move ( const bool send_packet )
{
    if ( send_packet )
        fake_animation( );

    real_animation ( send_packet );
}

void c_animations::fake_animation ( )
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    if ( m_fake_spawntime != g_local->spawn_time( ) || m_should_update_fake )
    {
        init_fake_anim = false;
        m_fake_spawntime = g_local->spawn_time( );
        m_should_update_fake = false;
    }

    if ( !init_fake_anim )
    {
        m_fake_state = static_cast< CCSGOPlayerAnimState* > ( interfaces::mem_alloc->Alloc ( sizeof ( CCSGOPlayerAnimState ) ) );

        if ( m_fake_state != nullptr )
            g_local->create_animation_state ( m_fake_state );

        init_fake_anim = true;
    }

    animation_layer_t layer_backup[13];
    float pose_backup[20];

    std::memcpy ( layer_backup, g_local->get_anim_overlays( ), sizeof ( layer_backup ) );
    std::memcpy ( pose_backup, g_local->pose_parameter( ).data( ), sizeof ( pose_backup ) );

    const auto ba_curtime = interfaces::global_vars->curtime;

    interfaces::global_vars->curtime = g_local->simulation_time( );

    g_local->update_animation_state ( m_fake_state, ctx::client.cmd->viewangles );

    m_fake_rotation = reinterpret_cast< c_base_player_anim_state* > ( m_fake_state )->m_flGoalFeetYaw;
    g_local->set_abs_angles ( qangle_t ( 0, reinterpret_cast< c_base_player_anim_state* > ( m_fake_state )->m_flGoalFeetYaw, 0 ) );
    ctx::client.should_setup_local_bones = true;
    m_got_fake_matrix = g_local->setup_bones ( m_fake_matrix, 128, bone_used_by_anything & ~bone_used_by_attachment,
                                               interfaces::global_vars->curtime );

    memcpy ( m_fake_position_matrix, m_fake_matrix, sizeof ( m_fake_position_matrix ) );

    const auto org_tmp = g_local->get_render_origin( );

    if ( m_got_fake_matrix )
    {
        for ( auto& i : m_fake_matrix )
        {
            i[ 0 ][ 3 ] -= org_tmp.x;
            i[ 1 ][ 3 ] -= org_tmp.y;
            i[ 2 ][ 3 ] -= org_tmp.z;
        }
    }

    g_local->set_abs_angles ( QAngle ( 0, m_server_abs_rotation, 0 ) ); // restore real abs rotation

    std::memcpy ( m_fake_layers.data( ), g_local->get_anim_overlays( ), sizeof ( m_fake_layers ) );
    std::memcpy ( m_fake_poses.data( ), g_local->pose_parameter( ).data( ), sizeof ( m_fake_poses ) );

    interfaces::global_vars->curtime = ba_curtime;

    std::memcpy ( g_local->get_anim_overlays( ), layer_backup, sizeof ( layer_backup ) );
    std::memcpy ( g_local->pose_parameter( ).data( ), pose_backup, sizeof ( pose_backup ) );
}

void c_animations::real_animation ( const bool send_packet )
{
    if ( !g_local || !g_local->is_alive( ) )
        return;

    if ( m_real_spawntime != g_local->spawn_time( ) || m_should_update_real )
    {
        init_real_anim = false;
        m_real_spawntime = g_local->spawn_time( );
        m_should_update_real = false;
    }

    if ( !init_real_anim )
    {
        m_real_state = static_cast< CCSGOPlayerAnimState* > ( interfaces::mem_alloc->Alloc ( sizeof ( CCSGOPlayerAnimState ) ) );

        if ( m_real_state != nullptr )
            g_local->create_animation_state ( m_real_state );

        init_real_anim = true;
    }
    auto anim_state = reinterpret_cast< c_base_player_anim_state* > ( m_real_state );
    anim_state->m_iLastClientSideAnimationUpdateFramecount -= 3;
    anim_state->m_flLastClientSideAnimationUpdateTime -= 3;

    g_local->invalidate_bone_cache( );

    g_local->update_animation_state ( m_real_state, qangle_t ( g_local->eye_angles( ).pitch, ctx::client.cmd->viewangles.yaw, 0.f ) );

    if ( send_packet )
    {
        m_server_abs_rotation = reinterpret_cast< c_base_player_anim_state* > ( m_real_state )->m_flGoalFeetYaw;
        g_local->set_abs_angles ( qangle_t ( 0, reinterpret_cast< c_base_player_anim_state* > ( m_real_state )->m_flGoalFeetYaw, 0 ) );
        ctx::client.should_setup_local_bones = true;
        m_got_real_matrix = g_local->setup_bones ( m_real_matrix, maxstudiobones, bone_used_by_anything, 0.f );
        m_current_real_angle = ctx::client.cmd->viewangles;
        const auto org_tmp = g_local->get_render_origin( );

        if ( m_got_real_matrix ) // todo maybe not use this and set abs yaw in setup_bones,..
        {
            for ( auto& i : m_real_matrix )
            {
                i[ 0 ][ 3 ] -= org_tmp.x;
                i[ 1 ][ 3 ] -= org_tmp.y;
                i[ 2 ][ 3 ] -= org_tmp.z;
            }
        }
    }

    if ( m_fake_state && m_real_state )
        m_fake_delta = m_server_abs_rotation - reinterpret_cast< c_base_player_anim_state* > ( m_fake_state )->m_flGoalFeetYaw;

    //if ( m_real_state )
    //    console::write_line ( *reinterpret_cast< float* > ( uintptr_t ( m_real_state ) + 0x110 ) );

    std::memcpy ( m_real_layers.data( ), g_local->get_anim_overlays( ), sizeof ( m_real_layers ) );
    std::memcpy ( m_real_poses.data( ), g_local->pose_parameter( ).data( ), sizeof ( m_real_poses ) );
}

void c_animations::player_animations ( )
{
    if ( !g_local )
        return;

    for ( auto idx = 1; idx < interfaces::global_vars->max_clients; idx++ )
    {
        auto player = reinterpret_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( idx ) );

        auto& data = m_player_states.at ( idx );

        if ( !player || player->is_dormant( ) || !player->is_alive( ) )
        {
            data.m_init = false;
            continue;
        }

        if ( player->ent_index( ) == interfaces::engine_client->get_localplayer( ) )
            continue;

        if ( data.m_spawntime != player->spawn_time( ) || m_should_update_entity_animstate )
        {
            data.m_init = false;
            data.m_spawntime = player->spawn_time( );
        }

        if ( !data.m_state )
            data.m_init = false;

        if ( !data.m_init )
        {
            data.m_state = static_cast< CCSGOPlayerAnimState* > ( interfaces::mem_alloc->Alloc ( sizeof ( CCSGOPlayerAnimState ) ) );

            if ( data.m_state != nullptr )
                player->create_animation_state ( data.m_state );

            data.m_init = true;
        }
        auto anim_state = reinterpret_cast< c_base_player_anim_state* > ( data.m_state );
        anim_state->m_iLastClientSideAnimationUpdateFramecount -= 3;
        anim_state->m_flLastClientSideAnimationUpdateTime -= 3;

        if ( *reinterpret_cast< float* > ( uintptr_t ( data.m_state ) + 0x110 ) > 0.8f )
            *reinterpret_cast< float* > ( uintptr_t ( data.m_state ) + 0x110 ) = 0.f;

        if ( data.m_last_simtime == player->simulation_time( ) )
        {
            const auto add = g_features.resolver.rdata.at ( player->ent_index( ) ).should_resolve
                                 ? g_features.resolver.rdata.at ( player->ent_index( ) ).desync_add < 0.f
                                       ? -60.f
                                       : 60.f
                                 : 0.f;
            player->update_animation_state ( data.m_state, qangle_t ( player->eye_angles( ).pitch,
                                                                      player->eye_angles( ).yaw + add, 0.f ) );

            if ( *reinterpret_cast< float* > ( uintptr_t ( data.m_state ) + 0x110 ) > 0.8f )
                *reinterpret_cast< float* > ( uintptr_t ( data.m_state ) + 0x110 ) = 0.f;
        }
        else
        {
            player->update_animation_state ( data.m_state, player->eye_angles( ) );

            if ( *reinterpret_cast< float* > ( uintptr_t ( data.m_state ) + 0x110 ) > 0.8f )
                *reinterpret_cast< float* > ( uintptr_t ( data.m_state ) + 0x110 ) = 0.f;

            // set var to setup bones and set them up
            player->invalidate_bone_cache( );
            player->set_abs_angles ( qangle_t ( 0, reinterpret_cast< c_base_player_anim_state* > ( data.m_state )->m_flGoalFeetYaw, 0 ) );

            data.m_should_recalculate_matrix = true;

            data.m_got_matrix = player->setup_bones ( data.m_matrix, maxstudiobones, bone_used_by_anything, 0.f );

            data.m_should_recalculate_matrix = false;
        }

        data.m_last_simtime = player->simulation_time( );
    }

    m_should_update_entity_animstate = false;
}
