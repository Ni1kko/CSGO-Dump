#pragma once

#include "../UTILS/offsets.h"
#include "../UTILS/interfaces.h"
#include "../SDK/ModelInfo.h"
#include "../UTILS/qangle.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CClientEntityList.h"

#include "AnimLayer.h"
#include "RecvData.h"
enum DataUpdateType_txd
{
	DATA_UPDATE_CREATEDxd = 0,
	DATA_UPDATE_DATATABLE_CHANGEDxd,
};
namespace SDK
{
	class CBaseAnimState;
	class Collideable;
	typedef unsigned long CBaseHandle;
	struct studiohdr_t;
	struct model_t;
	enum ItemDefinitionIndex
	{
		WEAPON_NONE = 0,
		WEAPON_DEAGLE,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALIL = 13,
		WEAPON_M249,
		WEAPON_M4A4 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5SD = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_ZEUS,
		WEAPON_P2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SHIELD,
		WEAPON_SCAR20,
		WEAPON_SG553,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INC,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1S,
		WEAPON_USPS,
		WEAPON_CZ75 = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_SNOWBALL,
		WEAPON_BUMPMINE,
		WEAPON_KNIFE_BAYONET = 500,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER,
		GLOVE_STUDDED_BLOODHOUND = 5027,
		GLOVE_T_SIDE = 5028,
		GLOVE_CT_SIDE = 5029,
		GLOVE_SPORTY = 5030,
		GLOVE_SLICK = 5031,
		GLOVE_LEATHER_WRAP = 5032,
		GLOVE_MOTORCYCLE = 5033,
		GLOVE_SPECIALIST = 5034,
		GLOVE_HYDRA = 5035
	};


