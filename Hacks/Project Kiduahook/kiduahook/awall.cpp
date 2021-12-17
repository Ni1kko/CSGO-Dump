#include "awall.h"

CAutowall g_Autowall;

bool TraceDidHitWorld( CEngineTraceClient::trace_t* pTrace )
{
	return ( reinterpret_cast< CBaseEntity* >( pTrace->m_pEnt ) == g_pEntList->GetClientEntity( 0 ) );
}

bool TraceDidHitNonWorldEntity( CEngineTraceClient::trace_t* pTrace )
{
	if( !pTrace )
		return false;

	if( pTrace->m_pEnt == nullptr )
		return false;

	if( TraceDidHitWorld( pTrace ) )
		return false;

	return true;
}

void TraceLine( const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, CEngineTraceClient::trace_t* ptr )
{
	CEngineTraceClient::Ray_t ray;
	ray.Init( vecAbsStart, vecAbsEnd );
	CEngineTraceClient::CTraceFilter filter;
	filter.pSkip = ignore;

	g_pEngineTrace->TraceRay( ray, mask, &filter, ptr );
}

void UTIL_ClipTraceToPlayers( const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CEngineTraceClient::ITraceFilter* filter, CEngineTraceClient::trace_t* tr )
{
	static DWORD dwAddress = g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "8B 43 10 56 57 52" ) - 28;
	
	if( !dwAddress )
		return;

	_asm
	{
		MOV EAX, filter
		LEA ECX, tr
		PUSH ECX
		PUSH EAX
		PUSH mask
		LEA EDX, vecAbsEnd
		LEA ECX, vecAbsStart
		CALL dwAddress
		ADD ESP, 0xC
	}
}

bool IsBreakAble( CBaseEntity* pEntity )
{
	if( !pEntity || pEntity->GetIndex() == 0 )
		return false;

	bool bBreakAble = ( ( bool( __thiscall* )( CBaseEntity* ) ) g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "55 8B EC 51 56 8B F1 85 F6 74 68" ) )( pEntity );

	if( !bBreakAble )
	{
		CClientClasses* pClientClass = pEntity->GetClientClasses();

		if( !pClientClass )
			return false;

		char* cName = pClientClass->m_cName;

		if( *( UINT* )( cName ) == 0x65724243 && *( UINT* )( cName + 0x7 ) == 0x53656C62 )
			return true;
	}

	return bBreakAble;
}

void ScaleDamage( int iHitgroup, CBaseEntity *pEnemy, float flArmorRatio, float &flCurDamage )
{
	switch( iHitgroup )
	{
	case HITGROUP_HEAD:
		flCurDamage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		flCurDamage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		flCurDamage *= 0.75f;
		break;
	default:
		flCurDamage *= 1.f;
		break;
	}

	float flArmor = float( pEnemy->GetArmor() );

	if( flArmor > 0.0f && iHitgroup < HITGROUP_LEFTLEG )
	{
		if( iHitgroup == HITGROUP_HEAD && !pEnemy->HasHelm() )
			return;

		float flArmorscaled = ( flArmorRatio * 0.5f ) * flCurDamage;

		if( ( flCurDamage - flArmorscaled ) * 0.5f > flArmor )
			flArmorscaled = flCurDamage - ( flArmor * 2.0f );

		flCurDamage = flArmorscaled;
	}
}

bool CAutowall::TraceToExit( Vector& vEnd, CEngineTraceClient::trace_t* tEnter, Vector vStart, Vector vDir, CEngineTraceClient::trace_t* tExit )
{
	float flDist = 0.0f;

	while( flDist <= 90.0f )
	{
		flDist += 4.0f;
		vEnd = vStart + vDir * flDist;

		int iPointContent = g_pEngineTrace->GetPointContents( vEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL );

		if( iPointContent & MASK_SHOT_HULL && !( iPointContent & CONTENTS_HITBOX ) )
			continue;

		Vector vNewEnd = vEnd - ( vDir * 4.0f );

		TraceLine( vEnd, vNewEnd, 0x4600400B, ( CBaseEntity* ) 0, tExit );

		if( tExit->startSolid && tExit->surface.flags & 0x8000 )
		{
			TraceLine( vEnd, vStart, 0x600400B, tExit->m_pEnt, tExit );

			if( ( tExit->fraction < 1.0f || tExit->allSolid ) && !tExit->startSolid )
			{
				vEnd = tExit->endpos;
				return true;
			}
			continue;
		}

		if( !tExit->DidHit() || tExit->startSolid )
		{
			if( tExit->m_pEnt )
			{
				if( TraceDidHitNonWorldEntity( tEnter ) )
				{
					*tExit = *tEnter;
					tExit->endpos = vStart + vDir;
					return true;
				}
			}
			continue;
		}

		if( ( ( tExit->surface.flags >> 7 ) & 1 ) && !( ( tEnter->surface.flags >> 7 ) & 1 ) )
			continue;

		if( tExit->plane.normal.Dot( vDir ) <= 1.0f )
		{
			vEnd = vEnd - ( vDir * ( tExit->fraction * 4.0f ) );
			return true;
		}
	}
	return false;
}

