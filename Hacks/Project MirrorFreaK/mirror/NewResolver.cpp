#include "NewResolver.h"
int Globals::Shots = 0;
int Globals::getShotsFired = 0;
bool Globals::change;

int Globals::TargetID;
IClientEntity* Globals::Target;

#include "Backtrack.h"
#include <algorithm>

#define TICK_INTERVAL           ( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )     ( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )      ( TICK_INTERVAL *( t ) )




void ayyResolver::Init() {


}

float tolerance = 10.f;

float Normalize(float delta) {

	while (delta <= -180) delta += 360;
	while (delta > 180) delta -= 360;

	return delta;
}

const inline float GetDelta(float a, float b) {
	return abs(Normalize(a - b));


}

const inline float LBYDelta(const CTickRecord& v) {
	return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
	return (GetDelta(a, b) > tolerance);
}

void ayyResolver::StoreVars(IClientEntity * pEntity) {

	if (m_arrInfos.at(pEntity->GetIndex()).m_sRecords.size() >= 10) {
		m_arrInfos.at(pEntity->GetIndex()).m_sRecords.pop_back();
	}

	m_arrInfos.at(pEntity->GetIndex()).m_sRecords.push_front(CTickRecord(pEntity));
}



bool ayyResolver::HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance) {
	auto minmax = std::minmax_element(std::begin(l), std::end(l), [](const CTickRecord& t1, const CTickRecord& t2) { return t1.m_flLowerBodyYawTarget < t2.m_flLowerBodyYawTarget; });
	return (fabs(minmax.first->m_flLowerBodyYawTarget - minmax.second->m_flLowerBodyYawTarget) <= tolerance);
}

bool ayyResolver::HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1;)
	{
		if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
			return false;
	}
	return true;
}

bool ayyResolver::HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance, IClientEntity* pEnt) {
	size_t misses = 0;
	for (size_t i = 0; i < l.size() - 1; i++) {
		float tickdif = static_cast<float>(l.at(i).m_flSimulationTime - l.at(i + 1).tickcount);
		float lbydif = GetDelta(l.at(i).m_flLowerBodyYawTarget, l.at(i + 1).m_flLowerBodyYawTarget);
		float ntickdif = static_cast<float>(Interfaces::Globals->tickcount - l.at(i).tickcount);
		if (((lbydif / tickdif) * ntickdif) > tolerance)
			misses++;
	}
	return (misses <= (l.size() / 3));
}

int ayyResolver::GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curdelta = LBYDelta(var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);
	}
	return vec.size();
}


int ayyResolver::GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l)
	{
		float curyaw = var.m_flLowerBodyYawTarget;
		bool add = true;
		for (auto fl : vec)
		{
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}

float ayyResolver::GetLBYByComparingTicks(const std::deque<CTickRecord>& l, IClientEntity* pEnt)
{
	int modulo = 1;
	int difangles = GetDifferentLBYs(l, tolerance);
	int inc = modulo * difangles;
	for (auto var : l)
	{
		for (int lasttick = var.tickcount; lasttick < Interfaces::Globals->tickcount; lasttick += inc)
		{
			if (lasttick == Interfaces::Globals->tickcount)
				return var.m_flLowerBodyYawTarget;
		}
	}
	return 0.f;
}

float ayyResolver::GetDeltaByComparingTicks(const std::deque<CTickRecord>& l, IClientEntity* pEnt) {
	int modulo = 1;
	int difangles = GetDifferentDeltas(l, tolerance);
	int inc = modulo * difangles;
	for (auto var : l)
	{
		for (int lasttick = var.tickcount; lasttick < Interfaces::Globals->tickcount; lasttick += inc) {
			if (lasttick == Interfaces::Globals->tickcount)
				return LBYDelta(var);
		}
	}
	return 0.f;
}
bool ayyResolver::DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentDeltas(cur, tolerance) >(int) cur.size() / 2);
}

bool ayyResolver::LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}

float random_between_two_floats(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}


