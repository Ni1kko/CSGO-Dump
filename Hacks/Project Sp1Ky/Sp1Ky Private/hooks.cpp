#include "hooks.hpp"
#include <intrin.h>  

#include "menu.hpp"
#include "options.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "features/bhop.hpp"
#include "features/chams.hpp"
#include "features/visuals.hpp"
#include "skins.h"
#include "aimbot.h"
#include "backtrack.h"
#include "Hitmarker.h"
#include "BulletBeams.h"
#include "./PredictionSystem.hpp"
#include <assert.h>
#include <crtdbg.h>
#include "../features/XorCompileTime.hpp"
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
#pragma intrinsic(_ReturnAddress) 

#define PI 3.14159265358979323846
#define min(a,b)            (((a) < (b)) ? (a) : (b))
class Vector;

namespace Global
{
	float hitmarkerAlpha = 0.f;

	float smt = 0.f;
	QAngle FakeAngle = QAngle(0.f,0.f,0.f);
	QAngle RealAngle = QAngle(0.f, 0.f, 0.f);
	bool bSendPacket = false;
	bool bShouldChoke = false;
	bool bAimbotting = false;
	bool bVisualAimbotting = false;
	QAngle vecVisualAimbotAngs = QAngle(0.f, 0.f, 0.f);
	Vector vecUnpredictedVel = Vector(0, 0, 0);
	CUserCmd *userCMD = nullptr;

	int nChockedTicks = 0;
}

template<class T, class U>
T Clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

namespace Hooks
{
	vfunc_hook hlclient_hook;
	vfunc_hook direct3d_hook;
	vfunc_hook vguipanel_hook;
	vfunc_hook vguisurf_hook;
	vfunc_hook mdlrender_hook;
	vfunc_hook clientmode_hook;
	vfunc_hook viewrender_hook;
	vfunc_hook sv_cheats;
	vfunc_hook gameevents_hook; recv_prop_hook* sequence_hook;
	typedef void(__thiscall* LockCursor)(void*);
	LockCursor oLockCursor;

	void __stdcall hkLockCursor()
	{
		oLockCursor = Hooks::vguisurf_hook.get_original<LockCursor>(67);

		if (Menu::Get().IsVisible())
		{
			g_VGuiSurface->UnlockCursor();
			return;
		}

		oLockCursor(g_VGuiSurface);
	}

	void Initialize()
	{
		hlclient_hook.setup(g_CHLClient, Utils::IsPanorama() ? "client_panorama.dll" : "client.dll");
		direct3d_hook.setup(g_D3DDevice9,"shaderapidx9.dll");
		vguipanel_hook.setup(g_VGuiPanel);
		vguisurf_hook.setup(g_VGuiSurface);
		viewrender_hook.setup(g_RenderView);
		mdlrender_hook.setup(g_MdlRender,"engine.dll");
		clientmode_hook.setup(g_ClientMode, Utils::IsPanorama() ? "client_panorama.dll" : "client.dll");
		ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
		sv_cheats.setup(sv_cheats_con);
		gameevents_hook.setup(g_GameEvents);
		gameevents_hook.hook_index(index::FireEvent, hkFireEvent);
		sequence_hook = new recv_prop_hook(C_BaseViewModel::m_nSequence(), hkRecvProxy);
		
		direct3d_hook.hook_index(index::EndScene, hkEndScene);
		direct3d_hook.hook_index(index::Reset, hkReset);

		hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
		hlclient_hook.hook_index(index::CreateMove, hkCreateMove_Proxy);
		
		vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);

		vguisurf_hook.hook_index(index::PlaySound, hkPlaySound);

