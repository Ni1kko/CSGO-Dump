#include "render.hpp"

#include <mutex>
#include <iostream>
#include <Windows.h>
#include <filesystem>

#include "../visuals/visuals.hpp"
#include "../../sdk/structs.hpp"
#include "../../helpers/input.hpp"
#include "../menu/menu.hpp"
#include "../config/options.hpp"
#include "../../fonts/fonts.hpp"
#include "../../helpers/math.hpp"
#include "backdrop.h"

ImDrawListSharedData _data;

std::mutex render_mutex;

void Render::Initialize() {
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	GetFonts();
}

void Render::GetFonts() {
	ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( 
		Fonts::Droid_compressed_data, Fonts::Droid_compressed_size,
		14.f, nullptr, ImGui::GetIO( ).Fonts->GetGlyphRangesCyrillic( ) );

	g_pDefaultFont = ImGui::GetIO( ).Fonts->AddFontFromMemoryTTF( 
		Fonts::espfont, sizeof(Fonts::espfont), 
		14.f, nullptr, ImGui::GetIO( ).Fonts->GetGlyphRangesCyrillic( ) );

	g_pSecondFont = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( 
		Fonts::Cousine_compressed_data, Fonts::Cousine_compressed_size, 
		18.f, nullptr, ImGui::GetIO( ).Fonts->GetGlyphRangesCyrillic( ) );
}

void Render::ClearDrawList() {
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}

void Render::BeginScene() {
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();

	
	if (Cfg.backdrop) backdrop::render();
		
	if (g_EngineClient->IsInGame() && g_LocalPlayer && Cfg.esp_enabled) Visuals::Get().AddToDrawList();

	if (g_EngineClient->IsInGame() && g_LocalPlayer && Cfg.other_drawfov) Visuals::Get().DrawFOV();

	int w, h;
	g_EngineClient->GetScreenSize(w, h);

	float x = w * 0.5f;
	float y = h * 0.5f;
	extern float side;
	if (g_EngineClient->IsInGame() && g_LocalPlayer) 
		if (Cfg.misc_desync) Render::Get().RenderText(side < 0.0f ? ">" : "<", 
			side > 0.0f ? x - 120 : x + 120, y - 0.f, 24.f, Color::White, true, false, g_pSecondFont);
	
	render_mutex.lock();
	*draw_list_act = *draw_list;
	render_mutex.unlock();
}

ImDrawList* Render::RenderScene() {

	if (render_mutex.try_lock()) {
		*draw_list_rendering = *draw_list_act;
		render_mutex.unlock();
	}

	return draw_list_rendering;
}

void Render::FilledRectange(int X1, int Y1, int X2, int Y2, Color DrawColor) {
	g_VGuiSurface->DrawSetColor(DrawColor);
	g_VGuiSurface->DrawFilledRect(X1, Y1, X2, Y2);
}

float Render::RenderText(const std::string& text, ImVec2 pos, float size, Color color, bool center, bool outline, ImFont* pFont) {
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
	if (!pFont->ContainerAtlas) return 0.f;
	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

	if (center) pos.x -= textSize.x / 2.0f;

	if (outline) {
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
	}

	draw_list->AddText(pFont, size, pos, GetU32(color), text.c_str());

	draw_list->PopTextureID();

	return pos.y + textSize.y;
}

void Render::RenderCircle3D(Vector position, float radius, Color color) {
	float points = 160.f;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step) {
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d)) return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}
