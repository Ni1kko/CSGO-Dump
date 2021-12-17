#include "sdk.hpp"
#include "../helpers/utils.hpp"
#include "../helpers/console.h"

C_LocalPlayer g_local;

namespace interfaces
{
    IVEngineClient* engine_client = nullptr;
    IBaseClientDLL* chl_client = nullptr;
    IClientEntityList* entity_list = nullptr;
    c_global_vars_base* global_vars = nullptr;
    IEngineTrace* engine_trace = nullptr;
    ICvar* cvar = nullptr;
    IPanel* vgui_panel = nullptr;
    IClientMode* client_mode = nullptr;
    IVDebugOverlay* debug_overlay = nullptr;
    ISurface* vgui_surface = nullptr;
    c_input* input = nullptr;
    IVModelInfoClient* mdl_info = nullptr;
    IVModelRender* mdl_render = nullptr;
    IVRenderView* render_view = nullptr;
    IMaterialSystem* mat_system = nullptr;
    IGameEventManager2* game_events = nullptr;
    IMoveHelper* move_helper = nullptr;
    IMDLCache* mdl_cache = nullptr;
    IPrediction* prediction = nullptr;
    CGameMovement* game_movement = nullptr;
    IEngineSound* engine_sound = nullptr;
    CGlowObjectManager* glow_obj_manager = nullptr;
    IViewRender* view_render = nullptr;
    IDirect3DDevice9* d3d_device9 = nullptr;
    CClientState* client_state = nullptr;
    IPhysicsSurfaceProps* phys_surface = nullptr;
    IInputSystem* input_system = nullptr;
    IViewRenderBeams* render_beams = nullptr;
    IMemAlloc* mem_alloc = nullptr;

    CreateInterfaceFn get_module_factory ( const HMODULE module )
    {
        return reinterpret_cast< CreateInterfaceFn > ( GetProcAddress ( module, xor_str ( "CreateInterface" ) ) );
    }

    template < typename T >
    T* get_interface ( const CreateInterfaceFn f, const char* sz_interface_version )
    {
        auto result = reinterpret_cast< T* > ( f ( sz_interface_version, nullptr ) );

        if ( !result )
            throw std::runtime_error ( std::string ( xor_str ( "[get_interface] Failed to GetOffset interface: " ) ) + sz_interface_version );

        return result;
    }

