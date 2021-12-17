#include "global_count.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "circlestrafer.h"
#include "CBulletListener.h"
#include "Interfaces.h"
#include "autodefuse.h"
#include "RenderManager.h"
#include "lodepng.h"
#include "knifebot.h"
#include "Visuals.h"
#include <d3d9.h>
#include "EnginePrediction.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "hitmarker.h"
#include "laggycompensation.h"
#include <intrin.h>
#include "DamageIndicator.h"
#include "RageBot.h"
#include "LagCompensation2.h"
#include "position_adjust.h"
#include "EnginePrediction.h"
#include "lin_extp.h"
#include "radar.h"
#include "fakelag.h"
#include "experimental.h"
std::vector<impact_info> impacts;
std::vector<hitmarker_info> Xhitmarkers;
static CPredictionSystem* Prediction = new CPredictionSystem();

CLagcompensation lagcompensation;
HANDLE worldmodel_handle;
C_BaseCombatWeapon* worldmodel;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif
#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#else
#define XorStr( s ) ( s )
#endif
Vector Globals::aim_point;
int bigboi::indicator;
bool bigboi::freestand;
bool round_change;
int bigboi::freestandval;
std::vector<trace_info> trace_logs;
int currentfov;
Vector LastAngleAA;
extern Vector LastAngleAA2;
Vector LastAngleAAFake;
Vector last_fake;
bool Resolver::didhitHS;
CUserCmd* Globals::UserCmd;
IClientEntity* Globals::Target;
int Globals::Shots;
bool Globals::change;
int Globals::TargetID;
bool Resolver::hitbaim;
bool Globals::Up2date;
int Globals::fired;
int Globals::hit;
extern float lineLBY;
extern float lineLBY2;

extern float current_desync;
extern float lineRealAngle;
//extern float lineFakeAngle;
extern float last_real;
extern float lspeed;
extern float pitchmeme;
extern float lby2;
extern float inaccuracy;

Vector LastAngleAAReal;
Vector LBYThirdpersonAngle;
float bigboi::current_yaw;
#define STUDIO_RENDER					0x00000001
std::map<int, QAngle>Globals::storedshit;
int Globals::missedshots;
static int missedLogHits[65];
float fakeangle;
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef long(__stdcall *EndScene_t)(IDirect3DDevice9*);
typedef int(__thiscall* DoPostScreenEffects_t)(IClientModeShared*, int);
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
typedef long(__stdcall *Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);

typedef float(__stdcall *oGetViewModelFOV)();
typedef void(__thiscall *SceneEnd_t)(void *pEcx);
EndScene_t o_EndScene;
SceneEnd_t pSceneEnd;
Reset_t o_Reset;
DoPostScreenEffects_t o_DoPostScreenEffects;
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
FireEventClientSideFn oFireEventClientSide;
RenderViewFn oRenderView;

void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd);
HRESULT __stdcall EndScene_hooked(IDirect3DDevice9 *pDevice);
int __stdcall Hooked_DoPostScreenEffects(int a1);
HRESULT __stdcall Reset_hooked(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
void __fastcall	hkSceneEnd(void *pEcx, void *pEdx);
typedef void(__thiscall* LockCursor)(void*);

LockCursor oLockCursor;

void	__stdcall Hooked_LockCursor()
{
	bool xd = Options::Menu.m_bIsOpen;
	if (xd) {
		Interfaces::Surface->unlockcursor();
		return;
	}
	oLockCursor(Interfaces::Surface);
}

namespace GlobalBREAK
{
	bool bVisualAimbotting = false;
	QAngle vecVisualAimbotAngs = QAngle(0.f, 0.f, 0.f);
	int ChokeAmount = 0;
	float flFakewalked = 0.f;
	bool NewRound = false;
	bool WeaponFire = false;
	QAngle fakeangleslocal;
	bool bRainbowCross = true;
	bool dohitmarker;
	float LastTimeWeFired = 0;
	int ShotsFiredLocally = 0;
	int ShotsHitPerEntity[65];
	bool HeadShottedEntity[65] = { false };
	float curFov = 0;
	bool bUsingFakeAngles[65];
	float HitMarkerAlpha = 0.f;
	int TicksOnGround = 0;
	int ticks_while_unducked = 0;
	char* breakmode;
	int AnimationPitchFix = 0;
	float hitchance;
	int NextPredictedLBYUpdate = 0;
	int breakangle;
	int prevChoked = 0;
	bool AAFlip = false;
	bool LEFT;
	bool RIGHT;
	bool BACK;
	char my_documents_folder[MAX_PATH];
	float smt = 0.f;
	QAngle visualAngles = QAngle(0.f, 0.f, 0.f);
	bool bSendPacket = false;
	bool bAimbotting = false;
	CUserCmd* userCMD = nullptr;
	char* szLastFunction = "<No function was called>";
	HMODULE hmDll = nullptr;
	bool bFakewalking = false;
	Vector vecUnpredictedVel = Vector(0, 0, 0);
	float flFakeLatencyAmount = 0.f;
	float flEstFakeLatencyOnServer = 0.f;
	matrix3x4_t traceHitboxbones[128];
	std::array<std::string, 64> resolverModes;
}
int ground_tick;
Vector OldOrigin;
namespace Hooks
{
	Utilities::Memory::VMTManager VMTPanel;
	Utilities::Memory::VMTManager VMTClient;
	Utilities::Memory::VMTManager VMTClientMode;
	Utilities::Memory::VMTManager VMTModelRender;
	Utilities::Memory::VMTManager VMTPrediction;
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager;
	Utilities::Memory::VMTManager VMTDIRECTX;
	Utilities::Memory::VMTManager VMTSurface;

};
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
	VMTDIRECTX.RestoreOriginal();
	VMTEventManager.RestoreOriginal();
	VMTSurface.RestoreOriginal();
	VMTRenderView.RestoreOriginal();
	VMTClient.RestoreOriginal();
}
void Hooks::Initialise()
{
	Interfaces::Engine->ExecuteClientCmd("clear");
	//--------------- D3D ---------------//
	VMTDIRECTX.Initialise((DWORD*)Interfaces::g_pD3DDevice9);
	o_EndScene = (EndScene_t)VMTDIRECTX.HookMethod((DWORD)&EndScene_hooked, 42);
	VMTDIRECTX.Initialise((DWORD*)Interfaces::g_pD3DDevice9);
	o_Reset = (Reset_t)VMTDIRECTX.HookMethod((DWORD)&Reset_hooked, 16);
	//--------------- NORMAL HOOKS ---------------//
	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	o_DoPostScreenEffects = (DoPostScreenEffects_t)VMTClientMode.HookMethod((DWORD)Hooked_DoPostScreenEffects, 44);
	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);
	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);
	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);
	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);
	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);
	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 37);
	VMTEventManager.Initialise((DWORD*)Interfaces::EventManager);
	oFireEventClientSide = (FireEventClientSideFn)VMTEventManager.HookMethod((DWORD)&Hooked_FireEventClientSide, 9);
	VMTRenderView.Initialise((DWORD*)Interfaces::RenderView);
	pSceneEnd = (SceneEnd_t)VMTRenderView.HookMethod((DWORD)&hkSceneEnd, 9);
	VMTSurface.Initialise((DWORD*)Interfaces::Surface);
	oLockCursor = (LockCursor)VMTSurface.HookMethod((DWORD)Hooked_LockCursor, 67);

	ConVar* nameVar = Interfaces::CVar->FindVar("name");
	//--------------- NAME CVAR ---------------//
	if (nameVar)
	{
		*(int*)((DWORD)&nameVar->fnChangeCallback + 0xC) = 0;
	}
	//--------------- EVENT LOG ---------------//
	static auto y = Interfaces::CVar->FindVar("sv_showanimstate"); //this probably isn't avaible in modern source
	y->SetValue(1);
	static auto developer = Interfaces::CVar->FindVar("developer");
	developer->SetValue(1);
	static auto con_filter_text_out = Interfaces::CVar->FindVar("con_filter_text_out");
	static auto con_filter_enable = Interfaces::CVar->FindVar("con_filter_enable");
	static auto con_filter_text = Interfaces::CVar->FindVar("con_filter_text");
	static auto dogstfu = Interfaces::CVar->FindVar("con_notifytime");
	dogstfu->SetValue(3);
	con_filter_text->SetValue(".     ");
	con_filter_text_out->SetValue("");
	con_filter_enable->SetValue(2);

}

AnimatedClanTag *animatedClanTag = new AnimatedClanTag();

