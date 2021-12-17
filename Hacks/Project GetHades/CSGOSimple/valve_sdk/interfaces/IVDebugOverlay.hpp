#pragma once

#include "../math/Vector.hpp"
#include "../math/QAngle.hpp"
#include "../math/VMatrix.hpp"

class OverlayText_t;

class IVDebugOverlay
{
public:
    virtual void __unkn ( ) = 0;
    virtual void add_entity_text_overlay ( int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ... ) = 0;
    virtual void add_box_overlay ( const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a,
                                   float duration ) = 0;
    virtual void add_sphere_overlay ( const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration ) = 0;
    virtual void add_triangle_overlay ( const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest,
                                        float duration ) = 0;
    virtual void add_line_overlay ( const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration ) = 0;
    virtual void add_text_overlay ( const Vector& origin, float duration, const char* format, ... ) = 0;
    virtual void add_text_overlay ( const Vector& origin, int line_offset, float duration, const char* format, ... ) = 0;
    virtual void add_screen_text_overlay ( float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text ) = 0;
    virtual void add_swept_box_overlay ( const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle& angles, int r, int g,
                                         int b, int a, float flDuration ) = 0;
    virtual void add_grid_overlay ( const Vector& origin ) = 0;
    virtual void add_coord_frame_overlay ( const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL ) = 0;
    virtual int screen_position ( const Vector& point, Vector& screen ) = 0;
    virtual int screen_position ( float flXPos, float flYPos, Vector& screen ) = 0;
    virtual OverlayText_t* get_first ( void ) = 0;
    virtual OverlayText_t* get_next ( OverlayText_t* current ) = 0;
    virtual void clear_dead_overlays ( void ) = 0;
    virtual void clear_all_overlays ( ) = 0;
    virtual void add_text_overlay_rgb ( const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format,
                                        ... ) = 0;
    virtual void add_text_overlay_rgb ( const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ... ) = 0;
    virtual void add_line_overlay_alpha ( const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration ) = 0;
    virtual void add_box_overlay2 ( const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, const uint8_t* faceColor,
                                    const uint8_t* edgeColor, float duration ) = 0;
    virtual void ourge_text_overlays ( ) = 0;
    //virtual void            draw_pill ( const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration ) = 0;
    void draw_pill ( const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration )
    {
        typedef void ( __thiscall * OriginalFn ) ( PVOID, const Vector&, const Vector&, float&, int, int, int, int, float );
        CallVFunction< OriginalFn > ( this, 23 ) ( this, mins, max, diameter, r, g, b, a, duration );
    }
};
