#include "visuals.h"

CVisuals g_Visuals;

void CVisuals::InitMaterials()
{
	if( g_pMatsystem )
	{
		BubbleMat = g_pMatsystem->FindMaterial( "debug/debugambientcube", TEXTURE_GROUP_MODEL );
		FlatMat = g_pMatsystem->FindMaterial( "debug/debugdrawflat", TEXTURE_GROUP_MODEL );
		Ghost2Mat = g_pMatsystem->FindMaterial( "effects/bubble", TEXTURE_GROUP_MODEL );
		TvMat = g_pMatsystem->FindMaterial( "models/props_interiors/tvebstest", TEXTURE_GROUP_OTHER );
		CryptsMat = g_pMatsystem->FindMaterial( "models/props_cemetery/crypts_wall", TEXTURE_GROUP_OTHER );
		PulseMat = g_pMatsystem->FindMaterial( "models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_OTHER );
		BanannaMat = g_pMatsystem->FindMaterial( "models/cs_italy/bananna", TEXTURE_GROUP_OTHER );
		Ghost3Mat = g_pMatsystem->FindMaterial( "models/effects/ghosts/ghost3", TEXTURE_GROUP_OTHER );
		NetMat = g_pMatsystem->FindMaterial( "models/props_shacks/fishing_net01", TEXTURE_GROUP_OTHER );
		AnimMat = g_pMatsystem->FindMaterial( "models/props_interiors/tvanimstatic", TEXTURE_GROUP_OTHER );
	}
}

void WeaponRange(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	if (!pWeapon)
		return;

	if( ((pWeapon->GetWeaponID() == WEAPON_TASER && g_Var.Misc.ZeusBot[1]) || (pWeapon->IsKnife() && g_Var.Misc.KnifeBot[1])) && *(bool*)((DWORD)g_pInput + 0xAD))
	{
		float step = M_PI * 2.0 / 125;
		float rad = pWeapon->IsKnife() ? 48.f : 150.f;
		Vector origin = pLocal->GetEyePosition();

		static double rainbow;

		Vector screenPos;
		static Vector prevScreenPos;

		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

			CEngineTraceClient::Ray_t ray;
			CEngineTraceClient::trace_t trace;
			CEngineTraceClient::CTraceFilter filter;

			filter.pSkip = pLocal;
			ray.Init(origin, pos);

			g_pEngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace);

			if (g_pDebugOverlay->ScreenPosition(trace.endpos, screenPos))
				continue;

			if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f && prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f)
			{
				g_Draw.LineRGBA(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, 255, 255, 255, 80);
			}
			prevScreenPos = screenPos;
		}
	}
}

void CVisuals::Run( CBaseEntity* pLocal )
{
	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize( iScreenWidth, iScreenHeight );

	if( g_Var.Visuals.World.Grenade[ 0 ] || g_Var.Visuals.World.Grenade[ 1 ] || g_Var.Misc.Indicator[ 3 ] )
		EntityVisuals( pLocal, iScreenWidth, iScreenHeight );

	if( g_Var.Visuals.Local.Enable )
		LocalVisuals( pLocal );

	if( g_Var.Visuals.Player.Enable )
		PlayerVisuals( pLocal, iScreenWidth, iScreenHeight );

	GlowMain( pLocal );

	if( g_Var.Visuals.World.Removals[ 2 ] )
	{
		if( pLocal->bInScope() && pLocal->GetHealth() > 0 )
		{
			g_Draw.LineRGBA( iScreenWidth / 2, 0, iScreenWidth / 2, iScreenHeight, 0, 0, 0, 255 );
			g_Draw.LineRGBA( 0, iScreenHeight / 2, iScreenWidth, iScreenHeight / 2, 0, 0, 0, 255 );
		}
	}

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	Crosshair( pLocal, pWeapon, iScreenWidth, iScreenHeight );

	if( pLocal->GetHealth() > 0 )
	{
		WeaponRange(pLocal, pWeapon);

		InitIndicators( pLocal, iScreenWidth, iScreenHeight );

		DrawAimbot( pLocal, pWeapon );
	}
}

void CVisuals::LocalVisuals( CBaseEntity* pLocal )
{
	if( pLocal->GetHealth() > 0 )
	{
		CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

		if( pWeapon == nullptr )
			return;

		VEngineClient::player_info_t pInfo;

		if( !g_pEngine->GetPlayerInfo( pLocal->GetIndex(), &pInfo ) )
			return;

		const matrix3x4_t& trans = *( matrix3x4_t* ) ( ( DWORD ) pLocal + Offsets::Entity::m_rgflCoordinateFrame );

		Vector min = *( Vector* ) ( ( DWORD ) pLocal + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins );
		Vector max = *( Vector* ) ( ( DWORD ) pLocal + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs );

		Vector points[] = { Vector( min.x, min.y, min.z ), Vector( min.x, max.y, min.z ), Vector( max.x, max.y, min.z ), Vector( max.x, min.y, min.z ), Vector( max.x, max.y, max.z ),
			Vector( min.x, max.y, max.z ), Vector( min.x, min.y, max.z ), Vector( max.x, min.y, max.z ) };

		Vector pointsTransformed[ 8 ];
		for( int i = 0; i < 8; i++ )
			g_Math.VectorTransform( points[ i ], trans, pointsTransformed[ i ] );

		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;

		if( !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 3 ], flb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 5 ], brt ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 0 ], blb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 4 ], frt ) != 1
			|| !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 2 ], frb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 1 ], brb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 6 ], blt ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 7 ], flt ) != 1 )
			return;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bot = flb.y;

		for( int i = 1; i < 8; i++ )
		{
			if( left > arr[ i ].x )
				left = arr[ i ].x;

			if( bot < arr[ i ].y )
				bot = arr[ i ].y;

			if( right < arr[ i ].x )
				right = arr[ i ].x;

			if( top > arr[ i ].y )
				top = arr[ i ].y;
		}

		float flHeight = ( bot - top );
		float flWidth = ( right - left );
		float flMiddle = left + flWidth / 2;

		if( g_Var.Visuals.Local.Draw[ 2 ] )
		{
			g_Draw.LineRGBA( left, top, right, top, 255, 255, 255, clamp( 255, 0, 200 ) ); //TOP
			g_Draw.LineRGBA( left + 1, top + 1, right - 1, top + 1, 0, 0, 0, clamp( 255, 0, 150 ) );
			g_Draw.LineRGBA( left - 1, top - 1, right + 1, top - 1, 0, 0, 0, clamp( 255, 0, 150 ) );

			g_Draw.LineRGBA( right, bot, left, bot, 255, 255, 255, clamp( 255, 0, 200 ) ); //BOT
			g_Draw.LineRGBA( right - 1, bot - 1, left + 1, bot - 1, 0, 0, 0, clamp( 255, 0, 150 ) );
			g_Draw.LineRGBA( right + 1, bot + 1, left - 1, bot + 1, 0, 0, 0, clamp( 255, 0, 150 ) );

			g_Draw.LineRGBA( left, bot, left, top, 255, 255, 255, clamp( 255, 0, 200 ) ); //LEFT
			g_Draw.LineRGBA( left - 1, bot + 1, left - 1, top - 1, 0, 0, 0, clamp( 255, 0, 150 ) );
			g_Draw.LineRGBA( left + 1, bot - 1, left + 1, top + 1, 0, 0, 0, clamp( 255, 0, 150 ) );

			g_Draw.LineRGBA( right, top, right, bot, 255, 255, 255, clamp( 255, 0, 200 ) ); //RIGHT
			g_Draw.LineRGBA( right + 1, top - 1, right + 1, bot + 1, 0, 0, 0, clamp( 255, 0, 150 ) );
			g_Draw.LineRGBA( right - 1, top + 1, right - 1, bot - 1, 0, 0, 0, clamp( 255, 0, 150 ) );
		}

		if( g_Var.Visuals.Local.Draw[ 0 ] )
			g_Draw.DrawString( g_Draw.espfont, true, flMiddle, top - 12, g_Var.Color.Name[ 0 ], g_Var.Color.Name[ 1 ], g_Var.Color.Name[ 2 ], 255, pInfo.name );

		if( g_Var.Visuals.Local.Draw[ 1 ] )
			g_Draw.DrawHealthBar( left - 5, top, 3, flHeight + 1, g_Var.Color.Health[ 0 ], g_Var.Color.Health[ 1 ], g_Var.Color.Health[ 2 ], 255, pLocal->GetHealth() );

		if( g_Var.Visuals.Local.Draw[ 4 ] && pWeapon->GetPrimaryClip() > 0 )
		{
			g_Draw.DrawArmorBar( left + 1, bot + 3, flWidth, 3, g_Var.Color.AmmoBar[ 0 ], g_Var.Color.AmmoBar[ 1 ], g_Var.Color.AmmoBar[ 2 ], 255, pWeapon->GetClip(), pWeapon->GetPrimaryClip() );

			if( g_Var.Visuals.Local.Draw[ 3 ] )
				g_Draw.DrawString( g_Draw.iconfont, true, flMiddle, bot + 8, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], 255, pWeapon->GetCustomIcon() );
		}
		else
			if( g_Var.Visuals.Local.Draw[ 3 ] )
				g_Draw.DrawString( g_Draw.iconfont, true, flMiddle, bot + 5, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], 255, pWeapon->GetCustomIcon() );
	}
}

