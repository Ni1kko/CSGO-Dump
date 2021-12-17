#pragma once
#include "../Include/Def.h"
#include "../Hooks/Render.h"
#include "../Hooks/Tables.h"
#include "../Engine/CSX/CSX_Memory.h"
#include "../Engine/CSX/CSX_Utils.h"
#include "../Engine/XorStr.h"
#include "../Engine/SDK/SDK.h"
#include "../Engine/NetVar.h"
#include "../Hooks/CreateMove.h"
#include "../Hooks/Views.h"
#include  "../Hooks/Sound.h"
#include "../Hooks/FrameStageNotify.h"
#include "../Hooks/DrawModelExecute.h"
#include "../Hacks/Setup.h"
#include "../License/License.h"
#include "Offsets.h"

using namespace SDK;
using namespace Engine;


class IIInit
{
public:
	virtual class IInit
	{
	public:
		virtual void InitHooks() = 0;
		virtual void InitOffsets() = 0;
		virtual bool Init() = 0;
		virtual void SendSteamId32() = 0;
		int ii = 0;
	};
};

inline int decod(const char *s)
{
	int z = 0, n = 0;
	while (isspace(*s)) s++;
	if (*s == '-') { z = 1; s++; }
	else if (*s == '+') s++;
	while (isdigit(*s)) n = 10 * n + *s++ - '0';
	return (z ? -n : n);
}

#define ResetIdx XorStr("16")
#define CreateMoveIdx XorStr("24")
#define OverrideViewIdx XorStr("18")
#define GetViewModelFOVIdx XorStr("35")
#define SSendMessageIdx XorStr("0")
#define RetrieveMessageIdx XorStr("2")
#define FireEventClientSideThinkIdx XorStr("9")
#define FrameStageNotifyIdx XorStr("37")
#define EmitSound1Idx XorStr("5")
#define EmitSound2Idx XorStr("6")
#define DrawModelExecuteIdx XorStr("21")
#define PlaySoundIdx XorStr("82")
#define LockCursorIdx XorStr("67")
#define ResetIdx XorStr("16")

using namespace HookTables;

