#include "misc.hpp"
#include "..//options.hpp"
#include "..//menu.hpp"
#include "..//helpers/math.hpp"
#include "..//helpers/input.hpp"
#include "..//helpers/memory.hpp"
#include "..//valve_sdk/csgostructs.hpp"
#include <filesystem>

void Misc::BunnyHop(CUserCmd* cmd)
{
	if (!g_Options.misc.bhop)
		return;

	if (g_LocalPlayer->m_nMoveType() != MOVETYPE_WALK)
		return;

	if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;

	AutoStafe(cmd);
}

void Misc::AutoStafe(CUserCmd* cmd)
{
	if (!g_Options.misc.autostrafe)
		return;

	if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
		return;

	static auto side = 1.f;
	side = -side;

	auto velocity = g_LocalPlayer->m_vecVelocity();

	QAngle wish_angle = cmd->viewangles;

	auto speed = velocity.Length2D();
	auto ideal_strafe = std::clamp(RAD2DEG(atan(15.f / speed)), 0.f, 90.f);

	cmd->forwardmove = 0.f;

	static auto cl_sidespeed = g_CVar->FindVar("cl_sidespeed");

	static float old_yaw = 0.f;
	auto yaw_delta = std::remainderf(wish_angle.yaw - old_yaw, 360.f);
	auto abs_yaw_delta = abs(yaw_delta);
	old_yaw = wish_angle.yaw;

	const auto cl_sidespeed_value = cl_sidespeed->GetFloat();

	if (abs_yaw_delta <= ideal_strafe || abs_yaw_delta >= 30.f)
	{
		QAngle velocity_direction;
		Math::VectorAngles(velocity, velocity_direction);
		auto velocity_delta = std::remainderf(wish_angle.yaw - velocity_direction.yaw, 360.0f);
		auto retrack = std::clamp(RAD2DEG(atan(30.f / speed)), 0.f, 90.f) * 2.f;
		if (velocity_delta <= retrack || speed <= 15.f)
		{
			if (-retrack <= velocity_delta || speed <= 15.0f)
			{
				wish_angle.yaw += side * ideal_strafe;
				cmd->sidemove = cl_sidespeed_value * side;
			}
			else
			{
				wish_angle.yaw = velocity_direction.yaw - retrack;
				cmd->sidemove = cl_sidespeed_value;
			}
		}
		else
		{
			wish_angle.yaw = velocity_direction.yaw + retrack;
			cmd->sidemove = -cl_sidespeed_value;
		}

		Math::CorrectMovement(cmd, wish_angle, cmd->viewangles);
	}
	else if (yaw_delta > 0.f)
		cmd->sidemove = -cl_sidespeed_value;
	else
		cmd->sidemove = cl_sidespeed_value;
}

void Misc::Fakelag(CUserCmd* cmd, bool& bSendPacket)
{
	if (!g_Options.misc.fakelag.enabled || (g_Options.misc.fakelag.onkey && !g_InputSys->IsKeyDown(g_Options.misc.fakelag.hotkey)))
		return;

	bSendPacket = g_ClientState->chokedcommands >= g_Options.misc.fakelag.ticks;
}
//shit for hud modulation

float r, g, b, a;
bool sprop = false;

static int frameCounter = 0;
struct ModColor
{
	float color[3];
	float alpha;

	void SetColorFloat(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	void SetColorFloat(float col[3])
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
	}

	void SetColor(Color col)
	{
		color[0] = col.r() / 255.f;
		color[1] = col.g() / 255.f;
		color[2] = col.b() / 255.f;
	}

	void GetColorFloat(float col[3])
	{
		col[0] = color[0];
		col[1] = color[1];
		col[2] = color[2];
	}

	Color GetColor()
	{
		return Color(color[0], color[1], color[2]);
	}

	ModColor(float r, float g, float b, float alph)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		alpha = alph;
	}

	ModColor(float col[3], float alph)
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
		alpha = alph;
	}
};
std::unordered_map<MaterialHandle_t, std::pair<ModColor, ModColor>> vguiMaterials;
bool hardDisable = false;

