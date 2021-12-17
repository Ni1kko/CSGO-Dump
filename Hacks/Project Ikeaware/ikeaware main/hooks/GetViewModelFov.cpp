#include "hooks.hpp"

float __fastcall Hooks::GetViewModelFov(void* _this, int)
{
	return g_Options.misc.fov.viewmodel;
}