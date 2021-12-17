#include "misc.h"
#include "updateanimations.h"

CMisc g_Misc;

bool CMisc::CanShoot( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon )
{
	float flNextPrimaryAttack = pWeapon->GetNextPrimaryAttack();
	float flNextAttack = *( float* ) ( ( uintptr_t ) pLocal + Offsets::Weapon::m_flNextAttack );

	if( flNextPrimaryAttack > UserCmd::flFixedCurTime || pWeapon->GetClip() == 0 || flNextAttack > UserCmd::flFixedCurTime || ( UserCmd::bQueuedShot && g_Var.Misc.Untrusted ) /*|| g_pClientState->GetChokedCmds() == 1*/ )
		return false;
	else
		return true;
}

bool CMisc::IsHitboxVisible( CBaseEntity* pLocal, CBaseEntity* pEntity, Vector vHitSpot, int iHitBox )
{
	CEngineTraceClient::trace_t tr;
	CEngineTraceClient::Ray_t ray;
	CEngineTraceClient::CTraceFilter filter;
	filter.pSkip = pLocal;

	ray.Init( pLocal->GetEyePosition(), vHitSpot );
	g_pEngineTrace->TraceRay( ray, 0x4600400B, &filter, &tr );

	return ( tr.m_pEnt == pEntity || tr.fraction == 1.0f ) && tr.hitbox == iHitBox;
}

bool CMisc::IsVisible( CBaseEntity* pLocal, CBaseEntity* pEntity )
{
	CEngineTraceClient::trace_t tr;
	CEngineTraceClient::Ray_t ray;
	CEngineTraceClient::CTraceFilter filter;

	filter.pSkip = pLocal;

	ray.Init( pLocal->GetEyePosition(), pEntity->GetEyePosition() );
	g_pEngineTrace->TraceRay( ray, 0x4600400B, &filter, &tr );

	return ( tr.m_pEnt == pEntity || tr.fraction == 1.0f );
}

bool CMisc::IsVisible( CBaseEntity* pLocal, CBaseEntity* pEntity, Vector vEnd )
{
	CEngineTraceClient::trace_t tr;
	CEngineTraceClient::Ray_t ray;
	CEngineTraceClient::CTraceFilter filter;

	filter.pSkip = pLocal;

	ray.Init( pLocal->GetEyePosition(), vEnd );
	g_pEngineTrace->TraceRay( ray, 0x4600400B, &filter, &tr );

	return ( tr.m_pEnt == pEntity || tr.fraction == 1.0f );
}

bool CMisc::HittedEntity( CBaseEntity* pLocal, CUserCmd* pCmd )
{
	CEngineTraceClient::Ray_t ray;
	CEngineTraceClient::trace_t tr;

	Vector vStart, vEnd, vAngles;
	Vector vCurrentAngles;

	vStart = pLocal->GetEyePosition();

	g_pEngine->GetViewAngles( vCurrentAngles );

	g_Math.AngleVector( vCurrentAngles, vAngles );

	vAngles *= 8192.f;

	vEnd = vStart + vAngles;

	ray.Init( vStart, vEnd );

	CEngineTraceClient::CTraceFilter pTraceFilter;
	pTraceFilter.pSkip = pLocal;

	g_pEngineTrace->TraceRay( ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX, &pTraceFilter, &tr );

	if( tr.hitgroup < 8 && tr.hitgroup > 0 )
	{
		if( tr.m_pEnt == pLocal )
			return false;

		return true;
	}
	return false;
}

bool CMisc::bInSameTeam(CBaseEntity* pLocal, CBaseEntity* pEnemy)
{
	static ConVar* game_type;
	
	if (g_pEvents->bNewMap)
	{
		game_type = g_pICvar->FindVar("game_type");
		g_pEvents->bNewMap = false;
	}

	if (game_type && game_type->GetInt() == 6 ? false : (pEnemy->GetTeamNumber() == pLocal->GetTeamNumber()))
		return true;

	return false;
}

float CMisc::MaxAbsYawDelta(CBaseEntity* pLocal)
{
	CAnimState* pAnimState = pLocal->GetAnimState();

	if (!pAnimState)
		return 0.f;

	//max body yaw deg 58.f | min body yaw deg -58.f, its just static the same value so just hardcode it
	float v60 = 58.f;
	float v61 = -58.f;

	//max speed
	float v22 = 260.f;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if (pWeapon && pWeapon->GetWeaponData())
		v22 = max(0.001f, pWeapon->GetWeaponData()->movement_speed_zoom);

	//calc feet speed stand and ducked just like server would
	float v54 = (1.923077f / v22) * pAnimState->m_velocity;//for enemies get correct velocity
	float v58 = (2.9411764f / v22) * pAnimState->m_velocity;

	//feet speed calc
	float v57 = (((pAnimState->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * clamp(v54, 0.f, 1.f)) + 1.f;

	float v56 = pAnimState->m_fDuckAmount;

	//change feet speed in duck
	if (v56 > 0.f)
		v57 = v57 + ((clamp(v58, 0.f, 1.f) * v56) * (0.5f - v57));

	v61 *= v57;
	v60 *= v57;

	//return max yaw deg because all we need
	return v60;
}

bool CMisc::InSight( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon )
{
	auto RotateAndExtendPosition = [ & ] ( float yaw, float distance ) -> Vector
	{
		Vector vDir;
		g_Math.AngleVector( Vector( 0, yaw, 0 ), vDir );

		return pLocal->GetEyePosition() + ( vDir * distance );
	};

	auto checkDamage = [ & ] ( CBaseEntity* pEnemy, Vector vPoint ) -> float
	{
		CAutowall::FireBulletData BulletData = CAutowall::FireBulletData( vPoint );
		BulletData.filter.pSkip = pLocal;

		Vector vAngle;
		g_Math.CalcAngle( BulletData.src, g_Math.GetHitspot( pEnemy, 1 ), vAngle );
		g_Math.AngleVector( vAngle, BulletData.direction );
		VectorNormalize( BulletData.direction );

		if( !g_Autowall.SimulateFireBullet( pLocal, pEnemy, pWeapon, BulletData, 0 ) )
			return 0.f;

		return BulletData.current_damage;
	};

	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy->PlayerCheck() )
			continue;

		if( pEnemy->GetTeamNumber() == pLocal->GetTeamNumber() )
			continue;

		if( pEnemy == pLocal )
			continue;

		const float flRight = g_Math.CalcAngle( pEnemy->GetOrigin(), pLocal->GetOrigin() ).y - 90.f,
			flLeft = g_Math.CalcAngle( pEnemy->GetOrigin(), pLocal->GetOrigin() ).y + 90.f;

		Vector vRightHead = RotateAndExtendPosition( flRight, 20.f ),
			vLeftHead = RotateAndExtendPosition( flLeft, 20.f );

		//yes yes we should use trace hull but i cry (( / also add 64 to origin imagine if they are sideways :facepalm:
		float flLeftDamage = checkDamage( pEnemy, vLeftHead );
		float flRightDamage = checkDamage( pEnemy, vRightHead);
/*		if( flLeftDamage > 0.f )
			g_pDebugOverlay->AddLineOverlay( vLeftHead, pEnemy->GetOrigin() + Vector( 0, 0, 64.f ), 0, 255, 0, 255, g_pGlobalVars->interval_per_tick * 2 );

		if( flRightDamage > 0.f )
			g_pDebugOverlay->AddLineOverlay( vRightHead, pEnemy->GetOrigin() + Vector( 0, 0, 64.f ), 0, 255, 0, 255, g_pGlobalVars->interval_per_tick * 2 );*/

		if( g_Math.Max<int>( flLeftDamage, flRightDamage ) > 0 )
			return true;
	}

	return false;
}

