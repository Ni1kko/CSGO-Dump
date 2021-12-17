#pragma once
#include "../../dependencies/common_includes.hpp"
#include "../features/features.hpp"
#include "../menu/menu.hpp"
#include "../../security/xor.h"

/*how to get entity:
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
	}
*/
hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::lock_cursor::fn lock_cursor_original = nullptr;
hooks::in_key_event::fn in_key_event_original = nullptr;
hooks::do_post_screen_effects::fn do_post_screen_effects_original = nullptr;
hooks::draw_model_execute::fn draw_model_execute_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::sv_cheats::fn sv_cheats_original = nullptr;


unsigned int get_virtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }

bool hooks::initialize() {
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto lock_cursor_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 67));
	auto in_key_event_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 21));
	auto do_post_screen_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto viewmodel_fov_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	auto sv_cheats_target = reinterpret_cast<void*>(get_virtual(interfaces::console->get_convar("sv_cheats"), 13));
	auto setup_bones_target = reinterpret_cast<void*>(utilities::pattern_scan(GetModuleHandleA(xorstr_("client.dll")), xorstr_("55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12")));

	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(lock_cursor_target, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize lock_cursor. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(in_key_event_target, &in_key_event::hook, reinterpret_cast<void**>(&in_key_event_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize in_key_event. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(do_post_screen_effects_target, &do_post_screen_effects::hook, reinterpret_cast<void**>(&do_post_screen_effects_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize do_post_screen_effects. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize draw_model_execute. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(viewmodel_fov_target, &viewmodel_fov::hook, NULL) != MH_OK) {
		throw std::runtime_error("failed to initialize viewmodel_fov_hook. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize override_view. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(sv_cheats_target, &sv_cheats::hook, reinterpret_cast<void**>(&sv_cheats_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize sv_cheats. (outdated index?)");
		return false;
	}

	
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}

	chams::init();
	skinchanger::animfix_hook();

	variables.misc.viewmodel[0] = interfaces::console->get_convar("viewmodel_offset_x")->get_int();
	variables.misc.viewmodel[1] = interfaces::console->get_convar("viewmodel_offset_y")->get_int();
	variables.misc.viewmodel[2] = interfaces::console->get_convar("viewmodel_offset_z")->get_int();

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	chams::shutdown();
	skinchanger::animfix_unhook();

	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return create_move_original(input_sample_frametime, cmd);

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	csgo::cmd = cmd;

	if (variables.misc.infduck)
		cmd->buttons |= in_bullrush;

	int unFlags = csgo::local_player->flags();
	int preFlags = csgo::local_player->flags();

	if (variables.misc.bEdgeJump && GetAsyncKeyState(variables.misc.edgejumpkey) && unFlags & (1 << 0) && !(preFlags & (1 << 0))) {

		//pCmd->buttons |= (IN_JUMP);
		cmd->buttons |= (in_jump); // jump
	}

	if (variables.misc.bDuckbug && !(unFlags & (1 << 0)) && preFlags & (1 << 0) && GetAsyncKeyState(variables.misc.edgebugkey))
	{
		cmd->buttons |= (in_duck);
	}





	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	csgo::send_packet = true;
	misc::disable_shadow;
	misc::nade_predict();
	misc::remove_3dsky();
	misc::remove_bloom();
	misc::bunny_hop(cmd);
	misc::disable_post_processing();
	misc::clantag();


	misc::chatspam();

	interfaces::console->get_convar("viewmodel_offset_x")->callbacks.size = 0;
	interfaces::console->get_convar("viewmodel_offset_x")->set_value(variables.misc.viewmodel[0]);
	interfaces::console->get_convar("viewmodel_offset_y")->callbacks.size = 0;
	interfaces::console->get_convar("viewmodel_offset_y")->set_value(variables.misc.viewmodel[1]);
	interfaces::console->get_convar("viewmodel_offset_z")->callbacks.size = 0;
	interfaces::console->get_convar("viewmodel_offset_z")->set_value(variables.misc.viewmodel[2]);


	csgo::aa_running = false;
	prediction::start(cmd); 
	{
		auto old_viewangles = cmd->viewangles;
		auto old_forwardmove = cmd->forwardmove;
		auto old_sidemove = cmd->sidemove;
		if (variables.aimbot_mode == aimbot_legit) {
			backtracking::store(cmd);
			aimbot::run(cmd);
			backtracking::run(cmd);
			triggerbot::run(cmd);
			antiaim::run(cmd, old_forwardmove);
		}
		else if (variables.aimbot_mode == aimbot_rage) {

		}
		math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);
	} 
	prediction::end();

	if (variables.misc.reveal_ranks && cmd->buttons & in_score) // rank revealer will work even after unhooking, idk how to "hide" ranks  again
		interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);

	if (variables.misc.antiaim_arrows)
		antiaim::fake_animations();

	*(bool*)(*frame_pointer - 0x1C) = csgo::send_packet;

	math::normalize_angle(cmd->viewangles);

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
		render::draw_text_string(0, 0, render::fonts::main_font, "plague", false, color::white(255)); // watermark if u dont know

		if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player)
			esp::render();

		visuals::sniper_crosshair();

		menu::draw();
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::lock_cursor::hook() {
	if (csgo::menu_open) {
		interfaces::surface->unlock_cursor();
		interfaces::inputsystem->enable_input(false);
		return;
	}
	else {
		interfaces::inputsystem->enable_input(true);
	}

	return lock_cursor_original(interfaces::surface);
}

