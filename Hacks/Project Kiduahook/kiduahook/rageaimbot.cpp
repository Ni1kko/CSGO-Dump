#include "rageaimbot.h"

CRage g_Rage;

bool CRage::CanTarget( CBaseEntity* pLocal, CUserCmd* pCmd, CBaseCombatWeapon* pWeapon )
{
	if( pWeapon->IsOther() )
		return false;

//	if( pWeapon->IsEmpty() )
//		return false;

	if( !g_Misc.CanShoot( pLocal, pWeapon ) /*|| !(g_Misc.bInRevolver && pWeapon->GetWeaponID() == WEAPON_R8REVOLVER)*/)
		return false;

/*	if( pWeapon->GetNextPrimaryAttack() > g_pGlobalVars->curtime )
		return false;

	if( pLocal->GetNextAttack() > g_pGlobalVars->curtime )
		return false;*/

/*	if( pWeapon->GetWeaponID() == WEAPON_R8REVOLVER && g_Var.Misc.Untrusted )
	{
		pCmd->buttons |= IN_ATTACK;

		float flPostponeFireReady = pWeapon->GetFireReadyTime();

		if( !flPostponeFireReady || flPostponeFireReady - 0.5f >= g_pGlobalVars->curtime )
			return false;

		pCmd->buttons &= ~IN_ATTACK;
	}*/

	return true;
}