int __stdcall Hooked_DoPostScreenEffects(int a1)
{
	auto m_local = hackManager.pLocal();

	for (auto i = 0; i < Interfaces::m_pGlowObjManager->size; i++)
	{
		auto glow_object = &Interfaces::m_pGlowObjManager->m_GlowObjectDefinitions[i];
		IClientEntity *m_entity = glow_object->m_pEntity;
		if (!glow_object->m_pEntity || glow_object->IsUnused() || !m_local)
			continue;
		if (m_entity->GetClientClass()->m_ClassID == 1 || m_entity->GetClientClass()->m_ClassID == 66 || m_entity->GetClientClass()->m_ClassID == 87 || m_entity->GetClientClass()->m_ClassID == 98 || m_entity->GetClientClass()->m_ClassID == 108 || m_entity->GetClientClass()->m_ClassID == 134 || m_entity->GetClientClass()->m_ClassID == 1 || m_entity->GetClientClass()->m_ClassID == 9 || m_entity->GetClientClass()->m_ClassID == 29 || m_entity->GetClientClass()->m_ClassID == 39 || m_entity->GetClientClass()->m_ClassID == 41 || strstr(m_entity->GetClientClass()->m_pNetworkName, "Weapon"))
		{
			if (Options::Menu.VisualsTab.OtherEntityGlow.GetState()) {
				float m_flRed = Options::Menu.ColorsTab.GlowOtherEnt.GetValue()[0], m_flGreen = Options::Menu.ColorsTab.GlowOtherEnt.GetValue()[1], m_flBlue = Options::Menu.ColorsTab.GlowOtherEnt.GetValue()[2];
				bool m_teammate = m_entity->team() == m_local->team();
				glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
				glow_object->m_flGlowAlpha = 1.f;
				glow_object->m_bRenderWhenOccluded = true;
				glow_object->m_bRenderWhenUnoccluded = false;
			}
		}

		if (m_entity->GetClientClass()->m_ClassID == 38)
		{
			if (m_entity == m_local && Options::Menu.VisualsTab.Glowz_lcl.GetValue() > 0)
			{
				if (m_local->IsAlive() && Options::Menu.VisualsTab.localmaterial.GetIndex() < 6)
				{
					float m_flRed = Options::Menu.ColorsTab.GlowLocal.GetValue()[0], m_flGreen = Options::Menu.ColorsTab.GlowLocal.GetValue()[1], m_flBlue = Options::Menu.ColorsTab.GlowLocal.GetValue()[2];
					glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
					glow_object->m_flGlowAlpha = Options::Menu.VisualsTab.Glowz_lcl.GetValue() / 100;
					glow_object->m_bRenderWhenOccluded = true;
					glow_object->m_bRenderWhenUnoccluded = false;
				//	glow_object->m_bPulsatingChams = 1; 
				}

				if (Options::Menu.VisualsTab.localmaterial.GetIndex() > 5 && m_local->IsScoped())
				{
					float m_flRed = Options::Menu.ColorsTab.GlowLocal.GetValue()[0], m_flGreen = Options::Menu.ColorsTab.GlowLocal.GetValue()[1], m_flBlue = Options::Menu.ColorsTab.GlowLocal.GetValue()[2];
					glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
					glow_object->m_flGlowAlpha = 100;
					glow_object->m_bRenderWhenOccluded = true;
					glow_object->m_bRenderWhenUnoccluded = false;
					glow_object->m_bPulsatingChams = 2; 
				}
			}

			if (m_entity->team() != m_local->team() && Options::Menu.VisualsTab.GlowZ.GetValue() > 0)
			{

				float m_flRed = Options::Menu.ColorsTab.GlowEnemy.GetValue()[0], m_flGreen = Options::Menu.ColorsTab.GlowEnemy.GetValue()[1], m_flBlue = Options::Menu.ColorsTab.GlowEnemy.GetValue()[2];
			
				glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
				glow_object->m_flGlowAlpha = Options::Menu.VisualsTab.GlowZ.GetValue() / 100;
				glow_object->m_bRenderWhenOccluded = true;
				glow_object->m_bRenderWhenUnoccluded = false;
			}

			if (m_entity->team() == m_local->team() && m_entity != m_local)
			{
				float m_flRed = Options::Menu.ColorsTab.GlowTeam.GetValue()[0], m_flGreen = Options::Menu.ColorsTab.GlowTeam.GetValue()[1], m_flBlue = Options::Menu.ColorsTab.GlowTeam.GetValue()[2];

				glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
				glow_object->m_flGlowAlpha = Options::Menu.VisualsTab.team_glow.GetValue() / 100;
				glow_object->m_bRenderWhenOccluded = true;
				glow_object->m_bRenderWhenUnoccluded = false;
			}
		}
	}
	return o_DoPostScreenEffects(Interfaces::ClientMode, a1);
}
HRESULT __stdcall Reset_hooked(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	auto hr = o_Reset(pDevice, pPresentationParameters);
	if (hr >= 0)
	{
		bool gey;
		gey = true;
	}
	return hr;
}
struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
};
#define M_PI 3.14159265358979323846
void CircleFilledRainbowColor(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;
	std::vector<CUSTOMVERTEX> circle(resolution + 2);
	float angle = rotate * M_PI / 180, pi = M_PI;
	if (type == 1)
		pi = M_PI; // Full circle
	if (type == 2)
		pi = M_PI / 2; // 1/2 circle
	if (type == 3)
		pi = M_PI / 4; // 1/4 circle
	pi = M_PI / type; // 1/4 circle
	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);
	float hue = 0.f;
	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		auto clr = Color::FromHSB(hue, 1.f, 1.f);
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a() - 175);
		hue += 0.02;
	}
	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));
		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}
	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);
	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();
	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}
#define M_PI 3.14159265358979323846


void greyone(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;
	std::vector<CUSTOMVERTEX> circle(resolution + 2);
	float angle = rotate * M_PI / 180, pi = M_PI;
	if (type == 1)
		pi = M_PI; // Full circle
	if (type == 2)
		pi = M_PI / 2; // 1/2 circle
	if (type == 3)
		pi = M_PI / 4; // 1/4 circle
	pi = M_PI / type; // 1/4 circle
	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);
	float hue = 0.f;
	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		auto clr = Color(15, 15, 15);
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a() - 175);
		hue += 0.02;
	}
	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));
		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}
	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);
	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();
	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}
void colorboy69(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;
	std::vector<CUSTOMVERTEX> circle(resolution + 2);
	float angle = rotate * M_PI / 180, pi = M_PI;
	if (type == 1)
		pi = M_PI; // Full circle
	if (type == 2)
		pi = M_PI / 2; // 1/2 circle
	if (type == 3)
		pi = M_PI / 4; // 1/4 circle
	pi = M_PI / type; // 1/4 circle
	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);
	float hue = 0.f;
	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		auto clr = (Color)Options::Menu.ColorsTab.spreadcrosscol.GetValue();
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a() - 175);
		hue += 0.02;
	}
	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));
		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}
	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);
	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();
	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}
HRESULT __stdcall EndScene_hooked(IDirect3DDevice9 *pDevice)
{
	//this will probably get drawn even over the console and other CSGO hud elements, but whatever
	//this will also draw over the menu so we should disable it if the menu is open
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto g_LocalPlayer = pLocal;
	if (g_LocalPlayer && g_LocalPlayer->IsAlive() && g_LocalPlayer->GetWeapon2()) {
		int w, h;
		Interfaces::Engine->GetScreenSize(w, h); w /= 2; h /= 2;
		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		{
			int w, h;
			static float rot = 0.f;
			Interfaces::Engine->GetScreenSize(w, h); w /= 2; h /= 2;
			C_BaseCombatWeapon* pWeapon = g_LocalPlayer->GetWeapon2();
			if (pWeapon)
			{
				short Index = (int)pWeapon->GetItemDefinitionIndex();
				if (Index == WEAPON_TASER) {
					//DrawTaserRange();
				}
				if (g_LocalPlayer && Index != 42 && Index != 59 && Index != 500)
				{
					if (Options::Menu.VisualsTab.SpreadCrosshair.GetIndex() != 0 && !Options::Menu.m_bIsOpen)
					{
						auto accuracy = pWeapon->GetInaccuracy() * (90 * 6.5);

						switch (Options::Menu.VisualsTab.SpreadCrosshair.GetIndex())
						{
							case 1:
							{
								greyone(w, h, accuracy, 0, 1, 50, pDevice);
							}
							break;

							case 2:
							{
								colorboy69(w, h, accuracy, 0, 1, 50, pDevice);
							}
							break;

							case 3:
							{
								CircleFilledRainbowColor(w, h, accuracy, 0, 1, 50, pDevice);
							}
							break;

							case 4:
							{
								CircleFilledRainbowColor(w, h, accuracy, rot, 1, 50, pDevice);
							}
							break;
						}

						rot += 1.f;
						if (rot > 360.f) 
							rot = 0.f;
					}
				}
			}
		}
	}
	return o_EndScene(pDevice);
}
void MovementCorrection(CUserCmd* userCMD, IClientEntity * local)
{
	if (!local)
		return;

	if (userCMD->forwardmove) {
		userCMD->buttons &= ~(userCMD->forwardmove < 0 ? IN_FORWARD : IN_BACK);
		userCMD->buttons |= (userCMD->forwardmove > 0 ? IN_FORWARD : IN_BACK);
	}
	if (userCMD->sidemove) {
		userCMD->buttons &= ~(userCMD->sidemove < 0 ? IN_MOVERIGHT : IN_MOVELEFT);
		userCMD->buttons |= (userCMD->sidemove > 0 ? IN_MOVERIGHT : IN_MOVELEFT);
	}

}
float clip(float n, float lower, float upper)
{
	return (std::max)(lower, (std::min)(n, upper));
}
int kek = 0;
int autism = 0;
int speed = 0;
static float testtimeToTick;
static float testServerTick;
static float testTickCount64 = 1;

