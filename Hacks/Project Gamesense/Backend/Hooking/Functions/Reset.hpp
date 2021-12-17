#pragma once
#include "../Hooks/Hooks.h"
#include "../../Include/GlobalIncludes.h"
#include "../../../Frontend/Rendering/Render.h"

#include "../../../Frontend/Menu/cmenu.h"

//typedef long( __stdcall* fnReset )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
//fnReset oReset = nullptr;
typedef HRESULT(__stdcall* fnReset2)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*); 
fnReset2 oReset;

namespace Cheat
{
	namespace Hooked
	{
/*		long __stdcall ResetHook( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParams )
		{
			if ( !Graphics::Render->IsInitialized() || !c_menu::get()->is_menu_initialized())
				return oReset( pDevice, pParams );

			Graphics::Render->ReleaseObjects();
			ImGui_ImplDX9_InvalidateDeviceObjects();
			auto result = oReset( pDevice, pParams );
			ImGui_ImplDX9_CreateDeviceObjects();
			Graphics::Render->CreateObjects();

			*/
	
		static HRESULT D3DAPI reset2(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.reset2 = 0;
				return oReset(device, params);
			}
			Cheat::Settings->UnloadReadyTable.reset2 = (int)GetTickCount();

			if (!c_menu::get()->is_menu_initialized())
				return oReset(device, params);

			Graphics::Render->ReleaseObjects();
			ImGui_ImplDX9_InvalidateDeviceObjects();
			auto result = oReset(device, params);
			ImGui_ImplDX9_CreateDeviceObjects();
			Graphics::Render->CreateObjects();

			return result;
		}
	}
}
