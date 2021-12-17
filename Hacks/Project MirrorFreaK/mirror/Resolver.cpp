#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "position_adjust.h"
#include "LagCompensation2.h"
#include "laggycompensation.h"
#include "global_count.h"
#include "position_adjust.h"
#include "Autowall.h"
#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#else
#define XorStr( s ) ( s )
#endif

resolver_setup * resolver = new resolver_setup();

namespace global_count
{
	int hits[65] = { 0.f };
	int shots_fired[65] = { 0.f };
	int missed_shots[64] = { 0.f };
	bool didhit[64] = { 0.f };
	bool on_fire;

	int missed;
	int hit;
}

void calculate_angle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	vec_t hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.0f;
	if (delta.x >= 0.0) angles.y += 180.0f;
}
void NormalizeNumX(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
void resolver_setup::preso(IClientEntity * pEntity)
{
	switch (Options::Menu.RageBotTab.preso.GetIndex())
	{
	case 1:
	{
		pEntity->GetEyeAnglesXY()->x = 89;
		resolver->resolved_pitch = 89.f;
	}
	break;
	case 2:
	{
		pEntity->GetEyeAnglesXY()->x = -89;
		resolver->resolved_pitch = -89.f;
	}
	break;
	case 3:
	{
		pEntity->GetEyeAnglesXY()->x = 0;
		resolver->resolved_pitch = 0.f;
	}
	break;
	case 4:
	{
		float last_simtime[64] = { 0.f };
		float stored_pitch_1[64] = { 0.f };
		float fixed_pitch[64] = { 0.f };

		bool has_been_set[64] = { false };

		const auto local = hackManager.pLocal();
		if (!local)
			return;

		for (auto i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
		{
			CMBacktracking* backtrack;
			const auto player = const_cast <IClientEntity*>(Interfaces::EntList->GetClientEntity(i));

			if (!player || local == player || player->team() == local->team() || player->IsImmune() || player->IsDormant())
			{
				stored_pitch_1[i] = { FLT_MAX };
				fixed_pitch[i] = { FLT_MAX };
				has_been_set[i] = { false };
				continue;
			}

			const auto eye = player->GetEyeAnglesXY();
			const auto sim = player->GetSimulationTime();

			auto pitch = 0.f;

			if (stored_pitch_1[i] == FLT_MAX || !has_been_set[i])
			{
				stored_pitch_1[i] = eye->x;
				has_been_set[i] = true;
			}
			
			if (stored_pitch_1[i] - eye->x < 30 && stored_pitch_1[i] - eye->x > -30)
			{
				pitch = eye->x;
			}
			else
			{
				pitch = stored_pitch_1[i];
			}

		}
	}
	break;

	}

}

player_info_t GetInfo2(int Index) {
	player_info_t Info;
	Interfaces::Engine->GetPlayerInfo(Index, &Info);
	return Info;
}

static int GetSequenceActivity(IClientEntity* pEntity, int sequence)
{
	const model_t* pModel = pEntity->GetModel();
	if (!pModel)
		return 0;

	auto hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!hdr)
		return -1;

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return get_sequence_activity(pEntity, hdr, sequence);
}

float NormalizeFloatToAngle(float input)
{
	for (auto i = 0; i < 3; i++) {
		while (input < -180.0f) input += 360.0f;
		while (input > 180.0f) input -= 360.0f;
	}
	return input;
}

float override_yaw(IClientEntity* player, IClientEntity* local) {
	Vector eye_pos, pos_enemy;
	CalcAngle(player->GetEyePosition(), local->GetEyePosition(), eye_pos);

	if (Render::TransformScreen(player->GetOrigin(), pos_enemy))
	{
		if (GUI.GetMouse().x < pos_enemy.x)
			return (eye_pos.y - 90);
		else if (GUI.GetMouse().x > pos_enemy.x)
			return (eye_pos.y + 90);
	}

}

#define M_PI 3.14159265358979323846
void VectorAnglesBrute(const Vector& forward, Vector &angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0) pitch = 270; else pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0) yaw += 360; tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]); pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0) pitch += 360;
	} angles[0] = pitch; angles[1] = yaw; angles[2] = 0;
}

Vector calc_angle_trash(Vector src, Vector dst)
{
	Vector ret;
	VectorAnglesBrute(dst - src, ret);
	return ret;
}

