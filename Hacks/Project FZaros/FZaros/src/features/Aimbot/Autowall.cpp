#include "Autowall.hpp"

Autowall g_Autowall;

////////////////////////////////////// Traces //////////////////////////////////////

void TraceLine( Vector3& vecAbsStart, Vector3& vecAbsEnd, unsigned int mask, CBaseEntity *ignore, CGameTrace *ptr )
{
	C_Ray ray;
	ray.Init( vecAbsStart, vecAbsEnd );
	CTraceFilter filter;
	filter.pSkip1 = ignore;

	g_Interfaces->trace->TraceRay( ray, mask, &filter, ptr );

}

bool VectortoVectorVisible( Vector3 src, Vector3 point )
{
	CGameTrace Trace;
	TraceLine( src, point, MASK_SOLID, Globals::localPlayer, &Trace );

	if ( Trace.flFraction == 1.0f )
	{
		return true;
	}

	return false;

}

bool Autowall::TraceToExit2( Vector3& vecEnd, CGameTrace* pEnterTrace, Vector3 vecStart, Vector3 vecDir, CGameTrace* pExitTrace )
{
	float dist = 0.f;

	while ( dist <= 90.f ) {
		dist += 4.f;
		vecEnd = vecStart + vecDir * dist;

		const int point_contents = g_Interfaces->trace->GetPointContents( vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );
		if ( point_contents & MASK_SHOT_HULL && !( point_contents & CONTENTS_HITBOX ) )
			continue;

		Vector3 a1 = vecEnd - vecDir * 4.f;
		C_Ray ray;
		ray.Init( vecEnd, a1 );

		g_Interfaces->trace->TraceRay( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, pExitTrace );

		if ( pExitTrace->start != Vector3(0, 0, 0) && pExitTrace->surface.flags & SURF_HITBOX ) {
			CTraceFilterSkipEntity filter( pExitTrace->m_pEnt );
			C_Ray ray2;
			ray2.Init( vecEnd, vecStart );
			g_Interfaces->trace->TraceRay( ray2, MASK_SHOT_HULL, &filter, pExitTrace );

			if ( ( pExitTrace->flFraction < 1.f || pExitTrace->allSolid ) && !pExitTrace->startSolid ) {
				vecEnd = pExitTrace->end;
				return true;
			}

			continue;
		}

		if ( !pExitTrace->DidHit( ) || pExitTrace->startSolid ) {
			if ( pEnterTrace->m_pEnt && ( pEnterTrace->m_pEnt != nullptr && pEnterTrace->m_pEnt != g_Interfaces->clientEntity->GetClientEntity( 0 ) ) ) {
				*pExitTrace = *pEnterTrace;
				pExitTrace->end = vecStart + vecDir;
				return true;
			}

			continue;
		}

		if ( !pExitTrace->DidHit( ) || pExitTrace->startSolid ) {
			if ( pEnterTrace->m_pEnt != nullptr && !pEnterTrace->m_pEnt->EntIndex( ) == 0 && IsBreakableEntity( static_cast< CBaseEntity* >( pEnterTrace->m_pEnt ) ) ) {
				*pExitTrace = *pEnterTrace;
				pExitTrace->end = vecStart + vecDir;
				return true;
			}

			continue;
		}

		if ( pExitTrace->surface.flags >> 7 & SURF_LIGHT && !( pEnterTrace->surface.flags >> 7 & SURF_LIGHT ) )
			continue;

		if ( pExitTrace->plane.normal.Dot( vecDir ) <= 1.f ) {
			vecEnd = vecEnd - vecDir * ( pExitTrace->flFraction * 4.f );
			return true;
		}
	}

	return false;
}

