#include "../MISC/includes.h"
#include "../UTILS/interfaces.h"
#include "../UTILS//render.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/AutoWall.h"

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
static bool jitter2 = false;

namespace AAUTILS
{

	float randnum(float Min, float Max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	}

}

inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

float get_curtime(SDK::CUserCmd* ucmd) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return 0;

	int g_tick = 0;
	SDK::CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * INTERFACES::Globals->interval_per_tick;
	return curtime;
}

bool next_lby_update(const float yaw_to_break, SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = INTERFACES::Globals->curtime;
	local_update = next_lby_update_time;

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f && !fake_walk)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

bool next_lby_update(SDK::CUserCmd* m_pcmd) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (local_player) {
		static float next_lby_update_time;
		const float current_time = get_curtime(m_pcmd);
		local_update = next_lby_update_time;
		static bool in_air;
		static bool stop;
		stop = local_player->GetVelocity().Length2D() < 0.1 && local_player->GetFlags() & FL_ONGROUND;
		in_air = !(local_player->GetFlags() & FL_ONGROUND);
		if (stop || fake_walk || in_air) {
			if ((next_lby_update_time < current_time)) {
				next_lby_update_time = current_time + 1.1f;
				return true;
			}
		}
	}
	return false;
}


float fov_player(Vector ViewOffSet, Vector View, SDK::CBaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = aimbot->get_hitbox_pos(entity, hitbox);

	MATH::AngleVectors(Angles, &Forward);
	MATH::VectorSubtract(AimPos, Origin, Delta);
	MATH::NormalizeNum(Delta, Delta);

	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

void CAntiAim::freestand(SDK::CUserCmd* cmd, float yawangs)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = aimbot->get_hitbox_pos(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](SDK::CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();

		SDK::Ray_t ray;
		ray.Init(newhead, eyepos);

		SDK::CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		SDK::trace_t trace1, trace2;
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.Dist(eyepos) - leyepos.Dist(eyepos) + 2.f;
		return endpos1.Dist(endpos2) + add / 1;
	};

	int index = closest_to_crosshair();

	static SDK::CBaseEntity* entity;

	if (index != -1)
		entity = INTERFACES::ClientEntityList->GetClientEntity(index);

	float step = (1 * M_PI) / 18.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}

	if (!GLOBAL::should_send_packet)
	{
		if (next_lby_update(cmd))
		{
			cmd->viewangles.y = last_real + SETTINGS::settings.antiaim.breaklby.delta;
		}
		else
		{
			if (no_active)
			{

			}
			else
				cmd->viewangles.y = RAD2DEG(bestrotation);

			last_real = cmd->viewangles.y;
		}
	}
}


void automatic_default(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = aimbot->get_hitbox_pos(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](SDK::CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();

		SDK::Ray_t ray;
		ray.Init(newhead, eyepos);

		SDK::CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		SDK::trace_t trace1, trace2;
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f;
	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}
	if (no_active)
		cmd->viewangles.y += 180.f;
	else
		cmd->viewangles.y = RAD2DEG(bestrotation) + (SETTINGS::settings.spinangle + 1) / 2.f + std::fmodf(INTERFACES::Globals->curtime * ((SETTINGS::settings.spinspeed1 * 5) + 1), (SETTINGS::settings.spinangle + 1));

	last_real = cmd->viewangles.y;
}


void automatic_jitter(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	float ranges = SETTINGS::settings.frestandingjitter / 2;

	static bool jitter = false;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = aimbot->get_hitbox_pos(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](SDK::CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();

		SDK::Ray_t ray;
		ray.Init(newhead, eyepos);

		SDK::CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		SDK::trace_t trace1, trace2;
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f;
	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}
	if (no_active)
		cmd->viewangles.y += 180.f + AAUTILS::randnum(ranges, -ranges);
	else
	{
		jitter = !jitter;
		if (jitter)
			cmd->viewangles.y = RAD2DEG(bestrotation) + ranges;
		else
			cmd->viewangles.y = RAD2DEG(bestrotation) - ranges;
	}
	

	last_real = cmd->viewangles.y;
}

void Manual_AA(SDK::CUserCmd* cmd)
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (GetAsyncKeyState(SETTINGS::settings.manualright))
	{
		side2 = true;
		side1 = false;
		back = false;
	}
	else if (GetAsyncKeyState(SETTINGS::settings.manualleft))
	{
		side2 = false;
		side1 = true;
		back = false;
	}

	else if (GetAsyncKeyState(SETTINGS::settings.manualback))
	{
		side2 = false;
		side1 = false;
		back = true;
	}


	if (side2)
	{


		cmd->viewangles.y -= 90;


	}
	else if (side1)
	{




		cmd->viewangles.y += 90;





	}
	else if (back)
	{



		cmd->viewangles.y -= 180;



	}


}

void CAntiAim::AtTargets(SDK::CUserCmd* pCmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	for (int i = 1; i <= INTERFACES::Engine->GetMaxClients(); ++i) {
		SDK::CBaseEntity* Entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!Entity
			|| !Entity->IsAlive()
			|| Entity->GetIsDormant()
			|| Entity == local_player
			|| Entity->GetTeam() == local_player->GetTeam())
			continue;

		pCmd->viewangles.y = UTILS::CalcAngle(local_player->GetOrigin(), Entity->GetOrigin()).y;
		continue;
	}
}

