#include "drawmodelstatsoverlay.h"

bool __fastcall HOOKED_DrawModelStats( PVOID pConVar, PVOID edx )
{
	//A1 ? ? ? ? B9 ? ? ? ? FF 50 34 85 C0 75 1D
/*	static DWORD dwStatiPropRetn = 0;

	if( !dwStatiPropRetn )
	{
		dwStatiPropRetn = g_Pattern.PatternSearchIDA( "engine.dll", "B9 ? ? ? ? FF 50 34 85 C0 75 03 32 C0" );
		dwStatiPropRetn += 8;
	}*/

	static DWORD dwWeaponsRetn = 0;

	if( !dwWeaponsRetn )
	{
		dwWeaponsRetn = g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "56 8B F1 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 80 BE ? ? ? ? ? 0F 85 ? ? ? ? 8B 0D" );
		dwWeaponsRetn += 96;
	}

/*	if( ( DWORD ) _ReturnAddress() == dwStatiPropRetn && g_Var.Visuals.World.ColorWorld > 0 )
		return true;*/

	if( ( DWORD ) _ReturnAddress() == dwWeaponsRetn && g_Var.Visuals.Chams.Weapons > 0 )
		return true;

	typedef bool( __thiscall* OriginalFuncFn )( PVOID );
	OriginalFuncFn dwOriginal = ( OriginalFuncFn ) dwModelStatsVMT;
	
	return dwOriginal( pConVar );
}
