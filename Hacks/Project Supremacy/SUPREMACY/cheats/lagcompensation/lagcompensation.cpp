#include "lagcompensation.h"
#include "..\..\ragebot.h"
#include "../../Resolver_Remake.h"
void lagcompensation::fsn(ClientFrameStage_t stage) {
	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++) {
		auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
		auto & player = players[i];

		player.m_e = e;

		if (!continue_loop(i, e, stage))
			continue;
		switch (stage) {
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START: {
			e->set_abs_origin(e->m_vecOrigin());
		}break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		{
			e->update_clientside_animation();
			player.log();
		}break;
		case FRAME_NET_UPDATE_END: {
			apply_interpolation_flags(e, disable_interpolation);
		}break;
		case FRAME_RENDER_START: {
			*(int*)((uintptr_t)e + 0xA30) = g_csgo.m_globals()->m_framecount;
			*(int*)((uintptr_t)e + 0xA28) = 0;

			if (g_cfg.antiaim.animfix)
				animation_fix(e);
		}break;
		}
	}
}

float GetLerpTime() {
	static ConVar* cl_interp = g_csgo.m_cvar()->FindVar("cl_interp");
	static ConVar* cl_updaterate = g_csgo.m_cvar()->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = g_csgo.m_cvar()->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = g_csgo.m_cvar()->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = g_csgo.m_cvar()->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = g_csgo.m_cvar()->FindVar("sv_client_max_interp_ratio");
	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();
	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;
	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;
	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;
	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;
	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;
	else
		return v20;
}
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	if (in >= high)
		return high;
	return in;
}
bool lagcompensation::IsTickValid(int tick)
{
	INetChannelInfo *nci = g_csgo.m_engine()->GetNetChannelInfo();
	if (!nci)
		return false;
	float correct = 0;
	correct += nci->GetLatency(FLOW_OUTGOING);
	correct += nci->GetLatency(FLOW_INCOMING);
	correct += GetLerpTime();
	ConVar *sv_maxunlag = g_csgo.m_cvar()->FindVar("sv_maxunlag");
	correct = clamp(correct, 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (g_csgo.m_globals()->m_curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}

void lagcompensation::create_move() {
	last_eye_positions.push_front(g_ctx.m_local->get_eye_pos());
	if (last_eye_positions.size() > 128)
		last_eye_positions.pop_back();

	auto nci = g_csgo.m_engine()->GetNetChannelInfo();
	if (!nci)
		return;

	const int latency_ticks = TIME_TO_TICKS(nci->GetLatency(FLOW_OUTGOING));
	const auto latency_based_eye_pos = last_eye_positions.size() <= latency_ticks ? last_eye_positions.back() : last_eye_positions[latency_ticks];

	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++) {
		auto e = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
		auto & player = players[i];

		player.m_e = e;

		if (!e) { player.m_e = nullptr; continue; }

		if (!e->valid(true))
			continue;
	}
}
void player_record_t::log() {
	if (!g_cfg.ragebot.lagcomp) {
		m_track.clear();
		return;
	}

	player_t * e = m_e;

	if (!m_track.size()) {
		tickrecord_t record;
		record.save(e);

		m_track.push_front(record);

		return;
	}

	if (m_track.front().m_simulation_time != e->m_flSimulationTime()) {
		tickrecord_t record;
		record.save(e);

		record.m_previous_origin = m_track.front().m_vec_origin;
		record.m_previous_simulation_time = m_track.front().m_simulation_time;
		record.m_previous_curtime = m_track.front().m_curtime;

		m_track.push_front(record);
	}

	if (g_csgo.m_globals()->m_curtime - m_track.back().m_curtime > 1.f)
		m_track.pop_back();
}
std::deque< tickrecord_t > player_record_t::get_valid_track() {
	auto delta_time = [&](float simulation_time) -> float {
		auto nci = g_csgo.m_engine()->GetNetChannelInfo();

		if (!nci)
			return FLT_MAX;

		float correct = 0.f;
		correct += nci->GetLatency(FLOW_OUTGOING);
		correct += nci->GetLatency(FLOW_INCOMING);
		correct += util::lerp_time();
		correct = math::clamp< float >(correct, 0.f, 1.f);

		return fabs(correct - (g_csgo.m_globals()->m_curtime - simulation_time));
	};

	std::deque< tickrecord_t > track;

	for (const auto & record : m_track) {
		if (delta_time(record.m_simulation_time) < 0.2f)
			track.push_back(record);
	}

	return track;
}
bool lagcompensation::continue_loop(int id, player_t * e, ClientFrameStage_t stage) {
	auto & player = players[id];
	if (!e) { player.m_e = nullptr; return false; }

	switch (stage) {
	case FRAME_RENDER_START:
		if (!e->valid(true))
			return false;

		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		if (!e->valid(true)) {
			return false;
		}

		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END: {
		if (e->m_iHealth() <= 0 || e->m_iTeamNum() == g_ctx.m_local->m_iTeamNum()) {
			player.m_track.clear();
			return false;
		}
		if (e->IsDormant() || e->m_bGunGameImmunity())
		{
			return false;
		}
		/*		if (g_ctx.m_globals.roundstart)	//addde this for big fps fIx crashing on first raunt
				{
					return false;
				}*/
	}break;
	case FRAME_NET_UPDATE_END:
		if (!e->valid(true))
			return false;

		break;
	}

	return true;
}
void lagcompensation::apply_interpolation_flags(player_t * e, int flag) {
	const auto var_map = reinterpret_cast<uintptr_t>(e) + 36;

	for (auto index = 0; index < *reinterpret_cast<int*>(var_map + 20); index++)
		*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(var_map) + index * 12) = flag;
}
void lagcompensation::animation_fix(player_t * e) {
	c_baseplayeranimationstate * animation_state = e->get_animation_state();

	if (!animation_state)
		return;

	auto player = players[e->EntIndex()];

	/*if (player.m_track.size()) {
		tickrecord_t * record = &player.m_track.front();

		if (e->m_flSimulationTime() != record->m_anim_time) {
			e->m_bClientSideAnimation() = true;

			e->update_clientside_animation();

			record->m_anim_time = e->m_flSimulationTime();
			record->m_anim_state = e->get_animation_state();
		}
		else {
			e->m_bClientSideAnimation() = false;

			e->set_animation_state(record->m_anim_state);
		}

		e->set_abs_angles(vec3_t(0, animation_state->m_flGoalFeetYaw, 0));
	}*/
}
void lagcompensation::update_sequence() {
	if (!g_csgo.m_clientstate())
		return;

	auto net_channel = g_csgo.m_clientstate()->m_NetChannel;

	if (net_channel) {
		if (net_channel->m_nInSequenceNr > m_last_incoming_sequence_number) {
			m_last_incoming_sequence_number = net_channel->m_nInSequenceNr;
			sequences.push_front(incoming_sequence_t(net_channel->m_nInReliableState, net_channel->m_nOutReliableState, net_channel->m_nInSequenceNr, g_csgo.m_globals()->m_realtime));
		}

		if (sequences.size() > 2048)
			sequences.pop_back();
	}
}
void lagcompensation::clear_sequence() {
	m_last_incoming_sequence_number = 0;

	sequences.clear();
}
void lagcompensation::add_latency(INetChannel * net_channel) {
	for (auto& seq : sequences) {
		if (g_csgo.m_globals()->m_realtime - seq.m_current_time >= g_cfg.misc.ping_spike_value / 1000.f) {
			net_channel->m_nInReliableState = seq.m_in_reliable_state;
			net_channel->m_nInSequenceNr = seq.m_sequence_nr;

			break;
		}
	}
}

