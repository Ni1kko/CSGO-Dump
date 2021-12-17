#pragma once

#include <sstream>

#ifdef NDEBUG
#define Assert( _exp ) ((void)0)
#else
#define Assert(x)
#endif

#define CHECK_VALID( _v ) 0
#define FastSqrt(x)	(sqrt)(x)
#define deg(a) a * 57.295779513082
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#define M_PI_2      (M_PI * 2.f)
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define M_PHI		1.61803398874989484820 // golden ratio

// NJS: Inlined to prevent floats from being autopromoted to doubles, as with the old system.
#ifndef RAD2DEG
#define RAD2DEG(x)  ((float)(x) * (float)(180.f / M_PI_F))
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#endif

#define ALIGN16 __declspec(align(16))
#define VALVE_RAND_MAX 0x7fff
#define VectorExpand(v) (v).x, (v).y, (v).z

class Vector3
{
public:
	float x, y, z;
	Vector3(void);
	Vector3(float X, float Y, float Z);
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	bool IsValid() const;
	float operator[](int i) const;
	float& operator[](int i);
	inline void Zero();
	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;
	inline Vector3&	operator+=(const Vector3 &v);
	inline Vector3&	operator-=(const Vector3 &v);
	inline Vector3&	operator*=(const Vector3 &v);
	inline Vector3&	operator*=(float s);
	inline Vector3&	operator/=(const Vector3 &v);
	inline Vector3&	operator/=(float s);
	inline Vector3&	operator+=(float fl);
	inline Vector3&	operator-=(float fl);
	inline float	Length() const;
	inline float LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (x*x + y * y + z * z);
	}
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}
	Vector3  Clamp();
	Vector3	Normalize();
	inline Vector3 Normalized( ) const;
	float	NormalizeInPlace();
	inline float	DistTo(const Vector3 &vOther) const;
	inline float	DistToSqr(const Vector3 &vOther) const;
	float	Dot(const Vector3& vOther) const;
	float	Length2D(void) const;
	float	Length2DSqr(void) const;
	void	MulAdd(const Vector3& a, const Vector3& b, float scalar);
	Vector3	Cross(const Vector3 &vOther) const;
	Vector3& operator=(const Vector3 &vOther);
	Vector3	operator-(void) const;
	Vector3	operator+(const Vector3& v) const;
	Vector3	operator-(const Vector3& v) const;
	Vector3	operator*(const Vector3& v) const;
	Vector3	operator/(const Vector3& v) const;
	Vector3	operator*(float fl) const;
	Vector3	operator/(float fl) const;
	// Base address...
	float* Base();
	float const* Base() const;

	auto clear() -> void
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	inline Vector3 Angle(Vector3* up = 0)
	{
		if (!x && !y)
			return Vector3(0, 0, 0);

		float roll = 0;

		if (up)
		{
			Vector3 left = (*up).Cross(*this);
			roll = deg(atan2f(left.z, (left.y * x) - (left.x * y)));
		}
		return Vector3(deg(atan2f(-z, sqrtf(x*x + y * y))), deg(atan2f(y, x)), roll);
	}
};

