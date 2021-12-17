#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Features\Features.h"
#include "NewMenu.h"
#include "Features/input.hpp"
Misc     g_Misc;
Hooks    g_Hooks;
Settings g_Settings;
static CFixMove *FixMoveManager = new CFixMove();
Vector AngleReal = Vector(0, 0, 0);
float  AngleRealY = 0.f;
static matrix3x4_t localMatrix[128];
const unsigned short INVALID_STRING_INDEX = (unsigned short)-1;

bool PrecacheModel(const char* szModelName)
{
	INetworkStringTable* m_pModelPrecacheTable = g_pNetworkString->FindTable("modelprecache");

	if (m_pModelPrecacheTable)
	{
		g_pModelInfo->FindOrLoadModel(szModelName);
		int idx = m_pModelPrecacheTable->AddString(false, szModelName);
		if (idx == INVALID_STRING_INDEX)
			return false;
	}
	return true;
}


void PrecacheModels()
{
	PrecacheModel("models/weapons/v_ak47beast.mdl");
	//PrecacheModel("models/weapons/v_minecraft_pickaxe.mdl");
	PrecacheModel("models/weapons/v_cod9_ballista.mdl");
}

void LogEvents()
{	
	static bool convar_performed = false, convar_lastsetting;
	//--- Log Events ---//
	static auto developer = g_pConVar->FindVar("developer");
	developer->SetValue("1");
	static auto con_filter_text_out = g_pConVar->FindVar("con_filter_text_out");
	static auto con_filter_enable = g_pConVar->FindVar("con_filter_enable");
	static auto con_filter_text = g_pConVar->FindVar("con_filter_text");

	con_filter_text->SetValue("    ");
	con_filter_text_out->SetValue("");
	con_filter_enable->SetValue("2");
	convar_performed = true;
}

void EngineCrosshair()
{
	static ConVar* weapon_debug_spread_show = g_pConVar->FindVar("weapon_debug_spread_show");

	if (g_Settings.bEngineCrosshair && g::pLocalEntity && !g::pLocalEntity->IsScoped())
	{
		weapon_debug_spread_show->SetValue("3");
	}
	else
	{
		
		weapon_debug_spread_show->SetValue("0");
	}
}

void NoScopeOverlay()
{
	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!g::pLocalEntity)
		return;

	C_BaseCombatWeapon* pWeapon = g::pLocalEntity->GetActiveWeapon();
	if (!pWeapon)
		return;

	int screenx;
	int screeny;
	g_pEngine->GetScreenSize(screenx, screeny);

	int centerx = screenx / 2;
	int centery = screeny / 2;

	float spread = pWeapon->GetInaccuracy() * 300;
	int height = std::clamp(spread, 1.f, 25.f);
	int alpha = 255 - (height * 7.5f);

	int screenw, screenh;
	g_pEngine->GetScreenSize(screenw, screenh);

	if (g::pLocalEntity->IsScoped())
	{
		if (g_Settings.bSpreadScope)
		{
			g_Render.RectFilled(0, centery - (height / 2), screenx, centery + (height / 2), Color(0, 0, 0, alpha));
			g_Render.RectFilled(centerx + (height / 2), 0, centerx - (height / 2), screeny, Color(0, 0, 0, alpha));

			if (height <= 1.f)
			{
				g_Render.Line(screenw / 2, 0, screenw / 2, screenh, Color::Black());
				g_Render.Line(0, screenh / 2, screenw, screenh / 2, Color::Black());
			}
		}
		else
		{
			g_Render.Line(screenw / 2, 0, screenw / 2, screenh, Color::Black());
			g_Render.Line(0, screenh / 2, screenw, screenh / 2, Color::Black());
		}
	}
}


void watermark(int Width, int Height, INetChannelInfo* nci)
{
	int tickrate = 1.f / g_pGlobalVars->intervalPerTick;
	SIZE sz;
	time_t current = time(0);
	std::string nomezao = (" Sesamhook | ");
	std::string usernaimezao = ("Sesamhook | ");
	std::string fpszao = "fps: " + std::to_string(static_cast<int>(1.f / g_pGlobalVars->frametime)) + " |  ";
	std::string pingzao = "ms: " + std::to_string((int)(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING) * 1000)) + " | ";
	std::string rate = "rt: " + std::to_string(tickrate) + " | ";

	std::string shots = "shots: " + std::to_string(Globals::shots_fired) + " | ";
	std::string shots2 = "missed: " + std::to_string(Globals::missedshots) + " | ";

	std::string infozao = nomezao + usernaimezao + fpszao.c_str() + pingzao.c_str() + rate.c_str() + shots.c_str() + shots2.c_str();

	g_Fonts.pFontRando->GetTextExtent(infozao.c_str(), &sz);

	g_Render.RectFilled(Width - sz.cx - 15, 7, Width - 7, 28, Color(g_Settings.cWatermark, g_Settings.cWatermark[3] * 255));
	g_Render.String(Width - sz.cx - 13, 11, CD3DFONT_DROPSHADOW, Color(255, 255, 255, 180), g_Fonts.pFontRando.get(), infozao.c_str());
	g_Render.Rect(Width - sz.cx - 15, 7, Width - 7, 28, Color(0, 0, 0, 40));
	g_Render.Rect(Width - sz.cx - 16, 6, Width - 6, 29, Color(0, 0, 0, 40));
}

