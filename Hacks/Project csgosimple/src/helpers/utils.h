#pragma once

#define NOMINMAX
#include <string>
#include <Windows.h>
#include <functional>
#include <initializer_list>

#include "../imgui/imgui.h"
#include "../valve_sdk/sdk.hpp"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / interfaces::global_vars->interval_per_tick ) )
#define TICKS_TO_TIME(t) ( interfaces::global_vars->interval_per_tick * (t) )

namespace utils
{
	int random(const int& min, const int& max);
	float random(const float& min, const float& max);

	bool can_lag(const bool& voice_check = true);
	float get_interpolation_compensation();

	bool is_connected();
	bool is_sniper(int iItemDefinitionIndex);

	std::string get_weapon_name(void* weapon);

	HMODULE get_module(const std::string& name);
	void* get_export(const char* module_name, const char* export_name);
	std::uint8_t* pattern_scan(void* module, const char* signature);
	std::uint8_t* pattern_scan(const char* moduleName, const char* signature);

	void force_full_update();
	void set_player_name(const char* name);
	void create_beam(const int& user_id, const Vector& end_pos);
	void sound_esp_beam(c_base_player* player, const Vector& sound_pos);
	bool is_line_goes_through_smoke(Vector vStartPos, Vector vEndPos);
	bool hitchance(c_base_entity* entity, const QAngle& angles, const float& chance, const float& hit_count = 150.f, const int& hit_group = -1);

	int get_active_key();
	ImU32 to_im32(const Color& color, const float& alpha = 1.f);
	wchar_t* to_wstring(const char* str);
	void RankRevealAll();

}

