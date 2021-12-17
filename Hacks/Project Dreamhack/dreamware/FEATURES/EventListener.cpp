#include "../MISC/includes.h"

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
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/Aimbot.h"

#include "EventListener.h"
#include <playsoundapi.h>

#pragma comment(lib, "Winmm.lib")

CGameEvents::ItemPurchaseListener item_purchase_listener;
CGameEvents::PlayerHurtListener player_hurt_listener;
CGameEvents::BulletImpactListener bullet_impact_listener;
CGameEvents::AntiAimDisableListener antiaim_disable_listener;
CGameEvents::RoundStartListener round_start_listener;
CGameEvents::WeaponFiredListener weapon_fired_listener;
HitmarkerStuff* hitmarkerstuff;

unsigned int EpochMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
void CGameEvents::InitializeEventListeners()
{
	INTERFACES::GameEventManager->AddListener(&item_purchase_listener, "item_purchase", false);
	INTERFACES::GameEventManager->AddListener(&player_hurt_listener, "player_hurt", false);
	INTERFACES::GameEventManager->AddListener(&bullet_impact_listener, "bullet_impact", false);
	INTERFACES::GameEventManager->AddListener(&antiaim_disable_listener, "round_prestart", false);
	INTERFACES::GameEventManager->AddListener(&antiaim_disable_listener, "round_end", false);
	INTERFACES::GameEventManager->AddListener(&antiaim_disable_listener, "round_freeze_end", false);
	INTERFACES::GameEventManager->AddListener(&round_start_listener, ("round_start"), false);
	INTERFACES::GameEventManager->AddListener(&weapon_fired_listener, "weapon_fire", false);

}
void CGameEvents::RemoveEventListeners()
{
	INTERFACES::GameEventManager->RemoveListener(&item_purchase_listener);
	INTERFACES::GameEventManager->RemoveListener(&player_hurt_listener);
	INTERFACES::GameEventManager->RemoveListener(&bullet_impact_listener);
	INTERFACES::GameEventManager->RemoveListener(&antiaim_disable_listener);
	INTERFACES::GameEventManager->RemoveListener(&round_start_listener);
	INTERFACES::GameEventManager->RemoveListener(&weapon_fired_listener);

}


typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void ConMsg(const char* msg, ...) {

	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything

	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.

	if (!INTERFACES::cvar->FindVar("developer")->GetInt())
		INTERFACES::cvar->FindVar("developer")->SetValue(true);

	if (!INTERFACES::cvar->FindVar("con_filter_enable")->GetInt())
		INTERFACES::cvar->FindVar("con_filter_enable")->SetValue(2);
}

float randnum(int Min, int Max)
{
	return ((rand() % (Max - Min)) + Min);
}

