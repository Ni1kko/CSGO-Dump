#pragma once
#include "vector.hpp"
#include <limits>

typedef float vec_t;

class matrix3x4_t
{
public:
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		flMatVal[0][0] = m00;	flMatVal[0][1] = m01; flMatVal[0][2] = m02; flMatVal[0][3] = m03;
		flMatVal[1][0] = m10;	flMatVal[1][1] = m11; flMatVal[1][2] = m12; flMatVal[1][3] = m13;
		flMatVal[2][0] = m20;	flMatVal[2][1] = m21; flMatVal[2][2] = m22; flMatVal[2][3] = m23;
	}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3 &vecOrigin)
	{
		flMatVal[0][0] = xAxis.x; flMatVal[0][1] = yAxis.x; flMatVal[0][2] = zAxis.x; flMatVal[0][3] = vecOrigin.x;
		flMatVal[1][0] = xAxis.y; flMatVal[1][1] = yAxis.y; flMatVal[1][2] = zAxis.y; flMatVal[1][3] = vecOrigin.y;
		flMatVal[2][0] = xAxis.z; flMatVal[2][1] = yAxis.z; flMatVal[2][2] = zAxis.z; flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3 &vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void SetOrigin(Vector3 const & p)
	{
		flMatVal[0][3] = p.x;
		flMatVal[1][3] = p.y;
		flMatVal[2][3] = p.z;
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				flMatVal[i][j] = std::numeric_limits<float>::infinity();;
			}
		}
	}

	float *operator[](int i) { return flMatVal[i]; }
	const float *operator[](int i) const { return flMatVal[i]; }
	float *Base() { return &flMatVal[0][0]; }
	const float *Base() const { return &flMatVal[0][0]; }

	float flMatVal[3][4];
};
class VMatrix
{
public:

