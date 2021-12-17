#pragma once
#include "../valve_sdk/csgostructs.hpp"
#include "../renderer/vgui_render.h"

#include <sstream>
#include <deque>

class hades_cursor
{
public:
    hades_cursor ( )
    {
        this->cursor_pos = g_render.get_screen_center( );
    };

    void render ( ) const;
    void run_think ( UINT u_msg, LPARAM lparam );

    bool in_vector ( Vector4D v ) const;
    bool in_vector_wh ( Vector4D wh ) const;

    pos_vec get_pos ( ) const
    {
        return this->cursor_pos;
    }

private:
    bool in_vector ( Vector4D next_vector, pos_vec current_pos ) const;

    void set_pos ( int x, int y )
    {
        set_pos ( pos_vec ( x, y ) );
    }

    void set_pos ( pos_vec p )
    {
        this->cursor_pos = p;
    }

    pos_vec cursor_pos;
};

class c_hades_menu
{
public:
    c_hades_menu ( )
    {
        //
    };
    void test_on_draw ( );

    void on_wnd_proc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    //
    //  start and end
    //
    void begin_menu ( int width = 0, int height = 0 );
    void end_menu ( );
    void colorp ( Color* clr, bool* open, float offset = 0.f );

    //
    // important components
    //
    void tab ( const std::string& title );

    int get_selected_tab ( ) const
    {
        return selected_tab;
    }

    //
    // components
    //
    /* buttons */

    template < typename Functor >
    void button ( const std::string& text, Functor lambda );
    /* sliders */
    void slider_int ( std::string label, int* i, int min, int max );
    void slider_float ( std::string label, float* f, float min, float max, int decimals = 2 );
    void slider_degrees ( std::string label, float* f, float min, float max, int decimals = 0 );
    void slider_percent ( std::string label, float* percent );
    void slider_text ( std::string label, const std::string& text, float* percent );
    /* checkboxes */
    void checkbox ( const std::string& text, bool* b );
    void checkbox_color ( ) const;
    /* dropdowns */
    void dropdown ( std::vector< std::string > items, int* selected, bool* open, bool close_on_select = true, bool enable_interaction = true );
    void dropdown ( std::string title, std::vector< std::string > items, int* selected, bool* open, bool close_on_select = true,
                    bool enable_interaction = true );
    void select_dropdown ( std::string text, std::vector< std::string > items, std::vector< bool* > selected, bool* open, bool enable_interaction = true );
    void dropdown_color ( ) const;
    /* misc */
    void title ( std::string text );
    void label ( std::string text );
    void set_section_count ( int n );
    void reset_sections ( );
    void next_section ( );
    void section ( float height = 1.f );
    void dubble_section ( );
    void trimline ( );
    void trimline ( const std::string& text );
    void trimline_title ( const std::string& text );
    void keybind ( int* key );
    void spacing ( );

    //
    // settings
    //
    //void set_margin ( int m );

private:
    bool should_render ( pos_vec v ) const;

    std::string get_name_for_key ( int key );

    int keybinds_rendered = 0;
    int keybind_selected = -1;
    int keybind_last_key_selected = -1;
    bool keybind_cancel = false;

    bool in_render = false;
    int items = 0;
    bool was_last_open = false;
    pos_vec last_mouse_pos = pos_vec{ 0, 0 };
    bool did_move_last = false;

    void hsv_to_rgb ( float h, float s, float v, float& out_r, float& out_g, float& out_b ) const;
    Color hsv_to_rgb ( float h, float s, float v );

    /* section data */
    int current_section_count = 1;
    int current_draw_section = 0;
    int current_section_width = 250;

    //int section_height = 250;
    int current_section_components_width = 246;
    float current_section_start_height = 0.f; // in percent
    float current_section_height = 0.f; // in percent
    int section_y_offset = 0;
    int section_original_x_add = 0;
    int section_original_y_add = 0;

    int last_element_y = 0;

    /* dropdown data */
    pos_vec last_dropdown_pos = { 0, 0 };
    pos_wh last_dropdown_wh = { 0, 0 };

    template < typename T >
    std::string to_string_with_precision ( const T a_value, const int n = 6 )
    {
        std::ostringstream out;
        out.precision ( n );
        out << std::fixed << a_value;
        return out.str( );
    }

    //bool in_vector ( int next_vectorx, int next_vectory, int current_posx, int current_posy );

    void add_size ( int x = 0, int y = 0 );
    pos_vec get_text_center ( vfont font, std::string text, int w, int h );

    bool was_m1_pressed = false;
    bool was_m1_last_down = false;
    Vector4D slider_last_hover = { 0, 0, 0, 0 };

    //
    // settings
    //
    int spacing_x = 4;
    int spacing_y = 4;

    Color text_unselected = Color ( 255, 255, 255, 255 );
    Color outline = Color ( 1, 1, 1, 255 );

    //
    // runtime related data lol
    //
    int current_x_add = 0;
    int current_y_add = 0;
    pos_vec menu_wh;

    int selected_tab = 0;
    int tab_count = 0;
    std::deque< std::string > tab_draw_list;

    Vector2D draw_pos = Vector2D ( 50, 50 );
};

extern hades_cursor g_cursor;
extern c_hades_menu hades_menu;
