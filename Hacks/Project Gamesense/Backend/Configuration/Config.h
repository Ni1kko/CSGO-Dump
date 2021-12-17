#pragma once
#include "../Include/GlobalIncludes.h"
#include "../SDK/color.h"
#include "../singleton.h"

#include <unordered_map>
#include <array>
#include <algorithm>
#include <deque>

#pragma comment(lib,"urlmon.lib")

class c_config : public singleton<c_config>
{
protected:
	void _set(c_config& new_cfg);
public:
	std::unordered_map<std::string, bool> b;
	std::unordered_map<std::string, int> i;
	std::unordered_map<std::string, float> f;
	std::unordered_map<std::string, float[4]> c;
	std::unordered_map<std::string, std::unordered_map<int, bool>> m;

	std::vector<const char*> presets = { "Legit", "Rage", "HvH", "Secret", "Headshot", "Alpha", "Bravo","Charlie","Delta","Echo","Foxtrot","Golf","Hotel","India","Juliet","Kiko","Auto","Duals","Revolver","Scout","Awp" };

	void save();
	void save_keys();
	void _load();
	void load();
	void _load_defaults();
	void load_defaults();
	void load_keys();

	CColor imcolor_to_ccolor(float* col)
	{
		return CColor((int)(col[0] * 255.f), (int)(col[1] * 255.f), (int)(col[2] * 255.f), (int)(col[3] * 255.f));
	}

	CColor imcolor_to_ccolor(float* col, float alpha_override)
	{
		return CColor((int)(col[0] * 255.f), (int)(col[1] * 255.f), (int)(col[2] * 255.f), (int)(alpha_override * 255.f));
	}
};

extern IDirect3DTexture9* iconTexture;
static std::deque<c_config*> c_config_deletion_stack;

#define cfg_preset_load(cfg_field, cfg_key, cfg_key_preset) cfg_field[cfg_key] = cfg_field[cfg_key + cfg_key_preset]
#define cfg_preset_reset(cfg_field, cfg_key, cfg_key_preset) cfg_field[cfg_key + cfg_key_preset] = cfg_field[cfg_key]