Vector CRage::CalculatePoint( CBaseEntity* pLocal, CBaseEntity* pEnemy, CBaseCombatWeapon* pWeapon, int iHitbox, float flMinDamage, matrix3x4_t Matrix[], float &flDamage, int iMultipoints )
{
	std::vector<Vector> vHitbox;
	Vector vReturn = Vector( 0.f, 0.f, 0.f );
	float flCurrentDamage = 0.f;

	const model_t* pModel = pEnemy->GetModel();

	if( !pModel )
		return Vector( 0.f, 0.f, 0.f );

	studiohdr_t* pStudioHdr = g_pModel->GetStudiomodel( pModel );

	if( !pStudioHdr )
		return Vector( 0.f, 0.f, 0.f );

	mstudiobbox_t* pHitbox = pStudioHdr->pHitbox( iHitbox, 0 );

	if( !pHitbox )
		return Vector( 0.f, 0.f, 0.f );

	float flMod = pHitbox->radius != -1.f ? pHitbox->radius : 0.f;

	Vector vMin, vMax;
	g_Math.VectorTransform( pHitbox->bbmin, Matrix[ pHitbox->bone ], vMin );
	g_Math.VectorTransform( pHitbox->bbmax, Matrix[ pHitbox->bone ], vMax );

	Vector vCenter = ( vMin + vMax ) * 0.5f;

	Vector vAngle = g_Math.CalcAngles( pLocal->GetEyePosition(), vCenter );

	float flCos = cosf( DEG2RAD( vAngle.y ) );
	float flSin = sinf( DEG2RAD( vAngle.y ) );

	int iWpnGroup = pWeapon->GetGroupID();

	/*	//basically we could apply 5 points in diff heights
				//so we want that for head and also for body for arms and legs we dont spread em we just add to shin and stuff 
				/*
			   - - - - - - x - - - - - -
				\                       \
                 \                       \
                  x           x           x
                   \                       \
                    \                       \
                     - - - - - - x - - - - - -

					 if (hitbox->m_flRadius == -1.0f) obb
					 if ( radius > -1.f )//sphere
				*/

	/*float spread_rad = ctx.m_weapon_inaccuracy + ctx.m_weapon_spread;

            float corner_angle = 90.f - math::rad2deg( spread_rad );
            float corner_rad = math::deg2rad( corner_angle );

            float dist = shot_origin.dist( center );

            float spread_radius = ( dist / sin( corner_rad ) ) * sin( spread_rad );

            if ( hitbox->radius > spread_radius )
            {
                hitbox_radius = hitbox->radius - spread_radius;
                pointscale = hitbox_radius / hitbox->radius;
            }*/

	//maybe a switch case to make it better to read :thinking: also we have to make sphere mpoints this way is garbbbb

	if( iMultipoints )
	{
		float flDownScale = pWeapon->GetInaccuracy() - ( ( pLocal->GetEyePosition() - vCenter ).Length() / 4000.f );

		float flHeadScale = g_Var.Ragebot[ iWpnGroup ].AutoPointScale ? clamp( g_Var.Ragebot[ iWpnGroup ].HeadPointScale - flDownScale, 0.f, g_Var.Ragebot[ iWpnGroup ].HeadPointScale )
			: g_Var.Ragebot[ iWpnGroup ].HeadPointScale;

		float flHeadResize = flMod * flHeadScale;

		float flBodyScale = g_Var.Ragebot[ iWpnGroup ].AutoPointScale ? clamp( g_Var.Ragebot[ iWpnGroup ].PointScale - flDownScale, 0.f, g_Var.Ragebot[ iWpnGroup ].PointScale )
			: g_Var.Ragebot[ iWpnGroup ].PointScale;

		float flBodyResize = flMod * flBodyScale;

		if( iHitbox == HITBOX_HEAD && g_Var.Ragebot[ iWpnGroup ].Multipointbox[ 0 ] ) //head
		{
			vHitbox.emplace_back( vCenter );

			if( !g_Misc.IsHitboxVisible( pLocal, pEnemy, vCenter, 0 ) )
			{
				vHitbox.emplace_back( Vector( vCenter.x + flCos + flHeadResize * flSin, vCenter.y + flSin - flHeadResize * flCos, vCenter.z ) );
				vHitbox.emplace_back( Vector( vCenter.x + flCos - flHeadResize * flSin, vCenter.y + flSin + flHeadResize * flCos, vCenter.z ) );
				
				Vector& vUpper = vMin;

				if( vMax.z > vMin.z )
					vUpper = vMax;
				
				vHitbox.emplace_back( Vector( vUpper.x, vUpper.y, vUpper.z + flHeadResize ) );
				vHitbox.emplace_back( Vector( vUpper.x + flCos + flHeadResize * flSin, vUpper.y + flSin - flHeadResize * flCos, vUpper.z ) );
				vHitbox.emplace_back( Vector( vUpper.x + flCos - flHeadResize * flSin, vUpper.y + flSin + flHeadResize * flCos, vUpper.z ) );
			}
		}
		else
			if( ( iHitbox <= HITBOX_UPPER_CHEST && iHitbox >= HITBOX_NECK ) && g_Var.Ragebot[ iWpnGroup ].Multipointbox[ 1 ] ) //body
			{
				vHitbox.emplace_back( vCenter );

				if( !g_Misc.IsVisible( pLocal, pEnemy, vCenter ) )
				{
					if( iHitbox == HITBOX_NECK )
					{
						vHitbox.emplace_back( Vector( vCenter.x, vCenter.y, vCenter.z + flBodyResize ) );
						vHitbox.emplace_back( Vector( vCenter.x + flCos + flBodyResize * flSin, vCenter.y + flSin - flBodyResize * flCos, vCenter.z ) );
						vHitbox.emplace_back( Vector( vCenter.x + flCos - flBodyResize * flSin, vCenter.y + flSin + flBodyResize * flCos, vCenter.z ) );
					}
					else
					{
						vHitbox.emplace_back( Vector( vCenter.x + flCos + flBodyResize * flSin, vCenter.y + flSin - flBodyResize * flCos, vCenter.z ) );
						vHitbox.emplace_back( Vector( vCenter.x + flCos - flBodyResize * flSin, vCenter.y + flSin + flBodyResize * flCos, vCenter.z ) );
					}
				}
			}
			else
				if( ( iHitbox == HITBOX_RIGHT_UPPER_ARM || iHitbox == HITBOX_LEFT_UPPER_ARM ) && g_Var.Ragebot[ iWpnGroup ].Multipointbox[ 1 ] ) // arms
				{
					vHitbox.emplace_back( Vector( vCenter.x, vCenter.y, vCenter.z ) );

					if( !g_Misc.IsVisible( pLocal, pEnemy, vCenter ) )
					{
						Vector& vUpper = vMin;

						if( vMax.z > vMin.z )
							vUpper = vMax;

						Vector shoulder_top = Vector( vUpper.x, vUpper.y, vUpper.z + flBodyResize );

						Vector a = Vector( vUpper.x + flCos + flBodyResize * flSin, vUpper.y + flSin - flBodyResize * flCos, vUpper.z );
						Vector b = Vector( vUpper.x + flCos - flBodyResize * flSin, vUpper.y + flSin + flBodyResize * flCos, vUpper.z );

						float a_dist = pEnemy->GetOrigin().DistTo( a );
						float b_dist = pEnemy->GetOrigin().DistTo( b );

						Vector shoulder_side = a_dist > b_dist ? a : b;

						vHitbox.emplace_back( ( shoulder_side + shoulder_top ) * 0.5f );
					}
				}
			else
				if (iHitbox <= HITBOX_RIGHT_FOOT && iHitbox >= HITBOX_LEFT_FOOT && g_Var.Ragebot[iWpnGroup].Multipointbox[4])
				{
					Vector vPos = g_Math.MatrixGetPosition(Matrix[pHitbox->bone]);

					matrix3x4_t mRotation;
					g_Math.AngleMatrix(pHitbox->rotation, mRotation);

					matrix3x4_t mTransform = g_Math.MultiplyMatrix(Matrix[pHitbox->bone], mRotation);

					QAngle vAngles;
					g_Math.MatrixAngle(mTransform, vAngles, vPos);

					Vector vDirection;
					g_Math.AngleVector(Vector(vAngles.x, vAngles.y, vAngles.z), vDirection);

					vCenter.z += 1.f;
					vHitbox.emplace_back(vCenter);

						/*	vDirection *= 4.5f;
							vHitbox.emplace_back(2, vCenter + vDirection);
							vHitbox.emplace_back(3, vCenter - vDirection);*/
				}
				else
					vHitbox.emplace_back(vCenter);
	}
	else
		vHitbox.emplace_back( vCenter );

	for( Vector vCurrent : vHitbox )
	{
		Vector vScreen;

		if( g_pDebugOverlay->ScreenPosition( vCurrent, vScreen ) != 1 && g_Var.Misc.DebugMultiPoint > 0 )
			g_Draw.FillRGBA( vScreen.x, vScreen.y, 2, 2, 255, 255, 255, 255 );

		float flCurDamage;

		if( !g_Autowall.GetDamage( vCurrent, pLocal, pEnemy, pWeapon, flCurDamage, flMinDamage ) )
		{
			flCurrentDamage = 0.f;
			continue;
		}

		if( flCurDamage > flCurrentDamage )
		{
			flCurrentDamage = flCurDamage;
			flDamage = flCurDamage;
			vReturn = vCurrent;
		}
	}

	return vReturn;
}

