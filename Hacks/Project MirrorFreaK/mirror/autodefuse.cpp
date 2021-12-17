#include "autodefuse.h"
void defususmaximus(CUserCmd* cmd) {
	if (!Options::Menu.MiscTab.AutoDefuse.GetState())
		return;
	IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!localplayer || !localplayer->IsAlive())
		return;
	if (localplayer->team() != 3)
		return;
	CCSBomb* bomb = nullptr;
	for (int i = 1; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity* entity = Interfaces::EntList->GetClientEntity(i);
		if (!entity)
			continue;
		if (entity->GetClientClass()->m_ClassID == 108)
		{
			bomb = (CCSBomb*)entity;
			break;
		}
		if (!bomb || bomb->IsBombDefused())
			return;
		float flBlow = bomb->GetC4BlowTime();
		auto local = hackManager.pLocal();
		float lifetime = flBlow - (Interfaces::Globals->interval_per_tick * local->GetTickBase());
		if (localplayer->HasDefuser() && lifetime > 5.5f)
			return;
		if (!localplayer->HasDefuser() && lifetime > 10.5f)
			return;
		if (bomb->IsBombDefused())
			return;
		float distance = localplayer->m_VecORIGIN().DistTo(bomb->GetAbsOrigin());
		if (distance <= 75.0f)
			cmd->buttons |= IN_USE;
	}
}