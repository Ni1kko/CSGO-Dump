#include "..\hooks.hpp"
#include "..\..\cheats\sdk\animation_state.h"
#include "..\..\cheats\visuals\other_esp.h"
#include "..\..\cheats\misc\circlestrafer.h"
#include "..\..\cheats\misc\prediction_system.h"
#include "..\..\cheats\legit\legit_backtrack.h"
#include "..\..\cheats\misc\bunnyhop.h"
#include "..\..\cheats\misc\airstrafe.h"
#include "..\..\cheats\lagcompensation\lagcompensation.h"
#include "..\..\cheats\misc\spammers.h"
#include "..\..\cheats\fakewalk\fakewalk.h"
#include "../../cheats/misc/fakelag.h"
#include "..\..\ragebot.h"
#include "..\..\Resolver_Remake.h"
#include "..\..\PWalk.h"
#include "..\..\Backtracking.h"
#include "..\..\extrapolation.h"
#include "..\..\autodefuser.exe.h"
using CreateMove_t = bool(__thiscall*)(IClientMode *, float, CUserCmd *);

bool datagram_setup = false;

bool __stdcall hooks::hooked_createmove(float smt, CUserCmd * m_pcmd) {
	static auto original_fn = clientmode_hook->get_func_address< CreateMove_t >(24);

	//if (!m_pcmd->m_command_number || !g_ctx.available() || !g_ctx.m_local)
	//	return original_fn(g_csgo.m_clientmode(), smt, m_pcmd);

	//if (g_cfg.misc.clantag_spammer)
	//	spammers::get().clan_tag();

	//if (!g_ctx.m_local->is_alive32())
	//	return original_fn(g_csgo.m_clientmode(), smt, m_pcmd);

	//g_ctx.set_command(m_pcmd);

	//util::server_time(m_pcmd);
	//if (g_cfg.misc.ping_spike && !netchannel_hook) {
	//	auto netchannel = g_csgo.m_clientstate()->m_NetChannel;
	//	if (netchannel) {
	//		netchannel_hook = new vmthook(reinterpret_cast<DWORD**>(netchannel));
	//		netchannel_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_senddatagram), 46);
	//	}
	//}
	//if (g_cfg.misc.sniper_crosshair) {
	//	static bool o = false;
	//	if (!o) { o = true;	g_csgo.m_cvar()->FindVar("weapon_debug_spread_show")->SetValue(3); }
	//}
	//else {
	//	static bool o = false;
	//	if (!o) { o = true;	g_csgo.m_cvar()->FindVar("weapon_debug_spread_show")->SetValue(0); }
	//}
	//ResolverOvermake::resolver.DoCM();
	//animation_state::get().create_move();
	//uintptr_t *frame_ptr;
	//__asm mov frame_ptr, ebp;
	//g_ctx.send_packet = true;
	//g_ctx.m_globals.fakelagging = false;
	//vec3_t original_angle = m_pcmd->m_viewangles;
	//if (g_cfg.legit.backtracking)
	//	legitbacktrack::get().create_move();
	//g_ctx.unpred_velocity = g_ctx.m_local->m_vecVelocity();

	//engineprediction::get().start();
	//{			//HERE
	//	fakewalk::get().create_move();

	//	fakelag::get().create_move(m_pcmd);

	//	if (g_cfg.misc.customviewmodel) {
	//		int viewX = g_cfg.misc.CustomViewModelX;

	//		ConVar* viewXcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_x");
	//		*(int*)((DWORD)&viewXcvar->fnChangeCallback + 0xC) = NULL;
	//		viewXcvar->SetValue(viewX);

	//		int viewY = g_cfg.misc.CustomViewModelY;

	//		ConVar* viewYcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_y");
	//		*(int*)((DWORD)&viewYcvar->fnChangeCallback + 0xC) = NULL;
	//		viewYcvar->SetValue(viewY);

	//		int viewZ = g_cfg.misc.CustomViewModelZ;

	//		ConVar* viewZcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_z");
	//		*(int*)((DWORD)&viewZcvar->fnChangeCallback + 0xC) = NULL;

	//		viewZcvar->SetValue(viewZ);
	//	}
	//	if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(CMBacktracking::get().current_record->m_nFlags & FL_ONGROUND)) {
	//		*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
	//	}
	//	if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && g_csgo.m_inputsys()->IsButtonDown(KEY_SPACE)) {
	//		*(bool*)(*(DWORD*)frame_ptr - 0x1C) = false;
	//	}
	//	if (!g_ctx.m_local->m_fFlags() & FL_ONGROUND && !g_csgo.m_inputsys()->IsButtonDown(KEY_SPACE)) {
	//		*(bool*)(*(DWORD*)frame_ptr - 0x1C) = false;
	//	}
	//	for (int i = 1; i < 2; i++)
	//	{
	//		if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][i].flags)) {
	//			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
	//		}
	//	}
	//	if (g_cfg.ragebot.extrapolation)
	//		LinearExtrapolationsrun();
	//	CMBacktracking::get().StartLagCompensation(g_ctx.m_local);

	//	if (g_cfg.misc.bunnyhop)
	//		bunnyhop::get().create_move();
	//	if (g_cfg.misc.airstrafe)
	//		airstrafe::get().create_move();
	//	vec3_t wish_angle = m_pcmd->m_viewangles;
	//	if (!g_cfg.esp.fakelagchams)
	//		m_pcmd->m_viewangles = original_angle;
	//	if (!g_csgo.m_clientstate()->m_nChokedCommands)


	//	antiaim::get().create_move();

	//	if (g_cfg.ragebot.enable)
	//		aimbot::get().create_move();

	//	g_ctx.m_globals.add_shot();

	//	util::movement_fix(wish_angle, m_pcmd);
	//}
	//lagcompensation::get().create_move();

	//m_pcmd->m_buttons |= IN_BULLRUSH;
	//if (m_pcmd->m_buttons & IN_DUCK)
	//{
	//	bool CurrentTickSendPacket, LastTickSendPacket;
	//	int lasttime, sendpacketsticks, notsendpacketticks, autism1, autism2, ifyoureadthisyouareabadpaster;
	//	// clamp choked commands
	//}
	//if (g_csgo.m_clientstate()->m_nChokedCommands > 14)
	//	fridge::ConsolePrint("WARNING: Chocked Commands extended 14! \n");
	//if (g_csgo.m_clientstate()->m_nChokedCommands > 14)
	//	g_ctx.send_packet = true;
	//// set send_packet
	//*(bool*)(*frame_ptr - 0x1C) = g_ctx.send_packet;
	//if (g_ctx.m_local && g_ctx.m_local->is_alive33())
	//{
	//	if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][0].flags & FL_ONGROUND)) {
	//		if (GetAsyncKeyState(VK_SPACE)) {
	//			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
	//		}
	//	}
	//	if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][2].flags & FL_ONGROUND)) {
	//		if (GetAsyncKeyState(VK_SPACE)) {
	//			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
	//		}
	//	}
	//	if (g_ctx.m_local->m_fFlags() & FL_ONGROUND && !(headPositions[g_ctx.m_local->EntIndex()][1].flags & FL_ONGROUND)) {
	//		if (GetAsyncKeyState(VK_SPACE)) {
	//			*(bool*)(*(DWORD*)frame_ptr - 0x1C) = true;
	//		}
	//	}
	//}

	//// get angles for thirdperson
	//if (!g_cfg.esp.fakelagchams)
	//{
	//	if (g_ctx.send_packet) {
	//		g_ctx.m_globals.fake_angles = m_pcmd->m_viewangles;
	//	}
	//	else
	//		g_ctx.m_globals.real_angles = m_pcmd->m_viewangles;
	//}
	//if (g_cfg.misc.anti_untrusted)
	//	math::normalize_angles(m_pcmd->m_viewangles);
	//if (m_pcmd->m_forwardmove > 450.0f)
	//	m_pcmd->m_forwardmove = 450.0f;
	//else if (m_pcmd->m_forwardmove < -450.0f)
	//	m_pcmd->m_forwardmove = -450.0f;
	//if (m_pcmd->m_sidemove > 450.0f)
	//	m_pcmd->m_sidemove = 450.0f;
	//else if (m_pcmd->m_sidemove < -450.0f)
	//	m_pcmd->m_sidemove = -450.0f;

	//return false;
	if (!m_pcmd->m_command_number || !g_ctx.available() || !g_ctx.m_local)
		return original_fn(g_csgo.m_clientmode(), smt, m_pcmd);

	if (g_cfg.misc.clantag_spammer)
		spammers::get().clan_tag();

	if (!g_ctx.m_local->is_alive())
		return original_fn(g_csgo.m_clientmode(), smt, m_pcmd);

	g_ctx.set_command(m_pcmd);

	// update fixed server time
	util::server_time(m_pcmd);

	// ping spike
	if (g_cfg.misc.ping_spike && !netchannel_hook) {
		auto netchannel = g_csgo.m_clientstate()->m_NetChannel;

		if (netchannel) {
			netchannel_hook = new vmthook(reinterpret_cast<DWORD**>(netchannel));
			netchannel_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_senddatagram), 46);
		}
	}

	if (g_cfg.misc.sniper_crosshair) {
		static bool o = false;
		if (!o) { o = true;	g_csgo.m_cvar()->FindVar("weapon_debug_spread_show")->SetValue(3); }
	}
	else {
		static bool o = false;
		if (!o) { o = true;	g_csgo.m_cvar()->FindVar("weapon_debug_spread_show")->SetValue(0); }
	}

	// update animations
	animation_state::get().create_move();

	uintptr_t *frame_ptr;
	__asm mov frame_ptr, ebp;

	g_ctx.send_packet = true;
	g_ctx.m_globals.fakelagging = false;
	vec3_t original_angle = m_pcmd->m_viewangles;

	// legit backtracking
	if (g_cfg.legit.backtracking)
		legitbacktrack::get().create_move();

	// upredicted velocity for lby breaker
	g_ctx.unpred_velocity = g_ctx.m_local->m_vecVelocity();

	// engine prediction
	engineprediction::get().start();
	{
		// fakewalk
		fakewalk::get().create_move();

		// fakelag
		fakelag::get().create_move(m_pcmd);

		// bunnyhopping
		if (g_cfg.misc.bunnyhop)
			bunnyhop::get().create_move();

		// airstrafe
		if (g_cfg.misc.airstrafe)
			airstrafe::get().create_move();

		vec3_t wish_angle = m_pcmd->m_viewangles;
		m_pcmd->m_viewangles = original_angle;

		// update lowerbody breaker
		if (!g_csgo.m_clientstate()->m_nChokedCommands)
			antiaim::get().update_lowerbody_breaker();

		// antiaim
		antiaim::get().create_move();

		// aimbot
		if (g_cfg.ragebot.enable)
			aimbot::get().create_move();

		// add shots to counter
		g_ctx.m_globals.add_shot();

		// fix movement
		util::movement_fix(wish_angle, m_pcmd);
	}

	lagcompensation::get().create_move();

	// clamp choked commands
	if (g_csgo.m_clientstate()->m_nChokedCommands > 14)
		g_ctx.send_packet = true;

	m_pcmd->m_buttons |= IN_BULLRUSH;

	// set send_packet
	*(bool*)(*frame_ptr - 0x1C) = g_ctx.send_packet;

	if (g_cfg.misc.customviewmodel) {
		int viewX = g_cfg.misc.CustomViewModelX;

		ConVar* viewXcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_x");
		*(int*)((DWORD)&viewXcvar->fnChangeCallback + 0xC) = NULL;
		viewXcvar->SetValue(viewX);

		int viewY = g_cfg.misc.CustomViewModelY;

		ConVar* viewYcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_y");
		*(int*)((DWORD)&viewYcvar->fnChangeCallback + 0xC) = NULL;
		viewYcvar->SetValue(viewY);

		int viewZ = g_cfg.misc.CustomViewModelZ;

		ConVar* viewZcvar = g_csgo.m_cvar()->FindVar("viewmodel_offset_z");
		*(int*)((DWORD)&viewZcvar->fnChangeCallback + 0xC) = NULL;

		viewZcvar->SetValue(viewZ);
	}

	// get angles for thirdperson
	if (!g_ctx.send_packet)
	{
		g_ctx.m_globals.real_angles = m_pcmd->m_viewangles;

		if (animation_state::get().animstate()->m_bInHitGroundAnimation)
			g_ctx.m_globals.real_angles.x = -12.0f;
	}
	else
		g_ctx.m_globals.real_angles = m_pcmd->m_viewangles;

	// clamp angles for anti-ut
	if (g_cfg.misc.anti_untrusted)
		math::normalize_angles(m_pcmd->m_viewangles);

	// movement clamp
	if (m_pcmd->m_forwardmove > 450.0f)
		m_pcmd->m_forwardmove = 450.0f;
	else if (m_pcmd->m_forwardmove < -450.0f)
		m_pcmd->m_forwardmove = -450.0f;

	if (m_pcmd->m_sidemove > 450.0f)
		m_pcmd->m_sidemove = 450.0f;
	else if (m_pcmd->m_sidemove < -450.0f)
		m_pcmd->m_sidemove = -450.0f;

	return false;
}