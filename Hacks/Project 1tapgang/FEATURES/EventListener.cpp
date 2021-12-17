#include "../includes.h"

#include "../UTILS/interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../SDK/ISurface.h"
#include "../UTILS/render.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/Aimbot.h"
#include "../sounds.h"
#include "EventListener.h"
#include <playsoundapi.h>
#pragma comment(lib, "Winmm.lib")

CGameEvents::ItemPurchaseListener item_purchase_listener;
CGameEvents::PlayerHurtListener player_hurt_listener;
CGameEvents::BulletImpactListener bullet_impact_listener;
CGameEvents::PlayerDeathListener player_death_listener;
CGameEvents::WeaponFiredListener weapon_fired_listener;

void CGameEvents::InitializeEventListeners()
{
	INTERFACES::GameEventManager->AddListener(&item_purchase_listener, "item_purchase", false);
	INTERFACES::GameEventManager->AddListener(&player_hurt_listener, "player_hurt", false);
	INTERFACES::GameEventManager->AddListener(&bullet_impact_listener, "bullet_impact", false);
	INTERFACES::GameEventManager->AddListener(&player_death_listener, "player_death", false);
	INTERFACES::GameEventManager->AddListener(&weapon_fired_listener, "weapon_fire", false);
}

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "left leg";
	case HITGROUP_RIGHTLEG:
		return "right leg";
	case HITGROUP_STOMACH:
		return "stomach";
	default:
		return "body";
	}
}

float randnum(int Min, int Max)
{
	return ((rand() % (Max - Min)) + Min);
}

void CGameEvents::WeaponFiredListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	int iUser = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid"));

	auto engine_local_player = INTERFACES::Engine->GetLocalPlayer();

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!engine_local_player || !local_player || !iUser)
		return;

	auto event_weapon = game_event->GetString("weapon");

	if (!event_weapon)
		return;

	if (event_weapon == "weapon_unknown")
		return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!weapon)
		return;

	if (engine_local_player == iUser)
	{
		shots_fired[target]++;

		if (SETTINGS::settings.info_bool)
		{
			if (didMiss && GoodestWeapon(event_weapon))
			{
				if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_KNIFE
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_KNIFE_T
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_HEGRENADE
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_INC
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE)
					return;	

				static auto nospread = INTERFACES::cvar->FindVar("weapon_accuracy_nospread")->GetBool();
				Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

				if (!nospread)
				{
					if (rand() % 100 < 50)
					{
						INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 0, 255, 255), "[nicetap beta] ");
						GLOBAL::Msg("missed shot due to spread    \n");
					}
				}
			}

			didMiss = true;
		}
	}
}

void CGameEvents::PlayerHurtListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto victim = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid"));
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));
	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;

	if (INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")) == INTERFACES::Engine->GetLocalPlayer() && INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")) != INTERFACES::Engine->GetLocalPlayer())
	{
		if (SETTINGS::settings.lag_comp_hitbox)
			visuals->lag_comp_hitbox(entity, INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	}


	if (entity_attacker == local_player)
	{
		didMiss = false;

		visuals->set_hitmarker_time(INTERFACES::Globals->curtime);
		switch (SETTINGS::settings.hit_sound)
		{
		case 0: break;
		case 1: INTERFACES::Surface->IPlaySound("buttons\\arena_switch_press_02.wav"); break;
		case 2: PlaySoundA(hitsound_wav, NULL, SND_ASYNC | SND_MEMORY); break;	
		case 3: PlaySoundA(argent_fingers_wav, NULL, SND_ASYNC | SND_MEMORY); break;
		case 4: PlaySoundA(bame_wav, NULL, SND_ASYNC | SND_MEMORY); break;
		case 5: PlaySoundA(bubble_wav, NULL, SND_ASYNC | SND_MEMORY); break;
		case 6: PlaySoundA(miau__online_audio_converter_com__wav, NULL, SND_ASYNC | SND_MEMORY); break;
		}

		if (SETTINGS::settings.info_bool)
		{
			auto pVictim = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(victim));

			SDK::player_info_t pinfo;
			INTERFACES::Engine->GetPlayerInfo(pVictim->GetIndex(), &pinfo);

			auto hitbox = game_event->GetInt("hitgroup");
			if (!hitbox)
				return;

			auto damage = game_event->GetInt("dmg_health");
			if (!damage)
				return;

			auto health = game_event->GetInt("health");
			if (!health && health != 0)
				return;

			auto hitgroup = HitgroupToName(hitbox);

			INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 0, 255, 255), "[nicetap beta] ");

			GLOBAL::Msg("hurt %s in the %s for %d damage.    \n", pinfo.name, hitgroup, damage, pinfo.name, health);
		}
		shots_hit[entity->GetIndex()]++;
	}
}

void CGameEvents::BulletImpactListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	int iUser = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid"));

	auto entity = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(iUser));

	if (!entity)
		return;

	if (entity->GetIsDormant())
		return;

	float x, y, z;
	x = game_event->GetFloat("x");
	y = game_event->GetFloat("y");
	z = game_event->GetFloat("z");

	UTILS::BulletImpact_t impact(entity, Vector(x, y, z), INTERFACES::Globals->curtime, iUser == INTERFACES::Engine->GetLocalPlayer() ? GREEN : RED);

	visuals->Impacts.push_back(impact);
}

void CGameEvents::ItemPurchaseListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;

	auto event_weapon = game_event->GetString("weapon");

	if (event_weapon == "weapon_unknown")
		return;

	if (!event_weapon)
		return;

	if (SETTINGS::settings.info_bool)
	{
		INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 0, 255, 255), "[nicetap beta] ");
		GLOBAL::Msg("%s bought %s    \n", player_info.name, event_weapon);
	}
};

bool CGameEvents::GoodestWeapon(const char* weapon)
{
	if (weapon == "WEAPON_ZEUS")
		return false;

	if (weapon == "weapon_molotov")
		return false;

	if (weapon == "weapon_decoy")
		return false;

	if (weapon == "weapon_flashbang")
		return false;

	if (weapon == "weapon_hegrenade")
		return false;

	if (weapon == "weapon_smokegrenade")
		return false;

	if (weapon == "weapon_incgrenade")
		return false;

	if (weapon == "weapon_knife")
		return false;

	if (weapon == "weapon_unknown")
		return false;

	return true;
}

bool CGameEvents::GoodWeapon(const char* weapon)
{
	if (weapon == "taser")
		return false;

	if (weapon == "molotov")
		return false;

	if (weapon == "decoy")
		return false;

	if (weapon == "flashbang")
		return false;

	if (weapon == "hegrenade")
		return false;

	if (weapon == "smokegrenade")
		return false;

	if (weapon == "incgrenade")
		return false;

	if (weapon == "knife")
		return false;

	if (weapon == "unknown")
		return false;

	return true;
}

void CGameEvents::PlayerDeathListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));
	if (!attacker)
		return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;

	auto event_weapon = game_event->GetString("weapon");

	if (!event_weapon)
		return;

	if (event_weapon == "weapon_unknown")
		return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!weapon)
		return;
}