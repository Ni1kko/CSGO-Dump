#include "GrenadePrediction.h"

void GrenadePrediction::Tick(int buttons)
{
	bool in_attack = buttons & IN_ATTACK, in_attack2 = buttons & IN_ATTACK2;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (local_player && local_player->GetHealth() > 0)
	{
		act = (in_attack && in_attack2) ? ACT_LOB :
			(in_attack2) ? ACT_DROP :
			(in_attack) ? ACT_THROW :
			ACT_NONE;
	}
}

bool IsGrenade(SDK::CBaseWeapon* weapon)
{
	auto WeaponIDz = weapon->GetItemDefenitionIndex();
	if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_HEGRENADE || WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_INCGRENADE ||
		WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_MOLOTOV || WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_FLASHBANG ||
		WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_DECOY || WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE)
		return true;
	return false;
}

void GrenadePrediction::View(SDK::CViewSetup* setup)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (local_player && local_player->GetHealth() > 0)
	{
		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon) return;
		if (IsGrenade(weapon) && act != ACT_NONE)
		{
			type = weapon->GetItemDefenitionIndex();
			Simulate(setup);
		}
		else
			type = 0;
	}
}

void GrenadePrediction::Paint()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() > 0)
	{
		if ((type) && path.size() > 1)
		{
			Vector nadeStart, nadeEnd;
			Vector prev = path[0];

			auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
			if (!weapon) return;
			auto WeaponIDz = weapon->GetItemDefenitionIndex();

			for (auto it = path.begin(), end = path.end(); it != end; ++it)
			{
				if (RENDER::WorldToScreen(prev, nadeStart) && RENDER::WorldToScreen(*it, nadeEnd))
					RENDER::DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y, SETTINGS::settings.grenadepredline_col);
				prev = *it;
			}
			for (auto it = others.begin(), end = others.end(); it != end; ++it)
				RENDER::DrawFilled3DBox(it->first, 5, 5, CColor(0, 135, 255, 200), CColor(0, 135, 255, 200));

			if (RENDER::WorldToScreen(prev, nadeEnd))
			{
				if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_FLASHBANG)
					RENDER::DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, FONTS::visuals_grenade_pred_font, true, true, CColor(255, 255, 255, 255), "");
				else if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_HEGRENADE)
					RENDER::DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, FONTS::visuals_grenade_pred_font, true, true, CColor(255, 255, 255, 255), "");
				else if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_INCGRENADE)
					RENDER::DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, FONTS::visuals_grenade_pred_font, true, true, CColor(255, 255, 255, 255), "");
				else if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_MOLOTOV)
					RENDER::DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, FONTS::visuals_grenade_pred_font, true, true, CColor(255, 255, 255, 255), "");
				else if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_DECOY)
					RENDER::DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, FONTS::visuals_grenade_pred_font, true, true, CColor(255, 255, 255, 255), "");
				else if (WeaponIDz == SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE)
					RENDER::DrawF((int)nadeEnd.x - 5, (int)nadeEnd.y, FONTS::visuals_grenade_pred_font, true, true, CColor(255, 255, 255, 255), "");
			}
		}
	}
}

void GrenadePrediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	Vector angThrow = viewangles;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
			pitch += 360.0f;
	}
	else
		pitch -= 360.0f;

	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	float flVel = 750.0f * 0.9f;
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	b = b * 0.7f; b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	MATH::AngleVectors2(angThrow, &vForward, &vRight, &vUp);

	vecSrc = local_player->GetEyePosition();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	SDK::trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	vecThrow = local_player->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void GrenadePrediction::Simulate(SDK::CViewSetup* setup)
{
	Vector vecSrc, vecThrow;
	Vector angles; INTERFACES::Engine->GetViewAngles(angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = INTERFACES::Globals->interval_per_tick;
	int logstep = (int)(0.05f / interval);
	int logtimer = 0;

	path.clear(); others.clear();
	for (unsigned int i = 0; i<path.max_size() - 1; ++i)
	{
		if (!logtimer) path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

#define PI 3.14159265358979323846f

void VectorAngles(const Vector& forward, QAngle& angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
		angles[1] = 0.0f;
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
}
int GrenadePrediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{
	Vector move; AddGravityMove(move, vecThrow, interval, false);
	SDK::trace_t tr; PushEntity(vecSrc, move, tr);

	int result = 0;
	if (CheckDetonate(vecThrow, tr, tick, interval))
		result |= 1;

	if (tr.flFraction != 1.0f)
	{
		result |= 2;
		ResolveFlyCollisionCustom(tr, vecThrow, interval);

		QAngle angles;
		VectorAngles((tr.end - tr.start).Normalized(), angles);
		others.push_back(std::make_pair(tr.end, angles));
	}

	vecSrc = tr.end;

	return result;
}


bool GrenadePrediction::CheckDetonate(const Vector& vecThrow, const SDK::trace_t& tr, int tick, float interval)
{
	switch (type)
	{
	case SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
	case SDK::ItemDefinitionIndex::WEAPON_DECOY:
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = (int)(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

	case SDK::ItemDefinitionIndex::WEAPON_MOLOTOV:
	case SDK::ItemDefinitionIndex::WEAPON_INCGRENADE:
		if (tr.flFraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;

	case SDK::ItemDefinitionIndex::WEAPON_FLASHBANG:
	case SDK::ItemDefinitionIndex::WEAPON_HEGRENADE:
		return (float)tick * interval > 1.5f && !(tick % (int)(0.2f / interval));
	default:
		assert(false);
		return false;
	}
}

void GrenadePrediction::TraceHull(Vector& src, Vector& end, SDK::trace_t& tr)
{
	SDK::Ray_t ray; SDK::CTraceWorldOnly filter;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));
	INTERFACES::Trace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void GrenadePrediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	Vector basevel(0.0f, 0.0f, 0.0f);
	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
		move.z = (vel.z + basevel.z) * frametime;
	else
	{
		float gravity = 800.0f * 0.4f;
		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;
		vel.z = newZ;
	}
}

void GrenadePrediction::PushEntity(Vector& src, const Vector& move, SDK::trace_t& tr)
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;
	TraceHull(src, vecAbsEnd, tr);
}

void GrenadePrediction::ResolveFlyCollisionCustom(SDK::trace_t& tr, Vector& vecVelocity, float interval)
{
	float flSurfaceElasticity = 1.0, flGrenadeElasticity = 0.45f;
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f;

	if (flSpeedSqr<flMinSpeedSqr)
	{
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.flFraction) * interval);
		PushEntity(tr.end, vecAbsVelocity, tr);
	}
	else
		vecVelocity = vecAbsVelocity;
}

int GrenadePrediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float backoff, change, angle;
	int   i, blocked;

	blocked = 0;
	angle = normal[2];

	if (angle > 0) blocked |= 1;
	if (!angle) blocked |= 2;

	backoff = in.Dot(normal) * overbounce;
	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0;
	}
	return blocked;
}
