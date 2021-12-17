#include "c_legit_backtrack.h"

void CLegitBacktrack::OnMove(CUserCmd* pCmd) {
	if (!Interfaces::Engine->IsInGame() || !G::LocalPlayer || !G::LocalPlayer->IsAlive()) {
		data.clear();
		return;
	}

	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
	if (!pWeapon || !pWeapon->is_gun()) {
		data.clear();
		return;
	}

	if (c_config::get()->i["legit_backtrack"] == 0) {
		data.clear();
		return;
	}

	static ConVar* sv_maxunlag = Interfaces::Var->FindVar("sv_maxunlag");
	static ConVar* sv_minupdaterate = Interfaces::Var->FindVar("sv_minupdaterate");
	static ConVar* sv_maxupdaterate = Interfaces::Var->FindVar("sv_maxupdaterate");

	static ConVar* sv_client_min_interp_ratio = Interfaces::Var->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces::Var->FindVar("sv_client_max_interp_ratio");

	static ConVar* cl_interp_ratio = Interfaces::Var->FindVar("cl_interp_ratio");
	static ConVar* cl_interp = Interfaces::Var->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces::Var->FindVar("cl_updaterate");

	float updaterate = cl_updaterate->GetFloat();

	float minupdaterate = sv_minupdaterate->GetFloat();
	float maxupdaterate = sv_maxupdaterate->GetFloat();

	float min_interp = sv_client_min_interp_ratio->GetFloat();
	float max_interp = sv_client_max_interp_ratio->GetFloat();

	float flLerpAmount = cl_interp->GetFloat();
	float flLerpRatio = cl_interp_ratio->GetFloat();
	flLerpRatio = std::clamp(flLerpRatio, min_interp, max_interp);
	if (flLerpRatio == 0.0f)
		flLerpRatio = 1.0f;

	float updateRate = std::clamp(updaterate, minupdaterate, maxupdaterate);
	lerp_time = std::fmaxf(flLerpAmount, flLerpRatio / updateRate);
	latency = Interfaces::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING) + Interfaces::Engine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);
	correct_time = latency + lerp_time;

	// setup records
	CBaseEntity* player;
	for (int i = 1; i <= Interfaces::Engine->GetMaxClients(); ++i) {
		player = Interfaces::EntityList->GetClientEntity(i);
		if (player->IsNotTarget()) {
			if (data.count(i) > 0)
				data.erase(i);
			continue;
		}

		if (player->GetTeam() == G::LocalPlayer->GetTeam()) {
			if (data.count(i) > 0)
				data.erase(i);
			continue;
		}

		auto& cur_data = data[i];
		if (!cur_data.empty()) {
			auto& front = cur_data.front();
			if (front.simTime == player->GetSimTime())
				continue;

			while (!cur_data.empty()) {
				auto& back = cur_data.back();
				float deltaTime = correct_time - (Interfaces::Globals->curtime - back.simTime);
				if (std::fabsf(deltaTime) <= 0.2f)
					break;

				cur_data.pop_back();
			}
		}

		auto model = player->GetModel();
		if (!model)
			continue;

		auto hdr = Interfaces::ModelInfo->GetStudioModel(model);
		if (!hdr)
			continue;

		auto hitbox_set = hdr->pHitboxSet(player->HitBoxSet());
		auto hitbox_head = hitbox_set->GetHitbox(HITBOX_HEAD);
		auto hitbox_center = (hitbox_head->bbmin + hitbox_head->bbmax) * 0.5f;

		CLegitBacktrackData bd;
		bd.simTime = player->GetSimTime();

		*(Vector*)((uintptr_t)player + 0xA0) = player->GetOrigin();
		*(int*)((uintptr_t)player + 0xA68) = 0;
		*(int*)((uintptr_t)player + 0xA30) = 0;
		player->InvalidateBoneCache();
		player->SetupBones(bd.boneMatrix, 128, BONE_USED_BY_ANYTHING, Interfaces::Globals->curtime);

		Cheat::Utilities->Math_VectorTransform(hitbox_center, bd.boneMatrix[hitbox_head->bone], bd.hitboxPos);

		data[i].push_front(bd);
	}

	Vector localEyePos = G::LocalPlayer->GetEyePosition();
	Vector angles;
	int tick_count = -1;
	float best_fov = 180.0f;
	for (auto& node : data) {
		auto& cur_data = node.second;
		if (cur_data.empty())
			continue;
		int i = -1;
		for (auto& bd : cur_data) {
			++i;

			float deltaTime = correct_time - (Interfaces::Globals->curtime - bd.simTime);
			if (c_config::get()->i["legit_backtrack"] == 1)
			{
				if (i != (int)(c_config::get()->f["legit_backtrack_range"] * (float)cur_data.size() / 201.f))
					continue;
			}
			else
			{
				if (std::fabsf(deltaTime) > c_config::get()->f["legit_backtrack_range"] / 1000.f)
					continue;
			}

			Cheat::Utilities->Math_VectorAngles(bd.hitboxPos - localEyePos, angles);
			Cheat::Utilities->Math_FixAngles(angles);
			float fov = GetFovToPlayer(pCmd->viewangles, angles);
			if (best_fov > fov) {
				best_fov = fov;
				tick_count = TIME_TO_TICKS(bd.simTime + lerp_time);
			}
		}
	}

	if (tick_count != -1) {
		pCmd->tick_count = tick_count;
	}
}

CLegitBacktrack g_legit_backtrack;