#pragma once

#include "main.h"

struct ResolverInfo
{
/*	ResolverInfo( const Vector& vShotPos, const  )
	{
		this->vShotPos = vShotPos;
	}*/

	Vector vShotPos = Vector( 0.f, 0.f, 0.f );
	Vector vHitPos = Vector( 0.f, 0.f, 0.f );

	int iShotsFired = 0;
	int iTick = 0;
	int iHitgroup = 0;

	float flTime = 0.f;

	int iShotsMissed = 0;
	float flUpdateYaw = 0.f;
	float flResolvedPitch = 0.f;
	bool bHeadHitted = false;
	bool bBodyHitted = false;
	bool bLBYUpdate = false;
	bool bAdjustPos = false;
	bool bAdjustLowPos = false;
	CAnimationLayer AnimLayer[ 15 ];
	float flOldCycle[ 15 ];
	float flAdjustStart;
	float flAdjustLowStart;
	std::string szStage = "";
};

extern std::map<int, ResolverInfo> g_ResolveInfo;

class PlayerResolveRecord
{
public:
	PlayerResolveRecord()
	{
		m_vecResolveAngles = Vector( 0.f, 0.f, 0.f );
		m_vecAngles = Vector( 0.f, 0.f, 0.f );
		m_vecOrigin = Vector( 0.f, 0.f, 0.f );

		m_iMissedAir = 0;
		m_iMissedMoving = 0;
		m_iMissedStand = 0;
		m_iMissedShots = 0;

		m_flMoveYaw = 0.f;
		m_flMoveDelta = 0.f;
		m_flPitchDown = 0.f;
		m_flDelta = 0.f;
		m_flSpeed = 0.f;

		m_bDormant = false;
		m_bValidMoveYaw = false;
		m_bFakeWalk = false;
		m_bFakeYaw = false;
	}

public:
	struct AntiFreestandingRecord
	{
		int right_damage = 0, left_damage = 0;
		float right_fraction = 0.f, left_fraction = 0.f;
	};

public:
	CAnimationLayer m_cLayer[ 13 ];
	AntiFreestandingRecord m_sAntiEdge;

	Vector m_vecResolveAngles, m_vecAngles;
	Vector m_vecOrigin;

	int m_iMissedAir;
	int m_iMissedMoving;
	int m_iMissedStand;
	int m_iMissedShots;

	float m_flMoveYaw;
	float m_flMoveDelta;
	float m_flPitchDown;
	float m_flDelta;
	float m_flSpeed;

	bool m_bDormant;
	bool m_bValidMoveYaw;
	bool m_bFakeWalk;
	bool m_bFakeYaw;
};

struct ShotSnapshot
{
	CBaseEntity* entity;
	PlayerResolveRecord resolve_record;

	Vector impact;
	Vector position;
	float time;
	float first_processed_time;
	bool was_shot_processed;
	int hitgroup_hit;
};

class CResolver
{
public:
	void Init( CBaseEntity* pLocal );

	void InitCM( CBaseEntity * pLocal, CBaseCombatWeapon * pWeapon );

	void InitFSN( CBaseEntity * pLocal, CBaseEntity * pEnemy );

	void AddShotSnapshot( CBaseEntity * entity, PlayerResolveRecord resolve_record );

	void CleanShotSnapshot();

	void ProcessSnapShots();

	void UpdateResolveRecord( CBaseEntity * pEnemy );

	void AntiFreestand( CBaseEntity * pEnemy, float & y, float flLeftDamage, float flRightDamage, float flRightFraction, float flLeftFraction, float flToMe, int & iShotsMissed );

/*	bool IsFakewalking( CBaseEntity* entity )
	{
		return player_resolve_records[ entity->GetIndex() ].is_fakewalking;
	}

	bool IsMovingOnGround( CBaseEntity* entity )
	{
		return player_resolve_records[ entity->GetIndex() ].animspeed > 0.5f && entity->GetFlags() & FL_ONGROUND;
	}

	PlayerResolveRecord& GetPlayerResolveInfo( CBaseEntity* entity )
	{
		return player_resolve_records[ entity->GetIndex() ];
	}*/

	PlayerResolveRecord player_resolve_records[ 33 ];
	std::vector<ShotSnapshot> shot_snapshots;
	std::vector<Vector> last_eye_positions;
};

extern CResolver g_Resolver;