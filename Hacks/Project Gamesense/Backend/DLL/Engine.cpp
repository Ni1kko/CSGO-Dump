#include "Engine.h"

#include "../Configuration/Settings.h"
#include "../Utility/Utilities.h"
#include "../Features/EventListener.h"
#include "../Interfaces/NetVar.h"
#include "../Hashing/xorstr.h"
#include "../Hooking/Functions/EndScene.hpp"
#include "../Hooking/Functions/listLeavesInBox.hpp"
#include "../Hooking/Functions/DoExtraBonesProcessing.hpp"
#include "../Hooking/Functions/Reset.hpp"
#include "../Hooking/Functions/WndProc.hpp"
#include "../Hooking/Functions/LockCursor.hpp"
#include "../Hooking/Functions/CreateMove.hpp"
#include "../Hooking/Functions/FrameStageNotify.hpp"
#include "../Hooking/Functions/PaintTreverse.hpp"
#include "../Hooking/Functions/DrawModelExecute.hpp"
#include "../Hooking/Functions/DoPostScreenSpaceEffects.hpp"
#include "../Hooking/Functions/SceneEnd.hpp"
#include "../Hooking/Functions/OverrideView.hpp"
#include "../Hooking/Functions/EmitSound.hpp"
#include "../Hooking/Functions/SendDataGram.hpp"
#include "../Hooking/Functions/Sequence.hpp"
#include "../Hooking/Functions/WriteuUsercmdDeltaToBuffer.hpp"
#include "../Hooking/Functions/ReadPacketEntities.hpp"
#include "../Hooking/Functions/FireEvent.hpp"
#include "../Hooking/Functions/StandardBlendingRules.hpp"
#include <detours.h>
//#include "../Hooking/Functions/StandartBlendingRules.h"

DWORD OriginalDoExtraBonesProcessing;
DWORD OriginalShouldSkipAnimFrame;

using namespace Cheat;
using namespace Cheat::Hooked;

