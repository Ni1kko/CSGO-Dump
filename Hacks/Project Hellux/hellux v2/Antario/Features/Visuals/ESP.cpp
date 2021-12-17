#include <Windows.h>
#include "ESP.h"
#include "../../Menu/config.h"
#include "..\..\SDK\ISurface.h"
#include "..\..\Utils\Math.h"
#include "..\Aimbot\Aimbot.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\SDK\IEngineTrace.h"
#include "../../SDK/ClientClass.h"

#include <algorithm>


float flPlayerAlpha[1024];
bool WorldToScreen(const Vector& world, Vector& screen) {
	auto screen_transform = [&world, &screen]() -> bool {
		static auto &matrix = g_pEngine->WorldToScreenMatrix();

		screen.x = matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3];
		screen.y = matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3];
		screen.z = 0.0f;

		float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

		if (w < 0.001f) {
			screen.x *= 100000;
			screen.y *= 100000;
			return false;
		}

		screen.x /= w;
		screen.y /= w;

		return true;
	};

	if (screen_transform()) {
		int w, h;
		g_pEngine->GetScreenSize(w, h);

		screen.x = (w / 2.0f) + (screen.x * w) / 2.0f;
		screen.y = (h / 2.0f) - (screen.y * h) / 2.0f;

		return true;
	}

	return false;
}

float StoredCurtimePlayer[1024];
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#include <minwindef.h>

void c_visuals::BoxESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(c_config::get().box_esp_color_r, c_config::get().box_esp_color_g, c_config::get().box_esp_color_b, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - ((width / 2) / 2);
		int x2 = pos.x - (width / 2);
		int h = height;
		int w = width / 2;

		int iw = w / 3.5;
		int ih = (h / 3.5) - 1;
		bool IsTeammate = Entity->GetTeam() == Globals::LocalPlayer->GetTeam();
		bool IsEnemy = Entity->GetTeam() != Globals::LocalPlayer->GetTeam();
		bool IsLocal = Entity == Globals::LocalPlayer;

		if (IsEnemy) {

				g_pSurface->OutlinedRect(x2, y, width, height, ESP);
				g_pSurface->OutlinedRect(x2 - 1, y - 1, width + 2, height + 2, ESP2);
				g_pSurface->OutlinedRect(x2 + 1, y + 1, width - 2, height - 2, ESP2);
		}
		
	}
}

void c_visuals::NameESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	PlayerInfo_s EntityInfo;
	g_pEngine->GetPlayerInfo(Entity->EntIndex(), &EntityInfo);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(c_config::get().name_esp_color_r, c_config::get().name_esp_color_g, c_config::get().name_esp_color_b, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		g_pSurface->DrawT(x + (width / 2), y - 14, ESP, Globals::CourierNew, true, EntityInfo.szName);
	}
}

mstudiobbox_t* get_hitbox(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}
Vector GetHitboxPosition(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitbox(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}


bool IsOnScreen(Vector origin, Vector& screen)
{
	if (!WorldToScreen(origin, screen)) return false;
	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
	bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
	return xOk && yOk;
}

void TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color)
{
	static int texture_id = g_pSurface->CreateNewTextureID(true); // 
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1); //
	g_pSurface->DrawSetColor(color); //
	g_pSurface->DrawSetTexture(texture_id); //
	g_pSurface->DrawTexturedPolygon(n, vertice.data()); //
}

#include <map>

