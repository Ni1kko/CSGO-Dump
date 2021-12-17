#include "../singleton.hpp"
#include "../valve_sdk/csgostructs.hpp"

#pragma once
class BunnyHop : public Singleton<BunnyHop>
{
public:
	void OnCreateMove(CUserCmd* cmd);
	void AutoStrafe(CUserCmd* cmd, QAngle va);
};