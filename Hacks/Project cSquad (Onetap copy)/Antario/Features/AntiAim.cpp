#include "AntiAim.h"
#include "Aimbot.h"


bool IsWeaponGrenade4(C_BaseCombatWeapon* weapon)
{
	if (weapon == nullptr) return false;
	int id = weapon->sGetItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INCGRENADE };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool IsBallisticWeapon4(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseEntity* weaponEnt = (C_BaseEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)EClassIds::CKnife || pWeaponClass->m_ClassID == (int)EClassIds::CHEGrenade ||
		pWeaponClass->m_ClassID == (int)EClassIds::CDecoyGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CIncendiaryGrenade ||
		pWeaponClass->m_ClassID == (int)EClassIds::CSmokeGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CC4 ||
		pWeaponClass->m_ClassID == (int)EClassIds::CMolotovGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CFlashbang)
		return false;
	else
		return true;
}


void NormalizeNum(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float fov_player(Vector ViewOffSet, Vector View, C_BaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = aimbot->GetHitboxPosition(entity, hitbox);

	Utils::AngleVectors(Angles, &Forward);
	VectorSubtract(AimPos, Origin, Delta);
	NormalizeNum(Delta, Delta);

	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	C_BaseEntity* local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetEyeOrigin();

	Vector angles;
	g_pEngine->GetViewAngles(angles);

	for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
	{
		C_BaseEntity *entity = g_pEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->IsDormant() || entity == local_player)
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

float get_curtime(CUserCmd* ucmd) 
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * g_pGlobalVars->intervalPerTick;
	return curtime;
}


bool next_lby_update(CUserCmd* cmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->GetAnimState();
	if (!animstate)
		return false;

	auto net_channel = g_pEngine->GetNetChannel();

	if (!net_channel || net_channel->m_nChokedPackets)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->m_velocity > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}
	return false;
}

void autoDirection(CUserCmd* cmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return;

	static bool shark = false;
	shark = !shark;
	static float last_real;
	bool no_active = true;
	int hold = 0;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetEyeOrigin();
	auto headpos = aimbot->GetHitboxPosition(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](C_BaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetEyePosition();
		Ray_t ray;
		ray.Init(newhead, eyepos);
		CTraceFilterSkipTwoEntities filter(pPlayer, g::pLocalEntity);
		trace_t trace1, trace2;
		g_pTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		g_pTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.endpos;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();
	auto entity = g_pEntityList->GetClientEntity(index);

	if (!local_player->IsAlive())
		hold = 0.f;

	if (entity == nullptr)
	{
		if (hold != 0.f)
			g::pCmd->viewangles.y = hold;
		else
		{
			g::pCmd->viewangles.y -= 180.f;
			return;
		}
	}

	float step = (2 * M_PI) / 18.f;
	float radius = Vector(headpos - origin).Length2D();

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float besthead = 0; besthead < 6; besthead += 0.1)
		{
			Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
			float totalthickness = 0.f;
			no_active = false;
			totalthickness += checkWallThickness(entity, newhead);
			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;

				hold = bestrotation;
					
				bestrotation = besthead;
			}
		}
	}
	if (no_active)
	{
		g::pCmd->viewangles.y -= 180.f;
	}
	else if(bestrotation > 0)
		cmd->viewangles.y = RAD2DEG(bestrotation);
	else if(bestrotation <= 0)
	{
		cmd->viewangles.y += shark ? 175 : -175;
	}
}

float MaxDesyncDelta()
{
	auto animstate = g::pLocalEntity->GetAnimState();
	float v49;
	float v46;
	float v51;

	if (animstate->m_flFeetSpeedForwardsOrSideWays >= 0.0)
		v46 = fminf(animstate->m_flFeetSpeedForwardsOrSideWays, 1.0);
	else
		v46 = 0.0;

	float v47 = (float)((animstate->m_flStopToFullRunningFraction * -0.30000001f) - 0.19999999f) * v46;
	v49 = v47 + 1.0;
	if (animstate->m_fDuckAmount > 0.0)
	{
		if (animstate->m_flFeetSpeedForwardsOrSideWays >= 0.0)
			v51 = fminf(animstate->m_flFeetSpeedForwardsOrSideWays, 1.0);
		else
			v51 = 0.0;
		float v52 = animstate->m_fDuckAmount * v51;
		v49 = v49 + (float)(v52 * (float)(0.5 - v49));
	}

	float v53 = *(float*)(animstate + 0x334) * v49;
	return v53;
}

