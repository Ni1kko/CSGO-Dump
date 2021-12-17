#pragma once
#include "../../GUI/Gui.h"
#include "../../GUI/Menu.h"
#include "../DXRender.h"
#include "../../Hacks/Setup.h"
#include "../TestTexture.h"
#include "../Fonts/Arial.h"
#include "../Fonts/Tahoma.h"
#include "../Fonts/WeaponIcons.h"

BOOL CreateRender = FALSE;

COverManager VisualsOverlay;
COverManager TestOverlay;
CMenu Menu;

CFont* VisualsFont = nullptr;
CFont* MenuFont = nullptr;


LPDIRECT3DTEXTURE9 UpdateImage(void *pArr, UINT szArr, float w, float h)
{
	LPDIRECT3DTEXTURE9 entry;
	D3DXCreateTextureFromFileInMemoryEx(GP_Render->m_pDevice, pArr, szArr, w, h,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &entry);
	return entry;
}


HRESULT STDMETHODCALLTYPE myPresent(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{

#ifndef ONLY_DRAW_HOOK
	CGlobal::IsGameReady = I::Engine()->IsInGame();
#else
	CGlobal::IsGameReady = false;
#endif 

#ifdef ENABLE_CLEAR_CONSOLE
	system("cls");
#endif 


	if (pDevice)
	{
		if (!CreateRender)
		{
			ADD_LOG("2-1-11-7-3-7-0\n");
			g_pDevice = pDevice;
			GP_Render->m_pDevice = pDevice;
			ADD_LOG("2-1-11-7-3-7-1\n");
			VisualsFont = new CFont();
			MenuFont = new CFont();
			ADD_LOG("2-1-11-7-3-7-2\n");
			CFont::FontConfig font_config;
			font_config.OversampleH = 3;
			font_config.OversampleV = 3;
			font_config.PixelSnapH = 1;
			ADD_LOG("2-1-11-7-3-7-3\n");
			static const Wchar ranges[] =
			{
				0x0020, 0x00FF,
				0x0400, 0x044F,
				0,
			};
			ADD_LOG("2-1-11-7-3-7-4\n");
			VisualsFont = VisualsFont->CreateFontFromCompressMemory(Arial_compressed_data, Arial_compressed_size, 16.f, &font_config, ranges);
			MenuFont = MenuFont->CreateFontFromCompressMemory(Tahoma_compressed_data, Tahoma_compressed_size, 16.f, &font_config, ranges);
			ADD_LOG("2-1-11-7-3-7-5\n");
			GP_Render->UpdateSzFonts();
			ADD_LOG("2-1-11-7-3-7-6\n");
			CreateFontsTexture();
			ADD_LOG("2-1-11-7-3-7-7\n");
			X1Gui().Start();
			ADD_LOG("2-1-11-7-3-7-8\n");
	//		img = GP_Render->AddImage(&TestTexturePNG, sizeof(TestTexturePNG), 100, 80);

			Message::Get().Start("INJECTED!");
			ADD_LOG("2-1-11-7-3-7-9\n");
			CreateRender = true;
		}
		else
		{
#ifndef ONLY_DRAW_HOOK
			I::Engine()->GetScreenSize(CGlobal::iScreenWidth, CGlobal::iScreenHeight);
#endif

			CGSettings::G().UpdateColors();

			GP_Render->BeginDraw();

			
			VisualsOverlay.ResetCount();
			GP_Render->SetOverlay(&VisualsOverlay);
			{
				GP_Render->SetFont(VisualsFont);
				if (CGlobal::IsGameReady)
				{
					if (GP_Esp)
						GP_Esp->Draw();

					if (GP_Misc)
						GP_Misc->Draw();

					if (GP_GHelper)
						if (GP_GHelper->Enable)
							GP_GHelper->Draw();

					if (GP_LegitAim)
						GP_LegitAim->Draw();

					if (GP_Esp->GranadePrediction)
						grenade_prediction::Get().Paint();
				}
				else
				{
					if (GP_Esp)
						GP_Esp->Reset();

					if (GP_Misc)
					{
						GP_Misc->Reset();
						GP_Misc->AutoAcceptEmit();
					}
				}

				if (MainSettings().Watermark)
				{
					GP_Render->DrawString(5, 3, 17, Color::Red(), false, false, "X");
					GP_Render->DrawString(16, 3, 17, Color::White(), false, false, "ONE");
				} 

				Message::Get().Show();
			}
			GP_Render->ReleaseOverlay();
			
			X1Gui().Update();

			if (GP_Radar)
				GP_Radar->Draw();

			if (GP_Misc->SpectatorList && !CGlobal::FullUpdateCheck)
				GP_Misc->ShowSpectatorList();

			if(!GP_Enable3DMenu->Check() || !GP_Enable3DMenu->Enable)
				Menu.Draw(MenuFont);

			if (GP_Enable3DMenu->Enable && CGlobal::IsGameReady)
			{
				if (GP_3DMenu->Begin("", GP_Enable3DMenu->Check(), Vector(35, 1, 65), Vector(-34, -7, 0), false))
				{
					if (X1Gui().GetIO().ActivWindowName.find("##3D") == string::npos)
						X1Gui().GetIO().ActivWindowName = "";

					bool OldIsGuiVisble = CGlobal::IsGuiVisble;
					CGlobal::IsGuiVisble = true;
					X1Gui().G_Enable3D = true;

					X1Gui().SetNextWindowPos(Vec2(0,0));

					X1Gui().GetIO().MousePos = Vec2(GP_3DMenu->mouse_pos_2d.x * 10,
						GP_3DMenu->mouse_pos_2d.y * 10);

					Menu.Draw(MenuFont);

					X1Gui().G_Enable3D = false;

					CGlobal::IsGuiVisble = OldIsGuiVisble;
					GP_3DMenu->End();
				}

			}
			else
				X1Gui().G_Enable3D = false;

			

			X1Gui().Cleanup();
			////TestOverlay.ResetCount();
			////GP_Render->SetOverlay(&TestOverlay);
			////{
			////	for (int ov_i = 0; ov_i < GP_Render->DrawLists.size(); ov_i++)
			////	{
			////		for (int i = 0; i < GP_Render->DrawLists[ov_i]->CmdBuffer.size(); i++)
			////		{
			////			Vec4 rect_vec = GP_Render->DrawLists[ov_i]->CmdBuffer[i].ClipRect;
			////			Rect rect = Rect(Vec2(rect_vec.x, rect_vec.y),Vec2(rect_vec.z, rect_vec.w));
			////			GP_Render->DrawBox(rect.min, rect.max,Color(255,255,0,100));
			////		}
			////	}


			////	//GP_Render->DrawImage(g_FontTexture, Vec2(0, 0), Vec2(512, 2048));
			////}
			////GP_Render->ReleaseOverlay();

			GP_Render->EndDraw();

		}
	}

	return pPresent->GetTrampoline()(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
