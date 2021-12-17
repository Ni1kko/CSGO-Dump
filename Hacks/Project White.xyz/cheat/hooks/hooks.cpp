#include "hooks.hpp"
#include <intrin.h>  

#include "../render/render.hpp"
#include "../menu/menu.hpp"
#include "../config/options.hpp"
#include "../../helpers/input.hpp"
#include "../../helpers/utils.hpp"
#include "../visuals/chams.hpp"
#include "../visuals/visuals.hpp"
#include "../visuals/glow.hpp"
#include "../legit/aimbot.hpp"
#include "../legit/backtrack.hpp"
#include "../visuals/grenade_prediction.hpp"
#include "../other/engine_prediction.hpp"
#include "../skins/skins.hpp"
#include "../misc/misc.hpp"
#include "../../sdk/structs.hpp"

#include "minhook.hpp"
#pragma comment(lib, "minhook.lib")

#pragma intrinsic(_ReturnAddress)  

float side = 1.0f;
float next_lby = 0.0f;
float real_angle = 0.0f;
float view_angle = 0.0f;
static int max_choke_ticks = 14;
static CCSGOPlayerAnimState g_AnimState;
//--------------------------------------------------------------------------------
void Hooks::Initialize() {
	ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
	auto rpe_targ = (void*)((uintptr_t)g_ClientState + 0x8);

	const auto gv_endscene		= reinterpret_cast<void*>(get_virtual(g_D3DDevice9,			index::EndScene));
	const auto gv_reset			= reinterpret_cast<void*>(get_virtual(g_D3DDevice9,			index::Reset));
	const auto gv_crmove		= reinterpret_cast<void*>(get_virtual(g_ClientMode,			index::CreateMove));
	const auto gv_painttr		= reinterpret_cast<void*>(get_virtual(g_VGuiPanel,			index::PaintTraverse));
	const auto gv_fsn			= reinterpret_cast<void*>(get_virtual(g_CHLClient,			index::FrameStageNotify));
	const auto gv_sound			= reinterpret_cast<void*>(get_virtual(g_EngineSound,		index::EmitSound1));
	const auto gv_lcursor		= reinterpret_cast<void*>(get_virtual(g_VGuiSurface,		index::LockCursor));
	const auto gv_dme			= reinterpret_cast<void*>(get_virtual(g_MdlRender,			index::DrawModelExecute));
	const auto gv_pse			= reinterpret_cast<void*>(get_virtual(g_ClientMode,			index::DoPostScreenSpaceEffects));
	const auto gv_ovview		= reinterpret_cast<void*>(get_virtual(g_ClientMode,			index::OverrideView));
	const auto gv_event			= reinterpret_cast<void*>(get_virtual(g_GameEvents,			index::FireEvent));
	const auto gv_cheats		= reinterpret_cast<void*>(get_virtual(sv_cheats_con,		index::SvCheatsGetBool));
	const auto gv_aspect_ratio  = reinterpret_cast<void*>(get_virtual(g_EngineClient,		index::AspectRatio));
	const auto gv_suppress_list = reinterpret_cast<void*>(get_virtual(g_SpatialPartition,	index::SuppressLists));
	const auto gv_rpe			= reinterpret_cast<void*>(get_virtual(rpe_targ,				index::ReadPacketEntities));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize hooks.");

	MH_CreateHook(gv_endscene,		&end_scene::hook,	  reinterpret_cast<void**>(&end_scene::o_end_scene));
	MH_CreateHook(gv_reset,			&reset::hook,		  reinterpret_cast<void**>(&reset::o_reset));
	MH_CreateHook(gv_crmove,		&creatmove::hook,	  reinterpret_cast<void**>(&creatmove::o_crmove));
	MH_CreateHook(gv_painttr,		&pant_traverse::hook, reinterpret_cast<void**>(&pant_traverse::o_paint));
	MH_CreateHook(gv_fsn,			&fsn::hook,			  reinterpret_cast<void**>(&fsn::o_fsn));
	MH_CreateHook(gv_sound,			&emit_sound::hook,    reinterpret_cast<void**>(&emit_sound::o_sound));
	MH_CreateHook(gv_lcursor,		&lock_cursour::hook,  reinterpret_cast<void**>(&lock_cursour::o_lock));
	MH_CreateHook(gv_dme,			&dme::hook,			  reinterpret_cast<void**>(&dme::o_dme));
	MH_CreateHook(gv_pse,			&dpse::hook,		  reinterpret_cast<void**>(&dpse::o_effects));
	MH_CreateHook(gv_ovview,		&override_view::hook, reinterpret_cast<void**>(&override_view::o_ovview));
	MH_CreateHook(gv_event,			&fire_event::hook,	  reinterpret_cast<void**>(&fire_event::o_events));
	MH_CreateHook(gv_cheats,		&cheats::hook,		  reinterpret_cast<void**>(&cheats::o_cheats));
	MH_CreateHook(gv_aspect_ratio,  &aspect_ratio::hook,  reinterpret_cast<void**>(&aspect_ratio::o_aspect_ratio));
	MH_CreateHook(gv_suppress_list, &suppress_list::hook, reinterpret_cast<void**>(&suppress_list::o_suppress_list));
	MH_CreateHook(gv_rpe,			&rpe::hook,			  reinterpret_cast<void**>(&rpe::o_rpe));

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	g_GameEvents->AddListener(&Visuals::Get(), "player_hurt", false);

	sequence_hook	 = new recv_prop_hook(C_BaseViewModel::m_nSequence(),    hkRecvProxy);
	m_pFlashDuration = new recv_prop_hook(C_BasePlayer::m_flFlashDuration(), hkFlashDuration);
}
//--------------------------------------------------------------------------------
void Hooks::Shutdown() {
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);

	sequence_hook->~recv_prop_hook();
	m_pFlashDuration->~recv_prop_hook();
	g_GameEvents->RemoveListener(&Visuals::Get());
	Glow::Get().Shutdown();
}
//--------------------------------------------------------------------------------
long __stdcall Hooks::end_scene::hook(IDirect3DDevice9* device) {
	IDirect3DVertexDeclaration9* vertDec;
	IDirect3DVertexShader9* vertShader;
	device->GetVertexDeclaration(&vertDec);
	device->GetVertexShader(&vertShader);

	DWORD colorwrite, srgbwrite;
	IDirect3DVertexDeclaration9* vert_dec = nullptr;
	IDirect3DVertexShader9* vert_shader = nullptr;
	DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	device->GetVertexDeclaration(&vert_dec);
	device->GetVertexShader(&vert_shader);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

	static auto misc_viewmodel_fov = g_CVar->FindVar("viewmodel_fov");

	misc_viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
	misc_viewmodel_fov->SetValue(Cfg.misc_viewmodel_fov);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto esp_drawlist = Render::Get().RenderScene();
	Menu::Get().Render();
	ImGui::Render(esp_drawlist);

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	device->SetVertexDeclaration(vertDec);
	device->SetVertexShader(vertShader);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vert_dec);
	device->SetVertexShader(vert_shader);

	return o_end_scene(device);
}
//--------------------------------------------------------------------------------
long __stdcall Hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters) {
	Menu::Get().OnDeviceLost();

	auto hr = o_reset(device, presentation_parameters);
	if (hr >= 0) Menu::Get().OnDeviceReset();

	return hr;
}
//--------------------------------------------------------------------------------
float final_feet_yaw[64];
float old_feet_yaw[64];
float eye_lby_delta[64];
bool __stdcall Hooks::creatmove::hook(float input_sample_frametime, CUserCmd* cmd) {
	bool* send_packet = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);
	o_crmove(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number || g_Unload) return false;

	static int latency_ticks = 0;
	float fl_latency = g_EngineClient->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
	int latency = TIME_TO_TICKS(fl_latency);;
	if (g_ClientState->m_nChokedCommands <= 0) latency_ticks = latency;
	else latency_ticks = max(latency, latency_ticks);

	if (g_GameRules->m_bIsValveDS()) {
		if (fl_latency >= g_GlobalVars->interval_per_tick) max_choke_ticks = 6 - latency_ticks;
		else max_choke_ticks = 6;
	}
	else max_choke_ticks = 13 - latency_ticks;

	static float SpawnTime = 0.0f;
	if (g_LocalPlayer->m_flSpawnTime() != SpawnTime) {
		g_AnimState.pBaseEntity = g_LocalPlayer;
		g_LocalPlayer->ResetAnimationState(&g_AnimState);
		SpawnTime = g_LocalPlayer->m_flSpawnTime();
	}

	if (Menu::Get().IsVisible()) cmd->buttons &= ~IN_ATTACK | IN_ATTACK2;	

	if (Cfg.misc_bhop)		 Misc::Get().BunnyHop(cmd);
	if (Cfg.misc_autostrafe) Misc::Get().AutoStrafe(cmd);
	if (Cfg.misc_name_spam)	 Misc::Get().Namespam();
	if (Cfg.misc_chat_spam)  Misc::Get().Chatspam();
	if (Cfg.misc_radio_spam) Misc::Get().Radiospam();

	Misc::Get().Clantag();
	Misc::Get().LeftKnife();
	Misc::Get().NoShadows();
	Misc::Get().SniperCrosshair();
	Misc::Get().RecoilCrosshair();

	if (Cfg.legitresolver)
	{
		for (int i = 1; i <= g_GlobalVars->maxClients; i++) {
			auto entity = reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));

			if (!entity
				|| !g_LocalPlayer
				|| !entity->IsAlive()
				|| entity == g_LocalPlayer
				|| entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				continue;

			player_info_t player_info;
			g_EngineClient->GetPlayerInfo(entity->EntIndex(), &player_info);

			if (player_info.fakeplayer) // skip bots
				continue;

			auto anim_state = entity->GetPlayerAnimState();


			// dont forget to include fakelag checks, 
			// some p2cs require up to 3 ticks in order to lby desync,
			// but you can do it with just 1

			// take their feetyaw and normaclampalize for extra safety
			final_feet_yaw[entity->EntIndex()] = Math::Clamp(Math::NormalizeYaw(anim_state->m_flGoalFeetYaw), -180.f, 180.f);

			// can be used in the future if u want
			old_feet_yaw[entity->EntIndex()] = Math::Clamp(Math::NormalizeYaw(old_feet_yaw[entity->EntIndex()]), -180.f, 180.f);

			// calculate their lby delta men!
			eye_lby_delta[entity->EntIndex()] = Math::NormalizeYaw(entity->m_angEyeAngles().yaw - entity->m_flLowerBodyYawTarget());

			// make sure their lby is broken and their standing. once again p2cs can do things differently,
			// they usually jitter their speed between 0.0f and 1.1f so make sure we are above that interval.
			// Other cheats that actually flick to break lby can be checked with 0.0f speed
			if (fabs(eye_lby_delta[entity->EntIndex()]) > 35.f && entity->m_vecVelocity().Length2D() <= 2.f /* && choking[entity->index()] */)
			{
				// now we go, clamped their feetyaw to max range using the opposite of their lby delta, 
				// pretty basic, but its really all you gotta do
				final_feet_yaw[entity->EntIndex()] += Math::Clamp(eye_lby_delta[entity->EntIndex()], -58.f, 58.f);

				// maybe we can save set this old_feet_yaw for future usage,
				// possibly saving their standing desync angle and using it while moving afterwards
				old_feet_yaw[entity->EntIndex()] = final_feet_yaw[entity->EntIndex()];
			}

			// finally set their feet_yaw
			anim_state->m_flGoalFeetYaw = final_feet_yaw[entity->EntIndex()];
		}

	}

	if (Cfg.misc_showranks && cmd->buttons & IN_SCORE) Utils::RankRevealAll();

	EnginePrediction::Begin(cmd); {
		LegitBot::Get().OnMove(cmd);
		Backtrack::Get().OnMove(cmd);
		if (Cfg.esp_grenade_prediction) GrenadePrediction::trace(cmd);

		if (Cfg.misc_desync && std::fabsf(g_LocalPlayer->m_flSpawnTime() - g_GlobalVars->curtime) > 1.0f)
			Misc::Get().Desync(cmd, send_packet);

		Math::FixAngles(cmd->viewangles);
		cmd->viewangles.yaw = std::remainderf(cmd->viewangles.yaw, 360.0f);

		if (Cfg.misc_desync && g_ClientState->m_nChokedCommands >= max_choke_ticks) {
			*send_packet = true;
			cmd->viewangles = g_ClientState->viewangles;
		}

		Misc::Get().correct_mouse(cmd);

		auto anim_state = g_LocalPlayer->GetPlayerAnimState();
		if (anim_state) {
			CCSGOPlayerAnimState anim_state_backup = *anim_state;
			*anim_state = g_AnimState;
			*g_LocalPlayer->GetVAngles() = cmd->viewangles;
			g_LocalPlayer->UpdateClientSideAnimation();

			if (anim_state->speed_2d > 0.1f || std::fabsf(anim_state->flUpVelocity)) {
				next_lby = g_GlobalVars->curtime + 0.22f;
			}
			else if (g_GlobalVars->curtime > next_lby) {
				if (std::fabsf(Utils::AngleDiff(anim_state->m_flGoalFeetYaw, anim_state->m_flEyeYaw)) > 35.0f) 
					next_lby = g_GlobalVars->curtime + 1.1f;				
			}

			g_AnimState = *anim_state;
			*anim_state = anim_state_backup;
		}

		if (*send_packet) {
			real_angle = g_AnimState.m_flGoalFeetYaw;
			view_angle = g_AnimState.m_flEyeYaw;
		}
	} EnginePrediction::End();

	return false;
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::pant_traverse::hook(void* _this, int edx, vgui::VPANEL panel, bool force_repaint, bool allow_force) {
	static auto panelId = vgui::VPANEL{ 0 };

	o_paint(g_VGuiPanel, edx, panel, force_repaint, allow_force);

	if (!panelId) {
		const auto panelName = g_VGuiPanel->GetName(panel);
		if (!strcmp(panelName, "FocusOverlayPanel")) 
			panelId = panel;
	}
	else if (panelId == panel) {
		static bool bSkip = false;
		bSkip = !bSkip;

		if (bSkip) return;

		Render::Get().BeginScene();
	}
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::fsn::hook(void* _this, int edx, ClientFrameStage_t stage) {
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		o_fsn(g_CHLClient, edx, stage);

	Misc::Get().UpdateWorldTextures();
	if (stage == FRAME_RENDER_START) {
		Misc::Get().DisablePanoramaBlur();
		Misc::Get().No3dSky();
		Misc::Get().DisablePostProcessing();
	}

	o_fsn(g_CHLClient, edx, stage);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::emit_sound::hook(void* _this, int edx, IRecipientFilter& filter, int ent_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const Vector* origin, const Vector* direction, void* utl_vec_origins, bool update_positions, float sound_time, int speaker_entity, int unk) {

	if (!Cfg.misc_autoaccept)
		o_sound(g_EngineSound, edx, filter, ent_index, channel, sound_entry, sound_entry_hash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utl_vec_origins, update_positions, sound_time, speaker_entity, unk);

	if (!strcmp(sound_entry, "UIPanorama.popup_accept_match_beep")) {
		static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

		if (fnAccept) {
			fnAccept("");
			auto window = FindWindowW(L"Valve001", NULL);
			FLASHWINFO flash{ sizeof(FLASHWINFO), window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
			FlashWindowEx(&flash);
		}
	}
	o_sound(g_EngineSound, edx, filter, ent_index, channel, sound_entry, sound_entry_hash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utl_vec_origins, update_positions, sound_time, speaker_entity, unk);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::lock_cursour::hook(void* _this) {
	if (Menu::Get().IsVisible()) {
		g_VGuiSurface->UnlockCursor();
		g_InputSystem->ResetInputState();
		return;
	}
	o_lock(g_VGuiSurface);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::dme::hook(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* custom_bone_to_world) {
	if (g_MdlRender->IsForcedMaterialOverride())
		return o_dme(_this, edx, ctx, state, info, custom_bone_to_world);

	Chams::Get().OnDrawModelExecute(ctx, state, info, custom_bone_to_world);
	o_dme(_this, edx, ctx, state, info, custom_bone_to_world);
	g_MdlRender->ForcedMaterialOverride(nullptr);
}
//--------------------------------------------------------------------------------
int __fastcall Hooks::dpse::hook(void* _this, int edx, int a1) {
	if (g_LocalPlayer && Cfg.glow_enabled)
		Glow::Get().Run();

	return o_effects(g_ClientMode, edx, a1);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::override_view::hook(void* _this, int edx, CViewSetup* vs_view) {
	if (g_EngineClient->IsInGame() && vs_view) {
		if (Cfg.misc_thirdperson_bind && InputSys::Get().WasKeyPressed(Cfg.misc_thirdperson_bind))
			Cfg.misc_thirdperson = !Cfg.misc_thirdperson;

		Visuals::Get().ThirdPerson();
	}

	if (g_EngineClient->IsInGame() && !g_LocalPlayer->m_bIsScoped()) 
		vs_view->fov = Cfg.misc_override_fov;
	
	o_ovview(g_ClientMode, edx, vs_view);
}
//--------------------------------------------------------------------------------
bool __fastcall Hooks::fire_event::hook(void* ecx, void* edx, IGameEvent* event) {
	if (strstr(event->GetName(), "player_death")) {
		if (g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer()) {
			auto& weapon = g_LocalPlayer->m_hActiveWeapon();
			if (weapon) {
				auto& skin_data = Cfg.skins.m_items[weapon->m_Item().m_iItemDefinitionIndex()];
				auto& stat_track = Cfg.skins.statrack_items[weapon->m_Item().m_iItemDefinitionIndex()];
				if (skin_data.stat_trak) {
					stat_track.statrack_new.counter++;
					weapon->m_nFallbackStatTrak() = stat_track.statrack_new.counter;
					weapon->GetClientNetworkable()->PostDataUpdate(0);
					weapon->GetClientNetworkable()->OnDataChanged(0);
				}
			}

			const auto icon_override = Skins::Get().GetIconOverride(event->GetString("weapon"));
			if (icon_override) event->GetString("weapon", icon_override);
		}
	}
	if (!strcmp(event->GetName(), "game_newmap")) {
		ConVar* sv_skyname = g_CVar->FindVar("sv_skyname");
		Misc::Get().OldSkyname = sv_skyname->GetString();
		Misc::Get().NightmodeDone = false;
	}
	return o_events(ecx, edx, event);
}
//--------------------------------------------------------------------------------
auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");
typedef bool(__thiscall* svc_get_bool_t)(PVOID);
bool __fastcall Hooks::cheats::hook(PVOID convar, void* edx) {
	if (!o_cheats) return false;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think)) return true;
	return o_cheats(convar, edx);
}
//--------------------------------------------------------------------------------		
float __fastcall Hooks::aspect_ratio::hook(void* ecx, void* edx, int32_t width, int32_t height) {
	if (Cfg.aspect_ratio_value == 0) o_aspect_ratio(ecx, edx, width, height);
	else return ((float)Cfg.aspect_ratio_value / 10);

	o_aspect_ratio(ecx, edx, width, height);
}
//--------------------------------------------------------------------------------
static auto random_sequence(const int low, const int high) -> int {
	return rand() % (high - low + 1) + low;
}
//--------------------------------------------------------------------------------
static auto fix_animation(const char* model, const int sequence) -> int {
	enum ESequence {
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT01 = 12,
		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT03 = 15,
		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT01 = 12,
		SEQUENCE_FALCHION_LOOKAT02 = 13,
		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,
		SEQUENCE_BOWIE_IDLE1 = 1,
	};
	if (strstr(model, "models/weapons/v_knife_butterfly.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default: return sequence + 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_falchion_advanced.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default: return sequence - 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_push.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default: return sequence + 2;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_survival_bowie.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default: return sequence - 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_ursus.mdl") || strstr(model, "models/weapons/v_knife_canis.mdl") || strstr(model, "models/weapons/v_knife_outdoor.mdl") || strstr(model, "models/weapons/v_knife_cord.mdl") || strstr(model, "models/weapons/v_knife_skeleton.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default: return sequence + 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_widowmaker.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(14, 15);
		default: return sequence;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_stiletto.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(12, 13);
		default: return sequence;
		}
	}
	else return sequence;	
}
//--------------------------------------------------------------------------------
void Hooks::hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output) {
	static auto original_fn = Hooks::sequence_hook->get_original_function();
	const auto local = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	if (local && local->IsAlive()) {
		const auto proxy_data = const_cast<CRecvProxyData*>(pData);
		const auto view_model = static_cast<C_BaseViewModel*>(entity);
		if (view_model && view_model->m_hOwner() && view_model->m_hOwner().IsValid()) {
			const auto owner = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(view_model->m_hOwner()));
			if (owner == g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer())) {
				const auto view_model_weapon_handle = view_model->m_hWeapon();
				if (view_model_weapon_handle.IsValid()) {
					const auto view_model_weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(view_model_weapon_handle));
					if (view_model_weapon) {
						if (k_weapon_info.count(view_model_weapon->m_Item().m_iItemDefinitionIndex())) {
							auto original_sequence = proxy_data->m_Value.m_Int;
							const auto override_model = k_weapon_info.at(view_model_weapon->m_Item().m_iItemDefinitionIndex()).model;
							proxy_data->m_Value.m_Int = fix_animation(override_model, proxy_data->m_Value.m_Int);
						}
					}
				}
			}
		}
	}
	original_fn(pData, entity, output);
}
//--------------------------------------------------------------------------------
void __stdcall Hooks::suppress_list::hook(int a2, bool a3) {
	static auto OnRenderStart_Return = Utils::PatternScan(GetModuleHandleA("client.dll"), "FF 50 40 8B 1D ? ? ? ?") + 0x3;
	static auto FrameNetUpdateEnd_Return = Utils::PatternScan(GetModuleHandleA("client.dll"), "5F 5E 5D C2 04 00 83 3D ? ? ? ? ?");

	if (g_LocalPlayer && g_LocalPlayer->IsAlive()) {
		if (_ReturnAddress() == OnRenderStart_Return) {
			static auto set_abs_angles = Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
			reinterpret_cast<void(__thiscall*)(void*, const QAngle&)>(set_abs_angles)(g_LocalPlayer, QAngle(0.0f, g_AnimState.m_flGoalFeetYaw, 0.0f));
		}
		else if (_ReturnAddress() == FrameNetUpdateEnd_Return) {
			Skins::Get().OnFrameStageNotify(true);
			if (ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
				if (Cfg.misc_model) {
					if (g_LocalPlayer->m_iTeamNum() == 3)
						Misc::Get().AgentChanger(Cfg.misc_model_type_ct - 1, g_EngineClient->GetLocalPlayer());
					if (g_LocalPlayer->m_iTeamNum() == 2)
						Misc::Get().AgentChanger(Cfg.misc_model_type_tt - 1, g_EngineClient->GetLocalPlayer());
				}
			}
		}
		else if (ClientFrameStage_t::FRAME_NET_UPDATE_START) 
			Visuals::Get().NoSmoke();
	}

	o_suppress_list(g_SpatialPartition, a2, a3);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::rpe::hook(void* client_state, void* edx, void* ent_info) {
	o_rpe(client_state, ent_info);
	Skins::Get().OnFrameStageNotify(false);
}
//--------------------------------------------------------------------------------	
void Hooks::hkFlashDuration(const CRecvProxyData* data, void* m_struct, void* m_result) {
	static auto orig = Hooks::m_pFlashDuration->get_original_function();

	if (Cfg.other_no_flash) *reinterpret_cast<float*>(m_result) = 0.f;
	else orig(data, m_struct, m_result);
}
//--------------------------------------------------------------------------------