void CVisuals::GlowMain( CBaseEntity* pLocal )
{
	static DWORD dwGlowObjectManager = *( DWORD* ) ( g_Offset.GlowObjectManager );

	if( !dwGlowObjectManager )
		return;

	CGlowObjectManager* GlowObjectManager = ( CGlowObjectManager* ) dwGlowObjectManager;

	if( !( g_Var.Visuals.Glow.Player && g_Var.Visuals.Player.Enable || g_Var.Visuals.Local.Enable && g_Var.Visuals.Local.Glow ) )
		return;

	for( int i = 0; i < GlowObjectManager->size; ++i )
	{
		CGlowObjectManager::GlowObjectDefinition_t* pGlowObject = &GlowObjectManager->m_GlowObjectDefinitions[ i ];

		CBaseEntity* pEntity = pGlowObject->getEntity();

		if( pGlowObject->IsEmpty() || pEntity == nullptr || pEntity->IsDormant() )
			continue;

		int iClassID = pEntity->GetClientClass()->GetClassID();

		if( iClassID == CCSPlayer )
		{
			if( pLocal == pEntity )
			{
				if( g_Var.Visuals.Local.Glow > 0 )
					pGlowObject->set( Color( g_Var.Color.LocalGlow[ 0 ], g_Var.Color.LocalGlow[ 1 ], g_Var.Color.LocalGlow[ 2 ], 250 ), 0.55, g_Var.Visuals.Local.Glow == 2 ? true : false );

				continue;
			}

			if(g_Misc.bInSameTeam(pLocal, pEntity) && !g_Var.Visuals.Player.Team )
				continue;

			if( g_Var.Visuals.Glow.Player > 0 )
			{
				int r, g, b;
				bool bisEnemyVis = g_Misc.IsVisible( pLocal, pEntity );

				if( !g_Misc.bInSameTeam(pLocal, pEntity))
					bisEnemyVis ? r = g_Var.Color.GlowVisible[ 0 ] : r = g_Var.Color.Glow[ 0 ], bisEnemyVis ? g = g_Var.Color.GlowVisible[ 1 ] : g = g_Var.Color.Glow[ 1 ], bisEnemyVis ? b = g_Var.Color.GlowVisible[ 2 ] : b = g_Var.Color.Glow[ 2 ];
				else
					bisEnemyVis ? r = g_Var.Color.TeamVisible[ 0 ] : r = g_Var.Color.Team[ 0 ], bisEnemyVis ? g = g_Var.Color.TeamVisible[ 1 ] : g = g_Var.Color.Team[ 1 ], bisEnemyVis ? b = g_Var.Color.TeamVisible[ 2 ] : b = g_Var.Color.Team[ 2 ];

				if( pEntity->IsDefusing() )
					pGlowObject->set( Color( 66, 244, 229, 250 ), 0.55, g_Var.Visuals.Glow.Player == 2 ? true : false );
				else
					pGlowObject->set( Color( r, g, b, 250 ), 0.55, g_Var.Visuals.Glow.Player == 2 ? true : false );
			}
		}

/*		if( g_Var.Visuals.Glow.Weapons )
		{
			if( iClassID == CDEagle || iClassID == CWeaponHKP2000 || iClassID == CWeaponGlock || iClassID == CWeaponP250 || iClassID == CWeaponP228 || iClassID == CWeaponTec9 || iClassID == CWeaponUSP || iClassID == CWeaponElite || iClassID == CWeaponFiveSeven
				|| iClassID == CWeaponXM1014 || iClassID == CWeaponMag7 || iClassID == CWeaponSawedoff || iClassID == CWeaponNOVA
				|| iClassID == CAK47 || iClassID == CWeaponAug || iClassID == CWeaponFamas || iClassID == CWeaponGalil || iClassID == CWeaponGalilAR || iClassID == CWeaponM249 || iClassID == CWeaponM4A1
				|| iClassID == CWeaponMAC10 || iClassID == CWeaponP90 || iClassID == CWeaponUMP45 || iClassID == CWeaponBizon || iClassID == CWeaponNegev || iClassID == CWeaponMP7 || iClassID == CWeaponMP9 || iClassID == CWeaponSG552
				|| iClassID == CWeaponSG556 || iClassID == CWeaponSG550 || iClassID == CWeaponAWP || iClassID == CWeaponSSG08 || iClassID == CWeaponG3SG1 || iClassID == CWeaponSCAR20 || iClassID == CWeaponTaser
				|| iClassID == CHEGrenade || iClassID == CDecoyGrenade || iClassID == CSmokeGrenade || iClassID == CFlashbang || iClassID == CIncendiaryGrenade || iClassID == CMolotovGrenade
				|| iClassID == CDecoyProjectile || iClassID == CSmokeGrenadeProjectile || iClassID == CSmokeStack || iClassID == ParticleSmokeGrenade || iClassID == CMolotovProjectile 
				|| iClassID == CBaseCSGrenadeProjectile || iClassID == CBaseCSGrenade || iClassID == CC4 || iClassID == CPlantedC4 )
			{
				pGlowObject->set( Color( g_Var.Color.GlowWeapons[ 0 ], g_Var.Color.GlowWeapons[ 1 ], g_Var.Color.GlowWeapons[ 2 ], 255 ), 0.8 );
			}
		}*/
	}
}

void DrawCustomSkeleton(CBaseEntity* entity, const matrix3x4_t* bone_matrix, int a, bool prior = false)
{
	auto studio_model = g_pModel->GetStudiomodel(entity->GetModel());

	if (!studio_model)
		return;

	for (int i = 0; i < studio_model->numbones; i++)
	{
		auto bone = studio_model->GetBone(i);

		if (!bone || bone->parent < 0 || !(bone->flags & 0x00000100))
			continue;

		matrix3x4_t bone_matrix_1, bone_matrix_2;
		if (bone_matrix)
		{
			bone_matrix_1 = bone_matrix[i];
			bone_matrix_2 = bone_matrix[bone->parent];
		}
		else
		{
			bone_matrix_1 = entity->GetBoneMatrix(i);
			bone_matrix_2 = entity->GetBoneMatrix(bone->parent);
		}

		Vector bone_position_1 = Vector(bone_matrix_1[0][3], bone_matrix_1[1][3], bone_matrix_1[2][3]),
			bone_position_2 = Vector(bone_matrix_2[0][3], bone_matrix_2[1][3], bone_matrix_2[2][3]);

		Vector screen1, screen2;
		if (g_pDebugOverlay->ScreenPosition(bone_position_1, screen1) != 1 && g_pDebugOverlay->ScreenPosition(bone_position_2, screen2) != 1)
			g_Draw.LineRGBA(screen1.x, screen1.y, screen2.x, screen2.y, prior ? 0 : 255, prior ? 255 : 255, prior ? 0 : 255, a);
	}
}

void BackTrackVisuals(CBaseEntity* local, CBaseEntity* entity, int a )
{
	if (!local || !local->IsAlive())
		return;

	auto weapon = local->GetWeapon();
	if (!weapon)
		return;

	auto LastRecord = g_History.GetLastRecord(entity);
	if (!LastRecord)
		return;

	DrawCustomSkeleton( entity, LastRecord->m_Matrix, a, false );

	/*auto ScanRecordsList = g_History.GetRecordsToScan(entity, 4);
	if (ScanRecordsList.empty())
		return;

	for (auto& sRecord : ScanRecordsList)
		DrawCustomSkeleton(entity, sRecord->m_Matrix, a, false);*/
}

void DrawBoneEsp( CBaseEntity* pEnemy, int a )
{
	matrix3x4_t m_Matrix[ 128 ];

	for( int i = 0; i < 128; i++ )
		m_Matrix[ i ] = pEnemy->GetBoneMatrix( i );

	DrawCustomSkeleton( pEnemy, m_Matrix, a );
}

