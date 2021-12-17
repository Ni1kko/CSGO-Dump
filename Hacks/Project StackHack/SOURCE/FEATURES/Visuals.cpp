#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/IViewRenderBeams.h"
#include "../FEATURES/Backtracking.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/AutoWall.h"
#include "../SDK/CTrace.h"	
#include "../NewResolverh.h"
#include "../FEATURES/Resolver.h"
#include "../SDK/CGlobalVars.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Visuals.h"
#include "../UTILS/render.h"
#include <intrin.h>
#include "../SDK/IVDebugOverlay.h"
#include "NewEventLog.h"
#include "../HOOKS/hooks.h"
#include "../FEATURES/dh.h"
#include <string.h>

//--- Misc Variable Initalization ---//
float flPlayerAlpha[255];
CColor breaking;
CColor backtrack;
static bool bPerformed = false, bLastSetting;
float fade_alpha[65];
float dormant_time[65];
CColor main_color;
CColor ammo;
SDK::CBaseEntity *BombCarrier;
Hitmarker* hitmarker;
CAutowall* awall;
void CVisuals::set_hitmarker_time( float time ) 
{
	GLOBAL::flHurtTime = time;
}
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
int get_fps()
{
	using namespace std::chrono;
	static int count = 0;
	static auto last = high_resolution_clock::now();
	auto now = high_resolution_clock::now();
	static int fps = 0;

	count++;

	if (duration_cast<milliseconds>(now - last).count() > 1000) {
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}
SDK::CBaseWeapon* GetActiveWeapon(SDK::CBaseEntity* entity)
{
	return reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
}
#define GREEN CColor(111, 255, 15, 255)


void CVisuals::LagCompHitbox(SDK::CBaseEntity* entity, int index)
{
	float duration = 1.5f;

	if (index < 0)
		return;

	if (!entity)
		return;

	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());

	if (!pStudioModel)
		return;

	SDK::mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);

	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		SDK::mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);

		if (!pHitbox)
			continue;

		auto bone_matrix = entity->GetBoneMatrix(pHitbox->bone);

		Vector vMin, vMax;

		MATH::VectorTransform(pHitbox->bbmin, bone_matrix, vMin);
		MATH::VectorTransform(pHitbox->bbmax, bone_matrix, vMax);

		if (pHitbox->radius > -1)
		{
			INTERFACES::DebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->radius, 20, 254, 0, 184, duration);
		}
	}
}

void DrawAutowallCrosshair()
{
	if (!SETTINGS::settings.awallcsh) return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());

	if (!weapon)
		return;

	static float rainbow;
	rainbow += 0.00001;

	if (rainbow > 1.f)
		rainbow = 0.f;

	CColor color;

	color = CColor::FromHSB(rainbow, 1.f, 1.f);

	Vector point = local_player->GetEyePosition();
	INTERFACES::Engine->GetViewAngles(point);

	int screen_x;
	int screen_y;
	INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
	screen_x /= 2; screen_y /= 2;

	int width, height;
	INTERFACES::Engine->GetScreenSize(width, height);

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	if (AutoWall->GetDamagez(point) >= 1.f) //.damage > SETTINGS::settings.damage_val)
		RENDER::DrawFilledRect(width * 0.5 - 1, height * 0.5 - 1, width * 0.5 + 1, height * 0.5 + 1, GREEN);
	else
		RENDER::DrawFilledRect(width * 0.5 - 1, height * 0.5 - 1, width * 0.5 + 1, height * 0.5 + 1, RED);
}

void DrawTaserRange() {

	if (!SETTINGS::settings.taser_range)
		return;

	Vector prev_scr_pos, scr_pos;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	if (!local_player->IsAlive())
		return;

	if (!GetActiveWeapon(local_player))
		return;

	if (GetActiveWeapon(local_player)->GetItemDefenitionIndex() != SDK::WEAPON_TASER)
		return;

	float step = M_PI * 2.0 / 360.f, rad = 184.7f;

	Vector origin = local_player->GetEyePosition();
	static float hue_offset = 0;

	for (auto rotation = 0.f; rotation < (M_PI * 2.f); rotation += step) {
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

		SDK::Ray_t ray; SDK::trace_t trace; SDK::CTraceFilter filter;

		filter.pSkip1 = local_player;
		ray.Init(origin, pos);

		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace);

		if (RENDER::WorldToScreen(trace.end, scr_pos)) {
			if (!prev_scr_pos.IsZero()) {
				int hue = RAD2DEG(rotation) + hue_offset;

				CColor range_colour = CColor(SETTINGS::settings.taserrange_col[0] * 255, SETTINGS::settings.taserrange_col[1] * 255, SETTINGS::settings.taserrange_col[2] * 255, SETTINGS::settings.taserrange_col[3] * 255);

				INTERFACES::Surface->SetDrawColor(range_colour.RGBA[0], range_colour.RGBA[1], range_colour.RGBA[2], range_colour.RGBA[3]);
				INTERFACES::Surface->DrawLine(prev_scr_pos.x, prev_scr_pos.y, scr_pos.x, scr_pos.y);
			} prev_scr_pos = scr_pos;
		}
	} hue_offset += 0.25;
}