bool lbyUpdateNEW()
{
	static float lby_update = 0;
	const float current_time = g::pLocalEntity->GetTickBase()* g_pGlobalVars->intervalPerTick;

	if (g::pLocalEntity->GetAnimState()->m_velocity > 0.0f || abs(g::pLocalEntity->GetAnimState()->flUpVelocity) > 100.0f) {
		lby_update = current_time + 0.22;
		return false;
	}
	else if (abs(g::pCmd->viewangles.y - g::pLocalEntity->GetAnimState()->feetyaw()) > 35.0f) {
		if ((lby_update < current_time))
		{
			lby_update = current_time + 1.1;
			return true;
		}
	}
}


float DesyncClamp(float Yaw)
{
	if (Yaw > 180)
	{
		Yaw -= (round(Yaw / 360) * 360.f);
	}
	else if (Yaw < -180)
	{
		Yaw += (round(Yaw / 360) * -360.f);
	}
	return Yaw;
}


void aimAtPlayer(CUserCmd *pCmd)
{
	
}

void AntiAimYaw(CUserCmd* pCmd) 
{
	switch (g_Settings.iYaw)
	{
	case 0:
		break;
	case 1:
		pCmd->viewangles.y += 180.f;
		break;
	case 2:
		autoDirection(pCmd);
		break;
	case 3:
	{
		static bool left, right, back;
		if (GetAsyncKeyState(VK_LEFT))
		{
			left = true;
			right = false;
			back = false;
		}
		else if (GetAsyncKeyState(VK_RIGHT))
		{
			left = false;
			right = true;
			back = false;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			left = false;
			right = false;
			back = true;
		}
		if (left)
			pCmd->viewangles.y += 90;
		else if (right)
			pCmd->viewangles.y -= 90;
		else if (back)
			pCmd->viewangles.y += 180;
		else
			pCmd->viewangles.y += 180;
	}
	break;
	}
}