float NormalizeYaw(float value)
{
	while (value > 180)
		value -= 360.f;
	while (value < -180)
		value += 360.f;
	return value;
}
float random_float(float min, float max)
{
	typedef float(*RandomFloat_t)(float, float);
	static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle(("vstdlib.dll")), ("RandomFloat"));
	return m_RandomFloat(min, max);
}
LinearExtrapolations linear_extraps;
std::string Tag = " "; // "freakratskids"
std::string Tag2 = " mirror ";
void set_clan_tag(const char* tag, const char* clan_name)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(Utilities::Memory::FindPatternV2(XorStr("engine.dll"), XorStr("53 56 57 8B DA 8B F9 FF 15")));
	pSetClanTag(tag, clan_name);
}
void clan_changer()
{
	auto m_local = hackManager.pLocal();	
	

/*	if (Options::Menu.ColorsTab.ClanTag.GetIndex() == 1)
	{
		if ( !m_local || !m_local->IsAlive() )
		{
			Tag2 += Tag.at(0);
			Tag2.erase(0, 1);
			set_clan_tag(Tag2.c_str(), " mirror ");
		}
		static size_t lastTime = 0;

		if (GetTickCount() > lastTime)
		{
			Tag += Tag.at(0);
			Tag.erase(0, 1);
			set_clan_tag(Tag.c_str(), "mirror ");
			lastTime = GetTickCount() + 650;
		}

	}
*/
	if (Options::Menu.ColorsTab.ClanTag.GetState())
	{
		if (!m_local || !m_local->IsAlive())
		{
			Tag2 += Tag.at(0);
			Tag2.erase(0, 1);
			set_clan_tag(Tag2.c_str(), " b1g p2c no pasta v6.67 ");
		}
		static size_t lastTime = 0;

		if (GetTickCount() > lastTime)
		{
			Tag += Tag.at(0);
			Tag.erase(0, 1);
			set_clan_tag(Tag.c_str(), " best hvh p2c no pasta v6.67 ");
			lastTime = GetTickCount() + 650;
		}

	}

}
struct CIncomingSequence
{
	CIncomingSequence::CIncomingSequence(int instate, int outstate, int seqnr, float time)
	{
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};
std::deque<CIncomingSequence> sequences;
int32_t lastincomingsequencenumber;

bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number )
		return true;
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	uintptr_t* FPointer; __asm { MOV FPointer, EBP }
	byte* SendPacket = (byte*)(*FPointer - 0x1C);
	GlobalBREAK::bSendPacket = *SendPacket;
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		static bool boost_fps = false;
		static bool abc = false;
		static bool freakware = false;

		if (Options::Menu.VisualsTab.colmodupdate.GetState())
		{
			if (!freakware)
			{
				ConVar* staticdrop = Interfaces::CVar->FindVar("r_DrawSpecificStaticProp");
				SpoofedConvar* staticdrop_spoofed = new SpoofedConvar(staticdrop);
				staticdrop_spoofed->SetInt(0);
				ConVar* NightSkybox1 = Interfaces::CVar->FindVar("sv_skyname");
				*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = NULL;
				for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

					if (!pMaterial)
						continue;
					if (!pMaterial || pMaterial->IsErrorMaterial())
						continue;

					Interfaces::Engine->ExecuteClientCmd("mat_queue_mode 2");

					float sky_r = Options::Menu.ColorsTab.sky_r.GetValue() / 10;
					float sky_g = Options::Menu.ColorsTab.sky_g.GetValue() / 10;
					float sky_b = Options::Menu.ColorsTab.sky_b.GetValue() / 10;

					float test = Options::Menu.VisualsTab.asusamount.GetValue() / 100;
					float amountr = Options::Menu.VisualsTab.colmod.GetValue() / 100;

					if (Options::Menu.VisualsTab.customskies.GetIndex() == 1)
					{
						NightSkybox1->SetValue("sky_csgo_night02b");
					}
					if (Options::Menu.VisualsTab.customskies.GetIndex() == 2)
					{
						NightSkybox1->SetValue("sky_l4d_rural02_ldr");
					}
					if (Options::Menu.VisualsTab.customskies.GetIndex() == 3)
					{
						NightSkybox1->SetValue("sky_csgo_cloudy01");
					}
					if (Options::Menu.VisualsTab.customskies.GetIndex() == 4)
					{
						NightSkybox1->SetValue("sky_csgo_nukeblank2");
					}

					if (Options::Menu.ColorsTab.ModulateSkyBox.GetState())
					{
						if (strstr(pMaterial->GetTextureGroupName(), ("SkyBox")))
						{
							pMaterial->ColorModulate(sky_r, sky_g, sky_b);
						//	Interfaces::CVar->ConsoleColorPrintf(Color(160, 5, 240, 255), "\n twitch.tv/attysu for giving me some will to keep up with the project \n");
						}
					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures") && Options::Menu.ColorsTab.asus_type.GetIndex() < 1)  // walls	
					{
						pMaterial->ColorModulation(amountr, amountr, amountr);				
					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures") && Options::Menu.ColorsTab.asus_type.GetIndex() > 0)  // walls	
					{
						pMaterial->AlphaModulate(test);
						pMaterial->ColorModulation(amountr, amountr, amountr);
					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->AlphaModulate(test);
						pMaterial->ColorModulation(amountr, amountr, amountr);
					}

				}
			}
			freakware = true;
			Options::Menu.VisualsTab.colmodupdate.SetState(false);
		}
		else
		{
			freakware = false;
		}
		pCmd->sidemove = pCmd->sidemove;
		pCmd->upmove = pCmd->upmove;
		pCmd->forwardmove = pCmd->forwardmove;
	
		GlobalBREAK::bSendPacket = *SendPacket;
		if (GetAsyncKeyState(Options::Menu.MiscTab.manualleft.GetKey()))
		{
			bigboi::indicator = 1;
		}
		else if (GetAsyncKeyState(Options::Menu.MiscTab.manualright.GetKey()))
		{
			bigboi::indicator = 2;
		}
		else if (GetAsyncKeyState(Options::Menu.MiscTab.manualback.GetKey()))
		{
			bigboi::indicator = 3;
		}
		else if (GetAsyncKeyState(Options::Menu.MiscTab.manualfront.GetKey()))
		{
			bigboi::indicator = 4;
		}

		defususmaximus(pCmd);

		GlobalBREAK::smt = frametime;
		GlobalBREAK::userCMD = pCmd;
		GlobalBREAK::vecUnpredictedVel = pLocal->GetVelocity();

		clan_changer();

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;
		uintptr_t* framePtr;
		__asm mov framePtr, ebp;
		GlobalBREAK::bSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);

		if (pLocal->GetFlags() & FL_ONGROUND)
			GlobalBREAK::TicksOnGround++;
		else
			GlobalBREAK::TicksOnGround = 0;

		if (pCmd->buttons & IN_DUCK)
			GlobalBREAK::ticks_while_unducked = 0;
		else
			GlobalBREAK::ticks_while_unducked++;

		if (GlobalBREAK::bSendPacket)
			GlobalBREAK::prevChoked = Interfaces::m_pClientState->chokedcommands;
		if (!GlobalBREAK::bSendPacket)
			GlobalBREAK::visualAngles = QAngle(pCmd->viewangles.x, pCmd->viewangles.y, pCmd->viewangles.z);
		if (GlobalBREAK::TicksOnGround == 1)
			*(bool*)(*(DWORD*)pebp - 0x1C) = false;
		if (GlobalBREAK::TicksOnGround == 1 && pLocal->getFlags() & FL_ONGROUND)
			*(bool*)(*(DWORD*)pebp - 0x1C) = false;
		if (GlobalBREAK::TicksOnGround == 0 && pLocal->GetFlags() & FL_ONGROUND)
			*(bool*)(*(DWORD*)pebp - 0x1C) = false;

		globalsh.bSendPaket = true;
		if (Options::Menu.MiscTab.FakeLagChoke.GetValue() > 0 || Options::Menu.MiscTab.FakeLagChoke2.GetValue() > 0)
			globalsh.bSendPaket = false;
		if (Interfaces::m_pClientState->chokedcommands > 13 || (Interfaces::m_pClientState->chokedcommands == globalsh.ChokeAmount &&
			(Options::Menu.MiscTab.FakeLagChoke.GetValue() > 0 || Options::Menu.MiscTab.FakeLagChoke2.GetValue() > 0)))
			globalsh.bSendPaket = false;
		globalsh.bSendPaket = (bool*)(*(DWORD*)pebp - 0x1C);
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		if (globalsh.bSendPaket)
			globalsh.prevChoked = Interfaces::m_pClientState->chokedcommands;
		IClientEntity* pEntity;
		Vector ClientAngles; Interfaces::Engine->GetViewAngles(ClientAngles);
		if (Options::Menu.MiscTab.SniperCrosshair.GetState() && pLocal->IsAlive() && !pLocal->IsScoped()) 
		{
			if (pLocal->GetWeapon2() && pLocal->GetWeapon2()->m_bIsSniper()) {
				ConVar* cross = Interfaces::CVar->FindVar("weapon_debug_spread_show");
				cross->nFlags &= ~FCVAR_CHEAT;
				cross->SetValue(3);
			}
		}
		else {
			ConVar* cross = Interfaces::CVar->FindVar("weapon_debug_spread_show");
			cross->nFlags &= ~FCVAR_CHEAT;
			cross->SetValue(0);
		}
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal)
		{
			if (pLocal->GetFlags() & FL_ONGROUND && !(CMBacktracking::Get().current_record->m_nFlags & FL_ONGROUND)) {
				*(bool*)(*(DWORD*)pebp - 0x1C) = true;
			}
			if (pLocal->GetFlags() & FL_ONGROUND && Interfaces::m_iInputSys->IsButtonDown(KEY_SPACE)) {
				*(bool*)(*(DWORD*)pebp - 0x1C) = false;
			}
			if (!pLocal->GetFlags() & FL_ONGROUND && !Interfaces::m_iInputSys->IsButtonDown(KEY_SPACE)) {
				*(bool*)(*(DWORD*)pebp - 0x1C) = false;
			}
			if (Interfaces::m_iInputSys->IsButtonDown(MOUSE_LEFT)) {
				*(bool*)(*(DWORD*)pebp - 0x1C) = false;
			}
			for (int i = 1; i < 2; i++)
			{
				if (pLocal->GetFlags() & FL_ONGROUND && !(headPositions[pLocal->GetIndex()][i].flags)) {
					*(bool*)(*(DWORD*)pebp - 0x1C) = true;
				}
			}
		
		//	backtracking->rageBackTrack(pCmd, pLocal);
			Prediction->StartPrediction(pCmd);
			{
				CMBacktracking::Get().StartLagCompensation(pLocal);

				if (Options::Menu.RageBotTab.extrapolation.GetState())
					linear_extraps.run();

				Hacks::MoveHacks(pCmd, bSendPacket);
			}
			Prediction->EndPrediction(pCmd);
			

			if (pCmd->forwardmove) {
				pCmd->buttons &= ~(pCmd->forwardmove < 0 ? IN_FORWARD : IN_BACK);
				pCmd->buttons |= (pCmd->forwardmove > 0 ? IN_FORWARD : IN_BACK);
			}
			if (pCmd->sidemove) {
				pCmd->buttons &= ~(pCmd->sidemove < 0 ? IN_MOVERIGHT : IN_MOVELEFT);
				pCmd->buttons |= (pCmd->sidemove > 0 ? IN_MOVERIGHT : IN_MOVELEFT);
			}

		}

		IClientEntity* LocalPlayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		float flServerTime = (float)(LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);
		static float next_time = 0;
		MovementCorrection(pCmd, LocalPlayer);
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);
		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		if (Options::Menu.MiscTab.OtherSafeMode.GetIndex() < 2)
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles); 
			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}
			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log(" Re-calculating angles");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
			if (pCmd->viewangles.x > 90)
			{
				pCmd->forwardmove = -pCmd->forwardmove;
			}
			if (pCmd->viewangles.x < -90)
			{
				pCmd->forwardmove = -pCmd->forwardmove;
			}
		}

		LastAngleAAReal = pCmd->viewangles;
	
		lineLBY = pLocal->GetLowerBodyYaw();
		lineLBY2 = LastAngleAAReal.y - pLocal->GetLowerBodyYaw();

		if (pLocal && pLocal->IsAlive() && pWeapon != nullptr && !GameUtils::IsGrenade(pWeapon) && !(pWeapon->isZeus() || pWeapon->IsC4()))
		{
			inaccuracy = pWeapon->GetInaccuracy() * 1000;
			lspeed = pLocal->GetVelocity().Length2D();
			pitchmeme = pCmd->viewangles.x;
		}
		
	}
	return false;
}