int total_missed[64];
int total_hit[64];
IGameEvent* event = nullptr;
extra s_extra;
void angle_correction::missed_due_to_desync(IGameEvent* event) {

	if (event == nullptr)
		return;
	int user = event->GetInt("userid");
	int attacker = event->GetInt("attacker");
	bool player_hurt[64], hit_entity[64];

	if (Interfaces::Engine->GetPlayerForUserID(user) != Interfaces::Engine->GetLocalPlayer()
		&& Interfaces::Engine->GetPlayerForUserID(attacker) == Interfaces::Engine->GetLocalPlayer()) {
		player_hurt[Interfaces::Engine->GetPlayerForUserID(user)] = true;
	}

	if (Interfaces::Engine->GetPlayerForUserID(user) != Interfaces::Engine->GetLocalPlayer())
	{
		Vector bullet_impact_location = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		if (Globals::aim_point != bullet_impact_location) return;
		hit_entity[Interfaces::Engine->GetPlayerForUserID(user)] = true;
	}
	if (!player_hurt[Interfaces::Engine->GetPlayerForUserID(user)] && hit_entity[Interfaces::Engine->GetPlayerForUserID(user)]) {
		s_extra.current_flag[Interfaces::Engine->GetPlayerForUserID(user)] = correction_flags::DESYNC;
		++total_missed[Interfaces::Engine->GetPlayerForUserID(user)];
	}
	if (player_hurt[Interfaces::Engine->GetPlayerForUserID(user)] && hit_entity[Interfaces::Engine->GetPlayerForUserID(user)]) {
		++total_hit[Interfaces::Engine->GetPlayerForUserID(user)];
	}
}

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
	int ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetFloat();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
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


void angle_correction::ac_simple(IClientEntity* entity)
{
	missed_due_to_desync(event);

	if (is_slow_walking(entity))
	{
		s_extra.current_flag[entity->GetIndex()] = correction_flags::SLOW_WALK;
		resolver->enemy_fakewalk = true;
	}
	else
		resolver->enemy_fakewalk = false;

	bool jitter_desync[64] = { false };;

	float old_yaw[64] = { NULL };
	float current_yaw[64] = { NULL };
	float oldlby[64] = { NULL };
	float test_yaw[64] = { NULL };
	float brutelby[64] = { NULL };
	float test[64] = { NULL };
	float resolved_yaw[64] = { 0.f };
	float b1gdelta[64], lbydelta[64] = { 0.f };

	b1gdelta[entity->GetIndex()] = entity->GetEyeAnglesXY()->y - old_yaw[entity->GetIndex()];

	if (total_missed[entity->GetIndex()] > 4)
	{
		resolver->enemy_fake = false;
		total_missed[entity->GetIndex()] = 0;
	}
	if (total_missed[entity->GetIndex()] > 1 && total_missed[entity->GetIndex()] <= 4)
	{
		resolver->enemy_fake = true;
	}
	auto feet_yaw = entity->GetBasePlayerAnimState()->m_flCurrentFeetYaw;
	auto body_max_rotation = entity->GetBasePlayerAnimState()->pad10[516];

	if (!entity->IsMoving())
	{
		if (feet_yaw <= 58)
		{
			if (feet_yaw < -58)
				entity->GetEyeAnglesXY()->y = body_max_rotation + entity->GetEyeAnglesXY()->y;
		}

	}
	else 
	{
		if (total_missed[entity->GetIndex() < 3])
		{
			if ((jitter_desync[entity->GetIndex()] || s_extra.current_flag[entity->GetIndex()] == correction_flags::SLOW_WALK))
			{
				entity->GetEyeAnglesXY()->y = entity->GetLowerBodyYaw();
			}
		}
		else
			entity->GetEyeAnglesXY()->y = entity->GetLowerBodyYaw();
	
	}

}

void angle_correction::ac_smart(IClientEntity* entity)
{
	if (!hackManager.pLocal()->IsAlive())
		return;

	if (is_slow_walking(entity))
	{
		s_extra.current_flag[entity->GetIndex()] = correction_flags::SLOW_WALK;
		resolver->enemy_fakewalk = true;
	}
	else
		resolver->enemy_fakewalk = false;

	float old_yaw[64] = { NULL };
	float current_yaw[64] = { NULL };
	float test_yaw[64] = { NULL };
	float delta[64] = { NULL };


	for (int i = 1; i < Interfaces::Globals->maxClients; i++)
	{
		IClientEntity * pentity = Interfaces::EntList->GetClientEntity(i);

		auto feet_yaw = pentity->GetBasePlayerAnimState()->m_flCurrentFeetYaw;
		auto body_max_rotation = pentity->GetBasePlayerAnimState()->pad10[516];
		if (feet_yaw <= 58)
		{
			if (feet_yaw < -58)
				pentity->GetEyeAnglesXY()->y = body_max_rotation + pentity->GetEyeAnglesXY()->y;
		}

		else
		{
			pentity->GetEyeAnglesXY()->y = body_max_rotation - pentity->GetEyeAnglesXY()->y;
		}
	}
}

