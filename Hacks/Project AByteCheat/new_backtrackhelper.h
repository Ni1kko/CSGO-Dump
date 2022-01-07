#pragma once
#include "SDK.h"
#include "Hooks.h"
#include "newbacktrack.h"
struct tick_record
{
	Vector m_vecOrigin;//0 //4 // 8
	bool bLowerBodyYawUpdated;
	bool bFakelagging;
	float m_flSimulationTime; //12
	float m_flAnimTime;
	int m_nSequence;//16
	float m_flCycle; //20
	Vector m_angEyeAngles;//x->24
	float m_flUpdateTime;//36
	Vector m_vecVelocity;//40 //44 //48
	float m_flPoseParameter[24];//52
	float backtrack_time;
	bool shot_in_that_record = false;
	bool needs_extrapolation = false;
	/*my addition*/
	matrix3x4 boneMatrix[128];
};
struct player_record
{
	//std::vector<tick_record> records;
	std::vector<tick_record> records;
	Vector EyeAngles;
	float LowerBodyYawTarget;
	int Flags;
	float unknown;
};

struct simulation_record
{
	Vector origin;
	Vector velocity;
	Vector acceleration;
	float simulation_time;
	float simulation_time_increasment_per_tick;
	float update_time;

};
class CBacktrackHelper
{
public:
	player_record PlayerRecord[64];
	simulation_record SimRecord[64][7];
	float GetLerpTime();
	int GetDesiredTickCount(float flTargetTime);
	float GetNetworkLatency();
	int GetLatencyTicks();
	int GetEstimateServerTickCount();
	float GetEstimateServerTime();
	void UpdateBacktrackRecords(IClientEntity* pPlayer);
	void UpdateExtrapolationRecords(IClientEntity* pPlayer);
	void ExtrapolatePosition(IClientEntity* pPlayer, Vector& position, float &simtime, Vector velocity);
//	void LagCompensate(IClientEntity* pPlayer);
	bool IsTickValid(tick_record record);


}; extern CBacktrackHelper* g_BacktrackHelper;