static void drawThiccLine(int x1, int y1, int x2, int y2, int type, Color color) {
	if (type > 1) {
		Render::Line(x1, y1 - 1, x2, y2 - 1, color);
		Render::Line(x1, y1, x2, y2, color);
		Render::Line(x1, y1 + 1, x2, y2 + 1, color);
		Render::Line(x1, y1 - 2, x2, y2 - 2, color);
		Render::Line(x1, y1 + 2, x2, y2 + 2, color);
	}
	else {
		Render::Line(x1 - 1, y1, x2 - 1, y2, color);
		Render::Line(x1, y1, x2, y2, color);
		Render::Line(x1 + 1, y1, x2 + 1, y2, color);
		Render::Line(x1 - 2, y1, x2 - 2, y2, color);
		Render::Line(x1 + 2, y1, x2 + 2, y2, color);
	}
}
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y | %d | %X", &tstruct);
	return buf;
}
Color urmamasuckmylargegenetalia(int speed, int offset)
{
	float hue = (float)((GetCurrentTime() + offset) % speed);
	hue /= speed;
	return Color::FromHSB(hue, 1.0F, 1.0F);
}
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Options::Menu.VisualsTab.Active.GetState() && Options::Menu.VisualsTab.OtherNoScope.GetState() && strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)) == 0)
		return;
	int w, h;
	int centerW, centerh, topH;
	Interfaces::Engine->GetScreenSize(w, h);
	centerW = w / 2;
	centerh = h / 2;
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, XorStr("MatSystemTopPanel")))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		Interfaces::m_iInputSys->EnableInput(!Options::Menu.m_bIsOpen);

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal->isValidPlayer())
		{
			CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
			CUserCmd* pCmd = cmdlist;
			RECT scrn = Render::GetViewport();
			if (Options::Menu.MiscTab.Radar.GetState())
				DrawRadar();
			if (globalsh.bSendPaket) {
				globalsh.prevChoked = Interfaces::m_pClientState->chokedcommands;
			}
			if (Options::Menu.VisualsTab.LCIndicator.GetIndex() != 0)
			{
				if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() )
				{
					RECT TextSize_2 = Render::GetTextSize(Render::Fonts::LBYIndicator, " Lag Comp Status:");
					bool breaklagcomp = false;
					auto last_origin = pLocal->GetAbsOrigin2();
					if (pLocal->GetAbsOrigin2() != last_origin) {
						if (!(pLocal->GetFlags() & FL_ONGROUND) && pLocal->GetAbsOrigin2().Length2DSqr() > 4096) {
							breaklagcomp = true;
							last_origin = pLocal->GetAbsOrigin2();
						}
					}

					else if (!(pLocal->GetFlags() & FL_ONGROUND) && pLocal->GetVelocity().Length2D() > 125 && GlobalBREAK::bSendPacket && GlobalBREAK::ChokeAmount == c_fakelag->break_lagcomp() || GlobalBREAK::ChokeAmount == 5) {
						breaklagcomp = true;

					}
					else {
						breaklagcomp = false;
					}
					//	Render::Text(6, scrn.bottom - 88, breaklagcomp ? Color(0, 255, 30, 255) : Color(255, 0, 30, 255), Render::Fonts::LBY, "LC");

					if (Options::Menu.VisualsTab.LCIndicator.GetIndex() == 1)
					{
						Render::Text(9, scrn.bottom - 88, breaklagcomp ? Color(0, 255, 30, 255) : Color(255, 0, 30, 255), Render::Fonts::LBY, "LC");
					}


					if (Options::Menu.VisualsTab.LCIndicator.GetIndex() > 1)
					{
						if (breaklagcomp)
						{
							Render::Text(9, centerh + 115, Color(255, 255, 255, 255), Render::Fonts::LBYIndicator, " Lag Comp Status:");
							Render::Text(TextSize_2.left + 11, centerh + 115, Color(0, 90, 250, 255), Render::Fonts::LBYIndicator, "Active");
						}

						if (!breaklagcomp)
						{
							Render::Text(9, centerh + 115, Color(255, 255, 255, 255), Render::Fonts::LBYIndicator, " Lag Comp Status:");
							Render::Text(TextSize_2.left + 11, centerh + 115, Color(255, 0, 80, 255), Render::Fonts::LBYIndicator, "Normal");
						}
					}

				}
			}

			if (Options::Menu.VisualsTab.LBYIndicator.GetIndex() != 0)
			{
				char angle[50];
				sprintf_s(angle, sizeof(angle), "%i", pLocal->GetLowerBodyYaw());

				RECT TextSize = Render::GetTextSize(Render::Fonts::LBY, "LBY");
				RECT TextSize_2 = Render::GetTextSize(Render::Fonts::LBYIndicator, " LBY Status:");
				switch (Options::Menu.VisualsTab.LBYIndicator.GetIndex())
				{
				case 1:
				{
					if (LastAngleAAReal.y - pLocal->GetLowerBodyYaw() >= -35 && LastAngleAAReal.y - pLocal->GetLowerBodyYaw() <= 35)
					{
						Render::Text(9, scrn.bottom - 71, Color(255, 0, 30, 255), Render::Fonts::LBY, "LBY");
					}
					else
					{
						if (LastAngleAAReal.y - pLocal->GetLowerBodyYaw() > 119.f)
						{
							Render::Text(9, scrn.bottom - 71, Color(150, 0, 250, 255), Render::Fonts::LBY, "LBY");
						}
						else
						{
							Render::Text(9, scrn.bottom - 71, Color(0, 250, 30, 255), Render::Fonts::LBY, "LBY");
						}
					}
				}
				break;

				case 2:
				{
					if (LastAngleAAReal.y - pLocal->GetLowerBodyYaw() >= -35 && LastAngleAAReal.y - pLocal->GetLowerBodyYaw() <= 35)
					{
						Render::Text(9, centerh + 100, Color(255, 255, 255, 255), Render::Fonts::LBYIndicator, " LBY Status:");
						Render::Text(TextSize_2.left + 11, centerh + 100, Color(255, 0, 80, 255), Render::Fonts::LBYIndicator, "Normal");
					}
					else
					{
						Render::Text(9, centerh + 100, Color(255, 255, 255, 255), Render::Fonts::LBYIndicator, " LBY Status:");
						Render::Text(TextSize_2.left + 11, centerh + 100, Color(0, 90, 250, 255), Render::Fonts::LBYIndicator, "Broken");
					}
				}
				break;
				}
			}

			if (Options::Menu.VisualsTab.manualaa_type.GetIndex() == 1)
			{
				int W, H, cW, cH;
				Interfaces::Engine->GetScreenSize(W, H);
				cW = W / 2;
				cH = H / 2;
				int rounding = 3;

				if (bigboi::indicator == 1)
				{
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH - 10, 1, Color(Options::Menu.ColorsTab.Menu.GetValue()));// RIGHT | UP
																															   //RIGHT TARROW
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH + 10, 1, Color(Options::Menu.ColorsTab.Menu.GetValue())); //RIGHT DOWN
				}
				if (bigboi::indicator == 2)
				{
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH - 10, 0, Color(Options::Menu.ColorsTab.Menu.GetValue())); //LEFT | UP 
																																//LEFT ARROW
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH + 10, 0, Color(Options::Menu.ColorsTab.Menu.GetValue())); //LEFT | DOWN 

				}
				if (bigboi::indicator == 3)
				{
					drawThiccLine(cW, cH + 45, cW - 10, cH + 35 - rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue()));// DOWN | LEFT
																															   //DOWN ARROW
					drawThiccLine(cW, cH + 45, cW + 10, cH + 35 - rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue())); // DOWN | RIGHT
				}
				if (bigboi::indicator == 4)
				{
					drawThiccLine(cW, cH - 45, cW + 10, cH - 35 + rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue()));// UP | LEFT
																															   //UP ARROW
					drawThiccLine(cW, cH - 45, cW - 10, cH - 35 + rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue())); // UP| RIGHT
				}
			}
			if (Options::Menu.VisualsTab.manualaa_type.GetIndex() > 1)
			{
				int W, H, cW, cH;
				Interfaces::Engine->GetScreenSize(W, H);
				cW = W / 2;
				cH = H / 2;
				int rounding = 3;

				if (bigboi::indicator == 1)
				{
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH - 10, 1, Color(Options::Menu.ColorsTab.Menu.GetValue()));// RIGHT | UP
																															   //RIGHT TARROW
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH + 10, 1, Color(Options::Menu.ColorsTab.Menu.GetValue())); //RIGHT DOWN

																																//------ Other arrows ------//
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH - 10, 0, Color(200, 200, 200, 10)); //LEFT | UP 
																										  //LEFT ARROW
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH + 10, 0, Color(200, 200, 200, 10)); //LEFT | DOWN 

					drawThiccLine(cW, cH + 45, cW - 10, cH + 35 - rounding, 2, Color(200, 200, 200, 10));// DOWN | LEFT
																										 //DOWN ARROW
					drawThiccLine(cW, cH + 45, cW + 10, cH + 35 - rounding, 2, Color(200, 200, 200, 10));// DOWN | RIGHT

					drawThiccLine(cW, cH - 45, cW + 10, cH - 35 + rounding, 2, Color(200, 200, 200, 10));// UP | LEFT
																										 //UP ARROW
					drawThiccLine(cW, cH - 45, cW - 10, cH - 35 + rounding, 2, Color(200, 200, 200, 10)); // UP| RIGHT
				}

				if (bigboi::indicator == 2)
				{
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH - 10, 0, Color(Options::Menu.ColorsTab.Menu.GetValue())); //LEFT | UP 
																																//LEFT ARROW
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH + 10, 0, Color(Options::Menu.ColorsTab.Menu.GetValue())); //LEFT | DOWN 

																																//------ Other arrows ------//
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH - 10, 1, Color(200, 200, 200, 10));// RIGHT | UP
																										 //RIGHT TARROW
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH + 10, 1, Color(200, 200, 200, 10));//RIGHT DOWN

					drawThiccLine(cW, cH + 45, cW - 10, cH + 35 - rounding, 2, Color(200, 200, 200, 10));// DOWN | LEFT
																										 //DOWN ARROW
					drawThiccLine(cW, cH + 45, cW + 10, cH + 35 - rounding, 2, Color(200, 200, 200, 10)); // DOWN | RIGHT

					drawThiccLine(cW, cH - 45, cW + 10, cH - 35 + rounding, 2, Color(200, 200, 200, 10));// UP | LEFT
																										 //UP ARROW
					drawThiccLine(cW, cH - 45, cW - 10, cH - 35 + rounding, 2, Color(200, 200, 200, 10)); // UP| RIGHT
				}

				if (bigboi::indicator == 3)
				{
					drawThiccLine(cW, cH + 45, cW - 10, cH + 35 - rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue()));// DOWN | LEFT
																															   //DOWN ARROW
					drawThiccLine(cW, cH + 45, cW + 10, cH + 35 - rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue())); // DOWN | RIGHT

																																//------ Other arrows ------//
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH - 10, 1, Color(200, 200, 200, 10));// RIGHT | UP
																										 //RIGHT TARROW
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH + 10, 1, Color(200, 200, 200, 10));//RIGHT DOWN

					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH - 10, 0, Color(200, 200, 200, 10)); //LEFT | UP 
																										  //LEFT ARROW
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH + 10, 0, Color(200, 200, 200, 10)); //LEFT | DOWN 

					drawThiccLine(cW, cH - 45, cW + 10, cH - 35 + rounding, 2, Color(200, 200, 200, 10));// UP | LEFT
																										 //UP ARROW
					drawThiccLine(cW, cH - 45, cW - 10, cH - 35 + rounding, 2, Color(200, 200, 200, 10)); // UP| RIGHT
				}

				if (bigboi::indicator == 4)
				{
					drawThiccLine(cW, cH - 45, cW + 10, cH - 35 + rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue()));// UP | LEFT
																															   //UP ARROW
					drawThiccLine(cW, cH - 45, cW - 10, cH - 35 + rounding, 2, Color(Options::Menu.ColorsTab.Menu.GetValue())); // UP| RIGHT

																																//------ Other arrows ------//
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH - 10, 1, Color(200, 200, 200, 10));// RIGHT | UP
																										 //RIGHT TARROW
					drawThiccLine(cW + 45, cH, cW + 35 - rounding, cH + 10, 1, Color(200, 200, 200, 10));//RIGHT DOWN

					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH - 10, 0, Color(200, 200, 200, 10)); //LEFT | UP 
																										  //LEFT ARROW
					drawThiccLine(cW - 45, cH, cW - 35 + rounding, cH + 10, 0, Color(200, 200, 200, 10)); //LEFT | DOWN 

					drawThiccLine(cW, cH + 45, cW - 10, cH + 35 - rounding, 2, Color(200, 200, 200, 10));// DOWN | LEFT
																										 //DOWN ARROW
					drawThiccLine(cW, cH + 45, cW + 10, cH + 35 - rounding, 2, Color(200, 200, 200, 10)); // DOWN | RIGHT
				}

			}

	/*
			if (Options::Menu.VisualsTab.cheatinfo.GetState())
			{
				char jew[64];

				float HC = Options::Menu.RageBotTab.AccuracyHitchance.GetValue();
				float MD = Options::Menu.RageBotTab.AccuracyMinimumDamage.GetValue();
				float blob = Interfaces::m_pClientState->chokedcommands;

				sprintf_s(jew, " Pitch: %.1f", pitchmeme);
				Render::Text(9, (centerh - 80), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);
				sprintf_s(jew, " Yaw: %.1f", lineRealAngle);
				Render::Text(9, (centerh - 65), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);

				sprintf_s(jew, " Lby: %.1f", lineLBY);
				Render::Text(9, (centerh - 50), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);

				sprintf_s(jew, " Real/LBY Delta: %.1f", lineLBY2);
				Render::Text(9, (centerh - 35), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);

				sprintf_s(jew, " Speed: %.1f", lspeed);
				Render::Text(9, (centerh - 20), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);

				sprintf_s(jew, " Choked: %.1f", blob);
				Render::Text(9, (centerh - 5), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);
				sprintf_s(jew, " Innacuracy: %.1f", inaccuracy);
				Render::Text(9, (centerh + 10), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);
				Render::Text(9, (centerh + 25), pLocal->GetFlags() & FL_ONGROUND ? Color(250, 250, 250, 255) : Color(250, 25, 25, 255), Render::Fonts::LBYIndicator, pLocal->GetFlags() & FL_ONGROUND ? " OnGround: True" : " OnGround: False");
				sprintf_s(jew, " Hit Chance: %.f", HC);
				Render::Text(9, (centerh + 40), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);
				sprintf_s(jew, " Min Damage: %.f", MD);
				Render::Text(9, (centerh + 55), Color(250, 250, 250, 255), Render::Fonts::LBYIndicator, jew);
			
			}
			*/
		}
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();
	
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && Options::Menu.VisualsTab.OtherHitmarker.GetState())
		{
			hitmarker::singleton()->on_paint();
			hitmarker_2->paint();
		}
		Options::DoUIFrame();
	}
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
}