float ayyResolver::ResolveYaw(IClientEntity* pPlayer, float org_yaw)
{
	auto index = pPlayer->GetIndex();

	static float LatestLowerBodyYaw[65];
	static float LatestLowerBodyYawUpdateTime[65];
	static float lbyproxytime;
	static float enemyLBYTimer[65];
	static float oldlowerbodyyawTest;


	float deltadif = abs(pPlayer->GetEyeAnglesXY()->y - pPlayer->GetLowerBodyYaw());
	lbyproxytime = enemysLastProxyTimer[pPlayer->GetIndex()] + 0.2f;

	auto new_yaw = org_yaw;
	int vel = pPlayer->GetVelocity().Length2D();
	float CurrentLowerbody = pPlayer->GetLowerBodyYaw();
	float flOriginalAngle = Normalize(pPlayer->GetEyeAngles().y);
	auto cur = this->m_arrInfos.at(index).m_sRecords;
	bool is_fakewalk = pPlayer->GetVelocity().Length2D() > 0 && pPlayer->GetVelocity().Length2D() < 50;
	bool is_moving = pPlayer->GetVelocity().Length2D() >= 50;
	bool is_standing = pPlayer->GetVelocity().Length2D() == 0;
	bool is_onground = pPlayer->GetFlags() & FL_ONGROUND;
	bool is_nospread = pPlayer->GetVelocity().Length2D() > 250;


	if (is_onground)
	{



		if (pPlayer->GetLowerBodyYaw() != LatestLowerBodyYaw[index] || (is_moving && !is_fakewalk && !is_standing))
		{
			LatestLowerBodyYaw[index] = pPlayer->GetLowerBodyYaw();
			ResolverStage[index] = 1;
			LatestLowerBodyYawUpdateTime[index] = Interfaces::Globals->curtime;

			return Normalize(pPlayer->GetLowerBodyYaw());

		}
		else
		{
			if (is_fakewalk)
			{

				if (DeltaKeepsChanging(cur, 10))
				{
					new_yaw = GetDeltaByComparingTicks(cur, pPlayer);
					//Delta Changes
					ResolverStage[index] = 2;
				}
				else if (LBYKeepsChanging(cur, 10))
				{
					new_yaw = GetLBYByComparingTicks(cur, pPlayer);
					//Lowerbody Changes
					ResolverStage[index] = 3;
				}
				else {
					new_yaw = pPlayer->GetEyeAnglesXY()->y + 180;
					ResolverStage[index] = 21;
				}


			}
			if (is_standing)
			{
				// Predict it only once a Round
				if (shotFakeHeadOnce[index] == false) {

					if (((Interfaces::Globals->curtime - LatestLowerBodyYawUpdateTime[index]) >= 1.1))
					{
						new_yaw = CurrentLowerbody + 180;
						ResolverStage[index] = 4;
						shotFakeHeadOnce[index] = true;
					}

				}
				else
				{
					if (std::abs(LatestLowerBodyYaw[index] - CurrentLowerbody) <= 10)
					{
						int random = rand() % 10;
						if (random < 6)
						{
							new_yaw = CurrentLowerbody + random_between_two_floats(-10, 10);
							ResolverStage[index] = 1;
						}
						else
						{
							new_yaw = CurrentLowerbody;
							ResolverStage[index] = 6;
						}
					}
					else
					{
						if (DeltaKeepsChanging(cur, 10))
						{
							new_yaw = GetDeltaByComparingTicks(cur, pPlayer);
							ResolverStage[index] = 2;
						}
						else if (LBYKeepsChanging(cur, 10))
						{
							new_yaw = GetLBYByComparingTicks(cur, pPlayer);
							ResolverStage[index] = 3;
						}
					}
				}

			}
		}


		if (missedLogHits[index] == 3 || missedLogHits[index] == 4) {
			// Try Database Log Hits
			ResolverStage[index] = 9;

			if (saveLastHeadshotFloat[index] != 0) {
				new_yaw = saveLastHeadshotFloat[index];

			}
			else if (saveLastBaimFloat[index] != 0) {
				new_yaw = saveLastBaimFloat[index];

			}
			else if (saveLastBaim30Float[index] != 0) {
				new_yaw = saveLastBaim30Float[index];

			}
			else {

				new_yaw = deltadif;

			}

		}
		else if (missedLogHits[index] == 5 || missedLogHits[index] == 6 || missedLogHits[index] == 7) {

			ResolverStage[index] = 11;

			if (missedLogHits[index] == 5) {
				new_yaw = deltadif - pPlayer->GetLowerBodyYaw();
			}

			if (missedLogHits[index] == 6 || missedLogHits[index] == 7) {
				if (lbyproxytime != enemyLBYTimer[pPlayer->GetIndex()] && abs(lbyproxytime - enemyLBYTimer[pPlayer->GetIndex()]) > 0.8f) {
					enemyLBYTimer[pPlayer->GetIndex()] = lbyproxytime;
					new_yaw = pPlayer->GetLowerBodyYaw();
					oldlowerbodyyawTest = pPlayer->GetLowerBodyYaw();

				}
				else {

					new_yaw = oldlowerbodyyawTest + deltadif;
				}
			}

		}
		else if (missedLogHits[index] == 8) {
			// Try Database Log Hits
			ResolverStage[index] = 9;

			if (saveLastHeadshotFloat[index] != 0) {

				new_yaw = saveLastHeadshotFloat[index];

			}
			else if (saveLastBaimFloat[index] != 0) {

				new_yaw = saveLastBaimFloat[index];

			}
			else if (saveLastBaim30Float[index] != 0) {

				new_yaw = saveLastBaim30Float[index];

			}
			else {
				new_yaw = abs(pPlayer->GetLowerBodyYaw() - org_yaw);
			}

		}



		if (fakeshotMissedLast[index] <= 3) {

			if (abs(pPlayer->GetLowerBodyYaw() - org_yaw) > 35) {
				// Enemy is using FakeAngle
				new_yaw = org_yaw - abs(pPlayer->GetLowerBodyYaw() - org_yaw);
				ResolverStage[index] = 13;
				fakeshotMissedLast[index] = missedLogHits[index];
			}
		}


	}
	else
	{
		if (is_nospread)
		{

			if (missedLogHits[index] <= 10) {

				switch (Globals::Shots % 4)
				{
				case 1:
					new_yaw = CurrentLowerbody - 15;
					break;
				case 2:
					new_yaw = CurrentLowerbody + 40;
					break;
				case 3:
					new_yaw = CurrentLowerbody + 15;
					break;
				case 4:
					new_yaw = CurrentLowerbody - 40;
					break;
				}

			}
			else {

				new_yaw = random_between_two_floats(-180, 180);

			}

		}
		else
		{
			new_yaw = GetLBYByComparingTicks(cur, pPlayer);
			ResolverStage[index] = 7;

		}

	}


	new_yaw = Normalize(new_yaw);
	return new_yaw;
}

