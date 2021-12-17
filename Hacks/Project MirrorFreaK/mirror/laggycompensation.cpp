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
	auto old_curtime = Interfaces::Globals->curtime;
	auto old_frametime = Interfaces::Globals->frametime;
	auto old_ragpos = entity->get_ragdoll_pos();

	Interfaces::Globals->curtime = entity->GetSimulationTime();
	Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

	auto player_animation_state = reinterpret_cast<DWORD*>(entity + 0x3884);
	auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
	if (player_animation_state != nullptr && player_model_time != nullptr)
		if (*player_model_time == Interfaces::Globals->framecount)
			*player_model_time = Interfaces::Globals->framecount - 1;
	entity->get_ragdoll_pos() = old_ragpos;
	entity->UpdateClientSideAnimation();
	Interfaces::Globals->curtime = old_curtime;
	Interfaces::Globals->frametime = old_frametime;
	player->client_side_animation() = false;
}

float CMBacktracking::GetLerpTime()
{
	int ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

bool CMBacktracking::IsTickValid(int tick)
{
	static auto sv_maxunlag = Interfaces::CVar->FindVar("sv_maxunlag");
	INetChannelInfo* nci = Interfaces::Engine->GetNetChannelInfo();

	if (!nci || !sv_maxunlag)
		return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (Interfaces::Globals->curtime - TICKS_TO_TIME(tick));
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

void CMBacktracking::FrameUpdatePostEntityThink()
{
	static auto sv_unlag = Interfaces::CVar->FindVar("sv_unlag");

	if (Interfaces::Globals->maxClients <= 1 || !sv_unlag)
	{
		CMBacktracking::Get().ClearHistory();
		return;
	}

	for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
	{
		IClientEntity *player = Interfaces::EntList->GetClientEntity(i);

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
