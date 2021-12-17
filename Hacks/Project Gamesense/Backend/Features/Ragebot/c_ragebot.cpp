#include "c_ragebot.h"
#include "c_playerlist.h"
#include "Resolver.h"
#include "..//Misc/LagCompensation.h"
#include "../c_fps.h"
void c_ragebot::create_move() {
	if (!G::LocalPlayer)
		return;

	if (!G::LocalPlayer->IsAlive())
		return;

	if (!G::LocalPlayer->GetActiveWeapon())
		return;

	if (G::LocalPlayer->GetActiveWeapon()->is_knife())
		return;

	if (G::LocalPlayer->GetActiveWeapon()->is_grenade())
		return;

	if (!c_config::get()->b["rage_aimbot"])
		return;

	if (!c_keyhandler::get()->auto_check("rage_aimbot_key"))
		return;
	
	bool FindNewTarget = true;
	int best_fov = 180;
	int best_health = 101;

	if (FindNewTarget)
	{
		TargetID = 0;
		tempTarget = nullptr;

		switch (c_config::get()->i["rage_selectionmode"])
		{
		case 0: TargetID = GetTargetCycle();
		case 1: TargetID = GetTargetCycle();
		case 2: TargetID = GetTargetFOV();
		case 3: TargetID = GetTargetHealth();
		case 4: TargetID = GetTargetPing();
		case 5: TargetID = GetTargetHealth();
		case 6: TargetID = GetTargetDistance();
		}
		if (TargetID >= 0)
			tempTarget = Interfaces::EntityList->GetClientEntity(TargetID);
		else
		{
			tempTarget = nullptr;
		}
	}
	if (TargetID >= 0 && tempTarget)
	{
		auto aimpoint = hitscan();
		if (aimpoint != Vector(0, 0, 0)) {
			aim_at_point(aimpoint, TargetID);
		}
	}

	if (G::UserCmd->buttons & IN_ATTACK) {
		if (c_config::get()->b["rage_removerecoil"])
			G::UserCmd->viewangles -= G::LocalPlayer->GetPunchAngles() * 2.f;
	}
}
bool c_ragebot::IsViable(CBaseEntity* pEntity)
{	
	
	auto m_local = G::LocalPlayer;
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != m_local->GetIndex())

	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)ClassID::CCSPlayer)
		{
			if (pEntity->GetTeam() != m_local->GetTeam() || c_config::get()->b["rage_friendlyfire"])
			{
				if (!pEntity->GetImmunity())
				{
					if (!c_playerlist::get()->should_disable_aimbot(pEntity))

						return true;
				}
			}
		}
	}
	return false;
}
int c_ragebot::GetTargetCycle()
{
	auto m_local = G::LocalPlayer;
	int target = -1;
	int minDist = 99999;
	CBaseEntity* pLocal = m_local;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(&View);
	auto BoundingBoxCheck = [this](CBaseEntity* entity) -> bool
	{
		auto collideable = entity->GetCollision();
		if (!collideable)
			return false;
		const auto bbmin = collideable->VecMins() + entity->GetOrigin();
		const auto bbmax = collideable->VecMaxs() + entity->GetOrigin();
		Vector points[7];

		points[0] = entity->hitbox_position(0);
		points[1] = (bbmin + bbmax) * 0.5f;
		points[2] = Vector((bbmax.x + bbmin.x) * 0.5f, (bbmax.y + bbmin.y) * 0.5f, bbmin.z);
		points[3] = bbmax;
		points[4] = Vector(bbmax.x, bbmin.y, bbmax.z);
		points[5] = Vector(bbmin.x, bbmin.y, bbmax.z);
		points[6] = Vector(bbmin.x, bbmax.y, bbmax.z);

		for (const auto& point : points)
		{
			if (c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, entity).damage > 0) {
				return true;
			}
			else {
			}
		}
		return false;
	};
	for (int i = 0; i < 64; i++)
	{
		CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			if (BoundingBoxCheck(pEntity))
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = Utilities->Math_GetFov2(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < c_config::get()->i["rage_fov"])
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}
	return target;
}
int c_ragebot::GetTargetFOV()
{
	auto m_local = G::LocalPlayer;
	int target = -1;
	float mfov = 180;
	Vector viewoffset = m_local->GetOrigin() + m_local->GetViewOffset();
	Vector view; Interfaces::Engine->GetViewAngles(&view);
	auto BoundingBoxCheck = [this](CBaseEntity* entity) -> bool
	{
		auto collideable = entity->GetCollision();

		if (!collideable)
			return false;

		const auto bbmin = collideable->VecMins() + entity->GetOrigin();
		const auto bbmax = collideable->VecMaxs() + entity->GetOrigin();
		Vector points[7];

		points[0] = entity->hitbox_position(0);
		points[1] = (bbmin + bbmax) * 0.5f;
		points[2] = Vector((bbmax.x + bbmin.x) * 0.5f, (bbmax.y + bbmin.y) * 0.5f, bbmin.z);

		points[3] = bbmax;
		points[4] = Vector(bbmax.x, bbmin.y, bbmax.z);
		points[5] = Vector(bbmin.x, bbmin.y, bbmax.z);
		points[6] = Vector(bbmin.x, bbmax.y, bbmax.z);

		for (const auto& point : points)
		{
			if (c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, entity).damage > 0) {
				return true;
			}
			else {
			}
		}
		return false;
	};
	for (int i = 0; i < 64; i++)
	{
		CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			if (BoundingBoxCheck(pEntity))
			{	
				float fov = Utilities->Math_GetFov2(viewoffset, view, pEntity, 0);
				if (fov < mfov && fov < c_config::get()->i["rage_fov"])
				{
					mfov = fov;
					target = i;
				}
			}
		}
	}
	return target;
}
int c_ragebot::GetTargetDistance()
{
	auto m_local = G::LocalPlayer;
	int target = -1;
	int minDist = 99999;
	CBaseEntity* pLocal = m_local;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(&View);
	auto BoundingBoxCheck = [this](CBaseEntity* entity) -> bool
	{
		auto collideable = entity->GetCollision();
		if (!collideable)
			return false;
		const auto bbmin = collideable->VecMins() + entity->GetOrigin();
		const auto bbmax = collideable->VecMaxs() + entity->GetOrigin();
		Vector points[7];

		points[0] = entity->hitbox_position(0);
		points[1] = (bbmin + bbmax) * 0.5f;
		points[2] = Vector((bbmax.x + bbmin.x) * 0.5f, (bbmax.y + bbmin.y) * 0.5f, bbmin.z);
		points[3] = bbmax;
		points[4] = Vector(bbmax.x, bbmin.y, bbmax.z);
		points[5] = Vector(bbmin.x, bbmin.y, bbmax.z);
		points[6] = Vector(bbmin.x, bbmax.y, bbmax.z);

		for (const auto& point : points)
		{
			if (c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, entity).damage > 0) {
				return true;
			}
			else {
			}
		}
		return false;
	};
	for (int i = 0; i < 64; i++)
	{
		CBaseEntity *pEntity = Interfaces::EntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			if (BoundingBoxCheck(pEntity))
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = Utilities->Math_GetFov2(ViewOffset, View, pEntity, 0);
				if (Distance < minDist  && fov < c_config::get()->i["rage_fov"])
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}
	return target;
}

