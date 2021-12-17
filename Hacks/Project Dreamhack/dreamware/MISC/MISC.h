#include <chrono>
#include <string>
#include "Includes.h"
#include "../UTILS/interfaces.h"
#include "../utils/offsets.h"
#include "../utils/NetvarHookManager.h"
#include "../utils/render.h"
#include "..//SDK/NetChannel.h"

#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CPrediction.h"
#include "../SDK/CBaseWeapon.h"


#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)




class Misc
{
	typedef bool(__cdecl* ServerRankRevealAllF)(float*);
	typedef float vec_t;


public:

	

	static void CalcAngle(Vector src, Vector dst, Vector& angles)
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		angles.y = atan(delta.y / delta.x) * 57.295779513082f;
		angles.x = atan(delta.z / hyp) * 57.295779513082f;
		if (delta.x >= 0.0)
			angles.y += 180.0f;
		angles.z = 0;
	}

	

	static void AutoBlocker(SDK::CUserCmd* cmd)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;

		float bestdist = 250.f;
		int index = -1;

		for (int i = 0; i < INTERFACES::Globals->maxclients; i++)
		{
			SDK::CBaseEntity* entity = INTERFACES::ClientEntityList->GetClientEntity(i);
			if (!entity)
				continue;

			if (!local_player->IsAlive() || local_player->GetIsDormant() || entity == local_player)
				continue;

			float dist = local_player->GetVecOrigin().DistTo(entity->GetVecOrigin());

			if (dist < bestdist)
			{
				bestdist = dist;
				index = i;
			}
		}

		if (index == -1)
			return;

		SDK::CBaseEntity* target = INTERFACES::ClientEntityList->GetClientEntity(index);

		if (!target)
			return;

		Vector angles;
		CalcAngle(local_player->GetVecOrigin(), target->GetVecOrigin(), angles);

		angles.y -= local_player->GetEyeAngles().y;
		angles.Normalized();

		if (angles.y < 0.0f)
			cmd->sidemove = 450.f;
		else if (angles.y > 0.0f)
			cmd->sidemove = -450.f;
	}

	static Vector Normalize(Vector& angs)
	{
		while (angs.y < -180.0f)
			angs.y += 360.0f;
		while (angs.y > 180.0f)
			angs.y -= 360.0f;
		if (angs.x > 89.0f)
			angs.x = 89.0f;
		if (angs.x < -89.0f)
			angs.x = -89.0f;
		angs.z = 0;
		return angs;
	}

	static vec_t Normalize_y(vec_t ang)
	{
		while (ang < -180.0f)
			ang += 360.0f;
		while (ang > 180.0f)
			ang -= 360.0f;
		return ang;
	}
	static bool AimStep(Vector angSource, Vector& angDestination)
	{
		Vector angDelta = Normalize(angDestination - angSource);
		if (angDelta.Abs() > 40.f)
		{
			angDestination = Normalize(angSource + angDelta / angDelta.Abs() * 40.f);
			return false;
		}
		return true;
	}


	static void g_correct_movement(SDK::CUserCmd* cmd)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!local_player)
			return;
		float normalizedx;


		cmd->forwardmove = clamp(cmd->forwardmove, -450.f, 450.f);
		cmd->sidemove = clamp(cmd->sidemove, -450.f, 450.f);
		cmd->upmove = clamp(cmd->upmove, -320.f, 320.f);

		cmd->viewangles.x = clamp(cmd->viewangles.x, -89.f, 89.f);

		if (SETTINGS::settings.Antiuntrusted)
			normalizedx = fmod(cmd->viewangles.x + 180.f, 360.f) - 180.f;
		Vector real_viewangles;
		INTERFACES::Engine->GetViewAngles(real_viewangles);

		Vector vecMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
		float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

		Vector angMove;
		MATH::VectorAngles(vecMove, angMove);

		float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

		cmd->forwardmove = cos(yaw) * speed;
		cmd->sidemove = sin(yaw) * speed;

		cmd->viewangles = MATH::NormalizeAngle(cmd->viewangles);

		if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->forwardmove *= -1;
	}

	static void FixCmd(SDK::CUserCmd* cmd)
	{
		if (SETTINGS::settings.Antiuntrusted) {

			cmd->viewangles.y = MATH::NormalizeYaw(cmd->viewangles.y);
			UTILS::ClampAngles(cmd->viewangles);

			if (cmd->forwardmove > 450)
				cmd->forwardmove = 450;
			if (cmd->forwardmove < -450)
				cmd->forwardmove = -450;

			if (cmd->sidemove > 450)
				cmd->sidemove = 450;
			if (cmd->sidemove < -450)
				cmd->sidemove = -450;
		}
	}

	static void g_bhop(SDK::CUserCmd* cmd)
	{
		if (SETTINGS::settings.AutoJump) {

			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player) return;

			if (cmd->buttons & IN_JUMP)
			{
				int local_flags = local_player->GetFlags();
				if (!(local_flags & FL_ONGROUND))
					cmd->buttons &= ~IN_JUMP;
			}
			if (SETTINGS::settings.zbhob && GetAsyncKeyState('Z') & 0x8000)
			{
				static int tick = 0;
				tick += 0.5;
				int handler = tick / 3.14;
				cmd->sidemove = 450 * std::sin(handler);
			}
		}
	}

	static void g_strafer(SDK::CUserCmd* cmd)
	{
		if (SETTINGS::settings.AutoStrafe)
		{
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			if (!local_player)
				return;

			if (!SETTINGS::settings.wasd)
			{
				bool bKeysPressed = true;
				if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x57) || GetAsyncKeyState(0x53) || GetAsyncKeyState(0x44))
					bKeysPressed = false;

				if (!(local_player->GetFlags() & FL_ONGROUND) && bKeysPressed)
				{
					cmd->forwardmove = (1550.f * 5) / local_player->GetVecVelocity().Length2D();
					cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
					if (cmd->forwardmove > 450.f)
						cmd->forwardmove = 450.f;
				}
			}
			else
			{
				if (!(local_player->GetFlags() & FL_ONGROUND))
				{
					if (GetAsyncKeyState(0X57)) //w
					{

						cmd->forwardmove = (1550.f * 5) / local_player->GetVecVelocity().Length2D();
						cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
						if (cmd->forwardmove > 450.f)
							cmd->forwardmove = 450.f;

					}
					else if (GetAsyncKeyState(0X41)) //a
					{

						cmd->sidemove = (-1550.f * 5) / local_player->GetVecVelocity().Length2D();
						cmd->forwardmove = (cmd->command_number % 2) == 0 ? 450.f : -450.f;
						if (cmd->sidemove < -450.f)
							cmd->sidemove = -450.f;
					}
					else if (GetAsyncKeyState(0x44)) //d
					{
						cmd->sidemove = (1550.f * 5) / local_player->GetVecVelocity().Length2D();
						cmd->forwardmove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
						if (cmd->sidemove > 450.f)
							cmd->sidemove = 450.f;
					}
					else if (GetAsyncKeyState(0x53)) //s
					{
						cmd->forwardmove = (-1550.f * 5) / local_player->GetVecVelocity().Length2D();
						cmd->sidemove = (cmd->command_number % 2) == 0 ? 450.f : -450.f;
						if (cmd->forwardmove < -450.f)
							cmd->forwardmove = -450.f;
					}
					else //default
					{
						cmd->forwardmove = (1550.f * 5) / local_player->GetVecVelocity().Length2D();
						cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
						if (cmd->forwardmove > 450.f)
							cmd->forwardmove = 450.f;
					}
				}
			}
		}
	}

	static void edgejump(SDK::CUserCmd* cmd)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return;


		if (local_player->GetMoveType() == SDK::MOVETYPE_LADDER || local_player->GetMoveType() ==SDK::MOVETYPE_NOCLIP)
			return;

		Vector Start, End;
		Start = local_player->GetVecOrigin();

		VectorCopy(local_player->GetVecOrigin(), Start);
		VectorCopy(Start, End);

		End.z -= 32;

		SDK::Ray_t ray;
		ray.Init(Start, End);

		SDK::trace_t trace;

		SDK::CTraceFilter filter;
		filter.pSkip1 = local_player;

		INTERFACES::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trace);

		int EdgeJumpKey = SETTINGS::settings.edgejumpkey;
		if (trace.flFraction == 1.0f && EdgeJumpKey > 0 && GetAsyncKeyState(EdgeJumpKey))
		{
			cmd->buttons |= IN_JUMP;
		}

		if (!(local_player->GetFlags() & FL_ONGROUND) && GetAsyncKeyState(EdgeJumpKey) && SETTINGS::settings.strafeedgejump)
		{
			cmd->forwardmove = (1550.f * 5) / local_player->GetVelocity().Length2D();
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (cmd->forwardmove > 450.f)
				cmd->forwardmove = 450.f;
		}
	}

	static void knife_Run(SDK::CUserCmd* cmd) {

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

		if (!weapon)
			return;
		if (!weapon->is_knife())
			return;
		for (int y = 0; y <= 360; y += 360.f / 6.f) {
			for (int x = -89; x <= 89; x += 179.f / 6.f) {
				Vector ang = Vector(x, y, 0);
				Vector dir;
				MATH::AngleVectors3(ang, dir);
				SDK::trace_t trace;

				auto TraceLine = [](Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, SDK::CBaseEntity *ignore, int collisionGroup, SDK::trace_t *ptr)
				{
					SDK::Ray_t ray;

					SDK::CTraceFilter filter;
					filter.pSkip1 = ignore;

					ray.Init(vecAbsStart, vecAbsEnd);

					INTERFACES::Trace->TraceRay(ray, mask, &filter, ptr);

				};

				

				TraceLine(local_player->GetEyePosition(), local_player->GetEyePosition() + (dir * 64), MASK_SHOT, local_player, 0, &trace);

				if (trace.m_pEnt == nullptr)
					continue;
				if (trace.m_pEnt == local_player)
					continue;
				if (!trace.m_pEnt->IsAlive())
					continue;
				if (!(trace.m_pEnt->GetHealth() > 0))
					continue;
				if (trace.m_pEnt->GetTeamNum() == local_player->GetTeamNum())
					continue;
				if (trace.m_pEnt->GetIsDormant())
					continue;
				if (trace.m_pEnt->GetImmunity())
					continue;
				SDK::player_info_t info;
				if (!(INTERFACES::Engine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info)))
					continue;
				cmd->viewangles = Vector(x, y, 0);
				cmd->buttons |= IN_ATTACK2;
				return;
			}
		}
	}
	


	static void fake_crouch(SDK::CUserCmd * cmd)  
	{
		static bool counter = false;
		if (GetAsyncKeyState(SETTINGS::settings.Fakecrouchkey))
		{
			if (SETTINGS::settings.fakecrouch)
			{
				static bool counter = false;
				static int counters = 0;
				if (counters == 9)
				{
					counters = 0;
					counter = !counter;
				}
				counters++;
				if (counter)
				{
					cmd->buttons |= IN_DUCK;
					GLOBAL::should_send_packet = true;
				}
				else
					cmd->buttons &= ~IN_DUCK;
			}
		}
	}
};

