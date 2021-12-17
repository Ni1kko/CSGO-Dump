#include "../features.hpp"
#include "../../../security/xor.h"
void misc::bunny_hop(c_usercmd* cmd) {
	if (!variables.misc.bunny_hop)
		return;

	static int hops_restricted = 0;
	static int hops_hit = 0;
	if (!(cmd->buttons & in_jump)
		|| (csgo::local_player->move_type() & movetype_ladder))
		return;
	if (!(csgo::local_player->flags() & fl_onground))
	{
		cmd->buttons &= ~in_jump;
		hops_restricted = 0;
	}
	else if ((rand() % 100 > variables.misc.bunny_hop_chance			//chance of hitting first hop is always the same, the 2nd part is that so it always doesn't rape your speed
		&& hops_restricted < variables.misc.bunny_hop_max_restricted)	//the same amount, it can be made a constant if you want to or can be removed, up to you
		|| (variables.misc.bunny_hop_max_landed > 0							//force fuck up after certain amount of hops to look more legit, you could add variance to this and
			&& hops_hit > variables.misc.bunny_hop_max_landed))				//everything but fuck off that's too much customisation in my opinion, i only added this one because prof told me to
	{
		cmd->buttons &= ~in_jump;
		hops_restricted++;
		hops_hit = 0;
	}
	else
		hops_hit++;
};

void set_clantag(const char* tag)
{
	static auto ret = (int(__fastcall*)(const char*, const char*))utilities::pattern_scan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

	ret(tag, tag);
}

void misc::clantag() {
	static bool reset = false;
	static float old_curtime = interfaces::globals->cur_time;

	if (!variables.misc.clantag && reset == false) {
		reset = true;
		set_clantag("plague ");
	}
	else if (variables.misc.clantag) {
		if (interfaces::globals->cur_time > old_curtime + 1.f) {
			set_clantag("69");
			old_curtime = interfaces::globals->cur_time;
		}
		reset = false;
	}
}

void misc::chatspam() {
	static float old_curtime = interfaces::globals->cur_time;
	if (variables.misc.chatspam) {
		if (interfaces::globals->cur_time > old_curtime + 2.f) {
			interfaces::engine->execute_cmd("say retard)");
			old_curtime = interfaces::globals->cur_time;
		}
	}
}

void misc::disable_post_processing() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_postprocess_enable");
	mat_postprocess_enable->set_value(variables.misc.disable_post_processing ? 0 : 1);
}


void misc::nade_predict() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("cl_grenadepreview");
	mat_postprocess_enable->set_value(variables.misc.nade_predict ? 0 : 1);
}

void misc::disable_shadow() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("cl_csm_enabled");
	mat_postprocess_enable->set_value(variables.misc.disable_shadow ? 0 : 1);
}

void misc::remove_3dsky() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("r_3dsky");
	mat_postprocess_enable->set_value(variables.misc.remove_3dsky ? 0 : 1);
}

void misc::remove_bloom() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_disable_bloom");
	mat_postprocess_enable->set_value(variables.misc.remove_bloom ? 0 : 1);
}








