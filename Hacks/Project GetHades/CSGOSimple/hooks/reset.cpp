#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"
#include "../renderer/vgui_render.h"
#include "../features/features.h"

long __stdcall hooks::hk_reset ( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
    auto ofunc = direct3d_hook.get_original< reset > ( index::reset );

    if ( g_unload )
        return ofunc ( device, pPresentationParameters );

    if ( g_render.initalized )
    {
        g_render.invalidate_objects( );
        long hr = ofunc ( device, pPresentationParameters );
        g_render.restore_objects ( device );

        return hr;
    }

    return ofunc ( device, pPresentationParameters );
}

long __stdcall hooks::hk_present ( IDirect3DDevice9* m_device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override,
                                   const RGNDATA* dirty_region )
{
    static auto original_fn = direct3d_hook.get_original< present > ( index::present );

    [ m_device ] ( )
        {
            if ( !g_render.initalized )
            {
                g_render.initalize_objects ( m_device );
                g_render.initalized = true;
            }
            else
            {
                static const D3DRENDERSTATETYPE backup_states[] = {
                    D3DRS_COLORWRITEENABLE, D3DRS_ALPHABLENDENABLE, D3DRS_SRCBLEND, D3DRS_DESTBLEND, D3DRS_BLENDOP, D3DRS_FOGENABLE
                };
                static const int size = sizeof ( backup_states ) / sizeof ( DWORD );
                IDirect3DStateBlock9* state_block = nullptr;

                DWORD old_states[ size ] = { 0 };

                for ( int i = 0; i < size; i++ )
                    m_device->GetRenderState ( backup_states[ i ], &old_states[ i ] );

                m_device->SetRenderState ( D3DRS_COLORWRITEENABLE, 0xFFFFFFFF ); // no draw fix
                m_device->CreateStateBlock ( D3DSBT_ALL, &state_block );
                state_block->Capture( );

                g_render.setup_states( ); // sets up proper render states for our state block
                g_features.visuals.inaccuracy ( m_device );

                state_block->Apply( );
                state_block->Release( );

                for ( int i = 0; i < size; i++ )
                    m_device->SetRenderState ( backup_states[ i ], old_states[ i ] ); // restore render states
            }
        }
        ( );

    return original_fn ( m_device, source_rect, dest_rect, dest_window_override, dirty_region );
}