void CVisuals::Draw()
{
	if ( !INTERFACES::Engine->IsInGame( ) ) 
	{
		GLOBAL::flHurtTime = 0.f;
		return;
	}
	
		
	auto crosshair = INTERFACES::cvar->FindVar("crosshair");
	if (SETTINGS::settings.awallcsh)
	{
		crosshair->SetValue("0");
		DrawAutowallCrosshair();
	}
	else
	{
		crosshair->SetValue("1");
	}
	
	if (SETTINGS::settings.beam_bool)
	{
		DrawBulletTracers();
	}

	DrawBulletBox();
	DrawCrosshair();
	DrawTaserRange();
	

	

	for (int i = 1; i <= 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!entity) continue;
		if (!local_player) continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player) continue;
		if (entity->GetHealth() <= 0) continue;
		if (entity->GetVecOrigin() == Vector(0, 0, 0)) continue;

		//--- Colors ---//
		if (entity->GetIsDormant() && flPlayerAlpha[i] > 0) flPlayerAlpha[i] -= .3;
		else if (!entity->GetIsDormant() && flPlayerAlpha[i] < 255) flPlayerAlpha[i] = 255;

		float playerAlpha = flPlayerAlpha[i];
		int enemy_hp = entity->GetHealth();
		int hp_red = 255 - (enemy_hp * 2.55);
		int hp_green = enemy_hp * 2.55;
		CColor health_color = CColor(hp_red, hp_green, 1, playerAlpha);
		CColor dormant_color = CColor(100, 100, 100, playerAlpha);
		CColor box_color, still_health, alt_color, zoom_color, name_color, weapon_color, distance_color, arrow_col;

		static auto alpha = 0.f; static auto plus_or_minus = false;
		if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
		alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = clamp(alpha, 0.f, 255.f);
		float arrow_colour[3] = { SETTINGS::settings.fov_col.RGBA[0], SETTINGS::settings.fov_col.RGBA[1], SETTINGS::settings.fov_col.RGBA[2] };
		float arrowteam_colour[3] = { SETTINGS::settings.arrowteam_col.RGBA[0], SETTINGS::settings.arrowteam_col.RGBA[1], SETTINGS::settings.arrowteam_col.RGBA[2] };

		if (entity->GetIsDormant())
		{
			main_color = dormant_color;
			still_health = health_color;
			alt_color = CColor(20, 20, 20, playerAlpha);
			zoom_color = dormant_color;
			breaking = dormant_color;
			backtrack = dormant_color;
			box_color = dormant_color;
			ammo = dormant_color;
			name_color = dormant_color;
			weapon_color = dormant_color;
			distance_color = dormant_color;
			arrow_col = dormant_color;
		}
		else if (!entity->GetIsDormant())
		{
			main_color = CColor(255, 255, 255, playerAlpha);
			still_health = health_color;
			alt_color = CColor(0, 0, 0, 165);
			zoom_color = CColor(150, 150, 220, 165);
			breaking = CColor(220, 150, 150, 165);
			backtrack = CColor(155, 220, 150, 165);
			box_color = SETTINGS::settings.box_col;
			ammo = CColor(61, 135, 255, 165);
			name_color = SETTINGS::settings.name_col;
			weapon_color = SETTINGS::settings.weapon_col;
			distance_color = SETTINGS::settings.distance_col;
			arrow_col = SETTINGS::settings.fov_col;
		}
		Vector min, max, pos, pos3D, top, top3D; entity->GetRenderBounds(min, max);
		pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10); top3D = pos3D + Vector(0, 0, max.z + 10);

		if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
		{
			if (!is_teammate)
			{
				if (SETTINGS::settings.box_bool) DrawBox(entity, box_color);
				if (SETTINGS::settings.Visuals.Visuals_Name) DrawName(entity, name_color, i, pos, top);
				if (SETTINGS::settings.Visuals.Visuals_Weapons) DrawWeapon(entity, weapon_color, i, pos, top);
			//	if (SETTINGS::settings.skeleton_esp) DrawSkeleton(entity, CColor(255, 255, 255, 255));
				if (SETTINGS::settings.Visuals.Visuals_HealthBar) DrawHealth(entity, still_health, alt_color, pos, top);
				if (SETTINGS::settings.ammo_bool) DrawAmmo(entity, ammo, alt_color, pos, top);
			}
			/*else if (is_teammate)
			{
				if (SETTINGS::settings.boxteam) DrawBox(entity, SETTINGS::settings.boxteam_col, pos, top);
				if (SETTINGS::settings.nameteam) DrawName(entity, SETTINGS::settings.nameteam_col, i, pos, top);
				if (SETTINGS::settings.weaponteam) DrawWeapon(entity, SETTINGS::settings.weaponteam_col, i, pos, top);
				if (SETTINGS::settings.healthteam) DrawHealth(entity, still_health, alt_color, pos, top);
				if (SETTINGS::settings.ammoteam) DrawAmmo(entity, ammo, alt_color, pos, top);
			}*/
			DrawInfo(entity, main_color, zoom_color, pos, top);
			//TranslateResolverRecord(entity, main_color, top, pos);
			
		}
	}
}

void CVisuals::ClientDraw()
{
	//if (SETTINGS::settings.spread_bool==1) DrawInaccuracy();
	//if (SETTINGS::settings.spread_bool ==2) DrawInaccuracy1();
	if (SETTINGS::settings.scope_bool) DrawBorderLines();
	//fix for cool
	//if (SETTINGS::settings.spread_bool != 0)
	//	INTERFACES::cvar->FindVar("crosshair")->SetValue(0);
	//else
	//	INTERFACES::cvar->FindVar("crosshair")->SetValue(1);
	

	DrawIndicator();
	if (SETTINGS::settings.hitmarker) DrawHitmarker();
	
	
}

std::string str_to_upper(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

	return strToConvert;
}

void CVisuals::DrawBox(SDK::CBaseEntity* entity, CColor color)
{
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;

		RENDER::DrawEmptyRect(pos.x - width / 2, top.y, (pos.x - width / 2) + width, top.y + height, color);
	//	RENDER::DrawEmptyRect((pos.x - width / 2) + 1, top.y + 1, (pos.x - width / 2) + width - 1, top.y + height - 1, CColor(20, 20, 20, flPlayerAlpha[entity->GetIndex()]));
	//	RENDER::DrawEmptyRect((pos.x - width / 2) - 1, top.y - 1, (pos.x - width / 2) + width + 1, top.y + height + 1, CColor(20, 20, 20, flPlayerAlpha[entity->GetIndex()]));
	}

}

void CVisuals::DrawName(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top)
{
	SDK::player_info_t ent_info; INTERFACES::Engine->GetPlayerInfo(index, &ent_info);

	int height = (pos.y - top.y), width = height / 2;
	RENDER::DrawF(pos.x, top.y - 7, FONTS::visuals_name_font, true, true, color, ent_info.name);
}

float CVisuals::resolve_distance(Vector src, Vector dest)
{
	Vector delta = src - dest;
	float fl_dist = ::sqrtf((delta.Length()));
	if (fl_dist < 1.0f) return 1.0f;
	return fl_dist;
}

void CVisuals::DrawDistance(SDK::CBaseEntity* entity, CColor color, Vector pos, Vector top)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	SDK::player_info_t ent_info;
	Vector vecOrigin = entity->GetVecOrigin(), vecOriginLocal = local_player->GetVecOrigin();

	char dist_to[32]; int height = (pos.y - top.y), width = height / 2;

	sprintf_s(dist_to, "%.0f ft", resolve_distance(vecOriginLocal, vecOrigin));
	RENDER::DrawF(pos.x, SETTINGS::settings.ammo_bool ? pos.y + 12: pos.y + 8, FONTS::visuals_esp_font, true, true, color, dist_to);
}

std::string fix_item_name(std::string name)
{
	if (name[0] == 'C')
		name.erase(name.begin());

	auto startOfWeap = name.find("Weapon");
	if (startOfWeap != std::string::npos)
		name.erase(name.begin() + startOfWeap, name.begin() + startOfWeap + 6);

	return name;
}

