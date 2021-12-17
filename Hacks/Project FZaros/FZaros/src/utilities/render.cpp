#include "../sdk/vector.hpp"
#include "../sdk/color.hpp"
#include "../core/managers/interfaceManager.hpp"
#include "../thirdparty/lodepng.h"
#include "render.hpp"
#include <array>

class Vertex2D
{
public:
	Vector3 position;
	float w;
	int color;

};

#pragma region Texture implementation

CTexture::CTexture( const std::vector<unsigned char>& textureData )
{
	std::vector<unsigned char> decodedTextureData;
	lodepng::decode( decodedTextureData, width, height, textureData );

	this->surfaceHandle = g_Interfaces->surface->reserveTexture( );
	g_Interfaces->surface->setTextureData( this->surfaceHandle, ( uint8_t * )decodedTextureData.data( ), width, height );

	D3DXCreateTextureFromFileInMemoryEx( ( LPDIRECT3DDEVICE9 )g_Interfaces->dx9Device, textureData.data( ), textureData.size( ),
		width, height, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_RGBA( 0, 0, 0, 255 ), NULL, NULL, &this->dx9Handle );
	D3DXCreateSprite( ( LPDIRECT3DDEVICE9 )g_Interfaces->dx9Device, &this->dx9Sprite );
}

unsigned int CTexture::getSurfaceHandle( )
{
	return this->surfaceHandle;
}

LPDIRECT3DTEXTURE9 CTexture::getDX9Handle( )
{
	return this->dx9Handle;
}

LPD3DXSPRITE CTexture::getDX9Sprite( )
{
	return this->dx9Sprite;
}

unsigned int CTexture::getWidth( )
{
	return this->width;
}

unsigned int CTexture::getHeight( )
{
	return this->height;
}

#pragma endregion

#pragma region Surface renderer

void CRenderManagerSurface::rectangle( int x, int y, int w, int h, const Color& color )
{
	g_Interfaces->surface->setDrawColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_Interfaces->surface->rectangle( x, y, x + w, y + h );
}

void CRenderManagerSurface::fillRectangle( int x, int y, int w, int h, const Color & color )
{
	g_Interfaces->surface->setDrawColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_Interfaces->surface->fillRectangle( x, y, x + w, y + h );
}

void CRenderManagerSurface::gradient( int x, int y, int w, int h, const Color & color1, const Color & color2, bool vertical )
{
	g_Interfaces->surface->setDrawColor( color1.r( ), color1.g( ), color1.b( ), color1.a( ) );
	g_Interfaces->surface->fadedRectangle( x, y, x + w, y + h, 255, 255, !vertical );

	g_Interfaces->surface->setDrawColor( color2.r( ), color2.g( ), color2.b( ), color2.a( ) );
	g_Interfaces->surface->fadedRectangle( x, y, x + w, y + h, 0, 255, !vertical );
}

void CRenderManagerSurface::texturedRectangle( int x, int y, int w, int h, unsigned int texture )
{
	g_Interfaces->surface->setDrawColor( 255, 255, 255 );
	g_Interfaces->surface->setDrawTexture( texture );
	g_Interfaces->surface->texturedRectangle( x, y, x + w, y + h );
}

void CRenderManagerSurface::line( int x1, int y1, int x2, int y2, const Color & color )
{
	g_Interfaces->surface->setDrawColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_Interfaces->surface->line( x1, y1, x2, y2 );
}

void CRenderManagerSurface::text( int x, int y, unsigned int font, const char* str, const Color & color, bool center )
{
	int apple = 0;
	char Buffer[ 2048 ] = { '\0' };
	va_list Args;

	va_start( Args, str );
	vsprintf_s( Buffer, str, Args );
	va_end( Args );

	size_t Size = strlen( Buffer ) + 1;
	wchar_t* WideBuffer = new wchar_t[ Size ];

	mbstowcs_s( 0, WideBuffer, Size, Buffer, Size - 1 );

	int Width = 0, Height = 0;

	if ( center )
		g_Interfaces->surface->calculateTextSize( font, WideBuffer, Width, Height );

	g_Interfaces->surface->setTextColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_Interfaces->surface->setTextFont( font );
	g_Interfaces->surface->setTextPos( x - ( Width / 2 ), y );
	g_Interfaces->surface->text( WideBuffer, wcslen( WideBuffer ) );
}

