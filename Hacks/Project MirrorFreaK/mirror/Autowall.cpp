#include "Autowall.h"
#include "Menu.h"
#include "XorStr.hpp"
#include "RageBot.h"
#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10
inline bool CGameTrace::DidHitWorld() const
{
	return m_pEnt->GetIndex() == 0;
}
inline bool CGameTrace::DidHitNonWorldEntity() const
{
	return m_pEnt != NULL && !DidHitWorld();
}

backup_autowall * backup_awall = new backup_autowall;
autowall_2* new_autowall = new autowall_2;

#define HITGROUP_GENERIC    0
#define HITGROUP_HEAD        1
#define HITGROUP_CHEST        2
#define HITGROUP_STOMACH    3
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
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.
/*
void ScaleDamage_1(int hitgroup, IClientEntity* enemy, float weapon_armor_ratio, float& current_damage)
{
	int armor = enemy->ArmorValue();
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

void autowall_2::TraceLine(Vector& absStart, Vector& absEnd, unsigned int mask, IClientEntity* ignore, CGameTrace* ptr)
{
	Ray_t ray;
	ray.Init(absStart, absEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	Interfaces::Trace->TraceRay(ray, mask, &filter, ptr);
}

void autowall_2::ClipTraceToPlayers(const Vector& absStart, const Vector absEnd, unsigned int mask, ITraceFilter* filter, CGameTrace* tr)
{

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	static DWORD dwAddress = Utilities::Memory::FindPatternV2("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

	if (!dwAddress)
		return;

	if  (weapon->IsKnife() || weapon->IsC4())
		return;
	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, absEnd
		LEA		ECX, absStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

////////////////////////////////////// Legacy Functions //////////////////////////////////////
void autowall_2::GetBulletTypeParameters(float& maxRange, float& maxDistance, char* bulletType, bool sv_penetration_type)
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
		if (!strcmp(bulletType, ("BULLET_PLAYER_357SIG")) || !strcmp(bulletType, ("BULLET_PLAYER_357SIG_SMALL")) || !strcmp(bulletType, ("BULLET_PLAYER_357SIG_P250")) || !strcmp(bulletType, ("BULLET_PLAYER_357SIG_MIN")))
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
bool autowall_2::BreakableEntity(IClientEntity* entity)
{

	ClientClass* pClass = (ClientClass*)entity->GetClientClass();

	if (!pClass)
		return false;

	if (pClass == nullptr)
		return false;

//	if (pClass->m_ClassID = (int)CSGOClassID::CChicken)
//	return false;

	return pClass->m_ClassID == (int)CSGOClassID::CBreakableProp || pClass->m_ClassID == (int)CSGOClassID::CBreakableSurface;

}

void autowall_2::ScaleDamage(CGameTrace &enterTrace, CSWeaponInfo *weaponData, float& currentDamage)
{

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	bool hasHeavyArmor = false;
	int armorValue = ((IClientEntity*)enterTrace.m_pEnt)->ArmorValue();
	int hitGroup = enterTrace.hitgroup;

	if (!pLocal)
		return;

	if (weapon->isZeus() || weapon->IsKnife() || weapon->IsC4())
		return;

	auto IsArmored = [&enterTrace]()->bool
	{
		IClientEntity* targetEntity = (IClientEntity*)enterTrace.m_pEnt;
		switch (enterTrace.hitgroup)
		{
		case HITGROUP_HEAD:
			return !!(IClientEntity*)targetEntity->HasHelmet();
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
		currentDamage *= 2.f;
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
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weaponData->armor_ratio / 2.f;

		if (hasHeavyArmor)
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = currentDamage * armorRatio;

		if (((currentDamage - (currentDamage * armorRatio)) * (bonusValue * armorBonusRatio)) > armorValue)
			NewDamage = currentDamage - (armorValue / armorBonusRatio);

		currentDamage = NewDamage;
	}
}

////////////////////////////////////// Main Autowall Functions //////////////////////////////////////
bool autowall_2::trace_to_exit(CGameTrace& enterTrace, CGameTrace& exitTrace, Vector startPosition, Vector direction)
{
	Vector start, end;
	float maxDistance = 90.f, rayExtension = 4.f, currentDistance = 0;
	int firstContents = 0;

	while (currentDistance <= maxDistance)
	{
		currentDistance += rayExtension;

		start = startPosition + direction * currentDistance;

		if (!firstContents)
			firstContents = Interfaces::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		int pointContents = Interfaces::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(pointContents & MASK_SHOT_HULL) || pointContents & CONTENTS_HITBOX && pointContents != firstContents)
		{
			end = start - (direction * rayExtension);

			TraceLine(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exitTrace);

			if (exitTrace.startsolid && exitTrace.surface.flags & SURF_HITBOX)
			{
				TraceLine(start, startPosition, MASK_SHOT_HULL, exitTrace.m_pEnt, &exitTrace);

				if (exitTrace.DidHit() && !exitTrace.startsolid)
				{
					start = exitTrace.endpos;
					return true;
				}
				continue;
			}

			if (exitTrace.DidHit() && !exitTrace.startsolid)
			{
				if (BreakableEntity(enterTrace.m_pEnt) && BreakableEntity(exitTrace.m_pEnt))
					return true;		

				if (enterTrace.surface.flags & SURF_NODRAW || !(exitTrace.surface.flags & SURF_NODRAW) && (exitTrace.plane.normal.Dot(direction) <= 1.f))
				{
					float multAmount = exitTrace.fraction * 4.f;
					start -= direction * multAmount;
					return true;
				}

				continue;
			}

			if (!exitTrace.DidHit() || exitTrace.startsolid)
			{
				if (enterTrace.DidHitNonWorldEntity() && BreakableEntity(enterTrace.m_pEnt))
				{
					exitTrace = enterTrace;
					exitTrace.endpos = start + direction;
					return true;
				}

				continue;
			}
		}
	}
	return false;
}

bool autowall_2::HandleBulletPenetration(CSWeaponInfo* weaponData, CGameTrace& enterTrace, Vector& eyePosition, Vector direction, int& possibleHitsRemaining, float& currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration)
{
	//Because there's been issues regarding this- putting this here.
	if (&currentDamage == nullptr)
	{
		handle_penetration = false;
		return false;
	}

	IClientEntity* local = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	auto data = FireBulletData(local->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;
	CGameTrace exitTrace;
	IClientEntity* pEnemy = (IClientEntity*)enterTrace.m_pEnt;
	surfacedata_t *enterSurfaceData = Interfaces::PhysProps->GetSurfaceData(enterTrace.surface.surfaceProps);

	int enterMaterial = enterSurfaceData->game.material;

	float enterSurfPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;
	float enterDamageModifier = enterSurfaceData->game.flDamageModifier;
	float thickness, modifier, lostDamage, finalDamageModifier, combinedPenetrationModifier;
	bool isSolidSurf = ((enterTrace.contents >> 3) & CONTENTS_SOLID);
	bool isLightSurf = ((enterTrace.surface.flags >> 7) & SURF_LIGHT);

	if (possibleHitsRemaining <= 0
		|| (enterTrace.surface.name == (const char*)0x2227c261 && exitTrace.surface.name == (const char*)0x2227c868)
		|| (!possibleHitsRemaining && !isLightSurf && !isSolidSurf && enterMaterial != CHAR_TEX_GRATE && enterMaterial != CHAR_TEX_GLASS)
		|| weaponData->penetration <= 0.f
		|| !trace_to_exit(enterTrace, exitTrace, enterTrace.endpos, direction)
		&& !(Interfaces::Trace->GetPointContents(enterTrace.endpos, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL))
	{
		handle_penetration = false;
		return false;
	}

	surfacedata_t *exitSurfaceData = Interfaces::PhysProps->GetSurfaceData(exitTrace.surface.surfaceProps);
	int exitMaterial = exitSurfaceData->game.material;
	float exitSurfPenetrationModifier = exitSurfaceData->game.flPenetrationModifier;
	float exitDamageModifier = exitSurfaceData->game.flDamageModifier;

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
		else if (enterMaterial == CHAR_TEX_FLESH && (local->team() == pEnemy->team() && ff_damage_reduction_bullets == 0.f))
		{
			if (ff_damage_bullet_penetration == 0.f)
			{
				handle_penetration = false;
				return false;
			}
			combinedPenetrationModifier = ff_damage_bullet_penetration;
			finalDamageModifier = 0.16f;
		}
		else
		{
			combinedPenetrationModifier = (enterSurfPenetrationModifier + exitSurfPenetrationModifier) / 2.f;
			finalDamageModifier = 0.16f;
		}

		if (enterMaterial == exitMaterial)
		{
			if (exitMaterial == CHAR_TEX_CARDBOARD || exitMaterial == CHAR_TEX_WOOD)
				combinedPenetrationModifier = 3.f;
			else if (exitMaterial == CHAR_TEX_PLASTIC)
				combinedPenetrationModifier = 2.f;
		}

		thickness = (exitTrace.endpos - enterTrace.endpos).LengthSqr();
		modifier = fmaxf(1.f / combinedPenetrationModifier, 0.f);

		lostDamage = fmaxf(
			((modifier * thickness) / 24.f)
			+ ((currentDamage * finalDamageModifier)
				+ (fmaxf(3.75f / penetrationPower, 0.f) * 3.f * modifier)), 0.f);

		if (lostDamage > currentDamage)
		{
			handle_penetration = false;
			return false;
		}

		if (lostDamage > 0.f)
			currentDamage -= lostDamage;

		if (currentDamage < 1.f)
		{
			handle_penetration = false;
			return false;
		}

		eyePosition = exitTrace.endpos;
		--possibleHitsRemaining;

		handle_penetration = true;
		return true;
	}
	else
	{
		combinedPenetrationModifier = 1.f;

		if (isSolidSurf || isLightSurf)
			finalDamageModifier = 0.99f;
		else
		{
			finalDamageModifier = fminf(enterDamageModifier, exitDamageModifier);
			combinedPenetrationModifier = fminf(enterSurfPenetrationModifier, exitSurfPenetrationModifier);
		}

		if (enterMaterial == exitMaterial && (exitMaterial == CHAR_TEX_METAL || exitMaterial == CHAR_TEX_WOOD))
			combinedPenetrationModifier += combinedPenetrationModifier;

		thickness = (exitTrace.endpos - enterTrace.endpos).LengthSqr();

		if (sqrt(thickness) <= combinedPenetrationModifier * penetrationPower)
		{
			currentDamage *= finalDamageModifier;
			eyePosition = exitTrace.endpos;
			--possibleHitsRemaining;
			handle_penetration = true;
			return true;
		}
		handle_penetration = false;
		return false;
	}
}

bool autowall_2::FireBullet(C_BaseCombatWeapon* pWeapon, Vector& direction, float& currentDamage)
{
	if (!pWeapon)
		return false;

	IClientEntity* local = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	auto data = FireBulletData(local->GetEyePosition());

	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	bool sv_penetration_type;

	float currentDistance = 0.f, penetrationPower, penetrationDistance, maxRange, ff_damage_reduction_bullets, ff_damage_bullet_penetration, rayExtension = 40.f;
	Vector eyePosition = local->GetEyePosition();

	static ConVar* penetrationSystem = Interfaces::CVar->FindVar(("sv_penetration_type"));
	static ConVar* damageReductionBullets = Interfaces::CVar->FindVar(("ff_damage_reduction_bullets"));
	static ConVar* damageBulletPenetration = Interfaces::CVar->FindVar(("ff_damage_bullet_penetration"));

	sv_penetration_type = penetrationSystem->GetBool();
	ff_damage_reduction_bullets = damageReductionBullets->GetFloat();
	ff_damage_bullet_penetration = damageBulletPenetration->GetFloat();

	damage_reduction = ff_damage_bullet_penetration;

	CSWeaponInfo* weaponData = pWeapon->GetCSWpnData();
	CGameTrace enterTrace;
	CTraceFilter filter;

	filter.pSkip = local;

	if (!weaponData)
		return false;

	maxRange = weaponData->range;

	GetBulletTypeParameters(penetrationPower, penetrationDistance, weaponData->hud_name, sv_penetration_type);

	if (sv_penetration_type)
		penetrationPower = weaponData->penetration;

	int possibleHitsRemaining = 4;

	currentDamage = weaponData->damage;

	while (possibleHitsRemaining > 0 && currentDamage >= 1.f)
	{
		maxRange -= currentDistance;

		Vector end = eyePosition + direction * maxRange;

		TraceLine(eyePosition, end, MASK_SHOT_HULL | CONTENTS_HITBOX, local, &enterTrace);
		ClipTraceToPlayers(eyePosition, end + direction * rayExtension, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &enterTrace); //  | CONTENTS_HITBOX

		surfacedata_t *enterSurfaceData = Interfaces::PhysProps->GetSurfaceData(enterTrace.surface.surfaceProps);

		if (enterSurfaceData == nullptr)
			return false;

		float enterSurfPenetrationModifier = enterSurfaceData->game.flPenetrationModifier;

		int enterMaterial = enterSurfaceData->game.material;

		if (enterTrace.fraction == 1.f)
			break;

		currentDistance += enterTrace.fraction * maxRange;

		currentDamage *= pow(weaponData->range_modifier, (currentDistance / 500.f));

		if (currentDistance > penetrationDistance && weaponData->penetration > 0.f || enterSurfPenetrationModifier < 0.1f)
			break;

		bool canDoDamage = (enterTrace.hitgroup != HITGROUP_GEAR && enterTrace.hitgroup != HITGROUP_GENERIC);
		bool isEnemy = (((IClientEntity*)local)->team() != ((IClientEntity*)enterTrace.m_pEnt)->team());

		if ((canDoDamage && isEnemy))
		{
			ScaleDamage(enterTrace, weaponData, currentDamage);
			return true;
		}

		if (!HandleBulletPenetration(weaponData, enterTrace, eyePosition, direction, possibleHitsRemaining, currentDamage, penetrationPower, sv_penetration_type, ff_damage_reduction_bullets, ff_damage_bullet_penetration))
			break;
	}
	return false;
}

////////////////////////////////////// Usage Calls //////////////////////////////////////
float autowall_2::CanHit(Vector &point)
{
	IClientEntity* local = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local || !local->IsAlive())
		return -1;

	auto data = FireBulletData(local->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;
	Vector angles, direction;
	Vector tmp = point - local->GetEyePosition();
	float currentDamage = 0;

	VectorAngles(tmp, angles);
	AngleVectors(angles, &direction);
	direction.NormalizeInPlace();

	if (FireBullet(local->GetWeapon2(), direction, currentDamage))
		return currentDamage;
	return -1;
}

bool autowall_2::PenetrateWall(IClientEntity* pBaseEntity, Vector& vecPoint)
{
	float min_damage = Options::Menu.RageBotTab.AccuracyMinimumDamage.GetValue();

	if (pBaseEntity->GetHealth() < min_damage)
		min_damage = pBaseEntity->GetHealth();

	if (CanHit(vecPoint) >= min_damage)
		return true;

	return false;
}
void trace_line(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
float autowall_2::trace_awall(float damage)
{
	auto m_local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (m_local && m_local->IsAlive()) {
		FireBulletData data = FireBulletData(m_local->GetEyePosition());
		data.filter = CTraceFilter();
		data.filter.pSkip = m_local;
		Vector eye_angle; Interfaces::Engine->GetViewAngles(eye_angle);
		Vector dst, forward;
		AngleVectors(eye_angle, &forward);
		dst = data.src + (forward * 8196.f);
		Vector angles;
		CalcAngle(data.src, dst, angles);
		AngleVectors(angles, &data.direction);
		VectorNormalize(data.direction);
		auto m_weapon = m_local->GetWeapon2();
		if (m_weapon) {
			data.penetrate_count = 1;
			data.trace_length = 0.0f;
			CSWeaponInfo *weapon_data = m_weapon->GetCSWpnData();
			if (weapon_data) {
				data.current_damage = weapon_data->damage;
				data.trace_length_remaining = weapon_data->range - data.trace_length;
				Vector end = data.src + data.direction * data.trace_length_remaining;
				trace_line(data.src, end, MASK_SHOT | CONTENTS_GRATE, m_local, 0, &data.enter_trace);
				if (data.enter_trace.fraction == 1.0f) return false;
				if (handle_penetration) {
					damage = data.current_damage;
					return true;
				}
			}
		}
	}
	return false;
}
*/