void CMisc::FakeLag( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, bool &bSendPackets, CUserCmd* pCmd )
{
	bInLag = false;

	static int iTicks = 0;
	static bool bPacketDrop = false;
	bool bResetCounter = false;

	if (UserCmd::bInFakeDuck)
		return;

	if( !g_Var.Fakelag.Enable )
	{
		if (g_Var.AntiAim.Break >= 3)
			bSendPackets = pCmd->command_number % 2;

		return;
	}

	if (g_pEngine->IsVoiceRecording())
	{
		bSendPackets = pCmd->command_number % 2;
		return;
	}

	if( ( pCmd->buttons & IN_ATTACK ))
	{
		bSendPackets = pCmd->command_number % 2;
		return;
	}

	if (!g_Var.Fakelag.Flags[1] && !(UserCmd::iOriginalFlags & FL_ONGROUND))
	{
		bSendPackets = pCmd->command_number % 2;
		return;
	}

	if ((!g_Var.Fakelag.Flags[0] || g_Var.AntiAim.Break >= 3) && (UserCmd::iOriginalFlags & FL_ONGROUND) && (pLocal->GetVelocity().Length() < 0.1 || g_AntiAim.m_bGroundAdjust))
	{
		bSendPackets = pCmd->command_number % 2;
		return;
	}

	if (!(g_Var.Fakelag.Flags[4] && InSight(pLocal, pWeapon) || g_Var.Fakelag.Flags[2]) && pLocal->GetVelocity().Length() > 0.1 && (UserCmd::iOriginalFlags & FL_ONGROUND))
	{
		bSendPackets = pCmd->command_number % 2;
		return;
	}

	if( g_Var.Fakelag.Flags[ 3 ] && !( UserCmd::iOriginalFlags & FL_ONGROUND ) && ( pLocal->GetFlags() & FL_ONGROUND ) && pCmd->buttons & IN_JUMP )
		bSendPackets = false;

	bInLag = true;

	switch( g_Var.Fakelag.Style )
	{
	case 0: //default
		if( iTicks > 0 )
			bSendPackets = false;

		if( iTicks >= g_Var.Fakelag.Factor)
			iTicks = 0;

		iTicks++;
		break;
	case 1: //burst
		if( bPacketDrop )
			bSendPackets = false;

		if( bPacketDrop && iTicks > g_Var.Fakelag.Factor)
		{
			bResetCounter = true;
			bPacketDrop = false;
		}
		else
			if( !bPacketDrop && iTicks > 0 )
			{
				bResetCounter = true;
				bPacketDrop = true;
			}

		iTicks++;

		if( bResetCounter )
			iTicks = 0;
		break;
	case 2: //adaptive
		bSendPackets = bSendPackets >= g_Var.Fakelag.Factor || UserCmd::bBreakingLC;
		!bSendPackets ? ++iTicks : iTicks = 0;
	}
}

void CMisc::AutoRevolver( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd )
{
	if( !g_Var.Misc.Untrusted )
		return;

	if( pWeapon->GetWeaponID() != WEAPON_R8REVOLVER )
		return;

	float flTime = UserCmd::flFixedCurTime;
	static float flNextShot;

	float flNextPrimaryAttack = pWeapon->GetNextPrimaryAttack();
	float flNextAttack = *(float*)((uintptr_t)pLocal + Offsets::Weapon::m_flNextAttack);

	if( !(flNextPrimaryAttack > UserCmd::flFixedCurTime || pWeapon->GetClip() == 0 || flNextAttack > UserCmd::flFixedCurTime) && flNextShot >= flTime )
	{
		pCmd->buttons |= IN_ATTACK;
	}
	else
		flNextShot = flTime + 0.234375f;
}

bool CMisc::HitChance( CBaseEntity* pLocal, Vector vSpot, CBaseCombatWeapon* pWeapon, CBaseEntity* pEnemy, float flPercentage )
{
	CEngineTraceClient::CTraceFilterEntities filter;
	filter.pSkip = pLocal;

	CEngineTraceClient::Ray_t ray;
	CEngineTraceClient::trace_t trace;

	int iHits = 0;

	Vector vEyePosition = pLocal->GetEyePosition();

	//trace 128 seeds and check for awall damage apparently trace is a nigger and returns even if just a PIXEL is visible so 128 traces awall shouldnt be THAT bad iguess
	for( int i = 0; i <= 255; i++ )
	{
		Vector vBulletLocation = Vector( 0.f, 0.f, 0.f );

		g_Math.BulletLocation( pLocal, pWeapon, vSpot, vBulletLocation );

		if (i <= 150)
		{
			float flCurDamage;
			if (g_Autowall.GetDamage(vBulletLocation, pLocal, pEnemy, pWeapon, flCurDamage, 1))
				iHits++;
		}
		else
		{
			ray.Init(vEyePosition, vBulletLocation);

			g_pEngineTrace->ClipRayToEntity(ray, 0x4600400B, pEnemy, &trace);

			if (trace.m_pEnt == pEnemy)
				iHits++;
		}

		/*	float flCurDamage = CGamePenetration::GetPenetrationDamage( pLocal, pWeapon, vBulletLocation );

		if( flCurDamage >= 1.f )
			iHits++;*/

//		g_pDebugOverlay->AddLineOverlay( pLocal->GetEyePosition(), vBulletLocation, 255, 255, 255, false, g_pGlobalVars->interval_per_tick * 2 );
//		g_pDebugOverlay->AddSweptBoxOverlay( pLocal->GetEyePosition(), vBulletLocation, Vector( -1, -1, -1 ), Vector( 1, 1, 1 ), QAngle( 0, 0, 0 ), 255, 0, 0, 255, g_pGlobalVars->interval_per_tick * 2 );

/*		ray.Init( vEyePosition, vBulletLocation );

		g_pEngineTrace->ClipRayToEntity( ray, 0x4600400B, pEnemy, &trace );

		if( trace.m_pEnt == pEnemy )
			iHits++;*/

		if( ( ( float ) iHits / ( float )2.55f ) >= flPercentage )
			return true;
	}

	return false;
}