int c_ragebot::GetTargetPing()
{
	auto m_local = G::LocalPlayer;
	int target = -1;
	int minHealth = 101;
	CBaseEntity* pLocal = m_local;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(&View);
	auto BoundingBoxCheck = [this](CBaseEntity* entity) -> bool
	{
		auto collideable = entity->GetCollision();
		if (!collideable)
			return false;
		const auto bbmin = collideable->VecMins() + entity->GetOrigin();
		const auto bbmax = collideable->VecMaxs() + entity->GetOrigin();
		Vector points[7];

		points[0] = entity->hitbox_position(0);
		points[1] = (bbmin + bbmax) * 0.5f;
		points[2] = Vector((bbmax.x + bbmin.x) * 0.5f, (bbmax.y + bbmin.y) * 0.5f, bbmin.z);
		points[3] = bbmax;
		points[4] = Vector(bbmax.x, bbmin.y, bbmax.z);
		points[5] = Vector(bbmin.x, bbmin.y, bbmax.z);
		points[6] = Vector(bbmin.x, bbmax.y, bbmax.z);

		for (const auto& point : points)
		{
			if (c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, entity).damage > 0) {

				return true;
			}
			else {
			}
		}
		return false;
	};

	for (int i = 0; i < 64; i++)
	{
		CBaseEntity* pEntity = Interfaces::EntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			if (BoundingBoxCheck(pEntity))
			{
				int Health = pEntity->GetPing();
				float fov = Utilities->Math_GetFov2(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < c_config::get()->i["rage_fov"])
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}
	return target;
}

int c_ragebot::GetTargetHealth()
{
	auto m_local = G::LocalPlayer;
	int target = -1;
	int minHealth = 1;
	CBaseEntity* pLocal = m_local;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(&View);
	auto BoundingBoxCheck = [this](CBaseEntity* entity) -> bool
	{
		auto collideable = entity->GetCollision();
		if (!collideable)
			return false;
		const auto bbmin = collideable->VecMins() + entity->GetOrigin();
		const auto bbmax = collideable->VecMaxs() + entity->GetOrigin();
		Vector points[7];

		points[0] = entity->hitbox_position(0);
		points[1] = (bbmin + bbmax) * 0.5f;
		points[2] = Vector((bbmax.x + bbmin.x) * 0.5f, (bbmax.y + bbmin.y) * 0.5f, bbmin.z);
		points[3] = bbmax;
		points[4] = Vector(bbmax.x, bbmin.y, bbmax.z);
		points[5] = Vector(bbmin.x, bbmin.y, bbmax.z);
		points[6] = Vector(bbmin.x, bbmax.y, bbmax.z);

		for (const auto& point : points)
		{
			if (c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, entity).damage > 0) {

				return true;
			}
			else {
			}
		}
		return false;
	};

	for (int i = 0; i < 64; i++)
	{
		CBaseEntity *pEntity = Interfaces::EntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			if (BoundingBoxCheck(pEntity))
			{
				int Health = c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), pEntity->hitbox_position(0), G::LocalPlayer, pEntity).damage;
				float fov = Utilities->Math_GetFov2(ViewOffset, View, pEntity, 0);
				if (Health > minHealth && fov < c_config::get()->i["rage_fov"])
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}
	return target;
}