void SetupOffsets()
{
	Cheat::Utilities->Console_Log( xorstr_("Initializing net vars") );
	Cheat::NetVarManager = new Cheat::CNetVarManager();
	Cheat::NetVarManager->init();
	offys.m_account = nv(xorstr_("CCSPlayer"), xorstr_("m_iAccount") );
	offys.m_angeyeangles = nv(xorstr_("CCSPlayer"), xorstr_("m_angEyeAngles[0]") );
	offys.m_flCycle = nv(xorstr_("CCSPlayer"), xorstr_("m_flCycle"));
	offys.m_nSequence = nv(xorstr_("CCSPlayer"), xorstr_("m_nSequence"));
	offys.m_bhashelmet = nv(xorstr_("CCSPlayer"), xorstr_("m_bHasHelmet") );
	offys.m_iPlayerC4 = nv(xorstr_("CCSPlayerResource"), xorstr_("m_iPlayerC4"));
	offys.m_flnextattack = nv(xorstr_("CBaseCombatCharacter"), xorstr_("m_flNextAttack") );
	offys.m_flnextprimaryattack = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_flNextPrimaryAttack") );
	offys.m_flpostponefirereadytime = nv(xorstr_("CWeaponCSBase"), xorstr_("m_flPostponeFireReadyTime") );
	offys.m_gungameimmunity = nv(xorstr_("CCSPlayer"), xorstr_("m_bGunGameImmunity") );
	offys.m_hactiveweapon = nv(xorstr_("CBaseCombatCharacter"), xorstr_("m_hActiveWeapon") );
	offys.m_weaponMode = nv(xorstr_("CWeaponCSBaseGun"), xorstr_("m_weaponMode"));
	offys.m_iarmor = nv(xorstr_("CCSPlayer"), xorstr_("m_ArmorValue") );
	offys.m_iclip1 = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_iClip1") );
	offys.m_ihealth = nv(xorstr_("CBasePlayer"), xorstr_("m_iHealth") );
	offys.m_iitemdefinitionindex = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_iItemDefinitionIndex") );
	offys.m_isscoped = nv(xorstr_("CCSPlayer"), xorstr_("m_bIsScoped") );
	offys.m_lifeState = nv(xorstr_("CBasePlayer"), xorstr_("m_lifeState") );
	offys.m_hitboxset = nv(xorstr_("CBasePlayer"), xorstr_("m_nHitboxSet") );
	offys.m_lowerbodyyawtarget = nv(xorstr_("CCSPlayer"), xorstr_("m_flLowerBodyYawTarget") );
	offys.m_observermode = nv(xorstr_("CBasePlayer"), xorstr_("m_iObserverMode") );
	offys.m_poseparam = nv(xorstr_("CBaseAnimating"), xorstr_("m_flPoseParameter") );
	offys.m_simtime = nv(xorstr_("CBaseEntity"), xorstr_("m_flSimulationTime"));
	offys.m_effects = nv(xorstr_("CBaseEntity"), xorstr_("m_fEffects"));
	offys.m_tickbase = nv(xorstr_("CBasePlayer"), xorstr_("m_nTickBase") );
	offys.m_vecorigin = nv(xorstr_("CBaseEntity"), xorstr_("m_vecOrigin") );
	offys.m_vecvelocity = nv( xorstr_("CBasePlayer"), xorstr_("m_vecVelocity[0]") );
	offys.m_ragdollpos = nv(xorstr_("CRagdollProp"), xorstr_("m_ragPos"));
	offys.m_vecviewoff = nv( xorstr_("CBasePlayer"), xorstr_("m_vecViewOffset[0]") );
	offys.m_howner = nv( xorstr_("CBaseCombatWeapon"), xorstr_("m_hOwner") );
	offys.m_iping = nv( xorstr_("CCSPlayerResource"), xorstr_("m_iPing") );
	offys.m_bclientsideanim = nv( xorstr_("CBaseAnimating"), xorstr_("m_bClientSideAnimation") );
	offys.m_sequence = nv(xorstr_("CBaseAnimating"), xorstr_("m_nSequence"));
	offys.m_hObserverTarget = nv( xorstr_("CCSPlayer"), xorstr_("m_hObserverTarget") );
	offys.m_viewPunchAngle = nv( xorstr_("CBasePlayer"), xorstr_("m_viewPunchAngle") );
	offys.m_flashalpha = nv( xorstr_("CCSPlayer"), xorstr_("m_flFlashDuration") );
	offys.m_spotted = nv(xorstr_("CCSPlayer"), xorstr_("m_bSpotted"));
	offys.m_fFlags = nv( xorstr_("CBasePlayer"), xorstr_("m_fFlags") );
	offys.m_aimpunchangle = nv( xorstr_("CBasePlayer"), xorstr_("m_aimPunchAngle") );
	offys.m_fduckamount = nv( xorstr_("CCSPlayer"), xorstr_("m_flDuckAmount") );
	offys.m_fduckspeed = nv( xorstr_("CCSPlayer"), xorstr_("m_flDuckSpeed") );
	offys.m_bBombTicking = nv( xorstr_("CPlantedC4"), xorstr_("m_bBombTicking") );
	offys.m_nBombSite = nv( xorstr_("CPlantedC4"), xorstr_("m_nBombSite") );
	offys.m_flC4Blow = nv( xorstr_("CPlantedC4"), xorstr_("m_flC4Blow") );
	offys.m_flTimerLength = nv( xorstr_("CPlantedC4"), xorstr_("m_flTimerLength") );
	offys.m_flDefuseLength = nv( xorstr_("CPlantedC4"), xorstr_("m_flDefuseLength") );
	offys.m_flDefuseCountDown = nv( xorstr_("CPlantedC4"), xorstr_("m_flDefuseCountDown") );
	offys.m_bBombDefused = nv( xorstr_("CPlantedC4"), xorstr_("m_bBombDefused") );
	offys.m_hBombDefuser = nv( xorstr_("CPlantedC4"), xorstr_("m_hBombDefuser") );
	offys.m_iShotsFired = nv( xorstr_("CCSPlayer"), xorstr_("m_iShotsFired") );
	offys.m_izoomLevel = nv( xorstr_("CWeaponAWP"), xorstr_("m_zoomLevel") );
	offys.m_bHasHeavyArmor = nv( xorstr_("CCSPlayer"), xorstr_("m_bHasHeavyArmor") );
	offys.m_bPinPulled = nv( xorstr_("CBaseCSGrenade"), xorstr_("m_bPinPulled") );
	offys.m_fThrowTime = nv( xorstr_("CBaseCSGrenade"), xorstr_("m_fThrowTime") );
	offys.dwReloading = Cheat::Utilities->Memory_PatternScan(Cheat::Settings->ClientDLL, xorstr_("C6 87 ? ? ? ? ? 8B 06 8B CE FF 90")) + 2;
	offys.m_flNextSecondaryAttack = nv( xorstr_("CBaseCombatWeapon"), xorstr_("m_flNextSecondaryAttack") );
	offys.dwPlayerResource = *( DWORD* )( Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") ) + 2 );
	offys.dwGetSequenceActivity = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 53 8B 5D 08 56 8B F1 83") );
	offys.dwSetAbsOrigin = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8") );
	offys.dwSetAbsAngles = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8") );
	offys.dwLoadFromBuffer = Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, ( xorstr_("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04") ) );
	offys.dwInitKeyValues = Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, ( xorstr_("8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03 C1 F8 08 66 89 46 12 8B C6") ) ) - 0x45;
	offys.dwSmokeCount = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("A3 ? ? ? ? 57 8B CB") ) + 1;
	offys.dwLoadSky = Utilities->Memory_PatternScan( xorstr_("engine.dll"), xorstr_("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45") );
	offys.dwCrosshair = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("83 F8 05 75 17") );
	offys.dwSetClanTag = Cheat::Utilities->Memory_PatternScan( xorstr_("engine.dll"), xorstr_("53 56 57 8B DA 8B F9 FF 15") );
	offys.dwServerRankRevealAll = Cheat::Utilities->Memory_PatternScan( Cheat::Settings->ClientDLL, xorstr_("55 8B EC 8B 0D ? ? ? ? 85 C9 75 28 A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 04 85 C0 74 0B 8B C8 E8 ? ? ? ? 8B C8 EB 02 33 C9 89 0D ? ? ? ? 8B 45 08") );

	/*offys.m_nModelIndex = nv(xorstr_("CBasePlayer"), xorstr_("m_nModelIndex"));
	offys.m_iViewModelIndex = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_iViewModelIndex"));
	offys.m_iWorldModelIndex = nv(xorstr_("CBaseCombatWeapon"), xorstr_("m_iWorldModelIndex"));
	offys.m_iEntityQuality = nv(xorstr_("CBaseAttributableItem"), xorstr_("m_iEntityQuality"));
	offys.m_OriginalOwnerXuidLow = nv(xorstr_("CBaseAttributableItem"), xorstr_("m_OriginalOwnerXuidLow"));
	offys.m_OriginalOwnerXuidHigh = nv(xorstr_("CBaseAttributableItem"), xorstr_("m_OriginalOwnerXuidHigh"));
	offys.m_flFallbackWear = nv(xorstr_("CBaseAttributableItem"), xorstr_("m_flFallbackWear"));
	offys.m_iItemIDHigh = nv(xorstr_("CBaseAttributableItem"), xorstr_("m_iItemIDHigh"));
	offys.m_nFallbackPaintKit = nv(xorstr_("CBaseAttributableItem"), xorstr_("m_nFallbackPaintKit"));*/

	Cheat::Utilities->Console_Log( xorstr_("Initialization finished") );
}