void c_visuals::PlayerIndicator(C_BaseEntity* entity)
{
	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return;

	Vector screenPos;

	auto client_viewangles = Vector();
	auto screen_width = 0, screen_height = 0;

	g_pEngine->GetViewAngles(client_viewangles);
	g_pEngine->GetScreenSize(screen_width, screen_height);

	auto radius = 450.f;
	Vector local_position = local_player->GetOrigin() + local_player->m_vecViewOffset();

	const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
	const auto rot = DEG2RAD(client_viewangles.y - g_Math.CalcAngle(local_position, GetHitboxPosition(entity, HitboxList::HITBOX_BODY)).y - 90);

	std::vector<Vertex_t> vertices;

	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - 16))));
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - 16))));

	TexturedPolygon(3, vertices, Color(255, 0, 0, 255)); //255, 40, 230
}
bool DisplayAmmo[65];
void c_visuals::WeaponESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	spectators();
	auto Weapon = Entity->GetActiveWeapon();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(c_config::get().weapon_esp_color_r, c_config::get().weapon_esp_color_g, c_config::get().weapon_esp_color_b, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		if (!Weapon)
			return;

		auto weapon_data = Weapon->GetCSWpnData();

		auto item_definition_index = Weapon->GetItemDefinitionIndex();

		if (!weapon_data)
			return;
	
		std::string WeaponName = weapon_data->hud_name;
		WeaponName.erase(0, 13);
		
		const auto xposition = x + (width / 2);
		const auto yposition = y + height + 1;

		bool IsTeammate = Entity->GetTeam() == Globals::LocalPlayer->GetTeam();
		bool IsEnemy = Entity->GetTeam() != Globals::LocalPlayer->GetTeam();
		bool IsLocal = Entity == Globals::LocalPlayer;
		int TypeOfEntity;
		if (IsLocal) {
			
		}
		else if (IsEnemy) {

		}
		else if (IsTeammate) {

		}
		auto add = c_config::get().ammo_enemy && DisplayAmmo[Entity->EntIndex()] ? 5 : 0;
		auto DrawWeaponIcon = [Weapon, xposition, yposition, IsLocal, IsEnemy, IsTeammate, add, ESP]() -> bool
		{
			char icon_letter = '\0';

			if (IsLocal && !c_config::get().show_icon_when_possible_local) {
				return false;
			}
			else if (IsTeammate && !c_config::get().show_icon_when_possible_teammates && !(IsLocal)) {
				return false;
			}
			else if (IsEnemy && !c_config::get().show_icon_when_possible_enemies) {
				return false;
			}

			switch (reinterpret_cast<C_BaseCombatWeapon*>(Weapon)->GetItemDefinitionIndex())
			{
			case ItemDefinitionIndex::WEAPON_DEAGLE:
				icon_letter = 'F';
				break;
			case ItemDefinitionIndex::WEAPON_KNIFE:
			case ItemDefinitionIndex::WEAPON_KNIFE_T:
				icon_letter = 'J';
				break;
			case ItemDefinitionIndex::WEAPON_AUG:
				icon_letter = 'E';
				break;

			case ItemDefinitionIndex::WEAPON_G3SG1:
				icon_letter = 'I';
				break;
			case ItemDefinitionIndex::WEAPON_MAC10:
				icon_letter = 'L';
				break;
			case ItemDefinitionIndex::WEAPON_P90:
				icon_letter = 'M';
				break;
			case ItemDefinitionIndex::WEAPON_SSG08:
				icon_letter = 'N';
				break;
			case ItemDefinitionIndex::WEAPON_SCAR20:
				icon_letter = 'O';
				break;
			case ItemDefinitionIndex::WEAPON_UMP45:
				icon_letter = 'Q';
				break;
			case ItemDefinitionIndex::WEAPON_ELITE:
				icon_letter = 'S';
				break;
			case ItemDefinitionIndex::WEAPON_FAMAS:
				icon_letter = 'T';
				break;
			case ItemDefinitionIndex::WEAPON_FIVESEVEN:
				icon_letter = 'U';
				break;
			case ItemDefinitionIndex::WEAPON_GALILAR:
				icon_letter = 'V';
				break;
			case ItemDefinitionIndex::WEAPON_M4A1:
				icon_letter = 'W';
				break;
			case ItemDefinitionIndex::WEAPON_P250:
				icon_letter = 'Y';
				break;
			case ItemDefinitionIndex::WEAPON_M249:
				icon_letter = 'Z';
				break;
			case ItemDefinitionIndex::WEAPON_XM1014:
				icon_letter = ']';
				break;
			case ItemDefinitionIndex::WEAPON_C4:
				icon_letter = 'd';
				break;
			case ItemDefinitionIndex::WEAPON_GLOCK:
				icon_letter = 'C';
				break;
			case ItemDefinitionIndex::WEAPON_HKP2000:
				icon_letter = 'Y';
				break;
			default:
				return false;
			}

			char buffer[2] = { icon_letter, '\0' };


			g_pSurface->DrawT(xposition, yposition - 2 + add, ESP, Globals::WeaponIcon, true, buffer);


			return true;
		};


		if (!DrawWeaponIcon())
		{
			std::transform(WeaponName.begin(), WeaponName.end(), WeaponName.begin(), ::toupper);
			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, Globals::WeaponESP, true, WeaponName.c_str());
		}

	}
}

