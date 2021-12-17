#pragma once

#include "../ValveSDK/Math/Vector.hpp"
#include "../ValveSDK/interfaces/IVEngineClient.hpp"
#include "../ValveSDK/interfaces.hpp"

class C_BasePlayer;
class C_BaseEntity;
class C_BaseCombatWeapon;
class C_PlantedC4;
class Color;
class ClientClass;
struct ModelRenderInfo_t;

enum FontRenderFlag_t
{
    FONT_LEFT = 0,
    FONT_RIGHT = 1,
    FONT_CENTER = 2
};
extern vgui::HFont esp_font;
extern vgui::HFont defuse_font;
extern vgui::HFont dropped_weapons_font;
extern vgui::HFont weapons_font;

inline void Text(int x, int y, Color color, DWORD font, const char* text, ...)
{
    size_t origsize = strlen(text) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);
    g_VGuiSurface->DrawSetTextFont(font);
    g_VGuiSurface->DrawSetTextColor(color);
    g_VGuiSurface->DrawSetTextPos(x, y);
    g_VGuiSurface->DrawPrintText(wcstring, wcslen(wcstring));
}
inline void DrawString2(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
    va_list va_alist;
    char buf[1024];
    va_start(va_alist, msg);
    _vsnprintf(buf, sizeof(buf), msg, va_alist);
    va_end(va_alist);
    wchar_t wbuf[1024];
    MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

    int r = 255, g = 255, b = 255, a = 255;
    color.GetColor(r, g, b, a);

    auto width = 0;
    auto height = 0;
    g_VGuiSurface->GetTextSize(font, wbuf, width, height);

    if (alignment & FONT_RIGHT)
        x -= width;
    if (alignment & FONT_CENTER)
        x -= width / 2;

    g_VGuiSurface->DrawSetTextFont(font);
    g_VGuiSurface->DrawSetTextColor(r, g, b, a);
    g_VGuiSurface->DrawSetTextPos(x, y - height / 2);
    g_VGuiSurface->DrawPrintText(wbuf, wcslen(wbuf));
}
namespace visuals
{

    struct ESPBox
    {
        int x, right, y, w, h, bottom;
    };

    namespace player
    {
        void draw_player(C_BasePlayer * entity, player_info_t pinfo, C_BasePlayer * local);
        void player_box(float x, float y, float w, float h, Color clr);
        void draw_health(Vector bot, Vector top, int health);
        void dlight(C_BasePlayer* entity);
    }

    namespace misc
    {
        Color GetPlayerColor(C_BasePlayer * entity);
        void box_and_text(C_BasePlayer * entity, std::string text);
        void render_grenade(C_BasePlayer * entity);
        void draw_bomb(C_BasePlayer* entity, ClientClass* cClass);
        void draw_bomb_planted(C_BasePlayer* entity, ClientClass* cClass);
        void draw_drop(C_BasePlayer * entity, ClientClass * cClass);
        bool get_box(C_BasePlayer * entity, ESPBox & result);
        void night_mode();
        void wireframe_hands(const ModelRenderInfo_t& pInfo);

    }

    bool create_fonts();
    void destroy_fonts();
    void paint_traverse();
    void draw_model_execute(const ModelRenderInfo_t& pInfo);








}
