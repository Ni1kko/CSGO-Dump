#include "csgostructs.hpp"
#include "../Helpers/Math.hpp"
#include "../Helpers/Utils.hpp"
#include "../helpers/utils.hpp"
#include <map>

//increase it if valve added some funcs to baseentity :lillulmoa:
constexpr auto VALVE_ADDED_FUNCS = 0ull;

CGameRules* g_GameRules = nullptr;


bool C_BaseEntity::IsPlayer()
{
	//index: 157
	//ref: "effects/nightvision"
	//sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 157 + VALVE_ADDED_FUNCS)(this);
}

void C_BasePlayer::SetAbsAngles(QAngle angle)
{
	using SetAbsAnglesFn = void(__thiscall*)(void*, const QAngle& Angles);
	static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
	SetAbsAngles(this, angle);
}

Vector C_BasePlayer::GetHitboxPos1(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

Vector C_BasePlayer::GetHitboxPos2(int hitbox_id, matrix3x4_t* boneMatrix)
{
	auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
	if (studio_model) {
		auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
		if (hitbox) {
			auto
				min = Vector{},
				max = Vector{};

			Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
			Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

			return (min + max) / 2.0f;
		}
	}
	return Vector{};
}
void C_BasePlayer::SetAbsOrigin(const Vector& origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector& origin);
	static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
	SetAbsOrigin(this, origin);
}
Vector C_BasePlayer::GetHitboxPos3(int hitbox_id, matrix3x4_t* boneMatrix, float& Radius, Vector& Min, Vector& Max, int& Bone)
{
	auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
	if (studio_model) {
		auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
		if (hitbox) {
			auto
				min = Vector{},
				max = Vector{};

			Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
			Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

			Radius = hitbox->m_flRadius;
			Min = hitbox->bbmin;
			Max = hitbox->bbmax;
			Bone = hitbox->bone;

			return (min + max) / 2.0f;
		}
	}
	return Vector{};
}

void C_BasePlayer::SetAbsAngles(Vector angle)
{
	using SetAbsAnglesFn = void(__thiscall*)(void*, const Vector& Angles);
	static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
	SetAbsAngles(this, angle);
}

bool C_BaseEntity::IsLoot() {
	return GetClientClass()->m_ClassID == ClassId_CPhysPropAmmoBox ||
		GetClientClass()->m_ClassID == ClassId_CPhysPropLootCrate ||
		GetClientClass()->m_ClassID == ClassId_CPhysPropRadarJammer ||
		GetClientClass()->m_ClassID == ClassId_CPhysPropWeaponUpgrade ||
		GetClientClass()->m_ClassID == ClassId_CDrone ||
		GetClientClass()->m_ClassID == ClassId_CDronegun ||
		GetClientClass()->m_ClassID == ClassId_CItem_Healthshot ||
		GetClientClass()->m_ClassID == ClassId_CItemCash || 
		GetClientClass()->m_ClassID == ClassId_CBumpMine;
}

bool C_BaseEntity::IsWeapon()
{
	//index: 160
	//ref: "CNewParticleEffect::DrawModel"
	//sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 165 + VALVE_ADDED_FUNCS)(this);
}