	enum class ItemDefinitionIndexx : short
	{
		WEAPON_NONE = 0,
		WEAPON_DEAGLE,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALIL = 13,
		WEAPON_M249,
		WEAPON_M4A4 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5SD = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_ZEUS,
		WEAPON_P2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SHIELD,
		WEAPON_SCAR20,
		WEAPON_SG553,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INC,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1S,
		WEAPON_USPS,
		WEAPON_CZ75 = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_SNOWBALL,
		WEAPON_BUMPMINE,
		WEAPON_KNIFE_BAYONET = 500,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER,
		GLOVE_STUDDED_BLOODHOUND = 5027,
		GLOVE_T_SIDE = 5028,
		GLOVE_CT_SIDE = 5029,
		GLOVE_SPORTY = 5030,
		GLOVE_SLICK = 5031,
		GLOVE_LEATHER_WRAP = 5032,
		GLOVE_MOTORCYCLE = 5033,
		GLOVE_SPECIALIST = 5034,
		GLOVE_HYDRA = 5035
	};

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_ISOMETRIC,
		MOVETYPE_WALK,
		MOVETYPE_STEP,
		MOVETYPE_FLY,
		MOVETYPE_FLYGRAVITY,
		MOVETYPE_VPHYSICS,
		MOVETYPE_PUSH,
		MOVETYPE_NOCLIP,
		MOVETYPE_LADDER,
		MOVETYPE_OBSERVER,
		MOVETYPE_CUSTOM,
		MOVETYPE_LAST = MOVETYPE_CUSTOM,
		MOVETYPE_MAX_BITS = 4
	};

	class CBaseAnimating
	{
	public:
		std::array<float, 24>* m_flPoseParameter()
		{
			static int offset = 0;
			if (!offset)
				offset = OFFSETS::m_flPoseParameter;
			return (std::array<float, 24>*)((uintptr_t)this + offset);
		}
		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			typedef model_t* (__thiscall* fnGetModel)(void*);

			return VMT::VMTHookManager::GetFunction<fnGetModel>(pRenderable, 8)(pRenderable);
		}
		void SetBoneMatrix(matrix3x4_t* boneMatrix)
		{
			//Offset found in C_BaseAnimating::GetBoneTransform, string search ankle_L and a function below is the right one
			const auto model = this->GetModel();
			if (!model)
				return;

			matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
			studiohdr_t *hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return;
			int size = hdr->numbones;
			if (matrix) {
				for (int i = 0; i < size; i++)
					memcpy(matrix + i, boneMatrix + i, sizeof(matrix3x4_t));
			}
		}
		void GetDirectBoneMatrix(matrix3x4_t* boneMatrix)
		{
			const auto model = this->GetModel();
			if (!model)
				return;

			matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
			studiohdr_t *hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return;
			int size = hdr->numbones;
			if (matrix) {
				for (int i = 0; i < size; i++)
					memcpy(boneMatrix + i, matrix + i, sizeof(matrix3x4_t));
			}
		}
	};

	class CBaseEntity
	{
	public:
		int GetHealth()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iHealth);
		}
		int GetFlags()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_fFlags);
		}
		void SetFlags(int flags)
		{
			*reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_fFlags) = flags;
		}
		int GetTeam()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iTeamNum);
		}
		int GetObserverMode()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iObserverMode);
		}
		int SetObserverMode(int mode)
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iObserverMode) = mode;
		}
		HANDLE GetObserverModeHandle() {
			return *(HANDLE*)((uintptr_t)this + OFFSETS::m_iObserverMode);
		}
		bool GetIsScoped()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bIsScoped);
		}
		Vector GetVelocity()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecVelocity);
		}
		void SetVelocity(Vector velocity)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecVelocity) = velocity;
		}
		int GetMoney()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iAccount);
		}
		int GetLifeState()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_lifeState);
		}
		float GetLowerBodyYaw()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flLowerBodyYawTarget);
		}
		Vector GetVecOrigin()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecOrigin);
		}
		bool IsAlive()
		{
			return this->GetLifeState() == 0;
		}
		bool is_dead()
		{
			return this->GetLifeState() == 2;
		}
		Vector& GetAbsOrigin()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 10))(this);
		}
		int GetTickBase()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_nTickBase);
		}
		bool GetIsDormant()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bDormant);
		}
		CBaseAnimState* GetAnimState()
		{
			return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + OFFSETS::animstate);
		}
		CAnimationLayer* GetAnimOverlaysModifiable()
		{
			return (*reinterpret_cast<CAnimationLayer**>(reinterpret_cast<std::uintptr_t>(this) + OFFSETS::animlayer));
		}
		Collideable* GetCollideable()
		{
			return (Collideable*)((DWORD)this + OFFSETS::m_Collision);
		}
		const matrix3x4_t& m_rgflCoordinateFrame()
		{
			static auto _m_rgflCoordinateFrame = OFFSETS::m_bCollisionGroup - 0x30;
			return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
		}
		void GetRenderBounds(Vector& mins, Vector& maxs)
		{
			void* pRenderable = (void*)(this + 0x4);
			typedef void(__thiscall* Fn)(void*, Vector&, Vector&);
			VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 17)(pRenderable, mins, maxs);
		}
		int GetIndex()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + 0x64);
		}
		int GetMoveType()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_MoveType);
		}
		CAnimationLayer& GetAnimOverlay(int Index)
		{
			return (*(CAnimationLayer**)((DWORD)this + OFFSETS::animlayer))[Index];
		}
		void SetAnimOverlay(int Index, CAnimationLayer layer)
		{
			(*(CAnimationLayer**)((DWORD)this + OFFSETS::animlayer))[Index] = layer;
		}
		int CBaseEntity::GetSequenceActivity(int sequence)
		{
			const auto model = GetModel();
			if (!model)
				return -1;

			const auto hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return -1;

			static auto offset = (DWORD)UTILS::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");
			static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, SDK::studiohdr_t*, int)>(offset);

			return GetSequenceActivity(this, hdr, sequence);
		}
		Vector* GetEyeAnglesPtr()
		{
			return reinterpret_cast<Vector*>((DWORD)this + OFFSETS::m_angEyeAngles);
		}
		Vector GetEyeAngles()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles);
		}
		Vector GetVecMins()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x320);
		}
		Vector GetVecMaxs()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x32C);
		}
		QAngle* EasyEyeAngles()
		{
			return (QAngle*)((uintptr_t)this + OFFSETS::m_angEyeAngles);
		}
		void SetModelIndex(int index)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(this, 75)(this, index);
		}
		IClientNetworkable* CBaseEntity::GetNetworkable()
		{
			return reinterpret_cast<IClientNetworkable*>(reinterpret_cast<uintptr_t>(this) + 0x8);
		}
		void PreDataUpdate(DataUpdateType_txd updateType)
		{
			PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
			typedef void(__thiscall* OriginalFn)(PVOID, int);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
		}
		CBaseHandle * m_hMyWeapons()
		{
			return (CBaseHandle*)((uintptr_t)this + OFFSETS::m_hMyWeapons);
		}
		void SetEyeAngles(Vector angles)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles) = angles;
		}
		void SetVecOrigin(Vector angles)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecOrigin) = angles;
		}
		float GetSimTime()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flSimulationTime);
		}
		Vector GetViewOffset()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecViewOffset);
		}
		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			typedef model_t* (__thiscall* fnGetModel)(void*);

			return VMT::VMTHookManager::GetFunction<fnGetModel>(pRenderable, 8)(pRenderable);
		}
		matrix3x4_t GetBoneMatrix(int BoneID)
		{
			matrix3x4_t matrix;

			auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + OFFSETS::m_dwBoneMatrix);
			if (offset)
				matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

			return matrix;
		}
		Vector GetBonePosition(int i)
		{
			VMatrix matrix[128];
			if (this->SetupBones(matrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
			{
				return Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]);
			}
			return Vector(0, 0, 0);
		}
		void CBaseEntity::CopyAnimLayers(SDK::CAnimationLayer* dest)
		{
			int count = 15;
			for (int i = 0; i < count; i++)
			{
				dest[i] = GetAnimOverlay(i);
			}
		}


		void CBaseEntity::CopyPoseParameters(float* dest)
		{
			float* flPose = (float*)((DWORD)this + 0x2764);
			memcpy(dest, flPose, sizeof(float) * 24);
		}
		Vector GetEyePosition(void)
		{
			return GetVecOrigin() + *(Vector*)((DWORD)this + OFFSETS::m_vecViewOffset);
		}
		Vector* GetViewPunchAngle()
		{
			return (Vector*)((uintptr_t)this + OFFSETS::m_viewPunchAngle);
		}
		Vector* GetAimPunchAngle()
		{
			return (Vector*)((uintptr_t)this + OFFSETS::m_aimPunchAngle);
		}
		Vector GetPunchAngles()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_aimPunchAngle);
		}
		bool GetImmunity()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bGunGameImmunity);
		}
		bool setup_bones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) {
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			if (!pRenderable)
				false;

			typedef bool(__thiscall* Fn)(void*, matrix3x4_t*, int, int, float);
			return VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
		}
		bool SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
		{
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
		bool& GetClientSideAnimation()
		{
			static int offset = OFFSETS::m_bClientSideAnimation;
			return *reinterpret_cast<bool*>((DWORD)this + offset);
		}
		CAnimationLayer *GetAnimOverlays()
		{
			return *(CAnimationLayer**)((DWORD)this + 0x2990);
		}
		int GetNumAnimOverlays()
		{
			return *(int*)((DWORD)this + 0x299C);
		}
		void UpdateClientSideAnimation()
		{
			typedef void(__thiscall* original)(void*);
			VMT::VMTHookManager::GetFunction<original>(this, 223)(this);
		}

		float get_max_desync_delta(SDK::CBaseEntity *local) {

			uintptr_t animstate = uintptr_t(local->GetAnimState());


			float duckammount = *(float *)(animstate + 0xA4);
			float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

			float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

			float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
			float unk2 = unk1 + 1.1f;
			float unk3;

			if (duckammount > 0) {

				unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

			}

			unk3 = *(float *)(animstate + 0x334) * unk2;

			return unk3;

		}

		void SetAbsOrigin(Vector ArgOrigin)
		{
			using Fn = void(__thiscall*)(CBaseEntity*, const Vector &origin);
			static Fn func;

			if (!func)
				func = (Fn)(UTILS::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));

			func(this, ArgOrigin);
		}
		void SetOriginz(Vector wantedpos)
		{
			typedef void(__thiscall* SetOriginFn)(void*, const Vector &);
			static SetOriginFn SetOriginze = (SetOriginFn)(UTILS::FindSignature("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
			SetOriginze(this, wantedpos);
		}
		Vector& GetAbsAngles()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 11))(this);
		}
		void SetAbsAngles(Vector angles)
		{
			using Fn = void(__thiscall*)(CBaseEntity*, const Vector &angles);
			static Fn func;

			if (!func)
				func = (Fn)(UTILS::FindPattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

			func(this, angles);
		}
		float* GetPoseParamaters()
		{
			return reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flPoseParameter);
		}
		int DrawModel(int flags, uint8_t alpha)
		{
			void* pRenderable = (void*)(this + 0x4);

			using fn = int(__thiscall*)(void*, int, uint8_t);
			return VMT::VMTHookManager::GetFunction< fn >(pRenderable, 9)(pRenderable, flags, alpha);
		}
		ClientClass* GetClientClass()
		{
			void* Networkable = (void*)(this + 0x8);
			typedef ClientClass*(__thiscall* OriginalFn)(void*);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(Networkable, 2)(Networkable);
		}
		float GetNextAttack()
		{
			return *reinterpret_cast<float*>(uint32_t(this) + OFFSETS::m_flNextAttack);
		}
		int GetActiveWeaponIndex()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_hActiveWeapon) & 0xFFF;
		}
		int GetArmor()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_ArmorValue);
		}
		bool HasHelmet()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bHasHelmet);
		}
		char* GetArmorName()
		{
			if (GetArmor() > 0)
			{
				if (HasHelmet())
					return "hk";
				else
					return "k";
			}
			else
				return " ";
		}
		bool HasC4()
		{
			int iBombIndex = *(int*)(*(DWORD*)(OFFSETS::dwPlayerResource) + OFFSETS::m_iPlayerC4);
			if (iBombIndex == this->GetIndex())
				return true;
			else
				return false;
		}
		bool IsValidRenderable(SDK::CBaseEntity* local_player) {

			if (!this || this == nullptr || local_player == nullptr)
				return false;

			if (this == local_player)
				return false;

			if (this->GetTeam() == local_player->GetTeam())
				return false;

			if (this->GetIsDormant())
				return false;

			if (!this->IsAlive())
				return false;

			return true;
		}
		void SetAngle2(Vector wantedang) {
			typedef void(__thiscall* oSetAngle)(void*, const Vector &);
			static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)UTILS::FindSignature("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
			_SetAngle(this, wantedang);
		}
		short GetItemDefenitionIndex()
		{
			return *reinterpret_cast<short*>(uintptr_t(this) + OFFSETS::m_iItemDefinitionIndex);
		}
		char* get_weapon_names() {
			int WeaponId = this->GetItemDefenitionIndex();

			switch (WeaponId) {
			case  SDK::ItemDefinitionIndex::WEAPON_KNIFE:
				return "knife";
			case  SDK::ItemDefinitionIndex::WEAPON_KNIFE_T:
				return "knife";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
				return "bayonet";
			case  SDK::ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
				return "flip knife";
			case  SDK::ItemDefinitionIndex::WEAPON_KNIFE_GUT:
				return "gut knife";
			case  SDK::ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
				return "karambit";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
				return "m9 bayonet";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
				return "huntsman knife";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
				return "falchion knife";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
				return "bowie knife";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
				return "butterfly knife";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
				return "shadow daggers";
			case SDK::ItemDefinitionIndex::WEAPON_DEAGLE:
				return "deagle";
			case SDK::ItemDefinitionIndex::WEAPON_ELITE:
				return "dual berretas";
			case SDK::ItemDefinitionIndex::WEAPON_FIVESEVEN:
				return "five-seven";
			case SDK::ItemDefinitionIndex::WEAPON_GLOCK:
				return "glock";
			case  SDK::ItemDefinitionIndex::WEAPON_P2000:
				return "p2000";
			case SDK::ItemDefinitionIndex::WEAPON_P250:
				return "p250";
			case  SDK::ItemDefinitionIndex::WEAPON_USPS:
				return "usp";
			case SDK::ItemDefinitionIndex::WEAPON_TEC9:
				return "tec9";
			case  SDK::ItemDefinitionIndex::WEAPON_CZ75:
				return "cz75";
			case  SDK::ItemDefinitionIndex::WEAPON_REVOLVER:
				return "revolver";
			case  SDK::ItemDefinitionIndex::WEAPON_MAC10:
				return "mac10";
			case  SDK::ItemDefinitionIndex::WEAPON_UMP45:
				return "ump45";
			case  SDK::ItemDefinitionIndex::WEAPON_BIZON:
				return "bizon";
			case  SDK::ItemDefinitionIndex::WEAPON_MP7:
				return "mp7";
			case SDK::ItemDefinitionIndex::WEAPON_MP9:
				return "mp9";
			case  SDK::ItemDefinitionIndex::WEAPON_P90:
				return "p90";
			case  SDK::ItemDefinitionIndex::WEAPON_GALIL:
				return "galil-ar";
			case  SDK::ItemDefinitionIndex::WEAPON_FAMAS:
				return "famas";
			case SDK::ItemDefinitionIndex::WEAPON_M4A1S:
				return "m4a1-s";
			case SDK::ItemDefinitionIndex::WEAPON_M4A4:
				return "m4a1";
			case SDK::ItemDefinitionIndex::WEAPON_AUG:
				return "aug";
			case SDK::ItemDefinitionIndex::WEAPON_SG553:
				return "sg556";
			case SDK::ItemDefinitionIndex::WEAPON_AK47:
				return "ak47";
			case SDK::ItemDefinitionIndex::WEAPON_G3SG1:
				return "g3sg1";
			case SDK::ItemDefinitionIndex::WEAPON_SCAR20:
				return "scar20";
			case  SDK::ItemDefinitionIndex::WEAPON_AWP:
				return "awp";
			case  SDK::ItemDefinitionIndex::WEAPON_SSG08:
				return "scout";
			case SDK::ItemDefinitionIndex::WEAPON_XM1014:
				return "xm1014";
			case  SDK::ItemDefinitionIndex::WEAPON_SAWEDOFF:
				return "sawed-off";
			case SDK::ItemDefinitionIndex::WEAPON_MAG7:
				return "mag7";
			case SDK::ItemDefinitionIndex::WEAPON_NOVA:
				return "nova";
			case SDK::ItemDefinitionIndex::WEAPON_NEGEV:
				return "negev";
			case SDK::ItemDefinitionIndex::WEAPON_M249:
				return "m249";
			case SDK::ItemDefinitionIndex::WEAPON_ZEUS:
				return "zeus";
			case SDK::ItemDefinitionIndex::WEAPON_FLASHBANG:
				return "flash";
			case SDK::ItemDefinitionIndex::WEAPON_HEGRENADE:
				return "he";
			case SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
				return "smoke";
			case SDK::ItemDefinitionIndex::WEAPON_MOLOTOV:
				return "molotov";
			case SDK::ItemDefinitionIndex::WEAPON_DECOY:
				return "decoy";
			case SDK::ItemDefinitionIndex::WEAPON_INC:
				return "incendiary";
			case SDK::ItemDefinitionIndex::WEAPON_C4:
				return "c4";
			default:
				return " ";
			}
		}

		char* get_gun_icons()
		{
			int WeaponId = this->GetItemDefenitionIndex();

			switch (WeaponId) {
			case SDK::ItemDefinitionIndex::WEAPON_DEAGLE:
				return "A";
			case SDK::ItemDefinitionIndex::WEAPON_REVOLVER:
				return "J";
			case SDK::ItemDefinitionIndex::WEAPON_ELITE:
				return "B";
			case SDK::ItemDefinitionIndex::WEAPON_FIVESEVEN:
				return "C";
			case SDK::ItemDefinitionIndex::WEAPON_GLOCK:
				return "D";
			case SDK::ItemDefinitionIndex::WEAPON_TEC9:
				return "H";
			case SDK::ItemDefinitionIndex::WEAPON_AK47:
				return "W";
			case SDK::ItemDefinitionIndex::WEAPON_AUG:
				return "U";
			case SDK::ItemDefinitionIndex::WEAPON_AWP:
				return "Z";
			case SDK::ItemDefinitionIndex::WEAPON_FAMAS:
				return "R";
			case SDK::ItemDefinitionIndex::WEAPON_G3SG1:
				return "X";
			case SDK::ItemDefinitionIndex::WEAPON_SCAR20:
				return "Y";
			case SDK::ItemDefinitionIndex::WEAPON_GALIL:
				return "Q";
			case SDK::ItemDefinitionIndex::WEAPON_M249:
				return "g";
			case SDK::ItemDefinitionIndex::WEAPON_M4A1S:
				return "S";
			case SDK::ItemDefinitionIndex::WEAPON_M4A4:
				return "T";
			case SDK::ItemDefinitionIndex::WEAPON_MAC10:
				return "K";
			case SDK::ItemDefinitionIndex::WEAPON_P90:
				return "P";
			case SDK::ItemDefinitionIndex::WEAPON_UMP45:
				return "L";
			case SDK::ItemDefinitionIndex::WEAPON_XM1014:
				return "b";
			case SDK::ItemDefinitionIndex::WEAPON_BIZON:
				return "M";
			case SDK::ItemDefinitionIndex::WEAPON_MP9:
				return "O";
			case SDK::ItemDefinitionIndex::WEAPON_MAG7:
				return "d";
			case SDK::ItemDefinitionIndex::WEAPON_SAWEDOFF:
				return "c";
			case SDK::ItemDefinitionIndex::WEAPON_NOVA:
				return "e";
			case SDK::ItemDefinitionIndex::WEAPON_NEGEV:
				return "f";
			case SDK::ItemDefinitionIndex::WEAPON_USPS:
				return "G";
			case SDK::ItemDefinitionIndex::WEAPON_ZEUS:
				return "h";
			case SDK::ItemDefinitionIndex::WEAPON_P2000:
				return "E";
			case SDK::ItemDefinitionIndex::WEAPON_P250:
				return "F";
			case SDK::ItemDefinitionIndex::WEAPON_CZ75:
				return "Y";
			case SDK::ItemDefinitionIndex::WEAPON_MP7:
				return "N";
			case SDK::ItemDefinitionIndex::WEAPON_SG553:
				return "V";
			case SDK::ItemDefinitionIndex::WEAPON_SSG08:
				return "a";
			case SDK::ItemDefinitionIndex::WEAPON_FLASHBANG:
				return "i";
			case SDK::ItemDefinitionIndex::WEAPON_DECOY:
				return "m";
			case SDK::ItemDefinitionIndex::WEAPON_HEGRENADE:
				return "j";
			case SDK::ItemDefinitionIndex::WEAPON_MOLOTOV:
				return "l";
			case SDK::ItemDefinitionIndex::WEAPON_INC:
				return "n";
			case SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
				return "k";
			case SDK::ItemDefinitionIndex::WEAPON_C4:
				return "o";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE:
				return "]";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_T:
				return "[";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
				return "1";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
				return "2";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_GUT:
				return "3";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
				return "4";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
				return "5";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
				return "6";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
				return "0";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
				return "7";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
				return "8";
			case SDK::ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
				return "9";
			}
		};
		bool IsVisibleVector(Vector bone);
		bool IsVisible(int bone);
};

class CBaseViewModel : public CModelInfo
{
public:

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + OFFSETS::m_hOwner);
	}

	inline int GetModelIndex() {

		return *(int*)((DWORD)this + OFFSETS::m_nModelIndex);
	}
};
}
