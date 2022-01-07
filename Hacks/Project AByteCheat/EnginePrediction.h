#pragma once
//way shorter but still same effect lol
#include "SDK.h"

class CPredictionSystem
{
public:
	float m_flOldCurtime;
	float m_flOldFrametime;
	CMoveData m_MoveData;

	int* m_pPredictionRandomSeed;

	void StartPrediction(CUserCmd * pCmd);
	void EndPrediction(CUserCmd * pCmd);
	int post_think(IClientEntity * player) const;
};