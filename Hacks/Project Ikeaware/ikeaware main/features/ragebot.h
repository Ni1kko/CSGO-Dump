#pragma once
#include "..//helpers/math.hpp"
#include "..//helpers/input.hpp"
#include "..//helpers/memory.hpp"
#include "..//options.hpp"
#include "..//valve_sdk/csgostructs.hpp"
#define Assert(x)
#define CHECK_VALID( _v ) 0
class CRagebot
{
public:
	void Run(CUserCmd* pCmd, bool& bSendPacket);
	float hitchance(QAngle angles, C_BasePlayer* ent, float chance);
	inline void VectorSubtractForFOV(const Vector& a, const Vector& b, Vector& c)
	{
		CHECK_VALID(a);
		CHECK_VALID(b);
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}
	void Normalize(Vector& vIn, Vector& vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}
	float FovToPlayer(Vector AimPos);
	int GetTargetFOV();
	int GetTargetDistance();
	int GetTargetHealth();
	bool IsViable(C_BasePlayer* entity);
	Vector HitScan(C_BasePlayer* entity);
	void DoAimbot(CUserCmd* pCmd, bool& bSendPacket);
	bool CanFirePostpone() {

		float rdyTime = g_LocalPlayer->m_hActiveWeapon()->m_flPostponeFireReadyTime();

		if (rdyTime > 0 && rdyTime < g_GlobalVars->curtime)
			return true;

		return false;
	}
	void AntiAim(CUserCmd* cmd, bool& bSendPacket);
	float next_lby_update = 0.f;
	bool break_lby = false;
	void predict(CUserCmd* pCmd);
	void Slowwalk(CUserCmd* pCmd);
	void AnimationFix(C_BasePlayer* player);
	bool IsLocked;
	int TargetID;
	C_BasePlayer* pTarget;
	int HitBox;
	bool aimbotted_in_current_tick;
};

inline CRagebot* g_Ragebot;