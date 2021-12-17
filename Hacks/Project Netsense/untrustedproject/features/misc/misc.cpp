#include "misc.hpp"
#include "../../options.hpp"
#include "../../addons/ui_addons/ui_addons.hpp"
#include "../../config.hpp"
#include "../../globals.hpp"
#include "../../render.hpp"
#include "../../helpers/input.hpp"
#include "DamageIndicator.hpp"
#include "grenade_prediction.hpp"
#include "Nightmode.hpp"
#include "MolotovTimer.hpp"
#include "SpecList.hpp"
#include "LegitAA.hpp"
#include "blockbot.hpp"
#include "../event_logger/logger.hpp"

// public
void c_misc::create_move(CUserCmd* cmd, bool* bSendPacket) {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	if (options.autostrafe && options.strafing) autostrafe(cmd);
	if (options.bhop) bhop(cmd);
	if (options.knife_bot) knife_bot(cmd, bSendPacket);
	if (options.grenade_prediction) nade_pred.trace(cmd);
	if (options.slow_walk) slow_walk(cmd);
	if (options.alpha_modulate != 255.f) g_LocalPlayer->m_flFlashMaxAlpha() = options.alpha_modulate;
	if (options.rank_reveal && cmd->buttons & IN_SCORE) g_CHLClient->DispatchUserMessage(CS_UM_ServerRankRevealAll, 0, 0, nullptr);
	if (options.block_botting) g_BlockBot->cmove(cmd);
	if (options.infinite_duck) cmd->buttons |= IN_BULLRUSH;
}

void c_misc::create_move_prediction(CUserCmd* cmd, bool* bSendPacket) {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	if (options.legit_aa) g_LegitAA->CreateMove(cmd, bSendPacket);
	this->jump_bug(cmd);
}

void c_misc::velocity_ind()
{
	int w2, h2;
	g_EngineClient->GetScreenSize(w2, h2);
	float velocity = g_LocalPlayer->m_vecVelocity().Length2D() + 0.5;
	static float groundvelocity = 0;
	if (options.velocity)
	{
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND) {
			groundvelocity = g_LocalPlayer->m_vecVelocity().Length2D() + 0.5;


			Render::Get().RenderText(std::to_string( (int)velocity) + " - Velocity info [ground]", ImVec2(w2 / 2, h2 - h2 / 5 - 15), 15, Color(255 ,51 ,0), true);
		}
		else
			Render::Get().RenderText(std::to_string((int)velocity) + " (" + std::to_string((int)groundvelocity) + ")" + " - Velocity info [air]", ImVec2(w2 / 2, h2 - h2 / 5 - 15), 15, Color(255, 51, 0), true);
	}
}

void c_misc::paint() {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	if (options.show_fov) this->show_fov();
	if (options.damage_indicator) DamageIndicators::Get().paint();
	if (options.grenade_prediction) nade_pred.draw();
	if (options.aa_arrows) g_LegitAA->DrawArrows();
	if (options.molotov_timer) g_MolotovTimer.Draw();
	if (g_Misc->options.block_botting) g_BlockBot->draw();
	if (options.velocity) velocity_ind();
	notify::render();
}

void c_misc::end_scene() {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	if (options.show_spread) this->show_spread();
	if (CheatGVars::UpdateNightMode) {
		NightMode::EndScene(); CheatGVars::UpdateNightMode = false;
	}
}

void c_misc::fire_event(IGameEvent* pEvent) {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	if (options.damage_indicator) DamageIndicators::Get().fireEvent(pEvent);
}

void c_misc::override_view() {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	this->ThirdPerson();
}

void c_misc::frame_stage(ClientFrameStage_t stage) {
	if (!g_EngineClient) return;
	if (!g_LocalPlayer) return;
	if (!g_EngineClient->IsInGame()) return;
	if (!g_EngineClient->IsConnected()) return;
	if (!g_LocalPlayer->IsAlive()) return;

	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	float aspect = (w / h);
	static auto aspect_ratio = g_CVar->FindVar("r_aspectratio");
	float ratio = (options.misc_aspect_ratio / 2);
	if (options.misc_aspect_ratio > 0 || options.misc_aspect_ratio < 1)
		aspect_ratio->SetValue(ratio);
	else
		aspect_ratio->SetValue(0);
	CustomFov();
}

