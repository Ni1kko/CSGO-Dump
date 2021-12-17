#include "c_legitbot.h"

bool CLegitAimbot::IsRcs()
{
	return (G::LocalPlayer->GetShotsFired() >= 1) || ((G::UserCmd->buttons & IN_ATTACK) && G::LocalPlayer->CanShoot()) || g_triggerbot.is_attack;
}

bool CLegitAimbot::IsEnabled(CUserCmd* pCmd)
{
	if (!Interfaces::Engine->IsConnected() || !G::LocalPlayer || !G::LocalPlayer->IsAlive())
		goto disabled;

	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
	if (!pWeapon)
		goto disabled;

	if ((pWeapon->GetItemDefenitionIndex() == WEAPON_AWP || pWeapon->GetItemDefenitionIndex() == WEAPON_SSG08) && !G::LocalPlayer->IsScoped())
		goto disabled;

	if ((pWeapon->GetAmmoInClip() == 0) || pWeapon->IsReloading())
		goto disabled;

	if (c_config::get()->b["legit_aimbot_enabled"] && c_keyhandler::get()->auto_check("legit_aimbot_key"))
		return true;

disabled:
	lock_on_starts_map.clear();
	return false;
}

float CLegitAimbot::GetSmooth()
{
	return max_smooth - ((G::UserCmd->buttons & IN_ATTACK) ? c_config::get()->f["legit_aimbot_attack_speed"] : c_config::get()->f["legit_aimbot_speed"]) * 5.98f;
}

void CLegitAimbot::Smooth(Vector currentAngle, Vector aimAngle, Vector& angle, float smooth_div)
{
	auto smooth_value = GetSmooth() / smooth_div;
	if (smooth_value <= 1)
		return;

	Vector delta = aimAngle - currentAngle;
	Cheat::Utilities->Math_FixAngles(delta);

	float deltaLength = fmaxf(sqrtf((delta.x * delta.x) + (delta.y * delta.y)), 0.01f);

	delta *= (1.0f / deltaLength);

	Interfaces::RandomSeed(Interfaces::Globals->tickcount);
	float randomize = RandomFloat(-0.1f, 0.1f);
	smooth_value = fminf((Interfaces::Globals->interval_per_tick * 64.0f) / (randomize + smooth_value * 0.15f), deltaLength);

	delta *= smooth_value;
	angle = currentAngle + delta;
	Cheat::Utilities->Math_FixAngles(angle);
}

void CLegitAimbot::RCS(Vector& angle, CBaseEntity* target, bool should_run)
{
	int rcs_pitch = c_config::get()->i["legit_rcs_pitch"],
		rcs_yaw = c_config::get()->i["legit_rcs_yaw"];
	if (rcs_pitch == 0 && rcs_yaw == 0){
		RCSLastPunch.Init();
		return;
	}

	Vector punch = G::LocalPlayer->GetPunchAngles() * Interfaces::Var->FindVar("weapon_recoil_scale")->GetFloat();

	auto weapon = G::LocalPlayer->GetActiveWeapon();
	if (weapon && weapon->GetNextPrimaryAttack() > Interfaces::Globals->curtime) {
		auto delta_angles = punch - RCSLastPunch;
		auto delta = weapon->GetNextPrimaryAttack() - Interfaces::Globals->curtime;
		if (delta >= Interfaces::Globals->interval_per_tick)
			punch = RCSLastPunch + delta_angles / static_cast<float>(TIME_TO_TICKS(delta));
	}

	CurrentPunch = punch;
	if (c_config::get()->b["legit_other_standalone_rcs"] && !should_run)
		punch -= { RCSLastPunch.x, RCSLastPunch.y, 0.f };

	RCSLastPunch = CurrentPunch;
	if (!IsRcs())
		return;

	angle.x -= punch.x * (rcs_pitch / 100.0f);
	angle.y -= punch.y * (rcs_yaw / 100.0f);

	Cheat::Utilities->Math_FixAngles(angle);
}

std::vector<int> CLegitAimbot::GetHitBoxes(bool hit_head, bool hit_chest, bool hit_stomach)
{
	std::vector<int> hitboxes;

	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
	if (!pWeapon)
		goto ret;

	auto pWeaponData = pWeapon->get_csweapon_info();
	if (!pWeaponData)
		goto ret;

	if ((pWeapon->GetItemDefenitionIndex() == WEAPON_AWP || pWeapon->GetItemDefenitionIndex() == WEAPON_SSG08) && !G::LocalPlayer->IsScoped())
		goto ret;

	if (pWeapon->GetItemDefenitionIndex() != WEAPON_TASER && hit_head)
		hitboxes.push_back(HITBOX_HEAD);
	if (hit_chest)
		hitboxes.push_back(HITBOX_CHEST);
	if (hit_stomach)
		hitboxes.push_back(HITBOX_BODY);

ret:
	return hitboxes;
}

