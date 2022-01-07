#pragma once

#include "Interfaces.h"
#include "Hacks.h"
#include <deque>
#include "Backtrack.h"
extern int missedLogHits[65];
extern float saveLastHeadshotFloat[65];
extern float saveLastBaimFloat[65];
extern float saveLastBaim30Float[65];
extern float enemysLastProxyTimer[65];
extern bool shotFakeHeadOnce[65];
extern int fakeshotMissedLast[65];

namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern bool change;
	extern int TargetID;
	extern int getShotsFired;
}
extern int ResolverStage[65];

struct CTickRecord {
	CTickRecord() {}
	CTickRecord(IClientEntity* ent) {
		m_flLowerBodyYawTarget = ent->GetLowerBodyYaw();
		m_angEyeAngles = ent->GetEyeAngles();
		m_flSimulationTime = ent->GetSimulationTime();
		m_vecOrigin = ent->GetOrigin();
		m_vecVelocity = ent->GetVelocity();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		tickcount = 0;
	}

	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;

};

class CResolveInfo {

	friend class ayyResolver;


protected:
	std::deque<CTickRecord> m_sRecords;
	bool	m_bEnemyShot; //priority
	bool	m_bLowerBodyYawChanged;
	bool	m_bBacktrackThisTick;
};


class ayyResolver
{
	friend class CBacktracking;
public:

	void Init();
	void StoreVars(IClientEntity * pEntity);

	bool HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance);

	bool HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance);

	bool HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance, IClientEntity * pEnt);

	int GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance);

	int GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance);

	float GetLBYByComparingTicks(const std::deque<CTickRecord>& l, IClientEntity * pEnt);

	float GetDeltaByComparingTicks(const std::deque<CTickRecord>& l, IClientEntity * pEnt);

	bool DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance);

	bool LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance);

	void Store(IClientEntity * Entity);

	float ResolveYaw(IClientEntity* pPlayer, float org_yaw);

private:
	std::array<CResolveInfo, 32> m_arrInfos;
};


