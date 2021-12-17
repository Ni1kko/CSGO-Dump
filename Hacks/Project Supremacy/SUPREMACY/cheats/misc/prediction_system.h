#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class engineprediction : public singleton< engineprediction > {
public:
	void start();
	void start_prediction(CUserCmd * cmd);
private:
	int m_tickbase_backup;
	int m_flags_backup;
	int * m_random_seed;
	int * m_prediction_player;
	char * m_movedata = nullptr;
};