void Misc::HUDModulation()
{
	if (hardDisable || frameCounter++ < 20)
		return;

	frameCounter = 0;

	for (MaterialHandle_t i = g_MatSystem->pFirstMaterial(); i != g_MatSystem->pInvalidMaterial(); i = g_MatSystem->pNextMaterial(i))
	{
		auto mat = g_MatSystem->pGetMaterial(i);

		Color originalColor = Color(255, 255, 255, 255);
		Color vguiColor = Color(25, 31, 125, 220);

		//Check if a VGUI material
		if (mat && (strcmp(mat->GetName(), "vgui_white") == 0 || strstr(mat->GetName(), "vgui/hud/800corner") != NULL)) {
			//Not yet in the list
			if (vguiMaterials.find(i) == vguiMaterials.end())
			{
				mat->GetColorModulation(&r, &g, &b);
				originalColor = Color(r, g, b, mat->GetAlphaModulation() * 255);
				vguiMaterials.emplace(i, std::make_pair(ModColor(1, 1, 1, 1.f), ModColor(1, 1, 1, vguiColor.a() / 255.f)));
			}
			//Color has been changed
			if (vguiMaterials.at(i).second.GetColor() != vguiColor || vguiMaterials.at(i).second.alpha != vguiColor.a() / 255.f)
			{
				mat->ColorModulate(0.00392156862f * vguiColor[0], 0.00392156862f * vguiColor[1], 0.00392156862f * vguiColor[2]);
				mat->AlphaModulate(vguiColor.a() / 255.f);
				vguiMaterials.at(i).second.SetColor(vguiColor);
			}
		}
	}
}

void Misc::ToggleVGuiModulation(bool enable)
{
	if (!hardDisable && enable)
	{
		for (const auto& it : vguiMaterials)
		{
			auto mat = g_MatSystem->pGetMaterial(it.first);

			if (!mat)
				continue;

			mat->ColorModulate(it.second.second.color[0], it.second.second.color[1], it.second.second.color[2]);
			mat->AlphaModulate(it.second.second.alpha);
		}
	}
	else
	{
		for (const auto& it : vguiMaterials)
		{
			auto mat = g_MatSystem->pGetMaterial(it.first);

			if (!mat)
				continue;

			mat->ColorModulate(1, 1, 1);
			mat->AlphaModulate(1);
		}
	}
}
void Misc::NightMode()
{
	static bool NightModeDone = false;

	if (!g_EngineClient->IsInGame() || !g_LocalPlayer)
	{
		NightModeDone = false;
		return;
	}


	static std::string mapname = g_EngineClient->GetLevelNameShort();
	if (mapname != g_EngineClient->GetLevelNameShort())
	{
		NightModeDone = false;
		mapname = g_EngineClient->GetLevelNameShort();
	}

	static bool OldValue = g_Options.misc.nightmode;

	if (OldValue != g_Options.misc.nightmode)
		NightModeDone = false;

	if (!NightModeDone)
	{
		static auto LoadNamedSky = reinterpret_cast<void(__fastcall*)(const char*)>(g_Memory->LoadNamedSky);
		static auto sv_skyname = g_CVar->FindVar("sv_skyname");
		static auto r_3dsky = g_CVar->FindVar("r_3dsky");

		LoadNamedSky(g_Options.misc.nightmode ? "sky_csgo_night02" : sv_skyname->GetString());
		r_3dsky->SetValue(!g_Options.misc.nightmode);
		for (MaterialHandle_t i = g_MatSystem->pFirstMaterial();
			i != g_MatSystem->pInvalidMaterial();
			i = g_MatSystem->pNextMaterial(i))
		{
			auto pMaterial = g_MatSystem->pGetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World"))
			{
				if (g_Options.misc.nightmode)
					pMaterial->ColorModulate(0.18, 0.18, 0.15);
				else
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
			}
			else if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (g_Options.misc.nightmode)
				{
					pMaterial->ColorModulate(0.28, 0.28, 0.28);
				}
				else
				{
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
				}
			}
		}
		NightModeDone = true;
	}

	OldValue = g_Options.misc.nightmode;
}

void Misc::ThirdPerson()
{
	if (!g_LocalPlayer)
		return;

	if (g_InputSys->WasKeyPressed(g_Options.misc.thirdperson.hotkey) && !g_Menu->IsVisible())
		g_Options.misc.thirdperson.thirdpersonenabled = !g_Options.misc.thirdperson.thirdpersonenabled;

	
	if (g_Options.misc.thirdperson.thirdpersonenabled && g_LocalPlayer->IsAlive())
	{
		g_Input->m_fCameraInThirdPerson = true;		
	}
	else
	{
		g_Input->m_fCameraInThirdPerson = false;
	}
}

