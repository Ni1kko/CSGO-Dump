#pragma once

#include "sdk.h"

class VDebugOverlay
{
public:
	void AddBoxOverlay( const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Vector&, const Vector&, const Vector&, QAngle const&, int, int, int, int, float );
		getvfunc<OriginalFn>( this, 1 )( this, origin, mins, max, orientation, r, g, b, a, duration );
	}
	void AddTriangleOverlay( const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Vector&, const Vector&, const Vector&, int, int, int, int, bool, float );
		getvfunc<OriginalFn>( this, 3 )( this, p1, p2, p2, r, g, b, a, noDepthTest, duration );
	}
	void AddLineOverlay( const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration );
		getvfunc<OriginalFn>( this, 5 )( this, origin, dest, r, g, b, noDepthTest, duration );
	}
	void AddSweptBoxOverlay( const Vector& origin, const Vector& end, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Vector&, const Vector&, const Vector&, const Vector&, QAngle const&, int, int, int, int, float );
		getvfunc<OriginalFn>( this, 9 )( this, origin, end, mins, max, orientation, r, g, b, a, duration );
	}
	bool ScreenPosition( const Vector& vIn, Vector& vOut )
	{
		typedef bool( __thiscall* OriginalFn )( PVOID, const Vector&, Vector& );
		return getvfunc<OriginalFn>( this, 13 )( this, vIn, vOut );
	}
	void DrawPill( const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Vector&, const Vector&, float&, int, int, int, int, float );
		getvfunc<OriginalFn>( this, 24 )( this, mins, max, diameter, r, g, b, a, duration );
	}
};

extern VDebugOverlay* g_pDebugOverlay;