#include "../includes.h"

#include "../UTILS/interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/IEngine.h"
#include "../SDK/SurfaceData.h"
#include "../SDK/CTrace.h"
#include "../SDK/ConVar.h"

#include "../FEATURES/AutoWall.h"
CAutowall* AutoWall = new CAutowall;
#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10
#define DAMAGE_NO		0
#define DAMAGE_EVENTS_ONLY	1	
#define DAMAGE_YES		2
#define DAMAGE_AIM		3
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
//#define CHAR_TEX_UNUSED		'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
//#define CHAR_TEX_UNUSED		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

void ScaleDamage(int hitgroup, SDK::CBaseEntity* enemy, float weapon_armor_ratio, float& current_damage)
{
	int armor = enemy->GetArmor();
	float ratio;

	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		current_damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	}

	if (armor > 0)
	{
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			if (enemy->HasHelmet())
			{
				ratio = (weapon_armor_ratio * 0.5) * current_damage;
				if (((current_damage - ratio) * 0.5) > armor)
					ratio = current_damage - (armor * 2.0);
				current_damage = ratio;
			}
			break;
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			ratio = (weapon_armor_ratio * 0.5) * current_damage;
			if (((current_damage - ratio) * 0.5) > armor)
				ratio = current_damage - (armor * 2.0);
			current_damage = ratio;
			break;
		}
	}
}

void CAutowall::TraceLine(Vector& absStart, Vector& absEnd, unsigned int mask, SDK::CBaseEntity* ignore, SDK::trace_t* ptr)
{
	SDK::Ray_t ray;
	ray.Init(absStart, absEnd);
	SDK::CTraceFilter filter;
	filter.pSkip1 = ignore;

	INTERFACES::Trace->TraceRay(ray, mask, &filter, ptr);
}

void CAutowall::ClipTraceToPlayers(Vector& absStart, Vector absEnd, unsigned int mask, SDK::ITraceFilter* filter, SDK::trace_t* tr)
{
	static DWORD dwAddress = UTILS::FindSignature("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");
	if (!dwAddress)
		return;

	_asm
	{
		MOV        EAX, filter
		LEA        ECX, tr
		PUSH    ECX
		PUSH    EAX
		PUSH    mask
		LEA        EDX, absEnd
		LEA        ECX, absStart
		CALL    dwAddress
		ADD        ESP, 0xC
	}
}

