#include "stdafx.h"
#include "Tools/Menu/Menu.h"
#include "Tools/Hacks/Misc.h"
#include "Tools/Utils/Playerlist.h"
#include "Tools/Hacks/Esp.h"
#include "Tools/Hacks/Antiaim.h"
#include "Tools/Hacks/Aimbot.h"
#include "Tools/Hacks/Legit.h"
#include "Tools/Menu/SettingsManager.h"
#include "Tools/Hacks/UI/UI.h"
#include "Tools/Hacks/KnifeBot.h"
#include "newgui.h"
#include "SDK/SDK Headers/CPred.h"
#define DEV

vfunc_hook hlclient_hook;
vfunc_hook direct3d_hook;
vfunc_hook vguipanel_hook;
vfunc_hook vguisurf_hook;
vfunc_hook mdlrender_hook;
vfunc_hook clientmode_hook;
vfunc_hook renderview_hook;
vfunc_hook svcheats_hook;

std::vector<int> CPlayerList::friends = {};
std::vector<int> CPlayerList::Players = {};

#define PI_2 PI/2
#define PI_4 PI/4
CHackManager Hacks;

std::vector<trace_info> trace_logs;
class Hitbox;

static int iShoots;

std::vector< int > AutoStraferConstants = {};
std::vector< const char* > smoke_materials =
{
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
};


typedef void(*Fucntion)(IGameEvent* event);

void Hurtss(IGameEvent* event)
{
	int iAttacker = Interfaces.pEngine->GetPlayerForUserID(event->GetInt("attacker"));
	static int iVictim = Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid"));
	if (iAttacker == Interfaces.pEngine->GetLocalPlayer() && iVictim != Interfaces.pEngine->GetLocalPlayer())
	{
		Esp::SetHitmarkerValues(Interfaces.pGlobalVars->curtime, event->GetInt("dmg_health"), iVictim);

		if (Vars.Visuals.Other.Hitmarker)
		{
			Interfaces.pSurface->PlaySoundS("buttons\\arena_switch_press_02.wav");
		}
	}
}

void Impactss(IGameEvent* event)
{
	if (Vars.Visuals.Other.BulletTracers)
	{
		auto* index = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetPlayerForUserID(event->GetInt("userid")));

		if (Hacks.LocalPlayer)
		{
			Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

			if (index)
				trace_logs.push_back(trace_info(index->GetEyePosition(), position, Interfaces.pGlobalVars->curtime, event->GetInt("userid")));
		}
	}
}

class CGameEventListener : public IGameEventListener2
{
private:
	std::string eventName;
	Fucntion Call;
	bool server;
public:
	CGameEventListener() : eventName(""), Call(nullptr), server(false)
	{
	}

	CGameEventListener(const std::string& gameEventName, const Fucntion Func, bool Server) : eventName(gameEventName), Call(Func), server(Server)
	{
		this->RegListener();
	}

	virtual void FireGameEvent(IGameEvent* event)
	{
		this->Call(event);
	}

	virtual int IndicateEventHandling(void)
	{
		return 0x2A;
	}
	 
	void RegListener(void)
	{
		static bool first = true;
		if (!first)
			return;
		if (Interfaces.g_GameEventMgr->AddListener(this, this->eventName.c_str(), server))
			Hacks.listeners.push_back(this);
	}
};

#define REG_EVENT_LISTENER(p, e, n, s) p = new CGameEventListener(n, e, s)
CGameEventListener* Hurt;
CGameEventListener* Impacts;

void Init()
{
	REG_EVENT_LISTENER(Hurt, &Hurtss, "player_hurt", false);
	REG_EVENT_LISTENER(Impacts, &Impactss, "bullet_impact", false);
}

#include <intrin.h>

static auto CAM_THINK = Utils.FindPattern("client_panorama.dll", "85 C0 75 30 38 86");

bool __fastcall Hooked_GetBool(void* pConVar, void* edx)
{
	auto oGetBool = svcheats_hook.get_original<SvCheatsGetBoolFn>(13);

	if ((uintptr_t)_ReturnAddress() == CAM_THINK)
		return true;

	return oGetBool(pConVar);
}

