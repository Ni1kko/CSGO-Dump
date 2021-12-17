#include "Render.h"
#include "../Other/ColorMgr.hpp"
#include "../../Backend/SDK/color.h"

Graphics::CRender* Graphics::Render = new Graphics::CRender();

using namespace Graphics;

namespace Graphics
{
	namespace Fonts
	{
		LPD3DXFONT Font_INF;
		LPD3DXFONT Font_ESP;
		LPD3DXFONT Font_IND;
	}
}

bool CRender::m_initialized = false;

void CRender::CreateObjects()
{
	D3DXCreateLine(m_device, &m_line);
	D3DXCreateSprite(m_device, &m_sprite);
	D3DXCreateFontA(m_device, 12, 0, 200, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, xorstr_("Verdana"), &Fonts::Font_INF);
	D3DXCreateFontA(m_device, 48, 0, 700, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, xorstr_("Verdana"), &Fonts::Font_IND);
	D3DXCreateFontA(m_device, 12, 0, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DRAFT_QUALITY, DEFAULT_PITCH, xorstr_("Verdana"), &Fonts::Font_ESP);
	m_candraw = true;
}

void CRender::Initialize(IDirect3DDevice9* device)
{
	if (!m_initialized)
	{
		m_device = device;
		CreateObjects();
		m_initialized = true;
	}
}

void CRender::ReleaseObjects()
{
	if (!m_candraw)
		return;

	if (m_line)
		m_line->Release();

	if (m_sprite)
		m_sprite->Release();

	if (Fonts::Font_ESP)
		Fonts::Font_ESP->Release();

	if (Fonts::Font_IND)
		Fonts::Font_IND->Release();

	if (Fonts::Font_INF)
		Fonts::Font_INF->Release();
}

void CRender::Begin()
{
	if (!m_candraw)
		return;

	//m_device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	//m_device->SetRenderState( D3DRS_ZENABLE, false );
	//m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	//m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//m_device->SetRenderState( D3DRS_SCISSORTESTENABLE, false );
	//m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
}

void CRender::End()
{
}

void CRender::Reset()
{
	D3DVIEWPORT9 screen;
	m_device->GetViewport(&screen);
	m_screen.w = screen.Width;
	m_screen.h = screen.Height;
	m_screen.xc = screen.Width / 2.0f;
	m_screen.yc = screen.Height / 2.0f;
}

int CRender::StringWidth(ID3DXFont* font, std::string in)
{
	auto rect = RECT();
	font->DrawTextA(0, in.c_str(), strlen(in.c_str()), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
	return rect.right - rect.left;
}

int CRender::StringHeight(ID3DXFont* font, std::string in)
{
	auto rect = RECT();
	font->DrawTextA(0, in.c_str(), strlen(in.c_str()), &rect, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
	return rect.bottom - rect.top;
}

void CRender::Line(int x, int y, int x2, int y2, D3DCOLOR color)
{
	Vertex pVertex[2] = { { x, y, 0.0f, 1.0f, color }, { x2, y2, 0.0f, 1.0f, color } };
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->SetTexture(0, nullptr);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	DWORD colorwrite, srgbwrite;
	m_device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	m_device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	m_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &pVertex, sizeof(Vertex));
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
}

void CRender::FilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	Vertex pVertex[4] = { { x, y + h, 0.0f, 1.0f, color }, { x, y, 0.0f, 1.0f, color }, { x + w, y + h, 0.0f, 1.0f, color }, { x + w, y, 0.0f, 1.0f, color } };
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->SetTexture(0, nullptr);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(Vertex));
}

void CRender::FilledRectOutlined(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness)
{
	FilledRect(x, y, w, h, color);
	BorderedRect(x, y, w, h, outline);
}

void CRender::BorderedRect(int x, int y, int w, int h, D3DCOLOR color, int thickness)
{
	FilledRect(x, y, w, thickness, color);
	FilledRect(x, y, thickness, h, color);
	FilledRect(x + w - thickness, y, thickness, h, color);
	FilledRect(x, y + h - thickness, w, thickness, color);
}

void CRender::BorderedRectOutlined(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness)
{
	BorderedRect(x, y, w, h, outline, thickness);
	BorderedRect(x + thickness, y + thickness, w - (thickness * 2), h - (thickness * 2), color, thickness);
	BorderedRect(x + (thickness * 2), y + (thickness * 2), w - (thickness * 4), h - (thickness * 4), outline, thickness);
}

