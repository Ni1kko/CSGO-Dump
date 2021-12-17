#pragma once
#include "settings.h"
#include "valve_sdk/csgostructs.hpp"
#include <map>
#include <deque>

struct backtrack_data {
	float simTime;
	Vector hitboxPos;
	Vector origin;
	QAngle angle;
	matrix3x4_t boneMatrix[128];
};

class Backtrack {
public:
	void OnMove(CUserCmd* pCmd);

	float correct_time = 0.0f;
	float latency = 0.0f;
	float lerp_time = 0.0f;
	std::map<int, std::deque<backtrack_data>> data;
};
extern Backtrack g_Backtrack;
