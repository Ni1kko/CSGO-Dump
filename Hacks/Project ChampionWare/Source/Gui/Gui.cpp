#include "Gui.h"
#include "../Globals/Globals.h"

using namespace Client;

//[enc_string_enable /]
//[junk_enable /]



static int iConfigSelect = 0;
static char ConfigName[64] = { 0 };

bool rage;
bool legit;
bool visuals;
bool misc;
bool skins;
bool colors;

bool bIsGuiInitalize = false;
bool bIsGuiVisible = true;
WNDPROC WndProc_o = nullptr;
static int hue = 140;




#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CGui::CGui() {}

ImFont* main;
ImFont* icons;

CGui::~CGui()
{
	ImGui_ImplDX9_Shutdown();
}

void CGui::GUI_Init(IDirect3DDevice9 * pDevice)
{
	//HWND hWindow = FindWindowA("Valve001", 0);

	//ImGui_ImplDX9_Init(hWindow, pDevice);

	//ImGuiIO& io = ImGui::GetIO();
	//ImGuiStyle& style = ImGui::GetStyle();

	//io.IniFilename = GuiFile.c_str();

	//ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf",
	//	12.f, 0, io.Fonts->GetGlyphRangesCyrillic());

	//io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 10);
	//io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 12);
	//io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 14);
	//io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 18);

	////main = io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 16.f);
	//main = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 10);
	//main = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 10);
	//main = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 12);
	//main = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 14);
	//main = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 16);
	//icons = io.Fonts->AddFontFromMemoryCompressedTTF(icon_compressed_data, icon_compressed_size, 19.f); // make this font a bit bigger for bigger icons

	//style.Alpha = 1.0f;             // Global alpha applies to everything in ImGui
	//style.WindowPadding = ImVec2(8, 8);      // Padding within a window
	//style.WindowRounding = 0.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	//style.WindowTitleAlign = ImVec2(0.5f, 0.5f);// Alignment for title bar text
	//style.FramePadding = ImVec2(4, 1);      // Padding within a framed rectangle (used by most widgets)
	//style.FrameRounding = 0.0f;             // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	//style.ItemSpacing = ImVec2(7, 4);     // Horizontal and vertical spacing between widgets/lines
	//style.ItemInnerSpacing = ImVec2(4, 4);      // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	//style.TouchExtraPadding = ImVec2(0, 0);      // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	//style.IndentSpacing = 21.0f;            // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	//style.ColumnsMinSpacing = 6.0f;             // Minimum horizontal spacing between two columns
	//style.ScrollbarSize = 10.0f;            // Width of the vertical scrollbar, Height of the horizontal scrollbar
	//style.ScrollbarRounding = 3.0f;             // Radius of grab corners rounding for scrollbar
	//style.GrabMinSize = 10.0f;            // Minimum width/height of a grab box for slider/scrollbar
	//style.GrabRounding = 0.0f;             // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	//style.ButtonTextAlign = ImVec2(0.5f, 0.5f);// Alignment of button text when button is larger than text.
	//style.DisplayWindowPadding = ImVec2(22, 22);    // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
	//style.DisplaySafeAreaPadding = ImVec2(4, 4);      // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	//style.AntiAliasedLines = true;             // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	//style.CurveTessellationTol = 1.25f;            // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

	//ImGui_ImplDX9_CreateDeviceObjects();

	//WndProc_o = (WNDPROC)SetWindowLongA(hWindow, GWL_WNDPROC, (LONG)(LONG_PTR)GUI_WndProc);

	//bIsGuiInitalize = true;

	HWND hWindow = FindWindowA("Valve001", 0);
	ImGui_ImplDX9_Init(hWindow, pDevice);
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.f / 60.f;
	D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
	pDevice->GetCreationParameters(&d3dcp);

	auto& style = ImGui::GetStyle();

	//Global::fDefault = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f);

	//Global::fTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\auifont.ttf", 62.0f);

	Global::fSubTabs = io.Fonts->AddFontFromMemoryCompressedTTF(astriumwep_compressed_data, astriumwep_compressed_size, 25.0f);

	Global::Tahomaregular = io.Fonts->AddFontFromMemoryCompressedTTF(Tahomaregular_compressed_data, Tahomaregular_compressed_size, 12.f); // make this font a bit bigger for bigger icons
	Global::icons = io.Fonts->AddFontFromMemoryCompressedTTF(auifont_compressed_data, auifont_compressed_size, 21.f); // make this font a bit bigger for bigger icons
	Global::fDefault = io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 15.f); // make this font a bit bigger for bigger icons
	Global::icons2 = io.Fonts->AddFontFromMemoryCompressedTTF(icon_compressed_data, icon_compressed_size, 22.f); // make this font a bit bigger for bigger icons
	Global::icons3 = io.Fonts->AddFontFromMemoryCompressedTTF(icomoon_compressed_data, icomoon_compressed_size, 21.f); // make this font a bit bigger for bigger icons
	io.IniFilename = GuiFile.c_str();


	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(31, 44, 54);
	ImVec4 col_area = ImColor(4, 32, 41);
	//ImVec4(1.0f, 0.63f, 0.5f, 1.00f);
	auto mColor = Settings::Misc::misc_themecolor;
	float themecolor[3];
	themecolor[0] = float(mColor[0]);
	themecolor[1] = float(mColor[1]);
	themecolor[2] = float(mColor[2]);

	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.95f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09f, .09f, .09f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13, 0.13, 0.13, 1.f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.11, 0.11, 0.11, 1.f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.15, 0.15, 0.15, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.50f, 0.50f, 0.50f, .3f); // combobox hover
	style.Colors[ImGuiCol_ComboSelectedText] = ImVec4(.52f, 0.f, 0.52f, .87f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .87f);
	style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.Alpha = .0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 4.0f; //4.0
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 0.0f; //2.0
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f; //16.0
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f; //16.0
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;

	ImGui_ImplDX9_CreateDeviceObjects();

	WndProc_o = (WNDPROC)SetWindowLongA(hWindow, GWL_WNDPROC, (LONG)(LONG_PTR)GUI_WndProc);

	bIsGuiInitalize = true;

}

