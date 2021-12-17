#include <algorithm>
#include "visuals.hpp"
#include "../config/options.hpp"
#include "../../helpers/math.hpp"
#include "../../helpers/utils.hpp"
#include "grenade_prediction.hpp"
#include "../legit/backtrack.hpp"
#include "../legit/aimbot.hpp"
#include "../menu/menu.hpp"
//--------------------------------------------------------------------------------
RECT GetBBox(C_BaseEntity* ent) {
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable) return rect;

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
	for (int i = 0; i < 8; i++) Math::VectorTransform(points[i], trans, pointsTransformed[i]);

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i])) return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x) left = screen_points[i].x;
		if (top < screen_points[i].y) top = screen_points[i].y;
		if (right < screen_points[i].x) right = screen_points[i].x;
		if (bottom > screen_points[i].y) bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}
//--------------------------------------------------------------------------------
Visuals::Visuals() {
	InitializeCriticalSection(&cs);
}
//--------------------------------------------------------------------------------
Visuals::~Visuals() {
	DeleteCriticalSection(&cs);
}
//--------------------------------------------------------------------------------
void Visuals::Render() { }
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl) {
	if (pl->IsDormant() || !pl->IsAlive()) return false;

	ctx.pl = pl;
	ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (!ctx.is_enemy && Cfg.esp_enemies_only) return false;

	ctx.clr = ctx.is_enemy ?
		(ctx.is_visible ? Color(Cfg.color_esp_enemy_visible) : Color(Cfg.color_esp_enemy_occluded)) :
		(ctx.is_visible ? Color(Cfg.color_esp_ally_visible) : Color(Cfg.color_esp_ally_occluded));

	if (!ctx.is_visible && Cfg.esp_visible_only) return false;

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	head.z += 6;

	if (!Math::WorldToScreen(head, ctx.head_pos) || !Math::WorldToScreen(origin, ctx.feet_pos)) return false;

	auto h = fabs(ctx.head_pos.y - ctx.feet_pos.y);
	auto w = h / 2.f;

	ctx.bbox.left = static_cast<long>(ctx.feet_pos.x - w * 0.45f);
	ctx.bbox.right = static_cast<long>(ctx.bbox.left + w);
	ctx.bbox.bottom = static_cast<long>(ctx.feet_pos.y);
	ctx.bbox.top = static_cast<long>(ctx.head_pos.y);

	return true;
}
//--------------------------------------------------------------------------------
void Visuals::FireGameEvent(IGameEvent* event) {
	if (!strcmp(event->GetName(), "player_hurt")) {
		int attacker = event->GetInt("attacker");
		if (g_EngineClient->GetPlayerForUserID(attacker) == g_EngineClient->GetLocalPlayer()) {
			flHurtTime = g_GlobalVars->realtime;
		}
	}
}
//--------------------------------------------------------------------------------
int Visuals::GetEventDebugID(void) {
	return EVENT_DEBUG_ID_INIT;
}
//--------------------------------------------------------------------------------
void Visuals::DrawLine(float x1, float y1, float x2, float y2, Color color, float size) {
	g_VGuiSurface->DrawSetColor(color);

	if (size == 1.f) g_VGuiSurface->DrawLine(x1, y1, x2, y2);
	else g_VGuiSurface->DrawFilledRect(x1 - (size / 2.f), y1 - (size / 2.f), x2 + (size / 2.f), y2 + (size / 2.f));
}
//--------------------------------------------------------------------------------
void Visuals::DrawBox(float x1, float y1, float x2, float y2, Color clr, float size) {
	DrawLine(x1, y1, x2, y1, clr, size);
	DrawLine(x1, y2, x2, y2, clr, size);
	DrawLine(x1, y1, x1, y2, clr, size);
	DrawLine(x2, y1, x2, y2 + 1, clr, size);
}
//--------------------------------------------------------------------------------
void Visuals::DrawBoxEdges(float x1, float y1, float x2, float y2, Color clr, float edge_size, float size) {
	if (fabs(x1 - x2) < (edge_size * 2)) edge_size = fabs(x1 - x2) / 4.f;
	
	DrawLine(x1, y1, x1, y1 + edge_size + (0.5f * edge_size), clr, size);
	DrawLine(x2, y1, x2, y1 + edge_size + (0.5f * edge_size), clr, size);
	DrawLine(x1, y2, x1, y2 - edge_size - (0.5f * edge_size), clr, size);
	DrawLine(x2, y2, x2, y2 - edge_size - (0.5f * edge_size), clr, size);
	DrawLine(x1, y1, x1 + edge_size, y1, clr, size);
	DrawLine(x2, y1, x2 - edge_size, y1, clr, size);
	DrawLine(x1, y2, x1 + edge_size, y2, clr, size);
	DrawLine(x2, y2, x2 - edge_size, y2, clr, size);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSkeleton(C_BaseEntity* ent) {
	auto model = ent->GetModel();
	if (ent && !model) return;

	studiohdr_t* pStudioModel = g_MdlInfo->GetStudiomodel(model);

	if (pStudioModel) {
		static matrix3x4_t pBoneToWorldOut[128];

		if (ent->SetupBones(pBoneToWorldOut, 128, 256, g_GlobalVars->curtime)) {
			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->GetBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1) continue;

				Vector vBonePos1;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1)) continue;

				Vector vBonePos2;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2)) continue;

				Visuals::Get().DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, Color(Cfg.color_esp_player_skeleton));
			}
		}
	}
}
//--------------------------------------------------------------------------------
#define FLAG_MACRO std::pair<std::string, Color> 
#define FLAG(string, color) vecFlags.push_back(FLAG_MACRO(string, color)) 
  //--------------------------------------------------------------------------------
