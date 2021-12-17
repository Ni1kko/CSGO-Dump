#include "CLua.h"
//#define SOL_ENABLE_INTEROP 1

class Beam_t {};

#include "../Utility/Utilities.h"
#include "../Configuration/Config.h"
#include "../Hooking/KeyHandler.h"
#include "../../Frontend/Menu/cmenu.h"
#include "../Interfaces/Interfaces.h"
#include "../SDK/CBaseEntity.h"
#include "../Features/Ragebot/CAutoWall.h"
#include "../SDK/INetChannelInfo.h"
#include "../features/misc/misc.h"
#include "../features/Visuals/Drawing.h"
#include "../features/Visuals/Visuals.h"
#include "../Hashing/xorstr.h"
#include "../SDK/NetChannel.h"
#include "../Security/base64.h"
#include "../Security/md5.h"
#include "../clip.h"

#include <time.h>

using namespace Cheat;
using namespace Features;
using namespace Graphics;
using namespace Interfaces;

void lua_panic(sol::optional<std::string> message) {
	Utilities->Game_Msg(("Lua: panic state!"));
	
	if (message) {
		std::string m = message.value();
		Utilities->Game_Msg(("    error: %s"), m.c_str());

		MessageBoxA(0, m.c_str(),  ("Lua: panic state"), MB_APPLMODAL | MB_OK);
	}
}

// ----- lua functions -----

int extract_owner(sol::this_state st) {
	sol::state_view lua_state(st);
	sol::table rs = lua_state["debug"]["getinfo"](2, "S");
	std::string source = rs["source"];
	std::string filename = std::filesystem::path(source.substr(1)).filename().string();
	return c_lua::get()->get_script_id(filename);
}

namespace ns_config {
	/*
	config.get(key)
	Returns value of given key or nil if key not found.
	*/
	std::tuple<sol::object, sol::object, sol::object, sol::object> get(sol::this_state s, std::string key) {
		auto cfg = c_config::get();
		std::tuple<sol::object, sol::object, sol::object, sol::object> retn = std::make_tuple(sol::nil, sol::nil, sol::nil, sol::nil);

		for (auto kv : cfg->b)
			if (kv.first == key)
				retn = std::make_tuple(sol::make_object(s, kv.second), sol::nil, sol::nil, sol::nil);

		for (auto kv : cfg->c)
			if (kv.first == key)
				retn = std::make_tuple(sol::make_object(s, (int)(kv.second[0] * 255)), sol::make_object(s, (int)(kv.second[1] * 255)), sol::make_object(s, (int)(kv.second[2] * 255)), sol::make_object(s, (int)(kv.second[3] * 255)));

		for (auto kv : cfg->f)
			if (kv.first == key)
				retn = std::make_tuple(sol::make_object(s, kv.second), sol::nil, sol::nil, sol::nil);

		for (auto kv : cfg->i)
			if (kv.first == key)
				retn = std::make_tuple(sol::make_object(s, kv.second), sol::nil, sol::nil, sol::nil);

		for (auto kv : cfg->m)
			if (kv.first == key)
				retn = std::make_tuple(sol::make_object(s, kv.second), sol::nil, sol::nil, sol::nil);
		
		return retn;
	}

	/*
	config.set(key, value)
	Sets value for key
	*/
	void set_bool(std::string key, bool v) {
		auto cfg = c_config::get();
		cfg->b[key] = v;
	}

	void set_float(std::string key, float v) {
		auto cfg = c_config::get();

		if (ceilf(v) != v)
			cfg->f[key] = v;
		else
			cfg->i[key] = (int)v;
	}

	void set_color(std::string key, int r, int g, int b, int a) {
		auto cfg = c_config::get();
		cfg->c[key][0] = r / 255.f;
		cfg->c[key][1] = g / 255.f;
		cfg->c[key][2] = b / 255.f;
		cfg->c[key][3] = a / 255.f;
	}

	void set_multiselect(std::string key, int pos, bool e) {
		auto cfg = c_config::get();
		cfg->m[key][pos] = e;
	}

	/*
	config.load()
	Loads selected config
	*/
	void load() {
		c_config::get()->load();
	}

	/*
	config.save()
	Saves selected config

	*/
	void save() {
		c_config::get()->save();
	}
}
namespace ns_cheat {
	/*
	cheat.get_cheat_version()
	Returns (major, minor, build) of the cheat
	*/
	std::tuple<int, int, int> get_cheat_version() {
		return std::tuple<int, int, int> { Settings->VersionMajor, Settings->VersionMinor, Settings->VersionBuild };
	}

	/*
	cheat.get_game_name()
	Returns name of current game
	*/
	std::string get_game_name() {
		return Settings->GameName;
	}

	/*
	cheat.get_user_name()
	Returns username of current user
	*/
	std::string get_user_name() {
		return Settings->UserName;
	}

	void set_event_callback(sol::this_state s, std::string eventname, sol::function func) {
		sol::state_view lua_state(s);
		sol::table rs = lua_state["debug"]["getinfo"](2, ("S"));
		std::string source = rs["source"];
		std::string filename = std::filesystem::path(source.substr(1)).filename().string();

		c_lua::get()->hooks->registerHook(eventname, c_lua::get()->get_script_id(filename), func);

		Utilities->Game_Msg("%s: subscribed to event %s", filename.c_str(), eventname.c_str());
	}

	void run_script(std::string scriptname) {
		int scrid = c_lua::get()->get_script_id(scriptname);
		if (scrid < 0)
			return;

		c_lua::get()->load_script(scrid);
	}

	void reload_active_scripts() {
		c_lua::get()->reload_all_scripts();
	}
}
namespace ns_models {
	/*
	models.get_studio_model(mdl)
	Returns studio model of mdl
	*/
	studiohdr_t* get_studio_model(model_t* mdl) {
		return ModelInfo->GetStudioModel(mdl);
	}

	/*
	models.get_model_index(name)
	Returns model index of given name
	*/
	int get_model_index(std::string name) {
		return ModelInfo->GetModelIndex(name.c_str());
	}

	/*
	models.get_model_name(mdl)
	Returns name of mdl
	*/
	std::string get_model_name(model_t* mdl) {
		return ModelInfo->GetModelName(mdl);
	}

	/*
	models.get_model_materials(mdl, count)
	Returns materials of mdl

	- mdl (model): a model to get materials of
	- count (num): how many materials you want to get
	*/
	sol::table get_model_materials(model_t* mdl, int count) {
		sol::table retn = c_lua::get()->lua.create_table();
		
		/*IMaterial* mats = new IMaterial[count];
		ModelInfo->GetModelMaterials(mdl, count, &mats);

		for (int i = 0; i < count; i++)
			retn.add(mats[i]);*/

		return retn;
	}
}
namespace ns_engine {
	std::tuple<int, int> get_screen_size() {
		int w, h;
		Engine->GetScreenSize(w, h);
		return std::make_tuple(w, h);
	}

	std::string get_system_time() {
		time_t current_time;
		struct tm* time_info;
		static char timeString[10];
		//Get current time
		time(&current_time);
		time_info = localtime(&current_time);
		//Get current time as string
		strftime(timeString, sizeof(timeString), "%X", time_info);
		return timeString;
	}

	float get_body_yaw() {
		return G::fake_obs;
	}

	float get_feet_yaw() {
		return G::proper_obs;
	}

	void client_cmd(std::string cmd) {
		Engine->ClientCmd(cmd.c_str());
	}

	player_info_t get_player_info(int ent) {
		player_info_t p;
		Engine->GetPlayerInfo(ent, &p);
		return p;
	}

	int get_player_for_user_id(int userid) {
		return Engine->GetPlayerForUserID(userid);
	}

