#pragma once

#include <windows.h>
#include "global.h"
#include "imgui/imgui.h"
#include "imgui/examples/directx9_example/imgui_impl_dx9.h"

class Renderer {
private:
	bool ready = false;
	bool active = false;
	HWND window = nullptr;
public:
	~Renderer();

	bool IsReady() const;
	bool IsActive() const;
	bool Initialize(HWND, IDirect3DDevice9*);
	bool HandleInputMessage(UINT, WPARAM, LPARAM);
};

extern Renderer* renderer;