void DrawUselessInfo()
{
	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!g::pLocalEntity)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	int Width;
	int Height;
	g_pEngine->GetScreenSize(Width, Height);

	int centerX = Width / 2;
 	int CenterY = Height / 2;
			
	auto nci = g_pEngine->GetNetChannelInfo(); 


	if (g_Settings.bUselessInfo)
	{
		int fps = 1.f / g_pGlobalVars->frametime;
		int ping = nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING) * 1000;

		if (1.f / g_pGlobalVars->frametime > 1.f / TICKS_TO_TIME(1))
			Globals::playable = true;

		if (g_Settings.iYaw == 2 && g_Settings.bAntiAim)
			g_Render.String(5, CenterY, CD3DFONT_DROPSHADOW | CD3DFONT_FILTERED, Color(159, 202, 43, 255), g_Fonts.pFontName.get(), "AUTO");

		if (GetAsyncKeyState(VK_XBUTTON2))
			g_Render.String(5, CenterY + 25, CD3DFONT_DROPSHADOW | CD3DFONT_FILTERED, Color(159, 202, 43, 255), g_Fonts.pFontName.get(), "BODY");

		Globals::lbybroken = abs(Globals::AngleAA.y - g::pLocalEntity->GetLBY()) > 35 ? true : false;

		if (g_Settings.bAntiAim)
		{
			g_Render.String(5, CenterY - 25, CD3DFONT_DROPSHADOW | CD3DFONT_FILTERED, !Globals::bsendpacket ? Color(159, 202, 43, 255) : Color(255, 0, 0, 255), g_Fonts.pFontName.get(), "LAG");

			g_Render.String(5, CenterY - 50, CD3DFONT_DROPSHADOW | CD3DFONT_FILTERED, Globals::lbybroken ? Color(159, 202, 43, 255) : Color(255, 0, 0, 255), g_Fonts.pFontName.get(), "FAKE");

			if (GetAsyncKeyState(VK_CAPITAL))
				g_Render.String(5, CenterY - 75, CD3DFONT_DROPSHADOW | CD3DFONT_FILTERED, Globals::fakeducked ? Color(159, 202, 43, 255) : Color(255, 0, 0, 255), g_Fonts.pFontName.get(), "DUCK");
		}
	}

	watermark(Width, Height, nci);
}

void Hooks::Init()
{
    // Get window handle
    while (!(g_Hooks.hCSGOWindow = FindWindowA("Valve001", nullptr)))
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(50ms);
    }

    interfaces::Init();                         // Get interfaces
    g_pNetvars = std::make_unique<NetvarTree>();// Get netvars after getting interfaces as we use them

    Utils::Log("Hooking in progress...");

    // D3D Device pointer
    const uintptr_t d3dDevice = **reinterpret_cast<uintptr_t**>(Utils::FindSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

  //  if (g_Hooks.hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
      //  g_Hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WndProc)));

    // VMTHooks
    g_Hooks.pD3DDevice9Hook = std::make_unique<VMTHook>(reinterpret_cast<void*>(d3dDevice));
    g_Hooks.pClientModeHook = std::make_unique<VMTHook>(g_pClientMode);
    g_Hooks.pSurfaceHook	= std::make_unique<VMTHook>(g_pSurface);
	g_Hooks.pRenderViewHook = std::make_unique<VMTHook>(g_pRenderView);
	g_Hooks.pModelRenderHook = std::make_unique<VMTHook>(g_pModelRender);
	g_Hooks.pClientDllHook  = std::make_unique<VMTHook>(g_pClientDll);
	g_Hooks.pPanelHook		= std::make_unique<VMTHook>(g_pPanel);
	//g_Hooks.pClientModeHook = std::make_unique<VMTHook>(sv_cheats);

	InputSys::get().Initialize();

    // Hook the table functions
    g_Hooks.pD3DDevice9Hook->Hook(vtable_indexes::reset,      Hooks::Reset);
    g_Hooks.pD3DDevice9Hook->Hook(vtable_indexes::present,    Hooks::Present);
    g_Hooks.pClientModeHook->Hook(vtable_indexes::createMove, Hooks::CreateMove);
    g_Hooks.pSurfaceHook   ->Hook(vtable_indexes::lockCursor, Hooks::LockCursor);
	g_Hooks.pRenderViewHook->Hook(vtable_indexes::sceneend,   Hooks::SceneEnd);
	g_Hooks.pClientModeHook->Hook(vtable_indexes::postscreenfx, Hooks::DoPostScreenEffects);
	g_Hooks.pClientDllHook->Hook(vtable_indexes::framestage, Hooks::FrameStageNotify);
	g_Hooks.pClientModeHook->Hook(vtable_indexes::overrideview, Hooks::OverrideView);
	g_Hooks.pClientModeHook->Hook(vtable_indexes::fov, Hooks::GGetViewModelFOV);
	g_Hooks.pPanelHook->Hook(vtable_indexes::painttraverse, Hooks::PaintTraverse);
	g_Hooks.pModelRenderHook->Hook(vtable_indexes::dmeidx, Hooks::DrawModelExecute);
	InputSys::get().RegisterHotkey(VK_INSERT, []() {
		g_Settings.bMenuOpened = !g_Settings.bMenuOpened;
		});
    // Create event listener, no need for it now so it will remain commented.
	const std::vector<const char*> vecEventNames = { "bullet_impact", "player_hurt", "round_prestart", "round_freeze_end", "item_purchase" };
    g_Hooks.pEventListener = std::make_unique<EventListener>(vecEventNames);
	//NetvarHook();
    Utils::Log("Hooking completed!");
}

