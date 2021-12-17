#include "inprediction.h"

InPrediction_t oInPrediction;

bool __fastcall HOOKED_InPrediction( void* ecx, void* )
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	static DWORD dwCalcPlayerView = g_Pattern.PatternSearch( g_Offset.szClient.c_str(), ( PBYTE )"\x84\xC0\x75\x08\x57\x8B\xCE\xE8\x00\x00\x00\x00\x8B\x06", "xxxxxxxx????xx", NULL, NULL );

	if( dwCalcPlayerView && dwCalcPlayerView == uintptr_t( _ReturnAddress() ) && pLocal != nullptr )
	{
		CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

		for( int i = 1; i < 33; i++ )
		{
			CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

			if( !pEnemy || pEnemy == pLocal )
				continue;

			if( pEnemy->GetTeamNumber() == pLocal->GetTeamNumber() )
				continue;

			//we wanna set last setupbones time to float nan will cause that all setupbones calls even game ones will break
			//only way to get setupbones to work then is by calling invalidate bone cache before calling setupbones
			//we do that because we dont wanna have a fight between cheat and game and it also gives us some frames

			if( pWeapon != nullptr && g_Var.Ragebot[ pWeapon->GetGroupID() ].PosAdjust > 0 && !pEnemy->IsDormant() && pEnemy->IsAlive() && pLocal->IsAlive() )
				*( uint32_t* ) ( uintptr_t( pEnemy ) + 0x2914 ) = 0x7FC00000;
			else
				if( *( uint32_t* ) ( uintptr_t( pEnemy ) + 0x2914 ) == 0x7FC00000 )
					pEnemy->InvalidateBoneCache();
		}
	}

	return oInPrediction( ecx );
}