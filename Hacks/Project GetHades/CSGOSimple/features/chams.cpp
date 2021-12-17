#include "chams.h"
#include "../options.h"
#include "../hooks/hooks.hpp"
#include "../helpers/math.hpp"
#include "../helpers/console.h"
#include "../helpers/helpers.h"
#include "features.h"

void c_chams::on_draw_model_execute ( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo, matrix3x4_t* matrix ) const
{
    static auto ofunc = hooks::mdlrender_hook.get_original< hooks::draw_model_execute > ( index::draw_model_execute );

    auto entity = dynamic_cast< C_BaseEntity* > ( interfaces::entity_list->get_client_entity ( pinfo.entity_index ) );

    if ( !entity || entity->is_dormant( ) || !g_local || !matrix || !g_options.chams_master )
        return ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );

    auto local = static_cast< C_BasePlayer* > ( g_local );

    if ( entity->is_player( ) )
    {
        const auto player = dynamic_cast< C_BasePlayer* > ( entity );

        if ( !player )
            return;

        if ( local && player == local && local->is_alive( ) && interfaces::global_vars->curtime - local->spawn_time( ) > 3.f )
        {
            if ( g_options.chams_fake_desync && g_features.animations.init_fake_anim )
            {
                if ( g_features.animations.m_got_fake_matrix )
                {
                    for ( auto& i : g_features.animations.m_fake_matrix )
                    {
                        i[ 0 ][ 3 ] += pinfo.origin.x;
                        i[ 1 ][ 3 ] += pinfo.origin.y;
                        i[ 2 ][ 3 ] += pinfo.origin.z;
                    }

                    g_helpers.material_helper.overwrite ( false, true, false, false, g_options.color_chams_fake_desync );
                    interfaces::render_view->set_blend ( 0.25f + sin ( interfaces::global_vars->curtime * 5.f ) * .2f );
                    ofunc ( interfaces::mdl_render, ctx, state, pinfo, g_features.animations.m_fake_matrix );
                    interfaces::mdl_render->forced_material_override ( nullptr );
                    interfaces::render_view->set_blend ( 1.f );

                    for ( auto& i : g_features.animations.m_fake_matrix )
                    {
                        i[ 0 ][ 3 ] -= pinfo.origin.x;
                        i[ 1 ][ 3 ] -= pinfo.origin.y;
                        i[ 2 ][ 3 ] -= pinfo.origin.z;
                    }
                }
            }

            if ( g_options.chams_fake_pos && g_features.animations.m_got_fake_matrix )
            {
                interfaces::render_view->set_blend ( 0.25f + sin ( interfaces::global_vars->curtime * 5.f ) * .2f );
                g_helpers.material_helper.overwrite ( false, true, false, false, g_options.color_chams_fake_pos );
                ofunc ( interfaces::mdl_render, ctx, state, pinfo, g_features.animations.m_fake_position_matrix );
                interfaces::mdl_render->forced_material_override ( nullptr );
                interfaces::render_view->set_blend ( 1.f );
            }

            switch ( static_cast< chams_modes_t > ( g_options.chams_type_local ) )
            {
            case chams_modes_t::none:
                {
                    interfaces::render_view->set_blend ( 1.f );
                }
                break;

            case chams_modes_t::normal:
                {
                    g_helpers.material_helper.overwrite ( false, false, false, false, g_options.chams_color_local );
                }
                break;

            case chams_modes_t::flat:
                {
                    g_helpers.material_helper.overwrite ( false, true, false, false, g_options.chams_color_local );
                }
                break;

            case chams_modes_t::metallic:
                {
                    g_helpers.material_helper.overwrite ( false, false, false, true, g_options.chams_color_local );
                }
                break;
            }

            if ( g_local && g_local->is_scoped( ) )
                interfaces::render_view->set_blend ( g_options.visuals_scope_transparency );

            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
        }
        else
        {
            if ( player->is_alive( ) )
            {
                if ( player->is_enemy( ) )
                {
                    if ( g_options.chams_enemy_backtrack && !g_features.rage_backtrack.saved_data[ player->ent_index( ) ].empty( ) )
                    {
                        interfaces::mdl_render->forced_material_override ( nullptr );
                        interfaces::render_view->set_blend ( 1.f );

                        g_helpers.material_helper.overwrite ( true, true, false, false, g_options.color_chams_backtrack );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo,
                                g_features.rage_backtrack.saved_data[ player->ent_index( ) ].front( ).ndata.matrix );
                    }

                    interfaces::render_view->set_blend ( 0.5f + sin ( interfaces::global_vars->realtime * 5.f ) * 0.5f );

                    switch ( static_cast< chams_modes_t > ( g_options.chams_type_enemy ) )
                    {
                    case chams_modes_t::none:
                        {
                            interfaces::render_view->set_blend ( 1.f );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                            interfaces::mdl_render->forced_material_override ( nullptr );
                            interfaces::render_view->set_blend ( 1.f );
                            return;
                        }

                    case chams_modes_t::normal:
                        {
                            g_helpers.material_helper.overwrite ( true, false, false, false, g_options.chams_color_xqz_enemy );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                            interfaces::mdl_render->forced_material_override ( nullptr );
                            interfaces::render_view->set_blend ( 1.f );

                            g_helpers.material_helper.overwrite ( false, false, false, false, g_options.chams_color_enemy );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        }
                        break;

                    case chams_modes_t::flat:
                        {
                            g_helpers.material_helper.overwrite ( true, true, false, false, g_options.chams_color_xqz_enemy );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                            interfaces::mdl_render->forced_material_override ( nullptr );
                            interfaces::render_view->set_blend ( 1.f );

                            g_helpers.material_helper.overwrite ( false, true, false, false, g_options.chams_color_enemy );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        }
                        break;

                    case chams_modes_t::metallic:
                        {
                            g_helpers.material_helper.overwrite ( true, false, false, true, g_options.chams_color_xqz_enemy );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                            interfaces::mdl_render->forced_material_override ( nullptr );
                            interfaces::render_view->set_blend ( 1.f );

                            g_helpers.material_helper.overwrite ( false, false, false, true, g_options.chams_color_enemy );
                            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        }
                        break;
                    }

                    return;
                }

                interfaces::render_view->set_blend ( 0.5f + sin ( interfaces::global_vars->realtime * 5.f ) * 0.5f );

                switch ( static_cast< chams_modes_t > ( g_options.chams_type_team ) )
                {
                case chams_modes_t::none:
                    {
                        interfaces::render_view->set_blend ( 1.f );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        interfaces::mdl_render->forced_material_override ( nullptr );
                        interfaces::render_view->set_blend ( 1.f );
                        return;
                    }

                case chams_modes_t::normal:
                    {
                        g_helpers.material_helper.overwrite ( true, false, false, false, g_options.chams_color_xqz_team );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        interfaces::mdl_render->forced_material_override ( nullptr );
                        interfaces::render_view->set_blend ( 1.f );

                        g_helpers.material_helper.overwrite ( false, false, false, false, g_options.chams_color_team );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                    }
                    break;

                case chams_modes_t::flat:
                    {
                        g_helpers.material_helper.overwrite ( true, true, false, false, g_options.chams_color_xqz_team );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        interfaces::mdl_render->forced_material_override ( nullptr );
                        interfaces::render_view->set_blend ( 1.f );

                        g_helpers.material_helper.overwrite ( false, true, false, false, g_options.chams_color_team );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                    }
                    break;

                case chams_modes_t::metallic:
                    {
                        g_helpers.material_helper.overwrite ( true, false, false, true, g_options.chams_color_xqz_team );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                        interfaces::mdl_render->forced_material_override ( nullptr );
                        interfaces::render_view->set_blend ( 1.f );

                        g_helpers.material_helper.overwrite ( false, false, false, true, g_options.chams_color_team );
                        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
                    }
                    break;
                }
            }

            ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
        }
    }
    else
        ofunc ( interfaces::mdl_render, ctx, state, pinfo, matrix );
}
