#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

#include "../sdk/color.hpp"


class CTexture
{
protected:
	unsigned int surfaceHandle;
	LPDIRECT3DTEXTURE9 dx9Handle;
	LPD3DXSPRITE dx9Sprite;

	unsigned int width, height;

public:
	CTexture(const std::vector<unsigned char>& textureData);

	unsigned int getSurfaceHandle();
	LPDIRECT3DTEXTURE9 getDX9Handle();
	LPD3DXSPRITE getDX9Sprite();

	unsigned int getWidth();
	unsigned int getHeight();

};

class CRenderManagerDX9
{
public:
	void rectangle(LPDIRECT3DDEVICE9 dx9Device, int x, int y, int w, int h, const Color& color);
	void fillRectangle(LPDIRECT3DDEVICE9 dx9Device, int x, int y, int w, int h, const Color& color);
	void gradient(LPDIRECT3DDEVICE9 dx9Device, int x, int y, int w, int h, const Color& color1, const Color& color2, bool vertical = false);
	void texturedRectangle(LPDIRECT3DDEVICE9 dx9Device, int x, int y, LPDIRECT3DTEXTURE9 texture, LPD3DXSPRITE sprite);
	void line(LPDIRECT3DDEVICE9 dx9Device, int x1, int y1, int x2, int y2, const Color& color);
	void text(LPDIRECT3DDEVICE9 dx9Device, int x, int y, unsigned int font, const std::string& str, const Color& color, bool center = false);

};

class CRenderManagerSurface
{
public:
	void rectangle(int x, int y, int w, int h, const Color& color);
	void fillRectangle(int x, int y, int w, int h, const Color& color);
	void gradient(int x, int y, int w, int h, const Color& color1, const Color& color2, bool vertical = false);
	void texturedRectangle(int x, int y, int w, int h, unsigned int texture);
	void line(int x1, int y1, int x2, int y2, const Color& color);
	void text(int x, int y, unsigned int font, const char* str, const Color& color, bool center = false);

};

class CRenderManager
{
private:
	LPDIRECT3DDEVICE9 dx9Device;
	LPDIRECT3DSTATEBLOCK9 dx9StateBlock;

	bool useSurfaceRenderer;

	CRenderManagerDX9 dx9Renderer;
	CRenderManagerSurface surfaceRenderer;

public:
	void initialize(LPDIRECT3DDEVICE9 dx9Device);
	void invalidate();

	void preRender();
	void postRender();

	void useSurface();
	void useDX9();

	bool isUsingSurface();
	bool isUsingDX9();

	void rectangle(int x, int y, int w, int h, const Color &color);
	void add_textured_polygon( int n, vertex_t * vertice, Color colour );
	void fillTriangle( std::array<Vector2, 3> points, Color colour );
	void fillRectangle(int x, int y, int w, int h, const Color &color);
	void gradient(int x, int y, int w, int h, const Color &color1, const Color &color2, bool vertical = false);
	void texturedRectangle(int x, int y, CTexture *texture);
	void line(int x1, int y1, int x2, int y2, const Color &color);
	void text(int x, int y, unsigned int font, const std::string &str, const Color &color, bool center = false);

};

enum ESurfaceFlags
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
	FONTFLAG_CUSTOM = 0x400,		// custom generated font - never fall back to asian compatibility mode
	FONTFLAG_BITMAP = 0x800,		// compiled bitmap font - no fallbacks
};

class CFontManager
{
public:
	unsigned int visuals;
	unsigned int visualsWeapon;
	unsigned int visualsWeaponIcon;
	unsigned int visualsFlags;
	unsigned int visualsAmmoText;
	unsigned int visualsAlternate;
	unsigned int spectators;
	unsigned int log;
	unsigned int watermark;
	unsigned int indicator;

	void createFonts();

};

class CTextureManager
{
public:
	CTexture *testTexture;

	void createTextures();

};

extern CRenderManager* g_Renderer;
extern CFontManager* g_Fonts;
extern CTextureManager* g_Textures;