#include "fgui_menu.hpp"

void gui::initialize() {
	fgui::element_font title_font = { "Tahoma", 11, fgui::external::font_flags::SHADOW , false };
	fgui::element_font tab_font = { "Calligraphr", 30, fgui::external::font_flags::ANTIALIAS , false };

	ADD_WINDOW(vars::container["#window"], 50, 50, "worthless aka clockwise fix", 660 + 58, 449, fgui::external::key_code::KEY_INSERT, title_font, false);
	REGISTER_CURSOR(fgui::cursor_type::NONE, fgui::input_state::UNLOCKED);


	static std::int32_t screen_width, screen_height;

	fgui::render.get_screen_size(screen_width, screen_height);

	REGISTER_NOTIFICATIONS(screen_width, 25, title_font);

	fgui::handler::call_notification("[worthless aka clockwise fix] success.", fgui::animation_type::LINEAR);  // doesnt recommend having this noti. its just annoying

	REGISTER_TAB(vars::tabs["#tab_panel"], 7, 1, fgui::tab_layout::VERTICAL, tab_font, vars::container["#window"], -1);

	ADD_TAB(vars::tabs["#tab_panel"], "aimbot"); { // aimbot tab, the aimbot is laggy, but i doesnt remove it. sometimes work, but most times it doesnt.


		ADD_GROUPBOX(vars::container["#groupbox"], 169, 15, "aim box 1", 260, 393, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#enable_aim"], 15, 15, "aimbot master switch", "vars.aimbot", title_font, vars::container["#groupbox"], 0);
		ADD_COMBOBOX(vars::combobox["#weapon_settings"], 20, 45, "weapon settings", "vars.wepset", title_font, vars::container["#groupbox"], 0);


		// pistol functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "pistols", 0); {
			ADD_SLIDER(vars::slider["#pistol_fov"], 20, 85, "pistol fov(0 - 30)", 5, 0, 30, "vars.pisstolfov", title_font, vars::container["#groupbox"], 0);
			ADD_CONTROLLER(vars::slider["#pistol_fov"], vars::combobox["#weapon_settings"]);



			ADD_SLIDER(vars::slider["#pistol_smooth"], 20, 115, "pistol smoothness(0 - 20)", 5, 1, 30, "vars.pisstolsmoth", title_font, vars::container["#groupbox"], 0);
			ADD_CONTROLLER(vars::slider["#pistol_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#pistol_rcs"], 20, 145, "pistol rcs(0 - 100)", 0, 0, 100, "vars.pistolrcs", title_font, vars::container["#groupbox"], 0);
			ADD_CONTROLLER(vars::slider["#pistol_rcs"], vars::combobox["#weapon_settings"]);

		}
		


		// heavy pistol functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "heavy pistols", 1); {
			ADD_SLIDER(vars::slider["#heavy_pistol_fov"], 20, 85, "heavy pistol fov(0 - 30)", 5, 0, 30, "vars.heaevypistolfov", title_font, vars::container["#groupbox"], 1);
			ADD_CONTROLLER(vars::slider["#heavy_pistol_fov"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#heavy_pistol_smooth"], 20, 115, "heavy pistol smoothness(0 - 20)", 5, 1, 30, "vars.heaevypistolsmooth", title_font, vars::container["#groupbox"], 1);
			ADD_CONTROLLER(vars::slider["#heavy_pistol_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#heavy_pistol_rcs"], 20, 145, "heavy pistol rcs(0 - 100)", 0, 0, 100, "vars.heaevypistolrcs", title_font, vars::container["#groupbox"], 1);
			ADD_CONTROLLER(vars::slider["#heavy_pistol_rcs"], vars::combobox["#weapon_settings"]);
		}



		// rifle functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "rifles", 2); {
			ADD_SLIDER(vars::slider["#rifle_fov"], 20, 85, "rifle fov(0 - 30)", 5, 0, 30, "vars.riflefov", title_font, vars::container["#groupbox"], 2);
			ADD_CONTROLLER(vars::slider["#rifle_fov"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#rifle_smooth"], 20, 115, "rifle smoothness(0 - 20)", 5, 1, 30, "vars.riflesmooth", title_font, vars::container["#groupbox"], 2);
			ADD_CONTROLLER(vars::slider["#rifle_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#rifle_rcs"], 20, 145, "rifle rcs(0 - 100)", 0, 0, 100, "vars.riflercs", title_font, vars::container["#groupbox"], 2);
			ADD_CONTROLLER(vars::slider["#rifle_rcs"], vars::combobox["#weapon_settings"]);
		}



		// smg functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "smg", 3); {
			ADD_SLIDER(vars::slider["#smg_fov"], 20, 85, "smg fov(0 - 30)", 5, 0, 30, "vars.smgfov", title_font, vars::container["#groupbox"], 3);
			ADD_CONTROLLER(vars::slider["#smg_fov"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#smg_smooth"], 20, 115, "smg smoothness(0 - 20)", 5, 1, 30, "vars.smgsmooth", title_font, vars::container["#groupbox"], 3);
			ADD_CONTROLLER(vars::slider["#smg_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#smg_rcs"], 20, 145, "smg rcs(0 - 100)", 0, 0, 100, "vars.smgrcs", title_font, vars::container["#groupbox"], 3);
			ADD_CONTROLLER(vars::slider["#smg_rcs"], vars::combobox["#weapon_settings"]);
		}



		// sniper functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "sniper", 4); {
			ADD_SLIDER(vars::slider["#sniper_fov"], 20, 85, "sniper fov(0 - 30)", 5, 0, 30, "vars.sniperefov", title_font, vars::container["#groupbox"], 4);
			ADD_CONTROLLER(vars::slider["#sniper_fov"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#sniper_smooth"], 20, 115, "sniper smoothness(0 - 20)", 5, 1, 30, "vars.snipersmooth", title_font, vars::container["#groupbox"], 4);
			ADD_CONTROLLER(vars::slider["#sniper_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#sniper_rcs"], 20, 145, "sniper rcs(0 - 100)", 0, 0, 100, "vars.sniperrcs", title_font, vars::container["#groupbox"], 4);
			ADD_CONTROLLER(vars::slider["#sniper_rcs"], vars::combobox["#weapon_settings"]);
		}



		// scout functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "scout", 5); {
			ADD_SLIDER(vars::slider["#scout_fov"], 20, 85, "scout fov(0 - 30)", 5, 0, 30, "vars.scoutfov", title_font, vars::container["#groupbox"], 5);
			ADD_CONTROLLER(vars::slider["#scout_fov"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#scout_smooth"], 20, 115, "scout smoothness(0 - 20)", 5, 1, 30, "vars.scoutsmooth", title_font, vars::container["#groupbox"], 5);
			ADD_CONTROLLER(vars::slider["#scout_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#scout_rcs"], 20, 145, "scout rcs(0 - 100)", 0, 0, 100, "vars.scoutrcs", title_font, vars::container["#groupbox"], 5);
			ADD_CONTROLLER(vars::slider["#scout_rcs"], vars::combobox["#weapon_settings"]);
		}



		// heavy weapon functions
		ADD_ITEM(vars::combobox["#weapon_settings"], "heavy", 6); {
			ADD_SLIDER(vars::slider["#heavy_fov"], 20, 85, "heavy fov(0 - 30)", 5, 0, 30, "vars.heavyfov", title_font, vars::container["#groupbox"], 6);
			ADD_CONTROLLER(vars::slider["#heavy_fov"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#heavy_smooth"], 20, 115, "heavy smoothness(0 - 20)", 5, 1, 30, "vars.heavysmooth", title_font, vars::container["#groupbox"], 6);
			ADD_CONTROLLER(vars::slider["#heavy_smooth"], vars::combobox["#weapon_settings"]);

			ADD_SLIDER(vars::slider["#heavy_rcs"], 20, 145, "heavy rcs(0 - 100)", 0, 0, 100, "vars.heavyrcs", title_font, vars::container["#groupbox"], 6);
			ADD_CONTROLLER(vars::slider["#heavy_rcs"], vars::combobox["#weapon_settings"]);
		}



		// aimbot functions(?)
		ADD_CHECKBOX(vars::checkbox["#silent_aim"], 20, 170, "silent aim", "vars.saim", title_font, vars::container["#groupbox"], 0);
		
		ADD_GROUPBOX(vars::container["#groupbox1"], 285 + 169 - 13, 15, "aim box 2", 260, 393, title_font, vars::container["#window"], 0, false, false, false);
		ADD_CONTROLLER(vars::container["#groupbox1"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#aim_on_key"], 15, 15, "aim on key", "vars.aimonkey", title_font, vars::container["#groupbox1"], 0);
		ADD_KEYBINDER(vars::keybinder["#aim_key"], 20, 45, "aim key", 0, "vars.aimkeye", title_font, vars::container["#groupbox1"], 0);
		ADD_CHECKBOX(vars::checkbox["#backtrack"], 15, 75, "backtrack", "vars.backtrack", title_font, vars::container["#groupbox1"], 0);
		ADD_SLIDER(vars::slider["#backtrack_amount"], 20, 105, "backtrack ms(0 - 200)", 100, 1, 200, "vars.btrackamount", title_font, vars::container["#groupbox1"], 0);
	}

	ADD_TAB(vars::tabs["#tab_panel"], "visuals"); {	// visuals tab. every glitchy stuff is removed so dont worry about it.


		ADD_GROUPBOX(vars::container["#player"], 169, 15, "player box", 260, 393, title_font, vars::container["#window"], 1, false, false, false);
		ADD_CONTROLLER(vars::container["#player"], vars::tabs["#tab_panel"]);



		// esp settings
		ADD_CHECKBOX(vars::checkbox["#enable_esp"], 15, 15, "enable esp", "vars.esp", title_font, vars::container["#player"], 1);
		ADD_CHECKBOX(vars::checkbox["#team_check"], 20, 35, "draw team", "vars.teamcheck", title_font, vars::container["#player"], 1);



		// radar esp, ect.., add anything here if you want:))
		ADD_CHECKBOX(vars::checkbox["#radar_esp"], 20, 55, "radar esp", "vars.radar", title_font, vars::container["#player"], 1);
		ADD_CHECKBOX(vars::checkbox["#name_esp"], 20, 95, "name esp", "vars.nameesp", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#name_color"], 235, 93, fgui::color(255, 255, 255), "vars.namecolor", title_font, vars::container["#player"], 1);
		ADD_CHECKBOX(vars::checkbox["#weapon_esp"], 20, 195, "weapon esp", "vars.weapon", title_font, vars::container["#player"], 1);

		// box esp + color, skeleton esp + color
		ADD_CHECKBOX(vars::checkbox["#box_esp"], 20, 75, "box esp", "vars.boxesp", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#normal_box_color"], 235, 75, fgui::color(255, 255, 255, 255), "vars.norma", title_font, vars::container["#player"], 1);
		ADD_TOOLTIP(vars::colorpicker["#normal_box_color"], "normal box color");


		ADD_CHECKBOX(vars::checkbox["#skeleton_esp"], 20, 195 + 20, "skeleton esp", "vars.skeleon", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#skeleton_color"], 235, 195 + 20, fgui::color(255, 255, 255), "vars.skeletoncolor", title_font, vars::container["#player"], 1);



		// glow, glow color
		ADD_CHECKBOX(vars::checkbox["#glow"], 20, 115, "glow esp", "vars.glow", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#glow_color"], 235, 112, fgui::color(188, 77, 117, 125), "vars.glow", title_font, vars::container["#player"], 1);
		
		

		// health esp, esp styles
		ADD_CHECKBOX(vars::checkbox["#health_esp"], 20, 135, "health esp", "vars.health", title_font, vars::container["#player"], 1);
		ADD_COMBOBOX(vars::combobox["#health_esp_style"], 20, 165, "health style", "vars.heatlstyle", title_font, vars::container["#player"], 1);
		ADD_ITEM(vars::combobox["#health_esp_style"], "normal", 0);
		ADD_ITEM(vars::combobox["#health_esp_style"], "legacy", 1);




		// chams + color
		ADD_CHECKBOX(vars::checkbox["#chams_enable"], 15, 195 + 40, "chams enable", "vars.chamsenable", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#regular_chams_color"], 210, 195 + 40, fgui::color(107, 255, 211, 255), "vars.regular_chams_color", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#regular_chams_color_xqz"], 235, 195 + 40, fgui::color(107, 255, 211, 255), "vars.regular_chams_color_xqz", title_font, vars::container["#player"], 1);
		ADD_COLORPICKER(vars::colorpicker["#chams_glow_color"], 185, 195 + 40, fgui::color(255, 255, 255, 255), "vars.glow_chams", title_font, vars::container["#player"], 1);
		
		
		


		// visuals tab (in visuals tab LMFAO)
		ADD_GROUPBOX(vars::container["#visuals"], 285 + 169 - 13, 15, "visuals box", 260, 393, title_font, vars::container["#window"], 1, false, false, false);
		ADD_CONTROLLER(vars::container["#visuals"], vars::tabs["#tab_panel"]); // making the tab + controller



		// every trash i added here
		ADD_CHECKBOX(vars::checkbox["#force_xhair"], 15, 75, "force crosshair(wip)", "vars.force_crosshair", title_font, vars::container["#visuals"], 1)
		ADD_CHECKBOX(vars::checkbox["#spectator_list"], 15, 95, "spectator list", "vars.spectator_list", title_font, vars::container["#visuals"], 1);
		ADD_CHECKBOX(vars::checkbox["#rank_reveal"], 15, 115, "rank reveal", "vars.rank_reveal", title_font, vars::container["#visuals"], 1);
		ADD_CHECKBOX(vars::checkbox["#disable_post_processing"], 15, 135, "disable post processing", "vars.disable_post_processing", title_font, vars::container["#visuals"], 1);
		ADD_CHECKBOX(vars::checkbox["#no_flash"], 15, 215, "no flash", "vars.noflassh", title_font, vars::container["#visuals"], 1);
	}

	ADD_TAB(vars::tabs["#tab_panel"], "misc"); { // misc tab. bhop works fine, other things too. jumpbug is sometimes glitchy.



		ADD_GROUPBOX(vars::container["#misc1"], 169, 15, "movement tab", 260, 393, title_font, vars::container["#window"], 2, false, false, false);
		ADD_CONTROLLER(vars::container["#misc1"], vars::tabs["#tab_panel"]); // making the "box" + controller

		// its just some basic movement tab:)
		ADD_CHECKBOX(vars::checkbox["#bhop_checkbox"], 15, 15, "automatic bhop", "vars.bhop", title_font, vars::container["#misc1"], 2);
		ADD_CHECKBOX(vars::checkbox["#edgejump"], 15, 35, "automatic jump on edge", "vars.edgejump", title_font, vars::container["#misc1"], 2);
		ADD_KEYBINDER(vars::keybinder["#edgejump_key"], 15, 65, "jump on edge key", 0, "vars.edgejump_key", title_font, vars::container["#misc1"], 2);
		ADD_CHECKBOX(vars::checkbox["#jumpbug_checkbox"], 15, 95, "automatic jumpbug", "vars.jumpbug", title_font, vars::container["#misc1"], 2);
		ADD_KEYBINDER(vars::keybinder["#jumpbug_key"], 15, 125, "jumpbug key", 0, "vars.jbkey", title_font, vars::container["#misc1"], 2);
		ADD_CHECKBOX(vars::checkbox["#edgebug_checkbox"], 15, 155, "edgebug assist", "vars.eb", title_font, vars::container["#misc1"], 2);
		ADD_KEYBINDER(vars::keybinder["#edgebug_key"], 15, 185, "edgebug assist key", 0, "vars.ebkey", title_font, vars::container["#misc1"], 2);
		ADD_CHECKBOX(vars::checkbox["#fastduck"], 15, 215, "fast duck", "vars.fastudck", title_font, vars::container["#misc1"], 2);
		ADD_CHECKBOX(vars::checkbox["#ljbind"], 15, 235, "lj bind on edge jump", "vars.ljbing", title_font, vars::container["#misc1"], 2);
		ADD_COMBOBOX(vars::combobox["#ljbind_type"], 15, 265, "bind type", "vars.bindtype", title_font, vars::container["#misc1"], 2);
		ADD_ITEM(vars::combobox["#ljbind_type"], "none", 0);
		ADD_ITEM(vars::combobox["#ljbind_type"], "-w", 1);
		ADD_ITEM(vars::combobox["#ljbind_type"], "-s", 2);
		ADD_ITEM(vars::combobox["#ljbind_type"], "-a", 3);
		ADD_ITEM(vars::combobox["#ljbind_type"], "-d", 4);
		ADD_CHECKBOX(vars::checkbox["#strafe_in_air"], 15, 295, "strafe in air", "vars.strafeinair", title_font, vars::container["#misc1"], 2);

		ADD_GROUPBOX(vars::container["#misc2"], 285 + 169 - 13, 15, "more visuals", 260, 393, title_font, vars::container["#window"], 2, false, false, false);
		ADD_CONTROLLER(vars::container["#misc2"], vars::tabs["#tab_panel"]);


		// velocity graph (visualize your movement speed)
		ADD_CHECKBOX(vars::checkbox["#drawvel"], 15, 15, "draw velocity", "vars.drawvel", title_font, vars::container["#misc2"], 2);
		ADD_CHECKBOX(vars::checkbox["#drawvelpre"], 15, 35, "draw velocity show prespeed", "vars.drawvelshowpre", title_font, vars::container["#misc2"], 2);
		ADD_SLIDER(vars::slider["#drawvel_amount"], 15, 45 + 20, "velocity indicator offset", 140, 0, 500, "vars.drwavely", title_font, vars::container["#misc2"], 2);


		// make sure to turn on indicators if you wanna see the texts:)
		ADD_CHECKBOX(vars::checkbox["#indicators"], 15, 105 + 20, "indicators", "vars.indi", title_font, vars::container["#misc2"], 2);
		ADD_COMBOBOX(vars::combobox["#draw_vel_style"], 15, 195 + 70, "draw velocity style", "vars.drawvelsstyle", title_font, vars::container["#misc2"], 2);
		ADD_ITEM(vars::combobox["#draw_vel_style"], "normal", 0);
		ADD_ITEM(vars::combobox["#draw_vel_style"], "simplekz", 1);

	}

	ADD_TAB(vars::tabs["#tab_panel"], "skin changer"); { // skin changer, doesnt finished yet:c


		ADD_GROUPBOX(vars::container["#skins"], 169, 15, "skins box 1", 260, 393, title_font, vars::container["#window"], 3, false, false, false);
		ADD_CONTROLLER(vars::container["#skins"], vars::tabs["#tab_panel"]);

		ADD_GROUPBOX(vars::container["#skins1"], 285 + 169 - 13, 15, "skins box 2", 260, 393, title_font, vars::container["#window"], 3, false, false, false);
		ADD_CONTROLLER(vars::container["#skins1"], vars::tabs["#tab_panel"]);
	}

	ADD_TAB(vars::tabs["#tab_panel"], "others"); { // others + configs, doesnt finished YET!
		ADD_GROUPBOX(vars::container["#cfg1"], 169, 15, "others", 260, 393, title_font, vars::container["#window"], 4, false, false, false);
		ADD_CONTROLLER(vars::container["#cfg1"], vars::tabs["#tab_panel"]);
		ADD_CHECKBOX(vars::checkbox["#watermark"], 15, 15, "watermark", "vars.watermark", title_font, vars::container["#cfg1"], 4);
		vars::checkbox["#watermark"]->set_bool(false); // if you still wanted the crappy watermark, then change false to true!:) 

		ADD_GROUPBOX(vars::container["#cfg2"], 285 + 169 - 13, 15, "configs", 260, 393, title_font, vars::container["#window"], 4, false, false, false);
		ADD_CONTROLLER(vars::container["#cfg2"], vars::tabs["#tab_panel"]);

	}
	ADD_TOOLTIP(vars::colorpicker["#normal_box_color"], "normal box color");
}