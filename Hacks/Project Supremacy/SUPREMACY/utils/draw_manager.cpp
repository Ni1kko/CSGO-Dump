#include "draw_manager.h"

vgui::HFont fonts[FONT_MAX];

void render::setup() {
	static auto create_font = [](char * name, int size, int weight, DWORD flags) -> vgui::HFont {
		vgui::HFont font = g_csgo.m_surface()->FontCreate();
		g_csgo.m_surface()->SetFontGlyphSet(font, name, size, weight, 0, 0, flags);

		return font;
	};

	fonts[rolexTEXT] = create_font("Arial", 12, FW_BOLD, FONTFLAG_DROPSHADOW);
	fonts[rolexBOLD] = create_font("Arial", 12, FW_BOLD, FONTFLAG_OUTLINE);
	fonts[ESPFLAG] = create_font("Ariald", 12, FW_THIN, FONTFLAG_OUTLINE);
	fonts[TAHOMA12] = create_font("Arial", 12, FW_NORMAL, FONTFLAG_OUTLINE);
	fonts[MENUNAME] = create_font("Arial", 15, FW_THIN, FONTFLAG_OUTLINE);
	fonts[VERDANA12] = create_font("Arial", 12, FW_NORMAL, FONTFLAG_NONE);
	fonts[VERDANA12OUT] = create_font("Arial", 12, FW_NORMAL, FONTFLAG_OUTLINE);
	fonts[VERDANA12BOLD] = create_font("Arial)", 12, FW_BOLD, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[VERDANA12BOLDOUT] = create_font("Arial", 12, FW_BOLD, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	fonts[ESPNAME] = create_font("Arial", 14, FW_BOLD, FONTFLAG_DROPSHADOW);
	fonts[ESPHEALTH] = create_font("Arial", 12, FW_THIN, FONTFLAG_OUTLINE);
	fonts[WEAPONICONS] = create_font("undefeated", 10, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	fonts[SUBTABWEAPONS] = create_font("undefeated", 12, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	fonts[INDICATORFONT] = create_font("Arial", 20, FW_BOLD, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[INDICATORFONT2] = create_font("Arial", 25, FW_BOLD, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[TABFONT] = create_font("Arial", 12, FW_NORMAL, FONTFLAG_ANTIALIAS);
	fonts[rolexFONT] = create_font("Arial", 40, FW_NORMAL, FONTFLAG_ANTIALIAS);
	fonts[SLIDERFONT] = create_font("Arial", 12, 800, FONTFLAG_ANTIALIAS);
	fonts[NYAFONT] = create_font("Arial", 12, FW_NORMAL, FONTFLAG_OUTLINE);
	fonts[ZeusOk] = create_font("Arial", 100, FW_NORMAL, FONTFLAG_OUTLINE);
	fonts[esp] = create_font("Arial", 12, FW_NORMAL, FONTFLAG_OUTLINE);
}

void render::set_alphafactor(float factor) {
	alpha_factor = factor;
}

float render::get_alphafactor() {
	return alpha_factor;
}

void render::rect(int x, int y, int w, int h, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawOutlinedRect(x, y, x + w, y + h);
}

void render::rect_filled(int x, int y, int w, int h, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawFilledRect(x, y, x + w, y + h);
}

void render::setup_states() const {
	this->m_device->SetVertexShader(nullptr);
	this->m_device->SetPixelShader(nullptr);
	this->m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	this->m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	this->m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	this->m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	this->m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	this->m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	this->m_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	this->m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	this->m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	this->m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	this->m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->m_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	this->m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->m_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	this->m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	this->m_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void render::initalize_objects(LPDIRECT3DDEVICE9 m_device) {
	this->m_device = m_device;

	this->m_device->GetViewport(&m_viewport);
}

void render::invalidate_objects() {
	this->m_device = nullptr;
}

void render::restore_objects(LPDIRECT3DDEVICE9 m_device) {
	this->m_device = m_device;
	this->m_device->GetViewport(&m_viewport);
}

void render::circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);

	float hue = 0.f;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;

		auto clr = Color::FromHSB(hue, 1.f, 1.f);
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), 150);
		hue += 0.02;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));

		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

void render::circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color2;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle)*(circle[i].x - x) - sin(angle)*(circle[i].y - y);
		circle[i].y = y + sin(angle)*(circle[i].x - x) + cos(angle)*(circle[i].y - y);
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

void render::Outline(int x, int y, int w, int h, Color color)
{
	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawOutlinedRect(x, y, x + w, y + h);
}

void render::gradient(int x, int y, int w, int h, Color first, Color second, GradientType type) {
	first.SetAlpha(static_cast<int>(first.a() * alpha_factor));
	second.SetAlpha(static_cast<int>(second.a() * alpha_factor));

	auto filled_rect_fade = [&](bool reversed, float alpha) {
		using Fn = void(__thiscall*)(VOID*, int, int, int, int, unsigned int, unsigned int, bool);
		call_virtual< Fn >(g_csgo.m_surface(), 123) (
			g_csgo.m_surface(), x, y, x + w, y + h,
			reversed ? alpha : 0,
			reversed ? 0 : alpha,
			type == GRADIENT_HORIZONTAL);
	};

	static auto blend = [](const Color & first, const Color & second, float t) -> Color {
		return Color(
			first.r() + t * (second.r() - first.r()),
			first.g() + t * (second.g() - first.g()),
			first.b() + t * (second.b() - first.b()),
			first.a() + t * (second.a() - first.a()));
	};

	if (first.a() == 255 || second.a() == 255) {
		g_csgo.m_surface()->DrawSetColor(blend(first, second, 0.5f));
		g_csgo.m_surface()->DrawFilledRect(x, y, x + w, y + h);
	}

	g_csgo.m_surface()->DrawSetColor(first);
	filled_rect_fade(true, first.a());

	g_csgo.m_surface()->DrawSetColor(second);
	filled_rect_fade(false, second.a());
}

void render::rounded_box(int x, int y, int w, int h, int points, int radius, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	Vertex_t *round = new Vertex_t[4 * points];

	for (int i = 0; i < 4; i++) {
		int _x = x + ((i < 2) ? (w - radius) : radius);
		int _y = y + ((i % 3) ? (h - radius) : radius);

		float a = 90.f * i;

		for (int j = 0; j < points; j++) {
			float _a = DEG2RAD(a + (j / (float)(points - 1)) * 90.f);

			round[(i * points) + j] = Vertex_t(Vector2D(_x + radius * sin(_a), _y - radius * cos(_a)));
		}
	}

	static int Texture = g_csgo.m_surface()->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	g_csgo.m_surface()->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawSetTexture(Texture);

	g_csgo.m_surface()->DrawTexturedPolygon(4 * points, round);
}

void render::circle(int x, int y, int points, int radius, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	static bool once = true;

	static std::vector<float> temppointsx;
	static std::vector<float> temppointsy;

	if (once)
	{
		float step = (float)DirectX::XM_PI * 2.0f / points;
		for (float a = 0; a < (DirectX::XM_PI * 2.0f); a += step)
		{
			temppointsx.push_back(cosf(a));
			temppointsy.push_back(sinf(a));
		}
		once = false;
	}

	std::vector<int> pointsx;
	std::vector<int> pointsy;
	std::vector<Vertex_t> vertices;

	for (int i = 0; i < temppointsx.size(); i++)
	{
		float eeks = radius * temppointsx[i] + x;
		float why = radius * temppointsy[i] + y;
		pointsx.push_back(eeks);
		pointsy.push_back(why);

		vertices.push_back(Vertex_t(Vector2D(eeks, why)));
	}

	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawPolyLine(pointsx.data(), pointsy.data(), points); // only if you want en extra outline
}

void render::circle_filled(int x, int y, int points, int radius, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	static bool once = true;

	static std::vector<float> temppointsx;
	static std::vector<float> temppointsy;

	if (once)
	{
		float step = (float)DirectX::XM_PI * 2.0f / points;
		for (float a = 0; a < (DirectX::XM_PI * 2.0f); a += step)
		{
			temppointsx.push_back(cosf(a));
			temppointsy.push_back(sinf(a));
		}
		once = false;
	}

	std::vector<int> pointsx;
	std::vector<int> pointsy;
	std::vector<Vertex_t> vertices;

	for (int i = 0; i < temppointsx.size(); i++)
	{
		float eeks = radius * temppointsx[i] + x;
		float why = radius * temppointsy[i] + y;
		pointsx.push_back(eeks);
		pointsy.push_back(why);

		vertices.push_back(Vertex_t(Vector2D(eeks, why)));
	}

	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawTexturedPolygon(points, vertices.data());
}

void render::triangle(Vector2D point_one, Vector2D point_two, Vector2D point_three, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	Vertex_t verts[3] = {
		Vertex_t(point_one),
		Vertex_t(point_two),
		Vertex_t(point_three)
	};

	static int texture = g_csgo.m_surface()->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	g_csgo.m_surface()->DrawSetTextureRGBA(texture, buffer, 1, 1);
	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawSetTexture(texture);

	g_csgo.m_surface()->DrawTexturedPolygon(3, verts);
}

void render::line(int x, int y, int x2, int y2, Color color) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	g_csgo.m_surface()->DrawSetColor(color);
	g_csgo.m_surface()->DrawLine(x, y, x2, y2);
}

