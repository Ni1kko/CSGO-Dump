#include "logger.hpp"
#include "../misc/misc.hpp"
#include "../../helpers/math.hpp"
#include "../../helpers/utils.hpp"
#include "../../globals.hpp"

std::map<std::string, std::string> event_to_normal =
{
	//others
	////////////////////////////////////////////////
	{ "weapon_taser", "zeus" },
	{ "item_kevlar", "kevlar" },
	{ "item_defuser", "defuse kit" },
	{ "item_assaultsuit", "full armor" },
	////////////////////////////////////////////////
	//


	//pistols
	////////////////////////////////////////////////
	{ "weapon_p250", "p250" },
	{ "weapon_tec9", "tec-9" },
	{ "weapon_cz75a", "cz75a" },
	{ "weapon_glock", "glock" },
	{ "weapon_elite", "elite" },
	{ "weapon_deagle", "deagle" },
	{ "weapon_hkp2000", "p2000" },
	{ "weapon_usp_silencer", "usp" },
	{ "weapon_revolver", "rovelver" },
	{ "weapon_fiveseven", "five-seven" },
	////////////////////////////////////////////////
	//

	//pp
	////////////////////////////////////////////////
	{ "weapon_mp9", "mp-9" },
	{ "weapon_mac10", "mac-10" },
	{ "weapon_mp7", "mp-7" },
	{ "weapon_mp5sd", "mp5-sd" },
	{ "weapon_ump45", "ump-45" },
	{ "weapon_p90", "p90" },
	{ "weapon_bizon", "bizon" },
	////////////////////////////////////////////////
	//

	//rifles
	////////////////////////////////////////////////
	{ "weapon_famas", "famas" },
	{ "weapon_m4a1_silencer", "m4a1-s" },
	{ "weapon_m4a1", "m4a1" },
	{ "weapon_ssg08", "ssg08" },
	{ "weapon_aug", "aug" },
	{ "weapon_awp", "awp" },
	{ "weapon_scar20", "scar20" },
	{ "weapon_galilar", "galil" },
	{ "weapon_ak47", "ak47" },
	{ "weapon_sg556", "sg553" },
	{ "weapon_g3sg1", "g3sg1" },
	////////////////////////////////////////////////
	//

	//have
	////////////////////////////////////////////////
	{ "weapon_nova", "nova" },
	{ "weapon_xm1014", "xm1014" },
	{ "weapon_sawedoff", "sawedoff" },
	{ "weapon_m249", "m249" },
	{ "weapon_negev", "negev" },
	{ "weapon_mag7", "mag7" },
	////////////////////////////////////////////////
	//

	//granades
	////////////////////////////////////////////////
	{ "weapon_flashbang", "flash" },
	{ "weapon_smokegrenade", "smoke" },
	{ "weapon_molotov", "molotov" },
	{ "weapon_incgrenade", "inc" },
	{ "weapon_decoy", "decoy" },
	{ "weapon_hegrenade", "hae" },
	////////////////////////////////////////////////
	//
};

std::string bomb_site(int site_id, std::string map_name)
{
	if (map_name == "de_inferno")
	{
		if (site_id == 333)
		{
			return "A";
		}
		else if (site_id == 422)
		{
			return "B";
		}
	}
	else if (map_name == "de_mirage")
	{
		if (site_id == 425)
		{
			return "A";
		}
		else if (site_id == 426)
		{
			return "B";
		}
	}
	else if (map_name == "de_dust2")
	{
		if (site_id == 281)
		{
			return "A";
		}
		else if (site_id == 282)
		{
			return "B";
		}
	}
	else if (map_name == "de_overpass")
	{
		if (site_id == 79)
		{
			return "A";
		}
		else if (site_id == 504)
		{
			return "B";
		}
	}
	else if (map_name == "de_vertigo")
	{
		if (site_id == 262)
		{
			return "A";
		}
		else if (site_id == 314)
		{
			return "B";
		}
	}
	else if (map_name == "de_nuke")
	{
		if (site_id == 154)
		{
			return "A";
		}
		else if (site_id == 167)
		{
			return "B";
		}
	}
	else if (map_name == "de_train")
	{
		if (site_id == 94)
		{
			return "A";
		}
		else if (site_id == 536)
		{
			return "B";
		}
	}
	else if (map_name == "de_cache")
	{
		if (site_id == 317)
		{
			return "A";
		}
		else if (site_id == 318)
		{
			return "B";
		}
	}
	else if (map_name == "de_cbble")
	{
		if (site_id == 216)
		{
			return "A";
		}
		else if (site_id == 107)
		{
			return "B";
		}
	}
	else if (map_name == "de_shortdust")
	{
		if (site_id == 217)
		{
			return "A";
		}
	}
	else if (map_name == "de_rialto")
	{
		if (site_id == 99)
		{
			return "A";
		}
	}
	else if (map_name == "de_lake")
	{
		if (site_id == 209)
		{
			return "A";
		}
	}

	return "unck";

}

struct notify_t
{
	float life_ime_local;
	float life_ime;
	std::string type;
	std::string message;
	Color c_type;
	Color c_message;
	float x;
	float y = -15;
	float max_x;
};

