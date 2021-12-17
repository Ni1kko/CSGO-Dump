#include "Aimbot.h"
#include "Autowall.h"
#include "LagComp.h"
#include "..\AntiAim\AntiAim.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Menu.h"
#include "..\..\Menu\config.h"
Aimbot g_Aimbot;

float Aimbot::SynchronizeHitchance() {
	auto me = Globals::LocalPlayer;
	auto cmd = Globals::pCmd;

	if (!me->GetActiveWeapon())
		return 0;

	if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20 || me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1) {
		return c_config::get().auto_hitchance;
	}
	else if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08) {
		return c_config::get().scout_hitchance;
	}
	else if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AWP) {
		return c_config::get().awp_hitchance;
	}
	else if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER || me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_DEAGLE) {
		return c_config::get().heavy_pistol_hitchance;
	}
	else {
		return c_config::get().other_hitchance;
	}
}

float Aimbot::SynchronizeMinDamage() {
	auto me = Globals::LocalPlayer;
	auto cmd = Globals::pCmd;

	if (!me->GetActiveWeapon())
		return 0;

	if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20 || me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1) {
		return c_config::get().auto_mindamage;
	}
	else if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08) {
		return c_config::get().scout_mindamage;
	}
	else if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AWP) {
		return c_config::get().awp_mindamage;
	}
	else if (me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER || me->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_DEAGLE) {
		return c_config::get().heavy_pistol_mindamage;
	}
	else {
		return c_config::get().other_mindamage;
	}
}

void Aimbot::Autostop()
{
	if (!c_config::get().autostop)
		return;

	Vector Velocity = Globals::LocalPlayer->GetVelocity();

	if (Velocity.Length2D() == 0)
		return;

	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	g_Math.VectorAngles(Velocity, Direction);
	g_pEngine->GetViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	g_Math.AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	Globals::pCmd->forwardmove = NegativeDirection.x;
	Globals::pCmd->sidemove = NegativeDirection.y;
}

void SinCosBitch(float a, float* s, float*c)
{
	*s = sin(a);
	*c = cos(a);
}
void AngleVectorsv2(const Vector &angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	SinCosBitch(DEG2RAD(angles[1]), &sy, &cy);
	SinCosBitch(DEG2RAD(angles[0]), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

bool Aimbot::HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0 || SynchronizeHitchance() == 0)
		return true;

	float Seeds =  256.f;

	Angle -= (Globals::LocalPlayer->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());

	Vector forward, right, up;

	g_Math.AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize();

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles(direction, up, viewanglesSpread);
		g_Math.NormalizeAngles(viewanglesSpread);

		g_Math.AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = Globals::LocalPlayer->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->range);

		C_Trace Trace;

		g_pTrace->ClipRayToEntity(C_Ray(Globals::LocalPlayer->GetEyePosition(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
}

bool ShouldBaim(C_BaseEntity* pEnt) // probably dosnt make sense
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimulationTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimulationTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}

	auto pWeapon = Globals::LocalPlayer->GetActiveWeapon();

	std::vector<int> baim_hitboxes;


	baim_hitboxes.push_back((int)HITBOX_UPPER_CHEST);
	baim_hitboxes.push_back((int)HITBOX_LOWER_CHEST);
	baim_hitboxes.push_back((int)HITBOX_PELVIS);
	baim_hitboxes.push_back((int)HITGROUP_STOMACH);

	float Radius = 0;
	Radius *= (c_config::get().aimbot_bodypointscale / 100.f);

	float Damage = 0;
	for (auto HitBoxID : baim_hitboxes)
	{
		Damage = g_Autowall.Damage(pEnt->GetHitboxPosition(HitBoxID, g_Aimbot.Matrix[pEnt->EntIndex()], &Radius));
	}

	if (Globals::LocalPlayer->GetVelocity().Length2D() > 125 && c_config::get().prefer_bodyaim[2])
		return true;

	if (BaimShot[pEnt->EntIndex()] && c_config::get().prefer_bodyaim[3])
		return true;

	if (!(pEnt->GetFlags() & FL_ONGROUND) && c_config::get().prefer_bodyaim[0])
		return true;

	if (c_config::get().prefer_bodyaim[1] && (pEnt->GetVelocity().Length2D() > 15 && pEnt->GetVelocity().Length2D() < 150))
		return true;

	if (pEnt->GetHealth() <= Damage && c_config::get().prefer_bodyaim[4])
		return true;

	return false;
}

