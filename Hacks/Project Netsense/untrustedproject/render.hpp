#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include "singleton.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"

#include "options.hpp"

#include "valve_sdk/misc/Color.hpp"

extern ImFont* g_pDefaultFont;
extern ImFont* g_pSecondFont;
extern ImFont* g_pFlagsFont;
extern ImFont* g_pMenuFont;

class Vector;

class Render
	: public Singleton<Render>
{
	friend class Singleton<Render>;

private:
	ImDrawList * draw_list_act;
	ImDrawList * draw_list_rendering;
	ImDrawList* draw_list;
	ImDrawData draw_data;

	ImU32 GetU32(Color _color) {
		return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8)
			+ (_color[0] & 0xff);
	}
public:
	void Initialize();
	void GetFonts();
	void ClearDrawList();
	void BeginScene();
	ImDrawList* RenderScene();

	float RenderText(const std::string& text, ImVec2 position, float size, Color color, bool center = false, bool outline = false, ImFont* pFont = g_pDefaultFont);

	void RenderCircle3D(Vector position, float points, float radius, Color color);

	void RenderImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a = ImVec2(0, 0), const ImVec2& uv_b = ImVec2(1, 1), ImU32 col = 0xFFFFFFFF) {
		if (!g_Unload)
			draw_list->AddImage(user_texture_id, a, b, uv_a, uv_b, col);
	}

	template <class T>
	inline void RenderBoxByType(T x1, T y1, T x2, T y2, Color color, float thickness = 1.f, int type = 0) {
		if (!g_Unload) {
			if (type == 0)
				RenderBox(x1, y1, x2, y2, color, thickness);
			else if (type == 1)
				RenderCoalBox(x1, y1, x2, y2, color);
			else if (type == 2)
				RenderBox(x1, y1, x2, y2, color, thickness, 8.f);
		}
	}

	template <class T>
	inline void RenderBoxFilledByType(T x1, T y1, T x2, T y2, Color color, float thickness = 1.f, int type = 0) {
		if (!g_Unload) {
			if (type == 0 || type == 1)
				RenderBoxFilled(x1, y1, x2, y2, color, thickness);
			else if (type == 2)
				RenderBoxFilled(x1, y1, x2, y2, color, thickness, 8.f);
		}
	}

	template <class T>
	inline void RenderCoalBox(T x1, T y1, T x2, T y2, Color color, float th = 1.f) {
		if (!g_Unload) {
			int w = x2 - x1;
			int h = y2 - y1;

			int iw = w / 4;
			int ih = h / 4;
			// top
			RenderLine(x1, y1, x1 + iw, y1, color, th);					// left
			RenderLine(x1 + w - iw, y1, x1 + w, y1, color, th);			// right
			RenderLine(x1, y1, x1, y1 + ih, color, th);					// top left
			RenderLine(x1 + w - 1, y1, x1 + w - 1, y1 + ih, color, th);	// top right
																		// bottom
			RenderLine(x1, y1 + h, x1 + iw, y1 + h, color, th);			// left
			RenderLine(x1 + w - iw, y1 + h, x1 + w, y1 + h, color, th);	// right
			RenderLine(x1, y1 + h - ih, x1, y1 + h, color, th);			// bottom left
			RenderLine(x1 + w - 1, y1 + h - ih, x1 + w - 1, y1 + h, color, th);	// bottom right
		}
	}

	template <class T>
	inline void RenderBox(T x1, T y1, T x2, T y2, Color color, float thickness = 1.f, float rounding = 0.f) {
		if (!g_Unload)
			draw_list->AddRect(ImVec2((float)x1, (float)y1), ImVec2((float)x2, (float)y2), GetU32(color), rounding, 15, thickness);
	}
	inline void RenderBox(RECT r, Color color,float thickness = 1.f, float rounding = 0.f) {
		if (!g_Unload)
			RenderBox(r.left, r.top, r.right, r.bottom, color, thickness, rounding);
	}
	template <class T>
	inline void RenderBoxGradient(T x, T y, T x1, T y1, Color c1, Color c2, float thickness = 1.f, float rounding = 0.f) {
		if (!g_Unload)
			draw_list->AddRectFilledMultiColor(ImVec2((float)x, (float)y), ImVec2((float)x1, (float)y1), GetU32(c1), GetU32(c1), GetU32(c2), GetU32(c2));
	}
	template <class T>
	inline void RenderBoxFilled(T x1, T y1, T x2, T y2, Color color, float thickness = 1.f, float rounding = 0.f) {
		if (!g_Unload)
			draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), GetU32(color), rounding, 15);
	}
	template <class T>
	inline void RenderLine(T x1, T y1, T x2, T y2, Color color, float thickness = 1.f) {
		if (!g_Unload)
			draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), GetU32(color), thickness);
	}
	template <class T>
	inline float RenderText(const std::string& text, T x, T y, float size, Color clr, bool center = false, bool outline = true, ImFont* pFont = g_pDefaultFont) {
		if (!g_Unload)
			return RenderText(text, ImVec2(x, y), size, clr, center, outline, pFont);
		return 0.f;
	}
	template <class T>
	inline void RenderCircle(T x, T y, float radius, int points, Color color, float thickness = 1.f) {
		if (!g_Unload)
			draw_list->AddCircle(ImVec2(x, y), radius, GetU32(color), points, thickness);
	}
	template <class T>
	inline void RenderCircleFilled(T x, T y, float radius, int points, Color color) {
		if (!g_Unload)
			draw_list->AddCircleFilled(ImVec2(x, y), radius, GetU32(color), points);
	}
	template <class T>
	inline void RenderTriangle(T p1, T p2, T p3, Color color) {
		if (!g_Unload)
			draw_list->AddTriangle(p1, p2, p3, GetU32(color));
	}
	template <class T>
	inline void RenderTriangleFilled(T p1, T p2, T p3, Color color) {
		if (!g_Unload)
			draw_list->AddTriangleFilled(p1, p2, p3, GetU32(color));
	}
	inline void PopClipRect() {
		if (!g_Unload)
			draw_list->PopClipRect();
	}
	void RenderCircleDualColor(float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2);
	void RenderCircle3DFilled(const Vector& origin, float radius, Color color);
	void Arc(float x, float y, int r1, int r2, float s, float d, Color color);
	void RenderCircleDualColor(float x, float y, float rad, float rotate, int type, int resolution);
};