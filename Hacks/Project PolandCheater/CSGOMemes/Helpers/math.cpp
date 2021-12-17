#include "math.hpp"

namespace math
{
    void normalize(Vector &vIn, Vector &vOut)
    {
        float flLen = vIn.Length();
        if (flLen == 0) {
            vOut.Init(0, 0, 1);
            return;
        }
        flLen = 1 / flLen;
        vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
    }
    void normalize_angles(QAngle& angles)
    {
        for(auto i = 0; i < 3; i++) {
            while(angles[i] < -180.0f) angles[i] += 360.0f;
            while(angles[i] >  180.0f) angles[i] -= 360.0f;
        }
    }
    //--------------------------------------------------------------------------------
    void clamp_angles(QAngle& angles)
    {
        if(angles.pitch > 89.0f) angles.pitch = 89.0f;
        else if(angles.pitch < -89.0f) angles.pitch = -89.0f;

        if(angles.yaw > 180.0f) angles.yaw = 180.0f;
        else if(angles.yaw < -180.0f) angles.yaw = -180.0f;

        angles.roll = 0;
    }
    //--------------------------------------------------------------------------------
    void correct_movement(QAngle vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
    {
        // side/forward move correction
        float deltaView;
        float f1;
        float f2;

        if (vOldAngles.yaw < 0.f)
            f1 = 360.0f + vOldAngles.yaw;
        else
            f1 = vOldAngles.yaw;

        if (pCmd->viewangles.yaw < 0.0f)
            f2 = 360.0f + pCmd->viewangles.yaw;
        else
            f2 = pCmd->viewangles.yaw;

        if (f2 < f1)
            deltaView = abs(f2 - f1);
        else
            deltaView = 360.0f - abs(f1 - f2);

        deltaView = 360.0f - deltaView;

        pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
        pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
    }
    //--------------------------------------------------------------------------------
    bool sanitize_angles(QAngle &angles)
    {
        QAngle temp = angles;
        normalize_angles(temp);
        clamp_angles(temp);

        if(!isfinite(temp.pitch) ||
            !isfinite(temp.yaw) ||
            !isfinite(temp.roll))
            return false;

        angles = temp;

        return true;
    }
    //--------------------------------------------------------------------------------
    float get_distance(const Vector &start, const Vector &end)
    {
        float distance = sqrt((start - end).Length());

        if (distance < 1.0f)
            distance = 1.0f;

        return distance;
    }
    float get_fov(const QAngle &viewAngles, const QAngle &aimAngles)
    {
        Vector ang, aim;
        angle_vectors(viewAngles, aim);
        angle_vectors(aimAngles, ang);

        return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
    }
    void compute_angle(const Vector &source, const Vector &destination, QAngle& angles)
    {
        Vector delta = source - destination;
        angles.pitch = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
        angles.yaw = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
        angles.roll = 0.0f;

        if (delta.x >= 0.0f)
            angles.yaw += 180.0f;
    }
    QAngle compute_angle(const Vector &source, const Vector &destination)
    {
        QAngle angles;

        Vector delta = source - destination;
        angles.pitch = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
        angles.yaw = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
        angles.roll = 0.0f;

        if (delta.x >= 0.0f)
            angles.yaw += 180.0f;

        return angles;
    }
    void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
    {
        out[0] = in1.Dot(in2[0]) + in2[0][3];
        out[1] = in1.Dot(in2[1]) + in2[1][3];
        out[2] = in1.Dot(in2[2]) + in2[2][3];
    }
    //--------------------------------------------------------------------------------
    void angle_vectors(const QAngle &angles, Vector& forward)
    {
        float	sp, sy, cp, cy;

        DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
        DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

        forward.x = cp*cy;
        forward.y = cp*sy;
        forward.z = -sp;
    }
    Vector angle_vector(QAngle meme)
    {
        auto sy = sin(meme.yaw / 180.f * static_cast<float>(DirectX::XM_PI));
        auto cy = cos(meme.yaw / 180.f * static_cast<float>(DirectX::XM_PI));

        auto sp = sin(meme.pitch / 180.f * static_cast<float>(DirectX::XM_PI));
        auto cp = cos(meme.pitch / 180.f* static_cast<float>(DirectX::XM_PI));

        return Vector(cp*cy, cp*sy, -sp);
    }
    float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
    {
        auto PointDir = Point - LineOrigin;

        auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
        if (TempOffset < 0.000001f)
            return FLT_MAX;

        auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

        return (Point - PerpendicularPoint).Length();
    }

    //--------------------------------------------------------------------------------
    void angle_vectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
    {
        float sr, sp, sy, cr, cp, cy;

        DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
        DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
        DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

        forward.x = (cp * cy);
        forward.y = (cp * sy);
        forward.z = (-sp);
        right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
        right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
        right.z = (-1 * sr * cp);
        up.x = (cr * sp * cy + -sr*-sy);
        up.y = (cr * sp * sy + -sr*cy);
        up.z = (cr * cp);
    }
    void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up)
    {
        float sr, sp, sy, cr, cp, cy;

        sp = static_cast<float>(sin(double(angles.x) * PIRAD));
        cp = static_cast<float>(cos(double(angles.x) * PIRAD));
        sy = static_cast<float>(sin(double(angles.y) * PIRAD));
        cy = static_cast<float>(cos(double(angles.y) * PIRAD));
        sr = static_cast<float>(sin(double(angles.z) * PIRAD));
        cr = static_cast<float>(cos(double(angles.z) * PIRAD));

        if (forward)
        {
            forward->x = cp*cy;
            forward->y = cp*sy;
            forward->z = -sp;
        }

        if (right)
        {
            right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
            right->y = (-1 * sr*sp*sy + -1 * cr*cy);
            right->z = -1 * sr*cp;
        }

        if (up)
        {
            up->x = (cr*sp*cy + -sr*-sy);
            up->y = (cr*sp*sy + -sr*cy);
            up->z = cr*cp;
        }
    }
    //--------------------------------------------------------------------------------
    void vector_angles(const Vector& forward, QAngle& angles)
    {
        float	tmp, yaw, pitch;

        if(forward[1] == 0 && forward[0] == 0) {
            yaw = 0;
            if(forward[2] > 0)
                pitch = 270;
            else
                pitch = 90;
        } else {
            yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
            if(yaw < 0)
                yaw += 360;

            tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
            pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
            if(pitch < 0)
                pitch += 360;
        }

        angles[0] = pitch;
        angles[1] = yaw;
        angles[2] = 0;
    }
    //--------------------------------------------------------------------------------
    static bool screen_transform(const Vector& in, Vector& out)
    {
        static auto& w2sMatrix = g_EngineClient->WorldToScreenMatrix();

        out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
        out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
        out.z = 0.0f;

        float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

        if(w < 0.001f) {
            out.x *= 100000;
            out.y *= 100000;
            return false;
        }

        out.x /= w;
        out.y /= w;

        return true;
    }
    //--------------------------------------------------------------------------------
    bool world_to_screen(const Vector& in, Vector& out)
    {
        if(screen_transform(in, out)) {
            int w, h;
            g_EngineClient->GetScreenSize(w, h);

            out.x = (w / 2.0f) + (out.x * w) / 2.0f;
            out.y = (h / 2.0f) - (out.y * h) / 2.0f;

            return true;
        }
        return false;
    }
    //--------------------------------------------------------------------------------
}