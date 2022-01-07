#include "knifebot.h"

void angleVectorsyES(const Vector& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
void trace_liney(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}
/*
void knife_bot(CUserCmd* m_pcmd) {
	auto m_local = hackManager.pLocal();

	if (!m_local->IsAlive())
		return;

	if (m_local->GetFlags() & FL_FROZEN)
		return;
	
	if (!m_local->GetActiveWeaponHandle())
		return;
	if (!m_local->GetWeapon2())
		return;
	if (!m_local->GetWeapon2()->MESSAUFFYA())
		return;
	for (int y = 0; y <= 360; y += 360 / 6.f) {
		for (int x = -89; x <= 89; x += 179.f / 6.f) {
			Vector ang = Vector(x, y, 0);
			Vector dir;
			angleVectorsyES(ang, dir);
			trace_t trace;
			trace_liney(m_local->GetEyePosition(), m_local->GetEyePosition() + (dir * 66), MASK_SHOT, m_local, 0, &trace);
			if (!trace.m_pEnt)
				continue;
			if (trace.m_pEnt == nullptr)
				continue;
			if (trace.m_pEnt == m_local)
				continue;
			if (!trace.m_pEnt->IsAlive())
				continue;
			if (!(trace.m_pEnt->GetHealth() > 0))
				continue;
			if (trace.m_pEnt->GetTeamNum() == m_local->GetTeamNum())
				continue;
			if (trace.m_pEnt->IsDormant())
				continue;
			if (trace.m_pEnt->HasGunGameImmunity())
				continue;
			player_info_t info;
			if (!(Interfaces::Engine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info)))
				continue;
			m_pcmd->viewangles = Vector(x, y, 0);
			m_pcmd->buttons |= IN_ATTACK2; //TODO: do a proper attack list
			return;
		}
	}
}*/























































































































































