void CGui::IconsFont()
{
	ImGui::PushFont(icons);
	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(31, 44, 54);
	ImVec4 col_area = ImColor(4, 32, 41);
	//ImVec4(1.0f, 0.63f, 0.5f, 1.00f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.95f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09f, .09f, .09f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13, 0.13, 0.13, 1.f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.11, 0.11, 0.11, 1.f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.63f, 0.5f, .7f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.63f, 0.5f, .7);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.63f, 0.5f, .7f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.63f, 0.5f, .7f);;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(.52f, 0.f, 0.52f, 1.f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.15, 0.15, 0.15, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.63f, 0.5f, 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.63f, 0.5f, 1.f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.63f, 0.5f, .7f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.50f, 0.50f, 0.50f, .3f); // combobox hover
	style.Colors[ImGuiCol_ComboSelectedText] = ImVec4(.52f, 0.f, 0.52f, .87f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.63f, 0.5f, .87f);
	style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.Alpha = .0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 4.0f; //4.0
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 0.0f; //2.0
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f; //16.0
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f; //16.0
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;
	
}

void CGui::ThemeColor()
{


	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(31, 44, 54);
	ImVec4 col_area = ImColor(4, 32, 41);
	//ImVec4(1.0f, 0.63f, 0.5f, 1.00f);
	auto mColor = Settings::Misc::misc_themecolor;
	float themecolor[3];
	themecolor[0] = float(mColor[0]);
	themecolor[1] = float(mColor[1]);
	themecolor[2] = float(mColor[2]);
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.95f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.27f, 0.27f, .27f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09f, .09f, .09f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13, 0.13, 0.13, 1.f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.11, 0.11, 0.11, 1.f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.15, 0.15, 0.15, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], 1.f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .7f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.50f, 0.50f, 0.50f, .3f); // combobox hover
	style.Colors[ImGuiCol_ComboSelectedText] = ImVec4(.52f, 0.f, 0.52f, .87f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(themecolor[0], themecolor[1], themecolor[2], .87f);
	style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.Alpha = .0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 4.0f; //4.0
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 0.0f; //2.0
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f; //16.0
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f; //16.0
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;


}

void TextColor(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
	{
		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
	}
	else
		style.Colors[ImGuiCol_Text] = ImVec4(0.39f, 0.39f, 0.39f, 1.f);
}



void BtnNormal()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.10, 0.10, 0.10, .98f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.f);
}

void BtnActive()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, .98f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
}