std::deque<notify_t> notifications;

notify_t find_notify(std::string pre_text, std::string body)
{
	for (size_t i = 0; i < notifications.size(); i++)
		if (notifications[i].type == pre_text && notifications[i].message == body)
			return notifications[i];
	return notify_t();
}

namespace notify
{
	void notify_events(IGameEvent* event) {
		static auto get_hitgroup_name = [](int hitgroup) -> std::string {
			switch (hitgroup)
			{
			case HITBOX_HEAD:
				return "head";
			case HITBOX_LEFT_FOOT:
				return "left leg";
			case HITBOX_RIGHT_FOOT:
				return "right leg";
			case HITBOX_CHEST:
				return "chest";
			case HITBOX_LOWER_CHEST:
				return "lower chest";
			case HITBOX_LEFT_HAND:
				return "left arm";
			case HITBOX_RIGHT_HAND:
				return "right arm";
			case HITBOX_NECK:
				return "neck";
			default:
				return "body";
			}
		};

		constexpr char* hasdefusekit[2] = { "without defuse kit.","with defuse kit." };
		constexpr char* hasbomb[2] = { "without the bomb.","with the bomb." };

		if (strstr(event->GetName(), "player_hurt"))
		{
			auto
				userid = event->GetInt("userid"),
				attacker = event->GetInt("attacker");

			if (!userid || !attacker)
				return;

			auto
				userid_id = g_EngineClient->GetPlayerForUserID(userid),
				attacker_id = g_EngineClient->GetPlayerForUserID(attacker);

			player_info_t userid_info, attacker_info;
			if (!g_EngineClient->GetPlayerInfo(userid_id, &userid_info))
				return;

			if (!g_EngineClient->GetPlayerInfo(attacker_id, &attacker_info))
				return;

			auto m_victim = C_BasePlayer::GetPlayerByUserId(userid_id);

			std::stringstream ss;
			if (attacker_id == g_LocalPlayer->EntIndex() && userid_id != g_LocalPlayer->EntIndex()) {
				ss << "you hurt " << userid_info.szName << " in the " << get_hitgroup_name(event->GetInt("hitgroup")) << " for " << event->GetInt("dmg_health");

				if (g_Misc->options.eventlogger_hurt) {
					screen::notify("Untrusted damage", ss.str(), g_Misc->colors.accent_color);
					console::notify("Untrusted damage", ss.str(), g_Misc->colors.accent_color);
				}
			}
			else if (userid_id == g_LocalPlayer->EntIndex() && attacker_id != g_LocalPlayer->EntIndex())
			{
				ss << "you took " << event->GetInt("dmg_health") << " damage from " << attacker_info.szName << "in the " << get_hitgroup_name(event->GetInt("hitgroup"));

				if (g_Misc->options.eventlogger_hurt) {
					screen::notify("Untrusted damage", ss.str(), g_Misc->colors.accent_color);
					console::notify("Untrusted damage", ss.str(), g_Misc->colors.accent_color);
				}
			}
		}

		if (strstr(event->GetName(), "item_purchase"))
		{
			auto userid = event->GetInt("userid");

			if (!userid)
				return;

			auto userid_id = g_EngineClient->GetPlayerForUserID(userid);

			player_info_t userid_info;
			if (!g_EngineClient->GetPlayerInfo(userid_id, &userid_info))
				return;

			auto m_player = C_BasePlayer::GetPlayerByUserId(userid_id);

			if (!g_LocalPlayer || !m_player)
				return;

			if (m_player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				return;

			std::string weapon_name = event->GetString("weapon");

			std::stringstream ss;
			ss << userid_info.szName << " purchased a(n) " << event_to_normal[weapon_name];

			if (g_Misc->options.eventlogger_player_buy) {
				screen::notify("buy", ss.str(), g_Misc->colors.accent_color);
				console::notify("buy", ss.str(), g_Misc->colors.accent_color);
			}
		}

		if (strstr(event->GetName(), "bomb_beginplant"))
		{
			auto userid = event->GetInt("userid");

			if (!userid)
				return;

			auto userid_id = g_EngineClient->GetPlayerForUserID(userid);

			player_info_t userid_info;
			if (!g_EngineClient->GetPlayerInfo(userid_id, &userid_info))
				return;

			auto m_player = C_BasePlayer::GetPlayerByUserId(userid_id);

			if (!m_player)
				return;

			int site_id = event->GetInt("site");

			std::string namemap = g_EngineClient->GetLevelNameShort();

			std::string bomb_site_s = bomb_site(site_id, namemap);

			std::stringstream ss;
			if (bomb_site_s != "unck")
			{
				ss << userid_info.szName << " has begin planting the bomb at site " << bomb_site_s << ".";
			}
			else if (bomb_site_s == "unck")
			{
				ss << userid_info.szName << " has begin planting the bomb.";
			}

			if (g_Misc->options.eventlogger_planting) {
				screen::notify("info", ss.str(), g_Misc->colors.accent_color);
				console::notify("info", ss.str(), g_Misc->colors.accent_color);
			}
		}

		if (strstr(event->GetName(), "bomb_planted"))
		{
			auto userid = event->GetInt("userid");

			if (!userid)
				return;

			auto userid_id = g_EngineClient->GetPlayerForUserID(userid);

			player_info_t userid_info;
			if (!g_EngineClient->GetPlayerInfo(userid_id, &userid_info))
				return;

			auto m_player = C_BasePlayer::GetPlayerByUserId(userid_id);

			if (!m_player)
				return;

			int site_id = event->GetInt("site");

			std::string namemap = g_EngineClient->GetLevelNameShort();

			std::string bomb_site_s = bomb_site(site_id, namemap);

			std::stringstream ss;
			if (bomb_site_s != "unck")
			{
				ss << userid_info.szName << " has planted bomb at site " << bomb_site_s << ".";
			}
			else if (bomb_site_s == "unck")
			{
				ss << userid_info.szName << " has planted the bomb.";
			}

			if (g_Misc->options.eventlogger_planting) {
				screen::notify("info", ss.str(), g_Misc->colors.accent_color);
				console::notify("info", ss.str(), g_Misc->colors.accent_color);
			}
		}

		if (strstr(event->GetName(), "bomb_begindefuse"))
		{
			auto userid = event->GetInt("userid");

			if (!userid)
				return;

			auto userid_id = g_EngineClient->GetPlayerForUserID(userid);

			player_info_t userid_info;
			if (!g_EngineClient->GetPlayerInfo(userid_id, &userid_info))
				return;

			auto m_player = C_BasePlayer::GetPlayerByUserId(userid_id);

			if (!m_player)
				return;

			std::stringstream ss;
			ss << userid_info.szName << " has began defusing the bomb kits -> " << hasdefusekit[event->GetBool("haskit")];

			if (g_Misc->options.eventlogger_defusing) {
				screen::notify("info", ss.str(), g_Misc->colors.accent_color);
				console::notify("info", ss.str(), g_Misc->colors.accent_color);
			}
		}
	}

	void render()
	{
		if (notifications.empty())
			return;

		float last_y = 0;

		for (size_t i = 0; i < notifications.size(); i++)
		{
			auto& notify = notifications.at(i);

			const auto pre = notify.type.c_str();
			const auto text = notify.message.c_str();
			ImVec2 textSize = g_pSecondFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, notify.type.c_str());

			std::string all_text;
			all_text += pre;
			all_text += " ";
			all_text += text;

			ImVec2 all_textSize = g_pSecondFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, all_text.c_str());

			notify.y = Utils::Lerp(notify.y, i * 15, 0.05f);

			if (notify.y > CheatGVars::SHeight + 16) {
				continue;
			}

			if (Utils::EpochTime() - notify.life_ime_local > notify.life_ime)
			{
				if ((notify.x + all_textSize.x + 16) < 0) {
					notifications.erase(notifications.begin() + i);
					continue;
				}

				notify.max_x = all_textSize.x + 16;

				notify.x = Utils::Lerp(notify.x, (notify.max_x * -1) - 10, 0.05f);

				int procent_x = (100 * (notify.max_x + notify.x)) / notify.max_x;

				auto opacity = int((255 / 100) * procent_x);

				if (procent_x >= 0 && procent_x <= 100)
				{
					notify.c_message = Color(notify.c_message, opacity);
					notify.c_type = Color(notify.c_type, opacity);
				}
				else
				{
					notify.c_message = Color(notify.c_message, 255);
					notify.c_type = Color(notify.c_type, 255);
				}
			}

			float box_w = (float)fabs(0 - (all_textSize.x + 16));


			Render::Get().RenderBoxFilled(0.f, last_y + notify.y - 1, notify.x + all_textSize.x + 16, last_y + notify.y + all_textSize.y + 2, Color(0, 0, 0, 77));
			Render::Get().RenderBoxFilled(notify.x + all_textSize.x + 16, last_y + notify.y - 1, notify.x + all_textSize.x + 18, last_y + notify.y + all_textSize.y + 2, Color(notify.c_type, 150));
			Render::Get().RenderText(pre, ImVec2(notify.x + 3, last_y + notify.y), 12.f, notify.c_type, false, false);
			Render::Get().RenderText(text, ImVec2(notify.x + 13 + textSize.x, last_y + notify.y), 12.f, notify.c_message, false, false);
		}
	}


	namespace console {
		void clear() {
			g_EngineClient->ExecuteClientCmd("clear");
		}

		void notify(std::string pre, std::string msg, Color clr_pre) {
			if (!g_EngineClient || !g_CVar)
				return;
			g_CVar->ConsolePrintf("[ %s ] %s\n", pre.data(), msg.data());
		}
	}

	namespace screen
	{
		void notify(std::string pre, std::string text, Color color_pre, Color color_text, int life_time) {
			std::string type_buf;
			type_buf += "[ ";
			type_buf += pre;
			type_buf += " ]";

			notifications.push_front(notify_t{ static_cast<float>(Utils::EpochTime()), (float)life_time, type_buf, text, color_pre, color_text });
		}
	}
}