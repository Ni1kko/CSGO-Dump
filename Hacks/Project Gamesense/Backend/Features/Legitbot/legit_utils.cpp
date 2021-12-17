#include "legit_utils.h"

bool IsLineGoesThroughSmoke(Vector vStartPos, Vector vEndPos)
{
	static auto LineGoesThroughSmokeFn = (bool(*)(Vector vStartPos, Vector vEndPos))Cheat::Utilities->Memory_PatternScan("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	return LineGoesThroughSmokeFn(vStartPos, vEndPos);
}

bool hitchance(float hitchance, Vector aimangle, CBaseEntity* target)
{
	auto wep = G::LocalPlayer->GetActiveWeapon();
	auto wepidx = wep->GetItemDefenitionIndex();

	if (!wep)
		return false;

	Vector fw, rw, uw;
	Utilities->Math_AngleVectors(aimangle, fw, rw, uw);

	int hits = 0;
	int needed_hits = static_cast<int>(64.f * (hitchance / 100.f));

	wep->UpdateAccuracyPenalty();
	float cone = wep->GetSpreadCone();
	float best_inacc = (g_legitbot.quick_stop_inaccuracy == -1.0) ? wep->GetInaccuracy() : g_legitbot.quick_stop_inaccuracy;

	Vector src = G::LocalPlayer->GetEyePosition();
	for (int i = 0; i < 64; i++) {
		float a = Utilities->Game_RandomFloat(0.f, 1.f);
		float b = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
		float c = Utilities->Game_RandomFloat(0.f, 1.f);
		float d = Utilities->Game_RandomFloat(0.f, M_PI * 2.f);
		float inaccuracy = a * best_inacc;
		float spread = c * cone;

		if (wep->GetItemDefenitionIndex() == WEAPON_REVOLVER) {
			if (G::UserCmd->buttons & IN_ATTACK2) {
				a = 1.f - a * a;
				c = 1.f - c * c;
			}
		}

		Vector spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0);
		Vector direction;

		direction.x = fw.x + (spread_view.x * rw.x) + (spread_view.y * uw.x);
		direction.y = fw.y + (spread_view.x * rw.y) + (spread_view.y * uw.y);
		direction.z = fw.z + (spread_view.x * rw.z) + (spread_view.y * uw.z);
		direction.Normalized();

		Vector viewangles_spread;
		Vector view_forward;

		Utilities->Math_VectorAngles(direction, uw, viewangles_spread);
		Utilities->Math_Normalize(viewangles_spread);
		Utilities->Math_AngleVectors(viewangles_spread, view_forward);

		view_forward.NormalizeInPlace();
		view_forward = src + (view_forward * wep->get_csweapon_info()->range);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, view_forward);
		Interfaces::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);

		if (tr.m_pEnt == target)
			++hits;

		if (static_cast<int>((static_cast<float>(hits) / 64.f) * 100.f) >= hitchance)
			return true;

		if ((64 - i + hits) < needed_hits)
		{
			if (g_legitbot.quick_stop_inaccuracy != -1.0)
				g_legitbot.QuickStop();
			return false;
		}
	}
}

float RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

float GetFovToPlayer(Vector viewAngle, Vector aimAngle)
{
	Vector delta = aimAngle - viewAngle;
	Cheat::Utilities->Math_FixAngles(delta);
	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

float GetRealDistanceFOV(float distance, Vector angle, Vector viewangles)
{
	Vector aimingAt;
	Cheat::Utilities->Math_AngleVectors(viewangles, aimingAt);
	aimingAt *= distance;
	Vector aimAt;
	Cheat::Utilities->Math_AngleVectors(angle, aimAt);
	aimAt *= distance;
	return aimingAt.DistTo(aimAt) / 5;
}