void c_visuals::Ammo(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	spectators();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(81, 140, 201, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	auto Weapon = Entity->GetActiveWeapon();
	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top) && Weapon)
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width2 = height / 2;
		int x = pos.x - (width2 / 2);

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * 3) / 100);

		auto animLayer = Entity->GetAnimOverlay4(1);

		if (!animLayer)
			return;

		auto activity = Entity->GetSequenceActivity(animLayer->m_nSequence);

		if (!activity)
			return;

		int iClip = Weapon->GetAmmo();
		int iClipMax = Weapon->GetCSWpnData()->max_clip;

		if (!animLayer || !activity) {
			DisplayAmmo[Entity->EntIndex()] = false;
			return;
		}
			

		float box_w = (float)fabs(height / 2);
		float width;

		DisplayAmmo[Entity->EntIndex()] = iClip > 0 || (activity == 967 && animLayer->m_flWeight != 0.f);

		if (activity == 967 && animLayer->m_flWeight != 0.f)
		{
			float cycle = animLayer->m_flCycle;
			width = (((box_w * cycle) / 1.f));
		}
		else
			width = (((box_w * iClip) / iClipMax));

		if (!DisplayAmmo[Entity->EntIndex()])
			return;

		g_pSurface->FilledRect(x, y + height + 2, box_w, 4, ESP2);
		g_pSurface->FilledRect(x, y + height + 2, width, 4, Color(ESP));
		g_pSurface->OutlinedRect(x, y + height + 2, box_w, 4, ESP2);
	}
}

void c_visuals::HealthESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 255, 0, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);


	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;
		auto health = Entity->GetHealth();

		int Style = c_config::get().health_enemies;

		if (Style == 1)/*Whats a switch statement?*/ {
			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			g_pSurface->FilledRect(x - 6, y, 4, (height / 100.f) * health, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);
		}
		else if (Style == 2) {
			float flBoxes = std::ceil(Entity->GetHealth() / 10.f);
			float flX = x - 6; float flY = y - 1;
			float flHeight = height / 10.f;
			float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;

			g_pSurface->FilledRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->OutlinedRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->FilledRect(flX + 1, flY, 2, flHeight * flBoxes + 1, ESP);

			for (int i = 0; i < 10; i++)
				g_pSurface->Line(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, ESP2);
		}

	}
}
inline void CrossProduct2(const Vector& a, const Vector& b, Vector& result)

{

	CHECK_VALID(a);

	CHECK_VALID(b);

	Assert(&a != &result);

	Assert(&b != &result);

	result.x = a.y*b.z - a.z*b.y;

	result.y = a.z*b.x - a.x*b.z;

	result.z = a.x*b.y - a.y*b.x;

}

void CalcAngle2(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D(); //delta.Length
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}



#include "../../SDK/vector2d.h"
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
void rotate(std::array< vec2_t, 3 >& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = DEG2RAD(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}
float meme = 0.f;
int arrow_alpha[65];
template <typename t> t clamp_value(t value, t min, t max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}

void arrows(C_BaseEntity* player) {
	auto local = Globals::LocalPlayer;

	// another case of unnecessary float divison. gotta watch out for that guys! ~chance
//	const int fade = ( int )( ( ( 1 / 0.25f ) * interfaces::globals->frame_time ) * 255 );
	const int fade = (int)((4 * g_pGlobalVars->frametime) * 255);



//	if (!elements::get().vis_arrows)
//		return;

	if (!local)
		return;

	if (!local->IsAlive())
		return;

	if (!player)
		return;

	if (player->IsDormant())
		return;

	arrow_alpha[player->EntIndex()] = 255;
	Vector position;
	if (WorldToScreen(GetHitboxPosition(player, 2), position) && !c_config::get().fov_arrows_always_indicate)
		return;

	Vector viewangles;
	int width, height;

	g_pEngine->GetViewAngles(viewangles);
	g_pEngine->GetScreenSize(width, height);

	// more float division, i'm just gonna stop putting comments next to it at this point xD. ~chance
//	const auto screen_center = vec2_t( width / 2.f, height / 2.f );
	const auto screen_center = vec2_t(width * .5f, height * .5f);

	const auto angle_yaw_rad = DEG2RAD(viewangles.y - g_Math.CalcAngle(local->GetEyePosition(), GetHitboxPosition(player, 2)).y - 90);

	int radius = c_config::get().fov_arrows_enemy_distance;
	int size = c_config::get().fov_arrows_enemy_size;

	const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
	const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

	Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()]) : Color(c_config::get().fov_arrows_esp_color_r, c_config::get().fov_arrows_esp_color_g, c_config::get().fov_arrows_esp_color_b, flPlayerAlpha[player->EntIndex()]);

	Color clr = ESP;

	std::array< vec2_t, 3 >points{ vec2_t(new_point_x - size, new_point_y - size),
		vec2_t(new_point_x + size, new_point_y),
		vec2_t(new_point_x - size, new_point_y + size) };

	rotate(points, viewangles.y - g_Math.CalcAngle(local->GetEyePosition(), GetHitboxPosition(player, 2)).y - 90);
	g_pSurface->draw_filled_triangle(points, clr);
}
#include "..\..\SDK\ICvar.h"
enum cvar_flags {
	fcvar_none = 0,
	fcvar_unregistered = (1 << 0),
	fcvar_developmentonly = (1 << 1),
	fcvar_gamedll = (1 << 2),
	fcvar_clientdll = (1 << 3),
	fcvar_hidden = (1 << 4),
	fcvar_protected = (1 << 5),
	fcvar_sponly = (1 << 6),
	fcvar_archive = (1 << 7),
	fcvar_notify = (1 << 8),
	fcvar_userinfo = (1 << 9),
	fcvar_printableonly = (1 << 10),
	fcvar_unlogged = (1 << 11),
	fcvar_never_as_string = (1 << 12),
	fcvar_replicated = (1 << 13),
	fcvar_cheat = (1 << 14),
	fcvar_ss = (1 << 15),
	fcvar_demo = (1 << 16),
	fcvar_dontrecord = (1 << 17),
	fcvar_ss_added = (1 << 18),
	fcvar_release = (1 << 19),
	fcvar_reload_materials = (1 << 20),
	fcvar_reload_textures = (1 << 21),
	fcvar_not_connected = (1 << 22),
	fcvar_material_system_thread = (1 << 23),
	fcvar_archive_xbox = (1 << 24),
	fcvar_accessible_from_threads = (1 << 25),
	fcvar_server_can_execute = (1 << 28),
	fcvar_server_cannot_query = (1 << 29),
	fcvar_clientcmd_can_execute = (1 << 30),
	fcvar_meme_dll = (1 << 31),
	fcvar_material_thread_mask = (fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread)
};