void CVisuals::DrawWeapon(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top)
{
	SDK::player_info_t ent_info; INTERFACES::Engine->GetPlayerInfo(index, &ent_info);

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex());
	if (!weapon) return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
	if (!c_baseweapon) return;

	bool is_teammate = local_player->GetTeam() == entity->GetTeam(), distanceThing, distanceThing2;
	if (SETTINGS::settings.ammo_bool) distanceThing = true; else distanceThing = false; if (SETTINGS::settings.ammoteam) distanceThing2 = true; else distanceThing2 = false;
	int height = (pos.y - top.y), width = height / 2, distanceOn = distanceThing ? pos.y + 12 : pos.y + 8, distanceOn2 = distanceThing2 ? pos.y + 12 : pos.y + 8;

	if (c_baseweapon->is_revolver())
		RENDER::DrawF(pos.x, is_teammate ? distanceOn2 : distanceOn, FONTS::visuals_name_font, true, true, color, "R8 REVOLVER");
	else if (c_baseweapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_USP_SILENCER)
		RENDER::DrawF(pos.x, is_teammate ? distanceOn2 : distanceOn, FONTS::visuals_name_font, true, true, color, "USP-S");
	else if (c_baseweapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_M4A1_SILENCER)
		RENDER::DrawF(pos.x, is_teammate ? distanceOn2 : distanceOn, FONTS::visuals_name_font, true, true, color, "M4A1-S");
	else
		RENDER::DrawF(pos.x, is_teammate ? distanceOn2 : distanceOn, FONTS::visuals_name_font, true, true, color, fix_item_name(weapon->GetClientClass()->m_pNetworkName));

}

void CVisuals::DrawHealth(SDK::CBaseEntity* entity, CColor color, CColor dormant, Vector pos, Vector top)
{
	int enemy_hp = entity->GetHealth(),
		hp_red = 255 - (enemy_hp * 2.55),
		hp_green = enemy_hp * 2.55,
		height = (pos.y - top.y),
		width = height / 2;

	float offset = (height / 4.f) + 5;
	UINT hp = height - (UINT)((height * enemy_hp) / 100);

	RENDER::DrawEmptyRect((pos.x - width / 2) - 6, top.y, (pos.x - width / 2) - 3, top.y + height, dormant);
	RENDER::DrawLine((pos.x - width / 2) - 4, top.y + hp, (pos.x - width / 2) - 4, top.y + height, color);
	RENDER::DrawLine((pos.x - width / 2) - 5, top.y + hp, (pos.x - width / 2) - 5, top.y + height, color);

	if (entity->GetHealth() < 100)
		RENDER::DrawF((pos.x - width / 2) - 4, top.y + hp, FONTS::visuals_name_font, true, true, main_color, std::to_string(enemy_hp));
}

void CVisuals::BombPlanted(SDK::CBaseEntity* entity)
{
	BombCarrier = nullptr;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetVecOrigin();
	SDK::CCSBomb* Bomb = (SDK::CCSBomb*)entity;

	float flBlow = Bomb->GetC4BlowTime();
	float TimeRemaining = flBlow;// -(INTERFACES::Globals->interval_per_tick * local_player->GetTickBase());
	char buffer[64];
	sprintf_s(buffer, "B - %.1fs", TimeRemaining);
	RENDER::DrawF(10, 10, FONTS::visuals_lby_font, false, false, CColor(124, 195, 13, 255), buffer);
}

void CVisuals::DrawDropped(SDK::CBaseEntity* entity)
{
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	SDK::CBaseWeapon* weapon_cast = (SDK::CBaseWeapon*)entity;

	if (!weapon_cast)
		return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex());
	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!c_baseweapon)
		return;

	if (!weapon)
		return;

	SDK::CBaseEntity* plr = INTERFACES::ClientEntityList->GetClientEntityFromHandle((HANDLE)weapon_cast->GetOwnerHandle());
	if (!plr && RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		std::string ItemName = fix_item_name(weapon->GetClientClass()->m_pNetworkName);
		int height = (pos.y - top.y);
		int width = height / 2;
		RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, WHITE, ItemName.c_str()); //numpad_menu_font
	}
}

void CVisuals::DrawAmmo(SDK::CBaseEntity* entity, CColor color, CColor dormant, Vector pos, Vector top)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
	if (!c_baseweapon) return;

	int height = (pos.y - top.y);

	float offset = (height / 4.f) + 5;
	UINT hp = height - (UINT)((height * 3) / 100);

	auto animLayer = entity->GetAnimOverlay(1);
	if (!animLayer.m_pOwner)
		return;

	auto activity = entity->GetSequenceActivity(animLayer.m_nSequence);

	int iClip = c_baseweapon->GetLoadedAmmo();
	int iClipMax = c_baseweapon->get_full_info()->max_clip;

	float box_w = (float)fabs(height / 2);
	float width;
	if (activity == 967 && animLayer.m_flWeight != 0.f)
	{
		float cycle = animLayer.m_flCycle;
		width = (((box_w * cycle) / 1.f));
	}
	else
		width = (((box_w * iClip) / iClipMax));

	RENDER::DrawFilledRect((pos.x - box_w / 2), top.y + height + 3, (pos.x - box_w / 2) + box_w + 2, top.y + height + 7, dormant); //outline
	RENDER::DrawFilledRect((pos.x - box_w / 2) + 1, top.y + height + 4, (pos.x - box_w / 2) + width + 1, top.y + height + 6, color); //ammo
}

void CVisuals::DrawInfo(SDK::CBaseEntity* entity, CColor color, CColor alt, Vector pos, Vector top)
{
	std::vector<std::pair<std::string, CColor>> stored_info;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

	/*if (SETTINGS::settings.money_bool && !is_teammate)
		stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), backtrack));
	else if (SETTINGS::settings.moneyteam && is_teammate)
		stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), backtrack));*/

	if (SETTINGS::settings.info_bool && !is_teammate)
	{
		if (entity->GetArmor() > 0)
			stored_info.push_back(std::pair<std::string, CColor>(entity->GetArmorName(), color));

		if (entity->GetIsScoped())
			stored_info.push_back(std::pair<std::string, CColor>("scoped", CColor(30, 144, 255)));
	}
	/*else if (SETTINGS::settings.flagsteam && is_teammate)
	{
		if (entity->GetArmor() > 0)
			stored_info.push_back(std::pair<std::string, CColor>(entity->GetArmorName(), color));

		if (entity->GetIsScoped())
			stored_info.push_back(std::pair<std::string, CColor>("zoom", alt));
	}*/

	int height = (pos.y - top.y), width = height / 2, i = 0;
	for (auto Text : stored_info)
	{
		RENDER::DrawF((pos.x + width / 2) + 5, top.y + i, FONTS::visuals_name_font, false, false, Text.second, Text.first);
		i += 8;
	}
}

