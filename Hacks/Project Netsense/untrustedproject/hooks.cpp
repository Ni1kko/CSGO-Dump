#include "hooks.hpp"
#include "Back.h"
#include <intrin.h>  
#include "features/features.hpp"
#include "render.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "globals.hpp"
#include "addons/listener/event_listener.hpp"
#include "features/misc/NoVisualRecoil.hpp"
#include "features/misc/EdgeJump.hpp"
//#include "features/misc/movement_recorder.h"
#include "features/misc/NoSmoke.hpp"
#include "features/misc/Nightmode.hpp"
#include "features/misc/LegitAA.hpp"
#include "features/visuals/glow.hpp"
#include "features/visuals/chams.hpp"
#include "features/visuals/visuals.hpp"
#include "addons/notifications/notifications.hpp"
#include "addons/keybinds/keybinds.hpp"

#include "minhook.h"
#pragma comment(lib, "minhook.lib")

#pragma intrinsic(_ReturnAddress)  


#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )	


void* SetupBones_addr;
PVOID oSetupBones;




/*void c_visuals::Fog()
{

	int fogr, fogg, fogb, foga;
	fogr = g_Visuals->other.FogCol[0] * 255;
	fogg = g_Visuals->other.FogCol[1] * 255;
	fogb = g_Visuals->other.FogCol[2] * 255;
	foga = g_Visuals->other.FogCol[3] * 255;

	static const auto fog_enable = g_CVar->FindVar("fog_enable");
	fog_enable->SetValue(g_Visuals->other.fog_enable); //Включает туман на карте если он выключен по дефолту
	static const auto fog_override = g_CVar->FindVar("fog_override");
	fog_override->SetValue(g_Visuals->other.fog_override); // Разрешает кастомизацию тумана
	static const auto fog_color = g_CVar->FindVar("fog_color");
	fog_color->SetValue(std::string(std::to_string(fogr) + " " + std::to_string(fogg) + " " + std::to_string(fogb)).c_str()); //Цвет тумана rgb
	static const auto fog_start = g_CVar->FindVar("fog_start");
	fog_start->SetValue(g_Visuals->other.fog_start); // Дистанция с которой туман появляется
	static const auto fog_end = g_CVar->FindVar("fog_end");
	fog_end->SetValue(g_Visuals->other.fog_end); // Дистанция с которой туман пропадает
	static const auto fog_destiny = g_CVar->FindVar("fog_maxdensity");
	fog_destiny->SetValue(g_Visuals->other.fog_maxdensity); //Максимальная насыщенность тумана(0-1)
}*/





/*void c_visuals::SkyBox()
{
	const char* skyboxes[]{ "cs_baggage_skybox_", "cs_tibet", "embassy", "italy", "jungle", "nukeblank", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night_flat", "sky_csgo_night02", "sky_day02_05_hdr", "sky_day02_05", "sky_dust", "sky_l4d_rural02_ldr", "sky_venice", "vertigo_hdr", "vertigo", "vertigoblue_hdr", "vietnam" };

	if (g_Visuals->other.skybox != -1)
		Utils::LoadNamedSkys(skyboxes[g_Visuals->other.skybox]);


}*/


/*void c_visuals::WorldCol()
{
	ConVar* StaticProp;
	StaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
	StaticProp->SetValue(false);*/

	/*int worldr, worldg, worldb, worlda;
	worldr = g_Visuals->other.world_clr[0] * 255;
	worldg = g_Visuals->other.world_clr[1] * 255;
	worldb = g_Visuals->other.world_clr[2] * 255;
	worlda = g_Visuals->other.world_clr[3] * 255;

	int skyr, skyg, skyb, skya;
	skyr = g_Visuals->other.sky_clr[0] * 255;
	skyg = g_Visuals->other.sky_clr[1] * 255;
	skyb = g_Visuals->other.sky_clr[2] * 255;
	skya = g_Visuals->other.sky_clr[3] * 255;*/


	/*if (!g_Visuals->other.WorldEnable && !g_Visuals->other.SkyEnable)
		return;

	for (auto h = g_MatSystem->FirstMaterial(); h != g_MatSystem->InvalidMaterial(); h = g_MatSystem->NextMaterial(h)) {
		const auto mat = g_MatSystem->GetMaterial(h);
		
		if (!mat)
			continue;
		//if (!mat || !mat->Precached())
			//continue;
		/*if (mat->IsErrorMaterial())
			continue;*/
		/*const std::string_view textureGroup = mat->GetTextureGroupName();

		if (g_Visuals->other.WorldEnable && (strstr(mat->GetTextureGroupName(), "StaticProp"))) {

				mat->ColorModulate(g_Visuals->other.world_clr.r(), g_Visuals->other.world_clr.g(), g_Visuals->other.world_clr.b());
		}
		else if (g_Visuals->other.SkyEnable && (strstr(mat->GetTextureGroupName(), "World"))) {
			
				mat->ColorModulate(g_Visuals->other.sky_clr.r(), g_Visuals->other.sky_clr.g(), g_Visuals->other.sky_clr.b());
		}
	}
}*/



