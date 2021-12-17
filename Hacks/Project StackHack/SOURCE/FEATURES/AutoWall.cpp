#include "../includes.h"

#include "../UTILS/interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/IEngine.h"
#include "../SDK/SurfaceData.h"
#include "../SDK/CTrace.h"
#include <Windows.h>
#include <math.h>
#include "../FEATURES/AutoWall.h"

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()


static bool is_autowalling = false;


CAutowall* AutoWall = new CAutowall;

float CAutowall::GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
	{
		return 4.0f;
	}
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
	{
		return 1.0f;
	}
	case HITGROUP_STOMACH:
	{
		return 1.25f;
	}
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
	{
		return 0.75f;
	}
	}
	return 1.0f;
}



void CAutowall::ScaleDamage(SDK::CBaseEntity* entity, SDK::Weapon_Info* weapon_info, int hitgroup, float& current_damage)
{
	//Cred. to N0xius for reversing this.
	//TODO: _xAE^; look into reversing this yourself sometime

	bool hasHeavyArmor = false;
	int armorValue = entity->GetArmor();

	//Fuck making a new function, lambda beste. ~ Does the person have armor on for the hitbox checked?
	auto IsArmored = [&entity, &hitgroup]()-> bool
	{
		SDK::CBaseEntity* targetEntity = entity;
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			return targetEntity->HasHelmet();
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

	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		current_damage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if (armorValue > 0 && IsArmored())
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weapon_info->m_ArmorRatio / 2.f;

		//Damage gets modified for heavy armor users
		if (hasHeavyArmor)
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = current_damage * armorRatio;

		if (hasHeavyArmor)
			NewDamage *= 0.85f;

		if (((current_damage - (current_damage * armorRatio)) * (bonusValue * armorBonusRatio)) > armorValue)
			NewDamage = current_damage - (armorValue / armorBonusRatio);

		current_damage = NewDamage;
	}
}



CAutowall::Autowall_Return_Info CAutowall::CalculateDamage(Vector start, Vector end, SDK::CBaseEntity* from_entity, SDK::CBaseEntity* to_entity, int specific_hitgroup)
{

	// default values for return info, in case we need to return abruptly
	Autowall_Return_Info return_info;
	return_info.damage = -1;
	return_info.hitgroup = -1;
	return_info.hit_entity = nullptr;
	return_info.penetration_count = 4;
	return_info.thickness = 0.f;
	return_info.did_penetrate_wall = false;

	FireBulletData autowall_info;
	autowall_info.penetration_count = 4;
	autowall_info.start = start;
	autowall_info.end = end;
	autowall_info.current_position = start;
	autowall_info.thickness = 0.f;

	// direction 
	MATH::AngleVectors(UTILS::CalcAngle(start, end), &autowall_info.direction);

	// attacking entity
	if (!from_entity)
		from_entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!from_entity)
		return return_info;

	auto filter_player = SDK::CTraceFilterOneEntity();
	filter_player.pEntity = to_entity;
	auto filter_local = SDK::CTraceFilter();
	filter_local.pSkip1 = from_entity;

	// setup filters
	if (to_entity)
		autowall_info.f0lter = &filter_player;
	else
		autowall_info.f0lter = &filter_player;

	// weapon
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(from_entity->GetActiveWeaponIndex()));
	if (!weapon)
		return return_info;

	// weapon data
	auto weapon_info = weapon->GetWeaponInfo();
	if (!weapon_info)
		return return_info;

	// client class
	auto weapon_client_class = reinterpret_cast<SDK::CBaseEntity*>(weapon)->GetClientClass();
	if (!weapon_client_class)
		return return_info;

	// weapon range
	float range = min(weapon_info->m_Range, (start - end).Length());
	end = start + (autowall_info.direction * range);
	autowall_info.current_damage = weapon_info->m_Damage;

	while (autowall_info.current_damage > 0 && autowall_info.penetration_count > 0)
	{
		return_info.penetration_count = autowall_info.penetration_count;

		UTIL_TraceLine(autowall_info.current_position, end, MASK_SHOT | CONTENTS_GRATE, from_entity, &autowall_info.enter_trace);
		UTIL_ClipTraceToPlayers(autowall_info.current_position, autowall_info.current_position + autowall_info.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, autowall_info.f0lter, &autowall_info.enter_trace);

		const float distance_traced = (autowall_info.enter_trace.end - start).Length();
		autowall_info.current_damage *= pow(weapon_info->m_RangeModifier, (distance_traced / 500.f));

		/// if reached the end
		if (autowall_info.enter_trace.flFraction == 1.f)
		{
			if (to_entity && specific_hitgroup != -1)
			{
				ScaleDamage(to_entity, weapon_info, specific_hitgroup, autowall_info.current_damage);

				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = specific_hitgroup;
				return_info.end = autowall_info.enter_trace.end;
				return_info.hit_entity = to_entity;
			}
			else
			{
				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = -1;
				return_info.end = autowall_info.enter_trace.end;
				return_info.hit_entity = nullptr;
			}

			break;
		}
		// if hit an entity
		if (autowall_info.enter_trace.hitGroup > 0 && autowall_info.enter_trace.hitGroup <= 7 && autowall_info.enter_trace.m_pEnt)
		{
			// checkles gg
			if ((to_entity && autowall_info.enter_trace.m_pEnt != to_entity) ||
				(autowall_info.enter_trace.m_pEnt->GetTeam() == from_entity->GetTeam()))
			{
				return_info.damage = -1;
				return return_info;
			}

			if (specific_hitgroup != -1)
				ScaleDamage(autowall_info.enter_trace.m_pEnt, weapon_info, specific_hitgroup, autowall_info.current_damage);
			else
				ScaleDamage(autowall_info.enter_trace.m_pEnt, weapon_info, autowall_info.enter_trace.hitGroup, autowall_info.current_damage);

			// fill the return info
			return_info.damage = autowall_info.current_damage;
			return_info.hitgroup = autowall_info.enter_trace.hitGroup;
			return_info.end = autowall_info.enter_trace.end;
			return_info.hit_entity = autowall_info.enter_trace.m_pEnt;

			break;
		}

		// break out of the loop retard
		if (!CanPenetrate(from_entity, autowall_info, weapon_info))
			break;

		return_info.did_penetrate_wall = true;
	}

	return_info.penetration_count = autowall_info.penetration_count;

	return return_info;
}

