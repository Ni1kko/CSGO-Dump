#include "Hooks.h"
#include "Bunnyhop.h"
#include "Legitbot.h"
#include "Ragebot.h"
#include "checksum_md5.h"
#include "Resolver.h"
#include "Knifebot.h"
#include <intrin.h>
#include "EnginePrediction.h"
#include "NetChannelhook.h"
#include "AntiAims.h"

static bool in_cm = false;

float last_time_got_impact = 0;

float IBasePlayer::GetOldSimulationTime()
{
	static int m_flSimulationTime = netvars.GetOffset(hs::DT_BaseEntity::s().c_str(), hs::m_flSimulationTime::s().c_str()) + 4;
	return *(float*)((DWORD)this + m_flSimulationTime);
}

void UpdateSimulationTime() {
	for (int i = 1; i < 65; i++)
	{
		auto ent = interfaces.ent_list->GetClientEntity(i);
		if (!ent)
			continue;
		if (ent->IsDormant() || !ent->isAlive() || !ent->IsPlayer())
			continue;
		if (csgo->old_simtime[ent->GetIndex()] != ent->GetSimulationTime())
			csgo->old_simtime[ent->GetIndex()] = ent->GetSimulationTime();
	}
}

void DoClantag()
{
	auto SetClanTag = [](const char* tag, const char* name)
	{
		static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>((DWORD)(csgo->Utils.FindPatternIDA(GetModuleHandleA(
			g_Modules[fnv::hash(hs::engine_dll::s().c_str())].c_str()),
			hs::set_clantag::s().c_str())));
		pSetClanTag(tag, name);
	};

	auto Marquee = [](std::string& clantag)
	{
		std::string temp = clantag;
		clantag.erase(0, 1);
		clantag += temp[0];
	};

	static bool Reset = false;

	if (vars.visuals.clantagspammer)
	{
		Reset = false;
		static float oldTime;
		float flCurTime = csgo->local->GetTickBase() * interfaces.global_vars->interval_per_tick;
		static float flNextTimeUpdate = 0;
		static int iLastTime;

		float latency = interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
		if (int(interfaces.global_vars->curtime * 2 + latency) != iLastTime)
		{
			if (flNextTimeUpdate <= flCurTime || flNextTimeUpdate - flCurTime > 1.f)
			{
				switch (int(interfaces.global_vars->curtime * 2.4) % 10) {
				case 0: SetClanTag(hs::clantag1::s().c_str(), hs::weave_su::s().c_str()); break;
				case 1: SetClanTag(hs::clantag2::s().c_str(), hs::weave_su::s().c_str()); break;
				case 2: SetClanTag(hs::clantag3::s().c_str(), hs::weave_su::s().c_str()); break;
				case 3: SetClanTag(hs::clantag4::s().c_str(), hs::weave_su::s().c_str()); break;
				case 4: SetClanTag(hs::clantag5::s().c_str(), hs::weave_su::s().c_str()); break;
				case 5: SetClanTag(hs::clantag6::s().c_str(), hs::weave_su::s().c_str()); break;
				case 6: SetClanTag(hs::clantag7::s().c_str(), hs::weave_su::s().c_str()); break;
				case 7: SetClanTag(hs::clantag8::s().c_str(), hs::weave_su::s().c_str()); break;
				case 8: SetClanTag(hs::clantag9::s().c_str(), hs::weave_su::s().c_str()); break;
				case 9: SetClanTag(hs::clantag10::s().c_str(), hs::weave_su::s().c_str()); break;
				}
			}
			iLastTime = int(interfaces.global_vars->curtime * 2 + latency);
		}
	}
	else
	{
		//if (!Reset)
		{
			static int iLastTime;

			float latency = interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
			if (int(interfaces.global_vars->curtime * 2 + latency) != iLastTime)
			{
				SetClanTag("", "");
				iLastTime = int(interfaces.global_vars->curtime * 2 + latency);
			}
			Reset = true;
		}
	}
}

