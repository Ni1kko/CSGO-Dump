#include "listener_entity.h"
#include "hooks.hpp"
#include "../helpers/console.h"

void __fastcall hooks::hk_post_data_update ( void* thisptr, void* unk, int update_type )
{
    auto player = reinterpret_cast< C_BasePlayer* > ( thisptr );

    console::write_line ( "omg hooked" );

    //auto ofunc = g_listener_entity.m_track.at ( player->ent_index(  ) ).vmt.get_original<post_data_update_t> ( index::post_data_update );
    g_listener_entity.m_track[ player->ent_index( ) ].vmt.get_original< post_data_update_t > ( index::post_data_update ) ( thisptr );

    //return ofunc ( thisptr, unk, update_type );
}