void CVisuals::DrawInaccuracy()
{
	
}

/*void CVisuals::DrawBox()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (!INTERFACES::Engine->IsInGame() || !local_player) { Impacts.clear(); return; }
	if (Impacts.size() > 30) Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = Impacts.at(i);
		if (!current.pPlayer) continue;
		if (current.pPlayer->GetIsDormant()) continue;

		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;

		INTERFACES::DebugOverlay->AddBoxOverlay(Impacts.end, CColor(255, 255,255,255));

		Impacts.erase(Impacts.begin() + i);
	}
}*/

void CVisuals::DrawSkeleton(SDK::CBaseEntity* entity, CColor color) //wut hek
{
	matrix3x4_t bone_matrix[128];
	if (!entity->SetupBones(bone_matrix, 128, BONE_USED_BY_ANYTHING, 0))
		return;

	auto studio_model = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!studio_model)
		return;

	for (int i = 0; i < studio_model->numbones; i++)
	{
		auto bone = studio_model->pBone(i);
		if (!bone || bone->parent < 0 || !(bone->flags & BONE_USED_BY_HITBOX))
			continue;

		/// 2 bone matrices, 1 for the child and 1 for the parent bone
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
		if (RENDER::WorldToScreen(bone_position_1, screen1) && RENDER::WorldToScreen(bone_position_2, screen2))
			RENDER::DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, color);
	}
	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!pStudioModel)
		return;

	matrix3x4_t pBoneToWorldOut[128];
	if (!entity->SetupBones(pBoneToWorldOut, 128, BONE_USED_BY_ANYTHING, 0))
		return;

	SDK::mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);
	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioModel->numbones; j++)
	{
		SDK::mstudiobone_t* pBone = pStudioModel->pBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = entity->GetBonePosition(j);
			vParent = entity->GetBonePosition(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = entity->GetBonePosition(iChestBone + 1) - entity->GetBonePosition(iChestBone); // direction vector from chest to neck
			vBreastBone = entity->GetBonePosition(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			INTERFACES::DebugOverlay->ScreenPosition(vParent, sParent);
			INTERFACES::DebugOverlay->ScreenPosition(vChild, sChild);

			INTERFACES::Surface->DrawSetColor(CColor(255, 255, 255, 255));
			INTERFACES::Surface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
		}
	}
	for (int i = 0; i < pStudioModel->numbones; i++)
	{
		SDK::mstudiobone_t* pBone = pStudioModel->pBone(i);
		if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
			continue;

		Vector vBonePos1;
		if (!RENDER::WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
			continue;

		Vector vBonePos2;
		if (!RENDER::WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
			continue;

		RENDER::DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, main_color);
	}
}
void CVisuals::DrawBulletBox()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	

	if (!INTERFACES::Engine->IsInGame() || !local_player) { Impacts.clear(); return; }
	if (Impacts.size() > 30) Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		if (local_player)
		{
			if (SETTINGS::settings.impacts)
			{
				auto current = visuals->Impacts.at(i);


				bool is_local_player = current.pPlayer == local_player;
				bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;

				if (is_local_player)
				{
					INTERFACES::DebugOverlay->AddBoxOverlay(current.vecImpactPos, Vector(-2.5, -2.5, -2.5), Vector(2.5, 2.5, 2.5), Vector(0, 0, 0), 0, 0, 255, 127, 3.8); // this is what [censored] and [censored] have, both red and blue impact boxes, not the best method but eh
				}

				Impacts.erase(Impacts.begin() + i);
			}
		}
	}

}



void CVisuals::TranslateResolverRecord(SDK::CBaseEntity* entity, CColor color, Vector top, Vector pos)
{
	std::vector<std::pair<std::string, CColor>> stored_info;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

	/*if (SETTINGS::settings.money_bool && !is_teammate)
	stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), backtrack));
	else if (SETTINGS::settings.moneyteam && is_teammate)
	stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), backtrack));*/

	if (SETTINGS::settings.experimental_resolver && !is_teammate)
	{
		

		if (g_Resolver.iResolve[entity->GetIndex()] == 0)
			stored_info.push_back(std::pair<std::string, CColor>("bruteforce", CColor(0, 255, 0)));
	    if (g_Resolver.iResolve[entity->GetIndex()] == -2)
			stored_info.push_back(std::pair<std::string, CColor>("reset", CColor(0, 255, 0)));
	    if (g_Resolver.iResolve[entity->GetIndex()] == -1)
			stored_info.push_back(std::pair<std::string, CColor>("logic bruteforce", CColor(0, 255, 0)));
	    if (g_Resolver.iResolve[entity->GetIndex()] == 1)
			stored_info.push_back(std::pair<std::string, CColor>("reverse freestanding", CColor(0, 255, 0)));
		if (g_Resolver.iResolve[entity->GetIndex()] == 2)
			stored_info.push_back(std::pair<std::string, CColor>("bruteforce hit", CColor(0, 255, 0)));
		if (g_Resolver.iResolve[entity->GetIndex()] == 3)
			stored_info.push_back(std::pair<std::string, CColor>("low delta", CColor(0, 255, 0)));
		if (g_Resolver.iResolve[entity->GetIndex()] == 4)
			stored_info.push_back(std::pair<std::string, CColor>("no fake", CColor(0, 255, 0)));
		if (g_Resolver.iResolve[entity->GetIndex()] == 5)
			stored_info.push_back(std::pair<std::string, CColor>("lby update", CColor(0, 255, 0)));
		if (g_Resolver.iResolve[entity->GetIndex()] == 6)
			stored_info.push_back(std::pair<std::string, CColor>("lby", CColor(0, 255, 0)));
		if (g_Resolver.iResolve[entity->GetIndex()] == 7)
			stored_info.push_back(std::pair<std::string, CColor>("override", CColor(0, 255, 0)));


	}
	/*else if (SETTINGS::settings.flagsteam && is_teammate)
	{
	if (entity->GetArmor() > 0)
	stored_info.push_back(std::pair<std::string, CColor>(entity->GetArmorName(), color));

	if (entity->GetIsScoped())
	stored_info.push_back(std::pair<std::string, CColor>("zoom", alt));
	}*/

	int height = (pos.y - top.y), width = height / 2, i = 0;
	for (auto Text : stored_info)
	{
		RENDER::DrawF((pos.x + width / 2) + 5, top.y + i, FONTS::visuals_name_font, false, false, Text.second, Text.first);
		i += 8;
	}
}