namespace MoveFixer
{
	void Run(Vector original)
	{
		Vector real_viewangles;
		interfaces.engine->GetViewAngles(real_viewangles);

		Vector vecMove(csgo->cmd->forwardmove, csgo->cmd->sidemove, csgo->cmd->upmove);
		float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

		Vector angMove;
		Math::VectorAngles(vecMove, angMove);

		float yaw = DEG2RAD(csgo->cmd->viewangles.y - original.y + angMove.y);

		csgo->cmd->forwardmove = cos(yaw) * speed;
		csgo->cmd->sidemove = sin(yaw) * speed;

		csgo->cmd->buttons &= ~IN_RIGHT;
		csgo->cmd->buttons &= ~IN_MOVERIGHT;
		csgo->cmd->buttons &= ~IN_LEFT;
		csgo->cmd->buttons &= ~IN_MOVELEFT;
		csgo->cmd->buttons &= ~IN_FORWARD;
		csgo->cmd->buttons &= ~IN_BACK;

		if (csgo->cmd->forwardmove > 0.f)
			csgo->cmd->buttons |= IN_FORWARD;
		else if (csgo->cmd->forwardmove < 0.f)
			csgo->cmd->buttons |= IN_BACK;

		if (csgo->cmd->sidemove > 0.f)
		{
			csgo->cmd->buttons |= IN_RIGHT;
			csgo->cmd->buttons |= IN_MOVERIGHT;
		}
		else if (csgo->cmd->sidemove < 0.f)
		{
			csgo->cmd->buttons |= IN_LEFT;
			csgo->cmd->buttons |= IN_MOVELEFT;
		}
	}
}

void ProcessMissedShots()
{
	if (shot_snapshots.size() == 0)
		return;
	auto& snapshot = shot_snapshots.front();
	if (fabs((csgo->local->GetTickBase() * interfaces.global_vars->interval_per_tick) - snapshot.time) > 1.f)
	{
		if (snapshot.weapon_fire) {
			if (snapshot.bullet_impact)
				Msg("Can't proceed shot", color_t(255, 0, 0));
			else
				Msg("Can't proceed impact", color_t(255, 0, 0));
		}
		shot_snapshots.erase(shot_snapshots.begin());
		return;
	}

	if (snapshot.first_processed_time != -1.f) {
		if (snapshot.damage == -1 && snapshot.weapon_fire && snapshot.bullet_impact && snapshot.record->player) {
			string msg;
			msg += "Missed shot to " + snapshot.entity->GetName();
			msg += snapshot.get_info();
			bool spread = false;
			if (snapshot.record->player) {
				const auto studio_model = interfaces.models.model_info->GetStudioModel(snapshot.record->player->GetModel());

				if (studio_model)
				{
					const auto angle = Math::CalculateAngle(snapshot.start, snapshot.impact);
					Vector forward;
					Math::AngleVectors(angle, forward);
					const auto end = snapshot.impact + forward * 2000.f;
					if (!CanHitHitbox(snapshot.start, end, snapshot.record, studio_model, snapshot.hitbox))
						spread = true;
				}
			}
			if (spread)
				msg += " [SPREAD] ";
			else {
				csgo->missedshots[snapshot.entity->GetIndex()]++;
			}
			Msg(msg, color_t(255, 0, 0));
			shot_snapshots.erase(shot_snapshots.begin());
		}
	}
}

void FakeDuck(bool& send_packet)
{
	if (csgo->cmd->buttons & IN_JUMP || !(csgo->local->GetFlags() & FL_ONGROUND) || !vars.antiaim.enable) {
		csgo->fake_duck = false;
		return;
	}
	csgo->cmd->buttons |= IN_BULLRUSH;

	int choke =/* csgo->game_rules->IsValveDS() ? 6 :*/ 14;

	if (vars.antiaim.fakeduck->active)
	{
		csgo->fake_duck = true;

		if (csgo->client_state->iChokedCommands <= choke / 2)
		{
			csgo->cmd->buttons &= ~IN_DUCK;

			if (csgo->client_state->iChokedCommands > (choke / 3) + 1)
				csgo->stand = true; // ������� � ����� ����� �� ������ �� ����� 
			else
				csgo->stand = false;
		}
		else
		{
			csgo->cmd->buttons |= IN_DUCK;
			csgo->stand = false;
		}

		if (csgo->client_state->iChokedCommands < choke)
			send_packet = false;								// choke
		else
			send_packet = true;
	}
	else {
		csgo->fake_duck = false;
		csgo->stand = false;
	}
}



