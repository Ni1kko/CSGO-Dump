#pragma once

#include "../math/QAngle.hpp"
#include "../misc/Studio.hpp"
#include "IEngineTrace.hpp" //Has some structs we need here

class CPhysCollide;
class CUtlBuffer;
class IClientRenderable;
class CStudioHdr;
struct virtualmodel_t;

enum RenderableTranslucencyType_t
{
    RENDERABLE_IS_OPAQUE = 0,
    RENDERABLE_IS_TRANSLUCENT,
    RENDERABLE_IS_TWO_PASS,
    // has both translucent and opaque sub-partsa
};

class IVModelInfo
{
public:
    virtual ~IVModelInfo ( void )
    {
    }

    virtual const model_t* get_model ( int modelindex ) const = 0;
    virtual int get_model_index ( const char* name ) const = 0;
    virtual const char* get_model_name ( const model_t* model ) const = 0;
    virtual vcollide_t* get_vcollide ( const model_t* model ) const = 0;
    virtual vcollide_t* get_vcollide ( int modelindex ) const = 0;
    virtual void get_model_bounds ( const model_t* model, Vector& mins, Vector& maxs ) const = 0;
    virtual void get_model_render_bounds ( const model_t* model, Vector& mins, Vector& maxs ) const = 0;
    virtual int get_model_frame_count ( const model_t* model ) const = 0;
    virtual int get_model_type ( const model_t* model ) const = 0;
    virtual void* get_model_extra_data ( const model_t* model ) = 0;
    virtual bool model_has_material_proxy ( const model_t* model ) const = 0;
    virtual bool is_translucent ( model_t const* model ) const = 0;
    virtual bool is_translucent_two_pass ( const model_t* model ) const = 0;

    virtual void Unused0 ( )
    {
    };
    virtual void UNUSED ( ) = 0;
    virtual void UNUSE11D ( ) = 0;
    virtual RenderableTranslucencyType_t compute_translucency_type ( const model_t* model, int nSkin, int nBody ) = 0;
    virtual int get_model_material_count ( const model_t* model ) const = 0;
    virtual void get_model_materials ( const model_t* model, int count, IMaterial** ppMaterial ) = 0;
    virtual bool is_model_vertex_lit ( const model_t* model ) const = 0;
    virtual const char* get_model_key_value_text ( const model_t* model ) = 0;
    virtual bool get_model_key_value ( const model_t* model, CUtlBuffer& buf ) = 0;
    virtual float get_model_radius ( const model_t* model ) = 0;
    virtual CStudioHdr* get_studio_hdr ( mdl_handle_t handle ) = 0;
    virtual const studiohdr_t* find_model ( const studiohdr_t* pStudioHdr, void** cache, const char* modelname ) const = 0;
    virtual const studiohdr_t* find_model ( void* cache ) const = 0;
    virtual virtualmodel_t* get_virtual_model ( const studiohdr_t* pStudioHdr ) const = 0;
    virtual uint8_t* get_anim_block ( const studiohdr_t* pStudioHdr, int iBlock ) const = 0;
    virtual void get_model_material_color_and_lighting ( const model_t* model, Vector const& origin, QAngle const& angles, trace_t* pTrace, Vector& lighting,
                                                         Vector& matColor ) = 0;
    virtual void get_illumination_point ( const model_t* model, IClientRenderable* pRenderable, Vector const& origin, QAngle const& angles,
                                          Vector* pLightingCenter ) = 0;
    virtual int get_model_contents ( int modelIndex ) const = 0;
    virtual studiohdr_t* get_studiomodel ( const model_t* mod ) = 0;
    virtual int get_model_sprite_width ( const model_t* model ) const = 0;
    virtual int get_model_sprite_height ( const model_t* model ) const = 0;
    virtual void set_level_screen_fade_range ( float flMinSize, float flMaxSize ) = 0;
    virtual void get_level_screen_fade_range ( float* pMinArea, float* pMaxArea ) const = 0;
    virtual void set_view_screen_fade_range ( float flMinSize, float flMaxSize ) = 0;
    virtual unsigned char compute_level_screen_fade ( const Vector& vecAbsOrigin, float flRadius, float flFadeScale ) const = 0;
    virtual unsigned char compute_view_screen_fade ( const Vector& vecAbsOrigin, float flRadius, float flFadeScale ) const = 0;
    virtual int get_autoplay_list ( const studiohdr_t* pStudioHdr, unsigned short** pAutoplayList ) const = 0;
    virtual CPhysCollide* get_collide_for_virtual_terrain ( int index ) = 0;
    virtual bool is_using_fbtexture ( const model_t* model, int nSkin, int nBody, IClientRenderable* * pClientRenderable ) const = 0;
    virtual const model_t* find_or_load_model ( const char* name ) const = 0;
    virtual mdl_handle_t get_cache_handle ( const model_t* model ) const = 0;
    virtual int get_brush_model_plane_count ( const model_t* model ) const = 0;
    virtual void get_brush_model_plane ( const model_t* model, int nIndex, cplane_t& plane, Vector* pOrigin ) const = 0;
    virtual int get_surfaceprops_for_virtual_terrain ( int index ) = 0;
    virtual bool uses_env_cubemap ( const model_t* model ) const = 0;
    virtual bool uses_static_lighting ( const model_t* model ) const = 0;
};

class IVModelInfoClient : public IVModelInfo
{
public:
};