#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4    
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR 10

bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);
	if (Options::Menu.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;
		float* m_LocalViewAngles = NULL;
		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}
		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();
		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}
	return result;
}

player_info_t GetInfo(int Index) {
	player_info_t Info;
	Interfaces::Engine->GetPlayerInfo(Index, &Info);
	return Info;
}
typedef void(__cdecl* MsgFn)(const char* msg, va_list);

bool warmup = false;
void draw_hitboxes(IClientEntity* pEntity, int r, int g, int b, int a, float duration, float diameter) {
	matrix3x4 matrix[128];
	if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimulationTime()))
		return;
	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	for (int i = 0; i < set->numhitboxes; i++) {
		mstudiobbox_t* hitbox = set->GetHitbox(i);
		if (!hitbox)
			continue;
		Vector vMin, vMax;
		auto VectorTransform_Wrapperx = [](const Vector& in1, const matrix3x4 &in2, Vector &out)
		{
			auto VectorTransform = [](const float *in1, const matrix3x4& in2, float *out)
			{
				auto DotProducts = [](const float *v1, const float *v2)
				{
					return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				};
				out[0] = DotProducts(in1, in2[0]) + in2[0][3];
				out[1] = DotProducts(in1, in2[1]) + in2[1][3];
				out[2] = DotProducts(in1, in2[2]) + in2[2][3];
			};
			VectorTransform(&in1.x, in2, &out.x);
		};
		VectorTransform_Wrapperx(hitbox->bbmin, matrix[hitbox->bone], vMin);
		VectorTransform_Wrapperx(hitbox->bbmax, matrix[hitbox->bone], vMax);
		Interfaces::DebugOverlay->DrawPill(vMin, vMax, hitbox->m_flRadius, r, g, b, a, duration);
	}
}