template<class T, class U>
T CMBacktracking::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

void LagRecord::SaveRecord(player_t* player)
{
	m_nFlags = player->m_fFlags();
	m_flSimulationTime = player->m_flSimulationTime();
}
void CMBacktracking::AnimationFix(player_t *player)
{
	auto &lag_records = this->m_LagRecord[player->EntIndex()];
	CMBacktracking::get().RemoveBadRecords(player->EntIndex(), lag_records);
	auto entity = player;
	auto player_index = entity->EntIndex() - 1;
	if (lag_records.size() < 2)
		return;
	player->client_side_animation2() = true;
	auto old_curtime = g_csgo.m_globals()->m_curtime;
	auto old_frametime = g_csgo.m_globals()->m_frametime;
	auto old_ragpos = entity->ragdollpos();

	g_csgo.m_globals()->m_curtime = entity->m_flSimulationTime();
	g_csgo.m_globals()->m_frametime = g_csgo.m_globals()->m_interval_per_tick;

	auto player_animation_state = reinterpret_cast<DWORD*>(entity + 0x3884);
	auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
	if (player_animation_state != nullptr && player_model_time != nullptr)
		if (*player_model_time == g_csgo.m_globals()->m_framecount)
			*player_model_time = g_csgo.m_globals()->m_framecount - 1;
	entity->ragdollpos() = old_ragpos;
	entity->client_side_animation2();
	g_csgo.m_globals()->m_curtime = old_curtime;
	g_csgo.m_globals()->m_frametime = old_frametime;
	player->client_side_animation2() = false;
}

