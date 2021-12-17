#include "antiaim.h"

CAntiAim g_AntiAim;

bool bNadeThrow( CBaseCombatWeapon * pWeapon, CBaseEntity * pLocal, CUserCmd * pCmd )
{
	if( pWeapon->IsGrenade() )
	{
		if( !pWeapon->bPinPulled() || pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2 )
		{
			float fThrowTime = pWeapon->GetThrowTime();

			if( ( fThrowTime > 0 ) )
				return true;
		}

		return false;
	}
	else
		return false;
}

CBaseEntity* GetBestPlayer( CBaseEntity * pLocal, int iBased )
{
	CBaseEntity* pBestEnemy = nullptr;

	float flBestValue = 9999.0f;

	Vector vView;
	g_pEngine->GetViewAngles( vView );

	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy->PlayerCheck() )
			continue;

		if( pEnemy->GetTeamNumber() == pLocal->GetTeamNumber() )
			continue;

		if( pEnemy == pLocal )
			continue;

		float flDistance = fabsf( pEnemy->GetOrigin().Length() - pLocal->GetOrigin().Length() );
		float flFov = g_Math.GetFoV( vView, pLocal->GetEyePosition(), pEnemy->GetEyePosition() );

		if( iBased == 1 )
		{
			if( flDistance < flBestValue )
			{
				flBestValue = flDistance;
				pBestEnemy = pEnemy;
			}
		}
		else
			if( iBased == 2 )
			{
				if( flFov < flBestValue )
				{
					flBestValue = flFov;
					pBestEnemy = pEnemy;
				}
			}
			else
			{
				if (flFov < flBestValue)
				{
					flBestValue = flFov;
					pBestEnemy = pEnemy;
				}
			}
	}
	return pBestEnemy;
}

/*
float c_anims::predict_abs_yaw( float abs_yaw, float viewangles, float lby, float update_delta, float stop_to_running_fraction,const vec3& velocity, float min_delta, float max_delta )
{
	viewangles = math::float_normalize( viewangles, -180.f, 180.f );
	abs_yaw = math::float_normalize( abs_yaw, -180.f, 180.f );

	float new_abs_yaw;

	// offset abs yaw
	{
		// normalized delta
		float yaw_delta = math::float_normalize( viewangles - abs_yaw, -180.f, 180.f );

		// clamp the delta to min/max
		yaw_delta = std::clamp( yaw_delta, min_delta, max_delta );

		// normalized server abs yaw start
		new_abs_yaw = math::float_normalize( viewangles - yaw_delta, -180.f, 180.f );
	}

	// approach angle
	if ( velocity.length2d( ) > 0.1f || abs( velocity.z ) > 100.f )
	{
		new_abs_yaw = math::approach_angle_rebuild( viewangles, new_abs_yaw, ( ( stop_to_running_fraction * 20.f ) + 30.f ) * update_delta );
	}
	else
	{
		new_abs_yaw = math::approach_angle_rebuild( lby, new_abs_yaw, update_delta * 100.f );
	}

	return new_abs_yaw;
}
*/

void CAntiAim::Run( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd, bool & bSendPacket )
{
	m_bInAntiAim = false;

	g_Misc.FakeLag(pLocal, pWeapon, bSendPacket, pCmd);

	if( !g_Var.AntiAim.Enable )
		return;

	if( pLocal->GetMoveType() == 8 || pLocal->GetMoveType() == 9 || UserCmd::iOriginalFlags & FL_FROZEN || pCmd->buttons & IN_USE || bNadeThrow( pWeapon, pLocal, pCmd ) )
	{
		UserCmd::vAntiAimReal.y = pCmd->viewangles.y;
		return;
	}

	//instead of that we could go for accuracy walk on key ? the 33 div meme 
	if (GetAsyncKeyState(g_Var.AntiAim.Fakewalk))
	{
		m_bInSlowWalk = true;
		CWeaponData* pWeaponData = pWeapon->GetWeaponData();

		if (pWeaponData)
			g_Misc.ClampWalk(pCmd, pWeaponData->movement_speed_zoom / 3.f);
	}
	else
		m_bInSlowWalk = false;

	if( ( g_Rage.bInRage || g_Rage.bInZeus || g_Misc.bInKnife ) && g_Misc.CanShoot( pLocal, pWeapon ) 
		|| GetAsyncKeyState(1) && !pWeapon->IsGrenade() && pWeapon->GetWeaponID() != WEAPON_C4 && !g_Gui.bCursorActive)
	{
	//	bSendPacket = false;

		return;
	}

	static float flOldLby;

	if( flOldLby != pLocal->LowerBodyYawTarget() )
	{
		if( m_bUpdated )
			m_bUpdated = false;
		else
			m_bUpdated = true;

		flOldLby = pLocal->LowerBodyYawTarget();
	}

	m_bInAntiAim = true;

	Pitch( pLocal, pCmd );

	//so since csgo fucked up stuff we can still fake our yaw
	//on send tick we go - or + abs yaw max delta ( 58° )

	static bool bInMove, bJustStopped;

	if (pLocal->GetVelocity().Length() > 0.1f)
		bInMove = true;
	else
		bInMove = false;

	if (bInMove)
		bJustStopped = true;

	if (g_Var.AntiAim.Break >= 3)
	{
		if (!g_pClientState->GetChokedCmds())
		{
			Yaw(pLocal, pCmd);

			UserCmd::vAntiAimReal.y = pCmd->viewangles.y;
		}
		else
			{
			if (g_Var.AntiAim.Break == 4)
			{
				static bool bFlip;
				bFlip = !bFlip;

				static float flAdd;

				bFlip ? flAdd = g_Misc.MaxAbsYawDelta(pLocal) : flAdd = -g_Misc.MaxAbsYawDelta(pLocal);

				if (pLocal->GetVelocity().Length() > 90.f)
					pCmd->viewangles.y = m_flReal - flAdd;
				else
					pCmd->viewangles.y = m_flReal - (flAdd + 180.0);
			}
			else
			{
				{
					if (m_bLeft)
						pCmd->viewangles.y = m_flReal + clamp(g_Var.AntiAim.BreakRad, 0.f, g_Misc.MaxAbsYawDelta(pLocal));
					else
						pCmd->viewangles.y = m_flReal - clamp(g_Var.AntiAim.BreakRad, 0.f, g_Misc.MaxAbsYawDelta(pLocal));
				}
			}

				m_flFake = pCmd->viewangles.y;
			}
	}
	else
	{
		if (m_bLbyUpdate && g_Var.AntiAim.Break == 2 && !GetKeyState(g_Var.AntiAim.BreakToggle))
		{
			if (m_bLeft)
				pCmd->viewangles.y = m_flReal + clamp(g_Var.AntiAim.BreakRad, m_bInFakeWalk ? 131.f : 99.3f, 180.f);
			else
				pCmd->viewangles.y = m_flReal - clamp(g_Var.AntiAim.BreakRad, m_bInFakeWalk ? 131.f : 99.3f, 180.f);

			m_flLastLowerBodyAngle = pCmd->viewangles.y;
			m_bLbyUpdate = false;
		}
		else
			Yaw(pLocal, pCmd);

		UserCmd::vAntiAimReal.y = pCmd->viewangles.y;
	}

	if (g_Var.AntiAim.Break >= 3 && UserCmd::iOriginalFlags & FL_ONGROUND && !pLocal->GetVelocity().Length())
	{
		if (!(g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_A) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_D) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_S) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_Q) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_Z) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_W)))
		{
			m_bGroundAdjust = true;
			static bool bFlip;
			bFlip = !bFlip;

			// we want lby to update constantly to misscalculate our abs yaw to desync

			if (pLocal->DuckAmount() > 0.f)
				pCmd->sidemove = bFlip ? 3.25 : -3.25;// 0.01 to high ?
			else
				pCmd->sidemove = bFlip ? 1.1 : -1.1;
		}
		else
			m_bGroundAdjust = false;
	}
}