//===============================================
inline void Vector3::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}
//===============================================
inline Vector3::Vector3(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}
//===============================================
inline Vector3::Vector3(void) {}
//===============================================
inline void Vector3::Zero()
{
	x = y = z = 0.0f;
}
//===============================================
inline void Vector3Clear(Vector3& a)
{
	a.x = a.y = a.z = 0.0f;
}
//===============================================
inline Vector3& Vector3::operator=(const Vector3 &vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}
//===============================================
inline float& Vector3::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
//===============================================
inline float Vector3::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
//===============================================
inline bool Vector3::operator==(const Vector3& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}
//===============================================
inline bool Vector3::operator!=(const Vector3& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}
//===============================================
inline void Vector3Copy(const Vector3& src, Vector3& dst)
{
	CHECK_VALID(src);
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
//===============================================
inline  Vector3& Vector3::operator+=(const Vector3& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator-=(const Vector3& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator*=(const Vector3& v)
{
	CHECK_VALID(v);
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline Vector3&	Vector3::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline Vector3&	Vector3::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline Vector3 Vector3::Clamp()
{
	CHECK_VALID(*this);

	if (this->x < -89.0f)
		this->x = -89.0f;

	if (this->x > 89.0f)
		this->x = 89.0f;

	while (this->y < -180.0f)
		this->y += 360.0f;

	while (this->y > 180.0f)
		this->y -= 360.0f;

	this->z = 0.0f;

	return *this;
}
//===============================================
inline  Vector3& Vector3::operator/=(const Vector3& v)
{
	CHECK_VALID(v);
	Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline float Vector3::Length(void) const
{
	CHECK_VALID(*this);

	float root = 0.0f;

	float sqsr = x * x + y * y + z * z;

	root = sqrt(sqsr);

	return root;
}
//===============================================
inline float Vector3::Length2D(void) const
{
	CHECK_VALID(*this);

	float root = 0.0f;

	float sqst = x * x + y * y;

	root = sqrt(sqst);

	return root;
}
//===============================================
inline float Vector3::Length2DSqr(void) const
{
	return (x*x + y * y);
}
//===============================================
inline Vector3 CrossProduct(const Vector3& a, const Vector3& b)
{
	return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
//===============================================
inline Vector3 Vector3::Cross(const Vector3& vOther) const
{
	return CrossProduct(*this, vOther);
}
//===============================================
float Vector3::DistTo(const Vector3 &vOther) const
{
	Vector3 delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.Length();
}
float Vector3::DistToSqr(const Vector3 &vOther) const
{
	Vector3 delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.LengthSqr();
}
//===============================================
inline Vector3 Vector3::Normalize()
{
	Vector3 Vector3;
	float length = this->Length();

	if (length != 0)
	{
		Vector3.x = x / length;
		Vector3.y = y / length;
		Vector3.z = z / length;
	}
	else
	{
		Vector3.x = Vector3.y = 0.0f; Vector3.z = 1.0f;
	}

	return Vector3;
}
inline Vector3 Vector3::Normalized( ) const {
	Vector3 vec = *this;
	vec.Normalize( );
	return vec;
}
//===============================================
// changed that to fit awall, paste from xaE
inline float Vector3::NormalizeInPlace()
{
	float radius = FastSqrt(x * x + y * y + z * z);

	// FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero.
	float iradius = 1.f / (radius + FLT_EPSILON);

	x *= iradius;
	y *= iradius;
	z *= iradius;

	return radius;
}
//===============================================
inline void Vector3::MulAdd(const Vector3& a, const Vector3& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}
//===============================================
inline float Vector3Normalize(Vector3& v)
{
	Assert(v.IsValid());
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		// FIXME:
		// Just copying the existing implemenation; shouldn't res.z == 0?
		v.x = v.y = 0.0f; v.z = 1.0f;
	}
	return l;
}
//===============================================
inline float Vector3Normalize(float * v)
{
	return Vector3Normalize(*(reinterpret_cast<Vector3 *>(v)));
}
//===============================================
inline Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

//===============================================
inline Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}
//===============================================
inline Vector3 Vector3::operator*(float fl) const
{
	Vector3 res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}
//===============================================
inline Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}
//===============================================
inline Vector3 Vector3::operator/(float fl) const
{
	Vector3 res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}
//===============================================
inline Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
inline float Vector3::Dot(const Vector3& vOther) const
{
	const Vector3& a = *this;

	return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
}

typedef float vec_t;

inline vec_t DotProduct(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------

inline float VectorLength(const Vector3& v)
{
	CHECK_VALID(v);
	return (float)FastSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

//Vector3 SUBTRAC
inline void Vector3Subtract(const Vector3& a, const Vector3& b, Vector3& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

//Vector3ADD
inline void Vector3Add(const Vector3& a, const Vector3& b, Vector3& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* Vector3::Base()
{
	return (float*)this;
}

inline float const* Vector3::Base() const
{
	return (float const*)this;
}

inline void Vector3MAInline(const float* start, float scale, const float* direction, float* dest)
{
	dest[0] = start[0] + direction[0] * scale;
	dest[1] = start[1] + direction[1] * scale;
	dest[2] = start[2] + direction[2] * scale;
}

inline void Vector3MAInline(const Vector3& start, float scale, const Vector3& direction, Vector3& dest)
{
	dest.x = start.x + direction.x*scale;
	dest.y = start.y + direction.y*scale;
	dest.z = start.z + direction.z*scale;
}

inline void Vector3MA(const Vector3& start, float scale, const Vector3& direction, Vector3& dest)
{
	Vector3MAInline(start, scale, direction, dest);
}

inline void Vector3MA(const float * start, float scale, const float *direction, float *dest)
{
	Vector3MAInline(start, scale, direction, dest);
}

class ALIGN16 VectorAligned : public Vector3
{
public:
    inline VectorAligned(void) {};
    inline VectorAligned(float X, float Y, float Z)
    {
        Init(X, Y, Z);
    }

#ifdef VECTOR_NO_SLOW_OPERATIONS

private:
    // No copy constructors allowed if we're in optimal mode
    VectorAligned(const VectorAligned& vOther);
    VectorAligned(const Vector &vOther);

#else
public:
    explicit VectorAligned(const Vector3 &vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
    }

    VectorAligned& operator=(const Vector3 &vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
        return *this;
    }

#endif
    float w;	// this space is used anyway
};

inline unsigned long& FloatBits(float& f)
{
    return *reinterpret_cast<unsigned long*>(&f);
}

inline bool IsFinite(float f)
{
    return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

class Vector2
{
public:
	// Members
	float x, y;

	// Construction/destruction
	Vector2(void);
	Vector2(float X, float Y);
	Vector2( int X, float Y );
	Vector2(int X, int Y);
	Vector2( float X, int Y );
	Vector2(const float *pFloat);

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	float  operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float*       Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);

	// equality
	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	// arithmetic operations
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator*=(float           s);
	Vector2& operator/=(const Vector2& v);
	Vector2& operator/=(float           s);

	inline Vector2 operator-( int v ) const {
		return Vector2( x - v, y - v );
	}

	// negate the Vector2 components
	void Negate();

	// Get the Vector2's magnitude.
	float Length() const;

	// Get the Vector2's magnitude squared.
	float LengthSqr(void) const;

	// return true if this vector is (0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance);
	}

	float Normalize();

	// Normalize in place and return the old length.
	float NormalizeInPlace();

	// Compare length.
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float    val) const;

	// Get the distance from this Vector2 to the other one.
	float DistTo(const Vector2& vOther) const;

	// Get the distance from this Vector2 to the other one squared.
	float DistToSqr(const Vector2& vOther) const;

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector2 equation (because it's done per-component
	// rather than per-Vector2).
	void MulAdd(const Vector2& a, const Vector2& b, float scalar);

	// Dot product.
	float Dot(const Vector2& vOther) const;

	// assignment
	Vector2& operator=(const Vector2& vOther);

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// copy constructors
	Vector2(const Vector2 &vOther);

	// arithmetic operations
	Vector2 operator-(void) const;

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator/(const Vector2& v) const;
	Vector2 operator+(const int       i1) const;
	Vector2 operator+(const float     fl) const;
	Vector2 operator*(const float     fl) const;
	Vector2 operator/(const float     fl) const;

	// Cross product between two vectors.
	Vector2 Cross(const Vector2& vOther) const;

	// Returns a Vector2 with the min or max in X, Y, and Z.
	Vector2 Min(const Vector2& vOther) const;
	Vector2 Max(const Vector2& vOther) const;

#else

private:
	// No copy constructors allowed if we're in optimal mode
	Vector2(const Vector2& vOther);
#endif
};

//-----------------------------------------------------------------------------

const Vector2 vec2_origin(0, 0);
//const Vector2 vec2_invalid(3.40282347E+38F, 3.40282347E+38F);

//-----------------------------------------------------------------------------
// Vector2 related operations
//-----------------------------------------------------------------------------

// Vector2 clear
void Vector2Clear(Vector2& a);

// Copy
void Vector2Copy(const Vector2& src, Vector2& dst);

// Vector2 arithmetic
void Vector2Add(const Vector2&      a, const Vector2& b, Vector2&       result);
void Vector2Subtract(const Vector2& a, const Vector2& b, Vector2&       result);
void Vector2Multiply(const Vector2& a, float           b, Vector2&       result);
void Vector2Multiply(const Vector2& a, const Vector2& b, Vector2&       result);
void Vector2Divide(const Vector2&   a, float           b, Vector2&       result);
void Vector2Divide(const Vector2&   a, const Vector2& b, Vector2&       result);
void Vector2MA(const Vector2&       start, float       s, const Vector2& dir, Vector2& result);

// Store the min or max of each of x, y, and z into the result.
void Vector2Min(const Vector2& a, const Vector2& b, Vector2& result);
void Vector2Max(const Vector2& a, const Vector2& b, Vector2& result);

#define Vector2Expand( v ) (v).x, (v).y

// Normalization
float Vector2Normalize(Vector2& v);

// Length
float Vector2Length(const Vector2& v);

// Dot Product
float DotProduct2D(const Vector2& a, const Vector2& b);

// Linearly interpolate between two vectors
void Vector2Lerp(const Vector2& src1, const Vector2& src2, float t, Vector2& dest);


//-----------------------------------------------------------------------------
//
// Inlined Vector2 methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline Vector2::Vector2(void)
{
#ifdef _DEBUG
    // Initialize to NAN to catch errors
    //x = y = float_NAN;
#endif
}

inline Vector2::Vector2(float X, float Y)
{
    x = X; y = Y;
    Assert(IsValid());
}

inline Vector2::Vector2( int X, float Y )
{
	x = X; y = Y;
	Assert( IsValid( ) );
}

inline Vector2::Vector2( int X, int Y )
{
	x = X; y = Y;
	Assert( IsValid( ) );
}

inline Vector2::Vector2( float X, int Y )
{
	x = X; y = Y;
	Assert( IsValid( ) );
}

inline Vector2::Vector2(const float *pFloat)
{
    Assert(pFloat);
    x = pFloat[0]; y = pFloat[1];
    Assert(IsValid());
}


//-----------------------------------------------------------------------------
// copy constructor
//-----------------------------------------------------------------------------

inline Vector2::Vector2(const Vector2 &vOther)
{
    Assert(vOther.IsValid());
    x = vOther.x; y = vOther.y;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector2::Init(float ix, float iy)
{
    x = ix; y = iy;
    Assert(IsValid());
}

inline void Vector2::Random(float minVal, float maxVal)
{
    x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
    y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline void Vector2Clear(Vector2& a)
{
    a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline Vector2& Vector2::operator=(const Vector2 &vOther)
{
    Assert(vOther.IsValid());
    x = vOther.x; y = vOther.y;
    return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

inline float& Vector2::operator[](int i)
{
    Assert((i >= 0) && (i < 2));
    return ((float*)this)[i];
}

inline float Vector2::operator[](int i) const
{
    Assert((i >= 0) && (i < 2));
    return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

inline float* Vector2::Base()
{
    return (float*)this;
}

inline float const* Vector2::Base() const
{
    return (float const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool Vector2::IsValid() const
{
    return IsFinite(x) && IsFinite(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector2::operator==(const Vector2& src) const
{
    Assert(src.IsValid() && IsValid());
    return (src.x == x) && (src.y == y);
}

inline bool Vector2::operator!=(const Vector2& src) const
{
    Assert(src.IsValid() && IsValid());
    return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

inline void Vector2Copy(const Vector2& src, Vector2& dst)
{
    Assert(src.IsValid());
    dst.x = src.x;
    dst.y = src.y;
}

inline void	Vector2::CopyToArray(float* rgfl) const
{
    Assert(IsValid());
    Assert(rgfl);
    rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------

inline void Vector2::Negate()
{
    Assert(IsValid());
    x = -x; y = -y;
}

inline Vector2& Vector2::operator+=(const Vector2& v)
{
    Assert(IsValid() && v.IsValid());
    x += v.x; y += v.y;
    return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& v)
{
    Assert(IsValid() && v.IsValid());
    x -= v.x; y -= v.y;
    return *this;
}

inline Vector2& Vector2::operator*=(float fl)
{
    x *= fl;
    y *= fl;
    Assert(IsValid());
    return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
    Assert(IsValid());
    return *this;
}

inline Vector2& Vector2::operator/=(float fl)
{
    Assert(fl != 0.0f);
    float oofl = 1.0f / fl;
    x *= oofl;
    y *= oofl;
    Assert(IsValid());
    return *this;
}

inline Vector2& Vector2::operator/=(const Vector2& v)
{
    Assert(v.x != 0.0f && v.y != 0.0f);
    x /= v.x;
    y /= v.y;
    Assert(IsValid());
    return *this;
}

inline void Vector2Add(const Vector2& a, const Vector2& b, Vector2& c)
{
    Assert(a.IsValid() && b.IsValid());
    c.x = a.x + b.x;
    c.y = a.y + b.y;
}

inline void Vector2Add(const Vector2& a, const int b, Vector2& c)
{
    Assert(a.IsValid());
    c.x = a.x + b;
    c.y = a.y + b;
}

inline void Vector2Add(const Vector2& a, const float b, Vector2& c)
{
    Assert(a.IsValid());
    c.x = a.x + b;
    c.y = a.y + b;
}

inline void Vector2Subtract(const Vector2& a, const Vector2& b, Vector2& c)
{
    Assert(a.IsValid() && b.IsValid());
    c.x = a.x - b.x;
    c.y = a.y - b.y;
}

inline void Vector2Multiply(const Vector2& a, const float b, Vector2& c)
{
    Assert(a.IsValid() && IsFinite(b));
    c.x = a.x * b;
    c.y = a.y * b;
}

inline void Vector2Multiply(const Vector2& a, const Vector2& b, Vector2& c)
{
    Assert(a.IsValid() && b.IsValid());
    c.x = a.x * b.x;
    c.y = a.y * b.y;
}


inline void Vector2Divide(const Vector2& a, const float b, Vector2& c)
{
    Assert(a.IsValid());
    Assert(b != 0.0f);
    float oob = 1.0f / b;
    c.x = a.x * oob;
    c.y = a.y * oob;
}

inline void Vector2Divide(const Vector2& a, const Vector2& b, Vector2& c)
{
    Assert(a.IsValid());
    Assert((b.x != 0.0f) && (b.y != 0.0f));
    c.x = a.x / b.x;
    c.y = a.y / b.y;
}

inline void Vector2MA(const Vector2& start, float s, const Vector2& dir, Vector2& result)
{
    Assert(start.IsValid() && IsFinite(s) && dir.IsValid());
    result.x = start.x + s*dir.x;
    result.y = start.y + s*dir.y;
}

// FIXME: Remove
// For backwards compatability
inline void	Vector2::MulAdd(const Vector2& a, const Vector2& b, float scalar)
{
    x = a.x + b.x * scalar;
    y = a.y + b.y * scalar;
}

inline void Vector2Lerp(const Vector2& src1, const Vector2& src2, float t, Vector2& dest)
{
    dest[0] = src1[0] + (src2[0] - src1[0]) * t;
    dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
inline float DotProduct2D(const Vector2& a, const Vector2& b)
{
    Assert(a.IsValid() && b.IsValid());
    return(a.x*b.x + a.y*b.y);
}

// for backwards compatability
inline float Vector2::Dot(const Vector2& vOther) const
{
    return DotProduct2D(*this, vOther);
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float Vector2Length(const Vector2& v)
{
    Assert(v.IsValid());
    return (float)FastSqrt(v.x*v.x + v.y*v.y);
}

inline float Vector2::LengthSqr(void) const
{
    Assert(IsValid());
    return (x*x + y*y);
}

inline float Vector2::NormalizeInPlace()
{
    return Vector2Normalize(*this);
}

inline bool Vector2::IsLengthGreaterThan(float val) const
{
    return LengthSqr() > val*val;
}

inline bool Vector2::IsLengthLessThan(float val) const
{
    return LengthSqr() < val*val;
}

inline float Vector2::Length(void) const
{
    return Vector2Length(*this);
}


inline void Vector2Min(const Vector2 &a, const Vector2 &b, Vector2 &result)
{
    result.x = (a.x < b.x) ? a.x : b.x;
    result.y = (a.y < b.y) ? a.y : b.y;
}


inline void Vector2Max(const Vector2 &a, const Vector2 &b, Vector2 &result)
{
    result.x = (a.x > b.x) ? a.x : b.x;
    result.y = (a.y > b.y) ? a.y : b.y;
}


//-----------------------------------------------------------------------------
// Normalization
//-----------------------------------------------------------------------------
inline float Vector2Normalize(Vector2& v)
{
    Assert(v.IsValid());
    float l = v.Length();
    if (l != 0.0f)
    {
        v /= l;
    }
    else
    {
        v.x = v.y = 0.0f;
    }
    return l;
}


//-----------------------------------------------------------------------------
// Get the distance from this Vector2 to the other one
//-----------------------------------------------------------------------------
inline float Vector2::DistTo(const Vector2 &vOther) const
{
    Vector2 delta;
    Vector2Subtract(*this, vOther, delta);
    return delta.Length();
}

inline float Vector2::DistToSqr(const Vector2 &vOther) const
{
    Vector2 delta;
    Vector2Subtract(*this, vOther, delta);
    return delta.LengthSqr();
}


//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
inline void ComputeClosestPoint2D(const Vector2& vecStart, float flMaxDist, const Vector2& vecTarget, Vector2 *pResult)
{
    Vector2 vecDelta;
    Vector2Subtract(vecTarget, vecStart, vecDelta);
    float flDistSqr = vecDelta.LengthSqr();
    if (flDistSqr <= flMaxDist * flMaxDist)
    {
        *pResult = vecTarget;
    }
    else
    {
        vecDelta /= FastSqrt(flDistSqr);
        Vector2MA(vecStart, flMaxDist, vecDelta, *pResult);
    }
}



//-----------------------------------------------------------------------------
//
// Slow methods
//
//-----------------------------------------------------------------------------

#ifndef VECTOR_NO_SLOW_OPERATIONS
#endif
//-----------------------------------------------------------------------------
// Returns a Vector2 with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

inline Vector2 Vector2::Min(const Vector2 &vOther) const
{
    return Vector2(x < vOther.x ? x : vOther.x,
        y < vOther.y ? y : vOther.y);
}

inline Vector2 Vector2::Max(const Vector2 &vOther) const
{
    return Vector2(x > vOther.x ? x : vOther.x,
        y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

inline Vector2 Vector2::operator-(void) const
{
    return Vector2(-x, -y);
}

inline Vector2 Vector2::operator+(const Vector2& v) const
{
    Vector2 res;
    Vector2Add(*this, v, res);
    return res;
}

inline Vector2 Vector2::operator-(const Vector2& v) const
{
    Vector2 res;
    Vector2Subtract(*this, v, res);
    return res;
}

inline Vector2 Vector2::operator+(const int i1) const
{
    Vector2 res;
    Vector2Add(*this, i1, res);
    return res;
}

inline Vector2 Vector2::operator+(const float fl) const
{
    Vector2 res;
    Vector2Add(*this, fl, res);
    return res;
}

inline Vector2 Vector2::operator*(const float fl) const
{
    Vector2 res;
    Vector2Multiply(*this, fl, res);
    return res;
}

inline Vector2 Vector2::operator*(const Vector2& v) const
{
    Vector2 res;
    Vector2Multiply(*this, v, res);
    return res;
}

inline Vector2 Vector2::operator/(const float fl) const
{
    Vector2 res;
    Vector2Divide(*this, fl, res);
    return res;
}

inline Vector2 Vector2::operator/(const Vector2& v) const
{
    Vector2 res;
    Vector2Divide(*this, v, res);
    return res;
}

inline Vector2 operator*(const float fl, const Vector2& v)
{
    return v * fl;
}



// 3D Vector4
class Vector4
{
public:
	// Members
	vec_t x, y, z, w;

	// Construction/destruction:
	Vector4(void);
	Vector4(vec_t X, vec_t Y, vec_t Z, vec_t W);
	Vector4(vec_t* clr);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	vec_t* Base();
	vec_t const* Base() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector4& v) const;
	bool operator!=(const Vector4& v) const;

	// arithmetic operations
	Vector4& operator+=(const Vector4& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	Vector4& operator-=(const Vector4& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	Vector4& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	Vector4& operator*=(const Vector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	Vector4& operator/=(const Vector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	// this ought to be an opcode.
	Vector4& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector4& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	Vector4& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// get the vector's magnitude.
	vec_t Length() const;

	// get the vector's magnitude squared.
	vec_t LengthSqr(void) const
	{
		return (x*x + y * y + z * z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance &&
			w > -tolerance && w < tolerance);
	}

	vec_t NormalizeInPlace();
	Vector4 Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector4 const &boxmin, Vector4 const &boxmax);

	// get the distance from this vector to the other one.
	vec_t DistTo(const Vector4 &vOther) const;

	// get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	vec_t DistToSqr(const Vector4 &vOther) const
	{
		Vector4 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;
		delta.w = w - vOther.w;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const Vector4& a, const Vector4& b, float scalar);

	// Dot product.
	vec_t Dot(const Vector4& vOther) const;

	// assignment
	Vector4& operator=(const Vector4 &vOther);

	// 2d
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	/// get the component of this vector parallel to some other given vector
	Vector4  ProjectOnto(const Vector4& onto);

	// copy constructors
	// Vector4(const Vector4 &vOther);

	// arithmetic operations
	Vector4 operator-(void) const;

	Vector4 operator+(const Vector4& v) const;
	Vector4 operator-(const Vector4& v) const;
	Vector4 operator*(const Vector4& v) const;
	Vector4 operator/(const Vector4& v) const;
	Vector4 operator*(float fl) const;
	Vector4 operator/(float fl) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector4 Min(const Vector4 &vOther) const;
	Vector4 Max(const Vector4 &vOther) const;
};

class Quaternion {
public:
	/**
	 * Konstruktor der Klasse.
	 */
	inline Quaternion( float _w = 1.0f, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) :
		w( _w ),
		x( _x ),
		y( _y ),
		z( _z ) { }

	inline bool operator==( const Quaternion &v ) const {
		return ( w == v.w ) && ( x == v.x ) && ( y == v.y ) && ( z == v.z );
	}

	inline bool operator!=( const Quaternion &v ) const {
		return !( *this == v );
	}

	inline Quaternion operator-( ) const {
		return Quaternion( -w, -x, -y, -z );
	}

	inline Quaternion operator*( float v ) const {
		return Quaternion( w * v, x * v, y * v, z * v );
	}

	inline friend Quaternion operator*( float v, const Quaternion &q ) {
		return Quaternion( v * q.w, v * q.x, v * q.y, v * q.z );
	}

	inline float Dot( const Quaternion &v ) const {
		return w * v.w + x * v.x + y * v.y + z * v.z;
	}

	inline Quaternion operator+( const Quaternion &v ) const {
		return Quaternion( w + v.w, x + v.x, y + v.y, z + v.z );
	}

	inline Quaternion operator*( const Quaternion &v ) const {
		return Quaternion(
			w * v.w - x * v.x - y * v.y - z * v.z,
			w * v.x + x * v.w + y * v.z - z * v.y,
			w * v.y + y * v.w + z * v.x - x * v.z,
			w * v.z + z * v.w + x * v.y - y * v.x
		);
	}

	/**
	 * Berechnet die Länge des Quaternion.
	 *
	 * \return Länge
	 */
	inline float Length( ) const {
		return std::sqrt( ( w * w ) + ( x * x ) + ( y * y ) + ( z * z ) );
	}

	/**
	 * Normalisiert das Quaternion.
	 */
	inline float Normalise( ) {
		const float len = Length( );
		const float factor = 1.0f / len;
		*this = *this * factor;

		return len;
	}

	static Quaternion EulerAnglesRadians( const float x, const float y, const float z ) {
		// the order of rotation:
		// 1) around Z
		// 2) around Y
		// 3) around X
		// even though it's passed in reverse...

		const float sin_z_2 = sinf( 0.5f * z );
		const float sin_y_2 = sinf( 0.5f * y );
		const float sin_x_2 = sinf( 0.5f * x );

		const float cos_z_2 = cosf( 0.5f * z );
		const float cos_y_2 = cosf( 0.5f * y );
		const float cos_x_2 = cosf( 0.5f * x );

		return Quaternion(
			cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2,
			cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2 * sin_y_2 * cos_x_2,
			cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2,
			sin_z_2 * cos_y_2 * cos_x_2 - cos_z_2 * sin_y_2 * sin_x_2
		);
	}

	//---------------------------------------------------------------------------
	static Quaternion EulerAnglesDegrees( const float x, const float y, const float z ) {
		static const float d2r = ( 4.0f * std::atan2( 1.0f, 1.0f ) ) / 180.0f;

		return EulerAnglesRadians( x * d2r, y * d2r, z * d2r );
	}

	//---------------------------------------------------------------------------

	float w;
	float x;
	float y;
	float z;
};