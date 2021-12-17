#include "interfaces.hpp"
#include "../common_includes.hpp"
#include "../../security/xor.h"
#include "../../security/xor2.h"

i_base_client_dll* interfaces::client = nullptr;
i_input* interfaces::input = nullptr;
i_client_entity_list* interfaces::entity_list = nullptr;
iv_engine_client* interfaces::engine = nullptr;
i_client_mode* interfaces::clientmode = nullptr;
i_client_state* interfaces::clientstate = nullptr;
i_panel* interfaces::panel = nullptr;
i_surface* interfaces::surface = nullptr;
c_global_vars_base* interfaces::globals = nullptr;
i_material_system* interfaces::material_system = nullptr;
iv_model_info* interfaces::model_info = nullptr;
iv_model_render* interfaces::model_render = nullptr;
i_render_view* interfaces::render_view = nullptr;
i_console* interfaces::console = nullptr;
i_localize* interfaces::localize = nullptr;
i_game_event_manager2* interfaces::event_manager = nullptr;
i_inputsytem* interfaces::inputsystem = nullptr;
iv_debug_overlay* interfaces::debug_overlay = nullptr;
IDirect3DDevice9* interfaces::directx = nullptr;
trace* interfaces::trace_ray = nullptr;
glow_manager_t* interfaces::glow_manager = nullptr;
player_game_movement* interfaces::game_movement = nullptr;
player_prediction* interfaces::prediction = nullptr;
player_move_helper* interfaces::move_helper = nullptr;
i_weapon_system* interfaces::weapon_system = nullptr;
i_mem_alloc* interfaces::mem_alloc = nullptr;

bool interfaces::initialize() {
	client = get_interface<i_base_client_dll, interface_type::index>(xorstr_("client.dll"), (xorstr_("VClient018")));
	entity_list = get_interface<i_client_entity_list, interface_type::index>(xorstr_("client.dll"), (xorstr_("VClientEntityList003")));
	engine = get_interface<iv_engine_client, interface_type::index>(xorstr_("engine.dll"), (xorstr_("VEngineClient014")));
	panel = get_interface<i_panel, interface_type::index>(xorstr_("vgui2.dll"), (xorstr_("VGUI_Panel009")));
	surface = get_interface<i_surface, interface_type::index>(xorstr_("vguimatsurface.dll"), (xorstr_("VGUI_Surface031")));
	material_system = get_interface<i_material_system, interface_type::index>(xorstr_("materialsystem.dll"), (xorstr_("VMaterialSystem080")));
	model_info = get_interface<iv_model_info, interface_type::index>(xorstr_("engine.dll"), (xorstr_("VModelInfoClient004")));
	model_render = get_interface<iv_model_render, interface_type::index>(xorstr_("engine.dll"), (xorstr_("VEngineModel016")));
	render_view = get_interface<i_render_view, interface_type::index>(xorstr_("engine.dll"), (xorstr_("VEngineRenderView014")));
	console = get_interface<i_console, interface_type::index>(xorstr_("vstdlib.dll"), (xorstr_("VEngineCvar007")));
	localize = get_interface<i_localize, interface_type::index>(xorstr_("localize.dll"), (xorstr_("Localize_001")));
	event_manager = get_interface<i_game_event_manager2, interface_type::index>(xorstr_("engine.dll"), (xorstr_("GAMEEVENTSMANAGER002")));
	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>(xorstr_("engine.dll"), (xorstr_("VDebugOverlay004")));
	inputsystem = get_interface<i_inputsytem, interface_type::index>(xorstr_("inputsystem.dll"), (xorstr_("InputSystemVersion001")));
	trace_ray = get_interface<trace, interface_type::index>(xorstr_("engine.dll"), (xorstr_("EngineTraceClient004")));
	game_movement = get_interface<player_game_movement, interface_type::index>(xorstr_("client.dll"), (xorstr_("GameMovement001")));
	prediction = get_interface<player_prediction, interface_type::index>(xorstr_("client.dll"), (xorstr_("VClientPrediction001")));

	/*custom interfaces*/
	clientmode = **reinterpret_cast<i_client_mode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client)[0] + 0x1F));

	clientstate = **(i_client_state***)(utilities::pattern_scan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	directx = **(IDirect3DDevice9***)(utilities::pattern_scan(GetModuleHandleA(XorString("shaderapidx9.dll")), XorString("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1);
	input = *(i_input**)(utilities::pattern_scan(GetModuleHandleA(XorString("client.dll")), XorString("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10")) + 1);
	glow_manager = (glow_manager_t*)(*(uintptr_t*)(utilities::pattern_scan(GetModuleHandleA(XorString("client.dll")), XorString("0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00")) + 3));
	move_helper = **(player_move_helper***)(utilities::pattern_scan(GetModuleHandleA(XorString("client.dll")), XorString("8B 0D ? ? ? ? 8B 46 08 68")) + 2);
	weapon_system = *(i_weapon_system**)(utilities::pattern_scan(GetModuleHandleA(XorString("client.dll")), XorString("8B 35 ? ? ? ? FF 10 0F B7 C0")) + 2);

	mem_alloc = *(i_mem_alloc**)(GetProcAddress(GetModuleHandleA(XorString("tier0.dll")), XorString("g_pMemAlloc")));

	console::log("[setup] interfaces initialized!\n");

	return true;
}