void Visuals::Player::DrawFlags() {
	if (!g_LocalPlayer || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame()) return;
	std::vector<FLAG_MACRO> vecFlags;

	int alpha = ctx.clr.a();

	if (Cfg.esp_flags) {
		if (Cfg.esp_player_flags_selected[0] && ctx.pl->m_iAccount())
			FLAG(std::string("$").append(std::to_string(ctx.pl->m_iAccount())), Color(50, 250, 50));
		if (Cfg.esp_player_flags_selected[1] && ctx.pl->IsFlashed())
			FLAG("Flashed", Color::White);
		if (Cfg.esp_player_flags_selected[2] && ctx.pl->m_ArmorValue() > 0)
			FLAG(ctx.pl->GetArmorName(), Color::White);
		if (Cfg.esp_player_flags_selected[3] && ctx.pl->m_bIsScoped())
			FLAG("Scoped", Color::White);
		if (Cfg.esp_player_flags_selected[4] && ctx.pl->m_bIsDefusing())
			FLAG("Defusing", Color::White);
		if (Cfg.esp_player_flags_selected[5] && ctx.pl->HasC4())
			FLAG("C4", Color::White);
		if (Cfg.esp_player_flags_selected[6] && ctx.pl->m_bHasDefuser())
			FLAG("Defusers", Color::White);
	}

	int offset = 0;
	for (auto Text : vecFlags) {
		Render::Get().RenderText(Text.first, ctx.bbox.right + 4 + ((ctx.pl->m_ArmorValue() > 0 && Cfg.esp_player_armour) ? 5 : 0), ctx.bbox.top - 2 + offset, 10.f, Text.second, false, false); //14
		offset += 12;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox() {
	float edge_size = 25.f;
	switch (Cfg.esp_player_boxes_type) {
	case 0: {
		Visuals::Get().DrawBox(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, ctx.clr, 1);
		Visuals::Get().DrawBox(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color::Black, 1);
		Visuals::Get().DrawBox(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color::Black, 1);
	} break;
	case 1: {
		Render::Get().RenderBox(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, ctx.clr, 1, Cfg.boxes_rounding);
		Render::Get().RenderBox(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color::Black, 1, Cfg.boxes_rounding);
		Render::Get().RenderBox(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color::Black, 1, Cfg.boxes_rounding);
	} break;
	case 2: {
		if (ctx.pl != g_LocalPlayer)
			edge_size = 4000.f / Math::VectorDistance(g_LocalPlayer->m_vecOrigin(), ctx.pl->m_vecOrigin());

		Visuals::Get().DrawBoxEdges(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, ctx.clr, edge_size, 1);
		Visuals::Get().DrawBoxEdges(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color::Black, edge_size, 1);
		Visuals::Get().DrawBoxEdges(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color::Black, edge_size, 1);
	} break;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName() {
	player_info_t info = ctx.pl->GetPlayerInfo();

	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, info.szName);
	Render::Get().RenderText(info.szName, ctx.feet_pos.x - sz.x / 2, ctx.head_pos.y - sz.y - 2, 14.f, Color(255, 255, 255), false, false);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth() {
	int HealthValue = ctx.pl->m_iHealth();
	HealthValue = std::clamp(HealthValue, 0, 100);

	auto Multiplier = 12 / 360.f;
	Multiplier *= (HealthValue / 10.f) - 1;
	Color HealthColor = Color::FromHSB(Multiplier, 1, 1);

	float Height = (ctx.bbox.bottom - ctx.bbox.top) * float(HealthValue) / 100.f;

	Render::Get().FilledRectange(ctx.bbox.left - 7, ctx.bbox.top - 1, ctx.bbox.left - 2, ctx.bbox.bottom + 1, Color(0, 0, 0, 150));
	Render::Get().FilledRectange(ctx.bbox.left - 6, ctx.bbox.top, ctx.bbox.left - 3, ctx.bbox.top + Height, HealthColor);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour() {
	auto armour = ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);

	int height = (((box_h * armour) / 100));

	int x = ctx.bbox.right + 4;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Render::Get().RenderBox(x, y - 1, x + w, y + h + 2, Color::Black, 1.f, true);
	Render::Get().RenderBox(x + 1, y, x + w - 1, y + height + 1, Color(0, 50, 255, 255), 1.f, true);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeaponName() {
	auto weapon = ctx.pl->m_hActiveWeapon().Get();

	if (!weapon->GetCSWeaponData()) return;

	auto weapon_data = weapon->GetCSWeaponData();
	if (!weapon_data) return;

	auto text = weapon_data->szHudName + 7;
	auto size = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, text);
	Render::Get().RenderText(text, ctx.feet_pos.x, ctx.feet_pos.y, 14.f, Color::White, true, false, g_pDefaultFont);
}
//--------------------------------------------------------------------------------
void Visuals::RenderCrosshair() {
	int w, h;
	g_EngineClient->GetScreenSize(w, h);

	int cx = w / 2;
	int cy = h / 2;
	Render::Get().RenderLine(cx - 25, cy, cx + 25, cy, Color(Cfg.color_esp_crosshair));
	Render::Get().RenderLine(cx, cy - 25, cx, cy + 25, Color(Cfg.color_esp_crosshair));
} 
//--------------------------------------------------------------------------------
void Visuals::RenderWeapon(C_BaseCombatWeapon* ent) {
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C') name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr) name = start + 6;

		return name;
	};

	if (ent->m_hOwnerEntity().IsValid()) return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0) return;

	Render::Get().RenderBox(bbox, Color(Cfg.color_esp_weapons));

	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);
	if (!name) return;

	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;

	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderDefuseKit(C_BaseEntity* ent) {
	if (ent->m_hOwnerEntity().IsValid()) return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0) return;

	Render::Get().RenderBox(bbox, Color(Cfg.color_esp_defuse));

	auto name = "Defuse Kit";
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;
	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::Radar() {
	for (auto i = 1; i <= g_GlobalVars->maxClients; i++) {
		const auto pEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));

		if (!pEntity) continue;
		if (!pEntity->IsEnemy()) continue;

		pEntity->m_bSpotted() = true;
	}
}
//--------------------------------------------------------------------------------
std::string string_format(const std::string fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2;
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]);
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n) n += abs(final_n - n + 1);
		else break;
	}
	return std::string(formatted.get());
}
//--------------------------------------------------------------------------------
void Visuals::RenderPlantedC4(C_BaseEntity* ent) { /*to-do*/ }
//--------------------------------------------------------------------------------
void Visuals::RenderItemEsp(C_BaseEntity* ent) {

	if (!(ent->GetClientClass()->m_ClassID == ClassId_CPhysPropAmmoBox ||
		ent->GetClientClass()->m_ClassID == ClassId_CPhysPropLootCrate ||
		ent->GetClientClass()->m_ClassID == ClassId_CPhysPropRadarJammer ||
		ent->GetClientClass()->m_ClassID == ClassId_CPhysPropWeaponUpgrade ||
		ent->GetClientClass()->m_ClassID == ClassId_CDrone ||
		ent->GetClientClass()->m_ClassID == ClassId_CDronegun ||
		ent->GetClientClass()->m_ClassID == ClassId_CItem_Healthshot ||
		ent->GetClientClass()->m_ClassID == ClassId_CItemCash))
		return;

	std::string itemstr = "Undefined";
	const model_t* itemModel = ent->GetModel();
	if (!itemModel) return;
	studiohdr_t* hdr = g_MdlInfo->GetStudiomodel(itemModel);
	if (!hdr) return;
	itemstr = hdr->szName;

	if (itemstr.find("case_pistol") != std::string::npos)
		itemstr = "Pistol Case";
	else if (itemstr.find("case_light_weapon") != std::string::npos)
		itemstr = "Light Case";
	else if (itemstr.find("case_heavy_weapon") != std::string::npos)
		itemstr = "Heavy Case";
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

	auto bbox = GetBBox(ent);
	if (bbox.right == 0 || bbox.bottom == 0) return;
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, itemstr.c_str());
	int w = bbox.right - bbox.left;

	Render::Get().RenderBox(bbox, Color(Cfg.color_esp_item));
	Render::Get().RenderText(itemstr, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color(255, 255, 255));
}
//--------------------------------------------------------------------------------
void Visuals::ThirdPerson() {
	if (!g_LocalPlayer) return;

	if (Cfg.misc_thirdperson && g_LocalPlayer->IsAlive()) {
		if (!g_Input->m_fCameraInThirdPerson) g_Input->m_fCameraInThirdPerson = true;
		
		float dist = Cfg.misc_thirdperson_dist;

		QAngle* view = g_LocalPlayer->GetVAngles();
		trace_t tr;
		Ray_t ray;

		Vector desiredCamOffset = Vector(cos(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(-view->pitch)) * dist
		);

		ray.Init(g_LocalPlayer->GetEyePos(), (g_LocalPlayer->GetEyePos() - desiredCamOffset));
		CTraceFilter traceFilter;
		traceFilter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		Vector diff = g_LocalPlayer->GetEyePos() - tr.endpos;

		float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));

		bool horOK = distance2D > (dist - 2.0f);
		bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

		float cameraDistance;

		if (horOK && vertOK) cameraDistance = dist;		
		else {
			if (vertOK) cameraDistance = distance2D * 0.95f;		
			else cameraDistance = abs(diff.z) * 0.95f;		
		}
		g_Input->m_fCameraInThirdPerson = true;

		g_Input->m_vecCameraOffset.z = cameraDistance;
	}
	else g_Input->m_fCameraInThirdPerson = false;
}
//--------------------------------------------------------------------------------
int GetDistance(C_BaseEntity* from, C_BaseEntity* to) {
	return from->GetRenderOrigin().DistTo(to->GetRenderOrigin());
}
//--------------------------------------------------------------------------------
void Visuals::AddToDrawList() {
	if (Cfg.esp_grenade_prediction) GrenadePrediction::draw();
	if (Cfg.esp_hitmarker)			DrawHitmarker();
	if (Cfg.esp_sounds)				RenderSounds();
	if (Cfg.esp_crosshair)			RenderCrosshair();

	auto drawAngleLine = [&](const Vector& origin, const Vector& w2sOrigin, const float& angle, const char* text, Color clr) {
		Vector forward;
		Math::AngleVectors(QAngle(0.0f, angle, 0.0f), forward);
		float AngleLinesLength = 30.0f;

		Vector w2sReal;
		if (Math::WorldToScreen(origin + forward * AngleLinesLength, w2sReal)) {
			Render::Get().RenderLine(w2sOrigin.x, w2sOrigin.y, w2sReal.x, w2sReal.y, Color::White, 1.0f);
			Render::Get().RenderBoxFilled(w2sReal.x - 5.0f, w2sReal.y - 5.0f, w2sReal.x + 5.0f, w2sReal.y + 5.0f, Color::White);
			Render::Get().RenderText(text, w2sReal.x, w2sReal.y - 5.0f, 14.0f, clr, true, true);
		}
	};

	extern float real_angle;
	extern float view_angle;

	if (Cfg.esp_angle_lines) {
		if (!g_LocalPlayer && !g_LocalPlayer->IsAlive()) return;
		Vector w2sOrigin;
		if (Math::WorldToScreen(g_LocalPlayer->m_vecOrigin(), w2sOrigin)) {
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, view_angle, "viewangles", Color(0.937f, 0.713f, 0.094f, 1.0f));
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, g_LocalPlayer->m_flLowerBodyYawTarget(), "lby", Color(0.0f, 0.0f, 1.0f, 1.0f));
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, real_angle, "real", Color(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
		auto entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity) continue;
		if (entity == g_LocalPlayer) continue;

		if (i <= g_GlobalVars->maxClients) {
			auto player = Player();
			if (player.Begin((C_BasePlayer*)entity)) {
				player.DrawFlags();
				if (Cfg.esp_player_skeleton)  player.RenderSkeleton(entity);
				if (Cfg.esp_player_boxes)     player.RenderBox();
				if (Cfg.esp_player_weapons)   player.RenderWeaponName();
				if (Cfg.esp_player_names)     player.RenderName();
				if (Cfg.esp_player_health)    player.RenderHealth();
				if (Cfg.esp_player_armour)    player.RenderArmour();
				if (Cfg.misc_radar)			  Radar();
				if (Cfg.misc_engine_radar)	  Radar();
			}
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::RenderSpectatorList() {
	int specs = 0;
	std::string spect = "";

	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
		int localIndex = g_EngineClient->GetLocalPlayer();
		C_BasePlayer* pLocalEntity = C_BasePlayer::GetPlayerByIndex(localIndex);
		if (pLocalEntity) {
			for (int i = 0; i < g_EngineClient->GetMaxClients(); i++) {
				C_BasePlayer* pBaseEntity = C_BasePlayer::GetPlayerByIndex(i);
				if (!pBaseEntity)										     continue;
				if (pBaseEntity->m_iHealth() > 0)							 continue;
				if (pBaseEntity == pLocalEntity)							 continue;
				if (pBaseEntity->IsDormant())								 continue;
				if (pBaseEntity->m_hObserverTarget() != pLocalEntity)		 continue;
				player_info_t pInfo;
				g_EngineClient->GetPlayerInfo(pBaseEntity->EntIndex(), &pInfo);
				if (pInfo.ishltv) continue;

				spect += pInfo.szName;
				spect += "\n";
				specs++;
			}
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	static auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	if (ImGui::Begin("Spectator List", nullptr, ImVec2(0, 0), 1.0f, flags)) {
		ImGui::PushFont(MainCaps);
		if (ImGui::BeginChild("Child1", ImVec2(-1, 25), false, flags)) {
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 3), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + -3), ImColor(Cfg.color_menu[0], Cfg.color_menu[1], Cfg.color_menu[2]));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 3), ImVec2(p.x + ImGui::GetWindowWidth() / 2, p.y + -3), ImColor(0, 0, 0, 125), ImColor(0, 0, 0, 15), ImColor(0, 0, 0, 15), ImColor(0, 0, 0, 125));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x + ImGui::GetWindowWidth() / 2, p.y + 3), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + -3), ImColor(0, 0, 0, 15), ImColor(0, 0, 0, 125), ImColor(0, 0, 0, 125), ImColor(0, 0, 0, 15));
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + 2), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + 2), ImColor(0, 0, 0, 150));

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("[ spectators ]").x) / 2);
			ImGui::TextColored(ImColor(255, 255, 255), "[");
			ImGui::SameLine();
			ImGui::Text("spectators");
			ImGui::SameLine();
			ImGui::TextColored(ImColor(255, 255, 255), "]");
		}
		ImGui::EndChild();

		if (specs > 0) spect += "\n";

		ImVec2 size = ImGui::CalcTextSize(spect.c_str());

		ImGui::SetWindowSize(ImVec2(200, 45 + size.y));

		if (ImGui::BeginChild("Child2", ImVec2(-1, -1), false, flags))
			ImGui::TextColored(ImColor(255, 255, 255), spect.c_str());

		ImGui::EndChild();
		ImGui::PopFont();
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
//--------------------------------------------------------------------------------
void Visuals::DrawFOV() {
	auto pWeapon = g_LocalPlayer->m_hActiveWeapon();
	if (!pWeapon) return;

	auto settings = Cfg.legitbot_items[pWeapon->m_Item().m_iItemDefinitionIndex()];

	if (settings.enabled) {

		float fov = static_cast<float>(g_LocalPlayer->GetFOV());

		int w, h;
		g_EngineClient->GetScreenSize(w, h);

		Vector2D screenSize = Vector2D(w, h);
		Vector2D center = screenSize * 0.5f;

		float ratio = screenSize.x / screenSize.y;
		float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(fov * 0.5f)));

		float radiusFOV = tanf(DEG2RAD(LegitBot::Get().GetFov())) / tanf(screenFov) * center.x;

		Render::Get().RenderCircle(center.x, center.y, radiusFOV, 32, Color(0, 0, 0, 100));

		if (settings.silent) {
			float silentRadiusFOV = tanf(DEG2RAD(settings.silent_fov)) / tanf(screenFov) * center.x;
			Render::Get().RenderCircle(center.x, center.y, silentRadiusFOV, 32, Color(255, 25, 10, 100));
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::DrawHitmarker() {
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive()) return;

	auto curtime = g_GlobalVars->realtime;
	float lineSize = 8.f;
	if (flHurtTime + .05f >= curtime) {
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		g_EngineClient->GetScreenSize(screenSizeX, screenSizeY);

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;

		Color bg = Color(0, 0, 0, 50);
		Color white = Color::White;

		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), bg, 2.5f);
		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), bg, 2.5f);
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), bg, 2.5f);
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), bg, 2.5f);

		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), Color(Cfg.color_esp_hitmarker));
		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), Color(Cfg.color_esp_hitmarker));
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), Color(Cfg.color_esp_hitmarker));
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), Color(Cfg.color_esp_hitmarker));
	}
}
//--------------------------------------------------------------------------------
void Visuals::RenderSounds() {
	UpdateSounds();

	for (auto& [entIndex, sound] : m_Sounds) {
		if (sound.empty()) continue;

		for (auto& info : sound) {
			if (info.soundTime + Cfg.esp_sounds_time < g_GlobalVars->realtime)
				info.alpha -= g_GlobalVars->frametime;

			if (info.alpha <= 0.0f) continue;

			float deltaTime = g_GlobalVars->realtime - info.soundTime;

			auto factor = deltaTime / Cfg.esp_sounds_time;
			if (factor > 1.0f) factor = 1.0f;

			float radius = Cfg.esp_sounds_radius * factor;
			Color color = Color(Cfg.color_esp_sounds[0], Cfg.color_esp_sounds[1], Cfg.color_esp_sounds[2], info.alpha);

			Render::Get().RenderCircle3D(info.soundPos, radius, color);
		}

		while (!sound.empty()) {
			auto& back = sound.back();
			if (back.alpha <= 0.0f) sound.pop_back();
			else break;
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::UpdateSounds() {
	sounds.RemoveAll();
	g_EngineSound->GetActiveSounds(sounds);
	if (sounds.Count() < 1) return;

	Vector eye_pos = g_LocalPlayer->GetEyePos();
	for (int i = 0; i < sounds.Count(); ++i) {
		SndInfo_t& sound = sounds.Element(i);
		if (sound.m_nSoundSource < 1) continue;

		C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(sound.m_nSoundSource);
		if (!player) continue;

		if (player->m_hOwnerEntity().IsValid() && player->IsWeapon()) 
			player = (C_BasePlayer*)player->m_hOwnerEntity().Get();
		
		if (!player->IsPlayer() || !player->IsAlive()) continue;

		if ((C_BasePlayer*)g_LocalPlayer == player || (Cfg.esp_enemies_only && g_LocalPlayer->m_iTeamNum() == player->m_iTeamNum())) continue;

		if (player->m_vecOrigin().DistTo(g_LocalPlayer->m_vecOrigin()) > 900) continue;

		auto& player_sound = m_Sounds[player->EntIndex()];
		if (player_sound.size() > 0) {
			bool should_break = false;
			for (const auto& snd : player_sound) {
				if (snd.guid == sound.m_nGuid) {
					should_break = true;
					break;
				}
			}

			if (should_break) continue;
		}

		SoundInfo_t& snd = player_sound.emplace_back();
		snd.guid = sound.m_nGuid;
		snd.soundPos = *sound.m_pOrigin;
		snd.soundTime = g_GlobalVars->realtime;
		snd.alpha = 1.0f;
	}
}
//--------------------------------------------------------------------------------
void Visuals::NoSmoke() {
	static bool set = true;
	static std::vector<const char*> smoke_materials = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!Cfg.other_no_smoke) {
		if (set) {
			for (auto material_name : smoke_materials) {
				IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}
			set = false;
		}
		return;
	}

	set = true;
	for (auto material_name : smoke_materials) {
		IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	}
}
