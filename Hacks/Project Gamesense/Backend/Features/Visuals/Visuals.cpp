#include "Visuals.h"
#include "Drawing.h"
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/Collideable.h"
#include "../../SDK/player_info_t.h"
#include "../../SDK/CBaseWeapon.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/handle.h"
#include "../Ragebot/c_playerlist.h"
#include "../Ragebot/CAutoWall.h"
#include "../../Hooking/KeyHandler.h"
#include <cwctype>
#include "..\\..\\..\\Frontend\Rendering\Render.h"
float multiplier = 1;

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

using namespace Features;
using namespace Graphics;

CVisuals* Features::Visuals = new CVisuals();

bool IsOnScreen(Vector origin, Vector &screen);
Vector get_hitbox_pos(CBaseEntity* entity, int hitbox_id);

bool GetBox( CBaseEntity* m_entity, EntityBoxInfo_t &box, bool dynamic )
{
	if ( !m_entity )
		return false;

	const VMatrix &trans = m_entity->GetCollisionBoundTrans();

	Vector min, max;
	m_entity->GetRenderBounds(min, max);

	if ( dynamic )
	{
		Vector points[] = { Vector( min.x, min.y, min.z ),
		        Vector( min.x, max.y, min.z ),
		        Vector( max.x, max.y, min.z ),
		        Vector( max.x, min.y, min.z ),
		        Vector( max.x, max.y, max.z ),
		        Vector( min.x, max.y, max.z ),
		        Vector( min.x, min.y, max.z ),
		        Vector( max.x, min.y, max.z )
		    };
		auto vector_transform = []( const Vector in1, const VMatrix & in2 )
		{
			auto dot_product = []( const Vector & v1, const float * v2 )
			{
				float ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				return ret;
			};
			auto out = Vector();
			out[0] = dot_product( in1, in2[0] ) + in2[0][3];
			out[1] = dot_product( in1, in2[1] ) + in2[1][3];
			out[2] = dot_product( in1, in2[2] ) + in2[2][3];
			return out;
		};
		Vector pointsTransformed[8];

		for ( int i = 0; i < 8; i++ )
			pointsTransformed[i] = vector_transform( points[i], trans );

		Vector pos = m_entity->GetAbsOrigin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;

		if ( !Utilities->Game_WorldToScreen( pointsTransformed[3], flb ) || !Utilities->Game_WorldToScreen( pointsTransformed[5], brt )
		    || !Utilities->Game_WorldToScreen( pointsTransformed[0], blb ) || !Utilities->Game_WorldToScreen( pointsTransformed[4], frt )
		    || !Utilities->Game_WorldToScreen( pointsTransformed[2], frb ) || !Utilities->Game_WorldToScreen( pointsTransformed[1], brb )
		    || !Utilities->Game_WorldToScreen( pointsTransformed[6], blt ) || !Utilities->Game_WorldToScreen( pointsTransformed[7], flt ) )
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for ( int i = 1; i < 8; i++ )
		{
			if ( left > arr[i].x )
				left = arr[i].x;

			if ( top < arr[i].y )
				top = arr[i].y;

			if ( right < arr[i].x )
				right = arr[i].x;

			if ( bottom > arr[i].y )
				bottom = arr[i].y;
		}

		Vector BotCenter = Vector( right - ( ( right - left ) / 2 ), top, 0 );
		Vector TopCenter = Vector( right - ( ( right - left ) / 2 ), bottom, 0 );
		box.x = left;
		box.y = TopCenter.y;
		box.w = right - left;
		box.h = BotCenter.y - TopCenter.y;
	}

	else
	{
		Vector pos, pos3D, top, top3D;
		pos3D = m_entity->GetAbsOrigin();
		top3D = pos3D + Vector( 0, 0, max.z );

		if ( Utilities->Game_WorldToScreen( pos3D, pos ) && Utilities->Game_WorldToScreen( top3D, top ) )
		{
			int height = ( pos.y - top.y );
			int width = height / 2;
			box.x = ( pos.x - width / 2 );
			box.y = top.y;
			box.w = width;
			box.h = height;
		}
	}

	box.pEnt = m_entity;
	return true;
}

void CVisuals::Run()
{
	if ( !Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame() || !G::LocalPlayer )
		return;

	if ( c_config::get()->b["vis_hitmarker"] && hitmarkerAlpha != 0 )
	{
		int w, h = 0;
		Interfaces::Engine->GetScreenSize( w, h );
		int screenCenterX, screenCenterY = 0;
		screenCenterX = w / 2;
		screenCenterY = h / 2;
		Graphics::Drawing->Line(screenCenterX - 4 * 2 + 1, screenCenterY - 4 * 2 + 1, screenCenterX - (4) + 1, screenCenterY - (4) + 1, CColor( 255, 255, 255, hitmarkerAlpha ) );
		Graphics::Drawing->Line(screenCenterX - 4 * 2 + 1, screenCenterY + 4 * 2 + 1, screenCenterX - (4) + 1, screenCenterY + (4) + 1, CColor( 255, 255, 255, hitmarkerAlpha ) );
		Graphics::Drawing->Line(screenCenterX + 4 * 2 + 1, screenCenterY + 4 * 2 + 1, screenCenterX + (4 + 1) + 1, screenCenterY + (4 + 1) + 1, CColor( 255, 255, 255, hitmarkerAlpha ) );
		Graphics::Drawing->Line(screenCenterX + 4 * 2 + 1, screenCenterY - 4 * 2 + 1, screenCenterX + (4 + 1) + 1, screenCenterY - (4 + 1) + 1, CColor( 255, 255, 255, hitmarkerAlpha ) );

		Graphics::Drawing->Line(screenCenterX - 4 * 2, screenCenterY - 4 * 2, screenCenterX - (4), screenCenterY - (4), CColor(0, 0, 0, hitmarkerAlpha));
		Graphics::Drawing->Line(screenCenterX - 4 * 2, screenCenterY + 4 * 2, screenCenterX - (4), screenCenterY + (4), CColor(0, 0, 0, hitmarkerAlpha));
		Graphics::Drawing->Line(screenCenterX + 4 * 2, screenCenterY + 4 * 2, screenCenterX + (4 + 1), screenCenterY + (4 + 1), CColor(0, 0, 0, hitmarkerAlpha));
		Graphics::Drawing->Line(screenCenterX + 4 * 2, screenCenterY - 4 * 2, screenCenterX + (4 + 1), screenCenterY - (4 + 1), CColor(0, 0, 0, hitmarkerAlpha));
		auto fade = ( ( 1.f / 0.8f ) * Interfaces::Globals->frametime ) * 255;

		if ( hitmarkerAlpha > 0 )
			hitmarkerAlpha -= fade;

		if ( hitmarkerAlpha < 0 )
			hitmarkerAlpha = 0;
	}
	DrawPlayers();
	DrawIndicators();
}
void CVisuals::RunHitmarker()
{
	hitmarkerAlpha = 255;
}
const wchar_t* to_wchar(const char* string) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, string);
	_vsnprintf_s(buf, sizeof(buf), string, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	return wbuf;
}
void CVisuals::DrawCircle(int x, int y) {

}
void CVisuals::RunSpectators()
{
	std::vector<std::string> specs;
	int w, h = 0;

	if ( Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && G::LocalPlayer && G::LocalPlayer->IsAlive() && c_config::get()->b["vis_spectators"] )
	{
		Interfaces::Engine->GetScreenSize( w, h );

		for ( int i = 0; i < 64; i++ )
		{
			CBaseEntity* pEnt = Interfaces::EntityList->GetClientEntity( i );

			if ( !pEnt || !pEnt->IsPlayer() || pEnt == G::LocalPlayer || pEnt->GetHealth() > 0 || pEnt->IsDormant() || pEnt->GetObserverTarget() != G::LocalPlayer )
				continue;

			player_info_t pInfo;
			Interfaces::Engine->GetPlayerInfo( i, &pInfo );
			specs.push_back( pInfo.szName);
		}

		for ( int i = 0; i < specs.size(); i++ )
		{
			int xOffset = Drawing->TextSize( specs.at( i ).c_str(), Drawing->fonts.esp_main ).right + 5;
			int y = i * 20;
			Drawing->Text( w - xOffset + 2, y + 6, specs.at( i ).c_str(), Drawing->fonts.esp_main, CColor( 255, 255, 255, 120 ) );
		}
		/*int index = 0;
		int w, h;
		Interfaces::Engine->GetScreenSize(w, h);


		Graphics::Drawing->rect_filled(1627, 460, 209, 109, CColor(71, 71, 71));
		Graphics::Drawing->rect_filled(1627, 460, 209, 15, CColor(66, 170, 255));
		Graphics::Drawing->Text(1645, 458, "Spectators", Drawing->fonts.esp_main, CColor(255, 255, 255));

		for (int i = 1; i < 65; i++) {
			auto p_local = static_cast<CBaseEntity *>(Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex())); // what's csgo::local_player???????
			auto p_entity = static_cast<CBaseEntity *>(Interfaces::EntityList->GetClientEntity(i));
			player_info_t e_info;

			if (p_entity && p_entity != p_local) {
				Interfaces::Engine->GetPlayerInfo(i, &e_info);
				if (!p_entity->IsAlive() && !p_entity->IsDormant()) {
					auto target = p_entity->GetObserverTarget();
					if (target) {
						auto p_target = Interfaces::EntityList->GetClientEntityFromHandle(target);
						if (p_target == p_local) {
							int x, y;
							Interfaces::Surface->GetTextSize(Drawing->fonts.esp_main, to_wchar(e_info.name), x, y);

							std::string player_name = e_info.name;
							player_info_t p_info;
							Interfaces::Engine->GetPlayerInfo(i, &e_info);

							Graphics::Drawing->Text(1645, 475 + (10 * index), player_name.c_str(), Drawing->fonts.esp_main, CColor(255, 255, 255, 255));
							index++;
						}
					}
				}
			}
		}*/
	}

}