#pragma endregion

#pragma region DX9 renderer

void CRenderManagerDX9::rectangle( LPDIRECT3DDEVICE9 dx9Device, int x, int y, int w, int h, const Color& color )
{
	int colorInt = D3DCOLOR_ARGB( color.a( ), color.r( ), color.g( ), color.b( ) );
	Vertex2D vertices[ 5 ] =
	{
		Vertex2D { Vector3( ( float )x, ( float )y, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x + ( float )w, ( float )y, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x + ( float )w, ( float )y + ( float )h, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x, ( float )y + ( float )h, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x, ( float )y, 1.0f ), 1.0f, colorInt },
	};

	dx9Device->SetTexture( 0, nullptr );
	dx9Device->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, &vertices, sizeof( Vertex2D ) );
}

void CRenderManagerDX9::fillRectangle( LPDIRECT3DDEVICE9 dx9Device, int x, int y, int w, int h, const Color & color )
{
	int colorInt = D3DCOLOR_ARGB( color.a( ), color.r( ), color.g( ), color.b( ) );
	Vertex2D vertices[ 5 ] =
	{
		Vertex2D { Vector3( ( float )x, ( float )y, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x + ( float )w, ( float )y, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x, ( float )y + ( float )h, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x + ( float )w, y + ( float )h, 1.0f ), 1.0f, colorInt },
	};

	dx9Device->SetTexture( 0, nullptr );
	dx9Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vertices, sizeof( Vertex2D ) );
}

void CRenderManagerDX9::gradient( LPDIRECT3DDEVICE9 dx9Device, int x, int y, int w, int h, const Color & color1, const Color & color2, bool vertical )
{
	int color1Int = D3DCOLOR_ARGB( color1.a( ), color1.r( ), color1.g( ), color1.b( ) );
	int color2Int = D3DCOLOR_ARGB( color2.a( ), color2.r( ), color2.g( ), color2.b( ) );

	Vertex2D vertices[ 5 ] =
	{
		Vertex2D { Vector3( ( float )x, ( float )y, 1.0f ), 1.0f, color1Int },
		Vertex2D { Vector3( ( float )x + ( float )w, ( float )y, 1.0f ), 1.0f, vertical ? color1Int : color2Int },
		Vertex2D { Vector3( ( float )x, ( float )y + ( float )h, 1.0f ), 1.0f, vertical ? color2Int : color1Int },
		Vertex2D { Vector3( ( float )x + ( float )w, ( float )y + ( float )h, 1.0f ), 1.0f, color2Int },
	};

	dx9Device->SetTexture( 0, nullptr );
	dx9Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vertices, sizeof( Vertex2D ) );
}

