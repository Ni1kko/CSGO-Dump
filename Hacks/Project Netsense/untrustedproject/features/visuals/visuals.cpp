#include "visuals.hpp"
#include "../../options.hpp"
#include "../../config.hpp"
#include "../../menu.hpp"
#include "../../render.hpp"
#include "chams.hpp"


void c_visuals::Run() {
	if (!g_LocalPlayer) return;

	//player drawing loop
	for (int i = 1; i <= g_GlobalVars->maxClients; i++) {
		auto entity = (C_BasePlayer*)C_BasePlayer::GetPlayerByIndex(i);

		if (!entity) continue;


		const int fade = (int)((3.4444444447f * g_GlobalVars->frametime) * 255);

		auto new_alpha = EspAlphas[i];
		new_alpha += entity->IsDormant() ? -fade : fade;

		if (new_alpha > (entity->m_bGunGameImmunity() ? 130 : 210))
			new_alpha = (entity->m_bGunGameImmunity() ? 130 : 210);
		auto alpha_divider = other.dormant ? 50 : 0;
		if (new_alpha < alpha_divider)
			new_alpha = alpha_divider;

		EspAlphas[i] = new_alpha;

		visuals_player player(entity, new_alpha);
		if (player.IsValid()) player.Draw();
		LastDormant[i] = entity->IsDormant();
	}

	if (other.sound_esp) DrawSounds();
}