// private
void c_misc::bhop(CUserCmd* cmd) {
	static bool jumped_last_tick = false;
	static bool should_fake_jump = false;

	if (g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType() == MOVETYPE_LADDER) return;

	if (cmd->buttons & IN_JUMP && !(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		cmd->buttons &= ~IN_JUMP;
	else if (rand() % 100 > options.bhop_chance)
		cmd->buttons &= ~IN_JUMP;
}
// ---
void c_misc::autostrafe(CUserCmd* pCmd) {
	static bool is_bhopping;
	static float calculated_direction;
	static bool in_transition;
	static float true_direction;
	static float wish_direction;
	static float step;
	static float rough_direction;

	if (!g_LocalPlayer) return;


	enum directions
	{
		FORWARDS = 0,
		BACKWARDS = 180,
		LEFT = 90,
		RIGHT = -90
	};

	// Reset direction when player is not strafing
	is_bhopping = pCmd->buttons & IN_JUMP;
	if (!is_bhopping && g_LocalPlayer->m_fFlags() & FL_ONGROUND){
		calculated_direction = directions::FORWARDS;
		in_transition = false;
		return;
	}

	// Get true view angles
	QAngle base{ };
	g_EngineClient->GetViewAngles(&base);

	// Calculate the rough direction closest to the player's true direction
	auto get_rough_direction = [&](float true_direction) -> float
	{
		// Make array with our four rough directions
		std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
		float best_angle, best_delta = 181.f;

		// Loop through our rough directions and find which one is closest to our true direction
		for (size_t i = 0; i < minimum.size(); ++i)
		{
			float rough_direction = base.yaw + minimum.at(i);
			float delta = fabsf(Math::NormalizeYaw(true_direction - rough_direction));

			// Only the smallest delta wins out
			if (delta < best_delta)
			{
				best_angle = rough_direction;
				best_delta = delta;
			}
		}

		return best_angle;
	};

	// Get true direction based on player velocity
	true_direction = g_LocalPlayer->m_vecVelocity().Angle().y;

	// Detect wish direction based on movement keypresses
	if (pCmd->buttons & IN_FORWARD)
	{
		wish_direction = base.yaw + directions::FORWARDS;
	}
	else if (pCmd->buttons & IN_BACK)
	{
		wish_direction = base.yaw + directions::BACKWARDS;
	}
	else if (pCmd->buttons & IN_MOVELEFT)
	{
		wish_direction = base.yaw + directions::LEFT;
	}
	else if (pCmd->buttons & IN_MOVERIGHT)
	{
		wish_direction = base.yaw + directions::RIGHT;
	}
	else
	{
		// Reset to forward when not pressing a movement key, then fix anti-aim strafing by setting IN_FORWARD
		pCmd->buttons |= IN_FORWARD;
		wish_direction = base.yaw + directions::FORWARDS;
	}
	// Calculate the ideal strafe rotation based on player speed (c) navewindre
	float speed_rotation = min(RAD2DEG(std::asinf(30.f / g_LocalPlayer->m_vecVelocity().Length2D())) * 0.5f, 45.f);
	if (in_transition)
	{
		// Get value to rotate by via calculated speed rotation
		float ideal_step = speed_rotation + calculated_direction;
		step = fabsf(Math::NormalizeYaw(calculated_direction - ideal_step)); // 15.f is a good alternative, but here is where you would do your "speed" slider value for the autostrafer

		// Check when the calculated direction arrives close to the wish direction
		if (fabsf(Math::NormalizeYaw(wish_direction - calculated_direction)) > step)
		{
			float add = Math::NormalizeYaw(calculated_direction + step);
			float sub = Math::NormalizeYaw(calculated_direction - step);

			// Step in direction that gets us closer to our wish direction
			if (fabsf(Math::NormalizeYaw(wish_direction - add)) >= fabsf(Math::NormalizeYaw(wish_direction - sub)))
			{
				calculated_direction -= step;
			}
			else
			{
				calculated_direction += step;
			}
		}
		else
		{
			// Stop transitioning when we meet our wish direction
			in_transition = false;
		}
	}
	else
	{
		// Get rough direction and setup calculated direction only when not transitioning
		rough_direction = get_rough_direction(true_direction);
		calculated_direction = rough_direction;

		// When we have a difference between our current (rough) direction and our wish direction, then transition
		if (rough_direction != wish_direction)
		{
			in_transition = true;
		}
	}

	// Set movement up to be rotated
	pCmd->forwardmove = 0.f;
	pCmd->sidemove = pCmd->command_number % 2 ? 450.f : -450.f;

	// Calculate ideal rotation based on our newly calculated direction
	float direction = (pCmd->command_number % 2 ? speed_rotation : -speed_rotation) + calculated_direction;

	// Rotate our direction based on our new, defininite direction
	float rotation = DEG2RAD(base.yaw - direction);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	// Apply newly rotated movement
	pCmd->forwardmove = forwardmove;
	pCmd->sidemove = sidemove;
}
// ---
void c_misc::jump_bug(CUserCmd* cmd) {
	float max_radias = M_PI * 2;
	float step = max_radias / 128;
	float xThick = 23;
	auto unduck = true;
	bool bDidJump;
	static bool needCHangeBhop = false;
	if (options.jump_bugging && (g_LocalPlayer->m_fFlags() & (1 << 0))) {
		if (options.bhop) {
			options.bhop = false;
			needCHangeBhop = true;
		}
		int screenWidth, screenHeight;
		g_EngineClient->GetScreenSize(screenWidth, screenHeight);

		if (unduck) {
			bool bDidJump = false;
			cmd->buttons &= ~IN_DUCK; // duck
			cmd->buttons |= IN_JUMP; // jump
			unduck = false;
		}
		Vector pos = g_LocalPlayer->m_vecOrigin();
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = (xThick * cos(a)) + pos.x;
			pt.y = (xThick * sin(a)) + pos.y;
			pt.z = pos.z;


			Vector pt2 = pt;
			pt2.z -= 6;

			trace_t fag;

			Ray_t ray;
			ray.Init(pt, pt2);

			CTraceFilter flt;
			flt.pSkip = g_LocalPlayer;
			g_EngineTrace->TraceRay(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				bDidJump = true;
				cmd->buttons |= IN_DUCK;
				cmd->buttons &= ~IN_JUMP;
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 6;

			trace_t fag;

			Ray_t ray;
			ray.Init(pt, pt2);

			CTraceFilter flt;
			flt.pSkip = g_LocalPlayer;
			g_EngineTrace->TraceRay(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				bDidJump = true;
				cmd->buttons |= IN_DUCK; // duck
				cmd->buttons &= ~IN_JUMP; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			Vector pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 6;

			trace_t fag;

			Ray_t ray;
			ray.Init(pt, pt2);

			CTraceFilter flt;
			flt.pSkip = g_LocalPlayer;
			g_EngineTrace->TraceRay(ray, MASK_SOLID_BRUSHONLY, &flt, &fag);

			if (fag.fraction != 1.f && fag.fraction != 0.f) {
				bDidJump = true;
				cmd->buttons |= IN_DUCK; // duck
				cmd->buttons &= ~IN_JUMP; // jump
				unduck = true;
			}
		}
	}
	else if(needCHangeBhop)  {
		options.bhop = true;
		needCHangeBhop = false;
	}
}
// --




void c_misc::show_fov() {
	static int fov, silent_fov;
	static bool silent_enable = false;

	float fov_fix = CheatGVars::OFOV >= 90.f ? (CheatGVars::SHeight / (80.f * CheatGVars::OFOV / 90.f) / (CheatGVars::OFOV / 90.f)) : (CheatGVars::SHeight / (80.f * CheatGVars::OFOV / 90.f) / (CheatGVars::OFOV / 80.f) * (CheatGVars::OFOV / 80.f));
	
	if (!g_LocalPlayer)
		return;

	auto wpn = g_LocalPlayer->m_hActiveWeapon();
	if (!wpn)
		return;

	short weapon = wpn.Get()->m_Item().m_iItemDefinitionIndex();

	auto setts = *g_CustomWeaponGroups->GetSettings(weapon);

	fov = setts.fov * fov_fix;
	silent_fov = setts.silent_fov * fov_fix;

	silent_enable = setts.silent || setts.psilent;

	Render::Get().RenderCircle(CheatGVars::SWidthHalf, CheatGVars::SHeightHalf, fov, 50, colors.fov_default, 0.3f);
	if (silent_enable) 
		Render::Get().RenderCircle(CheatGVars::SWidthHalf, CheatGVars::SHeightHalf, silent_fov, 50, colors.fov_silent, 0.3f);
}
// ---
void c_misc::CustomFov()
{
	static auto customX = g_CVar->FindVar("viewmodel_offset_x");
	customX->m_fnChangeCallbacks.m_Size = 0;
	customX->SetValue(options.customx);

	static auto customY = g_CVar->FindVar("viewmodel_offset_y");
	customY->m_fnChangeCallbacks.m_Size = 0;
	customY->SetValue(options.customy);

	static auto customZ = g_CVar->FindVar("viewmodel_offset_z");
	customZ->m_fnChangeCallbacks.m_Size = 0;
	customZ->SetValue(options.customz);
}
// ---
void c_misc::show_spread() {

	auto weapon = g_LocalPlayer->m_hActiveWeapon();

	if (!weapon) return;

	static float rot = 0.f;

	
	switch (options.show_spread_type) {
	case 0:
		Render::Get().RenderCircleDualColor(CheatGVars::SWidthHalf, CheatGVars::SHeightHalf, weapon->GetInaccuracy() * 500.0f, 0, 1, 50, D3DCOLOR_RGBA(colors.spread_color.r(), colors.spread_color.g(), colors.spread_color.b(), 100), D3DCOLOR_RGBA(0, 0, 0, 0));
		break;
	case 1:
		Render::Get().RenderCircleDualColor(CheatGVars::SWidthHalf, CheatGVars::SHeightHalf, weapon->GetInaccuracy() * 500.0f, rot, 1, 50);
		break;
	}

	rot += 0.5f;
	if (rot > 360.f)
		rot = 0.f;
}
// ---

void c_misc::BuyInfo()
{
	ImGui::Begin("BuyInfo", &options.autob, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::SetWindowSize(ImVec2(175, 100));

		//if (g_Options.BuySSG) //сука, сюда пишем какое оружие вы покупаете из байбота
		//{
			ImGui::Text("SSG-08");
		//}
		//if (g_Options.BuyDuals)
		//{
			ImGui::Text("Duals");
		//}
		//if (g_Options.BuyArmour)
		//{
			ImGui::Text("Armour");
		//}

	}
	ImGui::End();
}
// ---
void c_misc::knife_bot(CUserCmd* pCmd, bool* bSendPacket) {
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive() || !options.knife_bot) return;
	if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2) return;

	C_BaseCombatWeapon* pLocalWeapon = g_LocalPlayer->m_hActiveWeapon();
	if (!pLocalWeapon->GetCSWeaponData()) return;
	int wType = pLocalWeapon->GetCSWeaponData()->iWeaponType;
	if (pLocalWeapon && wType == 1) {
		for (int EntIndex = 1; EntIndex < g_GlobalVars->maxClients; EntIndex++)
		{
			C_BasePlayer* Entity = C_BasePlayer::GetPlayerByIndex(EntIndex);
			
			if (!Entity) 
				continue;

			if (Entity == g_LocalPlayer)
				continue;

			if (!(EntIndex > 0 && EntIndex < 65))
				continue;

			if (!Entity->IsAlive())
				continue;

			float Distance = Entity->GetEyePos().DistTo(g_LocalPlayer->GetEyePos());

			if (Distance > options.knife_bot_dist)
				continue;

			if (options.knife_bot_filter == 1) {
				if (Entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
					continue;
			}
			else if (options.knife_bot_filter == 2) {
				if (Entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
					continue;
			}

			Vector OrignWorld = Entity->GetRenderOrigin();
			Vector OrignScreen;

			if (!Math::WorldToScreen(OrignWorld, OrignScreen))
				continue;

			switch (options.knife_bot_mode)
			{
			case 0:
			{
				static int cur_attack = 0;

				if (Distance > 60.f)
					pCmd->buttons |= IN_ATTACK;
				else
				{
					if ((g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick) - pLocalWeapon->m_flNextPrimaryAttack() > 0) {
						if (Entity->m_ArmorValue() > 0) {
							if (Entity->m_iHealth() - 21 <= 0)
								pCmd->buttons |= IN_ATTACK;
							else
								pCmd->buttons |= IN_ATTACK2;
						}
						else
						{
							if (Entity->m_iHealth() - 34 <= 0)
								pCmd->buttons |= IN_ATTACK;
							else
								pCmd->buttons |= IN_ATTACK2;
						}
					}

				}
				break;
			}
			case 1:
			{
				static int cur_attack = 0;

				if (Distance > 60.f)
				{
					pCmd->buttons |= IN_ATTACK;
					cur_attack = 1;
				}
				else
				{
					float TimeSinceFire = (g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick) - pLocalWeapon->m_flNextPrimaryAttack();

					if (cur_attack > 0 && TimeSinceFire > 0)
					{
						pCmd->buttons |= IN_ATTACK2;
						TimeSinceFire = -1;
						cur_attack = 0;
					}

					if (cur_attack == 0 && TimeSinceFire > 0)
					{
						pCmd->buttons |= IN_ATTACK;
						cur_attack++;
					}
				}
				break;
			}
			case 2: pCmd->buttons |= IN_ATTACK2; break;
			case 3: pCmd->buttons |= IN_ATTACK; break;
			default: break;
			}
		}
	}
}
// ---
void c_misc::slow_walk(CUserCmd* pCmd) {
	if (!options.slow_walk && !CheatGVars::SlowWalking && !g_InputSystem->IsButtonDown(KEY_LSHIFT)) {
		return;
	}

	

		auto weapon_handle = g_LocalPlayer->m_hActiveWeapon();

		if (!weapon_handle)
			return;

		float amount = 0.0034f * options.slow_walk_speed; // options.misc.slow_walk_amount has 100 max value

		Vector velocity = g_LocalPlayer->m_vecVelocity();
		QAngle direction;

		Math::VectorAngles(velocity, direction);

		float speed = velocity.Length2D();

		direction.yaw = pCmd->viewangles.yaw - direction.yaw;

		Vector forward;

		Math::AngleVectors(direction, forward);

		Vector source = forward * -speed;

		if (speed >= (weapon_handle->GetCSWeaponData()->flSpread * amount)) {
			pCmd->forwardmove = source.x;
			pCmd->sidemove = source.y;
		}
	
}
// ---


/*void c_misc::ThirdPerson() {
	if (!g_LocalPlayer)
		return;

	static auto in_thirdperson = false;

	static bool down = false;
	static bool clicked = false;

	if (options.third_person) {
		clicked = false;
		down = true;
	}
	else if (!options.third_person && down) {
		clicked = true;
		down = false;
	}
	else {
		clicked = false;
		down = false;
	}

	if (clicked) in_thirdperson = !in_thirdperson;

	if (in_thirdperson && g_LocalPlayer->IsAlive()) {
		if (!g_Input->m_fCameraInThirdPerson) {
			g_Input->m_fCameraInThirdPerson = true;
		}

		float dist = options.third_person_dist;

		QAngle* view = g_LocalPlayer->GetVAngles();
		trace_t tr;
		Ray_t ray;

		Vector desiredCamOffset = Vector(cos(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(-view->pitch)) * dist
		);

		//cast a ray from the Current camera Origin to the Desired 3rd person Camera origin
		ray.Init(g_LocalPlayer->GetEyePos(), (g_LocalPlayer->GetEyePos() - desiredCamOffset));
		CTraceFilter traceFilter;
		traceFilter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		Vector diff = g_LocalPlayer->GetEyePos() - tr.endpos;

		float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));// Pythagorean

		bool horOK = distance2D > (dist - 2.0f);
		bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

		float cameraDistance;

		if (horOK && vertOK)  // If we are clear of obstacles
		{
			cameraDistance = dist; // go ahead and set the distance to the setting
		}
		else {
			if (vertOK) // if the Vertical Axis is OK
			{
				//percent = std::clamp(percent + g_GlobalVars->frametime * 3.f, 0.3f, 1.f);
				cameraDistance = distance2D * 0.95f;
			}
			else// otherwise we need to move closer to not go into the floor/ceiling
			{
				//percent = std::clamp(percent + g_GlobalVars->frametime * 3.f, 0.3f, 1.f);
				cameraDistance = abs(diff.z) * 0.95f;
			}
		}
		g_Input->m_fCameraInThirdPerson = true;

		g_Input->m_vecCameraOffset.z = cameraDistance;
	}
	else {
		g_Input->m_fCameraInThirdPerson = false;
	}
}*/


void c_misc::ThirdPerson() {
	if (!g_LocalPlayer || !g_EngineTrace || !g_Input)
		return;

	static auto in_thirdperson = false;

	static bool down = false;
	static bool clicked = false;

	if (options.third_person) {
		clicked = false;
		down = true;
	} else if (!options.third_person && down) {
		clicked = true;
		down = false;
	} else {
		clicked = false;
		down = false;
	}

	if (clicked) in_thirdperson = !in_thirdperson;

	static const auto cam_idealdist = g_CVar->FindVar("cam_idealdist");
	static const auto cam_collision = g_CVar->FindVar("cam_collision");

	if (!g_EngineClient->IsInGame() || !g_LocalPlayer) return;

	static auto percent = 0.f;

	if (g_LocalPlayer->IsAlive() && in_thirdperson) {
		g_Input->m_fCameraInThirdPerson = true;

		percent = std::clamp(percent + g_GlobalVars->frametime * 3.f, 0.3f, 1.f);

		cam_idealdist->SetValue(options.third_person_dist * percent);
		cam_collision->SetValue(1);
	}
	else if (g_Input->m_fCameraInThirdPerson) {
		percent = std::clamp(percent - g_GlobalVars->frametime * 3.f, 0.3f, 1.f);

		cam_idealdist->SetValue(options.third_person_dist * percent);
		cam_collision->SetValue(0);
		if (percent <= 0.3f) {
			g_Input->m_fCameraInThirdPerson = false;
		}
	}

}
// ---
void c_misc::SetupValues() {
	g_Config->PushItem(&options.bhop, "misc", "bhop", options.bhop);
	g_Config->PushItem(&options.bhop, "misc", "autobuy", options.autob);
	g_Config->PushItem(&options.bhop, "misc", "MenuColor", options.cMenuCol);
	g_Config->PushItem(&options.bhop_chance, "misc", "bhop_chance", options.bhop_chance);
	g_Config->PushItem(&options.nosmoke, "misc", "nosmoke", options.nosmoke);
	g_Config->PushItem(&options.autostrafe, "misc", "autostrafe", options.autostrafe);
	g_Config->PushItem(&options.show_fov, "misc", "show_fov", options.show_fov);
	g_Config->PushItem(&options.show_spread, "misc", "show_spread", options.show_spread);
	g_Config->PushItem(&options.show_spread_type, "misc", "show_spread_type", options.show_spread_type);
	g_Config->PushItem(&options.knife_bot, "misc", "knife_bot", options.knife_bot);
	g_Config->PushItem(&options.customize_viewmodel, "misc", "customize_viewmodel", options.customize_viewmodel);
	g_Config->PushItem(&options.customx, "misc", "customx", options.customx);
	g_Config->PushItem(&options.customy, "misc", "customy", options.customy);
	g_Config->PushItem(&options.customz, "misc", "customz", options.customz);
	g_Config->PushItem(&options.knife_bot_dist, "misc", "knife_bot_dist", options.knife_bot_dist);
	g_Config->PushItem(&options.knife_bot_filter, "misc", "knife_bot_filter", options.knife_bot_filter);
	g_Config->PushItem(&options.knife_bot_mode, "misc", "knife_bot_mode", options.knife_bot_mode);
	g_Config->PushItem(&options.velocity, "misc", "velocity_ind", options.velocity);
	g_Config->PushItem(&options.misc_aspect_ratio, "misc", "misc_aspect_ratio", options.misc_aspect_ratio);
	g_Config->PushItem(&options.edge_jump, "misc", "edge_jump", options.edge_jump);
	g_Config->PushItem(&options.edge_jump_key, "misc", "edge_jump_key", options.edge_jump_key);
	g_Config->PushItem(&options.damage_indicator, "misc", "damage_indicator", options.damage_indicator);
	g_Config->PushItem(&options.grenade_prediction, "misc", "grenade_prediction", options.grenade_prediction);
	g_Config->PushItem(&options.no_visual_recoil, "misc", "no_visual_recoil", options.no_visual_recoil);
	g_Config->PushItem(&options.autostrafe_key, "misc", "autostrafe_key", options.autostrafe_key);
	g_Config->PushItem(&options.slow_walk, "misc", "slow_walk", options.slow_walk);
	g_Config->PushItem(&options.slow_walk_bind, "misc", "slow_walk_bind", options.slow_walk_bind);
	g_Config->PushItem(&options.slow_walk_speed, "misc", "slow_walk_speed", options.slow_walk_speed);
	g_Config->PushItem(&options.molotov_timer, "misc", "molotov_timer", options.molotov_timer);
	g_Config->PushItem(&options.third_person_bind, "misc", "ThirdPerson", options.third_person_bind);
	g_Config->PushItem(&options.third_person_dist, "misc", "third_person_dist", options.third_person_dist);
	g_Config->PushItem(&options.fakelag_limit, "misc", "fakelag_limit", options.fakelag_limit);
	g_Config->PushItem(&options.third_person, "misc", "third_person", options.third_person);
	g_Config->PushItem(&options.legit_aa, "misc", "legit_aa", options.legit_aa);
	g_Config->PushItem(&options.aa_arrows, "misc", "aa_arrows", options.aa_arrows);
	g_Config->PushItem(&options.RageAimbotHead, "misc", "rage_aimbot_head", options.RageAimbotHead);
	g_Config->PushItem(&options.RageAimbotBody, "misc", "rage_aimbot_body", options.RageAimbotBody);
	g_Config->PushItem(&options.RageAimbotLegs, "misc", "rage_aimbot_legs", options.RageAimbotLegs);
	g_Config->PushItem(&options.RageAimbotToes, "misc", "rage_aimbot_head", options.RageAimbotToes);
	g_Config->PushItem(&options.RageAimbotSafePoint, "misc", "rage_aimbot_safepoint", options.RageAimbotSafePoint);
	g_Config->PushItem(&options.RageAimbotDelayShot, "misc", "rage_aimbot_delayshot", options.RageAimbotDelayShot);
	g_Config->PushItem(&options.RageAntiaimEnabled, "misc", "rage_antiaim_enable", options.RageAntiaimEnabled);
	g_Config->PushItem(&options.RageAimbotMinDmg, "misc", "rage_aimbot_mindamage", options.RageAimbotMinDmg);
	g_Config->PushItem(&options.RageAimbotHitchance, "misc", "rage_aimbot_hitchance", options.RageAimbotHitchance);
	g_Config->PushItem(&options.rage_enable, "misc", "rage_enable", options.rage_enable);
	g_Config->PushItem(&options.rage_silent, "misc", "rage_silent", options.rage_silent);
	g_Config->PushItem(&options.RageAimbotResolver, "misc", "rage_aimbot_resolver", options.RageAimbotResolver);
	g_Config->PushItem(&options.RageAimbotBaimAfter, "misc", "rage_aimbot_baim_after", options.RageAimbotBaimAfter);
	g_Config->PushItem(&options.RageAimbotHeadScale, "misc", "rage_aimbot_headscale", options.RageAimbotHeadScale);
	g_Config->PushItem(&options.RageAimbotBodyScale, "misc", "rage_aimbot_body_scale", options.RageAimbotBodyScale);
	g_Config->PushItem(&options.legit_aa_mode, "misc", "legit_aa_mode", options.legit_aa_mode);
	g_Config->PushItem(&options.aa_arrows_length, "misc", "aa_arrows_length", options.aa_arrows_length);
	g_Config->PushItem(&options.legit_aa_filp_key, "misc", "legit_aa_filp_key", options.legit_aa_filp_key);
	g_Config->PushItem(&options.resolver_enabled, "misc", "resolver_enabled", options.resolver_enabled);
	g_Config->PushItem(&options.resolver_mode, "misc", "resolver_mode", options.resolver_mode);
	g_Config->PushItem(&colors.spread_color, "misc", "spread_color", colors.spread_color);
	g_Config->PushItem(&colors.accent_color, "misc", "accent_color", colors.accent_color);
	g_Config->PushItem(&colors.fov_default, "misc", "fov_default", colors.fov_default);
	g_Config->PushItem(&colors.fov_silent, "misc", "fov_silent", colors.fov_silent);
	g_Config->PushItem(&colors.damage_indicator_color, "misc", "damage_indicator_color", colors.damage_indicator_color);
	g_Config->PushItem(&colors.grenade_prediction_color, "misc", "grenade_prediction_color", colors.grenade_prediction_color);
	g_Config->PushItem(&colors.molotov_timer_color, "misc", "molotov_timer_color", colors.molotov_timer_color);
	g_Config->PushItem(&options.alpha_modulate, "misc", "alpha_modulate", options.alpha_modulate);
	g_Config->PushItem(&options.no_zoom, "misc", "no_zoom", options.no_zoom);
	g_Config->PushItem(&options.view_fov, "misc", "view_fov", options.view_fov);
	g_Config->PushItem(&options.view_fov_changer, "misc", "view_fov_changer", options.view_fov_changer);
	g_Config->PushItem(&options.rank_reveal, "misc", "rank_reveal", options.rank_reveal);
	g_Config->PushItem(&options.show_crosshair, "misc", "show_crosshair", options.show_crosshair);
	g_Config->PushItem(&options.radar, "misc", "radar", options.radar);
	g_Config->PushItem(&options.blockbot_key, "misc", "blockbot_key", options.blockbot_key);
	g_Config->PushItem(&options.chams_backtrack_all_ticks, "misc", "chams_backtrack_all_ticks", options.chams_backtrack_all_ticks);
	g_Config->PushItem(&options.eventlogger_hurt, "eventlogger", "hurt", options.eventlogger_hurt);
	g_Config->PushItem(&options.eventlogger_player_buy, "eventlogger", "player_buy", options.eventlogger_player_buy);
	g_Config->PushItem(&options.eventlogger_planting, "eventlogger", "planting", options.eventlogger_planting);
	g_Config->PushItem(&options.eventlogger_defusing, "eventlogger", "defusing", options.eventlogger_defusing);
	g_Config->PushItem(&options.nightmode_value, "nightmode", "value", options.nightmode_value);
	g_Config->PushItem(&options.infinite_duck, "misc", "infinite_duck", options.infinite_duck);
	g_Config->PushItem(&options.jump_bug_key, "jump_bug", "key", options.jump_bug_key);
	g_Config->PushItem(&options.show_keybinds, "misc", "show_keybinds", options.show_keybinds);
	
}
Menu a;
// ---
void c_misc::Menu() {
	ImGui::Columns(3, nullptr, false);

	int menur, menug, menub, menua;
	menur = g_Misc->options.cMenuCol[0] * 255;
	menug = g_Misc->options.cMenuCol[1] * 255;
	menub = g_Misc->options.cMenuCol[2] * 255;
	menua = g_Misc->options.cMenuCol[3] * 255;

	ImGui::BeginChild("##misc", ImVec2(-1, -1), true); {
		ImGui::TextColored(ImColor(menur, menug, menub), "Misc");
		ImGui::Checkbox("Bunnyhop", &options.bhop);
		ImGui::Checkbox("Show Auto Buy", &options.autob);
		ImGui::Checkbox("Autostrafe", &options.autostrafe);
		ImGui::Keybind("autostrafe", &options.autostrafe_key);
		ImGui::Checkbox("Watermark", &g_WaterMark->params.enabled);
		ImGui::Checkbox("Velocity Indicator", &options.velocity);
		ImGui::Checkbox("Show FOV", &options.show_fov);
		ImGui::Checkbox("Show Spread", &options.show_spread);
		ImGui::Checkbox("Knife Bot", &options.knife_bot);
		ImGui::Checkbox("Edge Jump", &options.edge_jump);
		ImGui::Keybind("ejump", &options.edge_jump_key);
		ImGui::Checkbox("Damage Indicator", &options.damage_indicator);
		ImGui::Checkbox("Custom Viewmodel", &options.customize_viewmodel);
		ImGui::Checkbox("Grenade Prediction", &options.grenade_prediction);
		ImGui::Checkbox("No Visual Recoil", &options.no_visual_recoil);
		ImGui::Checkbox("No Smoke", &options.nosmoke);
		ImGui::Checkbox("Slow Walk", &options.slow_walk);
		ImGui::Keybind("sw", &options.slow_walk_bind);
		ImGui::Checkbox("Molotov Timer", &options.molotov_timer);
		ImGui::Checkbox("Spectator List", &g_SpecList->params.enabled);
		ImGui::Checkbox("View Fov Changer", &options.view_fov_changer);
		ImGui::Checkbox("No Zoom", &options.no_zoom);
		ImGui::Checkbox("Rank Reveal", &options.rank_reveal);
		ImGui::Checkbox("Legit AA", &options.legit_aa);
		ImGui::Keybind("legitaaflip", &options.legit_aa_filp_key);
		ImGui::Checkbox("Thirdperson", &options.third_person);
		/*if (options.third_person)
		{*/
			ImGui::Keybind("3rdpersbind", &options.third_person_bind);

		//}


		
		//ImGui::Checkbox("Resolver", &options.resolver_enabled);
		ImGui::Checkbox("Crosshair", &options.show_crosshair);
		ImGui::Checkbox("Inifinite Duck", &options.infinite_duck);
		ImGui::Checkbox("Show keybinds", &options.show_keybinds);
		if (ImGui::Checkbox("Radar", &options.radar)) {
			g_CVar->FindVar("mp_radar_showall")->SetValue(options.radar ? 1 : 0);
		}
		ImGui::Text("Block Bot");
		ImGui::Keybind("bbkey", &options.blockbot_key);
		ImGui::Text("Jump Bug");
		ImGui::Keybind("jbkey", &options.jump_bug_key);
		if (ImGui::Button("Reset Size", ImVec2(-1, 19))) {
			g_ChangeMenuSizeValue = ImVec2(900, 430);
			g_ChangeMenuSize = true;
		}
	};
	ImGui::EndChild();
	ImGui::NextColumn();

	

	ImGui::BeginChild("##miscoptions", ImVec2(-1, -1), true); {
		//ImGui::Text("Options");
		ImGui::TextColored(ImColor(menur, menug, menub), "Options & Colors");
		bool* events_screen_b[4]{ &options.eventlogger_hurt, &options.eventlogger_player_buy, &options.eventlogger_planting, &options.eventlogger_defusing };
		if (ImGui::BeginCombo("Logger", "Select")) {
			
			ImGui::Selectable("Hurt", &options.eventlogger_hurt, ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable("Buy", &options.eventlogger_player_buy, ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable("Planting", &options.eventlogger_planting, ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable("Defusing", &options.eventlogger_defusing, ImGuiSelectableFlags_DontClosePopups);

			ImGui::EndCombo();
		}
		if (options.bhop && ImGui::SliderInt("Bunnyhop Chance", &options.bhop_chance, 1, 100))
			options.bhop_chance = std::clamp(options.bhop_chance, 1, 100);
		if (options.show_spread)
			ImGui::Combo("Spread type", &options.show_spread_type, values::show_spread_types, 2);
		if (options.knife_bot) {
			if (ImGui::SliderInt("Knifebot Distance", &options.knife_bot_dist, 1, 100))
				options.knife_bot_dist = std::clamp(options.knife_bot_dist, 1, 100);
			ImGui::Combo("Knifebot Filter", &options.knife_bot_filter, values::knifebot_filters, 3);
			ImGui::Combo("Knifebot Mode", &options.knife_bot_mode, values::knifebot_modes, 4);
		}
		if (options.customize_viewmodel)
		{
			ImGui::SliderInt("Custom viewmodel X", &options.customx, -45, 45, "%d");
			ImGui::SliderInt("Custom viewmodel Y", &options.customy, -45, 45, "%d");
			ImGui::SliderInt("Custom viewmodel Z", &options.customz, -45, 45, "%d");
		}
		if (ImGui::SliderFloat("Nightmode Scale", &options.nightmode_value, 0.05f, 1.f, "%.3f")) {
			options.nightmode_value = std::clamp(options.nightmode_value, 0.05f, 1.f);
			CheatGVars::UpdateNightMode = true;
		}
		ImGui::SliderFloat("Aspect Ratio value", &options.misc_aspect_ratio, 0.f, 10.f, "%.3f");
		ImGui::SliderInt("Fakelag Limit", &options.fakelag_limit, 0, 14, "%");
		if (options.slow_walk && ImGui::SliderFloat("Slow walk Speed", &options.slow_walk_speed, 26.f, 100.f))
			options.slow_walk_speed = std::clamp(options.slow_walk_speed, 26.f, 100.f);
		if (ImGui::SliderFloat("Flash Alpha", &options.alpha_modulate, 0.f, 255.f))
			options.alpha_modulate = std::clamp(options.alpha_modulate, 0.f, 255.f);
		if (options.view_fov_changer)
			if (ImGui::SliderFloat("View FOV", &options.view_fov, 1.f, 120.f))
				options.view_fov = std::clamp(options.view_fov, 1.f, 120.f);
		if (options.third_person && ImGui::SliderFloat("ThirdPerson Dist", &options.third_person_dist, 30.f, 150.f))
			options.third_person_dist = std::clamp(options.third_person_dist, 30.f, 150.f);
		
		if (options.legit_aa) {
			
			ImGui::Combo("AA Mode", &options.legit_aa_mode, values::legitaa_modes, 1);
			ImGui::Checkbox("AA Arrows", &options.aa_arrows);
			if (options.aa_arrows && ImGui::SliderInt("Length", &options.aa_arrows_length, 15, 30))
				options.aa_arrows_length = std::clamp(options.aa_arrows_length, 15, 30);
		}

		if (options.show_fov) {
			ImGui::ColorEdit("FOV Default", colors.fov_default);
			ImGui::ColorEdit("FOV Silent", colors.fov_silent);
		}
		if (options.show_spread && options.show_spread_type != 1) {
			ImGui::ColorEdit("Spread", colors.spread_color);
		}
		if (options.damage_indicator) {
			ImGui::ColorEdit("Damage Indicator", colors.damage_indicator_color);
		}
		if (options.grenade_prediction) {
			ImGui::ColorEdit("Grenade Prediction", colors.grenade_prediction_color);
		}
		if (options.molotov_timer) {
			ImGui::ColorEdit("Molotov Timer", colors.molotov_timer_color);
		}
	};
	ImGui::EndChild();
	ImGui::NextColumn();

	static int animamisc[512];
	
	
	ImGui::BeginChild("##colorsmisc", ImVec2(-1, -1), true); {
		
		ImGui::TextColored(ImColor(menur, menug, menub), "Menu Misc");
		
		ImGui::ColorPicker3("Menu Color", options.cMenuCol);

		const auto old_style_state = options.newstyle;

		if (ImGui::Combo("Menu Style", &options.newstyle, values::menu_styles, 3))
			a.CreateStyle();

	};
	ImGui::EndChild();
}
// ---
c_misc* g_Misc = new c_misc();