#pragma once

#include "../../SDK/IClientMode.h"
#include "../../SDK/CUserCmd.h"
//#include "../../Features/Skinchanger/skinchanger.h"

typedef void(__thiscall* fnReadPacketEntities)(void*, void*);
fnReadPacketEntities oReadPacketEntities = nullptr;

namespace Cheat
{
	namespace Hooked
	{

		void __fastcall ReadPacketEntitiesHook(void* pClientState, void* pEDX, void* pEntInfo)
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.ReadPacketEntities = 0;
				oReadPacketEntities(pClientState, pEntInfo);
				return;
			}
			Cheat::Settings->UnloadReadyTable.ReadPacketEntities = (int)GetTickCount();

			oReadPacketEntities(pClientState, pEntInfo);
			// Skins::OnFrameStageNotify(false);
		}
	}
}