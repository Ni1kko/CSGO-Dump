#include "../MISC/includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/FakeLag.h"

int random_int(int min, int max)
{
	return rand() % max + min;
}
int CFakeLag::lag_comp_break()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player || local_player->GetHealth() <= 0)
		return 1;

	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;
	auto speed = velocity.Length();
	auto distance_per_tick = speed * INTERFACES::Globals->interval_per_tick;
	int choked_ticks = std::ceilf(64.f / distance_per_tick);
	return std::min<int>(choked_ticks, 14.f);
}

int WishTicks; // ecx
int WishTicks_1; // edx
signed int AdaptTicks; // ecx
float ExtrapolatedSpeed; // xmm1_4

void CFakeLag::do_fakelag(SDK::CUserCmd* cmd)
{
	GLOBAL::should_send_packet = true;
	int choke_amount;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	auto net_channel = INTERFACES::Engine->GetNetChannel();
	if (!net_channel)
		return;


	if (SETTINGS::settings.lag_bool)
	{
		static auto packets_choked = 0;
		static int choked_ticks = 0;
		int flTimer = -1;

		if (fake_walk) {
			if ((local_update - INTERFACES::Globals->curtime) > 0.22) {
				choke_amount = SETTINGS::settings.Fakelagamount;
			}
			else {
				choke_amount = 1;
			}

		}
		else
		{
			choke_amount = 1;
			if (!(local_player->GetFlags() & FL_ONGROUND))
			{
				
				choke_amount = SETTINGS::settings.Fakelagamount;
				
			}
			else if (local_player->GetVelocity().Length2D() > 0.1)
			{
				
				choke_amount = SETTINGS::settings.Fakelagamount;
				
			}
			else if (local_player->GetVelocity().Length2D() < 0.1)
			{
				choke_amount = SETTINGS::settings.Fakelagamount;

			}
			else if (SETTINGS::settings.breaklagcomp)
			{
				choke_amount = lag_comp_break();
			}

		}
	    if (SETTINGS::settings.addabtivelag)
		{
			float VelocityY = local_player->GetVelocity().y;
			float VelocityX = local_player->GetVelocity().x;
			WishTicks_1 = 0;
			AdaptTicks = 2;
			ExtrapolatedSpeed = sqrt((VelocityX * VelocityX) + (VelocityY * VelocityY))
				* INTERFACES::Globals->interval_per_tick;

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

			if (packets_choked < WishTicks_1)
				choked_ticks = 1;


			if (flTimer > AdaptTicks) {
				choked_ticks = 1;
				flTimer = -1;
			}
			else {
				if (flTimer == AdaptTicks)
					choked_ticks = 0;

			}
			flTimer++;
		}
	}
	else
		choke_amount = 1;

	if (net_channel->m_nChokedPackets >= min(14, choke_amount))
		GLOBAL::should_send_packet = true;
	else
		GLOBAL::should_send_packet = false;
}

CFakeLag* fakelag = new CFakeLag();