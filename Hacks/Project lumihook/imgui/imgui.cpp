// dear imgui, v1.52 WIP
// (main code and documentation)

// See ImGui::ShowTestWindow() in imgui_demo.cpp for demo code.
// Newcomers, read 'Programmer guide' below for notes on how to setup Dear ImGui in your codebase.
// Get latest version at https://github.com/ocornut/imgui
// Releases change-log at https://github.com/ocornut/imgui/releases
// Gallery (please post your screenshots/video there!): https://github.com/ocornut/imgui/issues/1269
// Developed by Omar Cornut and every direct or indirect contributors to the GitHub.
// This library is free but I need your support to sustain development and maintenance.
// If you work for a company, please consider financial support, e.g: https://www.patreon.com/imgui

/*

 Index
 - MISSION STATEMENT
 - END-USER GUIDE
 - PROGRAMMER GUIDE (read me!)
   - Read first
   - How to update to a newer version of Dear ImGui
   - Getting started with integrating Dear ImGui in your code/engine
 - API BREAKING CHANGES (read me when you update!)
 - ISSUES & TODO LIST
 - FREQUENTLY ASKED QUESTIONS (FAQ), TIPS
   - How can I help?
   - What is ImTextureID and how do I display an image?
   - I integrated Dear ImGui in my engine and the text or lines are blurry..
   - I integrated Dear ImGui in my engine and some elements are clipping or disappearing when I move windows around..
   - How can I have multiple widgets with the same label? Can I have widget without a label? (Yes). A primer on labels/IDs.
   - How can I tell when Dear ImGui wants my mouse/keyboard inputs VS when I can pass them to my application?
   - How can I load a different font than the default?
   - How can I easily use icons in my application?
   - How can I load multiple fonts?
   - How can I display and input non-latin characters such as Chinese, Japanese, Korean, Cyrillic?
   - How can I preserve my Dear ImGui context across reloading a DLL? (loss of the global/static variables)
   - How can I use the drawing facilities without an ImGui window? (using ImDrawList API)
 - ISSUES & TODO-LIST
 - CODE


 MISSION STATEMENT
 =================

 - Easy to use to create code-driven and data-driven tools
 - Easy to use to create ad hoc short-lived tools and long-lived, more elaborate tools
 - Easy to hack and improve
 - Minimize screen real-estate usage
 - Minimize setup and maintenance
 - Minimize state storage on user side
 - Portable, minimize dependencies, run on target (consoles, phones, etc.)
 - Efficient runtime and memory consumption (NB- we do allocate when "growing" content e.g. creating a window, opening a tree node 
   for the first time, etc. but a typical frame won't allocate anything)

 Designed for developers and content-creators, not the typical end-user! Some of the weaknesses includes:
 - Doesn't look fancy, doesn't animate
 - Limited layout features, intricate layouts are typically crafted in code


 END-USER GUIDE
 ==============

 - Double-click title bar to collapse window
 - Click upper right corner to close a window, available when 'bool* p_open' is passed to ImGui::Begin()
 - Click and drag on lower right corner to resize window
 - Click and drag on any empty space to move window
 - Double-click/double-tap on lower right corner grip to auto-fit to content
 - TAB/SHIFT+TAB to cycle through keyboard editable fields
 - Use mouse wheel to scroll
 - Use CTRL+mouse wheel to zoom window contents (if io.FontAllowScaling is true)
 - CTRL+Click on a slider or drag box to input value as text
 - Text editor:
   - Hold SHIFT or use mouse to select text.
   - CTRL+Left/Right to word jump
   - CTRL+Shift+Left/Right to select words
   - CTRL+A our Double-Click to select all
   - CTRL+X,CTRL+C,CTRL+V to use OS clipboard
   - CTRL+Z,CTRL+Y to undo/redo
   - ESCAPE to revert text to its original value
   - You can apply arithmetic operators +,*,/ on numerical values. Use +- to subtract (because - would set a negative value!)
   - Controls are automatically adjusted for OSX to match standard OSX text editing operations.


 PROGRAMMER GUIDE
 ================

 READ FIRST

 - Read the FAQ below this section!
 - Your code creates the UI, if your code doesn't run the UI is gone! == very dynamic UI, no construction/destructions steps, less data retention
   on your side, no state duplication, less sync, less bugs.
 - Call and read ImGui::ShowTestWindow() for demo code demonstrating most features.
 - You can learn about immediate-mode gui principles at http://www.johno.se/book/imgui.html or watch http://mollyrocket.com/861

 HOW TO UPDATE TO A NEWER VERSION OF DEAR IMGUI

 - Overwrite all the sources files except for imconfig.h (if you have made modification to your copy of imconfig.h)
 - Read the "API BREAKING CHANGES" section (below). This is where we list occasional API breaking changes. 
   If a function/type has been renamed / or marked obsolete, try to fix the name in your code before it is permanently removed from the public API.
   If you have a problem with a missing function/symbols, search for its name in the code, there will likely be a comment about it. 
   Please report any issue to the GitHub page!
 - Try to keep your copy of dear imgui reasonably up to date.

 GETTING STARTED WITH INTEGRATING DEAR IMGUI IN YOUR CODE/ENGINE

 - Add the Dear ImGui source files to your projects, using your preferred build system. 
   It is recommended you build the .cpp files as part of your project and not as a library.
 - You can later customize the imconfig.h file to tweak some compilation time behavior, such as integrating imgui types with your own maths types.
 - See examples/ folder for standalone sample applications. To understand the integration process, you can read examples/opengl2_example/ because 
   it is short, then switch to the one more appropriate to your use case.
 - You may be able to grab and copy a ready made imgui_impl_*** file from the examples/.
 - When using Dear ImGui, your programming IDE is your friend: follow the declaration of variables, functions and types to find comments about them.

 - Init: retrieve the ImGuiIO structure with ImGui::GetIO() and fill the fields marked 'Settings': at minimum you need to set io.DisplaySize
   (application resolution). Later on you will fill your keyboard mapping, clipboard handlers, and other advanced features but for a basic 
   integration you don't need to worry about it all.
 - Init: call io.Fonts->GetTexDataAsRGBA32(...), it will build the font atlas texture, then load the texture pixels into graphics memory.
 - Every frame:
    - In your main loop as early a possible, fill the IO fields marked 'Input' (e.g. mouse position, buttons, keyboard info, etc.)
    - Call ImGui::NewFrame() to begin the frame
    - You can use any ImGui function you want between NewFrame() and Render()
    - Call ImGui::Render() as late as you can to end the frame and finalize render data. it will call your io.RenderDrawListFn handler.
       (Even if you don't render, call Render() and ignore the callback, or call EndFrame() instead. Otherwhise some features will break)
 - All rendering information are stored into command-lists until ImGui::Render() is called.
 - Dear ImGui never touches or knows about your GPU state. the only function that knows about GPU is the RenderDrawListFn handler that you provide.
 - Effectively it means you can create widgets at any time in your code, regardless of considerations of being in "update" vs "render" phases 
   of your own application.
 - Refer to the examples applications in the examples/ folder for instruction on how to setup your code.
 - A minimal application skeleton may be:

     // Application init
     ImGuiIO& io = ImGui::GetIO();
     io.DisplaySize.x = 1920.0f;
     io.DisplaySize.y = 1280.0f;
     io.RenderDrawListsFn = MyRenderFunction;  // Setup a render function, or set to NULL and call GetDrawData() after Render() to access render data.
     // TODO: Fill others settings of the io structure later.

     // Load texture atlas (there is a default font so you don't need to care about choosing a font yet)
     unsigned char* pixels;
     int width, height;
     io.Fonts->GetTexDataAsRGBA32(pixels, &width, &height);
     // TODO: At this points you've got the texture data and you need to upload that your your graphic system:
     MyTexture* texture = MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA)
     // TODO: Store your texture pointer/identifier (whatever your engine uses) in 'io.Fonts->TexID'. This will be passed back to your via the renderer.
     io.Fonts->TexID = (void*)texture;

     // Application main loop
     while (true)
     {
        // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = 1.0f/60.0f;
        io.MousePos = mouse_pos;
        io.MouseDown[0] = mouse_button_0;
        io.MouseDown[1] = mouse_button_1;

        // Call NewFrame(), after this point you can use ImGui::* functions anytime
        ImGui::NewFrame();

        // Most of your application code here
        MyGameUpdate(); // may use any ImGui functions, e.g. ImGui::Begin("My window"); ImGui::Text("Hello, world!"); ImGui::End();
        MyGameRender(); // may use any ImGui functions as well!
     
        // Render & swap video buffers
        ImGui::Render();
        SwapBuffers();
     }

 - A minimal render function skeleton may be:

    void void MyRenderFunction(ImDrawData* draw_data)(ImDrawData* draw_data)
    {
       // TODO: Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
       // TODO: Setup viewport, orthographic projection matrix
       // TODO: Setup shader: vertex { float2 pos, float2 uv, u32 color }, fragment shader sample color from 1 texture, multiply by vertex color.
       for (int n = 0; n < draw_data->CmdListsCount; n++)
       {
          const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;  // vertex buffer generated by ImGui
          const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;   // index buffer generated by ImGui
          for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
          {
             const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
             if (pcmd->UserCallback)
             {
                 pcmd->UserCallback(cmd_list, pcmd);
             }
             else
             {
                 // The texture for the draw call is specified by pcmd->TextureId. 
                 // The vast majority of draw calls with use the imgui texture atlas, which value you have set yourself during initialization. 
                 MyEngineBindTexture(pcmd->TextureId);

                 // We are using scissoring to clip some objects. All low-level graphics API supports it.
                 // If your engine doesn't support scissoring yet, you will get some small glitches (some elements outside their bounds) which you can fix later.
                 MyEngineScissor((int)pcmd->ClipRect.x, (int)pcmd->ClipRect.y, (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));

                 // Render 'pcmd->ElemCount/3' indexed triangles.
                 // By default the indices ImDrawIdx are 16-bits, you can change them to 32-bits if your engine doesn't support 16-bits indices.
                 MyEngineDrawIndexedTriangles(pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer, vtx_buffer);
             }
             idx_buffer += pcmd->ElemCount;
          }
       }
    }

 - The examples/ folders contains many functional implementation of the pseudo-code above.
 - When calling NewFrame(), the 'io.WantCaptureMouse'/'io.WantCaptureKeyboard'/'io.WantTextInput' flags are updated. 
   They tell you if ImGui intends to use your inputs. So for example, if 'io.WantCaptureMouse' is set you would typically want to hide 
   mouse inputs from the rest of your application. Read the FAQ below for more information about those flags.



 API BREAKING CHANGES
 ====================

 Occasionally introducing changes that are breaking the API. The breakage are generally minor and easy to fix.
 Here is a change-log of API breaking changes, if you are using one of the functions listed, expect to have to fix some code.
 Also read releases logs https://github.com/ocornut/imgui/releases for more details.

 - 2017/10/17 (1.52) - marked the old 5-parameters version of Begin() as obsolete (still available). Use SetNextWindowSize()+Begin() instead!
 - 2017/10/11 (1.52) - renamed AlignFirstTextHeightToWidgets() to AlignTextToFramePadding(). Kept inline redirection function (will obsolete).
 - 2017/09/25 (1.52) - removed SetNextWindowPosCenter() because SetNextWindowPos() now has the optional pivot information to do the same and more. Kept redirection function (will obsolete). 
 - 2017/08/25 (1.52) - io.MousePos needs to be set to ImVec2(-FLT_MAX,-FLT_MAX) when mouse is unavailable/missing. Previously ImVec2(-1,-1) was enough but we now accept negative mouse coordinates. In your binding if you need to support unavailable mouse, make sure to replace "io.MousePos = ImVec2(-1,-1)" with "io.MousePos = ImVec2(-FLT_MAX,-FLT_MAX)".
 - 2017/08/22 (1.51) - renamed IsItemHoveredRect() to IsItemRectHovered(). Kept inline redirection function (will obsolete).
                     - renamed IsMouseHoveringAnyWindow() to IsAnyWindowHovered() for consistency. Kept inline redirection function (will obsolete).
                     - renamed IsMouseHoveringWindow() to IsWindowRectHovered() for consistency. Kept inline redirection function (will obsolete).
 - 2017/08/20 (1.51) - renamed GetStyleColName() to GetStyleColorName() for consistency.
 - 2017/08/20 (1.51) - added PushStyleColor(ImGuiCol idx, ImU32 col) overload, which _might_ cause an "ambiguous call" compilation error if you are using ImColor() with implicit cast. Cast to ImU32 or ImVec4 explicily to fix.
 - 2017/08/15 (1.51) - marked the weird IMGUI_ONCE_UPON_A_FRAME helper macro as obsolete. prefer using the more explicit ImGuiOnceUponAFrame.
 - 2017/08/15 (1.51) - changed parameter order for BeginPopupContextWindow() from (const char*,int buttons,bool also_over_items) to (const char*,int buttons,bool also_over_items). Note that most calls relied on default parameters completely.
 - 2017/08/13 (1.51) - renamed ImGuiCol_Columns*** to ImGuiCol_Separator***. Kept redirection enums (will obsolete).
 - 2017/08/11 (1.51) - renamed ImGuiSetCond_*** types and flags to ImGuiCond_***. Kept redirection enums (will obsolete).
 - 2017/08/09 (1.51) - removed ValueColor() helpers, they are equivalent to calling Text(label) + SameLine() + ColorButton().
 - 2017/08/08 (1.51) - removed ColorEditMode() and ImGuiColorEditMode in favor of ImGuiColorEditFlags and parameters to the various Color*() functions. The SetColorEditOptions() allows to initialize default but the user can still change them with right-click context menu.
                     - changed prototype of 'ColorEdit4(const char* label, float col[4], bool show_alpha = true)' to 'ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0)', where passing flags = 0x01 is a safe no-op (hello dodgy backward compatibility!). - check and run the demo window, under "Color/Picker Widgets", to understand the various new options.
                     - changed prototype of rarely used 'ColorButton(ImVec4 col, bool small_height = false, bool outline_border = true)' to 'ColorButton(const char* desc_id, ImVec4 col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0,0))'
 - 2017/07/20 (1.51) - removed IsPosHoveringAnyWindow(ImVec2), which was partly broken and misleading. ASSERT + redirect user to io.WantCaptureMouse
 - 2017/05/26 (1.50) - removed ImFontConfig::MergeGlyphCenterV in favor of a more multipurpose ImFontConfig::GlyphOffset.
 - 2017/05/01 (1.50) - renamed ImDrawList::PathFill() (rarely used directly) to ImDrawList::PathFillConvex() for clarity.
 - 2016/11/06 (1.50) - BeginChild(const char*) now applies the stack id to the provided label, consistently with other functions as it should always have been. It shouldn't affect you unless (extremely unlikely) you were appending multiple times to a same child from different locations of the stack id. If that's the case, generate an id with GetId() and use it instead of passing string to BeginChild().
 - 2016/10/15 (1.50) - avoid 'void* user_data' parameter to io.SetClipboardTextFn/io.GetClipboardTextFn pointers. We pass io.ClipboardUserData to it.
 - 2016/09/25 (1.50) - style.WindowTitleAlign is now a ImVec2 (ImGuiAlign enum was removed). set to (0.5f,0.5f) for horizontal+vertical centering, (0.0f,0.0f) for upper-left, etc.
 - 2016/07/30 (1.50) - SameLine(x) with x>0.0f is now relative to left of column/group if any, and not always to left of window. This was sort of always the intent and hopefully breakage should be minimal.
 - 2016/05/12 (1.49) - title bar (using ImGuiCol_TitleBg/ImGuiCol_TitleBgActive colors) isn't rendered over a window background (ImGuiCol_WindowBg color) anymore. 
                       If your TitleBg/TitleBgActive alpha was 1.0f or you are using the default theme it will not affect you. 
                       However if your TitleBg/TitleBgActive alpha was <1.0f you need to tweak your custom theme to readjust for the fact that we don't draw a WindowBg background behind the title bar.
                       This helper function will convert an old TitleBg/TitleBgActive color into a new one with the same visual output, given the OLD color and the OLD WindowBg color.
                           ImVec4 ConvertTitleBgCol(const ImVec4& win_bg_col, const ImVec4& title_bg_col)
                           {
                               float new_a = 1.0f - ((1.0f - win_bg_col.w) * (1.0f - title_bg_col.w)), k = title_bg_col.w / new_a;
                               return ImVec4((win_bg_col.x * win_bg_col.w + title_bg_col.x) * k, (win_bg_col.y * win_bg_col.w + title_bg_col.y) * k, (win_bg_col.z * win_bg_col.w + title_bg_col.z) * k, new_a);
                           }
                       If this is confusing, pick the RGB value from title bar from an old screenshot and apply this as TitleBg/TitleBgActive. Or you may just create TitleBgActive from a tweaked TitleBg color.
 - 2016/05/07 (1.49) - removed confusing set of GetInternalState(), GetInternalStateSize(), SetInternalState() functions. Now using CreateContext(), DestroyContext(), GetCurrentContext(), SetCurrentContext().
 - 2016/05/02 (1.49) - renamed SetNextTreeNodeOpened() to SetNextTreeNodeOpen(), no redirection.
 - 2016/05/01 (1.49) - obsoleted old signature of CollapsingHeader(const char* label, const char* str_id = NULL, bool display_frame = true, bool default_open = false) as extra parameters were badly designed and rarely used. You can replace the "default_open = true" flag in new API with CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen).
 - 2016/04/26 (1.49) - changed ImDrawList::PushClipRect(ImVec4 rect) to ImDraw::PushClipRect(Imvec2 min,ImVec2 max,bool intersect_with_current_clip_rect=false). Note that higher-level ImGui::PushClipRect() is preferable because it will clip at logic/widget level, whereas ImDrawList::PushClipRect() only affect your renderer.
 - 2016/04/03 (1.48) - removed style.WindowFillAlphaDefault setting which was redundant. Bake default BG alpha inside style.Colors[ImGuiCol_WindowBg] and all other Bg color values. (ref github issue #337).
 - 2016/04/03 (1.48) - renamed ImGuiCol_TooltipBg to ImGuiCol_PopupBg, used by popups/menus and tooltips. popups/menus were previously using ImGuiCol_WindowBg. (ref github issue #337)
 - 2016/03/21 (1.48) - renamed GetWindowFont() to GetFont(), GetWindowFontSize() to GetFontSize(). Kept inline redirection function (will obsolete).
 - 2016/03/02 (1.48) - InputText() completion/history/always callbacks: if you modify the text buffer manually (without using DeleteChars()/InsertChars() helper) you need to maintain the BufTextLen field. added an assert.
 - 2016/01/23 (1.48) - fixed not honoring exact width passed to PushItemWidth(), previously it would add extra FramePadding.x*2 over that width. if you had manual pixel-perfect alignment in place it might affect you.
 - 2015/12/27 (1.48) - fixed ImDrawList::AddRect() which used to render a rectangle 1 px too large on each axis.
 - 2015/12/04 (1.47) - renamed Color() helpers to ValueColor() - dangerously named, rarely used and probably to be made obsolete.
 - 2015/08/29 (1.45) - with the addition of horizontal scrollbar we made various fixes to inconsistencies with dealing with cursor position.
                       GetCursorPos()/SetCursorPos() functions now include the scrolled amount. It shouldn't affect the majority of users, but take note that SetCursorPosX(100.0f) puts you at +100 from the starting x position which may include scrolling, not at +100 from the window left side.
                       GetContentRegionMax()/GetWindowContentRegionMin()/GetWindowContentRegionMax() functions allow include the scrolled amount. Typically those were used in cases where no scrolling would happen so it may not be a problem, but watch out!
 - 2015/08/29 (1.45) - renamed style.ScrollbarWidth to style.ScrollbarSize
 - 2015/08/05 (1.44) - split imgui.cpp into extra files: imgui_demo.cpp imgui_draw.cpp imgui_internal.h that you need to add to your project.
 - 2015/07/18 (1.44) - fixed angles in ImDrawList::PathArcTo(), PathArcToFast() (introduced in 1.43) being off by an extra PI for no justifiable reason
 - 2015/07/14 (1.43) - add new ImFontAtlas::AddFont() API. For the old AddFont***, moved the 'font_no' parameter of ImFontAtlas::AddFont** functions to the ImFontConfig structure.
                       you need to render your textured triangles with bilinear filtering to benefit from sub-pixel positioning of text.
 - 2015/07/08 (1.43) - switched rendering data to use indexed rendering. this is saving a fair amount of CPU/GPU and enables us to get anti-aliasing for a marginal cost.
                       this necessary change will break your rendering function! the fix should be very easy. sorry for that :(
                     - if you are using a vanilla copy of one of the imgui_impl_XXXX.cpp provided in the example, you just need to update your copy and you can ignore the rest.
                     - the signature of the io.RenderDrawListsFn handler has changed!
                            ImGui_XXXX_RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
                       became:
                            ImGui_XXXX_RenderDrawLists(ImDrawData* draw_data).
                              argument   'cmd_lists'        -> 'draw_data->CmdLists'
                              argument   'cmd_lists_count'  -> 'draw_data->CmdListsCount'
                              ImDrawList 'commands'         -> 'CmdBuffer'
                              ImDrawList 'vtx_buffer'       -> 'VtxBuffer'
                              ImDrawList  n/a               -> 'IdxBuffer' (new)
                              ImDrawCmd  'vtx_count'        -> 'ElemCount'
                              ImDrawCmd  'clip_rect'        -> 'ClipRect'
                              ImDrawCmd  'user_callback'    -> 'UserCallback'
                              ImDrawCmd  'texture_id'       -> 'TextureId'
                     - each ImDrawList now contains both a vertex buffer and an index buffer. For each command, render ElemCount/3 triangles using indices from the index buffer.
                     - if you REALLY cannot render indexed primitives, you can call the draw_data->DeIndexAllBuffers() method to de-index the buffers. This is slow and a waste of CPU/GPU. Prefer using indexed rendering!
                     - refer to code in the examples/ folder or ask on the GitHub if you are unsure of how to upgrade. please upgrade!
 - 2015/07/10 (1.43) - changed SameLine() parameters from int to float.
 - 2015/07/02 (1.42) - renamed SetScrollPosHere() to SetScrollFromCursorPos(). Kept inline redirection function (will obsolete).
 - 2015/07/02 (1.42) - renamed GetScrollPosY() to GetScrollY(). Necessary to reduce confusion along with other scrolling functions, because positions (e.g. cursor position) are not equivalent to scrolling amount.
 - 2015/06/14 (1.41) - changed ImageButton() default bg_col parameter from (0,0,0,1) (black) to (0,0,0,0) (transparent) - makes a difference when texture have transparence
 - 2015/06/14 (1.41) - changed Selectable() API from (label, selected, size) to (label, selected, flags, size). Size override should have been rarely be used. Sorry!
 - 2015/05/31 (1.40) - renamed GetWindowCollapsed() to IsWindowCollapsed() for consistency. Kept inline redirection function (will obsolete).
 - 2015/05/31 (1.40) - renamed IsRectClipped() to IsRectVisible() for consistency. Note that return value is opposite! Kept inline redirection function (will obsolete).
 - 2015/05/27 (1.40) - removed the third 'repeat_if_held' parameter from Button() - sorry! it was rarely used and inconsistent. Use PushButtonRepeat(true) / PopButtonRepeat() to enable repeat on desired buttons.
 - 2015/05/11 (1.40) - changed BeginPopup() API, takes a string identifier instead of a bool. ImGui needs to manage the open/closed state of popups. Call OpenPopup() to actually set the "open" state of a popup. BeginPopup() returns true if the popup is opened.
 - 2015/05/03 (1.40) - removed style.AutoFitPadding, using style.WindowPadding makes more sense (the default values were already the same).
 - 2015/04/13 (1.38) - renamed IsClipped() to IsRectClipped(). Kept inline redirection function until 1.50.
 - 2015/04/09 (1.38) - renamed ImDrawList::AddArc() to ImDrawList::AddArcFast() for compatibility with future API
 - 2015/04/03 (1.38) - removed ImGuiCol_CheckHovered, ImGuiCol_CheckActive, replaced with the more general ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive.
 - 2014/04/03 (1.38) - removed support for passing -FLT_MAX..+FLT_MAX as the range for a SliderFloat(). Use DragFloat() or Inputfloat() instead.
 - 2015/03/17 (1.36) - renamed GetItemBoxMin()/GetItemBoxMax()/IsMouseHoveringBox() to GetItemRectMin()/GetItemRectMax()/IsMouseHoveringRect(). Kept inline redirection function until 1.50.
 - 2015/03/15 (1.36) - renamed style.TreeNodeSpacing to style.IndentSpacing, ImGuiStyleVar_TreeNodeSpacing to ImGuiStyleVar_IndentSpacing
 - 2015/03/13 (1.36) - renamed GetWindowIsFocused() to IsWindowFocused(). Kept inline redirection function until 1.50.
 - 2015/03/08 (1.35) - renamed style.ScrollBarWidth to style.ScrollbarWidth (casing)
 - 2015/02/27 (1.34) - renamed OpenNextNode(bool) to SetNextTreeNodeOpened(bool, ImGuiSetCond). Kept inline redirection function until 1.50.
 - 2015/02/27 (1.34) - renamed ImGuiSetCondition_*** to ImGuiSetCond_***, and _FirstUseThisSession becomes _Once.
 - 2015/02/11 (1.32) - changed text input callback ImGuiTextEditCallback return type from void-->int. reserved for future use, return 0 for now.
 - 2015/02/10 (1.32) - renamed GetItemWidth() to CalcItemWidth() to clarify its evolving behavior
 - 2015/02/08 (1.31) - renamed GetTextLineSpacing() to GetTextLineHeightWithSpacing()
 - 2015/02/01 (1.31) - removed IO.MemReallocFn (unused)
 - 2015/01/19 (1.30) - renamed ImGuiStorage::GetIntPtr()/GetFloatPtr() to GetIntRef()/GetIntRef() because Ptr was conflicting with actual pointer storage functions.
 - 2015/01/11 (1.30) - big font/image API change! now loads TTF file. allow for multiple fonts. no need for a PNG loader.
              (1.30) - removed GetDefaultFontData(). uses io.Fonts->GetTextureData*() API to retrieve uncompressed pixels.
                       this sequence:
                           const void* png_data;
                           unsigned int png_size;
                           ImGui::GetDefaultFontData(NULL, NULL, &png_data, &png_size);
                           // <Copy to GPU>
                       became:
                           unsigned char* pixels;
                           int width, height;
                           io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
                           // <Copy to GPU>
                           io.Fonts->TexID = (your_texture_identifier);
                       you now have much more flexibility to load multiple TTF fonts and manage the texture buffer for internal needs.
                       it is now recommended that you sample the font texture with bilinear interpolation.
              (1.30) - added texture identifier in ImDrawCmd passed to your render function (we can now render images). make sure to set io.Fonts->TexID.
              (1.30) - removed IO.PixelCenterOffset (unnecessary, can be handled in user projection matrix)
              (1.30) - removed ImGui::IsItemFocused() in favor of ImGui::IsItemActive() which handles all widgets
 - 2014/12/10 (1.18) - removed SetNewWindowDefaultPos() in favor of new generic API SetNextWindowPos(pos, ImGuiSetCondition_FirstUseEver)
 - 2014/11/28 (1.17) - moved IO.Font*** options to inside the IO.Font-> structure (FontYOffset, FontTexUvForWhite, FontBaseScale, FontFallbackGlyph)
 - 2014/11/26 (1.17) - reworked syntax of IMGUI_ONCE_UPON_A_FRAME helper macro to increase compiler compatibility
 - 2014/11/07 (1.15) - renamed IsHovered() to IsItemHovered()
 - 2014/10/02 (1.14) - renamed IMGUI_INCLUDE_IMGUI_USER_CPP to IMGUI_INCLUDE_IMGUI_USER_INL and imgui_user.cpp to imgui_user.inl (more IDE friendly)
 - 2014/09/25 (1.13) - removed 'text_end' parameter from IO.SetClipboardTextFn (the string is now always zero-terminated for simplicity)
 - 2014/09/24 (1.12) - renamed SetFontScale() to SetWindowFontScale()
 - 2014/09/24 (1.12) - moved IM_MALLOC/IM_REALLOC/IM_FREE preprocessor defines to IO.MemAllocFn/IO.MemReallocFn/IO.MemFreeFn
 - 2014/08/30 (1.09) - removed IO.FontHeight (now computed automatically)
 - 2014/08/30 (1.09) - moved IMGUI_FONT_TEX_UV_FOR_WHITE preprocessor define to IO.FontTexUvForWhite
 - 2014/08/28 (1.09) - changed the behavior of IO.PixelCenterOffset following various rendering fixes


 ISSUES & TODO-LIST
 ==================
 See TODO.txt


 FREQUENTLY ASKED QUESTIONS (FAQ), TIPS
 ======================================

 Q: How can I help?
 A: - If you are experienced enough with Dear ImGui and with C/C++, look at the todo list and see how you want/can help!
    - Become a Patron/donate! Convince your company to become a Patron or provide serious funding for development time! See http://www.patreon.com/imgui

 Q: What is ImTextureID and how do I display an image?
 A: ImTextureID is a void* used to pass renderer-agnostic texture references around until it hits your render function.
    Dear ImGui knows nothing about what those bits represent, it just passes them around. It is up to you to decide what you want the void* to carry!
    It could be an identifier to your OpenGL texture (cast GLuint to void*), a pointer to your custom engine material (cast MyMaterial* to void*), etc.
    At the end of the chain, your renderer takes this void* to cast it back into whatever it needs to select a current texture to render.
    Refer to examples applications, where each renderer (in a imgui_impl_xxxx.cpp file) is treating ImTextureID as a different thing.
    (c++ tip: OpenGL uses integers to identify textures. You can safely store an integer into a void*, just cast it to void*, don't take it's address!)
    To display a custom image/texture within an ImGui window, you may use ImGui::Image(), ImGui::ImageButton(), ImDrawList::AddImage() functions.
    Dear ImGui will generate the geometry and draw calls using the ImTextureID that you passed and which your renderer can use.
    It is your responsibility to get textures uploaded to your GPU.

 Q: I integrated Dear ImGui in my engine and the text or lines are blurry..
 A: In your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f).
    Also make sure your orthographic projection matrix and io.DisplaySize matches your actual framebuffer dimension.

 Q: I integrated Dear ImGui in my engine and some elements are clipping or disappearing when I move windows around..
 A: You are probably mishandling the clipping rectangles in your render function. 
    Rectangles provided by ImGui are defined as (x1=left,y1=top,x2=right,y2=bottom) and NOT as (x1,y1,width,height).

 Q: Can I have multiple widgets with the same label? Can I have widget without a label?
 A: Yes. A primer on the use of labels/IDs in Dear ImGui..

   - Elements that are not clickable, such as Text() items don't need an ID.

   - Interactive widgets require state to be carried over multiple frames (most typically Dear ImGui often needs to remember what is 
     the "active" widget). to do so they need a unique ID. unique ID are typically derived from a string label, an integer index or a pointer.

       Button("OK");        // Label = "OK",     ID = hash of "OK"
       Button("Cancel");    // Label = "Cancel", ID = hash of "Cancel"

   - ID are uniquely scoped within windows, tree nodes, etc. so no conflict can happen if you have two buttons called "OK"
     in two different windows or in two different locations of a tree.

   - If you have a same ID twice in the same location, you'll have a conflict:

       Button("OK");
       Button("OK");           // ID collision! Both buttons will be treated as the same.

     Fear not! this is easy to solve and there are many ways to solve it!

   - When passing a label you can optionally specify extra unique ID information within string itself. 
     This helps solving the simpler collision cases. Use "##" to pass a complement to the ID that won't be visible to the end-user:

       Button("Play");         // Label = "Play",   ID = hash of "Play"
       Button("Play##foo1");   // Label = "Play",   ID = hash of "Play##foo1" (different from above)
       Button("Play##foo2");   // Label = "Play",   ID = hash of "Play##foo2" (different from above)

   - If you want to completely hide the label, but still need an ID:

       Checkbox("##On", &b);   // Label = "",       ID = hash of "##On" (no label!)

   - Occasionally/rarely you might want change a label while preserving a constant ID. This allows you to animate labels.
     For example you may want to include varying information in a window title bar (and windows are uniquely identified by their ID.. obviously)
     Use "###" to pass a label that isn't part of ID:

       Button("Hello###ID";   // Label = "Hello",  ID = hash of "ID"
       Button("World###ID";   // Label = "World",  ID = hash of "ID" (same as above)

       sprintf(buf, "My game (%f FPS)###MyGame");
       Begin(buf);            // Variable label,   ID = hash of "MyGame"

   - Use PushID() / PopID() to create scopes and avoid ID conflicts within the same Window.
     This is the most convenient way of distinguishing ID if you are iterating and creating many UI elements.
     You can push a pointer, a string or an integer value. Remember that ID are formed from the concatenation of everything in the ID stack!

       for (int i = 0; i < 100; i++)
       {
         PushID(i);
         Button("Click");   // Label = "Click",  ID = hash of integer + "label" (unique)
         PopID();
       }

       for (int i = 0; i < 100; i++)
       {
         MyObject* obj = Objects[i];
         PushID(obj);
         Button("Click");   // Label = "Click",  ID = hash of pointer + "label" (unique)
         PopID();
       }

       for (int i = 0; i < 100; i++)
       {
         MyObject* obj = Objects[i];
         PushID(obj->Name);
         Button("Click");   // Label = "Click",  ID = hash of string + "label" (unique)
         PopID();
       }

   - More example showing that you can stack multiple prefixes into the ID stack:

       Button("Click");     // Label = "Click",  ID = hash of "Click"
       PushID("node");
       Button("Click");     // Label = "Click",  ID = hash of "node" + "Click"
         PushID(my_ptr);
           Button("Click"); // Label = "Click",  ID = hash of "node" + ptr + "Click"
         PopID();
       PopID();

   - Tree nodes implicitly creates a scope for you by calling PushID().

       Button("Click");     // Label = "Click",  ID = hash of "Click"
       if (TreeNode("node"))
       {
         Button("Click");   // Label = "Click",  ID = hash of "node" + "Click"
         TreePop();
       }

   - When working with trees, ID are used to preserve the open/close state of each tree node.
     Depending on your use cases you may want to use strings, indices or pointers as ID.
      e.g. when displaying a single object that may change over time (dynamic 1-1 relationship), using a static string as ID will preserve your
       node open/closed state when the targeted object change.
      e.g. when displaying a list of objects, using indices or pointers as ID will preserve the node open/closed state differently. 
       experiment and see what makes more sense!

 Q: How can I tell when Dear ImGui wants my mouse/keyboard inputs VS when I can pass them to my application?
 A: You can read the 'io.WantCaptureMouse'/'io.WantCaptureKeyboard'/'ioWantTextInput' flags from the ImGuiIO structure. 
    - When 'io.WantCaptureMouse' or 'io.WantCaptureKeyboard' flags are set you may want to discard/hide the inputs from the rest of your application.
    - When 'io.WantTextInput' is set to may want to notify your OS to popup an on-screen keyboard, if available (e.g. on a mobile phone, or console OS).
    Preferably read the flags after calling ImGui::NewFrame() to avoid them lagging by one frame. But reading those flags before calling NewFrame() is
    also generally ok, as the bool toggles fairly rarely and you don't generally expect to interact with either Dear ImGui or your application during
    the same frame when that transition occurs. Dear ImGui is tracking dragging and widget activity that may occur outside the boundary of a window, 
    so 'io.WantCaptureMouse' is more accurate and correct than checking if a window is hovered. 
    (Advanced note: text input releases focus on Return 'KeyDown', so the following Return 'KeyUp' event that your application receive will typically 
     have 'io.WantCaptureKeyboard=false'. Depending on your application logic it may or not be inconvenient. You might want to track which key-downs
     were for Dear ImGui, e.g. with an array of bool, and filter out the corresponding key-ups.)

 Q: How can I load a different font than the default? (default is an embedded version of ProggyClean.ttf, rendered at size 13)
 A: Use the font atlas to load the TTF/OTF file you want:

      ImGuiIO& io = ImGui::GetIO();
      io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels);
      io.Fonts->GetTexDataAsRGBA32() or GetTexDataAsAlpha8()

 Q: How can I easily use icons in my application?
 A: The most convenient and practical way is to merge an icon font such as FontAwesome inside you main font. Then you can refer to icons within your 
    strings. Read 'How can I load multiple fonts?' and the file 'extra_fonts/README.txt' for instructions and useful header files.

 Q: How can I load multiple fonts?
 A: Use the font atlas to pack them into a single texture:
    (Read extra_fonts/README.txt and the code in ImFontAtlas for more details.)

      ImGuiIO& io = ImGui::GetIO();
      ImFont* font0 = io.Fonts->AddFontDefault();
      ImFont* font1 = io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels);
      ImFont* font2 = io.Fonts->AddFontFromFileTTF("myfontfile2.ttf", size_in_pixels);
      io.Fonts->GetTexDataAsRGBA32() or GetTexDataAsAlpha8()
      // the first loaded font gets used by default
      // use ImGui::PushFont()/ImGui::PopFont() to change the font at runtime

      // Options
      ImFontConfig config;
      config.OversampleH = 3;
      config.OversampleV = 1;
      config.GlyphOffset.y -= 2.0f;      // Move everything by 2 pixels up
      config.GlyphExtraSpacing.x = 1.0f; // Increase spacing between characters
      io.Fonts->LoadFromFileTTF("myfontfile.ttf", size_pixels, &config);

      // Combine multiple fonts into one (e.g. for icon fonts)
      ImWchar ranges[] = { 0xf000, 0xf3ff, 0 };
      ImFontConfig config;
      config.MergeMode = true;
      io.Fonts->AddFontDefault();
      io.Fonts->LoadFromFileTTF("fontawesome-webfont.ttf", 16.0f, &config, ranges); // Merge icon font
      io.Fonts->LoadFromFileTTF("myfontfile.ttf", size_pixels, NULL, &config, io.Fonts->GetGlyphRangesJapanese()); // Merge japanese glyphs

 Q: How can I display and input non-Latin characters such as Chinese, Japanese, Korean, Cyrillic?
 A: When loading a font, pass custom Unicode ranges to specify the glyphs to load. 

      // Add default Japanese ranges
      io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, NULL, io.Fonts->GetGlyphRangesJapanese());
   
      // Or create your own custom ranges (e.g. for a game you can feed your entire game script and only build the characters the game need)
      ImVector<ImWchar> ranges;
      ImFontAtlas::GlyphRangesBuilder builder;
      builder.AddText("Hello world");                        // Add a string (here "Hello world" contains 7 unique characters)
      builder.AddChar(0x7262);                               // Add a specific character
      builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // Add one of the default ranges
      builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges with all the unique characters submitted)
      io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, NULL, ranges.Data);

    All your strings needs to use UTF-8 encoding. In C++11 you can encode a string literal in UTF-8 by using the u8"hello" syntax. 
    Specifying literal in your source code using a local code page (such as CP-923 for Japanese or CP-1251 for Cyrillic) will NOT work!
    Otherwise you can convert yourself to UTF-8 or load text data from file already saved as UTF-8.

    Text input: it is up to your application to pass the right character code to io.AddInputCharacter(). The applications in examples/ are doing that.
    For languages using IME, on Windows you can copy the Hwnd of your application to io.ImeWindowHandle.
    The default implementation of io.ImeSetInputScreenPosFn() on Windows will set your IME position correctly.

 Q: How can I preserve my Dear ImGui context across reloading a DLL? (loss of the global/static variables)
 A: Create your own context 'ctx = CreateContext()' + 'SetCurrentContext(ctx)' and your own font atlas 'ctx->GetIO().Fonts = new ImFontAtlas()' 
    so you don't rely on the default globals.

 Q: How can I use the drawing facilities without an ImGui window? (using ImDrawList API)
 A: The easiest way is to create a dummy window. Call Begin() with NoTitleBar|NoResize|NoMove|NoScrollbar|NoSavedSettings|NoInputs flag, 
    zero background alpha, then retrieve the ImDrawList* via GetWindowDrawList() and draw to it in any way you like.
    You can also perfectly create a standalone ImDrawList instance _but_ you need ImGui to be initialized because ImDrawList pulls from ImGui 
    data to retrieve the coordinates of the white pixel.

 - tip: you can call Begin() multiple times with the same name during the same frame, it will keep appending to the same window. 
   this is also useful to set yourself in the context of another window (to get/set other settings)
 - tip: you can create widgets without a Begin()/End() block, they will go in an implicit window called "Debug".
 - tip: the ImGuiOnceUponAFrame helper will allow run the block of code only once a frame. You can use it to quickly add custom UI in the middle
   of a deep nested inner loop in your code.
 - tip: you can call Render() multiple times (e.g for VR renders).
 - tip: call and read the ShowTestWindow() code in imgui_demo.cpp for more example of how to use ImGui!

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui_internal.h"

#include <ctype.h>      // toupper, isprint
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi
#include <stdio.h>      // vsnprintf, sscanf, printf
#include <limits.h>     // INT_MIN, INT_MAX
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4127) // condition expression is constant
#pragma warning (disable: 4505) // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

// Clang warnings with -Weverything
#ifdef __clang__
#pragma clang diagnostic ignored "-Wunknown-pragmas"        // warning : unknown warning group '-Wformat-pedantic *'        // not all warnings are known by all clang versions.. so ignoring warnings triggers new warnings on some configuration. great!
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is unsafe   // storing and comparing against same constants (typically 0.0f) is ok.
#pragma clang diagnostic ignored "-Wformat-nonliteral"      // warning : format string is not a string literal              // passing non-literal to vsnformat(). yes, user passing incorrect format strings can crash the code.
#pragma clang diagnostic ignored "-Wexit-time-destructors"  // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wglobal-constructors"    // warning : declaration requires a global destructor           // similar to above, not sure what the exact difference it.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness             //
#pragma clang diagnostic ignored "-Wformat-pedantic"        // warning : format specifies type 'void *' but the argument has type 'xxxx *' // unreasonable, would lead to casting every %p arg to void*. probably enabled by -pedantic. 
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast" // warning : cast to 'void *' from smaller integer type 'int' //
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"          // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"      // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat"                   // warning: format '%p' expects argument of type 'void*', but argument 6 has type 'ImGuiWindow*'
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wcast-qual"                // warning: cast from type 'xxxx' to type 'xxxx' casts away qualifiers
#pragma GCC diagnostic ignored "-Wformat-nonliteral"        // warning: format not a string literal, format string not checked
#endif

//-------------------------------------------------------------------------
// Forward Declarations
//-------------------------------------------------------------------------

static float            GetDraggedColumnOffset(int column_index);

static bool             IsKeyPressedMap(ImGuiKey key, bool repeat = true);

static ImFont*          GetDefaultFont();
static void             SetCurrentFont(ImFont* font);
static void             SetCurrentWindow(ImGuiWindow* window);
static void             SetWindowScrollY(ImGuiWindow* window, float new_scroll_y);
static void             SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond);
static void             SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond);
static void             SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond);
static ImGuiWindow*     FindHoveredWindow(ImVec2 pos, bool excluding_childs);
static ImGuiWindow*     CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags);
static void             ClearSetNextWindowData();
static void             CheckStacksSize(ImGuiWindow* window, bool write);
static ImVec2           CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window);

static void             AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list);
static void             AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window);
static void             AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window);

static ImGuiIniData*    FindWindowSettings(const char* name);
static ImGuiIniData*    AddWindowSettings(const char* name);
static void             LoadIniSettingsFromDisk(const char* ini_filename);
static void             SaveIniSettingsToDisk(const char* ini_filename);
static void             MarkIniSettingsDirty(ImGuiWindow* window);

static ImRect           GetVisibleRect();

static void             CloseInactivePopups();
static void             ClosePopupToLevel(int remaining);
static ImGuiWindow*     GetFrontMostModalRootWindow();
static ImVec2           FindBestPopupWindowPos(const ImVec2& base_pos, const ImVec2& size, int* last_dir, const ImRect& rect_to_avoid);

static bool             InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data);
static int              InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end);
static ImVec2           InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining = NULL, ImVec2* out_offset = NULL, bool stop_on_new_line = false);

static inline void      DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size);
static inline void      DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size);
static void             DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2);
static bool             DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* data_ptr, const char* scalar_format);

//-----------------------------------------------------------------------------
// Platform dependent default implementations
//-----------------------------------------------------------------------------

static const char*      GetClipboardTextFn_DefaultImpl(void* user_data);
static void             SetClipboardTextFn_DefaultImpl(void* user_data, const char* text);
static void             ImeSetInputScreenPosFn_DefaultImpl(int x, int y);

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------

// Default font atlas storage.
// New contexts always point by default to this font atlas. It can be changed by reassigning the GetIO().Fonts variable.
static ImFontAtlas      GImDefaultFontAtlas;

// Default context storage + current context pointer.
// Implicitely used by all ImGui functions. Always assumed to be != NULL. Change to a different context by calling ImGui::SetCurrentContext()
// If you are hot-reloading this code in a DLL you will lose the static/global variables. Create your own context+font atlas instead of relying on those default (see FAQ entry "How can I preserve my ImGui context across reloading a DLL?").
// ImGui is currently not thread-safe because of this variable. If you want thread-safety to allow N threads to access N different contexts, you might work around it by:
// - Having multiple instances of the ImGui code compiled inside different namespace (easiest/safest, if you have a finite number of contexts)
// - or: Changing this variable to be TLS. You may #define GImGui in imconfig.h for further custom hackery. Future development aim to make this context pointer explicit to all calls. Also read https://github.com/ocornut/imgui/issues/586
#ifndef GImGui
static ImGuiContext     GImDefaultContext;
ImGuiContext*           GImGui = &GImDefaultContext;
#endif

//-----------------------------------------------------------------------------
// User facing structures
//-----------------------------------------------------------------------------

ImGuiStyle::ImGuiStyle()
{
    Alpha                   = 1.0f;             // Global alpha applies to everything in ImGui
    WindowPadding           = ImVec2(8,8);      // Padding within a window
    WindowMinSize           = ImVec2(32,32);    // Minimum window size
    WindowRounding          = 9.0f;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    WindowTitleAlign        = ImVec2(0.0f,0.5f);// Alignment for title bar text
    ChildWindowRounding     = 0.0f;             // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
    FramePadding            = ImVec2(4,3);      // Padding within a framed rectangle (used by most widgets)
    FrameRounding           = 0.0f;             // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
    ItemSpacing             = ImVec2(8,4);      // Horizontal and vertical spacing between widgets/lines
    ItemInnerSpacing        = ImVec2(4,4);      // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    TouchExtraPadding       = ImVec2(0,0);      // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    IndentSpacing           = 21.0f;            // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    ColumnsMinSpacing       = 6.0f;             // Minimum horizontal spacing between two columns
    ScrollbarSize           = 16.0f;            // Width of the vertical scrollbar, Height of the horizontal scrollbar
    ScrollbarRounding       = 9.0f;             // Radius of grab corners rounding for scrollbar
    GrabMinSize             = 10.0f;            // Minimum width/height of a grab box for slider/scrollbar
    GrabRounding            = 0.0f;             // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    ButtonTextAlign         = ImVec2(0.5f,0.5f);// Alignment of button text when button is larger than text.
    DisplayWindowPadding    = ImVec2(22,22);    // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
    DisplaySafeAreaPadding  = ImVec2(4,4);      // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    AntiAliasedLines        = true;             // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
    AntiAliasedShapes       = true;             // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    CurveTessellationTol    = 1.25f;            // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality
	WindowPadThickness		= 0.f;				// 

    ImGui::StyleColorsClassic(this);
}

void ImGui::StyleColorsClassic(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_ChildWindowBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    colors[ImGuiCol_Border]                 = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);   // Background of checkbox, radio button, plot, slider, text input
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
    colors[ImGuiCol_ComboBg]                = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.65f, 0.65f, 0.65f, 0.30f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
    colors[ImGuiCol_Separator]              = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    colors[ImGuiCol_CloseButton]            = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
    colors[ImGuiCol_CloseButtonHovered]     = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
    colors[ImGuiCol_CloseButtonActive]      = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    colors[ImGuiCol_ModalWindowDarkening]   = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

// To scale your entire UI (e.g. if you want your app to use High DPI or generally be DPI aware) you may use this helper function. Scaling the fonts is done separately and is up to you.
// Tips: if you need to change your scale multiple times, prefer calling this on a freshly initialized ImGuiStyle structure rather than scaling multiple times (because floating point multiplications are lossy).
void ImGuiStyle::ScaleAllSizes(float scale_factor)
{
    WindowPadding *= scale_factor;
    WindowMinSize *= scale_factor;
    WindowRounding *= scale_factor;
    ChildWindowRounding *= scale_factor;
    FramePadding *= scale_factor;
    FrameRounding *= scale_factor;
    ItemSpacing *= scale_factor;
    ItemInnerSpacing *= scale_factor;
    TouchExtraPadding *= scale_factor;
    IndentSpacing *= scale_factor;
    ColumnsMinSpacing *= scale_factor;
    ScrollbarSize *= scale_factor;
    ScrollbarRounding *= scale_factor;
    GrabMinSize *= scale_factor;
    GrabRounding *= scale_factor;
    DisplayWindowPadding *= scale_factor;
    DisplaySafeAreaPadding *= scale_factor;
}

ImGuiIO::ImGuiIO()
{
    // Most fields are initialized with zero
    memset(this, 0, sizeof(*this));

    // Settings
    DisplaySize = ImVec2(-1.0f, -1.0f);
    DeltaTime = 1.0f/60.0f;
    IniSavingRate = 5.0f;
    IniFilename = "imgui.ini";
    LogFilename = "imgui_log.txt";
    MouseDoubleClickTime = 0.30f;
    MouseDoubleClickMaxDist = 6.0f;
    for (int i = 0; i < ImGuiKey_COUNT; i++)
        KeyMap[i] = -1;
    KeyRepeatDelay = 0.250f;
    KeyRepeatRate = 0.050f;
    UserData = NULL;

    Fonts = &GImDefaultFontAtlas;
    FontGlobalScale = 1.0f;
    FontDefault = NULL;
    FontAllowUserScaling = false;
    DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    DisplayVisibleMin = DisplayVisibleMax = ImVec2(0.0f, 0.0f);

    // User functions
    RenderDrawListsFn = NULL;
    MemAllocFn = malloc;
    MemFreeFn = free;
    GetClipboardTextFn = GetClipboardTextFn_DefaultImpl;   // Platform dependent default implementations
    SetClipboardTextFn = SetClipboardTextFn_DefaultImpl;
    ClipboardUserData = NULL;
    ImeSetInputScreenPosFn = ImeSetInputScreenPosFn_DefaultImpl;
    ImeWindowHandle = NULL;

    // Input (NB: we already have memset zero the entire structure)
    MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    MousePosPrev = ImVec2(-FLT_MAX, -FLT_MAX);
    MouseDragThreshold = 6.0f;
    for (int i = 0; i < IM_ARRAYSIZE(MouseDownDuration); i++) MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.0f;
    for (int i = 0; i < IM_ARRAYSIZE(KeysDownDuration); i++) KeysDownDuration[i] = KeysDownDurationPrev[i] = -1.0f;

    // Set OS X style defaults based on __APPLE__ compile time flag
#ifdef __APPLE__
    OSXBehaviors = true;
#endif
}

// Pass in translated ASCII characters for text input.
// - with glfw you can get those from the callback set in glfwSetCharCallback()
// - on Windows you can get those using ToAscii+keyboard state, or via the WM_CHAR message
void ImGuiIO::AddInputCharacter(ImWchar c)
{
    const int n = ImStrlenW(InputCharacters);
    if (n + 1 < IM_ARRAYSIZE(InputCharacters))
    {
        InputCharacters[n] = c;
        InputCharacters[n+1] = '\0';
    }
}

void ImGuiIO::AddInputCharactersUTF8(const char* utf8_chars)
{
    // We can't pass more wchars than ImGuiIO::InputCharacters[] can hold so don't convert more
    const int wchars_buf_len = sizeof(ImGuiIO::InputCharacters) / sizeof(ImWchar);
    ImWchar wchars[wchars_buf_len];
    ImTextStrFromUtf8(wchars, wchars_buf_len, utf8_chars, NULL);
    for (int i = 0; i < wchars_buf_len && wchars[i] != 0; i++)
        AddInputCharacter(wchars[i]);
}

//-----------------------------------------------------------------------------
// HELPERS
//-----------------------------------------------------------------------------

#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   // Unsaturated, for display purpose 
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))               // Saturated, always output 0..255

// Play it nice with Windows users. Notepad in 2015 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

ImVec2 ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p)
{
    ImVec2 ap = p - a;
    ImVec2 ab_dir = b - a;
    float ab_len = sqrtf(ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y);
    ab_dir *= 1.0f / ab_len;
    float dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
    if (dot < 0.0f)
        return a;
    if (dot > ab_len)
        return b;
    return a + ab_dir * dot;
}

bool ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p)
{
    bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.0f;
    bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.0f;
    bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.0f;
    return ((b1 == b2) && (b2 == b3));
}

void ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w)
{
    ImVec2 v0 = b - a;
    ImVec2 v1 = c - a;
    ImVec2 v2 = p - a;
    const float denom = v0.x * v1.y - v1.x * v0.y;
    out_v = (v2.x * v1.y - v1.x * v2.y) / denom;
    out_w = (v0.x * v2.y - v2.x * v0.y) / denom;
    out_u = 1.0f - out_v - out_w;
}

ImVec2 ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p)
{
    ImVec2 proj_ab = ImLineClosestPoint(a, b, p);
    ImVec2 proj_bc = ImLineClosestPoint(b, c, p);
    ImVec2 proj_ca = ImLineClosestPoint(c, a, p);
    float dist2_ab = ImLengthSqr(p - proj_ab);
    float dist2_bc = ImLengthSqr(p - proj_bc);
    float dist2_ca = ImLengthSqr(p - proj_ca);
    float m = ImMin(dist2_ab, ImMin(dist2_bc, dist2_ca));
    if (m == dist2_ab)
        return proj_ab;
    if (m == dist2_bc)
        return proj_bc;
    return proj_ca;
}

int ImStricmp(const char* str1, const char* str2)
{
    int d;
    while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; }
    return d;
}

int ImStrnicmp(const char* str1, const char* str2, int count)
{
    int d = 0;
    while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; count--; }
    return d;
}

void ImStrncpy(char* dst, const char* src, int count)
{
    if (count < 1) return;
    strncpy(dst, src, (size_t)count);
    dst[count-1] = 0;
}

char* ImStrdup(const char *str)
{
    size_t len = strlen(str) + 1;
    void* buff = ImGui::MemAlloc(len);
    return (char*)memcpy(buff, (const void*)str, len);
}

int ImStrlenW(const ImWchar* str)
{
    int n = 0;
    while (*str++) n++;
    return n;
}

const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin) // find beginning-of-line
{
    while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n')
        buf_mid_line--;
    return buf_mid_line;
}

const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end)
{
    if (!needle_end)
        needle_end = needle + strlen(needle);

    const char un0 = (char)toupper(*needle);
    while ((!haystack_end && *haystack) || (haystack_end && haystack < haystack_end))
    {
        if (toupper(*haystack) == un0)
        {
            const char* b = needle + 1;
            for (const char* a = haystack + 1; b < needle_end; a++, b++)
                if (toupper(*a) != toupper(*b))
                    break;
            if (b == needle_end)
                return haystack;
        }
        haystack++;
    }
    return NULL;
}

static const char* ImAtoi(const char* src, int* output)
{
    int negative = 0;
    if (*src == '-') { negative = 1; src++; }
    if (*src == '+') { src++; }
    int v = 0;
    while (*src >= '0' && *src <= '9')
        v = (v * 10) + (*src++ - '0');
    *output = negative ? -v : v;
    return src;
}

// MSVC version appears to return -1 on overflow, whereas glibc appears to return total count (which may be >= buf_size). 
// Ideally we would test for only one of those limits at runtime depending on the behavior the vsnprintf(), but trying to deduct it at compile time sounds like a pandora can of worm.
int ImFormatString(char* buf, int buf_size, const char* fmt, ...)
{
    IM_ASSERT(buf_size > 0);
    va_list args;
    va_start(args, fmt);
    int w = vsnprintf(buf, buf_size, fmt, args);
    va_end(args);
    if (w == -1 || w >= buf_size)
        w = buf_size - 1;
    buf[w] = 0;
    return w;
}

int ImFormatStringV(char* buf, int buf_size, const char* fmt, va_list args)
{
    IM_ASSERT(buf_size > 0);
    int w = vsnprintf(buf, buf_size, fmt, args);
    if (w == -1 || w >= buf_size)
        w = buf_size - 1;
    buf[w] = 0;
    return w;
}

// Pass data_size==0 for zero-terminated strings
// FIXME-OPT: Replace with e.g. FNV1a hash? CRC32 pretty much randomly access 1KB. Need to do proper measurements.
ImU32 ImHash(const void* data, int data_size, ImU32 seed)
{
    static ImU32 crc32_lut[256] = { 0 };
    if (!crc32_lut[1])
    {
        const ImU32 polynomial = 0xEDB88320;
        for (ImU32 i = 0; i < 256; i++)
        {
            ImU32 crc = i;
            for (ImU32 j = 0; j < 8; j++)
                crc = (crc >> 1) ^ (ImU32(-int(crc & 1)) & polynomial);
            crc32_lut[i] = crc;
        }
    }

    seed = ~seed;
    ImU32 crc = seed;
    const unsigned char* current = (const unsigned char*)data;

    if (data_size > 0)
    {
        // Known size
        while (data_size--)
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *current++];
    }
    else
    {
        // Zero-terminated string
        while (unsigned char c = *current++)
        {
            // We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
            // Because this syntax is rarely used we are optimizing for the common case.
            // - If we reach ### in the string we discard the hash so far and reset to the seed.
            // - We don't do 'current += 2; continue;' after handling ### to keep the code smaller.
            if (c == '#' && current[0] == '#' && current[1] == '#')
                crc = seed;
            crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
        }
    }
    return ~crc;
}

//-----------------------------------------------------------------------------
// ImText* helpers
//-----------------------------------------------------------------------------

// Convert UTF-8 to 32-bits character, process single character input.
// Based on stb_from_utf8() from github.com/nothings/stb/
// We handle UTF-8 decoding error by skipping forward.
int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
    unsigned int c = (unsigned int)-1;
    const unsigned char* str = (const unsigned char*)in_text;
    if (!(*str & 0x80))
    {
        c = (unsigned int)(*str++);
        *out_char = c;
        return 1;
    }
    if ((*str & 0xe0) == 0xc0)
    {
        *out_char = 0xFFFD; // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 2) return 1;
        if (*str < 0xc2) return 2;
        c = (unsigned int)((*str++ & 0x1f) << 6);
        if ((*str & 0xc0) != 0x80) return 2;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 2;
    }
    if ((*str & 0xf0) == 0xe0)
    {
        *out_char = 0xFFFD; // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 3) return 1;
        if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
        if (*str == 0xed && str[1] > 0x9f) return 3; // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x0f) << 12);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 3;
    }
    if ((*str & 0xf8) == 0xf0)
    {
        *out_char = 0xFFFD; // will be invalid but not end of string
        if (in_text_end && in_text_end - (const char*)str < 4) return 1;
        if (*str > 0xf4) return 4;
        if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
        if (*str == 0xf4 && str[1] > 0x8f) return 4; // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x07) << 18);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 12);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (*str++ & 0x3f);
        // utf-8 encodings of values used in surrogate pairs are invalid
        if ((c & 0xFFFFF800) == 0xD800) return 4;
        *out_char = c;
        return 4;
    }
    *out_char = 0;
    return 0;
}

int ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
    ImWchar* buf_out = buf;
    ImWchar* buf_end = buf + buf_size;
    while (buf_out < buf_end-1 && (!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c;
        in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
        if (c == 0)
            break;
        if (c < 0x10000)    // FIXME: Losing characters that don't fit in 2 bytes
            *buf_out++ = (ImWchar)c;
    }
    *buf_out = 0;
    if (in_text_remaining)
        *in_text_remaining = in_text;
    return (int)(buf_out - buf);
}

int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
    int char_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c;
        in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
        if (c == 0)
            break;
        if (c < 0x10000)
            char_count++;
    }
    return char_count;
}

// Based on stb_to_utf8() from github.com/nothings/stb/
static inline int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
    if (c < 0x80)
    {
        buf[0] = (char)c;
        return 1;
    }
    if (c < 0x800)
    {
        if (buf_size < 2) return 0;
        buf[0] = (char)(0xc0 + (c >> 6));
        buf[1] = (char)(0x80 + (c & 0x3f));
        return 2;
    }
    if (c >= 0xdc00 && c < 0xe000)
    {
        return 0;
    }
    if (c >= 0xd800 && c < 0xdc00)
    {
        if (buf_size < 4) return 0;
        buf[0] = (char)(0xf0 + (c >> 18));
        buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
        buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
        buf[3] = (char)(0x80 + ((c ) & 0x3f));
        return 4;
    }
    //else if (c < 0x10000)
    {
        if (buf_size < 3) return 0;
        buf[0] = (char)(0xe0 + (c >> 12));
        buf[1] = (char)(0x80 + ((c>> 6) & 0x3f));
        buf[2] = (char)(0x80 + ((c ) & 0x3f));
        return 3;
    }
}

static inline int ImTextCountUtf8BytesFromChar(unsigned int c)
{
    if (c < 0x80) return 1;
    if (c < 0x800) return 2;
    if (c >= 0xdc00 && c < 0xe000) return 0;
    if (c >= 0xd800 && c < 0xdc00) return 4;
    return 3;
}

int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end)
{
    char* buf_out = buf;
    const char* buf_end = buf + buf_size;
    while (buf_out < buf_end-1 && (!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            *buf_out++ = (char)c;
        else
            buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end-buf_out-1), c);
    }
    *buf_out = 0;
    return (int)(buf_out - buf);
}

int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end)
{
    int bytes_count = 0;
    while ((!in_text_end || in_text < in_text_end) && *in_text)
    {
        unsigned int c = (unsigned int)(*in_text++);
        if (c < 0x80)
            bytes_count++;
        else
            bytes_count += ImTextCountUtf8BytesFromChar(c);
    }
    return bytes_count;
}

ImVec4 ImGui::ColorConvertU32ToFloat4(ImU32 in)
{
    float s = 1.0f/255.0f;
    return ImVec4(
        ((in >> IM_COL32_R_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
        ((in >> IM_COL32_A_SHIFT) & 0xFF) * s);
}

ImU32 ImGui::ColorConvertFloat4ToU32(const ImVec4& in)
{
    ImU32 out;
    out  = ((ImU32)IM_F32_TO_INT8_SAT(in.x)) << IM_COL32_R_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << IM_COL32_G_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << IM_COL32_B_SHIFT;
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << IM_COL32_A_SHIFT;
    return out;
}

ImU32 ImGui::GetColorU32(ImGuiCol idx, float alpha_mul)  
{ 
    ImGuiStyle& style = GImGui->Style;
    ImVec4 c = style.Colors[idx]; 
    c.w *= style.Alpha * alpha_mul; 
    return ColorConvertFloat4ToU32(c); 
}

ImU32 ImGui::GetColorU32(const ImVec4& col)
{ 
    ImGuiStyle& style = GImGui->Style;
    ImVec4 c = col; 
    c.w *= style.Alpha; 
    return ColorConvertFloat4ToU32(c); 
}

const ImVec4& ImGui::GetStyleColorVec4(ImGuiCol idx)
{ 
    ImGuiStyle& style = GImGui->Style;
    return style.Colors[idx];
}

ImU32 ImGui::GetColorU32(ImU32 col)
{ 
    float style_alpha = GImGui->Style.Alpha;
    if (style_alpha >= 1.0f)
        return col;
    int a = (col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT;
    a = (int)(a * style_alpha); // We don't need to clamp 0..255 because Style.Alpha is in 0..1 range.
    return (col & ~IM_COL32_A_MASK) | (a << IM_COL32_A_SHIFT);
}

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if (g < b)
    {
        ImSwap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        ImSwap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void ImGui::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f)
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = fmodf(h, 1.0f) / (60.0f/360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

FILE* ImFileOpen(const char* filename, const char* mode)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
    // We need a fopen() wrapper because MSVC/Windows fopen doesn't handle UTF-8 filenames. Converting both strings from UTF-8 to wchar format (using a single allocation, because we can)
    const int filename_wsize = ImTextCountCharsFromUtf8(filename, NULL) + 1;
    const int mode_wsize = ImTextCountCharsFromUtf8(mode, NULL) + 1;
    ImVector<ImWchar> buf;
    buf.resize(filename_wsize + mode_wsize);
    ImTextStrFromUtf8(&buf[0], filename_wsize, filename, NULL);
    ImTextStrFromUtf8(&buf[filename_wsize], mode_wsize, mode, NULL);
    return _wfopen((wchar_t*)&buf[0], (wchar_t*)&buf[filename_wsize]);
#else
    return fopen(filename, mode);
#endif
}

// Load file content into memory
// Memory allocated with ImGui::MemAlloc(), must be freed by user using ImGui::MemFree()
void* ImFileLoadToMemory(const char* filename, const char* file_open_mode, int* out_file_size, int padding_bytes)
{
    IM_ASSERT(filename && file_open_mode);
    if (out_file_size)
        *out_file_size = 0;

    FILE* f;
    if ((f = ImFileOpen(filename, file_open_mode)) == NULL)
        return NULL;

    long file_size_signed;
    if (fseek(f, 0, SEEK_END) || (file_size_signed = ftell(f)) == -1 || fseek(f, 0, SEEK_SET))
    {
        fclose(f);
        return NULL;
    }

    int file_size = (int)file_size_signed;
    void* file_data = ImGui::MemAlloc(file_size + padding_bytes);
    if (file_data == NULL)
    {
        fclose(f);
        return NULL;
    }
    if (fread(file_data, 1, (size_t)file_size, f) != (size_t)file_size)
    {
        fclose(f);
        ImGui::MemFree(file_data);
        return NULL;
    }
    if (padding_bytes > 0)
        memset((void *)(((char*)file_data) + file_size), 0, padding_bytes);

    fclose(f);
    if (out_file_size)
        *out_file_size = file_size;

    return file_data;
}

//-----------------------------------------------------------------------------
// ImGuiStorage
//-----------------------------------------------------------------------------

// Helper: Key->value storage
void ImGuiStorage::Clear()
{
    Data.clear();
}

// std::lower_bound but without the bullshit
static ImVector<ImGuiStorage::Pair>::iterator LowerBound(ImVector<ImGuiStorage::Pair>& data, ImGuiID key)
{
    ImVector<ImGuiStorage::Pair>::iterator first = data.begin();
    ImVector<ImGuiStorage::Pair>::iterator last = data.end();
    int count = (int)(last - first);
    while (count > 0)
    {
        int count2 = count / 2;
        ImVector<ImGuiStorage::Pair>::iterator mid = first + count2;
        if (mid->key < key)
        {
            first = ++mid;
            count -= count2 + 1;
        }
        else
        {
            count = count2;
        }
    }
    return first;
}

int ImGuiStorage::GetInt(ImGuiID key, int default_val) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key)
        return default_val;
    return it->val_i;
}

bool ImGuiStorage::GetBool(ImGuiID key, bool default_val) const
{
    return GetInt(key, default_val ? 1 : 0) != 0;
}

float ImGuiStorage::GetFloat(ImGuiID key, float default_val) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key)
        return default_val;
    return it->val_f;
}

void* ImGuiStorage::GetVoidPtr(ImGuiID key) const
{
    ImVector<Pair>::iterator it = LowerBound(const_cast<ImVector<ImGuiStorage::Pair>&>(Data), key);
    if (it == Data.end() || it->key != key)
        return NULL;
    return it->val_p;
}

// References are only valid until a new value is added to the storage. Calling a Set***() function or a Get***Ref() function invalidates the pointer.
int* ImGuiStorage::GetIntRef(ImGuiID key, int default_val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
        it = Data.insert(it, Pair(key, default_val));
    return &it->val_i;
}

bool* ImGuiStorage::GetBoolRef(ImGuiID key, bool default_val)
{
    return (bool*)GetIntRef(key, default_val ? 1 : 0);
}

float* ImGuiStorage::GetFloatRef(ImGuiID key, float default_val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
        it = Data.insert(it, Pair(key, default_val));
    return &it->val_f;
}

void** ImGuiStorage::GetVoidPtrRef(ImGuiID key, void* default_val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
        it = Data.insert(it, Pair(key, default_val));
    return &it->val_p;
}

// FIXME-OPT: Need a way to reuse the result of lower_bound when doing GetInt()/SetInt() - not too bad because it only happens on explicit interaction (maximum one a frame)
void ImGuiStorage::SetInt(ImGuiID key, int val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
    {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_i = val;
}

void ImGuiStorage::SetBool(ImGuiID key, bool val)
{
    SetInt(key, val ? 1 : 0);
}

void ImGuiStorage::SetFloat(ImGuiID key, float val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
    {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_f = val;
}

void ImGuiStorage::SetVoidPtr(ImGuiID key, void* val)
{
    ImVector<Pair>::iterator it = LowerBound(Data, key);
    if (it == Data.end() || it->key != key)
    {
        Data.insert(it, Pair(key, val));
        return;
    }
    it->val_p = val;
}

void ImGuiStorage::SetAllInt(int v)
{
    for (int i = 0; i < Data.Size; i++)
        Data[i].val_i = v;
}

//-----------------------------------------------------------------------------
// ImGuiTextFilter
//-----------------------------------------------------------------------------

// Helper: Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
ImGuiTextFilter::ImGuiTextFilter(const char* default_filter)
{
    if (default_filter)
    {
        ImStrncpy(InputBuf, default_filter, IM_ARRAYSIZE(InputBuf));
        Build();
    }
    else
    {
        InputBuf[0] = 0;
        CountGrep = 0;
    }
}

bool ImGuiTextFilter::Draw(const char* label, float width)
{
    if (width != 0.0f)
        ImGui::PushItemWidth(width);
    bool value_changed = ImGui::InputText(label, InputBuf, IM_ARRAYSIZE(InputBuf));
    if (width != 0.0f)
        ImGui::PopItemWidth();
    if (value_changed)
        Build();
    return value_changed;
}

void ImGuiTextFilter::TextRange::split(char separator, ImVector<TextRange>& out)
{
    out.resize(0);
    const char* wb = b;
    const char* we = wb;
    while (we < e)
    {
        if (*we == separator)
        {
            out.push_back(TextRange(wb, we));
            wb = we + 1;
        }
        we++;
    }
    if (wb != we)
        out.push_back(TextRange(wb, we));
}

void ImGuiTextFilter::Build()
{
    Filters.resize(0);
    TextRange input_range(InputBuf, InputBuf+strlen(InputBuf));
    input_range.split(',', Filters);

    CountGrep = 0;
    for (int i = 0; i != Filters.Size; i++)
    {
        Filters[i].trim_blanks();
        if (Filters[i].empty())
            continue;
        if (Filters[i].front() != '-')
            CountGrep += 1;
    }
}

bool ImGuiTextFilter::PassFilter(const char* text, const char* text_end) const
{
    if (Filters.empty())
        return true;

    if (text == NULL)
        text = "";

    for (int i = 0; i != Filters.Size; i++)
    {
        const TextRange& f = Filters[i];
        if (f.empty())
            continue;
        if (f.front() == '-')
        {
            // Subtract
            if (ImStristr(text, text_end, f.begin()+1, f.end()) != NULL)
                return false;
        }
        else
        {
            // Grep
            if (ImStristr(text, text_end, f.begin(), f.end()) != NULL)
                return true;
        }
    }

    // Implicit * grep
    if (CountGrep == 0)
        return true;

    return false;
}

//-----------------------------------------------------------------------------
// ImGuiTextBuffer
//-----------------------------------------------------------------------------

// On some platform vsnprintf() takes va_list by reference and modifies it.
// va_copy is the 'correct' way to copy a va_list but Visual Studio prior to 2013 doesn't have it.
#ifndef va_copy
#define va_copy(dest, src) (dest = src)
#endif

// Helper: Text buffer for logging/accumulating text
void ImGuiTextBuffer::appendv(const char* fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    int len = vsnprintf(NULL, 0, fmt, args);         // FIXME-OPT: could do a first pass write attempt, likely successful on first pass.
    if (len <= 0)
        return;

    const int write_off = Buf.Size;
    const int needed_sz = write_off + len;
    if (write_off + len >= Buf.Capacity)
    {
        int double_capacity = Buf.Capacity * 2;
        Buf.reserve(needed_sz > double_capacity ? needed_sz : double_capacity);
    }

    Buf.resize(needed_sz);
    ImFormatStringV(&Buf[write_off] - 1, len+1, fmt, args_copy);
}

void ImGuiTextBuffer::append(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendv(fmt, args);
    va_end(args);
}

//-----------------------------------------------------------------------------
// ImGuiSimpleColumns
//-----------------------------------------------------------------------------

ImGuiSimpleColumns::ImGuiSimpleColumns()
{
    Count = 0;
    Spacing = Width = NextWidth = 0.0f;
    memset(Pos, 0, sizeof(Pos));
    memset(NextWidths, 0, sizeof(NextWidths));
}

void ImGuiSimpleColumns::Update(int count, float spacing, bool clear)
{
    IM_ASSERT(Count <= IM_ARRAYSIZE(Pos));
    Count = count;
    Width = NextWidth = 0.0f;
    Spacing = spacing;
    if (clear) memset(NextWidths, 0, sizeof(NextWidths));
    for (int i = 0; i < Count; i++)
    {
        if (i > 0 && NextWidths[i] > 0.0f)
            Width += Spacing;
        Pos[i] = (float)(int)Width;
        Width += NextWidths[i];
        NextWidths[i] = 0.0f;
    }
}

float ImGuiSimpleColumns::DeclColumns(float w0, float w1, float w2) // not using va_arg because they promote float to double
{
    NextWidth = 0.0f;
    NextWidths[0] = ImMax(NextWidths[0], w0);
    NextWidths[1] = ImMax(NextWidths[1], w1);
    NextWidths[2] = ImMax(NextWidths[2], w2);
    for (int i = 0; i < 3; i++)
        NextWidth += NextWidths[i] + ((i > 0 && NextWidths[i] > 0.0f) ? Spacing : 0.0f);
    return ImMax(Width, NextWidth);
}

float ImGuiSimpleColumns::CalcExtraSpace(float avail_w)
{
    return ImMax(0.0f, avail_w - Width);
}

//-----------------------------------------------------------------------------
// ImGuiListClipper
//-----------------------------------------------------------------------------

static void SetCursorPosYAndSetupDummyPrevLine(float pos_y, float line_height)
{
    // Set cursor position and a few other things so that SetScrollHere() and Columns() can work when seeking cursor. 
    // FIXME: It is problematic that we have to do that here, because custom/equivalent end-user code would stumble on the same issue. Consider moving within SetCursorXXX functions?
    ImGui::SetCursorPosY(pos_y);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y - line_height;      // Setting those fields so that SetScrollHere() can properly function after the end of our clipper usage.
    window->DC.PrevLineHeight = (line_height - GImGui->Style.ItemSpacing.y);    // If we end up needing more accurate data (to e.g. use SameLine) we may as well make the clipper have a fourth step to let user process and display the last item in their list.
    if (window->DC.ColumnsCount > 1)
        window->DC.ColumnsCellMinY = window->DC.CursorPos.y;                    // Setting this so that cell Y position are set properly
}

// Use case A: Begin() called from constructor with items_height<0, then called again from Sync() in StepNo 1
// Use case B: Begin() called from constructor with items_height>0
// FIXME-LEGACY: Ideally we should remove the Begin/End functions but they are part of the legacy API we still support. This is why some of the code in Step() calling Begin() and reassign some fields, spaghetti style.
void ImGuiListClipper::Begin(int count, float items_height)
{
    StartPosY = ImGui::GetCursorPosY();
    ItemsHeight = items_height;
    ItemsCount = count;
    StepNo = 0;
    DisplayEnd = DisplayStart = -1;
    if (ItemsHeight > 0.0f)
    {
        ImGui::CalcListClipping(ItemsCount, ItemsHeight, &DisplayStart, &DisplayEnd); // calculate how many to clip/display
        if (DisplayStart > 0)
            SetCursorPosYAndSetupDummyPrevLine(StartPosY + DisplayStart * ItemsHeight, ItemsHeight); // advance cursor
        StepNo = 2;
    }
}

void ImGuiListClipper::End()
{
    if (ItemsCount < 0)
        return;
    // In theory here we should assert that ImGui::GetCursorPosY() == StartPosY + DisplayEnd * ItemsHeight, but it feels saner to just seek at the end and not assert/crash the user.
    if (ItemsCount < INT_MAX)
        SetCursorPosYAndSetupDummyPrevLine(StartPosY + ItemsCount * ItemsHeight, ItemsHeight); // advance cursor
    ItemsCount = -1;
    StepNo = 3;
}

bool ImGuiListClipper::Step()
{
    if (ItemsCount == 0 || ImGui::GetCurrentWindowRead()->SkipItems)
    {
        ItemsCount = -1; 
        return false; 
    }
    if (StepNo == 0) // Step 0: the clipper let you process the first element, regardless of it being visible or not, so we can measure the element height.
    {
        DisplayStart = 0;
        DisplayEnd = 1;
        StartPosY = ImGui::GetCursorPosY();
        StepNo = 1;
        return true;
    }
    if (StepNo == 1) // Step 1: the clipper infer height from first element, calculate the actual range of elements to display, and position the cursor before the first element.
    {
        if (ItemsCount == 1) { ItemsCount = -1; return false; }
        float items_height = ImGui::GetCursorPosY() - StartPosY;
        IM_ASSERT(items_height > 0.0f);   // If this triggers, it means Item 0 hasn't moved the cursor vertically
        Begin(ItemsCount-1, items_height);
        DisplayStart++;
        DisplayEnd++;
        StepNo = 3;
        return true;
    }
    if (StepNo == 2) // Step 2: dummy step only required if an explicit items_height was passed to constructor or Begin() and user still call Step(). Does nothing and switch to Step 3.
    {
        IM_ASSERT(DisplayStart >= 0 && DisplayEnd >= 0);
        StepNo = 3;
        return true;
    }
    if (StepNo == 3) // Step 3: the clipper validate that we have reached the expected Y position (corresponding to element DisplayEnd), advance the cursor to the end of the list and then returns 'false' to end the loop.
        End();
    return false;
}

//-----------------------------------------------------------------------------
// ImGuiWindow
//-----------------------------------------------------------------------------

ImGuiWindow::ImGuiWindow(const char* name)
{
    Name = ImStrdup(name);
    ID = ImHash(name, 0);
    IDStack.push_back(ID);
    Flags = 0;
    OrderWithinParent = 0;
    PosFloat = Pos = ImVec2(0.0f, 0.0f);
    Size = SizeFull = ImVec2(0.0f, 0.0f);
    SizeContents = SizeContentsExplicit = ImVec2(0.0f, 0.0f);
    WindowPadding = ImVec2(0.0f, 0.0f);
    MoveId = GetID("#MOVE");
    Scroll = ImVec2(0.0f, 0.0f);
    ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);
    ScrollTargetCenterRatio = ImVec2(0.5f, 0.5f);
    ScrollbarX = ScrollbarY = false;
    ScrollbarSizes = ImVec2(0.0f, 0.0f);
    BorderSize = 0.0f;
    Active = WasActive = false;
    Accessed = false;
    Collapsed = false;
    SkipItems = false;
    Appearing = false;
    BeginCount = 0;
    PopupId = 0;
    AutoFitFramesX = AutoFitFramesY = -1;
    AutoFitOnlyGrows = false;
    AutoFitChildAxises = 0x00;
    AutoPosLastDirection = -1;
    HiddenFrames = 0;
    SetWindowPosAllowFlags = SetWindowSizeAllowFlags = SetWindowCollapsedAllowFlags = ImGuiCond_Always | ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing;
    SetWindowPosVal = SetWindowPosPivot = ImVec2(FLT_MAX, FLT_MAX);

    LastFrameActive = -1;
    ItemWidthDefault = 0.0f;
    FontWindowScale = 1.0f;

    DrawList = (ImDrawList*)ImGui::MemAlloc(sizeof(ImDrawList));
    IM_PLACEMENT_NEW(DrawList) ImDrawList();
    DrawList->_OwnerName = Name;
    ParentWindow = NULL;
    RootWindow = NULL;
    RootNonPopupWindow = NULL;

    FocusIdxAllCounter = FocusIdxTabCounter = -1;
    FocusIdxAllRequestCurrent = FocusIdxTabRequestCurrent = INT_MAX;
    FocusIdxAllRequestNext = FocusIdxTabRequestNext = INT_MAX;
}

ImGuiWindow::~ImGuiWindow()
{
    DrawList->~ImDrawList();
    ImGui::MemFree(DrawList);
    DrawList = NULL;
    ImGui::MemFree(Name);
    Name = NULL;
}

ImGuiID ImGuiWindow::GetID(const char* str, const char* str_end)
{
    ImGuiID seed = IDStack.back();
    ImGuiID id = ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
    ImGui::KeepAliveID(id);
    return id;
}

ImGuiID ImGuiWindow::GetID(const void* ptr)
{
    ImGuiID seed = IDStack.back();
    ImGuiID id = ImHash(&ptr, sizeof(void*), seed);
    ImGui::KeepAliveID(id);
    return id;
}

ImGuiID ImGuiWindow::GetIDNoKeepAlive(const char* str, const char* str_end)
{
    ImGuiID seed = IDStack.back();
    return ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
}

//-----------------------------------------------------------------------------
// Internal API exposed in imgui_internal.h
//-----------------------------------------------------------------------------

static void SetCurrentWindow(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow = window;
    if (window)
        g.FontSize = window->CalcFontSize();
}

ImGuiWindow* ImGui::GetParentWindow()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.CurrentWindowStack.Size >= 2);
    return g.CurrentWindowStack[(unsigned int)g.CurrentWindowStack.Size - 2];
}

void ImGui::SetActiveID(ImGuiID id, ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    g.ActiveIdIsJustActivated = (g.ActiveId != id);
    g.ActiveId = id;
    g.ActiveIdAllowOverlap = false;
    g.ActiveIdIsAlive |= (id != 0);
    g.ActiveIdWindow = window;
}

void ImGui::ClearActiveID()
{
    SetActiveID(0, NULL);
}

void ImGui::SetHoveredID(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    g.HoveredId = id;
    g.HoveredIdAllowOverlap = false;
}

void ImGui::KeepAliveID(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    if (g.ActiveId == id)
        g.ActiveIdIsAlive = true;
}

static inline bool IsWindowContentHoverable(ImGuiWindow* window)
{
    // An active popup disable hovering on other windows (apart from its own children)
    // FIXME-OPT: This could be cached/stored within the window.
    ImGuiContext& g = *GImGui;
    if (g.NavWindow)
        if (ImGuiWindow* focused_root_window = g.NavWindow->RootWindow)
            if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) != 0 && focused_root_window->WasActive && focused_root_window != window->RootWindow)
                return false;

    return true;
}

// Advance cursor given item size for layout.
void ImGui::ItemSize(const ImVec2& size, float text_offset_y)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    // Always align ourselves on pixel boundaries
    const float line_height = ImMax(window->DC.CurrentLineHeight, size.y);
    const float text_base_offset = ImMax(window->DC.CurrentLineTextBaseOffset, text_offset_y);
    //if (g.IO.KeyAlt) window->DrawList->AddRect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size.x, line_height), IM_COL32(255,0,0,200)); // [DEBUG]
    window->DC.CursorPosPrevLine = ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y);
    window->DC.CursorPos = ImVec2((float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX), (float)(int)(window->DC.CursorPos.y + line_height + g.Style.ItemSpacing.y));
    window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPosPrevLine.x);
    window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
    //if (g.IO.KeyAlt) window->DrawList->AddCircle(window->DC.CursorMaxPos, 3.0f, IM_COL32(255,0,0,255), 4); // [DEBUG]

    window->DC.PrevLineHeight = line_height;
    window->DC.PrevLineTextBaseOffset = text_base_offset;
    window->DC.CurrentLineHeight = window->DC.CurrentLineTextBaseOffset = 0.0f;

    // Horizontal layout mode
    if (window->DC.LayoutType == ImGuiLayoutType_Horizontal)
        SameLine();
}

void ImGui::ItemSize(const ImRect& bb, float text_offset_y)
{
    ItemSize(bb.GetSize(), text_offset_y);
}

// Declare item bounding box for clipping and interaction.
// Note that the size can be different than the one provided to ItemSize(). Typically, widgets that spread over available surface
// declares their minimum size requirement to ItemSize() and then use a larger region for drawing/interaction, which is passed to ItemAdd().
bool ImGui::ItemAdd(const ImRect& bb, ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const bool is_clipped = IsClippedEx(bb, id, false);
    window->DC.LastItemId = id;
    window->DC.LastItemRect = bb;
    window->DC.LastItemRectHoveredRect = false;
    if (is_clipped)
        return false;
    //if (g.IO.KeyAlt) window->DrawList->AddRect(bb.Min, bb.Max, IM_COL32(255,255,0,120)); // [DEBUG]

    // We need to calculate this now to take account of the current clipping rectangle (as items like Selectable may change them)
    window->DC.LastItemRectHoveredRect = IsMouseHoveringRect(bb.Min, bb.Max);
    return true;
}

// This is roughly matching the behavior of internal-facing ItemHoverable()
// - we allow hovering to be true when ActiveId==window->MoveID, so that clicking on non-interactive items such as a Text() item still returns true with IsItemHovered())
// - this should work even for non-interactive items that have no ID, so we cannot use LastItemId
bool ImGui::IsItemHovered()
{
    ImGuiContext& g = *GImGui;

    ImGuiWindow* window = g.CurrentWindow;
    if (!window->DC.LastItemRectHoveredRect)
        return false;
    // [2017/10/16] Reverted commit 344d48be3 and testing RootWindow instead. I believe it is correct to NOT test for RootWindow but this leaves us unable to use IsItemHovered() after EndChild() itself.
    // Until a solution is found I believe reverting to the test from 2017/09/27 is safe since this was the test that has been running for a long while.
    //if (g.HoveredWindow != window)
    //    return false;
    if (g.HoveredRootWindow != window->RootWindow)
        return false;
    if (g.ActiveId != 0 && g.ActiveId != window->DC.LastItemId && !g.ActiveIdAllowOverlap && g.ActiveId != window->MoveId)
        return false;
    if (!IsWindowContentHoverable(window))
        return false;
    return true;
}

bool ImGui::IsItemRectHovered()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRectHoveredRect;
}

// Internal facing ItemHoverable() used when submitting widgets. Differs slightly from IsItemHovered().
bool ImGui::ItemHoverable(const ImRect& bb, ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    if (g.HoveredId != 0 && g.HoveredId != id && !g.HoveredIdAllowOverlap)
        return false;

    ImGuiWindow* window = g.CurrentWindow;
    if (g.HoveredWindow != window)
        return false;
    if (g.ActiveId != 0 && g.ActiveId != id && !g.ActiveIdAllowOverlap)
        return false;
    if (!IsMouseHoveringRect(bb.Min, bb.Max))
        return false;
    if (!IsWindowContentHoverable(window))
        return false;

    SetHoveredID(id);
    return true;
}

bool ImGui::IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (!bb.Overlaps(window->ClipRect))
        if (id == 0 || id != g.ActiveId)
            if (clip_even_when_logged || !g.LogEnabled)
                return true;
    return false;
}

bool ImGui::FocusableItemRegister(ImGuiWindow* window, ImGuiID id, bool tab_stop)
{
    ImGuiContext& g = *GImGui;

    const bool allow_keyboard_focus = (window->DC.ItemFlags & ImGuiItemFlags_AllowKeyboardFocus) != 0;
    window->FocusIdxAllCounter++;
    if (allow_keyboard_focus)
        window->FocusIdxTabCounter++;

    // Process keyboard input at this point: TAB/Shift-TAB to tab out of the currently focused item.
    // Note that we can always TAB out of a widget that doesn't allow tabbing in.
    if (tab_stop && (g.ActiveId == id) && window->FocusIdxAllRequestNext == INT_MAX && window->FocusIdxTabRequestNext == INT_MAX && !g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_Tab))
        window->FocusIdxTabRequestNext = window->FocusIdxTabCounter + (g.IO.KeyShift ? (allow_keyboard_focus ? -1 : 0) : +1); // Modulo on index will be applied at the end of frame once we've got the total counter of items.

    if (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent)
        return true;

    if (allow_keyboard_focus)
        if (window->FocusIdxTabCounter == window->FocusIdxTabRequestCurrent)
            return true;

    return false;
}

void ImGui::FocusableItemUnregister(ImGuiWindow* window)
{
    window->FocusIdxAllCounter--;
    window->FocusIdxTabCounter--;
}

ImVec2 ImGui::CalcItemSize(ImVec2 size, float default_x, float default_y)
{
    ImGuiContext& g = *GImGui;
    ImVec2 content_max;
    if (size.x < 0.0f || size.y < 0.0f)
        content_max = g.CurrentWindow->Pos + GetContentRegionMax();
    if (size.x <= 0.0f)
        size.x = (size.x == 0.0f) ? default_x : ImMax(content_max.x - g.CurrentWindow->DC.CursorPos.x, 4.0f) + size.x;
    if (size.y <= 0.0f)
        size.y = (size.y == 0.0f) ? default_y : ImMax(content_max.y - g.CurrentWindow->DC.CursorPos.y, 4.0f) + size.y;
    return size;
}

float ImGui::CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x)
{
    if (wrap_pos_x < 0.0f)
        return 0.0f;

    ImGuiWindow* window = GetCurrentWindowRead();
    if (wrap_pos_x == 0.0f)
        wrap_pos_x = GetContentRegionMax().x + window->Pos.x;
    else if (wrap_pos_x > 0.0f)
        wrap_pos_x += window->Pos.x - window->Scroll.x; // wrap_pos_x is provided is window local space

    return ImMax(wrap_pos_x - pos.x, 1.0f);
}

//-----------------------------------------------------------------------------

void* ImGui::MemAlloc(size_t sz)
{
    GImGui->IO.MetricsAllocs++;
    return GImGui->IO.MemAllocFn(sz);
}

void ImGui::MemFree(void* ptr)
{
    if (ptr) GImGui->IO.MetricsAllocs--;
    return GImGui->IO.MemFreeFn(ptr);
}

const char* ImGui::GetClipboardText()
{
    return GImGui->IO.GetClipboardTextFn ? GImGui->IO.GetClipboardTextFn(GImGui->IO.ClipboardUserData) : "";
}

void ImGui::SetClipboardText(const char* text)
{
    if (GImGui->IO.SetClipboardTextFn)
        GImGui->IO.SetClipboardTextFn(GImGui->IO.ClipboardUserData, text);
}

const char* ImGui::GetVersion()
{
    return IMGUI_VERSION;
}

// Internal state access - if you want to share ImGui state between modules (e.g. DLL) or allocate it yourself
// Note that we still point to some static data and members (such as GFontAtlas), so the state instance you end up using will point to the static data within its module
ImGuiContext* ImGui::GetCurrentContext()
{
    return GImGui;
}

void ImGui::SetCurrentContext(ImGuiContext* ctx)
{
#ifdef IMGUI_SET_CURRENT_CONTEXT_FUNC
    IMGUI_SET_CURRENT_CONTEXT_FUNC(ctx); // For custom thread-based hackery you may want to have control over this.
#else
    GImGui = ctx;
#endif
}

ImGuiContext* ImGui::CreateContext(void* (*malloc_fn)(size_t), void (*free_fn)(void*))
{
    if (!malloc_fn) malloc_fn = malloc;
    ImGuiContext* ctx = (ImGuiContext*)malloc_fn(sizeof(ImGuiContext));
    IM_PLACEMENT_NEW(ctx) ImGuiContext();
    ctx->IO.MemAllocFn = malloc_fn;
    ctx->IO.MemFreeFn = free_fn ? free_fn : free;
    return ctx;
}

void ImGui::DestroyContext(ImGuiContext* ctx)
{
    void (*free_fn)(void*) = ctx->IO.MemFreeFn;
    ctx->~ImGuiContext();
    free_fn(ctx);
    if (GImGui == ctx)
        SetCurrentContext(NULL);
}

ImGuiIO& ImGui::GetIO()
{
    return GImGui->IO;
}

ImGuiStyle& ImGui::GetStyle()
{
    return GImGui->Style;
}

// Same value as passed to your RenderDrawListsFn() function. valid after Render() and until the next call to NewFrame()
ImDrawData* ImGui::GetDrawData()
{
    return GImGui->RenderDrawData.Valid ? &GImGui->RenderDrawData : NULL;
}

float ImGui::GetTime()
{
    return GImGui->Time;
}

int ImGui::GetFrameCount()
{
    return GImGui->FrameCount;
}

void ImGui::NewFrame()
{
    ImGuiContext& g = *GImGui;

    // Check user data
    IM_ASSERT(g.IO.DeltaTime >= 0.0f);               // Need a positive DeltaTime (zero is tolerated but will cause some timing issues)
    IM_ASSERT(g.IO.DisplaySize.x >= 0.0f && g.IO.DisplaySize.y >= 0.0f);
    IM_ASSERT(g.IO.Fonts->Fonts.Size > 0);           // Font Atlas not created. Did you call io.Fonts->GetTexDataAsRGBA32 / GetTexDataAsAlpha8 ?
    IM_ASSERT(g.IO.Fonts->Fonts[0]->IsLoaded());     // Font Atlas not created. Did you call io.Fonts->GetTexDataAsRGBA32 / GetTexDataAsAlpha8 ?
    IM_ASSERT(g.Style.CurveTessellationTol > 0.0f);  // Invalid style setting
    IM_ASSERT(g.Style.Alpha >= 0.0f && g.Style.Alpha <= 1.0f);  // Invalid style setting. Alpha cannot be negative (allows us to avoid a few clamps in color computations)

    // Initialize on first frame
    if (!g.Initialized)
        ImGui::Initialize();

    SetCurrentFont(GetDefaultFont());
    IM_ASSERT(g.Font->IsLoaded());

    g.Time += g.IO.DeltaTime;
    g.FrameCount += 1;
    g.TooltipOverrideCount = 0;
    g.OverlayDrawList.Clear();
    g.OverlayDrawList.PushTextureID(g.IO.Fonts->TexID);
    g.OverlayDrawList.PushClipRectFullScreen();

    // Mark rendering data as invalid to prevent user who may have a handle on it to use it
    g.RenderDrawData.Valid = false;
    g.RenderDrawData.CmdLists = NULL;
    g.RenderDrawData.CmdListsCount = g.RenderDrawData.TotalVtxCount = g.RenderDrawData.TotalIdxCount = 0;

    // Clear reference to active widget if the widget isn't alive anymore
    g.HoveredIdPreviousFrame = g.HoveredId;
    g.HoveredId = 0;
    g.HoveredIdAllowOverlap = false;
    if (!g.ActiveIdIsAlive && g.ActiveIdPreviousFrame == g.ActiveId && g.ActiveId != 0)
        ClearActiveID();
    g.ActiveIdPreviousFrame = g.ActiveId;
    g.ActiveIdIsAlive = false;
    g.ActiveIdIsJustActivated = false;
    if (g.ScalarAsInputTextId && g.ActiveId != g.ScalarAsInputTextId)
        g.ScalarAsInputTextId = 0;

    // Update keyboard input state
    memcpy(g.IO.KeysDownDurationPrev, g.IO.KeysDownDuration, sizeof(g.IO.KeysDownDuration));
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.KeysDown); i++)
        g.IO.KeysDownDuration[i] = g.IO.KeysDown[i] ? (g.IO.KeysDownDuration[i] < 0.0f ? 0.0f : g.IO.KeysDownDuration[i] + g.IO.DeltaTime) : -1.0f;

    // Update mouse input state
    // If mouse just appeared or disappeared (usually denoted by -FLT_MAX component, but in reality we test for -256000.0f) we cancel out movement in MouseDelta
    if (IsMousePosValid(&g.IO.MousePos) && IsMousePosValid(&g.IO.MousePosPrev))
        g.IO.MouseDelta = g.IO.MousePos - g.IO.MousePosPrev;
    else
        g.IO.MouseDelta = ImVec2(0.0f, 0.0f);
    g.IO.MousePosPrev = g.IO.MousePos;
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
    {
        g.IO.MouseClicked[i] = g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] < 0.0f;
        g.IO.MouseReleased[i] = !g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] >= 0.0f;
        g.IO.MouseDownDurationPrev[i] = g.IO.MouseDownDuration[i];
        g.IO.MouseDownDuration[i] = g.IO.MouseDown[i] ? (g.IO.MouseDownDuration[i] < 0.0f ? 0.0f : g.IO.MouseDownDuration[i] + g.IO.DeltaTime) : -1.0f;
        g.IO.MouseDoubleClicked[i] = false;
        if (g.IO.MouseClicked[i])
        {
            if (g.Time - g.IO.MouseClickedTime[i] < g.IO.MouseDoubleClickTime)
            {
                if (ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]) < g.IO.MouseDoubleClickMaxDist * g.IO.MouseDoubleClickMaxDist)
                    g.IO.MouseDoubleClicked[i] = true;
                g.IO.MouseClickedTime[i] = -FLT_MAX;    // so the third click isn't turned into a double-click
            }
            else
            {
                g.IO.MouseClickedTime[i] = g.Time;
            }
            g.IO.MouseClickedPos[i] = g.IO.MousePos;
            g.IO.MouseDragMaxDistanceSqr[i] = 0.0f;
        }
        else if (g.IO.MouseDown[i])
        {
            g.IO.MouseDragMaxDistanceSqr[i] = ImMax(g.IO.MouseDragMaxDistanceSqr[i], ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]));
        }
    }

    // Calculate frame-rate for the user, as a purely luxurious feature
    g.FramerateSecPerFrameAccum += g.IO.DeltaTime - g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx];
    g.FramerateSecPerFrame[g.FramerateSecPerFrameIdx] = g.IO.DeltaTime;
    g.FramerateSecPerFrameIdx = (g.FramerateSecPerFrameIdx + 1) % IM_ARRAYSIZE(g.FramerateSecPerFrame);
    g.IO.Framerate = 1.0f / (g.FramerateSecPerFrameAccum / (float)IM_ARRAYSIZE(g.FramerateSecPerFrame));

	// Handle the disabling of the move action if window has the required flag.
	if (g.MovedWindow)
	{
		if (g.MovedWindow->RootWindow->Flags & ImGuiWindowFlags_OnlyDragByTitleBar)
		{
			ImRect title_rect = g.MovedWindow->RootWindow->TitleBarRect();
			bool mouse_down = g.IO.MouseDown[0];
			if (mouse_down && IsMouseHoveringRect(title_rect.Min, title_rect.Max, false))
			{
				g.MovedWindow->RootWindow->StartedDragging = true;
			}
			else if (!mouse_down || !g.MovedWindow->RootWindow->StartedDragging)
			{
				g.MovedWindow->RootWindow->StartedDragging = false;
				g.MovedWindow = NULL;
				g.MovedWindowMoveId = 0;
			}
		}
	}

    // Handle user moving window with mouse (at the beginning of the frame to avoid input lag or sheering). Only valid for root windows.
    if (g.MovedWindowMoveId && g.MovedWindowMoveId == g.ActiveId)
    {
        KeepAliveID(g.MovedWindowMoveId);
        IM_ASSERT(g.MovedWindow && g.MovedWindow->RootWindow);
        IM_ASSERT(g.MovedWindow->MoveId == g.MovedWindowMoveId);
        if (g.IO.MouseDown[0])
        {
            g.MovedWindow->RootWindow->PosFloat += g.IO.MouseDelta;
            if (g.IO.MouseDelta.x != 0.0f || g.IO.MouseDelta.y != 0.0f)
                MarkIniSettingsDirty(g.MovedWindow->RootWindow);
            FocusWindow(g.MovedWindow);
        }
        else
        {
            ClearActiveID();
            g.MovedWindow = NULL;
            g.MovedWindowMoveId = 0;
        }
    }
    else
    {
        g.MovedWindow = NULL;
        g.MovedWindowMoveId = 0;
    }

    // Delay saving settings so we don't spam disk too much
    if (g.SettingsDirtyTimer > 0.0f)
    {
        g.SettingsDirtyTimer -= g.IO.DeltaTime;
        if (g.SettingsDirtyTimer <= 0.0f)
            SaveIniSettingsToDisk(g.IO.IniFilename);
    }

    // Find the window we are hovering. Child windows can extend beyond the limit of their parent so we need to derive HoveredRootWindow from HoveredWindow
    g.HoveredWindow = g.MovedWindow ? g.MovedWindow : FindHoveredWindow(g.IO.MousePos, false);
    if (g.HoveredWindow && (g.HoveredWindow->Flags & ImGuiWindowFlags_ChildWindow))
        g.HoveredRootWindow = g.HoveredWindow->RootWindow;
    else
        g.HoveredRootWindow = g.MovedWindow ? g.MovedWindow->RootWindow : FindHoveredWindow(g.IO.MousePos, true);

    if (ImGuiWindow* modal_window = GetFrontMostModalRootWindow())
    {
        g.ModalWindowDarkeningRatio = ImMin(g.ModalWindowDarkeningRatio + g.IO.DeltaTime * 6.0f, 1.0f);
        ImGuiWindow* window = g.HoveredRootWindow;
        while (window && window != modal_window)
            window = window->ParentWindow;
        if (!window)
            g.HoveredRootWindow = g.HoveredWindow = NULL;
    }
    else
    {
        g.ModalWindowDarkeningRatio = 0.0f;
    }

    // Are we using inputs? Tell user so they can capture/discard the inputs away from the rest of their application.
    // When clicking outside of a window we assume the click is owned by the application and won't request capture. We need to track click ownership.
    int mouse_earliest_button_down = -1;
    bool mouse_any_down = false;
    for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
    {
        if (g.IO.MouseClicked[i])
            g.IO.MouseDownOwned[i] = (g.HoveredWindow != NULL) || (!g.OpenPopupStack.empty());
        mouse_any_down |= g.IO.MouseDown[i];
        if (g.IO.MouseDown[i])
            if (mouse_earliest_button_down == -1 || g.IO.MouseClickedTime[mouse_earliest_button_down] > g.IO.MouseClickedTime[i])
                mouse_earliest_button_down = i;
    }
    bool mouse_avail_to_imgui = (mouse_earliest_button_down == -1) || g.IO.MouseDownOwned[mouse_earliest_button_down];
    if (g.WantCaptureMouseNextFrame != -1)
        g.IO.WantCaptureMouse = (g.WantCaptureMouseNextFrame != 0);
    else
        g.IO.WantCaptureMouse = (mouse_avail_to_imgui && (g.HoveredWindow != NULL || mouse_any_down)) || (!g.OpenPopupStack.empty());
    g.IO.WantCaptureKeyboard = (g.WantCaptureKeyboardNextFrame != -1) ? (g.WantCaptureKeyboardNextFrame != 0) : (g.ActiveId != 0);
    g.IO.WantTextInput = (g.WantTextInputNextFrame != -1) ? (g.WantTextInputNextFrame != 0) : 0;
    g.MouseCursor = ImGuiMouseCursor_Arrow;
    g.WantCaptureMouseNextFrame = g.WantCaptureKeyboardNextFrame = g.WantTextInputNextFrame = -1;
    g.OsImePosRequest = ImVec2(1.0f, 1.0f); // OS Input Method Editor showing on top-left of our window by default

    // If mouse was first clicked outside of ImGui bounds we also cancel out hovering.
    if (!mouse_avail_to_imgui)
        g.HoveredWindow = g.HoveredRootWindow = NULL;

    // Scale & Scrolling
    if (g.HoveredWindow && g.IO.MouseWheel != 0.0f && !g.HoveredWindow->Collapsed)
    {
        ImGuiWindow* window = g.HoveredWindow;
        if (g.IO.KeyCtrl && g.IO.FontAllowUserScaling)
        {
            // Zoom / Scale window
            const float new_font_scale = ImClamp(window->FontWindowScale + g.IO.MouseWheel * 0.10f, 0.50f, 2.50f);
            const float scale = new_font_scale / window->FontWindowScale;
            window->FontWindowScale = new_font_scale;

            const ImVec2 offset = window->Size * (1.0f - scale) * (g.IO.MousePos - window->Pos) / window->Size;
            window->Pos += offset;
            window->PosFloat += offset;
            window->Size *= scale;
            window->SizeFull *= scale;
        }
        else if (!g.IO.KeyCtrl && !(window->Flags & ImGuiWindowFlags_NoScrollWithMouse))
        {
            // Scroll
            const int scroll_lines = (window->Flags & ImGuiWindowFlags_ComboBox) ? 3 : 5;
            SetWindowScrollY(window, window->Scroll.y - g.IO.MouseWheel * window->CalcFontSize() * scroll_lines);
        }
    }

    // Pressing TAB activate widget focus
    if (g.ActiveId == 0 && g.NavWindow != NULL && g.NavWindow->Active && IsKeyPressedMap(ImGuiKey_Tab, false))
        g.NavWindow->FocusIdxTabRequestNext = 0;

    // Mark all windows as not visible
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        window->WasActive = window->Active;
        window->Active = false;
        window->Accessed = false;
    }

    // Closing the focused window restore focus to the first active root window in descending z-order
    if (g.NavWindow && !g.NavWindow->WasActive)
        for (int i = g.Windows.Size-1; i >= 0; i--)
            if (g.Windows[i]->WasActive && !(g.Windows[i]->Flags & ImGuiWindowFlags_ChildWindow))
            {
                FocusWindow(g.Windows[i]);
                break;
            }

    // No window should be open at the beginning of the frame.
    // But in order to allow the user to call NewFrame() multiple times without calling Render(), we are doing an explicit clear.
    g.CurrentWindowStack.resize(0);
    g.CurrentPopupStack.resize(0);
    CloseInactivePopups();

    // Create implicit window - we will only render it if the user has added something to it.
    // We don't use "Debug" to avoid colliding with user trying to create a "Debug" window with custom flags.
    ImGui::SetNextWindowSize(ImVec2(400,400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug##Default");
}

void ImGui::Initialize()
{
    ImGuiContext& g = *GImGui;
    g.LogClipboard = (ImGuiTextBuffer*)ImGui::MemAlloc(sizeof(ImGuiTextBuffer));
    IM_PLACEMENT_NEW(g.LogClipboard) ImGuiTextBuffer();

    IM_ASSERT(g.Settings.empty());
    LoadIniSettingsFromDisk(g.IO.IniFilename);
    g.Initialized = true;
}

// This function is merely here to free heap allocations.
void ImGui::Shutdown()
{
    ImGuiContext& g = *GImGui;

    // The fonts atlas can be used prior to calling NewFrame(), so we clear it even if g.Initialized is FALSE (which would happen if we never called NewFrame)
    if (g.IO.Fonts) // Testing for NULL to allow user to NULLify in case of running Shutdown() on multiple contexts. Bit hacky.
        g.IO.Fonts->Clear();

    // Cleanup of other data are conditional on actually having initialize ImGui.
    if (!g.Initialized)
        return;

    SaveIniSettingsToDisk(g.IO.IniFilename);

    for (int i = 0; i < g.Windows.Size; i++)
    {
        g.Windows[i]->~ImGuiWindow();
        ImGui::MemFree(g.Windows[i]);
    }
    g.Windows.clear();
    g.WindowsSortBuffer.clear();
    g.CurrentWindow = NULL;
    g.CurrentWindowStack.clear();
    g.NavWindow = NULL;
    g.HoveredWindow = NULL;
    g.HoveredRootWindow = NULL;
    g.ActiveIdWindow = NULL;
    g.MovedWindow = NULL;
    for (int i = 0; i < g.Settings.Size; i++)
        ImGui::MemFree(g.Settings[i].Name);
    g.Settings.clear();
    g.ColorModifiers.clear();
    g.StyleModifiers.clear();
    g.FontStack.clear();
    g.OpenPopupStack.clear();
    g.CurrentPopupStack.clear();
    g.SetNextWindowSizeConstraintCallback = NULL;
    g.SetNextWindowSizeConstraintCallbackUserData = NULL;
    for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
        g.RenderDrawLists[i].clear();
    g.OverlayDrawList.ClearFreeMemory();
    g.PrivateClipboard.clear();
    g.InputTextState.Text.clear();
    g.InputTextState.InitialText.clear();
    g.InputTextState.TempTextBuffer.clear();

    if (g.LogFile && g.LogFile != stdout)
    {
        fclose(g.LogFile);
        g.LogFile = NULL;
    }
    if (g.LogClipboard)
    {
        g.LogClipboard->~ImGuiTextBuffer();
        ImGui::MemFree(g.LogClipboard);
    }

    g.Initialized = false;
}

static ImGuiIniData* FindWindowSettings(const char* name)
{
    ImGuiContext& g = *GImGui;
    ImGuiID id = ImHash(name, 0);
    for (int i = 0; i != g.Settings.Size; i++)
    {
        ImGuiIniData* ini = &g.Settings[i];
        if (ini->Id == id)
            return ini;
    }
    return NULL;
}

static ImGuiIniData* AddWindowSettings(const char* name)
{
    GImGui->Settings.resize(GImGui->Settings.Size + 1);
    ImGuiIniData* ini = &GImGui->Settings.back();
    ini->Name = ImStrdup(name);
    ini->Id = ImHash(name, 0);
    ini->Collapsed = false;
    ini->Pos = ImVec2(FLT_MAX,FLT_MAX);
    ini->Size = ImVec2(0,0);
    return ini;
}

// Zero-tolerance, poor-man .ini parsing
// FIXME: Write something less rubbish
static void LoadIniSettingsFromDisk(const char* ini_filename)
{
    ImGuiContext& g = *GImGui;
    if (!ini_filename)
        return;

    int file_size;
    char* file_data = (char*)ImFileLoadToMemory(ini_filename, "rb", &file_size, 1);
    if (!file_data)
        return;

    ImGuiIniData* settings = NULL;
    const char* buf_end = file_data + file_size;
    for (const char* line_start = file_data; line_start < buf_end; )
    {
        const char* line_end = line_start;
        while (line_end < buf_end && *line_end != '\n' && *line_end != '\r')
            line_end++;

        if (line_start[0] == '[' && line_end > line_start && line_end[-1] == ']')
        {
            char name[64];
            ImFormatString(name, IM_ARRAYSIZE(name), "%.*s", (int)(line_end-line_start-2), line_start+1);
            settings = FindWindowSettings(name);
            if (!settings)
                settings = AddWindowSettings(name);
        }
        else if (settings)
        {
            float x, y;
            int i;
            if (sscanf(line_start, "Pos=%f,%f", &x, &y) == 2)
                settings->Pos = ImVec2(x, y);
            else if (sscanf(line_start, "Size=%f,%f", &x, &y) == 2)
                settings->Size = ImMax(ImVec2(x, y), g.Style.WindowMinSize);
            else if (sscanf(line_start, "Collapsed=%d", &i) == 1)
                settings->Collapsed = (i != 0);
        }

        line_start = line_end+1;
    }

    ImGui::MemFree(file_data);
}

static void SaveIniSettingsToDisk(const char* ini_filename)
{
    ImGuiContext& g = *GImGui;
    g.SettingsDirtyTimer = 0.0f;
    if (!ini_filename)
        return;

    // Gather data from windows that were active during this session
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        if (window->Flags & ImGuiWindowFlags_NoSavedSettings)
            continue;
        ImGuiIniData* settings = FindWindowSettings(window->Name);
        if (!settings)  // This will only return NULL in the rare instance where the window was first created with ImGuiWindowFlags_NoSavedSettings then had the flag disabled later on. We don't bind settings in this case (bug #1000).
            continue;
        settings->Pos = window->Pos;
        settings->Size = window->SizeFull;
        settings->Collapsed = window->Collapsed;
    }

    // Write .ini file
    // If a window wasn't opened in this session we preserve its settings
    FILE* f = ImFileOpen(ini_filename, "wt");
    if (!f)
        return;
    for (int i = 0; i != g.Settings.Size; i++)
    {
        const ImGuiIniData* settings = &g.Settings[i];
        if (settings->Pos.x == FLT_MAX)
            continue;
        const char* name = settings->Name;
        if (const char* p = strstr(name, "###"))  // Skip to the "###" marker if any. We don't skip past to match the behavior of GetID()
            name = p;
        fprintf(f, "[%s]\n", name);
        fprintf(f, "Pos=%d,%d\n", (int)settings->Pos.x, (int)settings->Pos.y);
        fprintf(f, "Size=%d,%d\n", (int)settings->Size.x, (int)settings->Size.y);
        fprintf(f, "Collapsed=%d\n", settings->Collapsed);
        fprintf(f, "\n");
    }

    fclose(f);
}

static void MarkIniSettingsDirty(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    if (!(window->Flags & ImGuiWindowFlags_NoSavedSettings))
        if (g.SettingsDirtyTimer <= 0.0f)
            g.SettingsDirtyTimer = g.IO.IniSavingRate;
}

// FIXME: Add a more explicit sort order in the window structure.
static int ChildWindowComparer(const void* lhs, const void* rhs)
{
    const ImGuiWindow* a = *(const ImGuiWindow**)lhs;
    const ImGuiWindow* b = *(const ImGuiWindow**)rhs;
    if (int d = (a->Flags & ImGuiWindowFlags_Popup) - (b->Flags & ImGuiWindowFlags_Popup))
        return d;
    if (int d = (a->Flags & ImGuiWindowFlags_Tooltip) - (b->Flags & ImGuiWindowFlags_Tooltip))
        return d;
    if (int d = (a->Flags & ImGuiWindowFlags_ComboBox) - (b->Flags & ImGuiWindowFlags_ComboBox))
        return d;
    return (a->OrderWithinParent - b->OrderWithinParent);
}

static void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window)
{
    out_sorted_windows.push_back(window);
    if (window->Active)
    {
        int count = window->DC.ChildWindows.Size;
        if (count > 1)
            qsort(window->DC.ChildWindows.begin(), (size_t)count, sizeof(ImGuiWindow*), ChildWindowComparer);
        for (int i = 0; i < count; i++)
        {
            ImGuiWindow* child = window->DC.ChildWindows[i];
            if (child->Active)
                AddWindowToSortedBuffer(out_sorted_windows, child);
        }
    }
}

static void AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list)
{
    if (draw_list->CmdBuffer.empty())
        return;

    // Remove trailing command if unused
    ImDrawCmd& last_cmd = draw_list->CmdBuffer.back();
    if (last_cmd.ElemCount == 0 && last_cmd.UserCallback == NULL)
    {
        draw_list->CmdBuffer.pop_back();
        if (draw_list->CmdBuffer.empty())
            return;
    }

    // Draw list sanity check. Detect mismatch between PrimReserve() calls and incrementing _VtxCurrentIdx, _VtxWritePtr etc. May trigger for you if you are using PrimXXX functions incorrectly.
    IM_ASSERT(draw_list->VtxBuffer.Size == 0 || draw_list->_VtxWritePtr == draw_list->VtxBuffer.Data + draw_list->VtxBuffer.Size);
    IM_ASSERT(draw_list->IdxBuffer.Size == 0 || draw_list->_IdxWritePtr == draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size);
    IM_ASSERT((int)draw_list->_VtxCurrentIdx == draw_list->VtxBuffer.Size);

    // Check that draw_list doesn't use more vertices than indexable in a single draw call (default ImDrawIdx = unsigned short = 2 bytes = 64K vertices per window)
    // If this assert triggers because you are drawing lots of stuff manually, you can:
    // A) Add '#define ImDrawIdx unsigned int' in imconfig.h to set the index size to 4 bytes. You'll need to handle the 4-bytes indices to your renderer.
    //    For example, the OpenGL example code detect index size at compile-time by doing:
    //    'glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);'
    //    Your own engine or render API may use different parameters or function calls to specify index sizes. 2 and 4 bytes indices are generally supported by most API.
    // B) If for some reason you cannot use 4 bytes indices or don't want to, a workaround is to call BeginChild()/EndChild() before reaching the 64K limit to split your draw commands in multiple draw lists.
    IM_ASSERT(((ImU64)draw_list->_VtxCurrentIdx >> (sizeof(ImDrawIdx)*8)) == 0);  // Too many vertices in same ImDrawList. See comment above.
    
    out_render_list.push_back(draw_list);
    GImGui->IO.MetricsRenderVertices += draw_list->VtxBuffer.Size;
    GImGui->IO.MetricsRenderIndices += draw_list->IdxBuffer.Size;
}

static void AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window)
{
    AddDrawListToRenderList(out_render_list, window->DrawList);
    for (int i = 0; i < window->DC.ChildWindows.Size; i++)
    {
        ImGuiWindow* child = window->DC.ChildWindows[i];
        if (!child->Active) // clipped children may have been marked not active
            continue;
        if ((child->Flags & ImGuiWindowFlags_Popup) && child->HiddenFrames > 0)
            continue;
        AddWindowToRenderList(out_render_list, child);
    }
}

static void AddWindowToRenderListSelectLayer(ImGuiWindow* window)
{
    // FIXME: Generalize this with a proper layering system so e.g. user can draw in specific layers, below text, ..
    ImGuiContext& g = *GImGui;
    g.IO.MetricsActiveWindows++;
    if (window->Flags & ImGuiWindowFlags_Popup)
        AddWindowToRenderList(g.RenderDrawLists[1], window);
    else if (window->Flags & ImGuiWindowFlags_Tooltip)
        AddWindowToRenderList(g.RenderDrawLists[2], window);
    else
        AddWindowToRenderList(g.RenderDrawLists[0], window);
}

// When using this function it is sane to ensure that float are perfectly rounded to integer values, to that e.g. (int)(max.x-min.x) in user's render produce correct result.
void ImGui::PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DrawList->PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
    window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void ImGui::PopClipRect()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DrawList->PopClipRect();
    window->ClipRect = window->DrawList->_ClipRectStack.back();
}

// This is normally called by Render(). You may want to call it directly if you want to avoid calling Render() but the gain will be very minimal.
void ImGui::EndFrame()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);                       // Forgot to call ImGui::NewFrame()
    IM_ASSERT(g.FrameCountEnded != g.FrameCount);   // ImGui::EndFrame() called multiple times, or forgot to call ImGui::NewFrame() again

    // Notify OS when our Input Method Editor cursor has moved (e.g. CJK inputs using Microsoft IME)
    if (g.IO.ImeSetInputScreenPosFn && ImLengthSqr(g.OsImePosRequest - g.OsImePosSet) > 0.0001f)
    {
        g.IO.ImeSetInputScreenPosFn((int)g.OsImePosRequest.x, (int)g.OsImePosRequest.y);
        g.OsImePosSet = g.OsImePosRequest;
    }

    // Hide implicit "Debug" window if it hasn't been used
    IM_ASSERT(g.CurrentWindowStack.Size == 1);    // Mismatched Begin()/End() calls
    if (g.CurrentWindow && !g.CurrentWindow->Accessed)
        g.CurrentWindow->Active = false;
    ImGui::End();

    // Click to focus window and start moving (after we're done with all our widgets)
    if (g.ActiveId == 0 && g.HoveredId == 0 && g.IO.MouseClicked[0])
    {
        if (!(g.NavWindow && !g.NavWindow->WasActive && g.NavWindow->Active)) // Unless we just made a popup appear
        {
            if (g.HoveredRootWindow != NULL)
            {
                FocusWindow(g.HoveredWindow);
                if (!(g.HoveredWindow->Flags & ImGuiWindowFlags_NoMove))
                {
                    g.MovedWindow = g.HoveredWindow;
                    g.MovedWindowMoveId = g.HoveredWindow->MoveId;
                    SetActiveID(g.MovedWindowMoveId, g.HoveredRootWindow);
                }
            }
            else if (g.NavWindow != NULL && GetFrontMostModalRootWindow() == NULL)
            {
                // Clicking on void disable focus
                FocusWindow(NULL);
            }
        }
    }

    // Sort the window list so that all child windows are after their parent
    // We cannot do that on FocusWindow() because childs may not exist yet
    g.WindowsSortBuffer.resize(0);
    g.WindowsSortBuffer.reserve(g.Windows.Size);
    for (int i = 0; i != g.Windows.Size; i++)
    {
        ImGuiWindow* window = g.Windows[i];
        if (window->Active && (window->Flags & ImGuiWindowFlags_ChildWindow))       // if a child is active its parent will add it
            continue;
        AddWindowToSortedBuffer(g.WindowsSortBuffer, window);
    }

    IM_ASSERT(g.Windows.Size == g.WindowsSortBuffer.Size);  // we done something wrong
    g.Windows.swap(g.WindowsSortBuffer);

    // Clear Input data for next frame
    g.IO.MouseWheel = 0.0f;
    memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));

    g.FrameCountEnded = g.FrameCount;
}

void ImGui::Render()
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(g.Initialized);   // Forgot to call ImGui::NewFrame()

    if (g.FrameCountEnded != g.FrameCount)
        ImGui::EndFrame();
    g.FrameCountRendered = g.FrameCount;

    // Skip render altogether if alpha is 0.0
    // Note that vertex buffers have been created and are wasted, so it is best practice that you don't create windows in the first place, or consistently respond to Begin() returning false.
    if (g.Style.Alpha > 0.0f)
    {
        // Gather windows to render
        g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices = g.IO.MetricsActiveWindows = 0;
        for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
            g.RenderDrawLists[i].resize(0);
        for (int i = 0; i != g.Windows.Size; i++)
        {
            ImGuiWindow* window = g.Windows[i];
            if (window->Active && window->HiddenFrames <= 0 && (window->Flags & (ImGuiWindowFlags_ChildWindow)) == 0)
                AddWindowToRenderListSelectLayer(window);
        }

        // Flatten layers
        int n = g.RenderDrawLists[0].Size;
        int flattened_size = n;
        for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
            flattened_size += g.RenderDrawLists[i].Size;
        g.RenderDrawLists[0].resize(flattened_size);
        for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
        {
            ImVector<ImDrawList*>& layer = g.RenderDrawLists[i];
            if (layer.empty())
                continue;
            memcpy(&g.RenderDrawLists[0][n], &layer[0], layer.Size * sizeof(ImDrawList*));
            n += layer.Size;
        }

        // Draw software mouse cursor if requested
        if (g.IO.MouseDrawCursor)
        {
            const ImGuiMouseCursorData& cursor_data = g.MouseCursorData[g.MouseCursor];
            const ImVec2 pos = g.IO.MousePos - cursor_data.HotOffset;
            const ImVec2 size = cursor_data.Size;
            const ImTextureID tex_id = g.IO.Fonts->TexID;
            g.OverlayDrawList.PushTextureID(tex_id);
            g.OverlayDrawList.AddImage(tex_id, pos+ImVec2(1,0), pos+ImVec2(1,0) + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0,0,0,48));        // Shadow
            g.OverlayDrawList.AddImage(tex_id, pos+ImVec2(2,0), pos+ImVec2(2,0) + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0,0,0,48));        // Shadow
            g.OverlayDrawList.AddImage(tex_id, pos,             pos + size,             cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0,0,0,255));       // Black border
            g.OverlayDrawList.AddImage(tex_id, pos,             pos + size,             cursor_data.TexUvMin[0], cursor_data.TexUvMax[0], IM_COL32(0, 153, 204, 255)); // White fill IM_COL32(255,255,255,255)
            g.OverlayDrawList.PopTextureID();
        }
        if (!g.OverlayDrawList.VtxBuffer.empty())
            AddDrawListToRenderList(g.RenderDrawLists[0], &g.OverlayDrawList);

        // Setup draw data
        g.RenderDrawData.Valid = true;
        g.RenderDrawData.CmdLists = (g.RenderDrawLists[0].Size > 0) ? &g.RenderDrawLists[0][0] : NULL;
        g.RenderDrawData.CmdListsCount = g.RenderDrawLists[0].Size;
        g.RenderDrawData.TotalVtxCount = g.IO.MetricsRenderVertices;
        g.RenderDrawData.TotalIdxCount = g.IO.MetricsRenderIndices;

        // Render. If user hasn't set a callback then they may retrieve the draw data via GetDrawData()
        if (g.RenderDrawData.CmdListsCount > 0 && g.IO.RenderDrawListsFn != NULL)
            g.IO.RenderDrawListsFn(&g.RenderDrawData);
    }
}

const char* ImGui::FindRenderedTextEnd(const char* text, const char* text_end)
{
    const char* text_display_end = text;
    if (!text_end)
        text_end = (const char*)-1;

    while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
        text_display_end++;
    return text_display_end;
}

// Pass text data straight to log (without being displayed)
void ImGui::LogText(const char* fmt, ...)
{
    ImGuiContext& g = *GImGui;
    if (!g.LogEnabled)
        return;

    va_list args;
    va_start(args, fmt);
    if (g.LogFile)
    {
        vfprintf(g.LogFile, fmt, args);
    }
    else
    {
        g.LogClipboard->appendv(fmt, args);
    }
    va_end(args);
}

// Internal version that takes a position to decide on newline placement and pad items according to their depth.
// We split text into individual lines to add current tree level padding
static void LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end = NULL)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (!text_end)
        text_end = ImGui::FindRenderedTextEnd(text, text_end);

    const bool log_new_line = ref_pos && (ref_pos->y > window->DC.LogLinePosY + 1);
    if (ref_pos)
        window->DC.LogLinePosY = ref_pos->y;

    const char* text_remaining = text;
    if (g.LogStartDepth > window->DC.TreeDepth)  // Re-adjust padding if we have popped out of our starting depth
        g.LogStartDepth = window->DC.TreeDepth;
    const int tree_depth = (window->DC.TreeDepth - g.LogStartDepth);
    for (;;)
    {
        // Split the string. Each new line (after a '\n') is followed by spacing corresponding to the current depth of our log entry.
        const char* line_end = text_remaining;
        while (line_end < text_end)
            if (*line_end == '\n')
                break;
            else
                line_end++;
        if (line_end >= text_end)
            line_end = NULL;

        const bool is_first_line = (text == text_remaining);
        bool is_last_line = false;
        if (line_end == NULL)
        {
            is_last_line = true;
            line_end = text_end;
        }
        if (line_end != NULL && !(is_last_line && (line_end - text_remaining)==0))
        {
            const int char_count = (int)(line_end - text_remaining);
            if (log_new_line || !is_first_line)
                ImGui::LogText(IM_NEWLINE "%*s%.*s", tree_depth*4, "", char_count, text_remaining);
            else
                ImGui::LogText(" %.*s", char_count, text_remaining);
        }

        if (is_last_line)
            break;
        text_remaining = line_end + 1;
    }
}

// Internal ImGui functions to render text
// RenderText***() functions calls ImDrawList::AddText() calls ImBitmapFont::RenderText()
void ImGui::RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    // Hide anything after a '##' string
    const char* text_display_end;
    if (hide_text_after_hash)
    {
        text_display_end = FindRenderedTextEnd(text, text_end);
    }
    else
    {
        if (!text_end)
            text_end = text + strlen(text); // FIXME-OPT
        text_display_end = text_end;
    }

    const int text_len = (int)(text_display_end - text);
    if (text_len > 0)
    {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end);
        if (g.LogEnabled)
            LogRenderedText(&pos, text, text_display_end);
    }
}

void ImGui::RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (!text_end)
        text_end = text + strlen(text); // FIXME-OPT

    const int text_len = (int)(text_end - text);
    if (text_len > 0)
    {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_end, wrap_width);
        if (g.LogEnabled)
            LogRenderedText(&pos, text, text_end);
    }
}

// Default clip_rect uses (pos_min,pos_max)
// Handle clipping on CPU immediately (vs typically let the GPU clip the triangles that are overlapping the clipping rectangle edges)
void ImGui::RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
    // Hide anything after a '##' string
    const char* text_display_end = FindRenderedTextEnd(text, text_end);
    const int text_len = (int)(text_display_end - text);
    if (text_len == 0)
        return;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    // Perform CPU side clipping for single clipped element to avoid using scissor state
    ImVec2 pos = pos_min;
    const ImVec2 text_size = text_size_if_known ? *text_size_if_known : CalcTextSize(text, text_display_end, false, 0.0f);

    const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
    const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
    bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
    if (clip_rect) // If we had no explicit clipping rectangle then pos==clip_min
        need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

    // Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
    if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
    if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

    // Render
    if (need_clipping)
    {
        ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
    }
    else
    {
        window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
    }
    if (g.LogEnabled)
        LogRenderedText(&pos, text, text_display_end);
}

// Render a rectangle shaped with optional rounding and borders
void ImGui::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
    if (border && (window->Flags & ImGuiWindowFlags_ShowBorders))
    {
        window->DrawList->AddRect(p_min+ImVec2(1,1), p_max+ImVec2(1,1), GetColorU32(ImGuiCol_BorderShadow), rounding);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding);
    }
}

void ImGui::RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->Flags & ImGuiWindowFlags_ShowBorders)
    {
        window->DrawList->AddRect(p_min+ImVec2(1,1), p_max+ImVec2(1,1), GetColorU32(ImGuiCol_BorderShadow), rounding);
        window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding);
    }
}

// Render a triangle to denote expanded/collapsed state
void ImGui::RenderTriangle(ImVec2 p_min, ImGuiDir dir, float scale)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    const float h = g.FontSize * 1.00f;
    float r = h * 0.40f * scale;
    ImVec2 center = p_min + ImVec2(h * 0.50f, h * 0.50f * scale);

    ImVec2 a, b, c;
    switch (dir)
    {
    case ImGuiDir_Up:
        r = -r; // ...fall through, no break!
    case ImGuiDir_Down:
        center.y -= r * 0.25f;
        a = ImVec2(0,1) * r;
        b = ImVec2(-0.866f,-0.5f) * r;
        c = ImVec2(+0.866f,-0.5f) * r;
        break;
    case ImGuiDir_Left:
        r = -r; // ...fall through, no break!
    case ImGuiDir_Right:
        a = ImVec2(1,0) * r;
        b = ImVec2(-0.500f,+0.866f) * r;
        c = ImVec2(-0.500f,-0.866f) * r;
        break;
    default:
        IM_ASSERT(0);
        break;
    }

    window->DrawList->AddTriangleFilled(center + a, center + b, center + c, GetColorU32(ImGuiCol_Text));
}

void ImGui::RenderBullet(ImVec2 pos)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddCircleFilled(pos, GImGui->FontSize*0.20f, GetColorU32(ImGuiCol_Text), 8);
}

void ImGui::RenderCheckMark(ImVec2 pos, ImU32 col, float sz)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    float thickness = ImMax(sz / 5.0f, 1.0f);
    sz -= thickness*0.5f;
    pos += ImVec2(thickness*0.25f, thickness*0.25f);

    float third = sz / 3.0f;
    float bx = pos.x + third;
    float by = pos.y + sz - third*0.5f;
    window->DrawList->PathLineTo(ImVec2(bx - third, by - third));
    window->DrawList->PathLineTo(ImVec2(bx, by));
    window->DrawList->PathLineTo(ImVec2(bx + third*2, by - third*2));
    window->DrawList->PathStroke(col, false, thickness);
}

// Calculate text size. Text can be multi-line. Optionally ignore text after a ## marker.
// CalcTextSize("") should return ImVec2(0.0f, GImGui->FontSize)
ImVec2 ImGui::CalcTextSize(const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
    ImGuiContext& g = *GImGui;

    const char* text_display_end;
    if (hide_text_after_double_hash)
        text_display_end = FindRenderedTextEnd(text, text_end);      // Hide anything after a '##' string
    else
        text_display_end = text_end;

    ImFont* font = g.Font;
    const float font_size = g.FontSize;
    if (text == text_display_end)
        return ImVec2(0.0f, font_size);
    ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

    // Cancel out character spacing for the last character of a line (it is baked into glyph->AdvanceX field)
    const float font_scale = font_size / font->FontSize;
    const float character_spacing_x = 1.0f * font_scale;
    if (text_size.x > 0.0f)
        text_size.x -= character_spacing_x;
    text_size.x = (float)(int)(text_size.x + 0.95f);

    return text_size;
}

// Helper to calculate coarse clipping of large list of evenly sized items.
// NB: Prefer using the ImGuiListClipper higher-level helper if you can! Read comments and instructions there on how those use this sort of pattern.
// NB: 'items_count' is only used to clamp the result, if you don't know your count you can use INT_MAX
void ImGui::CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (g.LogEnabled)
    {
        // If logging is active, do not perform any clipping
        *out_items_display_start = 0;
        *out_items_display_end = items_count;
        return;
    }
    if (window->SkipItems)
    {
        *out_items_display_start = *out_items_display_end = 0;
        return;
    }

    const ImVec2 pos = window->DC.CursorPos;
    int start = (int)((window->ClipRect.Min.y - pos.y) / items_height);
    int end = (int)((window->ClipRect.Max.y - pos.y) / items_height);
    start = ImClamp(start, 0, items_count);
    end = ImClamp(end + 1, start, items_count);
    *out_items_display_start = start;
    *out_items_display_end = end;
}

// Find window given position, search front-to-back
// FIXME: Note that we have a lag here because WindowRectClipped is updated in Begin() so windows moved by user via SetWindowPos() and not SetNextWindowPos() will have that rectangle lagging by a frame at the time FindHoveredWindow() is called, aka before the next Begin(). Moving window thankfully isn't affected.
static ImGuiWindow* FindHoveredWindow(ImVec2 pos, bool excluding_childs)
{
    ImGuiContext& g = *GImGui;
    for (int i = g.Windows.Size-1; i >= 0; i--)
    {
        ImGuiWindow* window = g.Windows[i];
        if (!window->Active)
            continue;
        if (window->Flags & ImGuiWindowFlags_NoInputs)
            continue;
        if (excluding_childs && (window->Flags & ImGuiWindowFlags_ChildWindow) != 0)
            continue;

        // Using the clipped AABB so a child window will typically be clipped by its parent.
        ImRect bb(window->WindowRectClipped.Min - g.Style.TouchExtraPadding, window->WindowRectClipped.Max + g.Style.TouchExtraPadding);
        if (bb.Contains(pos))
            return window;
    }
    return NULL;
}

// Test if mouse cursor is hovering given rectangle
// NB- Rectangle is clipped by our current clip setting
// NB- Expand the rectangle to be generous on imprecise inputs systems (g.Style.TouchExtraPadding)
bool ImGui::IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    // Clip
    ImRect rect_clipped(r_min, r_max);
    if (clip)
        rect_clipped.ClipWith(window->ClipRect);

    // Expand for touch input
    const ImRect rect_for_touch(rect_clipped.Min - g.Style.TouchExtraPadding, rect_clipped.Max + g.Style.TouchExtraPadding);
    return rect_for_touch.Contains(g.IO.MousePos);
}

bool ImGui::IsAnyWindowHovered()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredWindow != NULL;
}

static bool IsKeyPressedMap(ImGuiKey key, bool repeat)
{
    const int key_index = GImGui->IO.KeyMap[key];
    return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}

int ImGui::GetKeyIndex(ImGuiKey imgui_key)
{
    IM_ASSERT(imgui_key >= 0 && imgui_key < ImGuiKey_COUNT);
    return GImGui->IO.KeyMap[imgui_key];
}

// Note that imgui doesn't know the semantic of each entry of io.KeyDown[]. Use your own indices/enums according to how your backend/engine stored them into KeyDown[]!
bool ImGui::IsKeyDown(int user_key_index)
{
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(GImGui->IO.KeysDown));
    return GImGui->IO.KeysDown[user_key_index];
}

int ImGui::CalcTypematicPressedRepeatAmount(float t, float t_prev, float repeat_delay, float repeat_rate)
{
    if (t == 0.0f)
        return 1;
    if (t <= repeat_delay || repeat_rate <= 0.0f)
        return 0;
    const int count = (int)((t - repeat_delay) / repeat_rate) - (int)((t_prev - repeat_delay) / repeat_rate);
    return (count > 0) ? count : 0;
}

int ImGui::GetKeyPressedAmount(int key_index, float repeat_delay, float repeat_rate)
{
    ImGuiContext& g = *GImGui;
    if (key_index < 0) return false;
    IM_ASSERT(key_index >= 0 && key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    const float t = g.IO.KeysDownDuration[key_index];
    return CalcTypematicPressedRepeatAmount(t, t - g.IO.DeltaTime, repeat_delay, repeat_rate);
}

bool ImGui::IsKeyPressed(int user_key_index, bool repeat)
{
    ImGuiContext& g = *GImGui;
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    const float t = g.IO.KeysDownDuration[user_key_index];
    if (t == 0.0f)
        return true;
    if (repeat && t > g.IO.KeyRepeatDelay)
        return GetKeyPressedAmount(user_key_index, g.IO.KeyRepeatDelay, g.IO.KeyRepeatRate) > 0;
    return false;
}

bool ImGui::IsKeyReleased(int user_key_index)
{
    ImGuiContext& g = *GImGui;
    if (user_key_index < 0) return false;
    IM_ASSERT(user_key_index >= 0 && user_key_index < IM_ARRAYSIZE(g.IO.KeysDown));
    if (g.IO.KeysDownDurationPrev[user_key_index] >= 0.0f && !g.IO.KeysDown[user_key_index])
        return true;
    return false;
}

bool ImGui::IsMouseDown(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDown[button];
}

bool ImGui::IsMouseClicked(int button, bool repeat)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    const float t = g.IO.MouseDownDuration[button];
    if (t == 0.0f)
        return true;

    if (repeat && t > g.IO.KeyRepeatDelay)
    {
        float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
        if ((fmodf(t - delay, rate) > rate*0.5f) != (fmodf(t - delay - g.IO.DeltaTime, rate) > rate*0.5f))
            return true;
    }

    return false;
}

bool ImGui::IsMouseReleased(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseReleased[button];
}

bool ImGui::IsMouseDoubleClicked(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    return g.IO.MouseDoubleClicked[button];
}

bool ImGui::IsMouseDragging(int button, float lock_threshold)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    if (!g.IO.MouseDown[button])
        return false;
    if (lock_threshold < 0.0f)
        lock_threshold = g.IO.MouseDragThreshold;
    return g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold;
}

ImVec2 ImGui::GetMousePos()
{
    return GImGui->IO.MousePos;
}

// NB: prefer to call right after BeginPopup(). At the time Selectable/MenuItem is activated, the popup is already closed!
ImVec2 ImGui::GetMousePosOnOpeningCurrentPopup()
{
    ImGuiContext& g = *GImGui;
    if (g.CurrentPopupStack.Size > 0)
        return g.OpenPopupStack[g.CurrentPopupStack.Size-1].MousePosOnOpen;
    return g.IO.MousePos;
}

// We typically use ImVec2(-FLT_MAX,-FLT_MAX) to denote an invalid mouse position
bool ImGui::IsMousePosValid(const ImVec2* mouse_pos)
{
    if (mouse_pos == NULL)
        mouse_pos = &GImGui->IO.MousePos;
    const float MOUSE_INVALID = -256000.0f;
    return mouse_pos->x >= MOUSE_INVALID && mouse_pos->y >= MOUSE_INVALID;
}

ImVec2 ImGui::GetMouseDragDelta(int button, float lock_threshold)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    if (lock_threshold < 0.0f)
        lock_threshold = g.IO.MouseDragThreshold;
    if (g.IO.MouseDown[button])
        if (g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold)
            return g.IO.MousePos - g.IO.MouseClickedPos[button];     // Assume we can only get active with left-mouse button (at the moment).
    return ImVec2(0.0f, 0.0f);
}

void ImGui::ResetMouseDragDelta(int button)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(button >= 0 && button < IM_ARRAYSIZE(g.IO.MouseDown));
    // NB: We don't need to reset g.IO.MouseDragMaxDistanceSqr
    g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

ImGuiMouseCursor ImGui::GetMouseCursor()
{
    return GImGui->MouseCursor;
}

void ImGui::SetMouseCursor(ImGuiMouseCursor cursor_type)
{
    GImGui->MouseCursor = cursor_type;
}

void ImGui::CaptureKeyboardFromApp(bool capture)
{
    GImGui->WantCaptureKeyboardNextFrame = capture ? 1 : 0;
}

void ImGui::CaptureMouseFromApp(bool capture)
{
    GImGui->WantCaptureMouseNextFrame = capture ? 1 : 0;
}

bool ImGui::IsItemActive()
{
    ImGuiContext& g = *GImGui;
    if (g.ActiveId)
    {
        ImGuiWindow* window = g.CurrentWindow;
        return g.ActiveId == window->DC.LastItemId;
    }
    return false;
}

bool ImGui::IsItemClicked(int mouse_button)
{
    return IsMouseClicked(mouse_button) && IsItemHovered();
}

bool ImGui::IsAnyItemHovered()
{
    return GImGui->HoveredId != 0 || GImGui->HoveredIdPreviousFrame != 0;
}

bool ImGui::IsAnyItemActive()
{
    return GImGui->ActiveId != 0;
}

bool ImGui::IsItemVisible()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ClipRect.Overlaps(window->DC.LastItemRect);
}

// Allow last item to be overlapped by a subsequent item. Both may be activated during the same frame before the later one takes priority.
void ImGui::SetItemAllowOverlap()
{
    ImGuiContext& g = *GImGui;
    if (g.HoveredId == g.CurrentWindow->DC.LastItemId)
        g.HoveredIdAllowOverlap = true;
    if (g.ActiveId == g.CurrentWindow->DC.LastItemId)
        g.ActiveIdAllowOverlap = true;
}

ImVec2 ImGui::GetItemRectMin()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRect.Min;
}

ImVec2 ImGui::GetItemRectMax()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRect.Max;
}

ImVec2 ImGui::GetItemRectSize()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.LastItemRect.GetSize();
}

ImVec2 ImGui::CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge, float outward)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImRect rect = window->DC.LastItemRect;
    rect.Expand(outward);
    return rect.GetClosestPoint(pos, on_edge);
}

static ImRect GetVisibleRect()
{
    ImGuiContext& g = *GImGui;
    if (g.IO.DisplayVisibleMin.x != g.IO.DisplayVisibleMax.x && g.IO.DisplayVisibleMin.y != g.IO.DisplayVisibleMax.y)
        return ImRect(g.IO.DisplayVisibleMin, g.IO.DisplayVisibleMax);
    return ImRect(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
}

// Not exposed publicly as BeginTooltip() because bool parameters are evil. Let's see if other needs arise first.
static void BeginTooltipEx(ImGuiWindowFlags extra_flags, bool override_previous_tooltip)
{
    ImGuiContext& g = *GImGui;
    char window_name[16];
    ImFormatString(window_name, IM_ARRAYSIZE(window_name), "##Tooltip%02d", g.TooltipOverrideCount);
    if (override_previous_tooltip)
        if (ImGuiWindow* window = ImGui::FindWindowByName(window_name))
            if (window->Active)
            {
                // Hide previous tooltips. We can't easily "reset" the content of a window so we create a new one.
                window->HiddenFrames = 1;
                ImFormatString(window_name, IM_ARRAYSIZE(window_name), "##Tooltip%02d", ++g.TooltipOverrideCount);
            }
    ImGuiWindowFlags flags = ImGuiWindowFlags_Tooltip|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin(window_name, NULL, flags | extra_flags);
}

void ImGui::SetTooltipV(const char* fmt, va_list args)
{
    BeginTooltipEx(0, true);
    TextV(fmt, args);
    EndTooltip();
}

void ImGui::SetTooltip(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    SetTooltipV(fmt, args);
    va_end(args);
}

void ImGui::BeginTooltip()
{
    BeginTooltipEx(0, false);
}

void ImGui::EndTooltip()
{
    IM_ASSERT(GetCurrentWindowRead()->Flags & ImGuiWindowFlags_Tooltip);   // Mismatched BeginTooltip()/EndTooltip() calls
    ImGui::End();
}

// Mark popup as open (toggle toward open state).
// Popups are closed when user click outside, or activate a pressable item, or CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block.
// Popup identifiers are relative to the current ID-stack (so OpenPopup and BeginPopup needs to be at the same level).
// One open popup per level of the popup hierarchy (NB: when assigning we reset the Window member of ImGuiPopupRef to NULL)
void ImGui::OpenPopupEx(ImGuiID id, bool reopen_existing)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    int current_stack_size = g.CurrentPopupStack.Size;
    ImGuiPopupRef popup_ref = ImGuiPopupRef(id, window, window->GetID("##menus"), g.IO.MousePos); // Tagged as new ref because constructor sets Window to NULL (we are passing the ParentWindow info here)
    if (g.OpenPopupStack.Size < current_stack_size + 1)
        g.OpenPopupStack.push_back(popup_ref);
    else if (reopen_existing || g.OpenPopupStack[current_stack_size].PopupId != id)
    {
        g.OpenPopupStack.resize(current_stack_size+1);
        g.OpenPopupStack[current_stack_size] = popup_ref;
    }
}

void ImGui::OpenPopup(const char* str_id)
{
    ImGuiContext& g = *GImGui;
    OpenPopupEx(g.CurrentWindow->GetID(str_id), false);
}

static void CloseInactivePopups()
{
    ImGuiContext& g = *GImGui;
    if (g.OpenPopupStack.empty())
        return;

    // When popups are stacked, clicking on a lower level popups puts focus back to it and close popups above it.
    // Don't close our own child popup windows
    int n = 0;
    if (g.NavWindow)
    {
        for (n = 0; n < g.OpenPopupStack.Size; n++)
        {
            ImGuiPopupRef& popup = g.OpenPopupStack[n];
            if (!popup.Window)
                continue;
            IM_ASSERT((popup.Window->Flags & ImGuiWindowFlags_Popup) != 0);
            if (popup.Window->Flags & ImGuiWindowFlags_ChildWindow)
                continue;

            bool has_focus = false;
            for (int m = n; m < g.OpenPopupStack.Size && !has_focus; m++)
                has_focus = (g.OpenPopupStack[m].Window && g.OpenPopupStack[m].Window->RootWindow == g.NavWindow->RootWindow);
            if (!has_focus)
                break;
        }
    }
    if (n < g.OpenPopupStack.Size)   // This test is not required but it allows to set a useful breakpoint on the line below
        g.OpenPopupStack.resize(n);
}

static ImGuiWindow* GetFrontMostModalRootWindow()
{
    ImGuiContext& g = *GImGui;
    for (int n = g.OpenPopupStack.Size-1; n >= 0; n--)
        if (ImGuiWindow* front_most_popup = g.OpenPopupStack.Data[n].Window)
            if (front_most_popup->Flags & ImGuiWindowFlags_Modal)
                return front_most_popup;
    return NULL;
}

static void ClosePopupToLevel(int remaining)
{
    ImGuiContext& g = *GImGui;
    if (remaining > 0)
        ImGui::FocusWindow(g.OpenPopupStack[remaining-1].Window);
    else
        ImGui::FocusWindow(g.OpenPopupStack[0].ParentWindow);
    g.OpenPopupStack.resize(remaining);
}

void ImGui::ClosePopup(ImGuiID id)
{
    if (!IsPopupOpen(id))
        return;
    ImGuiContext& g = *GImGui;
    ClosePopupToLevel(g.OpenPopupStack.Size - 1);
}

// Close the popup we have begin-ed into.
void ImGui::CloseCurrentPopup()
{
    ImGuiContext& g = *GImGui;
    int popup_idx = g.CurrentPopupStack.Size - 1;
    if (popup_idx < 0 || popup_idx >= g.OpenPopupStack.Size || g.CurrentPopupStack[popup_idx].PopupId != g.OpenPopupStack[popup_idx].PopupId)
        return;
    while (popup_idx > 0 && g.OpenPopupStack[popup_idx].Window && (g.OpenPopupStack[popup_idx].Window->Flags & ImGuiWindowFlags_ChildMenu))
        popup_idx--;
    ClosePopupToLevel(popup_idx);
}

static inline void ClearSetNextWindowData()
{
    // FIXME-OPT
    ImGuiContext& g = *GImGui;
    g.SetNextWindowPosCond = g.SetNextWindowSizeCond = g.SetNextWindowContentSizeCond = g.SetNextWindowCollapsedCond = 0;
    g.SetNextWindowSizeConstraint = g.SetNextWindowFocus = false;
}

bool ImGui::BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (!IsPopupOpen(id))
    {
        ClearSetNextWindowData(); // We behave like Begin() and need to consume those values
        return false;
    }

    PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGuiWindowFlags flags = extra_flags|ImGuiWindowFlags_Popup|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings;

    char name[20];
    if (flags & ImGuiWindowFlags_ChildMenu)
        ImFormatString(name, IM_ARRAYSIZE(name), "##menu_%d", g.CurrentPopupStack.Size);    // Recycle windows based on depth
    else
        ImFormatString(name, IM_ARRAYSIZE(name), "##popup_%08x", id); // Not recycling, so we can close/open during the same frame

    bool is_open = Begin(name, NULL, flags);
    if (!(window->Flags & ImGuiWindowFlags_ShowBorders))
        g.CurrentWindow->Flags &= ~ImGuiWindowFlags_ShowBorders;
    if (!is_open) // NB: is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
        EndPopup();

    return is_open;
}

bool ImGui::BeginPopup(const char* str_id, int Flags)
{
    ImGuiContext& g = *GImGui;
    if (g.OpenPopupStack.Size <= g.CurrentPopupStack.Size) // Early out for performance
    {
        ClearSetNextWindowData(); // We behave like Begin() and need to consume those values
        return false;
    }
    return BeginPopupEx(g.CurrentWindow->GetID(str_id), Flags ? Flags : ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_AlwaysAutoResize);
}

bool ImGui::IsPopupOpen(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == id;
}

bool ImGui::IsPopupOpen(const char* str_id)
{
    ImGuiContext& g = *GImGui;
    return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == g.CurrentWindow->GetID(str_id);
}

bool ImGui::BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const ImGuiID id = window->GetID(name);
    if (!IsPopupOpen(id))
    {
        ClearSetNextWindowData(); // We behave like Begin() and need to consume those values
        return false;
    }

    // Center modal windows by default
    if ((window->SetWindowPosAllowFlags & g.SetNextWindowPosCond) == 0)
        SetNextWindowPos(g.IO.DisplaySize * 0.5f, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags flags = extra_flags|ImGuiWindowFlags_Popup|ImGuiWindowFlags_Modal|ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoSavedSettings;
    bool is_open = Begin(name, p_open, flags);
    if (!is_open || (p_open && !*p_open)) // NB: is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
    {
        EndPopup();
        if (is_open)
            ClosePopup(id);
        return false;
    }

    return is_open;
}

void ImGui::EndPopup()
{
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(window->Flags & ImGuiWindowFlags_Popup);  // Mismatched BeginPopup()/EndPopup() calls
    IM_ASSERT(GImGui->CurrentPopupStack.Size > 0);
    End();
    if (!(window->Flags & ImGuiWindowFlags_Modal))
        PopStyleVar();
}

// This is a helper to handle the simplest case of associating one named popup to one given widget.
// 1. If you have many possible popups (for different "instances" of a same widget, or for wholly different widgets), you may be better off handling
//    this yourself so you can store data relative to the widget that opened the popup instead of choosing different popup identifiers.
// 2. If you want right-clicking on the same item to reopen the popup at new location, use the same code replacing IsItemHovered() with IsItemRectHovered()
//    and passing true to the OpenPopupEx().
//    This is because hovering an item in a window below the popup won't work. IsItemRectHovered() skips this test.
//    The pattern of ignoring the fact that the item can be interacted with (because it is blocked by the active popup) may useful in some situation 
//    when e.g. large canvas where the content of menu driven by click position.
bool ImGui::BeginPopupContextItem(const char* str_id, int mouse_button)
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    ImGuiID id = str_id ? window->GetID(str_id) : window->DC.LastItemId; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
    IM_ASSERT(id != 0);                                                  // However, you cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
    if (IsItemHovered() && IsMouseClicked(mouse_button))
        OpenPopupEx(id, false);
    return BeginPopupEx(id, 0);
}

bool ImGui::BeginPopupContextWindow(const char* str_id, int mouse_button, bool also_over_items)
{
    if (!str_id)
        str_id = "window_context";
    ImGuiID id = GImGui->CurrentWindow->GetID(str_id);
    if (IsWindowRectHovered() && IsMouseClicked(mouse_button))
        if (also_over_items || !IsAnyItemHovered())
            OpenPopupEx(id, true);
    return BeginPopupEx(id, 0);
}

bool ImGui::BeginPopupContextVoid(const char* str_id, int mouse_button)
{
    if (!str_id) 
        str_id = "void_context";
    ImGuiID id = GImGui->CurrentWindow->GetID(str_id);
    if (!IsAnyWindowHovered() && IsMouseClicked(mouse_button))
        OpenPopupEx(id, true);
    return BeginPopupEx(id, 0);
}

static bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_ChildWindow;

    const ImVec2 content_avail = ImGui::GetContentRegionAvail();
    ImVec2 size = ImFloor(size_arg);
    const int auto_fit_axises = ((size.x == 0.0f) ? 0x01 : 0x00) | ((size.y == 0.0f) ? 0x02 : 0x00);
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x, 4.0f) - fabsf(size.x); // Arbitrary minimum zero-ish child size of 4.0f (0.0f causing too much issues)
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y, 4.0f) - fabsf(size.y);
    if (border)
        flags |= ImGuiWindowFlags_ShowBorders;
    flags |= extra_flags;

    char title[256];
    if (name)
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/%s_%08X", parent_window->Name, name, id);
    else
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/%08X", parent_window->Name, id);

    ImGui::SetNextWindowSize(size);
    bool ret = ImGui::Begin(title, NULL, flags);
    ImGuiWindow* child_window = ImGui::GetCurrentWindow();
    child_window->AutoFitChildAxises = auto_fit_axises;
    if (!(parent_window->Flags & ImGuiWindowFlags_ShowBorders))
        child_window->Flags &= ~ImGuiWindowFlags_ShowBorders;

    return ret;
}

bool ImGui::BeginChild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    return BeginChildEx(str_id, window->GetID(str_id), size_arg, border, extra_flags);
}

bool ImGui::BeginChild(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    return BeginChildEx(NULL, id, size_arg, border, extra_flags);
}

void ImGui::EndChild()
{
    ImGuiWindow* window = GetCurrentWindow();

    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() callss
    if ((window->Flags & ImGuiWindowFlags_ComboBox) || window->BeginCount > 1)
    {
        ImGui::End();
    }
    else
    {
        // When using auto-filling child window, we don't provide full width/height to ItemSize so that it doesn't feed back into automatic size-fitting.
        ImVec2 sz = GetWindowSize();
        if (window->AutoFitChildAxises & 0x01) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
            sz.x = ImMax(4.0f, sz.x);
        if (window->AutoFitChildAxises & 0x02)
            sz.y = ImMax(4.0f, sz.y);
        ImGui::End();

        ImGuiWindow* parent_window = GetCurrentWindow();
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
        ItemSize(sz);
        ItemAdd(bb, 0);
    }
}

// Helper to create a child window / scrolling region that looks like a normal widget frame.
bool ImGui::BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_FrameBg]);
    PushStyleVar(ImGuiStyleVar_ChildWindowRounding, style.FrameRounding);
    PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
    return BeginChild(id, size, (g.CurrentWindow->Flags & ImGuiWindowFlags_ShowBorders) ? true : false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
}

void ImGui::EndChildFrame()
{
    EndChild();
    PopStyleVar(2);
    PopStyleColor();
}

// Save and compare stack sizes on Begin()/End() to detect usage errors
static void CheckStacksSize(ImGuiWindow* window, bool write)
{
    // NOT checking: DC.ItemWidth, DC.AllowKeyboardFocus, DC.ButtonRepeat, DC.TextWrapPos (per window) to allow user to conveniently push once and not pop (they are cleared on Begin)
    ImGuiContext& g = *GImGui;
    int* p_backup = &window->DC.StackSizesBackup[0];
    { int current = window->IDStack.Size;       if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushID/PopID or TreeNode/TreePop Mismatch!");   p_backup++; }    // Too few or too many PopID()/TreePop()
    { int current = window->DC.GroupStack.Size; if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "BeginGroup/EndGroup Mismatch!");                p_backup++; }    // Too few or too many EndGroup()
    { int current = g.CurrentPopupStack.Size;   if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "BeginMenu/EndMenu or BeginPopup/EndPopup Mismatch"); p_backup++;}// Too few or too many EndMenu()/EndPopup()
    { int current = g.ColorModifiers.Size;      if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushStyleColor/PopStyleColor Mismatch!");       p_backup++; }    // Too few or too many PopStyleColor()
    { int current = g.StyleModifiers.Size;      if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushStyleVar/PopStyleVar Mismatch!");           p_backup++; }    // Too few or too many PopStyleVar()
    { int current = g.FontStack.Size;           if (write) *p_backup = current; else IM_ASSERT(*p_backup == current && "PushFont/PopFont Mismatch!");                   p_backup++; }    // Too few or too many PopFont()
    IM_ASSERT(p_backup == window->DC.StackSizesBackup + IM_ARRAYSIZE(window->DC.StackSizesBackup));
}

static ImVec2 FindBestPopupWindowPos(const ImVec2& base_pos, const ImVec2& size, int* last_dir, const ImRect& r_inner)
{
    const ImGuiStyle& style = GImGui->Style;

    // Clamp into visible area while not overlapping the cursor. Safety padding is optional if our popup size won't fit without it.
    ImVec2 safe_padding = style.DisplaySafeAreaPadding;
    ImRect r_outer(GetVisibleRect());
    r_outer.Expand(ImVec2((size.x - r_outer.GetWidth() > safe_padding.x*2) ? -safe_padding.x : 0.0f, (size.y - r_outer.GetHeight() > safe_padding.y*2) ? -safe_padding.y : 0.0f));
    ImVec2 base_pos_clamped = ImClamp(base_pos, r_outer.Min, r_outer.Max - size);

    for (int n = (*last_dir != -1) ? -1 : 0; n < 4; n++)   // Last, Right, down, up, left. (Favor last used direction).
    {
        const int dir = (n == -1) ? *last_dir : n;
        ImRect rect(dir == 0 ? r_inner.Max.x : r_outer.Min.x, dir == 1 ? r_inner.Max.y : r_outer.Min.y, dir == 3 ? r_inner.Min.x : r_outer.Max.x, dir == 2 ? r_inner.Min.y : r_outer.Max.y);
        if (rect.GetWidth() < size.x || rect.GetHeight() < size.y)
            continue;
        *last_dir = dir;
        return ImVec2(dir == 0 ? r_inner.Max.x : dir == 3 ? r_inner.Min.x - size.x : base_pos_clamped.x, dir == 1 ? r_inner.Max.y : dir == 2 ? r_inner.Min.y - size.y : base_pos_clamped.y);
    }

    // Fallback, try to keep within display
    *last_dir = -1;
    ImVec2 pos = base_pos;
    pos.x = ImMax(ImMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
    pos.y = ImMax(ImMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
    return pos;
}

ImGuiWindow* ImGui::FindWindowByName(const char* name)
{
    // FIXME-OPT: Store sorted hashes -> pointers so we can do a bissection in a contiguous block
    ImGuiContext& g = *GImGui;
    ImGuiID id = ImHash(name, 0);
    for (int i = 0; i < g.Windows.Size; i++)
        if (g.Windows[i]->ID == id)
            return g.Windows[i];
    return NULL;
}

static ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;

    // Create window the first time
    ImGuiWindow* window = (ImGuiWindow*)ImGui::MemAlloc(sizeof(ImGuiWindow));
    IM_PLACEMENT_NEW(window) ImGuiWindow(name);
    window->Flags = flags;

    if (flags & ImGuiWindowFlags_NoSavedSettings)
    {
        // User can disable loading and saving of settings. Tooltip and child windows also don't store settings.
        window->Size = window->SizeFull = size;
    }
    else
    {
        // Retrieve settings from .ini file
        // Use SetWindowPos() or SetNextWindowPos() with the appropriate condition flag to change the initial position of a window.
        window->PosFloat = ImVec2(60, 60);
        window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

        ImGuiIniData* settings = FindWindowSettings(name);
        if (!settings)
        {
            settings = AddWindowSettings(name);
        }
        else
        {
            window->SetWindowPosAllowFlags &= ~ImGuiCond_FirstUseEver;
            window->SetWindowSizeAllowFlags &= ~ImGuiCond_FirstUseEver;
            window->SetWindowCollapsedAllowFlags &= ~ImGuiCond_FirstUseEver;
        }

        if (settings->Pos.x != FLT_MAX)
        {
            window->PosFloat = settings->Pos;
            window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
            window->Collapsed = settings->Collapsed;
        }

        if (ImLengthSqr(settings->Size) > 0.00001f)
            size = settings->Size;
        window->Size = window->SizeFull = size;
    }

    if ((flags & ImGuiWindowFlags_AlwaysAutoResize) != 0)
    {
        window->AutoFitFramesX = window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = false;
    }
    else
    {
        if (window->Size.x <= 0.0f)
            window->AutoFitFramesX = 2;
        if (window->Size.y <= 0.0f)
            window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = (window->AutoFitFramesX > 0) || (window->AutoFitFramesY > 0);
    }

    if (flags & ImGuiWindowFlags_NoBringToFrontOnFocus)
        g.Windows.insert(g.Windows.begin(), window); // Quite slow but rare and only once
    else
        g.Windows.push_back(window);
    return window;
}

static ImVec2 CalcSizeFullWithConstraint(ImGuiWindow* window, ImVec2 new_size)
{
    ImGuiContext& g = *GImGui;
    if (g.SetNextWindowSizeConstraint)
    {
        // Using -1,-1 on either X/Y axis to preserve the current size.
        ImRect cr = g.SetNextWindowSizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
        if (g.SetNextWindowSizeConstraintCallback)
        {
            ImGuiSizeConstraintCallbackData data;
            data.UserData = g.SetNextWindowSizeConstraintCallbackUserData;
            data.Pos = window->Pos;
            data.CurrentSize = window->SizeFull;
            data.DesiredSize = new_size;
            g.SetNextWindowSizeConstraintCallback(&data);
            new_size = data.DesiredSize;
        }
    }
    if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
        new_size = ImMax(new_size, g.Style.WindowMinSize);
    return new_size;
}

static ImVec2 CalcSizeAutoFit(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    ImGuiWindowFlags flags = window->Flags;
    ImVec2 size_auto_fit;
    if ((flags & ImGuiWindowFlags_Tooltip) != 0)
    {
        // Tooltip always resize. We keep the spacing symmetric on both axises for aesthetic purpose.
        size_auto_fit = window->SizeContents + window->WindowPadding - ImVec2(0.0f, style.ItemSpacing.y);
    }
    else
    {
        // Handling case of auto fit window not fitting on the screen (on either axis): we are growing the size on the other axis to compensate for expected scrollbar. FIXME: Might turn bigger than DisplaySize-WindowPadding.
        size_auto_fit = ImClamp(window->SizeContents + window->WindowPadding, style.WindowMinSize, ImMax(style.WindowMinSize, g.IO.DisplaySize - g.Style.DisplaySafeAreaPadding));
        ImVec2 size_auto_fit_after_constraint = CalcSizeFullWithConstraint(window, size_auto_fit);
        if (size_auto_fit_after_constraint.x < window->SizeContents.x && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar))
            size_auto_fit.y += style.ScrollbarSize;
        if (size_auto_fit_after_constraint.y < window->SizeContents.y && !(flags & ImGuiWindowFlags_NoScrollbar))
            size_auto_fit.x += style.ScrollbarSize * 2.0f;
        size_auto_fit.y = ImMax(size_auto_fit.y - style.ItemSpacing.y, 0.0f);
    }
    return size_auto_fit;
}

static ImVec2 CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window)
{
    ImVec2 scroll = window->Scroll;
    float cr_x = window->ScrollTargetCenterRatio.x;
    float cr_y = window->ScrollTargetCenterRatio.y;
    if (window->ScrollTarget.x < FLT_MAX)
        scroll.x = window->ScrollTarget.x - cr_x * (window->SizeFull.x - window->ScrollbarSizes.x);
    if (window->ScrollTarget.y < FLT_MAX)
        scroll.y = window->ScrollTarget.y - (1.0f - cr_y) * (window->TitleBarHeight() + window->MenuBarHeight()) - cr_y * (window->SizeFull.y - window->ScrollbarSizes.y);
    scroll = ImMax(scroll, ImVec2(0.0f, 0.0f));
    if (!window->Collapsed && !window->SkipItems)
    {
        scroll.x = ImMin(scroll.x, ImMax(0.0f, window->SizeContents.x - (window->SizeFull.x - window->ScrollbarSizes.x))); // == GetScrollMaxX for that window
        scroll.y = ImMin(scroll.y, ImMax(0.0f, window->SizeContents.y - (window->SizeFull.y - window->ScrollbarSizes.y))); // == GetScrollMaxY for that window
    }
    return scroll;
}

static ImGuiCol GetWindowBgColorIdxFromFlags(ImGuiWindowFlags flags)
{
    if (flags & ImGuiWindowFlags_ComboBox)
        return ImGuiCol_ComboBg;
    if (flags & (ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_Popup))
        return ImGuiCol_PopupBg;
    if (flags & ImGuiWindowFlags_ChildWindow)
        return ImGuiCol_ChildWindowBg;
    return ImGuiCol_WindowBg;
}

// Push a new ImGui window to add widgets to.
// - A default window called "Debug" is automatically stacked at the beginning of every frame so you can use widgets without explicitly calling a Begin/End pair.
// - Begin/End can be called multiple times during the frame with the same window name to append content.
// - The window name is used as a unique identifier to preserve window information across frames (and save rudimentary information to the .ini file).
//   You can use the "##" or "###" markers to use the same label with different id, or same id with different label. See documentation at the top of this file.
// - Return false when window is collapsed, so you can early out in your code. You always need to call ImGui::End() even if false is returned.
// - Passing 'bool* p_open' displays a Close button on the upper-right corner of the window, the pointed value will be set to false when the button is pressed.
bool ImGui::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    IM_ASSERT(name != NULL);                        // Window name required
    IM_ASSERT(g.Initialized);                       // Forgot to call ImGui::NewFrame()
    IM_ASSERT(g.FrameCountEnded != g.FrameCount);   // Called ImGui::Render() or ImGui::EndFrame() and haven't called ImGui::NewFrame() again yet

    if (flags & ImGuiWindowFlags_NoInputs)
        flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    // Find or create
    bool window_is_new = false;
    ImGuiWindow* window = FindWindowByName(name);
    if (!window)
    {
        ImVec2 size_on_first_use = (g.SetNextWindowSizeCond != 0) ? g.SetNextWindowSizeVal : ImVec2(0.0f, 0.0f); // Any condition flag will do since we are creating a new window here.
        window = CreateNewWindow(name, size_on_first_use, flags);
        window_is_new = true;
    }

    const int current_frame = g.FrameCount;
    const bool first_begin_of_the_frame = (window->LastFrameActive != current_frame);
    if (first_begin_of_the_frame)
        window->Flags = (ImGuiWindowFlags)flags;
    else
        flags = window->Flags;

    // Add to stack
    ImGuiWindow* parent_window = !g.CurrentWindowStack.empty() ? g.CurrentWindowStack.back() : NULL;
    g.CurrentWindowStack.push_back(window);
    SetCurrentWindow(window);
    CheckStacksSize(window, true);
    IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow));

    bool window_just_activated_by_user = (window->LastFrameActive < current_frame - 1);   // Not using !WasActive because the implicit "Debug" window would always toggle off->on
    if (flags & ImGuiWindowFlags_Popup)
    {
        ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
        window_just_activated_by_user |= (window->PopupId != popup_ref.PopupId); // We recycle popups so treat window as activated if popup id changed
        window_just_activated_by_user |= (window != popup_ref.Window);
        popup_ref.Window = window;
        g.CurrentPopupStack.push_back(popup_ref);
        window->PopupId = popup_ref.PopupId;
    }

    const bool window_just_appearing_after_hidden_for_resize = (window->HiddenFrames == 1);
    window->Appearing = (window_just_activated_by_user || window_just_appearing_after_hidden_for_resize);

    // Process SetNextWindow***() calls
    bool window_pos_set_by_api = false, window_size_set_by_api = false;
    if (g.SetNextWindowPosCond)
    {
        if (window->Appearing) 
            window->SetWindowPosAllowFlags |= ImGuiCond_Appearing;
        window_pos_set_by_api = (window->SetWindowPosAllowFlags & g.SetNextWindowPosCond) != 0;
        if (window_pos_set_by_api && ImLengthSqr(g.SetNextWindowPosPivot) > 0.00001f)
        {
            // May be processed on the next frame if this is our first frame and we are measuring size
            // FIXME: Look into removing the branch so everything can go through this same code path for consistency.
            window->SetWindowPosVal = g.SetNextWindowPosVal;
            window->SetWindowPosPivot = g.SetNextWindowPosPivot;
            window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
        }
        else
        {
            SetWindowPos(window, g.SetNextWindowPosVal, g.SetNextWindowPosCond);
        }
        g.SetNextWindowPosCond = 0;
    }
    if (g.SetNextWindowSizeCond)
    {
        if (window->Appearing) 
            window->SetWindowSizeAllowFlags |= ImGuiCond_Appearing;
        window_size_set_by_api = (window->SetWindowSizeAllowFlags & g.SetNextWindowSizeCond) != 0;
        SetWindowSize(window, g.SetNextWindowSizeVal, g.SetNextWindowSizeCond);
        g.SetNextWindowSizeCond = 0;
    }
    if (g.SetNextWindowContentSizeCond)
    {
        window->SizeContentsExplicit = g.SetNextWindowContentSizeVal;
        g.SetNextWindowContentSizeCond = 0;
    }
    else if (first_begin_of_the_frame)
    {
        window->SizeContentsExplicit = ImVec2(0.0f, 0.0f);
    }
    if (g.SetNextWindowCollapsedCond)
    {
        if (window->Appearing)
            window->SetWindowCollapsedAllowFlags |= ImGuiCond_Appearing;
        SetWindowCollapsed(window, g.SetNextWindowCollapsedVal, g.SetNextWindowCollapsedCond);
        g.SetNextWindowCollapsedCond = 0;
    }
    if (g.SetNextWindowFocus)
    {
        SetWindowFocus();
        g.SetNextWindowFocus = false;
    }

    // Update known root window (if we are a child window, otherwise window == window->RootWindow)
    int root_idx, root_non_popup_idx;
    for (root_idx = g.CurrentWindowStack.Size - 1; root_idx > 0; root_idx--)
        if (!(g.CurrentWindowStack[root_idx]->Flags & ImGuiWindowFlags_ChildWindow))
            break;
    for (root_non_popup_idx = root_idx; root_non_popup_idx > 0; root_non_popup_idx--)
        if (!(g.CurrentWindowStack[root_non_popup_idx]->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)) || (g.CurrentWindowStack[root_non_popup_idx]->Flags & ImGuiWindowFlags_Modal))
            break;
    window->ParentWindow = parent_window;
    window->RootWindow = g.CurrentWindowStack[root_idx];
    window->RootNonPopupWindow = g.CurrentWindowStack[root_non_popup_idx];      // Used to display TitleBgActive color and for selecting which window to use for NavWindowing

    // When reusing window again multiple times a frame, just append content (don't need to setup again)
    if (first_begin_of_the_frame)
    {
        window->Active = true;
        window->OrderWithinParent = 0;
        window->BeginCount = 0;
        window->ClipRect = ImVec4(-FLT_MAX,-FLT_MAX,+FLT_MAX,+FLT_MAX);
        window->LastFrameActive = current_frame;
        window->IDStack.resize(1);

        // Clear draw list, setup texture, outer clipping rectangle
        window->DrawList->Clear();
        window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);
        ImRect fullscreen_rect(GetVisibleRect());
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_ComboBox|ImGuiWindowFlags_Popup)))
            PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max, true);
        else
            PushClipRect(fullscreen_rect.Min, fullscreen_rect.Max, true);

        if (window_just_activated_by_user)
        {
            // Popup first latch mouse position, will position itself when it appears next frame
            window->AutoPosLastDirection = -1;
            if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
                window->PosFloat = g.IO.MousePos;
        }

        // Collapse window by double-clicking on title bar
        // At this point we don't have a clipping rectangle setup yet, so we can use the title bar area for hit detection and drawing
        if (!(flags & ImGuiWindowFlags_NoTitleBar) && !(flags & ImGuiWindowFlags_NoCollapse))
        {
            ImRect title_bar_rect = window->TitleBarRect();
            if (g.HoveredWindow == window && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) && g.IO.MouseDoubleClicked[0])
            {
                window->Collapsed = !window->Collapsed;
                MarkIniSettingsDirty(window);
                FocusWindow(window);
            }
        }
        else
        {
            window->Collapsed = false;
        }

        // SIZE

        // Save contents size from last frame for auto-fitting (unless explicitly specified)
        window->SizeContents.x = (float)(int)((window->SizeContentsExplicit.x != 0.0f) ? window->SizeContentsExplicit.x : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.x - window->Pos.x) + window->Scroll.x));
        window->SizeContents.y = (float)(int)((window->SizeContentsExplicit.y != 0.0f) ? window->SizeContentsExplicit.y : ((window_is_new ? 0.0f : window->DC.CursorMaxPos.y - window->Pos.y) + window->Scroll.y));

        // Hide popup/tooltip window when first appearing while we measure size (because we recycle them)
        if (window->HiddenFrames > 0)
            window->HiddenFrames--;
        if ((flags & (ImGuiWindowFlags_Popup | ImGuiWindowFlags_Tooltip)) != 0 && window_just_activated_by_user)
        {
            window->HiddenFrames = 1;
            if (flags & ImGuiWindowFlags_AlwaysAutoResize)
            {
                if (!window_size_set_by_api)
                    window->Size = window->SizeFull = ImVec2(0.f, 0.f);
                window->SizeContents = ImVec2(0.f, 0.f);
            }
        }

        // Lock window padding so that altering the ShowBorders flag for children doesn't have side-effects.
        window->WindowPadding = ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_ComboBox | ImGuiWindowFlags_Popup))) ? ImVec2(0,0) : style.WindowPadding;

        // Calculate auto-fit size, handle automatic resize
        const ImVec2 size_auto_fit = CalcSizeAutoFit(window);
        if (window->Collapsed)
        {
            // We still process initial auto-fit on collapsed windows to get a window width,
            // But otherwise we don't honor ImGuiWindowFlags_AlwaysAutoResize when collapsed.
            if (window->AutoFitFramesX > 0)
                window->SizeFull.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
            if (window->AutoFitFramesY > 0)
                window->SizeFull.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
        }
        else if (!window_size_set_by_api)
        {
            if (flags & ImGuiWindowFlags_AlwaysAutoResize)
            {
                window->SizeFull = size_auto_fit;
            }
            else if (window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
            {
                // Auto-fit only grows during the first few frames
                if (window->AutoFitFramesX > 0)
                    window->SizeFull.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
                if (window->AutoFitFramesY > 0)
                    window->SizeFull.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
                MarkIniSettingsDirty(window);
            }
        }

        // Apply minimum/maximum window size constraints and final size
        window->SizeFull = CalcSizeFullWithConstraint(window, window->SizeFull);
        window->Size = window->Collapsed ? window->TitleBarRect().GetSize() : window->SizeFull;
        
        // POSITION

        // Position child window
        if (flags & ImGuiWindowFlags_ChildWindow)
        {
            window->OrderWithinParent = parent_window->DC.ChildWindows.Size;
            parent_window->DC.ChildWindows.push_back(window);
        }
        if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup))
        {
            IM_ASSERT(window_size_set_by_api); // Submitted by BeginChild()
            window->Pos = window->PosFloat = parent_window->DC.CursorPos;
            window->Size = window->SizeFull;
        }

        const bool window_pos_with_pivot = (window->SetWindowPosVal.x != FLT_MAX && window->HiddenFrames == 0);
        if (window_pos_with_pivot)
        {
            // Position given a pivot (e.g. for centering)
            SetWindowPos(window, ImMax(style.DisplaySafeAreaPadding, window->SetWindowPosVal - window->SizeFull * window->SetWindowPosPivot), 0);
        }
        else if (flags & ImGuiWindowFlags_ChildMenu)
        {
            // Child menus typically request _any_ position within the parent menu item, and then our FindBestPopupWindowPos() function will move the new menu outside the parent bounds.
            // This is how we end up with child menus appearing (most-commonly) on the right of the parent menu.
            IM_ASSERT(window_pos_set_by_api);
            float horizontal_overlap = style.ItemSpacing.x; // We want some overlap to convey the relative depth of each popup (currently the amount of overlap it is hard-coded to style.ItemSpacing.x, may need to introduce another style value).
            ImRect rect_to_avoid;
            if (parent_window->DC.MenuBarAppending)
                rect_to_avoid = ImRect(-FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight(), FLT_MAX, parent_window->Pos.y + parent_window->TitleBarHeight() + parent_window->MenuBarHeight());
            else
                rect_to_avoid = ImRect(parent_window->Pos.x + horizontal_overlap, -FLT_MAX, parent_window->Pos.x + parent_window->Size.x - horizontal_overlap - parent_window->ScrollbarSizes.x, FLT_MAX);
            window->PosFloat = FindBestPopupWindowPos(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
        }
        else if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api && window_just_appearing_after_hidden_for_resize)
        {
            ImRect rect_to_avoid(window->PosFloat.x - 1, window->PosFloat.y - 1, window->PosFloat.x + 1, window->PosFloat.y + 1);
            window->PosFloat = FindBestPopupWindowPos(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
        }

        // Position tooltip (always follows mouse)
        if ((flags & ImGuiWindowFlags_Tooltip) != 0 && !window_pos_set_by_api)
        {
            ImVec2 ref_pos = g.IO.MousePos;
            ImRect rect_to_avoid(ref_pos.x - 16, ref_pos.y - 8, ref_pos.x + 24, ref_pos.y + 24); // FIXME: Completely hard-coded. Perhaps center on cursor hit-point instead?
            window->PosFloat = FindBestPopupWindowPos(ref_pos, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
            if (window->AutoPosLastDirection == -1)
                window->PosFloat = ref_pos + ImVec2(2,2); // If there's not enough room, for tooltip we prefer avoiding the cursor at all cost even if it means that part of the tooltip won't be visible.
        }

        // Clamp position so it stays visible
        if (!(flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Tooltip))
        {
            if (!window_pos_set_by_api && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && g.IO.DisplaySize.x > 0.0f && g.IO.DisplaySize.y > 0.0f) // Ignore zero-sized display explicitly to avoid losing positions if a window manager reports zero-sized window when initializing or minimizing.
            {
                ImVec2 padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
                window->PosFloat = ImMax(window->PosFloat + window->Size, padding) - window->Size;
                window->PosFloat = ImMin(window->PosFloat, g.IO.DisplaySize - padding);
            }
        }
        window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);

        // Default item width. Make it proportional to window size if window manually resizes
        if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_Tooltip) && !(flags & ImGuiWindowFlags_AlwaysAutoResize))
            window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
        else
            window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);

        // Prepare for focus requests
        window->FocusIdxAllRequestCurrent = (window->FocusIdxAllRequestNext == INT_MAX || window->FocusIdxAllCounter == -1) ? INT_MAX : (window->FocusIdxAllRequestNext + (window->FocusIdxAllCounter+1)) % (window->FocusIdxAllCounter+1);
        window->FocusIdxTabRequestCurrent = (window->FocusIdxTabRequestNext == INT_MAX || window->FocusIdxTabCounter == -1) ? INT_MAX : (window->FocusIdxTabRequestNext + (window->FocusIdxTabCounter+1)) % (window->FocusIdxTabCounter+1);
        window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
        window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;

        // Apply scrolling
        window->Scroll = CalcNextScrollFromScrollTargetAndClamp(window);
        window->ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);

        // Modal window darkens what is behind them
        if ((flags & ImGuiWindowFlags_Modal) != 0 && window == GetFrontMostModalRootWindow())
            window->DrawList->AddRectFilled(fullscreen_rect.Min, fullscreen_rect.Max, GetColorU32(ImGuiCol_ModalWindowDarkening, g.ModalWindowDarkeningRatio));

        // Draw window + handle manual resize
        ImRect title_bar_rect = window->TitleBarRect();
        const float window_rounding = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildWindowRounding : style.WindowRounding;
        if (window->Collapsed)
        {
            // Title bar only
            RenderFrame(title_bar_rect.Min, title_bar_rect.Max, GetColorU32(ImGuiCol_TitleBgCollapsed), true, window_rounding);
        }
        else
        {
            ImU32 resize_col = 0;
            const float resize_corner_size = ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
            if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && !(flags & ImGuiWindowFlags_NoResize))
            {
                // Manual resize
                // Using the FlattenChilds button flag, we make the resize button accessible even if we are hovering over a child window
                const ImVec2 br = window->Rect().GetBR();
                const ImRect resize_rect(br - ImVec2(resize_corner_size * 0.75f, resize_corner_size * 0.75f), br);
                const ImGuiID resize_id = window->GetID("#RESIZE");
                bool hovered, held;
                ButtonBehavior(resize_rect, resize_id, &hovered, &held, ImGuiButtonFlags_FlattenChilds);
                resize_col = GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);
                if (hovered || held)
                    g.MouseCursor = ImGuiMouseCursor_ResizeNWSE;

                ImVec2 size_target(FLT_MAX,FLT_MAX);
                if (g.HoveredWindow == window && held && g.IO.MouseDoubleClicked[0])
                {
                    // Manual auto-fit when double-clicking
                    size_target = size_auto_fit;
                    ClearActiveID();
                }
                else if (held)
                {
                    // We don't use an incremental MouseDelta but rather compute an absolute target size based on mouse position
                    size_target = (g.IO.MousePos - g.ActiveIdClickOffset + resize_rect.GetSize()) - window->Pos;
                }

                if (size_target.x != FLT_MAX && size_target.y != FLT_MAX)
                {
                    window->SizeFull = CalcSizeFullWithConstraint(window, size_target);
                    MarkIniSettingsDirty(window);
                }
                window->Size = window->SizeFull;
                title_bar_rect = window->TitleBarRect();
            }

            // Scrollbars
            window->ScrollbarY = (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) || ((window->SizeContents.y > window->Size.y + style.ItemSpacing.y) && !(flags & ImGuiWindowFlags_NoScrollbar));
            window->ScrollbarX = (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) || ((window->SizeContents.x > window->Size.x - (window->ScrollbarY ? style.ScrollbarSize : 0.0f) - window->WindowPadding.x) && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar));
            if (window->ScrollbarX && !window->ScrollbarY)
                window->ScrollbarY = (window->SizeContents.y > window->Size.y + style.ItemSpacing.y - style.ScrollbarSize) && !(flags & ImGuiWindowFlags_NoScrollbar);
            window->ScrollbarSizes = ImVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f, window->ScrollbarX ? style.ScrollbarSize : 0.0f);
            window->BorderSize = (flags & ImGuiWindowFlags_ShowBorders) ? 1.0f : 0.0f;

            // Window background, Default Alpha
            ImU32 bg_col = GetColorU32(GetWindowBgColorIdxFromFlags(flags));
            window->DrawList->AddRectFilled(window->Pos+ImVec2(0,window->TitleBarHeight()), window->Pos+window->Size, bg_col, ((style.WindowPadThickness > 0.f && window->RootWindow == window) ? 0.f : window_rounding), (flags & ImGuiWindowFlags_NoTitleBar) ? ImGuiCorner_All : ImGuiCorner_BotLeft|ImGuiCorner_BotRight);

			// Title bar
			const bool is_focused = g.NavWindow && window->RootNonPopupWindow == g.NavWindow->RootNonPopupWindow;
			ImRect leftbar(ImVec2(title_bar_rect.GetBL() - ImVec2(style.WindowPadThickness, 0)), ImVec2(title_bar_rect.GetBL().x, window->Pos.y + window->Size.y));
			ImRect rightbar(title_bar_rect.GetBR(), ImVec2(title_bar_rect.GetBR().x + style.WindowPadThickness, window->Pos.y + window->Size.y));
			ImRect bottombar(ImVec2(leftbar.Min.x, window->Pos.y + window->Size.y), ImVec2(rightbar.Max.x, window->Pos.y + window->Size.y + style.WindowPadThickness));
			if (window->RootWindow == window)
			{
				if (!(flags & ImGuiWindowFlags_NoTitleBar))
				{
					window->DrawList->AddRectFilled(title_bar_rect.GetTL() - ImVec2(style.WindowPadThickness, 0), title_bar_rect.GetBR() + ImVec2(style.WindowPadThickness, 0), GetColorU32(is_focused ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg), window_rounding, ImGuiCorner_TopLeft | ImGuiCorner_TopRight);
					window->DrawList->AddRectFilled(leftbar.Min, leftbar.Max, GetColorU32(is_focused ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg));
					window->DrawList->AddRectFilled(rightbar.Min, rightbar.Max, GetColorU32(is_focused ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg));
					window->DrawList->AddRectFilled(bottombar.Min, bottombar.Max, GetColorU32(is_focused ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg), window_rounding, ImGuiCorner_BotLeft | ImGuiCorner_BotRight);
				}
			}

            // Menu bar
            if (flags & ImGuiWindowFlags_MenuBar)
            {
                ImRect menu_bar_rect = window->MenuBarRect();
                if (flags & ImGuiWindowFlags_ShowBorders)
                    window->DrawList->AddLine(menu_bar_rect.GetBL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_Border));
                window->DrawList->AddRectFilled(menu_bar_rect.GetTL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_MenuBarBg), (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f, ImGuiCorner_TopLeft|ImGuiCorner_TopRight);
            }

            // Scrollbars
            if (window->ScrollbarX)
                Scrollbar(ImGuiLayoutType_Horizontal);
            if (window->ScrollbarY)
                Scrollbar(ImGuiLayoutType_Vertical);

            // Render resize grip
            // (after the input handling so we don't have a frame of latency)
            if (!(flags & ImGuiWindowFlags_NoResize))
            {
                const ImVec2 br = window->Rect().GetBR();
                window->DrawList->PathLineTo(br + ImVec2(-resize_corner_size, -window->BorderSize));
                window->DrawList->PathLineTo(br + ImVec2(-window->BorderSize, -resize_corner_size));
                window->DrawList->PathArcToFast(ImVec2(br.x - window_rounding - window->BorderSize, br.y - window_rounding - window->BorderSize), window_rounding, 0, 3);
                window->DrawList->PathFillConvex(resize_col);
            }

            // Borders
            if (flags & ImGuiWindowFlags_ShowBorders)
			{
				window->DrawList->AddRect(title_bar_rect.GetBL(), window->Pos + window->Size, GetColorU32(ImGuiCol_Border), ((style.WindowPadThickness > 0.f && window->RootWindow == window) ? 0.f : window_rounding));
				if (!(flags & ImGuiWindowFlags_NoTitleBar))
				{
					if (style.WindowPadThickness > 0.f && window->RootWindow == window) {
						window->DrawList->AddRect(window->Pos - ImVec2(style.WindowPadThickness, 0) + ImVec2(1, 1), bottombar.Max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), window_rounding);
						window->DrawList->AddRect(window->Pos - ImVec2(style.WindowPadThickness, 0), bottombar.Max, GetColorU32(ImGuiCol_Border), window_rounding);
					}
					else
						window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(1, 0), title_bar_rect.GetBR() - ImVec2(1, 0), GetColorU32(ImGuiCol_Border));
				}
            }
        }

        // Update ContentsRegionMax. All the variable it depends on are set above in this function.
        window->ContentsRegionRect.Min.x = -window->Scroll.x + window->WindowPadding.x;
        window->ContentsRegionRect.Min.y = -window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() + window->MenuBarHeight();
        window->ContentsRegionRect.Max.x = -window->Scroll.x - window->WindowPadding.x + (window->SizeContentsExplicit.x != 0.0f ? window->SizeContentsExplicit.x : (window->Size.x - window->ScrollbarSizes.x)); 
        window->ContentsRegionRect.Max.y = -window->Scroll.y - window->WindowPadding.y + (window->SizeContentsExplicit.y != 0.0f ? window->SizeContentsExplicit.y : (window->Size.y - window->ScrollbarSizes.y)); 

        // Setup drawing context
        window->DC.IndentX = 0.0f + window->WindowPadding.x - window->Scroll.x;
        window->DC.GroupOffsetX = 0.0f;
        window->DC.ColumnsOffsetX = 0.0f;
        window->DC.CursorStartPos = window->Pos + ImVec2(window->DC.IndentX + window->DC.ColumnsOffsetX, window->TitleBarHeight() + window->MenuBarHeight() + window->WindowPadding.y - window->Scroll.y);
        window->DC.CursorPos = window->DC.CursorStartPos;
        window->DC.CursorPosPrevLine = window->DC.CursorPos;
        window->DC.CursorMaxPos = window->DC.CursorStartPos;
        window->DC.CurrentLineHeight = window->DC.PrevLineHeight = 0.0f;
        window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset = 0.0f;
        window->DC.MenuBarAppending = false;
        window->DC.MenuBarOffsetX = ImMax(window->WindowPadding.x, style.ItemSpacing.x);
        window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
        window->DC.ChildWindows.resize(0);
        window->DC.LayoutType = ImGuiLayoutType_Vertical;
        window->DC.ItemFlags = ImGuiItemFlags_Default_;
        window->DC.ItemWidth = window->ItemWidthDefault;
        window->DC.TextWrapPos = -1.0f; // disabled
        window->DC.ItemFlagsStack.resize(0);
        window->DC.ItemWidthStack.resize(0);
        window->DC.TextWrapPosStack.resize(0);
        window->DC.ColumnsCurrent = 0;
        window->DC.ColumnsCount = 1;
        window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
        window->DC.ColumnsStartMaxPosX = window->DC.CursorMaxPos.x;
        window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY = window->DC.ColumnsStartPosY;
        window->DC.TreeDepth = 0;
        window->DC.StateStorage = &window->StateStorage;
        window->DC.GroupStack.resize(0);
        window->MenuColumns.Update(3, style.ItemSpacing.x, window_just_activated_by_user);

        if ((flags & ImGuiWindowFlags_ChildWindow) && (window->DC.ItemFlags != parent_window->DC.ItemFlags))
        {
            window->DC.ItemFlags = parent_window->DC.ItemFlags;
            window->DC.ItemFlagsStack.push_back(window->DC.ItemFlags);
        }

        if (window->AutoFitFramesX > 0)
            window->AutoFitFramesX--;
        if (window->AutoFitFramesY > 0)
            window->AutoFitFramesY--;

        // New windows appears in front (we need to do that AFTER setting DC.CursorStartPos so our initial navigation reference rectangle can start around there)
        if (window_just_activated_by_user && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
            if (!(flags & (ImGuiWindowFlags_ChildWindow|ImGuiWindowFlags_Tooltip)) || (flags & ImGuiWindowFlags_Popup))
                FocusWindow(window);

        // Title bar
        if (!(flags & ImGuiWindowFlags_NoTitleBar))
        {
            // Collapse button
            if (!(flags & ImGuiWindowFlags_NoCollapse))
            {
                RenderTriangle(window->Pos + style.FramePadding, window->Collapsed ? ImGuiDir_Right : ImGuiDir_Down, 1.0f);
            }

            // Close button
            /*if (p_open != NULL)
            {
                const float PAD = 2.0f;
                const float rad = (window->TitleBarHeight() - PAD*2.0f) * 0.5f;
                if (CloseButton(window->GetID("#CLOSE"), window->Rect().GetTR() + ImVec2(-PAD - rad, PAD + rad), rad))
                    *p_open = false;
            }*/

            // Title text (FIXME: refactor text alignment facilities along with RenderText helpers)
            const ImVec2 text_size = CalcTextSize(name, NULL, true);
            ImVec2 text_min = window->Pos;
            ImVec2 text_max = window->Pos + ImVec2(window->Size.x, style.FramePadding.y*2 + text_size.y);
            ImRect clip_rect;
            clip_rect.Max = ImVec2(window->Pos.x + window->Size.x - (p_open ? title_bar_rect.GetHeight() - 3 : style.FramePadding.x), text_max.y); // Match the size of CloseWindowButton()
            float pad_left = (flags & ImGuiWindowFlags_NoCollapse) == 0 ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x) : style.FramePadding.x;
            float pad_right = (p_open != NULL) ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x) : style.FramePadding.x;
            if (style.WindowTitleAlign.x > 0.0f) pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
            text_min.x += pad_left;
            text_max.x -= pad_right;
            clip_rect.Min = ImVec2(text_min.x, window->Pos.y);
            RenderTextClipped(text_min, text_max, name, NULL, &text_size, style.WindowTitleAlign, &clip_rect);
        }

        // Save clipped aabb so we can access it in constant-time in FindHoveredWindow()
        window->WindowRectClipped = window->Rect();
        window->WindowRectClipped.ClipWith(window->ClipRect);

        // Pressing CTRL+C while holding on a window copy its content to the clipboard
        // This works but 1. doesn't handle multiple Begin/End pairs, 2. recursing into another Begin/End pair - so we need to work that out and add better logging scope.
        // Maybe we can support CTRL+C on every element?
        /*
        if (g.ActiveId == move_id)
            if (g.IO.KeyCtrl && IsKeyPressedMap(ImGuiKey_C))
                ImGui::LogToClipboard();
        */
    }

    // Inner clipping rectangle
    // We set this up after processing the resize grip so that our clip rectangle doesn't lag by a frame
    // Note that if our window is collapsed we will end up with a null clipping rectangle which is the correct behavior.
    const ImRect title_bar_rect = window->TitleBarRect();
    const float border_size = window->BorderSize;
	// Force round to ensure that e.g. (int)(max.x-min.x) in user's render code produce correct result.
    ImRect clip_rect;
    clip_rect.Min.x = ImFloor(0.5f + title_bar_rect.Min.x + ImMax(border_size, ImFloor(window->WindowPadding.x*0.5f)));
    clip_rect.Min.y = ImFloor(0.5f + title_bar_rect.Max.y + window->MenuBarHeight() + border_size);
    clip_rect.Max.x = ImFloor(0.5f + window->Pos.x + window->Size.x - window->ScrollbarSizes.x - ImMax(border_size, ImFloor(window->WindowPadding.x*0.5f)));
    clip_rect.Max.y = ImFloor(0.5f + window->Pos.y + window->Size.y - window->ScrollbarSizes.y - border_size);
    PushClipRect(clip_rect.Min, clip_rect.Max, true);

    // Clear 'accessed' flag last thing
    if (first_begin_of_the_frame)
        window->Accessed = false;
    window->BeginCount++;
    g.SetNextWindowSizeConstraint = false;

    // Child window can be out of sight and have "negative" clip windows.
    // Mark them as collapsed so commands are skipped earlier (we can't manually collapse because they have no title bar).
    if (flags & ImGuiWindowFlags_ChildWindow)
    {
        IM_ASSERT((flags & ImGuiWindowFlags_NoTitleBar) != 0);
        window->Collapsed = parent_window && parent_window->Collapsed;

        if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
            window->Collapsed |= (window->WindowRectClipped.Min.x >= window->WindowRectClipped.Max.x || window->WindowRectClipped.Min.y >= window->WindowRectClipped.Max.y);

        // We also hide the window from rendering because we've already added its border to the command list.
        // (we could perform the check earlier in the function but it is simpler at this point)
        if (window->Collapsed)
            window->Active = false;
    }
    if (style.Alpha <= 0.0f)
        window->Active = false;

    // Return false if we don't intend to display anything to allow user to perform an early out optimization
    window->SkipItems = (window->Collapsed || !window->Active) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0;
    return !window->SkipItems;
}

// Old Begin() API with 5 parameters, avoid calling this version directly! Use SetNextWindowSize()+Begin() instead.
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
bool ImGui::Begin(const char* name, bool* p_open, const ImVec2& size_on_first_use, float bg_alpha_override, ImGuiWindowFlags flags)
{
    // Old API feature: we could pass the initial window size as a parameter, however this was very misleading because in most cases it would only affect the window when it didn't have storage in the .ini file.
    if (size_on_first_use.x != 0.0f || size_on_first_use.y != 0.0f)
        SetNextWindowSize(size_on_first_use, ImGuiCond_FirstUseEver);

    // Old API feature: we could override the window background alpha with a parameter. This is actually tricky to reproduce manually because: 
    // (1) there are multiple variants of WindowBg (popup, tooltip, etc.) and (2) you can't call PushStyleColor before Begin and PopStyleColor just after Begin() because of how CheckStackSizes() behave.
    // The user-side solution is to do backup = GetStyleColorVec4(ImGuiCol_xxxBG), PushStyleColor(ImGuiCol_xxxBg), Begin, PushStyleColor(ImGuiCol_xxxBg, backup), [...], PopStyleColor(), End(); PopStyleColor() - which is super awkward.
    // The alpha override was rarely used but for now we'll leave the Begin() variant around for a bit. We may either lift the constraint on CheckStackSizes() either add a SetNextWindowBgAlpha() helper that does it magically.
    ImGuiContext& g = *GImGui;
    const ImGuiCol bg_color_idx = GetWindowBgColorIdxFromFlags(flags);
    const ImVec4 bg_color_backup = g.Style.Colors[bg_color_idx];
    if (bg_alpha_override >= 0.0f)
        g.Style.Colors[bg_color_idx].w = bg_alpha_override;

    bool ret = Begin(name, p_open, flags);

    if (bg_alpha_override >= 0.0f)
        g.Style.Colors[bg_color_idx] = bg_color_backup;
    return ret;
}
#endif // IMGUI_DISABLE_OBSOLETE_FUNCTIONS

void ImGui::End()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (window->DC.ColumnsCount != 1) // close columns set if any is open
        EndColumns();
    PopClipRect();   // inner window clip rectangle

    // Stop logging
    if (!(window->Flags & ImGuiWindowFlags_ChildWindow))    // FIXME: add more options for scope of logging
        LogFinish();

    // Pop
    // NB: we don't clear 'window->RootWindow'. The pointer is allowed to live until the next call to Begin().
    g.CurrentWindowStack.pop_back();
    if (window->Flags & ImGuiWindowFlags_Popup)
        g.CurrentPopupStack.pop_back();
    CheckStacksSize(window, false);
    SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back());
}

// Vertical scrollbar
// The entire piece of code below is rather confusing because:
// - We handle absolute seeking (when first clicking outside the grab) and relative manipulation (afterward or when clicking inside the grab)
// - We store values as normalized ratio and in a form that allows the window content to change while we are holding on a scrollbar
// - We handle both horizontal and vertical scrollbars, which makes the terminology not ideal.
void ImGui::Scrollbar(ImGuiLayoutType direction)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    const bool horizontal = (direction == ImGuiLayoutType_Horizontal);
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(horizontal ? "#SCROLLX" : "#SCROLLY");

    // Render background
    bool other_scrollbar = (horizontal ? window->ScrollbarY : window->ScrollbarX);
    float other_scrollbar_size_w = other_scrollbar ? style.ScrollbarSize : 0.0f;
    const ImRect window_rect = window->Rect();
    const float border_size = window->BorderSize;
    ImRect bb = horizontal
        ? ImRect(window->Pos.x + border_size, window_rect.Max.y - style.ScrollbarSize, window_rect.Max.x - other_scrollbar_size_w - border_size, window_rect.Max.y - border_size)
        : ImRect(window_rect.Max.x - style.ScrollbarSize, window->Pos.y + border_size, window_rect.Max.x - border_size, window_rect.Max.y - other_scrollbar_size_w - border_size);
    if (!horizontal)
        bb.Min.y += window->TitleBarHeight() + ((window->Flags & ImGuiWindowFlags_MenuBar) ? window->MenuBarHeight() : 0.0f);
    if (bb.GetWidth() <= 0.0f || bb.GetHeight() <= 0.0f)
        return;

    float window_rounding = (window->Flags & ImGuiWindowFlags_ChildWindow) ? style.ChildWindowRounding : style.WindowRounding;
    int window_rounding_corners;
    if (horizontal)
        window_rounding_corners = ImGuiCorner_BotLeft | (other_scrollbar ? 0 : ImGuiCorner_BotRight);
    else
        window_rounding_corners = (((window->Flags & ImGuiWindowFlags_NoTitleBar) && !(window->Flags & ImGuiWindowFlags_MenuBar)) ? ImGuiCorner_TopRight : 0) | (other_scrollbar ? 0 : ImGuiCorner_BotRight);
    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(ImGuiCol_ScrollbarBg), window_rounding, window_rounding_corners);
    bb.Expand(ImVec2(-ImClamp((float)(int)((bb.Max.x - bb.Min.x - 2.0f) * 0.5f), 0.0f, 3.0f), -ImClamp((float)(int)((bb.Max.y - bb.Min.y - 2.0f) * 0.5f), 0.0f, 3.0f)));

    // V denote the main, longer axis of the scrollbar (= height for a vertical scrollbar)
    float scrollbar_size_v = horizontal ? bb.GetWidth() : bb.GetHeight();
    float scroll_v = horizontal ? window->Scroll.x : window->Scroll.y;
    float win_size_avail_v = (horizontal ? window->SizeFull.x : window->SizeFull.y) - other_scrollbar_size_w;
    float win_size_contents_v = horizontal ? window->SizeContents.x : window->SizeContents.y;

    // Calculate the height of our grabbable box. It generally represent the amount visible (vs the total scrollable amount)
    // But we maintain a minimum size in pixel to allow for the user to still aim inside.
    IM_ASSERT(ImMax(win_size_contents_v, win_size_avail_v) > 0.0f); // Adding this assert to check if the ImMax(XXX,1.0f) is still needed. PLEASE CONTACT ME if this triggers.
    const float win_size_v = ImMax(ImMax(win_size_contents_v, win_size_avail_v), 1.0f);
    const float grab_h_pixels = ImClamp(scrollbar_size_v * (win_size_avail_v / win_size_v), style.GrabMinSize, scrollbar_size_v);
    const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

    // Handle input right away. None of the code of Begin() is relying on scrolling position before calling Scrollbar().
    bool held = false;
    bool hovered = false;
    const bool previously_held = (g.ActiveId == id);
    ButtonBehavior(bb, id, &hovered, &held);

    float scroll_max = ImMax(1.0f, win_size_contents_v - win_size_avail_v);
    float scroll_ratio = ImSaturate(scroll_v / scroll_max);
    float grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
    if (held && grab_h_norm < 1.0f)
    {
        float scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
        float mouse_pos_v = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
        float* click_delta_to_grab_center_v = horizontal ? &g.ScrollbarClickDeltaToGrabCenter.x : &g.ScrollbarClickDeltaToGrabCenter.y;

        // Click position in scrollbar normalized space (0.0f->1.0f)
        const float clicked_v_norm = ImSaturate((mouse_pos_v - scrollbar_pos_v) / scrollbar_size_v);
        SetHoveredID(id);

        bool seek_absolute = false;
        if (!previously_held)
        {
            // On initial click calculate the distance between mouse and the center of the grab
            if (clicked_v_norm >= grab_v_norm && clicked_v_norm <= grab_v_norm + grab_h_norm)
            {
                *click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm*0.5f;
            }
            else
            {
                seek_absolute = true;
                *click_delta_to_grab_center_v = 0.0f;
            }
        }

        // Apply scroll
        // It is ok to modify Scroll here because we are being called in Begin() after the calculation of SizeContents and before setting up our starting position
        const float scroll_v_norm = ImSaturate((clicked_v_norm - *click_delta_to_grab_center_v - grab_h_norm*0.5f) / (1.0f - grab_h_norm));
        scroll_v = (float)(int)(0.5f + scroll_v_norm * scroll_max);//(win_size_contents_v - win_size_v));
        if (horizontal)
            window->Scroll.x = scroll_v;
        else
            window->Scroll.y = scroll_v;

        // Update values for rendering
        scroll_ratio = ImSaturate(scroll_v / scroll_max);
        grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

        // Update distance to grab now that we have seeked and saturated
        if (seek_absolute)
            *click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm*0.5f;
    }

    // Render
    const ImU32 grab_col = GetColorU32(held ? ImGuiCol_ScrollbarGrabActive : hovered ? ImGuiCol_ScrollbarGrabHovered : ImGuiCol_ScrollbarGrab);
    if (horizontal)
        window->DrawList->AddRectFilled(ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y), ImVec2(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels, bb.Max.y), grab_col, style.ScrollbarRounding);
    else
        window->DrawList->AddRectFilled(ImVec2(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm)), ImVec2(bb.Max.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels), grab_col, style.ScrollbarRounding);
}

// Moving window to front of display (which happens to be back of our sorted list)
void ImGui::FocusWindow(ImGuiWindow* window)
{
    ImGuiContext& g = *GImGui;

    // Always mark the window we passed as focused. This is used for keyboard interactions such as tabbing.
    g.NavWindow = window;

    // Passing NULL allow to disable keyboard focus
    if (!window)
        return;

    // And move its root window to the top of the pile
    if (window->RootWindow)
        window = window->RootWindow;

    // Steal focus on active widgets
    if (window->Flags & ImGuiWindowFlags_Popup) // FIXME: This statement should be unnecessary. Need further testing before removing it..
        if (g.ActiveId != 0 && g.ActiveIdWindow && g.ActiveIdWindow->RootWindow != window)
            ClearActiveID();

    // Bring to front
    if ((window->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus) || g.Windows.back() == window)
        return;
    for (int i = 0; i < g.Windows.Size; i++)
        if (g.Windows[i] == window)
        {
            g.Windows.erase(g.Windows.begin() + i);
            break;
        }
    g.Windows.push_back(window);
}

void ImGui::PushItemWidth(float item_width)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemWidth = (item_width == 0.0f ? window->ItemWidthDefault : item_width);
    window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
}

void ImGui::PushMultiItemsWidths(int components, float w_full)
{
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = GImGui->Style;
    if (w_full <= 0.0f)
        w_full = CalcItemWidth();
    const float w_item_one  = ImMax(1.0f, (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components-1)) / (float)components));
    const float w_item_last = ImMax(1.0f, (float)(int)(w_full - (w_item_one + style.ItemInnerSpacing.x) * (components-1)));
    window->DC.ItemWidthStack.push_back(w_item_last);
    for (int i = 0; i < components-1; i++)
        window->DC.ItemWidthStack.push_back(w_item_one);
    window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

void ImGui::PopItemWidth()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemWidthStack.pop_back();
    window->DC.ItemWidth = window->DC.ItemWidthStack.empty() ? window->ItemWidthDefault : window->DC.ItemWidthStack.back();
}

float ImGui::CalcItemWidth()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    float w = window->DC.ItemWidth;
    if (w < 0.0f)
    {
        // Align to a right-side limit. We include 1 frame padding in the calculation because this is how the width is always used (we add 2 frame padding to it), but we could move that responsibility to the widget as well.
        float width_to_right_edge = GetContentRegionAvail().x;
        w = ImMax(1.0f, width_to_right_edge + w);
    }
    w = (float)(int)w;
    return w;
}

static ImFont* GetDefaultFont()
{
    ImGuiContext& g = *GImGui;
    return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0];
}

static void SetCurrentFont(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(font && font->IsLoaded());    // Font Atlas not created. Did you call io.Fonts->GetTexDataAsRGBA32 / GetTexDataAsAlpha8 ?
    IM_ASSERT(font->Scale > 0.0f);
    g.Font = font;
    g.FontBaseSize = g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale;
    g.FontSize = g.CurrentWindow ? g.CurrentWindow->CalcFontSize() : 0.0f;
    g.FontTexUvWhitePixel = g.Font->ContainerAtlas->TexUvWhitePixel;
}

void ImGui::PushFont(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    if (!font)
        font = GetDefaultFont();
    SetCurrentFont(font);
    g.FontStack.push_back(font);
    g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void  ImGui::PopFont()
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow->DrawList->PopTextureID();
    g.FontStack.pop_back();
    SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void ImGui::PushItemFlag(ImGuiItemFlags option, bool enabled)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (enabled)
        window->DC.ItemFlags |= option;
    else
        window->DC.ItemFlags &= ~option;
    window->DC.ItemFlagsStack.push_back(window->DC.ItemFlags);
}

void ImGui::PopItemFlag()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.ItemFlagsStack.pop_back();
    window->DC.ItemFlags = window->DC.ItemFlagsStack.empty() ? ImGuiItemFlags_Default_ : window->DC.ItemFlagsStack.back();
}

void ImGui::PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
    PushItemFlag(ImGuiItemFlags_AllowKeyboardFocus, allow_keyboard_focus);
}

void ImGui::PopAllowKeyboardFocus()
{
    PopItemFlag();
}

void ImGui::PushButtonRepeat(bool repeat)
{
    PushItemFlag(ImGuiItemFlags_ButtonRepeat, repeat);
}

void ImGui::PopButtonRepeat()
{
    PopItemFlag();
}

void ImGui::PushTextWrapPos(float wrap_pos_x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.TextWrapPos = wrap_pos_x;
    window->DC.TextWrapPosStack.push_back(wrap_pos_x);
}

void ImGui::PopTextWrapPos()
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.TextWrapPosStack.pop_back();
    window->DC.TextWrapPos = window->DC.TextWrapPosStack.empty() ? -1.0f : window->DC.TextWrapPosStack.back();
}

// FIXME: This may incur a round-trip (if the end user got their data from a float4) but eventually we aim to store the in-flight colors as ImU32
void ImGui::PushStyleColor(ImGuiCol idx, ImU32 col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorModifiers.push_back(backup);
    g.Style.Colors[idx] = ColorConvertU32ToFloat4(col);
}

void ImGui::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorModifiers.push_back(backup);
    g.Style.Colors[idx] = col;
}

void ImGui::PopStyleColor(int count)
{
    ImGuiContext& g = *GImGui;
    while (count > 0)
    {
        ImGuiColMod& backup = g.ColorModifiers.back();
        g.Style.Colors[backup.Col] = backup.BackupValue;
        g.ColorModifiers.pop_back();
        count--;
    }
}

struct ImGuiStyleVarInfo
{
    ImGuiDataType   Type;
    ImU32           Offset;
    void*           GetVarPtr(ImGuiStyle* style) const { return (void*)((unsigned char*)style + Offset); }
};

static const ImGuiStyleVarInfo GStyleVarInfo[ImGuiStyleVar_Count_] =
{
    { ImGuiDataType_Float,  (ImU32)IM_OFFSETOF(ImGuiStyle, Alpha) },                // ImGuiStyleVar_Alpha
    { ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowPadding) },        // ImGuiStyleVar_WindowPadding
    { ImGuiDataType_Float,  (ImU32)IM_OFFSETOF(ImGuiStyle, WindowRounding) },       // ImGuiStyleVar_WindowRounding
    { ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowMinSize) },        // ImGuiStyleVar_WindowMinSize
    { ImGuiDataType_Float,  (ImU32)IM_OFFSETOF(ImGuiStyle, ChildWindowRounding) },  // ImGuiStyleVar_ChildWindowRounding
    { ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, FramePadding) },         // ImGuiStyleVar_FramePadding
    { ImGuiDataType_Float,  (ImU32)IM_OFFSETOF(ImGuiStyle, FrameRounding) },        // ImGuiStyleVar_FrameRounding
    { ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemSpacing) },          // ImGuiStyleVar_ItemSpacing
    { ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemInnerSpacing) },     // ImGuiStyleVar_ItemInnerSpacing
    { ImGuiDataType_Float,  (ImU32)IM_OFFSETOF(ImGuiStyle, IndentSpacing) },        // ImGuiStyleVar_IndentSpacing
    { ImGuiDataType_Float,  (ImU32)IM_OFFSETOF(ImGuiStyle, GrabMinSize) },          // ImGuiStyleVar_GrabMinSize
    { ImGuiDataType_Float2, (ImU32)IM_OFFSETOF(ImGuiStyle, ButtonTextAlign) },      // ImGuiStyleVar_ButtonTextAlign
};

static const ImGuiStyleVarInfo* GetStyleVarInfo(ImGuiStyleVar idx)
{
    IM_ASSERT(idx >= 0 && idx < ImGuiStyleVar_Count_);
    return &GStyleVarInfo[idx];
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, float val)
{
    const ImGuiStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float)
    {
        ImGuiContext& g = *GImGui;
        float* pvar = (float*)var_info->GetVarPtr(&g.Style);
        g.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
    IM_ASSERT(0); // Called function with wrong-type? Variable is not a float.
}

void ImGui::PushStyleVar(ImGuiStyleVar idx, const ImVec2& val)
{
    const ImGuiStyleVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float2)
    {
        ImGuiContext& g = *GImGui;
        ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&g.Style);
        g.StyleModifiers.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
    IM_ASSERT(0); // Called function with wrong-type? Variable is not a ImVec2.
}

void ImGui::PopStyleVar(int count)
{
    ImGuiContext& g = *GImGui;
    while (count > 0)
    {
        ImGuiStyleMod& backup = g.StyleModifiers.back();
        const ImGuiStyleVarInfo* info = GetStyleVarInfo(backup.VarIdx);
        if (info->Type == ImGuiDataType_Float)          (*(float*)info->GetVarPtr(&g.Style)) = backup.BackupFloat[0];
        else if (info->Type == ImGuiDataType_Float2)    (*(ImVec2*)info->GetVarPtr(&g.Style)) = ImVec2(backup.BackupFloat[0], backup.BackupFloat[1]);
        else if (info->Type == ImGuiDataType_Int)       (*(int*)info->GetVarPtr(&g.Style)) = backup.BackupInt[0];
        g.StyleModifiers.pop_back();
        count--;
    }
}

const char* ImGui::GetStyleColorName(ImGuiCol idx)
{
    // Create switch-case from enum with regexp: ImGuiCol_{.*}, --> case ImGuiCol_\1: return "\1";
    switch (idx)
    {
    case ImGuiCol_Text: return "Text";
    case ImGuiCol_TextDisabled: return "TextDisabled";
    case ImGuiCol_WindowBg: return "WindowBg";
    case ImGuiCol_ChildWindowBg: return "ChildWindowBg";
    case ImGuiCol_PopupBg: return "PopupBg";
    case ImGuiCol_Border: return "Border";
    case ImGuiCol_BorderShadow: return "BorderShadow";
    case ImGuiCol_FrameBg: return "FrameBg";
    case ImGuiCol_FrameBgHovered: return "FrameBgHovered";
    case ImGuiCol_FrameBgActive: return "FrameBgActive";
    case ImGuiCol_TitleBg: return "TitleBg";
    case ImGuiCol_TitleBgActive: return "TitleBgActive";
    case ImGuiCol_TitleBgCollapsed: return "TitleBgCollapsed";
    case ImGuiCol_MenuBarBg: return "MenuBarBg";
    case ImGuiCol_ScrollbarBg: return "ScrollbarBg";
    case ImGuiCol_ScrollbarGrab: return "ScrollbarGrab";
    case ImGuiCol_ScrollbarGrabHovered: return "ScrollbarGrabHovered";
    case ImGuiCol_ScrollbarGrabActive: return "ScrollbarGrabActive";
    case ImGuiCol_ComboBg: return "ComboBg";
    case ImGuiCol_CheckMark: return "CheckMark";
    case ImGuiCol_SliderGrab: return "SliderGrab";
    case ImGuiCol_SliderGrabActive: return "SliderGrabActive";
    case ImGuiCol_Button: return "Button";
    case ImGuiCol_ButtonHovered: return "ButtonHovered";
    case ImGuiCol_ButtonActive: return "ButtonActive";
    case ImGuiCol_Header: return "Header";
    case ImGuiCol_HeaderHovered: return "HeaderHovered";
    case ImGuiCol_HeaderActive: return "HeaderActive";
    case ImGuiCol_Separator: return "Separator";
    case ImGuiCol_SeparatorHovered: return "SeparatorHovered";
    case ImGuiCol_SeparatorActive: return "SeparatorActive";
    case ImGuiCol_ResizeGrip: return "ResizeGrip";
    case ImGuiCol_ResizeGripHovered: return "ResizeGripHovered";
    case ImGuiCol_ResizeGripActive: return "ResizeGripActive";
    case ImGuiCol_CloseButton: return "CloseButton";
    case ImGuiCol_CloseButtonHovered: return "CloseButtonHovered";
    case ImGuiCol_CloseButtonActive: return "CloseButtonActive";
    case ImGuiCol_PlotLines: return "PlotLines";
    case ImGuiCol_PlotLinesHovered: return "PlotLinesHovered";
    case ImGuiCol_PlotHistogram: return "PlotHistogram";
    case ImGuiCol_PlotHistogramHovered: return "PlotHistogramHovered";
    case ImGuiCol_TextSelectedBg: return "TextSelectedBg";
    case ImGuiCol_ModalWindowDarkening: return "ModalWindowDarkening";
    }
    IM_ASSERT(0);
    return "Unknown";
}

bool ImGui::IsWindowHovered()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredWindow == g.CurrentWindow && IsWindowContentHoverable(g.HoveredRootWindow);
}

bool ImGui::IsWindowRectHovered()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredWindow == g.CurrentWindow;
}

bool ImGui::IsWindowFocused()
{
    ImGuiContext& g = *GImGui;
    return g.NavWindow == g.CurrentWindow;
}

bool ImGui::IsRootWindowFocused()
{
    ImGuiContext& g = *GImGui;
    return g.NavWindow == g.CurrentWindow->RootWindow;
}

bool ImGui::IsRootWindowOrAnyChildFocused()
{
    ImGuiContext& g = *GImGui;
    return g.NavWindow && g.NavWindow->RootWindow == g.CurrentWindow->RootWindow;
}

bool ImGui::IsRootWindowOrAnyChildHovered()
{
    ImGuiContext& g = *GImGui;
    return g.HoveredRootWindow && (g.HoveredRootWindow == g.CurrentWindow->RootWindow) && IsWindowContentHoverable(g.HoveredRootWindow);
}

float ImGui::GetWindowWidth()
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    return window->Size.x;
}

float ImGui::GetWindowHeight()
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    return window->Size.y;
}

ImVec2 ImGui::GetWindowPos()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    return window->Pos;
}

static void SetWindowScrollY(ImGuiWindow* window, float new_scroll_y)
{
    window->DC.CursorMaxPos.y += window->Scroll.y; // SizeContents is generally computed based on CursorMaxPos which is affected by scroll position, so we need to apply our change to it.
    window->Scroll.y = new_scroll_y;
    window->DC.CursorMaxPos.y -= window->Scroll.y;
}

static void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond)
{
    // Test condition (NB: bit 0 is always true) and clear flags for next time
    if (cond && (window->SetWindowPosAllowFlags & cond) == 0)
        return;
    window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
    window->SetWindowPosVal = ImVec2(FLT_MAX, FLT_MAX);

    // Set
    const ImVec2 old_pos = window->Pos;
    window->PosFloat = pos;
    window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
    window->DC.CursorPos += (window->Pos - old_pos);    // As we happen to move the window while it is being appended to (which is a bad idea - will smear) let's at least offset the cursor
    window->DC.CursorMaxPos += (window->Pos - old_pos); // And more importantly we need to adjust this so size calculation doesn't get affected.
}

void ImGui::SetWindowPos(const ImVec2& pos, ImGuiCond cond)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    SetWindowPos(window, pos, cond);
}

void ImGui::SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond)
{
    if (ImGuiWindow* window = FindWindowByName(name))
        SetWindowPos(window, pos, cond);
}

ImVec2 ImGui::GetWindowSize()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Size;
}

static void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond)
{
    // Test condition (NB: bit 0 is always true) and clear flags for next time
    if (cond && (window->SetWindowSizeAllowFlags & cond) == 0)
        return;
    window->SetWindowSizeAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);

    // Set
    if (size.x > 0.0f)
    {
        window->AutoFitFramesX = 0;
        window->SizeFull.x = size.x;
    }
    else
    {
        window->AutoFitFramesX = 2;
        window->AutoFitOnlyGrows = false;
    }
    if (size.y > 0.0f)
    {
        window->AutoFitFramesY = 0;
        window->SizeFull.y = size.y;
    }
    else
    {
        window->AutoFitFramesY = 2;
        window->AutoFitOnlyGrows = false;
    }
}

void ImGui::SetWindowSize(const ImVec2& size, ImGuiCond cond)
{
    SetWindowSize(GImGui->CurrentWindow, size, cond);
}

void ImGui::SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond)
{
    if (ImGuiWindow* window = FindWindowByName(name))
        SetWindowSize(window, size, cond);
}

static void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond)
{
    // Test condition (NB: bit 0 is always true) and clear flags for next time
    if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0)
        return;
    window->SetWindowCollapsedAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);

    // Set
    window->Collapsed = collapsed;
}

void ImGui::SetWindowCollapsed(bool collapsed, ImGuiCond cond)
{
    SetWindowCollapsed(GImGui->CurrentWindow, collapsed, cond);
}

bool ImGui::IsWindowCollapsed()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Collapsed;
}

bool ImGui::IsWindowAppearing()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Appearing;
}

void ImGui::SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond)
{
    ImGuiWindow* window = FindWindowByName(name);
    if (window)
        SetWindowCollapsed(window, collapsed, cond);
}

void ImGui::SetWindowFocus()
{
    FocusWindow(GImGui->CurrentWindow);
}

void ImGui::SetWindowFocus(const char* name)
{
    if (name)
    {
        if (ImGuiWindow* window = FindWindowByName(name))
            FocusWindow(window);
    }
    else
    {
        FocusWindow(NULL);
    }
}

void ImGui::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowPosVal = pos;
    g.SetNextWindowPosPivot = pivot;
    g.SetNextWindowPosCond = cond ? cond : ImGuiCond_Always;
}

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
void ImGui::SetNextWindowPosCenter(ImGuiCond cond)
{
    SetNextWindowPos(GetIO().DisplaySize * 0.5f, cond, ImVec2(0.5f, 0.5f));
}
#endif

void ImGui::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowSizeVal = size;
    g.SetNextWindowSizeCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeConstraintCallback custom_callback, void* custom_callback_user_data)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowSizeConstraint = true;
    g.SetNextWindowSizeConstraintRect = ImRect(size_min, size_max);
    g.SetNextWindowSizeConstraintCallback = custom_callback;
    g.SetNextWindowSizeConstraintCallbackUserData = custom_callback_user_data;
}

void ImGui::SetNextWindowContentSize(const ImVec2& size)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowContentSizeVal = size;
    g.SetNextWindowContentSizeCond = ImGuiCond_Always;
}

void ImGui::SetNextWindowContentWidth(float width)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowContentSizeVal = ImVec2(width, g.SetNextWindowContentSizeCond ? g.SetNextWindowContentSizeVal.y : 0.0f);
    g.SetNextWindowContentSizeCond = ImGuiCond_Always;
}

void ImGui::SetNextWindowCollapsed(bool collapsed, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowCollapsedVal = collapsed;
    g.SetNextWindowCollapsedCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowFocus()
{
    ImGuiContext& g = *GImGui;
    g.SetNextWindowFocus = true;
}

// In window space (not screen space!)
ImVec2 ImGui::GetContentRegionMax()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    ImVec2 mx = window->ContentsRegionRect.Max;
    if (window->DC.ColumnsCount != 1)
        mx.x = GetColumnOffset(window->DC.ColumnsCurrent + 1) - window->WindowPadding.x;
    return mx;
}

ImVec2 ImGui::GetContentRegionAvail()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return GetContentRegionMax() - (window->DC.CursorPos - window->Pos);
}

float ImGui::GetContentRegionAvailWidth()
{
    return GetContentRegionAvail().x;
}

// In window space (not screen space!)
ImVec2 ImGui::GetWindowContentRegionMin()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ContentsRegionRect.Min;
}

ImVec2 ImGui::GetWindowContentRegionMax()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ContentsRegionRect.Max;
}

float ImGui::GetWindowContentRegionWidth()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ContentsRegionRect.Max.x - window->ContentsRegionRect.Min.x;
}

float ImGui::GetTextLineHeight()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize;
}

float ImGui::GetTextLineHeightWithSpacing()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.ItemSpacing.y;
}

float ImGui::GetItemsLineHeightWithSpacing()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y;
}

ImDrawList* ImGui::GetWindowDrawList()
{
    ImGuiWindow* window = GetCurrentWindow();
    return window->DrawList;
}

ImFont* ImGui::GetFont()
{
    return GImGui->Font;
}

float ImGui::GetFontSize()
{
    return GImGui->FontSize;
}

ImVec2 ImGui::GetFontTexUvWhitePixel()
{
    return GImGui->FontTexUvWhitePixel;
}

void ImGui::SetWindowFontScale(float scale)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    window->FontWindowScale = scale;
    g.FontSize = window->CalcFontSize();
}

// User generally sees positions in window coordinates. Internally we store CursorPos in absolute screen coordinates because it is more convenient.
// Conversion happens as we pass the value to user, but it makes our naming convention confusing because GetCursorPos() == (DC.CursorPos - window.Pos). May want to rename 'DC.CursorPos'.
ImVec2 ImGui::GetCursorPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos - window->Pos + window->Scroll;
}

float ImGui::GetCursorPosX()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float ImGui::GetCursorPosY()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}

void ImGui::SetCursorPos(const ImVec2& local_pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
    window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

void ImGui::SetCursorPosX(float x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
    window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPos.x);
}

void ImGui::SetCursorPosY(float y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
    window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
}

ImVec2 ImGui::GetCursorStartPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorStartPos - window->Pos;
}

ImVec2 ImGui::GetCursorScreenPos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos;
}

void ImGui::SetCursorScreenPos(const ImVec2& screen_pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos = screen_pos;
    window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

float ImGui::GetScrollX()
{
    return GImGui->CurrentWindow->Scroll.x;
}

float ImGui::GetScrollY()
{
    return GImGui->CurrentWindow->Scroll.y;
}

float ImGui::GetScrollMaxX()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return ImMax(0.0f, window->SizeContents.x - (window->SizeFull.x - window->ScrollbarSizes.x));
}

float ImGui::GetScrollMaxY()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return ImMax(0.0f, window->SizeContents.y - (window->SizeFull.y - window->ScrollbarSizes.y));
}

void ImGui::SetScrollX(float scroll_x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->ScrollTarget.x = scroll_x;
    window->ScrollTargetCenterRatio.x = 0.0f;
}

void ImGui::SetScrollY(float scroll_y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->ScrollTarget.y = scroll_y + window->TitleBarHeight() + window->MenuBarHeight(); // title bar height canceled out when using ScrollTargetRelY
    window->ScrollTargetCenterRatio.y = 0.0f;
}

void ImGui::SetScrollFromPosY(float pos_y, float center_y_ratio)
{
    // We store a target position so centering can occur on the next frame when we are guaranteed to have a known window size
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(center_y_ratio >= 0.0f && center_y_ratio <= 1.0f);
    window->ScrollTarget.y = (float)(int)(pos_y + window->Scroll.y);
    if (center_y_ratio <= 0.0f && window->ScrollTarget.y <= window->WindowPadding.y)    // Minor hack to make "scroll to top" take account of WindowPadding, else it would scroll to (WindowPadding.y - ItemSpacing.y)
        window->ScrollTarget.y = 0.0f;
    window->ScrollTargetCenterRatio.y = center_y_ratio;
}

// center_y_ratio: 0.0f top of last item, 0.5f vertical center of last item, 1.0f bottom of last item.
void ImGui::SetScrollHere(float center_y_ratio)
{
    ImGuiWindow* window = GetCurrentWindow();
    float target_y = window->DC.CursorPosPrevLine.y - window->Pos.y; // Top of last item, in window space
    target_y += (window->DC.PrevLineHeight * center_y_ratio) + (GImGui->Style.ItemSpacing.y * (center_y_ratio - 0.5f) * 2.0f); // Precisely aim above, in the middle or below the last line.
    SetScrollFromPosY(target_y, center_y_ratio);
}

void ImGui::SetKeyboardFocusHere(int offset)
{
    IM_ASSERT(offset >= -1);    // -1 is allowed but not below
    ImGuiWindow* window = GetCurrentWindow();
    window->FocusIdxAllRequestNext = window->FocusIdxAllCounter + 1 + offset;
    window->FocusIdxTabRequestNext = INT_MAX;
}

void ImGui::SetStateStorage(ImGuiStorage* tree)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.StateStorage = tree ? tree : &window->StateStorage;
}

ImGuiStorage* ImGui::GetStateStorage()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.StateStorage;
}

void ImGui::TextV(const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    TextUnformatted(g.TempBuffer, text_end);
}

void ImGui::Text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextV(fmt, args);
    va_end(args);
}

void ImGui::TextColoredV(const ImVec4& col, const char* fmt, va_list args)
{
    PushStyleColor(ImGuiCol_Text, col);
    TextV(fmt, args);
    PopStyleColor();
}

void ImGui::TextColored(const ImVec4& col, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextColoredV(col, fmt, args);
    va_end(args);
}

void ImGui::TextDisabledV(const char* fmt, va_list args)
{
    PushStyleColor(ImGuiCol_Text, GImGui->Style.Colors[ImGuiCol_TextDisabled]);
    TextV(fmt, args);
    PopStyleColor();
}

void ImGui::TextDisabled(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextDisabledV(fmt, args);
    va_end(args);
}

void ImGui::TextWrappedV(const char* fmt, va_list args)
{
    bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);    // Keep existing wrap position is one ia already set
    if (need_wrap) PushTextWrapPos(0.0f);
    TextV(fmt, args);
    if (need_wrap) PopTextWrapPos();
}

void ImGui::TextWrapped(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextWrappedV(fmt, args);
    va_end(args);
}

void ImGui::TextUnformatted(const char* text, const char* text_end)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrentLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = wrap_pos_x >= 0.0f;
    if (text_end - text > 2000 && !wrap_enabled)
    {
        // Long text!
        // Perform manual coarse clipping to optimize for long multi-line text
        // From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
        // We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
        const char* line = text;
        const float line_height = GetTextLineHeight();
        const ImRect clip_rect = window->ClipRect;
        ImVec2 text_size(0,0);

        if (text_pos.y <= clip_rect.Max.y)
        {
            ImVec2 pos = text_pos;

            // Lines to skip (can't skip when logging text)
            if (!g.LogEnabled)
            {
                int lines_skippable = (int)((clip_rect.Min.y - text_pos.y) / line_height);
                if (lines_skippable > 0)
                {
                    int lines_skipped = 0;
                    while (line < text_end && lines_skipped < lines_skippable)
                    {
                        const char* line_end = strchr(line, '\n');
                        if (!line_end)
                            line_end = text_end;
                        line = line_end + 1;
                        lines_skipped++;
                    }
                    pos.y += lines_skipped * line_height;
                }
            }

            // Lines to render
            if (line < text_end)
            {
                ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
                while (line < text_end)
                {
                    const char* line_end = strchr(line, '\n');
                    if (IsClippedEx(line_rect, 0, false))
                        break;

                    const ImVec2 line_size = CalcTextSize(line, line_end, false);
                    text_size.x = ImMax(text_size.x, line_size.x);
                    RenderText(pos, line, line_end, false);
                    if (!line_end)
                        line_end = text_end;
                    line = line_end + 1;
                    line_rect.Min.y += line_height;
                    line_rect.Max.y += line_height;
                    pos.y += line_height;
                }

                // Count remaining lines
                int lines_skipped = 0;
                while (line < text_end)
                {
                    const char* line_end = strchr(line, '\n');
                    if (!line_end)
                        line_end = text_end;
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }

            text_size.y += (pos - text_pos).y;
        }

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(bb);
        ItemAdd(bb, 0);
    }
    else
    {
        const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
        const ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

        // Account of baseline offset
        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size);
        if (!ItemAdd(bb, 0))
            return;

        // Render (we don't hide text after ## in this end-user function)
        RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
    }
}

void ImGui::AlignTextToFramePadding()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    window->DC.CurrentLineHeight = ImMax(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y * 2);
    window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.CurrentLineTextBaseOffset, g.Style.FramePadding.y);
}

// Add a label+text combo aligned to other label+value widgets
void ImGui::LabelTextV(const char* label, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect value_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2));
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y*2) + label_size);
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0))
        return;

    // Render
    const char* value_text_begin = &g.TempBuffer[0];
    const char* value_text_end = value_text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    RenderTextClipped(value_bb.Min, value_bb.Max, value_text_begin, value_text_end, NULL, ImVec2(0.0f,0.5f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x, value_bb.Min.y + style.FramePadding.y), label);
}

void ImGui::LabelText(const char* label, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    LabelTextV(label, fmt, args);
    va_end(args);
}

bool ImGui::ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    if (flags & ImGuiButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default behavior requires click+release on same spot
    if ((flags & (ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick)) == 0)
        flags |= ImGuiButtonFlags_PressedOnClickRelease;

    ImGuiWindow* backup_hovered_window = g.HoveredWindow;
    if ((flags & ImGuiButtonFlags_FlattenChilds) && g.HoveredRootWindow == window)
        g.HoveredWindow = window;

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    if ((flags & ImGuiButtonFlags_FlattenChilds) && g.HoveredRootWindow == window)
        g.HoveredWindow = backup_hovered_window;

    if (hovered)
    {
        if (!(flags & ImGuiButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            //                        | CLICKING        | HOLDING with ImGuiButtonFlags_Repeat
            // PressedOnClickRelease  |  <on release>*  |  <on repeat> <on repeat> .. (NOT on release)  <-- MOST COMMON! (*) only if both click/release were over bounds
            // PressedOnClick         |  <on click>     |  <on click> <on repeat> <on repeat> ..
            // PressedOnRelease       |  <on release>   |  <on repeat> <on repeat> .. (NOT on release)
            // PressedOnDoubleClick   |  <on dclick>    |  <on dclick> <on repeat> <on repeat> ..
            if ((flags & ImGuiButtonFlags_PressedOnClickRelease) && g.IO.MouseClicked[0])
            {
                SetActiveID(id, window); // Hold on ID
                FocusWindow(window);
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;
            }
            if (((flags & ImGuiButtonFlags_PressedOnClick) && g.IO.MouseClicked[0]) || ((flags & ImGuiButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[0]))
            {
                pressed = true;
                if (flags & ImGuiButtonFlags_NoHoldingActiveID)
                    ClearActiveID();
                else
                    SetActiveID(id, window); // Hold on ID
                FocusWindow(window);
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;
            }
            if ((flags & ImGuiButtonFlags_PressedOnRelease) && g.IO.MouseReleased[0])
            {
                if (!((flags & ImGuiButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))  // Repeat mode trumps <on release>
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above). 
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if ((flags & ImGuiButtonFlags_Repeat) && g.ActiveId == id && g.IO.MouseDownDuration[0] > 0.0f && IsMouseClicked(0, true))
                pressed = true;
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.IO.MouseDown[0])
        {
            held = true;
        }
        else
        {
            if (hovered && (flags & ImGuiButtonFlags_PressedOnClickRelease))
                if (!((flags & ImGuiButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))  // Repeat mode trumps <on release>
                    pressed = true;
            ClearActiveID();
        }
    }

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & ImGuiButtonFlags_AllowOverlapMode) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = pressed = held = false;

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool ImGui::ButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

    if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    return pressed;
}

bool ImGui::Button(const char* label, const ImVec2& size_arg)
{
    return ButtonEx(label, size_arg, 0);
}

// Small buttons fits within text without additional vertical spacing.
bool ImGui::SmallButton(const char* label)
{
    ImGuiContext& g = *GImGui;
    float backup_padding_y = g.Style.FramePadding.y;
    g.Style.FramePadding.y = 0.0f;
    bool pressed = ButtonEx(label, ImVec2(0,0), ImGuiButtonFlags_AlignTextBaseLine);
    g.Style.FramePadding.y = backup_padding_y;
    return pressed;
}

// Tip: use ImGui::PushID()/PopID() to push indices or pointers in the ID stack.
// Then you can keep 'str_id' empty or the same for all your buttons (instead of creating a string based on a non-string id)
bool ImGui::InvisibleButton(const char* str_id, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(str_id);
    ImVec2 size = CalcItemSize(size_arg, 0.0f, 0.0f);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    return pressed;
}

// Upper-right button to close a window.
bool ImGui::CloseButton(ImGuiID id, const ImVec2& pos, float radius)
{
    ImGuiWindow* window = GetCurrentWindow();

	ImRect bb(pos - ImVec2(radius, radius - 1), pos + ImVec2(radius - 1, radius));

	bb = ImRect(bb.Min + ImVec2(3 - bb.GetWidth(), 0), bb.Max + ImVec2(3, 0));

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_CloseButtonActive : hovered ? ImGuiCol_CloseButtonHovered : ImGuiCol_CloseButton);
	const ImVec2 center = bb.GetCenter();
	const float window_rounding = (window->Flags & ImGuiWindowFlags_ChildWindow) ? ImGui::GetStyle().ChildWindowRounding : ImGui::GetStyle().WindowRounding;
	const auto pad = ImGui::GetStyle().WindowPadThickness * 0.33f;
	RenderFrame(bb.Min + ImVec2(0, pad), bb.Max - ImVec2(0, pad) , col, true, window_rounding);

    return pressed;
}

void ImGui::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    if (border_col.w > 0.0f)
        bb.Max += ImVec2(2,2);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    if (border_col.w > 0.0f)
    {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
        window->DrawList->AddImage(user_texture_id, bb.Min+ImVec2(1,1), bb.Max-ImVec2(1,1), uv0, uv1, GetColorU32(tint_col));
    }
    else
    {
        window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col));
    }
}

// frame_padding < 0: uses FramePadding from style (default)
// frame_padding = 0: no framing
// frame_padding > 0: set framing size
// The color used are the button colors.
bool ImGui::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Default to using texture ID as ID. User can still push string/integer prefixes.
    // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
    PushID((void *)user_texture_id);
    const ImGuiID id = window->GetID("#image");
    PopID();

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding*2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
    window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    return pressed;
}

// Start logging ImGui output to TTY
void ImGui::LogToTTY(int max_depth)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled)
        return;
    ImGuiWindow* window = g.CurrentWindow;

    g.LogEnabled = true;
    g.LogFile = stdout;
    g.LogStartDepth = window->DC.TreeDepth;
    if (max_depth >= 0)
        g.LogAutoExpandMaxDepth = max_depth;
}

// Start logging ImGui output to given file
void ImGui::LogToFile(int max_depth, const char* filename)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled)
        return;
    ImGuiWindow* window = g.CurrentWindow;

    if (!filename)
    {
        filename = g.IO.LogFilename;
        if (!filename)
            return;
    }

    g.LogFile = ImFileOpen(filename, "ab");
    if (!g.LogFile)
    {
        IM_ASSERT(g.LogFile != NULL); // Consider this an error
        return;
    }
    g.LogEnabled = true;
    g.LogStartDepth = window->DC.TreeDepth;
    if (max_depth >= 0)
        g.LogAutoExpandMaxDepth = max_depth;
}

// Start logging ImGui output to clipboard
void ImGui::LogToClipboard(int max_depth)
{
    ImGuiContext& g = *GImGui;
    if (g.LogEnabled)
        return;
    ImGuiWindow* window = g.CurrentWindow;

    g.LogEnabled = true;
    g.LogFile = NULL;
    g.LogStartDepth = window->DC.TreeDepth;
    if (max_depth >= 0)
        g.LogAutoExpandMaxDepth = max_depth;
}

void ImGui::LogFinish()
{
    ImGuiContext& g = *GImGui;
    if (!g.LogEnabled)
        return;

    LogText(IM_NEWLINE);
    g.LogEnabled = false;
    if (g.LogFile != NULL)
    {
        if (g.LogFile == stdout)
            fflush(g.LogFile);
        else
            fclose(g.LogFile);
        g.LogFile = NULL;
    }
    if (g.LogClipboard->size() > 1)
    {
        SetClipboardText(g.LogClipboard->begin());
        g.LogClipboard->clear();
    }
}

// Helper to display logging buttons
void ImGui::LogButtons()
{
    ImGuiContext& g = *GImGui;

    PushID("LogButtons");
    const bool log_to_tty = Button("Log To TTY"); SameLine();
    const bool log_to_file = Button("Log To File"); SameLine();
    const bool log_to_clipboard = Button("Log To Clipboard"); SameLine();
    PushItemWidth(80.0f);
    PushAllowKeyboardFocus(false);
    SliderInt("Depth", &g.LogAutoExpandMaxDepth, 0, 9, NULL);
    PopAllowKeyboardFocus();
    PopItemWidth();
    PopID();

    // Start logging at the end of the function so that the buttons don't appear in the log
    if (log_to_tty)
        LogToTTY(g.LogAutoExpandMaxDepth);
    if (log_to_file)
        LogToFile(g.LogAutoExpandMaxDepth, g.IO.LogFilename);
    if (log_to_clipboard)
        LogToClipboard(g.LogAutoExpandMaxDepth);
}

bool ImGui::TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags)
{
    if (flags & ImGuiTreeNodeFlags_Leaf)
        return true;

    // We only write to the tree storage if the user clicks (or explicitely use SetNextTreeNode*** functions)
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiStorage* storage = window->DC.StateStorage;

    bool is_open;
    if (g.SetNextTreeNodeOpenCond != 0)
    {
        if (g.SetNextTreeNodeOpenCond & ImGuiCond_Always)
        {
            is_open = g.SetNextTreeNodeOpenVal;
            storage->SetInt(id, is_open);
        }
        else
        {
            // We treat ImGuiCond_Once and ImGuiCond_FirstUseEver the same because tree node state are not saved persistently.
            const int stored_value = storage->GetInt(id, -1);
            if (stored_value == -1)
            {
                is_open = g.SetNextTreeNodeOpenVal;
                storage->SetInt(id, is_open);
            }
            else
            {
                is_open = stored_value != 0;
            }
        }
        g.SetNextTreeNodeOpenCond = 0;
    }
    else
    {
        is_open = storage->GetInt(id, (flags & ImGuiTreeNodeFlags_DefaultOpen) ? 1 : 0) != 0;
    }

    // When logging is enabled, we automatically expand tree nodes (but *NOT* collapsing headers.. seems like sensible behavior).
    // NB- If we are above max depth we still allow manually opened nodes to be logged.
    if (g.LogEnabled && !(flags & ImGuiTreeNodeFlags_NoAutoOpenOnLog) && window->DC.TreeDepth < g.LogAutoExpandMaxDepth)
        is_open = true;

    return is_open;
}

bool ImGui::TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, 0.0f);

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    const float text_base_offset_y = ImMax(padding.y, window->DC.CurrentLineTextBaseOffset); // Latch before ItemSize changes it
    const float frame_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + style.FramePadding.y*2), label_size.y + padding.y*2);
    ImRect bb = ImRect(window->DC.CursorPos, ImVec2(window->Pos.x + GetContentRegionMax().x, window->DC.CursorPos.y + frame_height));
    if (display_frame)
    {
        // Framed header expand a little outside the default padding
        bb.Min.x -= (float)(int)(window->WindowPadding.x*0.5f) - 1;
        bb.Max.x += (float)(int)(window->WindowPadding.x*0.5f) - 1;
    }

    const float text_offset_x = (g.FontSize + (display_frame ? padding.x*3 : padding.x*2));   // Collapser arrow width + Spacing
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x*2 : 0.0f);   // Include collapser
    ItemSize(ImVec2(text_width, frame_height), text_base_offset_y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    // (Ideally we'd want to add a flag for the user to specify if we want the hit test to be done up to the right side of the content or not)
    const ImRect interact_bb = display_frame ? bb : ImRect(bb.Min.x, bb.Min.y, bb.Min.x + text_width + style.ItemSpacing.x*2, bb.Max.y);
    bool is_open = TreeNodeBehaviorIsOpen(id, flags);
    if (!ItemAdd(interact_bb, id))
    {
        if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
            TreePushRawID(id);
        return is_open;
    }

    // Flags that affects opening behavior:
    // - 0(default) ..................... single-click anywhere to open
    // - OpenOnDoubleClick .............. double-click anywhere to open
    // - OpenOnArrow .................... single-click on arrow to open
    // - OpenOnDoubleClick|OpenOnArrow .. single-click on arrow or double-click anywhere to open
    ImGuiButtonFlags button_flags = ImGuiButtonFlags_NoKeyModifiers | ((flags & ImGuiTreeNodeFlags_AllowOverlapMode) ? ImGuiButtonFlags_AllowOverlapMode : 0);
    if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= ImGuiButtonFlags_PressedOnDoubleClick | ((flags & ImGuiTreeNodeFlags_OpenOnArrow) ? ImGuiButtonFlags_PressedOnClickRelease : 0);
    bool hovered, held, pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    if (pressed && !(flags & ImGuiTreeNodeFlags_Leaf))
    {
        bool toggled = !(flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick));
        if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
            toggled |= IsMouseHoveringRect(interact_bb.Min, ImVec2(interact_bb.Min.x + text_offset_x, interact_bb.Max.y));
        if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
            toggled |= g.IO.MouseDoubleClicked[0];
        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
        }
    }
    if (flags & ImGuiTreeNodeFlags_AllowOverlapMode)
        SetItemAllowOverlap();

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
    const ImVec2 text_pos = bb.Min + ImVec2(text_offset_x, text_base_offset_y);
    if (display_frame)
    {
        // Framed type
        RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
        RenderTriangle(bb.Min + ImVec2(padding.x, text_base_offset_y), is_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
        if (g.LogEnabled)
        {
            // NB: '##' is normally used to hide text (as a library-wide feature), so we need to specify the text range to make sure the ## aren't stripped out here.
            const char log_prefix[] = "\n##";
            const char log_suffix[] = "##";
            LogRenderedText(&text_pos, log_prefix, log_prefix+3);
            RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
            LogRenderedText(&text_pos, log_suffix+1, log_suffix+3);
        }
        else
        {
            RenderTextClipped(text_pos, bb.Max, label, label_end, &label_size);
        }
    }
    else
    {
        // Unframed typed for tree nodes
        if (hovered || (flags & ImGuiTreeNodeFlags_Selected))
            RenderFrame(bb.Min, bb.Max, col, false);

        if (flags & ImGuiTreeNodeFlags_Bullet)
            RenderBullet(bb.Min + ImVec2(text_offset_x * 0.5f, g.FontSize*0.50f + text_base_offset_y));
        else if (!(flags & ImGuiTreeNodeFlags_Leaf))
            RenderTriangle(bb.Min + ImVec2(padding.x, g.FontSize*0.15f + text_base_offset_y), is_open ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);
        if (g.LogEnabled)
            LogRenderedText(&text_pos, ">");
        RenderText(text_pos, label, label_end, false);
    }

    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        TreePushRawID(id);
    return is_open;
}

// CollapsingHeader returns true when opened but do not indent nor push into the ID stack (because of the ImGuiTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label, ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen). You can remove the _NoTreePushOnOpen flag if you want behavior closer to normal TreeNode().
bool ImGui::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return TreeNodeBehavior(window->GetID(label), flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen, label);
}

bool ImGui::CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    if (p_open && !*p_open)
        return false;

    ImGuiID id = window->GetID(label);
    bool is_open = TreeNodeBehavior(id, flags | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen | (p_open ? ImGuiTreeNodeFlags_AllowOverlapMode : 0), label);
    if (p_open)
    {
        // Create a small overlapping close button // FIXME: We can evolve this into user accessible helpers to add extra buttons on title bars, headers, etc.
        ImGuiContext& g = *GImGui;
        float button_sz = g.FontSize * 0.5f;
        ImGuiItemHoveredDataBackup last_item_backup;
        last_item_backup.Backup();
        if (CloseButton(window->GetID((void*)(intptr_t)(id+1)), ImVec2(ImMin(window->DC.LastItemRect.Max.x, window->ClipRect.Max.x) - g.Style.FramePadding.x - button_sz, window->DC.LastItemRect.Min.y + g.Style.FramePadding.y + button_sz), button_sz))
            *p_open = false;
        last_item_backup.Restore();
    }

    return is_open;
}

bool ImGui::TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return TreeNodeBehavior(window->GetID(label), flags, label, NULL);
}

bool ImGui::TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return TreeNodeBehavior(window->GetID(str_id), flags, g.TempBuffer, label_end);
}

bool ImGui::TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return TreeNodeBehavior(window->GetID(ptr_id), flags, g.TempBuffer, label_end);
}

bool ImGui::TreeNodeV(const char* str_id, const char* fmt, va_list args)
{
    return TreeNodeExV(str_id, 0, fmt, args);
}

bool ImGui::TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
{
    return TreeNodeExV(ptr_id, 0, fmt, args);
}

bool ImGui::TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(str_id, flags, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(ptr_id, flags, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::TreeNode(const char* str_id, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(str_id, 0, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::TreeNode(const void* ptr_id, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(ptr_id, 0, fmt, args);
    va_end(args);
    return is_open;
}

bool ImGui::TreeNode(const char* label)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    return TreeNodeBehavior(window->GetID(label), 0, label, NULL);
}

void ImGui::TreeAdvanceToLabelPos()
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow->DC.CursorPos.x += GetTreeNodeToLabelSpacing();
}

// Horizontal distance preceding label when using TreeNode() or Bullet()
float ImGui::GetTreeNodeToLabelSpacing()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + (g.Style.FramePadding.x * 2.0f);
}

void ImGui::SetNextTreeNodeOpen(bool is_open, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    g.SetNextTreeNodeOpenVal = is_open;
    g.SetNextTreeNodeOpenCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::PushID(const char* str_id)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetID(str_id));
}

void ImGui::PushID(const char* str_id_begin, const char* str_id_end)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetID(str_id_begin, str_id_end));
}

void ImGui::PushID(const void* ptr_id)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetID(ptr_id));
}

void ImGui::PushID(int int_id)
{
    const void* ptr_id = (void*)(intptr_t)int_id;
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.push_back(window->GetID(ptr_id));
}

void ImGui::PopID()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    window->IDStack.pop_back();
}

ImGuiID ImGui::GetID(const char* str_id)
{
    return GImGui->CurrentWindow->GetID(str_id);
}

ImGuiID ImGui::GetID(const char* str_id_begin, const char* str_id_end)
{
    return GImGui->CurrentWindow->GetID(str_id_begin, str_id_end);
}

ImGuiID ImGui::GetID(const void* ptr_id)
{
    return GImGui->CurrentWindow->GetID(ptr_id);
}

void ImGui::Bullet()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float line_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y*2), g.FontSize);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize, line_height));
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
    {
        SameLine(0, style.FramePadding.x*2);
        return;
    }

    // Render and stay on same line
    RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize*0.5f, line_height*0.5f));
    SameLine(0, style.FramePadding.x*2);
}

// Text with a little bullet aligned to the typical tree node.
void ImGui::BulletTextV(const char* fmt, va_list args)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const char* text_begin = g.TempBuffer;
    const char* text_end = text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    const ImVec2 label_size = CalcTextSize(text_begin, text_end, false);
    const float text_base_offset_y = ImMax(0.0f, window->DC.CurrentLineTextBaseOffset); // Latch before ItemSize changes it
    const float line_height = ImMax(ImMin(window->DC.CurrentLineHeight, g.FontSize + g.Style.FramePadding.y*2), g.FontSize);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize + (label_size.x > 0.0f ? (label_size.x + style.FramePadding.x*2) : 0.0f), ImMax(line_height, label_size.y)));  // Empty text doesn't add padding
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    RenderBullet(bb.Min + ImVec2(style.FramePadding.x + g.FontSize*0.5f, line_height*0.5f));
    RenderText(bb.Min+ImVec2(g.FontSize + style.FramePadding.x*2, text_base_offset_y), text_begin, text_end, false);
}

void ImGui::BulletText(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    BulletTextV(fmt, args);
    va_end(args);
}

static inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size)
{
    if (data_type == ImGuiDataType_Int)
        ImFormatString(buf, buf_size, display_format, *(int*)data_ptr);
    else if (data_type == ImGuiDataType_Float)
        ImFormatString(buf, buf_size, display_format, *(float*)data_ptr);
}

static inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size)
{
    if (data_type == ImGuiDataType_Int)
    {
        if (decimal_precision < 0)
            ImFormatString(buf, buf_size, "%d", *(int*)data_ptr);
        else
            ImFormatString(buf, buf_size, "%.*d", decimal_precision, *(int*)data_ptr);
    }
    else if (data_type == ImGuiDataType_Float)
    {
        if (decimal_precision < 0)
            ImFormatString(buf, buf_size, "%f", *(float*)data_ptr);     // Ideally we'd have a minimum decimal precision of 1 to visually denote that it is a float, while hiding non-significant digits?
        else
            ImFormatString(buf, buf_size, "%.*f", decimal_precision, *(float*)data_ptr);
    }
}

static void DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2)// Store into value1
{
    if (data_type == ImGuiDataType_Int)
    {
        if (op == '+')
            *(int*)value1 = *(int*)value1 + *(const int*)value2;
        else if (op == '-')
            *(int*)value1 = *(int*)value1 - *(const int*)value2;
    }
    else if (data_type == ImGuiDataType_Float)
    {
        if (op == '+')
            *(float*)value1 = *(float*)value1 + *(const float*)value2;
        else if (op == '-')
            *(float*)value1 = *(float*)value1 - *(const float*)value2;
    }
}

// User can input math operators (e.g. +100) to edit a numerical values.
static bool DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* data_ptr, const char* scalar_format)
{
    while (ImCharIsSpace(*buf))
        buf++;

    // We don't support '-' op because it would conflict with inputing negative value.
    // Instead you can use +-100 to subtract from an existing value
    char op = buf[0];
    if (op == '+' || op == '*' || op == '/')
    {
        buf++;
        while (ImCharIsSpace(*buf))
            buf++;
    }
    else
    {
        op = 0;
    }
    if (!buf[0])
        return false;

    if (data_type == ImGuiDataType_Int)
    {
        if (!scalar_format)
            scalar_format = "%d";
        int* v = (int*)data_ptr;
        const int old_v = *v;
        int arg0i = *v;
        if (op && sscanf(initial_value_buf, scalar_format, &arg0i) < 1)
            return false;

        // Store operand in a float so we can use fractional value for multipliers (*1.1), but constant always parsed as integer so we can fit big integers (e.g. 2000000003) past float precision
        float arg1f = 0.0f;
        if (op == '+')      { if (sscanf(buf, "%f", &arg1f) == 1) *v = (int)(arg0i + arg1f); }                 // Add (use "+-" to subtract)
        else if (op == '*') { if (sscanf(buf, "%f", &arg1f) == 1) *v = (int)(arg0i * arg1f); }                 // Multiply
        else if (op == '/') { if (sscanf(buf, "%f", &arg1f) == 1 && arg1f != 0.0f) *v = (int)(arg0i / arg1f); }// Divide
        else                { if (sscanf(buf, scalar_format, &arg0i) == 1) *v = arg0i; }                       // Assign constant (read as integer so big values are not lossy)
        return (old_v != *v);
    }
    else if (data_type == ImGuiDataType_Float)
    {
        // For floats we have to ignore format with precision (e.g. "%.2f") because sscanf doesn't take them in
        scalar_format = "%f";
        float* v = (float*)data_ptr;
        const float old_v = *v;
        float arg0f = *v;
        if (op && sscanf(initial_value_buf, scalar_format, &arg0f) < 1)
            return false;

        float arg1f = 0.0f;
        if (sscanf(buf, scalar_format, &arg1f) < 1)
            return false;
        if (op == '+')      { *v = arg0f + arg1f; }                    // Add (use "+-" to subtract)
        else if (op == '*') { *v = arg0f * arg1f; }                    // Multiply
        else if (op == '/') { if (arg1f != 0.0f) *v = arg0f / arg1f; } // Divide
        else                { *v = arg1f; }                            // Assign constant
        return (old_v != *v);
    }

    return false;
}

// Create text input in place of a slider (when CTRL+Clicking on slider)
// FIXME: Logic is messy and confusing.
bool ImGui::InputScalarAsWidgetReplacement(const ImRect& aabb, const char* label, ImGuiDataType data_type, void* data_ptr, ImGuiID id, int decimal_precision)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    // Our replacement widget will override the focus ID (registered previously to allow for a TAB focus to happen)
    // On the first frame, g.ScalarAsInputTextId == 0, then on subsequent frames it becomes == id
    SetActiveID(g.ScalarAsInputTextId, window);
    SetHoveredID(0);
    FocusableItemUnregister(window);

    char buf[32];
    DataTypeFormatString(data_type, data_ptr, decimal_precision, buf, IM_ARRAYSIZE(buf));
    bool text_value_changed = InputTextEx(label, buf, IM_ARRAYSIZE(buf), aabb.GetSize(), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_AutoSelectAll);
    if (g.ScalarAsInputTextId == 0)     // First frame we started displaying the InputText widget
    {
        IM_ASSERT(g.ActiveId == id);    // InputText ID expected to match the Slider ID (else we'd need to store them both, which is also possible)
        g.ScalarAsInputTextId = g.ActiveId;
        SetHoveredID(id);
    }
    if (text_value_changed)
        return DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, NULL);
    return false;
}

// Parse display precision back from the display format string
int ImGui::ParseFormatPrecision(const char* fmt, int default_precision)
{
    int precision = default_precision;
    while ((fmt = strchr(fmt, '%')) != NULL)
    {
        fmt++;
        if (fmt[0] == '%') { fmt++; continue; } // Ignore "%%"
        while (*fmt >= '0' && *fmt <= '9')
            fmt++;
        if (*fmt == '.')
        {
            fmt = ImAtoi(fmt + 1, &precision);
            if (precision < 0 || precision > 10)
                precision = default_precision;
        }
        if (*fmt == 'e' || *fmt == 'E') // Maximum precision with scientific notation
            precision = -1;
        break;
    }
    return precision;
}

static float GetMinimumStepAtDecimalPrecision(int decimal_precision)
{
    static const float min_steps[10] = { 1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f };
    return (decimal_precision >= 0 && decimal_precision < 10) ? min_steps[decimal_precision] : powf(10.0f, (float)-decimal_precision);
}

float ImGui::RoundScalar(float value, int decimal_precision)
{
    // Round past decimal precision
    // So when our value is 1.99999 with a precision of 0.001 we'll end up rounding to 2.0
    // FIXME: Investigate better rounding methods
    if (decimal_precision < 0)
        return value;
    const float min_step = GetMinimumStepAtDecimalPrecision(decimal_precision);
    bool negative = value < 0.0f;
    value = fabsf(value);
    float remainder = fmodf(value, min_step);
    if (remainder <= min_step*0.5f)
        value -= remainder;
    else
        value += (min_step - remainder);
    return negative ? -value : value;
}

static inline float SliderBehaviorCalcRatioFromValue(float v, float v_min, float v_max, float power, float linear_zero_pos)
{
    if (v_min == v_max)
        return 0.0f;

    const bool is_non_linear = (power < 1.0f-0.00001f) || (power > 1.0f+0.00001f);
    const float v_clamped = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
    if (is_non_linear)
    {
        if (v_clamped < 0.0f)
        {
            const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f,v_max) - v_min);
            return (1.0f - powf(f, 1.0f/power)) * linear_zero_pos;
        }
        else
        {
            const float f = (v_clamped - ImMax(0.0f,v_min)) / (v_max - ImMax(0.0f,v_min));
            return linear_zero_pos + powf(f, 1.0f/power) * (1.0f - linear_zero_pos);
        }
    }

    // Linear slider
    return (v_clamped - v_min) / (v_max - v_min);
}

bool ImGui::SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	const ImGuiStyle& style = g.Style;

	const_cast<float&>(frame_bb.Max.y) -= 2;
	const_cast<float&>(frame_bb.Min.y) += 2;

	// Draw frame
	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.GrabRounding);

	const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
	const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

	const float grab_padding = 2.0f;
	const float slider_sz = is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
	float grab_sz;
	if (decimal_precision != 0)
		grab_sz = ImMin(style.GrabMinSize, slider_sz);
	else
		grab_sz = ImMin(ImMax(1.0f * (slider_sz / ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)), style.GrabMinSize), slider_sz);  // Integer sliders, if possible have the grab size represent 1 unit
	const float slider_usable_sz = slider_sz - grab_sz;
	const float slider_usable_pos_min = (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz*0.5f;
	const float slider_usable_pos_max = (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz*0.5f;

	// For logarithmic sliders that cross over sign boundary we want the exponential increase to be symmetric around 0.0f
	float linear_zero_pos = 0.0f;   // 0.0->1.0f
	if (v_min * v_max < 0.0f)
	{
		// Different sign
		const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
		const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
		linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
	}
	else
	{
		// Same sign
		linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
	}

	// Process clicking on the slider
	bool value_changed = false;
	if (g.ActiveId == id)
	{
		bool set_new_value = false;
		float clicked_t = 0.0f;
		if (g.IO.MouseDown[0])
		{
			const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
			clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
			if (!is_horizontal)
				clicked_t = 1.0f - clicked_t;
			set_new_value = true;
		}
		else
		{
			ClearActiveID();
		}

		if (set_new_value)
		{
			float new_value;
			if (is_non_linear)
			{
				// Account for logarithmic scale on both sides of the zero
				if (clicked_t < linear_zero_pos)
				{
					// Negative: rescale to the negative range before powering
					float a = 1.0f - (clicked_t / linear_zero_pos);
					a = powf(a, power);
					new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
				}
				else
				{
					// Positive: rescale to the positive range before powering
					float a;
					if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
						a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
					else
						a = clicked_t;
					a = powf(a, power);
					new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
				}
			}
			else
			{
				// Linear slider
				new_value = ImLerp(v_min, v_max, clicked_t);
			}

			// Round past decimal precision
			new_value = RoundScalar(new_value, decimal_precision);
			if (*v != new_value)
			{
				*v = new_value;
				value_changed = true;
			}
		}
	}

	// Draw
	float grab_t = SliderBehaviorCalcRatioFromValue(*v, v_min, v_max, power, linear_zero_pos);
	if (!is_horizontal)
		grab_t = 1.0f - grab_t;
	const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
	ImRect grab_bb;
	if (is_horizontal)
		grab_bb = ImRect(ImVec2(grab_pos - grab_sz*0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz*0.5f, frame_bb.Max.y - grab_padding));
	else
		grab_bb = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz*0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz*0.5f));

	// lazy, just using clip :^)
	PushClipRect(frame_bb.Min + ImVec2(grab_padding, grab_padding), ImVec2(grab_bb.Min.x, grab_bb.Max.y), false);
	ImRect bb_slider(ImVec2(grab_bb.Min.x - frame_bb.GetWidth() + frame_bb.GetHeight() * 0.5f, frame_bb.Min.y), ImVec2(grab_bb.Max.x, frame_bb.Max.y));
	window->DrawList->AddRectFilled(bb_slider.Min, bb_slider.Max, GetColorU32(ImVec4(0.1f, 0.45, 1.f, 0.35)), style.GrabRounding);
	PopClipRect();

	window->DrawList->AddRect(grab_bb.Min - ImVec2(2, 3), grab_bb.Max + ImVec2(2, 3), GetColorU32(ImVec4(0, 0, 0, 1)), style.GrabRounding);
	window->DrawList->AddRectFilled(grab_bb.Min - ImVec2(1, 2), grab_bb.Max + ImVec2(1, 2), GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

	return value_changed;
}

// Use power!=1.0 for logarithmic sliders.
// Adjust display_format to decorate the value with a prefix or a suffix.
//   "%.3f"         1.234
//   "%5.2f secs"   01.23 secs
//   "Gold: %.0f"   Gold: 1
bool ImGui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    // NB- we don't call ItemSize() yet because we may turn into a text edit box below
    if (!ItemAdd(total_bb, id))
    {
        ItemSize(total_bb, style.FramePadding.y);
        return false;
    }
    const bool hovered = ItemHoverable(frame_bb, id);

    if (!display_format)
        display_format = "%.3f";
    int decimal_precision = ParseFormatPrecision(display_format, 3);

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    bool start_text_input = false;
    const bool tab_focus_requested = FocusableItemRegister(window, id);
    if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]))
    {
        SetActiveID(id, window);
        FocusWindow(window);
        if (tab_focus_requested || g.IO.KeyCtrl)
        {
            start_text_input = true;
            g.ScalarAsInputTextId = 0;
        }
    }
    if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
        return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float, v, id, decimal_precision);

    // Actual slider behavior + render grab
    ItemSize(total_bb, style.FramePadding.y);
    const bool value_changed = SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
    RenderTextClipped(frame_bb.Min, frame_bb.Max - ImVec2(0, 1), value_buf, value_buf_end, NULL, ImVec2(0.5f,0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

bool ImGui::VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;
    const bool hovered = ItemHoverable(frame_bb, id);

    if (!display_format)
        display_format = "%.3f";
    int decimal_precision = ParseFormatPrecision(display_format, 3);

    if (hovered && g.IO.MouseClicked[0])
    {
        SetActiveID(id, window);
        FocusWindow(window);
    }

    // Actual slider behavior + render grab
    bool value_changed = SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision, ImGuiSliderFlags_Vertical);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    // For the vertical slider we allow centered text to overlap the frame padding
    char value_buf[64];
    char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
    RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f,0.0f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

bool ImGui::SliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max)
{
    float v_deg = (*v_rad) * 360.0f / (2*IM_PI);
    bool value_changed = SliderFloat(label, &v_deg, v_degrees_min, v_degrees_max, "%.0f deg", 1.0f);
    *v_rad = v_deg * (2*IM_PI) / 360.0f;
    return value_changed;
}

bool ImGui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format)
{
    if (!display_format)
        display_format = "%.0f";
    float v_f = (float)*v;
    bool value_changed = SliderFloat(label, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
    *v = (int)v_f;
    return value_changed;
}

bool ImGui::VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* display_format)
{
    if (!display_format)
        display_format = "%.0f";
    float v_f = (float)*v;
    bool value_changed = VSliderFloat(label, size, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
    *v = (int)v_f;
    return value_changed;
}

// Add multiple sliders on 1 line for compact edition of multiple components
bool ImGui::SliderFloatN(const char* label, float* v, int components, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        value_changed |= SliderFloat("##v", &v[i], v_min, v_max, display_format, power);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* display_format, float power)
{
    return SliderFloatN(label, v, 2, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* display_format, float power)
{
    return SliderFloatN(label, v, 3, v_min, v_max, display_format, power);
}

bool ImGui::SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* display_format, float power)
{
    return SliderFloatN(label, v, 4, v_min, v_max, display_format, power);
}

bool ImGui::SliderIntN(const char* label, int* v, int components, int v_min, int v_max, const char* display_format)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        value_changed |= SliderInt("##v", &v[i], v_min, v_max, display_format);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* display_format)
{
    return SliderIntN(label, v, 2, v_min, v_max, display_format);
}

bool ImGui::SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* display_format)
{
    return SliderIntN(label, v, 3, v_min, v_max, display_format);
}

bool ImGui::SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* display_format)
{
    return SliderIntN(label, v, 4, v_min, v_max, display_format);
}

bool ImGui::DragBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_speed, float v_min, float v_max, int decimal_precision, float power)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    bool value_changed = false;

    // Process clicking on the drag
    if (g.ActiveId == id)
    {
        if (g.IO.MouseDown[0])
        {
            if (g.ActiveIdIsJustActivated)
            {
                // Lock current value on click
                g.DragCurrentValue = *v;
                g.DragLastMouseDelta = ImVec2(0.f, 0.f);
            }

            if (v_speed == 0.0f && (v_max - v_min) != 0.0f && (v_max - v_min) < FLT_MAX)
                v_speed = (v_max - v_min) * g.DragSpeedDefaultRatio;

            float v_cur = g.DragCurrentValue;
            const ImVec2 mouse_drag_delta = GetMouseDragDelta(0, 1.0f);
            float adjust_delta = 0.0f;
            //if (g.ActiveIdSource == ImGuiInputSource_Mouse)
            {
                adjust_delta = mouse_drag_delta.x - g.DragLastMouseDelta.x;
                if (g.IO.KeyShift && g.DragSpeedScaleFast >= 0.0f)
                    adjust_delta *= g.DragSpeedScaleFast;
                if (g.IO.KeyAlt && g.DragSpeedScaleSlow >= 0.0f)
                    adjust_delta *= g.DragSpeedScaleSlow;
            }
            adjust_delta *= v_speed;
            g.DragLastMouseDelta.x = mouse_drag_delta.x;

            if (fabsf(adjust_delta) > 0.0f)
            {
                if (fabsf(power - 1.0f) > 0.001f)
                {
                    // Logarithmic curve on both side of 0.0
                    float v0_abs = v_cur >= 0.0f ? v_cur : -v_cur;
                    float v0_sign = v_cur >= 0.0f ? 1.0f : -1.0f;
                    float v1 = powf(v0_abs, 1.0f / power) + (adjust_delta * v0_sign);
                    float v1_abs = v1 >= 0.0f ? v1 : -v1;
                    float v1_sign = v1 >= 0.0f ? 1.0f : -1.0f;          // Crossed sign line
                    v_cur = powf(v1_abs, power) * v0_sign * v1_sign;    // Reapply sign
                }
                else
                {
                    v_cur += adjust_delta;
                }

                // Clamp
                if (v_min < v_max)
                    v_cur = ImClamp(v_cur, v_min, v_max);
                g.DragCurrentValue = v_cur;
            }

            // Round to user desired precision, then apply
            v_cur = RoundScalar(v_cur, decimal_precision);
            if (*v != v_cur)
            {
                *v = v_cur;
                value_changed = true;
            }
        }
        else
        {
            ClearActiveID();
        }
    }

    return value_changed;
}

bool ImGui::DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    // NB- we don't call ItemSize() yet because we may turn into a text edit box below
    if (!ItemAdd(total_bb, id))
    {
        ItemSize(total_bb, style.FramePadding.y);
        return false;
    }
    const bool hovered = ItemHoverable(frame_bb, id);

    if (!display_format)
        display_format = "%.3f";
    int decimal_precision = ParseFormatPrecision(display_format, 3);

    // Tabbing or CTRL-clicking on Drag turns it into an input box
    bool start_text_input = false;
    const bool tab_focus_requested = FocusableItemRegister(window, id);
    if (tab_focus_requested || (hovered && (g.IO.MouseClicked[0] || g.IO.MouseDoubleClicked[0])))
    {
        SetActiveID(id, window);
        FocusWindow(window);
        if (tab_focus_requested || g.IO.KeyCtrl || g.IO.MouseDoubleClicked[0])
        {
            start_text_input = true;
            g.ScalarAsInputTextId = 0;
        }
    }
    if (start_text_input || (g.ActiveId == id && g.ScalarAsInputTextId == id))
        return InputScalarAsWidgetReplacement(frame_bb, label, ImGuiDataType_Float, v, id, decimal_precision);

    // Actual drag behavior
    ItemSize(total_bb, style.FramePadding.y);
    const bool value_changed = DragBehavior(frame_bb, id, v, v_speed, v_min, v_max, decimal_precision, power);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f,0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);

    return value_changed;
}

bool ImGui::DragFloatN(const char* label, float* v, int components, float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        value_changed |= DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format, power);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    return DragFloatN(label, v, 2, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    return DragFloatN(label, v, 3, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* display_format, float power)
{
    return DragFloatN(label, v, 4, v_speed, v_min, v_max, display_format, power);
}

bool ImGui::DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed, float v_min, float v_max, const char* display_format, const char* display_format_max, float power)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2);

    bool value_changed = DragFloat("##min", v_current_min, v_speed, (v_min >= v_max) ? -FLT_MAX : v_min, (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max), display_format, power);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragFloat("##max", v_current_max, v_speed, (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min), (v_min >= v_max) ? FLT_MAX : v_max, display_format_max ? display_format_max : display_format, power);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();
    PopID();

    return value_changed;
}

// NB: v_speed is float to allow adjusting the drag speed with more precision
bool ImGui::DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* display_format)
{
    if (!display_format)
        display_format = "%.0f";
    float v_f = (float)*v;
    bool value_changed = DragFloat(label, &v_f, v_speed, (float)v_min, (float)v_max, display_format);
    *v = (int)v_f;
    return value_changed;
}

bool ImGui::DragIntN(const char* label, int* v, int components, float v_speed, int v_min, int v_max, const char* display_format)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        value_changed |= DragInt("##v", &v[i], v_speed, v_min, v_max, display_format);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* display_format)
{
    return DragIntN(label, v, 2, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* display_format)
{
    return DragIntN(label, v, 3, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* display_format)
{
    return DragIntN(label, v, 4, v_speed, v_min, v_max, display_format);
}

bool ImGui::DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed, int v_min, int v_max, const char* display_format, const char* display_format_max)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2);

    bool value_changed = DragInt("##min", v_current_min, v_speed, (v_min >= v_max) ? INT_MIN : v_min, (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max), display_format);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragInt("##max", v_current_max, v_speed, (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min), (v_min >= v_max) ? INT_MAX : v_max, display_format_max ? display_format_max : display_format);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();
    PopID();

    return value_changed;
}

void ImGui::PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    if (graph_size.x == 0.0f)
        graph_size.x = CalcItemWidth();
    if (graph_size.y == 0.0f)
        graph_size.y = label_size.y + (style.FramePadding.y * 2);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size.x, graph_size.y));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0))
        return;
    const bool hovered = ItemHoverable(inner_bb, 0);

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data, i);
            v_min = ImMin(v_min, v);
            v_max = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    if (values_count > 0)
    {
        int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

        // Tooltip on hover
        int v_hovered = -1;
        if (hovered)
        {
            const float t = ImClamp((g.IO.MousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
            const int v_idx = (int)(t * item_count);
            IM_ASSERT(v_idx >= 0 && v_idx < values_count);

            const float v0 = values_getter(data, (v_idx + values_offset) % values_count);
            const float v1 = values_getter(data, (v_idx + 1 + values_offset) % values_count);
            if (plot_type == ImGuiPlotType_Lines)
                SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx+1, v1);
            else if (plot_type == ImGuiPlotType_Histogram)
                SetTooltip("%d: %8.4g", v_idx, v0);
            v_hovered = v_idx;
        }

        const float t_step = 1.0f / (float)res_w;

        float v0 = values_getter(data, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) / (scale_max - scale_min)) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min / (scale_max - scale_min)) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLines : ImGuiCol_PlotHistogram);
        const ImU32 col_hovered = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLinesHovered : ImGuiCol_PlotHistogramHovered);

        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) / (scale_max - scale_min)) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, (plot_type == ImGuiPlotType_Lines) ? tp1 : ImVec2(tp1.x, histogram_zero_line_t));
            if (plot_type == ImGuiPlotType_Lines)
            {
                window->DrawList->AddLine(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            }
            else if (plot_type == ImGuiPlotType_Histogram)
            {
                if (pos1.x >= pos0.x + 2.0f)
                    pos1.x -= 1.0f;
                window->DrawList->AddRectFilled(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            }

            t0 = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    if (overlay_text)
        RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f,0.0f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct ImGuiPlotArrayGetterData
{
    const float* Values;
    int Stride;

    ImGuiPlotArrayGetterData(const float* values, int stride) { Values = values; Stride = stride; }
};

static float Plot_ArrayGetter(void* data, int idx)
{
    ImGuiPlotArrayGetterData* plot_data = (ImGuiPlotArrayGetterData*)data;
    const float v = *(float*)(void*)((unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
    return v;
}

void ImGui::PlotLines(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
    ImGuiPlotArrayGetterData data(values, stride);
    PlotEx(ImGuiPlotType_Lines, label, &Plot_ArrayGetter, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    PlotEx(ImGuiPlotType_Lines, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotHistogram(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
    ImGuiPlotArrayGetterData data(values, stride);
    PlotEx(ImGuiPlotType_Histogram, label, &Plot_ArrayGetter, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void ImGui::PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    PlotEx(ImGuiPlotType_Histogram, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

// size_arg (for each axis) < 0.0f: align to end, 0.0f: auto, > 0.0f: specified size
void ImGui::ProgressBar(float fraction, const ImVec2& size_arg, const char* overlay)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, pos + CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y*2.0f));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    fraction = ImSaturate(fraction);
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    bb.Expand(ImVec2(-window->BorderSize, -window->BorderSize));
    const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
    RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(ImGuiCol_PlotHistogram), 0.0f, fraction, style.FrameRounding);

    // Default displaying the fraction as percentage string, but user can override it
    char overlay_buf[32];
    if (!overlay)
    {
        ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction*100+0.01f);
        overlay = overlay_buf;
    }

    ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f)
        RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y), bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f,0.5f), &bb);
}

bool ImGui::Checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y*2, label_size.y + style.FramePadding.y*2)); // We want a square shape to we use Y twice
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    if (label_size.x > 0)
        SameLine(0, style.ItemInnerSpacing.x);
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0,style.FramePadding.y), window->DC.CursorPos + ImVec2(0,style.FramePadding.y) + label_size);
    if (label_size.x > 0)
    {
        ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
        total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
    }

    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        *v = !(*v);

    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
    if (*v)
    {
        const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
        const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
        RenderCheckMark(check_bb.Min + ImVec2(pad,pad), GetColorU32(ImGuiCol_CheckMark), check_bb.GetWidth() - pad*2.0f);
    }

    if (g.LogEnabled)
        LogRenderedText(&text_bb.Min, *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f)
        RenderText(text_bb.Min, label);

    return pressed;
}

bool ImGui::CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value)
{
    bool v = ((*flags & flags_value) == flags_value);
    bool pressed = Checkbox(label, &v);
    if (pressed)
    {
        if (v)
            *flags |= flags_value;
        else
            *flags &= ~flags_value;
    }

    return pressed;
}

bool ImGui::RadioButton(const char* label, bool active)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y*2-1, label_size.y + style.FramePadding.y*2-1));
    ItemSize(check_bb, style.FramePadding.y);

    ImRect total_bb = check_bb;
    if (label_size.x > 0)
        SameLine(0, style.ItemInnerSpacing.x);
    const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
    if (label_size.x > 0)
    {
        ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
        total_bb.Add(text_bb);
    }

    if (!ItemAdd(total_bb, id))
        return false;

    ImVec2 center = check_bb.GetCenter();
    center.x = (float)(int)center.x + 0.5f;
    center.y = (float)(int)center.y + 0.5f;
    const float radius = check_bb.GetHeight() * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    if (active)
    {
        const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
        const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
        window->DrawList->AddCircleFilled(center, radius-pad, GetColorU32(ImGuiCol_CheckMark), 16);
    }

    if (window->Flags & ImGuiWindowFlags_ShowBorders)
    {
        window->DrawList->AddCircle(center+ImVec2(1,1), radius, GetColorU32(ImGuiCol_BorderShadow), 16);
        window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border), 16);
    }

    if (g.LogEnabled)
        LogRenderedText(&text_bb.Min, active ? "(x)" : "( )");
    if (label_size.x > 0.0f)
        RenderText(text_bb.Min, label);

    return pressed;
}

bool ImGui::RadioButton(const char* label, int* v, int v_button)
{
    const bool pressed = RadioButton(label, *v == v_button);
    if (pressed)
    {
        *v = v_button;
    }
    return pressed;
}

static int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end)
{
    int line_count = 0;
    const char* s = text_begin;
    while (char c = *s++) // We are only matching for \n so we can ignore UTF-8 decoding
        if (c == '\n')
            line_count++;
    s--;
    if (s[0] != '\n' && s[0] != '\r')
        line_count++;
    *out_text_end = s;
    return line_count;
}

static ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line)
{
    ImFont* font = GImGui->Font;
    const float line_height = GImGui->FontSize;
    const float scale = line_height / font->FontSize;

    ImVec2 text_size = ImVec2(0,0);
    float line_width = 0.0f;

    const ImWchar* s = text_begin;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)(*s++);
        if (c == '\n')
        {
            text_size.x = ImMax(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0.0f;
            if (stop_on_new_line)
                break;
            continue;
        }
        if (c == '\r')
            continue;

        const float char_width = font->GetCharAdvance((unsigned short)c) * scale;
        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (out_offset)
        *out_offset = ImVec2(line_width, text_size.y + line_height);  // offset allow for the possibility of sitting after a trailing \n

    if (line_width > 0 || text_size.y == 0.0f)                        // whereas size.y will ignore the trailing \n
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

// Wrapper for stb_textedit.h to edit text (our wrapper is for: statically sized buffer, single-line, wchar characters. InputText converts between UTF-8 and wchar)
namespace ImGuiStb
{

static int     STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING* obj)                             { return obj->CurLenW; }
static ImWchar STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING* obj, int idx)                      { return obj->Text[idx]; }
static float   STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING* obj, int line_start_idx, int char_idx)  { ImWchar c = obj->Text[line_start_idx+char_idx]; if (c == '\n') return STB_TEXTEDIT_GETWIDTH_NEWLINE; return GImGui->Font->GetCharAdvance(c) * (GImGui->FontSize / GImGui->Font->FontSize); }
static int     STB_TEXTEDIT_KEYTOTEXT(int key)                                                    { return key >= 0x10000 ? 0 : key; }
static ImWchar STB_TEXTEDIT_NEWLINE = '\n';
static void    STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, STB_TEXTEDIT_STRING* obj, int line_start_idx)
{
    const ImWchar* text = obj->Text.Data;
    const ImWchar* text_remaining = NULL;
    const ImVec2 size = InputTextCalcTextSizeW(text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
    r->x0 = 0.0f;
    r->x1 = size.x;
    r->baseline_y_delta = size.y;
    r->ymin = 0.0f;
    r->ymax = size.y;
    r->num_chars = (int)(text_remaining - (text + line_start_idx));
}

static bool is_separator(unsigned int c)                                        { return ImCharIsSpace(c) || c==',' || c==';' || c=='(' || c==')' || c=='{' || c=='}' || c=='[' || c==']' || c=='|'; }
static int  is_word_boundary_from_right(STB_TEXTEDIT_STRING* obj, int idx)      { return idx > 0 ? (is_separator( obj->Text[idx-1] ) && !is_separator( obj->Text[idx] ) ) : 1; }
static int  STB_TEXTEDIT_MOVEWORDLEFT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)   { idx--; while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--; return idx < 0 ? 0 : idx; }
#ifdef __APPLE__    // FIXME: Move setting to IO structure
static int  is_word_boundary_from_left(STB_TEXTEDIT_STRING* obj, int idx)       { return idx > 0 ? (!is_separator( obj->Text[idx-1] ) && is_separator( obj->Text[idx] ) ) : 1; }
static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)  { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_left(obj, idx)) idx++; return idx > len ? len : idx; }
#else
static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)  { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++; return idx > len ? len : idx; }
#endif
#define STB_TEXTEDIT_MOVEWORDLEFT   STB_TEXTEDIT_MOVEWORDLEFT_IMPL    // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING* obj, int pos, int n)
{
    ImWchar* dst = obj->Text.Data + pos;

    // We maintain our buffer length in both UTF-8 and wchar formats
    obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
    obj->CurLenW -= n;

    // Offset remaining text
    const ImWchar* src = obj->Text.Data + pos + n;
    while (ImWchar c = *src++)
        *dst++ = c;
    *dst = '\0';
}

static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING* obj, int pos, const ImWchar* new_text, int new_text_len)
{
    const int text_len = obj->CurLenW;
    IM_ASSERT(pos <= text_len);
    if (new_text_len + text_len + 1 > obj->Text.Size)
        return false;

    const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
    if (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufSizeA)
        return false;

    ImWchar* text = obj->Text.Data;
    if (pos != text_len)
        memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
    memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

    obj->CurLenW += new_text_len;
    obj->CurLenA += new_text_len_utf8;
    obj->Text[obj->CurLenW] = '\0';

    return true;
}

// We don't use an enum so we can build even with conflicting symbols (if another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT         0x10000 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT        0x10001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP           0x10002 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN         0x10003 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART    0x10004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND      0x10005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART    0x10006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND      0x10007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE       0x10008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE    0x10009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO         0x1000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO         0x1000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT     0x1000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT    0x1000D // keyboard input to move cursor right one word
#define STB_TEXTEDIT_K_SHIFT        0x20000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "stb_textedit.h"

}

void ImGuiTextEditState::OnKeyPressed(int key)
{
    stb_textedit_key(this, &StbState, key);
    CursorFollow = true;
    CursorAnimReset();
}

// Public API to manipulate UTF-8 text
// We expose UTF-8 to the user (unlike the STB_TEXTEDIT_* functions which are manipulating wchar)
// FIXME: The existence of this rarely exercised code path is a bit of a nuisance.
void ImGuiTextEditCallbackData::DeleteChars(int pos, int bytes_count)
{
    IM_ASSERT(pos + bytes_count <= BufTextLen);
    char* dst = Buf + pos;
    const char* src = Buf + pos + bytes_count;
    while (char c = *src++)
        *dst++ = c;
    *dst = '\0';

    if (CursorPos + bytes_count >= pos)
        CursorPos -= bytes_count;
    else if (CursorPos >= pos)
        CursorPos = pos;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty = true;
    BufTextLen -= bytes_count;
}

void ImGuiTextEditCallbackData::InsertChars(int pos, const char* new_text, const char* new_text_end)
{
    const int new_text_len = new_text_end ? (int)(new_text_end - new_text) : (int)strlen(new_text);
    if (new_text_len + BufTextLen + 1 >= BufSize)
        return;

    if (BufTextLen != pos)
        memmove(Buf + pos + new_text_len, Buf + pos, (size_t)(BufTextLen - pos));
    memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
    Buf[BufTextLen + new_text_len] = '\0';

    if (CursorPos >= pos)
        CursorPos += new_text_len;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty = true;
    BufTextLen += new_text_len;
}

// Return false to discard a character.
static bool InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    unsigned int c = *p_char;

    if (c < 128 && c != ' ' && !isprint((int)(c & 0xFF)))
    {
        bool pass = false;
        pass |= (c == '\n' && (flags & ImGuiInputTextFlags_Multiline));
        pass |= (c == '\t' && (flags & ImGuiInputTextFlags_AllowTabInput));
        if (!pass)
            return false;
    }

    if (c >= 0xE000 && c <= 0xF8FF) // Filter private Unicode range. I don't imagine anybody would want to input them. GLFW on OSX seems to send private characters for special keys like arrow keys.
        return false;

    if (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank))
    {
        if (flags & ImGuiInputTextFlags_CharsDecimal)
            if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') && (c != '*') && (c != '/'))
                return false;

        if (flags & ImGuiInputTextFlags_CharsHexadecimal)
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F'))
                return false;

        if (flags & ImGuiInputTextFlags_CharsUppercase)
            if (c >= 'a' && c <= 'z')
                *p_char = (c += (unsigned int)('A'-'a'));

        if (flags & ImGuiInputTextFlags_CharsNoBlank)
            if (ImCharIsSpace(c))
                return false;
    }

    if (flags & ImGuiInputTextFlags_CallbackCharFilter)
    {
        ImGuiTextEditCallbackData callback_data;
        memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
        callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
        callback_data.EventChar = (ImWchar)c;
        callback_data.Flags = flags;
        callback_data.UserData = user_data;
        if (callback(&callback_data) != 0)
            return false;
        *p_char = callback_data.EventChar;
        if (!callback_data.EventChar)
            return false;
    }

    return true;
}

// Edit a string of text
// NB: when active, hold on a privately held copy of the text (and apply back to 'buf'). So changing 'buf' while active has no effect.
// FIXME: Rather messy function partly because we are doing UTF8 > u16 > UTF8 conversions on the go to more easily handle stb_textedit calls. Ideally we should stay in UTF-8 all the time. See https://github.com/nothings/stb/issues/188
bool ImGui::InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline))); // Can't use both together (they both use up/down keys)
    IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) && (flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

    ImGuiContext& g = *GImGui;
    const ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
    const bool is_editable = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
    const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;

    if (is_multiline) // Open group before calling GetID() because groups tracks id created during their spawn
        BeginGroup();
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? GetTextLineHeight() * 8.0f : label_size.y) + style.FramePadding.y*2.0f); // Arbitrary default of 8 lines high for multi-line
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

    ImGuiWindow* draw_window = window;
    if (is_multiline)
    {
        if (!BeginChildFrame(id, frame_bb.GetSize()))
        {
            EndChildFrame();
            EndGroup();
            return false;
        }
        draw_window = GetCurrentWindow();
        size.x -= draw_window->ScrollbarSizes.x;
    }
    else
    {
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id))
            return false;
    }
    const bool hovered = ItemHoverable(frame_bb, id);
    if (hovered)
        g.MouseCursor = ImGuiMouseCursor_TextInput;

    // Password pushes a temporary font with only a fallback glyph
    if (is_password)
    {
        const ImFontGlyph* glyph = g.Font->FindGlyph('*');
        ImFont* password_font = &g.InputTextPasswordFont;
        password_font->FontSize = g.Font->FontSize;
        password_font->Scale = g.Font->Scale;
        password_font->DisplayOffset = g.Font->DisplayOffset;
        password_font->Ascent = g.Font->Ascent;
        password_font->Descent = g.Font->Descent;
        password_font->ContainerAtlas = g.Font->ContainerAtlas;
        password_font->FallbackGlyph = glyph;
        password_font->FallbackAdvanceX = glyph->AdvanceX;
        IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
        PushFont(password_font);
    }

    // NB: we are only allowed to access 'edit_state' if we are the active widget.
    ImGuiTextEditState& edit_state = g.InputTextState;

    const bool focus_requested = FocusableItemRegister(window, id, (flags & (ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_AllowTabInput)) == 0);    // Using completion callback disable keyboard tabbing
    const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
    const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

    const bool user_clicked = hovered && io.MouseClicked[0];
    const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id && g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");

    bool clear_active_id = false;

    bool select_all = (g.ActiveId != id) && (flags & ImGuiInputTextFlags_AutoSelectAll) != 0;
    if (focus_requested || user_clicked || user_scrolled)
    {
        if (g.ActiveId != id)
        {
            // Start edition
            // Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
            // From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
            const int prev_len_w = edit_state.CurLenW;
            edit_state.Text.resize(buf_size+1);        // wchar count <= UTF-8 count. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
            edit_state.InitialText.resize(buf_size+1); // UTF-8. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
            ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
            const char* buf_end = NULL;
            edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
            edit_state.CurLenA = (int)(buf_end - buf); // We can't get the result from ImFormatString() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.
            edit_state.CursorAnimReset();

            // Preserve cursor position and undo/redo stack if we come back to same widget
            // FIXME: We should probably compare the whole buffer to be on the safety side. Comparing buf (utf8) and edit_state.Text (wchar).
            const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
            if (recycle_state)
            {
                // Recycle existing cursor/selection/undo stack but clamp position
                // Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
                edit_state.CursorClamp();
            }
            else
            {
                edit_state.Id = id;
                edit_state.ScrollX = 0.0f;
                stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
                if (!is_multiline && focus_requested_by_code)
                    select_all = true;
            }
            if (flags & ImGuiInputTextFlags_AlwaysInsertMode)
                edit_state.StbState.insert_mode = true;
            if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
                select_all = true;
        }
        SetActiveID(id, window);
        FocusWindow(window);
    }
    else if (io.MouseClicked[0])
    {
        // Release focus when we click outside
        clear_active_id = true;
    }

    bool value_changed = false;
    bool enter_pressed = false;

    if (g.ActiveId == id)
    {
        if (!is_editable && !g.ActiveIdIsJustActivated)
        {
            // When read-only we always use the live data passed to the function
            edit_state.Text.resize(buf_size+1);
            const char* buf_end = NULL;
            edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
            edit_state.CurLenA = (int)(buf_end - buf);
            edit_state.CursorClamp();
        }

        edit_state.BufSizeA = buf_size;

        // Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
        // Down the line we should have a cleaner library-wide concept of Selected vs Active.
        g.ActiveIdAllowOverlap = !io.MouseDown[0];
        g.WantTextInputNextFrame = 1;

        // Edit in progress
        const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
        const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize*0.5f));

        const bool osx_double_click_selects_words = io.OSXBehaviors;      // OS X style: Double click selects by word instead of selecting whole text
        if (select_all || (hovered && !osx_double_click_selects_words && io.MouseDoubleClicked[0]))
        {
            edit_state.SelectAll();
            edit_state.SelectedAllMouseLock = true;
        }
        else if (hovered && osx_double_click_selects_words && io.MouseDoubleClicked[0])
        {
            // Select a word only, OS X style (by simulating keystrokes)
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
        }
        else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
        {
            stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
            edit_state.CursorAnimReset();
        }
        else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
            edit_state.CursorAnimReset();
            edit_state.CursorFollow = true;
        }
        if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
            edit_state.SelectedAllMouseLock = false;

        if (io.InputCharacters[0])
        {
            // Process text input (before we check for Return because using some IME will effectively send a Return?)
            // We ignore CTRL inputs, but need to allow CTRL+ALT as some keyboards (e.g. German) use AltGR - which is Alt+Ctrl - to input certain characters.
            if (!(io.KeyCtrl && !io.KeyAlt) && is_editable)
            {
                for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
                    if (unsigned int c = (unsigned int)io.InputCharacters[n])
                    {
                        // Insert character if they pass filtering
                        if (!InputTextFilterCharacter(&c, flags, callback, user_data))
                            continue;
                        edit_state.OnKeyPressed((int)c);
                    }
            }

            // Consume characters
            memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
        }
    }

    bool cancel_edit = false;
    if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
    {
        // Handle key-presses
        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_shortcut_key_only = (io.OSXBehaviors ? (io.KeySuper && !io.KeyCtrl) : (io.KeyCtrl && !io.KeySuper)) && !io.KeyAlt && !io.KeyShift; // OS X style: Shortcuts using Cmd/Super instead of Ctrl
        const bool is_wordmove_key_down = io.OSXBehaviors ? io.KeyAlt : io.KeyCtrl;                     // OS X style: Text editing cursor movement using Alt instead of Ctrl
        const bool is_startend_key_down = io.OSXBehaviors && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End

        if (IsKeyPressedMap(ImGuiKey_LeftArrow))                        { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_RightArrow))                  { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline)     { if (io.KeyCtrl) SetWindowScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline)   { if (io.KeyCtrl) SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY())); else edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_Home))                        { edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_End))                         { edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable)       { edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
        else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)
        {
            if (!edit_state.HasSelection())
            {
                if (is_wordmove_key_down) edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT|STB_TEXTEDIT_K_SHIFT);
                else if (io.OSXBehaviors && io.KeySuper && !io.KeyAlt && !io.KeyCtrl) edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART|STB_TEXTEDIT_K_SHIFT);
            }
            edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        }
        else if (IsKeyPressedMap(ImGuiKey_Enter))
        {
            bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
            {
                enter_pressed = clear_active_id = true;
            }
            else if (is_editable)
            {
                unsigned int c = '\n'; // Insert new line
                if (InputTextFilterCharacter(&c, flags, callback, user_data))
                    edit_state.OnKeyPressed((int)c);
            }
        }
        else if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl && !io.KeyShift && !io.KeyAlt && is_editable)
        {
            unsigned int c = '\t'; // Insert TAB
            if (InputTextFilterCharacter(&c, flags, callback, user_data))
                edit_state.OnKeyPressed((int)c);
        }
        else if (IsKeyPressedMap(ImGuiKey_Escape))                                      { clear_active_id = cancel_edit = true; }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Z) && is_editable)    { edit_state.OnKeyPressed(STB_TEXTEDIT_K_UNDO); edit_state.ClearSelection(); }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_Y) && is_editable)    { edit_state.OnKeyPressed(STB_TEXTEDIT_K_REDO); edit_state.ClearSelection(); }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_A))                   { edit_state.SelectAll(); edit_state.CursorFollow = true; }
        else if (is_shortcut_key_only && !is_password && ((IsKeyPressedMap(ImGuiKey_X) && is_editable) || IsKeyPressedMap(ImGuiKey_C)) && (!is_multiline || edit_state.HasSelection()))
        {
            // Cut, Copy
            const bool cut = IsKeyPressedMap(ImGuiKey_X);
            if (cut && !edit_state.HasSelection())
                edit_state.SelectAll();

            if (io.SetClipboardTextFn)
            {
                const int ib = edit_state.HasSelection() ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end) : 0;
                const int ie = edit_state.HasSelection() ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end) : edit_state.CurLenW;
                edit_state.TempTextBuffer.resize((ie-ib) * 4 + 1);
                ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data+ib, edit_state.Text.Data+ie);
                SetClipboardText(edit_state.TempTextBuffer.Data);
            }

            if (cut)
            {
                edit_state.CursorFollow = true;
                stb_textedit_cut(&edit_state, &edit_state.StbState);
            }
        }
        else if (is_shortcut_key_only && IsKeyPressedMap(ImGuiKey_V) && is_editable)
        {
            // Paste
            if (const char* clipboard = GetClipboardText())
            {
                // Filter pasted buffer
                const int clipboard_len = (int)strlen(clipboard);
                ImWchar* clipboard_filtered = (ImWchar*)ImGui::MemAlloc((clipboard_len+1) * sizeof(ImWchar));
                int clipboard_filtered_len = 0;
                for (const char* s = clipboard; *s; )
                {
                    unsigned int c;
                    s += ImTextCharFromUtf8(&c, s, NULL);
                    if (c == 0)
                        break;
                    if (c >= 0x10000 || !InputTextFilterCharacter(&c, flags, callback, user_data))
                        continue;
                    clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                }
                clipboard_filtered[clipboard_filtered_len] = 0;
                if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
                {
                    stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
                    edit_state.CursorFollow = true;
                }
                ImGui::MemFree(clipboard_filtered);
            }
        }
    }

    if (g.ActiveId == id)
    {
        if (cancel_edit)
        {
            // Restore initial value
            if (is_editable)
            {
                ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
                value_changed = true;
            }
        }

        // When using 'ImGuiInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
        // If we didn't do that, code like InputInt() with ImGuiInputTextFlags_EnterReturnsTrue would fail. Also this allows the user to use InputText() with ImGuiInputTextFlags_EnterReturnsTrue without maintaining any user-side storage.
        bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0);
        if (apply_edit_back_to_user_buffer)
        {
            // Apply new value immediately - copy modified buffer back
            // Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
            // FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
            // FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
            if (is_editable)
            {
                edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
                ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
            }

            // User callback
            if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways)) != 0)
            {
                IM_ASSERT(callback != NULL);

                // The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
                ImGuiInputTextFlags event_flag = 0;
                ImGuiKey event_key = ImGuiKey_COUNT;
                if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab))
                {
                    event_flag = ImGuiInputTextFlags_CallbackCompletion;
                    event_key = ImGuiKey_Tab;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
                {
                    event_flag = ImGuiInputTextFlags_CallbackHistory;
                    event_key = ImGuiKey_UpArrow;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
                {
                    event_flag = ImGuiInputTextFlags_CallbackHistory;
                    event_key = ImGuiKey_DownArrow;
                }
                else if (flags & ImGuiInputTextFlags_CallbackAlways)
                    event_flag = ImGuiInputTextFlags_CallbackAlways;

                if (event_flag)
                {
                    ImGuiTextEditCallbackData callback_data;
                    memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags = flags;
                    callback_data.UserData = user_data;
                    callback_data.ReadOnly = !is_editable;

                    callback_data.EventKey = event_key;
                    callback_data.Buf = edit_state.TempTextBuffer.Data;
                    callback_data.BufTextLen = edit_state.CurLenA;
                    callback_data.BufSize = edit_state.BufSizeA;
                    callback_data.BufDirty = false;

                    // We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
                    ImWchar* text = edit_state.Text.Data;
                    const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
                    const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
                    const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

                    // Call user code
                    callback(&callback_data);

                    // Read back what user may have modified
                    IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data);  // Invalid to modify those fields
                    IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
                    IM_ASSERT(callback_data.Flags == flags);
                    if (callback_data.CursorPos != utf8_cursor_pos)            edit_state.StbState.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
                    if (callback_data.SelectionStart != utf8_selection_start)  edit_state.StbState.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
                    if (callback_data.SelectionEnd != utf8_selection_end)      edit_state.StbState.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
                    if (callback_data.BufDirty)
                    {
                        IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
                        edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, callback_data.Buf, NULL);
                        edit_state.CurLenA = callback_data.BufTextLen;  // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
                        edit_state.CursorAnimReset();
                    }
                }
            }

            // Copy back to user buffer
            if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0)
            {
                ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
                value_changed = true;
            }
        }
    }

    // Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
    if (clear_active_id && g.ActiveId == id)
        ClearActiveID();

    // Render
    // Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value. We set buf to NULL to prevent accidental usage from now on.
    const char* buf_display = (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf; buf = NULL; 

    if (!is_multiline)
        RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
    ImVec2 text_size(0.f, 0.f);
    const bool is_currently_scrolling = (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
    if (g.ActiveId == id || is_currently_scrolling)
    {
        edit_state.CursorAnim += io.DeltaTime;

        // This is going to be messy. We need to:
        // - Display the text (this alone can be more easily clipped)
        // - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
        // - Measure text height (for scrollbar)
        // We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
        // FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
        const ImWchar* text_begin = edit_state.Text.Data;
        ImVec2 cursor_offset, select_start_offset;

        {
            // Count lines + find lines numbers straddling 'cursor' and 'select_start' position.
            const ImWchar* searches_input_ptr[2];
            searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
            searches_input_ptr[1] = NULL;
            int searches_remaining = 1;
            int searches_result_line_number[2] = { -1, -999 };
            if (edit_state.StbState.select_start != edit_state.StbState.select_end)
            {
                searches_input_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
                searches_result_line_number[1] = -1;
                searches_remaining++;
            }

            // Iterate all lines to find our line numbers
            // In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
            searches_remaining += is_multiline ? 1 : 0;
            int line_count = 0;
            for (const ImWchar* s = text_begin; *s != 0; s++)
                if (*s == '\n')
                {
                    line_count++;
                    if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_number[0] = line_count; if (--searches_remaining <= 0) break; }
                    if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_number[1] = line_count; if (--searches_remaining <= 0) break; }
                }
            line_count++;
            if (searches_result_line_number[0] == -1) searches_result_line_number[0] = line_count;
            if (searches_result_line_number[1] == -1) searches_result_line_number[1] = line_count;

            // Calculate 2d position by finding the beginning of the line and measuring distance
            cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
            cursor_offset.y = searches_result_line_number[0] * g.FontSize;
            if (searches_result_line_number[1] >= 0)
            {
                select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
                select_start_offset.y = searches_result_line_number[1] * g.FontSize;
            }

            // Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
            if (is_multiline)
                text_size = ImVec2(size.x, line_count * g.FontSize);
        }

        // Scroll
        if (edit_state.CursorFollow)
        {
            // Horizontal scroll in chunks of quarter width
            if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
            {
                const float scroll_increment_x = size.x * 0.25f;
                if (cursor_offset.x < edit_state.ScrollX)
                    edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
                else if (cursor_offset.x - size.x >= edit_state.ScrollX)
                    edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
            }
            else
            {
                edit_state.ScrollX = 0.0f;
            }

            // Vertical scroll
            if (is_multiline)
            {
                float scroll_y = draw_window->Scroll.y;
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y - size.y >= scroll_y)
                    scroll_y = cursor_offset.y - size.y;
                draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y);   // To avoid a frame of lag
                draw_window->Scroll.y = scroll_y;
                render_pos.y = draw_window->DC.CursorPos.y;
            }
        }
        edit_state.CursorFollow = false;
        const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

        // Draw selection
        if (edit_state.StbState.select_start != edit_state.StbState.select_end)
        {
            const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
            const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

            float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
            float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            ImU32 bg_color = GetColorU32(ImGuiCol_TextSelectedBg);
            ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
            for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
            {
                if (rect_pos.y > clip_rect.w + g.FontSize)
                    break;
                if (rect_pos.y < clip_rect.y)
                {
                    while (p < text_selected_end)
                        if (*p++ == '\n')
                            break;
                }
                else
                {
                    ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                    if (rect_size.x <= 0.0f) rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') * 0.50f); // So we can see selected empty lines
                    ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos +ImVec2(rect_size.x, bg_offy_dn));
                    rect.ClipWith(clip_rect);
                    if (rect.Overlaps(clip_rect))
                        draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                }
                rect_pos.x = render_pos.x - render_scroll.x;
                rect_pos.y += g.FontSize;
            }
        }

        draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, GetColorU32(ImGuiCol_Text), buf_display, buf_display + edit_state.CurLenA, 0.0f, is_multiline ? NULL : &clip_rect);

        // Draw blinking cursor
        bool cursor_is_visible = (g.InputTextState.CursorAnim <= 0.0f) || fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
        ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
        ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y-g.FontSize+0.5f, cursor_screen_pos.x+1.0f, cursor_screen_pos.y-1.5f);
        if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
            draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(ImGuiCol_Text));

        // Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
        if (is_editable)
            g.OsImePosRequest = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
    }
    else
    {
        // Render text only
        const char* buf_end = NULL;
        if (is_multiline)
            text_size = ImVec2(size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) * g.FontSize); // We don't need width
        draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
    }

    if (is_multiline)
    {
        Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
        EndChildFrame();
        EndGroup();
    }

    if (is_password)
        PopFont();

    // Log as text
    if (g.LogEnabled && !is_password)
        LogRenderedText(&render_pos, buf_display, NULL);

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
        return enter_pressed;
    else
        return value_changed;
}

bool ImGui::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()
    return InputTextEx(label, buf, (int)buf_size, ImVec2(0,0), flags, callback, user_data);
}

bool ImGui::InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
    return InputTextEx(label, buf, (int)buf_size, size, flags | ImGuiInputTextFlags_Multiline, callback, user_data);
}

static inline float SmallSquareSize()
{
    ImGuiContext& g = *GImGui;
    return g.FontSize + g.Style.FramePadding.y * 2.0f;
}

// NB: scalar_format here must be a simple "%xx" format string with no prefix/suffix (unlike the Drag/Slider functions "display_format" argument)
bool ImGui::InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr, void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    BeginGroup();
    PushID(label);
    const ImVec2 button_sz = ImVec2(SmallSquareSize(), SmallSquareSize());
    if (step_ptr)
        PushItemWidth(ImMax(1.0f, CalcItemWidth() - (button_sz.x + style.ItemInnerSpacing.x)*2));

    char buf[64];
    DataTypeFormatString(data_type, data_ptr, scalar_format, buf, IM_ARRAYSIZE(buf));

    bool value_changed = false;
    if (!(extra_flags & ImGuiInputTextFlags_CharsHexadecimal))
        extra_flags |= ImGuiInputTextFlags_CharsDecimal;
    extra_flags |= ImGuiInputTextFlags_AutoSelectAll;
    if (InputText("", buf, IM_ARRAYSIZE(buf), extra_flags)) // PushId(label) + "" gives us the expected ID from outside point of view
        value_changed = DataTypeApplyOpFromText(buf, GImGui->InputTextState.InitialText.begin(), data_type, data_ptr, scalar_format);

    // Step buttons
    if (step_ptr)
    {
        PopItemWidth();
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("-", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
        {
            DataTypeApplyOp(data_type, '-', data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
            value_changed = true;
        }
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("+", button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
        {
            DataTypeApplyOp(data_type, '+', data_ptr, g.IO.KeyCtrl && step_fast_ptr ? step_fast_ptr : step_ptr);
            value_changed = true;
        }
    }
    PopID();

    if (label_size.x > 0)
    {
        SameLine(0, style.ItemInnerSpacing.x);
        RenderText(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), label);
        ItemSize(label_size, style.FramePadding.y);
    }
    EndGroup();

    return value_changed;
}

bool ImGui::InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    char display_format[16];
    if (decimal_precision < 0)
        strcpy(display_format, "%f");      // Ideally we'd have a minimum decimal precision of 1 to visually denote that this is a float, while hiding non-significant digits? %f doesn't have a minimum of 1
    else
        ImFormatString(display_format, IM_ARRAYSIZE(display_format), "%%.%df", decimal_precision);
    return InputScalarEx(label, ImGuiDataType_Float, (void*)v, (void*)(step>0.0f ? &step : NULL), (void*)(step_fast>0.0f ? &step_fast : NULL), display_format, extra_flags);
}

bool ImGui::InputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags extra_flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* scalar_format = (extra_flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return InputScalarEx(label, ImGuiDataType_Int, (void*)v, (void*)(step>0.0f ? &step : NULL), (void*)(step_fast>0.0f ? &step_fast : NULL), scalar_format, extra_flags);
}

bool ImGui::InputFloatN(const char* label, float* v, int components, int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        value_changed |= InputFloat("##v", &v[i], 0, 0, decimal_precision, extra_flags);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::InputFloat2(const char* label, float v[2], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    return InputFloatN(label, v, 2, decimal_precision, extra_flags);
}

bool ImGui::InputFloat3(const char* label, float v[3], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    return InputFloatN(label, v, 3, decimal_precision, extra_flags);
}

bool ImGui::InputFloat4(const char* label, float v[4], int decimal_precision, ImGuiInputTextFlags extra_flags)
{
    return InputFloatN(label, v, 4, decimal_precision, extra_flags);
}

bool ImGui::InputIntN(const char* label, int* v, int components, ImGuiInputTextFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components);
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        value_changed |= InputInt("##v", &v[i], 0, 0, extra_flags);
        SameLine(0, g.Style.ItemInnerSpacing.x);
        PopID();
        PopItemWidth();
    }
    PopID();

    TextUnformatted(label, FindRenderedTextEnd(label));
    EndGroup();

    return value_changed;
}

bool ImGui::InputInt2(const char* label, int v[2], ImGuiInputTextFlags extra_flags)
{
    return InputIntN(label, v, 2, extra_flags);
}

bool ImGui::InputInt3(const char* label, int v[3], ImGuiInputTextFlags extra_flags)
{
    return InputIntN(label, v, 3, extra_flags);
}

bool ImGui::InputInt4(const char* label, int v[4], ImGuiInputTextFlags extra_flags)
{
    return InputIntN(label, v, 4, extra_flags);
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}

static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p)
        return false;
    if (out_text)
        *out_text = p;
    return true;
}

// Combo box helper allowing to pass an array of strings.
bool ImGui::Combo(const char* label, int* current_item, const char* const* items, int items_count, int height_in_items)
{
    const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

// Combo box helper allowing to pass all items in a single string.
bool ImGui::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

bool ImGui::BeginCombo(const char* label, const char* preview_value, ImVec2 popup_size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    float w = CalcItemWidth(); // Returns wrong values, itemwidth is -1 for some odd reason

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    const float arrow_size = SmallSquareSize();

    bool hovered, held;
    bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);

    bool popup_open = IsPopupOpen(id);

    const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    RenderFrame(ImVec2(frame_bb.Max.x-arrow_size, frame_bb.Min.y), frame_bb.Max, GetColorU32(popup_open || hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button), true, style.FrameRounding); // FIXME-ROUNDING
    RenderTriangle(ImVec2(frame_bb.Max.x - arrow_size + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), ImGuiDir_Down);

    if (preview_value != NULL)
        RenderTextClipped(frame_bb.Min + style.FramePadding, value_bb.Max, preview_value, NULL, NULL, ImVec2(0.0f,0.0f));

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if (pressed && !popup_open)
    {
        OpenPopupEx(id, false);
        popup_open = true;
    }

    if (!popup_open)
        return false;

    if (popup_size.x == 0.0f)
        popup_size.x = w;

    float popup_y1 = frame_bb.Max.y;
    float popup_y2 = ImClamp(popup_y1 + popup_size.y, popup_y1, g.IO.DisplaySize.y - style.DisplaySafeAreaPadding.y);
    if ((popup_y2 - popup_y1) < ImMin(popup_size.y, frame_bb.Min.y - style.DisplaySafeAreaPadding.y))
    {
        // Position our combo ABOVE because there's more space to fit! (FIXME: Handle in Begin() or use a shared helper. We have similar code in Begin() for popup placement)
        popup_y1 = ImClamp(frame_bb.Min.y - popup_size.y, style.DisplaySafeAreaPadding.y, frame_bb.Min.y);
        popup_y2 = frame_bb.Min.y;
        SetNextWindowPos(ImVec2(frame_bb.Min.x, frame_bb.Min.y), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    }
    else
    {
        // Position our combo below
        SetNextWindowPos(ImVec2(frame_bb.Min.x, frame_bb.Max.y), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    }
    SetNextWindowSize(ImVec2(popup_size.x, popup_y2 - popup_y1), ImGuiCond_Appearing);
    PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);

    const ImGuiWindowFlags flags = ImGuiWindowFlags_ComboBox | ((window->Flags & ImGuiWindowFlags_ShowBorders) ? ImGuiWindowFlags_ShowBorders : 0);
    if (!BeginPopupEx(id, flags))
    {
        IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    Spacing();

    return true;
}

void ImGui::EndCombo()
{
    EndPopup();
    PopStyleVar();
}

// Combo box function.
bool ImGui::Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const char* preview_text = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_text);

    // Size default to hold ~7 items
    if (height_in_items < 0)
        height_in_items = 7;
    float popup_height = (g.FontSize + style.ItemSpacing.y) * ImMin(items_count, height_in_items) + (style.FramePadding.y * 3);

    if (!BeginCombo(label, preview_text, ImVec2(0.0f, popup_height)))
        return false;

    // Display items
    // FIXME-OPT: Use clipper
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        PushID((void*)(intptr_t)i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";
        if (Selectable(item_text, item_selected))
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected && IsWindowAppearing())
            SetScrollHere();
        PopID();
    }

    EndCombo();
    return value_changed;
}

// Tip: pass an empty label (e.g. "##dummy") then you can use the space to draw other text or image.
// But you need to make sure the ID is unique, e.g. enclose calls in PushID/PopID.
bool ImGui::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1) // FIXME-OPT: Avoid if vertically clipped.
        PopClipRect();

    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrentLineTextBaseOffset;
    ImRect bb(pos, pos + size);
    ItemSize(bb);

    // Fill horizontal space.
    ImVec2 window_padding = window->WindowPadding;
    float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? GetWindowContentRegionMax().x : GetContentRegionMax().x;
    float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
    ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
    ImRect bb_with_spacing(pos, pos + size_draw); // stupid high value from combobox, no idea how to fix w/o band-aid bs
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
        bb_with_spacing.Max.x += window_padding.x;

    // Selectables are tightly packed together, we extend the box to cover spacing between selectable.
    float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
    float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
    float spacing_R = style.ItemSpacing.x - spacing_L;
    float spacing_D = style.ItemSpacing.y - spacing_U;
    bb_with_spacing.Min.x -= spacing_L;
    bb_with_spacing.Min.y -= spacing_U;
    bb_with_spacing.Max.x += spacing_R;
    bb_with_spacing.Max.y += spacing_D;
    if (!ItemAdd(bb_with_spacing, id))
    {
        if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1)
            PushColumnClipRect();
        return false;
    }

    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_Menu) button_flags |= ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_NoHoldingActiveID;
    if (flags & ImGuiSelectableFlags_MenuItem) button_flags |= ImGuiButtonFlags_PressedOnRelease;
    if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
    if (flags & ImGuiSelectableFlags_Disabled)
        selected = false;

    // Render
    if (hovered || selected)
    {
        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
    }

    if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsCount > 1)
    {
        PushColumnClipRect();
        bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
    }

    if (flags & ImGuiSelectableFlags_Disabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
    RenderTextClipped(bb.Min, bb_with_spacing.Max, label, NULL, &label_size, ImVec2(0.0f,0.0f));
    if (flags & ImGuiSelectableFlags_Disabled) PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();
    return pressed;
}

bool ImGui::Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (Selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

// Helper to calculate the size of a listbox and display a label on the right.
// Tip: To have a list filling the entire window width, PushItemWidth(-1) and pass an empty label "##empty"
bool ImGui::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = GetStyle();
    const ImGuiID id = GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    // Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
    ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
    ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
    ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    window->DC.LastItemRect = bb;

    BeginGroup();
    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    BeginChildFrame(id, frame_bb.GetSize());
    return true;
}

bool ImGui::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
    // Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
    // However we don't add +0.40f if items_count <= height_in_items. It is slightly dodgy, because it means a dynamic list of items will make the widget resize occasionally when it crosses that size.
    // I am expecting that someone will come and complain about this behavior in a remote future, then we can advise on a better solution.
    if (height_in_items < 0)
        height_in_items = ImMin(items_count, 7);
    float height_in_items_f = height_in_items < items_count ? (height_in_items + 0.40f) : (height_in_items + 0.00f);

    // We include ItemSpacing.y so that a list sized for the exact number of items doesn't make a scrollbar appears. We could also enforce that by passing a flag to BeginChild().
    ImVec2 size;
    size.x = 0.0f;
    size.y = GetTextLineHeightWithSpacing() * height_in_items_f + GetStyle().ItemSpacing.y;
    return ListBoxHeader(label, size);
}

void ImGui::ListBoxFooter()
{
    ImGuiWindow* parent_window = GetParentWindow();
    const ImRect bb = parent_window->DC.LastItemRect;
    const ImGuiStyle& style = GetStyle();

    EndChildFrame();

    // Redeclare item size so that it includes the label (we have stored the full size in LastItemRect)
    // We call SameLine() to restore DC.CurrentLine* data
    SameLine();
    parent_window->DC.CursorPos = bb.Min;
    ItemSize(bb, style.FramePadding.y);
    EndGroup();
}

bool ImGui::ListBox(const char* label, int* current_item, const char* const* items, int items_count, int height_items)
{
    const bool value_changed = ListBox(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_items);
    return value_changed;
}

bool ImGui::ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
    if (!ListBoxHeader(label, items_count, height_in_items))
        return false;

    // Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
    bool value_changed = false;
    ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const bool item_selected = (i == *current_item);
            const char* item_text;
            if (!items_getter(data, i, &item_text))
                item_text = "*Unknown item*";

            PushID(i);
            if (Selectable(item_text, item_selected))
            {
                *current_item = i;
                value_changed = true;
            }
            PopID();
        }
    ListBoxFooter();
    return value_changed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 shortcut_size = shortcut ? CalcTextSize(shortcut, NULL) : ImVec2(0.0f, 0.0f);
    float w = window->MenuColumns.DeclColumns(label_size.x, shortcut_size.x, (float)(int)(g.FontSize * 1.20f)); // Feedback for next frame
    float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);

    bool pressed = Selectable(label, false, ImGuiSelectableFlags_MenuItem | ImGuiSelectableFlags_DrawFillAvailWidth | (enabled ? 0 : ImGuiSelectableFlags_Disabled), ImVec2(w, 0.0f));
    if (shortcut_size.x > 0.0f)
    {
        PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
        RenderText(pos + ImVec2(window->MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
        PopStyleColor();
    }

    if (selected)
        RenderCheckMark(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * (0.20f+0.200f), g.FontSize * 0.134f * 0.5f), GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled), g.FontSize  * 0.866f);

    return pressed;
}

bool ImGui::MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
    if (MenuItem(label, shortcut, p_selected ? *p_selected : false, enabled))
    {
        if (p_selected)
            *p_selected = !*p_selected;
        return true;
    }
    return false;
}

bool ImGui::BeginMainMenuBar()
{
    ImGuiContext& g = *GImGui;
    SetNextWindowPos(ImVec2(0.0f, 0.0f));
    SetNextWindowSize(ImVec2(g.IO.DisplaySize.x, g.FontBaseSize + g.Style.FramePadding.y * 2.0f));
    PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0,0));
    if (!Begin("##MainMenuBar", NULL, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_MenuBar)
        || !BeginMenuBar())
    {
        End();
        PopStyleVar(2);
        return false;
    }
    g.CurrentWindow->DC.MenuBarOffsetX += g.Style.DisplaySafeAreaPadding.x;
    return true;
}

void ImGui::EndMainMenuBar()
{
    EndMenuBar();
    End();
    PopStyleVar(2);
}

bool ImGui::BeginMenuBar()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (!(window->Flags & ImGuiWindowFlags_MenuBar))
        return false;

    IM_ASSERT(!window->DC.MenuBarAppending);
    BeginGroup(); // Save position
    PushID("##menubar");
    ImRect rect = window->MenuBarRect();
    PushClipRect(ImVec2(ImFloor(rect.Min.x+0.5f), ImFloor(rect.Min.y + window->BorderSize + 0.5f)), ImVec2(ImFloor(rect.Max.x+0.5f), ImFloor(rect.Max.y+0.5f)), false);
    window->DC.CursorPos = ImVec2(rect.Min.x + window->DC.MenuBarOffsetX, rect.Min.y);// + g.Style.FramePadding.y);
    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
    window->DC.MenuBarAppending = true;
    AlignTextToFramePadding();
    return true;
}

void ImGui::EndMenuBar()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar);
    IM_ASSERT(window->DC.MenuBarAppending);
    PopClipRect();
    PopID();
    window->DC.MenuBarOffsetX = window->DC.CursorPos.x - window->MenuBarRect().Min.x;
    window->DC.GroupStack.back().AdvanceCursor = false;
    EndGroup();
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    window->DC.MenuBarAppending = false;
}

bool ImGui::BeginMenu(const char* label, bool enabled)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 label_size = CalcTextSize(label, NULL, true);

    bool pressed;
    bool menu_is_open = IsPopupOpen(id);
    bool menuset_is_open = !(window->Flags & ImGuiWindowFlags_Popup) && (g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].ParentMenuSet == window->GetID("##menus"));
    ImGuiWindow* backed_nav_window = g.NavWindow;
    if (menuset_is_open)
        g.NavWindow = window;  // Odd hack to allow hovering across menus of a same menu-set (otherwise we wouldn't be able to hover parent)

    // The reference position stored in popup_pos will be used by Begin() to find a suitable position for the child menu (using FindBestPopupWindowPos).
    ImVec2 popup_pos, pos = window->DC.CursorPos;
    if (window->DC.LayoutType == ImGuiLayoutType_Horizontal)
    {
        // Menu inside an horizontal menu bar
        // Selectable extend their highlight by half ItemSpacing in each direction.
        popup_pos = ImVec2(pos.x - window->WindowPadding.x, pos.y - style.FramePadding.y + window->MenuBarHeight());
        window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * 0.5f);
        PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 2.0f);
        float w = label_size.x;
        pressed = Selectable(label, menu_is_open, ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | (!enabled ? ImGuiSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
        PopStyleVar();
        window->DC.CursorPos.x += (float)(int)(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
    }
    else
    {
        // Menu inside a menu
        popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
        float w = window->MenuColumns.DeclColumns(label_size.x, 0.0f, (float)(int)(g.FontSize * 1.20f)); // Feedback to next frame
        float extra_w = ImMax(0.0f, GetContentRegionAvail().x - w);
        pressed = Selectable(label, menu_is_open, ImGuiSelectableFlags_Menu | ImGuiSelectableFlags_DontClosePopups | ImGuiSelectableFlags_DrawFillAvailWidth | (!enabled ? ImGuiSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
        if (!enabled) PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
        RenderTriangle(pos + ImVec2(window->MenuColumns.Pos[2] + extra_w + g.FontSize * 0.20f, 0.0f), ImGuiDir_Right);
        if (!enabled) PopStyleColor();
    }

    const bool hovered = enabled && ItemHoverable(window->DC.LastItemRect, id);
    if (menuset_is_open)
        g.NavWindow = backed_nav_window;

    bool want_open = false, want_close = false;
    if (window->DC.LayoutType != ImGuiLayoutType_Horizontal) // (window->Flags & (ImGuiWindowFlags_Popup|ImGuiWindowFlags_ChildMenu))
    {
        // Implement http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown to avoid using timers, so menus feels more reactive.
        bool moving_within_opened_triangle = false;
        if (g.HoveredWindow == window && g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].ParentWindow == window)
        {
            if (ImGuiWindow* next_window = g.OpenPopupStack[g.CurrentPopupStack.Size].Window)
            {
                ImRect next_window_rect = next_window->Rect();
                ImVec2 ta = g.IO.MousePos - g.IO.MouseDelta;
                ImVec2 tb = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetTL() : next_window_rect.GetTR();
                ImVec2 tc = (window->Pos.x < next_window->Pos.x) ? next_window_rect.GetBL() : next_window_rect.GetBR();
                float extra = ImClamp(fabsf(ta.x - tb.x) * 0.30f, 5.0f, 30.0f); // add a bit of extra slack.
                ta.x += (window->Pos.x < next_window->Pos.x) ? -0.5f : +0.5f;   // to avoid numerical issues
                tb.y = ta.y + ImMax((tb.y - extra) - ta.y, -100.0f);            // triangle is maximum 200 high to limit the slope and the bias toward large sub-menus // FIXME: Multiply by fb_scale?
                tc.y = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
                moving_within_opened_triangle = ImTriangleContainsPoint(ta, tb, tc, g.IO.MousePos);
                //window->DrawList->PushClipRectFullScreen(); window->DrawList->AddTriangleFilled(ta, tb, tc, moving_within_opened_triangle ? IM_COL32(0,128,0,128) : IM_COL32(128,0,0,128)); window->DrawList->PopClipRect(); // Debug
            }
        }

        want_close = (menu_is_open && !hovered && g.HoveredWindow == window && g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrame != id && !moving_within_opened_triangle);
        want_open = (!menu_is_open && hovered && !moving_within_opened_triangle) || (!menu_is_open && hovered && pressed);
    }
    else
    {
        // Menu bar
        if (menu_is_open && pressed && menuset_is_open) // Click an open menu again to close it
        {
            want_close = true;
            want_open = menu_is_open = false;
        }
        else if (pressed || (hovered && menuset_is_open && !menu_is_open)) // First click to open, then hover to open others
        {
            want_open = true;
        }
    }

    if (!enabled) // explicitly close if an open menu becomes disabled, facilitate users code a lot in pattern such as 'if (BeginMenu("options", has_object)) { ..use object.. }'
        want_close = true;
    if (want_close && IsPopupOpen(id))
        ClosePopupToLevel(GImGui->CurrentPopupStack.Size);

    if (!menu_is_open && want_open && g.OpenPopupStack.Size > g.CurrentPopupStack.Size)
    {
        // Don't recycle same menu level in the same frame, first close the other menu and yield for a frame.
        OpenPopup(label);
        return false;
    }

    menu_is_open |= want_open;
    if (want_open)
        OpenPopup(label);

    if (menu_is_open)
    {
        SetNextWindowPos(popup_pos, ImGuiCond_Always);
        ImGuiWindowFlags flags = ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_AlwaysAutoResize | ((window->Flags & (ImGuiWindowFlags_Popup|ImGuiWindowFlags_ChildMenu)) ? ImGuiWindowFlags_ChildMenu|ImGuiWindowFlags_ChildWindow : ImGuiWindowFlags_ChildMenu);
        menu_is_open = BeginPopupEx(id, flags); // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
    }

    return menu_is_open;
}

void ImGui::EndMenu()
{
    EndPopup();
}

// Note: only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
void ImGui::ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags)
{
    ImGuiContext& g = *GImGui;

    int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
    BeginTooltipEx(0, true);
    
    const char* text_end = text ? FindRenderedTextEnd(text, NULL) : text;
    if (text_end > text)
    {
        TextUnformatted(text, text_end);
        Separator();
    }

    ImVec2 sz(g.FontSize * 3, g.FontSize * 3);
    ColorButton("##preview", ImVec4(col[0], col[1], col[2], col[3]), (flags & (ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf)) | ImGuiColorEditFlags_NoTooltip, sz);
    SameLine();
    if (flags & ImGuiColorEditFlags_NoAlpha)
        Text("#%02X%02X%02X\nR: %d, G: %d, B: %d\n(%.3f, %.3f, %.3f)", cr, cg, cb, cr, cg, cb, col[0], col[1], col[2]);
    else
        Text("#%02X%02X%02X%02X\nR:%d, G:%d, B:%d, A:%d\n(%.3f, %.3f, %.3f, %.3f)", cr, cg, cb, ca, cr, cg, cb, ca, col[0], col[1], col[2], col[3]);
    EndTooltip();
}

static inline ImU32 ImAlphaBlendColor(ImU32 col_a, ImU32 col_b)
{
    float t = ((col_b >> IM_COL32_A_SHIFT) & 0xFF) / 255.f;
    int r = ImLerp((int)(col_a >> IM_COL32_R_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_R_SHIFT) & 0xFF, t);
    int g = ImLerp((int)(col_a >> IM_COL32_G_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_G_SHIFT) & 0xFF, t);
    int b = ImLerp((int)(col_a >> IM_COL32_B_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_B_SHIFT) & 0xFF, t);
    return IM_COL32(r, g, b, 0xFF);
}

// NB: This is rather brittle and will show artifact when rounding this enabled if rounded corners overlap multiple cells. Caller currently responsible for avoiding that.
// I spent a non reasonable amount of time trying to getting this right for ColorButton with rounding+anti-aliasing+ImGuiColorEditFlags_HalfAlphaPreview flag + various grid sizes and offsets, and eventually gave up... probably more reasonable to disable rounding alltogether.
void ImGui::RenderColorRectWithAlphaCheckerboard(ImVec2 p_min, ImVec2 p_max, ImU32 col, float grid_step, ImVec2 grid_off, float rounding, int rounding_corners_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (((col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT) < 0xFF)
    {
        ImU32 col_bg1 = GetColorU32(ImAlphaBlendColor(IM_COL32(204,204,204,255), col));
        ImU32 col_bg2 = GetColorU32(ImAlphaBlendColor(IM_COL32(128,128,128,255), col));
        window->DrawList->AddRectFilled(p_min, p_max, col_bg1, rounding, rounding_corners_flags);

        int yi = 0;
        for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
        {
            float y1 = ImClamp(y, p_min.y, p_max.y), y2 = ImMin(y + grid_step, p_max.y);
            if (y2 <= y1)
                continue;
            for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step; x < p_max.x; x += grid_step * 2.0f)
            {
                float x1 = ImClamp(x, p_min.x, p_max.x), x2 = ImMin(x + grid_step, p_max.x);
                if (x2 <= x1)
                    continue;
                int rounding_corners_flags_cell = 0;
                if (y1 <= p_min.y) { if (x1 <= p_min.x) rounding_corners_flags_cell |= ImGuiCorner_TopLeft;    if (x2 >= p_max.x) rounding_corners_flags_cell |= ImGuiCorner_TopRight; }
                if (y2 >= p_max.y) { if (x1 <= p_min.x) rounding_corners_flags_cell |= ImGuiCorner_BotLeft; if (x2 >= p_max.x) rounding_corners_flags_cell |= ImGuiCorner_BotRight; }
                rounding_corners_flags_cell &= rounding_corners_flags;
                window->DrawList->AddRectFilled(ImVec2(x1,y1), ImVec2(x2,y2), col_bg2, rounding_corners_flags_cell ? rounding : 0.0f, rounding_corners_flags_cell);
            }
        }
    }
    else
    {
        window->DrawList->AddRectFilled(p_min, p_max, col, rounding, rounding_corners_flags);
    }
}

void ImGui::SetColorEditOptions(ImGuiColorEditFlags flags)
{
    ImGuiContext& g = *GImGui;
    if ((flags & ImGuiColorEditFlags__InputsMask) == 0)
        flags |= ImGuiColorEditFlags__OptionsDefault & ImGuiColorEditFlags__InputsMask;
    if ((flags & ImGuiColorEditFlags__DataTypeMask) == 0)
        flags |= ImGuiColorEditFlags__OptionsDefault & ImGuiColorEditFlags__DataTypeMask;
    if ((flags & ImGuiColorEditFlags__PickerMask) == 0)
        flags |= ImGuiColorEditFlags__OptionsDefault & ImGuiColorEditFlags__PickerMask;
    IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__InputsMask)));   // Check only 1 option is selected
    IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__DataTypeMask))); // Check only 1 option is selected
    IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__PickerMask)));   // Check only 1 option is selected
    g.ColorEditOptions = flags;
}

bool ImGui::ColorButton(const char* desc_id, const float col[], ImGuiColorEditFlags flags, ImVec2 size)
{
	return ImGui::ColorButton(desc_id, ImColor(col[0], col[1], col[2], col[3]), flags, size);
}

// A little colored square. Return true when clicked.
// FIXME: May want to display/ignore the alpha component in the color display? Yet show it in the tooltip.
// 'desc_id' is not called 'label' because we don't display it next to the button, but only in the tooltip.
bool ImGui::ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, ImVec2 size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(desc_id);
    float default_size = SmallSquareSize();
    if (size.x == 0.0f)
        size.x = default_size;
    if (size.y == 0.0f)
        size.y = default_size;

	ImVec2 begin_Vec = ImVec2(window->Pos.x + GetContentRegionMax().x - size.x - 1, window->DC.CursorPos.y);
	const ImRect bb(begin_Vec, ImVec2(begin_Vec.x + size.x, begin_Vec.y + size.y));
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    if (flags & ImGuiColorEditFlags_NoAlpha)
        flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);
    
    ImVec4 col_without_alpha(col.x, col.y, col.z, 1.0f);
    float grid_step = ImMin(size.x, size.y) / 2.99f;
    float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
    ImRect bb_inner = bb;
    float off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middleground to reduce those artefacts.
    bb_inner.Expand(off);
    if ((flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col.w < 1.0f)
    {
        float mid_x = (float)(int)((bb_inner.Min.x + bb_inner.Max.x) * 0.5f + 0.5f);
        RenderColorRectWithAlphaCheckerboard(ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, GetColorU32(col), grid_step, ImVec2(-grid_step + off, off), rounding, ImGuiCorner_TopRight|ImGuiCorner_BotRight);
        window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), GetColorU32(col_without_alpha), rounding, ImGuiCorner_TopLeft|ImGuiCorner_BotLeft);
    }
    else
    {
        RenderColorRectWithAlphaCheckerboard(bb_inner.Min, bb_inner.Max, GetColorU32((flags & ImGuiColorEditFlags_AlphaPreview) ? col : col_without_alpha), grid_step, ImVec2(off, off), rounding);
    }
    if (window->Flags & ImGuiWindowFlags_ShowBorders)
        RenderFrameBorder(bb.Min, bb.Max, rounding);
    else
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), rounding); // Color button are often in need of some sort of border

    if (hovered && !(flags & ImGuiColorEditFlags_NoTooltip))
        ColorTooltip(desc_id, &col.x, flags & (ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf));

	if (pressed)
		g.IO.MousePos = bb_inner.Max;

    return pressed;
}

bool ImGui::ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
    return ColorEdit4(label, col, flags | ImGuiColorEditFlags_NoAlpha);
}

void ImGui::ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags)
{
    bool allow_opt_inputs = !(flags & ImGuiColorEditFlags__InputsMask);
    bool allow_opt_datatype = !(flags & ImGuiColorEditFlags__DataTypeMask);
    if ((!allow_opt_inputs && !allow_opt_datatype) || !BeginPopup("context"))
        return;
    ImGuiContext& g = *GImGui;
    ImGuiColorEditFlags opts = g.ColorEditOptions;
    if (allow_opt_inputs)
    {
        if (RadioButton("RGB", (opts & ImGuiColorEditFlags_RGB) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__InputsMask) | ImGuiColorEditFlags_RGB;
        if (RadioButton("HSV", (opts & ImGuiColorEditFlags_HSV) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__InputsMask) | ImGuiColorEditFlags_HSV;
        if (RadioButton("HEX", (opts & ImGuiColorEditFlags_HEX) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__InputsMask) | ImGuiColorEditFlags_HEX;
    }
    if (allow_opt_datatype)
    {
        if (allow_opt_inputs) Separator();
        if (RadioButton("0..255",     (opts & ImGuiColorEditFlags_Uint8) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__DataTypeMask) | ImGuiColorEditFlags_Uint8;
        if (RadioButton("0.00..1.00", (opts & ImGuiColorEditFlags_Float) ? 1 : 0)) opts = (opts & ~ImGuiColorEditFlags__DataTypeMask) | ImGuiColorEditFlags_Float;
    }

    if (allow_opt_inputs || allow_opt_datatype)
        Separator();
    if (Button("Copy as..", ImVec2(-1,0)))
        OpenPopup("Copy");
    if (BeginPopup("Copy"))
    {
        int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
        char buf[64];
        sprintf(buf, "(%.3ff, %.3ff, %.3ff, %.3ff)", col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if (Selectable(buf))
            SetClipboardText(buf);
        sprintf(buf, "(%d,%d,%d,%d)", cr, cg, cb, ca);
        if (Selectable(buf))
            SetClipboardText(buf);
        if (flags & ImGuiColorEditFlags_NoAlpha)
            sprintf(buf, "0x%02X%02X%02X", cr, cg, cb);
        else
            sprintf(buf, "0x%02X%02X%02X%02X", cr, cg, cb, ca);
        if (Selectable(buf))
            SetClipboardText(buf);
        EndPopup();
    }

    g.ColorEditOptions = opts;
    EndPopup();
}

static void ColorPickerOptionsPopup(ImGuiColorEditFlags flags, float* ref_col)
{
    bool allow_opt_picker = !(flags & ImGuiColorEditFlags__PickerMask);
    bool allow_opt_alpha_bar = !(flags & ImGuiColorEditFlags_NoAlpha) && !(flags & ImGuiColorEditFlags_AlphaBar);
    if ((!allow_opt_picker && !allow_opt_alpha_bar) || !ImGui::BeginPopup("context"))
        return;
    ImGuiContext& g = *GImGui;
    if (allow_opt_picker)
    {
        ImVec2 picker_size(g.FontSize * 8, ImMax(g.FontSize * 8 - (SmallSquareSize() + g.Style.ItemInnerSpacing.x), 1.0f)); // FIXME: Picker size copied from main picker function
        ImGui::PushItemWidth(picker_size.x);
        for (int picker_type = 0; picker_type < 2; picker_type++)
        {
            // Draw small/thumbnail version of each picker type (over an invisible button for selection)
            if (picker_type > 0) ImGui::Separator();
            ImGui::PushID(picker_type);
            ImGuiColorEditFlags picker_flags = ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoOptions|ImGuiColorEditFlags_NoLabel|ImGuiColorEditFlags_NoSidePreview|(flags & ImGuiColorEditFlags_NoAlpha);
            if (picker_type == 0) picker_flags |= ImGuiColorEditFlags_PickerHueBar;
            if (picker_type == 1) picker_flags |= ImGuiColorEditFlags_PickerHueWheel;
            ImVec2 backup_pos = ImGui::GetCursorScreenPos();
            if (ImGui::Selectable("##selectable", false, 0, picker_size)) // By default, Selectable() is closing popup
                g.ColorEditOptions = (g.ColorEditOptions & ~ImGuiColorEditFlags__PickerMask) | (picker_flags & ImGuiColorEditFlags__PickerMask);
            ImGui::SetCursorScreenPos(backup_pos);
            ImVec4 dummy_ref_col;
            memcpy(&dummy_ref_col.x, ref_col, sizeof(float) * (picker_flags & ImGuiColorEditFlags_NoAlpha ? 3 : 4));
            ImGui::ColorPicker4("##dummypicker", &dummy_ref_col.x, picker_flags);
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
    }
    if (allow_opt_alpha_bar)
    {
        if (allow_opt_picker) ImGui::Separator();
        ImGui::CheckboxFlags("Alpha Bar", (unsigned int*)&g.ColorEditOptions, ImGuiColorEditFlags_AlphaBar);
    }
    ImGui::EndPopup();
}

// Edit colors components (each component in 0.0f..1.0f range). 
// See enum ImGuiColorEditFlags_ for available options. e.g. Only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// With typical options: Left-click on colored square to open color picker. Right-click to open option menu. CTRL-Click over input fields to edit them and TAB to go to next item.
bool ImGui::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float square_sz = SmallSquareSize();
    const float w_extra = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_items_all = CalcItemWidth() - w_extra;
    const char* label_display_end = FindRenderedTextEnd(label);

    const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
    const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;
    const ImGuiColorEditFlags flags_untouched = flags;

    BeginGroup();
    PushID(label);

    // If we're not showing any slider there's no point in doing any HSV conversions
    if (flags & ImGuiColorEditFlags_NoInputs)
        flags = (flags & (~ImGuiColorEditFlags__InputsMask)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ColorEditOptionsPopup(col, flags);
 
    // Read stored options
    if (!(flags & ImGuiColorEditFlags__InputsMask))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags__InputsMask);
    if (!(flags & ImGuiColorEditFlags__DataTypeMask))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DataTypeMask);
    if (!(flags & ImGuiColorEditFlags__PickerMask))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags__PickerMask);
    flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask));

    // Convert to the formats we need
    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    if (flags & ImGuiColorEditFlags_HSV)
        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    if ((flags & (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB/HSV 0..255 Sliders
        const float w_item_one  = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components-1)) / (float)components));
        const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components-1)));

        const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
        const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
        const char* fmt_table_int[3][4] =
        {
            {   "%3.0f",   "%3.0f",   "%3.0f",   "%3.0f" }, // Short display
            { "R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f" }, // Long display for RGBA
            { "H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f" }  // Long display for HSVA
        };
        const char* fmt_table_float[3][4] =
        {
            {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
            { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
            { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
        };
        const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_HSV) ? 2 : 1;

        PushItemWidth(w_item_one);
        for (int n = 0; n < components; n++)
        {
            if (n > 0)
                SameLine(0, style.ItemInnerSpacing.x);
            if (n + 1 == components)
                PushItemWidth(w_item_last);
            if (flags & ImGuiColorEditFlags_Float)
                value_changed |= value_changed_as_float |= DragFloat(ids[n], &f[n], 1.0f/255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
            else
                value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
            if (!(flags & ImGuiColorEditFlags_NoOptions) && IsItemHovered() && IsMouseClicked(1))
                OpenPopup("context");
        }
        PopItemWidth();
        PopItemWidth();
    }
    else if ((flags & ImGuiColorEditFlags_HEX) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB Hexadecimal Input
        char buf[64];
        if (alpha)
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0],0,255), ImClamp(i[1],0,255), ImClamp(i[2],0,255), ImClamp(i[3],0,255));
        else
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0],0,255), ImClamp(i[1],0,255), ImClamp(i[2],0,255));
        PushItemWidth(w_items_all);
        if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
        {
            value_changed |= true;
            char* p = buf;
            while (*p == '#' || ImCharIsSpace(*p))
                p++;
            i[0] = i[1] = i[2] = i[3] = 0;
            if (alpha)
                sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
            else
                sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions) && IsItemHovered() && IsMouseClicked(1))
            OpenPopup("context");
        PopItemWidth();
    }

    bool picker_active = false;
    if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
    {
        if (!(flags & ImGuiColorEditFlags_NoInputs))
            SameLine(0, style.ItemInnerSpacing.x);

        const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (ColorButton("##ColorButton", col_v4, flags))
        {
            if (!(flags & ImGuiColorEditFlags_NoPicker))
            {
                // Store current color and open a picker
                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1,style.ItemSpacing.y));
            }
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions) && IsItemHovered() && IsMouseClicked(1))
            OpenPopup("context");

        if (BeginPopup("picker"))
        {
            picker_active = true;
            if (label != label_display_end)
            {
                TextUnformatted(label, label_display_end);
                Separator();
            }
            ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
            ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
            PushItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
            value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
            PopItemWidth();
            EndPopup();
        }
    }

    if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
    {
        SameLine(0, style.ItemInnerSpacing.x);
        TextUnformatted(label, label_display_end);
    }

    // Convert back
    if (!picker_active)
    {
        if (!value_changed_as_float) 
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if (flags & ImGuiColorEditFlags_HSV)
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        if (value_changed)
        {
            col[0] = f[0];
            col[1] = f[1];
            col[2] = f[2];
            if (alpha)
                col[3] = f[3];
        }
    }

    PopID();
    EndGroup();

    return value_changed;
}

bool ImGui::ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags)
{
    float col4[4] = { col[0], col[1], col[2], 1.0f };
    if (!ColorPicker4(label, col4, flags | ImGuiColorEditFlags_NoAlpha))
        return false;
    col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
    return true;
}

// 'pos' is position of the arrow tip. half_sz.x is length from base to tip. half_sz.y is length on each side.
static void RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col)
{
    switch (direction)
    {
    case ImGuiDir_Left:  draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), pos, col); return;
    case ImGuiDir_Right: draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), pos, col); return;
    case ImGuiDir_Up:    draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), pos, col); return;
    case ImGuiDir_Down:  draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), pos, col); return;
    default: return; // Fix warning for ImGuiDir_None
    }
}

static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w)
{
    RenderArrow(draw_list, ImVec2(pos.x + half_sz.x + 1,         pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32_BLACK);
    RenderArrow(draw_list, ImVec2(pos.x + half_sz.x,             pos.y), half_sz,                              ImGuiDir_Right, IM_COL32_WHITE);
    RenderArrow(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left,  IM_COL32_BLACK);
    RenderArrow(draw_list, ImVec2(pos.x + bar_w - half_sz.x,     pos.y), half_sz,                              ImGuiDir_Left,  IM_COL32_WHITE);
}

// ColorPicker
// Note: only access 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// FIXME: we adjust the big color square height based on item width, which may cause a flickering feedback loop (if automatic height makes a vertical scrollbar appears, affecting automatic width..) 
bool ImGui::ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    ImDrawList* draw_list = window->DrawList;

    ImGuiStyle& style = g.Style;
    ImGuiIO& io = g.IO;

    PushID(label);
    BeginGroup();

    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
        flags |= ImGuiColorEditFlags_NoSmallPreview;

    // Context menu: display and store options.
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ColorPickerOptionsPopup(flags, col);

    // Read stored options
    if (!(flags & ImGuiColorEditFlags__PickerMask))
        flags |= ((g.ColorEditOptions & ImGuiColorEditFlags__PickerMask) ? g.ColorEditOptions : ImGuiColorEditFlags__OptionsDefault) & ImGuiColorEditFlags__PickerMask; 
    IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__PickerMask))); // Check that only 1 is selected
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

    // Setup
    bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
    ImVec2 picker_pos = window->DC.CursorPos;
    float square_sz = SmallSquareSize();
    float bars_width = square_sz; // Arbitrary smallish width of Hue/Alpha picking bars
    float sv_picker_size = ImMax(bars_width * 1, CalcItemWidth() - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
    float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
    float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
    float bars_triangles_half_sz = (float)(int)(bars_width * 0.20f);

    float wheel_thickness = sv_picker_size * 0.08f;
    float wheel_r_outer = sv_picker_size * 0.50f;
    float wheel_r_inner = wheel_r_outer - wheel_thickness;
    ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width)*0.5f, picker_pos.y + sv_picker_size*0.5f);
    
    // Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
    float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
    ImVec2 triangle_pa = ImVec2(triangle_r, 0.0f); // Hue point.
    ImVec2 triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
    ImVec2 triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

    float H,S,V;
    ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

    bool value_changed = false, value_changed_h = false, value_changed_sv = false;

    if (flags & ImGuiColorEditFlags_PickerHueWheel)
    {
        // Hue wheel + SV triangle logic
        InvisibleButton("hsv", ImVec2(sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size));
        if (IsItemActive())
        {
            ImVec2 initial_off = g.IO.MouseClickedPos[0] - wheel_center;
            ImVec2 current_off = g.IO.MousePos - wheel_center;
            float initial_dist2 = ImLengthSqr(initial_off);
            if (initial_dist2 >= (wheel_r_inner-1)*(wheel_r_inner-1) && initial_dist2 <= (wheel_r_outer+1)*(wheel_r_outer+1))
            {
                // Interactive with Hue wheel
                H = atan2f(current_off.y, current_off.x) / IM_PI*0.5f;
                if (H < 0.0f)
                    H += 1.0f;
                value_changed = value_changed_h = true;
            }
            float cos_hue_angle = cosf(-H * 2.0f * IM_PI);
            float sin_hue_angle = sinf(-H * 2.0f * IM_PI);
            if (ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, ImRotate(initial_off, cos_hue_angle, sin_hue_angle)))
            {
                // Interacting with SV triangle
                ImVec2 current_off_unrotated = ImRotate(current_off, cos_hue_angle, sin_hue_angle);
                if (!ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated))
                    current_off_unrotated = ImTriangleClosestPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated);
                float uu, vv, ww;
                ImTriangleBarycentricCoords(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww);
                V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
                S = ImClamp(uu / V, 0.0001f, 1.0f);
                value_changed = value_changed_sv = true;
            }
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions) && IsItemHovered() && IsMouseClicked(1))
            OpenPopup("context");
    }
    else if (flags & ImGuiColorEditFlags_PickerHueBar)
    {
        // SV rectangle logic
        InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
        if (IsItemActive())
        {
            S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size-1));
            V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size-1));
            value_changed = value_changed_sv = true;
        }
        if (!(flags & ImGuiColorEditFlags_NoOptions) && IsItemHovered() && IsMouseClicked(1))
            OpenPopup("context");

        // Hue bar logic
        SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
        InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size-1));
            value_changed = value_changed_h = true;
        }
    }

    // Alpha bar logic
    if (alpha_bar)
    {
        SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
        InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size-1));
            value_changed = true;
        }
    }

    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    {
        SameLine(0, style.ItemInnerSpacing.x);
        BeginGroup();
    }

    if (!(flags & ImGuiColorEditFlags_NoLabel))
    {
        const char* label_display_end = FindRenderedTextEnd(label);
        if (label != label_display_end)
        {
            if ((flags & ImGuiColorEditFlags_NoSidePreview))
                SameLine(0, style.ItemInnerSpacing.x);
            TextUnformatted(label, label_display_end);
        }
    }

    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    {
        ImVec4 col_v4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if ((flags & ImGuiColorEditFlags_NoLabel))
            Text("Current");
        ColorButton("##current", col_v4, (flags & (ImGuiColorEditFlags_HDR|ImGuiColorEditFlags_AlphaPreview|ImGuiColorEditFlags_AlphaPreviewHalf|ImGuiColorEditFlags_NoTooltip)), ImVec2(square_sz * 3, square_sz * 2));
        if (ref_col != NULL)
        {
            Text("Original");
            ImVec4 ref_col_v4(ref_col[0], ref_col[1], ref_col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : ref_col[3]);
            if (ColorButton("##original", ref_col_v4, (flags & (ImGuiColorEditFlags_HDR|ImGuiColorEditFlags_AlphaPreview|ImGuiColorEditFlags_AlphaPreviewHalf|ImGuiColorEditFlags_NoTooltip)), ImVec2(square_sz * 3, square_sz * 2)))
            {
                memcpy(col, ref_col, ((flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4) * sizeof(float));
                value_changed = true;
            }
        }
        EndGroup();
    }

    // Convert back color to RGB
    if (value_changed_h || value_changed_sv)
        ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10*1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

    // R,G,B and H,S,V slider color editor
    if ((flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        PushItemWidth((alpha_bar ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
        ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf;
        ImGuiColorEditFlags sub_flags = (flags & sub_flags_to_forward) | ImGuiColorEditFlags_NoPicker;
        if (flags & ImGuiColorEditFlags_RGB || (flags & ImGuiColorEditFlags__InputsMask) == 0)
            value_changed |= ColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_RGB);
        if (flags & ImGuiColorEditFlags_HSV || (flags & ImGuiColorEditFlags__InputsMask) == 0)
            value_changed |= ColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_HSV);
        if (flags & ImGuiColorEditFlags_HEX || (flags & ImGuiColorEditFlags__InputsMask) == 0)
            value_changed |= ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);
        PopItemWidth();
    }

    // Try to cancel hue wrap (after ColorEdit), if any
    if (value_changed)
    {
        float new_H, new_S, new_V;
        ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
        if (new_H <= 0 && H > 0) 
        {
            if (new_V <= 0 && V != new_V)
                ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
            else if (new_S <= 0)
                ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
        }
    }

    ImVec4 hue_color_f(1, 1, 1, 1); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
    ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
    ImU32 col32_no_alpha = ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 1.0f));

    const ImU32 hue_colors[6+1] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
    ImVec2 sv_cursor_pos;
    
    if (flags & ImGuiColorEditFlags_PickerHueWheel)
    {
        // Render Hue Wheel
        const float aeps = 1.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
        const int segment_per_arc = ImMax(4, (int)wheel_r_outer / 12);
        for (int n = 0; n < 6; n++)
        {
            const float a0 = (n)     /6.0f * 2.0f * IM_PI - aeps;
            const float a1 = (n+1.0f)/6.0f * 2.0f * IM_PI + aeps;
            int vert_start_idx = draw_list->_VtxCurrentIdx;
            draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer)*0.5f, a0, a1, segment_per_arc);
            draw_list->PathStroke(IM_COL32_WHITE, false, wheel_thickness);

            // Paint colors over existing vertices
            ImVec2 gradient_p0(wheel_center.x + cosf(a0) * wheel_r_inner, wheel_center.y + sinf(a0) * wheel_r_inner);
            ImVec2 gradient_p1(wheel_center.x + cosf(a1) * wheel_r_inner, wheel_center.y + sinf(a1) * wheel_r_inner);
            ShadeVertsLinearColorGradientKeepAlpha(draw_list->_VtxWritePtr - (draw_list->_VtxCurrentIdx - vert_start_idx), draw_list->_VtxWritePtr, gradient_p0, gradient_p1, hue_colors[n], hue_colors[n+1]);
        }

        // Render Cursor + preview on Hue Wheel
        float cos_hue_angle = cosf(H * 2.0f * IM_PI);
        float sin_hue_angle = sinf(H * 2.0f * IM_PI);
        ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner+wheel_r_outer)*0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner+wheel_r_outer)*0.5f);
        float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
        int hue_cursor_segments = ImClamp((int)(hue_cursor_rad / 1.4f), 9, 32);
        draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
        draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad+1, IM_COL32(128,128,128,255), hue_cursor_segments);
        draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, IM_COL32_WHITE, hue_cursor_segments);

        // Render SV triangle (rotated according to hue)
        ImVec2 tra = wheel_center + ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
        ImVec2 trb = wheel_center + ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
        ImVec2 trc = wheel_center + ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
        ImVec2 uv_white = g.FontTexUvWhitePixel;
        draw_list->PrimReserve(6, 6);
        draw_list->PrimVtx(tra, uv_white, hue_color32);
        draw_list->PrimVtx(trb, uv_white, hue_color32);
        draw_list->PrimVtx(trc, uv_white, IM_COL32_WHITE);
        draw_list->PrimVtx(tra, uv_white, IM_COL32_BLACK_TRANS);
        draw_list->PrimVtx(trb, uv_white, IM_COL32_BLACK);
        draw_list->PrimVtx(trc, uv_white, IM_COL32_BLACK_TRANS);
        draw_list->AddTriangle(tra, trb, trc, IM_COL32(128,128,128,255), 1.5f);
        sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
    }
    else if (flags & ImGuiColorEditFlags_PickerHueBar)
    {
        // Render SV Square
        draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size,sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
        draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size,sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);
        RenderFrameBorder(picker_pos, picker_pos + ImVec2(sv_picker_size,sv_picker_size), 0.0f);
        sv_cursor_pos.x = ImClamp((float)(int)(picker_pos.x + ImSaturate(S)     * sv_picker_size + 0.5f), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
        sv_cursor_pos.y = ImClamp((float)(int)(picker_pos.y + ImSaturate(1 - V) * sv_picker_size + 0.5f), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

        // Render Hue Bar
        for (int i = 0; i < 6; ++i)
            draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)), hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
        float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
        RenderFrameBorder(ImVec2(bar0_pos_x, picker_pos.y), ImVec2(bar0_pos_x + bars_width, picker_pos.y + sv_picker_size), 0.0f);
        RenderArrowsForVerticalBar(draw_list, ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f);
    }

    // Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be out of range)
    float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
    draw_list->AddCircleFilled(sv_cursor_pos, sv_cursor_rad, col32_no_alpha, 12);
    draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad+1, IM_COL32(128,128,128,255), 12);
    draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad, IM_COL32_WHITE, 12);

    // Render alpha bar
    if (alpha_bar)
    {
        float alpha = ImSaturate(col[3]);
        ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);
        RenderColorRectWithAlphaCheckerboard(bar1_bb.Min, bar1_bb.Max, IM_COL32(0,0,0,0), bar1_bb.GetWidth() / 2.0f, ImVec2(0.0f, 0.0f));
        draw_list->AddRectFilledMultiColor(bar1_bb.Min, bar1_bb.Max, col32_no_alpha, col32_no_alpha, col32_no_alpha & ~IM_COL32_A_MASK, col32_no_alpha & ~IM_COL32_A_MASK);
        float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
        RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
        RenderArrowsForVerticalBar(draw_list, ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f);
    }

    EndGroup();
    PopID();

    return value_changed;
}

// Horizontal separating line.
void ImGui::Separator()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ImGuiContext& g = *GImGui;

    ImGuiWindowFlags flags = 0;
    if ((flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical)) == 0)
        flags |= (window->DC.LayoutType == ImGuiLayoutType_Horizontal) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;
    IM_ASSERT(ImIsPowerOfTwo((int)(flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical))));   // Check that only 1 option is selected
    if (flags & ImGuiSeparatorFlags_Vertical)
    {
        VerticalSeparator();
        return;
    }

    // Horizontal Separator
    if (window->DC.ColumnsCount > 1)
        PopClipRect();

    float x1 = window->Pos.x;
    float x2 = window->Pos.x + window->Size.x;
    if (!window->DC.GroupStack.empty())
        x1 += window->DC.IndentX;

    const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y+1.0f));
    ItemSize(ImVec2(0.0f, 0.0f)); // NB: we don't provide our width so that it doesn't get feed back into AutoFit, we don't provide height to not alter layout.
    if (!ItemAdd(bb, 0))
    {
        if (window->DC.ColumnsCount > 1)
            PushColumnClipRect();
        return;
    }

    window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x,bb.Min.y), GetColorU32(ImGuiCol_Separator));

    if (g.LogEnabled)
            LogRenderedText(NULL, IM_NEWLINE "--------------------------------");

    if (window->DC.ColumnsCount > 1)
    {
        PushColumnClipRect();
        window->DC.ColumnsCellMinY = window->DC.CursorPos.y;
    }
}

void ImGui::VerticalSeparator()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ImGuiContext& g = *GImGui;

    float y1 = window->DC.CursorPos.y;
    float y2 = window->DC.CursorPos.y + window->DC.CurrentLineHeight; 
    const ImRect bb(ImVec2(window->DC.CursorPos.x, y1), ImVec2(window->DC.CursorPos.x + 1.0f, y2));
    ItemSize(ImVec2(bb.GetWidth(), 0.0f));
    if (!ItemAdd(bb, 0))
        return;

    window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Min.x, bb.Max.y), GetColorU32(ImGuiCol_Separator));
    if (g.LogEnabled)
        LogText(" |");
}

void ImGui::Spacing()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ItemSize(ImVec2(0,0));
}

void ImGui::Dummy(const ImVec2& size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb);
    ItemAdd(bb, 0);
}

bool ImGui::IsRectVisible(const ImVec2& size)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ClipRect.Overlaps(ImRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool ImGui::IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->ClipRect.Overlaps(ImRect(rect_min, rect_max));
}

// Lock horizontal starting position + capture group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
void ImGui::BeginGroup()
{
    ImGuiWindow* window = GetCurrentWindow();

    window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
    ImGuiGroupData& group_data = window->DC.GroupStack.back();
    group_data.BackupCursorPos = window->DC.CursorPos;
    group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
    group_data.BackupIndentX = window->DC.IndentX;
    group_data.BackupGroupOffsetX = window->DC.GroupOffsetX;
    group_data.BackupCurrentLineHeight = window->DC.CurrentLineHeight;
    group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
    group_data.BackupLogLinePosY = window->DC.LogLinePosY;
    group_data.BackupActiveIdIsAlive = GImGui->ActiveIdIsAlive;
    group_data.AdvanceCursor = true;

    window->DC.GroupOffsetX = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffsetX;
    window->DC.IndentX = window->DC.GroupOffsetX;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.CurrentLineHeight = 0.0f;
    window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
}

void ImGui::EndGroup()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    IM_ASSERT(!window->DC.GroupStack.empty());    // Mismatched BeginGroup()/EndGroup() calls

    ImGuiGroupData& group_data = window->DC.GroupStack.back();

    ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
    group_bb.Max.y -= g.Style.ItemSpacing.y;      // Cancel out last vertical spacing because we are adding one ourselves.
    group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

    window->DC.CursorPos = group_data.BackupCursorPos;
    window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
    window->DC.CurrentLineHeight = group_data.BackupCurrentLineHeight;
    window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
    window->DC.IndentX = group_data.BackupIndentX;
    window->DC.GroupOffsetX = group_data.BackupGroupOffsetX;
    window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;

    if (group_data.AdvanceCursor)
    {
        window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrentLineTextBaseOffset);      // FIXME: Incorrect, we should grab the base offset from the *first line* of the group but it is hard to obtain now.
        ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
        ItemAdd(group_bb, 0);
    }

    // If the current ActiveId was declared within the boundary of our group, we copy it to LastItemId so IsItemActive() will be functional on the entire group.
    // It would be be neater if we replaced window.DC.LastItemId by e.g. 'bool LastItemIsActive', but if you search for LastItemId you'll notice it is only used in that context.
    const bool active_id_within_group = (!group_data.BackupActiveIdIsAlive && g.ActiveIdIsAlive && g.ActiveId && g.ActiveIdWindow->RootWindow == window->RootWindow);
    if (active_id_within_group)
        window->DC.LastItemId = g.ActiveId;
    window->DC.LastItemRect = group_bb;

    window->DC.GroupStack.pop_back();

    //window->DrawList->AddRect(group_bb.Min, group_bb.Max, IM_COL32(255,0,255,255));   // [Debug]
}

// Gets back to previous line and continue with horizontal layout
//      pos_x == 0      : follow right after previous item
//      pos_x != 0      : align to specified x position (relative to window/group left)
//      spacing_w < 0   : use default spacing if pos_x == 0, no spacing if pos_x != 0
//      spacing_w >= 0  : enforce spacing amount
void ImGui::SameLine(float pos_x, float spacing_w)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    if (pos_x != 0.0f)
    {
        if (spacing_w < 0.0f) spacing_w = 0.0f;
        window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + pos_x + spacing_w + window->DC.GroupOffsetX + window->DC.ColumnsOffsetX;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    else
    {
        if (spacing_w < 0.0f) spacing_w = g.Style.ItemSpacing.x;
        window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    window->DC.CurrentLineHeight = window->DC.PrevLineHeight;
    window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::NewLine()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiLayoutType backup_layout_type = window->DC.LayoutType;
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    if (window->DC.CurrentLineHeight > 0.0f)     // In the event that we are on a line with items that is smaller that FontSize high, we will preserve its height.
        ItemSize(ImVec2(0,0));
    else
        ItemSize(ImVec2(0.0f, g.FontSize));
    window->DC.LayoutType = backup_layout_type;
}

void ImGui::NextColumn()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems || window->DC.ColumnsCount <= 1)
        return;

    ImGuiContext& g = *GImGui;
    PopItemWidth();
    PopClipRect();

    window->DC.ColumnsCellMaxY = ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
    if (++window->DC.ColumnsCurrent < window->DC.ColumnsCount)
    {
        // Columns 1+ cancel out IndentX
        window->DC.ColumnsOffsetX = GetColumnOffset(window->DC.ColumnsCurrent) - window->DC.IndentX + g.Style.ItemSpacing.x;
        window->DrawList->ChannelsSetCurrent(window->DC.ColumnsCurrent);
    }
    else
    {
        window->DC.ColumnsCurrent = 0;
        window->DC.ColumnsOffsetX = 0.0f;
        window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY;
        window->DrawList->ChannelsSetCurrent(0);
    }
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
    window->DC.CursorPos.y = window->DC.ColumnsCellMinY;
    window->DC.CurrentLineHeight = 0.0f;
    window->DC.CurrentLineTextBaseOffset = 0.0f;

    PushColumnClipRect();
    PushItemWidth(GetColumnWidth() * 0.65f);  // FIXME: Move on columns setup
}

int ImGui::GetColumnIndex()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.ColumnsCurrent;
}

int ImGui::GetColumnsCount()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.ColumnsCount;
}

static float OffsetNormToPixels(ImGuiWindow* window, float offset_norm)
{
    return offset_norm * (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
}

static float PixelsToOffsetNorm(ImGuiWindow* window, float offset)
{
    return (offset - window->DC.ColumnsMinX) / (window->DC.ColumnsMaxX - window->DC.ColumnsMinX);
}

static float GetDraggedColumnOffset(int column_index)
{
    // Active (dragged) column always follow mouse. The reason we need this is that dragging a column to the right edge of an auto-resizing
    // window creates a feedback loop because we store normalized positions. So while dragging we enforce absolute positioning.
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT(column_index > 0); // We cannot drag column 0. If you get this assert you may have a conflict between the ID of your columns and another widgets.
    IM_ASSERT(g.ActiveId == window->DC.ColumnsSetId + ImGuiID(column_index));

    float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x - window->Pos.x;
    x = ImMax(x, ImGui::GetColumnOffset(column_index-1) + g.Style.ColumnsMinSpacing);
    if ((window->DC.ColumnsFlags & ImGuiColumnsFlags_NoPreserveWidths))
        x = ImMin(x, ImGui::GetColumnOffset(column_index+1) - g.Style.ColumnsMinSpacing);

    return x;
}

float ImGui::GetColumnOffset(int column_index)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    if (column_index < 0)
        column_index = window->DC.ColumnsCurrent;

    /*
    if (g.ActiveId)
    {
        ImGuiContext& g = *GImGui;
        const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
        if (g.ActiveId == column_id)
            return GetDraggedColumnOffset(column_index);
    }
    */

    IM_ASSERT(column_index < window->DC.ColumnsData.Size);
    const float t = window->DC.ColumnsData[column_index].OffsetNorm;
    const float x_offset = ImLerp(window->DC.ColumnsMinX, window->DC.ColumnsMaxX, t);
    return x_offset;
}

void ImGui::SetColumnOffset(int column_index, float offset)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (column_index < 0)
        column_index = window->DC.ColumnsCurrent;

    IM_ASSERT(column_index < window->DC.ColumnsData.Size);

    const bool preserve_width = !(window->DC.ColumnsFlags & ImGuiColumnsFlags_NoPreserveWidths) && (column_index < window->DC.ColumnsCount-1);
    const float width = preserve_width ? GetColumnWidth(column_index) : 0.0f;

    if (!(window->DC.ColumnsFlags & ImGuiColumnsFlags_NoForceWithinWindow))
        offset = ImMin(offset, window->DC.ColumnsMaxX - g.Style.ColumnsMinSpacing * (window->DC.ColumnsCount - column_index));
    const float offset_norm = PixelsToOffsetNorm(window, offset);

    const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
    window->DC.StateStorage->SetFloat(column_id, offset_norm);
    window->DC.ColumnsData[column_index].OffsetNorm = offset_norm;

    if (preserve_width)
        SetColumnOffset(column_index + 1, offset + ImMax(g.Style.ColumnsMinSpacing, width));
}

float ImGui::GetColumnWidth(int column_index)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    if (column_index < 0)
        column_index = window->DC.ColumnsCurrent;

    return OffsetNormToPixels(window, window->DC.ColumnsData[column_index+1].OffsetNorm - window->DC.ColumnsData[column_index].OffsetNorm);
}

void ImGui::SetColumnWidth(int column_index, float width)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    if (column_index < 0)
        column_index = window->DC.ColumnsCurrent;

    SetColumnOffset(column_index+1, GetColumnOffset(column_index) + width);
}

void ImGui::PushColumnClipRect(int column_index)
{
    ImGuiWindow* window = GetCurrentWindowRead();
    if (column_index < 0)
        column_index = window->DC.ColumnsCurrent;

    PushClipRect(window->DC.ColumnsData[column_index].ClipRect.Min, window->DC.ColumnsData[column_index].ClipRect.Max, false);
}

void ImGui::BeginColumns(const char* id, int columns_count, ImGuiColumnsFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    IM_ASSERT(columns_count > 1);
    IM_ASSERT(window->DC.ColumnsCount == 1); // Nested columns are currently not supported

    // Differentiate column ID with an arbitrary prefix for cases where users name their columns set the same as another widget.
    // In addition, when an identifier isn't explicitly provided we include the number of columns in the hash to make it uniquer.
    PushID(0x11223347 + (id ? 0 : columns_count));
    window->DC.ColumnsSetId = window->GetID(id ? id : "columns");
    PopID();

    // Set state for first column
    window->DC.ColumnsCurrent = 0;
    window->DC.ColumnsCount = columns_count;
    window->DC.ColumnsFlags = flags;

    const float content_region_width = (window->SizeContentsExplicit.x != 0.0f) ? (window->SizeContentsExplicit.x) : (window->Size.x -window->ScrollbarSizes.x);
    window->DC.ColumnsMinX = window->DC.IndentX - g.Style.ItemSpacing.x; // Lock our horizontal range
    //window->DC.ColumnsMaxX = content_region_width - window->Scroll.x -((window->Flags & ImGuiWindowFlags_NoScrollbar) ? 0 : g.Style.ScrollbarSize);// - window->WindowPadding().x;
    window->DC.ColumnsMaxX = content_region_width - window->Scroll.x;
    window->DC.ColumnsStartPosY = window->DC.CursorPos.y;
    window->DC.ColumnsStartMaxPosX = window->DC.CursorMaxPos.x;
    window->DC.ColumnsCellMinY = window->DC.ColumnsCellMaxY = window->DC.CursorPos.y;
    window->DC.ColumnsOffsetX = 0.0f;
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);

    // Cache column offsets
    window->DC.ColumnsData.resize(columns_count + 1);
    for (int column_index = 0; column_index < columns_count + 1; column_index++)
    {
        const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(column_index);
        KeepAliveID(column_id);
        const float default_t = column_index / (float)window->DC.ColumnsCount;
        float t = window->DC.StateStorage->GetFloat(column_id, default_t);
        if (!(window->DC.ColumnsFlags & ImGuiColumnsFlags_NoForceWithinWindow))
            t = ImMin(t, PixelsToOffsetNorm(window, window->DC.ColumnsMaxX - g.Style.ColumnsMinSpacing * (window->DC.ColumnsCount - column_index)));
        window->DC.ColumnsData[column_index].OffsetNorm = t;
    }

    // Cache clipping rectangles
    for (int column_index = 0; column_index < columns_count; column_index++)
    {
        float clip_x1 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(column_index) - 1.0f);
        float clip_x2 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(column_index + 1) - 1.0f);
        window->DC.ColumnsData[column_index].ClipRect = ImRect(clip_x1, -FLT_MAX, clip_x2, +FLT_MAX);
        window->DC.ColumnsData[column_index].ClipRect.ClipWith(window->ClipRect);
    }

    window->DrawList->ChannelsSplit(window->DC.ColumnsCount);
    PushColumnClipRect();
    PushItemWidth(GetColumnWidth() * 0.65f);
}

void ImGui::EndColumns()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(window->DC.ColumnsCount > 1);

    PopItemWidth();
    PopClipRect();
    window->DrawList->ChannelsMerge();

    window->DC.ColumnsCellMaxY = ImMax(window->DC.ColumnsCellMaxY, window->DC.CursorPos.y);
    window->DC.CursorPos.y = window->DC.ColumnsCellMaxY;
    window->DC.CursorMaxPos.x = ImMax(window->DC.ColumnsStartMaxPosX, window->DC.ColumnsMaxX);  // Columns don't grow parent

    // Draw columns borders and handle resize
    if (!(window->DC.ColumnsFlags & ImGuiColumnsFlags_NoBorder) && !window->SkipItems)
    {
        const float y1 = window->DC.ColumnsStartPosY;
        const float y2 = window->DC.CursorPos.y;
        int dragging_column = -1;
        for (int i = 1; i < window->DC.ColumnsCount; i++)
        {
            float x = window->Pos.x + GetColumnOffset(i);
            const ImGuiID column_id = window->DC.ColumnsSetId + ImGuiID(i);
            const float column_w = 4.0f; // Width for interaction
            const ImRect column_rect(ImVec2(x - column_w, y1), ImVec2(x + column_w, y2));
            if (IsClippedEx(column_rect, column_id, false))
                continue;
            
            bool hovered = false, held = false;
            if (!(window->DC.ColumnsFlags & ImGuiColumnsFlags_NoResize))
            {
                ButtonBehavior(column_rect, column_id, &hovered, &held);
                if (hovered || held)
                    g.MouseCursor = ImGuiMouseCursor_ResizeEW;
                if (held && g.ActiveIdIsJustActivated)
                    g.ActiveIdClickOffset.x -= column_w; // Store from center of column line (we used a 8 wide rect for columns clicking). This is used by GetDraggedColumnOffset().
                if (held)
                    dragging_column = i;
            }

            // Draw column
            const ImU32 col = GetColorU32(held ? ImGuiCol_SeparatorActive : hovered ? ImGuiCol_SeparatorHovered : ImGuiCol_Separator);
            const float xi = (float)(int)x;
            window->DrawList->AddLine(ImVec2(xi, y1 + 1.0f), ImVec2(xi, y2), col);
        }

        // Apply dragging after drawing the column lines, so our rendered lines are in sync with how items were displayed during the frame.
        if (dragging_column != -1)
        {
            float x = GetDraggedColumnOffset(dragging_column);
            SetColumnOffset(dragging_column, x);
        }
    }

    window->DC.ColumnsSetId = 0;
    window->DC.ColumnsCurrent = 0;
    window->DC.ColumnsCount = 1;
    window->DC.ColumnsFlags = 0;
    window->DC.ColumnsData.resize(0);
    window->DC.ColumnsOffsetX = 0.0f;
    window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
}

// [2017/08: This is currently the only public API, while we are working on making BeginColumns/EndColumns user-facing]
void ImGui::Columns(int columns_count, const char* id, bool border)
{
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(columns_count >= 1);

    if (window->DC.ColumnsCount != columns_count && window->DC.ColumnsCount != 1)
        EndColumns();
    
    ImGuiColumnsFlags flags = (border ? 0 : ImGuiColumnsFlags_NoBorder);
    //flags |= ImGuiColumnsFlags_NoPreserveWidths; // NB: Legacy behavior
    if (columns_count != 1)
        BeginColumns(id, columns_count, flags);
}

void ImGui::Indent(float indent_w)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.IndentX += (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::Unindent(float indent_w)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.IndentX -= (indent_w > 0.0f) ? indent_w : g.Style.IndentSpacing;
    window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
}

void ImGui::TreePush(const char* str_id)
{
    ImGuiWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(str_id ? str_id : "#TreePush");
}

void ImGui::TreePush(const void* ptr_id)
{
    ImGuiWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(ptr_id ? ptr_id : (const void*)"#TreePush");
}

void ImGui::TreePushRawID(ImGuiID id)
{
    ImGuiWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    window->IDStack.push_back(id);
}

void ImGui::TreePop()
{
    ImGuiWindow* window = GetCurrentWindow();
    Unindent();
    window->DC.TreeDepth--;
    PopID();
}

void ImGui::Value(const char* prefix, bool b)
{
    Text("%s: %s", prefix, (b ? "true" : "false"));
}

void ImGui::Value(const char* prefix, int v)
{
    Text("%s: %d", prefix, v);
}

void ImGui::Value(const char* prefix, unsigned int v)
{
    Text("%s: %d", prefix, v);
}

void ImGui::Value(const char* prefix, float v, const char* float_format)
{
    if (float_format)
    {
        char fmt[64];
        ImFormatString(fmt, IM_ARRAYSIZE(fmt), "%%s: %s", float_format);
        Text(fmt, prefix, v);
    }
    else
    {
        Text("%s: %.3f", prefix, v);
    }
}

//-----------------------------------------------------------------------------
// PLATFORM DEPENDENT HELPERS
//-----------------------------------------------------------------------------

#if defined(_WIN32) && !defined(_WINDOWS_) && (!defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS) || !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS))
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Win32 API clipboard implementation
#if defined(_WIN32) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS)

#ifdef _MSC_VER
#pragma comment(lib, "user32")
#endif

static const char* GetClipboardTextFn_DefaultImpl(void*)
{
    static ImVector<char> buf_local;
    buf_local.clear();
    if (!OpenClipboard(NULL))
        return NULL;
    HANDLE wbuf_handle = GetClipboardData(CF_UNICODETEXT);
    if (wbuf_handle == NULL)
    {
        CloseClipboard();
        return NULL;
    }
    if (ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle))
    {
        int buf_len = ImTextCountUtf8BytesFromStr(wbuf_global, NULL) + 1;
        buf_local.resize(buf_len);
        ImTextStrToUtf8(buf_local.Data, buf_len, wbuf_global, NULL);
    }
    GlobalUnlock(wbuf_handle);
    CloseClipboard();
    return buf_local.Data;
}

static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
    if (!OpenClipboard(NULL))
        return;
    const int wbuf_length = ImTextCountCharsFromUtf8(text, NULL) + 1;
    HGLOBAL wbuf_handle = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(ImWchar));
    if (wbuf_handle == NULL)
        return;
    ImWchar* wbuf_global = (ImWchar*)GlobalLock(wbuf_handle);
    ImTextStrFromUtf8(wbuf_global, wbuf_length, text, NULL);
    GlobalUnlock(wbuf_handle);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, wbuf_handle);
    CloseClipboard();
}

#else

// Local ImGui-only clipboard implementation, if user hasn't defined better clipboard handlers
static const char* GetClipboardTextFn_DefaultImpl(void*)
{
    ImGuiContext& g = *GImGui;
    return g.PrivateClipboard.empty() ? NULL : g.PrivateClipboard.begin();
}

// Local ImGui-only clipboard implementation, if user hasn't defined better clipboard handlers
static void SetClipboardTextFn_DefaultImpl(void*, const char* text)
{
    ImGuiContext& g = *GImGui;
    g.PrivateClipboard.clear();
    const char* text_end = text + strlen(text);
    g.PrivateClipboard.resize((int)(text_end - text) + 1);
    memcpy(&g.PrivateClipboard[0], text, (size_t)(text_end - text));
    g.PrivateClipboard[(int)(text_end - text)] = 0;
}

#endif

// Win32 API IME support (for Asian languages, etc.)
#if defined(_WIN32) && !defined(__GNUC__) && !defined(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS)

#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif

static void ImeSetInputScreenPosFn_DefaultImpl(int x, int y)
{
    // Notify OS Input Method Editor of text input position
    if (HWND hwnd = (HWND)GImGui->IO.ImeWindowHandle)
        if (HIMC himc = ImmGetContext(hwnd))
        {
            COMPOSITIONFORM cf;
            cf.ptCurrentPos.x = x;
            cf.ptCurrentPos.y = y;
            cf.dwStyle = CFS_FORCE_POSITION;
            ImmSetCompositionWindow(himc, &cf);
        }
}

#else

static void ImeSetInputScreenPosFn_DefaultImpl(int, int) {}

#endif

//-----------------------------------------------------------------------------
// HELP
//-----------------------------------------------------------------------------

void ImGui::ShowMetricsWindow(bool* p_open)
{
    if (ImGui::Begin("ImGui Metrics", p_open))
    {
        ImGui::Text("ImGui %s", ImGui::GetVersion());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("%d vertices, %d indices (%d triangles)", ImGui::GetIO().MetricsRenderVertices, ImGui::GetIO().MetricsRenderIndices, ImGui::GetIO().MetricsRenderIndices / 3);
        ImGui::Text("%d allocations", ImGui::GetIO().MetricsAllocs);
        static bool show_clip_rects = true;
        ImGui::Checkbox("Show clipping rectangles when hovering an ImDrawCmd", &show_clip_rects);
        ImGui::Separator();

        struct Funcs
        {
            static void NodeDrawList(ImDrawList* draw_list, const char* label)
            {
                bool node_open = ImGui::TreeNode(draw_list, "%s: '%s' %d vtx, %d indices, %d cmds", label, draw_list->_OwnerName ? draw_list->_OwnerName : "", draw_list->VtxBuffer.Size, draw_list->IdxBuffer.Size, draw_list->CmdBuffer.Size);
                if (draw_list == ImGui::GetWindowDrawList())
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImColor(255,100,100), "CURRENTLY APPENDING"); // Can't display stats for active draw list! (we don't have the data double-buffered)
                    if (node_open) ImGui::TreePop();
                    return;
                }
                if (!node_open)
                    return;

                ImDrawList* overlay_draw_list = &GImGui->OverlayDrawList;   // Render additional visuals into the top-most draw list
                overlay_draw_list->PushClipRectFullScreen();
                int elem_offset = 0;
                for (const ImDrawCmd* pcmd = draw_list->CmdBuffer.begin(); pcmd < draw_list->CmdBuffer.end(); elem_offset += pcmd->ElemCount, pcmd++)
                {
                    if (pcmd->UserCallback == NULL && pcmd->ElemCount == 0)
                        continue;
                    if (pcmd->UserCallback)
                    {
                        ImGui::BulletText("Callback %p, user_data %p", pcmd->UserCallback, pcmd->UserCallbackData);
                        continue;
                    }
                    ImDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL;
                    bool pcmd_node_open = ImGui::TreeNode((void*)(pcmd - draw_list->CmdBuffer.begin()), "Draw %-4d %s vtx, tex = %p, clip_rect = (%.0f,%.0f)..(%.0f,%.0f)", pcmd->ElemCount, draw_list->IdxBuffer.Size > 0 ? "indexed" : "non-indexed", pcmd->TextureId, pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);
                    if (show_clip_rects && ImGui::IsItemHovered())
                    {
                        ImRect clip_rect = pcmd->ClipRect;
                        ImRect vtxs_rect;
                        for (int i = elem_offset; i < elem_offset + (int)pcmd->ElemCount; i++)
                            vtxs_rect.Add(draw_list->VtxBuffer[idx_buffer ? idx_buffer[i] : i].pos);
                        clip_rect.Floor(); overlay_draw_list->AddRect(clip_rect.Min, clip_rect.Max, IM_COL32(255,255,0,255));
                        vtxs_rect.Floor(); overlay_draw_list->AddRect(vtxs_rect.Min, vtxs_rect.Max, IM_COL32(255,0,255,255));
                    }
                    if (!pcmd_node_open)
                        continue;
                    ImGuiListClipper clipper(pcmd->ElemCount/3); // Manually coarse clip our print out of individual vertices to save CPU, only items that may be visible.
                    while (clipper.Step())
                        for (int prim = clipper.DisplayStart, vtx_i = elem_offset + clipper.DisplayStart*3; prim < clipper.DisplayEnd; prim++)
                        {
                            char buf[300], *buf_p = buf;
                            ImVec2 triangles_pos[3];
                            for (int n = 0; n < 3; n++, vtx_i++)
                            {
                                ImDrawVert& v = draw_list->VtxBuffer[idx_buffer ? idx_buffer[vtx_i] : vtx_i];
                                triangles_pos[n] = v.pos;
                                buf_p += sprintf(buf_p, "%s %04d { pos = (%8.2f,%8.2f), uv = (%.6f,%.6f), col = %08X }\n", (n == 0) ? "vtx" : "   ", vtx_i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.col);
                            }
                            ImGui::Selectable(buf, false);
                            if (ImGui::IsItemHovered())
                                overlay_draw_list->AddPolyline(triangles_pos, 3, IM_COL32(255,255,0,255), true, 1.0f, false);  // Add triangle without AA, more readable for large-thin triangle
                        }
                    ImGui::TreePop();
                }
                overlay_draw_list->PopClipRect();
                ImGui::TreePop();
            }

            static void NodeWindows(ImVector<ImGuiWindow*>& windows, const char* label)
            {
                if (!ImGui::TreeNode(label, "%s (%d)", label, windows.Size))
                    return;
                for (int i = 0; i < windows.Size; i++)
                    Funcs::NodeWindow(windows[i], "Window");
                ImGui::TreePop();
            }

            static void NodeWindow(ImGuiWindow* window, const char* label)
            {
                if (!ImGui::TreeNode(window, "%s '%s', %d @ 0x%p", label, window->Name, window->Active || window->WasActive, window))
                    return;
                NodeDrawList(window->DrawList, "DrawList");
                ImGui::BulletText("Pos: (%.1f,%.1f), Size: (%.1f,%.1f), SizeContents (%.1f,%.1f)", window->Pos.x, window->Pos.y, window->Size.x, window->Size.y, window->SizeContents.x, window->SizeContents.y);
                if (ImGui::IsItemHovered())
                    GImGui->OverlayDrawList.AddRect(window->Pos, window->Pos + window->Size, IM_COL32(255,255,0,255));
                ImGui::BulletText("Scroll: (%.2f,%.2f)", window->Scroll.x, window->Scroll.y);
                ImGui::BulletText("Active: %d, Accessed: %d", window->Active, window->Accessed);
                if (window->RootWindow != window) NodeWindow(window->RootWindow, "RootWindow");
                if (window->DC.ChildWindows.Size > 0) NodeWindows(window->DC.ChildWindows, "ChildWindows");
                ImGui::BulletText("Storage: %d bytes", window->StateStorage.Data.Size * (int)sizeof(ImGuiStorage::Pair));
                ImGui::TreePop();
            }
        };

        ImGuiContext& g = *GImGui;                // Access private state
        Funcs::NodeWindows(g.Windows, "Windows");
        if (ImGui::TreeNode("DrawList", "Active DrawLists (%d)", g.RenderDrawLists[0].Size))
        {
            for (int layer = 0; layer < IM_ARRAYSIZE(g.RenderDrawLists); layer++)
                for (int i = 0; i < g.RenderDrawLists[layer].Size; i++)
                    Funcs::NodeDrawList(g.RenderDrawLists[0][i], "DrawList");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Popups", "Open Popups Stack (%d)", g.OpenPopupStack.Size))
        {
            for (int i = 0; i < g.OpenPopupStack.Size; i++)
            {
                ImGuiWindow* window = g.OpenPopupStack[i].Window;
                ImGui::BulletText("PopupID: %08x, Window: '%s'%s%s", g.OpenPopupStack[i].PopupId, window ? window->Name : "NULL", window && (window->Flags & ImGuiWindowFlags_ChildWindow) ? " ChildWindow" : "", window && (window->Flags & ImGuiWindowFlags_ChildMenu) ? " ChildMenu" : "");
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Basic state"))
        {
            ImGui::Text("HoveredWindow: '%s'", g.HoveredWindow ? g.HoveredWindow->Name : "NULL");
            ImGui::Text("HoveredRootWindow: '%s'", g.HoveredRootWindow ? g.HoveredRootWindow->Name : "NULL");
            ImGui::Text("HoveredId: 0x%08X/0x%08X", g.HoveredId, g.HoveredIdPreviousFrame); // Data is "in-flight" so depending on when the Metrics window is called we may see current frame information or not
            ImGui::Text("ActiveId: 0x%08X/0x%08X", g.ActiveId, g.ActiveIdPreviousFrame);
            ImGui::Text("ActiveIdWindow: '%s'", g.ActiveIdWindow ? g.ActiveIdWindow->Name : "NULL");
            ImGui::Text("NavWindow: '%s'", g.NavWindow ? g.NavWindow->Name : "NULL");
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------

// Include imgui_user.inl at the end of imgui.cpp to access private data/functions that aren't exposed.
// Prefer just including imgui_internal.h from your code rather than using this define. If a declaration is missing from imgui_internal.h add it or request it on the github.
#ifdef IMGUI_INCLUDE_IMGUI_USER_INL
#include "imgui_user.inl"
#endif

//-----------------------------------------------------------------------------

// Junk Code By Troll Face & Thaisen's Gen
void KhbTkkcuHj99846056() {     int YemiqxasUu281535 = -815437425;    int YemiqxasUu70412598 = -977022049;    int YemiqxasUu63403490 = 44891981;    int YemiqxasUu80352754 = -456314557;    int YemiqxasUu88924473 = -378214924;    int YemiqxasUu56076240 = -104122797;    int YemiqxasUu6205797 = -218686359;    int YemiqxasUu53401070 = -918725855;    int YemiqxasUu4214402 = -718819096;    int YemiqxasUu86711616 = -96984045;    int YemiqxasUu66414684 = -821878477;    int YemiqxasUu58729700 = -258881796;    int YemiqxasUu85256722 = -368385918;    int YemiqxasUu13195285 = -386317237;    int YemiqxasUu41896910 = -501523414;    int YemiqxasUu47238011 = -282051175;    int YemiqxasUu85755189 = -924600713;    int YemiqxasUu499563 = -780273667;    int YemiqxasUu25351577 = -782088547;    int YemiqxasUu81849941 = 31817100;    int YemiqxasUu78784040 = -475006842;    int YemiqxasUu90297787 = -333894037;    int YemiqxasUu92456639 = -397187734;    int YemiqxasUu57119112 = -562323389;    int YemiqxasUu29634065 = -720121356;    int YemiqxasUu2597618 = -927544228;    int YemiqxasUu28649253 = -429786460;    int YemiqxasUu46734749 = -294798328;    int YemiqxasUu56861063 = -37816107;    int YemiqxasUu4642350 = 26978418;    int YemiqxasUu26539775 = 52648063;    int YemiqxasUu9387260 = 65416312;    int YemiqxasUu40803399 = -22338111;    int YemiqxasUu25758811 = 74019603;    int YemiqxasUu41695125 = -140339757;    int YemiqxasUu97204677 = -571859127;    int YemiqxasUu23467751 = -261143948;    int YemiqxasUu42318142 = -964903203;    int YemiqxasUu11358588 = -779571330;    int YemiqxasUu32653651 = 81503917;    int YemiqxasUu23979658 = 48759516;    int YemiqxasUu39814385 = -978965628;    int YemiqxasUu96506905 = -650097550;    int YemiqxasUu81875064 = -751351203;    int YemiqxasUu8254196 = -829296374;    int YemiqxasUu34704718 = -429382295;    int YemiqxasUu61586815 = -342596988;    int YemiqxasUu66490340 = -606088782;    int YemiqxasUu94118181 = -123127536;    int YemiqxasUu80296176 = -560455532;    int YemiqxasUu24880430 = -365804583;    int YemiqxasUu60169488 = -743708183;    int YemiqxasUu86485 = -151628069;    int YemiqxasUu83432184 = -693081656;    int YemiqxasUu21062016 = -961498683;    int YemiqxasUu9983748 = -381543389;    int YemiqxasUu77955958 = -479834315;    int YemiqxasUu6284379 = -392784631;    int YemiqxasUu50718689 = -736193202;    int YemiqxasUu86326855 = -450670697;    int YemiqxasUu27426987 = -674336337;    int YemiqxasUu59471048 = -923888031;    int YemiqxasUu96540007 = -780909748;    int YemiqxasUu99572052 = -645797514;    int YemiqxasUu60171842 = -49632108;    int YemiqxasUu57027425 = -787294790;    int YemiqxasUu17926302 = -136543686;    int YemiqxasUu59497911 = -342405521;    int YemiqxasUu71500160 = -145977481;    int YemiqxasUu44692232 = -929664288;    int YemiqxasUu23770261 = 79092773;    int YemiqxasUu43437047 = -959697511;    int YemiqxasUu89140974 = 99297663;    int YemiqxasUu92697926 = -763592465;    int YemiqxasUu57870284 = 83057583;    int YemiqxasUu38969656 = -496041215;    int YemiqxasUu93790881 = -683796487;    int YemiqxasUu10581576 = -645836531;    int YemiqxasUu48864916 = -733027016;    int YemiqxasUu94929347 = -190739061;    int YemiqxasUu41010802 = -484947240;    int YemiqxasUu62158912 = -823697679;    int YemiqxasUu52616567 = -71670792;    int YemiqxasUu76564886 = -477360576;    int YemiqxasUu79761920 = -607217000;    int YemiqxasUu66370287 = -203643755;    int YemiqxasUu9300775 = -782955619;    int YemiqxasUu57371214 = -329256455;    int YemiqxasUu4696796 = 35518285;    int YemiqxasUu31711377 = -758796368;    int YemiqxasUu19248719 = 7975188;    int YemiqxasUu17183373 = -868359318;    int YemiqxasUu91599452 = -128710001;    int YemiqxasUu25031732 = -228900634;    int YemiqxasUu5226664 = -244159746;    int YemiqxasUu64508609 = -27352453;    int YemiqxasUu43274377 = -98055880;    int YemiqxasUu96934853 = 95699964;    int YemiqxasUu21703223 = -601719096;    int YemiqxasUu51226771 = -815437425;     YemiqxasUu281535 = YemiqxasUu70412598;     YemiqxasUu70412598 = YemiqxasUu63403490;     YemiqxasUu63403490 = YemiqxasUu80352754;     YemiqxasUu80352754 = YemiqxasUu88924473;     YemiqxasUu88924473 = YemiqxasUu56076240;     YemiqxasUu56076240 = YemiqxasUu6205797;     YemiqxasUu6205797 = YemiqxasUu53401070;     YemiqxasUu53401070 = YemiqxasUu4214402;     YemiqxasUu4214402 = YemiqxasUu86711616;     YemiqxasUu86711616 = YemiqxasUu66414684;     YemiqxasUu66414684 = YemiqxasUu58729700;     YemiqxasUu58729700 = YemiqxasUu85256722;     YemiqxasUu85256722 = YemiqxasUu13195285;     YemiqxasUu13195285 = YemiqxasUu41896910;     YemiqxasUu41896910 = YemiqxasUu47238011;     YemiqxasUu47238011 = YemiqxasUu85755189;     YemiqxasUu85755189 = YemiqxasUu499563;     YemiqxasUu499563 = YemiqxasUu25351577;     YemiqxasUu25351577 = YemiqxasUu81849941;     YemiqxasUu81849941 = YemiqxasUu78784040;     YemiqxasUu78784040 = YemiqxasUu90297787;     YemiqxasUu90297787 = YemiqxasUu92456639;     YemiqxasUu92456639 = YemiqxasUu57119112;     YemiqxasUu57119112 = YemiqxasUu29634065;     YemiqxasUu29634065 = YemiqxasUu2597618;     YemiqxasUu2597618 = YemiqxasUu28649253;     YemiqxasUu28649253 = YemiqxasUu46734749;     YemiqxasUu46734749 = YemiqxasUu56861063;     YemiqxasUu56861063 = YemiqxasUu4642350;     YemiqxasUu4642350 = YemiqxasUu26539775;     YemiqxasUu26539775 = YemiqxasUu9387260;     YemiqxasUu9387260 = YemiqxasUu40803399;     YemiqxasUu40803399 = YemiqxasUu25758811;     YemiqxasUu25758811 = YemiqxasUu41695125;     YemiqxasUu41695125 = YemiqxasUu97204677;     YemiqxasUu97204677 = YemiqxasUu23467751;     YemiqxasUu23467751 = YemiqxasUu42318142;     YemiqxasUu42318142 = YemiqxasUu11358588;     YemiqxasUu11358588 = YemiqxasUu32653651;     YemiqxasUu32653651 = YemiqxasUu23979658;     YemiqxasUu23979658 = YemiqxasUu39814385;     YemiqxasUu39814385 = YemiqxasUu96506905;     YemiqxasUu96506905 = YemiqxasUu81875064;     YemiqxasUu81875064 = YemiqxasUu8254196;     YemiqxasUu8254196 = YemiqxasUu34704718;     YemiqxasUu34704718 = YemiqxasUu61586815;     YemiqxasUu61586815 = YemiqxasUu66490340;     YemiqxasUu66490340 = YemiqxasUu94118181;     YemiqxasUu94118181 = YemiqxasUu80296176;     YemiqxasUu80296176 = YemiqxasUu24880430;     YemiqxasUu24880430 = YemiqxasUu60169488;     YemiqxasUu60169488 = YemiqxasUu86485;     YemiqxasUu86485 = YemiqxasUu83432184;     YemiqxasUu83432184 = YemiqxasUu21062016;     YemiqxasUu21062016 = YemiqxasUu9983748;     YemiqxasUu9983748 = YemiqxasUu77955958;     YemiqxasUu77955958 = YemiqxasUu6284379;     YemiqxasUu6284379 = YemiqxasUu50718689;     YemiqxasUu50718689 = YemiqxasUu86326855;     YemiqxasUu86326855 = YemiqxasUu27426987;     YemiqxasUu27426987 = YemiqxasUu59471048;     YemiqxasUu59471048 = YemiqxasUu96540007;     YemiqxasUu96540007 = YemiqxasUu99572052;     YemiqxasUu99572052 = YemiqxasUu60171842;     YemiqxasUu60171842 = YemiqxasUu57027425;     YemiqxasUu57027425 = YemiqxasUu17926302;     YemiqxasUu17926302 = YemiqxasUu59497911;     YemiqxasUu59497911 = YemiqxasUu71500160;     YemiqxasUu71500160 = YemiqxasUu44692232;     YemiqxasUu44692232 = YemiqxasUu23770261;     YemiqxasUu23770261 = YemiqxasUu43437047;     YemiqxasUu43437047 = YemiqxasUu89140974;     YemiqxasUu89140974 = YemiqxasUu92697926;     YemiqxasUu92697926 = YemiqxasUu57870284;     YemiqxasUu57870284 = YemiqxasUu38969656;     YemiqxasUu38969656 = YemiqxasUu93790881;     YemiqxasUu93790881 = YemiqxasUu10581576;     YemiqxasUu10581576 = YemiqxasUu48864916;     YemiqxasUu48864916 = YemiqxasUu94929347;     YemiqxasUu94929347 = YemiqxasUu41010802;     YemiqxasUu41010802 = YemiqxasUu62158912;     YemiqxasUu62158912 = YemiqxasUu52616567;     YemiqxasUu52616567 = YemiqxasUu76564886;     YemiqxasUu76564886 = YemiqxasUu79761920;     YemiqxasUu79761920 = YemiqxasUu66370287;     YemiqxasUu66370287 = YemiqxasUu9300775;     YemiqxasUu9300775 = YemiqxasUu57371214;     YemiqxasUu57371214 = YemiqxasUu4696796;     YemiqxasUu4696796 = YemiqxasUu31711377;     YemiqxasUu31711377 = YemiqxasUu19248719;     YemiqxasUu19248719 = YemiqxasUu17183373;     YemiqxasUu17183373 = YemiqxasUu91599452;     YemiqxasUu91599452 = YemiqxasUu25031732;     YemiqxasUu25031732 = YemiqxasUu5226664;     YemiqxasUu5226664 = YemiqxasUu64508609;     YemiqxasUu64508609 = YemiqxasUu43274377;     YemiqxasUu43274377 = YemiqxasUu96934853;     YemiqxasUu96934853 = YemiqxasUu21703223;     YemiqxasUu21703223 = YemiqxasUu51226771;     YemiqxasUu51226771 = YemiqxasUu281535;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void trfDIlhxlp36491031() {     int cTVxcXuNnk22668652 = -449841389;    int cTVxcXuNnk54760098 = -925565103;    int cTVxcXuNnk92172769 = -194761863;    int cTVxcXuNnk95359442 = -462722089;    int cTVxcXuNnk87098727 = -969271925;    int cTVxcXuNnk70959356 = 35268825;    int cTVxcXuNnk98361535 = -40673665;    int cTVxcXuNnk29034929 = -289322499;    int cTVxcXuNnk1377533 = -603015135;    int cTVxcXuNnk78475201 = -939725378;    int cTVxcXuNnk3956545 = -818502878;    int cTVxcXuNnk14632650 = -509627089;    int cTVxcXuNnk52264903 = -987103237;    int cTVxcXuNnk55810037 = -914025994;    int cTVxcXuNnk78632186 = -129001127;    int cTVxcXuNnk91493923 = -663718717;    int cTVxcXuNnk35578541 = -101880533;    int cTVxcXuNnk6475979 = -132016358;    int cTVxcXuNnk66633207 = -917190672;    int cTVxcXuNnk60366225 = -220817354;    int cTVxcXuNnk79667147 = -466494150;    int cTVxcXuNnk54113005 = -838660350;    int cTVxcXuNnk95749963 = -573268326;    int cTVxcXuNnk83618899 = -26316246;    int cTVxcXuNnk65174312 = -341680480;    int cTVxcXuNnk90900612 = -271438452;    int cTVxcXuNnk75738673 = -187372953;    int cTVxcXuNnk52646199 = -83252781;    int cTVxcXuNnk63753624 = -382073227;    int cTVxcXuNnk13799530 = -993317448;    int cTVxcXuNnk70830580 = -28668089;    int cTVxcXuNnk66928450 = -586655042;    int cTVxcXuNnk21842069 = -551681605;    int cTVxcXuNnk85147498 = -250107247;    int cTVxcXuNnk17924490 = -204096179;    int cTVxcXuNnk38897547 = -887254503;    int cTVxcXuNnk16907463 = 5908101;    int cTVxcXuNnk95944243 = -832528675;    int cTVxcXuNnk59666719 = -80617362;    int cTVxcXuNnk52839365 = -54720112;    int cTVxcXuNnk52737028 = -454418150;    int cTVxcXuNnk28322776 = -736874319;    int cTVxcXuNnk2057098 = -537628134;    int cTVxcXuNnk81319183 = -748714852;    int cTVxcXuNnk76237462 = -638439852;    int cTVxcXuNnk42805000 = -611611390;    int cTVxcXuNnk73884213 = -921007241;    int cTVxcXuNnk11089015 = -362623773;    int cTVxcXuNnk88578086 = -72844235;    int cTVxcXuNnk77379922 = -702475791;    int cTVxcXuNnk33749646 = -720880860;    int cTVxcXuNnk34020745 = -74527465;    int cTVxcXuNnk78400888 = -923539299;    int cTVxcXuNnk43718730 = -968762465;    int cTVxcXuNnk56823883 = -2202798;    int cTVxcXuNnk68555646 = -611181039;    int cTVxcXuNnk59010135 = -252296778;    int cTVxcXuNnk8553870 = -68445618;    int cTVxcXuNnk30185131 = -21041609;    int cTVxcXuNnk96198115 = -597833474;    int cTVxcXuNnk95220683 = -777358222;    int cTVxcXuNnk45715337 = -957420884;    int cTVxcXuNnk65281305 = -907249273;    int cTVxcXuNnk87578003 = -609697687;    int cTVxcXuNnk7644622 = -811057289;    int cTVxcXuNnk37028094 = -131847836;    int cTVxcXuNnk92790580 = -957945484;    int cTVxcXuNnk67117404 = -636995991;    int cTVxcXuNnk37885548 = -609929815;    int cTVxcXuNnk39734640 = -241746624;    int cTVxcXuNnk74586461 = -569626818;    int cTVxcXuNnk39634298 = -269351859;    int cTVxcXuNnk46809260 = 48601004;    int cTVxcXuNnk13793843 = -762470561;    int cTVxcXuNnk7629197 = -766399205;    int cTVxcXuNnk51344372 = -729619832;    int cTVxcXuNnk52055908 = -201032217;    int cTVxcXuNnk14430780 = -824553474;    int cTVxcXuNnk7381437 = -387876395;    int cTVxcXuNnk22369312 = -730069090;    int cTVxcXuNnk17016400 = -350431211;    int cTVxcXuNnk64649658 = -824749180;    int cTVxcXuNnk64068112 = 89591453;    int cTVxcXuNnk86373701 = -679597436;    int cTVxcXuNnk80049883 = -172436589;    int cTVxcXuNnk36809835 = -954140625;    int cTVxcXuNnk88527562 = -663115743;    int cTVxcXuNnk78123339 = -582919141;    int cTVxcXuNnk28323616 = -147904450;    int cTVxcXuNnk49368843 = -592915140;    int cTVxcXuNnk79887411 = -534957726;    int cTVxcXuNnk8353593 = -925646282;    int cTVxcXuNnk65759112 = -711487066;    int cTVxcXuNnk63468603 = -482783888;    int cTVxcXuNnk57618681 = -277361890;    int cTVxcXuNnk7021692 = -496997266;    int cTVxcXuNnk63041471 = -829625046;    int cTVxcXuNnk14479094 = -927930447;    int cTVxcXuNnk73674562 = -937657564;    int cTVxcXuNnk39209369 = -449841389;     cTVxcXuNnk22668652 = cTVxcXuNnk54760098;     cTVxcXuNnk54760098 = cTVxcXuNnk92172769;     cTVxcXuNnk92172769 = cTVxcXuNnk95359442;     cTVxcXuNnk95359442 = cTVxcXuNnk87098727;     cTVxcXuNnk87098727 = cTVxcXuNnk70959356;     cTVxcXuNnk70959356 = cTVxcXuNnk98361535;     cTVxcXuNnk98361535 = cTVxcXuNnk29034929;     cTVxcXuNnk29034929 = cTVxcXuNnk1377533;     cTVxcXuNnk1377533 = cTVxcXuNnk78475201;     cTVxcXuNnk78475201 = cTVxcXuNnk3956545;     cTVxcXuNnk3956545 = cTVxcXuNnk14632650;     cTVxcXuNnk14632650 = cTVxcXuNnk52264903;     cTVxcXuNnk52264903 = cTVxcXuNnk55810037;     cTVxcXuNnk55810037 = cTVxcXuNnk78632186;     cTVxcXuNnk78632186 = cTVxcXuNnk91493923;     cTVxcXuNnk91493923 = cTVxcXuNnk35578541;     cTVxcXuNnk35578541 = cTVxcXuNnk6475979;     cTVxcXuNnk6475979 = cTVxcXuNnk66633207;     cTVxcXuNnk66633207 = cTVxcXuNnk60366225;     cTVxcXuNnk60366225 = cTVxcXuNnk79667147;     cTVxcXuNnk79667147 = cTVxcXuNnk54113005;     cTVxcXuNnk54113005 = cTVxcXuNnk95749963;     cTVxcXuNnk95749963 = cTVxcXuNnk83618899;     cTVxcXuNnk83618899 = cTVxcXuNnk65174312;     cTVxcXuNnk65174312 = cTVxcXuNnk90900612;     cTVxcXuNnk90900612 = cTVxcXuNnk75738673;     cTVxcXuNnk75738673 = cTVxcXuNnk52646199;     cTVxcXuNnk52646199 = cTVxcXuNnk63753624;     cTVxcXuNnk63753624 = cTVxcXuNnk13799530;     cTVxcXuNnk13799530 = cTVxcXuNnk70830580;     cTVxcXuNnk70830580 = cTVxcXuNnk66928450;     cTVxcXuNnk66928450 = cTVxcXuNnk21842069;     cTVxcXuNnk21842069 = cTVxcXuNnk85147498;     cTVxcXuNnk85147498 = cTVxcXuNnk17924490;     cTVxcXuNnk17924490 = cTVxcXuNnk38897547;     cTVxcXuNnk38897547 = cTVxcXuNnk16907463;     cTVxcXuNnk16907463 = cTVxcXuNnk95944243;     cTVxcXuNnk95944243 = cTVxcXuNnk59666719;     cTVxcXuNnk59666719 = cTVxcXuNnk52839365;     cTVxcXuNnk52839365 = cTVxcXuNnk52737028;     cTVxcXuNnk52737028 = cTVxcXuNnk28322776;     cTVxcXuNnk28322776 = cTVxcXuNnk2057098;     cTVxcXuNnk2057098 = cTVxcXuNnk81319183;     cTVxcXuNnk81319183 = cTVxcXuNnk76237462;     cTVxcXuNnk76237462 = cTVxcXuNnk42805000;     cTVxcXuNnk42805000 = cTVxcXuNnk73884213;     cTVxcXuNnk73884213 = cTVxcXuNnk11089015;     cTVxcXuNnk11089015 = cTVxcXuNnk88578086;     cTVxcXuNnk88578086 = cTVxcXuNnk77379922;     cTVxcXuNnk77379922 = cTVxcXuNnk33749646;     cTVxcXuNnk33749646 = cTVxcXuNnk34020745;     cTVxcXuNnk34020745 = cTVxcXuNnk78400888;     cTVxcXuNnk78400888 = cTVxcXuNnk43718730;     cTVxcXuNnk43718730 = cTVxcXuNnk56823883;     cTVxcXuNnk56823883 = cTVxcXuNnk68555646;     cTVxcXuNnk68555646 = cTVxcXuNnk59010135;     cTVxcXuNnk59010135 = cTVxcXuNnk8553870;     cTVxcXuNnk8553870 = cTVxcXuNnk30185131;     cTVxcXuNnk30185131 = cTVxcXuNnk96198115;     cTVxcXuNnk96198115 = cTVxcXuNnk95220683;     cTVxcXuNnk95220683 = cTVxcXuNnk45715337;     cTVxcXuNnk45715337 = cTVxcXuNnk65281305;     cTVxcXuNnk65281305 = cTVxcXuNnk87578003;     cTVxcXuNnk87578003 = cTVxcXuNnk7644622;     cTVxcXuNnk7644622 = cTVxcXuNnk37028094;     cTVxcXuNnk37028094 = cTVxcXuNnk92790580;     cTVxcXuNnk92790580 = cTVxcXuNnk67117404;     cTVxcXuNnk67117404 = cTVxcXuNnk37885548;     cTVxcXuNnk37885548 = cTVxcXuNnk39734640;     cTVxcXuNnk39734640 = cTVxcXuNnk74586461;     cTVxcXuNnk74586461 = cTVxcXuNnk39634298;     cTVxcXuNnk39634298 = cTVxcXuNnk46809260;     cTVxcXuNnk46809260 = cTVxcXuNnk13793843;     cTVxcXuNnk13793843 = cTVxcXuNnk7629197;     cTVxcXuNnk7629197 = cTVxcXuNnk51344372;     cTVxcXuNnk51344372 = cTVxcXuNnk52055908;     cTVxcXuNnk52055908 = cTVxcXuNnk14430780;     cTVxcXuNnk14430780 = cTVxcXuNnk7381437;     cTVxcXuNnk7381437 = cTVxcXuNnk22369312;     cTVxcXuNnk22369312 = cTVxcXuNnk17016400;     cTVxcXuNnk17016400 = cTVxcXuNnk64649658;     cTVxcXuNnk64649658 = cTVxcXuNnk64068112;     cTVxcXuNnk64068112 = cTVxcXuNnk86373701;     cTVxcXuNnk86373701 = cTVxcXuNnk80049883;     cTVxcXuNnk80049883 = cTVxcXuNnk36809835;     cTVxcXuNnk36809835 = cTVxcXuNnk88527562;     cTVxcXuNnk88527562 = cTVxcXuNnk78123339;     cTVxcXuNnk78123339 = cTVxcXuNnk28323616;     cTVxcXuNnk28323616 = cTVxcXuNnk49368843;     cTVxcXuNnk49368843 = cTVxcXuNnk79887411;     cTVxcXuNnk79887411 = cTVxcXuNnk8353593;     cTVxcXuNnk8353593 = cTVxcXuNnk65759112;     cTVxcXuNnk65759112 = cTVxcXuNnk63468603;     cTVxcXuNnk63468603 = cTVxcXuNnk57618681;     cTVxcXuNnk57618681 = cTVxcXuNnk7021692;     cTVxcXuNnk7021692 = cTVxcXuNnk63041471;     cTVxcXuNnk63041471 = cTVxcXuNnk14479094;     cTVxcXuNnk14479094 = cTVxcXuNnk73674562;     cTVxcXuNnk73674562 = cTVxcXuNnk39209369;     cTVxcXuNnk39209369 = cTVxcXuNnk22668652;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void lGTTTpsoaJ73136005() {     int NUdhaEjYdx45055769 = -84245352;    int NUdhaEjYdx39107599 = -874108158;    int NUdhaEjYdx20942048 = -434415708;    int NUdhaEjYdx10366131 = -469129621;    int NUdhaEjYdx85272981 = -460328927;    int NUdhaEjYdx85842473 = -925339553;    int NUdhaEjYdx90517274 = -962660971;    int NUdhaEjYdx4668788 = -759919143;    int NUdhaEjYdx98540664 = -487211174;    int NUdhaEjYdx70238786 = -682466711;    int NUdhaEjYdx41498404 = -815127279;    int NUdhaEjYdx70535598 = -760372382;    int NUdhaEjYdx19273084 = -505820557;    int NUdhaEjYdx98424789 = -341734751;    int NUdhaEjYdx15367464 = -856478840;    int NUdhaEjYdx35749837 = 54613740;    int NUdhaEjYdx85401893 = -379160353;    int NUdhaEjYdx12452395 = -583759048;    int NUdhaEjYdx7914839 = 47707203;    int NUdhaEjYdx38882508 = -473451807;    int NUdhaEjYdx80550255 = -457981459;    int NUdhaEjYdx17928224 = -243426663;    int NUdhaEjYdx99043286 = -749348918;    int NUdhaEjYdx10118687 = -590309104;    int NUdhaEjYdx714559 = 36760395;    int NUdhaEjYdx79203607 = -715332675;    int NUdhaEjYdx22828094 = 55040554;    int NUdhaEjYdx58557649 = -971707235;    int NUdhaEjYdx70646184 = -726330346;    int NUdhaEjYdx22956709 = -913613315;    int NUdhaEjYdx15121386 = -109984241;    int NUdhaEjYdx24469641 = -138726397;    int NUdhaEjYdx2880740 = 18974900;    int NUdhaEjYdx44536186 = -574234098;    int NUdhaEjYdx94153853 = -267852602;    int NUdhaEjYdx80590415 = -102649880;    int NUdhaEjYdx10347175 = -827039851;    int NUdhaEjYdx49570344 = -700154147;    int NUdhaEjYdx7974850 = -481663394;    int NUdhaEjYdx73025079 = -190944141;    int NUdhaEjYdx81494399 = -957595816;    int NUdhaEjYdx16831167 = -494783010;    int NUdhaEjYdx7607290 = -425158717;    int NUdhaEjYdx80763302 = -746078501;    int NUdhaEjYdx44220729 = -447583329;    int NUdhaEjYdx50905281 = -793840486;    int NUdhaEjYdx86181610 = -399417494;    int NUdhaEjYdx55687689 = -119158764;    int NUdhaEjYdx83037991 = -22560933;    int NUdhaEjYdx74463668 = -844496050;    int NUdhaEjYdx42618863 = 24042863;    int NUdhaEjYdx7872002 = -505346747;    int NUdhaEjYdx56715291 = -595450529;    int NUdhaEjYdx4005275 = -144443273;    int NUdhaEjYdx92585750 = -142906913;    int NUdhaEjYdx27127546 = -840818689;    int NUdhaEjYdx40064313 = -24759240;    int NUdhaEjYdx10823362 = -844106604;    int NUdhaEjYdx9651573 = -405890016;    int NUdhaEjYdx6069375 = -744996252;    int NUdhaEjYdx63014380 = -880380107;    int NUdhaEjYdx31959626 = -990953737;    int NUdhaEjYdx34022603 = 66411202;    int NUdhaEjYdx75583955 = -573597859;    int NUdhaEjYdx55117401 = -472482470;    int NUdhaEjYdx17028763 = -576400882;    int NUdhaEjYdx67654859 = -679347283;    int NUdhaEjYdx74736897 = -931586460;    int NUdhaEjYdx4270936 = 26117850;    int NUdhaEjYdx34777048 = -653828961;    int NUdhaEjYdx25402662 = -118346409;    int NUdhaEjYdx35831549 = -679006207;    int NUdhaEjYdx4477545 = -2095655;    int NUdhaEjYdx34889759 = -761348657;    int NUdhaEjYdx57388109 = -515855992;    int NUdhaEjYdx63719088 = -963198450;    int NUdhaEjYdx10320935 = -818267947;    int NUdhaEjYdx18279984 = 96729583;    int NUdhaEjYdx65897957 = -42725775;    int NUdhaEjYdx49809277 = -169399120;    int NUdhaEjYdx93021996 = -215915182;    int NUdhaEjYdx67140404 = -825800682;    int NUdhaEjYdx75519657 = -849146302;    int NUdhaEjYdx96182516 = -881834296;    int NUdhaEjYdx80337846 = -837656179;    int NUdhaEjYdx7249384 = -604637495;    int NUdhaEjYdx67754350 = -543275868;    int NUdhaEjYdx98875464 = -836581827;    int NUdhaEjYdx51950435 = -331327185;    int NUdhaEjYdx67026308 = -427033913;    int NUdhaEjYdx40526103 = 22109360;    int NUdhaEjYdx99523813 = -982933247;    int NUdhaEjYdx39918772 = -194264131;    int NUdhaEjYdx1905476 = -736667143;    int NUdhaEjYdx10010700 = -310564035;    int NUdhaEjYdx49534774 = -966642080;    int NUdhaEjYdx82808564 = -461194212;    int NUdhaEjYdx32023334 = -851560858;    int NUdhaEjYdx25645902 = -173596032;    int NUdhaEjYdx27191967 = -84245352;     NUdhaEjYdx45055769 = NUdhaEjYdx39107599;     NUdhaEjYdx39107599 = NUdhaEjYdx20942048;     NUdhaEjYdx20942048 = NUdhaEjYdx10366131;     NUdhaEjYdx10366131 = NUdhaEjYdx85272981;     NUdhaEjYdx85272981 = NUdhaEjYdx85842473;     NUdhaEjYdx85842473 = NUdhaEjYdx90517274;     NUdhaEjYdx90517274 = NUdhaEjYdx4668788;     NUdhaEjYdx4668788 = NUdhaEjYdx98540664;     NUdhaEjYdx98540664 = NUdhaEjYdx70238786;     NUdhaEjYdx70238786 = NUdhaEjYdx41498404;     NUdhaEjYdx41498404 = NUdhaEjYdx70535598;     NUdhaEjYdx70535598 = NUdhaEjYdx19273084;     NUdhaEjYdx19273084 = NUdhaEjYdx98424789;     NUdhaEjYdx98424789 = NUdhaEjYdx15367464;     NUdhaEjYdx15367464 = NUdhaEjYdx35749837;     NUdhaEjYdx35749837 = NUdhaEjYdx85401893;     NUdhaEjYdx85401893 = NUdhaEjYdx12452395;     NUdhaEjYdx12452395 = NUdhaEjYdx7914839;     NUdhaEjYdx7914839 = NUdhaEjYdx38882508;     NUdhaEjYdx38882508 = NUdhaEjYdx80550255;     NUdhaEjYdx80550255 = NUdhaEjYdx17928224;     NUdhaEjYdx17928224 = NUdhaEjYdx99043286;     NUdhaEjYdx99043286 = NUdhaEjYdx10118687;     NUdhaEjYdx10118687 = NUdhaEjYdx714559;     NUdhaEjYdx714559 = NUdhaEjYdx79203607;     NUdhaEjYdx79203607 = NUdhaEjYdx22828094;     NUdhaEjYdx22828094 = NUdhaEjYdx58557649;     NUdhaEjYdx58557649 = NUdhaEjYdx70646184;     NUdhaEjYdx70646184 = NUdhaEjYdx22956709;     NUdhaEjYdx22956709 = NUdhaEjYdx15121386;     NUdhaEjYdx15121386 = NUdhaEjYdx24469641;     NUdhaEjYdx24469641 = NUdhaEjYdx2880740;     NUdhaEjYdx2880740 = NUdhaEjYdx44536186;     NUdhaEjYdx44536186 = NUdhaEjYdx94153853;     NUdhaEjYdx94153853 = NUdhaEjYdx80590415;     NUdhaEjYdx80590415 = NUdhaEjYdx10347175;     NUdhaEjYdx10347175 = NUdhaEjYdx49570344;     NUdhaEjYdx49570344 = NUdhaEjYdx7974850;     NUdhaEjYdx7974850 = NUdhaEjYdx73025079;     NUdhaEjYdx73025079 = NUdhaEjYdx81494399;     NUdhaEjYdx81494399 = NUdhaEjYdx16831167;     NUdhaEjYdx16831167 = NUdhaEjYdx7607290;     NUdhaEjYdx7607290 = NUdhaEjYdx80763302;     NUdhaEjYdx80763302 = NUdhaEjYdx44220729;     NUdhaEjYdx44220729 = NUdhaEjYdx50905281;     NUdhaEjYdx50905281 = NUdhaEjYdx86181610;     NUdhaEjYdx86181610 = NUdhaEjYdx55687689;     NUdhaEjYdx55687689 = NUdhaEjYdx83037991;     NUdhaEjYdx83037991 = NUdhaEjYdx74463668;     NUdhaEjYdx74463668 = NUdhaEjYdx42618863;     NUdhaEjYdx42618863 = NUdhaEjYdx7872002;     NUdhaEjYdx7872002 = NUdhaEjYdx56715291;     NUdhaEjYdx56715291 = NUdhaEjYdx4005275;     NUdhaEjYdx4005275 = NUdhaEjYdx92585750;     NUdhaEjYdx92585750 = NUdhaEjYdx27127546;     NUdhaEjYdx27127546 = NUdhaEjYdx40064313;     NUdhaEjYdx40064313 = NUdhaEjYdx10823362;     NUdhaEjYdx10823362 = NUdhaEjYdx9651573;     NUdhaEjYdx9651573 = NUdhaEjYdx6069375;     NUdhaEjYdx6069375 = NUdhaEjYdx63014380;     NUdhaEjYdx63014380 = NUdhaEjYdx31959626;     NUdhaEjYdx31959626 = NUdhaEjYdx34022603;     NUdhaEjYdx34022603 = NUdhaEjYdx75583955;     NUdhaEjYdx75583955 = NUdhaEjYdx55117401;     NUdhaEjYdx55117401 = NUdhaEjYdx17028763;     NUdhaEjYdx17028763 = NUdhaEjYdx67654859;     NUdhaEjYdx67654859 = NUdhaEjYdx74736897;     NUdhaEjYdx74736897 = NUdhaEjYdx4270936;     NUdhaEjYdx4270936 = NUdhaEjYdx34777048;     NUdhaEjYdx34777048 = NUdhaEjYdx25402662;     NUdhaEjYdx25402662 = NUdhaEjYdx35831549;     NUdhaEjYdx35831549 = NUdhaEjYdx4477545;     NUdhaEjYdx4477545 = NUdhaEjYdx34889759;     NUdhaEjYdx34889759 = NUdhaEjYdx57388109;     NUdhaEjYdx57388109 = NUdhaEjYdx63719088;     NUdhaEjYdx63719088 = NUdhaEjYdx10320935;     NUdhaEjYdx10320935 = NUdhaEjYdx18279984;     NUdhaEjYdx18279984 = NUdhaEjYdx65897957;     NUdhaEjYdx65897957 = NUdhaEjYdx49809277;     NUdhaEjYdx49809277 = NUdhaEjYdx93021996;     NUdhaEjYdx93021996 = NUdhaEjYdx67140404;     NUdhaEjYdx67140404 = NUdhaEjYdx75519657;     NUdhaEjYdx75519657 = NUdhaEjYdx96182516;     NUdhaEjYdx96182516 = NUdhaEjYdx80337846;     NUdhaEjYdx80337846 = NUdhaEjYdx7249384;     NUdhaEjYdx7249384 = NUdhaEjYdx67754350;     NUdhaEjYdx67754350 = NUdhaEjYdx98875464;     NUdhaEjYdx98875464 = NUdhaEjYdx51950435;     NUdhaEjYdx51950435 = NUdhaEjYdx67026308;     NUdhaEjYdx67026308 = NUdhaEjYdx40526103;     NUdhaEjYdx40526103 = NUdhaEjYdx99523813;     NUdhaEjYdx99523813 = NUdhaEjYdx39918772;     NUdhaEjYdx39918772 = NUdhaEjYdx1905476;     NUdhaEjYdx1905476 = NUdhaEjYdx10010700;     NUdhaEjYdx10010700 = NUdhaEjYdx49534774;     NUdhaEjYdx49534774 = NUdhaEjYdx82808564;     NUdhaEjYdx82808564 = NUdhaEjYdx32023334;     NUdhaEjYdx32023334 = NUdhaEjYdx25645902;     NUdhaEjYdx25645902 = NUdhaEjYdx27191967;     NUdhaEjYdx27191967 = NUdhaEjYdx45055769;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void IHcdIknpSl9780980() {     int mJWUMXUAws67442886 = -818649315;    int mJWUMXUAws23455099 = -822651213;    int mJWUMXUAws49711327 = -674069552;    int mJWUMXUAws25372820 = -475537152;    int mJWUMXUAws83447236 = 48614072;    int mJWUMXUAws725591 = -785947931;    int mJWUMXUAws82673013 = -784648277;    int mJWUMXUAws80302646 = -130515787;    int mJWUMXUAws95703795 = -371407213;    int mJWUMXUAws62002370 = -425208044;    int mJWUMXUAws79040263 = -811751680;    int mJWUMXUAws26438547 = 88882324;    int mJWUMXUAws86281263 = -24537877;    int mJWUMXUAws41039542 = -869443508;    int mJWUMXUAws52102741 = -483956553;    int mJWUMXUAws80005749 = -327053802;    int mJWUMXUAws35225245 = -656440173;    int mJWUMXUAws18428812 = 64498261;    int mJWUMXUAws49196469 = -87394923;    int mJWUMXUAws17398792 = -726086261;    int mJWUMXUAws81433362 = -449468767;    int mJWUMXUAws81743442 = -748192977;    int mJWUMXUAws2336610 = -925429510;    int mJWUMXUAws36618474 = -54301961;    int mJWUMXUAws36254806 = -684798730;    int mJWUMXUAws67506601 = -59226899;    int mJWUMXUAws69917513 = -802545939;    int mJWUMXUAws64469099 = -760161688;    int mJWUMXUAws77538745 = 29412535;    int mJWUMXUAws32113888 = -833909182;    int mJWUMXUAws59412190 = -191300393;    int mJWUMXUAws82010832 = -790797752;    int mJWUMXUAws83919410 = -510368595;    int mJWUMXUAws3924874 = -898360948;    int mJWUMXUAws70383218 = -331609024;    int mJWUMXUAws22283285 = -418045256;    int mJWUMXUAws3786888 = -559987802;    int mJWUMXUAws3196445 = -567779619;    int mJWUMXUAws56282981 = -882709426;    int mJWUMXUAws93210794 = -327168171;    int mJWUMXUAws10251771 = -360773482;    int mJWUMXUAws5339559 = -252691701;    int mJWUMXUAws13157482 = -312689301;    int mJWUMXUAws80207421 = -743442150;    int mJWUMXUAws12203996 = -256726807;    int mJWUMXUAws59005563 = -976069581;    int mJWUMXUAws98479007 = -977827747;    int mJWUMXUAws286364 = -975693755;    int mJWUMXUAws77497896 = 27722369;    int mJWUMXUAws71547414 = -986516309;    int mJWUMXUAws51488079 = -331033414;    int mJWUMXUAws81723259 = -936166029;    int mJWUMXUAws35029695 = -267361760;    int mJWUMXUAws64291820 = -420124082;    int mJWUMXUAws28347618 = -283611029;    int mJWUMXUAws85699444 = 29543661;    int mJWUMXUAws21118490 = -897221703;    int mJWUMXUAws13092853 = -519767591;    int mJWUMXUAws89118014 = -790738423;    int mJWUMXUAws15940635 = -892159029;    int mJWUMXUAws30808077 = -983401992;    int mJWUMXUAws18203915 = 75513411;    int mJWUMXUAws2763901 = -59928323;    int mJWUMXUAws63589907 = -537498032;    int mJWUMXUAws2590181 = -133907651;    int mJWUMXUAws97029431 = 79046071;    int mJWUMXUAws42519137 = -400749081;    int mJWUMXUAws82356390 = -126176930;    int mJWUMXUAws70656323 = -437834484;    int mJWUMXUAws29819456 = 34088702;    int mJWUMXUAws76218861 = -767066000;    int mJWUMXUAws32028800 = 11339445;    int mJWUMXUAws62145830 = -52792314;    int mJWUMXUAws55985675 = -760226753;    int mJWUMXUAws7147022 = -265312780;    int mJWUMXUAws76093803 = -96777067;    int mJWUMXUAws68585961 = -335503676;    int mJWUMXUAws22129188 = -81987360;    int mJWUMXUAws24414478 = -797575154;    int mJWUMXUAws77249242 = -708729149;    int mJWUMXUAws69027594 = -81399153;    int mJWUMXUAws69631150 = -826852184;    int mJWUMXUAws86971202 = -687884058;    int mJWUMXUAws5991332 = 15928844;    int mJWUMXUAws80625809 = -402875768;    int mJWUMXUAws77688931 = -255134365;    int mJWUMXUAws46981138 = -423435992;    int mJWUMXUAws19627590 = 9755486;    int mJWUMXUAws75577255 = -514749920;    int mJWUMXUAws84683774 = -261152686;    int mJWUMXUAws1164795 = -520823554;    int mJWUMXUAws90694034 = 59779789;    int mJWUMXUAws14078431 = -777041196;    int mJWUMXUAws40342347 = -990550397;    int mJWUMXUAws62402717 = -343766179;    int mJWUMXUAws92047855 = -336286893;    int mJWUMXUAws2575658 = -92763378;    int mJWUMXUAws49567574 = -775191270;    int mJWUMXUAws77617241 = -509534500;    int mJWUMXUAws15174564 = -818649315;     mJWUMXUAws67442886 = mJWUMXUAws23455099;     mJWUMXUAws23455099 = mJWUMXUAws49711327;     mJWUMXUAws49711327 = mJWUMXUAws25372820;     mJWUMXUAws25372820 = mJWUMXUAws83447236;     mJWUMXUAws83447236 = mJWUMXUAws725591;     mJWUMXUAws725591 = mJWUMXUAws82673013;     mJWUMXUAws82673013 = mJWUMXUAws80302646;     mJWUMXUAws80302646 = mJWUMXUAws95703795;     mJWUMXUAws95703795 = mJWUMXUAws62002370;     mJWUMXUAws62002370 = mJWUMXUAws79040263;     mJWUMXUAws79040263 = mJWUMXUAws26438547;     mJWUMXUAws26438547 = mJWUMXUAws86281263;     mJWUMXUAws86281263 = mJWUMXUAws41039542;     mJWUMXUAws41039542 = mJWUMXUAws52102741;     mJWUMXUAws52102741 = mJWUMXUAws80005749;     mJWUMXUAws80005749 = mJWUMXUAws35225245;     mJWUMXUAws35225245 = mJWUMXUAws18428812;     mJWUMXUAws18428812 = mJWUMXUAws49196469;     mJWUMXUAws49196469 = mJWUMXUAws17398792;     mJWUMXUAws17398792 = mJWUMXUAws81433362;     mJWUMXUAws81433362 = mJWUMXUAws81743442;     mJWUMXUAws81743442 = mJWUMXUAws2336610;     mJWUMXUAws2336610 = mJWUMXUAws36618474;     mJWUMXUAws36618474 = mJWUMXUAws36254806;     mJWUMXUAws36254806 = mJWUMXUAws67506601;     mJWUMXUAws67506601 = mJWUMXUAws69917513;     mJWUMXUAws69917513 = mJWUMXUAws64469099;     mJWUMXUAws64469099 = mJWUMXUAws77538745;     mJWUMXUAws77538745 = mJWUMXUAws32113888;     mJWUMXUAws32113888 = mJWUMXUAws59412190;     mJWUMXUAws59412190 = mJWUMXUAws82010832;     mJWUMXUAws82010832 = mJWUMXUAws83919410;     mJWUMXUAws83919410 = mJWUMXUAws3924874;     mJWUMXUAws3924874 = mJWUMXUAws70383218;     mJWUMXUAws70383218 = mJWUMXUAws22283285;     mJWUMXUAws22283285 = mJWUMXUAws3786888;     mJWUMXUAws3786888 = mJWUMXUAws3196445;     mJWUMXUAws3196445 = mJWUMXUAws56282981;     mJWUMXUAws56282981 = mJWUMXUAws93210794;     mJWUMXUAws93210794 = mJWUMXUAws10251771;     mJWUMXUAws10251771 = mJWUMXUAws5339559;     mJWUMXUAws5339559 = mJWUMXUAws13157482;     mJWUMXUAws13157482 = mJWUMXUAws80207421;     mJWUMXUAws80207421 = mJWUMXUAws12203996;     mJWUMXUAws12203996 = mJWUMXUAws59005563;     mJWUMXUAws59005563 = mJWUMXUAws98479007;     mJWUMXUAws98479007 = mJWUMXUAws286364;     mJWUMXUAws286364 = mJWUMXUAws77497896;     mJWUMXUAws77497896 = mJWUMXUAws71547414;     mJWUMXUAws71547414 = mJWUMXUAws51488079;     mJWUMXUAws51488079 = mJWUMXUAws81723259;     mJWUMXUAws81723259 = mJWUMXUAws35029695;     mJWUMXUAws35029695 = mJWUMXUAws64291820;     mJWUMXUAws64291820 = mJWUMXUAws28347618;     mJWUMXUAws28347618 = mJWUMXUAws85699444;     mJWUMXUAws85699444 = mJWUMXUAws21118490;     mJWUMXUAws21118490 = mJWUMXUAws13092853;     mJWUMXUAws13092853 = mJWUMXUAws89118014;     mJWUMXUAws89118014 = mJWUMXUAws15940635;     mJWUMXUAws15940635 = mJWUMXUAws30808077;     mJWUMXUAws30808077 = mJWUMXUAws18203915;     mJWUMXUAws18203915 = mJWUMXUAws2763901;     mJWUMXUAws2763901 = mJWUMXUAws63589907;     mJWUMXUAws63589907 = mJWUMXUAws2590181;     mJWUMXUAws2590181 = mJWUMXUAws97029431;     mJWUMXUAws97029431 = mJWUMXUAws42519137;     mJWUMXUAws42519137 = mJWUMXUAws82356390;     mJWUMXUAws82356390 = mJWUMXUAws70656323;     mJWUMXUAws70656323 = mJWUMXUAws29819456;     mJWUMXUAws29819456 = mJWUMXUAws76218861;     mJWUMXUAws76218861 = mJWUMXUAws32028800;     mJWUMXUAws32028800 = mJWUMXUAws62145830;     mJWUMXUAws62145830 = mJWUMXUAws55985675;     mJWUMXUAws55985675 = mJWUMXUAws7147022;     mJWUMXUAws7147022 = mJWUMXUAws76093803;     mJWUMXUAws76093803 = mJWUMXUAws68585961;     mJWUMXUAws68585961 = mJWUMXUAws22129188;     mJWUMXUAws22129188 = mJWUMXUAws24414478;     mJWUMXUAws24414478 = mJWUMXUAws77249242;     mJWUMXUAws77249242 = mJWUMXUAws69027594;     mJWUMXUAws69027594 = mJWUMXUAws69631150;     mJWUMXUAws69631150 = mJWUMXUAws86971202;     mJWUMXUAws86971202 = mJWUMXUAws5991332;     mJWUMXUAws5991332 = mJWUMXUAws80625809;     mJWUMXUAws80625809 = mJWUMXUAws77688931;     mJWUMXUAws77688931 = mJWUMXUAws46981138;     mJWUMXUAws46981138 = mJWUMXUAws19627590;     mJWUMXUAws19627590 = mJWUMXUAws75577255;     mJWUMXUAws75577255 = mJWUMXUAws84683774;     mJWUMXUAws84683774 = mJWUMXUAws1164795;     mJWUMXUAws1164795 = mJWUMXUAws90694034;     mJWUMXUAws90694034 = mJWUMXUAws14078431;     mJWUMXUAws14078431 = mJWUMXUAws40342347;     mJWUMXUAws40342347 = mJWUMXUAws62402717;     mJWUMXUAws62402717 = mJWUMXUAws92047855;     mJWUMXUAws92047855 = mJWUMXUAws2575658;     mJWUMXUAws2575658 = mJWUMXUAws49567574;     mJWUMXUAws49567574 = mJWUMXUAws77617241;     mJWUMXUAws77617241 = mJWUMXUAws15174564;     mJWUMXUAws15174564 = mJWUMXUAws67442886;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void JDJFILNnGN46425955() {     int bQTWPEXWnI89830004 = -453053278;    int bQTWPEXWnI7802600 = -771194267;    int bQTWPEXWnI78480605 = -913723396;    int bQTWPEXWnI40379508 = -481944684;    int bQTWPEXWnI81621490 = -542442929;    int bQTWPEXWnI15608708 = -646556309;    int bQTWPEXWnI74828752 = -606635583;    int bQTWPEXWnI55936505 = -601112432;    int bQTWPEXWnI92866926 = -255603252;    int bQTWPEXWnI53765955 = -167949377;    int bQTWPEXWnI16582123 = -808376081;    int bQTWPEXWnI82341496 = -161862969;    int bQTWPEXWnI53289444 = -643255196;    int bQTWPEXWnI83654293 = -297152265;    int bQTWPEXWnI88838017 = -111434267;    int bQTWPEXWnI24261662 = -708721344;    int bQTWPEXWnI85048596 = -933719993;    int bQTWPEXWnI24405228 = -387244430;    int bQTWPEXWnI90478100 = -222497048;    int bQTWPEXWnI95915074 = -978720715;    int bQTWPEXWnI82316469 = -440956076;    int bQTWPEXWnI45558661 = -152959290;    int bQTWPEXWnI5629933 = -1510102;    int bQTWPEXWnI63118261 = -618294818;    int bQTWPEXWnI71795052 = -306357854;    int bQTWPEXWnI55809596 = -503121123;    int bQTWPEXWnI17006934 = -560132432;    int bQTWPEXWnI70380549 = -548616142;    int bQTWPEXWnI84431306 = -314844584;    int bQTWPEXWnI41271068 = -754205048;    int bQTWPEXWnI3702996 = -272616545;    int bQTWPEXWnI39552023 = -342869106;    int bQTWPEXWnI64958080 = 60287910;    int bQTWPEXWnI63313560 = -122487798;    int bQTWPEXWnI46612583 = -395365447;    int bQTWPEXWnI63976154 = -733440633;    int bQTWPEXWnI97226599 = -292935754;    int bQTWPEXWnI56822546 = -435405091;    int bQTWPEXWnI4591113 = -183755457;    int bQTWPEXWnI13396509 = -463392200;    int bQTWPEXWnI39009141 = -863951148;    int bQTWPEXWnI93847949 = -10600392;    int bQTWPEXWnI18707674 = -200219885;    int bQTWPEXWnI79651541 = -740805799;    int bQTWPEXWnI80187262 = -65870285;    int bQTWPEXWnI67105845 = -58298677;    int bQTWPEXWnI10776406 = -456237999;    int bQTWPEXWnI44885039 = -732228746;    int bQTWPEXWnI71957801 = 78005670;    int bQTWPEXWnI68631159 = -28536568;    int bQTWPEXWnI60357295 = -686109691;    int bQTWPEXWnI55574516 = -266985311;    int bQTWPEXWnI13344098 = 60727010;    int bQTWPEXWnI24578366 = -695804891;    int bQTWPEXWnI64109485 = -424315144;    int bQTWPEXWnI44271343 = -200093988;    int bQTWPEXWnI2172668 = -669684166;    int bQTWPEXWnI15362345 = -195428578;    int bQTWPEXWnI68584455 = -75586830;    int bQTWPEXWnI25811894 = 60678193;    int bQTWPEXWnI98601773 = 13576123;    int bQTWPEXWnI4448204 = 41980558;    int bQTWPEXWnI71505198 = -186267848;    int bQTWPEXWnI51595859 = -501398204;    int bQTWPEXWnI50062960 = -895332832;    int bQTWPEXWnI77030100 = -365506975;    int bQTWPEXWnI17383416 = -122150880;    int bQTWPEXWnI89975884 = -420767399;    int bQTWPEXWnI37041711 = -901786819;    int bQTWPEXWnI24861864 = -377993634;    int bQTWPEXWnI27035062 = -315785591;    int bQTWPEXWnI28226051 = -398314903;    int bQTWPEXWnI19814115 = -103488973;    int bQTWPEXWnI77081591 = -759104849;    int bQTWPEXWnI56905934 = -14769567;    int bQTWPEXWnI88468519 = -330355684;    int bQTWPEXWnI26850987 = -952739406;    int bQTWPEXWnI25978392 = -260704304;    int bQTWPEXWnI82930998 = -452424534;    int bQTWPEXWnI4689207 = -148059178;    int bQTWPEXWnI45033191 = 53116876;    int bQTWPEXWnI72121895 = -827903686;    int bQTWPEXWnI98422747 = -526621813;    int bQTWPEXWnI15800147 = -186308017;    int bQTWPEXWnI80913772 = 31904642;    int bQTWPEXWnI48128479 = 94368765;    int bQTWPEXWnI26207925 = -303596117;    int bQTWPEXWnI40379715 = -243907200;    int bQTWPEXWnI99204075 = -698172654;    int bQTWPEXWnI2341240 = -95271459;    int bQTWPEXWnI61803487 = 36243532;    int bQTWPEXWnI81864255 = 2492824;    int bQTWPEXWnI88238090 = -259818261;    int bQTWPEXWnI78779218 = -144433651;    int bQTWPEXWnI14794736 = -376968323;    int bQTWPEXWnI34560938 = -805931707;    int bQTWPEXWnI22342751 = -824332544;    int bQTWPEXWnI67111815 = -698821681;    int bQTWPEXWnI29588582 = -845472968;    int bQTWPEXWnI3157162 = -453053278;     bQTWPEXWnI89830004 = bQTWPEXWnI7802600;     bQTWPEXWnI7802600 = bQTWPEXWnI78480605;     bQTWPEXWnI78480605 = bQTWPEXWnI40379508;     bQTWPEXWnI40379508 = bQTWPEXWnI81621490;     bQTWPEXWnI81621490 = bQTWPEXWnI15608708;     bQTWPEXWnI15608708 = bQTWPEXWnI74828752;     bQTWPEXWnI74828752 = bQTWPEXWnI55936505;     bQTWPEXWnI55936505 = bQTWPEXWnI92866926;     bQTWPEXWnI92866926 = bQTWPEXWnI53765955;     bQTWPEXWnI53765955 = bQTWPEXWnI16582123;     bQTWPEXWnI16582123 = bQTWPEXWnI82341496;     bQTWPEXWnI82341496 = bQTWPEXWnI53289444;     bQTWPEXWnI53289444 = bQTWPEXWnI83654293;     bQTWPEXWnI83654293 = bQTWPEXWnI88838017;     bQTWPEXWnI88838017 = bQTWPEXWnI24261662;     bQTWPEXWnI24261662 = bQTWPEXWnI85048596;     bQTWPEXWnI85048596 = bQTWPEXWnI24405228;     bQTWPEXWnI24405228 = bQTWPEXWnI90478100;     bQTWPEXWnI90478100 = bQTWPEXWnI95915074;     bQTWPEXWnI95915074 = bQTWPEXWnI82316469;     bQTWPEXWnI82316469 = bQTWPEXWnI45558661;     bQTWPEXWnI45558661 = bQTWPEXWnI5629933;     bQTWPEXWnI5629933 = bQTWPEXWnI63118261;     bQTWPEXWnI63118261 = bQTWPEXWnI71795052;     bQTWPEXWnI71795052 = bQTWPEXWnI55809596;     bQTWPEXWnI55809596 = bQTWPEXWnI17006934;     bQTWPEXWnI17006934 = bQTWPEXWnI70380549;     bQTWPEXWnI70380549 = bQTWPEXWnI84431306;     bQTWPEXWnI84431306 = bQTWPEXWnI41271068;     bQTWPEXWnI41271068 = bQTWPEXWnI3702996;     bQTWPEXWnI3702996 = bQTWPEXWnI39552023;     bQTWPEXWnI39552023 = bQTWPEXWnI64958080;     bQTWPEXWnI64958080 = bQTWPEXWnI63313560;     bQTWPEXWnI63313560 = bQTWPEXWnI46612583;     bQTWPEXWnI46612583 = bQTWPEXWnI63976154;     bQTWPEXWnI63976154 = bQTWPEXWnI97226599;     bQTWPEXWnI97226599 = bQTWPEXWnI56822546;     bQTWPEXWnI56822546 = bQTWPEXWnI4591113;     bQTWPEXWnI4591113 = bQTWPEXWnI13396509;     bQTWPEXWnI13396509 = bQTWPEXWnI39009141;     bQTWPEXWnI39009141 = bQTWPEXWnI93847949;     bQTWPEXWnI93847949 = bQTWPEXWnI18707674;     bQTWPEXWnI18707674 = bQTWPEXWnI79651541;     bQTWPEXWnI79651541 = bQTWPEXWnI80187262;     bQTWPEXWnI80187262 = bQTWPEXWnI67105845;     bQTWPEXWnI67105845 = bQTWPEXWnI10776406;     bQTWPEXWnI10776406 = bQTWPEXWnI44885039;     bQTWPEXWnI44885039 = bQTWPEXWnI71957801;     bQTWPEXWnI71957801 = bQTWPEXWnI68631159;     bQTWPEXWnI68631159 = bQTWPEXWnI60357295;     bQTWPEXWnI60357295 = bQTWPEXWnI55574516;     bQTWPEXWnI55574516 = bQTWPEXWnI13344098;     bQTWPEXWnI13344098 = bQTWPEXWnI24578366;     bQTWPEXWnI24578366 = bQTWPEXWnI64109485;     bQTWPEXWnI64109485 = bQTWPEXWnI44271343;     bQTWPEXWnI44271343 = bQTWPEXWnI2172668;     bQTWPEXWnI2172668 = bQTWPEXWnI15362345;     bQTWPEXWnI15362345 = bQTWPEXWnI68584455;     bQTWPEXWnI68584455 = bQTWPEXWnI25811894;     bQTWPEXWnI25811894 = bQTWPEXWnI98601773;     bQTWPEXWnI98601773 = bQTWPEXWnI4448204;     bQTWPEXWnI4448204 = bQTWPEXWnI71505198;     bQTWPEXWnI71505198 = bQTWPEXWnI51595859;     bQTWPEXWnI51595859 = bQTWPEXWnI50062960;     bQTWPEXWnI50062960 = bQTWPEXWnI77030100;     bQTWPEXWnI77030100 = bQTWPEXWnI17383416;     bQTWPEXWnI17383416 = bQTWPEXWnI89975884;     bQTWPEXWnI89975884 = bQTWPEXWnI37041711;     bQTWPEXWnI37041711 = bQTWPEXWnI24861864;     bQTWPEXWnI24861864 = bQTWPEXWnI27035062;     bQTWPEXWnI27035062 = bQTWPEXWnI28226051;     bQTWPEXWnI28226051 = bQTWPEXWnI19814115;     bQTWPEXWnI19814115 = bQTWPEXWnI77081591;     bQTWPEXWnI77081591 = bQTWPEXWnI56905934;     bQTWPEXWnI56905934 = bQTWPEXWnI88468519;     bQTWPEXWnI88468519 = bQTWPEXWnI26850987;     bQTWPEXWnI26850987 = bQTWPEXWnI25978392;     bQTWPEXWnI25978392 = bQTWPEXWnI82930998;     bQTWPEXWnI82930998 = bQTWPEXWnI4689207;     bQTWPEXWnI4689207 = bQTWPEXWnI45033191;     bQTWPEXWnI45033191 = bQTWPEXWnI72121895;     bQTWPEXWnI72121895 = bQTWPEXWnI98422747;     bQTWPEXWnI98422747 = bQTWPEXWnI15800147;     bQTWPEXWnI15800147 = bQTWPEXWnI80913772;     bQTWPEXWnI80913772 = bQTWPEXWnI48128479;     bQTWPEXWnI48128479 = bQTWPEXWnI26207925;     bQTWPEXWnI26207925 = bQTWPEXWnI40379715;     bQTWPEXWnI40379715 = bQTWPEXWnI99204075;     bQTWPEXWnI99204075 = bQTWPEXWnI2341240;     bQTWPEXWnI2341240 = bQTWPEXWnI61803487;     bQTWPEXWnI61803487 = bQTWPEXWnI81864255;     bQTWPEXWnI81864255 = bQTWPEXWnI88238090;     bQTWPEXWnI88238090 = bQTWPEXWnI78779218;     bQTWPEXWnI78779218 = bQTWPEXWnI14794736;     bQTWPEXWnI14794736 = bQTWPEXWnI34560938;     bQTWPEXWnI34560938 = bQTWPEXWnI22342751;     bQTWPEXWnI22342751 = bQTWPEXWnI67111815;     bQTWPEXWnI67111815 = bQTWPEXWnI29588582;     bQTWPEXWnI29588582 = bQTWPEXWnI3157162;     bQTWPEXWnI3157162 = bQTWPEXWnI89830004;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void OwXAUBjQaK83070929() {     int yoxNnmcZnR12217122 = -87457241;    int yoxNnmcZnR92150099 = -719737322;    int yoxNnmcZnR7249884 = -53377240;    int yoxNnmcZnR55386196 = -488352216;    int yoxNnmcZnR79795744 = -33499931;    int yoxNnmcZnR30491825 = -507164686;    int yoxNnmcZnR66984491 = -428622889;    int yoxNnmcZnR31570364 = 28290924;    int yoxNnmcZnR90030057 = -139799291;    int yoxNnmcZnR45529540 = 89309290;    int yoxNnmcZnR54123983 = -805000481;    int yoxNnmcZnR38244445 = -412608262;    int yoxNnmcZnR20297625 = -161972516;    int yoxNnmcZnR26269046 = -824861022;    int yoxNnmcZnR25573295 = -838911980;    int yoxNnmcZnR68517574 = 9611113;    int yoxNnmcZnR34871948 = -110999813;    int yoxNnmcZnR30381644 = -838987120;    int yoxNnmcZnR31759731 = -357599173;    int yoxNnmcZnR74431358 = -131355168;    int yoxNnmcZnR83199576 = -432443384;    int yoxNnmcZnR9373879 = -657725603;    int yoxNnmcZnR8923256 = -177590694;    int yoxNnmcZnR89618048 = -82287675;    int yoxNnmcZnR7335299 = 72083021;    int yoxNnmcZnR44112591 = -947015347;    int yoxNnmcZnR64096354 = -317718924;    int yoxNnmcZnR76291999 = -337070596;    int yoxNnmcZnR91323867 = -659101703;    int yoxNnmcZnR50428247 = -674500915;    int yoxNnmcZnR47993801 = -353932697;    int yoxNnmcZnR97093213 = -994940461;    int yoxNnmcZnR45996751 = -469055585;    int yoxNnmcZnR22702248 = -446614648;    int yoxNnmcZnR22841948 = -459121869;    int yoxNnmcZnR5669023 = 51163991;    int yoxNnmcZnR90666311 = -25883705;    int yoxNnmcZnR10448647 = -303030563;    int yoxNnmcZnR52899244 = -584801489;    int yoxNnmcZnR33582224 = -599616229;    int yoxNnmcZnR67766512 = -267128814;    int yoxNnmcZnR82356341 = -868509083;    int yoxNnmcZnR24257866 = -87750468;    int yoxNnmcZnR79095660 = -738169448;    int yoxNnmcZnR48170529 = -975013762;    int yoxNnmcZnR75206127 = -240527772;    int yoxNnmcZnR23073803 = 65351748;    int yoxNnmcZnR89483713 = -488763737;    int yoxNnmcZnR66417706 = -971711028;    int yoxNnmcZnR65714905 = -170556827;    int yoxNnmcZnR69226512 = 58814032;    int yoxNnmcZnR29425774 = -697804593;    int yoxNnmcZnR91658500 = -711184220;    int yoxNnmcZnR84864910 = -971485700;    int yoxNnmcZnR99871352 = -565019259;    int yoxNnmcZnR2843243 = -429731638;    int yoxNnmcZnR83226844 = -442146628;    int yoxNnmcZnR17631836 = -971089565;    int yoxNnmcZnR48050897 = -460435238;    int yoxNnmcZnR35683154 = -86484584;    int yoxNnmcZnR66395470 = -89445762;    int yoxNnmcZnR90692491 = 8447706;    int yoxNnmcZnR40246497 = -312607373;    int yoxNnmcZnR39601810 = -465298377;    int yoxNnmcZnR97535739 = -556758013;    int yoxNnmcZnR57030769 = -810060021;    int yoxNnmcZnR92247694 = -943552678;    int yoxNnmcZnR97595377 = -715357869;    int yoxNnmcZnR3427099 = -265739153;    int yoxNnmcZnR19904272 = -790075971;    int yoxNnmcZnR77851262 = -964505182;    int yoxNnmcZnR24423302 = -807969251;    int yoxNnmcZnR77482400 = -154185632;    int yoxNnmcZnR98177507 = -757982945;    int yoxNnmcZnR6664846 = -864226355;    int yoxNnmcZnR843236 = -563934302;    int yoxNnmcZnR85116013 = -469975135;    int yoxNnmcZnR29827596 = -439421247;    int yoxNnmcZnR41447519 = -107273913;    int yoxNnmcZnR32129172 = -687389207;    int yoxNnmcZnR21038788 = -912367095;    int yoxNnmcZnR74612641 = -828955188;    int yoxNnmcZnR9874293 = -365359568;    int yoxNnmcZnR25608962 = -388544877;    int yoxNnmcZnR81201735 = -633314948;    int yoxNnmcZnR18568027 = -656128105;    int yoxNnmcZnR5434713 = -183756241;    int yoxNnmcZnR61131840 = -497569886;    int yoxNnmcZnR22830895 = -881595389;    int yoxNnmcZnR19998706 = 70609769;    int yoxNnmcZnR22442179 = -506689381;    int yoxNnmcZnR73034475 = -54794141;    int yoxNnmcZnR62397749 = -842595326;    int yoxNnmcZnR17216091 = -398316905;    int yoxNnmcZnR67186753 = -410170467;    int yoxNnmcZnR77074020 = -175576520;    int yoxNnmcZnR42109844 = -455901710;    int yoxNnmcZnR84656055 = -622452092;    int yoxNnmcZnR81559921 = -81411436;    int yoxNnmcZnR91139759 = -87457241;     yoxNnmcZnR12217122 = yoxNnmcZnR92150099;     yoxNnmcZnR92150099 = yoxNnmcZnR7249884;     yoxNnmcZnR7249884 = yoxNnmcZnR55386196;     yoxNnmcZnR55386196 = yoxNnmcZnR79795744;     yoxNnmcZnR79795744 = yoxNnmcZnR30491825;     yoxNnmcZnR30491825 = yoxNnmcZnR66984491;     yoxNnmcZnR66984491 = yoxNnmcZnR31570364;     yoxNnmcZnR31570364 = yoxNnmcZnR90030057;     yoxNnmcZnR90030057 = yoxNnmcZnR45529540;     yoxNnmcZnR45529540 = yoxNnmcZnR54123983;     yoxNnmcZnR54123983 = yoxNnmcZnR38244445;     yoxNnmcZnR38244445 = yoxNnmcZnR20297625;     yoxNnmcZnR20297625 = yoxNnmcZnR26269046;     yoxNnmcZnR26269046 = yoxNnmcZnR25573295;     yoxNnmcZnR25573295 = yoxNnmcZnR68517574;     yoxNnmcZnR68517574 = yoxNnmcZnR34871948;     yoxNnmcZnR34871948 = yoxNnmcZnR30381644;     yoxNnmcZnR30381644 = yoxNnmcZnR31759731;     yoxNnmcZnR31759731 = yoxNnmcZnR74431358;     yoxNnmcZnR74431358 = yoxNnmcZnR83199576;     yoxNnmcZnR83199576 = yoxNnmcZnR9373879;     yoxNnmcZnR9373879 = yoxNnmcZnR8923256;     yoxNnmcZnR8923256 = yoxNnmcZnR89618048;     yoxNnmcZnR89618048 = yoxNnmcZnR7335299;     yoxNnmcZnR7335299 = yoxNnmcZnR44112591;     yoxNnmcZnR44112591 = yoxNnmcZnR64096354;     yoxNnmcZnR64096354 = yoxNnmcZnR76291999;     yoxNnmcZnR76291999 = yoxNnmcZnR91323867;     yoxNnmcZnR91323867 = yoxNnmcZnR50428247;     yoxNnmcZnR50428247 = yoxNnmcZnR47993801;     yoxNnmcZnR47993801 = yoxNnmcZnR97093213;     yoxNnmcZnR97093213 = yoxNnmcZnR45996751;     yoxNnmcZnR45996751 = yoxNnmcZnR22702248;     yoxNnmcZnR22702248 = yoxNnmcZnR22841948;     yoxNnmcZnR22841948 = yoxNnmcZnR5669023;     yoxNnmcZnR5669023 = yoxNnmcZnR90666311;     yoxNnmcZnR90666311 = yoxNnmcZnR10448647;     yoxNnmcZnR10448647 = yoxNnmcZnR52899244;     yoxNnmcZnR52899244 = yoxNnmcZnR33582224;     yoxNnmcZnR33582224 = yoxNnmcZnR67766512;     yoxNnmcZnR67766512 = yoxNnmcZnR82356341;     yoxNnmcZnR82356341 = yoxNnmcZnR24257866;     yoxNnmcZnR24257866 = yoxNnmcZnR79095660;     yoxNnmcZnR79095660 = yoxNnmcZnR48170529;     yoxNnmcZnR48170529 = yoxNnmcZnR75206127;     yoxNnmcZnR75206127 = yoxNnmcZnR23073803;     yoxNnmcZnR23073803 = yoxNnmcZnR89483713;     yoxNnmcZnR89483713 = yoxNnmcZnR66417706;     yoxNnmcZnR66417706 = yoxNnmcZnR65714905;     yoxNnmcZnR65714905 = yoxNnmcZnR69226512;     yoxNnmcZnR69226512 = yoxNnmcZnR29425774;     yoxNnmcZnR29425774 = yoxNnmcZnR91658500;     yoxNnmcZnR91658500 = yoxNnmcZnR84864910;     yoxNnmcZnR84864910 = yoxNnmcZnR99871352;     yoxNnmcZnR99871352 = yoxNnmcZnR2843243;     yoxNnmcZnR2843243 = yoxNnmcZnR83226844;     yoxNnmcZnR83226844 = yoxNnmcZnR17631836;     yoxNnmcZnR17631836 = yoxNnmcZnR48050897;     yoxNnmcZnR48050897 = yoxNnmcZnR35683154;     yoxNnmcZnR35683154 = yoxNnmcZnR66395470;     yoxNnmcZnR66395470 = yoxNnmcZnR90692491;     yoxNnmcZnR90692491 = yoxNnmcZnR40246497;     yoxNnmcZnR40246497 = yoxNnmcZnR39601810;     yoxNnmcZnR39601810 = yoxNnmcZnR97535739;     yoxNnmcZnR97535739 = yoxNnmcZnR57030769;     yoxNnmcZnR57030769 = yoxNnmcZnR92247694;     yoxNnmcZnR92247694 = yoxNnmcZnR97595377;     yoxNnmcZnR97595377 = yoxNnmcZnR3427099;     yoxNnmcZnR3427099 = yoxNnmcZnR19904272;     yoxNnmcZnR19904272 = yoxNnmcZnR77851262;     yoxNnmcZnR77851262 = yoxNnmcZnR24423302;     yoxNnmcZnR24423302 = yoxNnmcZnR77482400;     yoxNnmcZnR77482400 = yoxNnmcZnR98177507;     yoxNnmcZnR98177507 = yoxNnmcZnR6664846;     yoxNnmcZnR6664846 = yoxNnmcZnR843236;     yoxNnmcZnR843236 = yoxNnmcZnR85116013;     yoxNnmcZnR85116013 = yoxNnmcZnR29827596;     yoxNnmcZnR29827596 = yoxNnmcZnR41447519;     yoxNnmcZnR41447519 = yoxNnmcZnR32129172;     yoxNnmcZnR32129172 = yoxNnmcZnR21038788;     yoxNnmcZnR21038788 = yoxNnmcZnR74612641;     yoxNnmcZnR74612641 = yoxNnmcZnR9874293;     yoxNnmcZnR9874293 = yoxNnmcZnR25608962;     yoxNnmcZnR25608962 = yoxNnmcZnR81201735;     yoxNnmcZnR81201735 = yoxNnmcZnR18568027;     yoxNnmcZnR18568027 = yoxNnmcZnR5434713;     yoxNnmcZnR5434713 = yoxNnmcZnR61131840;     yoxNnmcZnR61131840 = yoxNnmcZnR22830895;     yoxNnmcZnR22830895 = yoxNnmcZnR19998706;     yoxNnmcZnR19998706 = yoxNnmcZnR22442179;     yoxNnmcZnR22442179 = yoxNnmcZnR73034475;     yoxNnmcZnR73034475 = yoxNnmcZnR62397749;     yoxNnmcZnR62397749 = yoxNnmcZnR17216091;     yoxNnmcZnR17216091 = yoxNnmcZnR67186753;     yoxNnmcZnR67186753 = yoxNnmcZnR77074020;     yoxNnmcZnR77074020 = yoxNnmcZnR42109844;     yoxNnmcZnR42109844 = yoxNnmcZnR84656055;     yoxNnmcZnR84656055 = yoxNnmcZnR81559921;     yoxNnmcZnR81559921 = yoxNnmcZnR91139759;     yoxNnmcZnR91139759 = yoxNnmcZnR12217122;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ILDgoViufU19715904() {     int hkEYAalRSi34604239 = -821861204;    int hkEYAalRSi76497600 = -668280377;    int hkEYAalRSi36019163 = -293031084;    int hkEYAalRSi70392885 = -494759747;    int hkEYAalRSi77969998 = -624556932;    int hkEYAalRSi45374941 = -367773064;    int hkEYAalRSi59140230 = -250610195;    int hkEYAalRSi7204223 = -442305720;    int hkEYAalRSi87193188 = -23995330;    int hkEYAalRSi37293125 = -753432043;    int hkEYAalRSi91665842 = -801624882;    int hkEYAalRSi94147394 = -663353556;    int hkEYAalRSi87305805 = -780689836;    int hkEYAalRSi68883798 = -252569779;    int hkEYAalRSi62308572 = -466389693;    int hkEYAalRSi12773487 = -372056429;    int hkEYAalRSi84695300 = -388279633;    int hkEYAalRSi36358060 = -190729811;    int hkEYAalRSi73041362 = -492701298;    int hkEYAalRSi52947641 = -383989622;    int hkEYAalRSi84082683 = -423930692;    int hkEYAalRSi73189097 = -62491916;    int hkEYAalRSi12216579 = -353671286;    int hkEYAalRSi16117835 = -646280532;    int hkEYAalRSi42875546 = -649476103;    int hkEYAalRSi32415585 = -290909571;    int hkEYAalRSi11185775 = -75305417;    int hkEYAalRSi82203449 = -125525049;    int hkEYAalRSi98216427 = 96641178;    int hkEYAalRSi59585426 = -594796781;    int hkEYAalRSi92284605 = -435248849;    int hkEYAalRSi54634405 = -547011816;    int hkEYAalRSi27035421 = -998399080;    int hkEYAalRSi82090935 = -770741498;    int hkEYAalRSi99071312 = -522878292;    int hkEYAalRSi47361892 = -264231386;    int hkEYAalRSi84106023 = -858831657;    int hkEYAalRSi64074747 = -170656035;    int hkEYAalRSi1207376 = -985847521;    int hkEYAalRSi53767938 = -735840258;    int hkEYAalRSi96523882 = -770306480;    int hkEYAalRSi70864732 = -626417774;    int hkEYAalRSi29808058 = 24718948;    int hkEYAalRSi78539779 = -735533097;    int hkEYAalRSi16153796 = -784157240;    int hkEYAalRSi83306409 = -422756868;    int hkEYAalRSi35371200 = -513058505;    int hkEYAalRSi34082388 = -245298727;    int hkEYAalRSi60877612 = -921427726;    int hkEYAalRSi62798651 = -312577086;    int hkEYAalRSi78095728 = -296262245;    int hkEYAalRSi3277031 = -28623875;    int hkEYAalRSi69972904 = -383095450;    int hkEYAalRSi45151456 = -147166508;    int hkEYAalRSi35633220 = -705723374;    int hkEYAalRSi61415141 = -659369288;    int hkEYAalRSi64281021 = -214609091;    int hkEYAalRSi19901328 = -646750552;    int hkEYAalRSi27517339 = -845283645;    int hkEYAalRSi45554413 = -233647361;    int hkEYAalRSi34189167 = -192467648;    int hkEYAalRSi76936780 = -25085147;    int hkEYAalRSi8987795 = -438946898;    int hkEYAalRSi27607762 = -429198549;    int hkEYAalRSi45008519 = -218183194;    int hkEYAalRSi37031438 = -154613067;    int hkEYAalRSi67111973 = -664954476;    int hkEYAalRSi5214871 = 90051662;    int hkEYAalRSi69812486 = -729691488;    int hkEYAalRSi14946680 = -102158308;    int hkEYAalRSi28667463 = -513224773;    int hkEYAalRSi20620553 = -117623599;    int hkEYAalRSi35150685 = -204882291;    int hkEYAalRSi19273424 = -756861040;    int hkEYAalRSi56423758 = -613683142;    int hkEYAalRSi13217952 = -797512919;    int hkEYAalRSi43381040 = 12789135;    int hkEYAalRSi33676799 = -618138190;    int hkEYAalRSi99964039 = -862123293;    int hkEYAalRSi59569137 = -126719236;    int hkEYAalRSi97044385 = -777851066;    int hkEYAalRSi77103387 = -830006690;    int hkEYAalRSi21325838 = -204097323;    int hkEYAalRSi35417777 = -590781737;    int hkEYAalRSi81489698 = -198534537;    int hkEYAalRSi89007575 = -306624975;    int hkEYAalRSi84661500 = -63916366;    int hkEYAalRSi81883965 = -751232572;    int hkEYAalRSi46457715 = 34981876;    int hkEYAalRSi37656171 = -863509004;    int hkEYAalRSi83080870 = 50377705;    int hkEYAalRSi64204696 = -112081105;    int hkEYAalRSi36557409 = -325372390;    int hkEYAalRSi55652962 = -652200160;    int hkEYAalRSi19578772 = -443372611;    int hkEYAalRSi19587102 = -645221334;    int hkEYAalRSi61876938 = -87470876;    int hkEYAalRSi2200296 = -546082503;    int hkEYAalRSi33531261 = -417349904;    int hkEYAalRSi79122357 = -821861204;     hkEYAalRSi34604239 = hkEYAalRSi76497600;     hkEYAalRSi76497600 = hkEYAalRSi36019163;     hkEYAalRSi36019163 = hkEYAalRSi70392885;     hkEYAalRSi70392885 = hkEYAalRSi77969998;     hkEYAalRSi77969998 = hkEYAalRSi45374941;     hkEYAalRSi45374941 = hkEYAalRSi59140230;     hkEYAalRSi59140230 = hkEYAalRSi7204223;     hkEYAalRSi7204223 = hkEYAalRSi87193188;     hkEYAalRSi87193188 = hkEYAalRSi37293125;     hkEYAalRSi37293125 = hkEYAalRSi91665842;     hkEYAalRSi91665842 = hkEYAalRSi94147394;     hkEYAalRSi94147394 = hkEYAalRSi87305805;     hkEYAalRSi87305805 = hkEYAalRSi68883798;     hkEYAalRSi68883798 = hkEYAalRSi62308572;     hkEYAalRSi62308572 = hkEYAalRSi12773487;     hkEYAalRSi12773487 = hkEYAalRSi84695300;     hkEYAalRSi84695300 = hkEYAalRSi36358060;     hkEYAalRSi36358060 = hkEYAalRSi73041362;     hkEYAalRSi73041362 = hkEYAalRSi52947641;     hkEYAalRSi52947641 = hkEYAalRSi84082683;     hkEYAalRSi84082683 = hkEYAalRSi73189097;     hkEYAalRSi73189097 = hkEYAalRSi12216579;     hkEYAalRSi12216579 = hkEYAalRSi16117835;     hkEYAalRSi16117835 = hkEYAalRSi42875546;     hkEYAalRSi42875546 = hkEYAalRSi32415585;     hkEYAalRSi32415585 = hkEYAalRSi11185775;     hkEYAalRSi11185775 = hkEYAalRSi82203449;     hkEYAalRSi82203449 = hkEYAalRSi98216427;     hkEYAalRSi98216427 = hkEYAalRSi59585426;     hkEYAalRSi59585426 = hkEYAalRSi92284605;     hkEYAalRSi92284605 = hkEYAalRSi54634405;     hkEYAalRSi54634405 = hkEYAalRSi27035421;     hkEYAalRSi27035421 = hkEYAalRSi82090935;     hkEYAalRSi82090935 = hkEYAalRSi99071312;     hkEYAalRSi99071312 = hkEYAalRSi47361892;     hkEYAalRSi47361892 = hkEYAalRSi84106023;     hkEYAalRSi84106023 = hkEYAalRSi64074747;     hkEYAalRSi64074747 = hkEYAalRSi1207376;     hkEYAalRSi1207376 = hkEYAalRSi53767938;     hkEYAalRSi53767938 = hkEYAalRSi96523882;     hkEYAalRSi96523882 = hkEYAalRSi70864732;     hkEYAalRSi70864732 = hkEYAalRSi29808058;     hkEYAalRSi29808058 = hkEYAalRSi78539779;     hkEYAalRSi78539779 = hkEYAalRSi16153796;     hkEYAalRSi16153796 = hkEYAalRSi83306409;     hkEYAalRSi83306409 = hkEYAalRSi35371200;     hkEYAalRSi35371200 = hkEYAalRSi34082388;     hkEYAalRSi34082388 = hkEYAalRSi60877612;     hkEYAalRSi60877612 = hkEYAalRSi62798651;     hkEYAalRSi62798651 = hkEYAalRSi78095728;     hkEYAalRSi78095728 = hkEYAalRSi3277031;     hkEYAalRSi3277031 = hkEYAalRSi69972904;     hkEYAalRSi69972904 = hkEYAalRSi45151456;     hkEYAalRSi45151456 = hkEYAalRSi35633220;     hkEYAalRSi35633220 = hkEYAalRSi61415141;     hkEYAalRSi61415141 = hkEYAalRSi64281021;     hkEYAalRSi64281021 = hkEYAalRSi19901328;     hkEYAalRSi19901328 = hkEYAalRSi27517339;     hkEYAalRSi27517339 = hkEYAalRSi45554413;     hkEYAalRSi45554413 = hkEYAalRSi34189167;     hkEYAalRSi34189167 = hkEYAalRSi76936780;     hkEYAalRSi76936780 = hkEYAalRSi8987795;     hkEYAalRSi8987795 = hkEYAalRSi27607762;     hkEYAalRSi27607762 = hkEYAalRSi45008519;     hkEYAalRSi45008519 = hkEYAalRSi37031438;     hkEYAalRSi37031438 = hkEYAalRSi67111973;     hkEYAalRSi67111973 = hkEYAalRSi5214871;     hkEYAalRSi5214871 = hkEYAalRSi69812486;     hkEYAalRSi69812486 = hkEYAalRSi14946680;     hkEYAalRSi14946680 = hkEYAalRSi28667463;     hkEYAalRSi28667463 = hkEYAalRSi20620553;     hkEYAalRSi20620553 = hkEYAalRSi35150685;     hkEYAalRSi35150685 = hkEYAalRSi19273424;     hkEYAalRSi19273424 = hkEYAalRSi56423758;     hkEYAalRSi56423758 = hkEYAalRSi13217952;     hkEYAalRSi13217952 = hkEYAalRSi43381040;     hkEYAalRSi43381040 = hkEYAalRSi33676799;     hkEYAalRSi33676799 = hkEYAalRSi99964039;     hkEYAalRSi99964039 = hkEYAalRSi59569137;     hkEYAalRSi59569137 = hkEYAalRSi97044385;     hkEYAalRSi97044385 = hkEYAalRSi77103387;     hkEYAalRSi77103387 = hkEYAalRSi21325838;     hkEYAalRSi21325838 = hkEYAalRSi35417777;     hkEYAalRSi35417777 = hkEYAalRSi81489698;     hkEYAalRSi81489698 = hkEYAalRSi89007575;     hkEYAalRSi89007575 = hkEYAalRSi84661500;     hkEYAalRSi84661500 = hkEYAalRSi81883965;     hkEYAalRSi81883965 = hkEYAalRSi46457715;     hkEYAalRSi46457715 = hkEYAalRSi37656171;     hkEYAalRSi37656171 = hkEYAalRSi83080870;     hkEYAalRSi83080870 = hkEYAalRSi64204696;     hkEYAalRSi64204696 = hkEYAalRSi36557409;     hkEYAalRSi36557409 = hkEYAalRSi55652962;     hkEYAalRSi55652962 = hkEYAalRSi19578772;     hkEYAalRSi19578772 = hkEYAalRSi19587102;     hkEYAalRSi19587102 = hkEYAalRSi61876938;     hkEYAalRSi61876938 = hkEYAalRSi2200296;     hkEYAalRSi2200296 = hkEYAalRSi33531261;     hkEYAalRSi33531261 = hkEYAalRSi79122357;     hkEYAalRSi79122357 = hkEYAalRSi34604239;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ZOyKapGpbI99222494() {     int FhrtAzGKoA44192246 = -66272758;    int FhrtAzGKoA51218845 = -531679668;    int FhrtAzGKoA49662066 = -38582774;    int FhrtAzGKoA47618469 = -318046662;    int FhrtAzGKoA313353 = -560508111;    int FhrtAzGKoA86558852 = -950323752;    int FhrtAzGKoA67182883 = -35998365;    int FhrtAzGKoA94547069 = -976738742;    int FhrtAzGKoA54431837 = -473725417;    int FhrtAzGKoA16117032 = 34300515;    int FhrtAzGKoA42361531 = -602507412;    int FhrtAzGKoA46683192 = -977722758;    int FhrtAzGKoA47876511 = -438941447;    int FhrtAzGKoA69562912 = -38169440;    int FhrtAzGKoA66081514 = -112307279;    int FhrtAzGKoA77733425 = -868423884;    int FhrtAzGKoA90472353 = -641014125;    int FhrtAzGKoA4777058 = -781405443;    int FhrtAzGKoA54018902 = -854298762;    int FhrtAzGKoA72956954 = -161428839;    int FhrtAzGKoA64130582 = -445034484;    int FhrtAzGKoA64549028 = -317383648;    int FhrtAzGKoA50506400 = -416663668;    int FhrtAzGKoA85410709 = -611769422;    int FhrtAzGKoA29406913 = -913981010;    int FhrtAzGKoA72548088 = -860784452;    int FhrtAzGKoA87302822 = -900785056;    int FhrtAzGKoA91569024 = -958115375;    int FhrtAzGKoA13848830 = -225539331;    int FhrtAzGKoA68171230 = -236069241;    int FhrtAzGKoA58964772 = 85665279;    int FhrtAzGKoA78304487 = -904007015;    int FhrtAzGKoA32932989 = -955776222;    int FhrtAzGKoA64968924 = -943477595;    int FhrtAzGKoA56956221 = -495230683;    int FhrtAzGKoA79487025 = -950595150;    int FhrtAzGKoA64664411 = 98822986;    int FhrtAzGKoA99832184 = -109186773;    int FhrtAzGKoA14367314 = -770405808;    int FhrtAzGKoA91434708 = -246510814;    int FhrtAzGKoA39561281 = 26102050;    int FhrtAzGKoA23065990 = -873599295;    int FhrtAzGKoA21041500 = -957638699;    int FhrtAzGKoA51819157 = -605653390;    int FhrtAzGKoA72164676 = -547049561;    int FhrtAzGKoA98634498 = -155756865;    int FhrtAzGKoA25482163 = -456514950;    int FhrtAzGKoA8258 = 35996349;    int FhrtAzGKoA48590246 = -935070173;    int FhrtAzGKoA53022822 = -117610655;    int FhrtAzGKoA95156191 = -655564228;    int FhrtAzGKoA78046255 = -752549315;    int FhrtAzGKoA64698854 = -763111414;    int FhrtAzGKoA81238817 = -797235278;    int FhrtAzGKoA35091913 = -365781558;    int FhrtAzGKoA79643218 = -748889110;    int FhrtAzGKoA712446 = -15016001;    int FhrtAzGKoA64251356 = -426813352;    int FhrtAzGKoA18211557 = -404065653;    int FhrtAzGKoA27765264 = -699723659;    int FhrtAzGKoA99256030 = 50461303;    int FhrtAzGKoA75613858 = -77882991;    int FhrtAzGKoA80698240 = -651199411;    int FhrtAzGKoA86260606 = -137656177;    int FhrtAzGKoA57152259 = 48635235;    int FhrtAzGKoA64057044 = -698500398;    int FhrtAzGKoA13750204 = 78053463;    int FhrtAzGKoA82907587 = -495463853;    int FhrtAzGKoA12606692 = -542938758;    int FhrtAzGKoA86594488 = -161712129;    int FhrtAzGKoA13069014 = -867246870;    int FhrtAzGKoA90640168 = -431827353;    int FhrtAzGKoA90409743 = 89000364;    int FhrtAzGKoA62584194 = -507787948;    int FhrtAzGKoA33395674 = -87530890;    int FhrtAzGKoA41064593 = -571435190;    int FhrtAzGKoA43507528 = -359744949;    int FhrtAzGKoA98687242 = -811010278;    int FhrtAzGKoA13246033 = 35280138;    int FhrtAzGKoA30772415 = -658224145;    int FhrtAzGKoA47065926 = -304269503;    int FhrtAzGKoA87294564 = -836781405;    int FhrtAzGKoA42978779 = 76954797;    int FhrtAzGKoA60826007 = -7928677;    int FhrtAzGKoA73015039 = -580505013;    int FhrtAzGKoA80918517 = -161785406;    int FhrtAzGKoA13605633 = -40895601;    int FhrtAzGKoA51694171 = -58540945;    int FhrtAzGKoA29877012 = -477696037;    int FhrtAzGKoA77313002 = -746341573;    int FhrtAzGKoA78774580 = -835579150;    int FhrtAzGKoA413055 = -474363663;    int FhrtAzGKoA81620628 = -705121120;    int FhrtAzGKoA86602050 = 29317851;    int FhrtAzGKoA92178678 = -196972118;    int FhrtAzGKoA63947422 = -896014959;    int FhrtAzGKoA42367749 = -122399884;    int FhrtAzGKoA34780893 = -719982523;    int FhrtAzGKoA94666898 = -554288626;    int FhrtAzGKoA8107633 = -66272758;     FhrtAzGKoA44192246 = FhrtAzGKoA51218845;     FhrtAzGKoA51218845 = FhrtAzGKoA49662066;     FhrtAzGKoA49662066 = FhrtAzGKoA47618469;     FhrtAzGKoA47618469 = FhrtAzGKoA313353;     FhrtAzGKoA313353 = FhrtAzGKoA86558852;     FhrtAzGKoA86558852 = FhrtAzGKoA67182883;     FhrtAzGKoA67182883 = FhrtAzGKoA94547069;     FhrtAzGKoA94547069 = FhrtAzGKoA54431837;     FhrtAzGKoA54431837 = FhrtAzGKoA16117032;     FhrtAzGKoA16117032 = FhrtAzGKoA42361531;     FhrtAzGKoA42361531 = FhrtAzGKoA46683192;     FhrtAzGKoA46683192 = FhrtAzGKoA47876511;     FhrtAzGKoA47876511 = FhrtAzGKoA69562912;     FhrtAzGKoA69562912 = FhrtAzGKoA66081514;     FhrtAzGKoA66081514 = FhrtAzGKoA77733425;     FhrtAzGKoA77733425 = FhrtAzGKoA90472353;     FhrtAzGKoA90472353 = FhrtAzGKoA4777058;     FhrtAzGKoA4777058 = FhrtAzGKoA54018902;     FhrtAzGKoA54018902 = FhrtAzGKoA72956954;     FhrtAzGKoA72956954 = FhrtAzGKoA64130582;     FhrtAzGKoA64130582 = FhrtAzGKoA64549028;     FhrtAzGKoA64549028 = FhrtAzGKoA50506400;     FhrtAzGKoA50506400 = FhrtAzGKoA85410709;     FhrtAzGKoA85410709 = FhrtAzGKoA29406913;     FhrtAzGKoA29406913 = FhrtAzGKoA72548088;     FhrtAzGKoA72548088 = FhrtAzGKoA87302822;     FhrtAzGKoA87302822 = FhrtAzGKoA91569024;     FhrtAzGKoA91569024 = FhrtAzGKoA13848830;     FhrtAzGKoA13848830 = FhrtAzGKoA68171230;     FhrtAzGKoA68171230 = FhrtAzGKoA58964772;     FhrtAzGKoA58964772 = FhrtAzGKoA78304487;     FhrtAzGKoA78304487 = FhrtAzGKoA32932989;     FhrtAzGKoA32932989 = FhrtAzGKoA64968924;     FhrtAzGKoA64968924 = FhrtAzGKoA56956221;     FhrtAzGKoA56956221 = FhrtAzGKoA79487025;     FhrtAzGKoA79487025 = FhrtAzGKoA64664411;     FhrtAzGKoA64664411 = FhrtAzGKoA99832184;     FhrtAzGKoA99832184 = FhrtAzGKoA14367314;     FhrtAzGKoA14367314 = FhrtAzGKoA91434708;     FhrtAzGKoA91434708 = FhrtAzGKoA39561281;     FhrtAzGKoA39561281 = FhrtAzGKoA23065990;     FhrtAzGKoA23065990 = FhrtAzGKoA21041500;     FhrtAzGKoA21041500 = FhrtAzGKoA51819157;     FhrtAzGKoA51819157 = FhrtAzGKoA72164676;     FhrtAzGKoA72164676 = FhrtAzGKoA98634498;     FhrtAzGKoA98634498 = FhrtAzGKoA25482163;     FhrtAzGKoA25482163 = FhrtAzGKoA8258;     FhrtAzGKoA8258 = FhrtAzGKoA48590246;     FhrtAzGKoA48590246 = FhrtAzGKoA53022822;     FhrtAzGKoA53022822 = FhrtAzGKoA95156191;     FhrtAzGKoA95156191 = FhrtAzGKoA78046255;     FhrtAzGKoA78046255 = FhrtAzGKoA64698854;     FhrtAzGKoA64698854 = FhrtAzGKoA81238817;     FhrtAzGKoA81238817 = FhrtAzGKoA35091913;     FhrtAzGKoA35091913 = FhrtAzGKoA79643218;     FhrtAzGKoA79643218 = FhrtAzGKoA712446;     FhrtAzGKoA712446 = FhrtAzGKoA64251356;     FhrtAzGKoA64251356 = FhrtAzGKoA18211557;     FhrtAzGKoA18211557 = FhrtAzGKoA27765264;     FhrtAzGKoA27765264 = FhrtAzGKoA99256030;     FhrtAzGKoA99256030 = FhrtAzGKoA75613858;     FhrtAzGKoA75613858 = FhrtAzGKoA80698240;     FhrtAzGKoA80698240 = FhrtAzGKoA86260606;     FhrtAzGKoA86260606 = FhrtAzGKoA57152259;     FhrtAzGKoA57152259 = FhrtAzGKoA64057044;     FhrtAzGKoA64057044 = FhrtAzGKoA13750204;     FhrtAzGKoA13750204 = FhrtAzGKoA82907587;     FhrtAzGKoA82907587 = FhrtAzGKoA12606692;     FhrtAzGKoA12606692 = FhrtAzGKoA86594488;     FhrtAzGKoA86594488 = FhrtAzGKoA13069014;     FhrtAzGKoA13069014 = FhrtAzGKoA90640168;     FhrtAzGKoA90640168 = FhrtAzGKoA90409743;     FhrtAzGKoA90409743 = FhrtAzGKoA62584194;     FhrtAzGKoA62584194 = FhrtAzGKoA33395674;     FhrtAzGKoA33395674 = FhrtAzGKoA41064593;     FhrtAzGKoA41064593 = FhrtAzGKoA43507528;     FhrtAzGKoA43507528 = FhrtAzGKoA98687242;     FhrtAzGKoA98687242 = FhrtAzGKoA13246033;     FhrtAzGKoA13246033 = FhrtAzGKoA30772415;     FhrtAzGKoA30772415 = FhrtAzGKoA47065926;     FhrtAzGKoA47065926 = FhrtAzGKoA87294564;     FhrtAzGKoA87294564 = FhrtAzGKoA42978779;     FhrtAzGKoA42978779 = FhrtAzGKoA60826007;     FhrtAzGKoA60826007 = FhrtAzGKoA73015039;     FhrtAzGKoA73015039 = FhrtAzGKoA80918517;     FhrtAzGKoA80918517 = FhrtAzGKoA13605633;     FhrtAzGKoA13605633 = FhrtAzGKoA51694171;     FhrtAzGKoA51694171 = FhrtAzGKoA29877012;     FhrtAzGKoA29877012 = FhrtAzGKoA77313002;     FhrtAzGKoA77313002 = FhrtAzGKoA78774580;     FhrtAzGKoA78774580 = FhrtAzGKoA413055;     FhrtAzGKoA413055 = FhrtAzGKoA81620628;     FhrtAzGKoA81620628 = FhrtAzGKoA86602050;     FhrtAzGKoA86602050 = FhrtAzGKoA92178678;     FhrtAzGKoA92178678 = FhrtAzGKoA63947422;     FhrtAzGKoA63947422 = FhrtAzGKoA42367749;     FhrtAzGKoA42367749 = FhrtAzGKoA34780893;     FhrtAzGKoA34780893 = FhrtAzGKoA94666898;     FhrtAzGKoA94666898 = FhrtAzGKoA8107633;     FhrtAzGKoA8107633 = FhrtAzGKoA44192246;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void XPOReTSlvB35867470() {     int OaQDxuqMbu66579364 = -800676721;    int OaQDxuqMbu35566346 = -480222723;    int OaQDxuqMbu78431344 = -278236618;    int OaQDxuqMbu62625157 = -324454194;    int OaQDxuqMbu98487606 = -51565112;    int OaQDxuqMbu1441970 = -810932130;    int OaQDxuqMbu59338622 = -957985671;    int OaQDxuqMbu70180928 = -347335386;    int OaQDxuqMbu51594968 = -357921456;    int OaQDxuqMbu7880616 = -808440818;    int OaQDxuqMbu79903390 = -599131813;    int OaQDxuqMbu2586142 = -128468052;    int OaQDxuqMbu14884692 = 42341233;    int OaQDxuqMbu12177665 = -565878197;    int OaQDxuqMbu2816791 = -839784992;    int OaQDxuqMbu21989338 = -150091426;    int OaQDxuqMbu40295705 = -918293945;    int OaQDxuqMbu10753474 = -133148134;    int OaQDxuqMbu95300533 = -989400887;    int OaQDxuqMbu51473238 = -414063293;    int OaQDxuqMbu65013689 = -436521793;    int OaQDxuqMbu28364247 = -822149961;    int OaQDxuqMbu53799723 = -592744260;    int OaQDxuqMbu11910497 = -75762280;    int OaQDxuqMbu64947159 = -535540134;    int OaQDxuqMbu60851083 = -204678676;    int OaQDxuqMbu34392243 = -658371548;    int OaQDxuqMbu97480474 = -746569828;    int OaQDxuqMbu20741391 = -569796450;    int OaQDxuqMbu77328410 = -156365107;    int OaQDxuqMbu3255578 = 4349127;    int OaQDxuqMbu35845678 = -456078369;    int OaQDxuqMbu13971660 = -385119717;    int OaQDxuqMbu24357612 = -167604445;    int OaQDxuqMbu33185586 = -558987106;    int OaQDxuqMbu21179895 = -165990527;    int OaQDxuqMbu58104123 = -734124966;    int OaQDxuqMbu53458285 = 23187755;    int OaQDxuqMbu62675445 = -71451839;    int OaQDxuqMbu11620423 = -382734844;    int OaQDxuqMbu68318652 = -477075616;    int OaQDxuqMbu11574381 = -631507986;    int OaQDxuqMbu26591692 = -845169283;    int OaQDxuqMbu51263277 = -603017039;    int OaQDxuqMbu40147943 = -356193038;    int OaQDxuqMbu6734780 = -337985960;    int OaQDxuqMbu37779560 = 65074797;    int OaQDxuqMbu44606932 = -820538642;    int OaQDxuqMbu43050151 = -884786871;    int OaQDxuqMbu50106568 = -259630914;    int OaQDxuqMbu4025408 = 89359495;    int OaQDxuqMbu51897513 = -83368598;    int OaQDxuqMbu43013257 = -435022644;    int OaQDxuqMbu41525363 = 27083914;    int OaQDxuqMbu70853780 = -506485674;    int OaQDxuqMbu38215117 = -978526760;    int OaQDxuqMbu81766623 = -887478464;    int OaQDxuqMbu66520847 = -102474339;    int OaQDxuqMbu97677997 = -788914060;    int OaQDxuqMbu37636524 = -846886436;    int OaQDxuqMbu67049726 = -52560582;    int OaQDxuqMbu61858147 = -111415843;    int OaQDxuqMbu49439538 = -777538936;    int OaQDxuqMbu74266558 = -101556349;    int OaQDxuqMbu4625039 = -712789946;    int OaQDxuqMbu44057713 = -43053444;    int OaQDxuqMbu88614482 = -743348335;    int OaQDxuqMbu90527080 = -790054323;    int OaQDxuqMbu78992079 = 93108908;    int OaQDxuqMbu81636896 = -573794466;    int OaQDxuqMbu63885214 = -415966461;    int OaQDxuqMbu86837419 = -841481701;    int OaQDxuqMbu48078028 = 38303705;    int OaQDxuqMbu83680110 = -506666044;    int OaQDxuqMbu83154586 = -936987677;    int OaQDxuqMbu53439309 = -805013807;    int OaQDxuqMbu1772555 = -976980679;    int OaQDxuqMbu2536446 = -989727221;    int OaQDxuqMbu71762553 = -719569242;    int OaQDxuqMbu58212379 = -97554174;    int OaQDxuqMbu23071523 = -169753474;    int OaQDxuqMbu89785310 = -837832907;    int OaQDxuqMbu54430324 = -861782958;    int OaQDxuqMbu70634822 = -210165537;    int OaQDxuqMbu73303002 = -145724603;    int OaQDxuqMbu51358065 = -912282276;    int OaQDxuqMbu92832420 = 78944274;    int OaQDxuqMbu72446296 = -312203631;    int OaQDxuqMbu53503831 = -661118772;    int OaQDxuqMbu94970468 = -580460346;    int OaQDxuqMbu39413272 = -278512064;    int OaQDxuqMbu91583275 = -531650628;    int OaQDxuqMbu55780287 = -187898185;    int OaQDxuqMbu25038922 = -224565403;    int OaQDxuqMbu44570697 = -230174262;    int OaQDxuqMbu6460505 = -265659773;    int OaQDxuqMbu62134843 = -853969050;    int OaQDxuqMbu52325134 = -643612935;    int OaQDxuqMbu46638238 = -890227094;    int OaQDxuqMbu96090230 = -800676721;     OaQDxuqMbu66579364 = OaQDxuqMbu35566346;     OaQDxuqMbu35566346 = OaQDxuqMbu78431344;     OaQDxuqMbu78431344 = OaQDxuqMbu62625157;     OaQDxuqMbu62625157 = OaQDxuqMbu98487606;     OaQDxuqMbu98487606 = OaQDxuqMbu1441970;     OaQDxuqMbu1441970 = OaQDxuqMbu59338622;     OaQDxuqMbu59338622 = OaQDxuqMbu70180928;     OaQDxuqMbu70180928 = OaQDxuqMbu51594968;     OaQDxuqMbu51594968 = OaQDxuqMbu7880616;     OaQDxuqMbu7880616 = OaQDxuqMbu79903390;     OaQDxuqMbu79903390 = OaQDxuqMbu2586142;     OaQDxuqMbu2586142 = OaQDxuqMbu14884692;     OaQDxuqMbu14884692 = OaQDxuqMbu12177665;     OaQDxuqMbu12177665 = OaQDxuqMbu2816791;     OaQDxuqMbu2816791 = OaQDxuqMbu21989338;     OaQDxuqMbu21989338 = OaQDxuqMbu40295705;     OaQDxuqMbu40295705 = OaQDxuqMbu10753474;     OaQDxuqMbu10753474 = OaQDxuqMbu95300533;     OaQDxuqMbu95300533 = OaQDxuqMbu51473238;     OaQDxuqMbu51473238 = OaQDxuqMbu65013689;     OaQDxuqMbu65013689 = OaQDxuqMbu28364247;     OaQDxuqMbu28364247 = OaQDxuqMbu53799723;     OaQDxuqMbu53799723 = OaQDxuqMbu11910497;     OaQDxuqMbu11910497 = OaQDxuqMbu64947159;     OaQDxuqMbu64947159 = OaQDxuqMbu60851083;     OaQDxuqMbu60851083 = OaQDxuqMbu34392243;     OaQDxuqMbu34392243 = OaQDxuqMbu97480474;     OaQDxuqMbu97480474 = OaQDxuqMbu20741391;     OaQDxuqMbu20741391 = OaQDxuqMbu77328410;     OaQDxuqMbu77328410 = OaQDxuqMbu3255578;     OaQDxuqMbu3255578 = OaQDxuqMbu35845678;     OaQDxuqMbu35845678 = OaQDxuqMbu13971660;     OaQDxuqMbu13971660 = OaQDxuqMbu24357612;     OaQDxuqMbu24357612 = OaQDxuqMbu33185586;     OaQDxuqMbu33185586 = OaQDxuqMbu21179895;     OaQDxuqMbu21179895 = OaQDxuqMbu58104123;     OaQDxuqMbu58104123 = OaQDxuqMbu53458285;     OaQDxuqMbu53458285 = OaQDxuqMbu62675445;     OaQDxuqMbu62675445 = OaQDxuqMbu11620423;     OaQDxuqMbu11620423 = OaQDxuqMbu68318652;     OaQDxuqMbu68318652 = OaQDxuqMbu11574381;     OaQDxuqMbu11574381 = OaQDxuqMbu26591692;     OaQDxuqMbu26591692 = OaQDxuqMbu51263277;     OaQDxuqMbu51263277 = OaQDxuqMbu40147943;     OaQDxuqMbu40147943 = OaQDxuqMbu6734780;     OaQDxuqMbu6734780 = OaQDxuqMbu37779560;     OaQDxuqMbu37779560 = OaQDxuqMbu44606932;     OaQDxuqMbu44606932 = OaQDxuqMbu43050151;     OaQDxuqMbu43050151 = OaQDxuqMbu50106568;     OaQDxuqMbu50106568 = OaQDxuqMbu4025408;     OaQDxuqMbu4025408 = OaQDxuqMbu51897513;     OaQDxuqMbu51897513 = OaQDxuqMbu43013257;     OaQDxuqMbu43013257 = OaQDxuqMbu41525363;     OaQDxuqMbu41525363 = OaQDxuqMbu70853780;     OaQDxuqMbu70853780 = OaQDxuqMbu38215117;     OaQDxuqMbu38215117 = OaQDxuqMbu81766623;     OaQDxuqMbu81766623 = OaQDxuqMbu66520847;     OaQDxuqMbu66520847 = OaQDxuqMbu97677997;     OaQDxuqMbu97677997 = OaQDxuqMbu37636524;     OaQDxuqMbu37636524 = OaQDxuqMbu67049726;     OaQDxuqMbu67049726 = OaQDxuqMbu61858147;     OaQDxuqMbu61858147 = OaQDxuqMbu49439538;     OaQDxuqMbu49439538 = OaQDxuqMbu74266558;     OaQDxuqMbu74266558 = OaQDxuqMbu4625039;     OaQDxuqMbu4625039 = OaQDxuqMbu44057713;     OaQDxuqMbu44057713 = OaQDxuqMbu88614482;     OaQDxuqMbu88614482 = OaQDxuqMbu90527080;     OaQDxuqMbu90527080 = OaQDxuqMbu78992079;     OaQDxuqMbu78992079 = OaQDxuqMbu81636896;     OaQDxuqMbu81636896 = OaQDxuqMbu63885214;     OaQDxuqMbu63885214 = OaQDxuqMbu86837419;     OaQDxuqMbu86837419 = OaQDxuqMbu48078028;     OaQDxuqMbu48078028 = OaQDxuqMbu83680110;     OaQDxuqMbu83680110 = OaQDxuqMbu83154586;     OaQDxuqMbu83154586 = OaQDxuqMbu53439309;     OaQDxuqMbu53439309 = OaQDxuqMbu1772555;     OaQDxuqMbu1772555 = OaQDxuqMbu2536446;     OaQDxuqMbu2536446 = OaQDxuqMbu71762553;     OaQDxuqMbu71762553 = OaQDxuqMbu58212379;     OaQDxuqMbu58212379 = OaQDxuqMbu23071523;     OaQDxuqMbu23071523 = OaQDxuqMbu89785310;     OaQDxuqMbu89785310 = OaQDxuqMbu54430324;     OaQDxuqMbu54430324 = OaQDxuqMbu70634822;     OaQDxuqMbu70634822 = OaQDxuqMbu73303002;     OaQDxuqMbu73303002 = OaQDxuqMbu51358065;     OaQDxuqMbu51358065 = OaQDxuqMbu92832420;     OaQDxuqMbu92832420 = OaQDxuqMbu72446296;     OaQDxuqMbu72446296 = OaQDxuqMbu53503831;     OaQDxuqMbu53503831 = OaQDxuqMbu94970468;     OaQDxuqMbu94970468 = OaQDxuqMbu39413272;     OaQDxuqMbu39413272 = OaQDxuqMbu91583275;     OaQDxuqMbu91583275 = OaQDxuqMbu55780287;     OaQDxuqMbu55780287 = OaQDxuqMbu25038922;     OaQDxuqMbu25038922 = OaQDxuqMbu44570697;     OaQDxuqMbu44570697 = OaQDxuqMbu6460505;     OaQDxuqMbu6460505 = OaQDxuqMbu62134843;     OaQDxuqMbu62134843 = OaQDxuqMbu52325134;     OaQDxuqMbu52325134 = OaQDxuqMbu46638238;     OaQDxuqMbu46638238 = OaQDxuqMbu96090230;     OaQDxuqMbu96090230 = OaQDxuqMbu66579364;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ycpSpaliXP72512444() {     int ljIskUDPXf88966481 = -435080684;    int ljIskUDPXf19913846 = -428765778;    int ljIskUDPXf7200623 = -517890462;    int ljIskUDPXf77631846 = -330861726;    int ljIskUDPXf96661860 = -642622114;    int ljIskUDPXf16325087 = -671540508;    int ljIskUDPXf51494361 = -779972977;    int ljIskUDPXf45814787 = -817932030;    int ljIskUDPXf48758099 = -242117495;    int ljIskUDPXf99644200 = -551182151;    int ljIskUDPXf17445250 = -595756213;    int ljIskUDPXf58489090 = -379213345;    int ljIskUDPXf81892872 = -576376087;    int ljIskUDPXf54792417 = 6413046;    int ljIskUDPXf39552068 = -467262705;    int ljIskUDPXf66245250 = -531758969;    int ljIskUDPXf90119056 = -95573765;    int ljIskUDPXf16729890 = -584890824;    int ljIskUDPXf36582164 = -24503012;    int ljIskUDPXf29989521 = -666697746;    int ljIskUDPXf65896796 = -428009101;    int ljIskUDPXf92179464 = -226916275;    int ljIskUDPXf57093046 = -768824852;    int ljIskUDPXf38410284 = -639755137;    int ljIskUDPXf487407 = -157099259;    int ljIskUDPXf49154078 = -648572900;    int ljIskUDPXf81481663 = -415958041;    int ljIskUDPXf3391925 = -535024282;    int ljIskUDPXf27633951 = -914053569;    int ljIskUDPXf86485589 = -76660974;    int ljIskUDPXf47546383 = -76967025;    int ljIskUDPXf93386868 = -8149724;    int ljIskUDPXf95010329 = -914463212;    int ljIskUDPXf83746298 = -491731295;    int ljIskUDPXf9414950 = -622743528;    int ljIskUDPXf62872764 = -481385903;    int ljIskUDPXf51543835 = -467072917;    int ljIskUDPXf7084387 = -944437717;    int ljIskUDPXf10983577 = -472497871;    int ljIskUDPXf31806138 = -518958873;    int ljIskUDPXf97076022 = -980253282;    int ljIskUDPXf82772 = -389416677;    int ljIskUDPXf32141884 = -732699867;    int ljIskUDPXf50707396 = -600380688;    int ljIskUDPXf8131210 = -165336516;    int ljIskUDPXf14835062 = -520215056;    int ljIskUDPXf50076958 = -513335456;    int ljIskUDPXf89205606 = -577073633;    int ljIskUDPXf37510056 = -834503569;    int ljIskUDPXf47190313 = -401651173;    int ljIskUDPXf12894625 = -265716782;    int ljIskUDPXf25748770 = -514187880;    int ljIskUDPXf21327660 = -106933874;    int ljIskUDPXf1811908 = -248596895;    int ljIskUDPXf6615648 = -647189789;    int ljIskUDPXf96787016 = -108164410;    int ljIskUDPXf62820800 = -659940926;    int ljIskUDPXf68790339 = -878135325;    int ljIskUDPXf77144439 = -73762467;    int ljIskUDPXf47507783 = -994049214;    int ljIskUDPXf34843423 = -155582467;    int ljIskUDPXf48102436 = -144948696;    int ljIskUDPXf18180836 = -903878462;    int ljIskUDPXf62272510 = -65456522;    int ljIskUDPXf52097818 = -374215127;    int ljIskUDPXf24058381 = -487606490;    int ljIskUDPXf63478760 = -464750134;    int ljIskUDPXf98146573 = 15355208;    int ljIskUDPXf45377467 = -370843427;    int ljIskUDPXf76679304 = -985876802;    int ljIskUDPXf14701415 = 35313948;    int ljIskUDPXf83034670 = -151136049;    int ljIskUDPXf5746314 = -12392954;    int ljIskUDPXf4776027 = -505544140;    int ljIskUDPXf32913498 = -686444465;    int ljIskUDPXf65814025 = 61407576;    int ljIskUDPXf60037581 = -494216408;    int ljIskUDPXf6385650 = -68444164;    int ljIskUDPXf30279074 = -374418621;    int ljIskUDPXf85652344 = -636884204;    int ljIskUDPXf99077120 = -35237445;    int ljIskUDPXf92276056 = -838884409;    int ljIskUDPXf65881869 = -700520713;    int ljIskUDPXf80443638 = -412402397;    int ljIskUDPXf73590965 = -810944193;    int ljIskUDPXf21797613 = -562779146;    int ljIskUDPXf72059208 = -901215850;    int ljIskUDPXf93198421 = -565866317;    int ljIskUDPXf77130651 = -844541506;    int ljIskUDPXf12627934 = -414579118;    int ljIskUDPXf51964 = -821444978;    int ljIskUDPXf82753496 = -588937592;    int ljIskUDPXf29939947 = -770675250;    int ljIskUDPXf63475793 = -478448658;    int ljIskUDPXf96962714 = -263376406;    int ljIskUDPXf48973587 = -735304586;    int ljIskUDPXf81901936 = -485538216;    int ljIskUDPXf69869374 = -567243346;    int ljIskUDPXf98609578 = -126165562;    int ljIskUDPXf84072828 = -435080684;     ljIskUDPXf88966481 = ljIskUDPXf19913846;     ljIskUDPXf19913846 = ljIskUDPXf7200623;     ljIskUDPXf7200623 = ljIskUDPXf77631846;     ljIskUDPXf77631846 = ljIskUDPXf96661860;     ljIskUDPXf96661860 = ljIskUDPXf16325087;     ljIskUDPXf16325087 = ljIskUDPXf51494361;     ljIskUDPXf51494361 = ljIskUDPXf45814787;     ljIskUDPXf45814787 = ljIskUDPXf48758099;     ljIskUDPXf48758099 = ljIskUDPXf99644200;     ljIskUDPXf99644200 = ljIskUDPXf17445250;     ljIskUDPXf17445250 = ljIskUDPXf58489090;     ljIskUDPXf58489090 = ljIskUDPXf81892872;     ljIskUDPXf81892872 = ljIskUDPXf54792417;     ljIskUDPXf54792417 = ljIskUDPXf39552068;     ljIskUDPXf39552068 = ljIskUDPXf66245250;     ljIskUDPXf66245250 = ljIskUDPXf90119056;     ljIskUDPXf90119056 = ljIskUDPXf16729890;     ljIskUDPXf16729890 = ljIskUDPXf36582164;     ljIskUDPXf36582164 = ljIskUDPXf29989521;     ljIskUDPXf29989521 = ljIskUDPXf65896796;     ljIskUDPXf65896796 = ljIskUDPXf92179464;     ljIskUDPXf92179464 = ljIskUDPXf57093046;     ljIskUDPXf57093046 = ljIskUDPXf38410284;     ljIskUDPXf38410284 = ljIskUDPXf487407;     ljIskUDPXf487407 = ljIskUDPXf49154078;     ljIskUDPXf49154078 = ljIskUDPXf81481663;     ljIskUDPXf81481663 = ljIskUDPXf3391925;     ljIskUDPXf3391925 = ljIskUDPXf27633951;     ljIskUDPXf27633951 = ljIskUDPXf86485589;     ljIskUDPXf86485589 = ljIskUDPXf47546383;     ljIskUDPXf47546383 = ljIskUDPXf93386868;     ljIskUDPXf93386868 = ljIskUDPXf95010329;     ljIskUDPXf95010329 = ljIskUDPXf83746298;     ljIskUDPXf83746298 = ljIskUDPXf9414950;     ljIskUDPXf9414950 = ljIskUDPXf62872764;     ljIskUDPXf62872764 = ljIskUDPXf51543835;     ljIskUDPXf51543835 = ljIskUDPXf7084387;     ljIskUDPXf7084387 = ljIskUDPXf10983577;     ljIskUDPXf10983577 = ljIskUDPXf31806138;     ljIskUDPXf31806138 = ljIskUDPXf97076022;     ljIskUDPXf97076022 = ljIskUDPXf82772;     ljIskUDPXf82772 = ljIskUDPXf32141884;     ljIskUDPXf32141884 = ljIskUDPXf50707396;     ljIskUDPXf50707396 = ljIskUDPXf8131210;     ljIskUDPXf8131210 = ljIskUDPXf14835062;     ljIskUDPXf14835062 = ljIskUDPXf50076958;     ljIskUDPXf50076958 = ljIskUDPXf89205606;     ljIskUDPXf89205606 = ljIskUDPXf37510056;     ljIskUDPXf37510056 = ljIskUDPXf47190313;     ljIskUDPXf47190313 = ljIskUDPXf12894625;     ljIskUDPXf12894625 = ljIskUDPXf25748770;     ljIskUDPXf25748770 = ljIskUDPXf21327660;     ljIskUDPXf21327660 = ljIskUDPXf1811908;     ljIskUDPXf1811908 = ljIskUDPXf6615648;     ljIskUDPXf6615648 = ljIskUDPXf96787016;     ljIskUDPXf96787016 = ljIskUDPXf62820800;     ljIskUDPXf62820800 = ljIskUDPXf68790339;     ljIskUDPXf68790339 = ljIskUDPXf77144439;     ljIskUDPXf77144439 = ljIskUDPXf47507783;     ljIskUDPXf47507783 = ljIskUDPXf34843423;     ljIskUDPXf34843423 = ljIskUDPXf48102436;     ljIskUDPXf48102436 = ljIskUDPXf18180836;     ljIskUDPXf18180836 = ljIskUDPXf62272510;     ljIskUDPXf62272510 = ljIskUDPXf52097818;     ljIskUDPXf52097818 = ljIskUDPXf24058381;     ljIskUDPXf24058381 = ljIskUDPXf63478760;     ljIskUDPXf63478760 = ljIskUDPXf98146573;     ljIskUDPXf98146573 = ljIskUDPXf45377467;     ljIskUDPXf45377467 = ljIskUDPXf76679304;     ljIskUDPXf76679304 = ljIskUDPXf14701415;     ljIskUDPXf14701415 = ljIskUDPXf83034670;     ljIskUDPXf83034670 = ljIskUDPXf5746314;     ljIskUDPXf5746314 = ljIskUDPXf4776027;     ljIskUDPXf4776027 = ljIskUDPXf32913498;     ljIskUDPXf32913498 = ljIskUDPXf65814025;     ljIskUDPXf65814025 = ljIskUDPXf60037581;     ljIskUDPXf60037581 = ljIskUDPXf6385650;     ljIskUDPXf6385650 = ljIskUDPXf30279074;     ljIskUDPXf30279074 = ljIskUDPXf85652344;     ljIskUDPXf85652344 = ljIskUDPXf99077120;     ljIskUDPXf99077120 = ljIskUDPXf92276056;     ljIskUDPXf92276056 = ljIskUDPXf65881869;     ljIskUDPXf65881869 = ljIskUDPXf80443638;     ljIskUDPXf80443638 = ljIskUDPXf73590965;     ljIskUDPXf73590965 = ljIskUDPXf21797613;     ljIskUDPXf21797613 = ljIskUDPXf72059208;     ljIskUDPXf72059208 = ljIskUDPXf93198421;     ljIskUDPXf93198421 = ljIskUDPXf77130651;     ljIskUDPXf77130651 = ljIskUDPXf12627934;     ljIskUDPXf12627934 = ljIskUDPXf51964;     ljIskUDPXf51964 = ljIskUDPXf82753496;     ljIskUDPXf82753496 = ljIskUDPXf29939947;     ljIskUDPXf29939947 = ljIskUDPXf63475793;     ljIskUDPXf63475793 = ljIskUDPXf96962714;     ljIskUDPXf96962714 = ljIskUDPXf48973587;     ljIskUDPXf48973587 = ljIskUDPXf81901936;     ljIskUDPXf81901936 = ljIskUDPXf69869374;     ljIskUDPXf69869374 = ljIskUDPXf98609578;     ljIskUDPXf98609578 = ljIskUDPXf84072828;     ljIskUDPXf84072828 = ljIskUDPXf88966481;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void YoGtEZcKdo9157419() {     int WAiCzPBxrU11353599 = -69484645;    int WAiCzPBxrU4261347 = -377308826;    int WAiCzPBxrU35969902 = -757544306;    int WAiCzPBxrU92638534 = -337269260;    int WAiCzPBxrU94836115 = -133679107;    int WAiCzPBxrU31208204 = -532148886;    int WAiCzPBxrU43650100 = -601960283;    int WAiCzPBxrU21448646 = -188528659;    int WAiCzPBxrU45921231 = -126313533;    int WAiCzPBxrU91407785 = -293923484;    int WAiCzPBxrU54987109 = -592380613;    int WAiCzPBxrU14392039 = -629958629;    int WAiCzPBxrU48901053 = -95093406;    int WAiCzPBxrU97407169 = -521295710;    int WAiCzPBxrU76287345 = -94740400;    int WAiCzPBxrU10501162 = -913426511;    int WAiCzPBxrU39942408 = -372853585;    int WAiCzPBxrU22706306 = 63366489;    int WAiCzPBxrU77863794 = -159605138;    int WAiCzPBxrU8505805 = -919332200;    int WAiCzPBxrU66779904 = -419496404;    int WAiCzPBxrU55994683 = -731682588;    int WAiCzPBxrU60386369 = -944905444;    int WAiCzPBxrU64910071 = -103747994;    int WAiCzPBxrU36027653 = -878658371;    int WAiCzPBxrU37457072 = 7532872;    int WAiCzPBxrU28571084 = -173544534;    int WAiCzPBxrU9303376 = -323478729;    int WAiCzPBxrU34526512 = -158310704;    int WAiCzPBxrU95642769 = 3043160;    int WAiCzPBxrU91837188 = -158283177;    int WAiCzPBxrU50928060 = -660221082;    int WAiCzPBxrU76049000 = -343806710;    int WAiCzPBxrU43134986 = -815858145;    int WAiCzPBxrU85644314 = -686499954;    int WAiCzPBxrU4565633 = -796781298;    int WAiCzPBxrU44983548 = -200020869;    int WAiCzPBxrU60710487 = -812063186;    int WAiCzPBxrU59291708 = -873543915;    int WAiCzPBxrU51991853 = -655182902;    int WAiCzPBxrU25833394 = -383430948;    int WAiCzPBxrU88591163 = -147325389;    int WAiCzPBxrU37692076 = -620230453;    int WAiCzPBxrU50151515 = -597744337;    int WAiCzPBxrU76114476 = 25520005;    int WAiCzPBxrU22935344 = -702444170;    int WAiCzPBxrU62374355 = 8254291;    int WAiCzPBxrU33804281 = -333608625;    int WAiCzPBxrU31969961 = -784220292;    int WAiCzPBxrU44274059 = -543671426;    int WAiCzPBxrU21763841 = -620793059;    int WAiCzPBxrU99600027 = -945007166;    int WAiCzPBxrU99642063 = -878845101;    int WAiCzPBxrU62098453 = -524277704;    int WAiCzPBxrU42377515 = -787893909;    int WAiCzPBxrU55358915 = -337802058;    int WAiCzPBxrU43874978 = -432403383;    int WAiCzPBxrU71059830 = -553796312;    int WAiCzPBxrU56610881 = -458610890;    int WAiCzPBxrU57379043 = -41211979;    int WAiCzPBxrU2637120 = -258604352;    int WAiCzPBxrU34346725 = -178481555;    int WAiCzPBxrU86922133 = 69782044;    int WAiCzPBxrU50278461 = -29356693;    int WAiCzPBxrU99570597 = -35640308;    int WAiCzPBxrU4059050 = -932159532;    int WAiCzPBxrU38343039 = -186151920;    int WAiCzPBxrU5766067 = -279235262;    int WAiCzPBxrU11762855 = -834795757;    int WAiCzPBxrU71721712 = -297959102;    int WAiCzPBxrU65517614 = -613405643;    int WAiCzPBxrU79231921 = -560790400;    int WAiCzPBxrU63414598 = -63089597;    int WAiCzPBxrU25871942 = -504422237;    int WAiCzPBxrU82672410 = -435901253;    int WAiCzPBxrU78188740 = -172171016;    int WAiCzPBxrU18302608 = -11452136;    int WAiCzPBxrU10234854 = -247161107;    int WAiCzPBxrU88795594 = -29268000;    int WAiCzPBxrU13092310 = -76214202;    int WAiCzPBxrU75082716 = 99278580;    int WAiCzPBxrU94766802 = -839935910;    int WAiCzPBxrU77333414 = -539258438;    int WAiCzPBxrU90252452 = -614639278;    int WAiCzPBxrU73878928 = -376163782;    int WAiCzPBxrU92237160 = -213276011;    int WAiCzPBxrU51285996 = -781375981;    int WAiCzPBxrU13950547 = -819529006;    int WAiCzPBxrU757471 = 72035764;    int WAiCzPBxrU30285400 = -248697896;    int WAiCzPBxrU60690655 = -264377916;    int WAiCzPBxrU73923717 = -646224557;    int WAiCzPBxrU4099606 = -253452296;    int WAiCzPBxrU1912665 = -732331937;    int WAiCzPBxrU49354733 = -296578550;    int WAiCzPBxrU91486668 = -104949394;    int WAiCzPBxrU1669030 = -117107433;    int WAiCzPBxrU87413614 = -490873760;    int WAiCzPBxrU50580918 = -462104030;    int WAiCzPBxrU72055426 = -69484645;     WAiCzPBxrU11353599 = WAiCzPBxrU4261347;     WAiCzPBxrU4261347 = WAiCzPBxrU35969902;     WAiCzPBxrU35969902 = WAiCzPBxrU92638534;     WAiCzPBxrU92638534 = WAiCzPBxrU94836115;     WAiCzPBxrU94836115 = WAiCzPBxrU31208204;     WAiCzPBxrU31208204 = WAiCzPBxrU43650100;     WAiCzPBxrU43650100 = WAiCzPBxrU21448646;     WAiCzPBxrU21448646 = WAiCzPBxrU45921231;     WAiCzPBxrU45921231 = WAiCzPBxrU91407785;     WAiCzPBxrU91407785 = WAiCzPBxrU54987109;     WAiCzPBxrU54987109 = WAiCzPBxrU14392039;     WAiCzPBxrU14392039 = WAiCzPBxrU48901053;     WAiCzPBxrU48901053 = WAiCzPBxrU97407169;     WAiCzPBxrU97407169 = WAiCzPBxrU76287345;     WAiCzPBxrU76287345 = WAiCzPBxrU10501162;     WAiCzPBxrU10501162 = WAiCzPBxrU39942408;     WAiCzPBxrU39942408 = WAiCzPBxrU22706306;     WAiCzPBxrU22706306 = WAiCzPBxrU77863794;     WAiCzPBxrU77863794 = WAiCzPBxrU8505805;     WAiCzPBxrU8505805 = WAiCzPBxrU66779904;     WAiCzPBxrU66779904 = WAiCzPBxrU55994683;     WAiCzPBxrU55994683 = WAiCzPBxrU60386369;     WAiCzPBxrU60386369 = WAiCzPBxrU64910071;     WAiCzPBxrU64910071 = WAiCzPBxrU36027653;     WAiCzPBxrU36027653 = WAiCzPBxrU37457072;     WAiCzPBxrU37457072 = WAiCzPBxrU28571084;     WAiCzPBxrU28571084 = WAiCzPBxrU9303376;     WAiCzPBxrU9303376 = WAiCzPBxrU34526512;     WAiCzPBxrU34526512 = WAiCzPBxrU95642769;     WAiCzPBxrU95642769 = WAiCzPBxrU91837188;     WAiCzPBxrU91837188 = WAiCzPBxrU50928060;     WAiCzPBxrU50928060 = WAiCzPBxrU76049000;     WAiCzPBxrU76049000 = WAiCzPBxrU43134986;     WAiCzPBxrU43134986 = WAiCzPBxrU85644314;     WAiCzPBxrU85644314 = WAiCzPBxrU4565633;     WAiCzPBxrU4565633 = WAiCzPBxrU44983548;     WAiCzPBxrU44983548 = WAiCzPBxrU60710487;     WAiCzPBxrU60710487 = WAiCzPBxrU59291708;     WAiCzPBxrU59291708 = WAiCzPBxrU51991853;     WAiCzPBxrU51991853 = WAiCzPBxrU25833394;     WAiCzPBxrU25833394 = WAiCzPBxrU88591163;     WAiCzPBxrU88591163 = WAiCzPBxrU37692076;     WAiCzPBxrU37692076 = WAiCzPBxrU50151515;     WAiCzPBxrU50151515 = WAiCzPBxrU76114476;     WAiCzPBxrU76114476 = WAiCzPBxrU22935344;     WAiCzPBxrU22935344 = WAiCzPBxrU62374355;     WAiCzPBxrU62374355 = WAiCzPBxrU33804281;     WAiCzPBxrU33804281 = WAiCzPBxrU31969961;     WAiCzPBxrU31969961 = WAiCzPBxrU44274059;     WAiCzPBxrU44274059 = WAiCzPBxrU21763841;     WAiCzPBxrU21763841 = WAiCzPBxrU99600027;     WAiCzPBxrU99600027 = WAiCzPBxrU99642063;     WAiCzPBxrU99642063 = WAiCzPBxrU62098453;     WAiCzPBxrU62098453 = WAiCzPBxrU42377515;     WAiCzPBxrU42377515 = WAiCzPBxrU55358915;     WAiCzPBxrU55358915 = WAiCzPBxrU43874978;     WAiCzPBxrU43874978 = WAiCzPBxrU71059830;     WAiCzPBxrU71059830 = WAiCzPBxrU56610881;     WAiCzPBxrU56610881 = WAiCzPBxrU57379043;     WAiCzPBxrU57379043 = WAiCzPBxrU2637120;     WAiCzPBxrU2637120 = WAiCzPBxrU34346725;     WAiCzPBxrU34346725 = WAiCzPBxrU86922133;     WAiCzPBxrU86922133 = WAiCzPBxrU50278461;     WAiCzPBxrU50278461 = WAiCzPBxrU99570597;     WAiCzPBxrU99570597 = WAiCzPBxrU4059050;     WAiCzPBxrU4059050 = WAiCzPBxrU38343039;     WAiCzPBxrU38343039 = WAiCzPBxrU5766067;     WAiCzPBxrU5766067 = WAiCzPBxrU11762855;     WAiCzPBxrU11762855 = WAiCzPBxrU71721712;     WAiCzPBxrU71721712 = WAiCzPBxrU65517614;     WAiCzPBxrU65517614 = WAiCzPBxrU79231921;     WAiCzPBxrU79231921 = WAiCzPBxrU63414598;     WAiCzPBxrU63414598 = WAiCzPBxrU25871942;     WAiCzPBxrU25871942 = WAiCzPBxrU82672410;     WAiCzPBxrU82672410 = WAiCzPBxrU78188740;     WAiCzPBxrU78188740 = WAiCzPBxrU18302608;     WAiCzPBxrU18302608 = WAiCzPBxrU10234854;     WAiCzPBxrU10234854 = WAiCzPBxrU88795594;     WAiCzPBxrU88795594 = WAiCzPBxrU13092310;     WAiCzPBxrU13092310 = WAiCzPBxrU75082716;     WAiCzPBxrU75082716 = WAiCzPBxrU94766802;     WAiCzPBxrU94766802 = WAiCzPBxrU77333414;     WAiCzPBxrU77333414 = WAiCzPBxrU90252452;     WAiCzPBxrU90252452 = WAiCzPBxrU73878928;     WAiCzPBxrU73878928 = WAiCzPBxrU92237160;     WAiCzPBxrU92237160 = WAiCzPBxrU51285996;     WAiCzPBxrU51285996 = WAiCzPBxrU13950547;     WAiCzPBxrU13950547 = WAiCzPBxrU757471;     WAiCzPBxrU757471 = WAiCzPBxrU30285400;     WAiCzPBxrU30285400 = WAiCzPBxrU60690655;     WAiCzPBxrU60690655 = WAiCzPBxrU73923717;     WAiCzPBxrU73923717 = WAiCzPBxrU4099606;     WAiCzPBxrU4099606 = WAiCzPBxrU1912665;     WAiCzPBxrU1912665 = WAiCzPBxrU49354733;     WAiCzPBxrU49354733 = WAiCzPBxrU91486668;     WAiCzPBxrU91486668 = WAiCzPBxrU1669030;     WAiCzPBxrU1669030 = WAiCzPBxrU87413614;     WAiCzPBxrU87413614 = WAiCzPBxrU50580918;     WAiCzPBxrU50580918 = WAiCzPBxrU72055426;     WAiCzPBxrU72055426 = WAiCzPBxrU11353599;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void tfCvVZghBH45802393() {     int IbeGuqsCaJ33740716 = -803888610;    int IbeGuqsCaJ88608847 = -325851887;    int IbeGuqsCaJ64739180 = -997198150;    int IbeGuqsCaJ7645223 = -343676789;    int IbeGuqsCaJ93010369 = -724736116;    int IbeGuqsCaJ46091320 = -392757264;    int IbeGuqsCaJ35805839 = -423947589;    int IbeGuqsCaJ97082504 = -659125319;    int IbeGuqsCaJ43084362 = -10509573;    int IbeGuqsCaJ83171370 = -36664817;    int IbeGuqsCaJ92528969 = -589005015;    int IbeGuqsCaJ70294988 = -880703932;    int IbeGuqsCaJ15909234 = -713810726;    int IbeGuqsCaJ40021921 = 50995532;    int IbeGuqsCaJ13022622 = -822218131;    int IbeGuqsCaJ54757075 = -195094054;    int IbeGuqsCaJ89765760 = -650133405;    int IbeGuqsCaJ28682723 = -388376206;    int IbeGuqsCaJ19145426 = -294707263;    int IbeGuqsCaJ87022087 = -71966653;    int IbeGuqsCaJ67663011 = -410983718;    int IbeGuqsCaJ19809902 = -136448901;    int IbeGuqsCaJ63679692 = -20986036;    int IbeGuqsCaJ91409858 = -667740851;    int IbeGuqsCaJ71567900 = -500217508;    int IbeGuqsCaJ25760067 = -436361348;    int IbeGuqsCaJ75660504 = 68868973;    int IbeGuqsCaJ15214826 = -111933189;    int IbeGuqsCaJ41419073 = -502567808;    int IbeGuqsCaJ4799949 = 82747293;    int IbeGuqsCaJ36127993 = -239599329;    int IbeGuqsCaJ8469251 = -212292433;    int IbeGuqsCaJ57087670 = -873150202;    int IbeGuqsCaJ2523674 = -39984995;    int IbeGuqsCaJ61873679 = -750256373;    int IbeGuqsCaJ46258502 = -12176656;    int IbeGuqsCaJ38423260 = 67031180;    int IbeGuqsCaJ14336588 = -679688661;    int IbeGuqsCaJ7599840 = -174589934;    int IbeGuqsCaJ72177567 = -791406931;    int IbeGuqsCaJ54590764 = -886608614;    int IbeGuqsCaJ77099554 = 94765941;    int IbeGuqsCaJ43242268 = -507761034;    int IbeGuqsCaJ49595635 = -595107986;    int IbeGuqsCaJ44097743 = -883623471;    int IbeGuqsCaJ31035626 = -884673247;    int IbeGuqsCaJ74671752 = -570155962;    int IbeGuqsCaJ78402956 = -90143614;    int IbeGuqsCaJ26429866 = -733936966;    int IbeGuqsCaJ41357805 = -685691690;    int IbeGuqsCaJ30633057 = -975869336;    int IbeGuqsCaJ73451284 = -275826444;    int IbeGuqsCaJ77956466 = -550756335;    int IbeGuqsCaJ22384999 = -799958513;    int IbeGuqsCaJ78139382 = -928598019;    int IbeGuqsCaJ13930815 = -567439710;    int IbeGuqsCaJ24929155 = -204865851;    int IbeGuqsCaJ73329322 = -229457299;    int IbeGuqsCaJ36077323 = -843459282;    int IbeGuqsCaJ67250302 = -188374769;    int IbeGuqsCaJ70430816 = -361626238;    int IbeGuqsCaJ20591014 = -212014401;    int IbeGuqsCaJ55663431 = -56557512;    int IbeGuqsCaJ38284413 = 6743133;    int IbeGuqsCaJ47043377 = -797065489;    int IbeGuqsCaJ84059718 = -276712582;    int IbeGuqsCaJ13207318 = 92446269;    int IbeGuqsCaJ13385561 = -573825731;    int IbeGuqsCaJ78148242 = -198748096;    int IbeGuqsCaJ66764119 = -710041475;    int IbeGuqsCaJ16333815 = -162125234;    int IbeGuqsCaJ75429172 = -970444744;    int IbeGuqsCaJ21082884 = -113786272;    int IbeGuqsCaJ46967859 = -503300332;    int IbeGuqsCaJ32431323 = -185358040;    int IbeGuqsCaJ90563456 = -405749659;    int IbeGuqsCaJ76567633 = -628687867;    int IbeGuqsCaJ14084058 = -425878051;    int IbeGuqsCaJ47312115 = -784117380;    int IbeGuqsCaJ40532274 = -615544262;    int IbeGuqsCaJ51088314 = -866205387;    int IbeGuqsCaJ97257548 = -840987413;    int IbeGuqsCaJ88784959 = -377996224;    int IbeGuqsCaJ61269 = -816876118;    int IbeGuqsCaJ74166891 = 58616628;    int IbeGuqsCaJ62676709 = -963772886;    int IbeGuqsCaJ30512784 = -661536099;    int IbeGuqsCaJ34702672 = 26808311;    int IbeGuqsCaJ24384291 = -111386976;    int IbeGuqsCaJ47942865 = -82816664;    int IbeGuqsCaJ21329348 = -807310805;    int IbeGuqsCaJ65093937 = -703511521;    int IbeGuqsCaJ78259265 = -836229380;    int IbeGuqsCaJ40349537 = -986215166;    int IbeGuqsCaJ1746751 = -329780694;    int IbeGuqsCaJ33999751 = -574594213;    int IbeGuqsCaJ21436123 = -848676548;    int IbeGuqsCaJ4957855 = -414504168;    int IbeGuqsCaJ2552258 = -798042498;    int IbeGuqsCaJ60038024 = -803888610;     IbeGuqsCaJ33740716 = IbeGuqsCaJ88608847;     IbeGuqsCaJ88608847 = IbeGuqsCaJ64739180;     IbeGuqsCaJ64739180 = IbeGuqsCaJ7645223;     IbeGuqsCaJ7645223 = IbeGuqsCaJ93010369;     IbeGuqsCaJ93010369 = IbeGuqsCaJ46091320;     IbeGuqsCaJ46091320 = IbeGuqsCaJ35805839;     IbeGuqsCaJ35805839 = IbeGuqsCaJ97082504;     IbeGuqsCaJ97082504 = IbeGuqsCaJ43084362;     IbeGuqsCaJ43084362 = IbeGuqsCaJ83171370;     IbeGuqsCaJ83171370 = IbeGuqsCaJ92528969;     IbeGuqsCaJ92528969 = IbeGuqsCaJ70294988;     IbeGuqsCaJ70294988 = IbeGuqsCaJ15909234;     IbeGuqsCaJ15909234 = IbeGuqsCaJ40021921;     IbeGuqsCaJ40021921 = IbeGuqsCaJ13022622;     IbeGuqsCaJ13022622 = IbeGuqsCaJ54757075;     IbeGuqsCaJ54757075 = IbeGuqsCaJ89765760;     IbeGuqsCaJ89765760 = IbeGuqsCaJ28682723;     IbeGuqsCaJ28682723 = IbeGuqsCaJ19145426;     IbeGuqsCaJ19145426 = IbeGuqsCaJ87022087;     IbeGuqsCaJ87022087 = IbeGuqsCaJ67663011;     IbeGuqsCaJ67663011 = IbeGuqsCaJ19809902;     IbeGuqsCaJ19809902 = IbeGuqsCaJ63679692;     IbeGuqsCaJ63679692 = IbeGuqsCaJ91409858;     IbeGuqsCaJ91409858 = IbeGuqsCaJ71567900;     IbeGuqsCaJ71567900 = IbeGuqsCaJ25760067;     IbeGuqsCaJ25760067 = IbeGuqsCaJ75660504;     IbeGuqsCaJ75660504 = IbeGuqsCaJ15214826;     IbeGuqsCaJ15214826 = IbeGuqsCaJ41419073;     IbeGuqsCaJ41419073 = IbeGuqsCaJ4799949;     IbeGuqsCaJ4799949 = IbeGuqsCaJ36127993;     IbeGuqsCaJ36127993 = IbeGuqsCaJ8469251;     IbeGuqsCaJ8469251 = IbeGuqsCaJ57087670;     IbeGuqsCaJ57087670 = IbeGuqsCaJ2523674;     IbeGuqsCaJ2523674 = IbeGuqsCaJ61873679;     IbeGuqsCaJ61873679 = IbeGuqsCaJ46258502;     IbeGuqsCaJ46258502 = IbeGuqsCaJ38423260;     IbeGuqsCaJ38423260 = IbeGuqsCaJ14336588;     IbeGuqsCaJ14336588 = IbeGuqsCaJ7599840;     IbeGuqsCaJ7599840 = IbeGuqsCaJ72177567;     IbeGuqsCaJ72177567 = IbeGuqsCaJ54590764;     IbeGuqsCaJ54590764 = IbeGuqsCaJ77099554;     IbeGuqsCaJ77099554 = IbeGuqsCaJ43242268;     IbeGuqsCaJ43242268 = IbeGuqsCaJ49595635;     IbeGuqsCaJ49595635 = IbeGuqsCaJ44097743;     IbeGuqsCaJ44097743 = IbeGuqsCaJ31035626;     IbeGuqsCaJ31035626 = IbeGuqsCaJ74671752;     IbeGuqsCaJ74671752 = IbeGuqsCaJ78402956;     IbeGuqsCaJ78402956 = IbeGuqsCaJ26429866;     IbeGuqsCaJ26429866 = IbeGuqsCaJ41357805;     IbeGuqsCaJ41357805 = IbeGuqsCaJ30633057;     IbeGuqsCaJ30633057 = IbeGuqsCaJ73451284;     IbeGuqsCaJ73451284 = IbeGuqsCaJ77956466;     IbeGuqsCaJ77956466 = IbeGuqsCaJ22384999;     IbeGuqsCaJ22384999 = IbeGuqsCaJ78139382;     IbeGuqsCaJ78139382 = IbeGuqsCaJ13930815;     IbeGuqsCaJ13930815 = IbeGuqsCaJ24929155;     IbeGuqsCaJ24929155 = IbeGuqsCaJ73329322;     IbeGuqsCaJ73329322 = IbeGuqsCaJ36077323;     IbeGuqsCaJ36077323 = IbeGuqsCaJ67250302;     IbeGuqsCaJ67250302 = IbeGuqsCaJ70430816;     IbeGuqsCaJ70430816 = IbeGuqsCaJ20591014;     IbeGuqsCaJ20591014 = IbeGuqsCaJ55663431;     IbeGuqsCaJ55663431 = IbeGuqsCaJ38284413;     IbeGuqsCaJ38284413 = IbeGuqsCaJ47043377;     IbeGuqsCaJ47043377 = IbeGuqsCaJ84059718;     IbeGuqsCaJ84059718 = IbeGuqsCaJ13207318;     IbeGuqsCaJ13207318 = IbeGuqsCaJ13385561;     IbeGuqsCaJ13385561 = IbeGuqsCaJ78148242;     IbeGuqsCaJ78148242 = IbeGuqsCaJ66764119;     IbeGuqsCaJ66764119 = IbeGuqsCaJ16333815;     IbeGuqsCaJ16333815 = IbeGuqsCaJ75429172;     IbeGuqsCaJ75429172 = IbeGuqsCaJ21082884;     IbeGuqsCaJ21082884 = IbeGuqsCaJ46967859;     IbeGuqsCaJ46967859 = IbeGuqsCaJ32431323;     IbeGuqsCaJ32431323 = IbeGuqsCaJ90563456;     IbeGuqsCaJ90563456 = IbeGuqsCaJ76567633;     IbeGuqsCaJ76567633 = IbeGuqsCaJ14084058;     IbeGuqsCaJ14084058 = IbeGuqsCaJ47312115;     IbeGuqsCaJ47312115 = IbeGuqsCaJ40532274;     IbeGuqsCaJ40532274 = IbeGuqsCaJ51088314;     IbeGuqsCaJ51088314 = IbeGuqsCaJ97257548;     IbeGuqsCaJ97257548 = IbeGuqsCaJ88784959;     IbeGuqsCaJ88784959 = IbeGuqsCaJ61269;     IbeGuqsCaJ61269 = IbeGuqsCaJ74166891;     IbeGuqsCaJ74166891 = IbeGuqsCaJ62676709;     IbeGuqsCaJ62676709 = IbeGuqsCaJ30512784;     IbeGuqsCaJ30512784 = IbeGuqsCaJ34702672;     IbeGuqsCaJ34702672 = IbeGuqsCaJ24384291;     IbeGuqsCaJ24384291 = IbeGuqsCaJ47942865;     IbeGuqsCaJ47942865 = IbeGuqsCaJ21329348;     IbeGuqsCaJ21329348 = IbeGuqsCaJ65093937;     IbeGuqsCaJ65093937 = IbeGuqsCaJ78259265;     IbeGuqsCaJ78259265 = IbeGuqsCaJ40349537;     IbeGuqsCaJ40349537 = IbeGuqsCaJ1746751;     IbeGuqsCaJ1746751 = IbeGuqsCaJ33999751;     IbeGuqsCaJ33999751 = IbeGuqsCaJ21436123;     IbeGuqsCaJ21436123 = IbeGuqsCaJ4957855;     IbeGuqsCaJ4957855 = IbeGuqsCaJ2552258;     IbeGuqsCaJ2552258 = IbeGuqsCaJ60038024;     IbeGuqsCaJ60038024 = IbeGuqsCaJ33740716;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void nFRVOwLZcK82447368() {     int ftOxePMbGk56127833 = -438292573;    int ftOxePMbGk72956347 = -274394942;    int ftOxePMbGk93508458 = -136851994;    int ftOxePMbGk22651911 = -350084321;    int ftOxePMbGk91184623 = -215793117;    int ftOxePMbGk60974437 = -253365642;    int ftOxePMbGk27961578 = -245934895;    int ftOxePMbGk72716363 = -29721963;    int ftOxePMbGk40247493 = -994705612;    int ftOxePMbGk74934954 = -879406150;    int ftOxePMbGk30070829 = -585629416;    int ftOxePMbGk26197937 = -31449225;    int ftOxePMbGk82917414 = -232528046;    int ftOxePMbGk82636673 = -476713225;    int ftOxePMbGk49757899 = -449695844;    int ftOxePMbGk99012987 = -576761596;    int ftOxePMbGk39589112 = -927413225;    int ftOxePMbGk34659139 = -840118897;    int ftOxePMbGk60427057 = -429809388;    int ftOxePMbGk65538371 = -324601107;    int ftOxePMbGk68546118 = -402471026;    int ftOxePMbGk83625120 = -641215214;    int ftOxePMbGk66973015 = -197066628;    int ftOxePMbGk17909646 = -131733708;    int ftOxePMbGk7108147 = -121776632;    int ftOxePMbGk14063062 = -880255572;    int ftOxePMbGk22749925 = -788717520;    int ftOxePMbGk21126276 = 99612358;    int ftOxePMbGk48311634 = -846824927;    int ftOxePMbGk13957128 = -937548574;    int ftOxePMbGk80418798 = -320915481;    int ftOxePMbGk66010441 = -864363788;    int ftOxePMbGk38126341 = -302493697;    int ftOxePMbGk61912360 = -364111845;    int ftOxePMbGk38103044 = -814012795;    int ftOxePMbGk87951371 = -327572033;    int ftOxePMbGk31862972 = -765916771;    int ftOxePMbGk67962689 = -547314133;    int ftOxePMbGk55907970 = -575635966;    int ftOxePMbGk92363281 = -927630961;    int ftOxePMbGk83348135 = -289786280;    int ftOxePMbGk65607945 = -763142750;    int ftOxePMbGk48792460 = -395291618;    int ftOxePMbGk49039754 = -592471635;    int ftOxePMbGk12081010 = -692766949;    int ftOxePMbGk39135908 = 33097658;    int ftOxePMbGk86969149 = -48566214;    int ftOxePMbGk23001631 = -946678605;    int ftOxePMbGk20889771 = -683653664;    int ftOxePMbGk38441550 = -827711949;    int ftOxePMbGk39502274 = -230945613;    int ftOxePMbGk47302541 = -706645726;    int ftOxePMbGk56270870 = -222667565;    int ftOxePMbGk82671543 = 24360679;    int ftOxePMbGk13901250 = 30697865;    int ftOxePMbGk72502713 = -797077360;    int ftOxePMbGk5983332 = 22671686;    int ftOxePMbGk75598813 = 94881714;    int ftOxePMbGk15543765 = -128307689;    int ftOxePMbGk77121562 = -335537546;    int ftOxePMbGk38224513 = -464648123;    int ftOxePMbGk6835303 = -245547254;    int ftOxePMbGk24404729 = -182897037;    int ftOxePMbGk26290365 = 42842961;    int ftOxePMbGk94516156 = -458490670;    int ftOxePMbGk64060387 = -721265628;    int ftOxePMbGk88071596 = -728955529;    int ftOxePMbGk21005054 = -868416201;    int ftOxePMbGk44533630 = -662700430;    int ftOxePMbGk61806527 = -22123812;    int ftOxePMbGk67150015 = -810844825;    int ftOxePMbGk71626423 = -280099092;    int ftOxePMbGk78751168 = -164482931;    int ftOxePMbGk68063775 = -502178428;    int ftOxePMbGk82190235 = 65185172;    int ftOxePMbGk2938173 = -639328277;    int ftOxePMbGk34832660 = -145923597;    int ftOxePMbGk17933262 = -604594994;    int ftOxePMbGk5828636 = -438966760;    int ftOxePMbGk67972239 = -54874291;    int ftOxePMbGk27093912 = -731689358;    int ftOxePMbGk99748294 = -842038915;    int ftOxePMbGk236505 = -216733979;    int ftOxePMbGk9870084 = 80887022;    int ftOxePMbGk74454854 = -606602961;    int ftOxePMbGk33116257 = -614269755;    int ftOxePMbGk9739572 = -541696224;    int ftOxePMbGk55454797 = -226854376;    int ftOxePMbGk48011111 = -294809711;    int ftOxePMbGk65600330 = 83064564;    int ftOxePMbGk81968039 = -250243719;    int ftOxePMbGk56264158 = -760798486;    int ftOxePMbGk52418925 = -319006444;    int ftOxePMbGk78786408 = -140098421;    int ftOxePMbGk54138769 = -362982839;    int ftOxePMbGk76512833 = 55760973;    int ftOxePMbGk41203216 = -480245714;    int ftOxePMbGk22502096 = -338134580;    int ftOxePMbGk54523597 = -33980966;    int ftOxePMbGk48020622 = -438292573;     ftOxePMbGk56127833 = ftOxePMbGk72956347;     ftOxePMbGk72956347 = ftOxePMbGk93508458;     ftOxePMbGk93508458 = ftOxePMbGk22651911;     ftOxePMbGk22651911 = ftOxePMbGk91184623;     ftOxePMbGk91184623 = ftOxePMbGk60974437;     ftOxePMbGk60974437 = ftOxePMbGk27961578;     ftOxePMbGk27961578 = ftOxePMbGk72716363;     ftOxePMbGk72716363 = ftOxePMbGk40247493;     ftOxePMbGk40247493 = ftOxePMbGk74934954;     ftOxePMbGk74934954 = ftOxePMbGk30070829;     ftOxePMbGk30070829 = ftOxePMbGk26197937;     ftOxePMbGk26197937 = ftOxePMbGk82917414;     ftOxePMbGk82917414 = ftOxePMbGk82636673;     ftOxePMbGk82636673 = ftOxePMbGk49757899;     ftOxePMbGk49757899 = ftOxePMbGk99012987;     ftOxePMbGk99012987 = ftOxePMbGk39589112;     ftOxePMbGk39589112 = ftOxePMbGk34659139;     ftOxePMbGk34659139 = ftOxePMbGk60427057;     ftOxePMbGk60427057 = ftOxePMbGk65538371;     ftOxePMbGk65538371 = ftOxePMbGk68546118;     ftOxePMbGk68546118 = ftOxePMbGk83625120;     ftOxePMbGk83625120 = ftOxePMbGk66973015;     ftOxePMbGk66973015 = ftOxePMbGk17909646;     ftOxePMbGk17909646 = ftOxePMbGk7108147;     ftOxePMbGk7108147 = ftOxePMbGk14063062;     ftOxePMbGk14063062 = ftOxePMbGk22749925;     ftOxePMbGk22749925 = ftOxePMbGk21126276;     ftOxePMbGk21126276 = ftOxePMbGk48311634;     ftOxePMbGk48311634 = ftOxePMbGk13957128;     ftOxePMbGk13957128 = ftOxePMbGk80418798;     ftOxePMbGk80418798 = ftOxePMbGk66010441;     ftOxePMbGk66010441 = ftOxePMbGk38126341;     ftOxePMbGk38126341 = ftOxePMbGk61912360;     ftOxePMbGk61912360 = ftOxePMbGk38103044;     ftOxePMbGk38103044 = ftOxePMbGk87951371;     ftOxePMbGk87951371 = ftOxePMbGk31862972;     ftOxePMbGk31862972 = ftOxePMbGk67962689;     ftOxePMbGk67962689 = ftOxePMbGk55907970;     ftOxePMbGk55907970 = ftOxePMbGk92363281;     ftOxePMbGk92363281 = ftOxePMbGk83348135;     ftOxePMbGk83348135 = ftOxePMbGk65607945;     ftOxePMbGk65607945 = ftOxePMbGk48792460;     ftOxePMbGk48792460 = ftOxePMbGk49039754;     ftOxePMbGk49039754 = ftOxePMbGk12081010;     ftOxePMbGk12081010 = ftOxePMbGk39135908;     ftOxePMbGk39135908 = ftOxePMbGk86969149;     ftOxePMbGk86969149 = ftOxePMbGk23001631;     ftOxePMbGk23001631 = ftOxePMbGk20889771;     ftOxePMbGk20889771 = ftOxePMbGk38441550;     ftOxePMbGk38441550 = ftOxePMbGk39502274;     ftOxePMbGk39502274 = ftOxePMbGk47302541;     ftOxePMbGk47302541 = ftOxePMbGk56270870;     ftOxePMbGk56270870 = ftOxePMbGk82671543;     ftOxePMbGk82671543 = ftOxePMbGk13901250;     ftOxePMbGk13901250 = ftOxePMbGk72502713;     ftOxePMbGk72502713 = ftOxePMbGk5983332;     ftOxePMbGk5983332 = ftOxePMbGk75598813;     ftOxePMbGk75598813 = ftOxePMbGk15543765;     ftOxePMbGk15543765 = ftOxePMbGk77121562;     ftOxePMbGk77121562 = ftOxePMbGk38224513;     ftOxePMbGk38224513 = ftOxePMbGk6835303;     ftOxePMbGk6835303 = ftOxePMbGk24404729;     ftOxePMbGk24404729 = ftOxePMbGk26290365;     ftOxePMbGk26290365 = ftOxePMbGk94516156;     ftOxePMbGk94516156 = ftOxePMbGk64060387;     ftOxePMbGk64060387 = ftOxePMbGk88071596;     ftOxePMbGk88071596 = ftOxePMbGk21005054;     ftOxePMbGk21005054 = ftOxePMbGk44533630;     ftOxePMbGk44533630 = ftOxePMbGk61806527;     ftOxePMbGk61806527 = ftOxePMbGk67150015;     ftOxePMbGk67150015 = ftOxePMbGk71626423;     ftOxePMbGk71626423 = ftOxePMbGk78751168;     ftOxePMbGk78751168 = ftOxePMbGk68063775;     ftOxePMbGk68063775 = ftOxePMbGk82190235;     ftOxePMbGk82190235 = ftOxePMbGk2938173;     ftOxePMbGk2938173 = ftOxePMbGk34832660;     ftOxePMbGk34832660 = ftOxePMbGk17933262;     ftOxePMbGk17933262 = ftOxePMbGk5828636;     ftOxePMbGk5828636 = ftOxePMbGk67972239;     ftOxePMbGk67972239 = ftOxePMbGk27093912;     ftOxePMbGk27093912 = ftOxePMbGk99748294;     ftOxePMbGk99748294 = ftOxePMbGk236505;     ftOxePMbGk236505 = ftOxePMbGk9870084;     ftOxePMbGk9870084 = ftOxePMbGk74454854;     ftOxePMbGk74454854 = ftOxePMbGk33116257;     ftOxePMbGk33116257 = ftOxePMbGk9739572;     ftOxePMbGk9739572 = ftOxePMbGk55454797;     ftOxePMbGk55454797 = ftOxePMbGk48011111;     ftOxePMbGk48011111 = ftOxePMbGk65600330;     ftOxePMbGk65600330 = ftOxePMbGk81968039;     ftOxePMbGk81968039 = ftOxePMbGk56264158;     ftOxePMbGk56264158 = ftOxePMbGk52418925;     ftOxePMbGk52418925 = ftOxePMbGk78786408;     ftOxePMbGk78786408 = ftOxePMbGk54138769;     ftOxePMbGk54138769 = ftOxePMbGk76512833;     ftOxePMbGk76512833 = ftOxePMbGk41203216;     ftOxePMbGk41203216 = ftOxePMbGk22502096;     ftOxePMbGk22502096 = ftOxePMbGk54523597;     ftOxePMbGk54523597 = ftOxePMbGk48020622;     ftOxePMbGk48020622 = ftOxePMbGk56127833;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void wFecskymgc19092343() {     int vsfhBppZYO78514950 = -72696537;    int vsfhBppZYO57303848 = -222937996;    int vsfhBppZYO22277738 = -376505838;    int vsfhBppZYO37658600 = -356491852;    int vsfhBppZYO89358877 = -806850119;    int vsfhBppZYO75857554 = -113974020;    int vsfhBppZYO20117317 = -67922202;    int vsfhBppZYO48350222 = -500318607;    int vsfhBppZYO37410624 = -878901651;    int vsfhBppZYO66698539 = -622147483;    int vsfhBppZYO67612688 = -582253817;    int vsfhBppZYO82100886 = -282194518;    int vsfhBppZYO49925595 = -851245365;    int vsfhBppZYO25251426 = 95578018;    int vsfhBppZYO86493176 = -77173557;    int vsfhBppZYO43268900 = -958429139;    int vsfhBppZYO89412463 = -104693044;    int vsfhBppZYO40635555 = -191861587;    int vsfhBppZYO1708688 = -564911513;    int vsfhBppZYO44054654 = -577235561;    int vsfhBppZYO69429225 = -393958335;    int vsfhBppZYO47440338 = -45981528;    int vsfhBppZYO70266338 = -373147220;    int vsfhBppZYO44409432 = -695726566;    int vsfhBppZYO42648394 = -843335757;    int vsfhBppZYO2366056 = -224149796;    int vsfhBppZYO69839345 = -546304013;    int vsfhBppZYO27037726 = -788842096;    int vsfhBppZYO55204194 = -91082046;    int vsfhBppZYO23114308 = -857844440;    int vsfhBppZYO24709604 = -402231633;    int vsfhBppZYO23551633 = -416435143;    int vsfhBppZYO19165012 = -831837191;    int vsfhBppZYO21301048 = -688238695;    int vsfhBppZYO14332409 = -877769218;    int vsfhBppZYO29644241 = -642967409;    int vsfhBppZYO25302684 = -498864723;    int vsfhBppZYO21588790 = -414939605;    int vsfhBppZYO4216102 = -976681998;    int vsfhBppZYO12548996 = 36145010;    int vsfhBppZYO12105506 = -792963946;    int vsfhBppZYO54116337 = -521051441;    int vsfhBppZYO54342652 = -282822202;    int vsfhBppZYO48483873 = -589835284;    int vsfhBppZYO80064276 = -501910427;    int vsfhBppZYO47236190 = -149131437;    int vsfhBppZYO99266547 = -626976467;    int vsfhBppZYO67600305 = -703213596;    int vsfhBppZYO15349676 = -633370362;    int vsfhBppZYO35525296 = -969732208;    int vsfhBppZYO48371490 = -586021890;    int vsfhBppZYO21153799 = -37465008;    int vsfhBppZYO34585273 = -994578795;    int vsfhBppZYO42958089 = -251320130;    int vsfhBppZYO49663117 = -110006250;    int vsfhBppZYO31074612 = 73284991;    int vsfhBppZYO87037509 = -849790777;    int vsfhBppZYO77868305 = -680779273;    int vsfhBppZYO95010205 = -513156096;    int vsfhBppZYO86992821 = -482700323;    int vsfhBppZYO6018210 = -567670008;    int vsfhBppZYO93079591 = -279080106;    int vsfhBppZYO93146027 = -309236562;    int vsfhBppZYO14296317 = 78942788;    int vsfhBppZYO41988936 = -119915851;    int vsfhBppZYO44061056 = -65818675;    int vsfhBppZYO62935874 = -450357328;    int vsfhBppZYO28624547 = -63006671;    int vsfhBppZYO10919018 = -26652764;    int vsfhBppZYO56848935 = -434206149;    int vsfhBppZYO17966216 = -359564416;    int vsfhBppZYO67823674 = -689753440;    int vsfhBppZYO36419453 = -215179590;    int vsfhBppZYO89159691 = -501056524;    int vsfhBppZYO31949148 = -784271615;    int vsfhBppZYO15312889 = -872906894;    int vsfhBppZYO93097686 = -763159326;    int vsfhBppZYO21782466 = -783311937;    int vsfhBppZYO64345156 = -93816140;    int vsfhBppZYO95412204 = -594204320;    int vsfhBppZYO3099509 = -597173329;    int vsfhBppZYO2239040 = -843090417;    int vsfhBppZYO11688050 = -55471734;    int vsfhBppZYO19678899 = -121349838;    int vsfhBppZYO74742817 = -171822551;    int vsfhBppZYO3555805 = -264766625;    int vsfhBppZYO88966359 = -421856348;    int vsfhBppZYO76206922 = -480517062;    int vsfhBppZYO71637930 = -478232446;    int vsfhBppZYO83257796 = -851054209;    int vsfhBppZYO42606731 = -793176633;    int vsfhBppZYO47434379 = -818085451;    int vsfhBppZYO26578584 = -901783509;    int vsfhBppZYO17223280 = -393981675;    int vsfhBppZYO6530787 = -396184983;    int vsfhBppZYO19025915 = -413883840;    int vsfhBppZYO60970310 = -111814880;    int vsfhBppZYO40046336 = -261764991;    int vsfhBppZYO6494938 = -369919434;    int vsfhBppZYO36003220 = -72696537;     vsfhBppZYO78514950 = vsfhBppZYO57303848;     vsfhBppZYO57303848 = vsfhBppZYO22277738;     vsfhBppZYO22277738 = vsfhBppZYO37658600;     vsfhBppZYO37658600 = vsfhBppZYO89358877;     vsfhBppZYO89358877 = vsfhBppZYO75857554;     vsfhBppZYO75857554 = vsfhBppZYO20117317;     vsfhBppZYO20117317 = vsfhBppZYO48350222;     vsfhBppZYO48350222 = vsfhBppZYO37410624;     vsfhBppZYO37410624 = vsfhBppZYO66698539;     vsfhBppZYO66698539 = vsfhBppZYO67612688;     vsfhBppZYO67612688 = vsfhBppZYO82100886;     vsfhBppZYO82100886 = vsfhBppZYO49925595;     vsfhBppZYO49925595 = vsfhBppZYO25251426;     vsfhBppZYO25251426 = vsfhBppZYO86493176;     vsfhBppZYO86493176 = vsfhBppZYO43268900;     vsfhBppZYO43268900 = vsfhBppZYO89412463;     vsfhBppZYO89412463 = vsfhBppZYO40635555;     vsfhBppZYO40635555 = vsfhBppZYO1708688;     vsfhBppZYO1708688 = vsfhBppZYO44054654;     vsfhBppZYO44054654 = vsfhBppZYO69429225;     vsfhBppZYO69429225 = vsfhBppZYO47440338;     vsfhBppZYO47440338 = vsfhBppZYO70266338;     vsfhBppZYO70266338 = vsfhBppZYO44409432;     vsfhBppZYO44409432 = vsfhBppZYO42648394;     vsfhBppZYO42648394 = vsfhBppZYO2366056;     vsfhBppZYO2366056 = vsfhBppZYO69839345;     vsfhBppZYO69839345 = vsfhBppZYO27037726;     vsfhBppZYO27037726 = vsfhBppZYO55204194;     vsfhBppZYO55204194 = vsfhBppZYO23114308;     vsfhBppZYO23114308 = vsfhBppZYO24709604;     vsfhBppZYO24709604 = vsfhBppZYO23551633;     vsfhBppZYO23551633 = vsfhBppZYO19165012;     vsfhBppZYO19165012 = vsfhBppZYO21301048;     vsfhBppZYO21301048 = vsfhBppZYO14332409;     vsfhBppZYO14332409 = vsfhBppZYO29644241;     vsfhBppZYO29644241 = vsfhBppZYO25302684;     vsfhBppZYO25302684 = vsfhBppZYO21588790;     vsfhBppZYO21588790 = vsfhBppZYO4216102;     vsfhBppZYO4216102 = vsfhBppZYO12548996;     vsfhBppZYO12548996 = vsfhBppZYO12105506;     vsfhBppZYO12105506 = vsfhBppZYO54116337;     vsfhBppZYO54116337 = vsfhBppZYO54342652;     vsfhBppZYO54342652 = vsfhBppZYO48483873;     vsfhBppZYO48483873 = vsfhBppZYO80064276;     vsfhBppZYO80064276 = vsfhBppZYO47236190;     vsfhBppZYO47236190 = vsfhBppZYO99266547;     vsfhBppZYO99266547 = vsfhBppZYO67600305;     vsfhBppZYO67600305 = vsfhBppZYO15349676;     vsfhBppZYO15349676 = vsfhBppZYO35525296;     vsfhBppZYO35525296 = vsfhBppZYO48371490;     vsfhBppZYO48371490 = vsfhBppZYO21153799;     vsfhBppZYO21153799 = vsfhBppZYO34585273;     vsfhBppZYO34585273 = vsfhBppZYO42958089;     vsfhBppZYO42958089 = vsfhBppZYO49663117;     vsfhBppZYO49663117 = vsfhBppZYO31074612;     vsfhBppZYO31074612 = vsfhBppZYO87037509;     vsfhBppZYO87037509 = vsfhBppZYO77868305;     vsfhBppZYO77868305 = vsfhBppZYO95010205;     vsfhBppZYO95010205 = vsfhBppZYO86992821;     vsfhBppZYO86992821 = vsfhBppZYO6018210;     vsfhBppZYO6018210 = vsfhBppZYO93079591;     vsfhBppZYO93079591 = vsfhBppZYO93146027;     vsfhBppZYO93146027 = vsfhBppZYO14296317;     vsfhBppZYO14296317 = vsfhBppZYO41988936;     vsfhBppZYO41988936 = vsfhBppZYO44061056;     vsfhBppZYO44061056 = vsfhBppZYO62935874;     vsfhBppZYO62935874 = vsfhBppZYO28624547;     vsfhBppZYO28624547 = vsfhBppZYO10919018;     vsfhBppZYO10919018 = vsfhBppZYO56848935;     vsfhBppZYO56848935 = vsfhBppZYO17966216;     vsfhBppZYO17966216 = vsfhBppZYO67823674;     vsfhBppZYO67823674 = vsfhBppZYO36419453;     vsfhBppZYO36419453 = vsfhBppZYO89159691;     vsfhBppZYO89159691 = vsfhBppZYO31949148;     vsfhBppZYO31949148 = vsfhBppZYO15312889;     vsfhBppZYO15312889 = vsfhBppZYO93097686;     vsfhBppZYO93097686 = vsfhBppZYO21782466;     vsfhBppZYO21782466 = vsfhBppZYO64345156;     vsfhBppZYO64345156 = vsfhBppZYO95412204;     vsfhBppZYO95412204 = vsfhBppZYO3099509;     vsfhBppZYO3099509 = vsfhBppZYO2239040;     vsfhBppZYO2239040 = vsfhBppZYO11688050;     vsfhBppZYO11688050 = vsfhBppZYO19678899;     vsfhBppZYO19678899 = vsfhBppZYO74742817;     vsfhBppZYO74742817 = vsfhBppZYO3555805;     vsfhBppZYO3555805 = vsfhBppZYO88966359;     vsfhBppZYO88966359 = vsfhBppZYO76206922;     vsfhBppZYO76206922 = vsfhBppZYO71637930;     vsfhBppZYO71637930 = vsfhBppZYO83257796;     vsfhBppZYO83257796 = vsfhBppZYO42606731;     vsfhBppZYO42606731 = vsfhBppZYO47434379;     vsfhBppZYO47434379 = vsfhBppZYO26578584;     vsfhBppZYO26578584 = vsfhBppZYO17223280;     vsfhBppZYO17223280 = vsfhBppZYO6530787;     vsfhBppZYO6530787 = vsfhBppZYO19025915;     vsfhBppZYO19025915 = vsfhBppZYO60970310;     vsfhBppZYO60970310 = vsfhBppZYO40046336;     vsfhBppZYO40046336 = vsfhBppZYO6494938;     vsfhBppZYO6494938 = vsfhBppZYO36003220;     vsfhBppZYO36003220 = vsfhBppZYO78514950;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void wASbPuOtuP55737317() {     int UJdwBcXqkB902069 = -807100498;    int UJdwBcXqkB41651348 = -171481045;    int UJdwBcXqkB51047016 = -616159682;    int UJdwBcXqkB52665288 = -362899387;    int UJdwBcXqkB87533131 = -297907112;    int UJdwBcXqkB90740671 = 25417602;    int UJdwBcXqkB12273056 = -989909508;    int UJdwBcXqkB23984080 = -970915236;    int UJdwBcXqkB34573755 = -763097689;    int UJdwBcXqkB58462124 = -364888816;    int UJdwBcXqkB5154549 = -578878217;    int UJdwBcXqkB38003835 = -532939803;    int UJdwBcXqkB16933775 = -369962685;    int UJdwBcXqkB67866178 = -432130738;    int UJdwBcXqkB23228453 = -804651252;    int UJdwBcXqkB87524812 = -240096681;    int UJdwBcXqkB39235815 = -381972864;    int UJdwBcXqkB46611971 = -643604274;    int UJdwBcXqkB42990318 = -700013639;    int UJdwBcXqkB22570938 = -829870014;    int UJdwBcXqkB70312332 = -385445638;    int UJdwBcXqkB11255557 = -550747841;    int UJdwBcXqkB73559661 = -549227812;    int UJdwBcXqkB70909219 = -159719423;    int UJdwBcXqkB78188640 = -464894869;    int UJdwBcXqkB90669049 = -668044024;    int UJdwBcXqkB16928766 = -303890505;    int UJdwBcXqkB32949176 = -577296543;    int UJdwBcXqkB62096755 = -435339180;    int UJdwBcXqkB32271487 = -778140307;    int UJdwBcXqkB69000408 = -483547785;    int UJdwBcXqkB81092823 = 31493500;    int UJdwBcXqkB203682 = -261180689;    int UJdwBcXqkB80689735 = 87634455;    int UJdwBcXqkB90561773 = -941525644;    int UJdwBcXqkB71337109 = -958362804;    int UJdwBcXqkB18742397 = -231812674;    int UJdwBcXqkB75214890 = -282565073;    int UJdwBcXqkB52524233 = -277728042;    int UJdwBcXqkB32734711 = -100079019;    int UJdwBcXqkB40862877 = -196141612;    int UJdwBcXqkB42624728 = -278960153;    int UJdwBcXqkB59892844 = -170352788;    int UJdwBcXqkB47927992 = -587198933;    int UJdwBcXqkB48047543 = -311053905;    int UJdwBcXqkB55336471 = -331360551;    int UJdwBcXqkB11563945 = -105386720;    int UJdwBcXqkB12198980 = -459748588;    int UJdwBcXqkB9809582 = -583087085;    int UJdwBcXqkB32609042 = -11752462;    int UJdwBcXqkB57240707 = -941098167;    int UJdwBcXqkB95005055 = -468284294;    int UJdwBcXqkB12899677 = -666490022;    int UJdwBcXqkB3244635 = -527000939;    int UJdwBcXqkB85424985 = -250710370;    int UJdwBcXqkB89646511 = -156352657;    int UJdwBcXqkB68091687 = -622253233;    int UJdwBcXqkB80137796 = -356440260;    int UJdwBcXqkB74476647 = -898004518;    int UJdwBcXqkB96864081 = -629863089;    int UJdwBcXqkB73811906 = -670691893;    int UJdwBcXqkB79323879 = -312612965;    int UJdwBcXqkB61887325 = -435576056;    int UJdwBcXqkB2302268 = -984957383;    int UJdwBcXqkB89461715 = -881341031;    int UJdwBcXqkB24061725 = -510371717;    int UJdwBcXqkB37800153 = -171759114;    int UJdwBcXqkB36244040 = -357597140;    int UJdwBcXqkB77304405 = -490605095;    int UJdwBcXqkB51891343 = -846288448;    int UJdwBcXqkB68782416 = 91715993;    int UJdwBcXqkB64020924 = 592209;    int UJdwBcXqkB94087738 = -265876232;    int UJdwBcXqkB10255607 = -499934621;    int UJdwBcXqkB81708060 = -533728403;    int UJdwBcXqkB27687605 = -6485486;    int UJdwBcXqkB51362713 = -280395054;    int UJdwBcXqkB25631669 = -962028880;    int UJdwBcXqkB22861677 = -848665518;    int UJdwBcXqkB22852169 = -33534318;    int UJdwBcXqkB79105104 = -462657304;    int UJdwBcXqkB4729786 = -844141918;    int UJdwBcXqkB23139595 = -994209459;    int UJdwBcXqkB29487714 = -323586719;    int UJdwBcXqkB75030780 = -837042141;    int UJdwBcXqkB73995353 = 84736509;    int UJdwBcXqkB68193146 = -302016479;    int UJdwBcXqkB96959047 = -734179751;    int UJdwBcXqkB95264750 = -661655176;    int UJdwBcXqkB915262 = -685172987;    int UJdwBcXqkB3245423 = -236109571;    int UJdwBcXqkB38604600 = -875372415;    int UJdwBcXqkB738244 = -384560556;    int UJdwBcXqkB55660151 = -647864954;    int UJdwBcXqkB58922805 = -429387127;    int UJdwBcXqkB61538997 = -883528648;    int UJdwBcXqkB80737403 = -843384097;    int UJdwBcXqkB57590576 = -185395405;    int UJdwBcXqkB58466277 = -705857902;    int UJdwBcXqkB23985818 = -807100498;     UJdwBcXqkB902069 = UJdwBcXqkB41651348;     UJdwBcXqkB41651348 = UJdwBcXqkB51047016;     UJdwBcXqkB51047016 = UJdwBcXqkB52665288;     UJdwBcXqkB52665288 = UJdwBcXqkB87533131;     UJdwBcXqkB87533131 = UJdwBcXqkB90740671;     UJdwBcXqkB90740671 = UJdwBcXqkB12273056;     UJdwBcXqkB12273056 = UJdwBcXqkB23984080;     UJdwBcXqkB23984080 = UJdwBcXqkB34573755;     UJdwBcXqkB34573755 = UJdwBcXqkB58462124;     UJdwBcXqkB58462124 = UJdwBcXqkB5154549;     UJdwBcXqkB5154549 = UJdwBcXqkB38003835;     UJdwBcXqkB38003835 = UJdwBcXqkB16933775;     UJdwBcXqkB16933775 = UJdwBcXqkB67866178;     UJdwBcXqkB67866178 = UJdwBcXqkB23228453;     UJdwBcXqkB23228453 = UJdwBcXqkB87524812;     UJdwBcXqkB87524812 = UJdwBcXqkB39235815;     UJdwBcXqkB39235815 = UJdwBcXqkB46611971;     UJdwBcXqkB46611971 = UJdwBcXqkB42990318;     UJdwBcXqkB42990318 = UJdwBcXqkB22570938;     UJdwBcXqkB22570938 = UJdwBcXqkB70312332;     UJdwBcXqkB70312332 = UJdwBcXqkB11255557;     UJdwBcXqkB11255557 = UJdwBcXqkB73559661;     UJdwBcXqkB73559661 = UJdwBcXqkB70909219;     UJdwBcXqkB70909219 = UJdwBcXqkB78188640;     UJdwBcXqkB78188640 = UJdwBcXqkB90669049;     UJdwBcXqkB90669049 = UJdwBcXqkB16928766;     UJdwBcXqkB16928766 = UJdwBcXqkB32949176;     UJdwBcXqkB32949176 = UJdwBcXqkB62096755;     UJdwBcXqkB62096755 = UJdwBcXqkB32271487;     UJdwBcXqkB32271487 = UJdwBcXqkB69000408;     UJdwBcXqkB69000408 = UJdwBcXqkB81092823;     UJdwBcXqkB81092823 = UJdwBcXqkB203682;     UJdwBcXqkB203682 = UJdwBcXqkB80689735;     UJdwBcXqkB80689735 = UJdwBcXqkB90561773;     UJdwBcXqkB90561773 = UJdwBcXqkB71337109;     UJdwBcXqkB71337109 = UJdwBcXqkB18742397;     UJdwBcXqkB18742397 = UJdwBcXqkB75214890;     UJdwBcXqkB75214890 = UJdwBcXqkB52524233;     UJdwBcXqkB52524233 = UJdwBcXqkB32734711;     UJdwBcXqkB32734711 = UJdwBcXqkB40862877;     UJdwBcXqkB40862877 = UJdwBcXqkB42624728;     UJdwBcXqkB42624728 = UJdwBcXqkB59892844;     UJdwBcXqkB59892844 = UJdwBcXqkB47927992;     UJdwBcXqkB47927992 = UJdwBcXqkB48047543;     UJdwBcXqkB48047543 = UJdwBcXqkB55336471;     UJdwBcXqkB55336471 = UJdwBcXqkB11563945;     UJdwBcXqkB11563945 = UJdwBcXqkB12198980;     UJdwBcXqkB12198980 = UJdwBcXqkB9809582;     UJdwBcXqkB9809582 = UJdwBcXqkB32609042;     UJdwBcXqkB32609042 = UJdwBcXqkB57240707;     UJdwBcXqkB57240707 = UJdwBcXqkB95005055;     UJdwBcXqkB95005055 = UJdwBcXqkB12899677;     UJdwBcXqkB12899677 = UJdwBcXqkB3244635;     UJdwBcXqkB3244635 = UJdwBcXqkB85424985;     UJdwBcXqkB85424985 = UJdwBcXqkB89646511;     UJdwBcXqkB89646511 = UJdwBcXqkB68091687;     UJdwBcXqkB68091687 = UJdwBcXqkB80137796;     UJdwBcXqkB80137796 = UJdwBcXqkB74476647;     UJdwBcXqkB74476647 = UJdwBcXqkB96864081;     UJdwBcXqkB96864081 = UJdwBcXqkB73811906;     UJdwBcXqkB73811906 = UJdwBcXqkB79323879;     UJdwBcXqkB79323879 = UJdwBcXqkB61887325;     UJdwBcXqkB61887325 = UJdwBcXqkB2302268;     UJdwBcXqkB2302268 = UJdwBcXqkB89461715;     UJdwBcXqkB89461715 = UJdwBcXqkB24061725;     UJdwBcXqkB24061725 = UJdwBcXqkB37800153;     UJdwBcXqkB37800153 = UJdwBcXqkB36244040;     UJdwBcXqkB36244040 = UJdwBcXqkB77304405;     UJdwBcXqkB77304405 = UJdwBcXqkB51891343;     UJdwBcXqkB51891343 = UJdwBcXqkB68782416;     UJdwBcXqkB68782416 = UJdwBcXqkB64020924;     UJdwBcXqkB64020924 = UJdwBcXqkB94087738;     UJdwBcXqkB94087738 = UJdwBcXqkB10255607;     UJdwBcXqkB10255607 = UJdwBcXqkB81708060;     UJdwBcXqkB81708060 = UJdwBcXqkB27687605;     UJdwBcXqkB27687605 = UJdwBcXqkB51362713;     UJdwBcXqkB51362713 = UJdwBcXqkB25631669;     UJdwBcXqkB25631669 = UJdwBcXqkB22861677;     UJdwBcXqkB22861677 = UJdwBcXqkB22852169;     UJdwBcXqkB22852169 = UJdwBcXqkB79105104;     UJdwBcXqkB79105104 = UJdwBcXqkB4729786;     UJdwBcXqkB4729786 = UJdwBcXqkB23139595;     UJdwBcXqkB23139595 = UJdwBcXqkB29487714;     UJdwBcXqkB29487714 = UJdwBcXqkB75030780;     UJdwBcXqkB75030780 = UJdwBcXqkB73995353;     UJdwBcXqkB73995353 = UJdwBcXqkB68193146;     UJdwBcXqkB68193146 = UJdwBcXqkB96959047;     UJdwBcXqkB96959047 = UJdwBcXqkB95264750;     UJdwBcXqkB95264750 = UJdwBcXqkB915262;     UJdwBcXqkB915262 = UJdwBcXqkB3245423;     UJdwBcXqkB3245423 = UJdwBcXqkB38604600;     UJdwBcXqkB38604600 = UJdwBcXqkB738244;     UJdwBcXqkB738244 = UJdwBcXqkB55660151;     UJdwBcXqkB55660151 = UJdwBcXqkB58922805;     UJdwBcXqkB58922805 = UJdwBcXqkB61538997;     UJdwBcXqkB61538997 = UJdwBcXqkB80737403;     UJdwBcXqkB80737403 = UJdwBcXqkB57590576;     UJdwBcXqkB57590576 = UJdwBcXqkB58466277;     UJdwBcXqkB58466277 = UJdwBcXqkB23985818;     UJdwBcXqkB23985818 = UJdwBcXqkB902069;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void eNckAzNmrY92382292() {     int AKSFlBssnb23289186 = -441504463;    int AKSFlBssnb25998849 = -120024106;    int AKSFlBssnb79816295 = -855813526;    int AKSFlBssnb67671976 = -369306916;    int AKSFlBssnb85707386 = -888964121;    int AKSFlBssnb5623789 = -935190775;    int AKSFlBssnb4428795 = -811896814;    int AKSFlBssnb99617938 = -341511896;    int AKSFlBssnb31736886 = -647293730;    int AKSFlBssnb50225709 = -107630149;    int AKSFlBssnb42696408 = -575502618;    int AKSFlBssnb93906783 = -783685105;    int AKSFlBssnb83941955 = -988680005;    int AKSFlBssnb10480931 = -959839496;    int AKSFlBssnb59963730 = -432128983;    int AKSFlBssnb31780725 = -621764223;    int AKSFlBssnb89059167 = -659252684;    int AKSFlBssnb52588388 = 4653031;    int AKSFlBssnb84271949 = -835115763;    int AKSFlBssnb1087221 = 17495532;    int AKSFlBssnb71195439 = -376932952;    int AKSFlBssnb75070775 = 44485846;    int AKSFlBssnb76852984 = -725308404;    int AKSFlBssnb97409006 = -723712280;    int AKSFlBssnb13728888 = -86454006;    int AKSFlBssnb78972045 = -11938244;    int AKSFlBssnb64018186 = -61476998;    int AKSFlBssnb38860626 = -365751003;    int AKSFlBssnb68989316 = -779596284;    int AKSFlBssnb41428667 = -698436173;    int AKSFlBssnb13291214 = -564863937;    int AKSFlBssnb38634014 = -620577852;    int AKSFlBssnb81242352 = -790524181;    int AKSFlBssnb40078422 = -236492395;    int AKSFlBssnb66791137 = 94717937;    int AKSFlBssnb13029979 = -173758162;    int AKSFlBssnb12182109 = 35239374;    int AKSFlBssnb28840992 = -150190548;    int AKSFlBssnb832365 = -678774061;    int AKSFlBssnb52920425 = -236303048;    int AKSFlBssnb69620248 = -699319278;    int AKSFlBssnb31133119 = -36868823;    int AKSFlBssnb65443036 = -57883369;    int AKSFlBssnb47372112 = -584562582;    int AKSFlBssnb16030809 = -120197382;    int AKSFlBssnb63436753 = -513589628;    int AKSFlBssnb23861342 = -683796973;    int AKSFlBssnb56797654 = -216283578;    int AKSFlBssnb4269487 = -532803759;    int AKSFlBssnb29692787 = -153772726;    int AKSFlBssnb66109923 = -196174444;    int AKSFlBssnb68856313 = -899103572;    int AKSFlBssnb91214079 = -338401255;    int AKSFlBssnb63531179 = -802681748;    int AKSFlBssnb21186853 = -391414480;    int AKSFlBssnb48218410 = -385990309;    int AKSFlBssnb49145864 = -394715702;    int AKSFlBssnb82407288 = -32101247;    int AKSFlBssnb53943089 = -182852910;    int AKSFlBssnb6735341 = -777025878;    int AKSFlBssnb41605602 = -773713778;    int AKSFlBssnb65568168 = -346145811;    int AKSFlBssnb30628623 = -561915612;    int AKSFlBssnb90308219 = -948857557;    int AKSFlBssnb36934495 = -542766212;    int AKSFlBssnb4062394 = -954924767;    int AKSFlBssnb12664432 = -993160924;    int AKSFlBssnb43863533 = -652187610;    int AKSFlBssnb43689793 = -954557433;    int AKSFlBssnb46933751 = -158370822;    int AKSFlBssnb19598617 = -557003598;    int AKSFlBssnb60218175 = -409062136;    int AKSFlBssnb51756023 = -316572908;    int AKSFlBssnb31351525 = -498812715;    int AKSFlBssnb31466973 = -283185190;    int AKSFlBssnb40062320 = -240064129;    int AKSFlBssnb9627740 = -897630785;    int AKSFlBssnb29480873 = -40745823;    int AKSFlBssnb81378197 = -503514899;    int AKSFlBssnb50292134 = -572864378;    int AKSFlBssnb55110703 = -328141271;    int AKSFlBssnb7220532 = -845193421;    int AKSFlBssnb34591140 = -832947245;    int AKSFlBssnb39296529 = -525823558;    int AKSFlBssnb75318743 = -402261730;    int AKSFlBssnb44434901 = -665760365;    int AKSFlBssnb47419935 = -182176597;    int AKSFlBssnb17711173 = -987842434;    int AKSFlBssnb18891570 = -845077916;    int AKSFlBssnb18572728 = -519291755;    int AKSFlBssnb63884115 = -779042461;    int AKSFlBssnb29774821 = -932659380;    int AKSFlBssnb74897902 = -967337639;    int AKSFlBssnb94097023 = -901748184;    int AKSFlBssnb11314823 = -462589271;    int AKSFlBssnb4052080 = -253173467;    int AKSFlBssnb504497 = -474953212;    int AKSFlBssnb75134816 = -109025813;    int AKSFlBssnb10437617 = 58203630;    int AKSFlBssnb11968416 = -441504463;     AKSFlBssnb23289186 = AKSFlBssnb25998849;     AKSFlBssnb25998849 = AKSFlBssnb79816295;     AKSFlBssnb79816295 = AKSFlBssnb67671976;     AKSFlBssnb67671976 = AKSFlBssnb85707386;     AKSFlBssnb85707386 = AKSFlBssnb5623789;     AKSFlBssnb5623789 = AKSFlBssnb4428795;     AKSFlBssnb4428795 = AKSFlBssnb99617938;     AKSFlBssnb99617938 = AKSFlBssnb31736886;     AKSFlBssnb31736886 = AKSFlBssnb50225709;     AKSFlBssnb50225709 = AKSFlBssnb42696408;     AKSFlBssnb42696408 = AKSFlBssnb93906783;     AKSFlBssnb93906783 = AKSFlBssnb83941955;     AKSFlBssnb83941955 = AKSFlBssnb10480931;     AKSFlBssnb10480931 = AKSFlBssnb59963730;     AKSFlBssnb59963730 = AKSFlBssnb31780725;     AKSFlBssnb31780725 = AKSFlBssnb89059167;     AKSFlBssnb89059167 = AKSFlBssnb52588388;     AKSFlBssnb52588388 = AKSFlBssnb84271949;     AKSFlBssnb84271949 = AKSFlBssnb1087221;     AKSFlBssnb1087221 = AKSFlBssnb71195439;     AKSFlBssnb71195439 = AKSFlBssnb75070775;     AKSFlBssnb75070775 = AKSFlBssnb76852984;     AKSFlBssnb76852984 = AKSFlBssnb97409006;     AKSFlBssnb97409006 = AKSFlBssnb13728888;     AKSFlBssnb13728888 = AKSFlBssnb78972045;     AKSFlBssnb78972045 = AKSFlBssnb64018186;     AKSFlBssnb64018186 = AKSFlBssnb38860626;     AKSFlBssnb38860626 = AKSFlBssnb68989316;     AKSFlBssnb68989316 = AKSFlBssnb41428667;     AKSFlBssnb41428667 = AKSFlBssnb13291214;     AKSFlBssnb13291214 = AKSFlBssnb38634014;     AKSFlBssnb38634014 = AKSFlBssnb81242352;     AKSFlBssnb81242352 = AKSFlBssnb40078422;     AKSFlBssnb40078422 = AKSFlBssnb66791137;     AKSFlBssnb66791137 = AKSFlBssnb13029979;     AKSFlBssnb13029979 = AKSFlBssnb12182109;     AKSFlBssnb12182109 = AKSFlBssnb28840992;     AKSFlBssnb28840992 = AKSFlBssnb832365;     AKSFlBssnb832365 = AKSFlBssnb52920425;     AKSFlBssnb52920425 = AKSFlBssnb69620248;     AKSFlBssnb69620248 = AKSFlBssnb31133119;     AKSFlBssnb31133119 = AKSFlBssnb65443036;     AKSFlBssnb65443036 = AKSFlBssnb47372112;     AKSFlBssnb47372112 = AKSFlBssnb16030809;     AKSFlBssnb16030809 = AKSFlBssnb63436753;     AKSFlBssnb63436753 = AKSFlBssnb23861342;     AKSFlBssnb23861342 = AKSFlBssnb56797654;     AKSFlBssnb56797654 = AKSFlBssnb4269487;     AKSFlBssnb4269487 = AKSFlBssnb29692787;     AKSFlBssnb29692787 = AKSFlBssnb66109923;     AKSFlBssnb66109923 = AKSFlBssnb68856313;     AKSFlBssnb68856313 = AKSFlBssnb91214079;     AKSFlBssnb91214079 = AKSFlBssnb63531179;     AKSFlBssnb63531179 = AKSFlBssnb21186853;     AKSFlBssnb21186853 = AKSFlBssnb48218410;     AKSFlBssnb48218410 = AKSFlBssnb49145864;     AKSFlBssnb49145864 = AKSFlBssnb82407288;     AKSFlBssnb82407288 = AKSFlBssnb53943089;     AKSFlBssnb53943089 = AKSFlBssnb6735341;     AKSFlBssnb6735341 = AKSFlBssnb41605602;     AKSFlBssnb41605602 = AKSFlBssnb65568168;     AKSFlBssnb65568168 = AKSFlBssnb30628623;     AKSFlBssnb30628623 = AKSFlBssnb90308219;     AKSFlBssnb90308219 = AKSFlBssnb36934495;     AKSFlBssnb36934495 = AKSFlBssnb4062394;     AKSFlBssnb4062394 = AKSFlBssnb12664432;     AKSFlBssnb12664432 = AKSFlBssnb43863533;     AKSFlBssnb43863533 = AKSFlBssnb43689793;     AKSFlBssnb43689793 = AKSFlBssnb46933751;     AKSFlBssnb46933751 = AKSFlBssnb19598617;     AKSFlBssnb19598617 = AKSFlBssnb60218175;     AKSFlBssnb60218175 = AKSFlBssnb51756023;     AKSFlBssnb51756023 = AKSFlBssnb31351525;     AKSFlBssnb31351525 = AKSFlBssnb31466973;     AKSFlBssnb31466973 = AKSFlBssnb40062320;     AKSFlBssnb40062320 = AKSFlBssnb9627740;     AKSFlBssnb9627740 = AKSFlBssnb29480873;     AKSFlBssnb29480873 = AKSFlBssnb81378197;     AKSFlBssnb81378197 = AKSFlBssnb50292134;     AKSFlBssnb50292134 = AKSFlBssnb55110703;     AKSFlBssnb55110703 = AKSFlBssnb7220532;     AKSFlBssnb7220532 = AKSFlBssnb34591140;     AKSFlBssnb34591140 = AKSFlBssnb39296529;     AKSFlBssnb39296529 = AKSFlBssnb75318743;     AKSFlBssnb75318743 = AKSFlBssnb44434901;     AKSFlBssnb44434901 = AKSFlBssnb47419935;     AKSFlBssnb47419935 = AKSFlBssnb17711173;     AKSFlBssnb17711173 = AKSFlBssnb18891570;     AKSFlBssnb18891570 = AKSFlBssnb18572728;     AKSFlBssnb18572728 = AKSFlBssnb63884115;     AKSFlBssnb63884115 = AKSFlBssnb29774821;     AKSFlBssnb29774821 = AKSFlBssnb74897902;     AKSFlBssnb74897902 = AKSFlBssnb94097023;     AKSFlBssnb94097023 = AKSFlBssnb11314823;     AKSFlBssnb11314823 = AKSFlBssnb4052080;     AKSFlBssnb4052080 = AKSFlBssnb504497;     AKSFlBssnb504497 = AKSFlBssnb75134816;     AKSFlBssnb75134816 = AKSFlBssnb10437617;     AKSFlBssnb10437617 = AKSFlBssnb11968416;     AKSFlBssnb11968416 = AKSFlBssnb23289186;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void wwOjTQmqzd29027267() {     int uxcxyFFAZu45676303 = -75908426;    int uxcxyFFAZu10346349 = -68567160;    int uxcxyFFAZu8585574 = 4532630;    int uxcxyFFAZu82678665 = -375714447;    int uxcxyFFAZu83881640 = -380021123;    int uxcxyFFAZu20506905 = -795799153;    int uxcxyFFAZu96584533 = -633884120;    int uxcxyFFAZu75251797 = -812108540;    int uxcxyFFAZu28900017 = -531489769;    int uxcxyFFAZu41989293 = -950371482;    int uxcxyFFAZu80238267 = -572127019;    int uxcxyFFAZu49809733 = 65569602;    int uxcxyFFAZu50950136 = -507397324;    int uxcxyFFAZu53095683 = -387548253;    int uxcxyFFAZu96699007 = -59606697;    int uxcxyFFAZu76036637 = 96568234;    int uxcxyFFAZu38882519 = -936532504;    int uxcxyFFAZu58564804 = -447089659;    int uxcxyFFAZu25553581 = -970217889;    int uxcxyFFAZu79603504 = -235138921;    int uxcxyFFAZu72078547 = -368420260;    int uxcxyFFAZu38885994 = -460280467;    int uxcxyFFAZu80146308 = -901388996;    int uxcxyFFAZu23908794 = -187705137;    int uxcxyFFAZu49269134 = -808013131;    int uxcxyFFAZu67275039 = -455832467;    int uxcxyFFAZu11107607 = -919063491;    int uxcxyFFAZu44772076 = -154205457;    int uxcxyFFAZu75881877 = -23853403;    int uxcxyFFAZu50585846 = -618732040;    int uxcxyFFAZu57582019 = -646180089;    int uxcxyFFAZu96175204 = -172649207;    int uxcxyFFAZu62281022 = -219867676;    int uxcxyFFAZu99467109 = -560619245;    int uxcxyFFAZu43020502 = 30961515;    int uxcxyFFAZu54722848 = -489153539;    int uxcxyFFAZu5621821 = -797708577;    int uxcxyFFAZu82467092 = -17816020;    int uxcxyFFAZu49140496 = 20179907;    int uxcxyFFAZu73106140 = -372527078;    int uxcxyFFAZu98377618 = -102496944;    int uxcxyFFAZu19641511 = -894777514;    int uxcxyFFAZu70993228 = 54586047;    int uxcxyFFAZu46816231 = -581926231;    int uxcxyFFAZu84014075 = 70659140;    int uxcxyFFAZu71537035 = -695818724;    int uxcxyFFAZu36158739 = -162207226;    int uxcxyFFAZu1396329 = 27181431;    int uxcxyFFAZu98729391 = -482520457;    int uxcxyFFAZu26776533 = -295792985;    int uxcxyFFAZu74979139 = -551250721;    int uxcxyFFAZu42707570 = -229922854;    int uxcxyFFAZu69528482 = -10312486;    int uxcxyFFAZu23817725 = 21637443;    int uxcxyFFAZu56948720 = -532118595;    int uxcxyFFAZu6790310 = -615627959;    int uxcxyFFAZu30200041 = -167178165;    int uxcxyFFAZu84676779 = -807762233;    int uxcxyFFAZu33409531 = -567701317;    int uxcxyFFAZu16606601 = -924188656;    int uxcxyFFAZu9399299 = -876735663;    int uxcxyFFAZu51812457 = -379678664;    int uxcxyFFAZu99369920 = -688255137;    int uxcxyFFAZu78314171 = -912757729;    int uxcxyFFAZu84407274 = -204191393;    int uxcxyFFAZu84063062 = -299477813;    int uxcxyFFAZu87528710 = -714562723;    int uxcxyFFAZu51483027 = -946778079;    int uxcxyFFAZu10075181 = -318509768;    int uxcxyFFAZu41976159 = -570453158;    int uxcxyFFAZu70414817 = -105723189;    int uxcxyFFAZu56415426 = -818716484;    int uxcxyFFAZu9424309 = -367269567;    int uxcxyFFAZu52447441 = -497690811;    int uxcxyFFAZu81225885 = -32641978;    int uxcxyFFAZu52437036 = -473642746;    int uxcxyFFAZu67892765 = -414866515;    int uxcxyFFAZu33330077 = -219462766;    int uxcxyFFAZu39894718 = -158364278;    int uxcxyFFAZu77732099 = -12194407;    int uxcxyFFAZu31116300 = -193625242;    int uxcxyFFAZu9711278 = -846244923;    int uxcxyFFAZu46042685 = -671685000;    int uxcxyFFAZu49105344 = -728060418;    int uxcxyFFAZu75606706 = 32518680;    int uxcxyFFAZu14874449 = -316257235;    int uxcxyFFAZu26646723 = -62336722;    int uxcxyFFAZu38463298 = -141505120;    int uxcxyFFAZu42518390 = 71499350;    int uxcxyFFAZu36230193 = -353410527;    int uxcxyFFAZu24522807 = -221975375;    int uxcxyFFAZu20945041 = -989946344;    int uxcxyFFAZu49057562 = -450114704;    int uxcxyFFAZu32533895 = -55631438;    int uxcxyFFAZu63706841 = -495791415;    int uxcxyFFAZu46565162 = -722818281;    int uxcxyFFAZu20271590 = -106522378;    int uxcxyFFAZu92679057 = -32656224;    int uxcxyFFAZu62408957 = -277734838;    int uxcxyFFAZu99951013 = -75908426;     uxcxyFFAZu45676303 = uxcxyFFAZu10346349;     uxcxyFFAZu10346349 = uxcxyFFAZu8585574;     uxcxyFFAZu8585574 = uxcxyFFAZu82678665;     uxcxyFFAZu82678665 = uxcxyFFAZu83881640;     uxcxyFFAZu83881640 = uxcxyFFAZu20506905;     uxcxyFFAZu20506905 = uxcxyFFAZu96584533;     uxcxyFFAZu96584533 = uxcxyFFAZu75251797;     uxcxyFFAZu75251797 = uxcxyFFAZu28900017;     uxcxyFFAZu28900017 = uxcxyFFAZu41989293;     uxcxyFFAZu41989293 = uxcxyFFAZu80238267;     uxcxyFFAZu80238267 = uxcxyFFAZu49809733;     uxcxyFFAZu49809733 = uxcxyFFAZu50950136;     uxcxyFFAZu50950136 = uxcxyFFAZu53095683;     uxcxyFFAZu53095683 = uxcxyFFAZu96699007;     uxcxyFFAZu96699007 = uxcxyFFAZu76036637;     uxcxyFFAZu76036637 = uxcxyFFAZu38882519;     uxcxyFFAZu38882519 = uxcxyFFAZu58564804;     uxcxyFFAZu58564804 = uxcxyFFAZu25553581;     uxcxyFFAZu25553581 = uxcxyFFAZu79603504;     uxcxyFFAZu79603504 = uxcxyFFAZu72078547;     uxcxyFFAZu72078547 = uxcxyFFAZu38885994;     uxcxyFFAZu38885994 = uxcxyFFAZu80146308;     uxcxyFFAZu80146308 = uxcxyFFAZu23908794;     uxcxyFFAZu23908794 = uxcxyFFAZu49269134;     uxcxyFFAZu49269134 = uxcxyFFAZu67275039;     uxcxyFFAZu67275039 = uxcxyFFAZu11107607;     uxcxyFFAZu11107607 = uxcxyFFAZu44772076;     uxcxyFFAZu44772076 = uxcxyFFAZu75881877;     uxcxyFFAZu75881877 = uxcxyFFAZu50585846;     uxcxyFFAZu50585846 = uxcxyFFAZu57582019;     uxcxyFFAZu57582019 = uxcxyFFAZu96175204;     uxcxyFFAZu96175204 = uxcxyFFAZu62281022;     uxcxyFFAZu62281022 = uxcxyFFAZu99467109;     uxcxyFFAZu99467109 = uxcxyFFAZu43020502;     uxcxyFFAZu43020502 = uxcxyFFAZu54722848;     uxcxyFFAZu54722848 = uxcxyFFAZu5621821;     uxcxyFFAZu5621821 = uxcxyFFAZu82467092;     uxcxyFFAZu82467092 = uxcxyFFAZu49140496;     uxcxyFFAZu49140496 = uxcxyFFAZu73106140;     uxcxyFFAZu73106140 = uxcxyFFAZu98377618;     uxcxyFFAZu98377618 = uxcxyFFAZu19641511;     uxcxyFFAZu19641511 = uxcxyFFAZu70993228;     uxcxyFFAZu70993228 = uxcxyFFAZu46816231;     uxcxyFFAZu46816231 = uxcxyFFAZu84014075;     uxcxyFFAZu84014075 = uxcxyFFAZu71537035;     uxcxyFFAZu71537035 = uxcxyFFAZu36158739;     uxcxyFFAZu36158739 = uxcxyFFAZu1396329;     uxcxyFFAZu1396329 = uxcxyFFAZu98729391;     uxcxyFFAZu98729391 = uxcxyFFAZu26776533;     uxcxyFFAZu26776533 = uxcxyFFAZu74979139;     uxcxyFFAZu74979139 = uxcxyFFAZu42707570;     uxcxyFFAZu42707570 = uxcxyFFAZu69528482;     uxcxyFFAZu69528482 = uxcxyFFAZu23817725;     uxcxyFFAZu23817725 = uxcxyFFAZu56948720;     uxcxyFFAZu56948720 = uxcxyFFAZu6790310;     uxcxyFFAZu6790310 = uxcxyFFAZu30200041;     uxcxyFFAZu30200041 = uxcxyFFAZu84676779;     uxcxyFFAZu84676779 = uxcxyFFAZu33409531;     uxcxyFFAZu33409531 = uxcxyFFAZu16606601;     uxcxyFFAZu16606601 = uxcxyFFAZu9399299;     uxcxyFFAZu9399299 = uxcxyFFAZu51812457;     uxcxyFFAZu51812457 = uxcxyFFAZu99369920;     uxcxyFFAZu99369920 = uxcxyFFAZu78314171;     uxcxyFFAZu78314171 = uxcxyFFAZu84407274;     uxcxyFFAZu84407274 = uxcxyFFAZu84063062;     uxcxyFFAZu84063062 = uxcxyFFAZu87528710;     uxcxyFFAZu87528710 = uxcxyFFAZu51483027;     uxcxyFFAZu51483027 = uxcxyFFAZu10075181;     uxcxyFFAZu10075181 = uxcxyFFAZu41976159;     uxcxyFFAZu41976159 = uxcxyFFAZu70414817;     uxcxyFFAZu70414817 = uxcxyFFAZu56415426;     uxcxyFFAZu56415426 = uxcxyFFAZu9424309;     uxcxyFFAZu9424309 = uxcxyFFAZu52447441;     uxcxyFFAZu52447441 = uxcxyFFAZu81225885;     uxcxyFFAZu81225885 = uxcxyFFAZu52437036;     uxcxyFFAZu52437036 = uxcxyFFAZu67892765;     uxcxyFFAZu67892765 = uxcxyFFAZu33330077;     uxcxyFFAZu33330077 = uxcxyFFAZu39894718;     uxcxyFFAZu39894718 = uxcxyFFAZu77732099;     uxcxyFFAZu77732099 = uxcxyFFAZu31116300;     uxcxyFFAZu31116300 = uxcxyFFAZu9711278;     uxcxyFFAZu9711278 = uxcxyFFAZu46042685;     uxcxyFFAZu46042685 = uxcxyFFAZu49105344;     uxcxyFFAZu49105344 = uxcxyFFAZu75606706;     uxcxyFFAZu75606706 = uxcxyFFAZu14874449;     uxcxyFFAZu14874449 = uxcxyFFAZu26646723;     uxcxyFFAZu26646723 = uxcxyFFAZu38463298;     uxcxyFFAZu38463298 = uxcxyFFAZu42518390;     uxcxyFFAZu42518390 = uxcxyFFAZu36230193;     uxcxyFFAZu36230193 = uxcxyFFAZu24522807;     uxcxyFFAZu24522807 = uxcxyFFAZu20945041;     uxcxyFFAZu20945041 = uxcxyFFAZu49057562;     uxcxyFFAZu49057562 = uxcxyFFAZu32533895;     uxcxyFFAZu32533895 = uxcxyFFAZu63706841;     uxcxyFFAZu63706841 = uxcxyFFAZu46565162;     uxcxyFFAZu46565162 = uxcxyFFAZu20271590;     uxcxyFFAZu20271590 = uxcxyFFAZu92679057;     uxcxyFFAZu92679057 = uxcxyFFAZu62408957;     uxcxyFFAZu62408957 = uxcxyFFAZu99951013;     uxcxyFFAZu99951013 = uxcxyFFAZu45676303;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void RveiJAcEvK65672241() {     int vHonoExvRh68063420 = -810312389;    int vHonoExvRh94693849 = -17110215;    int vHonoExvRh37354852 = -235121214;    int vHonoExvRh97685353 = -382121979;    int vHonoExvRh82055894 = -971078124;    int vHonoExvRh35390022 = -656407531;    int vHonoExvRh88740272 = -455871426;    int vHonoExvRh50885656 = -182705184;    int vHonoExvRh26063148 = -415685808;    int vHonoExvRh33752878 = -693112815;    int vHonoExvRh17780127 = -568751420;    int vHonoExvRh5712682 = -185175692;    int vHonoExvRh17958317 = -26114644;    int vHonoExvRh95710434 = -915257010;    int vHonoExvRh33434284 = -787084410;    int vHonoExvRh20292550 = -285099308;    int vHonoExvRh88705870 = -113812324;    int vHonoExvRh64541220 = -898832350;    int vHonoExvRh66835211 = -5320014;    int vHonoExvRh58119787 = -487773375;    int vHonoExvRh72961654 = -359907569;    int vHonoExvRh2701212 = -965046781;    int vHonoExvRh83439631 = 22530412;    int vHonoExvRh50408581 = -751697995;    int vHonoExvRh84809381 = -429572255;    int vHonoExvRh55578034 = -899726691;    int vHonoExvRh58197027 = -676649984;    int vHonoExvRh50683526 = 57340090;    int vHonoExvRh82774438 = -368110522;    int vHonoExvRh59743025 = -539027907;    int vHonoExvRh1872825 = -727496241;    int vHonoExvRh53716396 = -824720561;    int vHonoExvRh43319693 = -749211171;    int vHonoExvRh58855797 = -884746096;    int vHonoExvRh19249867 = -32794908;    int vHonoExvRh96415717 = -804548915;    int vHonoExvRh99061532 = -530656529;    int vHonoExvRh36093193 = -985441492;    int vHonoExvRh97448627 = -380866125;    int vHonoExvRh93291854 = -508751107;    int vHonoExvRh27134990 = -605674610;    int vHonoExvRh8149902 = -652686205;    int vHonoExvRh76543420 = -932944537;    int vHonoExvRh46260350 = -579289880;    int vHonoExvRh51997342 = -838484337;    int vHonoExvRh79637317 = -878047819;    int vHonoExvRh48456137 = -740617479;    int vHonoExvRh45995003 = -829353559;    int vHonoExvRh93189296 = -432237156;    int vHonoExvRh23860279 = -437813244;    int vHonoExvRh83848356 = -906326998;    int vHonoExvRh16558828 = -660742136;    int vHonoExvRh47842886 = -782223716;    int vHonoExvRh84104269 = -254043365;    int vHonoExvRh92710587 = -672822711;    int vHonoExvRh65362208 = -845265609;    int vHonoExvRh11254218 = 60359373;    int vHonoExvRh86946271 = -483423220;    int vHonoExvRh12875973 = -952549724;    int vHonoExvRh26477860 = 28648567;    int vHonoExvRh77192995 = -979757548;    int vHonoExvRh38056746 = -413211516;    int vHonoExvRh68111218 = -814594662;    int vHonoExvRh66320123 = -876657902;    int vHonoExvRh31880054 = -965616574;    int vHonoExvRh64063731 = -744030859;    int vHonoExvRh62392989 = -435964521;    int vHonoExvRh59102520 = -141368549;    int vHonoExvRh76460568 = -782462102;    int vHonoExvRh37018567 = -982535495;    int vHonoExvRh21231017 = -754442780;    int vHonoExvRh52612677 = -128370832;    int vHonoExvRh67092593 = -417966226;    int vHonoExvRh73543357 = -496568907;    int vHonoExvRh30984798 = -882098765;    int vHonoExvRh64811752 = -707221364;    int vHonoExvRh26157792 = 67897756;    int vHonoExvRh37179281 = -398179709;    int vHonoExvRh98411238 = -913213658;    int vHonoExvRh5172064 = -551524437;    int vHonoExvRh7121898 = -59109213;    int vHonoExvRh12202024 = -847296425;    int vHonoExvRh57494230 = -510422755;    int vHonoExvRh58914159 = -930297278;    int vHonoExvRh75894669 = -632700909;    int vHonoExvRh85313996 = 33245895;    int vHonoExvRh5873511 = 57503154;    int vHonoExvRh59215423 = -395167806;    int vHonoExvRh66145210 = -111923385;    int vHonoExvRh53887658 = -187529300;    int vHonoExvRh85161499 = -764908288;    int vHonoExvRh12115262 = 52766691;    int vHonoExvRh23217221 = 67108231;    int vHonoExvRh70970767 = -309514692;    int vHonoExvRh16098859 = -528993559;    int vHonoExvRh89078243 = -92463094;    int vHonoExvRh40038683 = -838091543;    int vHonoExvRh10223298 = 43713364;    int vHonoExvRh14380297 = -613673306;    int vHonoExvRh87933611 = -810312389;     vHonoExvRh68063420 = vHonoExvRh94693849;     vHonoExvRh94693849 = vHonoExvRh37354852;     vHonoExvRh37354852 = vHonoExvRh97685353;     vHonoExvRh97685353 = vHonoExvRh82055894;     vHonoExvRh82055894 = vHonoExvRh35390022;     vHonoExvRh35390022 = vHonoExvRh88740272;     vHonoExvRh88740272 = vHonoExvRh50885656;     vHonoExvRh50885656 = vHonoExvRh26063148;     vHonoExvRh26063148 = vHonoExvRh33752878;     vHonoExvRh33752878 = vHonoExvRh17780127;     vHonoExvRh17780127 = vHonoExvRh5712682;     vHonoExvRh5712682 = vHonoExvRh17958317;     vHonoExvRh17958317 = vHonoExvRh95710434;     vHonoExvRh95710434 = vHonoExvRh33434284;     vHonoExvRh33434284 = vHonoExvRh20292550;     vHonoExvRh20292550 = vHonoExvRh88705870;     vHonoExvRh88705870 = vHonoExvRh64541220;     vHonoExvRh64541220 = vHonoExvRh66835211;     vHonoExvRh66835211 = vHonoExvRh58119787;     vHonoExvRh58119787 = vHonoExvRh72961654;     vHonoExvRh72961654 = vHonoExvRh2701212;     vHonoExvRh2701212 = vHonoExvRh83439631;     vHonoExvRh83439631 = vHonoExvRh50408581;     vHonoExvRh50408581 = vHonoExvRh84809381;     vHonoExvRh84809381 = vHonoExvRh55578034;     vHonoExvRh55578034 = vHonoExvRh58197027;     vHonoExvRh58197027 = vHonoExvRh50683526;     vHonoExvRh50683526 = vHonoExvRh82774438;     vHonoExvRh82774438 = vHonoExvRh59743025;     vHonoExvRh59743025 = vHonoExvRh1872825;     vHonoExvRh1872825 = vHonoExvRh53716396;     vHonoExvRh53716396 = vHonoExvRh43319693;     vHonoExvRh43319693 = vHonoExvRh58855797;     vHonoExvRh58855797 = vHonoExvRh19249867;     vHonoExvRh19249867 = vHonoExvRh96415717;     vHonoExvRh96415717 = vHonoExvRh99061532;     vHonoExvRh99061532 = vHonoExvRh36093193;     vHonoExvRh36093193 = vHonoExvRh97448627;     vHonoExvRh97448627 = vHonoExvRh93291854;     vHonoExvRh93291854 = vHonoExvRh27134990;     vHonoExvRh27134990 = vHonoExvRh8149902;     vHonoExvRh8149902 = vHonoExvRh76543420;     vHonoExvRh76543420 = vHonoExvRh46260350;     vHonoExvRh46260350 = vHonoExvRh51997342;     vHonoExvRh51997342 = vHonoExvRh79637317;     vHonoExvRh79637317 = vHonoExvRh48456137;     vHonoExvRh48456137 = vHonoExvRh45995003;     vHonoExvRh45995003 = vHonoExvRh93189296;     vHonoExvRh93189296 = vHonoExvRh23860279;     vHonoExvRh23860279 = vHonoExvRh83848356;     vHonoExvRh83848356 = vHonoExvRh16558828;     vHonoExvRh16558828 = vHonoExvRh47842886;     vHonoExvRh47842886 = vHonoExvRh84104269;     vHonoExvRh84104269 = vHonoExvRh92710587;     vHonoExvRh92710587 = vHonoExvRh65362208;     vHonoExvRh65362208 = vHonoExvRh11254218;     vHonoExvRh11254218 = vHonoExvRh86946271;     vHonoExvRh86946271 = vHonoExvRh12875973;     vHonoExvRh12875973 = vHonoExvRh26477860;     vHonoExvRh26477860 = vHonoExvRh77192995;     vHonoExvRh77192995 = vHonoExvRh38056746;     vHonoExvRh38056746 = vHonoExvRh68111218;     vHonoExvRh68111218 = vHonoExvRh66320123;     vHonoExvRh66320123 = vHonoExvRh31880054;     vHonoExvRh31880054 = vHonoExvRh64063731;     vHonoExvRh64063731 = vHonoExvRh62392989;     vHonoExvRh62392989 = vHonoExvRh59102520;     vHonoExvRh59102520 = vHonoExvRh76460568;     vHonoExvRh76460568 = vHonoExvRh37018567;     vHonoExvRh37018567 = vHonoExvRh21231017;     vHonoExvRh21231017 = vHonoExvRh52612677;     vHonoExvRh52612677 = vHonoExvRh67092593;     vHonoExvRh67092593 = vHonoExvRh73543357;     vHonoExvRh73543357 = vHonoExvRh30984798;     vHonoExvRh30984798 = vHonoExvRh64811752;     vHonoExvRh64811752 = vHonoExvRh26157792;     vHonoExvRh26157792 = vHonoExvRh37179281;     vHonoExvRh37179281 = vHonoExvRh98411238;     vHonoExvRh98411238 = vHonoExvRh5172064;     vHonoExvRh5172064 = vHonoExvRh7121898;     vHonoExvRh7121898 = vHonoExvRh12202024;     vHonoExvRh12202024 = vHonoExvRh57494230;     vHonoExvRh57494230 = vHonoExvRh58914159;     vHonoExvRh58914159 = vHonoExvRh75894669;     vHonoExvRh75894669 = vHonoExvRh85313996;     vHonoExvRh85313996 = vHonoExvRh5873511;     vHonoExvRh5873511 = vHonoExvRh59215423;     vHonoExvRh59215423 = vHonoExvRh66145210;     vHonoExvRh66145210 = vHonoExvRh53887658;     vHonoExvRh53887658 = vHonoExvRh85161499;     vHonoExvRh85161499 = vHonoExvRh12115262;     vHonoExvRh12115262 = vHonoExvRh23217221;     vHonoExvRh23217221 = vHonoExvRh70970767;     vHonoExvRh70970767 = vHonoExvRh16098859;     vHonoExvRh16098859 = vHonoExvRh89078243;     vHonoExvRh89078243 = vHonoExvRh40038683;     vHonoExvRh40038683 = vHonoExvRh10223298;     vHonoExvRh10223298 = vHonoExvRh14380297;     vHonoExvRh14380297 = vHonoExvRh87933611;     vHonoExvRh87933611 = vHonoExvRh68063420;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void VoPBVzEBTS2317216() {     int FCCelMrGet90450537 = -444716350;    int FCCelMrGet79041349 = 34346736;    int FCCelMrGet66124131 = -474775058;    int FCCelMrGet12692042 = -388529513;    int FCCelMrGet80230148 = -462135117;    int FCCelMrGet50273139 = -517015909;    int FCCelMrGet80896011 = -277858732;    int FCCelMrGet26519515 = -653301813;    int FCCelMrGet23226279 = -299881846;    int FCCelMrGet25516463 = -435854148;    int FCCelMrGet55321987 = -565375820;    int FCCelMrGet61615630 = -435920976;    int FCCelMrGet84966497 = -644831964;    int FCCelMrGet38325187 = -342965766;    int FCCelMrGet70169561 = -414562104;    int FCCelMrGet64548462 = -666766851;    int FCCelMrGet38529222 = -391092144;    int FCCelMrGet70517636 = -250575037;    int FCCelMrGet8116842 = -140422140;    int FCCelMrGet36636071 = -740407829;    int FCCelMrGet73844761 = -351394872;    int FCCelMrGet66516430 = -369813094;    int FCCelMrGet86732954 = -153550180;    int FCCelMrGet76908368 = -215690852;    int FCCelMrGet20349628 = -51131367;    int FCCelMrGet43881028 = -243620919;    int FCCelMrGet5286448 = -434236477;    int FCCelMrGet56594976 = -831114358;    int FCCelMrGet89666998 = -712367657;    int FCCelMrGet68900205 = -459323773;    int FCCelMrGet46163629 = -808812393;    int FCCelMrGet11257587 = -376791919;    int FCCelMrGet24358364 = -178554669;    int FCCelMrGet18244485 = -108872946;    int FCCelMrGet95479231 = -96551334;    int FCCelMrGet38108586 = -19944310;    int FCCelMrGet92501245 = -263604480;    int FCCelMrGet89719294 = -853066961;    int FCCelMrGet45756758 = -781912169;    int FCCelMrGet13477570 = -644975136;    int FCCelMrGet55892360 = -8852276;    int FCCelMrGet96658292 = -410594917;    int FCCelMrGet82093612 = -820475123;    int FCCelMrGet45704469 = -576653529;    int FCCelMrGet19980609 = -647627816;    int FCCelMrGet87737599 = 39723067;    int FCCelMrGet60753535 = -219027732;    int FCCelMrGet90593677 = -585888551;    int FCCelMrGet87649201 = -381953879;    int FCCelMrGet20944025 = -579833498;    int FCCelMrGet92717572 = -161403275;    int FCCelMrGet90410084 = 8438578;    int FCCelMrGet26157290 = -454134943;    int FCCelMrGet44390815 = -529724174;    int FCCelMrGet28472455 = -813526831;    int FCCelMrGet23934108 = 25096744;    int FCCelMrGet92308395 = -812103084;    int FCCelMrGet89215762 = -159084207;    int FCCelMrGet92342413 = -237398147;    int FCCelMrGet36349120 = -118514198;    int FCCelMrGet44986692 = 17220567;    int FCCelMrGet24301035 = -446744375;    int FCCelMrGet36852516 = -940934157;    int FCCelMrGet54326074 = -840558073;    int FCCelMrGet79352833 = -627041755;    int FCCelMrGet44064400 = -88583901;    int FCCelMrGet37257267 = -157366308;    int FCCelMrGet66722013 = -435959018;    int FCCelMrGet42845956 = -146414433;    int FCCelMrGet32060975 = -294617795;    int FCCelMrGet72047217 = -303162371;    int FCCelMrGet48809928 = -538025183;    int FCCelMrGet24760879 = -468662868;    int FCCelMrGet94639272 = -495447004;    int FCCelMrGet80743710 = -631555553;    int FCCelMrGet77186468 = -940799956;    int FCCelMrGet84422818 = -549337972;    int FCCelMrGet41028485 = -576896652;    int FCCelMrGet56927759 = -568063036;    int FCCelMrGet32612029 = 9145565;    int FCCelMrGet83127493 = 75406812;    int FCCelMrGet14692770 = -848347926;    int FCCelMrGet68945775 = -349160480;    int FCCelMrGet68722974 = -32534159;    int FCCelMrGet76182632 = -197920499;    int FCCelMrGet55753545 = -717250971;    int FCCelMrGet85100297 = -922656977;    int FCCelMrGet79967548 = -648830495;    int FCCelMrGet89772029 = -295346115;    int FCCelMrGet71545124 = -21648078;    int FCCelMrGet45800190 = -207841227;    int FCCelMrGet3285483 = -4520274;    int FCCelMrGet97376880 = -515668815;    int FCCelMrGet9407638 = -563397971;    int FCCelMrGet68490877 = -562195704;    int FCCelMrGet31591326 = -562107902;    int FCCelMrGet59805776 = -469660761;    int FCCelMrGet27767538 = -979917050;    int FCCelMrGet66351636 = -949611774;    int FCCelMrGet75916209 = -444716350;     FCCelMrGet90450537 = FCCelMrGet79041349;     FCCelMrGet79041349 = FCCelMrGet66124131;     FCCelMrGet66124131 = FCCelMrGet12692042;     FCCelMrGet12692042 = FCCelMrGet80230148;     FCCelMrGet80230148 = FCCelMrGet50273139;     FCCelMrGet50273139 = FCCelMrGet80896011;     FCCelMrGet80896011 = FCCelMrGet26519515;     FCCelMrGet26519515 = FCCelMrGet23226279;     FCCelMrGet23226279 = FCCelMrGet25516463;     FCCelMrGet25516463 = FCCelMrGet55321987;     FCCelMrGet55321987 = FCCelMrGet61615630;     FCCelMrGet61615630 = FCCelMrGet84966497;     FCCelMrGet84966497 = FCCelMrGet38325187;     FCCelMrGet38325187 = FCCelMrGet70169561;     FCCelMrGet70169561 = FCCelMrGet64548462;     FCCelMrGet64548462 = FCCelMrGet38529222;     FCCelMrGet38529222 = FCCelMrGet70517636;     FCCelMrGet70517636 = FCCelMrGet8116842;     FCCelMrGet8116842 = FCCelMrGet36636071;     FCCelMrGet36636071 = FCCelMrGet73844761;     FCCelMrGet73844761 = FCCelMrGet66516430;     FCCelMrGet66516430 = FCCelMrGet86732954;     FCCelMrGet86732954 = FCCelMrGet76908368;     FCCelMrGet76908368 = FCCelMrGet20349628;     FCCelMrGet20349628 = FCCelMrGet43881028;     FCCelMrGet43881028 = FCCelMrGet5286448;     FCCelMrGet5286448 = FCCelMrGet56594976;     FCCelMrGet56594976 = FCCelMrGet89666998;     FCCelMrGet89666998 = FCCelMrGet68900205;     FCCelMrGet68900205 = FCCelMrGet46163629;     FCCelMrGet46163629 = FCCelMrGet11257587;     FCCelMrGet11257587 = FCCelMrGet24358364;     FCCelMrGet24358364 = FCCelMrGet18244485;     FCCelMrGet18244485 = FCCelMrGet95479231;     FCCelMrGet95479231 = FCCelMrGet38108586;     FCCelMrGet38108586 = FCCelMrGet92501245;     FCCelMrGet92501245 = FCCelMrGet89719294;     FCCelMrGet89719294 = FCCelMrGet45756758;     FCCelMrGet45756758 = FCCelMrGet13477570;     FCCelMrGet13477570 = FCCelMrGet55892360;     FCCelMrGet55892360 = FCCelMrGet96658292;     FCCelMrGet96658292 = FCCelMrGet82093612;     FCCelMrGet82093612 = FCCelMrGet45704469;     FCCelMrGet45704469 = FCCelMrGet19980609;     FCCelMrGet19980609 = FCCelMrGet87737599;     FCCelMrGet87737599 = FCCelMrGet60753535;     FCCelMrGet60753535 = FCCelMrGet90593677;     FCCelMrGet90593677 = FCCelMrGet87649201;     FCCelMrGet87649201 = FCCelMrGet20944025;     FCCelMrGet20944025 = FCCelMrGet92717572;     FCCelMrGet92717572 = FCCelMrGet90410084;     FCCelMrGet90410084 = FCCelMrGet26157290;     FCCelMrGet26157290 = FCCelMrGet44390815;     FCCelMrGet44390815 = FCCelMrGet28472455;     FCCelMrGet28472455 = FCCelMrGet23934108;     FCCelMrGet23934108 = FCCelMrGet92308395;     FCCelMrGet92308395 = FCCelMrGet89215762;     FCCelMrGet89215762 = FCCelMrGet92342413;     FCCelMrGet92342413 = FCCelMrGet36349120;     FCCelMrGet36349120 = FCCelMrGet44986692;     FCCelMrGet44986692 = FCCelMrGet24301035;     FCCelMrGet24301035 = FCCelMrGet36852516;     FCCelMrGet36852516 = FCCelMrGet54326074;     FCCelMrGet54326074 = FCCelMrGet79352833;     FCCelMrGet79352833 = FCCelMrGet44064400;     FCCelMrGet44064400 = FCCelMrGet37257267;     FCCelMrGet37257267 = FCCelMrGet66722013;     FCCelMrGet66722013 = FCCelMrGet42845956;     FCCelMrGet42845956 = FCCelMrGet32060975;     FCCelMrGet32060975 = FCCelMrGet72047217;     FCCelMrGet72047217 = FCCelMrGet48809928;     FCCelMrGet48809928 = FCCelMrGet24760879;     FCCelMrGet24760879 = FCCelMrGet94639272;     FCCelMrGet94639272 = FCCelMrGet80743710;     FCCelMrGet80743710 = FCCelMrGet77186468;     FCCelMrGet77186468 = FCCelMrGet84422818;     FCCelMrGet84422818 = FCCelMrGet41028485;     FCCelMrGet41028485 = FCCelMrGet56927759;     FCCelMrGet56927759 = FCCelMrGet32612029;     FCCelMrGet32612029 = FCCelMrGet83127493;     FCCelMrGet83127493 = FCCelMrGet14692770;     FCCelMrGet14692770 = FCCelMrGet68945775;     FCCelMrGet68945775 = FCCelMrGet68722974;     FCCelMrGet68722974 = FCCelMrGet76182632;     FCCelMrGet76182632 = FCCelMrGet55753545;     FCCelMrGet55753545 = FCCelMrGet85100297;     FCCelMrGet85100297 = FCCelMrGet79967548;     FCCelMrGet79967548 = FCCelMrGet89772029;     FCCelMrGet89772029 = FCCelMrGet71545124;     FCCelMrGet71545124 = FCCelMrGet45800190;     FCCelMrGet45800190 = FCCelMrGet3285483;     FCCelMrGet3285483 = FCCelMrGet97376880;     FCCelMrGet97376880 = FCCelMrGet9407638;     FCCelMrGet9407638 = FCCelMrGet68490877;     FCCelMrGet68490877 = FCCelMrGet31591326;     FCCelMrGet31591326 = FCCelMrGet59805776;     FCCelMrGet59805776 = FCCelMrGet27767538;     FCCelMrGet27767538 = FCCelMrGet66351636;     FCCelMrGet66351636 = FCCelMrGet75916209;     FCCelMrGet75916209 = FCCelMrGet90450537;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void BdxnpLuaOj38962191() {     int DagHTjeoIg12837655 = -79120315;    int DagHTjeoIg63388850 = 85803676;    int DagHTjeoIg94893409 = -714428902;    int DagHTjeoIg27698730 = -394937042;    int DagHTjeoIg78404402 = 46807874;    int DagHTjeoIg65156256 = -377624287;    int DagHTjeoIg73051750 = -99846038;    int DagHTjeoIg2153374 = -23898473;    int DagHTjeoIg20389411 = -184077886;    int DagHTjeoIg17280047 = -178595481;    int DagHTjeoIg92863846 = -562000222;    int DagHTjeoIg17518580 = -686666278;    int DagHTjeoIg51974678 = -163549283;    int DagHTjeoIg80939939 = -870674524;    int DagHTjeoIg6904839 = -42039836;    int DagHTjeoIg8804375 = 51565607;    int DagHTjeoIg88352574 = -668371964;    int DagHTjeoIg76494053 = -702317731;    int DagHTjeoIg49398473 = -275524264;    int DagHTjeoIg15152354 = -993042282;    int DagHTjeoIg74727868 = -342882185;    int DagHTjeoIg30331649 = -874579407;    int DagHTjeoIg90026277 = -329630773;    int DagHTjeoIg3408156 = -779683709;    int DagHTjeoIg55889875 = -772690504;    int DagHTjeoIg32184023 = -687515139;    int DagHTjeoIg52375867 = -191822969;    int DagHTjeoIg62506426 = -619568817;    int DagHTjeoIg96559559 = 43375239;    int DagHTjeoIg78057384 = -379619640;    int DagHTjeoIg90454434 = -890128545;    int DagHTjeoIg68798777 = 71136729;    int DagHTjeoIg5397034 = -707898161;    int DagHTjeoIg77633171 = -432999796;    int DagHTjeoIg71708596 = -160307753;    int DagHTjeoIg79801455 = -335339668;    int DagHTjeoIg85940957 = 3447568;    int DagHTjeoIg43345395 = -720692436;    int DagHTjeoIg94064889 = -82958188;    int DagHTjeoIg33663284 = -781199166;    int DagHTjeoIg84649731 = -512029942;    int DagHTjeoIg85166684 = -168503587;    int DagHTjeoIg87643804 = -708005704;    int DagHTjeoIg45148589 = -574017178;    int DagHTjeoIg87963875 = -456771293;    int DagHTjeoIg95837880 = -142506010;    int DagHTjeoIg73050931 = -797437985;    int DagHTjeoIg35192353 = -342423541;    int DagHTjeoIg82109106 = -331670552;    int DagHTjeoIg18027770 = -721853762;    int DagHTjeoIg1586790 = -516479552;    int DagHTjeoIg64261342 = -422380700;    int DagHTjeoIg4471693 = -126046176;    int DagHTjeoIg4677361 = -805404983;    int DagHTjeoIg64234322 = -954230941;    int DagHTjeoIg82506006 = -204540908;    int DagHTjeoIg73362572 = -584565552;    int DagHTjeoIg91485254 = -934745194;    int DagHTjeoIg71808855 = -622246538;    int DagHTjeoIg46220379 = -265676988;    int DagHTjeoIg12780389 = -85801318;    int DagHTjeoIg10545324 = -480277221;    int DagHTjeoIg5593815 = 32726288;    int DagHTjeoIg42332026 = -804458247;    int DagHTjeoIg26825613 = -288466936;    int DagHTjeoIg24065069 = -533136952;    int DagHTjeoIg12121546 = -978768118;    int DagHTjeoIg74341506 = -730549488;    int DagHTjeoIg9231344 = -610366771;    int DagHTjeoIg27103383 = -706700168;    int DagHTjeoIg22863418 = -951881962;    int DagHTjeoIg45007179 = -947679528;    int DagHTjeoIg82429163 = -519359544;    int DagHTjeoIg15735190 = -494325099;    int DagHTjeoIg30502623 = -381012341;    int DagHTjeoIg89561184 = -74378599;    int DagHTjeoIg42687845 = -66573703;    int DagHTjeoIg44877689 = -755613595;    int DagHTjeoIg15444280 = -222912417;    int DagHTjeoIg60051994 = -530184495;    int DagHTjeoIg59133092 = -890077155;    int DagHTjeoIg17183515 = -849399429;    int DagHTjeoIg80397320 = -187898266;    int DagHTjeoIg78531789 = -234770999;    int DagHTjeoIg76470595 = -863140089;    int DagHTjeoIg26193093 = -367747845;    int DagHTjeoIg64327085 = -802817095;    int DagHTjeoIg719674 = -902493178;    int DagHTjeoIg13398850 = -478768855;    int DagHTjeoIg89202589 = -955766845;    int DagHTjeoIg6438883 = -750774116;    int DagHTjeoIg94455703 = -61807238;    int DagHTjeoIg71536539 = 1554102;    int DagHTjeoIg47844510 = -817281201;    int DagHTjeoIg20882895 = -595397848;    int DagHTjeoIg74104408 = 68247279;    int DagHTjeoIg79572870 = -101229875;    int DagHTjeoIg45311778 = -903547458;    int DagHTjeoIg18322976 = -185550242;    int DagHTjeoIg63898807 = -79120315;     DagHTjeoIg12837655 = DagHTjeoIg63388850;     DagHTjeoIg63388850 = DagHTjeoIg94893409;     DagHTjeoIg94893409 = DagHTjeoIg27698730;     DagHTjeoIg27698730 = DagHTjeoIg78404402;     DagHTjeoIg78404402 = DagHTjeoIg65156256;     DagHTjeoIg65156256 = DagHTjeoIg73051750;     DagHTjeoIg73051750 = DagHTjeoIg2153374;     DagHTjeoIg2153374 = DagHTjeoIg20389411;     DagHTjeoIg20389411 = DagHTjeoIg17280047;     DagHTjeoIg17280047 = DagHTjeoIg92863846;     DagHTjeoIg92863846 = DagHTjeoIg17518580;     DagHTjeoIg17518580 = DagHTjeoIg51974678;     DagHTjeoIg51974678 = DagHTjeoIg80939939;     DagHTjeoIg80939939 = DagHTjeoIg6904839;     DagHTjeoIg6904839 = DagHTjeoIg8804375;     DagHTjeoIg8804375 = DagHTjeoIg88352574;     DagHTjeoIg88352574 = DagHTjeoIg76494053;     DagHTjeoIg76494053 = DagHTjeoIg49398473;     DagHTjeoIg49398473 = DagHTjeoIg15152354;     DagHTjeoIg15152354 = DagHTjeoIg74727868;     DagHTjeoIg74727868 = DagHTjeoIg30331649;     DagHTjeoIg30331649 = DagHTjeoIg90026277;     DagHTjeoIg90026277 = DagHTjeoIg3408156;     DagHTjeoIg3408156 = DagHTjeoIg55889875;     DagHTjeoIg55889875 = DagHTjeoIg32184023;     DagHTjeoIg32184023 = DagHTjeoIg52375867;     DagHTjeoIg52375867 = DagHTjeoIg62506426;     DagHTjeoIg62506426 = DagHTjeoIg96559559;     DagHTjeoIg96559559 = DagHTjeoIg78057384;     DagHTjeoIg78057384 = DagHTjeoIg90454434;     DagHTjeoIg90454434 = DagHTjeoIg68798777;     DagHTjeoIg68798777 = DagHTjeoIg5397034;     DagHTjeoIg5397034 = DagHTjeoIg77633171;     DagHTjeoIg77633171 = DagHTjeoIg71708596;     DagHTjeoIg71708596 = DagHTjeoIg79801455;     DagHTjeoIg79801455 = DagHTjeoIg85940957;     DagHTjeoIg85940957 = DagHTjeoIg43345395;     DagHTjeoIg43345395 = DagHTjeoIg94064889;     DagHTjeoIg94064889 = DagHTjeoIg33663284;     DagHTjeoIg33663284 = DagHTjeoIg84649731;     DagHTjeoIg84649731 = DagHTjeoIg85166684;     DagHTjeoIg85166684 = DagHTjeoIg87643804;     DagHTjeoIg87643804 = DagHTjeoIg45148589;     DagHTjeoIg45148589 = DagHTjeoIg87963875;     DagHTjeoIg87963875 = DagHTjeoIg95837880;     DagHTjeoIg95837880 = DagHTjeoIg73050931;     DagHTjeoIg73050931 = DagHTjeoIg35192353;     DagHTjeoIg35192353 = DagHTjeoIg82109106;     DagHTjeoIg82109106 = DagHTjeoIg18027770;     DagHTjeoIg18027770 = DagHTjeoIg1586790;     DagHTjeoIg1586790 = DagHTjeoIg64261342;     DagHTjeoIg64261342 = DagHTjeoIg4471693;     DagHTjeoIg4471693 = DagHTjeoIg4677361;     DagHTjeoIg4677361 = DagHTjeoIg64234322;     DagHTjeoIg64234322 = DagHTjeoIg82506006;     DagHTjeoIg82506006 = DagHTjeoIg73362572;     DagHTjeoIg73362572 = DagHTjeoIg91485254;     DagHTjeoIg91485254 = DagHTjeoIg71808855;     DagHTjeoIg71808855 = DagHTjeoIg46220379;     DagHTjeoIg46220379 = DagHTjeoIg12780389;     DagHTjeoIg12780389 = DagHTjeoIg10545324;     DagHTjeoIg10545324 = DagHTjeoIg5593815;     DagHTjeoIg5593815 = DagHTjeoIg42332026;     DagHTjeoIg42332026 = DagHTjeoIg26825613;     DagHTjeoIg26825613 = DagHTjeoIg24065069;     DagHTjeoIg24065069 = DagHTjeoIg12121546;     DagHTjeoIg12121546 = DagHTjeoIg74341506;     DagHTjeoIg74341506 = DagHTjeoIg9231344;     DagHTjeoIg9231344 = DagHTjeoIg27103383;     DagHTjeoIg27103383 = DagHTjeoIg22863418;     DagHTjeoIg22863418 = DagHTjeoIg45007179;     DagHTjeoIg45007179 = DagHTjeoIg82429163;     DagHTjeoIg82429163 = DagHTjeoIg15735190;     DagHTjeoIg15735190 = DagHTjeoIg30502623;     DagHTjeoIg30502623 = DagHTjeoIg89561184;     DagHTjeoIg89561184 = DagHTjeoIg42687845;     DagHTjeoIg42687845 = DagHTjeoIg44877689;     DagHTjeoIg44877689 = DagHTjeoIg15444280;     DagHTjeoIg15444280 = DagHTjeoIg60051994;     DagHTjeoIg60051994 = DagHTjeoIg59133092;     DagHTjeoIg59133092 = DagHTjeoIg17183515;     DagHTjeoIg17183515 = DagHTjeoIg80397320;     DagHTjeoIg80397320 = DagHTjeoIg78531789;     DagHTjeoIg78531789 = DagHTjeoIg76470595;     DagHTjeoIg76470595 = DagHTjeoIg26193093;     DagHTjeoIg26193093 = DagHTjeoIg64327085;     DagHTjeoIg64327085 = DagHTjeoIg719674;     DagHTjeoIg719674 = DagHTjeoIg13398850;     DagHTjeoIg13398850 = DagHTjeoIg89202589;     DagHTjeoIg89202589 = DagHTjeoIg6438883;     DagHTjeoIg6438883 = DagHTjeoIg94455703;     DagHTjeoIg94455703 = DagHTjeoIg71536539;     DagHTjeoIg71536539 = DagHTjeoIg47844510;     DagHTjeoIg47844510 = DagHTjeoIg20882895;     DagHTjeoIg20882895 = DagHTjeoIg74104408;     DagHTjeoIg74104408 = DagHTjeoIg79572870;     DagHTjeoIg79572870 = DagHTjeoIg45311778;     DagHTjeoIg45311778 = DagHTjeoIg18322976;     DagHTjeoIg18322976 = DagHTjeoIg63898807;     DagHTjeoIg63898807 = DagHTjeoIg12837655;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void AiULMQCvvY75607165() {     int jlIDMHYCiH35224773 = -813524278;    int jlIDMHYCiH47736350 = -962739379;    int jlIDMHYCiH23662689 = -954082747;    int jlIDMHYCiH42705419 = -401344574;    int jlIDMHYCiH76578656 = -544249128;    int jlIDMHYCiH80039373 = -238232665;    int jlIDMHYCiH65207489 = 78166656;    int jlIDMHYCiH77787232 = -494495117;    int jlIDMHYCiH17552542 = -68273925;    int jlIDMHYCiH9043632 = 78663186;    int jlIDMHYCiH30405706 = -558624623;    int jlIDMHYCiH73421528 = -937411572;    int jlIDMHYCiH18982859 = -782266603;    int jlIDMHYCiH23554692 = -298383280;    int jlIDMHYCiH43640115 = -769517549;    int jlIDMHYCiH53060287 = -330101936;    int jlIDMHYCiH38175926 = -945651783;    int jlIDMHYCiH82470469 = -54060422;    int jlIDMHYCiH90680104 = -410626390;    int jlIDMHYCiH93668637 = -145676736;    int jlIDMHYCiH75610975 = -334369494;    int jlIDMHYCiH94146867 = -279345721;    int jlIDMHYCiH93319600 = -505711365;    int jlIDMHYCiH29907943 = -243676566;    int jlIDMHYCiH91430121 = -394249629;    int jlIDMHYCiH20487018 = -31409363;    int jlIDMHYCiH99465287 = 50590538;    int jlIDMHYCiH68417876 = -408023271;    int jlIDMHYCiH3452121 = -300881880;    int jlIDMHYCiH87214564 = -299915506;    int jlIDMHYCiH34745240 = -971444697;    int jlIDMHYCiH26339969 = -580934625;    int jlIDMHYCiH86435704 = -137241656;    int jlIDMHYCiH37021859 = -757126646;    int jlIDMHYCiH47937961 = -224064175;    int jlIDMHYCiH21494325 = -650735044;    int jlIDMHYCiH79380669 = -829500383;    int jlIDMHYCiH96971496 = -588317908;    int jlIDMHYCiH42373021 = -484004220;    int jlIDMHYCiH53848998 = -917423195;    int jlIDMHYCiH13407103 = 84792392;    int jlIDMHYCiH73675075 = 73587722;    int jlIDMHYCiH93193996 = -595536288;    int jlIDMHYCiH44592708 = -571380827;    int jlIDMHYCiH55947142 = -265914770;    int jlIDMHYCiH3938163 = -324735105;    int jlIDMHYCiH85348329 = -275848237;    int jlIDMHYCiH79791027 = -98958532;    int jlIDMHYCiH76569011 = -281387251;    int jlIDMHYCiH15111516 = -863874021;    int jlIDMHYCiH10456006 = -871555829;    int jlIDMHYCiH38112599 = -853199982;    int jlIDMHYCiH82786095 = -897957406;    int jlIDMHYCiH64963905 = 18914208;    int jlIDMHYCiH99996189 = 5064944;    int jlIDMHYCiH41077905 = -434178558;    int jlIDMHYCiH54416750 = -357028015;    int jlIDMHYCiH93754745 = -610406181;    int jlIDMHYCiH51275297 = 92905055;    int jlIDMHYCiH56091639 = -412839765;    int jlIDMHYCiH80574085 = -188823203;    int jlIDMHYCiH96789612 = -513810074;    int jlIDMHYCiH74335112 = -93613238;    int jlIDMHYCiH30337978 = -768358419;    int jlIDMHYCiH74298392 = 50107883;    int jlIDMHYCiH4065738 = -977689998;    int jlIDMHYCiH86985824 = -700169917;    int jlIDMHYCiH81960999 = 74860042;    int jlIDMHYCiH75616731 = 25680894;    int jlIDMHYCiH22145791 = -18782505;    int jlIDMHYCiH73679618 = -500601553;    int jlIDMHYCiH41204430 = -257333876;    int jlIDMHYCiH40097448 = -570056203;    int jlIDMHYCiH36831106 = -493203195;    int jlIDMHYCiH80261535 = -130469128;    int jlIDMHYCiH1935901 = -307957216;    int jlIDMHYCiH952871 = -683809433;    int jlIDMHYCiH48726893 = -934330538;    int jlIDMHYCiH73960800 = -977761796;    int jlIDMHYCiH87491958 = 30485476;    int jlIDMHYCiH35138689 = -755561126;    int jlIDMHYCiH19674261 = -850450931;    int jlIDMHYCiH91848865 = -26636021;    int jlIDMHYCiH88340604 = -437007859;    int jlIDMHYCiH76758558 = -428359678;    int jlIDMHYCiH96632640 = -18244715;    int jlIDMHYCiH43553873 = -682977220;    int jlIDMHYCiH21471799 = -56155864;    int jlIDMHYCiH37025669 = -662191590;    int jlIDMHYCiH6860056 = -789885618;    int jlIDMHYCiH67077575 = -193707030;    int jlIDMHYCiH85625923 = -119094203;    int jlIDMHYCiH45696199 = -581222963;    int jlIDMHYCiH86281382 = 28835545;    int jlIDMHYCiH73274913 = -628599992;    int jlIDMHYCiH16617490 = -401397535;    int jlIDMHYCiH99339963 = -832799041;    int jlIDMHYCiH62856019 = -827177869;    int jlIDMHYCiH70294316 = -521488710;    int jlIDMHYCiH51881405 = -813524278;     jlIDMHYCiH35224773 = jlIDMHYCiH47736350;     jlIDMHYCiH47736350 = jlIDMHYCiH23662689;     jlIDMHYCiH23662689 = jlIDMHYCiH42705419;     jlIDMHYCiH42705419 = jlIDMHYCiH76578656;     jlIDMHYCiH76578656 = jlIDMHYCiH80039373;     jlIDMHYCiH80039373 = jlIDMHYCiH65207489;     jlIDMHYCiH65207489 = jlIDMHYCiH77787232;     jlIDMHYCiH77787232 = jlIDMHYCiH17552542;     jlIDMHYCiH17552542 = jlIDMHYCiH9043632;     jlIDMHYCiH9043632 = jlIDMHYCiH30405706;     jlIDMHYCiH30405706 = jlIDMHYCiH73421528;     jlIDMHYCiH73421528 = jlIDMHYCiH18982859;     jlIDMHYCiH18982859 = jlIDMHYCiH23554692;     jlIDMHYCiH23554692 = jlIDMHYCiH43640115;     jlIDMHYCiH43640115 = jlIDMHYCiH53060287;     jlIDMHYCiH53060287 = jlIDMHYCiH38175926;     jlIDMHYCiH38175926 = jlIDMHYCiH82470469;     jlIDMHYCiH82470469 = jlIDMHYCiH90680104;     jlIDMHYCiH90680104 = jlIDMHYCiH93668637;     jlIDMHYCiH93668637 = jlIDMHYCiH75610975;     jlIDMHYCiH75610975 = jlIDMHYCiH94146867;     jlIDMHYCiH94146867 = jlIDMHYCiH93319600;     jlIDMHYCiH93319600 = jlIDMHYCiH29907943;     jlIDMHYCiH29907943 = jlIDMHYCiH91430121;     jlIDMHYCiH91430121 = jlIDMHYCiH20487018;     jlIDMHYCiH20487018 = jlIDMHYCiH99465287;     jlIDMHYCiH99465287 = jlIDMHYCiH68417876;     jlIDMHYCiH68417876 = jlIDMHYCiH3452121;     jlIDMHYCiH3452121 = jlIDMHYCiH87214564;     jlIDMHYCiH87214564 = jlIDMHYCiH34745240;     jlIDMHYCiH34745240 = jlIDMHYCiH26339969;     jlIDMHYCiH26339969 = jlIDMHYCiH86435704;     jlIDMHYCiH86435704 = jlIDMHYCiH37021859;     jlIDMHYCiH37021859 = jlIDMHYCiH47937961;     jlIDMHYCiH47937961 = jlIDMHYCiH21494325;     jlIDMHYCiH21494325 = jlIDMHYCiH79380669;     jlIDMHYCiH79380669 = jlIDMHYCiH96971496;     jlIDMHYCiH96971496 = jlIDMHYCiH42373021;     jlIDMHYCiH42373021 = jlIDMHYCiH53848998;     jlIDMHYCiH53848998 = jlIDMHYCiH13407103;     jlIDMHYCiH13407103 = jlIDMHYCiH73675075;     jlIDMHYCiH73675075 = jlIDMHYCiH93193996;     jlIDMHYCiH93193996 = jlIDMHYCiH44592708;     jlIDMHYCiH44592708 = jlIDMHYCiH55947142;     jlIDMHYCiH55947142 = jlIDMHYCiH3938163;     jlIDMHYCiH3938163 = jlIDMHYCiH85348329;     jlIDMHYCiH85348329 = jlIDMHYCiH79791027;     jlIDMHYCiH79791027 = jlIDMHYCiH76569011;     jlIDMHYCiH76569011 = jlIDMHYCiH15111516;     jlIDMHYCiH15111516 = jlIDMHYCiH10456006;     jlIDMHYCiH10456006 = jlIDMHYCiH38112599;     jlIDMHYCiH38112599 = jlIDMHYCiH82786095;     jlIDMHYCiH82786095 = jlIDMHYCiH64963905;     jlIDMHYCiH64963905 = jlIDMHYCiH99996189;     jlIDMHYCiH99996189 = jlIDMHYCiH41077905;     jlIDMHYCiH41077905 = jlIDMHYCiH54416750;     jlIDMHYCiH54416750 = jlIDMHYCiH93754745;     jlIDMHYCiH93754745 = jlIDMHYCiH51275297;     jlIDMHYCiH51275297 = jlIDMHYCiH56091639;     jlIDMHYCiH56091639 = jlIDMHYCiH80574085;     jlIDMHYCiH80574085 = jlIDMHYCiH96789612;     jlIDMHYCiH96789612 = jlIDMHYCiH74335112;     jlIDMHYCiH74335112 = jlIDMHYCiH30337978;     jlIDMHYCiH30337978 = jlIDMHYCiH74298392;     jlIDMHYCiH74298392 = jlIDMHYCiH4065738;     jlIDMHYCiH4065738 = jlIDMHYCiH86985824;     jlIDMHYCiH86985824 = jlIDMHYCiH81960999;     jlIDMHYCiH81960999 = jlIDMHYCiH75616731;     jlIDMHYCiH75616731 = jlIDMHYCiH22145791;     jlIDMHYCiH22145791 = jlIDMHYCiH73679618;     jlIDMHYCiH73679618 = jlIDMHYCiH41204430;     jlIDMHYCiH41204430 = jlIDMHYCiH40097448;     jlIDMHYCiH40097448 = jlIDMHYCiH36831106;     jlIDMHYCiH36831106 = jlIDMHYCiH80261535;     jlIDMHYCiH80261535 = jlIDMHYCiH1935901;     jlIDMHYCiH1935901 = jlIDMHYCiH952871;     jlIDMHYCiH952871 = jlIDMHYCiH48726893;     jlIDMHYCiH48726893 = jlIDMHYCiH73960800;     jlIDMHYCiH73960800 = jlIDMHYCiH87491958;     jlIDMHYCiH87491958 = jlIDMHYCiH35138689;     jlIDMHYCiH35138689 = jlIDMHYCiH19674261;     jlIDMHYCiH19674261 = jlIDMHYCiH91848865;     jlIDMHYCiH91848865 = jlIDMHYCiH88340604;     jlIDMHYCiH88340604 = jlIDMHYCiH76758558;     jlIDMHYCiH76758558 = jlIDMHYCiH96632640;     jlIDMHYCiH96632640 = jlIDMHYCiH43553873;     jlIDMHYCiH43553873 = jlIDMHYCiH21471799;     jlIDMHYCiH21471799 = jlIDMHYCiH37025669;     jlIDMHYCiH37025669 = jlIDMHYCiH6860056;     jlIDMHYCiH6860056 = jlIDMHYCiH67077575;     jlIDMHYCiH67077575 = jlIDMHYCiH85625923;     jlIDMHYCiH85625923 = jlIDMHYCiH45696199;     jlIDMHYCiH45696199 = jlIDMHYCiH86281382;     jlIDMHYCiH86281382 = jlIDMHYCiH73274913;     jlIDMHYCiH73274913 = jlIDMHYCiH16617490;     jlIDMHYCiH16617490 = jlIDMHYCiH99339963;     jlIDMHYCiH99339963 = jlIDMHYCiH62856019;     jlIDMHYCiH62856019 = jlIDMHYCiH70294316;     jlIDMHYCiH70294316 = jlIDMHYCiH51881405;     jlIDMHYCiH51881405 = jlIDMHYCiH35224773;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void JFEbQRuaon12252140() {     int ZkgqGkJjjS57611890 = -447928241;    int ZkgqGkJjjS32083851 = -911282434;    int ZkgqGkJjjS52431967 = -93736591;    int ZkgqGkJjjS57712107 = -407752105;    int ZkgqGkJjjS74752911 = -35306129;    int ZkgqGkJjjS94922490 = -98841043;    int ZkgqGkJjjS57363228 = -843820650;    int ZkgqGkJjjS53421091 = -965091761;    int ZkgqGkJjjS14715673 = 47530036;    int ZkgqGkJjjS807217 = -764078147;    int ZkgqGkJjjS67947566 = -555249023;    int ZkgqGkJjjS29324478 = -88156865;    int ZkgqGkJjjS85991039 = -300983923;    int ZkgqGkJjjS66169444 = -826092037;    int ZkgqGkJjjS80375392 = -396995262;    int ZkgqGkJjjS97316199 = -711769478;    int ZkgqGkJjjS87999277 = -122931603;    int ZkgqGkJjjS88446885 = -505803113;    int ZkgqGkJjjS31961735 = -545728515;    int ZkgqGkJjjS72184920 = -398311190;    int ZkgqGkJjjS76494082 = -325856802;    int ZkgqGkJjjS57962085 = -784112034;    int ZkgqGkJjjS96612923 = -681791957;    int ZkgqGkJjjS56407730 = -807669423;    int ZkgqGkJjjS26970369 = -15808753;    int ZkgqGkJjjS8790013 = -475303587;    int ZkgqGkJjjS46554708 = -806995955;    int ZkgqGkJjjS74329326 = -196477724;    int ZkgqGkJjjS10344682 = -645138999;    int ZkgqGkJjjS96371743 = -220211373;    int ZkgqGkJjjS79036044 = 47239151;    int ZkgqGkJjjS83881159 = -133005980;    int ZkgqGkJjjS67474374 = -666585150;    int ZkgqGkJjjS96410546 = 18746504;    int ZkgqGkJjjS24167325 = -287820598;    int ZkgqGkJjjS63187194 = -966130421;    int ZkgqGkJjjS72820381 = -562448335;    int ZkgqGkJjjS50597597 = -455943380;    int ZkgqGkJjjS90681152 = -885050252;    int ZkgqGkJjjS74034713 = 46352776;    int ZkgqGkJjjS42164473 = -418385274;    int ZkgqGkJjjS62183466 = -784320969;    int ZkgqGkJjjS98744188 = -483066872;    int ZkgqGkJjjS44036827 = -568744476;    int ZkgqGkJjjS23930409 = -75058248;    int ZkgqGkJjjS12038445 = -506964201;    int ZkgqGkJjjS97645726 = -854258490;    int ZkgqGkJjjS24389702 = -955493523;    int ZkgqGkJjjS71028916 = -231103949;    int ZkgqGkJjjS12195262 = 94105720;    int ZkgqGkJjjS19325222 = -126632106;    int ZkgqGkJjjS11963856 = -184019264;    int ZkgqGkJjjS61100498 = -569868636;    int ZkgqGkJjjS25250451 = -256766600;    int ZkgqGkJjjS35758057 = -135639172;    int ZkgqGkJjjS99649804 = -663816208;    int ZkgqGkJjjS35470927 = -129490478;    int ZkgqGkJjjS96024237 = -286067168;    int ZkgqGkJjjS30741739 = -291943353;    int ZkgqGkJjjS65962898 = -560002543;    int ZkgqGkJjjS48367782 = -291845088;    int ZkgqGkJjjS83033901 = -547342927;    int ZkgqGkJjjS43076410 = -219952763;    int ZkgqGkJjjS18343929 = -732258592;    int ZkgqGkJjjS21771172 = -711317298;    int ZkgqGkJjjS84066406 = -322243044;    int ZkgqGkJjjS61850103 = -421571715;    int ZkgqGkJjjS89580493 = -219730427;    int ZkgqGkJjjS42002119 = -438271440;    int ZkgqGkJjjS17188199 = -430864842;    int ZkgqGkJjjS24495819 = -49321144;    int ZkgqGkJjjS37401681 = -666988224;    int ZkgqGkJjjS97765733 = -620752862;    int ZkgqGkJjjS57927022 = -492081291;    int ZkgqGkJjjS30020447 = -979925916;    int ZkgqGkJjjS14310616 = -541535833;    int ZkgqGkJjjS59217897 = -201045162;    int ZkgqGkJjjS52576096 = -13047481;    int ZkgqGkJjjS32477321 = -632611176;    int ZkgqGkJjjS14931924 = -508844553;    int ZkgqGkJjjS11144286 = -621045097;    int ZkgqGkJjjS22165007 = -851502433;    int ZkgqGkJjjS3300411 = -965373776;    int ZkgqGkJjjS98149420 = -639244719;    int ZkgqGkJjjS77046521 = 6420732;    int ZkgqGkJjjS67072189 = -768741585;    int ZkgqGkJjjS22780661 = -563137344;    int ZkgqGkJjjS42223924 = -309818551;    int ZkgqGkJjjS60652489 = -845614325;    int ZkgqGkJjjS24517521 = -624004391;    int ZkgqGkJjjS27716267 = -736639944;    int ZkgqGkJjjS76796144 = -176381168;    int ZkgqGkJjjS19855859 = -64000028;    int ZkgqGkJjjS24718254 = -225047709;    int ZkgqGkJjjS25666931 = -661802136;    int ZkgqGkJjjS59130572 = -871042348;    int ZkgqGkJjjS19107057 = -464368207;    int ZkgqGkJjjS80400259 = -750808281;    int ZkgqGkJjjS22265656 = -857427178;    int ZkgqGkJjjS39864003 = -447928241;     ZkgqGkJjjS57611890 = ZkgqGkJjjS32083851;     ZkgqGkJjjS32083851 = ZkgqGkJjjS52431967;     ZkgqGkJjjS52431967 = ZkgqGkJjjS57712107;     ZkgqGkJjjS57712107 = ZkgqGkJjjS74752911;     ZkgqGkJjjS74752911 = ZkgqGkJjjS94922490;     ZkgqGkJjjS94922490 = ZkgqGkJjjS57363228;     ZkgqGkJjjS57363228 = ZkgqGkJjjS53421091;     ZkgqGkJjjS53421091 = ZkgqGkJjjS14715673;     ZkgqGkJjjS14715673 = ZkgqGkJjjS807217;     ZkgqGkJjjS807217 = ZkgqGkJjjS67947566;     ZkgqGkJjjS67947566 = ZkgqGkJjjS29324478;     ZkgqGkJjjS29324478 = ZkgqGkJjjS85991039;     ZkgqGkJjjS85991039 = ZkgqGkJjjS66169444;     ZkgqGkJjjS66169444 = ZkgqGkJjjS80375392;     ZkgqGkJjjS80375392 = ZkgqGkJjjS97316199;     ZkgqGkJjjS97316199 = ZkgqGkJjjS87999277;     ZkgqGkJjjS87999277 = ZkgqGkJjjS88446885;     ZkgqGkJjjS88446885 = ZkgqGkJjjS31961735;     ZkgqGkJjjS31961735 = ZkgqGkJjjS72184920;     ZkgqGkJjjS72184920 = ZkgqGkJjjS76494082;     ZkgqGkJjjS76494082 = ZkgqGkJjjS57962085;     ZkgqGkJjjS57962085 = ZkgqGkJjjS96612923;     ZkgqGkJjjS96612923 = ZkgqGkJjjS56407730;     ZkgqGkJjjS56407730 = ZkgqGkJjjS26970369;     ZkgqGkJjjS26970369 = ZkgqGkJjjS8790013;     ZkgqGkJjjS8790013 = ZkgqGkJjjS46554708;     ZkgqGkJjjS46554708 = ZkgqGkJjjS74329326;     ZkgqGkJjjS74329326 = ZkgqGkJjjS10344682;     ZkgqGkJjjS10344682 = ZkgqGkJjjS96371743;     ZkgqGkJjjS96371743 = ZkgqGkJjjS79036044;     ZkgqGkJjjS79036044 = ZkgqGkJjjS83881159;     ZkgqGkJjjS83881159 = ZkgqGkJjjS67474374;     ZkgqGkJjjS67474374 = ZkgqGkJjjS96410546;     ZkgqGkJjjS96410546 = ZkgqGkJjjS24167325;     ZkgqGkJjjS24167325 = ZkgqGkJjjS63187194;     ZkgqGkJjjS63187194 = ZkgqGkJjjS72820381;     ZkgqGkJjjS72820381 = ZkgqGkJjjS50597597;     ZkgqGkJjjS50597597 = ZkgqGkJjjS90681152;     ZkgqGkJjjS90681152 = ZkgqGkJjjS74034713;     ZkgqGkJjjS74034713 = ZkgqGkJjjS42164473;     ZkgqGkJjjS42164473 = ZkgqGkJjjS62183466;     ZkgqGkJjjS62183466 = ZkgqGkJjjS98744188;     ZkgqGkJjjS98744188 = ZkgqGkJjjS44036827;     ZkgqGkJjjS44036827 = ZkgqGkJjjS23930409;     ZkgqGkJjjS23930409 = ZkgqGkJjjS12038445;     ZkgqGkJjjS12038445 = ZkgqGkJjjS97645726;     ZkgqGkJjjS97645726 = ZkgqGkJjjS24389702;     ZkgqGkJjjS24389702 = ZkgqGkJjjS71028916;     ZkgqGkJjjS71028916 = ZkgqGkJjjS12195262;     ZkgqGkJjjS12195262 = ZkgqGkJjjS19325222;     ZkgqGkJjjS19325222 = ZkgqGkJjjS11963856;     ZkgqGkJjjS11963856 = ZkgqGkJjjS61100498;     ZkgqGkJjjS61100498 = ZkgqGkJjjS25250451;     ZkgqGkJjjS25250451 = ZkgqGkJjjS35758057;     ZkgqGkJjjS35758057 = ZkgqGkJjjS99649804;     ZkgqGkJjjS99649804 = ZkgqGkJjjS35470927;     ZkgqGkJjjS35470927 = ZkgqGkJjjS96024237;     ZkgqGkJjjS96024237 = ZkgqGkJjjS30741739;     ZkgqGkJjjS30741739 = ZkgqGkJjjS65962898;     ZkgqGkJjjS65962898 = ZkgqGkJjjS48367782;     ZkgqGkJjjS48367782 = ZkgqGkJjjS83033901;     ZkgqGkJjjS83033901 = ZkgqGkJjjS43076410;     ZkgqGkJjjS43076410 = ZkgqGkJjjS18343929;     ZkgqGkJjjS18343929 = ZkgqGkJjjS21771172;     ZkgqGkJjjS21771172 = ZkgqGkJjjS84066406;     ZkgqGkJjjS84066406 = ZkgqGkJjjS61850103;     ZkgqGkJjjS61850103 = ZkgqGkJjjS89580493;     ZkgqGkJjjS89580493 = ZkgqGkJjjS42002119;     ZkgqGkJjjS42002119 = ZkgqGkJjjS17188199;     ZkgqGkJjjS17188199 = ZkgqGkJjjS24495819;     ZkgqGkJjjS24495819 = ZkgqGkJjjS37401681;     ZkgqGkJjjS37401681 = ZkgqGkJjjS97765733;     ZkgqGkJjjS97765733 = ZkgqGkJjjS57927022;     ZkgqGkJjjS57927022 = ZkgqGkJjjS30020447;     ZkgqGkJjjS30020447 = ZkgqGkJjjS14310616;     ZkgqGkJjjS14310616 = ZkgqGkJjjS59217897;     ZkgqGkJjjS59217897 = ZkgqGkJjjS52576096;     ZkgqGkJjjS52576096 = ZkgqGkJjjS32477321;     ZkgqGkJjjS32477321 = ZkgqGkJjjS14931924;     ZkgqGkJjjS14931924 = ZkgqGkJjjS11144286;     ZkgqGkJjjS11144286 = ZkgqGkJjjS22165007;     ZkgqGkJjjS22165007 = ZkgqGkJjjS3300411;     ZkgqGkJjjS3300411 = ZkgqGkJjjS98149420;     ZkgqGkJjjS98149420 = ZkgqGkJjjS77046521;     ZkgqGkJjjS77046521 = ZkgqGkJjjS67072189;     ZkgqGkJjjS67072189 = ZkgqGkJjjS22780661;     ZkgqGkJjjS22780661 = ZkgqGkJjjS42223924;     ZkgqGkJjjS42223924 = ZkgqGkJjjS60652489;     ZkgqGkJjjS60652489 = ZkgqGkJjjS24517521;     ZkgqGkJjjS24517521 = ZkgqGkJjjS27716267;     ZkgqGkJjjS27716267 = ZkgqGkJjjS76796144;     ZkgqGkJjjS76796144 = ZkgqGkJjjS19855859;     ZkgqGkJjjS19855859 = ZkgqGkJjjS24718254;     ZkgqGkJjjS24718254 = ZkgqGkJjjS25666931;     ZkgqGkJjjS25666931 = ZkgqGkJjjS59130572;     ZkgqGkJjjS59130572 = ZkgqGkJjjS19107057;     ZkgqGkJjjS19107057 = ZkgqGkJjjS80400259;     ZkgqGkJjjS80400259 = ZkgqGkJjjS22265656;     ZkgqGkJjjS22265656 = ZkgqGkJjjS39864003;     ZkgqGkJjjS39864003 = ZkgqGkJjjS57611890;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void cBcwSXeMvn48897114() {     int vpRpaonvCL79999007 = -82332202;    int vpRpaonvCL16431352 = -859825482;    int vpRpaonvCL81201245 = -333390435;    int vpRpaonvCL72718795 = -414159640;    int vpRpaonvCL72927165 = -626363122;    int vpRpaonvCL9805607 = 40550579;    int vpRpaonvCL49518967 = -665807956;    int vpRpaonvCL29054950 = -335688390;    int vpRpaonvCL11878804 = -936666002;    int vpRpaonvCL92570801 = -506819479;    int vpRpaonvCL5489426 = -551873423;    int vpRpaonvCL85227426 = -338902149;    int vpRpaonvCL52999220 = -919701242;    int vpRpaonvCL8784197 = -253800793;    int vpRpaonvCL17110670 = -24472957;    int vpRpaonvCL41572112 = 6562979;    int vpRpaonvCL37822629 = -400211423;    int vpRpaonvCL94423301 = -957545799;    int vpRpaonvCL73243365 = -680830641;    int vpRpaonvCL50701204 = -650945643;    int vpRpaonvCL77377189 = -317344106;    int vpRpaonvCL21777304 = -188878347;    int vpRpaonvCL99906246 = -857872549;    int vpRpaonvCL82907517 = -271662281;    int vpRpaonvCL62510615 = -737367866;    int vpRpaonvCL97093006 = -919197815;    int vpRpaonvCL93644128 = -564582448;    int vpRpaonvCL80240776 = 15067828;    int vpRpaonvCL17237242 = -989396133;    int vpRpaonvCL5528923 = -140507240;    int vpRpaonvCL23326850 = -34077001;    int vpRpaonvCL41422350 = -785077338;    int vpRpaonvCL48513045 = -95928648;    int vpRpaonvCL55799233 = -305380346;    int vpRpaonvCL396690 = -351577024;    int vpRpaonvCL4880063 = -181525816;    int vpRpaonvCL66260094 = -295396286;    int vpRpaonvCL4223698 = -323568849;    int vpRpaonvCL38989284 = -186096296;    int vpRpaonvCL94220427 = -89871253;    int vpRpaonvCL70921844 = -921562940;    int vpRpaonvCL50691858 = -542229681;    int vpRpaonvCL4294381 = -370597458;    int vpRpaonvCL43480946 = -566108125;    int vpRpaonvCL91913675 = -984201727;    int vpRpaonvCL20138727 = -689193314;    int vpRpaonvCL9943125 = -332668743;    int vpRpaonvCL68988376 = -712028515;    int vpRpaonvCL65488821 = -180820672;    int vpRpaonvCL9279008 = -47914534;    int vpRpaonvCL28194439 = -481708383;    int vpRpaonvCL85815113 = -614838550;    int vpRpaonvCL39414902 = -241779863;    int vpRpaonvCL85536995 = -532447409;    int vpRpaonvCL71519925 = -276343292;    int vpRpaonvCL58221703 = -893453856;    int vpRpaonvCL16525105 = 98047066;    int vpRpaonvCL98293728 = 38271845;    int vpRpaonvCL10208181 = -676791775;    int vpRpaonvCL75834159 = -707165308;    int vpRpaonvCL16161479 = -394866973;    int vpRpaonvCL69278190 = -580875785;    int vpRpaonvCL11817708 = -346292257;    int vpRpaonvCL6349881 = -696158763;    int vpRpaonvCL69243951 = -372742479;    int vpRpaonvCL64067075 = -766796086;    int vpRpaonvCL36714381 = -142973501;    int vpRpaonvCL97199986 = -514320897;    int vpRpaonvCL8387507 = -902223770;    int vpRpaonvCL12230607 = -842947141;    int vpRpaonvCL75312018 = -698040735;    int vpRpaonvCL33598932 = 23357425;    int vpRpaonvCL55434018 = -671449504;    int vpRpaonvCL79022937 = -490959388;    int vpRpaonvCL79779359 = -729382703;    int vpRpaonvCL26685332 = -775114425;    int vpRpaonvCL17482924 = -818280890;    int vpRpaonvCL56425300 = -191764424;    int vpRpaonvCL90993841 = -287460554;    int vpRpaonvCL42371889 = 51825448;    int vpRpaonvCL87149882 = -486529072;    int vpRpaonvCL24655753 = -852553934;    int vpRpaonvCL14751956 = -804111500;    int vpRpaonvCL7958235 = -841481600;    int vpRpaonvCL77334484 = -658798857;    int vpRpaonvCL37511737 = -419238451;    int vpRpaonvCL2007449 = -443297475;    int vpRpaonvCL62976049 = -563481240;    int vpRpaonvCL84279308 = 70962945;    int vpRpaonvCL42174987 = -458123168;    int vpRpaonvCL88354958 = -179572882;    int vpRpaonvCL67966365 = -233668132;    int vpRpaonvCL94015517 = -646777074;    int vpRpaonvCL63155125 = -478930988;    int vpRpaonvCL78058949 = -695004280;    int vpRpaonvCL1643655 = -240687156;    int vpRpaonvCL38874150 = -95937424;    int vpRpaonvCL97944499 = -674438695;    int vpRpaonvCL74236995 = -93365646;    int vpRpaonvCL27846601 = -82332202;     vpRpaonvCL79999007 = vpRpaonvCL16431352;     vpRpaonvCL16431352 = vpRpaonvCL81201245;     vpRpaonvCL81201245 = vpRpaonvCL72718795;     vpRpaonvCL72718795 = vpRpaonvCL72927165;     vpRpaonvCL72927165 = vpRpaonvCL9805607;     vpRpaonvCL9805607 = vpRpaonvCL49518967;     vpRpaonvCL49518967 = vpRpaonvCL29054950;     vpRpaonvCL29054950 = vpRpaonvCL11878804;     vpRpaonvCL11878804 = vpRpaonvCL92570801;     vpRpaonvCL92570801 = vpRpaonvCL5489426;     vpRpaonvCL5489426 = vpRpaonvCL85227426;     vpRpaonvCL85227426 = vpRpaonvCL52999220;     vpRpaonvCL52999220 = vpRpaonvCL8784197;     vpRpaonvCL8784197 = vpRpaonvCL17110670;     vpRpaonvCL17110670 = vpRpaonvCL41572112;     vpRpaonvCL41572112 = vpRpaonvCL37822629;     vpRpaonvCL37822629 = vpRpaonvCL94423301;     vpRpaonvCL94423301 = vpRpaonvCL73243365;     vpRpaonvCL73243365 = vpRpaonvCL50701204;     vpRpaonvCL50701204 = vpRpaonvCL77377189;     vpRpaonvCL77377189 = vpRpaonvCL21777304;     vpRpaonvCL21777304 = vpRpaonvCL99906246;     vpRpaonvCL99906246 = vpRpaonvCL82907517;     vpRpaonvCL82907517 = vpRpaonvCL62510615;     vpRpaonvCL62510615 = vpRpaonvCL97093006;     vpRpaonvCL97093006 = vpRpaonvCL93644128;     vpRpaonvCL93644128 = vpRpaonvCL80240776;     vpRpaonvCL80240776 = vpRpaonvCL17237242;     vpRpaonvCL17237242 = vpRpaonvCL5528923;     vpRpaonvCL5528923 = vpRpaonvCL23326850;     vpRpaonvCL23326850 = vpRpaonvCL41422350;     vpRpaonvCL41422350 = vpRpaonvCL48513045;     vpRpaonvCL48513045 = vpRpaonvCL55799233;     vpRpaonvCL55799233 = vpRpaonvCL396690;     vpRpaonvCL396690 = vpRpaonvCL4880063;     vpRpaonvCL4880063 = vpRpaonvCL66260094;     vpRpaonvCL66260094 = vpRpaonvCL4223698;     vpRpaonvCL4223698 = vpRpaonvCL38989284;     vpRpaonvCL38989284 = vpRpaonvCL94220427;     vpRpaonvCL94220427 = vpRpaonvCL70921844;     vpRpaonvCL70921844 = vpRpaonvCL50691858;     vpRpaonvCL50691858 = vpRpaonvCL4294381;     vpRpaonvCL4294381 = vpRpaonvCL43480946;     vpRpaonvCL43480946 = vpRpaonvCL91913675;     vpRpaonvCL91913675 = vpRpaonvCL20138727;     vpRpaonvCL20138727 = vpRpaonvCL9943125;     vpRpaonvCL9943125 = vpRpaonvCL68988376;     vpRpaonvCL68988376 = vpRpaonvCL65488821;     vpRpaonvCL65488821 = vpRpaonvCL9279008;     vpRpaonvCL9279008 = vpRpaonvCL28194439;     vpRpaonvCL28194439 = vpRpaonvCL85815113;     vpRpaonvCL85815113 = vpRpaonvCL39414902;     vpRpaonvCL39414902 = vpRpaonvCL85536995;     vpRpaonvCL85536995 = vpRpaonvCL71519925;     vpRpaonvCL71519925 = vpRpaonvCL58221703;     vpRpaonvCL58221703 = vpRpaonvCL16525105;     vpRpaonvCL16525105 = vpRpaonvCL98293728;     vpRpaonvCL98293728 = vpRpaonvCL10208181;     vpRpaonvCL10208181 = vpRpaonvCL75834159;     vpRpaonvCL75834159 = vpRpaonvCL16161479;     vpRpaonvCL16161479 = vpRpaonvCL69278190;     vpRpaonvCL69278190 = vpRpaonvCL11817708;     vpRpaonvCL11817708 = vpRpaonvCL6349881;     vpRpaonvCL6349881 = vpRpaonvCL69243951;     vpRpaonvCL69243951 = vpRpaonvCL64067075;     vpRpaonvCL64067075 = vpRpaonvCL36714381;     vpRpaonvCL36714381 = vpRpaonvCL97199986;     vpRpaonvCL97199986 = vpRpaonvCL8387507;     vpRpaonvCL8387507 = vpRpaonvCL12230607;     vpRpaonvCL12230607 = vpRpaonvCL75312018;     vpRpaonvCL75312018 = vpRpaonvCL33598932;     vpRpaonvCL33598932 = vpRpaonvCL55434018;     vpRpaonvCL55434018 = vpRpaonvCL79022937;     vpRpaonvCL79022937 = vpRpaonvCL79779359;     vpRpaonvCL79779359 = vpRpaonvCL26685332;     vpRpaonvCL26685332 = vpRpaonvCL17482924;     vpRpaonvCL17482924 = vpRpaonvCL56425300;     vpRpaonvCL56425300 = vpRpaonvCL90993841;     vpRpaonvCL90993841 = vpRpaonvCL42371889;     vpRpaonvCL42371889 = vpRpaonvCL87149882;     vpRpaonvCL87149882 = vpRpaonvCL24655753;     vpRpaonvCL24655753 = vpRpaonvCL14751956;     vpRpaonvCL14751956 = vpRpaonvCL7958235;     vpRpaonvCL7958235 = vpRpaonvCL77334484;     vpRpaonvCL77334484 = vpRpaonvCL37511737;     vpRpaonvCL37511737 = vpRpaonvCL2007449;     vpRpaonvCL2007449 = vpRpaonvCL62976049;     vpRpaonvCL62976049 = vpRpaonvCL84279308;     vpRpaonvCL84279308 = vpRpaonvCL42174987;     vpRpaonvCL42174987 = vpRpaonvCL88354958;     vpRpaonvCL88354958 = vpRpaonvCL67966365;     vpRpaonvCL67966365 = vpRpaonvCL94015517;     vpRpaonvCL94015517 = vpRpaonvCL63155125;     vpRpaonvCL63155125 = vpRpaonvCL78058949;     vpRpaonvCL78058949 = vpRpaonvCL1643655;     vpRpaonvCL1643655 = vpRpaonvCL38874150;     vpRpaonvCL38874150 = vpRpaonvCL97944499;     vpRpaonvCL97944499 = vpRpaonvCL74236995;     vpRpaonvCL74236995 = vpRpaonvCL27846601;     vpRpaonvCL27846601 = vpRpaonvCL79999007;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void RjrAPfRwsL85542089() {     int BVSzYdGjQT2386125 = -816736167;    int BVSzYdGjQT778852 = -808368543;    int BVSzYdGjQT9970525 = -573044279;    int BVSzYdGjQT87725484 = -420567169;    int BVSzYdGjQT71101419 = -117420132;    int BVSzYdGjQT24688724 = -920057799;    int BVSzYdGjQT41674706 = -487795263;    int BVSzYdGjQT4688809 = -806285049;    int BVSzYdGjQT9041935 = -820862042;    int BVSzYdGjQT84334385 = -249560812;    int BVSzYdGjQT43031285 = -548497825;    int BVSzYdGjQT41130376 = -589647452;    int BVSzYdGjQT20007401 = -438418562;    int BVSzYdGjQT51398948 = -781509551;    int BVSzYdGjQT53845946 = -751950688;    int BVSzYdGjQT85828025 = -375104563;    int BVSzYdGjQT87645981 = -677491243;    int BVSzYdGjQT399719 = -309288494;    int BVSzYdGjQT14524997 = -815932765;    int BVSzYdGjQT29217487 = -903580097;    int BVSzYdGjQT78260297 = -308831419;    int BVSzYdGjQT85592522 = -693644660;    int BVSzYdGjQT3199570 = 66046859;    int BVSzYdGjQT9407305 = -835655138;    int BVSzYdGjQT98050862 = -358927002;    int BVSzYdGjQT85396001 = -263092035;    int BVSzYdGjQT40733549 = -322168941;    int BVSzYdGjQT86152227 = -873386631;    int BVSzYdGjQT24129803 = -233653237;    int BVSzYdGjQT14686103 = -60803106;    int BVSzYdGjQT67617655 = -115393153;    int BVSzYdGjQT98963541 = -337148689;    int BVSzYdGjQT29551716 = -625272140;    int BVSzYdGjQT15187921 = -629507196;    int BVSzYdGjQT76626054 = -415333443;    int BVSzYdGjQT46572932 = -496921174;    int BVSzYdGjQT59699806 = -28344238;    int BVSzYdGjQT57849799 = -191194324;    int BVSzYdGjQT87297415 = -587142315;    int BVSzYdGjQT14406142 = -226095283;    int BVSzYdGjQT99679214 = -324740607;    int BVSzYdGjQT39200249 = -300138351;    int BVSzYdGjQT9844573 = -258128039;    int BVSzYdGjQT42925066 = -563471774;    int BVSzYdGjQT59896942 = -793345203;    int BVSzYdGjQT28239009 = -871422391;    int BVSzYdGjQT22240521 = -911078996;    int BVSzYdGjQT13587051 = -468563504;    int BVSzYdGjQT59948726 = -130537345;    int BVSzYdGjQT6362753 = -189934798;    int BVSzYdGjQT37063655 = -836784660;    int BVSzYdGjQT59666370 = 54342172;    int BVSzYdGjQT17729305 = 86308903;    int BVSzYdGjQT45823541 = -808128218;    int BVSzYdGjQT7281793 = -417047402;    int BVSzYdGjQT16793603 = -23091507;    int BVSzYdGjQT97579281 = -774415403;    int BVSzYdGjQT563221 = -737389141;    int BVSzYdGjQT89674621 = 38359833;    int BVSzYdGjQT85705417 = -854328097;    int BVSzYdGjQT83955174 = -497888859;    int BVSzYdGjQT55522479 = -614408632;    int BVSzYdGjQT80559005 = -472631813;    int BVSzYdGjQT94355832 = -660058937;    int BVSzYdGjQT16716731 = -34167660;    int BVSzYdGjQT44067744 = -111349136;    int BVSzYdGjQT11578660 = -964375312;    int BVSzYdGjQT4819480 = -808911366;    int BVSzYdGjQT74772894 = -266176109;    int BVSzYdGjQT7273015 = -155029515;    int BVSzYdGjQT26128219 = -246760326;    int BVSzYdGjQT29796182 = -386296920;    int BVSzYdGjQT13102304 = -722146180;    int BVSzYdGjQT118855 = -489837483;    int BVSzYdGjQT29538272 = -478839491;    int BVSzYdGjQT39060048 = 91306932;    int BVSzYdGjQT75747950 = -335516622;    int BVSzYdGjQT60274504 = -370481367;    int BVSzYdGjQT49510362 = 57690065;    int BVSzYdGjQT69811854 = -487504611;    int BVSzYdGjQT63155480 = -352013039;    int BVSzYdGjQT27146499 = -853605437;    int BVSzYdGjQT26203501 = -642849286;    int BVSzYdGjQT17767051 = 56281561;    int BVSzYdGjQT77622447 = -224018447;    int BVSzYdGjQT7951285 = -69735325;    int BVSzYdGjQT81234236 = -323457593;    int BVSzYdGjQT83728174 = -817143923;    int BVSzYdGjQT7906129 = -112459795;    int BVSzYdGjQT59832452 = -292241936;    int BVSzYdGjQT48993651 = -722505771;    int BVSzYdGjQT59136586 = -290955097;    int BVSzYdGjQT68175177 = -129554158;    int BVSzYdGjQT1591998 = -732814218;    int BVSzYdGjQT30450967 = -728206425;    int BVSzYdGjQT44156736 = -710331975;    int BVSzYdGjQT58641243 = -827506539;    int BVSzYdGjQT15488740 = -598069103;    int BVSzYdGjQT26208335 = -429304114;    int BVSzYdGjQT15829199 = -816736167;     BVSzYdGjQT2386125 = BVSzYdGjQT778852;     BVSzYdGjQT778852 = BVSzYdGjQT9970525;     BVSzYdGjQT9970525 = BVSzYdGjQT87725484;     BVSzYdGjQT87725484 = BVSzYdGjQT71101419;     BVSzYdGjQT71101419 = BVSzYdGjQT24688724;     BVSzYdGjQT24688724 = BVSzYdGjQT41674706;     BVSzYdGjQT41674706 = BVSzYdGjQT4688809;     BVSzYdGjQT4688809 = BVSzYdGjQT9041935;     BVSzYdGjQT9041935 = BVSzYdGjQT84334385;     BVSzYdGjQT84334385 = BVSzYdGjQT43031285;     BVSzYdGjQT43031285 = BVSzYdGjQT41130376;     BVSzYdGjQT41130376 = BVSzYdGjQT20007401;     BVSzYdGjQT20007401 = BVSzYdGjQT51398948;     BVSzYdGjQT51398948 = BVSzYdGjQT53845946;     BVSzYdGjQT53845946 = BVSzYdGjQT85828025;     BVSzYdGjQT85828025 = BVSzYdGjQT87645981;     BVSzYdGjQT87645981 = BVSzYdGjQT399719;     BVSzYdGjQT399719 = BVSzYdGjQT14524997;     BVSzYdGjQT14524997 = BVSzYdGjQT29217487;     BVSzYdGjQT29217487 = BVSzYdGjQT78260297;     BVSzYdGjQT78260297 = BVSzYdGjQT85592522;     BVSzYdGjQT85592522 = BVSzYdGjQT3199570;     BVSzYdGjQT3199570 = BVSzYdGjQT9407305;     BVSzYdGjQT9407305 = BVSzYdGjQT98050862;     BVSzYdGjQT98050862 = BVSzYdGjQT85396001;     BVSzYdGjQT85396001 = BVSzYdGjQT40733549;     BVSzYdGjQT40733549 = BVSzYdGjQT86152227;     BVSzYdGjQT86152227 = BVSzYdGjQT24129803;     BVSzYdGjQT24129803 = BVSzYdGjQT14686103;     BVSzYdGjQT14686103 = BVSzYdGjQT67617655;     BVSzYdGjQT67617655 = BVSzYdGjQT98963541;     BVSzYdGjQT98963541 = BVSzYdGjQT29551716;     BVSzYdGjQT29551716 = BVSzYdGjQT15187921;     BVSzYdGjQT15187921 = BVSzYdGjQT76626054;     BVSzYdGjQT76626054 = BVSzYdGjQT46572932;     BVSzYdGjQT46572932 = BVSzYdGjQT59699806;     BVSzYdGjQT59699806 = BVSzYdGjQT57849799;     BVSzYdGjQT57849799 = BVSzYdGjQT87297415;     BVSzYdGjQT87297415 = BVSzYdGjQT14406142;     BVSzYdGjQT14406142 = BVSzYdGjQT99679214;     BVSzYdGjQT99679214 = BVSzYdGjQT39200249;     BVSzYdGjQT39200249 = BVSzYdGjQT9844573;     BVSzYdGjQT9844573 = BVSzYdGjQT42925066;     BVSzYdGjQT42925066 = BVSzYdGjQT59896942;     BVSzYdGjQT59896942 = BVSzYdGjQT28239009;     BVSzYdGjQT28239009 = BVSzYdGjQT22240521;     BVSzYdGjQT22240521 = BVSzYdGjQT13587051;     BVSzYdGjQT13587051 = BVSzYdGjQT59948726;     BVSzYdGjQT59948726 = BVSzYdGjQT6362753;     BVSzYdGjQT6362753 = BVSzYdGjQT37063655;     BVSzYdGjQT37063655 = BVSzYdGjQT59666370;     BVSzYdGjQT59666370 = BVSzYdGjQT17729305;     BVSzYdGjQT17729305 = BVSzYdGjQT45823541;     BVSzYdGjQT45823541 = BVSzYdGjQT7281793;     BVSzYdGjQT7281793 = BVSzYdGjQT16793603;     BVSzYdGjQT16793603 = BVSzYdGjQT97579281;     BVSzYdGjQT97579281 = BVSzYdGjQT563221;     BVSzYdGjQT563221 = BVSzYdGjQT89674621;     BVSzYdGjQT89674621 = BVSzYdGjQT85705417;     BVSzYdGjQT85705417 = BVSzYdGjQT83955174;     BVSzYdGjQT83955174 = BVSzYdGjQT55522479;     BVSzYdGjQT55522479 = BVSzYdGjQT80559005;     BVSzYdGjQT80559005 = BVSzYdGjQT94355832;     BVSzYdGjQT94355832 = BVSzYdGjQT16716731;     BVSzYdGjQT16716731 = BVSzYdGjQT44067744;     BVSzYdGjQT44067744 = BVSzYdGjQT11578660;     BVSzYdGjQT11578660 = BVSzYdGjQT4819480;     BVSzYdGjQT4819480 = BVSzYdGjQT74772894;     BVSzYdGjQT74772894 = BVSzYdGjQT7273015;     BVSzYdGjQT7273015 = BVSzYdGjQT26128219;     BVSzYdGjQT26128219 = BVSzYdGjQT29796182;     BVSzYdGjQT29796182 = BVSzYdGjQT13102304;     BVSzYdGjQT13102304 = BVSzYdGjQT118855;     BVSzYdGjQT118855 = BVSzYdGjQT29538272;     BVSzYdGjQT29538272 = BVSzYdGjQT39060048;     BVSzYdGjQT39060048 = BVSzYdGjQT75747950;     BVSzYdGjQT75747950 = BVSzYdGjQT60274504;     BVSzYdGjQT60274504 = BVSzYdGjQT49510362;     BVSzYdGjQT49510362 = BVSzYdGjQT69811854;     BVSzYdGjQT69811854 = BVSzYdGjQT63155480;     BVSzYdGjQT63155480 = BVSzYdGjQT27146499;     BVSzYdGjQT27146499 = BVSzYdGjQT26203501;     BVSzYdGjQT26203501 = BVSzYdGjQT17767051;     BVSzYdGjQT17767051 = BVSzYdGjQT77622447;     BVSzYdGjQT77622447 = BVSzYdGjQT7951285;     BVSzYdGjQT7951285 = BVSzYdGjQT81234236;     BVSzYdGjQT81234236 = BVSzYdGjQT83728174;     BVSzYdGjQT83728174 = BVSzYdGjQT7906129;     BVSzYdGjQT7906129 = BVSzYdGjQT59832452;     BVSzYdGjQT59832452 = BVSzYdGjQT48993651;     BVSzYdGjQT48993651 = BVSzYdGjQT59136586;     BVSzYdGjQT59136586 = BVSzYdGjQT68175177;     BVSzYdGjQT68175177 = BVSzYdGjQT1591998;     BVSzYdGjQT1591998 = BVSzYdGjQT30450967;     BVSzYdGjQT30450967 = BVSzYdGjQT44156736;     BVSzYdGjQT44156736 = BVSzYdGjQT58641243;     BVSzYdGjQT58641243 = BVSzYdGjQT15488740;     BVSzYdGjQT15488740 = BVSzYdGjQT26208335;     BVSzYdGjQT26208335 = BVSzYdGjQT15829199;     BVSzYdGjQT15829199 = BVSzYdGjQT2386125;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void IuTYvCFwkb22187064() {     int MzwhiKdXGN24773242 = -451140130;    int MzwhiKdXGN85126351 = -756911598;    int MzwhiKdXGN38739803 = -812698123;    int MzwhiKdXGN2732173 = -426974700;    int MzwhiKdXGN69275673 = -708477133;    int MzwhiKdXGN39571841 = -780666177;    int MzwhiKdXGN33830445 = -309782569;    int MzwhiKdXGN80322667 = -176881694;    int MzwhiKdXGN6205066 = -705058081;    int MzwhiKdXGN76097970 = 7697855;    int MzwhiKdXGN80573144 = -545122226;    int MzwhiKdXGN97033324 = -840392745;    int MzwhiKdXGN87015581 = 42864118;    int MzwhiKdXGN94013700 = -209218308;    int MzwhiKdXGN90581223 = -379428401;    int MzwhiKdXGN30083938 = -756772106;    int MzwhiKdXGN37469333 = -954771063;    int MzwhiKdXGN6376135 = -761031185;    int MzwhiKdXGN55806628 = -951034890;    int MzwhiKdXGN7733771 = -56214550;    int MzwhiKdXGN79143404 = -300318728;    int MzwhiKdXGN49407741 = -98410974;    int MzwhiKdXGN6492893 = -110033733;    int MzwhiKdXGN35907091 = -299647995;    int MzwhiKdXGN33591109 = 19513873;    int MzwhiKdXGN73698996 = -706986259;    int MzwhiKdXGN87822969 = -79755433;    int MzwhiKdXGN92063677 = -661841085;    int MzwhiKdXGN31022364 = -577910356;    int MzwhiKdXGN23843282 = 18901027;    int MzwhiKdXGN11908461 = -196709305;    int MzwhiKdXGN56504732 = -989220044;    int MzwhiKdXGN10590386 = -54615635;    int MzwhiKdXGN74576608 = -953634046;    int MzwhiKdXGN52855419 = -479089865;    int MzwhiKdXGN88265801 = -812316550;    int MzwhiKdXGN53139518 = -861292189;    int MzwhiKdXGN11475900 = -58819796;    int MzwhiKdXGN35605546 = -988188347;    int MzwhiKdXGN34591857 = -362319312;    int MzwhiKdXGN28436586 = -827918273;    int MzwhiKdXGN27708641 = -58047042;    int MzwhiKdXGN15394765 = -145658623;    int MzwhiKdXGN42369185 = -560835423;    int MzwhiKdXGN27880209 = -602488681;    int MzwhiKdXGN36339290 = 46348513;    int MzwhiKdXGN34537919 = -389489249;    int MzwhiKdXGN58185725 = -225098495;    int MzwhiKdXGN54408631 = -80254044;    int MzwhiKdXGN3446499 = -331955057;    int MzwhiKdXGN45932872 = -91860937;    int MzwhiKdXGN33517628 = -376477110;    int MzwhiKdXGN96043708 = -685602327;    int MzwhiKdXGN6110087 = 16190973;    int MzwhiKdXGN43043660 = -557751517;    int MzwhiKdXGN75365501 = -252729157;    int MzwhiKdXGN78633458 = -546877866;    int MzwhiKdXGN2832712 = -413050128;    int MzwhiKdXGN69141063 = -346488574;    int MzwhiKdXGN95576677 = 98509125;    int MzwhiKdXGN51748871 = -600910744;    int MzwhiKdXGN41766768 = -647941484;    int MzwhiKdXGN49300303 = -598971338;    int MzwhiKdXGN82361784 = -623959109;    int MzwhiKdXGN64189510 = -795592841;    int MzwhiKdXGN24068413 = -555902183;    int MzwhiKdXGN86442938 = -685777111;    int MzwhiKdXGN12438973 = -3501836;    int MzwhiKdXGN41158282 = -730128443;    int MzwhiKdXGN2315423 = -567111851;    int MzwhiKdXGN76944419 = -895479917;    int MzwhiKdXGN25993433 = -795951268;    int MzwhiKdXGN70770588 = -772842839;    int MzwhiKdXGN21214771 = -488715579;    int MzwhiKdXGN79297184 = -228296278;    int MzwhiKdXGN51434764 = -142271686;    int MzwhiKdXGN34012977 = -952752351;    int MzwhiKdXGN64123708 = -549198311;    int MzwhiKdXGN8026883 = -697159314;    int MzwhiKdXGN97251818 = 73165359;    int MzwhiKdXGN39161078 = -217497010;    int MzwhiKdXGN29637245 = -854656939;    int MzwhiKdXGN37655046 = -481587042;    int MzwhiKdXGN27575866 = -145955300;    int MzwhiKdXGN77910410 = -889238036;    int MzwhiKdXGN78390832 = -820232195;    int MzwhiKdXGN60461024 = -203617718;    int MzwhiKdXGN4480300 = 29193391;    int MzwhiKdXGN31532949 = -295882529;    int MzwhiKdXGN77489917 = -126360709;    int MzwhiKdXGN9632343 = -165438685;    int MzwhiKdXGN50306806 = -348242061;    int MzwhiKdXGN42334836 = -712331223;    int MzwhiKdXGN40028869 = -986697472;    int MzwhiKdXGN82842985 = -761408569;    int MzwhiKdXGN86669818 = -79976789;    int MzwhiKdXGN78408337 = -459075705;    int MzwhiKdXGN33032981 = -521699514;    int MzwhiKdXGN78179675 = -765242582;    int MzwhiKdXGN3811796 = -451140130;     MzwhiKdXGN24773242 = MzwhiKdXGN85126351;     MzwhiKdXGN85126351 = MzwhiKdXGN38739803;     MzwhiKdXGN38739803 = MzwhiKdXGN2732173;     MzwhiKdXGN2732173 = MzwhiKdXGN69275673;     MzwhiKdXGN69275673 = MzwhiKdXGN39571841;     MzwhiKdXGN39571841 = MzwhiKdXGN33830445;     MzwhiKdXGN33830445 = MzwhiKdXGN80322667;     MzwhiKdXGN80322667 = MzwhiKdXGN6205066;     MzwhiKdXGN6205066 = MzwhiKdXGN76097970;     MzwhiKdXGN76097970 = MzwhiKdXGN80573144;     MzwhiKdXGN80573144 = MzwhiKdXGN97033324;     MzwhiKdXGN97033324 = MzwhiKdXGN87015581;     MzwhiKdXGN87015581 = MzwhiKdXGN94013700;     MzwhiKdXGN94013700 = MzwhiKdXGN90581223;     MzwhiKdXGN90581223 = MzwhiKdXGN30083938;     MzwhiKdXGN30083938 = MzwhiKdXGN37469333;     MzwhiKdXGN37469333 = MzwhiKdXGN6376135;     MzwhiKdXGN6376135 = MzwhiKdXGN55806628;     MzwhiKdXGN55806628 = MzwhiKdXGN7733771;     MzwhiKdXGN7733771 = MzwhiKdXGN79143404;     MzwhiKdXGN79143404 = MzwhiKdXGN49407741;     MzwhiKdXGN49407741 = MzwhiKdXGN6492893;     MzwhiKdXGN6492893 = MzwhiKdXGN35907091;     MzwhiKdXGN35907091 = MzwhiKdXGN33591109;     MzwhiKdXGN33591109 = MzwhiKdXGN73698996;     MzwhiKdXGN73698996 = MzwhiKdXGN87822969;     MzwhiKdXGN87822969 = MzwhiKdXGN92063677;     MzwhiKdXGN92063677 = MzwhiKdXGN31022364;     MzwhiKdXGN31022364 = MzwhiKdXGN23843282;     MzwhiKdXGN23843282 = MzwhiKdXGN11908461;     MzwhiKdXGN11908461 = MzwhiKdXGN56504732;     MzwhiKdXGN56504732 = MzwhiKdXGN10590386;     MzwhiKdXGN10590386 = MzwhiKdXGN74576608;     MzwhiKdXGN74576608 = MzwhiKdXGN52855419;     MzwhiKdXGN52855419 = MzwhiKdXGN88265801;     MzwhiKdXGN88265801 = MzwhiKdXGN53139518;     MzwhiKdXGN53139518 = MzwhiKdXGN11475900;     MzwhiKdXGN11475900 = MzwhiKdXGN35605546;     MzwhiKdXGN35605546 = MzwhiKdXGN34591857;     MzwhiKdXGN34591857 = MzwhiKdXGN28436586;     MzwhiKdXGN28436586 = MzwhiKdXGN27708641;     MzwhiKdXGN27708641 = MzwhiKdXGN15394765;     MzwhiKdXGN15394765 = MzwhiKdXGN42369185;     MzwhiKdXGN42369185 = MzwhiKdXGN27880209;     MzwhiKdXGN27880209 = MzwhiKdXGN36339290;     MzwhiKdXGN36339290 = MzwhiKdXGN34537919;     MzwhiKdXGN34537919 = MzwhiKdXGN58185725;     MzwhiKdXGN58185725 = MzwhiKdXGN54408631;     MzwhiKdXGN54408631 = MzwhiKdXGN3446499;     MzwhiKdXGN3446499 = MzwhiKdXGN45932872;     MzwhiKdXGN45932872 = MzwhiKdXGN33517628;     MzwhiKdXGN33517628 = MzwhiKdXGN96043708;     MzwhiKdXGN96043708 = MzwhiKdXGN6110087;     MzwhiKdXGN6110087 = MzwhiKdXGN43043660;     MzwhiKdXGN43043660 = MzwhiKdXGN75365501;     MzwhiKdXGN75365501 = MzwhiKdXGN78633458;     MzwhiKdXGN78633458 = MzwhiKdXGN2832712;     MzwhiKdXGN2832712 = MzwhiKdXGN69141063;     MzwhiKdXGN69141063 = MzwhiKdXGN95576677;     MzwhiKdXGN95576677 = MzwhiKdXGN51748871;     MzwhiKdXGN51748871 = MzwhiKdXGN41766768;     MzwhiKdXGN41766768 = MzwhiKdXGN49300303;     MzwhiKdXGN49300303 = MzwhiKdXGN82361784;     MzwhiKdXGN82361784 = MzwhiKdXGN64189510;     MzwhiKdXGN64189510 = MzwhiKdXGN24068413;     MzwhiKdXGN24068413 = MzwhiKdXGN86442938;     MzwhiKdXGN86442938 = MzwhiKdXGN12438973;     MzwhiKdXGN12438973 = MzwhiKdXGN41158282;     MzwhiKdXGN41158282 = MzwhiKdXGN2315423;     MzwhiKdXGN2315423 = MzwhiKdXGN76944419;     MzwhiKdXGN76944419 = MzwhiKdXGN25993433;     MzwhiKdXGN25993433 = MzwhiKdXGN70770588;     MzwhiKdXGN70770588 = MzwhiKdXGN21214771;     MzwhiKdXGN21214771 = MzwhiKdXGN79297184;     MzwhiKdXGN79297184 = MzwhiKdXGN51434764;     MzwhiKdXGN51434764 = MzwhiKdXGN34012977;     MzwhiKdXGN34012977 = MzwhiKdXGN64123708;     MzwhiKdXGN64123708 = MzwhiKdXGN8026883;     MzwhiKdXGN8026883 = MzwhiKdXGN97251818;     MzwhiKdXGN97251818 = MzwhiKdXGN39161078;     MzwhiKdXGN39161078 = MzwhiKdXGN29637245;     MzwhiKdXGN29637245 = MzwhiKdXGN37655046;     MzwhiKdXGN37655046 = MzwhiKdXGN27575866;     MzwhiKdXGN27575866 = MzwhiKdXGN77910410;     MzwhiKdXGN77910410 = MzwhiKdXGN78390832;     MzwhiKdXGN78390832 = MzwhiKdXGN60461024;     MzwhiKdXGN60461024 = MzwhiKdXGN4480300;     MzwhiKdXGN4480300 = MzwhiKdXGN31532949;     MzwhiKdXGN31532949 = MzwhiKdXGN77489917;     MzwhiKdXGN77489917 = MzwhiKdXGN9632343;     MzwhiKdXGN9632343 = MzwhiKdXGN50306806;     MzwhiKdXGN50306806 = MzwhiKdXGN42334836;     MzwhiKdXGN42334836 = MzwhiKdXGN40028869;     MzwhiKdXGN40028869 = MzwhiKdXGN82842985;     MzwhiKdXGN82842985 = MzwhiKdXGN86669818;     MzwhiKdXGN86669818 = MzwhiKdXGN78408337;     MzwhiKdXGN78408337 = MzwhiKdXGN33032981;     MzwhiKdXGN33032981 = MzwhiKdXGN78179675;     MzwhiKdXGN78179675 = MzwhiKdXGN3811796;     MzwhiKdXGN3811796 = MzwhiKdXGN24773242;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void gaAVHjyPKw58832038() {     int AgUlmmwYgG47160359 = -85544093;    int AgUlmmwYgG69473852 = -705454653;    int AgUlmmwYgG67509082 = 47648033;    int AgUlmmwYgG17738861 = -433382232;    int AgUlmmwYgG67449927 = -199534134;    int AgUlmmwYgG54454958 = -641274554;    int AgUlmmwYgG25986184 = -131769875;    int AgUlmmwYgG55956526 = -647478338;    int AgUlmmwYgG3368197 = -589254121;    int AgUlmmwYgG67861555 = -835043478;    int AgUlmmwYgG18115005 = -541746627;    int AgUlmmwYgG52936273 = 8861962;    int AgUlmmwYgG54023761 = -575853201;    int AgUlmmwYgG36628453 = -736927065;    int AgUlmmwYgG27316501 = -6906114;    int AgUlmmwYgG74339850 = -38439648;    int AgUlmmwYgG87292684 = -132050883;    int AgUlmmwYgG12352551 = -112773875;    int AgUlmmwYgG97088258 = 13862984;    int AgUlmmwYgG86250053 = -308849004;    int AgUlmmwYgG80026511 = -291806036;    int AgUlmmwYgG13222959 = -603177287;    int AgUlmmwYgG9786217 = -286114325;    int AgUlmmwYgG62406878 = -863640852;    int AgUlmmwYgG69131356 = -702045251;    int AgUlmmwYgG62001990 = -50880482;    int AgUlmmwYgG34912390 = -937341926;    int AgUlmmwYgG97975127 = -450295539;    int AgUlmmwYgG37914925 = -922167475;    int AgUlmmwYgG33000462 = 98605161;    int AgUlmmwYgG56199265 = -278025457;    int AgUlmmwYgG14045923 = -541291399;    int AgUlmmwYgG91629056 = -583959130;    int AgUlmmwYgG33965296 = -177760896;    int AgUlmmwYgG29084784 = -542846288;    int AgUlmmwYgG29958671 = -27711927;    int AgUlmmwYgG46579230 = -594240141;    int AgUlmmwYgG65102000 = 73554732;    int AgUlmmwYgG83913677 = -289234378;    int AgUlmmwYgG54777571 = -498543341;    int AgUlmmwYgG57193957 = -231095939;    int AgUlmmwYgG16217032 = -915955733;    int AgUlmmwYgG20944957 = -33189207;    int AgUlmmwYgG41813304 = -558199072;    int AgUlmmwYgG95863475 = -411632159;    int AgUlmmwYgG44439572 = -135880582;    int AgUlmmwYgG46835316 = -967899502;    int AgUlmmwYgG2784400 = 18366514;    int AgUlmmwYgG48868536 = -29970742;    int AgUlmmwYgG530244 = -473975316;    int AgUlmmwYgG54802088 = -446937214;    int AgUlmmwYgG7368885 = -807296392;    int AgUlmmwYgG74358111 = -357513557;    int AgUlmmwYgG66396631 = -259489835;    int AgUlmmwYgG78805527 = -698455632;    int AgUlmmwYgG33937401 = -482366807;    int AgUlmmwYgG59687636 = -319340328;    int AgUlmmwYgG5102204 = -88711115;    int AgUlmmwYgG48607505 = -731336981;    int AgUlmmwYgG5447938 = -48653652;    int AgUlmmwYgG19542568 = -703932629;    int AgUlmmwYgG28011057 = -681474337;    int AgUlmmwYgG18041602 = -725310863;    int AgUlmmwYgG70367735 = -587859282;    int AgUlmmwYgG11662290 = -457018022;    int AgUlmmwYgG4069082 = 99544771;    int AgUlmmwYgG61307217 = -407178909;    int AgUlmmwYgG20058466 = -298092306;    int AgUlmmwYgG7543670 = -94080778;    int AgUlmmwYgG97357829 = -979194188;    int AgUlmmwYgG27760620 = -444199508;    int AgUlmmwYgG22190684 = -105605615;    int AgUlmmwYgG28438874 = -823539498;    int AgUlmmwYgG42310688 = -487593675;    int AgUlmmwYgG29056097 = 22246934;    int AgUlmmwYgG63809480 = -375850303;    int AgUlmmwYgG92278002 = -469988081;    int AgUlmmwYgG67972912 = -727915254;    int AgUlmmwYgG66543403 = -352008694;    int AgUlmmwYgG24691784 = -466164670;    int AgUlmmwYgG15166675 = -82980981;    int AgUlmmwYgG32127990 = -855708441;    int AgUlmmwYgG49106591 = -320324797;    int AgUlmmwYgG37384681 = -348192160;    int AgUlmmwYgG78198373 = -454457626;    int AgUlmmwYgG48830381 = -470729065;    int AgUlmmwYgG39687812 = -83777842;    int AgUlmmwYgG25232425 = -224469295;    int AgUlmmwYgG55159768 = -479305264;    int AgUlmmwYgG95147383 = 39520519;    int AgUlmmwYgG70271034 = -708371599;    int AgUlmmwYgG41477027 = -405529026;    int AgUlmmwYgG16494496 = -195108287;    int AgUlmmwYgG78465740 = -140580727;    int AgUlmmwYgG35235003 = -794610713;    int AgUlmmwYgG29182901 = -549621602;    int AgUlmmwYgG98175430 = -90644871;    int AgUlmmwYgG50577221 = -445329926;    int AgUlmmwYgG30151015 = -1181050;    int AgUlmmwYgG91794393 = -85544093;     AgUlmmwYgG47160359 = AgUlmmwYgG69473852;     AgUlmmwYgG69473852 = AgUlmmwYgG67509082;     AgUlmmwYgG67509082 = AgUlmmwYgG17738861;     AgUlmmwYgG17738861 = AgUlmmwYgG67449927;     AgUlmmwYgG67449927 = AgUlmmwYgG54454958;     AgUlmmwYgG54454958 = AgUlmmwYgG25986184;     AgUlmmwYgG25986184 = AgUlmmwYgG55956526;     AgUlmmwYgG55956526 = AgUlmmwYgG3368197;     AgUlmmwYgG3368197 = AgUlmmwYgG67861555;     AgUlmmwYgG67861555 = AgUlmmwYgG18115005;     AgUlmmwYgG18115005 = AgUlmmwYgG52936273;     AgUlmmwYgG52936273 = AgUlmmwYgG54023761;     AgUlmmwYgG54023761 = AgUlmmwYgG36628453;     AgUlmmwYgG36628453 = AgUlmmwYgG27316501;     AgUlmmwYgG27316501 = AgUlmmwYgG74339850;     AgUlmmwYgG74339850 = AgUlmmwYgG87292684;     AgUlmmwYgG87292684 = AgUlmmwYgG12352551;     AgUlmmwYgG12352551 = AgUlmmwYgG97088258;     AgUlmmwYgG97088258 = AgUlmmwYgG86250053;     AgUlmmwYgG86250053 = AgUlmmwYgG80026511;     AgUlmmwYgG80026511 = AgUlmmwYgG13222959;     AgUlmmwYgG13222959 = AgUlmmwYgG9786217;     AgUlmmwYgG9786217 = AgUlmmwYgG62406878;     AgUlmmwYgG62406878 = AgUlmmwYgG69131356;     AgUlmmwYgG69131356 = AgUlmmwYgG62001990;     AgUlmmwYgG62001990 = AgUlmmwYgG34912390;     AgUlmmwYgG34912390 = AgUlmmwYgG97975127;     AgUlmmwYgG97975127 = AgUlmmwYgG37914925;     AgUlmmwYgG37914925 = AgUlmmwYgG33000462;     AgUlmmwYgG33000462 = AgUlmmwYgG56199265;     AgUlmmwYgG56199265 = AgUlmmwYgG14045923;     AgUlmmwYgG14045923 = AgUlmmwYgG91629056;     AgUlmmwYgG91629056 = AgUlmmwYgG33965296;     AgUlmmwYgG33965296 = AgUlmmwYgG29084784;     AgUlmmwYgG29084784 = AgUlmmwYgG29958671;     AgUlmmwYgG29958671 = AgUlmmwYgG46579230;     AgUlmmwYgG46579230 = AgUlmmwYgG65102000;     AgUlmmwYgG65102000 = AgUlmmwYgG83913677;     AgUlmmwYgG83913677 = AgUlmmwYgG54777571;     AgUlmmwYgG54777571 = AgUlmmwYgG57193957;     AgUlmmwYgG57193957 = AgUlmmwYgG16217032;     AgUlmmwYgG16217032 = AgUlmmwYgG20944957;     AgUlmmwYgG20944957 = AgUlmmwYgG41813304;     AgUlmmwYgG41813304 = AgUlmmwYgG95863475;     AgUlmmwYgG95863475 = AgUlmmwYgG44439572;     AgUlmmwYgG44439572 = AgUlmmwYgG46835316;     AgUlmmwYgG46835316 = AgUlmmwYgG2784400;     AgUlmmwYgG2784400 = AgUlmmwYgG48868536;     AgUlmmwYgG48868536 = AgUlmmwYgG530244;     AgUlmmwYgG530244 = AgUlmmwYgG54802088;     AgUlmmwYgG54802088 = AgUlmmwYgG7368885;     AgUlmmwYgG7368885 = AgUlmmwYgG74358111;     AgUlmmwYgG74358111 = AgUlmmwYgG66396631;     AgUlmmwYgG66396631 = AgUlmmwYgG78805527;     AgUlmmwYgG78805527 = AgUlmmwYgG33937401;     AgUlmmwYgG33937401 = AgUlmmwYgG59687636;     AgUlmmwYgG59687636 = AgUlmmwYgG5102204;     AgUlmmwYgG5102204 = AgUlmmwYgG48607505;     AgUlmmwYgG48607505 = AgUlmmwYgG5447938;     AgUlmmwYgG5447938 = AgUlmmwYgG19542568;     AgUlmmwYgG19542568 = AgUlmmwYgG28011057;     AgUlmmwYgG28011057 = AgUlmmwYgG18041602;     AgUlmmwYgG18041602 = AgUlmmwYgG70367735;     AgUlmmwYgG70367735 = AgUlmmwYgG11662290;     AgUlmmwYgG11662290 = AgUlmmwYgG4069082;     AgUlmmwYgG4069082 = AgUlmmwYgG61307217;     AgUlmmwYgG61307217 = AgUlmmwYgG20058466;     AgUlmmwYgG20058466 = AgUlmmwYgG7543670;     AgUlmmwYgG7543670 = AgUlmmwYgG97357829;     AgUlmmwYgG97357829 = AgUlmmwYgG27760620;     AgUlmmwYgG27760620 = AgUlmmwYgG22190684;     AgUlmmwYgG22190684 = AgUlmmwYgG28438874;     AgUlmmwYgG28438874 = AgUlmmwYgG42310688;     AgUlmmwYgG42310688 = AgUlmmwYgG29056097;     AgUlmmwYgG29056097 = AgUlmmwYgG63809480;     AgUlmmwYgG63809480 = AgUlmmwYgG92278002;     AgUlmmwYgG92278002 = AgUlmmwYgG67972912;     AgUlmmwYgG67972912 = AgUlmmwYgG66543403;     AgUlmmwYgG66543403 = AgUlmmwYgG24691784;     AgUlmmwYgG24691784 = AgUlmmwYgG15166675;     AgUlmmwYgG15166675 = AgUlmmwYgG32127990;     AgUlmmwYgG32127990 = AgUlmmwYgG49106591;     AgUlmmwYgG49106591 = AgUlmmwYgG37384681;     AgUlmmwYgG37384681 = AgUlmmwYgG78198373;     AgUlmmwYgG78198373 = AgUlmmwYgG48830381;     AgUlmmwYgG48830381 = AgUlmmwYgG39687812;     AgUlmmwYgG39687812 = AgUlmmwYgG25232425;     AgUlmmwYgG25232425 = AgUlmmwYgG55159768;     AgUlmmwYgG55159768 = AgUlmmwYgG95147383;     AgUlmmwYgG95147383 = AgUlmmwYgG70271034;     AgUlmmwYgG70271034 = AgUlmmwYgG41477027;     AgUlmmwYgG41477027 = AgUlmmwYgG16494496;     AgUlmmwYgG16494496 = AgUlmmwYgG78465740;     AgUlmmwYgG78465740 = AgUlmmwYgG35235003;     AgUlmmwYgG35235003 = AgUlmmwYgG29182901;     AgUlmmwYgG29182901 = AgUlmmwYgG98175430;     AgUlmmwYgG98175430 = AgUlmmwYgG50577221;     AgUlmmwYgG50577221 = AgUlmmwYgG30151015;     AgUlmmwYgG30151015 = AgUlmmwYgG91794393;     AgUlmmwYgG91794393 = AgUlmmwYgG47160359;}
// Junk Finished
