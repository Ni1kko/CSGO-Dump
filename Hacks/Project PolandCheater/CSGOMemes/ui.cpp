#include "ui.hpp"

#include "imgui/imgui_internal.h"

namespace ImGui
{
    void PushStyleScheme(const ImGuiStyle& style)
    {
        ImGuiContext& g = *GImGui;
        g.StyleStack.push_back(style);
        g.Style = &(g.StyleStack.back());
    }
    void PopStyleScheme()
    {
        ImGuiContext& g = *GImGui;
        g.StyleStack.pop_back();

        IM_ASSERT(g.StyleStack.Size > 0);

        g.Style = &(g.StyleStack.back());
    }
}