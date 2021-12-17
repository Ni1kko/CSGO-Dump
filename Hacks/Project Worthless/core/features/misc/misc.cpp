#include "../features.hpp"
#include "../..//menu/fgui_menu.hpp"
#include "../../menu/fgui_input.hpp"
#include "../aim/backtrack.h"
#include "../../../dependencies/interfaces/iv_render_beams.hpp"

#define D3DX_PI    ((FLOAT)  3.141592654f)

auto flags_backup = 0;

convar cvar;

void misc::movement::bunny_hop(c_usercmd* cmd) {
	static bool last_jumped = false, should_fake = false;

	if (!csgo::local_player)
		return;

	if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip)
		return;

	if (!vars::checkbox["#bhop_checkbox"]->get_bool())
		return;

	if (!last_jumped && should_fake) {
		should_fake = false;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (csgo::local_player->flags() & fl_onground) {
			last_jumped = true;
			should_fake = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			last_jumped = false;
		}
	}
	else {
		last_jumped = false;
		should_fake = false;
	}
};

void misc::movement::pre_prediction_flags() {
	if (!vars::checkbox["#jumpbug_checkbox"]->get_bool() &&
		!vars::checkbox["#edgebug_checkbox"]->get_bool() &&
		!vars::checkbox["#edgejump"]->get_bool())
		return;

	auto player = csgo::local_player;

	if (!player || !player->is_alive())
		return;

	flags_backup = player->flags();
}

void misc::movement::jumpbug(c_usercmd* pCmd) {
	auto local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player())); // lol
	float max_radias = D3DX_PI * 2;
	float step = max_radias / 128;
	float xThick = 23;
	auto local_player = csgo::local_player;
	
	if (!interfaces::engine->is_connected())
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!local_player)
		return;
	/*
	if (!vars::checkbox["#jumpbug_checkbox"]->get_bool())
		return;

	if (!fgui_input::get_key_state(vars::keybinder["#jumpbug_key"]->get_key()))
		return;

	if (!local_player || local_player->health() <= 0)
		return;

	if (local_player->move_type() == movetype_ladder || local_player->move_type() == movetype_noclip)
		return;

	if (local_player->flags() & fl_onground) {
		pCmd->buttons &= ~in_duck;
		pCmd->buttons |= in_jump;
	}
	*/


	
	if (vars::checkbox["#jumpbug_checkbox"] && fgui_input::get_key_state(vars::keybinder["#jumpbug_key"]->get_key()) && (local->flags() & (1 << 0))) {
		vars::checkbox["#bhop_checkbox"]->set_bool(false);
		bool unduck = pCmd->buttons &= ~in_duck;
		if (unduck) {
			pCmd->buttons &= ~in_duck; // duck
			pCmd->buttons |= in_jump; // jump
			unduck = false;
		}
		vec3_t pos = local->abs_origin();
		for (float a = 0.f; a < max_radias; a += step) {
			vec3_t pt;
			pt.x = (xThick * cos(a)) + pos.x;
			pt.y = (xThick * sin(a)) + pos.y;
			pt.z = pos.z;


			vec3_t pt2 = pt;
			pt2.z -= 8192;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = local;
			interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
				pCmd->buttons |= in_duck; // duck
				pCmd->buttons &= ~in_jump; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			vec3_t pt;
			pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			vec3_t pt2 = pt;
			pt2.z -= 8192;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = local;
			interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
				pCmd->buttons |= in_duck; // duck
				pCmd->buttons &= ~in_jump; // jump
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			vec3_t pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			vec3_t pt2 = pt;
			pt2.z -= 8192;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = local;
			interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
				pCmd->buttons |= in_duck; // duck
				pCmd->buttons &= ~in_jump; // jump
				unduck = true;
			}
		}
	}
	else {
		vars::checkbox["#bhop_checkbox"]->set_bool(true);

	}
	
}