void CVisuals::PlayerVisuals( CBaseEntity* pLocal, int iScreenWidth, int iScreenHeight )
{
	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy->PlayerCheckWithoutDormant() )
			continue;

		if(g_Misc.bInSameTeam(pLocal, pEnemy) && !g_Var.Visuals.Player.Team )
			continue;

		if( pEnemy == pLocal )
			continue;

		if( g_Var.Visuals.Player.Radar )
		{
			bool* bSpotted = ( bool* ) ( ( DWORD ) pEnemy + Offsets::Player::m_bSpotted );

			if( *bSpotted == false )
				*bSpotted = true;
		}
	
		static std::vector <int> alphas = std::vector<int>( 100000, 0 );
		int& a = alphas[ i ];

		if( pEnemy->IsDormant() )
		{
			if( a > 0 )
				a -= 1;
		}
		else
			if( a < 255 )
				a += 1;

		if( a > 255 )
			a = 255;
		else
			if( a < 0 )
				a = 0;

		if( g_Var.Visuals.Player.Bone == 1 )
			DrawBoneEsp( pEnemy, a );
		else
			if( g_Var.Visuals.Player.Bone == 2 )
				BackTrackVisuals( pLocal, pEnemy, a );

		if( g_Var.Visuals.Player.Surrounding > 10 && pLocal->GetHealth() > 0 )
		{
			Vector vAngles, vAngleVec, vCalc;
			g_pEngine->GetViewAngles( vAngles );
			g_Math.CalcAngle( Vector( pLocal->GetAbsOrigin().x, pLocal->GetAbsOrigin().y, 0 ), Vector( pEnemy->GetAbsOrigin().x, pEnemy->GetAbsOrigin().y, 0 ), vCalc );
			g_Draw.DirectionTriangle( Vector2D( iScreenWidth / 2, iScreenHeight / 2 ), vCalc.y - vAngles.y, g_Var.Visuals.Player.Surrounding, 15, g_Var.Color.Surrounding[ 0 ], g_Var.Color.Surrounding[ 1 ], g_Var.Color.Surrounding[ 2 ], a );
		}

		int r, g, b;

		bool bisEnemyVis = g_Misc.IsVisible( pLocal, pEnemy );

		if(!g_Misc.bInSameTeam(pLocal, pEnemy))
		{
			bisEnemyVis ? r = g_Var.Color.PlayerVisible[ 0 ] : r = g_Var.Color.Player[ 0 ], 
				bisEnemyVis ? g = g_Var.Color.PlayerVisible[ 1 ] : g = g_Var.Color.Player[ 1 ], 
				bisEnemyVis ? b = g_Var.Color.PlayerVisible[ 2 ] : b = g_Var.Color.Player[ 2 ];

		//	bisEnemyVis ? rbone = g_Var.Color.BoneEnemyVisible[ 0 ] : rbone = g_Var.Color.BoneEnemy[ 0 ], bisEnemyVis ? gbone = g_Var.Color.BoneEnemyVisible[ 1 ] : gbone = g_Var.Color.BoneEnemy[ 1 ], bisEnemyVis ? bbone = g_Var.Color.BoneEnemyVisible[ 2 ] : bbone = g_Var.Color.BoneEnemy[ 2 ];
		//	bisEnemyVis ? rdlight = g_Var.Color.DLightsVisible[ 0 ] : rdlight = g_Var.Color.DLights[ 0 ], bisEnemyVis ? gdlight = g_Var.Color.DLightsVisible[ 1 ] : gdlight = g_Var.Color.DLights[ 1 ], bisEnemyVis ? bdlight = g_Var.Color.DLightsVisible[ 2 ] : bdlight = g_Var.Color.DLights[ 2 ];
		}
		else
		{
			bisEnemyVis ? r = g_Var.Color.TeamVisible[ 0 ] : r = g_Var.Color.Team[ 0 ],
				bisEnemyVis ? g = g_Var.Color.TeamVisible[ 1 ] : g = g_Var.Color.Team[ 1 ], 
				bisEnemyVis ? b = g_Var.Color.TeamVisible[ 2 ] : b = g_Var.Color.Team[ 2 ];
		
		//	bisEnemyVis ? rbone = g_Var.Color.TeamVisible[ 0 ] : rbone = g_Var.Color.Team[ 0 ], bisEnemyVis ? gbone = g_Var.Color.TeamVisible[ 1 ] : gbone = g_Var.Color.Team[ 1 ], bisEnemyVis ? bbone = g_Var.Color.TeamVisible[ 2 ] : bbone = g_Var.Color.Team[ 2 ];
		//	bisEnemyVis ? rdlight = g_Var.Color.TeamVisible[ 0 ] : rdlight = g_Var.Color.Team[ 0 ], bisEnemyVis ? gdlight = g_Var.Color.TeamVisible[ 1 ] : gdlight = g_Var.Color.Team[ 1 ], bisEnemyVis ? bdlight = g_Var.Color.TeamVisible[ 2 ] : bdlight = g_Var.Color.Team[ 2 ];
		}

		CBaseCombatWeapon* pWeapon = pEnemy->GetWeapon();

		if( pWeapon == nullptr )
			continue;

		VEngineClient::player_info_t pInfo;

		if( !g_pEngine->GetPlayerInfo( i, &pInfo ) )
			continue;

		static float flTime[ 33 ];
		static float flLBY[ 33 ];

		if( pEnemy->LowerBodyYawTarget() != flLBY[ i ] || ( pEnemy->GetVelocity().Length2D() > 0.1f && ( pEnemy->GetFlags() & FL_ONGROUND ) ) )
		{
			flLBY[ i ] = pEnemy->LowerBodyYawTarget();
			flTime[ i ] = g_pGlobalVars->curtime;
		}

		const matrix3x4_t& trans = *( matrix3x4_t* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_rgflCoordinateFrame );

		Vector min = *( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins );
		Vector max = *( Vector* ) ( ( DWORD ) pEnemy + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs );

		Vector points[] = { Vector( min.x, min.y, min.z ), Vector( min.x, max.y, min.z ), Vector( max.x, max.y, min.z ), Vector( max.x, min.y, min.z ), Vector( max.x, max.y, max.z ),
			Vector( min.x, max.y, max.z ), Vector( min.x, min.y, max.z ), Vector( max.x, min.y, max.z ) };

		Vector pointsTransformed[ 8 ];
		for( int i = 0; i < 8; i++ )
			g_Math.VectorTransform( points[ i ], trans, pointsTransformed[ i ] );

		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;

		if( !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 3 ], flb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 5 ], brt ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 0 ], blb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 4 ], frt ) != 1
			|| !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 2 ], frb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 1 ], brb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 6 ], blt ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 7 ], flt ) != 1 )
			continue;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bot = flb.y;

		for( int i = 1; i < 8; i++ )
		{
			if( left > arr[ i ].x )
				left = arr[ i ].x;

			if( bot < arr[ i ].y )
				bot = arr[ i ].y;

			if( right < arr[ i ].x )
				right = arr[ i ].x;

			if( top > arr[ i ].y )
				top = arr[ i ].y;
		}

		float flHeight = ( bot - top );
		float flWidth = ( right - left );
		float flMiddle = left + flWidth / 2;

		if( g_Var.Visuals.Player.Box )
		{
			g_Draw.LineRGBA( left, top, right, top, r, g, b, clamp( a, 0, 200 ) ); //TOP
			g_Draw.LineRGBA( left + 1, top + 1, right - 1, top + 1, 0, 0, 0, clamp( a, 0, 150 ) );
			g_Draw.LineRGBA( left - 1, top - 1, right + 1, top - 1, 0, 0, 0, clamp( a, 0, 150 ) );

			g_Draw.LineRGBA( right, bot, left, bot, r, g, b, clamp( a, 0, 200 ) ); //BOT
			g_Draw.LineRGBA( right - 1, bot - 1, left + 1, bot - 1, 0, 0, 0, clamp( a, 0, 150 ) );
			g_Draw.LineRGBA( right + 1, bot + 1, left - 1, bot + 1, 0, 0, 0, clamp( a, 0, 150 ) );

			g_Draw.LineRGBA( left, bot, left, top, r, g, b, clamp( a, 0, 200 ) ); //LEFT
			g_Draw.LineRGBA( left - 1, bot + 1, left - 1, top - 1, 0, 0, 0, clamp( a, 0, 150 ) );
			g_Draw.LineRGBA( left + 1, bot - 1, left + 1, top + 1, 0, 0, 0, clamp( a, 0, 150 ) );

			g_Draw.LineRGBA( right, top, right, bot, r, g, b, clamp( a, 0, 200 ) ); //RIGHT
			g_Draw.LineRGBA( right + 1, top - 1, right + 1, bot + 1, 0, 0, 0, clamp( a, 0, 150 ) );
			g_Draw.LineRGBA( right - 1, top + 1, right - 1, bot - 1, 0, 0, 0, clamp( a, 0, 150 ) );
		}

		if( g_Var.Visuals.Player.LowerBody )
			g_Draw.DrawLBYBar( left + 1, top - 5, flWidth, 3, g_Var.Color.LowerBodyBar[ 0 ], g_Var.Color.LowerBodyBar[ 1 ], g_Var.Color.LowerBodyBar[ 2 ], a, 1.1f - g_pGlobalVars->curtime + flTime[ i ] );
	
		if( g_Var.Visuals.Player.Name )
			g_Draw.DrawString( g_Draw.espfont, true, flMiddle, g_Var.Visuals.Player.LowerBody ? ( top - 15 ) : ( top - 12 ), g_Var.Color.Name[ 0 ], g_Var.Color.Name[ 1 ], g_Var.Color.Name[ 2 ], a, pInfo.name );

		if( g_Var.Visuals.Player.Health )
			g_Draw.DrawHealthBar( left - 5, top, 3, flHeight + 1, g_Var.Color.Health[ 0 ], g_Var.Color.Health[ 1 ], g_Var.Color.Health[ 2 ], a, pEnemy->GetHealth() );

		if( g_Var.Visuals.Player.AmmoBar && pWeapon->GetPrimaryClip() > 0 )
		{
			g_Draw.DrawArmorBar( left + 1, bot + 3, flWidth, 3, g_Var.Color.AmmoBar[ 0 ], g_Var.Color.AmmoBar[ 1 ], g_Var.Color.AmmoBar[ 2 ], a, pWeapon->GetClip(), pWeapon->GetPrimaryClip() );

			if( g_Var.Visuals.Player.Weapon == 1 )
				g_Draw.DrawString( g_Draw.espfont, true, flMiddle, bot + 5, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], a, pWeapon->GetWeaponName() );
			else
				if( g_Var.Visuals.Player.Weapon == 2 )
					g_Draw.DrawString( g_Draw.iconfont, true, flMiddle, bot + 8, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], a, pWeapon->GetCustomIcon() );
		}
		else
		{
			if( g_Var.Visuals.Player.Weapon == 1 )
				g_Draw.DrawString( g_Draw.espfont, true, flMiddle, bot + 2, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], a, pWeapon->GetWeaponName() );
			else
				if( g_Var.Visuals.Player.Weapon == 2 )
					g_Draw.DrawString( g_Draw.iconfont, true, flMiddle, bot + 5, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], a, pWeapon->GetCustomIcon() );
		}

		if( g_Var.Visuals.Player.KevHelm > 0 )
		{
			if( g_Var.Visuals.Player.KevHelm == 1 )
			{
				if( pEnemy->HasHelm() )
					g_Draw.DrawString( g_Draw.espfont, true, right + 6, top - 2, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], a, "H" );
				else
					if( pEnemy->GetArmor() > 0 && !pEnemy->HasHelm() )
						g_Draw.DrawString( g_Draw.espfont, true, right + 6, top - 2, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], a, "K" );
					else
						if( pEnemy->GetArmor() > 0 && pEnemy->HasHelm() )
							g_Draw.DrawString( g_Draw.espfont, true, right + 6, top + 6, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], a, "K" );
			}
			else
				if( g_Var.Visuals.Player.KevHelm == 2 )
				{
					if( pEnemy->HasHelm() )
						g_Draw.DrawString( g_Draw.iconfont, true, right + 8, top - 1, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], a, "q" );
					else
						if( pEnemy->GetArmor() > 0 && !pEnemy->HasHelm() )
							g_Draw.DrawString( g_Draw.iconfont, true, right + 8, top - 1, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], a, "r" );
						else
							if( pEnemy->GetArmor() > 0 && pEnemy->HasHelm() )
								g_Draw.DrawString( g_Draw.iconfont, true, right + 8, top - 1, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], a, "q" );
				}
		}
	}
}