void CRenderManagerDX9::texturedRectangle( LPDIRECT3DDEVICE9 dx9Device, int x, int y, LPDIRECT3DTEXTURE9 texture, LPD3DXSPRITE sprite )
{
	D3DXVECTOR3 position( ( float )x, ( float )y, 1.0f );
	D3DXMATRIX transformationMatrix;

	D3DXMatrixTransformation( &transformationMatrix, NULL, NULL, NULL, NULL, NULL, &position );

	sprite->SetTransform( &transformationMatrix );
	sprite->Begin( D3DXSPRITE_ALPHABLEND );
	sprite->Draw( texture, NULL, NULL, NULL, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	sprite->End( );
}

void CRenderManagerDX9::line( LPDIRECT3DDEVICE9 dx9Device, int x1, int y1, int x2, int y2, const Color & color )
{
	int colorInt = D3DCOLOR_ARGB( color.a( ), color.r( ), color.g( ), color.b( ) );
	Vertex2D vertices[ 2 ] =
	{
		Vertex2D { Vector3( ( float )x1, ( float )y1, 1.0f ), 1.0f, colorInt },
		Vertex2D { Vector3( ( float )x2, ( float )y2, 1.0f ), 1.0f, colorInt }
	};

	dx9Device->SetTexture( 0, nullptr );
	dx9Device->DrawPrimitiveUP( D3DPT_LINELIST, 1, &vertices, sizeof( Vertex2D ) );
}

void CRenderManagerDX9::text( LPDIRECT3DDEVICE9 dx9Device, int x, int y, unsigned int font, const std::string & str, const Color & color, bool center )
{
	// todo
}

#pragma endregion

#pragma region Renderer ... ?

void CRenderManager::initialize( LPDIRECT3DDEVICE9 dx9Device )
{
	if ( this->isUsingDX9( ) && dx9Device != nullptr )
		this->dx9Device = dx9Device;

	g_Fonts->createFonts( );
	//g_Textures->createTextures();
}

void CRenderManager::invalidate( )
{
	// todo
	// g_Fonts->destroyFonts();
	// g_Textures->destroyTextures();
}

void CRenderManager::preRender( )
{
	this->dx9Device->CreateStateBlock( D3DSBT_PIXELSTATE, &this->dx9StateBlock );
	this->dx9StateBlock->Capture( );

	this->dx9Device->SetVertexShader( nullptr );
	this->dx9Device->SetPixelShader( nullptr );
	this->dx9Device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	this->dx9Device->SetRenderState( D3DRS_LIGHTING, FALSE );
	this->dx9Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	this->dx9Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	this->dx9Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	this->dx9Device->SetRenderState( D3DRS_ZENABLE, FALSE );
	this->dx9Device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	this->dx9Device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	this->dx9Device->SetRenderState( D3DRS_STENCILENABLE, FALSE );
	this->dx9Device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
	this->dx9Device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
	this->dx9Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	this->dx9Device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	this->dx9Device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
	this->dx9Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	this->dx9Device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	this->dx9Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	this->dx9Device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );
	this->dx9Device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	this->dx9Device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
		D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
}

void CRenderManager::postRender( )
{
	this->dx9StateBlock->Apply( );
	this->dx9StateBlock->Release( );
}

void CRenderManager::useSurface( )
{
	this->useSurfaceRenderer = true;
}

void CRenderManager::useDX9( )
{
	this->useSurfaceRenderer = false;
}

bool CRenderManager::isUsingSurface( )
{
	return this->useSurfaceRenderer;
}

bool CRenderManager::isUsingDX9( )
{
	return !this->useSurfaceRenderer;
}

void CRenderManager::rectangle( int x, int y, int w, int h, const Color &color )
{
	if ( this->isUsingSurface( ) )
		this->surfaceRenderer.rectangle( x, y, w, h, color );
	else
		this->dx9Renderer.rectangle( this->dx9Device, x, y, w - 1, h - 1, color );
}

void CRenderManager::add_textured_polygon( int n, vertex_t * vertice, Color colour ) {
	static int texture_id = g_Interfaces->surface->reserveTexture( );
	static unsigned char buf[ 4 ] = { 255, 255, 255, 255 };
	g_Interfaces->surface->setDrawColor( colour.r( ), colour.g( ), colour.b( ), colour.a( ) );
	g_Interfaces->surface->setDrawTexture( texture_id );
	g_Interfaces->surface->drawPolygon( n, vertice );
}

void CRenderManager::fillTriangle( std::array< Vector2, 3 >points, Color colour ) {
	std::array< vertex_t, 3 >vertices{ vertex_t( points.at( 0 ) ), vertex_t( points.at( 1 ) ), vertex_t( points.at( 2 ) ) };
	add_textured_polygon( 3, vertices.data( ), colour );
}

void CRenderManager::fillRectangle( int x, int y, int w, int h, const Color &color )
{
	if ( this->isUsingSurface( ) )
		this->surfaceRenderer.fillRectangle( x, y, w, h, color );
	else
		this->dx9Renderer.fillRectangle( this->dx9Device, x, y, w, h, color );
}

void CRenderManager::gradient( int x, int y, int w, int h, const Color &color1, const Color &color2, bool vertical )
{
	if ( this->isUsingSurface( ) )
		this->surfaceRenderer.gradient( x, y, w, h, color1, color2 );
	else
		this->dx9Renderer.gradient( this->dx9Device, x, y, w, h, color1, color2 );
}

