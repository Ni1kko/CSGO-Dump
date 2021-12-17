#include "overrideview.h"

OverrideViewFn_t oOverrideView;
void __fastcall HOOKED_OverrideView( void* thisptr, void* edx, CViewSetup* setup )
{
	static DWORD dwOrgOverrideView = g_pClientModeHOOKED->dwGetMethodAddress( 18 );

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal == nullptr || !g_pEngine->IsInGame() )
	{
		oOverrideView( thisptr, setup );
		return;
	}

	g_Misc.DoThirdperson( pLocal );

	static ConVar* zoom_sensitivity_ratio_mouse = g_pICvar->FindVar("zoom_sensitivity_ratio_mouse");
	static float old_zoom_sense;

	if (zoom_sensitivity_ratio_mouse->GetFloat() > 0.f)
		old_zoom_sense = zoom_sensitivity_ratio_mouse->GetFloat();

	if( pLocal->GetHealth() > 0 )
	{
		if( g_Var.Visuals.World.Grenade[ 2 ] )
			g_pGrenadePrediction->PredictGrenade( pLocal );

//		setup->origin.x += g_Var.Visuals.World.ViewModelOffset[0];
//		setup->origin.y += g_Var.Visuals.World.ViewModelOffset[1];
//		setup->origin.z += g_Var.Visuals.World.ViewModelOffset[2];

		if (!pLocal->bInScope())
		{
			setup->fov += g_Var.Visuals.World.Fieldofview;
			zoom_sensitivity_ratio_mouse->SetValue(old_zoom_sense);
		}
		else
		{
			CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

			if (pWeapon && g_Var.Visuals.World.Removals[4])
			{
				if (pWeapon->GetZoomLevel() == 1 && pWeapon->IsSniper())
				{
					setup->fov = 90.f;
					zoom_sensitivity_ratio_mouse->SetValue(0.f);
				}
				else
					zoom_sensitivity_ratio_mouse->SetValue(old_zoom_sense);
			}				
		}

		if (UserCmd::bInFakeDuck)
			setup->origin.z = pLocal->GetAbsOrigin().z + 64.f;
	}

	oOverrideView( thisptr, setup );
}