void __stdcall Hooked_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	auto oPaintTraverse = vguipanel_hook.get_original<iPaintTraverse>(41);

	if (strstr(Interfaces.pPanel->GetName(vguiPanel), ("HudZoom")) && Vars.Visuals.Effects.NoScope)
		return;

	oPaintTraverse(Interfaces.pPanel, vguiPanel, forceRepaint, allowForce);

	const char* pszPanelName = Interfaces.pPanel->GetName(vguiPanel);

	if (!strstr(pszPanelName, "FocusOverlayPanel"))
		return;

	if (Interfaces.pEngine->IsConnected() && Interfaces.pEngine->IsInGame()) 
	{
		/*static bool first = false;
		if (!first)
		{
			ConVar* svcheats = Interfaces.g_ICVars->FindVar("sv_cheats");
			SpoofedConvar* svcheatsspoof = new SpoofedConvar(svcheats);
			svcheatsspoof->SetInt(1);
			first = true;
		}*/

		if (Hacks.LocalPlayer) {
			Misc::DrawScope();
			Esp::DoEsp();
			Esp::NightMode();
		}

		static bool* s_bOverridePostProcessingDisable = *(bool**)(Utils.FindPattern("client_panorama.dll", "80 3D ? ? ? ? ? 53 56 57 0F 85") + 0x2);
		*s_bOverridePostProcessingDisable = Vars.Visuals.Effects.NoPostProcessing;

		Interfaces.pSurface->DrawT(0, 0, CColor(0, 0, 0, 2), Hacks.Font_Controls, false, ".");
	}
}

void OpenMenu()
{
	static bool is_down = false;
	static bool is_clicked = false;

	if (GetAsyncKeyState(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GetAsyncKeyState(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked)
	{
		Vars.Menu.Opened = !Vars.Menu.Opened;
	}
}

void GUI_Init(IDirect3DDevice9* pDevice)
{
	ImGui_ImplDX9_Init(INIT::Window, pDevice);

	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.50);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.50);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.50);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
	colors[ImGuiCol_CloseButton] = ImVec4(0.44f, 0.44f, 0.44f, 0);
	colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 0);
	colors[ImGuiCol_CloseButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 0);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 1.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(7, 8);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabMinSize = 1.0f;
	style.GrabRounding = 1.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	Vars.Menu.Init = true;
}


long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	static auto oEndScene = direct3d_hook.get_original<EndSceneFn>(42);

	D3DCOLOR rectColor = D3DCOLOR_XRGB(255, 0, 0);
	D3DRECT BarRect = { 1, 1, 1, 1 };

	pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, rectColor, 0, 0);

	if (!Vars.Menu.Init)
		GUI_Init(pDevice);

	ImGui::GetIO().MouseDrawCursor = Vars.Menu.Opened;

	ImGui_ImplDX9_NewFrame();

	POINT mp;

	GetCursorPos(&mp);

	ImGuiIO& io = ImGui::GetIO();

	io.MousePos.x = mp.x;
	io.MousePos.y = mp.y;

	if (Vars.Menu.Opened)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 500));

		if (ImGui::Begin("OneWay.cc [Closed Beta Test]", &Vars.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			drawMenu();
		}
		ImGui::End();
		
	}
	ImGui::Render();

	return oEndScene(pDevice);
}



