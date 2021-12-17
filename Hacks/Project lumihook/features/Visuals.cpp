#include "Visuals.hpp"
#include "../Options.hpp"
#include "../Structs.hpp"
#include "../helpers/Math.hpp"
#include "LagCompensation.hpp"

namespace Visuals
{
	vgui::HFont weapon_font;
	vgui::HFont ui_font;
	vgui::HFont watermark_font;
	vgui::HFont aainfo_font;
	vgui::HFont aa_info;
	vgui::HFont spectatorlist_font;
	VisualsStruct ESP_ctx;
	float ESP_Fade[64];
	C_BasePlayer* local_observed;
}

RECT GetBBox(C_BaseEntity* ent, Vector pointstransf[])
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t &trans = ent->m_rgflCoordinateFrame();

	Vector points[] =
	{
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

	Vector pos = ent->m_vecOrigin();
	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++)
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
		else
			pointstransf[i] = screen_points[i];

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++)
	{
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

void Visuals::RenderNadeEsp(C_BaseCombatWeapon* nade)
{
	if (!g_Options.esp_grenades)
		return;

	const model_t* model = nade->GetModel();
	if (!model)
		return;

	studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(model);
	if (!hdr)
		return;

	Color Nadecolor;
	std::string entityName = hdr->szName, icon_character;
	switch (nade->GetClientClass()->m_ClassID)
	{
	case 9:
		if (entityName[16] == 's')
		{
			Nadecolor = Color(255, 255, 0, 200);
			entityName = "Flash";
			icon_character = "G";
		}
		else
		{
			Nadecolor = Color(255, 0, 0, 200);
			entityName = "Frag";
			icon_character = "H";
		}
		break;
	case 134:
		Nadecolor = Color(170, 170, 170, 200);
		entityName = "Smoke";
		icon_character = "P";
		break;
	case 98:
		Nadecolor = Color(255, 0, 0, 200);
		entityName = "Fire";
		icon_character = "P";
		break;
	case 41:
		Nadecolor = Color(255, 255, 0, 200);
		entityName = "Decoy";
		icon_character = "G";
		break;
	default:
		return;
	}

	Vector points_transformed[8];
	RECT size = GetBBox(nade, points_transformed);
	if (size.right == 0 || size.bottom == 0)
		return;

	int width, height, width_icon, height_icon;
	Visuals::GetTextSize(ui_font, entityName.c_str(), width, height);
	Visuals::GetTextSize(weapon_font, icon_character.c_str(), width_icon, height_icon);

	// + distance? just make it customizable
	switch (g_Options.esp_grenades_type)
	{
	case 1:
		g_VGuiSurface->DrawSetColor(Color(20, 20, 20, 240));
		g_VGuiSurface->DrawLine(size.left - 1, size.bottom - 1, size.left - 1, size.top + 1);
		g_VGuiSurface->DrawLine(size.right + 1, size.top + 1, size.right + 1, size.bottom - 1);
		g_VGuiSurface->DrawLine(size.left - 1, size.top + 1, size.right + 1, size.top + 1);
		g_VGuiSurface->DrawLine(size.right + 1, size.bottom - 1, size.left + -1, size.bottom - 1);

		g_VGuiSurface->DrawSetColor(Nadecolor);
		g_VGuiSurface->DrawLine(size.left, size.bottom, size.left, size.top);
		g_VGuiSurface->DrawLine(size.left, size.top, size.right, size.top);
		g_VGuiSurface->DrawLine(size.right, size.top, size.right, size.bottom);
		g_VGuiSurface->DrawLine(size.right, size.bottom, size.left, size.bottom);
	case 0:
		DrawString(ui_font, size.left + ((size.right - size.left) * 0.5), size.bottom + (size.top - size.bottom) + height * 0.5f + 2, Nadecolor, FONT_CENTER, entityName.c_str());
		break;
	case 3:
		DrawString(ui_font, size.left + ((size.right - size.left) * 0.5), size.bottom + (size.top - size.bottom) + height_icon * 0.5f + 1, Nadecolor, FONT_CENTER, entityName.c_str());
	case 2:
		DrawString(weapon_font, size.left + ((size.right - size.left) * 0.5), size.bottom + (size.top - size.bottom), Nadecolor, FONT_CENTER, icon_character.c_str());
		break;
	}
}

bool Visuals::InitFont()
{
	ui_font = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(ui_font, "Bell Gothic", 10, 450, 0, 0, FONTFLAG_OUTLINE); 	//g_VGuiSurface->SetFontGlyphSet(ui_font, "Courier New", 14, 0, 0, 0, FONTFLAG_OUTLINE); // Styles

	watermark_font = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(watermark_font, "Tahoma", 16, 700, 0, 0, FONTFLAG_DROPSHADOW);

	aainfo_font = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(aainfo_font, "Tahoma", 32, 700, 0, 0, FONTFLAG_DROPSHADOW);

	aa_info = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(aa_info, "Tahoma", 32, 700, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

	weapon_font = g_VGuiSurface->CreateFont_();// 0xA1;
	g_VGuiSurface->SetFontGlyphSet(weapon_font, "Counter-Strike", 16, 500, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);

	spectatorlist_font = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(spectatorlist_font, "Tahoma", 14, 350, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

	return true;
}

bool Visuals::IsVisibleScan(C_BasePlayer *player)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	Vector eyePos = g_LocalPlayer->GetEyePos();

	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;

	if (!player->SetupBones2(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
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
		HITBOX_LEFT_FOOT,
		HITBOX_RIGHT_FOOT,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF,
		HITBOX_CHEST,
		HITBOX_STOMACH
	};
	
	for (int i = 0; i < ARRAYSIZE(scan_hitboxes); i++)
	{
		auto hitbox = studio_model->pHitboxSet(player->m_nHitboxSet())->pHitbox(scan_hitboxes[i]);
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

bool Visuals::ValidPlayer(C_BasePlayer *player, bool count_step)
{
	int idx = player->EntIndex();
	constexpr float frequency = 0.7f / 0.5f;
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

bool Visuals::Begin(C_BasePlayer *player)
{
	ESP_ctx.player = player;
	ESP_ctx.bEnemy = !player->IsTeamMate(); // not sure hmmmmm, still tired // g_LocalPlayer->m_iTeamNum() != player->m_iTeamNum();
	ESP_ctx.isVisible = Visuals::IsVisibleScan(player);
	local_observed = (C_BasePlayer*)g_EntityList->GetClientEntityFromHandle(g_LocalPlayer->m_hObserverTarget());

	int idx = player->EntIndex();
	bool playerTeam = player->m_iTeamNum() == 2;

	if (!ESP_ctx.bEnemy && g_Options.esp_enemies_only)
		return false;

	if (!player->m_bGunGameImmunity())
	{
		if (ESP_ctx.isVisible)
		{
			ESP_ctx.clr_fill.SetColor(playerTeam ? g_Options.esp_player_fill_color_t_visible : g_Options.esp_player_fill_color_ct_visible);
			ESP_ctx.clr.SetColor(playerTeam ? g_Options.esp_player_bbox_color_t_visible : g_Options.esp_player_bbox_color_ct_visible);
		}
		else
		{
			ESP_ctx.clr_fill.SetColor(playerTeam ? g_Options.esp_player_fill_color_t : g_Options.esp_player_fill_color_ct);
			ESP_ctx.clr.SetColor(playerTeam ? g_Options.esp_player_bbox_color_t : g_Options.esp_player_bbox_color_ct);
			ESP_ctx.clr.SetAlpha(255);
		}
		ESP_ctx.clr.SetAlpha(ESP_ctx.clr.a() * ESP_Fade[idx]);
		ESP_ctx.clr_fill.SetAlpha(g_Options.esp_fill_amount * ESP_Fade[idx]);
		ESP_ctx.clr_text = Color(245, 245, 245, (int)(ESP_ctx.clr.a() * ESP_Fade[idx]));
	}
	else
	{
		// Set all colors to grey if immune.
		ESP_ctx.clr.SetColor(166, 169, 174, (int)(225 * ESP_Fade[idx]));
		ESP_ctx.clr_text.SetColor(166, 169, 174, (int)(225 * ESP_Fade[idx]));
		ESP_ctx.clr_fill.SetColor(166, 169, 174, (int)(g_Options.esp_fill_amount * ESP_Fade[idx]));
	}

	// Do some touch ups if local player and scoped
	if (player == g_LocalPlayer && g_LocalPlayer->m_bIsScoped())
	{
		ESP_ctx.clr.SetAlpha(ESP_ctx.clr.a() * 0.1f);
		ESP_ctx.clr_text.SetAlpha(ESP_ctx.clr_text.a() * 0.1f);
		ESP_ctx.clr_fill.SetAlpha(ESP_ctx.clr_fill.a() * 0.1f);
	}

	Vector head = player->GetAbsOrigin() + Vector(0, 0, player->GetCollideable()->OBBMaxs().z);
	Vector origin = player->GetAbsOrigin();
	origin.z -= 5;

	if (!Math::WorldToScreen(head, ESP_ctx.head_pos) ||
		!Math::WorldToScreen(origin, ESP_ctx.feet_pos))
		return false;

	auto h = fabs(ESP_ctx.head_pos.y - ESP_ctx.feet_pos.y);
	auto w = h / 1.65f;

	switch (g_Options.esp_player_boundstype)
	{
	case 0:
		ESP_ctx.bbox.left = static_cast<long>(ESP_ctx.feet_pos.x - w * 0.5f);
		ESP_ctx.bbox.right = static_cast<long>(ESP_ctx.bbox.left + w);
		ESP_ctx.bbox.bottom = (ESP_ctx.feet_pos.y > ESP_ctx.head_pos.y ? static_cast<long>(ESP_ctx.feet_pos.y) : static_cast<long>(ESP_ctx.head_pos.y));
		ESP_ctx.bbox.top = (ESP_ctx.feet_pos.y > ESP_ctx.head_pos.y ? static_cast<long>(ESP_ctx.head_pos.y) : static_cast<long>(ESP_ctx.feet_pos.y));
		break;
	case 1:
	{
		Vector points_transformed[8];
		RECT BBox = GetBBox(player, points_transformed);
		ESP_ctx.bbox = BBox;
		ESP_ctx.bbox.top = BBox.bottom;
		ESP_ctx.bbox.bottom = BBox.top;
		break;
	}
	}

	return true;
}

void Visuals::RenderFill()
{
	g_VGuiSurface->DrawSetColor(ESP_ctx.clr_fill);
	g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.left + 2, ESP_ctx.bbox.top + 2, ESP_ctx.bbox.right - 2, ESP_ctx.bbox.bottom - 2);
}

void Visuals::RenderBox()
{
	float
		length_horizontal = (ESP_ctx.bbox.right - ESP_ctx.bbox.left) * 0.2f,
		length_vertical = (ESP_ctx.bbox.bottom - ESP_ctx.bbox.top) * 0.2f;

	Color col_black = Color(0, 0, 0, (int)(255.f * ESP_Fade[ESP_ctx.player->EntIndex()]));
	switch (g_Options.esp_player_boxtype)
	{
	case 0:
		break;

	case 1:
		g_VGuiSurface->DrawSetColor(ESP_ctx.clr);
		g_VGuiSurface->DrawOutlinedRect(ESP_ctx.bbox.left, ESP_ctx.bbox.top, ESP_ctx.bbox.right, ESP_ctx.bbox.bottom);
		g_VGuiSurface->DrawSetColor(col_black);
		g_VGuiSurface->DrawOutlinedRect(ESP_ctx.bbox.left - 1, ESP_ctx.bbox.top - 1, ESP_ctx.bbox.right + 1, ESP_ctx.bbox.bottom + 1);
		g_VGuiSurface->DrawOutlinedRect(ESP_ctx.bbox.left + 1, ESP_ctx.bbox.top + 1, ESP_ctx.bbox.right - 1, ESP_ctx.bbox.bottom - 1);
		break;

	case 2:
		g_VGuiSurface->DrawSetColor(col_black);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.left - 1, ESP_ctx.bbox.top - 1, ESP_ctx.bbox.left + 1 + length_horizontal, ESP_ctx.bbox.top + 2);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.right - 1 - length_horizontal, ESP_ctx.bbox.top - 1, ESP_ctx.bbox.right + 1, ESP_ctx.bbox.top + 2);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.left - 1, ESP_ctx.bbox.bottom - 2, ESP_ctx.bbox.left + 1 + length_horizontal, ESP_ctx.bbox.bottom + 1);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.right - 1 - length_horizontal, ESP_ctx.bbox.bottom - 2, ESP_ctx.bbox.right + 1, ESP_ctx.bbox.bottom + 1);

		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.left - 1, ESP_ctx.bbox.top + 2, ESP_ctx.bbox.left + 2, ESP_ctx.bbox.top + 1 + length_vertical);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.right - 2, ESP_ctx.bbox.top + 2, ESP_ctx.bbox.right + 1, ESP_ctx.bbox.top + 1 + length_vertical);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.left - 1, ESP_ctx.bbox.bottom - 1 - length_vertical, ESP_ctx.bbox.left + 2, ESP_ctx.bbox.bottom - 2);
		g_VGuiSurface->DrawFilledRect(ESP_ctx.bbox.right - 2, ESP_ctx.bbox.bottom - 1 - length_vertical, ESP_ctx.bbox.right + 1, ESP_ctx.bbox.bottom - 2);

		g_VGuiSurface->DrawSetColor(ESP_ctx.clr);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.left, ESP_ctx.bbox.top, ESP_ctx.bbox.left + length_horizontal - 1, ESP_ctx.bbox.top);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.right - length_horizontal, ESP_ctx.bbox.top, ESP_ctx.bbox.right - 1, ESP_ctx.bbox.top);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.left, ESP_ctx.bbox.bottom - 1, ESP_ctx.bbox.left + length_horizontal - 1, ESP_ctx.bbox.bottom - 1);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.right - length_horizontal, ESP_ctx.bbox.bottom - 1, ESP_ctx.bbox.right - 1, ESP_ctx.bbox.bottom - 1);

		g_VGuiSurface->DrawLine(ESP_ctx.bbox.left, ESP_ctx.bbox.top, ESP_ctx.bbox.left, ESP_ctx.bbox.top + length_vertical - 1);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.right - 1, ESP_ctx.bbox.top, ESP_ctx.bbox.right - 1, ESP_ctx.bbox.top + length_vertical - 1);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.left, ESP_ctx.bbox.bottom - length_vertical, ESP_ctx.bbox.left, ESP_ctx.bbox.bottom - 1);
		g_VGuiSurface->DrawLine(ESP_ctx.bbox.right - 1, ESP_ctx.bbox.bottom - length_vertical, ESP_ctx.bbox.right - 1, ESP_ctx.bbox.bottom - 1);
		break;
	}
}

void Visuals::GayPornYes()
{
	if (!g_LocalPlayer->IsAlive())
		return;

	g_VGuiSurface->DrawSetColor(Color(g_Options.xhair_color[0], g_Options.xhair_color[1], g_Options.xhair_color[2]));
	g_VGuiSurface->DrawLine(Global::iScreenX / 2 - g_Options.visuals_xhair_x, Global::iScreenY / 2, Global::iScreenX / 2 + g_Options.visuals_xhair_x, Global::iScreenY / 2);
	g_VGuiSurface->DrawLine(Global::iScreenX / 2, Global::iScreenY / 2 - g_Options.visuals_xhair_y, Global::iScreenX / 2, Global::iScreenY / 2 + g_Options.visuals_xhair_y);

}

void Visuals::NightMode()
{
	static std::string old_Skyname = "";
	static bool OldNightmode;
	static int OldSky;
	if (!g_LocalPlayer || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame())
	{
		old_Skyname = "";
		OldNightmode = false;
		OldSky = 0;
		return;
	}
	static ConVar*r_DrawSpecificStaticProp;
	if (OldNightmode != g_Options.visuals_nightmode)
	{
		//static auto sv_skyname = g_CVar->FindVar("sv_skyname");
		//if (g_Options.visuals_nightmode)
		//	sv_skyname->SetValue("sky_csgo_night02");
		//else sv_skyname->SetValue("vertigoblue_hdr");

		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->SetValue(0);
		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_MatSystem->GetMaterial(i);
			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (g_Options.visuals_nightmode)
					if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
						pMaterial->ColorModulate(g_Options.visuals_others_nightmode_color[0], g_Options.visuals_others_nightmode_color[1], g_Options.visuals_others_nightmode_color[2]);
					else
						pMaterial->ColorModulate(g_Options.visuals_others_nightmode_color[0], g_Options.visuals_others_nightmode_color[1], g_Options.visuals_others_nightmode_color[2]);
				else
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
			}

			if (strstr(pMaterial->GetName(), "models/props"))
			{
				if (g_Options.visuals_nightmode)
					pMaterial->ColorModulate(g_Options.visuals_others_nightmode_color[0], g_Options.visuals_others_nightmode_color[1], g_Options.visuals_others_nightmode_color[2]);
				else 
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
			}

			if (strstr(pMaterial->GetTextureGroupName(), ("SkyBox")))
			{
				if (g_Options.visuals_nightmode)
					pMaterial->ColorModulate(g_Options.visuals_others_skybox_color[0], g_Options.visuals_others_skybox_color[1], g_Options.visuals_others_skybox_color[2]);
				else 
					pMaterial->ColorModulate(1.f, 1.f, 1.f);
			}
		}
		OldNightmode = g_Options.visuals_nightmode;
	}
}

void Visuals::SpoofZeug()
{
	static auto cl_phys_timescale = g_CVar->FindVar("cl_phys_timescale");
	if (g_Options.cl_phys_timescale)
		cl_phys_timescale->SetValue(g_Options.cl_phys_timescale_value);
	else cl_phys_timescale->SetValue("1.0");

	static auto mp_radar_showall = g_CVar->FindVar("mp_radar_showall");
	if (g_Options.mp_radar_showall)
		mp_radar_showall->SetValue(1);
	else mp_radar_showall->SetValue(0);

	static auto cl_crosshair_recoil = g_CVar->FindVar("cl_crosshair_recoil");
	if (g_Options.cl_crosshair_recoil)
		cl_crosshair_recoil->SetValue(1);
	else cl_crosshair_recoil->SetValue(0);

	static auto fog_override = g_CVar->FindVar("fog_override");
	static auto fog_enable = g_CVar->FindVar("fog_enable");
	if (g_Options.fog_override) {
		fog_enable->SetValue(0);
		fog_override->SetValue(1);
	}
	else {
		fog_enable->SetValue(1);
		fog_override->SetValue(0);
	}

	static auto viewmodel_offset_x = g_CVar->FindVar("viewmodel_offset_x");
	static auto viewmodel_offset_y = g_CVar->FindVar("viewmodel_offset_y");
	static auto viewmodel_offset_z = g_CVar->FindVar("viewmodel_offset_z");
	viewmodel_offset_x->m_nFlags &= ~FCVAR_CHEAT;
	viewmodel_offset_y->m_nFlags &= ~FCVAR_CHEAT;
	viewmodel_offset_z->m_nFlags &= ~FCVAR_CHEAT;
	viewmodel_offset_x->m_fnChangeCallbacks.SetSize(0);
	viewmodel_offset_y->m_fnChangeCallbacks.SetSize(0);
	viewmodel_offset_z->m_fnChangeCallbacks.SetSize(0);
	if (g_Options.change_viewmodel_offset) {
		viewmodel_offset_x->SetValue(g_Options.viewmodel_offset_x);
		viewmodel_offset_y->SetValue(g_Options.viewmodel_offset_y);
		viewmodel_offset_z->SetValue(g_Options.viewmodel_offset_z);
	}
	else {
		viewmodel_offset_x->SetValue(2.5f);
		viewmodel_offset_y->SetValue(2);
		viewmodel_offset_z->SetValue(-2);
	}
}

void Visuals::AsusWalls()
{
	static std::string old_Skyname = "";
	static bool OldNightmode;
	static int OldSky;
	if (!g_LocalPlayer || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame())
	{
		old_Skyname = "";
		OldNightmode = false;
		OldSky = 0;
		return;
	}
	static ConVar*r_DrawSpecificStaticProp;
	if (OldNightmode != g_Options.visuals_asuswalls)
	{
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->SetValue(0);
		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_MatSystem->GetMaterial(i);
			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetName(), "models/props"))
			{
				if (g_Options.visuals_asuswalls)
					pMaterial->AlphaModulate(0.5f);
				else
					pMaterial->AlphaModulate(1.f);
			}
		}
		OldNightmode = g_Options.visuals_asuswalls;
	}
}

void Visuals::RenderName()
{
	wchar_t buf[128];
	std::string name = ESP_ctx.player->GetName(),
		s_name = (name.length() > 0 ? name : "##ERROR_empty_name");

	if (MultiByteToWideChar(CP_UTF8, 0, s_name.c_str(), -1, buf, 128) > 0)
	{
		int tw, th;
		g_VGuiSurface->GetTextSize(ui_font, buf, tw, th);

		g_VGuiSurface->DrawSetTextFont(ui_font);
		g_VGuiSurface->DrawSetTextColor(ESP_ctx.clr_text);
		g_VGuiSurface->DrawSetTextPos(ESP_ctx.bbox.left + (ESP_ctx.bbox.right - ESP_ctx.bbox.left) * 0.5 - tw * 0.5, ESP_ctx.bbox.top - th + 1);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}

void Visuals::DrawAngleLines()
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = g_LocalPlayer;
	Math::AngleVectors(QAngle(0, g_LocalPlayer->m_flLowerBodyYawTarget(), 0), forward);
	src3D = g_LocalPlayer->m_vecOrigin();
	dst3D = src3D + (forward * 50.f);

	ray.Init(src3D, dst3D);
	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!Math::WorldToScreen(src3D, src) || !Math::WorldToScreen(tr.endpos, dst))
		return;

	g_VGuiSurface->DrawSetColor(Color::Yellow);
	g_VGuiSurface->DrawLine(src.x, src.y, dst.x, dst.y);


	Math::AngleVectors(QAngle(0, g_LocalPlayer->m_angEyeAngles().yaw, 0), forward);
	dst3D = src3D + (forward * 50.f);

	ray.Init(src3D, dst3D);
	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!Math::WorldToScreen(src3D, src) || !Math::WorldToScreen(tr.endpos, dst))
		return;

	g_VGuiSurface->DrawSetColor(Color::Red);
	g_VGuiSurface->DrawLine(src.x, src.y, dst.x, dst.y);
}

void Visuals::DrawWatermark()
{
	int alpha;

	if (!g_Options.misc_logevents)
		alpha = 255;
	else alpha = 0;

	DrawString(watermark_font, 10, 10, Color(187, 76, 206, alpha), FONT_LEFT, "www.lumisquad.xyz | fantailcommunity.xyz | https://discord.gg/hGRZrXN");

	//if (Global::CanDoBacktrack)
	//	DrawString(watermark_font, 10, 30, Color(0, 255, 0, 255), FONT_LEFT, "true");
	//else
	//	DrawString(watermark_font, 10, 30, Color(255, 0, 0, 255), FONT_LEFT, "false");
}

void Visuals::DrawCapsuleOverlay(int idx, float duration)
{
	if (idx == g_EngineClient->GetLocalPlayer())
		return;

	auto player = C_BasePlayer::GetPlayerByIndex(idx);
	if (!player)
		return;

	if (g_Options.rage_lagcompensation && CMBacktracking::Get().m_LagRecord[player->EntIndex()].empty())
		return;

	matrix3x4_t boneMatrix_actual[MAXSTUDIOBONES];
	if (!player->SetupBones2(boneMatrix_actual, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, player->m_flSimulationTime()))
		return;

	studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(player->GetModel());
	if (studioHdr)
	{
		matrix3x4_t boneMatrix[MAXSTUDIOBONES];
		std::memcpy(boneMatrix, CMBacktracking::Get().current_record[player->EntIndex()].matrix, sizeof(CMBacktracking::Get().current_record[player->EntIndex()].matrix));

		mstudiohitboxset_t *set = studioHdr->pHitboxSet(player->m_nHitboxSet());
		if (set)
		{
			for (int i = 0; i < set->numhitboxes; i++)
			{
				mstudiobbox_t *hitbox = set->pHitbox(i);
				if (hitbox)
				{
					if (hitbox->m_flRadius == -1.0f)
					{
						Vector position, position_actual;
						QAngle angles, angles_actual;
						MatrixAngles(boneMatrix[hitbox->bone], angles, position);
						MatrixAngles(boneMatrix_actual[hitbox->bone], angles_actual, position_actual);

						g_DebugOverlay->AddBoxOverlay(position, hitbox->bbmin, hitbox->bbmax, angles, 255, 0, 0, 150, duration);

						if (g_Options.esp_lagcompensated_hitboxes_type == 1)
							g_DebugOverlay->AddBoxOverlay(position_actual, hitbox->bbmin, hitbox->bbmax, angles_actual, 0, 0, 255, 150, duration);
					}
					else
					{
						Vector min, max,
							min_actual, max_actual;

						Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
						Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

						Math::VectorTransform(hitbox->bbmin, boneMatrix_actual[hitbox->bone], min_actual);
						Math::VectorTransform(hitbox->bbmax, boneMatrix_actual[hitbox->bone], max_actual);

						g_DebugOverlay->AddCapsuleOverlay(min, max, hitbox->m_flRadius, 255, 0, 0, 150, duration);

						if (g_Options.esp_lagcompensated_hitboxes_type == 1)
							g_DebugOverlay->AddCapsuleOverlay(min_actual, max_actual, hitbox->m_flRadius, 0, 0, 255, 150, duration);
					}
				}
			}
		}
	}
}

void Visuals::RenderHealth()
{
	int health = ESP_ctx.player->m_iHealth();
	if (health > 100)
		health = 100;

	float box_h = (float)fabs(ESP_ctx.bbox.bottom - ESP_ctx.bbox.top);
	float off = 8;

	auto height = box_h - (((box_h * health) / 100));

	int x = ESP_ctx.bbox.left - off;
	int y = ESP_ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Color col_black = Color(0, 0, 0, (int)(255.f * ESP_Fade[ESP_ctx.player->EntIndex()]));
	g_VGuiSurface->DrawSetColor(col_black);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);

	g_VGuiSurface->DrawSetColor(Color((255 - health * int(2.55f)), (health * int(2.55f)), 0, (int)(180.f * ESP_Fade[ESP_ctx.player->EntIndex()])));
	g_VGuiSurface->DrawFilledRect(x + 1, y + height + 1, x + w - 1, y + h - 1);
}

void Visuals::RenderWeapon()
{
	wchar_t buf[80];
	auto clean_item_name = [](const char *name) -> const char*
	{
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	auto weapon = ESP_ctx.player->m_hActiveWeapon().Get();

	if (!weapon) return;

	if (weapon->m_hOwnerEntity().IsValid())
	{
		auto name = clean_item_name(weapon->GetClientClass()->m_pNetworkName);
		std::ostringstream oss;
		oss << name << " " << "[" << weapon->m_iClip1() << " / " << weapon->m_iPrimaryReserveAmmoCount() << "]";
		std::string var = oss.str();

		if (weapon->GetWeapInfo()->weapon_type() != WEAPONTYPE_KNIFE 
			&& weapon->GetWeapInfo()->weapon_type() != WEAPONTYPE_GRENADE 
			&& weapon->GetWeapInfo()->weapon_type() != WEAPONTYPE_C4
			&& weapon->GetWeapInfo()->weapon_type() != WEAPONTYPE_UNKNOWN)
		{
			if (MultiByteToWideChar(CP_UTF8, 0, var.c_str(), -1, buf, 80) > 0)
			{
				int tw, th;
				g_VGuiSurface->GetTextSize(ui_font, buf, tw, th);

				g_VGuiSurface->DrawSetTextFont(ui_font);
				g_VGuiSurface->DrawSetTextColor(ESP_ctx.clr_text);
				g_VGuiSurface->DrawSetTextPos(ESP_ctx.bbox.left + (ESP_ctx.bbox.right - ESP_ctx.bbox.left) * 0.5 - tw * 0.5, ESP_ctx.bbox.bottom + 1);
				g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
			}
		}
		else
		{
			if (MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, 80) > 0)
			{
				int tw, th;
				g_VGuiSurface->GetTextSize(ui_font, buf, tw, th);

				g_VGuiSurface->DrawSetTextFont(ui_font);
				g_VGuiSurface->DrawSetTextColor(ESP_ctx.clr_text);
				g_VGuiSurface->DrawSetTextPos(ESP_ctx.bbox.left + (ESP_ctx.bbox.right - ESP_ctx.bbox.left) * 0.5 - tw * 0.5, ESP_ctx.bbox.bottom + 1);
				g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
			}
		}
	}
}

void Visuals::RenderSnapline()
{
	g_EngineClient->GetScreenSize(Global::iScreenX, Global::iScreenY);
	int alpha = g_Options.visuals_manual_aa_opacity;
	if (g_Options.visuals_manual_aa)
	{
		if (Global::left) {
			DrawString(aa_info, Global::iScreenX / 2 - 50, Global::iScreenY / 2, Color(66, 134, 244, alpha), FONT_CENTER, ">");
			DrawString(aa_info, Global::iScreenX / 2 + 50, Global::iScreenY / 2, Color(200, 200, 200, alpha), FONT_CENTER, "<");
			DrawString(aa_info, Global::iScreenX / 2, Global::iScreenY / 2 + 50, Color(200, 200, 200, alpha), FONT_CENTER, "^");
		}
		if (Global::right) {
			DrawString(aa_info, Global::iScreenX / 2 - 50, Global::iScreenY / 2, Color(200, 200, 200, alpha), FONT_CENTER, ">");
			DrawString(aa_info, Global::iScreenX / 2 + 50, Global::iScreenY / 2, Color(66, 134, 244, alpha), FONT_CENTER, "<");
			DrawString(aa_info, Global::iScreenX / 2, Global::iScreenY / 2 + 50, Color(200, 200, 200, alpha), FONT_CENTER, "^");
		}
		if (Global::backwards) {
			DrawString(aa_info, Global::iScreenX / 2 - 50, Global::iScreenY / 2, Color(200, 200, 200, alpha), FONT_CENTER, ">");
			DrawString(aa_info, Global::iScreenX / 2 + 50, Global::iScreenY / 2, Color(200, 200, 200, alpha), FONT_CENTER, "<");
			DrawString(aa_info, Global::iScreenX / 2, Global::iScreenY / 2 + 50, Color(66, 134, 244, alpha), FONT_CENTER, "^");
		}
	}
}

void Visuals::RenderSkelet()
{
	studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(ESP_ctx.player->GetModel());
	if (studioHdr)
	{
		static matrix3x4_t boneToWorldOut[128];
		if (ESP_ctx.player->SetupBones2(boneToWorldOut, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, ESP_ctx.player->m_flSimulationTime()))
		{
			for (int i = 0; i < studioHdr->numbones; i++)
			{
				mstudiobone_t *bone = studioHdr->pBone(i);
				if (!bone || !(bone->flags & BONE_USED_BY_HITBOX) || bone->parent == -1)
					continue;

				Vector bonePos1;
				if (!Math::WorldToScreen(Vector(boneToWorldOut[i][0][3], boneToWorldOut[i][1][3], boneToWorldOut[i][2][3]), bonePos1))
					continue;

				Vector bonePos2;
				if (!Math::WorldToScreen(Vector(boneToWorldOut[bone->parent][0][3], boneToWorldOut[bone->parent][1][3], boneToWorldOut[bone->parent][2][3]), bonePos2))
					continue;

				g_VGuiSurface->DrawSetColor(ESP_ctx.clr);
				g_VGuiSurface->DrawLine((int)bonePos1.x, (int)bonePos1.y, (int)bonePos2.x, (int)bonePos2.y);
			}
		}
	}
}

void Visuals::RenderBacktrackedSkelet()
{
	if (!g_Options.rage_lagcompensation)
		return;

	auto records = &CMBacktracking::Get().m_LagRecord[ESP_ctx.player->EntIndex()];
	if (records->size() < 2)
		return;

	Vector previous_screenpos;
	for (auto record = records->begin(); record != records->end(); record++)
	{
		if (!CMBacktracking::Get().IsTickValid(TIME_TO_TICKS(record->m_flSimulationTime)))
			continue;

		Vector screen_pos;
		if (!Math::WorldToScreen(record->m_vecHeadSpot, screen_pos))
			continue;

		if (previous_screenpos.IsValid())
		{
			if (*record == CMBacktracking::Get().m_RestoreLagRecord[ESP_ctx.player->EntIndex()].first)
				g_VGuiSurface->DrawSetColor(Color(255, 255, 0, 255));
			else
				g_VGuiSurface->DrawSetColor(Color(255, 255, 255, 255));
			g_VGuiSurface->DrawLine(screen_pos.x, screen_pos.y, previous_screenpos.x, previous_screenpos.y);
		}

		previous_screenpos = screen_pos;
	}
}

void Visuals::RenderWeapon(C_BaseCombatWeapon *entity)
{
	wchar_t buf[80];
	auto clean_item_name = [](const char *name) -> const char*
	{
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	if (entity->m_hOwnerEntity().IsValid() ||
		entity->m_vecOrigin() == Vector(0, 0, 0))
		return;

	Vector pointsTransformed[8];
	auto bbox = GetBBox(entity, pointsTransformed);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	g_VGuiSurface->DrawSetColor(g_Options.dropped_weapons_color[0], g_Options.dropped_weapons_color[1],
		g_Options.dropped_weapons_color[2], g_Options.dropped_weapons_color[3]);
	switch (g_Options.esp_dropped_weapons)
	{
	case 1:
		g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
		g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.right, bbox.bottom);
		g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.left, bbox.bottom);
		g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
		break;
	case 2:
		g_VGuiSurface->DrawLine(pointsTransformed[0].x, pointsTransformed[0].y, pointsTransformed[1].x, pointsTransformed[1].y);
		g_VGuiSurface->DrawLine(pointsTransformed[0].x, pointsTransformed[0].y, pointsTransformed[6].x, pointsTransformed[6].y);
		g_VGuiSurface->DrawLine(pointsTransformed[1].x, pointsTransformed[1].y, pointsTransformed[5].x, pointsTransformed[5].y);
		g_VGuiSurface->DrawLine(pointsTransformed[6].x, pointsTransformed[6].y, pointsTransformed[5].x, pointsTransformed[5].y);

		g_VGuiSurface->DrawLine(pointsTransformed[2].x, pointsTransformed[2].y, pointsTransformed[1].x, pointsTransformed[1].y);
		g_VGuiSurface->DrawLine(pointsTransformed[4].x, pointsTransformed[4].y, pointsTransformed[5].x, pointsTransformed[5].y);
		g_VGuiSurface->DrawLine(pointsTransformed[6].x, pointsTransformed[6].y, pointsTransformed[7].x, pointsTransformed[7].y);
		g_VGuiSurface->DrawLine(pointsTransformed[3].x, pointsTransformed[3].y, pointsTransformed[0].x, pointsTransformed[0].y);

		g_VGuiSurface->DrawLine(pointsTransformed[3].x, pointsTransformed[3].y, pointsTransformed[2].x, pointsTransformed[2].y);
		g_VGuiSurface->DrawLine(pointsTransformed[2].x, pointsTransformed[2].y, pointsTransformed[4].x, pointsTransformed[4].y);
		g_VGuiSurface->DrawLine(pointsTransformed[7].x, pointsTransformed[7].y, pointsTransformed[4].x, pointsTransformed[4].y);
		g_VGuiSurface->DrawLine(pointsTransformed[7].x, pointsTransformed[7].y, pointsTransformed[3].x, pointsTransformed[3].y);
		break;
	case 3:
		break;
	}

	auto name = clean_item_name(entity->GetClientClass()->m_pNetworkName);
	if (MultiByteToWideChar(CP_UTF8, 0, name, -1, buf, 80) > 0)
	{
		int w = bbox.right - bbox.left;
		int tw, th;
		g_VGuiSurface->GetTextSize(ui_font, buf, tw, th);

		g_VGuiSurface->DrawSetTextFont(ui_font);
		g_VGuiSurface->DrawSetTextColor(g_Options.dropped_weapons_color[0], g_Options.dropped_weapons_color[1],
			g_Options.dropped_weapons_color[2], g_Options.dropped_weapons_color[3]);
		g_VGuiSurface->DrawSetTextPos(bbox.left + ((bbox.right - bbox.left) / 2) - (tw / 2), bbox.top + 1);
		g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}

void Visuals::RenderPlantedC4(C_BaseEntity *entity)
{
	//int width, height;
	//g_EngineClient->GetScreenSize(width, height);

	float lifetime = entity->m_flC4Blow() - (g_GlobalVars->interval_per_tick * g_LocalPlayer->m_nTickBase());

	if (g_EngineClient->IsConnected() && g_EngineClient->IsInGame())
	{
		if (lifetime > 0.01f && !entity->m_bBombDefused())
		{
			DrawString(aainfo_font, 30, Global::iScreenY / 2, Color(187, 76, 206, 255), FONT_LEFT, "%.1f", lifetime);
		}

		float countdown = entity->m_flDefuseCountDown() - (g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick);
		if (entity->m_hBombDefuser() > 0)
		{
			if (countdown > 0.01f)
			{
				if (lifetime > countdown)
				{
					DrawString(aainfo_font, 30, Global::iScreenY / 2 + 30, Color(28, 109, 239, 255), FONT_LEFT, "%.1f", countdown);
				}
			}
		}

	Vector screen_points[8];
	auto bbox = GetBBox(entity, screen_points);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	if (lifetime < 0.01f)
		return;

	g_VGuiSurface->DrawSetColor(Color::Red);
	g_VGuiSurface->DrawLine(bbox.left, bbox.bottom, bbox.left, bbox.top);
	g_VGuiSurface->DrawLine(bbox.left, bbox.top, bbox.right, bbox.top);
	g_VGuiSurface->DrawLine(bbox.right, bbox.top, bbox.right, bbox.bottom);
	g_VGuiSurface->DrawLine(bbox.right, bbox.bottom, bbox.left, bbox.bottom);

	const wchar_t *buf = L"C4";

	int w = bbox.right - bbox.left;
	int tw, th;
	g_VGuiSurface->GetTextSize(ui_font, buf, tw, th);

	g_VGuiSurface->DrawSetTextFont(ui_font);
	g_VGuiSurface->DrawSetTextColor(Color::Red);
	g_VGuiSurface->DrawSetTextPos((bbox.left + w * 0.5f) - tw * 0.5f, bbox.bottom + 1);
	g_VGuiSurface->DrawPrintText(buf, wcslen(buf));
	}
}

void Visuals::RenderSpectatorList()
{
	RECT scrn = GetViewport();
	int cnt = 0;
	wchar_t buf[128];
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

				wchar_t buf[128];

				player_info_t player_info2;
				if (g_EngineClient->GetPlayerInfo(target->EntIndex(), &player_info2))
				{
					if (target->EntIndex() == g_LocalPlayer->EntIndex())
					{
							char player_name[255] = { 0 };
							sprintf_s(player_name, "%s", player_info.szName);

							int w, h;
							GetTextSize(spectatorlist_font, player_name, w, h);

							std::string name = player_name,
								s_name = (name.length() > 0 ? name : "##ERROR_empty_name");

							if (MultiByteToWideChar(CP_UTF8, 0, s_name.c_str(), -1, buf, 128) > 0)
								DrawString(spectatorlist_font, false, scrn.right - w - 4, (scrn.bottom / 2) + (16 * cnt), Color(187, 76, 206, 255), s_name.c_str());
							++cnt;
					}
				}
			}
		}
	}
}

void Visuals::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = g_VGuiSurface->CreateNewTextureID(true);
	unsigned char buffer[4] = { color.r(), color.g(), color.b(), color.a() };

	g_VGuiSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	g_VGuiSurface->DrawSetColor(Color(255, 255, 255, 255));
	g_VGuiSurface->DrawSetTexture(Texture);

	g_VGuiSurface->DrawTexturedPolygon(count, Vertexs);
}

void Visuals::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	PolyLine(x, y, count, colorLine);
}

void Visuals::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	PolyLine(x, y, count, colorLine);
}

void Visuals::PolyLine(int *x, int *y, int count, Color color)
{
	g_VGuiSurface->DrawSetColor(color);
	g_VGuiSurface->DrawPolyLine(x, y, count);
}

void Visuals::Draw3DCube(float scalar, QAngle angles, Vector middle_origin, Color outline)
{
	Vector forward, right, up;
	Math::AngleVectors(angles, forward, right, up);

	Vector points[8];
	points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar); // BLT
	points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar); // BRT
	points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar); // BLB
	points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar); // BRB

	points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar); // FLT
	points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar); // FRT
	points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar); // FLB
	points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar); // FRB

	Vector points_screen[8];
	for (int i = 0; i < 8; i++)
		if (!Math::WorldToScreen(points[i], points_screen[i]))
			return;

	g_VGuiSurface->DrawSetColor(outline);

	// Back frame
	g_VGuiSurface->DrawLine(points_screen[0].x, points_screen[0].y, points_screen[1].x, points_screen[1].y);
	g_VGuiSurface->DrawLine(points_screen[0].x, points_screen[0].y, points_screen[2].x, points_screen[2].y);
	g_VGuiSurface->DrawLine(points_screen[3].x, points_screen[3].y, points_screen[1].x, points_screen[1].y);
	g_VGuiSurface->DrawLine(points_screen[3].x, points_screen[3].y, points_screen[2].x, points_screen[2].y);

	// Frame connector
	g_VGuiSurface->DrawLine(points_screen[0].x, points_screen[0].y, points_screen[4].x, points_screen[4].y);
	g_VGuiSurface->DrawLine(points_screen[1].x, points_screen[1].y, points_screen[5].x, points_screen[5].y);
	g_VGuiSurface->DrawLine(points_screen[2].x, points_screen[2].y, points_screen[6].x, points_screen[6].y);
	g_VGuiSurface->DrawLine(points_screen[3].x, points_screen[3].y, points_screen[7].x, points_screen[7].y);

	// Front frame
	g_VGuiSurface->DrawLine(points_screen[4].x, points_screen[4].y, points_screen[5].x, points_screen[5].y);
	g_VGuiSurface->DrawLine(points_screen[4].x, points_screen[4].y, points_screen[6].x, points_screen[6].y);
	g_VGuiSurface->DrawLine(points_screen[7].x, points_screen[7].y, points_screen[5].x, points_screen[5].y);
	g_VGuiSurface->DrawLine(points_screen[7].x, points_screen[7].y, points_screen[6].x, points_screen[6].y);
}

void Visuals::FillRGBA(int x, int y, int w, int h, Color c)
{
	g_VGuiSurface->DrawSetColor(c);
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
}

void Visuals::BorderBox(int x, int y, int w, int h, Color color, int thickness)
{
	FillRGBA(x, y, w, thickness, color);
	FillRGBA(x, y, thickness, h, color);
	FillRGBA(x + w, y, thickness, h, color);
	FillRGBA(x, y + h, w + thickness, thickness, color);
}

__inline void Visuals::DrawFilledRect(int x, int y, int w, int h)
{
	g_VGuiSurface->DrawFilledRect(x, y, x + w, y + h);
}

void Visuals::DrawRectOutlined(int x, int y, int w, int h, Color color, Color outlinedColor, int thickness)
{
	FillRGBA(x, y, w, h, color);
	BorderBox(x - 1, y - 1, w + 1, h + 1, outlinedColor, thickness);
}

void Visuals::DrawString(unsigned long font, int x, int y, Color color, unsigned long alignment, const char* msg, ...)
{
	FUNCTION_GUARD;

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
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

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

void Visuals::DrawCircle(int x, int y, float r, int step, Color color)
{
	float Step = PI * 2.0 / step;
	for (float a = 0; a < (PI*2.0); a += Step)
	{
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawLine(x1, y1, x2, y2);
	}
}

void Visuals::DrawOutlinedRect(int x, int y, int w, int h, Color &c)
{
	g_VGuiSurface->DrawSetColor(c);
	g_VGuiSurface->DrawOutlinedRect(x, y, x + w, y + h);
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

RECT Visuals::GetViewport()
{
	RECT viewport = { 0, 0, 0, 0 };
	//int w, h;
	//g_EngineClient->GetScreenSize(w, h);
	viewport.right = Global::iScreenX; viewport.bottom = Global::iScreenY;

	return viewport;
}
// Junk Code By Troll Face & Thaisen's Gen
void pPeYHeJvLO39213201() {     int gqZNXlukqO64816094 = -221684667;    int gqZNXlukqO95712836 = -39065567;    int gqZNXlukqO27330226 = -246569558;    int gqZNXlukqO93209260 = -455704316;    int gqZNXlukqO41479306 = -740971400;    int gqZNXlukqO54658800 = -641207713;    int gqZNXlukqO64095726 = -78497091;    int gqZNXlukqO60483560 = -978669031;    int gqZNXlukqO85436961 = 55866241;    int gqZNXlukqO68448418 = -69103918;    int gqZNXlukqO48553555 = -979342820;    int gqZNXlukqO72453229 = -130239387;    int gqZNXlukqO78874990 = -938031887;    int gqZNXlukqO13898642 = -755106879;    int gqZNXlukqO28874502 = 39188749;    int gqZNXlukqO38261258 = -979035218;    int gqZNXlukqO85772013 = -217240731;    int gqZNXlukqO23739904 = -3917220;    int gqZNXlukqO45229516 = 16492608;    int gqZNXlukqO60086486 = -153646286;    int gqZNXlukqO45366602 = -528198527;    int gqZNXlukqO65172528 = -757249626;    int gqZNXlukqO58809657 = -118513392;    int gqZNXlukqO6976276 = 67580692;    int gqZNXlukqO50058803 = -22830011;    int gqZNXlukqO8473524 = -47173349;    int gqZNXlukqO38450260 = -924302033;    int gqZNXlukqO41409849 = -733993142;    int gqZNXlukqO22871295 = -109791620;    int gqZNXlukqO75198809 = -713945785;    int gqZNXlukqO41369222 = -882464685;    int gqZNXlukqO56288098 = -553434035;    int gqZNXlukqO14037811 = -76686349;    int gqZNXlukqO96293221 = -837968316;    int gqZNXlukqO58244709 = -448553431;    int gqZNXlukqO88472023 = -437059567;    int gqZNXlukqO85997302 = -496101286;    int gqZNXlukqO61020418 = -977510301;    int gqZNXlukqO16281623 = -741376470;    int gqZNXlukqO40255011 = -62665223;    int gqZNXlukqO78383717 = -269985468;    int gqZNXlukqO36146919 = -478212419;    int gqZNXlukqO67406887 = -556047018;    int gqZNXlukqO975625 = -803983237;    int gqZNXlukqO92255789 = -376044614;    int gqZNXlukqO29171358 = -202503334;    int gqZNXlukqO69939444 = -339891250;    int gqZNXlukqO24147610 = 51676455;    int gqZNXlukqO89883905 = -23154518;    int gqZNXlukqO75812010 = -756453602;    int gqZNXlukqO57369076 = -70083032;    int gqZNXlukqO91231272 = -912201584;    int gqZNXlukqO6913685 = -287636523;    int gqZNXlukqO77690608 = 66506993;    int gqZNXlukqO93846599 = -371907815;    int gqZNXlukqO99643566 = -464435042;    int gqZNXlukqO36903179 = -920552176;    int gqZNXlukqO20353951 = -214150251;    int gqZNXlukqO43150457 = -332874306;    int gqZNXlukqO33005783 = -593798052;    int gqZNXlukqO16208540 = -716905681;    int gqZNXlukqO22685878 = -344503950;    int gqZNXlukqO37612265 = -768877412;    int gqZNXlukqO10238152 = -230187974;    int gqZNXlukqO27079196 = -186639234;    int gqZNXlukqO92265456 = -325908786;    int gqZNXlukqO58415418 = 46446962;    int gqZNXlukqO82581768 = -63571;    int gqZNXlukqO55653933 = -206553449;    int gqZNXlukqO40402479 = -523751684;    int gqZNXlukqO52263956 = -382933933;    int gqZNXlukqO24751595 = -239730430;    int gqZNXlukqO7458281 = -262540750;    int gqZNXlukqO4974506 = -920842170;    int gqZNXlukqO81702768 = -883660818;    int gqZNXlukqO9219683 = 50013892;    int gqZNXlukqO97765641 = -101202608;    int gqZNXlukqO57834033 = -314530156;    int gqZNXlukqO14720486 = -556374694;    int gqZNXlukqO20887446 = -820326678;    int gqZNXlukqO38534079 = -707282100;    int gqZNXlukqO14302651 = -875978488;    int gqZNXlukqO51525944 = -610838625;    int gqZNXlukqO47059285 = -353338018;    int gqZNXlukqO17829733 = -543862753;    int gqZNXlukqO50137949 = -970263101;    int gqZNXlukqO49374414 = -165797512;    int gqZNXlukqO36347202 = -43193342;    int gqZNXlukqO2446623 = -366060502;    int gqZNXlukqO58601142 = -984118389;    int gqZNXlukqO51568844 = -516507392;    int gqZNXlukqO65643351 = -181951035;    int gqZNXlukqO17869962 = -544635995;    int gqZNXlukqO83275839 = -47578419;    int gqZNXlukqO24046471 = -345759542;    int gqZNXlukqO55697840 = -925481518;    int gqZNXlukqO98534654 = -709335007;    int gqZNXlukqO57168735 = -225859044;    int gqZNXlukqO73896428 = -517344004;    int gqZNXlukqO99990332 = -221684667;     gqZNXlukqO64816094 = gqZNXlukqO95712836;     gqZNXlukqO95712836 = gqZNXlukqO27330226;     gqZNXlukqO27330226 = gqZNXlukqO93209260;     gqZNXlukqO93209260 = gqZNXlukqO41479306;     gqZNXlukqO41479306 = gqZNXlukqO54658800;     gqZNXlukqO54658800 = gqZNXlukqO64095726;     gqZNXlukqO64095726 = gqZNXlukqO60483560;     gqZNXlukqO60483560 = gqZNXlukqO85436961;     gqZNXlukqO85436961 = gqZNXlukqO68448418;     gqZNXlukqO68448418 = gqZNXlukqO48553555;     gqZNXlukqO48553555 = gqZNXlukqO72453229;     gqZNXlukqO72453229 = gqZNXlukqO78874990;     gqZNXlukqO78874990 = gqZNXlukqO13898642;     gqZNXlukqO13898642 = gqZNXlukqO28874502;     gqZNXlukqO28874502 = gqZNXlukqO38261258;     gqZNXlukqO38261258 = gqZNXlukqO85772013;     gqZNXlukqO85772013 = gqZNXlukqO23739904;     gqZNXlukqO23739904 = gqZNXlukqO45229516;     gqZNXlukqO45229516 = gqZNXlukqO60086486;     gqZNXlukqO60086486 = gqZNXlukqO45366602;     gqZNXlukqO45366602 = gqZNXlukqO65172528;     gqZNXlukqO65172528 = gqZNXlukqO58809657;     gqZNXlukqO58809657 = gqZNXlukqO6976276;     gqZNXlukqO6976276 = gqZNXlukqO50058803;     gqZNXlukqO50058803 = gqZNXlukqO8473524;     gqZNXlukqO8473524 = gqZNXlukqO38450260;     gqZNXlukqO38450260 = gqZNXlukqO41409849;     gqZNXlukqO41409849 = gqZNXlukqO22871295;     gqZNXlukqO22871295 = gqZNXlukqO75198809;     gqZNXlukqO75198809 = gqZNXlukqO41369222;     gqZNXlukqO41369222 = gqZNXlukqO56288098;     gqZNXlukqO56288098 = gqZNXlukqO14037811;     gqZNXlukqO14037811 = gqZNXlukqO96293221;     gqZNXlukqO96293221 = gqZNXlukqO58244709;     gqZNXlukqO58244709 = gqZNXlukqO88472023;     gqZNXlukqO88472023 = gqZNXlukqO85997302;     gqZNXlukqO85997302 = gqZNXlukqO61020418;     gqZNXlukqO61020418 = gqZNXlukqO16281623;     gqZNXlukqO16281623 = gqZNXlukqO40255011;     gqZNXlukqO40255011 = gqZNXlukqO78383717;     gqZNXlukqO78383717 = gqZNXlukqO36146919;     gqZNXlukqO36146919 = gqZNXlukqO67406887;     gqZNXlukqO67406887 = gqZNXlukqO975625;     gqZNXlukqO975625 = gqZNXlukqO92255789;     gqZNXlukqO92255789 = gqZNXlukqO29171358;     gqZNXlukqO29171358 = gqZNXlukqO69939444;     gqZNXlukqO69939444 = gqZNXlukqO24147610;     gqZNXlukqO24147610 = gqZNXlukqO89883905;     gqZNXlukqO89883905 = gqZNXlukqO75812010;     gqZNXlukqO75812010 = gqZNXlukqO57369076;     gqZNXlukqO57369076 = gqZNXlukqO91231272;     gqZNXlukqO91231272 = gqZNXlukqO6913685;     gqZNXlukqO6913685 = gqZNXlukqO77690608;     gqZNXlukqO77690608 = gqZNXlukqO93846599;     gqZNXlukqO93846599 = gqZNXlukqO99643566;     gqZNXlukqO99643566 = gqZNXlukqO36903179;     gqZNXlukqO36903179 = gqZNXlukqO20353951;     gqZNXlukqO20353951 = gqZNXlukqO43150457;     gqZNXlukqO43150457 = gqZNXlukqO33005783;     gqZNXlukqO33005783 = gqZNXlukqO16208540;     gqZNXlukqO16208540 = gqZNXlukqO22685878;     gqZNXlukqO22685878 = gqZNXlukqO37612265;     gqZNXlukqO37612265 = gqZNXlukqO10238152;     gqZNXlukqO10238152 = gqZNXlukqO27079196;     gqZNXlukqO27079196 = gqZNXlukqO92265456;     gqZNXlukqO92265456 = gqZNXlukqO58415418;     gqZNXlukqO58415418 = gqZNXlukqO82581768;     gqZNXlukqO82581768 = gqZNXlukqO55653933;     gqZNXlukqO55653933 = gqZNXlukqO40402479;     gqZNXlukqO40402479 = gqZNXlukqO52263956;     gqZNXlukqO52263956 = gqZNXlukqO24751595;     gqZNXlukqO24751595 = gqZNXlukqO7458281;     gqZNXlukqO7458281 = gqZNXlukqO4974506;     gqZNXlukqO4974506 = gqZNXlukqO81702768;     gqZNXlukqO81702768 = gqZNXlukqO9219683;     gqZNXlukqO9219683 = gqZNXlukqO97765641;     gqZNXlukqO97765641 = gqZNXlukqO57834033;     gqZNXlukqO57834033 = gqZNXlukqO14720486;     gqZNXlukqO14720486 = gqZNXlukqO20887446;     gqZNXlukqO20887446 = gqZNXlukqO38534079;     gqZNXlukqO38534079 = gqZNXlukqO14302651;     gqZNXlukqO14302651 = gqZNXlukqO51525944;     gqZNXlukqO51525944 = gqZNXlukqO47059285;     gqZNXlukqO47059285 = gqZNXlukqO17829733;     gqZNXlukqO17829733 = gqZNXlukqO50137949;     gqZNXlukqO50137949 = gqZNXlukqO49374414;     gqZNXlukqO49374414 = gqZNXlukqO36347202;     gqZNXlukqO36347202 = gqZNXlukqO2446623;     gqZNXlukqO2446623 = gqZNXlukqO58601142;     gqZNXlukqO58601142 = gqZNXlukqO51568844;     gqZNXlukqO51568844 = gqZNXlukqO65643351;     gqZNXlukqO65643351 = gqZNXlukqO17869962;     gqZNXlukqO17869962 = gqZNXlukqO83275839;     gqZNXlukqO83275839 = gqZNXlukqO24046471;     gqZNXlukqO24046471 = gqZNXlukqO55697840;     gqZNXlukqO55697840 = gqZNXlukqO98534654;     gqZNXlukqO98534654 = gqZNXlukqO57168735;     gqZNXlukqO57168735 = gqZNXlukqO73896428;     gqZNXlukqO73896428 = gqZNXlukqO99990332;     gqZNXlukqO99990332 = gqZNXlukqO64816094;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void JeoCKvgvaP75858176() {     int IfDqJlCtSE87203212 = -956088630;    int IfDqJlCtSE80060336 = 12391378;    int IfDqJlCtSE56099504 = -486223402;    int IfDqJlCtSE8215949 = -462111848;    int IfDqJlCtSE39653561 = -232028402;    int IfDqJlCtSE69541917 = -501816091;    int IfDqJlCtSE56251465 = 99515602;    int IfDqJlCtSE36117419 = -349265676;    int IfDqJlCtSE82600092 = -928329798;    int IfDqJlCtSE60212003 = -911845251;    int IfDqJlCtSE86095414 = -975967221;    int IfDqJlCtSE28356178 = -380984680;    int IfDqJlCtSE45883171 = -456749207;    int IfDqJlCtSE56513394 = -182815636;    int IfDqJlCtSE65609779 = -688288964;    int IfDqJlCtSE82517170 = -260702761;    int IfDqJlCtSE35595365 = -494520550;    int IfDqJlCtSE29716320 = -455659911;    int IfDqJlCtSE86511147 = -118609517;    int IfDqJlCtSE38602769 = -406280739;    int IfDqJlCtSE46249709 = -519685835;    int IfDqJlCtSE28987747 = -162015939;    int IfDqJlCtSE62102980 = -294593984;    int IfDqJlCtSE33476063 = -496412165;    int IfDqJlCtSE85599050 = -744389135;    int IfDqJlCtSE96776517 = -491067573;    int IfDqJlCtSE85539680 = -681888525;    int IfDqJlCtSE47321299 = -522447595;    int IfDqJlCtSE29763856 = -454048739;    int IfDqJlCtSE84355988 = -634241652;    int IfDqJlCtSE85660027 = -963780837;    int IfDqJlCtSE13829290 = -105505389;    int IfDqJlCtSE95076481 = -606029844;    int IfDqJlCtSE55681909 = -62095166;    int IfDqJlCtSE34474074 = -512309853;    int IfDqJlCtSE30164892 = -752454944;    int IfDqJlCtSE79437014 = -229049237;    int IfDqJlCtSE14646520 = -845135773;    int IfDqJlCtSE64589754 = -42422502;    int IfDqJlCtSE60440725 = -198889252;    int IfDqJlCtSE7141089 = -773163134;    int IfDqJlCtSE24655310 = -236121110;    int IfDqJlCtSE72957079 = -443577602;    int IfDqJlCtSE419744 = -801346886;    int IfDqJlCtSE60239056 = -185188092;    int IfDqJlCtSE37271640 = -384732429;    int IfDqJlCtSE82236841 = -918301502;    int IfDqJlCtSE68746284 = -804858536;    int IfDqJlCtSE84343810 = 27128784;    int IfDqJlCtSE72895756 = -898473861;    int IfDqJlCtSE66238292 = -425159309;    int IfDqJlCtSE65082530 = -243020866;    int IfDqJlCtSE85228087 = 40452246;    int IfDqJlCtSE37977154 = -209173816;    int IfDqJlCtSE29608467 = -512611930;    int IfDqJlCtSE58215465 = -694072691;    int IfDqJlCtSE17957357 = -693014639;    int IfDqJlCtSE22623442 = -989811238;    int IfDqJlCtSE22616899 = -717722713;    int IfDqJlCtSE42877043 = -740960829;    int IfDqJlCtSE84002236 = -819927566;    int IfDqJlCtSE8930167 = -378036803;    int IfDqJlCtSE6353563 = -895216937;    int IfDqJlCtSE98244103 = -194088147;    int IfDqJlCtSE74551975 = -948064415;    int IfDqJlCtSE72266125 = -770461832;    int IfDqJlCtSE33279697 = -774954837;    int IfDqJlCtSE90201262 = -294654041;    int IfDqJlCtSE22039321 = -670505784;    int IfDqJlCtSE35444887 = -935834021;    int IfDqJlCtSE3080157 = 68346476;    int IfDqJlCtSE20948846 = -649384778;    int IfDqJlCtSE65126566 = -313237409;    int IfDqJlCtSE26070422 = -919720266;    int IfDqJlCtSE31461681 = -633117606;    int IfDqJlCtSE21594399 = -183564726;    int IfDqJlCtSE56030667 = -718438338;    int IfDqJlCtSE61683236 = -493247099;    int IfDqJlCtSE73237006 = -211224073;    int IfDqJlCtSE48327411 = -259656707;    int IfDqJlCtSE14539676 = -572766071;    int IfDqJlCtSE16793397 = -877029990;    int IfDqJlCtSE62977489 = -449576380;    int IfDqJlCtSE56868100 = -555574878;    int IfDqJlCtSE18117696 = -109082343;    int IfDqJlCtSE20577498 = -620759971;    int IfDqJlCtSE28601202 = -45957636;    int IfDqJlCtSE57099327 = -296856028;    int IfDqJlCtSE26073442 = -549483237;    int IfDqJlCtSE76258608 = -818237162;    int IfDqJlCtSE12207536 = 40559694;    int IfDqJlCtSE56813572 = -239238000;    int IfDqJlCtSE92029620 = -27413060;    int IfDqJlCtSE21712711 = -301461674;    int IfDqJlCtSE76438489 = -378961686;    int IfDqJlCtSE98210921 = -295126332;    int IfDqJlCtSE18301748 = -340904173;    int IfDqJlCtSE74712975 = -149489456;    int IfDqJlCtSE25867768 = -853282472;    int IfDqJlCtSE87972930 = -956088630;     IfDqJlCtSE87203212 = IfDqJlCtSE80060336;     IfDqJlCtSE80060336 = IfDqJlCtSE56099504;     IfDqJlCtSE56099504 = IfDqJlCtSE8215949;     IfDqJlCtSE8215949 = IfDqJlCtSE39653561;     IfDqJlCtSE39653561 = IfDqJlCtSE69541917;     IfDqJlCtSE69541917 = IfDqJlCtSE56251465;     IfDqJlCtSE56251465 = IfDqJlCtSE36117419;     IfDqJlCtSE36117419 = IfDqJlCtSE82600092;     IfDqJlCtSE82600092 = IfDqJlCtSE60212003;     IfDqJlCtSE60212003 = IfDqJlCtSE86095414;     IfDqJlCtSE86095414 = IfDqJlCtSE28356178;     IfDqJlCtSE28356178 = IfDqJlCtSE45883171;     IfDqJlCtSE45883171 = IfDqJlCtSE56513394;     IfDqJlCtSE56513394 = IfDqJlCtSE65609779;     IfDqJlCtSE65609779 = IfDqJlCtSE82517170;     IfDqJlCtSE82517170 = IfDqJlCtSE35595365;     IfDqJlCtSE35595365 = IfDqJlCtSE29716320;     IfDqJlCtSE29716320 = IfDqJlCtSE86511147;     IfDqJlCtSE86511147 = IfDqJlCtSE38602769;     IfDqJlCtSE38602769 = IfDqJlCtSE46249709;     IfDqJlCtSE46249709 = IfDqJlCtSE28987747;     IfDqJlCtSE28987747 = IfDqJlCtSE62102980;     IfDqJlCtSE62102980 = IfDqJlCtSE33476063;     IfDqJlCtSE33476063 = IfDqJlCtSE85599050;     IfDqJlCtSE85599050 = IfDqJlCtSE96776517;     IfDqJlCtSE96776517 = IfDqJlCtSE85539680;     IfDqJlCtSE85539680 = IfDqJlCtSE47321299;     IfDqJlCtSE47321299 = IfDqJlCtSE29763856;     IfDqJlCtSE29763856 = IfDqJlCtSE84355988;     IfDqJlCtSE84355988 = IfDqJlCtSE85660027;     IfDqJlCtSE85660027 = IfDqJlCtSE13829290;     IfDqJlCtSE13829290 = IfDqJlCtSE95076481;     IfDqJlCtSE95076481 = IfDqJlCtSE55681909;     IfDqJlCtSE55681909 = IfDqJlCtSE34474074;     IfDqJlCtSE34474074 = IfDqJlCtSE30164892;     IfDqJlCtSE30164892 = IfDqJlCtSE79437014;     IfDqJlCtSE79437014 = IfDqJlCtSE14646520;     IfDqJlCtSE14646520 = IfDqJlCtSE64589754;     IfDqJlCtSE64589754 = IfDqJlCtSE60440725;     IfDqJlCtSE60440725 = IfDqJlCtSE7141089;     IfDqJlCtSE7141089 = IfDqJlCtSE24655310;     IfDqJlCtSE24655310 = IfDqJlCtSE72957079;     IfDqJlCtSE72957079 = IfDqJlCtSE419744;     IfDqJlCtSE419744 = IfDqJlCtSE60239056;     IfDqJlCtSE60239056 = IfDqJlCtSE37271640;     IfDqJlCtSE37271640 = IfDqJlCtSE82236841;     IfDqJlCtSE82236841 = IfDqJlCtSE68746284;     IfDqJlCtSE68746284 = IfDqJlCtSE84343810;     IfDqJlCtSE84343810 = IfDqJlCtSE72895756;     IfDqJlCtSE72895756 = IfDqJlCtSE66238292;     IfDqJlCtSE66238292 = IfDqJlCtSE65082530;     IfDqJlCtSE65082530 = IfDqJlCtSE85228087;     IfDqJlCtSE85228087 = IfDqJlCtSE37977154;     IfDqJlCtSE37977154 = IfDqJlCtSE29608467;     IfDqJlCtSE29608467 = IfDqJlCtSE58215465;     IfDqJlCtSE58215465 = IfDqJlCtSE17957357;     IfDqJlCtSE17957357 = IfDqJlCtSE22623442;     IfDqJlCtSE22623442 = IfDqJlCtSE22616899;     IfDqJlCtSE22616899 = IfDqJlCtSE42877043;     IfDqJlCtSE42877043 = IfDqJlCtSE84002236;     IfDqJlCtSE84002236 = IfDqJlCtSE8930167;     IfDqJlCtSE8930167 = IfDqJlCtSE6353563;     IfDqJlCtSE6353563 = IfDqJlCtSE98244103;     IfDqJlCtSE98244103 = IfDqJlCtSE74551975;     IfDqJlCtSE74551975 = IfDqJlCtSE72266125;     IfDqJlCtSE72266125 = IfDqJlCtSE33279697;     IfDqJlCtSE33279697 = IfDqJlCtSE90201262;     IfDqJlCtSE90201262 = IfDqJlCtSE22039321;     IfDqJlCtSE22039321 = IfDqJlCtSE35444887;     IfDqJlCtSE35444887 = IfDqJlCtSE3080157;     IfDqJlCtSE3080157 = IfDqJlCtSE20948846;     IfDqJlCtSE20948846 = IfDqJlCtSE65126566;     IfDqJlCtSE65126566 = IfDqJlCtSE26070422;     IfDqJlCtSE26070422 = IfDqJlCtSE31461681;     IfDqJlCtSE31461681 = IfDqJlCtSE21594399;     IfDqJlCtSE21594399 = IfDqJlCtSE56030667;     IfDqJlCtSE56030667 = IfDqJlCtSE61683236;     IfDqJlCtSE61683236 = IfDqJlCtSE73237006;     IfDqJlCtSE73237006 = IfDqJlCtSE48327411;     IfDqJlCtSE48327411 = IfDqJlCtSE14539676;     IfDqJlCtSE14539676 = IfDqJlCtSE16793397;     IfDqJlCtSE16793397 = IfDqJlCtSE62977489;     IfDqJlCtSE62977489 = IfDqJlCtSE56868100;     IfDqJlCtSE56868100 = IfDqJlCtSE18117696;     IfDqJlCtSE18117696 = IfDqJlCtSE20577498;     IfDqJlCtSE20577498 = IfDqJlCtSE28601202;     IfDqJlCtSE28601202 = IfDqJlCtSE57099327;     IfDqJlCtSE57099327 = IfDqJlCtSE26073442;     IfDqJlCtSE26073442 = IfDqJlCtSE76258608;     IfDqJlCtSE76258608 = IfDqJlCtSE12207536;     IfDqJlCtSE12207536 = IfDqJlCtSE56813572;     IfDqJlCtSE56813572 = IfDqJlCtSE92029620;     IfDqJlCtSE92029620 = IfDqJlCtSE21712711;     IfDqJlCtSE21712711 = IfDqJlCtSE76438489;     IfDqJlCtSE76438489 = IfDqJlCtSE98210921;     IfDqJlCtSE98210921 = IfDqJlCtSE18301748;     IfDqJlCtSE18301748 = IfDqJlCtSE74712975;     IfDqJlCtSE74712975 = IfDqJlCtSE25867768;     IfDqJlCtSE25867768 = IfDqJlCtSE87972930;     IfDqJlCtSE87972930 = IfDqJlCtSE87203212;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void VYRWovYoUO12503151() {     int LheJJWHwEL9590330 = -590492593;    int LheJJWHwEL64407837 = 63848323;    int LheJJWHwEL84868783 = -725877246;    int LheJJWHwEL23222637 = -468519380;    int LheJJWHwEL37827815 = -823085403;    int LheJJWHwEL84425034 = -362424469;    int LheJJWHwEL48407204 = -822471704;    int LheJJWHwEL11751277 = -819862320;    int LheJJWHwEL79763223 = -812525837;    int LheJJWHwEL51975587 = -654586584;    int LheJJWHwEL23637275 = -972591622;    int LheJJWHwEL84259127 = -631729974;    int LheJJWHwEL12891352 = 24533473;    int LheJJWHwEL99128146 = -710524393;    int LheJJWHwEL2345057 = -315766677;    int LheJJWHwEL26773083 = -642370303;    int LheJJWHwEL85418716 = -771800370;    int LheJJWHwEL35692736 = -907402602;    int LheJJWHwEL27792778 = -253711643;    int LheJJWHwEL17119053 = -658915193;    int LheJJWHwEL47132816 = -511173144;    int LheJJWHwEL92802964 = -666782252;    int LheJJWHwEL65396303 = -470674576;    int LheJJWHwEL59975849 = 39594978;    int LheJJWHwEL21139297 = -365948260;    int LheJJWHwEL85079512 = -934961797;    int LheJJWHwEL32629101 = -439475018;    int LheJJWHwEL53232749 = -310902049;    int LheJJWHwEL36656417 = -798305858;    int LheJJWHwEL93513167 = -554537518;    int LheJJWHwEL29950833 = 54903011;    int LheJJWHwEL71370480 = -757576744;    int LheJJWHwEL76115152 = -35373339;    int LheJJWHwEL15070597 = -386222017;    int LheJJWHwEL10703439 = -576066276;    int LheJJWHwEL71857761 = 32149680;    int LheJJWHwEL72876726 = 38002811;    int LheJJWHwEL68272620 = -712761245;    int LheJJWHwEL12897886 = -443468534;    int LheJJWHwEL80626440 = -335113281;    int LheJJWHwEL35898459 = -176340800;    int LheJJWHwEL13163702 = 5970199;    int LheJJWHwEL78507271 = -331108185;    int LheJJWHwEL99863862 = -798710535;    int LheJJWHwEL28222323 = 5668431;    int LheJJWHwEL45371921 = -566961524;    int LheJJWHwEL94534239 = -396711755;    int LheJJWHwEL13344959 = -561393527;    int LheJJWHwEL78803715 = 77412086;    int LheJJWHwEL69979502 = 59505880;    int LheJJWHwEL75107508 = -780235586;    int LheJJWHwEL38933787 = -673840148;    int LheJJWHwEL63542491 = -731458984;    int LheJJWHwEL98263699 = -484854625;    int LheJJWHwEL65370334 = -653316045;    int LheJJWHwEL16787365 = -923710341;    int LheJJWHwEL99011533 = -465477101;    int LheJJWHwEL24892934 = -665472225;    int LheJJWHwEL2083340 = -2571120;    int LheJJWHwEL52748302 = -888123606;    int LheJJWHwEL51795933 = -922949451;    int LheJJWHwEL95174455 = -411569655;    int LheJJWHwEL75094860 = 78443538;    int LheJJWHwEL86250055 = -157988319;    int LheJJWHwEL22024755 = -609489595;    int LheJJWHwEL52266794 = -115014878;    int LheJJWHwEL8143976 = -496356635;    int LheJJWHwEL97820755 = -589244511;    int LheJJWHwEL88424708 = -34458118;    int LheJJWHwEL30487295 = -247916358;    int LheJJWHwEL53896357 = -580373115;    int LheJJWHwEL17146097 = 40960874;    int LheJJWHwEL22794851 = -363934068;    int LheJJWHwEL47166338 = -918598362;    int LheJJWHwEL81220593 = -382574393;    int LheJJWHwEL33969115 = -417143343;    int LheJJWHwEL14295694 = -235674067;    int LheJJWHwEL65532440 = -671964042;    int LheJJWHwEL31753527 = -966073453;    int LheJJWHwEL75767375 = -798986736;    int LheJJWHwEL90545273 = -438250042;    int LheJJWHwEL19284143 = -878081492;    int LheJJWHwEL74429034 = -288314135;    int LheJJWHwEL66676915 = -757811738;    int LheJJWHwEL18405659 = -774301932;    int LheJJWHwEL91017045 = -271256841;    int LheJJWHwEL7827990 = 73882239;    int LheJJWHwEL77851452 = -550518714;    int LheJJWHwEL49700262 = -732905972;    int LheJJWHwEL93916073 = -652355935;    int LheJJWHwEL72846227 = -502373220;    int LheJJWHwEL47983793 = -296524964;    int LheJJWHwEL66189280 = -610190125;    int LheJJWHwEL60149583 = -555344928;    int LheJJWHwEL28830507 = -412163830;    int LheJJWHwEL40724004 = -764771145;    int LheJJWHwEL38068841 = 27526661;    int LheJJWHwEL92257215 = -73119867;    int LheJJWHwEL77839107 = -89220940;    int LheJJWHwEL75955528 = -590492593;     LheJJWHwEL9590330 = LheJJWHwEL64407837;     LheJJWHwEL64407837 = LheJJWHwEL84868783;     LheJJWHwEL84868783 = LheJJWHwEL23222637;     LheJJWHwEL23222637 = LheJJWHwEL37827815;     LheJJWHwEL37827815 = LheJJWHwEL84425034;     LheJJWHwEL84425034 = LheJJWHwEL48407204;     LheJJWHwEL48407204 = LheJJWHwEL11751277;     LheJJWHwEL11751277 = LheJJWHwEL79763223;     LheJJWHwEL79763223 = LheJJWHwEL51975587;     LheJJWHwEL51975587 = LheJJWHwEL23637275;     LheJJWHwEL23637275 = LheJJWHwEL84259127;     LheJJWHwEL84259127 = LheJJWHwEL12891352;     LheJJWHwEL12891352 = LheJJWHwEL99128146;     LheJJWHwEL99128146 = LheJJWHwEL2345057;     LheJJWHwEL2345057 = LheJJWHwEL26773083;     LheJJWHwEL26773083 = LheJJWHwEL85418716;     LheJJWHwEL85418716 = LheJJWHwEL35692736;     LheJJWHwEL35692736 = LheJJWHwEL27792778;     LheJJWHwEL27792778 = LheJJWHwEL17119053;     LheJJWHwEL17119053 = LheJJWHwEL47132816;     LheJJWHwEL47132816 = LheJJWHwEL92802964;     LheJJWHwEL92802964 = LheJJWHwEL65396303;     LheJJWHwEL65396303 = LheJJWHwEL59975849;     LheJJWHwEL59975849 = LheJJWHwEL21139297;     LheJJWHwEL21139297 = LheJJWHwEL85079512;     LheJJWHwEL85079512 = LheJJWHwEL32629101;     LheJJWHwEL32629101 = LheJJWHwEL53232749;     LheJJWHwEL53232749 = LheJJWHwEL36656417;     LheJJWHwEL36656417 = LheJJWHwEL93513167;     LheJJWHwEL93513167 = LheJJWHwEL29950833;     LheJJWHwEL29950833 = LheJJWHwEL71370480;     LheJJWHwEL71370480 = LheJJWHwEL76115152;     LheJJWHwEL76115152 = LheJJWHwEL15070597;     LheJJWHwEL15070597 = LheJJWHwEL10703439;     LheJJWHwEL10703439 = LheJJWHwEL71857761;     LheJJWHwEL71857761 = LheJJWHwEL72876726;     LheJJWHwEL72876726 = LheJJWHwEL68272620;     LheJJWHwEL68272620 = LheJJWHwEL12897886;     LheJJWHwEL12897886 = LheJJWHwEL80626440;     LheJJWHwEL80626440 = LheJJWHwEL35898459;     LheJJWHwEL35898459 = LheJJWHwEL13163702;     LheJJWHwEL13163702 = LheJJWHwEL78507271;     LheJJWHwEL78507271 = LheJJWHwEL99863862;     LheJJWHwEL99863862 = LheJJWHwEL28222323;     LheJJWHwEL28222323 = LheJJWHwEL45371921;     LheJJWHwEL45371921 = LheJJWHwEL94534239;     LheJJWHwEL94534239 = LheJJWHwEL13344959;     LheJJWHwEL13344959 = LheJJWHwEL78803715;     LheJJWHwEL78803715 = LheJJWHwEL69979502;     LheJJWHwEL69979502 = LheJJWHwEL75107508;     LheJJWHwEL75107508 = LheJJWHwEL38933787;     LheJJWHwEL38933787 = LheJJWHwEL63542491;     LheJJWHwEL63542491 = LheJJWHwEL98263699;     LheJJWHwEL98263699 = LheJJWHwEL65370334;     LheJJWHwEL65370334 = LheJJWHwEL16787365;     LheJJWHwEL16787365 = LheJJWHwEL99011533;     LheJJWHwEL99011533 = LheJJWHwEL24892934;     LheJJWHwEL24892934 = LheJJWHwEL2083340;     LheJJWHwEL2083340 = LheJJWHwEL52748302;     LheJJWHwEL52748302 = LheJJWHwEL51795933;     LheJJWHwEL51795933 = LheJJWHwEL95174455;     LheJJWHwEL95174455 = LheJJWHwEL75094860;     LheJJWHwEL75094860 = LheJJWHwEL86250055;     LheJJWHwEL86250055 = LheJJWHwEL22024755;     LheJJWHwEL22024755 = LheJJWHwEL52266794;     LheJJWHwEL52266794 = LheJJWHwEL8143976;     LheJJWHwEL8143976 = LheJJWHwEL97820755;     LheJJWHwEL97820755 = LheJJWHwEL88424708;     LheJJWHwEL88424708 = LheJJWHwEL30487295;     LheJJWHwEL30487295 = LheJJWHwEL53896357;     LheJJWHwEL53896357 = LheJJWHwEL17146097;     LheJJWHwEL17146097 = LheJJWHwEL22794851;     LheJJWHwEL22794851 = LheJJWHwEL47166338;     LheJJWHwEL47166338 = LheJJWHwEL81220593;     LheJJWHwEL81220593 = LheJJWHwEL33969115;     LheJJWHwEL33969115 = LheJJWHwEL14295694;     LheJJWHwEL14295694 = LheJJWHwEL65532440;     LheJJWHwEL65532440 = LheJJWHwEL31753527;     LheJJWHwEL31753527 = LheJJWHwEL75767375;     LheJJWHwEL75767375 = LheJJWHwEL90545273;     LheJJWHwEL90545273 = LheJJWHwEL19284143;     LheJJWHwEL19284143 = LheJJWHwEL74429034;     LheJJWHwEL74429034 = LheJJWHwEL66676915;     LheJJWHwEL66676915 = LheJJWHwEL18405659;     LheJJWHwEL18405659 = LheJJWHwEL91017045;     LheJJWHwEL91017045 = LheJJWHwEL7827990;     LheJJWHwEL7827990 = LheJJWHwEL77851452;     LheJJWHwEL77851452 = LheJJWHwEL49700262;     LheJJWHwEL49700262 = LheJJWHwEL93916073;     LheJJWHwEL93916073 = LheJJWHwEL72846227;     LheJJWHwEL72846227 = LheJJWHwEL47983793;     LheJJWHwEL47983793 = LheJJWHwEL66189280;     LheJJWHwEL66189280 = LheJJWHwEL60149583;     LheJJWHwEL60149583 = LheJJWHwEL28830507;     LheJJWHwEL28830507 = LheJJWHwEL40724004;     LheJJWHwEL40724004 = LheJJWHwEL38068841;     LheJJWHwEL38068841 = LheJJWHwEL92257215;     LheJJWHwEL92257215 = LheJJWHwEL77839107;     LheJJWHwEL77839107 = LheJJWHwEL75955528;     LheJJWHwEL75955528 = LheJJWHwEL9590330;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void niHXCccrCF49148125() {     int ZUPWSTUHWv31977447 = -224896556;    int ZUPWSTUHWv48755337 = -984694731;    int ZUPWSTUHWv13638062 = -965531090;    int ZUPWSTUHWv38229325 = -474926911;    int ZUPWSTUHWv36002069 = -314142404;    int ZUPWSTUHWv99308150 = -223032847;    int ZUPWSTUHWv40562943 = -644459010;    int ZUPWSTUHWv87385135 = -190458964;    int ZUPWSTUHWv76926354 = -696721876;    int ZUPWSTUHWv43739172 = -397327917;    int ZUPWSTUHWv61179134 = -969216023;    int ZUPWSTUHWv40162076 = -882475267;    int ZUPWSTUHWv79899532 = -594183846;    int ZUPWSTUHWv41742899 = -138233150;    int ZUPWSTUHWv39080333 = 56755610;    int ZUPWSTUHWv71028995 = 75962154;    int ZUPWSTUHWv35242069 = 50919810;    int ZUPWSTUHWv41669153 = -259145292;    int ZUPWSTUHWv69074409 = -388813768;    int ZUPWSTUHWv95635335 = -911549646;    int ZUPWSTUHWv48015923 = -502660452;    int ZUPWSTUHWv56618183 = -71548566;    int ZUPWSTUHWv68689626 = -646755168;    int ZUPWSTUHWv86475636 = -524397879;    int ZUPWSTUHWv56679544 = 12492616;    int ZUPWSTUHWv73382507 = -278856021;    int ZUPWSTUHWv79718521 = -197061511;    int ZUPWSTUHWv59144199 = -99356502;    int ZUPWSTUHWv43548978 = -42562977;    int ZUPWSTUHWv2670348 = -474833385;    int ZUPWSTUHWv74241637 = -26413141;    int ZUPWSTUHWv28911671 = -309648099;    int ZUPWSTUHWv57153822 = -564716834;    int ZUPWSTUHWv74459284 = -710348867;    int ZUPWSTUHWv86932802 = -639822698;    int ZUPWSTUHWv13550631 = -283245697;    int ZUPWSTUHWv66316438 = -794945140;    int ZUPWSTUHWv21898721 = -580386717;    int ZUPWSTUHWv61206016 = -844514565;    int ZUPWSTUHWv812155 = -471337311;    int ZUPWSTUHWv64655830 = -679518466;    int ZUPWSTUHWv1672093 = -851938492;    int ZUPWSTUHWv84057463 = -218638769;    int ZUPWSTUHWv99307981 = -796074184;    int ZUPWSTUHWv96205589 = -903475047;    int ZUPWSTUHWv53472203 = -749190620;    int ZUPWSTUHWv6831637 = -975122008;    int ZUPWSTUHWv57943633 = -317928518;    int ZUPWSTUHWv73263620 = -972304612;    int ZUPWSTUHWv67063247 = -82514379;    int ZUPWSTUHWv83976725 = -35311863;    int ZUPWSTUHWv12785045 = -4659431;    int ZUPWSTUHWv41856894 = -403370214;    int ZUPWSTUHWv58550244 = -760535434;    int ZUPWSTUHWv1132202 = -794020160;    int ZUPWSTUHWv75359263 = -53347991;    int ZUPWSTUHWv80065711 = -237939564;    int ZUPWSTUHWv27162425 = -341133212;    int ZUPWSTUHWv81549781 = -387419527;    int ZUPWSTUHWv62619562 = 64713616;    int ZUPWSTUHWv19589630 = 74028663;    int ZUPWSTUHWv81418744 = -445102508;    int ZUPWSTUHWv43836158 = -47895987;    int ZUPWSTUHWv74256007 = -121888492;    int ZUPWSTUHWv69497534 = -270914776;    int ZUPWSTUHWv32267463 = -559567924;    int ZUPWSTUHWv83008253 = -217758434;    int ZUPWSTUHWv5440249 = -883834980;    int ZUPWSTUHWv54810096 = -498410453;    int ZUPWSTUHWv25529703 = -659998694;    int ZUPWSTUHWv4712557 = -129092706;    int ZUPWSTUHWv13343348 = -368693474;    int ZUPWSTUHWv80463136 = -414630727;    int ZUPWSTUHWv68262254 = -917476458;    int ZUPWSTUHWv30979506 = -132031181;    int ZUPWSTUHWv46343831 = -650721961;    int ZUPWSTUHWv72560720 = -852909797;    int ZUPWSTUHWv69381644 = -850680985;    int ZUPWSTUHWv90270047 = -620922833;    int ZUPWSTUHWv3207341 = -238316765;    int ZUPWSTUHWv66550870 = -303734013;    int ZUPWSTUHWv21774889 = -879132994;    int ZUPWSTUHWv85880579 = -127051890;    int ZUPWSTUHWv76485730 = -960048598;    int ZUPWSTUHWv18693622 = -339521522;    int ZUPWSTUHWv61456593 = 78246289;    int ZUPWSTUHWv87054776 = -906277885;    int ZUPWSTUHWv98603577 = -804181401;    int ZUPWSTUHWv73327082 = -916328707;    int ZUPWSTUHWv11573540 = -486474708;    int ZUPWSTUHWv33484920 = 54693867;    int ZUPWSTUHWv39154013 = -353811929;    int ZUPWSTUHWv40348939 = -92967190;    int ZUPWSTUHWv98586454 = -809228182;    int ZUPWSTUHWv81222525 = -445365975;    int ZUPWSTUHWv83237086 = -134415959;    int ZUPWSTUHWv57835934 = -704042505;    int ZUPWSTUHWv9801457 = 3249722;    int ZUPWSTUHWv29810448 = -425159408;    int ZUPWSTUHWv63938126 = -224896556;     ZUPWSTUHWv31977447 = ZUPWSTUHWv48755337;     ZUPWSTUHWv48755337 = ZUPWSTUHWv13638062;     ZUPWSTUHWv13638062 = ZUPWSTUHWv38229325;     ZUPWSTUHWv38229325 = ZUPWSTUHWv36002069;     ZUPWSTUHWv36002069 = ZUPWSTUHWv99308150;     ZUPWSTUHWv99308150 = ZUPWSTUHWv40562943;     ZUPWSTUHWv40562943 = ZUPWSTUHWv87385135;     ZUPWSTUHWv87385135 = ZUPWSTUHWv76926354;     ZUPWSTUHWv76926354 = ZUPWSTUHWv43739172;     ZUPWSTUHWv43739172 = ZUPWSTUHWv61179134;     ZUPWSTUHWv61179134 = ZUPWSTUHWv40162076;     ZUPWSTUHWv40162076 = ZUPWSTUHWv79899532;     ZUPWSTUHWv79899532 = ZUPWSTUHWv41742899;     ZUPWSTUHWv41742899 = ZUPWSTUHWv39080333;     ZUPWSTUHWv39080333 = ZUPWSTUHWv71028995;     ZUPWSTUHWv71028995 = ZUPWSTUHWv35242069;     ZUPWSTUHWv35242069 = ZUPWSTUHWv41669153;     ZUPWSTUHWv41669153 = ZUPWSTUHWv69074409;     ZUPWSTUHWv69074409 = ZUPWSTUHWv95635335;     ZUPWSTUHWv95635335 = ZUPWSTUHWv48015923;     ZUPWSTUHWv48015923 = ZUPWSTUHWv56618183;     ZUPWSTUHWv56618183 = ZUPWSTUHWv68689626;     ZUPWSTUHWv68689626 = ZUPWSTUHWv86475636;     ZUPWSTUHWv86475636 = ZUPWSTUHWv56679544;     ZUPWSTUHWv56679544 = ZUPWSTUHWv73382507;     ZUPWSTUHWv73382507 = ZUPWSTUHWv79718521;     ZUPWSTUHWv79718521 = ZUPWSTUHWv59144199;     ZUPWSTUHWv59144199 = ZUPWSTUHWv43548978;     ZUPWSTUHWv43548978 = ZUPWSTUHWv2670348;     ZUPWSTUHWv2670348 = ZUPWSTUHWv74241637;     ZUPWSTUHWv74241637 = ZUPWSTUHWv28911671;     ZUPWSTUHWv28911671 = ZUPWSTUHWv57153822;     ZUPWSTUHWv57153822 = ZUPWSTUHWv74459284;     ZUPWSTUHWv74459284 = ZUPWSTUHWv86932802;     ZUPWSTUHWv86932802 = ZUPWSTUHWv13550631;     ZUPWSTUHWv13550631 = ZUPWSTUHWv66316438;     ZUPWSTUHWv66316438 = ZUPWSTUHWv21898721;     ZUPWSTUHWv21898721 = ZUPWSTUHWv61206016;     ZUPWSTUHWv61206016 = ZUPWSTUHWv812155;     ZUPWSTUHWv812155 = ZUPWSTUHWv64655830;     ZUPWSTUHWv64655830 = ZUPWSTUHWv1672093;     ZUPWSTUHWv1672093 = ZUPWSTUHWv84057463;     ZUPWSTUHWv84057463 = ZUPWSTUHWv99307981;     ZUPWSTUHWv99307981 = ZUPWSTUHWv96205589;     ZUPWSTUHWv96205589 = ZUPWSTUHWv53472203;     ZUPWSTUHWv53472203 = ZUPWSTUHWv6831637;     ZUPWSTUHWv6831637 = ZUPWSTUHWv57943633;     ZUPWSTUHWv57943633 = ZUPWSTUHWv73263620;     ZUPWSTUHWv73263620 = ZUPWSTUHWv67063247;     ZUPWSTUHWv67063247 = ZUPWSTUHWv83976725;     ZUPWSTUHWv83976725 = ZUPWSTUHWv12785045;     ZUPWSTUHWv12785045 = ZUPWSTUHWv41856894;     ZUPWSTUHWv41856894 = ZUPWSTUHWv58550244;     ZUPWSTUHWv58550244 = ZUPWSTUHWv1132202;     ZUPWSTUHWv1132202 = ZUPWSTUHWv75359263;     ZUPWSTUHWv75359263 = ZUPWSTUHWv80065711;     ZUPWSTUHWv80065711 = ZUPWSTUHWv27162425;     ZUPWSTUHWv27162425 = ZUPWSTUHWv81549781;     ZUPWSTUHWv81549781 = ZUPWSTUHWv62619562;     ZUPWSTUHWv62619562 = ZUPWSTUHWv19589630;     ZUPWSTUHWv19589630 = ZUPWSTUHWv81418744;     ZUPWSTUHWv81418744 = ZUPWSTUHWv43836158;     ZUPWSTUHWv43836158 = ZUPWSTUHWv74256007;     ZUPWSTUHWv74256007 = ZUPWSTUHWv69497534;     ZUPWSTUHWv69497534 = ZUPWSTUHWv32267463;     ZUPWSTUHWv32267463 = ZUPWSTUHWv83008253;     ZUPWSTUHWv83008253 = ZUPWSTUHWv5440249;     ZUPWSTUHWv5440249 = ZUPWSTUHWv54810096;     ZUPWSTUHWv54810096 = ZUPWSTUHWv25529703;     ZUPWSTUHWv25529703 = ZUPWSTUHWv4712557;     ZUPWSTUHWv4712557 = ZUPWSTUHWv13343348;     ZUPWSTUHWv13343348 = ZUPWSTUHWv80463136;     ZUPWSTUHWv80463136 = ZUPWSTUHWv68262254;     ZUPWSTUHWv68262254 = ZUPWSTUHWv30979506;     ZUPWSTUHWv30979506 = ZUPWSTUHWv46343831;     ZUPWSTUHWv46343831 = ZUPWSTUHWv72560720;     ZUPWSTUHWv72560720 = ZUPWSTUHWv69381644;     ZUPWSTUHWv69381644 = ZUPWSTUHWv90270047;     ZUPWSTUHWv90270047 = ZUPWSTUHWv3207341;     ZUPWSTUHWv3207341 = ZUPWSTUHWv66550870;     ZUPWSTUHWv66550870 = ZUPWSTUHWv21774889;     ZUPWSTUHWv21774889 = ZUPWSTUHWv85880579;     ZUPWSTUHWv85880579 = ZUPWSTUHWv76485730;     ZUPWSTUHWv76485730 = ZUPWSTUHWv18693622;     ZUPWSTUHWv18693622 = ZUPWSTUHWv61456593;     ZUPWSTUHWv61456593 = ZUPWSTUHWv87054776;     ZUPWSTUHWv87054776 = ZUPWSTUHWv98603577;     ZUPWSTUHWv98603577 = ZUPWSTUHWv73327082;     ZUPWSTUHWv73327082 = ZUPWSTUHWv11573540;     ZUPWSTUHWv11573540 = ZUPWSTUHWv33484920;     ZUPWSTUHWv33484920 = ZUPWSTUHWv39154013;     ZUPWSTUHWv39154013 = ZUPWSTUHWv40348939;     ZUPWSTUHWv40348939 = ZUPWSTUHWv98586454;     ZUPWSTUHWv98586454 = ZUPWSTUHWv81222525;     ZUPWSTUHWv81222525 = ZUPWSTUHWv83237086;     ZUPWSTUHWv83237086 = ZUPWSTUHWv57835934;     ZUPWSTUHWv57835934 = ZUPWSTUHWv9801457;     ZUPWSTUHWv9801457 = ZUPWSTUHWv29810448;     ZUPWSTUHWv29810448 = ZUPWSTUHWv63938126;     ZUPWSTUHWv63938126 = ZUPWSTUHWv31977447;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void hoxDepFYoO85793100() {     int DTaHzdiubo54364564 = -959300519;    int DTaHzdiubo33102838 = -933237786;    int DTaHzdiubo42407341 = -105184934;    int DTaHzdiubo53236014 = -481334443;    int DTaHzdiubo34176323 = -905199405;    int DTaHzdiubo14191268 = -83641225;    int DTaHzdiubo32718682 = -466446316;    int DTaHzdiubo63018994 = -661055608;    int DTaHzdiubo74089485 = -580917915;    int DTaHzdiubo35502757 = -140069250;    int DTaHzdiubo98720993 = -965840423;    int DTaHzdiubo96065024 = -33220560;    int DTaHzdiubo46907713 = -112901166;    int DTaHzdiubo84357650 = -665941907;    int DTaHzdiubo75815610 = -670722103;    int DTaHzdiubo15284908 = -305705388;    int DTaHzdiubo85065420 = -226360010;    int DTaHzdiubo47645569 = -710887983;    int DTaHzdiubo10356040 = -523915893;    int DTaHzdiubo74151619 = -64184100;    int DTaHzdiubo48899030 = -494147760;    int DTaHzdiubo20433402 = -576314879;    int DTaHzdiubo71982949 = -822835760;    int DTaHzdiubo12975424 = 11609264;    int DTaHzdiubo92219790 = -709066509;    int DTaHzdiubo61685501 = -722750245;    int DTaHzdiubo26807942 = 45351996;    int DTaHzdiubo65055649 = -987810956;    int DTaHzdiubo50441538 = -386820096;    int DTaHzdiubo11827527 = -395129251;    int DTaHzdiubo18532443 = -107729293;    int DTaHzdiubo86452862 = -961719453;    int DTaHzdiubo38192493 = 5939671;    int DTaHzdiubo33847971 = 65524283;    int DTaHzdiubo63162167 = -703579121;    int DTaHzdiubo55243500 = -598641073;    int DTaHzdiubo59756151 = -527893092;    int DTaHzdiubo75524822 = -448012189;    int DTaHzdiubo9514148 = -145560597;    int DTaHzdiubo20997870 = -607561340;    int DTaHzdiubo93413201 = -82696132;    int DTaHzdiubo90180483 = -609847183;    int DTaHzdiubo89607655 = -106169353;    int DTaHzdiubo98752101 = -793437833;    int DTaHzdiubo64188856 = -712618525;    int DTaHzdiubo61572485 = -931419715;    int DTaHzdiubo19129034 = -453532261;    int DTaHzdiubo2542308 = -74463509;    int DTaHzdiubo67723525 = -922021311;    int DTaHzdiubo64146993 = -224534638;    int DTaHzdiubo92845941 = -390388140;    int DTaHzdiubo86636301 = -435478713;    int DTaHzdiubo20171298 = -75281444;    int DTaHzdiubo18836790 = 63783758;    int DTaHzdiubo36894070 = -934724276;    int DTaHzdiubo33931163 = -282985641;    int DTaHzdiubo61119888 = -10402026;    int DTaHzdiubo29431917 = -16794198;    int DTaHzdiubo61016223 = -772267935;    int DTaHzdiubo72490821 = -82449161;    int DTaHzdiubo87383326 = -28993222;    int DTaHzdiubo67663033 = -478635360;    int DTaHzdiubo12577456 = -174235512;    int DTaHzdiubo62261958 = -85788664;    int DTaHzdiubo16970314 = 67660043;    int DTaHzdiubo12268132 = 95879030;    int DTaHzdiubo57872532 = 60839768;    int DTaHzdiubo13059742 = -78425450;    int DTaHzdiubo21195484 = -962362787;    int DTaHzdiubo20572111 = 27918969;    int DTaHzdiubo55528757 = -777812297;    int DTaHzdiubo9540599 = -778347822;    int DTaHzdiubo38131421 = -465327386;    int DTaHzdiubo89358170 = -916354554;    int DTaHzdiubo80738418 = -981487968;    int DTaHzdiubo58718547 = -884300578;    int DTaHzdiubo30825747 = -370145527;    int DTaHzdiubo73230848 = 70602072;    int DTaHzdiubo48786568 = -275772212;    int DTaHzdiubo30647306 = -777646794;    int DTaHzdiubo42556468 = -169217984;    int DTaHzdiubo24265634 = -880184496;    int DTaHzdiubo97332124 = 34210354;    int DTaHzdiubo86294545 = -62285458;    int DTaHzdiubo18981585 = 95258889;    int DTaHzdiubo31896141 = -672250581;    int DTaHzdiubo66281564 = -786438010;    int DTaHzdiubo19355703 = 42155913;    int DTaHzdiubo96953901 = 248558;    int DTaHzdiubo29231005 = -320593480;    int DTaHzdiubo94123611 = -488239047;    int DTaHzdiubo30324234 = -411098894;    int DTaHzdiubo14508599 = -675744255;    int DTaHzdiubo37023326 = 36888564;    int DTaHzdiubo33614543 = -478568119;    int DTaHzdiubo25750168 = -604060772;    int DTaHzdiubo77603027 = -335611671;    int DTaHzdiubo27345697 = 79619311;    int DTaHzdiubo81781787 = -761097876;    int DTaHzdiubo51920724 = -959300519;     DTaHzdiubo54364564 = DTaHzdiubo33102838;     DTaHzdiubo33102838 = DTaHzdiubo42407341;     DTaHzdiubo42407341 = DTaHzdiubo53236014;     DTaHzdiubo53236014 = DTaHzdiubo34176323;     DTaHzdiubo34176323 = DTaHzdiubo14191268;     DTaHzdiubo14191268 = DTaHzdiubo32718682;     DTaHzdiubo32718682 = DTaHzdiubo63018994;     DTaHzdiubo63018994 = DTaHzdiubo74089485;     DTaHzdiubo74089485 = DTaHzdiubo35502757;     DTaHzdiubo35502757 = DTaHzdiubo98720993;     DTaHzdiubo98720993 = DTaHzdiubo96065024;     DTaHzdiubo96065024 = DTaHzdiubo46907713;     DTaHzdiubo46907713 = DTaHzdiubo84357650;     DTaHzdiubo84357650 = DTaHzdiubo75815610;     DTaHzdiubo75815610 = DTaHzdiubo15284908;     DTaHzdiubo15284908 = DTaHzdiubo85065420;     DTaHzdiubo85065420 = DTaHzdiubo47645569;     DTaHzdiubo47645569 = DTaHzdiubo10356040;     DTaHzdiubo10356040 = DTaHzdiubo74151619;     DTaHzdiubo74151619 = DTaHzdiubo48899030;     DTaHzdiubo48899030 = DTaHzdiubo20433402;     DTaHzdiubo20433402 = DTaHzdiubo71982949;     DTaHzdiubo71982949 = DTaHzdiubo12975424;     DTaHzdiubo12975424 = DTaHzdiubo92219790;     DTaHzdiubo92219790 = DTaHzdiubo61685501;     DTaHzdiubo61685501 = DTaHzdiubo26807942;     DTaHzdiubo26807942 = DTaHzdiubo65055649;     DTaHzdiubo65055649 = DTaHzdiubo50441538;     DTaHzdiubo50441538 = DTaHzdiubo11827527;     DTaHzdiubo11827527 = DTaHzdiubo18532443;     DTaHzdiubo18532443 = DTaHzdiubo86452862;     DTaHzdiubo86452862 = DTaHzdiubo38192493;     DTaHzdiubo38192493 = DTaHzdiubo33847971;     DTaHzdiubo33847971 = DTaHzdiubo63162167;     DTaHzdiubo63162167 = DTaHzdiubo55243500;     DTaHzdiubo55243500 = DTaHzdiubo59756151;     DTaHzdiubo59756151 = DTaHzdiubo75524822;     DTaHzdiubo75524822 = DTaHzdiubo9514148;     DTaHzdiubo9514148 = DTaHzdiubo20997870;     DTaHzdiubo20997870 = DTaHzdiubo93413201;     DTaHzdiubo93413201 = DTaHzdiubo90180483;     DTaHzdiubo90180483 = DTaHzdiubo89607655;     DTaHzdiubo89607655 = DTaHzdiubo98752101;     DTaHzdiubo98752101 = DTaHzdiubo64188856;     DTaHzdiubo64188856 = DTaHzdiubo61572485;     DTaHzdiubo61572485 = DTaHzdiubo19129034;     DTaHzdiubo19129034 = DTaHzdiubo2542308;     DTaHzdiubo2542308 = DTaHzdiubo67723525;     DTaHzdiubo67723525 = DTaHzdiubo64146993;     DTaHzdiubo64146993 = DTaHzdiubo92845941;     DTaHzdiubo92845941 = DTaHzdiubo86636301;     DTaHzdiubo86636301 = DTaHzdiubo20171298;     DTaHzdiubo20171298 = DTaHzdiubo18836790;     DTaHzdiubo18836790 = DTaHzdiubo36894070;     DTaHzdiubo36894070 = DTaHzdiubo33931163;     DTaHzdiubo33931163 = DTaHzdiubo61119888;     DTaHzdiubo61119888 = DTaHzdiubo29431917;     DTaHzdiubo29431917 = DTaHzdiubo61016223;     DTaHzdiubo61016223 = DTaHzdiubo72490821;     DTaHzdiubo72490821 = DTaHzdiubo87383326;     DTaHzdiubo87383326 = DTaHzdiubo67663033;     DTaHzdiubo67663033 = DTaHzdiubo12577456;     DTaHzdiubo12577456 = DTaHzdiubo62261958;     DTaHzdiubo62261958 = DTaHzdiubo16970314;     DTaHzdiubo16970314 = DTaHzdiubo12268132;     DTaHzdiubo12268132 = DTaHzdiubo57872532;     DTaHzdiubo57872532 = DTaHzdiubo13059742;     DTaHzdiubo13059742 = DTaHzdiubo21195484;     DTaHzdiubo21195484 = DTaHzdiubo20572111;     DTaHzdiubo20572111 = DTaHzdiubo55528757;     DTaHzdiubo55528757 = DTaHzdiubo9540599;     DTaHzdiubo9540599 = DTaHzdiubo38131421;     DTaHzdiubo38131421 = DTaHzdiubo89358170;     DTaHzdiubo89358170 = DTaHzdiubo80738418;     DTaHzdiubo80738418 = DTaHzdiubo58718547;     DTaHzdiubo58718547 = DTaHzdiubo30825747;     DTaHzdiubo30825747 = DTaHzdiubo73230848;     DTaHzdiubo73230848 = DTaHzdiubo48786568;     DTaHzdiubo48786568 = DTaHzdiubo30647306;     DTaHzdiubo30647306 = DTaHzdiubo42556468;     DTaHzdiubo42556468 = DTaHzdiubo24265634;     DTaHzdiubo24265634 = DTaHzdiubo97332124;     DTaHzdiubo97332124 = DTaHzdiubo86294545;     DTaHzdiubo86294545 = DTaHzdiubo18981585;     DTaHzdiubo18981585 = DTaHzdiubo31896141;     DTaHzdiubo31896141 = DTaHzdiubo66281564;     DTaHzdiubo66281564 = DTaHzdiubo19355703;     DTaHzdiubo19355703 = DTaHzdiubo96953901;     DTaHzdiubo96953901 = DTaHzdiubo29231005;     DTaHzdiubo29231005 = DTaHzdiubo94123611;     DTaHzdiubo94123611 = DTaHzdiubo30324234;     DTaHzdiubo30324234 = DTaHzdiubo14508599;     DTaHzdiubo14508599 = DTaHzdiubo37023326;     DTaHzdiubo37023326 = DTaHzdiubo33614543;     DTaHzdiubo33614543 = DTaHzdiubo25750168;     DTaHzdiubo25750168 = DTaHzdiubo77603027;     DTaHzdiubo77603027 = DTaHzdiubo27345697;     DTaHzdiubo27345697 = DTaHzdiubo81781787;     DTaHzdiubo81781787 = DTaHzdiubo51920724;     DTaHzdiubo51920724 = DTaHzdiubo54364564;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void YGaEouVgzB22438075() {     int esYzgGtPRc76751681 = -593704482;    int esYzgGtPRc17450338 = -881780841;    int esYzgGtPRc71176619 = -344838778;    int esYzgGtPRc68242702 = -487741975;    int esYzgGtPRc32350577 = -396256407;    int esYzgGtPRc29074385 = 55750397;    int esYzgGtPRc24874421 = -288433622;    int esYzgGtPRc38652853 = -31652253;    int esYzgGtPRc71252616 = -465113954;    int esYzgGtPRc27266341 = -982810583;    int esYzgGtPRc36262853 = -962464824;    int esYzgGtPRc51967974 = -283965854;    int esYzgGtPRc13915894 = -731618486;    int esYzgGtPRc26972403 = -93650664;    int esYzgGtPRc12550888 = -298199817;    int esYzgGtPRc59540820 = -687372930;    int esYzgGtPRc34888772 = -503639830;    int esYzgGtPRc53621985 = -62630674;    int esYzgGtPRc51637671 = -659018018;    int esYzgGtPRc52667902 = -316818554;    int esYzgGtPRc49782138 = -485635069;    int esYzgGtPRc84248620 = 18918808;    int esYzgGtPRc75276272 = -998916352;    int esYzgGtPRc39475211 = -552383594;    int esYzgGtPRc27760038 = -330625633;    int esYzgGtPRc49988496 = -66644469;    int esYzgGtPRc73897362 = -812234497;    int esYzgGtPRc70967099 = -776265410;    int esYzgGtPRc57334099 = -731077215;    int esYzgGtPRc20984706 = -315425118;    int esYzgGtPRc62823248 = -189045445;    int esYzgGtPRc43994053 = -513790808;    int esYzgGtPRc19231163 = -523403824;    int esYzgGtPRc93236658 = -258602567;    int esYzgGtPRc39391532 = -767335543;    int esYzgGtPRc96936368 = -914036450;    int esYzgGtPRc53195863 = -260841043;    int esYzgGtPRc29150923 = -315637660;    int esYzgGtPRc57822279 = -546606629;    int esYzgGtPRc41183584 = -743785369;    int esYzgGtPRc22170572 = -585873798;    int esYzgGtPRc78688875 = -367755874;    int esYzgGtPRc95157847 = 6300063;    int esYzgGtPRc98196220 = -790801482;    int esYzgGtPRc32172123 = -521762002;    int esYzgGtPRc69672767 = -13648811;    int esYzgGtPRc31426432 = 68057486;    int esYzgGtPRc47140982 = -930998499;    int esYzgGtPRc62183430 = -871738009;    int esYzgGtPRc61230739 = -366554897;    int esYzgGtPRc1715159 = -745464417;    int esYzgGtPRc60487559 = -866297995;    int esYzgGtPRc98485700 = -847192674;    int esYzgGtPRc79123335 = -211897051;    int esYzgGtPRc72655937 = 24571609;    int esYzgGtPRc92503061 = -512623291;    int esYzgGtPRc42174066 = -882864489;    int esYzgGtPRc31701408 = -792455185;    int esYzgGtPRc40482665 = -57116342;    int esYzgGtPRc82362081 = -229611938;    int esYzgGtPRc55177023 = -132015107;    int esYzgGtPRc53907321 = -512168213;    int esYzgGtPRc81318754 = -300575038;    int esYzgGtPRc50267910 = -49688837;    int esYzgGtPRc64443093 = -693765138;    int esYzgGtPRc92268800 = -348674017;    int esYzgGtPRc32736811 = -760562031;    int esYzgGtPRc20679235 = -373015919;    int esYzgGtPRc87580871 = -326315121;    int esYzgGtPRc15614519 = -384163367;    int esYzgGtPRc6344958 = -326531888;    int esYzgGtPRc5737850 = -88002170;    int esYzgGtPRc95799706 = -516024045;    int esYzgGtPRc10454087 = -915232650;    int esYzgGtPRc30497331 = -730944756;    int esYzgGtPRc71093262 = -17879195;    int esYzgGtPRc89090772 = -987381256;    int esYzgGtPRc77080052 = -108114871;    int esYzgGtPRc7303089 = 69378408;    int esYzgGtPRc58087270 = -216976823;    int esYzgGtPRc18562065 = -34701955;    int esYzgGtPRc26756380 = -881235998;    int esYzgGtPRc8783670 = -904527401;    int esYzgGtPRc96103360 = -264522319;    int esYzgGtPRc19269548 = -569960701;    int esYzgGtPRc2335690 = -322747451;    int esYzgGtPRc45508352 = -666598134;    int esYzgGtPRc40107828 = -211506773;    int esYzgGtPRc20580722 = -183174176;    int esYzgGtPRc46888470 = -154712253;    int esYzgGtPRc54762303 = 68828039;    int esYzgGtPRc21494455 = -468385858;    int esYzgGtPRc88668258 = -158521319;    int esYzgGtPRc75460198 = -216994691;    int esYzgGtPRc86006561 = -511770263;    int esYzgGtPRc68263250 = 26294414;    int esYzgGtPRc97370121 = 32819163;    int esYzgGtPRc44889937 = -944011101;    int esYzgGtPRc33753127 = 2963656;    int esYzgGtPRc39903322 = -593704482;     esYzgGtPRc76751681 = esYzgGtPRc17450338;     esYzgGtPRc17450338 = esYzgGtPRc71176619;     esYzgGtPRc71176619 = esYzgGtPRc68242702;     esYzgGtPRc68242702 = esYzgGtPRc32350577;     esYzgGtPRc32350577 = esYzgGtPRc29074385;     esYzgGtPRc29074385 = esYzgGtPRc24874421;     esYzgGtPRc24874421 = esYzgGtPRc38652853;     esYzgGtPRc38652853 = esYzgGtPRc71252616;     esYzgGtPRc71252616 = esYzgGtPRc27266341;     esYzgGtPRc27266341 = esYzgGtPRc36262853;     esYzgGtPRc36262853 = esYzgGtPRc51967974;     esYzgGtPRc51967974 = esYzgGtPRc13915894;     esYzgGtPRc13915894 = esYzgGtPRc26972403;     esYzgGtPRc26972403 = esYzgGtPRc12550888;     esYzgGtPRc12550888 = esYzgGtPRc59540820;     esYzgGtPRc59540820 = esYzgGtPRc34888772;     esYzgGtPRc34888772 = esYzgGtPRc53621985;     esYzgGtPRc53621985 = esYzgGtPRc51637671;     esYzgGtPRc51637671 = esYzgGtPRc52667902;     esYzgGtPRc52667902 = esYzgGtPRc49782138;     esYzgGtPRc49782138 = esYzgGtPRc84248620;     esYzgGtPRc84248620 = esYzgGtPRc75276272;     esYzgGtPRc75276272 = esYzgGtPRc39475211;     esYzgGtPRc39475211 = esYzgGtPRc27760038;     esYzgGtPRc27760038 = esYzgGtPRc49988496;     esYzgGtPRc49988496 = esYzgGtPRc73897362;     esYzgGtPRc73897362 = esYzgGtPRc70967099;     esYzgGtPRc70967099 = esYzgGtPRc57334099;     esYzgGtPRc57334099 = esYzgGtPRc20984706;     esYzgGtPRc20984706 = esYzgGtPRc62823248;     esYzgGtPRc62823248 = esYzgGtPRc43994053;     esYzgGtPRc43994053 = esYzgGtPRc19231163;     esYzgGtPRc19231163 = esYzgGtPRc93236658;     esYzgGtPRc93236658 = esYzgGtPRc39391532;     esYzgGtPRc39391532 = esYzgGtPRc96936368;     esYzgGtPRc96936368 = esYzgGtPRc53195863;     esYzgGtPRc53195863 = esYzgGtPRc29150923;     esYzgGtPRc29150923 = esYzgGtPRc57822279;     esYzgGtPRc57822279 = esYzgGtPRc41183584;     esYzgGtPRc41183584 = esYzgGtPRc22170572;     esYzgGtPRc22170572 = esYzgGtPRc78688875;     esYzgGtPRc78688875 = esYzgGtPRc95157847;     esYzgGtPRc95157847 = esYzgGtPRc98196220;     esYzgGtPRc98196220 = esYzgGtPRc32172123;     esYzgGtPRc32172123 = esYzgGtPRc69672767;     esYzgGtPRc69672767 = esYzgGtPRc31426432;     esYzgGtPRc31426432 = esYzgGtPRc47140982;     esYzgGtPRc47140982 = esYzgGtPRc62183430;     esYzgGtPRc62183430 = esYzgGtPRc61230739;     esYzgGtPRc61230739 = esYzgGtPRc1715159;     esYzgGtPRc1715159 = esYzgGtPRc60487559;     esYzgGtPRc60487559 = esYzgGtPRc98485700;     esYzgGtPRc98485700 = esYzgGtPRc79123335;     esYzgGtPRc79123335 = esYzgGtPRc72655937;     esYzgGtPRc72655937 = esYzgGtPRc92503061;     esYzgGtPRc92503061 = esYzgGtPRc42174066;     esYzgGtPRc42174066 = esYzgGtPRc31701408;     esYzgGtPRc31701408 = esYzgGtPRc40482665;     esYzgGtPRc40482665 = esYzgGtPRc82362081;     esYzgGtPRc82362081 = esYzgGtPRc55177023;     esYzgGtPRc55177023 = esYzgGtPRc53907321;     esYzgGtPRc53907321 = esYzgGtPRc81318754;     esYzgGtPRc81318754 = esYzgGtPRc50267910;     esYzgGtPRc50267910 = esYzgGtPRc64443093;     esYzgGtPRc64443093 = esYzgGtPRc92268800;     esYzgGtPRc92268800 = esYzgGtPRc32736811;     esYzgGtPRc32736811 = esYzgGtPRc20679235;     esYzgGtPRc20679235 = esYzgGtPRc87580871;     esYzgGtPRc87580871 = esYzgGtPRc15614519;     esYzgGtPRc15614519 = esYzgGtPRc6344958;     esYzgGtPRc6344958 = esYzgGtPRc5737850;     esYzgGtPRc5737850 = esYzgGtPRc95799706;     esYzgGtPRc95799706 = esYzgGtPRc10454087;     esYzgGtPRc10454087 = esYzgGtPRc30497331;     esYzgGtPRc30497331 = esYzgGtPRc71093262;     esYzgGtPRc71093262 = esYzgGtPRc89090772;     esYzgGtPRc89090772 = esYzgGtPRc77080052;     esYzgGtPRc77080052 = esYzgGtPRc7303089;     esYzgGtPRc7303089 = esYzgGtPRc58087270;     esYzgGtPRc58087270 = esYzgGtPRc18562065;     esYzgGtPRc18562065 = esYzgGtPRc26756380;     esYzgGtPRc26756380 = esYzgGtPRc8783670;     esYzgGtPRc8783670 = esYzgGtPRc96103360;     esYzgGtPRc96103360 = esYzgGtPRc19269548;     esYzgGtPRc19269548 = esYzgGtPRc2335690;     esYzgGtPRc2335690 = esYzgGtPRc45508352;     esYzgGtPRc45508352 = esYzgGtPRc40107828;     esYzgGtPRc40107828 = esYzgGtPRc20580722;     esYzgGtPRc20580722 = esYzgGtPRc46888470;     esYzgGtPRc46888470 = esYzgGtPRc54762303;     esYzgGtPRc54762303 = esYzgGtPRc21494455;     esYzgGtPRc21494455 = esYzgGtPRc88668258;     esYzgGtPRc88668258 = esYzgGtPRc75460198;     esYzgGtPRc75460198 = esYzgGtPRc86006561;     esYzgGtPRc86006561 = esYzgGtPRc68263250;     esYzgGtPRc68263250 = esYzgGtPRc97370121;     esYzgGtPRc97370121 = esYzgGtPRc44889937;     esYzgGtPRc44889937 = esYzgGtPRc33753127;     esYzgGtPRc33753127 = esYzgGtPRc39903322;     esYzgGtPRc39903322 = esYzgGtPRc76751681;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void cFZPgMVGaO59083049() {     int UUULfmfwsa99138798 = -228108446;    int UUULfmfwsa1797839 = -830323895;    int UUULfmfwsa99945897 = -584492622;    int UUULfmfwsa83249390 = -494149506;    int UUULfmfwsa30524832 = -987313408;    int UUULfmfwsa43957502 = -904857981;    int UUULfmfwsa17030160 = -110420928;    int UUULfmfwsa14286712 = -502248897;    int UUULfmfwsa68415747 = -349309993;    int UUULfmfwsa19029926 = -725551916;    int UUULfmfwsa73804713 = -959089225;    int UUULfmfwsa7870923 = -534711147;    int UUULfmfwsa80924074 = -250335805;    int UUULfmfwsa69587155 = -621359421;    int UUULfmfwsa49286164 = 74322470;    int UUULfmfwsa3796733 = 30959527;    int UUULfmfwsa84712123 = -780919650;    int UUULfmfwsa59598401 = -514373364;    int UUULfmfwsa92919301 = -794120144;    int UUULfmfwsa31184186 = -569453007;    int UUULfmfwsa50665245 = -477122377;    int UUULfmfwsa48063838 = -485847506;    int UUULfmfwsa78569595 = -74996944;    int UUULfmfwsa65974998 = -16376451;    int UUULfmfwsa63300284 = 47815242;    int UUULfmfwsa38291491 = -510538693;    int UUULfmfwsa20986783 = -569820989;    int UUULfmfwsa76878549 = -564719863;    int UUULfmfwsa64226660 = 24665665;    int UUULfmfwsa30141886 = -235720984;    int UUULfmfwsa7114053 = -270361597;    int UUULfmfwsa1535244 = -65862163;    int UUULfmfwsa269834 = 47252682;    int UUULfmfwsa52625346 = -582729417;    int UUULfmfwsa15620897 = -831091966;    int UUULfmfwsa38629238 = -129431826;    int UUULfmfwsa46635575 = 6211005;    int UUULfmfwsa82777023 = -183263132;    int UUULfmfwsa6130411 = -947652661;    int UUULfmfwsa61369298 = -880009398;    int UUULfmfwsa50927943 = 10948536;    int UUULfmfwsa67197266 = -125664565;    int UUULfmfwsa708040 = -981230520;    int UUULfmfwsa97640339 = -788165131;    int UUULfmfwsa155390 = -330905480;    int UUULfmfwsa77773049 = -195877906;    int UUULfmfwsa43723829 = -510352767;    int UUULfmfwsa91739656 = -687533490;    int UUULfmfwsa56643335 = -821454707;    int UUULfmfwsa58314484 = -508575156;    int UUULfmfwsa10584375 = -540694;    int UUULfmfwsa34338816 = -197117277;    int UUULfmfwsa76800103 = -519103904;    int UUULfmfwsa39409880 = -487577860;    int UUULfmfwsa8417805 = -116132506;    int UUULfmfwsa51074961 = -742260940;    int UUULfmfwsa23228243 = -655326952;    int UUULfmfwsa33970900 = -468116172;    int UUULfmfwsa19949107 = -441964749;    int UUULfmfwsa92233341 = -376774716;    int UUULfmfwsa22970719 = -235036992;    int UUULfmfwsa40151610 = -545701066;    int UUULfmfwsa50060052 = -426914563;    int UUULfmfwsa38273862 = -13589009;    int UUULfmfwsa11915873 = -355190319;    int UUULfmfwsa72269469 = -793227063;    int UUULfmfwsa7601090 = -481963829;    int UUULfmfwsa28298728 = -667606389;    int UUULfmfwsa53966259 = -790267456;    int UUULfmfwsa10656927 = -796245704;    int UUULfmfwsa57161158 = -975251479;    int UUULfmfwsa1935101 = -497656518;    int UUULfmfwsa53467991 = -566720704;    int UUULfmfwsa31550004 = -914110746;    int UUULfmfwsa80256243 = -480401544;    int UUULfmfwsa83467978 = -251457813;    int UUULfmfwsa47355799 = -504616986;    int UUULfmfwsa80929256 = -286831814;    int UUULfmfwsa65819609 = -685470971;    int UUULfmfwsa85527235 = -756306852;    int UUULfmfwsa94567661 = 99814074;    int UUULfmfwsa29247126 = -882287500;    int UUULfmfwsa20235215 = -743265156;    int UUULfmfwsa5912176 = -466759179;    int UUULfmfwsa19557511 = -135180291;    int UUULfmfwsa72775237 = 26755679;    int UUULfmfwsa24735140 = -546758259;    int UUULfmfwsa60859953 = -465169459;    int UUULfmfwsa44207541 = -366596911;    int UUULfmfwsa64545936 = 11168974;    int UUULfmfwsa15400996 = -474104875;    int UUULfmfwsa12664676 = -525672823;    int UUULfmfwsa62827917 = -741298384;    int UUULfmfwsa13897070 = -470877945;    int UUULfmfwsa38398579 = -544972407;    int UUULfmfwsa10776333 = -443350399;    int UUULfmfwsa17137215 = -698750003;    int UUULfmfwsa62434177 = -867641512;    int UUULfmfwsa85724466 = -332974812;    int UUULfmfwsa27885920 = -228108446;     UUULfmfwsa99138798 = UUULfmfwsa1797839;     UUULfmfwsa1797839 = UUULfmfwsa99945897;     UUULfmfwsa99945897 = UUULfmfwsa83249390;     UUULfmfwsa83249390 = UUULfmfwsa30524832;     UUULfmfwsa30524832 = UUULfmfwsa43957502;     UUULfmfwsa43957502 = UUULfmfwsa17030160;     UUULfmfwsa17030160 = UUULfmfwsa14286712;     UUULfmfwsa14286712 = UUULfmfwsa68415747;     UUULfmfwsa68415747 = UUULfmfwsa19029926;     UUULfmfwsa19029926 = UUULfmfwsa73804713;     UUULfmfwsa73804713 = UUULfmfwsa7870923;     UUULfmfwsa7870923 = UUULfmfwsa80924074;     UUULfmfwsa80924074 = UUULfmfwsa69587155;     UUULfmfwsa69587155 = UUULfmfwsa49286164;     UUULfmfwsa49286164 = UUULfmfwsa3796733;     UUULfmfwsa3796733 = UUULfmfwsa84712123;     UUULfmfwsa84712123 = UUULfmfwsa59598401;     UUULfmfwsa59598401 = UUULfmfwsa92919301;     UUULfmfwsa92919301 = UUULfmfwsa31184186;     UUULfmfwsa31184186 = UUULfmfwsa50665245;     UUULfmfwsa50665245 = UUULfmfwsa48063838;     UUULfmfwsa48063838 = UUULfmfwsa78569595;     UUULfmfwsa78569595 = UUULfmfwsa65974998;     UUULfmfwsa65974998 = UUULfmfwsa63300284;     UUULfmfwsa63300284 = UUULfmfwsa38291491;     UUULfmfwsa38291491 = UUULfmfwsa20986783;     UUULfmfwsa20986783 = UUULfmfwsa76878549;     UUULfmfwsa76878549 = UUULfmfwsa64226660;     UUULfmfwsa64226660 = UUULfmfwsa30141886;     UUULfmfwsa30141886 = UUULfmfwsa7114053;     UUULfmfwsa7114053 = UUULfmfwsa1535244;     UUULfmfwsa1535244 = UUULfmfwsa269834;     UUULfmfwsa269834 = UUULfmfwsa52625346;     UUULfmfwsa52625346 = UUULfmfwsa15620897;     UUULfmfwsa15620897 = UUULfmfwsa38629238;     UUULfmfwsa38629238 = UUULfmfwsa46635575;     UUULfmfwsa46635575 = UUULfmfwsa82777023;     UUULfmfwsa82777023 = UUULfmfwsa6130411;     UUULfmfwsa6130411 = UUULfmfwsa61369298;     UUULfmfwsa61369298 = UUULfmfwsa50927943;     UUULfmfwsa50927943 = UUULfmfwsa67197266;     UUULfmfwsa67197266 = UUULfmfwsa708040;     UUULfmfwsa708040 = UUULfmfwsa97640339;     UUULfmfwsa97640339 = UUULfmfwsa155390;     UUULfmfwsa155390 = UUULfmfwsa77773049;     UUULfmfwsa77773049 = UUULfmfwsa43723829;     UUULfmfwsa43723829 = UUULfmfwsa91739656;     UUULfmfwsa91739656 = UUULfmfwsa56643335;     UUULfmfwsa56643335 = UUULfmfwsa58314484;     UUULfmfwsa58314484 = UUULfmfwsa10584375;     UUULfmfwsa10584375 = UUULfmfwsa34338816;     UUULfmfwsa34338816 = UUULfmfwsa76800103;     UUULfmfwsa76800103 = UUULfmfwsa39409880;     UUULfmfwsa39409880 = UUULfmfwsa8417805;     UUULfmfwsa8417805 = UUULfmfwsa51074961;     UUULfmfwsa51074961 = UUULfmfwsa23228243;     UUULfmfwsa23228243 = UUULfmfwsa33970900;     UUULfmfwsa33970900 = UUULfmfwsa19949107;     UUULfmfwsa19949107 = UUULfmfwsa92233341;     UUULfmfwsa92233341 = UUULfmfwsa22970719;     UUULfmfwsa22970719 = UUULfmfwsa40151610;     UUULfmfwsa40151610 = UUULfmfwsa50060052;     UUULfmfwsa50060052 = UUULfmfwsa38273862;     UUULfmfwsa38273862 = UUULfmfwsa11915873;     UUULfmfwsa11915873 = UUULfmfwsa72269469;     UUULfmfwsa72269469 = UUULfmfwsa7601090;     UUULfmfwsa7601090 = UUULfmfwsa28298728;     UUULfmfwsa28298728 = UUULfmfwsa53966259;     UUULfmfwsa53966259 = UUULfmfwsa10656927;     UUULfmfwsa10656927 = UUULfmfwsa57161158;     UUULfmfwsa57161158 = UUULfmfwsa1935101;     UUULfmfwsa1935101 = UUULfmfwsa53467991;     UUULfmfwsa53467991 = UUULfmfwsa31550004;     UUULfmfwsa31550004 = UUULfmfwsa80256243;     UUULfmfwsa80256243 = UUULfmfwsa83467978;     UUULfmfwsa83467978 = UUULfmfwsa47355799;     UUULfmfwsa47355799 = UUULfmfwsa80929256;     UUULfmfwsa80929256 = UUULfmfwsa65819609;     UUULfmfwsa65819609 = UUULfmfwsa85527235;     UUULfmfwsa85527235 = UUULfmfwsa94567661;     UUULfmfwsa94567661 = UUULfmfwsa29247126;     UUULfmfwsa29247126 = UUULfmfwsa20235215;     UUULfmfwsa20235215 = UUULfmfwsa5912176;     UUULfmfwsa5912176 = UUULfmfwsa19557511;     UUULfmfwsa19557511 = UUULfmfwsa72775237;     UUULfmfwsa72775237 = UUULfmfwsa24735140;     UUULfmfwsa24735140 = UUULfmfwsa60859953;     UUULfmfwsa60859953 = UUULfmfwsa44207541;     UUULfmfwsa44207541 = UUULfmfwsa64545936;     UUULfmfwsa64545936 = UUULfmfwsa15400996;     UUULfmfwsa15400996 = UUULfmfwsa12664676;     UUULfmfwsa12664676 = UUULfmfwsa62827917;     UUULfmfwsa62827917 = UUULfmfwsa13897070;     UUULfmfwsa13897070 = UUULfmfwsa38398579;     UUULfmfwsa38398579 = UUULfmfwsa10776333;     UUULfmfwsa10776333 = UUULfmfwsa17137215;     UUULfmfwsa17137215 = UUULfmfwsa62434177;     UUULfmfwsa62434177 = UUULfmfwsa85724466;     UUULfmfwsa85724466 = UUULfmfwsa27885920;     UUULfmfwsa27885920 = UUULfmfwsa99138798;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void souDALhrVo38589640() {     int zlCIwvJPUI8726807 = -572520000;    int zlCIwvJPUI76519083 = -693723187;    int zlCIwvJPUI13588801 = -330044312;    int zlCIwvJPUI60474975 = -317436421;    int zlCIwvJPUI52868185 = -923264587;    int zlCIwvJPUI85141412 = -387408669;    int zlCIwvJPUI25072813 = -995809098;    int zlCIwvJPUI1629560 = 63318081;    int zlCIwvJPUI35654396 = -799040080;    int zlCIwvJPUI97853832 = 62180642;    int zlCIwvJPUI24500401 = -759971755;    int zlCIwvJPUI60406721 = -849080350;    int zlCIwvJPUI41494780 = 91412583;    int zlCIwvJPUI70266269 = -406959083;    int zlCIwvJPUI53059106 = -671595116;    int zlCIwvJPUI68756671 = -465407927;    int zlCIwvJPUI90489176 = 66345858;    int zlCIwvJPUI28017399 = -5048996;    int zlCIwvJPUI73896842 = -55717607;    int zlCIwvJPUI51193499 = -346892224;    int zlCIwvJPUI30713144 = -498226169;    int zlCIwvJPUI39423769 = -740739237;    int zlCIwvJPUI16859417 = -137989326;    int zlCIwvJPUI35267873 = 18134659;    int zlCIwvJPUI49831651 = -216689664;    int zlCIwvJPUI78423993 = 19586426;    int zlCIwvJPUI97103830 = -295300628;    int zlCIwvJPUI86244124 = -297310189;    int zlCIwvJPUI79859061 = -297514843;    int zlCIwvJPUI38727690 = -976993444;    int zlCIwvJPUI73794219 = -849447468;    int zlCIwvJPUI25205326 = -422857362;    int zlCIwvJPUI6167402 = 89875539;    int zlCIwvJPUI35503335 = -755465514;    int zlCIwvJPUI73505805 = -803444357;    int zlCIwvJPUI70754371 = -815795591;    int zlCIwvJPUI27193962 = -136134352;    int zlCIwvJPUI18534461 = -121793871;    int zlCIwvJPUI19290349 = -732210947;    int zlCIwvJPUI99036068 = -390679954;    int zlCIwvJPUI93965340 = -292642934;    int zlCIwvJPUI19398524 = -372846086;    int zlCIwvJPUI91941481 = -863588168;    int zlCIwvJPUI70919717 = -658285424;    int zlCIwvJPUI56166270 = -93797801;    int zlCIwvJPUI93101138 = 71122097;    int zlCIwvJPUI33834792 = -453809212;    int zlCIwvJPUI57665527 = -406238414;    int zlCIwvJPUI44355969 = -835097154;    int zlCIwvJPUI48538656 = -313608725;    int zlCIwvJPUI27644837 = -359842678;    int zlCIwvJPUI9108041 = -921042717;    int zlCIwvJPUI71526053 = -899119868;    int zlCIwvJPUI75497241 = -37646629;    int zlCIwvJPUI7876497 = -876190690;    int zlCIwvJPUI69303037 = -831780763;    int zlCIwvJPUI59659667 = -455733862;    int zlCIwvJPUI78320928 = -248178972;    int zlCIwvJPUI10643324 = -746757;    int zlCIwvJPUI74444191 = -842851014;    int zlCIwvJPUI88037582 = 7891959;    int zlCIwvJPUI38828688 = -598498910;    int zlCIwvJPUI21770498 = -639167076;    int zlCIwvJPUI96926706 = -822046636;    int zlCIwvJPUI24059614 = -88371890;    int zlCIwvJPUI99295075 = -237114393;    int zlCIwvJPUI54239320 = -838955889;    int zlCIwvJPUI5991445 = -153121904;    int zlCIwvJPUI96760464 = -603514726;    int zlCIwvJPUI82304735 = -855799525;    int zlCIwvJPUI41562709 = -229273576;    int zlCIwvJPUI71954716 = -811860272;    int zlCIwvJPUI8727050 = -272838049;    int zlCIwvJPUI74860773 = -665037653;    int zlCIwvJPUI57228158 = 45750709;    int zlCIwvJPUI11314620 = -25380083;    int zlCIwvJPUI47482288 = -877151070;    int zlCIwvJPUI45939699 = -479703903;    int zlCIwvJPUI79101602 = -888067540;    int zlCIwvJPUI56730513 = -187811762;    int zlCIwvJPUI44589202 = -526604363;    int zlCIwvJPUI39438303 = -889062214;    int zlCIwvJPUI41888155 = -462213035;    int zlCIwvJPUI31320406 = -983906119;    int zlCIwvJPUI11082853 = -517150767;    int zlCIwvJPUI64686179 = -928404751;    int zlCIwvJPUI53679272 = -523737494;    int zlCIwvJPUI30670160 = -872477832;    int zlCIwvJPUI27626838 = -879274824;    int zlCIwvJPUI4202768 = -971663595;    int zlCIwvJPUI11094705 = -260061730;    int zlCIwvJPUI48873034 = -887955381;    int zlCIwvJPUI7891137 = -21047114;    int zlCIwvJPUI44846157 = -889359934;    int zlCIwvJPUI10998487 = -298571913;    int zlCIwvJPUI55136653 = -694144025;    int zlCIwvJPUI97628026 = -733679011;    int zlCIwvJPUI95014775 = 58458468;    int zlCIwvJPUI46860104 = -469913534;    int zlCIwvJPUI56871195 = -572520000;     zlCIwvJPUI8726807 = zlCIwvJPUI76519083;     zlCIwvJPUI76519083 = zlCIwvJPUI13588801;     zlCIwvJPUI13588801 = zlCIwvJPUI60474975;     zlCIwvJPUI60474975 = zlCIwvJPUI52868185;     zlCIwvJPUI52868185 = zlCIwvJPUI85141412;     zlCIwvJPUI85141412 = zlCIwvJPUI25072813;     zlCIwvJPUI25072813 = zlCIwvJPUI1629560;     zlCIwvJPUI1629560 = zlCIwvJPUI35654396;     zlCIwvJPUI35654396 = zlCIwvJPUI97853832;     zlCIwvJPUI97853832 = zlCIwvJPUI24500401;     zlCIwvJPUI24500401 = zlCIwvJPUI60406721;     zlCIwvJPUI60406721 = zlCIwvJPUI41494780;     zlCIwvJPUI41494780 = zlCIwvJPUI70266269;     zlCIwvJPUI70266269 = zlCIwvJPUI53059106;     zlCIwvJPUI53059106 = zlCIwvJPUI68756671;     zlCIwvJPUI68756671 = zlCIwvJPUI90489176;     zlCIwvJPUI90489176 = zlCIwvJPUI28017399;     zlCIwvJPUI28017399 = zlCIwvJPUI73896842;     zlCIwvJPUI73896842 = zlCIwvJPUI51193499;     zlCIwvJPUI51193499 = zlCIwvJPUI30713144;     zlCIwvJPUI30713144 = zlCIwvJPUI39423769;     zlCIwvJPUI39423769 = zlCIwvJPUI16859417;     zlCIwvJPUI16859417 = zlCIwvJPUI35267873;     zlCIwvJPUI35267873 = zlCIwvJPUI49831651;     zlCIwvJPUI49831651 = zlCIwvJPUI78423993;     zlCIwvJPUI78423993 = zlCIwvJPUI97103830;     zlCIwvJPUI97103830 = zlCIwvJPUI86244124;     zlCIwvJPUI86244124 = zlCIwvJPUI79859061;     zlCIwvJPUI79859061 = zlCIwvJPUI38727690;     zlCIwvJPUI38727690 = zlCIwvJPUI73794219;     zlCIwvJPUI73794219 = zlCIwvJPUI25205326;     zlCIwvJPUI25205326 = zlCIwvJPUI6167402;     zlCIwvJPUI6167402 = zlCIwvJPUI35503335;     zlCIwvJPUI35503335 = zlCIwvJPUI73505805;     zlCIwvJPUI73505805 = zlCIwvJPUI70754371;     zlCIwvJPUI70754371 = zlCIwvJPUI27193962;     zlCIwvJPUI27193962 = zlCIwvJPUI18534461;     zlCIwvJPUI18534461 = zlCIwvJPUI19290349;     zlCIwvJPUI19290349 = zlCIwvJPUI99036068;     zlCIwvJPUI99036068 = zlCIwvJPUI93965340;     zlCIwvJPUI93965340 = zlCIwvJPUI19398524;     zlCIwvJPUI19398524 = zlCIwvJPUI91941481;     zlCIwvJPUI91941481 = zlCIwvJPUI70919717;     zlCIwvJPUI70919717 = zlCIwvJPUI56166270;     zlCIwvJPUI56166270 = zlCIwvJPUI93101138;     zlCIwvJPUI93101138 = zlCIwvJPUI33834792;     zlCIwvJPUI33834792 = zlCIwvJPUI57665527;     zlCIwvJPUI57665527 = zlCIwvJPUI44355969;     zlCIwvJPUI44355969 = zlCIwvJPUI48538656;     zlCIwvJPUI48538656 = zlCIwvJPUI27644837;     zlCIwvJPUI27644837 = zlCIwvJPUI9108041;     zlCIwvJPUI9108041 = zlCIwvJPUI71526053;     zlCIwvJPUI71526053 = zlCIwvJPUI75497241;     zlCIwvJPUI75497241 = zlCIwvJPUI7876497;     zlCIwvJPUI7876497 = zlCIwvJPUI69303037;     zlCIwvJPUI69303037 = zlCIwvJPUI59659667;     zlCIwvJPUI59659667 = zlCIwvJPUI78320928;     zlCIwvJPUI78320928 = zlCIwvJPUI10643324;     zlCIwvJPUI10643324 = zlCIwvJPUI74444191;     zlCIwvJPUI74444191 = zlCIwvJPUI88037582;     zlCIwvJPUI88037582 = zlCIwvJPUI38828688;     zlCIwvJPUI38828688 = zlCIwvJPUI21770498;     zlCIwvJPUI21770498 = zlCIwvJPUI96926706;     zlCIwvJPUI96926706 = zlCIwvJPUI24059614;     zlCIwvJPUI24059614 = zlCIwvJPUI99295075;     zlCIwvJPUI99295075 = zlCIwvJPUI54239320;     zlCIwvJPUI54239320 = zlCIwvJPUI5991445;     zlCIwvJPUI5991445 = zlCIwvJPUI96760464;     zlCIwvJPUI96760464 = zlCIwvJPUI82304735;     zlCIwvJPUI82304735 = zlCIwvJPUI41562709;     zlCIwvJPUI41562709 = zlCIwvJPUI71954716;     zlCIwvJPUI71954716 = zlCIwvJPUI8727050;     zlCIwvJPUI8727050 = zlCIwvJPUI74860773;     zlCIwvJPUI74860773 = zlCIwvJPUI57228158;     zlCIwvJPUI57228158 = zlCIwvJPUI11314620;     zlCIwvJPUI11314620 = zlCIwvJPUI47482288;     zlCIwvJPUI47482288 = zlCIwvJPUI45939699;     zlCIwvJPUI45939699 = zlCIwvJPUI79101602;     zlCIwvJPUI79101602 = zlCIwvJPUI56730513;     zlCIwvJPUI56730513 = zlCIwvJPUI44589202;     zlCIwvJPUI44589202 = zlCIwvJPUI39438303;     zlCIwvJPUI39438303 = zlCIwvJPUI41888155;     zlCIwvJPUI41888155 = zlCIwvJPUI31320406;     zlCIwvJPUI31320406 = zlCIwvJPUI11082853;     zlCIwvJPUI11082853 = zlCIwvJPUI64686179;     zlCIwvJPUI64686179 = zlCIwvJPUI53679272;     zlCIwvJPUI53679272 = zlCIwvJPUI30670160;     zlCIwvJPUI30670160 = zlCIwvJPUI27626838;     zlCIwvJPUI27626838 = zlCIwvJPUI4202768;     zlCIwvJPUI4202768 = zlCIwvJPUI11094705;     zlCIwvJPUI11094705 = zlCIwvJPUI48873034;     zlCIwvJPUI48873034 = zlCIwvJPUI7891137;     zlCIwvJPUI7891137 = zlCIwvJPUI44846157;     zlCIwvJPUI44846157 = zlCIwvJPUI10998487;     zlCIwvJPUI10998487 = zlCIwvJPUI55136653;     zlCIwvJPUI55136653 = zlCIwvJPUI97628026;     zlCIwvJPUI97628026 = zlCIwvJPUI95014775;     zlCIwvJPUI95014775 = zlCIwvJPUI46860104;     zlCIwvJPUI46860104 = zlCIwvJPUI56871195;     zlCIwvJPUI56871195 = zlCIwvJPUI8726807;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void UrbfoPGcFP75234614() {     int mFvAtTWiMP31113924 = -206923963;    int mFvAtTWiMP60866584 = -642266242;    int mFvAtTWiMP42358080 = -569698156;    int mFvAtTWiMP75481663 = -323843953;    int mFvAtTWiMP51042440 = -414321588;    int mFvAtTWiMP24530 = -248017047;    int mFvAtTWiMP17228552 = -817796404;    int mFvAtTWiMP77263417 = -407278563;    int mFvAtTWiMP32817528 = -683236119;    int mFvAtTWiMP89617417 = -780560691;    int mFvAtTWiMP62042261 = -756596155;    int mFvAtTWiMP16309670 = 174357;    int mFvAtTWiMP8502961 = -527304736;    int mFvAtTWiMP12881022 = -934667839;    int mFvAtTWiMP89794383 = -299072829;    int mFvAtTWiMP13012584 = -847075470;    int mFvAtTWiMP40312529 = -210933962;    int mFvAtTWiMP33993815 = -456791687;    int mFvAtTWiMP15178473 = -190819732;    int mFvAtTWiMP29709782 = -599526678;    int mFvAtTWiMP31596251 = -489713477;    int mFvAtTWiMP3238988 = -145505551;    int mFvAtTWiMP20152740 = -314069918;    int mFvAtTWiMP61767660 = -545858198;    int mFvAtTWiMP85371898 = -938248789;    int mFvAtTWiMP66726988 = -424307798;    int mFvAtTWiMP44193251 = -52887121;    int mFvAtTWiMP92155574 = -85764642;    int mFvAtTWiMP86751622 = -641771963;    int mFvAtTWiMP47884869 = -897289310;    int mFvAtTWiMP18085025 = -930763620;    int mFvAtTWiMP82746516 = 25071283;    int mFvAtTWiMP87206071 = -439467956;    int mFvAtTWiMP94892022 = 20407636;    int mFvAtTWiMP49735170 = -867200780;    int mFvAtTWiMP12447241 = -31190967;    int mFvAtTWiMP20633675 = -969082304;    int mFvAtTWiMP72160561 = 10580657;    int mFvAtTWiMP67598480 = -33256979;    int mFvAtTWiMP19221784 = -526903984;    int mFvAtTWiMP22722712 = -795820600;    int mFvAtTWiMP7906915 = -130754777;    int mFvAtTWiMP97491673 = -751118751;    int mFvAtTWiMP70363837 = -655649073;    int mFvAtTWiMP24149537 = 97058722;    int mFvAtTWiMP1201420 = -111106999;    int mFvAtTWiMP46132189 = 67780535;    int mFvAtTWiMP2264202 = -162773405;    int mFvAtTWiMP38815875 = -784813852;    int mFvAtTWiMP45622401 = -455628984;    int mFvAtTWiMP36514054 = -714918955;    int mFvAtTWiMP82959297 = -251861999;    int mFvAtTWiMP49840457 = -571031099;    int mFvAtTWiMP35783787 = -313327438;    int mFvAtTWiMP43638364 = 83105194;    int mFvAtTWiMP27874937 = 38581587;    int mFvAtTWiMP40713844 = -228196324;    int mFvAtTWiMP80590420 = 76160041;    int mFvAtTWiMP90109765 = -385595164;    int mFvAtTWiMP84315451 = -990013791;    int mFvAtTWiMP55831279 = -95129927;    int mFvAtTWiMP25072977 = -632031762;    int mFvAtTWiMP90511795 = -765506601;    int mFvAtTWiMP84932658 = -785946809;    int mFvAtTWiMP71532393 = -849797071;    int mFvAtTWiMP79295744 = -681667439;    int mFvAtTWiMP29103598 = -560357688;    int mFvAtTWiMP13610939 = -447712373;    int mFvAtTWiMP63145852 = 32532940;    int mFvAtTWiMP77347143 = -167881862;    int mFvAtTWiMP92378909 = -877993167;    int mFvAtTWiMP68151967 = -121514620;    int mFvAtTWiMP66395334 = -323534708;    int mFvAtTWiMP95956689 = -663915749;    int mFvAtTWiMP6987071 = -803706079;    int mFvAtTWiMP23689336 = -258958701;    int mFvAtTWiMP5747314 = -394386800;    int mFvAtTWiMP49788903 = -658420846;    int mFvAtTWiMP37618123 = -542916920;    int mFvAtTWiMP84170478 = -727141791;    int mFvAtTWiMP20594800 = -392088334;    int mFvAtTWiMP41929049 = -890113716;    int mFvAtTWiMP53339700 = -300950791;    int mFvAtTWiMP41129221 = -86142979;    int mFvAtTWiMP11370816 = -82370356;    int mFvAtTWiMP35125727 = -578901621;    int mFvAtTWiMP32906060 = -403897619;    int mFvAtTWiMP51422285 = -26140518;    int mFvAtTWiMP51253658 = 37302441;    int mFvAtTWiMP21860234 = -805782367;    int mFvAtTWiMP71733396 = -802994643;    int mFvAtTWiMP40043255 = -945242345;    int mFvAtTWiMP82050796 = -603824179;    int mFvAtTWiMP83283029 = -43243189;    int mFvAtTWiMP63390504 = -331774058;    int mFvAtTWiMP97649734 = -63788838;    int mFvAtTWiMP17395120 = -365248177;    int mFvAtTWiMP12559016 = -965171943;    int mFvAtTWiMP98831444 = -805852002;    int mFvAtTWiMP44853793 = -206923963;     mFvAtTWiMP31113924 = mFvAtTWiMP60866584;     mFvAtTWiMP60866584 = mFvAtTWiMP42358080;     mFvAtTWiMP42358080 = mFvAtTWiMP75481663;     mFvAtTWiMP75481663 = mFvAtTWiMP51042440;     mFvAtTWiMP51042440 = mFvAtTWiMP24530;     mFvAtTWiMP24530 = mFvAtTWiMP17228552;     mFvAtTWiMP17228552 = mFvAtTWiMP77263417;     mFvAtTWiMP77263417 = mFvAtTWiMP32817528;     mFvAtTWiMP32817528 = mFvAtTWiMP89617417;     mFvAtTWiMP89617417 = mFvAtTWiMP62042261;     mFvAtTWiMP62042261 = mFvAtTWiMP16309670;     mFvAtTWiMP16309670 = mFvAtTWiMP8502961;     mFvAtTWiMP8502961 = mFvAtTWiMP12881022;     mFvAtTWiMP12881022 = mFvAtTWiMP89794383;     mFvAtTWiMP89794383 = mFvAtTWiMP13012584;     mFvAtTWiMP13012584 = mFvAtTWiMP40312529;     mFvAtTWiMP40312529 = mFvAtTWiMP33993815;     mFvAtTWiMP33993815 = mFvAtTWiMP15178473;     mFvAtTWiMP15178473 = mFvAtTWiMP29709782;     mFvAtTWiMP29709782 = mFvAtTWiMP31596251;     mFvAtTWiMP31596251 = mFvAtTWiMP3238988;     mFvAtTWiMP3238988 = mFvAtTWiMP20152740;     mFvAtTWiMP20152740 = mFvAtTWiMP61767660;     mFvAtTWiMP61767660 = mFvAtTWiMP85371898;     mFvAtTWiMP85371898 = mFvAtTWiMP66726988;     mFvAtTWiMP66726988 = mFvAtTWiMP44193251;     mFvAtTWiMP44193251 = mFvAtTWiMP92155574;     mFvAtTWiMP92155574 = mFvAtTWiMP86751622;     mFvAtTWiMP86751622 = mFvAtTWiMP47884869;     mFvAtTWiMP47884869 = mFvAtTWiMP18085025;     mFvAtTWiMP18085025 = mFvAtTWiMP82746516;     mFvAtTWiMP82746516 = mFvAtTWiMP87206071;     mFvAtTWiMP87206071 = mFvAtTWiMP94892022;     mFvAtTWiMP94892022 = mFvAtTWiMP49735170;     mFvAtTWiMP49735170 = mFvAtTWiMP12447241;     mFvAtTWiMP12447241 = mFvAtTWiMP20633675;     mFvAtTWiMP20633675 = mFvAtTWiMP72160561;     mFvAtTWiMP72160561 = mFvAtTWiMP67598480;     mFvAtTWiMP67598480 = mFvAtTWiMP19221784;     mFvAtTWiMP19221784 = mFvAtTWiMP22722712;     mFvAtTWiMP22722712 = mFvAtTWiMP7906915;     mFvAtTWiMP7906915 = mFvAtTWiMP97491673;     mFvAtTWiMP97491673 = mFvAtTWiMP70363837;     mFvAtTWiMP70363837 = mFvAtTWiMP24149537;     mFvAtTWiMP24149537 = mFvAtTWiMP1201420;     mFvAtTWiMP1201420 = mFvAtTWiMP46132189;     mFvAtTWiMP46132189 = mFvAtTWiMP2264202;     mFvAtTWiMP2264202 = mFvAtTWiMP38815875;     mFvAtTWiMP38815875 = mFvAtTWiMP45622401;     mFvAtTWiMP45622401 = mFvAtTWiMP36514054;     mFvAtTWiMP36514054 = mFvAtTWiMP82959297;     mFvAtTWiMP82959297 = mFvAtTWiMP49840457;     mFvAtTWiMP49840457 = mFvAtTWiMP35783787;     mFvAtTWiMP35783787 = mFvAtTWiMP43638364;     mFvAtTWiMP43638364 = mFvAtTWiMP27874937;     mFvAtTWiMP27874937 = mFvAtTWiMP40713844;     mFvAtTWiMP40713844 = mFvAtTWiMP80590420;     mFvAtTWiMP80590420 = mFvAtTWiMP90109765;     mFvAtTWiMP90109765 = mFvAtTWiMP84315451;     mFvAtTWiMP84315451 = mFvAtTWiMP55831279;     mFvAtTWiMP55831279 = mFvAtTWiMP25072977;     mFvAtTWiMP25072977 = mFvAtTWiMP90511795;     mFvAtTWiMP90511795 = mFvAtTWiMP84932658;     mFvAtTWiMP84932658 = mFvAtTWiMP71532393;     mFvAtTWiMP71532393 = mFvAtTWiMP79295744;     mFvAtTWiMP79295744 = mFvAtTWiMP29103598;     mFvAtTWiMP29103598 = mFvAtTWiMP13610939;     mFvAtTWiMP13610939 = mFvAtTWiMP63145852;     mFvAtTWiMP63145852 = mFvAtTWiMP77347143;     mFvAtTWiMP77347143 = mFvAtTWiMP92378909;     mFvAtTWiMP92378909 = mFvAtTWiMP68151967;     mFvAtTWiMP68151967 = mFvAtTWiMP66395334;     mFvAtTWiMP66395334 = mFvAtTWiMP95956689;     mFvAtTWiMP95956689 = mFvAtTWiMP6987071;     mFvAtTWiMP6987071 = mFvAtTWiMP23689336;     mFvAtTWiMP23689336 = mFvAtTWiMP5747314;     mFvAtTWiMP5747314 = mFvAtTWiMP49788903;     mFvAtTWiMP49788903 = mFvAtTWiMP37618123;     mFvAtTWiMP37618123 = mFvAtTWiMP84170478;     mFvAtTWiMP84170478 = mFvAtTWiMP20594800;     mFvAtTWiMP20594800 = mFvAtTWiMP41929049;     mFvAtTWiMP41929049 = mFvAtTWiMP53339700;     mFvAtTWiMP53339700 = mFvAtTWiMP41129221;     mFvAtTWiMP41129221 = mFvAtTWiMP11370816;     mFvAtTWiMP11370816 = mFvAtTWiMP35125727;     mFvAtTWiMP35125727 = mFvAtTWiMP32906060;     mFvAtTWiMP32906060 = mFvAtTWiMP51422285;     mFvAtTWiMP51422285 = mFvAtTWiMP51253658;     mFvAtTWiMP51253658 = mFvAtTWiMP21860234;     mFvAtTWiMP21860234 = mFvAtTWiMP71733396;     mFvAtTWiMP71733396 = mFvAtTWiMP40043255;     mFvAtTWiMP40043255 = mFvAtTWiMP82050796;     mFvAtTWiMP82050796 = mFvAtTWiMP83283029;     mFvAtTWiMP83283029 = mFvAtTWiMP63390504;     mFvAtTWiMP63390504 = mFvAtTWiMP97649734;     mFvAtTWiMP97649734 = mFvAtTWiMP17395120;     mFvAtTWiMP17395120 = mFvAtTWiMP12559016;     mFvAtTWiMP12559016 = mFvAtTWiMP98831444;     mFvAtTWiMP98831444 = mFvAtTWiMP44853793;     mFvAtTWiMP44853793 = mFvAtTWiMP31113924;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void KiAcqJPzPI11879590() {     int VnOiTIWeOB53501041 = -941327926;    int VnOiTIWeOB45214084 = -590809296;    int VnOiTIWeOB71127358 = -809352000;    int VnOiTIWeOB90488351 = -330251485;    int VnOiTIWeOB49216694 = 94621410;    int VnOiTIWeOB14907647 = -108625425;    int VnOiTIWeOB9384291 = -639783710;    int VnOiTIWeOB52897276 = -877875207;    int VnOiTIWeOB29980659 = -567432158;    int VnOiTIWeOB81381002 = -523302024;    int VnOiTIWeOB99584120 = -753220556;    int VnOiTIWeOB72212618 = -250570936;    int VnOiTIWeOB75511141 = -46022056;    int VnOiTIWeOB55495774 = -362376596;    int VnOiTIWeOB26529661 = 73449458;    int VnOiTIWeOB57268496 = -128743012;    int VnOiTIWeOB90135880 = -488213782;    int VnOiTIWeOB39970231 = -908534378;    int VnOiTIWeOB56460104 = -325921857;    int VnOiTIWeOB8226066 = -852161132;    int VnOiTIWeOB32479358 = -481200786;    int VnOiTIWeOB67054206 = -650271864;    int VnOiTIWeOB23446063 = -490150510;    int VnOiTIWeOB88267446 = -9851055;    int VnOiTIWeOB20912145 = -559807914;    int VnOiTIWeOB55029983 = -868202022;    int VnOiTIWeOB91282670 = -910473613;    int VnOiTIWeOB98067025 = -974219096;    int VnOiTIWeOB93644183 = -986029082;    int VnOiTIWeOB57042049 = -817585177;    int VnOiTIWeOB62375830 = 87920228;    int VnOiTIWeOB40287708 = -627000071;    int VnOiTIWeOB68244742 = -968811450;    int VnOiTIWeOB54280709 = -303719214;    int VnOiTIWeOB25964535 = -930957202;    int VnOiTIWeOB54140110 = -346586344;    int VnOiTIWeOB14073387 = -702030255;    int VnOiTIWeOB25786663 = -957044815;    int VnOiTIWeOB15906612 = -434303011;    int VnOiTIWeOB39407498 = -663128013;    int VnOiTIWeOB51480082 = -198998266;    int VnOiTIWeOB96415305 = -988663468;    int VnOiTIWeOB3041866 = -638649335;    int VnOiTIWeOB69807956 = -653012722;    int VnOiTIWeOB92132803 = -812084756;    int VnOiTIWeOB9301702 = -293336094;    int VnOiTIWeOB58429586 = -510629717;    int VnOiTIWeOB46862876 = 80691604;    int VnOiTIWeOB33275780 = -734530550;    int VnOiTIWeOB42706147 = -597649243;    int VnOiTIWeOB45383270 = 30004768;    int VnOiTIWeOB56810555 = -682681281;    int VnOiTIWeOB28154860 = -242942329;    int VnOiTIWeOB96070332 = -589008247;    int VnOiTIWeOB79400231 = -57598921;    int VnOiTIWeOB86446835 = -191056062;    int VnOiTIWeOB21768022 = -658787;    int VnOiTIWeOB82859911 = -699500946;    int VnOiTIWeOB69576207 = -770443572;    int VnOiTIWeOB94186710 = -37176568;    int VnOiTIWeOB23624976 = -198151812;    int VnOiTIWeOB11317266 = -665564615;    int VnOiTIWeOB59253093 = -891846126;    int VnOiTIWeOB72938609 = -749846981;    int VnOiTIWeOB19005173 = -511222252;    int VnOiTIWeOB59296413 = -26220485;    int VnOiTIWeOB3967877 = -281759486;    int VnOiTIWeOB21230432 = -742302843;    int VnOiTIWeOB29531240 = -431419395;    int VnOiTIWeOB72389550 = -579964199;    int VnOiTIWeOB43195110 = -426712758;    int VnOiTIWeOB64349218 = -531168968;    int VnOiTIWeOB24063620 = -374231367;    int VnOiTIWeOB17052606 = -662793845;    int VnOiTIWeOB56745983 = -553162866;    int VnOiTIWeOB36064052 = -492537318;    int VnOiTIWeOB64012340 = 88377471;    int VnOiTIWeOB53638107 = -837137789;    int VnOiTIWeOB96134643 = -197766300;    int VnOiTIWeOB11610443 = -166471820;    int VnOiTIWeOB96600396 = -257572305;    int VnOiTIWeOB44419795 = -891165218;    int VnOiTIWeOB64791245 = -139688546;    int VnOiTIWeOB50938036 = -288379839;    int VnOiTIWeOB11658779 = -747589946;    int VnOiTIWeOB5565275 = -229398491;    int VnOiTIWeOB12132848 = -284057743;    int VnOiTIWeOB72174410 = -279803204;    int VnOiTIWeOB74880478 = -146120294;    int VnOiTIWeOB39517699 = -639901140;    int VnOiTIWeOB32372088 = -245927557;    int VnOiTIWeOB31213475 = 97470690;    int VnOiTIWeOB56210455 = -86601244;    int VnOiTIWeOB21719901 = -297126443;    int VnOiTIWeOB15782523 = -364976202;    int VnOiTIWeOB40162817 = -533433652;    int VnOiTIWeOB37162213 = 3182657;    int VnOiTIWeOB30103256 = -888802354;    int VnOiTIWeOB50802784 = -41790470;    int VnOiTIWeOB32836391 = -941327926;     VnOiTIWeOB53501041 = VnOiTIWeOB45214084;     VnOiTIWeOB45214084 = VnOiTIWeOB71127358;     VnOiTIWeOB71127358 = VnOiTIWeOB90488351;     VnOiTIWeOB90488351 = VnOiTIWeOB49216694;     VnOiTIWeOB49216694 = VnOiTIWeOB14907647;     VnOiTIWeOB14907647 = VnOiTIWeOB9384291;     VnOiTIWeOB9384291 = VnOiTIWeOB52897276;     VnOiTIWeOB52897276 = VnOiTIWeOB29980659;     VnOiTIWeOB29980659 = VnOiTIWeOB81381002;     VnOiTIWeOB81381002 = VnOiTIWeOB99584120;     VnOiTIWeOB99584120 = VnOiTIWeOB72212618;     VnOiTIWeOB72212618 = VnOiTIWeOB75511141;     VnOiTIWeOB75511141 = VnOiTIWeOB55495774;     VnOiTIWeOB55495774 = VnOiTIWeOB26529661;     VnOiTIWeOB26529661 = VnOiTIWeOB57268496;     VnOiTIWeOB57268496 = VnOiTIWeOB90135880;     VnOiTIWeOB90135880 = VnOiTIWeOB39970231;     VnOiTIWeOB39970231 = VnOiTIWeOB56460104;     VnOiTIWeOB56460104 = VnOiTIWeOB8226066;     VnOiTIWeOB8226066 = VnOiTIWeOB32479358;     VnOiTIWeOB32479358 = VnOiTIWeOB67054206;     VnOiTIWeOB67054206 = VnOiTIWeOB23446063;     VnOiTIWeOB23446063 = VnOiTIWeOB88267446;     VnOiTIWeOB88267446 = VnOiTIWeOB20912145;     VnOiTIWeOB20912145 = VnOiTIWeOB55029983;     VnOiTIWeOB55029983 = VnOiTIWeOB91282670;     VnOiTIWeOB91282670 = VnOiTIWeOB98067025;     VnOiTIWeOB98067025 = VnOiTIWeOB93644183;     VnOiTIWeOB93644183 = VnOiTIWeOB57042049;     VnOiTIWeOB57042049 = VnOiTIWeOB62375830;     VnOiTIWeOB62375830 = VnOiTIWeOB40287708;     VnOiTIWeOB40287708 = VnOiTIWeOB68244742;     VnOiTIWeOB68244742 = VnOiTIWeOB54280709;     VnOiTIWeOB54280709 = VnOiTIWeOB25964535;     VnOiTIWeOB25964535 = VnOiTIWeOB54140110;     VnOiTIWeOB54140110 = VnOiTIWeOB14073387;     VnOiTIWeOB14073387 = VnOiTIWeOB25786663;     VnOiTIWeOB25786663 = VnOiTIWeOB15906612;     VnOiTIWeOB15906612 = VnOiTIWeOB39407498;     VnOiTIWeOB39407498 = VnOiTIWeOB51480082;     VnOiTIWeOB51480082 = VnOiTIWeOB96415305;     VnOiTIWeOB96415305 = VnOiTIWeOB3041866;     VnOiTIWeOB3041866 = VnOiTIWeOB69807956;     VnOiTIWeOB69807956 = VnOiTIWeOB92132803;     VnOiTIWeOB92132803 = VnOiTIWeOB9301702;     VnOiTIWeOB9301702 = VnOiTIWeOB58429586;     VnOiTIWeOB58429586 = VnOiTIWeOB46862876;     VnOiTIWeOB46862876 = VnOiTIWeOB33275780;     VnOiTIWeOB33275780 = VnOiTIWeOB42706147;     VnOiTIWeOB42706147 = VnOiTIWeOB45383270;     VnOiTIWeOB45383270 = VnOiTIWeOB56810555;     VnOiTIWeOB56810555 = VnOiTIWeOB28154860;     VnOiTIWeOB28154860 = VnOiTIWeOB96070332;     VnOiTIWeOB96070332 = VnOiTIWeOB79400231;     VnOiTIWeOB79400231 = VnOiTIWeOB86446835;     VnOiTIWeOB86446835 = VnOiTIWeOB21768022;     VnOiTIWeOB21768022 = VnOiTIWeOB82859911;     VnOiTIWeOB82859911 = VnOiTIWeOB69576207;     VnOiTIWeOB69576207 = VnOiTIWeOB94186710;     VnOiTIWeOB94186710 = VnOiTIWeOB23624976;     VnOiTIWeOB23624976 = VnOiTIWeOB11317266;     VnOiTIWeOB11317266 = VnOiTIWeOB59253093;     VnOiTIWeOB59253093 = VnOiTIWeOB72938609;     VnOiTIWeOB72938609 = VnOiTIWeOB19005173;     VnOiTIWeOB19005173 = VnOiTIWeOB59296413;     VnOiTIWeOB59296413 = VnOiTIWeOB3967877;     VnOiTIWeOB3967877 = VnOiTIWeOB21230432;     VnOiTIWeOB21230432 = VnOiTIWeOB29531240;     VnOiTIWeOB29531240 = VnOiTIWeOB72389550;     VnOiTIWeOB72389550 = VnOiTIWeOB43195110;     VnOiTIWeOB43195110 = VnOiTIWeOB64349218;     VnOiTIWeOB64349218 = VnOiTIWeOB24063620;     VnOiTIWeOB24063620 = VnOiTIWeOB17052606;     VnOiTIWeOB17052606 = VnOiTIWeOB56745983;     VnOiTIWeOB56745983 = VnOiTIWeOB36064052;     VnOiTIWeOB36064052 = VnOiTIWeOB64012340;     VnOiTIWeOB64012340 = VnOiTIWeOB53638107;     VnOiTIWeOB53638107 = VnOiTIWeOB96134643;     VnOiTIWeOB96134643 = VnOiTIWeOB11610443;     VnOiTIWeOB11610443 = VnOiTIWeOB96600396;     VnOiTIWeOB96600396 = VnOiTIWeOB44419795;     VnOiTIWeOB44419795 = VnOiTIWeOB64791245;     VnOiTIWeOB64791245 = VnOiTIWeOB50938036;     VnOiTIWeOB50938036 = VnOiTIWeOB11658779;     VnOiTIWeOB11658779 = VnOiTIWeOB5565275;     VnOiTIWeOB5565275 = VnOiTIWeOB12132848;     VnOiTIWeOB12132848 = VnOiTIWeOB72174410;     VnOiTIWeOB72174410 = VnOiTIWeOB74880478;     VnOiTIWeOB74880478 = VnOiTIWeOB39517699;     VnOiTIWeOB39517699 = VnOiTIWeOB32372088;     VnOiTIWeOB32372088 = VnOiTIWeOB31213475;     VnOiTIWeOB31213475 = VnOiTIWeOB56210455;     VnOiTIWeOB56210455 = VnOiTIWeOB21719901;     VnOiTIWeOB21719901 = VnOiTIWeOB15782523;     VnOiTIWeOB15782523 = VnOiTIWeOB40162817;     VnOiTIWeOB40162817 = VnOiTIWeOB37162213;     VnOiTIWeOB37162213 = VnOiTIWeOB30103256;     VnOiTIWeOB30103256 = VnOiTIWeOB50802784;     VnOiTIWeOB50802784 = VnOiTIWeOB32836391;     VnOiTIWeOB32836391 = VnOiTIWeOB53501041;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void AtpQbNdZDy48524564() {     int QfXtOMVRuu75888158 = -575731889;    int QfXtOMVRuu29561585 = -539352351;    int QfXtOMVRuu99896636 = 50994156;    int QfXtOMVRuu5495041 = -336659016;    int QfXtOMVRuu47390948 = -496435591;    int QfXtOMVRuu29790764 = 30766198;    int QfXtOMVRuu1540030 = -461771016;    int QfXtOMVRuu28531135 = -248471851;    int QfXtOMVRuu27143790 = -451628197;    int QfXtOMVRuu73144587 = -266043357;    int QfXtOMVRuu37125980 = -749844957;    int QfXtOMVRuu28115568 = -501316230;    int QfXtOMVRuu42519321 = -664739376;    int QfXtOMVRuu98110526 = -890085353;    int QfXtOMVRuu63264937 = -654028255;    int QfXtOMVRuu1524409 = -510410555;    int QfXtOMVRuu39959232 = -765493602;    int QfXtOMVRuu45946647 = -260277068;    int QfXtOMVRuu97741734 = -461023982;    int QfXtOMVRuu86742348 = -4795585;    int QfXtOMVRuu33362465 = -472688094;    int QfXtOMVRuu30869425 = -55038177;    int QfXtOMVRuu26739386 = -666231102;    int QfXtOMVRuu14767234 = -573843912;    int QfXtOMVRuu56452392 = -181367038;    int QfXtOMVRuu43332977 = -212096246;    int QfXtOMVRuu38372091 = -668060106;    int QfXtOMVRuu3978476 = -762673549;    int QfXtOMVRuu536745 = -230286201;    int QfXtOMVRuu66199228 = -737881044;    int QfXtOMVRuu6666636 = 6604076;    int QfXtOMVRuu97828898 = -179071426;    int QfXtOMVRuu49283412 = -398154945;    int QfXtOMVRuu13669397 = -627846064;    int QfXtOMVRuu2193899 = -994713625;    int QfXtOMVRuu95832978 = -661981720;    int QfXtOMVRuu7513099 = -434978206;    int QfXtOMVRuu79412763 = -824670287;    int QfXtOMVRuu64214743 = -835349043;    int QfXtOMVRuu59593213 = -799352042;    int QfXtOMVRuu80237453 = -702175932;    int QfXtOMVRuu84923697 = -746572159;    int QfXtOMVRuu8592058 = -526179919;    int QfXtOMVRuu69252075 = -650376371;    int QfXtOMVRuu60116070 = -621228234;    int QfXtOMVRuu17401984 = -475565190;    int QfXtOMVRuu70726984 = 10960030;    int QfXtOMVRuu91461550 = -775843386;    int QfXtOMVRuu27735685 = -684247249;    int QfXtOMVRuu39789893 = -739669502;    int QfXtOMVRuu54252487 = -325071509;    int QfXtOMVRuu30661812 = -13500563;    int QfXtOMVRuu6469263 = 85146441;    int QfXtOMVRuu56356877 = -864689056;    int QfXtOMVRuu15162099 = -198303036;    int QfXtOMVRuu45018734 = -420693712;    int QfXtOMVRuu2822199 = -873121250;    int QfXtOMVRuu85129403 = -375161933;    int QfXtOMVRuu49042649 = -55291979;    int QfXtOMVRuu4057971 = -184339346;    int QfXtOMVRuu91418672 = -301173697;    int QfXtOMVRuu97561554 = -699097467;    int QfXtOMVRuu27994391 = 81814349;    int QfXtOMVRuu60944561 = -713747154;    int QfXtOMVRuu66477952 = -172647433;    int QfXtOMVRuu39297082 = -470773532;    int QfXtOMVRuu78832155 = -3161285;    int QfXtOMVRuu28849925 = 63106688;    int QfXtOMVRuu95916627 = -895371729;    int QfXtOMVRuu67431958 = -992046535;    int QfXtOMVRuu94011310 = 24567651;    int QfXtOMVRuu60546469 = -940823316;    int QfXtOMVRuu81731904 = -424928026;    int QfXtOMVRuu38148522 = -661671941;    int QfXtOMVRuu6504896 = -302619654;    int QfXtOMVRuu48438768 = -726115935;    int QfXtOMVRuu22277367 = -528858259;    int QfXtOMVRuu57487310 = 84145268;    int QfXtOMVRuu54651164 = -952615679;    int QfXtOMVRuu39050408 = -705801849;    int QfXtOMVRuu72605993 = -123056276;    int QfXtOMVRuu46910541 = -892216720;    int QfXtOMVRuu76242790 = 21573699;    int QfXtOMVRuu60746851 = -490616699;    int QfXtOMVRuu11946742 = -312809536;    int QfXtOMVRuu76004823 = -979895361;    int QfXtOMVRuu91359635 = -164217868;    int QfXtOMVRuu92926535 = -533465890;    int QfXtOMVRuu98507297 = -329543028;    int QfXtOMVRuu57175164 = -474019913;    int QfXtOMVRuu93010780 = -788860471;    int QfXtOMVRuu22383696 = 40183726;    int QfXtOMVRuu30370115 = -669378309;    int QfXtOMVRuu60156772 = -551009697;    int QfXtOMVRuu68174540 = -398178346;    int QfXtOMVRuu82675899 = 96921535;    int QfXtOMVRuu56929306 = -728386509;    int QfXtOMVRuu47647496 = -812432765;    int QfXtOMVRuu2774124 = -377728938;    int QfXtOMVRuu20818989 = -575731889;     QfXtOMVRuu75888158 = QfXtOMVRuu29561585;     QfXtOMVRuu29561585 = QfXtOMVRuu99896636;     QfXtOMVRuu99896636 = QfXtOMVRuu5495041;     QfXtOMVRuu5495041 = QfXtOMVRuu47390948;     QfXtOMVRuu47390948 = QfXtOMVRuu29790764;     QfXtOMVRuu29790764 = QfXtOMVRuu1540030;     QfXtOMVRuu1540030 = QfXtOMVRuu28531135;     QfXtOMVRuu28531135 = QfXtOMVRuu27143790;     QfXtOMVRuu27143790 = QfXtOMVRuu73144587;     QfXtOMVRuu73144587 = QfXtOMVRuu37125980;     QfXtOMVRuu37125980 = QfXtOMVRuu28115568;     QfXtOMVRuu28115568 = QfXtOMVRuu42519321;     QfXtOMVRuu42519321 = QfXtOMVRuu98110526;     QfXtOMVRuu98110526 = QfXtOMVRuu63264937;     QfXtOMVRuu63264937 = QfXtOMVRuu1524409;     QfXtOMVRuu1524409 = QfXtOMVRuu39959232;     QfXtOMVRuu39959232 = QfXtOMVRuu45946647;     QfXtOMVRuu45946647 = QfXtOMVRuu97741734;     QfXtOMVRuu97741734 = QfXtOMVRuu86742348;     QfXtOMVRuu86742348 = QfXtOMVRuu33362465;     QfXtOMVRuu33362465 = QfXtOMVRuu30869425;     QfXtOMVRuu30869425 = QfXtOMVRuu26739386;     QfXtOMVRuu26739386 = QfXtOMVRuu14767234;     QfXtOMVRuu14767234 = QfXtOMVRuu56452392;     QfXtOMVRuu56452392 = QfXtOMVRuu43332977;     QfXtOMVRuu43332977 = QfXtOMVRuu38372091;     QfXtOMVRuu38372091 = QfXtOMVRuu3978476;     QfXtOMVRuu3978476 = QfXtOMVRuu536745;     QfXtOMVRuu536745 = QfXtOMVRuu66199228;     QfXtOMVRuu66199228 = QfXtOMVRuu6666636;     QfXtOMVRuu6666636 = QfXtOMVRuu97828898;     QfXtOMVRuu97828898 = QfXtOMVRuu49283412;     QfXtOMVRuu49283412 = QfXtOMVRuu13669397;     QfXtOMVRuu13669397 = QfXtOMVRuu2193899;     QfXtOMVRuu2193899 = QfXtOMVRuu95832978;     QfXtOMVRuu95832978 = QfXtOMVRuu7513099;     QfXtOMVRuu7513099 = QfXtOMVRuu79412763;     QfXtOMVRuu79412763 = QfXtOMVRuu64214743;     QfXtOMVRuu64214743 = QfXtOMVRuu59593213;     QfXtOMVRuu59593213 = QfXtOMVRuu80237453;     QfXtOMVRuu80237453 = QfXtOMVRuu84923697;     QfXtOMVRuu84923697 = QfXtOMVRuu8592058;     QfXtOMVRuu8592058 = QfXtOMVRuu69252075;     QfXtOMVRuu69252075 = QfXtOMVRuu60116070;     QfXtOMVRuu60116070 = QfXtOMVRuu17401984;     QfXtOMVRuu17401984 = QfXtOMVRuu70726984;     QfXtOMVRuu70726984 = QfXtOMVRuu91461550;     QfXtOMVRuu91461550 = QfXtOMVRuu27735685;     QfXtOMVRuu27735685 = QfXtOMVRuu39789893;     QfXtOMVRuu39789893 = QfXtOMVRuu54252487;     QfXtOMVRuu54252487 = QfXtOMVRuu30661812;     QfXtOMVRuu30661812 = QfXtOMVRuu6469263;     QfXtOMVRuu6469263 = QfXtOMVRuu56356877;     QfXtOMVRuu56356877 = QfXtOMVRuu15162099;     QfXtOMVRuu15162099 = QfXtOMVRuu45018734;     QfXtOMVRuu45018734 = QfXtOMVRuu2822199;     QfXtOMVRuu2822199 = QfXtOMVRuu85129403;     QfXtOMVRuu85129403 = QfXtOMVRuu49042649;     QfXtOMVRuu49042649 = QfXtOMVRuu4057971;     QfXtOMVRuu4057971 = QfXtOMVRuu91418672;     QfXtOMVRuu91418672 = QfXtOMVRuu97561554;     QfXtOMVRuu97561554 = QfXtOMVRuu27994391;     QfXtOMVRuu27994391 = QfXtOMVRuu60944561;     QfXtOMVRuu60944561 = QfXtOMVRuu66477952;     QfXtOMVRuu66477952 = QfXtOMVRuu39297082;     QfXtOMVRuu39297082 = QfXtOMVRuu78832155;     QfXtOMVRuu78832155 = QfXtOMVRuu28849925;     QfXtOMVRuu28849925 = QfXtOMVRuu95916627;     QfXtOMVRuu95916627 = QfXtOMVRuu67431958;     QfXtOMVRuu67431958 = QfXtOMVRuu94011310;     QfXtOMVRuu94011310 = QfXtOMVRuu60546469;     QfXtOMVRuu60546469 = QfXtOMVRuu81731904;     QfXtOMVRuu81731904 = QfXtOMVRuu38148522;     QfXtOMVRuu38148522 = QfXtOMVRuu6504896;     QfXtOMVRuu6504896 = QfXtOMVRuu48438768;     QfXtOMVRuu48438768 = QfXtOMVRuu22277367;     QfXtOMVRuu22277367 = QfXtOMVRuu57487310;     QfXtOMVRuu57487310 = QfXtOMVRuu54651164;     QfXtOMVRuu54651164 = QfXtOMVRuu39050408;     QfXtOMVRuu39050408 = QfXtOMVRuu72605993;     QfXtOMVRuu72605993 = QfXtOMVRuu46910541;     QfXtOMVRuu46910541 = QfXtOMVRuu76242790;     QfXtOMVRuu76242790 = QfXtOMVRuu60746851;     QfXtOMVRuu60746851 = QfXtOMVRuu11946742;     QfXtOMVRuu11946742 = QfXtOMVRuu76004823;     QfXtOMVRuu76004823 = QfXtOMVRuu91359635;     QfXtOMVRuu91359635 = QfXtOMVRuu92926535;     QfXtOMVRuu92926535 = QfXtOMVRuu98507297;     QfXtOMVRuu98507297 = QfXtOMVRuu57175164;     QfXtOMVRuu57175164 = QfXtOMVRuu93010780;     QfXtOMVRuu93010780 = QfXtOMVRuu22383696;     QfXtOMVRuu22383696 = QfXtOMVRuu30370115;     QfXtOMVRuu30370115 = QfXtOMVRuu60156772;     QfXtOMVRuu60156772 = QfXtOMVRuu68174540;     QfXtOMVRuu68174540 = QfXtOMVRuu82675899;     QfXtOMVRuu82675899 = QfXtOMVRuu56929306;     QfXtOMVRuu56929306 = QfXtOMVRuu47647496;     QfXtOMVRuu47647496 = QfXtOMVRuu2774124;     QfXtOMVRuu2774124 = QfXtOMVRuu20818989;     QfXtOMVRuu20818989 = QfXtOMVRuu75888158;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void tWNoXQbeVG85169538() {     int ZblzRGXbtT98275276 = -210135852;    int ZblzRGXbtT13909085 = -487895406;    int ZblzRGXbtT28665916 = -188659688;    int ZblzRGXbtT20501729 = -343066548;    int ZblzRGXbtT45565202 = 12507408;    int ZblzRGXbtT44673881 = -929842180;    int ZblzRGXbtT93695768 = -283758322;    int ZblzRGXbtT4164994 = -719068496;    int ZblzRGXbtT24306921 = -335824236;    int ZblzRGXbtT64908171 = -8784690;    int ZblzRGXbtT74667839 = -746469358;    int ZblzRGXbtT84018516 = -752061523;    int ZblzRGXbtT9527502 = -183456695;    int ZblzRGXbtT40725278 = -317794110;    int ZblzRGXbtT215 = -281505968;    int ZblzRGXbtT45780321 = -892078097;    int ZblzRGXbtT89782583 = 57226578;    int ZblzRGXbtT51923064 = -712019759;    int ZblzRGXbtT39023366 = -596126108;    int ZblzRGXbtT65258632 = -257430039;    int ZblzRGXbtT34245572 = -464175403;    int ZblzRGXbtT94684642 = -559804490;    int ZblzRGXbtT30032709 = -842311694;    int ZblzRGXbtT41267021 = -37836770;    int ZblzRGXbtT91992638 = -902926163;    int ZblzRGXbtT31635972 = -655990470;    int ZblzRGXbtT85461511 = -425646599;    int ZblzRGXbtT9889926 = -551128003;    int ZblzRGXbtT7429306 = -574543320;    int ZblzRGXbtT75356407 = -658176910;    int ZblzRGXbtT50957440 = -74712076;    int ZblzRGXbtT55370089 = -831142781;    int ZblzRGXbtT30322083 = -927498440;    int ZblzRGXbtT73058084 = -951972914;    int ZblzRGXbtT78423263 = 41529953;    int ZblzRGXbtT37525848 = -977377097;    int ZblzRGXbtT952811 = -167926158;    int ZblzRGXbtT33038864 = -692295759;    int ZblzRGXbtT12522875 = -136395074;    int ZblzRGXbtT79778927 = -935576071;    int ZblzRGXbtT8994825 = -105353598;    int ZblzRGXbtT73432088 = -504480850;    int ZblzRGXbtT14142250 = -413710503;    int ZblzRGXbtT68696194 = -647740019;    int ZblzRGXbtT28099337 = -430371711;    int ZblzRGXbtT25502266 = -657794285;    int ZblzRGXbtT83024381 = -567450223;    int ZblzRGXbtT36060225 = -532378377;    int ZblzRGXbtT22195590 = -633963947;    int ZblzRGXbtT36873638 = -881689761;    int ZblzRGXbtT63121703 = -680147786;    int ZblzRGXbtT4513070 = -444319845;    int ZblzRGXbtT84783666 = -686764789;    int ZblzRGXbtT16643423 = -40369864;    int ZblzRGXbtT50923966 = -339007151;    int ZblzRGXbtT3590634 = -650331362;    int ZblzRGXbtT83876376 = -645583712;    int ZblzRGXbtT87398894 = -50822919;    int ZblzRGXbtT28509090 = -440140386;    int ZblzRGXbtT13929230 = -331502123;    int ZblzRGXbtT59212369 = -404195582;    int ZblzRGXbtT83805843 = -732630320;    int ZblzRGXbtT96735688 = -44525176;    int ZblzRGXbtT48950513 = -677647326;    int ZblzRGXbtT13950732 = -934072614;    int ZblzRGXbtT19297751 = -915326578;    int ZblzRGXbtT53696434 = -824563083;    int ZblzRGXbtT36469418 = -231483782;    int ZblzRGXbtT62302015 = -259324064;    int ZblzRGXbtT62474366 = -304128872;    int ZblzRGXbtT44827510 = -624151940;    int ZblzRGXbtT56743719 = -250477664;    int ZblzRGXbtT39400190 = -475624685;    int ZblzRGXbtT59244438 = -660550037;    int ZblzRGXbtT56263808 = -52076441;    int ZblzRGXbtT60813484 = -959694553;    int ZblzRGXbtT80542393 = -46093988;    int ZblzRGXbtT61336514 = -94571675;    int ZblzRGXbtT13167685 = -607465059;    int ZblzRGXbtT66490373 = -145131878;    int ZblzRGXbtT48611591 = 11459753;    int ZblzRGXbtT49401287 = -893268222;    int ZblzRGXbtT87694335 = -917164056;    int ZblzRGXbtT70555667 = -692853559;    int ZblzRGXbtT12234705 = -978029125;    int ZblzRGXbtT46444371 = -630392231;    int ZblzRGXbtT70586423 = -44377992;    int ZblzRGXbtT13678661 = -787128576;    int ZblzRGXbtT22134118 = -512965763;    int ZblzRGXbtT74832630 = -308138685;    int ZblzRGXbtT53649472 = -231793385;    int ZblzRGXbtT13553917 = -17103239;    int ZblzRGXbtT4529774 = -152155373;    int ZblzRGXbtT98593644 = -804892952;    int ZblzRGXbtT20566559 = -431380490;    int ZblzRGXbtT25188981 = -372723279;    int ZblzRGXbtT76696399 = -359955675;    int ZblzRGXbtT65191737 = -736063177;    int ZblzRGXbtT54745463 = -713667406;    int ZblzRGXbtT8801587 = -210135852;     ZblzRGXbtT98275276 = ZblzRGXbtT13909085;     ZblzRGXbtT13909085 = ZblzRGXbtT28665916;     ZblzRGXbtT28665916 = ZblzRGXbtT20501729;     ZblzRGXbtT20501729 = ZblzRGXbtT45565202;     ZblzRGXbtT45565202 = ZblzRGXbtT44673881;     ZblzRGXbtT44673881 = ZblzRGXbtT93695768;     ZblzRGXbtT93695768 = ZblzRGXbtT4164994;     ZblzRGXbtT4164994 = ZblzRGXbtT24306921;     ZblzRGXbtT24306921 = ZblzRGXbtT64908171;     ZblzRGXbtT64908171 = ZblzRGXbtT74667839;     ZblzRGXbtT74667839 = ZblzRGXbtT84018516;     ZblzRGXbtT84018516 = ZblzRGXbtT9527502;     ZblzRGXbtT9527502 = ZblzRGXbtT40725278;     ZblzRGXbtT40725278 = ZblzRGXbtT215;     ZblzRGXbtT215 = ZblzRGXbtT45780321;     ZblzRGXbtT45780321 = ZblzRGXbtT89782583;     ZblzRGXbtT89782583 = ZblzRGXbtT51923064;     ZblzRGXbtT51923064 = ZblzRGXbtT39023366;     ZblzRGXbtT39023366 = ZblzRGXbtT65258632;     ZblzRGXbtT65258632 = ZblzRGXbtT34245572;     ZblzRGXbtT34245572 = ZblzRGXbtT94684642;     ZblzRGXbtT94684642 = ZblzRGXbtT30032709;     ZblzRGXbtT30032709 = ZblzRGXbtT41267021;     ZblzRGXbtT41267021 = ZblzRGXbtT91992638;     ZblzRGXbtT91992638 = ZblzRGXbtT31635972;     ZblzRGXbtT31635972 = ZblzRGXbtT85461511;     ZblzRGXbtT85461511 = ZblzRGXbtT9889926;     ZblzRGXbtT9889926 = ZblzRGXbtT7429306;     ZblzRGXbtT7429306 = ZblzRGXbtT75356407;     ZblzRGXbtT75356407 = ZblzRGXbtT50957440;     ZblzRGXbtT50957440 = ZblzRGXbtT55370089;     ZblzRGXbtT55370089 = ZblzRGXbtT30322083;     ZblzRGXbtT30322083 = ZblzRGXbtT73058084;     ZblzRGXbtT73058084 = ZblzRGXbtT78423263;     ZblzRGXbtT78423263 = ZblzRGXbtT37525848;     ZblzRGXbtT37525848 = ZblzRGXbtT952811;     ZblzRGXbtT952811 = ZblzRGXbtT33038864;     ZblzRGXbtT33038864 = ZblzRGXbtT12522875;     ZblzRGXbtT12522875 = ZblzRGXbtT79778927;     ZblzRGXbtT79778927 = ZblzRGXbtT8994825;     ZblzRGXbtT8994825 = ZblzRGXbtT73432088;     ZblzRGXbtT73432088 = ZblzRGXbtT14142250;     ZblzRGXbtT14142250 = ZblzRGXbtT68696194;     ZblzRGXbtT68696194 = ZblzRGXbtT28099337;     ZblzRGXbtT28099337 = ZblzRGXbtT25502266;     ZblzRGXbtT25502266 = ZblzRGXbtT83024381;     ZblzRGXbtT83024381 = ZblzRGXbtT36060225;     ZblzRGXbtT36060225 = ZblzRGXbtT22195590;     ZblzRGXbtT22195590 = ZblzRGXbtT36873638;     ZblzRGXbtT36873638 = ZblzRGXbtT63121703;     ZblzRGXbtT63121703 = ZblzRGXbtT4513070;     ZblzRGXbtT4513070 = ZblzRGXbtT84783666;     ZblzRGXbtT84783666 = ZblzRGXbtT16643423;     ZblzRGXbtT16643423 = ZblzRGXbtT50923966;     ZblzRGXbtT50923966 = ZblzRGXbtT3590634;     ZblzRGXbtT3590634 = ZblzRGXbtT83876376;     ZblzRGXbtT83876376 = ZblzRGXbtT87398894;     ZblzRGXbtT87398894 = ZblzRGXbtT28509090;     ZblzRGXbtT28509090 = ZblzRGXbtT13929230;     ZblzRGXbtT13929230 = ZblzRGXbtT59212369;     ZblzRGXbtT59212369 = ZblzRGXbtT83805843;     ZblzRGXbtT83805843 = ZblzRGXbtT96735688;     ZblzRGXbtT96735688 = ZblzRGXbtT48950513;     ZblzRGXbtT48950513 = ZblzRGXbtT13950732;     ZblzRGXbtT13950732 = ZblzRGXbtT19297751;     ZblzRGXbtT19297751 = ZblzRGXbtT53696434;     ZblzRGXbtT53696434 = ZblzRGXbtT36469418;     ZblzRGXbtT36469418 = ZblzRGXbtT62302015;     ZblzRGXbtT62302015 = ZblzRGXbtT62474366;     ZblzRGXbtT62474366 = ZblzRGXbtT44827510;     ZblzRGXbtT44827510 = ZblzRGXbtT56743719;     ZblzRGXbtT56743719 = ZblzRGXbtT39400190;     ZblzRGXbtT39400190 = ZblzRGXbtT59244438;     ZblzRGXbtT59244438 = ZblzRGXbtT56263808;     ZblzRGXbtT56263808 = ZblzRGXbtT60813484;     ZblzRGXbtT60813484 = ZblzRGXbtT80542393;     ZblzRGXbtT80542393 = ZblzRGXbtT61336514;     ZblzRGXbtT61336514 = ZblzRGXbtT13167685;     ZblzRGXbtT13167685 = ZblzRGXbtT66490373;     ZblzRGXbtT66490373 = ZblzRGXbtT48611591;     ZblzRGXbtT48611591 = ZblzRGXbtT49401287;     ZblzRGXbtT49401287 = ZblzRGXbtT87694335;     ZblzRGXbtT87694335 = ZblzRGXbtT70555667;     ZblzRGXbtT70555667 = ZblzRGXbtT12234705;     ZblzRGXbtT12234705 = ZblzRGXbtT46444371;     ZblzRGXbtT46444371 = ZblzRGXbtT70586423;     ZblzRGXbtT70586423 = ZblzRGXbtT13678661;     ZblzRGXbtT13678661 = ZblzRGXbtT22134118;     ZblzRGXbtT22134118 = ZblzRGXbtT74832630;     ZblzRGXbtT74832630 = ZblzRGXbtT53649472;     ZblzRGXbtT53649472 = ZblzRGXbtT13553917;     ZblzRGXbtT13553917 = ZblzRGXbtT4529774;     ZblzRGXbtT4529774 = ZblzRGXbtT98593644;     ZblzRGXbtT98593644 = ZblzRGXbtT20566559;     ZblzRGXbtT20566559 = ZblzRGXbtT25188981;     ZblzRGXbtT25188981 = ZblzRGXbtT76696399;     ZblzRGXbtT76696399 = ZblzRGXbtT65191737;     ZblzRGXbtT65191737 = ZblzRGXbtT54745463;     ZblzRGXbtT54745463 = ZblzRGXbtT8801587;     ZblzRGXbtT8801587 = ZblzRGXbtT98275276;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void yixZWdhuYt21814514() {     int OSmPftPSEO20662394 = -944539815;    int OSmPftPSEO98256585 = -436438460;    int OSmPftPSEO57435194 = -428313533;    int OSmPftPSEO35508417 = -349474079;    int OSmPftPSEO43739456 = -578549594;    int OSmPftPSEO59556997 = -790450558;    int OSmPftPSEO85851507 = -105745628;    int OSmPftPSEO79798852 = -89665140;    int OSmPftPSEO21470052 = -220020275;    int OSmPftPSEO56671756 = -851526023;    int OSmPftPSEO12209700 = -743093759;    int OSmPftPSEO39921466 = 97193184;    int OSmPftPSEO76535682 = -802174015;    int OSmPftPSEO83340030 = -845502867;    int OSmPftPSEO36735492 = 91016319;    int OSmPftPSEO90036233 = -173745640;    int OSmPftPSEO39605936 = -220053242;    int OSmPftPSEO57899480 = -63762450;    int OSmPftPSEO80304996 = -731228233;    int OSmPftPSEO43774915 = -510064492;    int OSmPftPSEO35128679 = -455662711;    int OSmPftPSEO58499861 = 35429196;    int OSmPftPSEO33326032 = 81607714;    int OSmPftPSEO67766808 = -601829627;    int OSmPftPSEO27532886 = -524485287;    int OSmPftPSEO19938967 = 115307;    int OSmPftPSEO32550932 = -183233092;    int OSmPftPSEO15801376 = -339582456;    int OSmPftPSEO14321866 = -918800439;    int OSmPftPSEO84513587 = -578472777;    int OSmPftPSEO95248245 = -156028228;    int OSmPftPSEO12911281 = -383214135;    int OSmPftPSEO11360754 = -356841935;    int OSmPftPSEO32446771 = -176099764;    int OSmPftPSEO54652628 = -22226470;    int OSmPftPSEO79218717 = -192772473;    int OSmPftPSEO94392523 = 99125891;    int OSmPftPSEO86664965 = -559921231;    int OSmPftPSEO60831006 = -537441106;    int OSmPftPSEO99964641 = 28199899;    int OSmPftPSEO37752195 = -608531264;    int OSmPftPSEO61940480 = -262389541;    int OSmPftPSEO19692442 = -301241086;    int OSmPftPSEO68140314 = -645103668;    int OSmPftPSEO96082602 = -239515189;    int OSmPftPSEO33602548 = -840023380;    int OSmPftPSEO95321778 = -45860476;    int OSmPftPSEO80658899 = -288913368;    int OSmPftPSEO16655495 = -583680645;    int OSmPftPSEO33957384 = 76289980;    int OSmPftPSEO71990919 = 64775937;    int OSmPftPSEO78364326 = -875139127;    int OSmPftPSEO63098069 = -358676019;    int OSmPftPSEO76929968 = -316050673;    int OSmPftPSEO86685834 = -479711266;    int OSmPftPSEO62162532 = -879969012;    int OSmPftPSEO64930553 = -418046175;    int OSmPftPSEO89668386 = -826483906;    int OSmPftPSEO7975532 = -824988793;    int OSmPftPSEO23800490 = -478664901;    int OSmPftPSEO27006066 = -507217467;    int OSmPftPSEO70050132 = -766163172;    int OSmPftPSEO65476986 = -170864701;    int OSmPftPSEO36956465 = -641547499;    int OSmPftPSEO61423511 = -595497795;    int OSmPftPSEO99298419 = -259879624;    int OSmPftPSEO28560713 = -545964882;    int OSmPftPSEO44088911 = -526074251;    int OSmPftPSEO28687403 = -723276398;    int OSmPftPSEO57516774 = -716211209;    int OSmPftPSEO95643710 = -172871531;    int OSmPftPSEO52940970 = -660132012;    int OSmPftPSEO97068474 = -526321344;    int OSmPftPSEO80340354 = -659428133;    int OSmPftPSEO6022720 = -901533229;    int OSmPftPSEO73188199 = -93273170;    int OSmPftPSEO38807420 = -663329718;    int OSmPftPSEO65185718 = -273288618;    int OSmPftPSEO71684205 = -262314438;    int OSmPftPSEO93930337 = -684461907;    int OSmPftPSEO24617188 = -954024218;    int OSmPftPSEO51892033 = -894319724;    int OSmPftPSEO99145881 = -755901812;    int OSmPftPSEO80364482 = -895090420;    int OSmPftPSEO12522668 = -543248715;    int OSmPftPSEO16883919 = -280889101;    int OSmPftPSEO49813211 = 75461883;    int OSmPftPSEO34430786 = 59208737;    int OSmPftPSEO45760937 = -696388498;    int OSmPftPSEO92490095 = -142257458;    int OSmPftPSEO14288164 = -774726299;    int OSmPftPSEO4724138 = -74390204;    int OSmPftPSEO78689433 = -734932438;    int OSmPftPSEO37030516 = 41223794;    int OSmPftPSEO72958576 = -464582634;    int OSmPftPSEO67702063 = -842368092;    int OSmPftPSEO96463493 = 8475159;    int OSmPftPSEO82735977 = -659693588;    int OSmPftPSEO6716804 = 50394126;    int OSmPftPSEO96784183 = -944539815;     OSmPftPSEO20662394 = OSmPftPSEO98256585;     OSmPftPSEO98256585 = OSmPftPSEO57435194;     OSmPftPSEO57435194 = OSmPftPSEO35508417;     OSmPftPSEO35508417 = OSmPftPSEO43739456;     OSmPftPSEO43739456 = OSmPftPSEO59556997;     OSmPftPSEO59556997 = OSmPftPSEO85851507;     OSmPftPSEO85851507 = OSmPftPSEO79798852;     OSmPftPSEO79798852 = OSmPftPSEO21470052;     OSmPftPSEO21470052 = OSmPftPSEO56671756;     OSmPftPSEO56671756 = OSmPftPSEO12209700;     OSmPftPSEO12209700 = OSmPftPSEO39921466;     OSmPftPSEO39921466 = OSmPftPSEO76535682;     OSmPftPSEO76535682 = OSmPftPSEO83340030;     OSmPftPSEO83340030 = OSmPftPSEO36735492;     OSmPftPSEO36735492 = OSmPftPSEO90036233;     OSmPftPSEO90036233 = OSmPftPSEO39605936;     OSmPftPSEO39605936 = OSmPftPSEO57899480;     OSmPftPSEO57899480 = OSmPftPSEO80304996;     OSmPftPSEO80304996 = OSmPftPSEO43774915;     OSmPftPSEO43774915 = OSmPftPSEO35128679;     OSmPftPSEO35128679 = OSmPftPSEO58499861;     OSmPftPSEO58499861 = OSmPftPSEO33326032;     OSmPftPSEO33326032 = OSmPftPSEO67766808;     OSmPftPSEO67766808 = OSmPftPSEO27532886;     OSmPftPSEO27532886 = OSmPftPSEO19938967;     OSmPftPSEO19938967 = OSmPftPSEO32550932;     OSmPftPSEO32550932 = OSmPftPSEO15801376;     OSmPftPSEO15801376 = OSmPftPSEO14321866;     OSmPftPSEO14321866 = OSmPftPSEO84513587;     OSmPftPSEO84513587 = OSmPftPSEO95248245;     OSmPftPSEO95248245 = OSmPftPSEO12911281;     OSmPftPSEO12911281 = OSmPftPSEO11360754;     OSmPftPSEO11360754 = OSmPftPSEO32446771;     OSmPftPSEO32446771 = OSmPftPSEO54652628;     OSmPftPSEO54652628 = OSmPftPSEO79218717;     OSmPftPSEO79218717 = OSmPftPSEO94392523;     OSmPftPSEO94392523 = OSmPftPSEO86664965;     OSmPftPSEO86664965 = OSmPftPSEO60831006;     OSmPftPSEO60831006 = OSmPftPSEO99964641;     OSmPftPSEO99964641 = OSmPftPSEO37752195;     OSmPftPSEO37752195 = OSmPftPSEO61940480;     OSmPftPSEO61940480 = OSmPftPSEO19692442;     OSmPftPSEO19692442 = OSmPftPSEO68140314;     OSmPftPSEO68140314 = OSmPftPSEO96082602;     OSmPftPSEO96082602 = OSmPftPSEO33602548;     OSmPftPSEO33602548 = OSmPftPSEO95321778;     OSmPftPSEO95321778 = OSmPftPSEO80658899;     OSmPftPSEO80658899 = OSmPftPSEO16655495;     OSmPftPSEO16655495 = OSmPftPSEO33957384;     OSmPftPSEO33957384 = OSmPftPSEO71990919;     OSmPftPSEO71990919 = OSmPftPSEO78364326;     OSmPftPSEO78364326 = OSmPftPSEO63098069;     OSmPftPSEO63098069 = OSmPftPSEO76929968;     OSmPftPSEO76929968 = OSmPftPSEO86685834;     OSmPftPSEO86685834 = OSmPftPSEO62162532;     OSmPftPSEO62162532 = OSmPftPSEO64930553;     OSmPftPSEO64930553 = OSmPftPSEO89668386;     OSmPftPSEO89668386 = OSmPftPSEO7975532;     OSmPftPSEO7975532 = OSmPftPSEO23800490;     OSmPftPSEO23800490 = OSmPftPSEO27006066;     OSmPftPSEO27006066 = OSmPftPSEO70050132;     OSmPftPSEO70050132 = OSmPftPSEO65476986;     OSmPftPSEO65476986 = OSmPftPSEO36956465;     OSmPftPSEO36956465 = OSmPftPSEO61423511;     OSmPftPSEO61423511 = OSmPftPSEO99298419;     OSmPftPSEO99298419 = OSmPftPSEO28560713;     OSmPftPSEO28560713 = OSmPftPSEO44088911;     OSmPftPSEO44088911 = OSmPftPSEO28687403;     OSmPftPSEO28687403 = OSmPftPSEO57516774;     OSmPftPSEO57516774 = OSmPftPSEO95643710;     OSmPftPSEO95643710 = OSmPftPSEO52940970;     OSmPftPSEO52940970 = OSmPftPSEO97068474;     OSmPftPSEO97068474 = OSmPftPSEO80340354;     OSmPftPSEO80340354 = OSmPftPSEO6022720;     OSmPftPSEO6022720 = OSmPftPSEO73188199;     OSmPftPSEO73188199 = OSmPftPSEO38807420;     OSmPftPSEO38807420 = OSmPftPSEO65185718;     OSmPftPSEO65185718 = OSmPftPSEO71684205;     OSmPftPSEO71684205 = OSmPftPSEO93930337;     OSmPftPSEO93930337 = OSmPftPSEO24617188;     OSmPftPSEO24617188 = OSmPftPSEO51892033;     OSmPftPSEO51892033 = OSmPftPSEO99145881;     OSmPftPSEO99145881 = OSmPftPSEO80364482;     OSmPftPSEO80364482 = OSmPftPSEO12522668;     OSmPftPSEO12522668 = OSmPftPSEO16883919;     OSmPftPSEO16883919 = OSmPftPSEO49813211;     OSmPftPSEO49813211 = OSmPftPSEO34430786;     OSmPftPSEO34430786 = OSmPftPSEO45760937;     OSmPftPSEO45760937 = OSmPftPSEO92490095;     OSmPftPSEO92490095 = OSmPftPSEO14288164;     OSmPftPSEO14288164 = OSmPftPSEO4724138;     OSmPftPSEO4724138 = OSmPftPSEO78689433;     OSmPftPSEO78689433 = OSmPftPSEO37030516;     OSmPftPSEO37030516 = OSmPftPSEO72958576;     OSmPftPSEO72958576 = OSmPftPSEO67702063;     OSmPftPSEO67702063 = OSmPftPSEO96463493;     OSmPftPSEO96463493 = OSmPftPSEO82735977;     OSmPftPSEO82735977 = OSmPftPSEO6716804;     OSmPftPSEO6716804 = OSmPftPSEO96784183;     OSmPftPSEO96784183 = OSmPftPSEO20662394;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void PcsrtjmLqV58459488() {     int LGorGXFSJs43049511 = -578943778;    int LGorGXFSJs82604085 = -384981515;    int LGorGXFSJs86204473 = -667967377;    int LGorGXFSJs50515106 = -355881611;    int LGorGXFSJs41913711 = -69606595;    int LGorGXFSJs74440114 = -651058936;    int LGorGXFSJs78007246 = 72267066;    int LGorGXFSJs55432711 = -560261784;    int LGorGXFSJs18633183 = -104216314;    int LGorGXFSJs48435341 = -594267356;    int LGorGXFSJs49751559 = -739718160;    int LGorGXFSJs95824414 = -153552110;    int LGorGXFSJs43543863 = -320891335;    int LGorGXFSJs25954783 = -273211624;    int LGorGXFSJs73470768 = -636461394;    int LGorGXFSJs34292146 = -555413182;    int LGorGXFSJs89429287 = -497333062;    int LGorGXFSJs63875896 = -515505141;    int LGorGXFSJs21586628 = -866330358;    int LGorGXFSJs22291199 = -762698946;    int LGorGXFSJs36011787 = -447150020;    int LGorGXFSJs22315080 = -469337117;    int LGorGXFSJs36619355 = -94472878;    int LGorGXFSJs94266595 = -65822484;    int LGorGXFSJs63073132 = -146044412;    int LGorGXFSJs8241962 = -443778917;    int LGorGXFSJs79640352 = 59180415;    int LGorGXFSJs21712826 = -128036910;    int LGorGXFSJs21214427 = -163057558;    int LGorGXFSJs93670766 = -498768643;    int LGorGXFSJs39539051 = -237344380;    int LGorGXFSJs70452471 = 64714510;    int LGorGXFSJs92399423 = -886185430;    int LGorGXFSJs91835458 = -500226614;    int LGorGXFSJs30881993 = -85982892;    int LGorGXFSJs20911587 = -508167850;    int LGorGXFSJs87832235 = -733822061;    int LGorGXFSJs40291066 = -427546703;    int LGorGXFSJs9139137 = -938487138;    int LGorGXFSJs20150357 = -108024130;    int LGorGXFSJs66509566 = -11708930;    int LGorGXFSJs50448871 = -20298232;    int LGorGXFSJs25242634 = -188771670;    int LGorGXFSJs67584433 = -642467317;    int LGorGXFSJs64065869 = -48658667;    int LGorGXFSJs41702829 = 77747524;    int LGorGXFSJs7619176 = -624270729;    int LGorGXFSJs25257574 = -45448359;    int LGorGXFSJs11115400 = -533397344;    int LGorGXFSJs31041130 = -65730279;    int LGorGXFSJs80860136 = -290300340;    int LGorGXFSJs52215584 = -205958409;    int LGorGXFSJs41412473 = -30587249;    int LGorGXFSJs37216513 = -591731482;    int LGorGXFSJs22447702 = -620415382;    int LGorGXFSJs20734432 = -9606662;    int LGorGXFSJs45984730 = -190508637;    int LGorGXFSJs91937877 = -502144893;    int LGorGXFSJs87441973 = -109837200;    int LGorGXFSJs33671750 = -625827678;    int LGorGXFSJs94799762 = -610239352;    int LGorGXFSJs56294421 = -799696025;    int LGorGXFSJs34218285 = -297204226;    int LGorGXFSJs24962416 = -605447671;    int LGorGXFSJs8896291 = -256922976;    int LGorGXFSJs79299087 = -704432670;    int LGorGXFSJs3424991 = -267366680;    int LGorGXFSJs51708405 = -820664721;    int LGorGXFSJs95072790 = -87228733;    int LGorGXFSJs52559182 = -28293545;    int LGorGXFSJs46459911 = -821591122;    int LGorGXFSJs49138221 = 30213640;    int LGorGXFSJs54736759 = -577018003;    int LGorGXFSJs1436272 = -658306229;    int LGorGXFSJs55781632 = -650990016;    int LGorGXFSJs85562915 = -326851788;    int LGorGXFSJs97072445 = -180565447;    int LGorGXFSJs69034922 = -452005561;    int LGorGXFSJs30200726 = 82836182;    int LGorGXFSJs21370303 = -123791936;    int LGorGXFSJs622786 = -819508189;    int LGorGXFSJs54382778 = -895371226;    int LGorGXFSJs10597427 = -594639567;    int LGorGXFSJs90173297 = 2672720;    int LGorGXFSJs12810631 = -108468304;    int LGorGXFSJs87323467 = 68614029;    int LGorGXFSJs29039999 = -904698241;    int LGorGXFSJs55182911 = -194453949;    int LGorGXFSJs69387757 = -879811233;    int LGorGXFSJs10147562 = 23623769;    int LGorGXFSJs74926856 = -217659213;    int LGorGXFSJs95894357 = -131677168;    int LGorGXFSJs52849093 = -217709503;    int LGorGXFSJs75467387 = -212659460;    int LGorGXFSJs25350595 = -497784779;    int LGorGXFSJs10215146 = -212012906;    int LGorGXFSJs16230587 = -723094007;    int LGorGXFSJs280218 = -583323999;    int LGorGXFSJs58688143 = -285544342;    int LGorGXFSJs84766781 = -578943778;     LGorGXFSJs43049511 = LGorGXFSJs82604085;     LGorGXFSJs82604085 = LGorGXFSJs86204473;     LGorGXFSJs86204473 = LGorGXFSJs50515106;     LGorGXFSJs50515106 = LGorGXFSJs41913711;     LGorGXFSJs41913711 = LGorGXFSJs74440114;     LGorGXFSJs74440114 = LGorGXFSJs78007246;     LGorGXFSJs78007246 = LGorGXFSJs55432711;     LGorGXFSJs55432711 = LGorGXFSJs18633183;     LGorGXFSJs18633183 = LGorGXFSJs48435341;     LGorGXFSJs48435341 = LGorGXFSJs49751559;     LGorGXFSJs49751559 = LGorGXFSJs95824414;     LGorGXFSJs95824414 = LGorGXFSJs43543863;     LGorGXFSJs43543863 = LGorGXFSJs25954783;     LGorGXFSJs25954783 = LGorGXFSJs73470768;     LGorGXFSJs73470768 = LGorGXFSJs34292146;     LGorGXFSJs34292146 = LGorGXFSJs89429287;     LGorGXFSJs89429287 = LGorGXFSJs63875896;     LGorGXFSJs63875896 = LGorGXFSJs21586628;     LGorGXFSJs21586628 = LGorGXFSJs22291199;     LGorGXFSJs22291199 = LGorGXFSJs36011787;     LGorGXFSJs36011787 = LGorGXFSJs22315080;     LGorGXFSJs22315080 = LGorGXFSJs36619355;     LGorGXFSJs36619355 = LGorGXFSJs94266595;     LGorGXFSJs94266595 = LGorGXFSJs63073132;     LGorGXFSJs63073132 = LGorGXFSJs8241962;     LGorGXFSJs8241962 = LGorGXFSJs79640352;     LGorGXFSJs79640352 = LGorGXFSJs21712826;     LGorGXFSJs21712826 = LGorGXFSJs21214427;     LGorGXFSJs21214427 = LGorGXFSJs93670766;     LGorGXFSJs93670766 = LGorGXFSJs39539051;     LGorGXFSJs39539051 = LGorGXFSJs70452471;     LGorGXFSJs70452471 = LGorGXFSJs92399423;     LGorGXFSJs92399423 = LGorGXFSJs91835458;     LGorGXFSJs91835458 = LGorGXFSJs30881993;     LGorGXFSJs30881993 = LGorGXFSJs20911587;     LGorGXFSJs20911587 = LGorGXFSJs87832235;     LGorGXFSJs87832235 = LGorGXFSJs40291066;     LGorGXFSJs40291066 = LGorGXFSJs9139137;     LGorGXFSJs9139137 = LGorGXFSJs20150357;     LGorGXFSJs20150357 = LGorGXFSJs66509566;     LGorGXFSJs66509566 = LGorGXFSJs50448871;     LGorGXFSJs50448871 = LGorGXFSJs25242634;     LGorGXFSJs25242634 = LGorGXFSJs67584433;     LGorGXFSJs67584433 = LGorGXFSJs64065869;     LGorGXFSJs64065869 = LGorGXFSJs41702829;     LGorGXFSJs41702829 = LGorGXFSJs7619176;     LGorGXFSJs7619176 = LGorGXFSJs25257574;     LGorGXFSJs25257574 = LGorGXFSJs11115400;     LGorGXFSJs11115400 = LGorGXFSJs31041130;     LGorGXFSJs31041130 = LGorGXFSJs80860136;     LGorGXFSJs80860136 = LGorGXFSJs52215584;     LGorGXFSJs52215584 = LGorGXFSJs41412473;     LGorGXFSJs41412473 = LGorGXFSJs37216513;     LGorGXFSJs37216513 = LGorGXFSJs22447702;     LGorGXFSJs22447702 = LGorGXFSJs20734432;     LGorGXFSJs20734432 = LGorGXFSJs45984730;     LGorGXFSJs45984730 = LGorGXFSJs91937877;     LGorGXFSJs91937877 = LGorGXFSJs87441973;     LGorGXFSJs87441973 = LGorGXFSJs33671750;     LGorGXFSJs33671750 = LGorGXFSJs94799762;     LGorGXFSJs94799762 = LGorGXFSJs56294421;     LGorGXFSJs56294421 = LGorGXFSJs34218285;     LGorGXFSJs34218285 = LGorGXFSJs24962416;     LGorGXFSJs24962416 = LGorGXFSJs8896291;     LGorGXFSJs8896291 = LGorGXFSJs79299087;     LGorGXFSJs79299087 = LGorGXFSJs3424991;     LGorGXFSJs3424991 = LGorGXFSJs51708405;     LGorGXFSJs51708405 = LGorGXFSJs95072790;     LGorGXFSJs95072790 = LGorGXFSJs52559182;     LGorGXFSJs52559182 = LGorGXFSJs46459911;     LGorGXFSJs46459911 = LGorGXFSJs49138221;     LGorGXFSJs49138221 = LGorGXFSJs54736759;     LGorGXFSJs54736759 = LGorGXFSJs1436272;     LGorGXFSJs1436272 = LGorGXFSJs55781632;     LGorGXFSJs55781632 = LGorGXFSJs85562915;     LGorGXFSJs85562915 = LGorGXFSJs97072445;     LGorGXFSJs97072445 = LGorGXFSJs69034922;     LGorGXFSJs69034922 = LGorGXFSJs30200726;     LGorGXFSJs30200726 = LGorGXFSJs21370303;     LGorGXFSJs21370303 = LGorGXFSJs622786;     LGorGXFSJs622786 = LGorGXFSJs54382778;     LGorGXFSJs54382778 = LGorGXFSJs10597427;     LGorGXFSJs10597427 = LGorGXFSJs90173297;     LGorGXFSJs90173297 = LGorGXFSJs12810631;     LGorGXFSJs12810631 = LGorGXFSJs87323467;     LGorGXFSJs87323467 = LGorGXFSJs29039999;     LGorGXFSJs29039999 = LGorGXFSJs55182911;     LGorGXFSJs55182911 = LGorGXFSJs69387757;     LGorGXFSJs69387757 = LGorGXFSJs10147562;     LGorGXFSJs10147562 = LGorGXFSJs74926856;     LGorGXFSJs74926856 = LGorGXFSJs95894357;     LGorGXFSJs95894357 = LGorGXFSJs52849093;     LGorGXFSJs52849093 = LGorGXFSJs75467387;     LGorGXFSJs75467387 = LGorGXFSJs25350595;     LGorGXFSJs25350595 = LGorGXFSJs10215146;     LGorGXFSJs10215146 = LGorGXFSJs16230587;     LGorGXFSJs16230587 = LGorGXFSJs280218;     LGorGXFSJs280218 = LGorGXFSJs58688143;     LGorGXFSJs58688143 = LGorGXFSJs84766781;     LGorGXFSJs84766781 = LGorGXFSJs43049511;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void wTHIcjjCtw95104462() {     int ocdmDnQUWm65436628 = -213347741;    int ocdmDnQUWm66951586 = -333524570;    int ocdmDnQUWm14973752 = -907621221;    int ocdmDnQUWm65521794 = -362289143;    int ocdmDnQUWm40087965 = -660663596;    int ocdmDnQUWm89323231 = -511667314;    int ocdmDnQUWm70162985 = -849720240;    int ocdmDnQUWm31066570 = 69141572;    int ocdmDnQUWm15796314 = 11587647;    int ocdmDnQUWm40198925 = -337008689;    int ocdmDnQUWm87293418 = -736342560;    int ocdmDnQUWm51727363 = -404297403;    int ocdmDnQUWm10552044 = -939608654;    int ocdmDnQUWm68569535 = -800920381;    int ocdmDnQUWm10206046 = -263939107;    int ocdmDnQUWm78548059 = -937080725;    int ocdmDnQUWm39252639 = -774612881;    int ocdmDnQUWm69852312 = -967247831;    int ocdmDnQUWm62868258 = 98567517;    int ocdmDnQUWm807482 = 84666600;    int ocdmDnQUWm36894894 = -438637328;    int ocdmDnQUWm86130298 = -974103430;    int ocdmDnQUWm39912679 = -270553470;    int ocdmDnQUWm20766383 = -629815341;    int ocdmDnQUWm98613378 = -867603536;    int ocdmDnQUWm96544955 = -887673141;    int ocdmDnQUWm26729773 = -798406077;    int ocdmDnQUWm27624276 = 83508637;    int ocdmDnQUWm28106988 = -507314677;    int ocdmDnQUWm2827947 = -419064510;    int ocdmDnQUWm83829855 = -318660532;    int ocdmDnQUWm27993662 = -587356845;    int ocdmDnQUWm73438094 = -315528925;    int ocdmDnQUWm51224146 = -824353464;    int ocdmDnQUWm7111358 = -149739315;    int ocdmDnQUWm62604455 = -823563226;    int ocdmDnQUWm81271947 = -466770012;    int ocdmDnQUWm93917166 = -295172174;    int ocdmDnQUWm57447268 = -239533169;    int ocdmDnQUWm40336071 = -244248159;    int ocdmDnQUWm95266936 = -514886596;    int ocdmDnQUWm38957262 = -878206923;    int ocdmDnQUWm30792826 = -76302254;    int ocdmDnQUWm67028552 = -639830966;    int ocdmDnQUWm32049136 = -957802145;    int ocdmDnQUWm49803111 = -104481571;    int ocdmDnQUWm19916574 = -102680982;    int ocdmDnQUWm69856248 = -901983350;    int ocdmDnQUWm5575305 = -483114042;    int ocdmDnQUWm28124876 = -207750538;    int ocdmDnQUWm89729352 = -645376617;    int ocdmDnQUWm26066841 = -636777692;    int ocdmDnQUWm19726876 = -802498479;    int ocdmDnQUWm97503058 = -867412291;    int ocdmDnQUWm58209569 = -761119497;    int ocdmDnQUWm79306330 = -239244312;    int ocdmDnQUWm27038908 = 37028900;    int ocdmDnQUWm94207368 = -177805880;    int ocdmDnQUWm66908415 = -494685607;    int ocdmDnQUWm43543009 = -772990455;    int ocdmDnQUWm62593458 = -713261237;    int ocdmDnQUWm42538709 = -833228877;    int ocdmDnQUWm2959583 = -423543751;    int ocdmDnQUWm12968368 = -569347844;    int ocdmDnQUWm56369070 = 81651843;    int ocdmDnQUWm59299756 = -48985716;    int ocdmDnQUWm78289269 = 11231521;    int ocdmDnQUWm59327898 = -15255191;    int ocdmDnQUWm61458178 = -551181067;    int ocdmDnQUWm47601590 = -440375882;    int ocdmDnQUWm97276111 = -370310713;    int ocdmDnQUWm45335472 = -379440707;    int ocdmDnQUWm12405045 = -627714662;    int ocdmDnQUWm22532188 = -657184325;    int ocdmDnQUWm5540545 = -400446804;    int ocdmDnQUWm97937631 = -560430405;    int ocdmDnQUWm55337472 = -797801177;    int ocdmDnQUWm72884126 = -630722504;    int ocdmDnQUWm88717246 = -672013197;    int ocdmDnQUWm48810268 = -663121965;    int ocdmDnQUWm76628382 = -684992160;    int ocdmDnQUWm56873524 = -896422728;    int ocdmDnQUWm22048972 = -433377322;    int ocdmDnQUWm99982112 = -199564140;    int ocdmDnQUWm13098594 = -773687894;    int ocdmDnQUWm57763015 = -681882841;    int ocdmDnQUWm8266787 = -784858366;    int ocdmDnQUWm75935036 = -448116635;    int ocdmDnQUWm93014576 = 36766032;    int ocdmDnQUWm27805027 = -910495003;    int ocdmDnQUWm35565548 = -760592126;    int ocdmDnQUWm87064578 = -188964133;    int ocdmDnQUWm27008752 = -800486568;    int ocdmDnQUWm13904260 = -466542715;    int ocdmDnQUWm77742612 = -530986923;    int ocdmDnQUWm52728228 = -681657719;    int ocdmDnQUWm35997680 = -354663173;    int ocdmDnQUWm17824458 = -506954410;    int ocdmDnQUWm10659483 = -621482810;    int ocdmDnQUWm72749379 = -213347741;     ocdmDnQUWm65436628 = ocdmDnQUWm66951586;     ocdmDnQUWm66951586 = ocdmDnQUWm14973752;     ocdmDnQUWm14973752 = ocdmDnQUWm65521794;     ocdmDnQUWm65521794 = ocdmDnQUWm40087965;     ocdmDnQUWm40087965 = ocdmDnQUWm89323231;     ocdmDnQUWm89323231 = ocdmDnQUWm70162985;     ocdmDnQUWm70162985 = ocdmDnQUWm31066570;     ocdmDnQUWm31066570 = ocdmDnQUWm15796314;     ocdmDnQUWm15796314 = ocdmDnQUWm40198925;     ocdmDnQUWm40198925 = ocdmDnQUWm87293418;     ocdmDnQUWm87293418 = ocdmDnQUWm51727363;     ocdmDnQUWm51727363 = ocdmDnQUWm10552044;     ocdmDnQUWm10552044 = ocdmDnQUWm68569535;     ocdmDnQUWm68569535 = ocdmDnQUWm10206046;     ocdmDnQUWm10206046 = ocdmDnQUWm78548059;     ocdmDnQUWm78548059 = ocdmDnQUWm39252639;     ocdmDnQUWm39252639 = ocdmDnQUWm69852312;     ocdmDnQUWm69852312 = ocdmDnQUWm62868258;     ocdmDnQUWm62868258 = ocdmDnQUWm807482;     ocdmDnQUWm807482 = ocdmDnQUWm36894894;     ocdmDnQUWm36894894 = ocdmDnQUWm86130298;     ocdmDnQUWm86130298 = ocdmDnQUWm39912679;     ocdmDnQUWm39912679 = ocdmDnQUWm20766383;     ocdmDnQUWm20766383 = ocdmDnQUWm98613378;     ocdmDnQUWm98613378 = ocdmDnQUWm96544955;     ocdmDnQUWm96544955 = ocdmDnQUWm26729773;     ocdmDnQUWm26729773 = ocdmDnQUWm27624276;     ocdmDnQUWm27624276 = ocdmDnQUWm28106988;     ocdmDnQUWm28106988 = ocdmDnQUWm2827947;     ocdmDnQUWm2827947 = ocdmDnQUWm83829855;     ocdmDnQUWm83829855 = ocdmDnQUWm27993662;     ocdmDnQUWm27993662 = ocdmDnQUWm73438094;     ocdmDnQUWm73438094 = ocdmDnQUWm51224146;     ocdmDnQUWm51224146 = ocdmDnQUWm7111358;     ocdmDnQUWm7111358 = ocdmDnQUWm62604455;     ocdmDnQUWm62604455 = ocdmDnQUWm81271947;     ocdmDnQUWm81271947 = ocdmDnQUWm93917166;     ocdmDnQUWm93917166 = ocdmDnQUWm57447268;     ocdmDnQUWm57447268 = ocdmDnQUWm40336071;     ocdmDnQUWm40336071 = ocdmDnQUWm95266936;     ocdmDnQUWm95266936 = ocdmDnQUWm38957262;     ocdmDnQUWm38957262 = ocdmDnQUWm30792826;     ocdmDnQUWm30792826 = ocdmDnQUWm67028552;     ocdmDnQUWm67028552 = ocdmDnQUWm32049136;     ocdmDnQUWm32049136 = ocdmDnQUWm49803111;     ocdmDnQUWm49803111 = ocdmDnQUWm19916574;     ocdmDnQUWm19916574 = ocdmDnQUWm69856248;     ocdmDnQUWm69856248 = ocdmDnQUWm5575305;     ocdmDnQUWm5575305 = ocdmDnQUWm28124876;     ocdmDnQUWm28124876 = ocdmDnQUWm89729352;     ocdmDnQUWm89729352 = ocdmDnQUWm26066841;     ocdmDnQUWm26066841 = ocdmDnQUWm19726876;     ocdmDnQUWm19726876 = ocdmDnQUWm97503058;     ocdmDnQUWm97503058 = ocdmDnQUWm58209569;     ocdmDnQUWm58209569 = ocdmDnQUWm79306330;     ocdmDnQUWm79306330 = ocdmDnQUWm27038908;     ocdmDnQUWm27038908 = ocdmDnQUWm94207368;     ocdmDnQUWm94207368 = ocdmDnQUWm66908415;     ocdmDnQUWm66908415 = ocdmDnQUWm43543009;     ocdmDnQUWm43543009 = ocdmDnQUWm62593458;     ocdmDnQUWm62593458 = ocdmDnQUWm42538709;     ocdmDnQUWm42538709 = ocdmDnQUWm2959583;     ocdmDnQUWm2959583 = ocdmDnQUWm12968368;     ocdmDnQUWm12968368 = ocdmDnQUWm56369070;     ocdmDnQUWm56369070 = ocdmDnQUWm59299756;     ocdmDnQUWm59299756 = ocdmDnQUWm78289269;     ocdmDnQUWm78289269 = ocdmDnQUWm59327898;     ocdmDnQUWm59327898 = ocdmDnQUWm61458178;     ocdmDnQUWm61458178 = ocdmDnQUWm47601590;     ocdmDnQUWm47601590 = ocdmDnQUWm97276111;     ocdmDnQUWm97276111 = ocdmDnQUWm45335472;     ocdmDnQUWm45335472 = ocdmDnQUWm12405045;     ocdmDnQUWm12405045 = ocdmDnQUWm22532188;     ocdmDnQUWm22532188 = ocdmDnQUWm5540545;     ocdmDnQUWm5540545 = ocdmDnQUWm97937631;     ocdmDnQUWm97937631 = ocdmDnQUWm55337472;     ocdmDnQUWm55337472 = ocdmDnQUWm72884126;     ocdmDnQUWm72884126 = ocdmDnQUWm88717246;     ocdmDnQUWm88717246 = ocdmDnQUWm48810268;     ocdmDnQUWm48810268 = ocdmDnQUWm76628382;     ocdmDnQUWm76628382 = ocdmDnQUWm56873524;     ocdmDnQUWm56873524 = ocdmDnQUWm22048972;     ocdmDnQUWm22048972 = ocdmDnQUWm99982112;     ocdmDnQUWm99982112 = ocdmDnQUWm13098594;     ocdmDnQUWm13098594 = ocdmDnQUWm57763015;     ocdmDnQUWm57763015 = ocdmDnQUWm8266787;     ocdmDnQUWm8266787 = ocdmDnQUWm75935036;     ocdmDnQUWm75935036 = ocdmDnQUWm93014576;     ocdmDnQUWm93014576 = ocdmDnQUWm27805027;     ocdmDnQUWm27805027 = ocdmDnQUWm35565548;     ocdmDnQUWm35565548 = ocdmDnQUWm87064578;     ocdmDnQUWm87064578 = ocdmDnQUWm27008752;     ocdmDnQUWm27008752 = ocdmDnQUWm13904260;     ocdmDnQUWm13904260 = ocdmDnQUWm77742612;     ocdmDnQUWm77742612 = ocdmDnQUWm52728228;     ocdmDnQUWm52728228 = ocdmDnQUWm35997680;     ocdmDnQUWm35997680 = ocdmDnQUWm17824458;     ocdmDnQUWm17824458 = ocdmDnQUWm10659483;     ocdmDnQUWm10659483 = ocdmDnQUWm72749379;     ocdmDnQUWm72749379 = ocdmDnQUWm65436628;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void rozheVvcZt31749437() {     int xSNQBuCYCi87823745 = -947751704;    int xSNQBuCYCi51299086 = -282067624;    int xSNQBuCYCi43743030 = -47275065;    int xSNQBuCYCi80528482 = -368696674;    int xSNQBuCYCi38262219 = -151720597;    int xSNQBuCYCi4206349 = -372275692;    int xSNQBuCYCi62318724 = -671707546;    int xSNQBuCYCi6700429 = -401455072;    int xSNQBuCYCi12959445 = -972608393;    int xSNQBuCYCi31962510 = -79750022;    int xSNQBuCYCi24835279 = -732966961;    int xSNQBuCYCi7630313 = -655042696;    int xSNQBuCYCi77560224 = -458325974;    int xSNQBuCYCi11184288 = -228629138;    int xSNQBuCYCi46941323 = -991416820;    int xSNQBuCYCi22803972 = -218748267;    int xSNQBuCYCi89075990 = 48107299;    int xSNQBuCYCi75828729 = -318990522;    int xSNQBuCYCi4149890 = -36534609;    int xSNQBuCYCi79323765 = -167967853;    int xSNQBuCYCi37778001 = -430124637;    int xSNQBuCYCi49945516 = -378869743;    int xSNQBuCYCi43206002 = -446634062;    int xSNQBuCYCi47266170 = -93808198;    int xSNQBuCYCi34153626 = -489162661;    int xSNQBuCYCi84847950 = -231567365;    int xSNQBuCYCi73819193 = -555992570;    int xSNQBuCYCi33535726 = -804945817;    int xSNQBuCYCi34999549 = -851571797;    int xSNQBuCYCi11985126 = -339360377;    int xSNQBuCYCi28120661 = -399976684;    int xSNQBuCYCi85534853 = -139428199;    int xSNQBuCYCi54476764 = -844872420;    int xSNQBuCYCi10612834 = -48480314;    int xSNQBuCYCi83340722 = -213495737;    int xSNQBuCYCi4297325 = -38958602;    int xSNQBuCYCi74711659 = -199717964;    int xSNQBuCYCi47543268 = -162797646;    int xSNQBuCYCi5755400 = -640579201;    int xSNQBuCYCi60521786 = -380472189;    int xSNQBuCYCi24024308 = 81935738;    int xSNQBuCYCi27465654 = -636115615;    int xSNQBuCYCi36343018 = 36167162;    int xSNQBuCYCi66472671 = -637194615;    int xSNQBuCYCi32403 = -766945622;    int xSNQBuCYCi57903393 = -286710667;    int xSNQBuCYCi32213971 = -681091235;    int xSNQBuCYCi14454924 = -658518341;    int xSNQBuCYCi35210 = -432830740;    int xSNQBuCYCi25208621 = -349770797;    int xSNQBuCYCi98598569 = 99547106;    int xSNQBuCYCi99918098 = 32403026;    int xSNQBuCYCi98041279 = -474409710;    int xSNQBuCYCi57789603 = -43093099;    int xSNQBuCYCi93971436 = -901823612;    int xSNQBuCYCi37878230 = -468881961;    int xSNQBuCYCi8093085 = -835433563;    int xSNQBuCYCi96476860 = -953466867;    int xSNQBuCYCi46374857 = -879534014;    int xSNQBuCYCi53414269 = -920153233;    int xSNQBuCYCi30387155 = -816283122;    int xSNQBuCYCi28782998 = -866761730;    int xSNQBuCYCi71700880 = -549883276;    int xSNQBuCYCi974320 = -533248016;    int xSNQBuCYCi3841850 = -679773338;    int xSNQBuCYCi39300425 = -493538763;    int xSNQBuCYCi53153548 = -810170277;    int xSNQBuCYCi66947391 = -309845660;    int xSNQBuCYCi27843566 = 84866599;    int xSNQBuCYCi42643998 = -852458218;    int xSNQBuCYCi48092312 = 80969696;    int xSNQBuCYCi41532723 = -789095055;    int xSNQBuCYCi70073329 = -678411321;    int xSNQBuCYCi43628104 = -656062421;    int xSNQBuCYCi55299457 = -149903592;    int xSNQBuCYCi10312348 = -794009023;    int xSNQBuCYCi13602499 = -315036906;    int xSNQBuCYCi76733330 = -809439447;    int xSNQBuCYCi47233767 = -326862577;    int xSNQBuCYCi76250232 = -102451995;    int xSNQBuCYCi52633979 = -550476131;    int xSNQBuCYCi59364270 = -897474230;    int xSNQBuCYCi33500517 = -272115077;    int xSNQBuCYCi9790928 = -401801000;    int xSNQBuCYCi13386557 = -338907484;    int xSNQBuCYCi28202563 = -332379711;    int xSNQBuCYCi87493574 = -665018490;    int xSNQBuCYCi96687161 = -701779321;    int xSNQBuCYCi16641397 = -146656703;    int xSNQBuCYCi45462493 = -744613776;    int xSNQBuCYCi96204239 = -203525040;    int xSNQBuCYCi78234799 = -246251098;    int xSNQBuCYCi1168412 = -283263633;    int xSNQBuCYCi52341131 = -720425969;    int xSNQBuCYCi30134631 = -564189067;    int xSNQBuCYCi95241309 = -51302533;    int xSNQBuCYCi55764773 = 13767661;    int xSNQBuCYCi35368699 = -430584822;    int xSNQBuCYCi62630822 = -957421278;    int xSNQBuCYCi60731977 = -947751704;     xSNQBuCYCi87823745 = xSNQBuCYCi51299086;     xSNQBuCYCi51299086 = xSNQBuCYCi43743030;     xSNQBuCYCi43743030 = xSNQBuCYCi80528482;     xSNQBuCYCi80528482 = xSNQBuCYCi38262219;     xSNQBuCYCi38262219 = xSNQBuCYCi4206349;     xSNQBuCYCi4206349 = xSNQBuCYCi62318724;     xSNQBuCYCi62318724 = xSNQBuCYCi6700429;     xSNQBuCYCi6700429 = xSNQBuCYCi12959445;     xSNQBuCYCi12959445 = xSNQBuCYCi31962510;     xSNQBuCYCi31962510 = xSNQBuCYCi24835279;     xSNQBuCYCi24835279 = xSNQBuCYCi7630313;     xSNQBuCYCi7630313 = xSNQBuCYCi77560224;     xSNQBuCYCi77560224 = xSNQBuCYCi11184288;     xSNQBuCYCi11184288 = xSNQBuCYCi46941323;     xSNQBuCYCi46941323 = xSNQBuCYCi22803972;     xSNQBuCYCi22803972 = xSNQBuCYCi89075990;     xSNQBuCYCi89075990 = xSNQBuCYCi75828729;     xSNQBuCYCi75828729 = xSNQBuCYCi4149890;     xSNQBuCYCi4149890 = xSNQBuCYCi79323765;     xSNQBuCYCi79323765 = xSNQBuCYCi37778001;     xSNQBuCYCi37778001 = xSNQBuCYCi49945516;     xSNQBuCYCi49945516 = xSNQBuCYCi43206002;     xSNQBuCYCi43206002 = xSNQBuCYCi47266170;     xSNQBuCYCi47266170 = xSNQBuCYCi34153626;     xSNQBuCYCi34153626 = xSNQBuCYCi84847950;     xSNQBuCYCi84847950 = xSNQBuCYCi73819193;     xSNQBuCYCi73819193 = xSNQBuCYCi33535726;     xSNQBuCYCi33535726 = xSNQBuCYCi34999549;     xSNQBuCYCi34999549 = xSNQBuCYCi11985126;     xSNQBuCYCi11985126 = xSNQBuCYCi28120661;     xSNQBuCYCi28120661 = xSNQBuCYCi85534853;     xSNQBuCYCi85534853 = xSNQBuCYCi54476764;     xSNQBuCYCi54476764 = xSNQBuCYCi10612834;     xSNQBuCYCi10612834 = xSNQBuCYCi83340722;     xSNQBuCYCi83340722 = xSNQBuCYCi4297325;     xSNQBuCYCi4297325 = xSNQBuCYCi74711659;     xSNQBuCYCi74711659 = xSNQBuCYCi47543268;     xSNQBuCYCi47543268 = xSNQBuCYCi5755400;     xSNQBuCYCi5755400 = xSNQBuCYCi60521786;     xSNQBuCYCi60521786 = xSNQBuCYCi24024308;     xSNQBuCYCi24024308 = xSNQBuCYCi27465654;     xSNQBuCYCi27465654 = xSNQBuCYCi36343018;     xSNQBuCYCi36343018 = xSNQBuCYCi66472671;     xSNQBuCYCi66472671 = xSNQBuCYCi32403;     xSNQBuCYCi32403 = xSNQBuCYCi57903393;     xSNQBuCYCi57903393 = xSNQBuCYCi32213971;     xSNQBuCYCi32213971 = xSNQBuCYCi14454924;     xSNQBuCYCi14454924 = xSNQBuCYCi35210;     xSNQBuCYCi35210 = xSNQBuCYCi25208621;     xSNQBuCYCi25208621 = xSNQBuCYCi98598569;     xSNQBuCYCi98598569 = xSNQBuCYCi99918098;     xSNQBuCYCi99918098 = xSNQBuCYCi98041279;     xSNQBuCYCi98041279 = xSNQBuCYCi57789603;     xSNQBuCYCi57789603 = xSNQBuCYCi93971436;     xSNQBuCYCi93971436 = xSNQBuCYCi37878230;     xSNQBuCYCi37878230 = xSNQBuCYCi8093085;     xSNQBuCYCi8093085 = xSNQBuCYCi96476860;     xSNQBuCYCi96476860 = xSNQBuCYCi46374857;     xSNQBuCYCi46374857 = xSNQBuCYCi53414269;     xSNQBuCYCi53414269 = xSNQBuCYCi30387155;     xSNQBuCYCi30387155 = xSNQBuCYCi28782998;     xSNQBuCYCi28782998 = xSNQBuCYCi71700880;     xSNQBuCYCi71700880 = xSNQBuCYCi974320;     xSNQBuCYCi974320 = xSNQBuCYCi3841850;     xSNQBuCYCi3841850 = xSNQBuCYCi39300425;     xSNQBuCYCi39300425 = xSNQBuCYCi53153548;     xSNQBuCYCi53153548 = xSNQBuCYCi66947391;     xSNQBuCYCi66947391 = xSNQBuCYCi27843566;     xSNQBuCYCi27843566 = xSNQBuCYCi42643998;     xSNQBuCYCi42643998 = xSNQBuCYCi48092312;     xSNQBuCYCi48092312 = xSNQBuCYCi41532723;     xSNQBuCYCi41532723 = xSNQBuCYCi70073329;     xSNQBuCYCi70073329 = xSNQBuCYCi43628104;     xSNQBuCYCi43628104 = xSNQBuCYCi55299457;     xSNQBuCYCi55299457 = xSNQBuCYCi10312348;     xSNQBuCYCi10312348 = xSNQBuCYCi13602499;     xSNQBuCYCi13602499 = xSNQBuCYCi76733330;     xSNQBuCYCi76733330 = xSNQBuCYCi47233767;     xSNQBuCYCi47233767 = xSNQBuCYCi76250232;     xSNQBuCYCi76250232 = xSNQBuCYCi52633979;     xSNQBuCYCi52633979 = xSNQBuCYCi59364270;     xSNQBuCYCi59364270 = xSNQBuCYCi33500517;     xSNQBuCYCi33500517 = xSNQBuCYCi9790928;     xSNQBuCYCi9790928 = xSNQBuCYCi13386557;     xSNQBuCYCi13386557 = xSNQBuCYCi28202563;     xSNQBuCYCi28202563 = xSNQBuCYCi87493574;     xSNQBuCYCi87493574 = xSNQBuCYCi96687161;     xSNQBuCYCi96687161 = xSNQBuCYCi16641397;     xSNQBuCYCi16641397 = xSNQBuCYCi45462493;     xSNQBuCYCi45462493 = xSNQBuCYCi96204239;     xSNQBuCYCi96204239 = xSNQBuCYCi78234799;     xSNQBuCYCi78234799 = xSNQBuCYCi1168412;     xSNQBuCYCi1168412 = xSNQBuCYCi52341131;     xSNQBuCYCi52341131 = xSNQBuCYCi30134631;     xSNQBuCYCi30134631 = xSNQBuCYCi95241309;     xSNQBuCYCi95241309 = xSNQBuCYCi55764773;     xSNQBuCYCi55764773 = xSNQBuCYCi35368699;     xSNQBuCYCi35368699 = xSNQBuCYCi62630822;     xSNQBuCYCi62630822 = xSNQBuCYCi60731977;     xSNQBuCYCi60731977 = xSNQBuCYCi87823745;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void zZCbYybKVb68394412() {     int tAKvhLyZUp10210863 = -582155667;    int tAKvhLyZUp35646587 = -230610679;    int tAKvhLyZUp72512309 = -286928909;    int tAKvhLyZUp95535170 = -375104206;    int tAKvhLyZUp36436473 = -742777599;    int tAKvhLyZUp19089466 = -232884070;    int tAKvhLyZUp54474463 = -493694853;    int tAKvhLyZUp82334287 = -872051717;    int tAKvhLyZUp10122576 = -856804432;    int tAKvhLyZUp23726095 = -922491355;    int tAKvhLyZUp62377138 = -729591362;    int tAKvhLyZUp63533261 = -905787990;    int tAKvhLyZUp44568405 = 22956706;    int tAKvhLyZUp53799040 = -756337895;    int tAKvhLyZUp83676599 = -618894533;    int tAKvhLyZUp67059884 = -600415809;    int tAKvhLyZUp38899343 = -229172521;    int tAKvhLyZUp81805145 = -770733213;    int tAKvhLyZUp45431520 = -171636734;    int tAKvhLyZUp57840048 = -420602307;    int tAKvhLyZUp38661108 = -421611945;    int tAKvhLyZUp13760735 = -883636057;    int tAKvhLyZUp46499325 = -622714654;    int tAKvhLyZUp73765957 = -657801056;    int tAKvhLyZUp69693872 = -110721785;    int tAKvhLyZUp73150945 = -675461589;    int tAKvhLyZUp20908614 = -313579063;    int tAKvhLyZUp39447176 = -593400271;    int tAKvhLyZUp41892109 = -95828916;    int tAKvhLyZUp21142305 = -259656243;    int tAKvhLyZUp72411466 = -481292836;    int tAKvhLyZUp43076044 = -791499554;    int tAKvhLyZUp35515435 = -274215915;    int tAKvhLyZUp70001520 = -372607164;    int tAKvhLyZUp59570086 = -277252160;    int tAKvhLyZUp45990194 = -354353979;    int tAKvhLyZUp68151372 = 67334085;    int tAKvhLyZUp1169369 = -30423118;    int tAKvhLyZUp54063531 = 58374767;    int tAKvhLyZUp80707500 = -516696218;    int tAKvhLyZUp52781679 = -421241928;    int tAKvhLyZUp15974045 = -394024306;    int tAKvhLyZUp41893210 = -951363421;    int tAKvhLyZUp65916791 = -634558264;    int tAKvhLyZUp68015669 = -576089100;    int tAKvhLyZUp66003675 = -468939762;    int tAKvhLyZUp44511368 = -159501488;    int tAKvhLyZUp59053598 = -415053331;    int tAKvhLyZUp94495114 = -382547438;    int tAKvhLyZUp22292367 = -491791056;    int tAKvhLyZUp7467786 = -255529171;    int tAKvhLyZUp73769355 = -398416256;    int tAKvhLyZUp76355682 = -146320940;    int tAKvhLyZUp18076149 = -318773908;    int tAKvhLyZUp29733304 = 57472273;    int tAKvhLyZUp96450128 = -698519611;    int tAKvhLyZUp89147262 = -607896025;    int tAKvhLyZUp98746351 = -629127854;    int tAKvhLyZUp25841298 = -164382421;    int tAKvhLyZUp63285528 = 32683990;    int tAKvhLyZUp98180851 = -919305007;    int tAKvhLyZUp15027287 = -900294583;    int tAKvhLyZUp40442178 = -676222801;    int tAKvhLyZUp88980271 = -497148189;    int tAKvhLyZUp51314629 = -341198519;    int tAKvhLyZUp19301094 = -938091809;    int tAKvhLyZUp28017827 = -531572075;    int tAKvhLyZUp74566884 = -604436130;    int tAKvhLyZUp94228953 = -379085736;    int tAKvhLyZUp37686406 = -164540555;    int tAKvhLyZUp98908511 = -567749895;    int tAKvhLyZUp37729974 = -98749403;    int tAKvhLyZUp27741615 = -729107980;    int tAKvhLyZUp64724020 = -654940517;    int tAKvhLyZUp5058370 = -999360379;    int tAKvhLyZUp22687064 = 72412360;    int tAKvhLyZUp71867525 = -932272636;    int tAKvhLyZUp80582534 = -988156390;    int tAKvhLyZUp5750288 = 18288044;    int tAKvhLyZUp3690198 = -641782024;    int tAKvhLyZUp28639577 = -415960102;    int tAKvhLyZUp61855016 = -898525732;    int tAKvhLyZUp44952062 = -110852833;    int tAKvhLyZUp19599743 = -604037860;    int tAKvhLyZUp13674520 = 95872927;    int tAKvhLyZUp98642110 = 17123419;    int tAKvhLyZUp66720362 = -545178615;    int tAKvhLyZUp17439287 = -955442007;    int tAKvhLyZUp40268217 = -330079438;    int tAKvhLyZUp63119958 = -578732549;    int tAKvhLyZUp56842932 = -746457954;    int tAKvhLyZUp69405020 = -303538062;    int tAKvhLyZUp75328070 = -866040698;    int tAKvhLyZUp90778002 = -974309223;    int tAKvhLyZUp82526648 = -597391211;    int tAKvhLyZUp37754392 = -520947346;    int tAKvhLyZUp75531866 = -717801505;    int tAKvhLyZUp52912939 = -354215233;    int tAKvhLyZUp14602163 = -193359746;    int tAKvhLyZUp48714575 = -582155667;     tAKvhLyZUp10210863 = tAKvhLyZUp35646587;     tAKvhLyZUp35646587 = tAKvhLyZUp72512309;     tAKvhLyZUp72512309 = tAKvhLyZUp95535170;     tAKvhLyZUp95535170 = tAKvhLyZUp36436473;     tAKvhLyZUp36436473 = tAKvhLyZUp19089466;     tAKvhLyZUp19089466 = tAKvhLyZUp54474463;     tAKvhLyZUp54474463 = tAKvhLyZUp82334287;     tAKvhLyZUp82334287 = tAKvhLyZUp10122576;     tAKvhLyZUp10122576 = tAKvhLyZUp23726095;     tAKvhLyZUp23726095 = tAKvhLyZUp62377138;     tAKvhLyZUp62377138 = tAKvhLyZUp63533261;     tAKvhLyZUp63533261 = tAKvhLyZUp44568405;     tAKvhLyZUp44568405 = tAKvhLyZUp53799040;     tAKvhLyZUp53799040 = tAKvhLyZUp83676599;     tAKvhLyZUp83676599 = tAKvhLyZUp67059884;     tAKvhLyZUp67059884 = tAKvhLyZUp38899343;     tAKvhLyZUp38899343 = tAKvhLyZUp81805145;     tAKvhLyZUp81805145 = tAKvhLyZUp45431520;     tAKvhLyZUp45431520 = tAKvhLyZUp57840048;     tAKvhLyZUp57840048 = tAKvhLyZUp38661108;     tAKvhLyZUp38661108 = tAKvhLyZUp13760735;     tAKvhLyZUp13760735 = tAKvhLyZUp46499325;     tAKvhLyZUp46499325 = tAKvhLyZUp73765957;     tAKvhLyZUp73765957 = tAKvhLyZUp69693872;     tAKvhLyZUp69693872 = tAKvhLyZUp73150945;     tAKvhLyZUp73150945 = tAKvhLyZUp20908614;     tAKvhLyZUp20908614 = tAKvhLyZUp39447176;     tAKvhLyZUp39447176 = tAKvhLyZUp41892109;     tAKvhLyZUp41892109 = tAKvhLyZUp21142305;     tAKvhLyZUp21142305 = tAKvhLyZUp72411466;     tAKvhLyZUp72411466 = tAKvhLyZUp43076044;     tAKvhLyZUp43076044 = tAKvhLyZUp35515435;     tAKvhLyZUp35515435 = tAKvhLyZUp70001520;     tAKvhLyZUp70001520 = tAKvhLyZUp59570086;     tAKvhLyZUp59570086 = tAKvhLyZUp45990194;     tAKvhLyZUp45990194 = tAKvhLyZUp68151372;     tAKvhLyZUp68151372 = tAKvhLyZUp1169369;     tAKvhLyZUp1169369 = tAKvhLyZUp54063531;     tAKvhLyZUp54063531 = tAKvhLyZUp80707500;     tAKvhLyZUp80707500 = tAKvhLyZUp52781679;     tAKvhLyZUp52781679 = tAKvhLyZUp15974045;     tAKvhLyZUp15974045 = tAKvhLyZUp41893210;     tAKvhLyZUp41893210 = tAKvhLyZUp65916791;     tAKvhLyZUp65916791 = tAKvhLyZUp68015669;     tAKvhLyZUp68015669 = tAKvhLyZUp66003675;     tAKvhLyZUp66003675 = tAKvhLyZUp44511368;     tAKvhLyZUp44511368 = tAKvhLyZUp59053598;     tAKvhLyZUp59053598 = tAKvhLyZUp94495114;     tAKvhLyZUp94495114 = tAKvhLyZUp22292367;     tAKvhLyZUp22292367 = tAKvhLyZUp7467786;     tAKvhLyZUp7467786 = tAKvhLyZUp73769355;     tAKvhLyZUp73769355 = tAKvhLyZUp76355682;     tAKvhLyZUp76355682 = tAKvhLyZUp18076149;     tAKvhLyZUp18076149 = tAKvhLyZUp29733304;     tAKvhLyZUp29733304 = tAKvhLyZUp96450128;     tAKvhLyZUp96450128 = tAKvhLyZUp89147262;     tAKvhLyZUp89147262 = tAKvhLyZUp98746351;     tAKvhLyZUp98746351 = tAKvhLyZUp25841298;     tAKvhLyZUp25841298 = tAKvhLyZUp63285528;     tAKvhLyZUp63285528 = tAKvhLyZUp98180851;     tAKvhLyZUp98180851 = tAKvhLyZUp15027287;     tAKvhLyZUp15027287 = tAKvhLyZUp40442178;     tAKvhLyZUp40442178 = tAKvhLyZUp88980271;     tAKvhLyZUp88980271 = tAKvhLyZUp51314629;     tAKvhLyZUp51314629 = tAKvhLyZUp19301094;     tAKvhLyZUp19301094 = tAKvhLyZUp28017827;     tAKvhLyZUp28017827 = tAKvhLyZUp74566884;     tAKvhLyZUp74566884 = tAKvhLyZUp94228953;     tAKvhLyZUp94228953 = tAKvhLyZUp37686406;     tAKvhLyZUp37686406 = tAKvhLyZUp98908511;     tAKvhLyZUp98908511 = tAKvhLyZUp37729974;     tAKvhLyZUp37729974 = tAKvhLyZUp27741615;     tAKvhLyZUp27741615 = tAKvhLyZUp64724020;     tAKvhLyZUp64724020 = tAKvhLyZUp5058370;     tAKvhLyZUp5058370 = tAKvhLyZUp22687064;     tAKvhLyZUp22687064 = tAKvhLyZUp71867525;     tAKvhLyZUp71867525 = tAKvhLyZUp80582534;     tAKvhLyZUp80582534 = tAKvhLyZUp5750288;     tAKvhLyZUp5750288 = tAKvhLyZUp3690198;     tAKvhLyZUp3690198 = tAKvhLyZUp28639577;     tAKvhLyZUp28639577 = tAKvhLyZUp61855016;     tAKvhLyZUp61855016 = tAKvhLyZUp44952062;     tAKvhLyZUp44952062 = tAKvhLyZUp19599743;     tAKvhLyZUp19599743 = tAKvhLyZUp13674520;     tAKvhLyZUp13674520 = tAKvhLyZUp98642110;     tAKvhLyZUp98642110 = tAKvhLyZUp66720362;     tAKvhLyZUp66720362 = tAKvhLyZUp17439287;     tAKvhLyZUp17439287 = tAKvhLyZUp40268217;     tAKvhLyZUp40268217 = tAKvhLyZUp63119958;     tAKvhLyZUp63119958 = tAKvhLyZUp56842932;     tAKvhLyZUp56842932 = tAKvhLyZUp69405020;     tAKvhLyZUp69405020 = tAKvhLyZUp75328070;     tAKvhLyZUp75328070 = tAKvhLyZUp90778002;     tAKvhLyZUp90778002 = tAKvhLyZUp82526648;     tAKvhLyZUp82526648 = tAKvhLyZUp37754392;     tAKvhLyZUp37754392 = tAKvhLyZUp75531866;     tAKvhLyZUp75531866 = tAKvhLyZUp52912939;     tAKvhLyZUp52912939 = tAKvhLyZUp14602163;     tAKvhLyZUp14602163 = tAKvhLyZUp48714575;     tAKvhLyZUp48714575 = tAKvhLyZUp10210863;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void TtHiikiadJ5039387() {     int xFcUdZqlno32597981 = -216559630;    int xFcUdZqlno19994087 = -179153734;    int xFcUdZqlno1281588 = -526582753;    int xFcUdZqlno10541860 = -381511738;    int xFcUdZqlno34610727 = -233834600;    int xFcUdZqlno33972583 = -93492448;    int xFcUdZqlno46630202 = -315682159;    int xFcUdZqlno57968146 = -242648361;    int xFcUdZqlno7285708 = -741000471;    int xFcUdZqlno15489680 = -665232688;    int xFcUdZqlno99918997 = -726215763;    int xFcUdZqlno19436211 = -56533283;    int xFcUdZqlno11576586 = -595760613;    int xFcUdZqlno96413791 = -184046652;    int xFcUdZqlno20411877 = -246372246;    int xFcUdZqlno11315797 = -982083352;    int xFcUdZqlno88722694 = -506452341;    int xFcUdZqlno87781561 = -122475903;    int xFcUdZqlno86713151 = -306738859;    int xFcUdZqlno36356332 = -673236760;    int xFcUdZqlno39544215 = -413099253;    int xFcUdZqlno77575953 = -288402370;    int xFcUdZqlno49792648 = -798795246;    int xFcUdZqlno265745 = -121793913;    int xFcUdZqlno5234120 = -832280910;    int xFcUdZqlno61453939 = -19355813;    int xFcUdZqlno67998034 = -71165556;    int xFcUdZqlno45358626 = -381854724;    int xFcUdZqlno48784670 = -440086035;    int xFcUdZqlno30299485 = -179952110;    int xFcUdZqlno16702272 = -562608988;    int xFcUdZqlno617235 = -343570908;    int xFcUdZqlno16554106 = -803559409;    int xFcUdZqlno29390208 = -696734015;    int xFcUdZqlno35799451 = -341008582;    int xFcUdZqlno87683063 = -669749355;    int xFcUdZqlno61591084 = -765613867;    int xFcUdZqlno54795469 = -998048590;    int xFcUdZqlno2371663 = -342671265;    int xFcUdZqlno893215 = -652920247;    int xFcUdZqlno81539049 = -924419594;    int xFcUdZqlno4482436 = -151932997;    int xFcUdZqlno47443402 = -838894005;    int xFcUdZqlno65360910 = -631921913;    int xFcUdZqlno35998936 = -385232578;    int xFcUdZqlno74103957 = -651168858;    int xFcUdZqlno56808765 = -737911740;    int xFcUdZqlno3652273 = -171588322;    int xFcUdZqlno88955019 = -332264137;    int xFcUdZqlno19376113 = -633811315;    int xFcUdZqlno16337003 = -610605448;    int xFcUdZqlno47620612 = -829235538;    int xFcUdZqlno54670085 = -918232170;    int xFcUdZqlno78362694 = -594454717;    int xFcUdZqlno65495171 = -83231843;    int xFcUdZqlno55022027 = -928157261;    int xFcUdZqlno70201439 = -380358488;    int xFcUdZqlno1015844 = -304788841;    int xFcUdZqlno5307740 = -549230828;    int xFcUdZqlno73156788 = -114478788;    int xFcUdZqlno65974548 = 77673108;    int xFcUdZqlno1271576 = -933827435;    int xFcUdZqlno9183476 = -802562327;    int xFcUdZqlno76986222 = -461048361;    int xFcUdZqlno98787408 = -2623700;    int xFcUdZqlno99301762 = -282644855;    int xFcUdZqlno2882106 = -252973874;    int xFcUdZqlno82186377 = -899026599;    int xFcUdZqlno60614341 = -843038070;    int xFcUdZqlno32728814 = -576622892;    int xFcUdZqlno49724712 = -116469486;    int xFcUdZqlno33927225 = -508403751;    int xFcUdZqlno85409899 = -779804639;    int xFcUdZqlno85819936 = -653818613;    int xFcUdZqlno54817282 = -748817167;    int xFcUdZqlno35061779 = -161166257;    int xFcUdZqlno30132551 = -449508365;    int xFcUdZqlno84431737 = -66873333;    int xFcUdZqlno64266808 = -736561336;    int xFcUdZqlno31130163 = -81112053;    int xFcUdZqlno4645174 = -281444073;    int xFcUdZqlno64345762 = -899577234;    int xFcUdZqlno56403607 = 50409412;    int xFcUdZqlno29408558 = -806274720;    int xFcUdZqlno13962483 = -569346663;    int xFcUdZqlno69081659 = -733373451;    int xFcUdZqlno45947149 = -425338739;    int xFcUdZqlno38191412 = -109104693;    int xFcUdZqlno63895036 = -513502173;    int xFcUdZqlno80777423 = -412851322;    int xFcUdZqlno17481624 = -189390868;    int xFcUdZqlno60575241 = -360825027;    int xFcUdZqlno49487730 = -348817763;    int xFcUdZqlno29214875 = -128192477;    int xFcUdZqlno34918667 = -630593355;    int xFcUdZqlno80267474 = -990592160;    int xFcUdZqlno95298960 = -349370671;    int xFcUdZqlno70457179 = -277845644;    int xFcUdZqlno66573502 = -529298214;    int xFcUdZqlno36697173 = -216559630;     xFcUdZqlno32597981 = xFcUdZqlno19994087;     xFcUdZqlno19994087 = xFcUdZqlno1281588;     xFcUdZqlno1281588 = xFcUdZqlno10541860;     xFcUdZqlno10541860 = xFcUdZqlno34610727;     xFcUdZqlno34610727 = xFcUdZqlno33972583;     xFcUdZqlno33972583 = xFcUdZqlno46630202;     xFcUdZqlno46630202 = xFcUdZqlno57968146;     xFcUdZqlno57968146 = xFcUdZqlno7285708;     xFcUdZqlno7285708 = xFcUdZqlno15489680;     xFcUdZqlno15489680 = xFcUdZqlno99918997;     xFcUdZqlno99918997 = xFcUdZqlno19436211;     xFcUdZqlno19436211 = xFcUdZqlno11576586;     xFcUdZqlno11576586 = xFcUdZqlno96413791;     xFcUdZqlno96413791 = xFcUdZqlno20411877;     xFcUdZqlno20411877 = xFcUdZqlno11315797;     xFcUdZqlno11315797 = xFcUdZqlno88722694;     xFcUdZqlno88722694 = xFcUdZqlno87781561;     xFcUdZqlno87781561 = xFcUdZqlno86713151;     xFcUdZqlno86713151 = xFcUdZqlno36356332;     xFcUdZqlno36356332 = xFcUdZqlno39544215;     xFcUdZqlno39544215 = xFcUdZqlno77575953;     xFcUdZqlno77575953 = xFcUdZqlno49792648;     xFcUdZqlno49792648 = xFcUdZqlno265745;     xFcUdZqlno265745 = xFcUdZqlno5234120;     xFcUdZqlno5234120 = xFcUdZqlno61453939;     xFcUdZqlno61453939 = xFcUdZqlno67998034;     xFcUdZqlno67998034 = xFcUdZqlno45358626;     xFcUdZqlno45358626 = xFcUdZqlno48784670;     xFcUdZqlno48784670 = xFcUdZqlno30299485;     xFcUdZqlno30299485 = xFcUdZqlno16702272;     xFcUdZqlno16702272 = xFcUdZqlno617235;     xFcUdZqlno617235 = xFcUdZqlno16554106;     xFcUdZqlno16554106 = xFcUdZqlno29390208;     xFcUdZqlno29390208 = xFcUdZqlno35799451;     xFcUdZqlno35799451 = xFcUdZqlno87683063;     xFcUdZqlno87683063 = xFcUdZqlno61591084;     xFcUdZqlno61591084 = xFcUdZqlno54795469;     xFcUdZqlno54795469 = xFcUdZqlno2371663;     xFcUdZqlno2371663 = xFcUdZqlno893215;     xFcUdZqlno893215 = xFcUdZqlno81539049;     xFcUdZqlno81539049 = xFcUdZqlno4482436;     xFcUdZqlno4482436 = xFcUdZqlno47443402;     xFcUdZqlno47443402 = xFcUdZqlno65360910;     xFcUdZqlno65360910 = xFcUdZqlno35998936;     xFcUdZqlno35998936 = xFcUdZqlno74103957;     xFcUdZqlno74103957 = xFcUdZqlno56808765;     xFcUdZqlno56808765 = xFcUdZqlno3652273;     xFcUdZqlno3652273 = xFcUdZqlno88955019;     xFcUdZqlno88955019 = xFcUdZqlno19376113;     xFcUdZqlno19376113 = xFcUdZqlno16337003;     xFcUdZqlno16337003 = xFcUdZqlno47620612;     xFcUdZqlno47620612 = xFcUdZqlno54670085;     xFcUdZqlno54670085 = xFcUdZqlno78362694;     xFcUdZqlno78362694 = xFcUdZqlno65495171;     xFcUdZqlno65495171 = xFcUdZqlno55022027;     xFcUdZqlno55022027 = xFcUdZqlno70201439;     xFcUdZqlno70201439 = xFcUdZqlno1015844;     xFcUdZqlno1015844 = xFcUdZqlno5307740;     xFcUdZqlno5307740 = xFcUdZqlno73156788;     xFcUdZqlno73156788 = xFcUdZqlno65974548;     xFcUdZqlno65974548 = xFcUdZqlno1271576;     xFcUdZqlno1271576 = xFcUdZqlno9183476;     xFcUdZqlno9183476 = xFcUdZqlno76986222;     xFcUdZqlno76986222 = xFcUdZqlno98787408;     xFcUdZqlno98787408 = xFcUdZqlno99301762;     xFcUdZqlno99301762 = xFcUdZqlno2882106;     xFcUdZqlno2882106 = xFcUdZqlno82186377;     xFcUdZqlno82186377 = xFcUdZqlno60614341;     xFcUdZqlno60614341 = xFcUdZqlno32728814;     xFcUdZqlno32728814 = xFcUdZqlno49724712;     xFcUdZqlno49724712 = xFcUdZqlno33927225;     xFcUdZqlno33927225 = xFcUdZqlno85409899;     xFcUdZqlno85409899 = xFcUdZqlno85819936;     xFcUdZqlno85819936 = xFcUdZqlno54817282;     xFcUdZqlno54817282 = xFcUdZqlno35061779;     xFcUdZqlno35061779 = xFcUdZqlno30132551;     xFcUdZqlno30132551 = xFcUdZqlno84431737;     xFcUdZqlno84431737 = xFcUdZqlno64266808;     xFcUdZqlno64266808 = xFcUdZqlno31130163;     xFcUdZqlno31130163 = xFcUdZqlno4645174;     xFcUdZqlno4645174 = xFcUdZqlno64345762;     xFcUdZqlno64345762 = xFcUdZqlno56403607;     xFcUdZqlno56403607 = xFcUdZqlno29408558;     xFcUdZqlno29408558 = xFcUdZqlno13962483;     xFcUdZqlno13962483 = xFcUdZqlno69081659;     xFcUdZqlno69081659 = xFcUdZqlno45947149;     xFcUdZqlno45947149 = xFcUdZqlno38191412;     xFcUdZqlno38191412 = xFcUdZqlno63895036;     xFcUdZqlno63895036 = xFcUdZqlno80777423;     xFcUdZqlno80777423 = xFcUdZqlno17481624;     xFcUdZqlno17481624 = xFcUdZqlno60575241;     xFcUdZqlno60575241 = xFcUdZqlno49487730;     xFcUdZqlno49487730 = xFcUdZqlno29214875;     xFcUdZqlno29214875 = xFcUdZqlno34918667;     xFcUdZqlno34918667 = xFcUdZqlno80267474;     xFcUdZqlno80267474 = xFcUdZqlno95298960;     xFcUdZqlno95298960 = xFcUdZqlno70457179;     xFcUdZqlno70457179 = xFcUdZqlno66573502;     xFcUdZqlno66573502 = xFcUdZqlno36697173;     xFcUdZqlno36697173 = xFcUdZqlno32597981;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void qjJWCijUtG41684361() {     int ngsUzlHDqw54985098 = -950963594;    int ngsUzlHDqw4341588 = -127696788;    int ngsUzlHDqw30050867 = -766236597;    int ngsUzlHDqw25548548 = -387919269;    int ngsUzlHDqw32784982 = -824891601;    int ngsUzlHDqw48855699 = 45899174;    int ngsUzlHDqw38785941 = -137669465;    int ngsUzlHDqw33602005 = -713245005;    int ngsUzlHDqw4448839 = -625196510;    int ngsUzlHDqw7253264 = -407974021;    int ngsUzlHDqw37460857 = -722840164;    int ngsUzlHDqw75339159 = -307278576;    int ngsUzlHDqw78584766 = -114477933;    int ngsUzlHDqw39028544 = -711755409;    int ngsUzlHDqw57147154 = -973849960;    int ngsUzlHDqw55571709 = -263750894;    int ngsUzlHDqw38546046 = -783732161;    int ngsUzlHDqw93757978 = -574218594;    int ngsUzlHDqw27994782 = -441840984;    int ngsUzlHDqw14872615 = -925871214;    int ngsUzlHDqw40427322 = -404586562;    int ngsUzlHDqw41391172 = -793168683;    int ngsUzlHDqw53085971 = -974875838;    int ngsUzlHDqw26765532 = -685786770;    int ngsUzlHDqw40774366 = -453840034;    int ngsUzlHDqw49756934 = -463250037;    int ngsUzlHDqw15087455 = -928752049;    int ngsUzlHDqw51270076 = -170309178;    int ngsUzlHDqw55677231 = -784343154;    int ngsUzlHDqw39456664 = -100247976;    int ngsUzlHDqw60993076 = -643925140;    int ngsUzlHDqw58158426 = -995642263;    int ngsUzlHDqw97592775 = -232902904;    int ngsUzlHDqw88778895 = 79139135;    int ngsUzlHDqw12028816 = -404765005;    int ngsUzlHDqw29375932 = -985144732;    int ngsUzlHDqw55030796 = -498561818;    int ngsUzlHDqw8421571 = -865674062;    int ngsUzlHDqw50679793 = -743717296;    int ngsUzlHDqw21078930 = -789144276;    int ngsUzlHDqw10296421 = -327597260;    int ngsUzlHDqw92990827 = 90158312;    int ngsUzlHDqw52993594 = -726424589;    int ngsUzlHDqw64805029 = -629285562;    int ngsUzlHDqw3982203 = -194376055;    int ngsUzlHDqw82204238 = -833397953;    int ngsUzlHDqw69106163 = -216321993;    int ngsUzlHDqw48250947 = 71876687;    int ngsUzlHDqw83414924 = -281980835;    int ngsUzlHDqw16459858 = -775831574;    int ngsUzlHDqw25206219 = -965681725;    int ngsUzlHDqw21471870 = -160054820;    int ngsUzlHDqw32984489 = -590143400;    int ngsUzlHDqw38649239 = -870135526;    int ngsUzlHDqw1257039 = -223935958;    int ngsUzlHDqw13593927 = -57794911;    int ngsUzlHDqw51255617 = -152820950;    int ngsUzlHDqw3285335 = 19550173;    int ngsUzlHDqw84774181 = -934079235;    int ngsUzlHDqw83028047 = -261641565;    int ngsUzlHDqw33768245 = -25348777;    int ngsUzlHDqw87515864 = -967360288;    int ngsUzlHDqw77924773 = -928901852;    int ngsUzlHDqw64992174 = -424948534;    int ngsUzlHDqw46260188 = -764048881;    int ngsUzlHDqw79302431 = -727197901;    int ngsUzlHDqw77746383 = 25624328;    int ngsUzlHDqw89805871 = -93617069;    int ngsUzlHDqw26999729 = -206990405;    int ngsUzlHDqw27771222 = -988705228;    int ngsUzlHDqw540913 = -765189077;    int ngsUzlHDqw30124476 = -918058099;    int ngsUzlHDqw43078185 = -830501298;    int ngsUzlHDqw6915853 = -652696708;    int ngsUzlHDqw4576195 = -498273954;    int ngsUzlHDqw47436495 = -394744875;    int ngsUzlHDqw88397577 = 33255905;    int ngsUzlHDqw88280941 = -245590277;    int ngsUzlHDqw22783329 = -391410715;    int ngsUzlHDqw58570127 = -620442082;    int ngsUzlHDqw80650771 = -146928044;    int ngsUzlHDqw66836508 = -900628736;    int ngsUzlHDqw67855152 = -888328343;    int ngsUzlHDqw39217373 = 91488419;    int ngsUzlHDqw14250446 = -134566252;    int ngsUzlHDqw39521207 = -383870321;    int ngsUzlHDqw25173937 = -305498864;    int ngsUzlHDqw58943536 = -362767379;    int ngsUzlHDqw87521856 = -696924907;    int ngsUzlHDqw98434889 = -246970094;    int ngsUzlHDqw78120315 = -732323782;    int ngsUzlHDqw51745461 = -418111991;    int ngsUzlHDqw23647389 = -931594827;    int ngsUzlHDqw67651746 = -382075732;    int ngsUzlHDqw87310684 = -663795499;    int ngsUzlHDqw22780556 = -360236973;    int ngsUzlHDqw15066054 = 19060164;    int ngsUzlHDqw88001419 = -201476055;    int ngsUzlHDqw18544842 = -865236682;    int ngsUzlHDqw24679771 = -950963594;     ngsUzlHDqw54985098 = ngsUzlHDqw4341588;     ngsUzlHDqw4341588 = ngsUzlHDqw30050867;     ngsUzlHDqw30050867 = ngsUzlHDqw25548548;     ngsUzlHDqw25548548 = ngsUzlHDqw32784982;     ngsUzlHDqw32784982 = ngsUzlHDqw48855699;     ngsUzlHDqw48855699 = ngsUzlHDqw38785941;     ngsUzlHDqw38785941 = ngsUzlHDqw33602005;     ngsUzlHDqw33602005 = ngsUzlHDqw4448839;     ngsUzlHDqw4448839 = ngsUzlHDqw7253264;     ngsUzlHDqw7253264 = ngsUzlHDqw37460857;     ngsUzlHDqw37460857 = ngsUzlHDqw75339159;     ngsUzlHDqw75339159 = ngsUzlHDqw78584766;     ngsUzlHDqw78584766 = ngsUzlHDqw39028544;     ngsUzlHDqw39028544 = ngsUzlHDqw57147154;     ngsUzlHDqw57147154 = ngsUzlHDqw55571709;     ngsUzlHDqw55571709 = ngsUzlHDqw38546046;     ngsUzlHDqw38546046 = ngsUzlHDqw93757978;     ngsUzlHDqw93757978 = ngsUzlHDqw27994782;     ngsUzlHDqw27994782 = ngsUzlHDqw14872615;     ngsUzlHDqw14872615 = ngsUzlHDqw40427322;     ngsUzlHDqw40427322 = ngsUzlHDqw41391172;     ngsUzlHDqw41391172 = ngsUzlHDqw53085971;     ngsUzlHDqw53085971 = ngsUzlHDqw26765532;     ngsUzlHDqw26765532 = ngsUzlHDqw40774366;     ngsUzlHDqw40774366 = ngsUzlHDqw49756934;     ngsUzlHDqw49756934 = ngsUzlHDqw15087455;     ngsUzlHDqw15087455 = ngsUzlHDqw51270076;     ngsUzlHDqw51270076 = ngsUzlHDqw55677231;     ngsUzlHDqw55677231 = ngsUzlHDqw39456664;     ngsUzlHDqw39456664 = ngsUzlHDqw60993076;     ngsUzlHDqw60993076 = ngsUzlHDqw58158426;     ngsUzlHDqw58158426 = ngsUzlHDqw97592775;     ngsUzlHDqw97592775 = ngsUzlHDqw88778895;     ngsUzlHDqw88778895 = ngsUzlHDqw12028816;     ngsUzlHDqw12028816 = ngsUzlHDqw29375932;     ngsUzlHDqw29375932 = ngsUzlHDqw55030796;     ngsUzlHDqw55030796 = ngsUzlHDqw8421571;     ngsUzlHDqw8421571 = ngsUzlHDqw50679793;     ngsUzlHDqw50679793 = ngsUzlHDqw21078930;     ngsUzlHDqw21078930 = ngsUzlHDqw10296421;     ngsUzlHDqw10296421 = ngsUzlHDqw92990827;     ngsUzlHDqw92990827 = ngsUzlHDqw52993594;     ngsUzlHDqw52993594 = ngsUzlHDqw64805029;     ngsUzlHDqw64805029 = ngsUzlHDqw3982203;     ngsUzlHDqw3982203 = ngsUzlHDqw82204238;     ngsUzlHDqw82204238 = ngsUzlHDqw69106163;     ngsUzlHDqw69106163 = ngsUzlHDqw48250947;     ngsUzlHDqw48250947 = ngsUzlHDqw83414924;     ngsUzlHDqw83414924 = ngsUzlHDqw16459858;     ngsUzlHDqw16459858 = ngsUzlHDqw25206219;     ngsUzlHDqw25206219 = ngsUzlHDqw21471870;     ngsUzlHDqw21471870 = ngsUzlHDqw32984489;     ngsUzlHDqw32984489 = ngsUzlHDqw38649239;     ngsUzlHDqw38649239 = ngsUzlHDqw1257039;     ngsUzlHDqw1257039 = ngsUzlHDqw13593927;     ngsUzlHDqw13593927 = ngsUzlHDqw51255617;     ngsUzlHDqw51255617 = ngsUzlHDqw3285335;     ngsUzlHDqw3285335 = ngsUzlHDqw84774181;     ngsUzlHDqw84774181 = ngsUzlHDqw83028047;     ngsUzlHDqw83028047 = ngsUzlHDqw33768245;     ngsUzlHDqw33768245 = ngsUzlHDqw87515864;     ngsUzlHDqw87515864 = ngsUzlHDqw77924773;     ngsUzlHDqw77924773 = ngsUzlHDqw64992174;     ngsUzlHDqw64992174 = ngsUzlHDqw46260188;     ngsUzlHDqw46260188 = ngsUzlHDqw79302431;     ngsUzlHDqw79302431 = ngsUzlHDqw77746383;     ngsUzlHDqw77746383 = ngsUzlHDqw89805871;     ngsUzlHDqw89805871 = ngsUzlHDqw26999729;     ngsUzlHDqw26999729 = ngsUzlHDqw27771222;     ngsUzlHDqw27771222 = ngsUzlHDqw540913;     ngsUzlHDqw540913 = ngsUzlHDqw30124476;     ngsUzlHDqw30124476 = ngsUzlHDqw43078185;     ngsUzlHDqw43078185 = ngsUzlHDqw6915853;     ngsUzlHDqw6915853 = ngsUzlHDqw4576195;     ngsUzlHDqw4576195 = ngsUzlHDqw47436495;     ngsUzlHDqw47436495 = ngsUzlHDqw88397577;     ngsUzlHDqw88397577 = ngsUzlHDqw88280941;     ngsUzlHDqw88280941 = ngsUzlHDqw22783329;     ngsUzlHDqw22783329 = ngsUzlHDqw58570127;     ngsUzlHDqw58570127 = ngsUzlHDqw80650771;     ngsUzlHDqw80650771 = ngsUzlHDqw66836508;     ngsUzlHDqw66836508 = ngsUzlHDqw67855152;     ngsUzlHDqw67855152 = ngsUzlHDqw39217373;     ngsUzlHDqw39217373 = ngsUzlHDqw14250446;     ngsUzlHDqw14250446 = ngsUzlHDqw39521207;     ngsUzlHDqw39521207 = ngsUzlHDqw25173937;     ngsUzlHDqw25173937 = ngsUzlHDqw58943536;     ngsUzlHDqw58943536 = ngsUzlHDqw87521856;     ngsUzlHDqw87521856 = ngsUzlHDqw98434889;     ngsUzlHDqw98434889 = ngsUzlHDqw78120315;     ngsUzlHDqw78120315 = ngsUzlHDqw51745461;     ngsUzlHDqw51745461 = ngsUzlHDqw23647389;     ngsUzlHDqw23647389 = ngsUzlHDqw67651746;     ngsUzlHDqw67651746 = ngsUzlHDqw87310684;     ngsUzlHDqw87310684 = ngsUzlHDqw22780556;     ngsUzlHDqw22780556 = ngsUzlHDqw15066054;     ngsUzlHDqw15066054 = ngsUzlHDqw88001419;     ngsUzlHDqw88001419 = ngsUzlHDqw18544842;     ngsUzlHDqw18544842 = ngsUzlHDqw24679771;     ngsUzlHDqw24679771 = ngsUzlHDqw54985098;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void YNPQmqFJWl78329336() {     int mSLBFLOJtE77372215 = -585367557;    int mSLBFLOJtE88689087 = -76239843;    int mSLBFLOJtE58820145 = 94109559;    int mSLBFLOJtE40555236 = -394326801;    int mSLBFLOJtE30959236 = -315948603;    int mSLBFLOJtE63738816 = -914709203;    int mSLBFLOJtE30941680 = 40343229;    int mSLBFLOJtE9235864 = -83841649;    int mSLBFLOJtE1611970 = -509392549;    int mSLBFLOJtE99016848 = -150715354;    int mSLBFLOJtE75002717 = -719464565;    int mSLBFLOJtE31242108 = -558023870;    int mSLBFLOJtE45592947 = -733195253;    int mSLBFLOJtE81643296 = -139464166;    int mSLBFLOJtE93882430 = -601327673;    int mSLBFLOJtE99827621 = -645418437;    int mSLBFLOJtE88369397 = 38988019;    int mSLBFLOJtE99734394 = 74038715;    int mSLBFLOJtE69276413 = -576943110;    int mSLBFLOJtE93388898 = -78505668;    int mSLBFLOJtE41310430 = -396073870;    int mSLBFLOJtE5206390 = -197934997;    int mSLBFLOJtE56379294 = -50956430;    int mSLBFLOJtE53265319 = -149779627;    int mSLBFLOJtE76314613 = -75399159;    int mSLBFLOJtE38059929 = -907144261;    int mSLBFLOJtE62176875 = -686338541;    int mSLBFLOJtE57181526 = 41236369;    int mSLBFLOJtE62569792 = -28600273;    int mSLBFLOJtE48613844 = -20543843;    int mSLBFLOJtE5283882 = -725241292;    int mSLBFLOJtE15699617 = -547713618;    int mSLBFLOJtE78631446 = -762246399;    int mSLBFLOJtE48167582 = -244987715;    int mSLBFLOJtE88258180 = -468521427;    int mSLBFLOJtE71068801 = -200540108;    int mSLBFLOJtE48470508 = -231509770;    int mSLBFLOJtE62047671 = -733299534;    int mSLBFLOJtE98987924 = -44763328;    int mSLBFLOJtE41264644 = -925368306;    int mSLBFLOJtE39053791 = -830774926;    int mSLBFLOJtE81499218 = -767750379;    int mSLBFLOJtE58543786 = -613955173;    int mSLBFLOJtE64249149 = -626649211;    int mSLBFLOJtE71965469 = -3519533;    int mSLBFLOJtE90304520 = 84372952;    int mSLBFLOJtE81403560 = -794732246;    int mSLBFLOJtE92849621 = -784658304;    int mSLBFLOJtE77874829 = -231697533;    int mSLBFLOJtE13543604 = -917851833;    int mSLBFLOJtE34075435 = -220758002;    int mSLBFLOJtE95323126 = -590874102;    int mSLBFLOJtE11298892 = -262054630;    int mSLBFLOJtE98935784 = -45816334;    int mSLBFLOJtE37018906 = -364640073;    int mSLBFLOJtE72165825 = -287432561;    int mSLBFLOJtE32309794 = 74716587;    int mSLBFLOJtE5554827 = -756110814;    int mSLBFLOJtE64240623 = -218927642;    int mSLBFLOJtE92899307 = -408804342;    int mSLBFLOJtE1561942 = -128370662;    int mSLBFLOJtE73760153 = 99106860;    int mSLBFLOJtE46666072 = 44758623;    int mSLBFLOJtE52998126 = -388848706;    int mSLBFLOJtE93732967 = -425474062;    int mSLBFLOJtE59303100 = -71750947;    int mSLBFLOJtE52610662 = -795777471;    int mSLBFLOJtE97425364 = -388207539;    int mSLBFLOJtE93385116 = -670942739;    int mSLBFLOJtE22813630 = -300787565;    int mSLBFLOJtE51357113 = -313908668;    int mSLBFLOJtE26321727 = -227712447;    int mSLBFLOJtE746470 = -881197957;    int mSLBFLOJtE28011769 = -651574804;    int mSLBFLOJtE54335107 = -247730742;    int mSLBFLOJtE59811211 = -628323492;    int mSLBFLOJtE46662604 = -583979824;    int mSLBFLOJtE92130145 = -424307220;    int mSLBFLOJtE81299849 = -46260095;    int mSLBFLOJtE86010092 = -59772111;    int mSLBFLOJtE56656368 = -12412015;    int mSLBFLOJtE69327253 = -901680238;    int mSLBFLOJtE79306697 = -727066098;    int mSLBFLOJtE49026188 = -110748441;    int mSLBFLOJtE14538409 = -799785842;    int mSLBFLOJtE9960755 = -34367191;    int mSLBFLOJtE4400725 = -185658988;    int mSLBFLOJtE79695661 = -616430065;    int mSLBFLOJtE11148676 = -880347642;    int mSLBFLOJtE16092355 = -81088867;    int mSLBFLOJtE38759008 = -175256696;    int mSLBFLOJtE42915682 = -475398956;    int mSLBFLOJtE97807048 = -414371892;    int mSLBFLOJtE6088618 = -635958986;    int mSLBFLOJtE39702703 = -696997644;    int mSLBFLOJtE65293638 = -829881787;    int mSLBFLOJtE34833147 = -712509002;    int mSLBFLOJtE5545661 = -125106467;    int mSLBFLOJtE70516182 = -101175150;    int mSLBFLOJtE12662369 = -585367557;     mSLBFLOJtE77372215 = mSLBFLOJtE88689087;     mSLBFLOJtE88689087 = mSLBFLOJtE58820145;     mSLBFLOJtE58820145 = mSLBFLOJtE40555236;     mSLBFLOJtE40555236 = mSLBFLOJtE30959236;     mSLBFLOJtE30959236 = mSLBFLOJtE63738816;     mSLBFLOJtE63738816 = mSLBFLOJtE30941680;     mSLBFLOJtE30941680 = mSLBFLOJtE9235864;     mSLBFLOJtE9235864 = mSLBFLOJtE1611970;     mSLBFLOJtE1611970 = mSLBFLOJtE99016848;     mSLBFLOJtE99016848 = mSLBFLOJtE75002717;     mSLBFLOJtE75002717 = mSLBFLOJtE31242108;     mSLBFLOJtE31242108 = mSLBFLOJtE45592947;     mSLBFLOJtE45592947 = mSLBFLOJtE81643296;     mSLBFLOJtE81643296 = mSLBFLOJtE93882430;     mSLBFLOJtE93882430 = mSLBFLOJtE99827621;     mSLBFLOJtE99827621 = mSLBFLOJtE88369397;     mSLBFLOJtE88369397 = mSLBFLOJtE99734394;     mSLBFLOJtE99734394 = mSLBFLOJtE69276413;     mSLBFLOJtE69276413 = mSLBFLOJtE93388898;     mSLBFLOJtE93388898 = mSLBFLOJtE41310430;     mSLBFLOJtE41310430 = mSLBFLOJtE5206390;     mSLBFLOJtE5206390 = mSLBFLOJtE56379294;     mSLBFLOJtE56379294 = mSLBFLOJtE53265319;     mSLBFLOJtE53265319 = mSLBFLOJtE76314613;     mSLBFLOJtE76314613 = mSLBFLOJtE38059929;     mSLBFLOJtE38059929 = mSLBFLOJtE62176875;     mSLBFLOJtE62176875 = mSLBFLOJtE57181526;     mSLBFLOJtE57181526 = mSLBFLOJtE62569792;     mSLBFLOJtE62569792 = mSLBFLOJtE48613844;     mSLBFLOJtE48613844 = mSLBFLOJtE5283882;     mSLBFLOJtE5283882 = mSLBFLOJtE15699617;     mSLBFLOJtE15699617 = mSLBFLOJtE78631446;     mSLBFLOJtE78631446 = mSLBFLOJtE48167582;     mSLBFLOJtE48167582 = mSLBFLOJtE88258180;     mSLBFLOJtE88258180 = mSLBFLOJtE71068801;     mSLBFLOJtE71068801 = mSLBFLOJtE48470508;     mSLBFLOJtE48470508 = mSLBFLOJtE62047671;     mSLBFLOJtE62047671 = mSLBFLOJtE98987924;     mSLBFLOJtE98987924 = mSLBFLOJtE41264644;     mSLBFLOJtE41264644 = mSLBFLOJtE39053791;     mSLBFLOJtE39053791 = mSLBFLOJtE81499218;     mSLBFLOJtE81499218 = mSLBFLOJtE58543786;     mSLBFLOJtE58543786 = mSLBFLOJtE64249149;     mSLBFLOJtE64249149 = mSLBFLOJtE71965469;     mSLBFLOJtE71965469 = mSLBFLOJtE90304520;     mSLBFLOJtE90304520 = mSLBFLOJtE81403560;     mSLBFLOJtE81403560 = mSLBFLOJtE92849621;     mSLBFLOJtE92849621 = mSLBFLOJtE77874829;     mSLBFLOJtE77874829 = mSLBFLOJtE13543604;     mSLBFLOJtE13543604 = mSLBFLOJtE34075435;     mSLBFLOJtE34075435 = mSLBFLOJtE95323126;     mSLBFLOJtE95323126 = mSLBFLOJtE11298892;     mSLBFLOJtE11298892 = mSLBFLOJtE98935784;     mSLBFLOJtE98935784 = mSLBFLOJtE37018906;     mSLBFLOJtE37018906 = mSLBFLOJtE72165825;     mSLBFLOJtE72165825 = mSLBFLOJtE32309794;     mSLBFLOJtE32309794 = mSLBFLOJtE5554827;     mSLBFLOJtE5554827 = mSLBFLOJtE64240623;     mSLBFLOJtE64240623 = mSLBFLOJtE92899307;     mSLBFLOJtE92899307 = mSLBFLOJtE1561942;     mSLBFLOJtE1561942 = mSLBFLOJtE73760153;     mSLBFLOJtE73760153 = mSLBFLOJtE46666072;     mSLBFLOJtE46666072 = mSLBFLOJtE52998126;     mSLBFLOJtE52998126 = mSLBFLOJtE93732967;     mSLBFLOJtE93732967 = mSLBFLOJtE59303100;     mSLBFLOJtE59303100 = mSLBFLOJtE52610662;     mSLBFLOJtE52610662 = mSLBFLOJtE97425364;     mSLBFLOJtE97425364 = mSLBFLOJtE93385116;     mSLBFLOJtE93385116 = mSLBFLOJtE22813630;     mSLBFLOJtE22813630 = mSLBFLOJtE51357113;     mSLBFLOJtE51357113 = mSLBFLOJtE26321727;     mSLBFLOJtE26321727 = mSLBFLOJtE746470;     mSLBFLOJtE746470 = mSLBFLOJtE28011769;     mSLBFLOJtE28011769 = mSLBFLOJtE54335107;     mSLBFLOJtE54335107 = mSLBFLOJtE59811211;     mSLBFLOJtE59811211 = mSLBFLOJtE46662604;     mSLBFLOJtE46662604 = mSLBFLOJtE92130145;     mSLBFLOJtE92130145 = mSLBFLOJtE81299849;     mSLBFLOJtE81299849 = mSLBFLOJtE86010092;     mSLBFLOJtE86010092 = mSLBFLOJtE56656368;     mSLBFLOJtE56656368 = mSLBFLOJtE69327253;     mSLBFLOJtE69327253 = mSLBFLOJtE79306697;     mSLBFLOJtE79306697 = mSLBFLOJtE49026188;     mSLBFLOJtE49026188 = mSLBFLOJtE14538409;     mSLBFLOJtE14538409 = mSLBFLOJtE9960755;     mSLBFLOJtE9960755 = mSLBFLOJtE4400725;     mSLBFLOJtE4400725 = mSLBFLOJtE79695661;     mSLBFLOJtE79695661 = mSLBFLOJtE11148676;     mSLBFLOJtE11148676 = mSLBFLOJtE16092355;     mSLBFLOJtE16092355 = mSLBFLOJtE38759008;     mSLBFLOJtE38759008 = mSLBFLOJtE42915682;     mSLBFLOJtE42915682 = mSLBFLOJtE97807048;     mSLBFLOJtE97807048 = mSLBFLOJtE6088618;     mSLBFLOJtE6088618 = mSLBFLOJtE39702703;     mSLBFLOJtE39702703 = mSLBFLOJtE65293638;     mSLBFLOJtE65293638 = mSLBFLOJtE34833147;     mSLBFLOJtE34833147 = mSLBFLOJtE5545661;     mSLBFLOJtE5545661 = mSLBFLOJtE70516182;     mSLBFLOJtE70516182 = mSLBFLOJtE12662369;     mSLBFLOJtE12662369 = mSLBFLOJtE77372215;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ACMzXSIrQU14974311() {     int aLDUkQmiwK99759332 = -219771520;    int aLDUkQmiwK73036588 = -24782898;    int aLDUkQmiwK87589423 = -145544285;    int aLDUkQmiwK55561924 = -400734333;    int aLDUkQmiwK29133490 = -907005604;    int aLDUkQmiwK78621933 = -775317581;    int aLDUkQmiwK23097419 = -881644077;    int aLDUkQmiwK84869722 = -554438294;    int aLDUkQmiwK98775100 = -393588588;    int aLDUkQmiwK90780433 = -993456687;    int aLDUkQmiwK12544577 = -716088965;    int aLDUkQmiwK87145057 = -808769163;    int aLDUkQmiwK12601127 = -251912572;    int aLDUkQmiwK24258049 = -667172923;    int aLDUkQmiwK30617708 = -228805386;    int aLDUkQmiwK44083534 = 72914021;    int aLDUkQmiwK38192749 = -238291801;    int aLDUkQmiwK5710811 = -377703975;    int aLDUkQmiwK10558044 = -712045235;    int aLDUkQmiwK71905181 = -331140121;    int aLDUkQmiwK42193537 = -387561179;    int aLDUkQmiwK69021608 = -702701310;    int aLDUkQmiwK59672617 = -227037022;    int aLDUkQmiwK79765105 = -713772485;    int aLDUkQmiwK11854860 = -796958284;    int aLDUkQmiwK26362923 = -251038485;    int aLDUkQmiwK9266296 = -443925034;    int aLDUkQmiwK63092976 = -847218085;    int aLDUkQmiwK69462353 = -372857392;    int aLDUkQmiwK57771023 = 59160290;    int aLDUkQmiwK49574687 = -806557444;    int aLDUkQmiwK73240807 = -99784972;    int aLDUkQmiwK59670116 = -191589894;    int aLDUkQmiwK7556270 = -569114565;    int aLDUkQmiwK64487545 = -532277850;    int aLDUkQmiwK12761671 = -515935485;    int aLDUkQmiwK41910221 = 35542279;    int aLDUkQmiwK15673772 = -600925006;    int aLDUkQmiwK47296056 = -445809360;    int aLDUkQmiwK61450359 = 38407665;    int aLDUkQmiwK67811162 = -233952593;    int aLDUkQmiwK70007609 = -525659070;    int aLDUkQmiwK64093978 = -501485756;    int aLDUkQmiwK63693268 = -624012860;    int aLDUkQmiwK39948736 = -912663011;    int aLDUkQmiwK98404802 = -97856144;    int aLDUkQmiwK93700957 = -273142499;    int aLDUkQmiwK37448296 = -541193295;    int aLDUkQmiwK72334734 = -181414232;    int aLDUkQmiwK10627350 = 40127908;    int aLDUkQmiwK42944652 = -575834279;    int aLDUkQmiwK69174384 = 78306616;    int aLDUkQmiwK89613295 = 66034140;    int aLDUkQmiwK59222329 = -321497143;    int aLDUkQmiwK72780774 = -505344188;    int aLDUkQmiwK30737725 = -517070210;    int aLDUkQmiwK13363971 = -797745876;    int aLDUkQmiwK7824318 = -431771801;    int aLDUkQmiwK43707065 = -603776050;    int aLDUkQmiwK2770567 = -555967120;    int aLDUkQmiwK69355638 = -231392548;    int aLDUkQmiwK60004442 = 65574007;    int aLDUkQmiwK15407370 = -81580902;    int aLDUkQmiwK41004077 = -352748879;    int aLDUkQmiwK41205747 = -86899243;    int aLDUkQmiwK39303769 = -516303993;    int aLDUkQmiwK27474941 = -517179269;    int aLDUkQmiwK5044858 = -682798008;    int aLDUkQmiwK59770504 = -34895074;    int aLDUkQmiwK17856038 = -712869901;    int aLDUkQmiwK2173314 = -962628259;    int aLDUkQmiwK22518978 = -637366795;    int aLDUkQmiwK58414754 = -931894616;    int aLDUkQmiwK49107685 = -650452900;    int aLDUkQmiwK4094020 = 2812471;    int aLDUkQmiwK72185927 = -861902110;    int aLDUkQmiwK4927631 = -101215554;    int aLDUkQmiwK95979349 = -603024163;    int aLDUkQmiwK39816370 = -801109474;    int aLDUkQmiwK13450058 = -599102140;    int aLDUkQmiwK32661965 = -977895986;    int aLDUkQmiwK71817999 = -902731740;    int aLDUkQmiwK90758242 = -565803854;    int aLDUkQmiwK58835003 = -312985301;    int aLDUkQmiwK14826372 = -365005431;    int aLDUkQmiwK80400303 = -784864061;    int aLDUkQmiwK83627512 = -65819113;    int aLDUkQmiwK447787 = -870092752;    int aLDUkQmiwK34775496 = 36229623;    int aLDUkQmiwK33749821 = 84792360;    int aLDUkQmiwK99397699 = -718189610;    int aLDUkQmiwK34085903 = -532685921;    int aLDUkQmiwK71966707 = -997148957;    int aLDUkQmiwK44525489 = -889842240;    int aLDUkQmiwK92094720 = -730199788;    int aLDUkQmiwK7806721 = -199526600;    int aLDUkQmiwK54600240 = -344078168;    int aLDUkQmiwK23089901 = -48736878;    int aLDUkQmiwK22487522 = -437113618;    int aLDUkQmiwK644967 = -219771520;     aLDUkQmiwK99759332 = aLDUkQmiwK73036588;     aLDUkQmiwK73036588 = aLDUkQmiwK87589423;     aLDUkQmiwK87589423 = aLDUkQmiwK55561924;     aLDUkQmiwK55561924 = aLDUkQmiwK29133490;     aLDUkQmiwK29133490 = aLDUkQmiwK78621933;     aLDUkQmiwK78621933 = aLDUkQmiwK23097419;     aLDUkQmiwK23097419 = aLDUkQmiwK84869722;     aLDUkQmiwK84869722 = aLDUkQmiwK98775100;     aLDUkQmiwK98775100 = aLDUkQmiwK90780433;     aLDUkQmiwK90780433 = aLDUkQmiwK12544577;     aLDUkQmiwK12544577 = aLDUkQmiwK87145057;     aLDUkQmiwK87145057 = aLDUkQmiwK12601127;     aLDUkQmiwK12601127 = aLDUkQmiwK24258049;     aLDUkQmiwK24258049 = aLDUkQmiwK30617708;     aLDUkQmiwK30617708 = aLDUkQmiwK44083534;     aLDUkQmiwK44083534 = aLDUkQmiwK38192749;     aLDUkQmiwK38192749 = aLDUkQmiwK5710811;     aLDUkQmiwK5710811 = aLDUkQmiwK10558044;     aLDUkQmiwK10558044 = aLDUkQmiwK71905181;     aLDUkQmiwK71905181 = aLDUkQmiwK42193537;     aLDUkQmiwK42193537 = aLDUkQmiwK69021608;     aLDUkQmiwK69021608 = aLDUkQmiwK59672617;     aLDUkQmiwK59672617 = aLDUkQmiwK79765105;     aLDUkQmiwK79765105 = aLDUkQmiwK11854860;     aLDUkQmiwK11854860 = aLDUkQmiwK26362923;     aLDUkQmiwK26362923 = aLDUkQmiwK9266296;     aLDUkQmiwK9266296 = aLDUkQmiwK63092976;     aLDUkQmiwK63092976 = aLDUkQmiwK69462353;     aLDUkQmiwK69462353 = aLDUkQmiwK57771023;     aLDUkQmiwK57771023 = aLDUkQmiwK49574687;     aLDUkQmiwK49574687 = aLDUkQmiwK73240807;     aLDUkQmiwK73240807 = aLDUkQmiwK59670116;     aLDUkQmiwK59670116 = aLDUkQmiwK7556270;     aLDUkQmiwK7556270 = aLDUkQmiwK64487545;     aLDUkQmiwK64487545 = aLDUkQmiwK12761671;     aLDUkQmiwK12761671 = aLDUkQmiwK41910221;     aLDUkQmiwK41910221 = aLDUkQmiwK15673772;     aLDUkQmiwK15673772 = aLDUkQmiwK47296056;     aLDUkQmiwK47296056 = aLDUkQmiwK61450359;     aLDUkQmiwK61450359 = aLDUkQmiwK67811162;     aLDUkQmiwK67811162 = aLDUkQmiwK70007609;     aLDUkQmiwK70007609 = aLDUkQmiwK64093978;     aLDUkQmiwK64093978 = aLDUkQmiwK63693268;     aLDUkQmiwK63693268 = aLDUkQmiwK39948736;     aLDUkQmiwK39948736 = aLDUkQmiwK98404802;     aLDUkQmiwK98404802 = aLDUkQmiwK93700957;     aLDUkQmiwK93700957 = aLDUkQmiwK37448296;     aLDUkQmiwK37448296 = aLDUkQmiwK72334734;     aLDUkQmiwK72334734 = aLDUkQmiwK10627350;     aLDUkQmiwK10627350 = aLDUkQmiwK42944652;     aLDUkQmiwK42944652 = aLDUkQmiwK69174384;     aLDUkQmiwK69174384 = aLDUkQmiwK89613295;     aLDUkQmiwK89613295 = aLDUkQmiwK59222329;     aLDUkQmiwK59222329 = aLDUkQmiwK72780774;     aLDUkQmiwK72780774 = aLDUkQmiwK30737725;     aLDUkQmiwK30737725 = aLDUkQmiwK13363971;     aLDUkQmiwK13363971 = aLDUkQmiwK7824318;     aLDUkQmiwK7824318 = aLDUkQmiwK43707065;     aLDUkQmiwK43707065 = aLDUkQmiwK2770567;     aLDUkQmiwK2770567 = aLDUkQmiwK69355638;     aLDUkQmiwK69355638 = aLDUkQmiwK60004442;     aLDUkQmiwK60004442 = aLDUkQmiwK15407370;     aLDUkQmiwK15407370 = aLDUkQmiwK41004077;     aLDUkQmiwK41004077 = aLDUkQmiwK41205747;     aLDUkQmiwK41205747 = aLDUkQmiwK39303769;     aLDUkQmiwK39303769 = aLDUkQmiwK27474941;     aLDUkQmiwK27474941 = aLDUkQmiwK5044858;     aLDUkQmiwK5044858 = aLDUkQmiwK59770504;     aLDUkQmiwK59770504 = aLDUkQmiwK17856038;     aLDUkQmiwK17856038 = aLDUkQmiwK2173314;     aLDUkQmiwK2173314 = aLDUkQmiwK22518978;     aLDUkQmiwK22518978 = aLDUkQmiwK58414754;     aLDUkQmiwK58414754 = aLDUkQmiwK49107685;     aLDUkQmiwK49107685 = aLDUkQmiwK4094020;     aLDUkQmiwK4094020 = aLDUkQmiwK72185927;     aLDUkQmiwK72185927 = aLDUkQmiwK4927631;     aLDUkQmiwK4927631 = aLDUkQmiwK95979349;     aLDUkQmiwK95979349 = aLDUkQmiwK39816370;     aLDUkQmiwK39816370 = aLDUkQmiwK13450058;     aLDUkQmiwK13450058 = aLDUkQmiwK32661965;     aLDUkQmiwK32661965 = aLDUkQmiwK71817999;     aLDUkQmiwK71817999 = aLDUkQmiwK90758242;     aLDUkQmiwK90758242 = aLDUkQmiwK58835003;     aLDUkQmiwK58835003 = aLDUkQmiwK14826372;     aLDUkQmiwK14826372 = aLDUkQmiwK80400303;     aLDUkQmiwK80400303 = aLDUkQmiwK83627512;     aLDUkQmiwK83627512 = aLDUkQmiwK447787;     aLDUkQmiwK447787 = aLDUkQmiwK34775496;     aLDUkQmiwK34775496 = aLDUkQmiwK33749821;     aLDUkQmiwK33749821 = aLDUkQmiwK99397699;     aLDUkQmiwK99397699 = aLDUkQmiwK34085903;     aLDUkQmiwK34085903 = aLDUkQmiwK71966707;     aLDUkQmiwK71966707 = aLDUkQmiwK44525489;     aLDUkQmiwK44525489 = aLDUkQmiwK92094720;     aLDUkQmiwK92094720 = aLDUkQmiwK7806721;     aLDUkQmiwK7806721 = aLDUkQmiwK54600240;     aLDUkQmiwK54600240 = aLDUkQmiwK23089901;     aLDUkQmiwK23089901 = aLDUkQmiwK22487522;     aLDUkQmiwK22487522 = aLDUkQmiwK644967;     aLDUkQmiwK644967 = aLDUkQmiwK99759332;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void kqOOMvsLPY51619285() {     int hLZWVSkmoz22146450 = -954175483;    int hLZWVSkmoz57384089 = 26674048;    int hLZWVSkmoz16358703 = -385198129;    int hLZWVSkmoz70568613 = -407141864;    int hLZWVSkmoz27307744 = -398062605;    int hLZWVSkmoz93505050 = -635925959;    int hLZWVSkmoz15253158 = -703631383;    int hLZWVSkmoz60503581 = 74965062;    int hLZWVSkmoz95938231 = -277784627;    int hLZWVSkmoz82544017 = -736198020;    int hLZWVSkmoz50086436 = -712713366;    int hLZWVSkmoz43048006 = 40485544;    int hLZWVSkmoz79609307 = -870629892;    int hLZWVSkmoz66872801 = -94881680;    int hLZWVSkmoz67352985 = -956283099;    int hLZWVSkmoz88339446 = -308753522;    int hLZWVSkmoz88016101 = -515571620;    int hLZWVSkmoz11687227 = -829446666;    int hLZWVSkmoz51839675 = -847147360;    int hLZWVSkmoz50421465 = -583774575;    int hLZWVSkmoz43076644 = -379048487;    int hLZWVSkmoz32836827 = -107467623;    int hLZWVSkmoz62965940 = -403117615;    int hLZWVSkmoz6264893 = -177765342;    int hLZWVSkmoz47395107 = -418517408;    int hLZWVSkmoz14665918 = -694932708;    int hLZWVSkmoz56355716 = -201511527;    int hLZWVSkmoz69004426 = -635672538;    int hLZWVSkmoz76354913 = -717114511;    int hLZWVSkmoz66928202 = -961135576;    int hLZWVSkmoz93865491 = -887873596;    int hLZWVSkmoz30781999 = -751856327;    int hLZWVSkmoz40708787 = -720933389;    int hLZWVSkmoz66944957 = -893241415;    int hLZWVSkmoz40716910 = -596034272;    int hLZWVSkmoz54454540 = -831330861;    int hLZWVSkmoz35349933 = -797405673;    int hLZWVSkmoz69299873 = -468550478;    int hLZWVSkmoz95604187 = -846855391;    int hLZWVSkmoz81636073 = -97816364;    int hLZWVSkmoz96568533 = -737130259;    int hLZWVSkmoz58516001 = -283567761;    int hLZWVSkmoz69644170 = -389016340;    int hLZWVSkmoz63137387 = -621376509;    int hLZWVSkmoz7932003 = -721806488;    int hLZWVSkmoz6505085 = -280085239;    int hLZWVSkmoz5998356 = -851552752;    int hLZWVSkmoz82046970 = -297728286;    int hLZWVSkmoz66794639 = -131130930;    int hLZWVSkmoz7711095 = -101892351;    int hLZWVSkmoz51813868 = -930910556;    int hLZWVSkmoz43025641 = -352512666;    int hLZWVSkmoz67927698 = -705877091;    int hLZWVSkmoz19508875 = -597177952;    int hLZWVSkmoz8542642 = -646048303;    int hLZWVSkmoz89309623 = -746707860;    int hLZWVSkmoz94418148 = -570208338;    int hLZWVSkmoz10093810 = -107432788;    int hLZWVSkmoz23173506 = -988624457;    int hLZWVSkmoz12641827 = -703129897;    int hLZWVSkmoz37149334 = -334414433;    int hLZWVSkmoz46248731 = 32041155;    int hLZWVSkmoz84148667 = -207920427;    int hLZWVSkmoz29010029 = -316649051;    int hLZWVSkmoz88678526 = -848324424;    int hLZWVSkmoz19304438 = -960857040;    int hLZWVSkmoz2339220 = -238581068;    int hLZWVSkmoz12664351 = -977388478;    int hLZWVSkmoz26155892 = -498847408;    int hLZWVSkmoz12898446 = -24952238;    int hLZWVSkmoz52989514 = -511347850;    int hLZWVSkmoz18716228 = 52978857;    int hLZWVSkmoz16083040 = -982591275;    int hLZWVSkmoz70203601 = -649330996;    int hLZWVSkmoz53852932 = -846644317;    int hLZWVSkmoz84560643 = 4519273;    int hLZWVSkmoz63192657 = -718451283;    int hLZWVSkmoz99828553 = -781741106;    int hLZWVSkmoz98332890 = -455958854;    int hLZWVSkmoz40890022 = -38432169;    int hLZWVSkmoz8667563 = -843379957;    int hLZWVSkmoz74308745 = -903783242;    int hLZWVSkmoz2209788 = -404541609;    int hLZWVSkmoz68643818 = -515222161;    int hLZWVSkmoz15114335 = 69774979;    int hLZWVSkmoz50839851 = -435360931;    int hLZWVSkmoz62854300 = 54020763;    int hLZWVSkmoz21199912 = -23755438;    int hLZWVSkmoz58402316 = -147193112;    int hLZWVSkmoz51407286 = -849326412;    int hLZWVSkmoz60036391 = -161122523;    int hLZWVSkmoz25256124 = -589972885;    int hLZWVSkmoz46126367 = -479926022;    int hLZWVSkmoz82962361 = -43725495;    int hLZWVSkmoz44486739 = -763401932;    int hLZWVSkmoz50319802 = -669171414;    int hLZWVSkmoz74367333 = 24352666;    int hLZWVSkmoz40634141 = 27632711;    int hLZWVSkmoz74458861 = -773052086;    int hLZWVSkmoz88627564 = -954175483;     hLZWVSkmoz22146450 = hLZWVSkmoz57384089;     hLZWVSkmoz57384089 = hLZWVSkmoz16358703;     hLZWVSkmoz16358703 = hLZWVSkmoz70568613;     hLZWVSkmoz70568613 = hLZWVSkmoz27307744;     hLZWVSkmoz27307744 = hLZWVSkmoz93505050;     hLZWVSkmoz93505050 = hLZWVSkmoz15253158;     hLZWVSkmoz15253158 = hLZWVSkmoz60503581;     hLZWVSkmoz60503581 = hLZWVSkmoz95938231;     hLZWVSkmoz95938231 = hLZWVSkmoz82544017;     hLZWVSkmoz82544017 = hLZWVSkmoz50086436;     hLZWVSkmoz50086436 = hLZWVSkmoz43048006;     hLZWVSkmoz43048006 = hLZWVSkmoz79609307;     hLZWVSkmoz79609307 = hLZWVSkmoz66872801;     hLZWVSkmoz66872801 = hLZWVSkmoz67352985;     hLZWVSkmoz67352985 = hLZWVSkmoz88339446;     hLZWVSkmoz88339446 = hLZWVSkmoz88016101;     hLZWVSkmoz88016101 = hLZWVSkmoz11687227;     hLZWVSkmoz11687227 = hLZWVSkmoz51839675;     hLZWVSkmoz51839675 = hLZWVSkmoz50421465;     hLZWVSkmoz50421465 = hLZWVSkmoz43076644;     hLZWVSkmoz43076644 = hLZWVSkmoz32836827;     hLZWVSkmoz32836827 = hLZWVSkmoz62965940;     hLZWVSkmoz62965940 = hLZWVSkmoz6264893;     hLZWVSkmoz6264893 = hLZWVSkmoz47395107;     hLZWVSkmoz47395107 = hLZWVSkmoz14665918;     hLZWVSkmoz14665918 = hLZWVSkmoz56355716;     hLZWVSkmoz56355716 = hLZWVSkmoz69004426;     hLZWVSkmoz69004426 = hLZWVSkmoz76354913;     hLZWVSkmoz76354913 = hLZWVSkmoz66928202;     hLZWVSkmoz66928202 = hLZWVSkmoz93865491;     hLZWVSkmoz93865491 = hLZWVSkmoz30781999;     hLZWVSkmoz30781999 = hLZWVSkmoz40708787;     hLZWVSkmoz40708787 = hLZWVSkmoz66944957;     hLZWVSkmoz66944957 = hLZWVSkmoz40716910;     hLZWVSkmoz40716910 = hLZWVSkmoz54454540;     hLZWVSkmoz54454540 = hLZWVSkmoz35349933;     hLZWVSkmoz35349933 = hLZWVSkmoz69299873;     hLZWVSkmoz69299873 = hLZWVSkmoz95604187;     hLZWVSkmoz95604187 = hLZWVSkmoz81636073;     hLZWVSkmoz81636073 = hLZWVSkmoz96568533;     hLZWVSkmoz96568533 = hLZWVSkmoz58516001;     hLZWVSkmoz58516001 = hLZWVSkmoz69644170;     hLZWVSkmoz69644170 = hLZWVSkmoz63137387;     hLZWVSkmoz63137387 = hLZWVSkmoz7932003;     hLZWVSkmoz7932003 = hLZWVSkmoz6505085;     hLZWVSkmoz6505085 = hLZWVSkmoz5998356;     hLZWVSkmoz5998356 = hLZWVSkmoz82046970;     hLZWVSkmoz82046970 = hLZWVSkmoz66794639;     hLZWVSkmoz66794639 = hLZWVSkmoz7711095;     hLZWVSkmoz7711095 = hLZWVSkmoz51813868;     hLZWVSkmoz51813868 = hLZWVSkmoz43025641;     hLZWVSkmoz43025641 = hLZWVSkmoz67927698;     hLZWVSkmoz67927698 = hLZWVSkmoz19508875;     hLZWVSkmoz19508875 = hLZWVSkmoz8542642;     hLZWVSkmoz8542642 = hLZWVSkmoz89309623;     hLZWVSkmoz89309623 = hLZWVSkmoz94418148;     hLZWVSkmoz94418148 = hLZWVSkmoz10093810;     hLZWVSkmoz10093810 = hLZWVSkmoz23173506;     hLZWVSkmoz23173506 = hLZWVSkmoz12641827;     hLZWVSkmoz12641827 = hLZWVSkmoz37149334;     hLZWVSkmoz37149334 = hLZWVSkmoz46248731;     hLZWVSkmoz46248731 = hLZWVSkmoz84148667;     hLZWVSkmoz84148667 = hLZWVSkmoz29010029;     hLZWVSkmoz29010029 = hLZWVSkmoz88678526;     hLZWVSkmoz88678526 = hLZWVSkmoz19304438;     hLZWVSkmoz19304438 = hLZWVSkmoz2339220;     hLZWVSkmoz2339220 = hLZWVSkmoz12664351;     hLZWVSkmoz12664351 = hLZWVSkmoz26155892;     hLZWVSkmoz26155892 = hLZWVSkmoz12898446;     hLZWVSkmoz12898446 = hLZWVSkmoz52989514;     hLZWVSkmoz52989514 = hLZWVSkmoz18716228;     hLZWVSkmoz18716228 = hLZWVSkmoz16083040;     hLZWVSkmoz16083040 = hLZWVSkmoz70203601;     hLZWVSkmoz70203601 = hLZWVSkmoz53852932;     hLZWVSkmoz53852932 = hLZWVSkmoz84560643;     hLZWVSkmoz84560643 = hLZWVSkmoz63192657;     hLZWVSkmoz63192657 = hLZWVSkmoz99828553;     hLZWVSkmoz99828553 = hLZWVSkmoz98332890;     hLZWVSkmoz98332890 = hLZWVSkmoz40890022;     hLZWVSkmoz40890022 = hLZWVSkmoz8667563;     hLZWVSkmoz8667563 = hLZWVSkmoz74308745;     hLZWVSkmoz74308745 = hLZWVSkmoz2209788;     hLZWVSkmoz2209788 = hLZWVSkmoz68643818;     hLZWVSkmoz68643818 = hLZWVSkmoz15114335;     hLZWVSkmoz15114335 = hLZWVSkmoz50839851;     hLZWVSkmoz50839851 = hLZWVSkmoz62854300;     hLZWVSkmoz62854300 = hLZWVSkmoz21199912;     hLZWVSkmoz21199912 = hLZWVSkmoz58402316;     hLZWVSkmoz58402316 = hLZWVSkmoz51407286;     hLZWVSkmoz51407286 = hLZWVSkmoz60036391;     hLZWVSkmoz60036391 = hLZWVSkmoz25256124;     hLZWVSkmoz25256124 = hLZWVSkmoz46126367;     hLZWVSkmoz46126367 = hLZWVSkmoz82962361;     hLZWVSkmoz82962361 = hLZWVSkmoz44486739;     hLZWVSkmoz44486739 = hLZWVSkmoz50319802;     hLZWVSkmoz50319802 = hLZWVSkmoz74367333;     hLZWVSkmoz74367333 = hLZWVSkmoz40634141;     hLZWVSkmoz40634141 = hLZWVSkmoz74458861;     hLZWVSkmoz74458861 = hLZWVSkmoz88627564;     hLZWVSkmoz88627564 = hLZWVSkmoz22146450;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void cbaBLGZvHG88264259() {     int aTzfCasKwV44533567 = -588579446;    int aTzfCasKwV41731589 = 78130993;    int aTzfCasKwV45127981 = -624851973;    int aTzfCasKwV85575301 = -413549396;    int aTzfCasKwV25481998 = -989119606;    int aTzfCasKwV8388168 = -496534337;    int aTzfCasKwV7408897 = -525618689;    int aTzfCasKwV36137440 = -395631582;    int aTzfCasKwV93101362 = -161980666;    int aTzfCasKwV74307602 = -478939353;    int aTzfCasKwV87628296 = -709337767;    int aTzfCasKwV98950955 = -210259750;    int aTzfCasKwV46617488 = -389347212;    int aTzfCasKwV9487554 = -622590437;    int aTzfCasKwV4088262 = -583760812;    int aTzfCasKwV32595359 = -690421064;    int aTzfCasKwV37839453 = -792851440;    int aTzfCasKwV17663644 = -181189357;    int aTzfCasKwV93121305 = -982249485;    int aTzfCasKwV28937748 = -836409029;    int aTzfCasKwV43959751 = -370535796;    int aTzfCasKwV96652044 = -612233936;    int aTzfCasKwV66259263 = -579198207;    int aTzfCasKwV32764680 = -741758199;    int aTzfCasKwV82935353 = -40076533;    int aTzfCasKwV2968913 = -38826932;    int aTzfCasKwV3445137 = 40901980;    int aTzfCasKwV74915877 = -424126992;    int aTzfCasKwV83247474 = 38628370;    int aTzfCasKwV76085382 = -881431443;    int aTzfCasKwV38156297 = -969189748;    int aTzfCasKwV88323189 = -303927682;    int aTzfCasKwV21747458 = -150276884;    int aTzfCasKwV26333644 = -117368265;    int aTzfCasKwV16946274 = -659790694;    int aTzfCasKwV96147408 = -46726238;    int aTzfCasKwV28789645 = -530353624;    int aTzfCasKwV22925974 = -336175950;    int aTzfCasKwV43912319 = -147901423;    int aTzfCasKwV1821788 = -234040393;    int aTzfCasKwV25325904 = -140307925;    int aTzfCasKwV47024392 = -41476452;    int aTzfCasKwV75194362 = -276546924;    int aTzfCasKwV62581506 = -618740158;    int aTzfCasKwV75915269 = -530949966;    int aTzfCasKwV14605367 = -462314335;    int aTzfCasKwV18295753 = -329963005;    int aTzfCasKwV26645645 = -54263276;    int aTzfCasKwV61254544 = -80847628;    int aTzfCasKwV4794841 = -243912610;    int aTzfCasKwV60683085 = -185986833;    int aTzfCasKwV16876899 = -783331948;    int aTzfCasKwV46242101 = -377788321;    int aTzfCasKwV79795420 = -872858761;    int aTzfCasKwV44304509 = -786752419;    int aTzfCasKwV47881522 = -976345510;    int aTzfCasKwV75472325 = -342670801;    int aTzfCasKwV12363301 = -883093775;    int aTzfCasKwV2639948 = -273472864;    int aTzfCasKwV22513086 = -850292675;    int aTzfCasKwV4943031 = -437436318;    int aTzfCasKwV32493020 = -1491698;    int aTzfCasKwV52889965 = -334259952;    int aTzfCasKwV17015981 = -280549224;    int aTzfCasKwV36151306 = -509749605;    int aTzfCasKwV99305106 = -305410086;    int aTzfCasKwV77203497 = 40017134;    int aTzfCasKwV20283844 = -171978947;    int aTzfCasKwV92541279 = -962799743;    int aTzfCasKwV7940854 = -437034575;    int aTzfCasKwV3805714 = -60067440;    int aTzfCasKwV14913479 = -356675491;    int aTzfCasKwV73751324 = 66712066;    int aTzfCasKwV91299518 = -648209092;    int aTzfCasKwV3611845 = -596101104;    int aTzfCasKwV96935358 = -229059344;    int aTzfCasKwV21457683 = -235687013;    int aTzfCasKwV3677758 = -960458049;    int aTzfCasKwV56849411 = -110808233;    int aTzfCasKwV68329987 = -577762199;    int aTzfCasKwV84673159 = -708863928;    int aTzfCasKwV76799491 = -904834744;    int aTzfCasKwV13661333 = -243279364;    int aTzfCasKwV78452633 = -717459021;    int aTzfCasKwV15402298 = -595444611;    int aTzfCasKwV21279399 = -85857801;    int aTzfCasKwV42081088 = -926139362;    int aTzfCasKwV41952037 = -277418124;    int aTzfCasKwV82029135 = -330615847;    int aTzfCasKwV69064751 = -683445185;    int aTzfCasKwV20675084 = -704055437;    int aTzfCasKwV16426344 = -647259850;    int aTzfCasKwV20286026 = 37296913;    int aTzfCasKwV21399233 = -297608749;    int aTzfCasKwV96878757 = -796604076;    int aTzfCasKwV92832884 = -38816227;    int aTzfCasKwV94134427 = -707216500;    int aTzfCasKwV58178381 = -995997700;    int aTzfCasKwV26430201 = -8990554;    int aTzfCasKwV76610162 = -588579446;     aTzfCasKwV44533567 = aTzfCasKwV41731589;     aTzfCasKwV41731589 = aTzfCasKwV45127981;     aTzfCasKwV45127981 = aTzfCasKwV85575301;     aTzfCasKwV85575301 = aTzfCasKwV25481998;     aTzfCasKwV25481998 = aTzfCasKwV8388168;     aTzfCasKwV8388168 = aTzfCasKwV7408897;     aTzfCasKwV7408897 = aTzfCasKwV36137440;     aTzfCasKwV36137440 = aTzfCasKwV93101362;     aTzfCasKwV93101362 = aTzfCasKwV74307602;     aTzfCasKwV74307602 = aTzfCasKwV87628296;     aTzfCasKwV87628296 = aTzfCasKwV98950955;     aTzfCasKwV98950955 = aTzfCasKwV46617488;     aTzfCasKwV46617488 = aTzfCasKwV9487554;     aTzfCasKwV9487554 = aTzfCasKwV4088262;     aTzfCasKwV4088262 = aTzfCasKwV32595359;     aTzfCasKwV32595359 = aTzfCasKwV37839453;     aTzfCasKwV37839453 = aTzfCasKwV17663644;     aTzfCasKwV17663644 = aTzfCasKwV93121305;     aTzfCasKwV93121305 = aTzfCasKwV28937748;     aTzfCasKwV28937748 = aTzfCasKwV43959751;     aTzfCasKwV43959751 = aTzfCasKwV96652044;     aTzfCasKwV96652044 = aTzfCasKwV66259263;     aTzfCasKwV66259263 = aTzfCasKwV32764680;     aTzfCasKwV32764680 = aTzfCasKwV82935353;     aTzfCasKwV82935353 = aTzfCasKwV2968913;     aTzfCasKwV2968913 = aTzfCasKwV3445137;     aTzfCasKwV3445137 = aTzfCasKwV74915877;     aTzfCasKwV74915877 = aTzfCasKwV83247474;     aTzfCasKwV83247474 = aTzfCasKwV76085382;     aTzfCasKwV76085382 = aTzfCasKwV38156297;     aTzfCasKwV38156297 = aTzfCasKwV88323189;     aTzfCasKwV88323189 = aTzfCasKwV21747458;     aTzfCasKwV21747458 = aTzfCasKwV26333644;     aTzfCasKwV26333644 = aTzfCasKwV16946274;     aTzfCasKwV16946274 = aTzfCasKwV96147408;     aTzfCasKwV96147408 = aTzfCasKwV28789645;     aTzfCasKwV28789645 = aTzfCasKwV22925974;     aTzfCasKwV22925974 = aTzfCasKwV43912319;     aTzfCasKwV43912319 = aTzfCasKwV1821788;     aTzfCasKwV1821788 = aTzfCasKwV25325904;     aTzfCasKwV25325904 = aTzfCasKwV47024392;     aTzfCasKwV47024392 = aTzfCasKwV75194362;     aTzfCasKwV75194362 = aTzfCasKwV62581506;     aTzfCasKwV62581506 = aTzfCasKwV75915269;     aTzfCasKwV75915269 = aTzfCasKwV14605367;     aTzfCasKwV14605367 = aTzfCasKwV18295753;     aTzfCasKwV18295753 = aTzfCasKwV26645645;     aTzfCasKwV26645645 = aTzfCasKwV61254544;     aTzfCasKwV61254544 = aTzfCasKwV4794841;     aTzfCasKwV4794841 = aTzfCasKwV60683085;     aTzfCasKwV60683085 = aTzfCasKwV16876899;     aTzfCasKwV16876899 = aTzfCasKwV46242101;     aTzfCasKwV46242101 = aTzfCasKwV79795420;     aTzfCasKwV79795420 = aTzfCasKwV44304509;     aTzfCasKwV44304509 = aTzfCasKwV47881522;     aTzfCasKwV47881522 = aTzfCasKwV75472325;     aTzfCasKwV75472325 = aTzfCasKwV12363301;     aTzfCasKwV12363301 = aTzfCasKwV2639948;     aTzfCasKwV2639948 = aTzfCasKwV22513086;     aTzfCasKwV22513086 = aTzfCasKwV4943031;     aTzfCasKwV4943031 = aTzfCasKwV32493020;     aTzfCasKwV32493020 = aTzfCasKwV52889965;     aTzfCasKwV52889965 = aTzfCasKwV17015981;     aTzfCasKwV17015981 = aTzfCasKwV36151306;     aTzfCasKwV36151306 = aTzfCasKwV99305106;     aTzfCasKwV99305106 = aTzfCasKwV77203497;     aTzfCasKwV77203497 = aTzfCasKwV20283844;     aTzfCasKwV20283844 = aTzfCasKwV92541279;     aTzfCasKwV92541279 = aTzfCasKwV7940854;     aTzfCasKwV7940854 = aTzfCasKwV3805714;     aTzfCasKwV3805714 = aTzfCasKwV14913479;     aTzfCasKwV14913479 = aTzfCasKwV73751324;     aTzfCasKwV73751324 = aTzfCasKwV91299518;     aTzfCasKwV91299518 = aTzfCasKwV3611845;     aTzfCasKwV3611845 = aTzfCasKwV96935358;     aTzfCasKwV96935358 = aTzfCasKwV21457683;     aTzfCasKwV21457683 = aTzfCasKwV3677758;     aTzfCasKwV3677758 = aTzfCasKwV56849411;     aTzfCasKwV56849411 = aTzfCasKwV68329987;     aTzfCasKwV68329987 = aTzfCasKwV84673159;     aTzfCasKwV84673159 = aTzfCasKwV76799491;     aTzfCasKwV76799491 = aTzfCasKwV13661333;     aTzfCasKwV13661333 = aTzfCasKwV78452633;     aTzfCasKwV78452633 = aTzfCasKwV15402298;     aTzfCasKwV15402298 = aTzfCasKwV21279399;     aTzfCasKwV21279399 = aTzfCasKwV42081088;     aTzfCasKwV42081088 = aTzfCasKwV41952037;     aTzfCasKwV41952037 = aTzfCasKwV82029135;     aTzfCasKwV82029135 = aTzfCasKwV69064751;     aTzfCasKwV69064751 = aTzfCasKwV20675084;     aTzfCasKwV20675084 = aTzfCasKwV16426344;     aTzfCasKwV16426344 = aTzfCasKwV20286026;     aTzfCasKwV20286026 = aTzfCasKwV21399233;     aTzfCasKwV21399233 = aTzfCasKwV96878757;     aTzfCasKwV96878757 = aTzfCasKwV92832884;     aTzfCasKwV92832884 = aTzfCasKwV94134427;     aTzfCasKwV94134427 = aTzfCasKwV58178381;     aTzfCasKwV58178381 = aTzfCasKwV26430201;     aTzfCasKwV26430201 = aTzfCasKwV76610162;     aTzfCasKwV76610162 = aTzfCasKwV44533567;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void DADuXaVeDB24909235() {     int wnarrUIHQK66920685 = -222983409;    int wnarrUIHQK26079090 = -970412062;    int wnarrUIHQK73897260 = -864505817;    int wnarrUIHQK581990 = -419956927;    int wnarrUIHQK23656252 = -480176608;    int wnarrUIHQK23271284 = -357142715;    int wnarrUIHQK99564635 = -347605995;    int wnarrUIHQK11771298 = -866228226;    int wnarrUIHQK90264493 = -46176705;    int wnarrUIHQK66071187 = -221680686;    int wnarrUIHQK25170156 = -705962168;    int wnarrUIHQK54853904 = -461005043;    int wnarrUIHQK13625669 = 91935469;    int wnarrUIHQK52102305 = -50299193;    int wnarrUIHQK40823539 = -211238525;    int wnarrUIHQK76851271 = 27911393;    int wnarrUIHQK87662804 = 29868740;    int wnarrUIHQK23640060 = -632932047;    int wnarrUIHQK34402937 = -17351610;    int wnarrUIHQK7454032 = 10956518;    int wnarrUIHQK44842858 = -362023104;    int wnarrUIHQK60467263 = -17000250;    int wnarrUIHQK69552587 = -755278799;    int wnarrUIHQK59264467 = -205751056;    int wnarrUIHQK18475601 = -761635657;    int wnarrUIHQK91271906 = -482721156;    int wnarrUIHQK50534557 = -816684513;    int wnarrUIHQK80827327 = -212581445;    int wnarrUIHQK90140035 = -305628750;    int wnarrUIHQK85242561 = -801727309;    int wnarrUIHQK82447102 = 49494100;    int wnarrUIHQK45864380 = -955999036;    int wnarrUIHQK2786128 = -679620379;    int wnarrUIHQK85722331 = -441495115;    int wnarrUIHQK93175638 = -723547117;    int wnarrUIHQK37840278 = -362121614;    int wnarrUIHQK22229357 = -263301576;    int wnarrUIHQK76552074 = -203801422;    int wnarrUIHQK92220450 = -548947455;    int wnarrUIHQK22007503 = -370264423;    int wnarrUIHQK54083275 = -643485591;    int wnarrUIHQK35532783 = -899385143;    int wnarrUIHQK80744554 = -164077508;    int wnarrUIHQK62025626 = -616103807;    int wnarrUIHQK43898536 = -340093444;    int wnarrUIHQK22705649 = -644543430;    int wnarrUIHQK30593150 = -908373258;    int wnarrUIHQK71244319 = -910798267;    int wnarrUIHQK55714449 = -30564327;    int wnarrUIHQK1878587 = -385932869;    int wnarrUIHQK69552301 = -541063110;    int wnarrUIHQK90728155 = -114151230;    int wnarrUIHQK24556505 = -49699551;    int wnarrUIHQK40081965 = -48539569;    int wnarrUIHQK80066376 = -927456534;    int wnarrUIHQK6453422 = -105983160;    int wnarrUIHQK56526503 = -115133264;    int wnarrUIHQK14632793 = -558754762;    int wnarrUIHQK82106389 = -658321271;    int wnarrUIHQK32384346 = -997455452;    int wnarrUIHQK72736727 = -540458203;    int wnarrUIHQK18737309 = -35024550;    int wnarrUIHQK21631263 = -460599477;    int wnarrUIHQK5021933 = -244449396;    int wnarrUIHQK83624085 = -171174786;    int wnarrUIHQK79305775 = -749963132;    int wnarrUIHQK52067776 = -781384665;    int wnarrUIHQK27903337 = -466569417;    int wnarrUIHQK58926667 = -326752077;    int wnarrUIHQK2983262 = -849116911;    int wnarrUIHQK54621914 = -708787031;    int wnarrUIHQK11110730 = -766329839;    int wnarrUIHQK31419610 = 16015407;    int wnarrUIHQK12395435 = -647087188;    int wnarrUIHQK53370757 = -345557892;    int wnarrUIHQK9310075 = -462637962;    int wnarrUIHQK79722709 = -852922743;    int wnarrUIHQK7526961 = -39174992;    int wnarrUIHQK15365932 = -865657613;    int wnarrUIHQK95769952 = -17092228;    int wnarrUIHQK60678757 = -574347899;    int wnarrUIHQK79290237 = -905886246;    int wnarrUIHQK25112878 = -82017119;    int wnarrUIHQK88261449 = -919695881;    int wnarrUIHQK15690261 = -160664200;    int wnarrUIHQK91718946 = -836354671;    int wnarrUIHQK21307876 = -806299486;    int wnarrUIHQK62704162 = -531080810;    int wnarrUIHQK5655956 = -514038582;    int wnarrUIHQK86722217 = -517563958;    int wnarrUIHQK81313775 = -146988351;    int wnarrUIHQK7596565 = -704546814;    int wnarrUIHQK94445685 = -545480152;    int wnarrUIHQK59836104 = -551492003;    int wnarrUIHQK49270775 = -829806220;    int wnarrUIHQK35345967 = -508461041;    int wnarrUIHQK13901521 = -338785666;    int wnarrUIHQK75722622 = -919628112;    int wnarrUIHQK78401541 = -344929022;    int wnarrUIHQK64592760 = -222983409;     wnarrUIHQK66920685 = wnarrUIHQK26079090;     wnarrUIHQK26079090 = wnarrUIHQK73897260;     wnarrUIHQK73897260 = wnarrUIHQK581990;     wnarrUIHQK581990 = wnarrUIHQK23656252;     wnarrUIHQK23656252 = wnarrUIHQK23271284;     wnarrUIHQK23271284 = wnarrUIHQK99564635;     wnarrUIHQK99564635 = wnarrUIHQK11771298;     wnarrUIHQK11771298 = wnarrUIHQK90264493;     wnarrUIHQK90264493 = wnarrUIHQK66071187;     wnarrUIHQK66071187 = wnarrUIHQK25170156;     wnarrUIHQK25170156 = wnarrUIHQK54853904;     wnarrUIHQK54853904 = wnarrUIHQK13625669;     wnarrUIHQK13625669 = wnarrUIHQK52102305;     wnarrUIHQK52102305 = wnarrUIHQK40823539;     wnarrUIHQK40823539 = wnarrUIHQK76851271;     wnarrUIHQK76851271 = wnarrUIHQK87662804;     wnarrUIHQK87662804 = wnarrUIHQK23640060;     wnarrUIHQK23640060 = wnarrUIHQK34402937;     wnarrUIHQK34402937 = wnarrUIHQK7454032;     wnarrUIHQK7454032 = wnarrUIHQK44842858;     wnarrUIHQK44842858 = wnarrUIHQK60467263;     wnarrUIHQK60467263 = wnarrUIHQK69552587;     wnarrUIHQK69552587 = wnarrUIHQK59264467;     wnarrUIHQK59264467 = wnarrUIHQK18475601;     wnarrUIHQK18475601 = wnarrUIHQK91271906;     wnarrUIHQK91271906 = wnarrUIHQK50534557;     wnarrUIHQK50534557 = wnarrUIHQK80827327;     wnarrUIHQK80827327 = wnarrUIHQK90140035;     wnarrUIHQK90140035 = wnarrUIHQK85242561;     wnarrUIHQK85242561 = wnarrUIHQK82447102;     wnarrUIHQK82447102 = wnarrUIHQK45864380;     wnarrUIHQK45864380 = wnarrUIHQK2786128;     wnarrUIHQK2786128 = wnarrUIHQK85722331;     wnarrUIHQK85722331 = wnarrUIHQK93175638;     wnarrUIHQK93175638 = wnarrUIHQK37840278;     wnarrUIHQK37840278 = wnarrUIHQK22229357;     wnarrUIHQK22229357 = wnarrUIHQK76552074;     wnarrUIHQK76552074 = wnarrUIHQK92220450;     wnarrUIHQK92220450 = wnarrUIHQK22007503;     wnarrUIHQK22007503 = wnarrUIHQK54083275;     wnarrUIHQK54083275 = wnarrUIHQK35532783;     wnarrUIHQK35532783 = wnarrUIHQK80744554;     wnarrUIHQK80744554 = wnarrUIHQK62025626;     wnarrUIHQK62025626 = wnarrUIHQK43898536;     wnarrUIHQK43898536 = wnarrUIHQK22705649;     wnarrUIHQK22705649 = wnarrUIHQK30593150;     wnarrUIHQK30593150 = wnarrUIHQK71244319;     wnarrUIHQK71244319 = wnarrUIHQK55714449;     wnarrUIHQK55714449 = wnarrUIHQK1878587;     wnarrUIHQK1878587 = wnarrUIHQK69552301;     wnarrUIHQK69552301 = wnarrUIHQK90728155;     wnarrUIHQK90728155 = wnarrUIHQK24556505;     wnarrUIHQK24556505 = wnarrUIHQK40081965;     wnarrUIHQK40081965 = wnarrUIHQK80066376;     wnarrUIHQK80066376 = wnarrUIHQK6453422;     wnarrUIHQK6453422 = wnarrUIHQK56526503;     wnarrUIHQK56526503 = wnarrUIHQK14632793;     wnarrUIHQK14632793 = wnarrUIHQK82106389;     wnarrUIHQK82106389 = wnarrUIHQK32384346;     wnarrUIHQK32384346 = wnarrUIHQK72736727;     wnarrUIHQK72736727 = wnarrUIHQK18737309;     wnarrUIHQK18737309 = wnarrUIHQK21631263;     wnarrUIHQK21631263 = wnarrUIHQK5021933;     wnarrUIHQK5021933 = wnarrUIHQK83624085;     wnarrUIHQK83624085 = wnarrUIHQK79305775;     wnarrUIHQK79305775 = wnarrUIHQK52067776;     wnarrUIHQK52067776 = wnarrUIHQK27903337;     wnarrUIHQK27903337 = wnarrUIHQK58926667;     wnarrUIHQK58926667 = wnarrUIHQK2983262;     wnarrUIHQK2983262 = wnarrUIHQK54621914;     wnarrUIHQK54621914 = wnarrUIHQK11110730;     wnarrUIHQK11110730 = wnarrUIHQK31419610;     wnarrUIHQK31419610 = wnarrUIHQK12395435;     wnarrUIHQK12395435 = wnarrUIHQK53370757;     wnarrUIHQK53370757 = wnarrUIHQK9310075;     wnarrUIHQK9310075 = wnarrUIHQK79722709;     wnarrUIHQK79722709 = wnarrUIHQK7526961;     wnarrUIHQK7526961 = wnarrUIHQK15365932;     wnarrUIHQK15365932 = wnarrUIHQK95769952;     wnarrUIHQK95769952 = wnarrUIHQK60678757;     wnarrUIHQK60678757 = wnarrUIHQK79290237;     wnarrUIHQK79290237 = wnarrUIHQK25112878;     wnarrUIHQK25112878 = wnarrUIHQK88261449;     wnarrUIHQK88261449 = wnarrUIHQK15690261;     wnarrUIHQK15690261 = wnarrUIHQK91718946;     wnarrUIHQK91718946 = wnarrUIHQK21307876;     wnarrUIHQK21307876 = wnarrUIHQK62704162;     wnarrUIHQK62704162 = wnarrUIHQK5655956;     wnarrUIHQK5655956 = wnarrUIHQK86722217;     wnarrUIHQK86722217 = wnarrUIHQK81313775;     wnarrUIHQK81313775 = wnarrUIHQK7596565;     wnarrUIHQK7596565 = wnarrUIHQK94445685;     wnarrUIHQK94445685 = wnarrUIHQK59836104;     wnarrUIHQK59836104 = wnarrUIHQK49270775;     wnarrUIHQK49270775 = wnarrUIHQK35345967;     wnarrUIHQK35345967 = wnarrUIHQK13901521;     wnarrUIHQK13901521 = wnarrUIHQK75722622;     wnarrUIHQK75722622 = wnarrUIHQK78401541;     wnarrUIHQK78401541 = wnarrUIHQK64592760;     wnarrUIHQK64592760 = wnarrUIHQK66920685;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void pmOCUbHBwp61554209() {     int BNifdFfyam89307802 = -957387372;    int BNifdFfyam10426590 = -918955116;    int BNifdFfyam2666539 = -4159662;    int BNifdFfyam15588679 = -426364459;    int BNifdFfyam21830507 = 28766391;    int BNifdFfyam38154401 = -217751093;    int BNifdFfyam91720374 = -169593301;    int BNifdFfyam87405156 = -236824870;    int BNifdFfyam87427624 = 69627256;    int BNifdFfyam57834772 = 35577982;    int BNifdFfyam62712015 = -702586569;    int BNifdFfyam10756853 = -711750336;    int BNifdFfyam80633849 = -526781851;    int BNifdFfyam94717057 = -578007950;    int BNifdFfyam77558816 = -938716238;    int BNifdFfyam21107184 = -353756149;    int BNifdFfyam37486156 = -247411080;    int BNifdFfyam29616476 = 15325262;    int BNifdFfyam75684567 = -152453736;    int BNifdFfyam85970314 = -241677936;    int BNifdFfyam45725965 = -353510413;    int BNifdFfyam24282482 = -521766563;    int BNifdFfyam72845910 = -931359391;    int BNifdFfyam85764254 = -769743913;    int BNifdFfyam54015847 = -383194782;    int BNifdFfyam79574901 = -926615380;    int BNifdFfyam97623977 = -574271006;    int BNifdFfyam86738777 = -1035899;    int BNifdFfyam97032596 = -649885869;    int BNifdFfyam94399741 = -722023176;    int BNifdFfyam26737908 = -31822052;    int BNifdFfyam3405572 = -508070391;    int BNifdFfyam83824798 = -108963874;    int BNifdFfyam45111019 = -765621965;    int BNifdFfyam69405003 = -787303539;    int BNifdFfyam79533147 = -677516991;    int BNifdFfyam15669070 = 3750473;    int BNifdFfyam30178176 = -71426894;    int BNifdFfyam40528581 = -949993487;    int BNifdFfyam42193217 = -506488452;    int BNifdFfyam82840645 = -46663257;    int BNifdFfyam24041175 = -657293834;    int BNifdFfyam86294746 = -51608091;    int BNifdFfyam61469745 = -613467456;    int BNifdFfyam11881803 = -149236921;    int BNifdFfyam30805930 = -826772525;    int BNifdFfyam42890547 = -386783511;    int BNifdFfyam15842995 = -667333258;    int BNifdFfyam50174355 = 19718975;    int BNifdFfyam98962331 = -527953128;    int BNifdFfyam78421517 = -896139387;    int BNifdFfyam64579412 = -544970512;    int BNifdFfyam2870908 = -821610781;    int BNifdFfyam368511 = -324220378;    int BNifdFfyam15828244 = 31839351;    int BNifdFfyam65025320 = -335620810;    int BNifdFfyam37580680 = -987595726;    int BNifdFfyam16902284 = -234415749;    int BNifdFfyam61572831 = 56830322;    int BNifdFfyam42255605 = -44618229;    int BNifdFfyam40530424 = -643480088;    int BNifdFfyam4981598 = -68557403;    int BNifdFfyam90372560 = -586939002;    int BNifdFfyam93027883 = -208349569;    int BNifdFfyam31096865 = -932599967;    int BNifdFfyam59306444 = -94516178;    int BNifdFfyam26932055 = -502786463;    int BNifdFfyam35522831 = -761159887;    int BNifdFfyam25312055 = -790704411;    int BNifdFfyam98025668 = -161199248;    int BNifdFfyam5438115 = -257506622;    int BNifdFfyam7307981 = -75984187;    int BNifdFfyam89087894 = -34681252;    int BNifdFfyam33491351 = -645965284;    int BNifdFfyam3129669 = -95014679;    int BNifdFfyam21684791 = -696216579;    int BNifdFfyam37987736 = -370158472;    int BNifdFfyam11376165 = -217891935;    int BNifdFfyam73882452 = -520506993;    int BNifdFfyam23209917 = -556422257;    int BNifdFfyam36684354 = -439831870;    int BNifdFfyam81780983 = -906937748;    int BNifdFfyam36564423 = 79245125;    int BNifdFfyam98070264 = -21932741;    int BNifdFfyam15978224 = -825883790;    int BNifdFfyam62158495 = -486851541;    int BNifdFfyam534664 = -686459611;    int BNifdFfyam83456287 = -784743496;    int BNifdFfyam29282775 = -697461317;    int BNifdFfyam4379683 = -351682730;    int BNifdFfyam41952467 = -689921265;    int BNifdFfyam98766785 = -761833779;    int BNifdFfyam68605345 = -28257216;    int BNifdFfyam98272976 = -805375258;    int BNifdFfyam1662794 = -863008365;    int BNifdFfyam77859048 = -978105854;    int BNifdFfyam33668614 = 29645168;    int BNifdFfyam93266862 = -843258523;    int BNifdFfyam30372881 = -680867490;    int BNifdFfyam52575358 = -957387372;     BNifdFfyam89307802 = BNifdFfyam10426590;     BNifdFfyam10426590 = BNifdFfyam2666539;     BNifdFfyam2666539 = BNifdFfyam15588679;     BNifdFfyam15588679 = BNifdFfyam21830507;     BNifdFfyam21830507 = BNifdFfyam38154401;     BNifdFfyam38154401 = BNifdFfyam91720374;     BNifdFfyam91720374 = BNifdFfyam87405156;     BNifdFfyam87405156 = BNifdFfyam87427624;     BNifdFfyam87427624 = BNifdFfyam57834772;     BNifdFfyam57834772 = BNifdFfyam62712015;     BNifdFfyam62712015 = BNifdFfyam10756853;     BNifdFfyam10756853 = BNifdFfyam80633849;     BNifdFfyam80633849 = BNifdFfyam94717057;     BNifdFfyam94717057 = BNifdFfyam77558816;     BNifdFfyam77558816 = BNifdFfyam21107184;     BNifdFfyam21107184 = BNifdFfyam37486156;     BNifdFfyam37486156 = BNifdFfyam29616476;     BNifdFfyam29616476 = BNifdFfyam75684567;     BNifdFfyam75684567 = BNifdFfyam85970314;     BNifdFfyam85970314 = BNifdFfyam45725965;     BNifdFfyam45725965 = BNifdFfyam24282482;     BNifdFfyam24282482 = BNifdFfyam72845910;     BNifdFfyam72845910 = BNifdFfyam85764254;     BNifdFfyam85764254 = BNifdFfyam54015847;     BNifdFfyam54015847 = BNifdFfyam79574901;     BNifdFfyam79574901 = BNifdFfyam97623977;     BNifdFfyam97623977 = BNifdFfyam86738777;     BNifdFfyam86738777 = BNifdFfyam97032596;     BNifdFfyam97032596 = BNifdFfyam94399741;     BNifdFfyam94399741 = BNifdFfyam26737908;     BNifdFfyam26737908 = BNifdFfyam3405572;     BNifdFfyam3405572 = BNifdFfyam83824798;     BNifdFfyam83824798 = BNifdFfyam45111019;     BNifdFfyam45111019 = BNifdFfyam69405003;     BNifdFfyam69405003 = BNifdFfyam79533147;     BNifdFfyam79533147 = BNifdFfyam15669070;     BNifdFfyam15669070 = BNifdFfyam30178176;     BNifdFfyam30178176 = BNifdFfyam40528581;     BNifdFfyam40528581 = BNifdFfyam42193217;     BNifdFfyam42193217 = BNifdFfyam82840645;     BNifdFfyam82840645 = BNifdFfyam24041175;     BNifdFfyam24041175 = BNifdFfyam86294746;     BNifdFfyam86294746 = BNifdFfyam61469745;     BNifdFfyam61469745 = BNifdFfyam11881803;     BNifdFfyam11881803 = BNifdFfyam30805930;     BNifdFfyam30805930 = BNifdFfyam42890547;     BNifdFfyam42890547 = BNifdFfyam15842995;     BNifdFfyam15842995 = BNifdFfyam50174355;     BNifdFfyam50174355 = BNifdFfyam98962331;     BNifdFfyam98962331 = BNifdFfyam78421517;     BNifdFfyam78421517 = BNifdFfyam64579412;     BNifdFfyam64579412 = BNifdFfyam2870908;     BNifdFfyam2870908 = BNifdFfyam368511;     BNifdFfyam368511 = BNifdFfyam15828244;     BNifdFfyam15828244 = BNifdFfyam65025320;     BNifdFfyam65025320 = BNifdFfyam37580680;     BNifdFfyam37580680 = BNifdFfyam16902284;     BNifdFfyam16902284 = BNifdFfyam61572831;     BNifdFfyam61572831 = BNifdFfyam42255605;     BNifdFfyam42255605 = BNifdFfyam40530424;     BNifdFfyam40530424 = BNifdFfyam4981598;     BNifdFfyam4981598 = BNifdFfyam90372560;     BNifdFfyam90372560 = BNifdFfyam93027883;     BNifdFfyam93027883 = BNifdFfyam31096865;     BNifdFfyam31096865 = BNifdFfyam59306444;     BNifdFfyam59306444 = BNifdFfyam26932055;     BNifdFfyam26932055 = BNifdFfyam35522831;     BNifdFfyam35522831 = BNifdFfyam25312055;     BNifdFfyam25312055 = BNifdFfyam98025668;     BNifdFfyam98025668 = BNifdFfyam5438115;     BNifdFfyam5438115 = BNifdFfyam7307981;     BNifdFfyam7307981 = BNifdFfyam89087894;     BNifdFfyam89087894 = BNifdFfyam33491351;     BNifdFfyam33491351 = BNifdFfyam3129669;     BNifdFfyam3129669 = BNifdFfyam21684791;     BNifdFfyam21684791 = BNifdFfyam37987736;     BNifdFfyam37987736 = BNifdFfyam11376165;     BNifdFfyam11376165 = BNifdFfyam73882452;     BNifdFfyam73882452 = BNifdFfyam23209917;     BNifdFfyam23209917 = BNifdFfyam36684354;     BNifdFfyam36684354 = BNifdFfyam81780983;     BNifdFfyam81780983 = BNifdFfyam36564423;     BNifdFfyam36564423 = BNifdFfyam98070264;     BNifdFfyam98070264 = BNifdFfyam15978224;     BNifdFfyam15978224 = BNifdFfyam62158495;     BNifdFfyam62158495 = BNifdFfyam534664;     BNifdFfyam534664 = BNifdFfyam83456287;     BNifdFfyam83456287 = BNifdFfyam29282775;     BNifdFfyam29282775 = BNifdFfyam4379683;     BNifdFfyam4379683 = BNifdFfyam41952467;     BNifdFfyam41952467 = BNifdFfyam98766785;     BNifdFfyam98766785 = BNifdFfyam68605345;     BNifdFfyam68605345 = BNifdFfyam98272976;     BNifdFfyam98272976 = BNifdFfyam1662794;     BNifdFfyam1662794 = BNifdFfyam77859048;     BNifdFfyam77859048 = BNifdFfyam33668614;     BNifdFfyam33668614 = BNifdFfyam93266862;     BNifdFfyam93266862 = BNifdFfyam30372881;     BNifdFfyam30372881 = BNifdFfyam52575358;     BNifdFfyam52575358 = BNifdFfyam89307802;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void gsoLpDDknh98199183() {     int hxATgtyHdL11694920 = -591791335;    int hxATgtyHdL94774090 = -867498171;    int hxATgtyHdL31435817 = -243813506;    int hxATgtyHdL30595367 = -432771991;    int hxATgtyHdL20004761 = -562290610;    int hxATgtyHdL53037518 = -78359471;    int hxATgtyHdL83876113 = 8419392;    int hxATgtyHdL63039015 = -707421515;    int hxATgtyHdL84590755 = -914568783;    int hxATgtyHdL49598356 = -807163351;    int hxATgtyHdL253875 = -699210970;    int hxATgtyHdL66659802 = -962495630;    int hxATgtyHdL47642030 = -45499171;    int hxATgtyHdL37331810 = -5716707;    int hxATgtyHdL14294093 = -566193951;    int hxATgtyHdL65363096 = -735423692;    int hxATgtyHdL87309508 = -524690900;    int hxATgtyHdL35592892 = -436417429;    int hxATgtyHdL16966199 = -287555861;    int hxATgtyHdL64486598 = -494312389;    int hxATgtyHdL46609072 = -344997721;    int hxATgtyHdL88097700 = 73467124;    int hxATgtyHdL76139233 = -7439983;    int hxATgtyHdL12264042 = -233736771;    int hxATgtyHdL89556094 = -4753906;    int hxATgtyHdL67877896 = -270509604;    int hxATgtyHdL44713398 = -331857498;    int hxATgtyHdL92650227 = -889490353;    int hxATgtyHdL3925157 = -994142988;    int hxATgtyHdL3556921 = -642319042;    int hxATgtyHdL71028712 = -113138204;    int hxATgtyHdL60946762 = -60141746;    int hxATgtyHdL64863468 = -638307369;    int hxATgtyHdL4499707 = 10251185;    int hxATgtyHdL45634368 = -851059962;    int hxATgtyHdL21226016 = -992912367;    int hxATgtyHdL9108782 = -829197479;    int hxATgtyHdL83804276 = 60947634;    int hxATgtyHdL88836712 = -251039518;    int hxATgtyHdL62378932 = -642712481;    int hxATgtyHdL11598017 = -549840923;    int hxATgtyHdL12549566 = -415202525;    int hxATgtyHdL91844938 = 60861325;    int hxATgtyHdL60913864 = -610831105;    int hxATgtyHdL79865068 = 41619601;    int hxATgtyHdL38906212 = 90998379;    int hxATgtyHdL55187945 = -965193763;    int hxATgtyHdL60441669 = -423868249;    int hxATgtyHdL44634260 = 70002277;    int hxATgtyHdL96046077 = -669973387;    int hxATgtyHdL87290734 = -151215664;    int hxATgtyHdL38430670 = -975789794;    int hxATgtyHdL81185311 = -493522011;    int hxATgtyHdL60655056 = -599901187;    int hxATgtyHdL51590111 = -108864764;    int hxATgtyHdL23597220 = -565258459;    int hxATgtyHdL18634857 = -760058189;    int hxATgtyHdL19171776 = 89923265;    int hxATgtyHdL41039273 = -328018085;    int hxATgtyHdL52126865 = -191781007;    int hxATgtyHdL8324121 = -746501973;    int hxATgtyHdL91225886 = -102090256;    int hxATgtyHdL59113858 = -713278527;    int hxATgtyHdL81033835 = -172249742;    int hxATgtyHdL78569644 = -594025148;    int hxATgtyHdL39307113 = -539069224;    int hxATgtyHdL1796334 = -224188262;    int hxATgtyHdL43142324 = 44249644;    int hxATgtyHdL91697442 = -154656746;    int hxATgtyHdL93068076 = -573281585;    int hxATgtyHdL56254315 = -906226213;    int hxATgtyHdL3505232 = -485638535;    int hxATgtyHdL46756180 = -85377911;    int hxATgtyHdL54587267 = -644843380;    int hxATgtyHdL52888581 = -944471467;    int hxATgtyHdL34059507 = -929795197;    int hxATgtyHdL96252762 = -987394202;    int hxATgtyHdL15225369 = -396608878;    int hxATgtyHdL32398973 = -175356372;    int hxATgtyHdL50649882 = 4247714;    int hxATgtyHdL12689952 = -305315841;    int hxATgtyHdL84271728 = -907989250;    int hxATgtyHdL48015968 = -859492630;    int hxATgtyHdL7879080 = -224169602;    int hxATgtyHdL16266187 = -391103379;    int hxATgtyHdL32598043 = -137348411;    int hxATgtyHdL79761451 = -566619735;    int hxATgtyHdL4208413 = 61593818;    int hxATgtyHdL52909595 = -880884051;    int hxATgtyHdL22037149 = -185801503;    int hxATgtyHdL2591160 = -132854179;    int hxATgtyHdL89937006 = -819120744;    int hxATgtyHdL42765004 = -611034281;    int hxATgtyHdL36709848 = 40741488;    int hxATgtyHdL54054811 = -896210509;    int hxATgtyHdL20372131 = -347750668;    int hxATgtyHdL53435707 = -701923998;    int hxATgtyHdL10811103 = -766888934;    int hxATgtyHdL82344220 = 83194042;    int hxATgtyHdL40557956 = -591791335;     hxATgtyHdL11694920 = hxATgtyHdL94774090;     hxATgtyHdL94774090 = hxATgtyHdL31435817;     hxATgtyHdL31435817 = hxATgtyHdL30595367;     hxATgtyHdL30595367 = hxATgtyHdL20004761;     hxATgtyHdL20004761 = hxATgtyHdL53037518;     hxATgtyHdL53037518 = hxATgtyHdL83876113;     hxATgtyHdL83876113 = hxATgtyHdL63039015;     hxATgtyHdL63039015 = hxATgtyHdL84590755;     hxATgtyHdL84590755 = hxATgtyHdL49598356;     hxATgtyHdL49598356 = hxATgtyHdL253875;     hxATgtyHdL253875 = hxATgtyHdL66659802;     hxATgtyHdL66659802 = hxATgtyHdL47642030;     hxATgtyHdL47642030 = hxATgtyHdL37331810;     hxATgtyHdL37331810 = hxATgtyHdL14294093;     hxATgtyHdL14294093 = hxATgtyHdL65363096;     hxATgtyHdL65363096 = hxATgtyHdL87309508;     hxATgtyHdL87309508 = hxATgtyHdL35592892;     hxATgtyHdL35592892 = hxATgtyHdL16966199;     hxATgtyHdL16966199 = hxATgtyHdL64486598;     hxATgtyHdL64486598 = hxATgtyHdL46609072;     hxATgtyHdL46609072 = hxATgtyHdL88097700;     hxATgtyHdL88097700 = hxATgtyHdL76139233;     hxATgtyHdL76139233 = hxATgtyHdL12264042;     hxATgtyHdL12264042 = hxATgtyHdL89556094;     hxATgtyHdL89556094 = hxATgtyHdL67877896;     hxATgtyHdL67877896 = hxATgtyHdL44713398;     hxATgtyHdL44713398 = hxATgtyHdL92650227;     hxATgtyHdL92650227 = hxATgtyHdL3925157;     hxATgtyHdL3925157 = hxATgtyHdL3556921;     hxATgtyHdL3556921 = hxATgtyHdL71028712;     hxATgtyHdL71028712 = hxATgtyHdL60946762;     hxATgtyHdL60946762 = hxATgtyHdL64863468;     hxATgtyHdL64863468 = hxATgtyHdL4499707;     hxATgtyHdL4499707 = hxATgtyHdL45634368;     hxATgtyHdL45634368 = hxATgtyHdL21226016;     hxATgtyHdL21226016 = hxATgtyHdL9108782;     hxATgtyHdL9108782 = hxATgtyHdL83804276;     hxATgtyHdL83804276 = hxATgtyHdL88836712;     hxATgtyHdL88836712 = hxATgtyHdL62378932;     hxATgtyHdL62378932 = hxATgtyHdL11598017;     hxATgtyHdL11598017 = hxATgtyHdL12549566;     hxATgtyHdL12549566 = hxATgtyHdL91844938;     hxATgtyHdL91844938 = hxATgtyHdL60913864;     hxATgtyHdL60913864 = hxATgtyHdL79865068;     hxATgtyHdL79865068 = hxATgtyHdL38906212;     hxATgtyHdL38906212 = hxATgtyHdL55187945;     hxATgtyHdL55187945 = hxATgtyHdL60441669;     hxATgtyHdL60441669 = hxATgtyHdL44634260;     hxATgtyHdL44634260 = hxATgtyHdL96046077;     hxATgtyHdL96046077 = hxATgtyHdL87290734;     hxATgtyHdL87290734 = hxATgtyHdL38430670;     hxATgtyHdL38430670 = hxATgtyHdL81185311;     hxATgtyHdL81185311 = hxATgtyHdL60655056;     hxATgtyHdL60655056 = hxATgtyHdL51590111;     hxATgtyHdL51590111 = hxATgtyHdL23597220;     hxATgtyHdL23597220 = hxATgtyHdL18634857;     hxATgtyHdL18634857 = hxATgtyHdL19171776;     hxATgtyHdL19171776 = hxATgtyHdL41039273;     hxATgtyHdL41039273 = hxATgtyHdL52126865;     hxATgtyHdL52126865 = hxATgtyHdL8324121;     hxATgtyHdL8324121 = hxATgtyHdL91225886;     hxATgtyHdL91225886 = hxATgtyHdL59113858;     hxATgtyHdL59113858 = hxATgtyHdL81033835;     hxATgtyHdL81033835 = hxATgtyHdL78569644;     hxATgtyHdL78569644 = hxATgtyHdL39307113;     hxATgtyHdL39307113 = hxATgtyHdL1796334;     hxATgtyHdL1796334 = hxATgtyHdL43142324;     hxATgtyHdL43142324 = hxATgtyHdL91697442;     hxATgtyHdL91697442 = hxATgtyHdL93068076;     hxATgtyHdL93068076 = hxATgtyHdL56254315;     hxATgtyHdL56254315 = hxATgtyHdL3505232;     hxATgtyHdL3505232 = hxATgtyHdL46756180;     hxATgtyHdL46756180 = hxATgtyHdL54587267;     hxATgtyHdL54587267 = hxATgtyHdL52888581;     hxATgtyHdL52888581 = hxATgtyHdL34059507;     hxATgtyHdL34059507 = hxATgtyHdL96252762;     hxATgtyHdL96252762 = hxATgtyHdL15225369;     hxATgtyHdL15225369 = hxATgtyHdL32398973;     hxATgtyHdL32398973 = hxATgtyHdL50649882;     hxATgtyHdL50649882 = hxATgtyHdL12689952;     hxATgtyHdL12689952 = hxATgtyHdL84271728;     hxATgtyHdL84271728 = hxATgtyHdL48015968;     hxATgtyHdL48015968 = hxATgtyHdL7879080;     hxATgtyHdL7879080 = hxATgtyHdL16266187;     hxATgtyHdL16266187 = hxATgtyHdL32598043;     hxATgtyHdL32598043 = hxATgtyHdL79761451;     hxATgtyHdL79761451 = hxATgtyHdL4208413;     hxATgtyHdL4208413 = hxATgtyHdL52909595;     hxATgtyHdL52909595 = hxATgtyHdL22037149;     hxATgtyHdL22037149 = hxATgtyHdL2591160;     hxATgtyHdL2591160 = hxATgtyHdL89937006;     hxATgtyHdL89937006 = hxATgtyHdL42765004;     hxATgtyHdL42765004 = hxATgtyHdL36709848;     hxATgtyHdL36709848 = hxATgtyHdL54054811;     hxATgtyHdL54054811 = hxATgtyHdL20372131;     hxATgtyHdL20372131 = hxATgtyHdL53435707;     hxATgtyHdL53435707 = hxATgtyHdL10811103;     hxATgtyHdL10811103 = hxATgtyHdL82344220;     hxATgtyHdL82344220 = hxATgtyHdL40557956;     hxATgtyHdL40557956 = hxATgtyHdL11694920;}
// Junk Finished
