#pragma once

#include "../math/VMatrix.hpp"

class IPanel;
class C_BaseEntity;

enum class ClearFlags_t
{
    VIEW_CLEAR_COLOR = 0x1,
    VIEW_CLEAR_DEPTH = 0x2,
    VIEW_CLEAR_FULL_TARGET = 0x4,
    VIEW_NO_DRAW = 0x8,
    VIEW_CLEAR_OBEY_STENCIL = 0x10,
    VIEW_CLEAR_STENCIL = 0x20,
};

enum class MotionBlurMode_t
{
    MOTION_BLUR_DISABLE = 1,
    MOTION_BLUR_GAME = 2,
    MOTION_BLUR_SFM = 3
};

class CViewSetup
{
public:
    __int32 x; //0x0000
    __int32 x_old; //0x0004
    __int32 y; //0x0008
    __int32 y_old; //0x000C
    __int32 width; //0x0010
    __int32 width_old; //0x0014
    __int32 height; //0x0018
    __int32 height_old; //0x001C
    char pad_0x0020[0x90]; //0x0020
    float fov; //0x00B0
    float viewmodel_fov; //0x00B4
    Vector origin; //0x00B8
    Vector angles; //0x00C4
    char pad_0x00D0[0x7C]; //0x00D0
}; //Size=0x014C

class IClientMode
{
public:
    virtual ~IClientMode ( )
    {
    }

    virtual int client_mode_cs_normal ( void* ) = 0;
    virtual void init ( ) = 0;
    virtual void init_viewport ( ) = 0;
    virtual void shutdown ( ) = 0;
    virtual void enable ( ) = 0;
    virtual void disable ( ) = 0;
    virtual void layout ( ) = 0;
    virtual IPanel* get_viewport ( ) = 0;
    virtual void* get_viewport_animatio_ncontroller ( ) = 0;
    virtual void process_input ( bool bActive ) = 0;
    virtual bool should_draw_detail_objects ( ) = 0;
    virtual bool should_draw_entity ( C_BaseEntity* pEnt ) = 0;
    virtual bool should_draw_localplayer ( C_BaseEntity* pPlayer ) = 0;
    virtual bool should_draw_particles ( ) = 0;
    virtual bool should_draw_fog ( void ) = 0;
    virtual void override_view ( CViewSetup* pSetup ) = 0;
    virtual int key_input ( int down, int keynum, const char* pszCurrentBinding ) = 0;
    virtual void start_message_mode ( int iMessageModeType ) = 0;
    virtual IPanel* get_message_panel ( ) = 0;
    virtual void override_mouse_input ( float* x, float* y ) = 0;
    virtual bool create_move ( float flInputSampleTime, void* usercmd ) = 0;
    virtual void level_init ( const char* newmap ) = 0;
    virtual void level_shutdown ( void ) = 0;
};
