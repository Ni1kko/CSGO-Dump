#pragma once

class CEnginePred
{
public:
	void run_prediction(SDK::CUserCmd * cmd);
	void end_prediction(SDK::CUserCmd * cmd);
	void eStartPrediction(SDK::CUserCmd * cmd);
	void eEndPrediction() const;
};

extern CEnginePred* prediction;