#pragma once
#include <vector>
#include <algorithm>
#include "stdafx.h"
#include "resource.h"
#include "SDK/SDK Headers/CGameEvent.h"

namespace CPlayerList
{
	extern std::vector<int> friends;
	extern std::vector<int> Players;
	//extern std::vector<int> FixPitch;
}

void __fastcall Hooked_SceneEnd(void* ecx, void* edx);

bool __fastcall Hooked_GetBool(void* pConVar, void* edx);

bool __fastcall Hooked_CreateMove(void* thisptr, void*, float flInputSampleTime, CInput::CUserCmd* cmd);

void __stdcall Hooked_PaintTraverse( unsigned int vguiPanel, bool forceRepaint, bool allowForce );

void __stdcall Hooked_Frame_Stage_Notify( ClientFrameStage_t curStage );

void __stdcall Hooked_OverrideView( CViewSetup* pSetup );

LRESULT __stdcall Hooked_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

void __fastcall Hooked_DrawModelExecute( void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4* pCustomBoneToWorld );

void __stdcall Hooked_ClientCmdUnrestricted( const char* szCmdString, char flag );

void __stdcall Hooked_PlaySound( const char* pSample );

void __fastcall Hooked_FindMDL(void* thisptr, void* edx, char *path);

void __stdcall hk_lockcursor();

long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);

long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

typedef void(__thiscall *iPaintTraverse)(void*, unsigned int, bool, bool);

typedef bool(__thiscall *iCreateMoveFn) (void*, float, CInput::CUserCmd*);

typedef void(__stdcall *iFrameStageNotifyFn)(ClientFrameStage_t);

typedef void(__stdcall *iOverRideViewFn)(CViewSetup*);

typedef void(__thiscall* IDrawModelExecute)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);

typedef void(__stdcall* ClientCmdUnresticted)(const char* szCmdString, char flag);

typedef bool(__cdecl* ServerRankRevealAllFn)(float*);

typedef void(__stdcall* PlaySoundFn)(const char* pSample);

typedef void(__stdcall* IsReadyFn)();

typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);

typedef long(__stdcall *EndSceneResetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

typedef void(__thiscall* SceneEndFn)(void*);

typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);

typedef void(__thiscall *FindMdlFn)(void*, char*);
using LockCursor_t = void(__thiscall*)(ISurface*);

class trace_info
{
public:
	trace_info(Vector starts, Vector positions, float times, int userids)
	{
		this->start = starts;
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	Vector position;
	Vector start;
	float time;
	int userid;
};

extern std::vector<trace_info> trace_logs;

void Init();

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern vfunc_hook hlclient_hook;
extern vfunc_hook direct3d_hook;
extern vfunc_hook vguipanel_hook;
extern vfunc_hook vguisurf_hook;
extern vfunc_hook mdlrender_hook;
extern vfunc_hook clientmode_hook;
extern vfunc_hook renderview_hook;
extern vfunc_hook svcheats_hook;

//extern VTHookManager VMTClientMode;
//extern VTHookManager VMTGameEvent;
//extern VTHookManager VMTFindMDL;
//extern VTHookManager VMTDirectx;

class CHackManager
{
private:

public:
	std::vector<IGameEventListener2*> listeners = {};

	/* Original Functions */
	/*iCreateMoveFn oCreateMove;
	iPaintTraverse oPaintTraverse;
	iFrameStageNotifyFn oFrameStageNotify;
	IDrawModelExecute oDrawModelExecute;
	iOverRideViewFn oOverrideView;
	EndSceneResetFn oEndSceneReset;
	EndSceneFn oEndScene;
	ClientCmdUnresticted oClientCmdUnresticted;
	PlaySoundFn oPlaySound;
	ServerRankRevealAllFn ServerRankRevealEx;
	IsReadyFn IsReadyEx;
	LockCursor_t oLockCursor;
	FindMdlFn oFindMDL;*/
	//iOverrideMouseInput oOverrideMouseInput;
	bool PressedKeys[256] = {};
	/* Holding Varialbles */
	CInput::CUserCmd* CurrentCmd;
	CBaseEntity* LocalPlayer;
	CBaseCombatWeapon* LocalWeapon;
	Vector LastAngles;
	Vector realAng;
	Vector OrigAng;
	Vector vecUnpredictedVel;
	bool SendPacket = true;
	float FOV;
	int chokedticks, timer;
	float next_lby_update;
	float hitmarkerAlpha;
	DWORD Font_MenuTitle;
	DWORD Font_HUD;
	DWORD Font_HUD_Big;
	DWORD Font_Watermark;
	DWORD Font_Tab;
	DWORD Font_Logo;
	DWORD Font_ESP;
	DWORD Font_Controls;
	DWORD Font_Icons;
	DWORD Font_Weapons;
	DWORD Font_CSGO;
	DWORD Font_Meme;
	DWORD Font_LBY;
	DWORD Font_Watermarkplus;
	DWORD Font_Spectators;
	

