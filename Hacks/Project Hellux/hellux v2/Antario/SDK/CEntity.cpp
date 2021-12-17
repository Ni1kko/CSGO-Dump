#include "CEntity.h"
#include "IVModelInfo.h"
#include "CGlobalVarsBase.h"
#include "..\Utils\GlobalVars.h"
#include "..\Utils\Math.h"

Vector C_BaseEntity::GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius)
{
	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(this->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (hitbox)
	{
		Vector vMin, vMax, vCenter, sCenter;
		g_Math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
		g_Math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
		vCenter = (vMin + vMax) * 0.5;

		*Radius = hitbox->radius;

		return vCenter;
	}
	
	return Vector(0, 0, 0);
}

Vector C_BaseEntity::GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix)
{
	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(this->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (hitbox)
	{
		Vector vMin, vMax, vCenter, sCenter;
		g_Math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
		g_Math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
		vCenter = (vMin + vMax) * 0.5;

		return vCenter;
	}

	return Vector(0, 0, 0);
}

float C_BaseEntity::FireRate()
{
	if (!this)
		return 0.f;
	if (!this->IsAlive())
		return 0.f;
	if (this->IsKnifeorNade())
		return 0.f;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_glock")
		return 0.15f;
	else if (WeaponName == "weapon_hkp2000")
		return 0.169f;
	else if (WeaponName == "weapon_p250")//the cz and p250 have the same name idky same with other guns
		return 0.15f;
	else if (WeaponName == "weapon_tec9")
		return 0.12f;
	else if (WeaponName == "weapon_elite")
		return 0.12f;
	else if (WeaponName == "weapon_fiveseven")
		return 0.15f;
	else if (WeaponName == "weapon_deagle")
		return 0.224f;
	else if (WeaponName == "weapon_nova")
		return 0.882f;
	else if (WeaponName == "weapon_sawedoff")
		return 0.845f;
	else if (WeaponName == "weapon_mag7")
		return 0.845f;
	else if (WeaponName == "weapon_xm1014")
		return 0.35f;
	else if (WeaponName == "weapon_mac10")
		return 0.075f;
	else if (WeaponName == "weapon_ump45")
		return 0.089f;
	else if (WeaponName == "weapon_mp9")
		return 0.070f;
	else if (WeaponName == "weapon_bizon")
		return 0.08f;
	else if (WeaponName == "weapon_mp7")
		return 0.08f;
	else if (WeaponName == "weapon_p90")
		return 0.070f;
	else if (WeaponName == "weapon_galilar")
		return 0.089f;
	else if (WeaponName == "weapon_ak47")
		return 0.1f;
	else if (WeaponName == "weapon_sg556")
		return 0.089f;
	else if (WeaponName == "weapon_m4a1")
		return 0.089f;
	else if (WeaponName == "weapon_aug")
		return 0.089f;
	else if (WeaponName == "weapon_m249")
		return 0.08f;
	else if (WeaponName == "weapon_negev")
		return 0.0008f;
	else if (WeaponName == "weapon_ssg08")
		return 1.25f;
	else if (WeaponName == "weapon_awp")
		return 1.463f;
	else if (WeaponName == "weapon_g3sg1")
		return 0.25f;
	else if (WeaponName == "weapon_scar20")
		return 0.25f;
	else if (WeaponName == "weapon_mp5sd")
		return 0.08f;
	else
		return .0f;

}
void C_BaseEntity::FixSetupBones(matrix3x4_t *Matrix){ // i took out alot of stuff from here because b1g secrets
static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
static int m_nForceBone = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nForceBone");

if (this == Globals::LocalPlayer)
{
	const auto Backup = *(int*)(uintptr_t(this) + ptrdiff_t(0x272));

	*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = -1;

	this->SetupBones(Matrix, 126, 0x00000100 | 0x200, g_pGlobalVars->curtime);

	*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = Backup;
}
else
{
	*reinterpret_cast<int*>(uintptr_t(this) + 0xA30) = g_pGlobalVars->framecount;
	*reinterpret_cast<int*>(uintptr_t(this) + 0xA28) = 0;

	const auto Backup = *(int*)(uintptr_t(this) + ptrdiff_t(0x272));

	*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = -1;

	this->SetupBones(Matrix, 126, 0x00000100 | 0x200, g_pGlobalVars->curtime);

	*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = Backup;
}
}

void C_BaseEntity::SetAbsAngles(Vector angles)
{
	using Fn = void(__thiscall*)(C_BaseEntity*, const Vector& angles);
	static Fn AbsAngles = (Fn)(Utils::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

	AbsAngles(this, angles);
}

void C_BaseEntity::SetAbsOrigin(Vector origin)
{
	using Fn = void(__thiscall*)(void*, const Vector &origin);
	static Fn AbsOrigin = (Fn)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");

	AbsOrigin(this, origin);
}

void C_BaseEntity::InvalidateBoneCache()
{
	static auto m_nForceBone = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nForceBone");

	static DWORD InvalidateBoneCache = Utils::FindPattern("client_panorama", (PBYTE)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1", "xx????xxxx");
	static unsigned long g_iModelBoneCounter = (unsigned long)(InvalidateBoneCache + 10);
	*(int*)((DWORD)this + m_nForceBone + 0x20) = 0;
	*(unsigned int*)((DWORD)this + 0x2914) = -FLT_MAX;
	*(unsigned int*)((DWORD)this + 0x2680) = (g_iModelBoneCounter - 1);
}

void C_BaseEntity::SetAbsVelocity(Vector velocity)
{
	using Fn = void(__thiscall*)(void*, const Vector &velocity);
	static Fn AbsVelocity = (Fn)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1 F3");

	AbsVelocity(this, velocity);
}

bool C_BaseEntity::IsKnifeorNade()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

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

bool C_BaseEntity::IsKnife()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_knife")
		return true;

	return false;
}

bool C_BaseEntity::IsPistol()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	ItemDefinitionIndex WeapID = pWeapon->GetItemDefinitionIndex();

	return WeapID == ItemDefinitionIndex::WEAPON_DEAGLE || WeapID == ItemDefinitionIndex::WEAPON_ELITE || WeapID == ItemDefinitionIndex::WEAPON_FIVESEVEN || WeapID == ItemDefinitionIndex::WEAPON_P250 ||
		WeapID == ItemDefinitionIndex::WEAPON_GLOCK || WeapID == ItemDefinitionIndex::WEAPON_USP_SILENCER || WeapID == ItemDefinitionIndex::WEAPON_CZ75A || WeapID == ItemDefinitionIndex::WEAPON_HKP2000 || WeapID == ItemDefinitionIndex::WEAPON_TEC9 || WeapID == ItemDefinitionIndex::WEAPON_REVOLVER;
}

bool C_BaseEntity::IsShotgun()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	ItemDefinitionIndex WeapID = pWeapon->GetItemDefinitionIndex();

	return WeapID == ItemDefinitionIndex::WEAPON_XM1014 || WeapID == ItemDefinitionIndex::WEAPON_NOVA || WeapID == ItemDefinitionIndex::WEAPON_SAWEDOFF || WeapID == ItemDefinitionIndex::WEAPON_MAG7;
}

