#pragma once
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CGlowObjectManager.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/GlobalDefenitions.h"

typedef bool( __thiscall* fnDoPostScreenSpaceEffects )( void*, void* );
fnDoPostScreenSpaceEffects oDoPostScreenSpaceEffects;

namespace Cheat
{
	namespace Hooked
	{
		bool __stdcall DoPostScreenSpaceEffectsHook( void* setup )
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.DoPostScreenSpaceEffects = 0;
				return oDoPostScreenSpaceEffects(Interfaces::ClientMode, setup);
			}
			Cheat::Settings->UnloadReadyTable.DoPostScreenSpaceEffects = (int)GetTickCount();

			for (auto hk : c_lua::get()->hooks->getHooks("on_do_post_screen_space_effects"))
				hk.func();
			return oDoPostScreenSpaceEffects( Interfaces::ClientMode, setup );
		}
	}
}




