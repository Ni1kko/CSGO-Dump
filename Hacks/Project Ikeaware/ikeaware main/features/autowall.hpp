#pragma once

#include "..//valve_sdk/sdk.hpp"
#include "..//valve_sdk/csgostructs.hpp"
#include "..//helpers/math.hpp"

template <typename T>
class singleton
{
public:
	static T* get()
	{
		static T* _inst = nullptr;

		if (!_inst)
			_inst = new T();

		return _inst;
	}
};

struct returninfo {
	int damage = -1;
	int hitgroup = -1;
	int walls = 4;
	bool did_penetrate_wall = false;
	float thickness = 1.f;

	IClientEntity* ent = nullptr;
	Vector end = Vector();
};

struct fbdata {
	Vector start = Vector();
	Vector end = Vector();
	Vector pos = Vector();
	Vector dir = Vector();

	ITraceFilter* filter = nullptr;
	trace_t trace;

	float thickness = 1.f;
	float damage = 1.f;
	int walls = 4;
};

class c_autowall : public singleton<c_autowall> {
public:
	float get_estimated_point_damage(Vector point);
	returninfo autowall(Vector start, Vector end, C_BasePlayer* from_ent = nullptr, C_BasePlayer* to_ent = nullptr, int hitgroup = -1);

private:
	inline void Math_AngleVectors(const Vector& angles, Vector& forward)
	{
		float sp, sy, cp, cy;
		Math::SinCos(DEG2RAD(angles[1]), &sy, &cy);
		Math::SinCos(DEG2RAD(angles[0]), &sp, &cp);
		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	inline Vector Math_CalcAngle(Vector src, Vector dst)
	{
		auto ret = Vector();
		auto delta = src - dst;
		double hyp = delta.Length2D();
		ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
		ret.x = (atan(delta.z / hyp) * 57.295779513082f);
		ret[2] = 0.00;

		if (delta.x >= 0.0)
			ret.y += 180.0f;

		return ret;
	}
	void clip_trace_to_player(Vector& start, Vector& end, C_BasePlayer* ent, unsigned int mask, ITraceFilter* filter, trace_t* trace);
	void scale_damage(C_BasePlayer* ent, CCSWeaponInfo* inf, int& hitgroup, float& damage);
	bool handle_bullet_penetration(CCSWeaponInfo* inf, fbdata& bullet);
	bool trace_to_exit(trace_t* enter_trace, Vector& start, Vector& dir, trace_t* exit_trace);
	bool is_breakable(C_BasePlayer* e);
};
