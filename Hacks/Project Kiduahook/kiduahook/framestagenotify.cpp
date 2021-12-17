#include "framestagenotify.h"

using FrameStageNotify_t = void( __stdcall* )( ClientFrameStage_t );

void __stdcall HOOKED_FrameStageNotify( ClientFrameStage_t curstage )
{
	static auto dwOrgFrameStageNotify = g_pClientHOOKED->GetFunctionAddress< FrameStageNotify_t >( 37 );

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal == nullptr || !g_pEngine->IsConnected() || !g_pEngine->IsInGame() )
	{
//    	g_History.ClearSequence();

		dwOrgFrameStageNotify( curstage );

		return;
	}

	g_Visuals.bVisualRecoil = false;

	switch( curstage )
	{
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:

//		g_History.UpdateSequence();

		g_SkinChanger.ChangeGloves(pLocal);

		if( pLocal->GetHealth() > 0 )
		{
			g_SkinChanger.ChangePattern( pLocal );
//			g_SkinChanger.ChangeGloves( pLocal );
		}

		//g_History.DisableInterpolation(pLocal);
		break;

	case FRAME_RENDER_START:

		if( pLocal->GetHealth() > 0 )
		{
			g_Misc.ThirdPersonAngle( pLocal );
			g_Misc.UpdateLocalAnimations(pLocal);

			if( g_Var.Visuals.World.Removals[ 3 ] )
				g_Visuals.bVisualRecoil = g_Visuals.HasVisualRecoil( pLocal );
		}

		break;
	}
	
	dwOrgFrameStageNotify( curstage );

	if( curstage == FRAME_NET_UPDATE_END )
	{
		if( pLocal->GetHealth() > 0 )
			g_Misc.FixUpPunch( pLocal );

		g_History.Store( pLocal, curstage );
	}

	if( g_Visuals.bVisualRecoil && pLocal->GetHealth() > 0 )
	{
		if( g_Visuals.vAimPunch && g_Visuals.vViewPunch )
		{
			*g_Visuals.vAimPunch = g_Visuals.vOldAimPunch;
			*g_Visuals.vViewPunch = g_Visuals.vOldViewPunch;
		}
	}
}