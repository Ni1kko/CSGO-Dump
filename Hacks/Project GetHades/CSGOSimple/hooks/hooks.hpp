#pragma once

#include "../helpers/vfunc_hook.hpp"

#include "../valve_sdk/csgostructs.hpp"

#include <d3d9.h>

namespace index
{
    constexpr auto emit_sound1 = 5;
    constexpr auto emit_sound2 = 6;
    constexpr auto end_scene = 42;
    constexpr auto reset = 16;
    constexpr auto present = 17;
    constexpr auto paint_traverse = 41;
    constexpr auto create_move = 22;
    constexpr auto play_sound = 82;
    constexpr auto frame_stage_notify = 37;
    constexpr auto draw_model_execute = 21;
    constexpr auto do_post_screen_space_effects = 44;
    constexpr auto sv_cheats_get_bool = 13;
    constexpr auto override_view = 18;
    constexpr auto lock_cursor = 67;
    constexpr auto scene_end = 9;
    constexpr auto send_datagram = 46;
    constexpr auto fire_event = 9;
    constexpr auto temp_entities = 36;
    constexpr auto setup_bones = 13;
    constexpr auto do_extra_bones_processing = 195;
    constexpr auto update_client_side_animation = 221;
    constexpr auto is_hltv = 93;
    constexpr auto in_prediction = 14;
    constexpr auto get_abs_origin = 1;
    constexpr auto calc_view = 270;
    constexpr auto accumulate_layers = 204;
    constexpr auto get_eye_angles = 167;
    constexpr auto post_data_update = 7;
}

class c_event_handler
{
public:
    class listener final : public IGameEventListener2
    {
    public:
        void start ( );
        void stop ( );
        void FireGameEvent ( IGameEvent* event ) override;
        int GetEventDebugID ( ) override;
    };

    void start ( );
    void stop ( );
private:
    listener m_listener;
};

extern c_event_handler event_handler;

namespace hooks
{
    void initialize ( );
    void shutdown ( );

    extern vfunc_hook hlclient_hook;
    extern vfunc_hook direct3d_hook;
    extern vfunc_hook vguipanel_hook;
    extern vfunc_hook vguisurf_hook;
    extern vfunc_hook sound_hook;
    extern vfunc_hook mdlrender_hook;
    extern vfunc_hook clientmode_hook;
    extern vfunc_hook sv_cheats;
    extern vfunc_hook render_view_hook;
    extern vfunc_hook gameevents_hook;
    extern vfunc_hook clientstate_hook;
    extern vfunc_hook entity_renderable_hook;
    extern vfunc_hook clientstate2_hook;
    extern vfunc_hook engine_client_hook;
    extern vfunc_hook prediction;
    extern vfunc_hook player_vtable_hook;