void CVisuals::EntityVisuals( CBaseEntity* pLocal, int iScreenWidth, int iScreenHeight )
{
	for( int i = 64; i < g_pEntList->GetHighestEntityIndex(); i++ )
	{
		CBaseEntity* pEntity = g_pEntList->GetClientEntity( i );

		if( pEntity == nullptr || !pEntity )
			continue;

		if( g_Var.Visuals.World.Grenade[ 0 ] || g_Var.Visuals.World.Grenade[ 1 ] )
			NadeVisuals( pEntity );

		if( g_Var.Misc.Indicator[ 3 ] )
			BombVisuals( pLocal, pEntity, iScreenWidth, iScreenHeight );
	}
}

void CVisuals::Crosshair( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, int iScreenWidth, int iScreenHeight )
{
	//or weapon debug show spread
	static char* DrawCrosshairWeaponTypeCheck = ( char* ) ( g_Pattern.PatternSearchIDA( g_Offset.szClient.c_str(), "83 F8 05 75 17" ) ); //https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_csbase.cpp#L908
	
	if( g_Var.Visuals.World.Crosshair[ 0 ] && pWeapon )
	{
		if( pWeapon->IsGrenade() )
			return;

		float flCone = pWeapon->GetInaccuracy();

		if( flCone > 0.0f )
		{
			if( flCone < 0.01f )
				flCone = 0.01f;

			float flSize = ( flCone * iScreenHeight ) * 0.7f;

			g_Draw.DrawFilledCircle( Vector2D( iScreenWidth / 2, iScreenHeight / 2 ), g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], 30, ( int ) flSize, 70 );
		}
	}

	if( g_Var.Visuals.World.Crosshair[ 1 ] )
	{
		if( pLocal->GetVelocity().Length() < 0.1 && pLocal->GetPunchAngle().Length() > 0.4 )
		{
			Vector vScreen;

			if( g_pDebugOverlay->ScreenPosition( UserCmd::vHitPosition, vScreen ) != 1 )
				g_Draw.DrawString( g_Draw.visualsfont, false, vScreen.x, vScreen.y, 255, 255, 255, 255, "+" );
		}
	}

	if( g_Var.Visuals.World.Crosshair[ 2 ] && pLocal && pWeapon)
	{
		float flDamage;

		if( g_Autowall.CanBangForward( pLocal, flDamage ) )
		{
			if( flDamage >= 1.f )
				g_Draw.DrawFilledCircle(Vector2D(iScreenWidth / 2, iScreenHeight / 2), 0, 255, 0, 200, 1, 70);
			else
				g_Draw.DrawFilledCircle(Vector2D(iScreenWidth / 2, iScreenHeight / 2), 255, 0, 0, 200, 1, 70);
		}
		else
			g_Draw.DrawFilledCircle(Vector2D(iScreenWidth / 2, iScreenHeight / 2), 255, 0, 0, 200, 1, 70);

	/*	CWeaponData* pWeaponData = pWeapon->GetWeaponData();

		if( pWeaponData )
		{
			Vector ViewAngles, vecDir;
			g_pEngine->GetViewAngles( ViewAngles );
			g_Math.AngleVector( ViewAngles, vecDir );
			auto vecEnd = pLocal->GetEyePosition() + vecDir * pWeapon->GetWeaponData()->range;
			auto flDamage = CGamePenetration::GetPenetrationDamageAfterFirstWall( pLocal, pWeapon, vecEnd );

			if( flDamage == 0.f )
			{
				g_Draw.FillRGBA( float( iScreenWidth / 2 - 0.5 ), float( iScreenHeight / 2 - 0.5 ), 2.5, 2.5, 255, 0, 0, 255 );
			}
			else
			{
				if( flDamage >= 1.f )
					g_Draw.FillRGBA( float( iScreenWidth / 2 - 0.5 ), float( iScreenHeight / 2 - 0.5 ), 2.5, 2.5, 0, 255, 0, 255 );
				else
					g_Draw.FillRGBA( float( iScreenWidth / 2 - 0.5 ), float( iScreenHeight / 2 - 0.5 ), 2.5, 2.5, 255, 0, 0, 255 );
			}
		}*/
	}
	
	DWORD dwOldProtect;
	VirtualProtectEx( GetCurrentProcess(), DrawCrosshairWeaponTypeCheck, 3, PAGE_EXECUTE_READWRITE, &dwOldProtect );

	if( g_Var.Visuals.World.Crosshair[ 3 ] && *( ( DrawCrosshairWeaponTypeCheck + 0x2 ) ) != 0xFF && !pLocal->bInScope() )
		*( ( DrawCrosshairWeaponTypeCheck + 0x2 ) ) = 0xFF;
	else
		*( ( DrawCrosshairWeaponTypeCheck + 0x2 ) ) = 0x5;

	VirtualProtectEx( GetCurrentProcess(), DrawCrosshairWeaponTypeCheck, 3, dwOldProtect, &dwOldProtect );
}

