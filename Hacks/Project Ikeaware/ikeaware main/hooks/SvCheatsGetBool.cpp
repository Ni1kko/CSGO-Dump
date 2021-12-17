#include "hooks.hpp"

bool __fastcall Hooks::SvCheatsGetBool(void* pConVar, void* edx)
{
	static auto ofunc = sv_cheats.get_original<decltype(&SvCheatsGetBool)>(index::GetBool);
	if (!ofunc)
		return false;

	if (_ReturnAddress() == g_Memory->CamThink)
		return true;

	return ofunc(pConVar, 0);
}