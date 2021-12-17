#pragma once
#include "..\\Visuals\Visuals.h"
#include "..\\Visuals\Drawing.h"
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

class c_antiaims : public singleton<c_antiaims> {
public:
	__forceinline void create_move();
	__forceinline void update_lowerbody_breaker();
	__forceinline void wasdstrafe(CUserCmd* cmd);
	__forceinline void fastStop(CUserCmd* cmd);
	__forceinline void gotoStart(CUserCmd* cmd);
	__forceinline void quickpeek(CUserCmd* cmd);
	__forceinline void slowwalk();
	__forceinline void fake_duck();
private:
	__forceinline void pitch(float pitch);
	__forceinline void yaw(float yaw);
	__forceinline void yaw_static(float yaw);
	__forceinline void pitch_emotion();
	__forceinline void pitch_up();
	__forceinline void pitch_minimal();
	__forceinline void yaw_stay_180();
	__forceinline void yaw_stay_jitter();
	__forceinline void yaw_stay_static();
	__forceinline void yaw_move_180();
	__forceinline void yaw_move_jitter();
	__forceinline void yaw_move_static();
	//__forceinline bool desync(); ÃÎÂÍÎÎÎÎÎÎÎÎÎÎÎÎÎÎ
	__forceinline float freestand();
private:
	bool side = false;
	int desync_autodir = false;
};

using namespace Cheat;

__forceinline bool IsAbleToShoot2()
{
	if (!Interfaces::Globals)
		return false;

	if (!G::LocalPlayer || !G::LocalPlayer->GetActiveWeapon())
		return false;

	auto flServerTime = (float)G::LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
	auto flNextPrimaryAttack = G::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack();

	return(!(flNextPrimaryAttack > flServerTime));
}

__forceinline bool is_throwing() {
		auto g = G::LocalPlayer->GetActiveWeapon();
		if (g->IsPinPulled())
		{
			return true;
		}
	return false;
}

__forceinline void rotate_movement2(float yaw) {
	Vector viewangles;
	Interfaces::Engine->GetViewAngles(&viewangles);

	float rotation = DEG2RAD(viewangles.y - yaw);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * G::UserCmd->forwardmove) - (sin_rot * G::UserCmd->sidemove);
	float new_sidemove = (sin_rot * G::UserCmd->forwardmove) + (cos_rot * G::UserCmd->sidemove);

	G::UserCmd->forwardmove = new_forwardmove;
	G::UserCmd->sidemove = new_sidemove;
}

