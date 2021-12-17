#include "animation_state.h"

float spawn_time = 0.0f;
CBaseHandle * entity_handle = nullptr;
c_baseplayeranimationstate * server_animation_state = nullptr;

void animation_state::create_move() {
	bool
		allocate = (server_animation_state == nullptr),
		change = (!allocate) && (&g_ctx.m_local->GetRefEHandle() != entity_handle),
		reset = (!allocate && !change) && (g_ctx.m_local->m_flSpawnTime() != spawn_time);

	if (change)
		g_csgo.m_memalloc()->Free(server_animation_state);

	if (reset) {
		reset_state(server_animation_state);
		spawn_time = g_ctx.m_local->m_flSpawnTime();
	}

	if (allocate || change) {
		auto state = (c_baseplayeranimationstate *)g_csgo.m_memalloc()->Alloc(sizeof(c_baseplayeranimationstate));

		if (state != nullptr)
			create_state(state, g_ctx.m_local);

		entity_handle = const_cast<CBaseHandle *>(&g_ctx.m_local->GetRefEHandle());
		spawn_time = g_ctx.m_local->m_flSpawnTime();

		server_animation_state = state;
	}

	float server_time = util::server_time();
	if (!g_csgo.m_clientstate()->m_nChokedCommands) {
		float pose_parameters[24];
		AnimationLayer anim_layers[13];

		memcpy(pose_parameters, g_ctx.m_local->m_flPoseParameter(), sizeof(float) * 24);
		memcpy(anim_layers, g_ctx.m_local->get_animlayers(), sizeof(AnimationLayer) * 13);

		update_state( server_animation_state, g_ctx.get_command( )->m_viewangles ); // this is what causes the legs to be fucked, but i know you guys are retarded enough not to realise OR CARE about it, so im leaving this comment here as a reminder that when im gonna leak this source everyone will realise how stupid you guys are and how you cant solve the most basic shit without breaking even mroe shit. hf crying under your bed you dumb """cheat developers""", i feel bad for whoever bought this cheat, especially the ones that paid $130 for it. talking about money, i dont want to get deep into this because its currently sunday morning 4 am, but either way, dusk/ecralion has told me that he will pay me at the end of the week (2 day late btw, and he hasnt even apologised / brung it up to me), which he for obvious reasons didnt do. one guy purchased the cheat and dusk never even bothered to add him to the users role on our discord (that he deleted lol), i assume because he didnt want me knowing a guy purchased the cheat, and didnt want to pay me. after i asked him if he bought the cheat, then he made him a user on the server. im really sorry for victims that fell for this terrible cheat, i'd do something if i got the money(refund or something), but unfortuantely i didnt get any money because the greedy dickheads kept it for themselves and used me to make 'their' cheat better. this is directed at "ecralion" on uc, and jeffrey which idk his uc name, but yeah, i am signing out as the 12 year old child aka alpha/"524245534435344", which is smarter than 15 year old dusk/ecralion. i dont give the slightest fuck if my rep gets ruined by this, but im not going to be forced to work on a cheat, promise to get paid, and get lied to. peace out.
		memcpy(g_ctx.m_local->m_flPoseParameter(), pose_parameters, sizeof(float) * 24);
		memcpy(g_ctx.m_local->get_animlayers(), anim_layers, sizeof(AnimationLayer) * 13);
	}
}

void animation_state::create_state(c_baseplayeranimationstate * state, player_t * player) {
	using Fn = void(__thiscall*)(c_baseplayeranimationstate*, player_t*);
	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
	fn(state, player);
}

void animation_state::update_state(c_baseplayeranimationstate * state, vec3_t angles) {
	using Fn = void(__vectorcall*)(void *, void *, float, float, float, void *);
	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));
	fn(state, nullptr, 0.0f, angles[1], angles[0], nullptr);
}

void animation_state::reset_state(c_baseplayeranimationstate * state) {
	using Fn = void(__thiscall*)(c_baseplayeranimationstate *);
	static auto fn = reinterpret_cast<Fn>(util::pattern_scan("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1"));
	fn(state);
}

c_baseplayeranimationstate * animation_state::animstate() {
	return server_animation_state;
}