	int get_local_player_index() {
		return Engine->GetLocalPlayerIndex();
	}

	float get_last_timestamp() {
		return Engine->GetLastTimeStamp();
	}

	Vector get_view_angles() {
		Vector va;
		Engine->GetViewAngles(&va);
		return va;
	}

	void set_view_angles(Vector va) {
		Engine->SetViewAngles(&va);
	}

	int get_max_clients() {
		return Engine->GetMaxClients();
	}

	bool is_in_game() {
		return Engine->IsInGame();
	}

	bool is_connected() {
		return Engine->IsConnected();
	}

	std::string get_game_directory() {
		return Engine->GetGameDirectory();
	}

	int level_leaf_count() {
		return Engine->LevelLeafCount();
	}

	bool supports_hdr() {
		return Engine->SupportsHDR();
	}

	std::string get_level_name() {
		return Engine->GetLevelName();
	}

	std::string get_level_name_short() {
		return Engine->GetLevelNameShort();
	}

	std::string get_map_group_name() {
		return Engine->GetMapGroupName();
	}

	bool is_occluded(Vector mins, Vector maxs) {
		return Engine->IsOccluded(mins, maxs);
	}

	INetChannelInfo* get_net_channel_info() {
		return Engine->GetNetChannelInfo();
	}

	bool is_playing_demo() {
		return Engine->IsPlayingDemo();
	}

	bool is_recording_demo() {
		return Engine->IsRecordingDemo();
	}

	bool is_paused() {
		return Engine->IsPaused();
	}

	float get_timescale() {
		return Engine->GetTimescale();
	}

	bool is_taking_screenshot() {
		return Engine->IsTakingScreenshot();
	}

	bool is_hltv() {
		return Engine->IsHLTV();
	}

	unsigned int get_engine_build_number() {
		return Engine->GetEngineBuildNumber();
	}

	std::string get_product_version_string() {
		return Engine->GetProductVersionString();
	}

	bool is_hammer_running() {
		return Engine->IsHammerRunning();
	}

	void execute_client_cmd(std::string cmd) {
		Engine->ExecuteClientCmd(cmd.c_str());
	}

	bool map_has_hdr_lighting() {
		return Engine->MapHasHDRLighting();
	}

	int get_app_id() {
		return Engine->GetAppID();
	}

	void client_cmd_unrestricted(std::string cmd) {
		Engine->ClientCmd_Unrestricted(cmd.c_str());
	}

	void set_restrict_client_commands(bool v) {
		Engine->SetRestrictClientCommands(v);
	}

	bool is_low_violence() {
		return Engine->IsLowViolence();
	}

	void write_screenshot(std::string filename) {
		Engine->WriteScreenshot(filename.c_str());
	}

	void set_timescale(float t) {
		Engine->SetTimescale(t);
	}

	std::tuple<int, int> get_mouse_delta(bool b) {
		int dx = 0, dy = 0;
		Engine->GetMouseDelta(dx, dy, b);
		return std::make_tuple(dx, dy);
	}

	void set_blur_fade(float amt) {
		Engine->SetBlurFade(amt);
	}

	void remove_all_paint() {
		Engine->RemoveAllPaint();
	}

	NetChannel* get_netchannel() {
		return Engine->GetNetChannel();
	}
}
namespace ns_entity_list {
	CBaseEntity* get_client_entity(int idx) {
		return EntityList->GetClientEntity(idx);
	}

	int number_of_entities(bool b) {
		return EntityList->NumberOfEntities(b);
	}

	int get_highest_entity_index() {
		return EntityList->GetHighestEntityIndex();
	}

	CBaseEntity* get_client_entity_from_handle(ULONG ent) {
		return EntityList->GetClientEntityFromHandle(ent);
	}
}
namespace ns_utils {
	sol::table get_player_data(player_info_t p) {
		sol::table t = c_lua::get()->lua.create_table();
		t["name"] = std::string(p.szName);
		t["guid"] = std::string(p.szSteamID);
		t["userid"] = p.userId;
		return t;
	}

	double clamp(double v, double mi, double ma) {
		return Utilities->Util_Clamp(v, mi, ma);
	}

	float distance_on_screen(Vector2D a, Vector2D b) {
		return Utilities->Game_DistanceScreen(a, b);
	}

	Vector world_to_screen(Vector pos) {
		Vector scr;
		Utilities->Game_WorldToScreen(pos, scr);
		return scr;
	}

	std::string clipboard_get() {
		std::string cliptext = "";
		clip::get_text(cliptext);
		return cliptext;
	}

	void clipboard_set(std::string txt) {
		clip::set_text(txt);
	}

	void clipboard_clear() {
		clip::clear();
	}
}
namespace ns_globals {
	float realtime() {
		return Globals->realtime;
	}

	int framecount() {
		return Globals->framecount;
	}

	float absoluteframetime() {
		return Globals->absoluteframetime;
	}

	float curtime() {
		return Globals->curtime;
	}

	float frametime() {
		return Globals->frametime;
	}

	int maxclients() {
		return Globals->maxclients;
	}

	int tickcount() {
		return Globals->tickcount;
	}

	float tickinterval() {
		return Globals->interval_per_tick;
	}
}
namespace ns_trace {
	int get_point_contents(Vector pos, int mask) {
		return Trace->GetPointContents(pos, mask);
	}

	std::tuple<float, CBaseEntity*> trace_ray(Vector from, Vector to, int mask) {
		Ray_t ray;
		CTraceFilter filter;
		trace_t trace;
		trace.start = from;
		trace.end = to;
		filter.pSkip1 = G::LocalPlayer;

		Trace->TraceRay(ray, mask, &filter, &trace);
		return std::make_tuple(trace.fraction, trace.m_pEnt);
	}
}
namespace ns_cvar {
	std::string get_command_line_value(std::string var) {
		return Var->GetCommandLineValue(var.c_str());
	}

	ConVar* find_var(std::string name) {
		return Var->FindVar(name.c_str());
	}

	void console_color_print(CColor color, sol::variadic_args args) {
		std::string txt = "";
		for (auto v : args)
			txt += v;

		Var->ConsoleColorPrintf(color, txt.c_str());
	}

	void revoke_callback(ConVar* var) {
		if (var) {
			*(int*)((DWORD)&var->fnChangeCallback + 0xC) = NULL;
		}
	}

	void console_color_print_rgba(int r, int g, int b, int a, sol::variadic_args args) {
		std::string txt = "";
		for (auto v : args)
			txt += v;

		Var->ConsoleColorPrintf(CColor(r, g, b, a), txt.c_str());
	}

	void console_print(sol::variadic_args args) {
		std::string txt = "";
		for (auto v : args)
			txt += v;

		Var->ConsolePrintf(txt.c_str());
	}

	float get_float(ConVar* var) {
		return var ? var->GetFloat() : 0.f;
	}

	int get_int(ConVar* var) {
		return var ? var->GetInt() : 0;
	}

	void set_float(ConVar* var, float f) {
		if (var) var->SetValue(std::to_string(f).c_str());
	}

	void set_string(ConVar* var, const char* f) {
		if (var) var->SetValue(f);
	}

	void set_int(ConVar* var, int i) {
		if (var) var->SetValue(std::to_string(i).c_str());
	}
}
namespace ns_overlay {
	void add_entity_text_overlay(int ent, int line_offset, float duration, int r, int g, int b, int a, sol::variadic_args txt) {
		std::string text = "";
		for (auto v : txt)
			text += v;

		DebugOverlay->AddEntityTextOverlay(ent, line_offset, duration, r, g, b, a, text.c_str());
	}

	void add_box_overlay(Vector pos, Vector mins, Vector maxs, Vector orientation, int r, int g, int b, int a, float duration) {
		DebugOverlay->AddBoxOverlay(pos, mins, maxs, orientation, r, g, b, a, duration);
	}

