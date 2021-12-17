#include "../MISC/includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CPrediction.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/EnginePred.h"

float flOldCurtime;
float flOldFrametime;

void CEnginePred::run_prediction(SDK::CUserCmd* cmd)
{
	static int flTickBase;
	static SDK::CUserCmd* pLastCmd;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	// fix tickbase if game didnt render previous tick
	if (pLastCmd)
	{
		if (pLastCmd->hasbeenpredicted)
			flTickBase = local_player->GetTickBase();
		else
			++flTickBase;
	}

	// get random_seed as its 0 in clientmode->createmove
	const auto getRandomSeed = [&]()
	{
		using MD5_PseudoRandomFn = unsigned long(__cdecl*)(std::uintptr_t);
		static auto offset = UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 70 6A 58");
		static auto MD5_PseudoRandom = reinterpret_cast<MD5_PseudoRandomFn>(offset);
		return MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;
	};


	pLastCmd = cmd;
	flOldCurtime = INTERFACES::Globals->curtime;
	flOldFrametime = INTERFACES::Globals->frametime;
	SDK::CGV::uRandomSeed = getRandomSeed();
	INTERFACES::Globals->curtime = flTickBase * INTERFACES::Globals->interval_per_tick;
	INTERFACES::Globals->frametime = INTERFACES::Globals->interval_per_tick;

	INTERFACES::pMovement->StartTrackPredictionErrors(local_player);

	SDK::CMoveData data;
	memset(&data, 0, sizeof(SDK::CMoveData));

	INTERFACES::pMoveHelper->set_host(local_player);
	INTERFACES::pPrediction->SetupMove(local_player, cmd, INTERFACES::pMoveHelper, &data);
	INTERFACES::pMovement->ProcessMovement(local_player, &data);
	INTERFACES::pPrediction->FinishMove(local_player, cmd, &data);
}

void CEnginePred::end_prediction(SDK::CUserCmd * cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return

	INTERFACES::pMovement->FinishTrackPredictionErrors(local_player); 
	INTERFACES::pMoveHelper->set_host(nullptr);

	INTERFACES::Globals->curtime = flOldCurtime;
	INTERFACES::Globals->frametime = flOldFrametime;
}

CEnginePred* prediction = new CEnginePred();