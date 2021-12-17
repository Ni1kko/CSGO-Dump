namespace SDK
{
	class IGameEvent;
}
#pragma once

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#ifndef RAD2DEG
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

namespace MATH
{
	void VectorAngles(const Vector& forward, Vector& angles);

	void SinCos(float radians, float* sine, float* cosine);
	void AngleVectors(const Vector& angles, Vector* forward = nullptr, Vector* right = nullptr, Vector* up = nullptr);

	float DotProduct(const float* a, const float* b);

	void VectorTransform(const float* in1, const matrix3x4_t& in2, float* out);
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);

	float CalcAngle2D(const Vector2D& src, const Vector2D& dst);
	void VectorAngle2D(const Vector2D& direction, float& angle);
	void AngleVectors2D(float angle, Vector2D& forward);

	inline float NormalizePitch(float pitch)
	{
		while (pitch > 89.f)
			pitch -= 180.f;
		while (pitch < -89.f)
			pitch += 180.f;

		return pitch;
	}

	inline float NormalizeYaw(float yaw)
	{
		if (yaw > 180)
			yaw -= (round(yaw / 360) * 360.f);
		else if (yaw < -180)
			yaw += (round(yaw / 360) * -360.f);

		return yaw;
	}

	inline float GetYawDelta(float yaw1, float yaw2)
	{
		return fabs(NormalizeYaw(yaw1 - yaw2));
	}

	inline float Lerp(float fraction, float min, float max)
	{
		return min + (fraction * (max - min));
	}

	inline Vector NormalizeAngle(Vector angle)
	{
		angle.x = NormalizePitch(angle.x);
		angle.y = NormalizeYaw(angle.y);

		if ((angle.z > 50) || (angle.z < 50))
			angle.z = 0;

		return angle;
	}

	inline Vector CalcAngle(const Vector& src, const Vector& dst)
	{
		Vector ret;
		VectorAngles(dst - src, ret);
		return ret;
	}
}
namespace ResolverOvermake
{
	struct resolver_info_t {
		float EyeAng;
		player_t* Entity;
		float Desync;
		float lastMaxDSC;
		float LBYT;
		float RealAng;
		float FakeAng;
	};
	class Resolver
	{
		resolver_info_t m_current;
		resolver_info_t m_previous;
	public:
		Resolver();

		void DoFSN(ClientFrameStage_t stage);
		void DoCM();

		void AnimationFix(player_t * pEnt);

		void HandleHits(player_t * pEnt);

		void record(player_t * entity, float new_yaw);

		bool is_slow_walking(player_t * entity);

		bool high_delta(player_t * player, AnimationLayer * layer);

		bool low_delta(player_t * player, AnimationLayer * layer);

		bool is_fakewalk(player_t * player, float speed, AnimationLayer * layer);

	public:
		/// resolve types, they're "flags" so to speak since a player can have more than 1 resolve type at once
		/// if angles overlap
		static const unsigned int RESOLVE_TYPE_NUM = 8;
		static const unsigned short RESOLVE_TYPE_OVERRIDE = 0b00000001,
			RESOLVE_TYPE_NO_FAKE = 0b00000010,
			RESOLVE_TYPE_LBY = 0b00000100,
			RESOLVE_TYPE_LBY_UPDATE = 0b00001000,
			RESOLVE_TYPE_PREDICTED_LBY_UPDATE = 0b00010000,
			RESOLVE_TYPE_LAST_MOVING_LBY = 0b00100000,
			RESOLVE_TYPE_NOT_BREAKING_LBY = 0b01000000,
			RESOLVE_TYPE_BRUTEFORCE = 0b10000000,
			RESOLVE_TYPE_NODESYNC = 0b100000000,
			RESOLVE_TYPE_ANTI_FREESTANDING = 0b1000000000,
			RESOLVE_TYPE_ANTI_SLOWWALK = 0b10000000000,
			RESOLVE_TYPE_DESYNC = 0b100000000000,
			RESOLVE_TYPE_DESYNC2 = 0b1000000000000,
			RESOLVE_TYPE_DESYNC3 = 0b10000000000000;
	public:
		/// a struct holding info the resolver needs, updated every frame for every player
		class PlayerResolveRecord
		{
		public:
			PlayerResolveRecord()
			{
				resolve_type = 0;
				shots_missed_moving_lby = 0;
				shots_missed_moving_lby_delta = 0;
				m_predicted_yaw_brute = 0.f;
				last_balance_adjust_trigger_time = 0.f;
				last_moving_lby_delta = 0.f;
				last_time_moving = 0.f;
				last_time_down_pitch = 0.f;
				next_predicted_lby_update = 0.f;

				has_fake = false;
				is_dormant = false, is_last_moving_lby_delta_valid = false;
				is_last_moving_lby_valid = false, is_fakewalking = false;
				is_balance_adjust_triggered = false, is_balance_adjust_playing = false;
				did_lby_flick = false, did_predicted_lby_flick = false;

				for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
				{
					shots_hit[i] = 0;
					shots_fired[i] = 0;
				}
			}

