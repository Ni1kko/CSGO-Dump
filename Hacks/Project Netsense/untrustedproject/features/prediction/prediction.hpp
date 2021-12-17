#pragma once
#include "../../singleton.hpp"
#include "../../valve_sdk/csgostructs.hpp"
#include <cstdint>
#include "../../valve_sdk/sdk.hpp"

class CPredictionSystem : public Singleton<CPredictionSystem>{
private:
	int* m_pPredictionRandomSeed;
	int  m_flOldCurtime;
	int  m_flOldFrametime;
	int  m_fFlags;

	CMoveData m_MoveData;

public:
	void StartPrediction(C_BasePlayer* pLocal, CUserCmd* cmd);
	void EndPrediction(C_BasePlayer* pLocal, CUserCmd* cmd);
};