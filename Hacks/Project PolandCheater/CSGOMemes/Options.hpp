#pragma once

#include "json.hpp"

#include "ValveSDK/misc/Color.hpp"

#define OPTION(var, val) Option var{#var, val}

class Option
{
    struct Value_t
    {
        std::string szString;
        float       fValue;
        int         nValue;
        Color       clrValue;
    };

public:
    Option(std::string name, std::string value);
    ~Option();

    const char*  get_name(void)   const { return m_szName.data(); }
    float        get_float(void)  const { return m_Value.fValue; }
    int          get_int(void)    const { return m_Value.nValue; }
    bool         get_bool(void)   const { return !!m_Value.nValue; }
    const char*  get_string(void) const { return m_Value.szString.data(); }
    const Color& get_color(void)  const { return m_Value.clrValue; }

    void set_float(float val);
    void set_int(int val);
    void set_bool(bool val);
    void set_string(const char* val);
    void set_color(const Color& val);

    template<class T>
    T get() const
    { // This function is not supposed to be used. The template should be deduced to the functions below
        static_assert(sizeof(T) == 0, "Invalid template type");
    }

    template<> float       get() const { return m_Value.fValue; }
    template<> int         get() const { return m_Value.nValue; }
    template<> bool        get() const { return !!m_Value.nValue; }
    template<> std::string get() const { return m_Value.szString; }
    template<> Color       get() const { return m_Value.clrValue; }

    template<class T>
    void set(T val)
    { // This function is not supposed to be used. The template should be deduced to the functions below
        static_assert(sizeof(T) == 0, "Invalid template type");
    }

    template<> void set(float       val) { set_float(val); }
    template<> void set(int         val) { set_int(val); }
    template<> void set(bool        val) { set_bool(val); }
    template<> void set(const char* val) { set_string(val); }
    template<> void set(const Color& val) { set_color(val); }

private:
    std::string m_szName;
    Value_t     m_Value;
};
class Config
{
public:
    Config() {}

    //
    // LEGITBOT
    //
    OPTION(legitbot_enabled, "0");
	//
	// RIFLE
	//
    OPTION(legitbot_key_rifle, "0");
    OPTION(legitbot_smooth_rifle, "1");
    OPTION(legitbot_fov_rifle, "0");
    OPTION(legitbot_randomized_smooth_rifle, "1");
    OPTION(legitbot_randomized_recoil_min_rifle, "0");
    OPTION(legitbot_randomized_recoil_max_rifle, "0");
    OPTION(legitbot_randomize_angle_rifle, "1");
	//
	// PISTOL
	//
	OPTION(legitbot_key_pistol, "0");
	OPTION(legitbot_smooth_pistol, "1");
	OPTION(legitbot_fov_pistol, "0");
	OPTION(legitbot_randomized_smooth_pistol, "1");
	OPTION(legitbot_randomized_recoil_min_pistol, "0");
	OPTION(legitbot_randomized_recoil_max_pistol, "0");
	OPTION(legitbot_randomize_angle_pistol, "1");
	//
	// SNIPER
	//
	OPTION(legitbot_key_sniper, "0");
	OPTION(legitbot_smooth_sniper, "1");
	OPTION(legitbot_fov_sniper, "0");
	OPTION(legitbot_randomized_smooth_sniper, "1");
	OPTION(legitbot_randomized_recoil_min_sniper, "0");
	OPTION(legitbot_randomized_recoil_max_sniper, "0");
	OPTION(legitbot_randomize_angle_sniper, "1");
    OPTION(legitbot_backtrack, "0");



    OPTION(triggerbot_enabled, "0");
    OPTION(triggerbot_delay, "0");
    OPTION(triggerbot_key, "0");
    OPTION(triggerbot_hitchance, "0");

