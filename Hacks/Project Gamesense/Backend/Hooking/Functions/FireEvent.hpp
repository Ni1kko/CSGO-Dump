#pragma once

#include "../../SDK/IClientMode.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/handle.h"
#include "../../Features/Skinchanger/skinchanger2.h"
#include "../../SDK/RecvData.h"

typedef bool(__thiscall* fnFireEvent)(IGameEventManager*, IGameEvent* pEvent);
fnFireEvent oFireEvent = nullptr;

namespace Cheat
{
	namespace Hooked
	{
		bool __stdcall FireEventHook(IGameEvent* pEvent) {
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.FireEvent = 0;
				return oFireEvent(Interfaces::GameEvents, pEvent);
			}
			Cheat::Settings->UnloadReadyTable.FireEvent = (int)GetTickCount();

			if (!strcmp(pEvent->GetName(), "player_death") && Interfaces::Engine->GetPlayerForUserID(pEvent->GetInt("attacker")) == G::LocalPlayer->GetIndex()) {
				auto key = pEvent->GetString("weapon");
				if (SkinChanger::iconOverrides.count(key))
				{
					const auto icon_override = SkinChanger::iconOverrides[key];
					if (icon_override)
						pEvent->SetWString("weapon", (wchar_t*)icon_override);
				}
			}
			return oFireEvent(Interfaces::GameEvents, pEvent);
		}
	}
}