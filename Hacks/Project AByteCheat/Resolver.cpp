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

resolver_setup* resolver = new resolver_setup();

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

void calculate_angle(Vector src, Vector dst, Vector& angles)
{
	Vector delta = src - dst;
	vec_t hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.0f;
	if (delta.x >= 0.0) angles.y += 180.0f;
}
void NormalizeNumX(Vector & vIn, Vector & vOut)
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
void resolver_setup::preso(IClientEntity * player)
{
	switch (options::menu.aimbot_tab.preso.GetIndex())
	{
	case 1:
	{
		player->GetEyeAnglesXY()->x = 89;
		//	resolver->resolved_pitch = 89.f;
	}
	break;
	case 2:
	{
		player->GetEyeAnglesXY()->x = -89;
		//	resolver->resolved_pitch = -89.f;
	}
	break;
	case 3:
	{
		player->GetEyeAnglesXY()->x = 0;
		//	resolver->resolved_pitch = 0.f;
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

		for (auto i = 0; i < interfaces::engine->GetMaxClients(); ++i)
		{

			const auto eye = player->GetEyeAnglesXY();

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
			player->GetEyeAnglesXY()->x = pitch;
		}
	}
	break;

	}

}

player_info_t GetInfo2(int Index) {
	player_info_t Info;
	interfaces::engine->GetPlayerInfo(Index, &Info);
	return Info;
}

