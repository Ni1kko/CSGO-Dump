#include "hooks.hpp"
#include "listener_entity.h"

void __fastcall hooks::hk_accumulate_layers ( C_BasePlayer* player, uintptr_t /*edx*/, void** bone_setup, Vector* pos, float time, quaternion_t* q )
{
    *reinterpret_cast< BYTE* > ( player + 2600 ) &= ~0xA;
    g_listener_entity.m_track[ player->ent_index( ) ].vmt.get_original< accumulate_layers_t > ( index::accumulate_layers ) (
        player, bone_setup, pos, time, q );
    *reinterpret_cast< BYTE* > ( player + 2600 ) |= 0xA;
}
