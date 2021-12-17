#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "..\Utils\Utils.h"
#include "..\Utils\NetvarManager.h"
#include "CInput.h"
#include "Definitions.h"
#include "CBaseAnimState.h"
#include "..\SDK\Materials.h"
// class predefinition
class C_BaseCombatWeapon;

class CAnimationLayer
{
public:
	char  pad_0000[20];
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038


#define RandomInt(min, max) (rand() % (max - min + 1) + min)

class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
	// For calling VMT functions in our classes
	template< typename Function > Function call_vfunc(PVOID Base, DWORD Index)
	{
		PDWORD* VTablePointer = (PDWORD*)Base;
		PDWORD VTableFunctionBase = *VTablePointer;
		DWORD dwAddress = VTableFunctionBase[Index];
		return (Function)(dwAddress);
	}

public:
	Vector				GetPredicted(Vector p0);
    C_BaseCombatWeapon* GetActiveWeapon()
    {
        static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
        const auto weaponData      = GetValue<CBaseHandle>(m_hActiveWeapon);
        return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(weaponData));
    }

	C_BaseCombatWeapon* GetMyWeapons()
	{
		auto get_weapons = g_pNetvars->GetOffset("DT_CSPlayer", "m_hMyWeapons") & 0xFFF;
		return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(get_weapons));
	}

	matrix3x4_t& get_rgflcoordinateframe()
	{
		static auto _m_rgflCoordinateFrame = g_pNetvars->GetOffset("DT_CSPlayer", "m_CollisionGroup") - 0x30;
		return *reinterpret_cast<matrix3x4_t*>(reinterpret_cast<uintptr_t>(this) + _m_rgflCoordinateFrame);
	}

	void SetCurrentCommand(CUserCmd *cmd)
	{
		static int m_hConstraintEntity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;
	}

	const matrix3x4_t& m_rgflCoordinateFrame()
	{
		static auto _m_rgflCoordinateFrame = 0x444;
		return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
	}

    int GetTeam()
    {
        static int m_iTeamNum = g_pNetvars->GetOffset("DT_BaseEntity", "m_iTeamNum");
        return GetValue<int>(m_iTeamNum);
	}

    EntityFlags GetFlags()
    {
        static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
        return GetValue<EntityFlags>(m_fFlags);
    }

	void SetFlags(EntityFlags flags)
	{
		*reinterpret_cast<EntityFlags*>(uintptr_t(this) + g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags")) = flags;
	}

	MoveType_t GetMoveType()
	{
		return *(MoveType_t*)((DWORD)this + 0x25C);
	}

	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}

    bool GetLifeState()
    {
        static int m_lifeState = g_pNetvars->GetOffset("DT_BasePlayer", "m_lifeState");
        return GetValue<bool>(m_lifeState);
    }

    int GetHealth()
    {
        static int m_iHealth = g_pNetvars->GetOffset("DT_BasePlayer", "m_iHealth");
        return GetValue<int>(m_iHealth);
    }

    bool IsAlive() { return this->GetHealth() > 0 && this->GetLifeState() == 0; }

    bool IsImmune()
    {
        static int m_bGunGameImmunity = g_pNetvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
        return GetValue<bool>(m_bGunGameImmunity);
    }

	float GetCrouchAmount()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x2FAC);
	}

	float SetCrouchAmount(float amount)
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x2FAC) = amount;
	}

	bool IsScoped()
	{
		static int m_bIsScoped = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsScoped");
		return GetValue<bool>(m_bIsScoped);
	}

	Vector GetAimPunchAngle()
	{
		static int aimang = 0x302C;
		return GetValue<Vector>(aimang);
	}

	Vector GetViewPunchAngle()
	{
		static int viewang = 0x3020;
		return GetValue<Vector>(viewang);
	}

    int GetTickBase()
    {
        static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
        return GetValue<int>(m_nTickBase);
    }

    Vector GetOrigin()
    {
        static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
        return GetValue<Vector>(m_vecOrigin);
    }

	int m_hOwnerEntity()
	{
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_hOwnerEntity");
		return GetValue<int>(m_vecOrigin);
	} 

	Vector GetVecOrigin()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x138);
	}

	int GetViewModelIndex()
	{
		static int m_hViewModel = g_pNetvars->GetOffset("DT_BasePlayer", "m_hViewModel[0]");
		return GetValue<int>(m_hViewModel);
	}

	Vector GetVecViewOffset()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x108);
	}
    Vector GetViewOffset()
    {
        static int m_vecViewOffset = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
        return GetValue<Vector>(m_vecViewOffset);
    }

	Vector GetPunchAngles()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x302C);
	}

	int GetArmor()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xB324);
	}

	float GetLBY()
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return GetValue<float>(m_flLowerBodyYawTarget);
	}

	void SetLowerBodyYaw(float value)
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}

	bool HasHelmet()
	{
		return *reinterpret_cast<bool*>(uintptr_t(this) + 0xB318);
	}

	uintptr_t* GetWeapons()
	{
		return reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x0F8);
	}

	CBaseHandle* GetWeaponsHandle()
	{
		return reinterpret_cast<CBaseHandle*>(uintptr_t(this) + 0x0F8);
	}

	CBaseHandle m_hWeaponWorldModel()
	{
		return *(CBaseHandle*)((DWORD)this + g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel"));
	}

	void SetAccountID(int value)
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x208) = value;
	}

	int GetActiveWeaponIndex()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x2EF8) & 0xFFF;
	}

	int SetObserverMode(int mode)
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x3374) = mode;
	}

	int GetObserverMode()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x3374);
	}

	HANDLE getObserverTarget()
	{
		return (HANDLE)(this + 0x3388);
	}

	float m_flSimulationTime()
	{
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return GetValue<float>(m_flSimulationTime);
	}


	float m_flOldSimulationTime()
	{
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return *(float*)((DWORD)this + (m_flSimulationTime + 0x4));
	}


	int GetChokedPackets()
	{
		if (m_flSimulationTime() > m_flOldSimulationTime())
			return TIME_TO_TICKS(fabs(m_flSimulationTime() - m_flOldSimulationTime()));
		return 0;
	}


	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		/*void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
		if (!pRenderable)
			false;

		typedef bool(__thiscall* Fn)(void*, matrix3x4_t*, int, int, float);
		return VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);*/
		__asm
		{
			mov edi, this
			lea ecx, dword ptr ds : [edi + 0x4]
			mov edx, dword ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call dword ptr ds : [edx + 0x34]
		}
	}

	bool SetupBones2(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
		if (!pRenderable)
			false;

		typedef bool(__thiscall* Fn)(void*, matrix3x4_t*, int, int, float);
		return call_vfunc<Fn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
	int GetHitboxSet()
	{
		if (!g_pEngine->IsConnected())
			return 0;

		return *(int*)((DWORD)this + g_pNetvars->GetOffset("DT_BasePlayer", "m_nHitboxSet"));
	}

	QAngle& GetAbsAngles()
	{
		return Utils::CallVFunc<11, QAngle&>(this);
	}

	Vector& GetAbsOrigin()
	{
		return Utils::CallVFunc<10, Vector&>(this);

		if (!this)
			return Vector(0, 0, 0);
	}

	void SetAbsOrigin(Vector origin)
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
		static SetAbsOriginFn SetAbsOrigin;
		if (!SetAbsOrigin)
			SetAbsOrigin = (SetAbsOriginFn)((DWORD)Utils::FindSignature("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ?? ??"));
		SetAbsOrigin(this, origin);
	}

	void SetAbsAngles(QAngle angle)
	{
		using Fn = void(__thiscall*)(C_BaseEntity*, const QAngle& angles);
		static Fn func;

		if (!func)
			func = (Fn)(Utils::FindSignature("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));

		func(this, angle);
	}

	//void UpdateClientSideAnimation()
	//{
	//	return Utils::CallVFunc<219, void>(this);
	//}

	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall* original)(void*);
		call_vfunc<original>(this, 221)(this);
	}

	int GetSequenceActivity(int sequence)
	{
		const auto model = GetModel();
		if (!model)
			return -1;

		const auto hdr = g_pModelInfo->GetStudiomodel(model);
		if (!hdr)
			return -1;

		static auto offset = (DWORD)Utils::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");
		static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(offset);

		return GetSequenceActivity(this, hdr, sequence);
	}

	Vector GetEyeAngles()
	{
		static int m_angEyeAngles = 0xB344;
		return GetValue<Vector>(m_angEyeAngles);
	}

	Vector* GetEyeAnglesPointer2()
	{
		static int m_angEyeAngles = 0xB344;
		return (Vector*)((DWORD)this + m_angEyeAngles);
	}

	Vector* GetEyeAnglesPointer()
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		return GetValue<Vector*>(m_angEyeAngles);
	}

	bool IsDefusing()
	{
		static int m_bIsDefusing = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsDefusing");
		return GetValue<bool>(m_bIsDefusing);
	}

	int ForceBone()
	{
		static int m_nForceBone = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nForceBone");
		return GetValue<int>(m_nForceBone);
	}

	bool& ClientSideAnimation()
	{
		auto m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		return *reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation);
	}

	CAnimationLayer& GetAnimOverlay(int Index)
	{
		return (*(CAnimationLayer**)((DWORD)this + 0x2990))[Index];
	}

	int GetNumAnimOverlays() {
		return *(int*)((DWORD)this + 0x298C);
	}


	CAnimationLayer *AnimOverlays()
	{
		return *reinterpret_cast<CAnimationLayer**>(uintptr_t(this) + 0x2980);
	}

	CBaseHandle* m_hMyWeapons()
	{
		return (CBaseHandle*)((uintptr_t)this + 0x2DF8);
	}

	void SetAnimOverlay(int Index, CAnimationLayer layer)
	{
		(*(CAnimationLayer**)((DWORD)this + 0x2990))[Index] = layer;
	}

	void SetEyeAngles(Vector angles)
	{
		*reinterpret_cast<Vector*>(uintptr_t(this) + g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles")) = angles;
	}

	Vector* GetEyeAngles2()
	{
		return (Vector*)((DWORD)this + g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles"));
	}

	void SetFlashDuration(float value)
	{
		*reinterpret_cast<float*>(uintptr_t(this) + g_pNetvars->GetOffset("DT_CSPlayer", "m_flFlashDuration")) = value;
	}

	Vector GetVelocity()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x114);
	}

	void SetVelocity(Vector velocity)
	{
		*reinterpret_cast<Vector*>(uintptr_t(this) + 0x114) = velocity;
	}

	void SetRenderAngles(QAngle angles)
	{
		*(QAngle*)((DWORD)this + 0x128) = angles;
	}

	short sGetItemDefinitionIndex()
	{
		static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return GetValue<short>(m_iItemDefinitionIndex);
	}

	void SetItemDefinitionIndex(short value)
	{
		*reinterpret_cast<float*>(uintptr_t(this) + g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex")) = value;
	}

	void SetItemIDHigh(int value)
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x200) = value;
	}

	int FallbackPaintKit()
	{
		static int m_nFallbackPaintKit = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_nFallbackPaintKit");
		return GetValue<int>(m_nFallbackPaintKit);
	}

	void SetFallbackPaintKit(int value)
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x31B8) = value;
	}

	void SetFallbackWear(float value)
	{
		*reinterpret_cast<float*>(uintptr_t(this) + 0x31C0) = value;
	}

	char* GetCustomName()
	{
		return reinterpret_cast<char*>(uintptr_t(this) + 0x27C);
	}

	void SetFallbackSeed(int value)
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x31BC) = value;
	}

	void SetFallbackStatTrak(int value)
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x31C4) = value;
	}

	void SetEntityQuality(int value)
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x1EC) = value;
	}


	float GetNextAttack()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + 0x2D70);
	}

	char* GetWeaponName()
	{
		int WeaponId = sGetItemDefinitionIndex();
		switch (WeaponId)
		{
		case WEAPON_AK47:
			return "AK47";
			break;
		case WEAPON_AUG:
			return "AUG";
			break;
		case WEAPON_REVOLVER:
			return "REVOLVER";
			break;
		case WEAPON_AWP:
			return "AWP";
			break;
		case WEAPON_BIZON:
			return "PPBIZON";
			break;
		case WEAPON_C4:
			return "C4";
			break;
		case WEAPON_CZ75A:
			return "CZ75A";
			break;
		case WEAPON_DEAGLE:
			return "DEAGLE";
			break;
		case WEAPON_DECOY:
			return "DECOY";
			break;
		case WEAPON_ELITE:
			return "ELITE";
			break;
		case WEAPON_FAMAS:
			return "FAMAS";
			break;
		case WEAPON_FIVESEVEN:
			return "FIVESEVEN";
			break;
		case WEAPON_FLASHBANG:
			return "FLASHBANG";
			break;
		case WEAPON_G3SG1:
			return "G3SG1";
			break;
		case WEAPON_GALILAR:
			return "GALIL";
			break;
		case WEAPON_GLOCK:
			return "GLOCK";
			break;
		case WEAPON_INCGRENADE:
			return "INCENDIARY";
			break;
		case WEAPON_MOLOTOV:
			return "MOLOTOV";
			break;
		case WEAPON_SSG08:
			return "SSG08";
			break;
		case WEAPON_HEGRENADE:
			return "NADE";
			break;
		case WEAPON_M249:
			return "M249";
			break;
		case WEAPON_M4A1:
			return "M4A1";
			break;
		case WEAPON_MAC10:
			return "MAC10";
			break;
		case WEAPON_MAG7:
			return "MAG7";
			break;
		case WEAPON_MP7:
			return "MP7";
			break;
		case WEAPON_MP9:
			return "MP9";
			break;
		case WEAPON_NOVA:
			return "NOVA";
			break;
		case WEAPON_NEGEV:
			return "NEGEV";
			break;
		case WEAPON_P250:
			return "P250";
			break;
		case WEAPON_P90:
			return "P90";
			break;
		case WEAPON_SAWEDOFF:
			return "SAWEDOFF";
			break;
		case WEAPON_SCAR20:
			return "SCAR20";
			break;
		case WEAPON_MP5SD:
			return "MP5";
			break;
		case WEAPON_SMOKEGRENADE:
			return "SMOKE";
			break;
		case WEAPON_SG556:
			return "SG553";
			break;
		case WEAPON_TEC9:
			return "TEC9";
			break;
		case WEAPON_HKP2000:
			return "P2000";
			break;
		case WEAPON_USP_SILENCER:
			return "USP";
			break;
		case WEAPON_UMP45:
			return "UMP45";
			break;
		case WEAPON_XM1014:
			return "XM1014";
			break;
		case WEAPON_TASER:
			return "ZEUS";
			break;
		case WEAPON_M4A1_SILENCER:
			return "M4A1S";
			break;
		case WEAPON_KNIFE_BAYONET:
			return "BAYONET";
			break;
		case WEAPON_KNIFE_M9_BAYONET:
			return "M9 BAYONET";
			break;
		case WEAPON_KNIFE_KARAMBIT:
			return "KARAMBIT";
			break;
		case WEAPON_KNIFE_FALCHION:
			return "FALCHION";
			break;
		case WEAPON_KNIFE_TACTICAL:
			return "KNIFE";
			break;
		case WEAPON_KNIFE_BUTTERFLY:
			return "BUTTERFLY";
			break;
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			return "BOWIE";
			break;
		case WEAPON_KNIFE_GUT:
			return "GUT KNIFE";
			break;
		case WEAPON_KNIFE_T:
			return "KNIFE";
			break;
		case WEAPON_KNIFE_PUSH:
			return "KNIFE";
			break;
		case WEAPON_KNIFE_FLIP:
			return "FLIP KNIFE";
			break;
		case WEAPON_KNIFE:
			return "KNIFE";
			break;
		}
		return "?";
	}
	
	const char* GunIconCustom()
	{
		switch (this->sGetItemDefinitionIndex())
		{
		case ItemDefinitionIndex::WEAPON_DEAGLE:
			return "a";
			break;
		case ItemDefinitionIndex::WEAPON_ELITE:
			return "b";
			break;
		case ItemDefinitionIndex::WEAPON_FIVESEVEN:
			return "c";
			break;
		case ItemDefinitionIndex::WEAPON_GLOCK:
			return "d";
			break;
		case ItemDefinitionIndex::WEAPON_AK47:
			return "e";
			break;
		case ItemDefinitionIndex::WEAPON_AUG:
			return "f";
			break;
		case ItemDefinitionIndex::WEAPON_AWP:
			return "g";
			break;
		case ItemDefinitionIndex::WEAPON_FAMAS:
			return "h";
			break;
		case ItemDefinitionIndex::WEAPON_G3SG1:
			return "j";
			break;
		case ItemDefinitionIndex::WEAPON_GALILAR:
			return "k";
			break;
		case ItemDefinitionIndex::WEAPON_M249:
			return "i";
			break;
		case ItemDefinitionIndex::WEAPON_M4A1:
			return "l";
			break;
		case ItemDefinitionIndex::WEAPON_MAC10:
			return "n";
			break;
		case ItemDefinitionIndex::WEAPON_P90:
			return "C";
			break;
		case ItemDefinitionIndex::WEAPON_MP5SD:
			return "p";
			break;
		case ItemDefinitionIndex::WEAPON_UMP45:
			return "q";
			break;
		case ItemDefinitionIndex::WEAPON_XM1014:
			return "r";
			break;
		case ItemDefinitionIndex::WEAPON_BIZON:
			return "s";
			break;
		case ItemDefinitionIndex::WEAPON_MAG7:
			return "t";
			break;
		case ItemDefinitionIndex::WEAPON_NEGEV:
			return "u";
			break;
		case ItemDefinitionIndex::WEAPON_SAWEDOFF:
			return "v";
			break;
		case ItemDefinitionIndex::WEAPON_TEC9:
			return "w";
			break;
		case ItemDefinitionIndex::WEAPON_TASER:
			return "x";
			break;
		case ItemDefinitionIndex::WEAPON_HKP2000:
			return "o";
			break;
		case ItemDefinitionIndex::WEAPON_MP7:
			return "z";
			break;
		case ItemDefinitionIndex::WEAPON_MP9:
			return "A";
			break;
		case ItemDefinitionIndex::WEAPON_NOVA:
			return "B";
			break;
		case ItemDefinitionIndex::WEAPON_P250:
			return "y";
			break;
		case ItemDefinitionIndex::WEAPON_SCAR20:
			return "D";
			break;
		case ItemDefinitionIndex::WEAPON_SG556:
			return "E";
			break;
		case ItemDefinitionIndex::WEAPON_SSG08:
			return "F";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE:
			return "G";
			break;
		case ItemDefinitionIndex::WEAPON_FLASHBANG:
			return "L";
			break;
		case ItemDefinitionIndex::WEAPON_HEGRENADE:
			return "I";
			break;
		case ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
			return "J";
			break;
		case ItemDefinitionIndex::WEAPON_MOLOTOV:
			return "K";
			break;
		case ItemDefinitionIndex::WEAPON_DECOY:
			return "H";
			break;
		case ItemDefinitionIndex::WEAPON_INCGRENADE:
			return "M";
			break;
		case ItemDefinitionIndex::WEAPON_C4:
			return "N";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFEGG:
		case ItemDefinitionIndex::WEAPON_KNIFE_T:
			return "O";
			break;
		case ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
			return "m";
			break;
		case ItemDefinitionIndex::WEAPON_USP_SILENCER:
			return "P";
			break;
		case ItemDefinitionIndex::WEAPON_CZ75A:
			return "Q";
			break;
		case ItemDefinitionIndex::WEAPON_REVOLVER:
			return "R";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
			return "V";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
			return "W";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
			return "X";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
			return "Y";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
			return "Z";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
			return "0";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
			return "1";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
			return "2";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
			return "3";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
			return "4";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_URSUS:
			return "5";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE:
			return "6";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_STILETTO:
			return "7";
			break;
		case ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER:
			return "8";
			break;
		}

		return "?";
	}
	CBaseAnimState* GetAnimState()
	{
		return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + 0x3900);
	}

	CBaseAnimState* SetAnimState(CBaseAnimState* state)
	{
		return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + 0x3900) = state;
	}

	Vector GetBonePosition(int i)
	{
		matrix3x4_t matrix[128];
		if (this->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
		{
			return Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}

	void SetModelIndex(int model)
	{
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		return call_vfunc< oSetModelIndex >(this, 75)(this, model);
	}

	int m_nModelIndex()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + g_pNetvars->GetOffset("DT_BaseEntity", "m_nModelIndex"));
	}

	float* GetPoseParamaters()
	{
		return reinterpret_cast<float*>(uintptr_t(this) + 0x18);
	}

	Vector GetEyePosition() { return this->GetVecOrigin() + this->GetVecViewOffset(); }

	const Vector WorldSpaceCenter()
	{
		Vector vecOrigin = GetVecOrigin();

		Vector min = this->GetCollideable()->OBBMins() + vecOrigin;
		Vector max = this->GetCollideable()->OBBMaxs() + vecOrigin;

		Vector size = max - min;
		size /= 2.f;
		size += min;

		return size;
	}

	Vector GetEyeOrigin()
	{
		if (!this)
			return Vector(0, 0, 0);

		Vector result;
		call_vfunc<void(__thiscall*)(void*, Vector&)>(this, 281)(this, result);
		return result;
	}

	bool IsWeapon()
	{
		return Utils::CallVFunc<160, bool>(this);
	}
	bool IsKnifeorNade();
};

