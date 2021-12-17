#pragma once

#include "../misc/GlobalVars.hpp"
#include "../misc/ClientClass.hpp"

enum ClientFrameStage_t
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};

// Used by RenderView
enum RenderViewInfo_t
{
    RENDERVIEW_UNSPECIFIED = 0,
    RENDERVIEW_DRAWVIEWMODEL = ( 1 << 0 ),
    RENDERVIEW_DRAWHUD = ( 1 << 1 ),
    RENDERVIEW_SUPPRESSMONITORRENDERING = ( 1 << 2 ),
};

class IBaseClientDLL
{
public:
    virtual int connect ( CreateInterfaceFn appSystemFactory, c_global_vars_base* pGlobals ) = 0;
    virtual int disconnect ( void ) = 0;
    virtual int init ( CreateInterfaceFn appSystemFactory, c_global_vars_base* pGlobals ) = 0;
    virtual void post_init ( ) = 0;
    virtual void shutdown ( void ) = 0;
    virtual void level_init_pre_entity ( char const* pMapName ) = 0;
    virtual void level_init_post_entity ( ) = 0;
    virtual void level_Shutdown ( void ) = 0;
    virtual ClientClass* get_all_classes ( void ) = 0;
};