void CMisc::RemoveFlash()
{
	IMaterial* MaterialFlash = g_pMatsystem->FindMaterial( "effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS );
	IMaterial* MaterialFlashW = g_pMatsystem->FindMaterial( "effects\\flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS );

	if( MaterialFlash != nullptr || !MaterialFlash->IsErrorMaterial() )
	{
		MaterialFlash->IncrementReferenceCount();
		MaterialFlash->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, g_Var.Visuals.World.Removals[ 0 ] );
	}

	if( MaterialFlashW != nullptr || !MaterialFlashW->IsErrorMaterial() )
	{
		MaterialFlashW->IncrementReferenceCount();
		MaterialFlashW->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, g_Var.Visuals.World.Removals[ 0 ] );
	}
}

void CMisc::RemoveSmoke()
{
	static std::vector<const char*> vSmokeMaterials = {
		"effects/overlaysmoke",
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull"
	};

	static std::vector< const char* > vSmokeMats = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	for( const char* cMaterialName : vSmokeMaterials )
	{
		IMaterial* pMaterial = g_pMatsystem->FindMaterial( cMaterialName, TEXTURE_GROUP_OTHER );

		if( pMaterial == nullptr || pMaterial->IsErrorMaterial() )
			continue;

		pMaterial->IncrementReferenceCount();
		pMaterial->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, g_Var.Visuals.World.Removals[ 1 ] );
	}

	for( const char* cMaterialName : vSmokeMats )
	{
		IMaterial* pMaterial = g_pMatsystem->FindMaterial( cMaterialName, TEXTURE_GROUP_OTHER );

		if( pMaterial == nullptr || pMaterial->IsErrorMaterial() )
			continue;

		pMaterial->IncrementReferenceCount();
		pMaterial->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, g_Var.Visuals.World.Removals[ 1 ] );
	}

	static auto iSmokeCount = *reinterpret_cast< uint32_t ** >( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "A3 ? ? ? ? 57 8B CB" ) + 1 );

	if( g_Var.Visuals.World.Removals[ 1 ] && *( int* ) iSmokeCount != 0 )
		*( int* ) iSmokeCount = 0;
}

void CMisc::StorePunch( CBaseEntity* pEntity )
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal && pLocal == pEntity )
	{
		int nTickBase = UserCmd::flFixedCurTime;
		sPunchRecord* pPunchData = &m_sPunchRecord[ nTickBase % 128 ];

		pPunchData->m_iTickBase = pEntity->GetTickBase();
		pPunchData->m_vStoredPunch = pEntity->GetPunchAngle();
		pPunchData->m_vStoredPunchVel = pEntity->GetPunchAngleVelocity();
		pPunchData->m_vStoredViewOffset = pEntity->GetViewOffset();
	}
}

void CMisc::FixUpPunch( CBaseEntity* pLocal )
{
	if( pLocal )
	{
		int nTickBase = UserCmd::flFixedCurTime;
		sPunchRecord* pPunchData = &m_sPunchRecord[ nTickBase % 128 ];

//		if( pPunchData->m_iTickBase != pLocal->GetTickBase() )
//			return;

		//check if the delta is in a good range , polak checks for 0.5? lol
		if( fabs( pPunchData->m_vStoredPunch.x - pLocal->GetPunchAngle().x ) <= 0.03125f
			&& fabs( pPunchData->m_vStoredPunch.y - pLocal->GetPunchAngle().y ) <= 0.03125f
			&&  fabs( pPunchData->m_vStoredPunch.z - pLocal->GetPunchAngle().z ) <= 0.03125f )
			*( Vector* ) ( ( DWORD ) pLocal + Offsets::Player::m_Local + Offsets::Player::m_aimPunchAngle ) = pPunchData->m_vStoredPunch;

		if( fabs( pPunchData->m_vStoredPunchVel.x - pLocal->GetPunchAngleVelocity().x ) <= 0.03125f
			&& fabs( pPunchData->m_vStoredPunchVel.y - pLocal->GetPunchAngleVelocity().y ) <= 0.03125f
			&&  fabs( pPunchData->m_vStoredPunchVel.z - pLocal->GetPunchAngleVelocity().z ) <= 0.03125f )
			*( Vector* ) ( ( DWORD ) pLocal + Offsets::Player::m_Local + Offsets::Player::m_aimPunchAngleVel ) = pPunchData->m_vStoredPunchVel;

		if( fabs( pPunchData->m_vStoredViewOffset.x - pLocal->GetViewOffset().x ) <= 0.03125f
			&& fabs( pPunchData->m_vStoredViewOffset.y - pLocal->GetViewOffset().y ) <= 0.03125f
			&&  fabs( pPunchData->m_vStoredViewOffset.z - pLocal->GetViewOffset().z ) <= 0.03125f)
			*( Vector* ) ( ( DWORD )pLocal + Offsets::Player::m_Local + Offsets::Player::m_vecViewOffset ) = pPunchData->m_vStoredViewOffset;
	}
}

template<class T>
static T* FindHudElement( const char* name )
{
	static auto pThis = *reinterpret_cast<DWORD**>( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "B9 ? ? ? ? A3 ? ? ? ? E8 ? ? ? ? 5D" ) + 1 );
	static auto FindElement = reinterpret_cast<DWORD( __thiscall* )( void*, const char* )>( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39" ) );

	return ( T* ) FindElement( pThis, name );
}

void CMisc::PreserverFeed( CBaseEntity* pLocal )
{
	static DWORD* dwDeathNotice = FindHudElement<DWORD>( "CCSGO_HudDeathNotice" );
	static void( __thiscall *ClearNotice )( DWORD ) = ( void( __thiscall* )( DWORD ) )g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "55 8B EC 83 EC 0C 53 56 8B 71 58" );

	static float flLastSpawn;
	float flSpawn = pLocal->GetSpawnTime();

	if( flLastSpawn != flSpawn )
	{
		flLastSpawn = flSpawn;

		dwDeathNotice = FindHudElement<DWORD>( "CCSGO_HudDeathNotice" );

		if( dwDeathNotice - 20 )
			ClearNotice( ( ( DWORD ) dwDeathNotice - 20 ) );

	//	g_pEvents->bRoundStart = false;
	}

	if( dwDeathNotice )
		*( float* ) ( ( DWORD ) dwDeathNotice + 0x50 ) = g_Var.Misc.ExtendKillfeed ? FLT_MAX : 1.5f;
}