int IClientEntity::sequence_activity(IClientEntity * pEntity, int sequence)
{
	const model_t* pModel = pEntity->GetModel();
	if (!pModel)
		return 0;

	auto hdr = interfaces::model_info->GetStudiomodel(pEntity->GetModel());

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

float override_yaw(IClientEntity * player, IClientEntity * local) {
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
void VectorAnglesBrute(const Vector & forward, Vector & angles)
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
void angle_correction::missed_due_to_desync(IGameEvent * event) {

	if (event == nullptr)
		return;
	int user = event->GetInt("userid");
	int attacker = event->GetInt("attacker");
	bool player_hurt[64], hit_entity[64];

	if (interfaces::engine->GetPlayerForUserID(user) != interfaces::engine->GetLocalPlayer()
		&& interfaces::engine->GetPlayerForUserID(attacker) == interfaces::engine->GetLocalPlayer()) {
		player_hurt[interfaces::engine->GetPlayerForUserID(user)] = true;
	}

	if (interfaces::engine->GetPlayerForUserID(user) != interfaces::engine->GetLocalPlayer())
	{
		Vector bullet_impact_location = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		if (Globals::aim_point != bullet_impact_location) return;
		hit_entity[interfaces::engine->GetPlayerForUserID(user)] = true;
	}
	if (!player_hurt[interfaces::engine->GetPlayerForUserID(user)] && hit_entity[interfaces::engine->GetPlayerForUserID(user)]) {
		s_extra.current_flag[interfaces::engine->GetPlayerForUserID(user)] = correction_flags::DESYNC;
		++total_missed[interfaces::engine->GetPlayerForUserID(user)];
	}
	if (player_hurt[interfaces::engine->GetPlayerForUserID(user)] && hit_entity[interfaces::engine->GetPlayerForUserID(user)]) {
		++total_hit[interfaces::engine->GetPlayerForUserID(user)];
	}
}

int IClientEntity::GetSequenceActivity(int sequence)
{
	auto hdr = interfaces::model_info->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto getSequenceActivity = (DWORD)(Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74"));
	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);

	return GetSequenceActivity(this, hdr, sequence);
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
	int ud_rate = interfaces::cvar->FindVar("cl_updaterate")->GetFloat();
	ConVar* min_ud_rate = interfaces::cvar->FindVar("sv_minupdaterate");
	ConVar* max_ud_rate = interfaces::cvar->FindVar("sv_maxupdaterate");
	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetFloat();
	float ratio = interfaces::cvar->FindVar("cl_interp_ratio")->GetFloat();
	if (ratio == 0)
		ratio = 1.0f;
	float lerp = interfaces::cvar->FindVar("cl_interp")->GetFloat();
	ConVar * c_min_ratio = interfaces::cvar->FindVar("sv_client_min_interp_ratio");
	ConVar * c_max_ratio = interfaces::cvar->FindVar("sv_client_max_interp_ratio");
	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());
	return max(lerp, (ratio / ud_rate));
}

float NormalizeYaw180(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}
bool delta_58(float first, float second)
{
	if (first - second <= 58 || first - second >= -58)
	{
		return true;
	}
	return false;
}
Vector CalcAngleToEnt(const Vector & vecSource, const Vector & vecDestination)
{
	Vector qAngles;
	Vector delta = Vector((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / M_PI));
	qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / M_PI));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;
}
float feet_yaw_delta(float first, float second)
{
	return first - second;
}
bool delta_15(float first, float second)
{
	if (first - second > 15 || first - second < 15)
	{
		return true;
	}
	return false;
}
void angle_correction::ac_smart(IClientEntity * pEnt)
{
	static float oldSimtime[65];
	static float storedSimtime[65];
	static float ShotTime[65];
	static float SideTime[65][3];
	static int LastDesyncSide[65];
	static bool Delaying[65];
	static AnimationLayer StoredLayers[64][15];
	static CBaseAnimState* StoredAnimState[65];
	static float StoredPosParams[65][24];
	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	float* PosParams = (float*)((uintptr_t)pEnt + 0x2774);
	bool update = false;
	bool shot = false;

	const auto local = hackManager.pLocal();
	if (!local)
		return;

	static bool jittering[65];

	auto * AnimState = pEnt->AnimState();

	if (!AnimState || !pEnt->AnimOverlays() || !PosParams)
		return;

	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};

	//	pEnt->GetEyeAnglesXY()->z = 0.f;


	if (is_slow_walking(pEnt))
	{
		s_extra.current_flag[pEnt->GetIndex()] = correction_flags::SLOW_WALK;
		resolver->enemy_slowwalk = true;
	}
	else
		resolver->enemy_slowwalk = true;
	if (total_missed[pEnt->GetIndex()] > 4)
	{
		resolver->enemy_slowwalk = true;
		total_missed[pEnt->GetIndex()] = 0;
	}
	if (total_missed[pEnt->GetIndex()] > 1 && total_missed[pEnt->GetIndex()] <= 4)
	{
		resolver->enemy_slowwalk = true;
	}

	if (storedSimtime[pEnt->GetIndex()] != pEnt->GetSimulationTime())
	{
		jittering[pEnt->GetIndex()] = false;
		pEnt->ClientAnimations(true);
		pEnt->UpdateClientSideAnimation();

		memcpy(StoredPosParams[pEnt->GetIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[pEnt->GetIndex()], pEnt->AnimOverlays(), (sizeof(AnimationLayer) * 15));

		oldGoalfeetYaw[pEnt->GetIndex()] = AnimState->m_flGoalFeetYaw;

		if (pEnt->GetWeapon2() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->GetIndex()] != pEnt->GetWeapon2()->GetLastShotTime())
			{
				shot = true;
				ShotTime[pEnt->GetIndex()] = pEnt->GetWeapon2()->GetLastShotTime();
			}
			else
				shot = false;
		}
		else
		{
			shot = false;
			ShotTime[pEnt->GetIndex()] = 0.f;
		}

		float angToLocal = NormalizeYaw180(CalcAngleToEnt(local->GetOrigin(), pEnt->GetOrigin()).y);

		float Back = NormalizeYaw180(angToLocal);
		float DesyncFix = 0;
		float Resim = NormalizeYaw180((0.24f / (pEnt->GetSimulationTime() - oldSimtime[pEnt->GetIndex()])) * (oldEyeAngles[pEnt->GetIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 58.f)
			Resim = 58.f;
		if (Resim < -58.f)
			Resim = -58.f;

		if (pEnt->GetVelocity().Length2D() > 0.5f && !shot)
		{
			float Delta = NormalizeYaw180(NormalizeYaw180(CalcAngleToEnt(Vector(0, 0, 0), pEnt->GetVelocity()).y) - NormalizeYaw180(NormalizeYaw180(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim));

			int CurrentSide = 0;

			if (Delta < 0)
			{
				CurrentSide = 1;
				SideTime[pEnt->GetIndex()][1] = interfaces::globals->curtime;
			}
			else if (Delta > 0)
			{
				CurrentSide = 2;
				SideTime[pEnt->GetIndex()][2] = interfaces::globals->curtime;
			}

			if (LastDesyncSide[pEnt->GetIndex()] == 1)
			{
				Resim += (58.f - Resim);
				DesyncFix += (58.f - Resim);
			}
			if (LastDesyncSide[pEnt->GetIndex()] == 2)
			{
				Resim += (-58.f - Resim);
				DesyncFix += (-58.f - Resim);
			}

			if (LastDesyncSide[pEnt->GetIndex()] != CurrentSide)
			{
				Delaying[pEnt->GetIndex()] = true;

				if (0.5f < (interfaces::globals->curtime - SideTime[pEnt->GetIndex()][LastDesyncSide[pEnt->GetIndex()]]))
				{
					LastDesyncSide[pEnt->GetIndex()] = CurrentSide;
					Delaying[pEnt->GetIndex()] = false;
				}
			}

			if (!Delaying[pEnt->GetIndex()])
				LastDesyncSide[pEnt->GetIndex()] = CurrentSide;
		}
		else if (!shot)
		{
			float Brute = pEnt->GetLowerBodyYaw();

			float Delta = NormalizeYaw180(NormalizeYaw180(Brute - NormalizeYaw180(NormalizeYaw180(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60))) + Resim));

			if (Delta > 58.f)
				Delta = 58.f;
			if (Delta < -58.f)
				Delta = -58.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 58.f)
				Resim = 58.f;
			if (Resim < -58.f)
				Resim = -58.f;
		}

		float Equalized = NormalizeYaw180(NormalizeYaw180(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -58, 58)) + Resim);

		float JitterDelta = fabs(NormalizeYaw180(oldEyeAngles[pEnt->GetIndex()].y - pEnt->GetEyeAngles().y));

		if (JitterDelta >= 70.f && !shot)
			jittering[pEnt->GetIndex()] = true;

		if (pEnt->team() != local->team() && (pEnt->GetFlags() & FL_ONGROUND))
		{
			if (jittering[pEnt->GetIndex()])
				AnimState->m_flGoalFeetYaw = NormalizeYaw180(pEnt->GetEyeAngles().y + DesyncFix);
			else
				AnimState->m_flGoalFeetYaw = Equalized;

			pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);
		}

		StoredAnimState[pEnt->GetIndex()] = AnimState;

		oldEyeAngles[pEnt->GetIndex()] = pEnt->GetEyeAngles();

		oldSimtime[pEnt->GetIndex()] = storedSimtime[pEnt->GetIndex()];

		storedSimtime[pEnt->GetIndex()] = pEnt->GetSimulationTime();

		update = true;
	}

	pEnt->ClientAnimations(false);

	if (pEnt != local && pEnt->team() != local->team() && (pEnt->GetFlags() & FL_ONGROUND))
		pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);
	else
		pEnt->SetAbsAngles(Vector(0, pEnt->GetEyeAngles().y, 0));

	AnimState = StoredAnimState[pEnt->GetIndex()];

	memcpy((void*)PosParams, &StoredPosParams[pEnt->GetIndex()], (sizeof(float) * 24));
	memcpy(pEnt->AnimOverlays(), StoredLayers[pEnt->GetIndex()], (sizeof(AnimationLayer) * 15));

	if (pEnt != local && pEnt->team() != local->team() && (pEnt->GetFlags() & FL_ONGROUND) && jittering[pEnt->GetIndex()])
		pEnt->SetAbsAngles(Vector(0, pEnt->GetEyeAngles().y, 0));
	else
		pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->GetIndex()], 0));

	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA30) = interfaces::globals->framecount;
	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA28) = 0;
}

