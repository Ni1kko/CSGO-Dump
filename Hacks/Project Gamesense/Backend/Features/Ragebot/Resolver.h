#pragma once
#include "../../singleton.h"
#include "../../Configuration/Config.h"
#include "../../SDK/CBaseEntity.h"
#include "..\\..\\Features\Ragebot\CAutoWall.h"
#include "../../SDK/INetChannelInfo.h"
#include <deque>
struct ResolverData
{
	float m_flFakeGoalFeetYaw;
};

extern ResolverData g_ResolverData[65];
class c_resolver : public singleton<c_resolver>
{
public:
	void run();
	float pitchHit[65];
	void Resolver(CBaseEntity* player);
	void init_player_ids();
	void AnimationFix(CBaseEntity* pEnt);
	bool ajusting_balance(CBaseEntity* e);
	int32_t m_iLayerCount = 0;
	CAnimationLayer animationLayer[15];
private:
	bool did_store_yaw[65];
	float stored_yaws[65];

};