inline bool lby() {
	static bool m_bWillUpd;
	static bool m_bWasMove;
	static float m_flLastLbyTime;

	float flTime = Interfaces::Globals->curtime;

	if (G::LocalPlayer->GetVelocity().Length2D() > 110 || !(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		m_bWasMove = true;
	else if (flTime >= m_flLastLbyTime + 1 && G::LocalPlayer->GetVelocity().Length2D() <= 110 && G::LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		if (m_bWasMove) {
			m_flLastLbyTime = flTime + 0.22f;
			m_bWillUpd = true;
		}
		else {
			m_flLastLbyTime = flTime + 1.1f;
			m_bWillUpd = true;
		}
	}
	else
		m_bWillUpd = false;

	return m_bWillUpd;
}
bool LBYUpdate()
{
	static float NextUpdate = 0;
	float Curtime = Interfaces::Globals->curtime;
	auto* AnimState = G::LocalPlayer->GetAnimState();

	if (!AnimState || !(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		return false;

	if (AnimState->m_unknown > 0.1f)
		NextUpdate = Curtime + 0.22f;

	if (NextUpdate < Curtime)
	{
		NextUpdate = Curtime + 1.1f;
		return true;
	}

	return false;
}
float NormalizeYaw(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}
float get_curtime2(CUserCmd* ucmd) {
	auto local_player = G::LocalPlayer;

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	return g_tick;
}
float get_curtime(CUserCmd* ucmd) {
	auto local_player = G::LocalPlayer;

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::Globals->interval_per_tick;
	return curtime;
}
bool next_lby_update(CUserCmd* cmd)
{
	auto local_player = G::LocalPlayer;

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->GetAnimState();
	if (!animstate)
		return false;

	auto net_channel = Interfaces::Engine->GetNetChannel();

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->m_velocity > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime && (std::fabsf(animstate->m_flGoalFeetYaw - animstate->m_flEyeYaw) > 35))
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

bool break_lby = false;
float next_update = 0;
void c_antiaims::update_lowerbody_breaker() {
	float speed = G::LocalPlayer->GetVelocity().Length2DSqr();

	if (speed > 0.1)
		next_update = Interfaces::Globals->curtime + 0.22;

	break_lby = false;

	if (next_update <= Interfaces::Globals->curtime) {
		next_update = Interfaces::Globals->curtime + 1.1;
		break_lby = true;
	}

	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		break_lby = false;
}
bool is_bhopping = false;
bool in_transition = true;
float calculated_direction = 0;
float true_direction = 0;
float wish_direction = 0;
float step = 0;
float rough_direction = 0;
void wasdstrafe(CUserCmd* cmd) {
	/*// Define rough directions
	enum directions {
		FORWARDS = 0,
		BACKWARDS = 180,
		LEFT = 90,
		RIGHT = -90
	};

	// Reset direction when player is not strafing
	is_bhopping = cmd->buttons & IN_JUMP;
	if (!is_bhopping && G::LocalPlayer->GetFlags() & FL_ONGROUND) {
		calculated_direction = directions::FORWARDS;
		in_transition = false;
		return;
	}

	// Get true view angles
	Vector base{ };
	Interfaces::Engine->GetViewAngles(&base);

	// Calculate the rough direction closest to the player's true direction
	auto get_rough_direction = [&](float true_direction) -> float {
		// Make array with our four rough directions
		std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
		float best_angle, best_delta = 181.f;

		// Loop through our rough directions and find which one is closest to our true direction
		for (size_t i = 0; i < minimum.size(); ++i) {
			float rough_direction = base.y + minimum.at(i);
			float delta = fabsf(NormalizeYaw(true_direction - rough_direction));

			// Only the smallest delta wins out
			if (delta < best_delta) {
				best_angle = rough_direction;
				best_delta = delta;
			}
		}

		return best_angle;
	};

	// Get true direction based on player velocity
	true_direction = G::LocalPlayer->GetVelocity().Angle(&G::LocalPlayer->GetVelocity()).y;

	// Detect wish direction based on movement keypresses
	if (cmd->buttons & IN_FORWARD) {
		wish_direction = base.y + directions::FORWARDS;
	}
	else if (cmd->buttons & IN_BACK) {
		wish_direction = base.y + directions::BACKWARDS;
	}
	else if (cmd->buttons & IN_MOVELEFT) {
		wish_direction = base.y + directions::LEFT;
	}
	else if (cmd->buttons & IN_MOVERIGHT) {
		wish_direction = base.y + directions::RIGHT;
	}
	else {
		// Reset to forward when not pressing a movement key, then fix anti-aim strafing by setting IN_FORWARD
		cmd->buttons |= IN_FORWARD;
		wish_direction = base.y + directions::FORWARDS;
	}

	// Calculate the ideal strafe rotation based on player speed (c) navewindre
	float speed_rotation = min(RAD2DEG(std::asinf(30.f / G::LocalPlayer->GetVelocity().Length2D())) * 0.5f, 45.f);
	if (in_transition) {
		// Calculate the step by using our ideal strafe rotation
		float ideal_step = speed_rotation + calculated_direction;
		step = fabsf(NormalizeYaw(calculated_direction - ideal_step));

		// Check when the calculated direction arrives close to the wish direction
		if (fabsf(NormalizeYaw(wish_direction - calculated_direction)) > step) {
			float add = NormalizeYaw(calculated_direction + step);
			float sub = NormalizeYaw(calculated_direction - step);

			// Step in direction that gets us closer to our wish direction
			if (fabsf(NormalizeYaw(wish_direction - add)) >= fabsf(NormalizeYaw(wish_direction - sub))) {
				calculated_direction -= step;
			}
			else {
				calculated_direction += step;
			}
		}
		else {
			in_transition = false;
		}
	}
	else {
		// Get rough direction and setup calculated direction only when not transitioning
		rough_direction = get_rough_direction(true_direction);
		calculated_direction = rough_direction;

		// When we have a difference between our current (rough) direction and our wish direction, then transition
		if (rough_direction != wish_direction) {
			in_transition = true;
		}
	}

	// Set movement up to be rotated
	cmd->forwardmove = 0.f;
	cmd->sidemove = cmd->command_number % 2 ? 450.f : -450.f;

	// Calculate ideal rotation based on our newly calculated direction
	float direction = (cmd->command_number % 2 ? speed_rotation : -speed_rotation) + calculated_direction;

	// Rotate our direction based on our new, defininite direction
	float rotation = DEG2RAD(base.y - direction);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
	float sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

	// Apply newly rotated movement
	cmd->forwardmove = forwardmove;
	cmd->sidemove = sidemove;*/	
}

void c_antiaims::wasdstrafe(CUserCmd* cmd) {
	if (G::LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (cmd->buttons & IN_SPEED && G::LocalPlayer->GetVelocity().Length2D() < 2.f)
		return;

	static float yaw_add = 0.f;
	static const auto cl_sidespeed = Interfaces::Var->FindVar("cl_sidespeed");

	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND)) {
		bool back = cmd->buttons & IN_BACK;
		bool forward = cmd->buttons & IN_FORWARD;
		bool right = cmd->buttons & IN_MOVELEFT;
		bool left = cmd->buttons & IN_MOVERIGHT;

		if (back) {
			yaw_add = -180.f;
			if (right)
				yaw_add -= 45.f;
			else if (left)
				yaw_add += 45.f;
		}
		else if (right) {
			yaw_add = 90.f;
			if (back)
				yaw_add += 45.f;
			else if (forward)
				yaw_add -= 45.f;
		}
		else if (left) {
			yaw_add = -90.f;
			if (back)
				yaw_add -= 45.f;
			else if (forward)
				yaw_add += 45.f;
		}
		else {
			yaw_add = 0.f;
		}

		cmd->viewangles.y += yaw_add;
		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;

		const auto delta = Utilities->Math_NormalizeYaw2(cmd->viewangles.y - RAD2DEG(atan2(G::LocalPlayer->GetVelocity().y, G::LocalPlayer->GetVelocity().x)));

		cmd->sidemove = delta > 0.f ? -cl_sidespeed->GetFloat() : cl_sidespeed->GetFloat();

		cmd->viewangles.y = Utilities->Math_NormalizeYaw2(cmd->viewangles.y - delta);
	}
}

void c_antiaims::fastStop(CUserCmd* cmd)
{
	Vector velocity = G::LocalPlayer->GetVelocity();
	Vector direction;
	Utilities->Math_VectorAngles(velocity, direction);
	float speed = velocity.Length2D();

	direction.y = cmd->viewangles.y - direction.y;

	Vector forward;
	Utilities->Math_AngleVectors(direction, forward);

	Vector right = (forward + Vector(0.217812, 0.217812, 0.217812)) * -speed;
	Vector left = (forward + Vector(-0.217812, -0.217812, -0.217812)) * -speed;

	Vector move_forward = (forward + Vector(0.217812, 0.217812, 0.217812)) * -speed;
	Vector move_backward = (forward + Vector(-0.217812, -0.217812, -0.217812)) * -speed;

	if (!(cmd->buttons & IN_MOVELEFT))
	{
		cmd->sidemove += +left.y;
	}

	if (!(cmd->buttons & IN_MOVERIGHT))
	{
		cmd->sidemove -= -right.y;
	}

	if (!(cmd->buttons & IN_FORWARD))
	{
		if (cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT || !(G::LocalPlayer->GetFlags() & FL_ONGROUND) || G::LocalPlayer->GetAnimState()->m_bInHitGroundAnimation) //insert here your bool for auto strafe
			return;

		cmd->forwardmove += +move_forward.x;
	}

	if (!(cmd->buttons & IN_BACK))
	{
		if (cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT || !(G::LocalPlayer->GetFlags() & FL_ONGROUND) || G::LocalPlayer->GetAnimState()->m_bInHitGroundAnimation) //insert here your bool for auto strafe
			return;

		cmd->forwardmove -= -move_backward.x;
	}
}
static Vector G::quickpeekstartpos = Vector(0, 0, 0);
static bool hasShot = false;
void c_antiaims::gotoStart(CUserCmd* cmd) {
	if (!G::LocalPlayer || !G::LocalPlayer->IsAlive()) return;
	Vector playerLoc = G::LocalPlayer->GetAbsOrigin();

	float yaw = cmd->viewangles.y;
	Vector VecForward = playerLoc - G::quickpeekstartpos;

	Vector translatedVelocity = Vector{
		(float)(VecForward.x * cos(yaw / 180 * (float)M_PI) + VecForward.y * sin(yaw / 180 * (float)M_PI)),
		(float)(VecForward.y * cos(yaw / 180 * (float)M_PI) - VecForward.x * sin(yaw / 180 * (float)M_PI)),
		VecForward.z
	};
	cmd->forwardmove = -translatedVelocity.x * 20.f;
	cmd->sidemove = translatedVelocity.y * 20.f;
}

void c_antiaims::quickpeek(CUserCmd* cmd) {
	if (!G::LocalPlayer || !G::LocalPlayer->IsAlive()) return;
	if (c_keyhandler::get()->auto_check("rage_quickpeek_key")) {
		if (G::quickpeekstartpos == Vector(0, 0, 0)) {
			G::quickpeekstartpos = G::LocalPlayer->GetAbsOrigin();
		}
		else {
			if (cmd->buttons & IN_ATTACK) hasShot = true;
			if (hasShot) {
				gotoStart(cmd);
				if (G::LocalPlayer->GetAbsOrigin() == G::quickpeekstartpos)
					hasShot = false;
			}
		}
	}
	else {
		hasShot = false;
		G::quickpeekstartpos = Vector{ 0, 0, 0 };
	}
}

__forceinline void c_antiaims::create_move() {
	this->slowwalk();

	/*if (*Interfaces::GameRules && (*Interfaces::GameRules)->IsFreezePeriod())
		return;*/

	if (((G::UserCmd->buttons & IN_ATTACK && G::LocalPlayer->GetActiveWeapon()->GetItemDefenitionIndex() != WEAPON_REVOLVER) || (G::RagebotIsShooting && G::LocalPlayer->GetActiveWeapon()->GetItemDefenitionIndex() == WEAPON_REVOLVER)) && IsAbleToShoot2())
		return;

	if ((G::UserCmd->buttons & IN_USE) || (G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER))
		return;

	if (is_throwing())
		return;

	static float SentYaw;
	static float Real;
	static float Fake;
	static float spin = 0;
	static float jitteradd = 0;
	static bool broke = false;
	if (c_config::get()->b["misc_antiuntrusted"]) {
		switch (c_config::get()->i["rage_antiaim_pitch"]) {
		case 1:
			G::UserCmd->viewangles.x = 89;
			break;
		case 2:
			G::UserCmd->viewangles.x = -89;
			break;
		case 3:
			G::UserCmd->viewangles.x = 89;
			break;
		case 4:
			G::UserCmd->viewangles.x = 80;
			break;
		case 5:
			static int switch_ = 0;
			switch (switch_) {
			case 0:
				G::UserCmd->viewangles.x = 89;
				switch_ = 1;
				break;
			case 1:
				G::UserCmd->viewangles.x = 0;
				switch_ = 2;
				break;
			case 2:
				G::UserCmd->viewangles.x = -89;
				switch_ = 0;
				break;
			}
			break;
		}
	}
	else {
		switch (c_config::get()->i["rage_antiaim_pitch"]) {
		case 1:
			G::UserCmd->viewangles.x = -90;
			break;
		case 2:
			G::UserCmd->viewangles.x = 90;
			break;
		case 3:
			G::UserCmd->viewangles.x = -178;
			break;
		case 4:
			G::UserCmd->viewangles.x = 89;
			break;
		case 5:
			static int switch_ = 0;
			switch (switch_) {
			case 0:
				G::UserCmd->viewangles.x = 178;
				switch_ = 1;
				break;
			case 1:
				G::UserCmd->viewangles.x = 90;
				switch_ = 2;
				break;
			case 2:
				G::UserCmd->viewangles.x = -178;
				switch_ = 0;
				break;
			}
			break;
		}
	}
	
	if (c_config::get()->i["rage_antiaim_yaw"] > 0) {
		if (G::SendPacket) {
			static int Jitter;
			static int jitterswitch_ = 0;
			switch (c_config::get()->i["rage_antiaim_yaw_jitter"]) {
			case 1:
				switch (jitterswitch_) {
				case 0:
					Jitter = (c_config::get()->i["rage_antiaim_yaw_jitter_custom"]);
					jitterswitch_ = 1;
					break;
				case 1:
					Jitter = 0;
					jitterswitch_ = 0;
					break;
				}
				break;
			case 2:
				switch (jitterswitch_) {
				case 0:
					Jitter = (c_config::get()->i["rage_antiaim_yaw_jitter_custom"] / 2);
					jitterswitch_ = 1;
					break;
				case 1:
					Jitter = (c_config::get()->i["rage_antiaim_yaw_jitter_custom"] / -2);
					jitterswitch_ = 0;
					break;
				}
				break;
			case 3:
				Jitter = Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_jitter_custom"] / 2, c_config::get()->i["rage_antiaim_yaw_jitter_custom"] / -2);
				break;
			}
			switch (c_config::get()->i["rage_antiaim_yaw"]) {
			case 1:
				Real = (G::UserCmd->viewangles.y + (c_config::get()->i["rage_antiaim_yaw_jitter"] > 0 ? Jitter : 0) + 180 + c_config::get()->i["rage_antiaim_yaw_custom"] + (c_config::get()->i["rage_antiaim_yawbase"] == 1 ?0 : 0));
				break;
			case 2:
				Real = spin;
				spin += c_config::get()->i["rage_antiaim_yaw_custom"];
				break;
			case 3:
				Real = (c_config::get()->i["rage_antiaim_yaw_custom"] + (c_config::get()->i["rage_antiaim_yaw_jitter"] > 0 ? Jitter : 0));
				break;
			case 4:
				Real = (G::UserCmd->viewangles.y + 180 + c_config::get()->i["rage_antiaim_yaw_custom"] + (c_config::get()->i["rage_antiaim_yaw_jitter"] > 0 ? Jitter : 0) + (c_config::get()->i["rage_antiaim_yawbase"] == 1 ? 0 : 0));
				Real += spin;
				spin += 4;
				if (G::LocalPlayer->GetFlags() & FL_ONGROUND)
					spin = 0;
				break;
			case 5:
				Real = (180 + c_config::get()->i["rage_antiaim_yaw_custom"] + (c_config::get()->i["rage_antiaim_yaw_jitter"] > 0 ? Jitter : 0) + (c_config::get()->i["rage_antiaim_yawbase"] == 1 ? 0 : 0));
				break;
			}
		}
		if (c_config::get()->i["rage_antiaim_desync"] > 0) {
			static bool fakeswitch_ = false;
			switch (c_config::get()->i["rage_antiaim_desync"]) {
			case 1:
				Fake = G::LocalPlayer->GetMaxDesyncDelta();
				break;
			case 2:
				if (fakeswitch_)
					Fake = c_config::get()->i["rage_antiaim_yaw_desync_custom"];
				else
					Fake = c_config::get()->i["rage_antiaim_yaw_desync_custom"] * -1;
				fakeswitch_ = !fakeswitch_;
				break;
			case 3:
				Fake = c_config::get()->i["rage_antiaim_yaw_desync_custom"];
				break;
			}
		}
			
		Fake = clamp(Fake, c_config::get()->i["rage_antiaim_yaw_desync_limit"] * -1, c_config::get()->i["rage_antiaim_yaw_desync_limit"]);

		if (c_keyhandler::get()->auto_check("rage_inverter_key")) {
			Fake = Fake * -1;
		}

		if (c_config::get()->i["rage_antiaim_desync"] > 0) {
			if (c_config::get()->i["rage_antiaim_lby"] > 0 && !c_config::get()->b["rage_doubletap"]) {
				if (c_config::get()->i["rage_antiaim_lby"] == 1) {
					if (next_update >= Interfaces::Globals->curtime) {
						if (!broke && G::SendPacket) {
							G::UserCmd->viewangles.y = Real;
						}
						else {
							broke = false;
							G::SendPacket = false;
							G::UserCmd->viewangles.y = (Real + Fake * 2);
						}
					}
					else {
						broke = true;
						G::SendPacket = false;
						G::UserCmd->viewangles.y = (Real + Fake * -2);
					}
				}
				if (c_config::get()->i["rage_antiaim_lby"] == 2) {
					if (G::LocalPlayer->GetVelocity().Length2D() < 2.f) {
						if (G::UserCmd->command_number % 3 == 0)
							G::SendPacket = true;
						else
							G::SendPacket = false;
					}
					
					if (break_lby) {
						if (c_config::get()->b["rage_fakepeek"]) {
							G::SendPacket = true;
							G::UserCmd->viewangles.y = Real + 180;
							G::TickbaseShift = 10;
						}
						else {
							G::UserCmd->viewangles.y = (Real + Fake * 2);
							G::SendPacket = false;
						}
					}
					else if (G::SendPacket) {
						G::UserCmd->viewangles.y = Real;
					}
					else {
						G::UserCmd->viewangles.y = (Real + Fake * -2);
					}
				}
				/*
				if (break_lby) {
						G::UserCmd->viewangles.y = (Real - clamp((Fake * -1), G::LocalPlayer->GetMaxDesyncDelta() * -1, G::LocalPlayer->GetMaxDesyncDelta()));
						G::SendPacket = false;

					}
					else if (G::SendPacket) {
						G::UserCmd->viewangles.y = Real;
					}
					else {
						G::UserCmd->viewangles.y = (Real + ((Fake * -1) * 2));
					}
				*/
				else if (c_config::get()->i["rage_antiaim_lby"] == 3) {
					if (break_lby) {
						if (c_config::get()->b["rage_fakepeek"]) {
							G::SendPacket = true;
							G::UserCmd->viewangles.y += 180;
							G::TickbaseShift = 10;
						}
						else {
							G::SendPacket = false;
							G::UserCmd->viewangles.y = -clamp(Fake, G::LocalPlayer->GetMaxDesyncDelta() * -1, G::LocalPlayer->GetMaxDesyncDelta());
						}
					}
					else if (G::SendPacket) {
						G::UserCmd->viewangles.y = Real;
					}
					else {
						G::UserCmd->viewangles.y = (Real + (Fake * 2));
					}
				}
			}
			else {
				if (G::SendPacket)
					G::UserCmd->viewangles.y = Real;
				else
					G::UserCmd->viewangles.y += Fake;
			}
		}
		else {
			G::UserCmd->viewangles.y = Real;
		}

			/*if (c_config::get()->i["rage_antiaim_desync"] > 0) {
				if (c_config::get()->i["rage_antiaim_lby"] > 0 && !c_config::get()->b["rage_doubletap"]) {
					if (c_config::get()->i["rage_antiaim_lby"] == 2) {
						if (next_lby_update(G::UserCmd)) {
							if (Fake > 0)
								G::UserCmd->viewangles.y -= G::LocalPlayer->GetMaxDesyncDelta();
							else
								G::UserCmd->viewangles.y += G::LocalPlayer->GetMaxDesyncDelta();
							G::SendPacket = false;
						}
						else if (!G::SendPacket) {
							G::UserCmd->viewangles.y = Fake;
						}
						else {
							G::UserCmd->viewangles.y = Real;
						}
					}
					else if (next_lby_update(G::UserCmd)) {
						G::SendPacket = false;
						if (c_config::get()->i["rage_antiaim_lby"] == 3) {
							G::UserCmd->viewangles.y += clamp(Fake, -58, 58) * -1;
						}
					}
					else if (G::SendPacket)
						G::UserCmd->viewangles.y = Real;
					else
						G::UserCmd->viewangles.y += Fake;
				}
				else {
					if (G::SendPacket) {
						G::UserCmd->viewangles.y = Real;
					}
						
					else {
						G::UserCmd->viewangles.y += Fake;
					}
						
				}
			}
			else {
				G::UserCmd->viewangles.y = Real;
			}
			*/
			
	}
	


	/*if (c_config::get()->i["rage_antiaim_pitch"] == 1 || c_config::get()->i["rage_antiaim_pitch"] == 3)
		G::UserCmd->viewangles.x = 89;

	if (c_config::get()->i["rage_antiaim_pitch"] == 2)
		G::UserCmd->viewangles.x = -89;

	if (c_config::get()->i["rage_antiaim_pitch"] == 4)
		G::UserCmd->viewangles.x = 80;

	if (c_config::get()->i["rage_antiaim_pitch"] == 5) {
		static int switch_ = 0;
		switch (switch_) {
		case 0:
			G::UserCmd->viewangles.x = 89;
			switch_ = 1;
			break;
		case 1:
			G::UserCmd->viewangles.x = 0;
			switch_ = 2;
			break;
		case 2:
			G::UserCmd->viewangles.x = -89;
			switch_ = 0;
			break;
		}
		
	}*/
		

	/*if (c_config::get()->i["rage_antiaim_pitch"] == 1)
		this->pitch_emotion();
	if (c_config::get()->i["rage_antiaim_pitch"] == 2)
		this->pitch_up();
	if (c_config::get()->i["rage_antiaim_pitch"] == 3)
		this->pitch_minimal();

	bool desisenkisleft = false;
	bool desisenkisleft2 = false;*/
		/*if (!G::SendPacket) {
			if (c_config::get()->i["rage_antiaim_yaw"] == 1)
				G::UserCmd->viewangles.y += c_config::get()->i["rage_antiaim_yaw_custom"];
			if (c_config::get()->i["rage_antiaim_yaw"] == 2)
				this->yaw_stay_jitter();
			if (c_config::get()->i["rage_antiaim_yaw"] == 3)
				this->yaw_stay_static();
			//realAngles = G::UserCmd->viewangles.y;
			//G::RealAngle.y = G::UserCmd->viewangles.y;
			//G::LocalPlayer->SetAbsAngles(Vector(0, G::UserCmd->viewangles.y, 0));
		}*/
}
__forceinline void c_antiaims::fake_duck() {
	static int cnt = 0;
	static bool do_ = false;
	if (c_config::get()->b["rage_fakeduck"] && c_keyhandler::get()->auto_check("rage_fakeduck_key") && (!c_config::get()->b["rage_doubletap"] || c_config::get()->i["rage_doubletap_mode"] == 0)) {
		G::SendPacket = false;
		G::FakelagShouldLag = false;
		G::IsDoubleTapShooting = true;
		if (cnt % 14 == 0)
			do_ = true;
		else if (cnt % 14 == 6)
			G::SendPacket = true;
		else if (cnt % 14 == 7)
			do_ = false;

		if (do_)
			G::UserCmd->buttons |= IN_DUCK;
		else
			G::UserCmd->buttons &= ~IN_DUCK;

		cnt++;
	}
	else {
		do_ = false;
		cnt = 0;
		G::FakelagShouldLag = true;
	}
}

__forceinline void c_antiaims::yaw_stay_180() {
	this->yaw(180);
}

__forceinline void c_antiaims::yaw_stay_jitter() {
	static bool isminnow = false;
	this->yaw((c_config::get()->m["rage_antiaim_freestand"][0] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_custom"] + (isminnow ? c_config::get()->i["rage_antiaim_yaw_jitter_min"] : c_config::get()->i["rage_antiaim_yaw_jitter_max"]));
	isminnow = !isminnow;
}

__forceinline void c_antiaims::yaw_stay_static() {
	this->yaw_static((c_config::get()->m["rage_antiaim_freestand"][0] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_jitter_min"], c_config::get()->i["rage_antiaim_yaw_jitter_max"]));
}

__forceinline void c_antiaims::yaw_move_180() {

	this->yaw((c_config::get()->m["rage_antiaim_freestand"][0] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_move_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_move_jitter_min"], c_config::get()->i["rage_antiaim_yaw_move_jitter_max"]));
}

__forceinline void c_antiaims::yaw_move_jitter() {
	static bool isminnow = false;
	this->yaw((c_config::get()->m["rage_antiaim_freestand"][0] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_move_custom"] + (isminnow ? c_config::get()->i["rage_antiaim_yaw_move_jitter_min"] : c_config::get()->i["rage_antiaim_yaw_move_jitter_max"]));
	isminnow = !isminnow;
}

__forceinline void c_antiaims::yaw_move_static() {
	this->yaw_static((c_config::get()->b["rage_antiaim_freestand"] ? freestand() : 180) + c_config::get()->i["rage_antiaim_yaw_move_custom"] + Utilities->Game_RandomFloat(c_config::get()->i["rage_antiaim_yaw_move_jitter_min"], c_config::get()->i["rage_antiaim_yaw_move_jitter_max"]));
}

__forceinline void c_antiaims::pitch(float pitch) {
	G::UserCmd->viewangles.x = pitch;
}

__forceinline void c_antiaims::yaw(float yaw) {
		G::UserCmd->viewangles.y = yaw;
}

__forceinline void c_antiaims::yaw_static(float yaw) {
	G::UserCmd->viewangles.y = yaw;
}

__forceinline void c_antiaims::pitch_emotion() {
	if (c_config::get()->b["misc_antiuntrusted"])
		this->pitch(89);
	else
		this->pitch(-178);
}

__forceinline void c_antiaims::pitch_up() {
	if (c_config::get()->b["misc_antiuntrusted"])
		this->pitch(-89);
	else
		this->pitch(-178);
}

__forceinline void c_antiaims::pitch_minimal() {
	this->pitch(79);
}

__forceinline void c_antiaims::slowwalk() {
	if (!c_config::get()->b["rage_slowmotion"] || !c_keyhandler::get()->auto_check("rage_slowmotion_key"))
		return;

	CBaseWeapon* weapon_handle = G::LocalPlayer->GetActiveWeapon();

	if (!weapon_handle)
		return;

	float amount = 0.34f;

	Vector velocity = G::UnpredictedVelocity;
	Vector direction;

	Utilities->Math_VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.y = G::UserCmd->viewangles.y - direction.y;

	Vector forward;

	Utilities->Math_AngleVectors(direction, forward);

	Vector source = forward * -speed;

	if (speed > (weapon_handle->get_full_info()->max_speed * amount))
	{
		G::UserCmd->forwardmove = source.x;
		G::UserCmd->sidemove = source.y;

	}
}
inline float ClampYaw(float yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}
/*CBaseAnimState *m_serverAnimState = nullptr;
float_t m_flSpawnTime = 0.f;
bool m_bBreakLowerBody = false;
float_t m_flNextBodyUpdate = 0.f;
CBaseHandle *m_ulEntHandle = nullptr;
void UpdateLBYBreaker(CUserCmd *usercmd)
{
	bool
		allocate = (m_serverAnimState == nullptr),
		change = (!allocate) && (&G::LocalPlayer->GetRefEHandle() != m_ulEntHandle),
		reset = (!allocate && !change) && (G::LocalPlayer->m_flSpawnTime() != m_flSpawnTime);

	// player changed, free old animation state.
	if (change)
		Interfaces::MemAlloc->Free(m_serverAnimState);

	// need to reset? (on respawn)
	if (reset)
	{
		// reset animation state.
		CBaseEntity::ResetAnimationState(m_serverAnimState);

		// note new spawn time.
		m_flSpawnTime = G::LocalPlayer->m_flSpawnTime();
	}

	// need to allocate or create new due to player change.
	if (allocate || change)
	{
		// only works with games heap alloc.
		CBaseAnimState *state = (CBaseAnimState*)Interfaces::MemAlloc->Alloc(sizeof(CBaseAnimState));

		if (state != nullptr)
			G::LocalPlayer->CreateAnimationState(state);

		// used to detect if we need to recreate / reset.
		m_ulEntHandle = const_cast<CBaseHandle*>(&G::LocalPlayer->GetRefEHandle());
		m_flSpawnTime = G::LocalPlayer->m_flSpawnTime();

		// note anim state for future use.
		m_serverAnimState = state;
	}

	float_t curtime = TICKS_TO_TIME(get_curtime2(usercmd));
	if (!Interfaces::ClientState->m_nChokedCommands && m_serverAnimState)
	{
		CBaseEntity::UpdateAnimationState(m_serverAnimState, usercmd->viewangles);

		// calculate delta.
		float_t delta = std::abs(ClampYaw(usercmd->viewangles.y - G::LocalPlayer->GetLowerBodyYaw()));

		// walking, delay next update by .22s.
		if (m_serverAnimState->m_velocity > 0.1f && (G::LocalPlayer->GetFlags() & FL_ONGROUND))
			m_flNextBodyUpdate = curtime + 0.22f;

		else if (curtime >= m_flNextBodyUpdate)
		{
			if (delta > 35.f)
				; // server will update lby.

			m_flNextBodyUpdate = curtime + 1.1f;
		}
	}

	// if was jumping and then onground and bsendpacket true, we're gonna update.
	m_bBreakLowerBody = (G::LocalPlayer->GetFlags() & FL_ONGROUND) && ((m_flNextBodyUpdate - curtime) <= Interfaces::Globals->interval_per_tick);
}*/
float AngleNormalize(float angle)
{
	angle = fmodf(angle, 360.0f);
	if (angle > 180)
	{
		angle -= 360;
	}
	if (angle < -180)
	{
		angle += 360;
	}
	return angle;
}

/*static bool xuy = false;

__forceinline bool c_antiaims::desync() {
	static bool flip;
	auto InitFlip = []() {
		static clock_t start1_t1 = clock();
		double timeSoFar1 = (double)(clock() - start1_t1) / CLOCKS_PER_SEC;
		if (timeSoFar1 < 0.05)
			return;
		flip = !flip;
		start1_t1 = clock();
	};
	InitFlip();
	static bool switch_;
	if (c_keyhandler::get()->is_key_down(c_config::get()->i["rage_flip_key"])) {
		xuy = !xuy;

	}
	switch_ = !switch_;
	float yaw = 0;
	float desyncdelta = std::clamp(G::LocalPlayer->GetMaxDesyncDelta(), 0.f, float(c_config::get()->i["rage_antiaim_desync_limit"]));
	if (c_config::get()->b["rage_flip"])
		yaw = realAngles + (xuy ? desyncdelta : -desyncdelta);
	else
		yaw = realAngles + (c_config::get()->i["rage_antiaim_desync"] == 1 ? desyncdelta : -desyncdelta);

		/*if (c_config::get()->i["rage_antiaim_desync"] == 3) {
			float desync = G::LocalPlayer->GetMaxDesyncDelta();
			float balance = 1.0f;

			int type = 2;
			if (type == 2)
				balance = -1.0f;

			if (Interfaces::Globals->curtime <= next_lby_update(G::UserCmd->viewangles.y, G::UserCmd)) {

				auto net_channel = Interfaces::Engine->GetNetChannel();
				if (net_channel->m_nChokedPackets >= 2) {
					G::UserCmd->viewangles.y = NormalizeYaw(G::UserCmd->viewangles.y);
				}


				if (type == 1)
					G::UserCmd->viewangles.y -= 100.0f;
				else
					G::UserCmd->viewangles.y += (balance * 120.0f);
			}
			else if (type != 1) {
				G::UserCmd->viewangles.y -= (desync + 30.0f) * balance;
			}
		}

	Utilities->Math_NormalizeYaw(yaw);

	G::FakeAngle.y = yaw;
	G::UserCmd->viewangles.y = yaw;

	return c_config::get()->i["rage_antiaim_desync"] == 1 ? switch_ : c_config::get()->i["rage_antiaim_desync"] == 2;
	//// ÕÓÉÍßßßßßßßßß ÁÀÉ ÃÈÄÅÎÍÙÈÊ Ñ ÔËÈÏÎÌ ÁÀÉ ÌÈ
}

void CVisuals::desyncdraw() {
	CColor col2 = CColor(255, 255, 255, 255);
	CColor col = CColor(87, 255, 62, 255);
	int screen_w, screen_h;
	Interfaces::Engine->GetScreenSize(screen_w, screen_h);
	if (G::xuy) {
		Graphics::Drawing->Text(screen_w / 2 - 64, screen_h / 2 - 13, "*", Graphics::Drawing->fonts.esp_indicator, col2);
		Graphics::Drawing->Text(screen_w / 2 + 56, screen_h / 2 - 13, "*", Graphics::Drawing->fonts.esp_indicator, col);
	}
	else {
		Graphics::Drawing->Text(screen_w / 2 + 56, screen_h / 2 - 13, "*", Graphics::Drawing->fonts.esp_indicator, col2);
		Graphics::Drawing->Text(screen_w / 2 - 64, screen_h / 2 - 13, "*", Graphics::Drawing->fonts.esp_indicator, col);
	}
	///////// ÎÒÎÁÐÀÆÅÍÈÅ ÄÅÑÈÍÊÎÂ ÄÅËÀË ÑÀÌ ÒÀÊÎÅ ÑÅÁÅ
}*/

__forceinline float c_antiaims::freestand() {

		enum {
			back,
			right,
			left
		};

		Vector view_angles;
		Interfaces::Engine->GetViewAngles(&view_angles);

		static constexpr int damage_tolerance = 50;

		std::vector< CBaseEntity* > enemies;

		auto get_target = [&]() -> CBaseEntity* {
			CBaseEntity* target = nullptr;
			float best_fov = 360.f;

			for (int id = 1; id <= Interfaces::Globals->maxclients; id++) {
				auto e = (Interfaces::EntityList->GetClientEntity(id));

				if (!e->valid(true))
					continue;

				float fov = Cheat::Utilities->Math_GetFov(view_angles, Cheat::Utilities->Math_CalcAngle(G::LocalPlayer->GetEyePosition(), e->GetOrigin()));

				if (fov < best_fov) {
					target = e;
					best_fov = fov;
				}

				enemies.push_back(e);
			}

			return target;
		};

		CBaseEntity* e = get_target();

		if (!e)
			return G::UserCmd->viewangles.y + 180;

		auto calculate_damage = [&](Vector point) -> int {
			int damage = 0;
			for (auto& enemy : enemies)
				damage += c_autowall::get()->autowall(enemy->GetEyePosition(), point, enemy, G::LocalPlayer, 1).damage;

			return damage;
		};

		auto rotate_and_extend = [](Vector position, float yaw, float distance) -> Vector {
			Vector direction;
			Cheat::Utilities->Math_AngleVectors(Vector(0, yaw, 0), direction);

			return position + (direction * distance);
		};

		Vector
			head_position = G::LocalPlayer->GetEyePosition(),
			at_target = Cheat::Utilities->Math_CalcAngle(G::LocalPlayer->GetOrigin(), e->GetOrigin());

		float angles[3] = {
			at_target.y + 180,
			at_target.y + 180 - 10.f,
			at_target.y + 180 + 10.f,
		};

		Vector head_positions[3] = {
			rotate_and_extend(head_position, at_target.y + 180, 999),
			rotate_and_extend(head_position, at_target.y + 180 - 10.f, 999),
			rotate_and_extend(head_position, at_target.y + 180 + 10.f, 999)
		};

		int damages[3] = {
			calculate_damage(head_positions[back]),
			calculate_damage(head_positions[right]),
			calculate_damage(head_positions[left])
		};

		if (damages[right] == damages[left]) {
			side = false;
			auto trace_to_end = [](Vector start, Vector end) -> Vector {
				trace_t trace;
				CTraceWorldOnly filter;
				Ray_t ray;

				ray.Init(start, end);
				Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

				return trace.end;
			};

			Vector
				trace_right_endpos = trace_to_end(head_position, head_positions[right]),
				trace_left_endpos = trace_to_end(head_position, head_positions[left]);

			Ray_t ray;
			trace_t trace;
			CTraceWorldOnly filter;

			ray.Init(trace_right_endpos, e->GetEyePosition());
			Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
			float distance_1 = (trace.start - trace.end).Length();

			ray.Init(trace_left_endpos, e->GetEyePosition());
			Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);
			float distance_2 = (trace.start - trace.end).Length();

			return angles[back];

		}
}