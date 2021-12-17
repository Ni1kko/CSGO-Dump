#include "visuals.hpp"
#include "../../options.hpp"
#include "../../render.hpp"
#include "../../globals.hpp"


c_visuals::visuals_player::visuals_player(C_BasePlayer* player, int alpha) {
	this->player = player;
	this->alpha = alpha;
	bbox = GetBBox(player);
}

void c_visuals::visuals_player::Initialize() {
	this->in_team = player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum();
	this->health = player->m_iHealth();
	this->armor = player->m_ArmorValue();

	player_info_t info;
	g_EngineClient->GetPlayerInfo(player->EntIndex(), &info);

	this->playerName = std::string(info.fakeplayer ? std::string("BOT ").append(info.szName) : info.szName);

	C_BaseCombatWeapon* weapon = player->m_hActiveWeapon();
	if (weapon && weapon->GetCSWeaponData()) {
		auto clean_item_name = [](const char* name) -> std::string {
			std::string res;

			if (name[0] == 'C')
				name++;

			auto start = strstr(name, "weapon");
			if (start != nullptr)
				name = start + 7;

			res = name;
			std::transform(res.begin(), res.end(), res.begin(), ::toupper);
			
			return res;
		};
		this->weaponName = clean_item_name(weapon->GetCSWeaponData()->szWeaponName);
	}

	previousInfo = &g_Visuals->PreviousInfos[player->EntIndex()];

	if (previousInfo->hp == -1)
		previousInfo->hp = std::clamp(health, 0, 100);
	else {
		if (health != previousInfo->hp) {
			if (previousInfo->hp > health) {
				if (previousInfo->hpDiffTime == 0.f)
					previousInfo->hpDifference = previousInfo->hp - health;
				else
					previousInfo->hpDifference += previousInfo->hp - health;

				previousInfo->hpDiffTime = g_GlobalVars->curtime;
			} else {
				previousInfo->hpDifference = 0;
				previousInfo->hpDiffTime = 0.f;
			}

			previousInfo->hp = health;
		}

		if (previousInfo->hpDifference != 0) {
			float diffFactor = ((1.f / 0.5f) * g_GlobalVars->frametime) * (previousInfo->hpDifference * 2);
			previousInfo->hpDifference -= diffFactor;
			previousInfo->hpDifference = std::clamp(previousInfo->hpDifference, 0, 100);

			if (previousInfo->hpDifference == 0)
				previousInfo->hpDiffTime = 0.f;
		}
	}

	if (previousInfo->armor == -1)
		previousInfo->armor = std::clamp(armor, 0, 100);
	else {
		if (armor != previousInfo->armor) {
			if (previousInfo->armor > armor) {
				if (previousInfo->armorDiffTime == 0.f)
					previousInfo->armorDifference = previousInfo->armor - armor;
				else
					previousInfo->armorDifference += previousInfo->armor - armor;
			} else {
				previousInfo->armorDifference = 0;
				previousInfo->armorDiffTime = 0.f;
			}
			previousInfo->armor = armor;
		}
		if (previousInfo->armorDifference != 0) {
			float diffFactor = ((1.f / 0.5f) * g_GlobalVars->frametime) * (previousInfo->armorDifference * 2);
			previousInfo->armorDifference -= diffFactor;
			previousInfo->armorDifference = std::clamp(previousInfo->armorDifference, 0, 100);

			if (previousInfo->armorDifference == 0)
				previousInfo->armorDiffTime = 0.f;
		}
	}
}

bool c_visuals::visuals_player::IsValid() {
	if (!this->player) return false;
	
	if (this->player == g_LocalPlayer) return false;
	
	if (this->player->IsDormant() && !g_Visuals->other.dormant) return false;
	
	if (!this->player->IsAlive()) return false;

	if (this->player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && !g_Visuals->other.team) return false;

	this->is_visible = g_LocalPlayer->CanSeePlayer(this->player, player->GetBonePos(HITBOX_CHEST));
	if ((!is_visible) && (g_Visuals->other.visible_only)) return false;

	if (!Math::WorldToScreen(player->GetHitboxPos(HITBOX_HEAD), head_pos)) return false;
	if (!Math::WorldToScreen(player->m_vecOrigin(), feet_pos)) return false;

	if (bbox.right <= 0.f || bbox.top <= 0.f || bbox.left <= 0.f || bbox.bottom <= 0.f) return false;

	return !(!bbox.top || !bbox.bottom || !bbox.left || !bbox.right);
}

