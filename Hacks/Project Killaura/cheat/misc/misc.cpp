#include "misc.hpp"
#include "../hooks/hooks.hpp"
#include "../../sdk/structs.hpp"
#include "../../helpers/math.hpp"
#include "../config/options.hpp"
#include "../../helpers/input.hpp"
#include <algorithm>

void Misc::UpdateWorldTextures() {
	if (Cfg.misc_nightmode) {
		if (!NightmodeDone) {
			NightMode();
			NightmodeDone = true;
		}
	}
}

void Misc::NightMode() {
	static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
	r_DrawSpecificStaticProp->SetValue(Cfg.misc_nightmode);
	auto sv_skyname = g_CVar->FindVar("sv_skyname");
	sv_skyname->SetValue(Cfg.misc_nightmode ? "sky_csgo_night02" : OldSkyname.c_str());

	for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i)) {
		IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

		if (!pMaterial) continue;

		const char* group = pMaterial->GetTextureGroupName();
		const char* name = pMaterial->GetName();
		float world_textures = Cfg.misc_nightmode ? 0.10f : 1.f;
		float staticprop = Cfg.misc_nightmode ? 0.30f : 1.f;
		float palace_pillars = Cfg.misc_nightmode ? 0.30f : 1.f;
		if (strstr(group, "World textures")) pMaterial->ColorModulate(world_textures, world_textures, world_textures);	
		if (strstr(group, "StaticProp")) pMaterial->ColorModulate(staticprop, staticprop, staticprop);	
		if (strstr(name, "models/props/de_dust/palace_bigdome")) pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Cfg.misc_nightmode);	
		if (strstr(name, "models/props/de_dust/palace_pillars")) pMaterial->ColorModulate(palace_pillars, palace_pillars, palace_pillars);	
		if (strstr(group, "Particle textures")) pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Cfg.misc_nightmode);		
	}
}

void Misc::Chatspam() {
	static DWORD lastspammed = 0;
	std::vector<std::string> chatspams = {
		"go have sexo with killaura at killaura.com",

	};

	if (GetTickCount64() - lastspammed > 800) {
		lastspammed = GetTickCount64();
		auto say = "say ";
		std::string p = say + chatspams[rand() % chatspams.size()];
		g_EngineClient->ServerCmd(p.data());
	}
}

void Misc::Radiospam() {
	static DWORD lastspammed = 0;
	if (GetTickCount64() - lastspammed > 800) {
		lastspammed = GetTickCount64();
		g_EngineClient->ServerCmd("getout");
	}
}

void Misc::Namespam() {

}

void Misc::Clantag() {
	static auto was_on = false;

	if (!Cfg.misc_clantag) {
		if (was_on) Utils::SetClantag("");
		was_on = false;
		return;
	}

	was_on = true;

	static int prevtime;
	static int prevval;

	if (int(g_GlobalVars->curtime * 2.9) != prevtime) {
		if (prevval != int(g_GlobalVars->curtime * 2.9) % 30) {
			prevval = int(g_GlobalVars->curtime * 2.9) % 30;
			switch (int(g_GlobalVars->curtime * 2.9) % 30)
			{
			case 1: { Utils::SetClantag("killaura ");  break; }
			case 2: { Utils::SetClantag("killaura ");  break; }
			case 3: { Utils::SetClantag("killaura ");  break; }
			case 4: { Utils::SetClantag("killaura ");  break; }
			case 5: { Utils::SetClantag("killaura ");  break; }
			case 6: { Utils::SetClantag("killaura ");  break; }

			default:;
			}
		}
	}

	prevtime = int(g_GlobalVars->curtime);
}

