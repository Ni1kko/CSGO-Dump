#pragma once

#include "main.h"

class CRunCommand
{
public:
	void RunPrediction( CBaseEntity * pLocal, CUserCmd * pUserCmd );
};

extern CRunCommand g_Prediction;

void __stdcall HOOKED_RunCommand( CBaseEntity* pEntity, CUserCmd* pCmd, void* moveHelper );