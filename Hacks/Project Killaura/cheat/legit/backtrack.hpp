#pragma once
#include "../config/options.hpp"
#include "../../sdk/structs.hpp"
#include <map>
#include <deque>

struct backtrack_data {
	float simTime;
	Vector hitboxPos;
	matrix3x4_t boneMatrix[128];
};

class Backtrack
	: public Singleton<Backtrack>
{
	friend class Singleton<Backtrack>;
public:
	void OnMove(CUserCmd* pCmd);
	float correct_time = 0.0f;
	float latency = 0.0f;
	float lerp_time = 0.0f;
	std::map<int, std::deque<backtrack_data>> data;
};