void Misc::BunnyHop(CUserCmd* cmd) {
	static bool jumped_last_tick = false;
	static bool should_fake_jump = false;

	if (!jumped_last_tick && should_fake_jump) {
		should_fake_jump = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP) {
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND) {
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else if (g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER) {
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else {
			cmd->buttons &= ~IN_JUMP;
			jumped_last_tick = false;
		}
	}
	else {
		jumped_last_tick = false;
		should_fake_jump = false;
	}
}

void Misc::AutoStrafe(CUserCmd* cmd) {
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || g_LocalPlayer->m_nMoveType() != MOVETYPE_WALK) return;

	bool on_ground = (g_LocalPlayer->m_fFlags() & FL_ONGROUND) && !(cmd->buttons & IN_JUMP);
	if (on_ground) return;
	
	static auto side = 1.0f;
	side = -side;

	auto velocity = g_LocalPlayer->m_vecVelocity();
	velocity.z = 0.0f;

	QAngle wish_angle = cmd->viewangles;

	auto speed = velocity.Length2D();
	auto ideal_strafe = std::clamp(RAD2DEG(atan(15.f / speed)), 0.0f, 90.0f);

	if (cmd->forwardmove > 0.0f) cmd->forwardmove = 0.0f;

	static auto cl_sidespeed = g_CVar->FindVar("cl_sidespeed");

	static float old_yaw = 0.f;
	auto yaw_delta = std::remainderf(wish_angle.yaw - old_yaw, 360.0f);
	auto abs_angle_delta = abs(yaw_delta);
	old_yaw = wish_angle.yaw;

	if (abs_angle_delta <= ideal_strafe || abs_angle_delta >= 30.0f) {
		QAngle velocity_direction;
		Math::VectorAngles(velocity, velocity_direction);
		auto velocity_delta = std::remainderf(wish_angle.yaw - velocity_direction.yaw, 360.0f);
		auto retrack = std::clamp(RAD2DEG(atan(30.0f / speed)), 0.0f, 90.0f) * Cfg.misc_bhop_speed;
		if (velocity_delta <= retrack || speed <= 15.0f) {
			if (-(retrack) <= velocity_delta || speed <= 15.0f) {
				wish_angle.yaw += side * ideal_strafe;
				cmd->sidemove = cl_sidespeed->GetFloat() * side;
			}
			else {
				wish_angle.yaw = velocity_direction.yaw - retrack;
				cmd->sidemove = cl_sidespeed->GetFloat();
			}
		}
		else {
			wish_angle.yaw = velocity_direction.yaw + retrack;
			cmd->sidemove = -cl_sidespeed->GetFloat();
		}

		Math::MovementFix(cmd, wish_angle, cmd->viewangles);
	}
	else if (yaw_delta > 0.0f) {
		cmd->sidemove = -cl_sidespeed->GetFloat();
	}
	else if (yaw_delta < 0.0f) {
		cmd->sidemove = cl_sidespeed->GetFloat();
	}
}

void Misc::EdgeJump(CUserCmd* cmd) {

}

void Misc::LeftKnife() {
	if (!Cfg.misc_leftknife) return;

	C_BaseCombatWeapon* weapon = g_LocalPlayer->m_hActiveWeapon();

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive()) {
		g_EngineClient->ExecuteClientCmd("cl_righthand 1");
		return;
	}

	if (weapon->IsKnife()) g_EngineClient->ExecuteClientCmd("cl_righthand 0");	
	else g_EngineClient->ExecuteClientCmd("cl_righthand 1");	
}

void Misc::SniperCrosshair() {
	static ConVar* weapon_debug_spread_show = g_CVar->FindVar("weapon_debug_spread_show");
	int expected = Cfg.esp_snipercrosshair && !g_LocalPlayer->m_bIsScoped() ? 3 : 0;
	if (expected != weapon_debug_spread_show->GetInt()) weapon_debug_spread_show->SetValue(expected);
}

void Misc::RecoilCrosshair() {
	static ConVar* cl_crosshair_recoil = g_CVar->FindVar("cl_crosshair_recoil");
	cl_crosshair_recoil->SetValue(Cfg.esp_recoilcrosshair);
}

void Misc::correct_mouse(CUserCmd* cmd) {
	static ConVar* m_yaw = m_yaw = g_CVar->FindVar("m_yaw");
	static ConVar* m_pitch = m_pitch = g_CVar->FindVar("m_pitch");
	static ConVar* sensitivity = sensitivity = g_CVar->FindVar("sensitivity");
	static QAngle m_angOldViewangles = g_ClientState->viewangles;

	float delta_x = std::remainderf(cmd->viewangles.pitch - m_angOldViewangles.pitch, 360.0f);
	float delta_y = std::remainderf(cmd->viewangles.yaw - m_angOldViewangles.yaw, 360.0f);

	if (delta_x != 0.0f) {
		float mouse_y = -((delta_x / m_pitch->GetFloat()) / sensitivity->GetFloat());
		short mousedy;
		if (mouse_y <= 32767.0f) {
			if (mouse_y >= -32768.0f) {
				if (mouse_y >= 1.0f || mouse_y < 0.0f) {
					if (mouse_y <= -1.0f || mouse_y > 0.0f)
						mousedy = static_cast<short>(mouse_y);
					else mousedy = -1;
				} else mousedy = 1;			
			} else mousedy = 0x8000u;		
		} else mousedy = 0x7FFF;
		cmd->mousedy = mousedy;
	}
	if (delta_y != 0.0f) {
		float mouse_x = -((delta_y / m_yaw->GetFloat()) / sensitivity->GetFloat());
		short mousedx;
		if (mouse_x <= 32767.0f) {
			if (mouse_x >= -32768.0f) {
				if (mouse_x >= 1.0f || mouse_x < 0.0f) {
					if (mouse_x <= -1.0f || mouse_x > 0.0f)
						mousedx = static_cast<short>(mouse_x);
					else mousedx = -1;
				} else mousedx = 1;				
			} else mousedx = 0x8000u;		
		} else mousedx = 0x7FFF;	
		cmd->mousedx = mousedx;
	}
}

