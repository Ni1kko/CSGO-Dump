#include "hooks.hpp"

struct RenderableInfo_t
{
	IClientRenderable* m_pRenderable;
	void* m_pAlphaProperty;
	int m_EnumCount;
	int m_nRenderFrame;
	unsigned short m_FirstShadow;
	unsigned short m_LeafList;
	short m_Area;
	uint16_t m_Flags;   // 0x0016
	uint16_t m_Flags2; // 0x0018
	Vector m_vecBloatedAbsMins;
	Vector m_vecBloatedAbsMaxs;
	Vector m_vecAbsMins;
	Vector m_vecAbsMaxs;
	int pad;
};

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

int __fastcall Hooks::ListLeavesInBox(void* _this, int, Vector& mins, Vector& maxs, unsigned short* pList, int listMax)
{
	static auto ofunc = bspquery_hook.get_original<decltype(&ListLeavesInBox)>(index::ListLeavesInBox);

	if (reinterpret_cast<uintptr_t>(_ReturnAddress()) != reinterpret_cast<uintptr_t>(g_Memory->ListLeavesInBox))
		return ofunc(_this, 0, mins, maxs, pList, listMax);

	auto info = *reinterpret_cast<RenderableInfo_t * *>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);
	if (!info || !info->m_pRenderable)
		return ofunc(_this, 0, mins, maxs, pList, listMax);

	auto ent = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
	if (ent && ent->IsPlayer())
	{
		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;

		static Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
		static Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);

		return ofunc(_this, 0, map_min, map_max, pList, listMax);
	}
	return ofunc(_this, 0, mins, maxs, pList, listMax);
}