float C_BasePlayer::GetMaxDesyncDelta() { // https://github.com/es3n1n/boynext_hook/blob/61deb6c399d8e6fb8030263ddddf8282a4f418aa/CSGOSimple/valve_sdk/csgostructs.cpp#L491
	auto animstate = uintptr_t(this->GetPlayerAnimState());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = std::max<float>(0, std::min<float>(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = std::max<float>(0, std::min<float>(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999)* speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}

float C_BasePlayer::DesyncValue() { // https://github.com/es3n1n/boynext_hook/blob/61deb6c399d8e6fb8030263ddddf8282a4f418aa/CSGOSimple/valve_sdk/csgostructs.cpp#L469
	if (!this)
		return 0.f;

	auto anim_state = this->GetPlayerAnimState();
	if (!anim_state)
		return 0.f;

	float duck_amount = anim_state->m_fDuckAmount;
	float speed_fraction = std::max< float >(0, std::min< float >(anim_state->m_flFeetSpeedForwardsOrSideWays, 1));
	float speed_factor = std::max< float >(0, std::min< float >(1, anim_state->m_flFeetSpeedUnknownForwardOrSideways));

	float yaw_modifier = (((anim_state->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * speed_fraction) + 1.0f;

	if (duck_amount > 0.f) {
		yaw_modifier += ((duck_amount * speed_factor) * (0.5f - yaw_modifier));
	}

	return anim_state->m_vVelocityY * yaw_modifier;
}

bool C_BasePlayer::IsReloading() {
	if (this) {
		for (int i = 0; i < 15; i++) {
			auto animlayer = this->GetAnimOverlay(i);
			auto sequence = this->GetSequenceActivity(animlayer->m_nSequence);
			if (sequence == 967 && animlayer->m_flWeight != 0.0f)
				return true;
		}
	}
	return false;
}

int C_BasePlayer::GetChockedPacketsAmount() {
	float simtime = m_flSimulationTime();
	float oldsimtime = m_flOldSimulationTime();
	if (simtime > oldsimtime)
		return TIME_TO_TICKS(fabs(simtime - oldsimtime));
	return 0;
}

float C_BasePlayer::m_flOldSimulationTime() {
	static uintptr_t offset = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flSimulationTimen") + 0x4;
	return *(float*)((DWORD)this + offset);
}

bool C_BaseEntity::IsPlantedC4()
{
	return GetClientClass()->m_ClassID == ClassId_CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit()
{
	return GetClientClass()->m_ClassID == ClassId_CBaseAnimating;
}

CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData()
{
	return g_WeaponSystem->GetWpnData(this->m_Item().m_iItemDefinitionIndex());
}

bool C_BaseCombatWeapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::CanFire()
{
	auto owner = this->m_hOwnerEntity().Get();
	if (!owner)
		return false;

	if (IsReloading() || m_iClip1() <= 0)
		return false;

	auto flServerTime = owner->m_nTickBase() * g_GlobalVars->interval_per_tick;

	if (owner->m_flNextAttack() > flServerTime)
		return false;


	return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::IsGrenade()
{
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_GRENADE;
}

bool C_BaseCombatWeapon::IsGun()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_C4:
		return false;
	case WEAPONTYPE_GRENADE:
		return false;
	case WEAPONTYPE_KNIFE:
		return false;
	case WEAPONTYPE_UNKNOWN:
		return false;
	default:
		return true;
	}
}

bool C_BaseCombatWeapon::IsKnife()
{
	if (this->m_Item().m_iItemDefinitionIndex() == WEAPON_TASER) return false;
	return GetCSWeaponData()->iWeaponType == WEAPONTYPE_KNIFE;
}

bool C_BaseCombatWeapon::IsRifle()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_RIFLE:
		return true;
	case WEAPONTYPE_SUBMACHINEGUN:
		return true;
	case WEAPONTYPE_SHOTGUN:
		return true;
	case WEAPONTYPE_MACHINEGUN:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsPistol()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_PISTOL:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsSniper()
{
	switch (GetCSWeaponData()->iWeaponType)
	{
	case WEAPONTYPE_SNIPER_RIFLE:
		return true;
	default:
		return false;
	}
}

bool C_BaseCombatWeapon::IsReloading()
{
	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
	return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 482 + VALVE_ADDED_FUNCS)(this);
}

float C_BaseCombatWeapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 452 + VALVE_ADDED_FUNCS)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 483 + VALVE_ADDED_FUNCS)(this);
}

CUtlVector<IRefCounted*>& C_BaseCombatWeapon::m_CustomMaterials()
{	static auto inReload = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "83 BE ? ? ? ? ? 7F 67") + 2) - 12;
	return *(CUtlVector<IRefCounted*>*)((uintptr_t)this + inReload);
}

bool* C_BaseCombatWeapon::m_bCustomMaterialInitialized()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "C6 86 ? ? ? ? ? FF 50 04") + 2);
	return (bool*)((uintptr_t)this + currentCommand);
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
	static auto currentCommand = *(uint32_t*)(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

int C_BasePlayer::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x298C);
}