void Misc::DisablePanoramaBlur() {
	static ConVar* blur = g_CVar->FindVar("@panorama_disable_blur");
	blur->SetValue(Cfg.fps_boost_selected[0]);
}

void Misc::No3dSky() {
	static ConVar* sky = g_CVar->FindVar("r_3dsky");
	sky->SetValue(!Cfg.fps_boost_selected[1]);
}

void Misc::NoShadows() {
	static ConVar* shadows = g_CVar->FindVar("cl_csm_enabled");
	shadows->SetValue(!Cfg.fps_boost_selected[2]);
}

void Misc::DisablePostProcessing() {
	if (Cfg.fps_boost_selected[3]) {
		
	}
}

extern float next_lby;
extern float side;
void Misc::Desync(CUserCmd* cmd, bool* send_packet) {
	if (cmd->buttons & (IN_ATTACK | IN_ATTACK2 | IN_USE) ||
		g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP
		|| !g_LocalPlayer->IsAlive())
		return;

	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();
	if (!weapon) return;

	auto weapon_index = weapon->m_Item().m_iItemDefinitionIndex();
	if ((weapon_index == WEAPON_GLOCK || weapon_index == WEAPON_FAMAS) && weapon->m_flNextPrimaryAttack() >= g_GlobalVars->curtime)
		return;

	auto weapon_data = weapon->GetCSWeaponData();

	if (weapon_data->WeaponType == WEAPONTYPE_GRENADE) {
		if (!weapon->m_bPinPulled()) {
			float throwTime = weapon->m_fThrowTime();
			if (throwTime > 0.f)
				return;
		}

		if ((cmd->buttons & IN_ATTACK) || (cmd->buttons & IN_ATTACK2)) {
			if (weapon->m_fThrowTime() > 0.f)
				return;
		}
	}

	static bool broke_lby = false;
	QAngle OldAngles = cmd->viewangles;

	if (InputSys::Get().WasKeyPressed(Cfg.misc_desync_bind)) 
		side = -side;
	
	switch (Cfg.misc_desync) {
	case 1: {
		float minimal_move = g_LocalPlayer->m_fFlags() & IN_DUCK ? 3.0f : 1.0f;
		bool should_move = g_LocalPlayer->m_vecVelocity().Length2D() <= 0.0f || 
			std::fabsf(g_LocalPlayer->m_vecVelocity().z) <= 100.0f;

		if (cmd->command_number % 2 == 1) {
			cmd->viewangles.yaw += 120.0f * side;
			if (should_move)
				cmd->sidemove -= minimal_move;
			*send_packet = false;
		}
		else if (should_move) 
			cmd->sidemove += minimal_move;	
	} break;
	case 2: {
		if (next_lby >= g_GlobalVars->curtime) {
			if (!broke_lby && *send_packet && g_ClientState->m_nChokedCommands > 0)
				return;

			broke_lby = false;
			*send_packet = false;
			cmd->viewangles.yaw += 120.0f * side;
		}
		else {
			broke_lby = true;
			*send_packet = false;
			cmd->viewangles.yaw += 120.0f * -side;
		}
	} break;
	}

	Math::FixAngles(cmd->viewangles);
	Math::MovementFix(cmd, OldAngles, cmd->viewangles);
}

void Misc::AgentChanger(int v_char, int p_id) {
	static int originalIdx = 0;

	if (!g_LocalPlayer) {
		originalIdx = 0;
		return;
	}

	constexpr std::array models{
	"models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
	"models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
	"models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
	"models/player/custom_player/legacy/ctm_sas_variantf.mdl",
	"models/player/custom_player/legacy/ctm_st6_variante.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantg.mdl",
	"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantk.mdl",
	"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantg.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianth.mdl",
	"models/player/custom_player/legacy/tm_balkan_varianti.mdl",
	"models/player/custom_player/legacy/tm_balkan_variantj.mdl",
	"models/player/custom_player/legacy/tm_leet_variantf.mdl",
	"models/player/custom_player/legacy/tm_leet_variantg.mdl",
	"models/player/custom_player/legacy/tm_leet_varianth.mdl",
	"models/player/custom_player/legacy/tm_leet_varianti.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
	"models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
	"models/player/custom_player/legacy/tm_phoenix_varianth.mdl"
	};

	auto entity = (C_BaseEntity*)g_EntityList->GetClientEntity(p_id);

	if (!entity) return;

	if (entity->IsPlayer()) {
		auto ModelIndex = g_MdlInfo->GetModelIndex(models[v_char]);

		if (ModelIndex) entity->SetModelIndex(ModelIndex);

		if (const auto ragdoll = g_LocalPlayer->get_entity_from_handle(g_LocalPlayer->m_hRagdoll()))
			ragdoll->SetModelIndex(ModelIndex);
	}
}
