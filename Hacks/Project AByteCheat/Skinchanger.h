#pragma once

#include "Interfaces.h"
#include "Hooks.h"
#include "ClientRecvProps.h"

#include <unordered_map>

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

#define	LIFE_ALIVE 0
inline void**& getvtable(void* inst, size_t offset = 0)
{

	return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
	if (!inst && !offset)
		return NULL;
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template< typename Fn >
inline Fn fvv(const void* inst, size_t index, size_t offset = 0)
{
	if (!inst && offset == 0)
		return NULL;

	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}
class CBaseViewModel : public IClientUnknown, public IClientRenderable, public IClientNetworkable {
public:
	inline int GetModelIndex() {
		return *(int*)((DWORD)this + 0x258);
	}
	inline void SetModelIndex(int val) {
		*(int*)((DWORD)this + 0x258) = val;
	}
	inline DWORD GetOwner() {
		return *(PDWORD)((DWORD)this + 0x29CC);
	}
	inline DWORD GetWeapon() {
		return *(PDWORD)((DWORD)this + 0x29C8);
	}
	inline void SetWeaponModel(const char* Filename, IClientUnknown* Weapon) {
		return fvv<void(__thiscall*)(void*, const char*, IClientUnknown*)>(this, 243)(this, Filename, Weapon);
	}
};

struct SkinStruct {
	int nItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int nFallbackSeed = 0;
	int nFallbackStatTrak = -1;
	int nEntityQuality = 0;
	float flFallbackWear = 0.1f;
	char* szWeaponName = nullptr;
};

extern std::unordered_map<int, SkinStruct>  m_skins;
extern std::unordered_map<int, const char*> g_ViewModelCFG;
extern std::unordered_map<const char*, const char*> g_KillIconCfg;

class SkinChanger
{
public:
	int get_team(IClientEntity * local);
	int get_skin_id_knife();
	void get_skin_id_sniperAWP();
	void get_skin_id_sniperScar();
	void update_settings();
	void set_skins();
	void set_viewmodel();
	bool apply_skins(CBaseAttributableItem* pWeapon, int nWeaponIndex);
	bool apply_viewmodel(IClientEntity* pLocal, CBaseAttributableItem* pWeapon, int nWeaponIndex);
	bool apply_killcon(IGameEvent* pEvent);

	int scar20_paint;
	int g3sg1_paint;
	int awp_paint;
};

extern SkinChanger skinchanger;