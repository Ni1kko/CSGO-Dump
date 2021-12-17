#pragma once

#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\SDK\CPrediction.h"
#include "..\..\Menu\Menu.h"
#include <iostream>
#include <algorithm>
#include "..\..\Menu\config.h"
#define _SOLVEY(a, b, c, d, e, f) ((c * b - d * a) / (c * f - d * e))
#define SOLVEY(...) _SOLVEY(?, ?, ?, ?, ?, ?)
#define SOLVEX(y, world, forward, right) ((world.x - right.x * y) / forward.x)
#define Square(x) ((x)*(x))
auto LoadNamedSky = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::FindSignature("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
//Find out how many ticks player is choking

float GetNetworkLatency()
{
	INetChannelInfo *nci = g_pEngine->GetNetChannelInfo();
	if (nci)
	{
		return nci->GetAvgLatency(FLOW_INCOMING);
	}
	return 0.0f;
}


int GetNumberOfTicksChoked(C_BaseEntity* pEntity)
{
	float flSimulationTime = pEntity->GetSimulationTime();
	float flSimDiff = g_pGlobalVars->curtime - flSimulationTime;
	float latency = GetNetworkLatency();
	return TIME_TO_TICKS(max(0.0f, flSimDiff - latency));
}

void GetChoked() {
	auto NetChannel = g_pEngine->GetNetChannel();

	if (!NetChannel)
		return;

}

class chat_translator : public singleton<chat_translator> {
public:
	auto client_cmd(const char* cmd, std::string& buf) -> int;
	auto dispatch_user_message(int entity_id, const std::string& msg) -> void;
	auto thread_tick() -> void; // call from another thread, for prevent game freezes.
public:
	auto translate(const std::string& text, const std::string& lang)->std::string;

private:
	std::string lang_me_{ "" };
	std::string lang_other_{ "" };

	struct msg_t {
		int entity_id;
		std::string msg;
	};
	std::deque<msg_t> msgs_;
};

enum EChatTranslatorResult {
	ECTR_None,
	ECTR_SetLang,
	ECTR_Translate
};

class Misc
{
public:
    void OnCreateMove()
    {
        this->pCmd   = Globals::pCmd;
        this->pLocal = Globals::LocalPlayer;

		this->DoAutostrafe();
		this->DoBhop();
		this->AutoRevolver();
		if (!g_pEngine->IsVoiceRecording()) {
			this->DoFakeLag();
		}
		//this->FakeDuck();
		
		this->DoSlowWalk();
		this->Animated_Clantag();
    };

	void FakeDuck() {
		static bool counter = false;

		bool once = false;
		if (GetAsyncKeyState(c_config::get().fakeduck_bind))
		{
				unsigned int chokegoal = c_config::get().fakeduck_test;
				auto choke = *(int*)(uintptr_t(g_pClientState) + 0x4D28);
				bool mexican_tryhard = choke >= chokegoal;

				if (Globals::LocalPlayer->GetFlags() & FL_ONGROUND)
				{
					if (mexican_tryhard)
						Globals::pCmd->buttons |= IN_DUCK;
					else
						Globals::pCmd->buttons &= ~IN_DUCK;
				}
			}
	}

	void MovementFix(Vector& oldang)
	{
		Vector vMovements(Globals::pCmd->forwardmove, Globals::pCmd->sidemove, 0.f);

		if (vMovements.Length2D() == 0)
			return;

		Vector vRealF, vRealR;
		Vector aRealDir = Globals::pCmd->viewangles;
		aRealDir.Clamp();

		g_Math.AngleVectors(aRealDir, &vRealF, &vRealR, nullptr);
		vRealF[2] = 0;
		vRealR[2] = 0;

		VectorNormalize(vRealF);
		VectorNormalize(vRealR);

		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector vWishF, vWishR;
		g_Math.AngleVectors(aWishDir, &vWishF, &vWishR, nullptr);

		vWishF[2] = 0;
		vWishR[2] = 0;

		VectorNormalize(vWishF);
		VectorNormalize(vWishR);

		Vector vWishVel;
		vWishVel[0] = vWishF[0] * Globals::pCmd->forwardmove + vWishR[0] * Globals::pCmd->sidemove;
		vWishVel[1] = vWishF[1] * Globals::pCmd->forwardmove + vWishR[1] * Globals::pCmd->sidemove;
		vWishVel[2] = 0;

		float a = vRealF[0], b = vRealR[0], c = vRealF[1], d = vRealR[1];
		float v = vWishVel[0], w = vWishVel[1];

		float flDivide = (a * d - b * c);
		float x = (d * v - b * w) / flDivide;
		float y = (a * w - c * v) / flDivide;

		Globals::pCmd->forwardmove = x;
		Globals::pCmd->sidemove = y;
	}
	template<class T, class U>
	T fine(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	void LinearExtrapolations()
	{
		if (c_config::get().fakelag_prediction)
		{
			auto m_local = Globals::LocalPlayer;
			if (m_local && m_local->IsAlive()) {
				for (int i = 1; i < g_pGlobalVars->maxClients; i++)
				{
					auto m_entity = g_pEntityList->GetClientEntity(i);
					if (m_entity && m_entity->is_valid(Globals::LocalPlayer)) {

						float simtime_delta = m_entity->GetSimulationTime() - m_entity->GetOldSimulationTime();
						int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
						Vector lastOrig;

						if (lastOrig.Length() != m_entity->GetOrigin().Length())
							lastOrig = m_entity->GetOrigin();

						float delta_distance = (m_entity->GetOrigin() - lastOrig).LengthSqr();
						if (delta_distance > 4096.f)
						{
							Vector velocity_per_tick = m_entity->GetVelocity() * g_pGlobalVars->intervalPerTick;
							auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
							m_entity->SetAbsOrigin(new_origin);
						}
					}

				}
			}
		}
	}

	void NightMode() {
		static std::string old_Skyname = "";
		static bool OldNightmode;
		static int OldSky;
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !Globals::LocalPlayer || !Globals::LocalPlayer->IsAlive())
		{
			old_Skyname = "";
			OldNightmode = false;
			OldSky = 0;
			return;
		}

		static ConVar*r_DrawSpecificStaticProp;
		if (OldNightmode != c_config::get().nightmode)
		{

			r_DrawSpecificStaticProp = g_pCvar->FindVar("r_DrawSpecificStaticProp");
			r_DrawSpecificStaticProp->SetValue(0);

			for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
			{
				IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);
				if (!pMaterial)
					continue;
				if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
				{
					if (c_config::get().nightmode) {
						LoadNamedSky("sky_csgo_night02");

						if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
							pMaterial->ColorModulate(0.11f, 0.11f, 0.11f);
						else
							pMaterial->ColorModulate(0.05f, 0.05f, 0.05f);
					}
					else {
						LoadNamedSky("sky_cs15_daylight04_hdr");
						pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
					}
				}
			}
			OldNightmode = c_config::get().nightmode;
		}
	}

	void AsusProps() {
		static std::string old_Skyname = "";
		static bool OldNightmode;
		static int OldSky;

		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !Globals::LocalPlayer || !Globals::LocalPlayer->IsAlive())
		{
			old_Skyname = "";
			OldNightmode = false;
			OldSky = 0;
			return;
		}

		if (OldNightmode != c_config::get().transparent_props)
		{
			for (MaterialHandle_t i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
			{
				IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);
				if (!pMaterial)
					continue;
				if (strstr(pMaterial->GetTextureGroupName(), "StaticProp textures"))
				{
					if (c_config::get().transparent_props) {
						pMaterial->AlphaModulate(0.7f);
					}
					else {
						pMaterial->AlphaModulate(1.f);
					}
						
				}
			}
			OldNightmode = c_config::get().transparent_props;
		}

	}
	void SwapManual()
	{
		if (GetKeyState(c_config::get().manual_swap_bind))
		{
			Globals::Manual_Side = true;
		}
		else {
			Globals::Manual_Side = false;
		}
	}
	void ThirdPerson()
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !Globals::LocalPlayer)
			return;
			
		static bool init = false;
		static bool set_angle = false;
		auto pLocalEntity = Globals::LocalPlayer;

		static int stored_thirdperson_distance;

		if (stored_thirdperson_distance != c_config::get().thirdperson_distance) {
			std::string command; command += "cam_idealdist "; command += std::to_string(c_config::get().thirdperson_distance + 30);
			g_pEngine->ExecuteClientCmd(command.c_str());

			stored_thirdperson_distance = c_config::get().thirdperson_distance;
		}
		static Vector vecAngles;
		g_pEngine->GetViewAngles(vecAngles);

		if (GetKeyState(c_config::get().thirdperson_bind) && Globals::LocalPlayer->IsAlive())
		{
			if (init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("thirdperson");

				std::string command; command += "cam_idealdist "; command += std::to_string(c_config::get().thirdperson_distance + 30);
				g_pEngine->ExecuteClientCmd(command.c_str());
			}
			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("firstperson");
			}
			init = true;
		}


	}

	void Thirdperson_FSN(ClientFrameStage_t curStage) {
		if (curStage == FRAME_RENDER_START && g_GameInput->m_fCameraInThirdPerson && Globals::LocalPlayer && Globals::LocalPlayer->IsAlive())
		{
				g_pPrediction->SetLocalViewAngles(Vector(Globals::RealAngle.x, Globals::RealAngle.y, 0));
		}
	}
	void NormalWalk()
	{

		Globals::pCmd->buttons &= ~IN_MOVERIGHT;
		Globals::pCmd->buttons &= ~IN_MOVELEFT;
		Globals::pCmd->buttons &= ~IN_FORWARD;
		Globals::pCmd->buttons &= ~IN_BACK;

		if (Globals::pCmd->forwardmove > 0.f)
			Globals::pCmd->buttons |= IN_FORWARD;
		else if (Globals::pCmd->forwardmove < 0.f)
			Globals::pCmd->buttons |= IN_BACK;
		if (Globals::pCmd->sidemove > 0.f)
		{
			Globals::pCmd->buttons |= IN_MOVERIGHT;
		}
		else if (Globals::pCmd->sidemove < 0.f)
		{
			Globals::pCmd->buttons |= IN_MOVELEFT;
		}

	}


	void NoRecoil(CUserCmd* cmd)
	{
		if (!c_config::get().aimbot_norecoil)
			return;

		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;

		auto weapon = local_player->GetActiveWeapon();
		if (weapon)
			weapon->GetAccuracyPenalty();

		cmd->viewangles -= local_player->GetAimPunchAngle() * 2;
	}

	void SetClan(const char* tag, const char* name) {
		static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utils::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
		pSetClanTag(tag, name);
	}


	void Animated_Clantag() {
		if (!c_config::get().misc_clantag)
			return;

		static int counter = 0;
		static std::string clantag = "geminisoftware               ";
		if (++counter > 25) {
			std::rotate(clantag.begin(), clantag.begin() + 1, clantag.end());
			SetClan(clantag.c_str(), clantag.c_str());
			counter = 0;
		}
	}

	void AutoRevolver()
	{
		auto me = Globals::LocalPlayer;
		auto cmd = Globals::pCmd;
		auto weapon = me->GetActiveWeapon();

		if (!c_config::get().autorevolver)
			return;

		if (!me || !me->IsAlive() || !weapon)
			return;

		if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
		{
			static int delay = 0; /// pasted delay meme from uc so we'll stop shooting on high ping
			delay++;

			if (delay <= 15)
				Globals::pCmd->buttons |= IN_ATTACK;
			else
				delay = 0;
		}
	}

	void slow_walk(CUserCmd *cmd)
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			return;

		if (!Globals::LocalPlayer)
			return;

		auto weapon_handle = Globals::LocalPlayer->GetActiveWeapon();

		if (!weapon_handle)
			return;

		Vector velocity = Globals::LocalPlayer->GetVelocity();
		Vector direction = velocity.Angle();
		float speed = velocity.Length();

		direction.y = cmd->viewangles.y - direction.y;

		Vector negated_direction = direction * -speed;
		if (velocity.Length() >= (weapon_handle->GetCSWpnData()->max_speed * .34f))
		{
			cmd->forwardmove = negated_direction.x;
			cmd->sidemove = negated_direction.y;
		}
	}

	void MinWalk(CUserCmd* get_cmd, float get_speed) const
	{
		if (get_speed <= 0.f)
			return;

		float min_speed = (float)(FastSqrt(Square(get_cmd->forwardmove) + Square(get_cmd->sidemove) + Square(get_cmd->upmove)));
		if (min_speed <= 0.f)
			return;

		if (get_cmd->buttons & IN_DUCK)
			get_speed *= 2.94117647f;

		if (min_speed <= get_speed)
			return;

		float kys = get_speed / min_speed;

		get_cmd->forwardmove *= kys;
		get_cmd->sidemove *= kys;
		get_cmd->upmove *= kys;
	}
	void DoSlowWalk() const
	{
		if (c_config::get().slowwalk_bind > 0 && !GetAsyncKeyState(c_config::get().slowwalk_bind) || c_config::get().slowwalk_bind <= 0)
		{
			return;
		}

		MinWalk(pCmd, c_config::get().slowwalk_speed);
	}


