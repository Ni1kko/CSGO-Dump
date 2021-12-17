#pragma once
#include "../../stdafx.h"
#include "Aimbot.h"

static float	flHurtTime;
static int		iHealth;
static 	int		iVictim;

template <typename t> t clamp_value(t value, t min, t max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}


static __forceinline float DotProduct(const float *a, const float *b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void VectorTransform(const float *in1, const matrix3x4& in2, float *out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}
static void VectorTransform(const Vector& in1, const matrix3x4& in2, Vector& out)
{
	VectorTransform(&in1.x, in2, &out.x);
}

__forceinline float DotProducttt(const float *a, const float *b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void VectorTransformmm(const float *in1, const matrix3x4_t& in2, float *out)
{
	out[0] = DotProducttt(in1, in2[0]) + in2[0][3];
	out[1] = DotProducttt(in1, in2[1]) + in2[1][3];
	out[2] = DotProducttt(in1, in2[2]) + in2[2][3];
}
void VectorTransformmm(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorTransformmm(&in1.x, in2, &out.x);
}
void VectorTransformmm(const float *in1, const VMatrix& in2, float *out)
{
	out[0] = DotProducttt(in1, in2[0]) + in2[0][3];
	out[1] = DotProducttt(in1, in2[1]) + in2[1][3];
	out[2] = DotProducttt(in1, in2[2]) + in2[2][3];
}
void VectorTransformmm(const Vector& in1, const VMatrix& in2, Vector& out)
{
	VectorTransformmm(&in1.x, in2, &out.x);
}

mstudiobbox_t* get_hitbox(CBaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = Interfaces.g_pModelInfo->GetStudioModel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}

Vector get_hitbox_pos(CBaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitbox(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);



	Vector bbmin, bbmax;
	VectorTransformmm(hitbox->bbmin, bone_matrix, bbmin);
	VectorTransformmm(hitbox->bbmax, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}

void TexturedPolygon(int n, std::vector<Vertex_Fov> vertice, CColor color)
{
	static int texture_id = Interfaces.pSurface->CreateNewTextureID(true); // 
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	Interfaces.pSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1); //
	Interfaces.pSurface->DrawSetColor1(color); //
	Interfaces.pSurface->DrawSetTexture(texture_id); //
	Interfaces.pSurface->DrawTexturedPolygon2(n, vertice.data()); //
}

static float dormantAlpha[64];

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif


#ifndef DEG2RADD
#define DEG2RADD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif
class Esp
{
public:

	static void SetHitmarkerValues(float hurttime, int health, int victim)
	{
		flHurtTime = hurttime;
		iHealth = health;
		iVictim = victim;
	}

	static void hitmarker()
	{
		if (!Vars.Visuals.Other.Hitmarker)
			return;

		static int lineSize = 4;

		static float alpha = 0;
		float step = 255.f / 0.3f * Interfaces.pGlobalVars->frametime;

		if (flHurtTime + 0.4f >= Interfaces.pGlobalVars->curtime)
			alpha = 255.f;
		else
		{
			if (alpha != 0)
				alpha -= 1;
		}

		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		Interfaces.pEngine->GetScreenSize(screenSizeX, screenSizeY);

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;

		CColor col = CColor(255, 255, 255, alpha);

		DrawLine(screenCenterX - lineSize * 2, screenCenterY - lineSize * 2, screenCenterX - (1), screenCenterY - (1), col);
		DrawLine(screenCenterX - lineSize * 2, screenCenterY + lineSize * 2, screenCenterX - (1), screenCenterY + (1), col);
		DrawLine(screenCenterX + lineSize * 2, screenCenterY + lineSize * 2, screenCenterX + (2), screenCenterY + (2), col);
		DrawLine(screenCenterX + lineSize * 2, screenCenterY - lineSize * 2, screenCenterX + (2), screenCenterY - (2), col);
	}

	static void ThirdPerson()
	{
		static bool enabledtp = false, check = false;

		if (GetAsyncKeyState(Vars.Visuals.Other.ThirdPersonKey) && Hacks.LocalPlayer->isAlive())
		{
			if (!check)
				enabledtp = !enabledtp;
			check = true;
		}
		else
			check = false;

		if (enabledtp)
		{
			*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(Hacks.LocalPlayer) + 0x31D8) = Hacks.LastAngles;
		}
		auto local_player = Hacks.LocalPlayer;

		auto GetCorrectDistance = [&local_player](float ideal_distance) -> float
		{
			Vector inverse_angles;
			Interfaces.pEngine->GetViewAngles(inverse_angles);

			inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

			Vector direction;
			g_Math.angleVectors(inverse_angles, direction);

			CTraceWorldOnly filter;
			trace_t trace;
			Ray_t ray;

			ray.Init(local_player->GetVecOrigin() + local_player->GetVecViewOffset(), (local_player->GetVecOrigin() + local_player->GetVecViewOffset()) + (direction * (ideal_distance + 5.f)));
			Interfaces.pTrace->TraceRay(ray, MASK_ALL, &filter, &trace);

			return ideal_distance * trace.fraction;
		};

		if (enabledtp)
		{
			if (!Interfaces.pInput->m_fCameraInThirdPerson)
			{
				Interfaces.pInput->m_fCameraInThirdPerson = true;
				Interfaces.pInput->m_vecCameraOffset = Vector(Hacks.LastAngles.x, Hacks.LastAngles.y, GetCorrectDistance(100));
				Vector camForward;

				g_Math.angleVectors(Vector(Interfaces.pInput->m_vecCameraOffset.x, Interfaces.pInput->m_vecCameraOffset.y, 0), camForward);
			}
		}
		else
		{
			Interfaces.pInput->m_fCameraInThirdPerson = false;
			Interfaces.pInput->m_vecCameraOffset = Vector(Hacks.LastAngles.x, Hacks.LastAngles.y, 0);
		}
	}

	static void Crosshair()
	{
		int x, y, MidX, MidY;
		Interfaces.pEngine->GetScreenSize(x, y);

		MidX = x / 2;
		MidY = y / 2;

		Interfaces.pSurface->DrawSetColor1(CColor(255, 255, 255, 255));
		Interfaces.pSurface->DrawOutlinedRect(MidX - 2, MidY - 2, 5, 5);
	}

	static void DrawInfo( RECT rect, CBaseEntity* pPlayer )
	{
		player_info_t info;
		static class Text
		{
		public:
			string text;
			int side;
			int Font;

			Text( string text, int side, int Font ) : text( text ), side( side ), Font( Font )
			{
			}
		};
		std::vector< Text > texts;
		if( pPlayer->IsHostage() )
		{
			if( Vars.Visuals.Player.Hostage)
				texts.push_back( Text( "Hostage", 2, Hacks.Font_ESP ) );
		}
		else if( Interfaces.pEngine->GetPlayerInfo( pPlayer->GetIndex(), &info ) )
		{
			if( Vars.Visuals.Player.Name )
				texts.push_back( Text( pPlayer->GetName(), 0, Hacks.Font_ESP ) );

			CBaseCombatWeapon* weapon = pPlayer->GetActiveBaseCombatWeapon();
			if( weapon )
			{
				if(Vars.Visuals.Player.Weapon && weapon )
					texts.push_back( Text( weapon->GetGunName(), 2, Hacks.Font_Controls) );
				if( Vars.Visuals.Player.Armor )
					texts.push_back( Text( pPlayer->GetArmorIcon(), 1, Hacks.Font_Controls) );
			}
			//delete[]weapon;
		}
		else if( pPlayer->IsWeapon() )
		{
			CBaseCombatWeapon* weapon = static_cast< CBaseCombatWeapon* >( pPlayer );

			if( weapon )
				texts.push_back( Text( ( char* )weapon->GetGunName(), 0, Hacks.Font_ESP) );
			int iClip = weapon->ammo();
			string s = iClip >= 0 && iClip <= 200 && weapon->IsGun() ? std::to_string(iClip) : "";
			if( weapon )
				texts.push_back( Text( strdup( ( s/* + "/" + std::to_string ( weapon->ammo2 () )*/ ).c_str() ), 2, Hacks.Font_ESP ) );
			//delete[]weapon;
		}

		int middle = ( ( rect.right - rect.left ) / 2 ) + rect.left;
		int Top[3] = { rect.top,rect.top, rect.bottom };
		for( auto text : texts )
		{
			RECT nameSize = GetTextSize( text.Font, ( char* )text.text.c_str() );
			switch( text.side )
			{
				case 0:
					Top[ 0 ] -= nameSize.bottom + 2;
					Interfaces.pSurface->DrawT( middle - ( nameSize.right / 2 ), Top[ 0 ], CColor( 255, 255, 255, dormantAlpha[pPlayer->GetIndex()]), text.Font, false, ( char* )text.text.c_str() );
					break;
				case 1:
					Interfaces.pSurface->DrawT( rect.right + 2, Top[ 1 ], CColor( 255, 255, 255, dormantAlpha[pPlayer->GetIndex()] / 2), text.Font, false, ( char* )text.text.c_str() );
					Top[ 1 ] += nameSize.bottom + 3;
					break;
				case 2:
					Interfaces.pSurface->DrawT( middle - ( nameSize.right / 2 ), Top[ 2 ], CColor( 255, 255, 255, dormantAlpha[pPlayer->GetIndex()] / 2), text.Font, false, ( char* )text.text.c_str() );
					Top[ 2 ] += nameSize.bottom + 2;
					break;
			}
		}
	}

	static void NightMode()
	{

		static std::string old_Skyname = "";
		static bool OldNightmode;
		static int OldSky;
		if (!Hacks.LocalPlayer || !Interfaces.pEngine->IsConnected() || !Interfaces.pEngine->IsInGame())
		{
			old_Skyname = "";
			OldNightmode = false;
			OldSky = 0;
			return;
		}
		static ConVar*r_DrawSpecificStaticProp;
		if (OldNightmode != Vars.Visuals.Other.Nightmode)
		{
				if (!r_DrawSpecificStaticProp)
					r_DrawSpecificStaticProp = Interfaces.g_ICVars->FindVar("r_DrawSpecificStaticProp");
				r_DrawSpecificStaticProp->SetValue(0);
			for (MaterialHandle_t i = Interfaces.pMaterialSystem->FirstMaterial(); i != Interfaces.pMaterialSystem->InvalidMaterial(); i = Interfaces.pMaterialSystem->NextMaterial(i))
			{
				IMaterial* pMaterial = Interfaces.pMaterialSystem->GetMaterial(i);
				if (!pMaterial)
					continue;
				if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp") || strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
				{
					if (Vars.Visuals.Other.Nightmode)
					{
						if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
							pMaterial->ColorModulate(0.3f, 0.3f, 0.3f);
						else
							pMaterial->ColorModulate(0.2f, 0.2f, 0.2f);

						if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
						{
							pMaterial->ColorModulate(0.5, 0.1, 0.4);

						}
					}
					else
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
				}
			}
			OldNightmode = Vars.Visuals.Other.Nightmode;
		}
	}

	static void DoEsp()
	{
		hitmarker();
		if (Vars.Visuals.Other.Crosshair)
		{
			Crosshair();
		}

		int width = 0;
		int height = 0;
		Interfaces.pEngine->GetScreenSize( width, height );
		for (auto i = 0; i <= Interfaces.pEntList->GetHighestEntityIndex(); i++)
		{
			CBaseEntity* pEntity = Interfaces.pEntList->GetClientEntity(i);
			if (pEntity == nullptr)
				continue;
			if (pEntity == Hacks.LocalPlayer)
				continue;
		/*	if (pEntity->IsDormant())
				continue;*/

			player_info_t info;
			if (Interfaces.pEngine->GetPlayerInfo(pEntity->GetIndex(), &info))
			{
				CColor clr = CColor(Vars.Visuals.Colors.Box, Vars.Visuals.Colors.Box[3] * dormantAlpha[pEntity->GetIndex()]);
				CColor clr2 = CColor(Vars.Visuals.Colors.SnapLines, Vars.Visuals.Colors.SnapLines[3] * dormantAlpha[pEntity->GetIndex()]);

				if (pEntity->GetTeam() == Hacks.LocalPlayer->GetTeam())
					continue;
				if (!pEntity->isAlive())
					continue;

				if (pEntity->IsDormant() && dormantAlpha[pEntity->GetIndex()] != 0)
				{
					if (dormantAlpha[pEntity->GetIndex()] < 0.f)
						dormantAlpha[pEntity->GetIndex()] = 0.f;
					else
						dormantAlpha[pEntity->GetIndex()] -= 3.f;
				}
				else if (!pEntity->IsDormant() && dormantAlpha[pEntity->GetIndex()] != 255)
					dormantAlpha[pEntity->GetIndex()] += 3.f;

				bool PVS = false;
				RECT rect = DynamicBox(pEntity, PVS);

				DrawInfo(rect, pEntity);

				if (Vars.Visuals.Player.Box)
					DrawBox(rect, clr, pEntity);
				if (Vars.Visuals.Player.SnapLines)
					DrawSnapLine(Vector(rect.left + ((rect.right - rect.left) / 2), rect.bottom, 0), clr2);
				if (Vars.Visuals.Player.Skeleton)
					Skeleton(pEntity);
				if (Vars.Visuals.Player.Health)
					DrawHealth(rect, pEntity);
				if (Vars.Visuals.Other.Radar)
					*pEntity->IsSpottedPtr() = true;
				if (Vars.Visuals.Other.FovArrows_Enable)
					DrawFovArrows(pEntity);
				//if ( Settings.GetSetting( Tab_Visuals, Visuals_Dlights ) ) DLight(pEntity, clr);
			}
			if (Vars.Visuals.DroppedWeapons && pEntity->IsWeapon())
			{
				auto weapon = static_cast<CBaseCombatWeapon*>(pEntity);
				auto plr = Interfaces.pEntList->GetClientEntityFromHandle((HANDLE)weapon->GetOwnerHandle());
				Vector vOrig3D = pEntity->GetAbsOrigin();
				Vector vOrig;
				Interfaces.g_pDebugOverlay->ScreenPosition(vOrig3D, vOrig);
				player_info_t info;
				if (plr == nullptr || plr->isValidPlayer() || !plr->isAlive() || !(plr->GetHealth() > 0) || !(Interfaces.pEngine->GetPlayerInfo(plr->GetIndex(), &info)))
				{
					if (!(vOrig3D.y >= -5 && vOrig3D.y <= 5 && vOrig3D.x >= -5 && vOrig3D.x <= 5))
					{
						bool k = false;
						if (*weapon->GetItemDefinitionIndex() != WEAPON_C4)
						{
							RECT rect = DynamicBox(pEntity, k);
							//DrawBox(rect, CColor(255, 255, 255, 255));
							DrawInfo(rect, pEntity);
						}
						else
						{
							bool k = false;
							Interfaces.pSurface->DrawT(vOrig.x, vOrig.y - 5, CColor(255, 255, 255, 255), 0xA1, true, "C4");
						}
					}
				}
				//delete[]weapon;
			}



			if( Vars.Visuals.C4 && strstr( pEntity->GetClientClass()->m_pNetworkName, "CPlantedC4" ) )
			{
				if( !pEntity->BombDefused() )
				{
					float flTime = pEntity->BombTimer() - Interfaces.pGlobalVars->curtime;
					if( flTime > 0.f )
					{
						Vector vPos3D = pEntity->GetAbsOrigin();
						Vector vPos;
						Interfaces.g_pDebugOverlay->ScreenPosition( vPos3D, vPos );

						bool bCanDefuse = ( Hacks.LocalPlayer->HasKit() && flTime > 5.5f ) || flTime > 10.5f;
						Interfaces.pSurface->DrawT( vPos.x, vPos.y - 5, CColor( 255, 255, 255, 255 ), 0xA1, false, "C4" );
						Interfaces.pSurface->DrawT( vPos.x + 25, vPos.y, bCanDefuse ? CColor( 255, 255, 255, 255 ) : CColor( 255, 0, 0, 255 ), Hacks.Font_ESP, false, "%.2f", flTime );

						if( Hacks.LocalPlayer->GetHealth() > 0 )
						{
							float flArmor = Hacks.LocalPlayer->GetArmor();
							float flDistance = Hacks.LocalPlayer->GetAbsOrigin().DistTo( pEntity->GetAbsOrigin() );
							float a = 450.7f;
							float b = 75.68f;
							float c = 789.2f;
							float d = ( ( flDistance - b ) / c );

							float flDmg = a * exp( -d * d );
							float flDamage = flDmg;

							if( flArmor > 0 )
							{
								float flNew = flDmg * 0.5f;
								float flArmor = ( flDmg - flNew ) * 0.5f;

								if( flArmor > static_cast< float >( flArmor ) )
								{
									flArmor = static_cast< float >( flArmor ) * ( 1.f / 0.5f );
									flNew = flDmg - flArmor;
								}

								flDamage = flNew;
							}

							int x, y;
							Interfaces.pEngine->GetScreenSize( x, y );

							if( flDamage > 1 )
								Interfaces.pSurface->DrawT( x / 2, y * 0.1, flDamage >= Hacks.LocalPlayer->GetHealth() ? CColor( 255, 0, 0, 255 ) : CColor( 255, 255, 255, 255 ), Hacks.Font_Watermark, true, "THE BOMB WILL EXPLODE IN %.1f SECONDS DEALING %.f DAMAGE ", flTime, flDamage );
						}
					}
				}
			}
			//delete[]pEntity;
		}
	}

	static void WorldESP( CBaseEntity* pEntity )
	{
	}

	static RECT DynamicBox( CBaseEntity* pPlayer, bool& PVS )
	{
		Vector trans = pPlayer->GetVecOrigin();

		Vector min;
		Vector max;

		min = pPlayer->BBMin();
		max = pPlayer->BBMax();

		Vector pointList[] = {
			Vector( min.x, min.y, min.z ),
			Vector( min.x, max.y, min.z ),
			Vector( max.x, max.y, min.z ),
			Vector( max.x, min.y, min.z ),
			Vector( max.x, max.y, max.z ),
			Vector( min.x, max.y, max.z ),
			Vector( min.x, min.y, max.z ),
			Vector( max.x, min.y, max.z )
		};

		Vector Distance = pointList[ 0 ] - pointList[ 1 ];
		int dst = Distance.Length();
		dst /= 1.3f;
		Vector angs;
		Misc::CalcAngle( trans, Hacks.LocalPlayer->GetEyePosition(), angs );

		Vector all[8];
		angs.y += 45;
		for( int i = 0; i < 4; i++ )
		{
			g_Math.angleVectors( angs, all[ i ] );
			all[ i ] *= dst;
			all[ i + 4 ] = all[ i ];
			all[ i ].z = max.z;
			all[ i + 4 ].z = min.z;
			VectorAdd( all[ i ], trans, all[ i ] );
			VectorAdd( all[ i + 4 ], trans, all[ i + 4 ] );
			angs.y += 90;
		}

		Vector flb, brt, blb, frt, frb, brb, blt, flt;
		PVS = true;

		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 3 ], flb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 0 ], blb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 2 ], frb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 6 ], blt ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 5 ], brt ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 4 ], frt ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 1 ], brb ) )
			PVS = false;
		if( !Interfaces.g_pDebugOverlay->ScreenPosition( all[ 7 ], flt ) )
			PVS = false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for( int i = 0; i < 8; i++ )
		{
			if( left > arr[ i ].x )
				left = arr[ i ].x;
			if( top > arr[ i ].y )
				top = arr[ i ].y;
			if( right < arr[ i ].x )
				right = arr[ i ].x;
			if( bottom < arr[ i ].y )
				bottom = arr[ i ].y;
		}
		RECT rect;
		rect.left = left;
		rect.bottom = bottom;
		rect.right = right;
		rect.top = top;
		return rect;
	}

	

	static 	bool IsOnScreen(Vector origin, Vector& screen)
	{
		if (!g_Math.WorldToScreen(origin, screen)) return false;
		int iScreenWidth, iScreenHeight;
		Interfaces.pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
		bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
		return xOk && yOk;
	}
	
	static void DrawFovArrows(CBaseEntity* entity)
	{
		auto local_player = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());

		if (!local_player)
			return;

		//if (entity->IsDormant())
		//	return;

		if (!Vars.Visuals.Other.FovArrows_Enable) //yes?
			return;

		Vector screenPos;
		if (IsOnScreen(get_hitbox_pos(entity, (int)CSGOHitboxID::Pelvis), screenPos)) return;

		auto client_viewangles = Vector();
		auto screen_width = 0, screen_height = 0;

		Interfaces.pEngine->GetViewAngles(client_viewangles);
		Interfaces.pEngine->GetScreenSize(screen_width, screen_height);

		auto radius = 450.f;
		Vector local_position = local_player->GetVecOrigin() + local_player->GetVecViewOffset();

		const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
		const auto rot = DEG2RADD(client_viewangles.y - g_Math.CalcAngle(local_position, get_hitbox_pos(entity, (int)CSGOHitboxID::Pelvis)).y - 90);

		std::vector<Vertex_Fov> Vertsss;

		Vertsss.push_back(Vertex_Fov(Vector2DDD(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
		Vertsss.push_back(Vertex_Fov(Vector2DDD(screen_center.x + cosf(rot + DEG2RADD(2)) * (radius - 20), screen_center.y + sinf(rot + DEG2RADD(2)) * (radius - 20))));
		Vertsss.push_back(Vertex_Fov(Vector2DDD(screen_center.x + cosf(rot - DEG2RADD(2)) * (radius - 20), screen_center.y + sinf(rot - DEG2RADD(2)) * (radius - 20))));

		static auto alpha = 0.f; static auto plus_or_minus = false;
		if (alpha <= 0.f || alpha >= dormantAlpha[entity->GetIndex()]) plus_or_minus = !plus_or_minus;
		alpha += plus_or_minus ? (dormantAlpha[entity->GetIndex()] / 7 * 0.015) : -(dormantAlpha[entity->GetIndex()] / 7 * 0.015); alpha = clamp_value(alpha, 0.f, dormantAlpha[entity->GetIndex()]);

		TexturedPolygon(3, Vertsss, CColor(Vars.Visuals.Colors.FovArrows, Vars.Visuals.Colors.FovArrows[3] * alpha)); //255, 40, 230
	}

	static void Skeleton( CBaseEntity* pEntity )
	{
		CColor col = CColor(Vars.Visuals.Colors.Skeleton, Vars.Visuals.Colors.Skeleton[3] * dormantAlpha[pEntity->GetIndex()]);
		if (Vars.Visuals.Player.Skeleton)
		{
			studiohdr_t* pStudioHdr = Interfaces.g_pModelInfo->GetStudioModel(pEntity->GetModel());

			if (!pStudioHdr)
				return;

			Vector vParent, vChild, sParent, sChild;

			for (int j = 0; j < pStudioHdr->numbones; j++)
			{
				mstudiobone_t* pBone = pStudioHdr->GetBone(j);

				if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
				{
					vChild = pEntity->GetBonePos(j);
					vParent = pEntity->GetBonePos(pBone->parent);
					Interfaces.g_pDebugOverlay->ScreenPosition(vParent, sParent);
					Interfaces.g_pDebugOverlay->ScreenPosition(vChild, sChild);
					int red = 0;
					int green = 0;
					int blue = 0;
					int alpha = 0;
					col.GetColor(red, green, blue, alpha);
					Interfaces.pSurface->DrawSetColor(red, green, blue, alpha);
					Interfaces.pSurface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
				}
				//delete[]pBone;
			}
		}
	}

	static void DrawSnapLine( Vector to, CColor clr )
	{
		int width = 0;
		int height = 0;
		Interfaces.pEngine->GetScreenSize( width, height );
		Vector From( ( width / 2 ), height - 1, 0 );
		DrawLine( From.x, From.y, to.x, to.y, clr );
	}

	static void DrawBox( RECT rect, CColor Color, CBaseEntity* pPlayer)
	{
		DrawOutlinedRect( rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1, CColor( 0, 0, 0, dormantAlpha[pPlayer->GetIndex()] / 2 ) );
		DrawOutlinedRect( rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1, CColor( 0, 0, 0, dormantAlpha[pPlayer->GetIndex()] / 2) );
		DrawOutlinedRect( rect.left, rect.top, rect.right, rect.bottom, Color );
	}

	static void DrawHealth( RECT rect, CBaseEntity* pPlayer )
	{
		float HealthValue = pPlayer->GetHealth();
		int iHealthValue = HealthValue;
		int Red = 255 - (HealthValue * 2.00);
		int Green = HealthValue * 2.00;
		float flBoxes = std::ceil(pPlayer->GetHealth() / 10.f);

		float height = (rect.bottom - rect.top) * (HealthValue / 100);
		float height2 = (rect.bottom - rect.top) * (100 / 100); // used for the styles like healthbar lines
		float flHeight = height2 / 10.f;

		DrawRect(rect.left - 5, rect.top - 1, rect.left - 1, rect.bottom + 1, CColor(0, 0, 0, dormantAlpha[pPlayer->GetIndex()] / 2));
		DrawRect(rect.left - 4, rect.bottom - height, rect.left - 2, rect.bottom, CColor(Red, Green, 0, dormantAlpha[pPlayer->GetIndex()]));

		//float HealthValue = pPlayer->GetHealth();
		//int Red = 255 - ( HealthValue * 2.55 );
		//int Green = HealthValue * 2.55;
		//float height = ( rect.bottom - rect.top ) * ( HealthValue / 100 );
		//DrawRect( rect.left - 4, rect.top - 2, rect.left - 2, rect.bottom + 2, CColor( 0, 0, 0, dormantAlpha[pPlayer->GetIndex()] / 2) );
		//DrawRect( rect.left - 4, rect.bottom - height, rect.left - 3, rect.bottom, CColor( Red, Green, 0, dormantAlpha[pPlayer->GetIndex()]) );
	}

	static void ArmorBar( Vector bot, Vector top, float armor, int off )
	{
		float h = ( bot.y - top.y );
		float offset = ( h / 4.f ) + 2 + off * 6;

		UINT hp = h - ( UINT )( ( h * armor ) / 100 );

		Interfaces.pSurface->DrawSetColor( 0, 0, 0, 150 );
		Interfaces.pSurface->DrawOutlinedRect( ( top.x - offset ) - 1, top.y - 1, ( top.x - offset ) - 1 + 5, top.y - 1 + h + 2 );
		Interfaces.pSurface->DrawSetColor( 30, 144, 255, 255 );
		Interfaces.pSurface->DrawFilledRect( ( top.x - offset ), top.y + hp, ( top.x - offset ) + 3, top.y + hp + h - hp + 0 );
	}

	static void IWBox( Vector vTopScreen, Vector vScreen, float h, float w, CColor clr )
	{
	}

	static void DLight( CBaseEntity* pEntity, CColor clr )
	{
		dlight_t* pDlight = Interfaces.g_pEffects->CL_AllocDlight( pEntity->GetIndex() );

		pDlight->origin = pEntity->GetAbsOrigin();
		pDlight->radius = 250.f;
		pDlight->color = clr;
		pDlight->die = Interfaces.pGlobalVars->curtime + 0.1f;
		pDlight->decay = 40.f;
		//delete[]pDlight;
	}

	static void GlowESP() {
		if (Interfaces.pEngine->IsConnected() && Interfaces.pEngine->IsInGame()) {
			for (auto i = 0; i < Interfaces.g_pGlowObjectManager->size; i++) {
				auto glow_object = &Interfaces.g_pGlowObjectManager->m_GlowObjectDefinitions[i];

				CBaseEntity *m_entity = glow_object->m_pEntity;

				if (!glow_object->m_pEntity || glow_object->IsUnused())
					continue;

				if (m_entity->GetClientClass()->m_ClassID == 38)
				{
					if (m_entity->GetTeam() == Hacks.LocalPlayer->GetTeam() && m_entity != Hacks.LocalPlayer)
						continue;

					if (m_entity != Hacks.LocalPlayer && Vars.Visuals.Player.Glow)
					{
						glow_object->m_vGlowColor = Vector(Vars.Visuals.Colors.Glow[0], Vars.Visuals.Colors.Glow[1], Vars.Visuals.Colors.Glow[2]);
						glow_object->m_flGlowAlpha = Vars.Visuals.Colors.Glow[3];

						glow_object->m_bRenderWhenOccluded = true;
						glow_object->m_bRenderWhenUnoccluded = false;
					}

					if (m_entity == Hacks.LocalPlayer && Vars.Visuals.Player.LocalGlow)
					{
						float Red = Vars.Visuals.Colors.LocalGlow[0];
						float Green = Vars.Visuals.Colors.LocalGlow[1];
						float Blue = Vars.Visuals.Colors.LocalGlow[2];
						float A = Vars.Visuals.Colors.LocalGlow[3];

						if (Vars.Visuals.Player.PulseLocalGlow) {
							glow_object->m_bPulsatingChams = true;
						}
						glow_object->m_vGlowColor = Vector(Red, Green, Blue);
						glow_object->m_flGlowAlpha = A;

						glow_object->m_bRenderWhenOccluded = true;
						glow_object->m_bRenderWhenUnoccluded = false;
					}
				}
			}
		}
	}
};
