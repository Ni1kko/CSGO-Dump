#include "..\hooks.hpp"
#include "..\..\cheats\lagcompensation\lagcompensation.h"
#include "..\..\cheats\visuals\nightmode.h"
#include "..\..\cheats\visuals\other_esp.h"
#include "..\..\Skins.h"
#include "..\..\Resolver_Remake.h"
#include "../../Skins.h"
#include "../../Recv.h"
#include "..\..\Backtracking.h"
#define RandomInt(min, max) (rand() % (max - min + 1) + min)
using FrameStageNotify_t = void(__stdcall*)(ClientFrameStage_t);

vec3_t flb_aim_punch;
vec3_t flb_view_punch;

vec3_t * aim_punch;
vec3_t * view_punch;

void remove_smoke() {
	static auto smoke_count = *reinterpret_cast<uint32_t **>(util::pattern_scan("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 1);

	static std::vector<const char*> smoke_materials = {
		"effects/overlaysmoke",
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull"
	};

	for (auto material_name : smoke_materials) {
		IMaterial * material = g_csgo.m_materialsystem()->FindMaterial(material_name, "Other textures");

		material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_cfg.esp.removesmoke);
	}

	if (g_cfg.esp.removesmoke)
		*(int*)smoke_count = 0;
}

weapon_t * local_weapon;

void thirdperson() {
	static bool
		setting,
		holding,
		in_thirdperson,
		last_dead;

	if (g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.thirdperson_toggle) && !holding) {
		setting = !setting;

		if (g_ctx.m_local->is_alive34()) {
			g_csgo.m_engine()->ExecuteClientCmd(setting ? "thirdperson" : "firstperson");
			in_thirdperson = setting;
		}
		holding = true;
	}
	else if (!g_csgo.m_inputsys()->IsButtonDown(g_cfg.misc.thirdperson_toggle) && holding)
		holding = false;

	if (g_ctx.m_local->is_alive35())
		return;

	if (in_thirdperson) {
		g_csgo.m_engine()->ExecuteClientCmd("firstperson");
		g_ctx.m_local->m_iObserverMode() = OBS_MODE_IN_EYE;
		in_thirdperson = false;
	}

	if (!setting && last_dead) {
		g_ctx.m_local->m_iObserverMode() = OBS_MODE_IN_EYE;
		last_dead = false;
	}

	if (setting && !last_dead) {
		g_ctx.m_local->m_iObserverMode() = OBS_MODE_CHASE;
		last_dead = true;
	}
}

void update_spoofer() {
	if (!g_ctx.available()) {
		if (g_csgo.m_cvarspoofer()->ready())
			g_csgo.m_cvarspoofer()->release();

		return;
	}

	if (!g_csgo.m_cvarspoofer()->ready()) {
		g_csgo.m_cvarspoofer()->init();

		g_csgo.m_cvarspoofer()->add("sv_cheats");
		g_csgo.m_cvarspoofer()->add("r_drawspecificstaticprop");

		g_csgo.m_cvarspoofer()->spoof();
	}

	if (g_csgo.m_cvarspoofer()->ready()) {
		g_csgo.m_cvarspoofer()->get("sv_cheats")->SetInt(1);
		//g_csgo.m_cvarspoofer()->get("sv_pure")->SetInt(1);
	}
}