void DrawMiscTab() 
{
	auto& style = ImGui::GetStyle();
	BtnNormal();
	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(0, 4);

}
void DrawAimbot()
{
	
	auto& style = ImGui::GetStyle();
	BtnNormal();
	ImGui::PushFont(Global::fSubTabs);
	style.ItemSpacing = ImVec2(0, 4);

	if (Global::LegitSubTab == 0)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);

	if (ImGui::Button("G", ImVec2(128.5, 32)))
	{
		Global::LegitSubTab = 0;
		Settings::Aimbot::WeaponStr = "pistola";
	}
	ImGui::SameLine();

	if (Global::LegitSubTab == 1)

		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("P", ImVec2(128.5, 32)))
	{
		Global::LegitSubTab = 1;
		Settings::Aimbot::WeaponStr = "smg";
	}
	ImGui::SameLine();

	if (Global::LegitSubTab == 2)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("W", ImVec2(128.5, 32)))
	{
		Global::LegitSubTab = 2;
		Settings::Aimbot::WeaponStr = "rifle";
	}
	ImGui::SameLine();

	if (Global::LegitSubTab == 3)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("d", ImVec2(128.5, 32)))
	{
		Global::LegitSubTab = 3;
		Settings::Aimbot::WeaponStr = "escopeta";
	}
	ImGui::SameLine();

	if (Global::LegitSubTab == 4)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);

	if (ImGui::Button("f", ImVec2(128.5, 32)))
	{
		Global::LegitSubTab = 4;
		Settings::Aimbot::WeaponStr = "pesadas";
	}
	ImGui::SameLine();

	if (Global::LegitSubTab == 5)
		style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(.3f, .3f, .3f, 1.f);
	if (ImGui::Button("a", ImVec2(128.5, 32)))
	{
		Global::LegitSubTab = 5;
		Settings::Aimbot::WeaponStr = "francos";
	}


	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::PushFont(Global::fDefault);

	

	
		
		
		
	
	
	
}

void DrawSkinsTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();
	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(0, 4);

	
}

void DrawVisualsTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	
	
	


}

void DrawConfigTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	


}
void DrawColorTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	
}

void DrawColorsTab()
{
	ImGui::BeginChild("##visualscolors", ImVec2(772, 434), true);
	{
		static int selectedItem = 0;

		ImVec2 lastCursor = ImGui::GetCursorPos();

		auto& style = ImGui::GetStyle();
		BtnNormal();

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);

		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);

		ImGui::ListBoxHeader("##Colors - Visuals", ImVec2(320, 418));

		
		ImGui::ListBoxFooter();


		float nc = lastCursor.x + 260;
		ImGui::SetCursorPos(ImVec2(nc, lastCursor.y));

		
		

		bool wip;

		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
		ImGui::SetCursorPosX(nc);


		

		

		ImVec2 curPos = ImGui::GetCursorPos();
		ImVec2 curWindowPos = ImGui::GetWindowPos();
		curPos.x += curWindowPos.x;
		curPos.y += curWindowPos.y;

		
		

	}ImGui::EndChild();


}

void CGui::GUI_Begin_Render()
{
	ImGui_ImplDX9_NewFrame();
}

void CGui::GUI_End_Render()
{
	ImGui::Render();
}

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	static bool bDown = false;
	static bool bClicked = false;
	static bool bPrevMenuState = bIsGuiVisible;
	static ConVar * men = Interfaces::GetConVar()->FindVar("cl_mouseenable");
	
	if (GUI_KEY_DOWN(VK_INSERT))
	{
		bClicked = false;
		bDown = true;
	}
	else if (!GUI_KEY_DOWN(VK_INSERT) && bDown)
	{
		bClicked = true;
		bDown = false;
	}
	else
	{
		bClicked = false;
		bDown = false;
	}

	if (bClicked)
	{
		bIsGuiVisible = !bIsGuiVisible;
		men->SetValue(bIsGuiVisible);
	}

	if (bPrevMenuState != bIsGuiVisible)
	{
		men->SetValue(!bIsGuiVisible);
	}
	bPrevMenuState = bIsGuiVisible;

	if (bIsGuiVisible && ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProcA(WndProc_o, hwnd, uMsg, wParam, lParam);
}


