#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"
#include "../helpers/console.h"
#include "../features/features.h"

#ifdef _DEBUG
C_BaseEntity* UTIL_PlayerByIndex ( int index )
{
    typedef C_BaseEntity* ( __fastcall * PlayerByIndex ) ( int );
    static PlayerByIndex UTIL_PlayerByIndex = reinterpret_cast< PlayerByIndex > ( g_utils.pattern_scan ( "server.dll", "85 C9 7E 2A A1" ) );

    if ( !UTIL_PlayerByIndex )
        return false;

    return UTIL_PlayerByIndex ( index );
}
#endif // _DEBUG

void __stdcall hooks::hk_frame_stage_notify ( ClientFrameStage_t stage )
{
    static auto ofunc = hlclient_hook.get_original< frame_stage_notify > ( index::frame_stage_notify );

    if ( g_unload )
        return ofunc ( interfaces::chl_client, stage );

    if ( g_options.visuals_nosmoke_mode != 0 )
    {
        static uint32_t smoke_count = * reinterpret_cast< uint32_t* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                               xor_str ( "A3 ? ? ? ? 57 8B CB" ) ) + 0x1 );
        * reinterpret_cast< int* > ( smoke_count ) = 0;
    }

    switch ( stage )
    {
    case FRAME_UNDEFINED:
        break;

    case FRAME_START:
        break;

    case FRAME_NET_UPDATE_START:
        break;

    case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
        g_features.misc.clantag_changer( );
        break;

    case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
        {
            g_features.resolver.on_frame_stage_notify( );

            for ( auto i = 1; i < interfaces::engine_client->get_max_clients( ); i++ )
            {
                auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

                if ( !entity || !g_local || !entity->is_player( ) || entity->is_local( ) || entity->is_dormant( ) || !entity->is_alive( ) || entity ==
                    g_local )
                    continue;

                auto map = entity->var_mapping( );

                if ( map )
                {
                    for ( auto j = 0; j < map->interpolated_entries; j++ )
                        map->entries[ j ].needs_to_interpolate = false;
                }
            }

            if ( interfaces::mat_system )
            {
                std::vector< const char* > smoke_materials = { };

                std::string t1 = xor_str ( "particle/vistasmokev1/vistasmokev1_fire" );
                std::string t2 = xor_str ( "particle/vistasmokev1/vistasmokev1_smokegrenade" );
                std::string t3 = xor_str ( "particle/vistasmokev1/vistasmokev1_emods" );
                std::string t4 = xor_str ( "particle/vistasmokev1/vistasmokev1_emods_impactdust" );

                smoke_materials.push_back ( t1.data( ) );
                smoke_materials.push_back ( t2.data( ) );
                smoke_materials.push_back ( t3.data( ) );
                smoke_materials.push_back ( t4.data( ) );

                if ( g_options.visuals_nosmoke_mode == 0 )
                {
                    for ( auto material_name : smoke_materials )
                    {
                        IMaterial* mat = interfaces::mat_system->find_material ( material_name, TEXTURE_GROUP_OTHER );
                        mat->set_material_var_flag ( MATERIAL_VAR_WIREFRAME, false );
                        mat->set_material_var_flag ( MATERIAL_VAR_NO_DRAW, false );
                    }
                }
                else
                {
                    for ( auto material_name : smoke_materials )
                    {
                        IMaterial* mat = interfaces::mat_system->find_material ( material_name, TEXTURE_GROUP_OTHER );
                        mat->set_material_var_flag ( MATERIAL_VAR_WIREFRAME, true );
                        mat->set_material_var_flag ( MATERIAL_VAR_NO_DRAW, g_options.visuals_nosmoke_mode == 2 );
                    }
                }
            }

            c_misc::radar( );

            if ( g_local && g_local->is_alive( ) )
            {
                //globals.last_local_eye_ang = g_local->eye_angles().yaw;
                //globals.fake_yaw_add = globals.last_local_eye_ang - globals.real_yaw;
            }

            break;
        }

    case FRAME_NET_UPDATE_END:
        break;

    case FRAME_RENDER_START:

        //animation_fix::get().on_frame_stage_notify();

        //anti_aim::get().set_thirdperson_angle();
        static auto load_named_sky = reinterpret_cast< void ( __fastcall*) ( const char* ) > ( g_utils.pattern_scan ( xor_str ( "engine.dll" ),
                                                                                                                      xor_str (
                                                                                                                          "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" ) )
        );
        load_named_sky ( xor_str ( "sky_csgo_night02" ) );

        c_misc::nightmode( );

        if ( g_local && g_local->is_alive( ) )
        {
            if ( g_options.visuals_noflash )
            {
                g_local->flash_max_alpha( ) = 0.f;
                //g_local->flash_duration() = 0.f;
            }
            else
                g_local->flash_max_alpha( ) = 255.f;

            globals.old_viewpunch = g_local->view_punch_angle( );
            globals.old_aimpunch = g_local->aim_punch_angle( );

            if ( g_options.visuals_remove_viewpunch )
            {
                g_local->view_punch_angle( ) = QAngle ( 0.f, 0.f, 0.f );
                g_local->aim_punch_angle( ) = QAngle ( 0.f, 0.f, 0.f );
            }
        }

        // viewmodel xyz changer
        {
            static auto init = false;
            static auto var_x = interfaces::cvar->find_var ( xor_str ( "viewmodel_offset_x" ) );
            static auto var_y = interfaces::cvar->find_var ( xor_str ( "viewmodel_offset_y" ) );
            static auto var_z = interfaces::cvar->find_var ( xor_str ( "viewmodel_offset_z" ) );

            if ( !init )
            {
                g_options.misc_viewmodel_x = var_x->get_float( );
                g_options.misc_viewmodel_y = var_y->get_float( );
                g_options.misc_viewmodel_z = var_z->get_float( );
                g_options.org_misc_viewmodel_x = g_options.misc_viewmodel_x;
                g_options.org_misc_viewmodel_y = g_options.misc_viewmodel_y;
                g_options.org_misc_viewmodel_z = g_options.misc_viewmodel_z;
                init = true;
            }

            var_x->m_fnChangeCallbacks.m_Size = 0;
            var_y->m_fnChangeCallbacks.m_Size = 0;
            var_z->m_fnChangeCallbacks.m_Size = 0;
            var_x->set_value ( g_options.misc_viewmodel_x );
            var_y->set_value ( g_options.misc_viewmodel_y );
            var_z->set_value ( g_options.misc_viewmodel_z );
        }

        for ( auto i = 1; i < interfaces::engine_client->get_max_clients( ); i++ )
        {
            auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( i ) );

            if ( !entity || !g_local || entity->is_dormant( ) || !entity->is_alive( ) || !entity->is_player( ) || entity == g_local )
                continue;

            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( entity ) + 0xA30 ) = interfaces::global_vars->framecount;
            //we'll skip occlusion checks now
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( entity ) + 0xA28 ) = 0; //clear occlusion flags
            *reinterpret_cast< int* > ( reinterpret_cast< uintptr_t > ( entity ) + 0xA64 ) = interfaces::global_vars->framecount;

            auto state = entity->get_base_player_anim_state( );
            //entity->set_vangle ( entity->eye_angles() );
            entity->set_abs_original ( entity->vec_origin( ) );

            if ( state )
            {
                //state->m_flFeetYawRate = 1.f;

                if ( entity->vec_velocity( ).length2d( ) < 0.1f )
                    * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( state ) + 292 ) = 0.f;
            }
        }

        if ( g_local && g_local->is_alive( ) )
        {
            auto state = g_local->get_base_player_anim_state( );

            if ( g_options.visuals_thirdperson_toggle )
            {
#ifdef _DEBUG
                static auto pCall = reinterpret_cast< uintptr_t > ( g_utils.pattern_scan (
                    "server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" ) );

                float fDuration = -1.f;

                PVOID pEntity = nullptr;
                pEntity = UTIL_PlayerByIndex ( g_local->ent_index( ) );

                if ( pEntity )
                {
                    __asm
                    {
                        pushad
                        movss xmm1, fDuration
                        push 0 //bool monoColor
                        mov ecx, pEntity
                        call pCall
                        popad
                    }
                }

#endif // _DEBUG

                //if ( interfaces::client_state->chokedcommands == 0 )
                //static auto ang = g_features.anti_aim.get_last_sent_real_angle();

                //if ( interfaces::client_state->chokedcommands == 0 )
                //    ang = g_features.anti_aim.get_last_sent_real_angle();

                //g_local->set_vangle ( ang );

                //globals.fsn_real_ang = ang;

                if ( state )
                {
                    if ( g_local->vec_velocity( ).length2d( ) < 0.1f )
                        * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( state ) + 292 ) = 0.f;
                }
            }

            g_local->update_client_side_animation( );
        }

        break;

    case FRAME_RENDER_END:
        //for ( auto i = 1; i < interfaces::engine_client->get_max_clients(); i++ )
        //{
        //    auto entity = dynamic_cast<C_BasePlayer*> ( interfaces::entity_list->get_client_entity ( i ) );

        //    if ( !entity || !g_local || !entity->is_player() || entity->is_local() || entity->is_dormant() || !entity->is_alive() )
        //        continue;

        //    globals.backup_abs_ang[i] = entity->get_abs_angles(  );
        //}

        break;
    }

    ofunc ( interfaces::chl_client, stage );
}
