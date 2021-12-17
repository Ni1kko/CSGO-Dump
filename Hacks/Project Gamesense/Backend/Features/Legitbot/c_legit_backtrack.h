#pragma once

#include "legit_utils.h"
#include <map>
#include <deque>
#include "../../SDK/INetChannelInfo.h"

struct CLegitBacktrackData {
	float simTime;
	Vector hitboxPos;
	matrix3x4_t boneMatrix[128];
	matrix3x4_t boneMatrixCur[128];
};

class CLegitBacktrack {
public:
	void OnMove(CUserCmd* pCmd);
	std::map<int, std::deque<CLegitBacktrackData>> data;
private:

	float correct_time = 0.0f;
	float latency = 0.0f;
	float lerp_time = 0.0f;

};

extern CLegitBacktrack g_legit_backtrack;