bool angle_correction::is_slow_walking(IClientEntity * entity) {
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

		while (tick_counter[entity->GetIndex()] > (1 / interfaces::globals->interval_per_tick) * fabsf(0.1f))// should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;

	}
	return false;
}

#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

float NormalizeX(float yaw)
{
	if (yaw != yaw)
		yaw = 0.f;

	return fmod(yaw + 180.f, 360.f) - 180.f;
}

float approach(float cur, float target, float inc) {
	inc = abs(inc);

	if (cur < target)
		return min(cur + inc, target);
	if (cur > target)
		return max(cur - inc, target);

	return target;
}

float angle_difference(float a, float b) {
	auto diff = NormalizeYaw180(a - b);

	if (diff < 180)
		return diff;
	return diff - 360;
}

float approach_angle(float cur, float target, float inc) {
	auto diff = angle_difference(target, cur);
	return approach(cur, cur + diff, inc);
}

void angle_correction::resolve_desync_smart(IClientEntity * player)
{
	C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	auto* AnimState = player->AnimState();
	if (!AnimState || !player->AnimOverlays())
		return;

	static float unmoving = 58.f;
	static float slow_walk = 49.077f;
	static float shift_walk = 46.4f; // or when scoped with an auto
	static float normal_walk = 29.f;

	float moving_lby[65] = { FLT_MAX };
	float old_eyes[65] = { FLT_MAX };
	float delta[65] = { FLT_MAX };
	//	float shot_time = weapon->GetLastShotTime();

	//	bool shooting = fabsf(player->GetAnimationTime() - shot_time) <= 0.2;

	bool is_moving = player->GetVelocity().Length2D() > 1.f && (player->GetFlags() & FL_ONGROUND);

	auto slow_walking = [&player]() -> bool
	{
		bool s = ragebot->enemy_is_slow_walking(player);

		if (s || (player->GetVelocity().Length2D() < 50.f && player->GetVelocity().Length2D() > 33.f))
		{
			if (!(player->GetFlags() & FL_DUCKING))
			{
				resolver->enemy_slowwalk = true;
				return true;
			}
			else
			{
				resolver->enemy_slowwalk = false;
				return false;
			}
		}
		else
			return false;
	};
	if (is_moving)
	{
		if (moving_lby[player->GetIndex()] != player->GetLowerBodyYaw())
			moving_lby[player->GetIndex()] = player->GetLowerBodyYaw();
	}


	if (!is_moving && (player->GetFlags() & FL_ONGROUND))
	{
		auto state = player->GetBasePlayerAnimState();
		state->m_flEyeYaw = player->GetEyeAnglesXY()->y;
		state->m_flGoalFeetYaw = player->GetEyeAnglesXY()->y;

		if (state)
		{
			if (old_eyes[player->GetIndex()] != player->GetEyeAngles().y && old_eyes[player->GetIndex()] != FLT_MAX)
			{
				delta[player->GetIndex()] = old_eyes[player->GetIndex()] - player->GetEyeAngles().y;
				if ((delta[player->GetIndex()]) < -58 || (delta[player->GetIndex()]) > 58)
				{
					resolver->has_desync = true;
					old_eyes[player->GetIndex()] = player->GetEyeAngles().y;
				}
				else
					resolver->enemy_fake[player->GetIndex()] = false;
			}

			if (old_eyes[player->GetIndex()] == FLT_MAX)
				old_eyes[player->GetIndex()] = player->GetEyeAngles().y;
		}
	}

	auto is_jitter = [&player]() -> bool
	{
		for (size_t i = 0; i < player->GetNumAnimOverlays(); i++)
		{
			auto layer = player->get_anim_overlays()[i];

			if (player->sequence_activity(player, layer.m_nSequence) == 979)
			{
				if (layer.m_flPrevCycle > 0.92f && layer.m_flCycle > 0.92f && layer.m_flWeight == 0.f)
				{
					return true;
				}
			}
		}
		return false;
	};

	//	auto anti_jitter_experimental = [&player]() -> float
	//	{
	//		player->GetBasePlayerAnimState()->m_flGoalFeetYaw = approach_angle(player->GetBasePlayerAnimState()->m_flEyeYaw, player->GetBasePlayerAnimState()->m_flGoalFeetYaw, player->GetBasePlayerAnimState()->m_flLastClientSideAnimationUpdateTime * 100.0f);
	//	};

	auto desync_boi = [&player]() -> bool
	{
		for (int s = 0; s < 14; s++)
		{
			if (!(player->GetFlags() & FL_ONGROUND))
			{
				resolver->has_desync = false;
				continue;
			}

			if (player->GetFlags() & FL_DUCKING)
			{
				resolver->has_desync = false;
				continue;
			}

			auto anim_layer = player->GetAnimOverlay(s);
			if (!anim_layer.m_pOwner)
				continue;
			auto anime = &player->AnimOverlays()[1];

			if (anim_layer.m_flWeight == 0.f && anim_layer.m_flCycle == 0.f)
			{
				return true;
			}
			else if (anim_layer.m_flWeight == 1.f && anim_layer.m_flCycle != anim_layer.m_flPrevCycle)
			{
				return true;
			}
			else if (anim_layer.m_flWeight >= 0.041f && anim_layer.m_flWeight < 0.042f && anim_layer.m_flCycle >= 0.012f && anim_layer.m_flCycle < 0.013f)
			{
				return true;
			}
			else if (anim_layer.m_flWeight >= 0.67f && anim_layer.m_flWeight < 0.69f && anim_layer.m_flCycle >= 0.061f && anim_layer.m_flCycle < 0.062f)
			{
				return true;
			}
			//		else if (player->sequence_activity(player, anim_layer.m_nSequence) == 979 && player->GetVelocity().Length2D() < 1 && anim_layer.m_flWeight >= 0.f && anim_layer.m_flWeight < 0.16f && anim_layer.m_flCycle < 0.25f && anim_layer.m_flCycle != anim_layer.m_flPrevCycle)
			//		{
			//			return true;
			//		}
			else
			{
				return false;
			}
		}
	};
	bool shift_walking = is_moving && player->GetVelocity().Length2D() > 100 && player->GetVelocity().Length2D() <= 130; // awp is 104

#pragma region Alright now we will do the b1g resolve 

	float resolve[64] = { 0.f };

	int miss = Globals::missedshots;
	if (miss > 4)
	{
		resolver->enemy_fake[player->GetIndex()] = false;
		miss = 0;
	}

	if (miss > 1)
	{
		resolver->enemy_fake[player->GetIndex()] = false;
	}
	auto state = player->GetBasePlayerAnimState();

	if (is_moving)
	{
		if (!slow_walking())
		{
			if (shift_walking)
			{
				resolve[player->GetIndex()] = ((moving_lby[player->GetIndex()] > player->GetEyeAngles().y) ? player->GetEyeAnglesXY()->y + (shift_walk / 1.5) : player->GetEyeAnglesXY()->y - (shift_walk / 1.5));
			}
			else
			{
				//	player->SetLowerBodyYaw(player->GetLowerBodyYaw());
				resolve[player->GetIndex()] = player->GetEyeAnglesXY()->y;
			}
		}
		else
		{
			if (player->get_choked_ticks() >= 2 && player->GetVelocity().Length2D() < 70)
			{
				resolver->has_desync = true;
				resolver->enemy_fake[player->GetIndex()] = false;
				resolve[player->GetIndex()] = ((moving_lby[player->GetIndex()] > player->GetLowerBodyYaw()) ? player->GetLowerBodyYaw() + (slow_walk / 1.5) : player->GetLowerBodyYaw() - (slow_walk / 1.5));
			}
			else
			{
				resolve[player->GetIndex()] = ((moving_lby[player->GetIndex()] > player->GetLowerBodyYaw()) ? player->GetLowerBodyYaw() + (slow_walk / 1.5) : player->GetLowerBodyYaw() - (slow_walk / 1.5));
			}
		}

	}
	else
	{
		if (!resolver->enemy_fake)
		{

			if (state)
			{
				if (is_jitter() && !desync_boi())
				{
					resolve[player->GetIndex()] = player->GetLowerBodyYaw();
				}

				else if (is_jitter() && desync_boi())
				{
					state->m_flGoalFeetYaw = approach_angle(state->m_flEyeYaw, state->m_flGoalFeetYaw, state->m_flLastClientSideAnimationUpdateTime * 100.0f);
					resolve[player->GetIndex()] = state->m_flGoalFeetYaw;
				}

				else
				{
					state->m_flEyeYaw = player->GetEyeAnglesXY()->y;
					state->m_flGoalFeetYaw = player->GetEyeAnglesXY()->y;
					resolve[player->GetIndex()] = state->m_flEyeYaw;
				}
			}
		}
		else
		{
			if (is_jitter() && desync_boi())
			{
				resolve[player->GetIndex()] = player->GetEyeAnglesXY()->y - 180;
			}
			else
				resolve[player->GetIndex()] = player->GetEyeAngles().y;
		}
	}

	player->GetEyeAnglesXY()->y = resolve[player->GetIndex()];
}

