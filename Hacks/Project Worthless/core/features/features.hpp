#pragma once
#include "../../dependencies/common_includes.hpp"

namespace aimbot {
	bool aim(c_usercmd* cmd);
	void smooth(vec3_t& viewangles, vec3_t& angles, int amount);
	void weapon_settings(weapon_t* weapon);
}

namespace misc {
	namespace movement {
		void pre_prediction_flags();
		void bunny_hop(c_usercmd* cmd);
		void jumpbug(c_usercmd* pCmd);
		void duckbug(c_usercmd* pCmd);
		void speed();
		void jbindicator();
		void fastduck(c_usercmd* cmd);
		void edgejump(c_usercmd* cmd);
		
	};

	void disable_post_processing() noexcept;
	void rank_reveal() noexcept;
	void no_flash();
	void spectators() noexcept;
	void keystrokes() noexcept;

}

namespace visuals {

	namespace chams {
		void run();
		void create_materials();
		void remove_materials();
	};

	namespace glow {
		void run();
	};

	void run();
	void force_crosshair();
	void player_esp(player_t* entity);
	
}