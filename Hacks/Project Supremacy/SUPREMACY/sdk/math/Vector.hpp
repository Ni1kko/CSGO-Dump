#pragma once

#include <sstream>

#define CHECK_VALID(_v) 0
#define Assert(_exp) ((void)0)

class Vector {
public:

	__inline Vector(void) {
		Invalidate();
	}

	Vector(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	Vector(const float* clr) {
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) {
		x = ix; y = iy; z = iz;
	}

	__inline void Mul(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	__inline Vector Clamp()
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

	__inline void MulAdd(const Vector& a, const Vector& b, float scalar) {
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	__inline bool IsValid() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	void __inline Invalidate() {
		x = y = z = std::numeric_limits<float>::infinity();
	}

	float &operator[](int i) {
		return ((float*)this)[i];
	}

	float operator[](int i) const {
		return ((float*)this)[i];
	}

	bool __inline IsZero() {
		return (!x && !y && !z);
	}

	void __inline Zero() {
		x = y = z = 0.0f;
	}

	bool operator==(const Vector &src) const {
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool operator!=(const Vector &src) const {
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	Vector &operator+=(const Vector &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector &operator-=(const Vector &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Vector &operator*=(float fl) {
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	Vector &operator*=(const Vector &v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector &operator/=(const Vector &v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vector &operator+=(float fl) {
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	Vector &operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	Vector &operator-=(float fl) {
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void NormalizeInPlace() {
		Vector& v = *this;

		float iradius = 1.f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;
	}

	Vector Normalized() const {
		Vector res = *this;
		float l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	float Normalize() const {
		Vector res = *this;
		float l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return l;
	}

	auto yaw_normalize()
	{
		auto x_rev = this->x / 360.f;
		if (this->x > 180.f || this->x < -180.f)
		{
			x_rev = abs(x_rev);
			x_rev = round(x_rev);

			if (this->x < 0.f)
				this->x = (this->x + 360.f * x_rev);

			else
				this->x = (this->x - 360.f * x_rev);
		}

		auto y_rev = this->y / 360.f;
		if (this->y > 180.f || this->y < -180.f)
		{
			y_rev = abs(y_rev);
			y_rev = round(y_rev);

			if (this->y < 0.f)
				this->y = (this->y + 360.f * y_rev);

			else
				this->y = (this->y - 360.f * y_rev);
		}

		auto z_rev = this->z / 360.f;
		if (this->z > 180.f || this->z < -180.f)
		{
			z_rev = abs(z_rev);
			z_rev = round(z_rev);

			if (this->z < 0.f)
				this->z = (this->z + 360.f * z_rev);

			else
				this->z = (this->z - 360.f * z_rev);
		}
	}

	float DistTo(const Vector &vOther) const {
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length();
	}

	float DistToSqr(const Vector &vOther) const {
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	float Dot(const Vector &vOther) const {
		return (x*vOther.x + y * vOther.y + z * vOther.z);
	}

	void VectorCrossProduct(const Vector &a, const Vector &b, Vector &result) {
		result.x = a.y*b.z - a.z*b.y;
		result.y = a.z*b.x - a.x*b.z;
		result.z = a.x*b.y - a.y*b.x;
	}

	Vector Cross(const Vector &vOther) {
		Vector res;
		VectorCrossProduct(*this, vOther, res);
		return res;
	}

	Vector Direction() {
		return Vector(cos(y * 3.141592654f / 180.0f) * cos(x * 3.141592654f / 180.0f), sin(y * 3.141592654f / 180.0f) * cos(x * 3.141592654f / 180.0f), sin(-x * 3.141592654f / 180.0f)).Normalized();
	}

	float Length() const {
		return sqrt(x*x + y * y + z * z);
	}

	float LengthSqr(void) const {
		return (x*x + y * y + z * z);
	}

	float Length2D() const {
		return sqrt(x*x + y * y);
	}

	Vector &operator=(const Vector &vOther) {
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	Vector Vector::operator-(void) const {
		return Vector(-x, -y, -z);
	}

	Vector Vector::operator+(const Vector &v) const {
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector Vector::operator+(float fl) const {
		return Vector(x + fl, y + fl, z + fl);
	}

	Vector Vector::operator-(const Vector &v) const {
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector Vector::operator-(float fl) const {
		return Vector(x - fl, y - fl, z - fl);
	}

	Vector Vector::operator*(float fl) const {
		return Vector(x * fl, y * fl, z * fl);
	}

	Vector Vector::operator*(const Vector &v) const {
		return Vector(x * v.x, y * v.y, z * v.z);
	}

	Vector Vector::operator/(float fl) const {
		return Vector(x / fl, y / fl, z / fl);
	}

	Vector Vector::operator/(const Vector &v) const {
		return Vector(x / v.x, y / v.y, z / v.z);
	}

	float x, y, z;
};

typedef Vector vec3_t;

inline Vector operator*(float lhs, const Vector &rhs) {
	return rhs * lhs;
}

inline Vector operator/(float lhs, const Vector &rhs) {
	return rhs / lhs;
}

class __declspec(align(16)) VectorAligned : public Vector {
public:

	inline VectorAligned(void) { };
	inline VectorAligned(float X, float Y, float Z) {
		Init(X, Y, Z);
	}

public:

	explicit VectorAligned(const Vector &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned &operator=(const Vector &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned &operator=(const VectorAligned &vOther) {
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};

typedef unsigned __int32		uint32;

inline uint32 const FloatBits(const float &f) {
	union Convertor_t {
		float f;
		uint32 ul;
	}tmp;
	tmp.f = f;
	return tmp.ul;
}

inline bool IsFinite(const float &f) {
#if _X360
	return f == f && fabs(f) <= FLT_MAX;
#else
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
#endif
}

inline void VectorMultiply(const Vector &a, float b, Vector &c) {
	CHECK_VALID(a);
	Assert(IsFinite(b));
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

inline void VectorMA(const Vector &start, float scale, const Vector &direction, Vector &dest) {
	CHECK_VALID(start);
	CHECK_VALID(direction);

	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}

inline void VectorAdd(const Vector &a, const Vector &b, Vector &c) {
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void VectorSubtract(const Vector &a, const Vector &b, Vector &c) {
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}