void Autowall::ClipTraceToPlayers( Vector3& absStart, Vector3 absEnd, unsigned int mask, CTraceFilter* filter, CGameTrace* tr, float minDistanceToRay )
{
	CGameTrace playerTrace;
	C_Ray ray;
	float smallestFraction = tr->flFraction;
	const float maxRange = 60.0f;

	ray.Init( absStart, absEnd );

	for ( int i = 1; i <= g_Interfaces->globalVars->maxClients; i++ )
	{
		CBaseEntity *player = g_Interfaces->clientEntity->GetClientEntity( i );

		if ( !player || !player->alive( ) || player->IsDormant( ) )
			continue;

		if ( filter && filter->ShouldHitEntity( player, mask ) == false )
			continue;

		float range = g_Math.distanceToRay( player->worldSpaceCenter( ), absStart, absEnd );
		if ( range < minDistanceToRay || range > maxRange )
			continue;

		g_Interfaces->trace->ClipRayToEntity( ray, mask | CONTENTS_HITBOX, player, &playerTrace );
		if ( playerTrace.flFraction < smallestFraction )
		{
			*tr = playerTrace;
			smallestFraction = playerTrace.flFraction;
		}
	}
}

////////////////////////////////////// Legacy Functions //////////////////////////////////////
void Autowall::GetBulletTypeParameters( float& maxRange, float& maxDistance, char* bulletType, bool sv_penetration_type )
{
	if ( sv_penetration_type )
	{
		maxRange = 35.0;
		maxDistance = 3000.0;
	}
	else
	{
		//Play tribune to framerate. Thanks, stringcompare
		//Regardless I doubt anyone will use the old penetration system anyway; so it won't matter much.
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_338MAG" ) ) )
		{
			maxRange = 45.0;
			maxDistance = 8000.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_762MM" ) ) )
		{
			maxRange = 39.0;
			maxDistance = 5000.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_556MM" ) ) || !strcmp( bulletType, ( "BULLET_PLAYER_556MM_SMALL" ) ) || !strcmp( bulletType, ( "BULLET_PLAYER_556MM_BOX" ) ) )
		{
			maxRange = 35.0;
			maxDistance = 4000.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_57MM" ) ) )
		{
			maxRange = 30.0;
			maxDistance = 2000.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_50AE" ) ) )
		{
			maxRange = 30.0;
			maxDistance = 1000.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_357SIG" ) ) || !strcmp( bulletType, ( "BULLET_PLAYER_357SIG_SMALL" ) ) || !strcmp( bulletType, ( "BULLET_PLAYER_357SIG_P250" ) ) || !strcmp( bulletType, ( "BULLET_PLAYER_357SIG_MIN" ) ) )
		{
			maxRange = 25.0;
			maxDistance = 800.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_9MM" ) ) )
		{
			maxRange = 21.0;
			maxDistance = 800.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_45ACP" ) ) )
		{
			maxRange = 15.0;
			maxDistance = 500.0;
		}
		if ( !strcmp( bulletType, ( "BULLET_PLAYER_BUCKSHOT" ) ) )
		{
			maxRange = 0.0;
			maxDistance = 0.0;
		}
	}
}

////////////////////////////////////// Misc Functions //////////////////////////////////////
bool Autowall::IsBreakableEntity( CBaseEntity* entity )
{
	if ( !entity || !entity->EntIndex( ) )
		return false;

	// backup original take_damage value.
	const int take_damage = entity->getTakeDamage( );

	const CClientClass *clientClass = entity->GetClientClass( );

	if ( clientClass->NetworkName[ 1 ] == 'B' && clientClass->NetworkName[ 9 ] == 'e' && clientClass->NetworkName[ 10 ] == 'S' && clientClass->NetworkName[ 16 ] == 'e' )
		entity->getTakeDamage( ) = 2;

	else if ( clientClass->NetworkName[ 1 ] != 'B' && clientClass->NetworkName[ 5 ] != 'D' )
		entity->getTakeDamage( ) = 2;

	else if ( clientClass->NetworkName[ 1 ] != 'F' && clientClass->NetworkName[ 4 ] != 'c' && clientClass->NetworkName[ 5 ] != 'B' && clientClass->NetworkName[ 9 ] != 'h' ) // CFuncBrush
		entity->getTakeDamage( ) = 2;

	using IsBreakableEntity_t = bool( __thiscall * )( CBaseEntity * );
	static IsBreakableEntity_t IsBreakableEntityEx = nullptr;

	if ( !IsBreakableEntityEx )
		IsBreakableEntityEx = reinterpret_cast< IsBreakableEntity_t >( g_Memory->findPattern( "client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68" ) );

	entity->getTakeDamage( ) = take_damage;

	return IsBreakableEntityEx( entity );
}

void Autowall::ScaleDamage( CGameTrace &enterTrace, CSWeaponInfo *weaponData, float& currentDamage )
{
	//Cred. to N0xius for reversing this.
	//TODO: _xAE^; look into reversing this yourself sometime

	bool hasHeavyArmor = ( ( CBaseEntity* )enterTrace.m_pEnt )->heavyArmor( );
	int armorValue = ( ( CBaseEntity* )enterTrace.m_pEnt )->armorValue( );

	//Fuck making a new function, lambda beste. ~ Does the person have armor on for the hitbox checked?
	auto IsArmored = [ &enterTrace ] ( )->bool
	{
		CBaseEntity* targetEntity = ( CBaseEntity* )enterTrace.m_pEnt;
		switch ( enterTrace.hitGroup )
		{
		case HITGROUP_HEAD:
			return !!( CBaseEntity* )targetEntity->hasHelmet( ); //Fuck compiler errors - force-convert it to a bool via (!!)
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch ( enterTrace.hitGroup )
	{
	case HITGROUP_HEAD:
		currentDamage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
		break;
	case HITGROUP_STOMACH:
		currentDamage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		currentDamage *= 0.75f;
		break;
	default:
		break;
	}

	if ( armorValue > 0 && IsArmored( ) )
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weaponData->armorRatio / 2.f;

		//Damage gets modified for heavy armor users
		if ( hasHeavyArmor )
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = currentDamage * armorRatio;

		if ( hasHeavyArmor )
			NewDamage *= 0.85f;

		if ( ( ( currentDamage - ( currentDamage * armorRatio ) ) * ( bonusValue * armorBonusRatio ) ) > armorValue )
			NewDamage = currentDamage - ( armorValue / armorBonusRatio );

		currentDamage = NewDamage;
	}
}

bool Autowall::DidHitWorld( CBaseEntity * ent )
{
	return ent == g_Interfaces->clientEntity->GetClientEntity( 0 );
}

bool Autowall::DidHitNonWorldEntity( CBaseEntity * ent )
{
	return ent != nullptr && !DidHitWorld( ent );
}

////////////////////////////////////// Main Autowall Functions //////////////////////////////////////
bool Autowall::TraceToExit( CGameTrace& enterTrace, CGameTrace& exitTrace, Vector3 startPosition, Vector3 direction )
{
	/*
	Masks used:
	MASK_SHOT_HULL					 = 0x600400B
	CONTENTS_HITBOX					 = 0x40000000
	MASK_SHOT_HULL | CONTENTS_HITBOX = 0x4600400B
	*/

	Vector3 start, end;
	float maxDistance = 90.f, rayExtension = 4.f, currentDistance = 0;
	int firstContents = 0;

	while ( currentDistance <= maxDistance )
	{
		//Add extra distance to our ray
		currentDistance += rayExtension;

		//Multiply the direction Vector3 to the distance so we go outwards, add our position to it.
		start = startPosition + direction * currentDistance;

		if ( !firstContents )
			firstContents = g_Interfaces->trace->GetPointContents( start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr ); /*0x4600400B*/
		int pointContents = g_Interfaces->trace->GetPointContents( start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );

		if ( !( pointContents & MASK_SHOT_HULL ) || pointContents & CONTENTS_HITBOX && pointContents != firstContents ) /*0x600400B, *0x40000000*/
		{
			//Let's setup our end position by deducting the direction by the extra added distance
			end = start - ( direction * rayExtension );

			//Let's cast a ray from our start pos to the end pos
			TraceLine( start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exitTrace );

			//Let's check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if ( exitTrace.startSolid && exitTrace.surface.flags & SURF_HITBOX )
			{
				TraceLine( start, startPosition, MASK_SHOT_HULL, exitTrace.m_pEnt, &exitTrace );

				if ( exitTrace.hitGroup != 0 && !exitTrace.startSolid )
				{
					start = exitTrace.end;
					return true;
				}

				continue;
			}

			//Can we hit? Is the wall solid?
			if ( exitTrace.DidHit( ) && !exitTrace.startSolid )
			{
				//Is the wall a breakable? If so, let's shoot through it.
				if ( IsBreakableEntity( enterTrace.m_pEnt ) && IsBreakableEntity( exitTrace.m_pEnt ) )
					return true;

				if ( enterTrace.surface.flags & SURF_NODRAW || !( exitTrace.surface.flags & SURF_NODRAW ) && ( exitTrace.plane.normal.Dot( direction ) <= 1.f ) )
				{
					float multAmount = exitTrace.flFraction * 4.f;
					start -= direction * multAmount;
					return true;
				}

				continue;
			}

			if ( !exitTrace.DidHit( ) || exitTrace.startSolid )
			{
				if ( DidHitNonWorldEntity( enterTrace.m_pEnt ) && IsBreakableEntity( enterTrace.m_pEnt ) )
				{
					exitTrace = enterTrace;
					exitTrace.end = start + direction;
					return true;
				}

				continue;
			}
		}
	}
	return false;
}

#define CHAR_TEX_FLESH		'F'
#define CHAR_TEX_GRATE		'G'
#define CHAR_TEX_PLASTIC	'L'
#define CHAR_TEX_METAL		'M'
#define CHAR_TEX_CARDBOARD	'U'
#define CHAR_TEX_WOOD		'W'
#define CHAR_TEX_GLASS		'Y'

bool Autowall::HandleBulletPenetration( CSWeaponInfo* weaponData, CGameTrace& enterTrace, Vector3& eyePosition, Vector3 direction, int& possibleHitsRemaining, float& currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration )
{
	//Because there's been issues regarding this- putting this here.
	if ( &currentDamage == nullptr )
		throw std::invalid_argument( "currentDamage is null!\n" );

	Vector3 end;
	CGameTrace exitTrace;
	CBaseEntity* pEnemy = ( CBaseEntity* )enterTrace.m_pEnt;
	surfacedata_t *enterSurfaceData = g_Interfaces->surfaceData->GetSurfaceData( enterTrace.surface.surfaceProps );
	int enterMaterial = enterSurfaceData->game.material;

	float enterSurfPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;
	float enterDamageModifier = enterSurfaceData->game.flDamageModifier;
	float thickness, modifier, lostDamage, finalDamageModifier, combinedPenetrationModifier;
	bool isSolidSurf = ( ( enterTrace.contents >> 3 ) & CONTENTS_SOLID );
	bool isLightSurf = ( ( enterTrace.surface.flags >> 7 ) & SURF_LIGHT );

	if ( possibleHitsRemaining <= 0
		//Test for "DE_CACHE/DE_CACHE_TELA_03" as the entering surface and "CS_ITALY/CR_MISCWOOD2B" as the exiting surface.
		//Fixes a wall in de_cache which seems to be broken in some way. Although bullet penetration is not recorded to go through this wall
		//Decals can be seen of bullets within the glass behind of the enemy. Hacky method, but works.
		//You might want to have a check for this to only be activated on de_cache.
		|| ( enterTrace.surface.name == ( const char* )0x2227c261 && exitTrace.surface.name == ( const char* )0x2227c868 )
		|| ( !possibleHitsRemaining && !isLightSurf && !isSolidSurf && enterMaterial != CHAR_TEX_GRATE && enterMaterial != CHAR_TEX_GLASS )
		|| weaponData->penetration <= 0.f
		|| !TraceToExit2( end, &enterTrace, enterTrace.end, direction, &exitTrace )
		&& !( g_Interfaces->trace->GetPointContents( enterTrace.end, MASK_SHOT_HULL, nullptr ) & MASK_SHOT_HULL ) )
		return false;

	surfacedata_t *exitSurfaceData = g_Interfaces->surfaceData->GetSurfaceData( exitTrace.surface.surfaceProps );
	int exitMaterial = exitSurfaceData->game.material;
	float exitSurfPenetrationModifier = exitSurfaceData->game.flPenetrationModifier;
	float exitDamageModifier = exitSurfaceData->game.flDamageModifier;

	//Are we using the newer penetration system?
	if ( sv_penetration_type )
	{
		if ( enterMaterial == CHAR_TEX_GRATE || enterMaterial == CHAR_TEX_GLASS )
		{
			combinedPenetrationModifier = 3.f;
			finalDamageModifier = 0.05f;
		}
		else if ( isSolidSurf || isLightSurf )
		{
			combinedPenetrationModifier = 1.f;
			finalDamageModifier = 0.16f;
		}
		else if ( enterMaterial == CHAR_TEX_FLESH && ( Globals::localPlayer->team( ) == pEnemy->team( ) && ff_damage_reduction_bullets == 0.f ) ) //TODO: Team check config
		{
			//Look's like you aren't shooting through your teammate today
			if ( ff_damage_bullet_penetration == 0.f )
				return false;

			//Let's shoot through teammates and get kicked for teamdmg! Whatever, atleast we did damage to the enemy. I call that a win.
			combinedPenetrationModifier = ff_damage_bullet_penetration;
			finalDamageModifier = 0.16f;
		}
		else
		{
			combinedPenetrationModifier = ( enterSurfPenetrationModifier + exitSurfPenetrationModifier ) / 2.f;
			finalDamageModifier = 0.16f;
		}

		//Do our materials line up?
		if ( enterMaterial == exitMaterial )
		{
			if ( exitMaterial == CHAR_TEX_CARDBOARD || exitMaterial == CHAR_TEX_WOOD )
				combinedPenetrationModifier = 3.f;
			else if ( exitMaterial == CHAR_TEX_PLASTIC )
				combinedPenetrationModifier = 2.f;
		}

		//Calculate thickness of the wall by getting the length of the range of the trace and squaring
		thickness = ( exitTrace.end - enterTrace.end ).LengthSqr( );
		modifier = fmaxf( 1.f / combinedPenetrationModifier, 0.f );

		//This calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
		lostDamage = fmaxf(
			( ( modifier * thickness ) / 24.f )
			+ ( ( currentDamage * finalDamageModifier )
				+ ( fmaxf( 3.75f / penetrationPower, 0.f ) * 3.f * modifier ) ), 0.f );

		//Did we loose too much damage?
		if ( lostDamage > currentDamage )
			return false;

		//We can't use any of the damage that we've lost
		if ( lostDamage > 0.f )
			currentDamage -= lostDamage;

		//Do we still have enough damage to deal?
		if ( currentDamage < 1.f )
			return false;

		eyePosition = exitTrace.end;
		--possibleHitsRemaining;

		return true;
	}
	else //Legacy penetration system
	{
		combinedPenetrationModifier = 1.f;

		if ( isSolidSurf || isLightSurf )
			finalDamageModifier = 0.99f; //Good meme :^)
		else
		{
			finalDamageModifier = fminf( enterDamageModifier, exitDamageModifier );
			combinedPenetrationModifier = fminf( enterSurfPenetrationModifier, exitSurfPenetrationModifier );
		}

		if ( enterMaterial == exitMaterial && ( exitMaterial == CHAR_TEX_METAL || exitMaterial == CHAR_TEX_WOOD ) )
			combinedPenetrationModifier += combinedPenetrationModifier;

		thickness = ( exitTrace.end - enterTrace.end ).LengthSqr( );

		if ( sqrt( thickness ) <= combinedPenetrationModifier * penetrationPower )
		{
			currentDamage *= finalDamageModifier;
			eyePosition = exitTrace.end;
			--possibleHitsRemaining;

			return true;
		}

		return false;
	}
}

bool Autowall::FireBullet( CBaseCombatWeapon* pWeapon, Vector3& direction, float& currentDamage )
{
	if ( !pWeapon )
		return false;

	bool sv_penetration_type;
	//	  Current bullet travel Power to penetrate Distance to penetrate Range               Player bullet reduction convars			  Amount to extend ray by
	float currentDistance = 0.f, penetrationPower, penetrationDistance, maxRange, ff_damage_reduction_bullets, ff_damage_bullet_penetration, rayExtension = 40.f;
	Vector3 eyePosition = Globals::localPlayer->eyePosition( );

	//For being superiour when the server owners think your autowall isn't well reversed. Imagine a meme HvH server with the old penetration system- pff
	static ConVar* penetrationSystem = g_Interfaces->cvar->FindVar( ( "sv_penetration_type" ) );
	static ConVar* damageReductionBullets = g_Interfaces->cvar->FindVar( ( "ff_damage_reduction_bullets" ) );
	static ConVar* damageBulletPenetration = g_Interfaces->cvar->FindVar( ( "ff_damage_bullet_penetration" ) );

	sv_penetration_type = penetrationSystem->GetInt( );
	ff_damage_reduction_bullets = damageReductionBullets->GetFloat( );
	ff_damage_bullet_penetration = damageBulletPenetration->GetFloat( );

	CSWeaponInfo* weaponData = pWeapon->weaponData( );
	CGameTrace enterTrace;

	//We should be skipping g_Globals->LocalPlayer when casting a ray to players.
	CTraceFilter filter;
	filter.pSkip1 = Globals::localPlayer;

	if ( !weaponData )
		return false;

	maxRange = weaponData->range;

	GetBulletTypeParameters( penetrationPower, penetrationDistance, ( char* )"", sv_penetration_type );
	penetrationPower = 35.0;
	penetrationDistance = 3000.0;

	if ( sv_penetration_type )
		penetrationPower = weaponData->penetration;

	//This gets set in FX_Firebullets to 4 as a pass-through value.
	//CS:GO has a maximum of 4 surfaces a bullet can pass-through before it 100% stops.
	//Excerpt from Valve: https://steamcommunity.com/sharedfiles/filedetails/?id=275573090
	//"The total number of surfaces any bullet can penetrate in a single flight is capped at 4." -CS:GO Official
	int possibleHitsRemaining = 4;

	//Set our current damage to what our gun's initial damage reports it will do
	currentDamage = weaponData->damage;

	//If our damage is greater than (or equal to) 1, and we can shoot, let's shoot.
	while ( possibleHitsRemaining > 0 && currentDamage >= 1.f )
	{
		//Calculate max bullet range
		maxRange -= currentDistance;

		//Create endpoint of bullet
		Vector3 end = eyePosition + direction * maxRange;

		TraceLine( eyePosition, end, MASK_SHOT_HULL | CONTENTS_HITBOX, Globals::localPlayer, &enterTrace );
		ClipTraceToPlayers( eyePosition, end + direction * rayExtension, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &enterTrace, 1.f );

		//We have to do this *after* tracing to the player.
		surfacedata_t *enterSurfaceData = g_Interfaces->surfaceData->GetSurfaceData( enterTrace.surface.surfaceProps );
		float enterSurfPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;
		int enterMaterial = enterSurfaceData->game.material;

		//"Fraction == 1" means that we didn't hit anything. We don't want that- so let's break on it.
		if ( enterTrace.flFraction == 1.f )
			break;

		//calculate the damage based on the distance the bullet traveled.
		currentDistance += enterTrace.flFraction * maxRange;

		//Let's make our damage drops off the further away the bullet is.
		currentDamage *= pow( weaponData->rangeModifier, ( currentDistance / 500.f ) );

		//Sanity checking / Can we actually shoot through?
		if ( currentDistance > penetrationDistance && weaponData->penetration > 0.f || enterSurfPenetrationModifier < 0.1f )
			break;

		//This looks gay as fuck if we put it into 1 long line of code.

		bool canDoDamage = ( enterTrace.hitGroup != HITGROUP_GEAR && enterTrace.hitGroup != HITGROUP_GENERIC );
		bool isPlayer = ( ( enterTrace.m_pEnt )->GetClientClass( )->iClassID == 40 );
		bool isEnemy = ( ( ( CBaseEntity* )Globals::localPlayer )->team( ) != ( ( CBaseEntity* )enterTrace.m_pEnt )->team( ) );
		bool onTeam = ( ( ( CBaseEntity* )enterTrace.m_pEnt )->team( ) == ( int32_t )3 || ( ( CBaseEntity* )enterTrace.m_pEnt )->team( ) == ( int32_t )2 );

		//TODO: Team check config
		if ( ( canDoDamage && isPlayer && isEnemy ) && onTeam )
		{
			ScaleDamage( enterTrace, weaponData, currentDamage );
			return true;
		}

		//Calling HandleBulletPenetration here reduces our penetrationCounter, and if it returns true, we can't shoot through it.
		if ( !HandleBulletPenetration( weaponData, enterTrace, eyePosition, direction, possibleHitsRemaining, currentDamage, penetrationPower, sv_penetration_type, ff_damage_reduction_bullets, ff_damage_bullet_penetration ) )
			break;
	}

	return false;
}

static Vector3 umomaim;

float GetHitgroupDamageMult( int iHitGroup )
{
	switch ( iHitGroup )
	{
	case HITGROUP_GENERIC:
		return 0.5f;
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_CHEST:
		return 1.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTARM:
		return 0.65f;
	case HITGROUP_RIGHTARM:
		return 0.65f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 0.5f;
	default:
		return 1.0f;
	}
}

void ScaleDamage( int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage )
{
	const float armor = static_cast< float >( enemy->armorValue( ) );
	current_damage *= GetHitgroupDamageMult( hitgroup );

	if ( armor > 0.f && hitgroup < HITGROUP_LEFTLEG ) {
		if ( hitgroup == HITGROUP_HEAD && !enemy->hasHelmet( ) )
			return;

		auto scaled = weapon_armor_ratio * .5f * current_damage;

		if ( ( current_damage - scaled ) * .5f > armor )
			scaled = current_damage - armor * 2.f;

		current_damage = scaled;
	}
}

bool Autowall::HandleBulletPenetration2( CSWeaponInfo *wpn_data, FireBulletData &data, bool extracheck )
{
	surfacedata_t *enter_surface_data = g_Interfaces->surfaceData->GetSurfaceData( data.enter_trace.surface.surfaceProps );
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= pow( wpn_data->rangeModifier, ( data.trace_length * 0.002f ) );

	if ( ( data.trace_length > 3000.f ) || ( enter_surf_penetration_mod < 0.1f ) )
		data.penetrate_count = 0;

	if ( data.penetrate_count <= 0 )
		return false;

	Vector3 dummy;
	CGameTrace trace_exit;
	if ( !TraceToExit2( dummy, &data.enter_trace, data.enter_trace.end, data.direction, &trace_exit ) )
		return false;

	surfacedata_t *exit_surface_data = g_Interfaces->surfaceData->GetSurfaceData( trace_exit.surface.surfaceProps );
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if ( ( ( data.enter_trace.contents & CONTENTS_GRATE ) != 0 ) || ( enter_material == 89 ) || ( enter_material == 71 ) )
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = ( enter_surf_penetration_mod + exit_surf_penetration_mod ) * 0.5f;
	}

	if ( enter_material == exit_material )
	{
		if ( exit_material == 87 || exit_material == 85 )
			combined_penetration_modifier = 3.0f;
		else if ( exit_material == 76 )
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf( 0.f, 1.0f / combined_penetration_modifier );
	float v35 = ( data.current_damage * final_damage_modifier ) + v34 * 3.0f * fmaxf( 0.0f, ( 3.0f / wpn_data->penetration ) * 1.25f );
	float thickness = VectorLength( trace_exit.end - data.enter_trace.end );

	if ( extracheck )
	{
		if ( !VectortoVectorVisible( trace_exit.end, umomaim ) )
			return false;
	}

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf( 0.0f, v35 + thickness );

	if ( lost_damage > data.current_damage )
		return false;

	if ( lost_damage >= 0.0f )
		data.current_damage -= lost_damage;

	if ( data.current_damage < 1.0f )
		return false;

	data.src = trace_exit.end;
	data.penetrate_count--;

	return true;
}

////////////////////////////////////// Usage Calls //////////////////////////////////////
float Autowall::CanHit( Vector3 &point )
{
	Vector3 angles, direction;
	Vector3 tmp = point - Globals::localPlayer->eyePosition( );
	float currentDamage = 0;

	g_Math.vectorAngles( tmp, angles );
	g_Math.angleVectors( angles, &direction );
	direction.NormalizeInPlace( );

	if ( FireBullet( Globals::localPlayer->activeWeapon( ), direction, currentDamage ) )
		return currentDamage;

	return -1; //That wall is just a bit too thick buddy
}

bool Autowall::CanHitFloatingPoint( const Vector3 &point, const Vector3 &source )
{
	umomaim = point;

	if ( !Globals::localPlayer )
		return false;

	FireBulletData data = FireBulletData( source );
	data.filter = CTraceFilter( );
	data.filter.pSkip1 = Globals::localPlayer;

	Vector3 angles = g_Math.calcAngle( data.src, point );
	g_Math.angleVectors( angles, &data.direction );
	Vector3Normalize( data.direction );

	CBaseCombatWeapon *pWeapon = ( CBaseCombatWeapon* )Globals::localPlayer->activeWeapon( );

	if ( !pWeapon )
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CSWeaponInfo *weaponData = pWeapon->weaponData( );

	if ( !weaponData )
		return false;

	data.current_damage = ( float )weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector3 end = data.src + data.direction * data.trace_length_remaining;

	TraceLine( data.src, end, MASK_SHOT | CONTENTS_GRATE, Globals::localPlayer, &data.enter_trace );

	if ( VectortoVectorVisible( point, data.src ) || HandleBulletPenetration2( weaponData, data, true ) )
	{
		return true;
	}

	return false;
}

bool Autowall::CanWallbang( float &dmg )
{
	if ( !Globals::localPlayer )
		return false;

	FireBulletData data = FireBulletData( Globals::localPlayer->eyePosition( ) );
	data.filter = CTraceFilter( );
	data.filter.pSkip1 = Globals::localPlayer;

	Vector3 EyeAng;
	g_Interfaces->gameEngine->getViewAngles( EyeAng );

	Vector3 dst, forward;

	g_Math.angleVectors( EyeAng, &forward );
	dst = data.src + ( forward * 8196.f );

	Vector3 angles = g_Math.calcAngle( data.src, dst );
	g_Math.angleVectors( angles, &data.direction );
	g_Math.vectorNormalize( data.direction );

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CSWeaponInfo *weaponData = Globals::localPlayer->activeWeapon( )->weaponData( );

	if ( !weaponData )
		return false;

	data.current_damage = ( float )weaponData->damage;

	data.trace_length_remaining = weaponData->range - data.trace_length;

	Vector3 end = data.src + data.direction * data.trace_length_remaining;

	TraceLine( data.src, end, MASK_SHOT | CONTENTS_GRATE, Globals::localPlayer, &data.enter_trace );

	if ( data.enter_trace.flFraction == 1.0f )
		return false;

	if ( HandleBulletPenetration2( weaponData, data, false ) )
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Wow I took way too long on autowall.
// Special Thanks to the following (No order intended)
// Killerra
// Flaw
// Zbe
// Heep
// Polak (indirectly; IsBreakable Fix)
// Esoterik (Autowall UC Post)
// N0xius (ScaleDamage)
// Requiii
// drew1ind#8957 (Scaling for ConVar)
// The non-autistic slice of CS:GO-UC