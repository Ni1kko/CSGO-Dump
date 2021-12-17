#include "hooks.h"
#include "TraceRay.hpp"
#include "IsEntBreakableHook.hpp"
#include "updateanimations.h"
#include "shouldskipanimframe.h"

CHooks g_Hooks;

ViewModelFn_t ViewModelOriginal;
WNDPROC OldWindowProc;
HWND hwndWindow = nullptr;

void CHooks::InitHooks()
{
	hwndWindow = FindWindow( "Valve001", NULL );
	OldWindowProc = ( WNDPROC ) SetWindowLongPtr( hwndWindow, GWLP_WNDPROC, ( LONG_PTR ) HOOKED_WindowProc );

	void *ClientMode = **( void*** ) ( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "8B 35 ? ? ? ? 8B 80" ) + 2 );
	//printf( "0x%X", ( *( DWORD** )g_pPred )[ 19 ] - dwClient ); func offset

	g_pRunCommandHOOKED = new CVMTHookManager( ( PDWORD* ) g_pPred );
//	oInPrediction = ( InPrediction_t ) g_pRunCommandHOOKED->dwHookMethod( ( DWORD ) HOOKED_InPrediction, 14 );
	g_pRunCommandHOOKED->dwHookMethod( ( DWORD ) HOOKED_RunCommand, 19 );

	g_pEngineHOOKED = new CVMTHookManager( ( PDWORD* ) g_pEngine );
	oIsBoxVisible = ( IsBoxVisibleFn ) g_pEngineHOOKED->dwHookMethod( reinterpret_cast< uintptr_t >( HOOKED_IsBoxVisible ), 32 );
	oFireEvent = ( FireEventFn ) g_pEngineHOOKED->dwHookMethod( reinterpret_cast< uintptr_t >( HOOKED_FireEvent ), 59 );
	g_pEngineHOOKED->dwHookMethod((DWORD)HOOKED_IsHLTV, 93);

	g_pClientHOOKED = new CVMTHookManager( ( PDWORD* ) g_pClient );
//	oWriteUserCmd = ( WriteUserCmdFn ) g_pClientHOOKED->dwHookMethod( ( DWORD ) HOOKED_WriteUserCmdDeltaToBuffer, 24 );
	g_pClientHOOKED->dwHookMethod( ( DWORD ) HOOKED_FrameStageNotify, 37 );

	g_pPaintTraverseHOOKED = new CVMTHookManager( ( PDWORD* ) g_pPanel );
	g_pPaintTraverseHOOKED->dwHookMethod( ( DWORD ) HOOKED_PaintTraverse, 41 );

	g_pSurfaceHOOKED = new CVMTHookManager( ( PDWORD* ) g_pSurface );
	g_pSurfaceHOOKED->dwHookMethod( ( DWORD ) HOOKED_LockCursor, 67 );

//	g_pBSPTreeHOOKED = new VMTHook(reinterpret_cast<DWORD**>( g_pEngine->GetBSPTreeQuery()) );
//	oListLeavesInBox = ( ListLeavesInBoxFn )g_pBSPTreeHOOKED->HookFunction(reinterpret_cast<uintptr_t>( HOOKED_ListLeavesInBox ), 6 );

	g_pDrawModelHOOKED = new CVMTHookManager((PDWORD*)g_pEngineModel);
	oDrawModel = (DrawModelExecuteFn)g_pDrawModelHOOKED->dwHookMethod((DWORD)HOOKED_DrawModel, 21);

	g_pRenderViewHOOKED = new CVMTHookManager((PDWORD*)g_pRenderView);
	g_pRenderViewHOOKED->dwHookMethod((DWORD)HOOKED_SceneEnd, 9);

	g_pMaterialSystemHOOKED = new CVMTHookManager( ( PDWORD* ) g_pMatsystem );
	oOverrideConfig = ( OverrideConfigFn ) g_pMaterialSystemHOOKED->dwHookMethod( ( DWORD ) HOOKED_OverrideConfig, 21 );

	g_pStudioRenderHOOKED = new VMTHook( reinterpret_cast< DWORD** > ( g_pStudioRender ) );
	oBeginFrame = ( BeginFrameFn ) g_pStudioRenderHOOKED->HookFunction( reinterpret_cast< uintptr_t >( HOOKED_BeginFrame ), 9 );

	g_pClientModeHOOKED = new CVMTHookManager( ( PDWORD* ) ClientMode );
	oOverrideView = ( OverrideViewFn_t ) g_pClientModeHOOKED->dwHookMethod( ( DWORD ) HOOKED_OverrideView, 18 );
	g_pClientModeHOOKED->dwHookMethod( ( DWORD ) HOOKED_CreateMove, 24 );
	ViewModelOriginal = ( ViewModelFn_t ) g_pClientModeHOOKED->dwHookMethod( ( DWORD ) ViewmodelHook, 35 );

	//g_pEngineTraceHOOKED = new CVMTHookManager( ( DWORD** ) g_pEngineTrace );
	//oTraceRay = ( TraceRayFn ) g_pEngineTraceHOOKED->dwHookMethod( ( DWORD ) hkTraceRay, 5 );

	/*
	convar cl_csm_shadows
	hook get int (index 13)
	return 0
	will give us HUGE fps boost ty volvo
	*/

	static bool bHooked = false;

	if( !bHooked )
	{
		bHooked = true;

		DWORD r_drawmodelstatsoverlay = ( DWORD ) g_pICvar->FindVar( "r_drawmodelstatsoverlay" );

		if( !r_drawmodelstatsoverlay )
			return;

		HookConvar( ( void* ) r_drawmodelstatsoverlay, &HOOKED_DrawModelStats, 13, dwModelStatsVMT );

		DWORD sv_cheats = ( DWORD ) g_pICvar->FindVar( "sv_cheats" );

		if( !sv_cheats )
			return;

		HookConvar( ( void* ) sv_cheats, &HOOKED_SvCheats, 13, dwSvCheatsVMT );

		/*	DWORD cl_csm_shadows = ( DWORD ) g_pICvar->FindVar( "cl_csm_shadows" );

		if( !cl_csm_shadows )
		return;

		HookConvar( ( void* ) cl_csm_shadows, &HOOKED_CSMShadow, 13, dwCSMShadowVMT );*/
	}

	//IsEntBreakableHook::SetupHook(); // I dont think we need this one anymore
	DoExtraBoneProcessingHook::SetupHook();
	CalcViewModelView::SetupHook();
//	AdjustPlayerTickHook::SetupHook();
//	HookAddRenderAble();
//	SetupBonesHook::SetupHook();
//	UpdateAnimsHook::SetupHook();
//	PacketStart::SetupHook();
//	ShouldSkipAnimFrameHook::SetupHook();
}