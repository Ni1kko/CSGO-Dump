#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	struct Weapon_Info;
	class CSWeaponInfo;
	struct mstudiobbox_t;
	struct mstudiobbox_t;
}

struct FireBulletData
{
	FireBulletData(const Vector &eye_pos) : src(eye_pos)
	{
	}

	Vector						src;
	SDK::trace_t       enter_trace;
	Vector						direction;
	SDK::CTraceFilter  filter;
	float						trace_length;
	float						trace_length_remaining;
	float						current_damage;
	int							penetrate_count;
};

class CAutowall
{
public:

	void UTIL_TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, SDK::CBaseEntity *ignore, SDK::trace_t *ptr)
	{
		SDK::Ray_t ray;

		SDK::CTraceFilter filter;
		filter.pSkip1 = ignore;

		ray.Init(vecAbsStart, vecAbsEnd);

		INTERFACES::Trace->TraceRay(ray, mask, &filter, ptr);
	}
	void trace_line_esp(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, SDK::CBaseEntity *ignore, int collisionGroup, SDK::trace_t *ptr)
	{
		SDK::Ray_t ray;

		SDK::CTraceFilter filter;
		filter.pSkip1 = ignore;

		ray.Init(vecAbsStart, vecAbsEnd);

		INTERFACES::Trace->TraceRay(ray, mask, &filter, ptr);
	}
	void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, SDK::ITraceFilter* filter, SDK::trace_t* tr)
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
			LEA        EDX, vecAbsEnd
			LEA        ECX, vecAbsStart
			CALL    dwAddress
			ADD        ESP, 0xC
		}
	}








	void TraceLine(Vector & absStart, Vector & absEnd, unsigned int mask, SDK::CBaseEntity * ignore, SDK::trace_t * ptr);

	void ClipTraceToPlayers(Vector & absStart, Vector absEnd, unsigned int mask, SDK::ITraceFilter * filter, SDK::trace_t * tr);

	void GetBulletTypeParameters(float & maxRange, float & maxDistance, char * bulletType, bool sv_penetration_type);

	bool IsBreakableEntity(SDK::CBaseEntity * entity);

	void ScaleDamage(SDK::trace_t & enterTrace, SDK::CSWeaponInfo * weaponData, float & currentDamage);

	bool TraceToExit(SDK::trace_t & enterTrace, SDK::trace_t & exitTrace, Vector startPosition, Vector direction);

	bool HandleBulletPenetration(SDK::CSWeaponInfo * weaponData, SDK::trace_t & enterTrace, Vector & eyePosition, Vector direction, int & possibleHitsRemaining, float & currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration);

	bool FireBullet(SDK::CBaseWeapon * pWeapon, Vector & direction, float & currentDamage);

	float CanHit(Vector & vecEyePos, Vector & point);

	float CanHit(Vector & point);

	float CanHit1(Vector & point, SDK::CBaseEntity * pBaseEntity);

	float CanHit1(Vector & point);

	bool PenetrateWall(SDK::CBaseEntity * pBaseEntity, Vector & vecPoint);

	bool CanWallbang(float & dmg);
};

extern CAutowall* AutoWall;