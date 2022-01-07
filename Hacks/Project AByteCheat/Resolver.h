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

struct resolver_infos
{
	AnimationLayer animation;

	bool jitter_desync = false, high_delta = false, low_delta = false, static_desync = false, lby_changed = false;

	float last_lby = FLT_MAX, lby_delta = FLT_MAX, last_eyes = FLT_MAX, eye_delta = FLT_MAX, eye_lby_delta = FLT_MAX, eyes = FLT_MAX;

};

struct resolver_record
{
	std::deque< resolver_infos > tick_records;

	resolver_infos current_tick;
	resolver_infos previous_tick;

};

class angle_correction
{
public:

	void ac_smart(IClientEntity * entity);
	bool solve_desync_simple(IClientEntity * e);
	bool breaking_lby_animations(IClientEntity * e);
	void resolve_tramadol(IClientEntity * player);
	void resolve_desync_smart(IClientEntity * player);



	bool laurie_aa[65] = { false };
	bool breaking[65] = { false };

	int resolver_stage;
	int resolver_flag[65];

private:
	resolver_record _player[64];
	void missed_due_to_desync(IGameEvent* event);
	bool is_slow_walking(IClientEntity* entity);


}; //extern angle_correction * a_c;


class resolver_setup
{
public:
	bool didhit;

	//	void resolver_main(IClientEntity * pEntity);

	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve

	void preso(IClientEntity * pEntity);

	bool enemy_slowwalk = false;
	bool enemy_inair;
	bool enemy_fake[65] = { false };
	bool enemy_shot[65] = { false };
	bool has_desync = false;

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