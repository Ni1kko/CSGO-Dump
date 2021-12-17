#include "writeusercmd.h"
#include "Detour.h"

namespace AdjustPlayerTickHook
{
	Detour _d;

	void __fastcall Adjust_Player_Time(void* ecx, void* edx, int simulation_ticks)
	{
		using org_fn = void(__thiscall*)(void*, int);

		int tickbase_pre = *(int*)(uintptr_t(ecx) + 0x10A0);

		_d.GetOriginal<org_fn>()(ecx, simulation_ticks);

		*(int*)(uintptr_t(ecx) + 0x1244) = 16;

		int tickbase_post = *(int*)(uintptr_t(ecx) + 0x10A0);

		int limit = *(int*)(uintptr_t(ecx) + 0x1244);

		printf("SIMTICKS: %d | PRE: %d | POST: %d | ADJUSTED: %d | LIMIT %d \n", simulation_ticks, tickbase_pre, tickbase_post, tickbase_pre - tickbase_post, limit);
	}

	void SetupHook()
	{
		DWORD PatternResult = g_Pattern.PatternSearchIDA("server.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 85");

		if (!PatternResult)
			throw "Failed to signature scan AdjustPlayerTime!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(PatternResult), reinterpret_cast<uint8_t*>(Adjust_Player_Time));
	}
}

/*hook this, will make testing a lot easier
55 8B EC 51 53 56 8B 75 08 8B D9 85*/

WriteUserCmdFn oWriteUserCmd;

class CCLCMsg_Move 
{
private:
	char cPad[8];
public:
	int iBackupCmd;
	int iNewCmd;
};

bool shift_backward(CCLCMsg_Move* msg, int shift, void* ecx, void* edx, int slot, void* buffer)
{
	static uintptr_t WriteUserCMD = g_Pattern.PatternSearchIDA(g_Offset.szClient.c_str(), "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");

	// store the real command count
	int new_commands = msg->iNewCmd;

	// manipulate CCLCMsg_Move
	int total_new_commands = new_commands + shift;
	msg->iNewCmd = total_new_commands;
	msg->iBackupCmd = 0;

	// write real commands    
	int next_cmd_number = g_pClientState->GetLastOutGoingCmd() + g_pClientState->GetChokedCmds() + 1;

	int from = -1;
	int to = next_cmd_number - new_commands + 1;

	for (; to <= next_cmd_number; to++)
	{
		if (!oWriteUserCmd(ecx, slot, buffer, from, to, true))
			return false;

		from = to;
	}

	// write fake commands
	CUserCmd* last_real_cmd = g_pInput->GetUserCmd(slot, from);

	if (last_real_cmd)
	{
		CUserCmd from_cmd = *last_real_cmd;
		CUserCmd to_cmd = from_cmd;

		to_cmd.command_number++;
		to_cmd.tick_count += 200; // prevent server from executing fake commands, due to the 2.5 sec check

		for (int i = new_commands; i <= total_new_commands; i++)
		{
			reinterpret_cast<void(__fastcall*)(void*, CUserCmd*, CUserCmd*)>(WriteUserCMD)(buffer, &to_cmd, &from_cmd);

			from_cmd = to_cmd;
			to_cmd.command_number++;
			to_cmd.tick_count++;
		}
	}

	return true;
}
 
bool bSend = false, bFirstSend = false;
bool __fastcall HOOKED_WriteUserCmdDeltaToBuffer(void* ecx, void* edx, int slot, void* buf, int from, int to, bool newcmd)
{
	// get ebp
	uintptr_t _ebp;
	__asm mov _ebp, ebp;

	// only run if we have to shift our tickbase
	if (UserCmd::iTickShift == 0)
		return oWriteUserCmd(ecx, slot, buf, from, to, newcmd);

	// skip next calls
	if (from != -1)
		return true;

	// setup vars
	CCLCMsg_Move* msg = (CCLCMsg_Move*)(_ebp + 0xFCC);

	if (UserCmd::iTickShift < 0)
	{
		int shift = abs(UserCmd::iTickShift);
		UserCmd::iTickShift = 0;

		return shift_backward(msg, shift, ecx, edx, slot, buf);
	}

	return true;
}