	void add_sphere_overlay(Vector pos, float rad, int theta, int phi, int r, int g, int b, int a, float duration) {
		DebugOverlay->AddSphereOverlay(pos, rad, theta, phi, r, g, b, a, duration);
	}

	void add_triangle_overlay(Vector p1, Vector p2, Vector p3, int r, int g, int b, int a, bool nodepth, float duration) {
		DebugOverlay->AddTriangleOverlay(p1, p2, p3, r, g, b, a, nodepth, duration);
	}

	void add_line_overlay(Vector p, Vector pp, int r, int g, int b, int a, bool nodepth, float duration) {
		DebugOverlay->AddLineOverlayAlpha(p, pp, r, g, b, a, nodepth, duration);
	}

	void add_text_overlay(Vector pos, int line_offset, float duration, int r, int g, int b, int a, sol::variadic_args txt) {
		std::string text = "";
		for (auto v : txt)
			text += v;

		DebugOverlay->AddTextOverlayRGB(pos, line_offset, duration, r, g, b, a, text.c_str());
	}

	void add_screen_text_overlay(Vector pos, float duration, int r, int g, int b, int a, sol::variadic_args txt) {
		std::string text = "";
		for (auto v : txt)
			text += v;

		DebugOverlay->AddScreenTextOverlay(pos.x, pos.y, duration, r, g, b, a, text.c_str());
	}

	Vector screen_position(Vector pos) {
		Vector scr;
		DebugOverlay->ScreenPosition(pos, scr);
		return scr;
	}

	void add_capsule_overlay(Vector mins, Vector maxs, float pill, int r, int g, int b, int a, float duration) {
		DebugOverlay->AddCapsuleOverlay(mins, maxs, pill, r, g, b, a, duration);
	}
}
namespace ns_beams {
	void draw_beam(Beam_t* beam) {
		RenderBeams->DrawBeam(beam);
	}

	Beam_t* create_ents(BeamInfo_t beam) {
		return RenderBeams->CreateBeamEnts(beam);
	}

	Beam_t* create_ent_point(BeamInfo_t beam) {
		return RenderBeams->CreateBeamEntPoint(beam);
	}

	Beam_t* create_points(BeamInfo_t beam) {
		return RenderBeams->CreateBeamPoints(beam);
	}

	Beam_t* create_ring(BeamInfo_t beam) {
		return RenderBeams->CreateBeamRing(beam);
	}

	Beam_t* create_ring_point(BeamInfo_t beam) {
		return RenderBeams->CreateBeamRingPoint(beam);
	}

	Beam_t* create_circle_points(BeamInfo_t beam) {
		return RenderBeams->CreateBeamCirclePoints(beam);
	}

	Beam_t* create_follow(BeamInfo_t beam) {
		return RenderBeams->CreateBeamFollow(beam);
	}

	void free_beam(Beam_t* beam) {
		RenderBeams->FreeBeam(beam);
	}

	void update_beam_info(Beam_t* beam, BeamInfo_t beaminf) {
		RenderBeams->UpdateBeamInfo(beam, beaminf);
	}
}
namespace ns_localize {
	std::wstring find(std::string token) {
		return Localize->FindSafe(token.c_str());
	}
}
namespace ns_crypto {
	namespace ns_base64 {
		std::string encode(std::string inp) {
			return base64_encode((unsigned char*)inp.c_str(), inp.size());
		}

		std::string decode(std::string inp) {
			return base64_decode(inp);
		}
	}

	std::string hash(std::string inp) {
		return md5(inp);
	}
}
namespace ns_ui {
	std::string new_checkbox(sol::this_state s, std::string tab, std::string container, std::string label, std::string key, std::optional<bool> def, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);
		
