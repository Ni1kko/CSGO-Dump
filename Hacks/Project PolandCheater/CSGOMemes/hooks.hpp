#pragma once

#include "ValveSDK/interfaces.hpp"
#include "Helpers/vfunc_hook.hpp"
#include <d3d9types.h>


namespace hooks
{
    void initialize();
    void cleanup();

    using paint_traverse_t = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
    using d3d9_endscene = long(__stdcall *)(IDirect3DDevice9*);
    using d3d9_reset = long(__stdcall *)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    using create_move_t = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
    using fsn_t = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
    using play_sound_t = void(__thiscall*)(ISurface*, const char* );
    using dme_t = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
    typedef bool(__thiscall *fire_event_t)(void*, IGameEvent*);
    using trace_ray_t = void(__thiscall*)(IEngineTrace*, const Ray_t&, unsigned int, ITraceFilter *, CGameTrace *);
    //using render_view_t = void(__thiscall*)(IViewRender*, const CViewSetup&, CViewSetup&, int, int);
    using do_post_screen_space_effects_t = bool(__thiscall*)(void*, CViewSetup*);
    using get_view_model_fov_t = float(__thiscall*)(void*);
    using override_view_t = void(__thiscall*)(void* _this, CViewSetup* setup);
    using in_prediction_t = bool(__thiscall*)(PVOID);
    using is_hltv_t = bool(__thiscall*)(void*);

    using ticket_t = void(__stdcall*)(int);

    typedef bool(__stdcall *FnCreateMove)(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
    void __stdcall received_ticket(int a1);
    long __stdcall end_scene(IDirect3DDevice9* device);
    long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
    //void __stdcall render_view(const CViewSetup& view, CViewSetup& a3, int a4, int a5);
    void __stdcall create_move_proxy(int sequence_number, float input_sample_frametime, bool active);
    void __stdcall paint_traverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
    void __stdcall frame_stage_notify(ClientFrameStage_t stage);
    void __stdcall play_sound(const char* name);
    void __stdcall draw_model_execute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
    bool __fastcall fire_event_client_side(void* ecx, void* edx, IGameEvent* event);
    void __stdcall trace_ray(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace);
    float __fastcall get_view_model_fov(void* ecx, void* edx);
    void __fastcall override_view(void* _this, void* _edx, CViewSetup* setup);
    bool __stdcall in_prediction();
    bool __fastcall is_hltv(void* ecx, void* edx);
    bool _fastcall do_post_screen_space_effects(void* ecx, void* edx, CViewSetup* pSetup);
    void sequence_hook(const CRecvProxyData *pDataConst, void *pStruct, void *pOut);
    bool __stdcall CreateMove(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
}