#include "enginePrediction.hpp"

CEnginePrediction g_EnginePrediction;

static char* cMoveData;

void CEnginePrediction::run(CUserCmd* cmd)
{
	if (!g_Interfaces->gameEngine->connected() && !g_Interfaces->gameEngine->inGame())
		return;

	//Nullptr'ing is fun!
	if (!g_Interfaces->moveHelper || !cmd || !Globals::localPlayer || !Globals::localPlayer->alive())
		return;

	//We need to cast g_LocalPlayer to C_BasePlayer so we can use it in our code
	CMoveData C_MoveData;

	//Let's back things up for later use
	iFlagsBackup = Globals::localPlayer->flags();
	iButtonsBackup = cmd->buttons;

	//Set m_pCurrentCommand to cmd. We'll set this to nullptr later.
	Globals::localPlayer->setCurrentCommand(cmd);

	//Let's get some signatures now
	if (!m_pPredictionRandomSeed || !m_pSetPredictionPlayer)
	{
		m_pPredictionRandomSeed = *reinterpret_cast<int**>(g_Memory->findPattern("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2); // A3 ? ? ? ? 66 0F 6E 86  / +1
		m_pSetPredictionPlayer = *reinterpret_cast<int**>(g_Memory->findPattern("client.dll", "89 35 ? ? ? ? F3 0F 10 48 20") + 2);
	}

	//We don't want to be calling this every tick, bad for framerate.
	//Thanks, zbe and Heep
	if (!cMoveData)
		cMoveData = (char*)(calloc(1, sizeof(CMoveData)));

	//Let's set it up so that it predicts ourselves
	g_Interfaces->moveHelper->setHost(Globals::localPlayer);
	*m_pPredictionRandomSeed = MD5_PseudoRandom(cmd->commandNumber) & 0x7FFFFFFF; //crashing here!!!!!!! some nullptr
	*m_pSetPredictionPlayer = uintptr_t(Globals::localPlayer);

	//Let's set some global variables
	g_Interfaces->globalVars->curtime = Globals::localPlayer->tickBase() * g_Interfaces->globalVars->intervalPerTick;
	g_Interfaces->globalVars->frametime = g_Interfaces->globalVars->intervalPerTick;

	//Don't know is for exactly, but here's the relevant IDA picture of it. https://i.imgur.com/hT6caQV.png
	cmd->buttons |= *reinterpret_cast<uint8_t*>(uintptr_t(Globals::localPlayer) + 0x3310);

	//This is for flashlights in older Source games, Thanks, friendly for the info
	if (cmd->impulse)
		*reinterpret_cast<uint8_t*>(uintptr_t(Globals::localPlayer) + 0x31EC) = cmd->impulse;

	//Here we're doing CBasePlayer::UpdateButtonState
	C_MoveData.m_nButtons = cmd->buttons;
	int buttonsChanged = cmd->buttons ^ *reinterpret_cast<int*>(uintptr_t(Globals::localPlayer) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(Globals::localPlayer) + 0x31DC) = (uintptr_t(Globals::localPlayer) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(Globals::localPlayer) + 0x31E8) = cmd->buttons;
	*reinterpret_cast<int*>(uintptr_t(Globals::localPlayer) + 0x31E0) = cmd->buttons & buttonsChanged;  //m_afButtonPressed ~ The changed ones still down are "pressed"
	*reinterpret_cast<int*>(uintptr_t(Globals::localPlayer) + 0x31E4) = buttonsChanged & ~cmd->buttons; //m_afButtonReleased ~ The ones not down are "released"

	g_Interfaces->gameMovement->StartTrackPredictionErrors(Globals::localPlayer);

	iTickBaseBackup = Globals::localPlayer->tickBase();

	g_Interfaces->prediction->SetupMove(Globals::localPlayer, cmd, g_Interfaces->moveHelper, reinterpret_cast<CMoveData*>(cMoveData));
	g_Interfaces->gameMovement->ProcessMovement(Globals::localPlayer, reinterpret_cast<CMoveData*>(cMoveData));
	g_Interfaces->prediction->FinishMove(Globals::localPlayer, cmd, reinterpret_cast<CMoveData*>(cMoveData));

	//Let's override our tickbase with the backed up tickbase
	Globals::localPlayer->setTickBase(iTickBaseBackup);

	g_Interfaces->gameMovement->FinishTrackPredictionErrors(Globals::localPlayer);

	//Let's nullify these here
	Globals::localPlayer->setCurrentCommand(nullptr);
	*m_pPredictionRandomSeed = -1;
	*m_pSetPredictionPlayer = 0;
	g_Interfaces->moveHelper->setHost(0);

	//Last but not least, set these to their backups 
	Globals::localPlayer->setFlags(iFlagsBackup);
	cmd->buttons = iButtonsBackup;
}