#include "createmove.h"

namespace UserCmd
{
	bool bQueuedShot = false;
	bool bInFakeDuck = false;
	bool bBreakingLC = false;
	int iOriginalFlags = 0;
	int iTickShift = 0;
	float flFixedCurTime = 0.f;
	float flTeleportDistance = 0.f;
	Vector vAntiAimReal = Vector( 0, 0, 0 );
	Vector vFakeAngles = Vector( 0, 0, 0 );
	Vector vOriginalView = Vector( 0, 0, 0 );
	Vector vViewAngles = Vector( 0, 0, 0 );
	Vector vHitPosition = Vector(0, 0, 0);
	CUserCmd* pCmd = nullptr;
	std::vector<sLocalData> vLocalRecord;
	std::vector<int> sent_cmd_numbers;
}

bool __fastcall HOOKED_CreateMove( void* thisptr, int edx, float frametime, CUserCmd* pCmd )
{
	g_AntiAim.m_bInAntiAim = false;
	g_Rage.bInRage = false;

	UserCmd::pCmd = pCmd;

	if( !pCmd || pCmd->command_number == 0 )
		return false;

	static void* _MD5_PseudoRandom = nullptr;

	// NOTE: the first 0xE8 call instruction = MD5_PseudoRandom so why not calculate the address of it from CInput::CreateMove
	if( !_MD5_PseudoRandom )
	{
		auto retnAddress = ( DWORD ) _ReturnAddress() + 0x3F;
		retnAddress += *( DWORD* ) retnAddress + 4;
		_MD5_PseudoRandom = ( PVOID ) retnAddress;
	}

	pCmd->random_seed = reinterpret_cast<int( __thiscall* )( int )>( _MD5_PseudoRandom )( pCmd->command_number ) & 0x7FFFFFFF;

	PDWORD pdwEBP;

	__asm MOV pdwEBP, EBP;

	static bool bbb;
	bool& bSendPackets = *(bool*)(*pdwEBP - 0x1C);

	if (!g_pEngine->IsInGame())
		return false;

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal == nullptr )
		return false;

	if( pLocal->GetLifeState() != LIFE_ALIVE )
		return false;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if( pWeapon == nullptr )
		return false;

	g_Misc.HandleBuybot(pLocal);

	UserCmd::iOriginalFlags = pLocal->GetFlags();
	UserCmd::vOriginalView = pCmd->viewangles;

	//meme to remove crouch stamina
	pCmd->buttons |= IN_BULLRUSH;

	//we want ppl to use cursor etc so we have to stop them from shooting while clicking around in the menu
	if( g_Gui.bCursorActive )
	{
		if( GetAsyncKeyState( 1 ) )
			pCmd->buttons &= ~IN_ATTACK;

		if( GetAsyncKeyState( 2 ) )
			pCmd->buttons &= ~IN_ATTACK2;
	}

	g_Misc.Movement( pLocal, pCmd );
	g_Prediction.RunPrediction( pLocal, pCmd );
	g_AntiAim.PredictLowerBodyUpdate( pLocal );

	if( g_Var.Visuals.World.Grenade[ 2 ] )
		g_pGrenadePrediction->Tick( pCmd->buttons );

	if( g_Var.Visuals.World.Crosshair[ 1 ] )
		g_Visuals.InitRecoil( pCmd, pLocal );

	g_AntiAim.Run( pLocal, pWeapon, pCmd, bSendPackets );

	if (GetAsyncKeyState(g_Var.Misc.FakeDuck) && g_Var.Misc.FakeDuck > 0 && (UserCmd::iOriginalFlags & FL_ONGROUND))
	{
		UserCmd::bInFakeDuck = true;

		static int iDrop = 0;

		iDrop = 14;

		if (g_pClientState->GetChokedCmds() >= (iDrop / 2))
			pCmd->buttons |= IN_DUCK;
		else
			pCmd->buttons &= ~IN_DUCK;

		if (iDrop > g_pClientState->GetChokedCmds())
			bSendPackets = false;
	}
	else
		UserCmd::bInFakeDuck = false;

	if( g_Var.Misc.KnifeBot[0] )
		g_Misc.DoKnifeBot( pLocal, pWeapon, pCmd );

	g_Misc.AutoRevolver(pLocal, pWeapon, pCmd);

	g_Rage.RunRage( pLocal, pWeapon, pCmd );

	g_Rage.AutoStop( pLocal, pWeapon, pCmd );

	if( g_Var.Misc.ZeusBot[0] )
		g_Rage.RunZeus( pLocal, pWeapon, pCmd );

	if( g_Var.AntiAim.Resolve > 0 )
		g_Resolver.InitCM( pLocal, pWeapon );

	g_Math.FixMovement( UserCmd::vOriginalView, pCmd, pLocal );

	if( g_Var.Misc.Untrusted )
	{
		g_Math.NormalizeVector( pCmd->viewangles );
		g_Math.ClampAngles( pCmd->viewangles );
	}

	if(pLocal->GetShotsFired() == 1)
		UserCmd::iTickShift = -10;

	if( g_AntiAim.m_bLbyUpdate && g_Var.AntiAim.Break == 2 )
	{
		if( pWeapon->GetWeaponID() == WEAPON_R8REVOLVER )
		{
			pCmd->buttons &= ~IN_ATTACK;
			pCmd->buttons &= ~IN_ATTACK2;
		}
		else
			if( !pWeapon->IsOther() )
				pCmd->buttons &= ~IN_ATTACK;
	}

	if( !g_pClientState->GetChokedCmds() )
	{
		static Vector vLastOrigin = pLocal->GetOrigin();
		if (pLocal->GetOrigin() != vLastOrigin)
		{
			Vector vDelta = pLocal->GetOrigin() - vLastOrigin;
			UserCmd::flTeleportDistance = vDelta.Length2DSqr();
			UserCmd::bBreakingLC = UserCmd::flTeleportDistance > 4096.f;
			vLastOrigin = pLocal->GetOrigin();
		}

		UserCmd::bQueuedShot = false;

		if( !( g_Rage.bInRage || g_AntiAim.m_bInAntiAim ) )
			UserCmd::vAntiAimReal = pCmd->viewangles;
	}
	else
	{
		// due to valve's psilent fix, a weird issue was introduced
		// if there is a choked shot queued, any follow up shots UNTIL AFTER the next send, will keep using the angles from the first choked shot
		if( pCmd->buttons & IN_ATTACK && g_Misc.CanShoot( pLocal, pWeapon ) )
			UserCmd::bQueuedShot = true;

		UserCmd::vFakeAngles = pCmd->viewangles;
	}

	if( g_pClientState->GetChokedCmds() >= 14 )
	{
		bSendPackets = true;

		if( pCmd->buttons & IN_ATTACK )
			pCmd->buttons &= IN_ATTACK;
	}

	// instant event fix
/*	if (bSendPackets)
	{
		// store cmd number for PacketStart, so we can confirm only the packets we want to confirm
		UserCmd::sent_cmd_numbers.push_back(pCmd->command_number);
	}
	else
		if (g_pClientState->GetChokedCmds() > 0 && g_pClientState->GetChokedCmds() % 4 == 0)
		{
			// backup stuff that SendDatagram will mess with
			int choked_packets = g_pClientState->GetChokedCmds();

			// dont let server know we have choked packets, its a SURPRISE!
			g_pClientState->GetChokedCmds() = 0;

			// send an empty datagram to keep communication going
			g_pClientState->GetNetChannel()->SendDatagram(nullptr);

			// SendDatagram incremented this, REVERT
			g_pClientState->GetNetChannel()->m_nOutSequenceNr--;

			// let rest of game know what we are really doing
			g_pClientState->GetChokedCmds() = choked_packets;
		}*/

    UserCmd::vViewAngles = pCmd->viewangles;

	return false;
}