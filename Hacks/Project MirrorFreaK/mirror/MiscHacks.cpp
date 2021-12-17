#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "IClientMode.h"
#include <chrono>
#include <algorithm>
#include <time.h>
#include "Hooks.h"
#include "position_adjust.h"
#include "RageBot.h"
#include "Autowall.h"
CMiscHacks* g_Misc = new CMiscHacks;
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
inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}
inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}
Vector AutoStrafeView;
void CMiscHacks::Init()
{
}
void CMiscHacks::Draw()
{
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

}

void set_name(const char* name)
{
	ConVar* nameConvar = Interfaces::CVar->FindVar(("name"));
	*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = NULL;
	nameConvar->SetValueChar(name);
}
void CMiscHacks::namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < .5)
		return;
	const char* result;
	std::vector <std::string> names;
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
		for (int i = 1; i < Interfaces::Globals->maxClients; i++)
		{
			IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

			player_info_t pInfo;

			if (entity && hackManager.pLocal()->team() == entity->team())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							names.push_back(pInfo.name);
					}
				}
			}
		}
	}

	set_name("\n\xAD\xAD\xAD");
	int randomIndex = rand() % names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", names[randomIndex].c_str());
	result = buffer;



	set_name(result);
	start_t = clock();

}
/*
template<class T>
static T* FindHudElement(const char* name)
{
	
	static auto pThis = *reinterpret_cast<DWORD**>(Utilities::Memory::FindPatternV2("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 8D 58") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));
		
	if (find_hud_element != nullptr)
	{
		return (T*)find_hud_element(pThis, name);
	}
	
}
void preservefeed(IGameEvent* Event) 
{
	if (hackManager.pLocal()->IsAlive())
	{
		static DWORD* _death_notice = FindHudElement<DWORD>("CCSGO_HudDeathNotice");

		if (_death_notice == nullptr)
			return;

		static void(__thiscall *_clear_notices)(DWORD) = (void(__thiscall*)(DWORD))Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");

		if (round_change)
		{

			_death_notice = FindHudElement<DWORD>("CCSGO_HudDeathNotice");
			if (_death_notice - 20)
				_clear_notices(((DWORD)_death_notice - 20));
			round_change = false;
		}

		if (_death_notice)
			*(float*)((DWORD)_death_notice + 0x50) = Options::Menu.VisualsTab.killfeed.GetState() ? 100 : 1;
	}
}*/
void CMiscHacks::buybot_primary()
{
	bool is_ct = hackManager.pLocal()->team() == TEAM_CS_CT;
	switch (Options::Menu.MiscTab.buybot_primary.GetIndex())
	{
	case 1: is_ct ? (Interfaces::Engine->ExecuteClientCmd("buy scar20;")) : (Interfaces::Engine->ExecuteClientCmd("buy g3sg1;"));
		break;
	case 2: Interfaces::Engine->ExecuteClientCmd("buy ssg08;");
		break;
	case 3: Interfaces::Engine->ExecuteClientCmd("buy awp;");
		break;
	case 4: is_ct ? (Interfaces::Engine->ExecuteClientCmd("buy m4a1; buy m4a1_silencer")) : (Interfaces::Engine->ExecuteClientCmd("buy ak47;"));
		break;
	case 5: is_ct ? (Interfaces::Engine->ExecuteClientCmd("buy aug;")) : (Interfaces::Engine->ExecuteClientCmd("buy sg556"));
		break;
	case 6: is_ct ? (Interfaces::Engine->ExecuteClientCmd("buy mp9")) : (Interfaces::Engine->ExecuteClientCmd("buy mac-10;"));
		break;
	}

}

void CMiscHacks::buybot_secondary()
{
	switch (Options::Menu.MiscTab.buybot_secondary.GetIndex())
	{
	case 1: Interfaces::Engine->ExecuteClientCmd("buy elite;");
		break;
	case 2: Interfaces::Engine->ExecuteClientCmd("buy deagle;");
		break;
	case 3: Interfaces::Engine->ExecuteClientCmd("buy fn57;");
		break;
	}

}

