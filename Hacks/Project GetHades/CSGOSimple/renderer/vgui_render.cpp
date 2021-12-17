#include "vgui_render.h"
#include "../helpers/math.hpp"
#include "../helpers/console.h"

vgui_renderer g_render;
fonts g_fonts;

void vgui_renderer::text ( const vfont font, std::string text, const float x, const float y, const Color clr ) const
{
    if ( !font.initialized )
        return;

    interfaces::vgui_surface->draw_set_text_font ( font.hfont );
    interfaces::vgui_surface->draw_set_text_color ( clr );

    interfaces::vgui_surface->draw_set_text_pos (
        static_cast< int > ( x ),
        static_cast< int > ( y )
    );
    interfaces::vgui_surface->draw_print_text ( std::wstring ( text.begin( ), text.end( ) ).data( ), text.size( ) );
}

void vgui_renderer::add_triangle_filled ( std::array< Vector2D, 3 > points, const Color color )
{
    std::array< Vertex_t, 3 > vertices{ Vertex_t ( points.at ( 0 ) ), Vertex_t ( points.at ( 1 ) ), Vertex_t ( points.at ( 2 ) ) };
    add_textured_polygon ( 3, vertices.data( ), color );
}

void vgui_renderer::add_textured_polygon ( int n, Vertex_t* vertice, const Color color ) const
{
    static auto texture = interfaces::vgui_surface->create_new_texture_id ( true );

    static unsigned char buf[4] = { 255, 255, 255, 255 };

    interfaces::vgui_surface->draw_set_texture_rgba ( texture, buf, 1, 1 );
    interfaces::vgui_surface->draw_set_color ( color );
    interfaces::vgui_surface->draw_set_texture ( texture );

    interfaces::vgui_surface->draw_textured_polygon ( n, vertice );
}

void vgui_renderer::polygon ( const int count, Vertex_t* vertexs, const Color color ) const
{
    static auto texture = interfaces::vgui_surface->create_new_texture_id ( true );
    static unsigned char buffer[4] = { 255, 255, 255, 255 };

    interfaces::vgui_surface->draw_set_texture_rgba ( texture, buffer, 1, 1 );
    interfaces::vgui_surface->draw_set_color ( color );
    interfaces::vgui_surface->draw_set_texture ( texture );

    interfaces::vgui_surface->draw_textured_polygon ( count, vertexs );
}

void vgui_renderer::line ( const float x1, const float y1, const float x2, const float y2, const Color clr, const float size ) const
{
    interfaces::vgui_surface->draw_set_color ( clr );

    if ( size == 1.f )
        interfaces::vgui_surface->draw_line (
            static_cast< int > ( std::ceil ( x1 ) ),
            static_cast< int > ( std::ceil ( y1 ) ),
            static_cast< int > ( std::ceil ( x2 ) ),
            static_cast< int > ( std::ceil ( y2 ) )
        );
    else
        interfaces::vgui_surface->draw_filled_rect (
            static_cast< int > ( std::ceil ( x1 - ( size / 2.f ) ) ),
            static_cast< int > ( std::ceil ( y1 - ( size / 2.f ) ) ),
            static_cast< int > ( std::ceil ( x2 + ( size / 2.f ) ) ),
            static_cast< int > ( std::ceil ( y2 + ( size / 2.f ) ) )
        );
}

void vgui_renderer::box ( const float x1, const float y1, const float x2, const float y2, const Color clr, const float size ) const
{
    line ( x1, y1, x2, y1, clr, size );
    line ( x1, y2, x2, y2, clr, size );
    line ( x1, y1, x1, y2, clr, size );
    line ( x2, y1, x2, y2, clr, size );
}

void vgui_renderer::box_wh ( const float x, const float y, const float w, const float h, const Color clr, const float size ) const
{
    box ( x, y, x + w, y + h, clr, size );
}