std::shared_ptr<sHistoryRecords> sRecord = nullptr;

sRageBot CRage::GetBestTarget( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd, int iWpnID )
{
	sRageBot RageRecord;
	std::vector< CBaseEntity* > vEnts;

	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy->PlayerCheckWithoutDormant() )
			continue;

		if( pEnemy->IsDormant() )
		{
			iShotsFired[ i ] = 0;
			continue;
		}

		if( pEnemy->IsProtected() )
			continue;

		if( pEnemy == pLocal )
			continue;

		if(g_Misc.bInSameTeam(pLocal, pEnemy ) && !g_Var.Ragebot[ iWpnID ].Team )
			continue;

		vEnts.push_back( pEnemy );
	}

	auto GetClosest = [ & ] () -> sRageBot
	{
		float flRange = pLocal->GetWeapon()->GetWeaponData()->range;
		sRageBot _RageRecord;

		for( CBaseEntity* pEntity : vEnts )
		{
			if( !pEntity || pEntity == nullptr )
				continue;

			float flDist;
			std::shared_ptr<sHistoryRecords> sBestRecord = nullptr;

			if( ( flDist = g_Math.GetDistance( pLocal->GetOrigin(), pEntity->GetOrigin() ) ) < flRange )
			{
				Vector vAimSpot = Vector( 0.f, 0.f, 0.f );

				auto PosAdjustMode = g_Var.Ragebot[iWpnID].PosAdjust;

				if (PosAdjustMode >= 1 || pWeapon->GetWeaponID() == WEAPON_TASER)
					sBestRecord = g_History.GetBestRecordAndPoint(pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot);
				else
					GetBestPoint( pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot );
		
				if (!vAimSpot.IsEmpty() && (PosAdjustMode != 2 || sBestRecord))
				{
					if( pWeapon->GetWeaponID() == WEAPON_TASER && Vector( vAimSpot - pLocal->GetEyePosition() ).Length() >= 150.f)
						continue;

					flRange = flDist;
					_RageRecord.bValid = true;
					_RageRecord.pEntity = pEntity;
					sRecord = sBestRecord;
					_RageRecord.vAim = vAimSpot;
				}
			}
		}

		return _RageRecord;
	};

	auto GetFov = [ & ] () -> sRageBot
	{
		float flHighestFov = 9999.f;
		std::shared_ptr<sHistoryRecords> sBestRecord = nullptr;

		sRageBot _RageRecord;

		Vector vView;
		g_pEngine->GetViewAngles( vView );

		for( CBaseEntity* pEntity : vEnts )
		{
			if( !pEntity || pEntity == nullptr )
				continue;

			float flFov = g_Math.GetFoV( vView, pLocal->GetEyePosition(), pEntity->GetEyePosition() );

			if( flFov < flHighestFov && g_Math.GetDistance( pLocal->GetOrigin(), pEntity->GetOrigin() ) < pLocal->GetWeapon()->GetWeaponData()->range )
			{
				Vector vAimSpot = Vector( 0.f, 0.f, 0.f );
				auto PosAdjustMode = g_Var.Ragebot[iWpnID].PosAdjust;

				if(PosAdjustMode >= 1 )
					sBestRecord = g_History.GetBestRecordAndPoint( pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot );
				else
					GetBestPoint( pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot );
			
				if( !vAimSpot.IsEmpty() && (PosAdjustMode != 2 || sBestRecord))
				{
					flHighestFov = flFov;
					_RageRecord.bValid = true;
					_RageRecord.pEntity = pEntity;
					_RageRecord.vAim = vAimSpot;
					sRecord = sBestRecord;
				}
			}
		}

		return _RageRecord;
	};

	auto GetRandom = [ & ] () -> sRageBot
	{
		float flRange = pLocal->GetWeapon()->GetWeaponData()->range;
		std::vector<sRageBot> vRandEnts;
		std::shared_ptr<sHistoryRecords> sBestRecord = nullptr;
		sRageBot _RageRecord;

		for( CBaseEntity* pEntity : vEnts )
		{
			if( !pEntity || pEntity == nullptr )
				continue;

			if( g_Math.GetDistance( pLocal->GetOrigin(), pEntity->GetOrigin() ) < flRange )
			{
				Vector vAimSpot = Vector( 0.f, 0.f, 0.f );

				auto PosAdjustMode = g_Var.Ragebot[iWpnID].PosAdjust;

				if (PosAdjustMode >= 1)
					sBestRecord = g_History.GetBestRecordAndPoint(pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot);
				else
					GetBestPoint( pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot );
				
				if (!vAimSpot.IsEmpty() && (PosAdjustMode != 2 || sBestRecord))
				{
					_RageRecord.bValid = true;
					_RageRecord.pEntity = pEntity;
					_RageRecord.vAim = vAimSpot;
					sRecord = sBestRecord;
				}
			}

			vRandEnts.push_back( _RageRecord );
		}

		std::random_device rDevice;
		std::mt19937 Random( rDevice() );
		shuffle( vRandEnts.begin(), vRandEnts.end(), Random );

		return !vRandEnts.empty() ? vRandEnts.at( 0 ) : _RageRecord;
	};

	auto GetLowestHealth = [ & ] () -> sRageBot
	{
		int iLowestDamage = 9999.f;
		std::shared_ptr<sHistoryRecords> sBestRecord = nullptr;
		sRageBot _RageRecord;

		for( CBaseEntity* pEntity : vEnts )
		{
			if( !pEntity || pEntity == nullptr )
				continue;

			int iHealth = pEntity->GetHealth();

			if( iHealth < iLowestDamage && g_Math.GetDistance( pLocal->GetOrigin(), pEntity->GetOrigin() ) < pLocal->GetWeapon()->GetWeaponData()->range )
			{
				Vector vAimSpot = Vector( 0.f, 0.f, 0.f );

				auto PosAdjustMode = g_Var.Ragebot[iWpnID].PosAdjust;

				if (PosAdjustMode >= 1)
					sBestRecord = g_History.GetBestRecordAndPoint(pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot);
				else
					GetBestPoint( pLocal, pEntity, pWeapon, pCmd, iWpnID, vAimSpot );
			
				if (!vAimSpot.IsEmpty() && (PosAdjustMode != 2 || sBestRecord))
				{
					iLowestDamage = iHealth;
					_RageRecord.bValid = true;
					_RageRecord.pEntity = pEntity;
					_RageRecord.vAim = vAimSpot;
					sRecord = sBestRecord;
				}
			}
		}

		return _RageRecord;
	};

	switch( pWeapon->GetWeaponID() == WEAPON_TASER ? 0 : g_Var.Ragebot[ iWpnID ].Selection )
	{
	case 0:
		RageRecord = GetClosest();
		break;
	case 1:
		RageRecord = GetFov();
		break;
	case 2:
		RageRecord = GetRandom();
		break;
	case 3:
		RageRecord = GetLowestHealth();
		break;
	}

	return RageRecord;
}