void CAntiAim::Pitch( CBaseEntity * pLocal, CUserCmd * pCmd )
{
	switch( g_Var.AntiAim.Pitch )
	{
	case 1:
		pCmd->viewangles.x = 89.f;
		break;
	case 2:
		pCmd->viewangles.x = g_Var.Misc.Untrusted ? -89.f : -540.f;
		break;
	case 3:
		pCmd->viewangles.x = g_Var.Misc.Untrusted ? 0.f : -540.f;
		break;
	case 4:
		pCmd->viewangles.x = rand() % 2 == 1 ? 89.f : 60.f;
		break;
	}

	UserCmd::vAntiAimReal.x = pCmd->viewangles.x;
}

void CAntiAim::FakeYaw( CBaseEntity * pLocal, CUserCmd * pCmd )
{
	pCmd->viewangles.y = fmod( g_pGlobalVars->curtime / 0.9f * 120.0f, 360.0f );
}

float CAntiAim::LowerBodyYaw( CBaseEntity * pLocal, float flReal, float flLowerBody )
{
	flLowerBody = fabs( g_Math.NormalizeYaw( flLowerBody - pLocal->LowerBodyYawTarget() ) ) < 35.f ? pLocal->LowerBodyYawTarget() : flLowerBody;

	if( m_bLbyUpdate )
	{
		const float flLbyDelta = g_Math.NormalizeYaw( flLowerBody - flReal );
		static bool bSwitch = false;

		bSwitch = !bSwitch;

		if( fabs( flLbyDelta ) < 100.f )
			flLowerBody = flReal + ( bSwitch ? flLbyDelta : -flLbyDelta );

		m_bLbyUpdate = false;
		m_flLastLowerBodyAngle = flLowerBody;

		return flLowerBody;
	}

	if( fabs( g_Math.NormalizeYaw( flReal - flLowerBody ) ) > 130.f && fabs( g_Math.NormalizeYaw( flLowerBody - pLocal->LowerBodyYawTarget() ) ) < 35.f )
	{
		if( fabs( g_pGlobalVars->curtime - m_flNextUpdate ) < g_pGlobalVars->interval_per_tick )
			return flLowerBody - 115.f;

		if( fabs( g_pGlobalVars->curtime - m_flNextUpdate ) > 1.1 - ( g_pGlobalVars->interval_per_tick * 5 ) )
			return flLowerBody + 115.f;
	}

	return flReal;
}

