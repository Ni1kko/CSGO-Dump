#pragma once
#include "../../Configuration/Config.h"
#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/CClientEntityList.h"
#include "../../Features/skinchanger/cskin.h"
#include "..\\..\\Features\Ragebot\animation.h"
#include "../../Features/Ragebot/Resolver.h"
#include "../../Features/Ragebot/c_backtrack.h"
#include "../Hooks/Hooks.h"
#include "../../Features/Ragebot/c_playerlist.h" // da
#include "../../Features/Ragebot/animation.h" // da
#include "../../Features/Skinchanger/skinchanger2.h"

typedef void( __stdcall* fnFrameStageNotify )( int );
extern fnFrameStageNotify oFrameStageNotify = nullptr;

void walls_transparency(int value, bool force) {
	static int m_iValue = 100;

	if (value != m_iValue || force) {
		for (
			MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial();
			i != Interfaces::MaterialSystem->InvalidMaterial();
			i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World"))
			{
				if (strstr(pMaterial->GetTextureGroupName(), "World"))
					pMaterial->AlphaModulate((float)value/ 100.f);
			}
		}

		m_iValue = value;
	}
}

void props_transparency(int value, bool force) {
	static int m_iValue = 100;
	static ConVar* r_drawspecificstaticprop;

	if (!r_drawspecificstaticprop)
		r_drawspecificstaticprop = Interfaces::Var->FindVar("r_drawspecificstaticprop");

	r_drawspecificstaticprop->SetValue(0);

	if (value != m_iValue || force) {
		for (
			MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial();
			i != Interfaces::MaterialSystem->InvalidMaterial();
			i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
					pMaterial->AlphaModulate((float)value / 100.f);
			}
		}

		m_iValue = value;
	}
}

