#include "laggycompensation.h"
#include "RageBot.h"
CMBacktracking * cm_backtrack = new CMBacktracking();
template<class T, class U>
T CMBacktracking::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void LagRecord::SaveRecord(IClientEntity* player)
{
	m_nFlags = player->getFlags();
	m_flSimulationTime = player->GetSimulationTime();
}
void CMBacktracking::AnimationFix(IClientEntity *player)
{
	auto &lag_records = this->m_LagRecord[player->GetIndex()];
	CMBacktracking::Get().RemoveBadRecords(player->GetIndex(), lag_records);
	auto entity = player;
	auto player_index = entity->GetIndex() - 1;
	if (lag_records.size() < 2)
		return;
	player->client_side_animation() = true;
	auto old_curtime = interfaces::globals->curtime;
	auto old_frametime = interfaces::globals->frametime;
	auto old_ragpos = entity->get_ragdoll_pos();

	interfaces::globals->curtime = entity->GetSimulationTime();
	interfaces::globals->frametime = interfaces::globals->interval_per_tick;

	auto player_animation_state = reinterpret_cast<DWORD*>(entity + 0x3884);
	auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
	if (player_animation_state != nullptr && player_model_time != nullptr)
		if (*player_model_time == interfaces::globals->framecount)
			*player_model_time = interfaces::globals->framecount - 1;
	entity->get_ragdoll_pos() = old_ragpos;
	entity->UpdateClientSideAnimation();
	interfaces::globals->curtime = old_curtime;
	interfaces::globals->frametime = old_frametime;
	player->client_side_animation() = false;
}