void CVisuals::BoundingBox( CBaseEntity* pEntity, float * flSides )
{
	const matrix3x4_t& trans = *( matrix3x4_t* ) ( ( DWORD ) pEntity + Offsets::Entity::m_rgflCoordinateFrame );

	Vector min = *( Vector* ) ( ( DWORD ) pEntity + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMins );
	Vector max = *( Vector* ) ( ( DWORD ) pEntity + Offsets::Entity::m_Collision + Offsets::Entity::m_vecMaxs );

	Vector points[] = { Vector( min.x, min.y, min.z ), Vector( min.x, max.y, min.z ), Vector( max.x, max.y, min.z ), Vector( max.x, min.y, min.z ), Vector( max.x, max.y, max.z ),
		Vector( min.x, max.y, max.z ), Vector( min.x, min.y, max.z ), Vector( max.x, min.y, max.z ) };

	Vector pointsTransformed[ 8 ];
	for( int i = 0; i < 8; i++ )
		g_Math.VectorTransform( points[ i ], trans, pointsTransformed[ i ] );

	Vector flb;
	Vector brt;
	Vector blb;
	Vector frt;
	Vector frb;
	Vector brb;
	Vector blt;
	Vector flt;

	if( !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 3 ], flb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 5 ], brt ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 0 ], blb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 4 ], frt ) != 1
		|| !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 2 ], frb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 1 ], brb ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 6 ], blt ) != 1 || !g_pDebugOverlay->ScreenPosition( pointsTransformed[ 7 ], flt ) != 1 )
		return;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bot = flb.y;

	for( int i = 1; i < 8; i++ )
	{
		if( left > arr[ i ].x )
			left = arr[ i ].x;

		if( bot < arr[ i ].y )
			bot = arr[ i ].y;

		if( right < arr[ i ].x )
			right = arr[ i ].x;

		if( top > arr[ i ].y )
			top = arr[ i ].y;
	}

	flSides[ SIDE_LEFT ] = left;
	flSides[ SIDE_RIGHT ] = right;
	flSides[ SIDE_BOT ] = bot;
	flSides[ SIDE_TOP ] = top;
}

void CVisuals::InitRecoil( CUserCmd* pCmd, CBaseEntity* pLocal )
{
	Vector vView = pCmd->viewangles + pLocal->GetPunchAngle() * 2.0f;
	Vector vStart = pLocal->GetEyePosition();
	Vector vEnd;

	CEngineTraceClient::Ray_t pRay;
	CEngineTraceClient::trace_t Trace;
	CEngineTraceClient::CTraceFilter TraceFilter;
	TraceFilter.pSkip = pLocal;

	g_Math.AngleVector( vView, vEnd );

	vEnd += vStart;

	pRay.Init( vStart, vEnd );

	g_pEngineTrace->TraceRay( pRay, MASK_SHOT, &TraceFilter, &Trace );

	UserCmd::vHitPosition = Trace.endpos;
}

bool CVisuals::HasVisualRecoil( CBaseEntity* pLocal )
{
	vAimPunch = ( Vector* ) ( ( DWORD ) pLocal + Offsets::Player::m_Local + Offsets::Player::m_aimPunchAngle );
	vViewPunch = ( Vector* ) ( ( DWORD ) pLocal + Offsets::Player::m_Local + Offsets::Player::m_ViewPunchAngle );

	if( vAimPunch && vViewPunch )
	{
		vOldAimPunch = *vAimPunch;
		vOldViewPunch = *vViewPunch;

		vAimPunch->Zero();
		vViewPunch->Zero();

		return true;
	}

	return false;
}

void CVisuals::SpecList( CBaseEntity* pLocal, int iScreenWidth, int iScreenHeight )
{
	int ii = 0;

	if( pLocal->GetLifeState() != LIFE_ALIVE )
		return;

	for( int i = 1; i <= 32; i++ )
	{
		CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

		if( !pEnemy )
			continue;

		if( pEnemy->GetHealth() > 0 )
			continue;

		if( pEnemy->IsDormant() )
			continue;

		if( pEnemy->GetLifeState() == LIFE_ALIVE )
			continue;

		if( pEnemy->GetObserverTarget() != 0/*OBS_MODE_NONE*/ )
		{
			VEngineClient::player_info_t pEnemyInfo;
			if( g_pEngine->GetPlayerInfo( pEnemy->GetIndex(), &pEnemyInfo ) && !pEnemyInfo.fakeplayer && !pEnemyInfo.ishltv )
			{
				CBaseEntity* pTarget = g_pEntList->GetClientEntityFromHandle( pEnemy->GetObserverTarget() );

				if( !pTarget )
					continue;

				VEngineClient::player_info_t pVictim;

				if( pTarget == pLocal && g_pEngine->GetPlayerInfo( pTarget->GetIndex(), &pVictim ) )
				{
					ii++;
					g_Draw.DrawString( g_Draw.keyfont, false, iScreenWidth - g_Draw.getWidth( g_Draw.keyfont, pEnemyInfo.name ) - 5, 5 + ( 16 * ii ), 255, 255, 255, 255, pEnemyInfo.name );
				}
			}
		}
	}
	ii = 0;
}

void CVisuals::RunMaterials()
{
	static auto mat_postprocess_enable = g_pICvar->FindVar( "mat_postprocess_enable" );

	if( g_Var.Visuals.World.Removals[ 5 ] )
		mat_postprocess_enable->SetValue( 0 );
	else
		mat_postprocess_enable->SetValue( 1 );

	static float flNightMode = g_Var.Visuals.World.Nightmode;
	static float flPropAlpha = g_Var.Visuals.World.PropsAlpha;

	g_Misc.RemoveFlash();

	g_Misc.RemoveSmoke();

	if( g_Visuals.bNewMap )
	{
		flPropAlpha = 1337;
		flNightMode = 1337;
		return;
	}

	static auto r_DrawSpecificStaticProp = g_pICvar->FindVar( "r_DrawSpecificStaticProp" );
	static auto r_dlightsenable = g_pICvar->FindVar( "r_dlightsenable" );
	static DWORD dwLoadedSky = g_Offset.loadSky;//skybox/%s%s sig func with it
	static auto LoadNamedSky = reinterpret_cast< void( __fastcall* )( const char* ) >( dwLoadedSky );
	static ConVar* skybox = g_pICvar->FindVar( "sv_skyname" );

	if( g_Var.Visuals.World.PropsAlpha < 100 || g_Var.Visuals.World.Nightmode < 100 )
	{
		if( r_DrawSpecificStaticProp->GetInt() != 0 )
			r_DrawSpecificStaticProp->SetValue( 0 );

		if( r_dlightsenable->GetInt() != 1 )
			r_dlightsenable->SetValue( 1 );
	}
	else
	{
		if( r_DrawSpecificStaticProp->GetInt() != 1 )
			r_DrawSpecificStaticProp->SetValue( 1 );

		if( r_dlightsenable->GetInt() != 0 )
			r_dlightsenable->SetValue( 0 );
	}

	if( flPropAlpha != g_Var.Visuals.World.PropsAlpha )
	{
		flPropAlpha = g_Var.Visuals.World.PropsAlpha;

		for( MaterialHandle_t handle = g_pMatsystem->FirstMaterial(); handle != g_pMatsystem->InvalidMaterial(); handle = g_pMatsystem->NextMaterial( handle ) )
		{
			IMaterial* pMat = g_pMatsystem->GetMaterial( handle );

			if( pMat == nullptr || pMat->IsErrorMaterial() )
				continue;

			const char* pszGroupName = pMat->GetTextureGroupName();

			if( !pszGroupName )
				continue;

			const int iGroupName = strlen( pszGroupName );

			if( !iGroupName )
				continue;

			const char* pszMaterialName = pMat->GetName();

			if( !pszMaterialName )
				continue;

			int iMatName = strlen( pszMaterialName );

			if( !iMatName )
				continue;

			if( g_Var.Visuals.World.PropsAlpha < 100 )
			{
				if( strstr( pszGroupName, "StaticProp" ) )
				{
					pMat->SetMaterialVarFlag( MATERIAL_VAR_FLAT, true );
					pMat->SetMaterialVarFlag( MATERIAL_VAR_VERTEXALPHA, true );
					pMat->AlphaModulate( g_Var.Visuals.World.PropsAlpha / 100 );
				}
			}
			else
				pMat->AlphaModulate( 1.f );
		}
	}

	if( flNightMode != g_Var.Visuals.World.Nightmode )
	{
		flNightMode = g_Var.Visuals.World.Nightmode;

		for( MaterialHandle_t handle = g_pMatsystem->FirstMaterial(); handle != g_pMatsystem->InvalidMaterial(); handle = g_pMatsystem->NextMaterial( handle ) )
		{
			IMaterial* pMat = g_pMatsystem->GetMaterial( handle );

			if( !pMat || pMat->IsErrorMaterial() || pMat == nullptr )
				continue;

			const char* pszGroupName = pMat->GetTextureGroupName();

			if( !pszGroupName )
				continue;

			const int iGroupName = strlen( pszGroupName );

			if( !iGroupName )
				continue;

			const char* pszMaterialName = pMat->GetName();

			if( !pszMaterialName )
				continue;

			int iMatName = strlen( pszMaterialName );

			if( !iMatName )
				continue;

			if( g_Var.Visuals.World.Nightmode < 100 )
			{
				LoadNamedSky( "sky_csgo_night02" );

				float r = g_Var.Visuals.World.Nightmode / 100.f, g = g_Var.Visuals.World.Nightmode / 100.f, b = g_Var.Visuals.World.Nightmode / 100;

				if( strstr( pszGroupName, "World textures" ) )
					pMat->ColorModulate( r, g, b );

				if( strstr( pszGroupName, "StaticProp" ) )
					pMat->ColorModulate( r + 0.1f, g + 0.1f, b + 0.1f );

				if( strstr( pszMaterialName, "models/props/de_dust/palace_pillars" ) )
					pMat->ColorModulate( r + 0.1f, g + 0.1f, b + 0.1f );

				g_pEngine->ExecuteClientCmd( "clear" );
			}
			else
			{
				LoadNamedSky( skybox->m_strstring );
				pMat->ColorModulate( 1.f, 1.f, 1.f );
			}
		}
	}
}