void Hooks::Restore()
{
	ResetWorld();
	Utils::Log("Unhooking in progress...");
    {   // Unhook every function we hooked and restore original one
        g_Hooks.pD3DDevice9Hook->Unhook(vtable_indexes::reset);
        g_Hooks.pD3DDevice9Hook->Unhook(vtable_indexes::present);
        g_Hooks.pClientModeHook->Unhook(vtable_indexes::createMove);
        g_Hooks.pSurfaceHook->Unhook(vtable_indexes::lockCursor);
		g_Hooks.pRenderViewHook->Unhook(vtable_indexes::sceneend);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::postscreenfx);
		g_Hooks.pClientDllHook->Unhook(vtable_indexes::framestage);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::overrideview);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::fov);
		g_Hooks.pPanelHook->Unhook(vtable_indexes::painttraverse);
		//g_Hooks.pClientModeHook->Unhook(vtable_indexes::getbool);
		//g_Hooks.pClientModeHook->Unhook(vtable_indexes::datagram);
		g_Hooks.pModelRenderHook->Unhook(vtable_indexes::dmeidx);
        //SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_Hooks.pOriginalWNDProc));
        g_pNetvars.reset();   /* Need to reset by-hand, global pointer so doesnt go out-of-scope */
    }
    Utils::Log("Unhooking succeded!");

	
    // Destroy fonts and all textures we created
    g_Render.InvalidateDeviceObjects();
    g_Fonts.DeleteDeviceObjects();
}

bool IsKnife(C_BaseCombatWeapon* pWeapon)
{
	if (pWeapon == nullptr) return false;
	int id = pWeapon->sGetItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_KNIFE_BAYONET, WEAPON_KNIFE_SURVIVAL_BOWIE, WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GUT, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_PUSH, WEAPON_KNIFE_TACTICAL , WEAPON_KNIFE, WEAPON_KNIFE_T };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void knifeleftside(C_BaseEntity* local)
{
	C_BaseCombatWeapon* pWeapon = local->GetActiveWeapon();
	if (!pWeapon)
		return;

	static ConVar* cl_righthand = g_pConVar->FindVar("cl_righthand");

	if(IsKnife(pWeapon))
		cl_righthand->SetValue("0");
	else
		cl_righthand->SetValue("1");
}

bool IsBallisticWeapon6(C_BaseEntity* local)
{
	C_BaseCombatWeapon* pWeapon = local->GetActiveWeapon();
	if (!pWeapon)
		return false;
	ClientClass* pWeaponClass = pWeapon->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)EClassIds::CKnife || pWeaponClass->m_ClassID == (int)EClassIds::CHEGrenade ||
		pWeaponClass->m_ClassID == (int)EClassIds::CDecoyGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CIncendiaryGrenade ||
		pWeaponClass->m_ClassID == (int)EClassIds::CSmokeGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CC4 ||
		pWeaponClass->m_ClassID == (int)EClassIds::CMolotovGrenade || pWeaponClass->m_ClassID == (int)EClassIds::CFlashbang)
		return false;
	else
		return true;
}

static auto smokecount = *(int**)(Utils::FindSignature("client.dll", "8B 1D ? ? ? ? 56 33 F6 57 85 DB") + 0x2);