float GetLerpTime() {
	static ConVar* cl_interp =Interfaces::Var->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces::Var->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = Interfaces::Var->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = Interfaces::Var->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = Interfaces::Var->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = Interfaces::Var->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces::Var->FindVar("sv_client_max_interp_ratio");
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
bool c_ragebot::aim_at_point(Vector point,int idx) {
	auto wep = G::LocalPlayer->GetActiveWeapon();
	float flServerTime = G::LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
	bool canShoot = (G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() <= Interfaces::Globals->curtime && G::LocalPlayer->GetActiveWeapon()->GetAmmoInClip() > 0);
	auto aimangle = Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), point);
	Vector localview;
	Interfaces::Engine->GetViewAngles(&localview);
	auto shoot_state = this->is_able_to_shoot();
	auto hc_state = this->hitchance(aimangle);
	if (hc_state)
	{	
		if (c_config::get()->b["rage_autoscope"] && G::LocalPlayer->GetActiveWeapon()->is_sniper() && G::LocalPlayer->IsScoped() == false && G::LocalPlayer->GetActiveWeapon()->GetZoomLevel() == 0)
		{
			if (G::UserCmd->buttons & IN_ATTACK)
				G::UserCmd->buttons &= ~IN_ATTACK;

			G::UserCmd->buttons |= IN_ATTACK2;
		}

		if (c_config::get()->b["rage_delayshot"] && tempTarget->GetSimTime() == tempTarget->GetOldSimTime())
			return false;

		if (c_config::get()->b["rage_autofire"] && shoot_state)
		{
			if (!c_keyhandler::get()->auto_check("rage_fakeduck_key") && !c_config::get()->b["rage_fakelagonshot"])
				G::SendPacket = true;

			if (c_config::get()->b["rage_doubletap"] && c_keyhandler::get()->auto_check("rage_exploit_key")) {
				int doubletapTickcountDelta = Interfaces::Globals->tickcount - G::lastDoubleTapInTickcount;

				if (doubletapTickcountDelta >= 128) {

					G::lastDoubleTapInTickcount = Interfaces::Globals->tickcount;

					G::TickbaseShift = 17;
					G::IsDoubleTapShooting = true;
				}
			}

			G::UserCmd->buttons |= IN_ATTACK;
			G::Shots[idx]++;
			G::RagebotIsShooting = true;
		}
		if (G::UserCmd->buttons & IN_ATTACK) {
			if (!c_keyhandler::get()->auto_check("rage_fakeduck_key"))
				G::SendPacket = true;

			G::UserCmd->tick_count = TIME_TO_TICKS(tempTarget->GetSimTime() + GetLerpTime());
			G::UserCmd->viewangles = aimangle;
			if (!c_config::get()->b["rage_silentaim"]) {
				Interfaces::Engine->SetViewAngles(&aimangle);
			}

		}
	}
	else 
	{
		/*if (c_config::get()->b["rage_autoscope"] && G::LocalPlayer->GetActiveWeapon()->is_sniper() && !G::LocalPlayer->IsScoped())
		{

			G::UserCmd->buttons |= IN_ATTACK2;
		}

		if (c_config::get()->b["rage_autoscope"] && G::LocalPlayer->GetActiveWeapon()->is_sniper() && G::LocalPlayer->GetActiveWeapon()->GetZoomLevel() == 0) {
			if (G::UserCmd->buttons & IN_ATTACK)
				G::UserCmd->buttons &= ~IN_ATTACK;

			G::UserCmd->buttons |= IN_ATTACK2;
		}*/
		if (c_config::get()->b["rage_autoscope"] && G::LocalPlayer->GetActiveWeapon()->is_sniper() && !G::LocalPlayer->IsScoped())
		{
			if (G::UserCmd->buttons & IN_ATTACK)
				G::UserCmd->buttons &= ~IN_ATTACK;

			G::UserCmd->buttons |= IN_ATTACK2;
		}

	}
	/*if (c_config::get()->b["rage_quickstop"] == 1) {
		static int MinimumVelocity = 0;
		bool shouldstop = c_config::get()->b["rage_stop_shots"] ? canShoot : true;
		G::UserCmd->forwardmove = 0;
		G::UserCmd->sidemove = 0;
	}*/
	if (c_config::get()->b["rage_quickstop"] && c_config::get()->m["rage_quickstop_options"][0]) {
		if (c_config::get()->m["rage_quickstop_options"][1]) {
			if (c_config::get()->m["rage_quickstop_options"][4] && !canShoot)
				return true;

			if (!c_config::get()->m["rage_quickstop_options"][5] && G::LocalPlayer->GetFlags() & FL_ONFIRE)
				return true;

			auto weapon_handle = G::LocalPlayer->GetActiveWeapon();

			float amount = 0.30f;
			Vector velocity = G::LocalPlayer->GetVelocity();
			Vector direction;
			Utilities->Math_VectorAngles(velocity, direction);
			float speed = velocity.Length2D();
			direction.y = G::UserCmd->viewangles.y - direction.y;
			Vector forward;
			Utilities->Math_AngleVectors(direction, forward);
			Vector source = forward * -speed;

			if (speed >= (weapon_handle->get_full_info()->max_speed * amount))
			{
				G::UserCmd->forwardmove = source.x;
				G::UserCmd->sidemove = source.y;

			}

			if (c_config::get()->m["rage_quickstop_options"][2])
				G::UserCmd->buttons &= IN_DUCK;

			static int cnt = 0;
			static bool do_ = false;
			if (c_config::get()->m["rage_quickstop_options"][3] && (!c_config::get()->b["rage_doubletap"])) {
				G::SendPacket = false;
				G::FakelagShouldLag = false;
				G::IsDoubleTapShooting = true;
				if (cnt % 14 == 0)
					do_ = true;
				else if (cnt % 14 == 6)
					G::SendPacket = true;
				else if (cnt % 14 == 7)
					do_ = false;

				if (do_)
					G::UserCmd->buttons |= IN_DUCK;
				else
					G::UserCmd->buttons &= ~IN_DUCK;

				cnt++;
			}
		}
		else {
			if (c_config::get()->m["rage_quickstop_options"][4] && !canShoot)
				return true;

			if (!c_config::get()->m["rage_quickstop_options"][5] && G::LocalPlayer->GetFlags() & FL_ONFIRE)
				return true;

			if (G::UserCmd->buttons & IN_MOVELEFT)
			{
				G::UserCmd->buttons |= IN_MOVERIGHT;
			}

			if (G::UserCmd->buttons & IN_MOVERIGHT)
			{
				G::UserCmd->buttons |= IN_MOVELEFT;
			}

			if (G::UserCmd->buttons & IN_FORWARD)
			{
				G::UserCmd->buttons |= IN_BACK;
			}

			if (G::UserCmd->buttons & IN_BACK)
			{
				G::UserCmd->buttons |= IN_FORWARD;
			}

			if (c_config::get()->m["rage_quickstop_options"][2])
				G::UserCmd->buttons |= IN_DUCK;

			static int cnt = 0;
			static bool do_ = false;
			if (c_config::get()->m["rage_quickstop_options"][3] && (!c_config::get()->b["rage_doubletap"])) {
				G::SendPacket = false;
				G::FakelagShouldLag = false;
				G::IsDoubleTapShooting = true;
				if (cnt % 14 == 0)
					do_ = true;
				else if (cnt % 14 == 6)
					G::SendPacket = true;
				else if (cnt % 14 == 7)
					do_ = false;

				if (do_)
					G::UserCmd->buttons |= IN_DUCK;
				else
					G::UserCmd->buttons &= ~IN_DUCK;

				cnt++;
			}
		}
	
	}
	/*else if (c_config::get()->i["rage_quickstop"] == 3) {
		static int MinimumVelocity = 0;
		bool shouldstop = c_config::get()->b["rage_stop_shots"] ? canShoot : true;
		MinimumVelocity = G::LocalPlayer->GetActiveWeapon()->get_full_info()->max_speed_alt * .17f;
		if (G::LocalPlayer->GetVelocity().Length() >= MinimumVelocity && shouldstop && !GetAsyncKeyState(VK_SPACE) && !wep->is_knife() && wep->GetItemDefenitionIndex() != WEAPON_TASER)
			quick_stop();
	}*/


	return true;
}

