#include "IsEntBreakableHook.hpp"
#include "Detour.h"
#include "awall.h"
#include "patternscan.h"

namespace IsEntBreakableHook
{
	Detour _d;
	using fn_prototype = bool(__thiscall*)(void*);

#pragma optimize("", off)
	bool __fastcall HOOKED_IsEntBreakable( void* entity, void* edx )
	{
		auto org_return = _d.GetOriginal<fn_prototype>()( entity );

		__asm pushad
		if( entity && !org_return )
		{
			CBaseEntity* pEntity = ( CBaseEntity* ) ( entity );
			CClientClasses* pClientClass = pEntity->GetClientClasses();

			if( pClientClass )
			{
				char* cName = pClientClass->m_cName;

				if( *reinterpret_cast<uint32_t*>( cName ) == 0x65724243 && *reinterpret_cast<uint32_t*>( cName + 0x7 ) == 0x53656C62 )
					org_return = true;
			}
		}
		__asm popad

		return org_return;
	}
#pragma optimize("", on)

	void SetupHook()
	{
		auto IsBreakableOrg = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 51 56 8B F1 85 F6 74 68");
		if (!IsBreakableOrg)
			throw "Failed to signature scan IsBreakable for autowall!";
		_d.SetupHook(reinterpret_cast<uint8_t*>(IsBreakableOrg), reinterpret_cast<uint8_t*>(HOOKED_IsEntBreakable));
	}

	void RemoveHook()
	{
		_d.Detach();
	}
}