auto HitgroupToString = [](int hitgroup) -> std::string
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "HEAD";
	case HITGROUP_CHEST:
		return "CHEST";
	case HITGROUP_STOMACH:
		return "STOMACH";
	case HITGROUP_LEFTARM:
		return "LEFT ARM";
	case HITGROUP_RIGHTARM:
		return "RIGHT ARM";
	case HITGROUP_LEFTLEG:
		return "LEFT LEG";
	case HITGROUP_RIGHTLEG:
		return "RIGHT LEG";
	default:
		return "BODY";
	}
};
void Msg(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}
void ConColorMsg(Color const &color, const char* buf, ...)
{
	using ConColFn = void(__stdcall*)(Color const &, const char*, ...);
	auto ConCol = reinterpret_cast<ConColFn>((GetProcAddress(GetModuleHandle("tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ")));
	ConCol(color, buf);
}

void hitsound()
{
	switch (Options::Menu.MiscTab.hitmarker_sound.GetIndex())
	{
	case 1: PlaySoundA(cod, NULL, SND_ASYNC | SND_MEMORY);
		break;
	case 2: Interfaces::Engine->ClientCmd_Unrestricted("play buttons\\arena_switch_press_02.wav");
		break;
	case 3: PlaySoundA(bubble, NULL, SND_ASYNC | SND_MEMORY);
		break;
	case 4: PlaySoundA(bameware_sound, NULL, SND_ASYNC | SND_MEMORY);
		break;
	case 5: PlaySoundA(anime, NULL, SND_ASYNC | SND_MEMORY);
		break;
	case 6: PlaySoundA(hitler_wav, NULL, SND_ASYNC | SND_MEMORY);
		break;

	}
}

bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event)
{
	CBulletListener::singleton()->OnStudioRender();
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		Color col = Color(Options::Menu.ColorsTab.Menu.GetValue());
		static float kys[3] = { 0.f, 0.f, 0.f };
		kys[0] = col.r() / 255.f;
		kys[1] = col.g() / 255.f;
		kys[2] = col.b() / 255.f;

		std::string event_name = Event->GetName();
		
		
		if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int attackerid = Event->GetInt("attacker");
			int entityid = Interfaces::Engine->GetPlayerForUserID(attackerid);
			if (entityid == Interfaces::Engine->GetLocalPlayer())
			{
				hitsound();
				int nUserID = Event->GetInt("attacker");
				int nDead = Event->GetInt("userid");
				int gaylol = Event->GetInt("hitgroup");
				if (nUserID || nDead)
				{			

					player_info_t killed_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nDead));
					player_info_t killer_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nUserID));
					IClientEntity* hurt = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(Event->GetInt("userid")));

					if (Options::Menu.ColorsTab.DebugLagComp.GetState())
						draw_hitboxes(hurt, 220, 220, 220, 255, 1, 0);

					auto remaining_health = Event->GetString("health");
					int remainaing = Event->GetInt("health");
					auto dmg_to_health = Event->GetString("dmg_health");
					std::string szHitgroup = HitgroupToString(gaylol);
					Interfaces::CVar->ConsoleColorPrintf(Color(160, 5, 240, 255), "Mirror-> ");
					std::string One = "-";
					std::string Two = dmg_to_health;
					std::string Three = " in the ";
					std::string Four = szHitgroup;
					std::string gey = " of ";
					std::string yes = killed_info.name;
					std::string yyes = " [";
					std::string yyyes = " hp: ";
					std::string yyyyes = remaining_health;
					std::string yyyyyes = " ]";
					std::string newline = ".     \n";//no,i'm not retarded, i tried with stringstream but it didn't work.
					std::string uremam = One + Two + Three + Four + gey + yes + yyes + yyyes + yyyyes + yyyyyes + newline;

					if (Options::Menu.VisualsTab.logs.GetState())
					{
						Msg(uremam.c_str());
					}	
					
				}
			}
		}

		if (!strcmp(Event->GetName(), "item_purchase"))
		{
			int nUserID = Event->GetInt("attacker");
			int nDead = Event->GetInt("userid");
			if (nUserID || nDead)
			{
				if (!(localplayer->GetIndex() == Interfaces::Engine->GetPlayerForUserID(nDead))) {
					player_info_t killed_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nDead));
					player_info_t killer_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nUserID));
					Interfaces::CVar->ConsoleColorPrintf(Color(30, 255, 30, 255), "Mirror-> ");
					std::string One = killed_info.name;
					std::string Two = (" bought ");
					std::string Three = Event->GetString("weapon");
					std::string Four = ".     \n";
					if (Options::Menu.VisualsTab.logs.GetState())
					{
						std::string msg = One + Two + Three + Four;
						std::string uremam = msg;
						Msg(uremam.c_str());
					}
				}
			}
		}
		if (Options::Menu.RageBotTab.resolver.GetIndex() > 0 )
		{
			IClientEntity* pLocal = hackManager.pLocal();
			if (!strcmp(Event->GetName(), "weapon_fire")) {
				auto userID = Event->GetInt("userid");
				auto attacker = Interfaces::Engine->GetPlayerForUserID(userID);
				if (attacker) {
					if (attacker == Interfaces::Engine->GetLocalPlayer()) {
						if (Globals::Target) {
							Globals::fired++;
						}
					}
				}
			}
			if (!strcmp(Event->GetName(), "player_hurt")) {
				int deadfag = Event->GetInt("userid");
				int attackingfag = Event->GetInt("attacker");
				if (Interfaces::Engine->GetPlayerForUserID(deadfag) != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) == Interfaces::Engine->GetLocalPlayer()) {
					Globals::hit++;
				}
			}
			if (Globals::fired > 7) {
				Globals::fired = 0;
				Globals::hit = 0;
			}
			Globals::missedshots = Globals::fired - Globals::hit;
		}
	}
	return oFireEventClientSide(ECX, Event);
}
#define TEXTURE_GROUP_LIGHTMAP                      "Lightmaps"
#define TEXTURE_GROUP_WORLD                         "World textures"
#define TEXTURE_GROUP_MODEL                         "Model textures"
#define TEXTURE_GROUP_VGUI                          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE                      "Particle textures"
#define TEXTURE_GROUP_DECAL                         "Decal textures"
#define TEXTURE_GROUP_SKYBOX                        "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS                "ClientEffect textures"
#define TEXTURE_GROUP_OTHER                         "Other textures"
#define TEXTURE_GROUP_PRECACHED                     "Precached"
#define TEXTURE_GROUP_CUBE_MAP                      "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET                 "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED                   "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER           "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP     "Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR    "Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD    "World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS   "Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER    "Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER          "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER         "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER                  "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL                    "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS                 "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS                "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE         "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS                 "Morph Targets"
void draw_hitbox_bt(IClientEntity* pEntity, int r, int g, int b, int a, float duration, float diameter) {
	matrix3x4 matrix[128];
	if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimulationTime()))
		return;
	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	for (int i = 0; i < set->numhitboxes; i++) {
		mstudiobbox_t* hitbox = set->GetHitbox(i);
		if (!hitbox)
			continue;
		Vector vMin, vMax;
		auto VectorTransform_Wrapperx = [](const Vector& in1, const matrix3x4 &in2, Vector &out)
		{
			auto VectorTransform = [](const float *in1, const matrix3x4& in2, float *out)
			{
				auto DotProducts = [](const float *v1, const float *v2)
				{
					return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				};
				out[0] = DotProducts(in1, in2[0]) + in2[0][3];
				out[1] = DotProducts(in1, in2[1]) + in2[1][3];
				out[2] = DotProducts(in1, in2[2]) + in2[2][3];
			};
			VectorTransform(&in1.x, in2, &out.x);
		};
		VectorTransform_Wrapperx(hitbox->bbmin, matrix[hitbox->bone], vMin);
		VectorTransform_Wrapperx(hitbox->bbmax, matrix[hitbox->bone], vMax);
		Interfaces::DebugOverlay->DrawPill(vMin, vMax, diameter, r, g, b, a, duration);
	}
}
void __fastcall  hkSceneEnd(void *pEcx, void *pEdx) 
{
	Hooks::VMTRenderView.GetMethod<SceneEnd_t>(9)(pEcx);

	pSceneEnd(pEcx);

	IClientEntity* pLocalPlayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (pLocalPlayer)
	{
		for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
		{
			auto ent = Interfaces::EntList->GetClientEntity(i);
			if (ent && ent->GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer && ent->IsAlive() && !ent->IsDormant())
			{

				if (Options::Menu.ColorsTab.BackTrackBones2.GetState())
				{
					if (ent->GetVelocity().Length2D() > 25 && pLocalPlayer->IsAlive() && pLocalPlayer->GetIndex() != ent->GetIndex() && ent->team() != pLocalPlayer->team())
						draw_hitbox_bt(ent, Options::Menu.ColorsTab.misc_lagcompBones.GetValue()[0], Options::Menu.ColorsTab.misc_lagcompBones.GetValue()[1], Options::Menu.ColorsTab.misc_lagcompBones.GetValue()[2], 255, 0.2, 0);
				}
				int teamnum = ent->team();

				bool wireframe = false;
				bool glass = false;

				IMaterial * usual = Interfaces::MaterialSystem->FindMaterial("dev/blurfiltery_nohdr", TEXTURE_GROUP_OTHER, true);
				IMaterial * estrogen = Interfaces::MaterialSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL, true);
				IMaterial * vitality = Interfaces::MaterialSystem->FindMaterial("vitality_regular", TEXTURE_GROUP_MODEL, true);
				static IMaterial* wire = CreateMaterial(true, false, true);
				static IMaterial* flat = CreateMaterial(false, true, false);
				static IMaterial* materialLIT = CreateMaterialLit();
				static IMaterial* materialIGNOREZ = CreateMaterialIgnorez();

			}
		}
	
	}
}