	VMatrix();
	VMatrix(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	VMatrix(const Vector3& forward, const Vector3& left, const Vector3& up);

	// Construct from a 3x4 matrix
	VMatrix(const matrix3x4_t& matrix3x4);

	// Set the values in the matrix.
	void		Init(
		vec_t m00, vec_t m01, vec_t m02, vec_t m03,
		vec_t m10, vec_t m11, vec_t m12, vec_t m13,
		vec_t m20, vec_t m21, vec_t m22, vec_t m23,
		vec_t m30, vec_t m31, vec_t m32, vec_t m33
	);


	// Initialize from a 3x4
	void		Init(const matrix3x4_t& matrix3x4);

	// array access
	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float *Base()
	{
		return &m[0][0];
	}

	inline const float *Base() const
	{
		return &m[0][0];
	}

	void		SetLeft(const Vector3 &vLeft);
	void		SetUp(const Vector3 &vUp);
	void		SetForward(const Vector3 &vForward);

	void		GetBasisVector3s(Vector3 &vForward, Vector3 &vLeft, Vector3 &vUp) const;
	void		SetBasisVector3s(const Vector3 &vForward, const Vector3 &vLeft, const Vector3 &vUp);

	// Get/set the translation.
	Vector3 &	GetTranslation(Vector3 &vTrans) const;
	void		SetTranslation(const Vector3 &vTrans);

	void		PreTranslate(const Vector3 &vTrans);
	void		PostTranslate(const Vector3 &vTrans);

	matrix3x4_t& As3x4();
	const matrix3x4_t& As3x4() const;
	void		CopyFrom3x4(const matrix3x4_t &m3x4);
	void		Set3x4(matrix3x4_t& matrix3x4) const;

	bool		operator==(const VMatrix& src) const;
	bool		operator!=(const VMatrix& src) const { return !(*this == src); }

	// Access the basis Vector3s.
	Vector3		GetLeft() const;
	Vector3		GetUp() const;
	Vector3		GetForward() const;
	Vector3		GetTranslation() const;


	// Matrix->Vector3 operations.
public:
	// Multiply by a 3D Vector3 (same as operator*).
	void		V3Mul(const Vector3 &vIn, Vector3 &vOut) const;

	// Multiply by a 4D Vector3.
	//void		V4Mul( const Vector34D &vIn, Vector34D &vOut ) const;

	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vector3		ApplyRotation(const Vector3 &vVec) const;

	// Multiply by a Vector3 (divides by w, assumes input w is 1).
	Vector3		operator*(const Vector3 &vVec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vector3		VMul3x3(const Vector3 &vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vector3		VMul3x3Transpose(const Vector3 &vVec) const;

	// Multiply by the upper 3 rows.
	Vector3		VMul4x3(const Vector3 &vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vector3		VMul4x3Transpose(const Vector3 &vVec) const;


	// Matrix->plane operations.
	//public:
	// Transform the plane. The matrix can only contain translation and rotation.
	//void		TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

	// Just calls TransformPlane and returns the result.
	//VPlane		operator*(const VPlane &thePlane) const;

	// Matrix->matrix operations.
public:

	VMatrix & operator=(const VMatrix &mOther);

	// Multiply two matrices (out = this * vm).
	void		MatrixMul(const VMatrix &vm, VMatrix &out) const;

	// Add two matrices.
	const VMatrix& operator+=(const VMatrix &other);

	// Just calls MatrixMul and returns the result.	
	VMatrix		operator*(const VMatrix &mOther) const;

	// Add/Subtract two matrices.
	VMatrix		operator+(const VMatrix &other) const;
	VMatrix		operator-(const VMatrix &other) const;

	// Negation.
	VMatrix		operator-() const;

	// Return inverse matrix. Be careful because the results are undefined 
	// if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
	VMatrix		operator~() const;

	// Matrix operations.
public:
	// Set to identity.
	void		Identity();

	bool		IsIdentity() const;

	// Setup a matrix for origin and angles.
	void		SetupMatrixOrgAngles(const Vector3 &origin, const Vector3 &vAngles);

	// General inverse. This may fail so check the return!
	bool		InverseGeneral(VMatrix &vInverse) const;

	// Does a fast inverse, assuming the matrix only contains translation and rotation.
	void		InverseTR(VMatrix &mRet) const;

	// Usually used for debug checks. Returns true if the upper 3x3 contains
	// unit Vector3s and they are all orthogonal.
	bool		IsRotationMatrix() const;

	// This calls the other InverseTR and returns the result.
	VMatrix		InverseTR() const;

	// Get the scale of the matrix's basis Vector3s.
	Vector3		GetScale() const;

	// (Fast) multiply by a scaling matrix setup from vScale.
	VMatrix		Scale(const Vector3 &vScale);

	// Normalize the basis Vector3s.
	VMatrix		NormalizeBasisVector3s() const;

	// Transpose.
	VMatrix		Transpose() const;

	// Transpose upper-left 3x3.
	VMatrix		Transpose3x3() const;

public:
	// The matrix.
	vec_t		m[4][4];
};

inline void MatrixGetColumn(const matrix3x4_t &src, int nCol, Vector3& pColumn)
{
	pColumn.x = src[0][nCol];
	pColumn.y = src[1][nCol];
	pColumn.z = src[2][nCol];
}

inline void MatrixAngles(const matrix3x4_t& matrix, float *angles)
{
	float forward[3];
	float left[3];
	float up[3];

	//
	// Extract the basis vectors from the matrix. Since we only need the Z
	// component of the up vector, we don't get X and Y.
	//
	forward[0] = matrix[0][0];
	forward[1] = matrix[1][0];
	forward[2] = matrix[2][0];
	left[0] = matrix[0][1];
	left[1] = matrix[1][1];
	left[2] = matrix[2][1];
	up[2] = matrix[2][2];

	float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

	// enough here to get angles?
	if (xyDist > 0.001f)
	{
		// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		// (roll)	z = ATAN( left.z, up.z );
		angles[2] = RAD2DEG(atan2f(left[2], up[2]));
	}
	else	// forward is mostly Z, gimbal lock-
	{
		// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
		angles[1] = RAD2DEG(atan2f(-left[0], left[1]));

		// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
		angles[2] = 0;
	}
}

inline void MatrixPosition(const matrix3x4_t &matrix, Vector3 &position)
{
	MatrixGetColumn(matrix, 3, position);
}

inline void MatrixAngles(const matrix3x4_t &matrix, Vector3 &angles)
{
	MatrixAngles(matrix, &angles.x);
}

inline void MatrixAngles(const matrix3x4_t &matrix, Vector3 &angles, Vector3 &position)
{
	MatrixAngles(matrix, angles);
	MatrixPosition(matrix, position);
}