#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Configuration/Config.h"

#include "../../../Frontend/Rendering/Render.h"
#include "../../../Frontend/Menu/cmenu.h"

#include <intrin.h>

//typedef long( __stdcall* fnPresent )( IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA* );
//fnPresent oPresent = nullptr;
typedef HRESULT(__stdcall* fnPresent2)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*); 
fnPresent2 oPresent;

namespace Cheat
{
	namespace Hooked
	{
		static HRESULT __stdcall present2(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion) noexcept
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.present2 = 0;
				return oPresent(device, src, dest, windowOverride, dirtyRegion);
			}
			Cheat::Settings->UnloadReadyTable.present2 = (int)GetTickCount();

			IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;

			device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
			device->GetVertexDeclaration(&vertDec);
			device->GetVertexShader(&vertShader);

			Graphics::Render->Initialize(device);
			Graphics::Render->Begin();
			Graphics::Render->Reset();

			static auto wanted_ret_address = _ReturnAddress();
			if (_ReturnAddress() == wanted_ret_address)
			{

				if (!Settings->UIDevMode)
				{
					if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
					{
						Interfaces::Engine->GetScreenSize(G::iScreenWidth, G::iScreenHeight);

						if (c_config::get()->i["vis_inaccuracyoverlay"] != 0)
						{
							int w, h = 0;
							Interfaces::Engine->GetScreenSize(w, h);

							if (G::LocalPlayer && G::LocalPlayer->IsAlive())
							{
								if (G::LocalPlayer->GetActiveWeapon()) {

									auto cone = (G::VisualLastInaccuracy + G::VisualLastCone) * 600;
									Graphics::Render->Line(0, 0, 0, 0, 0);

									if (cone > 0.f && !G::VisualLastKnife && !G::VisualLastNade)
									{
										if (c_config::get()->i["vis_inaccuracyoverlay"] == 1) {
											auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_inaccuracyoverlay_color"]);

											Graphics::Render->GradientCircle(w / 2, h / 2, cone, 0.f, 1, 100, D3DCOLOR_RGBA(col.r(), col.g(), col.b(), col.a()), D3DCOLOR_RGBA(col.r(), col.g(), col.b(), 0));

										}
										else
											Graphics::Render->RainbowCircle(w / 2, h / 2, cone, 0.f, CircleType::FULL, 50);
									}
								}
							}
						}
					}
				}

				DWORD colorwrite, srgbwrite;
				device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
				device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
				device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
				device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

				static bool once { false };
				if (!once) {
					c_menu::get()->initialize(device);
					once = true;
				}

				c_menu::get()->draw_begin();

				int w, h = 0;
				Interfaces::Engine->GetScreenSize(w, h);

				ui::SetNextWindowPos(ImVec2(0, 0));
				ui::SetNextWindowSize(ImVec2(w, h));
				ui::Begin("##visuals", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

				ui::End();

				c_menu::get()->draw();
				c_menu::get()->draw_end();

				device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
				device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
			}
			pixel_state->Apply();
			pixel_state->Release();

			device->SetVertexDeclaration(vertDec);
			device->SetVertexShader(vertShader);

			return oPresent(device, src, dest, windowOverride, dirtyRegion);
		}
	}





		//long __stdcall PresentHook( IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion )
		//{
		//	IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;

		//	pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
		//	pDevice->GetVertexDeclaration(&vertDec);
		//	pDevice->GetVertexShader(&vertShader);

		//	Graphics::Render->Initialize( pDevice );
		//	Graphics::Render->Begin();
		//	Graphics::Render->Reset();

		//	static auto wanted_ret_address = _ReturnAddress();
		//	if (_ReturnAddress() == wanted_ret_address)
		//	{

		//		if (!Settings->UIDevMode)
		//		{
		//			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		//			{
		//				Interfaces::Engine->GetScreenSize(G::iScreenWidth, G::iScreenHeight);

		//				if (c_config::get()->i["vis_inaccuracyoverlay"] != 0)
		//				{
		//					int w, h = 0;
		//					Interfaces::Engine->GetScreenSize(w, h);

		//					if (G::LocalPlayer && G::LocalPlayer->IsAlive())
		//					{
		//						if (G::LocalPlayer->GetActiveWeapon()) {

		//							auto cone = (G::VisualLastInaccuracy + G::VisualLastCone) * 600;
		//							Graphics::Render->Line(0, 0, 0, 0, 0);

		//							if (cone > 0.f && !G::VisualLastKnife && !G::VisualLastNade)
		//							{
		//								if (c_config::get()->i["vis_inaccuracyoverlay"] == 1) {
		//									auto col = c_config::get()->imcolor_to_ccolor(c_config::get()->c["vis_inaccuracyoverlay_color"]);

		//									Graphics::Render->GradientCircle(w / 2, h / 2, cone, 0.f, 1, 100, D3DCOLOR_RGBA(col.r(), col.g(), col.b(), col.a()), D3DCOLOR_RGBA(col.r(), col.g(), col.b(), 0));

		//								}
		//								else
		//									Graphics::Render->RainbowCircle(w / 2, h / 2, cone, 0.f, CircleType::FULL, 50);
		//							}
		//						}
		//					}
		//				}
		//			}
		//		}

		//		DWORD colorwrite, srgbwrite;
		//		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		//		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
		//		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		//		static bool once{ false };
		//		if (!once) {
		//			c_menu::get()->initialize(pDevice);

		//			c_config::get()->load_defaults();
		//			c_config::get()->load_keys();
		//			once = true;
		//		}

		//		c_menu::get()->draw_begin();

		//		int w, h = 0;
		//		Interfaces::Engine->GetScreenSize(w, h);

		//		ui::SetNextWindowPos(ImVec2(0, 0));
		//		ui::SetNextWindowSize(ImVec2(w, h));
		//		ui::Begin("##visuals", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

		//		ui::End();

		//		c_menu::get()->draw();
		//		c_menu::get()->draw_end();

		//		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		//		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
		//	}
		//	pixel_state->Apply();
		//	pixel_state->Release();
		//	
		//	pDevice->SetVertexDeclaration(vertDec);
		//	pDevice->SetVertexShader(vertShader);

		//	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		//}
	
}