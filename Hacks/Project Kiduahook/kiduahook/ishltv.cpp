#include "ishltv.h"

bool __fastcall HOOKED_IsHLTV()
{
	static DWORD dwOriginIsHLTV = g_pEngineHOOKED->dwGetMethodAddress( 94 );

	__asm PUSHAD

	//rtn addr  84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80
	//fixing velocity

	static DWORD dwAbsVelRtn = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80");

	if( ( DWORD )_ReturnAddress() == dwAbsVelRtn)
	{
		__asm
		{
			POPAD;
			CALL dwOriginIsHLTV;
			MOV AL, 1
		}
	}
	else
	{
		__asm
		{
			POPAD;
			CALL dwOriginIsHLTV
		}
	}
}