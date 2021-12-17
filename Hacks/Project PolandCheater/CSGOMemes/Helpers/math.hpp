#pragma once

#include "../ValveSDK/interfaces.hpp"

#include <DirectXMath.h>

#define RAD2DEG(x) ((x * 180.0f) / DirectX::XM_PI)
#define DEG2RAD(x) ((x * DirectX::XM_PI) / 180.0f)
#define M_RADPI 57.295779513082f
static const constexpr auto PIRAD = 0.01745329251f;
inline float RandomFloat(float min, float max)
{
    static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomFloat"));
    return fn(min, max);
}

inline void RandomSeed(int seed)
{
    static auto fn = (decltype(&RandomSeed))(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomSeed"));

    return fn(seed);
}
inline vec_t vector_lenght(const Vector& v)
{
    return (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
inline vec_t vector_normalize(Vector& v)
{
    vec_t l = v.Length();

    if (l != 0.0f)
    {
        v /= l;
    }
    else
    {
        v.x = v.y = 0.0f; v.z = 1.0f;
    }

    return l;
}
#define XM_2PI              6.283185307f

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

namespace math
{
    Vector angle_vector(QAngle meme);
    float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir);
    void normalize(Vector &vIn, Vector &vOut);
    void normalize_angles(QAngle& angles);
    void clamp_angles(QAngle& angles);
    bool sanitize_angles(QAngle &angles);
    void correct_movement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
    void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up);
    float get_distance(const Vector &start, const Vector &end);
    float get_fov(const QAngle &viewAngles, const QAngle &aimAngles);
    void compute_angle(const Vector &source, const Vector &destination, QAngle& angles);
    QAngle compute_angle(const Vector &source, const Vector &destination);
    void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
    void angle_vectors(const QAngle &angles, Vector& forward);
    void angle_vectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
    void vector_angles(const Vector& forward, QAngle& angles);
    bool world_to_screen(const Vector& in, Vector& out);
}