Vector Aimbot::Hitscan(C_BaseEntity* pEnt) // supremeemmemememememe
{
	float DamageArray[28];
	float tempDmg = 0.f;
	Vector tempHitbox = { 0,0,0 };
	static int HitboxForMuti[] = { 2,2,4,4,6,6 };

	float angToLocal = g_Math.CalcAngle(Globals::LocalPlayer->GetOrigin(), pEnt->GetOrigin()).y;

	Vector2D MutipointXY = { (sin(g_Math.GRD_TO_BOG(angToLocal))),(cos(g_Math.GRD_TO_BOG(angToLocal))) };
	Vector2D MutipointXY180 = { (sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };
	Vector2D Mutipoint[] = { Vector2D(MutipointXY.x, MutipointXY.y), Vector2D(MutipointXY180.x, MutipointXY180.y) };

	float Velocity = abs(pEnt->GetVelocity().Length2D());

	if (Velocity > 29.f)
		Velocity = 30.f;

	std::vector<int> Scan;

	int HeadHeight = 0;

	bool Baim = ShouldBaim(pEnt);

	bool PreferBaimIfLowVelcoity = (c_config::get().prefer_bodyaim_if_low_velocity && (Velocity > 15 && Velocity < 150));

	if (!Baim) {
		Scan.push_back(HITBOX_HEAD);
	}

		Scan.push_back(HITBOX_PELVIS);
		Scan.push_back(HITBOX_THORAX);
		Scan.push_back(HITBOX_LOWER_CHEST);
		Scan.push_back(HITBOX_UPPER_CHEST);


			Scan.push_back(19);//pelvis
			Scan.push_back(20);

			Scan.push_back(21);//thorax
			Scan.push_back(22);

			Scan.push_back(23);//upperchest
			Scan.push_back(24);

			if (!Baim)
			{
				Scan.push_back(25);//head
				Scan.push_back(26);
				Scan.push_back(27);
			}

			HeadHeight = c_config::get().aimbot_headpointscale;
		

			Scan.push_back(HITBOX_LEFT_FOOT);
			Scan.push_back(HITBOX_RIGHT_FOOT);
			Scan.push_back(HITBOX_LEFT_UPPER_ARM);
			Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
			Scan.push_back(HITBOX_LEFT_THIGH);
			Scan.push_back(HITBOX_RIGHT_THIGH);
		

			if (Baim && c_config::get().bodyaim_mode == 0)
				Scan.push_back(HITBOX_HEAD);


	Vector Hitbox;
	int bestHitboxint = 0;

	for (int hitbox : Scan)
	{
		if (hitbox < 19)
			Hitbox = pEnt->GetHitboxPosition(hitbox, Matrix[pEnt->EntIndex()]);
		else if (hitbox > 18 && hitbox < 25)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(HitboxForMuti[hitbox - 19], Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (c_config::get().aimbot_bodypointscale / 100.f);
			Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 19) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 19) % 2)].y), Hitbox.z);
		}
		else if (hitbox > 24 && hitbox < 28)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(0, Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (HeadHeight / 100.f);
			if (hitbox != 27)
				Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 25) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 25) % 2)].y), Hitbox.z);
			else
				Hitbox += Vector(0, 0, Radius);
		}

		float Damage = g_Autowall.Damage(Hitbox);

		if (Damage > 0.f)
			DamageArray[hitbox] = Damage;
		else
			DamageArray[hitbox] = 0;

		if (Baim && hitbox != 0 && hitbox != 25 && hitbox != 26 && hitbox != 27 && Damage >= (pEnt->GetHealth() + 10))
		{
			DamageArray[hitbox] = 400;
			Baim = true;
		}

		if (DamageArray[hitbox] > tempDmg)
		{
			tempHitbox = Hitbox;
			bestHitboxint = hitbox;
			tempDmg = DamageArray[hitbox];
		}

		Globals::AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;
	}

	PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[pEnt->EntIndex()].at(0);

	if (tempDmg >= SynchronizeMinDamage())
	{
		bestEntDmg = tempDmg;

		if ((bestHitboxint == 25 || bestHitboxint == 26 || bestHitboxint == 27) && abs(DamageArray[HITBOX_HEAD] - DamageArray[bestHitboxint]) <= 10.f)
			return pEnt->GetHitboxPosition(HITBOX_HEAD, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 19 || bestHitboxint == 20) && DamageArray[HITBOX_PELVIS] > 30)
			return pEnt->GetHitboxPosition(HITBOX_PELVIS, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 21 || bestHitboxint == 22) && DamageArray[HITBOX_THORAX] > 30)
			return pEnt->GetHitboxPosition(HITBOX_THORAX, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 23 || bestHitboxint == 24) && DamageArray[HITBOX_UPPER_CHEST] > 30)
			return pEnt->GetHitboxPosition(HITBOX_UPPER_CHEST, Matrix[pEnt->EntIndex()]);

		return tempHitbox;
	}

	return Vector(0, 0, 0);
}