void CMiscHacks::buybot_otr()
{
	std::vector<dropdownboxitem> otr_list = Options::Menu.MiscTab.buybot_otr.items;

	if (otr_list[0].GetSelected) 
	{
		Interfaces::Engine->ExecuteClientCmd("buy vest; buy vesthelm;");
	}

	if (otr_list[1].GetSelected)
	{
		Interfaces::Engine->ExecuteClientCmd("buy hegrenade;");
	}

	if (otr_list[2].GetSelected)
	{
		Interfaces::Engine->ExecuteClientCmd("buy flashbang;");
	}

	if (otr_list[3].GetSelected)
	{
		Interfaces::Engine->ExecuteClientCmd("buy smokegrenade;");
	}

	if (otr_list[4].GetSelected)
	{
		Interfaces::Engine->ExecuteClientCmd("buy molotov;");
	}

	if (otr_list[5].GetSelected)
	{
		Interfaces::Engine->ExecuteClientCmd("buy taser;");
	}

	if (otr_list[67].GetSelected)
	{
		Interfaces::Engine->ExecuteClientCmd("buy defuser;");
	}

}
inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	IGameEvent* Event;
	if (!hackManager.pLocal()->IsAlive())
	{
		_done = false;
		return;
	}
	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	// ------- Oi thundercunt, this is needed for the weapon configs ------- //

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());


	if (pWeapon != nullptr)
	{
		if (GetAsyncKeyState(Options::Menu.MiscTab.minimal_walk.GetKey()))
		{
			MinimalWalk(pCmd, get_gun(pWeapon));
		}

		if (GameUtils::AutoSniper(pWeapon))
		{

			Options::Menu.RageBotTab.AccuracyHitchance.SetValue((float)Options::Menu.RageBotTab.hc_auto.GetValue());
			Options::Menu.RageBotTab.AccuracyMinimumDamage.SetValue((float)Options::Menu.RageBotTab.md_auto.GetValue());

		}

		if (GameUtils::IsPistol(pWeapon))
		{

			Options::Menu.RageBotTab.AccuracyHitchance.SetValue((float)Options::Menu.RageBotTab.hc_pistol.GetValue());
			Options::Menu.RageBotTab.AccuracyMinimumDamage.SetValue((float)Options::Menu.RageBotTab.md_pistol.GetValue());

		}

		if (pWeapon->isScout())
		{

			Options::Menu.RageBotTab.AccuracyHitchance.SetValue((float)Options::Menu.RageBotTab.hc_scout.GetValue());
			Options::Menu.RageBotTab.AccuracyMinimumDamage.SetValue((float)Options::Menu.RageBotTab.md_scout.GetValue());

		}

		if (pWeapon->isAwp())
		{

			Options::Menu.RageBotTab.AccuracyHitchance.SetValue((float)Options::Menu.RageBotTab.hc_awp.GetValue());
			Options::Menu.RageBotTab.AccuracyMinimumDamage.SetValue((float)Options::Menu.RageBotTab.md_awp.GetValue());

		}

		if (GameUtils::IsRifle(pWeapon) || GameUtils::IsShotgun(pWeapon) || GameUtils::IsMP(pWeapon) || GameUtils::IsMachinegun(pWeapon))
		{

			Options::Menu.RageBotTab.AccuracyHitchance.SetValue((float)Options::Menu.RageBotTab.hc_otr.GetValue());
			Options::Menu.RageBotTab.AccuracyMinimumDamage.SetValue((float)Options::Menu.RageBotTab.md_otr.GetValue());

		}

		if (GameUtils::IsZeus(pWeapon))
		{
			Options::Menu.RageBotTab.AccuracyHitchance.SetValue(1);
			Options::Menu.RageBotTab.AccuracyMinimumDamage.SetValue(20);
		}
	}

	if (Options::Menu.MiscTab.infinite_duck.GetState())
	{
		pCmd->buttons |= IN_BULLRUSH;
	}

	if (Options::Menu.VisualsTab.override_viewmodel.GetState())
		viewmodel_x_y_z();

	if (Options::Menu.RageBotTab.AimbotEnable.GetState())
		AutoPistol(pCmd);
	if (Options::Menu.MiscTab.OtherAutoJump.GetState())
		AutoJump(pCmd);
	if (Options::Menu.MiscTab.airduck_type.GetIndex() != 0)
	{
		airduck(pCmd);
	}
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	if (Options::Menu.MiscTab.OtherAutoStrafe.GetState())
	{
		strafer(pCmd);
	}

	if (GetAsyncKeyState(Options::Menu.MiscTab.fw.GetKey()) && !Options::Menu.m_bIsOpen)
	{
		FakeWalk0(pCmd, bSendPacket);
	}

	fake_crouch(pCmd, bSendPacket);

	if (Options::Menu.VisualsTab.DisablePostProcess.GetState())
		PostProcces();

	if (!_done && hackManager.pLocal()->IsAlive())
	{
		buybot_otr();

		if (Options::Menu.MiscTab.buybot_primary.GetIndex() != 0)
			buybot_primary();

		if (Options::Menu.MiscTab.buybot_secondary.GetIndex() != 0)
			buybot_secondary();

		_done = true;
	}

	if (Options::Menu.MiscTab.NameChanger.GetState())
	{
		namespam();
	}
}
int GetFxPS()
{
	static int fps = 0;
	static int count = 0;
	using namespace std::chrono;
	auto now = high_resolution_clock::now();
	static auto last = high_resolution_clock::now();
	count++;
	if (duration_cast<milliseconds>(now - last).count() > 1000)
	{
		fps = count;
		count = 0;
		last = now;
	}
	return fps;
}
float curtime_fixedx(CUserCmd* ucmd) {
	auto local_player = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::Globals->interval_per_tick;
	return curtime;
}