void CRage::GetBestPoint( CBaseEntity* pLocal, CBaseEntity* pEnemy, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd, int iWpnID, Vector& vFinal )
{
	static float flMinDamage;

	if( g_Var.Ragebot[ iWpnID ].AutoMindDamage )
	{
		if( pEnemy->GetHealth() > 0 && pEnemy->GetHealth() <= 20 )
		{
			switch( pWeapon->GetWeaponID() )
			{
			case WEAPON_AK47:
				flMinDamage = 5; break;
			case WEAPON_SCOUT:
				flMinDamage = 35; break;
			case WEAPON_AWP:
				flMinDamage = pEnemy->GetHealth() + 10; break;
			case WEAPON_SCAR20:
			case WEAPON_G3SG1:
				flMinDamage = 10; break;
			case WEAPON_TASER:
				flMinDamage = pEnemy->GetHealth() + 10; break;
			default:
				flMinDamage = 10; break;
			}
		}
		else
			if( pEnemy->GetHealth() > 20 && pEnemy->GetHealth() <= 50 )
			{
				switch( pWeapon->GetWeaponID() )
				{
				case WEAPON_AK47:
					flMinDamage = 15; break;
				case WEAPON_SCOUT:
					flMinDamage = 55; break;
				case WEAPON_AWP:
					flMinDamage = pEnemy->GetHealth() + 10; break;
				case WEAPON_SCAR20:
				case WEAPON_G3SG1:
					flMinDamage = 25; break;
				case WEAPON_R8REVOLVER:
					flMinDamage = 30; break;
				case WEAPON_TASER:
					flMinDamage = pEnemy->GetHealth() + 10; break;
				default:
					flMinDamage = 20; break;
				}
			}
			else
				if( pEnemy->GetHealth() > 50 && pEnemy->GetHealth() <= 125 )
				{
					switch( pWeapon->GetWeaponID() )
					{
					case WEAPON_AK47:
						flMinDamage = 25; break;
					case WEAPON_SCOUT:
						flMinDamage = 80; break;
					case WEAPON_AWP:
						flMinDamage = pEnemy->GetHealth() + 10; break;
					case WEAPON_SCAR20:
					case WEAPON_G3SG1:
						flMinDamage = 40; break;
					case WEAPON_R8REVOLVER:
						flMinDamage = 60; break;
					case WEAPON_TASER:
						flMinDamage = pEnemy->GetHealth() + 10; break;
					default:
						flMinDamage = 20; break;
					}
				}
	}
	else
	{
		if( pWeapon->GetWeaponID() == WEAPON_TASER )
			flMinDamage = pEnemy->GetHealth() + 10;
		else
			flMinDamage = g_Var.Ragebot[ iWpnID ].Mindamage;
	}

	Vector vHitBox = Vector( 0.f, 0.f, 0.f );

	matrix3x4_t matrix[ 128 ];

	if( !pEnemy->SetupBones( matrix, 128, 0x100, pEnemy->GetSimulationTime() ) )
		return;

	float flDamage = 0.f;

	if( g_Var.Ragebot[ iWpnID ].Multibox )
	{
		Vector vPriorbox;

		if( g_Var.Ragebot[ iWpnID ].BodyAimHp > 0.9 && g_Var.Ragebot[ iWpnID ].BodyAimHp >= pEnemy->GetHealth() )
			vPriorbox = CalculatePoint( pLocal, pEnemy, pWeapon, HITBOX_PELVIS, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );
		else
			vPriorbox = CalculatePoint( pLocal, pEnemy, pWeapon, g_Var.Ragebot[ iWpnID ].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );

		if( g_Misc.IsVisible( pLocal, pEnemy, vPriorbox ) )
			vHitBox = vPriorbox;
		else
		{
			Vector vScreen = Vector( 0.f, 0.f, 0.f );
			Vector vHitbox = Vector( 0.f, 0.f, 0.f );
			float flCurrentDamage = 0.f;

			for( int i = 0; i < HITBOX_MAX; i++ )
			{
				if( i <= 1 )
				{
					if (GetAsyncKeyState(g_Var.Ragebot[iWpnID].ForceBaim) && g_Var.Ragebot[iWpnID].ForceBaim > 0)
						continue;

					if( pWeapon->GetWeaponID() == WEAPON_TASER )
						continue;

					if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 0 ] )
						continue;

					if( g_Var.Ragebot[ iWpnID ].IgnoreHead[ 0 ] && !( pEnemy->GetFlags() & FL_ONGROUND ) )
						continue;

					if( g_Var.Ragebot[ iWpnID ].IgnoreHead[ 1 ] && !( g_ResolveInfo[ pEnemy->GetIndex() ].bLBYUpdate || pEnemy->GetVelocity().Length() > 75.f ) )
						continue;

					if( g_Var.Ragebot[ iWpnID ].BodyAfter > 0.9 && g_Var.Ragebot[ iWpnID ].BodyAfter <= iShotsFired[ pEnemy->GetIndex() ] )
						continue;
				}

				if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 1 ] && ( i <= 6 && i >= 2 ) )
					continue;

				if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 3 ] && ( i <= 10 && i >= 7 ) )
					continue;

				if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 4 ] && ( i <= 12 && i >= 11 ) )
					continue;

				if( !g_Var.Ragebot[ iWpnID ].Multiboxes[ 2 ] && ( i <= 18 && i >= 13 ) )
					continue;

				if( i >= 7 && i <= 18 )
				{
					if( pWeapon->GetWeaponID() == WEAPON_TASER )
						continue;

					if( g_Var.Ragebot[ iWpnID ].Limbs[ 0 ] && pEnemy->GetVelocity().Length() > 25.f )
						continue;

					if( g_Var.Ragebot[ iWpnID ].Limbs[ 1 ] && !( pEnemy->GetFlags() & FL_ONGROUND ) )
						continue;
				}

				vHitbox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );

//				if( g_Var.Ragebot[ iWpnID ].IgnoreHead[ 2 ] && ( i <= 6 && i >= 2 ) && flDamage > pEnemy->GetHealth() )
//					vHitbox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, i, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );

				if( vHitbox == Vector( 0.f, 0.f, 0.f ) )
					continue;

				if( flDamage > flCurrentDamage )
				{
					flCurrentDamage = flDamage;
					vHitBox = vHitbox;
					break;
				}
			}
		}
	}
	else
	{
		if( g_Var.Ragebot[ iWpnID ].BodyAfter >= 1 && g_Var.Ragebot[ iWpnID ].BodyAfter <= g_Rage.iShotsFired[ g_Rage.iTarget ] )
			vHitBox = CalculatePoint( pLocal, pEnemy, pWeapon, HITBOX_PELVIS, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );
		else
			vHitBox = CalculatePoint( pLocal, pEnemy, pWeapon, g_Var.Ragebot[ iWpnID ].Spot, flMinDamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );
	}

	vFinal = vHitBox;
}

void CRage::RunRage( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd ) 
{
	bInRage = false;
	g_Misc.bShouldShotRevolver = false;

	int iWpnID = pWeapon->GetGroupID();

	if( !g_Var.Ragebot[ iWpnID ].Enable )
		return;

	if (g_pClientState->GetChokedCmds() >= 7)
		return;

	sRageBot BestRecord = GetBestTarget( pLocal, pWeapon, pCmd, iWpnID );

	if( !BestRecord.bValid )
	{
		bShouldStop = false;
		return;
	}

	pWeapon->UpdateAccuracyPenalty();

	Vector vAim = g_Math.CalcAngles( pLocal->GetEyePosition(), BestRecord.vAim );

	if( pLocal->GetPunchAngle().Length() >= 10.f )
		return;

	if( g_Var.Ragebot[ iWpnID ].Recoil )
		vAim -= pLocal->GetPunchAngle() * 2.f;

	g_Math.NormalizeVector( vAim );
	g_Math.ClampAngles( vAim );

	Vector vView;
	g_pEngine->GetViewAngles(vView);

	if( g_Var.Ragebot[ iWpnID ].Spread )
		g_Math.GetSpreadFix( pLocal, ( pCmd->random_seed & 0xFF ) + 1, vAim, pCmd );

	bShotTick = false;

	if( CanTarget( pLocal, pCmd, pWeapon ) && g_Math.GetRageFoV( vView, pLocal->GetEyePosition(), BestRecord.vAim ) <= g_Var.Ragebot[ iWpnID ].Fov + 1 )
	{
		if( g_Var.Ragebot[ iWpnID ].Autozoom && pWeapon->IsSniper() && pWeapon->GetZoomLevel() == 0 )
		{
			pCmd->buttons |= IN_ATTACK2;
			return;
		}

		auto CurrentRestore = sPreBacktrackRecord(BestRecord.pEntity);
		//we could store bonecache ptr here and replace it after hitchance so we dont fuck up others ?
		if( g_Var.Ragebot[ iWpnID ].PosAdjust >= 1 )
		{
			// safety check
			if (!sRecord)
				return;

			g_History.Restore( BestRecord.pEntity, sRecord );
		}

		bShouldStop = true;
		
		if( g_Misc.HitChance( pLocal, vAim, pWeapon, BestRecord.pEntity, g_Var.Ragebot[ iWpnID ].Hitchance ) )
		{
			bShotTick = true;

			if( g_Var.Ragebot[ iWpnID ].Autoshoot )
			{
				bInRage = true;
				pCmd->buttons |= IN_ATTACK;
			}
			else
				if( GetAsyncKeyState( 1 ) )
				{
					bInRage = true;
					pCmd->buttons |= IN_ATTACK;
				}
		}

		//traces are done we can restore it ?
		if( g_Var.Ragebot[ iWpnID ].PosAdjust >= 1 )
		{
			//g_History.RestoreToCurrentRecord( BestRecord.pEntity );
			CurrentRestore.Restore();
		}

		g_Misc.bShouldShotRevolver = true;

		if( pCmd->buttons & IN_ATTACK )
		{
			g_History.vUsedRecord = sRecord;

			if( g_Var.Ragebot[ iWpnID ].PosAdjust >= 1 )
				pCmd->tick_count = g_History.GetTickCount( sRecord );
			else
				pCmd->tick_count = TIME_TO_TICKS( BestRecord.pEntity->GetSimulationTime() ) + TIME_TO_TICKS( g_History.GetLerpTime() );

			pCmd->viewangles = vAim;

			if( !g_Var.Ragebot[ iWpnID ].Silent )
				g_pEngine->SetViewAngles( vAim );

			iShotsFired[ BestRecord.pEntity->GetIndex() ]++;

			if( g_Var.AntiAim.Resolve > 0 )
				g_Resolver.AddShotSnapshot( BestRecord.pEntity, g_Resolver.player_resolve_records[ BestRecord.pEntity->GetIndex() ] );

			iTarget = BestRecord.pEntity->GetIndex();
		}
	}
}

