//#include "c_legitbot2.h"
//
////--------------------------------------------------------------------------------
//bool Aimbot::IsRcs() {
//	return G::LocalPlayer->GetShotsFired() >= 1;
//}
//bool Aimbot::IsEnabled(CUserCmd* pCmd)
//{
//	//if(c_menu::get()->is_menu_opened())
//	//	goto disabled;
//
//	if (!Interfaces::Engine->IsConnected() || !G::LocalPlayer || !G::LocalPlayer->IsAlive())
//		goto disabled;
//
//	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
//	if (!pWeapon)
//		goto disabled;
//
//	if ((pWeapon->GetItemDefenitionIndex() == WEAPON_AWP || pWeapon->GetItemDefenitionIndex() == WEAPON_SSG08) && !G::LocalPlayer->IsScoped())
//		goto disabled;
//
//	if (!G::LocalPlayer->CanShoot(pWeapon) || pWeapon->IsReloading())
//		goto disabled;
//
//	if (c_keyhandler::get()->auto_check("legit_aimbot_key"))
//		return true;
//
//disabled:
//	return false;
//}
////--------------------------------------------------------------------------------
//float Aimbot::GetSmooth() {
//	return 7.f - (G::UserCmd->buttons & IN_ATTACK ? c_config::get()->f["legit_aimbot_attack_speed"] : c_config::get()->f["legit_aimbot_speed"]);
//}
////--------------------------------------------------------------------------------
//void Aimbot::Smooth(Vector currentAngle, Vector aimAngle, Vector& angle) {
//	auto smooth_value = GetSmooth();
//	if (smooth_value <= 1) {
//		return;
//	}
//
//	Vector delta = aimAngle - currentAngle;
//	Cheat::Utilities->Math_FixAngles(delta);
//
//	// ������ �� ������� � �� ������
//	/*if (settings.smooth_type == 1) {*/
//		float deltaLength = fmaxf(sqrtf((delta.x * delta.x) + (delta.y * delta.y)), 0.01f);
//		delta *= (1.0f / deltaLength);
//
//		Interfaces::RandomSeed(Interfaces::Globals->tickcount);
//		float randomize = RandomFloat(-0.1f, 0.1f);
//		smooth_value = fminf((Interfaces::Globals->interval_per_tick * 64.0f) / (randomize + smooth_value * 0.15f), deltaLength);
//	/*}
//	else {
//		smooth_value = (Interfaces::Globals->interval_per_tick * 64.0f) / smooth_value;
//	}*/
//
//	delta *= smooth_value;
//	angle = currentAngle + delta;
//	Cheat::Utilities->Math_FixAngles(angle);
//}
////--------------------------------------------------------------------------------
//void Aimbot::RCS(Vector& angle, CBaseEntity* target, bool should_run) {
//	int rcs_pitch = c_config::get()->i["legit_rcs_pitch"],
//		rcs_yaw = c_config::get()->i["legit_rcs_yaw"];
//	if (rcs_pitch == 0 && rcs_yaw == 0){
//		RCSLastPunch.Init();
//		return;
//	}
//
//	Vector punch = G::LocalPlayer->GetPunchAngles() * 2.0f;
//
//	auto weapon = G::LocalPlayer->GetActiveWeapon();
//	if (weapon && weapon->GetNextPrimaryAttack() > Interfaces::Globals->curtime) {
//		auto delta_angles = punch - RCSLastPunch;
//		auto delta = weapon->GetNextPrimaryAttack() - Interfaces::Globals->curtime;
//		if (delta >= Interfaces::Globals->interval_per_tick)
//			punch = RCSLastPunch + delta_angles / static_cast<float>(TIME_TO_TICKS(delta));
//	}
//
//	CurrentPunch = punch;
//	if (c_config::get()->b["legit_other_standalone_rcs"] && !should_run)
//		punch -= { RCSLastPunch.x, RCSLastPunch.y, 0.f };
//
//	RCSLastPunch = CurrentPunch;
//	if (!IsRcs()) {
//		return;
//	}
//
//	angle.x -= punch.x * (rcs_pitch / 100.0f);
//	angle.y -= punch.y * (rcs_yaw / 100.0f);
//
//	Cheat::Utilities->Math_FixAngles(angle);
//}
//
//std::vector<int> Aimbot::GetHitBoxes(bool hit_head, bool hit_chest, bool hit_stomach)
//{
//	std::vector<int> hitboxes;
//
//	auto pWeapon = G::LocalPlayer->GetActiveWeapon();
//	if (!pWeapon)
//		goto ret;
//
//	auto pWeaponData = pWeapon->get_csweapon_info();
//	if (!pWeaponData)
//		goto ret;
//
//	if ((pWeapon->GetItemDefenitionIndex() == WEAPON_AWP || pWeapon->GetItemDefenitionIndex() == WEAPON_SSG08) && !G::LocalPlayer->IsScoped())
//		goto ret;
//
//	if (pWeapon->GetItemDefenitionIndex() != WEAPON_TASER && hit_head)
//		hitboxes.push_back(HITBOX_HEAD);
//	if (hit_chest)
//		hitboxes.push_back(HITBOX_CHEST);
//	if (hit_stomach)
//		hitboxes.push_back(HITBOX_BODY);
//
//ret:
//	return hitboxes;
//}
////--------------------------------------------------------------------------------
//CBaseEntity* Aimbot::GetClosestPlayer(CUserCmd* cmd, int* bestBone)
//{
//	std::vector<int> hitboxes = GetHitBoxes(c_config::get()->b["legit_aimbot_head"], c_config::get()->b["legit_aimbot_chest"], c_config::get()->b["legit_aimbot_stomach"]);
//	Vector ang, eVecTarget, pVecTarget = G::LocalPlayer->GetEyePosition();
//	CBaseEntity* player, * bestTarget = nullptr;
//	float bestFov = 9999.f, bestBoneFov = 9999.f;
//	int bestTargetUserId;
//	for (int i = 1; i < Interfaces::Engine->GetMaxClients(); ++i)
//	{
//		player = Interfaces::EntityList->GetClientEntity(i);
//
//		if ((player == nullptr) || player->IsNotTarget() || (player->GetTeam() == G::LocalPlayer->GetTeam()))
//			continue;
//
//		player_info_t inf;
//		if (!player->GetPlayerInfo(&inf))
//			continue;
//
//		for (int bone : hitboxes)
//		{
//			eVecTarget = player->GetHitboxPosition(bone);
//			if (eVecTarget == Vector(0, 0, 0))
//				continue;
//
//			ang = Cheat::Utilities->Math_CalcAngle(pVecTarget, eVecTarget);
//
//			Cheat::Utilities->Math_FixAngles(ang);
//
//			float fov = GetRealDistanceFOV(pVecTarget.DistTo(eVecTarget), cmd->viewangles + RCSLastPunch, ang);
//
//			if ((fov > c_config::get()->f["legit_aimbot_fov"]))
//				continue;
//
//			if (!G::LocalPlayer->CanSeePlayer(player, eVecTarget))
//				continue;
//
//			if (!c_config::get()->b["legit_aimbot_smoke"] && IsLineGoesThroughSmoke(pVecTarget, eVecTarget))
//				continue;
//
//			if (bestBoneFov < fov)
//				continue;
//
//			bestBoneFov = fov;
//			if (bestFov > fov)
//			{
//				if (bestBone != nullptr)
//					*bestBone = bone;
//				bestFov = fov;
//				bestTarget = player;
//				bestTargetUserId = inf.userId;
//			}
//		}
//	}
//
//	return bestTarget;
//}
//
//CBaseEntity* Aimbot::GetClosestPlayerDelay(CUserCmd* pCmd, int& bestBone)
//{
//	if (target && !kill_delay && c_config::get()->i["legit_aimbot_delay"] > 0 && target->IsNotTarget())
//	{
//		target = nullptr;
//		kill_delay = true;
//		kill_delay_time = (int)GetTickCount() + c_config::get()->i["legit_aimbot_delay"];
//	}
//	if (kill_delay)
//	{
//		if (kill_delay_time <= (int)GetTickCount())
//			kill_delay = false;
//		else
//			return nullptr;
//	}
//	target = GetClosestPlayer(pCmd, &bestBone);
//	return target;
//}
////--------------------------------------------------------------------------------
//void Aimbot::OnMove(CUserCmd* pCmd) {
//	if (!IsEnabled(pCmd)) {
//		if (G::LocalPlayer && Interfaces::Engine->IsInGame() && G::LocalPlayer->IsAlive() && c_config::get()->b["legit_aimbot_enabled"] && c_config::get()->b["legit_other_standalone_rcs"]) {
//			auto pWeapon = G::LocalPlayer->GetActiveWeapon();
//			if (pWeapon && (pWeapon->is_sniper() || pWeapon->is_pistol() || pWeapon->is_rifle())) {
//				RCS(pCmd->viewangles, target, false);
//				Cheat::Utilities->Math_FixAngles(pCmd->viewangles);
//				Interfaces::Engine->SetViewAngles(&pCmd->viewangles);
//			}
//		}
//		else {
//			RCSLastPunch = { 0, 0, 0 };
//		}
//
//		kill_delay = false;
//		target = NULL;
//		return;
//	}
//
//	Interfaces::RandomSeed(pCmd->command_number);
//
//	auto weapon = G::LocalPlayer->GetActiveWeapon();
//	if (!weapon)
//		return;
//
//	auto weapon_data = weapon->get_csweapon_info();
//	if (!weapon_data)
//		return;
//
//	bool should_do_rcs = false;
//	Vector angles = pCmd->viewangles;
//	Vector current = angles;
//	float fov = 180.f;
//	if (c_config::get()->b["legit_aimbot_blind"] || !G::LocalPlayer->IsFlashed()) {
//		int bestBone = -1;
//		if (GetClosestPlayerDelay(pCmd, bestBone)) {
//			Cheat::Utilities->Math_VectorAngles(target->GetHitboxPosition(bestBone) - G::LocalPlayer->GetEyePosition(), angles);
//			Cheat::Utilities->Math_FixAngles(angles);
//			fov = GetRealDistanceFOV(G::LocalPlayer->GetEyePosition().DistTo(target->GetHitboxPosition(bestBone)), angles, pCmd->viewangles);
//
//			should_do_rcs = true;
//
//			/*if (!settings.silent && !is_delayed && !shot_delay && settings.shot_delay > 0) {
//				is_delayed = true;
//				shot_delay = true;
//				shot_delay_time = GetTickCount() + settings.shot_delay;
//			}
//
//			if (shot_delay && shot_delay_time <= GetTickCount()) {
//				shot_delay = false;
//			}
//
//			if (shot_delay) {
//				pCmd->buttons &= ~IN_ATTACK;
//			}
//
//			if (settings.autofire) {
//				if (!settings.autofire_key || InputSys::Get().IsKeyDown(settings.autofire_key)) {
//					if (!weapon_data->bFullAuto) {
//						if (pCmd->command_number % 2 == 0) {
//							pCmd->buttons &= ~IN_ATTACK;
//						}
//						else {
//							pCmd->buttons |= IN_ATTACK;
//						}
//					}
//					else {
//						pCmd->buttons |= IN_ATTACK;
//					}
//				}
//			}*/
//
//			/*if (settings.autostop) {
//				pCmd->forwardmove = pCmd->sidemove = 0;
//			}*/
//		}
//	}
//
//	if (should_do_rcs || c_config::get()->b["legit_other_standalone_rcs"])
//		RCS(angles, target, should_do_rcs);
//
//	if (target)
//		Smooth(current, angles, angles);
//
//	Cheat::Utilities->Math_FixAngles(angles);
//	pCmd->viewangles = angles;
//	Interfaces::Engine->SetViewAngles(&angles);
//}
//
//Aimbot g_Aimbot;