void FixRevolver()
{
	if (!csgo->weapon)
		return;

	if (csgo->weapon->GetItemDefinitionIndex() == 64 && csgo->weapon_struct.work && csgo->weapon->NextSecondaryAttack() == FLT_MAX)
		csgo->weapon->NextSecondaryAttack() = csgo->weapon_struct.next_attack;
}
float GetCurTime(CUserCmd* ucmd) {
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = csgo->local->GetTickBase();
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * interfaces.global_vars->interval_per_tick;
	return curtime;
}
void __fastcall Hooked_RunCommand(void* ecx, void* edx, IBasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper)
{
	static auto RunCommand = g_pPredictHook->GetOriginal< RunCommandFn >(19);

	if (csgo->DoUnload || player == nullptr || csgo->local == nullptr || !csgo->local->isAlive())
		return RunCommand(ecx, player, ucmd, moveHelper);

	if (interfaces.engine->IsConnected() && interfaces.engine->IsInGame()) {
		if (player == csgo->local) {
			if (1.f / interfaces.global_vars->interval_per_tick + csgo->unpdred_tick + 8 <= ucmd->tick_count)
			{
				ucmd->hasbeenpredicted = true;
				return;
			}
			//int ticksallowed = max(0, player->m_iTicksAllowedForProcessing - 1);
			//player->m_iTicksAllowedForProcessing = ticksallowed;

			//if (ticksallowed < 1)
			//{
			//	return;
			//}
			float m_flVelModBackup = csgo->local->GetVelocityModifier();

			int m_nTickbase = csgo->local->GetTickBase();
			int m_flCurtime = interfaces.global_vars->curtime;

			if (ucmd->command_number == csgo->shot_cmd) {
				csgo->local->GetTickBasePtr() = CMAntiAim::Get().base_tick - csgo->m_nTickbaseShift + 1;
				interfaces.global_vars->curtime = TICKS_TO_TIME(csgo->local->GetTickBasePtr());
			}

			//WriteAttackPacketValue(ucmd, true);

			if (csgo->g_bOverrideVelMod && ucmd->command_number == csgo->client_state->nLastCommandAck + 1)
				csgo->local->GetVelocityModifier() = csgo->g_flVelMod;

			//FixRevolver();

			RunCommand(ecx, player, ucmd, moveHelper);

			//FixRevolver();

			if (ucmd->command_number == csgo->shot_cmd)
			{
				csgo->local->GetTickBasePtr() = m_nTickbase;
				interfaces.global_vars->curtime = m_flCurtime;
			}

			if (!csgo->g_bOverrideVelMod)
				csgo->local->GetVelocityModifier() = m_flVelModBackup;

			//WriteAttackPacketValue(ucmd, false);
		}
	}
	else
		RunCommand(ecx, player, ucmd, moveHelper);
}

bool __fastcall Hooked_InPrediction(CPrediction* prediction, uint32_t)
{
	static auto InPrediction = g_pPredictHook->GetOriginal< InPredictionFn >(14);

	if (csgo->DoUnload)
		return InPrediction(prediction);

	static const auto return_to_maintain_sequence_transitions = csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::seq_transition::s().c_str());

	if (_ReturnAddress() == (uint32_t*)return_to_maintain_sequence_transitions)
		return false;

	return InPrediction(prediction);
}

void __fastcall Hooked_SetupMove(void* ecx, void* edx, IBasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper, void* pMoveData)
{
	using Fn = void(__thiscall*)(void*, IBasePlayer*, CUserCmd*, IMoveHelper*, void*);

	static auto SetupMove = g_pPredictHook->GetOriginal< Fn >(20);

	//if (&Prediction::Get().move_data != pMoveData && pMoveData != nullptr)
	//	memcpy(&Prediction::Get().move_data, pMoveData, 0x564);

	SetupMove(ecx, player, ucmd, moveHelper, pMoveData);
}

void chokelimit()
{
	static auto clMoveChokeClamp = csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::engine_dll::s().c_str())].c_str()),
		hs::choke_limit::s().c_str()) + 1;
	unsigned long protect = 0;
	VirtualProtect((void*)clMoveChokeClamp, 4, PAGE_EXECUTE_READWRITE, &protect);
	*(std::uint32_t*)clMoveChokeClamp = 62;
	VirtualProtect((void*)clMoveChokeClamp, 4, protect, &protect);
}

void WriteUsercmd(bf_write* buf, CUserCmd* in, CUserCmd* out)
{
	static DWORD WriteUsercmdF = (DWORD)csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::write_user_cmd::s().c_str());

	__asm
	{
		mov ecx, buf
		mov edx, in
		push out
		call WriteUsercmdF
		add esp, 4
	}
}

void __cdecl  Hooked_ClMove(float accumulated_extra_samples, bool bFinalTick)
{
	H::ClMove(accumulated_extra_samples, bFinalTick);
}

