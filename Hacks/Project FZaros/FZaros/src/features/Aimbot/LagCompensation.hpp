#pragma once

#include "../../utilities/includes.h"

#include <array>
#include <deque>

struct tickRecord
{
	Vector3 m_vecOrigin;
	bool bLowerBodyYawUpdated;

	float m_flSimulationTime;
	//float m_flAnimTime;
	int32_t m_nFlags;
	//int m_nSequence;
	//float m_flCycle;

	Vector3 m_angEyeAngles;
	Vector3 m_absangles;
	Vector3 m_vecMins;
	Vector3 m_vecMax;
	//	float m_flUpdateTime;
	Vector3 m_vecVelocity;

	float m_flPoseParameter[ 24 ];

	//float backtrackTime;
	Vector3 m_vecAbsOrigin;

	bool needs_extrapolation = false;

	matrix3x4_t boneMatrix[ 128 ];
};

struct playerRecord
{
	std::vector<tickRecord> records;
	Vector3 eyeAngles;
	float lowerBodyYawTarget;
	int flags;
	float unknown;
};

class CLagCompensation_new
{
public:
	playerRecord PlayerRecord[ 64 ];

	float getLerpTime( );

	float estimateServerTime( );
	float networkLatency( );
	void storeRecords( CBaseEntity* pPlayer );
	void clearRecords( );
	bool isTickValid( tickRecord record );
private:

}; extern CLagCompensation_new g_LagCompensation;
