#pragma once


class OverlayText_t;

class IVDebugOverlay
{
public:
    virtual void            addEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) = 0;
    virtual void            addBoxOverlay(const Vector3& origin, const Vector3& mins, const Vector3& max, Vector3 const& orientation, int r, int g, int b, int a, float duration) = 0;
    virtual void            addSphereOverlay(const Vector3& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
    virtual void            addTriangleOverlay(const Vector3& p1, const Vector3& p2, const Vector3& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void            __unkn() = 0;
	virtual void            addLineOverlay(const Vector3& origin, const Vector3& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
    virtual void            addTextOverlay(const Vector3& origin, float duration, const char *format, ...) = 0;
    virtual void            addTextOverlay(const Vector3& origin, int line_offset, float duration, const char *format, ...) = 0;
    virtual void            addScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char *text) = 0;
    virtual void            addSweptBoxOverlay(const Vector3& start, const Vector3& end, const Vector3& mins, const Vector3& max, const Vector3 & angles, int r, int g, int b, int a, float flDuration) = 0;
    virtual void            addGridOverlay(const Vector3& origin) = 0;
    virtual void            addCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	bool WorldToScreen( const Vector3& in, Vector3& out ) {
		using original_fn = int( __thiscall* )( IVDebugOverlay*, const Vector3&, Vector3& );
		int return_value = ( *( original_fn** )this )[ 13 ]( this, in, out );
		return static_cast< bool >( return_value != 1 );
	}

    virtual int             screenPosition(float flXPos, float flYPos, Vector3& screen) = 0;
    virtual OverlayText_t*  getFirst(void) = 0;
    virtual OverlayText_t*  getNext(OverlayText_t *current) = 0;
    virtual void            clearDeadOverlays(void) = 0;
    virtual void            clearAllOverlays() = 0;
    virtual void            addTextOverlayRGB(const Vector3& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char *format, ...) = 0;
    virtual void            addTextOverlayRGB(const Vector3& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) = 0;
    virtual void            addLineOverlayAlpha(const Vector3& origin, const Vector3& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
    virtual void            addBoxOverlay2(const Vector3& origin, const Vector3& mins, const Vector3& max, Vector3 const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration) = 0;
    virtual void            purgeTextOverlays() = 0;
    virtual void            drawPill(const Vector3& mins, const Vector3& max, float& diameter, int r, int g, int b, int a, float duration) = 0;
	void addCapsuleOverlay(Vector3& mins, Vector3& maxs, float pillradius, int r, int g, int b, int a, float duration)
	{
		using originalFn = VFUNC_SIGNATURE(void, Vector3&, Vector3&, float, int, int, int, int, float);

		((originalFn)g_InterfaceUtils->getVfuncTable(this)[23])(this, mins, maxs, pillradius, r, g, b, a, duration);
	}

private:
	inline void addTextOverlay(const Vector3& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) {} /* catch improper use of bad interface. Needed because '0' duration can be resolved by compiler to NULL format string (i.e., compiles but calls wrong function) */
};