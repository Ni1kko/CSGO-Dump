#pragma once

#include "animation.h"
#include "main.h"

#define TICK_INTERVAL            (g_pGlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )        ( TICK_INTERVAL *( t ) )

class VarMapEntry_t
{
public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void				*data;
	void            	*watcher;
};

struct VarMapping_t
{
	CUtlVector<VarMapEntry_t> m_Entries;
	int m_nInterpolatedEntries; // +0x14
	float m_lastInterpolationTime; // +0x18
};

struct sPreBacktrackRecord
{
	explicit sPreBacktrackRecord(CBaseEntity* pEnemy);

	CBaseEntity* m_pEnt;
	Vector m_vecOrigin;
	Vector m_vecAbsOrigin;
	QAngle m_angAbsAngles;
	Vector m_angEyeAngles;
	Vector m_vecVelocity;
	Vector m_vecBBMin;
	Vector m_vecBBMax;

	float m_flSimulationTime;
	float m_flLowerBodyYawTarget;
	int m_fFlags;

	matrix3x4_t m_BoneCacheMatrices[128];
	matrix3x4_t* m_BoneCachePtr;
	matrix3x4_t m_BoneCachePtrMatrices[128];
	float m_flPoseParameters[24];
	CAnimState m_pAnimState;
	CAnimationLayer m_AnimLayer[13];

	unsigned int m_flLastBoneSetupTime;
	unsigned int m_iMostRecentModelBoneCounter;

	void Restore();
};

struct sHistoryRecords
{
	sHistoryRecords() {};
	explicit sHistoryRecords( CBaseEntity* pEnemy );

	Vector m_vecOrigin;
	Vector m_vecAbsOrigin;
	QAngle m_angAbsAngles;
	Vector m_angEyeAngles;
	Vector m_vecVelocity;
	Vector m_vecBBMin;
	Vector m_vecBBMax;
	Vector m_vecPrevAbsOrigin;
	Vector m_vecPosition;
	Vector m_vecResolved;

	float m_flPoseParameters[ 24 ];
	float m_flSimulationTime;
	float m_flPrevSimulationTime;
	float m_flLowerBodyYawTarget;
	float m_flPrevLowerBodyYawTarget;
	float m_flCurTime;
	float m_flPrevCurTime;
	float m_flAnimTime;
	float m_flMaxAbsYaw;

	CAnimState m_pAnimState;
	CAnimationLayer m_AnimLayer[ 13 ];

	int m_fFlags;
	int m_iLag;

	alignas(16) matrix3x4_t m_Matrix[ 128 ];

	bool m_bIsSetup;
	bool m_bIsChamsCompatible;
	bool m_bResolved;
};

struct sSequence 
{
	sSequence::sSequence( int instate, int outstate, int seqnr, float time ) 
	{
		m_iInState = instate;
		m_iOutState = outstate;
		m_iSequenceNr = seqnr;
		m_flTime = time;
	}

	int m_iInState;
	int m_iOutState;
	int m_iSequenceNr;
	float m_flTime;
};

class CHistory
{
public:
	CHistory();

	// Pasta fiesta owo!
	bool BuildBones(CBaseEntity* pEntity, int iMask, matrix3x4_t* pMatrix, std::shared_ptr<sHistoryRecords> pHistoryRecord);
	bool GetBoneMatrix(CBaseEntity* pEntity, std::shared_ptr<sHistoryRecords> pHistoryRecord, matrix3x4_t* pMatrix);

	// Accuracy fixes
	void SetInterpolationFlags(CBaseEntity* pEnemy, int iFlag);
	void DisableInterpolation(CBaseEntity* pLocal);
	
	// Storing / restoring
	void Store( CBaseEntity * pLocal, int iStage );
	void Restore( CBaseEntity * pEnemy, std::shared_ptr<sHistoryRecords> sRecord );
	void RestoreToCurrentRecord( CBaseEntity* pEnemy );
	void ClearRecords();

	bool IsPriorityRecord(CBaseEntity * pEnemy);

	// Sorting helpers
	std::shared_ptr<sHistoryRecords> GetCurrentRecord(CBaseEntity* pEnemy);
	std::shared_ptr<sHistoryRecords> GetLastRecord(CBaseEntity* pEnemy);
	std::deque<std::shared_ptr<sHistoryRecords>> GetRecordsToScan(CBaseEntity* pEnemy, int iPosAdjustMode);

	// Visuals helpers
	std::deque<std::shared_ptr<sHistoryRecords>>& GetRecordsToDraw(CBaseEntity* pEnemy);

	// Fake latency stuff
	void UpdateSequence();
	void ClearSequence();
	void FixAnimation(CBaseEntity * pLocal, CBaseEntity * pEntity);
//	void AddLatency( CNetChannel * net_channel );

	// Utils
	float GetDeltaTime(std::shared_ptr<sHistoryRecords> sRecord );
	float GetLerpTime();
	int GetLerpTicks();
	int GetTickCount(std::shared_ptr<sHistoryRecords> sRecord );
	std::shared_ptr<sHistoryRecords> GetBestRecordAndPoint(CBaseEntity * pLocal, CBaseEntity * pEnemy, CBaseCombatWeapon * pWeapon, CUserCmd * pCmd, int iWpnID, Vector & vFinal);

	std::deque< sSequence > m_dSequences;
	int m_iLastSequence;

	std::deque<std::shared_ptr<sHistoryRecords>> vPlayerRecord[33];

	std::shared_ptr<sHistoryRecords> vUsedRecord;

	bool bNeedRestore[ 33 ];
};

extern CHistory g_History;