/*void c_misc::Night() {
	static bool OldNight;
	if (!g_EngineClient->IsConnected() || !g_EngineClient->IsInGame() || !g_LocalPlayer || !g_LocalPlayer->IsAlive())
	{
		OldNight = false;
		return;
	}
	ConVar* staticdrop;
	ConVar* r_3dsky;
	if (OldNight != g_Misc->options.NightMode)
	{

		staticdrop = g_CVar->FindVar("r_DrawSpecificStaticProp");
		r_3dsky = g_CVar->FindVar("r_3dsky");
		staticdrop->SetValue(0);



		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial* pMat = g_MatSystem->GetMaterial(i);
			if (!pMat || pMat->IsErrorMaterial())
				continue;

			if (pMat->GetReferenceCount() <= 0) {
				continue;
			}

			if (strstr(pMat->GetTextureGroupName(), "World") || strstr(pMat->GetTextureGroupName(), "StaticProp") || (strstr(pMat->GetTextureGroupName(), "Model")))
			{


				if (g_Misc->options.NightMode) {

					//Utils::LoadNamedSkys("sky_csgo_night02b");

					if (strstr(pMat->GetTextureGroupName(), "StaticProp"))
					{
						bool bFound = false;
						IMaterialVar* pMatVar = pMat->FindVar("$envmaptint", &bFound);
						if (bFound)
							(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pMatVar + 0x2c))((uintptr_t)pMatVar, 0.2f, 0.2f, 0.2f);

						pMat->ColorModulate(0.2f, 0.2f, 0.2f);
					}
					else if (strstr(pMat->GetTextureGroupName(), "Model"))
						pMat->ColorModulate(0.6f, 0.6f, 0.6f);
					else
						pMat->ColorModulate(0.2f, 0.2f, 0.2f);

					r_3dsky->SetValue(0);
				}
				else {
					//Utils::LoadNamedSkys(" sky_csgo_night02b");
					pMat->ColorModulate(1.0f, 1.0f, 1.0f);
					r_3dsky->SetValue(1);
				}
			}
		}
		OldNight = g_Misc->options.NightMode;
	}

}*/








namespace Hooks {

