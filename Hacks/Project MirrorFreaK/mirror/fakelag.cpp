#include "fakelag.h"
#include "Autowall.h"
#include "RageBot.h"
#include <array>

fakelag * c_fakelag = new fakelag();

int fakelag::break_lagcomp()
{
	float speed = hackManager.pLocal()->GetVelocity().Length2D();
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * Interfaces::Globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, in_air ? Options::Menu.MiscTab.Fakelagjump.GetValue() : Options::Menu.MiscTab.FakelagMove.GetValue());
	}
}

int fakelag::break_lagcomp_mm()
{
	float speed = hackManager.pLocal()->GetVelocity().Length2D();
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * Interfaces::Globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, in_air ? 9 : 10);
	}
}

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

static bool whatever = false;
void fakelag::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (GameUtils::IsGrenade(pWeapon))
		return;

	if (Interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;

	if (GameUtils::IsRevolver(pWeapon))
	{
		whatever = !whatever;
		if (whatever)
			bSendPacket = true;
		else
			bSendPacket = false;
	}

	float dmg = 0.f;
	float flVelocity = pLocal->GetVelocity().Length2D() * Interfaces::Globals->interval_per_tick;
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);
	static auto choked = 0;
	static int tick; tick++;
	static int factor = 8;
	const auto fakelag_standing = Options::Menu.MiscTab.FakelagStand.GetValue();
	const auto fakelag_moving = Options::Menu.MiscTab.FakelagBreakLC.GetState() ? break_lagcomp() : Options::Menu.MiscTab.FakelagMove.GetValue();
	const auto fakelag_inair = Options::Menu.MiscTab.FakelagBreakLC.GetState() ? break_lagcomp() : Options::Menu.MiscTab.Fakelagjump.GetValue();

	std::vector<dropdownboxitem> spike = Options::Menu.MiscTab.fl_spike.items;

	if ((spike[0].GetSelected && pCmd->buttons & IN_ATTACK) || (spike[1].GetSelected && fabs(pLocal->GetVelocity().z) <= 5.0f && !(pLocal->GetFlags() & FL_ONGROUND))
		|| (spike[2].GetSelected && ragebot->GetTargetCrosshair() != -1 && backup_awall->CanHit_2(ragebot->target_point, &dmg) && !(pCmd->buttons & IN_ATTACK))
		|| (GlobalBREAK::ticks_while_unducked != 0 && GlobalBREAK::ticks_while_unducked < 6 && spike[4].GetSelected))
	{
		if (choked < break_lagcomp()) // break lag comp
		{
			choked++;
			bSendPacket = false;
		}
		else
		{
			choked = 0;
			bSendPacket = true;
		}
	}

	else if (spike[5].GetSelected && GetAsyncKeyState(Options::Menu.MiscTab.fakelag_key.GetKey()))
	{
		if (Interfaces::m_pClientState->chokedcommands != 0)
		{
			bSendPacket = true;
		}
	}

	else if (pLocal->IsMoving() && spike[3].GetSelected && (pLocal->GetFlags() & FL_ONGROUND) && (!(fabs(pLocal->GetVelocity().z) <= 10.0f)))
	{
		bSendPacket = !(tick % factor);

		if (bSendPacket)
		{
			factor = clamp(static_cast<int>(std::ceil(69.f / flVelocity)), 1, 14);
			factor = clamp(static_cast<int>(std::ceil(64.f / hackManager.pLocal()->GetVelocity().Length2D())), 1, 14);
		}
	}

	else
	{
		if (in_air ? (choked < fakelag_inair) : (choked < fakelag_moving) && pLocal->IsMoving())
		{
			choked++;
			bSendPacket = false;
		}
	
		else if (choked < fakelag_standing && !pLocal->IsMoving())
		{
			choked++;
			bSendPacket = false;
		}

		else
		{
			choked = 0;
			bSendPacket = true;
		}
	}
}