		public:
			struct AntiFreestandingRecord
			{
				int right_damage = 0, left_damage = 0;
				float right_fraction = 0.f, left_fraction = 0.f;
			};

		public:
			AnimationLayer anim_layers[15];
			AntiFreestandingRecord anti_freestanding_record;

			Vector resolved_angles, networked_angles;
			Vector velocity, origin;

			int shots_hit[RESOLVE_TYPE_NUM], shots_fired[RESOLVE_TYPE_NUM];
			int shots_missed_moving_lby, shots_missed_moving_lby_delta;
			unsigned short resolve_type;
			float lower_body_yaw;
			float last_moving_lby;
			float last_moving_lby_delta;
			float desync_max_delta;
			float last_balance_adjust_trigger_time;
			float last_time_moving;
			float last_time_down_pitch;
			float next_predicted_lby_update;
			float m_predicted_yaw_brute;
			bool is_dormant;
			bool is_last_moving_lby_valid;
			bool is_fakewalking;
			bool is_last_moving_lby_delta_valid;
			bool is_balance_adjust_triggered, is_balance_adjust_playing;
			bool did_lby_flick, did_predicted_lby_flick;
			bool has_fake;
		};

		/// a snapshot holding info about the moment you shot, used to count shots missed / hit
		struct ShotSnapshot
		{
			player_t* entity; /// person we shot at
			PlayerResolveRecord resolve_record; /// their resolve record when we shot

			float time; /// time when snapshot was created
			float first_processed_time; /// time when the shot was first processed
			bool was_shot_processed;
			int hitgroup_hit;
		};

		struct Desync
		{
			float MoveDesync = 58.f;
			float DuckDesyncStatic = 58.f;
			float DuckDesyncMove = 58.f;
			float StandingDesyncMax = 58.f;
			float network_latency();
			int NetworkedChockedTicks(player_t * ent);
		};

	private:
		PlayerResolveRecord player_resolve_records[64];
		std::vector<ShotSnapshot> shot_snapshots;
		std::vector<Vector> last_eye_positions;

	public:
		PlayerResolveRecord& GetPlayerResolveInfo(player_t* entity)
		{
			return player_resolve_records[entity->EntIndex()];
		}

		std::string TranslateResolveRecord(unsigned short resolve_type);
		Color GetResolveColor(unsigned short resolve_type);
		int GetResolveTypeIndex(unsigned short resolve_type);
		int GetShotsMissed(player_t* entity, unsigned short resolve_type);

		bool IsResolved(const unsigned short& resolve_type)
		{
			if (resolve_type & RESOLVE_TYPE_NO_FAKE ||
				resolve_type & RESOLVE_TYPE_LBY_UPDATE ||
				resolve_type & RESOLVE_TYPE_PREDICTED_LBY_UPDATE ||
				resolve_type & RESOLVE_TYPE_NODESYNC ||
				resolve_type & RESOLVE_TYPE_DESYNC
				)
				return true;

			return false;
		}

		bool IsFakewalking(player_t* entity)
		{
			return player_resolve_records[entity->EntIndex()].is_fakewalking;
		}

		bool IsMovingOnGround(player_t* entity)
		{
			return player_resolve_records[entity->EntIndex()].velocity.Length2D() > 0.5f && entity->m_fFlags() & FL_ONGROUND;
		}
		bool UseFreestandAngle[65];
		float FreestandAngle[65];
		Vector absOriginBackup;
		float pitchHit[65];
		bool IsYawSideways(player_t* entity, float yaw);
		bool IsFakingYaw(player_t* entity);

		void Update(player_t * pEnt);

		void data(player_t * entity);

		/// pushback a record onto the shot snapshot queue
		void AddShotSnapshot(player_t* entity, PlayerResolveRecord resolve_record);
		void max_eye_ang_delta();
		void EventCallback(IGameEvent* game_event);

	private:
		void missed_due_to_desync(IGameEvent * event);
		void ProcessSnapShots();

		void UpdateResolveRecord(player_t* entity);
		void ResolveDesync(int res, player_t * pEnt);
		void ResolveYaw(player_t* entity);
		void ResolvePitch(player_t* entity);

		void ResolveYawBruteforce(player_t* entity);
		float ResolveYawOverride(player_t* entity);
		void ResolveSlowwalk(player_t * ent);
		bool AntiFreestanding(player_t* entity, float& yaw);
		const float LBYDelta();
		const bool IsDifferent(float a, float b, float tolerance);
		bool HasStaticYawDifference(float tolerance);
	};

	extern Resolver resolver;
}