bool c_ragebot::hitchance(Vector aimangle) {
	float chance;
	auto wep = G::LocalPlayer->GetActiveWeapon();
	auto wepidx = wep->GetItemDefenitionIndex();
		chance = float(c_config::get()->i["rage_hitchance_value1"]);

	if (!wep)
		return false;

	/*if (c_config::get()->b["rage_backtrack"]) {
		if (Backtrack[tempTarget->Index()] || ShotBacktrack[tempTarget->Index()]) // doing this bec im lazy
		{
			float Velocity = G::LocalPlayer->GetVelocity().Length();

			if (Velocity <= (G::LocalPlayer->GetActiveWeapon()->get_csweapon_info()->max_speed_alt * .34f))
				Velocity = 0.0f;

			float SpreadCone = G::LocalPlayer->GetActiveWeapon()->GetAccuracyPenalty() * 256.0f / M_PI + G::LocalPlayer->GetActiveWeapon()->get_csweapon_info()->max_speed * Velocity / 3000.0f; // kmeth https://github.com/DankPaster/kmethdude
			float a = (hitscan() - G::LocalPlayer->GetEyePosition()).Length();
			float b = sqrt(tan(SpreadCone * M_PI / 180.0f) * a);
			if (2.2f > b) return true;
			return (chance <= ((2.2f / fmax(b, 2.2f)) * 100.0f));
		}
	}*/

	Vector fw, rw, uw;
	Utilities->Math_AngleVectors(aimangle, fw, rw, uw);

	int hits = 0;
	int needed_hits = static_cast<int>(256.f * (chance / 100.f));
	if (c_config::get()->m["rage_lowfps"][4]) {
		needed_hits = static_cast<int>(64.f * (chance / 100.f));
	}

	wep->UpdateAccuracyPenalty();
	float cone = wep->GetSpreadCone();
	float inacc = wep->GetInaccuracy();

	Vector src = G::LocalPlayer->GetEyePosition();
	if (c_config::get()->m["rage_lowfps"][4]) {
		for (int i = 0; i < 64; i++) {
			float a = Utilities->Game_RandomFloat(0.f, 1.f);
			float b = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
			float c = Utilities->Game_RandomFloat(0.f, 1.f);
			float d = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
			float inaccuracy = a * inacc;
			float spread = c * cone;

			if (wep->GetItemDefenitionIndex() == WEAPON_REVOLVER) {
				if (G::UserCmd->buttons & IN_ATTACK2) {
					a = 1.f - a * a;
					c = 1.f - c * c;
				}
			}

			Vector spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0);
			Vector direction;

			direction.x = fw.x + (spread_view.x * rw.x) + (spread_view.y * uw.x);
			direction.y = fw.y + (spread_view.x * rw.y) + (spread_view.y * uw.y);
			direction.z = fw.z + (spread_view.x * rw.z) + (spread_view.y * uw.z);
			direction.Normalized();

			Vector viewangles_spread;
			Vector view_forward;

			Utilities->Math_VectorAngles(direction, uw, viewangles_spread);
			Utilities->Math_Normalize(viewangles_spread);
			Utilities->Math_AngleVectors(viewangles_spread, view_forward);

			view_forward.NormalizeInPlace();
			view_forward = src + (view_forward * wep->get_csweapon_info()->range);
			//view_forward = src + (view_forward * wep->get_full_info()->range);

			trace_t tr;
			Ray_t ray;

			ray.Init(src, view_forward);
			Interfaces::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, tempTarget, &tr);

			if (tr.m_pEnt == tempTarget)
				hits++;

			if (static_cast<int>((static_cast<float>(hits) / 64.f) * 100.f) >= chance)
				return true;

			if ((64 - i + hits) < needed_hits)
				return false;
		}
	}
	else {
		for (int i = 0; i < 256; i++) {
			float a = Utilities->Game_RandomFloat(0.f, 1.f);
			float b = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
			float c = Utilities->Game_RandomFloat(0.f, 1.f);
			float d = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
			float inaccuracy = a * inacc;
			float spread = c * cone;

			if (wep->GetItemDefenitionIndex() == WEAPON_REVOLVER) {
				if (G::UserCmd->buttons & IN_ATTACK2) {
					a = 1.f - a * a;
					c = 1.f - c * c;
				}
			}

			Vector spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0);
			Vector direction;

			direction.x = fw.x + (spread_view.x * rw.x) + (spread_view.y * uw.x);
			direction.y = fw.y + (spread_view.x * rw.y) + (spread_view.y * uw.y);
			direction.z = fw.z + (spread_view.x * rw.z) + (spread_view.y * uw.z);
			direction.Normalized();

			Vector viewangles_spread;
			Vector view_forward;

			Utilities->Math_VectorAngles(direction, uw, viewangles_spread);
			Utilities->Math_Normalize(viewangles_spread);
			Utilities->Math_AngleVectors(viewangles_spread, view_forward);

			view_forward.NormalizeInPlace();
			view_forward = src + (view_forward * wep->get_csweapon_info()->range);
			//view_forward = src + (view_forward * wep->get_full_info()->range);

			trace_t tr;
			Ray_t ray;

			ray.Init(src, view_forward);
			Interfaces::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, tempTarget, &tr);

			if (tr.m_pEnt == tempTarget)
				hits++;

			if (static_cast<int>((static_cast<float>(hits) / 256.f) * 100.f) >= chance)
				return true;

			if ((256 - i + hits) < needed_hits)
				return false;
		}
	}


	return false;
}