void misc::movement::duckbug(c_usercmd* pCmd) {
	auto local = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player())); // lol
	float max_radias = D3DX_PI * 2;
	float step = max_radias / 128;
	float xThick = 23;


	if (vars::checkbox["#edgebug_checkbox"] && fgui_input::get_key_state(vars::keybinder["#edgebug_key"]->get_key()) && (local->flags() & (1 << 0))) {
		bool unduck = pCmd->buttons &= ~in_duck;
		if (unduck) {
			pCmd->buttons &= ~in_duck; // duck
			unduck = false;
		}
		vec3_t pos = local->abs_origin();
		for (float a = 0.f; a < max_radias; a += step) {
			vec3_t pt;
			pt.x = (xThick * cos(a)) + pos.x;
			pt.y = (xThick * sin(a)) + pos.y;
			pt.z = pos.z;


			vec3_t pt2 = pt;
			pt2.z -= 8192;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = local;
			interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
				pCmd->buttons |= in_duck; // duck
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			vec3_t pt;
			pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			vec3_t pt2 = pt;
			pt2.z -= 8192;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = local;
			interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
				pCmd->buttons |= in_duck; // duck
				unduck = true;
			}
		}
		for (float a = 0.f; a < max_radias; a += step) {
			vec3_t pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			vec3_t pt2 = pt;
			pt2.z -= 8192;

			trace_t fag;

			ray_t ray;
			ray.initialize(pt, pt2);

			trace_filter flt;
			flt.skip = local;
			interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
				pCmd->buttons |= in_duck; // duck
				unduck = true;
			}
		}
	}
}

bool lastvelsaved = false; //saver
int lastjump, lastvel, lasttick = 0; // last vel holder
std::string drawvel; //text drawer holder
std::string drawvel2;
void misc::movement::speed()
{
	if (!interfaces::engine->is_connected())
		return;

	if (!interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	int screenWidth, screenHeight;
	interfaces::engine->get_screen_size(screenWidth, screenHeight);

	auto local_player = csgo::local_player;

	vec3_t speed = local_player->velocity();
	int intspeed = round(speed.Length2D());

	const float delta = intspeed - lastvel;

	std::string vel = std::to_string(intspeed);

	if (local_player->flags() & fl_onground)
	{
		if (lastvelsaved)
		{
			lastvelsaved = false;
		}

		drawvel = vel;
	}
	else
	{
		if (!lastvelsaved)
		{
			lastjump = intspeed;
			lastvelsaved = true;
		}
		if (vars::checkbox["#drawvelpre"]->get_bool())
			drawvel = vel + " (" + std::to_string(lastjump) + ")";
		else 
			drawvel = vel;		
	}
	drawvel2 = "(" + std::to_string(lastjump) + ")";

	color col(255, 255, 255, 255);


	if (vars::checkbox["#drawvel"]->get_bool()) {
		if (local_player->move_type() == movetype_noclip) {
			render::draw_text_string(screenWidth / 2 + 1, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) + 1, render::fonts::name_font_big, "noclip", true, color(0, 0, 0, 255));
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()), render::fonts::name_font_big, "noclip", true, col);

		}
		else {
			switch (vars::combobox["#draw_vel_style"]->get_value()) {
			case 0:
				render::draw_text_string(screenWidth / 2 + 1, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) + 1, render::fonts::name_font_big, drawvel, true, color(0, 0, 0, 255));
				render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()), render::fonts::name_font_big, drawvel, true, col);
				//render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) + 25, render::fonts::font_kz, std::to_string(delta), true, color(20, 20, 20, 170));
				break;
			case 1:

				render::draw_text_string(screenWidth / 2 + 1, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) + 1, render::fonts::name_font_big, std::to_string(intspeed), true, color(0, 0, 0, 255));
				if (intspeed >= 280)
					render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()), render::fonts::name_font_big, std::to_string(intspeed), true, color (170,255,0));
				else 
					render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()), render::fonts::name_font_big, std::to_string(intspeed), true, color(255, 255, 255));

				if (!(local_player->flags() & fl_onground)) {
					if (lastjump >= 270)
						render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) + 25, render::fonts::font_kz, drawvel2, true, color(170, 255, 0));
					else
						render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) + 25, render::fonts::font_kz, drawvel2, true, color(255, 255, 255));
				}
				
				break;
			}

		}

	}
	if (vars::checkbox["#edgebug_checkbox"]->get_bool() && fgui_input::get_key_state(vars::keybinder["#edgebug_key"]->get_key()) && vars::checkbox["#indicators"]->get_bool()) {
		if (flags_backup & fl_onground)
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "eb", true, color(179, 255, 135, 255));
		else
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "eb", true, color(255, 255, 255, 255));
	}

	else if (vars::checkbox["#jumpbug_checkbox"] && fgui_input::get_key_state(vars::keybinder["#jumpbug_key"]->get_key()) && vars::checkbox["#indicators"]->get_bool()) {
		if (csgo::local_player->flags() & fl_onground)
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "jb", true, color(179, 255, 135, 255));
		else
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "jb", true, color(255, 255, 255, 255));
	}
	else if (vars::checkbox["#edgejump"] && fgui_input::get_key_state(vars::keybinder["#edgejump_key"]->get_key()) && vars::checkbox["#indicators"]->get_bool()) {
		if (!(flags_backup & fl_onground))
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "ej", true, color(179, 255, 135, 255));
		else
			render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "ej", true, color(255, 255, 255, 255));
	}
	else if (vars::checkbox["#aim_on_key"] && fgui_input::get_key_state(vars::keybinder["#aim_key"]->get_key()) && vars::checkbox["#indicators"]->get_bool())
		render::draw_text_string(screenWidth / 2, screenHeight - (vars::slider["#drawvel_amount"]->get_value()) - 25, render::fonts::speedfont2, "aim", true, color(255, 255, 255, 255));

	if (interfaces::globals->tick_count > lasttick + 16)
	{
		lasttick = interfaces::globals->tick_count;
		lastvel = intspeed;
	}
		
}