std::uintptr_t reset_pattern = *reinterpret_cast<std::uintptr_t*>(Utilities->Memory_PatternScan(xorstr_("gameoverlayrenderer.dll"), xorstr_("C7 45 ? ? ? ? ? FF 15 ? ? ? ? 8B D8")) + 9);
std::uintptr_t present_pattern = *reinterpret_cast<std::uintptr_t*>(Utilities->Memory_PatternScan(xorstr_("gameoverlayrenderer.dll"), xorstr_("FF 15 ? ? ? ? 8B F0 85 FF")) + 2);

void Cheat::wrapgameoverlay()
{
	oPresent = *reinterpret_cast<decltype(oPresent)*>(present_pattern);
	*reinterpret_cast<decltype(::present2)**>(present_pattern) = ::present2;

	oReset = *reinterpret_cast<decltype(oReset)*>(reset_pattern);
	*reinterpret_cast<decltype(::reset2)**>(reset_pattern) = ::reset2;
}

void Cheat::unwrapgameoverlay()
{
	*reinterpret_cast<decltype(oPresent)*>(present_pattern) = oPresent;
	*reinterpret_cast<decltype(oReset)*>(reset_pattern) = oReset;
}

namespace G
{
	static Hooking::VMTHook SurfaceManager;
	static Hooking::VMTHook ClientManager;
	static Hooking::VMTHook ClientModeManager;
	static Hooking::VMTHook PanelManager;
	static Hooking::VMTHook BSPQueryManager;
	static Hooking::VMTHook ModelRenderManager;
	static Hooking::VMTHook RenderViewManager;
	static Hooking::VMTHook EngineSoundManager;
	static Hooking::VMTHook ClientStateManager;
	static Hooking::VMTHook GameEventsManager;
}

