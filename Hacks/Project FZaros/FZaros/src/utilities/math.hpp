#pragma once
#include "includes.h"
#include<random>
#define PI 3.14159265358979323846f

class CMath
{
public:

	void vectorAngles(const Vector3 &vecForward, Vector3 &vecAngles);
	void vectorAngles(const Vector3& forward, Vector3& up, Vector3& angles);
	void rotateMovement(float yaw, CUserCmd * pCmd);
	float distanceToRay(const Vector3 & pos, const Vector3 & ray_start, const Vector3 & ray_end);
	void vectorTransform(const Vector3 in1, const matrix3x4_t in2, Vector3 &out);
	Vector3 calcAngle(const Vector3& vecSource, const Vector3& vecDestination);
	vec_t vectorNormalize(Vector3& v);
	void angleVectors(const Vector3 &angles, Vector3 *forward);
	float normalieYawInPlace(float f);
	void normalizeAngles(Vector3& angles);
	float normalizeYaw(float yaw);
	void angleVectors(const Vector3& angles, Vector3* forward, Vector3* right, Vector3* up);
	void AngToVec( const Vector3 & angles, Vector3 & forward );
	void randomSeed(int seed);
	float randomFloat(float min, float max);
	bool clamp(Vector3 &angles);
	void clampAngles(Vector3 &angles);
	float GRD_TO_BOG(float GRD);
	float Distance(Vector2 point1, Vector2 point2);

	float normalizeAngle( float flAng );

	float NormalizeFloat( float angle ) const;

	void angleMatrix( const Vector3 angles, matrix3x4_t & matrix );

	void matrixSetColumn( const Vector3 & in, int column, matrix3x4_t & out );

	void angleMatrix( const Vector3 & angles, const Vector3 & position, matrix3x4_t & matrix_out );

	void matrixCopy( const matrix3x4_t & source, matrix3x4_t & target );

	void matrixMultiply( matrix3x4_t & in1, const matrix3x4_t & in2 );

	void vectorRotate( const float * in1, const matrix3x4_t & in2, float * out );

	void vectorRotate( const Vector3 & in1, const matrix3x4_t & in2, Vector3 & out );

	void vectorRotate( const Vector3 & in1, const Vector3 & in2, Vector3 & out );

	// returns a random floating point value in range.
	template< typename t >
	static t randomFloatRange( const t &min, const t &max ) {
		static_assert( std::is_floating_point< t >::value, "invalid type for: randomFloatRange." );

		std::random_device rd;
		std::mt19937 e2( rd( ) );
		std::uniform_real_distribution< t > distribution( min, max );

		return distribution( e2 );
	}

	float GetFov(const Vector3& viewAngle, const Vector3& aimAngle);

}; extern CMath g_Math;