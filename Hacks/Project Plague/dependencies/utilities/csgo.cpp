#include "csgo.hpp"

namespace csgo {
	player_t* local_player = nullptr;
	bool menu_open = false;
	int menu_tab = 0;
	bool send_packet = true;
	c_usercmd* cmd = nullptr;
	bool aa_side = false;
	bool aa_running = false;
	float real_abs = 0.f;
}