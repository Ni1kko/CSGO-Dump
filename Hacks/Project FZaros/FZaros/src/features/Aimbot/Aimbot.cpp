#include "Aimbot.hpp"
#include "Autowall.hpp"
#include "../../menu/menu.hpp"

CAimbot g_Aimbot;

void restoreRecord(CBaseEntity* pEntity, tickRecord record)
{
	//pEntity->invalidateBoneCache( );
	pEntity->setEyeAngles(record.m_angEyeAngles);
	pEntity->setAbsAngles(Vector3(0, record.m_absangles.y, 0));
	pEntity->vecOrigin() = record.m_vecOrigin;
	pEntity->setVelocity(record.m_vecVelocity);

	//pEntity->setFlags( record.m_nFlags ); //causing weird stuff, idk..
}

void applyRestore(CBaseEntity* pEntity, float curtime)
{
	pEntity->invalidateBoneCache();

	restoreRecord(pEntity, g_Aimbot.record[pEntity->EntIndex()]);

	pEntity->updateClientAnimation();
	pEntity->SetupBones(NULL, 128, 256, curtime);
}

void restoreToCurrent(CBaseEntity* pEntity)
{
	if (g_LagCompensation.PlayerRecord[pEntity->EntIndex()].records.empty())
		return;

	applyRestore(pEntity, g_Interfaces->globalVars->curtime);
}