bool __fastcall Hooked_WriteUsercmdDeltaToBuffer(void* ecx, void*, int slot, bf_write* buf, int from, int to, bool isnewcommand)
{
	static auto ofunct = g_pClientHook->GetOriginal <WriteUsercmdDeltaToBufferFn>(24);

	static auto retn = csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::engine_dll::s().c_str())].c_str()),
		hs::write_user_cmd_retn::s().c_str());

	if ((_ReturnAddress()) != retn || csgo->DoUnload)
		return ofunct(ecx, slot, buf, from, to, isnewcommand);

	if (interfaces.engine->IsConnected() && interfaces.engine->IsInGame()) {
		if (csgo->game_rules->IsFreezeTime())
			return ofunct(ecx, slot, buf, from, to, isnewcommand);

		//if (vars.ragebot.enable && vars.ragebot.double_tap)
		//{
		//	if (csgo->local && csgo->local->isAlive() && CanDT() && !csgo->game_rules->IsFreezeTime()) {
		//		if (csgo->skip_ticks > 0 && !csgo->dt_charged) {
		//			csgo->skip_ticks--;
		//			if (csgo->skip_ticks == 0)
		//				csgo->dt_charged = true;
		//			return false;
		//		}
		//	}
		//}

		if (csgo->m_nTickbaseShift <= 0 || csgo->client_state->iChokedCommands > 3)
			return ofunct(ecx, slot, buf, from, to, isnewcommand);

		if (from != -1)
			return true;
		uintptr_t stackbase;
		__asm mov stackbase, ebp;
		CCLCMsg_Move_t* msg = reinterpret_cast<CCLCMsg_Move_t*>(stackbase + 0xFCC);
		auto net_channel = *reinterpret_cast <INetChannel**> (reinterpret_cast <uintptr_t> (csgo->client_state) + 0x9C);
		int32_t new_commands = msg->m_nNewCommands;

		int32_t next_cmdnr = csgo->client_state->nLastOutgoingCommand + csgo->client_state->iChokedCommands + 1;
		int32_t total_new_commands = min(csgo->m_nTickbaseShift, 17);
		csgo->m_nTickbaseShift -= total_new_commands;

		from = -1;
		msg->m_nNewCommands = total_new_commands;
		msg->m_nBackupCommands = 0;

		for (to = next_cmdnr - new_commands + 1; to <= next_cmdnr; to++) {
			if (!ofunct(ecx, slot, buf, from, to, true))
				return false;

			from = to;
		}

		CUserCmd* last_realCmd = interfaces.input->GetUserCmd(slot, from);
		CUserCmd fromCmd;

		if (last_realCmd)
			fromCmd = *last_realCmd;

		CUserCmd toCmd = fromCmd;
		toCmd.command_number++;
		toCmd.tick_count++;

		for (int i = new_commands; i <= total_new_commands; i++) {
			WriteUsercmd(buf, &toCmd, &fromCmd);
			fromCmd = toCmd;
			toCmd.command_number++;
			toCmd.tick_count++;
		}
		return true;

	}
	else
		return ofunct(ecx, slot, buf, from, to, isnewcommand);
}

bool CMAntiAim::can_exploit(int tickbase_shift)
{
	auto weapon = csgo->weapon;

	if (!weapon)
		return false;

	const auto info = (weapon->GetCSWpnData());

	if (!(info))
		return false;

	float curtime = TICKS_TO_TIME(csgo->local->GetTickBase() - tickbase_shift);

	if (curtime < csgo->local->m_flNextAttack())
		return false;

	if (curtime < weapon->NextPrimaryAttack())
		return false;

	return true;
}