void misc::rank_reveal() noexcept {
	if (!vars::checkbox["#rank_reveal"]->get_bool())
		return;

	if (GetAsyncKeyState(VK_TAB))
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);
}

void misc::no_flash() {
	if (vars::checkbox["#no_flash"]->get_bool() && csgo::local_player) {

		if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
			return;

		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		if (!local_player)
			return;

		if (local_player->flash_duration() > 0.0f)
			local_player->flash_duration() = 0.0f;
	}
}

void misc::spectators() noexcept {

	int screenWidth, screenHeight;
	interfaces::engine->get_screen_size(screenWidth, screenHeight);

	if (vars::checkbox["#spectators_list"]->get_bool()) 
	{
		if (!interfaces::engine->is_connected())
			return;

		if (!interfaces::engine->is_in_game())
			return;

		int spectator_index = 0;
		for (int i = 0; i < interfaces::entity_list->get_highest_index(); i++) {
			auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

			if (!local_player)
				return;

			auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

			if (!entity)
				return;

			player_info_t info;

			if (entity && entity != local_player) {
			interfaces::engine->get_player_info(i, &info);
				if (!entity->is_alive() && !entity->dormant()) {
				auto target = entity->observer_target();

					if (!target)
						return;

					if (target) {
					auto spectator_target = interfaces::entity_list->get_client_entity_handle(target);
						if (spectator_target == local_player) {
								std::string player_name = info.name;
								std::transform(player_name.begin(), player_name.end(), player_name.begin(), ::tolower);
								player_info_t spectator_info;
								interfaces::engine->get_player_info(i, &spectator_info);
								render::draw_gradient_rect(screenWidth - 90, screenHeight / 2 - 3, 80, 18 + (18 * spectator_index), color(25, 25, 25, 255), color(10, 10, 10, 255), true);
								render::draw_gradient_rect(screenWidth - 90, screenHeight / 2 - 3, 80, 2, color(208, 128, 255, 255), color(88, 55, 105, 255), true);
								render::draw_text_string(screenWidth - 50, screenHeight / 2 + (7 * spectator_index), render::fonts::watermark_font, player_name.c_str(), true, color(255, 255, 255, 255));
								spectator_index++;
						}
					}
				}
			}
		}
	}
}