void Aimbot::OnCreateMove()
{
	if (!g_pEngine->IsInGame())
		return;

	Vector Aimpoint = { 0,0,0 };
	C_BaseEntity* Target = nullptr;
	targetID = 0;

	int tempDmg = 0;
	static bool shot = false;
	CanShootWithCurrentHitchanceAndValidTarget = false;

	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity->IsImmune())
		{
			g_LagComp.ClearRecords(i);
			continue;
		}

		g_LagComp.StoreRecord(pPlayerEntity);

		if (pPlayerEntity == Globals::LocalPlayer || pPlayerEntity->GetTeam() == Globals::LocalPlayer->GetTeam())
			continue;

		Globals::EnemyEyeAngs[i] = pPlayerEntity->GetEyeAngles();

		if (g_LagComp.PlayerRecord[i].size() == 0 || !Globals::LocalPlayer->IsAlive() || !c_config::get().aimbot_enabled || g_Menu.Config.LegitBacktrack)
			continue;

		if (!Globals::LocalPlayer->GetActiveWeapon() || Globals::LocalPlayer->IsKnifeorNade())
			continue;

		bestEntDmg = 0;

		Vector Hitbox = Hitscan(pPlayerEntity);

		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;
		}
	}

	if (!Globals::LocalPlayer->IsAlive())
	{
		shot = false;
		return;
	}

	if (!Globals::LocalPlayer->GetActiveWeapon() || Globals::LocalPlayer->IsKnifeorNade())
	{
		shot = false;
		return;
	}

	if (shot)
	{
		if (c_config::get().on_shot_aa) {

			if (c_config::get().on_shot_aa_type == 0) {
				Globals::bSendPacket = true;
				g_AntiAim.OnCreateMove();
			}
			else if (c_config::get().on_shot_aa_type == 1) {
				Globals::pCmd->viewangles.x = 180 - Globals::pCmd->viewangles.x;
				Globals::pCmd->viewangles.y = Globals::pCmd->viewangles.y + 180;
			}
			
		}
		shot = false;
	}

	float flServerTime = Globals::LocalPlayer->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (Globals::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime && Globals::LocalPlayer->GetActiveWeapon()->GetAmmo() > 0);

	if (Target)
	{
		Globals::TargetIndex = targetID;

		float SimulationTime = 0.f;

		SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

		Vector Angle = g_Math.CalcAngle(Globals::LocalPlayer->GetEyePosition(), Aimpoint);

		static int MinimumVelocity = 0;

		if (c_config::get().autostop_mode == 0)
		{
			MinimumVelocity = Globals::LocalPlayer->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f;
		}
		else if (c_config::get().autostop_mode == 1)
		{
			MinimumVelocity = 0;
		}

		bool shouldstop = c_config::get().stop_inbetween_shots ? true : canShoot;

		if (Globals::LocalPlayer->GetVelocity().Length() >= MinimumVelocity && shouldstop && !GetAsyncKeyState(VK_SPACE))
			Autostop();

		auto me = Globals::LocalPlayer;
		auto cmd = Globals::pCmd;
		if ((Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20 || Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1 || Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08)) {
			if (c_config::get().auto_scope) {
				if (!me->IsScoped()) {
					cmd->buttons |= IN_ATTACK2;
					return;
				}
			}
		}

		if (canShoot && HitChance(Target, Globals::LocalPlayer->GetActiveWeapon(), Angle, Aimpoint, SynchronizeHitchance()))
		{
			CanShootWithCurrentHitchanceAndValidTarget = true;
			if (Globals::pCmd->buttons & IN_ATTACK)
				return;
			
			if (!Backtrack[targetID] && !ShotBacktrack[targetID])
				Globals::Shot[targetID] = true;

			Globals::bSendPacket = true;
			shot = true;

			Globals::pCmd->viewangles = Angle;

			Globals::pCmd->buttons |= IN_ATTACK;

			Globals::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.LerpTime());
		}
	}
}