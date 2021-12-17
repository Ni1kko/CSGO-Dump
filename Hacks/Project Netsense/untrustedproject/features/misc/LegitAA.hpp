#pragma once
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"

class LegitAA {
public:
	float side = 1.0f;
private:
	CCSGOPlayerAnimState AnimState;
	float next_lby = 0.0f;
	float real_angle;
	float view_angle;
public:
	void CreateMove(CUserCmd* pCmd, bool* bSendPacket);
	void DrawArrows();
};

extern LegitAA* g_LegitAA;