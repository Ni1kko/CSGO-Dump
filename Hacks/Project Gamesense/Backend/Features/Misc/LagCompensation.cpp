#include "LagCompensation.h"
#include "../../Configuration/Config.h"
#include "../../SDK/INetChannelInfo.h"
#include "../../SDK/player_info_t.h"

using namespace Features;
using namespace Cheat;
CLagCompensation* Features::LagCompensation = new CLagCompensation();

float CLagCompensation::LerpTime() // GLAD https://github.com/sstokic-tgm/Gladiatorcheatz-v2.1/blob/eaa88bbb4eca71f8aebfed32a5b86300df8ce6a3/features/LagCompensation.cpp
{
	int updaterate = Interfaces::Var->FindVar("cl_updaterate")->GetInt();
	ConVar* minupdate = Interfaces::Var->FindVar("sv_minupdaterate");
	ConVar* maxupdate = Interfaces::Var->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetInt();

	float ratio = Interfaces::Var->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::Var->FindVar("cl_interp")->GetFloat();
	ConVar* cmin = Interfaces::Var->FindVar("sv_client_min_interp_ratio");
	ConVar* cmax = Interfaces::Var->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, (ratio / updaterate));
}

bool CLagCompensation::ValidTick(float simtime)
{
	auto nci = Interfaces::Engine->GetNetChannelInfo();

	if (!nci)
		return false;

	const auto sv_maxunlag = Interfaces::Var->FindVar("sv_maxunlag");

	auto Correct = clamp(nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING), 0.f, sv_maxunlag->GetFloat());

	return (fabs(Correct - (Interfaces::Globals->curtime - simtime)) <= 0.2f);
}

void CLagCompensation::CreateRecord(CBaseEntity* pEnt)
{
	if (!c_config::get()->b["rage_resolver"])
		return;

	TickInfo Setup;
	bool Shot;

	BYTE Baclp;

	if (pEnt->GetActiveWeapon() && !(pEnt->GetActiveWeapon()->is_grenade() || pEnt->GetActiveWeapon()->is_knife()))
	{
		if (ShotTime[pEnt->Index()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			if (Setup.Pitch != pEnt->GetEyeAngles().x)
				Shot = true;
			ShotTime[pEnt->Index()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->Index()] = 0.f;
	}

	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(pEnt) + 0xA30) = Interfaces::Globals->framecount;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(pEnt) + 0xA28) = 0;

	Baclp = *reinterpret_cast<BYTE*>(reinterpret_cast<uintptr_t>(pEnt) + 0x274);
	*reinterpret_cast<BYTE*>(reinterpret_cast<uintptr_t>(pEnt) + 0x274) = 0;

	pEnt->InvalidateBoneCache();

	pEnt->SetupBones(pMatrix[pEnt->Index()], 128, 0x0007FF00, Interfaces::Globals->curtime);

	*reinterpret_cast<BYTE*>(reinterpret_cast<uintptr_t>(pEnt) + 0x274) = Baclp;

	Setup.Pitch = pEnt->GetEyeAngles().x;
	Setup.SimTime = pEnt->GetSimTime();
	Setup.Head = pEnt->GetHitboxPosition(HITBOX_HEAD, pMatrix[pEnt->Index()]);
	Setup.Shot = Shot;
	Setup.Velocity = abs(pEnt->GetVelocity().Length2D());
	memcpy(Setup.Matrix, pMatrix[pEnt->Index()], (sizeof(matrix3x4_t) * 128));
	PlayerRecord[pEnt->Index()].push_back(Setup);
}

void CLagCompensation::FrameStage(ClientFrameStage_t stage)
{

	if (!Interfaces::Engine->IsInGame() || !Interfaces::Engine->IsConnected())
		return;

	CBaseEntity* pLocalEntity = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());

	if (!pLocalEntity)
		return;

	for (int i = 1; i <= Interfaces::Engine->GetMaxClients(); ++i)
	{
		CBaseEntity* pPlayerEntity = Interfaces::EntityList->GetClientEntity(i);

		if (stage == FRAME_RENDER_START && pPlayerEntity && !pPlayerEntity->IsDormant() && pPlayerEntity->GetClientClass()->m_ClassID == (int)ClassID::CCSRagdoll)
			pPlayerEntity->SetSequence(1); // allow game to setupbones when the player is a ragdoll

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			ClearRecords(i);
			continue;
		}

		if (stage == FRAME_RENDER_START)
		{
			if (PlayerRecord[i].size() == 0 || pPlayerEntity == G::LocalPlayer)
				CreateRecord(pPlayerEntity);
			else if ((PlayerRecord[i].size() != 0) && PlayerRecord[i].at(PlayerRecord[i].size() - 1).SimTime != pPlayerEntity->GetSimTime())
				CreateRecord(pPlayerEntity);

			if (pPlayerEntity != pLocalEntity)
				pPlayerEntity->SetSequence(-1); // dont allow the game to setup bones for other entitys *rly cancer and most likely very inproper and fux bouding box

			ShotTick[i] = -1;

			if (PlayerRecord[i].size() != 0)
				for (int tick = 0; tick < PlayerRecord[i].size(); tick++)
					if (!ValidTick(PlayerRecord[i].at(tick).SimTime))
						PlayerRecord[i].erase(PlayerRecord[i].begin() + tick);
					else if (PlayerRecord[i].at(tick).Shot)
						ShotTick[i] = tick;
		}

	}
}

void CLagCompensation::ClearRecords(int i)
{
	if (PlayerRecord[i].size() > 0)
	{
		for (int tick = 0; tick < PlayerRecord[i].size(); tick++)
		{
			PlayerRecord[i].erase(PlayerRecord[i].begin() + tick);
		}
	}
	ShotTick[i] = -1;
	ShotTime[i] = 0.f;
}

template<class T, class U>
T CLagCompensation::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}