void render::text(vgui::HFont font, int x, int y, Color color, DWORD flags, const char* msg, ...) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	va_list va_alist;
	char buffer[1024];
	va_start(va_alist, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

	int width, height;
	g_csgo.m_surface()->GetTextSize(font, wbuf, width, height);

	if (!(flags & HFONT_CENTERED_NONE)) {
		if (flags & HFONT_CENTERED_X)
			x -= width * 0.5f;

		if (flags & HFONT_CENTERED_Y)
			y -= height * 0.5f;
	}

	g_csgo.m_surface()->DrawSetTextFont(font);
	g_csgo.m_surface()->DrawSetTextColor(color);
	g_csgo.m_surface()->DrawSetTextPos(x, y);
	g_csgo.m_surface()->DrawPrintText(wbuf, wcslen(wbuf));
}

void render::wtext(vgui::HFont font, int x, int y, Color color, DWORD flags, wchar_t* msg) {
	color.SetAlpha(static_cast<int>(color.a() * alpha_factor));

	wchar_t buffer[256];
	va_list args;

	va_start(args, msg);
	wvsprintfW(buffer, msg, args);
	va_end(args);

	int width, height;
	g_csgo.m_surface()->GetTextSize(font, buffer, width, height);

	if (!(flags & HFONT_CENTERED_NONE)) {
		if (flags & HFONT_CENTERED_X)
			x -= width * 0.5f;

		if (flags & HFONT_CENTERED_Y)
			y -= height * 0.5f;
	}

	g_csgo.m_surface()->DrawSetTextFont(font);
	g_csgo.m_surface()->DrawSetTextColor(color);
	g_csgo.m_surface()->DrawSetTextPos(x, y);
	g_csgo.m_surface()->DrawPrintText(buffer, wcslen(buffer));
}

float render::text_width(vgui::HFont font, const char* msg, ...) {
	va_list va_alist;
	char buffer[1024];
	va_start(va_alist, msg);
	_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];

	MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

	int width, height;
	g_csgo.m_surface()->GetTextSize(font, wbuf, width, height);

	return width;
}

void render::custom_box(int x, int y, int w, int h)
{
	auto box = [](int x, int y, int w, int h, Color c)
	{
		render::get().line(x, y, x, y + h, c);
		render::get().line(x, y + h, x + w + 1, y + h, c);
		render::get().line(x + w, y, x + w, y + h, c);
		render::get().line(x, y, x + w, y, c);
	};

	render::get().rect_filled(x, y, w, h, Color(172, 89, 107));
}

RECT render::viewport() {
	RECT viewport = { 0, 0, 0, 0 };

	int width, height;
	g_csgo.m_engine()->GetScreenSize(width, height);
	viewport.right = width;
	viewport.bottom = height;

	return viewport;
}