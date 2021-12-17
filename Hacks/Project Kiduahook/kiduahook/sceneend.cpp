#include "sceneend.h"

using SceneEnd_t = void( __fastcall* )( void*, void* );

void __fastcall HOOKED_SceneEnd( void * ecx, void * edx )
{
	static auto dwOrgSceneEnd = g_pRenderViewHOOKED->GetFunctionAddress< SceneEnd_t >( 9 );

	dwOrgSceneEnd( ecx, edx );

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );
	static IMaterial* pPlayerMat;
	static IMaterial* pLocalMat;

	if( pLocal == nullptr || !g_pEngine->IsInGame() || g_pRenderView == nullptr || g_pMatsystem == nullptr )
	{
		pPlayerMat = nullptr;
		pLocalMat = nullptr;
		return;
	}

	if( !g_Var.Visuals.Chams.Enable )
		return;

	if( g_Var.Visuals.Chams.Players > 0 )
	{
		if( g_Var.Visuals.Chams.Players == 1 )
			pPlayerMat = g_pMatsystem->FindMaterial( "debug/debugambientcube", TEXTURE_GROUP_MODEL );
		else
			if( g_Var.Visuals.Chams.Players == 2 )
				pPlayerMat = g_pMatsystem->FindMaterial( "debug/debugdrawflat", TEXTURE_GROUP_MODEL );
			else
				if( g_Var.Visuals.Chams.Players == 3 )
					pPlayerMat = g_pMatsystem->FindMaterial( "effects/bubble", TEXTURE_GROUP_MODEL );
				else
					if( g_Var.Visuals.Chams.Players == 4 )
						pPlayerMat = g_pMatsystem->FindMaterial( "models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER );

		if( pPlayerMat == nullptr || pPlayerMat->IsErrorMaterial() )
			return;

		if( g_Var.Visuals.Chams.Players == 3 )
		{
			pPlayerMat->IncrementReferenceCount();
			pPlayerMat->SetMaterialVarFlag( MATERIAL_VAR_VERTEXCOLOR, false );
			pPlayerMat->SetMaterialVarFlag( MATERIAL_VAR_VERTEXALPHA, false );
		}

		g_pEngineModel->ForcedMaterialOverride( pPlayerMat );

		for( int i = 1; i <= 32; i++ )
		{
			CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

			if( !pEnemy->PlayerCheck() )
				continue;

			if( pEnemy == pLocal )
				continue;

			if (g_Misc.bInSameTeam(pLocal, pEnemy))
			{
				g_pRenderView->SetBlend(g_Var.Visuals.Chams.TeamTransparency / 100.f);

				if(!g_Var.Visuals.Chams.Team)
					continue;
			}

			float r, g, b;

			if( !g_Misc.bInSameTeam(pLocal, pEnemy))
				r = g_Var.Color.Chams[ 0 ], g = g_Var.Color.Chams[ 1 ], b = g_Var.Color.Chams[ 2 ];
			else
				r = g_Var.Color.TeamChams[ 0 ], g = g_Var.Color.TeamChams[ 1 ], b = g_Var.Color.TeamChams[ 2 ];

			if( g_Var.Visuals.Chams.DrawOriginal )
				pEnemy->DrawModel();

			g_pRenderView->SetBlend( g_Var.Visuals.Chams.Transparency / 100.f );

			g_pRenderView->SetColorRGB( r, g, b );

			pPlayerMat->IncrementReferenceCount();
			pPlayerMat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );

			pEnemy->DrawModel();

			float rvis, gvis, bvis;

			if( !g_Misc.bInSameTeam(pLocal, pEnemy))
				rvis = g_Var.Color.ChamsVisible[ 0 ], gvis = g_Var.Color.ChamsVisible[ 1 ], bvis = g_Var.Color.ChamsVisible[ 2 ];
			else
				rvis = g_Var.Color.TeamChamsVisible[ 0 ], gvis = g_Var.Color.TeamChamsVisible[ 1 ], bvis = g_Var.Color.TeamChamsVisible[ 2 ];

			g_pRenderView->SetColorRGB( rvis, gvis, bvis );

			pPlayerMat->IncrementReferenceCount();
			pPlayerMat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );

			pEnemy->DrawModel();

			// Backtrack chams
			if (g_Var.Visuals.Chams.BacktrackMaterial > 0)
			{
				m_bDrawingBacktrackChams = true;
				pEnemy->DrawModel();
				g_pEngineModel->ForcedMaterialOverride(pPlayerMat); // Reset material for chams
				m_bDrawingBacktrackChams = false;
			}
		}
	}

	if( g_Var.Visuals.Local.Enable )
	{
		if (g_Var.Visuals.Local.Chams > 0)
		{
			if (g_Var.Visuals.Local.Chams == 1)
				pLocalMat = g_pMatsystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
			else
				if (g_Var.Visuals.Local.Chams == 2)
					pLocalMat = g_pMatsystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
				else
					if (g_Var.Visuals.Local.Chams == 3)
						pLocalMat = g_pMatsystem->FindMaterial("effects/bubble", TEXTURE_GROUP_MODEL);
					else
						if (g_Var.Visuals.Local.Chams == 4)
							pLocalMat = g_pMatsystem->FindMaterial("models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER);

			if (pLocalMat == nullptr || pLocalMat->IsErrorMaterial())
				return;

			if (g_Var.Visuals.Local.Chams == 3)
			{
				pLocalMat->IncrementReferenceCount();
				pLocalMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
				pLocalMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
			}

			g_pEngineModel->ForcedMaterialOverride(pLocalMat);

			g_pRenderView->SetBlend(g_Var.Visuals.Local.Transparency / 100.f);

			if (pLocal->bInScope() && g_Var.Visuals.Local.Transparency / 100.f > 0.6f)
				g_pRenderView->SetBlend(0.6);

			g_pRenderView->SetColorRGB(g_Var.Color.LocalChams[0], g_Var.Color.LocalChams[1], g_Var.Color.LocalChams[2]);

			pLocalMat->IncrementReferenceCount();
			pLocalMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

			pLocal->DrawModel();

		}

		// Fake chams
		if (g_Var.Visuals.Local.FakeChams > 0)
		{
			m_bDrawingFakeChams = true;
			pLocal->DrawModel();
			g_pEngineModel->ForcedMaterialOverride(pLocalMat); // Reset material for chams
			m_bDrawingFakeChams = false;
		}
	}

	g_pEngineModel->ForcedMaterialOverride( 0 );
}