void __stdcall hooks::hooked_fsn(ClientFrameStage_t stage) {
	static auto original_fn = client_hook->get_func_address<FrameStageNotify_t>(37);

	if (!g_csgo.m_engine()->IsConnected() || !g_csgo.m_engine()->IsInGame())
		return original_fn(stage);;
	auto clientstate = *(uintptr_t *)(g_csgo.m_clientstate());
	if (!clientstate || !g_ctx.available()) {
		if (netchannel_hook) {
			netchannel_hook->clear_class_base();
			delete netchannel_hook;
			netchannel_hook = nullptr;
		}
	}
	else if (!g_csgo.m_clientstate()->m_NetChannel && netchannel_hook) {
		netchannel_hook->clear_class_base();
		delete netchannel_hook;
		netchannel_hook = nullptr;
	}

	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!g_ctx.available() || !local_player) {
		nightmode::get().clear_stored_materials();

		lagcompensation::get().clear_sequence();

		return original_fn(stage);;
	}
	for (int i = 0; i < g_csgo.m_globals()->m_maxclients; i++)
	{
		auto m_entity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (i == g_csgo.m_engine()->GetLocalPlayer())
			continue;

		player_t* pCurEntity = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
		if (!pCurEntity)
			continue;

		CTickRecord trans = CTickRecord(pCurEntity);
		CBacktracking::get().ClearRecord(pCurEntity);
		CBacktracking::get().SaveTemporaryRecord(pCurEntity, trans);
		//fridge::ConsolePrint("Cleared Backtrack Record \n");
		*(int*)((uintptr_t)pCurEntity + 0xA30) = g_csgo.m_globals()->m_framecount;
		*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;
	}
	g_ctx.m_local = local_player;

	aim_punch = nullptr;
	view_punch = nullptr;

	flb_aim_punch = vec3_t{ };
	flb_view_punch = vec3_t{ };

	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		lagcompensation::get().update_sequence();
		if (g_cfg.ragebot.lagcomp)
		{
			CMBacktracking::get().StartLagCompensation(local_player);
			CMBacktracking::get().FrameUpdatePostEntityThink();
		}

		//skinchanger();
	}
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
		if (g_cfg.ragebot.lagcomp)
		{
			CMBacktracking::get().FrameUpdatePostEntityThink();
			CMBacktracking::get().FinishLagCompensation(local_player);
		}
	}
	if (stage == FRAME_NET_UPDATE_END)
	{
		if (g_cfg.ragebot.lagcomp)
		{
			CMBacktracking::get().FrameUpdatePostEntityThink();
		}
	}
	if (stage == FRAME_RENDER_START) {
		update_spoofer();

		thirdperson();

		/* removals *///you're chatting shit theres more in post you fag		//fak off men
		remove_smoke();

		if (g_cfg.esp.removeflash && g_ctx.m_local->m_flFlashDuration() > 0.0f)
			g_ctx.m_local->m_flFlashDuration() = 0.0f;

		if (*(bool*)g_csgo.m_postprocessing() != g_cfg.esp.removescope)
			*(bool*)g_csgo.m_postprocessing() = g_cfg.esp.removescope;

		if (g_cfg.esp.removerecoil) {
			aim_punch = &g_ctx.m_local->m_aimPunchAngle();
			view_punch = &g_ctx.m_local->m_viewPunchAngle();

			flb_aim_punch = *aim_punch;
			flb_view_punch = *view_punch;

			*aim_punch = vec3_t(0, 0, 0);
			*view_punch = vec3_t(0, 0, 0);
		}

		for (int i = 1; i <= g_csgo.m_entitylist()->GetHighestEntityIndex(); i++) {
			auto e = static_cast<entity_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

			if (!e)
				continue;

			if (e->IsDormant())
				continue;

			if (e->GetClientClass()->m_ClassID == CCSRagdoll) {
				auto ragdoll = (ragdoll_t*)e;

				if (g_cfg.misc.exaggerated_ragdolls) {
					vec3_t
						new_force = ragdoll->m_vecForce() * g_cfg.misc.exaggerated_ragdolls_multiplier,
						new_velocity = ragdoll->m_vecRagdollVelocity() * g_cfg.misc.exaggerated_ragdolls_multiplier;

					ragdoll->m_vecForce() = new_force;
					ragdoll->m_vecRagdollVelocity() = new_velocity;
				}
			}
		}

		if (g_ctx.m_local->is_alive36()) {
			if (g_csgo.m_input()->m_fCameraInThirdPerson)
				*(Vector*)((uintptr_t)g_ctx.m_local + 0x31D8) = g_ctx.m_globals.real_angles;
		}
	}

	if (stage == FRAME_RENDER_END) {
		if (g_cfg.esp.nightmode)
			nightmode::get().apply();
		else
			nightmode::get().remove();
		if (g_cfg.ragebot.lagcomp)
			CMBacktracking::get().FrameUpdatePostEntityThink();
	}
	lagcompensation::get().fsn(stage);
	ResolverOvermake::resolver.DoFSN(stage);
	original_fn(stage);

	if (g_ctx.m_local && g_ctx.m_local->is_alive37()) {
		if (g_cfg.esp.removerecoil && (aim_punch && view_punch)) {
			*aim_punch = flb_aim_punch;
			*view_punch = flb_view_punch;
		}
	}
}