void CVisuals::DrawPlayers()
{
	for ( int i = 1; i < Interfaces::EntityList->GetHighestEntityIndex(); i++ )
	{
		EntityBoxInfo_t &box = infos[i - 1];
		auto ent = Interfaces::EntityList->GetClientEntity( i );

		if ( !ent )
			continue;

		if (c_playerlist::get()->should_disable_visuals(ent))
			continue;

		if (!c_keyhandler::get()->auto_check("vis_active_key"))
			continue;

		if ( ent->GetClientClass()->m_ClassID == ( int )ClassID::CCSPlayer )
		{
			box.w = 0;
			box.h = 0;
			box.x = 0;
			box.y = 0;

			Vector dist = G::LocalPlayer->GetOrigin() - ent->GetOrigin();
			float flDist = sqrtf( dist.Length() );

			if ( ent->GetTeam() == G::LocalPlayer->GetTeam() && !c_config::get()->b["vis_teammates"] && ent != G::LocalPlayer )
				continue;

			if ( ent == G::LocalPlayer )
				continue;

			if (ent == G::LocalPlayer->GetObserverTarget())
				continue;

			DrawArrow( ent );

			if ( GetBox( ent, box, false ))
			{
				if ( box.x == G::iScreenWidth / 2 && box.y == G::iScreenHeight / 2 && box.x + box.w > G::iScreenWidth - 1 && box.y + box.h > G::iScreenHeight - 1 )
					continue;

				if (box.x <= 0 || box.y <= 0)
					continue;

				if ( box.hp == -1 )
					box.hp = std::clamp( ent->GetHealth(), 0, 100 );

				else
				{
					int hp = std::clamp( ent->GetHealth(), 0, 100 );

					if ( hp != box.hp )
					{
						if ( box.hp > hp )
						{
							if ( box.hpDiffTime == 0.f )
								box.hpDifference = box.hp - hp;

							else
								box.hpDifference += box.hp - hp;

							box.hpDiffTime = Interfaces::Globals->curtime;
						}

						else
						{
							box.hpDifference = 0;
							box.hpDiffTime = 0.f;
						}

						box.hp = hp;
					}

					if ( Interfaces::Globals->curtime - box.hpDiffTime > 0.6f && box.hpDifference != 0 )
					{
						float diffFactor = ( ( 1.f / 0.5f ) * Interfaces::Globals->frametime ) * ( box.hpDifference * 2 );
						box.hpDifference -= diffFactor;
						box.hpDifference = std::clamp( box.hpDifference, 0, 100 );

						if ( box.hpDifference == 0 )
							box.hpDiffTime = 0.f;
					}
				}

				if ( ent->IsDormant() || !ent->IsAlive() )
				{
					if ( box.alpha_anim > 0 )
					{
						auto fadeNormal = ( ( 1.f / 0.25f ) * Interfaces::Globals->frametime ) * 255;

						if (ent->IsDormant() && c_config::get()->b["vis_dormant"] && ent->IsAlive())
						{
							if (box.dormancy == 0)
								box.dormancy = Interfaces::Globals->curtime;

							if (Interfaces::Globals->curtime - box.dormancy > 5.f)
								box.alpha_anim -= fadeNormal;
						}

						else if (ent->IsAlive())
							box.alpha_anim -= fadeNormal;
						else
							box.alpha_anim = 0;
					}

					else
					{
						box.dormancy = Interfaces::Globals->curtime;
						box.alpha_anim = 0;
					}
				}

				else
					box.alpha_anim = 1275;

				int alpha_anim_unc = box.alpha_anim / 5;
				bool isTeammate = false;

				if ( box.pEnt->GetTeam() == G::LocalPlayer->GetTeam() )
					isTeammate = true;

				int alpha_toc = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_boundingbox_color"]).a();
				int alpha_toc1 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_name_color"]).a();
				int alpha_toc2 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_ammo_color"]).a();
				int alpha_toc3 = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_skeleton_color"]).a();

				box.alpha = std::clamp( alpha_anim_unc, 0, alpha_toc );
				box.alpha1 = std::clamp(alpha_anim_unc, 0, alpha_toc1);
				box.alpha2 = std::clamp(alpha_anim_unc, 0, alpha_toc2);
				box.alpha4 = std::clamp(alpha_anim_unc, 0, alpha_toc3);
				box.alpha3 = std::clamp(alpha_anim_unc, 0, 220);

				if ( c_config::get()->b["vis_boundingbox"])
					DrawBoundingBox( box );

				if ( c_config::get()->b["vis_healthbar"] )
					DrawHealthBar( box );

				if ( c_config::get()->b["vis_name"] )
					DrawName( box );

				if (c_config::get()->b["vis_skeleton"])
					DrawSkeleton(box);

				if ( c_config::get()->b["vis_armor"] || c_config::get()->b["vis_latency"] || c_config::get()->b["vis_flags"] || c_config::get()->b["vis_money"])
					DrawSideInfo( box );

				if ( c_config::get()->b["vis_ammo"] || c_config::get()->b["vis_weapons"] || c_config::get()->b["vis_weaponsicons"] || c_config::get()->b["vis_distance"])
					DrawBottomInfo( box );
			}
		}

		else
		{
			if ( ent->GetClientClass()->m_ClassID == ( int )ClassID::CPlantedC4 && c_config::get()->b["vis_bomb"] )
			{
				DrawBombInterface( ent );
				continue;
			}

			if ((ent->GetClientClass()->m_ClassID == (int)ClassID::CMolotovProjectile || ent->GetClientClass()->m_ClassID == (int)ClassID::CInferno || ent->GetClientClass()->m_ClassID == (int)ClassID::CSmokeGrenadeProjectile
				|| ent->GetClientClass()->m_ClassID == (int)ClassID::CDecoyProjectile || ent->GetClientClass()->m_ClassID == (int)ClassID::CBaseCSGrenadeProjectile) && c_config::get()->b["vis_grenades"])
			{
				DrawProjectiles( ent );
				continue;
			}

			if (!ent->IsWeapon())
				continue;

			auto mdl = Interfaces::ModelInfo->GetModelName(ent->GetModel());

			if (!strstr(mdl, "w_"))
				continue;

			if (GetBox(ent, box, true))
			{
				box.alpha = 255;

				if (c_config::get()->i["vis_droppedwpns"] == 1 || c_config::get()->i["vis_droppedwpns"] == 3)
					DrawWeaponName(box);

				if (c_config::get()->b["vis_droppedwpnsammo"])
					DrawWeaponAmmo(box);
			}
		}
	}
}