SDK::CBaseWeapon* GetWeapon(SDK::CBaseEntity* entity)
{
	return reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
}

bool IsWeaponTaser(SDK::CBaseWeapon* weapon)
{
	if (!weapon)
		return false;

	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_TASER)
		return true;

	return false;
}

bool CAutowall::CanPenetrate(SDK::CBaseEntity* attacker, FireBulletData& info, SDK::Weapon_Info* weapon_data)
{
	//typedef bool(__thiscall* HandleBulletPenetrationFn)(SDK::CBaseEntity*, float&, int&, int*, SDK::trace_t*, Vector*, float, float, float, int, int, float, int*, Vector*, float, float, float*);
	//CBaseEntity *pLocalPlayer, float *flPenetration, int *SurfaceMaterial, char *IsSolid, trace_t *ray, Vector *vecDir, int unused, float flPenetrationModifier, float flDamageModifier, int unused2, int weaponmask, float flPenetration2, int *hitsleft, Vector *ResultPos, int unused3, int unused4, float *damage)
	typedef bool(__thiscall* HandleBulletPenetrationFn)(SDK::CBaseEntity*, float*, int*, char*, SDK::trace_t*, Vector*, int, float, float, int, int, float, int*, Vector*, int, int, float*);
	static HandleBulletPenetrationFn HBP = reinterpret_cast<HandleBulletPenetrationFn>(UTILS::FindPattern("client.dll",
		(PBYTE)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF8\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x83\xEC\x78\x8B\x53\x14",
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxx"));

	auto enter_surface_data = INTERFACES::PhysicsProps->GetSurfaceData(info.enter_trace.surface.surfaceProps);
	if (!enter_surface_data)
		return true;

	char is_solid = 0;
	int material = enter_surface_data->game.material;
	int mask = IsWeaponTaser(GetWeapon(attacker)) ? 0x1100 : 0x1002;

	// glass and shit gg
	if (info.enter_trace.m_pEnt && !strcmp(enc_char("CBreakableSurface"),
		info.enter_trace.m_pEnt->GetClientClass()->m_pNetworkName))
		*reinterpret_cast<byte*>(uintptr_t(info.enter_trace.m_pEnt + 0x27C)) = 2;

	is_autowalling = true;
	bool return_value = !HBP(attacker, &weapon_data->m_Penetration, &material, &is_solid, &info.enter_trace, &info.direction, 0, enter_surface_data->game.flPenetrationModifier, enter_surface_data->game.flDamageModifier, 0, mask, weapon_data->m_Penetration, &info.penetration_count, &info.current_position, 0, 0, &info.current_damage);
	is_autowalling = false;
	return return_value;
}

void CAutowall::ScaleDamage(int hitgroup, SDK::CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!SETTINGS::settings.friendfire && enemy->GetTeam() == local_player->GetTeam())
	{
		current_damage = 0;
		return;
	}

	int armor = enemy->GetArmor();
	int helmet = enemy->HasHelmet();

	if (armor > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (helmet)
				current_damage *= (weapon_armor_ratio * .5f);
		}
		else
		{
			current_damage *= (weapon_armor_ratio * .5f);
		}
	}
}

