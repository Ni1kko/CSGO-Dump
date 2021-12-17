#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

#include "../helpers/input.hpp"
#include "../menu/hades_menu.h"
#include "../renderer/vgui_render.h"
#include "../helpers/material_helper.h"
#include "../features/features.h"

#include "../lua/lua_support.h"

void __stdcall hooks::hk_paint_traverse ( const vgui::VPANEL panel, const bool force_repaint, const bool allow_force )
{
    static auto panel_id = vgui::VPANEL{ 0 };
    static auto panel_id_zoom = vgui::VPANEL{ 0 };

    if ( !panel_id_zoom )
    {
        if ( !strcmp ( xor_str ( "HudZoom" ), interfaces::vgui_panel->GetName ( panel ) ) )
        {
            panel_id_zoom = panel;
            return;
        }
    }

    if ( panel_id_zoom == panel )
        return;

    static auto ofunc = vguipanel_hook.get_original< paint_traverse > ( index::paint_traverse );

    ofunc ( interfaces::vgui_panel, panel, force_repaint, allow_force );

    if ( g_unload )
        return;

    if ( !panel_id )
    {
        const auto panel_name = interfaces::vgui_panel->GetName ( panel );

        if ( !strcmp ( panel_name, xor_str ( "FocusOverlayPanel" ) ) )
            panel_id = panel;
    }
    else if ( panel_id == panel )
    {
        //Ignore 50% cuz it called very often
        static auto bSkip = false;
        bSkip = !bSkip;

        if ( bSkip )
            return;

        /* if ( g_local && InputSys::get().IsKeyDown ( VK_TAB ) && g_Options.misc_showranks )
             utils::rank_reveal_all();*/

        static auto inited = false;

        if ( !inited )
        {
            g_fonts.init( );
            inited = true;
        }

        //g_features.visuals.watermark();

        g_features.visuals.add_to_draw_list( );

        //g_lua.on_function_call ( xor_str ( "draw" ) );
        if ( !interfaces::engine_client->is_in_game( ) )
        {
            g_features.animations.m_should_update_fake = true;
            g_features.animations.m_should_update_real = true;
            g_features.animations.m_should_update_entity_animstate = true;
        }

        //static hades_menu m;
        hades_menu.test_on_draw( );

        if ( !interfaces::engine_client->is_in_game( ) )
        {
            for ( auto& data_struct : g_features.resolver.rdata )
            {
                data_struct = { };
            }
        }
    }
}
