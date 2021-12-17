#include "render.hpp"

#include <mutex>

#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "fonts/fonts.hpp"
#include "images.hpp"
#include "helpers/math.hpp"

ImFont* g_pDefaultFont;
ImFont* g_pSecondFont;
IDirect3DTexture9 *brand_img = nullptr;

ImDrawListSharedData _data;

std::mutex render_mutex;

void Render::Initialize()
{
	ImGui::CreateContext();

	if (brand_img == nullptr)
		D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &images::brand_bytes, sizeof(images::brand_bytes), 142, 50, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &brand_img);

	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	GetFonts();
}

void Render::GetFonts() {

	// menu font
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::Droid_compressed_data,
		Fonts::Droid_compressed_size,
		14.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	
	// esp font
	g_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::Droid_compressed_data,
		Fonts::Droid_compressed_size,
		18.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	

	// font for watermark; just example
	g_pSecondFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::Cousine_compressed_data,
		Fonts::Cousine_compressed_size,
		18.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
}

void Render::ClearDrawList() {
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}

void Render::BeginScene() {
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();


	static float alpha;

	if (Menu::Get().IsVisible() && alpha < 0.5)			alpha += 0.025;
	else if (!Menu::Get().IsVisible() && alpha > 0)		alpha -= 0.025;

	int screenWidth, screenHeight;
	g_EngineClient->GetScreenSize(screenWidth, screenHeight);

	draw_list->AddRectFilled(ImVec2(0, 0), ImVec2(screenWidth, screenHeight), ImGui::GetColorU32(ImVec4(0, 0, 0, alpha)));


	if (g_Options.misc_watermark)
	{
		static float rainbow; rainbow += 0.005f; if (rainbow > 1.f) rainbow = 0.f;
		Render::Get().RenderText("Amnesia", 8, 5, 18.f, Color::FromHSB(rainbow, 1.f, 1.f), false, true);
	}

	if (g_EngineClient->IsInGame() && g_LocalPlayer && g_Options.esp_enabled)
		Visuals::Get().AddToDrawList();

	if (g_EngineClient->IsInGame() && g_LocalPlayer && g_Options.other_drawfov)
		Visuals::Get().DrawFOV();

	extern float side;

	if (g_EngineClient->IsInGame() && g_LocalPlayer && g_Options.misc_desync) {
		int PosDesyncY;

		if (g_Options.misc_watermark)
			PosDesyncY = 25;
		else PosDesyncY = 5;

		Render::Get().RenderText(side > 0.0f ? "LEFT" : "RIGHT", 8, PosDesyncY, 18.0f, Color::White);
	}
		

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


float Render::RenderText(const std::string& text, ImVec2 pos, float size, Color color, bool center, bool outline, ImFont* pFont)
{
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
	if (!pFont->ContainerAtlas) return 0.f;
	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

	if (center)
		pos.x -= textSize.x / 2.0f;

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

void Render::RenderCircle3D(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}