    using end_scene = long ( __stdcall*) ( IDirect3DDevice9* );
    using reset = long ( __stdcall*) ( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
    using present = long ( __stdcall*) ( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* );
    using create_move = void ( __thiscall*) ( IBaseClientDLL*, int, float, bool );
    using paint_traverse = void ( __thiscall*) ( IPanel*, vgui::VPANEL, bool, bool );
    using emit_sound1 = void ( __thiscall*) ( void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int,
                                              const Vector*, const Vector*, void*, bool, float, int, int );

    using frame_stage_notify = void ( __thiscall*) ( IBaseClientDLL*, ClientFrameStage_t );
    using play_sound = void ( __thiscall*) ( ISurface*, const char* name );
    using lock_cursor_t = void ( __thiscall*) ( ISurface* );
    using draw_model_execute = void ( __thiscall*) ( IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t* );
    using fire_event = bool ( __thiscall*) ( IGameEventManager2*, IGameEvent* p_event );
    using do_post_screen_effects = int ( __thiscall*) ( IClientMode*, int );
    using override_view = void ( __thiscall*) ( IClientMode*, CViewSetup* );
    using scene_end = void ( __fastcall*) ( void*, void* );
    using temp_entities_t = bool ( __thiscall*) ( void*, void* );
    using send_datagram = int ( __thiscall*) ( INetChannel*, bf_write* );
    using setup_bones_t = bool ( __thiscall*) ( void*, matrix3x4_t*, int, int, float );
    using calc_view_t = int ( __thiscall*) ( void* ecx, double st0, int Angle, int a3, int a4, int a5, float* a6 );
    using do_extra_bones_processing_t = void ( __thiscall*) ( void*, CStudioHdr*, Vector*, quaternion_t*, matrix3x4_t*, void*, void* );
    using update_client_side_animation_t = void ( __thiscall*) ( void* ); //<void ( __thiscall* ) ( void* )> ( this, 219 ) ( this );
    using is_hltv_t = bool ( __fastcall*) ( );
    using in_prediction_t = bool ( __thiscall*) ( void* ); //typedef bool(__thiscall* oInPrediction)(void*);
    using get_abs_origin_t = Vector ( __thiscall*) ( void* ); // virtual const Vector& get_abs_origin( void ) const = 0;
    using accumulate_layers_t = void ( __thiscall*) ( C_BasePlayer* player, void** bone_setup, Vector* pos, float time, quaternion_t* q );
    using get_eye_angles_t = qangle_t* ( __fastcall*) ( uintptr_t ecx, uintptr_t edx );
    //using post_data_update_t = void ( __fastcall* ) ( void*, void*, int );
    using post_data_update_t = void ( __thiscall*) ( void* thisptr );
    using setup_velocity_t = void ( __fastcall*) ( );

    extern temp_entities_t o_temp_entities;

    long __stdcall hk_end_scene ( IDirect3DDevice9* device );
    long __stdcall hk_present ( IDirect3DDevice9* m_device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override,
                                const RGNDATA* dirty_region );
    long __stdcall hk_reset ( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* p_presentation_parameters );
    void __stdcall hk_create_move ( int sequence_number, float input_sample_frametime, bool active, bool& send_packet );
    void __stdcall hk_create_move_proxy ( int sequence_number, float input_sample_frametime, bool active );
    void __stdcall hk_paint_traverse ( vgui::VPANEL panel, bool force_repaint, bool allow_force );
    void __stdcall hk_emit_sound1 ( IRecipientFilter& filter, int ent_index, int channel, const char* p_sound_entry, unsigned int n_sound_entry_hash,
                                    const char* p_sample, float fl_volume, int seed, float fl_attenuation, int flags, int pitch, const Vector* origin,
                                    const Vector* direction, void* p_utl_vec_origins, bool b_update_positions, float soundtime, int speakerentity, int unk );
    //void __stdcall hkPlaySound(const char* name);
    void __stdcall hk_draw_model_execute ( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo,
                                           matrix3x4_t* cpustom_bone_to_world );
    void __stdcall hk_frame_stage_notify ( ClientFrameStage_t stage );
    void __stdcall hk_override_view ( CViewSetup* vs_view );
    void __stdcall hk_lock_cursor ( );
    int __stdcall hk_do_post_screen_effects ( int a1 );
    bool __fastcall hk_sv_cheats_get_bool ( PVOID con_var, void* edx );
    //float __fastcall hk_cam_ideallag_get_float ( void* con_var, void* edx );
    void __fastcall hk_scene_end ( void* p_ecx, void* p_edx );
    bool __stdcall hk_fire_event ( IGameEvent* p_event );
    bool __fastcall hk_temp_entities ( void* ecx, void* edx, void* msg );
    bool __fastcall hk_setup_bones ( void* ecx, void* edx, matrix3x4_t* bone_to_world_out, int n_max_bones, int bone_mask, float current_time );
    LRESULT __stdcall hk_wnd_proc ( HWND h_wnd, UINT u_wsg, WPARAM w_param, LPARAM l_param );
    bool __fastcall hk_is_hltv ( );
    bool __fastcall hk_in_prediction ( void* );

    void __fastcall hk_setup_velocity ( );

    void __fastcall hk_post_data_update ( void* thisptr, void*, int update_type );

    Vector __fastcall hk_get_abs_origin ( void* ecx );

    //bool __fastcall hk_setup_bones ( void* ECX, void* EDX, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime );

    void fire_event_handler ( IGameEvent* event );
    void setup_fire_events ( );

    /*
    typedef int ( __thiscall* calc_view_t ) ( DWORD*, int a3, int a4, int a5, float a6 );
    extern calc_view_t o_calc_view;
    int __fastcall hk_calc_view ( DWORD* an, int a2, int a3, int a4, int a5, float a6 );
    */

    qangle_t* __fastcall hk_get_eye_angles ( uintptr_t ecx, uintptr_t edx );

    void __fastcall hk_accumulate_layers ( C_BasePlayer* player, uintptr_t edx, void** bone_setup, Vector* pos, float time, quaternion_t* q );

    int __fastcall hk_calc_view ( void* ecx, double st0, int Angle, int a3, int a4, int a5, float* a6 );

    void __fastcall hk_do_extra_bones_processing ( void* ecx, void* edx, CStudioHdr* hdr, Vector* pos, quaternion_t* q, matrix3x4_t* matrix, void* bone_list,
                                                   void* context );
    void __fastcall hk_update_client_side_animation ( void* ecx );

    inline setup_velocity_t o_setup_velocity;
    extern setup_bones_t o_setup_bones;
    //extern get_eye_angles_t o_get_eye_angles;
    //extern post_data_update_t o_post_data_update;
    extern HWND csgo_window;
    extern WNDPROC original_wnd_proc;
}
