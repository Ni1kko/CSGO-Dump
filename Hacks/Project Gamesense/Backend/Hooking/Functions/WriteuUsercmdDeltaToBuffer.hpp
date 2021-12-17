#pragma once
#include "../../SDK/GameEvents.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../Hooks/Hooks.h"
#include "../../Backend/Configuration/Config.h"

typedef bool(__thiscall* fnDoubletap)(void*, int, bf_write*, int, int, bool);
fnDoubletap oWriteUsercmdDeltaToBuffer = nullptr;

typedef void(__thiscall* MoveFn)(void*, void*);
MoveFn oMove;

namespace Cheat
{
	namespace Hooked
	{
        void __fastcall Move(void* ecx, void* edx)
        {
            if (Cheat::Settings->UnloadCheat)
            {
                Cheat::Settings->UnloadReadyTable.Move = 0;
                return oMove(ecx, edx);
            }
            Cheat::Settings->UnloadReadyTable.Move = (int)GetTickCount();

            if (!G::LocalPlayer->IsAlive())
               return oMove(ecx, edx);

            if (G::Recharge > 0 && c_config::get()->b["rage_doubletap"] && c_keyhandler::get()->auto_check("rage_exploit_key"))
            {
                G::Recharged = false;
                G::Recharge--;
                return;
            }
            else {
                G::Recharged = true;
                oMove(ecx, edx);
            }
        }

		void send_move() {
			using fnCLSendMove = void(__cdecl*)();
			static auto sendmove = (fnCLSendMove)Cheat::Utilities->Memory_PatternScan("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98");

			sendmove();
		}

		void write_usercmd(void* bf, void* from, void* to) {
			static auto fn = Cheat::Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");

			__asm {
				mov edx, to
				mov ecx, bf
				push from
				call fn
				add esp, 4
			}
		}

		bool __fastcall hooked_writeusercmddeltatobuffer(void* ecx, void*, int slot, bf_write* buf, int from, int to, bool isnewcommand) {
            static auto retn = Cheat::Utilities->Memory_PatternScan("engine.dll", "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?");

            if (Cheat::Settings->UnloadCheat)
            {
                Cheat::Settings->UnloadReadyTable.writeusercmddeltatobuffer = 0;
                return oWriteUsercmdDeltaToBuffer(ecx, slot, buf, from, to, isnewcommand);
            }
            Cheat::Settings->UnloadReadyTable.writeusercmddeltatobuffer = (int)GetTickCount();

            if (!G::LocalPlayer || (G::TickbaseShift <= 0 && G::Recharge <= 0))
                return oWriteUsercmdDeltaToBuffer(ecx, slot, buf, from, to, isnewcommand);

            if (from != -1)
                return true;

           /* if (G::Recharge > 0) {
                G::Recharge--;
                return false;
            }*/

            auto p_new_commands = (int*)(reinterpret_cast <uintptr_t> (buf) - 0x2C);
            auto p_backup_commands = (int*)(reinterpret_cast <uintptr_t> (buf) - 0x30);
            auto new_commands = *p_new_commands;

            auto channel = Interfaces::ClientState->pNetChannel;

            if (G::TickbaseShift > 0) {
                auto next_cmd_nr =
                    *(int*)((DWORD_PTR)Interfaces::ClientState + 0x4D24) + *(int*)((DWORD_PTR)Interfaces::ClientState + 0x4D28) + 1;
                auto total_new_commands = clamp(G::TickbaseShift, 0, 16);

                G::TickbaseShift -= total_new_commands;
                //channel->m_nOutSequenceNr += total_new_commands;

                from = -1;

                *p_new_commands = total_new_commands;
                *p_backup_commands = 0;

                for (to = next_cmd_nr - new_commands + 1; to <= next_cmd_nr; to++) {
                    if (!oWriteUsercmdDeltaToBuffer(ecx, slot, buf, from, to, isnewcommand))
                        return false;

                    from = to;
                }

                CUserCmd* last_real_cmd = Interfaces::Input->GetUserCmd(slot, from);
                CUserCmd from_cmd;

                if (last_real_cmd)
                    memcpy(&from_cmd, last_real_cmd, sizeof(CUserCmd));

                CUserCmd to_cmd;
                memcpy(&to_cmd, &from_cmd, sizeof(CUserCmd));

                to_cmd.command_number++;
                to_cmd.tick_count++;

                for (int i = new_commands; i <= total_new_commands; i++) {

                    write_usercmd(buf, &to_cmd, &from_cmd);
                    memcpy(&from_cmd, &to_cmd, sizeof(CUserCmd));
                    to_cmd.command_number++;
                    to_cmd.tick_count++;
                }

                //*(int*)((DWORD_PTR)Interfaces::ClientState + 0x4D24) += total_new_commands;
                //channel->send_datagram(false);
            }

            return true;
		}
	}
}