void CVisuals::NadeVisuals( CBaseEntity* pEntity )
{
	Vector vGrenadePos2D = Vector( 0.f, 0.f, 0.f );
	Vector vGrenadePos3D = Vector( 0.f, 0.f, 0.f );
	float fGrenadeModelSize = 0.0f;
	const model_t *model = pEntity->GetModel();
	char* WhatIconChar = "";
	char* WhatModelName = "";

	if( model )
	{
		studiohdr_t* hdr = g_pModel->GetStudiomodel( model );
		if( hdr )
		{
			std::string grenades = g_pModel->GetModelName( model );
			std::string grenade = g_pModel->GetModelName( pEntity->GetModel() );

			if( grenade.find( "_thrown", 0 ) != std::string::npos || grenade.find( "dropped", 0 ) != std::string::npos )
			{
				if( !( grenade.find( "w_eq", 0 ) != std::string::npos ) || ( grenade.find( "taser", 0 ) != std::string::npos ) || ( grenade.find( "defuser", 0 ) != std::string::npos ) || ( grenade.find( "multimeter", 0 ) != std::string::npos ) )
					return;

				vGrenadePos3D = pEntity->GetAbsOrigin();
				fGrenadeModelSize = sqrt( hdr->hull_max.DistToSqr( hdr->hull_min ) );

				if( g_pDebugOverlay->ScreenPosition( vGrenadePos3D, vGrenadePos2D ) == 1 )
					return;

				if( grenade.find( "flash" ) != std::string::npos )
				{
					WhatIconChar = "i";
					WhatModelName = "Flash";
				}

				if( grenade.find( "incendiarygrenade" ) != std::string::npos || grenade.find( "molotov" ) != std::string::npos || grenade.find( "fraggrenade" ) != std::string::npos )
				{
					if( grenade.find( "incendiarygrenade" ) != std::string::npos )
					{
						WhatIconChar = "l";
						WhatModelName = "Incendiary";
					}

					if( grenade.find( "molotov" ) != std::string::npos )
					{
						WhatIconChar = "l";
						WhatModelName = "Molotov";
					}

					if( grenade.find( "fraggrenade" ) != std::string::npos )
					{
						WhatIconChar = "j";
						WhatModelName = "Grenade";
					}
				}

				if( grenade.find( "smoke" ) != std::string::npos || grenade.find( "decoy" ) != std::string::npos )
				{
					if( grenade.find( "smoke" ) != std::string::npos )
					{
						WhatIconChar = "k";
						WhatModelName = "Smoke";
					}

					if( grenade.find( "decoy" ) != std::string::npos )
					{
						WhatIconChar = "m";
						WhatModelName = "Decoy";
					}
				}

				if( g_Var.Visuals.World.Grenade[ 0 ] )
					g_Draw.DrawString( g_Draw.espfont, true, vGrenadePos2D.x, vGrenadePos2D.y + 5, 255, 255, 255, 255, WhatModelName );

				if( g_Var.Visuals.World.Grenade[ 1 ] )
					g_Draw.DrawString( g_Draw.iconfont, true, vGrenadePos2D.x, vGrenadePos2D.y - 5, 255, 255, 255, 255, WhatIconChar );
			}
		}
	}
}

