#include "resolver.h"

/*
the trick is
u get ur raw layers from server, u know these are result from anim update
then
u simulate anims from prev anim layers
just like server would
for networked, negative fake, positive fake
then u compare
which one is closest
to the one server sent u
and thats how u can tell
*/

std::map<int, ResolverInfo> g_ResolveInfo;

CResolver g_Resolver;

float GetAimDirection(Vector& vViewAngles, CBaseEntity* pLocal, Vector& vEnd)
{
	/*positiv values are left negative right*/
	Vector vAng, vAim;
	Vector vTempView = vViewAngles;
	Vector vAimAngle;
	g_Math.CalcAngle(pLocal->GetEyePosition(), vEnd, vAimAngle);

	g_Math.AngleVector(vViewAngles, vAim);
	g_Math.AngleVector(vAimAngle, vAng);

	g_Math.NormalizeVector(vTempView);
	g_Math.NormalizeVector(vAimAngle);

	if (vTempView.y < 0.f)
		vTempView.y += 360.f;

	if (vAimAngle.y < 0.f)
		vAimAngle.y += 360.f;

	return fabs(vTempView.y - vAimAngle.y) > 180.f ? acosf(vAim.Dot(vAng)) * ((vAimAngle.y < vTempView.y) ? -1 : 1) : acosf(vAim.Dot(vAng)) * ((vAimAngle.y < vTempView.y) ? 1 : -1);
	//DEG2RAD for FOV usefull values
	/*if( fabs( vTempView.y - vAimAngle.y ) > 180.f )
	return acosf( vAim.Dot( vAng ) ) * ( ( vAimAngle.y < vTempView.y ) ? 1 : -1 ) * -1;
	else
	return acosf( vAim.Dot( vAng ) ) * ( ( vAimAngle.y < vTempView.y ) ? 1 : -1 );*/
}

float GetDamage(Vector vStart, Vector vEnd, CBaseEntity* pEnemy, CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	CAutowall::FireBulletData BulletData = CAutowall::FireBulletData(vStart);
	BulletData.filter.pSkip = pLocal;

	Vector vAngle;
	g_Math.CalcAngle(BulletData.src, vEnd, vAngle);
	g_Math.AngleVector(vAngle, BulletData.direction);
	VectorNormalize(BulletData.direction);

	if (!g_Autowall.SimulateFireBullet(pLocal, pEnemy, pWeapon, BulletData, 0))
		return 0.f;

	return BulletData.current_damage;
};

void CResolver::InitCM(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	ProcessSnapShots();

	last_eye_positions.insert(last_eye_positions.begin(), pLocal->GetEyePosition());

	if (last_eye_positions.size() > 128)
		last_eye_positions.pop_back();

	INetChannelInfo* pNetChannel = g_pEngine->GetNetChannelInfo();

	if (!pNetChannel)
		return;

	const int latency_ticks = TIME_TO_TICKS(pNetChannel->GetLatency(FLOW_OUTGOING));
	const auto latency_based_eye_pos = last_eye_positions.size() <= latency_ticks ? last_eye_positions.back() : last_eye_positions[latency_ticks];

	if (!g_Var.AntiAim.Resolve)
		return;

	for (int i = 1; i <= 32; i++)
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity(i);

		if (!pEnemy->PlayerCheck())
			continue;

		auto& resolve_record = player_resolve_records[i];

		const float height = 64;

		Vector direction_1, direction_2;
		g_Math.AngleVector(Vector(0.f, g_Math.CalcAngle(pLocal->GetOrigin(), pEnemy->GetOrigin()).y - 90.f, 0.f), direction_1);
		g_Math.AngleVector(Vector(0.f, g_Math.CalcAngle(pLocal->GetOrigin(), pEnemy->GetOrigin()).y + 90.f, 0.f), direction_2);

		g_Math.NormalizeVector(direction_1);
		g_Math.NormalizeVector(direction_2);

		const auto left_eye_pos = pEnemy->GetOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
		const auto right_eye_pos = pEnemy->GetOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);

		resolve_record.m_sAntiEdge.left_damage = GetDamage(latency_based_eye_pos, left_eye_pos, pEnemy, pLocal, pWeapon);
		resolve_record.m_sAntiEdge.right_damage = GetDamage(latency_based_eye_pos, right_eye_pos, pEnemy, pLocal, pWeapon);

		CEngineTraceClient::Ray_t ray;
		CEngineTraceClient::trace_t trace;
		CEngineTraceClient::CTraceWorldOnly filter;

		ray.Init(left_eye_pos, latency_based_eye_pos);
		g_pEngineTrace->TraceRay(ray, MASK_ALL, &filter, &trace);
		resolve_record.m_sAntiEdge.left_fraction = trace.fraction;

		ray.Init(right_eye_pos, latency_based_eye_pos);
		g_pEngineTrace->TraceRay(ray, MASK_ALL, &filter, &trace);
		resolve_record.m_sAntiEdge.right_fraction = trace.fraction;
	}
}