    OPTION(triggerbot_Head, "0");
    OPTION(triggerbot_Arms, "0");
    OPTION(triggerbot_Chest, "0");
    OPTION(triggerbot_Stomach, "0");
    OPTION(triggerbot_Legs, "0");




    //
    // RAGE
    //

    OPTION(rage_enabled, "0");
    OPTION(rage_auto_fire, "0");
    OPTION(rage_fov, "0");
    OPTION(rage_silent, "0");
    OPTION(rage_auto_pistol, "0");
    OPTION(rage_key, "0");

    OPTION(rage_antiaim_enable, "0");
    OPTION(rage_pitch, "0");
    OPTION(rage_yaw_true, "0");
    OPTION(rage_yaw_fake, "0");
    OPTION(rage_knife_aa, "0");


    OPTION(rage_friendly_fire, "0");
    OPTION(rage_hitbox, "0");
    OPTION(rage_hitscan, "0");
    OPTION(rage_no_recoil, "0");

    OPTION(rage_auto_stop, "0");
    OPTION(rage_auto_crouch, "0");
    OPTION(rage_auto_scope, "0");
    OPTION(rage_min_dmg, "0");
    OPTION(rage_hitchance, "0");
    OPTION(rage_hitchance_amount, "0");
    OPTION(rage_resolver, "0");

    OPTION(rage_backtrack, "0");



    OPTION(esp_enabled, "0");
    OPTION(esp_poland_box, "0");
    OPTION(esp_poland_name, "0");
    OPTION(esp_poland_hp, "0");
    OPTION(esp_poland_weapon, "0");
    OPTION(esp_poland_spread_crosshair, "0");
    OPTION(esp_poland_grenade, "0");
    OPTION(esp_poland_c4, "0");
    OPTION(esp_poland_dlight, "0");
    OPTION(esp_poland_skeleton, "0");


    OPTION(esp_poland_no_scope_border, "0");
    OPTION(esp_grenade_prediction, "0");

    OPTION(esp_poland_filter_players, "0");
    OPTION(esp_poland_filter_enemies_only, "0");

    OPTION(esp_poland_worldweapon, "0");
    OPTION(esp_poland_c4world, "0");


    OPTION(visual_no_smoke, "0");

    // 
    // GLOW
    // 
    OPTION(glow_enabled, "0");
    OPTION(glow_enemies_only, "0");
    OPTION(glow_players, "1");
    OPTION(glow_weapons, "1");

    OPTION(misc_bhop, "1");
    OPTION(misc_no_hands, "0");

    OPTION(fov_cs_debug, "90");
    OPTION(viewmodel_fov, "90");
    OPTION(no_visual_recoil, "0");
    OPTION(min_velocity_auto_strafe, "0");
    OPTION(auto_strafe, "0");
    OPTION(auto_strafe_keys_check, "0");
    OPTION(misc_wireframe_hands, "0");
    OPTION(misc_nightmode, "0");
    OPTION(misc_fakelag, "0");
    OPTION(misc_slowmotion, "0");
    OPTION(misc_adaptive_fakelag, "0");

    OPTION(knife_model, "0");
};

class ConfigScheme
{
public:
    ConfigScheme(std::map<std::string, Config> default);

    bool config_exists(int idx);
    void set_active_config(int idx);
    int add_config(const std::string& name);
    void remove_config(const std::string& name);

    const std::map<std::string, Config>& 
        get_config_list() const { 
        return config_list;
    }

    static constexpr char* json_file = "ucstaff.json";

    bool save_current_config();
    bool save_config();
    bool load_config(int idx = -1);

    static bool item_getter(void* data, int i, const char** name);

    int current_config_index;
private:
    std::string                     current_config_name;
    std::map<std::string, Config>   config_list;
    std::vector<std::string>        config_names;
};

extern ConfigScheme g_ConfigScheme;
extern Config g_Options;
extern bool g_Unload;

void to_json(nlohmann::json& json, const Config& p);
void from_json(const nlohmann::json& json, Config& p);