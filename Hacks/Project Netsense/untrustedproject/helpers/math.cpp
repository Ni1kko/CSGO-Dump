#include "Math.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include <algorithm>

namespace Math
{


    float AngleNormalize(float angle)
    {
        angle = fmodf(angle, 360.0f);
        if (angle > 180)
        {
            angle -= 360;
        }
        if (angle < -180)
        {
            angle += 360;
        }
        return angle;
    }


    float ApproachAngle(float target, float value, float speed)
    {
        target = (target * 182.04445f) * 0.0054931641f;
        value = (value * 182.04445f) * 0.0054931641f;

        float delta = target - value;

        // Speed is assumed to be positive
        if (speed < 0)
            speed = -speed;

        if (delta < -180.0f)
            delta += 360.0f;
        else if (delta > 180.0f)
            delta -= 360.0f;

        if (delta > speed)
            value += speed;
        else if (delta < -speed)
            value -= speed;
        else
            value = target;

        return value;
    }

    static constexpr auto forward_bounds = 450.f;
	//--------------------------------------------------------------------------------
	float VectorDistance(const Vector& v1, const Vector& v2)
	{
		return FASTSQRT(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
	}
	//--------------------------------------------------------------------------------
	QAngle CalcAngle(const Vector& src, const Vector& dst)
	{
		QAngle vAngle;
		Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
		double hyp = sqrt(delta.x*delta.x + delta.y*delta.y);
        static constexpr auto forward_bounds = 450.f;

		vAngle.pitch = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
		vAngle.yaw = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
		vAngle.roll = 0.0f;

		if (delta.x >= 0.0)
			vAngle.yaw += 180.0f;

		return vAngle;
	}
    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------
    float RandomFloat(float min, float max)
    {
        static auto ranFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomFloat"));
        if (ranFloat)
        {
            return ranFloat(min, max);
        }
        else
        {
            return 0.f;
        }
    }
	//--------------------------------------------------------------------------------
	float GetFOV(const QAngle& viewAngle, const QAngle& aimAngle)
	{
		Vector ang, aim;

		AngleVectors(viewAngle, aim);
		AngleVectors(aimAngle, ang);

		auto res = RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
		if (std::isnan(res))
			res = 0.f;
		return res;
	}
    //--------------------------------------------------------------------------------
    void ClampAngles(QAngle& angles)
    {
        if(angles.pitch > 89.0f) angles.pitch = 89.0f;
        else if(angles.pitch < -89.0f) angles.pitch = -89.0f;

        if(angles.yaw > 180.0f) angles.yaw = 180.0f;
        else if(angles.yaw < -180.0f) angles.yaw = -180.0f;

        angles.roll = 0;
    }
    //--------------------------------------------------------------------------------
    void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
    {
        out[0] = in1.Dot(in2[0]) + in2[0][3];
        out[1] = in1.Dot(in2[1]) + in2[1][3];
        out[2] = in1.Dot(in2[2]) + in2[2][3];
    }
    //--------------------------------------------------------------------------------
    void AngleVectors(const QAngle &angles, Vector& forward)
    {
        float	sp, sy, cp, cy;

        DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
        DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

        static constexpr auto forward_bounds = 450.f;

        forward.x = cp*cy;
        forward.y = cp*sy;
        forward.z = -sp;
    }
    //--------------------------------------------------------------------------------
    void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
    {
        float sr, sp, sy, cr, cp, cy;

        DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
        DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
        DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

        static constexpr auto forward_bounds = 450.f;

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
    //--------------------------------------------------------------------------------
    void VectorAngles(const Vector& forward, QAngle& angles)
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

        static constexpr auto forward_bounds = 450.f;
        angles[0] = pitch;
        angles[1] = yaw;
        angles[2] = 0;
    }
    //--------------------------------------------------------------------------------
    Vector CrossProduct(const Vector& a, const Vector& b)
    {
        return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }
    //--------------------------------------------------------------------------------
    void VectorAnglesHitchance(const Vector& forward, Vector& up, QAngle& angles)
    {
        Vector left = CrossProduct(up, forward);
        left.NormalizeInPlace();

        float forwardDist = forward.Length2D();

        if (forwardDist > 0.001f)
        {
            angles.pitch = atan2f(-forward.z, forwardDist) * 180 / M_PI;
            angles.yaw = atan2f(forward.y, forward.x) * 180 / M_PI;

            float upZ = (left.y * forward.x) - (left.x * forward.y);
            angles.roll = atan2f(left.z, upZ) * 180 / M_PI;
        }
        else
        {
            angles.pitch = atan2f(-forward.z, forwardDist) * 180 / M_PI;
            angles.yaw = atan2f(-left.x, left.y) * 180 / M_PI;
            angles.roll = 0;
        }
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
    bool WorldToScreen(const Vector& in, Vector& out)
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
    void FixAngles(QAngle& angles) 
    {
        Normalize3(angles);
        ClampAngles(angles);
       

    }
    //--------------------------------------------------------------------------------
    float Math::NormalizeYaw(float value)
    {
        while (value > 180)
            value -= 360.f;

        while (value < -180)
            value += 360.f;
        return value;
    }
    //--------------------------------------------------------------------------------
    void Math::RotateTriangle(std::array<ImVec2, 3>& points, float rotation)
    {
        //const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
        const auto points_center = ImVec2((points.at(0).x = points.at(1).x + points.at(2).x) / 3, (points.at(0).y = points.at(1).y + points.at(2).y) / 3);
        for (auto& point : points)
        {
            //point -= points_center;
            point.x -= points_center.x;
            point.y -= points_center.y;

            const auto temp_x = point.x;
            const auto temp_y = point.y;

            const auto theta = DEG2RAD(rotation);
            const auto c = cosf(theta);
            const auto s = sinf(theta);

            point.x = temp_x * c - temp_y * s;
            point.y = temp_x * s + temp_y * c;

            //point += points_center;
            point.x += points_center.x;
            point.y += points_center.y;
        }
    }
    //--------------------------------------------------------------------------------
    

    void MovementFix(CUserCmd* m_Cmd, QAngle wish_angle, QAngle old_angles)
    {
        if (old_angles.pitch != wish_angle.pitch || old_angles.yaw != wish_angle.yaw || old_angles.roll != wish_angle.roll) {
            auto viewangles = old_angles;

            auto movedata = Vector(m_Cmd->forwardmove, m_Cmd->sidemove, m_Cmd->upmove);
            viewangles.Normalize();

            if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND) && viewangles.roll != 0.f)
                movedata.y = 0.f;
           

            Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
            Math::AngleVectors(wish_angle, view_fwd, view_right, view_up);
            Math::AngleVectors(m_Cmd->viewangles, cmd_fwd, cmd_right, cmd_up);

            AngleVectors(wish_angle, view_fwd, view_right, view_up);
            AngleVectors(viewangles, cmd_fwd, cmd_right, cmd_up);

            static constexpr auto forward_bounds = 450.f;
            const auto v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
            const auto v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
            const auto v12 = sqrtf(view_up.z * view_up.z);

            const Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
            const Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
            const Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

            const auto v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
            const auto v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
            const auto v18 = sqrtf(cmd_up.z * cmd_up.z);

            const Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
            const Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
            const Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

            const auto v22 = norm_view_fwd.x * m_Cmd->forwardmove;
            const auto v26 = norm_view_fwd.y * m_Cmd->forwardmove;
            const auto v28 = norm_view_fwd.z * m_Cmd->forwardmove;
            const auto v24 = norm_view_right.x * m_Cmd->sidemove;
            const auto v23 = norm_view_right.y * m_Cmd->sidemove;
            const auto v25 = norm_view_right.z * m_Cmd->sidemove;
            const auto v30 = norm_view_up.x * m_Cmd->upmove;
            const auto v27 = norm_view_up.z * m_Cmd->upmove;
            const auto v29 = norm_view_up.y * m_Cmd->upmove;

            m_Cmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
                + (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
                + (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
            m_Cmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
                + (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
                + (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
            m_Cmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
                + (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
                + (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

            const auto ratio = 2.f - fmaxf(fabsf(m_Cmd->sidemove), fabsf(m_Cmd->forwardmove)) / Math::forward_bounds;
            m_Cmd->forwardmove *= ratio;
            m_Cmd->sidemove *= ratio;

            wish_angle = m_Cmd->viewangles;
        }
    }
    //--------------------------------------------------------------------------------
    float AngleDiff(float destAngle, float srcAngle) {
        float delta;

        delta = fmodf(destAngle - srcAngle, 360.0f);
        if (destAngle > srcAngle) {
            if (delta >= 180)
                delta -= 360;
        }
        else {
            if (delta <= -180)
                delta += 360;
        }
        return delta;
    }
    //--------------------------------------------------------------------------------
}
