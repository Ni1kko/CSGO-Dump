#include "nightmode.h"

bool executed = false;

ConVar * sv_skyname = nullptr;
std::string fallback_skybox = "";

std::vector<MaterialBackup> materials;
std::vector<MaterialBackup> skyboxes;

void nightmode::clear_stored_materials() {
	fallback_skybox = "";
	sv_skyname = nullptr;
	materials.clear();
	skyboxes.clear();
}

void nightmode::modulate(MaterialHandle_t i, IMaterial *material, bool backup = false) {
	if (strstr(material->GetTextureGroupName(), "World")) {
		if (backup) materials.push_back(MaterialBackup(i, material));

		material->ColorModulate(0.15f, 0.15f, 0.15f);
	}
	else if (strstr(material->GetTextureGroupName(), "StaticProp")) {
		if (backup)
			materials.push_back(MaterialBackup(i, material));

		material->ColorModulate(0.4f, 0.4f, 0.4f);
	}
}

void nightmode::apply() {
	if (executed || !g_csgo.m_cvarspoofer()->ready()) {
		return;
	}

	executed = true;

	if (!sv_skyname) {
		sv_skyname = g_csgo.m_cvar()->FindVar("sv_skyname");
		sv_skyname->m_nFlags &= ~FCVAR_CHEAT;
	}

	fallback_skybox = sv_skyname->GetString();
	if (g_cfg.esp.skybox == 0)
	{
		sv_skyname->SetValue("cs_tibet");
	}
	if (g_cfg.esp.skybox == 1)
	{
		sv_skyname->SetValue("cs_baggage_skybox_");
	}
	if (g_cfg.esp.skybox == 2)
	{
		sv_skyname->SetValue("embassy");
	}
	if (g_cfg.esp.skybox == 3)
	{
		sv_skyname->SetValue("italy");
	}
	if (g_cfg.esp.skybox == 4)
	{
		sv_skyname->SetValue("jungle");
	}
	if (g_cfg.esp.skybox == 5)
	{
		sv_skyname->SetValue("office");
	}
	if (g_cfg.esp.skybox == 6)
	{
		sv_skyname->SetValue("sky_cs15_daylight01_hdr");
	}
	if (g_cfg.esp.skybox == 7)
	{
		sv_skyname->SetValue("vertigoblue_hdr");
	}
	if (g_cfg.esp.skybox == 8)
	{
		sv_skyname->SetValue("sky_cs15_daylight02_hdr");
	}
	if (g_cfg.esp.skybox == 9)
	{
		sv_skyname->SetValue("vertigo");
	}
	if (g_cfg.esp.skybox == 10)
	{
		sv_skyname->SetValue("sky_day02_05_hdr");
	}
	if (g_cfg.esp.skybox == 11)
	{
		sv_skyname->SetValue("nukeblank");
	}
	if (g_cfg.esp.skybox == 12)
	{
		sv_skyname->SetValue("sky_venice");
	}
	if (g_cfg.esp.skybox == 13)
	{
		sv_skyname->SetValue("sky_cs15_daylight03_hdr");
	}
	if (g_cfg.esp.skybox == 14)
	{
		sv_skyname->SetValue("sky_cs15_daylight04_hdr");
	}
	if (g_cfg.esp.skybox == 15)
	{
		sv_skyname->SetValue("sky_csgo_cloudy01");
	}
	if (g_cfg.esp.skybox == 16)
	{
		sv_skyname->SetValue("sky_csgo_night02");
	}
	if (g_cfg.esp.skybox == 17)
	{
		sv_skyname->SetValue("sky_csgo_night02b");
	}
	if (g_cfg.esp.skybox == 18)
	{
		sv_skyname->SetValue("sky_csgo_night_flat");
	}
	if (g_cfg.esp.skybox == 19)
	{
		sv_skyname->SetValue("sky_dust");
	}
	if (g_cfg.esp.skybox == 20)
	{
		sv_skyname->SetValue("vietnam");
	}
	if (g_cfg.esp.skybox == 21)
	{
		sv_skyname->SetValue("amethyst");
	}
	if (g_cfg.esp.skybox == 22)
	{
		sv_skyname->SetValue("sky_descent");
	}
	if (g_cfg.esp.skybox == 23)
	{
		sv_skyname->SetValue("clear_night_sky");
	}
	if (g_cfg.esp.skybox == 24)
	{
		sv_skyname->SetValue("otherworld");
	}
	if (g_cfg.esp.skybox == 25)
	{
		sv_skyname->SetValue("cloudynight");
	}
	if (g_cfg.esp.skybox == 26)
	{
		sv_skyname->SetValue("dreamyocean");
	}
	if (g_cfg.esp.skybox == 27)
	{
		sv_skyname->SetValue("grimmnight");
	}
	if (g_cfg.esp.skybox == 28)
	{
		sv_skyname->SetValue("sky051");
	}
	if (g_cfg.esp.skybox == 29)
	{
		sv_skyname->SetValue("sky081");
	}
	if (g_cfg.esp.skybox == 30)
	{
		sv_skyname->SetValue("sky091");
	}
	if (g_cfg.esp.skybox == 31)
	{
		sv_skyname->SetValue("sky561");
	}
	static ConVar* mat_fullbright = g_csgo.m_cvar()->FindVar("mat_fullbright");
	if (g_cfg.esp.fullbright)
	{
		mat_fullbright->SetValue(1);
	}
	else
	{
		mat_fullbright->SetValue(0);
	}
	g_csgo.m_cvarspoofer()->get("r_drawspecificstaticprop")->SetInt(0);

	if (materials.size()) {
		for (int i = 0; i < (int)materials.size(); i++)
			modulate(materials[i].handle, materials[i].material);

		return;
	}

	materials.clear();

	for (MaterialHandle_t i = g_csgo.m_materialsystem()->FirstMaterial(); i != g_csgo.m_materialsystem()->InvalidMaterial(); i = g_csgo.m_materialsystem()->NextMaterial(i)) {
		IMaterial* material = g_csgo.m_materialsystem()->GetMaterial(i);

		if (!material || material->IsErrorMaterial()) {
			continue;
		}

		if (material->GetReferenceCount() <= 0) {
			continue;
		}

		modulate(i, material, true);
	}
}

void nightmode::remove() {
	if (!executed || !g_csgo.m_cvarspoofer()->ready()) {
		return;
	}

	executed = false;
	g_cfg.esp.nightmode = false;

	if (sv_skyname) {
		sv_skyname->SetValue(fallback_skybox.c_str());
	}

	g_csgo.m_cvarspoofer()->get("r_drawspecificstaticprop")->SetInt(1);

	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].material->GetReferenceCount() <= 0) continue;

		materials[i].restore();
		materials[i].material->Refresh();
	}

	materials.clear();

	sv_skyname = nullptr;
	fallback_skybox = "";
}