inline float CSGO_Armor( float flDamage, int ArmorValue )
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if( ArmorValue > 0 ) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = ( flDamage - flNew ) * flArmorBonus;

		if( flArmor > static_cast< float >( ArmorValue ) )
		{
			flArmor = static_cast< float >( ArmorValue )* ( 1.f / flArmorBonus );
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

void CVisuals::BombVisuals( CBaseEntity* pLocal, CBaseEntity* pEntity, int iScreenWidth, int iScreenHeight )
{
	Vector vScreen;

	int iClassID = pEntity->GetClientClass()->GetClassID();

	if( g_pEvents->bResetBomb )
	{
		g_pEvents->bGetsPlanted = false;
		g_pEvents->bDefused = false;
		g_pEvents->bExploded = false;
		g_pEvents->bDefusing = false;
	}

	if( iClassID == CPlantedC4 && !g_pEvents->bDefused && !g_pEvents->bExploded && !g_pEvents->bResetBomb )
	{
		Vector vPos = pEntity->GetAbsOrigin();

		float flBombTimer = pEntity->GetC4Blow();
		float flTimeLeft = ( g_pGlobalVars->curtime - flBombTimer ) * -1;
		float flDefuseTimer = pEntity->GetDefuseLenght();
		float TimeLeft = ( g_pGlobalVars->curtime - flDefuseTimer ) * -1;

		/*static DWORD m_bombsiteCenterA = g_pNetVars->GetOffset( "CCSPlayerResource", "m_bombsiteCenterA" );
		static DWORD m_bombsiteCenterB = g_pNetVars->GetOffset( "CCSPlayerResource", "m_bombsiteCenterB" );

		Vector vASite = *( Vector* ) ( ( DWORD ) pEntity + m_bombsiteCenterA );
		Vector vBSite = *( Vector* ) ( ( DWORD ) pEntity + m_bombsiteCenterB );

		Vector vDeltaA = Vector( fabsf( vASite.x - vPos.x ), fabsf( vASite.y - vPos.y ), 0 );
		Vector vDeltaB = Vector( fabsf( vBSite.x - vPos.x ), fabsf( vBSite.y - vPos.y ), 0 );*/

		if( flTimeLeft > 0.0f )
		{
			Vector vScreen;
			if( g_pDebugOverlay->ScreenPosition( vPos, vScreen ) != 1 )
				g_Draw.DrawString( g_Draw.iconfont, false, vScreen.x, vScreen.y, 200, 200, 200, 255, "o" );

			/*			if( vDeltaB.Length() > vDeltaA.Length() )
			g_Draw.DrawString( g_Draw.espfont, false, 1, Height / 2, 200, 200, 200, 255, "Bomb at A Explodes in: %2.0f", flTimeLeft );
			else
			g_Draw.DrawString( g_Draw.espfont, false, 1, Height / 2, 200, 200, 200, 255, "Bomb at B Explodes in: %2.0f", flTimeLeft );*/

			g_Draw.DrawString( g_Draw.keyfont, true, iScreenWidth / 2, iScreenHeight - 35, 200, 200, 200, 255, "Explodes in: %2.0f", flTimeLeft );

			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ( ( Vector( pEntity->GetOrigin() - pLocal->GetOrigin() ).Length() - b ) / c );
			float flDamage = a*exp( -d * d );

			int iDamage = max( ceilf( CSGO_Armor( flDamage, pLocal->GetArmor() ) ), 0 ) - 1;

			if( pLocal->GetHealth() > 0 )
			{
				if( iDamage >= pLocal->GetHealth() )
					g_Draw.DrawString( g_Draw.keyfont, true, iScreenWidth / 2, iScreenHeight - 20, 200, 0, 0, 255, "Damage: %i", iDamage );
				else
					g_Draw.DrawString( g_Draw.keyfont, true, iScreenWidth / 2, iScreenHeight - 20, 0, 200, 0, 255, "Damage: %i", iDamage );
			}
		}
	}
}

void DrawArrow( int x, int y, int w, int h, int dir, int r, int g, int b, int a )
{
	switch( dir )
	{
	case 0:
		for( int i = 0; i < h; i++ )
			g_Draw.FillRGBA( x - i * 2, y + i, w + i, h - i * 2, r, g, b, a );
		break;
	case 1:
		for( int i = 0; i < h; i++ )
			g_Draw.FillRGBA( x + i, y + i, w + i, h - i * 2, r, g, b, a );
		break;
	case 2:
		for( int i = 0; i < w; i++ )
			g_Draw.FillRGBA( x + i, y - i * 2, w - i * 2, h + i, r, g, b, a );
		break;
	case 3:
		for( int i = 0; i < w; i++ )
			g_Draw.FillRGBA( x + i, y + i, w - i * 2, h + i, r, g, b, a );
		break;
	}
}

void DrawAntiAimLine(Vector2D center, float flYaw, float flDistCenter, float flLength, int r, int g, int b, int a)
{
	Vector2D vSlope = { sin(DEG2RAD(-flYaw)), -cos(DEG2RAD(flYaw)) };
	Vector2D vDirection = vSlope * flLength;
	Vector2D vStart = center + (vSlope * flDistCenter);
	Vector2D vEnd = vStart + (vDirection);

	g_Draw.LineRGBA(vStart.x, vStart.y, vEnd.x, vEnd.y, r, g, b, a);
}

void CVisuals::InitIndicators( CBaseEntity* pLocal, int iScreenWidth, int iScreenHeight )
{
	int iMid = iScreenHeight / 2;

	if( g_Var.Misc.Indicator[ 0 ] )
	{
		if( g_AntiAim.m_bLeft )
			DrawArrow( iScreenWidth / 2 - 35, iScreenHeight / 2 - 8, 2, 16, 0, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], 255 );

		if( g_AntiAim.m_bRight )
			DrawArrow( iScreenWidth / 2 + 35, iScreenHeight / 2 - 8, 2, 16, 1, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], 255 );

		if( g_AntiAim.m_bFront )
			DrawArrow( iScreenWidth / 2 - 8, iScreenHeight / 2 - 35, 16, 2, 2, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], 255 );

		if( g_AntiAim.m_bBack )
			DrawArrow( iScreenWidth / 2 - 8, iScreenHeight / 2 + 35, 16, 2, 3, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], 255 );

		if( g_AntiAim.m_bLock )
			g_Draw.DrawString( g_Draw.indicatorfont, false, 2, iMid, 255, 255, 255, 255, "LOCK" );
	}

	if( g_Var.Misc.Indicator[ 1 ] )
	{
		Vector vDelta = Vector( 0, fabsf( UserCmd::vAntiAimReal.y - pLocal->LowerBodyYawTarget() ), 0 );

		g_Math.NormalizeVector( vDelta );

		if (g_Var.AntiAim.Break >= 3)
		{
			float flDownScale = g_Misc.MaxAbsYawDelta(pLocal) * 4.39655f;

			g_Draw.DrawString(g_Draw.indicatorfont, false, 2, (g_Var.Misc.Indicator[0] && g_AntiAim.m_bLock) ? iMid + 20 : iMid, 255 - flDownScale, flDownScale, 0, 255, "FAKE");
			g_Draw.FillRGBA(4, (g_Var.Misc.Indicator[0] && g_AntiAim.m_bLock) ? iMid + 39 : iMid + 19, g_Misc.MaxAbsYawDelta(pLocal) - 12, 3, 255, 255, 255, 255);
		}
		else
		{
			if (vDelta.y <= 35 && vDelta.y >= -35)
				g_Draw.DrawString(g_Draw.indicatorfont, false, 2, (g_Var.Misc.Indicator[0] && g_AntiAim.m_bLock) ? iMid + 20 : iMid, 255, 0, 0, 255, "FAKE");
			else
				g_Draw.DrawString(g_Draw.indicatorfont, false, 2, (g_Var.Misc.Indicator[0] && g_AntiAim.m_bLock) ? iMid + 20 : iMid, 0, 255, 0, 255, "FAKE");

			static float flTime;

			if (vDelta.y <= 35 && vDelta.y >= -35)
				flTime = UserCmd::flFixedCurTime;

			g_Draw.FillRGBA(4, (g_Var.Misc.Indicator[0] && g_AntiAim.m_bLock) ? iMid + 39 : iMid + 19, clamp(48 * (1.1f - UserCmd::flFixedCurTime + flTime) / 1.1 - 2, 0, 48), 3, 255, 255, 255, 255);
		}
	}

	if( g_Var.Misc.Indicator[ 2 ] )
	{
		static int iCount, iPackets;
		iCount++;

	//	if( iCount > 60 )
		{
			iPackets = g_pClientState->GetChokedCmds();
			iCount = 0;
		}

		if( g_Var.Misc.Indicator[ 0 ] && g_AntiAim.m_bLock )
		{
			g_Draw.DrawString( g_Draw.indicatorfont, false, 2, g_Var.Misc.Indicator[ 1 ] ? iMid + 44 : iMid + 20, ( iPackets * 6 ) * 2.834, 255 - ( iPackets * 6 ) * 2.834, 0, 255, "FAKELAG" );
			g_Draw.FillRGBA(4, g_Var.Misc.Indicator[ 1 ] ? iMid + 63 : iMid + 39, iPackets * 5.7, 3, 255, 255, 255, 255 );
		}
		else
		{
			g_Draw.DrawString( g_Draw.indicatorfont, false, 2, g_Var.Misc.Indicator[ 1 ] ? iMid + 24 : iMid, ( iPackets * 6 ) * 2.834, 255 - ( iPackets * 6 ) * 2.834, 0, 255, "FAKELAG" );
			g_Draw.FillRGBA(4, g_Var.Misc.Indicator[ 1 ] ? iMid + 43 : iMid + 19, iPackets * 5.7, 3, 255, 255, 255, 255 );
		}
	}

	if (g_Var.Misc.Indicator[5])
	{
		if ((!(pLocal->GetFlags() & FL_ONGROUND) || (UserCmd::pCmd->buttons & IN_JUMP)) && pLocal->GetVelocity().Length() > 280)
		{
			int iRed = 255 - clamp(UserCmd::flTeleportDistance / 16, 1, 255);
			int iGreen = clamp(UserCmd::flTeleportDistance / 16, 1, 255);

			//4096.f / 47
			if (g_Var.Misc.Indicator[0] && g_AntiAim.m_bLock)
			{
				if (g_Var.Misc.Indicator[2])
				{
					g_Draw.DrawString(g_Draw.indicatorfont, false, 2, g_Var.Misc.Indicator[1] ? iMid + 68 : iMid + 44, iRed, iGreen, 0, 255, "LAGCOMP");
					g_Draw.FillRGBA(4, g_Var.Misc.Indicator[1] ? iMid + 87 : iMid + 63, clamp(UserCmd::flTeleportDistance / 47, 1, 87), 3, 255, 255, 255, 255);
				}
				else
				{
					g_Draw.DrawString(g_Draw.indicatorfont, false, 2, g_Var.Misc.Indicator[1] ? iMid + 48 : iMid + 24, iRed, iGreen, 0, 255, "LAGCOMP");
					g_Draw.FillRGBA(4, g_Var.Misc.Indicator[1] ? iMid + 67 : iMid + 43, clamp(UserCmd::flTeleportDistance / 47, 1, 87), 3, 255, 255, 255, 255);
				}
			}
			else
			{
				if (g_Var.Misc.Indicator[2])
				{
					g_Draw.DrawString(g_Draw.indicatorfont, false, 2, g_Var.Misc.Indicator[1] ? iMid + 48 : iMid + 24, iRed, iGreen, 0, 255, "LAGCOMP");
					g_Draw.FillRGBA(4, g_Var.Misc.Indicator[1] ? iMid + 67 : iMid + 43, clamp(UserCmd::flTeleportDistance / 47, 1, 87), 3, 255, 255, 255, 255);
				}
				else
				{
					g_Draw.DrawString(g_Draw.indicatorfont, false, 2, g_Var.Misc.Indicator[1] ? iMid + 24 : iMid, iRed, iGreen, 0, 255, "LAGCOMP");
					g_Draw.FillRGBA(4, g_Var.Misc.Indicator[1] ? iMid + 43 : iMid + 19, clamp(UserCmd::flTeleportDistance / 47, 1, 87), 3, 255, 255, 255, 255);
				}
			}
		}
	}

	/*	if( g_Var.Misc.Indicator[ 3 ] )//lc
	{

	}*/

	// Angle lines indicator cos I miss something like that here :3
	if( g_Var.Misc.Indicator[ 4 ] )
	{
		if (*reinterpret_cast<bool*>(uintptr_t(g_pInput) + 0xAD))
		{
			if (pLocal->GetFlags() & FL_ONGROUND)
			{
				Vector LBYForward, RealForward, FakeForward;
				constexpr float m_LineLength = 27.5f;

				Vector Origin = pLocal->GetOrigin();
				Origin.z += 5.f;

				Vector LBYAngles = Vector(0.f, pLocal->LowerBodyYawTarget(), 0.f);
				g_Math.AngleVector(LBYAngles, LBYForward);
				Vector LBYEnd = Origin + LBYForward.Normalized() * m_LineLength;

				Vector FakeAngles = Vector(0.f, pLocal->GetEyeAngles().y, 0.f);
				g_Math.AngleVector(FakeAngles, FakeForward);
				Vector FakeEnd = Origin + FakeForward.Normalized() * m_LineLength;

				Vector RealAngles = Vector(0.f, UserCmd::vAntiAimReal.y, 0.f);
				g_Math.AngleVector(RealAngles, RealForward);
				Vector RealEnd = Origin + RealForward.Normalized() * m_LineLength;

				float vel = pLocal->GetVelocity().Length();

			//	if (vel <= 200.f)
				{
					auto alpha = 200;//static_cast<int>((200.f - vel) / 200.f * 255.f);

					auto DrawIndicatorIn3DSpace = [alpha](const Vector& f, const Vector& t, int r, int g, int b, const char* txt)
					{
						Vector S1, S2;

						if (g_pDebugOverlay->ScreenPosition(f, S1) != 1 && g_pDebugOverlay->ScreenPosition(t, S2) != 1)
						{
							g_Draw.LineRGBA(S1.x, S1.y, S2.x, S2.y, r, g, b, max(alpha - 65, 0));
							g_Draw.DrawString(g_Draw.visualsfont, true, S2.x, S2.y - 5, 255, 255, 255, alpha, txt);
						}
					};

				//	DrawIndicatorIn3DSpace(Origin, LBYEnd, 255, 0, 0, "LBY");
					DrawIndicatorIn3DSpace(Origin, FakeEnd, 0, 200, 0, "");
					DrawIndicatorIn3DSpace(Origin, RealEnd, 0, 0, 200, "");
				}
			}
		}
		else
		{
			Vector vView;
			g_pEngine->GetViewAngles(vView);

			Vector2D vCenter = Vector2D(iScreenWidth / 2, iScreenHeight / 2);

			DrawAntiAimLine(vCenter, pLocal->GetEyeAngles().y - vView.y, 25, 20, 0, 200, 0, 200);
			DrawAntiAimLine(vCenter, UserCmd::vAntiAimReal.y - vView.y, 25, 20, 0, 0, 200, 200);
		}
	}
}