void CMisc::HandleBuybot(CBaseEntity* pLocal)
{
	if (!pLocal || !pLocal->IsAlive())
		return;

	static auto flLastSpawnTime = 0.f;
	auto flSpawnTime = pLocal->GetSpawnTime();
	auto bSpawnTimeUpdated = (flSpawnTime != flLastSpawnTime);
	flLastSpawnTime = flSpawnTime;
	
	if (!bSpawnTimeUpdated)
		return;

	auto HasWeapon = [pLocal](ItemSchemaIndex_e Expected) -> bool
	{
		//auto hWeapons = pLocal->GetMyWeapons();			// this doesnt work for some reason? I'm too lazy to look
		UINT* hWeapons = (UINT*)((DWORD)pLocal + 0x2DE8);	// pasted from skinchanger
		if (!hWeapons)
			return false;

		for (int i = 0; hWeapons[i]; i++)
		{
			auto pWeapon = reinterpret_cast<CBaseCombatWeapon*>(g_pEntList->GetClientEntityFromHandle(hWeapons[i]));
			if (!pWeapon)
				continue;
			if (pWeapon->GetWeaponID() == Expected)
				return true;
		}

		return false;
	};

	// lets check if we can afford everything rn
	auto TotalCost = 0;
	auto TeamNumber = pLocal->GetTeamNumber();

	switch (g_Var.Misc.BuyBot.Primary)
	{
	case 1:
		if (!HasWeapon(WEAPON_AK47) && !HasWeapon(WEAPON_M4A1) && !HasWeapon(WEAPON_M4A1S))
			TotalCost += (TeamNumber == 2 ? 2700 : 3100); // M4A1/AK47
		break;
	case 2:
		if (!HasWeapon(WEAPON_AUG) && !HasWeapon(WEAPON_SG553))
			TotalCost += (TeamNumber == 2 ? 2750 : 3150); // Scoped ones
		break;
	case 3:
		if (!HasWeapon(WEAPON_SCAR20) && !HasWeapon(WEAPON_G3SG1))
			TotalCost += 5000; // SCAR
		break;
	case 4:
		if (!HasWeapon(WEAPON_SCOUT))
			TotalCost += 1700; // SCOUT
		break;
	case 5:
		if (!HasWeapon(WEAPON_AWP))
			TotalCost += 4750; // AWP
		break;
	}

	switch (g_Var.Misc.BuyBot.Secondary)
	{
	case 1:
		if (!HasWeapon(WEAPON_DUALS))
			TotalCost += 400; // Elites
		break;
	case 2:
		if (!HasWeapon(WEAPON_P250))
			TotalCost += 300; // P250
		break;
	case 3:
		if (!HasWeapon(WEAPON_DEAGLE) && !HasWeapon(WEAPON_R8REVOLVER))
			TotalCost += 700; // Revolver 600, deagle 700... We have no inventory api to check I guess
		break;
	}

	if (g_Var.Misc.BuyBot.Gear[0] && (pLocal->GetArmor() != 100 || !pLocal->HasHelm()))
		TotalCost += 1000; // KEVLAR+HELM

	if (g_Var.Misc.BuyBot.Gear[1] && TeamNumber == 3) // TODO: CHECK
		TotalCost += 400; // DEFKIT

	if (g_Var.Misc.BuyBot.Gear[2] && !HasWeapon(WEAPON_TASER))
		TotalCost += 200; // ZEUS

	if (g_Var.Misc.BuyBot.Grenades[0] && !HasWeapon(WEAPON_HE))
		TotalCost += 300; // HE

	if (g_Var.Misc.BuyBot.Grenades[1] && !HasWeapon(WEAPON_SMOKE))
		TotalCost += 300; // SMOKE

	if (g_Var.Misc.BuyBot.Grenades[2] && !HasWeapon(WEAPON_FLASH))
		TotalCost += 200; // FLASH

	if (g_Var.Misc.BuyBot.Grenades[3] && !HasWeapon(WEAPON_MOLOTOV) && !HasWeapon(WEAPON_INC))
		TotalCost += (TeamNumber == 2 ? 400 : 600); // MOLOTOV

	if (g_Var.Misc.BuyBot.Grenades[4] && !HasWeapon(WEAPON_DECOY))
		TotalCost += 50; // DECOY

	auto CurrentMoney = pLocal->GetCash();

	//g_pICvar->ConsoleColorPrintf(Color::LightBlue(), "Buybot:\nCost: %i\nHave: %i\n\n", TotalCost, CurrentMoney);

	if (CurrentMoney >= TotalCost)
	{
		switch (g_Var.Misc.BuyBot.Primary)
		{
		case 1:
			if (!HasWeapon(WEAPON_AK47) && !HasWeapon(WEAPON_M4A1) && !HasWeapon(WEAPON_M4A1S))
				g_pEngine->ExecuteClientCmd("buy ak47; buy m4a1"); 
			break;
		case 2:
			if (!HasWeapon(WEAPON_AUG) && !HasWeapon(WEAPON_SG553))
				g_pEngine->ExecuteClientCmd("buy sg556; buy aug"); 
			break;
		case 3:
			if (!HasWeapon(WEAPON_SCAR20) && !HasWeapon(WEAPON_G3SG1))
				g_pEngine->ExecuteClientCmd("buy scar20; buy g3sg1");
			break;
		case 4:
			if (!HasWeapon(WEAPON_SCOUT))
				g_pEngine->ExecuteClientCmd("buy ssg08"); 
			break;
		case 5:
			if (!HasWeapon(WEAPON_AWP))
				g_pEngine->ExecuteClientCmd("buy awp"); 
			break;
		}

		switch (g_Var.Misc.BuyBot.Secondary)
		{
		case 1:
			if (!HasWeapon(WEAPON_DUALS))
				g_pEngine->ExecuteClientCmd("buy elite"); 
			break;
		case 2:
			if (!HasWeapon(WEAPON_P250))
				g_pEngine->ExecuteClientCmd("buy p250"); 
			break;
		case 3:
			if (!HasWeapon(WEAPON_DEAGLE) && !HasWeapon(WEAPON_R8REVOLVER))
				g_pEngine->ExecuteClientCmd("buy deagle"); 
			break;
		}

		if (g_Var.Misc.BuyBot.Gear[0] && (pLocal->GetArmor() != 100 || !pLocal->HasHelm()))
			g_pEngine->ExecuteClientCmd("buy vesthelm; buy vest");

		if (g_Var.Misc.BuyBot.Gear[1] && TeamNumber == 3)
			g_pEngine->ExecuteClientCmd("buy defuser");

		if (g_Var.Misc.BuyBot.Gear[2] && !HasWeapon(WEAPON_TASER))
			g_pEngine->ExecuteClientCmd("buy taser 34");

		if (g_Var.Misc.BuyBot.Grenades[0] && !HasWeapon(WEAPON_HE))
			g_pEngine->ExecuteClientCmd("buy hegrenade");

		if (g_Var.Misc.BuyBot.Grenades[1] && !HasWeapon(WEAPON_SMOKE))
			g_pEngine->ExecuteClientCmd("buy smokegrenade");

		if (g_Var.Misc.BuyBot.Grenades[2] && !HasWeapon(WEAPON_FLASH))
			g_pEngine->ExecuteClientCmd("buy flashbang");

		if (g_Var.Misc.BuyBot.Grenades[3] && !HasWeapon(WEAPON_MOLOTOV) && !HasWeapon(WEAPON_INC))
			g_pEngine->ExecuteClientCmd("buy incgrenade; buy molotov");

		if (g_Var.Misc.BuyBot.Grenades[4] && !HasWeapon(WEAPON_DECOY))
			g_pEngine->ExecuteClientCmd("buy decoy");
	}
}

