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
	LagRecord()
	{

	}

	void SaveRecord(IClientEntity *player);
	float_t m_flSimulationTime;
	int32_t m_nFlags;
	int32_t m_iPriority;

};

class CMBacktracking : public Singleton<CMBacktracking>
{
public:
	std::deque<LagRecord> m_LagRecord[64]; // All records
	std::pair<LagRecord, LagRecord> m_RestoreLagRecord[64]; // Used to restore/change
	std::deque<LagRecord> backtrack_records; // Needed to select records based on menu option.
	LagRecord current_record[64]; // Used for various other actions where we need data of the current lag compensated player

	inline void ClearHistory()
	{
		for (int i = 0; i < 64; i++)
			m_LagRecord[i].clear();
	}
	int lastincomingsequencenumber;
	int GetPriorityLevel(IClientEntity * player, LagRecord * lag_record);

	float GetLerpTime();

	bool FindViableRecord(IClientEntity *player, LagRecord* record);
	bool StartLagCompensation(IClientEntity * player);
	bool IsTickValid(int tick);
	bool IsPlayerValid(IClientEntity * player);

	void AnimationFix(IClientEntity * player);
	void FrameUpdatePostEntityThink();
	void RemoveBadRecords(int Idx, std::deque<LagRecord>& records);
	void FinishLagCompensation(IClientEntity *player);

	template<class T, class U>
	T clamp(T in, U low, U high);

}; 
extern CMBacktracking * cm_backtrack;