void CRage::AutoStop( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd )
{
	if( !bShouldStop )
		return;

	Vector vVel = pLocal->GetVelocity();
	float flSpeed = vVel.Length();

	if( ( UserCmd::iOriginalFlags & FL_ONGROUND ) && !( pCmd->buttons & IN_RELOAD ) )
	{
		switch (g_Var.Ragebot[pWeapon->GetGroupID()].Autostop)
		{
		case 1:
			pCmd->sidemove = 0;
			pCmd->forwardmove = flSpeed > 20.f ? 450.f : 0.f;

			g_Math.RotateMovement(pCmd, g_Math.CalcAngle(Vector(0, 0, 0), vVel).y + 180.f);

			break;
		case 2:
			if (g_Misc.CanShoot(pLocal, pWeapon))
			{
				pCmd->sidemove = 0;
				pCmd->forwardmove = flSpeed > 20.f ? 450.f : 0.f;

				g_Math.RotateMovement(pCmd, g_Math.CalcAngle(Vector(0, 0, 0), vVel).y + 180.f);
			}
			break;
		case 3:
			CWeaponData* pWeaponData = pWeapon->GetWeaponData();

			if (pWeaponData)
				g_Misc.ClampWalk(pCmd, pWeaponData->movement_speed_zoom / 3.f);

			break;
		}
	}
}