inline float anglemod(float a)
{
	a = (360.f / 65536) * ((int)(a*(65536.f / 360.0f)) & 65535);
	return a;
}

float ApproachAngle(float target, float value, float speed)
{
	target = anglemod(target);
	value = anglemod(value);

	float delta = target - value;

	if (speed < 0)
		speed = -speed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}

void CResolver::InitFSN(CBaseEntity* pLocal, CBaseEntity* pEnemy)
{
	//layer 6
	//playbackrate
	//in move fake dir
	float flBestValue = 9999.0f;
	CBaseEntity* pBest = nullptr;

	if (pLocal->GetHealth() <= 0)
		return;

	UpdateResolveRecord(pEnemy);

	int iIndex = pEnemy->GetIndex();

	auto& vResolve = player_resolve_records[iIndex];

	Vector vView;
	g_pEngine->GetViewAngles(vView);

	float flFov = g_Math.GetFoV(vView, pLocal->GetEyePosition(), pEnemy->GetEyePosition());

	if (flFov < flBestValue)
	{
		flBestValue = flFov;
		pBest = pEnemy;
	}

	CAnimState* pAnimState = pEnemy->GetAnimState();

	if (!pAnimState)
		return;

	static auto GetSmoothedVelocity = [](float min_delta, Vector a, Vector b) {
		Vector delta = a - b;
		float delta_length = delta.Length();

		if (delta_length <= min_delta)
		{
			Vector result;

			if (-min_delta <= delta_length)
				return a;
			else
			{
				float iradius = 1.0f / (delta_length + FLT_EPSILON);
				return b - ((delta * iradius) * min_delta);
			}
		}
		else
		{
			float iradius = 1.0f / (delta_length + FLT_EPSILON);
			return b + ((delta * iradius) * min_delta);
		}
	};

	Vector velocity = pEnemy->GetVelocity();
	float spd = velocity.LengthSqr();

	if (spd > std::powf(1.2f * 260.0f, 2.f))
	{
		Vector velocity_normalized = velocity.Normalized();
		velocity = velocity_normalized * (1.2f * 260.0f);
	}

	float m_flChokedTime = pAnimState->m_flLastClientSideAnimationUpdateTime;
	float v25 = clamp(pEnemy->DuckAmount() + pAnimState->m_fLandingDuckAdditiveSomething, 0.0f, 1.0f);
	float v26 = pAnimState->m_fDuckAmount;
	float v27 = m_flChokedTime * 6.0f;
	float v28;

	// clamp
	if ((v25 - v26) <= v27) {
		if (-v27 <= (v25 - v26))
			v28 = v25;
		else
			v28 = v26 - v27;
	}
	else {
		v28 = v26 + v27;
	}

	float flDuckAmount = clamp(v28, 0.0f, 1.0f);
	float flSendGoalFeetYaw = 0.f;

	Vector animationVelocity = GetSmoothedVelocity(m_flChokedTime * 2000.0f, velocity, pEnemy->GetVelocity());
	float speed = std::fminf(animationVelocity.Length(), 260.0f);

	float flMaxMovementSpeed = 260.0f;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if (pWeapon && pWeapon->GetWeaponData())
		flMaxMovementSpeed = std::fmaxf(pWeapon->GetWeaponData()->movement_speed_zoom, 0.001f);

	float flRunningSpeed = speed / (flMaxMovementSpeed * 0.520f);
	float flDuckingSpeed = speed / (flMaxMovementSpeed * 0.340f);

	flRunningSpeed = clamp(flRunningSpeed, 0.0f, 1.0f);

	float flYawModifier = (((pAnimState->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * flRunningSpeed) + 1.0f;

	if (flDuckAmount > 0.0f)
	{
		float flDuckingSpeed = clamp(flDuckingSpeed, 0.0f, 1.0f);
		flYawModifier += (flDuckAmount * flDuckingSpeed) * (0.5f - flYawModifier);
	}

	const float v60 = -58.f;
	const float v61 = 58.f;

	float flMinBodyYaw = std::fabsf(v60 * flYawModifier);
	float flMaxBodyYaw = std::fabsf(v61 * flYawModifier);

	float flEyeYaw = pEnemy->GetEyeAngles().y;
	float flEyeDiff = std::remainderf(flEyeYaw - flSendGoalFeetYaw, 360.f);

	if (flEyeDiff <= flMaxBodyYaw)
	{
		if (flMinBodyYaw > flEyeDiff)
			flSendGoalFeetYaw = fabs(flMinBodyYaw) + flEyeYaw;
	}
	else
		flSendGoalFeetYaw = flEyeYaw - fabs(flMaxBodyYaw);

	flSendGoalFeetYaw = std::remainderf(flSendGoalFeetYaw, 360.f);

	if (speed > 0.1f || fabs(velocity.z) > 100.0f)
	{
		flSendGoalFeetYaw = ApproachAngle(
			flEyeYaw,
			flSendGoalFeetYaw,
			((pAnimState->m_flStopToFullRunningFraction * 20.0f) + 30.0f)
			* m_flChokedTime);
	}
	else
	{
		flSendGoalFeetYaw = ApproachAngle(
			pEnemy->LowerBodyYawTarget(),
			flSendGoalFeetYaw,
			m_flChokedTime * 100.0f);
	}

	float flLeftGoalFeetYaw = flEyeYaw - flMinBodyYaw;
	float flRightGoalFeetYaw = flEyeYaw + flMaxBodyYaw;

	float flNewGoalFeetYaw;

		// override also layer 6 = moving layer 3 = standing
		// if we hit left or right store it so we can swap instantly to left or right if we missed one of those cuz he is d

	if (GetAsyncKeyState(g_Var.AntiAim.Override) && pBest)
	{
		Vector vView;
		g_pEngine->GetViewAngles(vView);

		float flToMe = g_Math.CalcAngle(pBest->GetEyePosition(), pLocal->GetEyePosition()).y;

		float flAimDir = GetAimDirection(vView, pLocal, g_Math.GetHitspot(pBest, 4));

		if (flAimDir >= 0.04)
			flNewGoalFeetYaw = flToMe - 110;
		else
			if (flAimDir <= -0.04)
				flNewGoalFeetYaw = flToMe + 110;
			else
				flNewGoalFeetYaw = flToMe + 180;
	}
	else
	{
		switch (vResolve.m_iMissedShots)
		{
		case 0:
			flNewGoalFeetYaw = flSendGoalFeetYaw;
			break;
		case 1:
			flNewGoalFeetYaw = flLeftGoalFeetYaw;
			break;
		case 2:
			flNewGoalFeetYaw = flRightGoalFeetYaw;
			break;
		case 3:
			vResolve.m_iMissedShots = 0;
			break;
		default:
			break;
		}
	}

	pAnimState->m_flGoalFeetYaw = flNewGoalFeetYaw;

	/*	auto AntiEdge = [&](int &iShotsMissed, float flYaw, float flAdd) -> float
		{
			const auto vAntiEdge = player_resolve_records[iIndex].m_sAntiEdge;
			float flNewYaw;
			static int iStage, iOldStage;

			//switched stage reset miss counter
			if (iStage != iOldStage)
			{
				iShotsMissed = 0;
				iOldStage = iStage;
			}

			if (vAntiEdge.left_damage >= 15 && vAntiEdge.right_damage >= 15)
			{
				iStage = 1;
				//too much damage to both sides bruteforce!
				switch (iShotsMissed)
				{
				case 0:
					flNewYaw = flYaw;
					break;
				case 1:
					flNewYaw = flYaw + flAdd;
					break;
				case 2:
					flNewYaw = flYaw - flAdd;
					break;
				case 3:
					iShotsMissed = 0;
					break;
				}
			}
			else
			{
				if (vAntiEdge.left_damage <= 0 && vAntiEdge.right_damage <= 0)
				{
					//no damage at all use edge
					if (vAntiEdge.right_fraction == vAntiEdge.left_fraction)
					{
						iStage = 2;
						switch (iShotsMissed)
						{
						case 0:
							flNewYaw = flYaw;
							break;
						case 1:
							flNewYaw = flYaw + flAdd;
							break;
						case 2:
							flNewYaw = flYaw - flAdd;
							break;
						case 3:
							iShotsMissed = 0;
							break;
						}
					}
					else
						if (vAntiEdge.right_fraction < vAntiEdge.left_fraction)
						{
							iStage = 3;
							//negative
							switch (iShotsMissed)
							{
							case 0:
								flNewYaw = flYaw + flAdd;
								break;
							case 1:
								flNewYaw = flYaw;
								break;
							case 2:
								flNewYaw = flYaw - flAdd;
								break;
							case 3:
								iShotsMissed = 0;
								break;
							}
						}
						else
						{
							iStage = 4;
							//positive
							switch (iShotsMissed)
							{
							case 0:
								flNewYaw = flYaw - flAdd;
								break;
							case 1:
								flNewYaw = flYaw;
								break;
							case 2:
								flNewYaw = flYaw + flAdd;
								break;
							case 3:
								iShotsMissed = 0;
								break;
							}
						}
				}
				else
				{
					//got some damage turn damage based
					if (vAntiEdge.left_damage == vAntiEdge.right_damage)
					{
						iStage = 5;
						switch (iShotsMissed)
						{
						case 0:
							flNewYaw = flYaw - flAdd;
							break;
						case 1:
							flNewYaw = flYaw + flAdd;
							break;
						case 2:
							flNewYaw = flYaw;
							break;
						case 3:
							iShotsMissed = 0;
							break;
						}
					}
					else
						if (vAntiEdge.left_damage > vAntiEdge.right_damage)
						{
							iStage = 6;
							//negative
							switch (iShotsMissed)
							{
							case 0:
								flNewYaw = flYaw + flAdd;
								break;
							case 1:
								flNewYaw = flYaw;
								break;
							case 2:
								flNewYaw = flYaw - flAdd;
								break;
							case 3:
								iShotsMissed = 0;
								break;
							}
						}
						else
						{
							iStage = 7;
							//positive
							switch (iShotsMissed)
							{
							case 0:
								flNewYaw = flYaw - flAdd;
								break;
							case 1:
								flNewYaw = flYaw;
								break;
							case 2:
								flNewYaw = flYaw + flAdd;
								break;
							case 3:
								iShotsMissed = 0;
								break;
							}
						}
				}
			}

			return flNewYaw;
		};

		if (g_Var.AntiAim.Resolve == 1)
		{
			if ((pEnemy->GetFlags() & FL_ONGROUND) && flVelocity > 0.1)
			{
				//moving logic

				if (GetAsyncKeyState(g_Var.AntiAim.Override) && pBest)
				{
					Vector vView;
					g_pEngine->GetViewAngles(vView);

					float flToMe = g_Math.CalcAngle(pBest->GetEyePosition(), pLocal->GetEyePosition()).y;

					float flAimDir = GetAimDirection(vView, pLocal, g_Math.GetHitspot(pBest, 4));

					if (flAimDir >= 0.04)
						flYaw = vToPlayer.y - 110;
					else
						if (flAimDir <= -0.04)
							flYaw = vToPlayer.y + 110;
						else
							flYaw = vToPlayer.y + 180;
				}
				else
				{
					//	flYaw = AntiEdge(vResolve.m_iMissedStand, pEnemy->LowerBodyYawTarget(), flYawDelta);

					switch (vResolve.m_iMissedMoving)
					{
					case 0:
						flYaw = pEnemy->LowerBodyYawTarget();
						break;
					case 1:
						flYaw = pEnemy->LowerBodyYawTarget() + flYawDelta;
						break;
					case 2:
						flYaw = pEnemy->LowerBodyYawTarget() - flYawDelta;
						break;
					default:
						vResolve.m_iMissedMoving = 0;
						break;
					}
				}
			}
			else
			{
				if ((pEnemy->GetFlags() & FL_ONGROUND))
				{
					if (GetAsyncKeyState(g_Var.AntiAim.Override) && pBest)
					{
						Vector vView;
						g_pEngine->GetViewAngles(vView);

						float flToMe = g_Math.CalcAngle(pBest->GetEyePosition(), pLocal->GetEyePosition()).y;

						float flAimDir = GetAimDirection(vView, pLocal, g_Math.GetHitspot(pBest, 4));

						if (flAimDir >= 0.04)
							flYaw = vToPlayer.y - 110;
						else
							if (flAimDir <= -0.04)
								flYaw = vToPlayer.y + 110;
							else
								flYaw = vToPlayer.y + 180;
					}
					else
					{
						float flBack = fmodf(fabsf(pEnemy->GetEyeAngles().y - vToPlayer.y + 180), 360.f);
						float flForward = fmodf(fabsf(pEnemy->GetEyeAngles().y - vToPlayer.y), 360.f);
						float flLeft = fmodf(fabsf(pEnemy->GetEyeAngles().y - vToPlayer.y - 90), 360.f);
						float flRight = fmodf(fabsf(pEnemy->GetEyeAngles().y - vToPlayer.y + 90), 360.f);

						//	flYaw = AntiEdge(vResolve.m_iMissedStand, g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw, flYawDelta);

						switch (vResolve.m_iMissedStand)
						{
						case 0:
							flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw;
							break;
						case 1:
							flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw + flYawDelta;
							break;
						case 2:
							flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw - flYawDelta;
							break;
						default:
							vResolve.m_iMissedStand = 0;
							break;
						}
					}
				}
				else
				{
					switch (vResolve.m_iMissedAir)
					{
					case 0:
						flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw;
						break;
					case 1:
						flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw + flYawDelta;
						break;
					case 2:
						flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw - flYawDelta;
						break;
					default:
						vResolve.m_iMissedAir = 0;
						break;
					}
				}
			}
		}
		else
			if (g_Var.AntiAim.Resolve == 2)
			{
				if (GetAsyncKeyState(g_Var.AntiAim.Override) && pBest)
				{
					Vector vView;
					g_pEngine->GetViewAngles(vView);

					float flToMe = g_Math.CalcAngle(pBest->GetEyePosition(), pLocal->GetEyePosition()).y;

					float flAimDir = GetAimDirection(vView, pLocal, g_Math.GetHitspot(pBest, 4));

					if (flAimDir >= 0.04)
						flYaw = vToPlayer.y - 110;
					else
						if (flAimDir <= -0.04)
							flYaw = vToPlayer.y + 110;
						else
							flYaw = vToPlayer.y + 180;
				}
				else
				{
					if ((pEnemy->GetFlags() & FL_ONGROUND) && flVelocity > 0.1)
					{
						switch (vResolve.m_iMissedShots)
						{
						case 0:
							flYaw = pEnemy->LowerBodyYawTarget();
							break;
						case 1:
							flYaw = pEnemy->LowerBodyYawTarget() + flYawDelta;
							break;
						case 2:
							flYaw = pEnemy->LowerBodyYawTarget() - flYawDelta;
							break;
						default:
							vResolve.m_iMissedShots = 0;
							break;
						}
					}
					else
					{
						if ((pEnemy->GetFlags() & FL_ONGROUND))
							flYaw = AntiEdge(vResolve.m_iMissedShots, g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw, flYawDelta);
						else
						{
							switch (vResolve.m_iMissedShots)
							{
							case 0:
								flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw;
								break;
							case 1:
								flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw + flYawDelta;
								break;
							case 2:
								flYaw = g_ResolveInfo[pEnemy->GetIndex()].flUpdateYaw - flYawDelta;
								break;
							default:
								vResolve.m_iMissedShots = 0;
								break;
							}
						}
					}
				}
			}

		Vector vResolved = Vector( pEnemy->GetEyeAngles().x, flYaw, 0.f );
		pEnemy->SetEyeAngles( vResolved );*/
}

void CResolver::AddShotSnapshot(CBaseEntity* entity, PlayerResolveRecord resolve_record)
{
	ShotSnapshot snapshot;

	snapshot.entity = entity;
	snapshot.time = UserCmd::flFixedCurTime;
	snapshot.resolve_record = resolve_record;
	snapshot.impact = Vector(0.f, 0.f, 0.f);
	snapshot.first_processed_time = 0.f;
	snapshot.was_shot_processed = false;
	snapshot.hitgroup_hit = -1;

	shot_snapshots.push_back(snapshot);
}

void CResolver::CleanShotSnapshot()
{
	shot_snapshots.clear();
}

void CResolver::ProcessSnapShots()
{
	if (shot_snapshots.size() <= 0)
		return;

	int r = g_Var.Color.Menu[0], g = g_Var.Color.Menu[1], b = g_Var.Color.Menu[2];

	const auto snapshot = shot_snapshots.front();

	if (fabs(UserCmd::flFixedCurTime - snapshot.time) > 1.f)
	{
		shot_snapshots.erase(shot_snapshots.begin());
		return;
	}

	if (snapshot.entity == nullptr)
		return;

	const int player_index = snapshot.entity->GetIndex();

	//time check should be delayd by ping ( event ), look if bullet hit backtracked matrix if not "spread" miss or other inaccuracy
	if (snapshot.hitgroup_hit != -1) //hit
	{
		/*		CEventLogs::sLog EventLog;

				EventLog.EventText.PushBack( "Hit " + GetResolveStage( snapshot.resolve_record.resolve_type ), Color( 255, 255, 255 ) );

				g_EventLogs.AddLog( EventLog );*/
	}
	else
		if (snapshot.first_processed_time != 0.f && fabs(UserCmd::flFixedCurTime - snapshot.first_processed_time) > 0.1f)//missed
		{
			auto CurrentRestore = sPreBacktrackRecord(snapshot.entity);

			// safety check
			if (g_History.vUsedRecord == nullptr)
				return;

			g_History.Restore(snapshot.entity, g_History.vUsedRecord);

			//do checks

			CEngineTraceClient::Ray_t ray;
			CEngineTraceClient::trace_t trace;

			ray.Init(snapshot.position, snapshot.impact);

			g_pEngineTrace->ClipRayToEntity(ray, 0x4600400B, snapshot.entity, &trace);

			if (trace.m_pEnt && trace.m_pEnt == snapshot.entity)
			{
				//remove used record to avoid shootin it again
				g_History.vPlayerRecord[player_index].erase(std::remove_if(g_History.vPlayerRecord[player_index].begin(), g_History.vPlayerRecord[player_index].end(),
					[](std::shared_ptr<sHistoryRecords> Record) -> bool
				{
					return Record->m_flSimulationTime > g_History.vUsedRecord->m_flSimulationTime;
				}), g_History.vPlayerRecord[player_index].end());

				player_resolve_records[player_index].m_iMissedShots++;

				if ((snapshot.entity->GetFlags() & FL_ONGROUND) && snapshot.entity->GetVelocity().Length() > 0.1)
					player_resolve_records[player_index].m_iMissedMoving++;
				else
				{
					if ((snapshot.entity->GetFlags() & FL_ONGROUND))
						player_resolve_records[player_index].m_iMissedStand++;
					else
						player_resolve_records[player_index].m_iMissedAir++;
				}
			}
			else
			{
				if (g_Var.Misc.Untrusted)
				{
					CEventLogs::sLog EventLog;

					EventLog.EventText.PushBack("Inaccuracy Miss", Color(255, 255, 255));

					g_EventLogs.AddLog(EventLog);

					g_pICvar->ConsoleColorPrintf(Color(r, g, b), "[ KIDUAHOOK ] ");
					g_pICvar->ConsoleColorPrintf(Color(255, 255, 255), "Inaccuracy Miss \n");
				}
			}

			CurrentRestore.Restore();
		}
		else
			return;

	shot_snapshots.erase(shot_snapshots.begin());
}

void CResolver::UpdateResolveRecord(CBaseEntity* pEnemy)
{
	const PlayerResolveRecord sPrevRecord = player_resolve_records[pEnemy->GetIndex()];
	PlayerResolveRecord& sRecord = player_resolve_records[pEnemy->GetIndex()];

	sRecord.m_vecResolveAngles = sRecord.m_vecAngles;
	sRecord.m_flSpeed = pEnemy->GetVelocity().Length();
	sRecord.m_vecOrigin = pEnemy->GetOrigin();
	sRecord.m_bDormant = pEnemy->IsDormant();

	for (int i = 0; i < 13; i++)
		sRecord.m_cLayer[i] = pEnemy->GetAnimationOverlay(i);

	const bool bGroundMove = sRecord.m_flSpeed > 0.1f && pEnemy->GetFlags() & FL_ONGROUND;

	//weight 6 = 0.0 when not moving
	//playbackrate 6 = 0.0 when not moving
	if ((sRecord.m_cLayer[12].m_flWeight == 0.f /*|| sRecord.m_cLayer[ 6 ].m_flWeight == 0.f && sRecord.m_cLayer[ 6 ].m_flPlaybackRate == 0.f*/) && bGroundMove && sRecord.m_flSpeed < 100.f)
		sRecord.m_bFakeWalk = true;
	else
		sRecord.m_bFakeWalk = false;

	if (!sRecord.m_bFakeWalk)
	{
		if (bGroundMove && sRecord.m_flSpeed > 35.f)
		{
			sRecord.m_bValidMoveYaw = true;
			sRecord.m_flMoveYaw = pEnemy->LowerBodyYawTarget();
		}
		else
		{
			if (sRecord.m_bValidMoveYaw)
				sRecord.m_flDelta = fabsf(sRecord.m_flMoveYaw - pEnemy->LowerBodyYawTarget());
		}
	}

	if (!sRecord.m_bDormant && sPrevRecord.m_bDormant)
	{
		if ((sRecord.m_vecOrigin - sPrevRecord.m_vecOrigin).Length2D() > 16.f)
			sRecord.m_bValidMoveYaw = false;
	}

	if (g_Math.NormalizePitch(sRecord.m_vecAngles.x) > 35.f)
		sRecord.m_flPitchDown = g_pGlobalVars->curtime;
}

void CResolver::AntiFreestand(CBaseEntity* pEnemy, float& y, float flLeftDamage, float flRightDamage, float flRightFraction, float flLeftFraction, float flToMe, int& iShotsMissed)
{
	if (flLeftDamage >= 15 && flRightDamage >= 15)
	{
		//too much damage to both sides bruteforce!
		switch (iShotsMissed)
		{
		case 0:
			y = flToMe + 180;
			break;
		case 1:
			y = flToMe + 110;
			break;
		case 2:
			y = flToMe - 110;
			break;
		case 3:
			y = flToMe;
			break;
		case 4:
			iShotsMissed = 0;
			break;
		}
	}
	else
	{
		if (flLeftDamage <= 0 && flRightDamage <= 0)
		{
			//no damage at all use edge
			if (flRightFraction == flLeftFraction)
			{
				switch (iShotsMissed)
				{
				case 0:
					y = flToMe + 180;
					break;
				case 1:
					y = flToMe + 110;
					break;
				case 2:
					y = flToMe - 110;
					break;
				case 3:
					y = flToMe;
					break;
				case 4:
					iShotsMissed = 0;
					break;
				}
			}
			else
				if (flRightFraction < flLeftFraction)
				{
					//negative
					switch (iShotsMissed)
					{
					case 0:
						y = flToMe - 110.f;
						break;
					case 1:
						y = flToMe + 180;
						break;
					case 2:
						y = flToMe + 110.f;
						break;
					case 3:
						y = flToMe;
						break;
					case 4:
						iShotsMissed = 0;
						break;
					}
				}
				else
				{
					switch (iShotsMissed)
					{
					case 0:
						y = flToMe + 110.f;
						break;
					case 1:
						y = flToMe + 180;
						break;
					case 2:
						y = flToMe - 110.f;
						break;
					case 3:
						y = flToMe;
						break;
					case 4:
						iShotsMissed = 0;
						break;
					}
				}
		}
		else
		{
			//got some damage turn damage based
			if (flLeftDamage == flRightDamage)
			{
				switch (iShotsMissed)
				{
				case 0:
					y = flToMe + 110.f;
					break;
				case 1:
					y = flToMe - 110.f;
					break;
				case 2:
					y = flToMe + 180.f;
					break;
				case 3:
					y = flToMe;
					break;
				case 4:
					iShotsMissed = 0;
					break;
				}
			}
			else
				if (flLeftDamage > flRightDamage)
				{
					//negative
					switch (iShotsMissed)
					{
					case 0:
						y = flToMe - 110.f;
						break;
					case 1:
						y = flToMe + 180;
						break;
					case 2:
						y = flToMe + 110.f;
						break;
					case 3:
						y = flToMe;
						break;
					case 4:
						iShotsMissed = 0;
						break;
					}
				}
				else
				{
					switch (iShotsMissed)
					{
					case 0:
						y = flToMe + 110.f;
						break;
					case 1:
						y = flToMe + 180;
						break;
					case 2:
						y = flToMe - 110.f;
						break;
					case 3:
						y = flToMe;
						break;
					case 4:
						iShotsMissed = 0;
						break;
					}
				}
		}
	}
}

bool IsFakeWalking(CBaseEntity* pEntity)
{
	/*
	// determine if it's possible that they are fakewalking.
	if (choked_ticks > 1 && player.entity()->is_grounded() && anim_speed > 0.1f && simulation_delta < 1.f) {
	// either they're fakewalking or their LBY didn't change.
	if (anim_speed < 100.f && player.m_server_layers[6].m_flWeight == 0.f) { // sizeof(animlayer) * 6 + 32 = 6th index weight
	fakewalk = true;
	}
	}
	*/

	//esoterik told me about this method *jokes*
/*if( *( float * ) &velocity_length > 0.1 )
	{
		if( *( _DWORD * ) ( ( m_AnimOverlay ^ zero ) + player + 12 ) <= 6
			|| ( v64 = *( C_AnimationLayer ** ) ( ( m_AnimOverlay ^ zero ) + player ), v64[ 5 ].m_flWeight != 0.0 )
			|| v64[ 4 ].m_flWeight != 0.0
			|| v64[ 6 ].m_flPlaybackRate != 0.0 )
		{
			moving = 1;
			lagcomp_player->gap00[ 33 ] = 0;
			lagcomp_player->gap00[ 39 ] = 0;
		}
		else
		{
			moving = 0;
		}
	}
	else
	{
		moving = 0;
	}*/
	return true;
}

void CResolver::Init(CBaseEntity * pLocal)
{
	static float LowerBody[33];
	static float oldLowerBody[33];
	static float lastMoveYaw[33];
	static float lastUpdateTime[33];
	float flMoveDelta[33];
	float flBestValue = 9999.0f;
	CBaseEntity* pBest = nullptr;

	for (int i = 1; i <= 32; i++)
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity(i);

		if (!pEnemy->PlayerCheckWithoutDormant())
			continue;

		if (pEnemy->GetTeamNumber() == pLocal->GetTeamNumber())
			continue;

		if (pEnemy == pLocal)
			continue;

		player_resolve_records[pEnemy->GetIndex()].m_vecAngles.x = pEnemy->GetEyeAngles().x;
		player_resolve_records[pEnemy->GetIndex()].m_vecAngles.y = pEnemy->GetEyeAngles().y;

		UpdateResolveRecord(pEnemy);

		PlayerResolveRecord& resolve_record = player_resolve_records[pEnemy->GetIndex()];

		if (pEnemy->IsDormant())
		{
			resolve_record.m_iMissedStand = 0;
			continue;
		}

		Vector vView;
		g_pEngine->GetViewAngles(vView);

		float flFov = g_Math.GetFoV(vView, pLocal->GetEyePosition(), pEnemy->GetEyePosition());

		if (flFov < flBestValue)
		{
			flBestValue = flFov;
			pBest = pEnemy;
		}

		float *x = (float*)((DWORD)pEnemy + Offsets::Player::m_angEyeAngles);
		float *y = (float*)((DWORD)pEnemy + Offsets::Player::m_angEyeAnglesY);

		Vector vAngle = Vector(0, 0, 0);
		g_Math.CalcAngle(pEnemy->GetEyePosition(), pLocal->GetEyePosition(), vAngle);

		float flToMe = vAngle.y;

		//what we want compare last moving lby with the standing ( delta ) apply the delta on lby ???_???  IF  we can make sure that they are breaking lby in some sort
		//no info = use freestanding otherwise whe should go for delta checks

		if (fabs(g_pGlobalVars->curtime - resolve_record.m_flPitchDown) < 0.5f)
			resolve_record.m_vecResolveAngles.x = 89.f;
		else
			if (resolve_record.m_vecResolveAngles.x == 89.f && resolve_record.m_vecAngles.x != 89.f)
				resolve_record.m_vecResolveAngles.x = resolve_record.m_vecAngles.x;

		resolve_record.m_vecResolveAngles.x = g_Var.Misc.Untrusted ? g_Math.NormalizePitch(resolve_record.m_vecResolveAngles.x) : 89.f;

		*x = resolve_record.m_vecResolveAngles.x;

		const auto freestanding_record = player_resolve_records[i].m_sAntiEdge;

		if ((*x <= 89.f && *x >= 45.f) || *x == 0.f)
		{
			if ((pEnemy->GetFlags() & FL_ONGROUND) && pEnemy->GetVelocity().Length() > 0.1)
			{
				if (resolve_record.m_bFakeWalk)
				{
					if (resolve_record.m_bValidMoveYaw)
					{
						switch (resolve_record.m_iMissedStand)
						{
						case 0:
							*y = resolve_record.m_flMoveYaw;
							break;
						case 1:
							resolve_record.m_iMissedStand = 0;
							resolve_record.m_bValidMoveYaw = false;
							break;
						}
					}
					else
						AntiFreestand(pEnemy, *y, freestanding_record.left_damage, freestanding_record.right_damage, freestanding_record.right_fraction, freestanding_record.left_fraction, flToMe, resolve_record.m_iMissedStand);

					g_ResolveInfo[pEnemy->GetIndex()].szStage = "FAKEWALK";
				}
				else
				{
					*y = pEnemy->LowerBodyYawTarget();
					g_ResolveInfo[pEnemy->GetIndex()].szStage = "MOVING";
				}
			}
			else
			{
				if (LowerBody[i] != pEnemy->LowerBodyYawTarget())
				{
					LowerBody[i] = pEnemy->LowerBodyYawTarget();
					*y = pEnemy->LowerBodyYawTarget();
					g_ResolveInfo[pEnemy->GetIndex()].bLBYUpdate = true;

					if ((pEnemy->GetFlags() & FL_ONGROUND))
						lastUpdateTime[i] = pEnemy->GetSimulationTime();

					g_ResolveInfo[pEnemy->GetIndex()].szStage = "UPDATE";
				}
				else
				{
					g_ResolveInfo[pEnemy->GetIndex()].bLBYUpdate = false;

					if ((pEnemy->GetFlags() & FL_ONGROUND))
					{
						if (pEnemy->GetSimulationTime() - lastUpdateTime[i] > 1.1)
						{
							lastUpdateTime[i] = pEnemy->GetSimulationTime();
							*y = pEnemy->LowerBodyYawTarget();
							g_ResolveInfo[pEnemy->GetIndex()].bLBYUpdate = true;
							g_ResolveInfo[pEnemy->GetIndex()].szStage = "PREDICT";
						}
						else
						{
							if (resolve_record.m_bValidMoveYaw)
							{
								switch (resolve_record.m_iMissedStand)
								{
								case 0:
									*y = resolve_record.m_flMoveYaw;
									break;
								case 1:
									resolve_record.m_iMissedStand = 0;
									resolve_record.m_bValidMoveYaw = false;
									break;
								}
								g_ResolveInfo[pEnemy->GetIndex()].szStage = "DELTA";
							}
							else
							{
								AntiFreestand(pEnemy, *y, freestanding_record.left_damage, freestanding_record.right_damage, freestanding_record.right_fraction, freestanding_record.left_fraction, flToMe, resolve_record.m_iMissedStand);
								g_ResolveInfo[pEnemy->GetIndex()].szStage = "AVOID";
							}
						}
					}
					else
					{
						g_ResolveInfo[pEnemy->GetIndex()].szStage = "AIR";

						switch (resolve_record.m_iMissedStand)
						{
						case 0:
							*y = flToMe + 180;
							break;
						case 1:
							*y = flToMe - 145;
							break;
						case 2:
							*y = flToMe + 145;
							break;
						case 3:
							*y = flToMe;
							break;
						default:
							*y = flToMe + 180 + g_Math.RandomFloat(-35, 35);
							break;
						}
					}
				}
			}
		}
		else
		{
			g_ResolveInfo[pEnemy->GetIndex()].szStage = "UNRESOLVED";

			if (pEnemy->GetVelocity().Length() > 0.1)
				*y = pEnemy->LowerBodyYawTarget();
			else
				*y = pEnemy->GetEyeAngles().y;

			g_ResolveInfo[pEnemy->GetIndex()].bLBYUpdate = true;
		}

		//		g_ResolveInfo[ pEnemy->GetIndex() ].flResolvedYaw = *y;
		g_ResolveInfo[pEnemy->GetIndex()].flResolvedPitch = *x;
	}

	if (GetAsyncKeyState(g_Var.AntiAim.Override) && pBest)
	{
		Vector vView;
		g_pEngine->GetViewAngles(vView);

		float flToMe = g_Math.CalcAngle(pBest->GetEyePosition(), pLocal->GetEyePosition()).y;

		float flAimDir = GetAimDirection(vView, pLocal, g_Math.GetHitspot(pBest, 4));

		float *y = (float*)((DWORD)pBest + Offsets::Player::m_angEyeAnglesY);

		if (g_ResolveInfo[pBest->GetIndex()].bLBYUpdate)
			*y = pBest->LowerBodyYawTarget();
		else
		{
			if (flAimDir >= 0.04)
				*y = flToMe - 90;
			else
				if (flAimDir <= -0.04)
					*y = flToMe + 90;
				else
					*y = flToMe + 180;
		}
		g_ResolveInfo[pBest->GetIndex()].szStage = "OVERRIDE";
	}
}