void CVisuals::DrawAimbot( CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon )
{
	if( pWeapon )
	{
		int iWpnID = pWeapon->GetGroupID();

		if( g_Var.Misc.DebugMultiPoint <= 0 )
			return;

		for( int i = 1; i <= 32; i++ )
		{
			CBaseEntity* pEnemy = g_pEntList->GetClientEntity( i );

			if( !pEnemy->PlayerCheck() )
				continue;

			if( pEnemy == pLocal )
				continue;

			if( pEnemy->GetTeamNumber() == pLocal->GetTeamNumber() )
				continue;

			matrix3x4_t matrix[ 128 ];

			if( !pEnemy->SetupBones( matrix, 128, 256, g_pEngine->GetLastTimeStamp() ) )
				continue;

			Vector vScreen;
			Vector vHitbox;
			float flDamage = 0.f;
			float flCurrentDamage = 0.f;

			for( int i = 0; i < HITBOX_MAX; i++ )
			{
				vHitbox = g_Rage.CalculatePoint( pLocal, pEnemy, pWeapon, i, g_Var.Ragebot[ iWpnID ].Mindamage, matrix, flDamage, g_Var.Ragebot[ iWpnID ].Multipoint );

				if( vHitbox == Vector( 0.f, 0.f, 0.f ) )
					continue;

				if( g_pDebugOverlay->ScreenPosition( vHitbox, vScreen ) != 1 )
				{
					if( flDamage > flCurrentDamage )
					{
						flCurrentDamage = flDamage;

						if( g_Var.Misc.DebugMultiPoint == 1 )
							g_Draw.FillRGBA( vScreen.x, vScreen.y, 2, 2, 0, 255, 0, 255 );
						else
							g_Draw.DrawString( g_Draw.visualsfont, false, vScreen.x, vScreen.y, 0, 255, 0, 255, "%3.0f", flDamage );
					}
					else
					{
						if( g_Var.Misc.DebugMultiPoint == 2 )
							g_Draw.DrawString( g_Draw.visualsfont, false, vScreen.x, vScreen.y, 255, 255, 255, 255, "%3.0f", flDamage );
					}
				}
			}
		}
	}
}

void CVisuals::DrawHitbox( int iIndex )
{
	CBaseEntity* pEnemy = g_pEntList->GetClientEntity( iIndex );

	if( !pEnemy )
		return;

	studiohdr_t* pStudioModel = g_pModel->GetStudiomodel( pEnemy->GetModel() );

	if( !pStudioModel )
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet( 0 );

	if( !pHitboxSet )
		return;

	if( g_History.vUsedRecord == nullptr )
		return;

	for( int i = 0; i < pHitboxSet->numhitboxes; i++ )
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox( i );

		if( !pHitbox )
			continue;

		if (pHitbox->radius > -1)
		{
			Vector vMin, vMax;
			g_Math.VectorTransform(pHitbox->bbmin, g_History.vUsedRecord->m_Matrix[pHitbox->bone], vMin);
			g_Math.VectorTransform(pHitbox->bbmax, g_History.vUsedRecord->m_Matrix[pHitbox->bone], vMax);
			g_pDebugOverlay->DrawPill(vMin, vMax, pHitbox->radius, 255, 255, 255, 100, 3.8f);
		}
		else
		{
			Vector vPos = g_Math.MatrixGetPosition(g_History.vUsedRecord->m_Matrix[pHitbox->bone]);

			matrix3x4_t mRotation;
			g_Math.AngleMatrix(pHitbox->rotation, mRotation);

			matrix3x4_t mTransform = g_Math.MultiplyMatrix(g_History.vUsedRecord->m_Matrix[pHitbox->bone], mRotation);

			QAngle vAngles;
			g_Math.MatrixAngle(mTransform, vAngles, vPos);

			g_pDebugOverlay->AddSweptBoxOverlay(vPos, vPos, pHitbox->bbmin, pHitbox->bbmax, vAngles, 180, 180, 180, 100, 3.8f);
		}
	}
}

void CVisuals::DrawHitbox(int iIndex, matrix3x4_t* mat)
{
	CBaseEntity* pEnemy = g_pEntList->GetClientEntity(iIndex);

	if (!pEnemy)
		return;

	studiohdr_t* pStudioModel = g_pModel->GetStudiomodel(pEnemy->GetModel());

	if (!pStudioModel)
		return;
	
	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);

	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox(i);

		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		g_Math.VectorTransform(pHitbox->bbmin, mat[pHitbox->bone], vMin); //nullptr???
		g_Math.VectorTransform(pHitbox->bbmax, mat[pHitbox->bone], vMax);

		if (pHitbox->radius > -1)
			g_pDebugOverlay->DrawPill(vMin, vMax, pHitbox->radius, 255, 255, 255, 100, 3.f);
		else
		{
			Vector vPos = g_Math.MatrixGetPosition(mat[pHitbox->bone]);

			matrix3x4_t mRotation;
			g_Math.AngleMatrix(pHitbox->rotation, mRotation);

			matrix3x4_t mTransform = g_Math.MultiplyMatrix(mat[pHitbox->bone], mRotation);

			QAngle vAngles;
			g_Math.MatrixAngle(mTransform, vAngles, vPos);

			g_pDebugOverlay->AddSweptBoxOverlay(vPos, vPos, pHitbox->bbmin, pHitbox->bbmax, vAngles, 180, 180, 180, 100, 3.8f);
		}
	}
}

void CVisuals::DrawLocalFake(CBaseEntity* pLocal, matrix3x4_t* mMatrix)
{
	studiohdr_t* pStudioModel = g_pModel->GetStudiomodel(pLocal->GetModel());

	if (!pStudioModel)
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);

	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox(i);

		if (!pHitbox)
			continue;

		Vector vMin, vMax;
		g_Math.VectorTransform(pHitbox->bbmin, mMatrix[pHitbox->bone], vMin); //nullptr???
		g_Math.VectorTransform(pHitbox->bbmax, mMatrix[pHitbox->bone], vMax);

		if (pHitbox->radius > -1)
			g_pDebugOverlay->DrawPill(vMin, vMax, pHitbox->radius, g_Var.Color.LocalFakeChams[0], g_Var.Color.LocalFakeChams[1], g_Var.Color.LocalFakeChams[2], 50, g_pGlobalVars->interval_per_tick);
		else
		{
			Vector vPos;
			QAngle vAngles;
			g_Math.MatrixAngle(mMatrix[pHitbox->bone], vAngles, vPos);
			g_pDebugOverlay->AddSweptBoxOverlay(vPos, vPos, pHitbox->bbmin, pHitbox->bbmax, vAngles, g_Var.Color.LocalFakeChams[0], g_Var.Color.LocalFakeChams[1], g_Var.Color.LocalFakeChams[2], 30, g_pGlobalVars->interval_per_tick);
		}
	}
}