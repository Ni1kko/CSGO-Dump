#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"

#include "../../../Frontend/Menu/cmenu.h"

typedef void( __thiscall* fnLockCursor )( void* pThis );
fnLockCursor oLockCursor = nullptr;

namespace Cheat
{
	namespace Hooked
	{
		void __stdcall LockCursorHook()
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.LockCursor = 0;
				oLockCursor(Interfaces::Surface);
				return;
			}
			Cheat::Settings->UnloadReadyTable.LockCursor = (int)GetTickCount();

			if ( c_menu::get()->is_menu_initialized() && c_menu::get()->is_menu_opened() )
			{
				Interfaces::Surface->UnlockCursor();
				return;
			}

			oLockCursor( Interfaces::Surface );
		}
	}
}