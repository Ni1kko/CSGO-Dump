#pragma once
#include "common.h"
#include "Vector.h"
#include "matrix.h"
#define FMTFUNCTION( a, b )
class OverlayText_t;

#define member_func_args(...) (this, __VA_ARGS__ ); }
#define vfunc(index, func, sig) auto func { return reinterpret_cast<sig>((*(uint32_t**)this)[index]) member_func_args

class IVDebugOverlay
{
protected:
	~IVDebugOverlay() = default;
public:
	virtual void add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void add_box_overlay(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void add_sphere_overlay(const Vector& origin, float radius, int theta, int phi, int r, int g, int b, int a, float duration) = 0;
	virtual void add_triangle_overlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool no_depth_test, float duration) = 0;
private:
	virtual void a() = 0;
public:
	virtual void add_line_overlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool no_depth_test, float duration) = 0;

	vfunc(24, add_capsule_overlay(Vector& mins, Vector& maxs, float radius, const int r,
		const int g, const int b, const int a, const float duration), void(__thiscall*)(
			IVDebugOverlay*, Vector&, Vector&, float&, int, int, int, int, float))(mins, maxs, radius, r, g, b, a, duration)
};