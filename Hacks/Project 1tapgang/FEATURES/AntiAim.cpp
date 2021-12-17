#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/AutoWall.h"
#include "../SDK/NetChannel.h"
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
bool next_lby_update(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = INTERFACES::Globals->curtime;
	local_update = next_lby_update_time;

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f && !fake_walk)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

float fov_player(Vector ViewOffSet, Vector View, SDK::CBaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = aimbot->get_hitbox_pos(entity, hitbox);

	MATH::AngleVectors(Angles, &Forward);
	MATH::VectorSubtract(AimPos, Origin, Delta);
	MATH::NormalizeNum(Delta, Delta);

	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}


int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

float best_head_angle(float yaw)
{
	float Back, Right, Left;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return 0.f;

	Vector src3D, dst3D, forward, right, up, src, dst;
	SDK::trace_t tr;
	SDK::Ray_t ray, ray2, ray3, ray4, ray5;
	SDK::CTraceFilter filter;

	Vector engineViewAngles;
	INTERFACES::Engine->GetViewAngles(engineViewAngles);

	engineViewAngles.x = 0;

	MATH::AngleVectors2(engineViewAngles, &forward, &right, &up);

	filter.pSkip1 = local_player;
	src3D = local_player->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	INTERFACES::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.end - tr.start).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	INTERFACES::Trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.end - tr.start).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	INTERFACES::Trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.end - tr.start).Length();

	if (Left > Right)
	{
		return (yaw - 90);
	}
	else if (Right > Left)
	{
		return (yaw + 90);
	}
	else if (Back > Right || Back > Left)
	{
		return (yaw - 180);
	}
	return 0;


}
bool CAntiAim::IsTargettingLocal(SDK::CBaseEntity* local, SDK::CBaseEntity* entity)
{

	Vector src, dst, forward;
	SDK::trace_t tr;

	if (!this || !local || local->GetHealth() < 0)
		return false;

	Vector viewangle = entity->GetEyeAngles();

	MATH::AngleVectors(viewangle, &forward);
	forward *= 8142.f;
	src = entity->GetEyePosition();
	dst = src + forward;
	SDK::Ray_t ray;
	ray.Init(src, dst);
	SDK::CTraceEntity filter;
	filter.pSkip1 = local;

	INTERFACES::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);


	if (tr.m_pEnt && tr.hitGroup == HITGROUP_HEAD && tr.m_pEnt->GetTeam() != entity->GetTeam()/*== G::LocalPlayer*/)
		return true;

	return false;
}
void CAntiAim::freestand(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;
	Vector Angles;
	INTERFACES::Engine->GetViewAngles(Angles);

	if (GLOBAL::should_send_packet)
	{
		cmd->viewangles.y = best_head_angle(Angles.y) + local_player->get_max_desync_delta(local_player);
		cmd->viewangles.y = best_head_angle(Angles.y) - local_player->get_max_desync_delta(local_player);
	}
	else
		cmd->viewangles.y = best_head_angle(Angles.y);
}

void minimal(SDK::CUserCmd *cmd)
{
	auto local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local) return;

	if (local->GetAnimState()->m_bInHitGroundAnimation)
	{
		if (local->GetAnimState()->m_flHeadHeightOrOffsetFromHittingGroundAnimation)
			cmd->viewangles.x = 0.000000;
	}
	else
		cmd->viewangles.x = 89.000000;
}

static bool left, right, back, frees = true;
void manual(SDK::CUserCmd* cmd)
{
	auto local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local)
		return;

	if (GetAsyncKeyState(SETTINGS::settings.aa_left))
	{
		left = true;
		right = false;
		back = false;
	}
	else if (GetAsyncKeyState(SETTINGS::settings.aa_right))
	{
		left = false;
		right = true;
		back = false;
	}
	else if (GetAsyncKeyState(SETTINGS::settings.aa_back))
	{
		left = false;
		right = false;
		back = true;
	}

	if (left)
		cmd->viewangles.y += 90;
	else if (right)
		cmd->viewangles.y -= 90;
	else if (back)
		cmd->viewangles.y -= 180;
}

void CAntiAim::do_antiaim(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return; auto grenade = (SDK::CBaseCSGrenade*)weapon;
	if (local_player->GetMoveType() == SDK::MOVETYPE_LADDER) return;
	if (cmd->buttons & IN_USE) return;
	if (weapon->is_grenade()) return;

	if (cmd->buttons & IN_ATTACK && aimbot->can_shoot(cmd)) return;
	if (weapon->get_full_info()->m_WeaponType == 9) return;
	if (!SETTINGS::settings.aa_bool) return;
	if (GLOBAL::DisableAA) return;

	static float last_real; int local_flags = local_player->GetFlags();

	switch (SETTINGS::settings.aa_pitch_type)
	{
	case 0: break;
	case 1: cmd->viewangles.x = 89.f; break;
	case 2: minimal(cmd); break;
	}

	static float RealAng = 0;

	switch (SETTINGS::settings.aa_real_type)
	{
	case 1: cmd->viewangles.y += 180; break;
	case 2: cmd->viewangles.y += 180 + UTILS::RandomFloat(25, -25); break;
	case 3: {

		static float asd = 120;
		asd += 5;
		if (asd > 240)
			asd = 120;

		cmd->viewangles.y += asd;
	}
			break;
	case 4: manual(cmd); break;
	case 5: freestand(cmd); break;
	}

	RealAng = cmd->viewangles.y;

	static bool jitter = false;
	if (SETTINGS::settings.desync_aa)
	{
		if (GLOBAL::should_send_packet)
		{
			float server_time = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;
			float time = TIME_TO_TICKS(server_time);

			while (time >= server_time)
				time = 0.f;

			float idk = rand() % 100;

			jitter = !jitter;
			if (time >= server_time / 2)
			{
				if (idk < 70)
				{
					if (!jitter)
						cmd->viewangles.y = RealAng + 55;

				}
				else
				{
					if (!jitter)
						cmd->viewangles.y = RealAng - 55;

				}
			}
			else
			{
				if (idk < 70)
				{
					if (jitter)
						cmd->viewangles.y = RealAng - SETTINGS::settings.desync_range;
				}
				else
				{
					if (jitter)
						cmd->viewangles.y = RealAng + SETTINGS::settings.desync_range;

				}
			}
		}
	}
}

void CAntiAim::fix_movement(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (SETTINGS::settings.aa_pitch > 1)
	{
		cmd->forwardmove = clamp(cmd->forwardmove, -450.f, 450.f);
		cmd->sidemove = clamp(cmd->sidemove, -450.f, 450.f);
		cmd->upmove = clamp(cmd->upmove, -320.f, 320.f);

		cmd->viewangles.x = clamp(cmd->viewangles.x, -89.f, 89.f);
	}

	Vector real_viewangles;
	INTERFACES::Engine->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	MATH::VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->forwardmove = cos(yaw) * speed;
	cmd->sidemove = sin(yaw) * speed;

	cmd->viewangles = MATH::NormalizeAngle(cmd->viewangles);

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->forwardmove *= -1;
}

CAntiAim* antiaim = new CAntiAim();