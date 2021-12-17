#include "Renderer.h"

// Allow us to directly call the ImGui WndProc function.
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

// The original WndProc function.
WNDPROC game_wndproc = nullptr;

// Hooked WndProc function to process all incoming input messages.
LRESULT __stdcall hkWndProc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param) {
	switch (message_type) {
	case WM_LBUTTONDOWN:
		G::PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		G::PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		G::PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		G::PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		G::PressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		G::PressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(w_param);
		if (button == XBUTTON1)
		{
			G::PressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			G::PressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(w_param);
		if (button == XBUTTON1)
		{
			G::PressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			G::PressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		G::PressedKeys[w_param] = true;
		break;
	case WM_KEYUP:
		G::PressedKeys[w_param] = false;
		break;
	default: break;
	}
	// Let the renderer decide whether we should pass this input message to the game.
	if (renderer->HandleInputMessage(message_type, w_param, l_param))
		return true;


	// The GUI is inactive so pass the input to the game.
	return CallWindowProc(game_wndproc, window, message_type, w_param, l_param);
};

Renderer::~Renderer() {
	// Restore the original WndProc function.
	SetWindowLongPtr(this->window, GWLP_WNDPROC, LONG_PTR(game_wndproc));
}

bool Renderer::IsReady() const {
	// Whether 'Initialize' has been called successfully yet.
	return this->ready;
}

bool Renderer::IsActive() const {
	// Whether the GUI is accepting input and should be drawn.
	return this->ready && this->active;
}

bool Renderer::Initialize(HWND window, IDirect3DDevice9* device) {

	this->window = window;


	game_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(hkWndProc)));


	if (ImGui_ImplDX9_Init(window, device))
		this->ready = true;

	return this->ready;
}

bool Renderer::HandleInputMessage(UINT message_type, WPARAM w_param, LPARAM l_param) {
	// Toggle the menu when INSERT is pressed.
	if (message_type == WM_KEYUP && w_param == VK_INSERT)
		this->active = !this->active;

	// When the GUI is active ImGui can handle input by itself.
	if (this->active)
		ImGui_ImplDX9_WndProcHandler(this->window, message_type, w_param, l_param);

	return this->active;
}

Renderer* renderer = new Renderer;