void vgui_renderer::filled_box ( const float x1, const float y1, const float x2, const float y2, const Color clr ) const
{
    interfaces::vgui_surface->draw_set_color ( clr );
    interfaces::vgui_surface->draw_filled_rect (
        static_cast< int > ( std::ceil ( x1 ) ),
        static_cast< int > ( std::ceil ( y1 ) ),
        static_cast< int > ( std::ceil ( x2 ) ),
        static_cast< int > ( std::ceil ( y2 ) )
    );
}

void vgui_renderer::filled_box_wh ( const float x, const float y, const float w, const float h, const Color clr )
{
    filled_box ( x, y, x + w, y + h, clr );
}

void vgui_renderer::filled_box ( const Vector4D vec, const Color clr )
{
    filled_box ( vec.x, vec.y, vec.z, vec.w, clr );
}

void vgui_renderer::gradient_vertical ( const float x1, const float y1, const float x2, const float y2, const Color clr1, const Color clr2 )
{
    gradient_vertical_wh ( x1, y1, x2 - x1, y2 - y1, clr1, clr2 );
}

void vgui_renderer::gradient_horizontal ( const float x1, const float y1, const float x2, const float y2, const Color clr1, const Color clr2 )
{
    gradient_horizontal_wh ( x1, y1, x2 - x1, y2 - y1, clr1, clr2 );
}

void vgui_renderer::gradient_vertical_wh ( float x, float y, float w, float h, Color clr1, Color clr2 )
{
    filled_box ( x, y, w, h, clr1 );
    const auto first = clr2.r( );
    const auto second = clr2.g( );
    const auto third = clr2.b( );

    for ( auto i = 0; i < h; i++ )
    {
        const auto fi = i;
        const auto fh = h;
        const auto a = fi / fh;
        const auto ia = static_cast< int > ( a * 255.f );
        filled_box_wh ( x, y + i, w, 1, Color ( first, second, third, ia ) );
    }
}

void vgui_renderer::gradient_horizontal_wh ( float x, float y, float w, float h, Color clr1, Color clr2 )
{
    filled_box ( x, y, w, h, clr1 );
    const auto first = clr2.r( );
    const auto second = clr2.g( );
    const auto third = clr2.b( );

    for ( auto i = 0; i < h; i++ )
    {
        const auto fi = i;
        const auto fw = w;
        const auto a = fi / fw;
        const auto ia = static_cast< int > ( a * 255.f );
        filled_box_wh ( x + i, y, 1, h, Color ( first, second, third, ia ) );
    }
}

void vgui_renderer::circle ( const float x, const float y, const float r, const int seg, const Color clr ) const
{
    interfaces::vgui_surface->draw_set_color ( clr );
    interfaces::vgui_surface->draw_outlined_circle ( x, y, r, seg );
}

void vgui_renderer::circle_3d ( const Vector pos, const float points, const float radius, const Color clr ) const
{
    auto const step = static_cast< float > ( m_pi ) * 2.0f / points;

    for ( float a = 0; a < ( m_pi * 2.0f ); a += step )
    {
        Vector start ( radius * cosf ( a ) + pos.x, radius * sinf ( a ) + pos.y, pos.z );
        Vector end ( radius * cosf ( a + step ) + pos.x, radius * sinf ( a + step ) + pos.y, pos.z );

        Vector start2d, end2d;

        if ( interfaces::debug_overlay->screen_position ( start, start2d ) || interfaces::debug_overlay->screen_position ( end, end2d ) )
            return;

        line ( start2d.x, start2d.y, end2d.x, end2d.y, clr, 1.f );
    }
}

Vector2D vgui_renderer::get_size ( const vfont font, const std::string text ) const
{
    if ( !font.initialized )
        return Vector2D ( 0, 0 );

    int x, y;
    interfaces::vgui_surface->get_text_size ( font.hfont, std::wstring ( text.begin( ), text.end( ) ).c_str( ), x, y );

    return Vector2D ( x, y );
}