CBaseEntity* CLegitAimbot::GetClosestPlayer(CUserCmd* cmd, int* p_bestBone, float* p_bestBoneFov)
{
	std::vector<int> hitboxes = GetHitBoxes(c_config::get()->b["legit_aimbot_head"], c_config::get()->b["legit_aimbot_chest"], c_config::get()->b["legit_aimbot_stomach"]);
	Vector ang, eVecTarget, pVecTarget = G::LocalPlayer->GetEyePosition();
	CBaseEntity* player, * bestTarget = nullptr;
	float bestFov = 9999.f, bestBoneFov = 9999.f;
	int bestTargetUserId;
	for (int i = 1; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		player = Interfaces::EntityList->GetClientEntity(i);

		if ((player == nullptr) || player->IsNotTarget() || (player->GetTeam() == G::LocalPlayer->GetTeam()))
			continue;

		player_info_t inf;
		if (!player->GetPlayerInfo(&inf))
			continue;
		
		bool erase_from_lock_on = false;
		int fov_out_hitboxes = 0;
		for (int bone : hitboxes)
		{
			eVecTarget = player->GetHitboxPosition(bone);
			if (eVecTarget == Vector(0, 0, 0))
				continue;

			ang = Cheat::Utilities->Math_CalcAngle(pVecTarget, eVecTarget);

			Cheat::Utilities->Math_FixAngles(ang);

			float fov = GetFovToPlayer(cmd->viewangles + RCSLastPunch, ang);

			if ((fov > c_config::get()->f["legit_aimbot_fov"]))
			{
				++fov_out_hitboxes;
				continue;
			}

			if (!G::LocalPlayer->CanSeePlayer(player, eVecTarget) || (!c_config::get()->b["legit_aimbot_smoke"] && IsLineGoesThroughSmoke(pVecTarget, eVecTarget)))
			{
				erase_from_lock_on = true;
				continue;
			}

			if (bestBoneFov < fov)
				continue;

			bestBoneFov = fov;
			if (bestFov > fov)
			{
				if (p_bestBone != nullptr)
					*p_bestBone = bone;
				if (p_bestBoneFov != nullptr)
					*p_bestBoneFov = bestBoneFov;
				bestFov = fov;
				bestTarget = player;
				bestTargetUserId = inf.userId;
			}
		}
		if ((!erase_from_lock_on && (fov_out_hitboxes != hitboxes.size())) || (lock_on_starts_map.count(inf.userId) == 0))
			continue;
		lock_on_starts_map.erase(inf.userId);
	}

	if (bestTarget != nullptr)
	{
		int cfg_lock_on_time = c_config::get()->i["legit_aimbot_lock_on_time"];
		if(cfg_lock_on_time <= 1000)
		{
			int lock_on_start_time;
			if (lock_on_starts_map.count(bestTargetUserId) <= 0)
			{
				lock_on_start_time = (int)GetTickCount();
				lock_on_starts_map[bestTargetUserId] = lock_on_start_time;
			}
			else
				lock_on_start_time = lock_on_starts_map[bestTargetUserId];

			if ((int)GetTickCount() - lock_on_start_time >= cfg_lock_on_time)
				return nullptr;
		}
	}

	return bestTarget;
}

CBaseEntity* CLegitAimbot::GetClosestPlayerDelay(CUserCmd* pCmd, int& bestBone, float& bestBoneFov)
{
	if (target && !kill_delay && c_config::get()->i["legit_aimbot_delay"] > 0 && target->IsNotTarget())
	{
		target = nullptr;
		kill_delay = true;
		kill_delay_time = (int)GetTickCount() + c_config::get()->i["legit_aimbot_delay"];
	}
	if (kill_delay)
	{
		if (kill_delay_time <= (int)GetTickCount())
			kill_delay = false;
		else
		{
			target = nullptr;
			return target;
		}
	}
	target = GetClosestPlayer(pCmd, &bestBone, &bestBoneFov);
	return target;
}

void CLegitAimbot::QuickStop()
{
	Vector velocity = G::LocalPlayer->GetVelocity();
	float speed = velocity.Length2D();
	float max_speed = 10.f;
	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
	if (pWeapon)
	{
		auto info = pWeapon->get_csweapon_info();
		if (info)
			max_speed = (pWeapon->WeaponMode() == 0 ? info->max_speed : info->max_speed_alt) * 0.18f;
	}
	if (speed < max_speed)
	{
		G::UserCmd->sidemove = G::UserCmd->forwardmove = 0;
		quick_stop_inaccuracy = pWeapon->GetInaccuracy();
		return;
	}
	Vector direction;
	Cheat::Utilities->Math_VectorAngles(velocity, direction);
	direction.y = G::UserCmd->viewangles.y - direction.y;
	Vector forward;
	Cheat::Utilities->Math_AngleVectors(direction, forward);
	Vector negated_direction = forward * -speed;
	G::UserCmd->forwardmove = negated_direction.x;
	G::UserCmd->sidemove = negated_direction.y;
}

