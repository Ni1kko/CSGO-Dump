#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Options.hpp"

namespace ImGui
{
    void PushStyleScheme(const ImGuiStyle& style);
    void PopStyleScheme();

    inline bool BeginWindow(const char* title, bool* open, const ImVec2& start_pos, const ImVec2& size)
    {
        ImGui::SetNextWindowPos(start_pos, ImGuiSetCond_Once);
        ImGui::SetNextWindowSize(size, ImGuiSetCond_Once);
        return ImGui::Begin(
            title,
            open,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize);
    }

    inline void EndWindow() { ImGui::End(); }

    // Some wrappers that accept our Option class instead of bools and shit

    inline bool Checkbox(const char* label, Option* v)
    {
        auto b = v->get<bool>();
        if(Checkbox(label, &b)) {
            v->set(b);
            return true;
        }
        return false;
    }
    inline bool ColorEdit4(const char* label, Option* v, bool show_alpha = true)
    {
        auto clr = ImVec4(v->get_color());
        if(ColorEdit4(label, &clr.x, show_alpha)) {
            v->set_color(Color(clr.x, clr.y, clr.z, clr.w));
            return true;
        }
        return false;
    }
    inline bool ColorEdit3(const char* label, Option* v)
    {
        return ColorEdit4(label, v, false);
    }

    inline bool SliderFloat(const char* label, Option* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f)
    {
        float f = v->get_float();
        if(SliderFloat(label, &f, v_min, v_max, display_format, power)) {
            v->set_float(f);
            return true;
        }
        return false;
    }
    inline bool SliderAngle(const char* label, Option* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f)
    {
        float f = v_rad->get_float();
        if(SliderFloat(label, &f, v_degrees_min, v_degrees_max)) {
            v_rad->set_float(f);
            return true;
        }
        return false;
    }
    inline bool SliderInt(const char* label, Option* v, int v_min, int v_max, const char* display_format = "%.0f")
    {
        int i = v->get_int();
        if(SliderInt(label, &i, v_min, v_max, display_format)) {
            v->set_int(i);
            return true;
        }
        return false;
    }
    inline bool VSliderFloat(const char* label, const ImVec2& size, Option* v, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.0f)
    {
        float f = v->get_float();
        if(VSliderFloat(label, size, &f, v_min, v_max, display_format, power)) {
            v->set_float(f);
            return true;
        }
        return false;
    }
    inline bool VSliderInt(const char* label, const ImVec2& size, Option* v, int v_min, int v_max, const char* display_format = "%.0f")
    {
        int i = v->get_int();
        if(VSliderInt(label, size, &i, v_min, v_max, display_format)) {
            v->set_int(i);
            return true;
        }
        return false;
    }
    inline bool Hotkey(const char* label, Option* k, const ImVec2& size_arg = ImVec2(-1, 0))
    {
        int i = k->get_int();
        if (Hotkey(label, &i, size_arg)) {
            k->set_int(i);
            return true;
        }
        return false;
    }
    static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
    {
        const char* const* items = (const char* const*)data;
        if (out_text)
            *out_text = items[idx];
        return true;
    }
    inline bool Combo(const char* label, Option* k, const char* const* items, int items_count, int height_in_items = -1)
    {
        int i = k->get_int();
        if (Combo(label, &i, Items_ArrayGetter, (void*)items, items_count, height_in_items)) {
            k->set_int(i);
            return true;
        }
        return false;
    }
}