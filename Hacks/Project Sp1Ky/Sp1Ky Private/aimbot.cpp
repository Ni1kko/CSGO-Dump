#include "aimbot.h"
#include "helpers/math.hpp"
#include "helpers/utils.hpp"
#include "options.hpp"
#include <random>
#include "features/XorCompileTime.hpp"
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
//--------------------------------------------------------------------------------
LineGoesThroughSmokeFn LineGoesThroughSmoke;
std::vector<std::string> msgs =
{
	XorStr("U so owned by Sp1Ky"),
	XorStr("Bro u so very bad, refund your cheat"),
	XorStr("Are you sure with that your c+p?"),
	XorStr("It's a ez game for me"),
	XorStr("Can u start play dog?"),
	XorStr("I love if somebody is mad"),
	XorStr("Sp1Ky > all do u know?"),
	XorStr("U can't be better then me, just surrender hhh"),
	XorStr("Just come i one tap u"),
	XorStr("Who using aimware in 2018? Maybe u hurensohn"),
	XorStr("I don't like dog's, but i like cat on a plate")
};
void AimLegit::Work(CUserCmd *usercmd)
{
	this->usercmd = usercmd;
	if (!g_LocalPlayer->m_hActiveWeapon().Get())
		return;
	if (g_LocalPlayer->m_hActiveWeapon().Get()->IsWeaponNonAim() || g_LocalPlayer->m_hActiveWeapon().Get()->m_iClip1() < 1)
		return;
	if (g_Options.total_total_l_enabled || (usercmd->buttons & IN_ATTACK))
	{
		GetBestTarget();
		if (HasTarget())
			TargetRegion();
	}

	if (g_Options.total_total_l_r____cs)
		RecoilControlSystem();
}