void VectorAnglesXXX(Vector forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward[2] == 0 && forward[0] == 0)
	{
		yaw = 0;

		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / PI);

		if (yaw < 0)
			yaw += 360;
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);

		if (pitch < 0)
			pitch += 360;
	}

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
Vector CalcAngleFakewalk(Vector src, Vector dst)
{
	Vector ret;
	VectorAnglesXXX(dst - src, ret);
	return ret;
}

void rotate_movement(float yaw, CUserCmd* cmd)
{
	Vector viewangles;
	QAngle yamom;
	Interfaces::Engine->GetViewAngles(viewangles);
	float rotation = DEG2RAD(viewangles.y - yaw);
	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);
	float new_forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
	float new_sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);
	cmd->forwardmove = new_forwardmove;
	cmd->sidemove = new_sidemove;
}

float fakewalk_curtime(CUserCmd* ucmd)
{
	auto local_player = hackManager.pLocal();

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted)
	{
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::Globals->interval_per_tick;
	return curtime;
}
void CMiscHacks::FakeWalk0(CUserCmd* pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	globalsh.fakewalk = true;
	static int iChoked = -1;
	iChoked++;
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
	static int choked = 0;
	choked = choked > 14 ? 0 : choked + 1;

	float nani = Options::Menu.MiscTab.FakeWalkSpeed.GetValue() / 14;

	pCmd->forwardmove = choked < nani || choked > 14 ? 0 : pCmd->forwardmove;
	pCmd->sidemove = choked < nani || choked > 14 ? 0 : pCmd->sidemove; //100:6 are about 16,6, quick maths
	bSendPacket = choked < 1;
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

void CMiscHacks::AutoPistol(CUserCmd* pCmd)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	static bool WasFiring = false;
	if (GameUtils::IsPistol(pWeapon) && !GameUtils::IsBomb(pWeapon))
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
	else
		return;
}
void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	auto g_LocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto userCMD = pCmd;
	if (Options::Menu.MiscTab.autojump_type.GetIndex() < 1)
	{	
		if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER) return;
		if (userCMD->buttons & IN_JUMP && !(g_LocalPlayer->GetFlags() & FL_ONGROUND)) 
		{
			userCMD->buttons &= ~IN_JUMP;
		}
	}
	if (Options::Menu.MiscTab.autojump_type.GetIndex() > 0)
	{
		if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER) 
			return;
		userCMD->buttons |= IN_JUMP;
	}
}
void CMiscHacks::airduck(CUserCmd *pCmd) // quack
{
	auto local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	
	if (Options::Menu.MiscTab.airduck_type.GetIndex() == 1)
	{
		if (!(local->GetFlags() & FL_ONGROUND))
		{
			pCmd->buttons |= IN_DUCK;
		}
	}
	if (Options::Menu.MiscTab.airduck_type.GetIndex() == 2)
	{
		if (!(local->GetFlags() & FL_ONGROUND))
		{
			pCmd->buttons |= IN_DUCK;
			pCmd->buttons &= IN_DUCK;
			pCmd->buttons |= IN_DUCK;
			pCmd->buttons &= IN_DUCK;
		}
	}
}
template<class T, class U>
inline T clampangle(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
void CMiscHacks::RageStrafe(CUserCmd *userCMD)
{
	auto g_LocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());;
	if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER || !g_LocalPlayer->IsAlive()) return;
	if (!Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_SPACE) ||
		Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_A) ||
		Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_D) ||
		Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_S) ||
		Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_W))
		return;
	if (!(g_LocalPlayer->GetFlags() & FL_ONGROUND)) {
		if (userCMD->mousedx > 1 || userCMD->mousedx < -1) {
			userCMD->sidemove = clamp(userCMD->mousedx < 0.f ? -400.f : 400.f, -400, 400);
		}
		else {
			if (g_LocalPlayer->GetVelocity().Length2D() == 0 || g_LocalPlayer->GetVelocity().Length2D() == NAN || g_LocalPlayer->GetVelocity().Length2D() == INFINITE)
			{
				userCMD->forwardmove = 400;
				return;
			}
			userCMD->forwardmove = clamp(5850.f / g_LocalPlayer->GetVelocity().Length2D(), -400, 400);
			if (userCMD->forwardmove < -400 || userCMD->forwardmove > 400)
				userCMD->forwardmove = 0;
			userCMD->sidemove = clamp((userCMD->command_number % 2) == 0 ? -400.f : 400.f, -400, 400);
			if (userCMD->sidemove < -400 || userCMD->sidemove > 400)
				userCMD->sidemove = 0;
		}
	}
}