long __stdcall Hooked_EndScene_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	auto oEndSceneReset = direct3d_hook.get_original<EndSceneResetFn>(16);

	if (!Vars.Menu.Init)
		return oEndSceneReset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = oEndSceneReset(pDevice, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

class Prediction
{
private:
	float m_flOldCurtime;
	float m_flOldFrametime;
	CMoveData m_MoveData;

	int* m_pPredictionRandomSeed;
public:
	void Start()
	{
		static int flTickBase;

		if (Hacks.CurrentCmd)
		{
			if (Hacks.CurrentCmd->hasbeenpredicted)
				flTickBase = Hacks.LocalPlayer->GetTickBase();
			else
				++flTickBase;
		}

		static bool bInit = false;
		if (!bInit) {
			m_pPredictionRandomSeed = *(int**)(Utils.FindPattern("client_panorama.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
			bInit = true;
		}

		*m_pPredictionRandomSeed = MD5_PseudoRandom(Hacks.CurrentCmd->command_number) & 0x7FFFFFFF;

		m_flOldCurtime = Interfaces.pGlobalVars->curtime;
		m_flOldFrametime = Interfaces.pGlobalVars->frametime;

		Interfaces.pGlobalVars->curtime = flTickBase * Interfaces.pGlobalVars->interval_per_tick;
		Interfaces.pGlobalVars->frametime = Interfaces.pGlobalVars->interval_per_tick;

		Interfaces.g_pGameMovement->StartTrackPredictionErrors(Hacks.LocalPlayer);

		memset(&m_MoveData, 0, sizeof(m_MoveData));
		Interfaces.g_pMoveHelper->SetHost(Hacks.LocalPlayer);
		Interfaces.g_pPred->SetupMove(Hacks.LocalPlayer, Hacks.CurrentCmd, Interfaces.g_pMoveHelper, &m_MoveData);
		Interfaces.g_pGameMovement->ProcessMovement(Hacks.LocalPlayer, &m_MoveData);
		Interfaces.g_pPred->FinishMove(Hacks.LocalPlayer, Hacks.CurrentCmd, &m_MoveData);
	}

	void End()
	{
		Interfaces.g_pGameMovement->FinishTrackPredictionErrors(Hacks.LocalPlayer);
		Interfaces.g_pMoveHelper->SetHost(0);

		*m_pPredictionRandomSeed = -1;

		Interfaces.pGlobalVars->curtime = m_flOldCurtime;
		Interfaces.pGlobalVars->frametime = m_flOldFrametime;
	}
}Predict;

bool __fastcall Hooked_CreateMove(void* thisptr, void*, float flInputSampleTime, CInput::CUserCmd* cmd)
{
	if (!cmd || cmd->command_number == 0)
		return false;

	Hacks.CurrentCmd = cmd;
	Hacks.LocalPlayer = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
	Hacks.LocalWeapon = Hacks.LocalPlayer->GetActiveBaseCombatWeapon();

	uintptr_t* FPointer; __asm { MOV FPointer, EBP }
	byte* SendPacket = (byte*)(*FPointer - 0x1C);

	Hacks.SendPacket = *SendPacket;
	Vars.Globals.CurMemeShots = iShoots;

	if (Hacks.LocalPlayer && Hacks.LocalWeapon && Hacks.LocalPlayer->isAlive())
	{
		Misc::BunnyHop();
		Misc::AutoStrafe();

		KnifeBot::Run();
	
		Predict.Start();

		CAim::Get().Run();
		CAim::Get().AfterAimShit();

		CAntiAim::Get().FakeLag();
		CAntiAim::Get().Run();

		Predict.End();

		Misc::ClampAngles();
		Misc::MoveFix();

		LocalInfo.Choked = Interfaces.Client_State->chokedcommands;
		Hacks.LastAngles = Hacks.CurrentCmd->viewangles;
	}

	*SendPacket = Hacks.SendPacket;

	return false;
}

int CBaseCombatWeapon::ScopeLevel()
{
	return *(int*)((DWORD)this + 0x3390);
}

void __stdcall Hooked_OverrideView(CViewSetup* pSetup)
{
	auto oOverrideView = clientmode_hook.get_original<iOverRideViewFn>(18);

	if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
	{
		if (Hacks.LocalPlayer && Hacks.LocalPlayer->isAlive())
		{
			bool can_scope = Hacks.LocalWeapon && Hacks.LocalWeapon->isSniper() && Hacks.LocalPlayer->m_bIsScoped() && Vars.Visuals.Effects.NoZoom;

			if (can_scope)
			{
				if (Hacks.LocalWeapon->ScopeLevel() != 2)
					pSetup->fov = 90 + Vars.Misc.FOV;
			}
			else {
				if (!Hacks.LocalPlayer->m_bIsScoped())
					pSetup->fov = 90 + Vars.Misc.FOV;
			}

			if (Vars.Visuals.Effects.NoVisRecoil)
				pSetup->angles -= Hacks.LocalPlayer->GetPunchAngle() * 0.9f + Hacks.LocalPlayer->GetPunchViewAngle();
		}

		//pSetup->angles = Aimbot.Angles;
	}
	Hacks.FOV = pSetup->fov;
	oOverrideView(pSetup);
}

void __stdcall hk_lockcursor()
{
	auto oLockCursor = vguisurf_hook.get_original<LockCursor_t>(67);

	if (Vars.Menu.Opened)
	{
		Interfaces.pSurface->unlockcursor();
		return;
	}

	oLockCursor(Interfaces.pSurface);
}

void DrawBeam(Vector src, Vector end, Color color)
{
	BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMTESLA;
	beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
	beamInfo.m_nModelIndex = -1;
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = 3.0f;
	beamInfo.m_flWidth = 2.0f;
	beamInfo.m_flEndWidth = 2.0f;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = color.a();
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = color.r();
	beamInfo.m_flGreen = color.g();
	beamInfo.m_flBlue = color.b();
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_vecStart = src;
	beamInfo.m_vecEnd = end;
	Beam_t* myBeam = Interfaces.ViewRenderBeams->CreateBeamPoints(beamInfo);
	if (myBeam)
		Interfaces.ViewRenderBeams->DrawBeam(myBeam);
}

void calculate_angle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.0f;
	if (delta.x >= 0.0) angles.y += 180.0f;
}

bool CBaseEntity::IsValid(bool need_local)
{
	if (this == nullptr)
		return false;
	if (need_local)
	{
		if (this == Hacks.LocalPlayer)
			return false;
		if (this->GetTeam() == Hacks.LocalPlayer->GetTeam())
			return false;
	}
	if (!this->isAlive())
		return false;
	if (!(this->GetHealth() > 0))
		return false;
	if (this->HasGunGameImmunity())
		return false;

	return true;
}

int CBaseEntity::GetActivity(int sequence)
{
	auto hdr = Interfaces.g_pModelInfo->GetStudioModel(this->GetModel());

	if (!hdr)
		return -1;

	static DWORD fn = NULL;

	if (!fn) 
		fn = (DWORD)Utils.FindPattern("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");

	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(fn);

	return GetSequenceActivity(this, hdr, sequence);
}

void __stdcall Hooked_Frame_Stage_Notify(ClientFrameStage_t curStage)
{
	auto oFrameStageNotify = hlclient_hook.get_original<iFrameStageNotifyFn>(37);

	Hacks.LocalPlayer = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetLocalPlayer());
	if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected())
	{
		if (Hacks.LocalPlayer)
		{
			Hacks.LocalWeapon = Hacks.LocalPlayer->GetActiveBaseCombatWeapon();
			LocalInfo.PunchAns = *Hacks.LocalPlayer->GetPunchAnglePtr();
			LocalInfo.Flags = Hacks.LocalPlayer->GetFlags();

			switch (curStage)
			{
			case FRAME_RENDER_START:
				if (Hacks.LocalPlayer->isAlive())
				{
					Esp::ThirdPerson();
					Hacks.LocalPlayer->SetAngle2(Vector(0, Hacks.LocalPlayer->GetAnimState()->m_flGoalFeetYaw, 0));
				}
				Misc::PVSFix();
				break;
			case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
				Misc::AnimFix();
				break;
			case FRAME_NET_UPDATE_START:
				if (Vars.Visuals.Other.BulletTracers)
				{
					float Red, Green, Blue;
					Red = Vars.Visuals.Colors.BulletTracers[0] * 255;
					Green = Vars.Visuals.Colors.BulletTracers[1] * 255;
					Blue = Vars.Visuals.Colors.BulletTracers[2] * 255;
					for (unsigned int i = 0; i < trace_logs.size(); i++)
					{
						auto *shooter = Interfaces.pEntList->GetClientEntity(Interfaces.pEngine->GetPlayerForUserID(trace_logs[i].userid));
						if (!shooter)
							return;

						DrawBeam(trace_logs[i].start, trace_logs[i].position, Color(Red, Green, Blue, Vars.Visuals.Colors.BulletTracers[3] * 255));
						trace_logs.erase(trace_logs.begin() + i);
					}
				}
				if (Vars.Visuals.Effects.NoSmoke) {
					static int* smokecount = *(int**)(Utils.FindPattern("client_panorama.dll", "8B 1D ? ? ? ? 56 33 F6 57 85 DB") + 0x2);
					*smokecount = 0;
				}
				break;
			}
		}
	}

	oFrameStageNotify(curStage);
}