void CAutowall::ClipTraceToPlayer( Vector vStart, Vector vEnd, int mask, CEngineTraceClient::trace_t* TraceOld, CBaseEntity* pEntity )
{
	Vector v3 = *( Vector* ) ( ( DWORD ) pEntity + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins );
	Vector v4 = *( Vector* ) ( ( DWORD ) pEntity + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs );

	Vector v9( vEnd - vStart );
	v9.Normalize();

	Vector v7 = ( v4 + v3 ) / 2;
	Vector v12( v7 + pEntity->GetOrigin() );

	Vector v15 = v12 - vStart;
	float v11 = v9.Dot( v15 );
	float v13 = 0.f;

	if( v11 < 0.f )
		v13 = -v15.Length();

	else if( v11 > v9.Length() )
		v13 = -( v12 - vEnd ).Length();

	else
	{
		Vector v6( v12 - ( v9 * v13 + vStart ) );
		v13 = v6.Length();
	}

	if( v13 <= 60.f )
	{
		CEngineTraceClient::Ray_t TraceRay;
		CEngineTraceClient::trace_t Trace;

		TraceRay.Init( vStart, vEnd );

		g_pEngineTrace->ClipRayToEntity( TraceRay, mask, pEntity, &Trace );

		if( TraceOld->fraction > Trace.fraction )
			*TraceOld = Trace;
	}
}

bool CAutowall::HandleBulletPenetration( CWeaponData *wpn_data, FireBulletData &data )
{
	surfacedata_t *enter_surface_data = g_pPhysics->GetSurfaceData( data.enter_trace.surface.surfaceProps );
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	bool enter_contents = data.enter_trace.contents >> 3 & 1;
	bool enter_flags = data.enter_trace.surface.flags >> 7 & 1;

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow( wpn_data->range_modifier, data.trace_length * 0.002f );

	if( ( data.trace_length > 3000.f ) || ( enter_surf_penetration_mod < 0.1f ) )
		data.penetrate_count = 0;

	if( data.penetrate_count <= 0 )
		return false;

	Vector dummy;
	CEngineTraceClient::trace_t trace_exit;

	if( !TraceToExit( dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit ) )
	{
	    if( !( g_pEngineTrace->GetPointContents( data.enter_trace.endpos, MASK_SHOT_HULL, nullptr ) & MASK_SHOT_HULL ) )
			return false;
	}

	surfacedata_t *exit_surface_data = g_pPhysics->GetSurfaceData( trace_exit.surface.surfaceProps );
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier;
	float combined_penetration_modifier;

	if( ( enter_material == 89 ) || ( enter_material == 71 ) )
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.050000001f;
	}
	else
		if( enter_contents || enter_flags )
		{
			combined_penetration_modifier = 1.f;
			final_damage_modifier = 0.16f;
		}
		else
		{
			final_damage_modifier = 0.16f;
			combined_penetration_modifier = ( enter_surf_penetration_mod + exit_surf_penetration_mod ) * 0.5f;
		}

	if( enter_material == exit_material )
	{
		if( exit_material == 87 || exit_material == 85 )
			combined_penetration_modifier = 3.0f;
		else
			if( exit_material == 76 )
				combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf( 0.f, 1.0f / combined_penetration_modifier );
	float v39 = ( trace_exit.endpos - data.enter_trace.endpos ).Length();
	float v40 = v34 * 3.f * fmaxf( 0.f, 3.f / wpn_data->penetration * 1.25f ) + data.current_damage * final_damage_modifier + v39 * v39 * v34 / 24.f;

	data.current_damage -= fmaxf( 0.f, v40 );

	if( data.current_damage < 1.f )
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool CAutowall::SimulateFireBullet( CBaseEntity *local, CBaseEntity *player, CBaseCombatWeapon *weapon, FireBulletData &data, int spot )
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;

	CWeaponData *wpn_data = weapon->GetWeaponData();

	if( !wpn_data )
		return false;

	data.current_damage = wpn_data->damage;

	while( ( data.penetrate_count > 0 ) && ( data.current_damage >= 1.0f ) )
	{
		data.trace_length_remaining = wpn_data->range - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		CEngineTraceClient::Ray_t ray;
		ray.Init( data.src, end + data.direction * 40.f );
		g_pEngineTrace->TraceRay( ray, 0x4600400B, &data.filter, &data.enter_trace );

		ClipTraceToPlayer( data.src, end + data.direction * 40.f, MASK_SHOT_HULL | 0x40000000, &data.enter_trace, player );

		if( data.enter_trace.fraction == 1.0f )
			break;
	
		if( data.enter_trace.hitgroup <= 7 && data.enter_trace.hitgroup > 0 && data.enter_trace.m_pEnt 
			&& data.enter_trace.m_pEnt->GetIndex() > 0 && data.enter_trace.m_pEnt->GetIndex() <= 32
			&& !g_Misc.bInSameTeam( local, data.enter_trace.m_pEnt ) && !data.enter_trace.m_pEnt->bIsGhost() && data.enter_trace.m_pEnt->GetClientClass()->GetClassID() == CCSPlayer )
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow( wpn_data->range_modifier, data.trace_length * 0.002f );
			ScaleDamage( data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage );

			return true;
		}

		if( !HandleBulletPenetration( wpn_data, data ) )
			break;
	}

	return false;
}