	/* Hooking */
	void Hook()
	{
		Font_MenuTitle		= Interfaces.pSurface->Create_Font();
		Font_Watermark		= Interfaces.pSurface->Create_Font();
		Font_Tab			= Interfaces.pSurface->Create_Font();
		Font_ESP			= Interfaces.pSurface->Create_Font();
		Font_Logo			= Interfaces.pSurface->Create_Font();
		Font_Controls		= Interfaces.pSurface->Create_Font();
		Font_Icons			= Interfaces.pSurface->Create_Font();
		Font_Weapons		= Interfaces.pSurface->Create_Font();
		Font_HUD			= Interfaces.pSurface->Create_Font();
		Font_HUD_Big		= Interfaces.pSurface->Create_Font();
		Font_Meme			= Interfaces.pSurface->Create_Font();
		Font_LBY			= Interfaces.pSurface->Create_Font();
		Font_Watermarkplus	= Interfaces.pSurface->Create_Font();
		Font_Spectators = Interfaces.pSurface->Create_Font();
		Interfaces.pSurface->SetFontGlyphSet(Font_Spectators, "Courier New", 12, 750, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet( Font_Controls, "Tahoma", 12, 100, 0, 0, FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet( Font_Tab, "Impact", 22, 0, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE );
		Interfaces.pSurface->SetFontGlyphSet( Font_MenuTitle, "Impact", 25, 600, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE );
		Interfaces.pSurface->SetFontGlyphSet( Font_Watermark, "Verdana", 12, 100, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		Interfaces.pSurface->SetFontGlyphSet(Font_Watermarkplus, "Tahoma", 45, 800, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet( Font_Logo, "Impact", 30, 600, 0, 0, FONTFLAG_ANTIALIAS );
		Interfaces.pSurface->SetFontGlyphSet( Font_ESP, "Verdana", 13, 300, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		Interfaces.pSurface->SetFontGlyphSet( Font_HUD, "Tahoma", 16, 600, 0, 0, FONTFLAG_ANTIALIAS );
		Interfaces.pSurface->SetFontGlyphSet( Font_LBY, "Verdana", 28, 650, 0, 0, FONTFLAG_DROPSHADOW);
		Interfaces.pSurface->SetFontGlyphSet( Font_HUD_Big, "Tahoma", 36, 700, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE );
		Interfaces.pSurface->SetFontGlyphSet(Font_Meme, "Comic Sans MS", NULL, NULL, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
		Interfaces.pSurface->SetFontGlyphSet(Font_Meme, "Baar Zeitgeist", 50, NULL, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);

		HRSRC hResInst = FindResource( nullptr, MAKEINTRESOURCE( IDR_FONT1 ), RT_FONT );
		HGLOBAL hGlobal = LoadResource( nullptr, hResInst );
		size_t __LEN = SizeofResource( nullptr, hResInst );
		DWORD dwFonts;
		Interfaces.__FNTHANDLE = AddFontMemResourceEx( nullptr, __LEN, nullptr, &dwFonts );
		auto __FONT = CreateFont( 16, 0, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "undefeated" );

		Interfaces.pSurface->SetFontGlyphSet( Font_Icons, "undefeated", 32, 0, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE );
		Interfaces.pSurface->SetFontGlyphSet( Font_Weapons, "undefeated", 12, 0, 0, 0, FONTFLAG_ANTIALIAS );

		if (INIT::Window)
			INIT::OldWindow = (WNDPROC)SetWindowLongPtr(INIT::Window, GWL_WNDPROC, (LONG_PTR)Hooked_WndProc);

		static IDirect3DDevice9* DeviceStructureAddress;
		if (!DeviceStructureAddress)
			DeviceStructureAddress = **(IDirect3DDevice9***)(Utils.FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

		hlclient_hook.setup(Interfaces.pClient, "client_panorama.dll");
		direct3d_hook.setup(DeviceStructureAddress, "shaderapidx9.dll");
		vguipanel_hook.setup(Interfaces.pPanel);
		vguisurf_hook.setup(Interfaces.pSurface);
		mdlrender_hook.setup(Interfaces.g_pModelRender, "engine.dll");
		clientmode_hook.setup(Interfaces.pClientMode, "client_panorama.dll");
		renderview_hook.setup(Interfaces.g_pRenderView);
		auto sv_cheats = Interfaces.g_ICVars->FindVar("sv_cheats");
		if (sv_cheats)
		{
			svcheats_hook.setup(sv_cheats);
			svcheats_hook.hook_index(13, Hooked_GetBool);
		}

		direct3d_hook.hook_index(42, Hooked_EndScene);
		direct3d_hook.hook_index(16, Hooked_EndScene_Reset);

		hlclient_hook.hook_index(37, Hooked_Frame_Stage_Notify);

		vguipanel_hook.hook_index(41, Hooked_PaintTraverse);
		vguisurf_hook.hook_index(67, hk_lockcursor);

		mdlrender_hook.hook_index(21, Hooked_DrawModelExecute);

		clientmode_hook.hook_index(24, Hooked_CreateMove);
		clientmode_hook.hook_index(18, Hooked_OverrideView);

		renderview_hook.hook_index(9, Hooked_SceneEnd);

		// Hooks

		/*if (Interfaces.pClientMode)
		{
			VMTClientMode.bInitialize(reinterpret_cast<DWORD**>(Interfaces.pClientMode));
			oCreateMove = reinterpret_cast<iCreateMoveFn>(VMTClientMode.dwHookMethod(reinterpret_cast<DWORD>(&Hooked_CreateMove), 24));
			oOverrideView = reinterpret_cast<iOverRideViewFn>(VMTClientMode.dwHookMethod(reinterpret_cast<DWORD>(&Hooked_OverrideView), 18));
		}

		if( Interfaces.pClient )
		{
			VMTClient.bInitialize( reinterpret_cast< DWORD** >( Interfaces.pClient ) );
			
			oFrameStageNotify = reinterpret_cast< iFrameStageNotifyFn >( VMTClient.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_Frame_Stage_Notify ), 37 ) );
		}

		if( Interfaces.pEngine )
		{
			VMTEngine.bInitialize( reinterpret_cast< DWORD** >( Interfaces.pEngine ) );
			oClientCmdUnresticted = reinterpret_cast< ClientCmdUnresticted >( VMTEngine.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_ClientCmdUnrestricted ), 114 ) );
		}

		if( Interfaces.pSurface )
		{
			VMTPanel.bInitialize( reinterpret_cast< unsigned long** >( Interfaces.pPanel ) );
			oPaintTraverse = reinterpret_cast< iPaintTraverse >( VMTPanel.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_PaintTraverse ), 41 ) );
			cout << ired << "Paint Traverse Hooked (41)." << iyellow << " oPaintTraverse = 0x" << oPaintTraverse << white << endl;
		}

		if( Interfaces.g_pModelRender )
		{
			VMTModelRender.bInitialize( reinterpret_cast< DWORD** >( Interfaces.g_pModelRender ) );
			oDrawModelExecute = reinterpret_cast< IDrawModelExecute >( VMTModelRender.dwHookMethod( reinterpret_cast< DWORD >( &Hooked_DrawModelExecute ), 21 ) );
			cout << ired << "Draw Model Execute Hooked (21)." << iyellow << " oDrawModelExecute = 0x" << oDrawModelExecute << white << endl;
		}

		if( Interfaces.pSurface )
		{
			VMTSurface.bInitialize(reinterpret_cast< DWORD** >(Interfaces.pSurface));
			oLockCursor = reinterpret_cast< LockCursor_t >(VMTSurface.dwHookMethod(reinterpret_cast< DWORD >(&hk_lockcursor), 67));
		}

		//if (Interfaces.g_pMDLCache)
		//{
		//	VMTFindMDL.bInitialize((DWORD**)Interfaces.g_pMDLCache);
		//	oFindMDL = (FindMdlFn)VMTFindMDL.dwHookMethod((DWORD)&Hooked_FindMDL, 10);
		//}
		*/
		//std::cout << DeviceStructureAddress << endl;

		/*if (DeviceStructureAddress) {
			VMTDirectx.bInitialize((DWORD**)DeviceStructureAddress);
			oEndSceneReset = (EndSceneResetFn)VMTDirectx.dwHookMethod((DWORD)Hooked_EndScene_Reset, 16);
			oEndScene = (EndSceneFn)VMTDirectx.dwHookMethod((DWORD)&Hooked_EndScene, 42);
		}*/
		Init();
	}

	void UnHook()
	{
		hlclient_hook.unhook_all();
		direct3d_hook.unhook_all();
		vguipanel_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		clientmode_hook.unhook_all();
		renderview_hook.unhook_all();
		for (auto listener : listeners)
			Interfaces.g_GameEventMgr->RemoveListener(listener);
	}
};