float CMBacktracking::GetLerpTime()
{
	int ud_rate = interfaces::cvar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = interfaces::cvar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = interfaces::cvar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = interfaces::cvar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = interfaces::cvar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

bool CMBacktracking::IsTickValid(int tick)
{
	static auto sv_maxunlag = interfaces::cvar->FindVar("sv_maxunlag");
	INetChannelInfo* nci = interfaces::engine->GetNetChannelInfo();

	if (!nci || !sv_maxunlag)
		return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (interfaces::globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}


bool CMBacktracking::IsPlayerValid(IClientEntity* player)
{
	if (!player)
		return false;
	if (!hackManager.pLocal())
		return false;
	if (!hackManager.pLocal()->IsAlive())
		return false;
	if (!player->IsPlayer())
		return false;

	if (player == hackManager.pLocal())
		return false;

	if (player->IsDormant())
		return false;

	if (!player->IsAlive())
		return false;

	if (player->team() == hackManager.pLocal()->team())
		return false;

	if (player->HasGunGameImmunity())
		return false;

	return true;
}


int CMBacktracking::GetPriorityLevel(IClientEntity *player, LagRecord* lag_record)
{
	int priority = 0;
	return priority;
}

enum Typetype_t
{
	TYPE_GENERIC = 0,
	TYPE_LOCALPLAYER = 1
};

int CMBacktracking::GetLatencyTicks()
{
	double v0; // st7@0
	INetChannelInfo* v1; // esi@1
	INetChannelInfo* v2; // eax@1
	float v3; // ST08_4@1
	float v4; // ST0C_4@1
	v1 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v2 = (INetChannelInfo*)interfaces::engine->GetNetChannelInfo();
	v3 = v1->GetAvgLatency(Typetype_t::TYPE_LOCALPLAYER);
	v4 = v2->GetAvgLatency(Typetype_t::TYPE_GENERIC);
	float interval_per_tick = 1.0f / interfaces::globals->interval_per_tick;
	return floorf(((v3 + v4) * interval_per_tick) + 0.5f);
}

void CMBacktracking::ExtrapolatePosition(IClientEntity* pPlayer, Vector& position, float &simtime, Vector velocity)
{
	auto AirAccelerate = [](IClientEntity* pPlayer, Vector &wishdir, float wishspeed, float accel, Vector &velo)
	{
		float wishspd = wishspeed;
		if (wishspd > 30.f)
			wishspd = 30.f;
		float currentspeed = DotProduct(pPlayer->GetVelocity(), wishdir);
		float addspeed = wishspd - currentspeed;
		if (addspeed <= 0)
			return;
		float accelspeed = accel * wishspeed * interfaces::globals->frametime * pPlayer->GetFriction();
		if (accelspeed > addspeed)
			accelspeed = addspeed;
		velo += accelspeed * wishdir;
	};

	int index = pPlayer->GetIndex();
	simulation_record latest_record = this->SimRecord[index][0];

	int latency_ticks = GetLatencyTicks();
	Vector vel = velocity;
	float s_time = simtime;
	INetChannelInfo* nci = interfaces::engine->GetNetChannelInfo();
	float latency = nci->GetAvgLatency(0) + nci->GetAvgLatency(1);
	float ticks_per_second = 1.0f / interfaces::globals->interval_per_tick;
	//	if (ragebot.storedcmd) {
	float server_time = ((floorf(((latency)*ticks_per_second) + 0.5) + tick_count + 1) * interfaces::globals->interval_per_tick);
	float server_time_difference = server_time - s_time;
	if (server_time_difference > 1.0f)
		server_time_difference = 1.0f;
	float sim_time_difference = interfaces::globals->curtime - latest_record.update_time;
	//proper method
	int ticks_choked = /*TIME_TO_TICKS(latest_record.simulation_time - pre_latest_record.simulation_time_increasment_per_tick);*/TIME_TO_TICKS(sim_time_difference);
	if (ticks_choked <= 15)
	{
		latency_ticks = 1;
		if (ticks_choked < 1)
			ticks_choked = 1;
	}
	else
		ticks_choked = 15;
	int total_difference = floorf((server_time_difference * ticks_per_second) + 0.5) - ticks_choked;
	vel.z -= interfaces::cvar->FindVar("sv_gravity")->GetFloat() * interfaces::globals->interval_per_tick;
	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldAndPropsOnly filter;
	if (total_difference < 0)
	{
		return;
	}
	else
	{
		do
		{
			int chokes = ticks_choked;

			do
			{
				ray.Init(position, position + (vel* interfaces::globals->interval_per_tick), hackManager.pLocal()->GetMins(), hackManager.pLocal()->GetMaxs());
				interfaces::trace->TraceRay(ray, MASK_SOLID, &filter, &tr);
				if (tr.fraction > 0)
				{
					position += vel * (interfaces::globals->interval_per_tick);
					simtime += interfaces::globals->interval_per_tick;
				}
				else
				{
					return;
				}

			} while (chokes);
			total_difference -= ticks_choked;
		} while (total_difference >= 0);
	}


	//	}
}

void CMBacktracking::ProcessCMD(int iTargetIdx, CUserCmd *usercmd)
{
	LagRecord recentLR = m_RestoreLagRecord[iTargetIdx].first;

	if (!IsTickValid(TIME_TO_TICKS(recentLR.m_flSimulationTime)))
		usercmd->tick_count = TIME_TO_TICKS(recentLR.m_flSimulationTime + GetLerpTime());
	else
		usercmd->tick_count = TIME_TO_TICKS(recentLR.m_flSimulationTime);
}


void CMBacktracking::FrameUpdatePostEntityThink()
{
	static auto sv_unlag = interfaces::cvar->FindVar("sv_unlag");

	if (interfaces::globals->maxClients <= 1 || !sv_unlag)
	{
		CMBacktracking::Get().ClearHistory();
		return;
	}

	for (int i = 1; i <= interfaces::globals->maxClients; i++)
	{
		IClientEntity *player = interfaces::ent_list->get_client_entity(i);

		auto &lag_records = this->m_LagRecord[i];

		if (!IsPlayerValid(player))
		{
			if (lag_records.size() > 0)
				lag_records.clear();

			continue;
		}

		int32_t ent_index = player->GetIndex();
		float_t sim_time = player->GetSimulationTime();

		LagRecord cur_lagrecord;

		RemoveBadRecords(ent_index, lag_records);

		if (lag_records.size() > 0)
		{
			auto &tail = lag_records.back();

			if (tail.m_flSimulationTime == sim_time)
				continue;
		}

		cur_lagrecord.SaveRecord(player); // first let's create the record

		if (!lag_records.empty()) // apply specific stuff that is needed
		{
			auto &temp_lagrecord = lag_records.back();
			RemoveBadRecords(ent_index, lag_records);
			int32_t priority_level = GetPriorityLevel(player, &temp_lagrecord);

			cur_lagrecord.m_iPriority = priority_level;

			RemoveBadRecords(ent_index, lag_records);

		}

		lag_records.emplace_back(cur_lagrecord);
	}
}
bool CMBacktracking::StartLagCompensation(IClientEntity* player)
{
	backtrack_records.clear();
	auto& m_LagRecords = this->m_LagRecord[player->GetIndex()];
	m_RestoreLagRecord[player->GetIndex()].second.SaveRecord(player);
	CMBacktracking::Get().RemoveBadRecords(player->GetIndex(), m_LagRecords);
	LagRecord newest_record = LagRecord();
	for (auto it : m_LagRecords)
	{
		if (it.m_flSimulationTime > newest_record.m_flSimulationTime)
			newest_record = it;
		if (it.m_iPriority >= 1)
			backtrack_records.emplace_back(it);
	}
	backtrack_records.emplace_back(newest_record);
	std::sort(backtrack_records.begin(), backtrack_records.end(), [](LagRecord const &a, LagRecord const &b) { return a.m_iPriority > b.m_iPriority; });
	return backtrack_records.size() > 0;
}

void CMBacktracking::RemoveBadRecords(int Idx, std::deque<LagRecord>& records)
{
	auto& m_LagRecords = records; // Should use rbegin but can't erase
	for (auto lag_record = m_LagRecords.begin(); lag_record != m_LagRecords.end(); lag_record++)
	{
		if (!IsTickValid(TIME_TO_TICKS(lag_record->m_flSimulationTime)))
		{
			m_LagRecords.erase(lag_record);
			if (!m_LagRecords.empty())
				lag_record = m_LagRecords.begin();
			else
				break;
		}
	}
}

bool CMBacktracking::FindViableRecord(IClientEntity *player, LagRecord* record)
{
	return true;
}

void CMBacktracking::FinishLagCompensation(IClientEntity *player)
{
	int idx = player->GetIndex();
	player->SetFlags(m_RestoreLagRecord[idx].second.m_nFlags);
}
























































































































































