auto clantag = [](const char * name) -> void {
	using Fn = int(__fastcall *)(const char *, const char *);
	static auto fn = reinterpret_cast<Fn>(Utils::FindSignature("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	fn(name, name);
};

void UpdateAngleAA(CUserCmd* pCmd)
{
	Globals::AngleAA = pCmd->viewangles;
}

void Marquee(std::string& clantag)
{
	std::string temp = clantag;
	clantag.erase(0, 1);
	clantag += temp[0];
}

void ClantagCM()
{
	static std::string text = " Sesamhook ";

	static float LastChangeTime = 0.f;
	if (g_pGlobalVars->curtime - LastChangeTime < .45f)
	{
		return;
	}

	LastChangeTime = g_pGlobalVars->curtime;
	Marquee(text);

	clantag(text.data());
}

void SlideWalk(CUserCmd* pCmd)
{
	if (!g_Settings.bSlideWalk)
		return;

	if (pCmd->forwardmove > 0)
	{
		pCmd->buttons |= IN_BACK;
		pCmd->buttons &= ~IN_FORWARD;
	}

	if (pCmd->forwardmove < 0)
	{
		pCmd->buttons |= IN_FORWARD;
		pCmd->buttons &= ~IN_BACK;
	}

	if (pCmd->sidemove < 0)
	{
		pCmd->buttons |= IN_MOVERIGHT;
		pCmd->buttons &= ~IN_MOVELEFT;
	}

	if (pCmd->sidemove > 0)
	{
		pCmd->buttons |= IN_MOVELEFT;
		pCmd->buttons &= ~IN_MOVERIGHT;
	}
}

void FixWalk(CUserCmd* pCmd)
{
	if (!g_Settings.bFixLegMovement)
		return;

	if (pCmd->buttons & IN_FORWARD)
	{
		pCmd->forwardmove = 450;
		pCmd->buttons &= ~IN_FORWARD;
	}
	else if (pCmd->buttons & IN_BACK)
	{
		pCmd->forwardmove = -450;
		pCmd->buttons &= ~IN_BACK;
	}
	if (pCmd->buttons & IN_MOVELEFT)
	{
		pCmd->sidemove = -450;
		pCmd->buttons &= ~IN_MOVELEFT;
	}
	else if (pCmd->buttons & IN_MOVERIGHT)
	{
		pCmd->sidemove = 450;
		pCmd->buttons &= ~IN_MOVERIGHT;
	}
}

void AutomaticWeapons(CUserCmd* pCmd)
{
	C_BaseCombatWeapon* pWeapon = g::pLocalEntity->GetActiveWeapon();
	if (!pWeapon)
		return;

	if (pWeapon->GetAmmo() == 0)
		return;

	static bool WasFiring = false;
	WeaponInfo_t* WeaponInfo = pWeapon->GetCSWpnData();
	if (!WeaponInfo->full_auto)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}
}

void AutoRevolver(CUserCmd* cmd, C_BaseEntity* local_player)
{
	if (!g_Settings.bAutoRevolver)
		return;

	C_BaseCombatWeapon* weapon = local_player->GetActiveWeapon();
	if (!weapon || weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
		return;

	static float delay = 0.f;
	if (delay < 0.15f)
	{
		delay += g_pGlobalVars->intervalPerTick;
		cmd->buttons |= IN_ATTACK;
	}
	else
		delay = 0.f;
}

bool __fastcall Hooks::CreateMove(IClientMode* thisptr, void* edx, float sample_frametime, CUserCmd* pCmd)
{
	// Call original createmove before we start screwing with it
	//static auto oCreateMove = g_Hooks.pClientModeHook->GetOriginal<CreateMove_t>(24);
	//oCreateMove(thisptr, edx, sample_frametime, pCmd);

	if (!pCmd || !pCmd->command_number)
		return true;

	// Get globals
	g::pCmd = pCmd;
	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!g::pLocalEntity)
		return true;

	uintptr_t *framePtr;
	__asm mov framePtr, ebp;

	Globals::bsendpacket = true;

	if(g_Settings.bClantag)
		ClantagCM();

	if (g::pLocalEntity->IsAlive())
	{
		pCmd->buttons |= IN_BULLRUSH;

		g_Misc.OnCreateMove();

		if (g::pLocalEntity->GetFlags() & FL_ONGROUND && pCmd->buttons & IN_SPEED && g_Settings.bSlowWalk)
			aimbot->minspeed(g::pLocalEntity, pCmd);

		FixMoveManager->Start(pCmd);

		FixWalk(pCmd);

		KnifeBot::Run(g::pLocalEntity, pCmd);

	//	engine_prediction::RunEnginePred();

		if (g_Settings.bAutomaticWeapons && IsBallisticWeapon6(g::pLocalEntity))
			AutomaticWeapons(pCmd);


		aimbot->OnCreateMove();

	//	if (g_Settings.bAutoStop && Globals::shouldstop)
		//	aimbot->stopmovement(g::pLocalEntity, pCmd);

		fakelag->fakeduck2(pCmd, g::pLocalEntity);
		fakelag->do_fakelag(pCmd, g::pLocalEntity);
		antiaim->Do(pCmd);

	//	engine_prediction::EndEnginePred();

		FixMoveManager->Stop(pCmd);

		SlideWalk(pCmd);

		Globals::missedshots = Globals::shots_fired - Globals::shots_hit;
		if (Globals::shots_fired > 30)
			Globals::shots_fired = 0;
		if (Globals::shots_hit > 30)
			Globals::shots_hit = 0;
		if (Globals::missedshots > 30 || Globals::missedshots < 0)
			Globals::missedshots = 0;
		
		
		//g::pLocalEntity->SetupBones(localMatrix, 128, 0x100, g_pGlobalVars->curtime);
	}

	if(g_Settings.AntiUT)
		pCmd->viewangles = Utils::NormalizeAngle(pCmd->viewangles);

	*(bool*)(*framePtr - 0x1C) = Globals::bsendpacket;

	UpdateAngleAA(pCmd);

	pCmd = g::pCmd;

    return false;
}


void __fastcall Hooks::LockCursor(ISurface* thisptr, void* edx)
{
    static auto oLockCursor = g_Hooks.pSurfaceHook->GetOriginal<LockCursor_t>(vtable_indexes::lockCursor);

    if (!g_Settings.bMenuOpened)
        return oLockCursor(thisptr, edx);

    g_pSurface->UnlockCursor();
}


HRESULT __stdcall Hooks::Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    static auto oReset = g_Hooks.pD3DDevice9Hook->GetOriginal<Reset_t>(16);

    if (g_Hooks.bInitializedDrawManager)
    {
        Utils::Log("Reseting draw manager.");
        g_Render.InvalidateDeviceObjects();

		newmenu->invalidate_objects();
        HRESULT hr = oReset(pDevice, pPresentationParameters);
		newmenu->create_objects(pDevice);
        g_Render.RestoreDeviceObjects(pDevice);
        Utils::Log("DrawManager reset succeded.");
        return hr;
    }	

    return oReset(pDevice, pPresentationParameters);
}


