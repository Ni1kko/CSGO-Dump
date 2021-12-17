#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "ValveSDK/csgostructs.hpp"
#include "Options.hpp"
#include "Helpers/input_system.hpp"
#include "imgui/directx9/imgui_impl_dx9.h"
#include "Helpers/utils.hpp"
static auto cl_mouseenable = (ConVar*)nullptr;

static const char* sidebar_tabs[] = {
    "VISUALS",
    "AIMBOT",
    "MISC",
    "SKINS",
    "PLAYERS",
    "CONFIG"
};

static int active_sidebar_tab = 0;

constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return 50.0f; }

const ImVec2& get_sidebar_size()
{
    constexpr float padding = 10.0f;
    constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
    constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();

    static auto size = ImVec2{ size_w, ImMax(325.0f, size_h) };
    return size;
}

void menu::initialize(IDirect3DDevice9* device)
{
    m_bVisible = false;
    m_pDevice = device;

    cl_mouseenable = g_CVar->FindVar("cl_mouseenable");

    ImGui_ImplDX9_Init(input_system::instance().get_window(), device);

    create_style();
}

void menu::shutdown()
{
    ImGui_ImplDX9_Shutdown();
}

void menu::on_device_lost()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void menu::on_device_reset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}

void menu::render()
{
    using namespace std::chrono;
    static int count = 0;
    static auto last = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    static int fps = 0;

    count++;

    if(duration_cast<milliseconds>(now - last).count() > 1000) {
        fps = count;
        count = 0;
        last = now;
    }

    ImGui_ImplDX9_NewFrame();

    ImGui::GetIO().MouseDrawCursor = m_bVisible;

    ImGui::PushStyleScheme(m_Style);

    if(!m_bVisible) {
        ImGui::PopStyleScheme();
        return;
    }

    ImGui::BeginWindow("memehook", &m_bVisible, ImVec2{ 0, 0 }, ImVec2{ 1000, 0 });

    render_sidebar();
    render_body();

    ImGui::TextColored(ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f }, "FPS: %03d", fps);
    ImGui::SameLine(ImGui::GetWindowWidth() - 150 - ImGui::GetStyle().WindowPadding.x);
    if(ImGui::Button("Unload", ImVec2{ 150, 25 })) {
        g_Unload = true;
    }
    ImGui::EndWindow();
    ImGui::PopStyleScheme();

    ImGui::Render();
}

template<size_t N>
void render_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    bool values[N] = { false };

    values[activetab] = true;

    for(auto i = 0; i < N; ++i) {
        if(ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if(sameline && i < N - 1)
            ImGui::SameLine();
    }
}

void menu::render_sidebar()
{
    const auto sidebar_size = get_sidebar_size();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
    {
        ImGui::BeginGroupBox("##sidebar", sidebar_size);
        {
            ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

            render_tabs(sidebar_tabs, active_sidebar_tab, get_sidebar_item_width(), get_sidebar_item_height(), false);
        }
        ImGui::EndGroupBox();
    }
    ImGui::PopStyleVar();
    ImGui::SameLine();
}
const char* antiaims[] =
{
    "Off",
    "FakeSideways",
    "Spin Slow",
    "Spin Fast",
    "Backwards",
    "tank aa"
};
const char* antiaimpitch[] =
{
    "Off",
    "Down",
    "Up"
};
const char* aimBones[] =
{
    "Head",
    "Neck",
    "Chest",
    "Stomach",
    "Nearest"
};
const char* hitscan[] =
{
    "Off",
    "Low",
    "Medium",
    "High"
};