Vector get_hitbox_pos(CBaseEntity* entity, int hitbox_id)
{
	auto getHitbox = [](CBaseEntity* entity, int hitboxIndex) -> mstudiobbox_t*
	{
		if (entity->IsDormant() || entity->GetHealth() <= 0)
			return NULL;

		const auto pModel = entity->GetModel();

		if (!pModel)
			return NULL;

		auto pStudioHdr = Interfaces::ModelInfo->GetStudioModel(pModel);

		if (!pStudioHdr)
			return NULL;

		auto pSet = pStudioHdr->pHitboxSet(0);

		if (!pSet)
			return NULL;

		if (hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0)
			return NULL;

		return pSet->GetHitbox(hitboxIndex);
	};
	auto hitbox = getHitbox(entity, hitbox_id);

	if (!hitbox)
		return Vector(0, 0, 0);

	matrix3x4_t matrix[128];
	if (entity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		Vector bbmin, bbmax;
		Utilities->Math_VectorTransform(hitbox->bbmin, matrix[hitbox->bone], bbmin);
		Utilities->Math_VectorTransform(hitbox->bbmax, matrix[hitbox->bone], bbmax);
		return (bbmin + bbmax) / 2;
	}
}

bool IsOnScreen(Vector origin, Vector& screen)
{
	if (!Utilities->Game_WorldToScreen(origin, screen))
		return false;

	int iScreenWidth, iScreenHeight;
	Interfaces::Engine->GetScreenSize(iScreenWidth, iScreenHeight);
	bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
	return xOk && yOk;
}

void CVisuals::DrawArrow(CBaseEntity* pEnt)
{
	if (!c_config::get()->b["vis_outoffovarrows"])
		return;

	if (!G::LocalPlayer || pEnt->IsDormant() || pEnt->IsWeapon() || !pEnt->IsPlayer() || pEnt->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer || pEnt == G::LocalPlayer || !pEnt->IsAlive() || pEnt->GetHealth() <= 0)
		return;

	int alpha = std::clamp(floor(sin(Interfaces::Globals->realtime * 2) * 127 + 128), 0.f, 0.5f * 255.f);
	Vector screenPos, client_viewangles2, client_viewangles;
	int screen_width = 0, screen_height = 0;
	float radius = 50.f + c_config::get()->i["vis_outoffovarrows_dist"];

	if (IsOnScreen(pEnt->GetHitboxPosition(HITBOX_HEAD), screenPos))
		return;

	Interfaces::Engine->GetViewAngles(&client_viewangles);
	Interfaces::Engine->GetScreenSize(screen_width, screen_height);

	// υσι

	const auto scr = Vector2D(screen_width / 2.f, screen_height / 2.f);
	const auto rot = DEG2RAD(client_viewangles.y - Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), get_hitbox_pos(pEnt, HITBOX_HEAD)).y - 90);
	auto pos = scr + Vector2D(radius * cos(rot), radius * sin(rot));
	auto line = pos - scr;
	Vector2D arrowBase = pos - (line * (c_config::get()->i["vis_outoffovarrows_size"] / (2 * (tanf(45) / 2) * line.length())));
	Vector2D normal = Vector2D(-line.y, line.x);
	Vector2D left = arrowBase + normal * (c_config::get()->i["vis_outoffovarrows_size"] / (2 * line.length()));
	Vector2D right = arrowBase + normal * (-c_config::get()->i["vis_outoffovarrows_size"] / (2 * line.length()));
	std::vector<Vertex_t> vertices;
	vertices.push_back(Vertex_t(Vector2D(left.x, left.y)));
	vertices.push_back(Vertex_t(Vector2D(right.x, right.y)));
	vertices.push_back(Vertex_t(pos));
	Graphics::Drawing->Polygon((int)3, vertices.data(), c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_outoffovarrows_color"], alpha / 255.f));
}

int fl2bigint( float fl )
{
	char timerfl[32];
	sprintf( timerfl, "%.2f", fl );
	std::string timerfls = timerfl;
	timerfls.erase( std::remove_if( timerfls.begin(), timerfls.end(), []( char c )
	{
		return c == ',' || c == '.';
	} ) );
	return atoi( timerfls.c_str() );
}