typedef void(__thiscall* orgGameFunc_InitKeyValues)(void* thisptr, const char* name);
orgGameFunc_InitKeyValues pInitKeyValues;
void InitKeyValues(KeyValues* pKeyValues, const char* name)
{
	auto dwInitKeyValues = Utils.FindPattern(XorStr("client_panorama.dll"), XorStr("8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03 C1 F8 08 66 89 46 12 8B C6")) - 0x45;
	//printf(XorStr("InitKeyValues: 0x%X\n"), (DWORD)offys.dwInitKeyValues);

	if (!pInitKeyValues)
		pInitKeyValues = reinterpret_cast<orgGameFunc_InitKeyValues>(dwInitKeyValues);

	pInitKeyValues(pKeyValues, name);
}


typedef void(__thiscall* orgGameFunc_LoadFromBuffer)(void* thisptr, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);
orgGameFunc_LoadFromBuffer pLoadFromBuffer;
void GameFunc_LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr)
{
	/*55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04*/
	//Add Pattern Scan
	//static DWORD funcAddr = Utils::xFindPattern(client_dll, (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x34\x53\x8B\x5D\x0C\x89\x4C\x24\x04", "xxxxxxxxxxxxxxxxx");

	auto dwLoadFromBuffer = Utils.FindPattern(XorStr("client_panorama.dll"), XorStr("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));

	if (!pLoadFromBuffer)
		pLoadFromBuffer = reinterpret_cast<orgGameFunc_LoadFromBuffer>(dwLoadFromBuffer);

	pLoadFromBuffer(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);


}


IMaterial* Create_Material(bool Ignore, bool Lit, bool Wireframe)
{
	static int iCreated = 0;

	static const char szTmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$receiveflashlight\" \"1\"\
		\n\t\"$singlepassflashlight\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
		\n}\n"
	};

	char szMaterial[512];
	char szBaseType[64];

	if (Lit)
	{
		static const char pszBaseType[] = "UnlitGeneric";
		sprintf_s(szMaterial, sizeof(szMaterial), szTmp, pszBaseType, (Ignore) ? 1 : 0, (Wireframe) ? 1 : 0);
		strcpy_s(szBaseType, pszBaseType);
	}
	else
	{
		static const char pszBaseType[] = "VertexLitGeneric";
		sprintf_s(szMaterial, sizeof(szMaterial), szTmp, pszBaseType, (Ignore) ? 1 : 0, (Wireframe) ? 1 : 0);
		strcpy_s(szBaseType, pszBaseType);
	}

	char szName[512];
	sprintf_s(szName, sizeof(szName), "custom_material_%i.vmt", iCreated);

	++iCreated;

	KeyValues* keyValues = new KeyValues();
	InitKeyValues(keyValues, szBaseType);
	GameFunc_LoadFromBuffer(keyValues, szName, szMaterial, nullptr, NULL, nullptr);

	IMaterial *createdMaterial = Interfaces.pMaterialSystem->CreateMaterial(szName, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

static IMaterial* FlatXQZ;
static IMaterial* FlatVisible;

static IMaterial* DefaultXQZ;
static IMaterial* DefaultVisible;

void SetupChamsMaterial()
{
	std::ofstream("csgo\\materials\\regular_ref.vmt") << R"#("VertexLitGeneric"
{
	    "$basetexture"				    "vgui/white_additive"
		"$ignorez"					    "0"
		"$envmap"						"env_cubemap"
        "$envmaptint"                   "[.10 .10 .10]"
		"$phong"						"1"
		"$phongexponent"				"10"
		"$phongboost"					"3.0"
		"$rimlight"					    "1"
		"$nofog"						"1"
		"$model"						"1"
		"$nocull"						"0"
        "$lightwarptexture"             "metalic"
		"$selfillum"					"1"
		"$halflambert"				    "1"
}
)#";
	std::ofstream("csgo\\materials\\ignorez_ref.vmt") << R"#("VertexLitGeneric"
{
	 "$basetexture"				    "vgui/white_additive"
		"$ignorez"					    "1"
		"$envmap"						"env_cubemap"
        "$envmaptint"                   "[.10 .10 .10]"
		"$phong"						"1"
		"$phongexponent"				"10"
		"$phongboost"					"3.0"
		"$rimlight"					    "1"
		"$nofog"						"1"
		"$model"						"1"
		"$nocull"						"0"
        "$lightwarptexture"             "metalic"
		"$selfillum"					"1"
		"$halflambert"				    "1"
}
)#";

	FlatXQZ = Create_Material(true, true, false);
	FlatVisible = Create_Material(false, true, false);

	DefaultXQZ = Create_Material(true, false, false);
	DefaultVisible = Create_Material(false, false, false);
}

