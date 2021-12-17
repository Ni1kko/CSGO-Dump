#include "math.hpp"

#define M_RADPI 57.295779513082f

namespace math {
	void normalize_angles(Vector& angles) {
		while (angles.y < -180) { angles.y += 360; }//
		while (angles.y > 180) { angles.y -= 360; }//				Removed here bc it dumped the console xd
		while (angles.x < -180) { angles.x += 360; }//
		while (angles.x > 180) { angles.x -= 360; }//

		if (angles.x > 89.0) { angles.x = 89; fridge::ConsolePrint("X-Angles extended 89 clamping... \n"); }
		if (angles.x < -89.0) { angles.x = -89; fridge::ConsolePrint("X-Angles extended -89 clamping... \n"); }
		if (angles.y < -180) { angles.y = -179.999; fridge::ConsolePrint("Y-Angles extended -180 clamping... \n"); }
		if (angles.y > 180) { angles.y = 179.999; fridge::ConsolePrint("Y-Angles extended 180 clamping... \n"); }

		angles.z = 0;
	}

	float angle_distance(float firstangle, float secondangle)
	{
		if (firstangle == secondangle)
			return 0.f;

		bool oppositeSides = false;

		if (firstangle > 0 && secondangle < 0)
			oppositeSides = true;
		else if (firstangle < 0 && secondangle > 0)
			oppositeSides = true;

		if (!oppositeSides)
			return fabs(firstangle - secondangle);

		bool past90 = false;

		if (firstangle > 90 && secondangle < -90)
		{
			firstangle -= (firstangle - 90);
			secondangle += (secondangle + 90);
		}
		else if (firstangle < -90 && secondangle > 90)
		{
			firstangle += (firstangle + 90);
			secondangle -= (secondangle - 90);
		}

		float oneTwo;

		oneTwo = fabs(firstangle - secondangle);

		return oneTwo;
	}

	float lowerbody_rotate(float lby, float yaw) {
		float delta = math::clamp_yaw || math::normalize_angle || math::normalize_yaw(yaw - lby);
		if (fabs(delta) < 25.f)
			return lby;

		if (delta > 0.f)
			return yaw + 25.f;

		return yaw;
	}