void CMisc::DoRagdoll()
{
	static ConVar* cl_ragdoll_gravity = g_pICvar->FindVar( "cl_ragdoll_gravity" );
	static ConVar* cl_phys_timescale = g_pICvar->FindVar( "cl_phys_timescale" );

	switch( g_Var.Visuals.World.Ragdoll )
	{
	case 0:
		if( cl_ragdoll_gravity->GetInt() != 600 )
			cl_ragdoll_gravity->SetValue( 600 );

		if( cl_phys_timescale->GetFloat() != 1.f )
			cl_phys_timescale->SetValue( 1.f );
		break;
	case 1:
		if( cl_ragdoll_gravity->GetInt() != -200 )
			cl_ragdoll_gravity->SetValue( -200 );
		break;
	case 2:
		if( cl_ragdoll_gravity->GetInt() != 5 )
			cl_ragdoll_gravity->SetValue( 5 );

		if( cl_phys_timescale->GetFloat() != 10.f )
			cl_phys_timescale->SetValue( 10.f );
	}
}

//at the end of cm we make a update record
//angle is angle from cmd
void CMisc::UpdateLocal(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd)
{
	bool bHasShot = false;
	Vector vShotAngle = Vector( 0.f, 0.f, 0.f );

	if (!bHasShot && ( pCmd->buttons & IN_ATTACK ) && g_Misc.CanShoot(pLocal, pWeapon))
	{
		bHasShot = true;
		vShotAngle = pCmd->viewangles;
	}

	sLocalData LocalData;

	LocalData.m_flAnimTime = pLocal->GetTickBase() * g_pGlobalVars->interval_per_tick;
	LocalData.m_flPitch = bHasShot ? vShotAngle.x : pCmd->viewangles.x;
	LocalData.m_flYaw = bHasShot ? vShotAngle.y : pCmd->viewangles.y;
	LocalData.m_vecVelocity = pLocal->GetVelocity();
	LocalData.m_flDuckAmount = pLocal->DuckAmount();
	LocalData.m_fFlags = pLocal->GetFlags();

	UserCmd::vLocalRecord.push_back(LocalData);
}

void UpdateAnimState(CAnimState* state, Vector ang)
{
	using fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto ret = reinterpret_cast<fn>(g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "56 6A 01 68 ? ? ? ? 8B F1"));

	if (!ret)
		return;

	ret(state, NULL, NULL, ang.y, ang.x, NULL);
}

void CMisc::UpdateLocalAnimations(CBaseEntity* pLocal)
{
	{
		//pretty much do the stuff above in cm 
		float flCurTime = g_pGlobalVars->curtime;
		float flFrameTime = g_pGlobalVars->frametime;

		CAnimationLayer cLayer[13];
		std::memcpy(cLayer, pLocal->GetAnimOverlay(), (sizeof(CAnimationLayer) * pLocal->GetNumAnimOverlays()));

		int fFlags = pLocal->GetFlags();
		Vector vOrigin = pLocal->GetAbsOrigin();
		Vector vViewAngle = *(Vector*)((DWORD)pLocal + Offsets::Player::deathflag);
		float  flDuckAmount = pLocal->DuckAmount();

		g_pGlobalVars->curtime = pLocal->GetSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;

		pLocal->SetAbsVelocity(pLocal->GetVelocity());

		pLocal->SetAbsAngles(QAngle(0.f, pLocal->GetAnimState()->m_flGoalFeetYaw, 0.f));

		if (pLocal->GetAnimState()->m_velocity > 0.1f)
			pLocal->GetAnimState()->m_flGoalFeetYaw = UserCmd::vAntiAimReal.y;

/*		if (g_AntiAim.m_bInSlowWalk && UserCmd::iOriginalFlags & FL_ONGROUND)
		{
			cLayer[6].m_flWeight = 0.f;
			cLayer[12].m_flWeight = 0.f;
		}*/

		//visual effect, makes heels on local player not go full retard when u start moving
		pLocal->GetAnimState()->m_flFeetYawRate = 0.f;

		if ((UserCmd::iOriginalFlags & FL_ONGROUND))
			*(float*)((DWORD)pLocal->GetAnimState() + 0x110) = 0.0f;//time since in air

		*(bool*)((DWORD)pLocal + Offsets::Player::m_bClientSideAnimation) = true;
		pLocal->UpdateAnimation();
		*(bool*)((DWORD)pLocal + Offsets::Player::m_bClientSideAnimation) = false;
		
	//	pLocal->SetAbsOrigin(vOrigin);
		*(int*)((DWORD)pLocal + Offsets::Player::m_fFlags) = fFlags;
		*(Vector*)((DWORD)pLocal + Offsets::Player::deathflag) = vViewAngle;
		*(float*)((DWORD)pLocal + Offsets::Player::m_flDuckAmount) = flDuckAmount;

		std::memcpy(pLocal->GetAnimOverlay(), cLayer, (sizeof(CAnimationLayer) * pLocal->GetNumAnimOverlays()));

		g_pGlobalVars->curtime = flCurTime;
		g_pGlobalVars->frametime = flFrameTime;
	}
}

void CMisc::FixPVS( CBaseEntity* pLocal )
{
//
}

void CMisc::StrafeBot( CUserCmd* pUserCmd, CBaseEntity* pLocal )
{

}


