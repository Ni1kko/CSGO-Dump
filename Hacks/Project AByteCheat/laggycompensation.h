#pragma once
#include "Hooks.h"
#include "Hacks.h"
#include <algorithm>
#include "ESP.h"
#include "Entities.h"
#include "SDK.h"
#include "Singleton.hpp"
#include <deque>
struct LagRecord
{
	bool    m_bNoGoodSpots;
	bool m_bMatrixBuilt;

	Vector  m_vecLocalAimspot;

	LagRecord()
	{

	}

	void SaveRecord(IClientEntity *player);
	float_t m_flSimulationTime;
	int32_t m_nFlags;
	int32_t m_iPriority;
	matrix3x4	matrix[128];
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
class CMBacktracking : public Singleton<CMBacktracking>
{
public:
	std::deque<LagRecord> m_LagRecord[64]; // All records
	std::pair<LagRecord, LagRecord> m_RestoreLagRecord[64]; // Used to restore/change
	std::deque<LagRecord> backtrack_records; // Needed to select records based on menu option.
	LagRecord current_record[64]; // Used for various other actions where we need data of the current lag compensated player
	simulation_record SimRecord[64][7];
	inline void ClearHistory()
	{
		for (int i = 0; i < 64; i++)
			m_LagRecord[i].clear();
	}
	int lastincomingsequencenumber;
	int GetPriorityLevel(IClientEntity * player, LagRecord * lag_record);

	int GetLatencyTicks();

	float GetLerpTime();

	bool FindViableRecord(IClientEntity *player, LagRecord* record);
	bool StartLagCompensation(IClientEntity * player);
	bool IsTickValid(int tick);
	bool IsPlayerValid(IClientEntity * player);

	void AnimationFix(IClientEntity * player);
	void ExtrapolatePosition(IClientEntity * pPlayer, Vector & position, float & simtime, Vector velocity);
	void ProcessCMD(int iTargetIdx, CUserCmd * usercmd);
	void RageBacktrack(IClientEntity * target, CUserCmd * usercmd, Vector & aim_point, bool & hitchanced);
	void FrameUpdatePostEntityThink();
	void RemoveBadRecords(int Idx, std::deque<LagRecord>& records);
	void FinishLagCompensation(IClientEntity *player);

	template<class T, class U>
	T clamp(T in, U low, U high);

	int        tick_count;

};


extern CMBacktracking * cm_backtrack;