void c_visuals::visuals_player::DrawBoxes() {
	Color box_clr = Color::Red;
	if (is_visible) {
		if (in_team)
			box_clr = g_Visuals->boxes.allies_vis_clr;
		else
			box_clr = g_Visuals->boxes.enemy_vis_clr;
	} else {
		if (in_team)
			box_clr = g_Visuals->boxes.allies_invis_clr;
		else
			box_clr = g_Visuals->boxes.enemy_invis_clr;
	}
	auto black_clr = g_Visuals->boxes.outline_clr;
	auto fill_clr = g_Visuals->boxes.fill_color;
	ImplementAlpha(box_clr);
	ImplementAlpha(black_clr);
	ImplementAlpha(fill_clr);

	switch (g_Visuals->boxes.type) {
	case 0:
		if (g_Visuals->boxes.outline) {
			Render::Get().RenderBox(bbox.left - 1, bbox.top - 1, bbox.right + 1, bbox.bottom + 1, black_clr);
			Render::Get().RenderBox(bbox.left + 1, bbox.top + 1, bbox.right - 1, bbox.bottom - 1, black_clr);
		}
		Render::Get().RenderBox(bbox.left, bbox.top, bbox.right, bbox.bottom, box_clr, 1);
		break;
	case 1:
		if (g_Visuals->boxes.outline) {
			Render::Get().RenderCoalBox(bbox.left - 1, bbox.top - 1, bbox.right + 1, bbox.bottom + 1, black_clr);
			Render::Get().RenderCoalBox(bbox.left + 1, bbox.top + 1, bbox.right - 1, bbox.bottom - 1, black_clr);
		}
		Render::Get().RenderCoalBox(bbox.left, bbox.top, bbox.right, bbox.bottom, box_clr, 1);
		break;
	};

	if (g_Visuals->boxes.filled)
		Render::Get().RenderBoxFilled(bbox.left + 1, bbox.top + 1, bbox.right - 1, bbox.bottom - 1, fill_clr);
}

void c_visuals::visuals_player::DrawSkeleton() {
	auto model = player->GetModel();
	if (model) {
		auto skeleton_clr = g_Visuals->skeleton.clr;
		ImplementAlpha(skeleton_clr);
		studiohdr_t* studio_model = g_MdlInfo->GetStudiomodel(model);
		if (studio_model) {
			static matrix3x4_t bone_to_world[128];

			if (player->SetupBones(bone_to_world, 128, 256, g_GlobalVars->curtime)) {
				for (int i = 0; i < studio_model->numbones; i++) {
					mstudiobone_t* bone = studio_model->GetBone(i);
					if (!bone || !(bone->flags & 256) || bone->parent == -1) continue;

					Vector bone_pos_1;
					if (!Math::WorldToScreen(Vector(bone_to_world[i][0][3], bone_to_world[i][1][3], bone_to_world[i][2][3]), bone_pos_1)) continue;

					Vector bone_pos_2;
					if (!Math::WorldToScreen(Vector(bone_to_world[bone->parent][0][3], bone_to_world[bone->parent][1][3], bone_to_world[bone->parent][2][3]), bone_pos_2)) continue;

					Render::Get().RenderLine(bone_pos_1.x, bone_pos_1.y, bone_pos_2.x, bone_pos_2.y, skeleton_clr);
				}
			}
		}
	}
}

void c_visuals::visuals_player::DrawHP() {
	float box_h = (float)fabs(bbox.bottom - bbox.top);

	if (g_Visuals->boxes.outline)
		box_h += 2;

	int height = ((box_h * health) / 100) - 2;
	int x = bbox.left - 7;
	int y = bbox.top + 1;
	int w = 4;
	int h = box_h - 2;

	auto bClr = Color::Black;
	auto hClr = g_Visuals->other.hp_clr;

	ImplementAlpha(bClr);
	ImplementAlpha(hClr);

	if (g_Visuals->boxes.outline)
		y -= 1;

	Render::Get().RenderBox(x, y - 1, x + w, y + h + 1, bClr, 1.f, true); // outline

	Render::Get().RenderBox(x + 1, y, x + w - 1, y + height, hClr, 1.f, true);

	if (previousInfo->hpDifference != 0) {
		int diffHeight = ((box_h * previousInfo->hpDifference) / 100);
		Render::Get().RenderBox(x + 1, y + height + 1,
								x + w - 1, y + height + diffHeight + 1,
								hClr, 1.f, true);
	}
}