void CVisuals::DrawBombInterface( CBaseEntity* pEnt )
{
	auto bomb = ( CCSBomb* )pEnt;

	if ( !bomb || !bomb->IsBombTicking() || bomb->IsBombDefused() )
		return;

	float blow = bomb->GetBlowTime();
	float remain = blow - ( Interfaces::Globals->interval_per_tick * G::LocalPlayer->GetTickBase() );

	if ( remain < 0 )
		remain = 0;

	float def = bomb->GetDefuseTime();
	float defrem = def - ( Interfaces::Globals->interval_per_tick * G::LocalPlayer->GetTickBase() );

	if ( defrem < 0 )
		defrem = 0;

	int w, h = 0;
	Interfaces::Engine->GetScreenSize( w, h );
	char buffer[32];
	sprintf( buffer, "%s - %.1fs", bomb->GetBombSite() ? "B" : "A", remain );
	char defbuffer[32];
	sprintf( defbuffer, "Defuse: %.0fs", defrem );
	int remainanim, timeranim, defremanim, deftimeanim = 0;
	remainanim = fl2bigint( remain );
	timeranim = fl2bigint( bomb->GetTimerLength() );
	defremanim = fl2bigint( defrem );
	deftimeanim = fl2bigint( bomb->GetDefuseLength() );
	float a = 450.7f;
	float b = 75.68f;
	float c = 789.2f;
	float d = ( ( G::LocalPlayer->GetAbsOrigin().DistTo( pEnt->GetOrigin() ) - b ) / c );
	float damage = a * expf( -d * d );
	auto GetArmourHealth = []( float flDamage, int ArmorValue )
	{
		float flCurDamage = flDamage;

		if ( flCurDamage == 0.0f || ArmorValue == 0 )
			return flCurDamage;

		float flArmorRatio = 0.5f;
		float flArmorBonus = 0.5f;
		float flNew = flCurDamage * flArmorRatio;
		float flArmor = ( flCurDamage - flNew ) * flArmorBonus;

		if ( flArmor > ArmorValue )
		{
			flArmor = ArmorValue * ( 1.0f / flArmorBonus );
			flNew = flCurDamage - flArmor;
		}

		return flNew;
	};
	float fdamage = max( ( int )ceilf( GetArmourHealth( damage, G::LocalPlayer->GetArmor() ) ), 0 );
	bool someoneDef = Interfaces::EntityList->GetClientEntityFromHandle( bomb->GetBombDefuser() ) != nullptr;
	int height = h - ( ( int )remainanim * h / timeranim );
	int defheight = 0;

	if ( someoneDef )
		defheight = h - ( ( int )defremanim * h / deftimeanim );

	/*Graphics::Drawing->Rect( 0, 0, 8, h, CColor( 0, 0, 0, 150 ) );
	Graphics::Drawing->Rect( 0, height, 8, h - height, CColor( remain > 10 ? CColor( 100, 255, 100 ) : CColor( 255, 100, 100 ) ) );*/

	if ( someoneDef )
	{
		Graphics::Drawing->Rect( 0, 0, 16, h, CColor( 0, 0, 0, 150 ) );
		Graphics::Drawing->Rect( 0, defheight, 16, h - defheight, CColor( 0, 255, 0, 75 ) );
	}

	int textH = height;
	int textStrH = Graphics::Drawing->TextSize( buffer, Graphics::Drawing->fonts.esp_indicator ).bottom;

	if ( textH + textStrH > h )
		textH = h - textStrH;

	else
		textH = textH - textStrH;
	CColor bombcol;

	if (remain > 10) {
		bombcol = CColor(123, 194, 21);
	}
	else if (remain > 5) {
		bombcol = CColor(240, 220, 150);
	}
	else {
		bombcol = CColor(255, 0, 21);
	}

	bomb_inds.push_back({ buffer, CColor(255, 255, 255) });

	/*if ( someoneDef )
		Graphics::Drawing->Text( 1, 5, defbuffer, Graphics::Drawing->fonts.esp_indicator, CColor( 255, 255, 255 ) );*/

	char fldamage[32];
	//sprintf(fldamage, "-%s HP", std::basic_string(fdamage));


	if ( fdamage > G::LocalPlayer->GetHealth() )
		bomb_inds.push_back({ "FATAL", CColor(255, 0, 0) });

	else if (fdamage > 1)
		bomb_inds.push_back({ "-" + std::to_string((int)fdamage) + " HP", CColor(240, 220, 150) });

	/*else if (fdamage > 1)
		Graphics::Drawing->Text(5, 30, std::to_string(fdamage), Graphics::Drawing->fonts.esp_indicator, CColor(240, 220, 150));*/
}
#include "../../Hooking/KeyHandler.h"
#include "../c_fps.h"
//#include "../../../../../../../Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.16.27023/include/string"
float RoundUp(int number) {
	number -= number % 5;
	return number;
}
void CVisuals::DrawIndicators()
{
	std::vector<Indicator_t> indicators;

	for (auto i : custom_inds)
		indicators.push_back(i);

	static auto percent_col = [](int per) -> CColor {
		int red = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
		int green = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);

		return CColor(red, green, 0);
	};

	/*if (c_keyhandler::get()->auto_check("rage_forcebaim_key"))
		indicators.push_back({ "BAIM", percent_col(100) });*/

	if (c_config::get()->b["misc_lowfpswarning"] && c_fps::get()->get_fps() < (1 / Interfaces::Globals->interval_per_tick))
		indicators.push_back({ "FPS", CColor(220, 169, 21) });

	if (c_keyhandler::get()->auto_check("rage_fakeduck_key") && c_config::get()->b["rage_fakeduck"])
		indicators.push_back({ "DUCK", CColor(255, 255, 255) });
	
	CColor lccol;
	CColor dtcol;

	if (G::LocalPlayer && G::LocalPlayer->GetVelocity().Length2D() > 251 && G::LocalPlayer->IsAlive()) {
		static auto is_breaking_lagcomp = false;
		if (!Interfaces::ClientState->iChokedCommands)
		{
			static auto last_origin = G::LocalPlayer->GetOrigin();
			if (G::LocalPlayer->GetOrigin() != last_origin)
			{
				is_breaking_lagcomp = (last_origin.Length2DSqr() - G::LocalPlayer->GetOrigin().Length2DSqr()) > 4096;
				last_origin = G::LocalPlayer->GetOrigin();
			}
		}
		


		if (is_breaking_lagcomp)
			lccol = CColor(123, 194, 21);

		else
			lccol = CColor(255, 0, 21);

		

		indicators.push_back({ "LC", lccol });

		
	}

	int indred;
	int indgreen;
	static float view;
	static float real;
	static float fake;
	if (G::SendPacket && G::UserCmd) {
		view = G::UserCmd->viewangles.y;
		fake = G::LocalPlayer->GetAnimState()->m_flGoalFeetYaw;
		Cheat::Utilities->Math_NormalizeYaw(fake);
	}
	if (!G::SendPacket && G::UserCmd)
		real = G::UserCmd->viewangles.y;
		
	
	//int DesyncWidth = std::clamp(fabs(G::proper_obs - G::fake_obs), 0.f, 58.f);
	//int DesyncWidth = G::LocalPlayer->GetMaxDesyncDelta();
	

	/*if (DesyncWidth >= 55) {
		indred = 124;
		indgreen = 195;
	}
	else if (DesyncWidth < 55 && DesyncWidth >= 50) {
		indred = 150;
		indgreen = 194;
	}
	else if (DesyncWidth < 50 && DesyncWidth >= 45) {
		indred = 176;
		indgreen = 205;
	}
	else if (DesyncWidth < 45 && DesyncWidth >= 40) {
		indred = 213;
		indgreen = 201;
	}
	else if (DesyncWidth < 40 && DesyncWidth >= 35) {
		indred = 220;
		indgreen = 169;
	}
	else if (DesyncWidth < 35 && DesyncWidth >= 30) {
		indred = 228;
		indgreen = 126;
	}
	else if (DesyncWidth < 30 && DesyncWidth >= 25) {
		indred = 229;
		indgreen = 104;
	}
	else if (DesyncWidth < 25 && DesyncWidth >= 20) {
		indred = 235;
		indgreen = 63;
	}
	else if (DesyncWidth < 20 && DesyncWidth >= 1) {
		indred = 237;
		indgreen = 27;
	}
	else {
		indred = 255;
		indgreen = 0;
	}*/


	/*if (c_config::get()->b["rage_aainfo"] && G::LocalPlayer->IsAlive()) {
		indicators.push_back({ "VIEW: " + std::to_string(real), CColor(255, 255, 255) });
		indicators.push_back({ "REAL: " + std::to_string(view), CColor(255, 255, 255) });
		indicators.push_back({ "FAKE: " + std::to_string(fake), CColor(255, 255, 255) });
		indicators.push_back({ "LBY: " + std::to_string(G::LocalPlayer->GetLowerBodyYaw()), CColor(255, 255, 255) });
	}*/
		

	/*if (Config->desyncbeta)
		indicators.push_back({ "LBY", abs(G::RealAngle.y - G::LocalPlayer->GetLowerBodyYaw()) > 30 ? CColor(0, 255, 0) : CColor(255, 0, 0) });*/

	int doubletapTickcountDelta = Interfaces::Globals->tickcount - G::lastDoubleTapInTickcount;
	if (c_config::get()->i["rage_doubletap_mode"] == 0) {
		dtcol = G::Recharged ? CColor(255, 255, 255) : CColor(255, 0, 21);
	}
	else {
		if (doubletapTickcountDelta >= 64)
			dtcol = CColor(255, 255, 255);

		else
			dtcol = CColor(255, 0, 21);
	}
	

	if (c_config::get()->b["rage_doubletap"] && c_keyhandler::get()->auto_check("rage_exploit_key"))
		indicators.push_back({ "DT", dtcol });

	for (auto i : bomb_inds)
		indicators.push_back(i);

	int yOffset = 0;
	int screenW, screenH = 0;
	Interfaces::Engine->GetScreenSize( screenW, screenH );
	auto size = Graphics::Drawing->TextSize( "A", Graphics::Drawing->fonts.esp_indicator );
	if (G::LocalPlayer->IsAlive() && G::LocalPlayer->GetHealth() > 0) {
		for (auto i : indicators)
		{
			Graphics::Drawing->GradientHorizontal(5, screenH - 340 - yOffset - 3, Graphics::Drawing->TextSize(i.str.c_str(), Graphics::Drawing->fonts.esp_indicator).right / 2, 33, CColor(0, 0, 0, 0), CColor(0, 0, 0, 125));
			Graphics::Drawing->GradientHorizontal(5 + Graphics::Drawing->TextSize(i.str.c_str(), Graphics::Drawing->fonts.esp_indicator).right / 2, screenH - 340 - yOffset - 3, Graphics::Drawing->TextSize(i.str.c_str(), Graphics::Drawing->fonts.esp_indicator).right / 2, 33, CColor(0, 0, 0, 125), CColor(0, 0, 0, 0));
			Graphics::Drawing->Text(10, screenH - 340 - yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_indicator, i.color);
			yOffset += 36;
		}
	}
	
	custom_inds.clear();
	bomb_inds.clear();
}