bool c_ragebot::is_able_to_shoot() {
	if (G::LocalPlayer->GetActiveWeapon()->GetAmmoInClip() <= 0) {
		return false;
	}
	if (G::LocalPlayer->GetActiveWeapon()->is_grenade()) {
		if (G::LocalPlayer->GetActiveWeapon()->IsPinPulled())
			return false;
		if (G::LocalPlayer->GetActiveWeapon()->GetThrowTime() <= 0 || G::LocalPlayer->GetActiveWeapon()->GetThrowTime() > Interfaces::Globals->curtime)
			return false;
		return true;
	}
	if (!G::LocalPlayer) {
		return false;
	}

	return (G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() < Interfaces::Globals->curtime) && (G::LocalPlayer->GetNextAttack() < Interfaces::Globals->curtime);
}

void c_ragebot::multipoints(int hitbox, matrix3x4_t bones[128], std::vector<Vector>& points) {
	if (!c_config::get()->m["rage_multipoint"][0] && !c_config::get()->m["rage_multipoint"][1] && !c_config::get()->m["rage_multipoint"][2])
		return;

	if (c_config::get()->b["rage_safepoint"] && tempTarget->IsVisible(G::LocalPlayer))
		return;

	auto cfg = c_config::get();
	auto mdl = Interfaces::ModelInfo->GetStudioModel(tempTarget->GetModel());
	auto set = mdl->pHitboxSet(tempTarget->HitBoxSet());
	auto hbx = set->GetHitbox(hitbox);

	if (!hbx)
		return;

	Vector mins, maxs;
	Utilities->Math_VectorTransform(hbx->bbmin, bones[hbx->bone], mins);
	Utilities->Math_VectorTransform(hbx->bbmax, bones[hbx->bone], maxs);

	Vector center = (mins + maxs) * 0.5f;
	Vector angle = Utilities->Math_CalcAngle(center, G::LocalPlayer->GetEyePosition());

	Vector forward;
	Utilities->Math_AngleVectors(angle, forward);

	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);
	Vector top = Vector(0, 0, 1);
	Vector bottom = Vector(0, 0, -1);

	float adjusted_radius = 0.f;
	switch (hitbox) {
	case HITBOX_HEAD:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius});
			points.push_back({ center + right * adjusted_radius});
			points.push_back({ center + left * adjusted_radius });
		}
		break;

	case HITBOX_CHEST:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + right * adjusted_radius });
			points.push_back({ center + left * adjusted_radius});
			points.push_back({ center + bottom * adjusted_radius });
		}
		break;

	case HITBOX_UPPER_CHEST:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius });
			points.push_back({ center + right * adjusted_radius });
			points.push_back({ center + left * adjusted_radius });
		}
		break;
		
	case HITBOX_BODY:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius });
			points.push_back({ center + right * adjusted_radius });
			points.push_back({ center + left * adjusted_radius });
		}
		break;

	case HITBOX_PELVIS:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + right * adjusted_radius });
			points.push_back({ center + left * adjusted_radius });
			points.push_back({ center + bottom * adjusted_radius });
		}
		break;

	case HITBOX_LEFT_FOREARM:
	case HITBOX_RIGHT_FOREARM:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius });
			points.push_back({ center + bottom * adjusted_radius });
		}
		break;

	case HITBOX_LEFT_UPPER_ARM:
	case HITBOX_RIGHT_UPPER_ARM:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius });
			points.push_back({ center + bottom * adjusted_radius });
		}
		break;

	case HITBOX_LEFT_CALF:
	case HITBOX_RIGHT_CALF:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius });
			points.push_back({ center + bottom * adjusted_radius });
		}
		break;

	case HITBOX_LEFT_FOOT:
	case HITBOX_RIGHT_FOOT:
		adjusted_radius = hbx->radius * float(cfg->i["rage_headselection_scale"] / 100.f);

		if (adjusted_radius > 0.f) {
			points.push_back({ center + top * adjusted_radius });
			points.push_back({ center + bottom * adjusted_radius });
		}
		break;
	}
}

