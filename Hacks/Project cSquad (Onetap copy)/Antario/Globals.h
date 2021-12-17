#pragma once
#include "SDK\Vector.h"
class C_BaseEntity;
namespace Globals
{
	extern bool bsendpacket;
	extern bool can_shoot_someone;
	extern bool disable_fake_lag;
	extern bool shouldstop;
	extern float flHurtTime;
	extern bool dontdoaa;
	extern bool preservedelete;
	extern bool lbybroken;
	extern bool fakeducked;
	extern int shots_hit;
	extern int shots_fired;
	extern bool hit[65];
	extern bool shot[65];
	extern Vector AngleAA;
	extern bool playable;
	extern bool fired;
	extern bool isfakeducking;
	extern bool breakinglc;
	extern float dormanttime;
	extern int damage;
	extern int missedshots;
	extern int eventtimer;
	extern C_BaseEntity* pPlayer;
}