#define nn(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
bool bHasGroundSurface(IClientEntity* pLocalBaseEntity, const Vector& vPosition)
{
	trace_t pTrace;
	Vector vMins, vMaxs; pLocalBaseEntity->GetRenderBounds(vMins, vMaxs);

	UTIL_TraceLine(vPosition, { vPosition.x, vPosition.y, vPosition.z - 32.f }, MASK_PLAYERSOLID_BRUSHONLY, pLocalBaseEntity, 0, &pTrace);

	return pTrace.fraction != 1.f;
}

void CMiscHacks::strafe_2(CUserCmd * cmd)
{
	auto local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (local->GetMoveType() == MOVETYPE_NOCLIP || local->GetMoveType() == MOVETYPE_LADDER || local->is_ghost() || !local || !local->IsAlive())
		return;

	if (Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_A) || Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_D) || Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_S) || Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_W) || Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::KEY_LSHIFT))
		return;

	if (!(local->GetFlags() & FL_ONGROUND)) {
		if (cmd->mousedx > 1 || cmd->mousedx < -1) {
			cmd->sidemove = clamp(cmd->mousedx < 0.f ? -450.0f : 450.0f, -450.0f, 450.0f);
		}
		else {
			cmd->forwardmove = 10000.f / local->GetVelocity().Length();
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.0f : 450.0f;
			if (cmd->forwardmove > 450.0f)
				cmd->forwardmove = 450.0f;
		}
	}
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

void CMiscHacks::PostProcces()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::MinimalWalk(CUserCmd* cmd, float speed)
{
	if (speed <= 0.f)
		return;

	float fSpeed = (float)(FastSqrt(square(cmd->forwardmove) + square(cmd->sidemove) + square(cmd->upmove)));

	if (fSpeed <= 0.f)
		return;

	if (cmd->buttons & IN_DUCK)
		speed *= 2.94117647f;

	if (fSpeed <= speed)
		return;

	float fRatio = speed / fSpeed;

	cmd->forwardmove *= fRatio;
	cmd->sidemove *= fRatio;
	cmd->upmove *= fRatio;
}
void CMiscHacks::fake_crouch(CUserCmd * cmd, bool &packet)  // appears pasted
{
	static bool counter = false;
	if (GetAsyncKeyState(Options::Menu.MiscTab.fake_crouch_key.GetKey()))
	{
		if (Options::Menu.MiscTab.fake_crouch.GetState())
		{
			static bool counter = false;
			static int counters = 0;
			if (counters == 9)
			{
				counters = 0;
				counter = !counter;
			}
			counters++;
			if (counter)
			{
				cmd->buttons |= IN_DUCK;
				packet = true;
			}
			else
				cmd->buttons &= ~IN_DUCK;
		}
	}
}
float CMiscHacks::get_gun(C_BaseCombatWeapon* weapon)
{
	
	if (!weapon)
		return 0.f;

	if (weapon->isAuto())
		return 40.f;

	else if (weapon->isScout())
		return 70.f;

	else if (weapon->isAwp())
		return 30.f;

	else
		return 34.f;
}

