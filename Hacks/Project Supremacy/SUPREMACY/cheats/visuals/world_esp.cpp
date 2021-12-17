#include "world_esp.h"
void worldesp::paint_traverse() {
	if (!g_ctx.available())
		return;

	for (int i = 1; i < g_csgo.m_entitylist()->GetHighestEntityIndex(); i++) {
		auto e = static_cast<entity_t *>(g_csgo.m_entitylist()->GetClientEntity(i));

		if (!e)
			continue;

		if (e->IsDormant())
			continue;

		Box box; if (!util::get_bbox_world(e, box))
			continue;

		if (g_cfg.esp.projectiles)
			grenades((weapon_t *)e, box);

		if (g_cfg.esp.dropped_weapons)
			weapons((weapon_t *)e, box);
		//if (g_cfg.esp.planted_bomb)
		//	PlantedBomb((CCSBomb*)e, box);
	}
}

void worldesp::PlantedBomb(entity_t * ent, Box box) {
	Color clr = g_cfg.esp.planted_bomb_color;
	CCSBomb* bomb = nullptr;
	if (ent->GetClientClass()->m_ClassID == ClassId::CPlantedC4)
	{
		bomb = (CCSBomb*)ent;
	}
	else
		return;
	int bombTimer = std::ceil(bomb->m_flC4Blow() - g_csgo.m_globals()->m_curtime);

	if (bombTimer < 0.f)
		return;

	//Render::Get().RenderBox(bbox, clr);
	render::get().rect(box.x, box.y, box.w, box.h, clr);
	std::string timer = std::to_string(bombTimer);

	auto sz = render::get().text_width(fonts[rolexTEXT], timer.c_str());
	render::get().text(fonts[rolexTEXT], box.x + (box.w / 2), box.y, clr, HFONT_CENTERED_X, timer.c_str());
}

void worldesp::grenades(weapon_t * m_nade, Box box) {
	const model_t* model = m_nade->GetModel();
	if (!model)
		return;

	studiohdr_t* hdr = g_csgo.m_modelinfo()->GetStudiomodel(model);
	if (!hdr)
		return;

	std::string entity_name = hdr->szName;

	switch (m_nade->GetClientClass()->m_ClassID) {
	case 9:
		entity_name = entity_name[16] == 's' ? "Flashbang" : "HE-Grenade";
		break;
	case 134:
		entity_name = "Smoke";
		break;
	case 98:
		entity_name = "Molotov";
		break;
	case 41:
		entity_name = "Decoy";
		break;
	default:
		return;
	}

	Color color = g_cfg.esp.projectiles_color;

	render::get().text(fonts[rolexTEXT], box.x + (box.w / 2), box.y, color, HFONT_CENTERED_X, entity_name.c_str());
}
void worldesp::weapons(weapon_t * e, Box box) {
	if (e->GetClientClass()->m_ClassID != (int)ClassId::CBaseWeaponWorldModel && ((strstr(e->GetClientClass()->m_pNetworkName, "Weapon") || e->GetClientClass()->m_ClassID == (int)ClassId::CDEagle || e->GetClientClass()->m_ClassID == (int)ClassId::CAK47)))
	{
		char * name = e->get_name();

		Color color = g_cfg.esp.dropped_weapons_color;
		player_t* owner = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntityFromHandle(e->m_hOwnerEntity()));
		auto w = render::get().text_width(fonts[rolexTEXT], name);
		if (!owner)
			render::get().text(fonts[rolexTEXT], box.x + (box.w / 2) - (w / 2), box.y + box.h + 5, color, HFONT_CENTERED_X, name);
	}
}