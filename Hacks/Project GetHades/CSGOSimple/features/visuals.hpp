#pragma once
#include "../valve_sdk/csgostructs.hpp"
#include "../renderer/vgui_render.h"

enum noscope_line_t
{
    none,
    static_lines,
    dynamic_lines
};

class c_visuals
{
    CRITICAL_SECTION cs_{ };
public:
    c_visuals ( );
    ~c_visuals ( );

    Vector2D get_screensize ( ) const;
    void update_screensize ( );

    class Player
    {
    public:
        struct
        {
            C_BasePlayer* pl{ };
            bool is_enemy{ };
            bool is_visible{ };
            Color clr;
            Vector head_pos;
            //Vector        feet_pos;
            Vector origin_pos;
            Vector eye_pos;
            RECT bbox{ };
            int left_offset = 0;
            float percent = 1.f;
        } ctx;

        bool begin ( C_BasePlayer* pl );
        void render_box ( ) const;

        static void draw_crosshair ( );

        void render_name ( ) const;
        void render_health ( );
        void render_armor ( );
        void render_ammo ( ) const;
        void render_weapon ( ) const;
        void render_flags ( );
        void draw_damage ( C_BaseEntity* ent ) const;
        //void render_ground_weapon ( C_BaseCombatWeapon* ent );
        void draw_debug ( );
    };

    std::string get_name_for_weapon ( C_BaseCombatWeapon* entity ) const;
    std::string get_weapon_icon ( C_BaseCombatWeapon* entity ) const;

    struct damage_indicator_t
    {
        int damage = 0;
        float start_time = 0.f;
        Vector position;
        bool hs = false;
        int hs_add = 0;
        int hs_add_x = 0;
    };

    void add_damage_data ( int i, int damage, Vector position, bool headshot );
    std::array< std::vector< damage_indicator_t >, 64 > damage_data;

    static void inaccuracy ( IDirect3DDevice9* m_device );
    void draw_offscreenesp ( ) const;
    void render_ground_weapon ( C_BaseCombatWeapon* ent ) const;
    void draw_hitmarker ( ) const;
    void draw_grenades ( C_BaseCombatWeapon* nade ) const;
    void draw_indicators ( );
    void draw_scopelines ( ) const;
    void draw_manual_aa_indicators ( );
    void draw_angle_lines ( ) const;
    static void third_person ( );
    void watermark ( ) const;
    void add_to_draw_list ( );

private:
    struct
    {
        float time = 0.f;
        bool was_last = false;
    } dormant_data[64];

    vfont f_normal_;
    int screen_x_{ }, screen_y_{ };
};