pos_vec vgui_renderer::get_screen_center ( ) const
{
    if ( !interfaces::engine_client )
        return pos_vec ( 0, 0 );

    int x, y;
    interfaces::engine_client->get_screen_size ( x, y );

    return pos_vec ( x / 2, y / 2 );
}

void vgui_renderer::setup_states ( ) const
{
    D3DVIEWPORT9 d3d_viewport;
    this->m_device->GetViewport ( &d3d_viewport );

    this->m_device->SetVertexShader ( nullptr );
    this->m_device->SetPixelShader ( nullptr );
    this->m_device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

    this->m_device->SetRenderState ( D3DRS_LIGHTING, FALSE );
    this->m_device->SetRenderState ( D3DRS_FOGENABLE, FALSE );
    this->m_device->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
    this->m_device->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );

    this->m_device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE );
    this->m_device->SetRenderState ( D3DRS_SCISSORTESTENABLE, TRUE );
    this->m_device->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );
    this->m_device->SetRenderState ( D3DRS_STENCILENABLE, FALSE );

    this->m_device->SetRenderState ( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
    this->m_device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE, TRUE );

    this->m_device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
    this->m_device->SetRenderState ( D3DRS_ALPHATESTENABLE, FALSE );
    this->m_device->SetRenderState ( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
    this->m_device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    this->m_device->SetRenderState ( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
    this->m_device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    this->m_device->SetRenderState ( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

    this->m_device->SetRenderState ( D3DRS_SRGBWRITEENABLE, FALSE );
    this->m_device->SetRenderState ( D3DRS_COLORWRITEENABLE,
                                     D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
}

void vgui_renderer::initalize_objects ( LPDIRECT3DDEVICE9 m_device )
{
    this->m_device = m_device;

    this->m_device->GetViewport ( &m_viewport );
}

void vgui_renderer::invalidate_objects ( )
{
    this->m_device = nullptr;
}

void vgui_renderer::restore_objects ( LPDIRECT3DDEVICE9 m_device )
{
    this->m_device = m_device;
    this->m_device->GetViewport ( &m_viewport );
}

void vgui_renderer::circle_dual_colour ( float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device )
{
    LPDIRECT3DVERTEXBUFFER9 g_pVB2;

    std::vector< CUSTOMVERTEX > circle ( resolution + 2 );

    float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

    if ( type == 1 )
        pi = D3DX_PI; // Full circle

    if ( type == 2 )
        pi = D3DX_PI / 2; // 1/2 circle

    if ( type == 3 )
        pi = D3DX_PI / 4; // 1/4 circle

    pi = D3DX_PI / type; // 1/4 circle

    circle[ 0 ].x = x;
    circle[ 0 ].y = y;
    circle[ 0 ].z = 0;
    circle[ 0 ].rhw = 1;
    circle[ 0 ].color = D3DCOLOR_RGBA ( 0, 0, 0, 0 );

    float hue = 0.f;

    for ( int i = 1; i < resolution + 2; i++ )
    {
        circle[ i ].x = ( float )( x - rad * cos ( pi * ( ( i - 1 ) / ( resolution / 2.0f ) ) ) );
        circle[ i ].y = ( float )( y - rad * sin ( pi * ( ( i - 1 ) / ( resolution / 2.0f ) ) ) );
        circle[ i ].z = 0;
        circle[ i ].rhw = 1;

        auto clr = Color::FromHSB ( hue, 1.f, 1.f );
        circle[ i ].color = D3DCOLOR_RGBA ( clr.r( ), clr.g( ), clr.b( ), 150 );
        hue += 0.02;
    }

    // Rotate matrix
    int _res = resolution + 2;

    for ( int i = 0; i < _res; i++ )
    {
        float Vx1 = x + ( cosf ( angle ) * ( circle[ i ].x - x ) - sinf ( angle ) * ( circle[ i ].y - y ) );
        float Vy1 = y + ( sinf ( angle ) * ( circle[ i ].x - x ) + cosf ( angle ) * ( circle[ i ].y - y ) );

        circle[ i ].x = Vx1;
        circle[ i ].y = Vy1;
    }

    m_device->CreateVertexBuffer ( ( resolution + 2 ) * sizeof ( CUSTOMVERTEX ), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
                                   &g_pVB2, NULL );

    VOID* pVertices;
    g_pVB2->Lock ( 0, ( resolution + 2 ) * sizeof ( CUSTOMVERTEX ), ( void** )&pVertices, 0 );
    memcpy ( pVertices, &circle[ 0 ], ( resolution + 2 ) * sizeof ( CUSTOMVERTEX ) );
    g_pVB2->Unlock( );

    m_device->SetTexture ( 0, NULL );
    m_device->SetPixelShader ( NULL );
    m_device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
    m_device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    m_device->SetStreamSource ( 0, g_pVB2, 0, sizeof ( CUSTOMVERTEX ) );
    m_device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
    m_device->DrawPrimitive ( D3DPT_TRIANGLEFAN, 0, resolution );

    if ( g_pVB2 != NULL )
        g_pVB2->Release( );
}

void vgui_renderer::circle_dual_colour ( float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2,
                                         IDirect3DDevice9* m_device )
{
    LPDIRECT3DVERTEXBUFFER9 g_pVB2;

    std::vector< CUSTOMVERTEX > circle ( resolution + 2 );

    float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

    if ( type == 1 )
        pi = D3DX_PI; // Full circle

    if ( type == 2 )
        pi = D3DX_PI / 2; // 1/2 circle

    if ( type == 3 )
        pi = D3DX_PI / 4; // 1/4 circle

    pi = D3DX_PI / type; // 1/4 circle

    circle[ 0 ].x = x;
    circle[ 0 ].y = y;
    circle[ 0 ].z = 0;
    circle[ 0 ].rhw = 1;
    circle[ 0 ].color = color2;

    for ( int i = 1; i < resolution + 2; i++ )
    {
        circle[ i ].x = ( float )( x - rad * cos ( pi * ( ( i - 1 ) / ( resolution / 2.0f ) ) ) );
        circle[ i ].y = ( float )( y - rad * sin ( pi * ( ( i - 1 ) / ( resolution / 2.0f ) ) ) );
        circle[ i ].z = 0;
        circle[ i ].rhw = 1;
        circle[ i ].color = color;
    }

    // Rotate matrix
    int _res = resolution + 2;

    for ( int i = 0; i < _res; i++ )
    {
        circle[ i ].x = x + cos ( angle ) * ( circle[ i ].x - x ) - sin ( angle ) * ( circle[ i ].y - y );
        circle[ i ].y = y + sin ( angle ) * ( circle[ i ].x - x ) + cos ( angle ) * ( circle[ i ].y - y );
    }

    m_device->CreateVertexBuffer ( ( resolution + 2 ) * sizeof ( CUSTOMVERTEX ), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
                                   &g_pVB2, NULL );

    VOID* pVertices;
    g_pVB2->Lock ( 0, ( resolution + 2 ) * sizeof ( CUSTOMVERTEX ), ( void** )&pVertices, 0 );
    memcpy ( pVertices, &circle[ 0 ], ( resolution + 2 ) * sizeof ( CUSTOMVERTEX ) );
    g_pVB2->Unlock( );

    m_device->SetTexture ( 0, NULL );
    m_device->SetPixelShader ( NULL );
    m_device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
    m_device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    m_device->SetStreamSource ( 0, g_pVB2, 0, sizeof ( CUSTOMVERTEX ) );
    m_device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
    m_device->DrawPrimitive ( D3DPT_TRIANGLEFAN, 0, resolution );

    if ( g_pVB2 != NULL )
        g_pVB2->Release( );
}