void CLegitAimbot::OnMove(CUserCmd* pCmd)
{
	if (c_menu::get()->is_menu_opened())
	{
		lock_on_starts_map.clear();
		return;
	}

	if (c_config::get()->b["legit_aimbot_enabled"] && c_config::get()->b["legit_quickstop"] && c_keyhandler::get()->auto_check("legit_aimbot_key"))
	{
		if (quick_stop_inaccuracy == -1.0)
			QuickStop();

		if ((c_config::get()->b["legit_quickstop_trigger_shooting"] || GetClosestPlayer(pCmd, nullptr, nullptr) != nullptr) && IsRcs())
		{
			if (G::LocalPlayer->GetShotsFired() == 0)
			{
				if (delayed_shot == false)
				{
					Vector velocity = G::LocalPlayer->GetVelocity();
					float speed = velocity.Length2D();
					if (speed > 1)
						delayed_shot = true;
				}
			}
			else
				QuickStop();
		}
	}
	else
		quick_stop_inaccuracy = -1.0;

	if (delayed_shot)
	{
		QuickStop();
		G::UserCmd->buttons &= ~IN_ATTACK;
		auto pWeapon = G::LocalPlayer->GetActiveWeapon();
		if (pWeapon)
		{
			auto info = pWeapon->get_csweapon_info();
			if (info)
			{
				float max_speed = (pWeapon->WeaponMode() == 0 ? info->max_speed : info->max_speed_alt) * 0.18f;
				Vector velocity = G::LocalPlayer->GetVelocity();
				float speed = velocity.Length2D();
				if (speed < max_speed)
				{
					G::UserCmd->buttons |= IN_ATTACK;
					delayed_shot = false;
				}
			}
		}
	}

	if (!IsEnabled(pCmd))
	{
		if (G::LocalPlayer && Interfaces::Engine->IsInGame() && G::LocalPlayer->IsAlive() && c_config::get()->b["legit_other_standalone_rcs"])
		{
			auto pWeapon = G::LocalPlayer->GetActiveWeapon();
			if (pWeapon && (pWeapon->is_sniper() || pWeapon->is_pistol() || pWeapon->is_rifle()))
			{
				RCS(pCmd->viewangles, target, false);
				Cheat::Utilities->Math_FixAngles(pCmd->viewangles);
				Interfaces::Engine->SetViewAngles(&pCmd->viewangles);
			}
		}
		else
			RCSLastPunch = { 0, 0, 0 };

		kill_delay = false;
		target = nullptr;

		return;
	}

	Vector oldAngle;
	Interfaces::Engine->GetViewAngles(&oldAngle);

	Interfaces::RandomSeed(pCmd->command_number);

	auto weapon = G::LocalPlayer->GetActiveWeapon();
	if (!weapon)
		return;

	auto weapon_data = weapon->get_csweapon_info();
	if (!weapon_data)
		return;

	bool should_do_rcs = false;
	Vector angles = pCmd->viewangles;
	Vector current = angles;

	float smooth_div = 1.f;

	if (c_config::get()->b["legit_aimbot_enabled"] && (c_config::get()->b["legit_aimbot_blind"] || !G::LocalPlayer->IsFlashed()) && GetSmooth() != max_smooth)
	{
		int bestBone = -1;
		float bestBoneFov = 9999.f;
		if (GetClosestPlayerDelay(pCmd, bestBone, bestBoneFov))
		{
			Vector eVecTarget;
			eVecTarget = target->GetHitboxPosition(bestBone);
			angles = Cheat::Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), eVecTarget);

			float dist = Cheat::Utilities->Math_NormalizeYaw2(angles.y - oldAngle.y);
			if (dist > 180.0f) dist = 360.0f - dist;
			eVecTarget += Vector(0, 0, 1 * dist);

			angles = Cheat::Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), eVecTarget);

			Cheat::Utilities->Math_FixAngles(angles);

			should_do_rcs = true;

			smooth_div = ((float)c_config::get()->i["legit_aimbot_speed_scale"] + bestBoneFov / c_config::get()->f["legit_aimbot_fov"] * (100.f - (float)c_config::get()->i["legit_aimbot_speed_scale"])) / 100.f;
		}
	}
	else
		lock_on_starts_map.clear();
	
	if (should_do_rcs || c_config::get()->b["legit_other_standalone_rcs"])
		RCS(angles, target, should_do_rcs);

	if (target)
		Smooth(current, angles, angles, smooth_div);

	Cheat::Utilities->Math_FixAngles(angles);
	pCmd->viewangles = angles;
	Interfaces::Engine->SetViewAngles(&angles);
}

CLegitAimbot g_legitbot;