private:
    CUserCmd*     pCmd;
    C_BaseEntity* pLocal;

    void DoBhop() const
    {
        if (!c_config::get().misc_bhop)
            return;

        static bool bLastJumped = false;
        static bool bShouldFake = false;

        if (!bLastJumped && bShouldFake)
        {
            bShouldFake = false;
            pCmd->buttons |= IN_JUMP;
        }
        else if (pCmd->buttons & IN_JUMP)
        {
            if (pLocal->GetFlags() & FL_ONGROUND)
                bShouldFake = bLastJumped = true;
            else 
            {
                pCmd->buttons &= ~IN_JUMP;
                bLastJumped = false;
            }
        }
        else
            bShouldFake = bLastJumped = false;
    }

	void DoAutostrafe() const
	{
		if (!Globals::LocalPlayer || !g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !c_config::get().misc_autostrafe)
			return;

		if (!Globals::LocalPlayer->IsAlive())
			return;

		if (!(Globals::LocalPlayer->GetFlags() & FL_ONGROUND) && GetAsyncKeyState(VK_SPACE))
		{
			pCmd->forwardmove = (10000.f / Globals::LocalPlayer->GetVelocity().Length2D() > 450.f) ? 450.f : 10000.f / Globals::LocalPlayer->GetVelocity().Length2D();
			pCmd->sidemove = (pCmd->mousedx != 0) ? (pCmd->mousedx < 0.0f) ? -450.f : 450.f : (pCmd->command_number % 2) == 0 ? -450.f : 450.f;	
		}
	}
	int FakeLagVariance() const
	{
		static int factor;
		int maxvalue;

		if (c_config::get().variance > 0) {

			int variance = 0;

			variance = rand() % c_config::get().variance / 7.142857143;

			if (variance > 14) { // cool
				variance = 14;
			}

			return variance;
		}
		else {
			return c_config::get().fakelag;
		}
	}
	void DoFakeLag()
	{
		if (!Globals::LocalPlayer || !g_pEngine->IsConnected() || !g_pEngine->IsInGame() || (c_config::get().fakelag == 0 && !(GetAsyncKeyState(c_config::get().fakeduck_bind) && c_config::get().fakeduck_bind > 1)) || g_Menu.Config.LegitBacktrack)
			return;
		
		if (!Globals::LocalPlayer->IsAlive() || Globals::LocalPlayer->IsNade())
			return;

		auto NetChannel = g_pEngine->GetNetChannel();

		if (!NetChannel)
			return;
		
		if (GetAsyncKeyState(c_config::get().fakeduck_bind) && c_config::get().fakeduck_bind > 1)
		{
			Globals::bSendPacket = (14 <= *(int*)(uintptr_t(g_pClientState) + 0x4D28));
		}
		else {
			Globals::bSendPacket = (NetChannel->m_nChokedPackets >= FakeLagVariance());
		}
		
	}
};

extern Misc g_Misc;