void c_visuals::visuals_player::DrawName() {
	auto clr = g_Visuals->other.name_clr;
	ImplementAlpha(clr);

	auto sz = g_pDefaultFont->CalcTextSizeA(g_Visuals->other.name_size, FLT_MAX, 0.0f, playerName.c_str());
	ImVec2 nPos = ImVec2(bbox.left + (bbox.right - bbox.left) * 0.5f - sz.x * 0.5f, bbox.top - sz.y - 1);
	Render::Get().RenderText(playerName, nPos, g_Visuals->other.name_size, clr, false);
}

void c_visuals::visuals_player::DrawWeapon() {
	auto clr = g_Visuals->other.weapon_clr;
	ImplementAlpha(clr);

	auto sz = g_pDefaultFont->CalcTextSizeA(g_Visuals->other.weapon_size, FLT_MAX, NULL, weaponName.c_str());
	ImVec2 wPos = ImVec2((bbox.left + (bbox.right - bbox.left) * 0.5f) - sz.x * 0.5f, g_Visuals->other.ammo_bar ? bbox.bottom + 7 : bbox.bottom);
	Render::Get().RenderText(weaponName, wPos, g_Visuals->other.weapon_size, clr, false);
}

/*void c_visuals::visuals_player::DrawSnipeLine() {
	int screen_x = CheatGVars::SWidth * 0.5f,
		screen_y = CheatGVars::SHeight,
		target_x = bbox.left + (bbox.right - bbox.left) * 0.5,
		target_y = bbox.bottom,
		max_length = CheatGVars::SHeight * 0.3f;

	if (target_x == 0 ||
		target_y == 0)
		return;

	auto sClr = g_Visuals->other.snipeline_clr;
	ImplementAlpha(sClr);

	float length = sqrt(pow(target_x - screen_x, 2) + pow(target_y - screen_y, 2));
	if (length > max_length) {
		float
			x_normalized = (target_x - screen_x) / length,
			y_normalized = (target_y - screen_y) / length;
		target_x = screen_x + x_normalized * max_length;
		target_y = screen_y + y_normalized * max_length;
		Render::Get().RenderCircle(target_x + x_normalized * 3.5f, target_y + y_normalized * 3.5f, 3.f, 12, sClr);
	}

	Render::Get().RenderLine(screen_x, screen_y, target_x, target_y, sClr);
}*/

/*void c_visuals::visuals_player::DrawHeadDot() {
	auto clr = g_Visuals->other.head_dot_clr;
	ImplementAlpha(clr);
	Render::Get().RenderBox(head_pos.x - 2, head_pos.y - 2, head_pos.x + 2, head_pos.y + 2, clr);
}*/

void c_visuals::visuals_player::DrawArmor() {
	float box_h = (float)fabs(bbox.bottom - bbox.top);

	if (g_Visuals->boxes.outline)
		box_h += 2;

	int height = (((box_h * armor) / 100)) - 3;
	int x = bbox.right + 3;
	int y = bbox.top + 1;
	int w = 4;
	int h = box_h - 3;

	auto bClr = Color::Black;
	auto hClr = g_Visuals->other.armor_clr;

	ImplementAlpha(bClr);
	ImplementAlpha(hClr);

	if (g_Visuals->boxes.outline)
		y -= 1;

	if (armor > 1) {
		Render::Get().RenderBox(x, y - 1, x + w, y + h + 2, bClr, 1.f, true);
		Render::Get().RenderBox(x + 1, y, x + w - 1, y + height + 1, hClr, 1.f, true);
		
		if (previousInfo->armorDifference != 0) {
			int diffHeight = ((box_h * previousInfo->armorDifference) / 100);
			Render::Get().RenderBox(x + 1, y + height + 1,
									x + w - 1, y + height + diffHeight + 1,
									hClr, 1.f, true);
		}
	}
}