void CMisc::Movement( CBaseEntity* pLocal, CUserCmd* pCmd )
{
	if(pLocal->GetMoveType() != 8 && pLocal->GetMoveType() != 9)
	{
		if( g_Var.Misc.CrouchJump && !( pLocal->GetFlags() & FL_ONGROUND ) )
			pCmd->buttons |= IN_DUCK;

		if( pCmd->buttons & IN_JUMP )
		{
			if( g_Var.Misc.AutoJump && !( pLocal->GetFlags() & FL_ONGROUND ) )
				pCmd->buttons &= ~IN_JUMP;

			if (g_Var.Misc.AutoStrafe)
			{
				if (g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_A) ||
					g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_D) ||
					g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_S) ||
					g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_W) ||
					g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_Q) ||
					g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_Z) ||
					g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_LSHIFT))
					return;

				if (pLocal->GetVelocity().Length2D() == 0 && (pCmd->forwardmove == 0 && pCmd->sidemove == 0))
					pCmd->forwardmove = 450.f;
				else
					if (pCmd->forwardmove == 0 && pCmd->sidemove == 0)
					{
						if (pCmd->mousedx > 0 || pCmd->mousedx < -0)
							pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
						else
						{
							ConVar* sv_airaccelerate = g_pICvar->FindVar("sv_airaccelerate");
							ConVar* sv_maxspeed = g_pICvar->FindVar("sv_maxspeed");
							static int iZValue = 0;
							static int iTimer = 0;

							// z strafe
							if (false)
							{
								if (iZValue == 0)
									iZValue = -1;

								if (iTimer > 15)
								{
									iTimer = -1;
									iZValue *= -1;
								}
								iTimer++;
							}
							else
								iZValue = 0;

							double flYawRadius = UserCmd::vOriginalView.y * M_PI / 180;
							float flSpeed = sv_maxspeed->GetFloat();

							if (pLocal->GetFlags() & IN_DUCK)
								flSpeed *= 0.333;

							double flTau = g_pGlobalVars->interval_per_tick, flMA = flSpeed * sv_airaccelerate->GetFloat();
							int iStart = 0, iFinale = 0;
							double flVelocity[3] = { pLocal->GetVelocity().x, pLocal->GetVelocity().y, pLocal->GetVelocity().z };
							double flPos[2] = { 0, 0 };
							double flDir[2] = { std::cos((UserCmd::vOriginalView.y + 10 * iZValue) * M_PI / 180), std::sin((UserCmd::vOriginalView.y + 10 * iZValue) * M_PI / 180) };
							static AutoStrafer Strafer;
							Strafer.strafe_line_opt(flYawRadius, iStart, iFinale, flVelocity, flPos, 30.0, flTau, flMA, flPos, flDir);

							Vector vFinale = Vector(0, flYawRadius * 180 / M_PI, 0);
							g_Math.NormalizeVector(vFinale);
							UserCmd::vOriginalView.y = vFinale.y;
							pCmd->sidemove = iStart * 450;
						}
					}
			}
		}

		DoCounterStrafe(pLocal, pCmd);
	}
}

void CMisc::FixTickbase( CUserCmd* pCmd, CBaseEntity* pLocal )
{
	static CUserCmd* pLastUCmd = nullptr;
	static int iTickBase = 0;

	if( !pLastUCmd || !pLocal )
	{
		pLastUCmd = pCmd;
		return;
	}

	if( pLastUCmd->hasbeenpredicted )
		iTickBase = pLocal->GetTickBase();
	else
		++iTickBase;

	pLastUCmd = pCmd;

	UserCmd::flFixedCurTime = iTickBase * g_pGlobalVars->interval_per_tick;
}

#define square( x ) ( x * x )

void CMisc::ClampWalk(CUserCmd* pCmd, float flSpeed)
{
	if (flSpeed <= 0.f)
		return;

	float flMin = (float)(FastSqrt(square(pCmd->forwardmove) + square(pCmd->sidemove) + square(pCmd->upmove)));

	if (flMin <= 0.f)
		return;

	if (pCmd->buttons & IN_DUCK)
		flSpeed *= 2.94117647f;

	if (flMin <= flSpeed)
		return;

	float flNegate = flSpeed / flMin;

	pCmd->forwardmove *= flNegate;
	pCmd->sidemove *= flNegate;
	pCmd->upmove *= flNegate;
}

void CMisc::DoCounterStrafe( CBaseEntity* pLocal, CUserCmd* pCmd )
{
	if( ( UserCmd::iOriginalFlags & FL_ONGROUND ) && 
		!( g_pInputSystem->IsButtonDown( ButtonCode_t::KEY_A ) ||
			g_pInputSystem->IsButtonDown( ButtonCode_t::KEY_D ) ||
			g_pInputSystem->IsButtonDown( ButtonCode_t::KEY_S ) ||
			g_pInputSystem->IsButtonDown( ButtonCode_t::KEY_W ) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_Q) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_Z) ||
			g_pInputSystem->IsButtonDown(ButtonCode_t::KEY_SPACE) ||
			pCmd->buttons & IN_JUMP) )
	{
		Vector vVel = pLocal->GetVelocity();
		float flSpeed = vVel.Length();

		pCmd->sidemove = 0;
		pCmd->forwardmove = flSpeed > 20.f ? 450.f : 0.f;

		g_Math.RotateMovement(pCmd, g_Math.CalcAngle(Vector(0, 0, 0), vVel).y + 180.f);
	}
}

void CMisc::DoThirdperson( CBaseEntity* pLocal )
{
	static bool bThirdPerson;

	if( g_Var.Misc.Thirdperson && g_Misc.IsKeyPressed( g_Var.Misc.ThirdpersonToggleKey ) )
		bThirdPerson = !bThirdPerson;

	int* iObserver = ( int* ) ( ( DWORD ) pLocal + Offsets::Player::m_iObserverMode );

	if( pLocal->GetHealth() <= 0 )
	{
		*reinterpret_cast< bool* >( uintptr_t( g_pInput ) + 0xAD ) = false;

		if( bThirdPerson )
			*iObserver = 5;

		return;
	}

	*iObserver = 0;

	auto GetCorrectDistance = [ &pLocal ] ( float flDistance ) -> float
	{
		Vector vInverse;
		g_pEngine->GetViewAngles( vInverse );

		vInverse.x *= -1.f, vInverse.y += 180.f;

		Vector vDirection;
		g_Math.AngleVector( vInverse, vDirection );

		CEngineTraceClient::CTraceWorldOnly TraceFilter;
		CEngineTraceClient::trace_t Trace;
		CEngineTraceClient::Ray_t Ray;

		Ray.Init( pLocal->GetEyePosition(), ( pLocal->GetEyePosition() ) + ( vDirection * flDistance ) );
		g_pEngineTrace->TraceRay( Ray, MASK_ALL, &TraceFilter, &Trace );

		return ( flDistance * Trace.fraction ) - 10.f;
	};

	*reinterpret_cast<bool*>( uintptr_t( g_pInput ) + 0xAD) = bThirdPerson;

	Vector vView;
	g_pEngine->GetViewAngles( vView );

	vView.z = GetCorrectDistance( g_Var.Misc.ThirdpersonDist );
	*reinterpret_cast<Vector*>( uintptr_t( g_pInput ) + 0xB0) = vView; // B0
	//printf("g_pInput: 0x%p\n", g_pInput);

/*	static bool bFlip = false;

	static bool bThirdPerson;

	if( g_Misc.IsKeyPressed( g_Var.Misc.ThirdpersonToggleKey ) )
		bThirdPerson = !bThirdPerson;

	if( g_Var.Misc.Thirdperson && bThirdPerson && pLocal->GetLifeState() == LIFE_ALIVE )
	{
		if( !bFlip )
		{
			bFlip = true;
			g_pInput->CAM_ToThirdPerson();
		}
	}
	else
		if( bFlip )
		{
			bFlip = false;
			g_pInput->CAM_ToFirstPerson();
		}*/
}