class C_BaseCombatWeapon : public C_BaseEntity
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
public:
	HANDLE m_hWeaponWorldModel_h()
	{
		return *(HANDLE*)((uintptr_t)this + 0x3234); //m_hWeaponWorldModel
	}
	CBaseHandle m_hWeaponWorldModel_c()
	{
		return *(CBaseHandle*)((uintptr_t)this + 0x3234); //m_hWeaponWorldModel
	}
	ItemDefinitionIndex GetItemDefinitionIndex()
	{
		static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return GetValue<ItemDefinitionIndex>(m_iItemDefinitionIndex);
	}

    float GetNextPrimaryAttack()
    {
        static int m_flNextPrimaryAttack = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
        return GetValue<float>(m_flNextPrimaryAttack);
    }

	float GetNextSecondaryAttack()
	{
		static int m_flNextSecondaryAttack = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextSecondaryAttack");
		return GetValue<float>(m_flNextSecondaryAttack);
	}

	bool IsPinPulled()
	{
		static int m_bPinPulled = g_pNetvars->GetOffset("DT_BaseCSGrenade", "m_bPinPulled");
		return GetValue<bool>(m_bPinPulled);
	}

	float GetThrowTime()
	{
		static int m_fThrowTime = g_pNetvars->GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
		return GetValue<float>(m_fThrowTime);
	}

    int GetAmmo()
    {
        static int m_iClip1 = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
        return GetValue<int>(m_iClip1);
    }

	int GetAmmo2()
	{
		static int m_iClip2 = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
		return GetValue<int>(m_iClip2);
	}

    WeaponInfo_t* GetCSWpnData()
    {
        return Utils::CallVFunc<454, WeaponInfo_t*>(this);
    }

	float GetPostponeFireReadyTime()
	{
		static int m_flPostponeFireReadyTime = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
		return *reinterpret_cast<float*>(uintptr_t(this) + m_flPostponeFireReadyTime);
	}

	float GetSpreadCone()
	{
		return Utils::CallVFunc<446, float>(this);
	}

	float GetInaccuracy()
	{
		return Utils::CallVFunc<476, float>(this);
	}

	float GetLastShotTime()
	{
		static int m_fLastShotTime = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return GetValue<float>(m_fLastShotTime);
	}

	void UpdateAccuracyPenalty()
	{
		return Utils::CallVFunc<477, void>(this);
	}

    std::string GetName()
    {
        ///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
        return std::string(this->GetCSWpnData()->weapon_name);
    }

	bool is_knife()
	{
		int iWeaponID = this->sGetItemDefinitionIndex();
		return (iWeaponID == 42 || iWeaponID == 59
			|| iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}

	bool is_taser()
	{
		int iWeaponID = this->sGetItemDefinitionIndex();
		return (iWeaponID == 31);
	}

	bool is_grenade()
	{
		if (!this)
			return false;

		int WeaponId = this->sGetItemDefinitionIndex();

		if (!WeaponId)
			return false;

		if (WeaponId == 43 || WeaponId == 44 || WeaponId == 45 || WeaponId == 46 || WeaponId == 47 || WeaponId == 48)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int GetWorldModelIndex()
	{
		return *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x3234);
	}

	void SetViewModelIndex(int value)
	{
		*reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x3220) = value;
	}

	void SetWorldModelIndex(int value)
	{
		*reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x3234) = value;
	}

	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + 0x31B4);
	}
	int* ModelIndex()
	{
		return (int*)((uintptr_t)this + 0x258);
	}
	int* ViewModelIndex()
	{
		return (int*)((uintptr_t)this + 0x3220);
	}
	short* fixskins()
	{
		return (short*)((uintptr_t)this + 0x2FAA);
	}
	inline int* FallbackPaintKit() {
		return (int*)((DWORD)this + 0x31B8);
	}
	int* GetEntityQuality()
	{
		return (int*)((uintptr_t)this + 0x2FAC);
	}
	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + 0x31B0);
	}

	float* FallbackWear()
	{
		return (float*)((uintptr_t)this + 0x31C0);
	}
	int* ItemIDHigh()
	{
		return (int*)((uintptr_t)this + 0x2FC0);
	}
};

class C_BaseViewmodel : public C_BaseEntity
{
public:
	inline int GetModelIndex() {

		return *(int*)((DWORD)this + 0x258);
	}
	int GetWeaponIndex()
	{
		return (*reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x29C8)) & 0xFFF;
	}

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + 0x29CC);
	}

	int GetOwnerIndex()
	{
		return (*reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x3210)) & 0xFFF;
	}

private:
};