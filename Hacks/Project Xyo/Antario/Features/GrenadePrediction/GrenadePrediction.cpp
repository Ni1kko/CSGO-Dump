#include "GrenadePrediction.h"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/ISurface.h"
#include "../../Utils/Math.h"
#include "../../SDK/IEngineTrace.h"
#include "../../SDK/IClientMode.h"
#include "../../SDK/CGlobalVarsBase.h"

bool ExplectedWorldToScrean(Vector vOrigin, Vector &vScreen)
{
	float w = Globals::w2s_matrix[3][0] * vOrigin.x + Globals::w2s_matrix[3][1] * vOrigin.y + Globals::w2s_matrix[3][2] * vOrigin.z + Globals::w2s_matrix[3][3];
	int x, y;
	g_pEngine->GetScreenSize(x, y);
	float ScreenWidth = (float)x;
	float ScreenHeight = (float)y;

	if (w > 0.01f)
	{
		float inverseWidth = 1.f / w;
		vScreen.x = (float)((ScreenWidth / 2) + (0.5 * ((Globals::w2s_matrix[0][0] * vOrigin.x + Globals::w2s_matrix[0][1] * vOrigin.y + Globals::w2s_matrix[0][2] * vOrigin.z + Globals::w2s_matrix[0][3]) * inverseWidth) * ScreenWidth + 0.5));
		vScreen.y = (float)((ScreenHeight / 2) - (0.5 * ((Globals::w2s_matrix[1][0] * vOrigin.x + Globals::w2s_matrix[1][1] * vOrigin.y + Globals::w2s_matrix[1][2] * vOrigin.z + Globals::w2s_matrix[1][3]) * inverseWidth) * ScreenHeight + 0.5));
		return true;
	}

	return false;
}

void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, Color Color, std::string Input)
{
	/* char -> wchar */
	size_t size = Input.size() + 1;
	auto wide_buffer = std::make_unique<wchar_t[]>(size);
	mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

	/* check center */
	int width = 0, height = 0;
	g_pSurface->GetTextSize(Font, wide_buffer.get(), width, height);
	if (!center_width)
		width = 0;
	if (!center_height)
		height = 0;

	/* call and draw*/
	g_pSurface->DrawSetTextColor(Color.red, Color.green, Color.blue, Color.alpha);
	g_pSurface->DrawSetTextFont(Font);
	g_pSurface->DrawSetTextPos(X - (width * .5), Y - (height * .5));
	g_pSurface->DrawPrintText(wide_buffer.get(), wcslen(wide_buffer.get()));
}


void GrenadePrediction::Tick(int buttons)
{
	bool in_attack = buttons & IN_ATTACK, in_attack2 = buttons & IN_ATTACK2;

	if (Globals::LocalPlayer && Globals::LocalPlayer->GetHealth() > 0)
	{
		act = (in_attack && in_attack2) ? ACT_LOB :
			(in_attack2) ? ACT_DROP :
			(in_attack) ? ACT_THROW :
			ACT_NONE;
	}
}
bool IsGrenade(C_BaseCombatWeapon* weapon)
{
	auto WeaponIDz = *weapon->fixskins();
	if (WeaponIDz == 44 || WeaponIDz == 48 ||
		WeaponIDz == 46 || WeaponIDz == 43 ||
		WeaponIDz == 47 || WeaponIDz == 45)
		return true;
	else
		return false;
}

