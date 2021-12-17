#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../UTILS/render.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../SDK/AnimLayer.h"
#include "../UTILS/qangle.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/override.h"

void override::override_player(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (!entity)
		return;

	if ((SETTINGS::settings.override && GetAsyncKeyState(SETTINGS::settings.overridekey)))
	{
		Vector viewangles; INTERFACES::Engine->GetViewAngles(viewangles);
		auto at_target_yaw = UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y;

		auto delta = MATH::NormalizeYaw(viewangles.y - at_target_yaw);
		auto rightDelta = Vector(entity->GetEyeAngles().x, at_target_yaw + 180, entity->GetEyeAngles().z); 
		auto leftDelta = Vector(entity->GetEyeAngles().x, at_target_yaw - 180, entity->GetEyeAngles().z);
	
		if (delta > 0)
			entity->SetEyeAngles(rightDelta);
		else
			entity->SetEyeAngles(leftDelta);
		return;
	}
}

void override::record(SDK::CBaseEntity* entity, float new_yaw)
{
	if (entity->GetVelocity().Length2D() > 36) return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
	if (!c_baseweapon) return;

	auto &info = player_info[entity->GetIndex()];
	if (entity->GetActiveWeaponIndex() && info.last_ammo < c_baseweapon->GetLoadedAmmo()) {
		info.last_ammo = c_baseweapon->GetLoadedAmmo();
		return;
	}

	info.unresolved_yaw.insert(info.unresolved_yaw.begin(), new_yaw);
	if (info.unresolved_yaw.size() > 20) info.unresolved_yaw.pop_back();
	if (info.unresolved_yaw.size() < 2) return;

	auto average_unresolved_yaw = 0;
	for (auto val : info.unresolved_yaw)
		average_unresolved_yaw += val;
	average_unresolved_yaw /= info.unresolved_yaw.size();

	int delta = average_unresolved_yaw - entity->GetLowerBodyYaw();
	auto big_math_delta = abs((((delta + 180) % 360 + 360) % 360 - 180));

	info.lby_deltas.insert(info.lby_deltas.begin(), big_math_delta);
	if (info.lby_deltas.size() > 10) {
		info.lby_deltas.pop_back();
	}
}
void override::resolve(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!entity) return;
	if (!local_player) return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;
	if (is_local_player) return;
	if (is_teammate) return;
	if (entity->GetHealth() <= 0) return;
	if (local_player->GetHealth() <= 0) return;

	auto &info = player_info[entity->GetIndex()];
	float fl_lby = entity->GetLowerBodyYaw();

	info.lby = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw(), 0.f);
	info.inverse = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() + 180.f, 0.f);
	info.last_lby = Vector(entity->GetEyeAngles().x, info.last_moving_lby, 0.f);
	info.inverse_left = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() + 115.f, 0.f);
	info.inverse_right = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() - 115.f, 0.f);

	info.back = Vector(entity->GetEyeAngles().x, UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y + 180.f, 0.f);
	info.right = Vector(entity->GetEyeAngles().x, UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y + 70.f, 0.f);
	info.left = Vector(entity->GetEyeAngles().x, UTILS::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y - 70.f, 0.f);

	info.backtrack = Vector(entity->GetEyeAngles().x, lby_to_back[entity->GetIndex()], 0.f);

	shots_missed[entity->GetIndex()] = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()];

	if (SETTINGS::settings.fakefix_bool) info.is_moving = entity->GetVelocity().Length2D() > 0.1 && entity->GetFlags() & FL_ONGROUND && !info.could_be_slowmo;
	else info.is_moving = entity->GetVelocity().Length2D() > 0.1 && entity->GetFlags() & FL_ONGROUND;

	info.is_jumping = !entity->GetFlags() & FL_ONGROUND;
	info.could_be_slowmo = entity->GetVelocity().Length2D() > 6 && entity->GetVelocity().Length2D() < 36 && !info.is_crouching;
	info.is_crouching = entity->GetFlags() & FL_DUCKING;
	update_time[entity->GetIndex()] = info.next_lby_update_time;

	static float old_simtime[65];
	if (entity->GetSimTime() != old_simtime[entity->GetIndex()])
	{
		using_fake_angles[entity->GetIndex()] = entity->GetSimTime() - old_simtime[entity->GetIndex()] == INTERFACES::Globals->interval_per_tick;
		old_simtime[entity->GetIndex()] = entity->GetSimTime();
	}

	if (!using_fake_angles[entity->GetIndex()])
	{
		if (backtrack_tick[entity->GetIndex()])
		{
			resolve_type[entity->GetIndex()] = 7;
			entity->SetEyeAngles(info.backtrack);
		}
		else if (info.stored_lby != entity->GetLowerBodyYaw())
		{
			entity->SetEyeAngles(info.lby);
			info.stored_lby = entity->GetLowerBodyYaw();
			resolve_type[entity->GetIndex()] = 3;
		}
		else if (info.is_moving)
		{
			entity->SetEyeAngles(info.lby);
			info.last_moving_lby = entity->GetLowerBodyYaw();
			info.stored_missed = shots_missed[entity->GetIndex()];
			resolve_type[entity->GetIndex()] = 1;
		}
		else
		{
			if (shots_missed[entity->GetIndex()] > info.stored_missed)
			{
				resolve_type[entity->GetIndex()] = 4;
				switch (shots_missed[entity->GetIndex()] % 4)
				{
				case 0: entity->SetEyeAngles(info.inverse); break;
				case 1: entity->SetEyeAngles(info.left); break;
				case 2: entity->SetEyeAngles(info.back); break;
				case 3: entity->SetEyeAngles(info.right); break;
				}
			}
			else
			{
				entity->SetEyeAngles(info.last_lby);
				resolve_type[entity->GetIndex()] = 5;
			}
		}
	}
}

override* c_override = new override();