////////////////////////////////////// Legacy Functions //////////////////////////////////////
void CAutowall::GetBulletTypeParameters(float& maxRange, float& maxDistance, char* bulletType, bool sv_penetration_type)
{
	if (sv_penetration_type)
	{
		maxRange = 35.0;
		maxDistance = 3000.0;
	}
	else
	{
		//Play tribune to framerate. Thanks, stringcompare
		//Regardless I doubt anyone will use the old penetration system anyway; so it won't matter much.
		if (!strcmp(bulletType, ("BULLET_PLAYER_338MAG")))
		{
			maxRange = 45.0;
			maxDistance = 8000.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_762MM")))
		{
			maxRange = 39.0;
			maxDistance = 5000.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_556MM")) || !strcmp(bulletType, ("BULLET_PLAYER_556MM_SMALL")) || !strcmp(bulletType, ("BULLET_PLAYER_556MM_BOX")))
		{
			maxRange = 35.0;
			maxDistance = 4000.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_57MM")))
		{
			maxRange = 30.0;
			maxDistance = 2000.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_50AE")))
		{
			maxRange = 30.0;
			maxDistance = 1000.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_357SIG")) || !strcmp(bulletType, ("BULLET_PLAYER_357SIG_SMALL")) || !strcmp(bulletType, ("BULLET_PLAYER_357SIINTERFACES::250")) || !strcmp(bulletType, ("BULLET_PLAYER_357SIG_MIN")))
		{
			maxRange = 25.0;
			maxDistance = 800.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_9MM")))
		{
			maxRange = 21.0;
			maxDistance = 800.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_45ACP")))
		{
			maxRange = 15.0;
			maxDistance = 500.0;
		}
		if (!strcmp(bulletType, ("BULLET_PLAYER_BUCKSHOT")))
		{
			maxRange = 0.0;
			maxDistance = 0.0;
		}
	}
}

////////////////////////////////////// Misc Functions //////////////////////////////////////
bool CAutowall::IsBreakableEntity(SDK::CBaseEntity* entity)
{

	SDK::ClientClass* pClass = (SDK::ClientClass*)entity->GetClientClass();

	if (!pClass)
		return false;

	return pClass->m_ClassID == (int)SDK::ClassID::CBreakableProp || pClass->m_ClassID == (int)SDK::ClassID::CBreakableSurface;

}

void CAutowall::ScaleDamage(SDK::trace_t &enterTrace, SDK::CSWeaponInfo *weaponData, float& currentDamage)
{
	//Cred. to N0xius for reversing this.
	//TODO: _xAE^; look into reversing this yourself sometime

	bool hasHeavyArmor = false;
	int armorValue = ((SDK::CBaseEntity*)enterTrace.m_pEnt)->GetArmor();
	int hitGroup = enterTrace.hitGroup;

	//Fuck making a new function, lambda beste. ~ Does the person have armor on for the hitbox checked?
	auto IsArmored = [&enterTrace]()->bool
	{
		SDK::CBaseEntity* targetEntity = (SDK::CBaseEntity*)enterTrace.m_pEnt;
		switch (enterTrace.hitGroup)
		{
		case HITGROUP_HEAD:
			return !!(SDK::CBaseEntity*)targetEntity->HasHelmet(); //Fuck compiler errors - force-convert it to a bool via (!!)
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

	switch (hitGroup)
	{
	case HITGROUP_HEAD:
		currentDamage *= 2.f; //Heavy Armor does 1/2 damage
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

	if (armorValue > 0 && IsArmored())
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weaponData->m_ArmorRatio / 2.f;

		//Damage gets modified for heavy armor users
		if (hasHeavyArmor)
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = currentDamage * armorRatio;

		if (hasHeavyArmor)
			NewDamage *= 0.85f;

		if (((currentDamage - (currentDamage * armorRatio)) * (bonusValue * armorBonusRatio)) > armorValue)
			NewDamage = currentDamage - (armorValue / armorBonusRatio);

		currentDamage = NewDamage;
	}
}

////////////////////////////////////// Main Autowall Functions //////////////////////////////////////
bool CAutowall::TraceToExit(SDK::trace_t& enterTrace, SDK::trace_t& exitTrace, Vector startPosition, Vector direction)
{
	/*
	Masks used:
	MASK_SHOT_HULL					 = 0x600400B
	CONTENTS_HITBOX					 = 0x40000000
	MASK_SHOT_HULL | CONTENTS_HITBOX = 0x4600400B
	*/

	Vector start, end;
	float maxDistance = 90.f, rayExtension = 4.f, currentDistance = 0;
	int firstContents = 0;

	while (currentDistance <= maxDistance)
	{
		//Add extra distance to our ray
		currentDistance += rayExtension;

		//Multiply the direction vector to the distance so we go outwards, add our position to it.
		start = startPosition + direction * currentDistance;

		if (!firstContents)
			firstContents = INTERFACES::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr); /*0x4600400B*/
		int pointContents = INTERFACES::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(pointContents & MASK_SHOT_HULL) || pointContents & CONTENTS_HITBOX && pointContents != firstContents) /*0x600400B, *0x40000000*/
		{
			//Let's setup our end position by deducting the direction by the extra added distance
			end = start - (direction * rayExtension);

			//Let's cast a ray from our start pos to the end pos
			TraceLine(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exitTrace);

			//Let's check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if (exitTrace.startSolid && exitTrace.surface.flags & SURF_HITBOX)
			{
				TraceLine(start, startPosition, MASK_SHOT_HULL, exitTrace.m_pEnt, &exitTrace);

				if (exitTrace.DidHit() && !exitTrace.startSolid)
				{
					start = exitTrace.end;
					return true;
				}

				continue;
			}

			//Can we hit? Is the wall solid?
			if (exitTrace.DidHit() && !exitTrace.startSolid)
			{

				//Is the wall a breakable? If so, let's shoot through it.
				if (IsBreakableEntity(enterTrace.m_pEnt) && IsBreakableEntity(exitTrace.m_pEnt))
					return true;

				if (enterTrace.surface.flags & 0x0080 || !(exitTrace.surface.flags & 0x0080) && (exitTrace.plane.normal.Dot(direction) <= 1.f))
				{
					float multAmount = exitTrace.flFraction * 4.f;
					start -= direction * multAmount;
					return true;
				}

				continue;
			}

			if (!exitTrace.DidHit() || exitTrace.startSolid)
			{
				if (enterTrace.DidHit() && IsBreakableEntity(enterTrace.m_pEnt))
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

bool CAutowall::HandleBulletPenetration(SDK::CSWeaponInfo* weaponData, SDK::trace_t& enterTrace, Vector& eyePosition, Vector direction, int& possibleHitsRemaining, float& currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return false;
	//Because there's been issues regarding this- putting this here.
	if (&currentDamage == nullptr)
		throw std::invalid_argument("currentDamage is null!");

	auto data = FireBulletData(local_player->GetEyePosition());
	data.filter = SDK::CTraceFilter();
	data.filter.pSkip1 = local_player;
	SDK::trace_t exitTrace;
	SDK::CBaseEntity* pEnemy = (SDK::CBaseEntity*)enterTrace.m_pEnt;
	SDK::surfacedata_t *enterSurfaceData = INTERFACES::PhysicsProps->GetSurfaceData(enterTrace.surface.surfaceProps);
	int enterMaterial = enterSurfaceData->game.material;
	//IPhysicsSurfaceProps
	float enterSurfPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;
	float enterDamageModifier = enterSurfaceData->game.flDamageModifier;
	float thickness, modifier, lostDamage, finalDamageModifier, combinedPenetrationModifier;
	bool isSolidSurf = ((enterTrace.contents >> 3) & CONTENTS_SOLID);
	bool isLightSurf = ((enterTrace.surface.flags >> 7) & 0x0001);

	if (possibleHitsRemaining <= 0
		//Test for "DE_CACHE/DE_CACHE_TELA_03" as the entering surface and "CS_ITALY/CR_MISCWOOD2B" as the exiting surface.
		//Fixes a wall in de_cache which seems to be broken in some way. Although bullet penetration is not recorded to go through this wall
		//Decals can be seen of bullets within the glass behind of the enemy. Hacky method, but works.
		//You might want to have a check for this to only be activated on de_cache.
		|| (enterTrace.surface.name == (const char*)0x2227c261 && exitTrace.surface.name == (const char*)0x2227c868)
		|| (!possibleHitsRemaining && !isLightSurf && !isSolidSurf && enterMaterial != CHAR_TEX_GRATE && enterMaterial != CHAR_TEX_GLASS)
		|| weaponData->m_Penetration <= 0.f
		|| !TraceToExit(enterTrace, exitTrace, enterTrace.end, direction)
		&& !(INTERFACES::Trace->GetPointContents(enterTrace.end, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL))
		return false;

	SDK::surfacedata_t *exitSurfaceData = INTERFACES::PhysicsProps->GetSurfaceData(exitTrace.surface.surfaceProps);
	int exitMaterial = exitSurfaceData->game.material;
	float exitSurfPenetrationModifier = exitSurfaceData->game.flPenetrationModifier;
	float exitDamageModifier = exitSurfaceData->game.flDamageModifier;

	//Are we using the newer penetration system?
	if (sv_penetration_type)
	{
		if (enterMaterial == CHAR_TEX_GRATE || enterMaterial == CHAR_TEX_GLASS)
		{
			combinedPenetrationModifier = 3.f;
			finalDamageModifier = 0.05f;
		}
		else if (isSolidSurf || isLightSurf)
		{
			combinedPenetrationModifier = 1.f;
			finalDamageModifier = 0.16f;
		}
		else if (enterMaterial == CHAR_TEX_FLESH && (local_player->GetTeam() == pEnemy->GetTeam() && ff_damage_reduction_bullets == 0.f)) //TODO: Team check config
		{
			//Look's like you aren't shooting through your teammate today
			if (ff_damage_bullet_penetration == 0.f)
				return false;

			//Let's shoot through teammates and get kicked for teamdmg! Whatever, atleast we did damage to the enemy. I call that a win.
			combinedPenetrationModifier = ff_damage_bullet_penetration;
			finalDamageModifier = 0.16f;
		}
		else
		{
			combinedPenetrationModifier = (enterSurfPenetrationModifier + exitSurfPenetrationModifier) / 2.f;
			finalDamageModifier = 0.16f;
		}

		//Do our materials line up?
		if (enterMaterial == exitMaterial)
		{
			if (exitMaterial == CHAR_TEX_CARDBOARD || exitMaterial == CHAR_TEX_WOOD)
				combinedPenetrationModifier = 3.f;
			else if (exitMaterial == CHAR_TEX_PLASTIC)
				combinedPenetrationModifier = 2.f;
		}

		//Calculate thickness of the wall by getting the length of the range of the trace and squaring
		thickness = (exitTrace.end - enterTrace.end).LengthSqr();
		modifier = fmaxf(1.f / combinedPenetrationModifier, 0.f);

		//This calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
		lostDamage = fmaxf(
			((modifier * thickness) / 24.f)
			+ ((currentDamage * finalDamageModifier)
				+ (fmaxf(3.75f / penetrationPower, 0.f) * 3.f * modifier)), 0.f);

		//Did we loose too much damage?
		if (lostDamage > currentDamage)
			return false;

		//We can't use any of the damage that we've lost
		if (lostDamage > 0.f)
			currentDamage -= lostDamage;

		//Do we still have enough damage to deal?
		if (currentDamage < 1.f)
			return false;

		eyePosition = exitTrace.end;
		--possibleHitsRemaining;

		return true;
	}
	else //Legacy penetration system
	{
		combinedPenetrationModifier = 1.f;

		if (isSolidSurf || isLightSurf)
			finalDamageModifier = 0.99f; //Good meme :^)
		else
		{
			finalDamageModifier = fminf(enterDamageModifier, exitDamageModifier);
			combinedPenetrationModifier = fminf(enterSurfPenetrationModifier, exitSurfPenetrationModifier);
		}

		if (enterMaterial == exitMaterial && (exitMaterial == CHAR_TEX_METAL || exitMaterial == CHAR_TEX_WOOD))
			combinedPenetrationModifier += combinedPenetrationModifier;

		thickness = (exitTrace.end - enterTrace.end).LengthSqr();

		if (sqrt(thickness) <= combinedPenetrationModifier * penetrationPower)
		{
			currentDamage *= finalDamageModifier;
			eyePosition = exitTrace.end;
			--possibleHitsRemaining;

			return true;
		}

		return false;
	}
}

bool CAutowall::FireBullet(SDK::CBaseWeapon* pWeapon, Vector& direction, float& currentDamage)
{
	if (!pWeapon)
		return false;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return false;
	auto data = FireBulletData(local_player->GetEyePosition());
	SDK::CTraceFilter(data.filter);
	data.filter.pSkip1 = local_player;
	bool sv_penetration_type;
	//	  Current bullet travel Power to penetrate Distance to penetrate Range               Player bullet reduction convars			  Amount to extend ray by
	float currentDistance = 0.f, penetrationPower, penetrationDistance, maxRange, ff_damage_reduction_bullets, ff_damage_bullet_penetration, rayExtension = 40.f;
	Vector eyePosition = local_player->GetEyePosition();

	//For being superiour when the server owners think your autowall isn't well reversed. Imagine a meme HvH server with the old penetration system- pff
	static SDK::ConVar* penetrationSystem = INTERFACES::cvar->FindVar(("sv_penetration_type"));
	static SDK::ConVar* damageReductionBullets = INTERFACES::cvar->FindVar(("ff_damage_reduction_bullets"));
	static SDK::ConVar* damageBulletPenetration = INTERFACES::cvar->FindVar(("ff_damage_bullet_penetration"));

	sv_penetration_type = penetrationSystem->GetBool();
	ff_damage_reduction_bullets = damageReductionBullets->GetFloat();
	ff_damage_bullet_penetration = damageBulletPenetration->GetFloat();

	SDK::CSWeaponInfo* weaponData = pWeapon->get_full_info();
	SDK::trace_t enterTrace;

	//We should be skipping localplayer when casting a ray to players.
	SDK::CTraceFilter filter;
	filter.pSkip1 = local_player;

	if (!weaponData)
		return false;

	maxRange = weaponData->m_Range;

	GetBulletTypeParameters(penetrationPower, penetrationDistance, weaponData->hud_name, sv_penetration_type);

	if (sv_penetration_type)
		penetrationPower = weaponData->m_Penetration;

	//This gets set in FX_Firebullets to 4 as a pass-through value.
	//CS:GO has a maximum of 4 surfaces a bullet can pass-through before it 100% stops.
	//Excerpt from Valve: https://steamcommunity.com/sharedfiles/filedetails/?id=275573090
	//"The total number of surfaces any bullet can penetrate in a single flight is capped at 4." -CS:GO Official
	int possibleHitsRemaining = 4;

	//Set our current damage to what our gun's initial damage reports it will do
	currentDamage = weaponData->m_Damage;

	//If our damage is greater than (or equal to) 1, and we can shoot, let's shoot.
	while (possibleHitsRemaining > 0 && currentDamage >= 1.f)
	{
		//Calculate max bullet range
		maxRange -= currentDistance;

		//Create endpoint of bullet
		Vector end = eyePosition + direction * maxRange;

		TraceLine(eyePosition, end, MASK_SHOT_HULL | CONTENTS_HITBOX, local_player, &enterTrace);
		ClipTraceToPlayers(eyePosition, end + direction * rayExtension, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &enterTrace);

		//We have to do this *after* tracing to the player.
		SDK::surfacedata_t *enterSurfaceData = INTERFACES::PhysicsProps->GetSurfaceData(enterTrace.surface.surfaceProps);
		float enterSurfPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;
		int enterMaterial = enterSurfaceData->game.material;

		//"Fraction == 1" means that we didn't hit anything. We don't want that- so let's break on it.
		if (enterTrace.flFraction == 1.f)
			break;

		//calculate the damage based on the distance the bullet traveled.
		currentDistance += enterTrace.flFraction * maxRange;

		//Let's make our damage drops off the further away the bullet is.
		currentDamage *= pow(weaponData->m_RangeModifier, (currentDistance / 500.f));

		//Sanity checking / Can we actually shoot through?
		if (currentDistance > penetrationDistance && weaponData->m_Penetration > 0.f || enterSurfPenetrationModifier < 0.1f)
			break;

		//This looks gay as fuck if we put it into 1 long line of code.
		bool canDoDamage = (enterTrace.hitGroup != HITGROUP_GEAR && enterTrace.hitGroup != HITGROUP_GENERIC);
		//	bool isPlayer = enterTrace.m_pEnt->GetClientClass()->m_ClassID == CSClasses::CCSPlayer);
		bool isEnemy = (((SDK::CBaseEntity*)local_player)->GetTeam() != ((SDK::CBaseEntity*)enterTrace.m_pEnt)->GetTeam());
		//	bool onTeam = (((SDK::CBaseEntity*)enterTrace.m_pEnt)->GetTeamNum() == (int32_t)TEAM_CT || ((SDK::CBaseEntity*)enterTrace.m_pEnt)->GetTeamNum() == (int32_t)TeamNumbers::TEAM_T);

		//TODO: Team check config
		if ((canDoDamage &&  isEnemy))
		{
			ScaleDamage(enterTrace, weaponData, currentDamage);
			return true;
		}

		//Calling HandleBulletPenetration here reduces our penetrationCounter, and if it returns true, we can't shoot through it.
		if (!HandleBulletPenetration(weaponData, enterTrace, eyePosition, direction, possibleHitsRemaining, currentDamage, penetrationPower, sv_penetration_type, ff_damage_reduction_bullets, ff_damage_bullet_penetration))
			break;
	}
	return false;
}

////////////////////////////////////// Usage Calls //////////////////////////////////////
float CAutowall::CanHit(Vector &point)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return false;
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return false;
	auto data = FireBulletData(local_player->GetEyePosition());
	data.filter = SDK::CTraceFilter();
	data.filter.pSkip1 = local_player;
	Vector angles, direction;
	Vector tmp = point - local_player->GetEyePosition();
	float currentDamage = 0;

	MATH::VectorAngles(tmp, angles);
	MATH::AngleVectors(angles, &direction);
	direction.NormalizeInPlace();

	if (FireBullet(weapon, direction, currentDamage))
		return currentDamage;
	return -1; //That wall is just a bit too thick buddy
}

float CAutowall::CanHit1(Vector &point, SDK::CBaseEntity* pBaseEntity)
{
	Vector angles, direction;
	Vector tmp = point - pBaseEntity->GetEyePosition();
	float currentDamage = 0;
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(pBaseEntity->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return false;
	MATH::VectorAngles(tmp, angles);
	MATH::AngleVectors(angles, &direction);
	direction.NormalizeInPlace();

	if (FireBullet(weapon, direction, currentDamage))
		return currentDamage;
	return -1; //That wall is just a bit too thick buddy
}

bool CAutowall::PenetrateWall(SDK::CBaseEntity* pBaseEntity, Vector& vecPoint)
{
	float min_damage = 15.f;
	if (pBaseEntity->GetHealth() < min_damage)
		min_damage = pBaseEntity->GetHealth();

	if (CanHit(vecPoint) >= min_damage)
		return true;

	return false;
}

bool CAutowall::CanWallbang(float &dmg)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return false;
	FireBulletData data = FireBulletData(local_player->GetEyePosition());
	data.filter = SDK::CTraceFilter();
	data.filter.pSkip1 = local_player;

	Vector EyeAng;
	INTERFACES::Engine->GetViewAngles(EyeAng);

	Vector dst, forward;

	MATH::AngleVectors(EyeAng, &forward);
	dst = data.src + (forward * 8196.f);

	Vector angles;
	UTILS::calculate_angle(data.src, dst, angles);
	MATH::AngleVectors(angles, &data.direction);
	MATH::NormalizeAngle(data.direction);

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return false;


	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	SDK::CSWeaponInfo *weaponData = weapon->get_full_info();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->m_Damage;

	data.trace_length_remaining = weaponData->m_Range - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local_player, &data.enter_trace);

	static SDK::ConVar* penetrationSystem = INTERFACES::cvar->FindVar(("sv_penetration_type"));
	static SDK::ConVar* damageReductionBullets = INTERFACES::cvar->FindVar(("ff_damage_reduction_bullets"));
	static SDK::ConVar* damageBulletPenetration = INTERFACES::cvar->FindVar(("ff_damage_bullet_penetration"));

	auto ff_damage_reduction_bullets = damageReductionBullets->GetFloat();
	auto ff_damage_bullet_penetration = damageBulletPenetration->GetFloat();
	auto sv_penetration_type = penetrationSystem->GetBool();

	if (data.enter_trace.flFraction == 1.0f)
		return false;

	if (HandleBulletPenetration(weaponData, data.enter_trace, local_player->GetEyePosition(), data.direction, data.penetrate_count, data.current_damage, weaponData->m_Penetration, sv_penetration_type, ff_damage_reduction_bullets, ff_damage_bullet_penetration))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}