void AimLegit::ChatSpamer()
{
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;
	long curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static long timestamp = curTime;
	if ((curTime - timestamp) < 850)
		return;
	if (g_Options.more_more_ara_spemrkk)
	{
		if (msgs.empty())
			return;
		std::srand(time(NULL));
		std::string msg = msgs[rand() % msgs.size()];
		std::string str;
		str.append(XorStr("say "));
		str.append(msg);
		g_EngineClient->ExecuteClientCmd(str.c_str());
	}
	timestamp = curTime;
}
void AimLegit::Clantag(void)
{
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	if (!g_Options.more_more_ani_mejtid_klentek)
		return;

	static size_t lastTime = 0;

	if (GetTickCount() > lastTime)
	{
		nazevclanu += nazevclanu.at(0);
		nazevclanu.erase(0, 1);

		Utils::SetClantag(nazevclanu.c_str());

		lastTime = GetTickCount() + 650;
	}
}
void AimLegit::GetBestTarget()
{
	float maxDistance = 8192.f;
	float nearest;
	int index = -1;
	float fov = g_Options.total_total_l_ejfouvykkk;
	int firedShots = g_LocalPlayer->m_iShotsFired();
	for (int i = 1; i < g_GlobalVars->maxClients; i++)
	{
		auto player = C_BasePlayer::GetPlayerByIndex(i);
		if (!CheckTarget(player))
			continue;
		QAngle viewangles = usercmd->viewangles;
		Vector targetpos;
		if (firedShots > g_Options.total_total_l_vymrdanejzasobnik)
			targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_tdolkaa_hit_dolni_bejk]);
		else if (firedShots < g_Options.total_total_l_vymrdanejzasobnik)
			targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_topka_hit_bejx]);
		nearest = Math::GetFov(viewangles, Math::CalcAngle(g_LocalPlayer->GetEyePos(), targetpos));
		if (nearest > fov)
			continue;
		float distance = Math::GetDistance(g_LocalPlayer->m_vecOrigin(), player->m_vecOrigin());
		if (fabsf(fov - nearest) < 5)
		{
			if (distance < maxDistance)
			{
				fov = nearest;
				maxDistance = distance;
				index = i;
			}
		}
		else if (nearest < fov)
		{
			fov = nearest;
			maxDistance = distance;
			index = i;
		}
	}
	SetTarget(index);
}
bool AimLegit::CheckTarget(C_BasePlayer *player)
{
	if (!player || player == nullptr)
		return false;
	if (player == g_LocalPlayer)
		return false;
	if (player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		return false;
	if (player->IsDormant())
		return false;
	if (player->m_bGunGameImmunity())
		return false;
	if (!player->IsAlive())
		return false;
	int firedShots = g_LocalPlayer->m_iShotsFired();
	Vector targetpos;
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;
	auto start = g_LocalPlayer->GetEyePos();
	if (firedShots > g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_tdolkaa_hit_dolni_bejk]);
	else if (firedShots < g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_topka_hit_bejx]);
	ray.Init(start, targetpos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
	if (tr.hit_entity != player)
		return false;
	return true;
}
void AimLegit::TargetRegion()
{
	auto player = C_BasePlayer::GetPlayerByIndex(m_iTarget);
	int firedShots = g_LocalPlayer->m_iShotsFired();
	QAngle aimPunchAngle = g_LocalPlayer->m_aimPunchAngle();
	std::random_device r3nd0m;
	std::mt19937 r3nd0mGen(r3nd0m());
	std::uniform_real<float> r3nd0mXAngle(1.7f, 1.9f);
	std::uniform_real<float> r3nd0mYAngle(1.7f, 1.9f);
	aimPunchAngle.pitch *= r3nd0mXAngle(r3nd0mGen);
	aimPunchAngle.yaw *= r3nd0mYAngle(r3nd0mGen);
	aimPunchAngle.roll = 0.0f;

	Math::NormalizeAngles(aimPunchAngle);

	QAngle viewangles = usercmd->viewangles;
	Vector targetpos;
	if (firedShots > g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_tdolkaa_hit_dolni_bejk]);
	else if (firedShots < g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_topka_hit_bejx]);
	QAngle angle = Math::CalcAngle(g_LocalPlayer->GetEyePos(), targetpos);
	angle.pitch -= aimPunchAngle.pitch;
	angle.yaw -= aimPunchAngle.yaw;
	Math::SmoothAngle(viewangles, angle, g_Options.total_total_l_zmensimtitozatebe_sm_fct);
	Math::NormalizeAngles(angle);
	usercmd->viewangles = angle;
}

QAngle oldPunch;

void AimLegit::RecoilControlSystem()
{
	int firedShots = g_LocalPlayer->m_iShotsFired();
	if (usercmd->buttons & IN_ATTACK)
	{
		QAngle aimPunchAngle = g_LocalPlayer->m_aimPunchAngle();
		std::random_device r3nd0m;
		std::mt19937 r3nd0mGen(r3nd0m());
		std::uniform_real<float> r3nd0mXAngle(1.7f, 1.9f);
		std::uniform_real<float> r3nd0mYAngle(1.7f, 1.9f);
		aimPunchAngle.pitch *= r3nd0mXAngle(r3nd0mGen);
		aimPunchAngle.yaw *= r3nd0mYAngle(r3nd0mGen);
		aimPunchAngle.roll = 0.0f;
		Math::NormalizeAngles(aimPunchAngle);
		if (firedShots > 2)
		{
			QAngle viewangles = usercmd->viewangles;
			QAngle viewangles_mod = aimPunchAngle;
			viewangles_mod -= oldPunch;
			viewangles_mod.roll = 0.0f;
			Math::NormalizeAngles(viewangles_mod);
			viewangles -= viewangles_mod;
			viewangles.roll = 0.0f;
			Math::NormalizeAngles(viewangles);
			usercmd->viewangles = viewangles;
		}
		oldPunch = aimPunchAngle;
	}
	else
		oldPunch.Init();
}























































