	void Initialize()
	{
		ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
		ConVar* weapon_debug_spread_show_con = g_CVar->FindVar("weapon_debug_spread_show");

		engine_hook.setup(g_EngineClient);
		hlclient_hook.setup(g_CHLClient);
		direct3d_hook.setup(g_D3DDevice9);
		vguipanel_hook.setup(g_VGuiPanel);
		//sendnetmsg_hook.setup(g_CHLClient);
		vguisurf_hook.setup(g_VGuiSurface);
		sound_hook.setup(g_EngineSound);
		mdlrender_hook.setup(g_MdlRender);
		clientmode_hook.setup(g_ClientMode);
		bsp_query_hook.setup(g_EngineClient->GetBSPTreeQuery());
		sv_cheats.setup(sv_cheats_con);
		game_events.setup(g_GameEvents);
		weapon_debug_spread_show.setup(weapon_debug_spread_show_con);
		sequence_hook = new RecvPropHook(C_BaseViewModel::m_nSequence(), hkRecvProxy);

		g_CustomEventsManager.Init();

		direct3d_hook.hook_index(index::EndScene, hkEndScene);
		direct3d_hook.hook_index(index::Reset, hkReset);
		hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
		clientmode_hook.hook_index(index::ClientModeCreateMove, hkCreateMove);
		vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);
		//sendnetmsg_hook.hook_index(index::SendNetMsg, hkSendNetMsg);////
		sound_hook.hook_index(index::EmitSound1, hkEmitSound1);
		vguisurf_hook.hook_index(index::LockCursor, hkLockCursor);
		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);
		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);
		sv_cheats.hook_index(index::SvCheatsGetBool, hkSvCheatsGetBool);
		weapon_debug_spread_show.hook_index(13, hkGetInt_weapon_debug_spread_show);
		bsp_query_hook.hook_index(index::ListLeavesInBox, hkListLeavesInBox);
		engine_hook.hook_index(index::FireEvents, hkFireEvents);

		// fakelags fix
		const auto cl_sendmove_patch = (void*)(Utils::PatternScan(GetModuleHandleA("engine.dll"), "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98") + 0xBD);
		DWORD old;
		VirtualProtect(cl_sendmove_patch, 4, PAGE_EXECUTE_READWRITE, &old);
		*(uint32_t*)cl_sendmove_patch = 62;
		VirtualProtect(cl_sendmove_patch, 4, old, &old);


		MH_Initialize();

		// setupbones hook(currently needed only for slowwalk animation)
		SetupBones_addr = (void*)Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B 0D");
		MH_CreateHook(SetupBones_addr, hkSetupBones, &oSetupBones);
		MH_EnableHook(SetupBones_addr);
	}
	//--------------------------------------------------------------------------------
	void Shutdown()
	{
		hlclient_hook.unhook_all();
		direct3d_hook.unhook_all();
		vguipanel_hook.unhook_all();
		//sendnetmsg_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		clientmode_hook.unhook_all();
		sound_hook.unhook_all();
		sv_cheats.unhook_all();
		g_CustomEventsManager.Unload();

		MH_DisableHook(SetupBones_addr);
		MH_Uninitialize();

		g_Glow->Shutdown();
	}
	//--------------------------------------------------------------------------------

	long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
	{
		static auto oEndScene = direct3d_hook.get_original<decltype(&hkEndScene)>(index::EndScene);
		int w, h;
		g_EngineClient->GetScreenSize(w, h);



		if (!g_Unload) {
			static auto viewmodel_fov = g_CVar->FindVar("viewmodel_fov");
			//static auto mat_ambient_light_r = g_CVar->FindVar("mat_ambient_light_r");
			//static auto mat_ambient_light_g = g_CVar->FindVar("mat_ambient_light_g");
			//static auto mat_ambient_light_b = g_CVar->FindVar("mat_ambient_light_b");
			static auto crosshair_cvar = g_CVar->FindVar("crosshair");
			crosshair_cvar->SetValue(true);

			viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
			//viewmodel_fov->SetValue(g_Options.viewmodel_fov);
			/*mat_ambient_light_r->SetValue(g_Options.mat_ambient_light_r);
			mat_ambient_light_g->SetValue(g_Options.mat_ambient_light_g);
			mat_ambient_light_b->SetValue(g_Options.mat_ambient_light_b);

			crosshair_cvar->SetValue(!(g_Options.esp_enabled && g_Options.esp_crosshair));*/

			IDirect3DVertexDeclaration9* vertDec;
			IDirect3DVertexShader9* vertShader;
			IDirect3DStateBlock9* stateBlock = nullptr;
			pDevice->GetVertexDeclaration(&vertDec);
			pDevice->GetVertexShader(&vertShader);
			pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &stateBlock);

			pDevice->SetVertexDeclaration(nullptr);
			pDevice->SetVertexShader(nullptr);

			DWORD colorwrite, srgbwrite, colorvrtx;
			pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
			pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
			pDevice->GetRenderState(D3DRS_COLORVERTEX, &colorvrtx);

			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
			pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false); // removes the source engine color correction

			pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
			pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			/*if (tImage == nullptr)
				D3DXCreateTextureFromFileInMemoryEx(pDevice, &logo, sizeof(logo), 45, 45, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tImage);
				*/
			auto esp_drawlist = Render::Get().RenderScene();

			Menu::Get().Render();
			g_Misc->end_scene();
			//g_Misc->Night();
			//g_Visuals->WorldCol();
			g_Notification.Draw();
			g_KeyBinds->ExecuteKeyBinds();


			ImGui::Render(esp_drawlist);

			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

			stateBlock->Apply();
			stateBlock->Release();
			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
			pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
			pDevice->SetRenderState(D3DRS_COLORVERTEX, colorvrtx);
			pDevice->SetVertexDeclaration(vertDec);
			pDevice->SetVertexShader(vertShader);






		}

		return oEndScene(pDevice);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto oReset = direct3d_hook.get_original<decltype(&hkReset)>(index::Reset);

		Menu::Get().OnDeviceLost();

		auto hr = oReset(device, pPresentationParameters);

		if (hr >= 0)
			Menu::Get().OnDeviceReset();

		return hr;
	}
	//--------------------------------------------------------------------------------

	/*bool __fastcall hkSendNetMsg(INetChannel* thisptr, int edx, INetMessage* pMessage, bool bForceReliable, bool bVoice)
	{
		//static auto oSendNetMsg = SendNetMsg.GetOriginal<decltype(&hkSendNetMsg)>();
		static auto oSendNetMsg = sendnetmsg_hook.get_original<decltype(&hkSendNetMsg)>(index::SendNetMsg);

		
		if (pMessage->GetType() == 14)
			return false;

		
		if (pMessage->GetGroup() == INetChannelInfo::VOICE)
			bVoice = true;

		return oSendNetMsg(thisptr, edx, pMessage, bForceReliable, bVoice);
	}*/
	//--------------------------------------------------------------------------------
	bool __stdcall hkCreateMove(float InputSampleFrameTime, CUserCmd* cmd)
	{
		bool* bSendPacket = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);
		auto oCreateMove = clientmode_hook.get_original< CreateMoveClientMode >(index::ClientModeCreateMove);
		bool result = oCreateMove(g_ClientMode, InputSampleFrameTime, cmd);

		if (!cmd || !cmd->command_number || g_Unload)
			return result;

		if (Menu::Get().IsVisible())
			cmd->buttons &= ~IN_ATTACK;

		g_GlobalVars->ServerTime(cmd);

		//g_MovementRecorder.MovementP(cmd);
		//g_MovementRecorder.MovementR(cmd);

		//g_Visuals->SkyBox();



		g_Misc->create_move(cmd, bSendPacket);
		EdgeJump::PrePredictionCreateMove(cmd);

		//CPredictionSystem::Get().Process(g_LocalPlayer, cmd);
		{
			g_Aimbot.OnMove(cmd, bSendPacket);
			g_Backtrack->CMove(cmd);
			g_Misc->create_move_prediction(cmd, bSendPacket);

			/*static ConVar* m_yaw = m_yaw = g_CVar->FindVar("m_yaw");
			static ConVar* m_pitch = m_pitch = g_CVar->FindVar("m_pitch");
			static ConVar* sensitivity = sensitivity = g_CVar->FindVar("sensitivity");

			static QAngle m_angOldViewangles = g_ClientState->viewangles;

			float delta_x = std::remainderf(cmd->viewangles.pitch - m_angOldViewangles.pitch, 360.0f);
			float delta_y = std::remainderf(cmd->viewangles.yaw - m_angOldViewangles.yaw, 360.0f);

			if (delta_x != 0.0f) {
				float mouse_y = -((delta_x / m_pitch->GetFloat()) / sensitivity->GetFloat());
				short mousedy;
				if (mouse_y <= 32767.0f)
					if (mouse_y >= -32768.0f)
						if (mouse_y >= 1.0f || mouse_y < 0.0f) {
							if (mouse_y <= -1.0f || mouse_y > 0.0f)
								mousedy = static_cast<short>(mouse_y);
							else
								mousedy = -1;
						}
						else
							mousedy = 1;
					else
						mousedy = 0x8000u;
				else
					mousedy = 0x7FFF;

				cmd->mousedy = mousedy;
			}

			if (delta_y != 0.0f) {
				float mouse_x = -((delta_y / m_yaw->GetFloat()) / sensitivity->GetFloat());
				short mousedx;
				if (mouse_x <= 32767.0f) {
					if (mouse_x >= -32768.0f) {
						if (mouse_x >= 1.0f || mouse_x < 0.0f) {
							if (mouse_x <= -1.0f || mouse_x > 0.0f)
								mousedx = static_cast<short>(mouse_x);
							else
								mousedx = -1;
						}
						else
							mousedx = 1;
					}
					else
						mousedx = 0x8000u;
				}
				else
					mousedx = 0x7FFF;

				cmd->mousedx = mousedx;
			}*/
		}; //CPredictionSystem::Get().Restore(g_LocalPlayer, cmd);
		
		//CPredictionSystem::Get().Restore(g_LocalPlayer, cmd);
		//CPredictionSystem::Get().Process(g_LocalPlayer, cmd);
		//CPredictionSystem::Get().Update();
		//CPredictionSystem::Get().Predict(g_LocalPlayer, cmd);


		EdgeJump::PostPredictionCreateMove(cmd);


		return false;
	}
	
	//--------------------------------------------------------------------------------
	/*void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<decltype(&hkPaintTraverse)>(index::PaintTraverse);

		if (!panelId) {
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel")) {
				panelId = panel;
			}
		}
		else if (panelId == panel) {
			static bool bSkip{ false };
			if (!bSkip) {
				g_EngineClient->GetScreenSize(CheatGVars::SWidth, CheatGVars::SHeight);
				CheatGVars::SWidthHalf = CheatGVars::SWidth / 2;
				CheatGVars::SHeightHalf = CheatGVars::SHeight / 2;
				g_Config->ApplyRainbow();
				Render::Get().BeginScene();
			}
			bSkip = !bSkip;
		}

		oPaintTraverse(g_VGuiPanel, edx, panel, forceRepaint, allowForce);
	}*/

	void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<decltype(&hkPaintTraverse)>(index::PaintTraverse);

		if (!panelId) {
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel")) {
				panelId = panel;
			}
		}
		else if (panelId == panel) {
			static bool bSkip{ false };
			if (!bSkip) {
				g_EngineClient->GetScreenSize(CheatGVars::SWidth, CheatGVars::SHeight);
				CheatGVars::SWidthHalf = CheatGVars::SWidth / 2;
				CheatGVars::SHeightHalf = CheatGVars::SHeight / 2;
				g_Config->ApplyRainbow();
				Render::Get().BeginScene();
			}
			bSkip = !bSkip;
		}

		oPaintTraverse(g_VGuiPanel, edx, panel, forceRepaint, allowForce);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkEmitSound1(void* _this, int edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk) {
		static auto ofunc = sound_hook.get_original<decltype(&hkEmitSound1)>(index::EmitSound1);


		if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep")) {
			static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

			if (fnAccept) {

				fnAccept("");

				//This will flash the CSGO window on the taskbar
				//so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = InputSys::Get().GetMainWindow();
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}

		ofunc(g_EngineSound, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);

	}
	//--------------------------------------------------------------------------------
	int __fastcall hkDoPostScreenEffects(void* _this, int edx, int a1)
	{
		static auto oDoPostScreenEffects = clientmode_hook.get_original<decltype(&hkDoPostScreenEffects)>(index::DoPostScreenSpaceEffects);

		if (g_LocalPlayer && g_Visuals->glow.enabled)
			g_Glow->Run();

		return oDoPostScreenEffects(g_ClientMode, edx, a1);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t stage)
	{

		//CNightmode::Get().UpdateWorldTextures();
		static auto ofunc = hlclient_hook.get_original<decltype(&hkFrameStageNotify)>(index::FrameStageNotify);

		if (g_EngineClient->IsInGame()) {
			g_Misc->frame_stage(stage);

			if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
				g_SkinChanger->OnFrameStageNotify(false);
			else if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
				g_SkinChanger->OnFrameStageNotify(true);
			else if (stage == FRAME_NET_UPDATE_START)
				NoSmoke::OnFrameStageNotify();


			//c_visuals::Get().PlayerChanger(stage);
			//c_misc::Get().DrawTaserRange();
		}
		//if (g_EngineClient->IsInGame()) {

		//}

		ofunc(g_CHLClient, edx, stage);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkOverrideView(void* _this, int edx, CViewSetup* vsView)
	{
		static auto ofunc = clientmode_hook.get_original<decltype(&hkOverrideView)>(index::OverrideView);

		CheatGVars::OFOV = vsView->fov;
		NoVisualRecoil::OverrideView(vsView);

		if (g_LocalPlayer && g_LocalPlayer->IsAlive()) {
			g_Misc->override_view();
			if (g_Misc->options.view_fov_changer && !g_LocalPlayer->m_bIsScoped()) {
				vsView->fov = g_Misc->options.view_fov;
			}
			else if (g_Misc->options.no_zoom) {
				vsView->fov = g_Misc->options.view_fov_changer ? g_Misc->options.view_fov : 90.f;
			}
		}

		ofunc(g_ClientMode, edx, vsView);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = vguisurf_hook.get_original<decltype(&hkLockCursor)>(index::LockCursor);
		//g_Misc->DrawTaserRange();
		if (Menu::Get().IsVisible()) {
			g_VGuiSurface->UnlockCursor();
			g_InputSystem->ResetInputState();
			return;
		}
		ofunc(g_VGuiSurface);

	}
	//--------------------------------------------------------------------------------
	void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ofunc = mdlrender_hook.get_original<decltype(&hkDrawModelExecute)>(index::DrawModelExecute);

		if (g_MdlRender->IsForcedMaterialOverride() &&
			!strstr(pInfo.pModel->szName, "arms") &&
			!strstr(pInfo.pModel->szName, "weapons/v_")) {
			return ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
		}

		//Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
		g_LegitAA->DrawArrows();
		g_Chams->render(ctx, state, pInfo, pCustomBoneToWorld);

		ofunc(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);

		g_MdlRender->ForcedMaterialOverride(nullptr);
	}
	//--------------------------------------------------------------------------------
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
	{
		static auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");
		static auto ofunc = sv_cheats.get_original<bool(__thiscall*)(PVOID)>(13);
		if (!ofunc)
			return false;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
			return true;
		return ofunc(pConVar);
	}
	//--------------------------------------------------------------------------------
	//int __stdcall hkIsBoxVisible(const Vector& mins, const Vector& maxs) {
	//	static auto beam_cull_check = Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "85 C0 0F 84 ? ? ? ? 8B 0D ? ? ? ? 8D 54 24 6C");
	//	static auto oIsBoxVisible = iz engineclient;

	//	if (_ReturnAddress() == beam_cull_check) // ring beams
	//		return 1;

	//	return oisboxvisible(mins, maxs);
	//}
	//--------------------------------------------------------------------------------
	int __fastcall hkGetInt_weapon_debug_spread_show(void* ecx, void* edx) {
		typedef int(__thiscall* GetInFn)(void*);

		static auto dwRetAddr = Utils::PatternScan(GetModuleHandleA("client.dll"), "85 C0 0F 84 ? ? ? ? E8 ? ? ? ? 99");
		if (_ReturnAddress() != dwRetAddr || !g_Misc->options.show_crosshair || !g_LocalPlayer->m_hActiveWeapon())
			return weapon_debug_spread_show.get_original<GetInFn>(13)(ecx);

		//short weapon = g_LocalPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex();
		if (/*weapon != WEAPON_AWP && weapon != WEAPON_SSG08 && weapon != WEAPON_G3SG1 && weapon != WEAPON_SCAR20 && */(g_LocalPlayer->m_bIsScoped()))
			return weapon_debug_spread_show.get_original<GetInFn>(13)(ecx);

		return 2;
	}
	//--------------------------------------------------------------------------------
	bool __fastcall hkSetupBones(uintptr_t ecx, uintptr_t edx, matrix3x4_t* bone_to_world_out, int max_bones, int bone_bask, float current_time) {
		static auto original = reinterpret_cast<bool(__thiscall*)(uintptr_t ecx, matrix3x4_t * bone_to_world_out, int max_bones, int bone_mask, float current_time)>(oSetupBones);
		auto e = reinterpret_cast<C_BasePlayer*>(ecx - 4); // hardcoded aswell

		if (!e || e->GetClientClass()->m_ClassID != ClassId_CCSPlayer)
			return original(ecx, bone_to_world_out, max_bones, bone_bask, current_time);

		bool ret;

		if (e == g_LocalPlayer && CheatGVars::SlowWalking) {
			auto backup = e->GetAnimOverlays()[6];
			e->GetAnimOverlays()[6].m_flWeight = 0.0f;
			ret = original(ecx, bone_to_world_out, max_bones, bone_bask, current_time);
			e->GetAnimOverlays()[6] = backup;
		}
		else {
			ret = original(ecx, bone_to_world_out, max_bones, bone_bask, current_time);
		}

		return ret;
	}
	//--------------------------------------------------------------------------------
	int __fastcall hkListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax) {
		typedef int(__thiscall* ListLeavesInBox)(void*, const Vector&, const Vector&, unsigned short*, int);
		static auto ofunc = bsp_query_hook.get_original< ListLeavesInBox >(index::ListLeavesInBox);

		// occulusion getting updated on player movement/angle change,
		// in RecomputeRenderableLeaves ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L674 );
		// check for return in CClientLeafSystem::InsertIntoTree
		if (*(uint32_t*)_ReturnAddress() != 0x14244489) // 89 44 24 14 ( 0x14244489 ) - new / 8B 7D 08 8B ( 0x8B087D8B ) - old
			return ofunc(bsp, mins, maxs, pList, listMax);

		// get current renderable info from stack ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470 )
		auto info = *(RenderableInfo_t**)((uintptr_t)_AddressOfReturnAddress() + 0x14);
		if (!info || !info->m_pRenderable)
			return ofunc(bsp, mins, maxs, pList, listMax);

		// check if disabling occulusion for players ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1491 )
		auto base_entity = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
		if (!base_entity || !base_entity->IsPlayer())
			return ofunc(bsp, mins, maxs, pList, listMax);

		// fix render order, force translucent group ( https://www.unknowncheats.me/forum/2429206-post15.html )
		// AddRenderablesToRenderLists: https://i.imgur.com/hcg0NB5.png ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L2473 )
		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;

		// extend world space bounds to maximum ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707 )
		static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
		static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
		auto count = ofunc(bsp, map_min, map_max, pList, listMax);
		return count;
	}
	//--------------------------------------------------------------------------------
	bool _fastcall hkFireEvents(void* ecx, void* edx) {
		static auto original = engine_hook.get_original<decltype(&hkFireEvents)>(index::FireEvents);

		if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || !g_EngineClient->IsInGame())
			return original(ecx, edx);


		auto cur_event = *reinterpret_cast<CEventInfo**>(reinterpret_cast<uintptr_t>(g_ClientState) + 0x4E6C);// 0x4E64
		if (!cur_event) return original(ecx, edx);

		CEventInfo* next = nullptr;
		do {
			next = *(CEventInfo**)((uintptr_t)cur_event + 0x38);
			cur_event->fire_delay = 0.f;

			cur_event = next;
		} while (next);

		return original(ecx, edx);
	}


	//--------------------------------------------------------------------------------
	void hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output)
	{
		static auto oRecv = sequence_hook->GetOriginalFunction();

		if (g_LocalPlayer && g_LocalPlayer->IsAlive()) {
			const auto proxy_data = const_cast<CRecvProxyData*>(pData);
			const auto view_model = static_cast<C_BaseViewModel*>(entity);

			if (view_model && view_model->m_hOwner() && view_model->m_hOwner().IsValid()) {
				const auto owner = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(view_model->m_hOwner()));
				if (owner == g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer())) {
					const auto view_model_weapon_handle = view_model->m_hWeapon();
					if (view_model_weapon_handle.IsValid()) {
						const auto view_model_weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(view_model_weapon_handle));
						if (view_model_weapon) {
							if (values::WeaponInfo.count(view_model_weapon->m_Item().m_iItemDefinitionIndex())) {
								auto original_sequence = proxy_data->m_Value.m_Int;
								const auto override_model = values::WeaponInfo.at(view_model_weapon->m_Item().m_iItemDefinitionIndex()).model;
								proxy_data->m_Value.m_Int = g_KnifeAnimFix->Fix(override_model, proxy_data->m_Value.m_Int);
							}
						}
					}
				}
			}

		}

		oRecv(pData, entity, output);
	}
}