int __fastcall hooks::in_key_event::hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding) {
	if (csgo::menu_open)
		return 0;

	return in_key_event_original(event_code, key_num, current_binding);
}

bool __stdcall hooks::do_post_screen_effects::hook(int value) {
	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player)


	return do_post_screen_effects_original(interfaces::clientmode, value);
}

void __stdcall hooks::draw_model_execute::hook(IMatRenderContext* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (interfaces::model_render->is_material_overriden() &&
		!strstr(info.model->name, "arms") &&
		!strstr(info.model->name, "weapons/v_")) {
		return draw_model_execute_original(interfaces::model_render, ctx, state, info, matrix);
	}
	chams::run(draw_model_execute_original, ctx, state, info, matrix);
}

void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	static int nightmode_value = 0;
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		frame_stage_notify_original(interfaces::client, frame_stage);
		nightmode_value = 0;
		return;
	}
	antiaim::animation_fix(frame_stage);
	if (frame_stage == FRAME_RENDER_START && csgo::local_player) {
		static bool enabledtp = false, check = false;
		if (variables.misc.thirdperson && variables.misc.thirdperson_key) {
			if (GetAsyncKeyState(variables.misc.thirdperson_key) && csgo::local_player && csgo::local_player->is_alive())
			{
				if (!check)
					enabledtp = !enabledtp;
				check = true;
			}
			else
				check = false;

			if (csgo::local_player && csgo::local_player->is_alive())
			{
				if (enabledtp)
				{
					*(bool*)((DWORD)interfaces::input + 0xAD) = true;
					*(float*)((DWORD)interfaces::input + 0xA8 + 0x8 + 0x8) = 150.f;
				}
				else
				{
					*(bool*)((DWORD)interfaces::input + 0xAD) = false;
					*(float*)((DWORD)interfaces::input + 0xA8 + 0x8 + 0x8) = 0.f;
				}
			}
			else
			{
				*(bool*)((DWORD)interfaces::input + 0xAD) = false;
				*(float*)((DWORD)interfaces::input + 0xA8 + 0x8 + 0x8) = 0.f;
			}
		}
		else {
			*(bool*)((DWORD)interfaces::input + 0xAD) = false;
			*(float*)((DWORD)interfaces::input + 0xA8 + 0x8 + 0x8) = 0.f;
		}
	}
	if (frame_stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		if (variables.skins.enabled) {
			skinchanger::run();
			glovechanger::run();
		}
	}
	if (nightmode_value != variables.visuals.nightmode) {
		visuals::nightmode();
		nightmode_value = variables.visuals.nightmode;
	}
	frame_stage_notify_original(interfaces::client, frame_stage);
}

float __stdcall hooks::viewmodel_fov::hook() {
	return 68.f + float(std::clamp(variables.visuals.viewmodel_fov, 0, 30));
}

void __fastcall hooks::override_view::hook(void* _this, void* _edx, view_setup_t* setup) {
	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player && csgo::local_player->is_alive()) {
		if (!csgo::local_player->is_scoped() && !interfaces::input->m_fCameraInThirdPerson)
			setup->fov = 90.f + float(std::clamp(variables.visuals.fov, 0, 30));
	}
	override_view_original(_this, setup);
}

bool __fastcall hooks::sv_cheats::hook(PVOID convar, int edx) {
	if (!convar)
		return false;

	static DWORD CAM_THINK = (DWORD)utilities::pattern_scan(GetModuleHandleA(xorstr_("client.dll")), xorstr_("85 C0 75 30 38 86"));

	if (variables.misc.thirdperson) {
		if ((DWORD)_ReturnAddress() == CAM_THINK)
			return true;
	}

	if (!sv_cheats_original(convar))
		return false;

	return sv_cheats_original(convar);
}

