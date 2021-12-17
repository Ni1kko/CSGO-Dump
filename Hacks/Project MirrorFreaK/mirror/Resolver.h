#include <array>
#include <string>
#include <deque>
#include <algorithm>
#include "Entities.h"
#include "CommonIncludes.h"
#include "Entities.h"
#include "Vector.h"
#include <map>
#include "Interfaces.h"
#include "Hacks.h"
#include "Hooks.h"

enum correction_flags
{
	DESYNC,
	NO_DESYNC,
	SLOW_WALK
};
struct extra
{
	int current_flag[64];
};
class angle_correction
{
public:
	void ac_main(IClientEntity* entity);
	void ac_simple(IClientEntity * entity);
	void ac_smart(IClientEntity * entity);
private:
	void missed_due_to_desync(IGameEvent* event);
	bool is_slow_walking(IClientEntity* entity);

};
class resolver_setup
{
public:
	bool didhit;

	float in_air_reso(IClientEntity * player, float lby, float speed);

	bool b1gjitter(IClientEntity * pEntity);

	bool adjusting_stop(IClientEntity * player, AnimationLayer * layer);

	bool high_delta(IClientEntity * player, AnimationLayer * layer);

	bool low_delta(IClientEntity * player, AnimationLayer * layer);

	bool is_fakewalk(IClientEntity * player, float speed, AnimationLayer * layer);
//	bool predict_lby(IClientEntity * player, float oldlby[64], float lby, float speed);

	void resolver_nospread(IClientEntity * pEntity, int CurrentTarget);
	
//	void resolver_main(IClientEntity * pEntity);
	void resolver_alt(IClientEntity * pEntity, int CurrentTarget);
	void resolver_simple(IClientEntity * pEntity, int CurrentTarget);
	void resolver_freestand(IClientEntity * pEntity, int CurrentTarget);
	void resolver_release(IClientEntity * pEntity, int CurrentTarget);
	void AnimationFix(IClientEntity * pEnt);
	void force_freestanding(IClientEntity * enemy);
	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve

	void preso(IClientEntity * pEntity);
	
	bool enemy_fakewalk;
	bool enemy_inair;
	bool enemy_fake;

	float resolved_pitch;
	float resolved_yaw;

	static resolver_setup GetInst()
	{
		static resolver_setup instance;
		return instance;
	}
}; extern resolver_setup * resolver;

namespace Globals
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern int fired;
	extern int hit;
	extern bool change;
	extern int TargetID;
	extern bool didhitHS;
	extern int missedshots;
	extern std::map<int, QAngle> storedshit;
	extern bool Up2date;
	extern Vector aim_point;
}