bool CGameEvents::GoodestWeapon(const char* weapon)
{
	if (weapon == "weapon_taser")
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

		if (SETTINGS::settings.Logdamage)
		{
			if (didMiss && GoodestWeapon(event_weapon))
			{
				if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_KNIFE_CT
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_KNIFE_T
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_HEGRENADE
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_INCGRENADE
					|| weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE)
					return;

				static auto nospread = INTERFACES::cvar->FindVar("weapon_accuracy_nospread")->GetBool();
				Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

				if (!nospread)
				{
					if (rand() % 100 < 50)
					{
						INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[DreamHack] ");
						GLOBAL::Msg("Missed shot due to spread    \n");
					}
				}
			}

			didMiss = true;
		}
	}
}
void CGameEvents::AntiAimDisableListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event) return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	if (!strcmp(game_event->GetName(), "round_freeze_end"))
		GLOBAL::DisableAA = false;

	if (!strcmp(game_event->GetName(), "round_prestart") || !strcmp(game_event->GetName(), "round_end"))
         GLOBAL::DisableAA = true;
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
	case HITGROUP_LEFTARM:
		return "left arm";
	case HITGROUP_RIGHTARM:
		return "right arm";
	default:
		return "body";
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

	auto victim = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")); if (!victim) return;
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));
	if (!entity_attacker)
		return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;


	SDK::CBaseEntity* hurt = (SDK::CBaseEntity*) INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (SETTINGS::settings.dmg_bool)
	{
		if (strcmp(game_event->GetName(), "player_hurt") == 0) {

			if (hurt != local_player && entity_attacker == local_player) {
				CVisuals::damage_indicator_t DmgIndicator;
				DmgIndicator.dmg = game_event->GetInt("dmg_health");
				DmgIndicator.player = hurt;
				DmgIndicator.earse_time = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick + 3.f;
				DmgIndicator.initializes = false;

				visuals->dmg_indicator.push_back(DmgIndicator);
			}
		}
	}
	if (INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")) == INTERFACES::Engine->GetLocalPlayer() && INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")) != INTERFACES::Engine->GetLocalPlayer())
	{
		if (SETTINGS::settings.lagcompensatedhitboxes_bool)
			visuals->LagCompHitbox(entity, INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	}

	if (entity_attacker == local_player)
	{
		didMiss = false;

		visuals->set_hitmarker_time( INTERFACES::Globals->curtime );
		if (SETTINGS::settings.hitmarker_val)
		{
		  INTERFACES::Surface->IPlaySound("buttons\\arena_switch_press_02.wav");  // skeet.cc not overrated
		}

		if (SETTINGS::settings.Logdamage)
		{
			auto pVictim = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(victim));
			if (!pVictim) return;
			SDK::player_info_t pinfo;
			INTERFACES::Engine->GetPlayerInfo(pVictim->GetIndex(), &pinfo);

			auto hitbox = game_event->GetInt("hitgroup");
			if (!hitbox) return;

			auto damage = game_event->GetInt("dmg_health");
			if (!damage) return;

			auto health = game_event->GetInt("health");
			if (!health && health != 0) return;

			auto hitgroup = HitgroupToName(hitbox);

			


			INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[DreamHack] ");
			GLOBAL::Msg("Hurt player %s in the %s for %d damage (%d health remaining)    \n", pinfo.name, hitgroup, damage, health);
		}
		shots_hit[entity->GetIndex()]++;
		hurtcalled = true;
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
	if (!entity) return;

	if (entity->GetIsDormant()) return;

	float x, y, z;
	x = game_event->GetFloat("x");
	y = game_event->GetFloat("y");
	z = game_event->GetFloat("z");
	
	UTILS::BulletImpact_t impact(entity, Vector(x, y, z), INTERFACES::Globals->curtime, iUser == INTERFACES::Engine->GetLocalPlayer() ? GREEN : RED);

	visuals->Impacts.push_back(impact);
}

void  CGameEvents::RoundStartListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	if (!SETTINGS::settings.buybot_enabled)
		return;

	switch (SETTINGS::settings.buybot_rifle)
	{
	case 1:
		INTERFACES::Engine->ClientCmd("buy scar20;buy g3sg1");
		break;
	case 2:
		INTERFACES::Engine->ClientCmd("buy ssg08");
		break;
	case 3:
		INTERFACES::Engine->ClientCmd("buy awp");
		break;
	default:
		break;
	}

	switch (SETTINGS::settings.buybot_pistol)
	{
	case 1:
		INTERFACES::Engine->ClientCmd("buy deagle");
		break;
	case 2:
		INTERFACES::Engine->ClientCmd("buy elite");
		break;
	case 3:
		INTERFACES::Engine->ClientCmd("buy p250");
		break;
	default:
		break;
	}

	switch (SETTINGS::settings.buybot_armor)
	{
	case 1:
		INTERFACES::Engine->ClientCmd("buy vest");
		break;
	case 2:
		INTERFACES::Engine->ClientCmd("buy vesthelm");
		break;
	default:
		break;
	}

	if (SETTINGS::settings.buybot_grenade)
		INTERFACES::Engine->ClientCmd("buy hegrenade; buy molotov; buy incgrenade; buy smokegrenade;");



	if (SETTINGS::settings.buybot_zeus)
		INTERFACES::Engine->ClientCmd("buy taser 34");
}


void CGameEvents::ItemPurchaseListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)return;
	if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)return;

	auto entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)return;

	if (entity->GetTeam() == local_player->GetTeam())return;

	SDK::player_info_t player_info;
	if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))return;

	auto event_weapon = game_event->GetString("weapon");

	if (event_weapon = "weapon_unknown")return;
	if (!event_weapon)return;

	if (SETTINGS::settings.logweaponpurchase)
	{

		INTERFACES::cvar->ConsoleColorPrintf(CColor(200, 255, 0, 255), "[DreamHack] ");
		GLOBAL::Msg("Player %s bought %s    \n", player_info.name, event_weapon);

	}
};