HRESULT __stdcall Hooks::Present(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, 
                                 HWND hDestWindowOverride,  const RGNDATA* pDirtyRegion)
{
	static auto oPresent = g_Hooks.pD3DDevice9Hook->GetOriginal<Present_t>(17);

	IDirect3DStateBlock9* stateBlock = nullptr;
	IDirect3DVertexDeclaration9* vertDec = nullptr;

	pDevice->GetVertexDeclaration(&vertDec);
    pDevice->CreateStateBlock(D3DSBT_ALL, &stateBlock);
	stateBlock->Capture();
	newmenu->pre_render(pDevice);

    [pDevice]()
    {
        if (!g_Hooks.bInitializedDrawManager)
        {
            Utils::Log("Initializing Draw manager");
            g_Render.InitDeviceObjects(pDevice);
            //g_Hooks.nMenu.Initialize();
            g_Hooks.bInitializedDrawManager = true;
			newmenu->apply_fonts();
			newmenu->setup_resent(pDevice);
            Utils::Log("Draw manager initialized");
        }
        else
        {
            g_Render.SetupRenderStates(); // Sets up proper render states for our state block
			

			if (g_Settings.bScopeNoZoom)
				NoScopeOverlay();

            g_ESP.Render();

			EngineCrosshair();

			if(g_Settings.bDamageLogs)
				LogEvents();
	

			//clantag("otario.cc");
			DrawUselessInfo();

			DoNightMode();

			DoSkyBox();

			DoAsusWalls();

			if(g_Settings.bHitmaker)
				g_ESP.DrawHitmarker(); 	

			//c_menu::get().pre_render(pDevice);
			newmenu->post_render();

			newmenu->run_popup();
			newmenu->run();
			newmenu->end_present(pDevice);

			//antario gui

			/*if (g_Settings.bMenuOpened)
			{
				g_Hooks.nMenu.Render();             // Render our menu
				g_Hooks.nMenu.mouseCursor->Render();// Render mouse cursor in the end so its not overlapped
			}
			*/
        }
    }();

    stateBlock->Apply();
    stateBlock->Release();
	pDevice->SetVertexDeclaration(vertDec);
    
    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

LRESULT Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // for now as a lambda, to be transfered somewhere
    // Thanks uc/WasserEsser for pointing out my mistake!
    // Working when you HOLD th button, not when you press it.
    const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
    {
		if (wParam != vKey) return;

        if (uMsg == WM_KEYDOWN)
            bButton = true;
        else if (uMsg == WM_KEYUP)
            bButton = false;
    };

	const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
	{
		if (wParam != vKey) return;

		if (uMsg == WM_KEYUP)
			bButton = !bButton;
	};

	getButtonToggle(g_Settings.bMenuOpened, VK_INSERT);

    if (g_Hooks.bInitializedDrawManager)
    {
        // our wndproc capture fn
        if (g_Settings.bMenuOpened && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
        {
           // g_Hooks.nMenu.MsgProc(uMsg, wParam, lParam);
            return true;
        }
    }

    // Call original wndproc to make game use input again
    return CallWindowProcA(g_Hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam);
}

void __fastcall Hooks::SceneEnd(void* ecx, void* edx)
{
	static auto oSceneEnd = g_Hooks.pRenderViewHook->GetOriginal<SceneEnd_t>(9);
	oSceneEnd(ecx);

	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!g::pLocalEntity)
		return;

	float flColor[3] = { 0.f };
	float flColor2[3] = { 0.f };
	float flColor3[3] = { 0.f };

	flColor[0] = 60.f / 255.f;
	flColor[1] = 120.f / 255.f;
	flColor[2] = 180.f / 255.f;

	flColor2[0] = 150.f / 255.f;
	flColor2[1] = 200.f / 255.f;
	flColor2[2] = 60.f / 255.f;

	flColor3[0] = 255.f / 255.f;
	flColor3[1] = 255.f / 255.f;
	flColor3[2] = 255.f / 255.f;

	Color LocalColor = Color(g_Settings.chamshidden, g_Settings.chamshidden[3] * 255);
	Color EnemyColor = Color(g_Settings.chamshidden, g_Settings.chamshidden[3] * 255);

	Color LocalColorXqz = Color(g_Settings.chamsvisible, g_Settings.chamsvisible[3] * 255);
	Color EnemyColorXqz = Color(g_Settings.chamsvisible, g_Settings.chamsvisible[3]* 255);

	if (!g::pLocalEntity || !g_pEngine->IsInGame())
		return;

	if (!g_pEngine->IsInGame() && !g_pEngine->IsConnected())
		return;

	for (int it = 1; it <= g_pEngine->GetMaxClients(); ++it)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(it);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
			continue;

		bool IsLocal = pPlayerEntity == g::pLocalEntity;
		bool IsTeam = pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam();

		bool normal = false;
		bool flat = false;
		bool wireframe = false;
		bool glass = false;
		bool metallic = false;
		bool xqz = false;
		bool flat_xqz = false;
		bool wireframe_xqz = false;
		bool onetapsu = false;

		int mode = IsLocal ? g_Settings.iLocalChamsMode : g_Settings.iChamsMode;

		if (IsLocal && !g_Settings.bShowLocalChams)
		{
			continue;
		}
		if ((IsTeam && !IsLocal) && !g_Settings.bShowTeammates)
		{
			continue;
		}
		if (!IsTeam && !g_Settings.bShowChams)
		{
			continue;
		}

		Color clr = IsLocal ? LocalColor : EnemyColor;
		Color clr2 = IsLocal ? LocalColorXqz : EnemyColorXqz;


		switch (mode)
		{
		case 1:
			xqz = true;
			break;
		case 2:
			flat_xqz = true;
			break;
		case 3:
			wireframe_xqz = true;
			break;
		}

		switch (mode)
		{
		case 1:
			normal = true;
			break;
		case 2:
			flat = true;
			break;
		case 3:
			wireframe = true;
			break;
		}

		MaterialManager::get().OverrideMaterial(xqz || flat_xqz || wireframe_xqz, flat, wireframe, glass, metallic, onetapsu);
		g_pRenderView->SetColorModulation(clr2.red / 255.f, clr2.green / 255.f, clr2.blue / 255.f);
		g_pRenderView->SetBlend(clr2.alpha / 255.f);
		pPlayerEntity->GetClientRenderable()->DrawModel(0x1, 255);
		if (xqz || flat_xqz || wireframe_xqz)
		{
			MaterialManager::get().OverrideMaterial(false, flat, wireframe, glass, metallic, onetapsu);
			g_pRenderView->SetColorModulation(clr.red / 255.f, clr.green / 255.f, clr.blue / 255.f);
			g_pRenderView->SetBlend(clr.alpha / 255.f);
			pPlayerEntity->GetClientRenderable()->DrawModel(0x1, 255);
		}
		g_pModelRender->ForcedMaterialOverride(nullptr);
	}
	g_pModelRender->ForcedMaterialOverride(nullptr);

}