void CVisuals::DrawWeaponName( EntityBoxInfo_t inf )
{
	auto name = (Interfaces::Localize->FindSafe(((CBaseWeapon*)inf.pEnt)->get_full_info()->hud_name));
	auto icon = ((CBaseWeapon*)inf.pEnt)->GetGunIcon(((CBaseWeapon*)inf.pEnt)->GetItemDefenitionIndex());
	int textWidth = Graphics::Drawing->TextSize( name, Graphics::Drawing->fonts.esp_icons).right;
	int textWidth1 = Graphics::Drawing->TextSize(icon, Graphics::Drawing->fonts.weapon_icons).right;
	int x = inf.x;
	int yOffset = 0;
	Vector dist = G::LocalPlayer->GetOrigin() - inf.pEnt->GetOrigin();
	float flDist = sqrtf(dist.Length());

	if (flDist < 1.f)
		flDist = 1.f;

	float flDistShow = flDist / 0.254 - 25;
	if (c_config::get()->i["vis_droppedwpns"] == 1) {
		Graphics::Drawing->Text(inf.x + textWidth / 2 - Graphics::Drawing->TextSize((std::to_string((int)(RoundUp(flDistShow))) + " FT").c_str(), Graphics::Drawing->fonts.esp_icons).right / 2, inf.y + inf.h - 5, (std::to_string((int)(RoundUp(flDistShow))) + " FT").c_str(), Graphics::Drawing->fonts.esp_icons, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]));
		Graphics::Drawing->Text(inf.x, inf.y + inf.h + 4, _wcsupr(const_cast<wchar_t*>(Interfaces::Localize->FindSafe(((CBaseWeapon*)inf.pEnt)->get_full_info()->hud_name))), Graphics::Drawing->fonts.esp_icons, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]));
		yOffset += 4;
	}
	if (c_config::get()->i["vis_droppedwpns"] == 3) {
		Graphics::Drawing->Text(inf.x + textWidth1 / 2 - Graphics::Drawing->TextSize((std::to_string((int)(RoundUp(flDistShow))) + " FT").c_str(), Graphics::Drawing->fonts.esp_icons).right / 2, inf.y + inf.h - 5, (std::to_string((int)(RoundUp(flDistShow))) + " FT").c_str(), Graphics::Drawing->fonts.esp_icons, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]));
		Graphics::Drawing->Text(inf.x, inf.y + inf.h + 5, icon, Graphics::Drawing->fonts.weapon_icons, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]));
		yOffset += 8;
	}
}

void CVisuals::DrawProjectiles(CBaseEntity* pEnt)
{
	const model_t* model = pEnt->GetModel();

	if (!model)
		return;

	if (pEnt->IsDormant())
		return;

	const studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel(model);

	if (!hdr)
		return;

	const auto client_class = pEnt->GetClientClass();

	if (!client_class)
		return;

	const char* to_render = "GRENADE";

	switch (client_class->m_ClassID)
	{
	case (int)ClassID::CMolotovProjectile:
		to_render = "MOLLY";
		break;
	case (int)ClassID::CInferno:
		to_render = "MOLLY";
		break;
	case (int)ClassID::CSmokeGrenadeProjectile:
		to_render = "SMOKE";
		break;
	case (int)ClassID::CDecoyProjectile:
		to_render = "DECOY";
		break;
	case (int)ClassID::CBaseCSGrenadeProjectile:
		if (hdr->name[16] == 's')
			to_render = "FLASH";
		else
			to_render = "FRAG";
		break;
	default:
		break;
	}

	Vector screen;

	if (Cheat::Utilities->Game_WorldToScreen(pEnt->GetAbsOrigin(), screen))
	{
		Graphics::Drawing->Text(screen.x, screen.y, to_render, Graphics::Drawing->fonts.esp_icons, CColor(220, 220, 220, 220));
	}
}

