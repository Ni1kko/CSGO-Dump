#include "Client.h"

//[enc_string_enable /]
//[junk_enable /]

IDirect3DTexture9 *fontdraw = nullptr;

ImFont*  bigFont = nullptr;
ImFont*  forTabs = nullptr;

namespace Client
{
	//[swap_lines]
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	string BaseDir = "";
	string LogFile = "";
	string GuiFile = "";
	string IniFile = "";
	vector<string> ConfigList;
	
	static int iConfigSelect = 0;
	static char ConfigName[64] = { 0 };
	Vector2D	g_vCenterScreen = Vector2D(0.f, 0.f);
	CPlayers*	g_pPlayers = nullptr;
	CRender*	g_pRender = nullptr;
	CGui*		g_pGui = nullptr;

	CAimbot*	g_pAimbot = nullptr;
	CEsp*		g_pEsp = nullptr;
	CRadar*		g_pRadar = nullptr;
	CSkin*		g_pSkin = nullptr;
	CMisc*		g_pMisc = nullptr;

	bool		bC4Timer = false;
	int			iC4Timer = 40;

	int			iWeaponID = 0;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = -1;
	//[/swap_lines]



	const char* gloves_listbox_items[49] =
	{
		"Default",
		"Sport Gloves | Superconductor",
		"Sport Gloves | Pandora's Box",
		"Sport Gloves | Hedge Maze",
		"Sport Gloves | Arid",
		"Sport Gloves | Vice",
		"Sport Gloves | Omega",
		"Sport Gloves | Bronze Morph",
		"Sport Gloves | Amphibious",
		"Moto Gloves | Eclipse",
		"Moto Gloves | Spearmint",
		"Moto Gloves | Boom!",
		"Moto Gloves | Cool Mint",
		"Moto Gloves | Polygon",
		"Moto Gloves | Transport",
		"Moto Gloves | Turtle",
		"Moto Gloves | Pow",
		"Specialist Gloves | Crimson Kimono",
		"Specialist Gloves | Emerald Web",
		"Specialist Gloves | Foundation",
		"Specialist Gloves | Forest DDPAT",
		"Specialist Gloves | Mogul",
		"Specialist Gloves | Fade",
		"Specialist Gloves | Buckshot",
		"Specialist Gloves | Crimson Web",
		"Driver Gloves | Lunar Weave",
		"Driver Gloves | Convoy",
		"Driver Gloves | Crimson Weave",
		"Driver Gloves | Diamondback",
		"Driver Gloves | Racing Green",
		"Driver Gloves | Overtake",
		"Driver Gloves | Imperial Plad",
		"Driver Gloves | King Snake",
		"Hand Wraps | Leather",
		"Hand Wraps | Spruce DDPAT",
		"Hand Wraps | Badlands",
		"Hand Wraps | Slaughter",
		"Hand Wraps | Aboreal",
		"Hand Wraps | Duct Tape",
		"Hand Wraps | Overprint",
		"Hand Wraps | Cobalt Skulls",
		"Bloodhound Gloves | Charred",
		"Bloodhound Gloves | Snakebite",
		"Bloodhound Gloves | Bronzed",
		"Bloodhound Gloves | Guerrilla",
		"Hydra Gloves | Case Hardened",
		"Hydra Gloves | Rattler",
		"Hydra Gloves | Mangrove",
		"Hydra Gloves | Emerald",
	};

	const char* weapons[34] =
	{
		"deagle",
		"elite",
		"fiveseven",
		"glock18",
		"p2000",
		"p250",
		"usp_s",
		"cz75",
		"revolver",
		"tec9",
		"ak47",
		"aug",
		"famas",
		"galilar",
		"m249",
		"m4a4",
		"m4a1_s",
		"mac10",
		"p90",
		"mp5-sd",
		"ump45",
		"xm1014",
		"bizon",
		"mag7",
		"negev",
		"sawedoff",
		"mp7",
		"mp9",
		"nova",
		"sg553",
		"scar20",
		"g3sg1",
		"awp",
		"ssg08"
	};

	const char* iHitSound[] =
	{
		"Off",
		"Default",
		"Anime",
		"Roblox",
		"GameSense",
		"German",
		"HeadMeme",
		"FadeCSGO.tk"
	};