#define TEXTURE_GROUP_MODEL							"Model textures"

bool Do_Chams(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	static IMaterial* Covered_Lit;
	static IMaterial* Covered_Lit2;
	static IMaterial* Visable_Lit;
	static IMaterial* Visable_Lit2;
	static bool ff = false;
	if (!ff)
	{
		SetupChamsMaterial();
		ff = true;
	}

	auto oDrawModelExecute = mdlrender_hook.get_original<IDrawModelExecute>(21);
	if (Vars.Visuals.Chams.Enable)
	{
		switch (Vars.Visuals.Chams.Material)
		{
		case 0:
			Covered_Lit2 = FlatXQZ;
			Visable_Lit2 = FlatVisible;
			break;
		case 1:
			Covered_Lit = DefaultXQZ;
			Visable_Lit = DefaultVisible;
			Covered_Lit2 = DefaultXQZ;
			Visable_Lit2 = DefaultVisible;
			break;
		case 2:
			Covered_Lit = Interfaces.pMaterialSystem->FindMaterial("ignorez_ref", TEXTURE_GROUP_MODEL);
			Visable_Lit = Interfaces.pMaterialSystem->FindMaterial("regular_ref", TEXTURE_GROUP_MODEL);
			Covered_Lit2 = Interfaces.pMaterialSystem->FindMaterial("ignorez_ref", TEXTURE_GROUP_MODEL);
			Visable_Lit2 = Interfaces.pMaterialSystem->FindMaterial("regular_ref", TEXTURE_GROUP_MODEL);
			break;
		}

		CBaseEntity* Model_Entity = Interfaces.pEntList->GetClientEntity(pInfo.entity_index);
		auto Model_Name = Interfaces.g_pModelInfo->GetModelName(const_cast<model_t*>(pInfo.pModel));

		if (Model_Entity->IsPlayer())
		{
			if (Model_Entity->GetTeam() == Hacks.LocalPlayer->GetTeam() && !Vars.Visuals.Chams.Team)
				return false;

			if (Model_Entity->GetTeam() == Hacks.LocalPlayer->GetTeam())
			{
				if (Model_Entity == Hacks.LocalPlayer)
				{
					if (Model_Entity->HasGunGameImmunity())
						Covered_Lit->AlphaModulate(0.75f);
					else
						Covered_Lit->AlphaModulate(Vars.Visuals.Colors.Chams.LocalXQZ[3]);

					Visable_Lit->AlphaModulate(Vars.Visuals.Colors.Chams.Local[3]);

					Covered_Lit->ColorModulate(Vars.Visuals.Colors.Chams.LocalXQZ[0], Vars.Visuals.Colors.Chams.LocalXQZ[1], Vars.Visuals.Colors.Chams.LocalXQZ[2]); // t colour
					Visable_Lit->ColorModulate(Vars.Visuals.Colors.Chams.Local[0], Vars.Visuals.Colors.Chams.Local[1], Vars.Visuals.Colors.Chams.Local[2]); // t colour Visable

					if (Vars.Visuals.Chams.Local) {
						if (!Vars.Visuals.Chams.Visible)
						{
							Interfaces.g_pModelRender->ForcedMaterialOverride(Covered_Lit, OVERRIDE_NORMAL);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}

						Interfaces.g_pModelRender->ForcedMaterialOverride(Visable_Lit, OVERRIDE_NORMAL);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
				}
				else
				{
					if (Model_Entity->HasGunGameImmunity())
						Covered_Lit->AlphaModulate(0.75f);
					else
						Covered_Lit->AlphaModulate(Vars.Visuals.Colors.Chams.TeamXQZ[3]);

					Visable_Lit->AlphaModulate(Vars.Visuals.Colors.Chams.Team[3]);

					Covered_Lit->ColorModulate(Vars.Visuals.Colors.Chams.TeamXQZ[0], Vars.Visuals.Colors.Chams.TeamXQZ[1], Vars.Visuals.Colors.Chams.TeamXQZ[2]); // t colour
					Visable_Lit->ColorModulate(Vars.Visuals.Colors.Chams.Team[0], Vars.Visuals.Colors.Chams.Team[1], Vars.Visuals.Colors.Chams.Team[2]); // t colour Visable

					if (Vars.Visuals.Chams.Team) {
						if (!Vars.Visuals.Chams.Visible)
						{
							Interfaces.g_pModelRender->ForcedMaterialOverride(Covered_Lit, OVERRIDE_NORMAL);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}

						Interfaces.g_pModelRender->ForcedMaterialOverride(Visable_Lit, OVERRIDE_NORMAL);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}
				}
			}
			else
			{
				if (Model_Entity->HasGunGameImmunity())
					Covered_Lit->AlphaModulate(0.75f);
				else
					Covered_Lit->AlphaModulate(Vars.Visuals.Colors.Chams.XQZ[3]);

				Visable_Lit->AlphaModulate(Vars.Visuals.Colors.Chams.Visible[3]);

				Covered_Lit->ColorModulate(Vars.Visuals.Colors.Chams.XQZ[0], Vars.Visuals.Colors.Chams.XQZ[1], Vars.Visuals.Colors.Chams.XQZ[2]); // ct colour
				Visable_Lit->ColorModulate(Vars.Visuals.Colors.Chams.Visible[0], Vars.Visuals.Colors.Chams.Visible[1], Vars.Visuals.Colors.Chams.Visible[2]); // ct colour Visable

				if (Vars.Visuals.Chams.Enemy) {
					if (!Vars.Visuals.Chams.Visible)
					{
						Interfaces.g_pModelRender->ForcedMaterialOverride(Covered_Lit, OVERRIDE_NORMAL);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}

					Interfaces.g_pModelRender->ForcedMaterialOverride(Visable_Lit, OVERRIDE_NORMAL);
					oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
				}
			}

			if (!Model_Entity->isAlive())
				return false;

			return true;
		}
		if (Model_Entity->IsWeapon() && Vars.Visuals.Chams.Weapons)
		{
			Covered_Lit2->ColorModulate(Vars.Visuals.Colors.Chams.WeaponsXQZ[0], Vars.Visuals.Colors.Chams.WeaponsXQZ[1], Vars.Visuals.Colors.Chams.WeaponsXQZ[2]); // t colour
			Visable_Lit2->ColorModulate(Vars.Visuals.Colors.Chams.Weapons[0], Vars.Visuals.Colors.Chams.Weapons[1], Vars.Visuals.Colors.Chams.Weapons[2]); // t colour Visable
			Visable_Lit2->AlphaModulate(Vars.Visuals.Colors.Chams.WeaponsXQZ[3]);
			Covered_Lit2->AlphaModulate(Vars.Visuals.Colors.Chams.Weapons[3]);

			if (!Vars.Visuals.Chams.Visible)
			{
				Interfaces.g_pModelRender->ForcedMaterialOverride(Covered_Lit2, OVERRIDE_NORMAL);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
			}

			Interfaces.g_pModelRender->ForcedMaterialOverride(Visable_Lit2, OVERRIDE_NORMAL);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

			return true;
		}
	}


	return false;
}

void __fastcall Hooked_SceneEnd(void* ecx, void* edx) {
	auto oSceneEnd = renderview_hook.get_original<SceneEndFn>(9);
	oSceneEnd(ecx);

	Esp::GlowESP();
}

void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	auto oDrawModelExecute = mdlrender_hook.get_original<IDrawModelExecute>(21);

	const char* ModelName = Interfaces.g_pModelInfo->GetModelName((model_t*)pInfo.pModel);

	if (Vars.Visuals.Other.Nightmode) {
		if (strstr(ModelName, "models/props"))
			Interfaces.g_pRenderView->SetBlend(0.5f);
	}

	auto ent = Interfaces.pEntList->GetClientEntity(pInfo.entity_index);

	if (ent && ent->isAlive() && ent == Hacks.LocalPlayer && Hacks.LocalPlayer->m_bIsScoped())
	{
		Interfaces.g_pRenderView->SetBlend(0.65f);
	}

	if (Vars.Visuals.Effects.NoFlash)
	{
		IMaterial* Flash = Interfaces.pMaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
		IMaterial* FlashWhite = Interfaces.pMaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
		Flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		FlashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
	}

	//if (!Do_Chams(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld))
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces.g_pModelRender->ForcedMaterialOverride(nullptr, OVERRIDE_NORMAL);

}

LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		Hacks.PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		Hacks.PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		Hacks.PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		Hacks.PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		Hacks.PressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		Hacks.PressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			Hacks.PressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			Hacks.PressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			Hacks.PressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			Hacks.PressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		Hacks.PressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		Hacks.PressedKeys[wParam] = false;
		break;
	default: break;
	}

	OpenMenu();

	if (Vars.Menu.Init && Vars.Menu.Opened && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
}