#include "runcommand.h"
#include "md5.h"

CRunCommand g_Prediction;

void SetCurrentCmd( CUserCmd *pDest, CUserCmd *pSource )
{
	pDest->command_number = pSource->command_number;
	pDest->tick_count = pSource->tick_count;
	pDest->viewangles = pSource->viewangles;
	pDest->aimdirection = pSource->aimdirection;
	pDest->forwardmove = pSource->forwardmove;
	pDest->sidemove = pSource->sidemove;
	pDest->upmove = pSource->upmove;
	pDest->buttons = pSource->buttons;
	pDest->impulse = pSource->impulse;
	pDest->weaponselect = pSource->weaponselect;
	pDest->weaponsubtype = pSource->weaponsubtype;
	pDest->random_seed = pSource->random_seed;
	pDest->mousedx = pSource->mousedx;
	pDest->mousedy = pSource->mousedy;
	pDest->hasbeenpredicted = pSource->hasbeenpredicted;
}
CMoveData *move_data = new CMoveData;

/*
if we call updateaccuracy it will use predicted velocity
so if we ran engine pred ( at start of cm with full forward sidemove of origina input ) the velo will increase
therefor if u then call updateaccuracy and get spread/inaccurcy value will not be accuracy
so we set forward / sidemove to 0.f before predicting, restore after, then after we call update, store spread inaccuracy and use those values
in ragebot when we shoot forward sidemove 0.f and spread should be identical to the server*/

void CRunCommand::RunPrediction( CBaseEntity *pLocal, CUserCmd *pUserCmd )
{
	if( g_pPred == NULL || g_pMoveHelper == NULL || g_pGameMovement == NULL )
		return;

	if (g_pClientState->GetDeltaTick() > 0)
		g_pPred->Update(g_pClientState->GetDeltaTick(), g_pClientState->GetDeltaTick() > 0, g_pClientState->GetLastCmdAck(), g_pClientState->GetLastOutGoingCmd() + g_pClientState->GetChokedCmds());

	g_Misc.FixTickbase(pUserCmd, pLocal);

	static DWORD dwRunCommandAddress = g_pRunCommandHOOKED->dwGetMethodAddress( 19 );
	static DWORD dwPredictionSeedAddress = *( DWORD* ) ( dwRunCommandAddress + 0x30 ); //works
	static DWORD dwPredictionPlayerAddress = *( DWORD* ) ( dwRunCommandAddress + 0x54 ); //works

	static WORD wPlayerCommand = *( WORD* ) ( dwRunCommandAddress + 0x1B ); //works
	static WORD wCurrentCommand = *( WORD* ) ( dwRunCommandAddress + 0x21 ); //works

	float flOldCurTime = g_pGlobalVars->curtime;
	float flOldFrameTime = g_pGlobalVars->frametime;

	int iButtons = pUserCmd->buttons;
	int iTickBase = pLocal->GetTickBase();

	g_pGlobalVars->curtime = iTickBase * g_pGlobalVars->interval_per_tick;
	g_pGlobalVars->frametime = g_pGlobalVars->interval_per_tick;

	bool v3 = *( bool* ) ( ( uintptr_t ) g_pPred + 8 );
	*( bool* ) ( ( uintptr_t ) g_pPred + 8 ) = true;

	//restore predicted vars

	pUserCmd->buttons |= ( ( DWORD ) pLocal + 0x3330 );

	int v16 = pUserCmd->buttons;
	int v17 = pUserCmd->buttons ^ *( int* ) ( ( DWORD ) pLocal + 0x31F8 );

	*( int* ) ( ( DWORD ) pLocal + 0x31EC ) = *( int* ) ( ( DWORD ) pLocal + 0x31F8 );
	*( int* ) ( ( DWORD ) pLocal + 0x31F8 ) = v16;
	*( int* ) ( ( DWORD ) pLocal + 0x31F0 ) = v16 & v17;
	*( int* ) ( ( DWORD ) pLocal + 0x31F4 ) = v17 & ~v16;

	if( *( bool* ) ( ( uintptr_t ) g_pPred + 10 ) )
		g_pGlobalVars->frametime = 0;

	*( DWORD* ) ( ( DWORD ) pLocal + Offsets::Player::m_pCurrentCommand ) = ( DWORD ) pUserCmd;

	int iOldPredSeed = *( int* ) dwPredictionSeedAddress;

	*( int* ) dwPredictionSeedAddress = pUserCmd->random_seed;
	g_pMoveHelper->SetHost( pLocal );

	g_pPred->SetupMove( pLocal, pUserCmd, NULL, move_data );
	g_pGameMovement->ProcessMovement( pLocal, move_data );

	*( int* ) ( ( DWORD ) pLocal + Offsets::Player::m_nTickBase ) = iTickBase;

	g_pPred->FinishMove( pLocal, pUserCmd, move_data );

	*( bool* ) ( ( uintptr_t ) g_pPred + 8 ) = v3;
	*( int* ) dwPredictionSeedAddress = iOldPredSeed;

	pUserCmd->buttons = iButtons;

	g_pGlobalVars->curtime = flOldCurTime;
	g_pGlobalVars->frametime = flOldFrameTime;
}

using RunCommand_t = void( __thiscall* )( void*, CBaseEntity*, CUserCmd*, void* );

void __stdcall HOOKED_RunCommand( CBaseEntity* pEntity, CUserCmd* pCmd, void* moveHelper )
{
	static auto dwOriginRunCommand = g_pRunCommandHOOKED->GetFunctionAddress< RunCommand_t>( 19 );

	dwOriginRunCommand( g_pPred, pEntity, pCmd, moveHelper );

	g_pMoveHelper = ( IMoveHelper* )moveHelper;

	g_Misc.StorePunch( pEntity );
}