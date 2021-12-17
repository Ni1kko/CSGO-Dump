#include "drawmodel.h"
#include "Detour.h"

DrawModelExecuteFn oDrawModel;
ListLeavesInBoxFn oListLeavesInBox;

bool m_bDrawingBacktrackChams = false;
bool m_bDrawingFakeChams = false;

void DrawPlayerChams(void*thisptr, int edx, void*context, void*state, const ModelRenderInfo_t &info, matrix3x4_t *customBoneToWorld, CBaseEntity* pLocal)
{
	CBaseEntity* pEnemy = g_pEntList->GetClientEntity(info.entity_index);

	if (!pEnemy)
		return;

	if (g_Var.Visuals.Local.Enable)
	{
		if (pLocal->GetHealth() > 0 && pLocal == pEnemy)
		{
			g_pRenderView->SetBlend(g_Var.Visuals.Local.Transparency / 100.f);

			if (pLocal->bInScope() && g_Var.Visuals.Local.Transparency / 100.f > 0.2f)
				g_pRenderView->SetBlend(0.2f);

			int iLocalChams = g_Var.Visuals.Local.Chams;

			if (iLocalChams > 0)
			{
				IMaterial* pLocalMat = nullptr;

				switch (iLocalChams)
				{
				case 1:
					pLocalMat = g_pMatsystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
					break;
				case 2:
					pLocalMat = g_pMatsystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
					break;
				case 3:
					pLocalMat = g_pMatsystem->FindMaterial("effects/bubble", TEXTURE_GROUP_MODEL);
					break;
				case 4:
					pLocalMat = g_pMatsystem->FindMaterial("models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER);
					break;
				}

				if (!pLocalMat || pLocalMat->IsErrorMaterial())
					return;

				if (iLocalChams == 3)
				{
					pLocalMat->IncrementReferenceCount();
					pLocalMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
					pLocalMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
				}

				g_pEngineModel->ForcedMaterialOverride(pLocalMat);

				g_pRenderView->SetColorRGB(g_Var.Color.LocalChams[0], g_Var.Color.LocalChams[1], g_Var.Color.LocalChams[2]);
				pLocalMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
			}
		}
	}

	if (g_Var.Visuals.Chams.Enable)
	{
		if (pEnemy->GetHealth() > 0 && pLocal != pEnemy)
		{
			std::shared_ptr<sHistoryRecords> pRecord = g_History.GetCurrentRecord(pEnemy);
			std::shared_ptr<sHistoryRecords> pLastRecord = g_History.GetLastRecord(pEnemy);

			if (pRecord && pLastRecord && pLastRecord->m_bIsChamsCompatible && pLocal->GetHealth() > 0)
			{
				auto Distance = (pLastRecord->m_vecAbsOrigin - pRecord->m_vecAbsOrigin).Length2D();
				auto AlphaFactor = fmaxf(0.f, fminf(1.f, Distance / 30.f));

				auto PrevBlend = g_pRenderView->GetBlend();
				float PrevRGB[3];

				g_pRenderView->GetColorRGB(PrevRGB);
				g_pRenderView->SetBlend(clamp(g_Var.Visuals.Chams.BacktrackAlpha / 100.f* AlphaFactor, 0, 0.99999f));
				g_pRenderView->SetColorRGB(g_Var.Color.BacktrackChams[0], g_Var.Color.BacktrackChams[1], g_Var.Color.BacktrackChams[2]);

				IMaterial* pBTMat = nullptr;

				int iBTChams = g_Var.Visuals.Chams.BacktrackMaterial;

				switch (iBTChams)
				{
				case 1:
					pBTMat = g_pMatsystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
					break;
				case 2:
					pBTMat = g_pMatsystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
					break;
				case 3:
					pBTMat = g_pMatsystem->FindMaterial("effects/bubble", TEXTURE_GROUP_MODEL);
					break;
				case 4:
					pBTMat = g_pMatsystem->FindMaterial("models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER);
					break;
				}

				if (!pBTMat || pBTMat->IsErrorMaterial())
					return;

				if (iBTChams == 3)
				{
					pBTMat->IncrementReferenceCount();
					pBTMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
					pBTMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
				}

				auto BackupIgnoreZ = pBTMat->GetMaterialVarFlag(MATERIAL_VAR_IGNOREZ);
				pBTMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				g_pEngineModel->ForcedMaterialOverride(pBTMat);

				oDrawModel(thisptr, context, state, info, pLastRecord->m_Matrix);

				pBTMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, BackupIgnoreZ);
				g_pRenderView->SetBlend(PrevBlend);
				g_pRenderView->SetColorRGB(PrevRGB[0], PrevRGB[1], PrevRGB[2]);
			}

			int iPlayerChams = g_Var.Visuals.Chams.Players;

			if (iPlayerChams > 0)
			{
				if (g_Misc.bInSameTeam(pLocal, pEnemy) && !g_Var.Visuals.Chams.Team)
					return;

				IMaterial* pPlayerMat = nullptr;

				switch (iPlayerChams)
				{
				case 1:
					pPlayerMat = g_pMatsystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
					break;
				case 2:
					pPlayerMat = g_pMatsystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
					break;
				case 3:
					pPlayerMat = g_pMatsystem->FindMaterial("effects/bubble", TEXTURE_GROUP_MODEL);
					break;
				case 4:
					pPlayerMat = g_pMatsystem->FindMaterial("models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER);
					break;
				}

				if (!pPlayerMat || pPlayerMat->IsErrorMaterial())
					return;

				if (iPlayerChams == 3)
				{
					pPlayerMat->IncrementReferenceCount();
					pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
					pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
				}

				float r, g, b;

				if (!g_Misc.bInSameTeam(pLocal, pEnemy))
					r = g_Var.Color.Chams[0], g = g_Var.Color.Chams[1], b = g_Var.Color.Chams[2];
				else
					r = g_Var.Color.TeamChams[0], g = g_Var.Color.TeamChams[1], b = g_Var.Color.TeamChams[2];

				if (g_Var.Visuals.Chams.DrawOriginal)
					oDrawModel(thisptr, context, state, info, customBoneToWorld);

				if (!g_Misc.bInSameTeam(pLocal, pEnemy))
					g_pRenderView->SetBlend(clamp(g_Var.Visuals.Chams.Transparency / 100.f, 0, 0.99999f));
				else
					g_pRenderView->SetBlend(clamp(g_Var.Visuals.Chams.TeamTransparency / 100.f, 0, 0.99999f));

				g_pRenderView->SetColorRGB(r, g, b);

				pPlayerMat->IncrementReferenceCount();
				pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

				oDrawModel(thisptr, context, state, info, customBoneToWorld);

				float rvis, gvis, bvis;

				if (!g_Misc.bInSameTeam(pLocal, pEnemy))
					rvis = g_Var.Color.ChamsVisible[0], gvis = g_Var.Color.ChamsVisible[1], bvis = g_Var.Color.ChamsVisible[2];
				else
					rvis = g_Var.Color.TeamChamsVisible[0], gvis = g_Var.Color.TeamChamsVisible[1], bvis = g_Var.Color.TeamChamsVisible[2];

				g_pRenderView->SetColorRGB(rvis, gvis, bvis);
				pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				if (g_Var.Visuals.Chams.BacktrackMaterial > 0)
				{
					m_bDrawingBacktrackChams = true;
					oDrawModel(thisptr, context, state, info, customBoneToWorld);
					g_pEngineModel->ForcedMaterialOverride(pPlayerMat); // Reset material for chams
					m_bDrawingBacktrackChams = false;
				}
			}
		}
	}
}

void __fastcall HOOKED_DrawModel( void*thisptr, int edx, void*context, void*state, const ModelRenderInfo_t &info, matrix3x4_t *customBoneToWorld )
{
	std::string szModel = g_pModel->GetModelName( info.pModel );
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );
	static IMaterial* pWeaponMat;
	static IMaterial* pViewMat;
	static IMaterial* pHandMat;

	//fix for shadows
	if( ( info.flags & 0x40000000 ) != 0 )
	{
		oDrawModel( thisptr, context, state, info, customBoneToWorld );
		return;
	}

	if( pLocal == nullptr || !g_pEngine->IsInGame() )
	{
		oDrawModel( thisptr, context, state, info, customBoneToWorld );
		pWeaponMat = nullptr;
		pHandMat = nullptr;
		pViewMat = nullptr;
		return;
	}

	if( !(g_Var.Visuals.Chams.Enable || g_Var.Visuals.Local.Enable))
	{
		oDrawModel( thisptr, context, state, info, customBoneToWorld );
		return;
	}

	if( szModel.find( "models/player" ) != std::string::npos )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( info.entity_index );

		if (pEnemy)
		{
			auto BacktrackChamsMode = g_Var.Visuals.Chams.BacktrackMaterial;
			if (m_bDrawingBacktrackChams && !g_Misc.bInSameTeam(pLocal, pEnemy) && pEnemy->IsAlive() && pLocal->IsAlive())
			{
				auto CurrentRecord = g_History.GetCurrentRecord(pEnemy);
				auto LastRecord = g_History.GetLastRecord(pEnemy);
				if (CurrentRecord && LastRecord && LastRecord->m_bIsChamsCompatible)
				{
					auto Distance = (LastRecord->m_vecAbsOrigin - CurrentRecord->m_vecAbsOrigin).Length2D();
					auto AlphaFactor = fmaxf(0.f, fminf(1.f, Distance / 30.f));

					auto PrevBlend = g_pRenderView->GetBlend();
					float PrevRGB[3];
					g_pRenderView->GetColorRGB(PrevRGB);

					g_pRenderView->SetBlend(g_Var.Visuals.Chams.BacktrackAlpha / 100.f * AlphaFactor);
					g_pRenderView->SetColorRGB(g_Var.Color.BacktrackChams[0], g_Var.Color.BacktrackChams[1], g_Var.Color.BacktrackChams[2]);

					IMaterial* pPlayerMat = nullptr;
					switch (BacktrackChamsMode)
					{
					case 1:
						pPlayerMat = g_pMatsystem->FindMaterial("debug/debugambientcube", TEXTURE_GROUP_MODEL);
						break;
					case 2:
						pPlayerMat = g_pMatsystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
						break;
					case 3:
						pPlayerMat = g_pMatsystem->FindMaterial("effects/bubble", TEXTURE_GROUP_MODEL);
						break;
					case 4:
						pPlayerMat = g_pMatsystem->FindMaterial("models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER);
						break;
					}

					if (!pPlayerMat || pPlayerMat->IsErrorMaterial())
						return;

					if (BacktrackChamsMode == 3)
					{
						pPlayerMat->IncrementReferenceCount();
						pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
						pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
					}

					auto BackupIgnoreZ = pPlayerMat->GetMaterialVarFlag(MATERIAL_VAR_IGNOREZ);
					pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
					g_pEngineModel->ForcedMaterialOverride(pPlayerMat);

					oDrawModel(thisptr, context, state, info, LastRecord->m_Matrix);

					pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, BackupIgnoreZ);
					g_pRenderView->SetBlend(PrevBlend);
					g_pRenderView->SetColorRGB(PrevRGB[0], PrevRGB[1], PrevRGB[2]);
				}

				return;
			}

			if (pLocal->GetHealth() > 0)
			{
				if (pEnemy && pLocal == pEnemy)
				{
					if (g_Var.Visuals.Local.Enable)
					{
						g_pRenderView->SetBlend(g_Var.Visuals.Local.Transparency / 100.f);

						if (pLocal->bInScope() && g_Var.Visuals.Local.Transparency / 100.f > 0.2f)
							g_pRenderView->SetBlend(0.2f);

						Vector BonePos;
						Vector OutPos;
						matrix3x4_t BoneMatrix[128];

						for (int i = 0; i < 128; i++)
						{
							g_Math.AngleMatrix(Vector(0, pLocal->GetEyeAngles().y - UserCmd::vAntiAimReal.y, 0), BoneMatrix[i]);
							g_Math.MatrixMultiply(BoneMatrix[i], customBoneToWorld[i]);
							BonePos = Vector(customBoneToWorld[i][0][3], customBoneToWorld[i][1][3], customBoneToWorld[i][2][3]) - info.origin;
							g_Math.VectorRotate(BonePos, Vector(0, pLocal->GetEyeAngles().y - UserCmd::vAntiAimReal.y, 0), OutPos);
							OutPos += info.origin;
							BoneMatrix[i][0][3] = OutPos.x;
							BoneMatrix[i][1][3] = OutPos.y;
							BoneMatrix[i][2][3] = OutPos.z;
						}

						if (m_bDrawingFakeChams && customBoneToWorld != nullptr)
						{
							float PrevBlend = g_pRenderView->GetBlend();
							float PrevRGB[3];
							g_pRenderView->GetColorRGB(PrevRGB);

							g_pRenderView->SetBlend(clamp((g_Var.Visuals.Local.Transparency / 100.f) / 2, 0.1, 1));
							g_pRenderView->SetColorRGB(g_Var.Color.LocalFakeChams[0], g_Var.Color.LocalFakeChams[1], g_Var.Color.LocalFakeChams[2]);

							IMaterial* pPlayerMat = nullptr;

							pPlayerMat = g_pMatsystem->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_OTHER);

							if (pPlayerMat && !pPlayerMat->IsErrorMaterial())
							{
								Vector BonePos;
								Vector OutPos;
								matrix3x4_t BoneMatrix[128];

								for (int i = 0; i < 128; i++)
								{
									g_Math.AngleMatrix(Vector(0, pLocal->GetEyeAngles().y - UserCmd::vAntiAimReal.y, 0), BoneMatrix[i]);
									g_Math.MatrixMultiply(BoneMatrix[i], customBoneToWorld[i]);
									BonePos = Vector(customBoneToWorld[i][0][3], customBoneToWorld[i][1][3], customBoneToWorld[i][2][3]) - info.origin;
									g_Math.VectorRotate(BonePos, Vector(0, pLocal->GetEyeAngles().y - UserCmd::vAntiAimReal.y, 0), OutPos);
									OutPos += info.origin;
									BoneMatrix[i][0][3] = OutPos.x;
									BoneMatrix[i][1][3] = OutPos.y;
									BoneMatrix[i][2][3] = OutPos.z;
								}

								auto BackupIgnoreZ = pPlayerMat->GetMaterialVarFlag(MATERIAL_VAR_IGNOREZ);
								pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								g_pEngineModel->ForcedMaterialOverride(pPlayerMat);

								oDrawModel(thisptr, context, state, info, BoneMatrix);

								pPlayerMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, BackupIgnoreZ);
								g_pRenderView->SetBlend(PrevBlend);
								g_pRenderView->SetColorRGB(PrevRGB[0], PrevRGB[1], PrevRGB[2]);
							}
						}
					}
				}
				else
				{
					if (g_Var.Visuals.Chams.Enable && g_Misc.bInSameTeam(pLocal, pEnemy))
					{
						g_pRenderView->SetBlend(g_Var.Visuals.Chams.TeamTransparency / 100.f);

						if (g_Var.Visuals.Chams.Team)
						{
							oDrawModel(thisptr, context, state, info, customBoneToWorld);
							return;
						}
					}
					else
					{
						if (g_Var.Visuals.Chams.Enable && !g_Misc.bInSameTeam(pLocal, pEnemy))
							g_pRenderView->SetBlend(g_Var.Visuals.Chams.Transparency / 100.f);

						oDrawModel(thisptr, context, state, info, customBoneToWorld);
						return;
					}
				}
			}
			else
			{
				oDrawModel(thisptr, context, state, info, customBoneToWorld);
				return;
			}
		}
	}

	if( szModel.find( "models/weapons/w_" ) != std::string::npos && g_Var.Visuals.Chams.Weapons > 0 )
	{
		if( g_Var.Visuals.Chams.Weapons == 1 )
			pWeaponMat = g_pMatsystem->FindMaterial( "models/props_interiors/desk_metal", TEXTURE_GROUP_MODEL );
		else
			if( g_Var.Visuals.Chams.Weapons == 2 )
				pWeaponMat = g_pMatsystem->FindMaterial( "models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_OTHER );

		if( pWeaponMat == nullptr || pWeaponMat->IsErrorMaterial() )
		{
			oDrawModel( thisptr, context, state, info, customBoneToWorld );
			return;
		}

		pWeaponMat->ColorModulate( float( g_Var.Color.WeaponChams[ 0 ] / 255.f ), float( g_Var.Color.WeaponChams[ 1 ] / 255.f ), float( g_Var.Color.WeaponChams[ 2 ] / 255.f ) );

		pWeaponMat->IncrementReferenceCount();
		pWeaponMat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );

		g_pEngineModel->ForcedMaterialOverride( pWeaponMat );

		oDrawModel( thisptr, context, state, info, customBoneToWorld );

		pWeaponMat->ColorModulate( float( g_Var.Color.WeaponChamsVisible[ 0 ] / 255.f ), float( g_Var.Color.WeaponChamsVisible[ 1 ] / 255.f ), float( g_Var.Color.WeaponChamsVisible[ 2 ] / 255.f ) );

		pWeaponMat->IncrementReferenceCount();
		pWeaponMat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		g_pEngineModel->ForcedMaterialOverride( pWeaponMat );
	}

	if( ( ( szModel.find( "v_" ) != std::string::npos || szModel.find( "uid" ) != std::string::npos || szModel.find( "stattrack" ) != std::string::npos ) && szModel.find( "arms" ) == std::string::npos ) && g_Var.Visuals.Chams.View > 0 )
	{
		if( g_Var.Visuals.Chams.View == 1 )
			pViewMat = g_pMatsystem->FindMaterial( "models/weapons/v_models/knife_flip/ghost/knife_flip_ghost", TEXTURE_GROUP_MODEL );
		else
			if( g_Var.Visuals.Chams.View == 2 )
				pViewMat = g_pMatsystem->FindMaterial( "debug/debugwireframe", TEXTURE_GROUP_OTHER );
			else
				if( g_Var.Visuals.Chams.View == 3 )
					pViewMat = g_pMatsystem->FindMaterial( "models/effects/ghosts/ghost2", TEXTURE_GROUP_OTHER );

		if( pViewMat == nullptr || pViewMat->IsErrorMaterial() )
		{
			oDrawModel( thisptr, context, state, info, customBoneToWorld );
			return;
		}

		pViewMat->ColorModulate( float( g_Var.Color.ViewChams[ 0 ] / 255.f ), float( g_Var.Color.ViewChams[ 1 ] / 255.f ), float( g_Var.Color.ViewChams[ 2 ] / 255.f ) );

		pViewMat->IncrementReferenceCount();
		pViewMat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );

		g_pEngineModel->ForcedMaterialOverride( pViewMat );
	}

	if (szModel.find("models/weapons/v_models/arms/") != std::string::npos)
	{
		g_pRenderView->SetBlend(g_Var.Visuals.Chams.HandsTransparency / 100.f);

		if (g_Var.Visuals.Chams.Hands > 0)
		{
			if (g_Var.Visuals.Chams.Hands == 1) //models/props_farm/chicken_bunnyears  models/props_shacks/fishing_net01 dev/dev_prisontvoverlay001
				pHandMat = g_pMatsystem->FindMaterial("models/props_street/bollards", TEXTURE_GROUP_MODEL);
			else
				if (g_Var.Visuals.Chams.Hands == 2)
					pHandMat = g_pMatsystem->FindMaterial("models/effects/ghosts/ghost3", TEXTURE_GROUP_OTHER);
				else
					if (g_Var.Visuals.Chams.Hands == 3)
						pHandMat = g_pMatsystem->FindMaterial("debug/debugwireframe", TEXTURE_GROUP_OTHER);
					else
						if (g_Var.Visuals.Chams.Hands == 4)
							pHandMat = g_pMatsystem->FindMaterial("models/props_interiors/tvanimstatic", TEXTURE_GROUP_OTHER);

			if (pHandMat == nullptr || pHandMat->IsErrorMaterial())
			{
				oDrawModel(thisptr, context, state, info, customBoneToWorld);
				return;
			}

			pHandMat->ColorModulate(float(g_Var.Color.HandChams[0] / 255.f), float(g_Var.Color.HandChams[1] / 255.f), float(g_Var.Color.HandChams[2] / 255.f));

			pHandMat->IncrementReferenceCount();
			pHandMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

			g_pEngineModel->ForcedMaterialOverride(pHandMat);

			oDrawModel(thisptr, context, state, info, customBoneToWorld);

			pHandMat->ColorModulate(float(g_Var.Color.HandChamsVisible[0] / 255.f), float(g_Var.Color.HandChamsVisible[1] / 255.f), float(g_Var.Color.HandChamsVisible[2] / 255.f));

			pHandMat->IncrementReferenceCount();
			pHandMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

			g_pEngineModel->ForcedMaterialOverride(pHandMat);
		}
	}

	/*
	models/props_interiors/tvanimstatic /animated
	models/props_farm/chicken_bunnyears
	*/

	oDrawModel( thisptr, context, state, info, customBoneToWorld );