std::map<std::string, std::string> weaponNames = {
{ "weapon_ak47", "AK47" },
{ "weapon_aug", "AUG" },
{ "weapon_famas", "FAMAS" },
{ "weapon_m4a1", "M4A4" },
{ "weapon_m4a1_silencer", "M4A1-S" },
{ "weapon_sg556", "SG556" },
{ "weapon_galilar", "Galil AR" },

{ "weapon_awp", "AWP" },
{ "weapon_g3sg1", "G3SG1" },
{ "weapon_scar20", "SCAR20" },
{ "weapon_ssg08", "SSG08" },

{ "weapon_mac10", "MAC10" },
{ "weapon_mp7", "MP7" },
{ "weapon_mp9", "MP9" },
{ "weapon_bizon", "PP-Bizon" },
{ "weapon_p90", "P90" },
{ "weapon_ump45", "UMP-45" },
{ "weapon_mp5sd", "MP5-SD" },

{ "weapon_m249", "M249" },
{ "weapon_negev", "Negev" },

{ "weapon_hkp2000", "P2000" },
{ "weapon_p250", "P250" },
{ "weapon_elite", "Dual Berettas" },
{ "weapon_fiveseven", "Five-Seven" },
{ "weapon_glock", "Glock" },
{ "weapon_tec9", "Tec-9" },
{ "weapon_deagle", "Deagle" },
{ "weapon_cz75a", "CZ75 Auto" },
{ "weapon_revolver", "Revolver" },

{ "weapon_mag7", "MAG-7" },
{ "weapon_nova", "Nova" },
{ "weapon_sawedoff", "Sawed Off" },
{ "weapon_xm1014", "XM1014" },

{ "item_defuser", "Defuser" },
{ "item_kevlar", "Kevlar" },
{ "item_assaultsuit", "Helmet and Kevlar" },

{ "weapon_hegrenade", "High Explosive Grenade" },
{ "weapon_flashbang", "Flashbang" },
{ "weapon_molotov", "Molotov" },
{ "weapon_incgrenade", "Incendiary Grenade" },
{ "weapon_decoy", "Decoy Grenade" },
{ "weapon_smokegrenade", "Smoke Grenade" },

{ "weapon_taser", "Zeus x27" },
};

void Misc::BuyInfo(IGameEvent* pEvent)
{
	if (!g_Options.misc.buyinfo)
		return;

	const int buy_ent_id = g_EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"));
	C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(buy_ent_id);

	if (!entity || !entity->IsEnemy())
		return;

	const std::string weapon = pEvent->GetString("weapon");

	if (weapon == "weapon_unknown")
		return;

	std::string wepName;

	const auto it = weaponNames.find(weapon);
	if (it != weaponNames.end())
		wepName = it->second;
	else
		wepName = weapon;

	player_info_t info;
	g_EngineClient->GetPlayerInfo(buy_ent_id, &info);

	std::string message = info.szName;
	message += " bought an " + wepName;

	g_ChatElement->ChatPrintf(0, CHudChat::ChatFilters::CHAT_FILTER_NONE, message.c_str());
}

void Misc::SpectatorList()
{
	if (!g_Options.misc.spectator_list)
		return;

	std::string spectators;

	if (g_EngineClient->IsInGame() && g_LocalPlayer)
	{
		for (int i = 1; i <= g_GlobalVars->maxClients; i++)
		{
			auto ent = C_BasePlayer::GetPlayerByIndex(i);

			if (!ent || ent->IsAlive() || ent->IsDormant())
				continue;

			auto target = (C_BasePlayer*)ent->m_hObserverTarget();

			if (!target || target != g_LocalPlayer)
				continue;

			if (ent == target)
				continue;

			auto info = ent->GetPlayerInfo();

			spectators += std::string(info.szName) + "\n";
		}
	}

	const auto size = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, NULL, spectators.c_str());

	ImGui::SetNextWindowSize(ImVec2(300, size.y + ImGui::GetFontSize() * 3));
	if (ImGui::Begin("Spectator List", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y * 3 + ImGui::GetFontSize()));
		ImGui::Text(spectators.c_str());
	}
	ImGui::End();
}