void CAimbot::creatMove()
{
	if (!config->get_bool("rageEnable"))
		return;

	if (!g_Interfaces->gameEngine->inGame() || !Globals::localPlayer)
		return;

	Vector3 Aimpoint = { 0,0,0 };
	CBaseEntity* Target = nullptr;

	int targetID = 0;
	int tempDmg = 0;
	static bool shot = false;

	for (int i = 1; i <= g_Interfaces->globalVars->maxClients; ++i)
	{
		CBaseEntity* pEntity = g_Interfaces->clientEntity->GetClientEntity(i);

		if (!pEntity
			|| !pEntity->alive()
			|| pEntity->IsDormant())
		{
			continue;
		}

		if (pEntity == Globals::localPlayer || pEntity->team() == Globals::localPlayer->team() || pEntity->immunity())
			continue;

		Globals::enemyEyeAngles[i] = pEntity->eyeAngles();

		if (!Globals::localPlayer->alive())
			continue;

		if (config->get_bool("rageBacktrack"))
		{
			for (int j = g_LagCompensation.PlayerRecord[pEntity->EntIndex()].records.size() - 1; j >= 0; j--)
			{
				switch (config->get_int("rageBacktrackType"))
				{
				case 0:
					record[pEntity->EntIndex()] = g_LagCompensation.PlayerRecord[pEntity->EntIndex()].records.at(0);
					break;
				case 1:
					record[pEntity->EntIndex()] = g_LagCompensation.PlayerRecord[pEntity->EntIndex()].records.at(j);
					break;
				}
			}
		}

		//pEntity->fixSetupBones( matrix[i], g_Interfaces->globalVars->curtime );
		pEntity->SetupBones(matrix[i], 128, 256, g_Interfaces->globalVars->curtime); //feels more accurate

		if (config->get_bool("rageBacktrack"))
			restoreToCurrent(pEntity);

		if (!Globals::localPlayer->activeWeapon() || Globals::localPlayer->isKnifeorNade())
			continue;

		float flServerTime = Globals::localPlayer->tickBase() * g_Interfaces->globalVars->intervalPerTick;
		bool canShoot = (Globals::localPlayer->activeWeapon()->nextPrimaryAttack() <= flServerTime);

		if (!canShoot)
			continue;

		bestEntDmg = 0;

		Vector3 Hitbox = hitScan(pEntity);

		if (Hitbox != Vector3(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;
		}
	}

	if (!Globals::localPlayer->alive())
	{
		shot = false;
		return;
	}

	if (!Globals::localPlayer->activeWeapon() || Globals::localPlayer->isKnifeorNade())
	{
		shot = false;
		return;
	}

	if (shot)
	{
		shot = false;
	}

	float flServerTime = Globals::localPlayer->tickBase() * g_Interfaces->globalVars->intervalPerTick;
	bool canShoot = (Globals::localPlayer->activeWeapon()->nextPrimaryAttack() <= flServerTime);

	if (Target)
	{
		Globals::targetIndex = targetID;

		Vector3 Angle = g_Math.calcAngle(Globals::localPlayer->eyePosition(), Aimpoint);
		bool hitchanced = hitChance(Target, Globals::localPlayer->activeWeapon(), Angle, Aimpoint, config->get_int("rageHitchanceValue"));

		if (config->get_bool("rageAutomaticScope") && Globals::localPlayer->activeWeapon()->hasScope() && !(Globals::oCmd->buttons & IN_ATTACK2) && !Globals::localPlayer->scoped() && canShoot && !hitchanced)
			Globals::oCmd->buttons |= IN_ATTACK2; //sniper check....

		static int MinimumVelocity = 0;

		if (config->get_int("rageAutomaticStopType") == 0)
		{
			MinimumVelocity = Globals::localPlayer->activeWeapon()->weaponData()->maxSpeedAlt * .34f;
		}
		else if (config->get_int("rageAutomaticStopType") == 1)
		{
			MinimumVelocity = 0;
		}

		if (Globals::localPlayer->velocity().Length() >= MinimumVelocity && (Globals::localPlayer->flags() & FL_ONGROUND))
			autoStop();

		if (!(Globals::oCmd->buttons & IN_ATTACK) && canShoot && hitchanced)
		{
			if (!backtrack[targetID] && !shotBacktrack[targetID])
				Globals::Shot[targetID] = true;

			Globals::bSendPacket = true;

			Globals::oCmd->viewAngles = Angle - (Globals::localPlayer->aimPunchAngle() * g_Interfaces->cvar->FindVar("weapon_recoil_scale")->GetFloat());

			if (config->get_bool("rageAutomaticFire"))
			{
				Globals::oCmd->buttons |= IN_ATTACK;
			}

			shot = true;

			Globals::oCmd->tickCount = TIME_TO_TICKS(simtime[targetID]) + TIME_TO_TICKS(g_LagCompensation.getLerpTime());

			if (!config->get_bool("rageFakeDuck") && !menu->get_hotkey("rageFakeDuckKey"))
				Globals::bSendPacket = true;
			else
				Globals::bSendPacket = false;

			backtrack[targetID] = false;
		}
	}
}


void CAimbot::autoStop()
{
	if (!config->get_bool("rageAutomaticStop") || (menu->get_hotkey("aaSlowMotionKey") && config->get_bool("aaSlowMotion")))
		return;

	Vector3 Velocity = Globals::localPlayer->velocity();
	static float Speed = 450.f;

	Vector3 Direction;
	Vector3 RealView;
	g_Math.vectorAngles(Velocity, Direction);
	g_Interfaces->gameEngine->getViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector3 Forward;
	g_Math.angleVectors(Direction, &Forward);
	Vector3 NegativeDirection = Forward * -Speed;

	Globals::oCmd->forwardmove = NegativeDirection.x;
	Globals::oCmd->sidemove = NegativeDirection.y;
}


bool CAimbot::hitChance(CBaseEntity* pEnt, CBaseCombatWeapon* pWeapon, Vector3 Angle, Vector3 Point, int chance)
{
	if (chance == 0 || !config->get_int("rageHitchanceValue"))
		return true;

	int traces_hit = 0;

	if (!Globals::localPlayer)
		return false;

	Vector3 forward, right, up, eye_position = Globals::localPlayer->eyePosition();
	g_Math.angleVectors(Angle + Globals::localPlayer->aimPunchAngle() * 2.f, &forward, &right, &up);// maybe add an option to not account for punch.

	if (!pWeapon)
		return false;

	pWeapon->accuracyPenalty();
	float weapon_spread = pWeapon->spread();
	float weapon_cone = pWeapon->inaccuracy();

	auto get_bullet_location = [&](int seed)
	{
		static auto random_seed = (void(*)(int))GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed");
		random_seed(seed);

		float a = g_Math.randomFloatRange(0.f, 1.f);
		float b = g_Math.randomFloatRange(0.f, 2.f * PI);
		float c = g_Math.randomFloatRange(0.f, 1.f);
		float d = g_Math.randomFloatRange(0.f, 2.f * PI);

		float generated_spread = a * weapon_spread, generated_cone = c * weapon_cone;

		Vector3 spread = Vector3(
			std::cos(b) * generated_spread + std::cos(d) * generated_cone,
			std::sin(b) * generated_spread + std::sin(d) * generated_cone,
			0.f
		);

		return Vector3(forward + right * -spread.x + up * -spread.y).Normalize();
	};

	for (int i = 1; i <= 256; i++)
	{
		Vector3 spread_angle;
		Vector3 bullet_end;

		g_Math.vectorAngles(get_bullet_location(i), spread_angle);
		g_Math.AngToVec(Angle - (spread_angle - Angle), bullet_end);

		CGameTrace trace;
		C_Ray ray;
		ray.Init(eye_position, eye_position + bullet_end * pWeapon->weaponData()->range);
		g_Interfaces->trace->ClipRayToEntity(ray, MASK_SHOT, pEnt, &trace);

		if (trace.m_pEnt == pEnt)
			++traces_hit;


		if (traces_hit >= (int)(config->get_int("rageHitchanceValue") * 2.56f))
			return true;
	}

	return false;
}

bool shouldBaim(CBaseEntity* pEnt) // probably dosnt make sense
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->simulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->simulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->activeWeapon() && !pEnt->isKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->activeWeapon()->lastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->activeWeapon()->lastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (config->get_bool("rageBodyAimAir") && !(pEnt->flags() & FL_ONGROUND))
		return true;

	if (menu->get_hotkey("rageBodyAimKey"))
		return true;

	if (Globals::MissedShots[pEnt->EntIndex()] > config->get_int("rageBodyAimShotsAmount") && config->get_bool("rageBodyAimShots"))
		return true;

	return false;
}