void Hooks::DrawBeamd(Vector src, Vector end, Color color)
{
	/*
	float time = Options::Menu.VisualsTab.beamtime.GetValue();
	float size = Options::Menu.VisualsTab.beamsize.GetValue() / 100;
	int r, g, b, a;
	color.GetColor(r, g, b, a);
	BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMLASER;
	beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
	beamInfo.m_nModelIndex = -1; // will be set by CreateBeamPoints if its -1
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = time;
	beamInfo.m_flWidth = 3.5f;
	beamInfo.m_flEndWidth = 2.1f;
	beamInfo.m_flFadeLength = 0.1f;
	beamInfo.m_flAmplitude = 2.3f;
	beamInfo.m_flBrightness = 255.f;
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = Options::Menu.ColorsTab.Bullettracer.GetValue()[0];
	beamInfo.m_flGreen = Options::Menu.ColorsTab.Bullettracer.GetValue()[1];
	beamInfo.m_flBlue = Options::Menu.ColorsTab.Bullettracer.GetValue()[2];
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = 0;
	beamInfo.m_vecStart = src;
	beamInfo.m_vecEnd = end;
	Beam_t* myBeam = Interfaces::g_pViewRenderBeams->CreateBeamPoints(beamInfo);
	if (myBeam)
		Interfaces::g_pViewRenderBeams->DrawBeam(myBeam); 
	*/
	Interfaces::DebugOverlay->AddBoxOverlay(end, Vector(-2.5, -2.5, -2.5), Vector(2.5, 2.5, 2.5),
		QAngle(0, 0, 0), Options::Menu.ColorsTab.Bullettracer.GetValue()[0], Options::Menu.ColorsTab.Bullettracer.GetValue()[1], Options::Menu.ColorsTab.Bullettracer.GetValue()[2], Options::Menu.ColorsTab.Bullettracer.GetValue()[3], 1.25f);

}
bool random() {
	if (rand() % 2 == 0)
		return true;
	else return false;
}
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	Color color;
	float flColor[3] = { 0.f };
	bool DontDraw = false;
	static IMaterial* mat = CreateMaterialLit();
	int HandsStyle = Options::Menu.VisualsTab.HandCHAMS.GetIndex();

	int gunstyle = Options::Menu.VisualsTab.GunCHAMS.GetIndex();

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);

	IClientEntity* pentity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);

	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	float blend = Options::Menu.VisualsTab.transparency.GetValue() / 100;
	float hand_blend = Options::Menu.VisualsTab.hand_transparency.GetValue() / 100;
	float gun_blend = Options::Menu.VisualsTab.gun_transparency.GetValue() / 100;

	float blend_vis = Options::Menu.VisualsTab.enemy_blend.GetValue() / 100;
	float blend_invis = Options::Menu.VisualsTab.enemy_blend_invis.GetValue() / 100;
	float blend_local = Options::Menu.VisualsTab.blend_local.GetValue() / 100;

	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	static IMaterial* wire = CreateMaterial(true, false, true);
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);

	int ChamsStyle = Options::Menu.VisualsTab.OptionsChams.GetIndex();
	IMaterial *covered = CoveredFlat;	
	IMaterial *open = OpenLit;

	Color color_invis = Color(Options::Menu.ColorsTab.ChamsEnemyNotVis.GetValue());
	Color color_vis = Color(Options::Menu.ColorsTab.ChamsEnemyVis.GetValue());
	Color color_team = Color(Options::Menu.ColorsTab.ChamsTeamVis.GetValue());

	float fl_color[3] = { 0.f };

	float fl_color2[3] = { 0.f };

	float fl_color3[3] = { 0.f };

	float fl_color4[3] = { 0.f };

	if (pentity->isValidPlayer() && pentity->team() != pLocal->team() && strstr(ModelName, "models/player") && pentity->IsAlive())
	{

		if (Options::Menu.VisualsTab.ChamsEnemyNoVis.GetState())
		{
			fl_color[0] = color_invis.r() / 255.f;
			fl_color[1] = color_invis.g() / 255.f;
			fl_color[2] = color_invis.b() / 255.f;

			Interfaces::RenderView->SetColorModulation(fl_color);
			Interfaces::RenderView->SetBlend(blend_vis);
			Interfaces::ModelRender->ForcedMaterialOverride(covered);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

		}

		if (Options::Menu.VisualsTab.ChamsEnemyVis.GetState())
		{
			fl_color[0] = color_vis.r() / 255.f;
			fl_color[1] = color_vis.g() / 255.f;
			fl_color[2] = color_vis.b() / 255.f;

			Interfaces::RenderView->SetColorModulation(fl_color);
			Interfaces::RenderView->SetBlend(blend_invis);
			Interfaces::ModelRender->ForcedMaterialOverride(open);			
		}
	
	}

	if (pentity && pentity->GetIndex() != pLocal->GetIndex() && pentity->team() == pLocal->team() && strstr(ModelName, "models/player"))
	{
	
		if (Options::Menu.VisualsTab.ChamsTeamVis.GetState())
		{
			
			fl_color4[0] = color_team.r() / 255.f;
			fl_color4[1] = color_team.g() / 255.f;
			fl_color4[2] = color_team.b() / 255.f;

			Interfaces::RenderView->SetColorModulation(fl_color4);
			Interfaces::RenderView->SetBlend(1.f);
			Interfaces::ModelRender->ForcedMaterialOverride(open);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}

	if (!pLocal->IsScoped() && strstr(ModelName, "models/player") && pentity != nullptr &&  pentity->GetIndex() == pLocal->GetIndex() && Options::Menu.VisualsTab.ChamsLocal.GetState())
	{
		Color color_local = Color(Options::Menu.ColorsTab.ChamsLocal.GetValue());
		fl_color3[0] = color_local.r() / 255.f;
		fl_color3[1] = color_local.g() / 255.f;
		fl_color3[2] = color_local.b() / 255.f;

		Interfaces::RenderView->SetColorModulation(fl_color3);
		Interfaces::RenderView->SetBlend(blend_local);
		Interfaces::ModelRender->ForcedMaterialOverride(covered);
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

		fl_color3[0] = color_local.r() / 255.f;
		fl_color3[1] = color_local.g() / 255.f;
		fl_color3[2] = color_local.b() / 255.f;

		Interfaces::RenderView->SetColorModulation(fl_color3);
		Interfaces::RenderView->SetBlend(blend_local);
		Interfaces::ModelRender->ForcedMaterialOverride(open);
	//	oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	} 

	if (pLocal->IsScoped() && strstr(ModelName, "models/player") && pentity != nullptr && pentity == pLocal)
	{
		switch (Options::Menu.VisualsTab.localmaterial.GetIndex())
		{
		
		case 1:
		{
			Interfaces::RenderView->SetBlend(blend);
		}
		break;

		case 2:
		{
		
				flColor[0] = Options::Menu.ColorsTab.scoped_c.GetValue()[0] / 255.f;
				flColor[1] = Options::Menu.ColorsTab.scoped_c.GetValue()[1] / 255.f;
				flColor[2] = Options::Menu.ColorsTab.scoped_c.GetValue()[2] / 255.f;

				Interfaces::RenderView->SetBlend(blend);
				Interfaces::RenderView->SetColorModulation(flColor);
				Interfaces::ModelRender->ForcedMaterialOverride(covered);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
			
		}
		break;

		case 3:
		{
			
				flColor[0] = Options::Menu.ColorsTab.scoped_c.GetValue()[0] / 255.f;
				flColor[1] = Options::Menu.ColorsTab.scoped_c.GetValue()[1] / 255.f;
				flColor[2] = Options::Menu.ColorsTab.scoped_c.GetValue()[2] / 255.f;
				Interfaces::RenderView->SetColorModulation(flColor);
				Interfaces::RenderView->SetBlend(blend);
				Interfaces::ModelRender->ForcedMaterialOverride(wire);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
			
		}
		break;

		case 4:
		{
			
				flColor[0] = rand() % 250 / 255.f;
				flColor[1] = rand() % 250 / 255.f;
				flColor[2] = rand() % 250 / 255.f;
				Interfaces::RenderView->SetColorModulation(flColor);
				Interfaces::RenderView->SetBlend(blend);
				Interfaces::ModelRender->ForcedMaterialOverride(covered);
				oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
			
		}
		break;

		case 5:
		{

			flColor[0] = 5 / 255.f;
			flColor[1] = 5 / 255.f;
			flColor[2] = 5 / 255.f;
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::RenderView->SetBlend(blend);
			Interfaces::ModelRender->ForcedMaterialOverride(OpenFlat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

		}
		break;

		case 6:
		{
			Interfaces::RenderView->SetBlend(0.2f);
		}
		break;

		}
	}
	if (gunstyle != 0 && strstr(ModelName, "models/weapons/v_") && !strstr(ModelName, "arms") && pLocal && pLocal->IsAlive())
	{
		switch (gunstyle) // this shit was done in "else if"s with " == " instead of A > B or switch statements which is fucking HECKIN trash for optimization
		{
		case 1:
		{
			flColor[0] = Options::Menu.ColorsTab.GunChamsCol.GetValue()[0] / 255.f;
			flColor[1] = Options::Menu.ColorsTab.GunChamsCol.GetValue()[1] / 255.f;
			flColor[2] = Options::Menu.ColorsTab.GunChamsCol.GetValue()[2] / 255.f;
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::RenderView->SetBlend(gun_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

		case 2:
		{
			flColor[0] = Options::Menu.ColorsTab.GunChamsCol.GetValue()[0] / 255.f;
			flColor[1] = Options::Menu.ColorsTab.GunChamsCol.GetValue()[1] / 255.f;
			flColor[2] = Options::Menu.ColorsTab.GunChamsCol.GetValue()[2] / 255.f;
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			Interfaces::RenderView->SetBlend(gun_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;
/*
		case 3:
		{
			flColor[0] = 234 / 255.f;
			flColor[1] = 234 / 255.f;
			flColor[2] = 100 / 255.f;
			Interfaces::RenderView->SetBlend(0);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;
*/
		case 3:
		{
			Interfaces::RenderView->SetBlend(gun_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			IMaterial *material = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
			Interfaces::ModelRender->ForcedMaterialOverride(material);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

		case 4:
		{
			Interfaces::RenderView->SetBlend(gun_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			IMaterial *material = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
			Interfaces::ModelRender->ForcedMaterialOverride(material);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

		case 5:
		{
			Interfaces::RenderView->SetBlend(1.f);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPSPHERE, true);
			Color colRainbow = urmamasuckmylargegenetalia(5000, 1022);
			flColor[0] = colRainbow.r() / 255.f;
			flColor[1] = colRainbow.g() / 255.f;
			flColor[2] = colRainbow.b() / 255.f;
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;
		}
	}
	if (HandsStyle != 0 && strstr(ModelName, XorStr("arms")) && pLocal && pLocal->IsAlive())
	{
		switch (HandsStyle)
		{
		case 1:
		{
			flColor[0] = Options::Menu.ColorsTab.HandChamsCol.GetValue()[0] / 255.f;
			flColor[1] = Options::Menu.ColorsTab.HandChamsCol.GetValue()[1] / 255.f;
			flColor[2] = Options::Menu.ColorsTab.HandChamsCol.GetValue()[2] / 255.f;
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::RenderView->SetBlend(hand_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

		case 2:
		{
			flColor[0] = Options::Menu.ColorsTab.HandChamsCol.GetValue()[0] / 255.f;
			flColor[1] = Options::Menu.ColorsTab.HandChamsCol.GetValue()[1] / 255.f;
			flColor[2] = Options::Menu.ColorsTab.HandChamsCol.GetValue()[2] / 255.f;
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			Interfaces::RenderView->SetBlend(hand_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

	/*	case 3:
		{
			flColor[0] = 234 / 255.f;
			flColor[1] = 234 / 255.f;
			flColor[2] = 100 / 255.f;
			Interfaces::RenderView->SetBlend(0);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	//		mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			Interfaces::RenderView->SetColorModulation(flColor);
	//		Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break; */

		case 3:
		{
			Interfaces::RenderView->SetBlend(1.f);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			IMaterial *material = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
			Interfaces::ModelRender->ForcedMaterialOverride(material);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

		case 4:
		{
			Interfaces::RenderView->SetBlend(1.f);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			IMaterial *material = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
			Interfaces::ModelRender->ForcedMaterialOverride(material);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;

		case 6:
		{
			Interfaces::RenderView->SetBlend(hand_blend);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_ENVMAPSPHERE, true);
			Color colRainbow = urmamasuckmylargegenetalia(5000, 1022);
			flColor[0] = colRainbow.r() / 255.f;
			flColor[1] = colRainbow.g() / 255.f;
			flColor[2] = colRainbow.b() / 255.f;
			Interfaces::RenderView->SetColorModulation(flColor);
			Interfaces::ModelRender->ForcedMaterialOverride(mat);
			oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
		break;
		}
	}

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}
int RandomInt(int min, int max)
{
	return rand() % max + min;
}
std::vector<const char*> vistasmoke_mats =
{
	"particle/beam_smoke_01",
	"particle/particle_smokegrenade",
	"particle/particle_smokegrenade1",
	"particle/particle_smokegrenade2",
	"particle/particle_smokegrenade3",
	"particle/particle_smokegrenade_sc",
	"particle/smoke1/smoke1",
	"particle/smoke1/smoke1_ash",
	"particle/smoke1/smoke1_nearcull",
	"particle/smoke1/smoke1_nearcull2",
	"particle/smoke1/smoke1_snow",
	"particle/smokesprites_0001",
	"particle/smokestack",
	"particle/vistasmokev1/vistasmokev1",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_nearcull",
	"particle/vistasmokev1/vistasmokev1_nearcull_fog",
	"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev4_emods_nocull",
	"particle/vistasmokev1/vistasmokev4_nearcull",
	"particle/vistasmokev1/vistasmokev4_nocull"
};

class CBaseAnimating
{
public:
	std::array<float, 24>* m_flPoseParameter()
	{
		static int offset = 0;
		if (!offset)
			offset = 0x2764;
		return (std::array<float, 24>*)((uintptr_t)this + offset);
	}
	model_t* GetModel()
	{
		void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
		typedef model_t* (__thiscall* fnGetModel)(void*);
		return call_vfunc<fnGetModel>(pRenderable, 8)(pRenderable);
	}
	
	void SetBoneMatrix(matrix3x4_t* boneMatrix)
	{
		//Offset found in C_BaseAnimating::GetBoneTransform, string search ankle_L and a function below is the right one
		const auto model = this->GetModel();
		if (!model)
			return;
		matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
		studiohdr_t *hdr = Interfaces::ModelInfo->GetStudiomodel(model);
		if (!hdr)
			return;
		int size = hdr->numbones;
		if (matrix) {
			for (int i = 0; i < size; i++)
				memcpy(matrix + i, boneMatrix + i, sizeof(matrix3x4_t));
		}
	}
	void GetDirectBoneMatrix(matrix3x4_t* boneMatrix)
	{
		const auto model = this->GetModel();
		if (!model)
			return;
		matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
		studiohdr_t *hdr = Interfaces::ModelInfo->GetStudiomodel(model);
		if (!hdr)
			return;
		int size = hdr->numbones;
		if (matrix) {
			for (int i = 0; i < size; i++)
				memcpy(boneMatrix + i, matrix + i, sizeof(matrix3x4_t));
		}
	}
};

void UpdateIncomingSequences()
{
	auto clientState = Interfaces::m_pClientState; //DONT HARDCODE OFFESTS

	if (!clientState)
		return;

	auto intnetchan = clientState->m_NetChannel; //Can optimise, already done in CM hook, make a global

	INetChannel* netchan = reinterpret_cast<INetChannel*>(intnetchan);
	if (netchan)
	{
		if (netchan->m_nInSequenceNr > lastincomingsequencenumber)
		{
			//sequences.push_front(netchan->m_nInSequenceNr);
			lastincomingsequencenumber = netchan->m_nInSequenceNr;

			sequences.push_front(CIncomingSequence(netchan->m_nInReliableState, netchan->m_nOutReliableState, netchan->m_nInSequenceNr, Interfaces::Globals->realtime));
		}
		if (sequences.size() > 2048)
			sequences.pop_back();
	}
}

auto smoke_count = *(DWORD*)(Utilities::Memory::FindPatternV2("client_panorama", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0") + 0x8);

void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	IClientEntity* pEntity = nullptr;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		for (int i = 0; i < Interfaces::Globals->maxClients; i++)
		{
			auto m_entity = Interfaces::EntList->GetClientEntity(i);
			if (!m_entity || m_entity == pLocal || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive() && curStage != FRAME_NET_UPDATE_END) continue;
			{
				lagcompensation.disable_interpolation(m_entity);
			}

			if (i == Interfaces::Engine->GetLocalPlayer()) 
				continue;

			IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
			if (!pCurEntity)
				continue;

			CTickRecord trans = CTickRecord(pCurEntity);
			CBacktracking::Get().ClearRecord(pCurEntity);
			CBacktracking::Get().SaveTemporaryRecord(pCurEntity, trans);

			*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount;
			*(int*)((uintptr_t)pCurEntity + 0xA28) = 0; 
		}

		if (Options::Menu.MiscTab.OtherThirdperson.GetState() && pLocal)
		{

			static bool enabledtp = false, check = false;

			if (GetAsyncKeyState(Options::Menu.MiscTab.ThirdPersonKeyBind.GetKey()) && hackManager.pLocal()->IsAlive())
			{
				if (!check)
					enabledtp = !enabledtp;
				check = true;
			}
			else
				check = false;

			if (enabledtp)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				*reinterpret_cast<Vector*>(reinterpret_cast<DWORD>(hackManager.pLocal()) + 0x31D8) = LastAngleAAReal;
				pLocal->SetAbsAngles(Vector(0.f, pLocal->GetBasePlayerAnimState()->m_flGoalFeetYaw, 0.f));
			}
			auto local_player = hackManager.pLocal();
			IClientEntity* obstarget =Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetObserverTargetHandle());
			if (Interfaces::pInput->m_fCameraInThirdPerson)
			{
				//	Interfaces::Prediction1->set_local_viewangles_rebuilt(LastAngleAAReal);
				Vector viewangs = *(Vector*)(reinterpret_cast<uintptr_t>(pLocal) + 0x31D8); viewangs = LastAngleAAReal;
			}
		//	bool set = false;
			if (enabledtp && pLocal->IsAlive())
			{			
				Interfaces::pInput->m_fCameraInThirdPerson = true;
				Vector camForward;
			}
		
			else
			{
				Interfaces::pInput->m_fCameraInThirdPerson = false;
			}

		}
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			UpdateIncomingSequences();
			if (Options::Menu.RageBotTab.lag_pred.GetState())
			{
				CMBacktracking::Get().StartLagCompensation(pLocal);
				CMBacktracking::Get().FrameUpdatePostEntityThink();
			} 
		}
		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_END) 
		{
			if (Options::Menu.RageBotTab.lag_pred.GetState())
			{
				CMBacktracking::Get().FrameUpdatePostEntityThink();
				CMBacktracking::Get().FinishLagCompensation(pLocal);
			}
		}
		if (curStage == FRAME_RENDER_START)
		{		
		//	animation_fix();
			for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
			{
				if (i == Interfaces::Engine->GetLocalPlayer()) continue;
				IClientEntity* pEnt = Interfaces::EntList->GetClientEntity(i);
				if (!pEnt) continue;
				*(int*)((uintptr_t)pEnt + 0xA30) = Interfaces::Globals->framecount;
				*(int*)((uintptr_t)pEnt + 0xA28) = 0;
			}

			if (Options::Menu.VisualsTab.nosmoke.GetState())
			{
				for (auto matName : vistasmoke_mats)
				{
					IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(matName, "Other textures");
					mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
					*(int*)smoke_count = 0;
				}
			}
		}
		if (curStage == FRAME_NET_UPDATE_END)
		{
			if (Options::Menu.RageBotTab.lag_pred.GetState())
			{
				CMBacktracking::Get().FrameUpdatePostEntityThink();
		//		lagcomp->frame_net_update_end();
			}
		}
			
		resolver_setup::GetInst().FSN(pEntity, curStage);
		
	}
	oFrameStageNotify(curStage);
}
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	auto local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!local || !Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	if (local)
	{
		auto zoom = Options::Menu.VisualsTab.OtherFOV.GetValue();

		if (local->IsScoped() && Options::Menu.VisualsTab.RemoveZoom.GetState()) {
			zoom += 90.0f - pSetup->fov;
		}
		pSetup->fov += zoom;
	}
	oOverrideView(ecx, edx, pSetup);
}
void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (!localplayer)
			return;
		if (Options::Menu.VisualsTab.Active.GetState())
			fov = Options::Menu.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}
float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();
	GetViewModelFOV(fov);
	return fov;
}
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
}
