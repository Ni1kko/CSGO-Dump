#include "..\hooks.hpp"
#include "..\..\cheats\visuals\bullet_tracers.h"

using BeginFrame_t = void(__thiscall*)(void*, float);

void __fastcall hooks::hooked_beginframe(void * ecx, void * edx, float ft) {
	static auto original_fn = materialsys_hook->get_func_address< BeginFrame_t >(42);
	original_fn(ecx, ft);
}