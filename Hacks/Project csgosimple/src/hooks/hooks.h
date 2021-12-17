#pragma once

#include "../valve_sdk/csgostructs.hpp"
#include "../deps/blackbone/LocalHook/LocalHook.hpp"
#include "..//IStudioRender.h"

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

namespace hooks
{
	void initialize();
	void destroy();

	struct d3d9
	{
		static vfunc_hook hook;

		struct end_scene
		{
			static const int index = 42;
			using fn = long(__stdcall*)(IDirect3DDevice9*);
			static long __stdcall hooked(IDirect3DDevice9*);
		};

		struct reset
		{
			static const int index = 16;
			using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
			static long __stdcall hooked(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		};

		struct draw_indexed_primitive
		{
			static const int index = 82;
			using fn = long(__thiscall*)(void* thisptr, LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
			static long __stdcall hooked(LPDIRECT3DDEVICE9 device, D3DPRIMITIVETYPE type, INT base_vertex_index, UINT min_index, UINT num_vertices, UINT start_index, UINT prim_count);
		};
	};

	struct vgui_panel
	{
		static vfunc_hook hook;

		struct paint_traverse
		{
			static const int index = 41;
			using fn = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
			static void __stdcall hooked(vgui::VPANEL, bool forceRepaint, bool allowForce);
		};
	};

	struct events
	{
		static vfunc_hook hook;

		struct fire_event
		{
			static const int index = 9;
			using fn = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
			static bool __stdcall hooked(IGameEvent* pEvent);
		};
	};

	struct mdlrender
	{
		static vfunc_hook hook;

		struct draw_model_execute
		{
			static const int index = 29; //was 21
			using DrawModelExecute = void(__thiscall*)(void*, void*, DrawModelInfo_t*, matrix3x4_t*, float*, float*, Vector&, int);
			static void __fastcall hooked(void* pEcx, void* pEdx, void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights, float* flpFlexDelayedWeights, Vector& vrModelOrigin, int32_t iFlags);
		};
	};

	struct renderview
	{
		static vfunc_hook hook;

		struct scene_end
		{
			static const int index = 9;
			using fn = void(__thiscall*)(IVRenderView*);
			static void __fastcall hooked(IVRenderView*&);
		};
	};
	
	struct client_mode
	{
		static vfunc_hook hook;

		struct create_move_shared
		{
			static const int index = 24;
			using fn = bool(__thiscall*)(IClientMode*, float, CUserCmd*);
			static bool __stdcall hooked(float input_sample_frametime, CUserCmd* cmd);
		};

		struct override_view
		{
			static const int index = 18;
			using fn = void(__thiscall*)(IClientMode*, CViewSetup*);
			static void __stdcall hooked(CViewSetup*);
		};

		struct post_screen_effects
		{
			static const int index = 44;
			using fn = int(__thiscall*)(void*, int);
			static int __stdcall hooked(int value);
		};
	};

	struct SL
	{
		static vfunc_hook hook;
			
		struct SuppressList
		{
			static const int index = 16;
			using fn = bool(__thiscall*)(void*, int, bool);
			static void __stdcall hooked(int a2, bool a3);
		};
	};

	struct post_data_update
	{
		static const int index = 7;
		using fn = void(__thiscall*)(IClientNetworkable* thisptr, int update_type);
		static void __stdcall hooked(int update_type);
		static vfunc_hook hook;
		static void setup();
	};

	struct find_mdl
	{
		static const int index = 10;
		using fn = MDLHandle_t(__thiscall*)(IMDLCache*, const char* relative_path);
		//static MDLHandle_t __stdcall hooked(IMDLCache*&, const char*& relative_path);
		static MDLHandle_t __fastcall hooked(IMDLCache*&, char* FilePath);
		static blackbone::Detour<fn> hook;
	};

	struct scene_end
	{
		static const int index = 9;
		using fn = void(__thiscall*)(IVRenderView*);
		static void __fastcall hooked(IVRenderView*&);
		static blackbone::Detour<fn> hook;
	};
	
	struct render_view
	{
		static const int index = 6;
		using fn = void(__thiscall*)(IViewRender*, CViewSetup& view, CViewSetup& hudview, int nClearFlags, int whatToDraw);
		static void __stdcall hooked(IViewRender*&, CViewSetup& view, CViewSetup& hudview, int& nClearFlags, int& whatToDraw);
		static blackbone::Detour<fn> hook;
	};

	struct retrieve_message
	{
		static const int index = 2;
		using fn = EGCResults(__thiscall*)(void*, uint32_t*, void*, uint32_t, uint32_t*);
		static EGCResults __stdcall hooked(uint32_t* punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize);
		static vfunc_hook hook;
	};

	struct dispatch_user_message
	{
			static const int index = 38;
			using fn = bool(__thiscall*)(void*, int type, int a3, int length, void* msg_data);
			static bool __stdcall hooked(void*&, int& type, int& a3, int& length, void*& msg_data);
			static blackbone::Detour<fn> hook;
	};

	struct netchannel
	{
		static vfunc_hook hook;
		static void setup();
	};

	struct send_net_message
	{
		static const int index = 40;
		using fn = bool(__thiscall*)(void* thisptr, INetMessage& msg, bool bForceReliable, bool bVoice);
		static bool __stdcall hooked(INetMessage& msg, bool bForceReliable, bool bVoice);
	};

	struct send_datagram
	{
		static const int index = 46;
		using fn = int(__thiscall*)(INetChannel* netchan, bf_write* datagram);
		static int __stdcall hooked(bf_write* datagram);
	};

	struct sequence
	{
		static recv_prop_hook* hook;
		static void hooked(const CRecvProxyData* data, void* entity, void* output);
	};

	struct fire_bullets
	{
		static const int index = 7;
		using fn = void(__thiscall*)(void*, int);
		static void __stdcall hooked(int);
		static vfunc_hook hook;
	};
};