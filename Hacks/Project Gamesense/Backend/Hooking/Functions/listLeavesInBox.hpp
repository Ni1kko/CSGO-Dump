#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/CMaterialHelper.h"
#include "../../DLL/Engine.h"


using listLeavesInBoxfn = int(__thiscall*)(void*, const Vector&, const Vector&, unsigned short*, int);
listLeavesInBoxfn olistLeavesInBox = nullptr;

struct RenderableInfo {
	CBaseEntity* renderable;
	std::byte pad[18];
	uint16_t flags;
	uint16_t flags2;
};
#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
namespace Cheat
{
	namespace Hooked
	{
		int __fastcall listLeavesInBox(void* bsp, void* edx, const Vector& mins, const Vector& maxs, unsigned short* list, int list_max)
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.listLeavesInBox = 0;
				return olistLeavesInBox(bsp, mins, maxs, list, list_max);
			}
			Cheat::Settings->UnloadReadyTable.listLeavesInBox = (int)GetTickCount();

			static auto list_leaves = Utilities->Memory_PatternScan(Settings->ClientDLL, xorstr_("56 52 FF 50 18")) + 5;
			auto info = *reinterpret_cast<RenderableInfo**>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

			if (!c_config::get()->b["vis_chamsocclusion"])
				return olistLeavesInBox(bsp, mins, maxs, list, list_max);

            if (reinterpret_cast<uintptr_t>(_ReturnAddress()) != list_leaves)
                return olistLeavesInBox(bsp, mins, maxs, list, list_max);

            if (!info || !info->renderable)
                return olistLeavesInBox(bsp, mins, maxs, list, list_max);

            auto entity = info->renderable;

            if (!entity || !entity->IsPlayer())
                return olistLeavesInBox(bsp, mins, maxs, list, list_max);;

            info->flags &= ~0x100;
            info->flags2 |= 0xC0;

            static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
            static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);

            return olistLeavesInBox(bsp, map_min, map_max, list, list_max);
		}
	}
}