bool angle_correction::is_slow_walking(IClientEntity* entity) {
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

		while (tick_counter[entity->GetIndex()] > (1 / Interfaces::Globals->interval_per_tick) * fabsf(0.1f))// should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;

	}
	return false;
}

#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
float apply_freestanding_low_performance(IClientEntity *enemy)
{
	IClientEntity* local_player = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!(hackManager.pLocal()->GetHealth() > 0))
		return 0.0f;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;

	auto leyepos = enemy->GetOrigin_likeajew() + enemy->GetViewOffset();
	auto headpos = GetHitboxPosition(enemy, 0);
	auto origin = enemy->GetOrigin_likeajew();

	int index = ragebot->GetTargetCrosshair();

	if (index == -1)
		return 0.0f;

	auto checkWallThickness = [&](IClientEntity* pPlayer, Vector newhead) -> float
	{

		Vector endpos1, endpos2;

		Vector eyepos = local_player->GetOrigin_likeajew() + local_player->GetViewOffset();
		Ray_t ray;
		CTraceFilterSkipTwoEntities filter(local_player, enemy);
		trace_t trace1, trace2;

		ray.Init(newhead, eyepos);
		Interfaces::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (index == ragebot->GetTargetCrosshair())
		{
			if (trace1.DidHit())
			{
				endpos1 = trace1.endpos;
				float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 75.f;
				return endpos1.Dist(eyepos) + add / 2; // endpos2
			}

			else
			{
				endpos1 = trace1.endpos;
				float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) - 75.f;
				return endpos1.Dist(eyepos) + add / 2; // endpos2
			}
		}

		else
		{
			auto feet_yaw = enemy->GetBasePlayerAnimState()->m_flCurrentFeetYaw;
			auto body_max_rotation = enemy->GetBasePlayerAnimState()->pad10[516];
			if (feet_yaw <= 58)
			{
				if (feet_yaw < -58)
					return body_max_rotation + enemy->GetEyeAnglesXY()->y;
				else
					return body_max_rotation - enemy->GetEyeAnglesXY()->y;
			}
		}

	};

	float radius = Vector(headpos - origin).Length2D();

	for (float besthead = 0; besthead < 7; besthead += 0.1)
	{
		Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
		float totalthickness = 0.f;
		no_active = false;
		totalthickness += checkWallThickness(enemy, newhead);
		if (totalthickness > highestthickness)
		{
			highestthickness = totalthickness;

			bestrotation = besthead;
		}
	}
	return RAD2DEG(bestrotation);

}
void resolver_setup::force_freestanding(IClientEntity * enemy)
{
	enemy->GetEyeAnglesXY()->y = apply_freestanding_low_performance(enemy);
}

void resolver_setup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
		{
			pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!pEntity->valid_entity() || pEntity->team() == hackManager.pLocal()->team() || !(pEntity->GetFlags() & FL_ONGROUND))
				continue;

			angle_correction ac;

			if (Options::Menu.RageBotTab.preso.GetIndex() > 0)
			{
				resolver_setup::preso(pEntity);
			}
		
			if (Options::Menu.RageBotTab.resolver.GetIndex() == 1 && !GetAsyncKeyState(Options::Menu.RageBotTab.flip180.GetKey()))
			{
				ac.ac_simple(pEntity);
			}
			if (Options::Menu.RageBotTab.resolver.GetIndex() > 1 && !GetAsyncKeyState(Options::Menu.RageBotTab.flip180.GetKey()))
			{
				force_freestanding(pEntity);
			}

			if (GetAsyncKeyState(Options::Menu.RageBotTab.flip180.GetKey()))
			{
				pEntity->GetEyeAnglesXY()->y -= 180;
			}
		//	if (Options::Menu.RageBotTab.resolver.GetIndex() > 1)
		//		ac.ac_smart(pEntity);				
		}
	}
}


