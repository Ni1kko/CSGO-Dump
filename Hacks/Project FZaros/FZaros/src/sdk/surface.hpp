#pragma once

#include "../utilities/interfaceUtilities.hpp"
#include "color.hpp"

class CSurface
{
public:
	void setDrawColor(int r, int g, int b, int a = 255)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[15])(this, r, g, b, a);
	}
	void setDrawTexture(unsigned int texture)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, unsigned int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[38])(this, texture);
	}
	void setTextColor(int r, int g, int b, int a = 255)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[25])(this, r, g, b, a);
	}
	void setTextFont(unsigned long font)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, unsigned long);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[23])(this, font);
	}
	void setTextPos(int x, int y)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[26])(this, x, y);
	}

	void rectangle(int x1, int y1, int x2, int y2)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[18])(this, x1, y1, x2, y2);
	}
	void fillRectangle(int x1, int y1, int x2, int y2)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[16])(this, x1, y1, x2, y2);
	}
	void texturedRectangle(int x1, int y1, int x2, int y2)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[41])(this, x1, y1, x2, y2);
	}
	void fadedRectangle(int x1, int y1, int x2, int y2, int start_alpha, int end_alpha, bool horizontal)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int, int, int, bool);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[123])(this, x1, y1, x2, y2, start_alpha, end_alpha, horizontal);
	}
	void drawPolygon( int n, vertex_t *vertice, bool clip_vertices = true ) {
		using original_fn = void( __thiscall* )( CSurface*, int, vertex_t*, bool );
		return ( *( original_fn** )this )[ 106 ]( this, n, vertice, clip_vertices );
	}
	void line(int x1, int y1, int x2, int y2)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[19])(this, x1, y1, x2, y2);
	}
	void polyline(int x[], int y[], int vertex_count)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int *, int *, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[20])(this, x, y, vertex_count);
	}
	void text(const wchar_t *text, int text_length)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, const wchar_t *, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[28])(this, text, text_length, 0);
	}

	void setupFont(unsigned long font, const char *name, int size, int weight, int blur, int lines, int flags, int min, int max)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, unsigned long, const char *, int, int, int, int, int, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[72])(this, font, name, size, weight, blur, lines, flags, min, max);
	}
	void setTextureData(unsigned int texture, uint8_t *texture_data, int width, int height)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, unsigned int, uint8_t *, int, int);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[37])(this, texture, texture_data, width, height);
	}

	unsigned long reserveFont()
	{
		using OriginalFn = VFUNC_SIGNATURE(unsigned long);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[71])(this);
	}
	unsigned int reserveTexture()
	{
		using OriginalFn = VFUNC_SIGNATURE(unsigned int, bool);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[43])(this, true);
	}
	int createNewTextureID( bool procedural = false ) {
		using original_fn = int( __thiscall* )( CSurface*, bool );
		return ( *( original_fn** )this )[ 43 ]( this, procedural );
	}

	void calculateTextSize(unsigned long font, const wchar_t *text, int &width, int &height)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, unsigned long, const wchar_t *, int &, int &);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[79])(this, font, text, width, height);
	}

	void unlockCursor()
	{
		using OriginalFn = VFUNC_SIGNATURE(void);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[66])(this);
	}

};