void misc::movement::jbindicator() {
	int screenWidth, screenHeight;
	interfaces::engine->get_screen_size(screenWidth, screenHeight);

	if (!interfaces::engine->is_connected())
		return;

	if (!interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	auto local = csgo::local_player;
}

void misc::movement::fastduck(c_usercmd* cmd) {

	if (!vars::checkbox["#fastduck"]->get_bool()) return;

	cmd->buttons |= in_bullrush;
}

void misc::disable_post_processing() noexcept {

	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_postprocess_enable");
	mat_postprocess_enable->set_value(vars::checkbox["#disable_post_processing"]->get_bool() ? 0 : 1);

}

float normalize_angle(float angle) {
	if (angle > 180.f || angle < -180.f) {
		auto revolutions = std::round(std::abs(angle / 360.f));

		if (angle < 0.f)
			angle = angle + 360.f * revolutions;
		else
			angle = angle - 360.f * revolutions;
	}

	return angle;
}

void misc::movement::edgejump(c_usercmd* cmd) {
	if (!vars::checkbox["#edgejump"]->get_bool() || !fgui_input::get_key_state(vars::keybinder["#edgejump_key"]->get_key()))
		return;

	auto player = csgo::local_player;

	if (!player || !player->is_alive())
		return;

	if (player->move_type() == movetype_ladder || player->move_type() == movetype_noclip)
		return;
	auto user_cmd = cmd;
	auto local_player = csgo::local_player;
	if (flags_backup & fl_onground && !(player->flags() & fl_onground)) {
		cmd->buttons |= in_jump;
	}

	if (vars::checkbox["#strafe_in_air"]->get_bool() && !(player->flags() & fl_onground) && fgui_input::get_key_state(vars::keybinder["#edgejump_key"]->get_key())) {

		if (GetAsyncKeyState(0X57)) //w
		{

			user_cmd->forwardmove = (1550.f * 5) / local_player->velocity().Length2D();
			user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (user_cmd->forwardmove > 450.f)
				user_cmd->forwardmove = 450.f;

		}
		else if (GetAsyncKeyState(0X41)) //a
		{

			user_cmd->sidemove = (-1550.f * 5) / local_player->velocity().Length2D();
			user_cmd->forwardmove = (user_cmd->command_number % 2) == 0 ? 450.f : -450.f;
			if (user_cmd->sidemove < -450.f)
				user_cmd->sidemove = -450.f;

		}
		else if (GetAsyncKeyState(0x44)) //d
		{

			user_cmd->sidemove = (1550.f * 5) / local_player->velocity().Length2D();
			user_cmd->forwardmove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (user_cmd->sidemove > 450.f)
				user_cmd->sidemove = 450.f;

		}
		else if (GetAsyncKeyState(0x53)) //s
		{

			user_cmd->forwardmove = (-1550.f * 5) / local_player->velocity().Length2D();
			user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? 450.f : -450.f;
			if (user_cmd->forwardmove < -450.f)
				user_cmd->forwardmove = -450.f;
		}
		else //default
		{
			user_cmd->forwardmove = (1550.f * 5) / local_player->velocity().Length2D();
			user_cmd->sidemove = (user_cmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (user_cmd->forwardmove > 450.f)
				user_cmd->forwardmove = 450.f;

		}
	}
		

	if (vars::checkbox["#ljbind"]->get_bool() && !(player->flags() & fl_onground) && fgui_input::get_key_state(vars::keybinder["#edgejump_key"]->get_key())) {
		cmd->buttons |= in_duck;
	}
	switch (vars::combobox["#ljbind_type"]->get_value()) {
	case 0:
		break;
	case 1:
		interfaces::engine->execute_cmd("-forward");
		break;
	case 2:
		interfaces::engine->execute_cmd("-back");
		break;
	case 3:
		interfaces::engine->execute_cmd("-moveleft");
		break;
	case 4:
		interfaces::engine->execute_cmd("-moveright");
		break;
	}
}

static int IgonreTicks = 1;
static int Tick = 0;
int Velocity_old = 0;
static bool JumpFailed = false;

