#include "EventLogging.h"
const char* team[]{ "",""," (T)"," (CT)" };
const char* hasdefusekit[]{ "without defuse kit.","with defuse kit." };
const char* hasbomb[]{ "without the bomb.","with the bomb." };
#include "..\..\SDK\ISurface.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\config.h"

int epoch_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
enum hitgroups {
	hitgroup_generic = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_leftarm = 4,
	hitgroup_rightarm = 5,
	hitgroup_leftleg = 6,
	hitgroup_rightleg = 7,
	hitgroup_gear = 10
};
const char* hitgroup_name(int hitgroup) {
	switch (hitgroup) {
	case hitgroup_head:
		return "head";
	case hitgroup_leftleg:
		return "left leg";
	case hitgroup_rightleg:
		return "right leg";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
		return "left arm";
	case hitgroup_rightarm:
		return "right arm";
	default:
		return "body";
	}
}

void c_event_logs::run() {
	if (logs.empty())
		return;

	auto last_y = 2;
	auto last_x = 4;
	for (unsigned int i = 0; i < logs.size(); i++) {
		auto& log = logs.at(i);

		if (epoch_time() - log.log_time > 5000) {
			float factor = (log.log_time + 3100) - epoch_time();
			factor /= 1000;

			auto opacity = int(255 * factor);

			RECT text_size = g_pSurface->GetTextSizeRect(Globals::SmallText, log.message.c_str());
			if (epoch_time() - log.log_time > 6000) {
				logs.erase(logs.begin() + i);
				continue;
			}

			log.x -= 1 * (factor * 1.25);
			log.y += 1 * (factor * 1.25);
		}

		const auto text = log.message.c_str();
		g_pSurface->DrawT(last_x - log.x, last_y, Color(255, 255, 255, 255), Globals::SmallText, false, text);

		last_y += 14;
	}
}


void c_event_logs::event_item_purchase(IGameEvent* event) {
	if (!c_config::get().event_logger[0])
		return;

	auto userid = event->GetInt("userid");

	if (!userid) {
		return;
	}

	auto engine_userid = g_pEngine->GetPlayerForUserID(userid);

	PlayerInfo_t info;
	g_pEngine->GetPlayerInfo(engine_userid, &info);

	auto local_player = Globals::LocalPlayer;
	auto entity = g_pEntityList->GetClientEntity(engine_userid);

	if (!local_player || !entity) {
		return;
	}

	if (entity->GetTeam() == local_player->GetTeam()) {
		return;
	}

	std::string player_name = info.szName;
	std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

	std::stringstream ss;
	std::string weapon = event->GetString("weapon");

	for (int i = 0; i < player_name.size(); ++i)
	{
		if (!((player_name[i] >= 'a' && player_name[i] <= 'z') || (player_name[i] >= 'A' && player_name[i] <= 'Z')))
		{
			player_name[i] = '\0';
		}
	}

	weapon.erase(0, 7);
	ss << player_name << " purchased a " << weapon;

	add(ss.str(), Color(255, 255, 255, 255));
}

void c_event_logs::event_player_hurt(IGameEvent* event) {
	if (!c_config::get().event_logger[1])
		return;

	if (!event) {
		return;
	}

	auto victim = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));
	auto local_player = Globals::LocalPlayer;
	auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));

	if (attacker == local_player) {
		auto entity = g_pEntityList->GetClientEntity(victim);
		if (!entity) {
			return;
		}

		PlayerInfo_t info;
		g_pEngine->GetPlayerInfo(entity->EntIndex(), &info);

		auto hitbox = event->GetInt("hitgroup");
		if (!hitbox) {
			return;
		}

		auto damage = event->GetInt("dmg_health");
		if (!damage) {
			return;
		}

		auto health = event->GetInt("health");
		if (!health && health != 0) {
			return;
		}

		auto hitgroup = hitgroup_name(hitbox);
		std::string player_name = info.szName;
		std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

		std::stringstream ss;

		for (int i = 0; i < player_name.size(); ++i)
		{
			if (!((player_name[i] >= 'a' && player_name[i] <= 'z') || (player_name[i] >= 'A' && player_name[i] <= 'Z')))
			{
				player_name[i] = '\0';
			}
		}

		ss << "hit " << player_name.c_str() << " in the " << hitgroup << " for " << damage << " damage";
		ss << " (" << health << " health remaining).";

		c_event_logs::get().add(ss.str(), Color(255, 255, 255, 255));
	}
}

void c_event_logs::event_player_death(IGameEvent* event) {
	if (!c_config::get().event_logger[2])
		return;

	if (!event) {
		return;
	}

	auto victim = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));
	auto local_player = Globals::LocalPlayer;
	auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));

	if (attacker == local_player) {
		auto entity = g_pEntityList->GetClientEntity(victim);
		if (!entity) {
			return;
		}

		PlayerInfo_t info;
		g_pEngine->GetPlayerInfo(entity->EntIndex(), &info);

		std::string player_name = info.szName;
		std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);

		std::stringstream ss;

		for (int i = 0; i < player_name.size(); ++i)
		{
			if (!((player_name[i] >= 'a' && player_name[i] <= 'z') || (player_name[i] >= 'A' && player_name[i] <= 'Z')))
			{
				player_name[i] = '\0';
			}
		}

		ss << "killed " << player_name;

		c_event_logs::get().add(ss.str(), Color(255, 255, 255, 255));
	}
}


void c_event_logs::add(std::string text, Color colors) {
	logs.push_front(loginfo_t(epoch_time(), text, colors));
}