#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"
#include "../helpers/console.h"
#include "../helpers/math.hpp"
#include "../features/features.h"

void CL_ParseEventDelta ( void* RawData, void* pToData, RecvTable* pRecvTable )
{
    // "RecvTable_DecodeZeros: table '%s' missing a decoder.", look at the function that calls it.
    static uintptr_t CL_ParseEventDeltaF = reinterpret_cast< uintptr_t > ( g_utils.pattern_scan ( xor_str ( "engine.dll" ),
                                                                                                  ( xor_str ( "55 8B EC 83 E4 F8 53 57" ) ) ) );
    __asm
    {
        mov ecx, RawData
        mov edx, pToData
        push pRecvTable
        call CL_ParseEventDeltaF
        add esp, 4
    }
};

// todo update this so we can use it
//bool __fastcall hooks::hk_temp_entities ( void* ECX, void* EDX, void* msg )
//{
//    if ( !g_local || !interfaces::engine_client->is_in_game() || !interfaces::engine_client->is_connected() )
//        return o_temp_entities ( ECX, msg );
//
//    bool ret = o_temp_entities ( ECX, msg );
//
//    // Filtering events
//    if ( !g_local->is_alive() )
//        return ret;
//
//    CEventInfo* ei = interfaces::client_state->events;
//    CEventInfo* next = NULL;
//
//    if ( !ei )
//        return ret;
//
//    do
//    {
//        next = * ( CEventInfo** ) ( ( uintptr_t )ei + 0x38 );
//
//        uint16_t classID = ei->classID - 1;
//
//        auto m_pCreateEventFn = ei->pClientClass->create_event_fn; // ei->pClientClass->m_pCreateEventFn ptr
//
//        if ( !m_pCreateEventFn )
//            continue;
//
//        IClientNetworkable* pCE = m_pCreateEventFn();
//
//        if ( !pCE )
//            continue;
//
//        if ( classID == CTEFireBullets )
//        {
//            // set fire_delay to zero to send out event so its not here later.
//            ei->fire_delay = 0.0f;
//
//            C_BaseEntity* e = reinterpret_cast<C_BaseEntity*> ( interfaces::entity_list->get_client_entity ( pCE->ent_index() ) );
//
//            if ( !e )
//                break;
//
//            C_BasePlayer* test = e->owner_entity();
//
//            if ( test )
//            {
//                //console::write_line ( "test exists" );
//                console::write_line ( test->get_player_info().sz_name );
//            }
//
//            auto pRecvTable = ei->pClientClass->recv_table;
//            void* BasePtr = pCE->get_data_table_base_ptr();
//
//            CL_ParseEventDelta ( ei->pData, BasePtr, pRecvTable );
//
//            if ( !BasePtr )
//                continue;
//
//            console::write_line ( std::to_string ( ( uintptr_t )BasePtr ) );
//
//            int EntityIndex = * ( int* ) ( ( uintptr_t )BasePtr + 0xC ) + 1;
//
//            auto pEntity = static_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( EntityIndex ) );
//
//            if ( pEntity && pEntity->get_client_class() &&  pEntity->get_client_class()->class_id == class_id_t::CCSPlayer && pEntity->is_enemy() )
//            {
//                //g_features.resolver.set_entity_shot ( EntityIndex );
//
//                /*
//                QAngle EyeAngles = QAngle ( * ( float* ) ( ( uintptr_t )BasePtr + 0x24 ), * ( float* ) ( ( uintptr_t )BasePtr + 0x28 ), 0.0f ),
//                       CalcedAngle = g_math.calc_angle ( pEntity->get_eye_pos(), g_local->get_eye_pos() );
//
//                * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( BasePtr ) + 0x24 ) = CalcedAngle.pitch;
//                * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( BasePtr ) + 0x28 ) = CalcedAngle.yaw;
//                * reinterpret_cast< float* > ( reinterpret_cast< uintptr_t > ( BasePtr ) + 0x2C ) = 0;
//
//                float
//                event_time = TICKS_TO_TIME ( interfaces::global_vars->tickcount ),
//                player_time = pEntity->simulation_time();
//                */
//
//                /*
//                // Extrapolate tick to hit scouters etc
//                auto lag_records = CMBacktracking::Get().m_LagRecord[pEntity->EntIndex()];
//
//                float shot_time = event_time;
//
//                for ( auto& record : lag_records )
//                {
//                    if ( TICKS_TO_TIME ( record.m_iTickCount ) <= event_time )
//                    {
//                        shot_time = record.m_flSimulationTime + ( event_time - TICKS_TO_TIME ( record.m_iTickCount ) ); // also get choked from this
//                        #ifdef _DEBUG
//                        g_CVar->ConsoleColorPrintf ( Color ( 0, 255, 0, 255 ), "Found exact shot time: %f, ticks choked to get here: %d\n", shot_time, time_to_ticks ( event_time - TICKS_TO_TIME ( record.m_iTickCount ) ) );
//                        #endif
//                        break;
//                    }
//
//                    #ifdef _DEBUG
//                    else
//                        g_CVar->ConsolePrintf ( "Bad curtime difference, EVENT: %f, RECORD: %f\n", event_time, TICKS_TO_TIME ( record.m_iTickCount ) );
//
//                    #endif
//                }
//
//                #ifdef _DEBUG
//                g_CVar->ConsolePrintf ( "Calced angs: %f %f, Event angs: %f %f, CURTIME_TICKOUNT: %f, SIMTIME: %f, CALCED_TIME: %f\n", CalcedAngle.pitch, CalcedAngle.yaw, EyeAngles.pitch, EyeAngles.yaw, event_time, player_time, shot_time );
//                #endif
//
//                if ( !lag_records.empty() )
//                {
//                    int choked = floorf ( ( event_time - player_time ) / g_GlobalVars->interval_per_tick ) + 0.5;
//                    choked = ( choked > 14 ? 14 : choked < 1 ? 0 : choked );
//                    pEntity->m_vecOrigin() = ( lag_records.begin()->m_vecOrigin + ( g_GlobalVars->interval_per_tick * lag_records.begin()->m_vecVelocity * choked ) );
//                }
//
//                CMBacktracking::Get().SetOverwriteTick ( pEntity, CalcedAngle, shot_time, 1 );
//                */
//            }
//        }
//
//        ei = next;
//    }
//    while ( next != NULL );
//
//    return ret;
//    return o_temp_entities ( ECX, msg );
//}