void c_visuals::visuals_player::DrawFlags() {
	std::vector<sideinfo_t> sizeInfos;

	auto sClr = Color(65, 169, 244); // scoped
	auto rClr = Color(65, 169, 244); // reload
	auto bClr = Color(244, 65, 65); // bomb
	auto flClr = Color(244, 65, 65); // flashed
	auto hkClr = Color(244, 65, 65); // HK
	auto dClr = Color(244, 65, 65); // defusing

	ImplementAlpha(sClr);
	ImplementAlpha(rClr);
	ImplementAlpha(bClr);
	ImplementAlpha(flClr);
	ImplementAlpha(hkClr);

	if (player->m_bIsScoped() && g_Visuals->other.flags_scoped)
		sizeInfos.push_back({ "SCOPED", sClr, g_Visuals->other.flags_scoped_size });

	if (player->IsReloading() && g_Visuals->other.flags_reload)
		sizeInfos.push_back({ "RELOAD", rClr, g_Visuals->other.flags_reload_size });

	if (g_Visuals->other.flags_bomb && player->HasC4())
		sizeInfos.push_back({ "BOMB", bClr, g_Visuals->other.flags_bomb_size });

	if (g_Visuals->other.flags_flashed && player->IsFlashed())
		sizeInfos.push_back({ "FLASHED", flClr, g_Visuals->other.flags_flashed_size });

	if (g_Visuals->other.flags_hk && player->m_ArmorValue() > 0) {
		std::string txt;
		if (player->m_bHasHelmet())
			txt += "H";
		txt += "K";
		sizeInfos.push_back({ txt, hkClr, g_Visuals->other.flags_hk_size });
	}

	if (g_Visuals->other.flags_defusing && player->m_bIsDefusing())
		sizeInfos.push_back({ player->m_bHasDefuser() ? "DEFUSING WITH KITS" : "DEFUSING", dClr, g_Visuals->other.flags_defusing_size });


	int yOffset = 0;
	int adder = g_Visuals->other.armor ? 15 : 4;

	for (auto i : sizeInfos) {
		auto textsz = g_pFlagsFont->CalcTextSizeA(i.size, FLT_MAX, NULL, i.str.c_str());
		Render::Get().RenderText(i.str, bbox.right + adder, bbox.top + yOffset, i.size, i.color, false, true, g_pFlagsFont);
		yOffset += textsz.y + 2;
	}
}

void c_visuals::visuals_player::DrawAmmoBar() {
	C_BaseCombatWeapon* weapon = player->m_hActiveWeapon();
	if (!weapon) 
		return;

	auto animLayer = player->GetAnimOverlay(1);
	if (!animLayer->m_pOwner)
		return;

	auto activity = player->GetSequenceActivity(animLayer->m_nSequence);

	int iClip = weapon->m_iClip1();
	auto wpnData = weapon->GetCSWeaponData();
	if (!wpnData)
		return;
	int iClipMax = wpnData->iMaxClip1;

	float box_w = (float)fabs(bbox.right - bbox.left);
	float width;
	if (activity == 967 && animLayer->m_flWeight != 0.f) {
		float cycle = animLayer->m_flCycle; // 1 = finished 0 = just started
		width = (((box_w * cycle) / 1.f));
	}
	else
		width = (((box_w * iClip) / iClipMax));

	auto bClr = Color::Black;
	auto hClr = g_Visuals->other.ammobar_clr;
	ImplementAlpha(bClr);
	ImplementAlpha(hClr);

	int x1 = bbox.left;
	int y1 = bbox.bottom + 3;
	int x2 = bbox.right;
	int y2 = bbox.bottom + 7;

	if (g_Visuals->boxes.outline) {
		x1 -= 1;
		x2 += 1;
		width += 2;
	}

	Render::Get().RenderBox(x1, y1, x2, y2, bClr);
	Render::Get().RenderBoxFilled(x1 + 1, (y1 + 1), (int)(x1 + width - 1), (y2 - 1), hClr);
}

bool c_visuals::visuals_player::Draw() {
	this->Initialize();

	if (g_Visuals->boxes.enabled) this->DrawBoxes();

	if (g_Visuals->skeleton.enabled) this->DrawSkeleton();

	if (g_Visuals->other.hp) this->DrawHP();

	if (g_Visuals->other.name) this->DrawName();

	if (g_Visuals->other.weapon) this->DrawWeapon();

	//if (g_Visuals->other.snipeline) this->DrawSnipeLine();

	//if (g_Visuals->other.head_dot) this->DrawHeadDot();

	if (g_Visuals->other.armor) this->DrawArmor();

	if (g_Visuals->other.flags) this->DrawFlags();

	if (g_Visuals->other.ammo_bar) this->DrawAmmoBar();

	return true;
}