void CVisuals::DrawWeaponAmmo( EntityBoxInfo_t inf )
{
	if ( inf.pEnt->GetClientClass()->m_ClassID == ( int )ClassID::CC4 )
		return;

	std::vector<BottomInfo_t> botInfos;
	auto weapon = ( CBaseWeapon* )inf.pEnt;

	auto name = ((CBaseWeapon*)inf.pEnt)->GetGunText(((CBaseWeapon*)inf.pEnt)->GetItemDefenitionIndex());
	auto icon = ((CBaseWeapon*)inf.pEnt)->GetGunIcon(((CBaseWeapon*)inf.pEnt)->GetItemDefenitionIndex());
	int textWidth = Graphics::Drawing->TextSize(name, Graphics::Drawing->fonts.esp_icons).right;
	int textWidth1 = Graphics::Drawing->TextSize(icon, Graphics::Drawing->fonts.weapon_icons).right;

	if ( c_config::get()->b["vis_droppedwpnsammo"] && weapon )
	{
		int maxAmmo = weapon->get_full_info()->max_clip;
		int curAmmo = weapon->GetAmmoInClip();
		botInfos.push_back( { false, s2ws(""), ( double )maxAmmo, ( double )curAmmo, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_ammo_color"], Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, 1.f ) ) } );
	}

	int yOffset = 0;
	int textHeight = Graphics::Drawing->TextSize( "A", Graphics::Drawing->fonts.esp_main ).bottom + 1;

	for ( auto i : botInfos )
	{
		if ( i.isString )
		{
			auto w = Graphics::Drawing->TextSize( i.str.c_str(), Graphics::Drawing->fonts.esp_main ).right;

			if ( w > inf.w )
				continue;

			Graphics::Drawing->Text( inf.x + inf.w / 2 - w / 2, inf.y + inf.h + 2 + yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_main, i.color );
			yOffset += textHeight;
		}

		else
		{
			if (c_config::get()->i["vis_droppedwpns"] == 1) {
				Graphics::Drawing->Rect(inf.x - 1, inf.y + inf.h + 6 + yOffset + 7, textWidth + 2, 4, CColor(0, 0, 0, Utilities->Util_Clamp(inf.alpha, 0, 150)));
				Graphics::Drawing->Rect(inf.x, inf.y + inf.h + 6 + yOffset + 8, ceil((double)textWidth * (i.percentage / i.maxValue)), 2, i.color);
				yOffset += 5;
			}
			if (c_config::get()->i["vis_droppedwpns"] == 3) {
				Graphics::Drawing->Rect(inf.x - 1, inf.y + inf.h + 6 + yOffset + 16, textWidth1 + 2, 4, CColor(0, 0, 0, Utilities->Util_Clamp(inf.alpha, 0, 150)));
				Graphics::Drawing->Rect(inf.x, inf.y + inf.h + 6 + yOffset + 17, ceil((double)textWidth1 * (i.percentage / i.maxValue)), 2, i.color);
				yOffset += 5;
			}	
		}
	}
}

void CVisuals::DrawBoundingBox( EntityBoxInfo_t inf )
{
	CColor col;

	if ( !inf.pEnt->IsWeapon() )
	{
		if ( inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"] )
			col = CColor( 150, 150, 150, inf.alpha );

		else
			col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_boundingbox_color"], Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, 0.5f) );
	}

	else
		col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_droppedwpns_color"]);

	Graphics::Drawing->Outline( inf.x - 1, inf.y - 1, inf.w + 2, inf.h + 2, CColor( 0, 0, 0, std::clamp( inf.alpha, 0, 100 ) ) );
	Graphics::Drawing->Outline( inf.x, inf.y, inf.w, inf.h, col );
	Graphics::Drawing->Outline( inf.x + 1, inf.y + 1, inf.w - 2, inf.h - 2, CColor(0, 0, 0, std::clamp( inf.alpha, 0, 100 ) ) );
}

void CVisuals::DrawHealthBar( EntityBoxInfo_t inf )
{
	CColor col, col2;
	int hp_red, hp_green;

	if (inf.hp > 100)
		inf.hp = 100;

	if ( inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"] )
	{
		col = CColor( 150, 150, 150, (int)Utilities->Util_Clamp((float)(inf.alpha3), 0.f, 150.f));
		col2 = CColor( 150, 150, 150, (int)Utilities->Util_Clamp((float)(inf.alpha3), 0.f, 150.f));
	}

	else
	{
		if (inf.hp >= 50)
		{
			hp_red = 90;
			hp_green = 200;
		}
		else if (inf.hp >= 30)
		{
			hp_red = 200;
			hp_green = 200;
		}
		else {
			hp_red = 200;
			hp_green = 90;
		}

		col = CColor(hp_red, hp_green, 90, ( int )Utilities->Util_Clamp( ( float )( inf.alpha3 ), 0.f, 220.f ) );
		col2 = CColor(hp_red, hp_green, 90, ( int )Utilities->Util_Clamp( ( float )( inf.alpha3 ), 0.f, 220.f ) );
	}

	Graphics::Drawing->Rect( inf.x - 6, inf.y - 1, 4, inf.h + 2, CColor( 0, 0, 0, ( int )Utilities->Util_Clamp( ( float )( inf.alpha3 ), 0.f, 150.f ) ) );
	int height = inf.h - ( inf.hp * inf.h / 100 );
	Graphics::Drawing->Rect( inf.x - 5, height + inf.y, 2, inf.h - height, col );

	/*if ( inf.hpDifference != 0 )
		Graphics::Drawing->Rect( inf.x - 5, inf.y + height - ( inf.hpDifference * inf.h / 100 ), 2, inf.hpDifference * inf.h / 100, col2 );*/

	if (inf.hp <= 95 && inf.hp > 0)
		Graphics::Drawing->Text(inf.x - 6, inf.y + height - 8, std::to_string(inf.hp).c_str(), Graphics::Drawing->fonts.esp_side, CColor(255, 255, 255, (int)Utilities->Util_Clamp((float)(inf.alpha), 0.f, 150.f)));
}
void CVisuals::DrawName( EntityBoxInfo_t inf )
{
	player_info_t info;
	Interfaces::Engine->GetPlayerInfo( inf.pEnt->GetIndex(), &info );
	int textWidth = Graphics::Drawing->TextSize( info.szName, Graphics::Drawing->fonts.esp_main).right;
	int x = inf.x;
	x -= ( textWidth - ( inf.w - 8 ) ) / 2 - 2;
	CColor col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_name_color"], Utilities->Util_Clamp( ( float )( inf.alpha1 / 255.f ), 0.f, 0.45f ) );

	if ( inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"] )
		col = CColor( 150, 150, 150, inf.alpha );

	Graphics::Drawing->Text( inf.x + ( inf.w / 2 ) - ( textWidth / 2 ), inf.y - 12, info.szName, Graphics::Drawing->fonts.esp_main, col );
	/*int screenx = 0;
	int screeny = 0;
	Interfaces::Engine->GetScreenSize(screenx, screeny);
	RECT DrawArea = { inf.x + (inf.w / 2) - (textWidth / 2), inf.y - 12, screenx, DrawArea.top + 200 };
	Graphics::Fonts::Font_INF->DrawTextA(NULL, info.szName, -1, &DrawArea, 0, D3DCOLOR_RGBA(col.r(), col.g(), col.b(), col.a()));*/
}
void CVisuals::DrawSkeleton(EntityBoxInfo_t inf)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudioModel(inf.pEnt->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;
	CColor col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_skeleton_color"], Utilities->Util_Clamp((float)(inf.alpha4 / 255.f), 0.f, 1.f));
	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->pBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = inf.pEnt->GetBonePosition(j);
			vParent = inf.pEnt->GetBonePosition(pBone->parent);

			if (Utilities->Game_WorldToScreen(vParent, sParent) && Utilities->Game_WorldToScreen(vChild, sChild))
			{
				Graphics::Drawing->Line(sParent[0], sParent[1], sChild[0], sChild[1], col);
			}
		}
	}
}
void CVisuals::DrawSideInfo( EntityBoxInfo_t inf )
{
	if (inf.w <= 0)
		return;

	std::vector<SideInfo_t> sizeInfos;


	/*if ( c_config::get()->b["vis_latency"] && flDist < 48.f )
		sizeInfos.push_back( { std::to_string( inf.pEnt->GetPing() ) + "MS", CColor( 244, 149, 66, Utilities->Util_Clamp( inf.alpha, 0, 150 ) ) } );*/

	if (c_config::get()->b["vis_money"])
		sizeInfos.push_back({ "$" + std::to_string(inf.pEnt->GetMoney()), CColor(110, 170, 40, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

	if (c_config::get()->b["vis_flags"] )
	{
		if (inf.pEnt->GetArmor() > 0)
			sizeInfos.push_back({ inf.pEnt->GetArmorName(), CColor(220, 220, 220, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

		if ( inf.pEnt->IsScoped())
			sizeInfos.push_back( { "ZOOM", CColor( 61, 168, 208, Utilities->Util_Clamp( inf.alpha3, 0, 175) ) } );

		if (*inf.pEnt->FlashAlpha() > 1.f)
			sizeInfos.push_back({ "BLIND", CColor(61, 168, 208, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

		if (inf.pEnt->IsReloadingEnt())
			sizeInfos.push_back({ "R", CColor(61, 168, 208, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

		if (inf.pEnt->HasC4())
			sizeInfos.push_back({ "B", CColor(255, 0, 0, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

		if (inf.pEnt->GetActiveWeapon()->IsPinPulled())
			sizeInfos.push_back({ "PIN", CColor(255, 0, 0, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

		if (c_autowall::get()->autowall(inf.pEnt->GetEyePosition(), G::LocalPlayer->hitbox_position(0), inf.pEnt, G::LocalPlayer).damage > 0 && inf.pEnt->GetActiveWeapon()->is_knife() == false)
			sizeInfos.push_back({ "HIT", CColor(220, 220, 220, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });



		static int crouched_ticks[65] = { 0 };
		static int stored_tick = 0;
		if (*inf.pEnt->flDuckAmount() <= 0.9f && *inf.pEnt->flDuckAmount() > 0.1f && inf.pEnt->GetFlags() & 1 && *inf.pEnt->flDuckSpeed() == 8) {
			if (stored_tick != Interfaces::Globals->tickcount) {
				crouched_ticks[inf.pEnt->GetIndex()]++;
				stored_tick = Interfaces::Globals->tickcount;
			}

			if (crouched_ticks[inf.pEnt->GetIndex()] >= 5)
				sizeInfos.push_back({ "FD", CColor(220, 220, 220, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });
		}
		else
			crouched_ticks[inf.pEnt->GetIndex()] = 0;
	}

	auto weapon = inf.pEnt->GetActiveWeapon();
	auto icon = weapon->GetGunIcon( weapon->GetItemDefenitionIndex() );
	int yOffset = 0;
	int textHeight = 9;
	bool entDormant = inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"];

	for ( auto i : sizeInfos )
	{
		Graphics::Drawing->Text( inf.x + inf.w + 2, inf.y + yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_side, entDormant ? CColor( 150, 150, 150, inf.alpha ) : i.color );
		yOffset += textHeight;
	}
}

void CVisuals::DrawBottomInfo( EntityBoxInfo_t inf )
{
	std::vector<BottomInfo_t> botInfos;
	std::vector<BottomInfo_t> botInfos1;
	auto weapon = inf.pEnt->GetActiveWeapon();

	Vector dist = G::LocalPlayer->GetOrigin() - inf.pEnt->GetOrigin();
	float flDist = sqrtf(dist.Length());

	if (flDist < 1.f)
		flDist = 1.f;

	float flDistShow = flDist / 0.254 - 25;

	if ( c_config::get()->b["vis_ammo"] && weapon )
	{
		int maxAmmo = weapon->get_full_info()->max_clip;
		int curAmmo = weapon->GetAmmoInClip();
		botInfos.push_back( { false, s2ws(""), ( double )maxAmmo, ( double )curAmmo, c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_ammo_color"], (int)Utilities->Util_Clamp((float)(inf.alpha2), 0.f, 175.f)) } );
	}

	/*if ( Config->Vis_LBYTimer )
		botInfos.push_back( { false, "", 100, 100, im2pt( Config->Vis_LBYTimerCol, Utilities->Util_Clamp( ( float )( inf.alpha / 255.f ), 0.f, Config->Vis_LBYTimerCol[3] ) ) } );*/

	if (c_config::get()->b["vis_distance"])
		botInfos.push_back({ true, s2ws(std::to_string((int)(RoundUp(flDistShow))) + " FT"), 0, 0, CColor(220, 220, 220, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

	auto text = weapon->GetGunText(weapon->GetItemDefenitionIndex());
	auto icon = weapon->GetGunIcon( weapon->GetItemDefenitionIndex() );

	/*for (unsigned int l = 0; l < test.length(); l++)
	{
		test[l] = toupper(test[l]);
	}*/

	if ( c_config::get()->b["vis_weapons"] && weapon )
		botInfos.push_back( { true, _wcsupr(const_cast<wchar_t*>(Interfaces::Localize->FindSafe(weapon->get_full_info()->hud_name))), 0, 0, CColor( 220, 220, 220, Utilities->Util_Clamp( inf.alpha3, 0, 175 ) ) } ); //weapon->GetGunText( weapon->GetItemDefenitionIndex() )
	//Interfaces::Localize->FindSafe(weapon->get_full_info()->hud_name)

	if (c_config::get()->b["vis_weaponsicons"] && weapon)
		botInfos1.push_back({ true, s2ws(weapon->GetGunIcon(weapon->GetItemDefenitionIndex())), 0, 0, CColor(c_config::get()->c["vis_weaponicon_color"][0] * 255, c_config::get()->c["vis_weaponicon_color"][1] * 255, c_config::get()->c["vis_weaponicon_color"][2] * 255, Utilities->Util_Clamp(inf.alpha3, 0, 175)) });

	int yOffset = 0;
	int textHeight = Graphics::Drawing->TextSize( "A", Graphics::Drawing->fonts.esp_icons ).bottom / 2;
	bool entDormant = inf.pEnt->IsDormant() && c_config::get()->b["vis_dormant"];

	for ( auto i : botInfos )
	{
		if ( i.isString )
		{
			auto w = Graphics::Drawing->TextSize( i.str.c_str(), Graphics::Drawing->fonts.esp_icons).right;

			Graphics::Drawing->Text( inf.x + inf.w / 2 - w / 2, inf.y + inf.h + 2 + yOffset, i.str.c_str(), Graphics::Drawing->fonts.esp_icons, entDormant ? CColor( 150, 150, 150, inf.alpha3 ) : i.color );
			yOffset += 9;
		}

		else
		{
			Graphics::Drawing->Rect( inf.x - 1, inf.y + inf.h + 2 + yOffset, inf.w + 2, 4, CColor(0, 0, 0, (int)Utilities->Util_Clamp((float)(inf.alpha3), 0.f, 150.f)));
			Graphics::Drawing->Rect( inf.x, inf.y + inf.h + 2 + yOffset + 1, ceil( ( double )inf.w * ( i.percentage / i.maxValue ) ), 2, entDormant ? CColor( 150, 150, 150, inf.alpha2 ) : i.color );
			if (weapon->GetAmmoInClip() <= 3 && weapon->GetAmmoInClip() >= 0)
				Graphics::Drawing->Text(ceil((double)inf.w * (i.percentage / i.maxValue)), inf.y + inf.y + inf.h + 2 + yOffset + 2, std::to_string(weapon->GetAmmoInClip()).c_str(), Graphics::Drawing->fonts.esp_side, CColor(255, 255, 255, (int)Utilities->Util_Clamp((float)(inf.alpha), 0.f, 150.f)));
			yOffset += 6;
		}
	}
	for (auto i : botInfos1)
	{
			auto w = Graphics::Drawing->TextSize(i.str.c_str(), Graphics::Drawing->fonts.weapon_icons).right;
			Graphics::Drawing->Text(inf.x + inf.w / 2 - w / 2, inf.y + inf.h + 2 + yOffset, i.str.c_str(), Graphics::Drawing->fonts.weapon_icons, entDormant ? CColor(150, 150, 150, inf.alpha3) : i.color);
			yOffset += 9;
	}
}


void CVisuals::remove_smoke()
{
	static auto smoke_count = *reinterpret_cast<uint32_t**>( offys.dwSmokeCount );
	static std::vector<const char*> smoke_materials =
	{
		"effects/overlaysmoke",
		"particle/beam_smoke_01",
		"particle/particle_smokegrenade",
		"particle/particle_smokegrenade1",
		"particle/particle_smokegrenade2",
		"particle/particle_smokegrenade3",
		"particle/particle_smokegrenade_sc",
		"particle/smoke1/smoke1",
		"particle/smoke1/smoke1_ash",
		"particle/smoke1/smoke1_nearcull",
		"particle/smoke1/smoke1_nearcull2",
		"particle/smoke1/smoke1_snow",
		"particle/smokesprites_0001",
		"particle/smokestack",
		"particle/vistasmokev1/vistasmokev1",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_nearcull",
		"particle/vistasmokev1/vistasmokev1_nearcull_fog",
		"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev4_emods_nocull",
		"particle/vistasmokev1/vistasmokev4_nearcull",
		"particle/vistasmokev1/vistasmokev4_nocull"
	};

	for ( auto material_name : smoke_materials )
	{
		IMaterial* material = Interfaces::MaterialSystem->FindMaterial( material_name, "Other textures" );
		if (c_config::get()->b["vis_nosmoke"])
			material->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
		else
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
	}
	
	if (c_config::get()->b["vis_nosmoke"])
		*(int*)smoke_count = 0;
}

enum ACT
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};
void grenade_prediction::Tick(int buttons)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	bool in_attack = buttons & IN_ATTACK;
	bool in_attack2 = buttons & IN_ATTACK2;

	//g_pICvar->ConsoleColorPrintf(SDK::Color(255, 255, 255, 255), "OView");

	act = (in_attack && in_attack2) ? ACT_LOB :
		(in_attack2) ? ACT_DROP :
		(in_attack) ? ACT_THROW :
		ACT_NONE;
}
void grenade_prediction::View(CViewSetup* setup)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;

	if (G::LocalPlayer && G::LocalPlayer->IsAlive())
	{
		CBaseWeapon* weapon = G::LocalPlayer->GetActiveWeapon();

		if (weapon && weapon->is_grenade())
		{
			type = weapon->GetItemDefenitionIndex();
			Simulate(setup);
		}
		else
		{
			type = 0;
		}
	}
}
float CSGO_Armor(float flDamage, int ArmorValue) {
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue)* (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}
float flTotalElasticity;
void grenade_prediction::Paint()
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;

	if (!Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected())
		return;

	if ((type) && path.size() > 1)
	{
		Vector nadeStart, nadeEnd;
		Vector prev = path[0];
		Vector endpos = path[path.size() - 1];
		Vector absendpos = endpos;

		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (Cheat::Utilities->Game_WorldToScreen(prev, nadeStart) && Cheat::Utilities->Game_WorldToScreen(*it, nadeEnd))
			{
				Interfaces::Surface->SetDrawColor(CColor(c_config::get()->c["vis_grenadetrajectory_color"][0] * 255, c_config::get()->c["vis_grenadetrajectory_color"][1] * 255, c_config::get()->c["vis_grenadetrajectory_color"][2] * 255));
				Interfaces::Surface->DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y);			
			}

			prev = *it;
		}
		static float a = 105.0f;
		static float b = 30.0f;
		static float c = 140.0f;
		static bool showing = false;
		CColor dmgcol = CColor(255, 255, 255, 180);
		for (int i = 0; i < 65; i++) {
			auto e = Interfaces::EntityList->GetClientEntity(i);

			if (!e)
				continue;
			if (e == G::LocalPlayer)
				continue;
			if (e->GetTeam() == G::LocalPlayer->GetTeam())
				continue;

			float d = (((e->GetOrigin() - absendpos).Length() - b) / c);
			float flDamage = a * exp(-d * d);	
			int dmg = max(static_cast<int>(ceilf(CSGO_Armor(flDamage, e->GetArmor()))), 0);
			G::GrenadeDamage = (dmg > e->GetHealth());
			
			char dmgstr[128];
			sprintf_s(dmgstr, "%idmg", dmg);

			Cheat::Utilities->Game_WorldToScreen(prev, nadeEnd);

			if (dmg > e->GetHealth()) {
				dmgcol = CColor(255, 0, 0, 180);
			}
			else
				dmgcol = CColor(255, 255, 255, 180);

			if (dmg > 1 && G::LocalPlayer->GetActiveWeapon()->GetItemDefenitionIndex() == 44 && !showing) {
				Graphics::Drawing->Text(nadeEnd.x, nadeEnd.y, dmgstr, Graphics::Drawing->fonts.esp_main, dmgcol);
				showing = true;
			}
			else {
				showing = false;
			}
				
		}
		if (G::GrenadeDamage)
			Interfaces::DebugOverlay->AddBoxOverlay(prev, Vector(-1, -1, -1), Vector(1, 1, 1), Vector(0, 0, 0), 50, 255, 25, 127, 0.01);
		else
			Interfaces::DebugOverlay->AddBoxOverlay(prev, Vector(-1, -1, -1), Vector(1, 1, 1), Vector(0, 0, 0), 255, 0, 0, 127, 0.01);
	}
}
static const constexpr auto PIRAD = 0.01745329251f;
void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	sp = static_cast<float>(sin(double(angles.x) * PIRAD));
	cp = static_cast<float>(cos(double(angles.x) * PIRAD));
	sy = static_cast<float>(sin(double(angles.y) * PIRAD));
	cy = static_cast<float>(cos(double(angles.y) * PIRAD));
	sr = static_cast<float>(sin(double(angles.z) * PIRAD));
	cr = static_cast<float>(cos(double(angles.z) * PIRAD));

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}
void grenade_prediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;

	Vector angThrow = viewangles;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float flVel = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	angle_vectors2(angThrow, &vForward, &vRight, &vUp); //angThrow.ToVector(vForward, vRight, vUp);

	vecSrc = G::LocalPlayer->GetEyePosition();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f; //vecDest.MultAdd(vForward, 22.0f);

	TraceHull(vecSrc, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = G::LocalPlayer->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel; //	vecThrow.MultAdd(vForward, flVel);
}

void grenade_prediction::Simulate(CViewSetup* setup)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Vector vecSrc, vecThrow;
	Vector angles; Interfaces::Engine->GetViewAngles(&angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = Interfaces::Globals->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;


	path.clear();
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) {
			logtimer = 0;
		}
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int grenade_prediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{

	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
		Interfaces::DebugOverlay->AddBoxOverlay(tr.end, Vector(-1, -1, -1), Vector(1, 1, 1), Vector(0, 0, 0), 255, 255, 255, 127, 0.001);
	}

	// Set new position
	vecSrc = tr.end;

	return result;
}


bool grenade_prediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (type)
	{
	case WEAPON_SMOKEGRENADE:
	case WEAPON_DECOY:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick%det_tick_mod);
		}
		return false;

	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
		// OR we've been flying for too long

	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast<float>(tick)*interval > 1.5f && !(tick%static_cast<int>(0.2f / interval));

	default:
		assert(false);
		return false;
	}
}

void grenade_prediction::TraceHull(Vector& src, Vector& end, trace_t& tr)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceWorldOnly filter;
	//filter.SetIgnoreClass("BaseCSGrenadeProjectile");
	//filter.bShouldHitPlayers = false;

	Interfaces::Trace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void grenade_prediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void grenade_prediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void grenade_prediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	if (!c_config::get()->b["vis_grenadetrajectory"])
		return;
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector nadeEnd;
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;
		

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr < flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.end, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}

}

int grenade_prediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}
void CVisuals::DrawZeusRange() {
	/*if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	if (!G::LocalPlayer->IsAlive()) 
		return;

	if (!G::LocalPlayer) return;

	if (!*(bool*)((uintptr_t)Interfaces::Input + 0xAD))
		return;

	if (!c_config::get()->b["vis_zeus_range"])
		return;

	auto weapon = G::LocalPlayer->GetActiveWeapon();
	if (weapon->GetItemDefenitionIndex() != ItemDefinitionIndex::WEAPON_TASER)
		return;
#define M_PI 3.14159265358979323846
	float step = M_PI * 2.0 / 1023;
	float rad = weapon->get_csweapon_info()->range;
	Vector origin = G::LocalPlayer->GetEyePosition();

	static double rainbow;

	Vector screenPos;
	static Vector prevScreenPos;

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

		trace_t trace;
		CTraceFilter filter;

		filter.pSkip1 = G::LocalPlayer;


		Interfaces::Trace->TraceRay(Ray_t(origin, pos), MASK_SHOT_BRUSHONLY, &filter, &trace);


		if (Interfaces::DebugOverlay->ScreenPosition(trace.end, screenPos))
			continue;

		if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f&& prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
			rainbow += 0.00001;
			if (rainbow > 1.f)
				rainbow = 0;

			CColor color = CColor::FromHSB(rainbow, 1.f, 1.f);
			Drawing->Line(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color);
			//DrawLine(prevScreenPos.x, prevScreenPos.y + 1, screenPos.x, screenPos.y + 1, color);
			//DrawLine(Vector2D(prevScreenPos.x, prevScreenPos.y - 1), Vector2D(screenPos.x, screenPos.y - 1), color);
		}
		prevScreenPos = screenPos;
	}*/
}