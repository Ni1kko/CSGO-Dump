#pragma once

namespace hooks {
	bool initialize();
	void release();

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __fastcall hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace lock_cursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}

	namespace in_key_event {
		using fn = int(__stdcall*)(int, int, const char*);
		int __fastcall hook(void* ecx, int edx, int event_code, int key_num, const char* current_binding);
	}

	namespace do_post_screen_effects {
		using fn = bool(__thiscall*)(i_client_mode*, int);
		bool __stdcall hook(int value);
	}

	namespace draw_model_execute {
		using fn = void(__thiscall*)(iv_model_render*, IMatRenderContext*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		void __stdcall hook(IMatRenderContext* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		void __stdcall hook(int frame_stage);
	}

	namespace viewmodel_fov {
		float __stdcall hook();
	}

	namespace override_view {
		using fn = void* (__thiscall*)(void* _this, view_setup_t * setup);
		void __fastcall hook(void* _this, void* _edx, view_setup_t* setup);
	}

	namespace sv_cheats {
		using fn = bool (__thiscall*)(void*);
		bool __fastcall hook(PVOID convar, int edx);
	}

	namespace setup_bones {
		using fn = bool(__thiscall*) (void*, matrix_t*, int, int, float);
		bool __fastcall hook(void* ecx, void* edx, matrix_t* bone_to_world_out, int max_bones, int bone_mask, float curtime);
	}
}