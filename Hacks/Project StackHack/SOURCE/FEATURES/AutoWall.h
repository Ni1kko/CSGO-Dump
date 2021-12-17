#pragma once
#include "../SDK/CTrace.h"
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	struct Weapon_Info;
	class CSWeaponInfo;
}

class CAutowall
{
public:
	bool CanWallbangy(float & dmg);
	float GetDamagez(const Vector& vecPoint);

	bool CanHit_2(const Vector & point, float * damage_given);

	struct Autowall_Return_Info
	{
		int damage;
		int hitgroup;
		int penetration_count;
		bool did_penetrate_wall;
		float thickness;
		Vector end;
		SDK::CBaseEntity* hit_entity;
	};
	CAutowall::Autowall_Return_Info CalculateDamage(Vector start, Vector end, SDK::CBaseEntity * from_entity, SDK::CBaseEntity * to_entity, int specific_hitgroup);
	void UTIL_TraceLine2(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, SDK::CBaseEntity *ignore, int collisionGroup, SDK::trace_t *ptr)
	{
		SDK::Ray_t ray;

		SDK::CTraceFilter filter;
		filter.pSkip1 = ignore;

		ray.Init(vecAbsStart, vecAbsEnd);

		INTERFACES::Trace->TraceRay(ray, mask, &filter, ptr);
	}
	void UTIL_TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, SDK::CBaseEntity *ignore, SDK::trace_t *ptr)
	{
		SDK::Ray_t ray;

		SDK::CTraceFilter filter;
		filter.pSkip1 = ignore;

		ray.Init(vecAbsStart, vecAbsEnd);

		INTERFACES::Trace->TraceRay(ray, mask, &filter, ptr);
	}
	void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, SDK::ITraceFilter* filter, SDK::trace_t* tr)
	{
		static DWORD dwAddress = UTILS::FindSignature("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");
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
	struct FireBulletData
	{
		Vector start;
		Vector current_position;
		Vector end;
		Vector          src;
		SDK::trace_t         enter_trace;
		Vector          direction;
		SDK::ITraceFilter* f0lter;
		SDK::CTraceFilter    filter;
		float           trace_length;
		float           trace_length_remaining;

		float			thickness;
		float           current_damage;
		int             penetrate_count;
		int penetration_count;
	};

	float GetDamage(const Vector& vecPoint, FireBulletData& fData);
	bool SimulateFireBullet(FireBulletData &data);
	bool CanPenetrate(SDK::CBaseEntity* attacker, FireBulletData& info, SDK::Weapon_Info* weapon_data);
	bool HandleBulletPenetration(SDK::CSWeaponInfo *wpn_data, FireBulletData &data);
	float CanHit(Vector & point);
	bool TraceToExit(Vector& end, SDK::trace_t& tr, float x, float y, float z, float x2, float y2, float z2, SDK::trace_t* trace);
	bool DidHitNonWorldEntity(SDK::CBaseEntity* m_pEnt);
	void ScaleDamage(int hitgroup, SDK::CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage);
	float GetHitgroupDamageMultiplier(int iHitGroup);

	void ScaleDamage(SDK::CBaseEntity * entity, SDK::Weapon_Info * weapon_info, int hitgroup, float & current_damage);

	bool CanHitFloatingPoint(const Vector & point, const Vector & source);

};

extern CAutowall* AutoWall;