void InitDoubleTap()
{
	static bool did_shift_before = false;
	static int double_tapped = 0;
	static int prev_shift_ticks = 0;
	static bool reset = false;
	static int shot_tick = 0;
	static int process_ticks = 0;
	static int limit = 17;
	static auto idx = csgo->weapon->GetItemDefinitionIndex();

	CMAntiAim::Get().shot_dt = false;
	if (CanDT() && !csgo->game_rules->IsFreezeTime())
	{
		if (vars.ragebot.double_tap->active)
		{
			prev_shift_ticks = 0;

			auto can_shift_shot = CMAntiAim::Get().can_exploit(17 + vars.ragebot.disable_dt_delay / 8);
			auto can_shot = CMAntiAim::Get().can_exploit(abs(-1 - prev_shift_ticks));

			if (can_shift_shot || !can_shot && !did_shift_before)
			{
				prev_shift_ticks = 14 + vars.ragebot.disable_dt_delay / 8;
				double_tapped = 0;
			}
			else {
				double_tapped++;
				prev_shift_ticks = 0;
			}

			//if (csgo->send_packet)
			{
				if (prev_shift_ticks > 0)
				{
					if (CMAntiAim::Get().can_exploit(prev_shift_ticks))
					{
						CMAntiAim::Get().shot_dt = true;
						if (csgo->cmd->buttons & IN_ATTACK)
						{
							csgo->m_nTickbaseShift = prev_shift_ticks;
							CMAntiAim::Get().did_shot = true;
							reset = false;
							CMAntiAim::Get().cmd_tick = csgo->cmd->command_number;
							CMAntiAim::Get().base_tick = csgo->local->GetTickBase();
						}
						else
						{
							if ((!(csgo->cmd->buttons & IN_ATTACK) || !Ragebot::Get().shot) && CMAntiAim::Get().did_shot) {
								csgo->dt_charged = false;
								csgo->skip_ticks = vars.ragebot.dt_instant ? 17 : 8;
								CMAntiAim::Get().did_shot = false;
							}
						}
					}
					else
					{
						CMAntiAim::Get().shot_dt = false;
						process_ticks = 0;
					}
				}
				did_shift_before = prev_shift_ticks != 0;
			}
		}
		else {
			csgo->bShootedFirstBullet = false;
			csgo->TickShifted = false;
			csgo->nSinceUse = 0;
		}

		switch (idx) {
		case WEAPON_M249:
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_MP5SD:
		case WEAPON_NEGEV:
		case WEAPON_MP9:
			limit = 5;
			break;
		case WEAPON_UMP45:
		case WEAPON_BIZON:
		case WEAPON_TEC9:
		case WEAPON_MP7:
			limit = 6;
			break;
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_FAMAS:
		case weapon_galilar:
		case WEAPON_M4A1:
		case WEAPON_M4A1S: //coded by Syrex#2192 and bluryhell#2798
		case WEAPON_CZ75:
			limit = 5;
			break;
		case WEAPON_FIVESEVEN:
		case WEAPON_GLOCK:
		case WEAPON_P250:
		case WEAPON_SG553:
			limit = 8;
			break;
		case weapon_hkp2000:
		case WEAPON_USPS:
			limit = 5	;
			break;
		case WEAPON_DEAGLE:
			limit = 6;
			break;
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
			limit = 15;
			break;
		}
	}
}


float LagFix()
{
	float updaterate = interfaces.cvars->FindVar("cl_updaterate")->GetFloat();
	ConVar* minupdate = interfaces.cvars->FindVar("sv_minupdaterate");
	ConVar* maxupdate = interfaces.cvars->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetFloat();

	float ratio = interfaces.cvars->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces.cvars->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = interfaces.cvars->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = interfaces.cvars->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = std::clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, ratio / updaterate);
}

void FakeLatency()
{
	if (!vars.ragebot.extend_backtrack)
		return;

	static auto fake_ping = interfaces.cvars->FindVar("net_fakelag");

	fake_ping->SetValue(500 / 500);
}

