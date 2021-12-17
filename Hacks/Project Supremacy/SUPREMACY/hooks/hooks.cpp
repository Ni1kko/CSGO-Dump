#include "hooks.hpp"
namespace hooks {
	void load() {
		window = FindWindow("Valve001", NULL);
		old_window_proc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hooks::hooked_wndproc);
		client_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_client()));
		client_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_fsn), 37);
		fridge::ConsolePrint("Created Clienthook : 37 \n");
		panel_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_panel()));
		panel_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_painttraverse), 41);
		fridge::ConsolePrint("Created Panelhook : 41\n");
		clientmode_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_clientmode()));
		clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_createmove), 24);
		fridge::ConsolePrint("Created Clientmode - CreateMove : 24\n");
		clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_postscreeneffects), 44);
		fridge::ConsolePrint("Created Clientmode - PostScreenEffects : 44\n");
		clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_overrideview), 18);
		fridge::ConsolePrint("Created Clientmode - OverrideView : 18\n");
		renderview_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_renderview()));
		renderview_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_sceneend), 9);
		fridge::ConsolePrint("Created RenderView : 9\n");
		materialsys_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_materialsystem()));
		materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_beginframe), 42);
		fridge::ConsolePrint("Created MaterialSYS : 42\n");
		modelrender_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_modelrender()));
		modelrender_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_dme), 21);
		fridge::ConsolePrint("Created ModelRender : 21\n");
		surface_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_surface()));
		surface_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_lockcursor), 67);
		fridge::ConsolePrint("Created Surface : 67\n");
		directx_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_device()));
		directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_present), 17);
		fridge::ConsolePrint("Created DirectX - Preset : 17\n");
		directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_reset), 16);
		fridge::ConsolePrint("Created DirectX - Reset : 16\n");

		modelcache_hook = new vmthook(reinterpret_cast<DWORD**>(g_csgo.m_modelcache()));
		modelcache_hook->hook_function(reinterpret_cast<uintptr_t>(hooked_findmdl), 10);

		hooked_events.RegisterSelf();
	}
	void shutdown() {
		g_csgo.m_inputsys()->EnableInput(true);
		client_hook->unhook();
		panel_hook->unhook();
		clientmode_hook->unhook();
		renderview_hook->unhook();
		materialsys_hook->unhook();
		modelrender_hook->unhook();
		surface_hook->unhook();
		directx_hook->unhook();
		hooked_events.RemoveSelf();
		materials::get().remove();
		render::get().invalidate_objects();
		SetWindowLongPtrA(FindWindow("Valve001", NULL), GWL_WNDPROC, (LONG)old_window_proc);
	}
	HWND window = nullptr;
	WNDPROC old_window_proc = nullptr;
	vmthook * directx_hook;
	vmthook * client_hook;
	vmthook * engine_hook;
	vmthook * clientmode_hook;
	vmthook * inputinternal_hook;
	vmthook * renderview_hook;
	vmthook * panel_hook;
	vmthook * modelcache_hook;
	vmthook * materialsys_hook;
	vmthook * modelrender_hook;
	vmthook * netchannel_hook;
	vmthook * surface_hook;
	SetupBonesFn original_setupbones;
	C_HookedEvents hooked_events;
}