void nightmode(int value, bool force) {
	static int m_iValue = 100;
	static int oldr = 0;
	static int oldg = 0;
	static int oldb = 0;
	static ConVar* r_drawspecificstaticprop;
	static bool m_bSwitchedSkybox = false;
	static bool m_bReturnedSkybox = true;
	static std::string m_szOldSkybox = "";
	static auto fnLoadSky = reinterpret_cast<void(__fastcall*)(const char*)>(offys.dwLoadSky);

	if (!r_drawspecificstaticprop)
		r_drawspecificstaticprop = Interfaces::Var->FindVar("r_drawspecificstaticprop");

	r_drawspecificstaticprop->SetValue(0);

	if (force) {
		m_bSwitchedSkybox == false;
		m_bReturnedSkybox = true;
	}

	if (value != m_iValue || force) {
		if (value < 100 && !m_bSwitchedSkybox) {
			m_szOldSkybox = Interfaces::Var->FindVar("sv_skyname")->GetString();

			fnLoadSky("sky_csgo_night02");
			m_bSwitchedSkybox = true;
			m_bReturnedSkybox = false;
		}
		else {
			if (value >= 100 && !m_bReturnedSkybox) {
				fnLoadSky(m_szOldSkybox.c_str());
				m_bSwitchedSkybox = false;
				m_bReturnedSkybox = true;
			}
		}

		for (
			MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial();
			i != Interfaces::MaterialSystem->InvalidMaterial();
			i = Interfaces::MaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pMaterial) {
					auto br = 40.f / 100.f;
					auto nmr = c_config::get()->c["vis_nightmode_color"][0];
					auto nmg = c_config::get()->c["vis_nightmode_color"][1];
					auto nmb = c_config::get()->c["vis_nightmode_color"][2];

					if (value < 100) {
						pMaterial->ColorModulate(nmr, nmg, nmb);
					}
					else if (value >= 100) {
						oldr = 1.0f;
						oldg = 1.0f;
						oldb = 1.0f;
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
					}
				}
			}
		}

		m_iValue = value;
	}
	
}

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**> (Utilities->Memory_PatternScan("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1); //(Utilities::Memory::FindPatternV2("client.dll", "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89 46 24") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utilities->Memory_PatternScan("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
void NotReallyUsefull()
{
	/*if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		auto local_player = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());
		auto animations = local_player->GetAnimState();

		if (!animations)
			return;
		if (!local_player)
			return;

//		*local_player->clientsideanim() = true;
//
//		/*auto old_curtime = Interfaces::Globals->curtime;
//		auto old_frametime = Interfaces::Globals->frametime;
//		auto old_ragpos = local_player->get_ragdoll_pos();
//
//		Interfaces::Globals->curtime = local_player->GetSimTime();
//		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
//
//		auto player_animation_state = local_player->GetAnimState();
//		auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
//		if (player_animation_state != nullptr && player_model_time != nullptr)
//			if (*player_model_time == Interfaces::Globals->framecount)
//				*player_model_time = Interfaces::Globals->framecount - 1;
//
//		local_player->get_ragdoll_pos() = old_ragpos;
//		local_player->UpdateClientSideAnimation();
//
//		Interfaces::Globals->curtime = old_curtime;
//		Interfaces::Globals->frametime = old_frametime;
//
//		local_player->SetAbsAngles(Vector(0.f, local_player->GetAnimState()->m_flGoalFeetYaw, 0.f));//if u not doin dis it f*cks up the model lol
//
//		*local_player->clientsideanim() = false;
	}*/
}

namespace Cheat
{
	namespace Hooked
	{
		

		void __stdcall FrameStageNotifyHook(ClientFrameStage_t stage)
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.FrameStageNotify = 0;
				oFrameStageNotify(stage);
				return;
			}
			Cheat::Settings->UnloadReadyTable.FrameStageNotify = (int)GetTickCount();

			G::LocalPlayer = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());

			if (!G::LocalPlayer || !Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected())
			{
				G::ResetCheatState = true;
				//Backtrack->clear_sequence();
				return oFrameStageNotify(stage);
			}

			walls_transparency(c_config::get()->i["vis_wallstransp"], G::ResetCheatState);
			props_transparency(c_config::get()->i["vis_propstransp"], G::ResetCheatState);
			nightmode(c_config::get()->b["vis_nightmode"] ? 50 : 100, G::ResetCheatState);

			if (G::ResetCheatState)
				offys.dwPlayerResource = *(DWORD*)(Utilities->Memory_PatternScan(Settings->ClientDLL, xorstr_("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7")) + 2);

			G::ResetCheatState = false;
			/*Skin_OnFrameStageNotify(stage);
			Gloves_OnFrameStageNotify(stage);*/
			Features::LagCompensation->FrameStage(stage);
			static Vector oldViewPunch;
			static Vector oldAimPunch;
			Vector* view_punch = G::LocalPlayer->GetViewPunchPtr();
			Vector* aim_punch = G::LocalPlayer->GetPunchAnglePtr();

			if (SkinChanger::updating_skins && Interfaces::ClientState->iDeltaTick > 0) //-V807
				SkinChanger::updating_skins = false;

			SkinChanger::run(stage);

			if (!G::LocalPlayer->IsAlive() && G::LocalPlayer->GetObserverTarget() && c_config::get()->b["vis_thirdperson_dead"])
				G::LocalPlayer->SetObserverMode(OBS_MODE_CHASE);

			//if (stage == FRAME_NET_UPDATE_END)
			//{
			//	Skins::OnFrameStageNotify(true);
			//	//g_Animation->ManageLocalFake();
			//}

			if (stage == FRAME_RENDER_START)
			{
				Animation::get()->update_local_ghetto(G::LocalPlayer);	

				if (G::LocalPlayer->GetFlags() & FL_ONGROUND && c_config::get()->b["rage_fakeduck"] && c_keyhandler::get()->auto_check("rage_fakeduck_key") && G::IsInThirdPerson)
					G::LocalPlayer->GetAnimState()->m_fDuckAmount = 1.f;

				static bool enabledtp = false, check = false, oldCrosshair = false, oldScoped = false;

				if (!G::LocalPlayer->IsAlive() && !c_keyhandler::get()->auto_check("vis_thirdperson_key")) {
					Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
					G::IsInThirdPerson = false;
				}

				if (c_keyhandler::get()->auto_check("vis_thirdperson_key") && c_config::get()->b["vis_thirdperson"] && G::LocalPlayer->IsAlive() && G::LocalPlayer->GetHealth() > 0) {
					Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
					if (G::UserCmd) {
						*(Vector*)((uintptr_t)G::LocalPlayer + 0x31D8) = Vector(G::UserCmd->viewangles.x, G::RealAngle.y, G::RealAngle.z);
					}
						
					G::IsInThirdPerson = true;
				}
				else {
					Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
					G::IsInThirdPerson = false;
				}

					Visuals->remove_smoke();

				if (c_config::get()->b["vis_noflash"] && *G::LocalPlayer->FlashAlpha() > 0.f)
					*G::LocalPlayer->FlashAlpha() = 0.f;

				static ConVar* PostProcVar = Interfaces::Var->FindVar("mat_postprocess_enable");

				if (c_config::get()->b["vis_nopostprocess"])
					PostProcVar->SetValue(0);

				else
					PostProcVar->SetValue(1);

				static ConVar* sv_showimpacts = Interfaces::Var->FindVar("sv_showimpacts");
				if (c_config::get()->b["vis_bulletimpacts"])
					sv_showimpacts->SetValue(1);
				else
					sv_showimpacts->SetValue(0);

				static ConVar* zoom_sensitivity_ratio_mouse = Interfaces::Var->FindVar("zoom_sensitivity_ratio_mouse");
				if (c_config::get()->b["vis_nozoom"])
					zoom_sensitivity_ratio_mouse->SetValue(0);
				else
					zoom_sensitivity_ratio_mouse->SetValue(1);

				if (c_config::get()->b["vis_novisrecoil"] && G::LocalPlayer->IsAlive() && G::LocalPlayer->GetHealth() > 0)
				{
					oldViewPunch = *view_punch;
					oldAimPunch = *aim_punch;
					view_punch->Zero();
					aim_punch->Zero();
				}

				if (oldCrosshair != c_config::get()->b["vis_crosshair"] || G::LocalPlayer->IsScoped() != oldScoped) // макмс пердун
				{
					static auto* DrawCrosshairWeaponTypeCheck = reinterpret_cast<char*>(offys.dwCrosshair);
					if (DrawCrosshairWeaponTypeCheck != nullptr) // ?????
					{
						DWORD dwOldProtect;
						DWORD dwNewProtect;
						VirtualProtect(DrawCrosshairWeaponTypeCheck, 3, PAGE_EXECUTE_READWRITE, &dwOldProtect);

						if (oldCrosshair != c_config::get()->b["vis_crosshair"])
							*((DrawCrosshairWeaponTypeCheck + 0x2)) = c_config::get()->b["vis_crosshair"] ? 0xFF : 0x5;

						else
							*((DrawCrosshairWeaponTypeCheck + 0x2)) = G::LocalPlayer->IsScoped() ? 0x5 : 0xFF;

						VirtualProtect(DrawCrosshairWeaponTypeCheck, 3, dwOldProtect, &dwNewProtect);
						oldCrosshair = c_config::get()->b["vis_crosshair"];
						oldScoped = G::LocalPlayer->IsScoped();
					}
				}

				static bool ReallocatedDeathNoticeHUD = false;
				static void(__thiscall *ClearDeathNotices)(DWORD);
				static DWORD* deathNotice;

				if (G::PK_RoundStarted || G::PK_Reset) {
					ReallocatedDeathNoticeHUD = false;
					G::PK_RoundStarted = false;
					G::PK_Reset = false;
				}
				if (c_config::get()->b["vis_preservekillfeed"]) {
					if (!ReallocatedDeathNoticeHUD)
					{
						ReallocatedDeathNoticeHUD = true;
						deathNotice = FindHudElement<DWORD>("CCSGO_HudDeathNotice");
						ClearDeathNotices = (void(__thiscall*)(DWORD))Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");
					}
					else
					{
						if (deathNotice && !G::PK_Reset)
						{
							if (!G::PK_RoundStarted)
							{
								float* localDeathNotice = (float*)((DWORD)deathNotice + 0x50);
								if (localDeathNotice)
									*localDeathNotice = c_config::get()->b["vis_preservekillfeed"] ? 30.f : 1.5f;
							}

							if (G::roundstartxuy && deathNotice - 20)
							{
								if (ClearDeathNotices)
									ClearDeathNotices(((DWORD)deathNotice - 20));

								G::roundstartxuy = false;
							}
						}
						else
							G::PK_Reset = false;
					}
				}
				
				
			}
		
			if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
				for (int i = 0; i < 65; i++) {
					auto e = Interfaces::EntityList->GetClientEntity(i);

					if (!e)
						continue;
					if (e == G::LocalPlayer)
						continue;
					if (e->GetTeam() == G::LocalPlayer->GetTeam())
						continue;


					c_playerlist::get()->apply_for_player(e);
				}
				if (c_config::get()->b["skin_enabled"]) {

				}
				if (c_config::get()->b["rage_backtrack"]) {
					//c_backtrack::get()->update(Interfaces::Globals->tickcount);
				}
			}

	
			if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
				if (c_config::get()->b["misc_showaiminfo"]) {
					if (G::MissingDueToSpread) {
						if (G::RagebotTarget) {
							if (G::RagebotTarget->IsAlive()) {
								/*G::MissedShots[G::RagebotTarget->GetIndex()]++;
								G::ResolverAdd[G::RagebotTarget->GetIndex()] += 35.f;*/
								Cheat::Utilities->Math_NormalizeYaw(G::ResolverAdd[G::RagebotTarget->GetIndex()]);
							}
							else {
								/*G::MissedShots[G::RagebotTarget->GetIndex()] = 0;
								G::ResolverAdd[G::RagebotTarget->GetIndex()] = 0;*/
							}
						}

						//Utilities->Game_Msg("Missed shot due to spread");
						G::MissingDueToSpread = false;
					}
				}
			}
			if (stage == FRAME_NET_UPDATE_END) {
				for (int i = 0; i < 65; i++) {
					auto e = Interfaces::EntityList->GetClientEntity(i);

					if (!e)
						continue;
					if (e == G::LocalPlayer)
						continue;
					if (e->GetTeam() == G::LocalPlayer->GetTeam() && !c_config::get()->b["rage_friendlyfire"])
						continue;

					if (c_config::get()->b["rage_resolver"])
						c_resolver::get()->AnimationFix(e);
				}
			}
			//Backtrack->fsn(stage);

			for (auto hk : c_lua::get()->hooks->getHooks("on_frame_stage_notify"))
				hk.func((int)stage);

			oFrameStageNotify(stage);
			if (stage == FRAME_RENDER_START)
			{
				if (G::LocalPlayer->IsAlive() && G::LocalPlayer->GetHealth() > 0)
				{
					if (c_config::get()->b["vis_novisrecoil"])
					{
						*aim_punch = oldAimPunch;
						*view_punch = oldViewPunch;
					}
				}

				for (int i = 1; i < Interfaces::Globals->maxclients; i++) {
					CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity(i);
					if (pEntity) {
						if (pEntity->GetHealth() > 0 && !pEntity->IsDormant()) {
							g_BacktrackHelper->UpdateBacktrackRecords(pEntity);

							if (c_config::get()->b["rage_resolver"])
								c_resolver::get()->Resolver(pEntity);

							if (c_config::get()->b["vis_radar"]) {
								if (pEntity->GetTeam() != G::LocalPlayer->GetTeam())
									*pEntity->IsSpotted() = true;
							}
						}
					}
				}
			}
		}
	}
}