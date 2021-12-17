#include "painttraverse.h"

using PaintTraverse_t = void( __thiscall* )( void*, unsigned, bool, bool );

void __fastcall HOOKED_PaintTraverse( void* ecx, void* edx, unsigned vguiPanel, bool forceRepaint, bool allowForce )
{
	const char* cPanelName = g_pPanel->GetName( vguiPanel );

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( g_Var.Visuals.World.Removals[ 2 ] && pLocal != nullptr && pLocal->bInScope() )
	{
		if( cPanelName && cPanelName[ 0 ] == 'H' && cPanelName[ 3 ] == 'Z' )
			return;
	}

	static auto dwOrgPaintTraverse = g_pPaintTraverseHOOKED->GetFunctionAddress<PaintTraverse_t>( 41 );
	dwOrgPaintTraverse( ecx, vguiPanel, forceRepaint, allowForce );

	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize( iScreenWidth, iScreenHeight );

	if( cPanelName && cPanelName[ 0 ] == 'E' && cPanelName[ 7 ] == 'T' )
	{
		static int iResolution = 0;

		if( iResolution == 0 )
			iResolution = iScreenWidth + iScreenHeight;

		if( iScreenWidth + iScreenHeight != iResolution )
		{
			g_Draw.InitFonts();
			iResolution = 0;
		}

		g_EventLogs.Init();

		if( pLocal != nullptr && pLocal && g_pEngine->IsInGame() )
		{
	//		if( pLocal->GetHealth() > 0 )
	//		{
				static int iCount = 0;
				iCount++;

				if( iCount > 150 )
				{
					g_Visuals.bNewMap = false;
					iCount = 0;
				}
	//		}

			g_Misc.ClanTag(pLocal);

			g_Visuals.Run( pLocal );

			if( g_Var.Visuals.Player.Spectators )
				g_Visuals.SpecList( pLocal, iScreenWidth, iScreenHeight );

			if( g_Var.Visuals.World.Grenade[ 2 ] )
				g_pGrenadePrediction->PaintGrenadeTrajectory( pLocal );

			g_Hitmarker.Init();

			g_Hitmarker.InitBulletHit( pLocal );

			g_Visuals.RunMaterials();

			g_Misc.DoRagdoll();

//			g_Draw.DrawString( g_Draw.visualsfont, false, iScreenWidth / 2, iScreenHeight / 2, 255, 255, 255, 255, "%3.1f", pLocal->DuckAmount());

/*			CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

			if (pWeapon)
			{
				CWeaponData* pWeaponData = pWeapon->GetWeaponData();

				if (pWeaponData)
					g_Draw.DrawString(g_Draw.visualsfont, false, iScreenWidth / 2, iScreenHeight / 2, 255, 255, 255, 255, "%3.3f - %3.3f", pWeaponData->movement_speed / 3.f, pWeaponData->movement_speed_zoom / 3.f);
			}*/

		/*	{
				auto CanDamage = [&](Vector vPoint, float& flDamage, CBaseEntity * pEnemy, CBaseCombatWeapon* pWeapon) -> bool
				{
					CAutowall::FireBulletData BulletData = CAutowall::FireBulletData(vPoint);
					BulletData.filter.pSkip = pLocal;

					Vector vAngle;
					g_Math.CalcAngle(BulletData.src, g_Math.GetHitspot(pEnemy, 0), vAngle);
					g_Math.AngleVector(vAngle, BulletData.direction);
					VectorNormalize(BulletData.direction);

					if (g_Autowall.SimulateFireBullet(pLocal, pEnemy, pWeapon, BulletData, 0))
						return true;

					flDamage = BulletData.current_damage;

					return false;
				};

				for( int i = 1; i <= 32; i++ )
				{
					CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

					if( !pEnemy->PlayerCheck() )
						continue;

					if( pEnemy == pLocal )
						continue;

					if( pEnemy->GetTeamNumber() == pLocal->GetTeamNumber() )
						continue;

					CBaseCombatWeapon* pWeapon = pEnemy->GetWeapon();

					if (!pWeapon)
						continue;

					Vector vEye = pLocal->GetEyePosition();
					Vector vHead = g_Math.GetHitspot(pLocal, 0);
					Vector vOrigin = pLocal->GetAbsOrigin();

					float flBestRot = 0.f;
					float flStep = M_PI * 2.0 / g_Var.AntiAim.Points;
					float flAdd = pLocal->GetVelocity().Length() > 0.1 ? 8.f : 3.f;
					float flRadius = Vector(vHead - vOrigin).Length2D() + flAdd;
					float flBestDamage = 15.f;
					float flBestThickness = 0.f;

					for (float flRotation = 0; flRotation < (M_PI * 2.0); flRotation += flStep)
					{
						Vector vNewHead(flRadius * cos(flRotation) + vEye.x, flRadius * sin(flRotation) + vEye.y, vEye.z);

						float flDamage = 0;

						if (CanDamage(vNewHead, flDamage, pEnemy, pWeapon))
							g_pDebugOverlay->AddSweptBoxOverlay(vNewHead, vNewHead, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), 0, 255, 0, 255, g_pGlobalVars->interval_per_tick);
						else
							g_pDebugOverlay->AddSweptBoxOverlay(vNewHead, vNewHead, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), 255, 0, 0, 255, g_pGlobalVars->interval_per_tick);
					}
				}
			}*/
		}

		g_Gui.DrawMenu();
	}
}

using LockCursor_t = void( __thiscall* )( void * );

void HOOKED_LockCursor() 
{
	static auto dwOriginal = g_pSurfaceHOOKED->GetFunctionAddress< LockCursor_t>( 67 );

	if( g_Gui.bCursorActive )
		g_pSurface->UnlockCursor();
	else
		dwOriginal( g_pSurface );
}