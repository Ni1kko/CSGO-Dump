#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"
#include "../menu/hades_menu.h"

long __stdcall hooks::hk_end_scene ( IDirect3DDevice9* pDevice )
{
    auto ofunc = direct3d_hook.get_original< end_scene > ( index::end_scene );

    //if ( g_Unload )
    //    return oEndScene( pDevice );

    static auto viewmodel_fov = interfaces::cvar->find_var ( xor_str ( "viewmodel_fov" ) );
    static auto mat_ambient_light_r = interfaces::cvar->find_var ( xor_str ( "mat_ambient_light_r" ) );
    static auto mat_ambient_light_g = interfaces::cvar->find_var ( xor_str ( "mat_ambient_light_g" ) );
    static auto mat_ambient_light_b = interfaces::cvar->find_var ( xor_str ( "mat_ambient_light_b" ) );
    static auto crosshair_cvar = interfaces::cvar->find_var ( xor_str ( "crosshair" ) );

    viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
    //viewmodel_fov->set_value( g_Options.viewmodel_fov );
    //mat_ambient_light_r->set_value( g_Options.mat_ambient_light_r );
    //mat_ambient_light_g->set_value( g_Options.mat_ambient_light_g );
    //mat_ambient_light_b->set_value( g_Options.mat_ambient_light_b );
    //crosshair_cvar->set_value( !g_Options.esp_crosshair );

    DWORD colorwrite, srgbwrite;
    pDevice->GetRenderState ( D3DRS_COLORWRITEENABLE, &colorwrite );
    pDevice->GetRenderState ( D3DRS_SRGBWRITEENABLE, &srgbwrite );

    pDevice->SetRenderState ( D3DRS_COLORWRITEENABLE, 0xffffffff );
    //removes the source engine color correction
    pDevice->SetRenderState ( D3DRS_SRGBWRITEENABLE, false );

    pDevice->SetSamplerState ( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    pDevice->SetSamplerState ( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
    pDevice->SetSamplerState ( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
    pDevice->SetSamplerState ( NULL, D3DSAMP_SRGBTEXTURE, NULL );

    //ImGui_ImplDX9_NewFrame();

    //auto esp_drawlist = Render::get().RenderScene();

    //Menu::get().Render();

    //ImGui::Render ( esp_drawlist );

    //ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData() );

    pDevice->SetRenderState ( D3DRS_COLORWRITEENABLE, colorwrite );
    pDevice->SetRenderState ( D3DRS_SRGBWRITEENABLE, srgbwrite );

    return ofunc ( pDevice );
}