void CMiscHacks::strafer(CUserCmd* cmd) {

	if (hackManager.pLocal()->GetMoveType() == moveTypes::MOVETYPE_NOCLIP || hackManager.pLocal()->GetMoveType() == moveTypes::MOVETYPE_LADDER)
		return;

	if (!GetAsyncKeyState(VK_SPACE) || hackManager.pLocal()->GetVelocity().Length2D() < 0.5)
		return;

	if (!(hackManager.pLocal()->GetFlags() & FL_ONGROUND)) 
	{
		static float cl_sidespeed = Interfaces::CVar->FindVar("cl_sidespeed")->GetFloat();
		if (fabsf(cmd->mousedx > 2)) {
			cmd->sidemove = (cmd->mousedx < 0.f) ? -cl_sidespeed : cl_sidespeed;
			return;
		}

		if (GetAsyncKeyState('S')) {
			cmd->viewangles.y -= 180;
		}
		else if (GetAsyncKeyState('D')) {
			cmd->viewangles.y -= 90;
		}
		else if (GetAsyncKeyState('A')) {
			cmd->viewangles.y += 90;
		}

		if (!hackManager.pLocal()->GetVelocity().Length2D() > 0.5 || hackManager.pLocal()->GetVelocity().Length2D() == NAN || hackManager.pLocal()->GetVelocity().Length2D() == INFINITE)
		{
			cmd->forwardmove = 400;
			return;
		}

		cmd->forwardmove = clamp(5850.f / hackManager.pLocal()->GetVelocity().Length2D(), -400, 400);
		if ((cmd->forwardmove < -400 || cmd->forwardmove > 400))
			cmd->forwardmove = 0;

		const auto vel = hackManager.pLocal()->GetVelocity();
		const float y_vel = RAD2DEG(atan2(vel.y, vel.x));
		const float diff_ang = normalize_yaw(cmd->viewangles.y - y_vel);

		cmd->sidemove = (diff_ang > 0.0) ? -cl_sidespeed : cl_sidespeed;
		cmd->viewangles.y = normalize_yaw(cmd->viewangles.y - diff_ang);
	}
}

void CMiscHacks::viewmodel_x_y_z()
{
	static int vx, vy, vz;
	static ConVar* view_x = Interfaces::CVar->FindVar("viewmodel_offset_x");
	static ConVar* view_y = Interfaces::CVar->FindVar("viewmodel_offset_y");
	static ConVar* view_z = Interfaces::CVar->FindVar("viewmodel_offset_z");

	static ConVar* bob = Interfaces::CVar->FindVar("cl_bobcycle"); // sv_competitive_minspec 0

	ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
	SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
	sv_cheats_spoofed->SetInt(1);

	ConVar* sv_minspec = Interfaces::CVar->FindVar("sv_competitive_minspec");
	SpoofedConvar* sv_minspec_spoofed = new SpoofedConvar(sv_minspec);
	sv_minspec_spoofed->SetInt(0);

	view_x->nFlags &= ~FCVAR_CHEAT;
	view_y->nFlags &= ~FCVAR_CHEAT;
	view_z->nFlags &= ~FCVAR_CHEAT;

	vx = Options::Menu.VisualsTab.offset_x.GetValue();
	vy = Options::Menu.VisualsTab.offset_y.GetValue();
	vz = Options::Menu.VisualsTab.offset_z.GetValue();

	view_x->SetValue(vx);
	view_y->SetValue(vy);
	view_z->SetValue(vz);

}