float CMBacktracking::GetLerpTime()
{
	int ud_rate = g_csgo.m_cvar()->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = g_csgo.m_cvar()->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = g_csgo.m_cvar()->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = g_csgo.m_cvar()->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = g_csgo.m_cvar()->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = g_csgo.m_cvar()->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = g_csgo.m_cvar()->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

bool CMBacktracking::IsTickValid(int tick)
{
	static auto sv_maxunlag = g_csgo.m_cvar()->FindVar("sv_maxunlag");
	INetChannelInfo* nci = g_csgo.m_engine()->GetNetChannelInfo();

	if (!nci || !sv_maxunlag)
		return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, sv_maxunlag->GetFloat());
	float deltaTime = correct - (g_csgo.m_globals()->m_curtime - TICKS_TO_TIME(tick));
	return fabsf(deltaTime) < 0.2f;
}

bool CMBacktracking::IsPlayerValid(player_t* player)
{
	if (!player)
		return false;
	if (!g_ctx.m_local)
		return false;
	if (!g_ctx.m_local->is_alive12())
		return false;
	if (!player->is_player())
		return false;

	if (player == g_ctx.m_local)
		return false;

	if (player->IsDormant())
		return false;

	if (!player->is_alive13())
		return false;

	if (player->m_iTeamNum() == g_ctx.m_local->m_iTeamNum())
		return false;

	if (player->m_bGunGameImmunity())
		return false;

	return true;
}

int CMBacktracking::GetPriorityLevel(player_t *player, LagRecord* lag_record)
{
	int priority = 0;
	return priority;
}

void CMBacktracking::FrameUpdatePostEntityThink()
{
	static auto sv_unlag = g_csgo.m_cvar()->FindVar("sv_unlag");

	if (g_csgo.m_globals()->m_maxclients <= 1 || !sv_unlag)
	{
		CMBacktracking::get().ClearHistory();
		return;
	}

	for (int i = 1; i <= g_csgo.m_globals()->m_maxclients; i++)
	{
		player_t *player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		auto &lag_records = this->m_LagRecord[i];

		if (!IsPlayerValid(player))
		{
			if (lag_records.size() > 0)
				lag_records.clear();

			continue;
		}

		int32_t ent_index = player->EntIndex();
		float_t sim_time = player->m_flSimulationTime();

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
bool CMBacktracking::StartLagCompensation(player_t* player)
{
	backtrack_records.clear();
	auto& m_LagRecords = this->m_LagRecord[player->EntIndex()];
	m_RestoreLagRecord[player->EntIndex()].second.SaveRecord(player);
	CMBacktracking::get().RemoveBadRecords(player->EntIndex(), m_LagRecords);
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

bool CMBacktracking::FindViableRecord(player_t *player, LagRecord* record)
{
	return true;
}

void CMBacktracking::FinishLagCompensation(player_t *player)
{
	int idx = player->EntIndex();
	player->SetFlags(m_RestoreLagRecord[idx].second.m_nFlags);
}