Vector c_ragebot::hitscan(int* estimated_damage) {
	static DWORD addr = (DWORD)Cheat::Utilities->Memory_PatternScan("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	unsigned long ModelBoneCounter = **(unsigned long**)(addr + 10);

	std::vector<Vector> points = {};

	tempTarget->InvalidateBoneCache();

	auto cfg = c_config::get();
	auto wep = G::LocalPlayer->GetActiveWeapon();
	auto wepdmg = wep->get_full_info()->damage;
	auto wepacc = wep->GetAccuracyPenalty();
	auto wepidx = wep->GetItemDefenitionIndex();
	auto enemyhp = tempTarget->GetHealth();
	auto ent = tempTarget->GetIndex();
	auto force_baim = false;

	if (cfg->b["rage_baimpreference"] && (cfg->m["rage_points"][1] || cfg->m["rage_points"][2]))
		force_baim = true;
	

	if (G::LocalPlayer->GetVelocity().Length() < G::LocalPlayer->GetActiveWeapon()->get_full_info()->max_speed_alt * .33f && cfg->m["rage_baimdisablers"][0])
		force_baim = false;

	if (tempTarget->GetShotsFired() > 0 && cfg->m["rage_baimdisablers"][1])
		force_baim = false;

	if (G::Shots[tempTarget->GetIndex()] - G::ShotsHit[tempTarget->GetIndex()] % 2 == 1 && cfg->m["rage_baimdisablers"][2])
		force_baim = false;

	if (tempTarget->GetEyeAngles().x < 10 && tempTarget->GetEyeAngles().x > -89 && cfg->m["rage_baimdisablers"][3])
		force_baim = false;

	if (tempTarget->GetHealth() > cfg->i["rage_minimaldamage1"] * 2 && cfg->m["rage_baimdisablers"][4])
		force_baim = false;


	if (c_keyhandler::get()->auto_check("rage_forcebaim_key"))
		force_baim = true;
	if (cfg->b["rage_baimonpeek"] && G::LocalPlayer->GetVelocity().Length2D() > 80)
		force_baim = true;
	if (c_playerlist::get()->should_prefer_body(tempTarget))
		force_baim = true;
	if (wepidx == WEAPON_TASER || wep->is_knife())
		force_baim = true;
	if (c_keyhandler::get()->auto_check("rage_safepoint_key"))
		force_baim = true;

	static tick_record record;
	if (c_config::get()->i["rage_backtrack"] > 0 && !c_config::get()->b["rage_doubletap"]) {
		if (c_config::get()->m["rage_lowfps"][3] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
			for (int j = g_BacktrackHelper->PlayerRecord[tempTarget->GetIndex()].records.size() - 1; j >= 0; j--)
			{
				switch (c_config::get()->i["rage_backtrack"]) {
				case 1:
					record = g_BacktrackHelper->PlayerRecord[tempTarget->GetIndex()].records.at(0);
					break;
				case 2:
					if (j % 3 == 0)
						record = g_BacktrackHelper->PlayerRecord[tempTarget->GetIndex()].records.at(j);
					break;
				case 3:
					if (j % 2 == 0)
						record = g_BacktrackHelper->PlayerRecord[tempTarget->GetIndex()].records.at(j);
					break;
				case 4:
					record = g_BacktrackHelper->PlayerRecord[tempTarget->GetIndex()].records.at(j);
					break;
				}
			}
		}
		else if (!c_config::get()->m["rage_lowfps"][3]) {
			record = g_BacktrackHelper->PlayerRecord[tempTarget->GetIndex()].records.at(0);
		}
		
	}
	if (g_BacktrackHelper->IsTickValid(record))
	{
		if ((cfg->m["rage_points"][0] && !force_baim && !c_keyhandler::get()->auto_check("rage_safepoint_key"))) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_HEAD) });
			if (c_config::get()->m["rage_multipoint"][0] && c_keyhandler::get()->auto_check("rage_multipoint_key") && !c_keyhandler::get()->auto_check("rage_safepoint_key")) {
				multipoints(HITBOX_HEAD, record.boneMatrix, points);
			}
		}

		if (cfg->m["rage_points"][1]) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_CHEST) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_UPPER_CHEST) });
			if (c_config::get()->m["rage_multipoint"][1] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
				multipoints(HITBOX_CHEST, record.boneMatrix, points);
				multipoints(HITBOX_UPPER_CHEST, record.boneMatrix, points);
			}
		}

		if ((cfg->m["rage_points"][2] || force_baim && !c_keyhandler::get()->auto_check("rage_safepoint_key"))) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_BODY) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_PELVIS) });
			if (c_config::get()->m["rage_multipoint"][2] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
				multipoints(HITBOX_BODY, record.boneMatrix, points);
				multipoints(HITBOX_PELVIS, record.boneMatrix, points);
			}
		}

		if (cfg->m["rage_points"][3] && !c_keyhandler::get()->auto_check("rage_safepoint_key") && !cfg->b["rage_safepoint_limbs"]) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOREARM) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOREARM) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_UPPER_ARM) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_UPPER_ARM) });
			if (c_config::get()->m["rage_multipoint"][3] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
				if (c_config::get()->m["rage_lowfps"][0] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
					multipoints(HITBOX_LEFT_FOREARM, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_FOREARM, record.boneMatrix, points);
					multipoints(HITBOX_LEFT_UPPER_ARM, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_UPPER_ARM, record.boneMatrix, points);
				}
				else if (!c_config::get()->m["rage_lowfps"][0]) {
					multipoints(HITBOX_LEFT_FOREARM, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_FOREARM, record.boneMatrix, points);
					multipoints(HITBOX_LEFT_UPPER_ARM, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_UPPER_ARM, record.boneMatrix, points);
				}
			}
		}
		if (cfg->m["rage_points"][4] && !c_keyhandler::get()->auto_check("rage_safepoint_key") && !cfg->b["rage_safepoint_limbs"]) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_CALF) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_CALF) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_THIGH) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_THIGH) });
			if (c_config::get()->m["rage_multipoint"][4] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
				if (c_config::get()->m["rage_lowfps"][1] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
					multipoints(HITBOX_LEFT_CALF, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_CALF, record.boneMatrix, points);
					multipoints(HITBOX_LEFT_THIGH, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_THIGH, record.boneMatrix, points);
				}
				else if (!c_config::get()->m["rage_lowfps"][1]) {
					multipoints(HITBOX_LEFT_CALF, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_CALF, record.boneMatrix, points);
					multipoints(HITBOX_LEFT_THIGH, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_THIGH, record.boneMatrix, points);
				}
			}
		}

		if (cfg->m["rage_points"][5] && !c_keyhandler::get()->auto_check("rage_safepoint_key") && !cfg->b["rage_safepoint_limbs"]) {
			if (c_config::get()->m["rage_lowfps"][2] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
				points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOOT) });
				points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOOT) });
				if (c_config::get()->m["rage_multipoint"][5] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
					multipoints(HITBOX_LEFT_FOOT, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_FOOT, record.boneMatrix, points);
				}
			}
			else if (!c_config::get()->m["rage_lowfps"][2]) {
				points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOOT) });
				points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOOT) });
				if (c_config::get()->m["rage_multipoint"][5] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
					multipoints(HITBOX_LEFT_FOOT, record.boneMatrix, points);
					multipoints(HITBOX_RIGHT_FOOT, record.boneMatrix, points);
				}
			}	
		}
	}

	matrix3x4_t bones[128];
	tempTarget->SetupBones(bones, 128, 256, Interfaces::Globals->curtime);

	if ((cfg->m["rage_points"][0] && !force_baim && (!c_keyhandler::get()->auto_check("rage_safepoint_key")))) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_HEAD) });
		if (c_config::get()->m["rage_multipoint"][0] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
			multipoints(HITBOX_HEAD, bones, points);
		}
	}

	if (cfg->m["rage_points"][1]) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_CHEST) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_UPPER_CHEST) });
		if (c_config::get()->m["rage_multipoint"][1] && c_keyhandler::get()->auto_check("rage_multipoint_key")){
			multipoints(HITBOX_CHEST, bones, points);
			multipoints(HITBOX_UPPER_CHEST, bones, points);
		}
	}

	if ((cfg->m["rage_points"][2] || force_baim) && !c_keyhandler::get()->auto_check("rage_safepoint_key")) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_BODY) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_PELVIS) });
		if (c_config::get()->m["rage_multipoint"][2] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
			multipoints(HITBOX_BODY, bones, points);
			multipoints(HITBOX_PELVIS, bones, points);
		}
	}

	if (cfg->m["rage_points"][3] && !c_keyhandler::get()->auto_check("rage_safepoint_key") && !cfg->b["rage_safepoint_limbs"]) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOREARM) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOREARM) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_UPPER_ARM) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_UPPER_ARM) });
		if (c_config::get()->m["rage_multipoint"][3] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
			if (c_config::get()->m["rage_lowfps"][0] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
				multipoints(HITBOX_LEFT_FOREARM, bones, points);
				multipoints(HITBOX_RIGHT_FOREARM, bones, points);
				multipoints(HITBOX_LEFT_UPPER_ARM, bones, points);
				multipoints(HITBOX_RIGHT_UPPER_ARM, bones, points);
			}
			else if (!c_config::get()->m["rage_lowfps"][0]) {
				multipoints(HITBOX_LEFT_FOREARM, bones, points);
				multipoints(HITBOX_RIGHT_FOREARM, bones, points);
				multipoints(HITBOX_LEFT_UPPER_ARM, bones, points);
				multipoints(HITBOX_RIGHT_UPPER_ARM, bones, points);
			}
		}
	}
	if (cfg->m["rage_points"][4] && !c_keyhandler::get()->auto_check("rage_safepoint_key") && !cfg->b["rage_safepoint_limbs"]) {
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_CALF) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_CALF) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_THIGH) });
		points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_THIGH) });
		if (c_config::get()->m["rage_multipoint"][4] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
			if (c_config::get()->m["rage_lowfps"][1] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
				multipoints(HITBOX_LEFT_CALF, bones, points);
				multipoints(HITBOX_RIGHT_CALF, bones, points);
				multipoints(HITBOX_LEFT_THIGH, bones, points);
				multipoints(HITBOX_RIGHT_THIGH, bones, points);
			}
			else if (!c_config::get()->m["rage_lowfps"][1]) {
				multipoints(HITBOX_LEFT_CALF, bones, points);
				multipoints(HITBOX_RIGHT_CALF, bones, points);
				multipoints(HITBOX_LEFT_THIGH, bones, points);
				multipoints(HITBOX_RIGHT_THIGH, bones, points);
			}
		}
	}

	if (cfg->m["rage_points"][5] && !c_keyhandler::get()->auto_check("rage_safepoint_key") && !cfg->b["rage_safepoint_limbs"]) {
		if (c_config::get()->m["rage_lowfps"][2] && c_fps::get()->get_fps() > (1 / Interfaces::Globals->interval_per_tick)) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOOT) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOOT) });
			if (c_config::get()->m["rage_multipoint"][5] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
				multipoints(HITBOX_LEFT_FOOT, bones, points);
				multipoints(HITBOX_RIGHT_FOOT, bones, points);
			}
		}
		else if (!c_config::get()->m["rage_lowfps"][2]) {
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_LEFT_FOOT) });
			points.push_back({ tempTarget->GetHitboxPosition(HITBOX_RIGHT_FOOT) });
			if (c_config::get()->m["rage_multipoint"][5] && c_keyhandler::get()->auto_check("rage_multipoint_key")) {
				multipoints(HITBOX_LEFT_FOOT, bones, points);
				multipoints(HITBOX_RIGHT_FOOT, bones, points);
			}
		}
	}
	auto head_selected = cfg->m["rage_points"][0];
	auto body_selected = cfg->m["rage_points"][2];
	int best_damage = 0;
	Vector best_point;
	for (auto point : points) {
		auto isvisible = G::LocalPlayer->point_visible(point);
		if (cfg->b["rage_automaticpenetration"] && !isvisible) {
			if (wepidx != WEAPON_TASER)
			{
				auto aw_dmg = c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, tempTarget).damage;
				if (wepidx == WEAPON_G3SG1 || wepidx == WEAPON_SCAR20) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_SSG08) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_AWP) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_ELITE || wepidx == WEAPON_FIVESEVEN || wepidx == WEAPON_P250 || wepidx == WEAPON_GLOCK || wepidx == WEAPON_HKP2000 || wepidx == WEAPON_CZ75A || wepidx == WEAPON_USP_SILENCER || wepidx == WEAPON_TEC9) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_REVOLVER || wepidx == WEAPON_DEAGLE) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else
				{
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;
					}
				}
			}
		}
		if (isvisible)
		{
			if (wepidx != WEAPON_TASER)
			{
				auto aw_dmg = c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, tempTarget).damage;
				if (wepidx == WEAPON_G3SG1 || wepidx == WEAPON_SCAR20) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_SSG08) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_AWP) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_ELITE || wepidx == WEAPON_FIVESEVEN || wepidx == WEAPON_P250 || wepidx == WEAPON_GLOCK || wepidx == WEAPON_HKP2000 || wepidx == WEAPON_CZ75A || wepidx == WEAPON_USP_SILENCER || wepidx == WEAPON_TEC9) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else if (wepidx == WEAPON_REVOLVER || wepidx == WEAPON_DEAGLE) {
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}
				else
				{
					if ((aw_dmg >= best_damage) && (aw_dmg > min(enemyhp, cfg->i["rage_minimaldamage1"]))) {
						best_damage = aw_dmg;
						best_point = point;

					}
				}

			}
			else
			{
				auto aw_dmg = c_autowall::get()->autowall(G::LocalPlayer->GetEyePosition(), point, G::LocalPlayer, tempTarget).damage;
				if (aw_dmg >= enemyhp + 5) {
					best_damage = aw_dmg;
					best_point = point;

				}
			}

		}

	}


	if (estimated_damage)
		*estimated_damage = best_damage;

	if (best_damage >= c_config::get()->i["rage_minimaldamage1"]) {
		if (c_config::get()->b["rage_quickstop"] && !c_config::get()->m["rage_quickstop_options"][0]) {
			bool canShoot = (G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() <= Interfaces::Globals->curtime && G::LocalPlayer->GetActiveWeapon()->GetAmmoInClip() > 0);
			if (c_config::get()->m["rage_quickstop_options"][1]) {

				if (c_config::get()->m["rage_quickstop_options"][4] && !canShoot)
					goto jmp;

				if (!c_config::get()->m["rage_quickstop_options"][5] && G::LocalPlayer->GetFlags() & FL_ONFIRE)
					goto jmp;

				auto weapon_handle = G::LocalPlayer->GetActiveWeapon();

				float amount = 0.30f;
				Vector velocity = G::LocalPlayer->GetVelocity();
				Vector direction;
				Utilities->Math_VectorAngles(velocity, direction);
				float speed = velocity.Length2D();
				direction.y = G::UserCmd->viewangles.y - direction.y;
				Vector forward;
				Utilities->Math_AngleVectors(direction, forward);
				Vector source = forward * -speed;

				if (speed >= (weapon_handle->get_full_info()->max_speed * amount))
				{
					G::UserCmd->forwardmove = source.x;
					G::UserCmd->sidemove = source.y;

				}

				if (c_config::get()->m["rage_quickstop_options"][2])
					G::UserCmd->buttons |= IN_DUCK;

				static int cnt = 0;
				static bool do_ = false;
				if (c_config::get()->m["rage_quickstop_options"][3] && (!c_config::get()->b["rage_doubletap"])) {
					G::SendPacket = false;
					G::FakelagShouldLag = false;
					G::IsDoubleTapShooting = true;
					if (cnt % 14 == 0)
						do_ = true;
					else if (cnt % 14 == 6)
						G::SendPacket = true;
					else if (cnt % 14 == 7)
						do_ = false;

					if (do_)
						G::UserCmd->buttons |= IN_DUCK;
					else
						G::UserCmd->buttons &= ~IN_DUCK;

					cnt++;
				}
			}
			else {
				if (c_config::get()->m["rage_quickstop_options"][4] && !canShoot)
					goto jmp;

				if (!c_config::get()->m["rage_quickstop_options"][5] && G::LocalPlayer->GetFlags() & FL_ONFIRE)
					goto jmp;

				if (G::UserCmd->buttons & IN_MOVELEFT)
				{
					G::UserCmd->buttons |= IN_MOVERIGHT;
				}

				if (G::UserCmd->buttons & IN_MOVERIGHT)
				{
					G::UserCmd->buttons |= IN_MOVELEFT;
				}

				if (G::UserCmd->buttons & IN_FORWARD)
				{
					G::UserCmd->buttons |= IN_BACK;
				}

				if (G::UserCmd->buttons & IN_BACK)
				{
					G::UserCmd->buttons |= IN_FORWARD;
				}

				if (c_config::get()->m["rage_quickstop_options"][2])
					G::UserCmd->buttons |= IN_DUCK;

				static int cnt = 0;
				static bool do_ = false;
				if (c_config::get()->m["rage_quickstop_options"][3] && (!c_config::get()->b["rage_doubletap"])) {
					G::SendPacket = false;
					G::FakelagShouldLag = false;
					G::IsDoubleTapShooting = true;
					if (cnt % 14 == 0)
						do_ = true;
					else if (cnt % 14 == 6)
						G::SendPacket = true;
					else if (cnt % 14 == 7)
						do_ = false;

					if (do_)
						G::UserCmd->buttons |= IN_DUCK;
					else
						G::UserCmd->buttons &= ~IN_DUCK;

					cnt++;
				}
			}
		}

	jmp:
		return best_point;
	}
}
void AngleVectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}
void VectorAngles(const Vector &vecForward, Vector &vecAngles)
{
	Vector vecView;
	if (vecForward[1] == 0.f && vecForward[0] == 0.f)
	{
		vecView[0] = 0.f;
		vecView[1] = 0.f;
	}
	else
	{
		vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / 3.14159265358979323846f;

		if (vecView[1] < 0.f)
			vecView[1] += 360.f;

		vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

		vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / 3.14159265358979323846f;
	}

	vecAngles[0] = -vecView[0];
	vecAngles[1] = vecView[1];
	vecAngles[2] = 0.f;
}
void c_ragebot::quick_stop() 
{
	if (!c_config::get()->b["rage_quickstop"])
		return;

	Vector Velocity = G::LocalPlayer->GetVelocity();

	if (Velocity.Length2D() == 0)
		return;

	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	VectorAngles(Velocity, Direction);
	Interfaces::Engine->GetViewAngles(&RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	G::UserCmd->forwardmove = NegativeDirection.x;
	G::UserCmd->sidemove = NegativeDirection.y;
}
