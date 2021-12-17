#pragma once

#define NOMINMAX
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

// include this at the top so we can access it in every single file
#include "../build_config.h"

#include "../helpers/globals.h"

#include "misc/Enums.hpp"
#include "misc/vfunc.hpp"
#include "../helpers/security/security.hpp"
#include "../helpers/lazyimporter.h"

#include "math/VMatrix.hpp"
#include "math/QAngle.hpp"
#include "math/Vector.hpp"
#include "misc/Studio.hpp"

#include "interfaces/IAppSystem.hpp"
#include "interfaces/IBaseClientDll.hpp"
#include "interfaces/IClientEntity.hpp"
#include "interfaces/IClientEntityList.hpp"
#include "interfaces/IClientMode.hpp"
#include "interfaces/icon_var.hpp"
#include "interfaces/ICvar.hpp"
#include "interfaces/IEngineTrace.hpp"
#include "interfaces/IVEngineClient.hpp"
#include "interfaces/IVDebugOverlay.hpp"
#include "interfaces/ISurface.hpp"
#include "interfaces/c_input.hpp"
#include "interfaces/IVModelInfoClient.hpp"
#include "interfaces/IVModelRender.hpp"
#include "interfaces/IRenderView.hpp"
#include "interfaces/IGameEventmanager.hpp"
#include "interfaces/IMaterialSystem.hpp"
#include "interfaces/IMoveHelper.hpp"
#include "interfaces/IMDLCache.hpp"
#include "interfaces/IPrediction.hpp"
#include "interfaces/IPanel.hpp"
#include "interfaces/IEngineSound.hpp"
#include "interfaces/IViewRender.hpp"
#include "interfaces/CClientState.hpp"
#include "interfaces/IPhysics.hpp"
#include "interfaces/IInputSystem.hpp"
#include "interfaces/IViewRenderBeams.h"
#include "interfaces/memalloc.h"

#include "misc/Convar.hpp"
#include "misc/usercmd_t.hpp"
#include "misc/glow_outline_effect.hpp"

//#include "..\helpers\globals.h"
#include "netvars.hpp"

struct IDirect3DDevice9;

namespace interfaces
{
    void initialize ( );
    void dump ( );

    extern IVEngineClient* engine_client; //
    extern IBaseClientDLL* chl_client; //
    extern IClientEntityList* entity_list; //
    extern c_global_vars_base* global_vars; //
    extern IEngineTrace* engine_trace; //
    extern ICvar* cvar; //
    extern IPanel* vgui_panel; //
    extern IClientMode* client_mode; //
    extern IVDebugOverlay* debug_overlay; //
    extern ISurface* vgui_surface; //
    extern c_input* input; //
    extern IVModelInfoClient* mdl_info; //
    extern IVModelRender* mdl_render; //
    extern IVRenderView* render_view; //
    extern IMaterialSystem* mat_system; //
    extern IGameEventManager2* game_events; //
    extern IMoveHelper* move_helper; //
    extern IMDLCache* mdl_cache; //
    extern IPrediction* prediction; //
    extern CGameMovement* game_movement; //
    extern IEngineSound* engine_sound; //
    extern CGlowObjectManager* glow_obj_manager; //
    extern IViewRender* view_render; //
    extern IDirect3DDevice9* d3d_device9; //
    extern CClientState* client_state; //
    extern IPhysicsSurfaceProps* phys_surface; //
    extern IInputSystem* input_system; //
    extern IViewRenderBeams* render_beams;
    extern IMemAlloc* mem_alloc;
    //extern IClientRenderable* dwCCSPlayerRenderablevftable;
}

//namespace i = interfaces;

template < typename... args >
void con_msg ( const char* pMsg, args ... args )
{
    static auto import = reinterpret_cast< void ( *) ( const char*, ... ) > ( GetProcAddress ( GetModuleHandleW ( L"tier0.dll" ), "?ConMsg@@YAXPBDZZ" ) );
    return import ( pMsg, args... );
}

template < typename... Args >
void con_color_msg ( const Color& clr, const char* pMsg, Args ... args )
{
    static auto import = reinterpret_cast< void ( *) ( const Color&, const char*, ... ) > ( GetProcAddress (
            GetModuleHandleW ( L"tier0.dll" ), "?ConColorMsg@@YAXABVColor@@PBDZZ" )
    );
    return import ( clr, pMsg, args... );
}

#include "misc/EHandle.hpp"

class C_LocalPlayer
{
    friend bool operator== ( const C_LocalPlayer& lhs, void* rhs );
public:
    C_LocalPlayer ( ) : m_local_ ( nullptr )
    {
    }

    operator bool ( ) const
    {
        return *m_local_ != nullptr;
    }

    operator C_BasePlayer* ( ) const
    {
        return *m_local_;
    }

    C_BasePlayer* operator-> ( ) const
    {
        return *m_local_;
    }

private:
    C_BasePlayer** m_local_;
};

extern C_LocalPlayer g_local;

#include "../helpers/ctx/ctx.hpp"