void Pitch_minimal(SDK::CUserCmd* cmd)
{

	SDK::CBaseEntity *plocalminimal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	static float Timer = 0;
	Timer++;
	if (!plocalminimal->IsAlive())
		Timer = 0;

	if (plocalminimal->GetFlags() & FL_ONGROUND)
	{
		if (Timer > 1 && Timer < 50)
		{
			cmd->viewangles.x = 1080;
		}
		else
			cmd->viewangles.x = 87;
	}
	else
	{
		Timer = 0;
		cmd->viewangles.x = 87;
	}
}

void Pitch_Down(SDK::CUserCmd* cmd)
{
	cmd->viewangles.x = 91.f; 
}
void Pitch_Up(SDK::CUserCmd* cmd)
{
	cmd->viewangles.x = 179.f;
}
void Pitch_zero(SDK::CUserCmd* cmd)
{
	cmd->viewangles.x = -179.f;
}

void rotaion(SDK::CUserCmd* cmd)
{
	static int Tick;
	Tick += SETTINGS::settings.antiaim.spinspeed + 1;

	cmd->viewangles.y += Tick;
}

void CAntiAim::do_antiaim(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return; auto grenade = (SDK::CBaseCSGrenade*)weapon;
	if (local_player->GetMoveType() == SDK::MOVETYPE_LADDER) return;
	if (cmd->buttons & IN_USE) return;
	float desyncamount = SETTINGS::settings.desynoth;
	//if (SETTINGS::settings.bBlockBot && GetAsyncKeyState(SETTINGS::settings.iBlockBotKey))
	//	return;

	//if (SETTINGS::settings.Antiuntrusted)
	//	return;

	if (cmd->buttons & IN_ATTACK && aimbot->can_shoot(cmd)) return;
	if (weapon->get_full_info()->m_WeaponType == 9) return;
	if (!SETTINGS::settings.aa_bool) return;
	if (GLOBAL::DisableAA) return;
	if (weapon->is_grenade()) return;
	auto yawfeetdelta = local_player->GetAnimState()->m_flGoalFeetYaw - cmd->viewangles.y;




	
	switch (SETTINGS::settings.standpitch)
	{
	     case 0: break;
	     case 1: Pitch_Down(cmd); break;
	     case 2: Pitch_Up(cmd); break;
         case 3: Pitch_zero(cmd); break;
	     case 4: Pitch_minimal(cmd); break;
	     case 5: cmd->viewangles.x = randnum(-180, 1080) + local_player->getmaxdesync(); break;
	     case 6: cmd->viewangles.x = SETTINGS::settings.custompitch; break;

	}
	switch (SETTINGS::settings.yawbaseangle)
	{
	case 0: break;
	case 1:
	{
		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else
			cmd->viewangles.y += 0 + SETTINGS::settings.yawadd; break;
		
				

	}break;
	case 2:
	{
		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else
		   cmd->viewangles.y += 180.000000f + SETTINGS::settings.yawadd; break;

		

	}break;
	case 3:
	{
		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else
			AtTargets(cmd); break;

		


	}break;

	}

	switch (SETTINGS::settings.yawangle)
	{
	case 0: break;
	case 1:
	{
		static float asd = 120;
		asd += 5;
		if (asd > 240)
			asd = 120;
		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else
			cmd->viewangles.y += asd;
		


	}break;
	case 2:
	{
		float range = SETTINGS::settings.jitterrange / 2;

		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else 
		   cmd->viewangles.y += AAUTILS::randnum(range, -range);
		


	}break;
	case 3:
	{
		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else
			jitter2 = !jitter2;
			cmd->viewangles.y += jitter2 ? 180 : 0;
		

	}break;
	case 4:
	{
		if (GLOBAL::should_send_packet && cmd->command_number % 3)
		{

			if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
			{
				cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
			}
			else
			{
				cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
			}


		}
		else 
		   rotaion(cmd);
		

	}break;

	}

	if (SETTINGS::settings.enablefreestanding)
	{
		switch (SETTINGS::settings.freestand_type)
		{
		case 1:
		{
			if (GLOBAL::should_send_packet && cmd->command_number % 3)
			{

				if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
				{
					cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
				}
				else
				{
					cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
				}


			}
			else
				automatic_default(cmd);

			
		}
		break;
		case 2:
		{
			
			if (GLOBAL::should_send_packet && cmd->command_number % 3)
			{

				if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
				{
					cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
				}
				else
				{
					cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
				}


			}
			else
				automatic_jitter(cmd);

			
		}
		break;
		case 3:
		{

			if (GLOBAL::should_send_packet && cmd->command_number % 3)
			{

				if (yawfeetdelta < local_player->getmaxdesync() + randnum(-desyncamount, desyncamount))
				{
					cmd->viewangles.y = GLOBAL::real_angles.y + local_player->getmaxdesync() + randnum(-desyncamount, desyncamount); /* ye right!! x) */
				}
				else
				{
					cmd->viewangles.y = GLOBAL::real_angles.y - local_player->getmaxdesync() + randnum(-desyncamount, desyncamount);
				}


			}
			else
				Manual_AA(cmd); // manual

			
		}
		break;

		}
	}


	

	
}


void CAntiAim::fix_movement(SDK::CUserCmd* cmd)
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




CAntiAim* antiaim = new CAntiAim();