void CMisc::ThirdPersonAngle( CBaseEntity* pLocal )
{
	CAnimState* pAnimState = pLocal->GetAnimState();

	static int iGroundTicks = 0;
	float flPitch;

	if( ( UserCmd::iOriginalFlags & FL_ONGROUND ) )
		iGroundTicks++;
	else
		iGroundTicks = 0;

	if( pAnimState && pAnimState->m_bInHitGroundAnimation && !( UserCmd::pCmd->buttons & IN_JUMP ) && iGroundTicks < 600 )
		flPitch = -10.f;
	else
		flPitch = pLocal->GetEyeAngles().x;

	if( *( bool* ) ( ( DWORD ) g_pInput + 0xAD ) )
	{
		*( Vector* ) ( ( DWORD ) pLocal + Offsets::Player::deathflag ) = Vector( flPitch, UserCmd::vAntiAimReal.y, 0.f );
	//	g_pPred->SetLocalViewAngles( QAngle( flPitch, UserCmd::vAntiAimReal.y, 0.f ) );
		pLocal->SetAbsAngles( QAngle( 0.f, pAnimState->m_flGoalFeetYaw, 0.f ) );
	}
}

void CMisc::DoKnifeBot( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd *pCmd )
{
	bInKnife = false;

	if( !pWeapon->IsKnife() )
		return;

	auto IsBehind = [](CBaseEntity* pLocal, CBaseEntity* pEnemy)
	{
		Vector vecLOS = pEnemy->GetAbsOrigin() - pLocal->GetAbsOrigin();
		vecLOS.z = 0.0f;
		vecLOS.Normalized();

		Vector vTargetForward;
		g_Math.AngleVector(Vector(pEnemy->GetAbsAngles().x, pEnemy->GetAbsAngles().y, pEnemy->GetAbsAngles().z), vTargetForward);
		vTargetForward.z = 0.0f;

		// CSS:0.8, CSGO:0.475
		return vecLOS.Dot(vTargetForward) > 0.475;
	};

	for( int y = 0; y <= 360; y += 360.f / 6.f ) 
	{
		for( int x = -89; x <= 89; x += 179.f / 6.f ) 
		{
			Vector vAngle = Vector( x, y, 0.f );

			// attack range of knife attack
			float range = true ? 32.0f : 48.0f;

			// calc end spot
			Vector vecAim;
			g_Math.AngleVector(vAngle, vecAim);
			Vector end = pLocal->GetEyePosition() + vecAim * range;

			CEngineTraceClient::trace_t tr;
			CEngineTraceClient::Ray_t ray;
			CEngineTraceClient::CTraceFilter filter;
			filter.pSkip = pLocal;

			ray.Init(pLocal->GetEyePosition(), end);
			g_pEngineTrace->TraceRay( ray, MASK_SHOT, &filter, &tr);

			if (tr.fraction >= 1.0f)
			{
				static const Vector hull[] = { Vector(-16.0f, -16.0f, -18.0f), Vector(16.0f, 16.0f, 18.0f) };
				ray.Init(pLocal->GetEyePosition(), end, hull[0], hull[1]);
				g_pEngineTrace->TraceRay(ray, 0x200400B, &filter, &tr);
			}

			if( !tr.m_pEnt->PlayerCheck() )
				continue;

			if(tr.m_pEnt->GetTeamNumber() == pLocal->GetTeamNumber() )
				continue;

			struct table_t 
			{
				// [first][armor][back]
				unsigned char swing[2][2][2];
				// [armor][back]
				unsigned char stab[2][2];
			};

			static const table_t table = {
				{ { {25,90}, {21,76} }, { {40,90}, {34,76} } },
				{ {65,180}, {55,153} }
			};

			bool armor = (tr.m_pEnt->GetArmor() > 0);
			bool first = pWeapon->GetNextPrimaryAttack() + 0.4f < UserCmd::flFixedCurTime;
			bool back = IsBehind(pLocal, tr.m_pEnt);
			int stab_dmg = table.stab[armor][back];
			int slash_dmg = table.swing[false][armor][back];
			int swing_dmg = table.swing[first][armor][back];
			int health = tr.m_pEnt->GetHealth();
			bool stab;

			// IF self dies in one stab AND target will not die in one attack, do a stab <- TODO

			// IF health lower than swing_dmg, do a swing
			if (health <= swing_dmg) stab = false;
			// IF health lower than stab_dmg, do a stab
			else if (health <= stab_dmg) stab = true;
			// IF health more than swing+swing+stab, do a stab
			else if (health > (swing_dmg + slash_dmg + stab_dmg)) stab = true;
			// ELSE swing (initiate swing+swing+stab)
			else stab = false;

			bInKnife = true;

			pCmd->viewangles = Vector(vAngle.x, vAngle.y, 0 );
			pCmd->buttons |= stab ? IN_ATTACK2 : IN_ATTACK;;

			return;
		}
	}
}

