#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"

namespace Globals
{
    extern CUserCmd*      pCmd;
    extern C_BaseEntity*  LocalPlayer;
    extern std::uintptr_t uRandomSeed;
	extern Vector         OriginalView;
	extern bool           bSendPacket;
	extern bool			  LagPeek;
	extern int            TargetIndex;
	extern Vector         EnemyEyeAngs[65];
	extern Vector         AimbotHitbox[65][27];
	extern Vector         RealAngle;
	extern Vector         FakeAngle;
	extern bool           Shot[65];
	extern bool           Hit[65];
	extern int            MissedShots[65];
	extern bool			  Manual_Side;
	extern bool			FakeDuck[65];
	extern bool		   MissedShot;
	extern bool DesyncEnabledSpecialChecks;
	extern bool HitPlayer;
	extern std::string Username;
	extern DWORD CourierNew;
	extern DWORD SmallText;
	extern DWORD WeaponESP;
	extern DWORD WeaponIcon;
	extern DWORD TabFont;
	extern DWORD IndicatorFont;
	extern HMODULE	       Module;
	extern                float w2s_matrix[4][4];
}