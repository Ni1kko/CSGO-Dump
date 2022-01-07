#include "UTIL Functions.h"
#include "Utilities.h"
#include "Menu.h"
#include "Hacks.h"
#include "Autowall.h"

#include "RenderManager.h"

ServerRankRevealAllFn GameUtils::ServerRankRevealAllEx;

DWORD GameUtils::FindPattern1(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2; 
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

std::uint8_t* GameUtils::pattern_scan(void* module, const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector< int >{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}

	// Afterwards call server to stop dispatch of cheat and to alert us of update.
	//ConsolePrint(true, "A pattern has outtdated: %s", signature);
	return nullptr;
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
void UTIL_trace(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
void ClipToPlayer(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = Utilities::Memory::FindPattern("client_panorama.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");
	if (!dwAddress)
		return; 
	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}
void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{

		static DWORD dwAddress = Utilities::Memory::FindPatternV2("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

		if (!dwAddress)
			return;

		_asm
		{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
		}
	
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPatternV2(("client_panorama.dll"), ("55 8B EC 83 EC 30 F3 0F 10 75"));

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

bool trace_to_exit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPatternV2(("client_panorama.dll"), ("55 8B EC 83 EC 30 F3 0F 10 75"));

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

void GameUtils::NormaliseViewAngle(Vector &angle)
{
	
		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	
}

void GameUtils::CL_FixMove(CUserCmd* pCmd, Vector viewangles)
{




}

char shit[16];
trace_t Trace;
char shit2[16];
IClientEntity* entCopy;

bool GameUtils::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	entCopy = pEntity;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);


	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == entCopy)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

bool screen_transform(const Vector& point, Vector& screen)
{
	const matrix3x4& w2sMatrix = interfaces::engine->WorldToScreenMatrix();
	screen.x = w2sMatrix[0][0] * point.x + w2sMatrix[0][1] * point.y + w2sMatrix[0][2] * point.z + w2sMatrix[0][3];
	screen.y = w2sMatrix[1][0] * point.x + w2sMatrix[1][1] * point.y + w2sMatrix[1][2] * point.z + w2sMatrix[1][3];
	screen.z = 0.0f;

	float w = w2sMatrix[3][0] * point.x + w2sMatrix[3][1] * point.y + w2sMatrix[3][2] * point.z + w2sMatrix[3][3];

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;

	return false;
}
bool GameUtils::World2Screen(const Vector & origin, Vector & screen)
{
	if (!screen_transform(origin, screen)) {
		int iScreenWidth, iScreenHeight;
		interfaces::engine->GetScreenSize(iScreenWidth, iScreenHeight);
		screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
		screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

		return true;
	}
	return false;
}

bool GameUtils::IsBomb(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CC4)
		return true;
	else
		return false;
}

bool GameUtils::IsGrenade(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INC };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsRevolver(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


void vector_transform(const Vector in1, float in2[3][4], Vector &out)
{
	out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}



Vector GameUtils::get_hitbox_location(IClientEntity* obj, int hitbox_id) {
	matrix3x4 bone_matrix[128];
	if (obj->IsAlive()) {
		if (obj->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {
			if (obj->GetModel()) {
				auto studio_model = interfaces::model_info->GetStudiomodel(obj->GetModel());
				if (studio_model) {
					auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
					if (hitbox) {
						auto min = Vector{}, max = Vector{};

						vector_transform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
						vector_transform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

						return (min + max) / 2.0f;
					}
				}
			}
		}
		return Vector{};
	}
}

bool GameUtils::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_DEAGLE,WEAPON_CZ75,WEAPON_ELITE,WEAPON_USPS,WEAPON_P250,WEAPON_P2000, WEAPON_TEC9,WEAPON_REVOLVER,WEAPON_FIVESEVEN,WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}



bool GameUtils::AutoSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::LightSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsRifle(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_M4A4, WEAPON_AK47, WEAPON_AUG,  WEAPON_FAMAS,  WEAPON_GALIL, WEAPON_M4A1S, WEAPON_SG553 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool GameUtils::IsShotgun(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_NOVA, WEAPON_SAWEDOFF, WEAPON_XM1014, WEAPON_MAG7 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsMachinegun(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_NEGEV, WEAPON_M249 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsMP(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_MP5SD, WEAPON_BIZON, WEAPON_P90, WEAPON_MAC10, WEAPON_MP7, WEAPON_MP9, WEAPON_UMP45 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::AWP(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponAWP)
		return true;
	else
		return false;
}

bool GameUtils::IsZeus(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponTaser)
		return true;
	else
		return false;
}

bool GameUtils::IsScout(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponSSG08)
		return true;
	else
		return false;
}
bool GameUtils::IsScopedWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20, WEAPON_AUG, WEAPON_SG553 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	interfaces::engine->ClientCmd_Unrestricted(buffer);
}

QAngle CalcAngleA(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	VectorAngles(delta, angles);
	angles.NormalizeX();
	return angles;
}
void AngleVectorsA(const QAngle &angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
Vector MultipointVectors[] = { Vector(0,0,0), Vector(0,0,1.5),Vector(0,0,3),Vector(0,0,4), Vector(0,0,-2), Vector(0,0,-4), Vector(0,0,4.8), Vector(0,0,5), Vector(0,0,5.4), Vector(0,3,0), Vector(3,0,0),Vector(-3,0,0),Vector(0,-3,0), Vector(0,2,4.2), Vector(0,-2,4.2), Vector(2,0,4.2), Vector(-2,0,4.2),  Vector(3.8,0,0), Vector(-3.8,0,0),Vector(0,3.6,0), Vector(0,-3.6,0), Vector(0,1.2,3.2), Vector(0,0.6,1.4), Vector(0,3.1,5.1), Vector(0,0,6.2), Vector(0,2.5,0), Vector(2.1,2.1,2.1) };

Vector GetHitboxPosition(IClientEntity* obj, int Hitbox)
{
	matrix3x4 bone_matrix[128];
	if (obj->SetupBones(bone_matrix, 128, 0x00000100, obj->GetSimulationTime())) {
		if (obj->GetModel())
		{
			auto studio_model = interfaces::model_info->GetStudiomodel(obj->GetModel());
			if (studio_model)
			{
				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(Hitbox);
				Vector Point[] =
				{
					Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
					Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
					Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
					Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
					Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
					Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
					Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
					Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z)
				};
				Vector vMin, vMax, vCenter, sCenter;

				int head = options::menu.aimbot_tab.TargetPointscale.GetValue();
				int body = options::menu.aimbot_tab.Multival.GetValue();
				int legs = options::menu.aimbot_tab.TargetPointscale.GetValue();

				VectorTransform(hitbox->bbmin, bone_matrix[hitbox->bone], vMin);
				VectorTransform(hitbox->bbmax, bone_matrix[hitbox->bone], vMax);

				vCenter = ((vMin + vMax) *0.5f);
				int iCount = 7;

				for (int i = 0; i <= iCount; i++)
				{
					Vector vTargetPos;
					switch (i)
					{
					case 0:
					default:
						vTargetPos = vCenter; break;

					case 1:
					{
						vTargetPos = (Point[7] + Point[1]) * (head / 100);
					}
					break;

					case 2:
					{
						vTargetPos = (Point[3] + Point[4]) * (body / 100);
					}
					break;

					case 3:
					{
						vTargetPos = (Point[4] + Point[0]) * (body / 100);
					}
					break;

					case 4:
					{
						vTargetPos = (Point[2] + Point[7]) * (35 / 100);
					}
					break; 

					case 5:
					{
						vTargetPos = (Point[4] + Point[0]) * (legs / 100);
					}
					break;
					case 6:
						vTargetPos = (Point[5] + Point[3]) * 0.6; 
						break;
					case 7:
						vTargetPos = (Point[1] + Point[2]) * 0.6; 
						break;
				//	default: vTargetPos = vCenter;
					}
					return vTargetPos;
				}

			}

		}
	}
	return Vector{};
}

Vector hitbox_location(IClientEntity* obj, int hitbox_id)
{
	matrix3x4 bone_matrix[128];

	if (obj->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {
		if (obj->GetModel()) {
			auto studio_model = interfaces::model_info->GetStudiomodel(obj->GetModel());
			if (studio_model) {
				auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
				if (hitbox) {
					auto min = Vector{}, max = Vector{};

					VectorTransform(hitbox->bbmin, bone_matrix[hitbox->bone], min);
					VectorTransform(hitbox->bbmax, bone_matrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
	}
	return Vector{};
}


Vector GetEyePosition(IClientEntity* pEntity)
{
	Vector vecViewOffset = *reinterpret_cast<Vector*>(reinterpret_cast<int>(pEntity) + 0x104);

	return pEntity->GetOrigin() + vecViewOffset;
}

int GameUtils::GetPlayerCompRank(IClientEntity* pEntity)
{
	DWORD m_iCompetitiveRanking = NetVar.GetNetVar(0x75671F7F); 
	DWORD GameResources = *(DWORD*)(Utilities::Memory::FindPatternV2("client_panorama.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 0x2);
	
	return *(int*)((DWORD)GameResources + 0x1A44 + (int)pEntity->GetIndex() * 4);
}

extern void GameUtils::ServerRankRevealAll()
{
	static float fArray[3] = { 0.f, 0.f, 0.f };

	GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(Offsets::Functions::dwGetPlayerCompRank);
	//GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(offsets.ServerRankRevealAllEx);
	GameUtils::ServerRankRevealAllEx(fArray);
}

void ForceUpdate()
{
	// Shh
	static DWORD clientstateaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45\xF8\x83\xBF\xE8\x00\x00\x00\x02", "xx????xxxxxxxxxxxxxx");
	static uintptr_t pEngineBase = (uintptr_t)GetModuleHandleA("engine.dll");

	static uintptr_t pClientState = **(uintptr_t**)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2);

	static uintptr_t dwAddr1 = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????");

	//E8 call is being used here
	static uintptr_t dwRelAddr = *(uintptr_t*)(dwAddr1 + 1);
	static uintptr_t sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

	__asm
	{
		pushad
		mov edi, pClientState
		lea ecx, dword ptr[edi + 0x8]
		call sub_B5E60
		mov dword ptr[edi + 0x154], 0xFFFFFFFF
		popad
	}
}
























































































































































