void c_visuals::spectators() {
	if (!c_config::get().visual_spectator_list)
		return;

	if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) {
		int index = 0;
		int w, h;
		g_pEngine->GetScreenSize(w, h);

		for (int i = 1; i < 65; i++) {
			auto p_local = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()); // what's csgo::local_player???????
			auto p_entity = g_pEntityList->GetClientEntity(i);
			PlayerInfo_t e_info;

			if (p_entity && p_entity != p_local) {
				g_pEngine->GetPlayerInfo(i, &e_info);
				if (!p_entity->IsAlive() && !p_entity->IsDormant()) {
					//HANDLE target = p_entity->ObserverTarget();
				}
			}
		}
	}
}

void c_visuals::force_crosshair()
{
	auto local_player = Globals::LocalPlayer;

	static ConVar* weapon_debug_spread_show = g_pCvar->FindVar("weapon_debug_spread_show");

	weapon_debug_spread_show->nFlags &= ~fcvar_cheat;

	if (local_player && local_player->GetHealth() > 0)
	{
		weapon_debug_spread_show->SetValue(local_player->IsScoped() || !c_config::get().force_crosshair ? 0 : 3);
	}
}

void c_visuals::flags(C_BaseEntity* player) {

	Vector min, max;
	player->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = player->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(255, 255, 255, flPlayerAlpha[player->EntIndex()] / 1.3);

	Color Money = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(115, 180, 25, flPlayerAlpha[player->EntIndex()] / 1.3);

	Color Zoom = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(53, 166, 208, flPlayerAlpha[player->EntIndex()] / 1.3);

	Color FakeDuck = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(255, 0, 0, flPlayerAlpha[player->EntIndex()] / 1.3);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;

		auto fakeDuck = [&]() -> bool
		{
			static float storedTick;
			static float crouchedTicks[65];

			if (!player->AnimState())
				return false;

			float m_flDuckAmount = player->AnimState()->m_fDuckAmount;
			float m_flDuckSpeed = player->m_fDuckSpeed();
			float m_fFlags = player->GetFlags();

			if (m_flDuckSpeed != 0.f && m_flDuckAmount != 0.f)
			{
				if (m_flDuckSpeed == 8.f && m_flDuckAmount <= 0.9f && m_flDuckAmount > 0.01f)
				{
					if (storedTick != g_pGlobalVars->tickcount)
					{
						crouchedTicks[player->EntIndex()] = crouchedTicks[player->EntIndex()] + 1;
						storedTick = g_pGlobalVars->tickcount;
					}
					return (crouchedTicks[player->EntIndex()] >= 5);
				}
				else
					crouchedTicks[player->EntIndex()] = 0;
			}

			return false;
		};

		if (c_config::get().flags_enemy) {
			std::vector<std::pair<std::string, Color>> flags;

			if (player->Money())
				flags.push_back(std::pair<std::string, Color>(std::string("$").append(std::to_string(player->Money())), Money));

			if (player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("HK", ESP));
			else if (!player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("K", ESP));

			if (player->IsScoped())
				flags.push_back(std::pair<std::string, Color>(std::string("ZOOM"), Zoom));

			if (fakeDuck())
				flags.push_back(std::pair<std::string, Color>(std::string("FAKEDUCK"), FakeDuck));

			int pos = 0;
			for (auto text : flags) {
				g_pSurface->DrawT(x + width + 2, y + pos, text.second, Globals::WeaponESP, false, text.first.c_str());
				pos += 10;
			}
		}
	}
}
RECT GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	g_pEngine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void DrawWeapon(C_BaseEntity* player) {
	Vector min, max;
	player->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = player->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - ((width / 2) / 2);
		int x2 = pos.x - (width / 2);
		int h = height;
		int w = width / 2;

		g_pSurface->OutlinedRect(x2, y, width, height, Color(255, 255, 255));
		g_pSurface->OutlinedRect(x2 - 1, y - 1, width + 2, height + 2, Color(0, 0, 0));
		g_pSurface->OutlinedRect(x2 + 1, y + 1, width - 2, height - 2, Color(0, 0, 0));
	}
}