void CVisuals::DrawBulletTracers() /// [censored] bullet tracers
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (!INTERFACES::Engine->IsInGame() || !local_player) { Impacts.clear(); return; }
	if (Impacts.size() > 30) Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = visuals->Impacts.at(i);


		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;
		
		if (is_local_player)
		{                                                                                                           // i  divide so the bullet beam isnt exactly where our eyes are
			INTERFACES::DebugOverlay->AddLineOverlay(current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset() / 1.1, current.vecImpactPos, SETTINGS::settings.bulletlocal_col[0] * 255, SETTINGS::settings.bulletlocal_col[1] * 255, SETTINGS::settings.bulletlocal_col[2] * 255, SETTINGS::settings.bulletlocal_col[3] * 255, true, SETTINGS::settings.bulletlife);
		}
		if (!is_local_player && !is_teammate)
		{
			INTERFACES::DebugOverlay->AddLineOverlay(current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset(), current.vecImpactPos, SETTINGS::settings.bulletenemy_col[0] * 255, SETTINGS::settings.bulletenemy_col[1] * 255, SETTINGS::settings.bulletenemy_col[2] * 255, SETTINGS::settings.bulletenemy_col[3] * 255, true, SETTINGS::settings.bulletlife);

		}
		

		Impacts.erase(Impacts.begin() + i);
	}



}


void CVisuals::DrawBulletBeams()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (!INTERFACES::Engine->IsInGame() || !local_player) { Impacts.clear(); return; }
	if (Impacts.size() > 30) Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = Impacts.at(i);
		if (!current.pPlayer) continue;
		if (current.pPlayer->GetIsDormant()) continue;

		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;

		if (current.pPlayer == local_player)
			current.color = CColor(SETTINGS::settings.bulletlocal_col[0]*255, SETTINGS::settings.bulletlocal_col[1] * 255, SETTINGS::settings.bulletlocal_col[2] * 255, SETTINGS::settings.bulletlocal_col[3] * 255);
		else if (current.pPlayer != local_player && !is_teammate)
			current.color = CColor(SETTINGS::settings.bulletenemy_col[0]*255, SETTINGS::settings.bulletenemy_col[1] * 255, SETTINGS::settings.bulletenemy_col[2] * 255, SETTINGS::settings.bulletenemy_col[3] * 255);
		else if (current.pPlayer != local_player && is_teammate)
			current.color = CColor(SETTINGS::settings.bulletteam_col[0]*255, SETTINGS::settings.bulletteam_col[1] * 255, SETTINGS::settings.bulletteam_col[2] * 255, SETTINGS::settings.bulletteam_col[3] * 255);

		SDK::BeamInfo_t beamInfo;
		beamInfo.m_nType = SDK::TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = SETTINGS::settings.bulletlife;
		beamInfo.m_flWidth = SETTINGS::settings.bulletsize;
		beamInfo.m_flEndWidth = 2.0f;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = current.color.RGBA[0];
		beamInfo.m_flGreen = current.color.RGBA[1];
		beamInfo.m_flBlue = current.color.RGBA[2];
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = SDK::FBEAM_ONLYNOISEONCE | SDK::FBEAM_NOTILE | SDK::FBEAM_HALOBEAM;

		beamInfo.m_vecStart = current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset();
		beamInfo.m_vecEnd = current.vecImpactPos;

		auto beam = INTERFACES::ViewRenderBeams->CreateBeamPoints(beamInfo);
		if (beam) INTERFACES::ViewRenderBeams->DrawBeam(beam);

		Impacts.erase(Impacts.begin() + i);
	}


	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = Impacts.at(i);
		if (!current.pPlayer) continue;
		if (current.pPlayer->GetIsDormant()) continue;

		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;



		Impacts.erase(Impacts.begin() + i);
	}
}

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 8D 58") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));
	return (T*)find_hud_element(pThis, name);
}

void CVisuals::preservefeed(SDK::IGameEvent* Event)
{
	static DWORD* deathNotice;
	static void(__thiscall *ClearDeathNotices)(DWORD);
	auto m_local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (m_local) {
			if (!deathNotice) deathNotice = FindHudElement<DWORD>("CCSGO_HudDeathNotice");
			if (deathNotice) {
				float* localDeathNotice = (float*)((DWORD)deathNotice + 0x50);
				if (localDeathNotice) *localDeathNotice = SETTINGS::settings.preservekillfed ? FLT_MAX : 1.5f;
				if (0 && deathNotice - 20 || GLOBAL::round_change) {
					if (!ClearDeathNotices) ClearDeathNotices = (void(__thiscall*)(DWORD))UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");
					if (ClearDeathNotices) ClearDeathNotices(((DWORD)deathNotice - 20));
				}
			}
		}
}


void CVisuals::DrawCrosshair()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto crosshair = INTERFACES::cvar->FindVar("crosshair");
	if (SETTINGS::settings.xhair_type == 0)
	{
		crosshair->SetValue("1");
		return;
	}
	else 
		crosshair->SetValue("0");

	int W, H, cW, cH;
	INTERFACES::Engine->GetScreenSize(W, H);

	cW = W / 2; cH = H / 2;

	int dX = W / 120.f, dY = H / 120.f;
	int drX, drY;

	if (SETTINGS::settings.xhair_type == 2)
	{
		drX = cW - (int)(dX * (((local_player->GetPunchAngles().y * 2.f) * 0.45f) + local_player->GetPunchAngles().y));
		drY = cH + (int)(dY * (((local_player->GetPunchAngles().x * 2.f) * 0.45f) + local_player->GetPunchAngles().x));
	}
	else
	{
		drX = cW;
		drY = cH;
	}

	INTERFACES::Surface->DrawSetColor(BLACK);
	INTERFACES::Surface->DrawFilledRect(drX - 4, drY - 2, drX - 4 + 8, drY - 2 + 4);
	INTERFACES::Surface->DrawFilledRect(drX - 2, drY - 4, drX - 2 + 4, drY - 4 + 8);

	INTERFACES::Surface->DrawSetColor(WHITE);
	INTERFACES::Surface->DrawFilledRect(drX - 3, drY - 1, drX - 3 + 6, drY - 1 + 2);
	INTERFACES::Surface->DrawFilledRect(drX - 1, drY - 3, drX - 1 + 2, drY - 3 + 6);
}

