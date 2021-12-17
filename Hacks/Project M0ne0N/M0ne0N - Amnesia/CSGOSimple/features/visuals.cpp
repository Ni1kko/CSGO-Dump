#include <algorithm>

#include "visuals.hpp"

#include "../options.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"

#include "grenade_prediction.hpp"
#include "backtrack.hpp"
#include "aimbot.hpp"

RECT GetBBox(C_BaseEntity* ent) {
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

Visuals::Visuals() {
	InitializeCriticalSection(&cs);
}

Visuals::~Visuals() {
	DeleteCriticalSection(&cs);
}

//--------------------------------------------------------------------------------
void Visuals::Render() {
}
//--------------------------------------------------------------------------------
bool Visuals::Player::Begin(C_BasePlayer* pl) {
	if (pl->IsDormant() || !pl->IsAlive())
		return false;

	ctx.pl = pl;
	ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (!ctx.is_enemy && g_Options.esp_enemies_only)
		return false;

	ctx.clr = ctx.is_enemy ?
		(ctx.is_visible ? Color(g_Options.color_esp_enemy_visible) : Color(g_Options.color_esp_enemy_occluded)) :
		(ctx.is_visible ? Color(g_Options.color_esp_ally_visible) : Color(g_Options.color_esp_ally_occluded));

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	head.z += 6;

	if (!Math::WorldToScreen(head, ctx.head_pos) ||
		!Math::WorldToScreen(origin, ctx.feet_pos))
		return false;

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

	if (size == 1.f)
		g_VGuiSurface->DrawLine(x1, y1, x2, y2);
	else
		g_VGuiSurface->DrawFilledRect(x1 - (size / 2.f), y1 - (size / 2.f), x2 + (size / 2.f), y2 + (size / 2.f));
}

void Visuals::DrawBox(float x1, float y1, float x2, float y2, Color clr, float size) {
	DrawLine(x1, y1, x2, y1, clr, size);
	DrawLine(x1, y2, x2, y2, clr, size);
	DrawLine(x1, y1, x1, y2, clr, size);
	DrawLine(x2, y1, x2, y2 + 1, clr, size);
}
//--------------------------------------------------------------------------------
void Visuals::DrawBoxEdges(float x1, float y1, float x2, float y2, Color clr, float edge_size, float size) {
	if (fabs(x1 - x2) < (edge_size * 2)) {
		//x2 = x1 + fabs(x1 - x2);
		edge_size = fabs(x1 - x2) / 4.f;
	}

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
void Visuals::RenderSounds() {
	UpdateSounds();

	auto Add3DCircle = [](const Vector& position, Color color, float radius) {
		float precision = 24.0f;
		if (radius >= 60.0f)
			precision = 48.0f;
		else if (radius >= 30.0f)
			precision = 36.0f;

		const float step = DirectX::XM_2PI / precision;

		for (float a = 0.f; a < DirectX::XM_2PI; a += step) {
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
			Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

			Vector start2d, end2d;
			if (!Math::WorldToScreen(start, start2d) || !Math::WorldToScreen(end, end2d))
				return;

			Render::Get().RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
		}
	};


	for (auto&[entIndex, sound] : m_Sounds) {
		if (sound.empty())
			continue;

		for (auto& info : sound) {
			if (info.soundTime + g_Options.esp_sounds_time < g_GlobalVars->realtime)
				info.alpha -= g_GlobalVars->frametime;

			if (info.alpha <= 0.0f)
				continue;

			float deltaTime = g_GlobalVars->realtime - info.soundTime;

			auto factor = deltaTime / g_Options.esp_sounds_time;
			if (factor > 1.0f)
				factor = 1.0f;

			float radius = g_Options.esp_sounds_radius * factor;
			Color color = Color(g_Options.color_esp_sounds[0], g_Options.color_esp_sounds[1], g_Options.color_esp_sounds[2],
				info.alpha);

			Add3DCircle(info.soundPos, color, radius);
		}

		while (!sound.empty()) {
			auto& back = sound.back();
			if (back.alpha <= 0.0f)
				sound.pop_back();
			else
				break;
		}
	}
}
//--------------------------------------------------------------------------------
void Visuals::UpdateSounds() {
	CUtlVector< SndInfo_t > sounds;
	g_EngineSound->GetActiveSounds(sounds);
	if (sounds.Count() < 1)
		return;

	Vector eye_pos = g_LocalPlayer->GetEyePos();
	for (int i = 0; i < sounds.Count(); ++i) {
		SndInfo_t& sound = sounds.Element(i);
		if (sound.m_nSoundSource < 1)
			continue;

		C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(sound.m_nSoundSource);
		if (!player)
			continue;

		if (player->m_hOwnerEntity().IsValid() && player->IsWeapon()) {
			player = (C_BasePlayer*)player->m_hOwnerEntity().Get();
		}

		if (!player->IsPlayer() || !player->IsAlive())
			continue;

		if ((C_BasePlayer*)g_LocalPlayer == player || (g_Options.esp_enemies_only && g_LocalPlayer->m_iTeamNum() == player->m_iTeamNum()))
			continue;

		auto& player_sound = m_Sounds[player->EntIndex()];
		if (player_sound.size() > 0) {
			bool should_break = false;
			for (const auto& snd : player_sound) {
				if (snd.guid == sound.m_nGuid) {
					should_break = true;
					break;
				}
			}

			if (should_break)
				continue;
		}

		SoundInfo_t& snd = player_sound.emplace_back();
		snd.guid = sound.m_nGuid;
		snd.soundPos = *sound.m_pOrigin;
		snd.soundTime = g_GlobalVars->realtime;
		snd.alpha = 1.0f;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSkeleton(C_BaseEntity* ent) {
	auto model = ent->GetModel();
	if (ent && !model)
		return;

	studiohdr_t * pStudioModel = g_MdlInfo->GetStudiomodel(model);

	if (pStudioModel) {
		static matrix3x4_t pBoneToWorldOut[128];

		if (ent->SetupBones(pBoneToWorldOut, 128, 256, g_GlobalVars->curtime)) {
			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->GetBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (!Math::WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
					continue;

				Visuals::Get().DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, Color::White);
			}
		}
	}
}

void Visuals::Player::RenderBacktrack() {
	Vector localEyePos = g_LocalPlayer->GetEyePos();
	QAngle angles;
	int tick_count = -1;
	float best_fov = 180.0f;
	for (auto& node : g_Backtrack.data) {
		auto& cur_data = node.second;
		if (cur_data.empty())
			return;

		for (auto& bd : cur_data) {
			float deltaTime = g_Backtrack.correct_time - (g_GlobalVars->curtime - bd.simTime);
			if (std::fabsf(deltaTime) > g_Aimbot.settings.backtrack_time)
				continue;

			Vector w2s;
			if (!Math::WorldToScreen(bd.hitboxPos, w2s))
				continue;

			Render::Get().RenderBoxFilled(w2s.x - 1.0f, w2s.y - 1.0f, w2s.x + 1.0f, w2s.y + 1.0f, Color::White);
		}
	}

}

void Visuals::Player::RenderBox() {
	switch (g_Options.esp_player_boxes_type) {
	case 0:
		Visuals::Get().DrawBox(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, ctx.clr, 1);
		Visuals::Get().DrawBox(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color::Black, 1);
		Visuals::Get().DrawBox(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color::Black, 1);
		break;

	case 1:
		float edge_size = 25.f;

		if (ctx.pl != g_LocalPlayer)
			edge_size = 4000.f / Math::VectorDistance(g_LocalPlayer->m_vecOrigin(), ctx.pl->m_vecOrigin());

		Visuals::Get().DrawBoxEdges(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, ctx.clr, edge_size, 1);
		Visuals::Get().DrawBoxEdges(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color::Black, edge_size, 1);
		Visuals::Get().DrawBoxEdges(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color::Black, edge_size, 1);

		break;
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName() {
	player_info_t info = ctx.pl->GetPlayerInfo();

	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, info.szName);

	Render::Get().RenderText(info.szName, ctx.feet_pos.x - sz.x / 2, ctx.head_pos.y - sz.y - 2, 14.f, Color(255, 255, 255));
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth() {
	auto  hp = ctx.pl->m_iHealth();
	float box_w = (float)fabs(ctx.bbox.right - ctx.bbox.left);
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	float off = 7;

	int height = (box_h * hp) / 100;

	int green = int(hp * 2.55f);
	int red = 255 - green;

	int x = ctx.bbox.left - off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	Render::Get().RenderBox(x, y - 1, x + w, y + h + 2, Color::Black, 1.f, true);
	Render::Get().RenderBox(x + 1, y, x + w - 1, y + height + 1, Color(red, green, 0, 255), 1.f, true);
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

	if (!weapon) return;

	auto weapon_data = weapon->GetCSWeaponData();
	if (!weapon_data)
		return;

	auto text = weapon_data->szHudName + 7;
	auto size = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, text);
	Render::Get().RenderText(text, ctx.feet_pos.x, ctx.feet_pos.y, 14.f, Color::White, true, g_pDefaultFont);
}
//--------------------------------------------------------------------------------
void Visuals::RenderWeapon(C_BaseCombatWeapon* ent) {
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

	Render::Get().RenderBox(bbox, Color(g_Options.color_esp_weapons));

	auto name = clean_item_name(ent->GetClientClass()->m_pNetworkName);
	if (!name)
		return;

	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;

	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color(g_Options.color_esp_weapons));
}
//--------------------------------------------------------------------------------
void Visuals::RenderDefuseKit(C_BaseEntity* ent) {
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	//Render::Get().RenderBox(bbox, Config::ESP::defuse_color);

	auto name = "Defuse Kit";
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;
	Render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderPlantedC4(C_BaseEntity* ent) {
	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	int bombTimer = std::ceil(ent->m_flC4Blow() - g_GlobalVars->curtime);
	std::string timer = std::to_string(bombTimer);

	auto name = (bombTimer < 0.f) ? "Bomb" : timer;
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name.c_str());
	int w = bbox.right - bbox.left;

	//Render::Get().RenderBox(bbox, Config::ESP::planted_c4_color);
	Render::Get().RenderText(name, ImVec2((bbox.left + w / 2.f) - sz.x / 2.f, bbox.bottom + 1), 14.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderItemEsp(C_BaseEntity* ent) {
	std::string itemstr = "Undefined";
	const model_t * itemModel = ent->GetModel();
	if (!itemModel)
		return;
	studiohdr_t * hdr = g_MdlInfo->GetStudiomodel(itemModel);
	if (!hdr)
		return;
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
	if (bbox.right == 0 || bbox.bottom == 0)
		return;
	auto sz = g_pDefaultFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, itemstr.c_str());
	int w = bbox.right - bbox.left;

	//Render::Get().RenderBox(bbox, g_Options.color_esp_item);
	Render::Get().RenderText(itemstr, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 14.f, Color(255, 255, 255));
}
//--------------------------------------------------------------------------------
void Visuals::ThirdPerson() {
	if (!g_LocalPlayer)
		return;

	if (g_Options.misc_thirdperson && g_LocalPlayer->IsAlive()) {
		if (!g_Input->m_fCameraInThirdPerson) {
			g_Input->m_fCameraInThirdPerson = true;
		}

		float dist = g_Options.misc_thirdperson_dist;

		QAngle *view = g_LocalPlayer->GetVAngles();
		trace_t tr;
		Ray_t ray;

		Vector desiredCamOffset = Vector(cos(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(-view->pitch)) * dist
		);

		//cast a ray from the Current camera Origin to the Desired 3rd person Camera origin
		ray.Init(g_LocalPlayer->GetEyePos(), (g_LocalPlayer->GetEyePos() - desiredCamOffset));
		CTraceFilter traceFilter;
		traceFilter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		Vector diff = g_LocalPlayer->GetEyePos() - tr.endpos;

		float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));// Pythagorean

		bool horOK = distance2D > (dist - 2.0f);
		bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

		float cameraDistance;

		if (horOK && vertOK)  // If we are clear of obstacles
		{
			cameraDistance = dist; // go ahead and set the distance to the setting
		}
		else {
			if (vertOK) // if the Vertical Axis is OK
			{
				cameraDistance = distance2D * 0.95f;
			}
			else// otherwise we need to move closer to not go into the floor/ceiling
			{
				cameraDistance = abs(diff.z) * 0.95f;
			}
		}
		g_Input->m_fCameraInThirdPerson = true;

		g_Input->m_vecCameraOffset.z = cameraDistance;
	}
	else {
		g_Input->m_fCameraInThirdPerson = false;
	}
}
//--------------------------------------------------------------------------------
void Visuals::AddToDrawList() {
	if (g_Options.esp_grenade_prediction)
		GrenadePrediction::draw();

	if (g_Options.esp_sounds)
		RenderSounds();

	if (g_Options.esp_choke_indicator) {
		std::stringstream ss;
		ss << "choked: " << g_ClientState->chokedcommands;

		Render::Get().RenderText(ss.str(), ImVec2(10.0f, 450.0f), 14.0f, Color::White, false, true);
	}

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

	if (g_Options.esp_angle_lines) {
		Vector w2sOrigin;
		if (Math::WorldToScreen(g_LocalPlayer->m_vecOrigin(), w2sOrigin)) {
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, view_angle, "viewangles", Color(0.937f, 0.713f, 0.094f, 1.0f));
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, g_LocalPlayer->m_flLowerBodyYawTarget(), "lby", Color(0.0f, 0.0f, 1.0f, 1.0f));
			drawAngleLine(g_LocalPlayer->m_vecOrigin(), w2sOrigin, real_angle, "real", Color(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	if (g_Options.esp_hitmarker)
		DrawHitmarker();

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
		auto entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity == g_LocalPlayer)
			continue;

		if (i <= g_GlobalVars->maxClients) {
			auto player = Player();
			if (player.Begin((C_BasePlayer*)entity)) {
				if (g_Options.esp_player_skeleton)	 player.RenderSkeleton(entity);
				if (g_Options.esp_player_boxes)     player.RenderBox();
				if (g_Options.esp_player_weapons)   player.RenderWeaponName();
				if (g_Options.esp_player_names)     player.RenderName();
				if (g_Options.esp_player_health)    player.RenderHealth();
				if (g_Options.esp_player_armour)    player.RenderArmour();
				if (g_Options.esp_backtrack)				 player.RenderBacktrack();
			}
		}
		else if (g_Options.esp_dropped_weapons && entity->IsWeapon())
			RenderWeapon(static_cast<C_BaseCombatWeapon*>(entity));
		else if (g_Options.esp_dropped_weapons && entity->IsDefuseKit())
			RenderDefuseKit(entity);
		else if (entity->IsPlantedC4() && g_Options.esp_planted_c4)
			RenderPlantedC4(entity);
		else if( entity->IsLoot( ) && g_Options.esp_items )
			RenderItemEsp( entity );
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
				C_BasePlayer*  pBaseEntity = C_BasePlayer::GetPlayerByIndex(i);
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
	if (ImGui::Begin("Spectator List", nullptr, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (specs > 0) spect += "\n";

		ImVec2 size = ImGui::CalcTextSize(spect.c_str());
		ImGui::SetWindowSize(ImVec2(200, 25 + size.y));
		ImGui::Text(spect.c_str());
	}
	ImGui::End();
	ImGui::PopStyleVar();
}
//--------------------------------------------------------------------------------
void Visuals::DrawFOV() {
	auto pWeapon = g_LocalPlayer->m_hActiveWeapon();
	if (!pWeapon)
		return;

	auto settings = g_Options.legitbot_items[pWeapon->m_Item().m_iItemDefinitionIndex()];

	if (settings.enabled) {

		float fov = static_cast<float>(g_LocalPlayer->GetFOV());

		int w, h;
		g_EngineClient->GetScreenSize(w, h);

		Vector2D screenSize = Vector2D(w, h);
		Vector2D center = screenSize * 0.5f;

		float ratio = screenSize.x / screenSize.y;
		float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(fov * 0.5f)));

		float radiusFOV = tanf(DEG2RAD(g_Aimbot.GetFov())) / tanf(screenFov) * center.x;

		Render::Get().RenderCircleFilled(center.x, center.y, radiusFOV, 32, Color(0, 0, 0, 50));
		Render::Get().RenderCircle(center.x, center.y, radiusFOV, 32, Color(0, 0, 0, 100));

		if (settings.silent) {
			float silentRadiusFOV = tanf(DEG2RAD(settings.silent_fov)) / tanf(screenFov) * center.x;

			Render::Get().RenderCircleFilled(center.x, center.y, silentRadiusFOV, 32, Color(255, 25, 10, 50));
			Render::Get().RenderCircle(center.x, center.y, silentRadiusFOV, 32, Color(255, 25, 10, 100));
		}
	}
}

void Visuals::DrawHitmarker() {
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	auto curtime = g_GlobalVars->realtime;
	float lineSize = 8.f;
	if (flHurtTime + .05f >= curtime) {
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		g_EngineClient->GetScreenSize(screenSizeX, screenSizeY);

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;

		Color bg = Color(0, 0, 0, 50);
		Color white = Color(255, 255, 255, 255);

		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), bg, 2.5f);
		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), bg, 2.5f);
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), bg, 2.5f);
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), bg, 2.5f);

		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), white);
		Render::Get().RenderLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), white);
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), white);
		Render::Get().RenderLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), white);
	}
}
