#pragma once

#include "../../utilities/includes.h"
#include "../../sdk/checksum_md5.hpp"

#include "../../utilities/Singleton.hpp"


class CEnginePrediction
{
public:
	void run(CUserCmd* cmd);
private:
	int iTickBaseBackup;
	int iFlagsBackup;
	int iButtonsBackup;
	int* m_pPredictionRandomSeed;
	int* m_pSetPredictionPlayer;
}; extern CEnginePrediction g_EnginePrediction;