void CRender::Gradient(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, bool vertical)
{
	Vertex pVertex[4] = { { x, y, 0.0f, 1.0f, color }, { x + w, y, 0.0f, 1.0f, vertical ? color : other_color }, { x, y + h, 0.0f, 1.0f, vertical ? other_color : color }, { x + w, y + h, 0.0f, 1.0f, other_color } };
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->SetTexture(0, nullptr);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(Vertex));
}

void CRender::GradientOutlined(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, D3DCOLOR outline_color, bool vertical)
{
	Gradient(x, y, w, h, color, other_color, vertical);
	BorderedRect(x, y, w, h, outline_color, 1);
}

void CRender::BorderedCircle(int x, int y, int r, int p, D3DCOLOR color)
{
	Vertex* pVertex = new Vertex[p + 1];

	for (int i = 0; i <= p; i++)
		pVertex[i] = { x + r * cos(D3DX_PI * (i / (p / 2.0f))), y - r * sin(D3DX_PI * (i / (p / 2.0f))), 0.0f, 1.0f, color };

	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, p, pVertex, sizeof(Vertex));

	delete[] pVertex;
}

void CRender::FilledCircle(int x, int y, int r, int p, D3DCOLOR color)
{
	Vertex* pVertex = new Vertex[p + 1];
	auto r1 = get_r(color);
	auto g = get_g(color);
	auto b = get_b(color);

	for (int i = 0; i <= p; i++)
		pVertex[i] = { x + r * cos(D3DX_PI * (i / (p / 2.0f))), y + r * sin(D3DX_PI * (i / (p / 2.0f))), 0.0f, 1.0f, color };

	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, p, pVertex, sizeof(Vertex));

	delete[] pVertex;
}

void CRender::Text(std::string text, int x, int y, int o, LPD3DXFONT font, bool bordered, D3DCOLOR color)
{
	RECT rect;
	auto alpha = get_a(color);

	switch (o)
	{
	case LEFT:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x + 1, y, x + 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y - 1, x, y - 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y + 1, x, y + 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}

		SetRect(&rect, x, y, x, y);
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, color);
		break;

	case CENTER:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x + 1, y, x + 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y - 1, x, y - 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y + 1, x, y + 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}

		SetRect(&rect, x, y, x, y);
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, color);
		break;

	case RIGHT:
		if (bordered)
		{
			SetRect(&rect, x - 1, y, x - 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x + 1, y, x + 1, y);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y - 1, x, y - 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
			SetRect(&rect, x, y + 1, x, y + 1);
			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(alpha, 0, 0, 0));
		}

		SetRect(&rect, x, y, x, y);
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, color);
		break;
	}
}

void CRender::Box(int x, int y, int w, int h, D3DCOLOR color)
{
	int x2 = x + w;
	int y2 = y + h;
	Line(x, y, x2, y, color);
	Line(x2, y, x2, y2, color);
	Line(x2, y2, x, y2, color);
	Line(x, y2, x, y, color);
}
LPDIRECT3DVERTEXBUFFER9 g_pVB2;
void CRender::GradientCircle(float x, float y, float rad, float rotate, int type, int resolution, D3DCOLOR color, D3DCOLOR color2)
{
	std::vector<Vertex> circle(resolution + 2);
	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;
	pi = D3DX_PI / type; // 1/4 circle
	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color2;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;

	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle) * (circle[i].x - x) - sin(angle) * (circle[i].y - y);
		circle[i].y = y + sin(angle) * (circle[i].x - x) + cos(angle) * (circle[i].y - y);
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(Vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);
	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(Vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(Vertex));
	g_pVB2->Unlock();
	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(Vertex));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	DWORD colorwrite, srgbwrite;
	m_device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	m_device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

void CRender::RainbowCircle(float x, float y, float rad, float rotate, int type, int resolution)
{
	std::vector<Vertex> circle(resolution + 2);
	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;
	pi = D3DX_PI; // Full circle
	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);
	float hue = 0.f;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi * ((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		auto clr = CColor::FromHSB(hue, 1.f, 1.f, 100);
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a());
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

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(Vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);
	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(Vertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(Vertex));
	g_pVB2->Unlock();
	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(Vertex));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	DWORD colorwrite, srgbwrite;
	m_device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	m_device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}
