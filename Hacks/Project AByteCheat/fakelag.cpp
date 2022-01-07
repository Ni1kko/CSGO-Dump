#include "fakelag.h"
#include "Autowall.h"
#include "RageBot.h"
#include <array>
#include "MiscHacks.h"
fakelag * c_fakelag = new fakelag();

int random_int(int min, int max)
{
	return rand() % max + min;
}
void fakelag::break_lagcomp(bool & send_packet, int ammount)
{
	static int factor = random_int(1, ammount);
	static int tick; tick++;
	bool m_switch = false;
	int m_stepdown;
	send_packet = !(tick % factor);

	if (send_packet)
	{
		int m_choke = tick % ammount + 1;

		m_stepdown = 15 - m_choke;
		if (m_choke >= ammount)
			m_switch = !m_switch;

		factor = m_switch ? m_stepdown : m_choke;

		if (factor >= 15)
			factor = 14;
	}

}

int fakelag::break_lagcomp_mm(float ammount)
{
	float speed = hackManager.pLocal()->GetVelocity().Length2D();
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * interfaces::globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, ammount);
	}
}
int fakelag::break_lagcomp_mm_2()
{
	float speed = hackManager.pLocal()->GetVelocity().Length2D();
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * interfaces::globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
}
int random_integer(int min, int max)
{
	return rand() % max + min;
}
namespace i_hate_visual_studio
{
	template<class T, class U>
	T clamp(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	
}
static bool whatever = false;
void fakelag::when_enemy_sight(bool &bSendPacket, IClientEntity * local, float ammount, bool &break_lc)
{
	int s = break_lc ? break_lagcomp_mm(ammount) : ammount;

	if (ragebot->can_autowall)
	{
		bSendPacket = ( s <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else
	{
		whatever = !whatever;
		if (whatever)
			bSendPacket = true;
		else
			bSendPacket = false;
	}
}

void fakelag::mirror_adaptive(bool &packet, IClientEntity * local, int ammount, bool break_lc, CUserCmd* cmd)
{
	bool done = false;
	int choked;
	auto anim_state = local->GetBasePlayerAnimState();
	if (cmd->buttons & IN_ATTACK)
	{
		packet = (2 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else if (local->GetVelocity().Length2D() >= 120.f)
	{
		if (ragebot->can_autowall)
		{
			break_lagcomp_mm(13);
		}
		else
		{
			break_lagcomp_mm(6);
		}
	}
	else if (anim_state &&  anim_state->m_bInHitGroundAnimation && local->GetVelocity().Length2D() < 20)
	{
		packet = (14 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else if (local->GetVelocity().Length2D() < 120 && local->GetVelocity().Length2D() > 40.f)
	{
		if (break_lc)
			break_lagcomp(packet, ammount);
		else
			packet = (ammount + 1 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else
	{
		whatever = !whatever;
		if (whatever)
			packet = true;
		else
			packet = false;
	}
}


void fakelag::aimware_adaptive(bool &send_packet, IClientEntity * local, int ammount)
{
	if (!local)
		return;

	float ExtrapolatedSpeed;
	int WishTicks;
	float VelocityY;
	float VelocityX;
	int WishTicks_1;
	signed int AdaptTicks;
	WishTicks = (ammount + 0.5) - 1;

	VelocityY = local->GetVelocity().y;
	VelocityX = local->GetVelocity().x;
	WishTicks_1 = 0;
	AdaptTicks = 2;
	ExtrapolatedSpeed = sqrt((VelocityX * VelocityX) + (VelocityY * VelocityY))
		* interfaces::globals->interval_per_tick;

	while ((WishTicks_1 * ExtrapolatedSpeed) <= 68.0)
	{
		if (((AdaptTicks - 1) * ExtrapolatedSpeed) > 68.0)
		{
			++WishTicks_1;
			break;
		}
		if ((AdaptTicks * ExtrapolatedSpeed) > 68.0)
		{
			WishTicks_1 += 2;
			break;
		}
		if (((AdaptTicks + 1) * ExtrapolatedSpeed) > 68.0)
		{
			WishTicks_1 += 3;
			break;
		}
		if (((AdaptTicks + 2) * ExtrapolatedSpeed) > 68.0)
		{
			WishTicks_1 += 4;
			break;
		}
		AdaptTicks += 5;
		WishTicks_1 += 5;
		if (AdaptTicks > 16)
			break;
	}

	bool should_choke;

	if (*(int*)(uintptr_t(interfaces::client_state) + 0x4D28) < WishTicks_1 && *(int*)(uintptr_t(interfaces::client_state) + 0x4D28) < 15)
		should_choke = true;
	else
		should_choke = false;

	if (should_choke)
		send_packet = (WishTicks_1 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));

	else
		send_packet = (4 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
}

void fakelag::velocity(bool &packet, IClientEntity * local, int ammount, bool break_lc)
{
	static int factor = random_integer(ammount, 15);
	static int tick; tick++;

	packet = !(tick % factor);
	float flVelocity = local->GetVelocity().Length2D() * interfaces::globals->interval_per_tick;
	factor = i_hate_visual_studio::clamp(static_cast<int>(std::ceil(30.f / flVelocity)), ammount, 15);
}

void fakelag::Fakelag(CUserCmd *pCmd, bool &bSendPacket) // terrible code do not look please :(
{
	IClientEntity* pLocal = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)interfaces::ent_list->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!interfaces::engine->IsConnected() || !interfaces::engine->IsInGame())
		return;

	if (pCmd->buttons & IN_USE)
		return;

	if (pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (GameUtils::IsGrenade(pWeapon))
		return;

	if (interfaces::m_iInputSys->IsButtonDown(ButtonCode_t::MOUSE_LEFT))
		return;

	if (!options::menu.AntiAimTab.FEnable.GetState())
		return;


	if (GameUtils::IsRevolver(pWeapon))
	{
		whatever = !whatever;
		if (whatever)
			bSendPacket = true;
		else
			bSendPacket = false;
		return;
	}

	static auto choked = 0;
	float flVelocity = pLocal->GetVelocity().Length2D() * interfaces::globals->interval_per_tick;
	bool in_air = !(hackManager.pLocal()->GetFlags() & FL_ONGROUND);
	bool break_lc = options::menu.MiscTab.FakelagBreakLC.GetState();
	int fakelag_standing = 1;
	int fakelag_moving = options::menu.AntiAimTab.Limit.GetValue();
	int fakelag_inair = options::menu.AntiAimTab.Limit.GetValue();

//	std::vector<dropdownboxitem> spike = options::menu.MiscTab.fl_spike.items;

	int type = options::menu.AntiAimTab.Amount.GetIndex();

	if (GetAsyncKeyState(options::menu.AntiAimTab.FakeCrouchKey.GetKey()) && options::menu.AntiAimTab.FakeCrouch.GetState())
	{
		bSendPacket = (14 <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
	}
	else
	{
		switch (type)
		{
			case 0: //static
			{
				if (pLocal->IsMoving())
				{
					bSendPacket = (fakelag_moving <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
				}
				else
				{
					bSendPacket = (fakelag_standing <= *(int*)(uintptr_t(interfaces::client_state) + 0x4D28));
				}
			}
			break;

			case 1: //dynamic
			{
				velocity(bSendPacket, pLocal, fakelag_moving, break_lc);
			}
			break;

			case 2: //break
			{
				mirror_adaptive(bSendPacket, pLocal, fakelag_moving, break_lc, pCmd);
			}
			break;

			case 3: // Aimware Adaptive (not in menu)
			{
				aimware_adaptive(bSendPacket, pLocal, fakelag_moving);
			}
			break;

			case 4: // enemy sight (not in menu)
			{
				when_enemy_sight(bSendPacket, pLocal, in_air ? fakelag_inair : fakelag_moving, break_lc);
			}
			break;
			//				mirror_adaptive(bSendPacket, pLocal, in_air ? fakelag_inair : fakelag_moving, break_lc, pCmd);
		}
	}
}
























































































































































































