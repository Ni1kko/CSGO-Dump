#pragma once

#include "../settings.h"
#include "../interfaces.h"
#include "../Utils/entity.h"
#include "../Utils/math.h"

extern "C"
{
	#include <xdo.h>
}
struct KVP
{
	int Tick;
	int command_number;
	Vector Bone;
	C_BasePlayer* player;
};
extern std::map<int, KVP>Log[64];
extern KVP kvp;
extern int ticker;
namespace Aimbot
{
	extern bool aimStepInProgress;
	extern std::vector<int64_t> friends;

	void XDOCleanup();

	//Hooks
	void PrePredictionCreateMove(CUserCmd* cmd);
	void CreateMove(CUserCmd* cmd);
	void FireGameEvent(IGameEvent* event);
	void UpdateValues();

	extern int targetAimbot;
}

