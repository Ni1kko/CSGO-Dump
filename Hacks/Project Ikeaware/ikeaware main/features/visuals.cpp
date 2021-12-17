#include "visuals.hpp"

#include "../options.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"


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

Visuals::Visuals()
{
	InitializeCriticalSection(&cs);
}

Visuals::~Visuals() {
	DeleteCriticalSection(&cs);
}

//--------------------------------------------------------------------------------
void Visuals::Render() {
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl)
{
	if (pl->IsDormant() || !pl->IsAlive())
		return false;

	ctx.pl = pl;
	ctx.is_enemy = pl->IsEnemy();
	ctx.is_local = (pl == g_LocalPlayer);
	ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (ctx.is_visible)
	{
		if (ctx.is_enemy && !g_Options.esp.player_enemies)
			return false;

		if (!ctx.is_enemy && !ctx.is_local && !g_Options.esp.player_teammates)
			return false;

		if (ctx.is_local && !g_Options.esp.player_local)
			return false;
	}
	else
	{
		if (ctx.is_enemy && !g_Options.esp.player_enemies_occluded)
			return false;

		if (!ctx.is_enemy && !ctx.is_local && !g_Options.esp.player_teammates_occluded)
			return false;

		if (ctx.is_local && !g_Options.esp.player_local_occluded)
			return false;
	}
	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	if (!Math::WorldToScreen(head, ctx.head_pos) ||
		!Math::WorldToScreen(origin, ctx.feet_pos))
		return false;

	ctx.bbox = GetBBox(pl);

	std::swap(ctx.bbox.top, ctx.bbox.bottom);

	return !(!ctx.bbox.left || !ctx.bbox.top || !ctx.bbox.right || !ctx.bbox.bottom);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox() {
	auto is_enemy = ctx.pl->IsEnemy();
	auto is_local = (ctx.pl == g_LocalPlayer);

	if (is_enemy && !g_Options.esp.player_boxes_enemies)
		return;

	if (!is_enemy && !is_local && !g_Options.esp.player_boxes_teammates)
		return;

	if (is_local && !g_Options.esp.player_boxes_local)
		return;

	g_Render->RenderBoxByType(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color(5, 5, 5, 170), 1);
	g_Render->RenderBoxByType(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color(5, 5, 5, 170), 1);
	g_Render->RenderBoxByType(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, Color::Green, 1);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName()
{
	auto is_enemy = ctx.pl->IsEnemy();
	auto is_local = (ctx.pl == g_LocalPlayer);

	if (is_enemy && !g_Options.esp.player_names_enemies)
		return;

	if (!is_enemy && !is_local && !g_Options.esp.player_names_teammates)
		return;

	if (is_local && !g_Options.esp.player_names_local)
		return;

	player_info_t info = ctx.pl->GetPlayerInfo();

	auto sz = g_VisualsFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, info.szName);

	g_Render->RenderText(info.szName, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left - sz.x) / 2, int(ctx.bbox.top - sz.y - 1), 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth()
{
	auto is_enemy = ctx.pl->IsEnemy();
	auto is_local = (ctx.pl == g_LocalPlayer);

	if (is_enemy && !g_Options.esp.player_health_enemies)
		return;

	if (!is_enemy && !is_local && !g_Options.esp.player_health_teammates)
		return;

	if (is_local && !g_Options.esp.player_health_local)
		return;

	auto  hp = ctx.pl->m_iHealth();

	if (hp > 100)
		hp = 100;

	const int box_h = abs(ctx.bbox.bottom - ctx.bbox.top);
	const int off = 6;

	const int height = hp * box_h / 100;

	const int green = int(hp * 2.55f);
	const int red = 255 - green;

	const int x = ctx.bbox.left - off;
	const int y = ctx.bbox.top;
	const int w = 4;
	const int h = box_h;

	g_Render->RenderBoxFilled(x, y - 1, x + w, y + h + 1, Color(5, 5, 5, 170));
	g_Render->RenderBoxFilled(x + 1, y + h - height, x + w - 1, y + h, Color(red, green, 0, 255));
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour()
{
	auto is_enemy = ctx.pl->IsEnemy();
	auto is_local = (ctx.pl == g_LocalPlayer);

	if (is_enemy && !g_Options.esp.player_armour_enemies)
		return;

	if (!is_enemy && !is_local && !g_Options.esp.player_armour_teammates)
		return;

	if (is_local && !g_Options.esp.player_armour_local)
		return;

	auto armour = ctx.pl->m_ArmorValue();

	if (armour > 100)
		armour = 100;

	if (armour <= 0)
		return;

	const int box_h = abs(ctx.bbox.bottom - ctx.bbox.top);
	const int off = 2;

	const int height = armour * box_h / 100;

	const int x = ctx.bbox.right + off;
	const int y = ctx.bbox.top;
	const int w = 4;
	const int h = box_h;

	g_Render->RenderBoxFilled(x, y - 1, x + w, y + h + 1, Color(5, 5, 5, 170));
	g_Render->RenderBoxFilled(x + 1, y + h - height, x + w - 1, y + h, Color(70, 130, 180, 255));
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeaponName()
{
	auto is_enemy = ctx.pl->IsEnemy();
	auto is_local = (ctx.pl == g_LocalPlayer);

	if (is_enemy && !g_Options.esp.player_weapons_enemies)
		return;

	if (!is_enemy && !is_local && !g_Options.esp.player_weapons_teammates)
		return;

	if (is_local && !g_Options.esp.player_weapons_local)
		return;

	auto weapon = ctx.pl->m_hActiveWeapon().Get();

	if (!weapon) return;

	auto text = weapon->GetCSWeaponData()->szWeaponName + 7;
	auto sz = g_VisualsFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, text);
	g_Render->RenderText(text, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left - sz.x) / 2, ctx.bbox.bottom + 1, 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderWeapon(C_BaseCombatWeapon* ent)
{
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
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

	g_Render->RenderBox(bbox, g_Options.colors.esp.weapons);


	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);

	auto sz = g_VisualsFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;


	g_Render->RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderDefuseKit(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	g_Render->RenderBox(bbox, g_Options.colors.esp.defuse);

	auto name = "Defuse Kit";
	auto sz = g_VisualsFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;
	g_Render->RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderPlantedC4(C_BaseEntity* ent)
{
	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;


	g_Render->RenderBox(bbox, g_Options.colors.esp.c4);


	int bombTimer = std::ceil(ent->m_flC4Blow() - g_GlobalVars->curtime);
	std::string timer = std::to_string(bombTimer);

	auto name = (bombTimer < 0.f) ? "Bomb" : timer;
	auto sz = g_VisualsFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, name.c_str());
	int w = bbox.right - bbox.left;

	g_Render->RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderItemEsp(C_BaseEntity* ent)
{
	std::string itemstr = "Undefined";
	const model_t * itemModel = ent->GetModel();
	if (!itemModel)
		return;
	studiohdr_t * hdr = g_MdlInfo->GetStudiomodel(itemModel);
	if (!hdr)
		return;
	itemstr = hdr->szName;
	if (ent->GetClientClass()->m_ClassID == ClassId->CBumpMine)
		itemstr = "";
	else if (itemstr.find("case_pistol") != std::string::npos)
		itemstr = "Pistol Case";
	else if (itemstr.find("case_light_weapon") != std::string::npos)
		itemstr = "Weapon Case";
	else if (itemstr.find("case_heavy_weapon") != std::string::npos)
		itemstr = "Weapon Case";
	else if (itemstr.find("case_explosive") != std::string::npos)
		itemstr = "Explosive Case";
	else if (itemstr.find("case_tools") != std::string::npos)
		itemstr = "Tools Case";
	else if (itemstr.find("random") != std::string::npos)
		itemstr = "Airdrop";
	else if (itemstr.find("dz_armor_helmet") != std::string::npos)
		itemstr = "Full Armor";
	else if (itemstr.find("dz_helmet") != std::string::npos)
		itemstr = "Helmet";
	else if (itemstr.find("dz_armor") != std::string::npos)
		itemstr = "Armor";
	else if (itemstr.find("upgrade_tablet") != std::string::npos)
		itemstr = "Tablet Upgrade";
	else if (itemstr.find("briefcase") != std::string::npos)
		itemstr = "Briefcase";
	else if (itemstr.find("parachutepack") != std::string::npos)
		itemstr = "Parachute";
	else if (itemstr.find("dufflebag") != std::string::npos)
		itemstr = "Cash Dufflebag";
	else if (itemstr.find("ammobox") != std::string::npos)
		itemstr = "Ammobox";
	else if (itemstr.find("dronegun") != std::string::npos)
		itemstr = "Turrel";
	else if (itemstr.find("exojump") != std::string::npos)
		itemstr = "Exojump";
	else if (itemstr.find("healthshot") != std::string::npos)
		itemstr = "Healthshot";
	else {
		/*May be you will search some missing items..*/
		/*static std::vector<std::string> unk_loot;
		if (std::find(unk_loot.begin(), unk_loot.end(), itemstr) == unk_loot.end()) {
			Utils::ConsolePrint(itemstr.c_str());
			unk_loot.push_back(itemstr);
		}*/
		return;
	}
	
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	auto sz = g_VisualsFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, itemstr.c_str());
	int w = bbox.right - bbox.left;

	g_Render->RenderBox(bbox, g_Options.colors.esp.item);
	g_Render->RenderText(itemstr, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void DrawWave(Vector loc, float radius, Color color)
{
	static float Step = IM_PI * 3.0f / 40;
	Vector prev;
	for (float lat = 0; lat <= IM_PI * 3.0f; lat += Step)
	{
		float sin1 = sin(lat);
		float cos1 = cos(lat);
		float sin3 = sin(0.0);
		float cos3 = cos(0.0);

		Vector point1;
		point1 = Vector(sin1 * cos3, cos1, sin1 * sin3) * radius;
		Vector point3 = loc;
		Vector Out;
		point3 += point1;

		if (Math::WorldToScreen(point3, Out))
		{
			if (lat > 0.000)
				g_Render->RenderLine(prev.x, prev.y, Out.x, Out.y, color);
		}
		prev = Out;
	}
}

void Visuals::RenderSoundEsp()
{
	for (auto& i : g_SoundEsp->sound) {
		if (i.time + 800 <= GetTickCount()) {
			i.time = 0;
			i.origin = { 0.f, 0.f, 0.f };
		}
		else {
			if (i.enemy)
			{
				if (!g_Options.esp.sound_enemies)
				{
					continue;
				}
				else
				{
					DrawWave(i.origin, 12.f, g_Options.colors.esp.sounds_enemies);
				}
			}

			if (!i.enemy && !i.local)
			{
				if (!g_Options.esp.sound_teammates)
				{
					continue;
				}
				else
				{
					DrawWave(i.origin, 12.f, g_Options.colors.esp.sounds_teammates);
				}
			}

			if (i.local)
			{
				if (!g_Options.esp.sound_local)
				{
					continue;
				}
				else
				{
					DrawWave(i.origin, 12.f, g_Options.colors.esp.sounds_local);
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::AddToDrawList() {

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
		auto entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;
		
		if (entity == g_LocalPlayer && !g_Input->m_fCameraInThirdPerson)
			continue;

		if (i < 65) {
			auto player = Player();
			if (player.Begin((C_BasePlayer*)entity)) {
				player.RenderBox();
				player.RenderWeaponName();
				player.RenderName();
				player.RenderHealth();
				player.RenderArmour();
			}
		}
		else if (g_Options.esp.dropped_weapons && entity->IsWeapon())
			RenderWeapon(static_cast<C_BaseCombatWeapon*>(entity));
		else if (g_Options.esp.dropped_weapons && entity->IsDefuseKit())
			RenderDefuseKit(entity);
		else if (entity->IsPlantedC4() && g_Options.esp.planted_c4)
			RenderPlantedC4(entity);
		else if (entity->IsLoot() && g_Options.esp.items)
			RenderItemEsp(entity);
	}
	RenderSoundEsp();
}