class CInit : public IIInit
{
public:
	virtual class IInit
	{
	public:
		virtual void InitHooks()
		{
			auto LInitHooks = [&]() -> void
			{
				ADD_LOG("======================Init hooks:\n");
				ADD_LOG("2-1-11-0\n");
				auto& pContext = cContext::GetInstance();
				ADD_LOG("2-1-11-1\n");
				PVOID* SurfaceTable = *reinterpret_cast<PVOID**>(I::Surface());
				//PVOID* SteamGameCoordinatorTable = *reinterpret_cast<PVOID**>(I::SteamGameCoordinator());
				PVOID* ClientModeTable = *reinterpret_cast<PVOID**>(I::ClientMode());
				PVOID* ClientTable = *reinterpret_cast<PVOID**>(I::Client());
				PVOID* ModelRenderTable = *reinterpret_cast<PVOID**>(I::ModelRender());
				ADD_LOG("2-1-11-2\n");
#ifndef ONLY_DRAW_HOOK
				ADD_LOG("2-1-11-3\n");
				if (SurfaceTable)
				{
					pContext.ApplyDetour<PlaySoundFn>(static_cast<PlaySoundFn>(SurfaceTable[decod(PlaySoundIdx)/*82*/]),
						reinterpret_cast<PlaySoundFn>
						(PlaySound),
						&pPlaySound);
					ADD_LOG("Hook: Sound\n");

					pContext.ApplyDetour<LockCursorFn>(static_cast<LockCursorFn>(SurfaceTable[decod(LockCursorIdx)/*67*/]),
						reinterpret_cast<LockCursorFn>
						(LockCursor),
						&pLockCursor);
					ADD_LOG("Hook: Cursor\n");
				}
				ADD_LOG("2-1-11-4\n");
				//if (SteamGameCoordinatorTable)
				//{
				//	pContext.ApplyDetour<SendMessageFn>(static_cast<SendMessageFn>(SteamGameCoordinatorTable[decod(SSendMessageIdx)/*0*/]),
				//		reinterpret_cast<SendMessageFn>
				//		(SSendMessage),
				//		&pSendMessage);
				//	ADD_LOG("Hook: SteamSend\n");

				//	pContext.ApplyDetour<RetrieveMessageFn>(static_cast<RetrieveMessageFn>(SteamGameCoordinatorTable[decod(RetrieveMessageIdx)/*2*/]),
				//		reinterpret_cast<RetrieveMessageFn>
				//		(RetrieveMessage),
				//		&pRetrieveMessage);
				//	ADD_LOG("Hook: SteamRetrieve\n");
				//}
				ADD_LOG("2-1-11-5\n");
				if (ClientModeTable)
				{
					pContext.ApplyDetour<CreateMoveFn>(static_cast<CreateMoveFn>(ClientModeTable[decod(CreateMoveIdx)/*24*/]),
						reinterpret_cast<CreateMoveFn>
						(CreateMove),
						&pCreateMove);
					ADD_LOG("Hook: CreateMove\n");

					pContext.ApplyDetour<OverrideViewFn>(static_cast<OverrideViewFn>(ClientModeTable[decod(OverrideViewIdx)/*18*/]),
						reinterpret_cast<OverrideViewFn>
						(OverrideView),
						&pOverrideView);
					ADD_LOG("Hook: OverrideView\n");

					pContext.ApplyDetour<GetViewModelFOVFn>(static_cast<GetViewModelFOVFn>(ClientModeTable[decod(GetViewModelFOVIdx)/*35*/]),
						reinterpret_cast<GetViewModelFOVFn>
						(GetViewModelFOV),
						&pGetViewModelFOV);
					ADD_LOG("Hook: OverrideView\n");
				}
				ADD_LOG("2-1-11-6\n");
				if (ClientTable)
				{
					pContext.ApplyDetour<FrameStageNotifyFn>(static_cast<FrameStageNotifyFn>(ClientTable[decod(FrameStageNotifyIdx)/*37*/]),
						reinterpret_cast<FrameStageNotifyFn>
						(FrameStageNotify),
						&pFrameStageNotify);
					ADD_LOG("Hook: StageNotify\n");
				}

				if (ModelRenderTable)
				{
					pContext.ApplyDetour<DrawModelExecuteFn>(static_cast<DrawModelExecuteFn>(ModelRenderTable[decod(DrawModelExecuteIdx)/*21*/]),
						reinterpret_cast<DrawModelExecuteFn>
						(DrawModelExecute),
						&pDrawModelExecute);
					ADD_LOG("Hook: DME\n");
				}

#endif
				ADD_LOG("2-1-11-7\n");
				GP_Setup = new CSetup::ISetup();
				GP_Setup->Setup();
				ADD_LOG("2-1-11-8\n");
				ADD_LOG("All Hooks sucessful\n");
			};
			LInitHooks();
			ADD_LOG("2-1-11-9\n");
		}
		virtual void InitOffsets()
		{
#define mGetOffset(table, prop) g_NetVar.GetOffset(XorStr(table), XorStr(prop))
			auto LInitOffsets = [&]() -> void
			{
				ADD_LOG("2-1-9-0\n");
				ADD_LOG("======================Init Offsets:\n");

				offsets["m_hMyWeapons"] = mGetOffset("DT_BasePlayer", "m_hMyWeapons") / 2;
				offsets["m_hMyWearables"] = mGetOffset("DT_BasePlayer", "m_hMyWearables");
				offsets["m_vecOrigin"] = mGetOffset("DT_BasePlayer", "m_vecOrigin");
				offsets["m_hViewModel"] = mGetOffset("DT_BasePlayer", "m_hViewModel[0]");
				offsets["m_nTickBase"] = mGetOffset("DT_BasePlayer", "m_nTickBase");
				offsets["m_iObserverMode"] = mGetOffset("DT_BasePlayer", "m_iObserverMode");
				offsets["m_hObserverTarget"] = mGetOffset("DT_BasePlayer", "m_hObserverTarget");
				offsets["deadflag"] = mGetOffset("DT_BasePlayer", "deadflag");
				offsets["m_bIsDefusing"] = mGetOffset("DT_CSPlayer", "m_bIsDefusing");
				offsets["m_iAccount"] = mGetOffset("DT_CSPlayer", "m_iAccount");
				offsets["m_lifeState"] = mGetOffset("DT_CSPlayer", "m_lifeState");
				offsets["m_flFlashDuration"] = mGetOffset("DT_CSPlayer", "m_flFlashDuration");
				offsets["m_flC4Blow"] = mGetOffset("DT_PlantedC4", "m_flC4Blow");
				offsets["m_flDefuseCountDown"] = mGetOffset("DT_PlantedC4", "m_flDefuseCountDown");
				offsets["m_hBombDefuser"] = mGetOffset("DT_PlantedC4", "m_hBombDefuser");
				offsets["m_bBombDefused"] = mGetOffset("DT_PlantedC4", "m_bBombDefused");
				offsets["m_flFlashMaxAlpha"] = mGetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
				offsets["m_bHasHelmet"] = mGetOffset("DT_CSPlayer", "m_bHasHelmet");
				offsets["m_bHasDefuser"] = mGetOffset("DT_CSPlayer", "m_bHasDefuser");
				offsets["m_bIsScoped"] = mGetOffset("DT_CSPlayer", "m_bIsScoped");
				offsets["m_iFOVStart"] = mGetOffset("DT_CSPlayer", "m_iFOVStart");
				offsets["m_fFlags"] = mGetOffset("DT_CSPlayer", "m_fFlags");
				offsets["m_iHealth"] = mGetOffset("DT_CSPlayer", "m_iHealth");
				offsets["m_ArmorValue"] = mGetOffset("DT_CSPlayer", "m_ArmorValue");
				offsets["m_iTeamNum"] = mGetOffset("DT_CSPlayer", "m_iTeamNum");
				offsets["m_iShotsFired"] = mGetOffset("DT_CSPlayer", "m_iShotsFired");
				offsets["m_aimPunchAngle"] = mGetOffset("DT_CSPlayer", "m_aimPunchAngle");
				offsets["m_viewPunchAngle"] = mGetOffset("DT_CSPlayer", "m_viewPunchAngle");
				offsets["m_vecVelocity"] = mGetOffset("DT_CSPlayer", "m_vecVelocity[0]");
				offsets["m_vecPunchAngles"] = mGetOffset("DT_BasePlayer", "m_aimPunchAngle");
				offsets["m_vecViewOffset"] = mGetOffset("DT_CSPlayer", "m_vecViewOffset[0]");
				offsets["m_angEyeAngles"] = mGetOffset("DT_CSPlayer", "m_angEyeAngles");
				offsets["m_hActiveWeapon"] = mGetOffset("DT_CSPlayer", "m_hActiveWeapon");
				offsets["m_iClip1"] = mGetOffset("DT_BaseCombatWeapon", "m_iClip1");
				offsets["m_iClip2"] = mGetOffset("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
				offsets["m_flNextPrimaryAttack"] = mGetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
				offsets["m_bCanReload"] = offsets["m_flNextPrimaryAttack"] + 0x6D;
				offsets["m_iItemDefinitionIndex"] = mGetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
				offsets["m_bIsAutoaimTarget"] = mGetOffset("DT_BaseEntity", "m_bIsAutoaimTarget");
				offsets["m_iItemIDHigh"] = mGetOffset("DT_BaseAttributableItem", "m_iItemIDHigh");
				offsets["m_iAccountID"] = mGetOffset("DT_BaseAttributableItem", "m_iAccountID");
				offsets["m_iEntityQuality"] = mGetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
				offsets["m_OriginalOwnerXuidLow"] = mGetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
				offsets["m_OriginalOwnerXuidHigh"] = mGetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
				offsets["m_nFallbackPaintKit"] = mGetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
				offsets["m_flFallbackWear"] = mGetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
				offsets["m_nFallbackSeed"] = mGetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
				offsets["m_szCustomName"] = mGetOffset("DT_BaseAttributableItem", "m_szCustomName");
				offsets["m_nFallbackStatTrak"] = mGetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
				offsets["m_nModelIndex"] = 0x254;
				offsets["m_iViewModelIndex"] = 0x3210;
				offsets["m_hOwner"] = mGetOffset("DT_BaseViewModel", "m_hOwner");
				offsets["m_hWeapon"] = mGetOffset("DT_BaseViewModel", "m_hWeapon");
				offsets["m_bSpotted"] = mGetOffset("DT_BaseEntity", "m_bSpotted");
				offsets["m_zoomLevel"] = mGetOffset("DT_WeaponAWP", "m_zoomLevel");
				offsets["m_flSimulationTime"] = mGetOffset("DT_BaseEntity", "m_flSimulationTime");
				offsets["m_hWeaponWorldModel"] = mGetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel");
				offsets["m_iWorldModelIndex"] = mGetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
				ADD_LOG("2-1-9-1\n");
				offsets["KeyValues_KeyValues"] = CSX::Memory::FindPatternV2(GetClientDll(), KEY_VALUES_MASK);
				offsets["KeyValues_LoadFromBuffer"] = CSX::Memory::FindPatternV2(GetClientDll(), KEY_VALUES_LOAD_FROM_BUFFER_MASK);
				//ADD_LOG("Offsets init\n");
				ADD_LOG("All Offsets sucessful\n");
				ADD_LOG("2-1-9-2\n");
			};
			LInitOffsets();
			ADD_LOG("2-1-9-3\n");
		}

		vector<void*> InterfaceList;
		vector<void*> ModList;
		using getValue = DWORD(__stdcall*)(void);
		using getValueModule = bool(_cdecl*)(PCHAR, DWORD);

		virtual void SendSteamId32()
		{
			VMP_ULTRA("SendSteamId32");
			ADD_LOG("2-1-10-0\n");
			CSteamID steam_id = I::SteamUser()->GetSteamID();
			ADD_LOG("2-1-10-1\n");
			uint32 id32 = steam_id.GetAccountID();
			ADD_LOG("2-1-10-2\n");
			CInternet Inet(XorStr("dceowysgpv.info"));
			ADD_LOG("2-1-10-3\n");
			string id_str = to_string(id32);
			string type_str = CHEAT_TYPE;
			ADD_LOG("2-1-10-4\n");
			string url = XorStr("/XONE/SteamBans/AddAccount.php?ID=");
			url += base64_encode((const unsigned char *)id_str.c_str(), id_str.length());
			url += XorStr("&CHEAT=");
			url += base64_encode((const unsigned char *)type_str.c_str(), type_str.length());
			url += XorStr("&TTT=23FGDSG457");
			ADD_LOG("2-1-10-5\n");
			Inet.GetData(url);
			ADD_LOG("2-1-10-6\n");
			VMP_END;
		}

		virtual bool Init()
		{
			VMP_ULTRA("SetupThread_Init");
			auto LInit = [&]() -> bool
			{
#ifdef ENABLE_LICENSE
				ADD_LOG("2-1-0\n");
				try
				{
					CLicense::GetCheatUser();
					CLic1::ILic2* dab = new CLic1::ILic2();
					ADD_LOG("2-1-1\n");
					if (!dab)
					{
						ADD_LOG("2-1-1-1\n");
						FastCall::G().t_Beep(100, 100);
						return false;
					}

					ADD_LOG("2-1-2\n");
					if (!dab->Check())
					{
						ADD_LOG("2-1-2-1\n");
						FastCall::G().t_Beep(100, 100);
						FastCall::G().t_Beep(100, 100);
						FastCall::G().t_Beep(100, 100);

						return false;
					}
					ADD_LOG("2-1-3\n");
					if (!CLicense::CheckMemes())
					{
						ADD_LOG("2-1-3-1\n");
						FastCall::G().t_Beep(100, 100);
						FastCall::G().t_Beep(100, 100);
						return false;
					}

				}
				catch (const exception& e)
				{
				}
#endif

#ifndef ONLY_DRAW_HOOK
				ADD_LOG("2-1-4\n");
				//ModList.push_back(CSX::Utils::IsModuleLoad);
				//getValueModule CheckModule = (getValueModule)ModList.back();
				//if(!CheckModule)
				//	return false;
				ADD_LOG("2-1-5\n");
				if (!CSX::Utils::IsModuleLoad(ENGINE_DLL, 5001))
					return false;
				if (!CSX::Utils::IsModuleLoad(VGUI2_DLL, 5001))
					return false;
				if (!CSX::Utils::IsModuleLoad(VGUIMAT_DLL, 5001))
					return false;
				if (!CSX::Utils::IsModuleLoad(VSTDLIB_DLL, 5001))
					return false;
				if (!CSX::Utils::IsModuleLoad(STEAMAPI_DLL, 5001))
					return false;
				if (!CSX::Utils::IsModuleLoad(SERVERBROWSER_DLL, 40000))
					return false;

				FastCall::G().t_Sleep(1500);

				if (!CSX::Utils::IsModuleLoad("client_panorama.dll", 5001))
					return false;

				/*ADD_LOG("2-1-6\n");
				if (!I::Engine())
				{
					return false;
				}

				if (!I::Client())
				{
					return false;
				}

				if (!I::EntityList())
				{
					return false;
				}

				if (!I::GlobalVars())
				{
					return false;
				}


				if (!I::EngineTrace())
				{
					return false;
				}

				if (!I::ClientMode())
				{
					return false;
				}

				if (!I::ModelInfo())
				{
					return false;
				}

				if (!I::Sound())
				{
					return false;
				}

				if (!I::ModelRender())
				{
					return false;
				}

				if (!I::RenderView())
				{
					return false;
				}

				if (!I::MaterialSystem())
				{
					return false;
				}

				if (!I::Surface())
				{
					return false;
				}

				if (!I::GameEvent())
				{
					return false;
				}

				if (!I::InputSystem())
				{
					return false;
				}

				if (!I::SteamGameCoordinator())
				{
					return false;
				}*/
				/*InterfaceList.push_back((void*)I::Engine);
				InterfaceList.push_back((void*)I::Client);
				InterfaceList.push_back((void*)I::EntityList);
				InterfaceList.push_back((void*)I::GlobalVars);
				InterfaceList.push_back((void*)I::EngineTrace);
				InterfaceList.push_back((void*)I::ClientMode);
				InterfaceList.push_back((void*)I::ModelInfo);
				InterfaceList.push_back((void*)I::Sound);
				InterfaceList.push_back((void*)I::ModelRender);
				InterfaceList.push_back((void*)I::RenderView);
				InterfaceList.push_back((void*)I::MaterialSystem);
				InterfaceList.push_back((void*)I::Surface);
				InterfaceList.push_back((void*)I::GameEvent);
				InterfaceList.push_back((void*)I::InputSystem);
				InterfaceList.push_back((void*)I::SteamGameCoordinator);
				ADD_LOG("2-1-7\n");
				for (auto Intf : InterfaceList)
				{
					ADD_LOG("2-1-7-0\n");
					if (Intf)
					{
						getValue value = (getValue)Intf;
						if (!value())
							return false;
					}
					else
						return false;
				}*/
				ADD_LOG("2-1-8\n");
				if (!g_NetVar.Init(I::Client()->GetAllClasses()))
				{
					ADD_LOG("2-1-9-8-1\n");
					FastCall::G().t_MessageBoxA(0, "AllClasses error", "Error!", 0);
					return false;
				}
				ADD_LOG("2-1-9\n");
				InitOffsets();
				ADD_LOG("2-1-10\n");
				SendSteamId32();
#endif 
				ADD_LOG("2-1-11\n");
				InitHooks();
				return true;
			};
			return LInit();
			VMP_END;
		}
	};
};