void CMisc::ClanTag(CBaseEntity * pLocal)
{
/*	int __fastcall sub_100886E0( int a1, int a2 ) <-
	{
		int v2; // ebx@1
		int v3; // edi@1
		int v4; // eax@1
		int v5; // esi@2

		v2 = a2;
		v3 = a1;
		v4 = KeyValuesSystem();
		if( ( *( int( __thiscall ** )( int, signed int ) )( *( _DWORD * ) v4 + 4 ) )( v4, 36 ) )
			v5 = sub_102DFF30( "ClanTagChanged" );
		else
			v5 = 0;
		if( sub_102E0C30( "tag", 1 ) )
			sub_102E1590( v3 );
		if( sub_102E0C30( "name", 1 ) )
			sub_102E1590( v2 );
		return ( *( int( __thiscall ** )( void **, int ) )( ( int( __thiscall ** )( _DWORD, _DWORD ) )off_10586E18 + 187 ) )(
			&off_10586E18,
			v5 );
	}*/

	static auto SetClantag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(g_Pattern.PatternSearchIDA("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	static auto BaseString = std::string(" KIDUAHOOK                  ");

	static auto WasEnabled = g_Var.Misc.Clantag;
	auto IsEnabled = g_Var.Misc.Clantag;
	auto JustDisabled = !IsEnabled && WasEnabled;
	WasEnabled = IsEnabled;

	if (!IsEnabled)
	{
		if (JustDisabled)
			SetClantag("", "");
		return;
	}

	const auto StepTime = 0.257575;
	const auto StaticTicks = 8;

	// Calculate clantag position
	auto CurTime = g_pGlobalVars->curtime;
	if (pLocal && pLocal->GetLifeState() == LIFE_ALIVE)
		CurTime = UserCmd::flFixedCurTime;
	auto AnimStep = static_cast<int>(fmod(CurTime + 1.252575, StepTime * (BaseString.length() + StaticTicks)) / StepTime);
	AnimStep = max(0, AnimStep - StaticTicks);

	// Only change stuff when our clantag animation step has moved
	static auto LastAnimStep = -1;
	if (AnimStep == LastAnimStep)
		return;
	LastAnimStep = AnimStep;
	
	// Rotate it as much as we want for this tick
	auto CurrentString = BaseString;
	for (auto i = 0; i < AnimStep; i++)
		std::rotate(CurrentString.begin(), CurrentString.begin() + 1, CurrentString.end());

	CurrentString = CurrentString.substr(0, 15);
	SetClantag(CurrentString.c_str(), CurrentString.c_str());
}

void CMisc::NameChange()
{
	if( g_pEngine->IsInGame() )
	{
		NET_SetConVar pConvar( "name", "nigger" );
		g_pEngine->GetNetChannelInfo()->SendNetMsg( pConvar );
	}
}

char* cHeadShot[ 6 ]
{ "1", "1tap laff", "1 nerd", "tap", "tapped", "u pay for that?" };

char* cHeadShotUs[ 6 ]
{ "lucky dog", "i pressed e", "tryhard", "finally hit my head", "one way", "rage pingspike" };

char* cBodyShot[ 6 ]
{ "owned", "refund", "shamed nn", "sit dog", "do you sell?", "nice cheat" };

char* cBodyShotUs[ 6 ]
{ "baim", "nice resolver hhh", "wanna try to hit head next time ?", "baim 1way lmao", "nice config baim only", "head glitch dog" };

void CMisc::FlameEnemy( int iEnemy, int iLocal, int iUser, bool bHitHead )
{
	char cBuffer[ 2048 ];
	int iRand = ( rand() % 5 ) + 1;
	//we got killed
	if( iEnemy == iLocal && iUser != iLocal )
	{
		sprintf( cBuffer, "say %s", bHitHead ? cHeadShot[ iRand ] : cBodyShot[ iRand ] );

		if( cBuffer )
			g_pEngine->ExecuteClientCmd( cBuffer );
	}
	
	if( iEnemy != iLocal && iUser == iLocal )
	{
		sprintf( cBuffer, "say %s", bHitHead ? cHeadShotUs[ iRand ] : cBodyShotUs[ iRand ] );

		if( cBuffer )
			g_pEngine->ExecuteClientCmd( cBuffer );
	}
}

int CMisc::GetFPS()
{
	return static_cast< int >( 1.f / g_pGlobalVars->frametime );

/*static float m_Framerate = 0;
	m_Framerate = 0.9 * m_Framerate + (1.0 - 0.9) * INTERFACES::Globals->absoluteframetime; // https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/vgui_netgraphpanel.cpp#L744
 
	return int(1.f / m_Framerate);*/
}

bool CMisc::SetupBones(matrix3x4_t* matrix, CBaseEntity* pEntity )
{
	return true;
}

bool CMisc::HandleBoneSetup( CBaseEntity * pEnemy, int iBoneMask, matrix3x4_t * MatrixOut )
{
	return true;
}

void CMisc::SimulateMovement( Vector &velocity, Vector &origin, CBaseEntity *player, int &flags, bool was_in_air )
{
}

std::string CMisc::GetWeaponNameByID( int iIndex )
{
	switch( iIndex )
	{
	case WEAPON_DEAGLE:
		return "Deagle"; break;
	case WEAPON_DUALS:
		return "Duals"; break;
	case WEAPON_FIVE7:
		return "Five seven"; break;
	case WEAPON_GLOCK:
		return "Glock"; break;
	case WEAPON_AK47:
		return "Ak47"; break;
	case WEAPON_AUG:
		return "Aug"; break;
	case WEAPON_AWP:
		return "Awp"; break;
	case WEAPON_FAMAS:
		return "Famas"; break;
	case WEAPON_G3SG1:
		return "G3sg1"; break;
	case WEAPON_GALIL:
		return "Galil"; break;
	case WEAPON_M249:
		return "M249"; break;
	case WEAPON_M4A1:
		return "M4a1"; break;
	case WEAPON_MAC10:
		return "Mac10"; break;
	case WEAPON_P90:
		return "P90"; break;
	case WEAPON_MP9:
		return "Mp9"; break;
	case WEAPON_UMP45:
		return "Ump45"; break;
	case WEAPON_XM1014:
		return "Xm1014"; break;
	case WEAPON_BIZON:
		return "Bizon"; break;
	case WEAPON_MP5:
		return "Mp5"; break;
	case WEAPON_MAG7:
		return "Mag7"; break;
	case WEAPON_NEGEV:
		return "Negev"; break;
	case WEAPON_SAWEDOFF:
		return "Sawed off"; break;
	case WEAPON_TEC9:
		return "Tec9"; break;
	case WEAPON_TASER:
		return "Taser"; break;
	case WEAPON_P2000:
		return "P2000"; break;
	case WEAPON_MP7:
		return "Mp7"; break;
	case WEAPON_NOVA:
		return "Nova"; break;
	case WEAPON_P250:
		return "P250"; break;
	case WEAPON_SCAR20:
		return "Scar20"; break;
	case WEAPON_SG553:
		return "Sg553"; break;
	case WEAPON_SCOUT:
		return "Scout"; break;
	case WEAPON_R8REVOLVER:
		return "Revolver"; break;
	case WEAPON_C4:
		return "C4"; break;
	case WEAPON_KNIFE:
		return "Ct_knife"; break;
	case WEAPON_KNIFE_T:
		return "T_knife"; break;
	case WEAPON_KNIFE_GUT:
		return "Gut_knife"; break;
	case WEAPON_KNIFE_FLIP:
		return "Flip_knife"; break;
	case WEAPON_KNIFE_BAYONET:
		return "Bayonet"; break;
	case WEAPON_KNIFE_KARAMBIT:
		return "Karambit"; break;
	case WEAPON_KNIFE_M9BAYONET:
		return "M9_bayonet"; break;
	case WEAPON_KNIFE_BUTTERFLY:
		return "Butterfly"; break;
	case WEAPON_KNIFE_FALCHION:
		return "Falchion"; break;
	case WEAPON_KNIFE_SHADOWDAGGER:
		return "Dagger"; break;
	case WEAPON_KNIFE_BOWIE:
		return "Bowie"; break;
	case WEAPON_KNIFE_HUNTSMAN:
		return "Huntsman"; break;
	case WEAPON_FLASH:
		return "Flashbang"; break;
	case WEAPON_HE:
		return "He grenade"; break;
	case WEAPON_SMOKE:
		return "Smoke"; break;
	case WEAPON_MOLOTOV:
		return "Molotov"; break;
	case WEAPON_DECOY:
		return "Decoy"; break;
	case WEAPON_INC:
		return "Inc grenade"; break;
	case WEAPON_M4A1S:
		return "M4a1-s"; break;
	case WEAPON_USPS:
		return "Usp-s"; break;
	case WEAPON_CZ75:
		return "Cz75"; break;
	default:
		return ""; break;
	}
	return "";
}