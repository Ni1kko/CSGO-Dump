#include "..\hooks.hpp"

using SetKeyCodeState_t = void(__thiscall*)(void*, ButtonCode_t, bool);
using SetMouseCodeState_t = void(__thiscall*)(void*, ButtonCode_t, MouseCodeState_t);

void __fastcall hooks::hooked_setkeycodestate(void* thisptr, void* edx, ButtonCode_t code, bool pressed) {
	static auto original_fn = inputinternal_hook->get_func_address<SetKeyCodeState_t>(91);

	return original_fn(thisptr, code, pressed);
}

void __fastcall hooks::hooked_setmousecodestate(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state) {
	static auto original_fn = inputinternal_hook->get_func_address<SetMouseCodeState_t>(92);

	return original_fn(thisptr, code, state);
}