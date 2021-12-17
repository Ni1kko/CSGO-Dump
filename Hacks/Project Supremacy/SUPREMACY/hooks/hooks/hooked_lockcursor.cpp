#include "..\hooks.hpp"

using LockCursor_t = void(__thiscall*)(void *);

void __stdcall hooks::hooked_lockcursor() {
	static auto original_fn = surface_hook->get_func_address< LockCursor_t >(67);

	if (menu::get().open)
		g_csgo.m_surface()->UnlockCursor();
	else
		original_fn(g_csgo.m_surface());
}