float RandomJitter(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

float StaticJitter(float a, float b)
{
	return rand() % 2 ? a : b;
}

float JitterRange(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;

	if (g_Var.AntiAim.Break == 3)
	{
		if( b > 89.f)
			return rand() % 2 ? a + a : b - b;
		else
			return rand() % 2 ? a : b;
	}
	else
		return a + r;
}

void CAntiAim::Yaw( CBaseEntity * pLocal, CUserCmd * pCmd )
{
	CBaseEntity* pEnemy = nullptr;

	m_bLeft, m_bRight, m_bBack, m_bFront;

	if( g_Misc.IsKeyPressed( g_Var.AntiAim.LeftToggle ) )
	{
		m_bLeft = !m_bLeft;
		m_bRight = false, m_bBack = false, m_bFront = false;
	}

	if( g_Misc.IsKeyPressed( g_Var.AntiAim.RightToggle ) )
	{
		m_bRight = !m_bRight;
		m_bLeft = false, m_bBack = false, m_bFront = false;
	}

	if( g_Misc.IsKeyPressed( g_Var.AntiAim.BackToggle ) )
	{
		m_bBack = !m_bBack;
		m_bLeft = false, m_bRight = false, m_bFront = false;
	}

	if( g_Misc.IsKeyPressed( g_Var.AntiAim.FrontToggle ) )
	{
		m_bFront = !m_bFront;
		m_bLeft = false, m_bRight = false, m_bBack = false;
	}

	m_bLock = GetKeyState( g_Var.AntiAim.LockToggle );
	static float flLocked;

	if( m_bLock != GetKeyState( g_Var.AntiAim.LockToggle ) )
	{
		m_bLock = GetKeyState( g_Var.AntiAim.LockToggle );
		flLocked = pCmd->viewangles.y;
	}

	static float flYaw;

	if( fabsf( flYaw - pCmd->viewangles.y ) > g_Var.AntiAim.Tolerance )
		flYaw = pCmd->viewangles.y;

	float flSideTolerance = g_Var.AntiAim.SideWaysTolerance;

	if( UserCmd::iOriginalFlags & FL_ONGROUND )
	{
		if( pLocal->GetVelocity().Length() > 0.1 && !m_bInSlowWalk )
		{
			float flAdd = g_Var.AntiAim.MoveValue;
			float flStaticAdd = g_Var.AntiAim.MoveValue - 90;
			float flSpeed = g_Var.AntiAim.MoveSpeed * 30;
			pEnemy = GetBestPlayer( pLocal, g_Var.AntiAim.MoveEnemyBased );
			float flEdge = CanEdge( pLocal, pEnemy, pCmd );

			switch( g_Var.AntiAim.Move )
			{
			case 1:
				if( m_bLeft || m_bRight || m_bBack || m_bFront )
				{
					if( m_bLeft )
					{
						if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
						{
							Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
							g_Math.NormalizeVector( vToEnemy );
							g_Math.ClampAngles( vToEnemy );

							if( g_Var.AntiAim.MoveStyle == 1 )
								pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : vToEnemy.y + flSideTolerance + JitterRange( -flAdd, flAdd );
							else
								if( g_Var.AntiAim.MoveStyle == 2 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + flStaticAdd : vToEnemy.y + flSideTolerance;
						}
						else
						{
							if( g_Var.AntiAim.MoveStyle == 1 )
								pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + flSideTolerance + JitterRange( -flAdd, flAdd );
							else
								if( g_Var.AntiAim.MoveStyle == 2 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + flStaticAdd : pCmd->viewangles.y + flSideTolerance;
						}
					}
					else
						if( m_bRight )
						{
							if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
							{
								Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
								g_Math.NormalizeVector( vToEnemy );
								g_Math.ClampAngles( vToEnemy );

								if( g_Var.AntiAim.MoveStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : vToEnemy.y - flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.MoveStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + flStaticAdd : vToEnemy.y - flSideTolerance;
							}
							else
							{
								if( g_Var.AntiAim.MoveStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y - flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.MoveStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + flStaticAdd : pCmd->viewangles.y - flSideTolerance;
							}
						}
						else
							if( m_bBack )
							{
								if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
								{
									Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
									g_Math.NormalizeVector( vToEnemy );
									g_Math.ClampAngles( vToEnemy );

									if( g_Var.AntiAim.MoveStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : vToEnemy.y + 180 + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.MoveStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : vToEnemy.y + 180;
								}
								else
								{
									if( g_Var.AntiAim.MoveStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + 180 + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.MoveStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : pCmd->viewangles.y + 180;
								}
							}
							else
								if( m_bFront )
								{
									if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
									{
										Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
										g_Math.NormalizeVector( vToEnemy );
										g_Math.ClampAngles( vToEnemy );

										if( g_Var.AntiAim.MoveStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : vToEnemy.y + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.MoveStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + flStaticAdd : vToEnemy.y;
									}
									else
									{
										if( g_Var.AntiAim.MoveStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.MoveStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + flStaticAdd : pCmd->viewangles.y;
									}
								}
				}
				else
				{
					if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
					{
						Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
						g_Math.NormalizeVector( vToEnemy );
						g_Math.ClampAngles( vToEnemy );

						if( g_Var.AntiAim.MoveStyle == 1 )
							pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : vToEnemy.y + 180 + JitterRange( -flAdd, flAdd );
						else
							if( g_Var.AntiAim.MoveStyle == 2 )
								pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
							else
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : vToEnemy.y + 180 + flStaticAdd;
					}
					else
					{
						if( g_Var.AntiAim.MoveStyle == 1 )
							pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + 180 + JitterRange( -flAdd, flAdd );
						else
							if( g_Var.AntiAim.MoveStyle == 2 )
								pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
							else
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : pCmd->viewangles.y + 180 + flStaticAdd;
					}
				}
				break;
			case 2:
				if( m_bLeft || m_bRight || m_bBack || m_bFront )
				{
					if( m_bLeft )
					{
						if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
						{
							Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
							g_Math.NormalizeVector( vToEnemy );
							g_Math.ClampAngles( vToEnemy );

							if( g_Var.AntiAim.MoveStyle == 1 )
								pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : vToEnemy.y + flSideTolerance + JitterRange( -flAdd, flAdd );
							else
								if( g_Var.AntiAim.MoveStyle == 2 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + flStaticAdd : vToEnemy.y + flSideTolerance;
						}
						else
						{
							if( g_Var.AntiAim.MoveStyle == 1 )
								pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + flSideTolerance + JitterRange( -flAdd, flAdd );
							else
								if( g_Var.AntiAim.MoveStyle == 2 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + flStaticAdd : pCmd->viewangles.y + flSideTolerance;
						}
					}
					else
						if( m_bRight )
						{
							if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
							{
								Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
								g_Math.NormalizeVector( vToEnemy );
								g_Math.ClampAngles( vToEnemy );

								if( g_Var.AntiAim.MoveStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : vToEnemy.y - flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.MoveStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + flStaticAdd : vToEnemy.y - flSideTolerance;
							}
							else
							{
								if( g_Var.AntiAim.MoveStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y - flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.MoveStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + flStaticAdd : pCmd->viewangles.y - flSideTolerance;
							}
						}
						else
							if( m_bBack )
							{
								if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
								{
									Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
									g_Math.NormalizeVector( vToEnemy );
									g_Math.ClampAngles( vToEnemy );

									if( g_Var.AntiAim.MoveStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : vToEnemy.y + 180 + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.MoveStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : vToEnemy.y + 180;
								}
								else
								{
									if( g_Var.AntiAim.MoveStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + 180 + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.MoveStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : pCmd->viewangles.y + 180;
								}
							}
							else
								if( m_bFront )
								{
									if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
									{
										Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
										g_Math.NormalizeVector( vToEnemy );
										g_Math.ClampAngles( vToEnemy );

										if( g_Var.AntiAim.MoveStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : vToEnemy.y + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.MoveStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + flStaticAdd : vToEnemy.y;
									}
									else
									{
										if( g_Var.AntiAim.MoveStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.MoveStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + flStaticAdd : pCmd->viewangles.y;
									}
								}
				}
				else
				{
					if( pEnemy && g_Var.AntiAim.MoveEnemyBased >= 1 )
					{
						Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
						g_Math.NormalizeVector( vToEnemy );
						g_Math.ClampAngles( vToEnemy );

						if( g_Var.AntiAim.MoveStyle == 1 )
						{
							if( flEdge != 0.f )
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEdge + JitterRange( -flAdd, flAdd );
							else
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : vToEnemy.y + 180 + JitterRange( -flAdd, flAdd );
						}
						else
							if( g_Var.AntiAim.MoveStyle == 2 )
							{
								if( flEdge != 0.f )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );

							}
							else
							{
								if( flEdge != 0.f )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flEdge + flStaticAdd;
								else								
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : vToEnemy.y + 180 + flStaticAdd;
							}
					}
					else
					{
						if( g_Var.AntiAim.MoveStyle == 1 )
						{
							if( flEdge != 0.f )
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEdge + JitterRange( -flAdd, flAdd );
							else
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + 180 + JitterRange( -flAdd, flAdd );
						}
						else
							if( g_Var.AntiAim.MoveStyle == 2 )
							{
								if( flEdge != 0.f )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
							}
							else
							{
								if( flEdge != 0.f )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flEdge + flStaticAdd;
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : pCmd->viewangles.y + 180 + flStaticAdd;
							}
					}
				}
				break;
			}
		}
		else
		{
			float flAdd = g_Var.AntiAim.StandValue;
			float flStaticAdd = g_Var.AntiAim.StandValue - 90;
			float flSpeed = g_Var.AntiAim.StandSpeed * 30;
			pEnemy = GetBestPlayer( pLocal, g_Var.AntiAim.StandEnemyBased );
			float flEdge = CanEdge( pLocal, pEnemy, pCmd );

			switch( g_Var.AntiAim.Stand )
			{
			case 1:
				if( g_Var.AntiAim.Break == 1 )
				{
					if( m_bLeft || m_bRight || m_bBack || m_bFront )
					{
						if( m_bLeft )
						{
							if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
							{
								Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
								g_Math.NormalizeVector( vToEnemy );
								g_Math.ClampAngles( vToEnemy );

								static float flEnemy;

								if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
									flEnemy = vToEnemy.y;

								if( g_Var.AntiAim.StandStyle == 1 )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
								}
								else
									if( g_Var.AntiAim.StandStyle == 2 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
									}
							}
							else
							{
								if( g_Var.AntiAim.StandStyle == 1 )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

								}
								else
									if( g_Var.AntiAim.StandStyle == 2 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
									}
							}
						}
						else
							if( m_bRight )
							{
								if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
								{
									Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
									g_Math.NormalizeVector( vToEnemy );
									g_Math.ClampAngles( vToEnemy );

									static float flEnemy;

									if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
										flEnemy = vToEnemy.y;

									if( g_Var.AntiAim.StandStyle == 1 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									}
									else
										if( g_Var.AntiAim.StandStyle == 2 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										}
										else
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
										}
								}
								else
								{
									if( g_Var.AntiAim.StandStyle == 1 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

									}
									else
										if( g_Var.AntiAim.StandStyle == 2 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - ( 80 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - ( 80 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										}
										else
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
										}
								}
							}
							else
								if( m_bBack )
								{
									if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
									{
										Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
										g_Math.NormalizeVector( vToEnemy );
										g_Math.ClampAngles( vToEnemy );

										static float flEnemy;

										if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
											flEnemy = vToEnemy.y;

										if( g_Var.AntiAim.StandStyle == 1 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
										}
										else
											if( g_Var.AntiAim.StandStyle == 2 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											}
											else
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) );
											}
									}
									else
									{
										if( g_Var.AntiAim.StandStyle == 1 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

										}
										else
											if( g_Var.AntiAim.StandStyle == 2 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											}
											else
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) );
											}
									}
								}
								else
									if( m_bFront )
									{
										if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
										{
											Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
											g_Math.NormalizeVector( vToEnemy );
											g_Math.ClampAngles( vToEnemy );

											static float flEnemy;

											if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
												flEnemy = vToEnemy.y;

											if( g_Var.AntiAim.StandStyle == 1 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
											}
											else
												if( g_Var.AntiAim.StandStyle == 2 )
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												}
												else
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) );
												}
										}
										else
										{
											if( g_Var.AntiAim.StandStyle == 1 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

											}
											else
												if( g_Var.AntiAim.StandStyle == 2 )
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												}
												else
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) );
												}
										}
									}

					}
					else
					{
						if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
						{
							Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
							g_Math.NormalizeVector( vToEnemy );
							g_Math.ClampAngles( vToEnemy );

							static float flEnemy;

							if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
								flEnemy = vToEnemy.y;

							if( g_Var.AntiAim.StandStyle == 1 )
							{
								if( m_bUpdated )
									pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
							}
							else
								if( g_Var.AntiAim.StandStyle == 2 )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								}
								else
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
								}
						}
						else
						{
							if( g_Var.AntiAim.StandStyle == 1 )
							{
								if( m_bUpdated )
									pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

							}
							else
								if( g_Var.AntiAim.StandStyle == 2 )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								}
								else
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
								}
						}
					}
				}
				else
				{
				    if ((m_bLeft || m_bRight || m_bBack || m_bFront) && !(g_Var.AntiAim.StandStyle == 1 && flAdd > 89))
					{
						if( m_bLeft )
						{
							if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
							{
								Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
								g_Math.NormalizeVector( vToEnemy );
								g_Math.ClampAngles( vToEnemy );

								static float flEnemy;

								if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
									flEnemy = vToEnemy.y;

								if( g_Var.AntiAim.StandStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : flEnemy + flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.StandStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance : flEnemy + flSideTolerance;
							}
							else
							{
								if( g_Var.AntiAim.StandStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : flYaw + flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.StandStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance : flYaw + flSideTolerance;
							}
						}
						else
							if( m_bRight )
							{
								if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
								{
									Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
									g_Math.NormalizeVector( vToEnemy );
									g_Math.ClampAngles( vToEnemy );

									static float flEnemy;

									if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
										flEnemy = vToEnemy.y;

									if( g_Var.AntiAim.StandStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : flEnemy - flSideTolerance + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.StandStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance : flEnemy - flSideTolerance;
								}
								else
								{
									if( g_Var.AntiAim.StandStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : flYaw - flSideTolerance + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.StandStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance : flYaw - flSideTolerance;
								}
							}
							else
								if( m_bBack )
								{
									if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
									{
										Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
										g_Math.NormalizeVector( vToEnemy );
										g_Math.ClampAngles( vToEnemy );

										static float flEnemy;

										if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
											flEnemy = vToEnemy.y;

										if( g_Var.AntiAim.StandStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEnemy + 180 + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.StandStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + 180 : flEnemy + 180;
									}
									else
									{
										if( g_Var.AntiAim.StandStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flYaw + 180 + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.StandStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + 180 : flYaw + 180;
									}

								}
								else
									if( m_bFront )
									{
										if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
										{
											Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
											g_Math.NormalizeVector( vToEnemy );
											g_Math.ClampAngles( vToEnemy );

											static float flEnemy;

											if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
												flEnemy = vToEnemy.y;

											if( g_Var.AntiAim.StandStyle == 1 )
												pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : flEnemy + JitterRange( -flAdd, flAdd );
											else
												if( g_Var.AntiAim.StandStyle == 2 )
													pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked : flEnemy;
										}
										else
										{
											if( g_Var.AntiAim.StandStyle == 1 )
												pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : flYaw + JitterRange( -flAdd, flAdd );
											else
												if( g_Var.AntiAim.StandStyle == 2 )
													pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked : flYaw;
										}
									}
					}
					else
					{
						if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
						{
							Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
							g_Math.NormalizeVector( vToEnemy );
							g_Math.ClampAngles( vToEnemy );

							static float flEnemy;

							if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
								flEnemy = vToEnemy.y;

							if( g_Var.AntiAim.StandStyle == 1 )
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEnemy + 180 + JitterRange( -flAdd, flAdd );
							else
								if( g_Var.AntiAim.StandStyle == 2 )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flEnemy + 180 + flStaticAdd;
						}
						else
						{
							if( g_Var.AntiAim.StandStyle == 1 )
								pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flYaw + 180 + JitterRange( -flAdd, flAdd );
							else
								if( g_Var.AntiAim.StandStyle == 2 )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flYaw + 180 + flStaticAdd;
						}
					}
				}
				break;
			case 2:
				if( g_Var.AntiAim.Break == 1 )
				{
					if( m_bLeft || m_bRight || m_bBack || m_bFront )
					{
						if( m_bLeft )
						{
							if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
							{
								Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
								g_Math.NormalizeVector( vToEnemy );
								g_Math.ClampAngles( vToEnemy );

								static float flEnemy;

								if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
									flEnemy = vToEnemy.y;

								if( g_Var.AntiAim.StandStyle == 1 )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
								}
								else
									if( g_Var.AntiAim.StandStyle == 2 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
									}
							}
							else
							{
								if( g_Var.AntiAim.StandStyle == 1 )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

								}
								else
									if( g_Var.AntiAim.StandStyle == 2 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
									}
							}
						}
						else
							if( m_bRight )
							{
								if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
								{
									Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
									g_Math.NormalizeVector( vToEnemy );
									g_Math.ClampAngles( vToEnemy );

									static float flEnemy;

									if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
										flEnemy = vToEnemy.y;

									if( g_Var.AntiAim.StandStyle == 1 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									}
									else
										if( g_Var.AntiAim.StandStyle == 2 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										}
										else
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
										}
								}
								else
								{
									if( g_Var.AntiAim.StandStyle == 1 )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

									}
									else
										if( g_Var.AntiAim.StandStyle == 2 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - ( 80 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - ( 80 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										}
										else
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw - ( flSideTolerance - ( g_Var.AntiAim.BreakRad / 2 ) );
											else
												pCmd->viewangles.y = m_bLock ? flLocked - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw - ( flSideTolerance + ( g_Var.AntiAim.BreakRad / 2 ) );
										}
								}
							}
							else
								if( m_bBack )
								{
									if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
									{
										Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
										g_Math.NormalizeVector( vToEnemy );
										g_Math.ClampAngles( vToEnemy );

										static float flEnemy;

										if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
											flEnemy = vToEnemy.y;

										if( g_Var.AntiAim.StandStyle == 1 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
										}
										else
											if( g_Var.AntiAim.StandStyle == 2 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											}
											else
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) );
											}
									}
									else
									{
										if( g_Var.AntiAim.StandStyle == 1 )
										{
											if( m_bUpdated )
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

										}
										else
											if( g_Var.AntiAim.StandStyle == 2 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											}
											else
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) );
											}
									}
								}
								else
									if( m_bFront )
									{
										if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
										{
											Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
											g_Math.NormalizeVector( vToEnemy );
											g_Math.ClampAngles( vToEnemy );

											static float flEnemy;

											if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
												flEnemy = vToEnemy.y;

											if( g_Var.AntiAim.StandStyle == 1 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
											}
											else
												if( g_Var.AntiAim.StandStyle == 2 )
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												}
												else
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flEnemy + ( ( g_Var.AntiAim.BreakRad / 2 ) );
												}
										}
										else
										{
											if( g_Var.AntiAim.StandStyle == 1 )
											{
												if( m_bUpdated )
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );

											}
											else
												if( g_Var.AntiAim.StandStyle == 2 )
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												}
												else
												{
													if( m_bUpdated )
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) );
													else
														pCmd->viewangles.y = m_bLock ? flLocked + ( ( g_Var.AntiAim.BreakRad / 2 ) ) : flYaw + ( ( g_Var.AntiAim.BreakRad / 2 ) );
												}
										}
									}

					}
					else
					{
						if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
						{
							Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
							g_Math.NormalizeVector( vToEnemy );
							g_Math.ClampAngles( vToEnemy );

							static float flEnemy;

							if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
								flEnemy = vToEnemy.y;

							if( g_Var.AntiAim.StandStyle == 1 )
							{
								if( flEdge != 0.f )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEdge - ( g_Var.AntiAim.BreakRad / 2 ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEdge + ( g_Var.AntiAim.BreakRad / 2 ) + JitterRange( -flAdd, flAdd );
								}
								else
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
								}
							}
							else
								if( g_Var.AntiAim.StandStyle == 2 )
								{
									if( flEdge != 0.f )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge - ( g_Var.AntiAim.BreakRad / 2 ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge + ( g_Var.AntiAim.BreakRad / 2 ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
								}
								else
								{
									if( flEdge != 0.f )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEdge - ( g_Var.AntiAim.BreakRad / 2 ) + flStaticAdd;
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEdge + ( g_Var.AntiAim.BreakRad / 2 ) + flStaticAdd;
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEnemy + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEnemy + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
									}
								}
						}
						else
						{
							if( g_Var.AntiAim.StandStyle == 1 )
							{
								if( flEdge != 0.f )
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEdge - ( g_Var.AntiAim.BreakRad / 2 ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flEdge + ( g_Var.AntiAim.BreakRad / 2 ) + JitterRange( -flAdd, flAdd );
								}
								else
								{
									if( m_bUpdated )
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + JitterRange( -flAdd, flAdd );
								}
							}
							else
								if( g_Var.AntiAim.StandStyle == 2 )
								{
									if( flEdge != 0.f )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge - ( g_Var.AntiAim.BreakRad / 2 ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge + ( g_Var.AntiAim.BreakRad / 2 ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									}
								}
								else
								{
									if( flEdge != 0.f )
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEdge - ( g_Var.AntiAim.BreakRad / 2 ) + flStaticAdd;
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flEdge + ( g_Var.AntiAim.BreakRad / 2 ) + flStaticAdd;
									}
									else
									{
										if( m_bUpdated )
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flYaw + ( 180 - ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
										else
											pCmd->viewangles.y = m_bLock ? flLocked + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd : flYaw + ( 180 + ( g_Var.AntiAim.BreakRad / 2 ) ) + flStaticAdd;
									}
								}
						}
					}
				}
				else
				{
					if( m_bLeft || m_bRight || m_bBack || m_bFront )
					{
						if( m_bLeft )
						{
							if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
							{
								Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
								g_Math.NormalizeVector( vToEnemy );
								g_Math.ClampAngles( vToEnemy );

								static float flEnemy;

								if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
									flEnemy = vToEnemy.y;

								if( g_Var.AntiAim.StandStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : flEnemy + flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.StandStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance : flEnemy + flSideTolerance;
							}
							else
							{
								if( g_Var.AntiAim.StandStyle == 1 )
									pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance + JitterRange( -flAdd, flAdd ) : flYaw + flSideTolerance + JitterRange( -flAdd, flAdd );
								else
									if( g_Var.AntiAim.StandStyle == 2 )
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + flSideTolerance : flYaw + flSideTolerance;
							}
						}
						else
							if( m_bRight )
							{
								if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
								{
									Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
									g_Math.NormalizeVector( vToEnemy );
									g_Math.ClampAngles( vToEnemy );

									static float flEnemy;

									if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
										flEnemy = vToEnemy.y;

									if( g_Var.AntiAim.StandStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : flEnemy - flSideTolerance + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.StandStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance : flEnemy - flSideTolerance;
								}
								else
								{
									if( g_Var.AntiAim.StandStyle == 1 )
										pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance + JitterRange( -flAdd, flAdd ) : flYaw - flSideTolerance + JitterRange( -flAdd, flAdd );
									else
										if( g_Var.AntiAim.StandStyle == 2 )
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - flSideTolerance - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
										else
											pCmd->viewangles.y = m_bLock ? flLocked - flSideTolerance : flYaw - flSideTolerance;
								}
							}
							else
								if( m_bBack )
								{
									if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
									{
										Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
										g_Math.NormalizeVector( vToEnemy );
										g_Math.ClampAngles( vToEnemy );

										static float flEnemy;

										if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
											flEnemy = vToEnemy.y;

										if( g_Var.AntiAim.StandStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEnemy + 180 + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.StandStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + 180 : flEnemy + 180;
									}
									else
									{
										if( g_Var.AntiAim.StandStyle == 1 )
											pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flYaw + 180 + JitterRange( -flAdd, flAdd );
										else
											if( g_Var.AntiAim.StandStyle == 2 )
												pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
											else
												pCmd->viewangles.y = m_bLock ? flLocked + 180 : flYaw + 180;
									}

								}
								else
									if( m_bFront )
									{
										if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
										{
											Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
											g_Math.NormalizeVector( vToEnemy );
											g_Math.ClampAngles( vToEnemy );

											static float flEnemy;

											if( fabsf( flEnemy - vToEnemy.y ) >  g_Var.AntiAim.Tolerance )
												flEnemy = vToEnemy.y;

											if( g_Var.AntiAim.StandStyle == 1 )
												pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : flEnemy + JitterRange( -flAdd, flAdd );
											else
												if( g_Var.AntiAim.StandStyle == 2 )
													pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked : flEnemy;
										}
										else
										{
											if( g_Var.AntiAim.StandStyle == 1 )
												pCmd->viewangles.y = m_bLock ? flLocked + JitterRange( -flAdd, flAdd ) : flYaw + JitterRange( -flAdd, flAdd );
											else
												if( g_Var.AntiAim.StandStyle == 2 )
													pCmd->viewangles.y = m_bLock ? flLocked - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
												else
													pCmd->viewangles.y = m_bLock ? flLocked : flYaw;
										}
									}
					}
					else
					{
						if( pEnemy && g_Var.AntiAim.StandEnemyBased >= 1 )
						{
							Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
							g_Math.NormalizeVector( vToEnemy );
							g_Math.ClampAngles( vToEnemy );

							static float flEnemy;

							if( fabsf( flEnemy - vToEnemy.y ) > g_Var.AntiAim.Tolerance )
								flEnemy = vToEnemy.y;

							if( g_Var.AntiAim.StandStyle == 1 )
							{
								if( flEdge != 0.f )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEdge + JitterRange( -flAdd, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEnemy + 180 + JitterRange( -flAdd, flAdd );
							}
							else
								if( g_Var.AntiAim.StandStyle == 2 )
								{
									if( flEdge != 0.f )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEnemy + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								}
								else
								{
									if( flEdge != 0.f )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flEdge + flStaticAdd;
									else
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flEnemy + 180 + flStaticAdd;
								}
						}
						else
						{
							if( g_Var.AntiAim.StandStyle == 1 )
							{
								if( flEdge != 0.f )
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flEdge + JitterRange( -flAdd, flAdd );
								else
									pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : flYaw + 180 + JitterRange( -flAdd, flAdd );
							}
							else
								if( g_Var.AntiAim.StandStyle == 2 )
								{
									if( flEdge != 0.f )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flEdge - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
									else
										pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : flYaw + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
								}
								else
								{
									if( flEdge != 0.f )
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flEdge + flStaticAdd;
									else									
										pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : flYaw + 180 + flStaticAdd;
								}
						}
					}
				}
				break;
			}
		}
		m_flReal = pCmd->viewangles.y;
	}
	else
	{
		float flAdd = g_Var.AntiAim.AirValue;
		float flStaticAdd = g_Var.AntiAim.AirValue - 90;
		float flSpeed = g_Var.AntiAim.AirSpeed * 30;
		pEnemy = GetBestPlayer( pLocal, g_Var.AntiAim.AirEnemyBased );
		float flEdge = CanEdge( pLocal, pEnemy, pCmd );

		switch( g_Var.AntiAim.Air )
		{
		case 1:
			if( pEnemy && g_Var.AntiAim.AirEnemyBased >= 1 )
			{
				Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
				g_Math.NormalizeVector( vToEnemy );
				g_Math.ClampAngles( vToEnemy );

				if( g_Var.AntiAim.AirStyle == 1 )
					pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : vToEnemy.y + 180 + JitterRange( -flAdd, flAdd );
				else
					if( g_Var.AntiAim.AirStyle == 2 )
						pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
					else
						pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : vToEnemy.y + 180 + flStaticAdd;
			}
			else
			{
				if( g_Var.AntiAim.AirStyle == 1 )
					pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + 180 + JitterRange( -flAdd, flAdd );
				else
					if( g_Var.AntiAim.AirStyle == 2 )
						pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
					else
						pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : pCmd->viewangles.y + 180 + flStaticAdd;
			}
			break;
		case 2:
			if( flEdge != 0.f )
				pCmd->viewangles.y = flEdge;
			else
			{
				if( pEnemy && g_Var.AntiAim.AirEnemyBased >= 1 )
				{
					Vector vToEnemy = g_Math.CalcAngles( pLocal->GetAbsOrigin(), pEnemy->GetAbsOrigin() );
					g_Math.NormalizeVector( vToEnemy );
					g_Math.ClampAngles( vToEnemy );

					if( g_Var.AntiAim.AirStyle == 1 )
						pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : vToEnemy.y + 180 + JitterRange( -flAdd, flAdd );
					else
						if( g_Var.AntiAim.AirStyle == 2 )
							pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : vToEnemy.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
						else
							pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : vToEnemy.y + 180 + flStaticAdd;
				}
				else
				{
					if( g_Var.AntiAim.AirStyle == 1 )
						pCmd->viewangles.y = m_bLock ? flLocked + 180 + JitterRange( -flAdd, flAdd ) : pCmd->viewangles.y + 180 + JitterRange( -flAdd, flAdd );
					else
						if( g_Var.AntiAim.AirStyle == 2 )
							pCmd->viewangles.y = m_bLock ? flLocked + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd ) : pCmd->viewangles.y + 180 - ( flAdd / 2 ) + fmod( g_pGlobalVars->curtime / 0.9f * flSpeed, flAdd );
						else
							pCmd->viewangles.y = m_bLock ? flLocked + 180 + flStaticAdd : pCmd->viewangles.y + 180 + flStaticAdd;
				}
			}
			break;
		}
	}
}

