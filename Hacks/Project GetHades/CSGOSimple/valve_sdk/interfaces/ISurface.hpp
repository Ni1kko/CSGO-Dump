#pragma once

#include "IAppSystem.hpp"
#include "../math/Vector2D.hpp"

namespace vgui
{
    typedef unsigned long HFont;
    typedef unsigned int VPANEL;
};

enum font_feature_t
{
    FONT_FEATURE_ANTIALIASED_FONTS = 1,
    FONT_FEATURE_DROPSHADOW_FONTS = 2,
    FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum font_draw_type_t
{
    FONT_DRAW_DEFAULT = 0,
    FONT_DRAW_NONADDITIVE,
    FONT_DRAW_ADDITIVE,
    FONT_DRAW_TYPE_COUNT = 2,
};

enum font_flags_t
{
    FONTFLAG_NONE,
    FONTFLAG_ITALIC = 0x001,
    FONTFLAG_UNDERLINE = 0x002,
    FONTFLAG_STRIKEOUT = 0x004,
    FONTFLAG_SYMBOL = 0x008,
    FONTFLAG_ANTIALIAS = 0x010,
    FONTFLAG_GAUSSIANBLUR = 0x020,
    FONTFLAG_ROTARY = 0x040,
    FONTFLAG_DROPSHADOW = 0x080,
    FONTFLAG_ADDITIVE = 0x100,
    FONTFLAG_OUTLINE = 0x200,
    FONTFLAG_CUSTOM = 0x400,
    FONTFLAG_BITMAP = 0x800,
};

struct int_rect_t
{
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Vertex_t
{
    Vertex_t ( )
    {
    }

    Vertex_t ( const Vector2D& pos, const Vector2D& coord = Vector2D ( 0, 0 ) )
    {
        m_Position = pos;
        m_TexCoord = coord;
    }

    void Init ( const Vector2D& pos, const Vector2D& coord = Vector2D ( 0, 0 ) )
    {
        m_Position = pos;
        m_TexCoord = coord;
    }

    Vector2D m_Position;
    Vector2D m_TexCoord;
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface : public IAppSystem
{
public:
    virtual void run_frame ( ) = 0;
    virtual vgui::VPANEL get_embedded_panel ( ) = 0;
    virtual void set_embedded_panel ( vgui::VPANEL pPanel ) = 0;
    virtual void push_make_current ( vgui::VPANEL panel, bool useInsets ) = 0;
    virtual void pop_make_current ( vgui::VPANEL panel ) = 0;
    virtual void draw_set_color ( int r, int g, int b, int a ) = 0;
    virtual void draw_set_color ( Color col ) = 0;
    virtual void draw_filled_rect ( int x0, int y0, int x1, int y1 ) = 0;
    virtual void draw_filled_rect_array ( int_rect_t* pRects, int numRects ) = 0;
    virtual void draw_outlined_rect ( int x0, int y0, int x1, int y1 ) = 0;
    virtual void draw_line ( int x0, int y0, int x1, int y1 ) = 0;
    virtual void draw_poly_line ( int* px, int* py, int numPoints ) = 0;
    virtual void draw_set_apparent_depth ( float f ) = 0;
    virtual void draw_clear_apparent_depth ( void ) = 0;
    virtual void draw_set_text_font ( vgui::HFont font ) = 0;
    virtual void draw_set_text_color ( int r, int g, int b, int a ) = 0;
    virtual void draw_set_text_color ( Color col ) = 0;
    virtual void draw_set_text_pos ( int x, int y ) = 0;
    virtual void draw_get_text_pos ( int& x, int& y ) = 0;
    virtual void draw_print_text ( const wchar_t* text, int textLen, font_draw_type_t drawType = font_draw_type_t::FONT_DRAW_DEFAULT ) = 0;
    virtual void draw_unicode_char ( wchar_t wch, font_draw_type_t drawType = font_draw_type_t::FONT_DRAW_DEFAULT ) = 0;
    virtual void draw_flush_text ( ) = 0;
    virtual void* create_html_window ( void* events, vgui::VPANEL context ) = 0;
    virtual void paint_html_window ( void* htmlwin ) = 0;
    virtual void delete_html_window ( void* htmlwin ) = 0;
    virtual int draw_get_texture_id ( char const* filename ) = 0;
    virtual bool draw_get_texture_file ( int id, char* filename, int maxlen ) = 0;
    virtual void draw_set_texture_file ( int id, const char* filename, int hardwareFilter, bool forceReload ) = 0;
    virtual void draw_set_texture_rgba ( int id, const unsigned char* rgba, int wide, int tall ) = 0;
    virtual void draw_set_texture ( int id ) = 0;
    virtual void delete_texture_by_id ( int id ) = 0;
    virtual void draw_get_texture_size ( int id, int& wide, int& tall ) = 0;
    virtual void draw_textured_rect ( int x0, int y0, int x1, int y1 ) = 0;
    virtual bool is_texture_id_valid ( int id ) = 0;
    virtual int create_new_texture_id ( bool procedural = false ) = 0;
    virtual void get_screen_size ( int& wide, int& tall ) = 0;
    virtual void set_as_top_most ( vgui::VPANEL panel, bool state ) = 0;
    virtual void bring_to_front ( vgui::VPANEL panel ) = 0;
    virtual void set_foreground_window ( vgui::VPANEL panel ) = 0;
    virtual void set_panel_visible ( vgui::VPANEL panel, bool state ) = 0;
    virtual void set_minimized ( vgui::VPANEL panel, bool state ) = 0;
    virtual bool is_minimized ( vgui::VPANEL panel ) = 0;
    virtual void flash_window ( vgui::VPANEL panel, bool state ) = 0;
    virtual void set_title ( vgui::VPANEL panel, const wchar_t* title ) = 0;
    virtual void set_as_toolbar ( vgui::VPANEL panel, bool state ) = 0;
    virtual void create_popup ( vgui::VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true,
                                bool kbInput = true ) = 0;
    virtual void swap_buffers ( vgui::VPANEL panel ) = 0;
    virtual void invalidate ( vgui::VPANEL panel ) = 0;
    virtual void set_cursor ( unsigned long cursor ) = 0;
    virtual bool is_cursor_visible ( ) = 0;
    virtual void apply_changes ( ) = 0;
    virtual bool is_within ( int x, int y ) = 0;
    virtual bool has_focus ( ) = 0;
    virtual bool supports_feature ( int /*SurfaceFeature_t*/ feature ) = 0;
    virtual void restrict_paint_to_single_panel ( vgui::VPANEL panel, bool bForceAllowNonModalSurface = false ) = 0;
    virtual void set_modal_panel ( vgui::VPANEL ) = 0;
    virtual vgui::VPANEL get_modal_panel ( ) = 0;
    virtual void unlock_cursor ( ) = 0;
    virtual void lock_cursor ( ) = 0;
    virtual void set_translate_extended_keys ( bool state ) = 0;
    virtual vgui::VPANEL get_topmost_popup ( ) = 0;
    virtual void set_top_level_focus ( vgui::VPANEL panel ) = 0;
    virtual vgui::HFont create_font_ ( ) = 0;
    virtual bool set_font_glyph_set ( vgui::HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags,
                                      int nRangeMin = 0, int nRangeMax = 0 ) = 0;
    virtual bool add_custom_font_file ( const char* fontFileName ) = 0;
    virtual int get_font_tall ( vgui::HFont font ) = 0;
    virtual int get_font_ascent ( vgui::HFont font, wchar_t wch ) = 0;
    virtual bool is_font_additive ( vgui::HFont font ) = 0;
    virtual void get_char_abcwide ( vgui::HFont font, int ch, int& a, int& b, int& c ) = 0;
    virtual int get_character_width ( vgui::HFont font, int ch ) = 0;
    virtual void get_text_size ( vgui::HFont font, const wchar_t* text, int& wide, int& tall ) = 0;
    virtual vgui::VPANEL get_notify_panel ( ) = 0;
    virtual void set_notify_icon ( vgui::VPANEL context, unsigned long icon, vgui::VPANEL panelToReceiveMessages, const char* text ) = 0;
    virtual void play_sound_ ( const char* fileName ) = 0;
    virtual int get_popup_count ( ) = 0;
    virtual vgui::VPANEL get_popup ( int index ) = 0;
    virtual bool should_paint_child_panel ( vgui::VPANEL childPanel ) = 0;
    virtual bool recreate_context ( vgui::VPANEL panel ) = 0;
    virtual void add_panel ( vgui::VPANEL panel ) = 0;
    virtual void release_panel ( vgui::VPANEL panel ) = 0;
    virtual void move_popup_to_front ( vgui::VPANEL panel ) = 0;
    virtual void move_popup_to_back ( vgui::VPANEL panel ) = 0;
    virtual void solve_traverse ( vgui::VPANEL panel, bool forceApplySchemeSettings = false ) = 0;
    virtual void paint_traverse ( vgui::VPANEL panel ) = 0;
    virtual void enable_mouse_capture ( vgui::VPANEL panel, bool state ) = 0;
    virtual void get_workspace_bounds ( int& x, int& y, int& wide, int& tall ) = 0;
    virtual void get_absolute_window_bounds ( int& x, int& y, int& wide, int& tall ) = 0;
    virtual void get_proportional_base ( int& width, int& height ) = 0;
    virtual void calculate_mouse_visible ( ) = 0;
    virtual bool need_kb_input ( ) = 0;
    virtual bool has_cursor_pos_functions ( ) = 0;
    virtual void surface_get_cursor_pos ( int& x, int& y ) = 0;
    virtual void surface_set_cursor_pos ( int x, int y ) = 0;
    virtual void draw_textured_line ( const Vertex_t& a, const Vertex_t& b ) = 0;
    virtual void draw_outlined_circle ( int x, int y, int radius, int segments ) = 0;

    virtual void draw_textured_polyline ( const Vertex_t* p, int n ) = 0;
    virtual void draw_textured_subrect ( int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1 ) = 0;
    virtual void draw_textured_polygon ( int n, Vertex_t* pVertice, bool bClipVertices = true ) = 0;

    void get_clip_rect ( int& x, int& y, int& x2, int& y2 )
    {
        CallVFunction< void ( __thiscall*) ( void*, int&, int&, int&, int& ) > ( this, 146 ) ( this, x, y, x2, y2 );
    }

    void set_clip_rect ( const int x, const int y, const int x2, const int y2 )
    {
        CallVFunction< void ( __thiscall*) ( void*, int, int, int, int ) > ( this, 147 ) ( this, x, y, x2, y2 );
    }

    bool& clipping_enabled ( )
    {
        return *reinterpret_cast< bool* > ( reinterpret_cast< uintptr_t > ( this ) + 0x280 );
    }
};