AnimationLayer *C_BasePlayer::GetAnimOverlays()
{
	return *(AnimationLayer**)((DWORD)this + 0x2980);
}

AnimationLayer *C_BasePlayer::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];
	return nullptr;
}

int C_BasePlayer::GetSequenceActivity(int sequence)
{
	auto hdr = g_MdlInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.
	// Thanks @Kron1Q for merge request
	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::PatternScan(GetModuleHandle(L"client.dll"), "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return get_sequence_activity(this, hdr, sequence);
}

CCSGOPlayerAnimState *C_BasePlayer::GetPlayerAnimState()
{
	return *(CCSGOPlayerAnimState**)((DWORD)this + 0x3914);
}

void C_BasePlayer::UpdateAnimationState(CCSGOPlayerAnimState *state, QAngle angle)
{
	static auto UpdateAnimState = Utils::PatternScan(
		GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

	if (!UpdateAnimState)
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}

void C_BasePlayer::ResetAnimationState(CCSGOPlayerAnimState *state)
{
	using ResetAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*);
	static auto ResetAnimState = (ResetAnimState_t)Utils::PatternScan(GetModuleHandleA("client.dll"), "56 6A 01 68 ? ? ? ? 8B F1");
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void C_BasePlayer::CreateAnimationState(CCSGOPlayerAnimState *state)
{
	using CreateAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*, C_BasePlayer*);
	static auto CreateAnimState = (CreateAnimState_t)Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
	if (!CreateAnimState)
		return;

	CreateAnimState(state, this);
}

Vector C_BasePlayer::GetEyePos()
{
	return m_vecOrigin() + m_vecViewOffset();
}

player_info_t C_BasePlayer::GetPlayerInfo()
{
	player_info_t info;
	g_EngineClient->GetPlayerInfo(EntIndex(), &info);
	return info;
}

bool C_BasePlayer::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

bool C_BasePlayer::IsFlashed()
{
	static auto m_flFlashMaxAlpha = NetvarSys::Get().GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	return *(float*)((uintptr_t)this + m_flFlashMaxAlpha - 0x8) > 200.0;
}

bool C_BasePlayer::HasC4()
{
	static auto fnHasC4
		= reinterpret_cast<bool(__thiscall*)(void*)>(
			Utils::PatternScan(GetModuleHandleW(L"client.dll"), "56 8B F1 85 F6 74 31")//is_c4_owner
			);

	return fnHasC4(this);
}

Vector C_BasePlayer::GetHitboxPos(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

mstudiobbox_t* C_BasePlayer::GetHitbox(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
		auto studio_model = g_MdlInfo->GetStudiomodel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				return hitbox;
			}
		}
	}
	return nullptr;
}

bool C_BasePlayer::GetHitboxPos(int hitbox, Vector &output)
{
	if (hitbox >= HITBOX_MAX)
		return false;

	const model_t *model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
		return false;

	mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}

Vector C_BasePlayer::GetBonePos(int bone)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
		return boneMatrix[bone].at(3);
	}
	return Vector{};
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, int hitbox)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto endpos = player->GetHitboxPos(hitbox);

	ray.Init(GetEyePos(), endpos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init(GetEyePos(), pos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

float C_BasePlayer::CanSeePlayerD(C_BasePlayer* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	ray.Init(GetEyePos(), pos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.fraction;
}

void C_BasePlayer::UpdateClientSideAnimation()
{
	return CallVFunction<void(__thiscall*)(void*)>(this, 223 + VALVE_ADDED_FUNCS)(this);
}

void C_BasePlayer::InvalidateBoneCache()
{
	static DWORD addr = (DWORD)Utils::PatternScan(GetModuleHandleA("client.dll"), "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

	*(int*)((uintptr_t)this + 0xA30) = g_GlobalVars->framecount; //we'll skip occlusion checks now
	*(int*)((uintptr_t)this + 0xA28) = 0;//clear occlusion flags

	unsigned long g_iModelBoneCounter = **(unsigned long**)(addr + 10);
	*(unsigned int*)((DWORD)this + 0x2924) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)this + 0x2690) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
}

int C_BasePlayer::m_nMoveType()
{
	return *(int*)((uintptr_t)this + 0x25C);//m_MoveType
}

QAngle* C_BasePlayer::GetVAngles()
{
	static auto deadflag = NetvarSys::Get().GetOffset("DT_BasePlayer", "deadflag");
	return (QAngle*)((uintptr_t)this + deadflag + 0x4);
}

void C_BaseAttributableItem::SetGloveModelIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}



void C_BaseViewModel::SendViewModelMatchingSequence(int sequence)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 246 + VALVE_ADDED_FUNCS)(this, sequence);
}