void CGui::GUI_Draw_Elements()
{
	if (!bIsGuiInitalize || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsActiveApp())
		return;

	g_pGui->GUI_Begin_Render();

	bool bOpenTimer = (bIsGuiVisible || (bC4Timer && iC4Timer));

	if (Settings::Misc::misc_Spectators) g_pMisc->OnRenderSpectatorList();

	if (g_pEsp && Settings::Esp::esp_BombTimer && bOpenTimer && Interfaces::Engine()->IsInGame())
	{
		if (g_pEsp && Settings::Esp::esp_BombTimer && bOpenTimer)
		{
			g_pRender->Text(15, 480, false, true, Color::Red(), C4_TIMER_STRING, g_pEsp->fC4Timer);
			g_pRender->Text(15, 481, false, true, Color::Red(), "_____________");
		}
		else
		{
			//off
		}
	}
	if (Interfaces::Engine()->IsInGame())
	{
		if (Settings::Radar::rad_InGame && GetKeyState(Settings::Radar::rad_InGameKey))
		{
			g_pRender->Text(15, 465, false, true, Color::Aqua(), "RADAR: ON");
		}
		else if (Settings::Radar::rad_InGame)
		{
			g_pRender->Text(15, 465, false, true, Color::Aqua(), "RADAR: OFF");
		}
	}

	if (g_pEsp && Settings::Esp::esp_BombTimer && bOpenTimer)
	{
		ImVec2 OldMinSize = ImGui::GetStyle().WindowMinSize;

		ImGui::GetStyle().WindowMinSize = ImVec2(0.f, 0.f);

		ImGui::SetNextWindowSize(ImVec2(125.f, 30.f));

		ImGui::GetStyle().WindowMinSize = OldMinSize;
	}

	if (bIsGuiVisible)
	{
		int pX, pY;
		Interfaces::InputSystem()->GetCursorPosition(&pX, &pY);

		ImGui::GetIO().MousePos.x = (float)pX;
		ImGui::GetIO().MousePos.y = (float)pY;
		
		Client::OnRenderGUI();
	}

	if (g_pRadar)
		g_pRadar->OnRender();

	//g_pGui->MainFont();

	g_pGui->GUI_End_Render();
}


namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ListBoxConfigArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), 9);
	}

	IMGUI_API bool ComboBoxArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	IMGUI_API bool TabLabels(const char **tabLabels, int tabSize, int &tabIndex, int *tabOrder)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		const ImVec2 itemSpacing = style.ItemSpacing;
		const ImVec4 color = style.Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
		const ImVec4 colorText = style.Colors[ImGuiCol_Text];
		style.ItemSpacing.x = 1;
		style.ItemSpacing.y = 1;
		const ImVec4 colorSelectedTab = ImVec4(color.x, color.y, color.z, color.w*0.5f);
		const ImVec4 colorSelectedTabHovered = ImVec4(colorHover.x, colorHover.y, colorHover.z, colorHover.w*0.5f);
		const ImVec4 colorSelectedTabText = ImVec4(colorText.x*0.8f, colorText.y*0.8f, colorText.z*0.8f, colorText.w*0.8f);

		if (tabSize > 0 && (tabIndex < 0 || tabIndex >= tabSize))
		{
			if (!tabOrder)
			{
				tabIndex = 0;
			}
			else
			{
				tabIndex = -1;
			}
		}

		float windowWidth = 0.f, sumX = 0.f;
		windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY() > 0 ? style.ScrollbarSize : 0.f);

		static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;
		static ImVec2 draggingtabSize(0, 0);
		static ImVec2 draggingTabOffset(0, 0);

		const bool isMMBreleased = ImGui::IsMouseReleased(2);
		const bool isMouseDragging = ImGui::IsMouseDragging(0, 2.f);
		int justClosedTabIndex = -1, newtabIndex = tabIndex;

		bool selection_changed = false; bool noButtonDrawn = true;

		for (int j = 0, i; j < tabSize; j++)
		{
			i = tabOrder ? tabOrder[j] : j;
			if (i == -1) continue;

			if (sumX > 0.f)
			{
				sumX += style.ItemSpacing.x;
				sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f*style.FramePadding.x;

				if (sumX > windowWidth)
				{
					sumX = 0.f;
				}
				else
				{
					ImGui::SameLine();
				}
			}

			if (i != tabIndex)
			{
				// Push the style
				style.Colors[ImGuiCol_Button] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
				style.Colors[ImGuiCol_Text] = colorSelectedTabText;
			}
			// Draw the button
			ImGui::PushID(i);   // otherwise two tabs with the same name would clash.
			if (ImGui::Button(tabLabels[i], ImVec2(216.f, 50.f))) { selection_changed = (tabIndex != i); newtabIndex = i; } //216, 50 //165,30
			ImGui::PopID();
			if (i != tabIndex)
			{
				// Reset the style
				style.Colors[ImGuiCol_Button] = color;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				style.Colors[ImGuiCol_Text] = colorText;
			}
			noButtonDrawn = false;

			if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line

			if (ImGui::IsItemHoveredRect())
			{
				if (tabOrder)
				{
					// tab reordering
					if (isMouseDragging)
					{
						if (draggingTabIndex == -1)
						{
							draggingTabIndex = j;
							draggingtabSize = ImGui::GetItemRectSize();
							const ImVec2& mp = ImGui::GetIO().MousePos;
							const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
							draggingTabOffset = ImVec2(
								mp.x + draggingtabSize.x*0.5f - sumX + ImGui::GetScrollX(),
								mp.y + draggingtabSize.y*0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
							);

						}
					}
					else if (draggingTabIndex >= 0 && draggingTabIndex < tabSize && draggingTabIndex != j)
					{
						draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
					}
				}
			}

		}

		tabIndex = newtabIndex;


		// Reset draggingTabIndex if necessary
		if (!isMouseDragging) draggingTabIndex = -1;

		// Change selected tab when user closes the selected tab
		if (tabIndex == justClosedTabIndex && tabIndex >= 0)
		{
			tabIndex = -1;
			for (int j = 0, i; j < tabSize; j++)
			{
				i = tabOrder ? tabOrder[j] : j;
				if (i == -1)
				{
					continue;
				}
				tabIndex = i;
				break;
			}
		}

		// Restore the style
		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		style.Colors[ImGuiCol_Text] = colorText;
		style.ItemSpacing = itemSpacing;

		return selection_changed;
	}
}


