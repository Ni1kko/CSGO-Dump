#pragma once

#include "Hacks.h"
#include <deque>

extern int cmd_tick_count;
namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;

	extern int Shots;
	extern bool change;
	extern int TargetID;
	extern int getShotsFired;
}




struct LagRecord_t {
	float m_flSimulationTime;
	bool m_bValidRecord;
	QAngle m_vecAngles;
	Vector m_vecEyeAngles;
	Vector m_vecOrigin;
	Vector m_vecVelocity;
	Vector m_vecMins;
	Vector m_vecMaxs;
	float m_flLowerBodyYawTarget;
	Vector headSpot;
	std::array<float, 24> m_flPoseParameter;
	int m_flags;
	float m_flCycle;
	int m_nSequence;

	void Store(IClientEntity* pEnt) {
		m_vecOrigin = pEnt->GetAbsOrigin();
		m_vecAngles = pEnt->GetAbsAngles();
		m_vecEyeAngles = *pEnt->GetEyeAnglesXY();
		m_vecVelocity = pEnt->GetVelocity();
		//m_vecMins = pEnt->GetCollideable();
		//m_vecMaxs = pEnt->GetCollideable()->OBBMaxs();
		m_flSimulationTime = pEnt->GetSimulationTime();
		m_flLowerBodyYawTarget = pEnt->GetLowerBodyYaw();
		headSpot = pEnt->GetBonePos(8);
		m_flPoseParameter = pEnt->GetPoseParameters();
		m_flags = pEnt->GetFlags();
		m_flCycle = pEnt->getCycle();
		m_nSequence = pEnt->getSequence();

	};

	void Store(LagRecord_t record) {
		m_vecOrigin = record.m_vecOrigin;
		m_vecAngles = record.m_vecAngles;
		m_vecEyeAngles = record.m_vecEyeAngles;
		m_vecVelocity = record.m_vecVelocity;
		//m_vecMins = record.m_vecMins;
		//m_vecMaxs = record.m_vecMaxs;
		m_flSimulationTime = record.m_flSimulationTime;
		m_flLowerBodyYawTarget = record.m_flLowerBodyYawTarget;
		headSpot = record.headSpot;
		m_flPoseParameter = record.m_flPoseParameter;
		m_flags = record.m_flags;
		m_flCycle = record.m_flCycle;
		m_nSequence = record.m_nSequence;
		m_bValidRecord = true;
	};

	void Init() {
		m_vecAngles = Vector(0, 0, 0);
		m_flags = 0;
		m_nSequence = 0;
		m_flCycle = 0;
		m_vecOrigin = Vector(0, 0, 0);
		m_vecVelocity = Vector(0, 0, 0);
		m_vecEyeAngles = QAngle(0, 0, 0);
		headSpot = Vector(0, 0, 0);
		//m_vecMins = Vector(0, 0, 0);
		//m_vecMaxs = Vector(0, 0, 0);
		m_flLowerBodyYawTarget = 0.0f;
		m_flSimulationTime = 0.0f;
		std::array<float, 24> m_flPoseParameter = {};
	}
};


class CBackTrack {
public:
	void Inititalise();
	void Run(CUserCmd* pCmd, IClientEntity* pEnt);
	void RestoreEntity(IClientEntity* pEnt);
	void StoreEntity(IClientEntity* local, IClientEntity* pEntity);
	bool FindOldestRecord(IClientEntity * pEntity);
	bool IsValidTick(IClientEntity* pEnt, float simTime);

	float m_flTeleportDistanceSqr;
	int m_iTicks;

	struct BackTrackData_s {
		std::deque<LagRecord_t> m_PlayerRecords;

		void Init() {
			m_PlayerRecords.erase(m_PlayerRecords.begin(), m_PlayerRecords.end());
		};
	};

	BackTrackData_s m_BackTrackData[65];
	LagRecord_t m_PlayerTable;
	LagRecord_t m_RestoreData[65];

	ConVar*	pInterpVar;
	ConVar*	pRatioVar;
	ConVar*	pUpdateVar;
	ConVar*	pUnlagVar;
	ConVar*	pInterpolateVar;

	template< class T, class Y >
	T clamp(T const &val, Y const &minVal, Y const &maxVal) {
		if (val < minVal)
			return minVal;
		else if (val > maxVal)
			return maxVal;
		else
			return val;
	}
private:
	bool m_bInitialized = false;
};

extern std::unique_ptr<CBackTrack> pBackTrack;


enum PRIORITY : int
{
	PRIORITY_NONE = 0,
	PRIORITY_LOW = 1,
	PRIORITY_MEDIUM,
	PRIORITY_HIGH
};

struct LagRecord_tT
{
	int m_iTargetTickcount;
	int m_iPrevBullets;
	int m_iPrevFlags;
	int RecordPriorityFlag;
	bool m_bHasBetterRecord;
	bool m_bIsValidRecord;
	bool IsHeadVisible;
	std::array<float, 24> m_flAPoseParameters = {};
	float m_flSimulationTime;
	float m_flPreviousSimulationTime;
	float m_flBaseTime;
	Vector m_vecOrigin;
	Vector m_vecAngles;
	Vector m_vecPreviousOrigin;
	Vector m_vecPreviousAngles;

public:
	void InvalidateRecord() {
		m_bIsValidRecord = false;
	};

	void SaveInfo(IClientEntity* pEnt) {
		m_vecOrigin = pEnt->GetOrigin();
		m_vecAngles = *pEnt->GetEyeAnglesXY();
		m_flSimulationTime = pEnt->GetSimulationTime();
		m_flAPoseParameters = pEnt->GetPoseParameters();
		m_bIsValidRecord = false;
	};

};

class CTimeSimulator
{
public:

	LagRecord_tT m_PlayerTable[65];
	float OldLowerBodyYaw[65]; //not sure about array or other container
	float OldPoses[65]; //but as it doesn't complain about anything fuck it
	LagRecord_tT m_PrevPlayerTable[65];
	LagRecord_tT m_BacktrackedPlayerTable[65];
	std::deque<LagRecord_tT> m_PlayerGoodRecords[65];

	void UpdatePlayerTable(IClientEntity* pEntity);
	bool IsRecordValid(IClientEntity* pEntity, LagRecord_tT& Record);
	float lerpTime();
	bool IsTickValid(int Tick);
	bool LBYShouldUpdate(IClientEntity* pEntity);
	bool ProcessCmd(int iTargetIndex, CUserCmd* pCmd);
	void Cache();
	bool FindBestRecord(IClientEntity* pEntity);
	bool FindOldestRecord(IClientEntity* pEntity);
};
extern std::unique_ptr<CTimeSimulator> newtestBacktrack;