		MenuItem_t item;
		item.type = MENUITEM_CHECKBOX;
		item.script = extract_owner(s);
		item.label = label;
		item.key = key;
		item.b_default = def.value_or(false);
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_slider_int(sol::this_state s, std::string tab, std::string container, std::string label, std::string key, int min, int max, std::optional<std::string> format, std::optional<int> def, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_SLIDERINT;
		item.script = extract_owner(s);
		item.label = label;
		item.key = key;
		item.i_default = def.value_or(0);
		item.i_min = min;
		item.i_max = max;
		item.format = format.value_or("%d");
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_slider_float(sol::this_state s, std::string tab, std::string container, std::string label, std::string key, float min, float max, std::optional<std::string> format, std::optional<float> def, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_SLIDERFLOAT;
		item.script = extract_owner(s);
		item.label = label;
		item.key = key;
		item.f_default = def.value_or(0.f);
		item.f_min = min;
		item.f_max = max;
		item.format = format.value_or("%.0f");
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_keybind(sol::this_state s, std::string tab, std::string container, std::string id, std::string key, std::optional<bool> allow_sc, std::optional<int> def, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_KEYBIND;
		item.script = extract_owner(s);
		item.label = id;
		item.key = key;
		item.allow_style_change = allow_sc.value_or(true);
		item.i_default = def.value_or(0);
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_text(sol::this_state s, std::string tab, std::string container, std::string label, std::string key) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_TEXT;
		item.script = extract_owner(s);
		item.label = label;
		item.key = key;

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_singleselect(sol::this_state s, std::string tab, std::string container, std::string label, std::string key, std::vector<const char*> items, std::optional<int> def, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_SINGLESELECT;
		item.script = extract_owner(s);
		item.label = label;
		item.key = key;
		item.i_default = def.value_or(0);
		item.items = items;
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_multiselect(sol::this_state s, std::string tab, std::string container, std::string label, std::string key, std::vector<const char*> items, std::optional<std::map<int, bool>> def, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_MULTISELECT;
		item.script = extract_owner(s);
		item.label = label;
		item.key = key;
		item.m_default = def.value_or(std::map<int, bool> {});
		item.items = items;
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_colorpicker(sol::this_state s, std::string tab, std::string container, std::string id, std::string key, std::optional<int> r, std::optional<int> g, std::optional<int> b, std::optional<int> a, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_COLORPICKER;
		item.script = extract_owner(s);
		item.label = id;
		item.key = key;
		item.c_default[0] = r.value_or(255) / 255.f;
		item.c_default[1] = g.value_or(255) / 255.f;
		item.c_default[2] = b.value_or(255) / 255.f;
		item.c_default[3] = a.value_or(255) / 255.f;
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	std::string new_button(sol::this_state s, std::string tab, std::string container, std::string id, std::string key, std::optional<sol::function> cb) {
		std::transform(tab.begin(), tab.end(), tab.begin(), ::tolower);
		std::transform(container.begin(), container.end(), container.begin(), ::tolower);

		MenuItem_t item;
		item.type = MENUITEM_BUTTON;
		item.script = extract_owner(s);
		item.label = id;
		item.key = key;
		item.callback = cb.value_or(sol::nil);

		c_lua::get()->menu_items[tab][container].push_back(item);
		return key;
	}

	void set_visibility(std::string key, bool v) {
		for (auto t : c_lua::get()->menu_items) {
			for (auto c : t.second) {
				for (auto &i : c.second) {
					if (i.key == key)
						i.is_visible = v;
				}
			}
		}
	}

	void set_items(std::string key, std::vector<const char*> items) {
		for (auto t : c_lua::get()->menu_items) {
			for (auto c : t.second) {
				for (auto &i : c.second) {
					if (i.key == key)
						i.items = items;
				}
			}
		}
	}

	void set_callback(std::string key, sol::function v) {
		for (auto t : c_lua::get()->menu_items) {
			for (auto c : t.second) {
				for (auto &i : c.second) {
					if (i.key == key)
						i.callback = v;
				}
			}
		}
	}

	void set_label(std::string key, std::string v) {
		for (auto t : c_lua::get()->menu_items) {
			for (auto c : t.second) {
				for (auto &i : c.second) {
					if (i.key == key)
						i.label = v;
				}
			}
		}
	}

	bool is_bind_active(std::string key) {
		return c_keyhandler::get()->auto_check(key);
	}
}
namespace ns_surface {
	void set_texture(int id) {
		Surface->DrawSetTexture(id);
	}

	void set_texture_rgba(int id, const unsigned char* rgba, int w, int h) {
		Surface->DrawSetTextureRGBA(id, rgba, w, h);
	}

	void set_color(int r, int g, int b, int a) {
		Surface->SetDrawColor(r, g, b, a);
	}

	int create_texture() {
		return Surface->CreateNewTextureID();
	}

	void draw_filled_rect(int x, int y, int x2, int y2) {
		Surface->DrawFilledRect(x, y, x2, y2);
	}

	void draw_outlined_rect(int x, int y, int x2, int y2) {
		Surface->DrawOutlinedRect(x, y, x2, y2);
	}

	std::tuple<int, int> get_text_size(int font, std::wstring text) {
		int w, h;
		Surface->GetTextSize(font, text.c_str(), w, h);
		return std::make_tuple(w, h);
	}

	void draw_line(int x, int y, int x2, int y2) {
		Surface->DrawLine(x, y, x2, y2);
	}

	void draw_outlined_circle(int x, int y, int radius, int segments) {
		Surface->DrawOutlinedCircle(x, y, radius, segments);
	}

	void draw_circle(int x, int y, int radius, int start_angle, int percent, int thickness, float precision) {
		precision = (2.f / M_PI) / precision;
		float step = M_PI / 180.f;
		int inner = radius - thickness;
		float end_angle = (start_angle + percent) * step;
		start_angle = (start_angle * M_PI) / 180;

		for (; radius > inner; --radius) {
			for (float angle = start_angle; angle < end_angle; angle += precision) {
				int cx = round(x + radius * cos(angle));
				int cx2 = round(x + radius * cos(angle + precision));
				int cy = round(y + radius * sin(angle));
				int cy2 = round(y + radius * sin(angle + precision));
				Surface->DrawLine(cx, cy, cx2, cy2);
			}
		}
	}

	int create_font(std::string fontname, int w, int h, int blur, int flags) {
		auto f = Surface->SCreateFont();
		Surface->SetFontGlyphSet(f, fontname.c_str(), w, h, blur, 0, flags);
		return f;
	}

	void set_text_font(int font) {
		Surface->DrawSetTextFont(font);
	}

	void set_text_color(int r, int g, int b, int a) {
		Surface->DrawSetTextColor(r, g, b, a);
	}

	void set_text_pos(int x, int y) {
		Surface->DrawSetTextPos(x, y);
	}

	void draw_text(std::wstring str) {
		Surface->DrawPrintText(str.c_str(), str.length());
	}

	void draw_textured_rect(int x, int y, int x2, int y2) {
		Surface->DrawTexturedRect(x, y, x2, y2);
	}

	void indicator(std::string str, int r, int g, int b, int a) {
		Visuals->custom_inds.push_back({ str, CColor(r, g, b, a) });
	}

	void draw_filled_rect_fade(int x, int y, int x2, int y2, int alpha, int alpha2, bool horizontal) {
		Surface->DrawFilledRectFade(x, y, x2, y2, alpha, alpha2, horizontal);
	}
}

// ----- lua functions -----

void c_lua::initialize() {
	this->lua = sol::state(sol::c_call<decltype(&lua_panic), &lua_panic>);
	this->lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math, sol::lib::debug);

	this->lua["collectgarbage"] = sol::nil;
	this->lua["dofile"] = sol::nil;
	this->lua["load"] = sol::nil;
	this->lua["loadfile"] = sol::nil;
	this->lua["pcall"] = sol::nil;
	this->lua["print"] = sol::nil;
	this->lua["xpcall"] = sol::nil;
	this->lua["getmetatable"] = sol::nil;
	this->lua["setmetatable"] = sol::nil;
	this->lua["__nil_callback"] = [](){};

	this->lua["print"] = [](std::string s) { Utilities->Game_Msg(s.c_str()); };
	this->lua["error"] = [](std::string s) { Utilities->Game_Msg(s.c_str()); };

	this->lua.new_usertype<CUserCmd>("c_usercmd",
		"command_number", sol::readonly(&CUserCmd::command_number),
		"tick_count", sol::readonly(&CUserCmd::tick_count),
		"viewangles", &CUserCmd::viewangles,
		"aimdirection", &CUserCmd::aimdirection,
		"forwardmove", &CUserCmd::forwardmove,
		"sidemove", &CUserCmd::sidemove,
		"upmove", &CUserCmd::upmove,
		"buttons", &CUserCmd::buttons,
		"impulse", sol::readonly(&CUserCmd::impulse),
		"weaponselect", &CUserCmd::weaponselect,
		"weaponsubtype", sol::readonly(&CUserCmd::weaponsubtype),
		"random_seed", sol::readonly(&CUserCmd::random_seed),
		"mousedx", &CUserCmd::mousedx,
		"mousedy", &CUserCmd::mousedy,
		"hasbeenpredicted", sol::readonly(&CUserCmd::hasbeenpredicted)
	);
	this->lua.new_usertype<IGameEvent>("c_gameevent",
		"get_name", &IGameEvent::GetName,
		"is_reliable", &IGameEvent::IsReliable,
		"is_local", &IGameEvent::IsLocal,
		"is_empty", &IGameEvent::IsEmpty,
		"get_bool", &IGameEvent::GetBool,
		"get_int", &IGameEvent::GetInt,
		"get_uint64", &IGameEvent::GetUint64,
		"get_float", &IGameEvent::GetFloat,
		"get_string", &IGameEvent::GetString
	);
	this->lua.new_enum("HITBOXES",
		"HEAD", HitboxList::HITBOX_HEAD,
		"NECK", HitboxList::HITBOX_NECK,
		"PELVIS", HitboxList::HITBOX_PELVIS,
		"BODY", HitboxList::HITBOX_BODY,
		"THORAX", HitboxList::HITBOX_THORAX,
		"CHEST", HitboxList::HITBOX_CHEST,
		"UPPER_CHEST", HitboxList::HITBOX_UPPER_CHEST,
		"RIGHT_THIGH", HitboxList::HITBOX_RIGHT_THIGH,
		"LEFT_THIGH", HitboxList::HITBOX_LEFT_THIGH,
		"RIGHT_CALF", HitboxList::HITBOX_RIGHT_CALF,
		"LEFT_CALF", HitboxList::HITBOX_LEFT_CALF,
		"RIGHT_FOOT", HitboxList::HITBOX_RIGHT_FOOT,
		"LEFT_FOOT", HitboxList::HITBOX_LEFT_FOOT,
		"RIGHT_HAND", HitboxList::HITBOX_RIGHT_HAND,
		"LEFT_HAND", HitboxList::HITBOX_LEFT_HAND,
		"RIGHT_UPPER_ARM", HitboxList::HITBOX_RIGHT_UPPER_ARM,
		"RIGHT_FOREARM", HitboxList::HITBOX_RIGHT_FOREARM,
		"LEFT_UPPER_ARM", HitboxList::HITBOX_LEFT_UPPER_ARM,
		"LEFT_FOREARM", HitboxList::HITBOX_LEFT_FOREARM
	);
	this->lua.new_enum("FONTFLAGS",
		"NONE", EFontFlags::FONTFLAG_NONE,
		"ITALIC", EFontFlags::FONTFLAG_ITALIC,
		"UNDERLINE", EFontFlags::FONTFLAG_UNDERLINE,
		"STRIKEOUT", EFontFlags::FONTFLAG_STRIKEOUT,
		"SYMBOL", EFontFlags::FONTFLAG_SYMBOL,
		"ANTIALIAS", EFontFlags::FONTFLAG_ANTIALIAS,
		"GAUSSIANBLUR", EFontFlags::FONTFLAG_GAUSSIANBLUR,
		"ROTARY", EFontFlags::FONTFLAG_ROTARY,
		"DROPSHADOW", EFontFlags::FONTFLAG_DROPSHADOW,
		"ADDITIVE", EFontFlags::FONTFLAG_ADDITIVE,
		"OUTLINE", EFontFlags::FONTFLAG_OUTLINE,
		"CUSTOM", EFontFlags::FONTFLAG_CUSTOM,
		"BITMAP", EFontFlags::FONTFLAG_BITMAP
	);
	this->lua.new_enum("BEAMFLAGS",
		"STARTENTITY", FBEAM_STARTENTITY,
		"ENDENTITY", FBEAM_ENDENTITY,
		"FADEIN", FBEAM_FADEIN,
		"FADEOUT", FBEAM_FADEOUT,
		"SINENOISE", FBEAM_SINENOISE,
		"SOLID", FBEAM_SOLID,
		"SHADEIN", FBEAM_SHADEIN,
		"SHADEOUT", FBEAM_SHADEOUT,
		"ONLYNOISEONCE", FBEAM_ONLYNOISEONCE,
		"NOTILE", FBEAM_NOTILE,
		"USE_HITBOXES", FBEAM_USE_HITBOXES,
		"STARTVISIBLE", FBEAM_STARTVISIBLE,
		"ENDVISIBLE", FBEAM_ENDVISIBLE,
		"ISACTIVE", FBEAM_ISACTIVE,
		"FOREVER", FBEAM_FOREVER,
		"HALOBEAM", FBEAM_HALOBEAM,
		"REVERSED", FBEAM_REVERSED
	);
	this->lua.new_enum("BEAMTYPES",
		"POINTS", TE_BEAMPOINTS,
		"SPRITE", TE_SPRITE,
		"DISK", TE_BEAMDISK,
		"CYLINDER", TE_BEAMCYLINDER,
		"FOLLOW", TE_BEAMFOLLOW,
		"RING", TE_BEAMRING,
		"SPLINE", TE_BEAMSPLINE,
		"RINGPOINT", TE_BEAMRINGPOINT,
		"LASER", TE_BEAMLASER,
		"TESLA", TE_BEAMTESLA
	);
	this->lua.new_usertype<BeamInfo_t>("c_beaminfo",
		sol::constructors<BeamInfo_t()>(),
		"type", &BeamInfo_t::m_nType,
		"start_ent", &BeamInfo_t::m_pStartEnt,
		"start_attachment", &BeamInfo_t::m_nStartAttachment,
		"end_ent", &BeamInfo_t::m_pEndEnt,
		"end_attachment", &BeamInfo_t::m_nEndAttachment,
		"start", &BeamInfo_t::m_vecStart,
		"end", &BeamInfo_t::m_vecEnd,
		"model_index", &BeamInfo_t::m_nModelIndex,
		"model_name", &BeamInfo_t::m_pszModelName,
		"halo_index", &BeamInfo_t::m_nHaloIndex,
		"halo_name", &BeamInfo_t::m_pszHaloName,
		"halo_scale", &BeamInfo_t::m_flHaloScale,
		"life", &BeamInfo_t::m_flLife,
		"width", &BeamInfo_t::m_flWidth,
		"end_width", &BeamInfo_t::m_flEndWidth,
		"fade_length", &BeamInfo_t::m_flFadeLength,
		"amplitude", &BeamInfo_t::m_flAmplitude,
		"brightness", &BeamInfo_t::m_flBrightness,
		"speed", &BeamInfo_t::m_flSpeed,
		"start_frame", &BeamInfo_t::m_nStartFrame,
		"framerate", &BeamInfo_t::m_flFrameRate,
		"red", &BeamInfo_t::m_flRed,
		"green", &BeamInfo_t::m_flGreen,
		"blue", &BeamInfo_t::m_flBlue,
		"renderable", &BeamInfo_t::m_bRenderable,
		"segments", &BeamInfo_t::m_nSegments,
		"flags", &BeamInfo_t::m_nFlags,
		"center", &BeamInfo_t::m_vecCenter,
		"start_radius", &BeamInfo_t::m_flStartRadius,
		"end_radius", &BeamInfo_t::m_flEndRadius
	);
	this->lua.new_usertype<ImDrawList>("c_drawlist");
	this->lua.new_usertype<Beam_t>("c_beam");
	this->lua.new_usertype<player_info_t>("c_playerinfo");
	this->lua.new_usertype<ConVar>("c_convar");
	this->lua.new_usertype<INetChannelInfo>("c_netchannelinfo",
		"get_name", &INetChannelInfo::GetName,
		"get_address", &INetChannelInfo::GetAddress,
		"get_time", &INetChannelInfo::GetTime,
		"get_time_connected", &INetChannelInfo::GetTimeConnected,
		"get_buffer_size", &INetChannelInfo::GetBufferSize,
		"get_data_rate", &INetChannelInfo::GetDataRate,
		"is_loopback", &INetChannelInfo::IsLoopback,
		"is_timing_out", &INetChannelInfo::IsTimingOut,
		"is_playback", &INetChannelInfo::IsPlayback,
		"get_latency", &INetChannelInfo::GetLatency,
		"get_avg_latency", &INetChannelInfo::GetAvgLatency,
		"get_avg_loss", &INetChannelInfo::GetAvgLoss,
		"get_avg_choke", &INetChannelInfo::GetAvgChoke,
		"get_avg_data", &INetChannelInfo::GetAvgData,
		"get_avg_packets", &INetChannelInfo::GetAvgPackets,
		"get_total_data", &INetChannelInfo::GetTotalData,
		"get_total_packet", &INetChannelInfo::GetTotalPackets,
		"get_sequence_nr", &INetChannelInfo::GetSequenceNr,
		"is_valid_packet", &INetChannelInfo::IsValidPacket,
		"get_packet_time", &INetChannelInfo::GetPacketTime,
		"get_packet_bytes", &INetChannelInfo::GetPacketBytes,
		"get_timeout_seconds", &INetChannelInfo::GetTimeoutSeconds
	);
	this->lua.new_usertype<NetChannel>("c_netchannel",
		"out_sequence_nr", sol::readonly(&NetChannel::m_nOutSequenceNr),
		"in_sequence_nr", sol::readonly(&NetChannel::m_nInSequenceNr),
		"out_sequence_nr_ack", sol::readonly(&NetChannel::m_nOutSequenceNrAck),
		"out_reliable_state", sol::readonly(&NetChannel::m_nOutReliableState),
		"in_reliable_state", sol::readonly(&NetChannel::m_nInReliableState),
		"choked_packets", sol::readonly(&NetChannel::m_nChokedPackets)
	);
	this->lua.new_usertype<CColor>("c_color",
		sol::constructors<CColor(), CColor(int, int, int), CColor(int, int, int, int)>(),
		"r", &CColor::r,
		"g", &CColor::g,
		"b", &CColor::b,
		"a", &CColor::a
	);
	this->lua.new_usertype<Vector2D>("c_vector2d",
		sol::constructors<Vector2D(), Vector2D(float, float), Vector2D(Vector)>(),
		"x", &Vector2D::x,
		"y", &Vector2D::y,
		"length", &Vector2D::length
	);
	this->lua.new_usertype<Vertex_t>("c_vertex",
		sol::constructors<Vertex_t(), Vertex_t(Vector2D), Vertex_t(Vector2D, Vector2D)>(),
		"init", &Vertex_t::Init,
		"position", &Vertex_t::m_Position,
		"tex_coord", &Vertex_t::m_TexCoord
	);
	this->lua.new_usertype<Vector>("c_vector",
		sol::constructors<Vector(), Vector(float, float), Vector(float, float, float)>(),
		"x", &Vector::x,
		"y", &Vector::y,
		"z", &Vector::z,
		"length", &Vector::Length,
		"length_sqr", &Vector::LengthSqr,
		"length_2d", &Vector::Length2D,
		"length_2d_sqr", &Vector::Length2DSqr,
		"is_zero", &Vector::IsZero,
		"is_valid", &Vector::IsValid,
		"zero", &Vector::Zero,
		"rotate_2d", &Vector::Rotate2D,
		"dist_to", &Vector::DistTo,
		"dist_to_sqr", &Vector::DistToSqr,
		"cross_product", &Vector::Cross,
		"normalize", &Vector::Normalize,
		"normalize_angles", &Vector::NormalizeAngles
	);
	this->lua.new_usertype<Quaternion>("c_quaternion",
		"x", &Quaternion::x,
		"y", &Quaternion::y,
		"z", &Quaternion::z,
		"w", &Quaternion::w
	);
	this->lua.new_usertype<RadianEuler>("c_radianeuler",
		"x", &RadianEuler::x,
		"y", &RadianEuler::y,
		"z", &RadianEuler::z
	);
	this->lua.new_usertype<studiohdr_t>("c_studiohdr",
		"id", sol::readonly(&studiohdr_t::id),
		"version", sol::readonly(&studiohdr_t::version),
		"checksum", sol::readonly(&studiohdr_t::checksum),
		"length", sol::readonly(&studiohdr_t::length),
		"eyeposition", sol::readonly(&studiohdr_t::eyeposition),
		"illumposition", sol::readonly(&studiohdr_t::illumposition),
		"hull_min", sol::readonly(&studiohdr_t::hull_min),
		"hull_max", sol::readonly(&studiohdr_t::hull_max),
		"view_bbmin", sol::readonly(&studiohdr_t::view_bbmin),
		"view_bbmax", sol::readonly(&studiohdr_t::view_bbmax),
		"flags", sol::readonly(&studiohdr_t::flags),
		"numbones", sol::readonly(&studiohdr_t::numbones),
		"get_bone", &studiohdr_t::pBone,
		"numhitboxsets", sol::readonly(&studiohdr_t::numhitboxsets),
		"get_hitbox_set", &studiohdr_t::pHitboxSet,
		"get_hitbox", &studiohdr_t::pHitbox,
		"get_hitbox_count", &studiohdr_t::iHitboxCount,
		"mass", sol::readonly(&studiohdr_t::mass),
		"contents", sol::readonly(&studiohdr_t::contents)
	);
	this->lua.new_usertype<mstudiobone_t>("c_studiobone",
		"get_name", &mstudiobone_t::pszName,
		"parent", sol::readonly(&mstudiobone_t::parent),
		"pos", sol::readonly(&mstudiobone_t::pos),
		"quat", sol::readonly(&mstudiobone_t::quat),
		"rot", sol::readonly(&mstudiobone_t::rot),
		"posscale", sol::readonly(&mstudiobone_t::posscale),
		"rotscale", sol::readonly(&mstudiobone_t::rotscale),
		"flags", sol::readonly(&mstudiobone_t::flags),
		"contents", sol::readonly(&mstudiobone_t::contents)
	);
	this->lua.new_usertype<mstudiohitboxset_t>("c_studiohitboxset",
		"get_hitbox", &mstudiohitboxset_t::GetHitbox
	);
	this->lua.new_usertype<mstudiobbox_t>("c_studiobbox",
		"bone", sol::readonly(&mstudiobbox_t::bone),
		"group", sol::readonly(&mstudiobbox_t::group),
		"bbmin", sol::readonly(&mstudiobbox_t::bbmin),
		"bbmax", sol::readonly(&mstudiobbox_t::bbmax),
		"radius", sol::readonly(&mstudiobbox_t::radius),
		"get_hitbox_name", &mstudiobbox_t::pszHitboxName
	);
	this->lua.new_usertype<model_t>("c_model");
	this->lua.new_usertype<CBaseAnimState>("c_animstate",
		"entity", &CBaseAnimState::m_pBaseEntity,
		"last_cs_anim_update_time", &CBaseAnimState::m_flLastClientSideAnimationUpdateTime,
		"last_cs_anim_update_fc", &CBaseAnimState::m_iLastClientSideAnimationUpdateFramecount,
		"update_time_delta", &CBaseAnimState::m_flUpdateTimeDelta,
		"yaw", &CBaseAnimState::m_flEyeYaw,
		"pitch", &CBaseAnimState::m_flPitch,
		"goal_feet_yaw", &CBaseAnimState::m_flGoalFeetYaw,
		"current_feet_yaw", &CBaseAnimState::m_flCurrentFeetYaw,
		"current_torso_yaw", &CBaseAnimState::m_flCurrentTorsoYaw,
		"unk_velocity_lean", &CBaseAnimState::m_flUnknownVelocityLean,
		"lean_amount", &CBaseAnimState::m_flLeanAmount,
		"feet_cycle", &CBaseAnimState::m_flFeetCycle,
		"feet_yaw_rate", &CBaseAnimState::m_flFeetYawRate,
		"duck_amount", &CBaseAnimState::m_fDuckAmount,
		"landing_duck_additive", &CBaseAnimState::m_fLandingDuckAdditiveSomething,
		"origin", &CBaseAnimState::m_vOrigin,
		"last_origin", &CBaseAnimState::m_vLastOrigin,
		"velocity_x", &CBaseAnimState::m_vVelocityX,
		"velocity_y", &CBaseAnimState::m_vVelocityY,
		"up_velocity", &CBaseAnimState::flUpVelocity,
		"speed_normalized", &CBaseAnimState::m_flSpeedNormalized,
		"feet_spd_fwsw", &CBaseAnimState::m_flFeetSpeedForwardsOrSideWays,
		"feet_spd_unk_fwsw", &CBaseAnimState::m_flFeetSpeedUnknownForwardOrSideways,
		"time_since_started_moving", &CBaseAnimState::m_flTimeSinceStartedMoving,
		"time_since_stopped_moving", &CBaseAnimState::m_flTimeSinceStoppedMoving,
		"is_on_ground", &CBaseAnimState::m_bOnGround,
		"in_hit_ground_animation", &CBaseAnimState::m_bInHitGroundAnimation,
		"last_origin_z", &CBaseAnimState::m_flLastOriginZ,
		"head_height_from_hga", &CBaseAnimState::m_flHeadHeightOrOffsetFromHittingGroundAnimation,
		"stop_to_full_running_fract", &CBaseAnimState::m_flStopToFullRunningFraction,
		"feet_shuffle_fract", &CBaseAnimState::m_flUnknownFraction
	);
	this->lua.new_usertype<CBaseEntity>("c_baseentity",
		"get_prop", &CBaseEntity::get_prop,
		"set_prop", sol::overload(&CBaseEntity::set_prop_int, &CBaseEntity::set_prop_bool, &CBaseEntity::set_prop_float, &CBaseEntity::set_prop_vector),
		"is_player", &CBaseEntity::IsPlayer,
		"is_reloading", &CBaseEntity::IsReloadingEnt,
		"get_max_body_offset", &CBaseEntity::GetMaxDesyncDelta,
		"can_see", &CBaseEntity::IsVisible,
		"is_weapon", &CBaseEntity::IsWeapon,
		"get_abs_origin", &CBaseEntity::GetAbsOrigin,
		"get_render_bounds", &CBaseEntity::get_render_bounds,
		"get_index", &CBaseEntity::GetIndex,
		"get_spawn_time", &CBaseEntity::m_flSpawnTime,
		"get_anim_state", &CBaseEntity::GetAnimState
	);
	this->lua.new_usertype<ModelRenderInfo_t>("c_modelrenderinfo",
		"origin", sol::readonly(&ModelRenderInfo_t::origin),
		"angles", sol::readonly(&ModelRenderInfo_t::angles),
		"entity_index", sol::readonly(&ModelRenderInfo_t::entity_index)
	);
	this->lua.new_usertype<CViewSetup>("c_viewsetup",
		"x", &CViewSetup::x,
		"x_old", &CViewSetup::x_old,
		"y", &CViewSetup::y,
		"y_old", &CViewSetup::y_old,
		"width", &CViewSetup::width,
		"width_old", &CViewSetup::width_old,
		"height", &CViewSetup::height,
		"height_old", &CViewSetup::height_old,
		"fov", &CViewSetup::fov,
		"viewmodel_fov", &CViewSetup::viewmodel_fov,
		"origin", &CViewSetup::origin,
		"angles", &CViewSetup::angles
	);

	auto config = this->lua.create_table();
	config["get"] = ns_config::get;
	config["set"] = sol::overload(ns_config::set_bool, ns_config::set_color, ns_config::set_float, ns_config::set_multiselect);
	config["load"] = ns_config::load;
	config["save"] = ns_config::save;

	auto cheat = this->lua.create_table();
	cheat["set_event_callback"] = ns_cheat::set_event_callback;
	cheat["run_script"] = ns_cheat::run_script;
	cheat["reload_active_scripts"] = ns_cheat::reload_active_scripts;

	auto surface = this->lua.create_table();
	surface["create_font"] = ns_surface::create_font;
	surface["create_texture"] = ns_surface::create_texture;
	surface["draw_filled_rect"] = ns_surface::draw_filled_rect;
	surface["draw_line"] = ns_surface::draw_line;
	surface["draw_outlined_circle"] = ns_surface::draw_outlined_circle;
	surface["draw_circle"] = ns_surface::draw_circle;
	surface["draw_outlined_rect"] = ns_surface::draw_outlined_rect;
	surface["draw_text"] = ns_surface::draw_text;
	surface["draw_textured_rect"] = ns_surface::draw_textured_rect;
	surface["get_text_size"] = ns_surface::get_text_size;
	surface["indicator"] = ns_surface::indicator;
	surface["set_color"] = ns_surface::set_color;
	surface["set_texture"] = sol::overload(ns_surface::set_texture, ns_surface::set_texture_rgba);
	surface["set_text_color"] = ns_surface::set_text_color;
	surface["set_text_font"] = ns_surface::set_text_font;
	surface["set_text_pos"] = ns_surface::set_text_pos;
	surface["draw_filled_rect_fade"] = ns_surface::draw_filled_rect_fade;

	auto models = this->lua.create_table();
	models["get_model_index"] = ns_models::get_model_index;
	models["get_model_materials"] = ns_models::get_model_materials;
	models["get_model_name"] = ns_models::get_model_name;
	models["get_studio_model"] = ns_models::get_studio_model;

	auto engine = this->lua.create_table();
	engine["get_body_yaw"] = ns_engine::get_body_yaw;
	engine["get_feet_yaw"] = ns_engine::get_feet_yaw;
	engine["client_cmd"] = ns_engine::client_cmd;
	engine["client_cmd_unrestricted"] = ns_engine::client_cmd_unrestricted;
	engine["execute_client_cmd"] = ns_engine::execute_client_cmd;
	engine["get_app_id"] = ns_engine::get_app_id;
	engine["get_engine_build_number"] = ns_engine::get_engine_build_number;
	engine["get_game_directory"] = ns_engine::get_game_directory;
	engine["get_last_timestamp"] = ns_engine::get_last_timestamp;
	engine["get_level_name"] = ns_engine::get_level_name;
	engine["get_level_name_short"] = ns_engine::get_level_name_short;
	engine["get_local_player_index"] = ns_engine::get_local_player_index;
	engine["get_map_group_name"] = ns_engine::get_map_group_name;
	engine["get_max_clients"] = ns_engine::get_max_clients;
	engine["get_mouse_delta"] = ns_engine::get_mouse_delta;
	engine["get_netchannel"] = ns_engine::get_netchannel;
	engine["get_net_channel_info"] = ns_engine::get_net_channel_info;
	engine["get_player_for_user_id"] = ns_engine::get_player_for_user_id;
	engine["get_player_info"] = ns_engine::get_player_info;
	engine["get_product_version_string"] = ns_engine::get_product_version_string;
	engine["get_screen_size"] = ns_engine::get_screen_size;
	engine["get_system_time"] = ns_engine::get_system_time;
	engine["get_timescale"] = ns_engine::get_timescale;
	engine["get_view_angles"] = ns_engine::get_view_angles;
	engine["is_connected"] = ns_engine::is_connected;
	engine["is_hammer_running"] = ns_engine::is_hammer_running;
	engine["is_hltv"] = ns_engine::is_hltv;
	engine["is_in_game"] = ns_engine::is_in_game;
	engine["is_low_violence"] = ns_engine::is_low_violence;
	engine["is_occluded"] = ns_engine::is_occluded;
	engine["is_paused"] = ns_engine::is_paused;
	engine["is_playing_demo"] = ns_engine::is_playing_demo;
	engine["is_recording_demo"] = ns_engine::is_recording_demo;
	engine["is_taking_screenshot"] = ns_engine::is_taking_screenshot;
	engine["level_leaf_count"] = ns_engine::level_leaf_count;
	engine["map_has_hdr_lighting"] = ns_engine::map_has_hdr_lighting;
	engine["remove_all_paint"] = ns_engine::remove_all_paint;
	engine["set_blur_fade"] = ns_engine::set_blur_fade;
	engine["set_restrict_client_commands"] = ns_engine::set_restrict_client_commands;
	engine["set_timescale"] = ns_engine::set_timescale;
	engine["set_view_angles"] = ns_engine::set_view_angles;
	engine["supports_hdr"] = ns_engine::supports_hdr;
	engine["write_screenshot"] = ns_engine::write_screenshot;

	auto entity_list = this->lua.create_table();
	entity_list["get_client_entity"] = ns_entity_list::get_client_entity;
	entity_list["get_client_entity_from_handle"] = ns_entity_list::get_client_entity_from_handle;
	entity_list["get_highest_entity_index"] = ns_entity_list::get_highest_entity_index;
	entity_list["number_of_entities"] = ns_entity_list::number_of_entities;

	auto utils = this->lua.create_table();
	utils["get_player_data"] = ns_utils::get_player_data;
	utils["clamp"] = ns_utils::clamp;
	utils["clipboard_clear"] = ns_utils::clipboard_clear;
	utils["clipboard_get"] = ns_utils::clipboard_get;
	utils["clipboard_set"] = ns_utils::clipboard_set;
	utils["distance_on_screen"] = ns_utils::distance_on_screen;
	utils["world_to_screen"] = ns_utils::world_to_screen;

	auto globals = this->lua.create_table();
	globals["realtime"] = ns_globals::realtime;
	globals["framecount"] = ns_globals::framecount;
	globals["absoluteframetime"] = ns_globals::absoluteframetime;
	globals["curtime"] = ns_globals::curtime;
	globals["frametime"] = ns_globals::frametime;
	globals["maxclients"] = ns_globals::maxclients;
	globals["tickcount"] = ns_globals::tickcount;
	globals["tickinterval"] = ns_globals::tickinterval;

	auto trace = this->lua.create_table();
	trace["get_point_contents"] = ns_trace::get_point_contents;
	trace["trace_ray"] = ns_trace::trace_ray;

	auto cvar = this->lua.create_table();
	cvar["console_color_print"] = sol::overload(ns_cvar::console_color_print, ns_cvar::console_color_print_rgba);
	cvar["console_print"] = ns_cvar::console_print;
	cvar["find_var"] = ns_cvar::find_var;
	cvar["get_command_line_value"] = ns_cvar::get_command_line_value;
	cvar["revoke_callback"] = ns_cvar::revoke_callback;
	cvar["get_float"] = ns_cvar::get_float;
	cvar["get_int"] = ns_cvar::get_int;
	cvar["set_float"] = ns_cvar::set_float;
	cvar["set_int"] = ns_cvar::set_int;
	cvar["set_string"] = ns_cvar::set_string;

	auto overlay = this->lua.create_table();
	overlay["add_box_overlay"] = ns_overlay::add_box_overlay;
	overlay["add_capsule_overlay"] = ns_overlay::add_capsule_overlay;
	overlay["add_entity_text_overlay"] = ns_overlay::add_entity_text_overlay;
	overlay["add_line_overlay"] = ns_overlay::add_line_overlay;
	overlay["add_screen_text_overlay"] = ns_overlay::add_screen_text_overlay;
	overlay["add_sphere_overlay"] = ns_overlay::add_sphere_overlay;
	overlay["add_text_overlay"] = ns_overlay::add_text_overlay;
	overlay["add_triangle_overlay"] = ns_overlay::add_triangle_overlay;
	overlay["screen_position"] = ns_overlay::screen_position;

	auto beams = this->lua.create_table();
	beams["create_circle_points"] = ns_beams::create_circle_points;
	beams["create_ents"] = ns_beams::create_ents;
	beams["create_ent_point"] = ns_beams::create_ent_point;
	beams["create_follow"] = ns_beams::create_follow;
	beams["create_points"] = ns_beams::create_points;
	beams["create_ring"] = ns_beams::create_ring;
	beams["create_ring_point"] = ns_beams::create_ring_point;
	beams["draw_beam"] = ns_beams::draw_beam;
	beams["free_beam"] = ns_beams::free_beam;
	beams["update_beam_info"] = ns_beams::update_beam_info;

	auto base64 = this->lua.create_table();
	base64["encode"] = ns_crypto::ns_base64::encode;
	base64["decode"] = ns_crypto::ns_base64::decode;

	auto crypto = this->lua.create_table();
	crypto["hash"] = ns_crypto::hash;
	crypto["base64"] = base64;

	auto localize = this->lua.create_table();
	localize["find"] = ns_localize::find;

	auto ImGui = this->lua.create_table();
	ImGui["new_checkbox"] = ns_ui::new_checkbox;
	ImGui["new_colorpicker"] = ns_ui::new_colorpicker;
	ImGui["new_keybind"] = ns_ui::new_keybind;
	ImGui["new_multiselect"] = ns_ui::new_multiselect;
	ImGui["new_singleselect"] = ns_ui::new_singleselect;
	ImGui["new_slider_float"] = ns_ui::new_slider_float;
	ImGui["new_slider_int"] = ns_ui::new_slider_int;
	ImGui["new_text"] = ns_ui::new_text;
	ImGui["new_button"] = ns_ui::new_button;
	ImGui["set_callback"] = ns_ui::set_callback;
	ImGui["set_items"] = ns_ui::set_items;
	ImGui["set_label"] = ns_ui::set_label;
	ImGui["set_visibility"] = ns_ui::set_visibility;
	ImGui["is_bind_active"] = ns_ui::is_bind_active;

	this->lua["config"] = config;
	this->lua["cheat"] = cheat;
	this->lua["surface"] = surface;
	this->lua["models"] = models;
	this->lua["engine"] = engine;
	this->lua["entity_list"] = entity_list;
	this->lua["utils"] = utils;
	this->lua["globals"] = globals;
	this->lua["trace"] = trace;
	this->lua["cvar"] = cvar;
	this->lua["overlay"] = overlay;
	this->lua["beams"] = beams;
	this->lua["localize"] = localize;
	this->lua["crypto"] = crypto;
	this->lua["ImGui"] = ImGui;

	this->refresh_scripts();
	this->refresh_configs();
	this->load_script(this->get_script_id("autorun.lua"));
}

void c_lua::load_script(int id) {
	if (id == -1)
		return;

	if (this->loaded.at(id))
		return;

	auto path = this->get_script_path(id);
	if (path ==  (""))
		return;

	this->lua.script_file(path, [](lua_State*, sol::protected_function_result result) {
		if (!result.valid()) {
			sol::error err = result;
			Utilities->Game_Msg(err.what());
		}

		return result;
		});

	this->loaded.at(id) = true;
}

void c_lua::unload_script(int id) {
	if (id == -1)
		return;

	if (!this->loaded.at(id))
		return;

	std::map<std::string, std::map<std::string, std::vector<MenuItem_t>>> updated_items;
	for (auto i : this->menu_items) {
		for (auto k : i.second) {
			std::vector<MenuItem_t> updated_vec;

			for (auto m : k.second)
				if (m.script != id)
					updated_vec.push_back(m);

			updated_items[k.first][i.first] = updated_vec;
		}
	}
	this->menu_items = updated_items;

	hooks->unregisterHooks(id);
	this->loaded.at(id) = false;
}

void c_lua::reload_all_scripts() {
	for (auto s : this->scripts) {
		if (this->loaded.at(this->get_script_id(s))) {
			this->unload_script(this->get_script_id(s));
			this->load_script(this->get_script_id(s));
		}
	}
}

void c_lua::unload_all_scripts() {
	for (auto s : this->scripts)
		if (this->loaded.at(this->get_script_id(s)))
			this->unload_script(this->get_script_id(s));
}

void c_lua::refresh_scripts() {
	auto oldLoaded = this->loaded;
	auto oldScripts = this->scripts;

	this->loaded.clear();
	this->pathes.clear();
	this->scripts.clear();

	for (auto& entry : std::filesystem::directory_iterator( ("C:\\source\\lua"))) {
		if (entry.path().extension() ==  (".lua")) {
			auto path = entry.path();
			auto filename = path.filename().string();

			bool didPut = false;
			for (int i = 0; i < oldScripts.size(); i++) {
				if (filename == oldScripts.at(i)) {
					this->loaded.push_back(oldLoaded.at(i));
					didPut = true;
				}
			}

			if (!didPut)
				this->loaded.push_back(false);

			this->pathes.push_back(path);
			this->scripts.push_back(filename);
		}
	}
}

void c_lua::refresh_configs() {
	auto oldScripts = this->configs;
	this->configs.clear();

	for (auto& entry : std::filesystem::directory_iterator(("C:\\source\\cfg"))) {
		if (entry.path().extension() == (".ini")) {
			auto path = entry.path();
			auto filename = path.filename().string();
			if (filename != "csgo_keys.ini") {
				this->configspathes.push_back(path);
				this->configs.push_back(filename);
			}
		}
	}
}

int c_lua::get_script_id(std::string name) {
	for (int i = 0; i < this->scripts.size(); i++) {
		if (this->scripts.at(i) == name)
			return i;
	}

	return -1;
}

int c_lua::get_script_id_by_path(std::string path) {
	for (int i = 0; i < this->pathes.size(); i++) {
		if (this->pathes.at(i).string() == path)
			return i;
	}

	return -1;
}

std::string c_lua::get_script_path(std::string name) {
	return this->get_script_path(this->get_script_id(name));
}

std::string c_lua::get_script_path(int id) {
	if (id == -1)
		return  "";

	return this->pathes.at(id).string();
}