//void CGui::IconsFont()
//{
//	ImGui::PushFont(icons);
//	ImGuiStyle& style = ImGui::GetStyle();
//
//
//	style.Alpha = 1.0f;             // Global alpha applies to everything in ImGui
//	style.WindowPadding = ImVec2(8, 8);      // Padding within a window
//	style.WindowMinSize = ImVec2(32, 32);
//	style.WindowRounding = 10.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows
//	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);// Alignment for title bar text
//	style.FramePadding = ImVec2(4, 2);      // Padding within a framed rectangle (used by most widgets)
//	style.FrameRounding = 10.0f;             // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
//
//	style.ItemSpacing = ImVec2(7, 4);      // Horizontal and vertical spacing between widgets/lines
//	style.ItemInnerSpacing = ImVec2(4, 4);      // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
//	style.TouchExtraPadding = ImVec2(0, 0);      // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
//	style.IndentSpacing = 21.0f;            // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
//	style.ColumnsMinSpacing = 3.0f;             // Minimum horizontal spacing between two columns
//	style.ScrollbarSize = 12.0f;            // Width of the vertical scrollbar, Height of the horizontal scrollbar
//	style.ScrollbarRounding = 0.0f;             // Radius of grab corners rounding for scrollbar
//	style.GrabMinSize = 7.0f;            // Minimum width/height of a grab box for slider/scrollbar
//	style.GrabRounding = 3.0f;             // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
//	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);// Alignment of button text when button is larger than text.
//	style.DisplayWindowPadding = ImVec2(22, 22);    // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
//	style.DisplaySafeAreaPadding = ImVec2(4, 4);      // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
//	style.AntiAliasedLines = true;             // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
//	style.CurveTessellationTol = 1.25f;            // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
//
//	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
//	style.Colors[ImGuiCol_WindowBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
//	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
//	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.82f, 0.82f, 0.83f, 0.40f);
//	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.82f, 0.82f, 0.84f, 0.40f);
//	style.Colors[ImGuiCol_TitleBg] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_TitleBgActive] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.0f, 0.0f, 0.96f);
//	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(101, 112, 142);
//	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(97, 107, 135);
//	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(90, 100, 127);
//	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.92f, 0.90f, 0.90f, 0.00f);
//	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.00f);
//	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.00f);
//	style.Colors[ImGuiCol_CheckMark] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_SliderGrab] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(27, 178, 217);
//	style.Colors[ImGuiCol_Button] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_ButtonHovered] = ImColor(31, 143, 207);
//	style.Colors[ImGuiCol_ButtonActive] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_Header] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_HeaderHovered] = ImColor(31, 143, 207);
//	style.Colors[ImGuiCol_HeaderActive] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
//	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
//	style.Colors[ImGuiCol_ResizeGrip] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_ResizeGripActive] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.0f, 0.0f, 0.86f);
//	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.87f, 0.87f, 0.87f, 0.87f);
//	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.87f);
//	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
//	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.09f, 0.43f, 0.0f);
//}
//
//void CGui::MainFont()
//{
//	ImGui::PushFont(main);
//	ImGuiStyle& style = ImGui::GetStyle();
//
//	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
//	style.Colors[ImGuiCol_WindowBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_Border] = ImColor(77, 86, 115);
//	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
//	style.Colors[ImGuiCol_FrameBg] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(77, 86, 115);
//	style.Colors[ImGuiCol_FrameBgActive] = ImColor(70, 79, 104);
//	style.Colors[ImGuiCol_TitleBg] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_TitleBgActive] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.0f, 0.0f, 0.96f);
//	style.Colors[ImGuiCol_ScrollbarBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(101, 112, 142);
//	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(97, 107, 135);
//	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(90, 100, 127);
//	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.92f, 0.90f, 0.90f, 0.00f);
//	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.00f);
//	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.00f);
//	style.Colors[ImGuiCol_CheckMark] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.67f, 0.67f, 0.67f, 0.32f); //main half
//	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.67f, 0.67f, 0.67f, 2.f);
//	style.Colors[ImGuiCol_Button] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_ButtonHovered] = ImColor(31, 143, 207);
//	style.Colors[ImGuiCol_ButtonActive] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_Header] = ImColor(31, 143, 207); //Âûáîð â ëèñòå (Ïîäñâåòêà)
//	style.Colors[ImGuiCol_HeaderHovered] = ImColor(31, 143, 207);
//	style.Colors[ImGuiCol_HeaderActive] = ImColor(25, 176, 215);
//	style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
//	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
//	style.Colors[ImGuiCol_ResizeGrip] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_ResizeGripActive] = ImColor(64, 72, 95);
//	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
//	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.0f, 0.0f, 0.86f);
//	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.87f, 0.87f, 0.87f, 0.87f);
//	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.87f);
//	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.35f);
//	style.Colors[ImGuiCol_PopupBg] = ImColor(53, 59, 81);
//	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.09f, 0.43f, 0.0f);
//
//	//style.Alpha = 1.0f;             // Global alpha applies to everything in ImGui
//	style.WindowPadding = ImVec2(8, 8);      // Padding within a window
//	style.WindowMinSize = ImVec2(32, 32);
//	style.WindowRounding = 10.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows
//	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);// Alignment for title bar text
//	style.FramePadding = ImVec2(4, 2);      // Padding within a framed rectangle (used by most widgets)
//	style.FrameRounding = 10.0f;             // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
//
//	style.ItemSpacing = ImVec2(7, 4);      // Horizontal and vertical spacing between widgets/lines
//	style.ItemInnerSpacing = ImVec2(4, 4);      // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
//	style.TouchExtraPadding = ImVec2(0, 0);      // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
//	style.IndentSpacing = 21.0f;            // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
//	style.ColumnsMinSpacing = 3.0f;             // Minimum horizontal spacing between two columns
//	style.ScrollbarSize = 12.0f;            // Width of the vertical scrollbar, Height of the horizontal scrollbar
//	style.ScrollbarRounding = 0.0f;             // Radius of grab corners rounding for scrollbar
//	style.GrabMinSize = 7.0f;            // Minimum width/height of a grab box for slider/scrollbar
//	style.GrabRounding = 3.0f;             // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
//	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);// Alignment of button text when button is larger than text.
//	style.DisplayWindowPadding = ImVec2(22, 22);    // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
//	style.DisplaySafeAreaPadding = ImVec2(4, 4);      // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
//	style.AntiAliasedLines = true;             // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
//	style.CurveTessellationTol = 1.25f;            // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
//}