float CAntiAim::CanEdge( CBaseEntity * pLocal, CBaseEntity * pEnemy, CUserCmd * pCmd )
{
	if (pEnemy == nullptr)
		return 0.f;

	CBaseCombatWeapon* pWeapon = pEnemy->GetWeapon();

	if (pWeapon == nullptr)
		return 0.f;

	auto checkDamage = [ & ] ( Vector vPoint, float& flDamage, CBaseEntity * pEnemy, CBaseCombatWeapon* pWeapon) -> bool
	{
		CAutowall::FireBulletData BulletData = CAutowall::FireBulletData( vPoint );
		BulletData.filter.pSkip = pLocal;

		Vector vAngle;
		g_Math.CalcAngle( BulletData.src, g_Math.GetHitspot( pEnemy, 0 ), vAngle );
		g_Math.AngleVector( vAngle, BulletData.direction );
		VectorNormalize( BulletData.direction );

		if( g_Autowall.SimulateFireBullet( pLocal, pEnemy, pWeapon, BulletData, 0 ) )
			return true;

		flDamage = BulletData.current_damage;

		return false;
	};

	auto RotateAndExtendPosition = [ & ] ( float yaw, float distance ) -> Vector
	{
		Vector vDir;
		g_Math.AngleVector( Vector( 0, yaw, 0 ), vDir );

		return pLocal->GetEyePosition() + ( vDir * distance );
	};

	const float flToEnemy = g_Math.CalcAngle( pEnemy->GetOrigin(), pLocal->GetOrigin() ).y,
		flRight = flToEnemy - 90.f,
		flLeft = flToEnemy + 90.f,
		flBack = flToEnemy + 180.f,
		flTolerance = 15.f;

	Vector vFrontHead = RotateAndExtendPosition( flToEnemy, 15.f ),
		vRightHead = RotateAndExtendPosition( flRight, 15.f ),
		vLeftHead = RotateAndExtendPosition( flLeft, 15.f ),
		vBackHead = RotateAndExtendPosition( flBack, 15.f );

	float flFrontDmg = 0.f, flRightDmg = 0.f, flLeftDmg = 0.f, flBackDmg = 0.f, flBestYaw;

	checkDamage( vFrontHead, flFrontDmg, pEnemy, pWeapon );
	checkDamage( vLeftHead, flLeftDmg, pEnemy, pWeapon);
	checkDamage( vRightHead, flRightDmg, pEnemy, pWeapon);
	checkDamage( vBackHead, flBackDmg, pEnemy, pWeapon);

	if( flRightDmg > flTolerance && flLeftDmg > flTolerance )
	{
		if( flBackDmg < flTolerance && flTolerance > flFrontDmg )
		{
			//we prefer backwards so if both equals the same use backwards
			if( flBackDmg >= flFrontDmg )
				flBestYaw = flToEnemy;
			else
				flBestYaw = flBack;
		}
	}
	else
	{
		//if we are above someone check if back or front would be good
		if( g_Math.NormalizePitch( g_Math.CalcAngle( pLocal->GetOrigin(), pEnemy->GetOrigin() ).x ) > 15.f && ( flBackDmg < flTolerance || flFrontDmg < flTolerance ) )
		{
			//we prefer backwards so if both equals the same use backwards
			if( flBackDmg >= flFrontDmg )
				flBestYaw = flToEnemy;
			else
				flBestYaw = flBack;
		}

		if( flRightDmg == flLeftDmg )
		{
			if( flBackDmg >= flFrontDmg )
				flBestYaw = flToEnemy;
			else
				flBestYaw = flBack;
		}
		else
		{
			if( flRightDmg < flLeftDmg )
			{
				//make sure right is the best angle
				if( flRightDmg > flBackDmg || flFrontDmg < flRightDmg )
				{
					if( flBackDmg >= flFrontDmg )
						flBestYaw = flToEnemy;
					else
						flBestYaw = flBack;
				}
				else
				{
					//seems like right is the best spot apply it
					flBestYaw = flRight;
				}
			}
			else
			{
				//make sure right is the best angle
				if( flLeftDmg > flBackDmg || flFrontDmg < flLeftDmg )
				{
					if( flBackDmg >= flFrontDmg )
						flBestYaw = flToEnemy;
					else
						flBestYaw = flBack;
				}
				else
				{
					//seems like left is the best spot apply it
					flBestYaw = flLeft;
				}
			}
		}
	}
	return flBestYaw;
}

void CAntiAim::PredictLowerBodyUpdate( CBaseEntity* pLocal )
{
	const float flCurTime = UserCmd::flFixedCurTime;
	static float flLastMove, flLastLowerBody;

	CAnimState* pAnimState = pLocal->GetAnimState();

	if( !pAnimState )
		return;

	if ((pAnimState->m_velocity > 0.1f || abs(pLocal->GetVelocity().z) > 100.f) && !m_bInFakeWalk)
	{
		m_flNextUpdate = flCurTime + 0.22f;
		flLastMove = flCurTime;
	}

	if( flLastLowerBody != pLocal->LowerBodyYawTarget() )
	{
		flLastLowerBody = pLocal->LowerBodyYawTarget();

		if( fabs(flCurTime - flLastMove ) > 0.3 && fabs( g_Math.NormalizeYaw( pLocal->LowerBodyYawTarget() - m_flLastLowerBodyAngle ) ) > 35.f )
			m_flNextUpdate = flCurTime + 1.1 + g_pGlobalVars->interval_per_tick;
	}

	if( m_flNextUpdate < flCurTime)
	{
		m_flNextUpdate = flCurTime + 1.1f;
		m_bLbyUpdate = true;
	}
}