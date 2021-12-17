#pragma once
#include "../../Backend/singleton.h"
#include "../../Backend/Include/GlobalIncludes.h"

#include "../UI/imgui.h"
#include "../UI/imgui_freetype.h"

#pragma comment(lib, "freetype.lib")

#include "../UI/imgui_impl_dx9.h"
#include "../UI/imgui_impl_win32.h"

class c_menu : public singleton<c_menu> {
public:
	void initialize(IDirect3DDevice9* pDevice);
	void draw_begin();
	void draw();
	void draw_end();

	bool is_menu_initialized();
	bool is_menu_opened();
	void set_menu_opened(bool v);

	IDirect3DTexture9* get_texture_data();
	ImColor get_accent_color();

private:
	bool m_bInitialized;
	bool m_bIsOpened;
	IDirect3DTexture9* m_pTexture;

	int m_nCurrentTab;
};