#define WEAPON_TYPE_GRENADE 9
#define WEAPON_TYPE_KNIFE 0

/*void CVisuals::DrawAutowallCrosshair()
{
	if (!SETTINGS::settings.awallcsh)
		return;
	SDK::CBaseWeapon* wapon = nullptr;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;
	
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon)
		return;

	auto weapon_info = weapon->GetWeaponInfo();
	if (!weapon_info)
		return;

	
	if (weapon_info->m_WeaponType == WEAPON_TYPE_GRENADE || weapon_info->m_WeaponType == WEAPON_TYPE_KNIFE) return;
	
	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	Vector direction;
	MATH::AngleVectors(viewangles, &direction);

	Vector start = local_player->GetVecOrigin() + local_player->GetViewOffset();
	Vector end = start + (direction * 8192.f);
	
	auto autowall_info = awall->SimulateFireBullet(start, end, local_player);
	CColor crosshair_color;
	switch (autowall_info.penetration_count)
	{
	case 0:
		crosshair_color = BLACK;
		break;
	case 1:
		crosshair_color = BLUE;
		break;
	case 2:
		crosshair_color = YELLOW;
		break;
	case 3:
		crosshair_color = GREEN;
		break;
	default:
		crosshair_color = RED;
	}

	int width, height;
	INTERFACES::Engine->GetScreenSize(width, height);

	RENDER::DrawFilledRect(width * 0.5 - 1, height * 0.5 - 1, width * 0.5 + 1, height * 0.5 + 1, crosshair_color);

	
}*/

/*void CVisuals::DrawSkeleton(SDK::CBaseEntity* pEntity)
{
	SDK::studiohdr_t* pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pEntity->GetModel());
	if (!pStudioHdr)
		return;
	Vector vParent, vChild, sParent, sChild;
	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		SDK::mstudiobone_t* pBone = pStudioHdr->pBone(j);
		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePosition(j);
			vParent = pEntity->GetBonePosition(pBone->parent);
			if (RENDER::WorldToScreen(vParent, sParent) && RENDER::WorldToScreen(vChild, sChild))
			{
				RENDER::DrawLine(sParent[0], sParent[1], sChild[0], sChild[1], CColor(255, 255, 255, 255));
			}
		}
	}
}*/

