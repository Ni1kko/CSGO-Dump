#include "hooks.hpp"
#include "../options.h"
#include "../helpers/math.hpp"
#include "../helpers/console.h"
#include "../features/features.h"

struct setup_bones_data_t
{
    matrix3x4_t matrix[maxstudiobones];
    bool got = false;
    float simtime = 0.f;
    float yaw = 0.f;
    Vector pos = Vector ( 0, 0, 0 );
    int last_bonemask = 0;
    float spawntime = 0.f;
    CCSGOPlayerAnimState* new_state = nullptr;
    bool init_animstate = false;
    int last_tickcount = -1;
    float simtime_state = 0.f;
    animation_layer_t m_real_layers[13];
    float m_real_poses[20];
    float goal_feet_yaw = 0.f;
};

// #STR: "Client_Animation", "C_BaseAnimating::SetupBones", "*** ERROR: Bone access not allowed (entity %i:%s)\n", "%d:%s",
// "SetupBones: invalid bone array size (%d - needs %d)\n", "Client_Animation_Threaded"
bool __fastcall hooks::hk_setup_bones ( void* ecx, void* edx, matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float curtime )
{
    if ( g_unload )
        return o_setup_bones ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

    if ( ecx && static_cast< IClientRenderable* > ( ecx )->get_iclient_unknown( ) )
    {
        auto networkable = static_cast< IClientRenderable* > ( ecx )->get_iclient_unknown( )->get_client_networkable( );

        //static uintptr_t org_adr = *org_init_addr;
        auto tmp_renderable = static_cast< C_BaseCombatWeapon* > ( ecx );

        //console::write_line ( networkable->get_client_class ( )->class_id );

        if ( networkable && networkable->get_client_class( ) && networkable->get_client_class( )->class_id == CCSPlayer )
        {
            auto renderable = static_cast< IClientRenderable* > ( ecx );
            auto player = dynamic_cast< C_BasePlayer* > ( renderable );
            //auto real_player = reinterpret_cast<C_BasePlayer*>(reinterpret_cast<uintptr_t)

            if ( !player || !renderable )
                return false;

            const auto i = networkable->ent_index( );

            static setup_bones_data_t data[64];

            /*
            if (!o_setup_bones(ecx, _data[i].matrix, 128, bone_used_by_anything, curtime)) {

            }
            */
            //if ( networkable->ent_index ( ) == interfaces::engine_client->get_localplayer ( ) )
            //{
            //    const auto return_val = o_setup_bones ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

            //    return return_val;
            //}

            if ( curtime == 0.f )
                curtime = player->simulation_time( );

            static auto host_timescale = interfaces::cvar->find_var ( xor_str ( "host_timescale" ) );
            const auto old_cur_time = interfaces::global_vars->curtime;
            const auto old_real_time = interfaces::global_vars->realtime;
            const auto old_frame_time = interfaces::global_vars->frametime;
            const auto old_abs_frame_time = interfaces::global_vars->absoluteframetime;
            const auto old_interp_amount = interfaces::global_vars->interpolation_amount;
            const auto old_tick_count = interfaces::global_vars->tickcount;

            if ( networkable->ent_index( ) == interfaces::engine_client->get_localplayer( ) )
            {
                //if ( !ctx::client.should_setup_local_bones )
                //{
                //    if ( g_features.animations.m_got_real_matrix )
                //    {
                //        if ( !bone_to_world_out )
                //            return true;

                //        memcpy ( bone_to_world_out, g_features.animations.m_real_matrix, sizeof ( matrix3x4_t ) * max_bones );

                //        const auto t = player->get_render_origin( );

                //        for ( auto idx = 0; idx < max_bones; ++idx )
                //        {
                //            bone_to_world_out[ idx ][ 0 ][ 3 ] += t.x;
                //            bone_to_world_out[ idx ][ 1 ][ 3 ] += t.y;
                //            bone_to_world_out[ idx ][ 2 ][ 3 ] += t.z;
                //        }

                //        return true;
                //    }
                //    return false;
                //}

                ctx::client.should_setup_local_bones = false;

                static animation_layer_t backup_layers[13];
                static float backup_poses[20];
                //static matrix3x4_t local_matrix[maxstudiobones];
                //static auto last_time = 0.f;

                //if ( interfaces::global_vars->tickcount == last_tickcount && got && player->vec_origin() == last_origin )
                //{
                //    if ( bone_to_world_out )
                //        memcpy ( bone_to_world_out, local_matrix, sizeof ( matrix3x4_t ) * max_bones );

                //    return true;
                //}

                if ( !bone_to_world_out )
                {
                    static auto last_tickcount = 0;

                    bone_mask = bone_used_by_anything;

                    if ( last_tickcount == interfaces::global_vars->tickcount )
                        bone_mask &= ~bone_used_by_attachment;

                    last_tickcount = interfaces::global_vars->tickcount;

                    if ( player->vec_velocity( ).length2d( ) < 0.1f && data[ i ].simtime == player->simulation_time( ) ) //& ~BONE_USED_BY_ATTACHMENT {
                        bone_mask &= ~bone_used_by_attachment;

                    data[ i ].simtime = player->simulation_time( );

                    auto& last_animation_framecount = *reinterpret_cast< int* > ( uintptr_t ( player ) + 0xA68 );
                    last_animation_framecount = 0;

                    auto state = player->get_base_player_anim_state( );

                    if ( state )
                        state->m_flLastClientSideAnimationUpdateTime -= 1;

                    *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( player ) + 2664 ) = 0;
                    *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x269C ) = 0; //prev_bone_mask
                    bone_mask |= 0x200;
                    player->effects( ) |= 8;
                    player->eflag( ) &= ~EFL_DIRTY_ABSTRANSFORM;
                    //player->set_abs_angles ( qangle_t ( 0, g_features.anti_aim.m_current_rendered_real_abs_yaw, 0 ) );
                    player->invalidate_bone_cache( );

                    static animation_layer_t babackup_layers[13];
                    static float babackup_poses[20];

                    //std::memcpy ( babackup_layers, g_local->get_anim_overlay ( 0 ), sizeof ( babackup_layers ) );
                    //std::memcpy ( babackup_poses, g_local->pose_parameter().data(  ), sizeof ( babackup_poses ) );

                    //std::memcpy ( g_local->get_anim_overlay ( 0 ), backup_layers, sizeof ( backup_layers ) );
                    //std::memcpy ( g_local->pose_parameter().data(  ), backup_poses, sizeof ( backup_poses ) );

                    const auto rval = o_setup_bones ( ecx, bone_to_world_out, max_bones, bone_mask, globals.curtime );

                    //std::memcpy ( g_local->get_anim_overlay ( 0 ), babackup_layers, sizeof ( babackup_layers ) );
                    //std::memcpy ( g_local->pose_parameter().data(  ), babackup_poses, sizeof ( babackup_poses ) );

                    interfaces::global_vars->curtime = old_cur_time;
                    interfaces::global_vars->realtime = old_real_time;
                    //interfaces::global_vars->frametime = old_frame_time;
                    interfaces::global_vars->absoluteframetime = old_abs_frame_time;
                    interfaces::global_vars->tickcount = old_tick_count;
                    interfaces::global_vars->interpolation_amount = old_interp_amount;

                    return rval;
                }

                //if ( interfaces::client_state->chokedcommands != 0 )
                //    return true;

                //last_time = globals.curtime;

                interfaces::global_vars->curtime = globals.curtime;
                interfaces::global_vars->realtime = globals.curtime;

                if ( interfaces::global_vars->frametime > 0.0033333334f )
                    interfaces::global_vars->frametime = 0.003333333f;

                //interfaces::global_vars->frametime = //interfaces::global_vars->interval_per_tick * host_timescale->get_float ( );
                interfaces::global_vars->absoluteframetime = interfaces::global_vars->interval_per_tick * host_timescale->get_float( );
                interfaces::global_vars->interpolation_amount = 0;
                interfaces::global_vars->tickcount = time_to_ticks ( globals.curtime );

                //if ( data[i].simtime != player->simulation_time() )
                //    player->invalidate_bone_cache();

                //auto ba = *reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x28B8 );
                //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x28B8 ) = -1;
                //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x2698 ) = bone_used_by_anything; // last bonemask
                //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x2EA ) = 0;
                //interfaces::prediction->in_prediction();
                //0x268C
                auto state = player->get_base_player_anim_state( );

                if ( state )
                    state->m_flLastClientSideAnimationUpdateTime -= 1;

                auto& last_animation_framecount = *reinterpret_cast< int* > ( uintptr_t ( player ) + 0xA68 );
                last_animation_framecount = 0;
                *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x269C ) = 0; //prev_bone_mask
                bone_mask |= 0x200;
                player->effects( ) |= 8;

                *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( player ) + 2664 ) = 0;
                player->eflag( ) &= ~EFL_DIRTY_ABSTRANSFORM;
                //player->set_abs_angles ( qangle_t ( 0, g_features.anti_aim.m_current_rendered_real_abs_yaw, 0 ) );
                player->invalidate_bone_cache( );

                //if ( renderable ) // eyeposition
                //{
                //    *reinterpret_cast<float*> ( reinterpret_cast<uintptr_t> ( renderable ) + 0x80 ) = 0;
                //    *reinterpret_cast<float*> ( reinterpret_cast<uintptr_t> ( renderable ) + 0x84 ) = 0;
                //    *reinterpret_cast<float*> ( reinterpret_cast<uintptr_t> ( renderable ) + 0x88 ) = 0;
                //}

                //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x268C ) = 0; //GetMostRecentModelBoneCounter so its like we call invalideBoneCache
                //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x268C ) = -1; //  GetMostRecentModelBoneCounter()
                *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x26A8 ) = -1; //GetReadableBones --> so we skip cache

                if ( data[ i ].simtime == player->simulation_time( ) ) //& ~BONE_USED_BY_ATTACHMENT {
                    bone_mask &= ~bone_used_by_attachment;

                data[ i ].simtime = player->simulation_time( );

                //std::memcpy ( backup_layers, g_local->get_anim_overlay ( 0 ), sizeof ( backup_layers ) );
                //std::memcpy ( backup_poses, g_local->pose_parameter().data(  ), sizeof ( backup_poses ) );

                const auto return_val = o_setup_bones ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );

                interfaces::global_vars->curtime = old_cur_time;
                interfaces::global_vars->realtime = old_real_time;
                //interfaces::global_vars->frametime = old_frame_time;
                interfaces::global_vars->absoluteframetime = old_abs_frame_time;
                interfaces::global_vars->tickcount = old_tick_count;
                interfaces::global_vars->interpolation_amount = old_interp_amount;

                //if ( return_val && bone_to_world_out )
                //    memcpy ( bone_to_world_out, local_matrix, sizeof ( matrix3x4_t ) * max_bones );

                return return_val;
            }

            if ( !bone_to_world_out )
                return true;

            if ( networkable->is_dormant( ) )
                return false;

            bone_mask = bone_used_by_anything;

            if ( !g_features.animations.m_player_states.at ( player->ent_index( ) ).m_should_recalculate_matrix )
            {
                if ( !g_features.animations.m_player_states.at ( player->ent_index( ) ).m_got_matrix )
                    return false;

                memcpy ( bone_to_world_out, g_features.animations.m_player_states.at ( player->ent_index( ) ).m_matrix, sizeof ( matrix3x4_t ) * max_bones );

                return true;
            }
            data[ i ].got = false;

            auto state = player->get_base_player_anim_state( );

            if ( state )
                state->m_flLastClientSideAnimationUpdateTime -= 1;

            auto& last_animation_framecount = *reinterpret_cast< int* > ( uintptr_t ( player ) + 0xA68 );
            last_animation_framecount = 0;
            static auto offs_bone_mask = g_netvarsys.GetOffset ( xor_str ( "DT_BaseAnimating" ), xor_str ( "m_nForceBone" ) ) + 0x20;
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( player ) + offs_bone_mask ) = 0;
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x2698 ) = 0; // last bonemask
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x2EA ) = 0;
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x269C ) = 0; //prev_bone_mask
            bone_mask |= 0x200;
            player->effects( ) |= 8;
            //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0xA68 ) = interfaces::global_vars->framecount; //last_framecount
            //static auto sw = false;
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x26A8 ) = -1; //GetReadableBones --> so we skip cache
            //sw = !sw; // xd just for testing bc normaly GetReadableBones gets set within setupbones
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( renderable ) + 0x268C ) = -1; //  GetMostRecentModelBoneCounter()
            //*reinterpret_cast<int*> ( reinterpret_cast<uintptr_t> ( player ) + 0x268C ) = 0;

            *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( player ) + 2664 ) = 0;
            *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( player ) + 0x80 ) = 0;
            *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( player ) + 0x84 ) = 0;
            *reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( player ) + 0x88 ) = 0;
            player->eflag( ) &= ~EFL_DIRTY_ABSTRANSFORM;

            player->set_abs_original ( player->vec_origin( ) );
            player->invalidate_bone_cache( );

            *reinterpret_cast< int* > ( reinterpret_cast< int > ( renderable ) + 236 ) |= 8; // IsNoInterpolationFrame
            const auto return_val = o_setup_bones ( ecx, data[ i ].matrix, 128, bone_mask, curtime );
            *reinterpret_cast< int* > ( reinterpret_cast< int > ( renderable ) + 236 ) &= ~8; // (1 << 3)

            if ( return_val )
            {
                data[ i ].simtime = curtime;
                data[ i ].got = true;
                data[ i ].pos = player->vec_origin( );

                //for ( int idx = 0; idx < max_bones; idx++ )
                //    g_math.matrix_copy ( data[i].matrix[idx], bone_to_world_out[idx] );
                memcpy ( bone_to_world_out, data[ i ].matrix, sizeof ( matrix3x4_t ) * max_bones );

                data[ i ].yaw = player->eye_angles( ).yaw;
            }

            return return_val;
        }
    }

    return o_setup_bones ( ecx, bone_to_world_out, max_bones, bone_mask, curtime );
}