void CRage::RunZeus( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd )
{
	bInZeus = false;

	if( pWeapon->GetWeaponID() != WEAPON_TASER )
		return;

	if( pWeapon->GetNextPrimaryAttack() > g_pGlobalVars->curtime )
		return;

	if( pLocal->GetNextAttack() > g_pGlobalVars->curtime )
		return;

	CWeaponData* pWeaponData = pWeapon->GetWeaponData();

	if( !pWeaponData )
		return;

	sRageBot BestRecord = GetBestTarget( pLocal, pWeapon, pCmd, pWeapon->GetGroupID() );

	if( !BestRecord.bValid )
		return;

	Vector vAim = g_Math.CalcAngles( pLocal->GetEyePosition(), BestRecord.vAim );

	vAimAngle = vAim;

	vAim -= pLocal->GetPunchAngle() * 2.f;

	g_Math.NormalizeVector( vAim );
	g_Math.ClampAngles( vAim );

/*	auto CurrentRestore = sPreBacktrackRecord(BestRecord.pEntity);

	// safety check
	if (!sRecord)
		return;

	g_History.Restore(BestRecord.pEntity, sRecord);*/

	if( g_Misc.CanShoot( pLocal, pWeapon ) && g_Misc.HitChance( pLocal, vAim, pWeapon, BestRecord.pEntity, 80 ) )
	{
		bInZeus = true;
		pCmd->buttons |= IN_ATTACK;
	}

//	CurrentRestore.Restore();

	if( pCmd->buttons & IN_ATTACK )
	{
	//	g_History.vUsedRecord = sRecord;

		pCmd->tick_count = TIME_TO_TICKS(BestRecord.pEntity->GetSimulationTime()) + TIME_TO_TICKS(g_History.GetLerpTime());
	//	pCmd->tick_count = g_History.GetTickCount(sRecord);

		pCmd->viewangles = vAim;

	//	if (g_Var.AntiAim.Resolve > 0)
	//		g_Resolver.AddShotSnapshot(BestRecord.pEntity, g_Resolver.player_resolve_records[BestRecord.pEntity->GetIndex()]);
	}

/*	Vector vAimPoint = Vector( 0.f, 0.f, 0.f );

	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy->PlayerCheck() )
			continue;

		if( pEnemy == pLocal )
			continue;

		if( pEnemy->GetTeamNumber() == pLocal->GetTeamNumber() )
			continue;

		float flDamage = 0.f;

		matrix3x4 matrix[ 128 ];

		if( !pEnemy->SetupBones( matrix, 128, 256, g_pEngine->GetLastTimeStamp() ) )
			continue;

		Vector vScreen;
		Vector vHitbox;
		float flCurrentDamage = 0.f;

		for( int i = 0; i <= 7; i++ )
		{
			if( !( g_ResolveInfo[ pEnemy->GetIndex() ].bLBYUpdate || pEnemy->GetVelocity().Length2D() > 75.f ) && ( i <= 1 ) )
				continue;

			vHitbox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, i, pEnemy->GetHealth() + 15, matrix, flDamage, 0 );

			if( vHitbox == Vector( 0.f, 0.f, 0.f ) )
				continue;

			if( flDamage > flCurrentDamage )
			{
				flCurrentDamage = flDamage;
				vAimPoint = vHitbox;
				break;
			}
		}

		if( vAimPoint == Vector( 0, 0, 0 ) )
			continue;

		if( Vector( vAimPoint - pLocal->GetEyePosition() ).Length() >= pWeaponData->flRange - 15.f )
			continue;

//		printf( "%3.0f - %3.0f \n", pWeaponData->flRange, Vector( vAimPoint - pLocal->GetEyePosition() ).Length() );

		Vector vAim = g_Math.CalcAngles( pLocal->GetEyePosition(), vAimPoint );

		g_Math.NormalizeVector( vAim );
		g_Math.ClampAngles( vAim );

		if( g_Misc.CanShoot( pLocal, pWeapon ) )
		{
			if( !g_Misc.HitChance( pLocal, vAim, pWeapon, pEnemy, 80 ) )
				continue;

			pCmd->buttons |= IN_ATTACK;
			bInZeus = true;

			if( pCmd->buttons & IN_ATTACK )
			{
				pCmd->tick_count = TIME_TO_TICKS( pEnemy->GetSimulationTime() + CMBacktracking::Get().GetLerpTime() );

				pCmd->viewangles = vAim;
			}
		}
	}*/
}