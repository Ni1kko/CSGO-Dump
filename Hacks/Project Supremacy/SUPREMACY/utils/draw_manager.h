#pragma once

#include "..\includes.hpp"
#include "singleton.h"

enum FontCenteringFlags {
	HFONT_CENTERED_NONE = (1 << 0),
	HFONT_CENTERED_X = (1 << 1),
	HFONT_CENTERED_Y = (1 << 2),
	HFONT_OFF_SCREEN,
	HFONT_ON_MENU,
};

enum GradientType {
	GRADIENT_HORIZONTAL = 0,
	GRADIENT_VERTICAL
};

enum FONT_INDEX {
	rolexTEXT,
	rolexBOLD,
	ESPFLAG,
	TAHOMA12,
	MENUNAME,
	VERDANA12,
	VERDANA12OUT,
	VERDANA12BOLD,
	VERDANA12BOLDOUT,
	ESPNAME,
	ESPHEALTH,
	WEAPONICONS,
	SUBTABWEAPONS,
	INDICATORFONT,
	INDICATORFONT2,
	TABFONT,
	rolexFONT,
	SLIDERFONT,
	NYAFONT,
	ZeusOk,
	esp,
	FONT_MAX
};

extern vgui::HFont fonts[FONT_MAX];

template<typename T>
class singleton;

struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
};

class render : public singleton< render >
{
private:
	float alpha_factor = 1.f;
	LPDIRECT3DDEVICE9 m_device;
	D3DVIEWPORT9      m_viewport;
public:
	bool initalized = false;
	void setup();

	float get_alphafactor();
	void set_alphafactor(float factor);
	void rect(int x, int y, int w, int h, Color color);
	void rect_filled(int x, int y, int w, int h, Color color);
	void setup_states() const;
	void initalize_objects(LPDIRECT3DDEVICE9 m_device);
	void invalidate_objects();
	void restore_objects(LPDIRECT3DDEVICE9 m_device);
	void circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9 * m_device);
	void circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9 * m_device);
	void gradient(int x, int y, int w, int h, Color first, Color second, GradientType type);
	void rounded_box(int x, int y, int w, int h, int points, int radius, Color color);
	void circle(int x, int y, int points, int radius, Color color);
	void circle_filled(int x, int y, int points, int radius, Color color);
	void triangle(Vector2D point_one, Vector2D point_two, Vector2D point_three, Color color);
	void line(int x, int y, int x2, int y2, Color color);
	void text(vgui::HFont font, int x, int y, Color color, DWORD flags, const char* msg, ...);
	void wtext(vgui::HFont font, int x, int y, Color color, DWORD flags, wchar_t* msg);
	float text_width(vgui::HFont font, const char* msg, ...);
	void custom_box(int x, int y, int w, int h);
	void Outline(int x, int y, int w, int h, Color color);
	
	RECT viewport();
};