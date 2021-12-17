#include "config.hpp"
#include "../../dependencies/simpleini.hpp"

variables_t variables;

CSimpleIniA cfg;

const char* config_files[] = {
	"PlagueCfg.cfg",
	"PlagueCfg.cfg2",
	"PlagueCfg.cfg3",
	"PlagueCfg.cfg4",
	"PlagueCfg.cfg5"
};

void config::save()
{
	cfg.SetUnicode(config_files[variables.misc.config_file]);

	cfg.SetBoolValue("Aimbot", "Mode", variables.aimbot_mode);

	cfg.SetBoolValue("Aimbot", "Ignore Flash", variables.aimbot.ignore_flash);
	cfg.SetBoolValue("Aimbot", "Ignore Smoke", variables.aimbot.ignore_smoke);

	cfg.SetBoolValue("Aimbot", "Enabled Rifles", variables.aimbot.rifles.enabled);
	cfg.SetLongValue("Aimbot", "Mode Rifles", variables.aimbot.rifles.mode);
	cfg.SetLongValue("Aimbot", "Hitpoint Rifles", variables.aimbot.rifles.hitpoint);
	cfg.SetDoubleValue("Aimbot", "Fov Rifles", variables.aimbot.rifles.fov);
	cfg.SetDoubleValue("Aimbot", "Smooth Rifles", variables.aimbot.rifles.smooth);

	cfg.SetBoolValue("Aimbot", "Enabled Pistols", variables.aimbot.pistols.enabled);
	cfg.SetLongValue("Aimbot", "Mode Pistols", variables.aimbot.pistols.mode);
	cfg.SetLongValue("Aimbot", "Hitpoint Pistols", variables.aimbot.pistols.hitpoint);
	cfg.SetDoubleValue("Aimbot", "Fov Pistols", variables.aimbot.pistols.fov);
	cfg.SetDoubleValue("Aimbot", "Smooth Pistols", variables.aimbot.pistols.smooth);

	cfg.SetBoolValue("Aimbot", "Enabled Snipers", variables.aimbot.snipers.enabled);
	cfg.SetLongValue("Aimbot", "Mode Snipers", variables.aimbot.snipers.mode);
	cfg.SetLongValue("Aimbot", "Hitpoint Snipers", variables.aimbot.snipers.hitpoint);
	cfg.SetDoubleValue("Aimbot", "Fov Snipers", variables.aimbot.snipers.fov);
	cfg.SetDoubleValue("Aimbot", "Smooth Snipers", variables.aimbot.snipers.smooth);

	cfg.SetBoolValue("Backtracking", "Enabled", variables.backtracking.enabled);
	cfg.SetLongValue("Backtracking", "Range", variables.backtracking.range);
	cfg.SetBoolValue("Backtracking", "Aim At Bt", variables.backtracking.aimatbt);

	cfg.SetBoolValue("Triggerbot", "rifles Enabled", variables.triggerbot.rifles.enabled);
	cfg.SetLongValue("Triggerbot", "rifles Key", variables.triggerbot.rifles.key);
	cfg.SetBoolValue("Triggerbot", "rifles Ignore Flash", variables.triggerbot.rifles.ignore_flash);
	cfg.SetBoolValue("Triggerbot", "rifles Ignore Smoke", variables.triggerbot.rifles.ignore_smoke);
	cfg.SetLongValue("Triggerbot", "rifles Hitpoint", variables.triggerbot.rifles.hitpoint);
	cfg.SetLongValue("Triggerbot", "rifles Delay", variables.triggerbot.rifles.delay);
	cfg.SetLongValue("Triggerbot", "rifles Minimum Damage", variables.triggerbot.rifles.minimim_damage);

	cfg.SetBoolValue("Triggerbot", "snipers Enabled", variables.triggerbot.snipers.enabled);
	cfg.SetLongValue("Triggerbot", "snipers Key", variables.triggerbot.snipers.key);
	cfg.SetBoolValue("Triggerbot", "snipers Ignore Flash", variables.triggerbot.snipers.ignore_flash);
	cfg.SetBoolValue("Triggerbot", "snipers Ignore Smoke", variables.triggerbot.snipers.ignore_smoke);
	cfg.SetLongValue("Triggerbot", "snipers Hitpoint", variables.triggerbot.snipers.hitpoint);
	cfg.SetLongValue("Triggerbot", "snipers Delay", variables.triggerbot.snipers.delay);
	cfg.SetLongValue("Triggerbot", "snipers Minimum Damage", variables.triggerbot.snipers.minimim_damage);

	cfg.SetBoolValue("Triggerbot", "pistols Enabled", variables.triggerbot.pistols.enabled);
	cfg.SetLongValue("Triggerbot", "pistols Key", variables.triggerbot.pistols.key);
	cfg.SetBoolValue("Triggerbot", "pistols Ignore Flash", variables.triggerbot.pistols.ignore_flash);
	cfg.SetBoolValue("Triggerbot", "pistols Ignore Smoke", variables.triggerbot.pistols.ignore_smoke);
	cfg.SetLongValue("Triggerbot", "pistols Hitpoint", variables.triggerbot.pistols.hitpoint);
	cfg.SetLongValue("Triggerbot", "pistols Delay", variables.triggerbot.pistols.delay);
	cfg.SetLongValue("Triggerbot", "pistols Minimum Damage", variables.triggerbot.pistols.minimim_damage);

	cfg.SetBoolValue("Player Esp", "Dead Only", variables.pl_esp.dead_only);
	cfg.SetLongValue("Player Esp", "Keybind", variables.pl_esp.key_bind);
	cfg.SetBoolValue("Player Esp", "Box", variables.pl_esp.box);
	cfg.SetBoolValue("Player Esp", "Name", variables.pl_esp.name);
	cfg.SetBoolValue("Player Esp", "Weapon", variables.pl_esp.weapon);
	cfg.SetBoolValue("Player Esp", "Health", variables.pl_esp.health);
	cfg.SetBoolValue("Player Esp", "Ammo", variables.pl_esp.ammo);
	cfg.SetBoolValue("Player Esp", "Skeleton", variables.pl_esp.skeleton);

	cfg.SetBoolValue("Player Models", "Glow", variables.pl_mdl.glow.enabled);
	cfg.SetLongValue("Player Models", "Glow Bloom", variables.pl_mdl.glow.bloom);

	cfg.SetBoolValue("Player Models", "Chams", variables.pl_mdl.chams.enabled);
	cfg.SetBoolValue("Player Models", "Chams Flat", variables.pl_mdl.chams.flat);
	cfg.SetBoolValue("Player Models", "Chams IgnoreZ", variables.pl_mdl.chams.ignore_z);
	cfg.SetBoolValue("Player Models", "Chams Backtrack", variables.pl_mdl.chams.backtrack);
	cfg.SetBoolValue("Player Models", "Chams Glow Overlay", variables.pl_mdl.chams.glow_overlay);

	cfg.SetLongValue("Player Models", "Chams Transparency", variables.pl_mdl.chams.transparency);
	cfg.SetLongValue("Player Models", "Chams Backtrack Transparency", variables.pl_mdl.chams.transparency_backtrack);
	cfg.SetLongValue("Player Models", "Chams Glow Transparency", variables.pl_mdl.chams.transparency_glow);

	for (int i = 0; i < 3; i++)
	{
		cfg.SetDoubleValue("Player Esp", std::string(std::string("Box Color") + std::to_string(i)).c_str(), variables.pl_esp.box_clr[i]);
		cfg.SetDoubleValue("Player Esp", std::string(std::string("Name Color") + std::to_string(i)).c_str(), variables.pl_esp.name_clr[i]);
		cfg.SetDoubleValue("Player Esp", std::string(std::string("Weapon Color") + std::to_string(i)).c_str(), variables.pl_esp.weapon_clr[i]);
		cfg.SetDoubleValue("Player Esp", std::string(std::string("Ammo Color") + std::to_string(i)).c_str(), variables.pl_esp.ammo_clr[i]);
		cfg.SetDoubleValue("Player Esp", std::string(std::string("Skeleton Color") + std::to_string(i)).c_str(), variables.pl_esp.skeleton_clr[i]);


		cfg.SetDoubleValue("Player Models", std::string(std::string("Glow Color") + std::to_string(i)).c_str(), variables.pl_mdl.glow.clr[i]);
		cfg.SetDoubleValue("Player Models", std::string(std::string("Chams Color") + std::to_string(i)).c_str(), variables.pl_mdl.chams.clr[i]);
		cfg.SetDoubleValue("Player Models", std::string(std::string("Chams ZColor") + std::to_string(i)).c_str(), variables.pl_mdl.chams.clr_z[i]);
		cfg.SetDoubleValue("Player Models", std::string(std::string("Backtrack Color") + std::to_string(i)).c_str(), variables.pl_mdl.chams.clr_backtrack[i]);
		cfg.SetDoubleValue("Player Models", std::string(std::string("Glow Overlay Color") + std::to_string(i)).c_str(), variables.pl_mdl.chams.glow_clr[i]);

		cfg.SetDoubleValue("Antiaim", std::string(std::string("Desync Color1") + std::to_string(i)).c_str(), variables.misc.desyncclr1[i]);
		cfg.SetDoubleValue("Antiaim", std::string(std::string("Desync Color2") + std::to_string(i)).c_str(), variables.misc.desyncclr2[i]);

		cfg.SetLongValue("Misc", std::string(std::string("Viewmodel") + std::to_string(i)).c_str(), variables.misc.viewmodel[i]);
	}

	cfg.SetLongValue("Visuals", "Fov", variables.visuals.fov);
	cfg.SetLongValue("Visuals", "Viewmodel Fov", variables.visuals.viewmodel_fov);
	cfg.SetLongValue("Visuals", "Nightmode", variables.visuals.nightmode);
	cfg.SetBoolValue("Visuals", "Sniper Crosshair", variables.visuals.sniper_crosshair);
	cfg.SetBoolValue("Visuals", "Bomb Timer", variables.visuals.bomb_timer);

	cfg.SetBoolValue("Misc", "Bhop", variables.misc.bunny_hop);
	cfg.SetLongValue("Misc", "Bhop Chance", variables.misc.bunny_hop_chance);
	cfg.SetLongValue("Misc", "Bhop Max Landed", variables.misc.bunny_hop_max_landed);
	cfg.SetLongValue("Misc", "Bhop Max Restricted", variables.misc.bunny_hop_max_restricted);
	cfg.SetBoolValue("Misc", "Radar", variables.misc.radar);
	cfg.SetBoolValue("Misc", "Reveal Ranks", variables.misc.reveal_ranks);
	cfg.SetBoolValue("Misc", "Antiaim", variables.misc.antiaim);
	cfg.SetBoolValue("Misc", "Antiaim Arrows", variables.misc.antiaim_arrows);
	cfg.SetBoolValue("Misc", "Thirdperson", variables.misc.thirdperson);
	cfg.SetLongValue("Misc", "Antiaim Key", variables.misc.antiaim_invert);
	cfg.SetLongValue("Misc", "Thirdperson Key", variables.misc.thirdperson_key);
	cfg.SetBoolValue("Misc", "Clantag", variables.misc.clantag);
	cfg.SetBoolValue("Misc", "Chatspam", variables.misc.chatspam);
	cfg.SetBoolValue("Misc", "Spectator List", variables.misc.speclist);

	cfg.SetBoolValue("Misc", "infiniteduck", variables.misc.infduck);
	cfg.SetBoolValue("Misc", "edgejump", variables.misc.bEdgeJump);
	cfg.SetBoolValue("Misc", "DisablePostProcessing", variables.misc.disable_post_processing);
	cfg.SetBoolValue("Misc", "nadepredict", variables.misc.nade_predict);
	cfg.SetBoolValue("Misc", "disableshadow", variables.misc.disable_shadow);
	cfg.SetBoolValue("Misc", "disable3dsky", variables.misc.remove_3dsky);
	cfg.SetBoolValue("Misc", "disablebloom", variables.misc.remove_bloom);

	cfg.SetBoolValue("Skins", "Enabled", variables.skins.enabled);
	cfg.SetLongValue("Skins", "Knife Model", variables.skins.knife_model);
	cfg.SetLongValue("Skins", "Knife Skin", variables.skins.knife_skin);
	cfg.SetLongValue("Skins", "Glove Model", variables.skins.glove_model);
	cfg.SetLongValue("Skins", "Hydra Skin", variables.skins.hydra_skin);
	cfg.SetLongValue("Skins", "Bloodhound Skin", variables.skins.bloodhound_skin);
	cfg.SetLongValue("Skins", "Driver Skin", variables.skins.driver_skin);
	cfg.SetLongValue("Skins", "Handwrap Skin", variables.skins.handwrap_skin);
	cfg.SetLongValue("Skins", "Moto Skin", variables.skins.moto_skin);
	cfg.SetLongValue("Skins", "Specialist Skin", variables.skins.specialist_skin);
	cfg.SetLongValue("Skins", "Sport Skin", variables.skins.sport_skin);
	cfg.SetLongValue("Skins", "Usp Skin", variables.skins.paint_kit_index_usp);
	cfg.SetLongValue("Skins", "P2000 Skin", variables.skins.paint_kit_index_p2000);
	cfg.SetLongValue("Skins", "Glock Skin", variables.skins.paint_kit_index_glock);
	cfg.SetLongValue("Skins", "P250 Skin", variables.skins.paint_kit_index_p250);
	cfg.SetLongValue("Skins", "FiveSeven Skin", variables.skins.paint_kit_index_fiveseven);
	cfg.SetLongValue("Skins", "Tec9 Skin", variables.skins.paint_kit_index_tec);
	cfg.SetLongValue("Skins", "CZ Skin", variables.skins.paint_kit_index_cz);
	cfg.SetLongValue("Skins", "Duals Skin", variables.skins.paint_kit_index_duals);
	cfg.SetLongValue("Skins", "Deagle Skin", variables.skins.paint_kit_index_deagle);
	cfg.SetLongValue("Skins", "Revolver Skin", variables.skins.paint_kit_index_revolver);
	cfg.SetLongValue("Skins", "Famas Skin", variables.skins.paint_kit_index_famas);
	cfg.SetLongValue("Skins", "Galil Skin", variables.skins.paint_kit_index_galil);
	cfg.SetLongValue("Skins", "M4A4 Skin", variables.skins.paint_kit_index_m4a4);
	cfg.SetLongValue("Skins", "M4A1 Skin", variables.skins.paint_kit_index_m4a1);
	cfg.SetLongValue("Skins", "AK47 Skin", variables.skins.paint_kit_index_ak47);
	cfg.SetLongValue("Skins", "SG553 Skin", variables.skins.paint_kit_index_sg553);
	cfg.SetLongValue("Skins", "Aug Skin", variables.skins.paint_kit_index_aug);
	cfg.SetLongValue("Skins", "SSG08 Skin", variables.skins.paint_kit_index_ssg08);
	cfg.SetLongValue("Skins", "AWP Skin", variables.skins.paint_kit_index_awp);
	cfg.SetLongValue("Skins", "SCAR Skin", variables.skins.paint_kit_index_scar);
	cfg.SetLongValue("Skins", "G3SG1 Skin", variables.skins.paint_kit_index_g3sg1);
	cfg.SetLongValue("Skins", "Sawedoff Skin", variables.skins.paint_kit_index_sawoff);
	cfg.SetLongValue("Skins", "M249 Skin", variables.skins.paint_kit_index_m249);
	cfg.SetLongValue("Skins", "Negev Skin", variables.skins.paint_kit_index_negev);
	cfg.SetLongValue("Skins", "Mag7 Skin", variables.skins.paint_kit_index_mag7);
	cfg.SetLongValue("Skins", "XM1014 Skin", variables.skins.paint_kit_index_xm1014);
	cfg.SetLongValue("Skins", "Nova Skin", variables.skins.paint_kit_index_nova);
	cfg.SetLongValue("Skins", "Bizon Skin", variables.skins.paint_kit_index_bizon);
	cfg.SetLongValue("Skins", "MP5 Skin", variables.skins.paint_kit_index_mp5sd);
	cfg.SetLongValue("Skins", "MP7 Skin", variables.skins.paint_kit_index_mp7);
	cfg.SetLongValue("Skins", "MP9 Skin", variables.skins.paint_kit_index_mp9);
	cfg.SetLongValue("Skins", "MAC10 Skin", variables.skins.paint_kit_index_mac10);
	cfg.SetLongValue("Skins", "P90 Skin", variables.skins.paint_kit_index_p90);
	cfg.SetLongValue("Skins", "UMP Skin", variables.skins.paint_kit_index_ump45);

	cfg.SaveFile(config_files[variables.misc.config_file]);
}
void config::load()
{
	cfg.SetUnicode(config_files[variables.misc.config_file]);
	cfg.LoadFile(config_files[variables.misc.config_file]);

	variables.aimbot_mode = cfg.GetBoolValue("Aimbot", "Mode", variables.aimbot_mode);

	variables.aimbot.ignore_flash = cfg.GetBoolValue("Aimbot", "Ignore Flash", variables.aimbot.ignore_flash);
	variables.aimbot.ignore_smoke = cfg.GetBoolValue("Aimbot", "Ignore Smoke", variables.aimbot.ignore_smoke);

	variables.aimbot.rifles.enabled = cfg.GetBoolValue("Aimbot", "Enabled Rifles", variables.aimbot.rifles.enabled);
	variables.aimbot.rifles.mode = cfg.GetLongValue("Aimbot", "Mode Rifles", variables.aimbot.rifles.mode);
	variables.aimbot.rifles.hitpoint = cfg.GetLongValue("Aimbot", "Hitpoint Rifles", variables.aimbot.rifles.hitpoint);
	variables.aimbot.rifles.fov = cfg.GetDoubleValue("Aimbot", "Fov Rifles", variables.aimbot.rifles.fov);
	variables.aimbot.rifles.smooth = cfg.GetDoubleValue("Aimbot", "Smooth Rifles", variables.aimbot.rifles.smooth);

	variables.aimbot.pistols.enabled = cfg.GetBoolValue("Aimbot", "Enabled Pistols", variables.aimbot.pistols.enabled);
	variables.aimbot.pistols.mode = cfg.GetLongValue("Aimbot", "Mode Pistols", variables.aimbot.pistols.mode);
	variables.aimbot.pistols.hitpoint = cfg.GetLongValue("Aimbot", "Hitpoint Pistols", variables.aimbot.pistols.hitpoint);
	variables.aimbot.pistols.fov = cfg.GetDoubleValue("Aimbot", "Fov Pistols", variables.aimbot.pistols.fov);
	variables.aimbot.pistols.smooth = cfg.GetDoubleValue("Aimbot", "Smooth Pistols", variables.aimbot.pistols.smooth);

	variables.aimbot.snipers.enabled = cfg.GetBoolValue("Aimbot", "Enabled Snipers", variables.aimbot.snipers.enabled);
	variables.aimbot.snipers.mode = cfg.GetLongValue("Aimbot", "Mode Snipers", variables.aimbot.snipers.mode);
	variables.aimbot.snipers.hitpoint = cfg.GetLongValue("Aimbot", "Hitpoint Snipers", variables.aimbot.snipers.hitpoint);
	variables.aimbot.snipers.fov = cfg.GetDoubleValue("Aimbot", "Fov Snipers", variables.aimbot.snipers.fov);
	variables.aimbot.snipers.smooth = cfg.GetDoubleValue("Aimbot", "Smooth Snipers", variables.aimbot.snipers.smooth);

	variables.backtracking.enabled = cfg.GetBoolValue("Backtracking", "Enabled", variables.backtracking.enabled);
	variables.backtracking.range = cfg.GetLongValue("Backtracking", "Range", variables.backtracking.range);
	variables.backtracking.aimatbt = cfg.GetBoolValue("Backtracking", "Aim At Bt", variables.backtracking.aimatbt);
	
	variables.triggerbot.rifles.enabled = cfg.GetBoolValue("Triggerbot", "rifles Enabled", variables.triggerbot.rifles.enabled);
	variables.triggerbot.rifles.key = cfg.GetLongValue("Triggerbot", "rifles Key", variables.triggerbot.rifles.key);
	variables.triggerbot.rifles.ignore_flash = cfg.GetBoolValue("Triggerbot", "rifles Ignore Flash", variables.triggerbot.rifles.ignore_flash);
	variables.triggerbot.rifles.ignore_smoke = cfg.GetBoolValue("Triggerbot", "rifles Ignore Smoke", variables.triggerbot.rifles.ignore_smoke);
	variables.triggerbot.rifles.hitpoint = cfg.GetLongValue("Triggerbot", "rifles Hitpoint", variables.triggerbot.rifles.hitpoint);
	variables.triggerbot.rifles.delay = cfg.GetLongValue("Triggerbot", "rifles Delay", variables.triggerbot.rifles.delay);
	variables.triggerbot.rifles.minimim_damage = cfg.GetLongValue("Triggerbot", "rifles Minimum Damage", variables.triggerbot.rifles.minimim_damage);

	variables.triggerbot.snipers.enabled = cfg.GetBoolValue("Triggerbot", "snipers Enabled", variables.triggerbot.snipers.enabled);
	variables.triggerbot.snipers.key = cfg.GetLongValue("Triggerbot", "snipers Key", variables.triggerbot.snipers.key);
	variables.triggerbot.snipers.ignore_flash = cfg.GetBoolValue("Triggerbot", "snipers Ignore Flash", variables.triggerbot.snipers.ignore_flash);
	variables.triggerbot.snipers.ignore_smoke = cfg.GetBoolValue("Triggerbot", "snipers Ignore Smoke", variables.triggerbot.snipers.ignore_smoke);
	variables.triggerbot.snipers.hitpoint = cfg.GetLongValue("Triggerbot", "snipers Hitpoint", variables.triggerbot.snipers.hitpoint);
	variables.triggerbot.snipers.delay = cfg.GetLongValue("Triggerbot", "snipers Delay", variables.triggerbot.snipers.delay);
	variables.triggerbot.snipers.minimim_damage = cfg.GetLongValue("Triggerbot", "snipers Minimum Damage", variables.triggerbot.snipers.minimim_damage);

	variables.triggerbot.pistols.enabled = cfg.GetBoolValue("Triggerbot", "pistols Enabled", variables.triggerbot.pistols.enabled);
	variables.triggerbot.pistols.key = cfg.GetLongValue("Triggerbot", "pistols Key", variables.triggerbot.pistols.key);
	variables.triggerbot.pistols.ignore_flash = cfg.GetBoolValue("Triggerbot", "pistols Ignore Flash", variables.triggerbot.pistols.ignore_flash);
	variables.triggerbot.pistols.ignore_smoke = cfg.GetBoolValue("Triggerbot", "pistols Ignore Smoke", variables.triggerbot.pistols.ignore_smoke);
	variables.triggerbot.pistols.hitpoint = cfg.GetLongValue("Triggerbot", "pistols Hitpoint", variables.triggerbot.pistols.hitpoint);
	variables.triggerbot.pistols.delay = cfg.GetLongValue("Triggerbot", "pistols Delay", variables.triggerbot.pistols.delay);
	variables.triggerbot.pistols.minimim_damage = cfg.GetLongValue("Triggerbot", "pistols Minimum Damage", variables.triggerbot.pistols.minimim_damage);

	variables.pl_esp.dead_only = cfg.GetBoolValue("Player Esp", "Dead Only", variables.pl_esp.dead_only);
	variables.pl_esp.key_bind = cfg.GetLongValue("Player Esp", "Keybind", variables.pl_esp.key_bind);
	variables.pl_esp.box = cfg.GetBoolValue("Player Esp", "Box", variables.pl_esp.box);
	variables.pl_esp.name = cfg.GetBoolValue("Player Esp", "Name", variables.pl_esp.name);
	variables.pl_esp.weapon = cfg.GetBoolValue("Player Esp", "Weapon", variables.pl_esp.weapon);
	variables.pl_esp.health = cfg.GetBoolValue("Player Esp", "Health", variables.pl_esp.health);
	variables.pl_esp.ammo = cfg.GetBoolValue("Player Esp", "Ammo", variables.pl_esp.ammo);
	variables.pl_esp.skeleton = cfg.GetBoolValue("Player Esp", "Skeleton", variables.pl_esp.skeleton);

	variables.pl_mdl.glow.enabled = cfg.GetBoolValue("Player Models", "Glow", variables.pl_mdl.glow.enabled);
	variables.pl_mdl.glow.bloom = cfg.GetLongValue("Player Models", "Glow Bloom", variables.pl_mdl.glow.bloom);

	variables.pl_mdl.chams.enabled = cfg.GetBoolValue("Player Models", "Chams", variables.pl_mdl.chams.enabled);
	variables.pl_mdl.chams.flat = cfg.GetBoolValue("Player Models", "Chams Flat", variables.pl_mdl.chams.flat);
	variables.pl_mdl.chams.ignore_z = cfg.GetBoolValue("Player Models", "Chams IgnoreZ", variables.pl_mdl.chams.ignore_z);
	variables.pl_mdl.chams.backtrack = cfg.GetBoolValue("Player Models", "Chams Backtrack", variables.pl_mdl.chams.backtrack);
	variables.pl_mdl.chams.glow_overlay = cfg.GetBoolValue("Player Models", "Chams Glow Overlay", variables.pl_mdl.chams.glow_overlay);

	variables.pl_mdl.chams.transparency = cfg.GetLongValue("Player Models", "Chams Transparency", variables.pl_mdl.chams.transparency);
	variables.pl_mdl.chams.transparency_backtrack = cfg.GetLongValue("Player Models", "Chams Backtrack Transparency", variables.pl_mdl.chams.transparency_backtrack);
	variables.pl_mdl.chams.transparency_glow = cfg.GetLongValue("Player Models", "Chams Glow Transparency", variables.pl_mdl.chams.transparency_glow);

	for (int i = 0; i < 3; i++)
	{
		variables.pl_esp.box_clr[i] = cfg.GetDoubleValue("Player Esp", std::string(std::string("Box Color") + std::to_string(i)).c_str(), variables.pl_esp.box_clr[i]);
		variables.pl_esp.name_clr[i] = cfg.GetDoubleValue("Player Esp", std::string(std::string("Name Color") + std::to_string(i)).c_str(), variables.pl_esp.name_clr[i]);
		variables.pl_esp.weapon_clr[i] = cfg.GetDoubleValue("Player Esp", std::string(std::string("Weapon Color") + std::to_string(i)).c_str(), variables.pl_esp.weapon_clr[i]);
		variables.pl_esp.ammo_clr[i] = cfg.GetDoubleValue("Player Esp", std::string(std::string("Ammo Color") + std::to_string(i)).c_str(), variables.pl_esp.ammo_clr[i]);
		variables.pl_esp.skeleton_clr[i] = cfg.GetDoubleValue("Player Esp", std::string(std::string("Skeleton Color") + std::to_string(i)).c_str(), variables.pl_esp.skeleton_clr[i]);

		variables.pl_mdl.glow.clr[i] = cfg.GetDoubleValue("Player Models", std::string(std::string("Glow Color") + std::to_string(i)).c_str(), variables.pl_mdl.glow.clr[i]);
		variables.pl_mdl.chams.clr[i] = cfg.GetDoubleValue("Player Models", std::string(std::string("Chams Color") + std::to_string(i)).c_str(), variables.pl_mdl.chams.clr[i]);
		variables.pl_mdl.chams.clr_z[i] = cfg.GetDoubleValue("Player Models", std::string(std::string("Chams ZColor") + std::to_string(i)).c_str(), variables.pl_mdl.chams.clr_z[i]);
		variables.pl_mdl.chams.clr_backtrack[i] = cfg.GetDoubleValue("Player Models", std::string(std::string("Backtrack Color") + std::to_string(i)).c_str(), variables.pl_mdl.chams.clr_backtrack[i]);
		variables.pl_mdl.chams.glow_clr[i] = cfg.GetDoubleValue("Player Models", std::string(std::string("Glow Overlay Color") + std::to_string(i)).c_str(), variables.pl_mdl.chams.glow_clr[i]); 
		
		variables.misc.desyncclr1[i] = cfg.GetDoubleValue("Antiaim", std::string(std::string("Desync Color1") + std::to_string(i)).c_str(), variables.misc.desyncclr1[i]);
		variables.misc.desyncclr2[i] = cfg.GetDoubleValue("Antiaim", std::string(std::string("Desync Color2") + std::to_string(i)).c_str(), variables.misc.desyncclr2[i]);

		variables.misc.viewmodel[i] = cfg.GetLongValue("Misc", std::string(std::string("Viewmodel") + std::to_string(i)).c_str(), variables.misc.viewmodel[i]);
	}

	variables.visuals.fov = cfg.GetLongValue("Visuals", "Fov", variables.visuals.fov);
	variables.visuals.viewmodel_fov = cfg.GetLongValue("Visuals", "Viewmodel Fov", variables.visuals.viewmodel_fov);
	variables.visuals.nightmode = cfg.GetLongValue("Visuals", "Nightmode", variables.visuals.nightmode);
	variables.visuals.sniper_crosshair = cfg.GetBoolValue("Visuals", "Sniper Crosshair", variables.visuals.sniper_crosshair);
	variables.visuals.bomb_timer = cfg.GetBoolValue("Visuals", "Bomb Timer", variables.visuals.bomb_timer);

	variables.misc.bunny_hop = cfg.GetBoolValue("Misc", "Bhop", variables.misc.bunny_hop);
	variables.misc.bunny_hop_chance = cfg.GetLongValue("Misc", "Bhop Chance", variables.misc.bunny_hop_chance);
	variables.misc.bunny_hop_max_landed = cfg.GetLongValue("Misc", "Bhop Max Landed", variables.misc.bunny_hop_max_landed);
	variables.misc.bunny_hop_max_restricted = cfg.GetLongValue("Misc", "Bhop Max Restricted", variables.misc.bunny_hop_max_restricted);
	variables.misc.radar = cfg.GetBoolValue("Misc", "Radar", variables.misc.radar);
	variables.misc.reveal_ranks = cfg.GetBoolValue("Misc", "Reveal Ranks", variables.misc.reveal_ranks);
	variables.misc.antiaim = cfg.GetBoolValue("Misc", "Antiaim", variables.misc.antiaim);
	variables.misc.antiaim_arrows = cfg.GetBoolValue("Misc", "Antiaim Arrows", variables.misc.antiaim_arrows);
	variables.misc.thirdperson = cfg.GetBoolValue("Misc", "Thirdperson", variables.misc.thirdperson);
	variables.misc.antiaim_invert = cfg.GetLongValue("Misc", "Antiaim Key", variables.misc.antiaim_invert);
	variables.misc.thirdperson_key = cfg.GetLongValue("Misc", "Thirdperson Key", variables.misc.thirdperson_key);
	variables.misc.clantag = cfg.GetBoolValue("Misc", "Clantag", variables.misc.clantag);
	variables.misc.chatspam = cfg.GetBoolValue("Misc", "Chatspam", variables.misc.chatspam);
	variables.misc.speclist = cfg.GetBoolValue("Misc", "Spectator List", variables.misc.speclist);

	variables.misc.infduck = cfg.GetBoolValue("Misc", "infiniteduck", variables.misc.infduck);
	variables.misc.bEdgeJump = cfg.GetBoolValue("Misc", "edgejump", variables.misc.bEdgeJump);
	variables.misc.disable_post_processing = cfg.GetBoolValue("Misc", "DisablePostProcessing", variables.misc.disable_post_processing);
	variables.misc.nade_predict = cfg.GetBoolValue("Misc", "nadepredict", variables.misc.nade_predict);
	variables.misc.disable_shadow = cfg.GetBoolValue("Misc", "disableshadow", variables.misc.disable_shadow);
	variables.misc.remove_3dsky = cfg.GetBoolValue("Misc", "disable3dsky", variables.misc.remove_3dsky);
	variables.misc.remove_bloom = cfg.GetBoolValue("Misc", "disablebloom", variables.misc.remove_bloom);


	variables.skins.enabled = cfg.GetBoolValue("Skins", "Enabled", variables.skins.enabled);
	variables.skins.knife_model = cfg.GetLongValue("Skins", "Knife Model", variables.skins.knife_model);
	variables.skins.knife_skin = cfg.GetLongValue("Skins", "Knife Skin", variables.skins.knife_skin);
	variables.skins.glove_model = cfg.GetLongValue("Skins", "Glove Model", variables.skins.glove_model);
	variables.skins.hydra_skin = cfg.GetLongValue("Skins", "Hydra Skin", variables.skins.hydra_skin);
	variables.skins.bloodhound_skin = cfg.GetLongValue("Skins", "Bloodhound Skin", variables.skins.bloodhound_skin);
	variables.skins.driver_skin = cfg.GetLongValue("Skins", "Driver Skin", variables.skins.driver_skin);
	variables.skins.handwrap_skin = cfg.GetLongValue("Skins", "Handwrap Skin", variables.skins.handwrap_skin);
	variables.skins.moto_skin = cfg.GetLongValue("Skins", "Moto Skin", variables.skins.moto_skin);
	variables.skins.specialist_skin = cfg.GetLongValue("Skins", "Specialist Skin", variables.skins.specialist_skin);
	variables.skins.sport_skin = cfg.GetLongValue("Skins", "Sport Skin", variables.skins.sport_skin);
	variables.skins.paint_kit_index_usp = cfg.GetLongValue("Skins", "Usp Skin", variables.skins.paint_kit_index_usp);
	variables.skins.paint_kit_index_p2000 = cfg.GetLongValue("Skins", "P2000 Skin", variables.skins.paint_kit_index_p2000);
	variables.skins.paint_kit_index_glock = cfg.GetLongValue("Skins", "Glock Skin", variables.skins.paint_kit_index_glock);
	variables.skins.paint_kit_index_p250 = cfg.GetLongValue("Skins", "P250 Skin", variables.skins.paint_kit_index_p250);
	variables.skins.paint_kit_index_fiveseven = cfg.GetLongValue("Skins", "FiveSeven Skin", variables.skins.paint_kit_index_fiveseven);
	variables.skins.paint_kit_index_tec = cfg.GetLongValue("Skins", "Tec9 Skin", variables.skins.paint_kit_index_tec);
	variables.skins.paint_kit_index_cz = cfg.GetLongValue("Skins", "CZ Skin", variables.skins.paint_kit_index_cz);
	variables.skins.paint_kit_index_duals = cfg.GetLongValue("Skins", "Duals Skin", variables.skins.paint_kit_index_duals);
	variables.skins.paint_kit_index_deagle = cfg.GetLongValue("Skins", "Deagle Skin", variables.skins.paint_kit_index_deagle);
	variables.skins.paint_kit_index_revolver = cfg.GetLongValue("Skins", "Revolver Skin", variables.skins.paint_kit_index_revolver);
	variables.skins.paint_kit_index_famas = cfg.GetLongValue("Skins", "Famas Skin", variables.skins.paint_kit_index_famas);
	variables.skins.paint_kit_index_galil = cfg.GetLongValue("Skins", "Galil Skin", variables.skins.paint_kit_index_galil);
	variables.skins.paint_kit_index_m4a4 = cfg.GetLongValue("Skins", "M4A4 Skin", variables.skins.paint_kit_index_m4a4);
	variables.skins.paint_kit_index_m4a1 = cfg.GetLongValue("Skins", "M4A1 Skin", variables.skins.paint_kit_index_m4a1);
	variables.skins.paint_kit_index_ak47 = cfg.GetLongValue("Skins", "AK47 Skin", variables.skins.paint_kit_index_ak47);
	variables.skins.paint_kit_index_sg553 = cfg.GetLongValue("Skins", "SG553 Skin", variables.skins.paint_kit_index_sg553);
	variables.skins.paint_kit_index_aug = cfg.GetLongValue("Skins", "Aug Skin", variables.skins.paint_kit_index_aug);
	variables.skins.paint_kit_index_ssg08 = cfg.GetLongValue("Skins", "SSG08 Skin", variables.skins.paint_kit_index_ssg08);
	variables.skins.paint_kit_index_awp = cfg.GetLongValue("Skins", "AWP Skin", variables.skins.paint_kit_index_awp);
	variables.skins.paint_kit_index_scar = cfg.GetLongValue("Skins", "SCAR Skin", variables.skins.paint_kit_index_scar);
	variables.skins.paint_kit_index_g3sg1 = cfg.GetLongValue("Skins", "G3SG1 Skin", variables.skins.paint_kit_index_g3sg1);
	variables.skins.paint_kit_index_sawoff = cfg.GetLongValue("Skins", "Sawedoff Skin", variables.skins.paint_kit_index_sawoff);
	variables.skins.paint_kit_index_m249 = cfg.GetLongValue("Skins", "M249 Skin", variables.skins.paint_kit_index_m249);
	variables.skins.paint_kit_index_negev = cfg.GetLongValue("Skins", "Negev Skin", variables.skins.paint_kit_index_negev);
	variables.skins.paint_kit_index_mag7 = cfg.GetLongValue("Skins", "Mag7 Skin", variables.skins.paint_kit_index_mag7);
	variables.skins.paint_kit_index_xm1014 = cfg.GetLongValue("Skins", "XM1014 Skin", variables.skins.paint_kit_index_xm1014);
	variables.skins.paint_kit_index_nova = cfg.GetLongValue("Skins", "Nova Skin", variables.skins.paint_kit_index_nova);
	variables.skins.paint_kit_index_bizon = cfg.GetLongValue("Skins", "Bizon Skin", variables.skins.paint_kit_index_bizon);
	variables.skins.paint_kit_index_mp5sd = cfg.GetLongValue("Skins", "MP5 Skin", variables.skins.paint_kit_index_mp5sd);
	variables.skins.paint_kit_index_mp7 = cfg.GetLongValue("Skins", "MP7 Skin", variables.skins.paint_kit_index_mp7);
	variables.skins.paint_kit_index_mp9 = cfg.GetLongValue("Skins", "MP9 Skin", variables.skins.paint_kit_index_mp9);
	variables.skins.paint_kit_index_mac10 = cfg.GetLongValue("Skins", "MAC10 Skin", variables.skins.paint_kit_index_mac10);
	variables.skins.paint_kit_index_p90 = cfg.GetLongValue("Skins", "P90 Skin", variables.skins.paint_kit_index_p90);
	variables.skins.paint_kit_index_ump45 = cfg.GetLongValue("Skins", "UMP Skin", variables.skins.paint_kit_index_ump45);
}