int __fastcall Hooks::DoPostScreenEffects(void * ecx, void * edx, int a1)
{
	static auto oDoPostScreenEffects = g_Hooks.pClientModeHook->GetOriginal<DoPostScreenEffects_t>(44);

	if (g_Settings.bShowGlow)
		DoGlow->DrawGlow();

	return oDoPostScreenEffects(ecx, edx, a1);
}

bool ReallocatedDeathNoticeHUD = false;

void xyoResolver(C_BaseEntity* pEnt)
{
	if (pEnt->GetTeam() == g::pLocalEntity->GetTeam())
		return;

	const auto player_animation_state = pEnt->GetAnimState();

	if (!player_animation_state)
		return;

	float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

	auto v48 = 0.f;

	if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
	{
		v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
	}
	else
	{
		v48 = 0.0f;
	}

	float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

	float flYawModifier = v49 + 1.0;

	if (player_animation_state->m_fDuckAmount > 0.0)
	{
		float v53 = 0.0f;

		if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
		{
			v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
		}
		else
		{
			v53 = 0.0f;
		}
	}

	float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
	float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

	float newFeetYaw = 0.f;

	auto eyeYaw = player_animation_state->m_flEyeYaw;

	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}

	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	player_animation_state->m_flGoalFeetYaw = v136;

}

bool C_BaseEntity::IsKnifeorNade()
{
	if (!this)
		return false;
	if (!this->IsAlive())
		return false;

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_knife")
		return true;
	else if (WeaponName == "weapon_incgrenade")
		return true;
	else if (WeaponName == "weapon_decoy")
		return true;
	else if (WeaponName == "weapon_flashbang")
		return true;
	else if (WeaponName == "weapon_hegrenade")
		return true;
	else if (WeaponName == "weapon_smokegrenade")
		return true;
	else if (WeaponName == "weapon_molotov")
		return true;

	return false;
}