void CRenderManager::texturedRectangle( int x, int y, CTexture* texture )
{
	if ( this->isUsingSurface( ) )
		this->surfaceRenderer.texturedRectangle( x, y, texture->getWidth( ), texture->getHeight( ), texture->getSurfaceHandle( ) );
	else
		this->dx9Renderer.texturedRectangle( this->dx9Device, x, y, texture->getDX9Handle( ), texture->getDX9Sprite( ) );
}

void CRenderManager::line( int x1, int y1, int x2, int y2, const Color &color )
{
	if ( this->isUsingSurface( ) )
		this->surfaceRenderer.line( x1, y1, x2, y2, color );
	else
		this->dx9Renderer.line( this->dx9Device, x1, y1, x2, y2, color );
}

void CRenderManager::text( int x, int y, unsigned int font, const std::string &str, const Color &color, bool center )
{
	if ( this->isUsingSurface( ) )
		this->surfaceRenderer.text( x, y, font, str.c_str( ), color, center );
	else
		this->dx9Renderer.text( this->dx9Device, x, y, font, str, color );
}

#pragma endregion

void CFontManager::createFonts( )
{
	this->visuals = g_Interfaces->surface->reserveFont( );
	this->visualsWeapon = g_Interfaces->surface->reserveFont( );
	this->visualsFlags = g_Interfaces->surface->reserveFont( );
	this->visualsAmmoText = g_Interfaces->surface->reserveFont( );
	this->spectators = g_Interfaces->surface->reserveFont( );
	this->log = g_Interfaces->surface->reserveFont( );
	this->watermark = g_Interfaces->surface->reserveFont( );
	this->indicator = g_Interfaces->surface->reserveFont( );
	this->visualsWeaponIcon = g_Interfaces->surface->reserveFont( );

	g_Interfaces->surface->setupFont( this->visuals, "Verdana", 12, 700, 0, 0, ESurfaceFlags::FONTFLAG_DROPSHADOW, 0, 0 );
	g_Interfaces->surface->setupFont( this->visualsWeapon, "Small Fonts", 9, FW_MEDIUM, 0, 0, ESurfaceFlags::FONTFLAG_OUTLINE, 0, 0 );
	g_Interfaces->surface->setupFont( this->visualsFlags, "Small Fonts", 9, FW_MEDIUM, 0, 0, ESurfaceFlags::FONTFLAG_OUTLINE, 0, 0 );
	g_Interfaces->surface->setupFont( this->visualsAmmoText, "Small Fonts", 9, FW_MEDIUM, 0, 0, ESurfaceFlags::FONTFLAG_OUTLINE, 0, 0 );
	g_Interfaces->surface->setupFont( this->spectators, "Tahoma", 12, FW_MEDIUM, 0, 0, ESurfaceFlags::FONTFLAG_DROPSHADOW, 0, 0 );
	g_Interfaces->surface->setupFont( this->log, "Tahoma", 12, FW_MEDIUM, 0, 0, ESurfaceFlags::FONTFLAG_DROPSHADOW, 0, 0 );
	g_Interfaces->surface->setupFont( this->watermark, "Verdana", 15, FW_MEDIUM, 0, 0, ESurfaceFlags::FONTFLAG_ANTIALIAS, 0, 0 );
	g_Interfaces->surface->setupFont( this->indicator, "Verdana", 30, FW_EXTRABOLD, 0, 0, ESurfaceFlags::FONTFLAG_ANTIALIAS | ESurfaceFlags::FONTFLAG_DROPSHADOW, 0, 0 );
	g_Interfaces->surface->setupFont( this->visualsWeaponIcon, "Counter-Strike", 28, 0, 0, 0, ESurfaceFlags::FONTFLAG_ANTIALIAS | ESurfaceFlags::FONTFLAG_DROPSHADOW, 0, 0 );

	this->visualsAlternate = g_Interfaces->surface->reserveFont( );
	g_Interfaces->surface->setupFont( this->visualsAlternate, "Tahoma", 12, 500, 0, 0, ESurfaceFlags::FONTFLAG_ANTIALIAS | ESurfaceFlags::FONTFLAG_DROPSHADOW, 0, 0 );

}

void CTextureManager::createTextures( )
{

}

CRenderManager* g_Renderer = new CRenderManager( );
CFontManager* g_Fonts = new CFontManager( );
CTextureManager* g_Textures = new CTextureManager( );