		viewrender_hook.hook_index(index::SceneEnd, hkSceneEnd);

		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);

		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);

		sv_cheats.hook_index(index::SvCheatsGetBool, hkSvCheatsGetBool);

		vguisurf_hook.hook_index(67, hkLockCursor);

		Visuals::CreateFonts();
		std::cout << XorStr("Done!") << std::endl;
	}
	//--------------------------------------------------------------------------------
	void Shutdown()
	{
		hlclient_hook.unhook_all();
		direct3d_hook.unhook_all();
		vguipanel_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		clientmode_hook.unhook_all();
		gameevents_hook.unhook_all();
		sequence_hook->~recv_prop_hook();

		Visuals::DestroyFonts();
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkEndScene(IDirect3DDevice9* device)
	{
		auto oEndScene = direct3d_hook.get_original<EndScene>(index::EndScene);

		static auto viewmodel_fov = g_CVar->FindVar(XorStr("viewmodel_fov"));
		static auto crosshair_cvar = g_CVar->FindVar(XorStr("crosshair"));

		viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
		viewmodel_fov->SetValue(g_Options.viewmodel_fov);
		crosshair_cvar->SetValue(!g_Options.legit_self_cros);

		static auto wanted_ret_address = _ReturnAddress();
		if (_ReturnAddress() == wanted_ret_address)
		{
			Menu::Get().Render();
		}

		return oEndScene(device);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		auto oReset = direct3d_hook.get_original<Reset>(index::Reset);

		Visuals::DestroyFonts();
		Menu::Get().OnDeviceLost();

		auto hr = oReset(device, pPresentationParameters);

		if (hr >= 0) {
			Menu::Get().OnDeviceReset();
			Visuals::CreateFonts();
		}



		return hr;
	}

	CMoveData bMoveData[0x200];
	void Prediction(CUserCmd* pCmd, C_BasePlayer* LocalPlayer)
	{
		if (g_MoveHelper && LocalPlayer->IsAlive())
		{
			float curtime = g_GlobalVars->curtime;
			float frametime = g_GlobalVars->frametime;
			int iFlags = LocalPlayer->m_fFlags();

			g_GlobalVars->curtime = (float)LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;
			g_GlobalVars->frametime = g_GlobalVars->interval_per_tick;

			g_MoveHelper->SetHost(LocalPlayer);

			g_Prediction->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
			g_GameMovement->ProcessMovement(LocalPlayer, bMoveData);
			g_Prediction->FinishMove(LocalPlayer, pCmd, bMoveData);

			g_MoveHelper->SetHost(0);

			g_GlobalVars->curtime = curtime;
			g_GlobalVars->frametime = frametime;
			LocalPlayer->m_fFlags() = iFlags;
		}
	}

	float RandomFloat(float min, float max)
	{
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * (max - min);
	}
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
	{
		auto oCreateMove = hlclient_hook.get_original<CreateMove>(index::CreateMove);

		oCreateMove(g_CHLClient, sequence_number, input_sample_frametime, active);

		auto cmd = g_Input->GetUserCmd(sequence_number);
		auto verified = g_Input->GetVerifiedCmd(sequence_number);

		if (!cmd || !cmd->command_number)
			return;

		QAngle oldPunch;

		auto pLocal2 = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
		
		if (g_Options.total_total_l_m_exi__kero)
			TimeWarp::Get().CreateMove(cmd);

		if (g_Options.more_more_mi_b_hop) {
			BunnyHop::OnCreateMove(cmd);
		}

		if (g_Options.more_more_mi_a_str_ejf)
		{
			BunnyHop::AutoStrafe(cmd, cmd->viewangles);
		}

		PredictionSystem::Get().Start(cmd, g_LocalPlayer);
		{
			AimLegit::Get().Work(cmd);
		}
		PredictionSystem::Get().End(g_LocalPlayer);

		if (Global::bSendPacket)
			Global::FakeAngle = cmd->viewangles;
		if (!Global::bSendPacket)
			Global::RealAngle = cmd->viewangles;

		if (Global::bShouldChoke)
			Global::bSendPacket = Global::bShouldChoke = false;

		if (!Global::bSendPacket)
			Global::nChockedTicks++;
		else
			Global::nChockedTicks = 0;


		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}
	//--------------------------------------------------------------------------------
	__declspec(naked) void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active)
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::hkCreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>(index::PaintTraverse);
		oPaintTraverse(g_VGuiPanel, panel, forceRepaint, allowForce);
		if (!panelId) {
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, XorStr("FocusOverlayPanel"))) {
				panelId = panel;
			}
		}
		else if (panelId == panel) {
			if (g_EngineClient->IsInGame() && !g_EngineClient->IsTakingScreenshot())
				g_InputSystem->EnableInput(!Menu::Get().IsVisible()); 
			{

				if (!g_LocalPlayer)
					return;

				if (g_Options.legit_enabled_esp) {
					for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
						auto entity = C_BasePlayer::GetPlayerByIndex(i);

						if (!entity)
							continue;

						if (entity == g_LocalPlayer)
							continue;

						if (i < 65 && !entity->IsDormant() && entity->IsAlive()) {
							if (Visuals::Player::Begin(entity)) {
								if (g_Options.esp_player_snaplines) Visuals::Player::RenderSnapline();
								if (g_Options.legit_box_toggle)     Visuals::Player::RenderBox();
//								if (g_Options.legit_playa_weap)   Visuals::Player::RenderWeapon();
								if (g_Options.legit_playa_name)     Visuals::Player::RenderName();
								if (g_Options.legit_playa_heal)    Visuals::Player::RenderHealth();
								if (g_Options.legit_playa_arm)    Visuals::Player::RenderArmour();
								if (g_Options.more_more_more_bektrkr_ske__Lt)  Visuals::Player::BacktrackSkelt();
							}
						}
						else if (g_Options.legit_playa_dro_wep && entity->IsWeapon()) {
							Visuals::Misc::RenderWeapon((C_BaseCombatWeapon*)entity);
						}
						else if (g_Options.legit_playa_def && entity->IsDefuseKit()) {
							Visuals::Misc::RenderDefuseKit(entity);
						}
						else if (entity->IsPlantedC4()) {
							if (g_Options.legit_dog_bom)
								Visuals::Misc::RenderPlantedC4(entity);
						}
					}
				}

				if (g_Options.legit_self_cros)
					Visuals::Misc::RenderCrosshair();

				if (g_Options.more_more_mi_hejt_merkr)
				{
					HitMarkerEvent::Get().Paint();
				}

				if (g_Options.nightmode)
					Visuals::NightMode();

				if (g_Options.watermark)
					Visuals::DrawWatermark();

				if (g_Options.more_more_spe_tcetrs)
					Visuals::Spectators();

				if (g_Options.misc_thirdperson)
				{
					if (GetAsyncKeyState(VK_MBUTTON))
					{
						if (!g_Input->m_fCameraInThirdPerson)
						{
							g_Input->m_fCameraInThirdPerson = true;
						}
					}
					if (GetAsyncKeyState(VK_LMENU))
					{
						g_Input->m_fCameraInThirdPerson = false;
					}
				}
			}
		}
	}


	//--------------------------------------------------------------------------------
	void __stdcall hkPlaySound(const char* name)
	{
		static auto oPlaySound = vguisurf_hook.get_original<PlaySound>(index::PlaySound);

		oPlaySound(g_VGuiSurface, name);

		if (strstr(name, XorStr("UI/competitive_accept_beep.wav"))) {
			static auto fnAccept =
				(void(*)())Utils::PatternScan(GetModuleHandleA(Utils::IsPanorama() ? "client_panorama.dll" : "client.dll"), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");

			fnAccept();

			FLASHWINFO fi;
			fi.cbSize = sizeof(FLASHWINFO);
			fi.hwnd = InputSys::Get().GetMainWindow();
			fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			fi.uCount = 0;
			fi.dwTimeout = 0;
			FlashWindowEx(&fi);
		}
	}
	//--------------------------------------------------------------------------------
	int __stdcall hkDoPostScreenEffects(int a1)
	{
		auto oDoPostScreenEffects = clientmode_hook.get_original<DoPostScreenEffects>(index::DoPostScreenSpaceEffects);
		BulletBeamsEvent::Get().Paint();
		AimLegit::Get().Clantag();
		AimLegit::Get().ChatSpamer();

		return oDoPostScreenEffects(g_ClientMode, a1);
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
	{
		static auto ofunc = hlclient_hook.get_original<FrameStageNotify>(index::FrameStageNotify);
		Skins::OnFrameStageNotify(stage);
		ofunc(g_CHLClient, stage);
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkOverrideView(CViewSetup* vsView)
	{
		static auto ofunc = clientmode_hook.get_original<OverrideView>(index::OverrideView);

		if (g_EngineClient->IsInGame() && vsView) {
		}

		ofunc(g_ClientMode, vsView);
	}
	void __fastcall hkSceneEnd(void * thisptr, void * edx)
	{
		static auto ofunc = viewrender_hook.get_original<SceneEnd>(index::SceneEnd);

		if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
			return ofunc(thisptr);

		if (g_LocalPlayer && g_LocalPlayer->IsAlive())
		{
			if (g_Options.misc_thirdperson)
			{
				if (g_Options.fake_angle_chams) {
					C_BasePlayer* pLocal = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
					if (pLocal)
					{
						IMaterial* mat = g_MatSystem->FindMaterial(XorStr("simple_regular_reflective"), TEXTURE_GROUP_MODEL);
						if (mat)
						{
							QAngle OrigAng;
							OrigAng = g_LocalPlayer->m_angEyeAngles();
							pLocal->SetAngle2(QAngle(0.f, Global::FakeAngle.yaw, 0.f));
							g_RenderView->SetColorModulation(0.87f, 1.00f, 0.00f);
							g_MdlRender->ForcedMaterialOverride(mat);
							g_LocalPlayer->DrawModel(0x00000001, 255);
							g_MdlRender->ForcedMaterialOverride(nullptr);
							pLocal->SetAngle2(OrigAng);
						}
					}
				}
			}
		}

		ofunc(thisptr);

	} 
	//--------------------------------------------------------------------------------
	void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ofunc = mdlrender_hook.get_original<DrawModelExecute>(index::DrawModelExecute);

		Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

		ofunc(g_MdlRender, ctx, state, pInfo, pCustomBoneToWorld);

		g_MdlRender->ForcedMaterialOverride(nullptr);
	}

	auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(Utils::IsPanorama() ? L"client_panorama.dll" : L"client.dll"), "85 C0 75 30 38 86");
	typedef bool(__thiscall *svc_get_bool_t)(PVOID);
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
	{
		static auto ofunc = sv_cheats.get_original<svc_get_bool_t>(13);
		if (!ofunc)
			return false;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
			return true;
		return ofunc(pConVar);
	}

	bool __stdcall hkFireEvent(IGameEvent* pEvent)
	{
		static auto oFireEvent = gameevents_hook.get_original<FireEvent>(index::FireEvent);
		if (!strcmp(pEvent->GetName(), XorStr("player_death")) && g_EngineClient->GetPlayerForUserID(pEvent->GetInt(XorStr("attacker"))) == g_EngineClient->GetLocalPlayer()) {
			auto& weapon = g_LocalPlayer->m_hActiveWeapon();
			if (weapon && weapon->IsWeapon()) {
				auto& skin_data = g_Options.skins.m_items[weapon->m_Item().m_iItemDefinitionIndex()];
				if (skin_data.enabled && skin_data.stat_trak) {
					skin_data.stat_trak++;
					weapon->m_nFallbackStatTrak() = skin_data.stat_trak;
					weapon->GetClientNetworkable()->PostDataUpdate(0);
					weapon->GetClientNetworkable()->OnDataChanged(0);
				}
			}
			const auto icon_override = g_Options.skins.get_icon_override(pEvent->GetString(XorStr("weapon")));
			if (icon_override) {
				pEvent->SetString(XorStr("weapon"), icon_override);
			}
		}
		return oFireEvent(g_GameEvents, pEvent);
	}
	//--------------------------------------------------------------------------------
	static auto random_sequence(const int low, const int high) -> int
	{
		return rand() % (high - low + 1) + low;
	}
	static auto fix_animation(const char* model, const int sequence) -> int
	{
		enum ESequence
		{
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
		if (strstr(model, XorStr("models/weapons/v_knife_butterfly.mdl"))) {
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
				return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
			default:
				return sequence + 1;
			}
		}
		else if (strstr(model, XorStr("models/weapons/v_knife_falchion_advanced.mdl"))) {
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_FALCHION_IDLE1;
			case SEQUENCE_DEFAULT_HEAVY_MISS1:
				return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			default:
				return sequence - 1;
			}
		}
		else if (strstr(model, XorStr("models/weapons/v_knife_push.mdl"))) {
			switch (sequence)
			{
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
			default:
				return sequence + 2;
			}
		}
		else if (!strcmp(model, XorStr("models/weapons/v_knife_ursus.mdl")))
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
				return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
			case SEQUENCE_DEFAULT_LOOKAT01:
				return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
			default:
				return sequence + 1;
			}
		}
		else if (!strcmp(model, XorStr("models/weapons/v_knife_stiletto.mdl")))
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_LOOKAT01:
				return random_sequence(12, 13);
				break;
			}
		}
		else if (!strcmp(model, XorStr("models/weapons/v_knife_widowmaker.mdl")))
		{
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_LOOKAT01:
				return random_sequence(14, 15);
				break;
			}
		}
		else if (strstr(model, XorStr("models/weapons/v_knife_survival_bowie.mdl"))) {
			switch (sequence)
			{
			case SEQUENCE_DEFAULT_DRAW:
			case SEQUENCE_DEFAULT_IDLE1:
				return sequence;
			case SEQUENCE_DEFAULT_IDLE2:
				return SEQUENCE_BOWIE_IDLE1;
			default:
				return sequence - 1;
			}
		}
		else {
			return sequence;
		}
	}
	void hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output)
	{
		static auto original_fn = sequence_hook->get_original_function();
		const auto local = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
		if (local && local->IsAlive())
		{
			const auto proxy_data = const_cast<CRecvProxyData*>(pData);
			const auto view_model = static_cast<C_BaseViewModel*>(entity);
			if (view_model && view_model->m_hOwner() && view_model->m_hOwner().IsValid())
			{
				const auto owner = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(view_model->m_hOwner()));
				if (owner == g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()))
				{
					const auto view_model_weapon_handle = view_model->m_hWeapon();
					if (view_model_weapon_handle.IsValid())
					{
						const auto view_model_weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(view_model_weapon_handle));
						if (view_model_weapon)
						{
							if (k_weapon_info.count(view_model_weapon->m_Item().m_iItemDefinitionIndex()))
							{
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
}



























































































































































































































































































































