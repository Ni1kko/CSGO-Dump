#include "hooks.hpp"
#include "..//helpers/prediction.hpp"
#include <filesystem>

static int max_choke_ticks = 16;
bool fakefpscheck = false;

void patch(PVOID address, const int type, const int bytes)
{
	DWORD d, ds; //declared for future use.
	VirtualProtect(address, bytes, PAGE_EXECUTE_READWRITE, &d); //remove write protection!
	memset(address, type, bytes); //patch the data
	VirtualProtect(address, bytes, d, &ds); //set the write protection back to its normal state
}

void FixMovement(CUserCmd* cmd, QAngle& wishangle)
{
	Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	Math::AngleVectors(wishangle, view_fwd, view_right, view_up);
	Math::AngleVectors(cmd->viewangles, cmd_fwd, cmd_right, cmd_up);

	const auto v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	const auto v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	const auto v12 = sqrtf(view_up.z * view_up.z);

	const Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	const Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	const Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	const auto v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
	const auto v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
	const auto v18 = sqrtf(cmd_up.z * cmd_up.z);

	const Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
	const Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
	const Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

	const auto v22 = norm_view_fwd.x * cmd->forwardmove;
	const auto v26 = norm_view_fwd.y * cmd->forwardmove;
	const auto v28 = norm_view_fwd.z * cmd->forwardmove;
	const auto v24 = norm_view_right.x * cmd->sidemove;
	const auto v23 = norm_view_right.y * cmd->sidemove;
	const auto v25 = norm_view_right.z * cmd->sidemove;
	const auto v30 = norm_view_up.x * cmd->upmove;
	const auto v27 = norm_view_up.z * cmd->upmove;
	const auto v29 = norm_view_up.y * cmd->upmove;

	cmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
		+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
		+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
	cmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
		+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
		+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
	cmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
		+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
		+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

	const auto ratio = 2.f - fmaxf(fabsf(cmd->sidemove), fabsf(cmd->forwardmove)) / 450.f;
	cmd->forwardmove *= ratio;
	cmd->sidemove *= ratio;

	wishangle = cmd->viewangles;
}
bool __fastcall Hooks::CreateMove(void* _this, int, float input_sample_frametime, CUserCmd* cmd)
{
	static auto ofunc = clientmode_hook.get_original<decltype(&CreateMove)>(index::CreateMove);
	bool result = ofunc(g_ClientMode, 0, input_sample_frametime, cmd);

	bool& bSendPacket = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	if (!cmd || !cmd->command_number || !g_EngineClient->IsInGame())
		return result;

	if (g_Options.misc.fakefps)
	{
		if (!fakefpscheck)
		{
			g_EngineClient->ClientCmd("fps_max 60");
			fakefpscheck = true;
		}
		g_GlobalVars->absoluteframetime = 1.f / 1489.f;
	}
	else
	{
		if (fakefpscheck)
		{
			g_EngineClient->ClientCmd("fps_max 999");
			fakefpscheck = false;
		}
	}

	if (!g_LocalPlayer)
		return result;

	static auto prediction = new PredictionSystem();

	if (g_GameRules->m_bIsValveDS())
	{
			max_choke_ticks = 7;
	}
	else
	{
		max_choke_ticks = 16;
	}

	static std::once_flag fl;
	std::call_once(fl, []()
	{
		static auto fakelagboost = reinterpret_cast<int*> (Utils::PatternScan("engine.dll", "0F 4F F0 89 5D FC") - 0x6);
		patch(static_cast<PVOID> (fakelagboost), 17, 1);
	});

	*reinterpret_cast<int*>((DWORD)& g_CVar->FindVar("viewmodel_offset_x")->m_fnChangeCallbacks + 0xC) = 0;
	*reinterpret_cast<int*>((DWORD)& g_CVar->FindVar("viewmodel_offset_y")->m_fnChangeCallbacks + 0xC) = 0;
	*reinterpret_cast<int*>((DWORD)& g_CVar->FindVar("viewmodel_offset_z")->m_fnChangeCallbacks + 0xC) = 0;
	g_CVar->FindVar("viewmodel_offset_x")->SetValue(g_Options.misc.fov.viewmodel_offset_x);
	g_CVar->FindVar("viewmodel_offset_y")->SetValue(g_Options.misc.fov.viewmodel_offset_y);
	g_CVar->FindVar("viewmodel_offset_z")->SetValue(g_Options.misc.fov.viewmodel_offset_z);

	unpredvelocity = g_LocalPlayer->m_vecVelocity();
	unpredangle = cmd->viewangles;

	g_Misc->BunnyHop(cmd);

	auto flags = g_LocalPlayer->m_fFlags();

	prediction->StartPrediction(cmd);

	bSendPacket = cmd->command_number % 2;

	g_Misc->Fakelag(cmd, bSendPacket);

	g_Legitbot->Run(cmd);

	g_Ragebot->Run(cmd, bSendPacket);

	prediction->EndPrediction();

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND) && (flags & FL_ONGROUND) && g_Options.misc.edgejump.enabled && (!g_Options.misc.edgejump.onkey || g_InputSys->IsKeyDown(g_Options.misc.edgejump.hotkey)))
		cmd->buttons |= IN_JUMP;

	if (cmd->buttons & IN_SCORE)
		Utils::RankRevealAll();

	Math::FixAngles(cmd->viewangles);
	FixMovement(cmd, unpredangle);
	std::clamp(cmd->sidemove, -450.f, 450.f);
	std::clamp(cmd->forwardmove, -450.f, 450.f);
	std::clamp(cmd->upmove, -320.f, 320.f);

	if (g_ClientState->chokedcommands >= max_choke_ticks)
		bSendPacket = true;

	if (bSendPacket && g_LocalPlayer->IsAlive() && g_LocalPlayer->GetPlayerAnimState() != nullptr)
	{
		tpangle = cmd->viewangles;
	}

	if (g_Options.misc.sniper_xhair && !g_LocalPlayer->m_bIsScoped())
		g_CVar->FindVar("weapon_debug_spread_show")->SetValue(3);
	else
		g_CVar->FindVar("weapon_debug_spread_show")->SetValue(0);

	return false;
}