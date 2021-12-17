#pragma once
#include "../Sp1Ky Private/features/MainInclude.hpp"
#define NUM_OF_TICKS 12
class QAngle;
struct StoredData
{
	float simtime;
	Vector hitboxPos;
	Vector origin;
	QAngle viewangle;
	Vector headpos;
};
class TimeWarp : public Singleton<TimeWarp>
{
	int nLatestTick;
public:
	StoredData TimeWarpData[64][NUM_OF_TICKS];
	void CreateMove(CUserCmd* userCMD);
};