//	g_pEngineModel->ForcedMaterialOverride( 0 );
}

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

int __fastcall HOOKED_ListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax)
{
	static DWORD dwInsertIntoTreeReturn = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "85 C0 0F 84 ? ? ? ? 8B 0B 8B 01");

	if (!g_Var.Visuals.Chams.Enable || (DWORD)_ReturnAddress() != dwInsertIntoTreeReturn)
		return oListLeavesInBox(bsp, mins, maxs, pList, listMax);

	auto info = *(RenderableInfo_t**)((uintptr_t)_AddressOfReturnAddress() + 0x14);

	if (!info || !info->m_pRenderable)
		return oListLeavesInBox(bsp, mins, maxs, pList, listMax);

	auto base_entity = info->m_pRenderable->player();

	if (base_entity && base_entity->IsPlayer())
	{
		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;
	}

	static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
	static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
	auto count = oListLeavesInBox(bsp, mins, maxs, pList, listMax);
	return count;
}

Detour _d;

void __fastcall HOOKED_AddRenderAble(void* ecx, void* edx, void* renderable, int a1, int a2, int a3, int a4)
{
	using org_fn = void(__thiscall*)(void* , void* , int , int , int , int );

	auto ent = ((CRenderAble*)renderable)->player();

	if (ent && ent->IsPlayer())
	{
		//most likely RENDER_GROUP_TWOPASS, idk...
		a2 = 2;
	}

	return _d.GetOriginal<org_fn>()(ecx, renderable, a1, a2, a3, a4);
}

void HookAddRenderAble()
{
	DWORD PatternResult = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 56 8B 75 08 57 FF 75 18");

	if (!PatternResult)
		throw "Failed to signature scan AddRenderAble!";

	_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(HOOKED_AddRenderAble));
}