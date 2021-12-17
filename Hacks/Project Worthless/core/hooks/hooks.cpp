#pragma once
#include "../../dependencies/common_includes.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../features/misc/hitmarker.hpp"
#include "../menu/FGUI/FGUI.hh"
#include "../menu/fgui_menu.hpp"
#include "../features/aim/backtrack.h"

#pragma comment(lib, "Winmm.lib")

/*how to get entity:

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
	}

*/


hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::in_key_event::fn in_key_event_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::fire_game_event::fn fire_game_event_original = nullptr;

unsigned int get_virtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }

bool hooks::initialize() {
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	auto in_key_event_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 21));
	auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto scene_end_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	auto fire_game_event_target = reinterpret_cast<void*>(get_virtual(interfaces::event_manager, 9));

	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move.");
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse.");
		return false;
	}

	if (MH_CreateHook(scene_end_target, &scene_end::hook, reinterpret_cast<void**>(&scene_end_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse.");
		return false;
	}

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize lock_cursor.");
		return false;
	}

	if (MH_CreateHook(in_key_event_target, &in_key_event::hook, reinterpret_cast<void**>(&in_key_event_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize in_key_event.");
		return false;
	}

	if (MH_CreateHook(fire_game_event_target, &fire_game_event::hook, reinterpret_cast<void**>(&fire_game_event_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize fire_game_event.");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}

	visuals::chams::create_materials();

#ifdef debug_build
	console::log("[setup] hooks initialized!\n");
#endif
	return true;
}

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage)
{
	static auto backtrack_init = (backtrack.init(), false);

	if (frame_stage == FRAME_RENDER_START) {
		misc::no_flash();
	}

	if (frame_stage == FRAME_NET_UPDATE_END && interfaces::engine->is_in_game()) { 
		backtrack.update();
	}
	frame_stage_notify_original(interfaces::client, frame_stage);
}

void hooks::release() {

	visuals::chams::remove_materials();

	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

bool is_behind_smoke(vec3_t start_pos, vec3_t end_pos) {
	typedef bool(__cdecl* line_goes_through_smoke) (vec3_t, vec3_t);
	static line_goes_through_smoke line_goes_through_smoke_fn = 0;

	if (!line_goes_through_smoke_fn)
		line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(utilities::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

	if (line_goes_through_smoke_fn)
		return line_goes_through_smoke_fn(start_pos, end_pos);
	return false;
}

void __stdcall hooks::scene_end::hook() {

	visuals::chams::run();
	visuals::glow::run();

	return scene_end_original(interfaces::render_view);
}


bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);
	misc::movement::pre_prediction_flags();
	
	misc::movement::fastduck(cmd);
	prediction::start(cmd); {
		aimbot::aim(cmd);
		backtrack.run(cmd);
	} prediction::end();

	misc::movement::edgejump(cmd);
	misc::movement::duckbug(cmd);
	misc::movement::jumpbug(cmd);

	misc::spectators();
	misc::rank_reveal();
	visuals::force_crosshair();
	hitmarker.run();

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);

	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		
		if (vars::checkbox["#watermark"]->get_bool()) {

			render::draw_filled_rect(5, 5, 170, 30, color(25, 25, 25, 225));
			render::draw_filled_rect(10, 10, 160, 20, color(15, 15, 15, 225));

			render::draw_gradient_rect(10, 10, 160, 2, color(208, 128, 255, 255), color(88, 55, 105, 255), true);


			//88, 55, 105
			render::draw_outline(5, 5, 170, 30, color(2, 2, 2, 125));
			render::draw_outline(10, 10, 160, 20, color(2, 2, 2, 125));

			time_t now = time(0);
			tm* local_tm = localtime(&now);

			std::vector<std::string> time = {
				std::to_string(local_tm->tm_hour),
				std::to_string(local_tm->tm_min),
				std::to_string(local_tm->tm_sec)
			};

			for (auto& elem : time)
			{
				if (elem.length() < 2)
					elem = "0" + elem;
			}

			std::string time_str = time[0] + ":" + time[1] + ":" + time[2];
			render::draw_text_string(90, 15, render::fonts::watermark_font, "milsugi ?| " + time_str, true, color(232, 232, 232, 200));
		}
			
		visuals::run();
		misc::movement::speed();
		misc::spectators();
		hitmarker.run();
		misc::disable_post_processing();
		misc::rank_reveal();
		fgui::handler::render_window();

		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::lock_cursor::hook(c_usercmd* x) {
	if (vars::container["#window"]->get_state()) {
		interfaces::surface->unlock_cursor();
		return;
		
		interfaces::inputsystem->enable_input(false);

	}
	else {
		interfaces::inputsystem->enable_input(true);
	}

	return lock_cursor_original(interfaces::surface);
}

int __fastcall hooks::in_key_event::hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding) {

	if (vars::container["#window"]->get_state())
		return 0;

	return in_key_event_original(event_code, key_num, current_binding);
}           

bool __stdcall hooks::fire_game_event::hook(i_game_event* event) {
	auto event_name = event->get_name();

	if (!strcmp(event_name, "player_hurt")) {
		interfaces::console->console_printf("player was hurt.");
	}

	return fire_game_event_original(interfaces::event_manager, event);
}

