#pragma once
#include "HookIncludes.h"
#include "Misc.h"
#include "MovementRecorder.h"
#include "RageBot.h"
#include "LegitBot.h"
#include "GrenadePrediction.h"
#include "LagComp.h"

using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
QAngle qLastTickAngles = QAngle(0.0f, 0.0f, 0.0f);
void __stdcall CHLCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
{
    static auto ofunc = hooks::client.get_original<create_move_t>(21);
    ofunc(g_CHLClient, sequence_number, input_sample_frametime, active);
	C_BaseEntity *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CInput::CUserCmd *cmd = g_Input->GetUserCmd(0, sequence_number);
	if (!cmd)
		return;

    if (!cmd->command_number)
        return;

	CInput::CVerifiedUserCmd *verifiedCommands = *(CInput::CVerifiedUserCmd **)(reinterpret_cast< uint32_t >(g_Input) + 0xF0);
	CInput::CVerifiedUserCmd *verified = &verifiedCommands[sequence_number % 150];
	if (!verified)
		return;

    if (!local)
        return;
    QAngle oldAngle = cmd->viewangles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

    backtracking->legitBackTrack(cmd, local);



    //RankReveal(cmd);

    misc::instance().OnCreateMove(cmd, local);
    legitbot::instance().OnCreateMove(cmd, local);
    ragebot::instance().OnCreateMove(cmd, bSendPacket);

    grenade_prediction::instance().Tick(cmd->buttons);

    if (!bSendPacket && g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;
    else if (bSendPacket && !g_Options.Ragebot.YawFake) qLastTickAngles = cmd->viewangles;
        
    if (!sanitize_angles(cmd->viewangles))
        return;

    movementfix(oldAngle, cmd, oldForward, oldSideMove);
    clampMovement(cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}
#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	input_sample_frametime
		PUSH	sequence_number
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}