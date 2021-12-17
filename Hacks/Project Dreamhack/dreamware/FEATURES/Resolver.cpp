#include "../MISC/includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CClientEntityList.h"
#include "../UTILS/render.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../SDK/AnimLayer.h"
#include "../UTILS/qangle.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
float lerp_time()
{
	int ud_rate = INTERFACES::cvar->FindVar("cl_updaterate")->GetFloat();
	SDK::ConVar *min_ud_rate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	SDK::ConVar *max_ud_rate = INTERFACES::cvar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = INTERFACES::cvar->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = INTERFACES::cvar->FindVar("cl_interp")->GetFloat();
	SDK::ConVar *c_min_ratio = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	SDK::ConVar *c_max_ratio = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}

void CResolver::max_eye_ang_delta()
{
	resolver_info_t2
		&current = m_current,
		&previous = m_previous;

	float delta = previous.EyeAng - current.EyeAng;
	if (current.Desync >= abs(delta) && current.lastMaxDSC < abs(delta) && abs(delta) > 4.0)
		current.lastMaxDSC = delta;
	if (current.Desync < current.lastMaxDSC)
		current.lastMaxDSC = current.Desync;
}

bool is_slow_walking(SDK::CBaseEntity * entity) {
	float velocity_2D[64], old_velocity_2D[64];

	if (entity->GetVelocity().Length2D() != velocity_2D[entity->GetIndex()] && entity->GetVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->GetIndex()] = velocity_2D[entity->GetIndex()];
		velocity_2D[entity->GetIndex()] = entity->GetVelocity().Length2D();
	}

	if (velocity_2D[entity->GetIndex()] > 0.1) {
		int tick_counter[64];

		if (velocity_2D[entity->GetIndex()] == old_velocity_2D[entity->GetIndex()])
			++tick_counter[entity->GetIndex()];
		else
			tick_counter[entity->GetIndex()] = 0;

		while (tick_counter[entity->GetIndex()] > (1 / INTERFACES::Globals->interval_per_tick) * fabsf(0.1f))// should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;

	}
	return false;
}


float network_latency() {
	SDK::INetChannelInfo *nci = INTERFACES::Engine->GetNetChannelInfo();
	if (nci)
		return nci->GetAvgLatency(FLOW_INCOMING);

	return 0.0f;
}

int GetChokedTicksNetworked(SDK::CBaseEntity * entity) {
	float flSimulationTime = entity->GetSimTime();
	float flSimDiff = INTERFACES::Globals->curtime - flSimulationTime;
	float latency = network_latency();
	return (int)(0.5f + (float)(max(0.0f, flSimDiff - latency)) / INTERFACES::Globals->interval_per_tick);
}


void CResolver::c_data(SDK::CBaseEntity* entity) {
	bool IsMoving = entity->GetVelocity().Length2D() > 0.1;
	bool SlowMo = entity->GetVelocity().Length2D() < 40;
	bool IsDucking = entity->GetFlags() & FL_DUCKING;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto lowerbody = entity->GetLowerBodyYaw();

	auto animationstate = entity->GetAnimState();
	auto feet_yaw = animationstate->m_flCurrentFeetYaw;
	auto body_max_rotation = animationstate->pad10[516];

	for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
	{
		auto p_entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (p_entity && !p_entity->GetIsDormant())
		{

			if (feet_yaw <= 58)
			{
				if (-58 > feet_yaw)
					lowerbody = body_max_rotation + p_entity->GetLowerBodyYaw();
			}
			else
			{
				lowerbody = body_max_rotation - p_entity->GetLowerBodyYaw();
			}
			if (p_entity->GetAnimOverlays()->m_flPlaybackRate > 0.1)
			{
				for (int resolve_delta = 58.f; resolve_delta < -58.f; resolve_delta = resolve_delta - 20.f)
				{
					lowerbody = resolve_delta;
				}
			}
		}
	}
}


bool CResolver::AntiFreestanding(SDK::CBaseEntity* entity, float& yaw) {
	player_resolve_record player_resolve_records[64];

	const auto freestanding_record = player_resolve_records[entity->GetIndex()].anti_freestanding_record;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return false;

	if (freestanding_record.left_damage >= 20 && freestanding_record.right_damage >= 20)
		return false;

	const float at_target_yaw = UTILS::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y;
	if (freestanding_record.left_damage <= 0 && freestanding_record.right_damage <= 0) {
		if (freestanding_record.right_fraction < freestanding_record.left_fraction)
			yaw = at_target_yaw + 125.f;
		else
			yaw = at_target_yaw - 73.f;
	}
	else {
		if (freestanding_record.left_damage > freestanding_record.right_damage)
			yaw = at_target_yaw + 130.f;
		else
			yaw = at_target_yaw - 49.f;
	}

	return true;
}

void CResolver::c_resolver_code(SDK::CBaseEntity * entity) {
	/// a copy of the previous record for comparisons and shit
	const auto previous_record = player_resolve_records[entity->GetIndex()];
	auto& record = player_resolve_records[entity->GetIndex()];
	auto& resolve_record = player_resolve_records[entity->GetIndex()];


	record.resolved_angles = record.networked_angles;
	record.velocity = entity->GetVelocity();
	record.origin = entity->GetVecOrigin();
	record.lower_body_yaw = entity->GetLowerBodyYaw();
	record.is_dormant = entity->GetIsDormant();
	record.resolve_type = 0;

	record.is_balance_adjust_triggered = false, record.is_balance_adjust_playing = false;
	if (AntiFreestanding(entity, resolve_record.resolved_angles.y)) {
		resolve_record.resolve_type |= RESOLVE_TYPE_ANTI_FREESTANDING;
	}
	max_eye_ang_delta();

}

void CResolver::g_desync_correction(SDK::CBaseEntity* pPlayer)
{
	/*auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!pPlayer) return;
	if (!local_player) return;

	bool is_local_player = pPlayer == local_player;
	bool is_teammate = local_player->GetTeam() == pPlayer->GetTeam() && !is_local_player;
	if (is_local_player) return;
	if (is_teammate) return;
	if (pPlayer->GetHealth() <= 0) return;
	if (local_player->GetHealth() <= 0) return;

	float old_yaw[64] = { NULL };
	float current_yaw[64] = { NULL };
	float resolved_yaw[64] = { 0.f };

	float y_delta[64] = { 0.f };*/

	//c_resolver_code(pPlayer);
}

CResolver* resolver = new CResolver();



