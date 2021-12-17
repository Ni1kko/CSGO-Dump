#include "FakeLag.h"

int CFakeLag::Fakelag_AdaptiveFactor()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return 0;

	auto velocity = local_player->GetVelocity();;
	auto speed = velocity.Length();
	auto distance_per_tick = speed *
		g_pGlobalVars->intervalPerTick;
	//choked_ticks = std::ceilf(TELEPORT_DISTANCE / distance_per_tick);
	//return std::min<int>(choked_ticks, MAX_CHOKE);
	return 0;
}
int iChoke = 1;

void CFakeLag::do_fakelag(CUserCmd* cmd, C_BaseEntity* local)
{
	if (g_pEngine->IsVoiceRecording())
		return;

	if (!g_Settings.bFakeLagShooting && cmd->buttons & IN_ATTACK)
		return;

	auto NetChannel = g_pEngine->GetNetChannel();

	if (!NetChannel)
		return;

	bool standing = local->GetVelocity().Length2D() <= 0.1f;
	bool accelerating = local->GetVelocity().Length2D() >= 0.1f && local->GetVelocity().Length2D() <= 150.f;
	bool onhighspeed = local->GetVelocity().Length2D() >= 265.f;
	bool onground = local->GetFlags() & FL_ONGROUND;
	bool running = onground && accelerating || local->GetVelocity().Length2D() >= 175.f;

	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;

	if (g_Settings.bFakeLagFlags[0] && (onground && standing))
		flag1 = true;

	if (g_Settings.bFakeLagFlags[1] && accelerating)
		flag2 = true;

	if (g_Settings.bFakeLagFlags[2] && onhighspeed)
		flag3 = true;

	if (g_Settings.bFakeLagFlags[3] && running)
		flag4 = true;


	if (flag1 || flag2 || flag3 || flag4)
		iChoke = g_Settings.iFakeLagChoke;
	else
		return;

	if (Globals::fakeducked)
	{
		if (NetChannel->m_nChokedPackets < 15)
		{
			Globals::bsendpacket = false;
		}
		else
		{
			Globals::bsendpacket = true;
		}
	}
	else
	{
		if (g_Settings.iFakeLagType == 1)
		{
			if (NetChannel->m_nChokedPackets < iChoke)
			{
				Globals::bsendpacket = false;
			}
			else
			{
				Globals::bsendpacket = true;
			}
		}
		else if (g_Settings.iFakeLagType == 2)
		{
			auto velocity = g::pLocalEntity->GetVelocity();;

			auto speed = velocity.Length();

			auto distance_per_tick = speed * g_pGlobalVars->intervalPerTick;

			if (velocity.Length2D() > 0)
				iChoke = std::min<int>(std::ceilf(TELEPORT_DISTANCE / distance_per_tick), g_Settings.iFakeLagChoke);
			else
				iChoke = g_Settings.iFakeLagChoke;

			if (NetChannel->m_nChokedPackets < iChoke)
			{
				Globals::bsendpacket = false;
			}
			else
			{
				Globals::bsendpacket = true;
			}
		}
		else if (g_Settings.iFakeLagType == 3)
		{
			bool yo = rand() % 2 ? true : false;

			if (NetChannel->m_nChokedPackets < iChoke && yo)
			{
				Globals::bsendpacket = false;
			}
			else
			{
				Globals::bsendpacket = true;
			}

		}
	}
}

void CFakeLag::fakeduck2(CUserCmd * cmd, C_BaseEntity* local)
{
	if (!g_Settings.bFreezeWalk)
	{
		Globals::fakeducked = false;
		return;
	}

	if (!GetAsyncKeyState(g_Settings.iFakeDuckKey) )
	{
		Globals::fakeducked = false;
		return;
	}
		
	if (!local->GetFlags() & FL_ONGROUND)
		return;

	auto NetChannel = g_pEngine->GetNetChannel();

	if (!NetChannel)
		return;

	int fakelag_limit = 14;
	int choked_goal = fakelag_limit / 2;
	bool should_crouch = NetChannel->m_nChokedPackets >= choked_goal;

	if (should_crouch)
	{
		cmd->buttons |= IN_DUCK;
		Globals::isfakeducking = true;
	}
	else
	{
		cmd->buttons &= ~IN_DUCK;
		Globals::isfakeducking = false;
	}

	Globals::fakeducked = true;
}

CFakeLag* fakelag = new CFakeLag();