bool CAutowall::GetDamage( const Vector &point, CBaseEntity* pLocal, CBaseEntity* pEnemy, CBaseCombatWeapon* pWeapon, float& flDamage, float flMinDamage )
{
	FireBulletData data = FireBulletData( pLocal->GetEyePosition() );

	data.filter.pSkip = pLocal;

	Vector angles;

	g_Math.CalcAngle( data.src, point, angles );
	g_Math.AngleVector( angles, data.direction );

	VectorNormalize( data.direction );

	if( !SimulateFireBullet( pLocal, pEnemy, pWeapon, data, 0 ) )
		return false;

	flDamage = data.current_damage;

	return ( data.current_damage >= flMinDamage );
}

float CAutowall::GetCustomDamage( Vector vFrom, const Vector &vTo, CBaseEntity* pFrom, CBaseEntity* pTo, CBaseCombatWeapon* pWeapon )
{
	FireBulletData data = FireBulletData( vFrom );

	data.filter.pSkip = pFrom;

	Vector angles;

	g_Math.CalcAngle( data.src, vTo, angles );
	g_Math.AngleVector( angles, data.direction );

	VectorNormalize( data.direction );

	if( !SimulateFireBullet( pFrom, pTo, pWeapon, data, 0 ) )
		return 0.f;

	return data.current_damage;
}

bool CAutowall::CanBangForward( CBaseEntity* pLocal, float &dmg )
{
	auto data = FireBulletData( pLocal->GetEyePosition() );
	data.filter.pSkip = pLocal;

	Vector EyeAng;
	g_pEngine->GetViewAngles( EyeAng );

	Vector dst, forward;

	g_Math.AngleVector( EyeAng, forward );
	dst = data.src + ( forward * 8192.f );

	Vector angles;
	g_Math.CalcAngle( data.src, dst, angles );
	g_Math.AngleVector( angles, data.direction );
	VectorNormalize( data.direction );

    CBaseCombatWeapon* weapon = pLocal->GetWeapon();

	if( !weapon )
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	auto* weaponData = weapon->GetWeaponData();

	if( !weaponData )
		return false;

	data.current_damage = ( float ) weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	CEngineTraceClient::Ray_t ray;
	ray.Init( data.src, end );
	CEngineTraceClient::CTraceFilter filter;
	filter.pSkip = pLocal;

	g_pEngineTrace->TraceRay( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &data.enter_trace );

	if( data.enter_trace.fraction == 1.0f )
		return false;

	if( HandleBulletPenetration( weaponData, data ) )
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}

float CAutowall::GetThickness( Vector start, Vector end )
{
	float current_thickness = 0.f;

	Vector vDirection;
	g_Math.AngleVector( g_Math.CalcAngles( start, end ), vDirection );

	CEngineTraceClient::CTraceWorldOnly filter;
	CEngineTraceClient::trace_t enter_trace;
	CEngineTraceClient::trace_t exit_trace;
	CEngineTraceClient::Ray_t ray;

	int pen = 0;
	while( pen < 4 )
	{
		ray.Init( start, end );

		g_pEngineTrace->TraceRay( ray, MASK_ALL, &filter, &enter_trace );

		if( enter_trace.fraction == 1.f )
			return current_thickness;

		start = enter_trace.endpos;

		if( !TraceToExit( end, &enter_trace, start, vDirection, &exit_trace ) )
			return current_thickness + 90.f;

		start = exit_trace.endpos;

		current_thickness += ( start - exit_trace.endpos ).Length();

		pen++;
	}

	return current_thickness;
}