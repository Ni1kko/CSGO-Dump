#pragma once
#include "Hooks.h"
#include "RageBacktracking.h"
#include "Resolver.h"
#include "AnimationFix.h"
#include "SetupAnimation.h"
#include <intrin.h>
#include <random>
#include "Ragebot.h"

bool __fastcall Hooked_GetBool(void* pConVar, void* edx)
{
	static auto SvCheatsGetBool = g_pGetBoolHook->GetOriginal< SvCheatsGetBoolFn >(13);

	if (csgo->DoUnload)
		return SvCheatsGetBool(pConVar);

	static auto CAM_THINK = csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::retn_camera::s().c_str());
	if (_ReturnAddress() == CAM_THINK)
		return true;

	return SvCheatsGetBool(pConVar);
}

static bool enabledtp = false, check = false;

void UpdateCSGOKeyBinds()
{
	if (vars.misc.thirdperson)
	{
		if (!check)
			enabledtp = !enabledtp;
		check = true;
	}
	else
		check = false;


	csgo->SwitchAA = vars.antiaim.inverter->active;
}

void __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	static auto FrameStageNotify = g_pClientHook->GetOriginal< FrameStageNotifyFn >(37);

	csgo->updatelocalplayer();

	if (csgo->client_state != nullptr) {
		if (csgo->g_pNetChannelHook && csgo->g_pNetChannelHook->hooked) {
			if (csgo->client_state->pNetChannel != nullptr) {
				uintptr_t* vtable = *(uintptr_t**)csgo->client_state->pNetChannel;

				if (vtable != csgo->g_pNetChannelHook->shadow_vtable) {
					csgo->g_pNetChannelHook.reset();
				}
			}
			else
				csgo->g_pNetChannelHook.reset();
		}
	}

	if (!interfaces.engine->IsConnected() || !interfaces.engine->IsInGame())
		csgo->mapChanged = true;

	if (!csgo->local || !csgo->local->isAlive()) {
		csgo->ForceOffAA = false;

		if (curStage == FRAME_NET_UPDATE_END) {
			for (int i = 1; i < 65; i++) {
				auto entity = interfaces.ent_list->GetClientEntity(i);
				if (entity != nullptr && entity->IsPlayer() && entity != csgo->local) {
					entity->GetClientSideAnims() = csgo->EnableBones = true;
					entity->UpdateClientSideAnimation();
					entity->GetClientSideAnims() = csgo->EnableBones = false;
				}
			}
			csgo->disable_dt = false;
		}
		return FrameStageNotify(curStage);
	}

	if (curStage == FRAME_RENDER_START)
	{
		if (csgo->game_rules == nullptr || csgo->mapChanged) {
			csgo->game_rules = **reinterpret_cast<CCSGameRules***>(csgo->Utils.FindPatternIDA(GetModuleHandleA(
				g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
				hs::game_rules::s().c_str()) + 0x1);
		}
	}

	if (curStage == FRAME_RENDER_START) {
		features->Visuals->NightMode();
		features->Visuals->agent_changer((ClientFrameStage_t)curStage);
	}

	UpdateCSGOKeyBinds();

	static auto cycle = 0.f;
	static auto anim_time = 0.f;

	if (interfaces.engine->IsConnected() && interfaces.engine->IsInGame())
	{
		static int m_iLastCmdAck = 0;
		static float m_flNextCmdTime = 0.f;

		if (csgo->local && csgo->local->isAlive()) {
			if (csgo->client_state && (m_iLastCmdAck != csgo->client_state->nLastCommandAck || m_flNextCmdTime != csgo->client_state->flNextCmdTime))
			{
				if (csgo->g_flVelMod != csgo->local->GetVelocityModifier())
				{
					*(bool*)((uintptr_t)interfaces.prediction + 0x24) = true;
					csgo->g_flVelMod = csgo->local->GetVelocityModifier();
				}

				m_iLastCmdAck = csgo->client_state->nLastCommandAck;
				m_flNextCmdTime = csgo->client_state->flNextCmdTime;
			}

		}
		if (curStage == FRAME_NET_UPDATE_END)
		{
			static auto r_jiggle_bones = interfaces.cvars->FindVar("r_jiggle_bones");
			r_jiggle_bones->m_fnChangeCallbacks.m_Size = 0;
			if (r_jiggle_bones->GetInt() > 0)
				r_jiggle_bones->SetValue(0);

			for (int i = 1; i < 64; i++) {
				auto entity = interfaces.ent_list->GetClientEntity(i);
				if (csgo->local && entity != nullptr && entity->IsPlayer() && entity != csgo->local && entity->isAlive()) {
					const auto var_map = reinterpret_cast<uintptr_t>(entity) + 36;

					for (auto index = 0; index < *reinterpret_cast<int*>(var_map + 20); index++)
						*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(var_map) + index * 12) = 0;
				}
			}
			g_Animfix->UpdateFakeState();
			g_Animfix->ApplyLocalPlayer();
			g_Animfix->UpdatePlayers();
		}

		if (curStage == FRAME_RENDER_START)
		{
			if (csgo->local->isAlive()) {
				if (enabledtp)
					*(Vector*)((DWORD)csgo->local + 0x31D8) = csgo->FakeAngle;

				if (csgo->local->GetFlags() & FL_ONGROUND) {
					csgo->local->GetPlayerAnimState()->m_bOnGround = true;
					csgo->local->GetPlayerAnimState()->m_bInHitGroundAnimation = false;
				}
				csgo->local->SetAbsAngles(Vector(0, csgo->local->GetPlayerAnimState()->m_flGoalFeetYaw, 0));

				interfaces.input->m_fCameraInThirdPerson = false;
				if (vars.visuals.remove & 4)
					csgo->local->GetFlashDuration() = 0.f;

				static float old_simtime = 0.f;
				if (old_simtime != csgo->local->GetSimulationTime())
				{
					csgo->all_data[0].sim_time = old_simtime;
					csgo->all_data[0].interp_time = 0.f;
					csgo->all_data[0].origin = csgo->local->GetAbsOriginVec();
					old_simtime = csgo->local->GetSimulationTime();
				}

				csgo->all_data[1].sim_time = csgo->local->GetSimulationTime();

				if (!csgo->send_packet) {
					csgo->all_data[1].interp_time = 0.f;
					csgo->all_data[1].origin = csgo->local->GetAbsOriginVec();
				}
				auto animstate = csgo->local->GetPlayerAnimState();
				if (csgo->animstate && animstate)
					csgo->desync_angle = clamp(fabsf(Math::NormalizeYaw(csgo->animstate->m_flGoalFeetYaw - animstate->m_flGoalFeetYaw)), 0.f, 58.f);
			}
			features->BulletTracer->Proceed();
		}

		csgo->disable_dt = false;
		csgo->ForceOffAA = false;
	}
	else {
		csgo->disable_dt = false;
		csgo->mapChanged = true;
		csgo->dt_charged = false;
		csgo->skip_ticks = 0;
	}

	FrameStageNotify(curStage);
}

bool __fastcall Hooked_ShouldDrawFog(void* ecx, void* edx) {
	return !(vars.visuals.remove & 64);
}

bool __fastcall Hooked_ShouldDrawShadow(void*, uint32_t) {
	return !(vars.visuals.remove & 128);
}