bool __stdcall Hooked_CreateMove(float a, CUserCmd* cmd) {
	static auto CreateMove = g_pClientModeHook->GetOriginal< CreateMoveFn >(24);
	Ragebot::Get().DropTarget();

	csgo->updatelocalplayer();

	if (!cmd || !cmd->command_number || !csgo->local || csgo->DoUnload)
		return CreateMove(interfaces.client_mode, a, cmd);

	csgo->cmd = cmd;
	in_cm = false;
	csgo->original = cmd->viewangles;

	PVOID pebp;
	__asm mov pebp, ebp;
	bool* pbSendPacket = (bool*)(*(PDWORD)pebp - 0x1C);
	bool& bSendPacket = *pbSendPacket;

	csgo->send_packet = true;

	DoClantag();

	if (csgo->local->isAlive()) {
		if (csgo->skip_ticks > 0 && !csgo->dt_charged) {
			csgo->skip_ticks--;
			if (csgo->skip_ticks == 0) {
				csgo->dt_charged = true;
			}
			cmd->tick_count = INT_MAX;
			csgo->send_packet = true;
		}

		INetChannel* pNetChannel = (INetChannel*)csgo->client_state->pNetChannel;
		csgo->tick_rate = 1.f / interfaces.global_vars->interval_per_tick;

		csgo->vecUnpredictedVel = csgo->local->GetVelocity();

		in_cm = true;
		csgo->unpdred_tick = interfaces.global_vars->tickcount/*cmd->tick_count*/; // ekzi ����� ��� ���� � globalvars ����� ��� �� �� ��������
		Ragebot::Get().DropTarget();

		if (vars.misc.bunnyhop)
			features->Bunnyhop->Run(csgo->original);

		csgo->weapon = csgo->local->GetWeapon();

		csgo->origin = csgo->local->GetRenderOrigin();

		csgo->unpred_eyepos = csgo->local->GetEyePosition();

		if (csgo->weapon)
		{
			csgo->g_bOverrideVelMod = true;

			if (csgo->g_flVelMod < 1.f)
				*(bool*)((uintptr_t)interfaces.prediction + 0x24) = true;

			csgo->weapon_struct.next_attack = csgo->weapon->NextSecondaryAttack();

			CMAntiAim::Get().Fakelag(csgo->send_packet);

			CEnginePrediction::Get().Start(cmd, csgo->local);
			{
				csgo->weapon_data = csgo->weapon->GetCSWpnData();
				if (vars.visuals.innacuracyoverlay && csgo->weapon->IsGun())
				{
					csgo->spread = csgo->weapon->GetSpread();
					csgo->innacuracy = csgo->weapon->GetInaccuracy();
					csgo->weaponspread = (csgo->spread + csgo->innacuracy) * 1000.f;
				}
				else
					csgo->weaponspread = 0.f;

				if (vars.misc.knifebot)
					CKnifebot::Get().Run();

				if (vars.ragebot.enable)
					Ragebot::Get().Run();

				csgo->eyepos = csgo->local->GetEyePosition();
				Legitbot::Get().AutoPistol();

				ProcessMissedShots();
				if (vars.antiaim.enable)

				{
					FakeDuck(csgo->send_packet);
					CMAntiAim::Get().Run(csgo->send_packet, cmd);
					if (vars.antiaim.enable) {
						CMAntiAim::Get().Sidemove();
					}
				}

				if (csgo->cmd->buttons & IN_ATTACK)
				{
					if (auto net = interfaces.engine->GetNetChannelInfo(); net != nullptr)
						csgo->impact_time = interfaces.global_vars->curtime + net->GetLatency(MAX_FLOWS);
				}

				csgo->g_bOverrideVelMod = false;

				if (csgo->client_state->iChokedCommands <= 3) {
					InitDoubleTap();
				}

				if (vars.ragebot.enable && vars.ragebot.double_tap->active && !csgo->game_rules->IsFreezeTime())
				{
					if (!csgo->disable_dt)
					{
						if (!CanDT())
							csgo->dt_charged = false;
						else if (!csgo->dt_charged && csgo->skip_ticks == 0) {
							csgo->skip_ticks = vars.ragebot.dt_instant ? 16 : 8;
						}
					}
				}

				MoveFixer::Run(csgo->original);
			}
			CEnginePrediction::Get().Finish(csgo->local);

			CGrenadePrediction::Get().Tick(csgo->cmd->buttons);
		}

		csgo->cmd->viewangles = Math::normalize(csgo->cmd->viewangles);

		csgo->choke_meme = csgo->local->GetSimulationTime() != csgo->local->GetOldSimulationTime();

		if (csgo->send_packet) {
			csgo->FakeAngle = csgo->cmd->viewangles;
		}
		else {
			csgo->VisualAngle = csgo->cmd->viewangles;
		}
		resolver->StoreAntifreestand();
		if (vars.misc.hold_firinganims)
		{
			if (Ragebot::Get().IsAbleToShoot() && csgo->cmd->buttons & IN_ATTACK)
				csgo->last_shot_angle = csgo->cmd->viewangles;
			if (Ragebot::Get().HoldFiringAnimation())
				csgo->FakeAngle = csgo->last_shot_angle;
		}

		/*if (csgo->send_packet)
		{
			csgo->chockedticks = csgo->timer;
			csgo->timer = 0;
		}
		else
			csgo->timer++;*/

			/*if (csgo->send_packet)
			{
				csgo->last_choked = csgo->client_state->iChokedCommands;
				csgo->choked_history.push_back(csgo->client_state->iChokedCommands);
				while (csgo->choked_history.size() > 14)
					csgo->choked_history.erase(csgo->choked_history.begin());
			}*/

		static INetChannel* old_net_chan = nullptr;

		if (csgo->client_state != nullptr
			&& pNetChannel != nullptr
			&& csgo->local != nullptr && csgo->local->isAlive())
		{
			csgo->g_pNetChannelHook = std::make_unique<VMTHook>();
			csgo->g_pNetChannelHook->Setup(pNetChannel);
			csgo->g_pNetChannelHook->Hook(39, Hooked_ProcessPacket);
			csgo->g_pNetChannelHook->Hook(40, Hooked_SendNetMsg);
			//csgo->g_pNetChannelHook->Hook(46, Hooked_SendDatagram);
		}
		if (csgo->local->isAlive()) {
			auto animstate = csgo->local->GetPlayerAnimState();
			if (animstate)
				csgo->desync_angle = animstate->m_flGoalFeetYaw;

			csgo->ping = interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
			csgo->ping *= 1000.f;
		}
	}
	else {
		csgo->ForceOffAA = false;
		csgo->send_packet = true;
		in_cm = false;
	}

	cmd = csgo->cmd;
	bSendPacket = csgo->send_packet;

	return false;
}