Vector3 CAimbot::hitScan(CBaseEntity* pEnt) // supremeemmemememememe
{
	float DamageArray[28];
	float tempDmg = 0.f;
	Vector3 tempHitbox = { 0,0,0 };
	static int HitboxForMuti[] = { 2,2,4,4,6,6 };

	float angToLocal = g_Math.calcAngle(Globals::localPlayer->vecOrigin(), pEnt->vecOrigin()).y;

	Vector2 MutipointXY = { (sin(g_Math.GRD_TO_BOG(angToLocal))),(cos(g_Math.GRD_TO_BOG(angToLocal))) };
	Vector2 MutipointXY180 = { (sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };
	Vector2 Mutipoint[] = { Vector2(MutipointXY.x, MutipointXY.y), Vector2(MutipointXY180.x, MutipointXY180.y) };

	float Velocity = abs(pEnt->velocity().Length2D());

	std::vector<int> Scan;

	int HeadHeight = 0;

	bool Baim = shouldBaim(pEnt);

	bool AvoidHeadBool = false;
	bool AvoidLimbsBool = false;

	if (config->get_bool("rageAvoidHeadJumping"))
	{ 
		if ( pEnt->flags() & FL_ONGROUND )
			AvoidHeadBool = true;
		else
			AvoidHeadBool = false;
	}
	else
		AvoidHeadBool = false;

	if (config->get_bool("rageAvoidLimbsMoving"))
	{
		if (Velocity > 0.f)
			AvoidLimbsBool = true;
		else
			AvoidLimbsBool = false;
	}
	else
		AvoidLimbsBool = false;

	if (!Baim && (config->get_int("rageHitboxes") & (1 << 1)))
		Scan.push_back(HITBOX_HEAD);

	if (!Baim && (config->get_int("rageHitboxes") & (1 << 1)) && !AvoidHeadBool && (config->get_int("rageMultipoints") & (1 << 1)))
	{
		Scan.push_back(25);//head
		Scan.push_back(26);
		Scan.push_back(27);
	}

	if (config->get_int("rageHitboxes") & (1 << 2))
	{
		Scan.push_back(HITBOX_PELVIS);
		Scan.push_back(HITBOX_THORAX);
		Scan.push_back(HITBOX_LOWER_CHEST);
		Scan.push_back(HITBOX_UPPER_CHEST);
	}

	if (config->get_int("rageHitboxes") & (1 << 2) && (config->get_int("rageHitboxes") & (1 << 2)))
	{
		Scan.push_back(19);//pelvis
		Scan.push_back(20);

		Scan.push_back(21);//thorax
		Scan.push_back(22);

		Scan.push_back(23);//upperchest
		Scan.push_back(24);
	}

	if (config->get_int("rageHitboxes") & (1 << 3))
		Scan.push_back(HITBOX_BELLY);

	HeadHeight = config->get_int("rageHeadScale");

	if (!AvoidLimbsBool && config->get_int("rageHitboxes") & (1 << 4))
	{
		Scan.push_back(HITBOX_LEFT_UPPER_ARM);
		Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
	}

	if (!AvoidLimbsBool && config->get_int("rageHitboxes") & (1 << 5))
	{
		Scan.push_back(HITBOX_LEFT_CALF);
		Scan.push_back(HITBOX_RIGHT_CALF);

		Scan.push_back(HITBOX_LEFT_THIGH);
		Scan.push_back(HITBOX_RIGHT_THIGH);
	}

	if (!AvoidLimbsBool && config->get_int("rageHitboxes") & (1 << 6))
	{
		Scan.push_back(HITBOX_LEFT_FOOT);
		Scan.push_back(HITBOX_RIGHT_FOOT);
	}

	Vector3 Hitbox;
	int bestHitboxint = 0;

	for (int hitbox : Scan)
	{
		if (hitbox < 19)
			Hitbox = pEnt->hitboxPosition(hitbox, matrix[pEnt->EntIndex()]);
		else if (hitbox > 21 && hitbox < 28)
		{

			float Radius = 0;
			Hitbox = pEnt->hitboxPosition(HitboxForMuti[hitbox - 22], matrix[pEnt->EntIndex()], &Radius);
			Radius *= (config->get_int("ragePointScale") / 100.f);
			Hitbox = Vector3(Hitbox.x + (Radius * Mutipoint[((hitbox - 22) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 22) % 2)].y), Hitbox.z);
		}
		else if (hitbox > 27 && hitbox < 32)
		{
			float Radius = 0;
			Hitbox = pEnt->hitboxPosition(0, matrix[pEnt->EntIndex()], &Radius);
			Radius *= (HeadHeight / 100.f);

			if (hitbox != 31)
				Hitbox = Vector3(Hitbox.x + (Radius * Mutipoint[((hitbox - 28) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 28) % 2)].y), Hitbox.z);
			else
				Hitbox += Vector3(0, 0, Radius);
		}

		float Damage = g_Autowall.CanHit(Hitbox);

		if (Damage > 0.f)
			DamageArray[hitbox] = Damage;
		else
			DamageArray[hitbox] = 0;

		if (config->get_bool("rageBodyAimLethal") && hitbox != 0 && hitbox != 25 && hitbox != 26 && hitbox != 27 && Damage >= (pEnt->health() + 10))
		{
			DamageArray[hitbox] = 400;
			Baim = true;
		}

		if (DamageArray[hitbox] >= tempDmg)
		{
			tempHitbox = Hitbox;
			bestHitboxint = hitbox;
			tempDmg = DamageArray[hitbox];
		}

		Globals::aimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;
	}


	float dmg = config->get_int("rageMinDamageValue") == 100 ? pEnt->health() : config->get_int("rageMinDamageValue");
	if (tempDmg >= dmg)
	{
		bestEntDmg = tempDmg;

		if (!Baim && (bestHitboxint == 28 || bestHitboxint == 29 || bestHitboxint == 30) && abs(DamageArray[HITBOX_HEAD] - DamageArray[bestHitboxint]) <= 10.f)
			return pEnt->hitboxPosition(HITBOX_HEAD, matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 22 || bestHitboxint == 23) && DamageArray[HITBOX_PELVIS] > 30)
			return pEnt->hitboxPosition(HITBOX_PELVIS, matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 24 || bestHitboxint == 25) && DamageArray[HITBOX_THORAX] > 30)
			return pEnt->hitboxPosition(HITBOX_THORAX, matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 26 || bestHitboxint == 27) && DamageArray[HITBOX_UPPER_CHEST] > 30)
			return pEnt->hitboxPosition(HITBOX_UPPER_CHEST, matrix[pEnt->EntIndex()]);

		simtime[pEnt->EntIndex()] = pEnt->simulationTime();

		return tempHitbox;
	}

	backtrack[pEnt->EntIndex()] = false;

	if (g_LagCompensation.isTickValid(g_Aimbot.record[pEnt->EntIndex()]))
	{
		if (config->get_bool("rageBacktrack"))
		{
			if (g_Autowall.CanHitFloatingPoint(pEnt->hitboxPosition(HITBOX_HEAD, g_Aimbot.record[pEnt->EntIndex()].boneMatrix), Globals::localPlayer->eyePosition()) && !backtrack[pEnt->EntIndex()])
			{
				bestEntDmg = (100000.f - fabs(g_Math.Distance(Vector2(Globals::localPlayer->vecOrigin().x, Globals::localPlayer->vecOrigin().y), Vector2(pEnt->vecOrigin().x, pEnt->vecOrigin().y))));
				simtime[pEnt->EntIndex()] = g_Aimbot.record[pEnt->EntIndex()].m_flSimulationTime;

				backtrack[pEnt->EntIndex()] = true;

				restoreToCurrent(pEnt);

				return pEnt->hitboxPosition(HITBOX_HEAD, g_Aimbot.record[pEnt->EntIndex()].boneMatrix);
			}
		}
	}

	return Vector3(0, 0, 0);
}
