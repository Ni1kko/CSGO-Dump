#include "viewmodel.h"

float __fastcall ViewmodelHook( void* ecx, void* edx )
{
	float flOriginal = ViewModelOriginal( ecx );

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal == nullptr || pLocal->GetHealth() <= 0 || !g_pEngine->IsInGame() )
		return ViewModelOriginal( ecx );

	if( !pLocal->bInScope() )
		return flOriginal += g_Var.Visuals.World.ViewModel;

	return ViewModelOriginal( ecx );
}