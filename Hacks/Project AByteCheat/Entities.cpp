#include "Entities.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
 float intervalPerTick;
#define TICK_INTERVAL			(  )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / intervalPerTick ) )


C_BaseCombatWeapon* IClientEntity::GetWeapon2()
{
	static int iOffset = GET_NETVAR("DT_BaseCombatCharacter", "m_hActiveWeapon");
	ULONG pWeepEhandle = *(PULONG)((DWORD)this + iOffset);
	return (C_BaseCombatWeapon*)(interfaces::ent_list->GetClientEntityFromHandle((HANDLE)pWeepEhandle));
}

int IClientEntity::get_choked_ticks()
{
	if (!this || !this->IsAlive()) 
		return 0;

	float flSimulationTime = this->GetSimulationTime();
	float flOldSimulationTime = this->m_flOldSimulationTime();
	float flSimDiff = flSimulationTime - flOldSimulationTime;

	return TIME_TO_TICKS(max(0, flSimDiff /*- latency*/));
}

bool IClientEntity::IsKnifeorNade()
{
	if (!this)
		return false;
	if (!this->IsAlive())
		return false;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetWeapon2();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_knife")
		return true;
	else if (WeaponName == "weapon_incgrenade")
		return true;
	else if (WeaponName == "weapon_decoy")
		return true;
	else if (WeaponName == "weapon_flashbang")
		return true;
	else if (WeaponName == "weapon_hegrenade")
		return true;
	else if (WeaponName == "weapon_smokegrenade")
		return true;
	else if (WeaponName == "weapon_molotov")
		return true;

	return false;
}

inline Vector ExtrapolateTick(Vector p0, Vector v0)
{
	return p0 + (v0 *intervalPerTick);
}
Vector IClientEntity::GetPredicted(Vector p0)
{
	return ExtrapolateTick(p0, this->GetVelocity());
}
void IClientEntity::CopyPoseParameters(float* dest)
{
	float* flPose = (float*)((DWORD)this + 0x2764);
	memcpy(dest, flPose, sizeof(float) * 24);
}

void IClientEntity::CopyAnimLayers(CAnimationLayer* dest)
{
	int count = 15;
	for (int i = 0; i < count; i++)
	{
		dest[i] = GetAnimOverlayX(i);
	}
}
Vector IClientEntity::GetBonePosAIZ(int i)
{
	matrix3x4 boneMatrix[128];
	if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, interfaces::engine->GetLastTimeStamp()))
	{
		return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
	}
	return Vector(0, 0, 0);
}
bool IClientEntity::has_gungame_immunity()
{
	static int m_bGunGameImmunity = GET_NETVAR("DT_CSPlayer", "m_bGunGameImmunity");
	return *(bool*)((DWORD)this + m_bGunGameImmunity);
}


























































































































































































