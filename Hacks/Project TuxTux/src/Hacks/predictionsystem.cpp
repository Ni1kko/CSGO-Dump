#include "predictionsystem.h"

float m_flOldCurtime;
float m_flOldFrametime;


float oldGetEyeAngles;
void PredictionSystem::StartPrediction_e()
{
	for (int i = 0; i < engine->GetMaxClients(); i++)
	{
		C_BasePlayer* p_ent = (C_BasePlayer*) entityList->GetClientEntity(i);

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
				continue;

			if (!p_ent
				|| p_ent->GetDormant()
				|| !p_ent->GetAlive()
				|| p_ent->GetImmune()
				|| localplayer == p_ent)
				continue;

		CCSGOAnimState* animState = p_ent->GetAnimState();
			if (!animState)
				continue;
		
	

		m_flOldCurtime = globalVars->curtime;
		m_flOldFrametime = globalVars->frametime;

		oldGetEyeAngles = p_ent->GetEyeAngles()->y;

		*p_ent->ClientSideAnimation() = true;
		
		globalVars->curtime = p_ent->GetTickBase() * globalVars->interval_per_tick;
		globalVars->frametime = globalVars->interval_per_tick;

			float maxDelta = AntiAim::GetMaxDelta(animState);
			float delta = std::abs(animState->yaw - animState->goalFeetYaw);
			float m_flMaxBodyYaw = maxDelta;
            float m_flMinBodyYaw = -maxDelta;
			float ResolvedYaw = animState->yaw;
            if (m_flMaxBodyYaw < delta)
            {
                ResolvedYaw = p_ent->GetEyeAngles()->y - m_flMaxBodyYaw;
 
            }
            else if (m_flMinBodyYaw > delta)
            {
                ResolvedYaw = p_ent->GetEyeAngles()->y +m_flMinBodyYaw;
 
            }

			p_ent->GetEyeAngles()->y = ResolvedYaw;

		gameMovement->StartTrackPredictionErrors(p_ent);
		gameMovement->ProcessMovement(localplayer, g_MoveData);
		
		EndPrediction_e(p_ent);
	}
	
	
}

void PredictionSystem::EndPrediction_e(C_BasePlayer* p_ent)
{

		*p_ent->ClientSideAnimation() = false;

		gameMovement->FinishTrackPredictionErrors(p_ent);

		p_ent->GetEyeAngles()->y = oldGetEyeAngles;

		globalVars->curtime = m_flOldCurtime;
		globalVars->frametime = m_flOldFrametime;

		
}
void PredictionSystem::StartPrediction(CUserCmd* cmd)
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	*nPredictionRandomSeed = MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;

	m_flOldCurtime = globalVars->curtime;
	m_flOldFrametime = globalVars->frametime;

	globalVars->curtime = localplayer->GetTickBase() * globalVars->interval_per_tick;
	globalVars->frametime = globalVars->interval_per_tick;

	
	

	gameMovement->StartTrackPredictionErrors(localplayer);

	moveHelper->SetHost(localplayer);
	prediction->SetupMove(localplayer, cmd, moveHelper, g_MoveData);
	gameMovement->ProcessMovement(localplayer, g_MoveData);
	prediction->FinishMove(localplayer, cmd, g_MoveData);
}

void PredictionSystem::EndPrediction()
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	gameMovement->FinishTrackPredictionErrors(localplayer);
	moveHelper->SetHost(0);

	*nPredictionRandomSeed = -1;

	globalVars->curtime = m_flOldCurtime;
	globalVars->frametime = m_flOldFrametime;
}
