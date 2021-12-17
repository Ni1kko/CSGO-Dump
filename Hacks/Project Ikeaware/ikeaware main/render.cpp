#include "render.hpp"

#include <mutex>
#include <chrono>
#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "fonts/fonts.hpp"
#include <iomanip>
#include "helpers/math.hpp"
#include <ctime>
#include <chrono>

ImDrawListSharedData _data;

std::mutex render_mutex;

void Render::Initialize()
{
	ImGui::CreateContext();


	ImGui_ImplWin32_Init(g_InputSys->GetMainWindow());
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
		15.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	// esp font
	g_VisualsFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(u8"C:\\Windows\\Fonts\\Verdana.ttf", 12.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//watermark font
	g_WatermarkFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Fonts::Droid_compressed_data, Fonts::Droid_compressed_size, 18.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
}

void Render::ClearDrawList() {
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}
std::vector<int> data;
int GetFPS()
{
	int curFps = int(1.f / g_GlobalVars->absoluteframetime);

	data.push_back(curFps);
	if (data.size() > 10000)
		data.erase(data.begin());

	int absFps = 0;
	for (auto fps : data)
		absFps += fps;

	absFps /= data.size();

	return absFps;
}

void Render::DrawWatermark()
{
	int x, y;
	std::time_t cur_sec = std::time(0);
	std::tm* now = std::localtime(&cur_sec);
	std::ostringstream wtrstream;
	wtrstream << "ikeaware | fps: " << std::right << std::setw(3) << std::setfill('0') << std::to_string(GetFPS())
		<< " | time: " << std::setw(2) << std::setfill('0') << std::to_string(now->tm_hour) << ":" << std::setw(2) << std::setfill('0') << std::to_string(now->tm_min)
		<< ":" << std::setw(2) << std::setfill('0') << std::to_string(now->tm_sec);
	g_EngineClient->GetScreenSize(x, y);
	ImVec2 t = g_WatermarkFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, wtrstream.str().data());

	RenderBoxFilled(x - 187, 0 + 10, x - 173 + t.x, 0 + 17 + t.y, Color(12.5, 15.5, 62.5, 250));
	RenderBoxFilled(x - 185, 0 + 12, x - 175 + t.x, 0 + 15 + t.y, Color(25, 31, 125, 150));
	RenderTextNoOutline(wtrstream.str().data(), ImVec2(x - 180, 0 + 13), 12.f, Color::White);
}

void Render::BeginScene() {
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();


	if (g_Options.misc.watermark)
	{
		DrawWatermark();
	}

	if (g_EngineClient->IsInGame() && g_LocalPlayer)
	{
		g_Visuals->AddToDrawList();
		if (g_Options.misc.removals.no_scope)
		{
			if (g_LocalPlayer->IsAlive() && g_LocalPlayer->m_hActiveWeapon() && g_LocalPlayer->m_hActiveWeapon()->IsSniper() && g_LocalPlayer->m_bIsScoped())
			{
				int w, h;
				g_EngineClient->GetScreenSize(w, h);
				float centerX = static_cast<int>(w * 0.5f);
				float centerY = static_cast<int>(h * 0.5f);
				g_Render->RenderLine(0, centerY, w, centerY, Color(9, 11, 20, 255));
				g_Render->RenderLine(centerX, 0, centerX, h, Color(9, 11, 20, 255));
			}
		}
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

float Render::RenderTextNoOutline(const std::string& text, const ImVec2& pos, float size, Color color, bool center, ImFont* pFont)
{
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());

	if (!pFont->ContainerAtlas)
	{
		return 0.f;
	}

	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);
	if (center)
	{
		draw_list->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y), GetU32(color), text.c_str());
	}
	else
	{
		draw_list->AddText(pFont, size, ImVec2(pos.x, pos.y), GetU32(color), text.c_str());
	}

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
		if (!Math::WorldToScreen(start, start2d) || !Math::WorldToScreen(end, end2d))
			return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}
