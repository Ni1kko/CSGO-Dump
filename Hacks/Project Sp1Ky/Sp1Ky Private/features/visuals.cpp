#include "visuals.hpp"
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../backtrack.h"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include <windows.h>
#include "../features/XorCompileTime.hpp"
vgui::HFont esp_font;
vgui::HFont defuse_font;
vgui::HFont dropped_weapons_font;
C_BasePlayer* local_observed;
float ESP_Fade[64];
struct
{
	C_BasePlayer* pl;
	bool          is_enemy;
	bool          is_visible;
	Color         clr;
	Vector        head_pos;
	Vector        feet_pos;
	RECT          bbox;
} esp_ctx;
RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();
	if (!collideable)
		return rect;
	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();
	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();
	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};
	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}
	Vector screen_points[8] = {};
	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}
	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;
	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}
//--------------------------------------------------------------------------------
bool Visuals::CreateFonts()
{
	esp_font = g_VGuiSurface->CreateFont_();
	defuse_font = g_VGuiSurface->CreateFont_();
	dropped_weapons_font = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(esp_font, XorStr("Arial"), 11, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyphSet(defuse_font, XorStr("Arial"), 15, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyphSet(dropped_weapons_font, XorStr("Arial"), 10, 700, 0, 0, FONTFLAG_DROPSHADOW);
	return true;
}
//--------------------------------------------------------------------------------
void Visuals::DestroyFonts()
{
}
bool Visuals::IsVisibleScan(C_BasePlayer *player)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	Vector eyePos = g_LocalPlayer->GetEyePos();

	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;

	if (!player->SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
	{
		return false;
	}

	auto studio_model = g_MdlInfo->GetStudiomodel(player->GetModel());
	if (!studio_model)
	{
		return false;
	}

	int scan_hitboxes[] = {
		HITBOX_HEAD,
		HITBOX_LEFT_FOREARM,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_LEFT_FOOT,
		HITBOX_RIGHT_FOOT,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF,
		HITBOX_CHEST,
		HITBOX_STOMACH
	};

	for (int i = 0; i < ARRAYSIZE(scan_hitboxes); i++)
	{
		auto hitbox = studio_model->GetHitboxSet(player->m_nHitboxSet())->GetHitbox(scan_hitboxes[i]);
		if (hitbox)
		{
			auto
				min = Vector{},
				max = Vector{};

			Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
			Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

			ray.Init(eyePos, (min + max) * 0.5);
			g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

			if (tr.hit_entity == player || tr.fraction > 0.97f)
				return true;
		}
	}
	return false;
}
void Visuals::GetTextSize(unsigned long font, const char *txt, int &width, int &height)
{
	FUNCTION_GUARD;

	size_t origsize = strlen(txt) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	int x, y;

	mbstowcs_s(&convertedChars, wcstring, origsize, txt, _TRUNCATE);

	g_VGuiSurface->GetTextSize(font, wcstring, x, y);

	width = x;
	height = y;
}
void Visuals::Spectators()
{
	RECT scrn = GetViewport();
	int cnt = 0;
	for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++)
	{

		C_BasePlayer *player = C_BasePlayer::GetPlayerByIndex(i);

		if (!player || player == nullptr)
			continue;

		player_info_t player_info;
		if (player != g_LocalPlayer)
		{
			if (g_EngineClient->GetPlayerInfo(i, &player_info) && !player->IsAlive() && !player->IsDormant())
			{
				auto observer_target = player->m_hObserverTarget();
				if (!observer_target)
					continue;

				auto target = observer_target.Get();
				if (!target)
					continue;

				player_info_t player_info2;
				if (g_EngineClient->GetPlayerInfo(target->EntIndex(), &player_info2))
				{
					char player_name[255] = { 0 };
					sprintf_s(player_name, "%s >> %s", player_info.szName, player_info2.szName);

					int w, h;
					GetTextSize(esp_font, player_name, w, h);

					g_VGuiSurface->DrawSetColor(Color(38, 39, 40, 140));
					DrawFilledRect(scrn.right - 260, (scrn.bottom / 2) + (16 * cnt), 260, 16);
					DrawString(esp_font, false, scrn.right - w - 4, (scrn.bottom / 2) + (16 * cnt), target->EntIndex() == g_LocalPlayer->EntIndex() ? Color(51, 153, 255) : Color(255, 255, 255, 255), player_name);
					++cnt;
				}
			}
		}
	}
}
RECT Visuals::GetViewport()
{
	RECT viewport = { 0, 0, 0, 0 };
	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	viewport.right = w; viewport.bottom = h;

	return viewport;
}
__inline void Visuals::DrawFilledRect(int x, int y, int w, int h)
{
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
}
void Visuals::DrawWatermark()
{
	DrawString(esp_font, 10, 10, Color(51, 153, 255), FONT_LEFT, XorStr("Sp1Ky | Premium CS:GO Cheat"));
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl)
{
	esp_ctx.pl = pl;
	esp_ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	esp_ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);
	if (!esp_ctx.is_enemy && g_Options.legit_only_enemy)
		return false;
	esp_ctx.clr = esp_ctx.is_enemy ? (esp_ctx.is_visible ? g_Options.legit_dog_dog_vis_col_en_vis : g_Options.legit_dog_dog_vis_col_entm_invs) : (esp_ctx.is_visible ? g_Options.legit_dog_dog_vis_col_tm : g_Options.legit_dog_dog_vis_col_tm_vis_cl);
	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();
	head.z += 15;
	if (!Math::WorldToScreen(head, esp_ctx.head_pos) ||
		!Math::WorldToScreen(origin, esp_ctx.feet_pos))
		return false;
	auto h = fabs(esp_ctx.head_pos.y - esp_ctx.feet_pos.y);
	auto w = h / 1.65f;
	esp_ctx.bbox.left = static_cast<long>(esp_ctx.feet_pos.x - w * 0.5f);
	esp_ctx.bbox.right = static_cast<long>(esp_ctx.bbox.left + w);
	esp_ctx.bbox.bottom = static_cast<long>(esp_ctx.feet_pos.y);
	esp_ctx.bbox.top = static_cast<long>(esp_ctx.head_pos.y);
	return true;
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox()
{
	g_VGuiSurface->DrawSetColor(esp_ctx.clr);
	g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left, esp_ctx.bbox.top, esp_ctx.bbox.right, esp_ctx.bbox.bottom);
	g_VGuiSurface->DrawSetColor(Color::Black);
	g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left - 1, esp_ctx.bbox.top - 1, esp_ctx.bbox.right + 1, esp_ctx.bbox.bottom + 1);
	g_VGuiSurface->DrawOutlinedRect(esp_ctx.bbox.left + 1, esp_ctx.bbox.top + 1, esp_ctx.bbox.right - 1, esp_ctx.bbox.bottom - 1);
}
//--------------------------------------------------------------------------------
void Visuals::SkyColor()
{
	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
	{
		IMaterial *pMaterial = g_MatSystem->GetMaterial(i);
		if (!pMaterial)
			continue;
		const char* group = pMaterial->GetTextureGroupName();
		const char* name = pMaterial->GetName();
		if (strstr(pMaterial->GetTextureGroupName(), XorStr("SkyBox")))
		{
		}
	}
}
void Visuals::NightMode()
{
	bool done = false;
	if (g_Options.nightmode)
	{
		static auto sv_skyname = g_CVar->FindVar(XorStr("sv_skyname"));
		static auto r_DrawSpecificStaticProp = g_CVar->FindVar(XorStr("r_DrawSpecificStaticProp"));
		r_DrawSpecificStaticProp->SetValue(1);
		sv_skyname->SetValue(XorStr("sky_csgo_night02b"));
		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_MatSystem->GetMaterial(i);
			if (!pMaterial)
				continue;
			const char* group = pMaterial->GetTextureGroupName();
			const char* name = pMaterial->GetName();
			if (strstr(pMaterial->GetTextureGroupName(), XorStr("SkyBox")))
			{
				pMaterial->ColorModulate(g_Options.sky_color[0] * 1, g_Options.sky_color[1] * 1, g_Options.sky_color[2] * 1);
			}
			if (strstr(group, XorStr("World textures")))
			{
				pMaterial->ColorModulate(g_Options.nightmode_intensity, g_Options.nightmode_intensity, g_Options.nightmode_intensity);
			}
			if (strstr(group, XorStr("StaticProp")))
			{
				pMaterial->ColorModulate(g_Options.nightmode_intensity, g_Options.nightmode_intensity, g_Options.nightmode_intensity);
			}
			if (strstr(name, XorStr("models/props/de_dust/palace_bigdome")))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			}
			if (strstr(name, XorStr("models/props/de_dust/palace_pillars")))
			{
				pMaterial->ColorModulate(g_Options.nightmode_intensity, g_Options.nightmode_intensity, g_Options.nightmode_intensity);
			}
			if (strstr(group, XorStr("Particle textures")))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			}
			done = true;
		}
	}
	else
	{
		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_MatSystem->GetMaterial(i);
			if (!pMaterial)
				continue;
			const char* group = pMaterial->GetTextureGroupName();
			const char* name = pMaterial->GetName();
			if (strstr(pMaterial->GetTextureGroupName(), XorStr("SkyBox")))
			{
				pMaterial->ColorModulate(1, 1, 1);
			}
			if (strstr(group, XorStr("World textures")))
			{
				pMaterial->ColorModulate(1, 1, 1);
			}
			if (strstr(group, XorStr("StaticProp")))
			{
				pMaterial->ColorModulate(1, 1, 1);
			}
			if (strstr(name, XorStr("models/props/de_dust/palace_bigdome")))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			}
			if (strstr(name, XorStr("models/props/de_dust/palace_pillars")))
			{
				pMaterial->ColorModulate(1, 1, 1);
			}
			if (strstr(group, XorStr("Particle textures")))
			{
				pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			}
		}
	}
}
//--------------------------------------------------------------------------------
bool Visuals::ValidPlayer(C_BasePlayer *player, bool count_step)
{
	int idx = player->EntIndex();
	constexpr float frequency = 0.35f / 0.5f;
	float step = frequency * g_GlobalVars->frametime;
	if (!player->IsAlive())
		return false;

	// Don't render esp if in firstperson viewing player.
	if (player == local_observed)
	{
		if (g_LocalPlayer->m_iObserverMode() == 4)
			return false;
	}

	if (player == g_LocalPlayer)
	{
		if (!g_Input->m_fCameraInThirdPerson)
			return false;
	}

	if (count_step)
	{
		if (!player->IsDormant()) {
			if (ESP_Fade[idx] < 1.f)
				ESP_Fade[idx] += step;
		}
		else {
			if (ESP_Fade[idx] > 0.f)
				ESP_Fade[idx] -= step;
		}
		ESP_Fade[idx] = (ESP_Fade[idx] > 1.f ? 1.f : ESP_Fade[idx] < 0.f ? 0.f : ESP_Fade[idx]);
	}

	return (ESP_Fade[idx] > 0.f);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName()
{
	wchar_t buf[128];
	player_info_t info = esp_ctx.pl->GetPlayerInfo();
	if (MultiByteToWideChar(CP_UTF8, 0, info.szName, -1, buf, 128) > 0) {
		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);
		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
		g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.head_pos.y - th);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth()
{
	auto  hp = esp_ctx.pl->m_iHealth();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	float off = 8;
	auto height = (((box_h * hp) / 100));
	int green = int(hp * 2.55f);
	int red = 255 - green;
	int x = esp_ctx.bbox.left - off;
	int y = esp_ctx.bbox.top;
	int w = 4;
	int h = box_h;
	g_VGuiSurface->DrawSetColor(Color::Black);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
	g_VGuiSurface->DrawSetColor(Color(red, green, 0, 255));
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x + w - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour()
{
	auto  armour = esp_ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(esp_ctx.bbox.bottom - esp_ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 4;
	auto height = (((box_h * armour) / 100));
	int x = esp_ctx.bbox.right + off;
	int y = esp_ctx.bbox.top;
	int w = 4;
	int h = box_h;
	g_VGuiSurface->DrawSetColor(Color::Black);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
	g_VGuiSurface->DrawSetColor(Color(0, 50, 255, 255));
	g_VGuiSurface->DrawFilledRect(x + 1, y + 1, x + w - 1, y + height - 2);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeapon()
{
	wchar_t buf[80];
	auto weapon = esp_ctx.pl->m_hActiveWeapon().Get();
	if (!weapon) return;
	if (MultiByteToWideChar(CP_UTF8, 0, weapon->GetCSWeaponData()->weapon_name + 7, -1, buf, 80) > 0) {
		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);
		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
		g_VGuiSurface->DrawSetTextPos(esp_ctx.feet_pos.x - tw / 2, esp_ctx.feet_pos.y);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::BacktrackSkelt()
{
	if (!g_Options.total_total_l_m_exi__kero)
		return;
	Vector previous_screenpos[64];
	for (int j = 1; j < 11; j++)
	{
		if (!TimeWarp::Get().TimeWarpData[esp_ctx.pl->EntIndex()][j].simtime)
			continue;
		Vector screen_pos;
		if (!Math::WorldToScreen(TimeWarp::Get().TimeWarpData[esp_ctx.pl->EntIndex()][j].hitboxPos, screen_pos))
			continue;
		if (previous_screenpos[esp_ctx.pl->EntIndex()].IsValid())
		{
			g_VGuiSurface->DrawSetColor(Color::Blue);
			g_VGuiSurface->DrawLine(screen_pos.x, screen_pos.y, previous_screenpos[esp_ctx.pl->EntIndex()].x, previous_screenpos[esp_ctx.pl->EntIndex()].y);
		}
		if(previous_screenpos[esp_ctx.pl->EntIndex()] != screen_pos)
			previous_screenpos[esp_ctx.pl->EntIndex()] = screen_pos;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSnapline()
{
	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);
	g_VGuiSurface->DrawSetColor(esp_ctx.clr);
	g_VGuiSurface->DrawLine(
		screen_w / 2,
		screen_h,
		esp_ctx.feet_pos.x,
		esp_ctx.feet_pos.y);
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderCrosshair()
{
	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	g_VGuiSurface->DrawSetColor(g_Options.legit_dog_dog_dog_cr_col);
	int cx = w / 2;
	int cy = h / 2;
	g_VGuiSurface->DrawLine(cx - 25, cy, cx + 25, cy);
	g_VGuiSurface->DrawLine(cx, cy - 25, cx, cy + 25);
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderWeapon(C_BaseCombatWeapon* ent)
{
	wchar_t buf[80];
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;
		auto start = strstr(name, XorStr("Weapon"));
		if (start != nullptr)
			name = start + 6;
		return name;
	};
	// We don't want to Render weapons that are being held
	if (ent->m_hOwnerEntity().IsValid())
		return;
	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	g_VGuiSurface->DrawSetColor(g_Options.legit_pes_dog_wep);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.left, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);
	if (MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, 80) > 0) {
		int w = bbox.right - bbox.left;
		int tw, th;
		g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);
		g_VGuiSurface->DrawSetTextFont(esp_font);
		g_VGuiSurface->DrawSetTextColor(g_Options.legit_pes_dog_wep);
		g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderDefuseKit(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;
	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	g_VGuiSurface->DrawSetColor(g_Options.legit_col_def_kyt);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);
	const wchar_t* buf = L"Defuse Kit";
	int w = bbox.right - bbox.left;
	int tw, th;
	g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
	g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
	g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
}
//--------------------------------------------------------------------------------
void Visuals::Misc::RenderPlantedC4(C_BaseEntity* ent)
{
	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	g_VGuiSurface->DrawSetColor(g_Options.legit_col_pl_c_bo);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);
	const wchar_t* buf = L"Bomb";
	int w = bbox.right - bbox.left;
	int tw, th;
	g_VGuiSurface->GetTextSize(esp_font, buf, tw, th);
	g_VGuiSurface->DrawSetTextFont(esp_font);
	g_VGuiSurface->DrawSetTextColor(esp_ctx.clr);
	g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
	g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

void Visuals::Misc::ThirdPerson(QAngle realangle) {

}
void Visuals::Draw3DCube(float scalar, QAngle angles, Vector middle_origin, Color outline)
{
	Vector Points[8];
	Vector PointsScreen[8];
	Vector forward, right, up;
	Math::AngleVectors(angles, forward, right, up);

	Points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar);
	Points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar);
	Points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar);
	Points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar);

	Points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar);
	Points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar);
	Points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar);
	Points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar);

	for (int i = 0; i < 8; i++)
		if (!Math::WorldToScreen(Points[i], PointsScreen[i]))
			return;

	g_VGuiSurface->DrawSetColor(outline);

	g_VGuiSurface->DrawLine(PointsScreen[0].x, PointsScreen[0].y, PointsScreen[1].x, PointsScreen[1].y);
	g_VGuiSurface->DrawLine(PointsScreen[0].x, PointsScreen[0].y, PointsScreen[2].x, PointsScreen[2].y);
	g_VGuiSurface->DrawLine(PointsScreen[3].x, PointsScreen[3].y, PointsScreen[1].x, PointsScreen[1].y);
	g_VGuiSurface->DrawLine(PointsScreen[3].x, PointsScreen[3].y, PointsScreen[2].x, PointsScreen[2].y);

	g_VGuiSurface->DrawLine(PointsScreen[0].x, PointsScreen[0].y, PointsScreen[4].x, PointsScreen[4].y);
	g_VGuiSurface->DrawLine(PointsScreen[1].x, PointsScreen[1].y, PointsScreen[5].x, PointsScreen[5].y);
	g_VGuiSurface->DrawLine(PointsScreen[2].x, PointsScreen[2].y, PointsScreen[6].x, PointsScreen[6].y);
	g_VGuiSurface->DrawLine(PointsScreen[3].x, PointsScreen[3].y, PointsScreen[7].x, PointsScreen[7].y);

	g_VGuiSurface->DrawLine(PointsScreen[4].x, PointsScreen[4].y, PointsScreen[5].x, PointsScreen[5].y);
	g_VGuiSurface->DrawLine(PointsScreen[4].x, PointsScreen[4].y, PointsScreen[6].x, PointsScreen[6].y);
	g_VGuiSurface->DrawLine(PointsScreen[7].x, PointsScreen[7].y, PointsScreen[5].x, PointsScreen[5].y);
	g_VGuiSurface->DrawLine(PointsScreen[7].x, PointsScreen[7].y, PointsScreen[6].x, PointsScreen[6].y);
}
void Visuals::DrawString(unsigned long font, int x, int y, Color color, unsigned long alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	g_VGuiSurface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	g_VGuiSurface->DrawSetTextFont(font);
	g_VGuiSurface->DrawSetTextColor(r, g, b, a);
	g_VGuiSurface->DrawSetTextPos(x, y - height / 2);
	g_VGuiSurface->DrawPrintText(wbuf, wcslen(wbuf));
}
void Visuals::DrawString(unsigned long font, bool center, int x, int y, Color c, const char *fmt, ...)
{
	wchar_t *pszStringWide = reinterpret_cast<wchar_t*>(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	TextW(center, font, x, y, c, pszStringWide);

	free(pszStringWide);
}
void Visuals::TextW(bool center, unsigned long font, int x, int y, Color c, wchar_t *pszString)
{
	if (center)
	{
		int wide, tall;
		g_VGuiSurface->GetTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}
	g_VGuiSurface->DrawSetTextColor(c);
	g_VGuiSurface->DrawSetTextFont(font);
	g_VGuiSurface->DrawSetTextPos(x, y);
	g_VGuiSurface->DrawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}



















































































































































































































































