void c_visuals::DrawSounds() {
	UpdateSounds();

	auto Add3DCircle = [](const Vector& position, Color color, float radius) {
		float precision = 160.0f;

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


	for (auto& [entIndex, sound] : m_Sounds) {
		if (sound.empty())
			continue;

		for (auto& info : sound) {
			if (info.soundTime + other.sound_esp_time < g_GlobalVars->realtime)
				info.alpha -= g_GlobalVars->frametime;

			if (info.alpha <= 0.0f)
				continue;

			float deltaTime = g_GlobalVars->realtime - info.soundTime;

			auto factor = deltaTime / other.sound_esp_time;
			if (factor > 1.0f)
				factor = 1.0f;

			float radius = other.sound_esp_radius * factor;

			switch (other.sound_esp_type) {
			case 0:
				Add3DCircle(info.soundPos, other.sound_esp_clr, radius);
				break;
			case 1:
			{
				static float lasttime[64] = { 0.f };

				if (g_GlobalVars->curtime - lasttime[entIndex] > .5f) { // pFix
					BeamInfo_t beamInfo;
					beamInfo.m_nType = TE_BEAMRINGPOINT;
					beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
					beamInfo.m_nModelIndex = g_MdlInfo->GetModelIndex("sprites/purplelaser1.vmt");
					beamInfo.m_nHaloIndex = -1;
					beamInfo.m_flHaloScale = 5;
					beamInfo.m_flLife = 1.f; //
					beamInfo.m_flWidth = 10.f;
					beamInfo.m_flFadeLength = 1.0f;
					beamInfo.m_flAmplitude = 0.f;
					beamInfo.m_flRed = other.sound_esp_clr.r();
					beamInfo.m_flGreen = other.sound_esp_clr.g();
					beamInfo.m_flBlue = other.sound_esp_clr.b();
					beamInfo.m_flBrightness = other.sound_esp_clr.a();
					beamInfo.m_flSpeed = 0.f;
					beamInfo.m_nStartFrame = 0.f;
					beamInfo.m_flFrameRate = 60.f;
					beamInfo.m_nSegments = -1;
					beamInfo.m_nFlags = FBEAM_FADEOUT;
					beamInfo.m_vecCenter = info.soundPos + Vector(0, 0, 5);
					beamInfo.m_flStartRadius = 20.f;
					beamInfo.m_flEndRadius = 640.f;
					auto myBeam = g_ViewRenderBeams->CreateBeamRingPoint(beamInfo);

					if (myBeam)
						g_ViewRenderBeams->DrawBeam(myBeam);

					lasttime[entIndex] = g_GlobalVars->curtime;
				}
				else
					info.alpha = 0.f;
			}
			break;
			}
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



void c_visuals::UpdateSounds() {
	sounds.RemoveAll();
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

		if ((C_BasePlayer*)g_LocalPlayer == player)
			continue;

		if (!other.sound_esp_onteam && player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) return;

		if (player->m_vecOrigin().DistTo(g_LocalPlayer->m_vecOrigin()) > 900)
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



void c_visuals::SetupValues() {
	g_Config->PushItem(&boxes.enabled, "esp", "boxes.enabled", boxes.enabled);
	g_Config->PushItem(&boxes.outline, "esp", "boxes.outline", boxes.outline);
	g_Config->PushItem(&boxes.type, "esp", "boxes.type", boxes.type);
	g_Config->PushItem(&boxes.allies_vis_clr, "esp", "boxes.allies_vis_clr", boxes.allies_vis_clr);
	g_Config->PushItem(&boxes.enemy_vis_clr, "esp", "boxes.enemy_vis_clr", boxes.enemy_vis_clr);
	g_Config->PushItem(&boxes.allies_invis_clr, "esp", "boxes.allies_invis_clr", boxes.allies_invis_clr);
	g_Config->PushItem(&boxes.enemy_invis_clr, "esp", "boxes.enemy_invis_clr", boxes.enemy_invis_clr);

	g_Config->PushItem(&skeleton.enabled, "esp", "skeleton.enabled", skeleton.enabled);
	g_Config->PushItem(&skeleton.clr, "esp", "skeleton.clr", skeleton.clr);

	g_Config->PushItem(&other.hp, "esp", "other.hp", other.hp);
	g_Config->PushItem(&other.name, "esp", "other.name", other.name);
	g_Config->PushItem(&other.weapon, "esp", "other.weapon", other.weapon);
	g_Config->PushItem(&other.armor, "esp", "other.armor", other.armor);
	//g_Config->PushItem(&other.snipeline, "esp", "other.snipeline", other.snipeline);
	g_Config->PushItem(&other.team, "esp", "other.team", other.team);
	g_Config->PushItem(&other.visible_only, "esp", "other.visible_only", other.visible_only);
	//g_Config->PushItem(&other.head_dot, "esp", "other.head_dot", other.head_dot);
	g_Config->PushItem(&other.dormant, "esp", "other.dormant", other.dormant);
	g_Config->PushItem(&other.name_clr, "esp", "other.name_clr", other.name_clr);
	g_Config->PushItem(&other.weapon_clr, "esp", "other.weapon_clr", other.weapon_clr);
	/*g_Config->PushItem(&other.snipeline_clr, "esp", "other.snipeline_clr", other.snipeline_clr);
	g_Config->PushItem(&other.head_dot_clr, "esp", "other.head_dot_clr", other.head_dot_clr);*/
	g_Config->PushItem(&other.weapon_size, "esp", "weapon_size", other.weapon_size);
	g_Config->PushItem(&other.name_size, "esp", "name_size", other.name_size);

	g_Config->PushItem(&other.flags, "esp", "flags", other.flags);
	g_Config->PushItem(&other.flags_reload, "esp", "flags_reload", other.flags_reload);
	g_Config->PushItem(&other.flags_reload_size, "esp", "flags_reload_size", other.flags_reload_size);
	g_Config->PushItem(&other.flags_scoped, "esp", "flags_scoped", other.flags_scoped);
	g_Config->PushItem(&other.flags_scoped_size, "esp", "flags_scoped_size", other.flags_scoped_size);
	g_Config->PushItem(&other.flags_bomb, "esp", "flags_bomb", other.flags_bomb);
	g_Config->PushItem(&other.flags_bomb_size, "esp", "flags_bomb_size", other.flags_bomb_size);
	g_Config->PushItem(&other.flags_flashed, "esp", "flags_flashed", other.flags_flashed);
	g_Config->PushItem(&other.flags_flashed_size, "esp", "flags_flashed_size", other.flags_flashed_size);
	g_Config->PushItem(&other.flags_hk, "esp", "flags_hk", other.flags_hk);
	g_Config->PushItem(&other.flags_hk_size, "esp", "flags_hk_size", other.flags_hk_size);
	g_Config->PushItem(&other.flags_defusing, "esp", "flags_defusing", other.flags_defusing);
	g_Config->PushItem(&other.flags_defusing_size, "esp", "flags_defusing_size", other.flags_hk_size);
	g_Config->PushItem(&other.hp_clr, "esp", "hp_clr", other.hp_clr);
	g_Config->PushItem(&other.armor_clr, "esp", "armor_clr", other.armor_clr);

	g_Config->PushItem(&other.sound_esp, "esp", "sound_esp", other.sound_esp);
	g_Config->PushItem(&other.sound_esp_type, "esp", "sound_esp_type", other.sound_esp_type);
	g_Config->PushItem(&other.sound_esp_clr, "esp", "sound_esp_clr", other.sound_esp_clr);
	g_Config->PushItem(&other.sound_esp_onteam, "esp", "sound_esp_onteam", other.sound_esp_onteam);
	g_Config->PushItem(&other.sound_esp_time, "esp", "sound_esp_time", other.sound_esp_time);
	g_Config->PushItem(&other.sound_esp_radius, "esp", "sound_esp_radius", other.sound_esp_radius);

	g_Config->PushItem(&glow.enabled, "glow", "enabled", glow.enabled);
	g_Config->PushItem(&glow.players, "glow", "players", glow.players);
	g_Config->PushItem(&glow.on_team, "glow", "on_team", glow.on_team);
	g_Config->PushItem(&glow.on_chickens, "glow", "on_chickens", glow.on_chickens);
	g_Config->PushItem(&glow.c4_carrier, "glow", "c4_carrier", glow.c4_carrier);
	g_Config->PushItem(&glow.c4_planted, "glow", "c4_planted", glow.c4_planted);
	g_Config->PushItem(&glow.defuse_kits, "glow", "defuse_kits", glow.defuse_kits);
	g_Config->PushItem(&glow.type, "glow", "type", glow.type);
	g_Config->PushItem(&glow.bloom, "glow", "bloom", glow.bloom);
	g_Config->PushItem(&glow.enemy_clr, "glow", "enemy_clr", glow.enemy_clr);
	g_Config->PushItem(&glow.enemy_invis_clr, "glow", "enemy_invis_clr", glow.enemy_invis_clr);
	g_Config->PushItem(&glow.allies_clr, "glow", "allies_clr", glow.allies_clr);
	g_Config->PushItem(&glow.allies_invis_clr, "glow", "allies_invis_clr", glow.allies_invis_clr);
	g_Config->PushItem(&glow.chickens_clr, "glow", "chickens_clr", glow.chickens_clr);
	g_Config->PushItem(&glow.c4_carrier_clr, "glow", "c4_carrier_clr", glow.c4_carrier_clr);
	g_Config->PushItem(&glow.c4_planted_clr, "glow", "c4_planted_clr", glow.c4_planted_clr);
	g_Config->PushItem(&glow.defuse_kits_clr, "glow", "defuse_kits_clr", glow.defuse_kits_clr);
	g_Config->PushItem(&glow.weapons_clr, "glow", "weapons_clr", glow.weapons_clr);

	g_Config->PushItem(&boxes.filled, "esp", "boxes.filled", boxes.filled);
	g_Config->PushItem(&boxes.fill_color, "esp", "boxes.fill_color", boxes.fill_color);
	g_Config->PushItem(&boxes.outline_clr, "esp", "boxes.outline_clr", boxes.outline_clr);

	g_Config->PushItem(&other.ammobar_clr, "esp", "other.ammobar_clr", other.ammobar_clr);
	g_Config->PushItem(&other.ammo_bar, "esp", "other.ammo_bar", other.ammo_bar);

	auto setupChams = [&](ConfChams& chams, std::string szName) -> void {
		g_Config->PushItem(&chams.DoNotRemoveModel, szName, "do_not_remove_model", chams.DoNotRemoveModel);
		for (size_t i = 0; i < chams.materials.size(); i++) {
			auto bName = szName + "_" + std::to_string(i);
			g_Config->PushItem(&chams.materials[i].blinking, bName, "pulsing", chams.materials[i].blinking);
			g_Config->PushItem(&chams.materials[i].color, bName, "clr", chams.materials[i].color);
			g_Config->PushItem(&chams.materials[i].enabled, bName, "enabled", chams.materials[i].enabled);
			g_Config->PushItem(&chams.materials[i].healthBased, bName, "hBased", chams.materials[i].healthBased);
			g_Config->PushItem(&chams.materials[i].material, bName, "materialIndex", chams.materials[i].material);
			g_Config->PushItem(&chams.materials[i].wireframe, bName, "wireframe", chams.materials[i].wireframe);
		}
	};
	setupChams(g_Chams->chams_player_allies_all, "chams_player_allies_all");
	setupChams(g_Chams->chams_player_allies_visible, "chams_player_allies_visible");
	setupChams(g_Chams->chams_player_allies_occluded, "chams_player_allies_occluded");
	
	setupChams(g_Chams->chams_player_enemies_all, "chams_player_enemies_all");
	setupChams(g_Chams->chams_player_enemies_visible, "chams_player_enemies_visible");
	setupChams(g_Chams->chams_player_enemies_occluded, "chams_player_enemies_occluded");

	setupChams(g_Chams->chams_player_backtrack, "chams_player_backtrack");
	
	setupChams(g_Chams->chams_player_local_player, "chams_player_local_player");
	setupChams(g_Chams->chams_weapons, "chams_weapons");
	setupChams(g_Chams->chams_sleeves, "chams_sleeves");
	setupChams(g_Chams->chams_hands, "chams_hands");
}
static int subtab = 1;
static int subtab2 = 0;





void c_visuals::Menu() 
{
	

	static int page = 1;


	if (ImGui::SButton("Visuals", page == 1, ImVec2(200, 30)))
		page = 1;
	
		

	/*ImGui::SameLine();
	if (ImGui::SButton("Chams", page = 2, ImVec2(200, 30)))
		page = 2;*/

	
		


	auto textSettings = [&](std::string show, std::string safe, float* size) -> void {
		ImGui::SameLine();

		bool openPopup = ImGui::Button(("+" + safe).c_str());

		if (openPopup)
			ImGui::OpenPopup((safe + "##popup").c_str());

		if (ImGui::BeginPopup((safe + "##popup").c_str())) {
			ImGui::PushItemWidth(160.f);
			if (ImGui::SliderFloat(("Size" + safe).c_str(), size, 8.f, 20.f))
				*size = std::clamp(*size, 8.f, 20.f);
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
	};

	int menur, menug, menub, menua;
	menur = g_Misc->options.cMenuCol[0] * 255;
	menug = g_Misc->options.cMenuCol[1] * 255;
	menub = g_Misc->options.cMenuCol[2] * 255;
	menua = g_Misc->options.cMenuCol[3] * 255;
	
	
	switch (page) {
	case 1:
		ImGui::Columns(3, nullptr, false);

		/*ImGui::BeginChild("##chams", ImVec2(-1, -1), true); {
			ImGui::TextColored(ImColor(menur, menug, menub), "Chams");
			g_Chams->Menu();
		}
		ImGui::EndChild();*/
		ImGui::BeginChild("##1st", ImVec2(-1, 205), true); {
			ImGui::TextColored(ImColor(menur, menug, menub), "ESP");
			ImGui::Checkbox("Boxes", &boxes.enabled);
			ImGui::Checkbox("Skeleton", &skeleton.enabled);
			ImGui::Checkbox("Name", &other.name);
			textSettings("ESP Name Settings", "##namesettings", &other.name_size);
			ImGui::Checkbox("Health", &other.hp);
			ImGui::Checkbox("Armor", &other.armor);
			ImGui::Checkbox("Ammos", &other.ammo_bar);
			ImGui::Checkbox("Weapon", &other.weapon);
			textSettings("ESP Weapon Settings", "##weaponsettings", &other.weapon_size);
			ImGui::Checkbox("Flags", &other.flags);
			ImGui::SameLine();
			bool openPopupFlags = ImGui::Button("+##flags");

			if (openPopupFlags)
				ImGui::OpenPopup("##flagspopup");

			if (ImGui::BeginPopup("##flagspopup")) {
				ImGui::PushItemWidth(160.f);

				ImGui::Checkbox("Reload", &other.flags_reload);
				if (ImGui::SliderFloat("Size##reloadsize", &other.flags_reload_size, 8.f, 20.f))
					other.flags_reload_size = std::clamp(other.flags_reload_size, 8.f, 20.f);

				ImGui::Checkbox("Scoped", &other.flags_scoped);
				if (ImGui::SliderFloat("Size##scopedsize", &other.flags_scoped_size, 8.f, 20.f))
					other.flags_scoped_size = std::clamp(other.flags_scoped_size, 8.f, 20.f);

				ImGui::Checkbox("Bomb", &other.flags_bomb);
				if (ImGui::SliderFloat("Size##bombsize", &other.flags_bomb_size, 8.f, 20.f))
					other.flags_bomb_size = std::clamp(other.flags_bomb_size, 8.f, 20.f);

				ImGui::Checkbox("Flashed", &other.flags_flashed);
				if (ImGui::SliderFloat("Size##flashedsize", &other.flags_flashed_size, 8.f, 20.f))
					other.flags_flashed_size = std::clamp(other.flags_flashed_size, 8.f, 20.f);

				ImGui::Checkbox("HK", &other.flags_hk);
				if (ImGui::IsItemHovered()) {
					ImGui::SameLine();
					ImGui::Text(" | HK = Helmet Kevlar");
				}
				if (ImGui::SliderFloat("Size##hksize", &other.flags_hk_size, 8.f, 20.f))
					other.flags_hk_size = std::clamp(other.flags_hk_size, 8.f, 20.f);


				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

			ImGui::Checkbox("Sound ESP", &other.sound_esp);
			ImGui::SameLine();
			bool openPopupSoundESP = ImGui::Button("+##soundesp");

			if (openPopupSoundESP)
				ImGui::OpenPopup("##flagsoundesp");

			if (ImGui::BeginPopup("##flagsoundesp")) {
				ImGui::PushItemWidth(160.f);

				ImGui::Combo("Type", &other.sound_esp_type, values::soundesp_modes, 2);
				ImGui::Checkbox("On team", &other.sound_esp_onteam);
				if (ImGui::SliderFloat("SoundESP Time", &other.sound_esp_time, 0.5f, 3.f))
					other.sound_esp_time = std::clamp(other.sound_esp_time, 0.5f, 3.f);
				if (ImGui::SliderFloat("SoundESP Radius", &other.sound_esp_radius, 15.f, 150.f))
					other.sound_esp_radius = std::clamp(other.sound_esp_radius, 15.f, 150.f);

				ImGui::PopItemWidth();
				ImGui::EndPopup();
			}

		}	ImGui::EndChild();


		ImGui::BeginChild("##glow", ImVec2(-1, -1), true); {
			//ImGui::Text("Glow");
			ImGui::TextColored(ImColor(menur, menug, menub), "Glow");
			ImGui::Checkbox("Enabled##glow", &glow.enabled);
			ImGui::Checkbox("Players##glow", &glow.players);
			ImGui::Checkbox("Team##glow", &glow.on_team);
			ImGui::Checkbox("Chickens##glow", &glow.on_chickens);
			ImGui::Checkbox("C4 Carrier##glow", &glow.c4_carrier);
			ImGui::Checkbox("C4 Planted##glow", &glow.c4_planted);
			ImGui::Checkbox("Defuse Kits##glow", &glow.defuse_kits);
			ImGui::Checkbox("Weapons##glow", &glow.weapons);
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("##second", ImVec2(-1, 195), true); {
			//ImGui::Text("Options");
			ImGui::TextColored(ImColor(menur, menug, menub), "Options");
			ImGui::Combo("Boxes type", &boxes.type, values::box_types, 2);
			ImGui::Checkbox("Boxes outline", &boxes.outline);
			ImGui::Checkbox("Boxes filled", &boxes.filled);
			ImGui::Checkbox("On team", &other.team);
			ImGui::Checkbox("Dormant", &other.dormant);
			ImGui::Combo("Glow type", &glow.type, values::glow_modes, 4);
			ImGui::Checkbox("Glow filled", &glow.bloom);
			ImGui::Checkbox("Visible Only", &other.visible_only);
		}
		ImGui::EndChild();

		ImGui::BeginChild("##chams", ImVec2(-1, -1), true); {
			ImGui::TextColored(ImColor(menur, menug, menub), "Chams");
			g_Chams->Menu();
		}
		ImGui::EndChild();

		ImGui::NextColumn();


		ImGui::BeginChild("##colors", ImVec2(-1, -1), true); {
			ImGui::TextColored(ImColor(menur, menug, menub), "Colors");
			ImGui::ColorEdit("Boxes Allies Visible", boxes.allies_vis_clr);
			ImGui::ColorEdit("Boxes Allies Invisible", boxes.allies_invis_clr);
			ImGui::ColorEdit("Boxes Enemy Visible", boxes.enemy_vis_clr);
			ImGui::ColorEdit("Boxes Enemy Invisible", boxes.enemy_invis_clr);
			ImGui::ColorEdit("Boxes Fill", boxes.fill_color);
			ImGui::ColorEdit("Boxes Outline", boxes.outline_clr);
			ImGui::ColorEdit("Health Bar", other.hp_clr);
			ImGui::ColorEdit("Armor Bar", other.armor_clr);
			ImGui::ColorEdit("Ammo Bar", other.ammobar_clr);
			ImGui::ColorEdit("Skeleton", skeleton.clr);
			ImGui::ColorEdit("Name", other.name_clr);
			ImGui::ColorEdit("Weapon", other.weapon_clr);
			ImGui::ColorEdit("SoundESP circles", other.sound_esp_clr);
			ImGui::ColorEdit("Glow Enemy", glow.enemy_clr);
			ImGui::ColorEdit("Glow Enemy Invisible", glow.enemy_invis_clr);
			ImGui::ColorEdit("Glow Team", glow.allies_clr);
			ImGui::ColorEdit("Glow Team Invisible", glow.allies_invis_clr);
			ImGui::ColorEdit("Glow Chickens", glow.chickens_clr);
			ImGui::ColorEdit("Glow C4 Carrier", glow.c4_carrier_clr);
			ImGui::ColorEdit("Glow C4 Planted", glow.c4_planted_clr);
			ImGui::ColorEdit("Glow Defuse Kits", glow.defuse_kits_clr);
			ImGui::ColorEdit("Glow Weapons", glow.weapons_clr);

			g_Chams->MenuColors();
		}
		ImGui::EndChild();
		break;

	//case 2:

		//ImGui::Columns(2, nullptr, false);
		

		//ImGui::NextColumn();

		/*ImGui::BeginChild("##third", ImVec2(-1, -1), true); {
			//ImGui::Text("Colors");
			ImGui::TextColored(ImColor(menur, menug, menub), "Chams Colors");
			
		}ImGui::EndChild();
		break;*/
		
	}
	

}

c_visuals* g_Visuals = new c_visuals();