#pragma once
#include "../valve_sdk/csgostructs.hpp"

struct vfont
{
    void create ( std::string font_name, const int size, const int flags )
    {
        this->hfont = interfaces::vgui_surface->create_font_( );
        interfaces::vgui_surface->set_font_glyph_set ( this->hfont, font_name.data( ), size, 700, 0, 0, flags );
        this->name = font_name;
        this->size = size;
        this->initialized = true;
    }

    std::string get_name ( )
    {
        return this->name;
    }

    int get_size ( ) const
    {
        return this->size;
    }

    vgui::HFont hfont{ };
    int size = 0;
    std::string name = "";
    bool initialized = false;
};

struct pos_vec
{
    pos_vec ( ) = default;;

    pos_vec ( const int x, const int y )
    {
        this->y = y;
        this->x = x;
    };

    int x = 0;
    int y = 0;
};

struct pos_wh
{
    pos_wh ( ) = default;;

    pos_wh ( int w, int h )
    {
        this->w = w;
        this->h = h;
    };

    int w = 0;
    int h = 0;
};

struct CUSTOMVERTEX
{
    FLOAT x, y, z;
    FLOAT rhw;
    DWORD color;
};

class vgui_renderer
{
private:
    float alpha_factor = 1.f;
    LPDIRECT3DDEVICE9 m_device;
    D3DVIEWPORT9 m_viewport;

public:
    bool initalized = false;

    void setup_states ( ) const;

    void initalize_objects ( LPDIRECT3DDEVICE9 m_device );
    void invalidate_objects ( );
    void restore_objects ( LPDIRECT3DDEVICE9 m_device );
    void circle_dual_colour ( float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device );
    void circle_dual_colour ( float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9* m_device );

    void text ( vfont font, std::string text, float x, float y, Color clr ) const;
    void polygon ( int count, Vertex_t* vertexs, Color color ) const;
    void line ( float x1, float y1, float x2, float y2, Color clr, float size ) const;
    void box ( float x1, float y1, float x2, float y2, Color clr, float size ) const;
    void box_wh ( float x, float y, float w, float h, Color clr, float size ) const;
    void add_triangle_filled ( std::array< Vector2D, 3 > points, Color color );
    void add_textured_polygon ( int n, Vertex_t* vertice, Color color ) const;
    void filled_box ( float x1, float y1, float x2, float y2, Color clr ) const;
    void filled_box_wh ( float x, float y, float w, float h, Color clr );
    void filled_box ( Vector4D vec, Color clr );
    void gradient_vertical_wh ( float x, float y, float w, float h, Color clr1, Color clr2 );
    void gradient_horizontal_wh ( float x, float y, float w, float h, Color clr1, Color clr2 );
    void gradient_vertical ( float x1, float y1, float x2, float y2, Color clr1, Color clr2 );
    void gradient_horizontal ( float x1, float y1, float x2, float y2, Color clr1, Color clr2 );
    void circle ( float x, float y, float r, int seg, Color clr ) const;
    void circle_3d ( Vector pos, float points, float radius, Color clr ) const;
    Vector2D get_size ( vfont font, const std::string text ) const;
    pos_vec get_screen_center ( ) const;
};

class fonts
{
public:
    void init ( )
    {
        normal.create ( xor_str ( "Sans-serif" ), 12, FONTFLAG_DROPSHADOW );
        normal_small.create ( xor_str ( "Sans-serif" ), 6, FONTFLAG_DROPSHADOW );
        font_16.create ( xor_str ( "Sans-serif" ), 16, FONTFLAG_OUTLINE );
        font_32.create ( xor_str ( "Sans-serif" ), 32, FONTFLAG_OUTLINE );
        font_14.create ( xor_str ( "Sans-serif" ), 14, FONTFLAG_DROPSHADOW );
        itemesp.create ( xor_str ( "Sans-serif" ), 12, FONTFLAG_OUTLINE );

        indicators.create ( xor_str ( "Calibri" ), 35, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS );

        menu10.create ( xor_str ( "verdana" ), 10, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS );
        menu_watermark.create ( xor_str ( "Verdana" ), 10, FONTFLAG_OUTLINE );
        menu12.create ( xor_str ( "Sans-serif" ), 12, FONTFLAG_OUTLINE );
        menu14.create ( xor_str ( "Sans-serif" ), 14, FONTFLAG_OUTLINE );
        icon.create ( xor_str ( "Sans forgetica" ), 64, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE );
        weapons.create ( xor_str ( "Verdana" ), 12, FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE );
    }

    vfont normal_small;
    vfont normal;
    vfont itemesp;
    vfont font_16;
    vfont font_32;
    vfont font_14;
    vfont weapons;

    vfont indicators;

    vfont menu10;
    vfont menu_watermark;
    vfont menu12;
    vfont menu14;
    vfont icon;
};

extern vgui_renderer g_render;
extern fonts g_fonts;
