#include "Globals.h"

namespace Globals
{
	bool bsendpacket = false;
	bool can_shoot_someone = false;
	bool disable_fake_lag = false;
	bool shouldstop = false;
	float flHurtTime = 0.f;
	bool dontdoaa = false;
	bool preservedelete = false;
	bool lbybroken = false;
	bool fakeducked = false;
	int shots_hit;
	int shots_fired;
	bool hit[65];
	bool shot[65];
	bool fired = false;
	bool isfakeducking = false;
	Vector AngleAA = Vector(0, 0, 0);
	bool playable = false;
	bool breakinglc = false;
	float dormanttime = 0.f;
	int damage = -1;
	int missedshots = 0;
	int eventtimer = 0;
	C_BaseEntity* pPlayer;
}