/*void __stdcall Hooked_CreateMove(int sequence_number, float input_sample_frametime, bool active, bool& sendpacket);
__declspec(naked) void __fastcall Hooked_CreateMoveProxy(PVOID    _this,
	int,
	int        sequence_number,
	float    input_sample_frametime,
	bool    active)
{
	//__asm
	//{
	//	push ebp
	//	mov  ebp, esp
	//	push ebx; not sure if we need this
	//	push esp
	//	push dword ptr[active]
	//	push dword ptr[input_sample_frametime]
	//	push dword ptr[sequence_number]
	//	call Hooked_CreateMove
	//	pop  ebx
	//	pop  ebp
	//	retn 0Ch
	//}

	__asm {
		push ebp
		mov  ebp, esp
		push ebx
		lea  ecx, [esp]
		push ecx
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call Hooked_CreateMove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

void __stdcall Hooked_CreateMove(int sequence_number, float input_sample_frametime, bool active, bool& sendpacket)
{
	static auto CreateMove = g_pClientHook->GetOriginal< decltype(&Hooked_CreateMoveProxy) >(22);

	CreateMove(interfaces.client, 0, sequence_number, input_sample_frametime, active);

	auto cmd = interfaces.input->GetUserCmd(sequence_number);
	auto verified_cmd = interfaces.input->GetVerifiedCmd(sequence_number);
	if (!cmd || cmd->command_number == 0 || csgo->DoUnload)
		return CreateMove(interfaces.client, 0, sequence_number, input_sample_frametime, active);

	csgo->updatelocalplayer();

	if (csgo->client_state->iDeltaTick == -1)
		return;

	if (!csgo->local)
		return;

	if (csgo->skip_ticks > 0 && !csgo->dt_charged) {
		csgo->skip_ticks--;
		if (csgo->skip_ticks == 0) {
			csgo->dt_charged = true;
		}
		cmd->tick_count = INT_MAX;
		sendpacket = true;
	}

	csgo->cmd = cmd;
	in_cm = false;
	csgo->original = cmd->viewangles;
	if (interfaces.engine->IsConnected() && interfaces.engine->IsInGame()) {
		DoClantag();
		INetChannel* pNetChannel = (INetChannel*)csgo->client_state->pNetChannel;
		csgo->tick_rate = 1.f / interfaces.global_vars->interval_per_tick;
		if (csgo->local->isAlive())
		{
			csgo->vecUnpredictedVel = csgo->local->GetVelocity();

			in_cm = true;
			csgo->unpdred_tick = interfaces.global_vars->tickcount;
			Ragebot::Get().DropTarget();

			if (vars.misc.bunnyhop)
				features->Bunnyhop->Run(csgo->original);

			csgo->weapon = csgo->local->GetWeapon();

			csgo->origin = csgo->local->GetRenderOrigin();

			csgo->unpred_eyepos = csgo->local->GetEyePosition();

			if (csgo->weapon)
			{
				if (csgo->client_state->iDeltaTick > 0)
					interfaces.prediction->Update(csgo->client_state->iDeltaTick,
						csgo->client_state->iDeltaTick > 0,
						csgo->client_state->nLastCommandAck,
						csgo->client_state->nLastOutgoingCommand + csgo->client_state->iChokedCommands);

				csgo->g_bOverrideVelMod = true;

				if (csgo->g_flVelMod < 1.f)
					*(bool*)((uintptr_t)interfaces.prediction + 0xB) = true;

				csgo->weapon_struct.next_attack = csgo->weapon->NextSecondaryAttack();

				c_prediction_system::Get().initial(csgo->local, cmd, sequence_number);

				csgo->weapon_data = csgo->weapon->GetCSWpnData();
				if (vars.visuals.innacuracyoverlay && csgo->weapon->IsGun())
				{
					csgo->spread = csgo->weapon->GetSpread();
					csgo->innacuracy = csgo->weapon->GetInaccuracy();
					csgo->weaponspread = (csgo->spread + csgo->innacuracy) * 1000.f;
				}
				else
					csgo->weaponspread = 0.f;

				if (vars.misc.knifebot)
					CKnifebot::Get().Run();

				if (vars.ragebot.enable)
					Ragebot::Get().Run();

				CMAntiAim::Get().Fakelag(sendpacket);
				csgo->eyepos = csgo->local->GetEyePosition();
				Legitbot::Get().AutoPistol();

				ProcessMissedShots();
				if (vars.antiaim.enable)
				{
					FakeDuck(sendpacket);
					CMAntiAim::Get().Run(sendpacket);
					if (vars.antiaim.desync > 0) {
						CMAntiAim::Get().Sidemove();
					}
				}

				if (csgo->cmd->buttons & IN_ATTACK)
				{
					if (auto net = interfaces.engine->GetNetChannelInfo(); net != nullptr)
						csgo->impact_time = interfaces.global_vars->curtime + net->GetLatency(MAX_FLOWS);
				}

				c_prediction_system::Get().repredict(csgo->local, cmd);
				c_prediction_system::Get().restore();

				csgo->g_bOverrideVelMod = false;

				if (csgo->client_state->iChokedCommands <= 3) {
					InitDoubleTap();
				}

				if (vars.ragebot.enable && vars.ragebot.double_tap->active && !csgo->game_rules->IsFreezeTime())
				{
					if (!csgo->disable_dt)
					{
						if (!CanDT())
							csgo->dt_charged = false;
						else if (!csgo->dt_charged && csgo->skip_ticks == 0) {
							csgo->skip_ticks = 16;
						}
					}
				}
			}

			grenade_prediction::instance().Tick(csgo->cmd->buttons);
			if (csgo->StrafeModifiedAngles)
				csgo->original = csgo->StrafeAngles;

			MoveFixer::Run(csgo->original);

			csgo->cmd->viewangles = Math::normalize(csgo->cmd->viewangles);

			csgo->choke_meme = csgo->local->GetSimulationTime() != csgo->local->GetOldSimulationTime();

			if (sendpacket) {
				csgo->FakeAngle = csgo->cmd->viewangles;
			}
			else
			{
				csgo->VisualAngle = csgo->cmd->viewangles;
			}

			if (vars.misc.hold_firinganims)
			{
				if (Ragebot::Get().IsAbleToShoot() && csgo->cmd->buttons & IN_ATTACK)
					csgo->last_shot_angle = csgo->cmd->viewangles;
				if (Ragebot::Get().HoldFiringAnimation())
					csgo->FakeAngle = csgo->last_shot_angle;
			}
			if (sendpacket)
			{
				csgo->chockedticks = csgo->timer;
				csgo->timer = 0;
			}
			else
				csgo->timer++;

			if (sendpacket)
			{
				csgo->last_choked = csgo->client_state->iChokedCommands;
				csgo->choked_history.push_back(csgo->client_state->iChokedCommands);
				f (csgo->choked_history.size() > 14)
					csgo->choked_history.erase(csgo->choked_history.begin());
			}
			if (csgo->local->isAlive()) {
				auto animstate = csgo->local->GetPlayerAnimState();
				if (animstate)
					csgo->desync_angle = animstate->m_flGoalFeetYaw;

				csgo->ping = interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + interfaces.engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
				csgo->ping *= 1000.f;

			}

			if (!csgo->g_pNetChannelHook && csgo->client_state != nullptr
				&& pNetChannel != nullptr
				&& csgo->local != nullptr && csgo->local->isAlive())
			{
				csgo->g_pNetChannelHook = std::make_unique<VMTHook>();
				csgo->g_pNetChannelHook->Setup((DWORD**)pNetChannel);
				csgo->g_pNetChannelHook->Hook(39, Hooked_ProcessPacket);
			}
		}
	}

	//HookNetChannel();

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();

	cmd = csgo->cmd;
	csgo->send_packet = sendpacket;
	in_cm = false;
}*/

