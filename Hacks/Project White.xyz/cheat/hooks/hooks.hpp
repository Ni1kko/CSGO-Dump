#pragma once

#include "../../sdk/structs.hpp"
#include <d3d9.h>

namespace index {
    constexpr auto EmitSound1				= 5;
    constexpr auto EndScene					= 42;
    constexpr auto Reset					= 16;
    constexpr auto PaintTraverse			= 41;
    constexpr auto CreateMove				= 24;
    constexpr auto PlaySound				= 82;
    constexpr auto FrameStageNotify			= 37;
    constexpr auto DrawModelExecute			= 21;
    constexpr auto DoPostScreenSpaceEffects = 44;
    constexpr auto SvCheatsGetBool			= 13;
    constexpr auto OverrideView				= 18;
    constexpr auto FireEvent				= 9;
    constexpr auto LockCursor				= 67;
    constexpr auto ClientModeCreateMove		= 24;
    constexpr auto ListLeavesInBox			= 6;
    constexpr auto SuppressLists			= 16;
    constexpr auto ReadPacketEntities		= 64;
	constexpr auto AspectRatio				= 101;
}

namespace Hooks {
	inline unsigned int get_virtual(void* _class, unsigned int index) {
		return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); 
	}

    void Initialize();
    void Shutdown();

	inline recv_prop_hook* m_pFlashDuration;
	inline recv_prop_hook* sequence_hook;

	namespace end_scene {
		using fn_endscene = long(__stdcall*)(IDirect3DDevice9*);
		long __stdcall hook(IDirect3DDevice9* device);
		inline fn_endscene o_end_scene = nullptr;
	}
	
	namespace reset {
		using fn_reset = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters);
		inline fn_reset o_reset = nullptr;
	}
	
	namespace creatmove {
		using fn_crmove = bool(__stdcall*)(float, CUserCmd*);
		bool __stdcall hook(float input_sample_frametime, CUserCmd* cmd);
		inline fn_crmove o_crmove = nullptr;
	}
	
	namespace pant_traverse {
		using fn_paint = void(__fastcall*)(void*, int, vgui::VPANEL, bool, bool);
		void __fastcall hook(void* _this, int edx, vgui::VPANEL panel, bool force_repaint, bool allow_force);
		inline fn_paint o_paint = nullptr;
	}
	
	namespace fsn {
		using fn_fsn = void(__fastcall*)(void*, int, ClientFrameStage_t);
		void __fastcall hook(void* _this, int, ClientFrameStage_t stage);
		inline fn_fsn o_fsn = nullptr;
	}
	
	namespace emit_sound {
		using fn_sound = long(__fastcall*)(void*, int, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);
		void __fastcall hook(void* _this, int edx, IRecipientFilter& filter, int ent_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const Vector* origin, const Vector* direction, void* utl_vec_origins, bool update_positions, float sound_time, int speaker_entity, int unk);
		inline fn_sound o_sound = nullptr;
	}

	namespace lock_cursour {
		using fn_lock = void(__fastcall*)(void*);
		void __fastcall hook(void* _this);
		inline fn_lock o_lock = nullptr;
	}

	namespace dme {
		using fn_dme = void(__fastcall*)(void*, int, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		void __fastcall hook(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* custom_bone_to_world);
		inline fn_dme o_dme = nullptr;
	}

	namespace dpse {
		using fn_effects = int(__fastcall*)(void*, int, int);
		int  __fastcall hook(void* _this, int edx, int a1);
		inline fn_effects o_effects = nullptr;
	}

	namespace override_view {
		using fn_ovview = void(__fastcall*)(void*, int, CViewSetup*);
		void __fastcall hook(void* _this, int, CViewSetup* vs_view);
		inline fn_ovview o_ovview = nullptr;
	}

	namespace fire_event {
		using fn_events = int(__fastcall*)(void*, void*, IGameEvent*);
		bool __fastcall hook(void* ecx, void* edx, IGameEvent* event);
		inline fn_events o_events = nullptr;
	}
	
	namespace cheats {
		using fn_cheats = bool(__fastcall*)(void*, void*);
		bool __fastcall hook(void* convar, void* edx);
		inline fn_cheats o_cheats = nullptr;
	}
	
	namespace aspect_ratio {
		using fn_aspect_ratio = void(__fastcall*)(void*, void*, int32_t, int32_t);
		float __fastcall hook(void* ecx, void* edx, int32_t width, int32_t height);
		inline fn_aspect_ratio o_aspect_ratio = nullptr;
	}

	namespace suppress_list {
		using fn_supress_list = bool(__thiscall*)(void*, int, bool);
		void  __stdcall hook(int a2, bool a3);
		inline fn_supress_list o_suppress_list = nullptr;
	}

	namespace rpe {
		using fn_rpe = void(__thiscall*)(void*, void*);
		void __fastcall hook(void* client_state, void* edx, void* ent_info);
		inline fn_rpe o_rpe = nullptr;
	}
	
	void hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output);
    void hkFlashDuration(const CRecvProxyData* m_pData, void* m_pStruct, void* m_pResult);
}