void CVisuals::DrawInaccuracy1()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return;

	int spread_Col[3] = { SETTINGS::settings.spreadcirclecol[0], SETTINGS::settings.spreadcirclecol[1], SETTINGS::settings.spreadcirclecol[2] };


	int W, H, cW, cH;
	INTERFACES::Engine->GetScreenSize(W, H);
	cW = W / 2; cH = H / 2;

	if (local_player->IsAlive())
	{
		auto accuracy = (weapon->GetInaccuracy() + weapon->GetSpreadCone()) * 500.f;

		float r;
		float alpha, newAlpha;

		for (r = accuracy; r>0; r--)
		{
			if (!weapon->is_grenade() && !weapon->is_knife())


				alpha = r / accuracy;
			newAlpha = pow(alpha, 5);

			RENDER::DrawCircle(cW, cH, r, 60, CColor(spread_Col[0], spread_Col[1], spread_Col[2], newAlpha * 130));
		}
	}
}
void CVisuals::DrawIndicator()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;

	float breaking_lby_fraction = fabs(MATH::NormalizeYaw(GLOBAL::real_angles.y - local_player->GetLowerBodyYaw())) / 180.f;
	float lby_delta = abs(MATH::NormalizeYaw(GLOBAL::real_angles.y - local_player->GetLowerBodyYaw()));

	int screen_width, screen_height;
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

	if (SETTINGS::settings.showlbydelta)
	{
		RENDER::DrawF(10, screen_height / 2 - 10, FONTS::menu_checkbox_font, false, false, CColor((1.f - breaking_lby_fraction) * 255.f, breaking_lby_fraction * 255.f, 0), "LBY Delta: " + std::to_string(lby_delta));
	}
	int iY = 88;
	static CColor lby;
	if (local_player->GetVelocity().Length2D() > 0.1f && !fake_walk && local_player->GetFlags() & FL_ONGROUND) //fixes moving delay with fast moving angles, thanks red
	{
		lby = CColor(255, 0, 0);
	}
	else
	{
		if (lby_delta > 35)
		{
			lby = CColor(147, 189, 29);
		}
		else
		{
			lby = CColor(252, 17, 17);
		}
	}
	Vector pre_position;
	Vector origin_delta;
	if (local_player->GetVecOrigin() != pre_position)
	{
		origin_delta = local_player->GetVecOrigin() - pre_position;
		pre_position = local_player->GetVecOrigin();
	}


	const auto breaking_lc = origin_delta.Length() > 40;
	if (local_player->GetVelocity().Length2D() >= 270 && local_player->IsAlive())
	{


		if (breaking_lc)
			RENDER::DrawF(10, screen_height - 54, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "LC"); // greem

		else
			RENDER::DrawF(10, screen_height - 54, FONTS::visuals_lby_font, false, false, CColor(252, 17, 17), "LC"); // red




		RENDER::DrawF(10, screen_height - 88, FONTS::visuals_lby_font, false, false, lby, "LBY");


		RENDER::DrawF(10, screen_height - 122, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "PING");

		if ((get_fps() < 120))
			RENDER::DrawF(10, screen_height - 151, FONTS::visuals_lby_font, false, false, CColor(252, 17, 17), "FPS");


	}
	else
	{

		RENDER::DrawF(10, screen_height - 88, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "PING");

		RENDER::DrawF(10, screen_height - 54, FONTS::visuals_lby_font, false, false, lby, "LBY");
		if ((get_fps() < 120)) RENDER::DrawF(10, screen_height - 122, FONTS::visuals_lby_font, false, false, CColor(252, 17, 17), "FPS");

		/*if (INTERFACES::Engine->IsConnected())
		{
		for (int i = 0; i < INTERFACES::Engine->GetMaxClients(); ++i)
		{
		SDK::CBaseEntity *pEntity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!pEntity || !pEntity->IsAlive())
		continue;
		if (pEntity->GetFlags() & FL_ONGROUND && pEntity->GetVelocity().Length2D() <= 35) {
		RENDER::DrawF(10, screen_height - 162, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "F"); // ñóêà åáàíîå 1 ñåíòÿáðÿ êàê ïàñòèòü(c)Yomeel
		}
		else
		RENDER::DrawF(10, screen_height - 162, FONTS::visuals_lby_font, false, false, CColor(252, 17, 17), "F"); // êðàñíûé ïiçäà ðåñîëüâåðó
		}*/

		if (SETTINGS::settings.lag_bool && SETTINGS::settings.flindicate)
			RENDER::DrawF(10, screen_height - 155, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "FL"); // greem

		else
			RENDER::DrawF(10, screen_height - 120, FONTS::visuals_lby_font, false, false, CColor(252, 17, 17), "FL"); // red



		if (SETTINGS::settings.a_mode == 1)
			RENDER::DrawF(5, screen_height / 2 + 26, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "MANUAL");

		if (SETTINGS::settings.aa_pitch_type == 0)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "NO PITCH");

		if (SETTINGS::settings.aa_pitch_type == 1)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "EMOTION");
		if (SETTINGS::settings.aa_pitch_type == 2)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "FAKE DOWN");
		if (SETTINGS::settings.aa_pitch_type == 3)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "FAKE UP");
		if (SETTINGS::settings.aa_pitch_type == 4)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "FAKE ZERO");
		if (SETTINGS::settings.aa_pitch_type == 5)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "NOSPREAD");
		if (SETTINGS::settings.aa_pitch_type == 6)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "HALF-PITCH");
		if (SETTINGS::settings.aa_pitch_type == 7)
			RENDER::DrawF(5, screen_height / 2, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "MINIMAL");

		if (SETTINGS::settings.aa_real1_type == 0)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "NO FAKE");
		if (SETTINGS::settings.aa_real1_type == 1)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "BACK JITTER");
		if (SETTINGS::settings.aa_real1_type == 2)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "RANDOM");
		if (SETTINGS::settings.aa_real1_type == 3)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "LOCAL WIEW");
		if (SETTINGS::settings.aa_real1_type == 4)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "OPPOSITE");
		if (SETTINGS::settings.aa_real1_type == 5)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "ROTATE");
		if (SETTINGS::settings.aa_real1_type == 6)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "180Z");
		if (SETTINGS::settings.aa_real1_type == 7)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "180TREEHOUSE");
		if (SETTINGS::settings.aa_real1_type == 8)
			RENDER::DrawF(5, screen_height / 2 + 50, FONTS::visuals_lby_font, false, false, CColor(147, 189, 29), "LBY BREAKER");

	}
	if (SETTINGS::settings.rifk_arrow)
	{
		if (!in_tp)
		{
			auto client_viewangles = Vector();
			INTERFACES::Engine->GetViewAngles(client_viewangles);
			const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
			//float aa_arrow_width = SETTINGS::settings.aa_arrows_width;
			//
			constexpr auto radius = 80.f;
			auto draw_arrow = [&](float rot, CColor color) -> void
			{
				std::vector<SDK::Vertex_t> vertices;
				vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
			///	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(aa_arrow_width)) * (radius - aa_arrow_height), screen_center.y + sinf(rot + DEG2RAD(aa_arrow_width)) * (radius - aa_arrow_height))));
			//	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(aa_arrow_width)) * (radius - aa_arrow_height), screen_center.y + sinf(rot - DEG2RAD(aa_arrow_width)) * (radius - aa_arrow_height))));
			//	RENDER::TexturedPolygon(3, vertices, color);
			};

			static auto alpha = 0.f; static auto plus_or_minus = false;
			if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
			alpha += plus_or_minus ? (255.f / 7 * 0.30) : -(255.f / 7 * 0.30); alpha = clamp(alpha, 0.f, 255.f);

			auto lby_color = CColor(0, 0, 255, alpha);
			const auto lby_rot = DEG2RAD(client_viewangles.y - local_player->GetLowerBodyYaw() - 90);
			draw_arrow(lby_rot, lby_color);

			auto fake_color = CColor(255, 0, 0, alpha);
			const auto fake_rot = DEG2RAD(client_viewangles.y - GLOBAL::fake_angles.y - 90);
			draw_arrow(fake_rot, fake_color);


			auto real_color = CColor(0, 255, 0, alpha);
			const auto real_rot = DEG2RAD(client_viewangles.y - GLOBAL::real_angles.y - 90);
			draw_arrow(real_rot, real_color);
		}
		else
		{
			Vector src3D, dst3D, forward, src, dst;
			SDK::trace_t tr;
			SDK::Ray_t ray;
			SDK::CTraceFilter filter;

			filter.pSkip1 = local_player;

			MATH::AngleVectors(Vector(0, local_player->GetLowerBodyYaw(), 0), &forward);
			src3D = local_player->GetVecOrigin();
			dst3D = src3D + (forward * 42.f); //replace 50 with the length you want the line to have 

			ray.Init(src3D, dst3D);

			INTERFACES::Trace->TraceRay(ray, 0, &filter, &tr);

			if (!RENDER::WorldToScreen(src3D, src) || !RENDER::WorldToScreen(tr.end, dst))
				return;

			RENDER::DrawLine(src.x, src.y, dst.x, dst.y, CColor(0, 0, 255, 255));

			MATH::AngleVectors(Vector(0, GLOBAL::fake_angles.y, 0), &forward);
			dst3D = src3D + (forward * 42.f); //replace 50 with the length you want the line to have 

			ray.Init(src3D, dst3D);

			INTERFACES::Trace->TraceRay(ray, 0, &filter, &tr);

			if (!RENDER::WorldToScreen(src3D, src) || !RENDER::WorldToScreen(tr.end, dst))
				return;

			RENDER::DrawLine(src.x, src.y, dst.x, dst.y, CColor(255, 0, 0, 255));

			MATH::AngleVectors(Vector(0, GLOBAL::real_angles.y, 0), &forward);
			src3D = local_player->GetVecOrigin();
			dst3D = src3D + (forward * 42.f); //replace 50 with the length you want the line to have 

			ray.Init(src3D, dst3D);

			INTERFACES::Trace->TraceRay(ray, 0, &filter, &tr);

			if (!RENDER::WorldToScreen(src3D, src) || !RENDER::WorldToScreen(tr.end, dst))
				return;

			RENDER::DrawLine(src.x, src.y, dst.x, dst.y, CColor(0, 255, 0, 255));
		}
	}
	if (!SETTINGS::settings.aa_bool)
		return;

	//if (SETTINGS::settings.indicator_bool)
	{
		static float rainbow;
		rainbow += 0.003f;
		if (rainbow > 1.f) rainbow = 0.f;
		if (SETTINGS::settings.flip_bool)
		{
			RENDER::DrawF((screen_width / 2) - 40, screen_height / 2, FONTS::visuals_side_font, true, true, CColor().FromHSB(rainbow, 1.f, 1.f), "◀"); //green
			RENDER::DrawF((screen_width / 2) + 40, screen_height / 2, FONTS::visuals_side_font, true, true, CColor(255, 255, 255, 255), "►");
		}
		else
		{
			RENDER::DrawF((screen_width / 2) + 40, screen_height / 2, FONTS::visuals_side_font, true, true, CColor().FromHSB(rainbow, 1.f, 1.f), "►"); //green
			RENDER::DrawF((screen_width / 2) - 40, screen_height / 2, FONTS::visuals_side_font, true, true, CColor(255, 255, 255, 255), "◀");
		}
	}
}