void Antiaim::Do(CUserCmd* pCmd)
{
	if (!g_Settings.bAntiAim)
		return;

	if (g_Settings.iPitch > 0 || g_Settings.iYaw > 0)
	{
		static QAngle angles;

		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;

		if (!local_player->IsAlive())
			return;

		if (g::pLocalEntity->GetFlags() & FL_ATCONTROLS)
			return;

		if (g::pCmd->buttons & IN_USE)
			return;

		C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();

		if (!weapon)
			return;

		if (g::pLocalEntity->GetMoveType() == MOVETYPE_LADDER)
			return;

		if (Globals::dontdoaa)
			return;

		if (IsWeaponGrenade4(weapon))
		{
			if (!weapon->IsPinPulled() || (pCmd->buttons & IN_ATTACK) || (pCmd->buttons & IN_ATTACK2))
			{
				float throwTime = weapon->GetThrowTime();

				if (throwTime > 0)
					return;
			}
		}
		else if (pCmd->buttons & IN_ATTACK && aimbot->CanShootAntiaim(pCmd) && !weapon->GetAmmo() == 0)
		{
			if (weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_C4)
				return;
		}
		else if (pCmd->buttons & IN_ATTACK2 && aimbot->CanShootAntiaim(pCmd) && !IsBallisticWeapon4(weapon))
		{
			if (weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_C4)
				return;
		}	static float maxDesyncDelta = MaxDesyncDelta();
		static float angleBeforeLbyUpdate = 0.f,
			realAngle = 0.f;

		realAngle = pCmd->viewangles.y;

		aimAtPlayer(pCmd);

		pCmd->viewangles.x = 89;

		if (g_Settings.bDesync && g_Settings.DesyncType != 0)
		{
			switch (g_Settings.DesyncType)
			{
			case 1:
			{
				if (next_lby_update(pCmd) && !Globals::bsendpacket)
					pCmd->viewangles.y = pCmd->viewangles.y - 119;
				else
				{
					if (pCmd->command_number % 3 && Globals::bsendpacket)
					{
						pCmd->viewangles.y += pCmd->viewangles.y - MaxDesyncDelta() + 30;
					}
					else
					{
						switch (g_Settings.iYaw)
						{
						case 0:
							break;
						case 1:
							pCmd->viewangles.y += 180.f;
							break;
						case 2:
							autoDirection(pCmd);
							break;
						case 3:
						{
							static bool left, right, back;
							if (GetAsyncKeyState(VK_LEFT))
							{
								left = true;
								right = false;
								back = false;
							}
							else if (GetAsyncKeyState(VK_RIGHT))
							{
								left = false;
								right = true;
								back = false;
							}
							else if (GetAsyncKeyState(VK_DOWN))
							{
								left = false;
								right = false;
								back = true;
							}
							if (left)
								pCmd->viewangles.y += 90;
							else if (right)
								pCmd->viewangles.y -= 90;
							else if (back)
							{
								pCmd->viewangles.y += 180;
							}
						}
						break;
						}
					}
				}
			}

			break;
			case 2:
			{
				static float Angle = 0.f;
				static bool sFlip = false;
				if ((g_pGlobalVars->tickcount % 100) > 1 && (g_pGlobalVars->tickcount % 100) < 50)
					sFlip = true;
				else
					sFlip = false;

				if (next_lby_update(pCmd) && !Globals::bsendpacket)
					pCmd->viewangles.y = Angle - 50;
				else
				{
					if (sFlip)
					{
						if (pCmd->command_number % 3)
						{
							pCmd->viewangles.y -= MaxDesyncDelta() - Angle;
						}
						else
							pCmd->viewangles.y += MaxDesyncDelta() + Angle;
					}
					else
					{
						switch (g_Settings.iYaw)
						{
						case 0:
							break;
						case 1:
							pCmd->viewangles.y += 180.f;
							break;
						case 2:
							autoDirection(pCmd);
							break;
						case 3:
						{
							static bool left, right, back;
							if (GetAsyncKeyState(VK_LEFT))
							{
								left = true;
								right = false;
								back = false;
							}
							else if (GetAsyncKeyState(VK_RIGHT))
							{
								left = false;
								right = true;
								back = false;
							}
							else if (GetAsyncKeyState(VK_DOWN))
							{
								left = false;
								right = false;
								back = true;
							}
							if (left)
								pCmd->viewangles.y += 90;
							else if (right)
								pCmd->viewangles.y -= 90;
							else if (back)
							{
								pCmd->viewangles.y += 180;
							}
						}
						break;
						}

						Angle = pCmd->viewangles.y;
					}
				}
			}
			break;
			case 3://static
			{
				static bool sFlip = false;
				if ((g_pGlobalVars->tickcount % 100) > 1 && (g_pGlobalVars->tickcount % 100) < 50)
					sFlip = true;
				else
					sFlip = false;

				if (next_lby_update(pCmd) && !Globals::bsendpacket)
					pCmd->viewangles.y -= 100;
				else
					pCmd->viewangles.y += sFlip ? -135 : 135;
			}
			break;
			case 4://switch
			{
				static bool jitter = false;
				if (Globals::bsendpacket)
				{
					float server_time = local_player->GetTickBase() * g_pGlobalVars->intervalPerTick;
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
								pCmd->viewangles.y = realAngle + 55;

						}
						else
						{
							if (!jitter)
								pCmd->viewangles.y = realAngle - 55;

						}
					}
					else
					{
						if (idk < 70)
						{
							if (jitter)
								pCmd->viewangles.y = realAngle - 50;
						}
						else
						{
							if (jitter)
								pCmd->viewangles.y = realAngle + 50;

						}
					}
				}

				switch (g_Settings.iYaw)
				{
				case 0:
					break;
				case 1:
					realAngle += 180.f;
					break;
				case 2:
					autoDirection(pCmd);
					break;
				case 3:
				{
					static bool left, right, back;
					if (GetAsyncKeyState(VK_LEFT))
					{
						left = true;
						right = false;
						back = false;
					}
					else if (GetAsyncKeyState(VK_RIGHT))
					{
						left = false;
						right = true;
						back = false;
					}
					else if (GetAsyncKeyState(VK_DOWN))
					{
						left = false;
						right = false;
						back = true;
					}
					if (left)
						realAngle += 90;
					else if (right)
						realAngle -= 90;
					else if (back)
					{
						realAngle += 180;
					}
				}
				break;
				}
			}
			break;
			}
		}
		else
		{
			switch (g_Settings.iYaw)
			{
			case 0:
				break;
			case 1:
				pCmd->viewangles.y += 180.f;
				break;
			case 2:
				autoDirection(pCmd);
				break;
			case 3:
			{
				static bool left, right, back;
				if (GetAsyncKeyState(VK_LEFT))
				{
					left = true;
					right = false;
					back = false;
				}
				else if (GetAsyncKeyState(VK_RIGHT))
				{
					left = false;
					right = true;
					back = false;
				}
				else if (GetAsyncKeyState(VK_DOWN))
				{
					left = false;
					right = false;
					back = true;
				}
				if (left)
					pCmd->viewangles.y += 90;
				else if (right)
					pCmd->viewangles.y -= 90;
				else if (back)
				{
					pCmd->viewangles.y += 180;
				}
			}
			break;
			}
		}
	}
}


void Antiaim::Desync(CUserCmd* pCmd)
{
}

Antiaim* antiaim = new Antiaim();