    void initialize ( )
    {
        const auto engine_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "engine.dll" ) ) );
        const auto client_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "client_panorama.dll" ) ) );
        const auto valve_std_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "vstdlib.dll" ) ) );
        const auto vgui_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "vguimatsurface.dll" ) ) );
        const auto vgui2_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "vgui2.dll" ) ) );
        const auto mat_sys_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "materialsystem.dll" ) ) );
        const auto data_cache_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "datacache.dll" ) ) );
        const auto vphysics_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "vphysics.dll" ) ) );
        const auto input_sys_factory = get_module_factory ( GetModuleHandleA ( xor_str ( "inputsystem.dll" ) ) );

        chl_client = get_interface< IBaseClientDLL > ( client_factory, xor_str ( "VClient018" ) );
        entity_list = get_interface< IClientEntityList > ( client_factory, xor_str ( "VClientEntityList003" ) );
        prediction = get_interface< IPrediction > ( client_factory, xor_str ( "VClientPrediction001" ) );
        game_movement = get_interface< CGameMovement > ( client_factory, xor_str ( "GameMovement001" ) );
        mdl_cache = get_interface< IMDLCache > ( data_cache_factory, xor_str ( "MDLCache004" ) );
        engine_client = get_interface< IVEngineClient > ( engine_factory, xor_str ( "VEngineClient014" ) );
        mdl_info = get_interface< IVModelInfoClient > ( engine_factory, xor_str ( "VModelInfoClient004" ) );
        mdl_render = get_interface< IVModelRender > ( engine_factory, xor_str ( "VEngineModel016" ) );
        render_view = get_interface< IVRenderView > ( engine_factory, xor_str ( "VEngineRenderView014" ) );
        engine_trace = get_interface< IEngineTrace > ( engine_factory, xor_str ( "EngineTraceClient004" ) );
        debug_overlay = get_interface< IVDebugOverlay > ( engine_factory, xor_str ( "VDebugOverlay004" ) );
        game_events = get_interface< IGameEventManager2 > ( engine_factory, xor_str ( "GAMEEVENTSMANAGER002" ) );
        engine_sound = get_interface< IEngineSound > ( engine_factory, xor_str ( "IEngineSoundClient003" ) );
        mat_system = get_interface< IMaterialSystem > ( mat_sys_factory, xor_str ( "VMaterialSystem080" ) );
        cvar = get_interface< ICvar > ( valve_std_factory, xor_str ( "VEngineCvar007" ) );
        vgui_panel = get_interface< IPanel > ( vgui2_factory, xor_str ( "VGUI_Panel009" ) );
        vgui_surface = get_interface< ISurface > ( vgui_factory, xor_str ( "VGUI_Surface031" ) );
        phys_surface = get_interface< IPhysicsSurfaceProps > ( vphysics_factory, xor_str ( "VPhysicsSurfaceProps001" ) );
        input_system = get_interface< IInputSystem > ( input_sys_factory, xor_str ( "InputSystemVersion001" ) );
        mem_alloc = *reinterpret_cast< IMemAlloc** > ( GetProcAddress ( GetModuleHandleA ( xor_str ( "tier0.dll" ) ), xor_str ( "g_pMemAlloc" ) ) );

        //const auto client = GetModuleHandleA ( xor_str ( "client_panorama.dll" ) );
        //const auto engine = GetModuleHandleA ( xor_str ( "engine.dll" ) );
        //const auto dx9api = GetModuleHandleA ( xor_str ( "shaderapidx9.dll" ) );

        global_vars = ** reinterpret_cast< c_global_vars_base*** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                            xor_str ( "A1 ? ? ? ? 5E 8B 40 10" ) ) + 1 );
        client_mode = * reinterpret_cast< IClientMode** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                   xor_str ( "B9 ? ? ? ? E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 53" ) ) + 1 );
        input = * reinterpret_cast< c_input** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                         xor_str ( "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85" ) ) + 1 );
        move_helper = ** reinterpret_cast< IMoveHelper*** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                     xor_str ( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ) ) + 2 );
        glow_obj_manager = * reinterpret_cast< CGlowObjectManager** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                               xor_str ( "0F 11 05 ? ? ? ? 83 C8 01" ) ) + 3 );
        view_render = * reinterpret_cast< IViewRender** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                   xor_str ( "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10" ) ) + 1 );
        d3d_device9 = ** reinterpret_cast< IDirect3DDevice9*** > ( g_utils.pattern_scan ( xor_str ( "shaderapidx9.dll" ),
                                                                                          xor_str ( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) ) + 1 );
        client_state = ** reinterpret_cast< CClientState*** > ( g_utils.pattern_scan ( xor_str ( "engine.dll" ), xor_str ( "A1 ? ? ? ? 8B 80 ? ? ? ? C3" ) )
            + 1 );
        render_beams = * reinterpret_cast< IViewRenderBeams** > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                         xor_str ( "A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) ) + 1 );
        //dwCCSPlayerRenderablevftable = ** ( IClientRenderable*** ) ( g_utils.pattern_scan ( client, "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C" ) + 0x4E ); //C7 47 ? ? ? ? ? C7 47 ? ? ? ? ? C7 47 ? ? ? ? ? C7 47 ? ? ? ? ? C7 07 ? ? ? ? C7 47 ? ? ? ? ? + 4
        g_local = * reinterpret_cast< C_LocalPlayer* > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                xor_str ( "8B 0D ? ? ? ? 83 FF FF 74 07" ) ) + 2 );
    }

    void dump ( )
    {
#ifdef _DEBUG
#define STRINGIFY_IMPL(s) #s
#define STRINGIFY(s)      STRINGIFY_IMPL(s)
#define PRINT_INTERFACE(name) console::print("%-20s: 0x%p\n", STRINGIFY(name), name)

        PRINT_INTERFACE ( chl_client );
        PRINT_INTERFACE ( entity_list );
        PRINT_INTERFACE ( prediction );
        PRINT_INTERFACE ( game_movement );
        PRINT_INTERFACE ( mdl_cache );
        PRINT_INTERFACE ( engine_client );
        PRINT_INTERFACE ( mdl_info );
        PRINT_INTERFACE ( mdl_render );
        PRINT_INTERFACE ( render_view );
        PRINT_INTERFACE ( engine_trace );
        PRINT_INTERFACE ( debug_overlay );
        PRINT_INTERFACE ( game_events );
        PRINT_INTERFACE ( engine_sound );
        PRINT_INTERFACE ( mat_system );
        PRINT_INTERFACE ( cvar );
        PRINT_INTERFACE ( vgui_panel );
        PRINT_INTERFACE ( vgui_surface );
        PRINT_INTERFACE ( phys_surface );
        PRINT_INTERFACE ( input_system );
        PRINT_INTERFACE ( input );
        PRINT_INTERFACE ( g_local );
#endif // _DEBUG
    }
}