void c_visuals::DrawPlayers() {

	force_crosshair();
	int spectator_count = 0;
	DrawRadarBackground();
	for (int Index = 1; Index < g_pGlobalVars->maxClients; Index++)
	{
		auto Entity = g_pEntityList->GetClientEntity(Index);
		auto LocalPlayer = Globals::LocalPlayer;

		if (Entity && !Entity->IsDormant())
			worldWeapon(Entity);

		if (!Entity || !LocalPlayer || !Entity->IsAlive())
			continue;

		

		ClientClass* cClass = (ClientClass*)Entity->GetClientClass();
		if (cClass->ClassID != (int)EClassIds::CBaseWeaponWorldModel && ((strstr(cClass->pNetworkName, "Weapon") || cClass->ClassID == (int)EClassIds::CDEagle || cClass->ClassID == (int)EClassIds::CAK47))) {
			DrawWeapon(Entity);
		}

		bool IsTeammate = Entity->GetTeam() == LocalPlayer->GetTeam();
		bool IsEnemy = Entity->GetTeam() != LocalPlayer->GetTeam();
		bool IsLocal = Entity == LocalPlayer;

		if (!Entity->IsDormant()) {
			StoredCurtimePlayer[Entity->EntIndex()] = g_pGlobalVars->curtime;
		}

		if (Entity->IsDormant() && flPlayerAlpha[Entity->EntIndex()] > 0 &&  g_pGlobalVars->curtime - StoredCurtimePlayer[Entity->EntIndex()] > 2)
		{
			flPlayerAlpha[Entity->EntIndex()] -= 5;
		}
		else if (flPlayerAlpha[Entity->EntIndex()] < 255 && !(Entity->IsDormant()))
		{
			flPlayerAlpha[Entity->EntIndex()] += 5;
		}
		
		spectators();
		if (IsEnemy) {

			DrawRadar(Entity);
			if (c_config::get().name_enemies)	NameESP(Entity);
			if (c_config::get().box_enemies)	BoxESP(Entity);
			if (c_config::get().weapon_enemies)	WeaponESP(Entity);
			if (c_config::get().ammo_enemy)	Ammo(Entity);
			if (c_config::get().health_enemies > 0)	HealthESP(Entity);
			if (c_config::get().fov_arrows_enemy) arrows(Entity);
			flags(Entity);
		}
	}
}



template<class T>
constexpr const T& clampv2(const T& v, const T& lo, const T& hi)
{
	return clampv2(v, lo, hi, std::less<>());
}

