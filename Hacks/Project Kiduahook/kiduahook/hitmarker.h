#pragma once

#include "main.h"

struct BulletEvent_t
{
	float			flImpactTime;
	Vector			vecImpactPos;
	int			    m_Red, m_Green, m_Blue;
	CBaseEntity*	pPlayer;

	__forceinline BulletEvent_t()
	{
		vecImpactPos = { 0.0f, 0.0f, 0.0f };
		flImpactTime = 0.0f;
		m_Red = 255, m_Green = 255, m_Blue = 255;
		pPlayer = nullptr;
	}

	__forceinline BulletEvent_t( CBaseEntity* player, Vector pos, float time, int r, int g, int b )
	{
		pPlayer = player;
		flImpactTime = time;
		vecImpactPos = pos;
		m_Red = r; m_Green = g, m_Blue = b;
	}
};

extern std::deque<BulletEvent_t> g_BulletImpact;

struct sBulletImpact
{
	float flTime, flHitTime;
	bool bHit;
	Vector vPos, vEye, vEnemy;
};

extern std::vector<sBulletImpact> g_BulletHit;

class CHitmarker
{
public:

	bool ShouldHit();
	void InitTrace();
	void InitBulletHit( CBaseEntity * pLocal );
	void Init();

	bool bHit;
};

extern CHitmarker g_Hitmarker;