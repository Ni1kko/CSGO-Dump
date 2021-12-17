#include "packetstart.h"
#include "Detour.h"

namespace PacketStart
{
	Detour _d;
	using org_fn = bool(__thiscall*)(CBaseEntity*);
	org_fn org_call = nullptr;

	void __fastcall HOOKED_PacketStart(void* ecx, void* edx, int incoming_sequence, int outgoing_acknowledged)
	{
		CBaseEntity* pLocal = g_pEntList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (!pLocal)
		{
			uintptr_t clientstate = uintptr_t(ecx);

			//80 3D ? ? ? ? ? 56 75 14 | *(_DWORD *)(v0 + 0x0) - *(_DWORD *)(v0 + 0x4D2C) <- outgoing acknowledged
			//search immidant value of outgoin to get func with incoming sequence
			//IDA Style Sig: 55 8B EC 8B 45 08 89 81 ? ? ? ? 8B 45 0C 89 81 ? ? ? ? 5D C2 08 00
			*(int*)(clientstate + 0x114) = incoming_sequence;
			*(int*)(clientstate + 0x4D2C) = outgoing_acknowledged;

			return;
		}

		for (auto cmd_number = UserCmd::sent_cmd_numbers.begin(); cmd_number != UserCmd::sent_cmd_numbers.end();)
		{
			if (*cmd_number == outgoing_acknowledged)
			{
				uintptr_t clientstate = uintptr_t(ecx);

				*(int*)(clientstate + 0x114) = incoming_sequence;
				*(int*)(clientstate + 0x4D2C) = outgoing_acknowledged;

				UserCmd::sent_cmd_numbers.erase(cmd_number);
				return;
			}
			cmd_number++;
		}
	}

	void SetupHook()
	{
		DWORD dwClientState = g_Pattern.PatternSearchIDA("engine.dll", "C7 07 ? ? ? ? 8D B7") + 2;
		uintptr_t packet_start_address = *(uintptr_t*)(dwClientState + 5 * sizeof(uintptr_t));

		DWORD* dwClientStateDT = (DWORD *)dwClientState;
		dwClientStateDT = (DWORD*)dwClientStateDT[0];

		if (!packet_start_address)
			throw "Failed to signature scan PacketStart!";

		_d.SetupHook(reinterpret_cast<uint8_t*>(dwClientStateDT[5]), reinterpret_cast<uint8_t*>(HOOKED_PacketStart));
		org_call = _d.GetOriginal<org_fn>();
	}
}