void CVisuals::LogEvents()
{
	static bool convar_performed = false, convar_lastsetting;

	if (convar_lastsetting != SETTINGS::settings.info_bool)
	{
		convar_lastsetting = SETTINGS::settings.info_bool;
		convar_performed = false;
	}

	if (!convar_performed)
	{
		static auto developer = INTERFACES::cvar->FindVar("developer");
		developer->SetValue(1);
		static auto con_filter_text_out = INTERFACES::cvar->FindVar("con_filter_text_out");
		static auto con_filter_enable = INTERFACES::cvar->FindVar("con_filter_enable");
		static auto con_filter_text = INTERFACES::cvar->FindVar("con_filter_text");

		con_filter_text->SetValue(".     ");
		con_filter_text_out->SetValue("");
		con_filter_enable->SetValue(2);
		convar_performed = true;
	}
}

void CVisuals::ModulateWorld()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	static auto sv_skyname = INTERFACES::cvar->FindVar("sv_skyname");
	sv_skyname->nFlags &= ~FCVAR_CHEAT;

	static auto static_val = 100;
	auto night_val = SETTINGS::settings.daytimevalue;

	//if (SETTINGS::settings.nightmode)
	//{
		//if (static_val != night_val)
		//{
		//	static_val = night_val;
		//	for (auto i = INTERFACES::MaterialSystem->FirstMaterial(); i != INTERFACES::MaterialSystem->InvalidMaterial(); i = INTERFACES::MaterialSystem->NextMaterial(i))
		//	{
			//	auto pMaterial = INTERFACES::MaterialSystem->GetMaterial(i);
				////if (!pMaterial) continue;
//
			//	auto finalnightval = night_val / 100.f;
				//if (strstr(pMaterial->GetTextureGroupName(), "World"))
					//pMaterial->ColorModulate(finalnightval, finalnightval, finalnightval);
				//else if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
					//
			//}
			//::settings.nightmode = false;
			
		//}
	//}
}

void CVisuals::ModulateSky()
{
	static bool nightmode_performed = false, nightmode_lastsetting;

	if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame())
	{
		if (nightmode_performed)
			nightmode_performed = false;
		return;
	}

	if (nightmode_performed != SETTINGS::settings.night_bool)
	{
		nightmode_lastsetting = SETTINGS::settings.night_bool;
		nightmode_performed = false;
	}

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (!local_player)
	{
		if (nightmode_performed) nightmode_performed = false; 
		return;
	}

	if (nightmode_lastsetting != SETTINGS::settings.night_bool)
	{
		nightmode_lastsetting = SETTINGS::settings.night_bool;
		nightmode_performed = false;
	}

	if (!nightmode_performed)
	{

		//if (SETTINGS::settings.night_bool) sv_skyname->SetValue("sky_csgo_night02");
		//else sv_skyname->SetValue("vertigoblue_hdr");
	}
}

void CVisuals::DrawHitmarker()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	static int lineSize = 5;

	static float alpha = 0;
	float step = 255.f / 0.3f * INTERFACES::Globals->frametime;


	if ( GLOBAL::flHurtTime + 0.4f >= INTERFACES::Globals->curtime )
		alpha = 255.f;
	else
		alpha -= step;

	if ( alpha > 0 ) {
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		INTERFACES::Engine->GetScreenSize( screenSizeX, screenSizeY );

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;
		CColor col = CColor( 255, 255, 255, alpha );
		RENDER::DrawLine( screenCenterX - lineSize * 2, screenCenterY - lineSize * 2, screenCenterX - ( lineSize ), screenCenterY - ( lineSize ), col );
		RENDER::DrawLine( screenCenterX - lineSize * 2, screenCenterY + lineSize * 2, screenCenterX - ( lineSize ), screenCenterY + ( lineSize ), col );
		RENDER::DrawLine( screenCenterX + lineSize * 2, screenCenterY + lineSize * 2, screenCenterX + ( lineSize ), screenCenterY + ( lineSize ), col );
		RENDER::DrawLine( screenCenterX + lineSize * 2, screenCenterY - lineSize * 2, screenCenterX + ( lineSize ), screenCenterY - ( lineSize ), col );
	}
}

void CVisuals::DisableFlashDuration()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	local_player->SetFlashDuration(0.f);
}

void CVisuals::DrawBorderLines()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());
	if (!weapon) return;

	int screen_x;
	int screen_y;
	int center_x;
	int center_y;
	INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
	INTERFACES::Engine->GetScreenSize(center_x, center_y);
	center_x /= 2; center_y /= 2;

	if (local_player->GetIsScoped())
	{
		RENDER::DrawLine(0, center_y, screen_x, center_y, CColor(0, 0, 0, 255));
		RENDER::DrawLine(center_x, 0, center_x, screen_y, CColor(0, 0, 0, 255));
	}
}

void setClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)UTILS::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}
int kek = 0;
int autism = 0;
const char* clantagAnim[18] = {
	"         ", "        r", "       ru",
	"      rush", "     rush", "    rushh",
	"   rushho", "  rushhour", " rushhour.",
	"rushhour.cc", "r ", "ru  ",
	"rus   ", "rush    ", "rush n     ",
	"rush no      ", "rush now       ", "now        "
};
void CVisuals::Clantag()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	static size_t lastTime = 0;
	if (GetTickCount() > lastTime)
	{
		kek++;
		if (kek > 10) {
			autism++; if (autism > 18) autism = 0;
			setClanTag(clantagAnim[autism], "rushhour.cc");
			lastTime = GetTickCount() + 500;
		}

		if (kek > 11) kek = 0;
	}
}
void CVisuals::GameSense1()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	static size_t lastTime = 0;
	if (GetTickCount() > lastTime)
	{
		kek++;
		if (kek > 10) {
			autism++; if (autism > 20) autism = 0;
			setClanTag(clantagAnim[autism], "rushhour.cc");
			lastTime = GetTickCount() + 500;
		}

		if (kek > 11) kek = 0;
	}
}

CVisuals* visuals = new CVisuals();









































































































































































































































