	const char* skybox_items[] =
	{
		"cs_baggage_skybox_",
		"cs_tibet",
		"embassy",
		"italy",
		"jungle",
		"nukeblank",
		"office",
		"sky_cs15_daylight01_hdr",
		"sky_cs15_daylight02_hdr",
		"sky_cs15_daylight03_hdr",
		"sky_cs15_daylight04_hdr",
		"sky_csgo_cloudy01",
		"sky_csgo_night02",
		"sky_csgo_night02b",
		"sky_csgo_night_flat",
		"sky_day02_05_hdr",
		"sky_day02_05",
		"sky_dust",
		"sky_l4d_rural02_ldr",
		"sky_venice",
		"vertigo_hdr",
		"vertigoblue_hdr",
		"vertigo",
		"vietnam",
		"greenscreen", //start of custom
		"amethyst",
		"extreme_glaciation_",
		"persistent_fog_",
		"polluted_atm_",
		"toxic_atm_",
		"water_sunset_",
		"blue1",
		"blue2",
		"blue3",
		"blue4",
		"blue5",
		"blue6",
		"dark1",
		"dark2",
		"dark3",
		"dark4",
		"dark5",
		"green1",
		"green2",
		"green3",
		"green4",
		"green5",
		"night1",
		"night2",
		"night3",
		"night4",
		"night5",
		"orange1",
		"orange2",
		"orange3",
		"orange4",
		"orange5",
		"orange6",
		"pink1",
		"pink2",
		"pink3",
		"pink4",
		"pink5",
	};


	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.ini"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = "C:\\Aimware.cc\\*.ini";
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}

	bool Initialize(IDirect3DDevice9* pDevice)
	{

		g_pPlayers = new CPlayers();
		g_pRender = new CRender(pDevice);
		g_pGui = new CGui();
		g_pAimbot = new CAimbot();
		g_pEsp = new CEsp();
		g_pRadar = new CRadar();
		g_pSkin = new CSkin();
		g_pMisc = new CMisc();

		CreateDirectoryW(L"C:\\Aimware.cc", NULL);
		GuiFile = "C:\\Windows\\gui.ini";
		IniFile = "C:\\Aimware.cc\\settings.ini";

		g_pSkin->InitalizeSkins();

		Settings::LoadSettings(IniFile);

		iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		g_pGui->GUI_Init(pDevice);

		RefreshConfigs();

		return true;
	}

	void Shutdown()
	{
		DELETE_MOD(g_pPlayers);
		DELETE_MOD(g_pRender);
		DELETE_MOD(g_pGui);

		DELETE_MOD(g_pAimbot);
		DELETE_MOD(g_pEsp);
		DELETE_MOD(g_pRadar);
		DELETE_MOD(g_pSkin);
		DELETE_MOD(g_pMisc);
	}

	int get_fps()
	{
		return 0;
	}

	void OnRender()
	{
		if (g_pRender && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			g_pRender->BeginRender();

			if (g_pGui)
				g_pGui->GUI_Draw_Elements();

			
			Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);
			

			g_vCenterScreen.x = iScreenWidth / 2.f;
			g_vCenterScreen.y = iScreenHeight / 2.f;

			float watermarkRainbowSpeed = 0.005f;
			float watermarkScrollSpeed = 2.5f;

			EventLog->DrawLogs();

			

			

			if (Client::g_pPlayers && Client::g_pPlayers->GetLocal() && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			{
				if (g_pEsp)
					g_pEsp->OnRender();

				if (g_pMisc)
				{
					g_pMisc->OnRender();
					g_pMisc->OnRenderSpectatorList();
				}
			}

			if (Settings::Misc::misc_Spectators && Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
			{
				g_pRender->Text(15, 500, false, true, Color::White(), "Spectators List:");
				g_pRender->Text(15, 501, false, true, Color::White(), "______________");
			}

			g_pRender->EndRender();
		}
	}

	void OnLostDevice()
	{
		if (g_pRender)
			g_pRender->OnLostDevice();

		if (g_pGui)
			ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void OnResetDevice()
	{
		if (g_pRender)
			g_pRender->OnResetDevice();

		if (g_pGui)
			ImGui_ImplDX9_CreateDeviceObjects();
	}


	void OnCreateMove(CUserCmd* pCmd)
	{
		if (g_pPlayers && Interfaces::Engine()->IsInGame())
		{
			g_pPlayers->Update();

			if (g_pEsp)
				g_pEsp->OnCreateMove(pCmd);

			if (IsLocalAlive())
			{
				if (!bIsGuiVisible)
				{
					int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

					if (iWeaponSettingsSelectID >= 0)
						iWeaponID = iWeaponSettingsSelectID;
				}

				if (g_pAimbot)
					g_pAimbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pMisc)
					g_pMisc->OnCreateMove(pCmd);


				backtracking->legitBackTrack(pCmd);
			}
		}
	}

	void OnFireEventClientSideThink(IGameEvent* pEvent)
	{
		if (!strcmp(pEvent->GetName(), "player_connect_full") ||
			!strcmp(pEvent->GetName(), "round_start") ||
			!strcmp(pEvent->GetName(), "cs_game_disconnected"))
		{
			if (g_pPlayers)
				g_pPlayers->Clear();

			if (g_pEsp)
				g_pEsp->OnReset();

			
		}

		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			hitmarker::singleton()->initialize();

			if (g_pEsp)
				g_pEsp->OnEvents(pEvent);

			if (g_pAimbot)
				g_pAimbot->KillDelay(pEvent);

			if (g_pSkin)
				g_pSkin->OnEvents(pEvent);

			if (g_pMisc)
				g_pMisc->KillSpam(pEvent);
		}
	}

	void Emit_Sound(const char* sound_entry)
	{
		if (g_pMisc)
			g_pMisc->Emit_Sound(sound_entry);
	}

	void OnFrameStageNotify(ClientFrameStage_t Stage)
	{
		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			Skin_OnFrameStageNotify(Stage);
			Gloves_OnFrameStageNotify(Stage);
		}
	}

	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		if (Interfaces::Engine()->IsInGame() && ctx && pCustomBoneToWorld)
		{
			if (g_pEsp)
				g_pEsp->OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			if (g_pMisc)
				g_pMisc->OnDrawModelExecute();
		}
	}

	/*	void OnPlaySound(const char* pszSoundName)
	{
	if (g_pMisc)
	g_pMisc->OnPlaySound(pszSoundName);
	}

	/*	void OnOverrideView(CViewSetup* pSetup)
	{
	if (g_pMisc)
	g_pMisc->OnOverrideView(pSetup);
	}

	void OnGetViewModelFOV(float& fov)
	{
	if (g_pMisc)
	g_pMisc->OnGetViewModelFOV(fov);
	}
	*/
	void ImDrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
	{
		ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

		Color colColor(0, 0, 0, 255);

		flRainbow += flSpeed;
		if (flRainbow > 1.f) flRainbow = 0.f;//1 0 

		for (int i = 0; i < width; i = i + 1)
		{
			float hue = (1.f / (float)width) * i;
			hue -= flRainbow;
			if (hue < 0.f) hue += 1.f;

			Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
			windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
		}
	}

	void OnRenderGUI()
	{

		static int TabSelect = false;

	auto& style = ImGui::GetStyle();
	if(bIsGuiVisible){

		
		if (style.Alpha > 1.f)
		style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
		style.Alpha += 0.01f;
		style.WindowMinSize = ImVec2(0, 0);
		ImGui::PushFont(Global::fDefault);
		int w;
		int h;
		int W, H;

		float SpaceLineOne = 120.f;
		float SpaceLineTwo = 240.f;
		float SpaceLineThr = 360.f;
		float SpaceLineFor = 480.f;
		float SpaceLineFiv = 120.f;
		float SpaceLineFev = 240.f;
		float SpaceLineEsp = 200.f;

		style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
		ImVec2 mainWindowPos; // wir brauchen die main window pos um nen vernünftigen border zu drawen
	
		
		Interfaces::Engine()->GetScreenSize(W, H);

		////Erstmal drawen wir das main menu
		//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Appearing);
		//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1, 0.1f, 1.f);

		//ImGui::Begin("##bg", &bIsGuiVisible, ImVec2(W, H), .5f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
		//{

		//}ImGui::End();



		style.WindowPadding = ImVec2(1, 1);
		style.Colors[ImGuiCol_Button] = ImVec4(0.54, 0., 0.54, 1.f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.54, 0., 0.54, 1.f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.54, 0., 0.54, 1.f);
		ImGui::SetNextWindowSize(ImVec2(800, 20));
		ImGui::Begin("WWW.Aimware.cc", &bIsGuiVisible, ImVec2(800, 0), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		{
			ImVec4 col_text = ImColor::HSV(140 / 255.f, 20.f / 255.f, 235.f / 255.f);
			ImVec4 col_main = ImColor(9, 82, 128);
			ImVec4 col_back = ImColor(31, 44, 54);
			ImVec4 col_area = ImColor(4, 32, 41);
			//ImVec4(1.0f, 0.63f, 0.5f, 1.00f);
			auto mColor = Settings::Misc::misc_themecolor;
			float themecolor[3];
			themecolor[0] = float(mColor[0]);
			themecolor[1] = float(mColor[1]);
			themecolor[2] = float(mColor[2]);
			

			style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.95f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09f, .09f, .09f, 1.f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13, 0.13, 0.13, 1.f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.11, 0.11, 0.11, 1.f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);;
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
			style.Colors[ImGuiCol_ComboBg] = ImVec4(0.15, 0.15, 0.15, 1.00f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, 1.f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, 1.f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, 1.f);
			style.Colors[ImGuiCol_Header] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.50f, 0.50f, 0.50f, .3f); // combobox hover
			style.Colors[ImGuiCol_ComboSelectedText] = ImVec4(.52f, 0.f, 0.52f, .87f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .87f);
			style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
			style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.f);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
			style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

			



			
			mainWindowPos = ImGui::GetWindowPos();
			//ImGui::Button("Astrium.cc", ImVec2(798, 8));
		}ImGui::End();

		// Als Border machen wir ein 3-lagigen: farbe-grau
		style.WindowPadding = ImVec2(8, 8);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 6, mainWindowPos.y + 80));
		BtnNormal();
		//style.WindowPadding = ImVec2(0, 0);
		ImGui::Begin("Hurensohn Visual Studio", &bIsGuiVisible, ImVec2(788, 450), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
		{
			//g_pGui->ThemeColor();

			if (TabSelect == 0) //Aimbot
			{
				
				ImGui::BeginChild("Aimbott - General", ImVec2(771, 90), true);
				{
					
					ImGui::Text(" ");
					ImGui::Spacing();
					ImGui::Checkbox("Enable aimbot", &Settings::Aimbot::aim_Active);
					ImGui::Text(" ");
					ImGui::Checkbox("Auto-wall", &Settings::Aimbot::aim_WallAttack);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Activates aimbot if player takes damage through penetrable walls");
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Anti-Jump", &Settings::Aimbot::aim_AntiJump);
					ImGui::SameLine(SpaceLineTwo);
					ImGui::Checkbox("Check-Smoke", &Settings::Aimbot::aim_CheckSmoke);
					
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Does not activate aimbot if smoke is between you and the enemy");
					ImGui::SameLine(SpaceLineThr);
					ImGui::Checkbox("Check-Flash", &Settings::Aimbot::aim_CheckFlash);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Does not activate aimbot if you are flashed");
					ImGui::SameLine(SpaceLineFor);
					
					ImGui::Checkbox("Friendly Fire", &Settings::Aimbot::aim_Deathmatch);
				}ImGui::EndChild();
				style.ItemSpacing = ImVec2(0, 4);
				//ImGui::Text(" ");
				DrawAimbot(); // cogemos estilos y demas



				

			

				ImGui::BeginChild("Aimbot - Accuracy", ImVec2(385.5, -1), true);
				{
					/*ImGui::Text("Aimbot");
					ImGui::Separator();*/
					//ImGui::Checkbox("Activate aimbot", &Settings::Aimbot::aim_Active);

					//ImGui::Checkbox("On Key", &Settings::Aimbot::aim_OnKey);
					
					ImGui::Text("Aimbot");
					ImGui::Separator();
					ImGui::Spacing();
					//Pistolas
					if (Global::LegitSubTab == 0)
					{
						ImGui::Text("");
						//ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);
						if (iWeaponID <= 9)
						{
							ImGui::SameLine();
							ImGui::Checkbox("Autopistol", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol);
						}
						ImGui::PushItemWidth(280.f);
						ImGui::SliderInt("Fov", &Settings::Aimbot::aim_Fov[3], 0, 300);
						ImGui::SliderInt("Smooth", &Settings::Aimbot::aim_Smooth[3], 0, 300);
						//ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_Backtracktickrate, 1, 200);
						ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_backtrack[3], 1, 200);
						ImGui::PopItemWidth();
						ImGui::Text("");
						ImGui::PushItemWidth(280.f);
						const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
						ImGui::Combo("Best Hit", &Settings::Aimbot::aim_BestHit[3], BestHit, IM_ARRAYSIZE(BestHit));
						const char* AimFovType[] = { "Dynamic" , "Static" };
						ImGui::Combo("Fov Type", &Settings::Aimbot::aim_FovType[3], AimFovType, IM_ARRAYSIZE(AimFovType));
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Dynamic changes size of FOV depending on distance between you and enemy");
						ImGui::PopItemWidth();


						const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
						ImGui::PushItemWidth(280.f);
						ImGui::Combo("Aimspot", &Settings::Aimbot::aim_Spot[3], Aimspot, IM_ARRAYSIZE(Aimspot));
						ImGui::PopItemWidth();

						//ImGui::PushItemWidth(280.f);
						//ImGui::SliderInt("Shot Delay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
						//ImGui::PopItemWidth();
						ImGui::Text("");
					}
					//MSG
					else if (Global::LegitSubTab == 1)
					{
						ImGui::Text("");
						//ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);
						
						ImGui::PushItemWidth(280.f);
						ImGui::SliderInt("Fov", &Settings::Aimbot::aim_Fov[1], 0, 300);
						ImGui::SliderInt("Smooth", &Settings::Aimbot::aim_Smooth[1], 0, 300);
						//ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_Backtracktickrate, 1, 200);
						ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_backtrack[1], 1, 200);
						ImGui::PopItemWidth();
						ImGui::Text("");
						ImGui::PushItemWidth(280.f);
						const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
						ImGui::Combo("Best Hit", &Settings::Aimbot::aim_BestHit[1], BestHit, IM_ARRAYSIZE(BestHit));
						const char* AimFovType[] = { "Dynamic" , "Static" };
						ImGui::Combo("Fov Type", &Settings::Aimbot::aim_FovType[1], AimFovType, IM_ARRAYSIZE(AimFovType));
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Dynamic changes size of FOV depending on distance between you and enemy");
						ImGui::PopItemWidth();


						const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
						ImGui::PushItemWidth(280.f);
						ImGui::Combo("Aimspot", &Settings::Aimbot::aim_Spot[1], Aimspot, IM_ARRAYSIZE(Aimspot));
						ImGui::PopItemWidth();

						//ImGui::PushItemWidth(280.f);
						//ImGui::SliderInt("Shot Delay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
						//ImGui::PopItemWidth();
						ImGui::Text("");
					}
					//RIFLE
					else if (Global::LegitSubTab == 2)
					{
						ImGui::Text("");
						//ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);

						ImGui::PushItemWidth(280.f);
						ImGui::SliderInt("Fov", &Settings::Aimbot::aim_Fov[2], 0, 300);
						ImGui::SliderInt("Smooth", &Settings::Aimbot::aim_Smooth[2], 0, 300);
						//ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_Backtracktickrate, 1, 200);
						ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_backtrack[2], 1, 200);
						ImGui::PopItemWidth();
						ImGui::Text("");
						ImGui::PushItemWidth(280.f);
						const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
						ImGui::Combo("Best Hit", &Settings::Aimbot::aim_BestHit[2], BestHit, IM_ARRAYSIZE(BestHit));
						const char* AimFovType[] = { "Dynamic" , "Static" };
						ImGui::Combo("Fov Type", &Settings::Aimbot::aim_FovType[2], AimFovType, IM_ARRAYSIZE(AimFovType));
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Dynamic changes size of FOV depending on distance between you and enemy");
						ImGui::PopItemWidth();


						const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
						ImGui::PushItemWidth(280.f);
						ImGui::Combo("Aimspot", &Settings::Aimbot::aim_Spot[2], Aimspot, IM_ARRAYSIZE(Aimspot));
						ImGui::PopItemWidth();

						//ImGui::PushItemWidth(280.f);
						//ImGui::SliderInt("Shot Delay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
						//ImGui::PopItemWidth();
						ImGui::Text("");
					}
					//ESCOPETA
					else if (Global::LegitSubTab == 3)
					{
						ImGui::Text("");
						//ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);

						ImGui::PushItemWidth(280.f);
						ImGui::SliderInt("Fov", &Settings::Aimbot::aim_Fov[4], 0, 300);
						ImGui::SliderInt("Smooth", &Settings::Aimbot::aim_Smooth[4], 0, 300);
						//ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_Backtracktickrate, 1, 200);
						ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_backtrack[4], 1, 200);
						ImGui::PopItemWidth();
						ImGui::Text("");
						ImGui::PushItemWidth(280.f);
						const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
						ImGui::Combo("Best Hit", &Settings::Aimbot::aim_BestHit[4], BestHit, IM_ARRAYSIZE(BestHit));
						const char* AimFovType[] = { "Dynamic" , "Static" };
						ImGui::Combo("Fov Type", &Settings::Aimbot::aim_FovType[4], AimFovType, IM_ARRAYSIZE(AimFovType));
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Dynamic changes size of FOV depending on distance between you and enemy");
						ImGui::PopItemWidth();


						const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
						ImGui::PushItemWidth(280.f);
						ImGui::Combo("Aimspot", &Settings::Aimbot::aim_Spot[4], Aimspot, IM_ARRAYSIZE(Aimspot));
						ImGui::PopItemWidth();

						//ImGui::PushItemWidth(280.f);
						//ImGui::SliderInt("Shot Delay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
						//ImGui::PopItemWidth();
						ImGui::Text("");
						}
					//MACHINEGUN
					else if (Global::LegitSubTab == 4)
					{
						ImGui::Text("");
						//ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);

						ImGui::PushItemWidth(280.f);
						ImGui::SliderInt("Fov", &Settings::Aimbot::aim_Fov[0], 0, 300);
						ImGui::SliderInt("Smooth", &Settings::Aimbot::aim_Smooth[0], 0, 300);
						//ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_Backtracktickrate, 1, 200);
						ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_backtrack[0], 1, 200);
						ImGui::PopItemWidth();
						ImGui::Text("");
						ImGui::PushItemWidth(280.f);
						const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
						ImGui::Combo("Best Hit", &Settings::Aimbot::aim_BestHit[0], BestHit, IM_ARRAYSIZE(BestHit));
						const char* AimFovType[] = { "Dynamic" , "Static" };
						ImGui::Combo("Fov Type", &Settings::Aimbot::aim_FovType[0], AimFovType, IM_ARRAYSIZE(AimFovType));
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Dynamic changes size of FOV depending on distance between you and enemy");
						ImGui::PopItemWidth();


						const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
						ImGui::PushItemWidth(280.f);
						ImGui::Combo("Aimspot", &Settings::Aimbot::aim_Spot[0], Aimspot, IM_ARRAYSIZE(Aimspot));
						ImGui::PopItemWidth();

						//ImGui::PushItemWidth(280.f);
						//ImGui::SliderInt("Shot Delay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
						//ImGui::PopItemWidth();
						ImGui::Text("");
					}
					//SNIPER
					else if (Global::LegitSubTab == 5)
					{
						ImGui::Text("");
						//ImGui::Checkbox("Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);

						ImGui::PushItemWidth(280.f);
						ImGui::SliderInt("Fov", &Settings::Aimbot::aim_Fov[5], 0, 300);
						ImGui::SliderInt("Smooth", &Settings::Aimbot::aim_Smooth[5], 0, 300);
						//ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_Backtracktickrate, 1, 200);
						ImGui::SliderInt("Backtrack Time (ms)", &Settings::Aimbot::aim_backtrack[5], 1, 200);
						ImGui::PopItemWidth();
						ImGui::Text("");
						ImGui::PushItemWidth(280.f);
						const char* BestHit[] = { "Prefer Manually" , "Nearest" , "Ignore Head" , "Ignore Arms/Legs" , "Ignore Arms/Legs/Neck" };
						ImGui::Combo("Best Hit", &Settings::Aimbot::aim_BestHit[5], BestHit, IM_ARRAYSIZE(BestHit));
						const char* AimFovType[] = { "Dynamic" , "Static" };
						ImGui::Combo("Fov Type", &Settings::Aimbot::aim_FovType[5], AimFovType, IM_ARRAYSIZE(AimFovType));
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Dynamic changes size of FOV depending on distance between you and enemy");
						ImGui::PopItemWidth();


						const char* Aimspot[] = { "Head" , "Neck" ,  "Body" , "Thorax" , "Chest" , "Right Thigh" , "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" , "Left Upperarm" , "Left Forearm" };
						ImGui::PushItemWidth(280.f);
						ImGui::Combo("Aimspot", &Settings::Aimbot::aim_Spot[5], Aimspot, IM_ARRAYSIZE(Aimspot));
						ImGui::PopItemWidth();

						//ImGui::PushItemWidth(280.f);
						//ImGui::SliderInt("Shot Delay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
						//ImGui::PopItemWidth();
						ImGui::Text("");
					}
				

					/*if (Settings::Aimbot::aim_OnKey)
					{
						ImGui::SameLine();
						ImGui::Hotkey("Key", &Settings::Aimbot::aim_Key, ImVec2(100, 25));
					}*/
					//ImGui::PushItemWidth(-1);
					//ImGui::PushItemHeight(-1);
					//ImGui::ListBox("##Colorstab", &Settings::Esp::esp_colors_tab, colors_tab, ARRAYSIZE(colors_tab), 11);
					//ImGui::ListBox("##Weaponstab", &iWeaponID, weapons, IM_ARRAYSIZE(weapons), 21);
					//ImGui::PopItemWidth();
					//ImGui::SameLine();
					

					
					

					

				}ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("Aimbot23 - Accuracy", ImVec2(385.5, -1), true);
				{
					//// Sub-Tabs ghetto

					//if (Global::Aimbotsubtab == 0)
					//	style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
					//else
					//	style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);

					//if (ImGui::Button("Aimbot", ImVec2(183.6, 29))) Global::Aimbotsubtab = 0;
					//ImGui::SameLine();

					//if (Global::Aimbotsubtab == 1)
					//	style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
					//else
					//	style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
					//if (ImGui::Button("RCS", ImVec2(183.6, 29))) Global::Aimbotsubtab = 1;

					//style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
					ImGui::Text("(RCS) Recoil Control System");
					ImGui::Separator();
					ImGui::Spacing();
						//PISTOLAS
						if (Global::LegitSubTab == 0)
						{
							ImGui::Text("");
							const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
							const char* RCSType[] = { "Default", "RCS Horizontal & Vertical" };


							ImGui::PushItemWidth(280.f);
							
							ImGui::Combo("RCS Type", &Settings::Aimbot::aim_RCSType[3], RCSType, IM_ARRAYSIZE(RCSType));
							//ImGui::SliderInt("RCS after X shots", &Settings::Aimbot::weapon_aim_settings[iWeaponID].AimBot_RcsAfterXShot, 1, 30);
							if (Settings::Aimbot::aim_RCSType[3] <= 0)
							{
								ImGui::SliderInt("RCS", &Settings::Aimbot::aim_Rcs[3], 0, 100);
							}
							if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RCSType >= 1)
							{
								ImGui::SliderInt("RCS X", &Settings::Aimbot::aim_Rcsx[3], 0, 100);
								ImGui::SliderInt("RCS Y", &Settings::Aimbot::aim_Rcsy[3], 0, 100);
							}
							





						}
						 else if (Global::LegitSubTab == 1)
						{
							ImGui::Text("");
							const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
							const char* RCSType[] = { "Default", "RCS Horizontal & Vertical" };


							ImGui::PushItemWidth(280.f);
							
							ImGui::Combo("RCS Clamp", &Settings::Aimbot::aim_RcsClampType[1], ClampType, IM_ARRAYSIZE(ClampType));
							
							ImGui::Combo("RCS Type", &Settings::Aimbot::aim_RCSType[1], RCSType, IM_ARRAYSIZE(RCSType));
							//ImGui::SliderInt("RCS after X shots", &Settings::Aimbot::weapon_aim_settings[iWeaponID].AimBot_RcsAfterXShot, 1, 30);
							if (Settings::Aimbot::aim_RCSType[1] <= 0)
							{
								ImGui::SliderInt("RCS", &Settings::Aimbot::aim_Rcs[1], 0, 100);
							}
							if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RCSType >= 1)
							{
								ImGui::SliderInt("RCS X", &Settings::Aimbot::aim_Rcsx[1], 0, 100);
								ImGui::SliderInt("RCS Y", &Settings::Aimbot::aim_Rcsy[1], 0, 100);
							}
							
								ImGui::SliderInt("RCS Fov", &Settings::Aimbot::aim_RcsFov[1], 0, 300);
								ImGui::SliderInt("RCS Smooth", &Settings::Aimbot::aim_RcsSmooth[1], 0, 300);
							
						
						
						
						}
						 else if (Global::LegitSubTab == 2)
						{
							ImGui::Text("");
							const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
							const char* RCSType[] = { "Default", "RCS Horizontal & Vertical" };


							ImGui::PushItemWidth(280.f);
							
								ImGui::Combo("RCS Clamp", &Settings::Aimbot::aim_RcsClampType[2], ClampType, IM_ARRAYSIZE(ClampType));
							
							ImGui::Combo("RCS Type", &Settings::Aimbot::aim_RCSType[2], RCSType, IM_ARRAYSIZE(RCSType));
							//ImGui::SliderInt("RCS after X shots", &Settings::Aimbot::weapon_aim_settings[iWeaponID].AimBot_RcsAfterXShot, 1, 30);
							if (Settings::Aimbot::aim_RCSType[2] <= 0)
							{
								ImGui::SliderInt("RCS", &Settings::Aimbot::aim_Rcs[2], 0, 100);
							}
							if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RCSType >= 1)
							{
								ImGui::SliderInt("RCS X", &Settings::Aimbot::aim_Rcsx[2], 0, 100);
								ImGui::SliderInt("RCS Y", &Settings::Aimbot::aim_Rcsy[2], 0, 100);
							}
							
								ImGui::SliderInt("RCS Fov", &Settings::Aimbot::aim_RcsFov[2], 0, 300);
								ImGui::SliderInt("RCS Smooth", &Settings::Aimbot::aim_RcsSmooth[2], 0, 300);
							
						}
						 else if (Global::LegitSubTab == 3)
						{
							ImGui::Text("");
							const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
							const char* RCSType[] = { "Default", "RCS Horizontal & Vertical" };


							ImGui::PushItemWidth(280.f);
							
							ImGui::Combo("RCS Type", &Settings::Aimbot::aim_RCSType[4], RCSType, IM_ARRAYSIZE(RCSType));
							//ImGui::SliderInt("RCS after X shots", &Settings::Aimbot::weapon_aim_settings[iWeaponID].AimBot_RcsAfterXShot, 1, 30);
							if (Settings::Aimbot::aim_RCSType[4] <= 0)
							{
								ImGui::SliderInt("RCS", &Settings::Aimbot::aim_Rcs[4], 0, 100);
							}
							if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RCSType >= 1)
							{
								ImGui::SliderInt("RCS X", &Settings::Aimbot::aim_Rcsx[4], 0, 100);
								ImGui::SliderInt("RCS Y", &Settings::Aimbot::aim_Rcsy[4], 0, 100);
							}
							
						}
						 else if (Global::LegitSubTab == 4)
						{
							ImGui::Text("");
							const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
							const char* RCSType[] = { "Default", "RCS Horizontal & Vertical" };


							ImGui::PushItemWidth(280.f);
							
							ImGui::Combo("RCS Clamp", &Settings::Aimbot::aim_RcsClampType[0], ClampType, IM_ARRAYSIZE(ClampType));
							
							ImGui::Combo("RCS Type", &Settings::Aimbot::aim_RCSType[0], RCSType, IM_ARRAYSIZE(RCSType));
							//ImGui::SliderInt("RCS after X shots", &Settings::Aimbot::weapon_aim_settings[iWeaponID].AimBot_RcsAfterXShot, 1, 30);
							if (Settings::Aimbot::aim_RCSType[0] <= 0)
							{
								ImGui::SliderInt("RCS", &Settings::Aimbot::aim_Rcs[0], 0, 100);
							}
							if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RCSType >= 1)
							{
								ImGui::SliderInt("RCS X", &Settings::Aimbot::aim_Rcsx[0], 0, 100);
								ImGui::SliderInt("RCS Y", &Settings::Aimbot::aim_Rcsy[0], 0, 100);
							}
							
							ImGui::SliderInt("RCS Fov", &Settings::Aimbot::aim_RcsFov[0], 0, 300);
							ImGui::SliderInt("RCS Smooth", &Settings::Aimbot::aim_RcsSmooth[0], 0, 300);
							
 }
						 else if (Global::LegitSubTab == 5)
						{

								ImGui::Text("");
								const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
								const char* RCSType[] = { "Default", "RCS Horizontal & Vertical" };


								ImGui::PushItemWidth(280.f);
								
								ImGui::Combo("RCS Type", &Settings::Aimbot::aim_RCSType[5], RCSType, IM_ARRAYSIZE(RCSType));
								//ImGui::SliderInt("RCS after X shots", &Settings::Aimbot::weapon_aim_settings[iWeaponID].AimBot_RcsAfterXShot, 1, 30);
								if (Settings::Aimbot::aim_RCSType[5] <= 0)
								{
									ImGui::SliderInt("RCS", &Settings::Aimbot::aim_Rcs[5], 0, 100);
								}
								if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RCSType >= 1)
								{
									ImGui::SliderInt("RCS X", &Settings::Aimbot::aim_Rcsx[5], 0, 100);
									ImGui::SliderInt("RCS Y", &Settings::Aimbot::aim_Rcsy[5], 0, 100);
								}
								
						}





						
						
					

				}ImGui::EndChild();

			}
			
			if (TabSelect == 1) //Visuals
			{
				DrawVisualsTab();
				ImGui::BeginChild("Visuals - General", ImVec2(384, -1), true);
				{
					ImGui::Text("General");
					ImGui::Separator();
					ImGui::Checkbox("Esp Line", &Settings::Esp::esp_Line);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Bomb Player", &Settings::Esp::esp_Bomb);
					ImGui::Checkbox("Esp Defusing", &Settings::Esp::esp_Defusing);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Esp Scoped", &Settings::Esp::esp_Scoped);
					ImGui::Checkbox("Esp Flashed", &Settings::Esp::esp_Flashed);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Bomb Planted", &Settings::Esp::esp_BombPlanted);
					ImGui::Checkbox("Rank", &Settings::Esp::esp_Rank);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Drop Weapon", &Settings::Esp::esp_WorldWeapons);
					ImGui::Checkbox("Drop Grenade", &Settings::Esp::esp_WorldGrenade);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Draw Aim-Spot", &Settings::Aimbot::aim_DrawSpot);
					ImGui::Checkbox("Draw Aim-Fov", &Settings::Aimbot::aim_DrawFov);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Renders radius of influence of aimbot");
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Night Mode", &Settings::Esp::esp_NightMode);
					ImGui::Checkbox("Dlight", &Settings::Esp::esp_Dlightz);
					//ImGui::Checkbox("Asus Walls", &Settings::Esp::esp_AsusWalls);
					/*if (Settings::Esp::esp_AsusWalls)
					{
						
						ImGui::SliderInt("Walls Opacity", &Settings::Esp::esp_WallsOpacity, 0, 100);
					}*/

					
					/*if (ImGui::Combo("Skybox", &Settings::Misc::misc_CurrentSky, skybox_items, IM_ARRAYSIZE(skybox_items)))
					{
						Settings::Misc::misc_SkyName = skybox_items[Settings::Misc::misc_CurrentSky];
					}*/
					ImGui::Text("Materials");
					ImGui::Separator();
					ImGui::PushItemWidth(130.f);
					
					ImGui::Combo("Hands materials", &Settings::Esp::esp_rendergrenadecheck, "None\0\Wireframe\0\0", -1);
					ImGui::PopItemWidth();
					
					
					ImGui::Text("Filter");
					ImGui::Separator();
					ImGui::Checkbox("Team", &Settings::Esp::esp_Team);
					ImGui::Checkbox("Enemy", &Settings::Esp::esp_Enemy);
					ImGui::Checkbox("Animals", &Settings::Esp::esp_Animals);
				}ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("Aimbot - General", ImVec2(384, -1), true);
				{
					ImGui::Text("Players");
					ImGui::Separator();
					ImGui::Checkbox("Name", &Settings::Esp::esp_Name);
					ImGui::Checkbox("Skeleton", &Settings::Esp::esp_Skeleton);
					ImGui::Checkbox("Ammo", &Settings::Esp::esp_Ammo);
					ImGui::Checkbox("Distance", &Settings::Esp::esp_Distance);
					ImGui::Checkbox("Glow", &Settings::Esp::esp_Glow);
					
					
					ImGui::PushItemWidth(80.f);
					//ImGui::Checkbox("Weapon", &Settings::Esp::esp_Weapon);
					ImGui::Combo("Esp Weapon##", &Settings::Esp::esp_WeaponIcon, "None\0\Name\0\rIcon\0\0", -1);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(280.f);
					//ImGui::SliderInt("Esp Size", &Settings::Esp::esp_Size, 1, 10);
					ImGui::SliderInt("Aim Direction", &Settings::Esp::esp_BulletTrace, 0, 500);  //3000


					string style_1 = "None";
					string style_2 = "Box";
					string style_3 = "Frame";
					string style_4 = "Genuine";
					const char* items1[] = { style_1.c_str() , style_2.c_str() , style_3.c_str() , style_4.c_str() };
					ImGui::PushItemWidth(130.f);
					ImGui::Combo("Box ", &Settings::Esp::esp_Style, items1, IM_ARRAYSIZE(items1));
					ImGui::SameLine();
					ImGui::Checkbox("Outline", &Settings::Esp::esp_Outline);
					ImGui::SameLine();
					ImGui::Checkbox("XYZ Esp", &Settings::Esp::esp_Visible);

					ImGui::Spacing();

					ImGui::Text("Info Health & Armor");
					ImGui::Separator();
					string hpbar_1 = "None";
					string hpbar_2 = "Number";
					string hpbar_3 = "Bottom";
					string hpbar_4 = "Left";
					string hpbar_5 = "Edgy";
					const char* items3[] = { hpbar_1.c_str() , hpbar_2.c_str() , hpbar_3.c_str() , hpbar_4.c_str() , hpbar_5.c_str() };

					ImGui::PushItemWidth(130.f);
					ImGui::Combo("Health", &Settings::Esp::esp_Health, items3, IM_ARRAYSIZE(items3));
					ImGui::PopItemWidth();

					string arbar_1 = "None";
					string arbar_2 = "Number";
					string arbar_3 = "Bottom";
					string arbar_4 = "Right";
					string arbar_5 = "Edgy";
					const char* items4[] = { arbar_1.c_str() , arbar_2.c_str() , arbar_3.c_str() , arbar_4.c_str() , arbar_5.c_str() };

					ImGui::PushItemWidth(130.f);

					ImGui::Combo("Armor", &Settings::Esp::esp_Armor, items4, IM_ARRAYSIZE(items4));
					ImGui::PopItemWidth();

					ImGui::Spacing();

					ImGui::Text("Chams");
					ImGui::Separator();
					string chams_1 = "None";
					string chams_2 = "Flat";
					string chams_3 = "Texture";
					string chams_4 = "Flat + WH";
					string chams_5 = "Texture + WH";
					string chams_6 = "Material";
					const char* items5[] = { chams_1.c_str() , chams_2.c_str() , chams_3.c_str(), chams_4.c_str(), chams_5.c_str(), chams_6.c_str() };

					ImGui::PushItemWidth(130.f);
					ImGui::Combo("Chams", &Settings::Esp::esp_Chams, items5, IM_ARRAYSIZE(items5));
					if (Settings::Esp::esp_Chams == 4 || Settings::Esp::esp_Chams == 3)
						Settings::Esp::esp_ChamsVisible = true;
					else
						Settings::Esp::esp_ChamsVisible = false;

					ImGui::SameLine();
					//ImGui::Checkbox("Chams Wall", &Settings::Esp::esp_ChamsVisible);
					if (Settings::Esp::esp_Chams == 6)
					{
						ImGui::SliderInt("Light of Chams", &Settings::Esp::esp_Chams_Light, 1, 255);
					}

				}ImGui::EndChild();
				
				
			}
			if (TabSelect == 2) //Skins
			{
				DrawSkinsTab();

				// Sub-Tabs ghetto

				if (Global::RageSubTab == 0)
					style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
				else
					style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);

				if (ImGui::Button("Weapon", ImVec2(386, 32))) Global::RageSubTab = 0;
				ImGui::SameLine();

				if (Global::RageSubTab == 1)
					style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
				else
					style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
				if (ImGui::Button("Knife", ImVec2(386, 32))) Global::RageSubTab = 1;

				style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

				if (Global::RageSubTab == 1)
				{
					ImGui::BeginChild("knfifechanger", ImVec2(772, 398), true);
					{

						const char* knife_models_items[] =
						{
							"Default","Bayonet","Flip","Gut","Karambit" ,"M9 Bayonet",
							"Huntsman","Falchion","Bowie","Butterfly","Shadow Daggers", "Stiletto",
							"Ursus"

						};

						/*if (ImGui::BeginCombo("removals", "...", ImVec2(0, 105)))
						{
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("smoke"), &c_config::get().remove_smoke, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("flash"), &c_config::get().reduce_flash, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("sleeves"), &c_config::get().remove_sleeves, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("hands"), &c_config::get().remove_hands, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("scope overlay"), &c_config::get().remove_scope, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}*/

						static int iSelectKnifeCTSkinIndex = -1;
						static int iSelectKnifeTTSkinIndex = -1;

						int iKnifeCTModelIndex = Settings::Skin::knf_ct_model;
						int iKnifeTTModelIndex = Settings::Skin::knf_tt_model;

						static int iOldKnifeCTModelIndex = -1;
						static int iOldKnifeTTModelIndex = -1;

						if (iOldKnifeCTModelIndex != iKnifeCTModelIndex && Settings::Skin::knf_ct_model)
							iSelectKnifeCTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, false);

						if (iOldKnifeTTModelIndex != iKnifeTTModelIndex && Settings::Skin::knf_tt_model)
							iSelectKnifeTTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, true);

						iOldKnifeCTModelIndex = iKnifeCTModelIndex;
						iOldKnifeTTModelIndex = iKnifeTTModelIndex;

						string KnifeCTModel = knife_models_items[Settings::Skin::knf_ct_model];
						string KnifeTTModel = knife_models_items[Settings::Skin::knf_tt_model];

						ImGui::PushItemWidth(210.f);
						ImGui::Combo("Knife CT", &Settings::Skin::knf_ct_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
						ImGui::ComboBoxArray(KnifeCTModel.c_str(), &iSelectKnifeCTSkinIndex, KnifeSkins[iKnifeCTModelIndex].SkinNames);
						ImGui::InputInt("Custom Knife CT ID", &Settings::Skin::knf_ct_skin);
						
						ImGui::SliderFloat("Wear CT", &g_SkinChangerCfg[WEAPON_KNIFE].flFallbackWear, 0.000f, 1.f);
						ImGui::SliderInt("Seed CT", &g_SkinChangerCfg[WEAPON_KNIFE].nFallbackSeed, 0, 1000);
						ImGui::Separator();

						ImGui::Spacing();;

						ImGui::Combo("Knife TT", &Settings::Skin::knf_tt_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
						ImGui::ComboBoxArray(KnifeTTModel.c_str(), &iSelectKnifeTTSkinIndex, KnifeSkins[iKnifeTTModelIndex].SkinNames);
						ImGui::InputInt("Custom Knife TT ID", &Settings::Skin::knf_tt_skin);
						ImGui::SliderFloat("Wear TT", &g_SkinChangerCfg[WEAPON_KNIFE_T].flFallbackWear, 0.000f, 1.f);
						ImGui::SliderInt("Seed TT", &g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackSeed, 0, 1000);
						ImGui::PopItemWidth();

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						if (ImGui::Button("Apply Skins Knifes", ImVec2(-1, 0)))
						{

							if (iSelectKnifeCTSkinIndex >= 0)
							{
								Settings::Skin::knf_ct_skin = KnifeSkins[iKnifeCTModelIndex].SkinPaintKit[iSelectKnifeCTSkinIndex];
							}
							if (iSelectKnifeTTSkinIndex >= 0)
							{
								Settings::Skin::knf_tt_skin = KnifeSkins[iKnifeTTModelIndex].SkinPaintKit[iSelectKnifeTTSkinIndex];
							}

							ForceFullUpdate();
						}

					}ImGui::EndChild();
				}
				else if (Global::RageSubTab == 0)
				{

					ImGui::BeginChild("Skinchanger", ImVec2(386, 398), true);
					{
						const char* knife_models_items[] =
						{
							"Default","Bayonet","Flip","Gut","Karambit" ,"M9 Bayonet",
							"Huntsman","Falchion","Bowie","Butterfly","Shadow Daggers", "Navaja", "Stiletto",
							"Ursus", "Talon"

						};

						static int iSelectKnifeCTSkinIndex = -1;
						static int iSelectKnifeTTSkinIndex = -1;

						int iKnifeCTModelIndex = Settings::Skin::knf_ct_model;
						int iKnifeTTModelIndex = Settings::Skin::knf_tt_model;

						static int iOldKnifeCTModelIndex = -1;
						static int iOldKnifeTTModelIndex = -1;

						if (iOldKnifeCTModelIndex != iKnifeCTModelIndex && Settings::Skin::knf_ct_model)
							iSelectKnifeCTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, false);

						if (iOldKnifeTTModelIndex != iKnifeTTModelIndex && Settings::Skin::knf_tt_model)
							iSelectKnifeTTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, true);

						iOldKnifeCTModelIndex = iKnifeCTModelIndex;
						iOldKnifeTTModelIndex = iKnifeTTModelIndex;

						string KnifeCTModel = knife_models_items[Settings::Skin::knf_ct_model];
						string KnifeTTModel = knife_models_items[Settings::Skin::knf_tt_model];

						ImGui::Text("Weapon: %s", weapons[iWeaponID]);
						ImGui::Separator();
						static int iOldWeaponID = -1;

						ImGui::PushItemWidth(300.f);
						ImGui::Combo("Weapon##WeaponSelect", &iWeaponID, weapons, IM_ARRAYSIZE(weapons));
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(300.f);
						ImGui::InputInt("Custom ID", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackPaintKit);
						ImGui::PopItemWidth();
						iWeaponSelectIndex = pWeaponItemIndexData[iWeaponID];

						if (iOldWeaponID != iWeaponID)
							iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

						iOldWeaponID = iWeaponID;

						string WeaponSkin = weapons[iWeaponID];

						ImGui::PushItemWidth(350.f);
						ImGui::ComboBoxArray(WeaponSkin.c_str(), &iWeaponSelectSkinIndex, WeaponSkins[iWeaponID].SkinNames);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(300.f);
						ImGui::SliderFloat("Wear WP", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].flFallbackWear, 0.000f, 1.f);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(300.f);
						ImGui::SliderInt("Seed WP", &g_SkinChangerCfg[pWeaponItemIndexData[iWeaponID]].nFallbackSeed, 0, 1000);
						ImGui::PopItemWidth();

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						if (ImGui::Button("Apply Skins Weapons", ImVec2(-1, 0)))
						{

							if (iWeaponSelectSkinIndex >= 0)
							{
								g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit = WeaponSkins[iWeaponID].SkinPaintKit[iWeaponSelectSkinIndex];
							}

							ForceFullUpdate();
						}

					}ImGui::EndChild();
					ImGui::SameLine();
					ImGui::BeginChild("Gloveschanger", ImVec2(386, 398), true);
					{

						ImGui::PushItemWidth(300.f);
						ImGui::Combo("Gloves", &Settings::Skin::gloves_skin, gloves_listbox_items, IM_ARRAYSIZE(gloves_listbox_items));
						ImGui::PopItemWidth();
				
						ImGui::PopItemWidth();

						if (ImGui::Button("Apply Gloves", ImVec2(-1, 0)))
						{
							ForceFullUpdate();
						}


						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						ImGui::BeginChild("##childstick", ImVec2(265, -1), true); // uffja
						{
							static ImGuiTextFilter filter;
							filter.Draw();
							const char* lines[] =
							{
								"2: Groundwater",
								"3: Candy Apple",
								"5: Forest DDPAT",
								"6: Arctic Camo",
								"8: Desert Storm",
								"9: Bengal Tiger",
								"10: Copperhead",
								"11: Skulls",
								"12: Crimson Web",
								"13: Blue Streak",
								"14: Red Laminate",
								"15: Gunsmoke",
								"16: Jungle Tiger",
								"17: Urban DDPAT",
								"20: Virus",
								"21: Granite Marbleized",
								"22: Contrast Spray",
								"25: Forest Leaves",
								"26: Lichen Dashed",
								"27: Bone Mask",
								"28: Anodized Navy",
								"30: Snake Camo",
								"32: Silver",
								"33: Hot Rod",
								"34: Metallic DDPAT",
								"36: Ossified",
								"37: Blaze",
								"38: Fade",
								"39: Bulldozer",
								"40: Night",
								"41: Copper",
								"42: Blue Steel",
								"43: Stained",
								"44: Case Hardened",
								"46: Contractor",
								"47: Colony",
								"48: Dragon Tattoo",
								"51: Lightning Strike",
								"59: Slaughter",
								"60: Dark Water",
								"61: Hypnotic",
								"62: Bloomstick",
								"67: Cold Blooded",
								"70: Carbon Fiber",
								"71: Scorpion",
								"72: Safari Mesh",
								"73: Wings",
								"74: Polar Camo",
								"75: Blizzard Marbleized",
								"76: Winter Forest",
								"77: Boreal Forest",
								"78: Forest Night",
								"83: Orange DDPAT",
								"84: Pink DDPAT",
								"90: Mudder",
								"92: Cyanospatter",
								"93: Caramel",
								"95: Grassland",
								"96: Blue Spruce",
								"98: Ultraviolet",
								"99: Sand Dune",
								"100: Storm",
								"101: Tornado",
								"102: Whiteout",
								"104: Grassland Leaves",
								"107: Polar Mesh",
								"110: Condemned",
								"111: Glacier Mesh",
								"116: Sand Mesh",
								"119: Sage Spray",
								"122: Jungle Spray",
								"124: Sand Spray",
								"135: Urban Perforated",
								"136: Waves Perforated",
								"141: Orange Peel",
								"143: Urban Masked",
								"147: Jungle Dashed",
								"148: Sand Dashed",
								"149: Urban Dashed",
								"151: Jungle",
								"153: Demolition",
								"154: Afterimage",
								"155: Bullet Rain",
								"156: Death by Kitty",
								"157: Palm",
								"158: Walnut",
								"159: Brass",
								"162: Splash",
								"164: Modern Hunter",
								"165: Splash Jam",
								"166: Blaze Orange",
								"167: Radiation Hazard",
								"168: Nuclear Threat",
								"169: Fallout Warning",
								"170: Predator",
								"171: Irradiated Alert",
								"172: Black Laminate",
								"174: BOOM",
								"175: Scorched",
								"176: Faded Zebra",
								"177: Memento",
								"178: Doomkitty",
								"179: Nuclear Threat",
								"180: Fire Serpent",
								"181: Corticera",
								"182: Emerald Dragon",
								"183: Overgrowth",
								"184: Corticera",
								"185: Golden Koi",
								"186: Wave Spray",
								"187: Zirka",
								"188: Graven",
								"189: Bright Water",
								"190: Black Limba",
								"191: Tempest",
								"192: Shattered",
								"193: Bone Pile",
								"194: Spitfire",
								"195: Demeter",
								"196: Emerald",
								"197: Anodized Navy",
								"198: Hazard",
								"199: Dry Season",
								"200: Mayan Dreams",
								"201: Palm",
								"202: Jungle DDPAT",
								"203: Rust Coat",
								"204: Mosaico",
								"205: Jungle",
								"206: Tornado",
								"207: Facets",
								"208: Sand Dune",
								"209: Groundwater",
								"210: Anodized Gunmetal",
								"211: Ocean Foam",
								"212: Graphite",
								"213: Ocean Foam",
								"214: Graphite",
								"215: X-Ray",
								"216: Blue Titanium",
								"217: Blood Tiger",
								"218: Hexane",
								"219: Hive",
								"220: Hemoglobin",
								"221: Serum",
								"222: Blood in the Water",
								"223: Nightshade",
								"224: Water Sigil",
								"225: Ghost Camo",
								"226: Blue Laminate",
								"227: Electric Hive",
								"228: Blind Spot",
								"229: Azure Zebra",
								"230: Steel Disruption",
								"231: Cobalt Disruption",
								"232: Crimson Web",
								"233: Tropical Storm",
								"234: Ash Wood",
								"235: VariCamo",
								"236: Night Ops",
								"237: Urban Rubble",
								"238: VariCamo Blue",
								"240: CaliCamo",
								"241: Hunting Blind",
								"242: Army Mesh",
								"243: Gator Mesh",
								"244: Teardown",
								"245: Army Recon",
								"246: Amber Fade",
								"247: Damascus Steel",
								"248: Red Quartz",
								"249: Cobalt Quartz",
								"250: Full Stop",
								"251: Pit Viper",
								"252: Silver Quartz",
								"253: Acid Fade",
								"254: Nitro",
								"255: Asiimov",
								"256: The Kraken",
								"257: Guardian",
								"258: Mehndi",
								"259: Redline",
								"260: Pulse",
								"261: Marina",
								"262: Rose Iron",
								"263: Rising Skull",
								"264: Sandstorm",
								"265: Kami",
								"266: Magma",
								"267: Cobalt Halftone",
								"268: Tread Plate",
								"269: The Fuschia Is Now",
								"270: Victoria",
								"271: Undertow",
								"272: Titanium Bit",
								"273: Heirloom",
								"274: Copper Galaxy",
								"275: Red FragCam",
								"276: Panther",
								"277: Stainless",
								"278: Blue Fissure",
								"279: Asiimov",
								"280: Chameleon",
								"281: Corporal",
								"282: Redline",
								"283: Trigon",
								"284: Heat",
								"285: Terrain",
								"286: Antique",
								"287: Pulse",
								"288: Sergeant",
								"289: Sandstorm",
								"290: Guardian",
								"291: Heaven Guard",
								"293: Death Rattle",
								"294: Green Apple",
								"295: Franklin",
								"296: Meteorite",
								"297: Tuxedo",
								"298: Army Sheen",
								"299: Caged Steel",
								"300: Emerald Pinstripe",
								"301: Atomic Alloy",
								"302: Vulcan",
								"303: Isaac",
								"304: Slashed",
								"305: Torque",
								"306: Antique",
								"307: Retribution",
								"308: Kami",
								"309: Howl",
								"310: Curse",
								"311: Desert Warfare",
								"312: Cyrex",
								"313: Orion",
								"314: Heaven Guard",
								"315: Poison Dart",
								"316: Jaguar",
								"317: Bratatat",
								"318: Road Rash",
								"319: Detour",
								"320: Red Python",
								"321: Master Piece",
								"322: Nitro",
								"323: Rust Coat",
								"325: Chalice",
								"326: Knight",
								"327: Chainmail",
								"328: Hand Cannon",
								"329: Dark Age",
								"330: Briar",
								"332: Royal Blue",
								"333: Indigo",
								"334: Twist",
								"335: Module",
								"336: Desert-Strike",
								"337: Tatter",
								"338: Pulse",
								"339: Caiman",
								"340: Jet Set",
								"341: First Class",
								"342: Leather",
								"343: Commuter",
								"344: Dragon Lore",
								"345: First Class",
								"346: Coach Class",
								"347: Pilot",
								"348: Red Leather",
								"349: Osiris",
								"350: Tigris",
								"351: Conspiracy",
								"352: Fowl Play",
								"353: Water Elemental",
								"354: Urban Hazard",
								"355: Desert-Strike",
								"356: Koi",
								"357: Ivory",
								"358: Supernova",
								"359: Asiimov",
								"360: Cyrex",
								"361: Abyss",
								"362: Labyrinth",
								"363: Traveler",
								"364: Business Class",
								"365: Olive Plaid",
								"366: Green Plaid",
								"367: Reactor",
								"368: Setting Sun",
								"369: Nuclear Waste",
								"370: Bone Machine",
								"371: Styx",
								"372: Nuclear Garden",
								"373: Contamination",
								"374: Toxic",
								"375: Radiation Hazard",
								"376: Chemical Green",
								"377: Hot Shot",
								"378: Fallout Warning",
								"379: Cerberus",
								"380: Wasteland Rebel",
								"381: Grinder",
								"382: Murky",
								"383: Basilisk",
								"384: Griffin",
								"385: Firestarter",
								"386: Dart",
								"387: Urban Hazard",
								"388: Cartel",
								"389: Fire Elemental",
								"390: Highwayman",
								"391: Cardiac",
								"392: Delusion",
								"393: Tranquility",
								"394: Cartel",
								"395: Man-o'-war",
								"396: Urban Shock",
								"397: Naga",
								"398: Chatterbox",
								"399: Catacombs",
								"400: 龍王 (Dragon King)",
								"401: System Lock",
								"402: Malachite",
								"403: Deadly Poison",
								"404: Muertos",
								"405: Serenity",
								"406: Grotto",
								"407: Quicksilver",
								"409: Tiger Tooth",
								"410: Damascus Steel",
								"411: Damascus Steel",
								"413: Marble Fade",
								"414: Rust Coat",
								"415: Doppler",
								"416: Doppler",
								"417: Doppler",
								"418: Doppler",
								"419: Doppler",
								"420: Doppler",
								"421: Doppler",
								"422: Elite Build",
								"423: Armor Core",
								"424: Worm God",
								"425: Bronze Deco",
								"426: Valence",
								"427: Monkey Business",
								"428: Eco",
								"429: Djinn",
								"430: Hyper Beast",
								"431: Heat",
								"432: Man-o'-war",
								"433: Neon Rider",
								"434: Origami",
								"435: Pole Position",
								"436: Grand Prix",
								"437: Twilight Galaxy",
								"438: Chronos",
								"439: Hades",
								"440: Icarus Fell",
								"441: Minotaur's Labyrinth",
								"442: Asterion",
								"443: Pathfinder",
								"444: Daedalus",
								"445: Hot Rod",
								"446: Medusa",
								"447: Duelist",
								"448: Pandora's Box",
								"449: Poseidon",
								"450: Moon in Libra",
								"451: Sun in Leo",
								"452: Shipping Forecast",
								"453: Emerald",
								"454: Para Green",
								"455: Akihabara Accept",
								"456: Hydroponic",
								"457: Bamboo Print",
								"458: Bamboo Shadow",
								"459: Bamboo Forest",
								"460: Aqua Terrace",
								"462: Counter Terrace",
								"463: Terrace",
								"464: Neon Kimono",
								"465: Orange Kimono",
								"466: Crimson Kimono",
								"467: Mint Kimono",
								"468: Midnight Storm",
								"469: Sunset Storm 壱",
								"470: Sunset Storm 弐",
								"471: Daybreak",
								"472: Impact Drill",
								"473: Seabird",
								"474: Aquamarine Revenge",
								"475: Hyper Beast",
								"476: Yellow Jacket",
								"477: Neural Net",
								"478: Rocket Pop",
								"479: Bunsen Burner",
								"480: Evil Daimyo",
								"481: Nemesis",
								"482: Ruby Poison Dart",
								"483: Loudmouth",
								"484: Ranger",
								"485: Handgun",
								"486: Elite Build",
								"487: Cyrex",
								"488: Riot",
								"489: Torque",
								"490: Frontside Misty",
								"491: Dualing Dragons",
								"492: Survivor Z",
								"493: Flux",
								"494: Stone Cold",
								"495: Wraiths",
								"496: Nebula Crusader",
								"497: Golden Coil",
								"498: Rangeen",
								"499: Cobalt Core",
								"500: Special Delivery",
								"501: Wingshot",
								"502: Green Marine",
								"503: Big Iron",
								"504: Kill Confirmed",
								"505: Scumbria",
								"506: Point Disarray",
								"507: Ricochet",
								"508: Fuel Rod",
								"509: Corinthian",
								"510: Retrobution",
								"511: The Executioner",
								"512: Royal Paladin",
								"514: Power Loader",
								"515: Imperial",
								"516: Shapewood",
								"517: Yorick",
								"518: Outbreak",
								"519: Tiger Moth",
								"520: Avalanche",
								"521: Teclu Burner",
								"522: Fade",
								"523: Amber Fade",
								"524: Fuel Injector",
								"525: Elite Build",
								"526: Photic Zone",
								"527: Kumicho Dragon",
								"528: Cartel",
								"529: Valence",
								"530: Triumvirate",
								"532: Royal Legion",
								"533: The Battlestar",
								"534: Lapis Gator",
								"535: Praetorian",
								"536: Impire",
								"537: Hyper Beast",
								"538: Necropos",
								"539: Jambiya",
								"540: Lead Conduit",
								"541: Fleet Flock",
								"542: Judgement of Anubis",
								"543: Red Astor",
								"544: Ventilators",
								"545: Orange Crash",
								"546: Firefight",
								"547: Spectre",
								"548: Chantico's Fire",
								"549: Bioleak",
								"550: Oceanic",
								"551: Asiimov",
								"552: Fubar",
								"553: Atlas",
								"554: Ghost Crusader",
								"555: Re-Entry",
								"556: Primal Saber",
								"557: Black Tie",
								"558: Lore",
								"559: Lore",
								"560: Lore",
								"561: Lore",
								"562: Lore",
								"563: Black Laminate",
								"564: Black Laminate",
								"565: Black Laminate",
								"566: Black Laminate",
								"567: Black Laminate",
								"568: Gamma Doppler",
								"569: Gamma Doppler",
								"570: Gamma Doppler",
								"571: Gamma Doppler",
								"572: Gamma Doppler",
								"573: Autotronic",
								"574: Autotronic",
								"575: Autotronic",
								"576: Autotronic",
								"577: Autotronic",
								"578: Bright Water",
								"579: Bright Water",
								"580: Freehand",
								"581: Freehand",
								"582: Freehand",
								"583: Aristocrat",
								"584: Phobos",
								"585: Violent Daimyo",
								"586: Wasteland Rebel",
								"587: Mecha Industries",
								"588: Desolate Space",
								"589: Carnivore",
								"590: Exo",
								"591: Imperial Dragon",
								"592: Iron Clad",
								"593: Chopper",
								"594: Harvester",
								"595: Reboot",
								"596: Limelight",
								"597: Bloodsport",
								"598: Aerial",
								"599: Ice Cap",
								"600: Neon Revolution",
								"601: Syd Mead",
								"602: Imprint",
								"603: Directive",
								"604: Roll Cage",
								"605: Scumbria",
								"606: Ventilator",
								"607: Weasel",
								"608: Petroglyph",
								"609: Airlock",
								"610: Dazzle",
								"611: Grim",
								"612: Powercore",
								"613: Triarch",
								"614: Fuel Injector",
								"615: Briefing",
								"616: Slipstream",
								"617: Doppler",
								"618: Doppler",
								"619: Doppler",
								"620: Ultraviolet",
								"621: Ultraviolet",
								"622: Polymer",
								"623: Ironwork",
								"624: Dragonfire",
								"625: Royal Consorts",
								"626: Mecha Industries",
								"627: Cirrus",
								"628: Stinger",
								"629: Black Sand",
								"630: Sand Scale",
								"631: Flashback",
								"632: Buzz Kill",
								"633: Sonar",
								"634: Gila",
								"635: Turf",
								"636: Shallow Grave",
								"637: Cyrex",
								"638: Wasteland Princess",
								"639: Bloodsport",
								"640: Fever Dream",
								"641: Jungle Slipstream",
								"642: Blueprint",
								"643: Xiangliu",
								"644: Decimator",
								"645: Oxide Blaze",
								"646: Capillary",
								"647: Crimson Tsunami",
								"648: Emerald Poison Dart",
								"649: Akoben",
								"650: Ripple",
								"651: Last Dive",
								"652: Scaffold",
								"653: Neo-Noir",
								"654: Seasons",
								"655: Zander",
								"656: Orbit Mk01",
								"657: Blueprint",
								"658: Cobra Strike",
								"659: Macabre",
								"660: Hyper Beast",
								"661: Sugar Rush",
								"662: Oni Taiji",
								"663: Briefing",
								"664: Hellfire",
								"665: Aloha",
								"666: Hard Water",
								"667: Woodsman",
								"668: Red Rock",
								"669: Death Grip",
								"670: Death's Head",
								"671: Cut Out",
								"672: Metal Flowers",
								"673: Morris",
								"674: Triqua",
								"675: The Empress",
								"676: High Roller",
								"677: Hunter",
								"678: See Ya Later",
								"679: Goo",
								"680: Off World",
								"681: Leaded Glass",
								"682: Oceanic",
								"683: Llama Cannon",
								"684: Cracked Opal",
								"685: Jungle Slipstream",
								"686: Phantom",
								"687: Tacticat",
								"688: Exposure",
								"689: Ziggy",
								"690: Stymphalian",
								"691: Mortis",
								"692: Night Riot",
								"693: Flame Test",
								"694: Moonrise",
								"695: Neo-Noir",
								"696: Bloodsport",
								"697: Black Sand",
								"698: Lionfish",
								"699: Wild Six",
								"700: Urban Hazard",
								"701: Grip",
								"702: Aloha",
								"703: SWAG-7",
								"704: Arctic Wolf",
								"705: Cortex",
								"706: Oxide Blaze",
							};
							for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
								if (filter.PassFilter(lines[i]))
									ImGui::Text("%s", lines[i]);
						}ImGui::EndChild();




					}ImGui::EndChild();
				}
			}

			if (TabSelect == 3) //Misc
			{
				DrawMiscTab();

				ImGui::BeginChild("##Misc1", ImVec2(384, -1), true);
				{
					
					
					
						ImGui::Text("Boost");
						ImGui::Separator();
						ImGui::Checkbox("Snow", &Settings::Misc::misc_Snow);
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("All map textures are blank");
						ImGui::Checkbox("NoSky ", &Settings::Misc::misc_NoSky);

						ImGui::Text("Crosshairs");
						ImGui::Separator();
						ImGui::Checkbox("Punch Dot", &Settings::Misc::misc_Punch);
						ImGui::Checkbox("Recoil Dot", &Settings::Misc::misc_Punch2);
						ImGui::Checkbox("Sniper Dot", &Settings::Misc::misc_AwpAim);
						//ImGui::Checkbox("Show Spread", &Settings::Misc::ShowSpread);
						

						ImGui::Text("Bhop");
						ImGui::Separator();
						ImGui::Checkbox("Bhop", &Settings::Misc::misc_Bhop);
						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Auto bunny hop");
						ImGui::Checkbox("AutoStrafe", &Settings::Misc::misc_AutoStrafe);
						ImGui::Checkbox("Spectators", &Settings::Misc::misc_Spectators);
						/*ImGui::Text("Removals");
						ImGui::Separator();
						if (ImGui::BeginCombo("removals", "...", ImVec2(0, 105)))
						{
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("smoke"), &Settings::Misc::misc_NoSmoke, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("flash"), &Settings::Misc::misc_NoFlash, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}*/
						

						
					
					
					

				} ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("##Misc2", ImVec2(384, -1), true);
				{
					ImGui::Text("Other");
					ImGui::Separator();
					ImGui::Checkbox("Anti-AFK", &Settings::Misc::misc_antiafk);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("LeftKnife", &Settings::Misc::misc_LeftHandKnife);
					ImGui::Checkbox("RightKnife", &Settings::Misc::misc_RightHandKnife);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Fast Duck", &Settings::Misc::misc_Duck);
					ImGui::Checkbox("Kill say spam", &Settings::Misc::misc_killspam);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("No smoke", &Settings::Misc::misc_NoSmoke);
					ImGui::Checkbox("No Flash", &Settings::Misc::misc_NoFlash);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Ragdolls", &Settings::Misc::Ragdoll);
					ImGui::Checkbox("BulletTracer", &Settings::Esp::esp_beams_tracer);
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Blackout", &Settings::Misc::Blackout);
					ImGui::Checkbox("Auto-accept", &Settings::Misc::Autoaccept);


					
					ImGui::Text("HitMarker & Hitlog");
					ImGui::Separator();
					ImGui::Checkbox("Hit Log", &Settings::Esp::esp_hitevent);
					ImGui::Checkbox("Hit Marker", &Settings::Misc::misc_HitMarker);
					ImGui::Combo("Sound", &Settings::Misc::misc_HitMarkerSound, iHitSound, ARRAYSIZE(iHitSound));
					ImGui::Spacing();
					
					

					ImGui::Spacing();
					ImGui::Text("Name Changer");
					ImGui::Separator();
					static char Change_Name[64] = { 0 };
					ImGui::PushItemWidth(150.f);


					ImGui::InputText("Name", Change_Name, 64);

					ImGui::SameLine();
					if (ImGui::Button("Change")) {
						ConVar* Name = Interfaces::GetConVar()->FindVar("name");
						*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;
						Name->SetValue(Change_Name);



					}
					ImGui::SameLine();
					if (ImGui::Button("No name")) {
						ConVar* Name = Interfaces::GetConVar()->FindVar("name");
						*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;
						Name->SetValue("\n");
					}


					if (ImGui::Button("Big name")) {
						ConVar* Name = Interfaces::GetConVar()->FindVar("name");
						*(int*)((DWORD)&Name->fnChangeCallback + 0xC) = 0;
						Name->SetValue("\n");
						Name->SetValue("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					}

				}ImGui::EndChild();

				
			}
			if (TabSelect == 4) //Colors
			{
				DrawColorTab();
				ImGui::BeginChild("##Colors", ImVec2(768, 398), true);
				{
					ImGui::Text("Colors");
										ImGui::Separator();
										ImGui::ColorEdit3("Esp Color CT", Settings::Esp::esp_Color_CT);
										
										ImGui::ColorEdit3("Esp Vis CT", Settings::Esp::esp_Color_VCT);
										ImGui::ColorEdit3("Chams CT", Settings::Esp::chams_Color_CT);
										
										ImGui::ColorEdit3("Chams Vis CT", Settings::Esp::chams_Color_VCT);
										ImGui::ColorEdit3("Esp Color T", Settings::Esp::esp_Color_TT);
										
										ImGui::ColorEdit3("Esp Vis T", Settings::Esp::esp_Color_VTT);
										ImGui::ColorEdit3("Chams T", Settings::Esp::chams_Color_TT);
										
										ImGui::ColorEdit3("Chams Vis T", Settings::Esp::chams_Color_VTT);
										ImGui::ColorEdit3("Esp Line", Settings::Esp::Visuals_Line_Color);
										ImGui::ColorEdit3("Aim-fov Color", Settings::Aimbot::aim_FovColor);
										ImGui::ColorEdit3("Drop Weapon & Grenade", Settings::Esp::WorldWeapon_Color);
										ImGui::ColorEdit3("Ambient Light", Settings::Esp::esp_Ambient);
										ImGui::ColorEdit3("Hit Color", Settings::Misc::misc_HitMarkerColor);
										ImGui::ColorEdit3("Tracer Color", Settings::Esp::flTracer_Beam);
										ImGui::ColorEdit3("Dot Color", Settings::Misc::misc_AwpAimColor);
										ImGui::ColorEdit3("Theme color", Settings::Misc::misc_themecolor);
				
				
				
				}ImGui::EndChild();
			}
			


			if (TabSelect == 5) //config
			{
				DrawConfigTab();
				ImGui::BeginChild("Config - General", ImVec2(768, 398), true);
				{
					ImGui::Text("Config");
					ImGui::Separator();

					ImGui::PushItemWidth(200.f);
					ImGui::InputText("", ConfigName, 64);
					ImGui::PopItemWidth();
					if (ImGui::Button("Create & Save new Config", ImVec2(200, 0)))
					{
						string ConfigFileName = ConfigName;

						if (ConfigFileName.size() < 1)
						{
							ConfigFileName = "settings";
						}
						Settings::SaveSettings("C:\\Aimware.cc\\" + ConfigFileName + ".ini");
						RefreshConfigs();
					}
					ImGui::PushItemWidth(200.f);
					ImGui::ListBoxConfigArray("##Select", &iConfigSelect, ConfigList);
					
					if (ImGui::Button("Refresh list", ImVec2(200, 0)))
					{
						RefreshConfigs();
					}
					ImGui::PopItemWidth();



					if (ImGui::Button("Load Config", ImVec2(200, 0)))
					{
						Settings::LoadSettings("C:\\Aimware.cc\\" + ConfigList[iConfigSelect]);
					}
					if (ImGui::Button("Save Config", ImVec2(200, 0)))
					{
						if (iConfigSelect >= 0)
							Settings::SaveSettings("C:\\Aimware.cc\\" + ConfigList[iConfigSelect]);
						else
							Settings::SaveSettings("C:\\Aimware.cc\\clearcfg.ini");
					}
					if (ImGui::Button("Delete Config", ImVec2(200, 0)))
					{
						remove(string("C:\\Aimware.cc\\" + ConfigList[iConfigSelect]).c_str());
						RefreshConfigs();
					}
					


				}ImGui::EndChild();
			}
		}ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x, mainWindowPos.y + 26));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
		ImGui::Begin("##border2", &bIsGuiVisible, ImVec2(800, 510), 0.98f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_ShowBorders);
		{

		}ImGui::End();




		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 6, mainWindowPos.y + 32));
		style.Colors[ImGuiCol_WindowBg] = ImVec4(1.f, 1.f, 1.f, 0.f);
		style.WindowPadding = ImVec2(0, 0);
		style.ItemSpacing = ImVec2(4, 0);
		ImGui::SetNextWindowSize(ImVec2(788, 42));
		ImGui::Begin("##tabarea", &bIsGuiVisible, ImVec2(788, 42), 0.f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
		{
			ImGui::PushFont(Global::icons3);
			if (TabSelect == 0) BtnActive(); else BtnNormal();
			if (ImGui::Button("A", ImVec2(128, 42))) TabSelect = 0 ;
			ImGui::SameLine();
			if (TabSelect == 1) BtnActive(); else BtnNormal();
			if (ImGui::Button("B", ImVec2(128, 42))) TabSelect = 1;
			ImGui::SameLine();
			ImGui::PushFont(Global::icons2);
			if (TabSelect == 2) BtnActive(); else BtnNormal();
			if (ImGui::Button("?", ImVec2(128, 42))) TabSelect = 2;
			ImGui::SameLine();
			ImGui::PushFont(Global::icons3);
			if (TabSelect == 3) BtnActive(); else BtnNormal();
			if (ImGui::Button("D", ImVec2(128, 42))) TabSelect = 3;
			ImGui::SameLine();
			if (TabSelect == 4) BtnActive(); else BtnNormal();
			if (ImGui::Button("C", ImVec2(128, 42))) TabSelect = 4;
			ImGui::SameLine();
			if (TabSelect == 5) BtnActive(); else BtnNormal();
			if (ImGui::Button("E", ImVec2(128, 42))) TabSelect = 5;
			// lila

			ImGui::PushFont(Global::fDefault);
		}ImGui::End();


		if (TabSelect == 0)
		{
			if (Global::curLil > 0)
				Global::curLil -= 2;
		}
		else if (TabSelect == 1)
		{
			if (Global::curLil > 132)
				Global::curLil -= 2;
			else if (Global::curLil < 132)
				Global::curLil += 2;
		}
		else if (TabSelect == 2)
		{
			if (Global::curLil > 264)
				Global::curLil -= 2;
			else if (Global::curLil < 264)
				Global::curLil += 2;
		}
		else if (TabSelect == 3)
		{
			if (Global::curLil > 396)
				Global::curLil -= 2;
			else if (Global::curLil < 396)
				Global::curLil += 2;
		}
		else if (TabSelect == 4)
		{
			if (Global::curLil > 528)
				Global::curLil -= 2;
			else if (Global::curLil < 528)
				Global::curLil += 2;
		}
		else if (TabSelect == 5)
		{
			if (Global::curLil > 660)
				Global::curLil -= 2;
			else if (Global::curLil < 660)
				Global::curLil += 2;
		}

		ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 6 + Global::curLil, mainWindowPos.y + 72), ImGuiSetCond_Always);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(1.f, 1.f, 1.f, 0.f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 0.00f);
		style.WindowPadding = ImVec2(0, 0);
		style.ItemSpacing = ImVec2(0, 0);
		ImGui::Begin("##tabarea", &bIsGuiVisible, ImVec2(500, 7), 0.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar);
		{
			// lila

			ImVec4 col_text = ImColor::HSV(140 / 255.f, 20.f / 255.f, 235.f / 255.f);
			ImVec4 col_main = ImColor(9, 82, 128);
			ImVec4 col_back = ImColor(31, 44, 54);
			ImVec4 col_area = ImColor(4, 32, 41);
			//ImVec4(1.0f, 0.63f, 0.5f, 1.00f);
			auto mColor = Settings::Misc::misc_themecolor;
			float themecolor[3];
			themecolor[0] = float(mColor[0]);
			themecolor[1] = float(mColor[1]);
			themecolor[2] = float(mColor[2]);
			style.Colors[ImGuiCol_Button] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);

			ImGui::Button("##ggffghgf", ImVec2(Global::curLil + 128, 6));

		}ImGui::End();

		//background

	}
	else
	{
	if (style.Alpha < 0.f)
		style.Alpha = 0.f;
	else if (style.Alpha != 0.f)
		style.Alpha -= 0.01f;
	}


	ImGui::Render();



	}
}