const char* knives[] = { "Default", "Bayonet", "Bowie Knife", "Butterfly Knife", "Falchion Knife", "Flip Knife", "Gut Knife", "Huntsman Knife", "Karambit", "M9 Bayonet","Shadow Daggers" };
void menu::render_body()
{
    static const char* esp_tab_names[] = { "ESP", "GLOW", "CHAMS" };
    static const char* aim_tab_names[] = { "LEGIT", "RAGE" };
    static const char* aim_legit_weapon_names[] = { "RIFLE", "PISTOL", "SNIPER" };

    static int active_esp_tab = 0;
    static int active_aim_tab = 0;
    static int active_legit_weapon = 0;
    bool placeholder_true = true;

    auto& style = ImGui::GetStyle();
    auto size = get_sidebar_size();
    size.x = 0.0f;

    ImGui::BeginGroupBox("##body", size);
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
    if(active_sidebar_tab == 0) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        {
            render_tabs(esp_tab_names, active_esp_tab, group_w / _countof(esp_tab_names), 25.0f, true);
        }
        ImGui::PopStyleVar();
        ImGui::BeginGroupBox("##body_content");
        {
            if(active_esp_tab == 0) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
                ImGui::Columns(3, NULL, false);
                ImGui::SetColumnOffset(1, group_w / 3.0f);
                ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
                ImGui::SetColumnOffset(3, group_w);

                ImGui::Checkbox("Enabled", &g_Options.esp_enabled);
                ImGui::Checkbox("Team check", &g_Options.esp_poland_filter_enemies_only);
                ImGui::Checkbox("Boxes", &g_Options.esp_poland_box);
                ImGui::Checkbox("Names", &g_Options.esp_poland_name);
                ImGui::Checkbox("Health", &g_Options.esp_poland_hp);
                ImGui::Checkbox("Weapon", &g_Options.esp_poland_weapon);
                ImGui::Checkbox("C4", &g_Options.esp_poland_c4);
                ImGui::Checkbox("DLight", &g_Options.esp_poland_dlight);

                ImGui::NextColumn();

                ImGui::Checkbox("Dropped Weapons", &g_Options.esp_poland_worldweapon);
                ImGui::Checkbox("Planted C4", &g_Options.esp_poland_c4world);
                ImGui::Checkbox("Spread Crosshair", &g_Options.esp_poland_spread_crosshair);
                ImGui::Checkbox("No Scope Border", &g_Options.esp_poland_no_scope_border);
                ImGui::Checkbox("Grenade ESP", &g_Options.esp_poland_grenade);
                ImGui::Checkbox("Grenade Prediction", &g_Options.esp_grenade_prediction);
                ImGui::Checkbox("NoSmoke", &g_Options.visual_no_smoke);
                ImGui::Checkbox("Show Backtrack", &g_Options.legitbot_backtrack);

                ImGui::NextColumn();

                ImGui::PushItemWidth(100);
                ImGui::ColorEditMode(ImGuiColorEditMode_HEX);

                ImGui::PopItemWidth();

                ImGui::Columns(1, NULL, false);
                ImGui::PopStyleVar();
            } else if(active_esp_tab == 1) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
                ImGui::Columns(3, NULL, false);
                ImGui::SetColumnOffset(1, group_w / 3.0f);
                ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
                ImGui::SetColumnOffset(3, group_w);

                ImGui::Checkbox("Enabled", &g_Options.glow_enabled);
                ImGui::Checkbox("Team check", &g_Options.glow_enemies_only);
                ImGui::Checkbox("Players", &g_Options.glow_players);
                ImGui::Checkbox("Weapons", &g_Options.glow_weapons);


                ImGui::NextColumn();

                ImGui::PushItemWidth(100);
                ImGui::ColorEditMode(ImGuiColorEditMode_HEX);
                ImGui::PopItemWidth();

                ImGui::NextColumn();

                ImGui::Columns(1, NULL, false);
                ImGui::PopStyleVar();
            } else if(active_esp_tab == 2) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
                ImGui::Columns(3, NULL, false);
                ImGui::SetColumnOffset(1, group_w / 3.0f);
                ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
                ImGui::SetColumnOffset(3, group_w);

                ImGui::Columns(1, NULL, false);
                ImGui::PopStyleVar();
            }
        }
        ImGui::EndGroupBox();
    } else if(active_sidebar_tab == 1) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        {
            render_tabs(aim_tab_names, active_aim_tab, group_w / _countof(aim_tab_names), 25.0f, true);
        }
        ImGui::PopStyleVar();

        ImGui::BeginGroupBox("##body_content");
        {
            auto tabsize = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });


            if(active_aim_tab == 0) 
            {
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, style.ItemSpacing.y });
                    render_tabs(aim_legit_weapon_names, active_legit_weapon, tabsize / _countof(aim_legit_weapon_names), 25.0f, true);
                    ImGui::Columns(3, NULL, false);
                    ImGui::SetColumnOffset(1, tabsize / 3.0f);
                    ImGui::SetColumnOffset(2, 2 * tabsize / 3.0f);
                    ImGui::SetColumnOffset(3, tabsize);
                    ImGui::PopStyleVar();
                    ImGui::Checkbox("Enable", &g_Options.legitbot_enabled);
                    ImGui::PushID("foo");
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::Hotkey("Hotkey       ", &g_Options.legitbot_key_rifle);
                        ImGui::SliderFloat("Smooth       ", &g_Options.legitbot_smooth_rifle, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("FOV          ", &g_Options.legitbot_fov_rifle, 0.00f, 30.00f, "%.2f");
                        ImGui::SliderFloat("Random Smooth", &g_Options.legitbot_randomized_smooth_rifle, 1.00f, 10.00f, "%.2f");
                        ImGui::SliderFloat("Min RCS      ", &g_Options.legitbot_randomized_recoil_min_rifle, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("Max RCS      ", &g_Options.legitbot_randomized_recoil_max_rifle, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("Random Angle ", &g_Options.legitbot_randomize_angle_rifle, 1.00f, 20.00f, "%.2f");
                        ImGui::PopItemWidth();
                    }
                    ImGui::PopID();
                    ImGui::NextColumn();
                }
                {
                    ImGui::PushID("bar");
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::NewLine();
                        ImGui::Hotkey("Hotkey       ", &g_Options.legitbot_key_pistol);
                        ImGui::SliderFloat("Smooth       ", &g_Options.legitbot_smooth_pistol, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("FOV          ", &g_Options.legitbot_fov_pistol, 0.00f, 30.00f, "%.2f");
                        ImGui::SliderFloat("Random Smooth", &g_Options.legitbot_randomized_smooth_pistol, 1.00f, 10.00f, "%.2f");
                        ImGui::SliderFloat("Min RCS      ", &g_Options.legitbot_randomized_recoil_min_pistol, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("Max RCS      ", &g_Options.legitbot_randomized_recoil_max_pistol, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("Random Angle ", &g_Options.legitbot_randomize_angle_pistol, 1.00f, 20.00f, "%.2f");
                        ImGui::PopItemWidth();
                    }
                    ImGui::PopID();
                    ImGui::NextColumn();
                }
                {
                    ImGui::PushID("zoo");
                    {
                        ImGui::PushItemWidth(-1);
                        ImGui::NewLine();
                        ImGui::Hotkey("Hotkey       ", &g_Options.legitbot_key_sniper);
                        ImGui::SliderFloat("Smooth       ", &g_Options.legitbot_smooth_sniper, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("FOV          ", &g_Options.legitbot_fov_sniper, 0.00f, 30.00f, "%.2f");
                        ImGui::SliderFloat("Random Smooth", &g_Options.legitbot_randomized_smooth_sniper, 1.00f, 10.00f, "%.2f");
                        ImGui::SliderFloat("Min RCS      ", &g_Options.legitbot_randomized_recoil_min_sniper, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("Max RCS      ", &g_Options.legitbot_randomized_recoil_max_sniper, 1.00f, 100.00f, "%.2f");
                        ImGui::SliderFloat("Random Angle ", &g_Options.legitbot_randomize_angle_sniper, 1.00f, 20.00f, "%.2f");
                        ImGui::PopItemWidth();
                    }
                    ImGui::PopID();
                    ImGui::NextColumn();
                }               
                ImGui::Checkbox("Triggerbot   ", &g_Options.triggerbot_enabled);
                ImGui::SliderFloat(("Delay        "), &g_Options.triggerbot_delay, 1.f, 150.f, "%.0f");
                ImGui::Hotkey(("Key          "), &g_Options.triggerbot_key);
                ImGui::SliderFloat(("Hitchance    "), &g_Options.triggerbot_hitchance, 0.f, 100.f, "%.0f");
                ImGui::Text(("Filter       "));

                //ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 5));
               // {
                    ImGui::Checkbox(("Head         "), &g_Options.triggerbot_Head);
                    ImGui::Checkbox(("Arms         "), &g_Options.triggerbot_Arms);
                    ImGui::Checkbox(("Chest        "), &g_Options.triggerbot_Chest);
                    ImGui::Checkbox(("Stomach      "), &g_Options.triggerbot_Stomach);
                    ImGui::Checkbox(("Legs         "), &g_Options.triggerbot_Legs);
                //} ImGui::EndChild();
            }


            ImGui::Columns(1, NULL, false);
            ImGui::PopStyleVar();
        }
        ImGui::EndGroupBox();
    } else if(active_sidebar_tab == 2) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        ImGui::ToggleButton("MISC", &placeholder_true, ImVec2{ group_w, 25.0f });
        ImGui::PopStyleVar();

        ImGui::BeginGroupBox("##body_content");
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, NULL, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Checkbox("Bunny hop", &g_Options.misc_bhop);
            ImGui::Checkbox("No Visual Recoil", &g_Options.no_visual_recoil);
            ImGui::Checkbox("AutoStrafe", &g_Options.auto_strafe);

            ImGui::Text(("Custom Name"));
            ImGui::PushItemWidth(198);
            static char buf1[128] = ""; ImGui::InputText(("##Name"), buf1, 128, ImGuiInputTextFlags_AllowTabInput);
            ImGui::SameLine();
            ImGui::PushItemWidth(50);
            if (ImGui::Button(("Set##0")))
            {
                utils::set_name(buf1);
            }

            ImGui::Text(("Custom ClanTag"));
            ImGui::PushItemWidth(198);
            static char buf2[17] = ""; ImGui::InputText(("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
            ImGui::SameLine();
            ImGui::PushItemWidth(50);
            if (ImGui::Button(("Set##1")))
            {
                utils::set_clantag(buf2);
            }
            ImGui::PushItemWidth(-1.0f);
            ImGui::SliderInt("Min strafe VEL ", &g_Options.min_velocity_auto_strafe, 0, 300);
            ImGui::SliderInt("Field of View  ", &g_Options.fov_cs_debug, 60, 120);
            ImGui::SliderInt("Viewmodel FoV  ", &g_Options.viewmodel_fov, 68, 120);

            ImGui::NextColumn();
            ImGui::SliderInt("FakeLag        ", &g_Options.misc_fakelag, 0, 15);
            ImGui::Checkbox("Adaptive Fakelag", &g_Options.misc_adaptive_fakelag);
            ImGui::Checkbox("Wireframe Hands ", &g_Options.misc_wireframe_hands);
            if (g_Options.misc_nightmode.get_bool() && !g_EngineClient->IsInGame())
                g_Options.misc_nightmode.set_bool(false);
            ImGui::Checkbox("NightMode       ", &g_Options.misc_nightmode);

            ImGui::NextColumn();
            ImGui::PopItemWidth();

            ImGui::Columns(1, NULL, false);
            ImGui::PopStyleVar();

        }
        ImGui::EndGroupBox();
    } else if(active_sidebar_tab == 3) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        ImGui::ToggleButton("SKINCHANGER", &placeholder_true, ImVec2{ group_w, 25.0f });
        ImGui::PopStyleVar();
        ImGui::BeginGroupBox("##body_content");
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, NULL, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Combo(("Knife Model"), &g_Options.knife_model, knives, ARRAYSIZE(knives));
            if(ImGui::Button("Force Update"))
            {
                g_ClientState->ForceFullUpdate();
            }

            ImGui::Columns(1, NULL, false);
            ImGui::PopStyleVar();
        }
        ImGui::EndGroupBox();
    } else if(active_sidebar_tab == 4) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        ImGui::ToggleButton("PLAYER LIST", &placeholder_true, ImVec2{ group_w, 25.0f });
        ImGui::PopStyleVar();
        ImGui::BeginGroupBox("##body_content");
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, NULL, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Columns(1, NULL, false);
            ImGui::PopStyleVar();
        }
        ImGui::EndGroupBox();
    } else if(active_sidebar_tab == 5) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        ImGui::ToggleButton("CONFIGURATION", &placeholder_true, ImVec2{ group_w, 25.0f });
        ImGui::PopStyleVar();

        ImGui::BeginGroupBox("##body_content");
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, NULL, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            static char buf[128] = "";
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##configname", buf, 128);
            if(ImGui::Button("Create Profile", ImVec2{ -1.0f, 25.0f })) {
                int idx = g_ConfigScheme.add_config(buf);
                g_ConfigScheme.set_active_config(idx);
            }
            ImGui::PopItemWidth();

            ImGui::NextColumn();

            ImGui::PushItemWidth(-1);
            if(ImGui::Combo("##configlist", &g_ConfigScheme.current_config_index, ConfigScheme::item_getter, &g_ConfigScheme, g_ConfigScheme.get_config_list().size())) {
                g_ConfigScheme.set_active_config(g_ConfigScheme.current_config_index);
            }
            if(ImGui::Button("Save config", ImVec2{ -1.0f, 25.0f })) {
                g_ConfigScheme.save_current_config();
            }
            ImGui::PopItemWidth();

            ImGui::NextColumn();

            ImGui::PushItemWidth(-1);
            if(ImGui::Button("Save Profiles", ImVec2{ -1.0f, 25.0f })) {
                g_ConfigScheme.save_config();
            }
            if(ImGui::Button("Load Profiles", ImVec2{ -1.0f, 25.0f })) {
                g_ConfigScheme.current_config_index = 0;
                g_ConfigScheme.load_config();
            }
            ImGui::PopItemWidth();

            ImGui::Columns(1, NULL, false);
            ImGui::PopStyleVar();
        }
        ImGui::EndGroupBox();
    }
    ImGui::EndGroupBox();
}