void DoAnimFix() 
{
	auto OldCurtime = g_pGlobalVars->curtime;
	auto OldFrametime = g_pGlobalVars->framecount;
	auto OldFraction = g::pLocalEntity->GetAnimState()->m_flUnknownFraction = 0;

	g_pGlobalVars->curtime = g::pLocalEntity->m_flSimulationTime();
	g_pGlobalVars->framecount = g_pGlobalVars->intervalPerTick;

	CAnimationLayer Layers[15];
	std::memcpy(Layers, g::pLocalEntity->AnimOverlays(), (sizeof(CAnimationLayer) * 15));

	if (g::pLocalEntity->GetAnimState())
		g::pLocalEntity->GetAnimState()->m_iLastClientSideAnimationUpdateFramecount = g_pGlobalVars->framecount - 1;

	g::pLocalEntity->ClientSideAnimation() = true;
	g::pLocalEntity->UpdateClientSideAnimation();
	g::pLocalEntity->ClientSideAnimation() = false;

	//end
	std::memcpy(g::pLocalEntity->AnimOverlays(), Layers, (sizeof(CAnimationLayer) * 15));

	g_pGlobalVars->curtime = OldCurtime;
	g_pGlobalVars->framecount = OldFrametime;

	g::pLocalEntity->GetAnimState()->m_flUnknownFraction = OldFraction;

	g::pLocalEntity->SetAbsAngles(QAngle(0, g::pLocalEntity->GetAnimState()->m_flGoalFeetYaw, 0));
}

inline float NormalizePitch(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}

void __fastcall Hooks::FrameStageNotify(void* ecx, void* edx, int stage)
{
	static auto oFrameStageNotify = g_Hooks.pClientDllHook->GetOriginal<FrameStageNotify_t>(37);

	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	
	if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
	{
		switch (stage)
		{
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:

			if (g::pLocalEntity->IsAlive())
			{
				skin_changer->KnifeChanger();
				//skin_changer->ChangeSkins();

				if (g_Settings.bNoFlash)
					g::pLocalEntity->SetFlashDuration(0);
			}

			for (int i = 0; i < 64; ++i)
			{
				auto Entity = g_pEntityList->GetClientEntity(i);
				if (!Entity || !g::pLocalEntity || !Entity->IsAlive() || Entity->IsDormant() || Entity->GetTeam() == g::pLocalEntity->GetTeam() || Entity == g::pLocalEntity)
					continue;

				if (g_Settings.bResolver)
				{
					static float yaw = Entity->GetEyeAnglesPointer2()->y;
					int missedshot = Globals::missedshots;

					static float yaw_2 = 0;
					switch (Globals::missedshots % 5)
					{
					case 0:
						xyoResolver(Entity);
						break;
					case 1:
						Entity->GetEyeAnglesPointer2()->y = yaw - 30;
						break;
					case 2:
						Entity->GetEyeAnglesPointer2()->y = Entity->GetLBY() - 58;
						break;
					case 3:
						Entity->GetEyeAnglesPointer2()->y = yaw + 30;
						break;
					case 4:
						Entity->GetEyeAnglesPointer2()->y = Entity->GetLBY() + 58;
						break;
					}


					Entity->GetEyeAnglesPointer2()->x = NormalizePitch(Entity->GetEyeAnglesPointer2()->x);
					//Entity->GetEyeAnglesPointer2()->y = Utils::NormalizeYaw(yaw_2);

					//xyoResolver(Entity);


				}

			}

			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		{
			
		}
			break;
		case FRAME_NET_UPDATE_END:
			if (g_Settings.bNoSmoke)
			{
				std::vector<const char*> vistasmoke_mats =
				{
					"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
				};

				for (auto matName : vistasmoke_mats)
				{
					static IMaterial* mat = g_pMaterialSystem->FindMaterial(matName, "Other textures");
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}
				*smokecount = 0;
			}
			break;
		case FRAME_RENDER_START:
			if (g::pLocalEntity && g::pLocalEntity->IsAlive())
			{/*
				if (g::pLocalEntity->GetAnimState()->m_bInHitGroundAnimation)
					if (g::pLocalEntity->GetAnimState()->m_flHeadHeightOrOffsetFromHittingGroundAnimation)
						Globals::AngleAA.x = 0;*/

				if (*(bool*)((DWORD)g_pInput + 0xAD))
				{
					g_pPrediction->SetLocalViewAngles(Globals::AngleAA);
				}

				if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->buttons & IN_SPEED && g_Settings.bSlowWalk)
					g::pLocalEntity->GetAnimState()->m_flUnknownFraction = 0.f;

				DoAnimFix();
			}
		
			for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
			{
				if (i == g_pEngine->GetLocalPlayer()) continue;

				C_BaseEntity * pCurEntity = g_pEntityList->GetClientEntity(i);
				if (!pCurEntity) continue;

				*(int*)((uintptr_t)pCurEntity + 0xA30) = g_pGlobalVars->framecount; //we'll skip occlusion checks now
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;//clear occlusion flags
			}

			break;
		}
	}
	aimbot->DisableInterpolation((ClientFrameStage_t)stage);
	oFrameStageNotify(ecx, stage);
}