float backup_autowall::GetHitgroupDamageMult_2(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 0.5f;
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_CHEST:
		return 0.75f;
	case HITGROUP_STOMACH:
		return 0.75f;
	case HITGROUP_LEFTARM:
		return 0.5f;
	case HITGROUP_RIGHTARM:
		return 0.5f;
	case HITGROUP_LEFTLEG:
		return 0.375f;
	case HITGROUP_RIGHTLEG:
		return 0.375f;
	case HITGROUP_GEAR:
		return 0.5f;
	default:
		return 1.0f;

	}

	return 1.0f;
}

void backup_autowall::ScaleDamage_2(int hitgroup, IClientEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult_2(hitgroup);

//	if (enemy->ArmorValue() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= (weapon_armor_ratio);
		}
		else
		{
			current_damage *= (weapon_armor_ratio);
		}
	}
}


void angle_vectors_2(const Vector &angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

bool backup_autowall::HandleBulletPenetration_2(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	
	surfacedata_t *enter_surface_data = Interfaces::PhysProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow(wpn_data->range_modifier, (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;

	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit)) 
		return false;

	surfacedata_t *exit_surface_data = Interfaces::PhysProps->GetSurfaceData(trace_exit.surface.surfaceProps);

	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71)) 
	{ 
		combined_penetration_modifier = 3.0f; final_damage_modifier = 0.05f; 
	}
	else 
	{ 
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f; 
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->penetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)return false;
	if (lost_damage >= 0.0f)data.current_damage -= lost_damage;
	if (data.current_damage < 1.0f) return false;
	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