bool Cheat::Initialize()
{
	if ( !Settings->UIDevMode )
	{
		SetupOffsets();
		MaterialHelper = new CMaterialHelper();
		Utilities->Console_Log( xorstr_("Preparing hooking managers (1/1)") );
	}

	std::unique_ptr<Hooking::VMTHook> DirectXManager = std::make_unique<Hooking::VMTHook>();
	//DirectXManager->Setup( **reinterpret_cast<void*** >( ( Utilities->Memory_PatternScan( xorstr_("shaderapidx9.dll"), xorstr_("A1 ? ? ? ? 50 8B 08 FF 51 0C") ) + 1 ) ) );
	if (!Settings->UIDevMode)
	{
		G::SurfaceManager = Hooking::VMTHook(Interfaces::Surface);
		G::ClientManager = Hooking::VMTHook(Interfaces::Client);
		G::ClientModeManager = Hooking::VMTHook(Interfaces::ClientMode);
		G::PanelManager = Hooking::VMTHook(Interfaces::GuiPanel);
		G::BSPQueryManager = Hooking::VMTHook(Interfaces::Engine->GetBSPTreeQuery());
		G::ModelRenderManager = Hooking::VMTHook(Interfaces::ModelRender);
		G::RenderViewManager = Hooking::VMTHook(Interfaces::RenderView);
		G::EngineSoundManager = Hooking::VMTHook(Interfaces::EngineSound);
		G::ClientStateManager = Hooking::VMTHook((void*)((uintptr_t)Interfaces::ClientState + 0x8));
		G::GameEventsManager = Hooking::VMTHook(Interfaces::GameEvents);
		//CCSPlayer->Setup(Interfaces::Client->LevelInitPostEntity());
	}
	//std::unique_ptr<Hooking::VMTHook> ccsplayer = std::make_unique<Hooking::VMTHook>();
	//auto ccsplayer = new Hooking::VFTableHook((void*)(Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x47));
	//std::unique_ptr<Hooking::VMTHook> ccsplayer = std::make_unique<Hooking::VMTHook>(reinterpret_cast<uintptr_t*>(Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x47));
	//Hooking::VMTHook* ccsplayer = new Hooking::VMTHook(*reinterpret_cast<uintptr_t**>(Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x47));
	//ccsplayer->Setup(reinterpret_cast<void**>(Cheat::Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C") + 0x47));
	//std::unique_ptr<Hooking::VMTHook> BSPQuery = std::make_unique<Hooking::VMTHook>();

	if ( !Settings->UIDevMode )
	{
		Utilities->Console_Log( xorstr_("Initialize GameEventListeners (1/1)") );
		g_pGameEvents.InitializeEventListeners();
		Utilities->Console_Log( xorstr_("Hooking VPanel (1/1)") );
		G::PanelManager.Hook( 41, PaintTraverseHook );
		oPaintTraverse = G::PanelManager.GetOriginal<fnPaintTraverse>( 41 );
		G::BSPQueryManager.Hook(6, listLeavesInBox);
		olistLeavesInBox = G::BSPQueryManager.GetOriginal<listLeavesInBoxfn>(6);
		Utilities->Console_Log( xorstr_("Hooking RenderView (1/1)") );
		G::RenderViewManager.Hook( 9, SceneEndHook );
		oSceneEnd = G::RenderViewManager.GetOriginal<fnSceneEnd>( 9 );
		Utilities->Console_Log( xorstr_("Hooking ModelRender (1/1)") );
		G::ModelRenderManager.Hook( 21, DrawModelExecuteHook );
		oDrawModelExecute = G::ModelRenderManager.GetOriginal<fnDrawModelExecute>( 21 );
		Utilities->Console_Log( xorstr_("Hooking Surface (1/1)") );
		G::SurfaceManager.Hook( 67, LockCursorHook );
		oLockCursor = G::SurfaceManager.GetOriginal<fnLockCursor>( 67 );
		// G::ClientStateManager.Hook(64, ReadPacketEntitiesHook);
		// oReadPacketEntities = G::ClientStateManager.GetOriginal<fnReadPacketEntities>(64);
		G::GameEventsManager.Hook(9, FireEventHook);
		oFireEvent = G::GameEventsManager.GetOriginal<fnFireEvent>(9);
	}

	/*Utilities->Console_Log( xorstr_("Hooking DirectX 9 (1/2)") ); // commented for test
	DirectXManager->Hook( 17, PresentHook );
	oPresent = DirectXManager->GetOriginal<fnPresent>( 17 );
	Utilities->Console_Log( xorstr_("Hooking DirectX 9 (2/2)") );
	DirectXManager->Hook( 16, ResetHook );
	oReset = DirectXManager->GetOriginal<fnReset>( 16 );*/

	if ( !Settings->UIDevMode )
	{
		Utilities->Console_Log( xorstr_("Hooking Client (1/1)") );
		G::ClientManager.Hook( 37, FrameStageNotifyHook );
		oFrameStageNotify = G::ClientManager.GetOriginal<fnFrameStageNotify>( 37 );
		G::ClientManager.Hook(24, hooked_writeusercmddeltatobuffer);
		oWriteUsercmdDeltaToBuffer = G::ClientManager.GetOriginal<fnDoubletap>(24);
		Utilities->Console_Log( xorstr_("Hooking ClientMode (1/3)") );
		G::ClientModeManager.Hook( 24, CreateMoveHook );
		oCreateMove = G::ClientModeManager.GetOriginal<fnCreateMove>( 24 );
		////ccsplayer->Hook(197, hkDoExtraBonesProcessing);
		////ccsplayer->Hook(205, hkStandartBlendingRules);
		////ccsplayer->apply<fnhkDoExtraBonesProcessing>(197, hkDoExtraBonesProcessing);
		////ohkDoExtraBonesProcessing = ccsplayer->GetOriginal<fnhkDoExtraBonesProcessing>(197);
		Utilities->Console_Log( xorstr_("Hooking ClientMode (2/3)") );
		G::ClientModeManager.Hook( 44, DoPostScreenSpaceEffectsHook );
		oDoPostScreenSpaceEffects = G::ClientModeManager.GetOriginal<fnDoPostScreenSpaceEffects>( 44 );
		Utilities->Console_Log( xorstr_("Hooking ClientMode (3/3)") );
		G::ClientModeManager.Hook( 18, OverrideViewHook );
		oOverrideView = G::ClientModeManager.GetOriginal<fnOverrideView>( 18 );

		static auto dwDoExtraBonesProcessing = (DWORD)((Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C")));
		static auto dwShouldSkipAnimFrame = (DWORD)(Utilities->Memory_PatternScan("client.dll", "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02 5F C3"));
		// static auto dwStandardBlendingRules = (DWORD)(Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"));
		static auto dwMove = (DWORD)((Utilities->Memory_PatternScan("engine.dll", "55 8B EC 81 EC ? ? ? ? 53 56 57 8B 3D ? ? ? ? 8A")));


		OriginalDoExtraBonesProcessing = (DWORD)DetourFunction((byte*)dwDoExtraBonesProcessing, (byte*)Hooked::DoExtraBonesProcessing);
		OriginalShouldSkipAnimFrame = (DWORD)DetourFunction((byte*)dwShouldSkipAnimFrame, (byte*)Hooked::ShouldSkipAnimFrame);
		// oStandardBlendingRules = (fnStandardBlendingRules)DetourFunction((PBYTE)dwStandardBlendingRules, (PBYTE)Hooked::StandardBlendingRules);
		oMove = (MoveFn)DetourFunction((PBYTE)dwMove, (PBYTE)Hooked::Move);

		Utilities->Console_Log(xorstr_("Hooking NetVars (1/1)"));
		NetVarManager->hookProp("CBaseViewModel", "m_nSequence", SequenceHook, oSequence);
	}

	/*Utilities->Console_Log(xorstr_("Hooking EngineSound (1/1)"));
	G::EngineSoundManager->Hook(5, emit_sound);
	oEmitSound = G::EngineSoundManager->GetOriginal<fnEmitSound>(5);
	*/

	Utilities->Console_Log( xorstr_("Hooking WndProc") );
	/*HWND hWnd = FindWindow( xorstr_("Valve001"), 0 );
	oWndProc = ( WNDPROC )SetWindowLongPtr( hWnd, GWL_WNDPROC, ( LONG )&WndProcHook );*/

	HWND hWnd = FindWindow(xorstr_("Valve001"), 0);
	oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWL_WNDPROC, (long)&wndProc); if (!oWndProc) return false;

//	Utilities->Console_Log( xorstr_("Saving hooking managers (1/8)") );
	//Hooking::Hooks->AddHookMgr( xorstr_("d3dx9"), std::move( DirectXManager ) );

	return true;
}

void Cheat::Unload()
{
	HWND hWnd = FindWindow( xorstr_("Valve001"), 0 );
	SetWindowLongPtr( hWnd, GWL_WNDPROC, ( LONG )oWndProc );
	//Hooking::Hooks->ReleaseHookMgr( xorstr_("d3dx9") );

	if ( !Settings->UIDevMode )
	{
		DetourRemove((PBYTE)OriginalDoExtraBonesProcessing, (PBYTE)Hooked::DoExtraBonesProcessing);
		DetourRemove((PBYTE)OriginalShouldSkipAnimFrame, (PBYTE)Hooked::ShouldSkipAnimFrame);
		// DetourRemove((PBYTE)oStandardBlendingRules, (PBYTE)Hooked::StandardBlendingRules);
		DetourRemove((PBYTE)oMove, (PBYTE)Hooked::Move);
	
		NetVarManager->hookProp("CBaseViewModel", "m_nSequence", oSequence);
		G::SurfaceManager.RestoreTable();
		G::ClientManager.RestoreTable();
		G::ClientModeManager.RestoreTable();
		G::PanelManager.RestoreTable();
		G::BSPQueryManager.RestoreTable();
		G::ModelRenderManager.RestoreTable();
		G::RenderViewManager.RestoreTable();
		G::EngineSoundManager.RestoreTable();
		G::ClientStateManager.RestoreTable();
		G::GameEventsManager.RestoreTable();

		g_pGameEvents.RemoveListeners();

		delete Cheat::NetVarManager, MaterialHelper;
	}
}

unsigned int FindInDataMap(datamap_t* pMap, const char* name)
{
	while (pMap)
	{
		for (int i = 0; i < pMap->dataNumFields; i++)
		{
			if (pMap->dataDesc[i].fieldName == NULL)
				continue;

			if (strcmp(name, pMap->dataDesc[i].fieldName) == 0)
				return pMap->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

			if (pMap->dataDesc[i].fieldType == 10)
			{
				if (pMap->dataDesc[i].td)
				{
					unsigned int offset;

					if ((offset = FindInDataMap(pMap->dataDesc[i].td, name)) != 0)
						return offset;
				}
			}
		}
		pMap = pMap->baseMap;
	}

	return 0;
}