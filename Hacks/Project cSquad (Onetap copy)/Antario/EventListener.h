#pragma once
#include <vector>
#include "SDK\IGameEvent.h"
#include "Settings.h"
#include "SDK/ConVar.h"
#include "Features/ESP.h"
#include "Globals.h"
#include "Features\BulletListener.h"
#include "Features/sounds.h"
class EventListener : public IGameEventListener2
{
public:
	EventListener(std::vector<const char*> events)
	{
		for (auto& it : events)
			g_pEventManager->AddListener(this, it, false);
	}

	~EventListener()
	{
		g_pEventManager->RemoveListener(this);
	}

	using msg_t = void(__cdecl*)(const char*, ...);
	msg_t Msg = reinterpret_cast<msg_t>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));

	char* HitgroupToName(int hitgroup)
	{
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			return "head";
		case HITGROUP_LEFTLEG:
			return "left toe";
		case HITGROUP_RIGHTLEG:
			return "right toe";
		case HITGROUP_STOMACH:
			return "stomach";
		case HITGROUP_CHEST:
			return "chest";
		case HITGROUP_LEFTARM:
			return "left arm";
		case HITGROUP_RIGHTARM:
			return "right arm";
		default:
			return "body";
		}
	}

	void LagCompHitbox(C_BaseEntity * entity, int index)
	{
		float duration = 4;
		if (index < 0)
			return;
		if (!entity)
			return;
		studiohdr_t* pStudioModel = g_pModelInfo->GetStudiomodel(entity->GetModel());
		if (!pStudioModel)
			return;
		mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);
		if (!pHitboxSet)
			return;
		for (int i = 0; i < pHitboxSet->numhitboxes; i++)
		{
			mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);
			if (!pHitbox)
				continue;
			auto bone_matrix = entity->GetBoneMatrix(pHitbox->bone);
			Vector vMin, vMax;
			Utils::VectorTransform(pHitbox->bbmin, bone_matrix, vMin);
			Utils::VectorTransform(pHitbox->bbmax, bone_matrix, vMax);
			if (pHitbox->radius > -1)
			{
				g_pDebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->radius, 0, 255, 0, 255, duration);
			}
		}
	}


	void playerhurt(IGameEvent * game_event)
	{
		if (!game_event)
			return;

		if (!(g_pEngine->IsConnected() && g_pEngine->IsInGame()))
			return;

		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;

		auto victim = g_pEngine->GetPlayerForUserID(game_event->GetInt("userid"));
		auto entity = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("userid")));
		if (!entity)
			return;

		auto entity_attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("attacker")));
		if (!entity_attacker)
			return;

		if (entity->GetTeam() == local_player->GetTeam())
			return;

		PlayerInfo_t player_info;
		if (!g_pEngine->GetPlayerInfo(entity->EntIndex(), &player_info))
			return;



		if (entity_attacker == local_player)
		{
			Globals::shots_hit++;
			Globals::hit[entity->EntIndex()] = true;
			//didMiss = false;
			LagCompHitbox(entity, victim);

			g_ESP.set_hitmarker_time(g_pGlobalVars->realtime);

			dddd->sounds();

			auto pVictim = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(victim));

			PlayerInfo_t pinfo;
			g_pEngine->GetPlayerInfo(pVictim->EntIndex(), &pinfo);

			int hitbox = game_event->GetInt("hitgroup");

			Globals::damage = game_event->GetInt("dmg_health");
			if (!Globals::damage)
				return;

			auto hitgroup = HitgroupToName(hitbox);

			int health = game_event->GetInt("health");
			if (!health && health != 0)
				return;

			if (g_Settings.bDamageLogs)
			{
				g_pConVar->ConsoleColorPrintf(Color2(0, 255, 0, 255), "[Sesamhook] ");
				Msg("hit %s in the %s for %d (%d remaining)     \n", pinfo.szName, hitgroup, Globals::damage, health);
			}

			if (g_Settings.bKillSay)
			{
				if (health == 0)
					g_pEngine->ExecuteClientCmd("say owned f12");
			}
		}
	}

	void ItemPurchaseListener(IGameEvent * game_event)
	{
		if (!game_event)
			return;

		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;

		auto entity = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("userid")));
		if (!entity)
			return;

		if (entity->GetTeam() == local_player->GetTeam())
			return;

		PlayerInfo_t player_info;
		if (!g_pEngine->GetPlayerInfo(entity->EntIndex(), &player_info))
			return;

		auto event_weapon = game_event->GetString("weapon");

		if (event_weapon == "weapon_unknown")
			return;

		if (!event_weapon)
			return;

		g_pConVar->ConsoleColorPrintf(Color2(0, 255, 0, 255), "[Sesamhook] ");
		Msg("%s bought %s     \n", player_info.szName, event_weapon);
	};

	void FireGameEvent(IGameEvent * game_event) override
	{
		//static ConVar* sv_showimpacts = g_pConVar->FindVar("sv_showimpacts");

		int eventtimer = 0;

		auto event_name = game_event->GetName();

		if (!strcmp(event_name, "round_prestart"))
		{
			Globals::dontdoaa = true;
			Globals::preservedelete = true;
		}
		else if (!strcmp(event_name, "round_freeze_end"))
		{
			Globals::dontdoaa = false;
		}

		if (!strcmp(event_name, "item_purchase"))
			ItemPurchaseListener(game_event);

		if (!strcmp(event_name, "bullet_impact"))
			bullettracers::get().events(game_event);

		if (!strcmp(event_name, "player_hurt"))
			playerhurt(game_event);

		//if (!strcmp(game_event->GetName(), "bullet_impact"))
		//{
		//	auto shooter = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("attacker")));
		//	if (shooter && shooter == g::pLocalEntity)
		//	{
		//		if (eventtimer == 0)
		//			eventtimer = 2;
		//		else if (eventtimer == 2)
		//			eventtimer = 3;
		//	}
		//}
		//else
		//{
		//	if (eventtimer == 2 || eventtimer == 3)
		//	{
		//		if (!strcmp(game_event->GetName(), "player_hurt"))
		//		{

		//		}
		//		else
		//		{
		//			if (Globals::fired)
		//			{
		//				g_pConVar->ConsoleColorPrintf(Color2(0, 255, 0, 255), "[Sesamhook] ");
		//				Msg("Missed shot due to spread     \n");

		//				Globals::missedshots++;
		//				Globals::pPlayer = nullptr;
		//				//Globals::fired = false;
		//			}
		//		}
		//	}
		//	eventtimer = 0;
		//}

	}

	int GetEventDebugID() override
	{
		return EVENT_DEBUG_ID_INIT;
	}
};

extern EventListener g_pHit;
