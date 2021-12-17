#pragma once

#include "../settings.h"
#include "../SDK/SDK.h"

namespace PredictionSystem
{
	//Hooks
	/* CreateMove */


	void StartPrediction_e();
	void EndPrediction_e(C_BasePlayer* p_ent);
	
	void StartPrediction(CUserCmd* cmd);
	void EndPrediction();
};
