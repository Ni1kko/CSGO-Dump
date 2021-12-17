// dear imgui, v1.52 WIP
// (drawing and font code)

// Contains implementation for
// - ImDrawList
// - ImDrawData
// - ImFontAtlas
// - ImFont
// - Default font data

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

#include <stdio.h>      // vsnprintf, sscanf, printf
#if !defined(alloca)
#ifdef _WIN32
#include <malloc.h>     // alloca
#elif defined(__GLIBC__) || defined(__sun)
#include <alloca.h>     // alloca
#else
#include <stdlib.h>     // alloca
#endif
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is unsafe   // storing and comparing against same constants ok.
#pragma clang diagnostic ignored "-Wglobal-constructors"    // warning : declaration requires a global destructor           // similar to above, not sure what the exact difference it.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness             //
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"      // warning : macro name is a reserved identifier                //
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"       // warning: implicit conversion from 'float' to 'double' when passing argument to function
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"          // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wcast-qual"                // warning: cast from type 'xxxx' to type 'xxxx' casts away qualifiers
#endif

//-------------------------------------------------------------------------
// STB libraries implementation
//-------------------------------------------------------------------------

//#define IMGUI_STB_NAMESPACE     ImGuiStb
//#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
//#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION

#ifdef IMGUI_STB_NAMESPACE
namespace IMGUI_STB_NAMESPACE
{
#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4456)                             // declaration of 'xx' hides previous local declaration
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"              // warning: comparison is always true due to limited range of data type [-Wtype-limits]
#endif

#define STBRP_ASSERT(x)    IM_ASSERT(x)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#include "stb_rect_pack.h"

#define STBTT_malloc(x,u)  ((void)(u), ImGui::MemAlloc(x))
#define STBTT_free(x,u)    ((void)(u), ImGui::MemFree(x))
#define STBTT_assert(x)    IM_ASSERT(x)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#include "stb_truetype.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#ifdef IMGUI_STB_NAMESPACE
} // namespace ImGuiStb
using namespace IMGUI_STB_NAMESPACE;
#endif

//-----------------------------------------------------------------------------
// ImDrawList
//-----------------------------------------------------------------------------

static const ImVec4 GNullClipRect(-8192.0f, -8192.0f, +8192.0f, +8192.0f); // Large values that are easy to encode in a few bits+shift

void ImDrawList::Clear()
{
    CmdBuffer.resize(0);
    IdxBuffer.resize(0);
    VtxBuffer.resize(0);
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.resize(0);
    _TextureIdStack.resize(0);
    _Path.resize(0);
    _ChannelsCurrent = 0;
    _ChannelsCount = 1;
    // NB: Do not clear channels so our allocations are re-used after the first frame.
}

void ImDrawList::ClearFreeMemory()
{
    CmdBuffer.clear();
    IdxBuffer.clear();
    VtxBuffer.clear();
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.clear();
    _TextureIdStack.clear();
    _Path.clear();
    _ChannelsCurrent = 0;
    _ChannelsCount = 1;
    for (int i = 0; i < _Channels.Size; i++)
    {
        if (i == 0) memset(&_Channels[0], 0, sizeof(_Channels[0]));  // channel 0 is a copy of CmdBuffer/IdxBuffer, don't destruct again
        _Channels[i].CmdBuffer.clear();
        _Channels[i].IdxBuffer.clear();
    }
    _Channels.clear();
}

// Use macros because C++ is a terrible language, we want guaranteed inline, no code in header, and no overhead in Debug mode
#define GetCurrentClipRect()    (_ClipRectStack.Size ? _ClipRectStack.Data[_ClipRectStack.Size-1]  : GNullClipRect)
#define GetCurrentTextureId()   (_TextureIdStack.Size ? _TextureIdStack.Data[_TextureIdStack.Size-1] : NULL)

void ImDrawList::AddDrawCmd()
{
    ImDrawCmd draw_cmd;
    draw_cmd.ClipRect = GetCurrentClipRect();
    draw_cmd.TextureId = GetCurrentTextureId();

    IM_ASSERT(draw_cmd.ClipRect.x <= draw_cmd.ClipRect.z && draw_cmd.ClipRect.y <= draw_cmd.ClipRect.w);
    CmdBuffer.push_back(draw_cmd);
}

void ImDrawList::AddCallback(ImDrawCallback callback, void* callback_data)
{
    ImDrawCmd* current_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
    if (!current_cmd || current_cmd->ElemCount != 0 || current_cmd->UserCallback != NULL)
    {
        AddDrawCmd();
        current_cmd = &CmdBuffer.back();
    }
    current_cmd->UserCallback = callback;
    current_cmd->UserCallbackData = callback_data;

    AddDrawCmd(); // Force a new command after us (see comment below)
}

// Our scheme may appears a bit unusual, basically we want the most-common calls AddLine AddRect etc. to not have to perform any check so we always have a command ready in the stack.
// The cost of figuring out if a new command has to be added or if we can merge is paid in those Update** functions only.
void ImDrawList::UpdateClipRect()
{
    // If current command is used with different settings we need to add a new command
    const ImVec4 curr_clip_rect = GetCurrentClipRect();
    ImDrawCmd* curr_cmd = CmdBuffer.Size > 0 ? &CmdBuffer.Data[CmdBuffer.Size-1] : NULL;
    if (!curr_cmd || (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) != 0) || curr_cmd->UserCallback != NULL)
    {
        AddDrawCmd();
        return;
    }

    // Try to merge with previous command if it matches, else use current command
    ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
    if (curr_cmd->ElemCount == 0 && prev_cmd && memcmp(&prev_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) == 0 && prev_cmd->TextureId == GetCurrentTextureId() && prev_cmd->UserCallback == NULL)
        CmdBuffer.pop_back();
    else
        curr_cmd->ClipRect = curr_clip_rect;
}

void ImDrawList::UpdateTextureID()
{
    // If current command is used with different settings we need to add a new command
    const ImTextureID curr_texture_id = GetCurrentTextureId();
    ImDrawCmd* curr_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
    if (!curr_cmd || (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != curr_texture_id) || curr_cmd->UserCallback != NULL)
    {
        AddDrawCmd();
        return;
    }

    // Try to merge with previous command if it matches, else use current command
    ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
    if (curr_cmd->ElemCount == 0 && prev_cmd && prev_cmd->TextureId == curr_texture_id && memcmp(&prev_cmd->ClipRect, &GetCurrentClipRect(), sizeof(ImVec4)) == 0 && prev_cmd->UserCallback == NULL)
        CmdBuffer.pop_back();
    else
        curr_cmd->TextureId = curr_texture_id;
}

#undef GetCurrentClipRect
#undef GetCurrentTextureId

// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)
void ImDrawList::PushClipRect(ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect)
{
    ImVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
    if (intersect_with_current_clip_rect && _ClipRectStack.Size)
    {
        ImVec4 current = _ClipRectStack.Data[_ClipRectStack.Size-1];
        if (cr.x < current.x) cr.x = current.x;
        if (cr.y < current.y) cr.y = current.y;
        if (cr.z > current.z) cr.z = current.z;
        if (cr.w > current.w) cr.w = current.w;
    }
    cr.z = ImMax(cr.x, cr.z);
    cr.w = ImMax(cr.y, cr.w);

    _ClipRectStack.push_back(cr);
    UpdateClipRect();
}

void ImDrawList::PushClipRectFullScreen()
{
    PushClipRect(ImVec2(GNullClipRect.x, GNullClipRect.y), ImVec2(GNullClipRect.z, GNullClipRect.w));
    //PushClipRect(GetVisibleRect());   // FIXME-OPT: This would be more correct but we're not supposed to access ImGuiContext from here?
}

void ImDrawList::PopClipRect()
{
    IM_ASSERT(_ClipRectStack.Size > 0);
    _ClipRectStack.pop_back();
    UpdateClipRect();
}

void ImDrawList::PushTextureID(const ImTextureID& texture_id)
{
    _TextureIdStack.push_back(texture_id);
    UpdateTextureID();
}

void ImDrawList::PopTextureID()
{
    IM_ASSERT(_TextureIdStack.Size > 0);
    _TextureIdStack.pop_back();
    UpdateTextureID();
}

void ImDrawList::ChannelsSplit(int channels_count)
{
    IM_ASSERT(_ChannelsCurrent == 0 && _ChannelsCount == 1);
    int old_channels_count = _Channels.Size;
    if (old_channels_count < channels_count)
        _Channels.resize(channels_count);
    _ChannelsCount = channels_count;

    // _Channels[] (24 bytes each) hold storage that we'll swap with this->_CmdBuffer/_IdxBuffer
    // The content of _Channels[0] at this point doesn't matter. We clear it to make state tidy in a debugger but we don't strictly need to.
    // When we switch to the next channel, we'll copy _CmdBuffer/_IdxBuffer into _Channels[0] and then _Channels[1] into _CmdBuffer/_IdxBuffer
    memset(&_Channels[0], 0, sizeof(ImDrawChannel));
    for (int i = 1; i < channels_count; i++)
    {
        if (i >= old_channels_count)
        {
            IM_PLACEMENT_NEW(&_Channels[i]) ImDrawChannel();
        }
        else
        {
            _Channels[i].CmdBuffer.resize(0);
            _Channels[i].IdxBuffer.resize(0);
        }
        if (_Channels[i].CmdBuffer.Size == 0)
        {
            ImDrawCmd draw_cmd;
            draw_cmd.ClipRect = _ClipRectStack.back();
            draw_cmd.TextureId = _TextureIdStack.back();
            _Channels[i].CmdBuffer.push_back(draw_cmd);
        }
    }
}

void ImDrawList::ChannelsMerge()
{
    // Note that we never use or rely on channels.Size because it is merely a buffer that we never shrink back to 0 to keep all sub-buffers ready for use.
    if (_ChannelsCount <= 1)
        return;

    ChannelsSetCurrent(0);
    if (CmdBuffer.Size && CmdBuffer.back().ElemCount == 0)
        CmdBuffer.pop_back();

    int new_cmd_buffer_count = 0, new_idx_buffer_count = 0;
    for (int i = 1; i < _ChannelsCount; i++)
    {
        ImDrawChannel& ch = _Channels[i];
        if (ch.CmdBuffer.Size && ch.CmdBuffer.back().ElemCount == 0)
            ch.CmdBuffer.pop_back();
        new_cmd_buffer_count += ch.CmdBuffer.Size;
        new_idx_buffer_count += ch.IdxBuffer.Size;
    }
    CmdBuffer.resize(CmdBuffer.Size + new_cmd_buffer_count);
    IdxBuffer.resize(IdxBuffer.Size + new_idx_buffer_count);

    ImDrawCmd* cmd_write = CmdBuffer.Data + CmdBuffer.Size - new_cmd_buffer_count;
    _IdxWritePtr = IdxBuffer.Data + IdxBuffer.Size - new_idx_buffer_count;
    for (int i = 1; i < _ChannelsCount; i++)
    {
        ImDrawChannel& ch = _Channels[i];
        if (int sz = ch.CmdBuffer.Size) { memcpy(cmd_write, ch.CmdBuffer.Data, sz * sizeof(ImDrawCmd)); cmd_write += sz; }
        if (int sz = ch.IdxBuffer.Size) { memcpy(_IdxWritePtr, ch.IdxBuffer.Data, sz * sizeof(ImDrawIdx)); _IdxWritePtr += sz; }
    }
    UpdateClipRect(); // We call this instead of AddDrawCmd(), so that empty channels won't produce an extra draw call.
    _ChannelsCount = 1;
}

void ImDrawList::ChannelsSetCurrent(int idx)
{
    IM_ASSERT(idx < _ChannelsCount);
    if (_ChannelsCurrent == idx) return;
    memcpy(&_Channels.Data[_ChannelsCurrent].CmdBuffer, &CmdBuffer, sizeof(CmdBuffer)); // copy 12 bytes, four times
    memcpy(&_Channels.Data[_ChannelsCurrent].IdxBuffer, &IdxBuffer, sizeof(IdxBuffer));
    _ChannelsCurrent = idx;
    memcpy(&CmdBuffer, &_Channels.Data[_ChannelsCurrent].CmdBuffer, sizeof(CmdBuffer));
    memcpy(&IdxBuffer, &_Channels.Data[_ChannelsCurrent].IdxBuffer, sizeof(IdxBuffer));
    _IdxWritePtr = IdxBuffer.Data + IdxBuffer.Size;
}

// NB: this can be called with negative count for removing primitives (as long as the result does not underflow)
void ImDrawList::PrimReserve(int idx_count, int vtx_count)
{
    ImDrawCmd& draw_cmd = CmdBuffer.Data[CmdBuffer.Size-1];
    draw_cmd.ElemCount += idx_count;

    int vtx_buffer_old_size = VtxBuffer.Size;
    VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
    _VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

    int idx_buffer_old_size = IdxBuffer.Size;
    IdxBuffer.resize(idx_buffer_old_size + idx_count);
    _IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

// Fully unrolled with inline call to keep our debug builds decently fast.
void ImDrawList::PrimRect(const ImVec2& a, const ImVec2& c, ImU32 col)
{
    ImVec2 b(c.x, a.y), d(a.x, c.y), uv(GImGui->FontTexUvWhitePixel);
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void ImDrawList::PrimRectUV(const ImVec2& a, const ImVec2& c, const ImVec2& uv_a, const ImVec2& uv_c, ImU32 col)
{
    ImVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void ImDrawList::PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
void ImDrawList::AddPolyline(const ImVec2* points, const int points_count, ImU32 col, bool closed, float thickness, bool anti_aliased)
{
    if (points_count < 2)
        return;

    const ImVec2 uv = GImGui->FontTexUvWhitePixel;
    anti_aliased &= GImGui->Style.AntiAliasedLines;
    //if (ImGui::GetIO().KeyCtrl) anti_aliased = false; // Debug

    int count = points_count;
    if (!closed)
        count = points_count-1;

    const bool thick_line = thickness > 1.0f;
    if (anti_aliased)
    {
        // Anti-aliased stroke
        const float AA_SIZE = 1.0f;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;

        const int idx_count = thick_line ? count*18 : count*12;
        const int vtx_count = thick_line ? points_count*4 : points_count*3;
        PrimReserve(idx_count, vtx_count);

        // Temporary buffer
        ImVec2* temp_normals = (ImVec2*)alloca(points_count * (thick_line ? 5 : 3) * sizeof(ImVec2));
        ImVec2* temp_points = temp_normals + points_count;

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1+1) == points_count ? 0 : i1+1;
            ImVec2 diff = points[i2] - points[i1];
            diff *= ImInvLength(diff, 1.0f);
            temp_normals[i1].x = diff.y;
            temp_normals[i1].y = -diff.x;
        }
        if (!closed)
            temp_normals[points_count-1] = temp_normals[points_count-2];

        if (!thick_line)
        {
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
                temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
                temp_points[(points_count-1)*2+0] = points[points_count-1] + temp_normals[points_count-1] * AA_SIZE;
                temp_points[(points_count-1)*2+1] = points[points_count-1] - temp_normals[points_count-1] * AA_SIZE;
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1+1) == points_count ? 0 : i1+1;
                unsigned int idx2 = (i1+1) == points_count ? _VtxCurrentIdx : idx1+3;

                // Average normals
                ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
                float dmr2 = dm.x*dm.x + dm.y*dm.y;
                if (dmr2 > 0.000001f)
                {
                    float scale = 1.0f / dmr2;
                    if (scale > 100.0f) scale = 100.0f;
                    dm *= scale;
                }
                dm *= AA_SIZE;
                temp_points[i2*2+0] = points[i2] + dm;
                temp_points[i2*2+1] = points[i2] - dm;

                // Add indexes
                _IdxWritePtr[0] = (ImDrawIdx)(idx2+0); _IdxWritePtr[1] = (ImDrawIdx)(idx1+0); _IdxWritePtr[2] = (ImDrawIdx)(idx1+2);
                _IdxWritePtr[3] = (ImDrawIdx)(idx1+2); _IdxWritePtr[4] = (ImDrawIdx)(idx2+2); _IdxWritePtr[5] = (ImDrawIdx)(idx2+0);
                _IdxWritePtr[6] = (ImDrawIdx)(idx2+1); _IdxWritePtr[7] = (ImDrawIdx)(idx1+1); _IdxWritePtr[8] = (ImDrawIdx)(idx1+0);
                _IdxWritePtr[9] = (ImDrawIdx)(idx1+0); _IdxWritePtr[10]= (ImDrawIdx)(idx2+0); _IdxWritePtr[11]= (ImDrawIdx)(idx2+1);
                _IdxWritePtr += 12;

                idx1 = idx2;
            }

            // Add vertexes
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = points[i];          _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
                _VtxWritePtr[1].pos = temp_points[i*2+0]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;
                _VtxWritePtr[2].pos = temp_points[i*2+1]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col_trans;
                _VtxWritePtr += 3;
            }
        }
        else
        {
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[(points_count-1)*4+0] = points[points_count-1] + temp_normals[points_count-1] * (half_inner_thickness + AA_SIZE);
                temp_points[(points_count-1)*4+1] = points[points_count-1] + temp_normals[points_count-1] * (half_inner_thickness);
                temp_points[(points_count-1)*4+2] = points[points_count-1] - temp_normals[points_count-1] * (half_inner_thickness);
                temp_points[(points_count-1)*4+3] = points[points_count-1] - temp_normals[points_count-1] * (half_inner_thickness + AA_SIZE);
            }

            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx;
            for (int i1 = 0; i1 < count; i1++)
            {
                const int i2 = (i1+1) == points_count ? 0 : i1+1;
                unsigned int idx2 = (i1+1) == points_count ? _VtxCurrentIdx : idx1+4;

                // Average normals
                ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
                float dmr2 = dm.x*dm.x + dm.y*dm.y;
                if (dmr2 > 0.000001f)
                {
                    float scale = 1.0f / dmr2;
                    if (scale > 100.0f) scale = 100.0f;
                    dm *= scale;
                }
                ImVec2 dm_out = dm * (half_inner_thickness + AA_SIZE);
                ImVec2 dm_in = dm * half_inner_thickness;
                temp_points[i2*4+0] = points[i2] + dm_out;
                temp_points[i2*4+1] = points[i2] + dm_in;
                temp_points[i2*4+2] = points[i2] - dm_in;
                temp_points[i2*4+3] = points[i2] - dm_out;

                // Add indexes
                _IdxWritePtr[0]  = (ImDrawIdx)(idx2+1); _IdxWritePtr[1]  = (ImDrawIdx)(idx1+1); _IdxWritePtr[2]  = (ImDrawIdx)(idx1+2);
                _IdxWritePtr[3]  = (ImDrawIdx)(idx1+2); _IdxWritePtr[4]  = (ImDrawIdx)(idx2+2); _IdxWritePtr[5]  = (ImDrawIdx)(idx2+1);
                _IdxWritePtr[6]  = (ImDrawIdx)(idx2+1); _IdxWritePtr[7]  = (ImDrawIdx)(idx1+1); _IdxWritePtr[8]  = (ImDrawIdx)(idx1+0);
                _IdxWritePtr[9]  = (ImDrawIdx)(idx1+0); _IdxWritePtr[10] = (ImDrawIdx)(idx2+0); _IdxWritePtr[11] = (ImDrawIdx)(idx2+1);
                _IdxWritePtr[12] = (ImDrawIdx)(idx2+2); _IdxWritePtr[13] = (ImDrawIdx)(idx1+2); _IdxWritePtr[14] = (ImDrawIdx)(idx1+3);
                _IdxWritePtr[15] = (ImDrawIdx)(idx1+3); _IdxWritePtr[16] = (ImDrawIdx)(idx2+3); _IdxWritePtr[17] = (ImDrawIdx)(idx2+2);
                _IdxWritePtr += 18;

                idx1 = idx2;
            }

            // Add vertexes
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = temp_points[i*4+0]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col_trans;
                _VtxWritePtr[1].pos = temp_points[i*4+1]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
                _VtxWritePtr[2].pos = temp_points[i*4+2]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
                _VtxWritePtr[3].pos = temp_points[i*4+3]; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col_trans;
                _VtxWritePtr += 4;
            }
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else
    {
        // Non Anti-aliased Stroke
        const int idx_count = count*6;
        const int vtx_count = count*4;      // FIXME-OPT: Not sharing edges
        PrimReserve(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1+1) == points_count ? 0 : i1+1;
            const ImVec2& p1 = points[i1];
            const ImVec2& p2 = points[i2];
            ImVec2 diff = p2 - p1;
            diff *= ImInvLength(diff, 1.0f);

            const float dx = diff.x * (thickness * 0.5f);
            const float dy = diff.y * (thickness * 0.5f);
            _VtxWritePtr[0].pos.x = p1.x + dy; _VtxWritePtr[0].pos.y = p1.y - dx; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = p2.x + dy; _VtxWritePtr[1].pos.y = p2.y - dx; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
            _VtxWritePtr[2].pos.x = p2.x - dy; _VtxWritePtr[2].pos.y = p2.y + dx; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
            _VtxWritePtr[3].pos.x = p1.x - dy; _VtxWritePtr[3].pos.y = p1.y + dx; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
            _VtxWritePtr += 4;

            _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx+1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx+2);
            _IdxWritePtr[3] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[4] = (ImDrawIdx)(_VtxCurrentIdx+2); _IdxWritePtr[5] = (ImDrawIdx)(_VtxCurrentIdx+3);
            _IdxWritePtr += 6;
            _VtxCurrentIdx += 4;
        }
    }
}

void ImDrawList::AddConvexPolyFilled(const ImVec2* points, const int points_count, ImU32 col, bool anti_aliased)
{
    const ImVec2 uv = GImGui->FontTexUvWhitePixel;
    anti_aliased &= GImGui->Style.AntiAliasedShapes;
    //if (ImGui::GetIO().KeyCtrl) anti_aliased = false; // Debug

    if (anti_aliased)
    {
        // Anti-aliased Fill
        const float AA_SIZE = 1.0f;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;
        const int idx_count = (points_count-2)*3 + points_count*6;
        const int vtx_count = (points_count*2);
        PrimReserve(idx_count, vtx_count);

        // Add indexes for fill
        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx+1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx+((i-1)<<1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx+(i<<1));
            _IdxWritePtr += 3;
        }

        // Compute normals
        ImVec2* temp_normals = (ImVec2*)alloca(points_count * sizeof(ImVec2));
        for (int i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const ImVec2& p0 = points[i0];
            const ImVec2& p1 = points[i1];
            ImVec2 diff = p1 - p0;
            diff *= ImInvLength(diff, 1.0f);
            temp_normals[i0].x = diff.y;
            temp_normals[i0].y = -diff.x;
        }

        for (int i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            // Average normals
            const ImVec2& n0 = temp_normals[i0];
            const ImVec2& n1 = temp_normals[i1];
            ImVec2 dm = (n0 + n1) * 0.5f;
            float dmr2 = dm.x*dm.x + dm.y*dm.y;
            if (dmr2 > 0.000001f)
            {
                float scale = 1.0f / dmr2;
                if (scale > 100.0f) scale = 100.0f;
                dm *= scale;
            }
            dm *= AA_SIZE * 0.5f;

            // Add vertices
            _VtxWritePtr[0].pos = (points[i1] - dm); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
            _VtxWritePtr[1].pos = (points[i1] + dm); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
            _VtxWritePtr += 2;

            // Add indexes for fringes
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx+(i1<<1)); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx+(i0<<1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx+(i0<<1));
            _IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx+(i0<<1)); _IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx+(i1<<1)); _IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx+(i1<<1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else
    {
        // Non Anti-aliased Fill
        const int idx_count = (points_count-2)*3;
        const int vtx_count = points_count;
        PrimReserve(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr++;
        }
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx+i-1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx+i);
            _IdxWritePtr += 3;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
}

void ImDrawList::PathArcToFast(const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12)
{
    static ImVec2 circle_vtx[12];
    static bool circle_vtx_builds = false;
    const int circle_vtx_count = IM_ARRAYSIZE(circle_vtx);
    if (!circle_vtx_builds)
    {
        for (int i = 0; i < circle_vtx_count; i++)
        {
            const float a = ((float)i / (float)circle_vtx_count) * 2*IM_PI;
            circle_vtx[i].x = cosf(a);
            circle_vtx[i].y = sinf(a);
        }
        circle_vtx_builds = true;
    }

    if (radius == 0.0f || a_min_of_12 > a_max_of_12)
    {
        _Path.push_back(centre);
        return;
    }
    _Path.reserve(_Path.Size + (a_max_of_12 - a_min_of_12 + 1));
    for (int a = a_min_of_12; a <= a_max_of_12; a++)
    {
        const ImVec2& c = circle_vtx[a % circle_vtx_count];
        _Path.push_back(ImVec2(centre.x + c.x * radius, centre.y + c.y * radius));
    }
}

void ImDrawList::PathArcTo(const ImVec2& centre, float radius, float a_min, float a_max, int num_segments)
{
    if (radius == 0.0f)
    {
        _Path.push_back(centre);
        return;
    }
    _Path.reserve(_Path.Size + (num_segments + 1));
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        _Path.push_back(ImVec2(centre.x + cosf(a) * radius, centre.y + sinf(a) * radius));
    }
}

static void PathBezierToCasteljau(ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
{
    float dx = x4 - x1;
    float dy = y4 - y1;
    float d2 = ((x2 - x4) * dy - (y2 - y4) * dx);
    float d3 = ((x3 - x4) * dy - (y3 - y4) * dx);
    d2 = (d2 >= 0) ? d2 : -d2;
    d3 = (d3 >= 0) ? d3 : -d3;
    if ((d2+d3) * (d2+d3) < tess_tol * (dx*dx + dy*dy))
    {
        path->push_back(ImVec2(x4, y4));
    }
    else if (level < 10)
    {
        float x12 = (x1+x2)*0.5f,       y12 = (y1+y2)*0.5f;
        float x23 = (x2+x3)*0.5f,       y23 = (y2+y3)*0.5f;
        float x34 = (x3+x4)*0.5f,       y34 = (y3+y4)*0.5f;
        float x123 = (x12+x23)*0.5f,    y123 = (y12+y23)*0.5f;
        float x234 = (x23+x34)*0.5f,    y234 = (y23+y34)*0.5f;
        float x1234 = (x123+x234)*0.5f, y1234 = (y123+y234)*0.5f;

        PathBezierToCasteljau(path, x1,y1,        x12,y12,    x123,y123,  x1234,y1234, tess_tol, level+1);
        PathBezierToCasteljau(path, x1234,y1234,  x234,y234,  x34,y34,    x4,y4,       tess_tol, level+1);
    }
}

void ImDrawList::PathBezierCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments)
{
    ImVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        // Auto-tessellated
        PathBezierToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, GImGui->Style.CurveTessellationTol, 0);
    }
    else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
        {
            float t = t_step * i_step;
            float u = 1.0f - t;
            float w1 = u*u*u;
            float w2 = 3*u*u*t;
            float w3 = 3*u*t*t;
            float w4 = t*t*t;
            _Path.push_back(ImVec2(w1*p1.x + w2*p2.x + w3*p3.x + w4*p4.x, w1*p1.y + w2*p2.y + w3*p3.y + w4*p4.y));
        }
    }
}

void ImDrawList::PathRect(const ImVec2& a, const ImVec2& b, float rounding, int rounding_corners)
{
    const int corners_top = ImGuiCorner_TopLeft | ImGuiCorner_TopRight;
    const int corners_bottom = ImGuiCorner_BotLeft | ImGuiCorner_BotRight;
    const int corners_left = ImGuiCorner_TopLeft | ImGuiCorner_BotLeft;
    const int corners_right = ImGuiCorner_TopRight | ImGuiCorner_BotRight;

    rounding = ImMin(rounding, fabsf(b.x - a.x) * ( ((rounding_corners & corners_top)  == corners_top)  || ((rounding_corners & corners_bottom) == corners_bottom) ? 0.5f : 1.0f ) - 1.0f);
    rounding = ImMin(rounding, fabsf(b.y - a.y) * ( ((rounding_corners & corners_left) == corners_left) || ((rounding_corners & corners_right)  == corners_right)  ? 0.5f : 1.0f ) - 1.0f);

    if (rounding <= 0.0f || rounding_corners == 0)
    {
        PathLineTo(a);
        PathLineTo(ImVec2(b.x, a.y));
        PathLineTo(b);
        PathLineTo(ImVec2(a.x, b.y));
    }
    else
    {
        const float rounding_tl = (rounding_corners & ImGuiCorner_TopLeft) ? rounding : 0.0f;
        const float rounding_tr = (rounding_corners & ImGuiCorner_TopRight) ? rounding : 0.0f;
        const float rounding_br = (rounding_corners & ImGuiCorner_BotRight) ? rounding : 0.0f;
        const float rounding_bl = (rounding_corners & ImGuiCorner_BotLeft) ? rounding : 0.0f;
        PathArcToFast(ImVec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
        PathArcToFast(ImVec2(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
        PathArcToFast(ImVec2(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
        PathArcToFast(ImVec2(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
    }
}

void ImDrawList::AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    PathLineTo(a + ImVec2(0.5f,0.5f));
    PathLineTo(b + ImVec2(0.5f,0.5f));
    PathStroke(col, false, thickness);
}

// a: upper-left, b: lower-right. we don't render 1 px sized rectangles properly.
void ImDrawList::AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    PathRect(a + ImVec2(0.5f,0.5f), b - ImVec2(0.5f,0.5f), rounding, rounding_corners_flags);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    if (rounding > 0.0f)
    {
        PathRect(a, b, rounding, rounding_corners_flags);
        PathFillConvex(col);
    }
    else
    {
        PrimReserve(6, 4);
        PrimRect(a, b, col);
    }
}

void ImDrawList::AddRectFilledMultiColor(const ImVec2& a, const ImVec2& c, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
    if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) & IM_COL32_A_MASK) == 0)
        return;

    const ImVec2 uv = GImGui->FontTexUvWhitePixel;
    PrimReserve(6, 4);
    PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+1)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+2));
    PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+2)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx+3));
    PrimWriteVtx(a, uv, col_upr_left);
    PrimWriteVtx(ImVec2(c.x, a.y), uv, col_upr_right);
    PrimWriteVtx(c, uv, col_bot_right);
    PrimWriteVtx(ImVec2(a.x, c.y), uv, col_bot_left);
}

void ImDrawList::AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathLineTo(d);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathLineTo(d);
    PathFillConvex(col);
}

void ImDrawList::AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(a);
    PathLineTo(b);
    PathLineTo(c);
    PathFillConvex(col);
}

void ImDrawList::AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const float a_max = IM_PI*2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(centre, radius-0.5f, 0.0f, a_max, num_segments);
    PathStroke(col, true, thickness);
}

void ImDrawList::AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const float a_max = IM_PI*2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(centre, radius, 0.0f, a_max, num_segments);
    PathFillConvex(col);
}

void ImDrawList::AddBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(pos0);
    PathBezierCurveTo(cp0, cp1, pos1, num_segments);
    PathStroke(col, false, thickness);
}

void ImDrawList::AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    if (text_end == NULL)
        text_end = text_begin + strlen(text_begin);
    if (text_begin == text_end)
        return;

    // IMPORTANT: This is one of the few instance of breaking the encapsulation of ImDrawList, as we pull this from ImGui state, but it is just SO useful.
    // Might just move Font/FontSize to ImDrawList?
    if (font == NULL)
        font = GImGui->Font;
    if (font_size == 0.0f)
        font_size = GImGui->FontSize;

    IM_ASSERT(font->ContainerAtlas->TexID == _TextureIdStack.back());  // Use high-level ImGui::PushFont() or low-level ImDrawList::PushTextureId() to change font.

    ImVec4 clip_rect = _ClipRectStack.back();
    if (cpu_fine_clip_rect)
    {
        clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}

void ImDrawList::AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
    AddText(NULL, 0.0f, pos, col, text_begin, text_end);
}

void ImDrawList::AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    // FIXME-OPT: This is wasting draw calls.
    const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimRectUV(a, b, uv_a, uv_b, col);

    if (push_texture_id)
        PopTextureID();
}

void ImDrawList::AddImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);

    if (push_texture_id)
        PopTextureID();
}

//-----------------------------------------------------------------------------
// ImDrawData
//-----------------------------------------------------------------------------

// For backward compatibility: convert all buffers from indexed to de-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
void ImDrawData::DeIndexAllBuffers()
{
    ImVector<ImDrawVert> new_vtx_buffer;
    TotalVtxCount = TotalIdxCount = 0;
    for (int i = 0; i < CmdListsCount; i++)
    {
        ImDrawList* cmd_list = CmdLists[i];
        if (cmd_list->IdxBuffer.empty())
            continue;
        new_vtx_buffer.resize(cmd_list->IdxBuffer.Size);
        for (int j = 0; j < cmd_list->IdxBuffer.Size; j++)
            new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
        cmd_list->VtxBuffer.swap(new_vtx_buffer);
        cmd_list->IdxBuffer.resize(0);
        TotalVtxCount += cmd_list->VtxBuffer.Size;
    }
}

// Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.
void ImDrawData::ScaleClipRects(const ImVec2& scale)
{
    for (int i = 0; i < CmdListsCount; i++)
    {
        ImDrawList* cmd_list = CmdLists[i];
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            ImDrawCmd* cmd = &cmd_list->CmdBuffer[cmd_i];
            cmd->ClipRect = ImVec4(cmd->ClipRect.x * scale.x, cmd->ClipRect.y * scale.y, cmd->ClipRect.z * scale.x, cmd->ClipRect.w * scale.y);
        }
    }
}

//-----------------------------------------------------------------------------
// Shade functions
//-----------------------------------------------------------------------------

// Generic linear color gradient, write to RGB fields, leave A untouched.
void ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImDrawVert* vert_start, ImDrawVert* vert_end, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
    ImVec2 gradient_extent = gradient_p1 - gradient_p0;
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = ImDot(vert->pos - gradient_p0, gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = ImLerp((int)(col0 >> IM_COL32_R_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_R_SHIFT) & 0xFF, t);
        int g = ImLerp((int)(col0 >> IM_COL32_G_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_G_SHIFT) & 0xFF, t);
        int b = ImLerp((int)(col0 >> IM_COL32_B_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_B_SHIFT) & 0xFF, t);
        vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (vert->col & IM_COL32_A_MASK);
    }
}

// Scan and shade backward from the end of given vertices. Assume vertices are text only (= vert_start..vert_end going left to right) so we can break as soon as we are out the gradient bounds.
void ImGui::ShadeVertsLinearAlphaGradientForLeftToRightText(ImDrawVert* vert_start, ImDrawVert* vert_end, float gradient_p0_x, float gradient_p1_x)
{
    float gradient_extent_x = gradient_p1_x - gradient_p0_x;
    float gradient_inv_length2 = 1.0f / (gradient_extent_x * gradient_extent_x);
    int full_alpha_count = 0;
    for (ImDrawVert* vert = vert_end - 1; vert >= vert_start; vert--)
    {
        float d = (vert->pos.x - gradient_p0_x) * (gradient_extent_x);
        float alpha_mul = 1.0f - ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        if (alpha_mul >= 1.0f && ++full_alpha_count > 2)
            return; // Early out
        int a = (int)(((vert->col >> IM_COL32_A_SHIFT) & 0xFF) * alpha_mul);
        vert->col = (vert->col & ~IM_COL32_A_MASK) | (a << IM_COL32_A_SHIFT);
    }
}

//-----------------------------------------------------------------------------
// ImFontConfig
//-----------------------------------------------------------------------------

ImFontConfig::ImFontConfig()
{
    FontData = NULL;
    FontDataSize = 0;
    FontDataOwnedByAtlas = true;
    FontNo = 0;
    SizePixels = 0.0f;
    OversampleH = 3;
    OversampleV = 1;
    PixelSnapH = false;
    GlyphExtraSpacing = ImVec2(0.0f, 0.0f);
    GlyphOffset = ImVec2(0.0f, 0.0f);
    GlyphRanges = NULL;
    MergeMode = false;
    RasterizerFlags = 0x00;
    RasterizerMultiply = 1.0f;
    memset(Name, 0, sizeof(Name));
    DstFont = NULL;
}

//-----------------------------------------------------------------------------
// ImFontAtlas
//-----------------------------------------------------------------------------

// A work of art lies ahead! (. = white layer, X = black layer, others are blank)
// The white texels on the top left are the ones we'll use everywhere in ImGui to render filled shapes.
const int FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF = 90;
const int FONT_ATLAS_DEFAULT_TEX_DATA_H      = 27;
const unsigned int FONT_ATLAS_DEFAULT_TEX_DATA_ID = 0x80000000;
const char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] =
{
    "..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX"
    "..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X"
    "---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X"
    "X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X"
    "XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X"
    "X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X"
    "X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX"
    "X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      "
    "X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       "
    "X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        "
    "X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         "
    "X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          "
    "X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           "
    "X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            "
    "X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           "
    "X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          "
    "X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          "
    "X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       ------------------------------------"
    "X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           "
    "XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           "
    "      X..X          -  X...X  -         X...X         -  X..X           X..X  -           "
    "       XX           -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           "
    "------------        -    X    -           X           -X.....................X-           "
    "                    ----------------------------------- X...XXXXXXXXXXXXX...X -           "
    "                                                      -  X..X           X..X  -           "
    "                                                      -   X.X           X.X   -           "
    "                                                      -    XX           XX    -           "
};

ImFontAtlas::ImFontAtlas()
{
    TexID = NULL;
    TexDesiredWidth = 0;
    TexGlyphPadding = 1;
    TexPixelsAlpha8 = NULL;
    TexPixelsRGBA32 = NULL;
    TexWidth = TexHeight = 0;
    TexUvWhitePixel = ImVec2(0, 0);
    for (int n = 0; n < IM_ARRAYSIZE(CustomRectIds); n++)
        CustomRectIds[n] = -1;
}

ImFontAtlas::~ImFontAtlas()
{
    Clear();
}

void    ImFontAtlas::ClearInputData()
{
    for (int i = 0; i < ConfigData.Size; i++)
        if (ConfigData[i].FontData && ConfigData[i].FontDataOwnedByAtlas)
        {
            ImGui::MemFree(ConfigData[i].FontData);
            ConfigData[i].FontData = NULL;
        }

    // When clearing this we lose access to  the font name and other information used to build the font.
    for (int i = 0; i < Fonts.Size; i++)
        if (Fonts[i]->ConfigData >= ConfigData.Data && Fonts[i]->ConfigData < ConfigData.Data + ConfigData.Size)
        {
            Fonts[i]->ConfigData = NULL;
            Fonts[i]->ConfigDataCount = 0;
        }
    ConfigData.clear();
    CustomRects.clear();
    for (int n = 0; n < IM_ARRAYSIZE(CustomRectIds); n++)
        CustomRectIds[n] = -1;
}

void    ImFontAtlas::ClearTexData()
{
    if (TexPixelsAlpha8)
        ImGui::MemFree(TexPixelsAlpha8);
    if (TexPixelsRGBA32)
        ImGui::MemFree(TexPixelsRGBA32);
    TexPixelsAlpha8 = NULL;
    TexPixelsRGBA32 = NULL;
}

void    ImFontAtlas::ClearFonts()
{
    for (int i = 0; i < Fonts.Size; i++)
    {
        Fonts[i]->~ImFont();
        ImGui::MemFree(Fonts[i]);
    }
    Fonts.clear();
}

void    ImFontAtlas::Clear()
{
    ClearInputData();
    ClearTexData();
    ClearFonts();
}

void    ImFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
    // Build atlas on demand
    if (TexPixelsAlpha8 == NULL)
    {
        if (ConfigData.empty())
            AddFontDefault();
        Build();
    }

    *out_pixels = TexPixelsAlpha8;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
}

void    ImFontAtlas::GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
    // Convert to RGBA32 format on demand
    // Although it is likely to be the most commonly used format, our font rendering is 1 channel / 8 bpp
    if (!TexPixelsRGBA32)
    {
        unsigned char* pixels;
        GetTexDataAsAlpha8(&pixels, NULL, NULL);
        TexPixelsRGBA32 = (unsigned int*)ImGui::MemAlloc((size_t)(TexWidth * TexHeight * 4));
        const unsigned char* src = pixels;
        unsigned int* dst = TexPixelsRGBA32;
        for (int n = TexWidth * TexHeight; n > 0; n--)
            *dst++ = IM_COL32(255, 255, 255, (unsigned int)(*src++));
    }

    *out_pixels = (unsigned char*)TexPixelsRGBA32;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
}

ImFont* ImFontAtlas::AddFont(const ImFontConfig* font_cfg)
{
    IM_ASSERT(font_cfg->FontData != NULL && font_cfg->FontDataSize > 0);
    IM_ASSERT(font_cfg->SizePixels > 0.0f);

    // Create new font
    if (!font_cfg->MergeMode)
    {
        ImFont* font = (ImFont*)ImGui::MemAlloc(sizeof(ImFont));
        IM_PLACEMENT_NEW(font) ImFont();
        Fonts.push_back(font);
    }
    else
    {
        IM_ASSERT(!Fonts.empty()); // When using MergeMode make sure that a font has already been added before. You can use ImGui::GetIO().Fonts->AddFontDefault() to add the default imgui font.
    }

    ConfigData.push_back(*font_cfg);
    ImFontConfig& new_font_cfg = ConfigData.back();
    if (!new_font_cfg.DstFont)
        new_font_cfg.DstFont = Fonts.back();
    if (!new_font_cfg.FontDataOwnedByAtlas)
    {
        new_font_cfg.FontData = ImGui::MemAlloc(new_font_cfg.FontDataSize);
        new_font_cfg.FontDataOwnedByAtlas = true;
        memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
    }

    // Invalidate texture
    ClearTexData();
    return new_font_cfg.DstFont;
}

// Default font TTF is compressed with stb_compress then base85 encoded (see extra_fonts/binary_to_compressed_c.cpp for encoder)
static unsigned int stb_decompress_length(unsigned char *input);
static unsigned int stb_decompress(unsigned char *output, unsigned char *i, unsigned int length);
static const char*  GetDefaultCompressedFontDataTTFBase85();
static unsigned int Decode85Byte(char c)                                    { return c >= '\\' ? c-36 : c-35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
    while (*src)
    {
        unsigned int tmp = Decode85Byte(src[0]) + 85*(Decode85Byte(src[1]) + 85*(Decode85Byte(src[2]) + 85*(Decode85Byte(src[3]) + 85*Decode85Byte(src[4]))));
        dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
        src += 5;
        dst += 4;
    }
}

// Load embedded ProggyClean.ttf at size 13, disable oversampling
ImFont* ImFontAtlas::AddFontDefault(const ImFontConfig* font_cfg_template)
{
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (!font_cfg_template)
    {
        font_cfg.OversampleH = font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
    }
    if (font_cfg.Name[0] == '\0') strcpy(font_cfg.Name, "ProggyClean.ttf, 13px");
    if (font_cfg.SizePixels <= 0.0f) font_cfg.SizePixels = 13.0f;

    const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
    ImFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, font_cfg.SizePixels, &font_cfg, GetGlyphRangesDefault());
    return font;
}

ImFont* ImFontAtlas::AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    int data_size = 0;
    void* data = ImFileLoadToMemory(filename, "rb", &data_size, 0);
    if (!data)
    {
        IM_ASSERT(0); // Could not load file.
        return NULL;
    }
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (font_cfg.Name[0] == '\0')
    {
        // Store a short copy of filename into into the font name for convenience
        const char* p;
        for (p = filename + strlen(filename); p > filename && p[-1] != '/' && p[-1] != '\\'; p--) {}
        snprintf(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %.0fpx", p, size_pixels);
    }
    return AddFontFromMemoryTTF(data, data_size, size_pixels, &font_cfg, glyph_ranges);
}

// NB: Transfer ownership of 'ttf_data' to ImFontAtlas, unless font_cfg_template->FontDataOwnedByAtlas == false. Owned TTF buffer will be deleted after Build().
ImFont* ImFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontData = ttf_data;
    font_cfg.FontDataSize = ttf_size;
    font_cfg.SizePixels = size_pixels;
    if (glyph_ranges)
        font_cfg.GlyphRanges = glyph_ranges;
    return AddFont(&font_cfg);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    const unsigned int buf_decompressed_size = stb_decompress_length((unsigned char*)compressed_ttf_data);
    unsigned char* buf_decompressed_data = (unsigned char *)ImGui::MemAlloc(buf_decompressed_size);
    stb_decompress(buf_decompressed_data, (unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);

    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontDataOwnedByAtlas = true;
    return AddFontFromMemoryTTF(buf_decompressed_data, (int)buf_decompressed_size, size_pixels, &font_cfg, glyph_ranges);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedBase85TTF(const char* compressed_ttf_data_base85, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges)
{
    int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
    void* compressed_ttf = ImGui::MemAlloc((size_t)compressed_ttf_size);
    Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
    ImFont* font = AddFontFromMemoryCompressedTTF(compressed_ttf, compressed_ttf_size, size_pixels, font_cfg, glyph_ranges);
    ImGui::MemFree(compressed_ttf);
    return font;
}

int ImFontAtlas::AddCustomRectRegular(unsigned int id, int width, int height)
{
    IM_ASSERT(id >= 0x10000);
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    CustomRect r;
    r.ID = id;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    CustomRects.push_back(r);
    return CustomRects.Size - 1; // Return index
}

int ImFontAtlas::AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset)
{
    IM_ASSERT(font != NULL);
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    CustomRect r;
    r.ID = id;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    r.GlyphAdvanceX = advance_x;
    r.GlyphOffset = offset;
    r.Font = font;
    CustomRects.push_back(r);
    return CustomRects.Size - 1; // Return index
}

void ImFontAtlas::CalcCustomRectUV(const CustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max)
{
    IM_ASSERT(TexWidth > 0 && TexHeight > 0);   // Font atlas needs to be built before we can calculate UV coordinates
    IM_ASSERT(rect->IsPacked());                // Make sure the rectangle has been packed
    *out_uv_min = ImVec2((float)rect->X / TexWidth, (float)rect->Y / TexHeight);
    *out_uv_max = ImVec2((float)(rect->X + rect->Width) / TexWidth, (float)(rect->Y + rect->Height) / TexHeight);
}

bool    ImFontAtlas::Build()
{
    return ImFontAtlasBuildWithStbTruetype(this);
}

void    ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_brighten_factor)
{
    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned int value = (unsigned int)(i * in_brighten_factor);
        out_table[i] = value > 255 ? 255 : (value & 0xFF);
    }
}

void    ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride)
{
    unsigned char* data = pixels + x + y * stride;
    for (int j = h; j > 0; j--, data += stride)
        for (int i = 0; i < w; i++)
            data[i] = table[data[i]];
}

bool    ImFontAtlasBuildWithStbTruetype(ImFontAtlas* atlas)
{
    IM_ASSERT(atlas->ConfigData.Size > 0);

    ImFontAtlasBuildRegisterDefaultCustomRects(atlas);

    atlas->TexID = NULL;
    atlas->TexWidth = atlas->TexHeight = 0;
    atlas->TexUvWhitePixel = ImVec2(0, 0);
    atlas->ClearTexData();

    // Count glyphs/ranges
    int total_glyphs_count = 0;
    int total_ranges_count = 0;
    for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[input_i];
        if (!cfg.GlyphRanges)
            cfg.GlyphRanges = atlas->GetGlyphRangesDefault();
        for (const ImWchar* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2, total_ranges_count++)
            total_glyphs_count += (in_range[1] - in_range[0]) + 1;
    }

    // We need a width for the skyline algorithm. Using a dumb heuristic here to decide of width. User can override TexDesiredWidth and TexGlyphPadding if they wish.
    // Width doesn't really matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
    atlas->TexWidth = (atlas->TexDesiredWidth > 0) ? atlas->TexDesiredWidth : (total_glyphs_count > 4000) ? 4096 : (total_glyphs_count > 2000) ? 2048 : (total_glyphs_count > 1000) ? 1024 : 512;
    atlas->TexHeight = 0;

    // Start packing
    const int max_tex_height = 1024*32;
    stbtt_pack_context spc;
    stbtt_PackBegin(&spc, NULL, atlas->TexWidth, max_tex_height, 0, atlas->TexGlyphPadding, NULL);
    stbtt_PackSetOversampling(&spc, 1, 1);

    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    ImFontAtlasBuildPackCustomRects(atlas, spc.pack_info);

    // Initialize font information (so we can error without any cleanup)
    struct ImFontTempBuildData
    {
        stbtt_fontinfo      FontInfo;
        stbrp_rect*         Rects;
        int                 RectsCount;
        stbtt_pack_range*   Ranges;
        int                 RangesCount;
    };
    ImFontTempBuildData* tmp_array = (ImFontTempBuildData*)ImGui::MemAlloc((size_t)atlas->ConfigData.Size * sizeof(ImFontTempBuildData));
    for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[input_i];
        ImFontTempBuildData& tmp = tmp_array[input_i];
        IM_ASSERT(cfg.DstFont && (!cfg.DstFont->IsLoaded() || cfg.DstFont->ContainerAtlas == atlas));

        const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData, cfg.FontNo);
        IM_ASSERT(font_offset >= 0);
        if (!stbtt_InitFont(&tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
            return false;
    }

    // Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
    int buf_packedchars_n = 0, buf_rects_n = 0, buf_ranges_n = 0;
    stbtt_packedchar* buf_packedchars = (stbtt_packedchar*)ImGui::MemAlloc(total_glyphs_count * sizeof(stbtt_packedchar));
    stbrp_rect* buf_rects = (stbrp_rect*)ImGui::MemAlloc(total_glyphs_count * sizeof(stbrp_rect));
    stbtt_pack_range* buf_ranges = (stbtt_pack_range*)ImGui::MemAlloc(total_ranges_count * sizeof(stbtt_pack_range));
    memset(buf_packedchars, 0, total_glyphs_count * sizeof(stbtt_packedchar));
    memset(buf_rects, 0, total_glyphs_count * sizeof(stbrp_rect));              // Unnecessary but let's clear this for the sake of sanity.
    memset(buf_ranges, 0, total_ranges_count * sizeof(stbtt_pack_range));

    // First font pass: pack all glyphs (no rendering at this point, we are working with rectangles in an infinitely tall texture at this point)
    for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[input_i];
        ImFontTempBuildData& tmp = tmp_array[input_i];

        // Setup ranges
        int font_glyphs_count = 0;
        int font_ranges_count = 0;
        for (const ImWchar* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2, font_ranges_count++)
            font_glyphs_count += (in_range[1] - in_range[0]) + 1;
        tmp.Ranges = buf_ranges + buf_ranges_n;
        tmp.RangesCount = font_ranges_count;
        buf_ranges_n += font_ranges_count;
        for (int i = 0; i < font_ranges_count; i++)
        {
            const ImWchar* in_range = &cfg.GlyphRanges[i * 2];
            stbtt_pack_range& range = tmp.Ranges[i];
            range.font_size = cfg.SizePixels;
            range.first_unicode_codepoint_in_range = in_range[0];
            range.num_chars = (in_range[1] - in_range[0]) + 1;
            range.chardata_for_range = buf_packedchars + buf_packedchars_n;
            buf_packedchars_n += range.num_chars;
        }

        // Pack
        tmp.Rects = buf_rects + buf_rects_n;
        tmp.RectsCount = font_glyphs_count;
        buf_rects_n += font_glyphs_count;
        stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
        int n = stbtt_PackFontRangesGatherRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
        IM_ASSERT(n == font_glyphs_count);
        stbrp_pack_rects((stbrp_context*)spc.pack_info, tmp.Rects, n);

        // Extend texture height
        for (int i = 0; i < n; i++)
            if (tmp.Rects[i].was_packed)
                atlas->TexHeight = ImMax(atlas->TexHeight, tmp.Rects[i].y + tmp.Rects[i].h);
    }
    IM_ASSERT(buf_rects_n == total_glyphs_count);
    IM_ASSERT(buf_packedchars_n == total_glyphs_count);
    IM_ASSERT(buf_ranges_n == total_ranges_count);

    // Create texture
    atlas->TexHeight = ImUpperPowerOfTwo(atlas->TexHeight);
    atlas->TexPixelsAlpha8 = (unsigned char*)ImGui::MemAlloc(atlas->TexWidth * atlas->TexHeight);
    memset(atlas->TexPixelsAlpha8, 0, atlas->TexWidth * atlas->TexHeight);
    spc.pixels = atlas->TexPixelsAlpha8;
    spc.height = atlas->TexHeight;

    // Second pass: render font characters
    for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[input_i];
        ImFontTempBuildData& tmp = tmp_array[input_i];
        stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
        stbtt_PackFontRangesRenderIntoRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
        if (cfg.RasterizerMultiply != 1.0f)
        {
            unsigned char multiply_table[256];
            ImFontAtlasBuildMultiplyCalcLookupTable(multiply_table, cfg.RasterizerMultiply);
            for (const stbrp_rect* r = tmp.Rects; r != tmp.Rects + tmp.RectsCount; r++)
                if (r->was_packed)
                    ImFontAtlasBuildMultiplyRectAlpha8(multiply_table, spc.pixels, r->x, r->y, r->w, r->h, spc.stride_in_bytes);
        }
        tmp.Rects = NULL;
    }

    // End packing
    stbtt_PackEnd(&spc);
    ImGui::MemFree(buf_rects);
    buf_rects = NULL;

    // Third pass: setup ImFont and glyphs for runtime
    for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[input_i];
        ImFontTempBuildData& tmp = tmp_array[input_i];
        ImFont* dst_font = cfg.DstFont; // We can have multiple input fonts writing into a same destination font (when using MergeMode=true)

        const float font_scale = stbtt_ScaleForPixelHeight(&tmp.FontInfo, cfg.SizePixels);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

        const float ascent = unscaled_ascent * font_scale;
        const float descent = unscaled_descent * font_scale;
        ImFontAtlasBuildSetupFont(atlas, dst_font, &cfg, ascent, descent);
        const float off_x = cfg.GlyphOffset.x;
        const float off_y = cfg.GlyphOffset.y + (float)(int)(dst_font->Ascent + 0.5f);

        dst_font->FallbackGlyph = NULL; // Always clear fallback so FindGlyph can return NULL. It will be set again in BuildLookupTable()
        for (int i = 0; i < tmp.RangesCount; i++)
        {
            stbtt_pack_range& range = tmp.Ranges[i];
            for (int char_idx = 0; char_idx < range.num_chars; char_idx += 1)
            {
                const stbtt_packedchar& pc = range.chardata_for_range[char_idx];
                if (!pc.x0 && !pc.x1 && !pc.y0 && !pc.y1)
                    continue;

                const int codepoint = range.first_unicode_codepoint_in_range + char_idx;
                if (cfg.MergeMode && dst_font->FindGlyph((unsigned short)codepoint))
                    continue;

                stbtt_aligned_quad q;
                float dummy_x = 0.0f, dummy_y = 0.0f;
                stbtt_GetPackedQuad(range.chardata_for_range, atlas->TexWidth, atlas->TexHeight, char_idx, &dummy_x, &dummy_y, &q, 0);
                dst_font->AddGlyph((ImWchar)codepoint, q.x0 + off_x, q.y0 + off_y, q.x1 + off_x, q.y1 + off_y, q.s0, q.t0, q.s1, q.t1, pc.xadvance);
            }
        }
    }

    // Cleanup temporaries
    ImGui::MemFree(buf_packedchars);
    ImGui::MemFree(buf_ranges);
    ImGui::MemFree(tmp_array);

    ImFontAtlasBuildFinish(atlas);

    return true;
}

void ImFontAtlasBuildRegisterDefaultCustomRects(ImFontAtlas* atlas)
{
    if (atlas->CustomRectIds[0] < 0)
        atlas->CustomRectIds[0] = atlas->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_ID, FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF*2+1, FONT_ATLAS_DEFAULT_TEX_DATA_H);
}

void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent)
{
    if (!font_config->MergeMode)
    {
        font->ContainerAtlas = atlas;
        font->ConfigData = font_config;
        font->ConfigDataCount = 0;
        font->FontSize = font_config->SizePixels;
        font->Ascent = ascent;
        font->Descent = descent;
        font->Glyphs.resize(0);
        font->MetricsTotalSurface = 0;
    }
    font->ConfigDataCount++;
}

void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* pack_context_opaque)
{
    stbrp_context* pack_context = (stbrp_context*)pack_context_opaque;

    ImVector<ImFontAtlas::CustomRect>& user_rects = atlas->CustomRects;
    IM_ASSERT(user_rects.Size >= 1); // We expect at least the default custom rects to be registered, else something went wrong.

    ImVector<stbrp_rect> pack_rects;
    pack_rects.resize(user_rects.Size);
    memset(pack_rects.Data, 0, sizeof(stbrp_rect) * user_rects.Size);
    for (int i = 0; i < user_rects.Size; i++)
    {
        pack_rects[i].w = user_rects[i].Width;
        pack_rects[i].h = user_rects[i].Height;
    }
    stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.Size);
    for (int i = 0; i < pack_rects.Size; i++)
        if (pack_rects[i].was_packed)
        {
            user_rects[i].X = pack_rects[i].x;
            user_rects[i].Y = pack_rects[i].y;
            IM_ASSERT(pack_rects[i].w == user_rects[i].Width && pack_rects[i].h == user_rects[i].Height);
            atlas->TexHeight = ImMax(atlas->TexHeight, pack_rects[i].y + pack_rects[i].h);
        }
}

static void ImFontAtlasBuildRenderDefaultTexData(ImFontAtlas* atlas)
{
    IM_ASSERT(atlas->CustomRectIds[0] >= 0);
    ImFontAtlas::CustomRect& r = atlas->CustomRects[atlas->CustomRectIds[0]];
    IM_ASSERT(r.ID == FONT_ATLAS_DEFAULT_TEX_DATA_ID);
    IM_ASSERT(r.Width == FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF*2+1);
    IM_ASSERT(r.Height == FONT_ATLAS_DEFAULT_TEX_DATA_H);
    IM_ASSERT(r.IsPacked());
    IM_ASSERT(atlas->TexPixelsAlpha8 != NULL);

    // Render/copy pixels
    for (int y = 0, n = 0; y < FONT_ATLAS_DEFAULT_TEX_DATA_H; y++)
        for (int x = 0; x < FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF; x++, n++)
        {
            const int offset0 = (int)(r.X + x) + (int)(r.Y + y) * atlas->TexWidth;
            const int offset1 = offset0 + FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF + 1;
            atlas->TexPixelsAlpha8[offset0] = FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[n] == '.' ? 0xFF : 0x00;
            atlas->TexPixelsAlpha8[offset1] = FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[n] == 'X' ? 0xFF : 0x00;
        }
    const ImVec2 tex_uv_scale(1.0f / atlas->TexWidth, 1.0f / atlas->TexHeight);
    atlas->TexUvWhitePixel = ImVec2((r.X + 0.5f) * tex_uv_scale.x, (r.Y + 0.5f) * tex_uv_scale.y);

    // Setup mouse cursors
    const ImVec2 cursor_datas[ImGuiMouseCursor_Count_][3] =
    {
        // Pos ........ Size ......... Offset ......
        { ImVec2(0,3),  ImVec2(12,19), ImVec2( 0, 0) }, // ImGuiMouseCursor_Arrow
        { ImVec2(13,0), ImVec2(7,16),  ImVec2( 4, 8) }, // ImGuiMouseCursor_TextInput
        { ImVec2(31,0), ImVec2(23,23), ImVec2(11,11) }, // ImGuiMouseCursor_Move
        { ImVec2(21,0), ImVec2( 9,23), ImVec2( 5,11) }, // ImGuiMouseCursor_ResizeNS
        { ImVec2(55,18),ImVec2(23, 9), ImVec2(11, 5) }, // ImGuiMouseCursor_ResizeEW
        { ImVec2(73,0), ImVec2(17,17), ImVec2( 9, 9) }, // ImGuiMouseCursor_ResizeNESW
        { ImVec2(55,0), ImVec2(17,17), ImVec2( 9, 9) }, // ImGuiMouseCursor_ResizeNWSE
    };

    for (int type = 0; type < ImGuiMouseCursor_Count_; type++)
    {
        ImGuiMouseCursorData& cursor_data = GImGui->MouseCursorData[type];
        ImVec2 pos = cursor_datas[type][0] + ImVec2((float)r.X, (float)r.Y);
        const ImVec2 size = cursor_datas[type][1];
        cursor_data.Type = type;
        cursor_data.Size = size;
        cursor_data.HotOffset = cursor_datas[type][2];
        cursor_data.TexUvMin[0] = (pos) * tex_uv_scale;
        cursor_data.TexUvMax[0] = (pos + size) * tex_uv_scale;
        pos.x += FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF + 1;
        cursor_data.TexUvMin[1] = (pos) * tex_uv_scale;
        cursor_data.TexUvMax[1] = (pos + size) * tex_uv_scale;
    }
}

void ImFontAtlasBuildFinish(ImFontAtlas* atlas)
{
    // Render into our custom data block
    ImFontAtlasBuildRenderDefaultTexData(atlas);

    // Register custom rectangle glyphs
    for (int i = 0; i < atlas->CustomRects.Size; i++)
    {
        const ImFontAtlas::CustomRect& r = atlas->CustomRects[i];
        if (r.Font == NULL || r.ID > 0x10000)
            continue;

        IM_ASSERT(r.Font->ContainerAtlas == atlas);
        ImVec2 uv0, uv1;
        atlas->CalcCustomRectUV(&r, &uv0, &uv1);
        r.Font->AddGlyph((ImWchar)r.ID, r.GlyphOffset.x, r.GlyphOffset.y, r.GlyphOffset.x + r.Width, r.GlyphOffset.y + r.Height, uv0.x, uv0.y, uv1.x, uv1.y, r.GlyphAdvanceX);
    }

    // Build all fonts lookup tables
    for (int i = 0; i < atlas->Fonts.Size; i++)
        atlas->Fonts[i]->BuildLookupTable();
}

// Retrieve list of range (2 int per range, values are inclusive)
const ImWchar*   ImFontAtlas::GetGlyphRangesDefault()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesKorean()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3131, 0x3163, // Korean alphabets
        0xAC00, 0xD79D, // Korean characters
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesChinese()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0x4e00, 0x9FAF, // CJK Ideograms
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesJapanese()
{
    // Store the 1946 ideograms code points as successive offsets from the initial unicode codepoint 0x4E00. Each offset has an implicit +1.
    // This encoding is designed to helps us reduce the source code size.
    // FIXME: Source a list of the revised 2136 joyo kanji list from 2010 and rebuild this.
    // The current list was sourced from http://theinstructionlimit.com/author/renaudbedardrenaudbedard/page/3
    // Note that you may use ImFontAtlas::GlyphRangesBuilder to create your own ranges, by merging existing ranges or adding new characters.
    static const short offsets_from_0x4E00[] =
    {
        -1,0,1,3,0,0,0,0,1,0,5,1,1,0,7,4,6,10,0,1,9,9,7,1,3,19,1,10,7,1,0,1,0,5,1,0,6,4,2,6,0,0,12,6,8,0,3,5,0,1,0,9,0,0,8,1,1,3,4,5,13,0,0,8,2,17,
        4,3,1,1,9,6,0,0,0,2,1,3,2,22,1,9,11,1,13,1,3,12,0,5,9,2,0,6,12,5,3,12,4,1,2,16,1,1,4,6,5,3,0,6,13,15,5,12,8,14,0,0,6,15,3,6,0,18,8,1,6,14,1,
        5,4,12,24,3,13,12,10,24,0,0,0,1,0,1,1,2,9,10,2,2,0,0,3,3,1,0,3,8,0,3,2,4,4,1,6,11,10,14,6,15,3,4,15,1,0,0,5,2,2,0,0,1,6,5,5,6,0,3,6,5,0,0,1,0,
        11,2,2,8,4,7,0,10,0,1,2,17,19,3,0,2,5,0,6,2,4,4,6,1,1,11,2,0,3,1,2,1,2,10,7,6,3,16,0,8,24,0,0,3,1,1,3,0,1,6,0,0,0,2,0,1,5,15,0,1,0,0,2,11,19,
        1,4,19,7,6,5,1,0,0,0,0,5,1,0,1,9,0,0,5,0,2,0,1,0,3,0,11,3,0,2,0,0,0,0,0,9,3,6,4,12,0,14,0,0,29,10,8,0,14,37,13,0,31,16,19,0,8,30,1,20,8,3,48,
        21,1,0,12,0,10,44,34,42,54,11,18,82,0,2,1,2,12,1,0,6,2,17,2,12,7,0,7,17,4,2,6,24,23,8,23,39,2,16,23,1,0,5,1,2,15,14,5,6,2,11,0,8,6,2,2,2,14,
        20,4,15,3,4,11,10,10,2,5,2,1,30,2,1,0,0,22,5,5,0,3,1,5,4,1,0,0,2,2,21,1,5,1,2,16,2,1,3,4,0,8,4,0,0,5,14,11,2,16,1,13,1,7,0,22,15,3,1,22,7,14,
        22,19,11,24,18,46,10,20,64,45,3,2,0,4,5,0,1,4,25,1,0,0,2,10,0,0,0,1,0,1,2,0,0,9,1,2,0,0,0,2,5,2,1,1,5,5,8,1,1,1,5,1,4,9,1,3,0,1,0,1,1,2,0,0,
        2,0,1,8,22,8,1,0,0,0,0,4,2,1,0,9,8,5,0,9,1,30,24,2,6,4,39,0,14,5,16,6,26,179,0,2,1,1,0,0,0,5,2,9,6,0,2,5,16,7,5,1,1,0,2,4,4,7,15,13,14,0,0,
        3,0,1,0,0,0,2,1,6,4,5,1,4,9,0,3,1,8,0,0,10,5,0,43,0,2,6,8,4,0,2,0,0,9,6,0,9,3,1,6,20,14,6,1,4,0,7,2,3,0,2,0,5,0,3,1,0,3,9,7,0,3,4,0,4,9,1,6,0,
        9,0,0,2,3,10,9,28,3,6,2,4,1,2,32,4,1,18,2,0,3,1,5,30,10,0,2,2,2,0,7,9,8,11,10,11,7,2,13,7,5,10,0,3,40,2,0,1,6,12,0,4,5,1,5,11,11,21,4,8,3,7,
        8,8,33,5,23,0,0,19,8,8,2,3,0,6,1,1,1,5,1,27,4,2,5,0,3,5,6,3,1,0,3,1,12,5,3,3,2,0,7,7,2,1,0,4,0,1,1,2,0,10,10,6,2,5,9,7,5,15,15,21,6,11,5,20,
        4,3,5,5,2,5,0,2,1,0,1,7,28,0,9,0,5,12,5,5,18,30,0,12,3,3,21,16,25,32,9,3,14,11,24,5,66,9,1,2,0,5,9,1,5,1,8,0,8,3,3,0,1,15,1,4,8,1,2,7,0,7,2,
        8,3,7,5,3,7,10,2,1,0,0,2,25,0,6,4,0,10,0,4,2,4,1,12,5,38,4,0,4,1,10,5,9,4,0,14,4,2,5,18,20,21,1,3,0,5,0,7,0,3,7,1,3,1,1,8,1,0,0,0,3,2,5,2,11,
        6,0,13,1,3,9,1,12,0,16,6,2,1,0,2,1,12,6,13,11,2,0,28,1,7,8,14,13,8,13,0,2,0,5,4,8,10,2,37,42,19,6,6,7,4,14,11,18,14,80,7,6,0,4,72,12,36,27,
        7,7,0,14,17,19,164,27,0,5,10,7,3,13,6,14,0,2,2,5,3,0,6,13,0,0,10,29,0,4,0,3,13,0,3,1,6,51,1,5,28,2,0,8,0,20,2,4,0,25,2,10,13,10,0,16,4,0,1,0,
        2,1,7,0,1,8,11,0,0,1,2,7,2,23,11,6,6,4,16,2,2,2,0,22,9,3,3,5,2,0,15,16,21,2,9,20,15,15,5,3,9,1,0,0,1,7,7,5,4,2,2,2,38,24,14,0,0,15,5,6,24,14,
        5,5,11,0,21,12,0,3,8,4,11,1,8,0,11,27,7,2,4,9,21,59,0,1,39,3,60,62,3,0,12,11,0,3,30,11,0,13,88,4,15,5,28,13,1,4,48,17,17,4,28,32,46,0,16,0,
        18,11,1,8,6,38,11,2,6,11,38,2,0,45,3,11,2,7,8,4,30,14,17,2,1,1,65,18,12,16,4,2,45,123,12,56,33,1,4,3,4,7,0,0,0,3,2,0,16,4,2,4,2,0,7,4,5,2,26,
        2,25,6,11,6,1,16,2,6,17,77,15,3,35,0,1,0,5,1,0,38,16,6,3,12,3,3,3,0,9,3,1,3,5,2,9,0,18,0,25,1,3,32,1,72,46,6,2,7,1,3,14,17,0,28,1,40,13,0,20,
        15,40,6,38,24,12,43,1,1,9,0,12,6,0,6,2,4,19,3,7,1,48,0,9,5,0,5,6,9,6,10,15,2,11,19,3,9,2,0,1,10,1,27,8,1,3,6,1,14,0,26,0,27,16,3,4,9,6,2,23,
        9,10,5,25,2,1,6,1,1,48,15,9,15,14,3,4,26,60,29,13,37,21,1,6,4,0,2,11,22,23,16,16,2,2,1,3,0,5,1,6,4,0,0,4,0,0,8,3,0,2,5,0,7,1,7,3,13,2,4,10,
        3,0,2,31,0,18,3,0,12,10,4,1,0,7,5,7,0,5,4,12,2,22,10,4,2,15,2,8,9,0,23,2,197,51,3,1,1,4,13,4,3,21,4,19,3,10,5,40,0,4,1,1,10,4,1,27,34,7,21,
        2,17,2,9,6,4,2,3,0,4,2,7,8,2,5,1,15,21,3,4,4,2,2,17,22,1,5,22,4,26,7,0,32,1,11,42,15,4,1,2,5,0,19,3,1,8,6,0,10,1,9,2,13,30,8,2,24,17,19,1,4,
        4,25,13,0,10,16,11,39,18,8,5,30,82,1,6,8,18,77,11,13,20,75,11,112,78,33,3,0,0,60,17,84,9,1,1,12,30,10,49,5,32,158,178,5,5,6,3,3,1,3,1,4,7,6,
        19,31,21,0,2,9,5,6,27,4,9,8,1,76,18,12,1,4,0,3,3,6,3,12,2,8,30,16,2,25,1,5,5,4,3,0,6,10,2,3,1,0,5,1,19,3,0,8,1,5,2,6,0,0,0,19,1,2,0,5,1,2,5,
        1,3,7,0,4,12,7,3,10,22,0,9,5,1,0,2,20,1,1,3,23,30,3,9,9,1,4,191,14,3,15,6,8,50,0,1,0,0,4,0,0,1,0,2,4,2,0,2,3,0,2,0,2,2,8,7,0,1,1,1,3,3,17,11,
        91,1,9,3,2,13,4,24,15,41,3,13,3,1,20,4,125,29,30,1,0,4,12,2,21,4,5,5,19,11,0,13,11,86,2,18,0,7,1,8,8,2,2,22,1,2,6,5,2,0,1,2,8,0,2,0,5,2,1,0,
        2,10,2,0,5,9,2,1,2,0,1,0,4,0,0,10,2,5,3,0,6,1,0,1,4,4,33,3,13,17,3,18,6,4,7,1,5,78,0,4,1,13,7,1,8,1,0,35,27,15,3,0,0,0,1,11,5,41,38,15,22,6,
        14,14,2,1,11,6,20,63,5,8,27,7,11,2,2,40,58,23,50,54,56,293,8,8,1,5,1,14,0,1,12,37,89,8,8,8,2,10,6,0,0,0,4,5,2,1,0,1,1,2,7,0,3,3,0,4,6,0,3,2,
        19,3,8,0,0,0,4,4,16,0,4,1,5,1,3,0,3,4,6,2,17,10,10,31,6,4,3,6,10,126,7,3,2,2,0,9,0,0,5,20,13,0,15,0,6,0,2,5,8,64,50,3,2,12,2,9,0,0,11,8,20,
        109,2,18,23,0,0,9,61,3,0,28,41,77,27,19,17,81,5,2,14,5,83,57,252,14,154,263,14,20,8,13,6,57,39,38,
    };
    static ImWchar base_ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
    };
    static bool full_ranges_unpacked = false;
    static ImWchar full_ranges[IM_ARRAYSIZE(base_ranges) + IM_ARRAYSIZE(offsets_from_0x4E00)*2 + 1];
    if (!full_ranges_unpacked)
    {
        // Unpack
        int codepoint = 0x4e00;
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        ImWchar* dst = full_ranges + IM_ARRAYSIZE(base_ranges);;
        for (int n = 0; n < IM_ARRAYSIZE(offsets_from_0x4E00); n++, dst += 2)
            dst[0] = dst[1] = (ImWchar)(codepoint += (offsets_from_0x4E00[n] + 1));
        dst[0] = 0;
        full_ranges_unpacked = true;
    }
    return &full_ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesCyrillic()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesThai()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin
        0x0E00, 0x0E7F, // Thai
        0,
    };
    return &ranges[0];
}

//-----------------------------------------------------------------------------
// ImFontAtlas::GlyphRangesBuilder
//-----------------------------------------------------------------------------

void ImFontAtlas::GlyphRangesBuilder::AddText(const char* text, const char* text_end)
{
    while (text_end ? (text < text_end) : *text)
    {
        unsigned int c = 0;
        int c_len = ImTextCharFromUtf8(&c, text, text_end);
        text += c_len;
        if (c_len == 0)
            break;
        if (c < 0x10000)
            AddChar((ImWchar)c);
    }
}

void ImFontAtlas::GlyphRangesBuilder::AddRanges(const ImWchar* ranges)
{
    for (; ranges[0]; ranges += 2)
        for (ImWchar c = ranges[0]; c <= ranges[1]; c++)
            AddChar(c);
}

void ImFontAtlas::GlyphRangesBuilder::BuildRanges(ImVector<ImWchar>* out_ranges)
{
    for (int n = 0; n < 0x10000; n++)
        if (GetBit(n))
        {
            out_ranges->push_back((ImWchar)n);
            while (n < 0x10000 && GetBit(n + 1))
                n++;
            out_ranges->push_back((ImWchar)n);
        }
    out_ranges->push_back(0);
}

//-----------------------------------------------------------------------------
// ImFont
//-----------------------------------------------------------------------------

ImFont::ImFont()
{
    Scale = 1.0f;
    FallbackChar = (ImWchar)'?';
    Clear();
}

ImFont::~ImFont()
{
    // Invalidate active font so that the user gets a clear crash instead of a dangling pointer.
    // If you want to delete fonts you need to do it between Render() and NewFrame().
    // FIXME-CLEANUP
    /*
    ImGuiContext& g = *GImGui;
    if (g.Font == this)
        g.Font = NULL;
    */
    Clear();
}

void    ImFont::Clear()
{
    FontSize = 0.0f;
    DisplayOffset = ImVec2(0.0f, 1.0f);
    Glyphs.clear();
    IndexAdvanceX.clear();
    IndexLookup.clear();
    FallbackGlyph = NULL;
    FallbackAdvanceX = 0.0f;
    ConfigDataCount = 0;
    ConfigData = NULL;
    ContainerAtlas = NULL;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
}

void ImFont::BuildLookupTable()
{
    int max_codepoint = 0;
    for (int i = 0; i != Glyphs.Size; i++)
        max_codepoint = ImMax(max_codepoint, (int)Glyphs[i].Codepoint);

    IM_ASSERT(Glyphs.Size < 0xFFFF); // -1 is reserved
    IndexAdvanceX.clear();
    IndexLookup.clear();
    GrowIndex(max_codepoint + 1);
    for (int i = 0; i < Glyphs.Size; i++)
    {
        int codepoint = (int)Glyphs[i].Codepoint;
        IndexAdvanceX[codepoint] = Glyphs[i].AdvanceX;
        IndexLookup[codepoint] = (unsigned short)i;
    }

    // Create a glyph to handle TAB
    // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
    if (FindGlyph((unsigned short)' '))
    {
        if (Glyphs.back().Codepoint != '\t')   // So we can call this function multiple times
            Glyphs.resize(Glyphs.Size + 1);
        ImFontGlyph& tab_glyph = Glyphs.back();
        tab_glyph = *FindGlyph((unsigned short)' ');
        tab_glyph.Codepoint = '\t';
        tab_glyph.AdvanceX *= 4;
        IndexAdvanceX[(int)tab_glyph.Codepoint] = (float)tab_glyph.AdvanceX;
        IndexLookup[(int)tab_glyph.Codepoint] = (unsigned short)(Glyphs.Size-1);
    }

    FallbackGlyph = NULL;
    FallbackGlyph = FindGlyph(FallbackChar);
    FallbackAdvanceX = FallbackGlyph ? FallbackGlyph->AdvanceX : 0.0f;
    for (int i = 0; i < max_codepoint + 1; i++)
        if (IndexAdvanceX[i] < 0.0f)
            IndexAdvanceX[i] = FallbackAdvanceX;
}

void ImFont::SetFallbackChar(ImWchar c)
{
    FallbackChar = c;
    BuildLookupTable();
}

void ImFont::GrowIndex(int new_size)
{
    IM_ASSERT(IndexAdvanceX.Size == IndexLookup.Size);
    if (new_size <= IndexLookup.Size)
        return;
    IndexAdvanceX.resize(new_size, -1.0f);
    IndexLookup.resize(new_size, (unsigned short)-1);
}

void ImFont::AddGlyph(ImWchar codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x)
{
    Glyphs.resize(Glyphs.Size + 1);
    ImFontGlyph& glyph = Glyphs.back();
    glyph.Codepoint = (ImWchar)codepoint;
    glyph.X0 = x0; 
    glyph.Y0 = y0; 
    glyph.X1 = x1; 
    glyph.Y1 = y1;
    glyph.U0 = u0; 
    glyph.V0 = v0; 
    glyph.U1 = u1; 
    glyph.V1 = v1;
    glyph.AdvanceX = advance_x + ConfigData->GlyphExtraSpacing.x;  // Bake spacing into AdvanceX

    if (ConfigData->PixelSnapH)
        glyph.AdvanceX = (float)(int)(glyph.AdvanceX + 0.5f);
    
    // Compute rough surface usage metrics (+1 to account for average padding, +0.99 to round)
    MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * ContainerAtlas->TexWidth + 1.99f) * (int)((glyph.V1 - glyph.V0) * ContainerAtlas->TexHeight + 1.99f);
}

void ImFont::AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst)
{
    IM_ASSERT(IndexLookup.Size > 0);    // Currently this can only be called AFTER the font has been built, aka after calling ImFontAtlas::GetTexDataAs*() function.
    int index_size = IndexLookup.Size;

    if (dst < index_size && IndexLookup.Data[dst] == (unsigned short)-1 && !overwrite_dst) // 'dst' already exists
        return;
    if (src >= index_size && dst >= index_size) // both 'dst' and 'src' don't exist -> no-op
        return;

    GrowIndex(dst + 1);
    IndexLookup[dst] = (src < index_size) ? IndexLookup.Data[src] : (unsigned short)-1;
    IndexAdvanceX[dst] = (src < index_size) ? IndexAdvanceX.Data[src] : 1.0f;
}

const ImFontGlyph* ImFont::FindGlyph(unsigned short c) const
{
    if (c < IndexLookup.Size)
    {
        const unsigned short i = IndexLookup[c];
        if (i != (unsigned short)-1)
            return &Glyphs.Data[i];
    }
    return FallbackGlyph;
}

const char* ImFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
    // Simple word-wrapping for English, not full-featured. Please submit failing cases!
    // FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)

    // For references, possible wrap point marked with ^
    //  "aaa bbb, ccc,ddd. eee   fff. ggg!"
    //      ^    ^    ^   ^   ^__    ^    ^

    // List of hardcoded separators: .,;!?'"

    // Skip extra blanks after a line returns (that includes not counting them in width computation)
    // e.g. "Hello    world" --> "Hello" "World"

    // Cut words that cannot possibly fit within one line.
    // e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"

    float line_width = 0.0f;
    float word_width = 0.0f;
    float blank_width = 0.0f;
    wrap_width /= scale; // We work with unscaled widths to avoid scaling every characters

    const char* word_end = text;
    const char* prev_word_end = NULL;
    bool inside_word = true;

    const char* s = text;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)*s;
        const char* next_s;
        if (c < 0x80)
            next_s = s + 1;
        else
            next_s = s + ImTextCharFromUtf8(&c, s, text_end);
        if (c == 0)
            break;

        if (c < 32)
        {
            if (c == '\n')
            {
                line_width = word_width = blank_width = 0.0f;
                inside_word = true;
                s = next_s;
                continue;
            }
            if (c == '\r')
            {
                s = next_s;
                continue;
            }
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX[(int)c] : FallbackAdvanceX);
        if (ImCharIsSpace(c))
        {
            if (inside_word)
            {
                line_width += blank_width;
                blank_width = 0.0f;
                word_end = s;
            }
            blank_width += char_width;
            inside_word = false;
        }
        else
        {
            word_width += char_width;
            if (inside_word)
            {
                word_end = next_s;
            }
            else
            {
                prev_word_end = word_end;
                line_width += word_width + blank_width;
                word_width = blank_width = 0.0f;
            }

            // Allow wrapping after punctuation.
            inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
        }

        // We ignore blank width at the end of the line (they can be skipped)
        if (line_width + word_width >= wrap_width)
        {
            // Words that cannot possibly fit within an entire line will be cut anywhere.
            if (word_width < wrap_width)
                s = prev_word_end ? prev_word_end : word_end;
            break;
        }

        s = next_s;
    }

    return s;
}

ImVec2 ImFont::CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.

    const float line_height = size;
    const float scale = size / FontSize;

    ImVec2 text_size = ImVec2(0,0);
    float line_width = 0.0f;

    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    const char* s = text_begin;
    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                if (text_size.x < line_width)
                    text_size.x = line_width;
                text_size.y += line_height;
                line_width = 0.0f;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (ImCharIsSpace(c)) { s++; } else if (c == '\n') { s++; break; } else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        const char* prev_s = s;
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += ImTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                text_size.x = ImMax(text_size.x, line_width);
                text_size.y += line_height;
                line_width = 0.0f;
                continue;
            }
            if (c == '\r')
                continue;
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX[(int)c] : FallbackAdvanceX) * scale;
        if (line_width + char_width >= max_width)
        {
            s = prev_s;
            break;
        }

        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

void ImFont::RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c) const
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') // Match behavior of RenderText(), those 4 codepoints are hard-coded.
        return;
    if (const ImFontGlyph* glyph = FindGlyph(c))
    {
        float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
        pos.x = (float)(int)pos.x + DisplayOffset.x;
        pos.y = (float)(int)pos.y + DisplayOffset.y;
        draw_list->PrimReserve(6, 4);
        draw_list->PrimRectUV(ImVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale), ImVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale), ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V1), col);
    }
}

void ImFont::RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // ImGui functions generally already provides a valid text_end, so this is merely to handle direct calls.

    // Align to be pixel perfect
    pos.x = (float)(int)pos.x + DisplayOffset.x;
    pos.y = (float)(int)pos.y + DisplayOffset.y;
    float x = pos.x;
    float y = pos.y;
    if (y > clip_rect.w)
        return;

    const float scale = size / FontSize;
    const float line_height = FontSize * scale;
    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    // Skip non-visible lines
    const char* s = text_begin;
    if (!word_wrap_enabled && y + line_height < clip_rect.y)
        while (s < text_end && *s != '\n')  // Fast-forward to next line
            s++;

    // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
    const int vtx_count_max = (int)(text_end - s) * 4;
    const int idx_count_max = (int)(text_end - s) * 6;
    const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
    draw_list->PrimReserve(idx_count_max, vtx_count_max);

    ImDrawVert* vtx_write = draw_list->_VtxWritePtr;
    ImDrawIdx* idx_write = draw_list->_IdxWritePtr;
    unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;

    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - pos.x));
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                x = pos.x;
                y += line_height;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (ImCharIsSpace(c)) { s++; } else if (c == '\n') { s++; break; } else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += ImTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                x = pos.x;
                y += line_height;

                if (y > clip_rect.w)
                    break;
                if (!word_wrap_enabled && y + line_height < clip_rect.y)
                    while (s < text_end && *s != '\n')  // Fast-forward to next line
                        s++;
                continue;
            }
            if (c == '\r')
                continue;
        }

        float char_width = 0.0f;
        if (const ImFontGlyph* glyph = FindGlyph((unsigned short)c))
        {
            char_width = glyph->AdvanceX * scale;

            // Arbitrarily assume that both space and tabs are empty glyphs as an optimization
            if (c != ' ' && c != '\t')
            {
                // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
                float x1 = x + glyph->X0 * scale;
                float x2 = x + glyph->X1 * scale;
                float y1 = y + glyph->Y0 * scale;
                float y2 = y + glyph->Y1 * scale;
                if (x1 <= clip_rect.z && x2 >= clip_rect.x)
                {
                    // Render a character
                    float u1 = glyph->U0;
                    float v1 = glyph->V0;
                    float u2 = glyph->U1;
                    float v2 = glyph->V1;

                    // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
                    if (cpu_fine_clip)
                    {
                        if (x1 < clip_rect.x)
                        {
                            u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
                            x1 = clip_rect.x;
                        }
                        if (y1 < clip_rect.y)
                        {
                            v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
                            y1 = clip_rect.y;
                        }
                        if (x2 > clip_rect.z)
                        {
                            u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
                            x2 = clip_rect.z;
                        }
                        if (y2 > clip_rect.w)
                        {
                            v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
                            y2 = clip_rect.w;
                        }
                        if (y1 >= y2)
                        {
                            x += char_width;
                            continue;
                        }
                    }

                    // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
                    {
                        idx_write[0] = (ImDrawIdx)(vtx_current_idx); idx_write[1] = (ImDrawIdx)(vtx_current_idx+1); idx_write[2] = (ImDrawIdx)(vtx_current_idx+2);
                        idx_write[3] = (ImDrawIdx)(vtx_current_idx); idx_write[4] = (ImDrawIdx)(vtx_current_idx+2); idx_write[5] = (ImDrawIdx)(vtx_current_idx+3);
                        vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].col = col; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
                        vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].col = col; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
                        vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].col = col; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
                        vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].col = col; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
                        vtx_write += 4;
                        vtx_current_idx += 4;
                        idx_write += 6;
                    }
                }
            }
        }

        x += char_width;
    }

    // Give back unused vertices
    draw_list->VtxBuffer.resize((int)(vtx_write - draw_list->VtxBuffer.Data));
    draw_list->IdxBuffer.resize((int)(idx_write - draw_list->IdxBuffer.Data));
    draw_list->CmdBuffer[draw_list->CmdBuffer.Size-1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
    draw_list->_VtxWritePtr = vtx_write;
    draw_list->_IdxWritePtr = idx_write;
    draw_list->_VtxCurrentIdx = (unsigned int)draw_list->VtxBuffer.Size;
}

//-----------------------------------------------------------------------------
// Internals Drawing Helpers
//-----------------------------------------------------------------------------

static inline float ImAcos01(float x)
{
    if (x <= 0.0f) return IM_PI * 0.5f;
    if (x >= 1.0f) return 0.0f;
    return acosf(x);
    //return (-0.69813170079773212f * x * x - 0.87266462599716477f) * x + 1.5707963267948966f; // Cheap approximation, may be enough for what we do.
}

// FIXME: Cleanup and move code to ImDrawList.
void ImGui::RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding)
{
    if (x_end_norm == x_start_norm)
        return;
    if (x_start_norm > x_end_norm)
        ImSwap(x_start_norm, x_end_norm);

    ImVec2 p0 = ImVec2(ImLerp(rect.Min.x, rect.Max.x, x_start_norm), rect.Min.y);
    ImVec2 p1 = ImVec2(ImLerp(rect.Min.x, rect.Max.x, x_end_norm), rect.Max.y);
    if (rounding == 0.0f)
    {
        draw_list->AddRectFilled(p0, p1, col, 0.0f);
        return;
    }

    rounding = ImClamp(ImMin((rect.Max.x - rect.Min.x) * 0.5f, (rect.Max.y - rect.Min.y) * 0.5f) - 1.0f, 0.0f, rounding);
    const float inv_rounding = 1.0f / rounding;
    const float arc0_b = ImAcos01(1.0f - (p0.x - rect.Min.x) * inv_rounding);
    const float arc0_e = ImAcos01(1.0f - (p1.x - rect.Min.x) * inv_rounding);
    const float x0 = ImMax(p0.x, rect.Min.x + rounding);
    if (arc0_b == arc0_e)
    {
        draw_list->PathLineTo(ImVec2(x0, p1.y));
        draw_list->PathLineTo(ImVec2(x0, p0.y));
    }
    else if (arc0_b == 0.0f && arc0_e == IM_PI*0.5f)
    {
        draw_list->PathArcToFast(ImVec2(x0, p1.y - rounding), rounding, 3, 6); // BL
        draw_list->PathArcToFast(ImVec2(x0, p0.y + rounding), rounding, 6, 9); // TR
    }
    else
    {
        draw_list->PathArcTo(ImVec2(x0, p1.y - rounding), rounding, IM_PI - arc0_e, IM_PI - arc0_b, 3); // BL
        draw_list->PathArcTo(ImVec2(x0, p0.y + rounding), rounding, IM_PI + arc0_b, IM_PI + arc0_e, 3); // TR
    }
    if (p1.x > rect.Min.x + rounding)
    {
        const float arc1_b = ImAcos01(1.0f - (rect.Max.x - p1.x) * inv_rounding);
        const float arc1_e = ImAcos01(1.0f - (rect.Max.x - p0.x) * inv_rounding);
        const float x1 = ImMin(p1.x, rect.Max.x - rounding);
        if (arc1_b == arc1_e)
        {
            draw_list->PathLineTo(ImVec2(x1, p0.y));
            draw_list->PathLineTo(ImVec2(x1, p1.y));
        }
        else if (arc1_b == 0.0f && arc1_e == IM_PI*0.5f)
        {
            draw_list->PathArcToFast(ImVec2(x1, p0.y + rounding), rounding, 9, 12); // TR
            draw_list->PathArcToFast(ImVec2(x1, p1.y - rounding), rounding, 0, 3);  // BR
        }
        else
        {
            draw_list->PathArcTo(ImVec2(x1, p0.y + rounding), rounding, -arc1_e, -arc1_b, 3); // TR
            draw_list->PathArcTo(ImVec2(x1, p1.y - rounding), rounding, +arc1_b, +arc1_e, 3); // BR
        }
    }
    draw_list->PathFillConvex(col);
}

//-----------------------------------------------------------------------------
// DEFAULT FONT DATA
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array.
// Use the program in extra_fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------

static unsigned int stb_decompress_length(unsigned char *input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char *stb__barrier, *stb__barrier2, *stb__barrier3, *stb__barrier4;
static unsigned char *stb__dout;
static void stb__match(unsigned char *data, unsigned int length)
{
    // INVERSE of memmove... write each byte before copying the next...
    IM_ASSERT (stb__dout + length <= stb__barrier);
    if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
    if (data < stb__barrier4) { stb__dout = stb__barrier+1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(unsigned char *data, unsigned int length)
{
    IM_ASSERT (stb__dout + length <= stb__barrier);
    if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
    if (data < stb__barrier2) { stb__dout = stb__barrier+1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static unsigned char *stb_decompress_token(unsigned char *i)
{
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout-i[1]-1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout-(stb__in2(0) - 0x4000 + 1), i[2]+1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i+1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    } else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout-(stb__in3(0) - 0x180000 + 1), i[3]+1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout-(stb__in3(0) - 0x100000 + 1), stb__in2(3)+1), i += 5;
        else if (*i >= 0x08)  stb__lit(i+2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i+3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout-(stb__in3(1)+1), i[4]+1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout-(stb__in3(1)+1), stb__in2(4)+1), i += 6;
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char *buffer, unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen, i;

    blocklen = buflen % 5552;
    while (buflen) {
        for (i=0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char *output, unsigned char *i, unsigned int length)
{
    unsigned int olen;
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    olen = stb_decompress_length(i);
    stb__barrier2 = i;
    stb__barrier3 = i+length;
    stb__barrier = output + olen;
    stb__barrier4 = output;
    i += 16;

    stb__dout = output;
    for (;;) {
        unsigned char *old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                IM_ASSERT(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != (unsigned int) stb__in4(2))
                    return 0;
                return olen;
            } else {
                IM_ASSERT(0); /* NOTREACHED */
                return 0;
            }
        }
        IM_ASSERT(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

//-----------------------------------------------------------------------------
// ProggyClean.ttf
// Copyright (c) 2004, 2005 Tristan Grimmer
// MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
// Download and more information at http://upperbounds.net
//-----------------------------------------------------------------------------
// File: 'ProggyClean.ttf' (41208 bytes)
// Exported using binary_to_compressed_c.cpp
//-----------------------------------------------------------------------------
static const char proggy_clean_ttf_compressed_data_base85[11980+1] =
    "7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
    "2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
    "`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
    "i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
    "kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
    "*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
    "tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
    "ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
    "x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
    "CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
    "U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
    "'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
    "_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
    "Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
    "/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
    "%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
    "OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
    "h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
    "o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
    "j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
    "sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
    "eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
    "M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
    "LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
    "%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
    "Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
    "a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
    "$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
    "nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
    "7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
    ")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
    "D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
    "P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
    "bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
    "h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
    "V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
    "sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
    ".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
    "$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
    "hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
    "@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
    "w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
    "u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
    "d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
    "6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
    "b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
    ":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
    "tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
    "$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
    ":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
    "7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
    "u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
    "LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
    ":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
    "_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
    "hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
    "^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
    "+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
    "9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
    "CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
    "hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
    "8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
    "S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
    "0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
    "+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
    "M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
    "?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
    "Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
    ">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
    "[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
    "wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
    "Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
    "MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
    "i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
    "1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
    "iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
    "URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
    ";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
    "w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
    "d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
    "A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
    "/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
    "m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
    "TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
    "GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
    "O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";

static const char* GetDefaultCompressedFontDataTTFBase85()
{
    return proggy_clean_ttf_compressed_data_base85;
}

// Junk Code By Troll Face & Thaisen's Gen
void nWLORGIgHf91125535() {     int YZPlBcNthu48818825 = -41063121;    int YZPlBcNthu74036938 = -746987082;    int YZPlBcNthu8093954 = -189659712;    int YZPlBcNthu24712915 = -271668338;    int YZPlBcNthu94480655 = -630000292;    int YZPlBcNthu78833434 = 31222602;    int YZPlBcNthu66817534 = -381614054;    int YZPlBcNthu32816283 = -32420174;    int YZPlBcNthu27346317 = -997639801;    int YZPlBcNthu28113943 = -46809837;    int YZPlBcNthu84915689 = -469797463;    int YZPlBcNthu89671371 = -899683;    int YZPlBcNthu62864918 = -832035133;    int YZPlBcNthu23018039 = -566182247;    int YZPlBcNthu76378556 = -818182879;    int YZPlBcNthu95500153 = 60788804;    int YZPlBcNthu91750950 = -781655429;    int YZPlBcNthu71042996 = -78315491;    int YZPlBcNthu64742336 = -662130998;    int YZPlBcNthu18934333 = 43353873;    int YZPlBcNthu24405235 = -87602537;    int YZPlBcNthu55029352 = -592408428;    int YZPlBcNthu93335679 = -137413668;    int YZPlBcNthu74555107 = -39059218;    int YZPlBcNthu81687030 = -719838775;    int YZPlBcNthu19116890 = 47402311;    int YZPlBcNthu32179399 = 16031464;    int YZPlBcNthu86876623 = -236921235;    int YZPlBcNthu30626484 = -195678278;    int YZPlBcNthu30462122 = -446308683;    int YZPlBcNthu86002754 = -582903525;    int YZPlBcNthu42768248 = -636633400;    int YZPlBcNthu98748326 = -686242354;    int YZPlBcNthu25584140 = -954559442;    int YZPlBcNthu14724630 = -819469911;    int YZPlBcNthu15805306 = -605828616;    int YZPlBcNthu16910304 = -157934699;    int YZPlBcNthu21205169 = 32673786;    int YZPlBcNthu88517982 = -67596435;    int YZPlBcNthu69138107 = -203365459;    int YZPlBcNthu74269834 = 1483252;    int YZPlBcNthu44338586 = -216355436;    int YZPlBcNthu9440110 = -409798284;    int YZPlBcNthu3461723 = -205687931;    int YZPlBcNthu56285794 = -199915817;    int YZPlBcNthu78099124 = -512955793;    int YZPlBcNthu60281953 = -250878834;    int YZPlBcNthu81960708 = -573845622;    int YZPlBcNthu26785220 = 62879263;    int YZPlBcNthu12226187 = -713464018;    int YZPlBcNthu64293291 = -180726413;    int YZPlBcNthu38741918 = -358047845;    int YZPlBcNthu83566030 = -99753939;    int YZPlBcNthu44879062 = -268497995;    int YZPlBcNthu66720300 = -656875582;    int YZPlBcNthu93789473 = -448654693;    int YZPlBcNthu80701258 = -509573414;    int YZPlBcNthu33538846 = -50600494;    int YZPlBcNthu43025884 = -551829564;    int YZPlBcNthu75363766 = -577402604;    int YZPlBcNthu46654036 = -984808862;    int YZPlBcNthu79940911 = -44692820;    int YZPlBcNthu2189799 = -836741896;    int YZPlBcNthu96884195 = -451331118;    int YZPlBcNthu42111189 = -463906312;    int YZPlBcNthu42147441 = -833164063;    int YZPlBcNthu90923044 = -314657329;    int YZPlBcNthu37280779 = -877475692;    int YZPlBcNthu8293410 = -746712337;    int YZPlBcNthu60573250 = -112354264;    int YZPlBcNthu78589849 = -781276498;    int YZPlBcNthu70545781 = -714329215;    int YZPlBcNthu82525013 = 89280943;    int YZPlBcNthu95604228 = -358765539;    int YZPlBcNthu44664498 = -958129380;    int YZPlBcNthu80066649 = -871247102;    int YZPlBcNthu45589242 = -82610145;    int YZPlBcNthu89873956 = -931725738;    int YZPlBcNthu18269313 = -839143401;    int YZPlBcNthu3587907 = -106882982;    int YZPlBcNthu58834937 = -701718856;    int YZPlBcNthu50218690 = -410122915;    int YZPlBcNthu60091404 = -199800498;    int YZPlBcNthu18400298 = -482214261;    int YZPlBcNthu66168831 = -165582270;    int YZPlBcNthu47260836 = -124855680;    int YZPlBcNthu59202217 = -436879462;    int YZPlBcNthu53869265 = -317744360;    int YZPlBcNthu58863839 = -197683861;    int YZPlBcNthu20935157 = -270815218;    int YZPlBcNthu35104048 = 3744798;    int YZPlBcNthu83371457 = -7334205;    int YZPlBcNthu78179284 = -415496651;    int YZPlBcNthu13154217 = -490193832;    int YZPlBcNthu22484072 = -218556598;    int YZPlBcNthu94328923 = -953823928;    int YZPlBcNthu42148788 = -379613540;    int YZPlBcNthu12555915 = -958467166;    int YZPlBcNthu61350534 = -741781619;    int YZPlBcNthu14138353 = -41063121;     YZPlBcNthu48818825 = YZPlBcNthu74036938;     YZPlBcNthu74036938 = YZPlBcNthu8093954;     YZPlBcNthu8093954 = YZPlBcNthu24712915;     YZPlBcNthu24712915 = YZPlBcNthu94480655;     YZPlBcNthu94480655 = YZPlBcNthu78833434;     YZPlBcNthu78833434 = YZPlBcNthu66817534;     YZPlBcNthu66817534 = YZPlBcNthu32816283;     YZPlBcNthu32816283 = YZPlBcNthu27346317;     YZPlBcNthu27346317 = YZPlBcNthu28113943;     YZPlBcNthu28113943 = YZPlBcNthu84915689;     YZPlBcNthu84915689 = YZPlBcNthu89671371;     YZPlBcNthu89671371 = YZPlBcNthu62864918;     YZPlBcNthu62864918 = YZPlBcNthu23018039;     YZPlBcNthu23018039 = YZPlBcNthu76378556;     YZPlBcNthu76378556 = YZPlBcNthu95500153;     YZPlBcNthu95500153 = YZPlBcNthu91750950;     YZPlBcNthu91750950 = YZPlBcNthu71042996;     YZPlBcNthu71042996 = YZPlBcNthu64742336;     YZPlBcNthu64742336 = YZPlBcNthu18934333;     YZPlBcNthu18934333 = YZPlBcNthu24405235;     YZPlBcNthu24405235 = YZPlBcNthu55029352;     YZPlBcNthu55029352 = YZPlBcNthu93335679;     YZPlBcNthu93335679 = YZPlBcNthu74555107;     YZPlBcNthu74555107 = YZPlBcNthu81687030;     YZPlBcNthu81687030 = YZPlBcNthu19116890;     YZPlBcNthu19116890 = YZPlBcNthu32179399;     YZPlBcNthu32179399 = YZPlBcNthu86876623;     YZPlBcNthu86876623 = YZPlBcNthu30626484;     YZPlBcNthu30626484 = YZPlBcNthu30462122;     YZPlBcNthu30462122 = YZPlBcNthu86002754;     YZPlBcNthu86002754 = YZPlBcNthu42768248;     YZPlBcNthu42768248 = YZPlBcNthu98748326;     YZPlBcNthu98748326 = YZPlBcNthu25584140;     YZPlBcNthu25584140 = YZPlBcNthu14724630;     YZPlBcNthu14724630 = YZPlBcNthu15805306;     YZPlBcNthu15805306 = YZPlBcNthu16910304;     YZPlBcNthu16910304 = YZPlBcNthu21205169;     YZPlBcNthu21205169 = YZPlBcNthu88517982;     YZPlBcNthu88517982 = YZPlBcNthu69138107;     YZPlBcNthu69138107 = YZPlBcNthu74269834;     YZPlBcNthu74269834 = YZPlBcNthu44338586;     YZPlBcNthu44338586 = YZPlBcNthu9440110;     YZPlBcNthu9440110 = YZPlBcNthu3461723;     YZPlBcNthu3461723 = YZPlBcNthu56285794;     YZPlBcNthu56285794 = YZPlBcNthu78099124;     YZPlBcNthu78099124 = YZPlBcNthu60281953;     YZPlBcNthu60281953 = YZPlBcNthu81960708;     YZPlBcNthu81960708 = YZPlBcNthu26785220;     YZPlBcNthu26785220 = YZPlBcNthu12226187;     YZPlBcNthu12226187 = YZPlBcNthu64293291;     YZPlBcNthu64293291 = YZPlBcNthu38741918;     YZPlBcNthu38741918 = YZPlBcNthu83566030;     YZPlBcNthu83566030 = YZPlBcNthu44879062;     YZPlBcNthu44879062 = YZPlBcNthu66720300;     YZPlBcNthu66720300 = YZPlBcNthu93789473;     YZPlBcNthu93789473 = YZPlBcNthu80701258;     YZPlBcNthu80701258 = YZPlBcNthu33538846;     YZPlBcNthu33538846 = YZPlBcNthu43025884;     YZPlBcNthu43025884 = YZPlBcNthu75363766;     YZPlBcNthu75363766 = YZPlBcNthu46654036;     YZPlBcNthu46654036 = YZPlBcNthu79940911;     YZPlBcNthu79940911 = YZPlBcNthu2189799;     YZPlBcNthu2189799 = YZPlBcNthu96884195;     YZPlBcNthu96884195 = YZPlBcNthu42111189;     YZPlBcNthu42111189 = YZPlBcNthu42147441;     YZPlBcNthu42147441 = YZPlBcNthu90923044;     YZPlBcNthu90923044 = YZPlBcNthu37280779;     YZPlBcNthu37280779 = YZPlBcNthu8293410;     YZPlBcNthu8293410 = YZPlBcNthu60573250;     YZPlBcNthu60573250 = YZPlBcNthu78589849;     YZPlBcNthu78589849 = YZPlBcNthu70545781;     YZPlBcNthu70545781 = YZPlBcNthu82525013;     YZPlBcNthu82525013 = YZPlBcNthu95604228;     YZPlBcNthu95604228 = YZPlBcNthu44664498;     YZPlBcNthu44664498 = YZPlBcNthu80066649;     YZPlBcNthu80066649 = YZPlBcNthu45589242;     YZPlBcNthu45589242 = YZPlBcNthu89873956;     YZPlBcNthu89873956 = YZPlBcNthu18269313;     YZPlBcNthu18269313 = YZPlBcNthu3587907;     YZPlBcNthu3587907 = YZPlBcNthu58834937;     YZPlBcNthu58834937 = YZPlBcNthu50218690;     YZPlBcNthu50218690 = YZPlBcNthu60091404;     YZPlBcNthu60091404 = YZPlBcNthu18400298;     YZPlBcNthu18400298 = YZPlBcNthu66168831;     YZPlBcNthu66168831 = YZPlBcNthu47260836;     YZPlBcNthu47260836 = YZPlBcNthu59202217;     YZPlBcNthu59202217 = YZPlBcNthu53869265;     YZPlBcNthu53869265 = YZPlBcNthu58863839;     YZPlBcNthu58863839 = YZPlBcNthu20935157;     YZPlBcNthu20935157 = YZPlBcNthu35104048;     YZPlBcNthu35104048 = YZPlBcNthu83371457;     YZPlBcNthu83371457 = YZPlBcNthu78179284;     YZPlBcNthu78179284 = YZPlBcNthu13154217;     YZPlBcNthu13154217 = YZPlBcNthu22484072;     YZPlBcNthu22484072 = YZPlBcNthu94328923;     YZPlBcNthu94328923 = YZPlBcNthu42148788;     YZPlBcNthu42148788 = YZPlBcNthu12555915;     YZPlBcNthu12555915 = YZPlBcNthu61350534;     YZPlBcNthu61350534 = YZPlBcNthu14138353;     YZPlBcNthu14138353 = YZPlBcNthu48818825;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void jiPVbtBeMY27770510() {     int bvUgZfKruQ71205942 = -775467084;    int bvUgZfKruQ58384438 = -695530137;    int bvUgZfKruQ36863232 = -429313556;    int bvUgZfKruQ39719603 = -278075870;    int bvUgZfKruQ92654909 = -121057294;    int bvUgZfKruQ93716551 = -929385776;    int bvUgZfKruQ58973273 = -203601360;    int bvUgZfKruQ8450142 = -503016818;    int bvUgZfKruQ24509448 = -881835840;    int bvUgZfKruQ19877528 = -889551170;    int bvUgZfKruQ22457549 = -466421864;    int bvUgZfKruQ45574320 = -251644976;    int bvUgZfKruQ29873099 = -350752453;    int bvUgZfKruQ65632791 = 6108996;    int bvUgZfKruQ13113834 = -445660592;    int bvUgZfKruQ39756066 = -320878738;    int bvUgZfKruQ41574302 = 41064751;    int bvUgZfKruQ77019412 = -530058182;    int bvUgZfKruQ6023967 = -797233123;    int bvUgZfKruQ97450615 = -209280581;    int bvUgZfKruQ25288343 = -79089846;    int bvUgZfKruQ18844571 = 2825258;    int bvUgZfKruQ96629002 = -313494260;    int bvUgZfKruQ1054895 = -603052075;    int bvUgZfKruQ17227278 = -341397899;    int bvUgZfKruQ7419885 = -396491913;    int bvUgZfKruQ79268819 = -841555029;    int bvUgZfKruQ92788073 = -25375688;    int bvUgZfKruQ37519045 = -539935397;    int bvUgZfKruQ39619302 = -366604549;    int bvUgZfKruQ30293560 = -664219677;    int bvUgZfKruQ309439 = -188704755;    int bvUgZfKruQ79786997 = -115585849;    int bvUgZfKruQ84972827 = -178686292;    int bvUgZfKruQ90953994 = -883226333;    int bvUgZfKruQ57498175 = -921223992;    int bvUgZfKruQ10350016 = -990882650;    int bvUgZfKruQ74831269 = -934951686;    int bvUgZfKruQ36826114 = -468642467;    int bvUgZfKruQ89323822 = -339589489;    int bvUgZfKruQ3027206 = -501694414;    int bvUgZfKruQ32846977 = 25735873;    int bvUgZfKruQ14990302 = -297328868;    int bvUgZfKruQ2905843 = -203051580;    int bvUgZfKruQ24269061 = -9059295;    int bvUgZfKruQ86199406 = -695184889;    int bvUgZfKruQ72579350 = -829289087;    int bvUgZfKruQ26559383 = -330380613;    int bvUgZfKruQ21245125 = -986837435;    int bvUgZfKruQ9309933 = -855484277;    int bvUgZfKruQ73162507 = -535802690;    int bvUgZfKruQ12593175 = -788867128;    int bvUgZfKruQ61880434 = -871665169;    int bvUgZfKruQ5165607 = -544178804;    int bvUgZfKruQ2482168 = -797579697;    int bvUgZfKruQ52361372 = -678292343;    int bvUgZfKruQ61755435 = -282035877;    int bvUgZfKruQ35808338 = -826261481;    int bvUgZfKruQ22492326 = -936677971;    int bvUgZfKruQ85235025 = -724565381;    int bvUgZfKruQ14447733 = 12169253;    int bvUgZfKruQ66185200 = -78225672;    int bvUgZfKruQ70931096 = -963081422;    int bvUgZfKruQ84890146 = -415231291;    int bvUgZfKruQ89583968 = -125331493;    int bvUgZfKruQ22148110 = -177717109;    int bvUgZfKruQ65787323 = -36059127;    int bvUgZfKruQ44900272 = -72066162;    int bvUgZfKruQ74678797 = -110664672;    int bvUgZfKruQ55615658 = -524436601;    int bvUgZfKruQ29406050 = -329996089;    int bvUgZfKruQ66743032 = -23983563;    int bvUgZfKruQ40193299 = 38584284;    int bvUgZfKruQ16700145 = -357643635;    int bvUgZfKruQ94423410 = -707586168;    int bvUgZfKruQ92441365 = -4825719;    int bvUgZfKruQ3854269 = -699845874;    int bvUgZfKruQ93723160 = -10442681;    int bvUgZfKruQ76785833 = -493992780;    int bvUgZfKruQ31027871 = -646213011;    int bvUgZfKruQ34840534 = -567202827;    int bvUgZfKruQ52709436 = -411174417;    int bvUgZfKruQ71542949 = -38538253;    int bvUgZfKruQ28209113 = -684451121;    int bvUgZfKruQ66456794 = -830801859;    int bvUgZfKruQ17700385 = -875352550;    int bvUgZfKruQ38429005 = -317039586;    int bvUgZfKruQ74621390 = -571407046;    int bvUgZfKruQ82490659 = -381106595;    int bvUgZfKruQ38592622 = -104933991;    int bvUgZfKruQ95742739 = -539188116;    int bvUgZfKruQ74541678 = -64621169;    int bvUgZfKruQ52338944 = -998273716;    int bvUgZfKruQ51591088 = -744077086;    int bvUgZfKruQ74876089 = -251758742;    int bvUgZfKruQ36842006 = -323468742;    int bvUgZfKruQ61915881 = -11182706;    int bvUgZfKruQ30100156 = -882097577;    int bvUgZfKruQ13321875 = 22279913;    int bvUgZfKruQ2120951 = -775467084;     bvUgZfKruQ71205942 = bvUgZfKruQ58384438;     bvUgZfKruQ58384438 = bvUgZfKruQ36863232;     bvUgZfKruQ36863232 = bvUgZfKruQ39719603;     bvUgZfKruQ39719603 = bvUgZfKruQ92654909;     bvUgZfKruQ92654909 = bvUgZfKruQ93716551;     bvUgZfKruQ93716551 = bvUgZfKruQ58973273;     bvUgZfKruQ58973273 = bvUgZfKruQ8450142;     bvUgZfKruQ8450142 = bvUgZfKruQ24509448;     bvUgZfKruQ24509448 = bvUgZfKruQ19877528;     bvUgZfKruQ19877528 = bvUgZfKruQ22457549;     bvUgZfKruQ22457549 = bvUgZfKruQ45574320;     bvUgZfKruQ45574320 = bvUgZfKruQ29873099;     bvUgZfKruQ29873099 = bvUgZfKruQ65632791;     bvUgZfKruQ65632791 = bvUgZfKruQ13113834;     bvUgZfKruQ13113834 = bvUgZfKruQ39756066;     bvUgZfKruQ39756066 = bvUgZfKruQ41574302;     bvUgZfKruQ41574302 = bvUgZfKruQ77019412;     bvUgZfKruQ77019412 = bvUgZfKruQ6023967;     bvUgZfKruQ6023967 = bvUgZfKruQ97450615;     bvUgZfKruQ97450615 = bvUgZfKruQ25288343;     bvUgZfKruQ25288343 = bvUgZfKruQ18844571;     bvUgZfKruQ18844571 = bvUgZfKruQ96629002;     bvUgZfKruQ96629002 = bvUgZfKruQ1054895;     bvUgZfKruQ1054895 = bvUgZfKruQ17227278;     bvUgZfKruQ17227278 = bvUgZfKruQ7419885;     bvUgZfKruQ7419885 = bvUgZfKruQ79268819;     bvUgZfKruQ79268819 = bvUgZfKruQ92788073;     bvUgZfKruQ92788073 = bvUgZfKruQ37519045;     bvUgZfKruQ37519045 = bvUgZfKruQ39619302;     bvUgZfKruQ39619302 = bvUgZfKruQ30293560;     bvUgZfKruQ30293560 = bvUgZfKruQ309439;     bvUgZfKruQ309439 = bvUgZfKruQ79786997;     bvUgZfKruQ79786997 = bvUgZfKruQ84972827;     bvUgZfKruQ84972827 = bvUgZfKruQ90953994;     bvUgZfKruQ90953994 = bvUgZfKruQ57498175;     bvUgZfKruQ57498175 = bvUgZfKruQ10350016;     bvUgZfKruQ10350016 = bvUgZfKruQ74831269;     bvUgZfKruQ74831269 = bvUgZfKruQ36826114;     bvUgZfKruQ36826114 = bvUgZfKruQ89323822;     bvUgZfKruQ89323822 = bvUgZfKruQ3027206;     bvUgZfKruQ3027206 = bvUgZfKruQ32846977;     bvUgZfKruQ32846977 = bvUgZfKruQ14990302;     bvUgZfKruQ14990302 = bvUgZfKruQ2905843;     bvUgZfKruQ2905843 = bvUgZfKruQ24269061;     bvUgZfKruQ24269061 = bvUgZfKruQ86199406;     bvUgZfKruQ86199406 = bvUgZfKruQ72579350;     bvUgZfKruQ72579350 = bvUgZfKruQ26559383;     bvUgZfKruQ26559383 = bvUgZfKruQ21245125;     bvUgZfKruQ21245125 = bvUgZfKruQ9309933;     bvUgZfKruQ9309933 = bvUgZfKruQ73162507;     bvUgZfKruQ73162507 = bvUgZfKruQ12593175;     bvUgZfKruQ12593175 = bvUgZfKruQ61880434;     bvUgZfKruQ61880434 = bvUgZfKruQ5165607;     bvUgZfKruQ5165607 = bvUgZfKruQ2482168;     bvUgZfKruQ2482168 = bvUgZfKruQ52361372;     bvUgZfKruQ52361372 = bvUgZfKruQ61755435;     bvUgZfKruQ61755435 = bvUgZfKruQ35808338;     bvUgZfKruQ35808338 = bvUgZfKruQ22492326;     bvUgZfKruQ22492326 = bvUgZfKruQ85235025;     bvUgZfKruQ85235025 = bvUgZfKruQ14447733;     bvUgZfKruQ14447733 = bvUgZfKruQ66185200;     bvUgZfKruQ66185200 = bvUgZfKruQ70931096;     bvUgZfKruQ70931096 = bvUgZfKruQ84890146;     bvUgZfKruQ84890146 = bvUgZfKruQ89583968;     bvUgZfKruQ89583968 = bvUgZfKruQ22148110;     bvUgZfKruQ22148110 = bvUgZfKruQ65787323;     bvUgZfKruQ65787323 = bvUgZfKruQ44900272;     bvUgZfKruQ44900272 = bvUgZfKruQ74678797;     bvUgZfKruQ74678797 = bvUgZfKruQ55615658;     bvUgZfKruQ55615658 = bvUgZfKruQ29406050;     bvUgZfKruQ29406050 = bvUgZfKruQ66743032;     bvUgZfKruQ66743032 = bvUgZfKruQ40193299;     bvUgZfKruQ40193299 = bvUgZfKruQ16700145;     bvUgZfKruQ16700145 = bvUgZfKruQ94423410;     bvUgZfKruQ94423410 = bvUgZfKruQ92441365;     bvUgZfKruQ92441365 = bvUgZfKruQ3854269;     bvUgZfKruQ3854269 = bvUgZfKruQ93723160;     bvUgZfKruQ93723160 = bvUgZfKruQ76785833;     bvUgZfKruQ76785833 = bvUgZfKruQ31027871;     bvUgZfKruQ31027871 = bvUgZfKruQ34840534;     bvUgZfKruQ34840534 = bvUgZfKruQ52709436;     bvUgZfKruQ52709436 = bvUgZfKruQ71542949;     bvUgZfKruQ71542949 = bvUgZfKruQ28209113;     bvUgZfKruQ28209113 = bvUgZfKruQ66456794;     bvUgZfKruQ66456794 = bvUgZfKruQ17700385;     bvUgZfKruQ17700385 = bvUgZfKruQ38429005;     bvUgZfKruQ38429005 = bvUgZfKruQ74621390;     bvUgZfKruQ74621390 = bvUgZfKruQ82490659;     bvUgZfKruQ82490659 = bvUgZfKruQ38592622;     bvUgZfKruQ38592622 = bvUgZfKruQ95742739;     bvUgZfKruQ95742739 = bvUgZfKruQ74541678;     bvUgZfKruQ74541678 = bvUgZfKruQ52338944;     bvUgZfKruQ52338944 = bvUgZfKruQ51591088;     bvUgZfKruQ51591088 = bvUgZfKruQ74876089;     bvUgZfKruQ74876089 = bvUgZfKruQ36842006;     bvUgZfKruQ36842006 = bvUgZfKruQ61915881;     bvUgZfKruQ61915881 = bvUgZfKruQ30100156;     bvUgZfKruQ30100156 = bvUgZfKruQ13321875;     bvUgZfKruQ13321875 = bvUgZfKruQ2120951;     bvUgZfKruQ2120951 = bvUgZfKruQ71205942;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void zSDCdEDBsX64415485() {     int LpMaGhFXnJ93593060 = -409871047;    int LpMaGhFXnJ42731939 = -644073191;    int LpMaGhFXnJ65632510 = -668967400;    int LpMaGhFXnJ54726292 = -284483402;    int LpMaGhFXnJ90829164 = -712114295;    int LpMaGhFXnJ8599669 = -789994154;    int LpMaGhFXnJ51129012 = -25588666;    int LpMaGhFXnJ84084000 = -973613463;    int LpMaGhFXnJ21672580 = -766031879;    int LpMaGhFXnJ11641112 = -632292503;    int LpMaGhFXnJ59999408 = -463046265;    int LpMaGhFXnJ1477270 = -502390270;    int LpMaGhFXnJ96881279 = -969469773;    int LpMaGhFXnJ8247544 = -521599761;    int LpMaGhFXnJ49849110 = -73138306;    int LpMaGhFXnJ84011978 = -702546281;    int LpMaGhFXnJ91397653 = -236215069;    int LpMaGhFXnJ82995829 = -981800873;    int LpMaGhFXnJ47305598 = -932335248;    int LpMaGhFXnJ75966899 = -461915034;    int LpMaGhFXnJ26171450 = -70577154;    int LpMaGhFXnJ82659789 = -501941055;    int LpMaGhFXnJ99922325 = -489574852;    int LpMaGhFXnJ27554682 = -67044932;    int LpMaGhFXnJ52767524 = 37042976;    int LpMaGhFXnJ95722878 = -840386137;    int LpMaGhFXnJ26358240 = -599141522;    int LpMaGhFXnJ98699524 = -913830142;    int LpMaGhFXnJ44411606 = -884192516;    int LpMaGhFXnJ48776481 = -286900416;    int LpMaGhFXnJ74584364 = -745535829;    int LpMaGhFXnJ57850630 = -840776109;    int LpMaGhFXnJ60825667 = -644929344;    int LpMaGhFXnJ44361514 = -502813142;    int LpMaGhFXnJ67183358 = -946982756;    int LpMaGhFXnJ99191044 = -136619369;    int LpMaGhFXnJ3789729 = -723830602;    int LpMaGhFXnJ28457371 = -802577158;    int LpMaGhFXnJ85134245 = -869688499;    int LpMaGhFXnJ9509537 = -475813518;    int LpMaGhFXnJ31784576 = 95127920;    int LpMaGhFXnJ21355369 = -832172818;    int LpMaGhFXnJ20540494 = -184859451;    int LpMaGhFXnJ2349962 = -200415229;    int LpMaGhFXnJ92252327 = -918202773;    int LpMaGhFXnJ94299688 = -877413984;    int LpMaGhFXnJ84876748 = -307699340;    int LpMaGhFXnJ71158057 = -86915604;    int LpMaGhFXnJ15705030 = -936554134;    int LpMaGhFXnJ6393678 = -997504536;    int LpMaGhFXnJ82031723 = -890878967;    int LpMaGhFXnJ86444432 = -119686410;    int LpMaGhFXnJ40194837 = -543576399;    int LpMaGhFXnJ65452152 = -819859613;    int LpMaGhFXnJ38244035 = -938283812;    int LpMaGhFXnJ10933271 = -907929992;    int LpMaGhFXnJ42809613 = -54498340;    int LpMaGhFXnJ38077829 = -501922468;    int LpMaGhFXnJ1958768 = -221526378;    int LpMaGhFXnJ95106285 = -871728158;    int LpMaGhFXnJ82241429 = -90852632;    int LpMaGhFXnJ52429488 = -111758525;    int LpMaGhFXnJ39672394 = 10579053;    int LpMaGhFXnJ72896098 = -379131463;    int LpMaGhFXnJ37056748 = -886756674;    int LpMaGhFXnJ2148779 = -622270156;    int LpMaGhFXnJ40651602 = -857460926;    int LpMaGhFXnJ52519765 = -366656631;    int LpMaGhFXnJ41064185 = -574617006;    int LpMaGhFXnJ50658066 = -936518937;    int LpMaGhFXnJ80222250 = -978715679;    int LpMaGhFXnJ62940283 = -433637911;    int LpMaGhFXnJ97861583 = -12112375;    int LpMaGhFXnJ37796061 = -356521731;    int LpMaGhFXnJ44182323 = -457042955;    int LpMaGhFXnJ4816081 = -238404336;    int LpMaGhFXnJ62119295 = -217081604;    int LpMaGhFXnJ97572364 = -189159624;    int LpMaGhFXnJ35302354 = -148842160;    int LpMaGhFXnJ58467836 = -85543040;    int LpMaGhFXnJ10846131 = -432686798;    int LpMaGhFXnJ55200182 = -412225919;    int LpMaGhFXnJ82994494 = -977276009;    int LpMaGhFXnJ38017928 = -886687981;    int LpMaGhFXnJ66744757 = -396021449;    int LpMaGhFXnJ88139932 = -525849420;    int LpMaGhFXnJ17655793 = -197199711;    int LpMaGhFXnJ95373515 = -825069732;    int LpMaGhFXnJ6117479 = -564529330;    int LpMaGhFXnJ56250088 = 60947236;    int LpMaGhFXnJ56381432 = 17878970;    int LpMaGhFXnJ65711899 = -121908134;    int LpMaGhFXnJ26498604 = -481050781;    int LpMaGhFXnJ90027960 = -997960341;    int LpMaGhFXnJ27268108 = -284960886;    int LpMaGhFXnJ79355087 = -793113555;    int LpMaGhFXnJ81682974 = -742751872;    int LpMaGhFXnJ47644396 = -805727988;    int LpMaGhFXnJ65293214 = -313658555;    int LpMaGhFXnJ90103548 = -409871047;     LpMaGhFXnJ93593060 = LpMaGhFXnJ42731939;     LpMaGhFXnJ42731939 = LpMaGhFXnJ65632510;     LpMaGhFXnJ65632510 = LpMaGhFXnJ54726292;     LpMaGhFXnJ54726292 = LpMaGhFXnJ90829164;     LpMaGhFXnJ90829164 = LpMaGhFXnJ8599669;     LpMaGhFXnJ8599669 = LpMaGhFXnJ51129012;     LpMaGhFXnJ51129012 = LpMaGhFXnJ84084000;     LpMaGhFXnJ84084000 = LpMaGhFXnJ21672580;     LpMaGhFXnJ21672580 = LpMaGhFXnJ11641112;     LpMaGhFXnJ11641112 = LpMaGhFXnJ59999408;     LpMaGhFXnJ59999408 = LpMaGhFXnJ1477270;     LpMaGhFXnJ1477270 = LpMaGhFXnJ96881279;     LpMaGhFXnJ96881279 = LpMaGhFXnJ8247544;     LpMaGhFXnJ8247544 = LpMaGhFXnJ49849110;     LpMaGhFXnJ49849110 = LpMaGhFXnJ84011978;     LpMaGhFXnJ84011978 = LpMaGhFXnJ91397653;     LpMaGhFXnJ91397653 = LpMaGhFXnJ82995829;     LpMaGhFXnJ82995829 = LpMaGhFXnJ47305598;     LpMaGhFXnJ47305598 = LpMaGhFXnJ75966899;     LpMaGhFXnJ75966899 = LpMaGhFXnJ26171450;     LpMaGhFXnJ26171450 = LpMaGhFXnJ82659789;     LpMaGhFXnJ82659789 = LpMaGhFXnJ99922325;     LpMaGhFXnJ99922325 = LpMaGhFXnJ27554682;     LpMaGhFXnJ27554682 = LpMaGhFXnJ52767524;     LpMaGhFXnJ52767524 = LpMaGhFXnJ95722878;     LpMaGhFXnJ95722878 = LpMaGhFXnJ26358240;     LpMaGhFXnJ26358240 = LpMaGhFXnJ98699524;     LpMaGhFXnJ98699524 = LpMaGhFXnJ44411606;     LpMaGhFXnJ44411606 = LpMaGhFXnJ48776481;     LpMaGhFXnJ48776481 = LpMaGhFXnJ74584364;     LpMaGhFXnJ74584364 = LpMaGhFXnJ57850630;     LpMaGhFXnJ57850630 = LpMaGhFXnJ60825667;     LpMaGhFXnJ60825667 = LpMaGhFXnJ44361514;     LpMaGhFXnJ44361514 = LpMaGhFXnJ67183358;     LpMaGhFXnJ67183358 = LpMaGhFXnJ99191044;     LpMaGhFXnJ99191044 = LpMaGhFXnJ3789729;     LpMaGhFXnJ3789729 = LpMaGhFXnJ28457371;     LpMaGhFXnJ28457371 = LpMaGhFXnJ85134245;     LpMaGhFXnJ85134245 = LpMaGhFXnJ9509537;     LpMaGhFXnJ9509537 = LpMaGhFXnJ31784576;     LpMaGhFXnJ31784576 = LpMaGhFXnJ21355369;     LpMaGhFXnJ21355369 = LpMaGhFXnJ20540494;     LpMaGhFXnJ20540494 = LpMaGhFXnJ2349962;     LpMaGhFXnJ2349962 = LpMaGhFXnJ92252327;     LpMaGhFXnJ92252327 = LpMaGhFXnJ94299688;     LpMaGhFXnJ94299688 = LpMaGhFXnJ84876748;     LpMaGhFXnJ84876748 = LpMaGhFXnJ71158057;     LpMaGhFXnJ71158057 = LpMaGhFXnJ15705030;     LpMaGhFXnJ15705030 = LpMaGhFXnJ6393678;     LpMaGhFXnJ6393678 = LpMaGhFXnJ82031723;     LpMaGhFXnJ82031723 = LpMaGhFXnJ86444432;     LpMaGhFXnJ86444432 = LpMaGhFXnJ40194837;     LpMaGhFXnJ40194837 = LpMaGhFXnJ65452152;     LpMaGhFXnJ65452152 = LpMaGhFXnJ38244035;     LpMaGhFXnJ38244035 = LpMaGhFXnJ10933271;     LpMaGhFXnJ10933271 = LpMaGhFXnJ42809613;     LpMaGhFXnJ42809613 = LpMaGhFXnJ38077829;     LpMaGhFXnJ38077829 = LpMaGhFXnJ1958768;     LpMaGhFXnJ1958768 = LpMaGhFXnJ95106285;     LpMaGhFXnJ95106285 = LpMaGhFXnJ82241429;     LpMaGhFXnJ82241429 = LpMaGhFXnJ52429488;     LpMaGhFXnJ52429488 = LpMaGhFXnJ39672394;     LpMaGhFXnJ39672394 = LpMaGhFXnJ72896098;     LpMaGhFXnJ72896098 = LpMaGhFXnJ37056748;     LpMaGhFXnJ37056748 = LpMaGhFXnJ2148779;     LpMaGhFXnJ2148779 = LpMaGhFXnJ40651602;     LpMaGhFXnJ40651602 = LpMaGhFXnJ52519765;     LpMaGhFXnJ52519765 = LpMaGhFXnJ41064185;     LpMaGhFXnJ41064185 = LpMaGhFXnJ50658066;     LpMaGhFXnJ50658066 = LpMaGhFXnJ80222250;     LpMaGhFXnJ80222250 = LpMaGhFXnJ62940283;     LpMaGhFXnJ62940283 = LpMaGhFXnJ97861583;     LpMaGhFXnJ97861583 = LpMaGhFXnJ37796061;     LpMaGhFXnJ37796061 = LpMaGhFXnJ44182323;     LpMaGhFXnJ44182323 = LpMaGhFXnJ4816081;     LpMaGhFXnJ4816081 = LpMaGhFXnJ62119295;     LpMaGhFXnJ62119295 = LpMaGhFXnJ97572364;     LpMaGhFXnJ97572364 = LpMaGhFXnJ35302354;     LpMaGhFXnJ35302354 = LpMaGhFXnJ58467836;     LpMaGhFXnJ58467836 = LpMaGhFXnJ10846131;     LpMaGhFXnJ10846131 = LpMaGhFXnJ55200182;     LpMaGhFXnJ55200182 = LpMaGhFXnJ82994494;     LpMaGhFXnJ82994494 = LpMaGhFXnJ38017928;     LpMaGhFXnJ38017928 = LpMaGhFXnJ66744757;     LpMaGhFXnJ66744757 = LpMaGhFXnJ88139932;     LpMaGhFXnJ88139932 = LpMaGhFXnJ17655793;     LpMaGhFXnJ17655793 = LpMaGhFXnJ95373515;     LpMaGhFXnJ95373515 = LpMaGhFXnJ6117479;     LpMaGhFXnJ6117479 = LpMaGhFXnJ56250088;     LpMaGhFXnJ56250088 = LpMaGhFXnJ56381432;     LpMaGhFXnJ56381432 = LpMaGhFXnJ65711899;     LpMaGhFXnJ65711899 = LpMaGhFXnJ26498604;     LpMaGhFXnJ26498604 = LpMaGhFXnJ90027960;     LpMaGhFXnJ90027960 = LpMaGhFXnJ27268108;     LpMaGhFXnJ27268108 = LpMaGhFXnJ79355087;     LpMaGhFXnJ79355087 = LpMaGhFXnJ81682974;     LpMaGhFXnJ81682974 = LpMaGhFXnJ47644396;     LpMaGhFXnJ47644396 = LpMaGhFXnJ65293214;     LpMaGhFXnJ65293214 = LpMaGhFXnJ90103548;     LpMaGhFXnJ90103548 = LpMaGhFXnJ93593060;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void OuhasNsOsT1060460() {     int NPHwJfOtYY15980178 = -44275010;    int NPHwJfOtYY27079439 = -592616246;    int NPHwJfOtYY94401789 = -908621244;    int NPHwJfOtYY69732980 = -290890933;    int NPHwJfOtYY89003418 = -203171296;    int NPHwJfOtYY23482786 = -650602532;    int NPHwJfOtYY43284751 = -947575972;    int NPHwJfOtYY59717859 = -344210107;    int NPHwJfOtYY18835711 = -650227918;    int NPHwJfOtYY3404697 = -375033836;    int NPHwJfOtYY97541268 = -459670665;    int NPHwJfOtYY57380218 = -753135563;    int NPHwJfOtYY63889460 = -488187092;    int NPHwJfOtYY50862296 = 50691482;    int NPHwJfOtYY86584387 = -800616019;    int NPHwJfOtYY28267891 = 15786177;    int NPHwJfOtYY41221005 = -513494889;    int NPHwJfOtYY88972245 = -333543564;    int NPHwJfOtYY88587228 = 32562626;    int NPHwJfOtYY54483182 = -714549488;    int NPHwJfOtYY27054557 = -62064463;    int NPHwJfOtYY46475007 = 93292632;    int NPHwJfOtYY3215650 = -665655444;    int NPHwJfOtYY54054468 = -631037789;    int NPHwJfOtYY88307771 = -684516149;    int NPHwJfOtYY84025873 = -184280361;    int NPHwJfOtYY73447660 = -356728015;    int NPHwJfOtYY4610975 = -702284595;    int NPHwJfOtYY51304167 = -128449636;    int NPHwJfOtYY57933660 = -207196282;    int NPHwJfOtYY18875170 = -826851981;    int NPHwJfOtYY15391821 = -392847464;    int NPHwJfOtYY41864338 = -74272839;    int NPHwJfOtYY3750202 = -826939992;    int NPHwJfOtYY43412723 = 89260822;    int NPHwJfOtYY40883914 = -452014745;    int NPHwJfOtYY97229440 = -456778553;    int NPHwJfOtYY82083471 = -670202630;    int NPHwJfOtYY33442377 = -170734530;    int NPHwJfOtYY29695252 = -612037547;    int NPHwJfOtYY60541947 = -408049746;    int NPHwJfOtYY9863760 = -590081509;    int NPHwJfOtYY26090686 = -72390035;    int NPHwJfOtYY1794081 = -197778878;    int NPHwJfOtYY60235594 = -727346250;    int NPHwJfOtYY2399971 = 40356920;    int NPHwJfOtYY97174145 = -886109593;    int NPHwJfOtYY15756732 = -943450595;    int NPHwJfOtYY10164935 = -886270832;    int NPHwJfOtYY3477424 = -39524795;    int NPHwJfOtYY90900940 = -145955244;    int NPHwJfOtYY60295689 = -550505692;    int NPHwJfOtYY18509240 = -215487629;    int NPHwJfOtYY25738697 = 4459578;    int NPHwJfOtYY74005902 = 21012073;    int NPHwJfOtYY69505170 = -37567642;    int NPHwJfOtYY23863790 = -926960802;    int NPHwJfOtYY40347321 = -177583455;    int NPHwJfOtYY81425208 = -606374785;    int NPHwJfOtYY4977545 = 81109064;    int NPHwJfOtYY50035126 = -193874518;    int NPHwJfOtYY38673777 = -145291378;    int NPHwJfOtYY8413692 = -115760472;    int NPHwJfOtYY60902050 = -343031636;    int NPHwJfOtYY84529527 = -548181855;    int NPHwJfOtYY82149447 = 33176798;    int NPHwJfOtYY15515880 = -578862724;    int NPHwJfOtYY60139258 = -661247101;    int NPHwJfOtYY7449573 = 61430660;    int NPHwJfOtYY45700474 = -248601274;    int NPHwJfOtYY31038451 = -527435270;    int NPHwJfOtYY59137534 = -843292259;    int NPHwJfOtYY55529869 = -62809034;    int NPHwJfOtYY58891977 = -355399827;    int NPHwJfOtYY93941235 = -206499743;    int NPHwJfOtYY17190797 = -471982954;    int NPHwJfOtYY20384322 = -834317334;    int NPHwJfOtYY1421569 = -367876567;    int NPHwJfOtYY93818874 = -903691540;    int NPHwJfOtYY85907801 = -624873069;    int NPHwJfOtYY86851728 = -298170769;    int NPHwJfOtYY57690928 = -413277420;    int NPHwJfOtYY94446039 = -816013764;    int NPHwJfOtYY47826743 = 11075159;    int NPHwJfOtYY67032720 = 38758961;    int NPHwJfOtYY58579480 = -176346290;    int NPHwJfOtYY96882580 = -77359835;    int NPHwJfOtYY16125641 = 21267582;    int NPHwJfOtYY29744299 = -747952065;    int NPHwJfOtYY73907553 = -873171536;    int NPHwJfOtYY17020124 = -525053944;    int NPHwJfOtYY56882120 = -179195099;    int NPHwJfOtYY658263 = 36172155;    int NPHwJfOtYY28464832 = -151843595;    int NPHwJfOtYY79660125 = -318163030;    int NPHwJfOtYY21868170 = -162758369;    int NPHwJfOtYY1450068 = -374321038;    int NPHwJfOtYY65188636 = -729358400;    int NPHwJfOtYY17264554 = -649597023;    int NPHwJfOtYY78086146 = -44275010;     NPHwJfOtYY15980178 = NPHwJfOtYY27079439;     NPHwJfOtYY27079439 = NPHwJfOtYY94401789;     NPHwJfOtYY94401789 = NPHwJfOtYY69732980;     NPHwJfOtYY69732980 = NPHwJfOtYY89003418;     NPHwJfOtYY89003418 = NPHwJfOtYY23482786;     NPHwJfOtYY23482786 = NPHwJfOtYY43284751;     NPHwJfOtYY43284751 = NPHwJfOtYY59717859;     NPHwJfOtYY59717859 = NPHwJfOtYY18835711;     NPHwJfOtYY18835711 = NPHwJfOtYY3404697;     NPHwJfOtYY3404697 = NPHwJfOtYY97541268;     NPHwJfOtYY97541268 = NPHwJfOtYY57380218;     NPHwJfOtYY57380218 = NPHwJfOtYY63889460;     NPHwJfOtYY63889460 = NPHwJfOtYY50862296;     NPHwJfOtYY50862296 = NPHwJfOtYY86584387;     NPHwJfOtYY86584387 = NPHwJfOtYY28267891;     NPHwJfOtYY28267891 = NPHwJfOtYY41221005;     NPHwJfOtYY41221005 = NPHwJfOtYY88972245;     NPHwJfOtYY88972245 = NPHwJfOtYY88587228;     NPHwJfOtYY88587228 = NPHwJfOtYY54483182;     NPHwJfOtYY54483182 = NPHwJfOtYY27054557;     NPHwJfOtYY27054557 = NPHwJfOtYY46475007;     NPHwJfOtYY46475007 = NPHwJfOtYY3215650;     NPHwJfOtYY3215650 = NPHwJfOtYY54054468;     NPHwJfOtYY54054468 = NPHwJfOtYY88307771;     NPHwJfOtYY88307771 = NPHwJfOtYY84025873;     NPHwJfOtYY84025873 = NPHwJfOtYY73447660;     NPHwJfOtYY73447660 = NPHwJfOtYY4610975;     NPHwJfOtYY4610975 = NPHwJfOtYY51304167;     NPHwJfOtYY51304167 = NPHwJfOtYY57933660;     NPHwJfOtYY57933660 = NPHwJfOtYY18875170;     NPHwJfOtYY18875170 = NPHwJfOtYY15391821;     NPHwJfOtYY15391821 = NPHwJfOtYY41864338;     NPHwJfOtYY41864338 = NPHwJfOtYY3750202;     NPHwJfOtYY3750202 = NPHwJfOtYY43412723;     NPHwJfOtYY43412723 = NPHwJfOtYY40883914;     NPHwJfOtYY40883914 = NPHwJfOtYY97229440;     NPHwJfOtYY97229440 = NPHwJfOtYY82083471;     NPHwJfOtYY82083471 = NPHwJfOtYY33442377;     NPHwJfOtYY33442377 = NPHwJfOtYY29695252;     NPHwJfOtYY29695252 = NPHwJfOtYY60541947;     NPHwJfOtYY60541947 = NPHwJfOtYY9863760;     NPHwJfOtYY9863760 = NPHwJfOtYY26090686;     NPHwJfOtYY26090686 = NPHwJfOtYY1794081;     NPHwJfOtYY1794081 = NPHwJfOtYY60235594;     NPHwJfOtYY60235594 = NPHwJfOtYY2399971;     NPHwJfOtYY2399971 = NPHwJfOtYY97174145;     NPHwJfOtYY97174145 = NPHwJfOtYY15756732;     NPHwJfOtYY15756732 = NPHwJfOtYY10164935;     NPHwJfOtYY10164935 = NPHwJfOtYY3477424;     NPHwJfOtYY3477424 = NPHwJfOtYY90900940;     NPHwJfOtYY90900940 = NPHwJfOtYY60295689;     NPHwJfOtYY60295689 = NPHwJfOtYY18509240;     NPHwJfOtYY18509240 = NPHwJfOtYY25738697;     NPHwJfOtYY25738697 = NPHwJfOtYY74005902;     NPHwJfOtYY74005902 = NPHwJfOtYY69505170;     NPHwJfOtYY69505170 = NPHwJfOtYY23863790;     NPHwJfOtYY23863790 = NPHwJfOtYY40347321;     NPHwJfOtYY40347321 = NPHwJfOtYY81425208;     NPHwJfOtYY81425208 = NPHwJfOtYY4977545;     NPHwJfOtYY4977545 = NPHwJfOtYY50035126;     NPHwJfOtYY50035126 = NPHwJfOtYY38673777;     NPHwJfOtYY38673777 = NPHwJfOtYY8413692;     NPHwJfOtYY8413692 = NPHwJfOtYY60902050;     NPHwJfOtYY60902050 = NPHwJfOtYY84529527;     NPHwJfOtYY84529527 = NPHwJfOtYY82149447;     NPHwJfOtYY82149447 = NPHwJfOtYY15515880;     NPHwJfOtYY15515880 = NPHwJfOtYY60139258;     NPHwJfOtYY60139258 = NPHwJfOtYY7449573;     NPHwJfOtYY7449573 = NPHwJfOtYY45700474;     NPHwJfOtYY45700474 = NPHwJfOtYY31038451;     NPHwJfOtYY31038451 = NPHwJfOtYY59137534;     NPHwJfOtYY59137534 = NPHwJfOtYY55529869;     NPHwJfOtYY55529869 = NPHwJfOtYY58891977;     NPHwJfOtYY58891977 = NPHwJfOtYY93941235;     NPHwJfOtYY93941235 = NPHwJfOtYY17190797;     NPHwJfOtYY17190797 = NPHwJfOtYY20384322;     NPHwJfOtYY20384322 = NPHwJfOtYY1421569;     NPHwJfOtYY1421569 = NPHwJfOtYY93818874;     NPHwJfOtYY93818874 = NPHwJfOtYY85907801;     NPHwJfOtYY85907801 = NPHwJfOtYY86851728;     NPHwJfOtYY86851728 = NPHwJfOtYY57690928;     NPHwJfOtYY57690928 = NPHwJfOtYY94446039;     NPHwJfOtYY94446039 = NPHwJfOtYY47826743;     NPHwJfOtYY47826743 = NPHwJfOtYY67032720;     NPHwJfOtYY67032720 = NPHwJfOtYY58579480;     NPHwJfOtYY58579480 = NPHwJfOtYY96882580;     NPHwJfOtYY96882580 = NPHwJfOtYY16125641;     NPHwJfOtYY16125641 = NPHwJfOtYY29744299;     NPHwJfOtYY29744299 = NPHwJfOtYY73907553;     NPHwJfOtYY73907553 = NPHwJfOtYY17020124;     NPHwJfOtYY17020124 = NPHwJfOtYY56882120;     NPHwJfOtYY56882120 = NPHwJfOtYY658263;     NPHwJfOtYY658263 = NPHwJfOtYY28464832;     NPHwJfOtYY28464832 = NPHwJfOtYY79660125;     NPHwJfOtYY79660125 = NPHwJfOtYY21868170;     NPHwJfOtYY21868170 = NPHwJfOtYY1450068;     NPHwJfOtYY1450068 = NPHwJfOtYY65188636;     NPHwJfOtYY65188636 = NPHwJfOtYY17264554;     NPHwJfOtYY17264554 = NPHwJfOtYY78086146;     NPHwJfOtYY78086146 = NPHwJfOtYY15980178;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void iUatykNNmv37705434() {     int oQVNsstciI38367295 = -778678973;    int oQVNsstciI11426940 = -541159301;    int oQVNsstciI23171068 = -48275088;    int oQVNsstciI84739668 = -297298465;    int oQVNsstciI87177672 = -794228297;    int oQVNsstciI38365902 = -511210910;    int oQVNsstciI35440490 = -769563278;    int oQVNsstciI35351718 = -814806751;    int oQVNsstciI15998842 = -534423957;    int oQVNsstciI95168281 = -117775169;    int oQVNsstciI35083128 = -456295066;    int oQVNsstciI13283167 = 96119144;    int oQVNsstciI30897641 = -6904412;    int oQVNsstciI93477047 = -477017275;    int oQVNsstciI23319665 = -428093732;    int oQVNsstciI72523803 = -365881365;    int oQVNsstciI91044357 = -790774709;    int oQVNsstciI94948661 = -785286254;    int oQVNsstciI29868860 = -102539499;    int oQVNsstciI32999466 = -967183942;    int oQVNsstciI27937664 = -53551771;    int oQVNsstciI10290226 = -411473681;    int oQVNsstciI6508973 = -841736036;    int oQVNsstciI80554255 = -95030647;    int oQVNsstciI23848018 = -306075273;    int oQVNsstciI72328868 = -628174585;    int oQVNsstciI20537080 = -114314508;    int oQVNsstciI10522425 = -490739049;    int oQVNsstciI58196728 = -472706755;    int oQVNsstciI67090840 = -127492149;    int oQVNsstciI63165975 = -908168133;    int oQVNsstciI72933011 = 55081181;    int oQVNsstciI22903009 = -603616334;    int oQVNsstciI63138889 = -51066842;    int oQVNsstciI19642088 = 25504399;    int oQVNsstciI82576782 = -767410122;    int oQVNsstciI90669152 = -189726505;    int oQVNsstciI35709572 = -537828102;    int oQVNsstciI81750508 = -571780562;    int oQVNsstciI49880966 = -748261577;    int oQVNsstciI89299318 = -911227412;    int oQVNsstciI98372151 = -347990200;    int oQVNsstciI31640878 = 40079381;    int oQVNsstciI1238200 = -195142527;    int oQVNsstciI28218861 = -536489728;    int oQVNsstciI10500252 = -141872175;    int oQVNsstciI9471543 = -364519846;    int oQVNsstciI60355407 = -699985586;    int oQVNsstciI4624840 = -835987530;    int oQVNsstciI561170 = -181545054;    int oQVNsstciI99770156 = -501031521;    int oQVNsstciI34146947 = -981324974;    int oQVNsstciI96823643 = -987398859;    int oQVNsstciI86025242 = -271221230;    int oQVNsstciI9767771 = -119692043;    int oQVNsstciI28077069 = -267205292;    int oQVNsstciI4917968 = -699423265;    int oQVNsstciI42616812 = -953244442;    int oQVNsstciI60891650 = -991223192;    int oQVNsstciI14848805 = -66053713;    int oQVNsstciI17828822 = -296896403;    int oQVNsstciI24918066 = -178824230;    int oQVNsstciI77154990 = -242099997;    int oQVNsstciI48908001 = -306931808;    int oQVNsstciI32002307 = -209607036;    int oQVNsstciI62150116 = -411376248;    int oQVNsstciI90380158 = -300264523;    int oQVNsstciI67758752 = -955837571;    int oQVNsstciI73834960 = -402521675;    int oQVNsstciI40742882 = -660683610;    int oQVNsstciI81854651 = -76154861;    int oQVNsstciI55334785 = -152946607;    int oQVNsstciI13198154 = -113505693;    int oQVNsstciI79987893 = -354277923;    int oQVNsstciI43700148 = 44043470;    int oQVNsstciI29565513 = -705561571;    int oQVNsstciI78649347 = -351553063;    int oQVNsstciI5270773 = -546593510;    int oQVNsstciI52335395 = -558540919;    int oQVNsstciI13347767 = -64203099;    int oQVNsstciI62857325 = -163654740;    int oQVNsstciI60181673 = -414328922;    int oQVNsstciI5897585 = -654751519;    int oQVNsstciI57635558 = -191161701;    int oQVNsstciI67320683 = -626460628;    int oQVNsstciI29019028 = -926843160;    int oQVNsstciI76109368 = 42480040;    int oQVNsstciI36877766 = -232395104;    int oQVNsstciI53371119 = -931374800;    int oQVNsstciI91565018 = -707290309;    int oQVNsstciI77658815 = 32013143;    int oQVNsstciI48052340 = -236482063;    int oQVNsstciI74817922 = -546604910;    int oQVNsstciI66901703 = -405726849;    int oQVNsstciI32052144 = -351365174;    int oQVNsstciI64381252 = -632403182;    int oQVNsstciI21217161 = -5890204;    int oQVNsstciI82732876 = -652988811;    int oQVNsstciI69235893 = -985535491;    int oQVNsstciI66068744 = -778678973;     oQVNsstciI38367295 = oQVNsstciI11426940;     oQVNsstciI11426940 = oQVNsstciI23171068;     oQVNsstciI23171068 = oQVNsstciI84739668;     oQVNsstciI84739668 = oQVNsstciI87177672;     oQVNsstciI87177672 = oQVNsstciI38365902;     oQVNsstciI38365902 = oQVNsstciI35440490;     oQVNsstciI35440490 = oQVNsstciI35351718;     oQVNsstciI35351718 = oQVNsstciI15998842;     oQVNsstciI15998842 = oQVNsstciI95168281;     oQVNsstciI95168281 = oQVNsstciI35083128;     oQVNsstciI35083128 = oQVNsstciI13283167;     oQVNsstciI13283167 = oQVNsstciI30897641;     oQVNsstciI30897641 = oQVNsstciI93477047;     oQVNsstciI93477047 = oQVNsstciI23319665;     oQVNsstciI23319665 = oQVNsstciI72523803;     oQVNsstciI72523803 = oQVNsstciI91044357;     oQVNsstciI91044357 = oQVNsstciI94948661;     oQVNsstciI94948661 = oQVNsstciI29868860;     oQVNsstciI29868860 = oQVNsstciI32999466;     oQVNsstciI32999466 = oQVNsstciI27937664;     oQVNsstciI27937664 = oQVNsstciI10290226;     oQVNsstciI10290226 = oQVNsstciI6508973;     oQVNsstciI6508973 = oQVNsstciI80554255;     oQVNsstciI80554255 = oQVNsstciI23848018;     oQVNsstciI23848018 = oQVNsstciI72328868;     oQVNsstciI72328868 = oQVNsstciI20537080;     oQVNsstciI20537080 = oQVNsstciI10522425;     oQVNsstciI10522425 = oQVNsstciI58196728;     oQVNsstciI58196728 = oQVNsstciI67090840;     oQVNsstciI67090840 = oQVNsstciI63165975;     oQVNsstciI63165975 = oQVNsstciI72933011;     oQVNsstciI72933011 = oQVNsstciI22903009;     oQVNsstciI22903009 = oQVNsstciI63138889;     oQVNsstciI63138889 = oQVNsstciI19642088;     oQVNsstciI19642088 = oQVNsstciI82576782;     oQVNsstciI82576782 = oQVNsstciI90669152;     oQVNsstciI90669152 = oQVNsstciI35709572;     oQVNsstciI35709572 = oQVNsstciI81750508;     oQVNsstciI81750508 = oQVNsstciI49880966;     oQVNsstciI49880966 = oQVNsstciI89299318;     oQVNsstciI89299318 = oQVNsstciI98372151;     oQVNsstciI98372151 = oQVNsstciI31640878;     oQVNsstciI31640878 = oQVNsstciI1238200;     oQVNsstciI1238200 = oQVNsstciI28218861;     oQVNsstciI28218861 = oQVNsstciI10500252;     oQVNsstciI10500252 = oQVNsstciI9471543;     oQVNsstciI9471543 = oQVNsstciI60355407;     oQVNsstciI60355407 = oQVNsstciI4624840;     oQVNsstciI4624840 = oQVNsstciI561170;     oQVNsstciI561170 = oQVNsstciI99770156;     oQVNsstciI99770156 = oQVNsstciI34146947;     oQVNsstciI34146947 = oQVNsstciI96823643;     oQVNsstciI96823643 = oQVNsstciI86025242;     oQVNsstciI86025242 = oQVNsstciI9767771;     oQVNsstciI9767771 = oQVNsstciI28077069;     oQVNsstciI28077069 = oQVNsstciI4917968;     oQVNsstciI4917968 = oQVNsstciI42616812;     oQVNsstciI42616812 = oQVNsstciI60891650;     oQVNsstciI60891650 = oQVNsstciI14848805;     oQVNsstciI14848805 = oQVNsstciI17828822;     oQVNsstciI17828822 = oQVNsstciI24918066;     oQVNsstciI24918066 = oQVNsstciI77154990;     oQVNsstciI77154990 = oQVNsstciI48908001;     oQVNsstciI48908001 = oQVNsstciI32002307;     oQVNsstciI32002307 = oQVNsstciI62150116;     oQVNsstciI62150116 = oQVNsstciI90380158;     oQVNsstciI90380158 = oQVNsstciI67758752;     oQVNsstciI67758752 = oQVNsstciI73834960;     oQVNsstciI73834960 = oQVNsstciI40742882;     oQVNsstciI40742882 = oQVNsstciI81854651;     oQVNsstciI81854651 = oQVNsstciI55334785;     oQVNsstciI55334785 = oQVNsstciI13198154;     oQVNsstciI13198154 = oQVNsstciI79987893;     oQVNsstciI79987893 = oQVNsstciI43700148;     oQVNsstciI43700148 = oQVNsstciI29565513;     oQVNsstciI29565513 = oQVNsstciI78649347;     oQVNsstciI78649347 = oQVNsstciI5270773;     oQVNsstciI5270773 = oQVNsstciI52335395;     oQVNsstciI52335395 = oQVNsstciI13347767;     oQVNsstciI13347767 = oQVNsstciI62857325;     oQVNsstciI62857325 = oQVNsstciI60181673;     oQVNsstciI60181673 = oQVNsstciI5897585;     oQVNsstciI5897585 = oQVNsstciI57635558;     oQVNsstciI57635558 = oQVNsstciI67320683;     oQVNsstciI67320683 = oQVNsstciI29019028;     oQVNsstciI29019028 = oQVNsstciI76109368;     oQVNsstciI76109368 = oQVNsstciI36877766;     oQVNsstciI36877766 = oQVNsstciI53371119;     oQVNsstciI53371119 = oQVNsstciI91565018;     oQVNsstciI91565018 = oQVNsstciI77658815;     oQVNsstciI77658815 = oQVNsstciI48052340;     oQVNsstciI48052340 = oQVNsstciI74817922;     oQVNsstciI74817922 = oQVNsstciI66901703;     oQVNsstciI66901703 = oQVNsstciI32052144;     oQVNsstciI32052144 = oQVNsstciI64381252;     oQVNsstciI64381252 = oQVNsstciI21217161;     oQVNsstciI21217161 = oQVNsstciI82732876;     oQVNsstciI82732876 = oQVNsstciI69235893;     oQVNsstciI69235893 = oQVNsstciI66068744;     oQVNsstciI66068744 = oQVNsstciI38367295;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void gCTxqLdlkm74350408() {     int gMimtMvyZc60754412 = -413082936;    int gMimtMvyZc95774439 = -489702355;    int gMimtMvyZc51940347 = -287928932;    int gMimtMvyZc99746356 = -303705996;    int gMimtMvyZc85351926 = -285285299;    int gMimtMvyZc53249019 = -371819288;    int gMimtMvyZc27596229 = -591550585;    int gMimtMvyZc10985577 = -185403395;    int gMimtMvyZc13161973 = -418619996;    int gMimtMvyZc86931865 = -960516502;    int gMimtMvyZc72624987 = -452919467;    int gMimtMvyZc69186116 = -154626150;    int gMimtMvyZc97905821 = -625621732;    int gMimtMvyZc36091800 = 95273968;    int gMimtMvyZc60054941 = -55571445;    int gMimtMvyZc16779716 = -747548908;    int gMimtMvyZc40867709 = 31945472;    int gMimtMvyZc925078 = -137028945;    int gMimtMvyZc71150490 = -237641624;    int gMimtMvyZc11515749 = -119818395;    int gMimtMvyZc28820771 = -45039079;    int gMimtMvyZc74105444 = -916239995;    int gMimtMvyZc9802296 = 82183372;    int gMimtMvyZc7054043 = -659023504;    int gMimtMvyZc59388265 = 72365602;    int gMimtMvyZc60631862 = 27931191;    int gMimtMvyZc67626500 = -971901000;    int gMimtMvyZc16433875 = -279193502;    int gMimtMvyZc65089288 = -816963874;    int gMimtMvyZc76248019 = -47788015;    int gMimtMvyZc7456780 = -989484285;    int gMimtMvyZc30474203 = -596990173;    int gMimtMvyZc3941679 = -32959829;    int gMimtMvyZc22527576 = -375193692;    int gMimtMvyZc95871452 = -38252023;    int gMimtMvyZc24269652 = 17194502;    int gMimtMvyZc84108864 = 77325544;    int gMimtMvyZc89335673 = -405453574;    int gMimtMvyZc30058639 = -972826594;    int gMimtMvyZc70066680 = -884485606;    int gMimtMvyZc18056689 = -314405078;    int gMimtMvyZc86880542 = -105898891;    int gMimtMvyZc37191070 = -947451202;    int gMimtMvyZc682320 = -192506176;    int gMimtMvyZc96202127 = -345633206;    int gMimtMvyZc18600534 = -324101270;    int gMimtMvyZc21768940 = -942930098;    int gMimtMvyZc4954082 = -456520576;    int gMimtMvyZc99084744 = -785704229;    int gMimtMvyZc97644914 = -323565313;    int gMimtMvyZc8639374 = -856107798;    int gMimtMvyZc7998204 = -312144256;    int gMimtMvyZc75138046 = -659310089;    int gMimtMvyZc46311788 = -546902039;    int gMimtMvyZc45529638 = -260396158;    int gMimtMvyZc86648968 = -496842942;    int gMimtMvyZc85972144 = -471885727;    int gMimtMvyZc44886304 = -628905429;    int gMimtMvyZc40358092 = -276071599;    int gMimtMvyZc24720064 = -213216491;    int gMimtMvyZc85622518 = -399918288;    int gMimtMvyZc11162355 = -212357083;    int gMimtMvyZc45896288 = -368439522;    int gMimtMvyZc36913953 = -270831981;    int gMimtMvyZc79475086 = -971032217;    int gMimtMvyZc42150785 = -855929294;    int gMimtMvyZc65244437 = -21666321;    int gMimtMvyZc75378245 = -150428040;    int gMimtMvyZc40220348 = -866474009;    int gMimtMvyZc35785290 = 27234053;    int gMimtMvyZc32670851 = -724874452;    int gMimtMvyZc51532036 = -562600955;    int gMimtMvyZc70866438 = -164202352;    int gMimtMvyZc1083810 = -353156019;    int gMimtMvyZc93459060 = -805413318;    int gMimtMvyZc41940229 = -939140189;    int gMimtMvyZc36914374 = -968788793;    int gMimtMvyZc9119977 = -725310453;    int gMimtMvyZc10851916 = -213390299;    int gMimtMvyZc40787731 = -603533128;    int gMimtMvyZc38862923 = -29138711;    int gMimtMvyZc62672419 = -415380424;    int gMimtMvyZc17349130 = -493489274;    int gMimtMvyZc67444373 = -393398561;    int gMimtMvyZc67608646 = -191680218;    int gMimtMvyZc99458576 = -577340030;    int gMimtMvyZc55336156 = -937680084;    int gMimtMvyZc57629891 = -486057790;    int gMimtMvyZc76997938 = -14797535;    int gMimtMvyZc9222485 = -541409082;    int gMimtMvyZc38297507 = -510919771;    int gMimtMvyZc39222561 = -293769028;    int gMimtMvyZc48977581 = -29381975;    int gMimtMvyZc5338576 = -659610104;    int gMimtMvyZc84444161 = -384567319;    int gMimtMvyZc6894334 = -2047996;    int gMimtMvyZc40984255 = -737459370;    int gMimtMvyZc277118 = -576619222;    int gMimtMvyZc21207234 = -221473959;    int gMimtMvyZc54051342 = -413082936;     gMimtMvyZc60754412 = gMimtMvyZc95774439;     gMimtMvyZc95774439 = gMimtMvyZc51940347;     gMimtMvyZc51940347 = gMimtMvyZc99746356;     gMimtMvyZc99746356 = gMimtMvyZc85351926;     gMimtMvyZc85351926 = gMimtMvyZc53249019;     gMimtMvyZc53249019 = gMimtMvyZc27596229;     gMimtMvyZc27596229 = gMimtMvyZc10985577;     gMimtMvyZc10985577 = gMimtMvyZc13161973;     gMimtMvyZc13161973 = gMimtMvyZc86931865;     gMimtMvyZc86931865 = gMimtMvyZc72624987;     gMimtMvyZc72624987 = gMimtMvyZc69186116;     gMimtMvyZc69186116 = gMimtMvyZc97905821;     gMimtMvyZc97905821 = gMimtMvyZc36091800;     gMimtMvyZc36091800 = gMimtMvyZc60054941;     gMimtMvyZc60054941 = gMimtMvyZc16779716;     gMimtMvyZc16779716 = gMimtMvyZc40867709;     gMimtMvyZc40867709 = gMimtMvyZc925078;     gMimtMvyZc925078 = gMimtMvyZc71150490;     gMimtMvyZc71150490 = gMimtMvyZc11515749;     gMimtMvyZc11515749 = gMimtMvyZc28820771;     gMimtMvyZc28820771 = gMimtMvyZc74105444;     gMimtMvyZc74105444 = gMimtMvyZc9802296;     gMimtMvyZc9802296 = gMimtMvyZc7054043;     gMimtMvyZc7054043 = gMimtMvyZc59388265;     gMimtMvyZc59388265 = gMimtMvyZc60631862;     gMimtMvyZc60631862 = gMimtMvyZc67626500;     gMimtMvyZc67626500 = gMimtMvyZc16433875;     gMimtMvyZc16433875 = gMimtMvyZc65089288;     gMimtMvyZc65089288 = gMimtMvyZc76248019;     gMimtMvyZc76248019 = gMimtMvyZc7456780;     gMimtMvyZc7456780 = gMimtMvyZc30474203;     gMimtMvyZc30474203 = gMimtMvyZc3941679;     gMimtMvyZc3941679 = gMimtMvyZc22527576;     gMimtMvyZc22527576 = gMimtMvyZc95871452;     gMimtMvyZc95871452 = gMimtMvyZc24269652;     gMimtMvyZc24269652 = gMimtMvyZc84108864;     gMimtMvyZc84108864 = gMimtMvyZc89335673;     gMimtMvyZc89335673 = gMimtMvyZc30058639;     gMimtMvyZc30058639 = gMimtMvyZc70066680;     gMimtMvyZc70066680 = gMimtMvyZc18056689;     gMimtMvyZc18056689 = gMimtMvyZc86880542;     gMimtMvyZc86880542 = gMimtMvyZc37191070;     gMimtMvyZc37191070 = gMimtMvyZc682320;     gMimtMvyZc682320 = gMimtMvyZc96202127;     gMimtMvyZc96202127 = gMimtMvyZc18600534;     gMimtMvyZc18600534 = gMimtMvyZc21768940;     gMimtMvyZc21768940 = gMimtMvyZc4954082;     gMimtMvyZc4954082 = gMimtMvyZc99084744;     gMimtMvyZc99084744 = gMimtMvyZc97644914;     gMimtMvyZc97644914 = gMimtMvyZc8639374;     gMimtMvyZc8639374 = gMimtMvyZc7998204;     gMimtMvyZc7998204 = gMimtMvyZc75138046;     gMimtMvyZc75138046 = gMimtMvyZc46311788;     gMimtMvyZc46311788 = gMimtMvyZc45529638;     gMimtMvyZc45529638 = gMimtMvyZc86648968;     gMimtMvyZc86648968 = gMimtMvyZc85972144;     gMimtMvyZc85972144 = gMimtMvyZc44886304;     gMimtMvyZc44886304 = gMimtMvyZc40358092;     gMimtMvyZc40358092 = gMimtMvyZc24720064;     gMimtMvyZc24720064 = gMimtMvyZc85622518;     gMimtMvyZc85622518 = gMimtMvyZc11162355;     gMimtMvyZc11162355 = gMimtMvyZc45896288;     gMimtMvyZc45896288 = gMimtMvyZc36913953;     gMimtMvyZc36913953 = gMimtMvyZc79475086;     gMimtMvyZc79475086 = gMimtMvyZc42150785;     gMimtMvyZc42150785 = gMimtMvyZc65244437;     gMimtMvyZc65244437 = gMimtMvyZc75378245;     gMimtMvyZc75378245 = gMimtMvyZc40220348;     gMimtMvyZc40220348 = gMimtMvyZc35785290;     gMimtMvyZc35785290 = gMimtMvyZc32670851;     gMimtMvyZc32670851 = gMimtMvyZc51532036;     gMimtMvyZc51532036 = gMimtMvyZc70866438;     gMimtMvyZc70866438 = gMimtMvyZc1083810;     gMimtMvyZc1083810 = gMimtMvyZc93459060;     gMimtMvyZc93459060 = gMimtMvyZc41940229;     gMimtMvyZc41940229 = gMimtMvyZc36914374;     gMimtMvyZc36914374 = gMimtMvyZc9119977;     gMimtMvyZc9119977 = gMimtMvyZc10851916;     gMimtMvyZc10851916 = gMimtMvyZc40787731;     gMimtMvyZc40787731 = gMimtMvyZc38862923;     gMimtMvyZc38862923 = gMimtMvyZc62672419;     gMimtMvyZc62672419 = gMimtMvyZc17349130;     gMimtMvyZc17349130 = gMimtMvyZc67444373;     gMimtMvyZc67444373 = gMimtMvyZc67608646;     gMimtMvyZc67608646 = gMimtMvyZc99458576;     gMimtMvyZc99458576 = gMimtMvyZc55336156;     gMimtMvyZc55336156 = gMimtMvyZc57629891;     gMimtMvyZc57629891 = gMimtMvyZc76997938;     gMimtMvyZc76997938 = gMimtMvyZc9222485;     gMimtMvyZc9222485 = gMimtMvyZc38297507;     gMimtMvyZc38297507 = gMimtMvyZc39222561;     gMimtMvyZc39222561 = gMimtMvyZc48977581;     gMimtMvyZc48977581 = gMimtMvyZc5338576;     gMimtMvyZc5338576 = gMimtMvyZc84444161;     gMimtMvyZc84444161 = gMimtMvyZc6894334;     gMimtMvyZc6894334 = gMimtMvyZc40984255;     gMimtMvyZc40984255 = gMimtMvyZc277118;     gMimtMvyZc277118 = gMimtMvyZc21207234;     gMimtMvyZc21207234 = gMimtMvyZc54051342;     gMimtMvyZc54051342 = gMimtMvyZc60754412;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void MXviUCrVwW10995384() {     int FToGifbfWy83141529 = -47486899;    int FToGifbfWy80121940 = -438245410;    int FToGifbfWy80709625 = -527582776;    int FToGifbfWy14753046 = -310113528;    int FToGifbfWy83526180 = -876342300;    int FToGifbfWy68132136 = -232427665;    int FToGifbfWy19751968 = -413537891;    int FToGifbfWy86619434 = -656000040;    int FToGifbfWy10325104 = -302816035;    int FToGifbfWy78695450 = -703257835;    int FToGifbfWy10166848 = -449543868;    int FToGifbfWy25089065 = -405371443;    int FToGifbfWy64914002 = -144339051;    int FToGifbfWy78706552 = -432434789;    int FToGifbfWy96790218 = -783049158;    int FToGifbfWy61035628 = -29216450;    int FToGifbfWy90691060 = -245334348;    int FToGifbfWy6901495 = -588771636;    int FToGifbfWy12432121 = -372743749;    int FToGifbfWy90032032 = -372452849;    int FToGifbfWy29703878 = -36526388;    int FToGifbfWy37920663 = -321006308;    int FToGifbfWy13095619 = -93897221;    int FToGifbfWy33553830 = -123016361;    int FToGifbfWy94928511 = -649193522;    int FToGifbfWy48934857 = -415963032;    int FToGifbfWy14715921 = -729487493;    int FToGifbfWy22345325 = -67647956;    int FToGifbfWy71981849 = -61220993;    int FToGifbfWy85405199 = 31916118;    int FToGifbfWy51747585 = 29199563;    int FToGifbfWy88015393 = -149061528;    int FToGifbfWy84980349 = -562303324;    int FToGifbfWy81916263 = -699320542;    int FToGifbfWy72100817 = -102008446;    int FToGifbfWy65962521 = -298200875;    int FToGifbfWy77548577 = -755622407;    int FToGifbfWy42961774 = -273079046;    int FToGifbfWy78366770 = -273872625;    int FToGifbfWy90252395 = 79290365;    int FToGifbfWy46814060 = -817582744;    int FToGifbfWy75388933 = -963807582;    int FToGifbfWy42741262 = -834981786;    int FToGifbfWy126439 = -189869825;    int FToGifbfWy64185394 = -154776683;    int FToGifbfWy26700816 = -506330366;    int FToGifbfWy34066338 = -421340351;    int FToGifbfWy49552756 = -213055567;    int FToGifbfWy93544649 = -735420927;    int FToGifbfWy94728660 = -465585572;    int FToGifbfWy17508590 = -111184075;    int FToGifbfWy81849461 = -742963538;    int FToGifbfWy53452450 = -331221320;    int FToGifbfWy6598333 = -822582848;    int FToGifbfWy81291505 = -401100273;    int FToGifbfWy45220867 = -726480592;    int FToGifbfWy67026322 = -244348190;    int FToGifbfWy47155795 = -304566416;    int FToGifbfWy19824534 = -660920006;    int FToGifbfWy34591324 = -360379268;    int FToGifbfWy53416215 = -502940173;    int FToGifbfWy97406643 = -245889935;    int FToGifbfWy14637586 = -494779047;    int FToGifbfWy24919905 = -234732154;    int FToGifbfWy26947866 = -632457398;    int FToGifbfWy22151454 = -200482340;    int FToGifbfWy40108716 = -843068120;    int FToGifbfWy82997738 = -445018510;    int FToGifbfWy6605736 = -230426344;    int FToGifbfWy30827698 = -384848284;    int FToGifbfWy83487051 = -273594043;    int FToGifbfWy47729287 = -972255303;    int FToGifbfWy28534724 = -214899011;    int FToGifbfWy22179726 = -352034115;    int FToGifbfWy43217972 = -554870105;    int FToGifbfWy54314945 = -72718806;    int FToGifbfWy95179400 = -486024522;    int FToGifbfWy12969180 = -904027396;    int FToGifbfWy69368436 = -968239678;    int FToGifbfWy68227696 = -42863157;    int FToGifbfWy14868520 = -994622682;    int FToGifbfWy65163165 = -416431926;    int FToGifbfWy28800675 = -332227029;    int FToGifbfWy77253188 = -595635422;    int FToGifbfWy67896609 = -856899807;    int FToGifbfWy69898124 = -227836900;    int FToGifbfWy34562944 = -817840209;    int FToGifbfWy78382016 = -739720476;    int FToGifbfWy624759 = -198220270;    int FToGifbfWy26879950 = -375527854;    int FToGifbfWy98936199 = 46147315;    int FToGifbfWy30392782 = -351055992;    int FToGifbfWy23137241 = -612159040;    int FToGifbfWy43775447 = -913493358;    int FToGifbfWy36836180 = -417769463;    int FToGifbfWy49407416 = -471692809;    int FToGifbfWy60751348 = -369028536;    int FToGifbfWy17821358 = -500249633;    int FToGifbfWy73178573 = -557412427;    int FToGifbfWy42033940 = -47486899;     FToGifbfWy83141529 = FToGifbfWy80121940;     FToGifbfWy80121940 = FToGifbfWy80709625;     FToGifbfWy80709625 = FToGifbfWy14753046;     FToGifbfWy14753046 = FToGifbfWy83526180;     FToGifbfWy83526180 = FToGifbfWy68132136;     FToGifbfWy68132136 = FToGifbfWy19751968;     FToGifbfWy19751968 = FToGifbfWy86619434;     FToGifbfWy86619434 = FToGifbfWy10325104;     FToGifbfWy10325104 = FToGifbfWy78695450;     FToGifbfWy78695450 = FToGifbfWy10166848;     FToGifbfWy10166848 = FToGifbfWy25089065;     FToGifbfWy25089065 = FToGifbfWy64914002;     FToGifbfWy64914002 = FToGifbfWy78706552;     FToGifbfWy78706552 = FToGifbfWy96790218;     FToGifbfWy96790218 = FToGifbfWy61035628;     FToGifbfWy61035628 = FToGifbfWy90691060;     FToGifbfWy90691060 = FToGifbfWy6901495;     FToGifbfWy6901495 = FToGifbfWy12432121;     FToGifbfWy12432121 = FToGifbfWy90032032;     FToGifbfWy90032032 = FToGifbfWy29703878;     FToGifbfWy29703878 = FToGifbfWy37920663;     FToGifbfWy37920663 = FToGifbfWy13095619;     FToGifbfWy13095619 = FToGifbfWy33553830;     FToGifbfWy33553830 = FToGifbfWy94928511;     FToGifbfWy94928511 = FToGifbfWy48934857;     FToGifbfWy48934857 = FToGifbfWy14715921;     FToGifbfWy14715921 = FToGifbfWy22345325;     FToGifbfWy22345325 = FToGifbfWy71981849;     FToGifbfWy71981849 = FToGifbfWy85405199;     FToGifbfWy85405199 = FToGifbfWy51747585;     FToGifbfWy51747585 = FToGifbfWy88015393;     FToGifbfWy88015393 = FToGifbfWy84980349;     FToGifbfWy84980349 = FToGifbfWy81916263;     FToGifbfWy81916263 = FToGifbfWy72100817;     FToGifbfWy72100817 = FToGifbfWy65962521;     FToGifbfWy65962521 = FToGifbfWy77548577;     FToGifbfWy77548577 = FToGifbfWy42961774;     FToGifbfWy42961774 = FToGifbfWy78366770;     FToGifbfWy78366770 = FToGifbfWy90252395;     FToGifbfWy90252395 = FToGifbfWy46814060;     FToGifbfWy46814060 = FToGifbfWy75388933;     FToGifbfWy75388933 = FToGifbfWy42741262;     FToGifbfWy42741262 = FToGifbfWy126439;     FToGifbfWy126439 = FToGifbfWy64185394;     FToGifbfWy64185394 = FToGifbfWy26700816;     FToGifbfWy26700816 = FToGifbfWy34066338;     FToGifbfWy34066338 = FToGifbfWy49552756;     FToGifbfWy49552756 = FToGifbfWy93544649;     FToGifbfWy93544649 = FToGifbfWy94728660;     FToGifbfWy94728660 = FToGifbfWy17508590;     FToGifbfWy17508590 = FToGifbfWy81849461;     FToGifbfWy81849461 = FToGifbfWy53452450;     FToGifbfWy53452450 = FToGifbfWy6598333;     FToGifbfWy6598333 = FToGifbfWy81291505;     FToGifbfWy81291505 = FToGifbfWy45220867;     FToGifbfWy45220867 = FToGifbfWy67026322;     FToGifbfWy67026322 = FToGifbfWy47155795;     FToGifbfWy47155795 = FToGifbfWy19824534;     FToGifbfWy19824534 = FToGifbfWy34591324;     FToGifbfWy34591324 = FToGifbfWy53416215;     FToGifbfWy53416215 = FToGifbfWy97406643;     FToGifbfWy97406643 = FToGifbfWy14637586;     FToGifbfWy14637586 = FToGifbfWy24919905;     FToGifbfWy24919905 = FToGifbfWy26947866;     FToGifbfWy26947866 = FToGifbfWy22151454;     FToGifbfWy22151454 = FToGifbfWy40108716;     FToGifbfWy40108716 = FToGifbfWy82997738;     FToGifbfWy82997738 = FToGifbfWy6605736;     FToGifbfWy6605736 = FToGifbfWy30827698;     FToGifbfWy30827698 = FToGifbfWy83487051;     FToGifbfWy83487051 = FToGifbfWy47729287;     FToGifbfWy47729287 = FToGifbfWy28534724;     FToGifbfWy28534724 = FToGifbfWy22179726;     FToGifbfWy22179726 = FToGifbfWy43217972;     FToGifbfWy43217972 = FToGifbfWy54314945;     FToGifbfWy54314945 = FToGifbfWy95179400;     FToGifbfWy95179400 = FToGifbfWy12969180;     FToGifbfWy12969180 = FToGifbfWy69368436;     FToGifbfWy69368436 = FToGifbfWy68227696;     FToGifbfWy68227696 = FToGifbfWy14868520;     FToGifbfWy14868520 = FToGifbfWy65163165;     FToGifbfWy65163165 = FToGifbfWy28800675;     FToGifbfWy28800675 = FToGifbfWy77253188;     FToGifbfWy77253188 = FToGifbfWy67896609;     FToGifbfWy67896609 = FToGifbfWy69898124;     FToGifbfWy69898124 = FToGifbfWy34562944;     FToGifbfWy34562944 = FToGifbfWy78382016;     FToGifbfWy78382016 = FToGifbfWy624759;     FToGifbfWy624759 = FToGifbfWy26879950;     FToGifbfWy26879950 = FToGifbfWy98936199;     FToGifbfWy98936199 = FToGifbfWy30392782;     FToGifbfWy30392782 = FToGifbfWy23137241;     FToGifbfWy23137241 = FToGifbfWy43775447;     FToGifbfWy43775447 = FToGifbfWy36836180;     FToGifbfWy36836180 = FToGifbfWy49407416;     FToGifbfWy49407416 = FToGifbfWy60751348;     FToGifbfWy60751348 = FToGifbfWy17821358;     FToGifbfWy17821358 = FToGifbfWy73178573;     FToGifbfWy73178573 = FToGifbfWy42033940;     FToGifbfWy42033940 = FToGifbfWy83141529;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void wEiePpuojD38259443() {     int WHOdJoJezb63387675 = -37523442;    int WHOdJoJezb84944386 = -130692876;    int WHOdJoJezb73008235 = -758300312;    int WHOdJoJezb46830056 = -502998002;    int WHOdJoJezb18479754 = -127344505;    int WHOdJoJezb64510377 = -817126693;    int WHOdJoJezb77626180 = -493165303;    int WHOdJoJezb61590612 = -733072834;    int WHOdJoJezb40688643 = -32247380;    int WHOdJoJezb83846304 = -579813756;    int WHOdJoJezb82791090 = -325856255;    int WHOdJoJezb8879757 = -200026076;    int WHOdJoJezb73459180 = -790469247;    int WHOdJoJezb9388480 = -773909609;    int WHOdJoJezb38111071 = -616003895;    int WHOdJoJezb83959659 = -862771841;    int WHOdJoJezb34468181 = -587639401;    int WHOdJoJezb72613452 = -771541842;    int WHOdJoJezb54689173 = -823546888;    int WHOdJoJezb96754291 = -80233919;    int WHOdJoJezb85218107 = -255842946;    int WHOdJoJezb12380093 = -397191462;    int WHOdJoJezb16450852 = -265774905;    int WHOdJoJezb93046133 = -899985635;    int WHOdJoJezb17141578 = -162909263;    int WHOdJoJezb3090864 = -75916430;    int WHOdJoJezb28872173 = -549345194;    int WHOdJoJezb4089600 = -796395061;    int WHOdJoJezb7078292 = -31689404;    int WHOdJoJezb57073229 = -492320038;    int WHOdJoJezb92087069 = 88773241;    int WHOdJoJezb71473079 = -442532129;    int WHOdJoJezb88370854 = -264697859;    int WHOdJoJezb29876390 = -558904591;    int WHOdJoJezb25651924 = -761993692;    int WHOdJoJezb15252724 = -984025441;    int WHOdJoJezb89957082 = -986907594;    int WHOdJoJezb61594020 = -460213;    int WHOdJoJezb34746401 = -401478133;    int WHOdJoJezb51149571 = -439556867;    int WHOdJoJezb62069074 = -317249194;    int WHOdJoJezb70375521 = -786586091;    int WHOdJoJezb22658305 = -144963231;    int WHOdJoJezb20682219 = -575000646;    int WHOdJoJezb32132281 = -853055997;    int WHOdJoJezb8006772 = -185622847;    int WHOdJoJezb22610711 = 414027;    int WHOdJoJezb5709255 = -875129430;    int WHOdJoJezb68040346 = -71063481;    int WHOdJoJezb73334895 = -966603133;    int WHOdJoJezb89499007 = -438503172;    int WHOdJoJezb83942933 = 46037048;    int WHOdJoJezb27805708 = -746981317;    int WHOdJoJezb22662728 = -501613263;    int WHOdJoJezb53041336 = -415200094;    int WHOdJoJezb51007583 = -640331981;    int WHOdJoJezb68493535 = -864917971;    int WHOdJoJezb79962102 = -858314678;    int WHOdJoJezb29688478 = -240088739;    int WHOdJoJezb15388890 = 48571925;    int WHOdJoJezb35638205 = -167781500;    int WHOdJoJezb73536580 = -696770243;    int WHOdJoJezb54512320 = -601383431;    int WHOdJoJezb83615414 = -539927343;    int WHOdJoJezb91759235 = -568586998;    int WHOdJoJezb11318012 = -883324127;    int WHOdJoJezb20508903 = -935328217;    int WHOdJoJezb43582790 = -131564657;    int WHOdJoJezb83736556 = 88084082;    int WHOdJoJezb22858347 = -631978455;    int WHOdJoJezb94002577 = -875864247;    int WHOdJoJezb72874161 = -487179189;    int WHOdJoJezb37867051 = -270063710;    int WHOdJoJezb3539603 = -283990021;    int WHOdJoJezb34685217 = -762984726;    int WHOdJoJezb14842587 = -469256856;    int WHOdJoJezb89721788 = -152228231;    int WHOdJoJezb95768632 = -690774259;    int WHOdJoJezb60913852 = 53070362;    int WHOdJoJezb9134807 = -977286416;    int WHOdJoJezb80480153 = 23669542;    int WHOdJoJezb23162918 = -674215764;    int WHOdJoJezb36049253 = -625331580;    int WHOdJoJezb33743397 = -65086271;    int WHOdJoJezb67574222 = 46183133;    int WHOdJoJezb8144137 = -957263808;    int WHOdJoJezb43667371 = -695550812;    int WHOdJoJezb65708126 = -763084597;    int WHOdJoJezb76835054 = -43704498;    int WHOdJoJezb74644341 = -21661712;    int WHOdJoJezb46759189 = -19107470;    int WHOdJoJezb9994980 = -28592917;    int WHOdJoJezb31905542 = -760371474;    int WHOdJoJezb19357512 = -350050058;    int WHOdJoJezb15511366 = -171775368;    int WHOdJoJezb88532493 = -620478951;    int WHOdJoJezb15863201 = -85202661;    int WHOdJoJezb39042891 = -605035889;    int WHOdJoJezb28922984 = 93586351;    int WHOdJoJezb20814269 = -37523442;     WHOdJoJezb63387675 = WHOdJoJezb84944386;     WHOdJoJezb84944386 = WHOdJoJezb73008235;     WHOdJoJezb73008235 = WHOdJoJezb46830056;     WHOdJoJezb46830056 = WHOdJoJezb18479754;     WHOdJoJezb18479754 = WHOdJoJezb64510377;     WHOdJoJezb64510377 = WHOdJoJezb77626180;     WHOdJoJezb77626180 = WHOdJoJezb61590612;     WHOdJoJezb61590612 = WHOdJoJezb40688643;     WHOdJoJezb40688643 = WHOdJoJezb83846304;     WHOdJoJezb83846304 = WHOdJoJezb82791090;     WHOdJoJezb82791090 = WHOdJoJezb8879757;     WHOdJoJezb8879757 = WHOdJoJezb73459180;     WHOdJoJezb73459180 = WHOdJoJezb9388480;     WHOdJoJezb9388480 = WHOdJoJezb38111071;     WHOdJoJezb38111071 = WHOdJoJezb83959659;     WHOdJoJezb83959659 = WHOdJoJezb34468181;     WHOdJoJezb34468181 = WHOdJoJezb72613452;     WHOdJoJezb72613452 = WHOdJoJezb54689173;     WHOdJoJezb54689173 = WHOdJoJezb96754291;     WHOdJoJezb96754291 = WHOdJoJezb85218107;     WHOdJoJezb85218107 = WHOdJoJezb12380093;     WHOdJoJezb12380093 = WHOdJoJezb16450852;     WHOdJoJezb16450852 = WHOdJoJezb93046133;     WHOdJoJezb93046133 = WHOdJoJezb17141578;     WHOdJoJezb17141578 = WHOdJoJezb3090864;     WHOdJoJezb3090864 = WHOdJoJezb28872173;     WHOdJoJezb28872173 = WHOdJoJezb4089600;     WHOdJoJezb4089600 = WHOdJoJezb7078292;     WHOdJoJezb7078292 = WHOdJoJezb57073229;     WHOdJoJezb57073229 = WHOdJoJezb92087069;     WHOdJoJezb92087069 = WHOdJoJezb71473079;     WHOdJoJezb71473079 = WHOdJoJezb88370854;     WHOdJoJezb88370854 = WHOdJoJezb29876390;     WHOdJoJezb29876390 = WHOdJoJezb25651924;     WHOdJoJezb25651924 = WHOdJoJezb15252724;     WHOdJoJezb15252724 = WHOdJoJezb89957082;     WHOdJoJezb89957082 = WHOdJoJezb61594020;     WHOdJoJezb61594020 = WHOdJoJezb34746401;     WHOdJoJezb34746401 = WHOdJoJezb51149571;     WHOdJoJezb51149571 = WHOdJoJezb62069074;     WHOdJoJezb62069074 = WHOdJoJezb70375521;     WHOdJoJezb70375521 = WHOdJoJezb22658305;     WHOdJoJezb22658305 = WHOdJoJezb20682219;     WHOdJoJezb20682219 = WHOdJoJezb32132281;     WHOdJoJezb32132281 = WHOdJoJezb8006772;     WHOdJoJezb8006772 = WHOdJoJezb22610711;     WHOdJoJezb22610711 = WHOdJoJezb5709255;     WHOdJoJezb5709255 = WHOdJoJezb68040346;     WHOdJoJezb68040346 = WHOdJoJezb73334895;     WHOdJoJezb73334895 = WHOdJoJezb89499007;     WHOdJoJezb89499007 = WHOdJoJezb83942933;     WHOdJoJezb83942933 = WHOdJoJezb27805708;     WHOdJoJezb27805708 = WHOdJoJezb22662728;     WHOdJoJezb22662728 = WHOdJoJezb53041336;     WHOdJoJezb53041336 = WHOdJoJezb51007583;     WHOdJoJezb51007583 = WHOdJoJezb68493535;     WHOdJoJezb68493535 = WHOdJoJezb79962102;     WHOdJoJezb79962102 = WHOdJoJezb29688478;     WHOdJoJezb29688478 = WHOdJoJezb15388890;     WHOdJoJezb15388890 = WHOdJoJezb35638205;     WHOdJoJezb35638205 = WHOdJoJezb73536580;     WHOdJoJezb73536580 = WHOdJoJezb54512320;     WHOdJoJezb54512320 = WHOdJoJezb83615414;     WHOdJoJezb83615414 = WHOdJoJezb91759235;     WHOdJoJezb91759235 = WHOdJoJezb11318012;     WHOdJoJezb11318012 = WHOdJoJezb20508903;     WHOdJoJezb20508903 = WHOdJoJezb43582790;     WHOdJoJezb43582790 = WHOdJoJezb83736556;     WHOdJoJezb83736556 = WHOdJoJezb22858347;     WHOdJoJezb22858347 = WHOdJoJezb94002577;     WHOdJoJezb94002577 = WHOdJoJezb72874161;     WHOdJoJezb72874161 = WHOdJoJezb37867051;     WHOdJoJezb37867051 = WHOdJoJezb3539603;     WHOdJoJezb3539603 = WHOdJoJezb34685217;     WHOdJoJezb34685217 = WHOdJoJezb14842587;     WHOdJoJezb14842587 = WHOdJoJezb89721788;     WHOdJoJezb89721788 = WHOdJoJezb95768632;     WHOdJoJezb95768632 = WHOdJoJezb60913852;     WHOdJoJezb60913852 = WHOdJoJezb9134807;     WHOdJoJezb9134807 = WHOdJoJezb80480153;     WHOdJoJezb80480153 = WHOdJoJezb23162918;     WHOdJoJezb23162918 = WHOdJoJezb36049253;     WHOdJoJezb36049253 = WHOdJoJezb33743397;     WHOdJoJezb33743397 = WHOdJoJezb67574222;     WHOdJoJezb67574222 = WHOdJoJezb8144137;     WHOdJoJezb8144137 = WHOdJoJezb43667371;     WHOdJoJezb43667371 = WHOdJoJezb65708126;     WHOdJoJezb65708126 = WHOdJoJezb76835054;     WHOdJoJezb76835054 = WHOdJoJezb74644341;     WHOdJoJezb74644341 = WHOdJoJezb46759189;     WHOdJoJezb46759189 = WHOdJoJezb9994980;     WHOdJoJezb9994980 = WHOdJoJezb31905542;     WHOdJoJezb31905542 = WHOdJoJezb19357512;     WHOdJoJezb19357512 = WHOdJoJezb15511366;     WHOdJoJezb15511366 = WHOdJoJezb88532493;     WHOdJoJezb88532493 = WHOdJoJezb15863201;     WHOdJoJezb15863201 = WHOdJoJezb39042891;     WHOdJoJezb39042891 = WHOdJoJezb28922984;     WHOdJoJezb28922984 = WHOdJoJezb20814269;     WHOdJoJezb20814269 = WHOdJoJezb63387675;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void OpbuyIhgPa74904417() {     int ffvxTKOQHG85774792 = -771927405;    int ffvxTKOQHG69291887 = -79235930;    int ffvxTKOQHG1777514 = -997954156;    int ffvxTKOQHG61836744 = -509405534;    int ffvxTKOQHG16654008 = -718401506;    int ffvxTKOQHG79393494 = -677735071;    int ffvxTKOQHG69781919 = -315152609;    int ffvxTKOQHG37224471 = -103669478;    int ffvxTKOQHG37851774 = 83556581;    int ffvxTKOQHG75609889 = -322555089;    int ffvxTKOQHG20332951 = -322480656;    int ffvxTKOQHG64782706 = -450771369;    int ffvxTKOQHG40467361 = -309186566;    int ffvxTKOQHG52003232 = -201618366;    int ffvxTKOQHG74846348 = -243481608;    int ffvxTKOQHG28215572 = -144439384;    int ffvxTKOQHG84291532 = -864919221;    int ffvxTKOQHG78589869 = -123284533;    int ffvxTKOQHG95970804 = -958649013;    int ffvxTKOQHG75270574 = -332868373;    int ffvxTKOQHG86101214 = -247330255;    int ffvxTKOQHG76195310 = -901957775;    int ffvxTKOQHG19744175 = -441855497;    int ffvxTKOQHG19545920 = -363978492;    int ffvxTKOQHG52681824 = -884468388;    int ffvxTKOQHG91393858 = -519810654;    int ffvxTKOQHG75961593 = -306931687;    int ffvxTKOQHG10001050 = -584849514;    int ffvxTKOQHG13970853 = -375946523;    int ffvxTKOQHG66230408 = -412615905;    int ffvxTKOQHG36377875 = 7457089;    int ffvxTKOQHG29014270 = 5396517;    int ffvxTKOQHG69409525 = -794041354;    int ffvxTKOQHG89265077 = -883031441;    int ffvxTKOQHG1881289 = -825750114;    int ffvxTKOQHG56945593 = -199420818;    int ffvxTKOQHG83396794 = -719855546;    int ffvxTKOQHG15220121 = -968085685;    int ffvxTKOQHG83054532 = -802524165;    int ffvxTKOQHG71335285 = -575780897;    int ffvxTKOQHG90826444 = -820426860;    int ffvxTKOQHG58883912 = -544494782;    int ffvxTKOQHG28208497 = -32493815;    int ffvxTKOQHG20126338 = -572364295;    int ffvxTKOQHG115548 = -662199475;    int ffvxTKOQHG16107054 = -367851943;    int ffvxTKOQHG34908108 = -577996226;    int ffvxTKOQHG50307929 = -631664421;    int ffvxTKOQHG62500252 = -20780180;    int ffvxTKOQHG70418641 = -8623392;    int ffvxTKOQHG98368223 = -793579449;    int ffvxTKOQHG57794190 = -384782234;    int ffvxTKOQHG6120112 = -418892548;    int ffvxTKOQHG82949273 = -777294071;    int ffvxTKOQHG88803203 = -555904209;    int ffvxTKOQHG9579482 = -869969630;    int ffvxTKOQHG49547712 = -637380434;    int ffvxTKOQHG82231593 = -533975665;    int ffvxTKOQHG9154920 = -624937147;    int ffvxTKOQHG25260150 = -98590853;    int ffvxTKOQHG3431902 = -270803385;    int ffvxTKOQHG59780869 = -730303095;    int ffvxTKOQHG23253618 = -727722956;    int ffvxTKOQHG71621365 = -503827515;    int ffvxTKOQHG39232015 = -230012179;    int ffvxTKOQHG91318680 = -227877173;    int ffvxTKOQHG95373181 = -656730016;    int ffvxTKOQHG51202284 = -426155126;    int ffvxTKOQHG50121944 = -375868252;    int ffvxTKOQHG17900755 = 55939209;    int ffvxTKOQHG44818778 = -424583838;    int ffvxTKOQHG69071411 = -896833537;    int ffvxTKOQHG95535336 = -320760369;    int ffvxTKOQHG24635519 = -282868117;    int ffvxTKOQHG84444129 = -512441514;    int ffvxTKOQHG27217302 = -702835473;    int ffvxTKOQHG47986814 = -769463961;    int ffvxTKOQHG99617836 = -869491202;    int ffvxTKOQHG19430373 = -701779018;    int ffvxTKOQHG36574771 = -416616446;    int ffvxTKOQHG56485750 = -941814429;    int ffvxTKOQHG25653664 = -675267266;    int ffvxTKOQHG47500798 = -464069335;    int ffvxTKOQHG43552212 = -267323132;    int ffvxTKOQHG67862185 = -619036456;    int ffvxTKOQHG78583684 = -607760678;    int ffvxTKOQHG22894159 = -575710936;    int ffvxTKOQHG86460251 = 83252717;    int ffvxTKOQHG461875 = -227127232;    int ffvxTKOQHG92301806 = -955780484;    int ffvxTKOQHG7397881 = -562040384;    int ffvxTKOQHG1165201 = -85879882;    int ffvxTKOQHG6065202 = -243148539;    int ffvxTKOQHG57794383 = -603933313;    int ffvxTKOQHG67903384 = -204977512;    int ffvxTKOQHG31045576 = 9876235;    int ffvxTKOQHG35630294 = -816771827;    int ffvxTKOQHG56587131 = -528666300;    int ffvxTKOQHG80894323 = -242352117;    int ffvxTKOQHG8796867 = -771927405;     ffvxTKOQHG85774792 = ffvxTKOQHG69291887;     ffvxTKOQHG69291887 = ffvxTKOQHG1777514;     ffvxTKOQHG1777514 = ffvxTKOQHG61836744;     ffvxTKOQHG61836744 = ffvxTKOQHG16654008;     ffvxTKOQHG16654008 = ffvxTKOQHG79393494;     ffvxTKOQHG79393494 = ffvxTKOQHG69781919;     ffvxTKOQHG69781919 = ffvxTKOQHG37224471;     ffvxTKOQHG37224471 = ffvxTKOQHG37851774;     ffvxTKOQHG37851774 = ffvxTKOQHG75609889;     ffvxTKOQHG75609889 = ffvxTKOQHG20332951;     ffvxTKOQHG20332951 = ffvxTKOQHG64782706;     ffvxTKOQHG64782706 = ffvxTKOQHG40467361;     ffvxTKOQHG40467361 = ffvxTKOQHG52003232;     ffvxTKOQHG52003232 = ffvxTKOQHG74846348;     ffvxTKOQHG74846348 = ffvxTKOQHG28215572;     ffvxTKOQHG28215572 = ffvxTKOQHG84291532;     ffvxTKOQHG84291532 = ffvxTKOQHG78589869;     ffvxTKOQHG78589869 = ffvxTKOQHG95970804;     ffvxTKOQHG95970804 = ffvxTKOQHG75270574;     ffvxTKOQHG75270574 = ffvxTKOQHG86101214;     ffvxTKOQHG86101214 = ffvxTKOQHG76195310;     ffvxTKOQHG76195310 = ffvxTKOQHG19744175;     ffvxTKOQHG19744175 = ffvxTKOQHG19545920;     ffvxTKOQHG19545920 = ffvxTKOQHG52681824;     ffvxTKOQHG52681824 = ffvxTKOQHG91393858;     ffvxTKOQHG91393858 = ffvxTKOQHG75961593;     ffvxTKOQHG75961593 = ffvxTKOQHG10001050;     ffvxTKOQHG10001050 = ffvxTKOQHG13970853;     ffvxTKOQHG13970853 = ffvxTKOQHG66230408;     ffvxTKOQHG66230408 = ffvxTKOQHG36377875;     ffvxTKOQHG36377875 = ffvxTKOQHG29014270;     ffvxTKOQHG29014270 = ffvxTKOQHG69409525;     ffvxTKOQHG69409525 = ffvxTKOQHG89265077;     ffvxTKOQHG89265077 = ffvxTKOQHG1881289;     ffvxTKOQHG1881289 = ffvxTKOQHG56945593;     ffvxTKOQHG56945593 = ffvxTKOQHG83396794;     ffvxTKOQHG83396794 = ffvxTKOQHG15220121;     ffvxTKOQHG15220121 = ffvxTKOQHG83054532;     ffvxTKOQHG83054532 = ffvxTKOQHG71335285;     ffvxTKOQHG71335285 = ffvxTKOQHG90826444;     ffvxTKOQHG90826444 = ffvxTKOQHG58883912;     ffvxTKOQHG58883912 = ffvxTKOQHG28208497;     ffvxTKOQHG28208497 = ffvxTKOQHG20126338;     ffvxTKOQHG20126338 = ffvxTKOQHG115548;     ffvxTKOQHG115548 = ffvxTKOQHG16107054;     ffvxTKOQHG16107054 = ffvxTKOQHG34908108;     ffvxTKOQHG34908108 = ffvxTKOQHG50307929;     ffvxTKOQHG50307929 = ffvxTKOQHG62500252;     ffvxTKOQHG62500252 = ffvxTKOQHG70418641;     ffvxTKOQHG70418641 = ffvxTKOQHG98368223;     ffvxTKOQHG98368223 = ffvxTKOQHG57794190;     ffvxTKOQHG57794190 = ffvxTKOQHG6120112;     ffvxTKOQHG6120112 = ffvxTKOQHG82949273;     ffvxTKOQHG82949273 = ffvxTKOQHG88803203;     ffvxTKOQHG88803203 = ffvxTKOQHG9579482;     ffvxTKOQHG9579482 = ffvxTKOQHG49547712;     ffvxTKOQHG49547712 = ffvxTKOQHG82231593;     ffvxTKOQHG82231593 = ffvxTKOQHG9154920;     ffvxTKOQHG9154920 = ffvxTKOQHG25260150;     ffvxTKOQHG25260150 = ffvxTKOQHG3431902;     ffvxTKOQHG3431902 = ffvxTKOQHG59780869;     ffvxTKOQHG59780869 = ffvxTKOQHG23253618;     ffvxTKOQHG23253618 = ffvxTKOQHG71621365;     ffvxTKOQHG71621365 = ffvxTKOQHG39232015;     ffvxTKOQHG39232015 = ffvxTKOQHG91318680;     ffvxTKOQHG91318680 = ffvxTKOQHG95373181;     ffvxTKOQHG95373181 = ffvxTKOQHG51202284;     ffvxTKOQHG51202284 = ffvxTKOQHG50121944;     ffvxTKOQHG50121944 = ffvxTKOQHG17900755;     ffvxTKOQHG17900755 = ffvxTKOQHG44818778;     ffvxTKOQHG44818778 = ffvxTKOQHG69071411;     ffvxTKOQHG69071411 = ffvxTKOQHG95535336;     ffvxTKOQHG95535336 = ffvxTKOQHG24635519;     ffvxTKOQHG24635519 = ffvxTKOQHG84444129;     ffvxTKOQHG84444129 = ffvxTKOQHG27217302;     ffvxTKOQHG27217302 = ffvxTKOQHG47986814;     ffvxTKOQHG47986814 = ffvxTKOQHG99617836;     ffvxTKOQHG99617836 = ffvxTKOQHG19430373;     ffvxTKOQHG19430373 = ffvxTKOQHG36574771;     ffvxTKOQHG36574771 = ffvxTKOQHG56485750;     ffvxTKOQHG56485750 = ffvxTKOQHG25653664;     ffvxTKOQHG25653664 = ffvxTKOQHG47500798;     ffvxTKOQHG47500798 = ffvxTKOQHG43552212;     ffvxTKOQHG43552212 = ffvxTKOQHG67862185;     ffvxTKOQHG67862185 = ffvxTKOQHG78583684;     ffvxTKOQHG78583684 = ffvxTKOQHG22894159;     ffvxTKOQHG22894159 = ffvxTKOQHG86460251;     ffvxTKOQHG86460251 = ffvxTKOQHG461875;     ffvxTKOQHG461875 = ffvxTKOQHG92301806;     ffvxTKOQHG92301806 = ffvxTKOQHG7397881;     ffvxTKOQHG7397881 = ffvxTKOQHG1165201;     ffvxTKOQHG1165201 = ffvxTKOQHG6065202;     ffvxTKOQHG6065202 = ffvxTKOQHG57794383;     ffvxTKOQHG57794383 = ffvxTKOQHG67903384;     ffvxTKOQHG67903384 = ffvxTKOQHG31045576;     ffvxTKOQHG31045576 = ffvxTKOQHG35630294;     ffvxTKOQHG35630294 = ffvxTKOQHG56587131;     ffvxTKOQHG56587131 = ffvxTKOQHG80894323;     ffvxTKOQHG80894323 = ffvxTKOQHG8796867;     ffvxTKOQHG8796867 = ffvxTKOQHG85774792;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void SDoZxOoxEZ11549392() {     int ptTZuSfjGa8161911 = -406331368;    int ptTZuSfjGa53639387 = -27778985;    int ptTZuSfjGa30546793 = -137608000;    int ptTZuSfjGa76843432 = -515813066;    int ptTZuSfjGa14828263 = -209458508;    int ptTZuSfjGa94276611 = -538343449;    int ptTZuSfjGa61937658 = -137139915;    int ptTZuSfjGa12858330 = -574266122;    int ptTZuSfjGa35014905 = -900639459;    int ptTZuSfjGa67373474 = -65296422;    int ptTZuSfjGa57874810 = -319105056;    int ptTZuSfjGa20685655 = -701516662;    int ptTZuSfjGa7475542 = -927903886;    int ptTZuSfjGa94617984 = -729327123;    int ptTZuSfjGa11581625 = -970959321;    int ptTZuSfjGa72471484 = -526106926;    int ptTZuSfjGa34114884 = -42199041;    int ptTZuSfjGa84566285 = -575027223;    int ptTZuSfjGa37252435 = 6248862;    int ptTZuSfjGa53786858 = -585502827;    int ptTZuSfjGa86984321 = -238817563;    int ptTZuSfjGa40010529 = -306724088;    int ptTZuSfjGa23037498 = -617936089;    int ptTZuSfjGa46045707 = -927971349;    int ptTZuSfjGa88222071 = -506027512;    int ptTZuSfjGa79696853 = -963704878;    int ptTZuSfjGa23051014 = -64518179;    int ptTZuSfjGa15912501 = -373303968;    int ptTZuSfjGa20863414 = -720203642;    int ptTZuSfjGa75387588 = -332911771;    int ptTZuSfjGa80668680 = -73859063;    int ptTZuSfjGa86555460 = -646674838;    int ptTZuSfjGa50448195 = -223384849;    int ptTZuSfjGa48653765 = -107158291;    int ptTZuSfjGa78110653 = -889506537;    int ptTZuSfjGa98638462 = -514816194;    int ptTZuSfjGa76836506 = -452803497;    int ptTZuSfjGa68846222 = -835711157;    int ptTZuSfjGa31362664 = -103570196;    int ptTZuSfjGa91521000 = -712004926;    int ptTZuSfjGa19583816 = -223604526;    int ptTZuSfjGa47392303 = -302403473;    int ptTZuSfjGa33758689 = 79975601;    int ptTZuSfjGa19570457 = -569727944;    int ptTZuSfjGa68098814 = -471342952;    int ptTZuSfjGa24207336 = -550081038;    int ptTZuSfjGa47205506 = -56406479;    int ptTZuSfjGa94906604 = -388199412;    int ptTZuSfjGa56960157 = 29503122;    int ptTZuSfjGa67502386 = -150643651;    int ptTZuSfjGa7237440 = -48655726;    int ptTZuSfjGa31645447 = -815601516;    int ptTZuSfjGa84434514 = -90803778;    int ptTZuSfjGa43235819 = 47025120;    int ptTZuSfjGa24565071 = -696608324;    int ptTZuSfjGa68151381 = 392720;    int ptTZuSfjGa30601890 = -409842897;    int ptTZuSfjGa84501085 = -209636652;    int ptTZuSfjGa88621361 = 90214446;    int ptTZuSfjGa35131409 = -245753630;    int ptTZuSfjGa71225598 = -373825270;    int ptTZuSfjGa46025158 = -763835948;    int ptTZuSfjGa91994915 = -854062481;    int ptTZuSfjGa59627317 = -467727688;    int ptTZuSfjGa86704794 = -991437360;    int ptTZuSfjGa71319349 = -672430219;    int ptTZuSfjGa70237459 = -378131814;    int ptTZuSfjGa58821777 = -720745596;    int ptTZuSfjGa16507332 = -839820587;    int ptTZuSfjGa12943163 = -356143128;    int ptTZuSfjGa95634978 = 26696571;    int ptTZuSfjGa65268662 = -206487885;    int ptTZuSfjGa53203621 = -371457027;    int ptTZuSfjGa45731435 = -281746213;    int ptTZuSfjGa34203042 = -261898302;    int ptTZuSfjGa39592018 = -936414091;    int ptTZuSfjGa6251841 = -286699690;    int ptTZuSfjGa3467041 = 51791855;    int ptTZuSfjGa77946893 = -356628397;    int ptTZuSfjGa64014736 = -955946475;    int ptTZuSfjGa32491348 = -807298400;    int ptTZuSfjGa28144410 = -676318768;    int ptTZuSfjGa58952343 = -302807091;    int ptTZuSfjGa53361027 = -469559992;    int ptTZuSfjGa68150148 = -184256046;    int ptTZuSfjGa49023233 = -258257548;    int ptTZuSfjGa2120946 = -455871061;    int ptTZuSfjGa7212377 = -170409969;    int ptTZuSfjGa24088694 = -410549967;    int ptTZuSfjGa9959272 = -789899257;    int ptTZuSfjGa68036573 = -4973298;    int ptTZuSfjGa92335421 = -143166846;    int ptTZuSfjGa80224860 = -825925603;    int ptTZuSfjGa96231254 = -857816567;    int ptTZuSfjGa20295402 = -238179657;    int ptTZuSfjGa73558657 = -459768578;    int ptTZuSfjGa55397387 = -448340992;    int ptTZuSfjGa74131371 = -452296712;    int ptTZuSfjGa32865663 = -578290585;    int ptTZuSfjGa96779464 = -406331368;     ptTZuSfjGa8161911 = ptTZuSfjGa53639387;     ptTZuSfjGa53639387 = ptTZuSfjGa30546793;     ptTZuSfjGa30546793 = ptTZuSfjGa76843432;     ptTZuSfjGa76843432 = ptTZuSfjGa14828263;     ptTZuSfjGa14828263 = ptTZuSfjGa94276611;     ptTZuSfjGa94276611 = ptTZuSfjGa61937658;     ptTZuSfjGa61937658 = ptTZuSfjGa12858330;     ptTZuSfjGa12858330 = ptTZuSfjGa35014905;     ptTZuSfjGa35014905 = ptTZuSfjGa67373474;     ptTZuSfjGa67373474 = ptTZuSfjGa57874810;     ptTZuSfjGa57874810 = ptTZuSfjGa20685655;     ptTZuSfjGa20685655 = ptTZuSfjGa7475542;     ptTZuSfjGa7475542 = ptTZuSfjGa94617984;     ptTZuSfjGa94617984 = ptTZuSfjGa11581625;     ptTZuSfjGa11581625 = ptTZuSfjGa72471484;     ptTZuSfjGa72471484 = ptTZuSfjGa34114884;     ptTZuSfjGa34114884 = ptTZuSfjGa84566285;     ptTZuSfjGa84566285 = ptTZuSfjGa37252435;     ptTZuSfjGa37252435 = ptTZuSfjGa53786858;     ptTZuSfjGa53786858 = ptTZuSfjGa86984321;     ptTZuSfjGa86984321 = ptTZuSfjGa40010529;     ptTZuSfjGa40010529 = ptTZuSfjGa23037498;     ptTZuSfjGa23037498 = ptTZuSfjGa46045707;     ptTZuSfjGa46045707 = ptTZuSfjGa88222071;     ptTZuSfjGa88222071 = ptTZuSfjGa79696853;     ptTZuSfjGa79696853 = ptTZuSfjGa23051014;     ptTZuSfjGa23051014 = ptTZuSfjGa15912501;     ptTZuSfjGa15912501 = ptTZuSfjGa20863414;     ptTZuSfjGa20863414 = ptTZuSfjGa75387588;     ptTZuSfjGa75387588 = ptTZuSfjGa80668680;     ptTZuSfjGa80668680 = ptTZuSfjGa86555460;     ptTZuSfjGa86555460 = ptTZuSfjGa50448195;     ptTZuSfjGa50448195 = ptTZuSfjGa48653765;     ptTZuSfjGa48653765 = ptTZuSfjGa78110653;     ptTZuSfjGa78110653 = ptTZuSfjGa98638462;     ptTZuSfjGa98638462 = ptTZuSfjGa76836506;     ptTZuSfjGa76836506 = ptTZuSfjGa68846222;     ptTZuSfjGa68846222 = ptTZuSfjGa31362664;     ptTZuSfjGa31362664 = ptTZuSfjGa91521000;     ptTZuSfjGa91521000 = ptTZuSfjGa19583816;     ptTZuSfjGa19583816 = ptTZuSfjGa47392303;     ptTZuSfjGa47392303 = ptTZuSfjGa33758689;     ptTZuSfjGa33758689 = ptTZuSfjGa19570457;     ptTZuSfjGa19570457 = ptTZuSfjGa68098814;     ptTZuSfjGa68098814 = ptTZuSfjGa24207336;     ptTZuSfjGa24207336 = ptTZuSfjGa47205506;     ptTZuSfjGa47205506 = ptTZuSfjGa94906604;     ptTZuSfjGa94906604 = ptTZuSfjGa56960157;     ptTZuSfjGa56960157 = ptTZuSfjGa67502386;     ptTZuSfjGa67502386 = ptTZuSfjGa7237440;     ptTZuSfjGa7237440 = ptTZuSfjGa31645447;     ptTZuSfjGa31645447 = ptTZuSfjGa84434514;     ptTZuSfjGa84434514 = ptTZuSfjGa43235819;     ptTZuSfjGa43235819 = ptTZuSfjGa24565071;     ptTZuSfjGa24565071 = ptTZuSfjGa68151381;     ptTZuSfjGa68151381 = ptTZuSfjGa30601890;     ptTZuSfjGa30601890 = ptTZuSfjGa84501085;     ptTZuSfjGa84501085 = ptTZuSfjGa88621361;     ptTZuSfjGa88621361 = ptTZuSfjGa35131409;     ptTZuSfjGa35131409 = ptTZuSfjGa71225598;     ptTZuSfjGa71225598 = ptTZuSfjGa46025158;     ptTZuSfjGa46025158 = ptTZuSfjGa91994915;     ptTZuSfjGa91994915 = ptTZuSfjGa59627317;     ptTZuSfjGa59627317 = ptTZuSfjGa86704794;     ptTZuSfjGa86704794 = ptTZuSfjGa71319349;     ptTZuSfjGa71319349 = ptTZuSfjGa70237459;     ptTZuSfjGa70237459 = ptTZuSfjGa58821777;     ptTZuSfjGa58821777 = ptTZuSfjGa16507332;     ptTZuSfjGa16507332 = ptTZuSfjGa12943163;     ptTZuSfjGa12943163 = ptTZuSfjGa95634978;     ptTZuSfjGa95634978 = ptTZuSfjGa65268662;     ptTZuSfjGa65268662 = ptTZuSfjGa53203621;     ptTZuSfjGa53203621 = ptTZuSfjGa45731435;     ptTZuSfjGa45731435 = ptTZuSfjGa34203042;     ptTZuSfjGa34203042 = ptTZuSfjGa39592018;     ptTZuSfjGa39592018 = ptTZuSfjGa6251841;     ptTZuSfjGa6251841 = ptTZuSfjGa3467041;     ptTZuSfjGa3467041 = ptTZuSfjGa77946893;     ptTZuSfjGa77946893 = ptTZuSfjGa64014736;     ptTZuSfjGa64014736 = ptTZuSfjGa32491348;     ptTZuSfjGa32491348 = ptTZuSfjGa28144410;     ptTZuSfjGa28144410 = ptTZuSfjGa58952343;     ptTZuSfjGa58952343 = ptTZuSfjGa53361027;     ptTZuSfjGa53361027 = ptTZuSfjGa68150148;     ptTZuSfjGa68150148 = ptTZuSfjGa49023233;     ptTZuSfjGa49023233 = ptTZuSfjGa2120946;     ptTZuSfjGa2120946 = ptTZuSfjGa7212377;     ptTZuSfjGa7212377 = ptTZuSfjGa24088694;     ptTZuSfjGa24088694 = ptTZuSfjGa9959272;     ptTZuSfjGa9959272 = ptTZuSfjGa68036573;     ptTZuSfjGa68036573 = ptTZuSfjGa92335421;     ptTZuSfjGa92335421 = ptTZuSfjGa80224860;     ptTZuSfjGa80224860 = ptTZuSfjGa96231254;     ptTZuSfjGa96231254 = ptTZuSfjGa20295402;     ptTZuSfjGa20295402 = ptTZuSfjGa73558657;     ptTZuSfjGa73558657 = ptTZuSfjGa55397387;     ptTZuSfjGa55397387 = ptTZuSfjGa74131371;     ptTZuSfjGa74131371 = ptTZuSfjGa32865663;     ptTZuSfjGa32865663 = ptTZuSfjGa96779464;     ptTZuSfjGa96779464 = ptTZuSfjGa8161911;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void osVXLULVNL48194367() {     int MfxyeqQXzd30549028 = -40735331;    int MfxyeqQXzd37986888 = 23677960;    int MfxyeqQXzd59316071 = -377261844;    int MfxyeqQXzd91850120 = -522220597;    int MfxyeqQXzd13002517 = -800515509;    int MfxyeqQXzd9159729 = -398951827;    int MfxyeqQXzd54093397 = 40872778;    int MfxyeqQXzd88492188 = 55137233;    int MfxyeqQXzd32178036 = -784835498;    int MfxyeqQXzd59137058 = -908037755;    int MfxyeqQXzd95416669 = -315729457;    int MfxyeqQXzd76588604 = -952261956;    int MfxyeqQXzd74483722 = -446621206;    int MfxyeqQXzd37232737 = -157035880;    int MfxyeqQXzd48316902 = -598437035;    int MfxyeqQXzd16727398 = -907774468;    int MfxyeqQXzd83938236 = -319478861;    int MfxyeqQXzd90542701 = 73230086;    int MfxyeqQXzd78534066 = -128853264;    int MfxyeqQXzd32303141 = -838137280;    int MfxyeqQXzd87867428 = -230304872;    int MfxyeqQXzd3825748 = -811490402;    int MfxyeqQXzd26330821 = -794016681;    int MfxyeqQXzd72545494 = -391964206;    int MfxyeqQXzd23762318 = -127586637;    int MfxyeqQXzd67999847 = -307599102;    int MfxyeqQXzd70140434 = -922104672;    int MfxyeqQXzd21823951 = -161758421;    int MfxyeqQXzd27755975 = 35539239;    int MfxyeqQXzd84544767 = -253207638;    int MfxyeqQXzd24959485 = -155175215;    int MfxyeqQXzd44096652 = -198746193;    int MfxyeqQXzd31486866 = -752728344;    int MfxyeqQXzd8042452 = -431285141;    int MfxyeqQXzd54340018 = -953262959;    int MfxyeqQXzd40331331 = -830211571;    int MfxyeqQXzd70276219 = -185751449;    int MfxyeqQXzd22472323 = -703336629;    int MfxyeqQXzd79670795 = -504616228;    int MfxyeqQXzd11706715 = -848228955;    int MfxyeqQXzd48341187 = -726782192;    int MfxyeqQXzd35900695 = -60312164;    int MfxyeqQXzd39308881 = -907554983;    int MfxyeqQXzd19014577 = -567091593;    int MfxyeqQXzd36082081 = -280486430;    int MfxyeqQXzd32307617 = -732310134;    int MfxyeqQXzd59502903 = -634816732;    int MfxyeqQXzd39505279 = -144734402;    int MfxyeqQXzd51420062 = 79786424;    int MfxyeqQXzd64586132 = -292663910;    int MfxyeqQXzd16106657 = -403732003;    int MfxyeqQXzd5496705 = -146420798;    int MfxyeqQXzd62748918 = -862715008;    int MfxyeqQXzd3522364 = -228655689;    int MfxyeqQXzd60326938 = -837312439;    int MfxyeqQXzd26723280 = -229244930;    int MfxyeqQXzd11656067 = -182305359;    int MfxyeqQXzd86770576 = -985297638;    int MfxyeqQXzd68087803 = -294633961;    int MfxyeqQXzd45002669 = -392916407;    int MfxyeqQXzd39019295 = -476847155;    int MfxyeqQXzd32269447 = -797368801;    int MfxyeqQXzd60736214 = -980402006;    int MfxyeqQXzd47633269 = -431627860;    int MfxyeqQXzd34177574 = -652862541;    int MfxyeqQXzd51320018 = -16983265;    int MfxyeqQXzd45101738 = -99533613;    int MfxyeqQXzd66441270 = 84663935;    int MfxyeqQXzd82892719 = -203772921;    int MfxyeqQXzd7985571 = -768225464;    int MfxyeqQXzd46451178 = -622023020;    int MfxyeqQXzd61465913 = -616142233;    int MfxyeqQXzd10871907 = -422153686;    int MfxyeqQXzd66827351 = -280624309;    int MfxyeqQXzd83961954 = -11355089;    int MfxyeqQXzd51966734 = -69992708;    int MfxyeqQXzd64516867 = -903935420;    int MfxyeqQXzd7316245 = -126925089;    int MfxyeqQXzd36463414 = -11477777;    int MfxyeqQXzd91454701 = -395276504;    int MfxyeqQXzd8496945 = -672782371;    int MfxyeqQXzd30635155 = -677370270;    int MfxyeqQXzd70403888 = -141544846;    int MfxyeqQXzd63169842 = -671796852;    int MfxyeqQXzd68438111 = -849475635;    int MfxyeqQXzd19462781 = 91245582;    int MfxyeqQXzd81347733 = -336031185;    int MfxyeqQXzd27964502 = -424072655;    int MfxyeqQXzd47715514 = -593972702;    int MfxyeqQXzd27616738 = -624018030;    int MfxyeqQXzd28675265 = -547906212;    int MfxyeqQXzd83505642 = -200453811;    int MfxyeqQXzd54384520 = -308702668;    int MfxyeqQXzd34668127 = -11699821;    int MfxyeqQXzd72687420 = -271381801;    int MfxyeqQXzd16071740 = -929413392;    int MfxyeqQXzd75164481 = -79910158;    int MfxyeqQXzd91675611 = -375927123;    int MfxyeqQXzd84837003 = -914229053;    int MfxyeqQXzd84762062 = -40735331;     MfxyeqQXzd30549028 = MfxyeqQXzd37986888;     MfxyeqQXzd37986888 = MfxyeqQXzd59316071;     MfxyeqQXzd59316071 = MfxyeqQXzd91850120;     MfxyeqQXzd91850120 = MfxyeqQXzd13002517;     MfxyeqQXzd13002517 = MfxyeqQXzd9159729;     MfxyeqQXzd9159729 = MfxyeqQXzd54093397;     MfxyeqQXzd54093397 = MfxyeqQXzd88492188;     MfxyeqQXzd88492188 = MfxyeqQXzd32178036;     MfxyeqQXzd32178036 = MfxyeqQXzd59137058;     MfxyeqQXzd59137058 = MfxyeqQXzd95416669;     MfxyeqQXzd95416669 = MfxyeqQXzd76588604;     MfxyeqQXzd76588604 = MfxyeqQXzd74483722;     MfxyeqQXzd74483722 = MfxyeqQXzd37232737;     MfxyeqQXzd37232737 = MfxyeqQXzd48316902;     MfxyeqQXzd48316902 = MfxyeqQXzd16727398;     MfxyeqQXzd16727398 = MfxyeqQXzd83938236;     MfxyeqQXzd83938236 = MfxyeqQXzd90542701;     MfxyeqQXzd90542701 = MfxyeqQXzd78534066;     MfxyeqQXzd78534066 = MfxyeqQXzd32303141;     MfxyeqQXzd32303141 = MfxyeqQXzd87867428;     MfxyeqQXzd87867428 = MfxyeqQXzd3825748;     MfxyeqQXzd3825748 = MfxyeqQXzd26330821;     MfxyeqQXzd26330821 = MfxyeqQXzd72545494;     MfxyeqQXzd72545494 = MfxyeqQXzd23762318;     MfxyeqQXzd23762318 = MfxyeqQXzd67999847;     MfxyeqQXzd67999847 = MfxyeqQXzd70140434;     MfxyeqQXzd70140434 = MfxyeqQXzd21823951;     MfxyeqQXzd21823951 = MfxyeqQXzd27755975;     MfxyeqQXzd27755975 = MfxyeqQXzd84544767;     MfxyeqQXzd84544767 = MfxyeqQXzd24959485;     MfxyeqQXzd24959485 = MfxyeqQXzd44096652;     MfxyeqQXzd44096652 = MfxyeqQXzd31486866;     MfxyeqQXzd31486866 = MfxyeqQXzd8042452;     MfxyeqQXzd8042452 = MfxyeqQXzd54340018;     MfxyeqQXzd54340018 = MfxyeqQXzd40331331;     MfxyeqQXzd40331331 = MfxyeqQXzd70276219;     MfxyeqQXzd70276219 = MfxyeqQXzd22472323;     MfxyeqQXzd22472323 = MfxyeqQXzd79670795;     MfxyeqQXzd79670795 = MfxyeqQXzd11706715;     MfxyeqQXzd11706715 = MfxyeqQXzd48341187;     MfxyeqQXzd48341187 = MfxyeqQXzd35900695;     MfxyeqQXzd35900695 = MfxyeqQXzd39308881;     MfxyeqQXzd39308881 = MfxyeqQXzd19014577;     MfxyeqQXzd19014577 = MfxyeqQXzd36082081;     MfxyeqQXzd36082081 = MfxyeqQXzd32307617;     MfxyeqQXzd32307617 = MfxyeqQXzd59502903;     MfxyeqQXzd59502903 = MfxyeqQXzd39505279;     MfxyeqQXzd39505279 = MfxyeqQXzd51420062;     MfxyeqQXzd51420062 = MfxyeqQXzd64586132;     MfxyeqQXzd64586132 = MfxyeqQXzd16106657;     MfxyeqQXzd16106657 = MfxyeqQXzd5496705;     MfxyeqQXzd5496705 = MfxyeqQXzd62748918;     MfxyeqQXzd62748918 = MfxyeqQXzd3522364;     MfxyeqQXzd3522364 = MfxyeqQXzd60326938;     MfxyeqQXzd60326938 = MfxyeqQXzd26723280;     MfxyeqQXzd26723280 = MfxyeqQXzd11656067;     MfxyeqQXzd11656067 = MfxyeqQXzd86770576;     MfxyeqQXzd86770576 = MfxyeqQXzd68087803;     MfxyeqQXzd68087803 = MfxyeqQXzd45002669;     MfxyeqQXzd45002669 = MfxyeqQXzd39019295;     MfxyeqQXzd39019295 = MfxyeqQXzd32269447;     MfxyeqQXzd32269447 = MfxyeqQXzd60736214;     MfxyeqQXzd60736214 = MfxyeqQXzd47633269;     MfxyeqQXzd47633269 = MfxyeqQXzd34177574;     MfxyeqQXzd34177574 = MfxyeqQXzd51320018;     MfxyeqQXzd51320018 = MfxyeqQXzd45101738;     MfxyeqQXzd45101738 = MfxyeqQXzd66441270;     MfxyeqQXzd66441270 = MfxyeqQXzd82892719;     MfxyeqQXzd82892719 = MfxyeqQXzd7985571;     MfxyeqQXzd7985571 = MfxyeqQXzd46451178;     MfxyeqQXzd46451178 = MfxyeqQXzd61465913;     MfxyeqQXzd61465913 = MfxyeqQXzd10871907;     MfxyeqQXzd10871907 = MfxyeqQXzd66827351;     MfxyeqQXzd66827351 = MfxyeqQXzd83961954;     MfxyeqQXzd83961954 = MfxyeqQXzd51966734;     MfxyeqQXzd51966734 = MfxyeqQXzd64516867;     MfxyeqQXzd64516867 = MfxyeqQXzd7316245;     MfxyeqQXzd7316245 = MfxyeqQXzd36463414;     MfxyeqQXzd36463414 = MfxyeqQXzd91454701;     MfxyeqQXzd91454701 = MfxyeqQXzd8496945;     MfxyeqQXzd8496945 = MfxyeqQXzd30635155;     MfxyeqQXzd30635155 = MfxyeqQXzd70403888;     MfxyeqQXzd70403888 = MfxyeqQXzd63169842;     MfxyeqQXzd63169842 = MfxyeqQXzd68438111;     MfxyeqQXzd68438111 = MfxyeqQXzd19462781;     MfxyeqQXzd19462781 = MfxyeqQXzd81347733;     MfxyeqQXzd81347733 = MfxyeqQXzd27964502;     MfxyeqQXzd27964502 = MfxyeqQXzd47715514;     MfxyeqQXzd47715514 = MfxyeqQXzd27616738;     MfxyeqQXzd27616738 = MfxyeqQXzd28675265;     MfxyeqQXzd28675265 = MfxyeqQXzd83505642;     MfxyeqQXzd83505642 = MfxyeqQXzd54384520;     MfxyeqQXzd54384520 = MfxyeqQXzd34668127;     MfxyeqQXzd34668127 = MfxyeqQXzd72687420;     MfxyeqQXzd72687420 = MfxyeqQXzd16071740;     MfxyeqQXzd16071740 = MfxyeqQXzd75164481;     MfxyeqQXzd75164481 = MfxyeqQXzd91675611;     MfxyeqQXzd91675611 = MfxyeqQXzd84837003;     MfxyeqQXzd84837003 = MfxyeqQXzd84762062;     MfxyeqQXzd84762062 = MfxyeqQXzd30549028;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void vBkPSpFdFt84839341() {     int FidrJPIbEk52936145 = -775139294;    int FidrJPIbEk22334388 = 75134906;    int FidrJPIbEk88085349 = -616915688;    int FidrJPIbEk6856810 = -528628129;    int FidrJPIbEk11176771 = -291572510;    int FidrJPIbEk24042846 = -259560205;    int FidrJPIbEk46249136 = -881114528;    int FidrJPIbEk64126047 = -415459411;    int FidrJPIbEk29341167 = -669031537;    int FidrJPIbEk50900643 = -650779088;    int FidrJPIbEk32958529 = -312353858;    int FidrJPIbEk32491553 = -103007249;    int FidrJPIbEk41491903 = 34661475;    int FidrJPIbEk79847489 = -684744637;    int FidrJPIbEk85052179 = -225914748;    int FidrJPIbEk60983310 = -189442011;    int FidrJPIbEk33761588 = -596758680;    int FidrJPIbEk96519117 = -378512605;    int FidrJPIbEk19815697 = -263955389;    int FidrJPIbEk10819425 = 9228266;    int FidrJPIbEk88750535 = -221792180;    int FidrJPIbEk67640966 = -216256715;    int FidrJPIbEk29624144 = -970097273;    int FidrJPIbEk99045281 = -955957063;    int FidrJPIbEk59302565 = -849145761;    int FidrJPIbEk56302842 = -751493326;    int FidrJPIbEk17229855 = -679691165;    int FidrJPIbEk27735401 = 49787125;    int FidrJPIbEk34648535 = -308717880;    int FidrJPIbEk93701946 = -173503505;    int FidrJPIbEk69250290 = -236491367;    int FidrJPIbEk1637843 = -850817547;    int FidrJPIbEk12525537 = -182071838;    int FidrJPIbEk67431139 = -755411991;    int FidrJPIbEk30569382 = 82980618;    int FidrJPIbEk82024200 = -45606947;    int FidrJPIbEk63715931 = 81300600;    int FidrJPIbEk76098423 = -570962100;    int FidrJPIbEk27978927 = -905662260;    int FidrJPIbEk31892430 = -984452985;    int FidrJPIbEk77098557 = -129959858;    int FidrJPIbEk24409086 = -918220855;    int FidrJPIbEk44859073 = -795085566;    int FidrJPIbEk18458696 = -564455242;    int FidrJPIbEk4065348 = -89629908;    int FidrJPIbEk40407899 = -914539229;    int FidrJPIbEk71800300 = -113226985;    int FidrJPIbEk84103953 = 98730607;    int FidrJPIbEk45879967 = -969930274;    int FidrJPIbEk61669878 = -434684169;    int FidrJPIbEk24975873 = -758808280;    int FidrJPIbEk79347961 = -577240080;    int FidrJPIbEk41063321 = -534626238;    int FidrJPIbEk63808909 = -504336498;    int FidrJPIbEk96088805 = -978016555;    int FidrJPIbEk85295179 = -458882580;    int FidrJPIbEk92710243 = 45232178;    int FidrJPIbEk89040068 = -660958625;    int FidrJPIbEk47554244 = -679482368;    int FidrJPIbEk54873928 = -540079185;    int FidrJPIbEk6812992 = -579869040;    int FidrJPIbEk18513736 = -830901653;    int FidrJPIbEk29477512 = -6741531;    int FidrJPIbEk35639220 = -395528033;    int FidrJPIbEk81650353 = -314287721;    int FidrJPIbEk31320687 = -461536311;    int FidrJPIbEk19966017 = -920935411;    int FidrJPIbEk74060763 = -209926535;    int FidrJPIbEk49278107 = -667725255;    int FidrJPIbEk3027979 = -80307801;    int FidrJPIbEk97267378 = -170742611;    int FidrJPIbEk57663164 = 74203420;    int FidrJPIbEk68540191 = -472850345;    int FidrJPIbEk87923267 = -279502405;    int FidrJPIbEk33720867 = -860811877;    int FidrJPIbEk64341450 = -303571326;    int FidrJPIbEk22781894 = -421171149;    int FidrJPIbEk11165449 = -305642032;    int FidrJPIbEk94979934 = -766327156;    int FidrJPIbEk18894666 = -934606533;    int FidrJPIbEk84502541 = -538266342;    int FidrJPIbEk33125901 = -678421772;    int FidrJPIbEk81855433 = 19717399;    int FidrJPIbEk72978657 = -874033712;    int FidrJPIbEk68726074 = -414695225;    int FidrJPIbEk89902328 = -659251288;    int FidrJPIbEk60574521 = -216191310;    int FidrJPIbEk48716627 = -677735341;    int FidrJPIbEk71342334 = -777395437;    int FidrJPIbEk45274203 = -458136802;    int FidrJPIbEk89313956 = 9160874;    int FidrJPIbEk74675863 = -257740775;    int FidrJPIbEk28544179 = -891479733;    int FidrJPIbEk73104998 = -265583076;    int FidrJPIbEk25079439 = -304583945;    int FidrJPIbEk58584822 = -299058205;    int FidrJPIbEk94931574 = -811479324;    int FidrJPIbEk9219853 = -299557534;    int FidrJPIbEk36808343 = -150167521;    int FidrJPIbEk72744660 = -775139294;     FidrJPIbEk52936145 = FidrJPIbEk22334388;     FidrJPIbEk22334388 = FidrJPIbEk88085349;     FidrJPIbEk88085349 = FidrJPIbEk6856810;     FidrJPIbEk6856810 = FidrJPIbEk11176771;     FidrJPIbEk11176771 = FidrJPIbEk24042846;     FidrJPIbEk24042846 = FidrJPIbEk46249136;     FidrJPIbEk46249136 = FidrJPIbEk64126047;     FidrJPIbEk64126047 = FidrJPIbEk29341167;     FidrJPIbEk29341167 = FidrJPIbEk50900643;     FidrJPIbEk50900643 = FidrJPIbEk32958529;     FidrJPIbEk32958529 = FidrJPIbEk32491553;     FidrJPIbEk32491553 = FidrJPIbEk41491903;     FidrJPIbEk41491903 = FidrJPIbEk79847489;     FidrJPIbEk79847489 = FidrJPIbEk85052179;     FidrJPIbEk85052179 = FidrJPIbEk60983310;     FidrJPIbEk60983310 = FidrJPIbEk33761588;     FidrJPIbEk33761588 = FidrJPIbEk96519117;     FidrJPIbEk96519117 = FidrJPIbEk19815697;     FidrJPIbEk19815697 = FidrJPIbEk10819425;     FidrJPIbEk10819425 = FidrJPIbEk88750535;     FidrJPIbEk88750535 = FidrJPIbEk67640966;     FidrJPIbEk67640966 = FidrJPIbEk29624144;     FidrJPIbEk29624144 = FidrJPIbEk99045281;     FidrJPIbEk99045281 = FidrJPIbEk59302565;     FidrJPIbEk59302565 = FidrJPIbEk56302842;     FidrJPIbEk56302842 = FidrJPIbEk17229855;     FidrJPIbEk17229855 = FidrJPIbEk27735401;     FidrJPIbEk27735401 = FidrJPIbEk34648535;     FidrJPIbEk34648535 = FidrJPIbEk93701946;     FidrJPIbEk93701946 = FidrJPIbEk69250290;     FidrJPIbEk69250290 = FidrJPIbEk1637843;     FidrJPIbEk1637843 = FidrJPIbEk12525537;     FidrJPIbEk12525537 = FidrJPIbEk67431139;     FidrJPIbEk67431139 = FidrJPIbEk30569382;     FidrJPIbEk30569382 = FidrJPIbEk82024200;     FidrJPIbEk82024200 = FidrJPIbEk63715931;     FidrJPIbEk63715931 = FidrJPIbEk76098423;     FidrJPIbEk76098423 = FidrJPIbEk27978927;     FidrJPIbEk27978927 = FidrJPIbEk31892430;     FidrJPIbEk31892430 = FidrJPIbEk77098557;     FidrJPIbEk77098557 = FidrJPIbEk24409086;     FidrJPIbEk24409086 = FidrJPIbEk44859073;     FidrJPIbEk44859073 = FidrJPIbEk18458696;     FidrJPIbEk18458696 = FidrJPIbEk4065348;     FidrJPIbEk4065348 = FidrJPIbEk40407899;     FidrJPIbEk40407899 = FidrJPIbEk71800300;     FidrJPIbEk71800300 = FidrJPIbEk84103953;     FidrJPIbEk84103953 = FidrJPIbEk45879967;     FidrJPIbEk45879967 = FidrJPIbEk61669878;     FidrJPIbEk61669878 = FidrJPIbEk24975873;     FidrJPIbEk24975873 = FidrJPIbEk79347961;     FidrJPIbEk79347961 = FidrJPIbEk41063321;     FidrJPIbEk41063321 = FidrJPIbEk63808909;     FidrJPIbEk63808909 = FidrJPIbEk96088805;     FidrJPIbEk96088805 = FidrJPIbEk85295179;     FidrJPIbEk85295179 = FidrJPIbEk92710243;     FidrJPIbEk92710243 = FidrJPIbEk89040068;     FidrJPIbEk89040068 = FidrJPIbEk47554244;     FidrJPIbEk47554244 = FidrJPIbEk54873928;     FidrJPIbEk54873928 = FidrJPIbEk6812992;     FidrJPIbEk6812992 = FidrJPIbEk18513736;     FidrJPIbEk18513736 = FidrJPIbEk29477512;     FidrJPIbEk29477512 = FidrJPIbEk35639220;     FidrJPIbEk35639220 = FidrJPIbEk81650353;     FidrJPIbEk81650353 = FidrJPIbEk31320687;     FidrJPIbEk31320687 = FidrJPIbEk19966017;     FidrJPIbEk19966017 = FidrJPIbEk74060763;     FidrJPIbEk74060763 = FidrJPIbEk49278107;     FidrJPIbEk49278107 = FidrJPIbEk3027979;     FidrJPIbEk3027979 = FidrJPIbEk97267378;     FidrJPIbEk97267378 = FidrJPIbEk57663164;     FidrJPIbEk57663164 = FidrJPIbEk68540191;     FidrJPIbEk68540191 = FidrJPIbEk87923267;     FidrJPIbEk87923267 = FidrJPIbEk33720867;     FidrJPIbEk33720867 = FidrJPIbEk64341450;     FidrJPIbEk64341450 = FidrJPIbEk22781894;     FidrJPIbEk22781894 = FidrJPIbEk11165449;     FidrJPIbEk11165449 = FidrJPIbEk94979934;     FidrJPIbEk94979934 = FidrJPIbEk18894666;     FidrJPIbEk18894666 = FidrJPIbEk84502541;     FidrJPIbEk84502541 = FidrJPIbEk33125901;     FidrJPIbEk33125901 = FidrJPIbEk81855433;     FidrJPIbEk81855433 = FidrJPIbEk72978657;     FidrJPIbEk72978657 = FidrJPIbEk68726074;     FidrJPIbEk68726074 = FidrJPIbEk89902328;     FidrJPIbEk89902328 = FidrJPIbEk60574521;     FidrJPIbEk60574521 = FidrJPIbEk48716627;     FidrJPIbEk48716627 = FidrJPIbEk71342334;     FidrJPIbEk71342334 = FidrJPIbEk45274203;     FidrJPIbEk45274203 = FidrJPIbEk89313956;     FidrJPIbEk89313956 = FidrJPIbEk74675863;     FidrJPIbEk74675863 = FidrJPIbEk28544179;     FidrJPIbEk28544179 = FidrJPIbEk73104998;     FidrJPIbEk73104998 = FidrJPIbEk25079439;     FidrJPIbEk25079439 = FidrJPIbEk58584822;     FidrJPIbEk58584822 = FidrJPIbEk94931574;     FidrJPIbEk94931574 = FidrJPIbEk9219853;     FidrJPIbEk9219853 = FidrJPIbEk36808343;     FidrJPIbEk36808343 = FidrJPIbEk72744660;     FidrJPIbEk72744660 = FidrJPIbEk52936145;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void hCvSRmOYKQ21484316() {     int SlbxxhOjvD75323262 = -409543258;    int SlbxxhOjvD6681889 = -973408149;    int SlbxxhOjvD16854629 = -856569532;    int SlbxxhOjvD21863498 = -535035660;    int SlbxxhOjvD9351025 = -882629511;    int SlbxxhOjvD38925962 = -120168583;    int SlbxxhOjvD38404875 = -703101834;    int SlbxxhOjvD39759906 = -886056055;    int SlbxxhOjvD26504298 = -553227576;    int SlbxxhOjvD42664228 = -393520421;    int SlbxxhOjvD70500389 = -308978259;    int SlbxxhOjvD88394501 = -353752542;    int SlbxxhOjvD8500084 = -584055845;    int SlbxxhOjvD22462241 = -112453394;    int SlbxxhOjvD21787456 = -953392461;    int SlbxxhOjvD5239223 = -571109553;    int SlbxxhOjvD83584939 = -874038500;    int SlbxxhOjvD2495535 = -830255295;    int SlbxxhOjvD61097328 = -399057514;    int SlbxxhOjvD89335707 = -243406187;    int SlbxxhOjvD89633642 = -213279488;    int SlbxxhOjvD31456184 = -721023028;    int SlbxxhOjvD32917467 = -46177865;    int SlbxxhOjvD25545069 = -419949921;    int SlbxxhOjvD94842811 = -470704886;    int SlbxxhOjvD44605837 = -95387550;    int SlbxxhOjvD64319275 = -437277658;    int SlbxxhOjvD33646851 = -838667329;    int SlbxxhOjvD41541096 = -652975000;    int SlbxxhOjvD2859127 = -93799371;    int SlbxxhOjvD13541096 = -317807519;    int SlbxxhOjvD59179033 = -402888902;    int SlbxxhOjvD93564206 = -711415333;    int SlbxxhOjvD26819827 = 20461159;    int SlbxxhOjvD6798747 = 19224196;    int SlbxxhOjvD23717070 = -361002324;    int SlbxxhOjvD57155643 = -751647352;    int SlbxxhOjvD29724525 = -438587572;    int SlbxxhOjvD76287058 = -206708291;    int SlbxxhOjvD52078144 = -20677014;    int SlbxxhOjvD5855929 = -633137524;    int SlbxxhOjvD12917477 = -676129546;    int SlbxxhOjvD50409265 = -682616150;    int SlbxxhOjvD17902815 = -561818891;    int SlbxxhOjvD72048614 = -998773385;    int SlbxxhOjvD48508181 = 3231676;    int SlbxxhOjvD84097697 = -691637238;    int SlbxxhOjvD28702628 = -757804384;    int SlbxxhOjvD40339872 = -919646973;    int SlbxxhOjvD58753623 = -576704428;    int SlbxxhOjvD33845090 = -13884557;    int SlbxxhOjvD53199219 = 91940638;    int SlbxxhOjvD19377724 = -206537468;    int SlbxxhOjvD24095455 = -780017306;    int SlbxxhOjvD31850673 = -18720670;    int SlbxxhOjvD43867078 = -688520230;    int SlbxxhOjvD73764421 = -827230285;    int SlbxxhOjvD91309559 = -336619612;    int SlbxxhOjvD27020686 = 35669225;    int SlbxxhOjvD64745188 = -687241962;    int SlbxxhOjvD74606687 = -682890925;    int SlbxxhOjvD4758025 = -864434506;    int SlbxxhOjvD98218809 = -133081056;    int SlbxxhOjvD23645172 = -359428205;    int SlbxxhOjvD29123133 = 24287098;    int SlbxxhOjvD11321356 = -906089358;    int SlbxxhOjvD94830295 = -642337210;    int SlbxxhOjvD81680256 = -504517004;    int SlbxxhOjvD15663495 = -31677590;    int SlbxxhOjvD98070386 = -492390138;    int SlbxxhOjvD48083579 = -819462202;    int SlbxxhOjvD53860415 = -335450928;    int SlbxxhOjvD26208477 = -523547004;    int SlbxxhOjvD9019184 = -278380501;    int SlbxxhOjvD83479779 = -610268664;    int SlbxxhOjvD76716166 = -537149943;    int SlbxxhOjvD81046919 = 61593121;    int SlbxxhOjvD15014653 = -484358975;    int SlbxxhOjvD53496455 = -421176536;    int SlbxxhOjvD46334631 = -373936562;    int SlbxxhOjvD60508139 = -403750313;    int SlbxxhOjvD35616647 = -679473274;    int SlbxxhOjvD93306978 = -919020356;    int SlbxxhOjvD82787472 = 23729428;    int SlbxxhOjvD69014037 = 20085185;    int SlbxxhOjvD60341877 = -309748158;    int SlbxxhOjvD39801309 = -96351434;    int SlbxxhOjvD69468752 = -931398028;    int SlbxxhOjvD94969153 = -960818172;    int SlbxxhOjvD62931669 = -292255575;    int SlbxxhOjvD49952649 = -533772039;    int SlbxxhOjvD65846083 = -315027740;    int SlbxxhOjvD2703839 = -374256798;    int SlbxxhOjvD11541870 = -519466330;    int SlbxxhOjvD77471456 = -337786089;    int SlbxxhOjvD1097904 = -768703019;    int SlbxxhOjvD14698668 = -443048490;    int SlbxxhOjvD26764093 = -223187945;    int SlbxxhOjvD88779682 = -486105989;    int SlbxxhOjvD60727258 = -409543258;     SlbxxhOjvD75323262 = SlbxxhOjvD6681889;     SlbxxhOjvD6681889 = SlbxxhOjvD16854629;     SlbxxhOjvD16854629 = SlbxxhOjvD21863498;     SlbxxhOjvD21863498 = SlbxxhOjvD9351025;     SlbxxhOjvD9351025 = SlbxxhOjvD38925962;     SlbxxhOjvD38925962 = SlbxxhOjvD38404875;     SlbxxhOjvD38404875 = SlbxxhOjvD39759906;     SlbxxhOjvD39759906 = SlbxxhOjvD26504298;     SlbxxhOjvD26504298 = SlbxxhOjvD42664228;     SlbxxhOjvD42664228 = SlbxxhOjvD70500389;     SlbxxhOjvD70500389 = SlbxxhOjvD88394501;     SlbxxhOjvD88394501 = SlbxxhOjvD8500084;     SlbxxhOjvD8500084 = SlbxxhOjvD22462241;     SlbxxhOjvD22462241 = SlbxxhOjvD21787456;     SlbxxhOjvD21787456 = SlbxxhOjvD5239223;     SlbxxhOjvD5239223 = SlbxxhOjvD83584939;     SlbxxhOjvD83584939 = SlbxxhOjvD2495535;     SlbxxhOjvD2495535 = SlbxxhOjvD61097328;     SlbxxhOjvD61097328 = SlbxxhOjvD89335707;     SlbxxhOjvD89335707 = SlbxxhOjvD89633642;     SlbxxhOjvD89633642 = SlbxxhOjvD31456184;     SlbxxhOjvD31456184 = SlbxxhOjvD32917467;     SlbxxhOjvD32917467 = SlbxxhOjvD25545069;     SlbxxhOjvD25545069 = SlbxxhOjvD94842811;     SlbxxhOjvD94842811 = SlbxxhOjvD44605837;     SlbxxhOjvD44605837 = SlbxxhOjvD64319275;     SlbxxhOjvD64319275 = SlbxxhOjvD33646851;     SlbxxhOjvD33646851 = SlbxxhOjvD41541096;     SlbxxhOjvD41541096 = SlbxxhOjvD2859127;     SlbxxhOjvD2859127 = SlbxxhOjvD13541096;     SlbxxhOjvD13541096 = SlbxxhOjvD59179033;     SlbxxhOjvD59179033 = SlbxxhOjvD93564206;     SlbxxhOjvD93564206 = SlbxxhOjvD26819827;     SlbxxhOjvD26819827 = SlbxxhOjvD6798747;     SlbxxhOjvD6798747 = SlbxxhOjvD23717070;     SlbxxhOjvD23717070 = SlbxxhOjvD57155643;     SlbxxhOjvD57155643 = SlbxxhOjvD29724525;     SlbxxhOjvD29724525 = SlbxxhOjvD76287058;     SlbxxhOjvD76287058 = SlbxxhOjvD52078144;     SlbxxhOjvD52078144 = SlbxxhOjvD5855929;     SlbxxhOjvD5855929 = SlbxxhOjvD12917477;     SlbxxhOjvD12917477 = SlbxxhOjvD50409265;     SlbxxhOjvD50409265 = SlbxxhOjvD17902815;     SlbxxhOjvD17902815 = SlbxxhOjvD72048614;     SlbxxhOjvD72048614 = SlbxxhOjvD48508181;     SlbxxhOjvD48508181 = SlbxxhOjvD84097697;     SlbxxhOjvD84097697 = SlbxxhOjvD28702628;     SlbxxhOjvD28702628 = SlbxxhOjvD40339872;     SlbxxhOjvD40339872 = SlbxxhOjvD58753623;     SlbxxhOjvD58753623 = SlbxxhOjvD33845090;     SlbxxhOjvD33845090 = SlbxxhOjvD53199219;     SlbxxhOjvD53199219 = SlbxxhOjvD19377724;     SlbxxhOjvD19377724 = SlbxxhOjvD24095455;     SlbxxhOjvD24095455 = SlbxxhOjvD31850673;     SlbxxhOjvD31850673 = SlbxxhOjvD43867078;     SlbxxhOjvD43867078 = SlbxxhOjvD73764421;     SlbxxhOjvD73764421 = SlbxxhOjvD91309559;     SlbxxhOjvD91309559 = SlbxxhOjvD27020686;     SlbxxhOjvD27020686 = SlbxxhOjvD64745188;     SlbxxhOjvD64745188 = SlbxxhOjvD74606687;     SlbxxhOjvD74606687 = SlbxxhOjvD4758025;     SlbxxhOjvD4758025 = SlbxxhOjvD98218809;     SlbxxhOjvD98218809 = SlbxxhOjvD23645172;     SlbxxhOjvD23645172 = SlbxxhOjvD29123133;     SlbxxhOjvD29123133 = SlbxxhOjvD11321356;     SlbxxhOjvD11321356 = SlbxxhOjvD94830295;     SlbxxhOjvD94830295 = SlbxxhOjvD81680256;     SlbxxhOjvD81680256 = SlbxxhOjvD15663495;     SlbxxhOjvD15663495 = SlbxxhOjvD98070386;     SlbxxhOjvD98070386 = SlbxxhOjvD48083579;     SlbxxhOjvD48083579 = SlbxxhOjvD53860415;     SlbxxhOjvD53860415 = SlbxxhOjvD26208477;     SlbxxhOjvD26208477 = SlbxxhOjvD9019184;     SlbxxhOjvD9019184 = SlbxxhOjvD83479779;     SlbxxhOjvD83479779 = SlbxxhOjvD76716166;     SlbxxhOjvD76716166 = SlbxxhOjvD81046919;     SlbxxhOjvD81046919 = SlbxxhOjvD15014653;     SlbxxhOjvD15014653 = SlbxxhOjvD53496455;     SlbxxhOjvD53496455 = SlbxxhOjvD46334631;     SlbxxhOjvD46334631 = SlbxxhOjvD60508139;     SlbxxhOjvD60508139 = SlbxxhOjvD35616647;     SlbxxhOjvD35616647 = SlbxxhOjvD93306978;     SlbxxhOjvD93306978 = SlbxxhOjvD82787472;     SlbxxhOjvD82787472 = SlbxxhOjvD69014037;     SlbxxhOjvD69014037 = SlbxxhOjvD60341877;     SlbxxhOjvD60341877 = SlbxxhOjvD39801309;     SlbxxhOjvD39801309 = SlbxxhOjvD69468752;     SlbxxhOjvD69468752 = SlbxxhOjvD94969153;     SlbxxhOjvD94969153 = SlbxxhOjvD62931669;     SlbxxhOjvD62931669 = SlbxxhOjvD49952649;     SlbxxhOjvD49952649 = SlbxxhOjvD65846083;     SlbxxhOjvD65846083 = SlbxxhOjvD2703839;     SlbxxhOjvD2703839 = SlbxxhOjvD11541870;     SlbxxhOjvD11541870 = SlbxxhOjvD77471456;     SlbxxhOjvD77471456 = SlbxxhOjvD1097904;     SlbxxhOjvD1097904 = SlbxxhOjvD14698668;     SlbxxhOjvD14698668 = SlbxxhOjvD26764093;     SlbxxhOjvD26764093 = SlbxxhOjvD88779682;     SlbxxhOjvD88779682 = SlbxxhOjvD60727258;     SlbxxhOjvD60727258 = SlbxxhOjvD75323262;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void rPZUPVZJNQ58129291() {     int lihzdosWXQ97710379 = -43947221;    int lihzdosWXQ91029388 = -921951204;    int lihzdosWXQ45623907 = 3776624;    int lihzdosWXQ36870186 = -541443192;    int lihzdosWXQ7525279 = -373686513;    int lihzdosWXQ53809079 = 19223040;    int lihzdosWXQ30560614 = -525089140;    int lihzdosWXQ15393764 = -256652699;    int lihzdosWXQ23667429 = -437423615;    int lihzdosWXQ34427813 = -136261754;    int lihzdosWXQ8042249 = -305602660;    int lihzdosWXQ44297451 = -604497836;    int lihzdosWXQ75508264 = -102773165;    int lihzdosWXQ65076993 = -640162151;    int lihzdosWXQ58522733 = -580870174;    int lihzdosWXQ49495135 = -952777096;    int lihzdosWXQ33408291 = -51318320;    int lihzdosWXQ8471951 = -181997986;    int lihzdosWXQ2378959 = -534159639;    int lihzdosWXQ67851991 = -496040641;    int lihzdosWXQ90516750 = -204766797;    int lihzdosWXQ95271402 = -125789342;    int lihzdosWXQ36210790 = -222258457;    int lihzdosWXQ52044856 = -983942778;    int lihzdosWXQ30383059 = -92264011;    int lihzdosWXQ32908831 = -539281774;    int lihzdosWXQ11408696 = -194864151;    int lihzdosWXQ39558301 = -627121782;    int lihzdosWXQ48433657 = -997232119;    int lihzdosWXQ12016306 = -14095238;    int lihzdosWXQ57831901 = -399123671;    int lihzdosWXQ16720225 = 45039743;    int lihzdosWXQ74602877 = -140758828;    int lihzdosWXQ86208514 = -303665691;    int lihzdosWXQ83028111 = -44532227;    int lihzdosWXQ65409939 = -676397700;    int lihzdosWXQ50595355 = -484595303;    int lihzdosWXQ83350625 = -306213044;    int lihzdosWXQ24595189 = -607754323;    int lihzdosWXQ72263858 = -156901043;    int lihzdosWXQ34613299 = -36315190;    int lihzdosWXQ1425869 = -434038237;    int lihzdosWXQ55959456 = -570146734;    int lihzdosWXQ17346934 = -559182540;    int lihzdosWXQ40031881 = -807916863;    int lihzdosWXQ56608463 = -178997420;    int lihzdosWXQ96395095 = -170047490;    int lihzdosWXQ73301302 = -514339375;    int lihzdosWXQ34799777 = -869363671;    int lihzdosWXQ55837369 = -718724687;    int lihzdosWXQ42714306 = -368960834;    int lihzdosWXQ27050476 = -338878644;    int lihzdosWXQ97692127 = -978448698;    int lihzdosWXQ84381999 = 44301885;    int lihzdosWXQ67612540 = -159424785;    int lihzdosWXQ2438978 = -918157880;    int lihzdosWXQ54818598 = -599692747;    int lihzdosWXQ93579051 = -12280599;    int lihzdosWXQ6487128 = -349179182;    int lihzdosWXQ74616447 = -834404740;    int lihzdosWXQ42400384 = -785912810;    int lihzdosWXQ91002313 = -897967358;    int lihzdosWXQ66960107 = -259420581;    int lihzdosWXQ11651124 = -323328378;    int lihzdosWXQ76595912 = -737138083;    int lihzdosWXQ91322024 = -250642404;    int lihzdosWXQ69694574 = -363739008;    int lihzdosWXQ89299750 = -799107474;    int lihzdosWXQ82048882 = -495629924;    int lihzdosWXQ93112794 = -904472474;    int lihzdosWXQ98899779 = -368181793;    int lihzdosWXQ50057666 = -745105276;    int lihzdosWXQ83876761 = -574243663;    int lihzdosWXQ30115100 = -277258597;    int lihzdosWXQ33238692 = -359725452;    int lihzdosWXQ89090881 = -770728561;    int lihzdosWXQ39311946 = -555642608;    int lihzdosWXQ18863856 = -663075918;    int lihzdosWXQ12012976 = -76025915;    int lihzdosWXQ73774596 = -913266591;    int lihzdosWXQ36513736 = -269234284;    int lihzdosWXQ38107393 = -680524776;    int lihzdosWXQ4758525 = -757758112;    int lihzdosWXQ92596287 = -178507432;    int lihzdosWXQ69302000 = -645134404;    int lihzdosWXQ30781425 = 39754972;    int lihzdosWXQ19028097 = 23488441;    int lihzdosWXQ90220877 = -85060714;    int lihzdosWXQ18595974 = -44240907;    int lihzdosWXQ80589134 = -126374348;    int lihzdosWXQ10591341 = 23295047;    int lihzdosWXQ57016304 = -372314705;    int lihzdosWXQ76863497 = -957033863;    int lihzdosWXQ49978741 = -773349584;    int lihzdosWXQ29863475 = -370988233;    int lihzdosWXQ43610986 = -138347832;    int lihzdosWXQ34465761 = -74617656;    int lihzdosWXQ44308333 = -146818357;    int lihzdosWXQ40751022 = -822044457;    int lihzdosWXQ48709856 = -43947221;     lihzdosWXQ97710379 = lihzdosWXQ91029388;     lihzdosWXQ91029388 = lihzdosWXQ45623907;     lihzdosWXQ45623907 = lihzdosWXQ36870186;     lihzdosWXQ36870186 = lihzdosWXQ7525279;     lihzdosWXQ7525279 = lihzdosWXQ53809079;     lihzdosWXQ53809079 = lihzdosWXQ30560614;     lihzdosWXQ30560614 = lihzdosWXQ15393764;     lihzdosWXQ15393764 = lihzdosWXQ23667429;     lihzdosWXQ23667429 = lihzdosWXQ34427813;     lihzdosWXQ34427813 = lihzdosWXQ8042249;     lihzdosWXQ8042249 = lihzdosWXQ44297451;     lihzdosWXQ44297451 = lihzdosWXQ75508264;     lihzdosWXQ75508264 = lihzdosWXQ65076993;     lihzdosWXQ65076993 = lihzdosWXQ58522733;     lihzdosWXQ58522733 = lihzdosWXQ49495135;     lihzdosWXQ49495135 = lihzdosWXQ33408291;     lihzdosWXQ33408291 = lihzdosWXQ8471951;     lihzdosWXQ8471951 = lihzdosWXQ2378959;     lihzdosWXQ2378959 = lihzdosWXQ67851991;     lihzdosWXQ67851991 = lihzdosWXQ90516750;     lihzdosWXQ90516750 = lihzdosWXQ95271402;     lihzdosWXQ95271402 = lihzdosWXQ36210790;     lihzdosWXQ36210790 = lihzdosWXQ52044856;     lihzdosWXQ52044856 = lihzdosWXQ30383059;     lihzdosWXQ30383059 = lihzdosWXQ32908831;     lihzdosWXQ32908831 = lihzdosWXQ11408696;     lihzdosWXQ11408696 = lihzdosWXQ39558301;     lihzdosWXQ39558301 = lihzdosWXQ48433657;     lihzdosWXQ48433657 = lihzdosWXQ12016306;     lihzdosWXQ12016306 = lihzdosWXQ57831901;     lihzdosWXQ57831901 = lihzdosWXQ16720225;     lihzdosWXQ16720225 = lihzdosWXQ74602877;     lihzdosWXQ74602877 = lihzdosWXQ86208514;     lihzdosWXQ86208514 = lihzdosWXQ83028111;     lihzdosWXQ83028111 = lihzdosWXQ65409939;     lihzdosWXQ65409939 = lihzdosWXQ50595355;     lihzdosWXQ50595355 = lihzdosWXQ83350625;     lihzdosWXQ83350625 = lihzdosWXQ24595189;     lihzdosWXQ24595189 = lihzdosWXQ72263858;     lihzdosWXQ72263858 = lihzdosWXQ34613299;     lihzdosWXQ34613299 = lihzdosWXQ1425869;     lihzdosWXQ1425869 = lihzdosWXQ55959456;     lihzdosWXQ55959456 = lihzdosWXQ17346934;     lihzdosWXQ17346934 = lihzdosWXQ40031881;     lihzdosWXQ40031881 = lihzdosWXQ56608463;     lihzdosWXQ56608463 = lihzdosWXQ96395095;     lihzdosWXQ96395095 = lihzdosWXQ73301302;     lihzdosWXQ73301302 = lihzdosWXQ34799777;     lihzdosWXQ34799777 = lihzdosWXQ55837369;     lihzdosWXQ55837369 = lihzdosWXQ42714306;     lihzdosWXQ42714306 = lihzdosWXQ27050476;     lihzdosWXQ27050476 = lihzdosWXQ97692127;     lihzdosWXQ97692127 = lihzdosWXQ84381999;     lihzdosWXQ84381999 = lihzdosWXQ67612540;     lihzdosWXQ67612540 = lihzdosWXQ2438978;     lihzdosWXQ2438978 = lihzdosWXQ54818598;     lihzdosWXQ54818598 = lihzdosWXQ93579051;     lihzdosWXQ93579051 = lihzdosWXQ6487128;     lihzdosWXQ6487128 = lihzdosWXQ74616447;     lihzdosWXQ74616447 = lihzdosWXQ42400384;     lihzdosWXQ42400384 = lihzdosWXQ91002313;     lihzdosWXQ91002313 = lihzdosWXQ66960107;     lihzdosWXQ66960107 = lihzdosWXQ11651124;     lihzdosWXQ11651124 = lihzdosWXQ76595912;     lihzdosWXQ76595912 = lihzdosWXQ91322024;     lihzdosWXQ91322024 = lihzdosWXQ69694574;     lihzdosWXQ69694574 = lihzdosWXQ89299750;     lihzdosWXQ89299750 = lihzdosWXQ82048882;     lihzdosWXQ82048882 = lihzdosWXQ93112794;     lihzdosWXQ93112794 = lihzdosWXQ98899779;     lihzdosWXQ98899779 = lihzdosWXQ50057666;     lihzdosWXQ50057666 = lihzdosWXQ83876761;     lihzdosWXQ83876761 = lihzdosWXQ30115100;     lihzdosWXQ30115100 = lihzdosWXQ33238692;     lihzdosWXQ33238692 = lihzdosWXQ89090881;     lihzdosWXQ89090881 = lihzdosWXQ39311946;     lihzdosWXQ39311946 = lihzdosWXQ18863856;     lihzdosWXQ18863856 = lihzdosWXQ12012976;     lihzdosWXQ12012976 = lihzdosWXQ73774596;     lihzdosWXQ73774596 = lihzdosWXQ36513736;     lihzdosWXQ36513736 = lihzdosWXQ38107393;     lihzdosWXQ38107393 = lihzdosWXQ4758525;     lihzdosWXQ4758525 = lihzdosWXQ92596287;     lihzdosWXQ92596287 = lihzdosWXQ69302000;     lihzdosWXQ69302000 = lihzdosWXQ30781425;     lihzdosWXQ30781425 = lihzdosWXQ19028097;     lihzdosWXQ19028097 = lihzdosWXQ90220877;     lihzdosWXQ90220877 = lihzdosWXQ18595974;     lihzdosWXQ18595974 = lihzdosWXQ80589134;     lihzdosWXQ80589134 = lihzdosWXQ10591341;     lihzdosWXQ10591341 = lihzdosWXQ57016304;     lihzdosWXQ57016304 = lihzdosWXQ76863497;     lihzdosWXQ76863497 = lihzdosWXQ49978741;     lihzdosWXQ49978741 = lihzdosWXQ29863475;     lihzdosWXQ29863475 = lihzdosWXQ43610986;     lihzdosWXQ43610986 = lihzdosWXQ34465761;     lihzdosWXQ34465761 = lihzdosWXQ44308333;     lihzdosWXQ44308333 = lihzdosWXQ40751022;     lihzdosWXQ40751022 = lihzdosWXQ48709856;     lihzdosWXQ48709856 = lihzdosWXQ97710379;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void xAQvTQUAib94774265() {     int eSsvXnFfii20097497 = -778351184;    int eSsvXnFfii75376889 = -870494258;    int eSsvXnFfii74393186 = -235877220;    int eSsvXnFfii51876875 = -547850724;    int eSsvXnFfii5699534 = -964743514;    int eSsvXnFfii68692196 = -941385338;    int eSsvXnFfii22716353 = -347076446;    int eSsvXnFfii91027622 = -727249344;    int eSsvXnFfii20830561 = -321619654;    int eSsvXnFfii26191397 = -979003087;    int eSsvXnFfii45584108 = -302227061;    int eSsvXnFfii200400 = -855243129;    int eSsvXnFfii42516445 = -721490484;    int eSsvXnFfii7691746 = -67870908;    int eSsvXnFfii95258010 = -208347887;    int eSsvXnFfii93751047 = -234444638;    int eSsvXnFfii83231643 = -328598140;    int eSsvXnFfii14448367 = -633740677;    int eSsvXnFfii43660590 = -669261765;    int eSsvXnFfii46368274 = -748675095;    int eSsvXnFfii91399857 = -196254105;    int eSsvXnFfii59086621 = -630555655;    int eSsvXnFfii39504113 = -398339049;    int eSsvXnFfii78544643 = -447935635;    int eSsvXnFfii65923305 = -813823135;    int eSsvXnFfii21211826 = -983175998;    int eSsvXnFfii58498115 = 47549357;    int eSsvXnFfii45469751 = -415576236;    int eSsvXnFfii55326218 = -241489238;    int eSsvXnFfii21173485 = 65608896;    int eSsvXnFfii2122706 = -480439823;    int eSsvXnFfii74261415 = -607031611;    int eSsvXnFfii55641547 = -670102323;    int eSsvXnFfii45597201 = -627792541;    int eSsvXnFfii59257476 = -108288649;    int eSsvXnFfii7102808 = -991793076;    int eSsvXnFfii44035068 = -217543255;    int eSsvXnFfii36976726 = -173838516;    int eSsvXnFfii72903320 = 91199645;    int eSsvXnFfii92449573 = -293125072;    int eSsvXnFfii63370670 = -539492856;    int eSsvXnFfii89934259 = -191946928;    int eSsvXnFfii61509648 = -457677318;    int eSsvXnFfii16791054 = -556546189;    int eSsvXnFfii8015148 = -617060341;    int eSsvXnFfii64708745 = -361226515;    int eSsvXnFfii8692493 = -748457743;    int eSsvXnFfii17899977 = -270874366;    int eSsvXnFfii29259682 = -819080369;    int eSsvXnFfii52921115 = -860744946;    int eSsvXnFfii51583522 = -724037111;    int eSsvXnFfii901734 = -769697926;    int eSsvXnFfii76006530 = -650359929;    int eSsvXnFfii44668545 = -231378924;    int eSsvXnFfii3374409 = -300128900;    int eSsvXnFfii61010876 = -47795529;    int eSsvXnFfii35872776 = -372155210;    int eSsvXnFfii95848542 = -787941586;    int eSsvXnFfii85953569 = -734027589;    int eSsvXnFfii84487707 = -981567517;    int eSsvXnFfii10194081 = -888934695;    int eSsvXnFfii77246602 = -931500211;    int eSsvXnFfii35701405 = -385760106;    int eSsvXnFfii99657075 = -287228550;    int eSsvXnFfii24068692 = -398563264;    int eSsvXnFfii71322693 = -695195450;    int eSsvXnFfii44558852 = -85140807;    int eSsvXnFfii96919243 = 6302056;    int eSsvXnFfii48434270 = -959582259;    int eSsvXnFfii88155202 = -216554811;    int eSsvXnFfii49715980 = 83098616;    int eSsvXnFfii46254917 = -54759624;    int eSsvXnFfii41545046 = -624940322;    int eSsvXnFfii51211016 = -276136693;    int eSsvXnFfii82997604 = -109182239;    int eSsvXnFfii1465598 = 95692822;    int eSsvXnFfii97576972 = -72878338;    int eSsvXnFfii22713060 = -841792861;    int eSsvXnFfii70529496 = -830875295;    int eSsvXnFfii1214561 = -352596620;    int eSsvXnFfii12519334 = -134718255;    int eSsvXnFfii40598139 = -681576278;    int eSsvXnFfii16210070 = -596495867;    int eSsvXnFfii2405103 = -380744292;    int eSsvXnFfii69589963 = -210353994;    int eSsvXnFfii1220973 = -710741898;    int eSsvXnFfii98254884 = -956671683;    int eSsvXnFfii10973003 = -338723400;    int eSsvXnFfii42222793 = -227663642;    int eSsvXnFfii98246599 = 39506880;    int eSsvXnFfii71230032 = -519637867;    int eSsvXnFfii48186525 = -429601669;    int eSsvXnFfii51023157 = -439810928;    int eSsvXnFfii88415613 = 72767162;    int eSsvXnFfii82255492 = -404190378;    int eSsvXnFfii86124068 = -607992646;    int eSsvXnFfii54232854 = -806186822;    int eSsvXnFfii61852573 = -70448768;    int eSsvXnFfii92722362 = -57982925;    int eSsvXnFfii36692454 = -778351184;     eSsvXnFfii20097497 = eSsvXnFfii75376889;     eSsvXnFfii75376889 = eSsvXnFfii74393186;     eSsvXnFfii74393186 = eSsvXnFfii51876875;     eSsvXnFfii51876875 = eSsvXnFfii5699534;     eSsvXnFfii5699534 = eSsvXnFfii68692196;     eSsvXnFfii68692196 = eSsvXnFfii22716353;     eSsvXnFfii22716353 = eSsvXnFfii91027622;     eSsvXnFfii91027622 = eSsvXnFfii20830561;     eSsvXnFfii20830561 = eSsvXnFfii26191397;     eSsvXnFfii26191397 = eSsvXnFfii45584108;     eSsvXnFfii45584108 = eSsvXnFfii200400;     eSsvXnFfii200400 = eSsvXnFfii42516445;     eSsvXnFfii42516445 = eSsvXnFfii7691746;     eSsvXnFfii7691746 = eSsvXnFfii95258010;     eSsvXnFfii95258010 = eSsvXnFfii93751047;     eSsvXnFfii93751047 = eSsvXnFfii83231643;     eSsvXnFfii83231643 = eSsvXnFfii14448367;     eSsvXnFfii14448367 = eSsvXnFfii43660590;     eSsvXnFfii43660590 = eSsvXnFfii46368274;     eSsvXnFfii46368274 = eSsvXnFfii91399857;     eSsvXnFfii91399857 = eSsvXnFfii59086621;     eSsvXnFfii59086621 = eSsvXnFfii39504113;     eSsvXnFfii39504113 = eSsvXnFfii78544643;     eSsvXnFfii78544643 = eSsvXnFfii65923305;     eSsvXnFfii65923305 = eSsvXnFfii21211826;     eSsvXnFfii21211826 = eSsvXnFfii58498115;     eSsvXnFfii58498115 = eSsvXnFfii45469751;     eSsvXnFfii45469751 = eSsvXnFfii55326218;     eSsvXnFfii55326218 = eSsvXnFfii21173485;     eSsvXnFfii21173485 = eSsvXnFfii2122706;     eSsvXnFfii2122706 = eSsvXnFfii74261415;     eSsvXnFfii74261415 = eSsvXnFfii55641547;     eSsvXnFfii55641547 = eSsvXnFfii45597201;     eSsvXnFfii45597201 = eSsvXnFfii59257476;     eSsvXnFfii59257476 = eSsvXnFfii7102808;     eSsvXnFfii7102808 = eSsvXnFfii44035068;     eSsvXnFfii44035068 = eSsvXnFfii36976726;     eSsvXnFfii36976726 = eSsvXnFfii72903320;     eSsvXnFfii72903320 = eSsvXnFfii92449573;     eSsvXnFfii92449573 = eSsvXnFfii63370670;     eSsvXnFfii63370670 = eSsvXnFfii89934259;     eSsvXnFfii89934259 = eSsvXnFfii61509648;     eSsvXnFfii61509648 = eSsvXnFfii16791054;     eSsvXnFfii16791054 = eSsvXnFfii8015148;     eSsvXnFfii8015148 = eSsvXnFfii64708745;     eSsvXnFfii64708745 = eSsvXnFfii8692493;     eSsvXnFfii8692493 = eSsvXnFfii17899977;     eSsvXnFfii17899977 = eSsvXnFfii29259682;     eSsvXnFfii29259682 = eSsvXnFfii52921115;     eSsvXnFfii52921115 = eSsvXnFfii51583522;     eSsvXnFfii51583522 = eSsvXnFfii901734;     eSsvXnFfii901734 = eSsvXnFfii76006530;     eSsvXnFfii76006530 = eSsvXnFfii44668545;     eSsvXnFfii44668545 = eSsvXnFfii3374409;     eSsvXnFfii3374409 = eSsvXnFfii61010876;     eSsvXnFfii61010876 = eSsvXnFfii35872776;     eSsvXnFfii35872776 = eSsvXnFfii95848542;     eSsvXnFfii95848542 = eSsvXnFfii85953569;     eSsvXnFfii85953569 = eSsvXnFfii84487707;     eSsvXnFfii84487707 = eSsvXnFfii10194081;     eSsvXnFfii10194081 = eSsvXnFfii77246602;     eSsvXnFfii77246602 = eSsvXnFfii35701405;     eSsvXnFfii35701405 = eSsvXnFfii99657075;     eSsvXnFfii99657075 = eSsvXnFfii24068692;     eSsvXnFfii24068692 = eSsvXnFfii71322693;     eSsvXnFfii71322693 = eSsvXnFfii44558852;     eSsvXnFfii44558852 = eSsvXnFfii96919243;     eSsvXnFfii96919243 = eSsvXnFfii48434270;     eSsvXnFfii48434270 = eSsvXnFfii88155202;     eSsvXnFfii88155202 = eSsvXnFfii49715980;     eSsvXnFfii49715980 = eSsvXnFfii46254917;     eSsvXnFfii46254917 = eSsvXnFfii41545046;     eSsvXnFfii41545046 = eSsvXnFfii51211016;     eSsvXnFfii51211016 = eSsvXnFfii82997604;     eSsvXnFfii82997604 = eSsvXnFfii1465598;     eSsvXnFfii1465598 = eSsvXnFfii97576972;     eSsvXnFfii97576972 = eSsvXnFfii22713060;     eSsvXnFfii22713060 = eSsvXnFfii70529496;     eSsvXnFfii70529496 = eSsvXnFfii1214561;     eSsvXnFfii1214561 = eSsvXnFfii12519334;     eSsvXnFfii12519334 = eSsvXnFfii40598139;     eSsvXnFfii40598139 = eSsvXnFfii16210070;     eSsvXnFfii16210070 = eSsvXnFfii2405103;     eSsvXnFfii2405103 = eSsvXnFfii69589963;     eSsvXnFfii69589963 = eSsvXnFfii1220973;     eSsvXnFfii1220973 = eSsvXnFfii98254884;     eSsvXnFfii98254884 = eSsvXnFfii10973003;     eSsvXnFfii10973003 = eSsvXnFfii42222793;     eSsvXnFfii42222793 = eSsvXnFfii98246599;     eSsvXnFfii98246599 = eSsvXnFfii71230032;     eSsvXnFfii71230032 = eSsvXnFfii48186525;     eSsvXnFfii48186525 = eSsvXnFfii51023157;     eSsvXnFfii51023157 = eSsvXnFfii88415613;     eSsvXnFfii88415613 = eSsvXnFfii82255492;     eSsvXnFfii82255492 = eSsvXnFfii86124068;     eSsvXnFfii86124068 = eSsvXnFfii54232854;     eSsvXnFfii54232854 = eSsvXnFfii61852573;     eSsvXnFfii61852573 = eSsvXnFfii92722362;     eSsvXnFfii92722362 = eSsvXnFfii36692454;     eSsvXnFfii36692454 = eSsvXnFfii20097497;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void VuPmKzpzVa31419240() {     int eoxcUgfRql42484615 = -412755147;    int eoxcUgfRql59724389 = -819037313;    int eoxcUgfRql3162465 = -475531065;    int eoxcUgfRql66883563 = -554258255;    int eoxcUgfRql3873788 = -455800515;    int eoxcUgfRql83575313 = -801993716;    int eoxcUgfRql14872092 = -169063752;    int eoxcUgfRql66661481 = -97845988;    int eoxcUgfRql17993692 = -205815693;    int eoxcUgfRql17954982 = -721744420;    int eoxcUgfRql83125968 = -298851461;    int eoxcUgfRql56103349 = -5988422;    int eoxcUgfRql9524626 = -240207804;    int eoxcUgfRql50306498 = -595579665;    int eoxcUgfRql31993287 = -935825600;    int eoxcUgfRql38006960 = -616112181;    int eoxcUgfRql33054995 = -605877960;    int eoxcUgfRql20424783 = 14516633;    int eoxcUgfRql84942220 = -804363890;    int eoxcUgfRql24884558 = 98690452;    int eoxcUgfRql92282964 = -187741414;    int eoxcUgfRql22901840 = -35321968;    int eoxcUgfRql42797437 = -574419641;    int eoxcUgfRql5044431 = 88071508;    int eoxcUgfRql1463553 = -435382260;    int eoxcUgfRql9514821 = -327070221;    int eoxcUgfRql5587536 = -810037136;    int eoxcUgfRql51381201 = -204030689;    int eoxcUgfRql62218778 = -585746357;    int eoxcUgfRql30330665 = -954686971;    int eoxcUgfRql46413511 = -561755975;    int eoxcUgfRql31802606 = -159102966;    int eoxcUgfRql36680218 = -99445818;    int eoxcUgfRql4985889 = -951919391;    int eoxcUgfRql35486841 = -172045072;    int eoxcUgfRql48795677 = -207188453;    int eoxcUgfRql37474780 = 49508794;    int eoxcUgfRql90602827 = -41463988;    int eoxcUgfRql21211452 = -309846387;    int eoxcUgfRql12635288 = -429349102;    int eoxcUgfRql92128041 = 57329478;    int eoxcUgfRql78442650 = 50144381;    int eoxcUgfRql67059840 = -345207901;    int eoxcUgfRql16235173 = -553909838;    int eoxcUgfRql75998414 = -426203818;    int eoxcUgfRql72809026 = -543455611;    int eoxcUgfRql20989890 = -226867996;    int eoxcUgfRql62498651 = -27409357;    int eoxcUgfRql23719587 = -768797068;    int eoxcUgfRql50004860 = 97234795;    int eoxcUgfRql60452739 = 20886612;    int eoxcUgfRql74752990 = -100517208;    int eoxcUgfRql54320934 = -322271159;    int eoxcUgfRql4955090 = -507059733;    int eoxcUgfRql39136276 = -440833016;    int eoxcUgfRql19582775 = -277433179;    int eoxcUgfRql16926953 = -144617672;    int eoxcUgfRql98118034 = -463602573;    int eoxcUgfRql65420011 = -18875996;    int eoxcUgfRql94358966 = -28730294;    int eoxcUgfRql77987777 = -991956580;    int eoxcUgfRql63490891 = -965033063;    int eoxcUgfRql4442703 = -512099631;    int eoxcUgfRql87663026 = -251128723;    int eoxcUgfRql71541471 = -59988445;    int eoxcUgfRql51323362 = -39748496;    int eoxcUgfRql19423131 = -906542605;    int eoxcUgfRql4538737 = -288288413;    int eoxcUgfRql14819658 = -323534593;    int eoxcUgfRql83197610 = -628637148;    int eoxcUgfRql532180 = -565620975;    int eoxcUgfRql42452168 = -464413972;    int eoxcUgfRql99213331 = -675636981;    int eoxcUgfRql72306932 = -275014789;    int eoxcUgfRql32756517 = -958639027;    int eoxcUgfRql13840314 = -137885795;    int eoxcUgfRql55841999 = -690114067;    int eoxcUgfRql26562264 = 79490196;    int eoxcUgfRql29046017 = -485724674;    int eoxcUgfRql28654526 = -891926650;    int eoxcUgfRql88524930 = -202226;    int eoxcUgfRql43088885 = -682627780;    int eoxcUgfRql27661615 = -435233622;    int eoxcUgfRql12213918 = -582981153;    int eoxcUgfRql69877926 = -875573583;    int eoxcUgfRql71660520 = -361238768;    int eoxcUgfRql77481672 = -836831808;    int eoxcUgfRql31725128 = -592386086;    int eoxcUgfRql65849613 = -411086376;    int eoxcUgfRql15904066 = -894611893;    int eoxcUgfRql31868725 = 37429219;    int eoxcUgfRql39356746 = -486888634;    int eoxcUgfRql25182817 = 77412007;    int eoxcUgfRql26852485 = -181116093;    int eoxcUgfRql34647511 = -437392522;    int eoxcUgfRql28637150 = 22362541;    int eoxcUgfRql73999948 = -437755988;    int eoxcUgfRql79396814 = 5920821;    int eoxcUgfRql44693702 = -393921393;    int eoxcUgfRql24675052 = -412755147;     eoxcUgfRql42484615 = eoxcUgfRql59724389;     eoxcUgfRql59724389 = eoxcUgfRql3162465;     eoxcUgfRql3162465 = eoxcUgfRql66883563;     eoxcUgfRql66883563 = eoxcUgfRql3873788;     eoxcUgfRql3873788 = eoxcUgfRql83575313;     eoxcUgfRql83575313 = eoxcUgfRql14872092;     eoxcUgfRql14872092 = eoxcUgfRql66661481;     eoxcUgfRql66661481 = eoxcUgfRql17993692;     eoxcUgfRql17993692 = eoxcUgfRql17954982;     eoxcUgfRql17954982 = eoxcUgfRql83125968;     eoxcUgfRql83125968 = eoxcUgfRql56103349;     eoxcUgfRql56103349 = eoxcUgfRql9524626;     eoxcUgfRql9524626 = eoxcUgfRql50306498;     eoxcUgfRql50306498 = eoxcUgfRql31993287;     eoxcUgfRql31993287 = eoxcUgfRql38006960;     eoxcUgfRql38006960 = eoxcUgfRql33054995;     eoxcUgfRql33054995 = eoxcUgfRql20424783;     eoxcUgfRql20424783 = eoxcUgfRql84942220;     eoxcUgfRql84942220 = eoxcUgfRql24884558;     eoxcUgfRql24884558 = eoxcUgfRql92282964;     eoxcUgfRql92282964 = eoxcUgfRql22901840;     eoxcUgfRql22901840 = eoxcUgfRql42797437;     eoxcUgfRql42797437 = eoxcUgfRql5044431;     eoxcUgfRql5044431 = eoxcUgfRql1463553;     eoxcUgfRql1463553 = eoxcUgfRql9514821;     eoxcUgfRql9514821 = eoxcUgfRql5587536;     eoxcUgfRql5587536 = eoxcUgfRql51381201;     eoxcUgfRql51381201 = eoxcUgfRql62218778;     eoxcUgfRql62218778 = eoxcUgfRql30330665;     eoxcUgfRql30330665 = eoxcUgfRql46413511;     eoxcUgfRql46413511 = eoxcUgfRql31802606;     eoxcUgfRql31802606 = eoxcUgfRql36680218;     eoxcUgfRql36680218 = eoxcUgfRql4985889;     eoxcUgfRql4985889 = eoxcUgfRql35486841;     eoxcUgfRql35486841 = eoxcUgfRql48795677;     eoxcUgfRql48795677 = eoxcUgfRql37474780;     eoxcUgfRql37474780 = eoxcUgfRql90602827;     eoxcUgfRql90602827 = eoxcUgfRql21211452;     eoxcUgfRql21211452 = eoxcUgfRql12635288;     eoxcUgfRql12635288 = eoxcUgfRql92128041;     eoxcUgfRql92128041 = eoxcUgfRql78442650;     eoxcUgfRql78442650 = eoxcUgfRql67059840;     eoxcUgfRql67059840 = eoxcUgfRql16235173;     eoxcUgfRql16235173 = eoxcUgfRql75998414;     eoxcUgfRql75998414 = eoxcUgfRql72809026;     eoxcUgfRql72809026 = eoxcUgfRql20989890;     eoxcUgfRql20989890 = eoxcUgfRql62498651;     eoxcUgfRql62498651 = eoxcUgfRql23719587;     eoxcUgfRql23719587 = eoxcUgfRql50004860;     eoxcUgfRql50004860 = eoxcUgfRql60452739;     eoxcUgfRql60452739 = eoxcUgfRql74752990;     eoxcUgfRql74752990 = eoxcUgfRql54320934;     eoxcUgfRql54320934 = eoxcUgfRql4955090;     eoxcUgfRql4955090 = eoxcUgfRql39136276;     eoxcUgfRql39136276 = eoxcUgfRql19582775;     eoxcUgfRql19582775 = eoxcUgfRql16926953;     eoxcUgfRql16926953 = eoxcUgfRql98118034;     eoxcUgfRql98118034 = eoxcUgfRql65420011;     eoxcUgfRql65420011 = eoxcUgfRql94358966;     eoxcUgfRql94358966 = eoxcUgfRql77987777;     eoxcUgfRql77987777 = eoxcUgfRql63490891;     eoxcUgfRql63490891 = eoxcUgfRql4442703;     eoxcUgfRql4442703 = eoxcUgfRql87663026;     eoxcUgfRql87663026 = eoxcUgfRql71541471;     eoxcUgfRql71541471 = eoxcUgfRql51323362;     eoxcUgfRql51323362 = eoxcUgfRql19423131;     eoxcUgfRql19423131 = eoxcUgfRql4538737;     eoxcUgfRql4538737 = eoxcUgfRql14819658;     eoxcUgfRql14819658 = eoxcUgfRql83197610;     eoxcUgfRql83197610 = eoxcUgfRql532180;     eoxcUgfRql532180 = eoxcUgfRql42452168;     eoxcUgfRql42452168 = eoxcUgfRql99213331;     eoxcUgfRql99213331 = eoxcUgfRql72306932;     eoxcUgfRql72306932 = eoxcUgfRql32756517;     eoxcUgfRql32756517 = eoxcUgfRql13840314;     eoxcUgfRql13840314 = eoxcUgfRql55841999;     eoxcUgfRql55841999 = eoxcUgfRql26562264;     eoxcUgfRql26562264 = eoxcUgfRql29046017;     eoxcUgfRql29046017 = eoxcUgfRql28654526;     eoxcUgfRql28654526 = eoxcUgfRql88524930;     eoxcUgfRql88524930 = eoxcUgfRql43088885;     eoxcUgfRql43088885 = eoxcUgfRql27661615;     eoxcUgfRql27661615 = eoxcUgfRql12213918;     eoxcUgfRql12213918 = eoxcUgfRql69877926;     eoxcUgfRql69877926 = eoxcUgfRql71660520;     eoxcUgfRql71660520 = eoxcUgfRql77481672;     eoxcUgfRql77481672 = eoxcUgfRql31725128;     eoxcUgfRql31725128 = eoxcUgfRql65849613;     eoxcUgfRql65849613 = eoxcUgfRql15904066;     eoxcUgfRql15904066 = eoxcUgfRql31868725;     eoxcUgfRql31868725 = eoxcUgfRql39356746;     eoxcUgfRql39356746 = eoxcUgfRql25182817;     eoxcUgfRql25182817 = eoxcUgfRql26852485;     eoxcUgfRql26852485 = eoxcUgfRql34647511;     eoxcUgfRql34647511 = eoxcUgfRql28637150;     eoxcUgfRql28637150 = eoxcUgfRql73999948;     eoxcUgfRql73999948 = eoxcUgfRql79396814;     eoxcUgfRql79396814 = eoxcUgfRql44693702;     eoxcUgfRql44693702 = eoxcUgfRql24675052;     eoxcUgfRql24675052 = eoxcUgfRql42484615;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void vVeMvMVAvl68064214() {     int tqApDfNFhx64871732 = -47159110;    int tqApDfNFhx44071890 = -767580368;    int tqApDfNFhx31931743 = -715184909;    int tqApDfNFhx81890251 = -560665787;    int tqApDfNFhx2048042 = 53142483;    int tqApDfNFhx98458430 = -662602094;    int tqApDfNFhx7027831 = 8948942;    int tqApDfNFhx42295340 = -568442632;    int tqApDfNFhx15156823 = -90011732;    int tqApDfNFhx9718567 = -464485753;    int tqApDfNFhx20667828 = -295475862;    int tqApDfNFhx12006298 = -256733716;    int tqApDfNFhx76532805 = -858925124;    int tqApDfNFhx92921250 = -23288421;    int tqApDfNFhx68728564 = -563303313;    int tqApDfNFhx82262872 = -997779723;    int tqApDfNFhx82878346 = -883157780;    int tqApDfNFhx26401200 = -437226058;    int tqApDfNFhx26223852 = -939466015;    int tqApDfNFhx3400841 = -153944002;    int tqApDfNFhx93166071 = -179228722;    int tqApDfNFhx86717057 = -540088281;    int tqApDfNFhx46090760 = -750500233;    int tqApDfNFhx31544218 = -475921350;    int tqApDfNFhx37003799 = -56941384;    int tqApDfNFhx97817815 = -770964445;    int tqApDfNFhx52676956 = -567623629;    int tqApDfNFhx57292651 = 7514857;    int tqApDfNFhx69111339 = -930003476;    int tqApDfNFhx39487844 = -874982838;    int tqApDfNFhx90704316 = -643072127;    int tqApDfNFhx89343797 = -811174321;    int tqApDfNFhx17718889 = -628789313;    int tqApDfNFhx64374576 = -176046242;    int tqApDfNFhx11716206 = -235801494;    int tqApDfNFhx90488546 = -522583829;    int tqApDfNFhx30914492 = -783439158;    int tqApDfNFhx44228928 = 90910540;    int tqApDfNFhx69519583 = -710892418;    int tqApDfNFhx32821003 = -565573131;    int tqApDfNFhx20885412 = -445848188;    int tqApDfNFhx66951042 = -807764310;    int tqApDfNFhx72610032 = -232738485;    int tqApDfNFhx15679292 = -551273487;    int tqApDfNFhx43981681 = -235347296;    int tqApDfNFhx80909308 = -725684706;    int tqApDfNFhx33287288 = -805278249;    int tqApDfNFhx7097326 = -883944347;    int tqApDfNFhx18179492 = -718513766;    int tqApDfNFhx47088606 = -44785464;    int tqApDfNFhx69321955 = -334189665;    int tqApDfNFhx48604247 = -531336490;    int tqApDfNFhx32635337 = 5817611;    int tqApDfNFhx65241635 = -782740541;    int tqApDfNFhx74898143 = -581537131;    int tqApDfNFhx78154674 = -507070829;    int tqApDfNFhx97981130 = 82919865;    int tqApDfNFhx387526 = -139263560;    int tqApDfNFhx44886452 = -403724403;    int tqApDfNFhx4230227 = -175893072;    int tqApDfNFhx45781474 = 5021534;    int tqApDfNFhx49735180 = -998565916;    int tqApDfNFhx73184001 = -638439156;    int tqApDfNFhx75668978 = -215028895;    int tqApDfNFhx19014251 = -821413626;    int tqApDfNFhx31324031 = -484301542;    int tqApDfNFhx94287409 = -627944403;    int tqApDfNFhx12158230 = -582878883;    int tqApDfNFhx81205045 = -787486928;    int tqApDfNFhx78240017 = 59280516;    int tqApDfNFhx51348380 = -114340566;    int tqApDfNFhx38649419 = -874068320;    int tqApDfNFhx56881616 = -726333640;    int tqApDfNFhx93402849 = -273892885;    int tqApDfNFhx82515429 = -708095814;    int tqApDfNFhx26215030 = -371464413;    int tqApDfNFhx14107026 = -207349797;    int tqApDfNFhx30411468 = -99226747;    int tqApDfNFhx87562537 = -140574054;    int tqApDfNFhx56094491 = -331256679;    int tqApDfNFhx64530527 = -965686197;    int tqApDfNFhx45579630 = -683679282;    int tqApDfNFhx39113160 = -273971377;    int tqApDfNFhx22022734 = -785218013;    int tqApDfNFhx70165889 = -440793173;    int tqApDfNFhx42100069 = -11735638;    int tqApDfNFhx56708460 = -716991932;    int tqApDfNFhx52477253 = -846048772;    int tqApDfNFhx89476432 = -594509111;    int tqApDfNFhx33561531 = -728730666;    int tqApDfNFhx92507416 = -505503695;    int tqApDfNFhx30526966 = -544175598;    int tqApDfNFhx99342475 = -505365057;    int tqApDfNFhx65289356 = -434999347;    int tqApDfNFhx87039528 = -470594666;    int tqApDfNFhx71150232 = -447282273;    int tqApDfNFhx93767041 = -69325154;    int tqApDfNFhx96941054 = 82290410;    int tqApDfNFhx96665041 = -729859861;    int tqApDfNFhx12657650 = -47159110;     tqApDfNFhx64871732 = tqApDfNFhx44071890;     tqApDfNFhx44071890 = tqApDfNFhx31931743;     tqApDfNFhx31931743 = tqApDfNFhx81890251;     tqApDfNFhx81890251 = tqApDfNFhx2048042;     tqApDfNFhx2048042 = tqApDfNFhx98458430;     tqApDfNFhx98458430 = tqApDfNFhx7027831;     tqApDfNFhx7027831 = tqApDfNFhx42295340;     tqApDfNFhx42295340 = tqApDfNFhx15156823;     tqApDfNFhx15156823 = tqApDfNFhx9718567;     tqApDfNFhx9718567 = tqApDfNFhx20667828;     tqApDfNFhx20667828 = tqApDfNFhx12006298;     tqApDfNFhx12006298 = tqApDfNFhx76532805;     tqApDfNFhx76532805 = tqApDfNFhx92921250;     tqApDfNFhx92921250 = tqApDfNFhx68728564;     tqApDfNFhx68728564 = tqApDfNFhx82262872;     tqApDfNFhx82262872 = tqApDfNFhx82878346;     tqApDfNFhx82878346 = tqApDfNFhx26401200;     tqApDfNFhx26401200 = tqApDfNFhx26223852;     tqApDfNFhx26223852 = tqApDfNFhx3400841;     tqApDfNFhx3400841 = tqApDfNFhx93166071;     tqApDfNFhx93166071 = tqApDfNFhx86717057;     tqApDfNFhx86717057 = tqApDfNFhx46090760;     tqApDfNFhx46090760 = tqApDfNFhx31544218;     tqApDfNFhx31544218 = tqApDfNFhx37003799;     tqApDfNFhx37003799 = tqApDfNFhx97817815;     tqApDfNFhx97817815 = tqApDfNFhx52676956;     tqApDfNFhx52676956 = tqApDfNFhx57292651;     tqApDfNFhx57292651 = tqApDfNFhx69111339;     tqApDfNFhx69111339 = tqApDfNFhx39487844;     tqApDfNFhx39487844 = tqApDfNFhx90704316;     tqApDfNFhx90704316 = tqApDfNFhx89343797;     tqApDfNFhx89343797 = tqApDfNFhx17718889;     tqApDfNFhx17718889 = tqApDfNFhx64374576;     tqApDfNFhx64374576 = tqApDfNFhx11716206;     tqApDfNFhx11716206 = tqApDfNFhx90488546;     tqApDfNFhx90488546 = tqApDfNFhx30914492;     tqApDfNFhx30914492 = tqApDfNFhx44228928;     tqApDfNFhx44228928 = tqApDfNFhx69519583;     tqApDfNFhx69519583 = tqApDfNFhx32821003;     tqApDfNFhx32821003 = tqApDfNFhx20885412;     tqApDfNFhx20885412 = tqApDfNFhx66951042;     tqApDfNFhx66951042 = tqApDfNFhx72610032;     tqApDfNFhx72610032 = tqApDfNFhx15679292;     tqApDfNFhx15679292 = tqApDfNFhx43981681;     tqApDfNFhx43981681 = tqApDfNFhx80909308;     tqApDfNFhx80909308 = tqApDfNFhx33287288;     tqApDfNFhx33287288 = tqApDfNFhx7097326;     tqApDfNFhx7097326 = tqApDfNFhx18179492;     tqApDfNFhx18179492 = tqApDfNFhx47088606;     tqApDfNFhx47088606 = tqApDfNFhx69321955;     tqApDfNFhx69321955 = tqApDfNFhx48604247;     tqApDfNFhx48604247 = tqApDfNFhx32635337;     tqApDfNFhx32635337 = tqApDfNFhx65241635;     tqApDfNFhx65241635 = tqApDfNFhx74898143;     tqApDfNFhx74898143 = tqApDfNFhx78154674;     tqApDfNFhx78154674 = tqApDfNFhx97981130;     tqApDfNFhx97981130 = tqApDfNFhx387526;     tqApDfNFhx387526 = tqApDfNFhx44886452;     tqApDfNFhx44886452 = tqApDfNFhx4230227;     tqApDfNFhx4230227 = tqApDfNFhx45781474;     tqApDfNFhx45781474 = tqApDfNFhx49735180;     tqApDfNFhx49735180 = tqApDfNFhx73184001;     tqApDfNFhx73184001 = tqApDfNFhx75668978;     tqApDfNFhx75668978 = tqApDfNFhx19014251;     tqApDfNFhx19014251 = tqApDfNFhx31324031;     tqApDfNFhx31324031 = tqApDfNFhx94287409;     tqApDfNFhx94287409 = tqApDfNFhx12158230;     tqApDfNFhx12158230 = tqApDfNFhx81205045;     tqApDfNFhx81205045 = tqApDfNFhx78240017;     tqApDfNFhx78240017 = tqApDfNFhx51348380;     tqApDfNFhx51348380 = tqApDfNFhx38649419;     tqApDfNFhx38649419 = tqApDfNFhx56881616;     tqApDfNFhx56881616 = tqApDfNFhx93402849;     tqApDfNFhx93402849 = tqApDfNFhx82515429;     tqApDfNFhx82515429 = tqApDfNFhx26215030;     tqApDfNFhx26215030 = tqApDfNFhx14107026;     tqApDfNFhx14107026 = tqApDfNFhx30411468;     tqApDfNFhx30411468 = tqApDfNFhx87562537;     tqApDfNFhx87562537 = tqApDfNFhx56094491;     tqApDfNFhx56094491 = tqApDfNFhx64530527;     tqApDfNFhx64530527 = tqApDfNFhx45579630;     tqApDfNFhx45579630 = tqApDfNFhx39113160;     tqApDfNFhx39113160 = tqApDfNFhx22022734;     tqApDfNFhx22022734 = tqApDfNFhx70165889;     tqApDfNFhx70165889 = tqApDfNFhx42100069;     tqApDfNFhx42100069 = tqApDfNFhx56708460;     tqApDfNFhx56708460 = tqApDfNFhx52477253;     tqApDfNFhx52477253 = tqApDfNFhx89476432;     tqApDfNFhx89476432 = tqApDfNFhx33561531;     tqApDfNFhx33561531 = tqApDfNFhx92507416;     tqApDfNFhx92507416 = tqApDfNFhx30526966;     tqApDfNFhx30526966 = tqApDfNFhx99342475;     tqApDfNFhx99342475 = tqApDfNFhx65289356;     tqApDfNFhx65289356 = tqApDfNFhx87039528;     tqApDfNFhx87039528 = tqApDfNFhx71150232;     tqApDfNFhx71150232 = tqApDfNFhx93767041;     tqApDfNFhx93767041 = tqApDfNFhx96941054;     tqApDfNFhx96941054 = tqApDfNFhx96665041;     tqApDfNFhx96665041 = tqApDfNFhx12657650;     tqApDfNFhx12657650 = tqApDfNFhx64871732;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ENiIEMdEbR4709190() {     int HuRMJRptnJ87258849 = -781563073;    int HuRMJRptnJ28419390 = -716123422;    int HuRMJRptnJ60701022 = -954838753;    int HuRMJRptnJ96896939 = -567073319;    int HuRMJRptnJ222296 = -537914518;    int HuRMJRptnJ13341548 = -523210472;    int HuRMJRptnJ99183569 = -913038364;    int HuRMJRptnJ17929199 = 60960724;    int HuRMJRptnJ12319954 = 25792229;    int HuRMJRptnJ1482152 = -207227086;    int HuRMJRptnJ58209687 = -292100263;    int HuRMJRptnJ67909246 = -507479009;    int HuRMJRptnJ43540986 = -377642443;    int HuRMJRptnJ35536003 = -550997178;    int HuRMJRptnJ5463842 = -190781026;    int HuRMJRptnJ26518785 = -279447266;    int HuRMJRptnJ32701698 = -60437600;    int HuRMJRptnJ32377616 = -888968749;    int HuRMJRptnJ67505482 = 25431860;    int HuRMJRptnJ81917124 = -406578456;    int HuRMJRptnJ94049178 = -170716031;    int HuRMJRptnJ50532276 = 55145405;    int HuRMJRptnJ49384083 = -926580825;    int HuRMJRptnJ58044005 = 60085793;    int HuRMJRptnJ72544046 = -778500509;    int HuRMJRptnJ86120809 = -114858669;    int HuRMJRptnJ99766376 = -325210122;    int HuRMJRptnJ63204101 = -880939596;    int HuRMJRptnJ76003900 = -174260595;    int HuRMJRptnJ48645024 = -795278704;    int HuRMJRptnJ34995121 = -724388279;    int HuRMJRptnJ46884988 = -363245675;    int HuRMJRptnJ98757558 = -58132808;    int HuRMJRptnJ23763263 = -500173092;    int HuRMJRptnJ87945569 = -299557917;    int HuRMJRptnJ32181416 = -837979206;    int HuRMJRptnJ24354204 = -516387109;    int HuRMJRptnJ97855028 = -876714932;    int HuRMJRptnJ17827715 = -11938450;    int HuRMJRptnJ53006717 = -701797160;    int HuRMJRptnJ49642783 = -949025854;    int HuRMJRptnJ55459433 = -565673001;    int HuRMJRptnJ78160224 = -120269069;    int HuRMJRptnJ15123412 = -548637136;    int HuRMJRptnJ11964947 = -44490774;    int HuRMJRptnJ89009590 = -907913801;    int HuRMJRptnJ45584685 = -283688502;    int HuRMJRptnJ51696000 = -640479338;    int HuRMJRptnJ12639397 = -668230464;    int HuRMJRptnJ44172352 = -186805723;    int HuRMJRptnJ78191172 = -689265942;    int HuRMJRptnJ22455505 = -962155772;    int HuRMJRptnJ10949741 = -766093619;    int HuRMJRptnJ25528181 = 41578650;    int HuRMJRptnJ10660011 = -722241246;    int HuRMJRptnJ36726573 = -736708479;    int HuRMJRptnJ79035307 = -789542598;    int HuRMJRptnJ2657018 = -914924546;    int HuRMJRptnJ24352894 = -788572810;    int HuRMJRptnJ14101486 = -323055849;    int HuRMJRptnJ13575171 = -98000351;    int HuRMJRptnJ35979468 = 67901232;    int HuRMJRptnJ41925299 = -764778682;    int HuRMJRptnJ63674930 = -178929068;    int HuRMJRptnJ66487030 = -482838807;    int HuRMJRptnJ11324700 = -928854588;    int HuRMJRptnJ69151688 = -349346202;    int HuRMJRptnJ19777723 = -877469352;    int HuRMJRptnJ47590433 = -151439262;    int HuRMJRptnJ73282425 = -352801821;    int HuRMJRptnJ2164581 = -763060157;    int HuRMJRptnJ34846670 = -183722668;    int HuRMJRptnJ14549902 = -777030299;    int HuRMJRptnJ14498766 = -272770980;    int HuRMJRptnJ32274342 = -457552602;    int HuRMJRptnJ38589746 = -605043030;    int HuRMJRptnJ72372051 = -824585526;    int HuRMJRptnJ34260672 = -277943690;    int HuRMJRptnJ46079058 = -895423433;    int HuRMJRptnJ83534455 = -870586708;    int HuRMJRptnJ40536125 = -831170168;    int HuRMJRptnJ48070376 = -684730784;    int HuRMJRptnJ50564705 = -112709132;    int HuRMJRptnJ31831549 = -987454873;    int HuRMJRptnJ70453851 = -6012763;    int HuRMJRptnJ12539617 = -762232508;    int HuRMJRptnJ35935248 = -597152057;    int HuRMJRptnJ73229378 = 288542;    int HuRMJRptnJ13103253 = -777931846;    int HuRMJRptnJ51218997 = -562849439;    int HuRMJRptnJ53146108 = 51563391;    int HuRMJRptnJ21697187 = -601462563;    int HuRMJRptnJ73502135 = 11857878;    int HuRMJRptnJ3726229 = -688882601;    int HuRMJRptnJ39431547 = -503796810;    int HuRMJRptnJ13663315 = -916927086;    int HuRMJRptnJ13534135 = -800894320;    int HuRMJRptnJ14485295 = -941340002;    int HuRMJRptnJ48636381 = 34201671;    int HuRMJRptnJ640248 = -781563073;     HuRMJRptnJ87258849 = HuRMJRptnJ28419390;     HuRMJRptnJ28419390 = HuRMJRptnJ60701022;     HuRMJRptnJ60701022 = HuRMJRptnJ96896939;     HuRMJRptnJ96896939 = HuRMJRptnJ222296;     HuRMJRptnJ222296 = HuRMJRptnJ13341548;     HuRMJRptnJ13341548 = HuRMJRptnJ99183569;     HuRMJRptnJ99183569 = HuRMJRptnJ17929199;     HuRMJRptnJ17929199 = HuRMJRptnJ12319954;     HuRMJRptnJ12319954 = HuRMJRptnJ1482152;     HuRMJRptnJ1482152 = HuRMJRptnJ58209687;     HuRMJRptnJ58209687 = HuRMJRptnJ67909246;     HuRMJRptnJ67909246 = HuRMJRptnJ43540986;     HuRMJRptnJ43540986 = HuRMJRptnJ35536003;     HuRMJRptnJ35536003 = HuRMJRptnJ5463842;     HuRMJRptnJ5463842 = HuRMJRptnJ26518785;     HuRMJRptnJ26518785 = HuRMJRptnJ32701698;     HuRMJRptnJ32701698 = HuRMJRptnJ32377616;     HuRMJRptnJ32377616 = HuRMJRptnJ67505482;     HuRMJRptnJ67505482 = HuRMJRptnJ81917124;     HuRMJRptnJ81917124 = HuRMJRptnJ94049178;     HuRMJRptnJ94049178 = HuRMJRptnJ50532276;     HuRMJRptnJ50532276 = HuRMJRptnJ49384083;     HuRMJRptnJ49384083 = HuRMJRptnJ58044005;     HuRMJRptnJ58044005 = HuRMJRptnJ72544046;     HuRMJRptnJ72544046 = HuRMJRptnJ86120809;     HuRMJRptnJ86120809 = HuRMJRptnJ99766376;     HuRMJRptnJ99766376 = HuRMJRptnJ63204101;     HuRMJRptnJ63204101 = HuRMJRptnJ76003900;     HuRMJRptnJ76003900 = HuRMJRptnJ48645024;     HuRMJRptnJ48645024 = HuRMJRptnJ34995121;     HuRMJRptnJ34995121 = HuRMJRptnJ46884988;     HuRMJRptnJ46884988 = HuRMJRptnJ98757558;     HuRMJRptnJ98757558 = HuRMJRptnJ23763263;     HuRMJRptnJ23763263 = HuRMJRptnJ87945569;     HuRMJRptnJ87945569 = HuRMJRptnJ32181416;     HuRMJRptnJ32181416 = HuRMJRptnJ24354204;     HuRMJRptnJ24354204 = HuRMJRptnJ97855028;     HuRMJRptnJ97855028 = HuRMJRptnJ17827715;     HuRMJRptnJ17827715 = HuRMJRptnJ53006717;     HuRMJRptnJ53006717 = HuRMJRptnJ49642783;     HuRMJRptnJ49642783 = HuRMJRptnJ55459433;     HuRMJRptnJ55459433 = HuRMJRptnJ78160224;     HuRMJRptnJ78160224 = HuRMJRptnJ15123412;     HuRMJRptnJ15123412 = HuRMJRptnJ11964947;     HuRMJRptnJ11964947 = HuRMJRptnJ89009590;     HuRMJRptnJ89009590 = HuRMJRptnJ45584685;     HuRMJRptnJ45584685 = HuRMJRptnJ51696000;     HuRMJRptnJ51696000 = HuRMJRptnJ12639397;     HuRMJRptnJ12639397 = HuRMJRptnJ44172352;     HuRMJRptnJ44172352 = HuRMJRptnJ78191172;     HuRMJRptnJ78191172 = HuRMJRptnJ22455505;     HuRMJRptnJ22455505 = HuRMJRptnJ10949741;     HuRMJRptnJ10949741 = HuRMJRptnJ25528181;     HuRMJRptnJ25528181 = HuRMJRptnJ10660011;     HuRMJRptnJ10660011 = HuRMJRptnJ36726573;     HuRMJRptnJ36726573 = HuRMJRptnJ79035307;     HuRMJRptnJ79035307 = HuRMJRptnJ2657018;     HuRMJRptnJ2657018 = HuRMJRptnJ24352894;     HuRMJRptnJ24352894 = HuRMJRptnJ14101486;     HuRMJRptnJ14101486 = HuRMJRptnJ13575171;     HuRMJRptnJ13575171 = HuRMJRptnJ35979468;     HuRMJRptnJ35979468 = HuRMJRptnJ41925299;     HuRMJRptnJ41925299 = HuRMJRptnJ63674930;     HuRMJRptnJ63674930 = HuRMJRptnJ66487030;     HuRMJRptnJ66487030 = HuRMJRptnJ11324700;     HuRMJRptnJ11324700 = HuRMJRptnJ69151688;     HuRMJRptnJ69151688 = HuRMJRptnJ19777723;     HuRMJRptnJ19777723 = HuRMJRptnJ47590433;     HuRMJRptnJ47590433 = HuRMJRptnJ73282425;     HuRMJRptnJ73282425 = HuRMJRptnJ2164581;     HuRMJRptnJ2164581 = HuRMJRptnJ34846670;     HuRMJRptnJ34846670 = HuRMJRptnJ14549902;     HuRMJRptnJ14549902 = HuRMJRptnJ14498766;     HuRMJRptnJ14498766 = HuRMJRptnJ32274342;     HuRMJRptnJ32274342 = HuRMJRptnJ38589746;     HuRMJRptnJ38589746 = HuRMJRptnJ72372051;     HuRMJRptnJ72372051 = HuRMJRptnJ34260672;     HuRMJRptnJ34260672 = HuRMJRptnJ46079058;     HuRMJRptnJ46079058 = HuRMJRptnJ83534455;     HuRMJRptnJ83534455 = HuRMJRptnJ40536125;     HuRMJRptnJ40536125 = HuRMJRptnJ48070376;     HuRMJRptnJ48070376 = HuRMJRptnJ50564705;     HuRMJRptnJ50564705 = HuRMJRptnJ31831549;     HuRMJRptnJ31831549 = HuRMJRptnJ70453851;     HuRMJRptnJ70453851 = HuRMJRptnJ12539617;     HuRMJRptnJ12539617 = HuRMJRptnJ35935248;     HuRMJRptnJ35935248 = HuRMJRptnJ73229378;     HuRMJRptnJ73229378 = HuRMJRptnJ13103253;     HuRMJRptnJ13103253 = HuRMJRptnJ51218997;     HuRMJRptnJ51218997 = HuRMJRptnJ53146108;     HuRMJRptnJ53146108 = HuRMJRptnJ21697187;     HuRMJRptnJ21697187 = HuRMJRptnJ73502135;     HuRMJRptnJ73502135 = HuRMJRptnJ3726229;     HuRMJRptnJ3726229 = HuRMJRptnJ39431547;     HuRMJRptnJ39431547 = HuRMJRptnJ13663315;     HuRMJRptnJ13663315 = HuRMJRptnJ13534135;     HuRMJRptnJ13534135 = HuRMJRptnJ14485295;     HuRMJRptnJ14485295 = HuRMJRptnJ48636381;     HuRMJRptnJ48636381 = HuRMJRptnJ640248;     HuRMJRptnJ640248 = HuRMJRptnJ87258849;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void XImliEihJe41354164() {     int PGoKmoaojH9645967 = -415967036;    int PGoKmoaojH12766891 = -664666477;    int PGoKmoaojH89470300 = -94492597;    int PGoKmoaojH11903629 = -573480850;    int PGoKmoaojH98396549 = -28971519;    int PGoKmoaojH28224664 = -383818850;    int PGoKmoaojH91339308 = -735025670;    int PGoKmoaojH93563057 = -409635920;    int PGoKmoaojH9483085 = -958403810;    int PGoKmoaojH93245735 = 50031581;    int PGoKmoaojH95751546 = -288724664;    int PGoKmoaojH23812196 = -758224302;    int PGoKmoaojH10549167 = -996359763;    int PGoKmoaojH78150754 = 21294065;    int PGoKmoaojH42199118 = -918258739;    int PGoKmoaojH70774697 = -661114808;    int PGoKmoaojH82525050 = -337717419;    int PGoKmoaojH38354032 = -240711439;    int PGoKmoaojH8787114 = -109670265;    int PGoKmoaojH60433407 = -659212909;    int PGoKmoaojH94932285 = -162203339;    int PGoKmoaojH14347495 = -449620908;    int PGoKmoaojH52677406 = -2661417;    int PGoKmoaojH84543791 = -503907064;    int PGoKmoaojH8084293 = -400059633;    int PGoKmoaojH74423804 = -558752893;    int PGoKmoaojH46855797 = -82796615;    int PGoKmoaojH69115551 = -669394050;    int PGoKmoaojH82896461 = -518517714;    int PGoKmoaojH57802203 = -715574571;    int PGoKmoaojH79285926 = -805704431;    int PGoKmoaojH4426179 = 84682970;    int PGoKmoaojH79796229 = -587476303;    int PGoKmoaojH83151950 = -824299942;    int PGoKmoaojH64174934 = -363314339;    int PGoKmoaojH73874284 = -53374582;    int PGoKmoaojH17793917 = -249335061;    int PGoKmoaojH51481130 = -744340404;    int PGoKmoaojH66135845 = -412984482;    int PGoKmoaojH73192431 = -838021190;    int PGoKmoaojH78400153 = -352203520;    int PGoKmoaojH43967824 = -323581692;    int PGoKmoaojH83710416 = -7799652;    int PGoKmoaojH14567531 = -546000785;    int PGoKmoaojH79948213 = -953634251;    int PGoKmoaojH97109872 = 9857103;    int PGoKmoaojH57882082 = -862098755;    int PGoKmoaojH96294675 = -397014329;    int PGoKmoaojH7099302 = -617947163;    int PGoKmoaojH41256098 = -328825982;    int PGoKmoaojH87060388 = 55657781;    int PGoKmoaojH96306761 = -292975054;    int PGoKmoaojH89264143 = -438004849;    int PGoKmoaojH85814725 = -234102159;    int PGoKmoaojH46421878 = -862945361;    int PGoKmoaojH95298472 = -966346129;    int PGoKmoaojH60089484 = -562005060;    int PGoKmoaojH4926509 = -590585533;    int PGoKmoaojH3819336 = -73421217;    int PGoKmoaojH23972746 = -470218626;    int PGoKmoaojH81368867 = -201022236;    int PGoKmoaojH22223757 = 34368379;    int PGoKmoaojH10666597 = -891118207;    int PGoKmoaojH51680881 = -142829240;    int PGoKmoaojH13959810 = -144263988;    int PGoKmoaojH91325368 = -273407635;    int PGoKmoaojH44015966 = -70748000;    int PGoKmoaojH27397217 = -72059822;    int PGoKmoaojH13975821 = -615391597;    int PGoKmoaojH68324833 = -764884157;    int PGoKmoaojH52980781 = -311779748;    int PGoKmoaojH31043920 = -593377016;    int PGoKmoaojH72218186 = -827726958;    int PGoKmoaojH35594682 = -271649076;    int PGoKmoaojH82033254 = -207009389;    int PGoKmoaojH50964461 = -838621648;    int PGoKmoaojH30637078 = -341821256;    int PGoKmoaojH38109876 = -456660633;    int PGoKmoaojH4595579 = -550272813;    int PGoKmoaojH10974421 = -309916737;    int PGoKmoaojH16541722 = -696654139;    int PGoKmoaojH50561122 = -685782286;    int PGoKmoaojH62016250 = 48553112;    int PGoKmoaojH41640364 = -89691733;    int PGoKmoaojH70741814 = -671232352;    int PGoKmoaojH82979164 = -412729378;    int PGoKmoaojH15162036 = -477312181;    int PGoKmoaojH93981503 = -253374144;    int PGoKmoaojH36730073 = -961354581;    int PGoKmoaojH68876462 = -396968211;    int PGoKmoaojH13784801 = -491369522;    int PGoKmoaojH12867408 = -658749528;    int PGoKmoaojH47661794 = -570919187;    int PGoKmoaojH42163100 = -942765856;    int PGoKmoaojH91823564 = -536998954;    int PGoKmoaojH56176397 = -286571900;    int PGoKmoaojH33301228 = -432463486;    int PGoKmoaojH32029535 = -864970413;    int PGoKmoaojH607722 = -301736797;    int PGoKmoaojH88622845 = -415967036;     PGoKmoaojH9645967 = PGoKmoaojH12766891;     PGoKmoaojH12766891 = PGoKmoaojH89470300;     PGoKmoaojH89470300 = PGoKmoaojH11903629;     PGoKmoaojH11903629 = PGoKmoaojH98396549;     PGoKmoaojH98396549 = PGoKmoaojH28224664;     PGoKmoaojH28224664 = PGoKmoaojH91339308;     PGoKmoaojH91339308 = PGoKmoaojH93563057;     PGoKmoaojH93563057 = PGoKmoaojH9483085;     PGoKmoaojH9483085 = PGoKmoaojH93245735;     PGoKmoaojH93245735 = PGoKmoaojH95751546;     PGoKmoaojH95751546 = PGoKmoaojH23812196;     PGoKmoaojH23812196 = PGoKmoaojH10549167;     PGoKmoaojH10549167 = PGoKmoaojH78150754;     PGoKmoaojH78150754 = PGoKmoaojH42199118;     PGoKmoaojH42199118 = PGoKmoaojH70774697;     PGoKmoaojH70774697 = PGoKmoaojH82525050;     PGoKmoaojH82525050 = PGoKmoaojH38354032;     PGoKmoaojH38354032 = PGoKmoaojH8787114;     PGoKmoaojH8787114 = PGoKmoaojH60433407;     PGoKmoaojH60433407 = PGoKmoaojH94932285;     PGoKmoaojH94932285 = PGoKmoaojH14347495;     PGoKmoaojH14347495 = PGoKmoaojH52677406;     PGoKmoaojH52677406 = PGoKmoaojH84543791;     PGoKmoaojH84543791 = PGoKmoaojH8084293;     PGoKmoaojH8084293 = PGoKmoaojH74423804;     PGoKmoaojH74423804 = PGoKmoaojH46855797;     PGoKmoaojH46855797 = PGoKmoaojH69115551;     PGoKmoaojH69115551 = PGoKmoaojH82896461;     PGoKmoaojH82896461 = PGoKmoaojH57802203;     PGoKmoaojH57802203 = PGoKmoaojH79285926;     PGoKmoaojH79285926 = PGoKmoaojH4426179;     PGoKmoaojH4426179 = PGoKmoaojH79796229;     PGoKmoaojH79796229 = PGoKmoaojH83151950;     PGoKmoaojH83151950 = PGoKmoaojH64174934;     PGoKmoaojH64174934 = PGoKmoaojH73874284;     PGoKmoaojH73874284 = PGoKmoaojH17793917;     PGoKmoaojH17793917 = PGoKmoaojH51481130;     PGoKmoaojH51481130 = PGoKmoaojH66135845;     PGoKmoaojH66135845 = PGoKmoaojH73192431;     PGoKmoaojH73192431 = PGoKmoaojH78400153;     PGoKmoaojH78400153 = PGoKmoaojH43967824;     PGoKmoaojH43967824 = PGoKmoaojH83710416;     PGoKmoaojH83710416 = PGoKmoaojH14567531;     PGoKmoaojH14567531 = PGoKmoaojH79948213;     PGoKmoaojH79948213 = PGoKmoaojH97109872;     PGoKmoaojH97109872 = PGoKmoaojH57882082;     PGoKmoaojH57882082 = PGoKmoaojH96294675;     PGoKmoaojH96294675 = PGoKmoaojH7099302;     PGoKmoaojH7099302 = PGoKmoaojH41256098;     PGoKmoaojH41256098 = PGoKmoaojH87060388;     PGoKmoaojH87060388 = PGoKmoaojH96306761;     PGoKmoaojH96306761 = PGoKmoaojH89264143;     PGoKmoaojH89264143 = PGoKmoaojH85814725;     PGoKmoaojH85814725 = PGoKmoaojH46421878;     PGoKmoaojH46421878 = PGoKmoaojH95298472;     PGoKmoaojH95298472 = PGoKmoaojH60089484;     PGoKmoaojH60089484 = PGoKmoaojH4926509;     PGoKmoaojH4926509 = PGoKmoaojH3819336;     PGoKmoaojH3819336 = PGoKmoaojH23972746;     PGoKmoaojH23972746 = PGoKmoaojH81368867;     PGoKmoaojH81368867 = PGoKmoaojH22223757;     PGoKmoaojH22223757 = PGoKmoaojH10666597;     PGoKmoaojH10666597 = PGoKmoaojH51680881;     PGoKmoaojH51680881 = PGoKmoaojH13959810;     PGoKmoaojH13959810 = PGoKmoaojH91325368;     PGoKmoaojH91325368 = PGoKmoaojH44015966;     PGoKmoaojH44015966 = PGoKmoaojH27397217;     PGoKmoaojH27397217 = PGoKmoaojH13975821;     PGoKmoaojH13975821 = PGoKmoaojH68324833;     PGoKmoaojH68324833 = PGoKmoaojH52980781;     PGoKmoaojH52980781 = PGoKmoaojH31043920;     PGoKmoaojH31043920 = PGoKmoaojH72218186;     PGoKmoaojH72218186 = PGoKmoaojH35594682;     PGoKmoaojH35594682 = PGoKmoaojH82033254;     PGoKmoaojH82033254 = PGoKmoaojH50964461;     PGoKmoaojH50964461 = PGoKmoaojH30637078;     PGoKmoaojH30637078 = PGoKmoaojH38109876;     PGoKmoaojH38109876 = PGoKmoaojH4595579;     PGoKmoaojH4595579 = PGoKmoaojH10974421;     PGoKmoaojH10974421 = PGoKmoaojH16541722;     PGoKmoaojH16541722 = PGoKmoaojH50561122;     PGoKmoaojH50561122 = PGoKmoaojH62016250;     PGoKmoaojH62016250 = PGoKmoaojH41640364;     PGoKmoaojH41640364 = PGoKmoaojH70741814;     PGoKmoaojH70741814 = PGoKmoaojH82979164;     PGoKmoaojH82979164 = PGoKmoaojH15162036;     PGoKmoaojH15162036 = PGoKmoaojH93981503;     PGoKmoaojH93981503 = PGoKmoaojH36730073;     PGoKmoaojH36730073 = PGoKmoaojH68876462;     PGoKmoaojH68876462 = PGoKmoaojH13784801;     PGoKmoaojH13784801 = PGoKmoaojH12867408;     PGoKmoaojH12867408 = PGoKmoaojH47661794;     PGoKmoaojH47661794 = PGoKmoaojH42163100;     PGoKmoaojH42163100 = PGoKmoaojH91823564;     PGoKmoaojH91823564 = PGoKmoaojH56176397;     PGoKmoaojH56176397 = PGoKmoaojH33301228;     PGoKmoaojH33301228 = PGoKmoaojH32029535;     PGoKmoaojH32029535 = PGoKmoaojH607722;     PGoKmoaojH607722 = PGoKmoaojH88622845;     PGoKmoaojH88622845 = PGoKmoaojH9645967;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void QEmLoaCnym77999138() {     int UtayLCDKTZ32033084 = -50370999;    int UtayLCDKTZ97114390 = -613209532;    int UtayLCDKTZ18239580 = -334146441;    int UtayLCDKTZ26910317 = -579888382;    int UtayLCDKTZ96570804 = -620028520;    int UtayLCDKTZ43107781 = -244427228;    int UtayLCDKTZ83495047 = -557012976;    int UtayLCDKTZ69196916 = -880232565;    int UtayLCDKTZ6646216 = -842599850;    int UtayLCDKTZ85009320 = -792709752;    int UtayLCDKTZ33293407 = -285349065;    int UtayLCDKTZ79715144 = 91030404;    int UtayLCDKTZ77557347 = -515077083;    int UtayLCDKTZ20765507 = -506414692;    int UtayLCDKTZ78934395 = -545736452;    int UtayLCDKTZ15030610 = 57217650;    int UtayLCDKTZ32348402 = -614997239;    int UtayLCDKTZ44330448 = -692454130;    int UtayLCDKTZ50068744 = -244772391;    int UtayLCDKTZ38949691 = -911847363;    int UtayLCDKTZ95815393 = -153690648;    int UtayLCDKTZ78162713 = -954387221;    int UtayLCDKTZ55970729 = -178742009;    int UtayLCDKTZ11043579 = 32100079;    int UtayLCDKTZ43624540 = -21618758;    int UtayLCDKTZ62726799 = 97352883;    int UtayLCDKTZ93945217 = -940383108;    int UtayLCDKTZ75027001 = -457848503;    int UtayLCDKTZ89789022 = -862774833;    int UtayLCDKTZ66959382 = -635870437;    int UtayLCDKTZ23576732 = -887020583;    int UtayLCDKTZ61967369 = -567388385;    int UtayLCDKTZ60834899 = -16819797;    int UtayLCDKTZ42540638 = -48426792;    int UtayLCDKTZ40404299 = -427070762;    int UtayLCDKTZ15567154 = -368769959;    int UtayLCDKTZ11233629 = 17716988;    int UtayLCDKTZ5107231 = -611965876;    int UtayLCDKTZ14443977 = -814030513;    int UtayLCDKTZ93378146 = -974245219;    int UtayLCDKTZ7157525 = -855381186;    int UtayLCDKTZ32476216 = -81490383;    int UtayLCDKTZ89260608 = -995330236;    int UtayLCDKTZ14011650 = -543364434;    int UtayLCDKTZ47931480 = -762777729;    int UtayLCDKTZ5210155 = -172371992;    int UtayLCDKTZ70179479 = -340509008;    int UtayLCDKTZ40893350 = -153549320;    int UtayLCDKTZ1559207 = -567663861;    int UtayLCDKTZ38339843 = -470846241;    int UtayLCDKTZ95929604 = -299418496;    int UtayLCDKTZ70158019 = -723794336;    int UtayLCDKTZ67578546 = -109916079;    int UtayLCDKTZ46101271 = -509782968;    int UtayLCDKTZ82183745 = 96350524;    int UtayLCDKTZ53870371 = -95983778;    int UtayLCDKTZ41143662 = -334467523;    int UtayLCDKTZ7196001 = -266246520;    int UtayLCDKTZ83285777 = -458269625;    int UtayLCDKTZ33844005 = -617381404;    int UtayLCDKTZ49162563 = -304044121;    int UtayLCDKTZ8468046 = 835526;    int UtayLCDKTZ79407894 = 82542268;    int UtayLCDKTZ39686833 = -106729413;    int UtayLCDKTZ61432589 = -905689169;    int UtayLCDKTZ71326037 = -717960681;    int UtayLCDKTZ18880245 = -892149799;    int UtayLCDKTZ35016710 = -366650292;    int UtayLCDKTZ80361208 = 20656069;    int UtayLCDKTZ63367241 = -76966494;    int UtayLCDKTZ3796982 = -960499339;    int UtayLCDKTZ27241171 = 96968636;    int UtayLCDKTZ29886472 = -878423617;    int UtayLCDKTZ56690598 = -270527172;    int UtayLCDKTZ31792166 = 43533823;    int UtayLCDKTZ63339177 = 27799735;    int UtayLCDKTZ88902104 = -959056985;    int UtayLCDKTZ41959080 = -635377576;    int UtayLCDKTZ63112099 = -205122193;    int UtayLCDKTZ38414386 = -849246766;    int UtayLCDKTZ92547319 = -562138110;    int UtayLCDKTZ53051868 = -686833788;    int UtayLCDKTZ73467795 = -890184643;    int UtayLCDKTZ51449179 = -291928593;    int UtayLCDKTZ71029777 = -236451942;    int UtayLCDKTZ53418713 = -63226248;    int UtayLCDKTZ94388823 = -357472306;    int UtayLCDKTZ14733629 = -507036830;    int UtayLCDKTZ60356892 = -44777316;    int UtayLCDKTZ86533928 = -231086984;    int UtayLCDKTZ74423492 = 65697564;    int UtayLCDKTZ4037629 = -716036492;    int UtayLCDKTZ21821454 = -53696252;    int UtayLCDKTZ80599971 = -96649110;    int UtayLCDKTZ44215583 = -570201099;    int UtayLCDKTZ98689478 = -756216713;    int UtayLCDKTZ53068321 = -64032652;    int UtayLCDKTZ49573776 = -788600824;    int UtayLCDKTZ52579061 = -637675265;    int UtayLCDKTZ76605443 = -50370999;     UtayLCDKTZ32033084 = UtayLCDKTZ97114390;     UtayLCDKTZ97114390 = UtayLCDKTZ18239580;     UtayLCDKTZ18239580 = UtayLCDKTZ26910317;     UtayLCDKTZ26910317 = UtayLCDKTZ96570804;     UtayLCDKTZ96570804 = UtayLCDKTZ43107781;     UtayLCDKTZ43107781 = UtayLCDKTZ83495047;     UtayLCDKTZ83495047 = UtayLCDKTZ69196916;     UtayLCDKTZ69196916 = UtayLCDKTZ6646216;     UtayLCDKTZ6646216 = UtayLCDKTZ85009320;     UtayLCDKTZ85009320 = UtayLCDKTZ33293407;     UtayLCDKTZ33293407 = UtayLCDKTZ79715144;     UtayLCDKTZ79715144 = UtayLCDKTZ77557347;     UtayLCDKTZ77557347 = UtayLCDKTZ20765507;     UtayLCDKTZ20765507 = UtayLCDKTZ78934395;     UtayLCDKTZ78934395 = UtayLCDKTZ15030610;     UtayLCDKTZ15030610 = UtayLCDKTZ32348402;     UtayLCDKTZ32348402 = UtayLCDKTZ44330448;     UtayLCDKTZ44330448 = UtayLCDKTZ50068744;     UtayLCDKTZ50068744 = UtayLCDKTZ38949691;     UtayLCDKTZ38949691 = UtayLCDKTZ95815393;     UtayLCDKTZ95815393 = UtayLCDKTZ78162713;     UtayLCDKTZ78162713 = UtayLCDKTZ55970729;     UtayLCDKTZ55970729 = UtayLCDKTZ11043579;     UtayLCDKTZ11043579 = UtayLCDKTZ43624540;     UtayLCDKTZ43624540 = UtayLCDKTZ62726799;     UtayLCDKTZ62726799 = UtayLCDKTZ93945217;     UtayLCDKTZ93945217 = UtayLCDKTZ75027001;     UtayLCDKTZ75027001 = UtayLCDKTZ89789022;     UtayLCDKTZ89789022 = UtayLCDKTZ66959382;     UtayLCDKTZ66959382 = UtayLCDKTZ23576732;     UtayLCDKTZ23576732 = UtayLCDKTZ61967369;     UtayLCDKTZ61967369 = UtayLCDKTZ60834899;     UtayLCDKTZ60834899 = UtayLCDKTZ42540638;     UtayLCDKTZ42540638 = UtayLCDKTZ40404299;     UtayLCDKTZ40404299 = UtayLCDKTZ15567154;     UtayLCDKTZ15567154 = UtayLCDKTZ11233629;     UtayLCDKTZ11233629 = UtayLCDKTZ5107231;     UtayLCDKTZ5107231 = UtayLCDKTZ14443977;     UtayLCDKTZ14443977 = UtayLCDKTZ93378146;     UtayLCDKTZ93378146 = UtayLCDKTZ7157525;     UtayLCDKTZ7157525 = UtayLCDKTZ32476216;     UtayLCDKTZ32476216 = UtayLCDKTZ89260608;     UtayLCDKTZ89260608 = UtayLCDKTZ14011650;     UtayLCDKTZ14011650 = UtayLCDKTZ47931480;     UtayLCDKTZ47931480 = UtayLCDKTZ5210155;     UtayLCDKTZ5210155 = UtayLCDKTZ70179479;     UtayLCDKTZ70179479 = UtayLCDKTZ40893350;     UtayLCDKTZ40893350 = UtayLCDKTZ1559207;     UtayLCDKTZ1559207 = UtayLCDKTZ38339843;     UtayLCDKTZ38339843 = UtayLCDKTZ95929604;     UtayLCDKTZ95929604 = UtayLCDKTZ70158019;     UtayLCDKTZ70158019 = UtayLCDKTZ67578546;     UtayLCDKTZ67578546 = UtayLCDKTZ46101271;     UtayLCDKTZ46101271 = UtayLCDKTZ82183745;     UtayLCDKTZ82183745 = UtayLCDKTZ53870371;     UtayLCDKTZ53870371 = UtayLCDKTZ41143662;     UtayLCDKTZ41143662 = UtayLCDKTZ7196001;     UtayLCDKTZ7196001 = UtayLCDKTZ83285777;     UtayLCDKTZ83285777 = UtayLCDKTZ33844005;     UtayLCDKTZ33844005 = UtayLCDKTZ49162563;     UtayLCDKTZ49162563 = UtayLCDKTZ8468046;     UtayLCDKTZ8468046 = UtayLCDKTZ79407894;     UtayLCDKTZ79407894 = UtayLCDKTZ39686833;     UtayLCDKTZ39686833 = UtayLCDKTZ61432589;     UtayLCDKTZ61432589 = UtayLCDKTZ71326037;     UtayLCDKTZ71326037 = UtayLCDKTZ18880245;     UtayLCDKTZ18880245 = UtayLCDKTZ35016710;     UtayLCDKTZ35016710 = UtayLCDKTZ80361208;     UtayLCDKTZ80361208 = UtayLCDKTZ63367241;     UtayLCDKTZ63367241 = UtayLCDKTZ3796982;     UtayLCDKTZ3796982 = UtayLCDKTZ27241171;     UtayLCDKTZ27241171 = UtayLCDKTZ29886472;     UtayLCDKTZ29886472 = UtayLCDKTZ56690598;     UtayLCDKTZ56690598 = UtayLCDKTZ31792166;     UtayLCDKTZ31792166 = UtayLCDKTZ63339177;     UtayLCDKTZ63339177 = UtayLCDKTZ88902104;     UtayLCDKTZ88902104 = UtayLCDKTZ41959080;     UtayLCDKTZ41959080 = UtayLCDKTZ63112099;     UtayLCDKTZ63112099 = UtayLCDKTZ38414386;     UtayLCDKTZ38414386 = UtayLCDKTZ92547319;     UtayLCDKTZ92547319 = UtayLCDKTZ53051868;     UtayLCDKTZ53051868 = UtayLCDKTZ73467795;     UtayLCDKTZ73467795 = UtayLCDKTZ51449179;     UtayLCDKTZ51449179 = UtayLCDKTZ71029777;     UtayLCDKTZ71029777 = UtayLCDKTZ53418713;     UtayLCDKTZ53418713 = UtayLCDKTZ94388823;     UtayLCDKTZ94388823 = UtayLCDKTZ14733629;     UtayLCDKTZ14733629 = UtayLCDKTZ60356892;     UtayLCDKTZ60356892 = UtayLCDKTZ86533928;     UtayLCDKTZ86533928 = UtayLCDKTZ74423492;     UtayLCDKTZ74423492 = UtayLCDKTZ4037629;     UtayLCDKTZ4037629 = UtayLCDKTZ21821454;     UtayLCDKTZ21821454 = UtayLCDKTZ80599971;     UtayLCDKTZ80599971 = UtayLCDKTZ44215583;     UtayLCDKTZ44215583 = UtayLCDKTZ98689478;     UtayLCDKTZ98689478 = UtayLCDKTZ53068321;     UtayLCDKTZ53068321 = UtayLCDKTZ49573776;     UtayLCDKTZ49573776 = UtayLCDKTZ52579061;     UtayLCDKTZ52579061 = UtayLCDKTZ76605443;     UtayLCDKTZ76605443 = UtayLCDKTZ32033084;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void bdqDHRCHbF14644113() {     int DFfhlYxFWr54420201 = -784774962;    int DFfhlYxFWr81461891 = -561752587;    int DFfhlYxFWr47008858 = -573800285;    int DFfhlYxFWr41917005 = -586295913;    int DFfhlYxFWr94745058 = -111085522;    int DFfhlYxFWr57990898 = -105035606;    int DFfhlYxFWr75650786 = -379000283;    int DFfhlYxFWr44830775 = -250829209;    int DFfhlYxFWr3809347 = -726795889;    int DFfhlYxFWr76772905 = -535451085;    int DFfhlYxFWr70835266 = -281973466;    int DFfhlYxFWr35618094 = -159714889;    int DFfhlYxFWr44565528 = -33794402;    int DFfhlYxFWr63380259 = 65876551;    int DFfhlYxFWr15669673 = -173214165;    int DFfhlYxFWr59286522 = -324449893;    int DFfhlYxFWr82171753 = -892277059;    int DFfhlYxFWr50306865 = -44196821;    int DFfhlYxFWr91350375 = -379874516;    int DFfhlYxFWr17465974 = -64481816;    int DFfhlYxFWr96698500 = -145177956;    int DFfhlYxFWr41977931 = -359153534;    int DFfhlYxFWr59264052 = -354822601;    int DFfhlYxFWr37543366 = -531892778;    int DFfhlYxFWr79164786 = -743177882;    int DFfhlYxFWr51029793 = -346541341;    int DFfhlYxFWr41034638 = -697969600;    int DFfhlYxFWr80938451 = -246302957;    int DFfhlYxFWr96681582 = -107031953;    int DFfhlYxFWr76116562 = -556166304;    int DFfhlYxFWr67867537 = -968336735;    int DFfhlYxFWr19508561 = -119459739;    int DFfhlYxFWr41873570 = -546163292;    int DFfhlYxFWr1929326 = -372553642;    int DFfhlYxFWr16633664 = -490827184;    int DFfhlYxFWr57260023 = -684165335;    int DFfhlYxFWr4673341 = -815230963;    int DFfhlYxFWr58733331 = -479591348;    int DFfhlYxFWr62752108 = -115076545;    int DFfhlYxFWr13563861 = -10469248;    int DFfhlYxFWr35914896 = -258558852;    int DFfhlYxFWr20984607 = -939399074;    int DFfhlYxFWr94810800 = -882860820;    int DFfhlYxFWr13455769 = -540728083;    int DFfhlYxFWr15914747 = -571921207;    int DFfhlYxFWr13310436 = -354601088;    int DFfhlYxFWr82476877 = -918919261;    int DFfhlYxFWr85492024 = 89915689;    int DFfhlYxFWr96019111 = -517380559;    int DFfhlYxFWr35423589 = -612866500;    int DFfhlYxFWr4798822 = -654494773;    int DFfhlYxFWr44009276 = -54613618;    int DFfhlYxFWr45892950 = -881827309;    int DFfhlYxFWr6387817 = -785463776;    int DFfhlYxFWr17945613 = -44353592;    int DFfhlYxFWr12442271 = -325621428;    int DFfhlYxFWr22197839 = -106929986;    int DFfhlYxFWr9465492 = 58092493;    int DFfhlYxFWr62752219 = -843118032;    int DFfhlYxFWr43715265 = -764544181;    int DFfhlYxFWr16956260 = -407066006;    int DFfhlYxFWr94712334 = -32697326;    int DFfhlYxFWr48149192 = -43797257;    int DFfhlYxFWr27692785 = -70629585;    int DFfhlYxFWr8905369 = -567114350;    int DFfhlYxFWr51326706 = -62513727;    int DFfhlYxFWr93744523 = -613551597;    int DFfhlYxFWr42636203 = -661240761;    int DFfhlYxFWr46746596 = -443296265;    int DFfhlYxFWr58409649 = -489048831;    int DFfhlYxFWr54613182 = -509218930;    int DFfhlYxFWr23438422 = -312685712;    int DFfhlYxFWr87554756 = -929120276;    int DFfhlYxFWr77786514 = -269405268;    int DFfhlYxFWr81551078 = -805922965;    int DFfhlYxFWr75713893 = -205778882;    int DFfhlYxFWr47167131 = -476292715;    int DFfhlYxFWr45808283 = -814094519;    int DFfhlYxFWr21628620 = -959971572;    int DFfhlYxFWr65854350 = -288576795;    int DFfhlYxFWr68552916 = -427622081;    int DFfhlYxFWr55542614 = -687885290;    int DFfhlYxFWr84919340 = -728922398;    int DFfhlYxFWr61257994 = -494165453;    int DFfhlYxFWr71317740 = -901671531;    int DFfhlYxFWr23858261 = -813723118;    int DFfhlYxFWr73615610 = -237632430;    int DFfhlYxFWr35485754 = -760699517;    int DFfhlYxFWr83983712 = -228200051;    int DFfhlYxFWr4191394 = -65205757;    int DFfhlYxFWr35062184 = -477235350;    int DFfhlYxFWr95207848 = -773323457;    int DFfhlYxFWr95981112 = -636473317;    int DFfhlYxFWr19036844 = -350532364;    int DFfhlYxFWr96607600 = -603403243;    int DFfhlYxFWr41202561 = -125861527;    int DFfhlYxFWr72835414 = -795601818;    int DFfhlYxFWr67118016 = -712231235;    int DFfhlYxFWr4550401 = -973613733;    int DFfhlYxFWr64588041 = -784774962;     DFfhlYxFWr54420201 = DFfhlYxFWr81461891;     DFfhlYxFWr81461891 = DFfhlYxFWr47008858;     DFfhlYxFWr47008858 = DFfhlYxFWr41917005;     DFfhlYxFWr41917005 = DFfhlYxFWr94745058;     DFfhlYxFWr94745058 = DFfhlYxFWr57990898;     DFfhlYxFWr57990898 = DFfhlYxFWr75650786;     DFfhlYxFWr75650786 = DFfhlYxFWr44830775;     DFfhlYxFWr44830775 = DFfhlYxFWr3809347;     DFfhlYxFWr3809347 = DFfhlYxFWr76772905;     DFfhlYxFWr76772905 = DFfhlYxFWr70835266;     DFfhlYxFWr70835266 = DFfhlYxFWr35618094;     DFfhlYxFWr35618094 = DFfhlYxFWr44565528;     DFfhlYxFWr44565528 = DFfhlYxFWr63380259;     DFfhlYxFWr63380259 = DFfhlYxFWr15669673;     DFfhlYxFWr15669673 = DFfhlYxFWr59286522;     DFfhlYxFWr59286522 = DFfhlYxFWr82171753;     DFfhlYxFWr82171753 = DFfhlYxFWr50306865;     DFfhlYxFWr50306865 = DFfhlYxFWr91350375;     DFfhlYxFWr91350375 = DFfhlYxFWr17465974;     DFfhlYxFWr17465974 = DFfhlYxFWr96698500;     DFfhlYxFWr96698500 = DFfhlYxFWr41977931;     DFfhlYxFWr41977931 = DFfhlYxFWr59264052;     DFfhlYxFWr59264052 = DFfhlYxFWr37543366;     DFfhlYxFWr37543366 = DFfhlYxFWr79164786;     DFfhlYxFWr79164786 = DFfhlYxFWr51029793;     DFfhlYxFWr51029793 = DFfhlYxFWr41034638;     DFfhlYxFWr41034638 = DFfhlYxFWr80938451;     DFfhlYxFWr80938451 = DFfhlYxFWr96681582;     DFfhlYxFWr96681582 = DFfhlYxFWr76116562;     DFfhlYxFWr76116562 = DFfhlYxFWr67867537;     DFfhlYxFWr67867537 = DFfhlYxFWr19508561;     DFfhlYxFWr19508561 = DFfhlYxFWr41873570;     DFfhlYxFWr41873570 = DFfhlYxFWr1929326;     DFfhlYxFWr1929326 = DFfhlYxFWr16633664;     DFfhlYxFWr16633664 = DFfhlYxFWr57260023;     DFfhlYxFWr57260023 = DFfhlYxFWr4673341;     DFfhlYxFWr4673341 = DFfhlYxFWr58733331;     DFfhlYxFWr58733331 = DFfhlYxFWr62752108;     DFfhlYxFWr62752108 = DFfhlYxFWr13563861;     DFfhlYxFWr13563861 = DFfhlYxFWr35914896;     DFfhlYxFWr35914896 = DFfhlYxFWr20984607;     DFfhlYxFWr20984607 = DFfhlYxFWr94810800;     DFfhlYxFWr94810800 = DFfhlYxFWr13455769;     DFfhlYxFWr13455769 = DFfhlYxFWr15914747;     DFfhlYxFWr15914747 = DFfhlYxFWr13310436;     DFfhlYxFWr13310436 = DFfhlYxFWr82476877;     DFfhlYxFWr82476877 = DFfhlYxFWr85492024;     DFfhlYxFWr85492024 = DFfhlYxFWr96019111;     DFfhlYxFWr96019111 = DFfhlYxFWr35423589;     DFfhlYxFWr35423589 = DFfhlYxFWr4798822;     DFfhlYxFWr4798822 = DFfhlYxFWr44009276;     DFfhlYxFWr44009276 = DFfhlYxFWr45892950;     DFfhlYxFWr45892950 = DFfhlYxFWr6387817;     DFfhlYxFWr6387817 = DFfhlYxFWr17945613;     DFfhlYxFWr17945613 = DFfhlYxFWr12442271;     DFfhlYxFWr12442271 = DFfhlYxFWr22197839;     DFfhlYxFWr22197839 = DFfhlYxFWr9465492;     DFfhlYxFWr9465492 = DFfhlYxFWr62752219;     DFfhlYxFWr62752219 = DFfhlYxFWr43715265;     DFfhlYxFWr43715265 = DFfhlYxFWr16956260;     DFfhlYxFWr16956260 = DFfhlYxFWr94712334;     DFfhlYxFWr94712334 = DFfhlYxFWr48149192;     DFfhlYxFWr48149192 = DFfhlYxFWr27692785;     DFfhlYxFWr27692785 = DFfhlYxFWr8905369;     DFfhlYxFWr8905369 = DFfhlYxFWr51326706;     DFfhlYxFWr51326706 = DFfhlYxFWr93744523;     DFfhlYxFWr93744523 = DFfhlYxFWr42636203;     DFfhlYxFWr42636203 = DFfhlYxFWr46746596;     DFfhlYxFWr46746596 = DFfhlYxFWr58409649;     DFfhlYxFWr58409649 = DFfhlYxFWr54613182;     DFfhlYxFWr54613182 = DFfhlYxFWr23438422;     DFfhlYxFWr23438422 = DFfhlYxFWr87554756;     DFfhlYxFWr87554756 = DFfhlYxFWr77786514;     DFfhlYxFWr77786514 = DFfhlYxFWr81551078;     DFfhlYxFWr81551078 = DFfhlYxFWr75713893;     DFfhlYxFWr75713893 = DFfhlYxFWr47167131;     DFfhlYxFWr47167131 = DFfhlYxFWr45808283;     DFfhlYxFWr45808283 = DFfhlYxFWr21628620;     DFfhlYxFWr21628620 = DFfhlYxFWr65854350;     DFfhlYxFWr65854350 = DFfhlYxFWr68552916;     DFfhlYxFWr68552916 = DFfhlYxFWr55542614;     DFfhlYxFWr55542614 = DFfhlYxFWr84919340;     DFfhlYxFWr84919340 = DFfhlYxFWr61257994;     DFfhlYxFWr61257994 = DFfhlYxFWr71317740;     DFfhlYxFWr71317740 = DFfhlYxFWr23858261;     DFfhlYxFWr23858261 = DFfhlYxFWr73615610;     DFfhlYxFWr73615610 = DFfhlYxFWr35485754;     DFfhlYxFWr35485754 = DFfhlYxFWr83983712;     DFfhlYxFWr83983712 = DFfhlYxFWr4191394;     DFfhlYxFWr4191394 = DFfhlYxFWr35062184;     DFfhlYxFWr35062184 = DFfhlYxFWr95207848;     DFfhlYxFWr95207848 = DFfhlYxFWr95981112;     DFfhlYxFWr95981112 = DFfhlYxFWr19036844;     DFfhlYxFWr19036844 = DFfhlYxFWr96607600;     DFfhlYxFWr96607600 = DFfhlYxFWr41202561;     DFfhlYxFWr41202561 = DFfhlYxFWr72835414;     DFfhlYxFWr72835414 = DFfhlYxFWr67118016;     DFfhlYxFWr67118016 = DFfhlYxFWr4550401;     DFfhlYxFWr4550401 = DFfhlYxFWr64588041;     DFfhlYxFWr64588041 = DFfhlYxFWr54420201;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void XRVJKSzLwE51289088() {     int rEDyffjaai76807319 = -419178925;    int rEDyffjaai65809391 = -510295641;    int rEDyffjaai75778136 = -813454129;    int rEDyffjaai56923694 = -592703445;    int rEDyffjaai92919312 = -702142523;    int rEDyffjaai72874015 = 34356016;    int rEDyffjaai67806525 = -200987589;    int rEDyffjaai20464634 = -721425853;    int rEDyffjaai972478 = -610991928;    int rEDyffjaai68536489 = -278192418;    int rEDyffjaai8377126 = -278597866;    int rEDyffjaai91521042 = -410460183;    int rEDyffjaai11573709 = -652511722;    int rEDyffjaai5995012 = -461832206;    int rEDyffjaai52404949 = -900691878;    int rEDyffjaai3542435 = -706117435;    int rEDyffjaai31995105 = -69556879;    int rEDyffjaai56283281 = -495939512;    int rEDyffjaai32632006 = -514976641;    int rEDyffjaai95982257 = -317116270;    int rEDyffjaai97581607 = -136665264;    int rEDyffjaai5793150 = -863919848;    int rEDyffjaai62557375 = -530903193;    int rEDyffjaai64043153 = 4114364;    int rEDyffjaai14705034 = -364737007;    int rEDyffjaai39332788 = -790435565;    int rEDyffjaai88124058 = -455556093;    int rEDyffjaai86849901 = -34757411;    int rEDyffjaai3574144 = -451289072;    int rEDyffjaai85273741 = -476462171;    int rEDyffjaai12158342 = 50347112;    int rEDyffjaai77049751 = -771531094;    int rEDyffjaai22912241 = 24493213;    int rEDyffjaai61318012 = -696680492;    int rEDyffjaai92863028 = -554583607;    int rEDyffjaai98952892 = -999560712;    int rEDyffjaai98113052 = -548178915;    int rEDyffjaai12359433 = -347216820;    int rEDyffjaai11060240 = -516122577;    int rEDyffjaai33749576 = -146693277;    int rEDyffjaai64672266 = -761736518;    int rEDyffjaai9492999 = -697307765;    int rEDyffjaai360993 = -770391404;    int rEDyffjaai12899889 = -538091732;    int rEDyffjaai83898013 = -381064685;    int rEDyffjaai21410718 = -536830183;    int rEDyffjaai94774274 = -397329513;    int rEDyffjaai30090699 = -766619302;    int rEDyffjaai90479016 = -467097258;    int rEDyffjaai32507335 = -754886759;    int rEDyffjaai13668038 = 90428950;    int rEDyffjaai17860534 = -485432900;    int rEDyffjaai24207353 = -553738540;    int rEDyffjaai66674361 = 38855415;    int rEDyffjaai53707480 = -185057707;    int rEDyffjaai71014169 = -555259078;    int rEDyffjaai3252017 = -979392448;    int rEDyffjaai11734984 = -717568494;    int rEDyffjaai42218660 = -127966439;    int rEDyffjaai53586525 = -911706959;    int rEDyffjaai84749956 = -510087891;    int rEDyffjaai80956623 = -66230179;    int rEDyffjaai16890490 = -170136782;    int rEDyffjaai15698737 = -34529758;    int rEDyffjaai56378148 = -228539531;    int rEDyffjaai31327375 = -507066773;    int rEDyffjaai68608802 = -334953396;    int rEDyffjaai50255696 = -955831231;    int rEDyffjaai13131984 = -907248600;    int rEDyffjaai53452057 = -901131167;    int rEDyffjaai5429382 = -57938521;    int rEDyffjaai19635673 = -722340060;    int rEDyffjaai45223041 = -979816935;    int rEDyffjaai98882430 = -268283364;    int rEDyffjaai31309991 = -555379752;    int rEDyffjaai88088609 = -439357500;    int rEDyffjaai5432157 = 6471556;    int rEDyffjaai49657487 = -992811462;    int rEDyffjaai80145140 = -614820952;    int rEDyffjaai93294315 = -827906824;    int rEDyffjaai44558514 = -293106052;    int rEDyffjaai58033360 = -688936792;    int rEDyffjaai96370885 = -567660153;    int rEDyffjaai71066809 = -696402313;    int rEDyffjaai71605703 = -466891121;    int rEDyffjaai94297808 = -464219988;    int rEDyffjaai52842398 = -117792555;    int rEDyffjaai56237879 = 85637797;    int rEDyffjaai7610532 = -411622786;    int rEDyffjaai21848859 = -999324529;    int rEDyffjaai95700875 = 79831736;    int rEDyffjaai86378069 = -830610422;    int rEDyffjaai70140772 = -119250382;    int rEDyffjaai57473715 = -604415619;    int rEDyffjaai48999619 = -636605387;    int rEDyffjaai83715643 = -595506340;    int rEDyffjaai92602508 = -427170984;    int rEDyffjaai84662256 = -635861647;    int rEDyffjaai56521740 = -209552201;    int rEDyffjaai52570639 = -419178925;     rEDyffjaai76807319 = rEDyffjaai65809391;     rEDyffjaai65809391 = rEDyffjaai75778136;     rEDyffjaai75778136 = rEDyffjaai56923694;     rEDyffjaai56923694 = rEDyffjaai92919312;     rEDyffjaai92919312 = rEDyffjaai72874015;     rEDyffjaai72874015 = rEDyffjaai67806525;     rEDyffjaai67806525 = rEDyffjaai20464634;     rEDyffjaai20464634 = rEDyffjaai972478;     rEDyffjaai972478 = rEDyffjaai68536489;     rEDyffjaai68536489 = rEDyffjaai8377126;     rEDyffjaai8377126 = rEDyffjaai91521042;     rEDyffjaai91521042 = rEDyffjaai11573709;     rEDyffjaai11573709 = rEDyffjaai5995012;     rEDyffjaai5995012 = rEDyffjaai52404949;     rEDyffjaai52404949 = rEDyffjaai3542435;     rEDyffjaai3542435 = rEDyffjaai31995105;     rEDyffjaai31995105 = rEDyffjaai56283281;     rEDyffjaai56283281 = rEDyffjaai32632006;     rEDyffjaai32632006 = rEDyffjaai95982257;     rEDyffjaai95982257 = rEDyffjaai97581607;     rEDyffjaai97581607 = rEDyffjaai5793150;     rEDyffjaai5793150 = rEDyffjaai62557375;     rEDyffjaai62557375 = rEDyffjaai64043153;     rEDyffjaai64043153 = rEDyffjaai14705034;     rEDyffjaai14705034 = rEDyffjaai39332788;     rEDyffjaai39332788 = rEDyffjaai88124058;     rEDyffjaai88124058 = rEDyffjaai86849901;     rEDyffjaai86849901 = rEDyffjaai3574144;     rEDyffjaai3574144 = rEDyffjaai85273741;     rEDyffjaai85273741 = rEDyffjaai12158342;     rEDyffjaai12158342 = rEDyffjaai77049751;     rEDyffjaai77049751 = rEDyffjaai22912241;     rEDyffjaai22912241 = rEDyffjaai61318012;     rEDyffjaai61318012 = rEDyffjaai92863028;     rEDyffjaai92863028 = rEDyffjaai98952892;     rEDyffjaai98952892 = rEDyffjaai98113052;     rEDyffjaai98113052 = rEDyffjaai12359433;     rEDyffjaai12359433 = rEDyffjaai11060240;     rEDyffjaai11060240 = rEDyffjaai33749576;     rEDyffjaai33749576 = rEDyffjaai64672266;     rEDyffjaai64672266 = rEDyffjaai9492999;     rEDyffjaai9492999 = rEDyffjaai360993;     rEDyffjaai360993 = rEDyffjaai12899889;     rEDyffjaai12899889 = rEDyffjaai83898013;     rEDyffjaai83898013 = rEDyffjaai21410718;     rEDyffjaai21410718 = rEDyffjaai94774274;     rEDyffjaai94774274 = rEDyffjaai30090699;     rEDyffjaai30090699 = rEDyffjaai90479016;     rEDyffjaai90479016 = rEDyffjaai32507335;     rEDyffjaai32507335 = rEDyffjaai13668038;     rEDyffjaai13668038 = rEDyffjaai17860534;     rEDyffjaai17860534 = rEDyffjaai24207353;     rEDyffjaai24207353 = rEDyffjaai66674361;     rEDyffjaai66674361 = rEDyffjaai53707480;     rEDyffjaai53707480 = rEDyffjaai71014169;     rEDyffjaai71014169 = rEDyffjaai3252017;     rEDyffjaai3252017 = rEDyffjaai11734984;     rEDyffjaai11734984 = rEDyffjaai42218660;     rEDyffjaai42218660 = rEDyffjaai53586525;     rEDyffjaai53586525 = rEDyffjaai84749956;     rEDyffjaai84749956 = rEDyffjaai80956623;     rEDyffjaai80956623 = rEDyffjaai16890490;     rEDyffjaai16890490 = rEDyffjaai15698737;     rEDyffjaai15698737 = rEDyffjaai56378148;     rEDyffjaai56378148 = rEDyffjaai31327375;     rEDyffjaai31327375 = rEDyffjaai68608802;     rEDyffjaai68608802 = rEDyffjaai50255696;     rEDyffjaai50255696 = rEDyffjaai13131984;     rEDyffjaai13131984 = rEDyffjaai53452057;     rEDyffjaai53452057 = rEDyffjaai5429382;     rEDyffjaai5429382 = rEDyffjaai19635673;     rEDyffjaai19635673 = rEDyffjaai45223041;     rEDyffjaai45223041 = rEDyffjaai98882430;     rEDyffjaai98882430 = rEDyffjaai31309991;     rEDyffjaai31309991 = rEDyffjaai88088609;     rEDyffjaai88088609 = rEDyffjaai5432157;     rEDyffjaai5432157 = rEDyffjaai49657487;     rEDyffjaai49657487 = rEDyffjaai80145140;     rEDyffjaai80145140 = rEDyffjaai93294315;     rEDyffjaai93294315 = rEDyffjaai44558514;     rEDyffjaai44558514 = rEDyffjaai58033360;     rEDyffjaai58033360 = rEDyffjaai96370885;     rEDyffjaai96370885 = rEDyffjaai71066809;     rEDyffjaai71066809 = rEDyffjaai71605703;     rEDyffjaai71605703 = rEDyffjaai94297808;     rEDyffjaai94297808 = rEDyffjaai52842398;     rEDyffjaai52842398 = rEDyffjaai56237879;     rEDyffjaai56237879 = rEDyffjaai7610532;     rEDyffjaai7610532 = rEDyffjaai21848859;     rEDyffjaai21848859 = rEDyffjaai95700875;     rEDyffjaai95700875 = rEDyffjaai86378069;     rEDyffjaai86378069 = rEDyffjaai70140772;     rEDyffjaai70140772 = rEDyffjaai57473715;     rEDyffjaai57473715 = rEDyffjaai48999619;     rEDyffjaai48999619 = rEDyffjaai83715643;     rEDyffjaai83715643 = rEDyffjaai92602508;     rEDyffjaai92602508 = rEDyffjaai84662256;     rEDyffjaai84662256 = rEDyffjaai56521740;     rEDyffjaai56521740 = rEDyffjaai52570639;     rEDyffjaai52570639 = rEDyffjaai76807319;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void KvoVElMlcx87934062() {     int xFOEGWOvNL99194436 = -53582888;    int xFOEGWOvNL50156892 = -458838696;    int xFOEGWOvNL4547416 = 46892027;    int xFOEGWOvNL71930382 = -599110977;    int xFOEGWOvNL91093566 = -193199524;    int xFOEGWOvNL87757132 = -926252361;    int xFOEGWOvNL59962264 = -22974895;    int xFOEGWOvNL96098492 = -92022497;    int xFOEGWOvNL98135609 = -495187967;    int xFOEGWOvNL60300074 = -20933751;    int xFOEGWOvNL45918986 = -275222267;    int xFOEGWOvNL47423991 = -661205476;    int xFOEGWOvNL78581889 = -171229042;    int xFOEGWOvNL48609764 = -989540963;    int xFOEGWOvNL89140226 = -528169591;    int xFOEGWOvNL47798347 = 12215022;    int xFOEGWOvNL81818457 = -346836699;    int xFOEGWOvNL62259697 = -947682202;    int xFOEGWOvNL73913637 = -650078766;    int xFOEGWOvNL74498540 = -569750724;    int xFOEGWOvNL98464714 = -128152573;    int xFOEGWOvNL69608368 = -268686161;    int xFOEGWOvNL65850698 = -706983786;    int xFOEGWOvNL90542940 = -559878493;    int xFOEGWOvNL50245280 = 13703869;    int xFOEGWOvNL27635783 = -134329789;    int xFOEGWOvNL35213479 = -213142586;    int xFOEGWOvNL92761352 = -923211864;    int xFOEGWOvNL10466705 = -795546191;    int xFOEGWOvNL94430921 = -396758037;    int xFOEGWOvNL56449147 = -30969040;    int xFOEGWOvNL34590942 = -323602448;    int xFOEGWOvNL3950911 = -504850282;    int xFOEGWOvNL20706700 = 79192658;    int xFOEGWOvNL69092393 = -618340029;    int xFOEGWOvNL40645761 = -214956088;    int xFOEGWOvNL91552765 = -281126866;    int xFOEGWOvNL65985533 = -214842292;    int xFOEGWOvNL59368371 = -917168609;    int xFOEGWOvNL53935290 = -282917307;    int xFOEGWOvNL93429637 = -164914184;    int xFOEGWOvNL98001389 = -455216456;    int xFOEGWOvNL5911185 = -657921987;    int xFOEGWOvNL12344008 = -535455381;    int xFOEGWOvNL51881280 = -190208162;    int xFOEGWOvNL29511000 = -719059278;    int xFOEGWOvNL7071672 = -975739766;    int xFOEGWOvNL74689373 = -523154292;    int xFOEGWOvNL84938921 = -416813956;    int xFOEGWOvNL29591080 = -896907018;    int xFOEGWOvNL22537255 = -264647327;    int xFOEGWOvNL91711790 = -916252183;    int xFOEGWOvNL2521757 = -225649770;    int xFOEGWOvNL26960907 = -236825394;    int xFOEGWOvNL89469347 = -325761822;    int xFOEGWOvNL29586068 = -784896728;    int xFOEGWOvNL84306193 = -751854911;    int xFOEGWOvNL14004475 = -393229481;    int xFOEGWOvNL21685102 = -512814846;    int xFOEGWOvNL63457784 = 41130264;    int xFOEGWOvNL52543653 = -613109776;    int xFOEGWOvNL67200912 = -99763031;    int xFOEGWOvNL85631788 = -296476307;    int xFOEGWOvNL3704688 = 1570070;    int xFOEGWOvNL3850928 = -989964712;    int xFOEGWOvNL11328044 = -951619819;    int xFOEGWOvNL43473081 = -56355194;    int xFOEGWOvNL57875189 = -150421700;    int xFOEGWOvNL79517371 = -271200934;    int xFOEGWOvNL48494465 = -213213504;    int xFOEGWOvNL56245582 = -706658112;    int xFOEGWOvNL15832924 = -31994408;    int xFOEGWOvNL2891327 = 69486406;    int xFOEGWOvNL19978347 = -267161460;    int xFOEGWOvNL81068903 = -304836540;    int xFOEGWOvNL463326 = -672936117;    int xFOEGWOvNL63697183 = -610764174;    int xFOEGWOvNL53506691 = -71528405;    int xFOEGWOvNL38661661 = -269670331;    int xFOEGWOvNL20734281 = -267236853;    int xFOEGWOvNL20564111 = -158590023;    int xFOEGWOvNL60524105 = -689988294;    int xFOEGWOvNL7822431 = -406397909;    int xFOEGWOvNL80875624 = -898639174;    int xFOEGWOvNL71893666 = -32110710;    int xFOEGWOvNL64737356 = -114716858;    int xFOEGWOvNL32069186 = 2047321;    int xFOEGWOvNL76990004 = -168024889;    int xFOEGWOvNL31237352 = -595045520;    int xFOEGWOvNL39506325 = -833443302;    int xFOEGWOvNL56339568 = -463101178;    int xFOEGWOvNL77548290 = -887897386;    int xFOEGWOvNL44300431 = -702027446;    int xFOEGWOvNL95910586 = -858298873;    int xFOEGWOvNL1391637 = -669807531;    int xFOEGWOvNL26228725 = 34848846;    int xFOEGWOvNL12369602 = -58740150;    int xFOEGWOvNL2206497 = -559492058;    int xFOEGWOvNL8493081 = -545490669;    int xFOEGWOvNL40553237 = -53582888;     xFOEGWOvNL99194436 = xFOEGWOvNL50156892;     xFOEGWOvNL50156892 = xFOEGWOvNL4547416;     xFOEGWOvNL4547416 = xFOEGWOvNL71930382;     xFOEGWOvNL71930382 = xFOEGWOvNL91093566;     xFOEGWOvNL91093566 = xFOEGWOvNL87757132;     xFOEGWOvNL87757132 = xFOEGWOvNL59962264;     xFOEGWOvNL59962264 = xFOEGWOvNL96098492;     xFOEGWOvNL96098492 = xFOEGWOvNL98135609;     xFOEGWOvNL98135609 = xFOEGWOvNL60300074;     xFOEGWOvNL60300074 = xFOEGWOvNL45918986;     xFOEGWOvNL45918986 = xFOEGWOvNL47423991;     xFOEGWOvNL47423991 = xFOEGWOvNL78581889;     xFOEGWOvNL78581889 = xFOEGWOvNL48609764;     xFOEGWOvNL48609764 = xFOEGWOvNL89140226;     xFOEGWOvNL89140226 = xFOEGWOvNL47798347;     xFOEGWOvNL47798347 = xFOEGWOvNL81818457;     xFOEGWOvNL81818457 = xFOEGWOvNL62259697;     xFOEGWOvNL62259697 = xFOEGWOvNL73913637;     xFOEGWOvNL73913637 = xFOEGWOvNL74498540;     xFOEGWOvNL74498540 = xFOEGWOvNL98464714;     xFOEGWOvNL98464714 = xFOEGWOvNL69608368;     xFOEGWOvNL69608368 = xFOEGWOvNL65850698;     xFOEGWOvNL65850698 = xFOEGWOvNL90542940;     xFOEGWOvNL90542940 = xFOEGWOvNL50245280;     xFOEGWOvNL50245280 = xFOEGWOvNL27635783;     xFOEGWOvNL27635783 = xFOEGWOvNL35213479;     xFOEGWOvNL35213479 = xFOEGWOvNL92761352;     xFOEGWOvNL92761352 = xFOEGWOvNL10466705;     xFOEGWOvNL10466705 = xFOEGWOvNL94430921;     xFOEGWOvNL94430921 = xFOEGWOvNL56449147;     xFOEGWOvNL56449147 = xFOEGWOvNL34590942;     xFOEGWOvNL34590942 = xFOEGWOvNL3950911;     xFOEGWOvNL3950911 = xFOEGWOvNL20706700;     xFOEGWOvNL20706700 = xFOEGWOvNL69092393;     xFOEGWOvNL69092393 = xFOEGWOvNL40645761;     xFOEGWOvNL40645761 = xFOEGWOvNL91552765;     xFOEGWOvNL91552765 = xFOEGWOvNL65985533;     xFOEGWOvNL65985533 = xFOEGWOvNL59368371;     xFOEGWOvNL59368371 = xFOEGWOvNL53935290;     xFOEGWOvNL53935290 = xFOEGWOvNL93429637;     xFOEGWOvNL93429637 = xFOEGWOvNL98001389;     xFOEGWOvNL98001389 = xFOEGWOvNL5911185;     xFOEGWOvNL5911185 = xFOEGWOvNL12344008;     xFOEGWOvNL12344008 = xFOEGWOvNL51881280;     xFOEGWOvNL51881280 = xFOEGWOvNL29511000;     xFOEGWOvNL29511000 = xFOEGWOvNL7071672;     xFOEGWOvNL7071672 = xFOEGWOvNL74689373;     xFOEGWOvNL74689373 = xFOEGWOvNL84938921;     xFOEGWOvNL84938921 = xFOEGWOvNL29591080;     xFOEGWOvNL29591080 = xFOEGWOvNL22537255;     xFOEGWOvNL22537255 = xFOEGWOvNL91711790;     xFOEGWOvNL91711790 = xFOEGWOvNL2521757;     xFOEGWOvNL2521757 = xFOEGWOvNL26960907;     xFOEGWOvNL26960907 = xFOEGWOvNL89469347;     xFOEGWOvNL89469347 = xFOEGWOvNL29586068;     xFOEGWOvNL29586068 = xFOEGWOvNL84306193;     xFOEGWOvNL84306193 = xFOEGWOvNL14004475;     xFOEGWOvNL14004475 = xFOEGWOvNL21685102;     xFOEGWOvNL21685102 = xFOEGWOvNL63457784;     xFOEGWOvNL63457784 = xFOEGWOvNL52543653;     xFOEGWOvNL52543653 = xFOEGWOvNL67200912;     xFOEGWOvNL67200912 = xFOEGWOvNL85631788;     xFOEGWOvNL85631788 = xFOEGWOvNL3704688;     xFOEGWOvNL3704688 = xFOEGWOvNL3850928;     xFOEGWOvNL3850928 = xFOEGWOvNL11328044;     xFOEGWOvNL11328044 = xFOEGWOvNL43473081;     xFOEGWOvNL43473081 = xFOEGWOvNL57875189;     xFOEGWOvNL57875189 = xFOEGWOvNL79517371;     xFOEGWOvNL79517371 = xFOEGWOvNL48494465;     xFOEGWOvNL48494465 = xFOEGWOvNL56245582;     xFOEGWOvNL56245582 = xFOEGWOvNL15832924;     xFOEGWOvNL15832924 = xFOEGWOvNL2891327;     xFOEGWOvNL2891327 = xFOEGWOvNL19978347;     xFOEGWOvNL19978347 = xFOEGWOvNL81068903;     xFOEGWOvNL81068903 = xFOEGWOvNL463326;     xFOEGWOvNL463326 = xFOEGWOvNL63697183;     xFOEGWOvNL63697183 = xFOEGWOvNL53506691;     xFOEGWOvNL53506691 = xFOEGWOvNL38661661;     xFOEGWOvNL38661661 = xFOEGWOvNL20734281;     xFOEGWOvNL20734281 = xFOEGWOvNL20564111;     xFOEGWOvNL20564111 = xFOEGWOvNL60524105;     xFOEGWOvNL60524105 = xFOEGWOvNL7822431;     xFOEGWOvNL7822431 = xFOEGWOvNL80875624;     xFOEGWOvNL80875624 = xFOEGWOvNL71893666;     xFOEGWOvNL71893666 = xFOEGWOvNL64737356;     xFOEGWOvNL64737356 = xFOEGWOvNL32069186;     xFOEGWOvNL32069186 = xFOEGWOvNL76990004;     xFOEGWOvNL76990004 = xFOEGWOvNL31237352;     xFOEGWOvNL31237352 = xFOEGWOvNL39506325;     xFOEGWOvNL39506325 = xFOEGWOvNL56339568;     xFOEGWOvNL56339568 = xFOEGWOvNL77548290;     xFOEGWOvNL77548290 = xFOEGWOvNL44300431;     xFOEGWOvNL44300431 = xFOEGWOvNL95910586;     xFOEGWOvNL95910586 = xFOEGWOvNL1391637;     xFOEGWOvNL1391637 = xFOEGWOvNL26228725;     xFOEGWOvNL26228725 = xFOEGWOvNL12369602;     xFOEGWOvNL12369602 = xFOEGWOvNL2206497;     xFOEGWOvNL2206497 = xFOEGWOvNL8493081;     xFOEGWOvNL8493081 = xFOEGWOvNL40553237;     xFOEGWOvNL40553237 = xFOEGWOvNL99194436;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void uiIQmJKYBP24579037() {     int wCvTUDEIgK21581554 = -787986851;    int wCvTUDEIgK34504392 = -407381751;    int wCvTUDEIgK33316694 = -192761817;    int wCvTUDEIgK86937070 = -605518508;    int wCvTUDEIgK89267820 = -784256526;    int wCvTUDEIgK2640249 = -786860739;    int wCvTUDEIgK52118003 = -944962201;    int wCvTUDEIgK71732351 = -562619142;    int wCvTUDEIgK95298740 = -379384006;    int wCvTUDEIgK52063659 = -863675084;    int wCvTUDEIgK83460845 = -271846668;    int wCvTUDEIgK3326941 = -911950769;    int wCvTUDEIgK45590070 = -789946361;    int wCvTUDEIgK91224516 = -417249720;    int wCvTUDEIgK25875504 = -155647305;    int wCvTUDEIgK92054259 = -369452520;    int wCvTUDEIgK31641809 = -624116519;    int wCvTUDEIgK68236113 = -299424893;    int wCvTUDEIgK15195268 = -785180892;    int wCvTUDEIgK53014824 = -822385177;    int wCvTUDEIgK99347821 = -119639881;    int wCvTUDEIgK33423587 = -773452474;    int wCvTUDEIgK69144022 = -883064378;    int wCvTUDEIgK17042728 = -23871350;    int wCvTUDEIgK85785527 = -707855256;    int wCvTUDEIgK15938777 = -578224013;    int wCvTUDEIgK82302899 = 29270921;    int wCvTUDEIgK98672802 = -711666318;    int wCvTUDEIgK17359266 = -39803310;    int wCvTUDEIgK3588101 = -317053904;    int wCvTUDEIgK739953 = -112285192;    int wCvTUDEIgK92132133 = -975673803;    int wCvTUDEIgK84989581 = 65806223;    int wCvTUDEIgK80095387 = -244934192;    int wCvTUDEIgK45321758 = -682096452;    int wCvTUDEIgK82338630 = -530351465;    int wCvTUDEIgK84992477 = -14074818;    int wCvTUDEIgK19611634 = -82467764;    int wCvTUDEIgK7676503 = -218214640;    int wCvTUDEIgK74121004 = -419141336;    int wCvTUDEIgK22187008 = -668091850;    int wCvTUDEIgK86509780 = -213125147;    int wCvTUDEIgK11461377 = -545452571;    int wCvTUDEIgK11788127 = -532819030;    int wCvTUDEIgK19864547 = 648360;    int wCvTUDEIgK37611282 = -901288374;    int wCvTUDEIgK19369069 = -454150019;    int wCvTUDEIgK19288048 = -279689283;    int wCvTUDEIgK79398826 = -366530654;    int wCvTUDEIgK26674826 = 61072723;    int wCvTUDEIgK31406471 = -619723604;    int wCvTUDEIgK65563048 = -247071465;    int wCvTUDEIgK80836159 = -997561000;    int wCvTUDEIgK87247451 = -512506203;    int wCvTUDEIgK25231216 = -466465937;    int wCvTUDEIgK88157967 = 85465622;    int wCvTUDEIgK65360370 = -524317373;    int wCvTUDEIgK16273967 = -68890468;    int wCvTUDEIgK1151544 = -897663253;    int wCvTUDEIgK73329044 = -106032513;    int wCvTUDEIgK20337350 = -716131661;    int wCvTUDEIgK53445201 = -133295884;    int wCvTUDEIgK54373086 = -422815832;    int wCvTUDEIgK91710639 = 37669897;    int wCvTUDEIgK51323707 = -651389893;    int wCvTUDEIgK91328712 = -296172866;    int wCvTUDEIgK18337359 = -877756993;    int wCvTUDEIgK65494683 = -445012170;    int wCvTUDEIgK45902759 = -735153269;    int wCvTUDEIgK43536873 = -625295840;    int wCvTUDEIgK7061783 = -255377703;    int wCvTUDEIgK12030175 = -441648756;    int wCvTUDEIgK60559611 = 18789747;    int wCvTUDEIgK41074263 = -266039556;    int wCvTUDEIgK30827816 = -54293327;    int wCvTUDEIgK12838042 = -906514735;    int wCvTUDEIgK21962210 = -127999904;    int wCvTUDEIgK57355895 = -250245349;    int wCvTUDEIgK97178181 = 75480289;    int wCvTUDEIgK48174245 = -806566883;    int wCvTUDEIgK96569707 = -24073994;    int wCvTUDEIgK63014851 = -691039796;    int wCvTUDEIgK19273976 = -245135664;    int wCvTUDEIgK90684439 = -876034;    int wCvTUDEIgK72181629 = -697330300;    int wCvTUDEIgK35176905 = -865213728;    int wCvTUDEIgK11295974 = -978112804;    int wCvTUDEIgK97742129 = -421687575;    int wCvTUDEIgK54864172 = -778468255;    int wCvTUDEIgK57163790 = -667562075;    int wCvTUDEIgK16978260 = 93965908;    int wCvTUDEIgK68718511 = -945184351;    int wCvTUDEIgK18460091 = -184804511;    int wCvTUDEIgK34347459 = -12182127;    int wCvTUDEIgK53783655 = -703009675;    int wCvTUDEIgK68741807 = -434795967;    int wCvTUDEIgK32136695 = -790309316;    int wCvTUDEIgK19750738 = -483122469;    int wCvTUDEIgK60464420 = -881429137;    int wCvTUDEIgK28535835 = -787986851;     wCvTUDEIgK21581554 = wCvTUDEIgK34504392;     wCvTUDEIgK34504392 = wCvTUDEIgK33316694;     wCvTUDEIgK33316694 = wCvTUDEIgK86937070;     wCvTUDEIgK86937070 = wCvTUDEIgK89267820;     wCvTUDEIgK89267820 = wCvTUDEIgK2640249;     wCvTUDEIgK2640249 = wCvTUDEIgK52118003;     wCvTUDEIgK52118003 = wCvTUDEIgK71732351;     wCvTUDEIgK71732351 = wCvTUDEIgK95298740;     wCvTUDEIgK95298740 = wCvTUDEIgK52063659;     wCvTUDEIgK52063659 = wCvTUDEIgK83460845;     wCvTUDEIgK83460845 = wCvTUDEIgK3326941;     wCvTUDEIgK3326941 = wCvTUDEIgK45590070;     wCvTUDEIgK45590070 = wCvTUDEIgK91224516;     wCvTUDEIgK91224516 = wCvTUDEIgK25875504;     wCvTUDEIgK25875504 = wCvTUDEIgK92054259;     wCvTUDEIgK92054259 = wCvTUDEIgK31641809;     wCvTUDEIgK31641809 = wCvTUDEIgK68236113;     wCvTUDEIgK68236113 = wCvTUDEIgK15195268;     wCvTUDEIgK15195268 = wCvTUDEIgK53014824;     wCvTUDEIgK53014824 = wCvTUDEIgK99347821;     wCvTUDEIgK99347821 = wCvTUDEIgK33423587;     wCvTUDEIgK33423587 = wCvTUDEIgK69144022;     wCvTUDEIgK69144022 = wCvTUDEIgK17042728;     wCvTUDEIgK17042728 = wCvTUDEIgK85785527;     wCvTUDEIgK85785527 = wCvTUDEIgK15938777;     wCvTUDEIgK15938777 = wCvTUDEIgK82302899;     wCvTUDEIgK82302899 = wCvTUDEIgK98672802;     wCvTUDEIgK98672802 = wCvTUDEIgK17359266;     wCvTUDEIgK17359266 = wCvTUDEIgK3588101;     wCvTUDEIgK3588101 = wCvTUDEIgK739953;     wCvTUDEIgK739953 = wCvTUDEIgK92132133;     wCvTUDEIgK92132133 = wCvTUDEIgK84989581;     wCvTUDEIgK84989581 = wCvTUDEIgK80095387;     wCvTUDEIgK80095387 = wCvTUDEIgK45321758;     wCvTUDEIgK45321758 = wCvTUDEIgK82338630;     wCvTUDEIgK82338630 = wCvTUDEIgK84992477;     wCvTUDEIgK84992477 = wCvTUDEIgK19611634;     wCvTUDEIgK19611634 = wCvTUDEIgK7676503;     wCvTUDEIgK7676503 = wCvTUDEIgK74121004;     wCvTUDEIgK74121004 = wCvTUDEIgK22187008;     wCvTUDEIgK22187008 = wCvTUDEIgK86509780;     wCvTUDEIgK86509780 = wCvTUDEIgK11461377;     wCvTUDEIgK11461377 = wCvTUDEIgK11788127;     wCvTUDEIgK11788127 = wCvTUDEIgK19864547;     wCvTUDEIgK19864547 = wCvTUDEIgK37611282;     wCvTUDEIgK37611282 = wCvTUDEIgK19369069;     wCvTUDEIgK19369069 = wCvTUDEIgK19288048;     wCvTUDEIgK19288048 = wCvTUDEIgK79398826;     wCvTUDEIgK79398826 = wCvTUDEIgK26674826;     wCvTUDEIgK26674826 = wCvTUDEIgK31406471;     wCvTUDEIgK31406471 = wCvTUDEIgK65563048;     wCvTUDEIgK65563048 = wCvTUDEIgK80836159;     wCvTUDEIgK80836159 = wCvTUDEIgK87247451;     wCvTUDEIgK87247451 = wCvTUDEIgK25231216;     wCvTUDEIgK25231216 = wCvTUDEIgK88157967;     wCvTUDEIgK88157967 = wCvTUDEIgK65360370;     wCvTUDEIgK65360370 = wCvTUDEIgK16273967;     wCvTUDEIgK16273967 = wCvTUDEIgK1151544;     wCvTUDEIgK1151544 = wCvTUDEIgK73329044;     wCvTUDEIgK73329044 = wCvTUDEIgK20337350;     wCvTUDEIgK20337350 = wCvTUDEIgK53445201;     wCvTUDEIgK53445201 = wCvTUDEIgK54373086;     wCvTUDEIgK54373086 = wCvTUDEIgK91710639;     wCvTUDEIgK91710639 = wCvTUDEIgK51323707;     wCvTUDEIgK51323707 = wCvTUDEIgK91328712;     wCvTUDEIgK91328712 = wCvTUDEIgK18337359;     wCvTUDEIgK18337359 = wCvTUDEIgK65494683;     wCvTUDEIgK65494683 = wCvTUDEIgK45902759;     wCvTUDEIgK45902759 = wCvTUDEIgK43536873;     wCvTUDEIgK43536873 = wCvTUDEIgK7061783;     wCvTUDEIgK7061783 = wCvTUDEIgK12030175;     wCvTUDEIgK12030175 = wCvTUDEIgK60559611;     wCvTUDEIgK60559611 = wCvTUDEIgK41074263;     wCvTUDEIgK41074263 = wCvTUDEIgK30827816;     wCvTUDEIgK30827816 = wCvTUDEIgK12838042;     wCvTUDEIgK12838042 = wCvTUDEIgK21962210;     wCvTUDEIgK21962210 = wCvTUDEIgK57355895;     wCvTUDEIgK57355895 = wCvTUDEIgK97178181;     wCvTUDEIgK97178181 = wCvTUDEIgK48174245;     wCvTUDEIgK48174245 = wCvTUDEIgK96569707;     wCvTUDEIgK96569707 = wCvTUDEIgK63014851;     wCvTUDEIgK63014851 = wCvTUDEIgK19273976;     wCvTUDEIgK19273976 = wCvTUDEIgK90684439;     wCvTUDEIgK90684439 = wCvTUDEIgK72181629;     wCvTUDEIgK72181629 = wCvTUDEIgK35176905;     wCvTUDEIgK35176905 = wCvTUDEIgK11295974;     wCvTUDEIgK11295974 = wCvTUDEIgK97742129;     wCvTUDEIgK97742129 = wCvTUDEIgK54864172;     wCvTUDEIgK54864172 = wCvTUDEIgK57163790;     wCvTUDEIgK57163790 = wCvTUDEIgK16978260;     wCvTUDEIgK16978260 = wCvTUDEIgK68718511;     wCvTUDEIgK68718511 = wCvTUDEIgK18460091;     wCvTUDEIgK18460091 = wCvTUDEIgK34347459;     wCvTUDEIgK34347459 = wCvTUDEIgK53783655;     wCvTUDEIgK53783655 = wCvTUDEIgK68741807;     wCvTUDEIgK68741807 = wCvTUDEIgK32136695;     wCvTUDEIgK32136695 = wCvTUDEIgK19750738;     wCvTUDEIgK19750738 = wCvTUDEIgK60464420;     wCvTUDEIgK60464420 = wCvTUDEIgK28535835;     wCvTUDEIgK28535835 = wCvTUDEIgK21581554;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void EJcFBOJJUz61224012() {     int QjwUmlCIWF43968671 = -422390814;    int QjwUmlCIWF18851893 = -355924805;    int QjwUmlCIWF62085973 = -432415661;    int QjwUmlCIWF1943759 = -611926040;    int QjwUmlCIWF87442075 = -275313527;    int QjwUmlCIWF17523366 = -647469117;    int QjwUmlCIWF44273742 = -766949507;    int QjwUmlCIWF47366210 = 66784214;    int QjwUmlCIWF92461871 = -263580045;    int QjwUmlCIWF43827244 = -606416417;    int QjwUmlCIWF21002705 = -268471069;    int QjwUmlCIWF59229889 = -62696063;    int QjwUmlCIWF12598251 = -308663681;    int QjwUmlCIWF33839268 = -944958477;    int QjwUmlCIWF62610780 = -883125018;    int QjwUmlCIWF36310173 = -751120063;    int QjwUmlCIWF81465160 = -901396339;    int QjwUmlCIWF74212530 = -751167584;    int QjwUmlCIWF56476899 = -920283017;    int QjwUmlCIWF31531107 = 24980369;    int QjwUmlCIWF230929 = -111127190;    int QjwUmlCIWF97238804 = -178218788;    int QjwUmlCIWF72437345 = 40855030;    int QjwUmlCIWF43542515 = -587864207;    int QjwUmlCIWF21325774 = -329414381;    int QjwUmlCIWF4241772 = 77881764;    int QjwUmlCIWF29392320 = -828315572;    int QjwUmlCIWF4584253 = -500120771;    int QjwUmlCIWF24251826 = -384060429;    int QjwUmlCIWF12745280 = -237349770;    int QjwUmlCIWF45030757 = -193601344;    int QjwUmlCIWF49673324 = -527745158;    int QjwUmlCIWF66028251 = -463537272;    int QjwUmlCIWF39484074 = -569061042;    int QjwUmlCIWF21551122 = -745852874;    int QjwUmlCIWF24031500 = -845746841;    int QjwUmlCIWF78432189 = -847022769;    int QjwUmlCIWF73237735 = 49906764;    int QjwUmlCIWF55984633 = -619260672;    int QjwUmlCIWF94306719 = -555365365;    int QjwUmlCIWF50944379 = -71269517;    int QjwUmlCIWF75018172 = 28966162;    int QjwUmlCIWF17011569 = -432983155;    int QjwUmlCIWF11232246 = -530182679;    int QjwUmlCIWF87847813 = -908495118;    int QjwUmlCIWF45711564 = 16482531;    int QjwUmlCIWF31666467 = 67439728;    int QjwUmlCIWF63886722 = -36224274;    int QjwUmlCIWF73858732 = -316247352;    int QjwUmlCIWF23758572 = -80947536;    int QjwUmlCIWF40275688 = -974799881;    int QjwUmlCIWF39414305 = -677890747;    int QjwUmlCIWF59150563 = -669472230;    int QjwUmlCIWF47533997 = -788187011;    int QjwUmlCIWF60993083 = -607170052;    int QjwUmlCIWF46729866 = -144172027;    int QjwUmlCIWF46414548 = -296779836;    int QjwUmlCIWF18543458 = -844551454;    int QjwUmlCIWF80617985 = -182511660;    int QjwUmlCIWF83200303 = -253195291;    int QjwUmlCIWF88131046 = -819153546;    int QjwUmlCIWF39689490 = -166828736;    int QjwUmlCIWF23114384 = -549155357;    int QjwUmlCIWF79716591 = 73769725;    int QjwUmlCIWF98796486 = -312815074;    int QjwUmlCIWF71329380 = -740725912;    int QjwUmlCIWF93201637 = -599158791;    int QjwUmlCIWF73114176 = -739602639;    int QjwUmlCIWF12288147 = -99105603;    int QjwUmlCIWF38579281 = 62621823;    int QjwUmlCIWF57877983 = -904097294;    int QjwUmlCIWF8227426 = -851303104;    int QjwUmlCIWF18227897 = -31906912;    int QjwUmlCIWF62170179 = -264917652;    int QjwUmlCIWF80586728 = -903750115;    int QjwUmlCIWF25212757 = -40093352;    int QjwUmlCIWF80227236 = -745235633;    int QjwUmlCIWF61205099 = -428962292;    int QjwUmlCIWF55694701 = -679369090;    int QjwUmlCIWF75614210 = -245896912;    int QjwUmlCIWF72575305 = -989557965;    int QjwUmlCIWF65505597 = -692091298;    int QjwUmlCIWF30725521 = -83873419;    int QjwUmlCIWF493255 = -203112894;    int QjwUmlCIWF72469592 = -262549890;    int QjwUmlCIWF5616453 = -515710597;    int QjwUmlCIWF90522761 = -858272928;    int QjwUmlCIWF18494255 = -675350261;    int QjwUmlCIWF78490991 = -961890990;    int QjwUmlCIWF74821256 = -501680847;    int QjwUmlCIWF77616951 = -448967006;    int QjwUmlCIWF59888732 = 97528685;    int QjwUmlCIWF92619750 = -767581576;    int QjwUmlCIWF72784330 = -266065382;    int QjwUmlCIWF6175673 = -736211819;    int QjwUmlCIWF11254890 = -904440781;    int QjwUmlCIWF51903788 = -421878482;    int QjwUmlCIWF37294978 = -406752880;    int QjwUmlCIWF12435760 = -117367605;    int QjwUmlCIWF16518433 = -422390814;     QjwUmlCIWF43968671 = QjwUmlCIWF18851893;     QjwUmlCIWF18851893 = QjwUmlCIWF62085973;     QjwUmlCIWF62085973 = QjwUmlCIWF1943759;     QjwUmlCIWF1943759 = QjwUmlCIWF87442075;     QjwUmlCIWF87442075 = QjwUmlCIWF17523366;     QjwUmlCIWF17523366 = QjwUmlCIWF44273742;     QjwUmlCIWF44273742 = QjwUmlCIWF47366210;     QjwUmlCIWF47366210 = QjwUmlCIWF92461871;     QjwUmlCIWF92461871 = QjwUmlCIWF43827244;     QjwUmlCIWF43827244 = QjwUmlCIWF21002705;     QjwUmlCIWF21002705 = QjwUmlCIWF59229889;     QjwUmlCIWF59229889 = QjwUmlCIWF12598251;     QjwUmlCIWF12598251 = QjwUmlCIWF33839268;     QjwUmlCIWF33839268 = QjwUmlCIWF62610780;     QjwUmlCIWF62610780 = QjwUmlCIWF36310173;     QjwUmlCIWF36310173 = QjwUmlCIWF81465160;     QjwUmlCIWF81465160 = QjwUmlCIWF74212530;     QjwUmlCIWF74212530 = QjwUmlCIWF56476899;     QjwUmlCIWF56476899 = QjwUmlCIWF31531107;     QjwUmlCIWF31531107 = QjwUmlCIWF230929;     QjwUmlCIWF230929 = QjwUmlCIWF97238804;     QjwUmlCIWF97238804 = QjwUmlCIWF72437345;     QjwUmlCIWF72437345 = QjwUmlCIWF43542515;     QjwUmlCIWF43542515 = QjwUmlCIWF21325774;     QjwUmlCIWF21325774 = QjwUmlCIWF4241772;     QjwUmlCIWF4241772 = QjwUmlCIWF29392320;     QjwUmlCIWF29392320 = QjwUmlCIWF4584253;     QjwUmlCIWF4584253 = QjwUmlCIWF24251826;     QjwUmlCIWF24251826 = QjwUmlCIWF12745280;     QjwUmlCIWF12745280 = QjwUmlCIWF45030757;     QjwUmlCIWF45030757 = QjwUmlCIWF49673324;     QjwUmlCIWF49673324 = QjwUmlCIWF66028251;     QjwUmlCIWF66028251 = QjwUmlCIWF39484074;     QjwUmlCIWF39484074 = QjwUmlCIWF21551122;     QjwUmlCIWF21551122 = QjwUmlCIWF24031500;     QjwUmlCIWF24031500 = QjwUmlCIWF78432189;     QjwUmlCIWF78432189 = QjwUmlCIWF73237735;     QjwUmlCIWF73237735 = QjwUmlCIWF55984633;     QjwUmlCIWF55984633 = QjwUmlCIWF94306719;     QjwUmlCIWF94306719 = QjwUmlCIWF50944379;     QjwUmlCIWF50944379 = QjwUmlCIWF75018172;     QjwUmlCIWF75018172 = QjwUmlCIWF17011569;     QjwUmlCIWF17011569 = QjwUmlCIWF11232246;     QjwUmlCIWF11232246 = QjwUmlCIWF87847813;     QjwUmlCIWF87847813 = QjwUmlCIWF45711564;     QjwUmlCIWF45711564 = QjwUmlCIWF31666467;     QjwUmlCIWF31666467 = QjwUmlCIWF63886722;     QjwUmlCIWF63886722 = QjwUmlCIWF73858732;     QjwUmlCIWF73858732 = QjwUmlCIWF23758572;     QjwUmlCIWF23758572 = QjwUmlCIWF40275688;     QjwUmlCIWF40275688 = QjwUmlCIWF39414305;     QjwUmlCIWF39414305 = QjwUmlCIWF59150563;     QjwUmlCIWF59150563 = QjwUmlCIWF47533997;     QjwUmlCIWF47533997 = QjwUmlCIWF60993083;     QjwUmlCIWF60993083 = QjwUmlCIWF46729866;     QjwUmlCIWF46729866 = QjwUmlCIWF46414548;     QjwUmlCIWF46414548 = QjwUmlCIWF18543458;     QjwUmlCIWF18543458 = QjwUmlCIWF80617985;     QjwUmlCIWF80617985 = QjwUmlCIWF83200303;     QjwUmlCIWF83200303 = QjwUmlCIWF88131046;     QjwUmlCIWF88131046 = QjwUmlCIWF39689490;     QjwUmlCIWF39689490 = QjwUmlCIWF23114384;     QjwUmlCIWF23114384 = QjwUmlCIWF79716591;     QjwUmlCIWF79716591 = QjwUmlCIWF98796486;     QjwUmlCIWF98796486 = QjwUmlCIWF71329380;     QjwUmlCIWF71329380 = QjwUmlCIWF93201637;     QjwUmlCIWF93201637 = QjwUmlCIWF73114176;     QjwUmlCIWF73114176 = QjwUmlCIWF12288147;     QjwUmlCIWF12288147 = QjwUmlCIWF38579281;     QjwUmlCIWF38579281 = QjwUmlCIWF57877983;     QjwUmlCIWF57877983 = QjwUmlCIWF8227426;     QjwUmlCIWF8227426 = QjwUmlCIWF18227897;     QjwUmlCIWF18227897 = QjwUmlCIWF62170179;     QjwUmlCIWF62170179 = QjwUmlCIWF80586728;     QjwUmlCIWF80586728 = QjwUmlCIWF25212757;     QjwUmlCIWF25212757 = QjwUmlCIWF80227236;     QjwUmlCIWF80227236 = QjwUmlCIWF61205099;     QjwUmlCIWF61205099 = QjwUmlCIWF55694701;     QjwUmlCIWF55694701 = QjwUmlCIWF75614210;     QjwUmlCIWF75614210 = QjwUmlCIWF72575305;     QjwUmlCIWF72575305 = QjwUmlCIWF65505597;     QjwUmlCIWF65505597 = QjwUmlCIWF30725521;     QjwUmlCIWF30725521 = QjwUmlCIWF493255;     QjwUmlCIWF493255 = QjwUmlCIWF72469592;     QjwUmlCIWF72469592 = QjwUmlCIWF5616453;     QjwUmlCIWF5616453 = QjwUmlCIWF90522761;     QjwUmlCIWF90522761 = QjwUmlCIWF18494255;     QjwUmlCIWF18494255 = QjwUmlCIWF78490991;     QjwUmlCIWF78490991 = QjwUmlCIWF74821256;     QjwUmlCIWF74821256 = QjwUmlCIWF77616951;     QjwUmlCIWF77616951 = QjwUmlCIWF59888732;     QjwUmlCIWF59888732 = QjwUmlCIWF92619750;     QjwUmlCIWF92619750 = QjwUmlCIWF72784330;     QjwUmlCIWF72784330 = QjwUmlCIWF6175673;     QjwUmlCIWF6175673 = QjwUmlCIWF11254890;     QjwUmlCIWF11254890 = QjwUmlCIWF51903788;     QjwUmlCIWF51903788 = QjwUmlCIWF37294978;     QjwUmlCIWF37294978 = QjwUmlCIWF12435760;     QjwUmlCIWF12435760 = QjwUmlCIWF16518433;     QjwUmlCIWF16518433 = QjwUmlCIWF43968671;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void nFPHofOTAw97868986() {     int tCkfxUAqlF66355788 = -56794778;    int tCkfxUAqlF3199393 = -304467860;    int tCkfxUAqlF90855251 = -672069505;    int tCkfxUAqlF16950448 = -618333572;    int tCkfxUAqlF85616329 = -866370528;    int tCkfxUAqlF32406483 = -508077495;    int tCkfxUAqlF36429481 = -588936813;    int tCkfxUAqlF23000069 = -403812430;    int tCkfxUAqlF89625002 = -147776084;    int tCkfxUAqlF35590828 = -349157750;    int tCkfxUAqlF58544564 = -265095470;    int tCkfxUAqlF15132839 = -313441356;    int tCkfxUAqlF79606431 = -927381001;    int tCkfxUAqlF76454020 = -372667234;    int tCkfxUAqlF99346057 = -510602731;    int tCkfxUAqlF80566085 = -32787605;    int tCkfxUAqlF31288512 = -78676158;    int tCkfxUAqlF80188946 = -102910274;    int tCkfxUAqlF97758529 = 44614858;    int tCkfxUAqlF10047391 = -227654084;    int tCkfxUAqlF1114037 = -102614498;    int tCkfxUAqlF61054023 = -682985101;    int tCkfxUAqlF75730668 = -135225562;    int tCkfxUAqlF70042302 = -51857065;    int tCkfxUAqlF56866021 = 49026495;    int tCkfxUAqlF92544766 = -366012460;    int tCkfxUAqlF76481740 = -585902064;    int tCkfxUAqlF10495703 = -288575225;    int tCkfxUAqlF31144387 = -728317548;    int tCkfxUAqlF21902460 = -157645637;    int tCkfxUAqlF89321562 = -274917496;    int tCkfxUAqlF7214515 = -79816512;    int tCkfxUAqlF47066922 = -992880767;    int tCkfxUAqlF98872761 = -893187892;    int tCkfxUAqlF97780486 = -809609297;    int tCkfxUAqlF65724368 = -61142218;    int tCkfxUAqlF71871901 = -579970721;    int tCkfxUAqlF26863836 = -917718708;    int tCkfxUAqlF4292765 = 79693296;    int tCkfxUAqlF14492434 = -691589394;    int tCkfxUAqlF79701750 = -574447183;    int tCkfxUAqlF63526563 = -828942529;    int tCkfxUAqlF22561761 = -320513739;    int tCkfxUAqlF10676366 = -527546328;    int tCkfxUAqlF55831080 = -717638595;    int tCkfxUAqlF53811845 = -165746565;    int tCkfxUAqlF43963864 = -510970525;    int tCkfxUAqlF8485397 = -892759265;    int tCkfxUAqlF68318637 = -265964051;    int tCkfxUAqlF20842317 = -222967795;    int tCkfxUAqlF49144904 = -229876158;    int tCkfxUAqlF13265562 = -8710029;    int tCkfxUAqlF37464966 = -341383460;    int tCkfxUAqlF7820543 = 36132180;    int tCkfxUAqlF96754950 = -747874168;    int tCkfxUAqlF5301766 = -373809677;    int tCkfxUAqlF27468725 = -69242299;    int tCkfxUAqlF20812950 = -520212441;    int tCkfxUAqlF60084427 = -567360067;    int tCkfxUAqlF93071563 = -400358068;    int tCkfxUAqlF55924743 = -922175431;    int tCkfxUAqlF25933779 = -200361589;    int tCkfxUAqlF91855681 = -675494882;    int tCkfxUAqlF67722543 = -990130448;    int tCkfxUAqlF46269266 = 25759745;    int tCkfxUAqlF51330049 = -85278958;    int tCkfxUAqlF68065916 = -320560590;    int tCkfxUAqlF80733669 = 65806891;    int tCkfxUAqlF78673534 = -563057938;    int tCkfxUAqlF33621689 = -349460514;    int tCkfxUAqlF8694184 = -452816885;    int tCkfxUAqlF4424677 = -160957451;    int tCkfxUAqlF75896181 = -82603571;    int tCkfxUAqlF83266095 = -263795748;    int tCkfxUAqlF30345641 = -653206902;    int tCkfxUAqlF37587473 = -273671970;    int tCkfxUAqlF38492263 = -262471363;    int tCkfxUAqlF65054303 = -607679235;    int tCkfxUAqlF14211222 = -334218470;    int tCkfxUAqlF3054176 = -785226941;    int tCkfxUAqlF48580902 = -855041936;    int tCkfxUAqlF67996343 = -693142800;    int tCkfxUAqlF42177066 = 77388826;    int tCkfxUAqlF10302070 = -405349754;    int tCkfxUAqlF72757555 = -927769479;    int tCkfxUAqlF76056000 = -166207467;    int tCkfxUAqlF69749549 = -738433053;    int tCkfxUAqlF39246380 = -929012947;    int tCkfxUAqlF2117812 = -45313725;    int tCkfxUAqlF92478721 = -335799620;    int tCkfxUAqlF38255644 = -991899919;    int tCkfxUAqlF51058952 = 40241720;    int tCkfxUAqlF66779409 = -250358641;    int tCkfxUAqlF11221202 = -519948636;    int tCkfxUAqlF58567691 = -769413964;    int tCkfxUAqlF53767971 = -274085594;    int tCkfxUAqlF71670881 = -53447647;    int tCkfxUAqlF54839218 = -330383291;    int tCkfxUAqlF64407100 = -453306073;    int tCkfxUAqlF4501031 = -56794778;     tCkfxUAqlF66355788 = tCkfxUAqlF3199393;     tCkfxUAqlF3199393 = tCkfxUAqlF90855251;     tCkfxUAqlF90855251 = tCkfxUAqlF16950448;     tCkfxUAqlF16950448 = tCkfxUAqlF85616329;     tCkfxUAqlF85616329 = tCkfxUAqlF32406483;     tCkfxUAqlF32406483 = tCkfxUAqlF36429481;     tCkfxUAqlF36429481 = tCkfxUAqlF23000069;     tCkfxUAqlF23000069 = tCkfxUAqlF89625002;     tCkfxUAqlF89625002 = tCkfxUAqlF35590828;     tCkfxUAqlF35590828 = tCkfxUAqlF58544564;     tCkfxUAqlF58544564 = tCkfxUAqlF15132839;     tCkfxUAqlF15132839 = tCkfxUAqlF79606431;     tCkfxUAqlF79606431 = tCkfxUAqlF76454020;     tCkfxUAqlF76454020 = tCkfxUAqlF99346057;     tCkfxUAqlF99346057 = tCkfxUAqlF80566085;     tCkfxUAqlF80566085 = tCkfxUAqlF31288512;     tCkfxUAqlF31288512 = tCkfxUAqlF80188946;     tCkfxUAqlF80188946 = tCkfxUAqlF97758529;     tCkfxUAqlF97758529 = tCkfxUAqlF10047391;     tCkfxUAqlF10047391 = tCkfxUAqlF1114037;     tCkfxUAqlF1114037 = tCkfxUAqlF61054023;     tCkfxUAqlF61054023 = tCkfxUAqlF75730668;     tCkfxUAqlF75730668 = tCkfxUAqlF70042302;     tCkfxUAqlF70042302 = tCkfxUAqlF56866021;     tCkfxUAqlF56866021 = tCkfxUAqlF92544766;     tCkfxUAqlF92544766 = tCkfxUAqlF76481740;     tCkfxUAqlF76481740 = tCkfxUAqlF10495703;     tCkfxUAqlF10495703 = tCkfxUAqlF31144387;     tCkfxUAqlF31144387 = tCkfxUAqlF21902460;     tCkfxUAqlF21902460 = tCkfxUAqlF89321562;     tCkfxUAqlF89321562 = tCkfxUAqlF7214515;     tCkfxUAqlF7214515 = tCkfxUAqlF47066922;     tCkfxUAqlF47066922 = tCkfxUAqlF98872761;     tCkfxUAqlF98872761 = tCkfxUAqlF97780486;     tCkfxUAqlF97780486 = tCkfxUAqlF65724368;     tCkfxUAqlF65724368 = tCkfxUAqlF71871901;     tCkfxUAqlF71871901 = tCkfxUAqlF26863836;     tCkfxUAqlF26863836 = tCkfxUAqlF4292765;     tCkfxUAqlF4292765 = tCkfxUAqlF14492434;     tCkfxUAqlF14492434 = tCkfxUAqlF79701750;     tCkfxUAqlF79701750 = tCkfxUAqlF63526563;     tCkfxUAqlF63526563 = tCkfxUAqlF22561761;     tCkfxUAqlF22561761 = tCkfxUAqlF10676366;     tCkfxUAqlF10676366 = tCkfxUAqlF55831080;     tCkfxUAqlF55831080 = tCkfxUAqlF53811845;     tCkfxUAqlF53811845 = tCkfxUAqlF43963864;     tCkfxUAqlF43963864 = tCkfxUAqlF8485397;     tCkfxUAqlF8485397 = tCkfxUAqlF68318637;     tCkfxUAqlF68318637 = tCkfxUAqlF20842317;     tCkfxUAqlF20842317 = tCkfxUAqlF49144904;     tCkfxUAqlF49144904 = tCkfxUAqlF13265562;     tCkfxUAqlF13265562 = tCkfxUAqlF37464966;     tCkfxUAqlF37464966 = tCkfxUAqlF7820543;     tCkfxUAqlF7820543 = tCkfxUAqlF96754950;     tCkfxUAqlF96754950 = tCkfxUAqlF5301766;     tCkfxUAqlF5301766 = tCkfxUAqlF27468725;     tCkfxUAqlF27468725 = tCkfxUAqlF20812950;     tCkfxUAqlF20812950 = tCkfxUAqlF60084427;     tCkfxUAqlF60084427 = tCkfxUAqlF93071563;     tCkfxUAqlF93071563 = tCkfxUAqlF55924743;     tCkfxUAqlF55924743 = tCkfxUAqlF25933779;     tCkfxUAqlF25933779 = tCkfxUAqlF91855681;     tCkfxUAqlF91855681 = tCkfxUAqlF67722543;     tCkfxUAqlF67722543 = tCkfxUAqlF46269266;     tCkfxUAqlF46269266 = tCkfxUAqlF51330049;     tCkfxUAqlF51330049 = tCkfxUAqlF68065916;     tCkfxUAqlF68065916 = tCkfxUAqlF80733669;     tCkfxUAqlF80733669 = tCkfxUAqlF78673534;     tCkfxUAqlF78673534 = tCkfxUAqlF33621689;     tCkfxUAqlF33621689 = tCkfxUAqlF8694184;     tCkfxUAqlF8694184 = tCkfxUAqlF4424677;     tCkfxUAqlF4424677 = tCkfxUAqlF75896181;     tCkfxUAqlF75896181 = tCkfxUAqlF83266095;     tCkfxUAqlF83266095 = tCkfxUAqlF30345641;     tCkfxUAqlF30345641 = tCkfxUAqlF37587473;     tCkfxUAqlF37587473 = tCkfxUAqlF38492263;     tCkfxUAqlF38492263 = tCkfxUAqlF65054303;     tCkfxUAqlF65054303 = tCkfxUAqlF14211222;     tCkfxUAqlF14211222 = tCkfxUAqlF3054176;     tCkfxUAqlF3054176 = tCkfxUAqlF48580902;     tCkfxUAqlF48580902 = tCkfxUAqlF67996343;     tCkfxUAqlF67996343 = tCkfxUAqlF42177066;     tCkfxUAqlF42177066 = tCkfxUAqlF10302070;     tCkfxUAqlF10302070 = tCkfxUAqlF72757555;     tCkfxUAqlF72757555 = tCkfxUAqlF76056000;     tCkfxUAqlF76056000 = tCkfxUAqlF69749549;     tCkfxUAqlF69749549 = tCkfxUAqlF39246380;     tCkfxUAqlF39246380 = tCkfxUAqlF2117812;     tCkfxUAqlF2117812 = tCkfxUAqlF92478721;     tCkfxUAqlF92478721 = tCkfxUAqlF38255644;     tCkfxUAqlF38255644 = tCkfxUAqlF51058952;     tCkfxUAqlF51058952 = tCkfxUAqlF66779409;     tCkfxUAqlF66779409 = tCkfxUAqlF11221202;     tCkfxUAqlF11221202 = tCkfxUAqlF58567691;     tCkfxUAqlF58567691 = tCkfxUAqlF53767971;     tCkfxUAqlF53767971 = tCkfxUAqlF71670881;     tCkfxUAqlF71670881 = tCkfxUAqlF54839218;     tCkfxUAqlF54839218 = tCkfxUAqlF64407100;     tCkfxUAqlF64407100 = tCkfxUAqlF4501031;     tCkfxUAqlF4501031 = tCkfxUAqlF66355788;}
// Junk Finished