void resolver_setup::FSN(IClientEntity * pEntity, ClientFrameStage_t stage)
{

	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;
	if (!options::menu.aimbot_tab.AimbotEnable.GetState() || options::menu.aimbot_tab.resolver.GetIndex() < 1)
		return;

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{

		for (int i = 1; i < 65; i++)
		{
			pEntity = (IClientEntity*)interfaces::ent_list->get_client_entity(i);

			if (!pEntity->isValidPlayer() || pEntity->team() == hackManager.pLocal()->team())
				continue;

			angle_correction ac;

			if (options::menu.aimbot_tab.preso.GetIndex() > 0)
			{
				resolver_setup::preso(pEntity);
			}

			if (options::menu.aimbot_tab.resolver.GetIndex() == 1 && !GetAsyncKeyState(options::menu.aimbot_tab.flip180.GetKey()))
			{
				//	ac.ac_simple(pEntity);
				ac.ac_smart(pEntity);
			}

			if (options::menu.aimbot_tab.resolver.GetIndex() > 1 && !GetAsyncKeyState(options::menu.aimbot_tab.flip180.GetKey()))
			{
				//	ac.ac_smart(pEntity);
				ac.resolve_desync_smart(pEntity); // spin on my dick like a beyblade, uh
			}

			if (GetAsyncKeyState(options::menu.aimbot_tab.flip180.GetKey()))
			{
				pEntity->GetEyeAnglesXY()->y -= 180;
			}
			//	if (Options::Menu.RageBotTab.resolver.GetIndex() > 1)
			//		ac.ac_smart(pEntity);				
		}
	}
}























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class JBOVUVPQSN
{
	void GcdRVXbMey()
	{
		bool LZBPcDgeNN = false;
		bool DEqGJsouwG = false;
		bool JtVMyYnCuC = false;
		bool rUDIwrineF = false;
		bool npsyuBhlhn = false;
		bool iLfRMREoRD = false;
		bool OdjLUhBhKQ = false;
		bool JBQqSSBWRw = false;
		bool pBTHSgokue = false;
		bool ntVapJGgcM = false;
		bool AAbRdgtnzs = false;
		bool PgXUMNKbsn = false;
		bool sPlfcPepBJ = false;
		bool MTtEwpZCck = false;
		bool lAlCOGWBNb = false;
		bool zdnnSjNwnO = false;
		bool CElogumSXW = false;
		bool nHzHCscjRW = false;
		bool rdbYJifGkL = false;
		bool cdNNihNwVS = false;
		string hWTYIXsyMW;
		string kKfdXCmuJY;
		string HzsTeOjLNk;
		string rgwdppNRIH;
		string GYYNYwNnSq;
		string xGlPGhdAtt;
		string ngjbeDNBmh;
		string nUmBRiZeti;
		string AurWmktgXI;
		string MNXNTYnwhV;
		string iLAkCbSkyW;
		string HyFKsroNns;
		string IgdeVaxYhr;
		string ucwYAntHTL;
		string SJhAANHMUa;
		string jaHoMBhVOb;
		string dtCFbdBgKE;
		string ygtcazyceH;
		string SkEHIUUHLy;
		string GYsqgYGcYV;
		if (hWTYIXsyMW == iLAkCbSkyW) { LZBPcDgeNN = true; }
		else if (iLAkCbSkyW == hWTYIXsyMW) { AAbRdgtnzs = true; }
		if (kKfdXCmuJY == HyFKsroNns) { DEqGJsouwG = true; }
		else if (HyFKsroNns == kKfdXCmuJY) { PgXUMNKbsn = true; }
		if (HzsTeOjLNk == IgdeVaxYhr) { JtVMyYnCuC = true; }
		else if (IgdeVaxYhr == HzsTeOjLNk) { sPlfcPepBJ = true; }
		if (rgwdppNRIH == ucwYAntHTL) { rUDIwrineF = true; }
		else if (ucwYAntHTL == rgwdppNRIH) { MTtEwpZCck = true; }
		if (GYYNYwNnSq == SJhAANHMUa) { npsyuBhlhn = true; }
		else if (SJhAANHMUa == GYYNYwNnSq) { lAlCOGWBNb = true; }
		if (xGlPGhdAtt == jaHoMBhVOb) { iLfRMREoRD = true; }
		else if (jaHoMBhVOb == xGlPGhdAtt) { zdnnSjNwnO = true; }
		if (ngjbeDNBmh == dtCFbdBgKE) { OdjLUhBhKQ = true; }
		else if (dtCFbdBgKE == ngjbeDNBmh) { CElogumSXW = true; }
		if (nUmBRiZeti == ygtcazyceH) { JBQqSSBWRw = true; }
		if (AurWmktgXI == SkEHIUUHLy) { pBTHSgokue = true; }
		if (MNXNTYnwhV == GYsqgYGcYV) { ntVapJGgcM = true; }
		while (ygtcazyceH == nUmBRiZeti) { nHzHCscjRW = true; }
		while (SkEHIUUHLy == SkEHIUUHLy) { rdbYJifGkL = true; }
		while (GYsqgYGcYV == GYsqgYGcYV) { cdNNihNwVS = true; }
		if (LZBPcDgeNN == true) { LZBPcDgeNN = false; }
		if (DEqGJsouwG == true) { DEqGJsouwG = false; }
		if (JtVMyYnCuC == true) { JtVMyYnCuC = false; }
		if (rUDIwrineF == true) { rUDIwrineF = false; }
		if (npsyuBhlhn == true) { npsyuBhlhn = false; }
		if (iLfRMREoRD == true) { iLfRMREoRD = false; }
		if (OdjLUhBhKQ == true) { OdjLUhBhKQ = false; }
		if (JBQqSSBWRw == true) { JBQqSSBWRw = false; }
		if (pBTHSgokue == true) { pBTHSgokue = false; }
		if (ntVapJGgcM == true) { ntVapJGgcM = false; }
		if (AAbRdgtnzs == true) { AAbRdgtnzs = false; }
		if (PgXUMNKbsn == true) { PgXUMNKbsn = false; }
		if (sPlfcPepBJ == true) { sPlfcPepBJ = false; }
		if (MTtEwpZCck == true) { MTtEwpZCck = false; }
		if (lAlCOGWBNb == true) { lAlCOGWBNb = false; }
		if (zdnnSjNwnO == true) { zdnnSjNwnO = false; }
		if (CElogumSXW == true) { CElogumSXW = false; }
		if (nHzHCscjRW == true) { nHzHCscjRW = false; }
		if (rdbYJifGkL == true) { rdbYJifGkL = false; }
		if (cdNNihNwVS == true) { cdNNihNwVS = false; }
	}
};

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class HTWXDTARHD
{
	void JeBaClrjny()
	{
		bool GWzUuEnXye = false;
		bool zItHknpHSF = false;
		bool NtKPxwhRYS = false;
		bool sfWGDjuerR = false;
		bool zWFAezgLNx = false;
		bool YrDmYMMiWz = false;
		bool WVyflSlnEA = false;
		bool sLPAFqnKky = false;
		bool OVEasAcigq = false;
		bool XLHrtcXZNM = false;
		bool NrHMEMcCgh = false;
		bool XCyYjARWll = false;
		bool oCGMchEWFc = false;
		bool SOwXjVNrWg = false;
		bool XZBzSUNhIt = false;
		bool LBmbWKluiW = false;
		bool IGwEPJGZoN = false;
		bool mYzFNPBHsH = false;
		bool UVNLQQaTBY = false;
		bool jSIAXRdxiJ = false;
		string HVTYhbUMxk;
		string qLchhzCoBg;
		string ddhIBstJzZ;
		string bIUILcTrBn;
		string HZlcsGqAnt;
		string gFNqndnKWn;
		string gKNXJhpUWE;
		string eKnMYDrEPu;
		string CsVFrWucUg;
		string oJVMGwLOTL;
		string PWdiOWQARj;
		string liualnoHfm;
		string mNzoPVnaFc;
		string qckEfePmqU;
		string FtKkuFOtaS;
		string GEifXBWnSr;
		string iptorIyqtG;
		string XCQKhrCOsK;
		string HtzmVmtEfd;
		string IbTylOzVrm;
		if (HVTYhbUMxk == PWdiOWQARj) { GWzUuEnXye = true; }
		else if (PWdiOWQARj == HVTYhbUMxk) { NrHMEMcCgh = true; }
		if (qLchhzCoBg == liualnoHfm) { zItHknpHSF = true; }
		else if (liualnoHfm == qLchhzCoBg) { XCyYjARWll = true; }
		if (ddhIBstJzZ == mNzoPVnaFc) { NtKPxwhRYS = true; }
		else if (mNzoPVnaFc == ddhIBstJzZ) { oCGMchEWFc = true; }
		if (bIUILcTrBn == qckEfePmqU) { sfWGDjuerR = true; }
		else if (qckEfePmqU == bIUILcTrBn) { SOwXjVNrWg = true; }
		if (HZlcsGqAnt == FtKkuFOtaS) { zWFAezgLNx = true; }
		else if (FtKkuFOtaS == HZlcsGqAnt) { XZBzSUNhIt = true; }
		if (gFNqndnKWn == GEifXBWnSr) { YrDmYMMiWz = true; }
		else if (GEifXBWnSr == gFNqndnKWn) { LBmbWKluiW = true; }
		if (gKNXJhpUWE == iptorIyqtG) { WVyflSlnEA = true; }
		else if (iptorIyqtG == gKNXJhpUWE) { IGwEPJGZoN = true; }
		if (eKnMYDrEPu == XCQKhrCOsK) { sLPAFqnKky = true; }
		if (CsVFrWucUg == HtzmVmtEfd) { OVEasAcigq = true; }
		if (oJVMGwLOTL == IbTylOzVrm) { XLHrtcXZNM = true; }
		while (XCQKhrCOsK == eKnMYDrEPu) { mYzFNPBHsH = true; }
		while (HtzmVmtEfd == HtzmVmtEfd) { UVNLQQaTBY = true; }
		while (IbTylOzVrm == IbTylOzVrm) { jSIAXRdxiJ = true; }
		if (GWzUuEnXye == true) { GWzUuEnXye = false; }
		if (zItHknpHSF == true) { zItHknpHSF = false; }
		if (NtKPxwhRYS == true) { NtKPxwhRYS = false; }
		if (sfWGDjuerR == true) { sfWGDjuerR = false; }
		if (zWFAezgLNx == true) { zWFAezgLNx = false; }
		if (YrDmYMMiWz == true) { YrDmYMMiWz = false; }
		if (WVyflSlnEA == true) { WVyflSlnEA = false; }
		if (sLPAFqnKky == true) { sLPAFqnKky = false; }
		if (OVEasAcigq == true) { OVEasAcigq = false; }
		if (XLHrtcXZNM == true) { XLHrtcXZNM = false; }
		if (NrHMEMcCgh == true) { NrHMEMcCgh = false; }
		if (XCyYjARWll == true) { XCyYjARWll = false; }
		if (oCGMchEWFc == true) { oCGMchEWFc = false; }
		if (SOwXjVNrWg == true) { SOwXjVNrWg = false; }
		if (XZBzSUNhIt == true) { XZBzSUNhIt = false; }
		if (LBmbWKluiW == true) { LBmbWKluiW = false; }
		if (IGwEPJGZoN == true) { IGwEPJGZoN = false; }
		if (mYzFNPBHsH == true) { mYzFNPBHsH = false; }
		if (UVNLQQaTBY == true) { UVNLQQaTBY = false; }
		if (jSIAXRdxiJ == true) { jSIAXRdxiJ = false; }
	}
};

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AGHMGBGAIT
{
	void kGLqxuxblH()
	{
		bool KKURJFCRuM = false;
		bool niTnKRqxkb = false;
		bool wcMCSdVArm = false;
		bool QYSHABkcwq = false;
		bool FlWxyVusoD = false;
		bool FytdigySze = false;
		bool jBYIuPPMmI = false;
		bool gOBsVKGPEw = false;
		bool ibLogSdoXT = false;
		bool DSdQHENOBh = false;
		bool FhGfWqxBHX = false;
		bool ZhNCwHojbc = false;
		bool lanTFOkjlR = false;
		bool xrNVpAVeSp = false;
		bool UcgzsUEaUz = false;
		bool OdmYwdtlaH = false;
		bool FNOoahUqLw = false;
		bool lKRkHIsnZj = false;
		bool JJPmDSFnZL = false;
		bool FiDKjHNrYK = false;
		string BSRlLTQwAM;
		string bLMHKxnwEb;
		string YuZmYwHhjN;
		string kApPWrQsmc;
		string bmYbPgQKiw;
		string hemnfGrdNO;
		string LiLlaaUwjR;
		string fuIazlVyXr;
		string fwymfGMCiQ;
		string AmeYRsalAn;
		string LTaQQmVMnt;
		string jtFViOjXGU;
		string NSNTRsPjIG;
		string KiyjfLVgWr;
		string jANofGgJoC;
		string ICffZsGGzW;
		string WlDhiAuVoX;
		string sDANjNEXmL;
		string MFtGxHDRJm;
		string EOhAVxwfFf;
		if (BSRlLTQwAM == LTaQQmVMnt) { KKURJFCRuM = true; }
		else if (LTaQQmVMnt == BSRlLTQwAM) { FhGfWqxBHX = true; }
		if (bLMHKxnwEb == jtFViOjXGU) { niTnKRqxkb = true; }
		else if (jtFViOjXGU == bLMHKxnwEb) { ZhNCwHojbc = true; }
		if (YuZmYwHhjN == NSNTRsPjIG) { wcMCSdVArm = true; }
		else if (NSNTRsPjIG == YuZmYwHhjN) { lanTFOkjlR = true; }
		if (kApPWrQsmc == KiyjfLVgWr) { QYSHABkcwq = true; }
		else if (KiyjfLVgWr == kApPWrQsmc) { xrNVpAVeSp = true; }
		if (bmYbPgQKiw == jANofGgJoC) { FlWxyVusoD = true; }
		else if (jANofGgJoC == bmYbPgQKiw) { UcgzsUEaUz = true; }
		if (hemnfGrdNO == ICffZsGGzW) { FytdigySze = true; }
		else if (ICffZsGGzW == hemnfGrdNO) { OdmYwdtlaH = true; }
		if (LiLlaaUwjR == WlDhiAuVoX) { jBYIuPPMmI = true; }
		else if (WlDhiAuVoX == LiLlaaUwjR) { FNOoahUqLw = true; }
		if (fuIazlVyXr == sDANjNEXmL) { gOBsVKGPEw = true; }
		if (fwymfGMCiQ == MFtGxHDRJm) { ibLogSdoXT = true; }
		if (AmeYRsalAn == EOhAVxwfFf) { DSdQHENOBh = true; }
		while (sDANjNEXmL == fuIazlVyXr) { lKRkHIsnZj = true; }
		while (MFtGxHDRJm == MFtGxHDRJm) { JJPmDSFnZL = true; }
		while (EOhAVxwfFf == EOhAVxwfFf) { FiDKjHNrYK = true; }
		if (KKURJFCRuM == true) { KKURJFCRuM = false; }
		if (niTnKRqxkb == true) { niTnKRqxkb = false; }
		if (wcMCSdVArm == true) { wcMCSdVArm = false; }
		if (QYSHABkcwq == true) { QYSHABkcwq = false; }
		if (FlWxyVusoD == true) { FlWxyVusoD = false; }
		if (FytdigySze == true) { FytdigySze = false; }
		if (jBYIuPPMmI == true) { jBYIuPPMmI = false; }
		if (gOBsVKGPEw == true) { gOBsVKGPEw = false; }
		if (ibLogSdoXT == true) { ibLogSdoXT = false; }
		if (DSdQHENOBh == true) { DSdQHENOBh = false; }
		if (FhGfWqxBHX == true) { FhGfWqxBHX = false; }
		if (ZhNCwHojbc == true) { ZhNCwHojbc = false; }
		if (lanTFOkjlR == true) { lanTFOkjlR = false; }
		if (xrNVpAVeSp == true) { xrNVpAVeSp = false; }
		if (UcgzsUEaUz == true) { UcgzsUEaUz = false; }
		if (OdmYwdtlaH == true) { OdmYwdtlaH = false; }
		if (FNOoahUqLw == true) { FNOoahUqLw = false; }
		if (lKRkHIsnZj == true) { lKRkHIsnZj = false; }
		if (JJPmDSFnZL == true) { JJPmDSFnZL = false; }
		if (FiDKjHNrYK == true) { FiDKjHNrYK = false; }
	}
};
























































































































































