bool backup_autowall::SimulateFireBullet_2(IClientEntity *local, C_BaseCombatWeapon *weapon, FireBulletData &data)
{

	data.penetrate_count = 4; // Max Amount Of Penitration
	data.trace_length = 0.0f; // wow what a meme

	if (!weapon)
		return false;

	if (!&data)
		return false;


		auto *wpn_data = weapon->GetCSWpnData(); // Get Weapon Info

		if (!wpn_data || wpn_data == nullptr)
			return false;

		data.current_damage = (float)wpn_data->damage;// Set Damage Memes
		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = wpn_data->range - data.trace_length;
			Vector End_Point = data.src + data.direction * data.trace_length_remaining;
			UTIL_TraceLine(data.src, End_Point, 0x4600400B, local, 0, &data.enter_trace);
			UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

			if (data.enter_trace.fraction == 1.0f)
				break;

			if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && local->team() != data.enter_trace.m_pEnt->team())
			{
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow(wpn_data->range, data.trace_length * 0.002);
				ScaleDamage_2(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage);
				return true;
			}
			if (!HandleBulletPenetration_2(wpn_data, data))
				break;
		}
	
	return false;
}
bool backup_autowall::CanHit_2(const Vector &point, float *damage_given)
{
	auto *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto data = FireBulletData(local->GetOrigin() + local->GetViewOffset());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	Vector angles;
	CalcAngle(data.src, point, angles);
	angle_vectors_2(angles, data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet_2(local, (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle((HANDLE)local->GetActiveWeaponHandle()), data))
	{
		*damage_given = data.current_damage;
		return true;
	}

	return false;
}
