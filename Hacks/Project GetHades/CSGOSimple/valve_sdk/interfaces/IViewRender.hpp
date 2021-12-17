#pragma once

enum DrawFlags_t
{
    DF_RENDER_REFRACTION = 0x1,
    DF_RENDER_REFLECTION = 0x2,
    DF_CLIP_Z = 0x4,
    DF_CLIP_BELOW = 0x8,
    DF_RENDER_UNDERWATER = 0x10,
    DF_RENDER_ABOVEWATER = 0x20,
    DF_RENDER_WATER = 0x40,
    DF_UNUSED1 = 0x100,
    DF_WATERHEIGHT = 0x200,
    DF_UNUSED2 = 0x400,
    DF_DRAWSKYBOX = 0x800,
    DF_FUDGE_UP = 0x1000,
    DF_DRAW_ENTITITES = 0x2000,
    DF_UNUSED3 = 0x4000,
    DF_UNUSED4 = 0x8000,
    DF_UNUSED5 = 0x10000,
    DF_SAVEGAMESCREENSHOT = 0x20000,
    DF_CLIP_SKYBOX = 0x40000,
    DF_SHADOW_DEPTH_MAP = 0x100000 // Currently rendering a shadow depth map
};

//-----------------------------------------------------------------------------
// Purpose: View setup and rendering
//-----------------------------------------------------------------------------
class CViewSetup;
class C_BaseEntity;
struct vrect_t;
class C_BaseViewModel;
class IMaterial;

class IViewRender
{
public:
    virtual void init ( void ) = 0;
    virtual void level_init ( void ) = 0;
    virtual void level_shutdown ( void ) = 0;
    virtual void shutdown ( void ) = 0;
    virtual void on_render_start ( ) = 0;
    virtual void render ( vrect_t* rect ) = 0;
    virtual void render_view ( const CViewSetup& view, int nClearFlags, int whatToDraw ) = 0;
    virtual int get_draw_flags ( ) = 0;
    virtual void start_pitch_drift ( void ) = 0;
    virtual void stop_pitch_drift ( void ) = 0;
    virtual void* get_frustum ( ) = 0;
    virtual bool should_draw_brush_models ( void ) = 0;
    virtual const CViewSetup* get_player_view_setup ( void ) const = 0;
    virtual const CViewSetup* get_view_setup ( void ) const = 0;
    virtual void disable_vis ( void ) = 0;
    virtual int build_world_lists_number ( ) const = 0;
    virtual void set_cheap_water_start_distance ( float flCheapWaterStartDistance ) = 0;
    virtual void set_cheap_water_end_distance ( float flCheapWaterEndDistance ) = 0;
    virtual void get_water_lod_params ( float& flCheapWaterStartDistance, float& flCheapWaterEndDistance ) = 0;
    virtual void drift_pitch ( void ) = 0;
    virtual void set_screen_overlay_material ( IMaterial* pMaterial ) = 0;
    virtual IMaterial* get_screen_overlay_material ( ) = 0;
    virtual void write_save_game_screenshot ( const char* pFilename ) = 0;
    virtual void write_save_game_screenshot_of_size ( const char* pFilename, int width, int height ) = 0;
    virtual void queue_overlay_render_view ( const CViewSetup& view, int nClearFlags, int whatToDraw ) = 0;
    virtual float get_znear ( ) = 0;
    virtual float get_zfar ( ) = 0;
    virtual void get_screen_fade_distances ( float* min, float* max ) = 0;
    virtual C_BaseEntity* get_currently_drawing_entity ( ) = 0;
    virtual void set_currently_drawing_entity ( C_BaseEntity* pEnt ) = 0;
};
