#include "bhop.hpp"

#include "../ValveSDK/csgostructs.hpp"
#include "../Options.hpp"

void bhop::on_create_move(C_BasePlayer* local, CUserCmd* cmd)
{
  // 
  // Not pasted from aimtux, i swear!!!
  // 
  if(!g_Options.misc_bhop.get_bool())
    return;

  if (cmd->buttons & IN_JUMP && !(local->GetMoveType() & MOVETYPE_LADDER))
  {
      int iFlags = local->m_fFlags();
      if (!(iFlags & FL_ONGROUND))
          cmd->buttons &= ~IN_JUMP;
  }
}