	void normalize_angle(QAngle& angles) {
		for (auto i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] > 180.0f) angles[i] -= 360.0f;
		}
	}

	void normalize_vector(vec3_t & vector) {
		for (auto i = 0; i < 3; i++) {
			while (vector[i] < -180.0f) vector[i] += 360.0f;
			while (vector[i] > 180.0f) vector[i] -= 360.0f;
		}

		vector[2] = 0.f;
	}

	//--------------------------------------------------------------------------------
	float normalize_yaw(float f) {
		if (std::isnan(f) || std::isinf(f))
			f = 0;

		if (f > 9999999)
			f = 0;

		if (f < -9999999)
			f = 0;

		while (f < -180.0f) f += 360.0f;
		while (f > 180.0f) f -= 360.0f;

		return f;
	}

	float normalize_pitch(float pitch) {
		while (pitch > 89.f)
			pitch -= 180.f;

		while (pitch < -89.f)
			pitch += 180.f;

		return pitch;
	}
	//--------------------------------------------------------------------------------
	float vector_normalize(Vector& v) {
		float l = v.Length();

		if (l != 0.0f)
			v /= l;
		else
			v.x = v.y = 0.0f; v.z = 1.0f;

		return l;
	}
	//--------------------------------------------------------------------------------
	void clamp_angles(Vector& angles) {
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}
	//--------------------------------------------------------------------------------
	void vector_transform(const Vector& in1, const matrix3x4_t& in2, Vector& out) {
		auto DotProduct = [](const Vector& a, const Vector& b) {
			return (a.x * b.x + a.y * b.y + a.z * b.z);
		};

		out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	}
	//--------------------------------------------------------------------------------
	void angle_vectors(const Vector &angles, Vector& forward) {
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	void angle_vectors(const QAngle &angles, Vector& forward) {
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	vec3_t angle_vectors(const vec3_t & angle) {
		float sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angle[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angle[1]));

		vec3_t forward;
		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		return forward;
	}
	//--------------------------------------------------------------------------------
	vec3_t calculate_angle(vec3_t src, vec3_t dst) {
		vec3_t angles;

		vec3_t delta = src - dst;
		float hyp = delta.Length2D();

		angles.y = std::atanf(delta.y / delta.x) * 57.2957795131f;
		angles.x = std::atanf(-delta.z / hyp) * -57.2957795131f;
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}
	//--------------------------------------------------------------------------------
	void AngleVectors(const Vector &angles, Vector *forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	float get_fov(vec3_t view_angle, vec3_t aim_angle)
	{
		/*CONST FLOAT MaxDegrees = 360.0f;
		Vector Delta(0, 0, 0);
		Vector Forward(0, 0, 0);
		vec3_t delta = aim_angle - view_angle;
		clamp_angles(delta);
		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));*/
		CONST FLOAT MaxDegrees = 360.0f;

		Vector Angles = view_angle;

		Vector Delta(0, 0, 0);

		Vector Forward(0, 0, 0);

		AngleVectors(Angles, &Forward);

		vec3_t delta = aim_angle - view_angle;

		clamp_angles(delta);

		FLOAT DotProduct = Forward.Dot(delta);
		return (acos(DotProduct) * (MaxDegrees / PI));
	}

	//--------------------------------------------------------------------------------
	void angle_vectors(const Vector &angles, Vector *forward, Vector *right, Vector* up) {
		auto sincos = [](float radians, float *sine, float *cosine) {
			*sine = sin(radians);
			*cosine = cos(radians);
		};

		float sr, sp, sy, cr, cp, cy;

		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);
		sincos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up) {
			up->x = (cr*sp*cy + -sr * -sy);
			up->y = (cr*sp*sy + -sr * cy);
			up->z = cr * cp;
		}
	}
	//--------------------------------------------------------------------------------
	void vector_angles(const Vector& forward, Vector& angles) {
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0) {
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
			if (pitch < 0)
				pitch += 360;
		}

		if (forward.x >= 0.0f) yaw += 180;

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	//--------------------------------------------------------------------------------
	void vector_angles(const Vector& forward, QAngle &angles)
	{
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0) {
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
			if (pitch < 0)
				pitch += 360;
		}

		if (forward.x >= 0.0f) yaw += 180;

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	//--------------------------------------------------------------------------------
	void vector_angles(const Vector& forward, Vector& up, Vector& angles) {
		Vector left = cross_product(up, forward);
		left.NormalizeInPlace();

		float forward_dist = forward.Length2D();

		if (forward_dist > 0.001f) {
			angles.x = atan2f(-forward.z, forward_dist) * 180 / DirectX::XM_PI;
			angles.y = atan2f(forward.y, forward.x) * 180 / DirectX::XM_PI;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / DirectX::XM_PI;
		}
		else {
			angles.x = atan2f(-forward.z, forward_dist) * 180 / DirectX::XM_PI;
			angles.y = atan2f(-left.x, left.y) * 180 / DirectX::XM_PI;
			angles.z = 0;
		}
	}

	//--------------------------------------------------------------------------------
	Vector cross_product(const Vector & a, const Vector & b) {
		Vector result;
		result.x = a.y*b.z - a.z*b.y;
		result.y = a.z*b.x - a.x*b.z;
		result.z = a.x*b.y - a.y*b.x;

		return  result;
	}
	//--------------------------------------------------------------------------------
	float dot_product(const Vector & a, const Vector & b) {
		return (a.x * b.x
			+ a.y * b.y
			+ a.z * b.z);
	}
	//--------------------------------------------------------------------------------
	bool screen_transform(const Vector& point, Vector& screen) {
		float w = g_ctx.m_globals.w2s_matrix[3][0] * point.x + g_ctx.m_globals.w2s_matrix[3][1] * point.y + g_ctx.m_globals.w2s_matrix[3][2] * point.z + g_ctx.m_globals.w2s_matrix[3][3];
		screen.x = g_ctx.m_globals.w2s_matrix[0][0] * point.x + g_ctx.m_globals.w2s_matrix[0][1] * point.y + g_ctx.m_globals.w2s_matrix[0][2] * point.z + g_ctx.m_globals.w2s_matrix[0][3];
		screen.y = g_ctx.m_globals.w2s_matrix[1][0] * point.x + g_ctx.m_globals.w2s_matrix[1][1] * point.y + g_ctx.m_globals.w2s_matrix[1][2] * point.z + g_ctx.m_globals.w2s_matrix[1][3];
		screen.z = 0.0f;

		int inverse_width = (w < 0.001f) ? -1.0f / w :
			1.0f / w;

		screen.x *= inverse_width;
		screen.y *= inverse_width;
		return (w < 0.001f);
	}

	bool world_to_screen(const Vector &origin, Vector &screen) {
		auto find_point = [](Vector &point, int screen_w, int screen_h, int degrees) -> void {
			float x2 = screen_w * 0.5f;
			float y2 = screen_h * 0.5f;

			float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
			float r = degrees / d; //Segment ratio

			point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
			point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
		};

		float w = g_ctx.m_globals.w2s_matrix[3][0] * origin.x + g_ctx.m_globals.w2s_matrix[3][1] * origin.y + g_ctx.m_globals.w2s_matrix[3][2] * origin.z + g_ctx.m_globals.w2s_matrix[3][3];

		auto
			screen_width = render::get().viewport().right,
			screen_height = render::get().viewport().bottom;

		float inverse_width = -1.0f / w;
		bool behind = true;

		if (w > 0.01) {
			inverse_width = 1.0f / w;
			behind = false;
		}

		screen.x = (float)((screen_width / 2) + (0.5 * ((g_ctx.m_globals.w2s_matrix[0][0] * origin.x
			+ g_ctx.m_globals.w2s_matrix[0][1] * origin.y
			+ g_ctx.m_globals.w2s_matrix[0][2] * origin.z
			+ g_ctx.m_globals.w2s_matrix[0][3]) * inverse_width) * screen_width + 0.5));

		screen.y = (float)((screen_height / 2) - (0.5 * ((g_ctx.m_globals.w2s_matrix[1][0] * origin.x
			+ g_ctx.m_globals.w2s_matrix[1][1] * origin.y
			+ g_ctx.m_globals.w2s_matrix[1][2] * origin.z
			+ g_ctx.m_globals.w2s_matrix[1][3]) * inverse_width) * screen_height + 0.5));

		if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
			find_point(screen, screen_width, screen_height, screen_height / 2);
			return false;
		}

		return !(behind);
	}
	//--------------------------------------------------------------------------------
	void rotate_movement(float yaw) {
		vec3_t viewangles;
		g_csgo.m_engine()->GetViewAngles(viewangles);

		float rotation = DEG2RAD(viewangles.y - yaw);

		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);

		float new_forwardmove = (cos_rot * g_ctx.get_command()->m_forwardmove) - (sin_rot * g_ctx.get_command()->m_sidemove);
		float new_sidemove = (sin_rot * g_ctx.get_command()->m_forwardmove) + (cos_rot * g_ctx.get_command()->m_sidemove);

		g_ctx.get_command()->m_forwardmove = new_forwardmove;
		g_ctx.get_command()->m_sidemove = new_sidemove;
	}
	//--------------------------------------------------------------------------------
	float random_float(float min, float max) {
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
		return m_RandomFloat(min, max);
	}
	//--------------------------------------------------------------------------------
	void random_seed(int seed) {
		typedef void(*RandomSeed_t)(int);
		static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
		return m_RandomSeed(seed);
	}
}