template<class T, class Compare>
constexpr const T& clampv2(const T& v, const T& lo, const T& hi, Compare comp)
{
	return assert(!comp(hi, lo)),
		comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

void c_visuals::draw_scope() {

	if (!c_config::get().remove_scope || !c_config::get().visuals_enabled)
		return;

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;

	auto local_player = Globals::LocalPlayer;

	if (!local_player)
		return;

	if (!local_player->IsAlive())
		return;

	int width_2, height_2;

	g_pEngine->GetScreenSize(width_2, height_2);

	if (local_player->IsScoped()) {
		auto weapon = local_player->GetActiveWeapon();

		if (!weapon)
			return;

		float spread = weapon->GetInaccuracy() * 100;
		int height = clampv2(spread, 1.f, 30.f);
		int alpha = 255 - (height * 4.2f);

		bool Dynamic = c_config::get().dynamic_scope;

		if (Dynamic) {
			g_pSurface->FilledRect(0, height_2 / 2 - (height / 2), width_2, height, Color(0, 0, 0, alpha));
			g_pSurface->FilledRect(width_2 / 2 - (height / 2), 0, height, height_2, Color(0, 0, 0, alpha));
		}
		else {
			g_pSurface->Line(0, height_2 / 2, width_2, height_2 / 2, Color(0, 0, 0, 255));
			g_pSurface->Line(width_2 / 2, 0, width_2 / 2, height_2, Color(0, 0, 0, 255));
		}

	}
}
int RadarX = 110;
int RadarY = 110;
int RadarSize = 100;
void c_visuals::DrawRadarPoint(Vector vOriginX, Vector vOriginY, QAngle qAngle, C_BaseEntity *pBaseEntity, Color col)
{
	auto pLocalEntity = Globals::LocalPlayer;
	if (pLocalEntity == NULL)
		return;

	float flDeltaX = vOriginX.x - vOriginY.x;
	float flDeltaY = vOriginX.y - vOriginY.y;

	float flAngle = qAngle.y;

	float flYaw = (flAngle)* (3.14159265358979323846 / 180.0);
	float flMainViewAngles_CosYaw = cos(flYaw);
	float flMainViewAngles_SinYaw = sin(flYaw);

	// rotate
	float x = flDeltaY * (-flMainViewAngles_CosYaw) + flDeltaX * flMainViewAngles_SinYaw;
	float y = flDeltaX * (-flMainViewAngles_CosYaw) - flDeltaY * flMainViewAngles_SinYaw;

	float flRange = 2000;

	if (fabs(x) > flRange || fabs(y) > flRange)
	{
		// clipping
		if (y > x)
		{
			if (y > -x)
			{
				x = flRange * x / y;
				y = flRange;
			}
			else
			{
				y = -flRange * y / x;
				x = -flRange;
			}
		}
		else
		{
			if (y > -x)
			{
				y = flRange * y / x;
				x = flRange;
			}
			else
			{
				x = -flRange * x / y;
				y = -flRange;
			}
		}
	}

	int	iScreenX = RadarX + int(x / flRange * RadarSize);
	int iScreenY = RadarY + int(y / flRange * RadarSize);

	g_pSurface->FilledRect(iScreenX - 3, iScreenY - 3, 7, 7, Color(0, 0, 0, 255));



	g_pSurface->FilledRect(iScreenX - 2, iScreenY - 2, 5, 5, Color(255, 255, 255));

}
#define M_PI		3.14159265358979323846f
Vector RotatePoint(Vector p, Vector c, float ang)
{
	float angleInRadians = ang * (M_PI / 180.f);
	float cosTheta = cos(angleInRadians);
	float sinTheta = sin(angleInRadians);
	return Vector(
		(cosTheta * (p.x - c.x) -
			sinTheta * (p.y - c.y) + c.x),
			(sinTheta * (p.x - c.x) +
				cosTheta * (p.y - c.y) + c.y),
		0);
}
void GradientV2(int x, int y, int w, int h, Color c1, Color c2)
{

	g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x, y + i, w, 1, Color(first, second, third, ia));
	}
}
//===================================================================================
int RadarSizeFinal = 150;
void GradientH2(int x, int y, int w, int h, Color c1, Color c2)
{

	g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x + i, y, 1, h, Color(first, second, third, ia));
	}

}
#include "../../Menu/Menu.h"
static Vector TL(110, 110, 0);
void c_visuals::DrawRadarBackground()
{
	if (!c_config::get().visuals_radar)
		return;

	static int W, H;
	static int centerW, centerh, topH;
	centerW = W / 2;
	centerh = H / 2;
	g_pEngine->GetScreenSize(W, H);
	 // pos
	Vector TR(TL.x + RadarSizeFinal, TL.y, 0);
	Vector BL(TL.x, TL.y + RadarSizeFinal, 0);
	Vector BR(TL.x + RadarSizeFinal, TL.y + RadarSizeFinal, 0);
	
	Vector2D _mouse_pos = g_pSurface->GetMousePosition();

	static Vector2D oldPos;
	static Vector2D mousePos;
	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;
	static int _drag_x = 300;
	static int _drag_y = 300;

	if (g_Menu.menuOpened)
	{
		//MenuAlpha_Main = 255;
		//MenuAlpha_Text = 255;

		if (GetAsyncKeyState(VK_LBUTTON))
			_click = true;

		Vector2D _mouse_pos = g_pSurface->GetMousePosition();

		if (_dragging && !_click)
			_dragging = false;

		if (_resizing && !_click)
			_resizing = false;

		if (_dragging && _click)
		{
			TL.x = _mouse_pos.x - _drag_x;
			TL.y = _mouse_pos.y - _drag_y;
		}

		if (g_pSurface->MouseInRegion(TL.x, TL.y, RadarSizeFinal, RadarSizeFinal) && !(g_pSurface->MouseInRegion(
			TL.x + 10, TL.y + 20, RadarSizeFinal - 10, RadarSizeFinal - 10)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - TL.x;
			_drag_y = _mouse_pos.y - TL.y;
		}
	}

	GradientH2(TL.x, TL.y, RadarSizeFinal, RadarSizeFinal, Color(29, 29, 37, 255), Color(49, 49, 57, 255));

	g_pSurface->FilledRect(TL.x + 1, TL.y + 20, RadarSizeFinal - 2, RadarSizeFinal - 20, Color(23, 23, 30, 255));


	for (int i = 0; i < RadarSizeFinal / 2; i++) {
		g_pSurface->Line(TL.x + (i * 2), TL.y + 20, TL.x + (i * 2), TL.y + RadarSizeFinal, Color(10, 10, 18, 255));
	}

	for (int i = 0; i < RadarSizeFinal / 2.4; i++) {
		g_pSurface->Line(TL.x, TL.y + 22 + (i * 2), TL.x + RadarSizeFinal, TL.y + 22 + (i * 2), Color(10, 10, 18, 255));
	}

	g_pSurface->OutlinedRect(TL.x, TL.y, RadarSizeFinal, RadarSizeFinal, Color(52,73,94, 255));
	g_pSurface->OutlinedRect(TL.x, TL.y + 19, RadarSizeFinal, 1, Color(52,73,94, 255));

	RECT TitleText_Size = g_pSurface->GetTextSizeRect(Globals::CourierNew, "Radar");

	g_pSurface->DrawT(TL.x + 3, TL.y + (20 / 2) - (TitleText_Size.bottom / 2), Color(143, 143, 143, 255), Globals::CourierNew, false, "Radar");
}

void c_visuals::DrawRadar(C_BaseEntity *pEntity)
{
	if (!c_config::get().visuals_radar)
		return;

	static int W, H;
	int centerW, centerh, topH;
	centerW = W / 2;
	centerh = H / 2;
	g_pEngine->GetScreenSize(W, H);
	Vector TR(TL.x + RadarSizeFinal, TL.y, 0);
	Vector BL(TL.x, TL.y + RadarSizeFinal, 0);
	Vector BR(TL.x + RadarSizeFinal, TL.y + RadarSizeFinal, 0);

	if (!pEntity || !Globals::LocalPlayer)
		return;
	Color ESP = pEntity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[pEntity->EntIndex()]) : Color(c_config::get().box_esp_color_r, c_config::get().box_esp_color_g, c_config::get().box_esp_color_b, flPlayerAlpha[pEntity->EntIndex()]);
	Vector center = Vector((TL.x + TR.x) / 2, (TL.y + BL.y) / 2, 0.f);
	auto m_local = Globals::LocalPlayer;
			Color c = pEntity->GetTeam() == m_local->GetTeam() ? Color(50, 50, 255, 255) : Color(255, 50, 50, 255);
			Vector relPos = (pEntity->GetAbsOrigin() - m_local->GetAbsOrigin()) * 0.05f;
			relPos.y *= -1.f;
			Vector drawAt = Vector(relPos.x + center.x, relPos.y + center.y, 0);
			Vector localAng;
			g_pEngine->GetViewAngles(localAng);
			drawAt = RotatePoint(drawAt, center, localAng.y - 90);
			if (drawAt.x < TR.x && drawAt.x > TL.x && drawAt.y < BL.y && drawAt.y > TL.y + 25)
				g_pSurface->FilledRect(drawAt.x - 2, drawAt.y - 2, 4, 4, ESP);
}
auto spectator_count = 0;
bool WorldToScreenAlpha(const Vector &origin, Vector2D &screen)
{
	const auto screenTransform = [&origin, &screen]() -> bool
	{
		static std::uintptr_t pViewMatrix;
		if (!pViewMatrix)
		{
			pViewMatrix = static_cast<std::uintptr_t>(Utils::FindSignature("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			pViewMatrix += 3;
			pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
			pViewMatrix += 176;
		}

		const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
		screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
		screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];

		float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

		if (w < 0.001f)
		{
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	};

	if (!screenTransform())
	{
		int iScreenWidth, iScreenHeight;
		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

		screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
		screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

		return true;
	}
	return false;
}
struct
{
	//int left, right, top, bottom;
	int x, y, w, h;
} Box;
bool boundingBox(C_BaseEntity* pEntity, Vector offset = Vector(0, 0, 0))
{
	Vector min, max;
	pEntity->GetRenderBounds(min, max);

	Vector pos3D, top3D;
	Vector2D pos, top;
	pos3D = pEntity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 15);

	if (WorldToScreenAlpha(pos3D, pos) && WorldToScreenAlpha(top3D, top))
	{
		int height = (pos.y - top.y);
		Box.h = height; //h
		int width = Box.h / 2;
		Box.w = width; //w

		Box.x = pos.x - width / 2; //x
		Box.y = top.y; //y

		return true;
	}

	return false;
}

void c_visuals::worldWeapon(C_BaseEntity * pEntity)
{
	ClientClass* clientClass = pEntity->GetClientClass();

	if (!clientClass)
		return;

	if (!boundingBox(pEntity))
		return;

	auto owner = g_pEntityList->GetClientEntityFromHandle(pEntity->owner());

	if (!owner && (clientClass->ClassID != (int)EClassIds::CBaseWeaponWorldModel && (std::strstr(clientClass->pNetworkName, "Weapon") || clientClass->ClassID == (int)EClassIds::CDEagle || clientClass->ClassID == (int)EClassIds::CAK47)))
	{
		auto weapon = static_cast<C_BaseCombatWeapon*>(pEntity);

		if (!weapon)
			return;

		if (!weapon->ItemDefinitionIndex2())
			return;

		const auto info = weapon->GetCSWpnData();

		if (!info)
			return;

		//if (config->get_int("worldWeapon") & (1 << 1))
		//{

			std::string localised_name = info->hud_name;
			localised_name.erase(0, 13);

			std::transform(localised_name.begin(), localised_name.end(), localised_name.begin(), ::toupper);

			g_pSurface->DrawT(Box.x + (Box.w / 2), Box.y + Box.h - 11, Color(255, 255, 255), Globals::SmallText, true, localised_name.c_str());
		//}


	}
}

void GrenadePrediction(C_BaseEntity* localPlayer, C_BaseCombatWeapon* weapon)
{
	auto IsGrenade = [](ItemDefinitionIndex item)
	{
		if (item == ItemDefinitionIndex::WEAPON_FLASHBANG
			|| item == ItemDefinitionIndex::WEAPON_HEGRENADE
			|| item == ItemDefinitionIndex::WEAPON_SMOKEGRENADE
			|| item == ItemDefinitionIndex::WEAPON_MOLOTOV
			|| item == ItemDefinitionIndex::WEAPON_DECOY
			|| item == ItemDefinitionIndex::WEAPON_INCGRENADE)
			//|| item == ItemDefinitionIndex::weapon_tagrenade
			return true;
		else
			return false;
	};
	auto molotov_throw_detonate_time = g_pCvar->FindVar("molotov_throw_detonate_time");
	auto GetGrenadeDetonateTime = [molotov_throw_detonate_time](ItemDefinitionIndex item)
	{
		switch (item)
		{
		case ItemDefinitionIndex::WEAPON_FLASHBANG:
		case ItemDefinitionIndex::WEAPON_HEGRENADE:
			return 1.5f;
			break;
		case ItemDefinitionIndex::WEAPON_INCGRENADE:
		case ItemDefinitionIndex::WEAPON_MOLOTOV:
			return molotov_throw_detonate_time->GetFloat();
			break;
		//case ItemDefinitionIndex::weapon_tagrenade:
		//	return 5.f;
		//	break;
		}

		return 3.f;
	};

	auto DrawLine = [](Vector start, Vector end, int r, int g, int b, int a = 255)
	{
		Vector startw2s, endw2s;

		if (!Utils::WorldToScreen2(start, startw2s)
			|| !Utils::WorldToScreen2(end, endw2s))
			return;

		g_pSurface->DrawSetColor(r, g, b, a);
		g_pSurface->DrawLine(startw2s.x, startw2s.y, endw2s.x, endw2s.y);
	};

	auto PhysicsClipVelocity = [](const Vector& in, const Vector& normal, Vector& out, float overbounce)
	{
		int blocked = 0;
		float angle = normal[2];

		if (angle > 0)
			blocked |= 1; // floor

		if (!angle)
			blocked |= 2; // step

		float backoff = in.Dot(normal) * overbounce;

		for (int i = 0; i < 3; i++)
		{
			out[i] = in[i] - (normal[i] * backoff);

			if (out[i] > -0.1f && out[i] < 0.1f)
				out[i] = 0;
		}

		return blocked;
	};

	PlayerInfo_t playerInfo;
	auto itemIndex = weapon->GetItemDefinitionIndex();
	auto wpnData = weapon->GetCSWpnData();

	if (!wpnData
		|| !IsGrenade(itemIndex))
		return;

	Vector angThrow, forward;

	g_pEngine->GetViewAngles(angThrow);

	angThrow.x = g_Math.NormalizeYaw(angThrow.x);
	angThrow.x -= (90.f - abs(angThrow.x)) * 0.11111111f;

	g_Math.AngleVectors(angThrow, &forward);

	// WIP
}

#include "..\..\SDK\IVModelInfo.h"
