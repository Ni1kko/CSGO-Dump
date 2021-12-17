#include "..\hooks.hpp"

//extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT __stdcall hooks::hooked_wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return CallWindowProc(old_window_proc, hWnd, uMsg, wParam, lParam);
}