bool C_BaseEntity::IsSniper()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	ItemDefinitionIndex WeapID = pWeapon->GetItemDefinitionIndex();

	return WeapID == ItemDefinitionIndex::WEAPON_AWP || WeapID == ItemDefinitionIndex::WEAPON_SCAR20 || WeapID == ItemDefinitionIndex::WEAPON_G3SG1 || WeapID == ItemDefinitionIndex::WEAPON_SSG08;
}


bool C_BaseEntity::IsScopableWeapon()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_scar20")
		return true;
	else if (WeaponName == "weapon_g3sg1")
		return true;
	else if (WeaponName == "weapon_ssg08")
		return true;
	else if (WeaponName == "weapon_aug")
		return true;
	else if (WeaponName == "weapon_sg553")
		return true;

	return false;
}
int C_BaseEntity::GetSequenceActivity(int sequence)
{
	auto hdr = g_pModelInfo->GetStudiomodel(this->GetModel());

	if (hdr == nullptr)
		return -1;

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return get_sequence_activity(this, hdr, sequence);
}
#include <intrin.h>
// sse min.
template< typename t = float >
t minimum(const t &a, const t &b) {
	// check type.
	static_assert(std::is_arithmetic< t >::value, "Math::min only supports integral types.");
	return (t)_mm_cvtss_f32(
		_mm_min_ss(_mm_set_ss((float)a),
			_mm_set_ss((float)b))
	);
}

// sse max.
template< typename t = float >
t maximum(const t &a, const t &b) {
	// check type.
	static_assert(std::is_arithmetic< t >::value, "Math::max only supports integral types.");
	return (t)_mm_cvtss_f32(
		_mm_max_ss(_mm_set_ss((float)a),
			_mm_set_ss((float)b))
	);
}

float C_BaseEntity::DesyncValue() {

	if (!this)
		return 0.f;

	auto anim_state = this->AnimStatev2();
	if (!anim_state)
		return 0.f;

	float duck_amount = anim_state->duck_amount;
	float speed_fraction = maximum< float >(0, minimum< float >(anim_state->feet_speed_forwards_or_sideways, 1));
	float speed_factor = maximum< float >(0, minimum< float >(1, anim_state->feet_speed_unknown_forwards_or_sideways));

	float yaw_modifier = (((anim_state->stop_to_full_running_fraction * -0.3f) - 0.2f) * speed_fraction) + 1.0f;

	if (duck_amount > 0.f) {
		yaw_modifier += ((duck_amount * speed_factor) * (0.5f - yaw_modifier));
	}

	return anim_state->velocity_subtract_y * yaw_modifier;
}


bool C_BaseEntity::IsNade()
{
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_incgrenade")
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
bool C_BaseEntity::IsEnemy()
{
	return this->GetTeam() != Globals::LocalPlayer->GetTeam();
}