bool CAutowall::DidHitNonWorldEntity(SDK::CBaseEntity* m_pEnt)
{
	return m_pEnt != NULL && m_pEnt == INTERFACES::ClientEntityList->GetClientEntity(0);
}

bool CAutowall::TraceToExit(Vector& end, SDK::trace_t& tr, float x, float y, float z, float x2, float y2, float z2, SDK::trace_t* trace)
{

	typedef bool(__fastcall* TraceToExitFn)(Vector&, SDK::trace_t&, float, float, float, float, float, float, SDK::trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 EC 30 F3 0F 10 75");
	if (!TraceToExit)
	{
		return false;
	}//(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	_asm
	{
		push trace
		push z2
		push y2
		push x2
		push z
		push y
		push x
		mov edx, tr
		mov ecx, end
		call TraceToExit
		add esp, 0x1C
	}
}

bool CAutowall::HandleBulletPenetration(SDK::CSWeaponInfo *wpn_data, FireBulletData &data)
{
	SDK::surfacedata_t *enter_surface_data = INTERFACES::PhysicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = *(float*)((DWORD)enter_surface_data + 80);

	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= pow((wpn_data->m_RangeModifier), (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	SDK::trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.end.x, data.enter_trace.end.y, data.enter_trace.end.z, data.direction.x, data.direction.y, data.direction.z, &trace_exit))
		return false;




	SDK::surfacedata_t *exit_surface_data = INTERFACES::PhysicsProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = *(float*)((DWORD)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
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
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_Penetration) * 1.25f);
	float thickness = (trace_exit.end - data.enter_trace.end).Length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.end;
	data.penetrate_count--;

	return true;
}

inline float VectorNormalizeStick(Vector& v)
{
	Assert(v.IsValid());
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		// FIXME:
		// Just copying the existing implemenation; shouldn't res.z == 0?
		v.x = v.y = 0.0f; v.z = 1.0f;
	}
	return l;
}

void LARE(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}





bool CAutowall::SimulateFireBullet(FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));


	// weapon data
	auto weaponData = weapon->get_full_info();

	if (weaponData == NULL)
		return false;

	data.current_damage = (float)weaponData->m_Damage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weaponData->m_Range - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;



		UTIL_TraceLine(data.src, end, 0x4600400B, local_player, &data.enter_trace);
		UTIL_ClipTraceToPlayers(data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

		if (data.enter_trace.flFraction == 1.0f)
			break;

		if ((data.enter_trace.hitGroup <= 7) && (data.enter_trace.hitGroup > 0))
		{
			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= pow(weaponData->m_RangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitGroup, data.enter_trace.m_pEnt, weaponData->m_ArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponData, data))
			break;
	}

	return false;
}
void VectorAnglesxd(const Vector& forward, QAngle &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
}
QAngle CalcAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	VectorAnglesxd(delta, angles);
	delta.Normalize();
	return angles;
}
void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}
void AngleVectors(const QAngle &angles, Vector *forward)
{
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}
void AngleVectorsUwU(const Vector &angles, Vector *forward)
{
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}
void CalcAngleMore(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D(); //delta.Length
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}

bool CAutowall::CanWallbangy(float &dmg)
{
	const Vector point;
	Vector dst = point;
	FireBulletData data;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	data.src = local_player->GetEyePosition();
	data.filter.pSkip1 = local_player;

	//data.filter.pSkip = G::LocalPlayer;

	Vector angles;
	CalcAngleMore(data.src, dst, angles);
	AngleVectorsUwU(angles, &data.direction);
	Vector dataNormalized;

	SDK::CBaseWeapon* weapon = (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandle((HANDLE)local_player->GetActiveWeaponIndex());

	if (!weapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	SDK::CSWeaponInfo *weaponData = weapon->get_full_info();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->m_Damage;

	data.trace_length_remaining = weaponData->m_Range - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	UTIL_TraceLine2(data.src, end, MASK_SHOT | CONTENTS_GRATE, local_player, 0, &data.enter_trace);

	if (data.enter_trace.flFraction == 1.0f)
		return false;

	//data.direction.Normalize();

	//auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (HandleBulletPenetration(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}
float CAutowall::GetDamagez(const Vector& point)
{
	float damage = 0.f;
	Vector dst = point;
	FireBulletData data;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	data.src = local_player->GetEyePosition();
	data.filter.pSkip1 = local_player;

	//data.filter.pSkip = G::LocalPlayer;

	QAngle angles = CalcAngle(data.src, dst);
	AngleVectors(angles, &data.direction);
	Vector dataNormalized;

	data.direction.Normalize();

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (SimulateFireBullet(data))
	{
		damage = data.current_damage;
	}

	return damage;
}
















































































































































































































































