float_t C_BasePlayer::m_flSpawnTime()
{
	return *(float_t*)((uintptr_t)this + 0xA360);
}

float CGlobalVarsBase::ServerTime(CUserCmd* cmd) {
	static int tick;
	static CUserCmd* lastCmd;

	if (cmd) {
		if (!lastCmd || lastCmd->hasbeenpredicted)
			tick = g_LocalPlayer->m_nTickBase();
		else
			tick++;
		lastCmd = cmd;
	}
	return tick * interval_per_tick;
}

KeyValues* KeyValues::FindKey(const char* keyName, bool bCreate)
{
	static auto key_values_find_key = reinterpret_cast<KeyValues * (__thiscall*)(void*, const char*, bool)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
	return key_values_find_key(this, keyName, bCreate);
}

const char* KeyValues::GetString(KeyValues* pThis, const char* keyName, const char* defaultValue)
{
	static auto key_values_get_string = reinterpret_cast<const char* (__thiscall*)(void*, const char*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));
	return key_values_get_string(pThis, keyName, defaultValue);
}

bool KeyValues::LoadFromBuffer(KeyValues* pThis, const char* pszFirst, const char* pszSecond, PVOID pSomething, PVOID pAnother, PVOID pLast, PVOID pAnother2)
{
	//.text : 1041E409                 call    LoadFromBuffer2 <----- here
	//.text : 1041E40E                 call    sub_10402010
	//.text : 1041E413                 push    offset FileName
	//.text : 1041E418                 push    offset aGame_1; "game"
	//.text : 1041E41D                 mov     ecx, [eax + 200h]
	//.text : 1041E423                 call    sub_10898B00
	//.text : 1041E428                 push    offset FileName
	//.text : 1041E42D                 push    offset aGame_1; "game"

	static auto callLoadBuff = reinterpret_cast<bool(__thiscall*)(KeyValues*, const char*, const char*, PVOID, PVOID, PVOID, PVOID)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));

	return callLoadBuff(pThis, pszFirst, pszSecond, pSomething, pAnother, pLast, pAnother2);
}

void KeyValues::SetString(const char* name, const char* value)
{
	auto keyvalues = FindKey(name, 1);

	static auto fnSetString = reinterpret_cast<void(__thiscall*)(KeyValues*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
	if (keyvalues)
		fnSetString(keyvalues, value);
}
void KeyValues::SetInt(const char* name, int value)
{
	auto keyvalues = FindKey(name, 1);

	if (keyvalues)
	{
		m_iValue = value;
		m_iDataType = 2;
	}
}

void KeyValues::SetFloat(const char* name, float value)
{
	auto keyvalues = FindKey(name, 1);

	if (keyvalues)
	{
		m_flValue = value;
		m_iDataType = 3;
	}
}

void C_BasePlayer::SetGloveModelIIndex(int modelIndex)
{
	return CallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

/*Vector& C_BasePlayer::GetAbsAngles()
{
	return CallVFunction<Vector & (__thiscall*)(void*)>(this, 11)(this);
}*/
Vector& C_BasePlayer::GetAbsAngles2()
{
	return CallVFunction<Vector & (__thiscall*)(void*)>(this, 11)(this);
}

void C_BasePlayer::SetAngle2(Vector wantedang)
{
	typedef void(__thiscall* SetAngleFn)(void*, const Vector&);
	static SetAngleFn SetAngle = (SetAngleFn)((DWORD)Utils::PatternScan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));//"name": "set_abs_angles"
	SetAngle(this, wantedang);
}