void GrenadePrediction::View(CViewSetup* setup)
{

	if (Globals::LocalPlayer && Globals::LocalPlayer->GetHealth() > 0)
	{
		auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(Globals::LocalPlayer->GetActiveWeaponIndex()));
		if (!weapon) return;
		if (IsGrenade(weapon))
		{
			type = *weapon->fixskins();
			Simulate(setup);
		}
		else
			type = 0;
	}
}
void DrawLine(int x1, int y1, int x2, int y2, Color color)
{
	g_pSurface->DrawSetColor(color);
	g_pSurface->Line(x1, y1, x2, y2, color);
}
void DrawFilled3DBox(Vector origin, int width, int height, Color outline, Color filling)
{
	float difw = float(width / 2);
	float difh = float(height / 2);
	Vector boxVectors[8] =
	{
		Vector(origin.x - difw, origin.y - difh, origin.z - difw),
		Vector(origin.x - difw, origin.y - difh, origin.z + difw),
		Vector(origin.x + difw, origin.y - difh, origin.z + difw),
		Vector(origin.x + difw, origin.y - difh, origin.z - difw),
		Vector(origin.x - difw, origin.y + difh, origin.z - difw),
		Vector(origin.x - difw, origin.y + difh, origin.z + difw),
		Vector(origin.x + difw, origin.y + difh, origin.z + difw),
		Vector(origin.x + difw, origin.y + difh, origin.z - difw),
	};

	static Vector vec0, vec1, vec2, vec3,
		vec4, vec5, vec6, vec7;

	if (ExplectedWorldToScrean(boxVectors[0], vec0) &&
		ExplectedWorldToScrean(boxVectors[1], vec1) &&
		ExplectedWorldToScrean(boxVectors[2], vec2) &&
		ExplectedWorldToScrean(boxVectors[3], vec3) &&
		ExplectedWorldToScrean(boxVectors[4], vec4) &&
		ExplectedWorldToScrean(boxVectors[5], vec5) &&
		ExplectedWorldToScrean(boxVectors[6], vec6) &&
		ExplectedWorldToScrean(boxVectors[7], vec7))
	{
		Vector lines[12][2];
		lines[0][0] = vec0;
		lines[0][1] = vec1;
		lines[1][0] = vec1;
		lines[1][1] = vec2;
		lines[2][0] = vec2;
		lines[2][1] = vec3;
		lines[3][0] = vec3;
		lines[3][1] = vec0;

		// top of box
		lines[4][0] = vec4;
		lines[4][1] = vec5;
		lines[5][0] = vec5;
		lines[5][1] = vec6;
		lines[6][0] = vec6;
		lines[6][1] = vec7;
		lines[7][0] = vec7;
		lines[7][1] = vec4;

		lines[8][0] = vec0;
		lines[8][1] = vec4;

		lines[9][0] = vec1;
		lines[9][1] = vec5;

		lines[10][0] = vec2;
		lines[10][1] = vec6;

		lines[11][0] = vec3;
		lines[11][1] = vec7;

		for (int i = 0; i < 12; i++)
			DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, outline);
	}
}
void GrenadePrediction::Paint()
{
	if (!Globals::LocalPlayer) return;
	if (Globals::LocalPlayer->GetHealth() > 0)
	{
		if ((type) && path.size() > 1)
		{
			Vector nadeStart, nadeEnd;
			Vector nadeStart2, nadeEnd2;
			Vector prev = path[0];
			Vector prevoth = others[0].first;
			auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(Globals::LocalPlayer->GetActiveWeaponIndex()));
			if (!weapon) return;
			bool in_attack = Globals::pCmd->buttons & IN_ATTACK, in_attack2 = Globals::pCmd->buttons & IN_ATTACK2;
			if (!(in_attack || in_attack2)) return;

			auto WeaponIDz = *weapon->fixskins();

			for (auto it = path.begin(), end = path.end(); it != end; ++it)
			{
				if (ExplectedWorldToScrean(prev, nadeStart) && ExplectedWorldToScrean(*it, nadeEnd))
					DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y, Color(123, 153, 172));
				prev = *it;
			}
			for (auto it = others.begin(), end = others.end(); it != end; ++it) {

				if (ExplectedWorldToScrean(prevoth, nadeStart2) && ExplectedWorldToScrean(it->first, nadeEnd2)) {
					g_pSurface->FilledRect((int)nadeStart2.x - 2, (int)nadeStart2.y - 2, 4, 4, Color(0, 255, 0, 255));
				}
				

				prevoth = it->first;
			}
			

			//RENDER::DrawFilled3DBox(others.rbegin()->first, 4, 4, CColor(255, 0, 0, 200), CColor(0, 135, 255, 200));
		}
	}
}

void GrenadePrediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{

	if (!Globals::LocalPlayer) return;
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
	g_Math.AngleVectors(angThrow, &vForward, &vRight, &vUp);

	vecSrc = Globals::LocalPlayer->GetEyePosition();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	C_Trace tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;

	TraceHull(vecSrc, vecDest, tr);

	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.end;
	vecSrc -= vecBack;

	vecThrow = Globals::LocalPlayer->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void GrenadePrediction::Simulate(CViewSetup* setup)
{
	Vector vecSrc, vecThrow;
	Vector angles; g_pEngine->GetViewAngles(angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = g_pGlobalVars->intervalPerTick;
	int logstep = (int)(0.05f / interval);
	int logtimer = 0;

	path.clear(); others.clear();
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
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
	C_Trace tr; PushEntity(vecSrc, move, tr);

	int result = 0;

	auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(Globals::LocalPlayer->GetActiveWeaponIndex()));
	if (!weapon) return 0;
	if (!IsGrenade(weapon)) return 0;


	if (CheckDetonate(vecThrow, tr, tick, interval))
		result |= 1;

	if (tr.flFraction != 1.0f)
	{
		result |= 2;
		ResolveFlyCollisionCustom(tr, vecThrow, interval);

		QAngle angles;
		VectorAngles((tr.end - tr.start).Normalize(), angles);
		others.push_back(std::make_pair(tr.end, angles));
	}

	vecSrc = tr.end;

	return result;
}


bool GrenadePrediction::CheckDetonate(const Vector& vecThrow, const C_Trace& tr, int tick, float interval)
{
	switch (type)
	{
	case 45:
	case 47:
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = (int)(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

	case 46:
	case 48:
		if (tr.flFraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;

	case 43:
	case 44:
		return (float)tick * interval > 1.5f && !(tick % (int)(0.2f / interval));
	default:
		assert(false);
		return false;
	}
}

void GrenadePrediction::TraceHull(Vector& src, Vector& end, C_Trace& tr)
{
	CTraceFilterWorldOnly filter;
	g_pTrace->TraceRay(C_Ray(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f)), 0x200400B, &filter, &tr);
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

void GrenadePrediction::PushEntity(Vector& src, const Vector& move, C_Trace& tr)
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;
	TraceHull(src, vecAbsEnd, tr);
}

void GrenadePrediction::ResolveFlyCollisionCustom(C_Trace& tr, Vector& vecVelocity, float interval)
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

	if (flSpeedSqr < flMinSpeedSqr)
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