void menu::show()
{
    m_bVisible = true;
    cl_mouseenable->SetValue(false);
}

void menu::hide()
{
    m_bVisible = false;
    cl_mouseenable->SetValue(true);
}

void menu::toggle()
{
    m_bVisible = !m_bVisible;
    cl_mouseenable->SetValue(!m_bVisible);
}

void menu::create_style()
{
    m_Style.Alpha = 1.0f;                                // Global alpha applies to everything in ImGui
    m_Style.WindowPadding = ImVec2(10, 10);              // Padding within a window
    m_Style.WindowMinSize = ImVec2(100, 100);            // Minimum window size
    m_Style.WindowRounding = 0.0f;                       // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    m_Style.WindowTitleAlign = ImVec2(0.0f, 0.5f);       // Alignment for title bar text
    m_Style.ChildWindowRounding = 0.0f;                  // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
    m_Style.FramePadding = ImVec2(5, 5);                 // Padding within a framed rectangle (used by most widgets)
    m_Style.FrameRounding = 0.0f;                        // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
    m_Style.ItemSpacing = ImVec2(5, 5);                  // Horizontal and vertical spacing between widgets/lines
    m_Style.ItemInnerSpacing = ImVec2(4, 4);             // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    m_Style.TouchExtraPadding = ImVec2(0, 0);            // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    m_Style.IndentSpacing = 21.0f;                       // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    m_Style.ColumnsMinSpacing = 6.0f;                    // Minimum horizontal spacing between two columns
    m_Style.ScrollbarSize = 16.0f;                       // Width of the vertical scrollbar, Height of the horizontal scrollbar
    m_Style.ScrollbarRounding = 9.0f;                    // Radius of grab corners rounding for scrollbar
    m_Style.GrabMinSize = 10.0f;                         // Minimum width/height of a grab box for slider/scrollbar
    m_Style.GrabRounding = 0.0f;                         // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    m_Style.ButtonTextAlign = ImVec2(0.5f, 0.5f);        // Alignment of button text when button is larger than text.
    m_Style.DisplayWindowPadding = ImVec2(22, 22);       // Window positions are clamped to be is_visible within the display area by at least this amount. Only covers regular windows.
    m_Style.DisplaySafeAreaPadding = ImVec2(4, 4);       // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    m_Style.AntiAliasedLines = true;                     // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
    m_Style.AntiAliasedShapes = true;                    // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    m_Style.CurveTessellationTol = 1.25f;                // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

    m_Style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    m_Style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    m_Style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    m_Style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    m_Style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
    m_Style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    m_Style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    m_Style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    m_Style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    m_Style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    m_Style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    m_Style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    m_Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    m_Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    m_Style.Colors[ImGuiCol_TitleText] = ImVec4(0.80f, 0.80f, 1.00f, 1.00f);
    m_Style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
    m_Style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    m_Style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
    m_Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
    m_Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
    m_Style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
    m_Style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
    m_Style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    m_Style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    m_Style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    m_Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
    m_Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
    m_Style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
    m_Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
    m_Style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
    m_Style.Colors[ImGuiCol_Column] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    m_Style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    m_Style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    m_Style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    m_Style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    m_Style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    m_Style.Colors[ImGuiCol_CloseButton] = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
    m_Style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
    m_Style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
    m_Style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    m_Style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    m_Style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    m_Style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    m_Style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}