void ThirdpersonOV(C_BaseEntity* local)
{
	bool enable = GetKeyState(g_Settings.iThirdPersonKey);

	/* return if local is nullptr */
	if (g::pLocalEntity == nullptr)
		return;

	/* check if we are connected */
	if (!g_pEngine->IsConnected() && !g_pEngine->IsInGame())
		return;

	/* vec angles */
	static Vector vecAngles;

	/* pointer to localplayer */
	C_BaseEntity* localplayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	/* get view angles */
	g_pEngine->GetViewAngles(vecAngles);

	if (g_Settings.bThirdPersonDead && !g::pLocalEntity->IsAlive() && g::pLocalEntity->GetObserverMode() == 4)
	{
		g::pLocalEntity->SetObserverMode(5);
	}

	/* if we have clicked the key */
	if (g_Settings.bThirdPersonAlive && enable && g_Settings.iThirdPersonDistance > 0 && local->IsAlive())
	{
		/* if we are not in thirdperson */
		if (!g_pInput->m_fCameraInThirdPerson)
		{
			/* getting correct distance */
			auto GetCorrectDistance = [&localplayer](float ideal_distance) -> float
			{
				/* vector for the inverse angles */
				Vector inverseAngles;
				g_pEngine->GetViewAngles(inverseAngles);

				/* inverse angles by 180 */
				inverseAngles.x *= -1.f, inverseAngles.y += 180.f;

				/* vector for direction */
				Vector direction;
				Utils::AngleVectors(inverseAngles, &direction);

				/* ray, trace & filters */
				Ray_t ray;
				trace_t trace;
				CTraceFilter filter;

				/* dont trace local player */
				filter.pSkip = g::pLocalEntity;

				/* create ray */
				ray.Init(localplayer->GetVecOrigin() + localplayer->GetViewOffset(), (localplayer->GetVecOrigin() + localplayer->GetViewOffset()) + (direction * ideal_distance));

				/* trace ray */
				g_pTrace->TraceRay(ray, MASK_SHOT, &filter, &trace);

				/* return the ideal distance */
				return (ideal_distance * trace.fraction) - 10.f;
			};

			/* change the distance from player to camera */
			vecAngles.z = GetCorrectDistance(120);

			/* make player thirdperson */
			g_pInput->m_fCameraInThirdPerson = true;

			/* set camera view */
			g_pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, vecAngles.z);
		}
	}
	else
	{
		/* set player to firstperson */
		g_pInput->m_fCameraInThirdPerson = false;

		/* return to default view */
		g_pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
	}
}

void __fastcall Hooks::OverrideView(void* thisptr, void* edx, CViewSetup* setup)
{
	static auto oOverrideView = g_Hooks.pClientModeHook->GetOriginal<OverrideView_t>(18);

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (local_player && local_player->IsAlive())
	{
		if (g_Settings.bNoVisRecoil) {
			Vector viewPunch = g::pLocalEntity->GetViewPunchAngle();
			Vector aimPunch = g::pLocalEntity->GetAimPunchAngle();

			setup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
			setup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
			setup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		}
	}

	bool scoped = g::pLocalEntity && g::pLocalEntity->IsScoped();

	if (!scoped || (scoped && g_Settings.bNoZoom))
		setup->fov = g_Settings.flWorldFov == 0 ? 90 : g_Settings.flWorldFov;

	ThirdpersonOV(local_player);

	if (Globals::fakeducked && g::pLocalEntity && g::pLocalEntity->IsAlive())
		setup->origin.z = g::pLocalEntity->GetAbsOrigin().z + 64.f;

	oOverrideView(thisptr, setup);
}

float __stdcall Hooks::GGetViewModelFOV()
{
	float fov = g_Hooks.pClientModeHook->GetOriginal<oGetViewModelFOV>(35)();

	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (g::pLocalEntity)
		{
			fov = g_Settings.flViewModelFov == 0 ? 68 : g_Settings.flViewModelFov;
		}
	}
	return fov;
}

void __fastcall Hooks::DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
{
	static auto oDrawModelExecute = g_Hooks.pModelRenderHook->GetOriginal<DrawModelExecute_t>(21);

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	auto entity = g_pEntityList->GetClientEntity(render_info.entity_index);
	const char* ModelName = g_pModelInfo->GetModelName((model_t*)render_info.pModel);
	/// localplayer
	if (entity == local_player)
	{
		if (local_player && local_player->IsScoped())
			g_pRenderView->SetBlend(g_Settings.iScopedBlend * 0.01f);

#ifdef Debug
		oDrawModelExecute(ecx, context, state, render_info, localMatrix);
#endif
	}
	//else if (entity != nullptr && entity->IsAlive() && !entity->IsDormant() && entity->GetTeam() != g::pLocalEntity->GetTeam() && strstr(ModelName, "models/player")) {
	//	g_pRenderView->SetBlend(g_Settings.iEntBlend * 0.01f);
	//	//oDrawModelExecute(ecx, context, state, render_info, matrix);
	//}
	//
		oDrawModelExecute(ecx, context, state, render_info, matrix);
}

void __stdcall Hooks::PaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
{
	static auto oPaintTraverse = g_Hooks.pPanelHook->GetOriginal<PaintTraverse_t>(41);

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	std::string panel_name = g_pPanel->GetName(VGUIPanel);
/*	if (panel_name == "FocusOverlayPanel")
	{
		//LogEvents();
	}
	*/
	// no scope overlay
	if (panel_name == "HudZoom" && g_Settings.bScopeNoZoom)
		return;

	oPaintTraverse(g_pPanel, VGUIPanel, ForceRepaint, AllowForce);

	static auto cv = g_pConVar->FindVar("mat_postprocess_enable");
	cv->fnChangeCallback = 0;
	cv->SetValue(!g_Settings.bNoPostProcess);
}