#include "hooks.hpp"
#include "../options.h"
#include "../helpers/math.hpp"
#include "../features/features.h"

void __stdcall hooks::hk_draw_model_execute ( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo,
                                              matrix3x4_t* custom_bone_to_world )
{
    static auto ofunc = mdlrender_hook.get_original< draw_model_execute > ( index::draw_model_execute );

    if ( g_unload || !custom_bone_to_world || !interfaces::render_view )
        return ofunc ( interfaces::mdl_render, ctx, state, pinfo, custom_bone_to_world );

    //static auto last_simtime = 0.f;

    auto entity = dynamic_cast< C_BasePlayer* > ( interfaces::entity_list->get_client_entity ( pinfo.entity_index ) );

    if ( entity && entity->is_player( ) && entity == g_local )
    {
        if ( !g_features.animations.m_got_real_matrix )
            return;

        if ( !custom_bone_to_world )
            return;

        memcpy ( custom_bone_to_world, g_features.animations.m_real_matrix, sizeof ( g_features.animations.m_real_matrix ) );

        for ( auto i = 0; i < maxstudiobones; i++ )
        {
            custom_bone_to_world[ i ][ 0 ][ 3 ] += pinfo.origin.x;
            custom_bone_to_world[ i ][ 1 ][ 3 ] += pinfo.origin.y;
            custom_bone_to_world[ i ][ 2 ][ 3 ] += pinfo.origin.z;
        }
    }

    g_features.chams.on_draw_model_execute ( ctx, state, pinfo, custom_bone_to_world );

    interfaces::mdl_render->forced_material_override ( nullptr );
    interfaces::render_view->set_blend ( 1.f );
}