int __stdcall Hooked_Camera(ConVar* var, void* return_address, void* eax)
{
	if (csgo->DoUnload)
		return 0;

	static auto return_to_modify_eye_pos = csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::client_dll::s().c_str())].c_str()),
		hs::modify_eye_pos::s().c_str());

	const auto local = csgo->local;
	bool on_ground = false;
	auto animstate = local->GetPlayerAnimState();
	if (animstate)
		on_ground = animstate->m_bInHitGroundAnimation;
	if (return_address == (void*)return_to_modify_eye_pos)
		return !in_cm || (local && !on_ground);

	return 0;
}

// ReSharper disable once CppDeclaratorNeverUsed
static uint32_t get_bool_retn_address = 0;

__declspec(naked) bool get_bool(void* ecx, void* edx)
{
	_asm
	{
		push eax

		mov eax, [esp + 4]
		mov get_bool_retn_address, eax

		push get_bool_retn_address
		push ecx
		call Hooked_Camera

		cmp eax, 0
		je _retn0

		cmp eax, 1
		je _retn1

		cmp eax, 2
		je _retn2

		_retn0 :
		mov al, 0
			retn

			_retn1 :
		pop eax

			mov eax, get_bool_retn_address
			add eax, 0x462

			push eax

			retn

			_retn2 :
		pop eax

			mov eax, get_bool_retn_address
			add eax, 0x462

			push eax

			retn
	}
}